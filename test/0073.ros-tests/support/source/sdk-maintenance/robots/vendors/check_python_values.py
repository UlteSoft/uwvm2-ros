#!/usr/bin/env python3
"""Exercise actual complete Python classes against original-source wire values.

The same checks can execute inside a frozen CPython/Wasm application. Running
this script with native Python qualifies representation only, not a Wasm Guest,
ROS transport, native robot SDK or upstream official test suite.
"""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import sys


def check(module, proof, cases):
    records = {r['type']:r for r in proof['records']}
    assert set(records)=={v['type']for v in cases}
    scalar_instances = 0
    methods = 0
    def value(actual, expected):
        nonlocal scalar_instances
        if 'scalar' in expected:
            scalar_instances += 1
            assert type(actual) is type(expected['value']), (type(actual),expected)
            assert actual==expected['value'],(actual,expected)
        elif 'array' in expected:
            assert type(actual) is list and len(actual)==len(expected['array'])
            for a,b in zip(actual,expected['array']):value(a,b)
        else:
            names = proof['member_map'][expected['record']]
            # The proof JSON sorts object keys; the independent original field
            # tree preserves declaration order. Do not confuse the two orders.
            assert [f.name for f in dataclasses.fields(actual)]==[names[n] for n in expected['members']]
            for native, expected_value in expected['members'].items():
                value(getattr(actual,names[native]),expected_value)
    def refuses(call):
        try:
            call()
        except (ValueError,TypeError,OverflowError):
            return
        raise AssertionError('malformed canonical input was accepted')
    for case in cases:
        row = records[case['type']]
        codec = getattr(module,row['symbol'])
        assert codec.NATIVE_TYPE==row['type'] and codec.CODEC_IDENTITY==row['codec_identity']
        assert codec.NATIVE_FIELD_MAP==proof['member_map'][row['type']]
        wire = bytes.fromhex(case['wire'])
        actual = codec.decode(wire)
        value(actual,case['value'])
        assert actual.encode()==wire
        default = codec()
        assert codec.decode(default.encode()).encode()==default.encode()
        if wire:refuses(lambda:codec.decode(wire[:-1]))
        refuses(lambda:codec.decode(wire+b'\xff'))
        refuses(lambda:codec.decode(bytearray(wire)))
        # Repeated construction must not share any mutable nested/array value.
        other = codec()
        for field in dataclasses.fields(default):
            a,b = getattr(default,field.name),getattr(other,field.name)
            if isinstance(a,list) or dataclasses.is_dataclass(a):assert a is not b
        methods += 6 + bool(wire)
    return dict(records=len(cases),populated_scalar_or_string_instances=scalar_instances,
                completed_codec_methods=methods)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--sdk-python',type=Path,required=True)
    p.add_argument('--generated',type=Path,required=True)
    p.add_argument('--vectors',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a = p.parse_args()
    assert not a.out.exists()
    sys.path.insert(0,str(a.sdk_python.resolve(strict=True)))
    import roswasm.codecs as codecs
    assert Path(codecs.__file__).resolve()==a.sdk_python.resolve()/'roswasm/codecs.py'
    proof = json.loads((a.generated/'generation.python.json').read_bytes())
    path = a.generated/'bounded_records.py'
    pin = proof['outputs']['bounded_records.py']
    assert path.stat().st_size==pin['bytes'] and hashlib.sha256(path.read_bytes()).hexdigest()==pin['sha256']
    spec = importlib.util.spec_from_file_location('_complete_vendor_python_test',path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name]=module
    spec.loader.exec_module(module)
    result = check(module,proof,json.loads(a.vectors.read_bytes())['cases'])
    result.update(status='NATIVE_PYTHON_COMPLETE_ORIGINAL_FIELD_VECTORS_PASS',
                  execution_mode='NATIVE_PYTHON_NOT_A_WASM_OR_NATIVE_ROS_RESULT',default_authority=[],
                  source_sha256=pin['sha256'],original_official_cases_run=0)
    a.out.write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],result['records'])


if __name__=='__main__':
    main()
