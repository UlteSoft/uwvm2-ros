#!/usr/bin/env python3
"""Exercise actual default-denied bridge and original ROS1 parser boundaries.

Native SDK calls are explicitly replaced with recorded in-process stand-ins;
those authored cases cannot qualify a vendor library, ROS transport or robot.
Separately, the exact installed Noetic genmsg and captured official genaction
parser are the grammar oracles. Neither upstream controller nor installer runs.
"""
import argparse
from contextlib import redirect_stdout
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import runpy
import sys
import threading
from types import SimpleNamespace


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', required=True, type=Path)
    p.add_argument('--out', required=True, type=Path)
    a = p.parse_args()
    inputs = json.loads((a.root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    a.out.mkdir(mode=0o700)
    bridge_module = load('selected_fourier_boundary_subject', source / 'sdk/origin/robots/platforms/fourier/native/bridge.py')
    audit = io.StringIO()
    assert not any(name.startswith(('rospy', 'rclpy', 'fourier_aurora_client', 'uwvm_fourier_bridge')) for name in sys.modules)
    with redirect_stdout(audit):
        assert bridge_module.main([]) == 0
    assert json.loads(audit.getvalue()) == dict(status='DEFAULT_DENIED_NO_NATIVE_OR_ROS_IMPORT', default_authority=[])
    assert not any(name.startswith(('rospy', 'rclpy', 'fourier_aurora_client', 'uwvm_fourier_bridge')) for name in sys.modules)
    calls = []
    class NativeStandIn:
        def get_fsm_state(self):
            calls.append(('get_fsm_state',))
            return 3
        def set_velocity(self, *values):
            calls.append(('set_velocity', values))
        def set_fsm_state(self, value):
            calls.append(('set_fsm_state', value))
            raise RuntimeError('A deliberately unknown native outcome')
        def get_group_state(self, name, key):
            calls.append(('get_group_state', name, key))
            return [0.0, 1.0]
        def set_group_cmd(self, *values):
            calls.append(('set_group_cmd', values))
    configuration = dict(model='gr3', groups={}, limits={
        'velocity_vx': [-.5, .5], 'velocity_vy': [-.5, .5], 'velocity_yaw': [-.2, .2],
        'velocity_duration': [.01, 1], 'fsm_states': [3]}, motion_complete_states=[])
    instance = bridge_module.Bridge(NativeStandIn(), None, None,
        ['get_fsm_state', 'set_velocity', 'set_fsm_state', 'set_group_cmd'], configuration, 'gr3')
    def invoke(operation, sequence, **fields):
        response = SimpleNamespace(status=999, error='unset')
        return instance.invoke(operation, SimpleNamespace(sequence=sequence, **fields), response)
    cases = []
    with redirect_stdout(audit):
        assert invoke('set_upper_fsm_state', 1, state=4).status == 13 and not calls
        cases.append('unselected_method_refused_before_native_effect')
        assert invoke('get_fsm_state', 0).status == 22 and not calls
        value = invoke('get_fsm_state', 1)
        assert value.status == 0 and value.value == 3 and calls == [('get_fsm_state',)]
        assert invoke('get_fsm_state', 1).status == 22 and len(calls) == 1
        cases.append('increasing_sequence_and_exact_selected_read')
        before = len(calls)
        for sequence, value in enumerate((float('nan'), float('inf'), 9), 1):
            assert invoke('set_velocity', sequence, vx=value, vy=0., yaw=0., duration=.1).status == 22
        assert len(calls) == before
        assert invoke('set_velocity', 4, vx=.1, vy=0., yaw=.1, duration=.1).status == 0
        assert calls[-1] == ('set_velocity', (.1, 0., .1, .1))
        assert invoke('set_velocity', 4, vx=.1, vy=0., yaw=.1, duration=.1).status == 22
        cases.append('nonfinite_limits_and_replay_refused_before_native_write')
        response = invoke('set_fsm_state', 1, state=3)
        assert response.status == 5 and response.error == 'NATIVE_RESULT_UNKNOWN_NO_RETRY'
        before = len(calls)
        assert invoke('set_fsm_state', 1, state=3).status == 22 and len(calls) == before
        cases.append('unknown_native_outcome_consumes_sequence_without_retry')
        configuration['groups']['arm'] = dict(position=dict(lower=[-1.], upper=[1.]))
        assert invoke('set_group_cmd', 1, position=[SimpleNamespace(group='arm', values=[.5])], velocity=[], torque=[]).status == 22
        assert not any(v[0] == 'set_group_cmd' for v in calls)
        cases.append('live_model_extent_differs_from_operator_profile_no_write')
        # Same-sequence requests race as rospy callbacks can. Exactly one may
        # reach the native stand-in; the host lock must cover admission/effect.
        barrier, outcomes = threading.Barrier(16), []
        def concurrent():
            barrier.wait(timeout=5)
            outcomes.append(invoke('get_fsm_state', 2).status)
        workers = [threading.Thread(target=concurrent) for _ in range(16)]
        before = sum(v[0] == 'get_fsm_state' for v in calls)
        for worker in workers:
            worker.start()
        for worker in workers:
            worker.join(timeout=5)
            assert not worker.is_alive()
        assert sorted(outcomes) == [0] + [22] * 15
        assert sum(v[0] == 'get_fsm_state' for v in calls) == before + 1
        cases.append('sixteen_concurrent_callbacks_invoke_native_once')
    (a.out / 'bridge-audit.jsonl').write_text(audit.getvalue())
    reference = Path(inputs['noetic_source_root']) / 'opt/ros/noetic/lib/python3/dist-packages'
    manifest_path = Path(inputs['noetic_source_manifest'])
    manifest_bytes = manifest_path.read_bytes()
    restored = {v['path']: v for v in json.loads(manifest_bytes)}
    native_pins = []
    for path in sorted((reference / 'genmsg').glob('*.py')):
        raw = path.read_bytes()
        role = str(path.relative_to(inputs['noetic_source_root']))
        pin = dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())
        assert restored[role]['type'] == 'regular' and all(restored[role][k] == pin[k] for k in ('bytes', 'sha256'))
        native_pins.append(pin)
    sys.path.insert(0, str(reference))
    import genmsg
    import genmsg.msg_loader
    oracle_path = source / 'sdk/origin/robots/platforms/pal/upstream/core-source-references/genaction.py.source'
    oracle_proof = json.loads((oracle_path.parent / 'extras.json').read_bytes())['action_parser']
    assert hashlib.sha256(oracle_path.read_bytes()).hexdigest() == oracle_proof['sha256']
    oracle = runpy.run_path(str(oracle_path))
    production = load('selected_original_section_subject', source / 'sdk/origin/robots/industrial/vendor_profile.py')
    def signature(text, name):
        message = genmsg.msg_loader.load_msg_from_string(genmsg.MsgContext.create_default(), text, name)
        return message.names, message.types, [(v.name, v.type, v.val) for v in message.constants]
    actions = services = components = 0
    for kit in sorted((source / 'sdk/origin/robots/platforms').iterdir()):
        if not (kit / 'profiles.json').exists():
            continue
        for profile in json.loads((kit / 'profiles.json').read_bytes())['profiles']:
            if profile['ros_version'] != 1:
                continue
            catalog_root = kit / 'catalogs' / profile['id']
            for row in json.loads((catalog_root / 'catalog.json').read_bytes())['types']:
                suffix = Path(row['file']).suffix
                if suffix not in ('.srv', '.action'):
                    continue
                body = (catalog_root / row['file']).read_text()
                pieces = production._interface_sections(body, 1, 'service' if suffix == '.srv' else 'action')
                if suffix == '.srv':
                    expected = genmsg.msg_loader.load_srv_from_string(genmsg.MsgContext.create_default(), body, row['type'])
                    original_pieces = [expected.request.text, expected.response.text]
                    roles = ('Request', 'Response')
                    services += 1
                else:
                    original_pieces = oracle['parse_action_spec'](body)
                    roles = ('Goal', 'Result', 'Feedback')
                    actions += 1
                assert len(pieces) == len(original_pieces) == len(roles)
                for first, second, role in zip(pieces, original_pieces, roles):
                    assert signature(first, row['type'] + role) == signature(second, row['type'] + role)
                    components += 1
    assert manifest_path.read_bytes() == manifest_bytes
    assert all(Path(v['path']).stat().st_size == v['bytes'] and hashlib.sha256(Path(v['path']).read_bytes()).hexdigest() == v['sha256'] for v in native_pins)
    report = dict(status='DEFAULT_DENIED_BRIDGE_AND_EXACT_ORIGINAL_ROS1_PROTOCOL_GRAMMAR_BOUNDARIES_PASS',
        authored_bridge_cases=cases, concurrent_callbacks=16, native_sdk_used=False,
        original_ros1_action_instances=actions, original_ros1_service_instances=services,
        complete_native_parser_component_comparisons=components, default_authority=[],
        original_noetic_parser_files=native_pins, original_action_parser=oracle_proof,
        original_official_cases_run=0, native_ros_controller_or_performance_qualified=False)
    (a.out / 'boundaries.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
