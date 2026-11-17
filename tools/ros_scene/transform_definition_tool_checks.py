#!/usr/bin/env python3
"""Retain valid and invalid transform encodings for independent C++ qualification."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import transform_definition as tool

GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def fixture():
    return dict(identity='frames', frames=['base', 'map'], edges=[dict(parent=1, child=0, dynamic=True, static=False)],
                physical_limits=dict(translation_metres=1000, quaternion_tolerance=1e-12, linear_metres_per_second=10, angular_radians_per_second=5))


def unchecked(frames=(b'base', b'map'), edges=((1, 0, 1, 0),), bounds=(1000, 1e-12, 10, 5), magic=None, identity=b'frames'):
    writer = tool.Writer(); writer.blob(tool.MAGIC.encode() if magic is None else magic); writer.blob(identity); writer.u64(len(frames))
    for frame in frames: writer.blob(frame)
    writer.u64(len(edges))
    for edge in edges:
        for item in edge: writer.u64(item)
    for bound in bounds: writer.f64(bound)
    return bytes(writer.data)


def main():
    parser = argparse.ArgumentParser(description=__doc__); parser.add_argument('--output', type=Path, required=True); args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736' and (GROUP/'memory.swap.max').read_text().strip() == '0'
    args.output.mkdir(parents=True, exist_ok=False); negative = 0
    def denied(value):
        nonlocal negative
        try: tool.encode(value)
        except (ValueError, TypeError, UnicodeError, OverflowError): negative += 1
        else: raise AssertionError('Invalid transform policy accepted')
    original = fixture()
    for key in original:
        value = copy.deepcopy(original); del value[key]; denied(value)
    for key in original['physical_limits']:
        value = copy.deepcopy(original); del value['physical_limits'][key]; denied(value)
    for key in original['edges'][0]:
        value = copy.deepcopy(original); del value['edges'][0][key]; denied(value)
    for bad in ('', 'a\0b', 'x'*97, '\ud800', 17):
        value = fixture(); value['identity'] = bad; denied(value)
    for bad in ([], ['x']*129, ['base', 'base'], ['map', 'base'], ['', 'map'], ['base', '€'*86], ['base', 'm\0ap'], ['base', '\ud800'], {}):
        value = fixture(); value['frames'] = bad; denied(value)
    for field in ('parent', 'child'):
        for bad in (-1, 2, True, 0.0, 'base', None):
            value = fixture(); value['edges'][0][field] = bad; denied(value)
    for field in ('dynamic', 'static'):
        for bad in (0, 1, 'false', None):
            value = fixture(); value['edges'][0][field] = bad; denied(value)
    value = fixture(); value['edges'][0]['child'] = 1; denied(value)
    value = fixture(); value['edges'][0]['dynamic'] = False; denied(value)
    value = fixture(); value['edges'] *= 2; denied(value)
    value = fixture(); value['edges'] *= 513; denied(value)
    value = fixture(); value['edges'].append(dict(parent=0, child=1, dynamic=False, static=True)); denied(value)
    for field in tool.PHYSICAL_FIELDS:
        for bad in (-1, float('inf'), float('nan'), True, '1'):
            value = fixture(); value['physical_limits'][field] = bad; denied(value)
    value = fixture(); value['physical_limits']['quaternion_tolerance'] = 1; denied(value)
    for value in (dict(original, grant=True), dict(original, path='/tmp/implicit'), dict(original, limits={})):
        denied(value)
    value = fixture(); value['edges'][0]['owner'] = 0; denied(value)
    value = fixture(); value['physical_limits']['rate'] = 1; denied(value)
    for raw in (b'{"identity":"a","identity":"b"}', b'{"physical_limits":{"x":1,"x":2}}', b'{"bound":NaN}', b'{"bound":Infinity}', b' '*(tool.MAXIMUM_INPUT+1)):
        try: tool.parse(raw)
        except ValueError: negative += 1
        else: raise AssertionError('Invalid JSON input accepted')
    fixtures = []; commands = []
    def retain(name, data, identity='frames', accept=False):
        path = args.output/(name+'.canonical'); path.write_bytes(data)
        fixtures.append(dict(path=str(path), identity=identity, sha256=hashlib.sha256(data).hexdigest(), bytes=len(data), expected='accept' if accept else 'reject'))
    values = [('ordinary', fixture())]
    unicode = fixture(); unicode['identity'] = '机器人'; unicode['frames'] = ['base', 'é' * 127 + 'a']; values.append(('unicode-255', unicode))
    empty = fixture(); empty['frames'] = ['world']; empty['edges'] = []; empty['physical_limits'] = {name: 0 for name in tool.PHYSICAL_FIELDS}; values.append(('zero-bounds', empty))
    maximal = fixture(); maximal['frames'] = [('frame'+str(index).zfill(3)).ljust(255, 'x') for index in range(128)]
    maximal['edges'] = [dict(parent=parent, child=child, dynamic=True, static=True) for parent in range(128) for child in range(parent+1, 128)][:512]
    values.append(('maximum-counts', maximal))
    for name, value in values:
        encoded, metadata = tool.encode(value); retain(name, encoded, value['identity'], True)
        source = args.output/(name+'.json'); source.write_text(json.dumps(value, ensure_ascii=False, indent=2)+'\n')
        output = args.output/(name+'.cli.canonical'); report = args.output/(name+'.metadata.json')
        argv = ['/usr/bin/python3', str(Path(tool.__file__)), '--input', str(source), '--output', str(output), '--metadata', str(report), '--environment', 'plant', '--alias', 'frames']
        result = subprocess.run(argv, capture_output=True, text=True, timeout=10); commands.append(dict(argv=argv, exit=result.returncode)); assert result.returncode == 0, result.stderr
        assert output.read_bytes() == encoded; displayed = json.loads(result.stdout); assert json.loads(report.read_text()) == displayed
        assert displayed['sha256'] == metadata['sha256'] and not displayed['authority_granted'] and displayed['required_separate_limits'] == list(tool.LIMIT_NAMES)
        assert len(displayed['suggested_cli_arguments']) == 2 and all('grant' not in word for command in displayed['suggested_cli_arguments'] for word in command)
        repeated = subprocess.run(argv, capture_output=True, text=True, timeout=10); commands.append(dict(argv=argv, exit=repeated.returncode)); assert repeated.returncode != 0
        assert output.read_bytes() == encoded
    reordered = copy.deepcopy(maximal); reordered['edges'].reverse(); assert tool.encode(reordered)[0] == tool.encode(maximal)[0]
    malformed = [
        ('magic', unchecked(magic=b'wrong')),
        ('identity', unchecked(identity=b'other')),
        ('unsorted', unchecked(frames=(b'map', b'base'))),
        ('duplicate-frame', unchecked(frames=(b'base', b'base'))),
        ('invalid-utf8', unchecked(frames=(b'base', b'\xff'))),
        ('nul-frame', unchecked(frames=(b'base', b'm\0ap'))),
        ('long-frame', unchecked(frames=(b'base', b'x'*256))),
        ('empty-frame', unchecked(frames=(b'', b'map'))),
        ('too-many-frames', unchecked(frames=tuple(('f'+str(i).zfill(3)).encode() for i in range(129)), edges=())),
        ('too-many-edges', unchecked(edges=((1, 0, 1, 0),)*513)),
        ('bad-index', unchecked(edges=((2, 0, 1, 0),))),
        ('boolean', unchecked(edges=((1, 0, 2, 0),))),
        ('self-edge', unchecked(edges=((1, 1, 1, 0),))),
        ('no-mode', unchecked(edges=((1, 0, 0, 0),))),
        ('duplicate-edge', unchecked(edges=((1, 0, 1, 0), (1, 0, 0, 1)))),
        ('union-cycle', unchecked(edges=((1, 0, 1, 0), (0, 1, 0, 1)))),
        ('nan', unchecked(bounds=(float('nan'), 0, 1, 1))),
        ('infinity', unchecked(bounds=(1, 0, float('inf'), 1))),
        ('negative-speed', unchecked(bounds=(1, 0, 1, -1))),
        ('quaternion', unchecked(bounds=(1, 1, 1, 1))),
        ('trailing', unchecked()+b'\0'),
    ]
    for name, data in malformed: retain(name, data)
    canonical = unchecked()
    for cut in (1, 7, 8, 31, 57, 95, len(canonical)-1): retain('truncated-'+str(cut), canonical[:cut])
    result = dict(complete=True, json_refusals=negative, canonical_accepts=len(values), canonical_refusals=len(fixtures)-len(values),
                  commands=commands, fixtures=fixtures, cpp_decode_qualified=False, cgroup=str(GROUP))
    (args.output/'results.json').write_text(json.dumps(result, indent=2)+'\n'); print('passed transform encoder JSON/CLI cases; retained '+str(len(fixtures))+' C++ fixtures')


if __name__ == '__main__':
    main()
