#!/usr/bin/env python3
"""Independently inspect retained ROS1 data without executing any artifact.

Use on the admitted Linux owner after a run or after restoring its owned tree.
This verifier imports no archived controller/SDK, runs no subprocess, opens no
network connection and never loads a Wasm or ELF. External SDK/compiler files
remain separately admitted inputs; their recorded preservation is not a new
execution, upstream suite, security certification or performance result.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def verify(root, cohort='r46'):
    data = json.loads((root / 'out/native-ros1-results.json').read_bytes())
    assert data['status'] == ('ACTUAL_' + cohort.upper() + '_NATIVE_ROS1_DELIVERY_PASS' if cohort in ('java', 'csharp', 'javascript', 'python', 'moonbit')
                              else 'ACTUAL_FIVE_LANGUAGE_NATIVE_ROS1_DELIVERY_PASS')
    old_root = Path(data['inputs']['runtime']).parent.parent

    def owned(path):
        return root / Path(path).relative_to(old_root)

    checked = []
    for row in data['inputs']['pins']:
        path = Path(row['path'])
        if not path.is_relative_to(old_root):
            continue
        current = owned(path)
        assert current.stat().st_size == row['bytes'] and digest(current) == row['sha256'], str(path)
        checked.append(str(path.relative_to(old_root)))
    peer = data['peer']
    assert owned(peer['path']).stat().st_size == peer['bytes'] and digest(owned(peer['path'])) == peer['sha256']
    assert data['input_and_compile_dependencies_preserved'] is True
    assert data['compile_dependencies']
    commands = {row['label']: row for row in data['commands']}
    assert len(commands) == len(data['commands']) == (7 if cohort in ('java', 'csharp', 'javascript', 'python', 'moonbit') else 27)
    for label, row in commands.items():
        assert row['exit'] == 0 and row['forced_signal'] is None, label
        assert 0 < row['max_sampled_owned_rss'] <= 768 << 20
        for stream in ('stdout', 'stderr'):
            assert digest(root / 'out/commands' / label / stream) == row[stream + '_sha256']
    assert commands['native-master']['administrative_stop'] == 'SIGINT after owned cases'
    if cohort in ('java', 'csharp', 'javascript', 'python', 'moonbit'):
        oom = lambda body: {key: value for key, value in map(str.split, body.splitlines()) if key.startswith('oom')}
        assert oom(data['events_before']) == oom(data['events_after'])
        assert data['memory_accounting']['highest_admission']['admission_bytes'] < 63 << 30
    else:
        assert data['events_before'] == data['events_after']
    events = dict(line.split() for line in data['events_after'].splitlines())
    assert all(events[name] == '0' for name in ('oom', 'oom_kill', 'oom_group_kill'))
    assert data['inputs']['cgroup']['identity'] == [29, 19075]
    languages = {'r46': {'go', 'zig', 'fsharp', 'vbnet', 'kotlin'},
                 'portable': {'c', 'cpp', 'rust', 'tinygo', 'assemblyscript'},
                 'java': {'java'}, 'csharp': {'csharp'}, 'javascript': {'javascript'}, 'python': {'python'}, 'moonbit': {'moonbit'}}[cohort]
    assert len(data['cases']) == len(data['zero_grant']) == len(languages)
    assert {row['language'] for row in data['cases']} == languages
    assert {row['language'] for row in data['zero_grant']} == languages
    zero_results = {row['language']: row for row in data['zero_grant']}
    selected = {row['language']: row for row in data['inputs']['wasms']}

    def output(label, stream='stdout'):
        return (root / 'out/commands' / label / stream).read_text()

    def endpoints(graph, category, topic):
        rows = [row for row in graph[category] if row[0] == topic]
        assert len(rows) <= 1
        return sorted(rows[0][1]) if rows else []

    cases = []
    expected_events = ['peer-ready', 'worker-exec-released', 'native-match-and-master-graph-checked',
                       'P-credit-sent', 'complete-native-value-and-header-observed', 'A-credit-sent',
                       'natural-exits-quiet-and-master-retirement-checked']
    for case in data['cases']:
        lang = case['language']
        assert case['wasm'] == selected[lang]
        policy = case['policy']
        assert policy['backend'] == 1 and len(policy['grants']) == 1
        grant = policy['grants'][0]
        assert grant['alias'] == 'pub' and grant['operation'] == 'topic.publish'
        assert grant['type'] == 'std_msgs/String' and grant['identity'] == '992ce8a1687cec8c8bd883ec73ca41d1'
        namespace = policy['connection']['namespace']
        topic = namespace + '/telemetry'
        assert grant['resource'] == topic and grant['limits']['latch'] == 'false'
        assert policy['connection']['master-uri'] == 'http://127.0.0.1:11386'
        assert policy['connection']['advertise-host'] == '127.0.0.1'
        assert policy['connection']['transport'] == 'tcpros'
        assert policy['connection']['connect-timeout-ms'] == '3000'
        assert policy['Gateway_config_text'].splitlines().count('timeout-ms 15000') == 1
        assert endpoints(case['graph_before'], 0, topic) == []
        assert endpoints(case['graph_before'], 1, topic) == [namespace + '/roswasm_language_native_peer']
        assert endpoints(case['graph_matched'], 0, topic) == [namespace + '/roswasm_language_gateway']
        assert endpoints(case['graph_matched'], 1, topic) == [namespace + '/roswasm_language_native_peer']
        assert endpoints(case['graph_after_retirement'], 0, topic) == []
        assert endpoints(case['graph_after_retirement'], 1, topic) == []
        assert output(lang + '-native-worker') == 'READY_ROS_PUBLISH\nPUBLISHED_ROS_VALUE\nPASS_ROS_PUBLISH\n'
        audit = [json.loads(line) for line in output(lang + '-native-worker', 'stderr').splitlines()]
        denied = [row for row in audit if row.get('event') == 'unresolved-denial']
        assert denied == [case['ungranted_alias_denial']]
        assert denied[0]['error'] == 13 and denied[0]['ros'] == 'ros1'
        assert denied[0]['module'] == 'ros1_language_' + lang
        assert all(row.get('error') in (0, 13) for row in audit)
        argv = commands[lang + '-native-worker']['argv']
        assert argv[argv.index('--run') + 1:] == [selected[lang]['path']] + ([] if lang in ('javascript', 'moonbit') else ['publish'])
        assert argv.count('--ros1-global-grant') == 1
        assert argv[argv.index('--ros1-global-grant') + 1:][:5] == [
            'pub', 'topic.publish', topic, 'std_msgs/String', '992ce8a1687cec8c8bd883ec73ca41d1']
        assert '--wasip1-global-grant' not in argv or (
            lang == 'go' and argv.count('--wasip1-global-grant') == 1 and
            argv[argv.index('--wasip1-global-grant') + 1] == 'monotonic-clock')
        assert output(lang + '-enabled-zero-grant') == 'PASS_ROS_DEFAULT_DENIED\n'
        zero_argv = commands[lang + '-enabled-zero-grant']['argv']
        assert '--ros1-global-enable' in zero_argv and '--ros1-global-grant' not in zero_argv
        assert zero_argv[zero_argv.index('--run') + 1:] == [selected[lang]['path']] + ([] if lang in ('javascript', 'moonbit') else ['deny'])
        zero = zero_results[lang]
        assert zero['policy']['backend'] == 1 and zero['policy']['grants'] == []
        assert zero['graph_before_guest'][0] == zero['graph_before_guest'][1] == []
        assert zero['graph_after_retirement'] == [[], [], []]
        assert zero['no_native_topic_endpoints'] is True
        zero_audit = [json.loads(line) for line in output(lang + '-enabled-zero-grant', 'stderr').splitlines()]
        zero_denials = [row for row in zero_audit if row.get('event') == 'unresolved-denial']
        assert zero_denials == zero['exact_denials'] and len(zero_denials) == 2
        assert all(row['error'] == 13 and row['ros'] == 'ros1' for row in zero_denials)
        lines = output(lang + '-native-peer').splitlines()
        assert len(lines) == 4 and output(lang + '-native-peer', 'stderr') == ''
        derived = []
        for phase, line in zip(('READY', 'MATCHED', 'DELIVERED', 'PASS'), lines):
            pattern = (phase + '_NATIVE_ROS1_TELEMETRY namespace=' + re.escape(namespace) +
                ' gateway=' + re.escape(namespace) + r'/roswasm_language_gateway transport=tcpros type=std_msgs/String md5=992ce8a1687cec8c8bd883ec73ca41d1 latch=0 warmup=([01]) delivered=([01]) publishers=([01])')
            match = re.fullmatch(pattern, line)
            assert match, line
            derived.append({'phase': phase, 'warmup': int(match[1]), 'delivered': int(match[2]), 'publishers': int(match[3])})
        assert derived == case['protocol']
        assert [row['delivered'] for row in derived] == [0, 0, 1, 1]
        assert derived[0]['publishers'] == derived[-1]['publishers'] == 0 and derived[1]['publishers'] == 1
        assert case['complete_value'] == 'bounded WasmROS telemetry' and case['quiet_seconds_after_zero_publishers'] == 2
        journal = [row for row in data['protocol_events'] if row['language'] == lang]
        assert [row['phase'] for row in journal] == expected_events
        assert all(a['monotonic_ns'] < b['monotonic_ns'] for a, b in zip(journal, journal[1:]))
        for mapping in case['actual_executable_maps'].values():
            assert mapping['text'] and mapping['files']
            assert all(len(item['sha256']) == 64 and len(item['identity']) == 5 for item in mapping['files'].values())
        cases.append({'language': lang, 'wasm_sha256': selected[lang]['sha256'],
                      'complete_tcp_header_value_graph_and_retirement': True,
                      'enabled_zero_ros_grant_refused': True})
    return {'status': ('INDEPENDENT_' + cohort.upper() + '_ROS1_DATA_VERIFICATION_PASS' if cohort in ('java', 'csharp', 'javascript', 'python', 'moonbit')
                        else 'INDEPENDENT_FIVE_LANGUAGE_ROS1_DATA_VERIFICATION_PASS'),
            'execution_performed': False, 'cases': cases, 'commands': len(commands),
            'owned_pinned_files_checked': len(checked), 'peer_sha256': peer['sha256'],
            'max_sampled_actor_tree_rss': max(row['max_sampled_owned_rss'] for row in commands.values()),
            'events_before': data['events_before'], 'events_after': data['events_after'],
            'scope': 'Inert raw-output/status/product/policy/graph/protocol correspondence; '
                     'no new native, Wasm, compiler, upstream suite, QEMU or performance execution'}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--work-root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--cohort', choices=('r46', 'portable', 'java', 'csharp', 'javascript', 'python', 'moonbit'), default='r46',
                        help='Explicit expected product cohort; never infer coverage from report length')
    args = parser.parse_args()
    result = verify(args.work_root.resolve(strict=True), args.cohort)
    with args.out.open('x') as stream:
        json.dump(result, stream, indent=2)
        stream.write('\n')
    print(result['status'])
