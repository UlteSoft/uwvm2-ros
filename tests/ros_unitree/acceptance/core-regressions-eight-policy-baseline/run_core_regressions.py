#!/usr/bin/env python3
"""Run eight harness policy tests and seven SDK-free ticket lifetime groups.

Root invokes this entry from a complete current source freeze, with the same
closed native compiler configuration accepted by build_interpreter_workers.py.
It imports no project code until its own bytes, manifest and actual original
64 GiB / zero-swap cgroup have been admitted using only the Python standard
library. All commands are serial and retained by the existing pidfd owner.
This entry executes no ROS SDK, Master, Gateway, Worker, Guest or simulator.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import stat
import sys
import time
import traceback
import types

sys.dont_write_bytecode = True
MIB, GIB = 1024**2, 1024**3
LIMITS = {'jobs': 1, 'rss_bytes': 4 * GIB, 'command_seconds': 600, 'log_bytes': 16 * MIB}
CGROUP = {'memory_max', 'memory_swap_max', 'expected_leaf', 'expected_device',
          'expected_inode', 'expected_boot_id', 'expected_cpus'}
EXPECTED = (b'PASS ticket-lifetime groups=7 completed=1048608 queue_rejected=1048608 '
            b'exact_uint8_nonzero_domain=255 parallel_ids=131072 parallel_Service_ids=8192 '
            b'no_ROS_or_Guest_execution\n')
SUITES = (
    ('output-budget-policy', 'tests/ros_unitree/test_output_budget_guard.py', 5),
    ('actor-ledger-policy', 'tests/ros_unitree/test_actor_retirement_ledgers.py', 3),
)


def need(value, reason):
    if not value:
        raise ValueError(reason)


def pairs(items):
    result = {}
    for key, value in items:
        need(key not in result, 'duplicate JSON key: ' + key)
        result[key] = value
    return result


def read(path, limit):
    """Open bounded ordinary stable bytes before loading any project helper."""
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute control required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(descriptor)
        need(stat.S_ISREG(first.st_mode) and 0 < first.st_size <= limit, 'bounded nonempty ordinary control required')
        body = bytearray()
        while True:
            data = os.read(descriptor, min(65536, limit + 1 - len(body)))
            if not data:
                break
            body.extend(data)
            need(len(body) <= limit, 'control grew beyond bound')
        last, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(body) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'opened control changed')
        return bytes(body), {'path': str(path), 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest(),
            'device': first.st_dev, 'inode': first.st_ino, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}
    finally:
        os.close(descriptor)


def document(path, limit=16 * MIB):
    raw, pin = read(path, limit)
    return json.loads(raw, object_pairs_hook=pairs,
        parse_constant=lambda _: need(False, 'nonfinite JSON')), pin


def module(source, manifest, relative, name):
    raw, pin = read(source / relative, MIB)
    need(manifest.get(relative) == {key: pin[key] for key in ('bytes', 'sha256')},
         'actual current helper absent from complete manifest: ' + relative)
    result = types.ModuleType(name)
    result.__file__ = str(source / relative)
    sys.modules[name] = result
    exec(compile(raw, result.__file__, 'exec'), result.__dict__)
    return result, pin


def raw_current(cgroup):
    """Check the selected live boundary with stdlib before project imports."""
    need(type(cgroup) is dict and set(cgroup) == CGROUP, 'closed actual current cgroup required')
    need(type(cgroup['memory_max']) is int and cgroup['memory_max'] == 64 * GIB and
         type(cgroup['memory_swap_max']) is int and cgroup['memory_swap_max'] == 0,
         'original hard64GiB and zero swap required')
    need(type(cgroup['expected_leaf']) is str and type(cgroup['expected_boot_id']) is str and
         type(cgroup['expected_cpus']) is str and
         all(type(cgroup[key]) is int and cgroup[key] > 0 for key in ('expected_device', 'expected_inode')),
         'actual closed cgroup observations required')
    leaf = Path(cgroup['expected_leaf'])
    need(leaf.is_absolute() and leaf.resolve(strict=True) == leaf and leaf.is_relative_to('/sys/fs/cgroup'),
         'canonical actual cgroup leaf required')
    state = leaf.stat()
    need((state.st_dev, state.st_ino) == (cgroup['expected_device'], cgroup['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip() == cgroup['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::' + str(leaf).removeprefix('/sys/fs/cgroup')] and
         (leaf / 'memory.max').read_text().strip() == str(64 * GIB) and
         (leaf / 'memory.swap.max').read_text().strip() == '0' and
         (leaf / 'cpuset.cpus.effective').read_text().strip() == cgroup['expected_cpus'],
         'coordinator is outside the exact selected current64GiB/zero-swap cgroup')


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('source', 'manifest', 'config', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    for name in ('manifest-sha256', 'config-sha256'):
        parser.add_argument('--' + name, required=True)
    args = parser.parse_args()
    need(sys.platform == 'linux' and hasattr(os, 'pidfd_open'), 'actual native Linux pidfd owner required')
    need(all(getattr(args, name).is_absolute() for name in ('source', 'manifest', 'config', 'out')),
         'absolute explicit arguments required')
    need(all(re.fullmatch('[0-9a-f]{64}', getattr(args, name)) for name in ('manifest_sha256', 'config_sha256')),
         'actual manifest and configuration hashes required')
    source = args.source.resolve(strict=True)
    need(Path(__file__).absolute() == source / 'tests/ros_unitree/run_core_regressions.py',
         'execute the maintained entry from the actual complete current freeze')
    frozen, manifest_pin = document(args.manifest)
    need(type(frozen) is dict and frozen and manifest_pin['sha256'] == args.manifest_sha256,
         'complete current source manifest binding differs')
    own_raw, own_pin = read(Path(__file__).absolute(), MIB)
    need(frozen.get('tests/ros_unitree/run_core_regressions.py') ==
         {key: own_pin[key] for key in ('bytes', 'sha256')}, 'entry missing from actual complete manifest')
    config, config_pin = document(args.config)
    need(type(config) is dict and config_pin['sha256'] == args.config_sha256, 'actual compiler configuration differs')
    raw_current(config['cgroup'])
    probe, probe_pin = module(source, frozen, 'tests/ros_cgroup/probe.py', 'core_regressions_probe')
    common, common_pin = module(source, frozen, 'tools/ros_distro/common.py', 'core_regressions_owned_primitives')
    common._CGROUP_PROBE = probe
    builder, builder_pin = module(source, frozen, 'tests/ros_debugger/build_runtime.py', 'core_regressions_native_readers')
    worker, worker_pin = module(source, frozen, 'tests/ros_unitree/build_interpreter_workers.py', 'core_regressions_native_configuration')
    roots, tools, selections, headers, gcc = worker.configuration(config, source, builder)
    need(not any(selections.values()) and not any(headers.values()), 'SDK-free lane accepts no ROS registry or profile flags')
    out = args.out
    need(out.parent.resolve(strict=True) == out.parent and not out.exists() and not out.is_symlink() and
         all(not worker.inside(out, [root]) and not worker.inside(root, [out]) for root in [source, *roots]),
         'fresh disjoint ordinary owned output required')
    out.mkdir(mode=0o700)
    for name in ('home', 'tmp'):
        (out / name).mkdir(mode=0o700)
    cancelled = common.install_interrupts()
    started, next_output = time.monotonic(), 0.0
    active_seconds = min(config['build_seconds'], 1800)
    finalizing, before, source_before, runner = False, None, None, None
    inputs, products, root_before = {}, {}, []
    aliases = {name: worker.aliases(path) for name, path in tools.items()}
    report = {'format_version': 1, 'kind': 'unitree-core-policy-and-sdk-free-ticket-regressions', 'status': 'RUNNING',
        'source': str(source), 'source_manifest_sha256': manifest_pin['sha256'], 'configuration': config_pin,
        'commands': [], 'policy_suites': [], 'parallelism': 1, 'limits': LIMITS,
        'native_execution_scope': 'SDK_FREE_SHARED_SERVICE_AND_TICKET_CONTRACT_ONLY',
        'mapped_stop_qualification': 'NOT_PERFORMED_NO_LIVE_MAPPING_CLAIM',
        'ROS_executed': False, 'Runtime_executed': False, 'Guest_executed': False,
        'simulation_executed': False, 'Guest_rights_changed': False}

    def remember(path, expected=None):
        pin = common.opened(Path(path).resolve(strict=True))
        need(expected is None or pin == expected, 'parsed/control input changed')
        need(pin['path'] not in inputs or inputs[pin['path']] == pin, 'observed input changed')
        inputs[pin['path']] = pin
        return pin

    def product(path, limit=512 * MIB):
        pin = common.opened(Path(path), limit)
        need(pin['path'] not in products or products[pin['path']] == pin, 'owned product changed')
        products[pin['path']] = pin
        return pin

    def storage():
        for path in (out, *out.parents):
            facts = os.statvfs(path)
            need(facts.f_bavail * facts.f_frsize >= 8 * GIB, 'active output/backing reserve below8GiB')

    def output_budget():
        total = 0
        for folder, directories, names in os.walk(out, followlinks=False):
            for name in [*directories, *names]:
                path = Path(folder) / name
                try:
                    info = path.lstat()
                except FileNotFoundError:
                    # Tests own TemporaryDirectory below the selected TMPDIR;
                    # its intentional fixture retirement can race this active
                    # output-only scan. Immutable inputs never use this rule.
                    need(not finalizing and (name.endswith('.json.pending') or path.is_relative_to(out / 'tmp')),
                         'owned output unexpectedly disappeared')
                    continue
                if stat.S_ISLNK(info.st_mode):
                    # The alias-refusal policy test deliberately creates one
                    # alias between two of its own temporary fixture paths.
                    # Retain its literal size without following it as a file;
                    # no selected source/SDK or host target gains authority.
                    need(not finalizing and path.is_relative_to(out / 'tmp'), 'unexpected owned output alias')
                    try:
                        target = Path(os.readlink(path))
                    except FileNotFoundError:
                        continue
                    target = target if target.is_absolute() else path.parent / target
                    need(target.resolve(strict=False).is_relative_to(out / 'tmp'), 'temporary policy alias escapes owned TMPDIR')
                    total += info.st_size
                    need(total <= GIB, 'finite one-GiB output budget exhausted')
                    continue
                need(stat.S_ISDIR(info.st_mode) or stat.S_ISREG(info.st_mode), 'owned output contains alias or special node')
                if stat.S_ISREG(info.st_mode):
                    total += info.st_size
                need(total <= GIB, 'finite one-GiB output budget exhausted')

    def guard():
        nonlocal next_output
        need(finalizing or not cancelled['cancelled'], 'cooperative cancellation requested')
        need(time.monotonic() - started <= active_seconds + (600 if finalizing else 0),
             'finite regression/finalization deadline exhausted')
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= GIB, 'coordinator metadata RSS exceeds1GiB')
        current = common.cgroup_state(config['cgroup'])
        if before is not None:
            common.cgroup_preserved(before, current)
        need(all(worker.aliases(tools[name]) == old for name, old in aliases.items()), 'compiler/tool invocation alias changed')
        if not finalizing:
            storage()
            if time.monotonic() >= next_output:
                output_budget()
                next_output = time.monotonic() + .5
        return current

    def save():
        report['commands'] = runner.commands if runner is not None else []
        report['input_pins'], report['product_pins'] = list(inputs.values()), list(products.values())
        common.atomic_json(out / 'report.json', report)

    environment = {**config['environment'], 'HOME': str(out / 'home'), 'TMPDIR': str(out / 'tmp'),
        'PYTHONDONTWRITEBYTECODE': '1', 'MAKEFLAGS': '-j1', 'OMP_NUM_THREADS': '1', 'OPENBLAS_NUM_THREADS': '1'}

    def run(label, argv):
        try:
            row = runner.run(label, list(map(str, argv)), out, environment)
            runner.discover()
            terminal = []
            for key, item in runner.owned.items():
                need(common.pidfd_terminal(item['fd']), 'owned descendant remains live between serial commands')
                terminal.append({'birth': item['identity'], 'after': common.identity(key[0]), 'pidfd_terminal': True})
            for key in runner.unproven:
                actual = common.identity(key[0])
                need(common.birth(actual) != key or actual['state'] in ('Z', 'X'), 'unproven live descendant remains')
            for key, item in list(runner.owned.items()):
                os.close(item['fd'])
                runner.owned.pop(key)
            row['owned_terminal_barrier'] = terminal
            return row
        finally:
            if runner.commands and 'log' in runner.commands[-1]:
                row = runner.commands[-1]
                need(product(row['log']['path'], LIMITS['log_bytes']) == row['log'], 'original command log changed')
            save()

    try:
        before = guard()
        report['cgroup_before'] = before
        need(all(row['events'].get(key, 0) == 0 for row in before['ancestors']
                 for key in ('oom', 'oom_kill', 'oom_group_kill')), 'initial OOM counter is nonzero')
        leaf = Path(config['cgroup']['expected_leaf'])
        need(int((leaf / 'memory.current').read_text()) + LIMITS['rss_bytes'] + 2 * GIB < 60 * GIB,
             'actual compiler and supervisor memory headroom unavailable')
        source_before = common.source_state(source, args.manifest)
        need(source_before['manifest'] == manifest_pin, 'whole initial current manifest observation changed')
        report['source_before'] = source_before
        for pin in (manifest_pin, config_pin, own_pin, probe_pin, common_pin, builder_pin, worker_pin):
            remember(pin['path'], pin)
        for name, path in tools.items():
            remember(path)
            builder.elf(common, Path(path).resolve(strict=True), config['target'])
        report['tools'] = {name: remember(path) for name, path in tools.items()}
        report['selected_aliases_before'], report['gcc_install_dir'] = aliases, str(gcc)
        for index, root in enumerate(roots):
            guard()
            item = common.catalog(root, out / ('inputs-before-%02d.jsonl' % index), roots)
            root_before.append(item)
            product(item['catalog']['path'])
            report['input_roots_before'] = root_before
            save()
        runner = common.SerialRunner(out, LIMITS, guard)
        row = run('compiler-version', [tools['cxx'], '--driver-mode=g++', '--version'])
        body, _ = common.body_pin(Path(row['log']['path']), LIMITS['log_bytes'])
        need(re.search(rb'\bclang version 23(?:\.|\s)', body), 'actual selected compiler is not Clang23')
        for label, relative, expected in SUITES:
            pin = remember(source / relative)
            need(frozen[relative] == {key: pin[key] for key in ('bytes', 'sha256')}, 'policy suite not current')
            row = run(label, [tools['python'], '-I', '-B', source / relative, '-v'])
            body, _ = common.body_pin(Path(row['log']['path']), LIMITS['log_bytes'])
            need(re.search(rb'\nRan ' + str(expected).encode() + rb' tests in [0-9.]+s\n\nOK\n\Z', body) and
                 body.count(b' ... ok\n') == expected, 'exact complete actual unittest count differs')
            report['policy_suites'].append({'suite': relative, 'source': pin, 'actual_tests': expected,
                'command_index': len(runner.commands) - 1, 'status': 'PASS_ACTUAL_POLICY_TESTS_ONLY'})
            save()
        primary = source / 'tests/ros_ticket_lifetime/contract.cc'
        obj, dep, binary = out / 'contract.o', out / 'contract.d', out / 'contract'
        flags = ['--driver-mode=g++', '--target=' + config['target']['triple'], '-std=c++20', '-O2', '-g0',
            '-UNDEBUG', '-pthread', '-fno-fast-math', '-ffp-contract=off', *config['cxxflags'],
            '-I', str(source / 'src'), '-I', str(source / 'third-parties/fast_io/include')]
        if config['sysroot']:
            flags.append('--sysroot=' + config['sysroot'])
        compile_row = run('ticket-contract-compile', [tools['cxx'], *flags,
            '-MD', '-MF', dep, '-MT', obj, '-c', primary, '-o', obj])
        dep_pin, dependencies = builder.dependency_inputs(common, dep, obj, source, roots)
        for pin in dependencies:
            remember(pin['path'], pin)
        required = [primary, source / 'gateways/common/service.h', source / 'tests/ros_ipc/probe_common.h',
                    source / 'third-parties/fast_io/include/fast_io.h']
        need({str(path) for path in required} <= {pin['path'] for pin in dependencies},
             'actual full-MD omitted primary/shared Service/probe or C++20 fast_io header')
        object_pin = builder.elf(common, obj, config['target'], (1,))
        product(obj)
        product(dep)
        link = [tools['cxx'], '--driver-mode=g++', '--target=' + config['target']['triple'], *config['link_flags'],
            '-fuse-ld=' + tools['linker'], '-pthread', '-Wl,--threads=1', '-Wl,--trace', '-Wl,--no-undefined']
        if config['sysroot']:
            link.append('--sysroot=' + config['sysroot'])
        link += [str(obj), '-o', str(binary)]
        link_row = run('ticket-contract-link', link)
        body, _ = common.body_pin(Path(link_row['log']['path']), LIMITS['log_bytes'])
        linked, archives = {}, {}
        for line in body.decode('utf-8', 'strict').splitlines():
            if not line.strip():
                continue
            path = Path(line.split('(', 1)[0])
            path = path if path.is_absolute() else out / path
            path = path.resolve(strict=True)
            need(path.is_file() and worker.inside(path, [out, *roots]), 'link trace escaped complete selected inputs')
            builder.no_extra_llvm([str(path)], 'SDK-free ticket link input')
            if path.suffix == '.a':
                _, archives[str(path)] = builder.archive(common, path, config['target'])
            pin = product(path) if worker.inside(path, [out]) else remember(path)
            linked[pin['path']] = pin
        need(str(obj) in linked and any(Path(path).name.startswith('libstdc++') for path in linked),
             'actual trace omitted the owned object or selected GCC standard library')
        binary_pin = builder.elf(common, binary, config['target'])
        product(binary)
        need(os.access(binary, os.X_OK), 'fresh SDK-free ticket executable is not executable')
        execution = run('ticket-contract-run', [binary])
        body, _ = common.body_pin(Path(execution['log']['path']), LIMITS['log_bytes'])
        need(body == EXPECTED, 'actual complete seven-group ticket output differs')
        report['ticket_contract'] = {'status': 'PASS_ACTUAL_SDK_FREE_SEVEN_GROUPS', 'primary': remember(primary),
            'object': object_pin, 'dependency_file': dep_pin, 'dependencies': dependencies,
            'compile_argv': compile_row['argv'], 'compile_cwd': compile_row['cwd'], 'link_argv': link,
            'link_inputs': list(linked.values()), 'archive_members': archives, 'binary': binary_pin,
            'elf': {'machine': 62, 'bits': 64, 'endian': 'little'}, 'standard': 'c++20',
            'fixture_output_branch': 'ACTUAL_MD_INCLUDES_FAST_IO_HEADER_AND_CXX20',
            'actual_groups': 7, 'expected_stdout': EXPECTED.decode(), 'execution_command_index': len(runner.commands) - 1}
        need(sum(row['actual_tests'] for row in report['policy_suites']) == 8, 'both complete policy suites required')
        report['all_requested_checks_completed'] = True
    except BaseException as error:
        report.update(status='FAIL', error=repr(error), traceback=traceback.format_exc())
    finally:
        finalizing = True
        errors = []
        def finish(label, action):
            try:
                action()
            except BaseException as error:
                errors.append({'stage': label, 'error': repr(error)})
                report['status'] = 'FAIL'
        def cleanup():
            report['cleanup'] = runner.close() if runner is not None else {'status': 'PASS', 'remaining': [], 'actors_started': 0}
            need(report['cleanup']['status'] == 'PASS' and not report['cleanup']['remaining'], 'owned command cleanup incomplete')
        finish('owned-pidfd-cleanup', cleanup)
        if runner is not None:
            for row in runner.commands:
                if 'log' in row:
                    finish('retain-command-log', lambda row=row: product(row['log']['path'], LIMITS['log_bytes']))
            finish('retain-command-ledger', lambda: product(out / 'commands.json', 32 * MIB))
        def preserve():
            need(source_before is not None and len(root_before) == len(roots), 'complete initial source/root proof unavailable')
            report['source_after'] = common.source_state(source, args.manifest)
            need(report['source_after'] == source_before, 'complete current frozen source changed')
            after = []
            for index, root in enumerate(roots):
                guard()
                item = common.catalog(root, out / ('inputs-after-%02d.jsonl' % index), roots)
                after.append(item)
                product(item['catalog']['path'])
                need({key: value for key, value in item.items() if key != 'catalog'} ==
                     {key: value for key, value in root_before[index].items() if key != 'catalog'},
                     'complete selected input root changed')
            report['input_roots_after'] = after
            need(all(common.opened(Path(pin['path'])) == pin for pin in [*inputs.values(), *products.values()]),
                 'original observed input or produced byte stream changed')
            need(aliases == {name: worker.aliases(path) for name, path in tools.items()}, 'selected invocation alias changed')
            report.update(source_preserved=True, inputs_preserved=True, products_preserved=True)
            report['cgroup_after'] = guard()
        finish('full-source-input-product-preservation', preserve)
        finish('fresh-terminal-output-budget', output_budget)
        # Reserve failure still fails acceptance, but cannot skip the earlier
        # owned cleanup or whole-input preservation stages.
        finish('fresh-terminal-eight-GiB-backing-reserve', storage)
        report['finalization_errors'] = errors
        if report.get('all_requested_checks_completed') and report['status'] == 'RUNNING' and not errors:
            report['status'] = 'PASS_EIGHT_POLICY_TESTS_AND_SEVEN_SDK_FREE_TICKET_GROUPS'
        save()
    return 0 if report['status'] == 'PASS_EIGHT_POLICY_TESTS_AND_SEVEN_SDK_FREE_TICKET_GROUPS' else 1


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except Exception:
        traceback.print_exc()
        raise SystemExit(1)
