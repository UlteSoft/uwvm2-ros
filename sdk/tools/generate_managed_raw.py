#!/usr/bin/env python3
"""Generate complete numeric Java and copied-buffer Python ROS import layers.

Input is the reviewed SDK ABI JSON, not an invented foreign runtime interface.
The output path is explicit; use a /tmp staging directory when checking generated
bytes. Generation performs no compilation, guest execution or ROS operation.
Handwritten lifetime/codecs remain in their respective language packages.
"""
import argparse
import hashlib
import json
from pathlib import Path


def java_raw(module, spec):
    """Emit numeric @Import declarations for TeaVM's WASI linear-heap target."""
    backend = spec['backend']
    rows = [
        'package org.uwvm.roswasm;',
        '',
        'import org.teavm.interop.Import;',
        'import org.teavm.interop.Unmanaged;',
        '',
        '/** Complete ROS %d ABI v1 imports for TeaVM WEBASSEMBLY_WASI.\n'
        ' * Numeric offsets are long/i64; memory indices are int/i32.\n'
        ' * No method grants authority. Do not pass WasmGC references as offsets.\n'
        ' * Generated from sdk/abi/abi-v1.json; edit the generator instead. */' % backend,
        'public final class Ros%dRaw {' % backend,
        '    private Ros%dRaw() {}' % backend,
    ]
    for f in spec['functions']:
        params = ', '.join(('int' if p['type'] == 'i32' else 'long') + ' ' + p['name'] for p in f['params'])
        rows += ['    /** ' + f['description'] + ' */',
                 '    @Import(module="%s", name="%s")' % (module, f['name']),
                 '    @Unmanaged',
                 '    public static native int %s(%s);' % (f['name'], params)]
    rows += ['}', '']
    return '\n'.join(rows)


def java_transport(spec):
    """Keep ROS1/ROS2 reachable classes separate so a binary selects one module."""
    backend = spec['backend']
    rows = [
        'package org.uwvm.roswasm;', '',
        '/** Copied-buffer ROS%d transport. Constructing this object opens nothing.\n'
        ' * Select this class explicitly; no runtime backend fallback is used.\n'
        ' * Generated from the complete ABI registry. */' % backend,
        'public final class Ros%dTransport implements Transport, RetirementTransport {' % backend,
        '    private final OpenCleanup openCleanup = new OpenCleanup();',
        '    /** Reserved result-delivery cleanup diagnostics, reset on the next open. */',
        '    public OpenCleanup lastOpenCleanup() { return openCleanup; }',
        '    public int backend() { return %d; }' % backend,
        '    public Result invoke(String operation, long handle, byte[] input, int capacity, long other, long correlation) {',
        '        return invokePrepared(operation, handle, input, capacity, other, correlation, null);',
        '    }',
        '    public Result retire(String operation, long handle, byte[] empty, RetirementAttempt attempt) {',
        '        if (!"capability_close".equals(operation) && !"capability_revoke".equals(operation))',
        '            throw new IllegalArgumentException("retirement operation");',
        '        return invokePrepared(operation, handle, empty, 0, 0, 0, attempt);',
        '    }',
        '    private Result invokePrepared(String operation, long handle, byte[] input, int capacity, long other, long correlation, RetirementAttempt attempt) {',
        '        if (operation == null) throw new IllegalArgumentException("operation");',
        '        LinearMemory.Frame f = LinearMemory.enter(input, capacity);',
        '        long openedHandle = 0;',
        '        try {',
        '            int code;',
        '            switch (operation) {',
    ]
    for f in spec['functions']:
        n, form = f['name'], f['form']
        args = {
            'version': '', 'open': '0, f.inputOffset(), f.input.length, f.slotOffset()',
            'handle': 'handle', 'write': 'handle, 0, f.inputOffset(), f.input.length',
            'poll': 'handle, 0, f.outputOffset(), capacity, f.slotOffset()',
            'watch': 'handle, other, correlation', 'ready': 'handle, 0, f.outputOffset()',
            'submit': 'handle, other, 0, f.inputOffset(), f.input.length, f.slotOffset()',
            'rpc': 'handle, 0, f.inputOffset(), f.input.length, f.outputOffset(), capacity, f.slotOffset()',
        }[form]
        rows.append('            case "%s":' % n)
        if form == 'ready':
            rows.append('                if (capacity != 32) throw new IllegalArgumentException("event_ready needs32 bytes");')
        if form == 'open':
            rows.append('                openCleanup.reset();')
            rows.append('                if (f.input.length == 0 || f.input.length > 255) throw new IllegalArgumentException("alias length");')
        if form in ('open', 'submit'):
            rows.append('                f.clearValueSlot();')
        if form == 'version':
            rows.append(f'                return Result.value(Ros{backend}Raw.{n}());')
        else:
            if n in ('capability_close', 'capability_revoke'):
                rows.append(f'                if (attempt != null) attempt.markAttempted("uwvm_ros{backend}_v1.{n}");')
            rows.append(f'                code = Ros{backend}Raw.{n}({args});')
            if n in ('capability_close', 'capability_revoke'):
                rows.append('                if (attempt != null) attempt.returned(code);')
            if form == 'open':
                rows.append('                if (code == 0) openedHandle = f.slotValue();')
            kind = {'open': 'VALUE', 'submit': 'VALUE', 'rpc': 'LENGTH', 'poll': 'LENGTH', 'ready': 'READY'}.get(form, 'NONE')
            if form == 'open':
                rows.append('                Result opened = f.finish(code, LinearMemory.VALUE);')
                rows.append('                openedHandle = 0; return opened;')
            else:
                rows.append('                return f.finish(code, LinearMemory.%s);' % kind)
    rows += ['            default: return new Result(95);', '            }',
             '        } catch (RuntimeException|Error primary) {',
             '            if (openedHandle != 0) {',
             '                openCleanup.started(primary);',
             '                try {',
             f'                    int cleanup = Ros{backend}Raw.capability_close(openedHandle);',
             '                    openCleanup.returned(cleanup);',
             '                } catch (RuntimeException|Error cleanup) { openCleanup.failed(cleanup); }',
             '            }',
             '            throw primary;',
             '        } finally { LinearMemory.leave(f); }', '    }', '}', '']
    return '\n'.join(rows)


def python_dispatch(spec):
    """Generate direct header calls; all borrowed storage remains in C scope."""
    rows = []
    for backend in (1, 2):
        rows.append('#%s ROSWASM_BACKEND == %d' % ('if' if backend == 1 else 'elif', backend))
        for i, f in enumerate(spec['modules'][f'uwvm_ros{backend}_v1']['functions']):
            n, form = f['name'], f['form']
            p = 'ROSWASM_OFFSET(request.buf)'
            q = 'ROSWASM_OFFSET(PyBytes_AS_STRING(output))'
            s = 'ROSWASM_OFFSET(&slot)'
            args = {
                'version': '', 'open': f'0, {p}, (uint64_t)request.len, {s}',
                'handle': 'handle', 'write': f'handle, 0, {p}, (uint64_t)request.len',
                'poll': f'handle, 0, {q}, (uint64_t)capacity, {s}',
                'watch': 'handle, other, correlation', 'ready': f'handle, 0, {q}',
                'submit': f'handle, other, 0, {p}, (uint64_t)request.len, {s}',
                'rpc': f'handle, 0, {p}, (uint64_t)request.len, {q}, (uint64_t)capacity, {s}',
            }[form]
            rows.append('    %s (strcmp(name, "%s") == 0) {' % ('if' if i == 0 else 'else if', n))
            rows.append('        found = 1;')
            if form == 'ready':
                rows.append('        if (capacity != 32) { PyErr_SetString(PyExc_ValueError, "event_ready requires capacity32"); goto done; }')
            if form == 'open':
                rows.append('        if (request.len == 0 || request.len > 255) { PyErr_SetString(PyExc_ValueError, "alias length must be 1..255 UTF-8 bytes"); goto done; }')
            if form in ('open', 'submit'):
                rows.append('        slot = 0;')
            if n in ('capability_close', 'capability_revoke'):
                rows.append('        roswasm_mark_retirement(retirement_witness);')
            if form == 'version':
                rows.append(f'        slot = uwvm_ros{backend}_{n}({args}); output_kind = 4;')
            else:
                rows.append(f'        code = uwvm_ros{backend}_{n}({args});')
                if form == 'open':
                    rows.append('        owns_open_handle = 1;')
                kind = {'open': 2, 'submit': 2, 'rpc': 1, 'poll': 1, 'ready': 3}.get(form, 0)
                rows.append(f'        output_kind = {kind};')
            rows.append('    }')
    rows.append('#endif')
    return '\n'.join(rows)


def python_registry(spec):
    """Expose all names/forms without importing the optional native extension."""
    return ('"""Complete ABI function inventory. Generated; importing grants no authority."""\n'
            'FUNCTIONS = ' + repr({m['backend']: {f['name']: f['form'] for f in m['functions']}
                                   for m in spec['modules'].values()}) + '\n')


def main():
    """Write only managed raw sources underneath the caller-selected output."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--sdk', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    abi_path = args.sdk / 'abi/abi-v1.json'
    abi_bytes = abi_path.read_bytes()
    spec = json.loads(abi_bytes)
    if spec['abi_version'] != 1 or spec['memory_contract']['offset_length_handle'] != 'i64':
        raise ValueError('unreviewed ABI contract')
    files = {}
    for module, backend in spec['modules'].items():
        files['java/src/main/java/org/uwvm/roswasm/Ros%dRaw.java' % backend['backend']] = java_raw(module, backend)
        files['java/src/main/java/org/uwvm/roswasm/Ros%dTransport.java' % backend['backend']] = java_transport(backend)
    template = (args.sdk / 'python/roswasm/_native.c.in').read_text()
    if template.count('@DISPATCH@') != 1:
        raise ValueError('native dispatch marker drift')
    files['python/roswasm/_native.c'] = template.replace('@DISPATCH@', python_dispatch(spec))
    files['python/roswasm/_registry.py'] = python_registry(spec)
    for relative, text in files.items():
        destination = args.out / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(text)
    print(json.dumps({'abi_sha256': hashlib.sha256(abi_bytes).hexdigest(), 'files': sorted(files),
                      'ros1': 38, 'ros2': 43, 'runtime_tests_executed': 0}))


if __name__ == '__main__':
    main()
