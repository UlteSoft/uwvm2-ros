#!/usr/bin/env python3
"""Exercise the closed JSON encoder and retain canonical files for C++ decoding.

This script runs only under the existing memory gate. The companion C++ runner
must decode both generated files; Python encoding alone is not qualification.
"""
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import struct
import encode_parameter_definition as encoder

GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def fixture(algebra):
    def key(name, kind, data):
        return dict(descriptor=dict(name=name, type=kind, description='trusted metadata', additional_constraints='',
                    read_only=False, dynamic_type=False, range=None), types=[kind], integer_range=None, floating_range=None,
                    override=dict(type=kind, value=data))
    values = [('boolean', True), ('integer', 17), ('floating', 0.5), ('string', 'robot ☃'), ('bytes', 'AAECAw==')]
    if algebra == 'ros2':
        values += [('not_set', None), ('booleans', [True, False]), ('integers', [-3, 0, 1 << 60]),
                   ('floats', [-1.5, 0.25]), ('strings', ['a', 'b\u0000c'])]
    else:
        values += [('xmlrpc_array', [dict(type='integer', value=7), dict(type='string', value='heterogeneous')]),
                   ('xmlrpc_struct', [dict(name='first', value=dict(type='integer', value=3)),
                                      dict(name='second', value=dict(type='xmlrpc_array', value=[]))]),
                   ('xmlrpc_datetime', '20240229T23:59:59')]
    keys = [key('key.' + str(index).zfill(2), kind, data) for index, (kind, data) in enumerate(values)]
    keys[1]['descriptor']['range'] = dict(kind='integer', minimum=0, maximum=20, step=1)
    keys[1]['integer_range'] = dict(minimum=0, maximum=20, step=1)
    keys[2]['descriptor']['range'] = dict(kind='floating', minimum=0, maximum=1, step=0.5)
    keys[2]['floating_range'] = dict(minimum=0, maximum=1, step=0.5)
    keys.append(key('zz.absent', 'integer', 1)); keys[-1]['override'] = None
    if algebra == 'ros1':
        keys[5]['descriptor']['type'] = 'integer'; keys[5]['descriptor']['dynamic_type'] = True
        keys[5]['types'] = ['integer', 'xmlrpc_array']
    return dict(profile=dict(identity='parameter-tool-' + algebra), node_profile=dict(identity='node', sha256='02' * 32),
                algebra=algebra, nonfinite=False, deferred_validation=False, floating_tolerance=0,
                search_root='/plant' if algebra == 'ros1' else '', search_candidates=2 if algebra == 'ros1' else 0, keys=keys)


def main():
    parser = argparse.ArgumentParser(description=__doc__); parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736' and (GROUP/'memory.swap.max').read_text().strip() == '0'
    args.output.mkdir(parents=True, exist_ok=False)
    negative = 0
    def rejected(document):
        nonlocal negative
        try: encoder.encode(document)
        except (ValueError, TypeError, UnicodeError, OverflowError): negative += 1
        else: raise AssertionError('Invalid definition was encoded')
    original = fixture('ros2')
    for field in original:
        value = copy.deepcopy(original); del value[field]; rejected(value)
    value = copy.deepcopy(original); value['implicit_namespace'] = '/'; rejected(value)
    for field in original['keys'][0]:
        value = copy.deepcopy(original); del value['keys'][0][field]; rejected(value)
    for field in original['keys'][0]['descriptor']:
        value = copy.deepcopy(original); del value['keys'][0]['descriptor'][field]; rejected(value)
    for bad in ('', 'a\0b', 'x' * 97, '\ud800'):
        value = copy.deepcopy(original); value['profile']['identity'] = bad; rejected(value)
    for bad in ('bad', '00' * 31, 'gg' * 32):
        value = copy.deepcopy(original); value['node_profile']['sha256'] = bad; rejected(value)
    for bad in ('ros3', '', None):
        value = copy.deepcopy(original); value['algebra'] = bad; rejected(value)
    for bad in ('', 1, [], {'type': 'boolean'}, {'type': 'integer', 'value': 1}):
        value = copy.deepcopy(original); value['keys'][0]['override'] = bad; rejected(value)
    for bad in (['boolean', 'boolean'], [], ['xmlrpc_array'], ['unknown']):
        value = copy.deepcopy(original); value['keys'][0]['types'] = bad; rejected(value)
    value = copy.deepcopy(original); value['keys'].reverse(); rejected(value)
    value = copy.deepcopy(original); value['keys'][1]['descriptor']['name'] = 'key.00'; rejected(value)
    value = copy.deepcopy(original); value['keys'][4]['override']['value'] = 'AB=='; rejected(value)
    value = copy.deepcopy(original); value['keys'][5]['override']['value'] = 0; rejected(value)
    value = copy.deepcopy(original); value['keys'][7]['override']['value'] = [1 << 63]; rejected(value)
    value = copy.deepcopy(original); value['keys'][2]['override']['value'] = 'nan'; rejected(value)
    value = copy.deepcopy(original); value['keys'][2]['override']['value'] = True; rejected(value)
    value = fixture('ros1'); value['keys'][1]['override']['value'] = 1 << 31; rejected(value)
    value = fixture('ros1'); value['keys'][3]['override']['value'] = '\0'; rejected(value)
    value = fixture('ros1'); value['keys'][7]['override']['value'] = '20230229T23:59:59'; rejected(value)
    value = fixture('ros1'); value['keys'][6]['override']['value'][1]['name'] = 'first'; rejected(value)
    value = fixture('ros1'); value['keys'][5]['override'] = dict(type='not_set', value=None); rejected(value)
    for root, candidates in [('', 1), ('/', 0), ('/plant', -1), ('/plant', 257), ('/plant', True), ('/plant', 1.0),
                             ('plant', 1), ('/plant/', 1), ('/plant//a', 1), ('/plant/../a', 1), ('/1bad', 1), ('/plant\0', 1)]:
        value = fixture('ros1'); value.update(search_root=root, search_candidates=candidates); rejected(value)
    value = fixture('ros2'); value.update(search_root='/', search_candidates=1); rejected(value)
    for root, candidates in [('', 0), ('/', 1), ('/plant/line_a', 256)]:
        value = fixture('ros1'); value.update(search_root=root, search_candidates=candidates)
        encoded, metadata = encoder.encode(value)
        assert metadata['profile']['sha256'] != encoder.encode(fixture('ros1'))[1]['profile']['sha256']
    try: json.loads('{"profile":1,"profile":2}', object_pairs_hook=encoder.object_pairs)
    except ValueError: negative += 1
    else: raise AssertionError('Duplicate JSON field accepted')
    fixtures = []
    for algebra in ('ros1', 'ros2'):
        value = fixture(algebra); data, metadata = encoder.encode(value)
        # Locate the body independently: text magic, text identity, digest blob.
        cursor = 0
        for _ in range(3):
            size = struct.unpack_from('<Q', data, cursor)[0]; cursor += 8 + size
        assert hashlib.sha256(data[cursor:]).hexdigest() == metadata['profile']['sha256']
        assert hashlib.sha256(data).hexdigest() == metadata['definition_sha256']
        assert metadata['profile']['sha256'] != metadata['definition_sha256']
        source = args.output/(algebra + '.json'); binary = args.output/(algebra + '.canonical')
        source.write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n'); binary.write_bytes(data)
        fixtures.append(dict(source=str(source), file=str(binary), **metadata))
    result = dict(complete=True, negative_cases=negative, fixtures=fixtures, cgroup=str(GROUP), cpu_affinity=sorted(os.sched_getaffinity(0)),
                  cpp_decode_qualified=False)
    (args.output/'results.json').write_text(json.dumps(result, indent=2) + '\n')
    print('passed ' + str(negative) + ' parameter definition tool refusals and 2 canonical encodings')


if __name__ == '__main__':
    main()
