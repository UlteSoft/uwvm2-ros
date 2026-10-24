#!/usr/bin/env python3
"""Independently verify the scoped native continuation and preserved old FAILs.

The remote mode reads retained source, compiler prerequisites, binaries, logs
and kernel/cgroup records without mounting, compiling or changing input files.
The --local-root mode checks exported report hashes against the current local
source and protects the prior delivery; it does not claim a local native run.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import sys

SCOPE = 'Native ROS deadline and Image continuation, not a new full A15 acceptance'
WORKER_SHA = '38b83fa9d79f2eeae32b2bb105bb30abc22ba3f8edac3c18333ea28ba6ac58c6'
PROTECTED_LOCAL = {
    'documents/ros/test-report.md': '2f1bb20cb504550c8e84f9b7ba28da7e2f2f5dee04cdc671f6eff6997706afc8',
    'documents/ros/evidence-20261012-v8.tgz': 'fe8b37f3bb67c05cd0eff420f39214f7ffe7a46ae19ca19da39c9a612c88e114',
    'documents/ros/followup-20261013-report.md': '639c06f0b35c120f6dddfe0602f37735d4253dbb75d868185460724962021577',
    'documents/ros/evidence-20261013-followup.tgz': '06a5422f7f7d179867931f77836ffd9e57027ea9a12a23f1c83e8499e32536b1',
    'documents/ros/task-files-followup-20261013.json': '635a7053bfe167738ab967b467a4a1635183cb8bbef7e577b2c5f37f740a8086',
    'test/0073.ros-tests/support/source/ros/run_ros2_followup.py': '060b20b13b918e8fb3372e352e504c37968534ccaf341968d38f70d7e806885a',
    'test/0073.ros-tests/support/source/ros/run_action_deadline_reproduction.py': 'd93fb8b627b202c39ae23f991179c77e459cee54548548277d00fb0af75e1bed',
    'test/0073.ros-tests/support/source/ros/verify_ros2_followup.py': '56253d3fe16b1c0d104071d3fc18afc56ab48bc196b76b758f563290420c2b0c',
}
VARIANTS = [
    ('admission', 'src/ros/gateway/common/server.h', 'test/0073.ros-tests/support/source/ros_ipc/admission_deadline_test.cc', 'admission-deadline', [
        ('expired', ['expired IPC request reached adapter side effect']),
        ('remaining', ['adapter budget reset time spent on admission']),
        ('cap', ['adapter budget reset time spent on admission']),
        ('cap-expired', ['expired capability result failed to return timeout on live IPC session'])]),
    ('query', 'src/ros/gateway/ros2/action.h', 'test/0073.ros-tests/support/source/ros_action/query_deadline_ros2.cc', 'query-deadline', [
        ('feedback', ['expired feedback query delivered queued data']),
        ('result', ['expired result query delivered queued data'])]),
    ('parameter', 'src/ros/gateway/ros2/resources.h', 'test/0073.ros-tests/support/source/ros_resources/parameter_deadline_ros2.cc', 'parameter-deadline', [
        ('set', ['expired local parameter set accepted success']),
        ('declare', ['expired local parameter declare accepted success']),
        ('undeclare', ['expired undeclare preflight removed the actual native parameter']),
        ('get', ['expired local parameter encoding accepted success']),
        ('describe', ['expired local parameter encoding accepted success']),
        ('list', ['expired local parameter encoding accepted success']),
        ('remote', ['expired remote parameter inspector accepted success']),
        ('graph', ['expired actual Graph inspect-node encoding accepted success'])]),
    ('tf', 'src/ros/gateway/ros2/tf.h', 'test/0073.ros-tests/support/source/ros_tf/deadline_native_ros2.cc', 'tf-deadline', [('', [
        'zero-budget lookup must create no native subscription',
        'cached zero-budget lookup must not deliver a successful result'])]),
    ('image', 'src/ros/gateway/ros2/main.cc', 'test/0073.ros-tests/support/source/ros_ipc/image_native_ros2.cc', 'image-native', [('', [
        'malformed native Image endian was normalized or valid barrier changed'])]),
    ('service', 'src/ros/gateway/ros2/main.cc', 'test/0073.ros-tests/support/source/ros_ipc/response_deadline_ros2.cc', 'response-deadline', [
        ('service', ['expired native Service response accepted success'])]),
    ('lifecycle', 'src/ros/gateway/ros2/lifecycle.h', 'test/0073.ros-tests/support/source/ros_ipc/response_deadline_ros2.cc', 'response-deadline', [
        ('get', ['expired native Lifecycle state response accepted success']),
        ('change', ['expired native Lifecycle change response accepted success'])]),
]
NATIVE_PROBES = ['bounded-native', 'timer-publication', 'action-deadline', 'query-deadline',
                 'parameter-deadline', 'tf-deadline', 'image-native', 'response-deadline']


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def read_json(path):
    return json.loads(Path(path).read_text())


def inventory(root):
    root = Path(root)
    result = {}
    for directory, _, files in os.walk(root, followlinks=False):
        for name in files:
            path = Path(directory) / name
            if path.is_file():
                result[str(path.relative_to(root))] = digest(path)
    return result


def make_words(text):
    words, current, at = [], [], 0
    while at < len(text):
        c = text[at]
        if c == '\\' and at + 1 < len(text):
            at += 1
            current.append(text[at])
        elif c == '#':
            break
        elif c.isspace():
            if current:
                words.append(''.join(current)); current = []
        elif c == '$' and at + 1 < len(text) and text[at + 1] == '$':
            current.append('$'); at += 1
        else:
            current.append(c)
        at += 1
    if current:
        words.append(''.join(current))
    return words


def prerequisites(path):
    # Parse only prerequisites, never the absolute .o target. GCC escapes spaces,
    # #, :, $, and line continuations; do not use the runner's /work regex.
    text = Path(path).read_text().replace('\\\r\n', ' ').replace('\\\n', ' ')
    result = []
    for line in text.splitlines():
        escaped = False
        for at, c in enumerate(line):
            if escaped:
                escaped = False
            elif c == '\\':
                escaped = True
            elif c == ':':
                result.extend(make_words(line[at + 1:])); break
    return result


def cmake_dependencies(build):
    root = Path(build) / 'CMakeFiles'
    return sorted(set(root.rglob('*.d')) | set(root.rglob('depend.make')))


def expected_commands(reused=False):
    names = []
    for v, distro in ((1, 'noetic'), (2, 'jazzy')):
        if not reused:
            names.append(f'ros{v}-native-build')
        names += [distro + '-compiler-version', distro + '-packages']
        for probe in ('bounded-text', 'image-encode', 'admission-deadline'):
            names += [distro + '-' + probe + '-build', distro + '-' + probe + '-run']
    for name in NATIVE_PROBES:
        names += [name + '-build', name + '-link', name + '-run']
    for label, _, _, _, cases in VARIANTS:
        names.append('before-' + label + '-build')
        if label != 'admission':
            names.append('before-' + label + '-link')
        names += ['before-' + label + '-' + (mode or 'default') for mode, _ in cases]
    return names + ['native-regression', 'wasm-resources', 'wasm-actions', 'wasm-topics']


class Verification:
    def __init__(self):
        self.count = 0
        self.result = {'status': 'FAIL', 'scope': SCOPE, 'checks': 0,
                       'executed_verifier_sha256': digest(Path(__file__))}

    def check(self, condition, message):
        self.count += 1
        if not condition:
            raise RuntimeError(message)

    def local(self, args, report):
        root, check = args.local_root.resolve(), self.check
        check(report['status'] == 'PASS' and report['scope'] == SCOPE, 'exported aggregate scope/status differs')
        for name, expected in {**report['compiled_project_inputs'], **report['overlay_inputs']}.items():
            check(not Path(name).is_absolute() and '..' not in Path(name).parts, 'unsafe reported project path')
            check(digest(root / name) == expected, 'current local compiled/overlay file differs: ' + name)
        for name, expected in PROTECTED_LOCAL.items():
            check(digest(root / name) == expected, 'previous delivery or driver changed locally: ' + name)
        if args.verification:
            verification = read_json(args.verification)
            check(verification['status'] == 'PASS', 'remote independent validation is not PASS')
            check(verification['source_report_sha256'] == digest(args.report), 'remote validation used a different actual aggregate')
            check(verification['executed_verifier_sha256'] == digest(Path(__file__)), 'local/remote verifier bytes differ')
        self.result.update(mode='current local source and protected previous delivery only',
                           compiled_files=len(report['compiled_project_inputs']),
                           overlay_files=len(report['overlay_inputs']), protected_files=len(PROTECTED_LOCAL))

    def remote(self, args, report):
        check = self.check
        work, base, previous, overlay = (p.resolve() for p in (args.workspace, args.baseline, args.previous, args.overlay))
        source, reports = work / 'source', work / 'reports'
        check(sys.platform == 'linux', 'remote verification requires the supplied Linux environment')
        check(report['status'] == 'PASS' and report['scope'] == SCOPE, 'actual aggregate scope/status differs')
        frozen, compiled, patches = (report[k] for k in ('candidate_source_inputs', 'compiled_project_inputs', 'overlay_inputs'))
        check(inventory(base / 'source') == report['baseline_source_inputs'], 'frozen v8 source changed')
        check(inventory(previous / 'source') == report['previous_followup_source_inputs'], 'completed follow-up source changed')
        check(read_json(previous / 'reports/report.json')['status'] == 'PASS', 'previous completed follow-up no longer PASS')
        for key, root in (('previous_native_binary_inputs', base), ('previous_followup_binaries', previous)):
            entries = report[key]
            check(len(entries) == 18, 'expected all 18 old native binaries: ' + key)
            check(sum('uwvm-ros1-' in Path(name).name for name in entries) == 8 and
                  sum('uwvm-ros2-' in Path(name).name for name in entries) == 10, 'old native distribution differs')
            for name, expected in entries.items():
                check(digest(root / name) == expected, 'old native binary changed: ' + name)
        check(report['baseline_preservation'] == {
            'previous_followup_source_unchanged': True, 'previous_followup_binaries_unchanged': True,
            'source_unchanged': True, 'native_binaries_unchanged': True, 'native_binary_count': 18},
            'finally preservation guards did not pass')
        check(read_json(overlay / 'manifest.json') == patches, 'reviewed overlay manifest differs')
        check(frozen == {**report['baseline_source_inputs'], **patches}, 'candidate differs from baseline plus explicit overlay')
        for name, expected in patches.items():
            check(digest(overlay / name) == expected and frozen[name] == expected, 'reviewed overlay bytes differ: ' + name)
        driver = 'test/0073.ros-tests/support/source/ros/run_ros2_continuation.py'
        check(patches[driver] == report['executed_driver_sha256'] == digest(source / driver), 'executed runner is not reviewed overlay driver')
        check(bool(compiled), 'empty actual compiled input proof')
        for name, expected in compiled.items():
            check(frozen[name] == expected, 'compiled input not frozen: ' + name)
            check(digest(source / name) == expected, 'current remote compiled file changed: ' + name)
            check(digest(reports / 'compiled-inputs' / name) == expected, 'compiled-input immutable copy differs: ' + name)

        before, after = report['cgroup_before'], report['cgroup_after']
        membership = before['membership']
        check(Path('/proc/self/cgroup').read_text().splitlines() == membership, 'verifier not in original cgroup/mount namespace')
        for snapshot in (before, after):
            check(snapshot['status'] == 'PASS' and snapshot['effective_memory_max'] == 68719476736 and
                  snapshot['expected_memory_max'] == 68719476736 and snapshot['membership'] == membership,
                  'actual effective supplied 64 GiB cgroup differs')
        def event_values(snapshot):
            return dict(line.split() for line in snapshot['cgroup_ancestors'][0]['memory.events'].splitlines())
        check(all(event_values(before).get(k) == event_values(after).get(k) for k in ('oom', 'oom_kill', 'oom_group_kill')), 'actual OOM counters increased')
        cgroup_path = next(line.split(':', 2)[2] for line in membership if line.startswith('0::'))
        visible = Path('/sys/fs/cgroup') / cgroup_path.lstrip('/')
        current_limits = []
        while True:
            if (visible / 'memory.max').is_file():
                value = (visible / 'memory.max').read_text().strip()
                if value != 'max':
                    current_limits.append(int(value))
            if visible == Path('/sys/fs/cgroup'):
                break
            visible = visible.parent
        check(bool(current_limits) and min(current_limits) == 68719476736, 'current kernel effective memory limit differs')
        cleanup = report['mount_cleanup']
        check(len(cleanup) == 3 and all(row['returncode'] == 0 for row in cleanup), 'owned roots/alias not cleanly unmounted')
        for row in cleanup:
            attempts = row['attempts']
            check(isinstance(attempts, list) and 1 <= len(attempts) <= 3,
                  'owned cleanup attempt count outside bounded retry policy: ' + row['mount'])
            check(all(set(attempt) == {'returncode', 'stderr'} and type(attempt['returncode']) is int and
                      isinstance(attempt['stderr'], str) for attempt in attempts),
                  'owned cleanup attempt record malformed: ' + row['mount'])
            check(attempts[-1] == {'returncode': row['returncode'], 'stderr': row['stderr']},
                  'owned cleanup final status differs from final actual attempt: ' + row['mount'])
            check(all(attempt['returncode'] != 0 and 'target is busy' in attempt['stderr'] for attempt in attempts[:-1]),
                  'owned cleanup retried a success or a different failure: ' + row['mount'])
        aliases = [row['mount'] for row in cleanup if re.fullmatch('/[a-z][0-9]+', row['mount'])]
        check(len(aliases) == 1, 'expected one short recursive alias')
        alias = aliases[0]
        check({row['mount'] for row in cleanup} == {alias, str(work / 'gateway/noetic-rootfs'), str(work / 'gateway/jazzy-rootfs')},
              'owned cleanup roots differ from current candidate roots/alias')
        mount_points = [line.split(' - ', 1)[0].split()[4] for line in Path('/proc/self/mountinfo').read_text().splitlines()]
        for row in cleanup:
            check(not any(p == row['mount'] or p.startswith(row['mount'] + '/') for p in mount_points), 'owned mount remains: ' + row['mount'])

        def actual(path):
            path = str(path)
            if path == alias or path.startswith(alias + '/'):
                path = str(work) + path[len(alias):]
            if path == '/u8' or path.startswith('/u8/'):
                path = str(base) + path[3:]
            for distro, version in (('noetic', 1), ('jazzy', 2)):
                prefix = str(work / 'gateway' / (distro + '-rootfs') / 'tmp') + '/'
                if path.startswith(prefix):
                    suffix = path[len(prefix):]
                    canonical = f'ros{version}-build/'
                    if suffix.startswith(canonical):
                        suffix = f'ros{version}-continuation-build/' + suffix[len(canonical):]
                    return work / 'native-temp' / distro / suffix
            return Path(path)

        def project_deps(path, prefix):
            result = set()
            for token in prerequisites(path):
                if token.startswith(prefix):
                    normalized = posixpath.normpath(token)
                    check(normalized.startswith(prefix), 'Make prerequisite escaped source root')
                    result.add(normalized[len(prefix):])
            return result
        parsed, dependency_files = set(), []
        for distro in ('noetic', 'jazzy'):
            for path in sorted(set((work / 'native-temp' / distro).glob('ros*-continuation-build/**/*.d')) |
                               set((work / 'native-temp' / distro).glob('ros*-continuation-build/**/depend.make'))):
                dependency_files.append(path); parsed |= project_deps(path, '/work/')
        check(bool(dependency_files) and parsed == set(compiled), 'actual Make prerequisite union differs from compiled input proof')
        required = {item[1] for item in VARIANTS} | {item[2] for item in VARIANTS} | {
            'src/ros/gateway/ros1/main.cc', 'src/uwvm2/imported/ros/ipc/image.h', 'test/0073.ros-tests/support/source/ros_ipc/image_encode_test.cc'}
        check(required.issubset(parsed), 'reviewed production/probe inputs missing from actual compilation')
        image_header = 'src/uwvm2/imported/ros/ipc/image.h'
        old_image, new_image = ((root / image_header).read_text() for root in (base / 'source', source))
        decoder = r'    inline bool decode_image\([^\n]+\)\n    \{.*?\n    \}'
        check(re.search(decoder, old_image, re.S).group(0) == re.search(decoder, new_image, re.S).group(0),
              'owning decoder changed despite encode-only scope')
        encoder = r'    inline std::vector<std::uint8_t> encode_image\(image const& in\)\n    \{.*?\n    \}'
        old_function = re.search(encoder, old_image, re.S).group(0)
        check(old_function == re.search(encoder, (source / 'test/0073.ros-tests/support/source/ros_ipc/image_encode_test.cc').read_text(), re.S).group(0),
              'Image differential baseline is not the exact preserved encoder function')

        commands = report['commands']
        reused = bool(report.get('native_reuse'))
        check([row['name'] for row in commands] == expected_commands(reused), 'actual command sequence incomplete/duplicated/reordered')
        by_name = {row['name']: row for row in commands}
        expected_failures = {'before-' + label + '-' + (mode or 'default') for label, _, _, _, cases in VARIANTS for mode, _ in cases}
        check(len(expected_failures) == 19 and len(commands) == (76 if reused else 78), 'expected exact scoped command/case counts')
        def command_proof(row, directory):
            check(digest(directory / (row['name'] + '.log')) == row['log_sha256'], 'actual log SHA differs: ' + row['name'])
            proof = read_json(directory / (row['name'] + '-exec.json'))
            check(proof == row['pre_exec_live_PID'], 'preexec record differs: ' + row['name'])
            check(proof['pid'] == row['pid'] and proof['membership'] == membership and proof['live_cgroup_procs'] is True and
                  proof['exec_command'] == row['command'], 'actual preexec PID/cgroup/command not bound: ' + row['name'])
        for row in commands:
            old = row['name'] in expected_failures
            check(row['status'] == ('FAIL' if old else 'PASS') and row['returncode'] == (1 if old else 0), 'actual command status/exit differs: ' + row['name'])
            check(bool(row.get('expected_failure', False)) == old, 'old failure designation differs: ' + row['name'])
            command_proof(row, reports)

        owned_executables = []
        def main_proof(row, binary, native_environment=True):
            proof = row['actual_main_live_PID']; stat = binary.stat(); chain = proof['ancestry']
            check(proof['membership'] == membership and proof['live_cgroup_procs'] is True, 'native actual-main cgroup not proven: ' + row['name'])
            check(proof['exe_device'] == stat.st_dev and proof['exe_inode'] == stat.st_ino and proof['exe_sha256'] == digest(binary), 'native actual-main inode/device/SHA differs: ' + row['name'])
            check(len(chain) >= 1 and chain[0] == row['pid'] and chain[-1] == proof['pid'] and len(chain) == len(set(chain)) and
                  all(isinstance(pid, int) and pid > 0 for pid in chain) and
                  (len(chain) == 1 or proof['parent_pid'] == chain[-2]), 'native actual-main descendant chain differs: ' + row['name'])
            check(any(sample['pid'] == proof['pid'] and sample['exe_device'] == stat.st_dev and sample['exe_inode'] == stat.st_ino and
                      sample['ancestry'] == chain and sample['parent_pid'] == proof['parent_pid'] for sample in row['observed_execs']),
                  'matched actual-main sample missing: ' + row['name'])
            if native_environment:
                check(row['pre_exec_live_PID']['environment'] == {'ROS_DOMAIN_ID': '91', 'RMW_IMPLEMENTATION': 'rmw_fastrtps_cpp',
                      'ROS_SECURITY_ENABLE': 'false'}, 'actual native probe environment differs: ' + row['name'])
            owned_executables.append(binary)

        if reused:
            reuse_records = report['native_reuse']
            check(set(reuse_records) == {'noetic', 'jazzy'}, 'partial or unexpected native reuse distribution')
            reuse_roots = {Path(row['source_workspace']).resolve() for row in reuse_records.values()}
            check(len(reuse_roots) == 1, 'native distributions reused different source workspaces')
            reuse_root = reuse_roots.pop()
            check(reuse_root != work and reuse_root not in work.parents and work not in reuse_root.parents,
                  'native reuse source overlaps current workspace')
            reuse_report_path = reuse_root / 'reports/report.json'
            reuse_report_sha = digest(reuse_report_path)
            history = read_json(reuse_report_path)
            check(history['status'] == 'FAIL' and history['scope'] == SCOPE,
                  'native reuse history must preserve its actual aggregate FAIL')
            check(history['executed_driver_sha256'] == history['overlay_inputs'][driver] ==
                  digest(reuse_root / 'source' / driver), 'historical executed driver differs from retained bytes')
            check(inventory(reuse_root / 'source') == history['candidate_source_inputs'], 'historical failed source changed')
            check(history['baseline_source_inputs'] == report['baseline_source_inputs'] and
                  history['previous_followup_source_inputs'] == report['previous_followup_source_inputs'] and
                  history['previous_native_binary_inputs'] == report['previous_native_binary_inputs'] and
                  history['previous_followup_binaries'] == report['previous_followup_binaries'],
                  'historical run used different preserved baseline/follow-up inputs')
            check(history['baseline_preservation'] == report['baseline_preservation'], 'historical baseline preservation differs')
            historical_commands = history['commands']
            check([row['name'] for row in historical_commands] == expected_commands()[:28],
                  'historical failed command prefix differs')
            historical_by_name = {row['name']: row for row in historical_commands}
            for at, row in enumerate(historical_commands):
                last = at == len(historical_commands) - 1
                check(row['status'] == ('FAIL' if last else 'PASS') and row['returncode'] == (1 if last else 0) and
                      not row.get('expected_failure', False), 'historical command status was rewritten: ' + row['name'])
                command_proof(row, reuse_root / 'reports')
            check(history['error'] == 'unexpected exit or missing actual native main proof for query-deadline-build',
                  'historical aggregate failed for a different reason')
            failed_log = (reuse_root / 'reports/query-deadline-build.log').read_text()
            check('error:' in failed_log and 'SharedPtr' in failed_log and 'not a member of' in failed_log and
                  'server_goal' in failed_log, 'historical Jazzy query probe compile failure not retained')
            for snapshot in (history['cgroup_before'], history['cgroup_after']):
                check(snapshot['status'] == 'PASS' and snapshot['membership'] == membership and
                      snapshot['effective_memory_max'] == snapshot['expected_memory_max'] == 68719476736,
                      'historical failed run cgroup evidence differs')
            check(all(event_values(history['cgroup_before']).get(k) == event_values(history['cgroup_after']).get(k)
                      for k in ('oom', 'oom_kill', 'oom_group_kill')), 'historical failed run increased OOM counters')
            historical_cleanup = history['mount_cleanup']
            check(len(historical_cleanup) == 2 and all(row['returncode'] == 0 for row in historical_cleanup),
                  'historical failed run did not clean its two native roots')
            for row in historical_cleanup:
                check(not any(p == row['mount'] or p.startswith(row['mount'] + '/') for p in mount_points),
                      'historical owned native mount remains: ' + row['mount'])
            old_snapshots = inventory(reuse_root / 'native-binaries')
            old_snapshots = {'native-binaries/' + name: value for name, value in old_snapshots.items()}
            check(len(old_snapshots) == 18 and len(history['native_binaries']) == 18,
                  'historical native reuse snapshot/build count differs')
            historical_dependencies = {}
            for distro, version in (('noetic', 1), ('jazzy', 2)):
                record = reuse_records[distro]
                check(record['source_report_sha256'] == reuse_report_sha and record['source_overall_status'] == 'FAIL' and
                      record['build_status'] == 'PASS', 'reuse record rewrote historical aggregate/build status')
                build_row = historical_by_name[f'ros{version}-native-build']
                check(record['source_build_command'] == build_row and build_row['status'] == 'PASS' and build_row['returncode'] == 0,
                      'reuse build proof is not the original actual PASS command')
                check(digest(reports / 'native-reuse' / distro / 'build.log') == build_row['log_sha256'],
                      'copied historical native build log differs')
                check(record['source_binary_snapshots'] == old_snapshots, 'reuse snapshot record differs from historical bytes')
                old_build = reuse_root / 'native-temp' / distro / f'ros{version}-continuation-build'
                new_build = work / 'native-temp' / distro / old_build.name
                old_deps = cmake_dependencies(old_build)
                check(bool(old_deps), 'empty historical native compiler prerequisite proof: ' + distro)
                check({str(path.relative_to(old_build)) for path in old_deps} ==
                      {str(path.relative_to(new_build)) for path in cmake_dependencies(new_build)},
                      'copied native compiler dependency files differ: ' + distro)
                native_inputs = set()
                for dep in old_deps:
                    relative = dep.relative_to(old_build)
                    check(digest(dep) == digest(new_build / relative), 'copied native prerequisite bytes differ: ' + str(relative))
                    native_inputs |= project_deps(dep, '/work/')
                check(f'src/ros/gateway/ros{version}/main.cc' in native_inputs and native_inputs == set(record['compiled_inputs']),
                      'reused native compiled-input proof differs from independent prerequisite parser: ' + distro)
                for name in native_inputs:
                    expected = history['candidate_source_inputs'][name]
                    check(expected == record['compiled_inputs'][name] == frozen[name] == compiled[name] == digest(source / name),
                          'reused native build input differs from current reviewed source: ' + name)
                historical_dependencies[distro] = {'dependency_files': len(old_deps), 'compiled_files': len(native_inputs)}
                for name in (distro + '-compiler-version', distro + '-packages'):
                    check(by_name[name]['log_sha256'] == historical_by_name[name]['log_sha256'] ==
                          digest(reuse_root / 'reports' / (name + '.log')),
                          'new compiler/package environment differs from reused actual build: ' + name)
                metadata = [old_build / 'CMakeCache.txt', *old_build.rglob('flags.make'), *old_build.rglob('link.txt')]
                check(bool(metadata), 'historical native build metadata missing')
                for path in metadata:
                    relative = path.relative_to(old_build)
                    check(digest(path) == digest(new_build / relative) ==
                          digest(reuse_root / 'reports/build-metadata' / f'ros{version}' / relative) ==
                          digest(reports / 'build-metadata' / f'ros{version}' / relative),
                          'historical/copied native build metadata differs: ' + str(relative))
                for key, item in history['native_binaries'].items():
                    if not key.startswith(f'ros{version}:'):
                        continue
                    name = key.split(':', 1)[1]
                    snapshot = reuse_root / 'native-binaries' / f'ros{version}' / name
                    check(Path(item['snapshot']).resolve() == snapshot.resolve(), 'historical native snapshot source differs')
                    old_binary, new_binary = old_build / name, new_build / name
                    with old_binary.open('rb') as stream:
                        check(stream.read(4) == b'\x7fELF', 'historical native artifact is not ELF: ' + key)
                    check(digest(old_binary) == item['sha256'] == digest(snapshot) == digest(new_binary) ==
                          report['native_binaries'][key]['sha256'] and old_binary.stat().st_size == item['bytes'],
                          'historical ELF, immutable snapshot and reused build SHA differ: ' + key)
                main_proof(historical_by_name[distro + '-admission-deadline-run'], old_build / 'admission-deadline', False)
            for name in NATIVE_PROBES[:3]:
                main_proof(historical_by_name[name + '-run'], reuse_root / 'native-temp/jazzy/ros2-continuation-build' / name)
            check(report['native_reuse_preservation'] == {
                'source_unchanged': True, 'binaries_unchanged': True, 'report_unchanged': True, 'report_sha256': reuse_report_sha},
                'finally native reuse preservation guards differ')
            check(digest(reuse_report_path) == reuse_report_sha, 'historical aggregate changed during verification')
            self.result['native_reuse'] = {'source_workspace': str(reuse_root), 'source_overall_status': 'FAIL',
                'source_report_sha256': reuse_report_sha, 'source_command_count': len(historical_commands),
                'retained_failure': {'command': 'query-deadline-build', 'returncode': 1, 'marker': 'SharedPtr'},
                'native_binary_snapshots': len(old_snapshots), 'dependencies': historical_dependencies,
                'new_native_build_commands': 0}
        else:
            check('native_reuse_preservation' not in report, 'reuse preservation present without actual reuse provenance')
        sanitizer_elf_types = {}
        for distro, version in (('noetic', 1), ('jazzy', 2)):
            check(('9.' if version == 1 else '13.') in (reports / (distro + '-compiler-version.log')).read_text(), 'actual GCC version differs')
            check('PASS bounded text 1139 checks:' in (reports / (distro + '-bounded-text-run.log')).read_text(), 'bounded-text sanitizer oracle incomplete')
            image = (reports / (distro + '-image-encode-run.log')).read_text()
            check('IMAGE_ENCODE_PASS checks=675 differential_cases=30' in image, 'Image differential sanitizer oracle incomplete')
            allocation = re.search(r'IMAGE_NATIVE_ALLOCATION before_calls=(\d+) before_bytes=(\d+) after_calls=(\d+) after_bytes=(\d+) wire_bytes=(\d+)', image)
            check(allocation is not None, 'Image allocation oracle missing')
            bc, bb, ac, ab, wb = map(int, allocation.groups())
            check(bc > ac == 1 and bb > ab == wb == 1049123, 'actual Image scoped allocation relation differs')
            admission = (reports / (distro + '-admission-deadline-run.log')).read_text()
            rows = re.findall(r'PASS actual authenticated IPC (expired admission: adapter_calls0|remaining budget: adapter_calls1) timeout_ms=(\d+) barrier_ms=(\d+) actual_budget=(\d+) hits=(\d+)', admission)
            check(len(rows) == 3 and [int(r[1]) for r in rows] == [20, 200, 400], 'admission three independent scenarios incomplete')
            check(all(int(r[4]) == 1 for r in rows) and int(rows[0][3]) == 0 and all(0 < int(r[3]) <= 140 for r in rows[1:]), 'actual caller/cap admission budgets differ')
            check([int(r[2]) for r in rows] == [80, 60, 60], 'admission barrier durations differ')
            capability_rows = re.findall(r'^PASS actual authenticated IPC expired capability: timeout_ms=(\d+) cap_timeout_ms=(\d+) barrier_ms=(\d+) mock_pause_ms=(\d+) first_budget=(\d+) timeout_error110 empty_payload1 same_session_next_success1 next_budget=(\d+) next_mock_pause_ms=0 adapter_calls=(\d+) hits=(\d+)$', admission, re.M)
            check(len(capability_rows) == 1, 'actual live-session capability-timeout response oracle missing/duplicated')
            ipc_ms, cap_ms, barrier_ms, pause_ms, first_budget, next_budget, calls, hits = map(int, capability_rows[0])
            check((ipc_ms, cap_ms, barrier_ms, pause_ms, calls, hits) == (400, 200, 60, 180, 2, 1) and
                  0 < first_budget <= 140 and 200 < next_budget <= 400,
                  'actual capability timeout response/same-session next operation budgets differ')
            pass_lines = re.findall(r'^PASS actual authenticated IPC .*$', admission, re.M)
            check(len(pass_lines) == 4 and pass_lines[-1].startswith('PASS actual authenticated IPC expired capability:'),
                  'admission four independent scenario sequence incomplete')
            for probe in ('bounded-text', 'image-encode', 'admission-deadline'):
                build = by_name[distro + '-' + probe + '-build']['command']
                check('-std=c++17' in build and '-fsanitize=address,undefined' in build, 'actual sanitizer build flags differ')
                check('-fno-pie' in build and '-no-pie' in build, 'actual standalone sanitizer build lacks fixed executable layout')
                binary = work / 'native-temp' / distro / f'ros{version}-continuation-build' / probe
                with binary.open('rb') as stream:
                    header = stream.read(18)
                check(len(header) == 18 and header[:4] == b'\x7fELF' and header[5] == 1 and
                      int.from_bytes(header[16:18], 'little') == 2,
                      'actual standalone sanitizer ELF is not little-endian ET_EXEC: ' + distro + '-' + probe)
                sanitizer_elf_types[distro + '-' + probe] = 2
            main_proof(by_name[distro + '-admission-deadline-run'], work / 'native-temp' / distro / f'ros{version}-continuation-build/admission-deadline', False)
        check(len(sanitizer_elf_types) == 6, 'expected both distributions and all three standalone ET_EXEC sanitizer probes')
        for probe in NATIVE_PROBES:
            main_proof(by_name[probe + '-run'], work / 'native-temp/jazzy/ros2-continuation-build' / probe)
        marker_requirements = {
            'timer-publication': ['PASS'], 'action-deadline': ['respond110', 'ABORTED'],
            'query-deadline': ['PASS actual feedback query after unpumped deadline:110 empty', 'PASS actual result query after unpumped deadline:110 empty',
                               'PASS actual on-time SUCCEEDED result remains readable after stored deadline'],
            'parameter-deadline': ['PASS actual local set callback deadline result110 empty', 'PASS actual local declare callback deadline result110 empty',
                                   'PASS actual local undeclare wire allocation barrier crossed deadline result110 empty native_parameter_retained1',
                                   'PASS actual local get wire allocation barrier crossed deadline result110 empty', 'PASS actual local describe wire allocation barrier crossed deadline result110 empty',
                                   'PASS actual local list wire allocation barrier crossed deadline result110 empty', 'PASS actual generated GetParameters ready before deadline',
                                   'PASS actual Graph inspect-node exact own FQN', 'mode=all'],
            'tf-deadline': ['PASS actual ROS2 TF cold/cached timeout0', 'delayed50ms beyond10ms returns110 empty'],
            'image-native': ['PASS actual ROS2 Image native uint8 endian2/255 rejected', 'native witness4/invalid2'],
            'response-deadline': ['PASS actual ROS2 Trigger/GetState/ChangeState normal exact wire', 'native requests3 each'],
        }
        for name, markers in marker_requirements.items():
            log = (reports / (name + '-run.log')).read_text()
            check(all(marker in log for marker in markers), 'native semantic oracle incomplete: ' + name)
        bounded = re.search(r'^PASS actual ROS2 bounded metadata (\d+) checks:', (reports / 'bounded-native-run.log').read_text(), re.M)
        check(bounded is not None and int(bounded.group(1)) >= 41, 'bounded native oracle lacks fixed assertions')

        variants = report['before_variants']
        check([row['label'] for row in variants] == [item[0] for item in VARIANTS], 'before variant identities differ')
        old_results = []
        for record, (label, changed, probe_path, candidate_probe, cases) in zip(variants, VARIANTS):
            prefix = '/tmp/variants/' + label + '/'
            variant_root = work / 'native-temp/jazzy/variants' / label
            old_hash = digest(previous / 'source' / changed)
            check(record['changed_file'] == changed and record['old_sha256'] == old_hash != frozen[changed] and
                  record['candidate_sha256'] == frozen[changed] and record['probe_sha256'] == frozen[probe_path], 'before variant/probe source binding differs: ' + label)
            check(Path(record['old_source']).resolve() == (previous / 'source' / changed).resolve(), 'before variant not based on preserved previous source')
            check(inventory(variant_root) == {**frozen, changed: old_hash}, 'full old variant differs in more than one production file: ' + label)
            dependency_file = work / 'native-temp/jazzy/ros2-continuation-build' / ('before-' + label + '.d')
            old_deps = project_deps(dependency_file, prefix)
            current_deps = project_deps(work / 'native-temp/jazzy/ros2-continuation-build' / (candidate_probe + '.d'), '/work/')
            check(old_deps == current_deps == set(record['compiled_inputs']) and changed in old_deps and probe_path in old_deps,
                  'old/candidate actual compiler prerequisite sets differ: ' + label)
            differing = []
            for name, expected in record['compiled_inputs'].items():
                check(expected == (old_hash if name == changed else frozen[name]) and digest(variant_root / name) == expected and
                      digest(reports / 'before-inputs' / label / name) == expected, 'old immutable actual compiled bytes differ: ' + label + '/' + name)
                if expected != frozen[name]:
                    differing.append(name)
            check(differing == [changed], 'actual compiler inputs differ in more than the declared production file')
            binary = work / 'native-temp/jazzy/ros2-continuation-build' / ('before-' + label)
            check(digest(binary) == record['binary_sha256'], 'before variant executable differs')
            expected_names = ['before-' + label + '-' + (mode or 'default') for mode, _ in cases]
            check([r['name'] for r in record['runs']] == expected_names, 'missing old semantic failure scenario: ' + label)
            for run, (mode, allowed) in zip(record['runs'], cases):
                row = by_name[run['name']]; marker = run['failure_marker']; log = (reports / (run['name'] + '.log')).read_text()
                check(run['status'] == 'FAIL' and run['returncode'] == 1 and run['expected_failure'] is True and
                      run['log_sha256'] == row['log_sha256'], 'old actual FAIL was rewritten or unbound')
                check(marker in allowed and marker in log and row['actual_failure_marker'] == marker, 'old failure is not the expected semantic oracle: ' + run['name'])
                check(run['allowed_failure_markers'] == (allowed if label == 'tf' else allowed[0]), 'old allowed failure markers broadened')
                check(row['command'][-1] == mode if mode else row['command'][-1].endswith('/before-' + label), 'old scenario command differs')
                main_proof(row, binary);old_results.append({'name': run['name'], 'status': 'FAIL', 'expected_failure': True, 'actual_failure_marker': marker})
        check(len(old_results) == 19, 'expected all 19 retained old semantic FAILs')

        binaries = report['native_binaries']
        check(len(binaries) == 18 and sum(k.startswith('ros1:') for k in binaries) == 8 and sum(k.startswith('ros2:') for k in binaries) == 10, 'new native 8+10 build proof differs')
        for key, item in binaries.items():
            version, name = key.split(':', 1);distro = 'noetic' if version == 'ros1' else 'jazzy'
            binary = work / 'native-temp' / distro / (version + '-continuation-build') / name
            check(digest(binary) == item['sha256'] == digest(actual(item['snapshot'])) and binary.stat().st_size == item['bytes'], 'actual native binary and pre-test snapshot differ: ' + key)
            owned_executables.append(binary)
        check(report['worker_sha256'] == WORKER_SHA == digest(base / 'product-final-v8-io/dual/uwvm2-ros'), 'reused frozen v8 Worker differs')

        nested = [(reports / 'native-regression/report.json', 'native')]
        nested += [(p, 'network') for p in sorted((work / 'network').glob('*/report.json'))]
        nested += [(p, 'rpc') for p in sorted((work / 'rpc').glob('*/report.json'))]
        check(len(nested) == 4, 'expected actual native/resources/actions/topics reports')
        counts, names_by_kind = {}, {}
        for path, kind in nested:
            child = read_json(path);results = child['results']
            check(child['status'] == 'PASS' and bool(results) and all(r['status'] == 'PASS' for r in results), 'nested actual report not PASS: ' + str(path))
            counts[str(path.relative_to(work))] = len(results)
            names = {r['test' if kind == 'native' else 'name'] for r in results};names_by_kind.setdefault(kind, []).append(names)
            for key, artifact in child.get('artifacts', {}).items():
                check(digest(actual(artifact['snapshot'])) == artifact['sha256'], 'nested immutable artifact differs: ' + key)
                if key == 'uwvm':
                    check(artifact['sha256'] == WORKER_SHA, 'nested Worker is not the frozen product')
                elif key.startswith(('ros1/', 'ros2/')):
                    check(artifact['sha256'] == binaries[key.replace('/', ':', 1)]['sha256'], 'nested native program is not the reviewed build')
            for key, artifact in child.get('binary_snapshots', {}).items():
                check(digest(actual(artifact['snapshot'])) == artifact['sha256'], 'native immutable executable snapshot differs')
                native_key = key.split(':', 1)[0] + ':' + Path(artifact['source']).name
                check(artifact['sha256'] == binaries[native_key]['sha256'], 'native regression used a different build')
            for row in results:
                if 'product_sha256' in row:
                    check(row['product_sha256'] == WORKER_SHA and row['wasm_exit'] == 0 and
                          row['guest_sha256'] == child['artifacts'][row['name'] + '/guest']['sha256'], 'actual Guest/product binding differs')
                for version, expected in row.get('gateway_sha256', {}).items():
                    check(expected == binaries['ros' + version + ':uwvm-ros' + version + '-gateway']['sha256'], 'actual result Gateway differs')
                if kind == 'native':
                    check(row['returncode'] == 0 and row['membership'] == membership and bool(row['actual_process_tree']) and
                          all(node['membership'] == membership for node in row['actual_process_tree']), 'native real descendants left supplied cgroup')
            if kind == 'native':
                check(len(results) == 13, 'expected all13 actual native regressions')
                check(names == {'ros1-native', 'ros2-native', 'ros1-resources', 'ros2-resources', 'ros1-tf', 'ros2-tf',
                                'ros2-lifecycle', 'ros1-action', 'ros2-action', 'ros1-ipc-service', 'ros2-ipc-service',
                                'sros2-generate', 'sros2-enforce'}, 'actual native regression case identities differ')
                for snapshot in (child['cgroup_before'], child['cgroup_after']):
                    check(snapshot['status'] == 'PASS' and snapshot['membership'] == membership and snapshot['effective_memory_max'] == 68719476736, 'native regression actual cgroup differs')
            else:
                for snapshot in (child['cgroup_before'], child['cgroup_after']):
                    check(snapshot['path'] == cgroup_path and min(snapshot['limits']) == 68719476736, 'Wasm network actual cgroup differs')
                values = [dict(line.split() for line in child[k]['memory.events'].splitlines()) for k in ('cgroup_before', 'cgroup_after')]
                check(all(values[0].get(k) == values[1].get(k) for k in ('oom', 'oom_kill', 'oom_group_kill')), 'nested actual OOM counters increased')
                if kind == 'rpc':
                    check(len(results) == 78, 'expected actual78 ROS2 Wasm resource cases')
        actions = {'ros2-wasm-action-client', 'ros2-wasm-action-server'}
        topics = {f'ros{v}-wasm-{op}' + suffix for v in (1, 2) for op in ('publish', 'subscribe') for suffix in ('', '-twist', '-image')}
        check(len(names_by_kind['network']) == 2 and actions in names_by_kind['network'] and topics in names_by_kind['network'], 'expected exact2 Action and12 Topic Wasm/native cases')
        owned_ids = {(p.stat().st_dev, p.stat().st_ino) for p in owned_executables}
        live_owned = []
        for entry in Path('/proc').iterdir():
            if not entry.name.isdecimal():
                continue
            try:
                exe = entry / 'exe';stat = exe.stat();path = os.readlink(exe)
                if (stat.st_dev, stat.st_ino) in owned_ids or path.startswith(str(work) + '/') or path.startswith(alias + '/'):
                    live_owned.append({'pid': int(entry.name), 'exe': path})
            except (FileNotFoundError, ProcessLookupError, PermissionError):
                pass
        check(not live_owned, 'owned native process survived final cleanup: ' + str(live_owned))
        self.result.update(mode='remote retained evidence and current kernel cleanup', commands=len(commands),
                           expected_old_failures=old_results, before_variants=len(variants),
                           compiled_files=len(compiled), dependency_files=len(dependency_files),
                           nested_counts=counts, bounded_native_actual_checks=int(bounded.group(1)),
                           standalone_sanitizer_elf_types=sanitizer_elf_types,
                           cleanup_attempt_counts={row['mount']: len(row['attempts']) for row in cleanup},
                           live_owned_processes=live_owned)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path)
    parser.add_argument('--baseline', type=Path)
    parser.add_argument('--previous', type=Path)
    parser.add_argument('--overlay', type=Path)
    parser.add_argument('--report', type=Path)
    parser.add_argument('--local-root', type=Path)
    parser.add_argument('--verification', type=Path)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    if args.local_root:
        if not args.report:
            parser.error('--local-root requires exported --report')
    elif not all((args.workspace, args.baseline, args.previous, args.overlay)):
        parser.error('remote mode requires --workspace --baseline --previous --overlay')
    if not args.report:
        args.report = args.workspace / 'reports/report.json'
    if args.output and args.output.resolve() == args.report.resolve():
        parser.error('verification output must not overwrite the actual report')
    verification = Verification()
    try:
        report = read_json(args.report)
        verification.result['source_report_sha256'] = digest(args.report)
        if args.local_root:
            verification.local(args, report)
        else:
            verification.remote(args, report)
        verification.result['status'] = 'PASS'
    except Exception as error:
        verification.result['error'] = str(error)
    verification.result['checks'] = verification.count
    rendered = json.dumps(verification.result, indent=2) + '\n'
    if args.output:
        args.output.write_text(rendered)
    print(rendered, end='')
    return 0 if verification.result['status'] == 'PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
