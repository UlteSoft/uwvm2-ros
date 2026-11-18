#!/usr/bin/env python3
"""Qualify real five-ABI ELF closure packing, not execution or ROS middleware.

Cross-compile minimal ELF dependency graphs at -O3 -g without target system
libraries, then package their actual PT_INTERP/DT_NEEDED closure. Mutations must
reject wrong class, machine, endianness, ARM PCS and escaped target libraries.
All compilers and checks stay inside the existing 64 GiB, zero-swap gate.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def guard():
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip() == '68719476736'
    assert (GATE/'memory.swap.max').read_text().strip() == '0'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    guard()
    output = args.output.resolve()
    assert output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    output.mkdir(parents=True, exist_ok=False)
    original = ROOT/'tools/ros_scene/pack_communication_worker.py'
    helper = output/'pack_communication_worker.py'
    shutil.copyfile(original, helper)
    spec = importlib.util.spec_from_file_location('pinned_pack', helper)
    pack = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(pack)
    compiler = Path('/usr/bin/clang-22').resolve()
    inputs = {str(p): sha(p) for p in (original, Path(__file__).resolve(), compiler)}
    record = dict(complete=False, scope=__doc__, inputs=inputs, commands=[], checks=[],
        cgroup=str(GATE), memory_max=68719476736, swap_max=0,
        events_before=(GATE/'memory.events').read_text(), native_execution_qualified=False)

    def save():
        (output/'results.json').write_text(json.dumps(record, indent=2)+'\n')

    def run(name, argv, success=True):
        guard()
        before = time.monotonic()
        done = subprocess.run([str(a) for a in argv], capture_output=True, text=True, timeout=120)
        (output/(name+'.stdout')).write_text(done.stdout)
        (output/(name+'.stderr')).write_text(done.stderr)
        passed = done.returncode == 0 if success else done.returncode > 0
        record['commands'].append(dict(name=name, argv=[str(a) for a in argv], exit=done.returncode,
            passed=passed, expected_success=success, seconds=time.monotonic()-before))
        save()
        assert passed, (name, done.stderr[-4000:])

    def refused(name, body, architecture):
        try:
            pack.inspect_elf(body, architecture)
        except ValueError:
            record['checks'].append(dict(name=name, passed=True))
        else:
            raise AssertionError(name)

    driver = ('import importlib.util,pathlib,sys;'
              's=importlib.util.spec_from_file_location("frozen_pack",sys.argv[1]);'
              'm=importlib.util.module_from_spec(s);s.loader.exec_module(m);'
              'm.ROOT=pathlib.Path(sys.argv[2]);sys.argv=[sys.argv[1],*sys.argv[3:]];m.main()')
    source = output/'dependency.c'
    source.write_text('int approved_dependency(void) { return 7; }\n')
    backend = output/'backend.c'
    backend.write_text('extern int approved_dependency(void); int approved_backend(void) { return approved_dependency(); }\n')
    startup = output/'startup.c'
    startup.write_text('extern int approved_dependency(void); int _start(void) { return approved_dependency(); }\n')
    try:
        for architecture, (_, _, library_triple, interpreter) in pack.LINUX_ABIS.items():
            root = output/architecture
            system = root/'system'
            library = system/'usr/lib'/library_triple
            ros = root/'ros'
            library.mkdir(parents=True)
            (ros/'opt/ros/jazzy/lib').mkdir(parents=True)
            hooks = ros/'opt/ros/jazzy/share/ros_environment/environment'
            hooks.mkdir(parents=True)
            (hooks/'1.ros_distro.dsv').write_text('set;ROS_DISTRO;jazzy\n')
            (hooks/'1.ros_version.dsv').write_text('set;ROS_VERSION;2\n')
            (ros/'system-libs').mkdir()
            profile = root/'profile'
            profile.write_bytes(b'bounded configuration, never executed')
            triple = {'i686': 'i686-linux-gnu', 'arm': 'arm-linux-gnueabi', 'armhf': 'arm-linux-gnueabihf'}.get(architecture, architecture+'-linux-gnu')
            flags = [compiler, '--target='+triple, '-O3', '-g', '-fPIC', '-fuse-ld=lld', '-nostdlib']
            if architecture in ('arm', 'armhf'):
                flags += ['-march=armv7-a', '-mfpu=vfpv3-d16', '-mfloat-abi='+('hard' if architecture == 'armhf' else 'soft')]
            shared = library/'libapproved.so'
            loader = system/interpreter.lstrip('/')
            loader.parent.mkdir(parents=True, exist_ok=True)
            run(architecture+'-dependency', [*flags, '-shared', '-Wl,-soname,libapproved.so', source, '-o', shared])
            run(architecture+'-loader', [*flags, '-shared', source, '-o', loader])
            image = root/'backend.so'
            boot = root/'bootstrap'
            run(architecture+'-backend', [*flags, '-shared', backend, '-L'+str(library), '-lapproved', '-o', image])
            run(architecture+'-bootstrap', [*flags, '-no-pie', '-Wl,-e,_start', '-Wl,--dynamic-linker,'+interpreter,
                startup, '-L'+str(library), '-lapproved', '-o', boot])
            body = boot.read_bytes()
            abi = pack.inspect_elf(body, architecture)
            record['checks'].append(dict(name=architecture+'-real-elf-abi', passed=True, abi=abi, sha256=sha(boot)))
            common = ['/usr/bin/python3', '-c', driver, helper, ROOT, '--architecture', architecture,
                '--system-root', system, '--ros-root', ros, '--bootstrap', boot, '--backend', image,
                '--launcher', boot, '--profile', profile]
            run(architecture+'-pack', [*common, '--output', root/'packed'])
            packed = json.loads((root/'packed/manifest.json').read_text())
            assert packed['complete'] and not packed['qualified_runtime']
            assert {row['destination'] for row in packed['files']} == {'/worker/bootstrap', '/worker/backend.so',
                '/worker/profile', '/usr/lib/'+library_triple+'/libapproved.so', interpreter}
            assert all(row.get('abi', {}).get('architecture', architecture) == architecture for row in packed['files'])
            for length in (0, 7, 19, 35, 51):
                refused(architecture+'-truncated-'+str(length), body[:length], architecture)
            for name, index, value in (('class', 4, 1 if body[4] == 2 else 2), ('endian', 5, 2),
                                       ('machine', 18, 0), ('version', 20, 2), ('header-size', 52 if body[4] == 2 else 40, 0)):
                bad = bytearray(body)
                bad[index] = value
                refused(architecture+'-'+name, bad, architecture)
            if architecture in ('arm', 'armhf'):
                refused(architecture+'-opposite-pcs', body, 'armhf' if architecture == 'arm' else 'arm')
                bad = bytearray(body)
                bad[36:40] = (0x05000600).to_bytes(4, 'little')
                refused(architecture+'-ambiguous-pcs', bad, architecture)
            # No approved target library exists after replacement. Its name
            # must never resolve to an unrelated host file through a symlink.
            shared.unlink()
            shared.symlink_to('/bin/true')
            run(architecture+'-escaped-library', [*common, '--output', root/'escaped'], False)
            if architecture != 'x86_64':
                run(architecture+'-host-root-refused', [*common, '--system-root', '/', '--output', root/'host-root'], False)
            print('PASS '+architecture+' ELF closure and refusal checks', flush=True)
        record['inputs_unchanged'] = all(sha(path) == value for path, value in inputs.items())
        assert record['inputs_unchanged']
        record['complete'] = True
    finally:
        record['events_after'] = (GATE/'memory.events').read_text()
        save()


if __name__ == '__main__':
    main()
