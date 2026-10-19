#!/usr/bin/env python3
"""Prepare a closed JS/QuickJS Wasm build; never execute a compiler or JS code.

This producer reads explicit immutable source/tool selections, stages the pinned
official engine with a small recorded WASI adaptation, embeds exact ESM bytes,
and writes serial compiler/link/validator argv. An owner executes those jobs
inside its cgroup/RSS/deadline/pidfd framework. PREPARED is not COMPILED or PASS.
No PATH lookup, package manager, host qjsc, source evaluator or shell is used.
"""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import re
import stat
import sys

HERE = Path(__file__).resolve().parent
TYPES = {'i32': ('int32_t', 'uwvm_js_checked_i32', 'JS_NewInt32'),
         'u32': ('uint32_t', 'uwvm_js_checked_u32', 'JS_NewUint32'),
         'i64': ('int64_t', 'uwvm_js_checked_i64', 'JS_NewBigInt64'),
         'u64': ('uint64_t', 'uwvm_js_checked_u64', 'JS_NewBigUint64'),
         'f32': ('float', 'uwvm_js_checked_f32', 'JS_NewFloat64'),
         'f64': ('double', 'uwvm_js_checked_f64', 'JS_NewFloat64')}
LIMITS = ('heap_bytes', 'stack_bytes', 'max_jobs', 'interrupt_polls', 'input_bytes',
          'output_bytes', 'print_bytes', 'max_text_bytes', 'max_modules', 'max_source_bytes')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def opened(path, maximum):
    """Open one bounded regular input without following a final symlink.

    Check the size before allocating any body buffer. The resolved-parent and
    post-open path comparison also bind the selected name to that descriptor;
    callers retain its complete physical/stat/hash pin, not only a pathname.
    """
    path = Path(path)
    require(type(maximum) is int and maximum > 0, 'explicit positive input byte limit')
    require(path.is_absolute() and path.resolve(strict=True) == path, 'explicit resolved input required')
    selected = path.stat(follow_symlinks=False)
    require(stat.S_ISREG(selected.st_mode) and 0 <= selected.st_size <= maximum, 'selected input is not a bounded regular file')
    # O_NONBLOCK prevents a raced FIFO/device from hanging before fstat can
    # reject it; it changes no regular-file read semantics.
    descriptor = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= maximum, 'regular input exceeds its pre-read bound')
        selected = path.stat(follow_symlinks=False)
        require((selected.st_dev, selected.st_ino) == (before.st_dev, before.st_ino), 'selected input identity changed at open')
        return descriptor, before
    except BaseException:
        os.close(descriptor)
        raise


def stable_pin(path, before, after, size, digest):
    """Bind final descriptor and pathname identities after a bounded read."""
    selected = Path(path).stat(follow_symlinks=False)
    fields = ('st_dev', 'st_ino', 'st_mode', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
    require(all(getattr(before, key) == getattr(after, key) == getattr(selected, key) for key in fields), 'input changed while reading')
    require(size == before.st_size, 'input length changed while reading')
    return {'path': str(path), 'bytes': size, 'sha256': digest, 'device': before.st_dev,
            'inode': before.st_ino, 'mode': before.st_mode, 'mtime_ns': before.st_mtime_ns,
            'ctime_ns': before.st_ctime_ns}


def read(path, maximum):
    """Read at most maximum+1 bytes only after a regular-file size admission."""
    descriptor, before = opened(path, maximum)
    with os.fdopen(descriptor, 'rb') as stream:
        data = stream.read(maximum + 1)
        require(len(data) <= maximum, 'input grew beyond its body allocation bound')
        after = os.fstat(stream.fileno())
    return data, stable_pin(path, before, after, len(data), hashlib.sha256(data).hexdigest())


def canonical(name):
    return isinstance(name, str) and name not in ('std', 'os') and not name.endswith('.so') and len(name) <= 1024 and not name.startswith(('/', '.')) and all(
        part not in ('', '.', '..') and re.fullmatch(r'[A-Za-z0-9_.-]+', part) for part in name.split('/'))


def literal(data):
    # Keep at most1024 tiny integer strings live; a multi-MiB source must not
    # allocate a Python object for every byte simultaneously.
    return '\n'.join(','.join(str(value) for value in data[start:start+1024]) + ','
                     for start in range(0, len(data), 1024)) + '0'


def pin_file(path, maximum=1073741824):
    """Hash bounded compiler/archive bytes with a1-MiB read buffer."""
    descriptor, before = opened(path, maximum)
    digest = hashlib.sha256(); size = 0
    with os.fdopen(descriptor, 'rb') as stream:
        while block := stream.read(1048576):
            digest.update(block); size += len(block)
            require(size <= maximum, 'input grew beyond its streamed byte limit')
        after = os.fstat(stream.fileno())
    return stable_pin(path, before, after, size, digest.hexdigest())


def failure_literal(kind, value):
    """Require an explicit finite scalar fallback; valid results may be NaN/Inf.

    Floating failure values are finite C hexadecimal literals. They are not
    mistaken for a successful result: the engine's separate status is poisoned
    on a JS conversion/exception failure. Signed minimum literals avoid an
    out-of-range positive signed C token before unary minus is applied.
    """
    if kind in ('f32', 'f64'):
        require(type(value) in (int, float), 'numeric finite floating failure required')
        limit = float.fromhex('0x1.fffffep+127') if kind == 'f32' else float.fromhex('0x1.fffffffffffffp+1023')
        require(-limit <= value <= limit, 'finite representable floating failure required')
        number = float(value)
        require(math.isfinite(number), 'finite floating failure required')
        return '(' + TYPES[kind][0] + ')(' + number.hex() + ('f)' if kind == 'f32' else ')')
    bits = 32 if kind.endswith('32') else 64
    low, high = (0, 2**bits-1) if kind.startswith('u') else (-2**(bits-1), 2**(bits-1)-1)
    require(type(value) is int and low <= value <= high, 'exact scalar failure value')
    macro = ('UINT' if kind.startswith('u') else 'INT') + str(bits) + '_C'
    token = ('(-' + macro + '(' + str(-(value+1)) + ')-1)') if value == low and low < 0 else (
        '-' + macro + '(' + str(-value) + ')' if value < 0 else macro + '(' + str(value) + ')')
    return '(' + TYPES[kind][0] + ')(' + token + ')'


def expand_recipe(document, variant):
    """Expand one explicit recipe ID without evaluating source or permissions."""
    if document.get('schema') != 'uwvm-javascript-guest-recipes-v1':
        require(variant is None, 'variant supplied for an already expanded manifest')
        return document
    require(set(document) == {'schema', 'common', 'variants'}, 'closed recipe document')
    require(isinstance(variant, str) and variant in document['variants'], 'explicit known variant required')
    require(set(document['variants'][variant]) <= {'mode', 'backend', 'entry', 'modules', 'imports', 'exports', 'limits', 'initial_memory', 'maximum_memory'},
            'closed variant fields')
    return {**document['common'], **document['variants'][variant]}


def module_table(modules, entry):
    """Emit the literal module_table.c product; no JS parser/evaluator runs."""
    text = ['/* Exact source bytes from the selected closed manifest. */', '#include "engine.h"']
    for index, (_, data) in enumerate(modules):
        text.append('static unsigned char const source_' + str(index) + '[]={' + literal(data) + '};')
    text.append('struct uwvm_js_source const uwvm_js_sources[]={')
    for index, (name, data) in enumerate(modules):
        text.append('{' + json.dumps(name) + ',source_' + str(index) + ',' + str(len(data)) + '},')
    text += ['};', 'size_t const uwvm_js_source_count=sizeof uwvm_js_sources/sizeof *uwvm_js_sources;',
             'char const uwvm_js_main_module[]=' + json.dumps(entry) + ';',
             'char const uwvm_js_bootstrap_module[]="sdk/javascript/guest/bootstrap.mjs";']
    return '\n'.join(text) + '\n'


def scalar_trampolines(exports, imports):
    """Emit scalar_trampolines.c with fixed real Wasm import/export signatures.

    Each configured export supplies an explicit failure scalar and can select
    initialization on its first admitted call. A JS exception poisons status;
    no successful-looking scalar silently clears that error. Pointers/objects
    are never transported between modules.
    """
    text = ['/* Generated exact scalar Wasm boundary; inspect its binary. */', '#include "engine.h"']
    names = set()
    for index, row in enumerate(imports):
        require(set(row) == {'module', 'name', 'parameters', 'result'}, 'closed scalar import fields required')
        require(canonical(row['module']) and re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', row['name']), 'scalar import name')
        require(row['module'] != 'uwvm_ros2_rmw_srv_v1', 'reserved owned-RMW namespace requires dedicated checked bridge')
        params = row['parameters']; result = row['result']
        require(isinstance(params, list) and len(params) <= 7 and all(item in TYPES for item in params) and result in TYPES,
                'scalar import types')
        signature = ','.join(TYPES[item][0] for item in params) or 'void'
        text.append('__attribute__((import_module(' + json.dumps(row['module']) + '),import_name(' + json.dumps(row['name']) + ')))')
        text.append('extern ' + TYPES[result][0] + ' imported_' + str(index) + '(' + signature + ');')
        text.append('static JSValue native_' + str(index) + '(JSContext *ctx,JSValueConst self,int argc,JSValueConst *argv){(void)self;')
        text.append('if(argc!=' + str(len(params)) + ')return JS_ThrowTypeError(ctx,"Scalar import arity");')
        for argument, kind in enumerate(params):
            text.append(TYPES[kind][0] + ' a' + str(argument) + ';if(' + TYPES[kind][1] + '(ctx,argv[' + str(argument) + '],&a' + str(argument) + ')<0)return JS_EXCEPTION;')
        text.append('return ' + TYPES[result][2] + '(ctx,imported_' + str(index) + '(' + ','.join('a' + str(n) for n in range(len(params))) + '));}')
    text.append('static int init_imports(JSContext *ctx,JSModuleDef *module){')
    for index, row in enumerate(imports):
        require(row['name'] not in names, 'duplicate JS scalar import field')
        names.add(row['name'])
        text.append('if(JS_SetModuleExport(ctx,module,' + json.dumps(row['name']) + ',JS_NewCFunction(ctx,native_' + str(index) + ',' + json.dumps(row['name']) + ',' + str(len(row['parameters'])) + '))<0)return -1;')
    text += ['return 0;}', 'JSModuleDef *uwvm_js_register_library_imports(JSContext *ctx,char const *name){',
             'JSModuleDef *module=JS_NewCModule(ctx,name,init_imports);if(!module)return NULL;']
    for row in imports:
        text.append('if(JS_AddModuleExport(ctx,module,' + json.dumps(row['name']) + ')<0)return NULL;')
    text.append('return module;}')
    names = set()
    for index, row in enumerate(exports):
        require(set(row) == {'name', 'function', 'parameters', 'result', 'failure', 'initialize'}, 'closed scalar export fields required')
        require(re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', row['name']) and re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*', row['function']), 'scalar export name')
        require(row['name'] not in names and not row['name'].startswith(('roswasm_js_', '_start')), 'reserved/duplicate scalar export')
        names.add(row['name'])
        params = row['parameters']; result = row['result']
        require(isinstance(params, list) and len(params) <= 7 and all(kind in TYPES for kind in params) and result in TYPES and type(row['initialize']) is bool, 'scalar export types')
        failure = failure_literal(result, row['failure'])
        signature = ','.join(TYPES[kind][0] + ' a' + str(n) for n, kind in enumerate(params)) or 'void'
        text.append('__attribute__((export_name(' + json.dumps(row['name']) + '))) ' + TYPES[result][0] + ' exported_' + str(index) + '(' + signature + '){')
        if row['initialize']:
            text.append('if(roswasm_js_init()!=UWVM_JS_OK)return ' + failure + ';')
        text.append('if(roswasm_js_status()!=UWVM_JS_OK)return ' + failure + ';')
        text.append('JSValue args[' + str(max(1, len(params))) + '];')
        for n, kind in enumerate(params):
            text.append('args[' + str(n) + ']=' + TYPES[kind][2] + '(uwvm_js_context,a' + str(n) + ');')
            text.append('if(JS_IsException(args[' + str(n) + '])){')
            for old in range(n):
                text.append('JS_FreeValue(uwvm_js_context,args[' + str(old) + ']);')
            text.append('uwvm_js_fail(UWVM_JS_LIMIT);return ' + failure + ';}')
        text.append('JSValue value=uwvm_js_call_export(' + json.dumps(row['function']) + ',' + str(len(params)) + ',args);')
        for n in range(len(params)):
            text.append('JS_FreeValue(uwvm_js_context,args[' + str(n) + ']);')
        text.append(TYPES[result][0] + ' result;int good=JS_IsException(value)?-1:' + TYPES[result][1] + '(uwvm_js_context,value,&result);JS_FreeValue(uwvm_js_context,value);')
        text.append('if(good<0){uwvm_js_fail(UWVM_JS_EXCEPTION);return ' + failure + ';}return result;}')
    return '\n'.join(text) + '\n'


def patch_engine(text, seed):
    """Adapt only the exact hash-checked release; every source match is unique."""
    changes = [('#if !defined(__EMSCRIPTEN__)\n#define CONFIG_ATOMICS\n#endif',
                '#if !defined(__EMSCRIPTEN__) && !defined(UWVM_JS_SANDBOX)\n#define CONFIG_ATOMICS\n#endif'),
               ('#elif defined(__EMSCRIPTEN__)\n    return 0;',
                '#elif defined(__EMSCRIPTEN__) || defined(UWVM_JS_SANDBOX)\n    return 0;')]
    old = '''static void js_random_init(JSContext *ctx)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ctx->random_state = ((int64_t)tv.tv_sec * 1000000) + tv.tv_usec;
    /* the state must be non zero */
    if (ctx->random_state == 0)
        ctx->random_state = 1;
}'''
    changes.append((old, '/* Explicit pure deterministic PRNG seed; no entropy/time grant. */\nstatic void js_random_init(JSContext *ctx)\n{ ctx->random_state = UINT64_C(' + str(seed) + '); }'))
    # The pinned C API returns initial ta->length for a length-tracking RAB,
    # whereas actual JS byteLength follows the current element count. No JS
    # property getter may supply a native byte extent. The preceding upstream
    # typed_array_is_oob check rejects detached/OOB views; the current uint32
    # element count times its actual size is bounded by the real buffer extent.
    changes.append(('*pbyte_length = ta->length;',
                    '*pbyte_length = (size_t)p->u.array.count << typed_array_size_log2(p->class_id);'))
    for old, new in changes:
        require(text.count(old) == 1, 'pinned release patch match changed')
        text = text.replace(old, new)
    return text


def patch_dtoa(text):
    """Remove an unused setjmp include, not a setjmp implementation.

    This exact release contains no setjmp/longjmp/jmp_buf use. WASI toolchains
    may deliberately reject that header because nonlocal jumps need a separate
    ABI. Removing the unused include adds neither an exception ABI nor a stub.
    """
    require(text.count('#include <setjmp.h>') == 1, 'release dtoa include changed')
    result = text.replace('#include <setjmp.h>', '/* Unused setjmp header omitted for the Wasm guest build. */')
    require(not re.search(r'\b(?:setjmp|longjmp|jmp_buf)\s*[\(;]', result), 'release now uses nonlocal jumps')
    return result


RMW_MODULE = 'uwvm_ros2_rmw_srv_v1'
RMW_SIGNATURES = [
    ('abi_version', []),
    ('client_activate', ['i64', 'i64', 'i32', 'i64']),
    ('service_activate', ['i64', 'i64', 'i32', 'i64']),
    ('endpoint_inspect', ['i64', 'i64', 'i32', 'i64']),
    ('client_submit', ['i64', 'i64', 'i32', 'i64', 'i64', 'i64']),
    ('client_abandon', ['i64', 'i64', 'i64']),
    ('service_respond', ['i64', 'i64', 'i64', 'i64', 'i32', 'i64', 'i64']),
    ('endpoint_peek', ['i64', 'i64', 'i32', 'i32', 'i64', 'i64']),
    ('endpoint_commit', ['i64', 'i64', 'i64', 'i64']),
    ('endpoint_cancel', ['i64', 'i64', 'i64', 'i64']),
    ('client_available', ['i64', 'i64', 'i64', 'i64', 'i32', 'i64']),
    ('endpoint_retire', ['i64', 'i64'])]
RMW_OUTPUT = {
    'activation_inspection_bytes': 256, 'submit_ticket_bytes': 8,
    'availability_bytes': 1,
    'peek': 'one complete UMRC160+URMI192+canonical body checked copy, no separate length write',
    'minimum_peek_capacity': 352, 'maximum_peek_capacity': 4194304,
    'nonzero_error_preserves_output': True, 'ack_abandon_retire_no_output': True}
LEGACY_CATALOG_SHA256 = '0a13f6c49100173c156eb76acfd35ebe103a40b2a90c752d3d1302c31f875ec0'


def rmw_catalog_contract(document):
    """Validate the complete optional12 numeric/output contract as DATA only."""
    fields = {'schema_version', 'extension_version', 'kind', 'source_status',
              'authority_source', 'implementation_source', 'raw_header',
              'original_business_modules_unchanged', 'authority_contract', 'modules',
              'output_contract', 'quota_contract', 'pair_history',
              'transaction_contract', 'legacy_preservation', 'actual_compilers_models_Guest_native_ROS'}
    require(type(document) is dict and set(document) == fields, 'closed owned-RMW service catalog')
    require(type(document['schema_version']) is int and document['schema_version'] == 1 and
            type(document['extension_version']) is int and document['extension_version'] == 1 and
            document['kind'] == 'optional-owned-rmw-service-v1' and
            document['source_status'] == 'SOURCE_ONLY_HELD_UNEXECUTED' and
            document['original_business_modules_unchanged'] is True and
            type(document['actual_compilers_models_Guest_native_ROS']) is int and
            document['actual_compilers_models_Guest_native_ROS'] == 0, 'owned-RMW source-only catalog version')
    paths = {'authority_source': 'src/uwvm2/uwvm/imported/ros/host.h',
             'implementation_source': 'src/uwvm2/uwvm/imported/ros/rmw_service_host.h',
             'raw_header': 'sdk/include/roswasm/rmw_service_v1.h'}
    for key, path in paths.items():
        pin = document[key]
        require(type(pin) is dict and set(pin) == {'path', 'sha256'} and pin['path'] == path and
                isinstance(pin['sha256'], str) and re.fullmatch('[0-9a-f]{64}', pin['sha256']), 'fixed owned-RMW source binding')
    functions = [{'name': name, 'params': params, 'results': ['i32']} for name, params in RMW_SIGNATURES]
    require(document['modules'] == {RMW_MODULE: {'backend': 2, 'function_count': 12, 'functions': functions}},
            'complete ordered12 exact owned-RMW signatures required')
    require(type(document['modules'][RMW_MODULE]['backend']) is int and
            type(document['modules'][RMW_MODULE]['function_count']) is int, 'exact numeric catalog fields')
    require(document['output_contract'] == RMW_OUTPUT and
            all(type(document['output_contract'][k]) is int for k in
                ('activation_inspection_bytes', 'submit_ticket_bytes', 'availability_bytes', 'minimum_peek_capacity', 'maximum_peek_capacity')) and
            document['output_contract']['nonzero_error_preserves_output'] is True and
            document['output_contract']['ack_abandon_retire_no_output'] is True, 'exact owned-RMW output contract')
    require(document['legacy_preservation'] == {'catalog_path': 'sdk/abi/abi-v1.json',
            'catalog_sha256': LEGACY_CATALOG_SHA256}, 'unchanged original business81 catalog required')
    require(isinstance(document['authority_contract'], str) and document['authority_contract'] and
            isinstance(document['pair_history'], str) and document['pair_history'] and
            type(document['quota_contract']) is dict and type(document['transaction_contract']) is dict,
            'catalog authority/lifetime scope required')
    return functions


def full_import_contract(manifest, original, optional, modular_motion=False):
    """Build a closed multiset of actual numeric imports, never a module allowance.

    Optional activation is decided by the explicit CLI, not manifest modules or
    generic scalar declarations. Keep business81/client extension files unchanged.
    """
    rows = []
    def add(module, name, params, results, role):
        require(isinstance(module, str) and isinstance(name, str) and module and name and
                type(params) is list and type(results) is list and
                all(t in ('i32', 'i64', 'f32', 'f64') for t in params + results), 'closed numeric import row')
        require(not any(r['module'] == module and r['name'] == name for r in rows), 'duplicate expected module/name')
        rows.append({'module': module, 'name': name, 'kind': 0, 'params': list(params), 'results': list(results), 'role': role})
    backend = manifest['backend']
    if backend:
        module = 'uwvm_ros' + str(backend) + '_v1'
        require(type(original) is dict and set(original['modules']) == {'uwvm_ros1_v1', 'uwvm_ros2_v1'}, 'original business modules')
        for number in (1, 2):
            block = original['modules']['uwvm_ros' + str(number) + '_v1']
            require(type(block['backend']) is int and block['backend'] == number and
                    type(block['function_count']) is int and block['function_count'] == (38 if number == 1 else 43) and
                    len(block['functions']) == block['function_count'], 'complete unchanged business inventory')
        for row in original['modules'][module]['functions']:
            add(module, row['name'], [p['type'] for p in row['params']], row['results'], 'business')
    if optional is not None:
        require(backend == 2, 'owned-RMW optional bridge is ROS2 only')
        for row in rmw_catalog_contract(optional):
            add(RMW_MODULE, row['name'], row['params'], row['results'], 'owned-rmw-service')
    if backend:
        for name in ('fd_read', 'fd_write'):
            add('wasi_snapshot_preview1', name, ['i32'] * 4, ['i32'], 'basic-wasi')
    if manifest['mode'] == 'command':
        add('wasi_snapshot_preview1', 'proc_exit', ['i32'], [], 'basic-wasi')
    for row in manifest['imports']:
        require(row['module'] != RMW_MODULE, 'reserved owned-RMW namespace requires checked dedicated bridge')
        add(row['module'], row['name'], [t.replace('u', 'i', 1) if t.startswith('u') else t for t in row['parameters']],
            [row['result'].replace('u', 'i', 1) if row['result'].startswith('u') else row['result']], 'scalar-library')
    if modular_motion:
        for module, name, count in [('motion', 'motion_begin', 2), ('motion', 'motion_status', 0),
                ('motion', 'motion_word', 3), ('motion', 'motion_end', 1),
                ('perception', 'perception_begin', 1), ('perception', 'perception_status', 0),
                ('perception', 'perception_word', 3), ('perception', 'perception_end', 2), ('perception', 'perception_abort', 1)]:
            add(module, name, ['i32'] * count, ['i32'], 'unitree-pure-scalar')
    return rows



def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source-root', 'quickjs', 'manifest', 'out', 'clang', 'linker', 'sysroot', 'resource-dir', 'libc', 'libm', 'printscan', 'builtins', 'validator'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--variant', help='exact key in a prepared recipes.json; no implicit variant')
    parser.add_argument('--unitree-direct-enabled', type=int, choices=(0, 1),
        help='explicit simulator-only Go2 command C-kernel selection;0 remains default-denied')
    parser.add_argument('--unitree-modular-motion', type=int, choices=(0, 1), default=None,
        help='explicit positive simulator graph: pure perception/motion/callback Wasm imports; no added authority')
    parser.add_argument('--rmw-service-enabled', type=int, choices=(0, 1), default=0,
        help='explicit optional owned-RMW copied bridge; default0, ROS2 only')
    parser.add_argument('--rmw-service-catalog-sha256',
        help='required exact raw sdk/abi/rmw-service-v1.json SHA256 only when enabled1')
    args = parser.parse_args()
    for key, path in vars(args).items():
        if key in ('variant', 'unitree_direct_enabled', 'unitree_modular_motion',
                   'rmw_service_enabled', 'rmw_service_catalog_sha256'): continue
        require(path.is_absolute(), 'every selected path must be absolute')
        if key not in ('out', 'linker'): setattr(args, key, path.resolve(strict=True))
    # wasm-ld may be a multicall alias for lld. Preserve its selected basename
    # in -fuse-ld while separately pinning the resolved physical executable.
    require(args.linker.parent.resolve(strict=True) == args.linker.parent and
            re.fullmatch(r'wasm-ld(?:-[0-9.]+)?', args.linker.name), 'explicit wasm-ld alias path required')
    linker_physical = args.linker.resolve(strict=True)
    root, upstream = args.source_root.resolve(strict=True), args.quickjs.resolve(strict=True)
    require(root != Path('/') and upstream != Path('/'), 'root source input refused')
    require(HERE == root / 'sdk/javascript/guest', 'execute the producer in the exact selected source root')
    out = args.out
    require(not out.exists() and out.parent.resolve(strict=True) == out.parent and out.name not in ('', '.', '..'), 'fresh output below a resolved parent required')
    for original in (root, upstream, args.sysroot, args.resource_dir):
        resolved = original.resolve(strict=True)
        require(not out.is_relative_to(resolved) and not resolved.is_relative_to(out), 'output overlaps frozen input')
    manifest_data, manifest_pin = read(args.manifest.resolve(strict=True), 1048576)
    manifest = expand_recipe(json.loads(manifest_data), args.variant)
    require(set(manifest) == {'schema', 'mode', 'backend', 'entry', 'modules', 'imports', 'exports', 'limits', 'random_seed', 'optimization', 'initial_memory', 'maximum_memory'}, 'closed guest manifest fields required')
    require(manifest['schema'] == 'uwvm-embedded-javascript-build-v1' and manifest['mode'] in ('command', 'reactor') and type(manifest['backend']) is int and manifest['backend'] in (0, 1, 2), 'guest mode/backend')
    require(manifest['mode'] != 'command' or manifest['backend'] in (1, 2), 'command uses explicit ROS/basic-stdio flavor')
    require(args.rmw_service_enabled == 0 and args.rmw_service_catalog_sha256 is None or
            args.rmw_service_enabled == 1 and manifest['backend'] == 2 and
            isinstance(args.rmw_service_catalog_sha256, str) and
            re.fullmatch('[0-9a-f]{64}', args.rmw_service_catalog_sha256),
            'owned-RMW requires explicit ROS2 enabled1 plus exact catalog SHA; disabled0 takes no catalog')
    if args.unitree_direct_enabled is not None:
        require(manifest['mode'] == 'command' and manifest['backend'] == 2 and
                manifest['entry'] == 'sdk/robots/unitree/examples/javascript/direct_go2.mjs' and
                manifest['imports'] == [] and manifest['exports'] == [],
                'local Unitree kernel is restricted to the exact standalone Go2 ROS2 command')
    require(not args.unitree_modular_motion or args.unitree_direct_enabled == 1,
            'pure modular imports require the explicit positive Unitree simulator selection')
    require(type(manifest['random_seed']) is int and 1 <= manifest['random_seed'] <= 2**64-1, 'explicit nonzero deterministic PRNG seed')
    require(manifest['optimization'] in ('-O2', '-O3'), 'explicit reviewed optimization')
    limits = manifest['limits']
    require(set(limits) == set(LIMITS) and all(type(value) is int and value > 0 for value in limits.values()), 'complete finite numeric limits')
    require(32 <= limits['output_bytes'] <= 4193786 and limits['input_bytes'] <= 4193786 and limits['max_text_bytes'] <= 4193786 and limits['print_bytes'] <= 4193786, 'wire scratch/print bounds')
    require(not args.rmw_service_enabled or limits['output_bytes'] >= 352, 'owned-RMW requires output scratch at least352')
    require(limits['max_jobs'] <= 65536 and limits['interrupt_polls'] <= 10000000 and limits['max_modules'] <= 1024 and limits['max_source_bytes'] <= 16777216, 'engine resource upper bounds')
    require(65536 <= limits['stack_bytes'] <= 4194304 and limits['stack_bytes'] < limits['heap_bytes'], 'stack/heap bounds')
    initial, maximum = manifest['initial_memory'], manifest['maximum_memory']
    require(type(initial) is int and type(maximum) is int and 0 < initial <= maximum <= 268435456 and initial % 65536 == maximum % 65536 == 0 and limits['heap_bytes'] < maximum, 'explicit bounded Wasm pages')
    require(canonical(manifest['entry']), 'canonical embedded main ID')
    require(isinstance(manifest['modules'], list) and 1 <= len(manifest['modules']) <= limits['max_modules'], 'closed module count before reading bodies')
    modules, inputs, names, source_bytes = [], [manifest_pin], set(), 0
    for row in manifest['modules']:
        require(set(row) == {'name', 'source'} and canonical(row['name']) and canonical(row['source']) and row['name'] not in names, 'unique closed module entry')
        names.add(row['name'])
        path = (root / row['source']).resolve(strict=True)
        require(path.is_relative_to(root), 'module source escaped explicit frozen source root')
        remaining = limits['max_source_bytes'] - source_bytes
        require(remaining > 0, 'total source byte bound exhausted before read')
        data, pin = read(path, min(8388608, remaining))
        data.decode('utf-8')
        require(b'\x00' not in data, 'module source contains NUL')
        modules.append((row['name'], data)); inputs.append(pin); source_bytes += len(data)
    require(manifest['entry'] in names and 'sdk/javascript/guest/bootstrap.mjs' in names, 'main/bootstrap absent from closed map')
    require(len(modules) <= limits['max_modules'] and sum(len(data) for _, data in modules) <= limits['max_source_bytes'], 'closed source allocation bound')
    lock_data, lock_pin = read(HERE / 'upstream-lock.json', 1048576)
    lock = json.loads(lock_data); inputs.append(lock_pin)
    require(lock['archive_sha256'] == 'b376e839b322978313d929fd20663b11ba58b75df5a46c126dd19ea2fa70ad2a', 'exact official engine release lock')
    engine_files = []
    for row in lock['files']:
        path = (upstream / row['path']).resolve(strict=True)
        require(path.is_relative_to(upstream), 'engine source escaped selected root')
        require(type(row['bytes']) is int and 0 < row['bytes'] <= 8388608, 'bounded official source size')
        data, pin = read(path, row['bytes'])
        require(pin['sha256'] == row['sha256'] and pin['bytes'] == row['bytes'], 'official engine file differs')
        engine_files.append((row['path'], data)); inputs.append(pin)
    tools = {}
    require(args.printscan == args.sysroot / 'lib/wasm32-wasip1/libc-printscan-long-double.a',
            'explicit matching nonthread WASIp1 full numeric formatting archive required')
    for name in ('clang', 'libc', 'libm', 'printscan', 'builtins', 'validator'):
        tools[name] = pin_file(getattr(args, name))
    tools['linker'] = {'alias': str(args.linker), 'physical': pin_file(linker_physical)}
    # Validate every source/manifest/type selection before creating owned output.
    table = module_table(modules, manifest['entry'])
    trampolines = scalar_trampolines(manifest['exports'], manifest['imports'])
    rmw_plan = None
    if args.rmw_service_enabled:
        catalog_raw, catalog_pin = read(root / 'sdk/abi/rmw-service-v1.json', 1048576)
        require(catalog_pin['sha256'] == args.rmw_service_catalog_sha256, 'raw optional catalog SHA differs')
        optional = json.loads(catalog_raw)
        rmw_catalog_contract(optional)
        original_raw, original_pin = read(root / 'sdk/abi/abi-v1.json', 1048576)
        require(original_pin['sha256'] == LEGACY_CATALOG_SHA256, 'original business81 catalog changed')
        original = json.loads(original_raw)
        inputs += [catalog_pin, original_pin]
        paths = {'producer': 'sdk/javascript/guest/prepare.py',
                 'checker': 'sdk/tools/check_javascript_guest_imports.py',
                 'inspector': 'sdk/tools/check_wasm_imports.py',
                 'engine_header': 'sdk/javascript/guest/engine.h',
                 'engine_source': 'sdk/javascript/guest/engine.c',
                 'native_bridge': 'sdk/javascript/guest/native_rmw_service.c'}
        paths.update({key: optional[key]['path'] for key in ('authority_source', 'implementation_source', 'raw_header')})
        if args.unitree_modular_motion:
            paths.update(unitree_motion='sdk/examples/robotics_modular/unitree_motion_abi.h',
                         unitree_perception='sdk/examples/robotics_modular/unitree_perception_abi.h')
        bindings = {'catalog': catalog_pin, 'legacy_catalog': original_pin}
        for key, relative in paths.items():
            _data, pin = read(root / relative, 2097152)
            if key in ('authority_source', 'implementation_source', 'raw_header'):
                require(pin['sha256'] == optional[key]['sha256'], 'catalog does not bind actual ' + key)
            bindings[key] = pin
            inputs.append(pin)
        python_path = Path(sys.executable)
        require(python_path.is_absolute(), 'owner-selected producer Python path must be absolute')
        python_path = python_path.resolve(strict=True)
        tools['binary_checker_python'] = pin_file(python_path)
        rmw_plan = {'schema': 'uwvm-javascript-owned-rmw-full-imports-v1',
                    'backend': 2, 'mode': manifest['mode'], 'rmw_service_enabled': 1,
                    'source_root': str(root), 'catalog_sha256': args.rmw_service_catalog_sha256,
                    'scalar_imports': manifest['imports'], 'unitree_modular_motion': bool(args.unitree_modular_motion),
                    'scratch': {key: limits[key] for key in ('input_bytes', 'output_bytes')},
                    'expected_imports': full_import_contract(manifest, original, optional, bool(args.unitree_modular_motion)),
                    'source_bindings': {key: {field: pin[field] for field in ('path', 'bytes', 'sha256')}
                                        for key, pin in bindings.items()}}
    out.mkdir()
    (out / 'quickjs').mkdir(); (out / 'objects').mkdir()
    for name, data in engine_files:
        path = out / 'quickjs' / name
        path.parent.mkdir(parents=True, exist_ok=True)
        if name == 'quickjs.c': data = patch_engine(data.decode(), manifest['random_seed']).encode()
        if name == 'dtoa.c': data = patch_dtoa(data.decode()).encode()
        path.write_bytes(data)
    (out / 'module_table.c').write_text(table)
    (out / 'scalar_trampolines.c').write_text(trampolines)
    del table, trampolines
    allowed = []
    if manifest['backend']:
        abi_data, abi_pin = read(root / 'sdk/abi/abi-v1.json', 1048576)
        inputs.append(abi_pin); abi = json.loads(abi_data)
        allowed += ['uwvm_ros' + str(manifest['backend']) + '_' + row['name'] for row in abi['modules']['uwvm_ros' + str(manifest['backend']) + '_v1']['functions']]
        allowed += ['fd_read', 'fd_write']
    if manifest['mode'] == 'command': allowed.append('exit_process')
    if args.rmw_service_enabled:
        allowed += ['uwvm_ros2_rmw_srv_v1_' + name for name, _params in RMW_SIGNATURES]
    allowed += ['imported_' + str(index) for index in range(len(manifest['imports']))]
    if args.unitree_modular_motion:
        allowed += ['uwvm_unitree_motion_' + name for name in ('begin', 'status', 'word', 'end')]
        allowed += ['uwvm_unitree_perception_' + name for name in ('begin', 'status', 'word', 'end', 'abort')]
    (out / 'allowed-symbols.txt').write_text('\n'.join(allowed) + '\n')
    common = [str(args.clang), '--target=wasm32-wasip1', '--sysroot=' + str(args.sysroot), '-resource-dir=' + str(args.resource_dir),
              '-std=gnu11', manifest['optimization'], '-UNDEBUG', '-fwrapv', '-funsigned-char', '-fno-fast-math', '-ffp-contract=off',
              '-ffunction-sections', '-fdata-sections', '-DUWVM_JS_SANDBOX=1', '-DCONFIG_VERSION="2026-06-04"',
              '-D_GNU_SOURCE', '-DUWVM_JS_BACKEND=' + str(manifest['backend']), '-I', str(HERE), '-I', str(out / 'quickjs')]
    common += ['-DUWVM_JS_' + key.upper() + '=' + str(value) for key, value in limits.items()]
    if args.rmw_service_enabled:
        common += ['-DUWVM_JS_RMW_SERVICE=1']
    if args.unitree_direct_enabled is not None:
        common += ['-DUWVM_JS_UNITREE_CONTROL=1',
                   '-DUWVM_UNITREE_DIRECT_ENABLED=' + str(args.unitree_direct_enabled),
                   *(['-DUWVM_UNITREE_MODULAR_MOTION='+str(args.unitree_modular_motion),
                      '-DUWVM_UNITREE_MODULAR_PERCEPTION='+str(args.unitree_modular_motion), '-I', str(root / 'sdk/examples/robotics_modular')]
                     if args.unitree_modular_motion is not None else []),
                   '-I', str(root / 'sdk/robots/unitree/include')]
    sources = [out / 'quickjs' / name for name in ('quickjs.c', 'dtoa.c', 'libregexp.c', 'libunicode.c', 'cutils.c')]
    sources += [HERE / name for name in ('engine.c', 'scalars.c', 'utf8.c')]
    if manifest['backend']: sources += [HERE / 'native_ros.c', HERE / 'stdio.c']
    if args.rmw_service_enabled: sources.append(HERE / 'native_rmw_service.c')
    if manifest['mode'] == 'command': sources.append(HERE / 'command.c')
    if args.unitree_direct_enabled is not None:
        sources.append(root / 'sdk/robots/unitree/examples/javascript/control_kernel.c')
    sources += [out / 'module_table.c', out / 'scalar_trampolines.c']
    jobs, objects = [], []
    for index, path in enumerate(sources):
        pin = pin_file(path, 134217728); inputs.append(pin)
        obj = out / 'objects' / (str(index) + '.o'); dependency = obj.with_suffix('.d'); objects.append(str(obj))
        jobs.append({'id': 'compile-' + str(index), 'argv': [*common, '-MD', '-MF', str(dependency), '-MT', str(obj), '-c', str(path), '-o', str(obj)],
                     'cwd': str(out), 'source': pin, 'artifact': str(obj), 'dependencies': str(dependency)})
    artifact = out / 'guest.wasm'
    # LLVM22's WebAssembly driver selects an absolute linker with -fuse-ld;
    # --ld-path is not consumed by that target's getLinkerPath implementation.
    # WASI libc's default formatter has a long-double refusal diagnostic that
    # retains generic stderr/close/seek/write backends even for snprintf. The
    # official complete numeric formatter preserves those algorithms without
    # that refusal path. Select its exact pinned archive before libc; do not
    # replace libc functions with success stubs or broaden the Wasm ABI.
    # Wasm LLD admits archive members lazily, so pass the pinned archives
    # directly: ELF --start-group/--end-group options are not Wasm options.
    argv = [str(args.clang), '--target=wasm32-wasip1', '--sysroot=' + str(args.sysroot), '-resource-dir=' + str(args.resource_dir),
            '-fuse-ld=' + str(args.linker), '-nostdlib', '-nostartfiles', '-Wl,--threads=1', '-Wl,--gc-sections', '-Wl,--trace', '-Wl,--export-memory', '-Wl,--stack-first',
            '-Wl,-z,stack-size=' + str(limits['stack_bytes']), '-Wl,--initial-memory=' + str(initial), '-Wl,--max-memory=' + str(maximum),
            '-Wl,--allow-undefined-file=' + str(out / 'allowed-symbols.txt'),
            '-Wl,--entry=_start' if manifest['mode'] == 'command' else '-Wl,--no-entry',
            *objects, str(args.libm), str(args.printscan), str(args.libc), str(args.builtins), '-o', str(artifact)]
    jobs.append({'id': 'link', 'argv': argv, 'cwd': str(out), 'artifact': str(artifact), 'requires_retained_link_trace': True})
    jobs.append({'id': 'validate', 'argv': [str(args.validator), 'validate', str(artifact)], 'cwd': str(out), 'artifact': str(artifact)})
    if rmw_plan is not None:
        contract_path = out / 'full-imports-contract.json'
        contract_path.write_text(json.dumps(rmw_plan, indent=2, sort_keys=True) + '\n')
        contract_pin = pin_file(contract_path, 2097152)
        inputs.append(contract_pin)
        jobs.append({'id': 'full-binary-import-contract', 'argv': [str(python_path), '-I', '-S', '-B',
            str(root / 'sdk/tools/check_javascript_guest_imports.py'), '--wasm', str(artifact),
            '--contract', str(contract_path), '--contract-sha256', contract_pin['sha256'],
            '--report', str(out / 'full-imports-result.json')], 'cwd': str(out), 'artifact': str(artifact),
            'checker': rmw_plan['source_bindings']['checker'], 'contract': contract_pin,
            'requires_external_entry_interpreter_stdlib_and_process_admission': True})
    report = {'schema': 'uwvm-embedded-javascript-preparation-v1', 'status': 'PREPARED_NOT_COMPILED', 'jobs': jobs,
              'artifact': str(artifact), 'manifest': manifest, 'inputs': inputs, 'tools': tools,
              'upstream_archive_sha256': lock['archive_sha256'], 'expected_ros_imports': 38 if manifest['backend'] == 1 else 43 if manifest['backend'] == 2 else 0,
              'expected_wasi_imports': ['fd_read', 'fd_write', 'proc_exit'] if manifest['mode'] == 'command' else ['fd_read', 'fd_write'] if manifest['backend'] else [],
              'expected_library_imports': manifest['imports'], 'implicit_permissions': [], 'compiler_executed': False,
              'owner_requirements': 'Execute serially under exact cgroup, reserve/RSS/deadline/pidfd guards; retain every job, fullMD closure, linker trace and final binary import/export check. Source preparation grants nothing.'}
    report['expected_rmw_service_imports'] = 12 if rmw_plan is not None else 0
    report['expected_total_ros_imports'] = report['expected_ros_imports'] + report['expected_rmw_service_imports']
    report['owned_rmw_service'] = {'enabled': args.rmw_service_enabled,
        'module': 'roswasm:rmw_service_native', 'optional_functions': 12 if rmw_plan is not None else 0,
        'catalog_sha256': args.rmw_service_catalog_sha256, 'full_import_contract': rmw_plan,
        'scope': 'Source-only planning; exact full binary contract is mandatory when enabled, no native authority or execution proof'}
    if args.unitree_direct_enabled is not None:
        report['unitree_control'] = {'module': 'roswasm:unitree_control',
            'direct_enabled': args.unitree_direct_enabled,
            'scope': 'SAME_WASM_C_KERNEL_AND_CRC_ONLY_NO_NATIVE_ROS_OR_PERMISSION_BRIDGE',
            'source': pin_file(root / 'sdk/robots/unitree/examples/javascript/control_kernel.c')}
        if args.unitree_modular_motion:
            report['unitree_control']['composition'] = 'PURE_PERCEPTION_MOTION_CALLBACK_PRELOADS_NO_HOST_AUTHORITY'
            report['unitree_control']['scope'] = 'SAME_MAIN_C_ADAPTER_PURE_WASM_PRELOAD_MATH_NO_NATIVE_BRIDGE'
    (out / 'report.json').write_text(json.dumps(report, indent=2, sort_keys=True) + '\n')
    print(json.dumps({'status': report['status'], 'report': str(out / 'report.json'), 'jobs': len(jobs)}))


if __name__ == '__main__':
    main()
