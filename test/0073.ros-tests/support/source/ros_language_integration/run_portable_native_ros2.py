#!/usr/bin/env python3
"""Real ROS2 publication and endpoint-absence acceptance for five new Wasms.

The supplied inputs-ros2.json explicitly admits current products and the native
SDK. This fixture uses disabled-test security and loopback DDS only. It is not
a production multi-program launcher or an upstream/hardware/performance suite.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import re
import shlex
import stat
import sys
import traceback


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--cohort', choices=('portable', 'java', 'csharp', 'javascript', 'python', 'moonbit'), default='portable')
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs-ros2.json').read_bytes())
    expected = {args.cohort} if args.cohort != 'portable' else {'c', 'cpp', 'rust', 'tinygo', 'assemblyscript'}
    assert len(inputs['wasms']) == len(expected) and {row['language'] for row in inputs['wasms']} == expected
    helper = module('selected_native_owner', Path(__file__).with_name('native_owner.py'))
    policy = module('selected_policy', root / 'source/sdk/origin/tools/example_policy.py')
    report = {'status': 'RUNNING', 'scope': 'Five fresh portable-language ROS2 Humble x86_64 '
              'publications and independent enabled-zero-resource endpoint-absence cases; '
              'explicit disabled-test security, no vendor/QEMU/Enforce/performance credit',
              'inputs': inputs, 'cases': [], 'zero_grant': []}
    owner_type = helper.Owner
    if args.cohort != 'portable':
        budget = module('selected_java_budget', Path(__file__).with_name('cgroup_budget.py'))
        class JavaOwner(helper.Owner):
            def guard(self):
                super().guard()
                current = budget.sample(self.group)
                accounting = self.report.setdefault('memory_accounting', {})
                accounting.setdefault('first', current)
                accounting['last'] = current
                if current['admission_bytes'] > accounting.get('highest_admission', {}).get('admission_bytes', -1):
                    accounting['highest_admission'] = current
                return current['admission_bytes']
        owner_type = JavaOwner
        report['scope'] = 'One fresh ' + args.cohort + ' ROS2 Humble x86_64 publication and independent enabled-zero-resource endpoint-absence case; cache-aware bounded owner, disabled-test security, no vendor/QEMU/Enforce/performance credit'
    report['cohort'] = args.cohort
    owner = owner_type(root, inputs, report, 'native-ros2-results.json')
    sdk, loader, gateway, runtime = map(Path, (inputs['sdk'], inputs['loader'], inputs['gateway'], inputs['runtime']))
    libraries = inputs['library_path']
    peer = root / 'out/native-ros2-peer'
    try:
        for row in inputs['pins']:
            assert Path(row['path']).stat().st_size == row['bytes'] and helper.sha(row['path']) == row['sha256'], row['path']
        argv = ['/usr/bin/clang++-21', '--sysroot=' + str(sdk), '--gcc-toolchain=' + str(sdk / 'usr'),
                '-std=c++20', '-O1', '-g', '-pthread', '-I', root / 'source/third-parties/fast_io/include',
                *inputs['native_includes'], '-MD', '-MF', root / 'out/native-ros2-peer.d', inputs['peer_source'],
                '-o', peer, '-L', sdk / 'opt/ros/humble/lib', '-Wl,--dynamic-linker=' + str(loader),
                '-Wl,--disable-new-dtags', '-Wl,-rpath,' + libraries, '-Wl,-rpath-link,' + libraries,
                *inputs['native_libraries'], '-ldl', '-latomic']
        owner.admit()
        built = owner.launch('ros2-native-peer-build', argv, deadline=180)
        owner.wait(built)
        report['peer'] = {'path': str(peer), 'bytes': peer.stat().st_size, 'sha256': helper.sha(peer)}
        dependencies = shlex.split((root / 'out/native-ros2-peer.d').read_text().replace('\\\n', ' '))[1:]
        report['compile_dependencies'] = {name: {'bytes': Path(name).stat().st_size, 'sha256': helper.sha(name)} for name in dict.fromkeys(dependencies)}
        env = {'AMENT_PREFIX_PATH': str(sdk / 'opt/ros/humble'), 'RMW_IMPLEMENTATION': 'rmw_fastrtps_cpp',
               'ROS_DOMAIN_ID': '209', 'ROS_SECURITY_ENABLE': 'false', 'ROS_LOCALHOST_ONLY': '1',
               'ROS_LOG_DIR': str(root / 'out/ros2-logs')}

        def prepare(lang, namespace, directory, positive):
            directory.mkdir(mode=0o700)
            options = argparse.Namespace(backend=2, gateway_timeout_ms=15000, socket=directory / 'g',
                gateway_uid=os.getuid(), worker_uid=os.getuid(), worker_pid=1, node_name='roswasm_language_gateway',
                namespace=namespace, domain_id=209, disabled_security_tutorial=True,
                master_uri=None, advertise_host=None, operation=['publish'] if positive else [], resource=namespace + '/telemetry')
            assert len(str(options.socket).encode()) <= 107
            plan = policy.policy(options)
            name = lang + '-ros2-' + ('positive' if positive else 'zero')
            wasm = next(row for row in inputs['wasms'] if row['language'] == lang)
            worker = owner.launch(name + '-worker', [runtime, '-Rint', *plan['Worker_arguments'],
                '--wasm-set-main-module-name', 'portable_ros2_' + lang, '--run', wasm['path'],
                *([] if lang in ('javascript', 'moonbit') else ['publish' if positive else 'deny'])], held=True)
            options.worker_pid = worker['process'].pid
            bound = policy.policy(options)
            assert bound['Worker_arguments'] == plan['Worker_arguments']
            config = directory / 'gateway.conf'
            config.write_text(bound['Gateway_config_text'])
            config.chmod(0o600)
            (directory / 'policy.json').write_text(json.dumps(bound, indent=2) + '\n')
            native = owner.launch(name + '-gateway', [loader, '--inhibit-cache', '--library-path', libraries, gateway, config], env)
            while not options.socket.exists():
                owner.observe()
                assert native['process'].poll() is None, owner.text(native, 'stderr')[-2000:]
                __import__('time').sleep(0.02)
            assert stat.S_ISSOCK(options.socket.lstat().st_mode) and stat.S_IMODE(options.socket.lstat().st_mode) == 0o600
            return worker, native, bound

        for index, wasm in enumerate(inputs['wasms']):
            owner.admit()
            lang = wasm['language']
            zero_ns = '/uwvm_portable_r48_zero/c' + str(index)
            zero_peer = owner.launch(lang + '-ros2-zero-peer', [loader, '--inhibit-cache', '--library-path', libraries, peer, zero_ns, 'absent'], env, stdin=True)
            owner.marker(zero_peer, 'READY_NATIVE_TELEMETRY ')
            zero_worker, zero_gateway, zero_policy = prepare(lang, zero_ns, root / ('r2z' + str(index)), False)
            assert zero_policy['grants'] == []
            owner.release(zero_worker)
            owner.wait(zero_worker)
            owner.wait(zero_gateway)
            assert owner.text(zero_worker) == 'PASS_ROS_DEFAULT_DENIED\n'
            zero_audit = [json.loads(line) for line in owner.text(zero_worker, 'stderr').splitlines()]
            zero_denied = [row for row in zero_audit if row.get('event') == 'unresolved-denial']
            assert len(zero_denied) == 2 and all(row['error'] == 13 and row['ros'] == 'ros2' for row in zero_denied)
            owner.credit(zero_peer, b'Q')
            owner.event(lang, 'zero-native-quiet-interval-started')
            owner.wait(zero_peer)
            zero_lines = owner.text(zero_peer).splitlines()
            assert zero_lines == [phase + '_NATIVE_TELEMETRY namespace=' + zero_ns + ' gateway=roswasm_language_gateway gid=' + '0' * 48 + ' warmup=0 delivered=0' for phase in ('READY', 'ABSENT')]
            assert owner.text(zero_peer, 'stderr') == ''
            report['zero_grant'].append({'language': lang, 'wasm': wasm, 'policy': zero_policy,
                'denials': zero_denied, 'native_zero_publishers': True, 'quiet_seconds_after_Q': 2})
            namespace = '/uwvm_portable_r48/c' + str(index)
            observer = owner.launch(lang + '-ros2-positive-peer', [loader, '--inhibit-cache', '--library-path', libraries, peer, namespace], env)
            owner.marker(observer, 'READY_NATIVE_TELEMETRY ')
            owner.event(lang, 'peer-ready')
            worker, native, bound = prepare(lang, namespace, root / ('r2p' + str(index)), True)
            owner.release(worker)
            owner.event(lang, 'worker-exec-released')
            owner.marker(worker, 'READY_ROS_PUBLISH')
            owner.marker(observer, 'MATCHED_NATIVE_TELEMETRY ')
            owner.event(lang, 'native-graph-qos-gid-match-checked')
            mapping = {'gateway': owner.maps(native, [sdk, gateway]), 'peer': owner.maps(observer, [sdk, peer])}
            owner.credit(worker, b'P')
            owner.event(lang, 'P-credit-sent')
            owner.marker(worker, 'PUBLISHED_ROS_VALUE')
            owner.marker(observer, 'DELIVERED_NATIVE_TELEMETRY ')
            owner.event(lang, 'complete-native-value-and-gid-observed')
            owner.credit(worker, b'A')
            owner.event(lang, 'A-credit-sent')
            worker['process'].stdin.close()
            owner.wait(worker)
            owner.wait(native)
            owner.wait(observer)
            owner.event(lang, 'natural-exits-and-quiet-retirement-checked')
            assert owner.text(worker) == 'READY_ROS_PUBLISH\nPUBLISHED_ROS_VALUE\nPASS_ROS_PUBLISH\n'
            audit = [json.loads(line) for line in owner.text(worker, 'stderr').splitlines()]
            denied = [row for row in audit if row.get('event') == 'unresolved-denial']
            assert len(denied) == 1 and denied[0]['error'] == 13 and denied[0]['ros'] == 'ros2' and denied[0]['module'] == 'portable_ros2_' + lang
            assert all(row.get('error') in (0, 13) for row in audit)
            lines = owner.text(observer).splitlines()
            assert len(lines) == 4 and owner.text(observer, 'stderr') == ''
            protocol = []
            for phase, line in zip(('READY', 'MATCHED', 'DELIVERED', 'PASS'), lines):
                match = re.fullmatch(phase + '_NATIVE_TELEMETRY namespace=' + re.escape(namespace) + r' gateway=roswasm_language_gateway gid=([0-9a-f]{48}) warmup=([01]) delivered=([01])', line)
                assert match, line
                protocol.append({'phase': phase, 'gid': match[1], 'warmup': int(match[2]), 'delivered': int(match[3])})
            assert protocol[0]['gid'] == '0' * 48 and protocol[1]['gid'] != '0' * 48 and len({row['gid'] for row in protocol[1:]}) == 1
            assert [row['delivered'] for row in protocol] == [0, 0, 1, 1]
            owner.finish_maps(mapping)
            report['cases'].append({'language': lang, 'wasm': wasm, 'policy': bound, 'protocol': protocol,
                'denial': denied[0], 'actual_executable_maps': mapping, 'complete_value': 'bounded WasmROS telemetry',
                'quiet_seconds_after_zero_publishers': 2, 'publisher_count_after_retirement': 0})
            owner.save()
            print('NATIVE ROS2 PASS', lang, flush=True)
        for row in inputs['pins']:
            assert Path(row['path']).stat().st_size == row['bytes'] and helper.sha(row['path']) == row['sha256']
        for path, row in report['compile_dependencies'].items():
            assert Path(path).stat().st_size == row['bytes'] and helper.sha(path) == row['sha256']
        report['input_and_compile_dependencies_preserved'] = True
        report['status'] = ('ACTUAL_' + args.cohort.upper() + '_NATIVE_ROS2_PASS' if args.cohort != 'portable'
                            else 'ACTUAL_FIVE_PORTABLE_LANGUAGE_NATIVE_ROS2_PASS')
    except BaseException as error:
        report['status'] = 'FAILED_RETAINED'
        report['error'] = repr(error)
        report['traceback'] = traceback.format_exc()
        print('NATIVE ROS2 FAILURE', repr(error), flush=True)
    finally:
        owner.close()
    assert not report['status'].endswith('_PASS') or all(row['exit'] == 0 and row['forced_signal'] is None for row in report['commands'])
    return 0 if report['status'].endswith('_PASS') else 1


if __name__ == '__main__':
    raise SystemExit(main())
