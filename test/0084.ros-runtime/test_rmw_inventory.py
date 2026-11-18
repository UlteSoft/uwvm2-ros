#!/usr/bin/env python3
"""Distinguish missing release indexes, historical prereleases and actual native rows."""
import json
from pathlib import Path
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/ros_scene'))
import rmw_inventory as inv


def write(path, obj):
    path.write_text(json.dumps(obj))
    return inv.load(path)[1]


def rejected(fn):
    try:
        fn()
    except ValueError:
        return
    raise AssertionError('invalid inventory input accepted')


def main():
    with tempfile.TemporaryDirectory() as temp:
        base = Path(temp)
        catalog = base / 'catalog.json'
        rows = [{'backend': 1, 'distro': 'noetic'}, {'backend': 2, 'distro': 'jazzy'}]
        write(catalog, {'distributions': rows + [{'backend': 2, 'distro': 'alpha2'}]})
        write(base / 'alpha2-packages.json', [])
        index = [{'distro': 'jazzy', 'package': p, 'version': '1.2.3', 'released': 'released'}
                 for p in ('rmw_opensplice_cpp', 'rmw_example_vendor', 'rmw_implementation')]
        write(base / 'jazzy-packages.json', index)
        digest = 'd' * 64
        sdk = {'format': 'uwvm-actual-native-ros-v1', 'passed': True, 'backend': 'ros2', 'distribution': 'jazzy',
               'executable_hashes': {'runtime': digest}, 'physical_robot': False, 'cases': [{'status': 'PASS', 'grant': True, 'refused': False, 'guest_exit': 0,
                  'native_events': [{'kind': 'trigger'}], 'path': 'guest', 'address_bits': 32, 'artifact_sha256': digest}]}
        legacy = base / 'legacy.json'
        write(legacy, sdk)
        packs = [base / f'p{i}.json' for i in range(2)]
        pack = {'ros_prefix': '/opt/ros/jazzy', 'files': [
            {'destination': '/opt/ros/jazzy/lib/librmw_fastrtps_cpp.so', 'sha256': digest}]}
        sums = [write(p, {**pack, 'index': i}) for i, p in enumerate(packs)]
        run = base / 'run.json'
        case = {'exit': 0, 'guestAssertions': 5, 'engine': 'aot', 'layout': '64-1', 'guest': digest}
        scene = {'format': 'uwvm-scene-dds-roundtrip-v1', 'complete': True, 'vm': digest, 'cases': [case], 'manifests': sums}
        write(run, scene)
        def collect():
            return inv.inventory(catalog, base, [legacy], [[run, *packs]])['targets']
        targets = collect()
        assert len(targets) == 13
        assert [r['release'] for r in targets if r['stage'] == 'prerelease'] == inv.PRERELEASES
        by_name = {r['release']: r for r in targets}
        assert by_name['noetic']['index']['status'] == 'not-applicable'
        assert by_name['alpha1']['index'] == {'status': 'not-supplied'}
        assert by_name['alpha2']['index']['packages'] == [] and by_name['alpha2']['executions'] == []
        assert [r['package'] for r in by_name['jazzy']['index']['packages']] == sorted(r['package'] for r in index)
        assert by_name['jazzy']['index']['packages'][-1]['knownImplementation'] == 'OpenSplice'
        assert sum(r['interface'] == 'sdk-v1' for r in by_name['jazzy']['executions']) == 1
        assert sum(r['interface'] == 'scene-v2' for r in by_name['jazzy']['executions']) == 1
        assert all(r['rmwReportedAtRuntime'] is None for r in by_name['jazzy']['executions'])
        write(run, {**scene, 'manifests': sums[::-1]})
        rejected(collect)
        write(run, {**scene, 'complete': False})
        rejected(collect)
        write(run, scene)
        write(legacy, {**sdk, 'passed': False})
        rejected(collect)
        write(legacy, sdk)
        write(base / 'jazzy-packages.json', [*index, index[0]])
        rejected(collect)
        write(base / 'jazzy-packages.json', [{**index[0], 'distro': 'humble'}])
        rejected(collect)
    print('PASS ROS release / RMW inventory')


if __name__ == '__main__':
    main()
