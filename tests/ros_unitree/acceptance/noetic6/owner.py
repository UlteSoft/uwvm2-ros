#!/usr/bin/env python3
"""Own one explicitly staged Noetic r6 acquisition in the current 64 GiB leaf.

The owner validates current pins and the exact reviewed r4-to-r6 spec delta,
then starts one serial producer. It never installs packages, runs target code,
tests ROS, or promotes the interrupted r5 SDK. On interruption it gives only its
same-birth direct coordinator a bounded TERM grace period before the frozen
child-first pidfd cleanup. No process group or numeric-PID signal is used.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import signal
import stat
import sys
import time
import traceback
import types

MIB, GIB = 1024 ** 2, 1024 ** 3
P = Path('/var/tmp/uwvm-ros-complete-20261015-r1')
M = P / 'acceptance-storage-r1'; B = Path('/var/tmp/uwvm-ros-unitree-20261016-r1')
S = P / 'inputs-r35/source'; MF = S.parent / 'source-manifest.json'
MH = 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
CODE = B / 'noetic-amd64-native-sdk-acquisition-controls-r6'
OLD_CODE = B / 'noetic-amd64-native-sdk-acquisition-controls-r4'
TARGET = M / 'noetic-amd64-native-sdk-acquisition-r6'
OUT = M / 'noetic-amd64-native-sdk-acquisition-root-supervisor-r6'
R4 = M / 'noetic-amd64-native-sdk-acquisition-r4'
CG = None
CG_KEYS = {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode', 'expected_boot_id', 'expected_leaf', 'expected_cpus'}
ORIGINAL_LEAF = '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'
R5 = M / 'noetic-amd64-native-sdk-acquisition-r5'

BUDGET = {'self_rss_bytes': 512 * MIB, 'tree_rss_bytes': GIB, 'command_seconds': 43800,
          'active_seconds': 44400, 'whole_seconds': 46800, 'cleanup_seconds': 600,
          'reserve_bytes': 8 * GIB}
NAMES = {'producer': 'acquire-sdk.py', 'resume': 'fresh_rebuild.py', 'model': 'signed_tar_model.py',
         'storage': 'owned_storage.py', 'restart': 'requalify_after_restart.py', 'owner': 'launcher.py'}
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}


def need(value, message):
    if not value: raise ValueError(message)


def pairs(rows):
    result = {}
    for key, value in rows:
        need(key not in result, 'duplicate owner JSON key'); result[key] = value
    return result


def read(path, limit=16 * MIB):
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute metadata required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= limit, 'bounded regular owner input required')
        body = bytearray()
        while True:
            block = os.read(fd, min(MIB, limit + 1 - len(body)))
            if not block: break
            body.extend(block); need(len(body) <= limit, 'owner input exceeded bound')
        last, named = os.fstat(fd), path.stat(follow_symlinks=False)
        need(len(body) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')), 'owner input changed')
        return bytes(body), {'path': str(path), 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest(),
            'device': first.st_dev, 'inode': first.st_ino, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}
    finally: os.close(fd)


def parse(raw):
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite owner JSON'))


def current_context(value):
    """Validate Root's SHA-bound current selection before project helper load.

    Only the boot and physical cgroup identity may vary between explicitly
    selected incarnations of the same task container. Memory, zero swap, CPU
    selection and the exact original leaf spelling remain mandatory. Nothing
    is inferred from an old producer or converted into execution continuity.
    """
    need(type(value) is dict and set(value) == CG_KEYS and
         value['memory_max'] == 64 * GIB and value['memory_swap_max'] == 0 and
         type(value['memory_max']) is int and type(value['memory_swap_max']) is int and
         value['expected_leaf'] == ORIGINAL_LEAF and value['expected_cpus'] == '0-31' and
         all(type(value[key]) is int and value[key] > 0 for key in ('expected_device', 'expected_inode')) and
         type(value['expected_boot_id']) is str and len(value['expected_boot_id']) == 36,
         'closed explicit current selection of original hard64GiB zero-swap task leaf required')
    leaf = Path(ORIGINAL_LEAF); info = leaf.stat()
    need(leaf.resolve(strict=True) == leaf and
         (info.st_dev, info.st_ino) == (value['expected_device'], value['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip() == value['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::' + ORIGINAL_LEAF.removeprefix('/sys/fs/cgroup')] and
         (leaf / 'memory.max').read_text().strip() == str(64 * GIB) and
         (leaf / 'memory.swap.max').read_text().strip() == '0' and
         (leaf / 'cpuset.cpus.effective').read_text().strip() == '0-31',
         'Root selected current boot/leaf physical facts or hard resource limits differ')
    return dict(value)


def main():
    global CG
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--control', type=Path, required=True); parser.add_argument('--control-sha256', required=True)
    args = parser.parse_args(); started = time.monotonic()
    need(sys.platform == 'linux' and os.geteuid() == 0 and hasattr(os, 'pidfd_open'), 'Linux Root pidfd owner required')
    raw, control_pin = read(args.control); control = parse(raw)
    need(control_pin['sha256'] == args.control_sha256 and type(control) is dict, 'Root selected current control SHA differs')
    CG = current_context(control.get('cgroup'))
    need(control_pin['sha256'] == args.control_sha256 and set(control) == {
        'format_version', 'kind', 'source', 'manifest', 'manifest_sha256', 'manifest_pin', 'cgroup', 'code_root',
        'actor_out', 'owner_out', 'python', 'helpers', 'spec', 'cache', 'resume_control', 'original_spec',
        'original_producer', 'infrastructure_pins', 'resolver_alias', 'budgets', 'stage_request', 'source_package'} and
        control['format_version'] == 1 and control['kind'] == 'noetic-r6-current-root-acquisition-control',
        'closed exact reviewed owner control required')
    need(control['source'] == str(S) and control['manifest'] == str(MF) and control['manifest_sha256'] == MH and
         control['cgroup'] == CG and control['code_root'] == str(CODE) and control['actor_out'] == str(TARGET) and
         control['owner_out'] == str(OUT) and control['budgets'] == BUDGET and Path(__file__) == CODE / NAMES['owner'],
         'exact fresh r6 paths, current cgroup and bounded acquisition scope required')
    raw, mf_pin = read(MF); frozen = parse(raw)
    need(mf_pin == control['manifest_pin'] and mf_pin['sha256'] == MH and mf_pin['bytes'] == 3397553 and
         len(frozen) == 17882, 'actual complete current Source35 manifest required')
    bootstrap = [control_pin, mf_pin]; bodies = {}
    for relative in ('tools/ros_distro/common.py', 'tests/ros_cgroup/probe.py'):
        body, pin = read(S / relative, MIB)
        need(frozen[relative] == {key: pin[key] for key in ('bytes', 'sha256')}, 'frozen owner/probe differs')
        bodies[relative] = body; bootstrap.append(pin)
    common = types.ModuleType('noetic_r6_root_frozen_common'); common.__file__ = str(S / 'tools/ros_distro/common.py')
    sys.modules[common.__name__] = common
    exec(compile(bodies['tools/ros_distro/common.py'], common.__file__, 'exec'), common.__dict__)
    common.install_interrupts(); before = common.cgroup_state(CG)
    need(all(row['events'].get(key, 0) == 0 for row in before['ancestors'] for key in
         ('oom', 'oom_kill', 'oom_group_kill')), 'zero current ancestor OOM counters required')
    pins = {}; products = {}; runner = None; finalizing = False; roots = []; catalogs = []; source_before = None
    need(not OUT.exists() and not OUT.is_symlink() and OUT.parent.resolve(strict=True) == OUT.parent and
         not TARGET.exists() and not TARGET.is_symlink(), 'fresh r6 owner and actor outputs required')
    OUT.mkdir(mode=0o700)
    report = {'format_version': 1, 'kind': 'noetic-r6-root-owned-fresh-all513-rebuild', 'status': 'RUNNING',
        'source': str(S), 'source_manifest_sha256': MH, 'control': control_pin, 'cgroup_before': before,
        'commands': [], 'errors': [], 'compiler_executed': False, 'target_program_executed': False,
        'compatibility_qualified': False, 'old_failed_status_promoted': False, 'Guest_authority': 'UNCHANGED_EMPTY'}

    def guard():
        elapsed = time.monotonic() - started
        need(elapsed < BUDGET['whole_seconds'] and (finalizing or elapsed < BUDGET['active_seconds']), 'bounded owner deadline exceeded')
        now = common.cgroup_state(CG); common.cgroup_preserved(before, now)
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= BUDGET['self_rss_bytes'], 'owner self RSS exceeded')
        if not finalizing:
            for path in (OUT, *OUT.parents):
                space = os.statvfs(path)
                need(space.f_bavail * space.f_frsize >= BUDGET['reserve_bytes'], 'active owner backing-filesystem reserve exhausted')
        return now

    def remember(expected):
        need(type(expected) is dict and set(expected) == PIN, 'actual current full7 required')
        actual = common.opened(Path(expected['path']), max(16 * MIB, expected['bytes']))
        need(actual == expected and (actual['path'] not in pins or pins[actual['path']] == actual), 'current owner input changed')
        pins[actual['path']] = actual; return actual

    def product(path, expected=None):
        # The producer permits a signed compressed DEB up to1GiB. Reopening an
        # admitted product must stream its actual bounded size rather than
        # impose the smaller metadata bootstrap limit or collect it in RAM.
        limit = 128 * MIB
        if expected is not None:
            need(type(expected) is dict and set(expected) == PIN and type(expected['bytes']) is int and
                 0 <= expected['bytes'] <= 2 * GIB, 'bounded actual product full7 required')
            limit = max(limit, expected['bytes'])
        actual = common.opened(Path(path), limit)
        need(expected is None or actual == expected, 'owned product changed')
        need(actual['path'] not in products or products[actual['path']] == actual, 'owned product changed after registration')
        products[actual['path']] = actual; return actual

    def save():
        report['input_pins'] = list(pins.values()); report['product_pins'] = list(products.values())
        if runner: report['commands'] = runner.commands
        common.atomic_json(OUT / 'report.json', report)

    def load(role):
        expected = control['helpers'][role]; remember(expected); body, actual = read(expected['path'], MIB)
        need(actual == expected and expected['path'] == str(CODE / NAMES[role]), 'exact staged helper role required')
        module = types.ModuleType('noetic_r6_owner_' + role); module.__file__ = expected['path']; sys.modules[module.__name__] = module
        exec(compile(body, module.__file__, 'exec'), module.__dict__); return module

    save()
    try:
        guard()
        for pin in [*bootstrap, control['stage_request'], control['source_package']]: remember(pin)
        need(set(control['helpers']) == set(NAMES), 'six exact reviewed helper roles required')
        for pin in control['helpers'].values(): remember(pin)
        need(control['helpers']['owner']['path'] == str(Path(__file__)), 'current owner self selected')
        python = remember(control['python'])
        need(Path(sys.executable).resolve(strict=True) == Path(python['path']) == Path('/usr/bin/python3.14') and
             python['sha256'] == 'b8d8288faefdd300201f43fcf00f6f539a27218eeed3a3dff5ab10b9c4c99700', 'exact selected interpreter required')
        producer = load('producer'); restart = load('restart'); load('resume'); load('model'); load('storage')
        documents = {}
        for role, name in (('spec', 'noetic-amd64.spec.json'), ('cache', 'deb-cache-inputs.json'), ('resume_control', 'fresh-rebuild.json')):
            pin = remember(control[role]); body, actual = read(pin['path'])
            need(actual == pin and pin['path'] == str(CODE / name), 'exact current staged document role required')
            documents[role] = parse(body)
        seed_pin = remember(control['original_spec']); seed_raw, _ = read(seed_pin['path']); seed = parse(seed_raw)
        need(seed_pin['path'] == str(OLD_CODE / 'noetic-amd64.spec.json') and seed_pin['bytes'] == 9397 and
             seed_pin['sha256'] == '9e725094c847cfbcd8b98d647de868597ac78c440d5a7cd4d7cc70e42a28be18', 'genuine r4 reviewed seed spec required')
        old_producer = remember(control['original_producer']); old_body, _ = read(old_producer['path'], MIB)
        need(old_producer['path'] == str(OLD_CODE / 'acquire-sdk.py') and old_producer['bytes'] == 85269 and
             old_producer['sha256'] == '17f7676328192a4e914dc82d8dfd65092959aff344426b46d78ad67522dc5c53', 'genuine r4 producer source required')
        new_body, _ = read(CODE / NAMES['producer'], MIB)
        extraction = lambda body: body.split(b'class SafeTree:\n', 1)[1].split(b'\n\ndef main():', 1)[0]
        need(extraction(old_body) == extraction(new_body), 'maintained SafeTree methods changed')
        expected_spec = dict(seed); expected_spec['cgroup'] = CG
        expected_spec['input_roots'] = [*seed['input_roots'], str(CODE), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads'), '/usr/sbin']
        expected_spec['tools'] = {**seed['tools'], 'fstrim': {'path': '/usr/sbin/fstrim', 'sha256':
            '74f99afbf098df6701fdf4bda8d258ca03b1306d131f341f7ec9530681f55b1d'}}
        need(documents['spec'] == expected_spec, 'spec differs beyond current cgroup, four explicit roots and pinned fstrim')
        roots = producer.validate_spec(documents['spec'])
        continuation_control = documents['resume_control']
        need(continuation_control['kind'] == 'noetic-fresh-all513-authenticated-CoW-subset-v1' and
             continuation_control['committed_packages'] == 0 and continuation_control['complete_packages'] == 513 and
             continuation_control['old_report']['path'] == str(R5 / 'report.json') and
             (continuation_control['old_report']['bytes'], continuation_control['old_report']['sha256']) ==
             (6283350, 'fa300972b9639153446ac2005d31944b748f7d3de12cd2f799db5abcb668ceb1') and
             continuation_control['old_outer_report']['sha256'] == '0c32ebee7563a3d8e25c0ad54a115850986beb40410debd36a4d63b18ba9901d' and
             continuation_control['old_ledger']['sha256'] == 'd4d6c432569455916b4fec9bfd1df2c63e7f98e6bb04975001b92526d403f021' and
             continuation_control['old_partial_sdk']['path'] == str(R5 / 'sdk'), 'genuine interrupted r5 content-only provenance and zero continued prefix required')
        for key in ('old_report', 'old_outer_report', 'old_ledger'): remember(continuation_control[key])
        remember(continuation_control['owned_storage']['fstrim'])
        need(documents['cache']['roots'] == [str(M / 'noetic-amd64-native-sdk-acquisition-r1/apt/downloads'), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads')] and
             documents['cache']['required_hit_packages'] == ['ros-noetic-ros-base'], 'exact three retained DEB caches required')
        need(all(not (TARGET == root or TARGET in root.parents or root in TARGET.parents or OUT == root or OUT in root.parents or root in OUT.parents)
                 for root in roots), 'actor/owner outputs must be disjoint from every input root')
        for pin in control['infrastructure_pins']: remember(pin)
        need(producer.alias_binding('/etc/resolv.conf') == control['resolver_alias'], 'current resolver alias changed')
        source_before = common.source_state(S, MF); report['source_before'] = source_before
        for index, root in enumerate(roots):
            guard(); entry = common.catalog(root, OUT / ('host-before-%02d.jsonl' % index), roots)
            product(entry['catalog']['path']); catalogs.append(entry)
        report['input_roots_before'] = catalogs
        runner = common.SerialRunner(OUT, {'rss_bytes': BUDGET['tree_rss_bytes'],
            'command_seconds': BUDGET['command_seconds'], 'log_bytes': 32 * MIB}, guard)
        argv = [python['path'], '-B', str(CODE / NAMES['producer']), '--source', str(S), '--manifest', str(MF),
            '--manifest-sha256', MH, '--spec', control['spec']['path'], '--spec-sha256', control['spec']['sha256'],
            '--deb-cache', control['cache']['path'], '--deb-cache-sha256', control['cache']['sha256'],
            '--resume-control', control['resume_control']['path'], '--resume-control-sha256', control['resume_control']['sha256'],
            '--resume-helper', control['helpers']['resume']['path'], '--resume-helper-sha256', control['helpers']['resume']['sha256'],
            '--restart-helper', control['helpers']['restart']['path'], '--restart-helper-sha256', control['helpers']['restart']['sha256'],
            '--model-helper', control['helpers']['model']['path'], '--model-helper-sha256', control['helpers']['model']['sha256'],
            '--storage-helper', control['helpers']['storage']['path'], '--storage-helper-sha256', control['helpers']['storage']['sha256'],
            '--out', str(TARGET)]
        report['actual_acquisition_argv'] = argv; save()
        environment = {'LANG': 'C', 'LC_ALL': 'C', 'PATH': '/usr/bin', 'PYTHONDONTWRITEBYTECODE': '1',
            'GOMAXPROCS': '1', 'MAKEFLAGS': '-j1', 'DPKG_DEB_THREADS_MAX': '1'}
        runner.run('noetic-r6-authenticated-partial-sdk-continuation', argv, CODE, environment)
        raw, result_pin = read(TARGET / 'report.json', 64 * MIB); result = parse(raw); product(result_pin['path'], result_pin)
        need(result['status'] == 'PREPARED_AUTHENTICATED_EXTRACTED_SYSROOT' and result['source_manifest_sha256'] == MH and
             result['spec'] == control['spec'] and result['resume_control'] == control['resume_control'] and
             len(result['packages']) == 513 and result['acquisition_complete'] is True and result['cleanup']['status'] == 'PASS' and
             not result['cleanup']['remaining'] and result['source_preserved'] is True and result['inputs_preserved'] is True and
             result['products_preserved'] is True and all(row['status'] == 'PASS' and row.get('returncode') == 0 for row in result['commands']),
             'complete actual fresh-authenticated preserved 513-package r6 extraction required')
        continuation = result['continuation']
        need(continuation['actual_old_inner_status'] == 'RUNNING' and continuation['actual_old_outer_status'] == 'FAIL' and
             continuation['old_status_promoted'] is False and continuation['historical_rows_admitted_as_completed'] == 0 and
             continuation['old_cgroup_or_process_continuity'] == 'NOT_CLAIMED' and continuation['old_SDK_renamed_or_modified'] is False and
             continuation['old_SDK_preserved'] is True and continuation['all_packages_freshly_extracted'] is True and
             continuation['fresh_package_count'] == 513 and continuation['fresh_all513_virtual_model'] is True and
             continuation['authenticated_subset_clone_completed'] is True and continuation['ordinary_copy_fallback'] is False and
             continuation['new_SafeTree_initial_state'] == {'entries': 0, 'bytes': 0, 'sequence': 0, 'pending': []} and
             continuation['new_payload_ledger_initial_bytes'] == 0 and continuation['old_ledger_used_as_authority'] is False,
             'honest fresh-all513 extraction/model/ledger/independent-CoW-content contract required')
        need(result['owned_storage']['files_deleted'] is False and result['owned_storage']['root_filesystems_trimmed'] is False and
             len(result['owned_storage']['trim_barriers']) >= 1027, 'owned unused-block barriers required for all model and extraction tar retirements')
        for pin in result['input_pins']: remember(pin)
        for pin in result['product_pins']: product(pin['path'], pin)
        report['actual_sdk'] = result['sdk']; report['acquisition_report'] = result_pin
        report['acquisition_completed'] = True
    except BaseException as error:
        report.update(status='FAIL', error=repr(error), traceback=traceback.format_exc())
    finally:
        finalizing = True; errors = []
        def finish(label, operation):
            try: operation()
            except BaseException as error:
                errors.append({'stage': label, 'error': repr(error)}); report['status'] = 'FAIL'
        def cooperative():
            if not runner: return
            for child in runner.children:
                if child.poll() is not None: continue
                first, now = runner.roots.get(child.pid), common.identity(child.pid)
                need(first is not None and now is not None and common.birth(first) == common.birth(now) and
                     now['parent_pid'] == os.getpid(), 'only same-birth direct producer may receive TERM')
                runner.acquire(child.pid, os.getpid()); key = common.birth(first)
                need(key in runner.owned or child.poll() is not None, 'live direct producer lacks pidfd ownership')
                if key not in runner.owned: continue
                try: signal.pidfd_send_signal(runner.owned[key]['fd'], signal.SIGTERM)
                except ProcessLookupError: pass
                deadline = min(time.monotonic() + BUDGET['cleanup_seconds'], started + BUDGET['whole_seconds'])
                while child.poll() is None and time.monotonic() < deadline:
                    guard(); runner.discover()
                    need(runner.rss() <= BUDGET['tree_rss_bytes'], 'cleanup owned RSS bound exceeded')
                    time.sleep(0.05)
        def cleanup():
            report['cleanup'] = runner.close() if runner else {'status': 'PASS', 'remaining': [], 'scope': 'NO_ACTOR_STARTED'}
            need(report['cleanup']['status'] == 'PASS' and not report['cleanup']['remaining'] and not report['cleanup'].get('errors'),
                 'child-first owned-pidfd cleanup incomplete')
        finish('same-birth-direct-coordinator-600-second-grace', cooperative)
        finish('child-first-owned-pidfd-fallback', cleanup)
        def actual_failure_metadata():
            path = TARGET / 'report.json'
            if path.exists():
                body, pin = read(path, 64 * MIB); actual = parse(body); product(path, pin)
                report['actual_terminal_or_interrupted_inner_report'] = pin
                report['actual_inner_status'] = actual.get('status')
                report['actual_inner_report_promoted'] = False
        finish('actual-inner-failure-metadata', actual_failure_metadata)
        if runner:
            for row in runner.commands:
                path = OUT / (row['label'] + '.log')
                if path.exists(): finish('final-raw-log', lambda path=path: product(path))
            if (OUT / 'commands.json').exists(): finish('final-command-ledger', lambda: product(OUT / 'commands.json'))
        def preserve():
            need(source_before is not None and len(catalogs) == len(roots), 'initial complete input closure was not admitted')
            need(common.source_state(S, MF) == source_before, 'frozen complete Source35 changed')
            after = []
            for index, root in enumerate(roots):
                guard(); entry = producer.read_only_catalog(common, root, roots, guard)
                need({k: v for k, v in entry.items() if k not in {'catalog', 'catalog_disposition'}} ==
                     {k: v for k, v in catalogs[index].items() if k != 'catalog'}, 'complete immutable input root changed')
                after.append(entry)
            for pin in pins.values(): need(common.opened(Path(pin['path']), max(16 * MIB, pin['bytes'])) == pin, 'current selected input changed')
            for pin in products.values(): need(common.opened(Path(pin['path']), max(128 * MIB, pin['bytes'])) == pin, 'retained product changed')
            need(producer.alias_binding('/etc/resolv.conf') == control['resolver_alias'], 'current resolver alias changed at finalization')
            report.update(input_roots_after=after, source_preserved=True, inputs_preserved=True,
                          products_preserved=True, cgroup_after=guard())
        finish('current-source-roots-pins-and-alias-preservation', preserve)
        report['finalization_errors'] = errors
        if report.get('acquisition_completed') and report['status'] == 'RUNNING' and not errors:
            report['status'] = 'PASS_AUTHENTICATED_FRESH_ALL513_INDEPENDENT_COW_CONTENT_SYSROOT_ONLY'
        report['elapsed_seconds'] = time.monotonic() - started; save()
    return 0 if report['status'].startswith('PASS_AUTHENTICATED_') else 1


if __name__ == '__main__': raise SystemExit(main())
