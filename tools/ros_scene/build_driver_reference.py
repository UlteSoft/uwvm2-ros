#!/usr/bin/env python3
"""Build the matched native driver reference from an exact qualified VM cohort.

Reuse the VM's actual compiler invocation and frozen headers. Only the translation
unit, output and assertion setting change. The explicit native link receipt uses
the same bundled C++ runtime; no ROS/vendor library is linked into this client.
All compilation stays inside the existing 64 GiB resource gate.
"""
import argparse
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT/'test/0084.ros-runtime'))
from run_scene_vm import GATE, verify_gate, sha
from performance_build_policy import compile_policy, vm_policy


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--link-recipe', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args(); verify_gate()
    runtime = args.runtime.resolve(); build_path = runtime.parent/'full-build-results.json'
    build = json.loads(build_path.read_text())
    assert build['complete'] and build['binary_sha256'] == sha(runtime)
    source = Path(build['source_root']); manifest = source/'source-manifest.json'
    files = json.loads(manifest.read_text())
    assert all(sha(source/name) == digest for name, digest in files.items())
    fixture = source/'test/0084.ros-runtime/driver_roundtrip.cpp'
    assert files[str(fixture.relative_to(source))] == sha(fixture)
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    compile_path = runtime.parent/'main.default.cpp.o.argv.json'
    compile_command = json.loads(compile_path.read_text())
    original = str(source/'src/uwvm2/uwvm/main.default.cpp')
    assert compile_command.count(original) == 1 and compile_command.count('-o') == 1
    compile_command[compile_command.index(original)] = str(fixture)
    compile_command[compile_command.index('-o')+1] = str(out/'native.o')
    compile_command = [arg for arg in compile_command if arg not in ('-DNDEBUG', '-UNDEBUG')]
    compile_command.append('-UNDEBUG')
    policy = compile_policy(compile_command)
    link_recipe = args.link_recipe.resolve(); native = json.loads(link_recipe.read_text())
    assert native['complete']
    link_command = next(row['argv'] for row in native['commands'] if row['name'] == 'link').copy()
    assert link_command[0] == compile_command[0] and link_command.count('-o') == 1
    objects = [arg for arg in link_command if arg.endswith('.o')]
    assert len(objects) == 1
    link_command[link_command.index(objects[0])] = str(out/'native.o')
    link_command[link_command.index('-o')+1] = str(out/'native')
    inputs = {str(path): sha(path) for path in (build_path, manifest, compile_path, fixture, link_recipe, Path(__file__))}
    for arg in link_command:
        if arg.endswith(('.a', '.so')):
            path = Path(arg); assert path.is_file(); inputs[str(path)] = sha(path)
    report = dict(complete=False, scope=__doc__, source_root=str(source), runtime_sha256=sha(runtime),
                  compile_policy=policy, vm_policy=vm_policy(runtime), inputs=inputs, commands=[],
                  cgroup=str(GATE), events_before=(GATE/'memory.events').read_text())
    def save():
        (out/'native-build.json').write_text(json.dumps(report, indent=2)+'\n')
    try:
        for name, command in [('compile', compile_command), ('link', link_command)]:
            verify_gate()
            result = subprocess.run(command, cwd=ROOT, capture_output=True, timeout=600)
            (out/(name+'.stdout')).write_bytes(result.stdout); (out/(name+'.stderr')).write_bytes(result.stderr)
            report['commands'].append(dict(name=name, argv=command, exit=result.returncode)); save()
            assert result.returncode == 0, result.stderr.decode(errors='replace')[-6000:]
            print('PASS '+name, flush=True)
        report['native_sha256'] = sha(out/'native')
        report['sources_unchanged'] = all(sha(source/name) == digest for name, digest in files.items())
        report['inputs_unchanged'] = all(sha(path) == digest for path, digest in inputs.items())
        report['complete'] = report['sources_unchanged'] and report['inputs_unchanged']
    finally:
        report['events_after'] = (GATE/'memory.events').read_text(); save()
    assert report['complete']


if __name__ == '__main__':
    main()
