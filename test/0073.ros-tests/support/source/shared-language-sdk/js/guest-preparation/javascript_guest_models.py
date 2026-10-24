#!/usr/bin/env python3
"""Source/recipe models for the actual embedded-JavaScript Wasm feature.

The owner may run these finite standard-library models serially in its frozen
source snapshot. They never import the producer, run QuickJS/Node/a compiler,
start a process, write build files or connect ROS. Only reviewed pure functions
are extracted from the producer AST into an isolated namespace. A source-model
PASS is not actual engine, binary-import, Runtime or native-delivery evidence.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

import ast
import copy
import json
import math
from pathlib import Path
import posixpath
import re
import unittest

SDK = _CHECKOUT_ROOT / 'sdk/origin'
ROOT = _CHECKOUT_ROOT
GUEST = _sdk_path(SDK, 'javascript/guest')
BASE_IDS = {
    lesson + '-ros' + str(version)
    for lesson in ('00-raw', '01-codecs', '02-node', '03-application',
                   '04-async-application', '05-ownership-scheduling',
                   '06-custom-codec', '07-complete-denied-abi',
                   '08-unified-events', '09-boundary-refusals')
    for version in (1, 2)
} | {'limit-interrupt', 'limit-heap', 'limit-jobs', 'reactor-vision',
     'reactor-callback', 'reactor-motion', 'benchmark_guest', 'benchmark_session_guest'}
EXPECTED_IDS = BASE_IDS | {'float_guest', '10-floating-modules'}
ACCEPTANCE_RECIPE_OVERRIDES = {'vision_guest':'reactor-vision',
    'callback_guest':'reactor-callback', 'motion_guest':'reactor-motion'}
EXPECTED_ACCEPTANCE_IDS = (EXPECTED_IDS - set(ACCEPTANCE_RECIPE_OVERRIDES.values())) | set(ACCEPTANCE_RECIPE_OVERRIDES)


def source(path):
    return path.read_text(encoding='utf-8')


def document(name):
    return json.loads(source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', name)))


def pure():
    """Select only source-to-text constructors, never planner IO or main()."""
    path = _sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'prepare.py')
    tree = ast.parse(source(path))
    wanted = {'require', 'canonical', 'literal', 'failure_literal',
              'expand_recipe', 'module_table', 'scalar_trampolines',
              'patch_engine', 'patch_dtoa'}
    functions = [item for item in tree.body if isinstance(item, ast.FunctionDef) and item.name in wanted]
    if {item.name for item in functions} != wanted:
        raise AssertionError('reviewed pure constructors changed')
    types = next(item for item in tree.body if isinstance(item, ast.Assign) and
                 any(isinstance(target, ast.Name) and target.id == 'TYPES' for target in item.targets))
    namespace = {'json': json, 'math': math, 're': re, 'TYPES': ast.literal_eval(types.value)}
    for function in functions:
        for item in ast.walk(function):
            if isinstance(item, (ast.Import, ast.ImportFrom)) or isinstance(item, ast.Attribute) and item.attr in (
                    'read_text', 'read_bytes', 'open', 'write_text', 'write_bytes', 'mkdir', 'run', 'Popen'):
                raise AssertionError('side effect introduced into a pure constructor')
    selected = ast.fix_missing_locations(ast.Module(body=functions, type_ignores=[]))
    exec(compile(selected, str(path), 'exec'), namespace)
    return namespace


def static_imports(text):
    """Read the explicitly used static import/re-export source grammar.

    This is deliberately not a JS parser or syntax validator. Actual QuickJS
    compilation and closed-loader execution remain separate qualification jobs.
    Dynamic negative imports are handled by actual boundary-refusal fixtures.
    """
    pattern = r'''(?m)^[ \t]*(?:import|export)[ \t]+(?:(?:\{[^}]*\}|\*\s*(?:as\s+\w+)?|[A-Za-z_$][A-Za-z0-9_$]*(?:\s*,\s*\{[^}]*\})?)\s+from\s+)?['"]([^'"]+)['"]'''
    return re.findall(pattern, text)


def export_row(kind='u32'):
    return {'name': 'checked_result', 'function': 'checkedResult',
            'parameters': ['i32', 'u32', 'i64', 'u64', 'f32', 'f64'],
            'result': kind, 'failure': 0, 'initialize': True}


class JavaScriptGuestModels(unittest.TestCase):
    def test_exact_original28_and_two_additive_float_variants(self):
        recipes, plan = document('recipes.json'), document('acceptance.json')
        self.assertEqual(len(BASE_IDS), 28)
        self.assertEqual(set(recipes['variants']), EXPECTED_IDS)
        self.assertEqual({row['id'] for row in plan['variants']}, EXPECTED_ACCEPTANCE_IDS)
        self.assertEqual({row['recipe_variant'] for row in plan['variants']}, EXPECTED_IDS)
        for row in plan['variants']:
            self.assertEqual(row['recipe_variant'], ACCEPTANCE_RECIPE_OVERRIDES.get(row['id'], row['id']))
        self.assertEqual(len(plan['variants']), plan['variant_count'])
        self.assertEqual(plan['variant_count'], 30)
        self.assertEqual(plan['status'], 'SOURCE_PLAN_NOT_EXECUTED')

    def test_every_recipe_has_a_complete_finite_closed_source_map(self):
        constructors, recipes = pure(), document('recipes.json')
        for name in sorted(EXPECTED_IDS):
            manifest = constructors['expand_recipe'](recipes, name)
            modules = manifest['modules']; table = {row['name']: row['source'] for row in modules}
            self.assertEqual(len(table), len(modules), name)
            self.assertIn(manifest['entry'], table)
            self.assertIn('sdk/origin/javascript/guest/bootstrap.mjs', table)
            self.assertLessEqual(len(modules), manifest['limits']['max_modules'])
            total = 0
            for module, relative in table.items():
                self.assertTrue(constructors['canonical'](module), module)
                self.assertTrue(constructors['canonical'](relative), relative)
                path = (ROOT / relative).resolve(strict=True)
                self.assertTrue(path.is_relative_to(ROOT))
                text = source(path); total += len(text.encode('utf-8'))
                self.assertNotIn('\x00', text)
                for requested in static_imports(text):
                    if requested.startswith('roswasm:'):
                        allowed = {'roswasm:utf8', 'roswasm:library_imports'}
                        if manifest['backend']: allowed.add('roswasm:guest_native')
                        self.assertIn(requested, allowed, (name, module))
                    else:
                        resolved = posixpath.normpath(posixpath.join(posixpath.dirname(module), requested)) if requested.startswith('.') else requested
                        self.assertIn(resolved, table, (name, module, requested))
            self.assertLessEqual(total, manifest['limits']['max_source_bytes'])
            self.assertLess(manifest['limits']['heap_bytes'], manifest['maximum_memory'])
            self.assertLessEqual(manifest['initial_memory'], manifest['maximum_memory'])
            self.assertEqual(manifest['initial_memory'] % 65536, 0)
            self.assertEqual(manifest['maximum_memory'] % 65536, 0)

    def test_recipe_ids_and_closed_overrides_cannot_silently_fallback(self):
        expand, recipes = pure()['expand_recipe'], document('recipes.json')
        for name in (None, '', 'unknown', '../reactor-vision'):
            with self.assertRaises(ValueError): expand(recipes, name)
        invalid = copy.deepcopy(recipes)
        invalid['variants']['reactor-vision']['permissions'] = ['all']
        with self.assertRaises(ValueError): expand(invalid, 'reactor-vision')
        for name in ('std', 'os', '/etc/passwd', '../entry', 'sdk/../entry', 'x//y', 'node:fs', 'native.so', 'x\\y'):
            self.assertFalse(pure()['canonical'](name), name)

    def test_pure_provider_recipes_have_no_ros_or_basic_wasi_path(self):
        constructors, recipes = pure(), document('recipes.json')
        for name in ('reactor-vision', 'reactor-callback', 'reactor-motion', 'float_guest'):
            manifest = constructors['expand_recipe'](recipes, name)
            self.assertEqual((manifest['backend'], manifest['mode']), (0, 'reactor'))
            self.assertTrue(all('javascript/guest.mjs' != row['name'].removeprefix('sdk/') for row in manifest['modules']))
            self.assertFalse(any(row['source'].endswith(('guest.mjs', 'relay.mjs')) and row['source'].startswith('sdk/origin/javascript/') for row in manifest['modules']))
            self.assertTrue(all(row['module'] == 'callbacks' and row['name'] == 'on_motion' for row in manifest['imports']))
        producer = ast.parse(source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'prepare.py')))
        main = next(item for item in producer.body if isinstance(item, ast.FunctionDef) and item.name == 'main')
        backend_if = [item for item in main.body if isinstance(item, ast.If) and
            isinstance(item.test, ast.Subscript) and isinstance(item.test.value, ast.Name) and
            item.test.value.id == 'manifest' and isinstance(item.test.slice, ast.Constant) and item.test.slice.value == 'backend']
        self.assertTrue(any('native_ros.c' in ast.get_source_segment(source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'prepare.py')), item) and
                            'stdio.c' in ast.get_source_segment(source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'prepare.py')), item) for item in backend_if))

    def test_shared_native_dispatch_is_exact_public_38_and43(self):
        schema = json.loads(source(_sdk_path(SDK, 'abi/abi-v1.json')))
        header = source(_sdk_path(SDK, 'javascript/relay_protocol.h'))
        blocks = header.split('#define UWVM_JS_RELAY_ABI_ROWS(X)')[1:]
        self.assertEqual(len(blocks), 2)
        for version, block in enumerate(blocks, 1):
            rows = re.findall(r'X\(([0-9]+), ([a-z0-9_]+), ([A-Z]+), ([0-9]+), ([0-9]+)u\)', block)
            functions = schema['modules']['uwvm_ros' + str(version) + '_v1']['functions']
            self.assertEqual(len(rows), 38 if version == 1 else 43)
            self.assertEqual([row[1] for row in rows], [row['name'] for row in functions])
            for index, (row, function) in enumerate(zip(rows, functions)):
                self.assertEqual(int(row[0]), index)
                self.assertEqual(int(row[3]), len(function['params']))
                mask = sum(1 << bit for bit, parameter in enumerate(function['params']) if parameter['type'] == 'i64')
                self.assertEqual(int(row[4]), mask)
                self.assertEqual(function['results'], ['i32'])
        native = source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'native_ros.c'))
        self.assertIn('UWVM_JS_RELAY_ABI_ROWS(ROW)', native)
        self.assertIn('#include "../../ros1/abi.h"', native)
        self.assertIn('#include "../../ros2/abi.h"', native)
        self.assertNotIn('socket(', native)
        self.assertNotIn('getenv(', native)

    def test_scalar_generation_keeps_all_core_widths_and_finite_failure(self):
        constructors = pure()
        imports = [{'module': 'numerical', 'name': 'mixed', 'parameters': export_row()['parameters'], 'result': 'f64'}]
        for kind, (ctype, checked, factory) in constructors['TYPES'].items():
            emitted = constructors['scalar_trampolines']([export_row(kind)], imports)
            self.assertIn(ctype + ' exported_0(', emitted)
            self.assertIn('import_module("numerical"),import_name("mixed")', emitted)
            self.assertIn(checked + '(uwvm_js_context,value,&result)', emitted)
            self.assertIn(factory + '(uwvm_js_context', emitted)
            self.assertIn('uwvm_js_fail(UWVM_JS_EXCEPTION)', emitted)
        for kind in ('f32', 'f64'):
            for value in (float('inf'), float('-inf'), float('nan'), True, '0'):
                with self.assertRaises(ValueError): constructors['failure_literal'](kind, value)
        self.assertIn('(-INT64_C(9223372036854775807)-1)', constructors['failure_literal']('i64', -2**63))
        self.assertIn('-0x0.0p+0', constructors['failure_literal']('f32', -0.0))
        for kind, value in (('i32', 2**31), ('u32', -1), ('u64', 2**64), ('f32', 1e100)):
            with self.assertRaises(ValueError): constructors['failure_literal'](kind, value)

    def test_scalar_symbols_are_closed_and_failures_do_not_clear_status(self):
        emit = pure()['scalar_trampolines']
        for field, value in (('name', '_start'), ('name', 'roswasm_js_init'), ('function', 'bad-name'),
                             ('parameters', ['u32'] * 8), ('result', 'pointer'), ('initialize', 1), ('failure', 0.5)):
            row = export_row(); row[field] = value
            with self.assertRaises(ValueError): emit([row], [])
        with self.assertRaises(ValueError): emit([export_row(), export_row()], [])
        with self.assertRaises(ValueError): emit([], [{'module':'os', 'name':'open', 'parameters':[], 'result':'u32'}])
        emitted = emit([export_row()], [])
        self.assertIn('if(roswasm_js_status()!=UWVM_JS_OK)return', emitted)
        self.assertNotIn('uwvm_js_fail(UWVM_JS_OK)', emitted)

    def test_generated_literal_products_preserve_nul_termination_and_size(self):
        text = pure()['module_table']([('safe/entry.mjs', b'export const marker="x";\n')], 'safe/entry.mjs')
        self.assertIn('#include "engine.h"', text)
        self.assertIn('"safe/entry.mjs",source_0,25', text)
        self.assertIn('uwvm_js_bootstrap_module[]="sdk/origin/javascript/guest/bootstrap.mjs"', text)
        self.assertTrue(text.endswith('\n'))
        producer = source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'prepare.py'))
        self.assertIn("(out / 'module_table.c').write_text(table)", producer)
        self.assertIn("(out / 'scalar_trampolines.c').write_text(trampolines)", producer)
        self.assertNotIn('module_table.h', producer)

    def test_actual_release_lock_is_complete_and_no_moving_branch(self):
        lock = document('upstream-lock.json')
        self.assertEqual(lock['version'], '2026-06-04')
        self.assertEqual(lock['archive_bytes'], 621500)
        self.assertEqual(lock['archive_sha256'], 'b376e839b322978313d929fd20663b11ba58b75df5a46c126dd19ea2fa70ad2a')
        self.assertEqual(lock['official_url'], 'https://bellard.org/quickjs/quickjs-2026-06-04.tar.xz')
        files = {row['path']: row for row in lock['files']}
        self.assertEqual(len(files), 29)
        self.assertTrue({'quickjs.c','quickjs.h','dtoa.c','libregexp.c','libunicode.c','cutils.c','LICENSE','VERSION'} <= set(files))
        self.assertEqual(files['quickjs.h']['sha256'], '2165f47772af9faee1798999a599fa9de850d1bf0259502dade1c25d4a588316')
        self.assertTrue(all(re.fullmatch('[0-9a-f]{64}', row['sha256']) and 0 < row['bytes'] <= 8388608 for row in files.values()))

    def test_producer_has_no_execution_and_bounded_regular_reads(self):
        text = source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'prepare.py')); tree = ast.parse(text)
        for item in ast.walk(tree):
            if isinstance(item, (ast.Import, ast.ImportFrom)):
                names = [value.name for value in item.names] if isinstance(item, ast.Import) else [item.module]
                self.assertFalse(any(name and name.split('.')[0] in ('subprocess','importlib','socket','urllib') for name in names))
            if isinstance(item, ast.Call):
                self.assertFalse(isinstance(item.func, ast.Name) and item.func.id in ('eval','exec','compile','__import__'))
                self.assertFalse(isinstance(item.func, ast.Attribute) and item.func.attr in ('system','popen','Popen','read_bytes'))
        self.assertIn('os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK', text)
        self.assertIn('stat.S_ISREG(before.st_mode)', text)
        self.assertIn('before.st_size <= maximum', text)
        self.assertIn('data = stream.read(maximum + 1)', text)
        self.assertIn("min(8388608, remaining)", text)
        self.assertIn("limits['print_bytes'] <= 4193786", text)

    def test_job_source_pins_explicit_linker_single_thread_and_full_md(self):
        text = source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'prepare.py'))
        for literal in ("'-fuse-ld=' + str(args.linker)", "'-Wl,--threads=1'", "'-UNDEBUG'",
                        "'-MD', '-MF', str(dependency), '-MT', str(obj)", "'requires_retained_link_trace': True",
                        "'alias': str(args.linker), 'physical': pin_file(linker_physical)"):
            self.assertIn(literal, text)
        for unsupported in ("'-Wl,--start-group'", "'-Wl,--end-group'", "'--ld-path=' + str(args.linker)"):
            self.assertNotIn(unsupported, text)
        self.assertIn("'quickjs.c', 'dtoa.c', 'libregexp.c', 'libunicode.c', 'cutils.c'", text)
        self.assertNotIn("sources.append(out / 'quickjs' / 'quickjs-libc.c')", text)
        self.assertNotIn("'-Wl,--allow-undefined'", text)
        self.assertIn("'PREPARED_NOT_COMPILED'", text)
        self.assertIn("'libc', 'libm', 'printscan', 'builtins'", text)
        self.assertIn("args.sysroot / 'lib/wasm32-wasip1/libc-printscan-long-double.a'", text)
        self.assertIn("*objects, str(args.libm), str(args.printscan), str(args.libc), str(args.builtins)", text)

    def test_engine_limits_are_independent_of_ros_rights_and_owned_io(self):
        engine, scalars = source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'engine.c')), source(_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'javascript/guest', 'scalars.c'))
        for literal in ('JS_NewRuntime2(&allocator,NULL)', 'JS_SetMaxStackSize', 'JS_SetInterruptHandler',
                        'JS_SetModuleLoaderFunc2(runtime,normalize,loader,attributes,NULL)', 'JS_SetCanBlock(runtime,0)',
                        'jobs_used>=UWVM_JS_MAX_JOBS', 'interrupt_remaining=UWVM_JS_INTERRUPT_POLLS'):
            self.assertIn(literal, engine)
        for forbidden in ('JS_AddIntrinsicDate(', 'quickjs-libc.h', 'js_std_', 'getenv(', 'dlopen(', 'socket('):
            self.assertNotIn(forbidden, engine)
        self.assertIn('if (!JS_IsNumber(value))', scalars)
        self.assertIn('*output = (float)number', scalars)
        self.assertIn('JS_StrictEq(ctx, value, restored)', scalars)
        self.assertIn('JS_GetTypedArrayBuffer(ctx, value', scalars)
        self.assertIn('JS_NewArrayBufferCopy(ctx, data, length)', scalars)
        self.assertIn('__assert_fail(char const *expression,char const *file,int line,char const *function)', engine)
        for libc_override in ('__stdio_close(', '__stdio_seek(', '__stdio_write(', '__stdio_exit_needed('):
            self.assertNotIn(libc_override, engine)

    def test_copied_async_reply_preserves_shared_owned_slot_contract(self):
        copied = source(_sdk_path(SDK, 'javascript/copied_transport.mjs'))
        asynchronous = source(_sdk_path(SDK, 'javascript/async.mjs'))
        self.assertIn('const slotBytes = new Uint8Array(8)', copied)
        self.assertIn('setBigUint64(0, reply.slot, true)', copied)
        self.assertIn('return { status, slot: reply.slot, slotBytes, snapshot: reply.snapshot }', copied)
        self.assertIn('slotBytes:reply.slotBytes.slice()', asynchronous)
        self.assertIn('channel instanceof CopiedChannel', asynchronous)
        self.assertNotIn('new WebAssembly.Memory(', copied)

    def test_limit_plans_require_actual_armed_exit_and_no_timeout_substitute(self):
        rows = {row['id']:row for row in document('acceptance.json')['variants']}
        for name in ('limit-heap', 'limit-interrupt', 'limit-jobs'):
            row = rows[name]
            self.assertEqual(row['exit'], 3)
            self.assertEqual(row['stdout'], {'kind':'exact', 'text':'JS_GUEST_LIMIT_ARMED\n'})
            self.assertEqual(row['default_profile'], 'empty_ros')
            self.assertIn('JS_GUEST_LIMIT_ARMED\\n', source(_sdk_path(SDK, 'examples/javascript_guest', name + '.mjs')))
        self.assertEqual(rows['02-node-ros2']['runtime_scope'], 'actual_native_delivery')
        self.assertTrue(rows['02-node-ros2']['independent_native_witness'])
        self.assertEqual(rows['benchmark_session_guest']['runtime_scope'], 'actual_modular_synthetic_computation')
        self.assertEqual(rows['benchmark_session_guest']['expected_ros_count'], 43)


if __name__ == '__main__':
    unittest.main()
