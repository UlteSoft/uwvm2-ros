#!/usr/bin/env python3
"""Read-only, independent checks of an actual completed ROS2 follow-up run.

Print JSON to stdout; never compile, start ROS, mount, or modify evidence.
Historical FAIL reports remain FAIL and are checked separately from acceptance.
"""
import argparse
import hashlib
import json
from pathlib import Path
import posixpath
import re
import sys


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def json_file(path):
    return json.loads(Path(path).read_text())


def make_words(text):
    """Decode Linux Make prerequisite escapes independently of runner regex."""
    words, word = [], []
    i = 0
    while i < len(text):
        c = text[i]
        if c == '\\' and i + 1 < len(text):
            i += 1
            word.append(text[i])
        elif c == '#' :
            break
        elif c.isspace():
            if word:
                words.append(''.join(word)); word = []
        elif c == '$' and i + 1 < len(text) and text[i + 1] == '$':
            word.append('$'); i += 1
        else:
            word.append(c)
        i += 1
    if word:
        words.append(''.join(word))
    return words


def prerequisites(path):
    text = Path(path).read_text().replace('\\\r\n', ' ').replace('\\\n', ' ')
    result = []
    for line in text.splitlines():
        escaped = False
        for i, c in enumerate(line):
            if escaped:
                escaped = False
            elif c == '\\':
                escaped = True
            elif c == ':':
                result.extend(make_words(line[i + 1:])); break
    return result


def inventory(root):
    return {str(p.relative_to(root)): digest(p) for p in root.rglob('*') if p.is_file()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path, required=True)
    parser.add_argument('--baseline', type=Path, required=True)
    parser.add_argument('--overlay', type=Path, required=True)
    parser.add_argument('--history-report', type=Path, action='append', default=[])
    args = parser.parse_args()
    work, base, overlay = args.workspace.resolve(), args.baseline.resolve(), args.overlay.resolve()
    reports, source = work / 'reports', work / 'source'
    count = 0
    result = {'status': 'FAIL', 'scope': 'ROS2 follow-up only; frozen v8 A15 is separate',
              'workspace': str(work), 'checks': 0, 'historical_failures': []}

    def check(condition, message):
        nonlocal count
        count += 1
        if not condition:
            raise RuntimeError(message)

    try:
        report_path = reports / 'report.json'
        report = json_file(report_path)
        check(report['status'] == 'PASS', 'actual final aggregate is not PASS')
        check(report['scope'] == 'ROS2 follow-up, not a new full A15 acceptance', 'aggregate scope changed')
        result['source_report_sha256'] = digest(report_path)
        baseline = report['baseline_source_inputs']
        previous = report['previous_native_binary_inputs']
        patches = report['overlay_inputs']
        frozen = report['candidate_source_inputs']
        compiled = report['compiled_project_inputs']
        check(len(previous) == 18, 'expected all 18 old native binaries')
        old_by_version = {1: [], 2: []}
        for relative in previous:
            for version in (1, 2):
                if Path(relative).name.startswith('uwvm-ros' + str(version) + '-'):
                    old_by_version[version].append(relative)
        check([len(old_by_version[1]), len(old_by_version[2])] == [8, 10], 'expected actual ROS1 8 + ROS2 10 distribution')
        check(inventory(base / 'source') == baseline, 'original source changed versus recorded pre-run inventory')
        for relative, expected in previous.items():
            check(digest(base / relative) == expected, 'old native binary changed: ' + relative)
        check(report['baseline_preservation'] == {'source_unchanged': True, 'native_binaries_unchanged': True, 'native_binary_count': 18},
              'runner did not prove baseline preservation in finally')
        check(json_file(overlay / 'manifest.json') == patches, 'overlay manifest differs from actual report')
        for relative, expected in patches.items():
            check(digest(overlay / relative) == expected, 'overlay file changed: ' + relative)
            check(frozen.get(relative) == expected, 'candidate does not contain reviewed overlay: ' + relative)
        expected_candidate = dict(baseline); expected_candidate.update(patches)
        check(frozen == expected_candidate, 'pre-build candidate differs from baseline plus explicit overlay')
        driver = 'test/0073.ros-tests/support/source/ros/run_ros2_followup.py'
        check(report['executed_driver_sha256'] == patches[driver] == digest(source / driver), 'executed runner SHA not exact reviewed driver')
        check(bool(compiled), 'empty compiled input proof')
        for relative, expected in compiled.items():
            check(frozen.get(relative) == expected, 'compiled input absent from frozen candidate: ' + relative)
            check(digest(source / relative) == expected, 'compiled source changed after tests: ' + relative)
            check(digest(reports / 'compiled-inputs' / relative) == expected, 'frozen compiled-input copy differs: ' + relative)

        dependency_files, parsed = [], set()
        for distro in ('noetic', 'jazzy'):
            for path in (work / 'native-temp' / distro).glob('ros*-followup-build/**/*.d'):
                dependency_files.append(path)
                for token in prerequisites(path):
                    if not token.startswith('/work/'):
                        continue
                    normalized = posixpath.normpath(token)
                    check(normalized.startswith('/work/'), 'Make dependency escaped source root')
                    relative = normalized[len('/work/'):]
                    check(relative in frozen, 'actual Make prerequisite not frozen: ' + relative)
                    parsed.add(relative)
        check(bool(dependency_files), 'no actual dependency files retained')
        check(parsed == set(compiled), 'independent actual prerequisite set differs from compiled_project_inputs')
        check({driver}.issubset(patches), 'driver not packaged in overlay')
        check({'src/ros/gateway/common/bounded_text.h', 'src/ros/gateway/ros2/resources.h', 'src/ros/gateway/ros2/action.h',
               'src/ros/gateway/ros2/main.cc', 'src/ros/gateway/ros2/lifecycle.h', 'test/0073.ros-tests/support/source/ros_resources/bounded_text_test.cc',
               'test/0073.ros-tests/support/source/ros_resources/bounded_native_ros2.cc', 'test/0073.ros-tests/support/source/ros_action/timer_publication_ros2.cc',
               'test/0073.ros-tests/support/source/ros_action/deadline_native_ros2.cc'}.issubset(parsed), 'missing reviewed active compiled inputs')

        before, after = report['cgroup_before'], report['cgroup_after']
        for snapshot in (before, after):
            check(snapshot['status'] == 'PASS' and snapshot['effective_memory_max'] == 68719476736,
                  'recorded effective cgroup differs from supplied 64 GiB')
            check(snapshot['expected_memory_max'] == 68719476736 and bool(snapshot['cgroup_ancestors']), 'missing cgroup proof')
        check(before['membership'] == after['membership'], 'driver cgroup membership changed')
        membership = before['membership']
        check(Path('/proc/self/cgroup').read_text().splitlines() == membership,
              'run verifier in the same supplied cgroup and mount namespace as the follow-up')
        cgroup_path = next(line.split(':', 2)[2] for line in membership if line.startswith('0::'))
        def events(snapshot):
            return dict(line.split() for line in snapshot['cgroup_ancestors'][0]['memory.events'].splitlines())
        check(all(events(before).get(k) == events(after).get(k) for k in ('oom', 'oom_kill', 'oom_group_kill')), 'actual OOM counters increased')
        check(len(report['mount_cleanup']) == 3 and all(row['returncode'] == 0 for row in report['mount_cleanup']),
              'three private mount roots were not cleanly unmounted')
        aliases = {row['mount'] for row in report['mount_cleanup'] if re.fullmatch('/[a-z][0-9]+', row['mount'])}
        check(len(aliases) == 1, 'missing unique recursive short alias')
        mounts = [line.split(' - ', 1)[0].split()[4] for line in Path('/proc/self/mountinfo').read_text().splitlines()]
        for row in report['mount_cleanup']:
            check(not any(p == row['mount'] or p.startswith(row['mount'].rstrip('/') + '/') for p in mounts),
                  'owned mount survived final cleanup: ' + row['mount'])

        def actual(path):
            path = str(path)
            for alias in aliases:
                if path == alias or path.startswith(alias + '/'):
                    path = str(work) + path[len(alias):]
            if path.startswith('/u8/'):
                path = str(base) + path[3:]
            for distro, version in (('noetic', 1), ('jazzy', 2)):
                prefix = str(work / 'gateway' / (distro + '-rootfs') / 'tmp') + '/'
                if path.startswith(prefix):
                    relative = path[len(prefix):]
                    old = 'ros' + str(version) + '-build/'
                    if relative.startswith(old):
                        relative = 'ros' + str(version) + '-followup-build/' + relative[len(old):]
                    return work / 'native-temp' / distro / relative
            return Path(path)

        commands = report['commands']
        expected_names = []
        for version, distro in ((1, 'noetic'), (2, 'jazzy')):
            expected_names.extend(['ros' + str(version) + '-native-build', distro + '-compiler-version', distro + '-packages',
                                   distro + '-bounded-text-build', distro + '-bounded-text-run'])
        for name in ('bounded-native', 'timer-publication', 'action-deadline'):
            expected_names.extend([name + '-build', name + '-link', name + '-run'])
        expected_names.extend(['native-regression', 'wasm-resources', 'wasm-actions'])
        check([row['name'] for row in commands] == expected_names, 'missing, duplicated or reordered actual commands')
        for row in commands:
            check(row['status'] == 'PASS' and row['returncode'] == 0, 'actual command failed: ' + row['name'])
            check(digest(reports / (row['name'] + '.log')) == row['log_sha256'], 'actual log differs: ' + row['name'])
            proof = json_file(reports / (row['name'] + '-exec.json'))
            check(proof == row['pre_exec_live_PID'], 'standalone PID proof differs: ' + row['name'])
            check(proof['pid'] == row['pid'] and proof['membership'] == membership and proof['live_cgroup_procs'] is True,
                  'pre-exec actual cgroup PID not proven: ' + row['name'])
            check(proof['exec_command'] == row['command'], 'command differs from pre-exec proof: ' + row['name'])
            if row['name'] in ('bounded-native-run', 'timer-publication-run', 'action-deadline-run'):
                check(proof['environment'] == {'ROS_DOMAIN_ID': '89', 'RMW_IMPLEMENTATION': 'rmw_fastrtps_cpp',
                                               'ROS_SECURITY_ENABLE': 'false'},
                      'actual native probe pre-exec environment differs: ' + row['name'])
        for distro, version_text in (('noetic', '9.'), ('jazzy', '13.')):
            check(version_text in (reports / (distro + '-compiler-version.log')).read_text(), 'unexpected GCC version: ' + distro)
            check('PASS bounded text 1139 checks:' in (reports / (distro + '-bounded-text-run.log')).read_text(), 'bounded text test did not complete')
        bounded_marker = re.search(r'^PASS actual ROS2 bounded metadata (\d+) checks:',
                                   (reports / 'bounded-native-run.log').read_text(), re.MULTILINE)
        # Each DDS discovery retry performs one extra transient-state check.
        # The fixed assertions are 41; successful first discovery needs no retry.
        check(bounded_marker is not None and int(bounded_marker.group(1)) >= 41,
              'bounded native oracle did not complete its fixed assertions')
        result['bounded_native_actual_checks'] = int(bounded_marker.group(1))
        for name in ('timer-publication-run', 'action-deadline-run'):
            check('PASS' in (reports / (name + '.log')).read_text(), 'missing actual test completion: ' + name)
        deadline_log = (reports / 'action-deadline-run.log').read_text()
        check('respond110' in deadline_log and 'ABORTED' in deadline_log,
              'actual expired response/native terminal witness not confirmed')

        binaries = report['native_binaries']
        expected_binary_names = {'ros' + str(v) + ':' + Path(p).name for v in (1, 2) for p in old_by_version[v]}
        check(set(binaries) == expected_binary_names, 'new native binary identities differ from actual 18 baseline targets')
        for key, value in binaries.items():
            snapshot = actual(value['snapshot'])
            check(snapshot.stat().st_size == value['bytes'] and digest(snapshot) == value['sha256'], 'new binary snapshot differs: ' + key)
            version, name = key.split(':', 1)
            distro = 'noetic' if version == 'ros1' else 'jazzy'
            built = work / 'native-temp' / distro / (version + '-followup-build') / name
            check(digest(built) == value['sha256'], 'built binary differs from pre-test snapshot: ' + key)
        worker = base / 'product-final-v8-io/dual/uwvm2-ros'
        check(digest(worker) == report['worker_sha256'], 'frozen product worker changed')

        nested = [(reports / 'native-regression/report.json', 'native-regression')]
        nested += [(path, 'network') for path in sorted((work / 'network').glob('*/report.json'))]
        nested += [(path, 'rpc') for path in sorted((work / 'rpc').glob('*/report.json'))]
        check(len(nested) == 3, 'expected actual native, RPC-resources and Action reports')
        nested_counts = {}
        for path, kind in nested:
            child = json_file(path)
            check(child['status'] == 'PASS' and bool(child['results']), 'nested actual report not PASS: ' + str(path))
            check(all(row['status'] == 'PASS' for row in child['results']), 'nested failed result retained in final accepted run')
            nested_counts[str(path.relative_to(work))] = len(child['results'])
            for artifact in child.get('artifacts', {}).values():
                check(digest(actual(artifact['snapshot'])) == artifact['sha256'], 'nested immutable executable snapshot differs')
            for key, artifact in child.get('binary_snapshots', {}).items():
                check(digest(actual(artifact['snapshot'])) == artifact['sha256'], 'native immutable executable snapshot differs')
                native_key = key.split(':', 1)[0] + ':' + Path(artifact['source']).name
                check(artifact['sha256'] == binaries[native_key]['sha256'], 'native regression used different built executable: ' + key)
            for key, value in child.get('artifacts', {}).items():
                if key == 'uwvm':
                    check(value['sha256'] == report['worker_sha256'], 'nested test used different product worker')
                elif key.startswith(('ros1/', 'ros2/')):
                    check(value['sha256'] == binaries[key.replace('/', ':', 1)]['sha256'], 'nested test used different native executable')
            for row in child['results']:
                if 'product_sha256' in row:
                    check(row['product_sha256'] == report['worker_sha256'], 'result product binding differs')
                    check(row['wasm_exit'] == 0 and row['guest_sha256'] == child['artifacts'][row['name'] + '/guest']['sha256'],
                          'result executed Guest binding/exit differs')
                for version, value in row.get('gateway_sha256', {}).items():
                    key = 'ros' + version + ':uwvm-ros' + version + '-gateway'
                    check(value == binaries[key]['sha256'], 'result Gateway binding differs')
            if kind in ('network', 'rpc'):
                for snapshot in (child['cgroup_before'], child['cgroup_after']):
                    check(snapshot['path'] == cgroup_path and min(snapshot['limits']) == 68719476736, 'nested actual cgroup differs')
                nested_events = [dict(line.split() for line in snapshot['memory.events'].splitlines())
                                 for snapshot in (child['cgroup_before'], child['cgroup_after'])]
                check(all(nested_events[0].get(k) == nested_events[1].get(k) for k in ('oom', 'oom_kill', 'oom_group_kill')),
                      'nested actual OOM counters increased')
            else:
                for snapshot in (child['cgroup_before'], child['cgroup_after']):
                    check(snapshot['status'] == 'PASS' and snapshot['membership'] == membership and
                          snapshot['effective_memory_max'] == 68719476736, 'native regression actual cgroup differs')
                for row in child['results']:
                    check(row['returncode'] == 0 and row['membership'] == membership, 'native actual process membership/exit differs')
                    check(all(node['membership'] == membership for node in row['actual_process_tree']), 'native descendant left supplied cgroup')
        action_reports = [json_file(path) for path, kind in nested if kind == 'network'
                          and any(row.get('name', '').startswith('ros2-wasm-action-') for row in json_file(path)['results'])]
        check(len(action_reports) == 1 and {row['name'] for row in action_reports[0]['results']} == {'ros2-wasm-action-client', 'ros2-wasm-action-server'},
              'expected two explicit Wasm Action cases')

        for history_path in args.history_report:
            history = json_file(history_path)
            check(history['status'] == 'FAIL', 'historical failure was rewritten as pass')
            names = [row['name'] for row in history['commands']]
            error = history.get('error', '')
            if names[-1] == 'action-deadline-build' and history['commands'][-1]['status'] == 'FAIL':
                scope = 'r1 native builds/standalone and prior probes completed; deadline probe failed compilation; no completed regression/RPC/Action acceptance'
                check(history['commands'][-1]['returncode'] != 0 and 'action-deadline-build' in error, 'r1 compile failure scope changed')
            elif error.startswith('unfrozen compiler input ') and error.endswith('.o'):
                scope = 'r2 all19 build/probe commands passed; aggregate FAIL parsing Make output target as input; no regression/RPC/Action acceptance'
                check(len(names) == 19 and all(row['status'] == 'PASS' and row['returncode'] == 0 for row in history['commands']), 'r2 successful probe scope changed')
            elif names[-1] == 'action-deadline-run' and history['commands'][-1]['status'] == 'FAIL':
                scope = 'r3 deadline cutoff/110/goal-timer-ID retirement assertions passed before terminal-witness wait failed; native terminal result not confirmed; no regression/RPC/Action acceptance'
                failed_log = (history_path.parent / 'action-deadline-run.log').read_text()
                check(history['commands'][-1]['returncode'] != 0 and
                      'native terminal result did not arrive after executor restart' in failed_log,
                      'r3 actual terminal-witness failure scope changed')
                check(history.get('baseline_preservation') == {'source_unchanged': True, 'native_binaries_unchanged': True, 'native_binary_count': 18},
                      'r3 failed attempt did not verify baseline preservation')
            else:
                raise RuntimeError('unexpected historical failure scope: ' + error)
            for row in history['commands']:
                check(digest(history_path.parent / (row['name'] + '.log')) == row['log_sha256'], 'historical actual log changed')
            historical_source = history_path.parent.parent / 'source'
            old_fixture = 'test/0073.ros-tests/support/source/ros_action/deadline_native_ros2.cc'
            old_fixture_sha = history['candidate_source_inputs'][old_fixture]
            old_driver_sha = history['candidate_source_inputs'][driver]
            check(digest(historical_source / old_fixture) == old_fixture_sha, 'historical deadline fixture changed')
            check(digest(historical_source / driver) == old_driver_sha, 'historical executed driver source changed')
            if 'executed_driver_sha256' in history:
                check(history['executed_driver_sha256'] == old_driver_sha, 'historical driver prehash differs')
            result['historical_failures'].append({'report': str(history_path), 'sha256': digest(history_path),
                                                  'status': 'FAIL', 'actual_error': error, 'scope': scope,
                                                  'frozen_fixture_sha256': old_fixture_sha, 'frozen_driver_source_sha256': old_driver_sha,
                                                  'driver_prehash_recorded': 'executed_driver_sha256' in history})
        result.update(status='PASS', dependency_files=len(dependency_files), compiled_project_inputs=len(compiled),
                      native_binaries_by_version={'ros1': 8, 'ros2': 10}, actual_commands=len(commands), nested_result_counts=nested_counts,
                      baseline_preservation=True, source_inventory_scope='Pre-build baseline+overlay; only actual compiled inputs required unchanged after RPC fixture generation')
    except (OSError, ValueError, KeyError, RuntimeError, IndexError, StopIteration) as error:
        result['error'] = str(error)
    result['checks'] = count
    print(json.dumps(result, indent=2) + '\n', end='')
    return 0 if result['status'] == 'PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
