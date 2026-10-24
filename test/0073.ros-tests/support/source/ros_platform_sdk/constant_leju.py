#!/usr/bin/env python3
"""Compare Leju's original lowercase constant with the actual Noetic parser.

This authored source-grammar regression is not an original upstream test or a
robot/native-delivery result. It imports only the explicit installed genmsg
reference and source-paired project generator; no vendor application executes.
The ROS1 constant is preserved and consumes no serialized request field.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import sys


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    original = Path(inputs['captured_sources']) / 'upstream/main/files/src/kuavo_msgs/srv/changeArmCtrlMode.srv'
    body = original.read_text()
    sdk = Path(inputs['native_dependency_root'])
    reference = sdk / 'opt/ros/noetic/lib/python3/dist-packages'
    sys.path.insert(0, str(reference))
    import genmsg
    import genmsg.msg_loader
    service = genmsg.msg_loader.load_srv_from_string(genmsg.MsgContext.create_default(), body, 'kuavo_msgs/changeArmCtrlMode')
    assert [(c.name, c.val) for c in service.request.constants] == [('ik_ultra_fast_mode', 3)]
    assert service.request.names == ['control_mode'] and service.request.types == ['int32']
    project = load('leju_constant_source_pair', source / 'sdk/origin/robots/industrial/vendor_profile.py')
    parent_path = source / 'sdk/origin/tools/custom_types.py'
    bounded_path = source / 'sdk/origin/custom-types/bounded_profile.py'
    parent, bounded, adapter = parent_path.read_bytes(), bounded_path.read_bytes(), Path(project.__file__).read_bytes()
    templates = {n: (bounded_path.parent / n).read_bytes() for n in
                 ('interface_templates/bounded_interfaces.' + e + '.in' for e in ('rs', 'mjs', 'go', 'ts'))}
    api = project.API(parent, bounded, parent_path=parent_path, bounded_path=bounded_path,
                      parent_sha256=hashlib.sha256(parent).hexdigest(), bounded_sha256=hashlib.sha256(bounded).hexdigest(),
                      templates=templates, adapter_raw=adapter, adapter_sha256=hashlib.sha256(adapter).hexdigest())
    section = body.split('---', 1)[0]
    constants = []
    parsed = api.env['parse_fields'](api.parent, section, 1, 'kuavo_msgs', {}, set(),
                                     native_type='kuavo_msgs/changeArmCtrlModeRequest', constants=constants)
    assert [f.name for f in parsed] == service.request.names and [f.type for f in parsed] == service.request.types
    assert [(c['name'], int(c['value'])) for c in constants] == [('ik_ultra_fast_mode', 3)]
    assert sum(api.parent.SCALARS[f.type][1] for f in parsed) == 4
    # The independent native-definition comparison must recognize the same
    # original constant; accepting it only in source emission is insufficient.
    signature = api._signature(section, 'kuavo_msgs')
    assert [r for r in signature if r[0] == 'field'] == [('field', 'int32', 'control_mode', '', 0)]
    compared = [r for r in signature if r[0] == 'constant']
    assert len(compared) == 1 and compared[0][:3] == ('constant', 'int32', 'ik_ultra_fast_mode')
    assert int(compared[0][3]) == 3
    try:
        api.env['parse_fields'](api.parent, section, 2, 'kuavo_msgs', {}, set(), constants=[])
    except ValueError:
        pass
    else:
        raise AssertionError('ROS2 lowercase constant was unexpectedly admitted')
    pins = []
    for path in sorted((reference / 'genmsg').glob('*.py')):
        raw = path.read_bytes()
        pins.append(dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest()))
    result = dict(status='ORIGINAL_LEJU_LOWERCASE_CONSTANT_MATCHES_ACTUAL_NOETIC_GENMSG',
                  original_service_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),
                  preserved_constant='ik_ultra_fast_mode', constant_value=3, request_fields=service.request.names,
                  request_canonical_bytes=4, ros2_wrong_case_refused=True, reference_sources=pins,
                  native_definition_comparison_preserves_lowercase_constant=True,
                  original_official_cases_run=0, native_or_guest_runs=0, default_authority=[])
    (out / 'constant.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], flush=True)


if __name__ == '__main__':
    main()
