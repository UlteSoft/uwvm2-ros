"""Prepare interpreter/CLI witnesses; importing this module executes no commands.

The actual ROS CLI suite compiles these WAT strings and runs the current Worker
inside the qualified Linux64GiB acceptance scope. Source generation alone is
never a compilation, runtime, native-side-effect or ARM verdict.
"""
PERMISSIONS = ('realtime-clock', 'monotonic-clock', 'process-clock', 'thread-clock',
               'random', 'proc-raise', 'sched-yield', 'stdio-full-rights')


def fixtures(width):
    """Build both ABI pointer-width witnesses with exact denial sentinels.

    Signal success is deliberately exercised by counted native callbacks in
    authority.cc; this Guest only verifies signal refusal and never raises a
    host signal. Positive clock/entropy/yield cases are real permitted calls.
    """
    if width not in (32, 64):
        raise ValueError('unsupported pointer width')
    pointer = 'i32' if width == 32 else 'i64'
    suffix = '' if width == 32 else '_wasm64'
    memory = '(memory 1)' if width == 32 else '(memory i64 1)'

    def ptr(value):
        return f'({pointer}.const {value})'

    def module(imports, body):
        return '(module\n' + imports + '\n' + memory + '\n(func (export "_start")\n' + body + '\n))\n'

    def expect(call, errno):
        return f'(if (i32.ne {call} (i32.const {errno})) (then unreachable))\n'

    sentinel = f'(i64.store {ptr(128)} (i64.const -1))\n'
    unchanged = f'(if (i64.ne (i64.load {ptr(128)}) (i64.const -1)) (then unreachable))\n'
    result = {}
    for clock, name in enumerate(PERMISSIONS[:4]):
        imports = (f'(import "wasi_snapshot_preview1" "clock_time_get{suffix}" '
                   f'(func $time (param i32 i64 {pointer}) (result i32)))\n'
                   f'(import "wasi_snapshot_preview1" "clock_res_get{suffix}" '
                   f'(func $res (param i32 {pointer}) (result i32)))')
        for granted in (False, True):
            error = 0 if granted else 76
            body = sentinel + expect(f'(call $time (i32.const {clock}) (i64.const 1) {ptr(128)})', error)
            if not granted:
                body += unchanged
            body += sentinel + expect(f'(call $res (i32.const {clock}) {ptr(128)})', error)
            if not granted:
                body += unchanged
            result[name + ('-granted' if granted else '-denied')] = module(imports, body)
    imports = (f'(import "wasi_snapshot_preview1" "random_get{suffix}" '
               f'(func $random (param {pointer} {pointer}) (result i32)))')
    for granted in (False, True):
        body = sentinel + expect(f'(call $random {ptr(128)} {ptr(8)})', 0 if granted else 76)
        if not granted:
            body += unchanged
        result['random' + ('-granted' if granted else '-denied')] = module(imports, body)
    # Scalar process APIs retain i32 arguments/results for both pointer models.
    for name, abi_name, call in (('proc-raise', 'proc_raise', '(call $call (i32.const 0))'),
                                 ('sched-yield', 'sched_yield', '(call $call)')):
        imports = f'(import "wasi_snapshot_preview1" "{abi_name}{suffix}" (func $call ' + ('(param i32) ' if name == 'proc-raise' else '') + '(result i32)))'
        result[name + '-denied'] = module(imports, expect(call, 76))
        if name == 'sched-yield':
            result[name + '-granted'] = module(imports, expect(call, 0))
    imports = (f'(import "wasi_snapshot_preview1" "environ_sizes_get{suffix}" '
               f'(func $sizes (param {pointer} {pointer}) (result i32)))')
    for count, name in ((0, 'empty-environment'), (1, 'one-explicit-environment')):
        body = expect(f'(call $sizes {ptr(128)} {ptr(144)})', 0)
        body += f'(if ({pointer}.ne ({pointer}.load {ptr(128)}) ({pointer}.const {count})) (then unreachable))\n'
        result[name] = module(imports, body)
    imports = (f'(import "wasi_snapshot_preview1" "fd_fdstat_get{suffix}" '
               f'(func $stat (param i32 {pointer}) (result i32)))')
    body = ''
    for fd, base in ((0, 0x08200002), (1, 0x08200040), (2, 0x08200040)):
        body += expect(f'(call $stat (i32.const {fd}) {ptr(128)})', 0)
        body += f'(if (i64.ne (i64.load {ptr(136)}) (i64.const {base})) (then unreachable))\n'
        body += f'(if (i64.ne (i64.load {ptr(144)}) (i64.const 0)) (then unreachable))\n'
    result['basic-stdio-rights'] = module(imports, body)
    return result


def cases(benign, compiled):
    """Return explicit static recipes; no permission is inferred from imports.

    ``compiled`` maps (width, fixture name) to freshly compiled paths. The
    caller preserves compiler logs and exact Worker commands in its report.
    Each actual scope test names a real main module and real Single/Group.
    """
    rows = []

    def add(name, options, success=True):
        rows.append((name, options, success))

    for name in PERMISSIONS:
        for scope in ('global', 'single', 'group'):
            prefix = '--wasip1-' + scope + '-'
            target = [] if scope == 'global' else ['rights_probe' if scope == 'single' else 'rights_group']
            setup = ['--wasm-set-main-module-name', 'rights_probe']
            if scope != 'global':
                setup += [prefix + 'create', *target]
            if scope == 'group':
                setup += [prefix + 'add-module', *target, 'rights_probe']
            grant = [prefix + 'grant', *target, name]
            deny = [prefix + 'deny', *target, name]
            tail = ['--run', str(benign)]
            add(f'wasi-{scope}-{name}-explicit-grant', [*setup, *grant, *tail])
            add(f'wasi-{scope}-{name}-duplicate-grant', [*setup, *grant, *grant, *tail], False)
            add(f'wasi-{scope}-{name}-conflicting-grant-deny', [*setup, *grant, *deny, *tail], False)
    add('wasi-explicit-noinherit-with-safe-default', ['--wasip1-global-noinherit-system-environment', '--run', str(benign)])
    add('wasi-inherit-noinherit-conflict', ['--wasip1-global-inherit-system-environment', '--wasip1-global-noinherit-system-environment', '--run', str(benign)], False)
    add('wasi-noinherit-inherit-conflict', ['--wasip1-global-noinherit-system-environment', '--wasip1-global-inherit-system-environment', '--run', str(benign)], False)
    add('wasi-unknown-permission-refused', ['--wasip1-global-grant', '*', '--run', str(benign)], False)
    for width in (32, 64):
        feature = [] if width == 32 else ['--wasm-feature-enable-memory64']
        for name, artifact in sorted((name, path) for (bits, name), path in compiled.items() if bits == width):
            authority = []
            if name.endswith('-granted'):
                authority = ['--wasip1-global-grant', name.removesuffix('-granted')]
            if name == 'one-explicit-environment':
                authority = ['--wasip1-global-add-or-replace-environment', 'ROSWASM_EXPLICIT_TEST', 'one']
            add(f'wasi{width}-{name}', [*feature, *authority, '--run', str(artifact)])
        for name in PERMISSIONS[:4]:
            for scope, target in (('single', 'rights_probe'), ('group', 'rights_group')):
                prefix = '--wasip1-' + scope + '-'
                setup = [*feature, '--wasm-set-main-module-name', 'rights_probe', prefix + 'create', target]
                if scope == 'group':
                    setup += [prefix + 'add-module', target, 'rights_probe']
                add(f'wasi{width}-{scope}-{name}-grant', [*setup, prefix + 'grant', target, name,
                    '--run', str(compiled[(width, name + '-granted')])])
                add(f'wasi{width}-{scope}-{name}-deny-over-global', [*setup, '--wasip1-global-grant', name,
                    prefix + 'deny', target, name, '--run', str(compiled[(width, name + '-denied')])])
    return rows
