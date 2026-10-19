#!/usr/bin/env python3
"""Inspect L01 configuration source and evaluate its extracted bit-mask expression.

This tool performs source checks and a Python truth-table model only. It does not
compile C++, instantiate WebAssembly, execute a Guest, or validate ROS behavior.
The report binds every inspected source to its SHA-256. Native and Guest tests
remain separate evidence and must run inside the prescribed Linux cgroup.
"""
from __future__ import annotations

import argparse
import ast
import hashlib
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument('--output', type=Path, required=True, help='New report path; existing evidence is never overwritten')
    args = parser.parse_args()
    root = args.source.resolve()
    paths = {
        'environment': 'src/uwvm2/imported/wasi/wasip1/environment/environment.h',
        'storage': 'src/uwvm2/uwvm/imported/wasi/wasip1/storage/env.h',
        'initializer': 'src/uwvm2/uwvm/imported/wasi/wasip1/init/init_env.h',
        'parameters': 'src/uwvm2/uwvm/cmdline/params/wasip1_permissions.h',
        'callbacks': 'src/uwvm2/uwvm/cmdline/callback/wasip1_permissions.h',
        'dispatch': 'src/uwvm2/uwvm/cmdline/callback/wasip1_module_common.h',
        'noinherit': 'src/uwvm2/uwvm/cmdline/params/wasip1_global_noinherit_system_environment.h',
        'registration': 'src/uwvm2/uwvm/cmdline/params.h',
        'parameter_header': 'src/uwvm2/uwvm/cmdline/params/impl.h',
        'callback_header': 'src/uwvm2/uwvm/cmdline/callback/impl.h',
        'parameter_module': 'src/uwvm2/uwvm/cmdline/params/impl.cppm',
        'callback_module': 'src/uwvm2/uwvm/cmdline/callback/impl.cppm',
        'parameter_partition': 'src/uwvm2/uwvm/cmdline/params/wasip1_permissions.cppm',
        'callback_partition': 'src/uwvm2/uwvm/cmdline/callback/wasip1_permissions.cppm',
        'checkpoint_world': 'src/uwvm2/runtime/lib/uwvm_runtime_checkpoint_world_wasip1.h',
        'checkpoint_restore': 'src/uwvm2/runtime/lib/uwvm_runtime_wasip1_environment_restore.h',
        'fixture': 'tests/wasip1_default_deny/configuration.cc',
        'checker': 'tests/wasip1_default_deny/check_configuration_sources.py',
    }
    sources = {key: (root / path).read_text(encoding='utf-8-sig') for key, path in paths.items()}
    checks: list[dict[str, object]] = []

    def check(name: str, condition: bool) -> None:
        checks.append({'name': name, 'status': 'PASS' if condition else 'FAIL'})

    expected = {
        'clock_realtime': 1, 'clock_monotonic': 2, 'clock_process_cputime': 4,
        'clock_thread_cputime': 8, 'random': 16, 'proc_raise': 32,
        'sched_yield': 64, 'stdio_full_rights': 128,
    }
    enum_match = re.search(r'enum class permission_t[^\{]*\{([^}]+)\}', sources['environment'])
    found = {} if enum_match is None else {
        name: int(value) for name, value in re.findall(r'(\w+)\s*=\s*(\d+)u', enum_match.group(1))
    }
    check('finite eight independent permission bits', found == expected)
    check('concrete environment has empty default authority', 'permission_t non_fd_permissions{};' in sources['environment'])
    check('zero requested authority is never allowed', 'requested != permission_t{} && (non_fd_permissions & requested) == requested' in sources['environment'])
    check('Global environment inheritance default is disabled', 'inline bool wasip1_noinherit_system_environment{true};' in sources['storage'])
    check('Global environment option presence defaults unset', 'inline bool wasip1_noinherit_system_environment_is_set{};' in sources['storage'])
    check('explicit noinherit uses separate marker', 'storage::wasip1_noinherit_system_environment_is_set)' in sources['noinherit'])
    check('explicit noinherit has a value callback', 'details::wasip1_global_noinherit_system_environment_callback)' in sources['noinherit'])
    check('explicit inherit shares the marker', 'storage::wasip1_noinherit_system_environment_is_set)' in sources['parameters'])
    check('inherit and noinherit set opposite policy values',
          'storage::wasip1_noinherit_system_environment = false;' in sources['callbacks'] and
          'storage::wasip1_noinherit_system_environment = true;' in sources['callbacks'])
    check('target permission presence activates override', 'this->permission_presence !=' in sources['storage'])
    check('initializer calls shared permission merge', 'state.env.non_fd_permissions = effective_wasip1_permissions(' in sources['initializer'])
    check('ordinary stdio uses directional projection', 'new_fd_fd.rights_base = env.stdio_base_rights(input);' in sources['initializer'])
    check('ordinary stdio uses explicit inheriting projection', 'new_fd_fd.rights_inherit = env.stdio_inheriting_rights();' in sources['initializer'])
    check('stdin and output streams select correct direction',
          'init_stdio(*fd0.fd_p, ::fast_io::in(), true)' in sources['initializer'] and
          'init_stdio(*fd1.fd_p, ::fast_io::out(), false)' in sources['initializer'] and
          'init_stdio(*fd2.fd_p, ::fast_io::err(), false)' in sources['initializer'])
    check('debugger keeps stricter stdio overrides', sources['initializer'].count('rights_inherit = {};') >= 2 and 'fd0.fd_p->rights_inherit={};' in sources['initializer'])
    check('basic rights have no ambient derivation',
          'rights::right_fd_filestat_get | rights::right_poll_fd_readwrite |' in sources['environment'] and
          '(input ? rights::right_fd_read : rights::right_fd_write)' in sources['environment'])
    check('basic inheriting rights are zero', 'static_cast<rights>(-1) : rights{};' in sources['environment'])
    for scope in ('global', 'single', 'group'):
        for operation in ('grant', 'deny'):
            name = f'wasip1_{scope}_{operation}'
            check(f'{scope} {operation} CLI parameter is declared', f'.name{{u8"--wasip1-{scope}-{operation}"}}' in sources['parameters'])
            check(f'{scope} {operation} callback is defined', f'{name}_callback(' in sources['callbacks'])
            check(f'{scope} {operation} is registered', f'params::{name})' in sources['registration'])
    check('global per-bit duplicate/conflict is checked', '(storage::wasip1_global_permission_presence & bit) != permission{}' in sources['callbacks'])
    check('target per-bit duplicate/conflict is checked', '(target.permission_presence & bit) != permission{}' in sources['dispatch'])
    check('single and group use the common dispatcher',
          'wasip1_single_details::apply_action' in sources['callbacks'] and 'wasip1_group_details::apply_action' in sources['callbacks'])
    check('unknown permission is rejected before presence mutation',
          sources['callbacks'].index('if(bit == permission{})') < sources['callbacks'].index('storage::wasip1_global_permission_presence ='))
    for key in ('parameter_header', 'callback_header'):
        check(f'{key} includes new commands', '# include "wasip1_permissions.h"' in sources[key])
    for key in ('parameter_module', 'callback_module'):
        check(f'{key} exports new partition', 'export import :wasip1_permissions;' in sources[key])
    check('module callback imports both scoped dispatchers and shared dispatcher',
          all(f'import :wasip1_{kind};' in sources['callback_partition'] for kind in ('module_common', 'single_common', 'group_common')))
    check('native fixture exists and calls actual callbacks', 'invoke(callbacks::wasip1_single_deny_callback' in sources['fixture'])
    check('private checkpoint world retains current trusted permission policy',
          'next.non_fd_permissions = original.non_fd_permissions;' in sources['checkpoint_world'])
    check('same-object checkpoint publication does not overwrite live permission policy',
          'non_fd_permissions' not in sources['checkpoint_restore'])

    # Extract only the pure expression from the C++ helper. Parse it using the
    # Python AST and allow only names and bitwise operators before evaluation.
    # Expected values use independent sets, rather than restating the mask rule.
    match = re.search(r'effective_wasip1_permissions\s*\([^)]*\)\s*noexcept\s*\{\s*return\s*([^;]+);\s*\}', sources['storage'], re.S)
    expression = '' if match is None else match.group(1).strip().replace('global', 'global_value')
    check('merge expression is a bounded bitwise projection', expression == '(global_value & ~presence) | (target & presence)')
    evaluations = 0
    model_failure = None
    if expression:
        tree = ast.parse(expression, mode='eval')
        allowed = (ast.Expression, ast.BinOp, ast.UnaryOp, ast.BitAnd, ast.BitOr, ast.Invert, ast.Name, ast.Load)
        if any(not isinstance(node, allowed) for node in ast.walk(tree)):
            raise ValueError('unsupported source expression')
        code = compile(tree, '<extracted C++ policy expression>', 'eval')
        bit_sets = [{bit for bit in expected.values() if mask & bit} for mask in range(256)]
        presence_values = [0, 255, *expected.values()]
        for global_value in range(256):
            for target in range(256):
                for presence in presence_values:
                    actual = eval(code, {'__builtins__': {}}, {'global_value': global_value, 'target': target, 'presence': presence})
                    selected = (bit_sets[global_value] - bit_sets[presence]) | (bit_sets[target] & bit_sets[presence])
                    expected_value = sum(selected)
                    evaluations += 1
                    if actual != expected_value:
                        model_failure = {'global': global_value, 'target': target, 'presence': presence, 'actual': actual, 'expected': expected_value}
                        break
                if model_failure is not None:
                    break
            if model_failure is not None:
                break
    check('extracted merge expression agrees with independent set selection', evaluations == 655360 and model_failure is None)
    report = {
        'status': 'PASS' if all(row['status'] == 'PASS' for row in checks) else 'FAIL',
        'scope': 'Source inspection and Python bit-mask model only; no native compilation, Guest execution, or ROS/cgroup qualification',
        'source_checks': checks,
        'model_evaluations': evaluations,
        'model_failure': model_failure,
        'compiled_native_configuration_fixture': 'PENDING_LINUX_CGROUP',
        'source_hashes': {path: hashlib.sha256((root / path).read_bytes()).hexdigest() for path in paths.values()},
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open('x', encoding='utf-8') as stream:
        json.dump(report, stream, indent=2)
        stream.write('\n')
    print(json.dumps({'status': report['status'], 'source_checks': len(checks), 'model_evaluations': evaluations, 'report': str(args.output.resolve())}))
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
