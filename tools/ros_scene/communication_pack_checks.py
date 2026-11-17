#!/usr/bin/env python3
"""Exercise real ELF closure packing and reject unknown/ambiguous file authority."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--worker-fixture', type=Path, required=True)
    parser.add_argument('--ros-root', type=Path, required=True)
    parser.add_argument('--profile', type=Path, required=True)
    args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset({12,13})
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    source = out/'pack_communication_worker.py'
    source.write_bytes((ROOT/'tools/ros_scene/pack_communication_worker.py').read_bytes())
    images = args.worker_fixture.resolve()/'images'
    common = ['--bootstrap', str(images/'bootstrap'), '--backend', str(images/'backend.so'),
        '--profile', str(args.profile.resolve()), '--ros-root', str(args.ros_root.resolve()), '--launcher', str(images/'bwrap')]
    ros = args.ros_root.resolve()/'opt/ros/jazzy'
    resource = 'share/ament_index/resource_index/rmw_typesupport_cpp/rmw_fastrtps_cpp'
    trace = out/'synthetic-complete-open.strace'
    trace.write_text('10 openat(AT_FDCWD, '+json.dumps(str(ros/'lib/librmw_fastrtps_cpp.so'))+', O_RDONLY|O_CLOEXEC <unfinished ...>\n'
        '11 openat(AT_FDCWD, '+json.dumps(str(ros/resource))+', O_RDONLY) = 3\n'
        '10 <... openat resumed>) = 4\n'
        '11 openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 5\n')
    unknown = out/'unknown-open.strace'
    unknown.write_text('openat(AT_FDCWD, "/etc/passwd", O_RDONLY) = 3\n')
    partial = out/'partial-open.strace'
    partial.write_text('10 openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY <unfinished ...>\n')
    pins = {str(path):digest(path) for path in [Path(__file__), source, images/'bootstrap', images/'backend.so', images/'bwrap', args.profile.resolve(), trace, unknown, partial]}
    receipt = dict(complete=False, scope='actual readelf packaging; trace parser uses explicit synthetic records, not ROS runtime proof',
        commands=[], inputs=pins, events_before=(GROUP/'memory.events').read_text(), cgroup=str(GROUP), memory_max=68719476736, swap_max=0)
    # Keep source lookup/output constraints anchored at the actual workspace
    # while executing the frozen helper, rather than importing the live copy.
    script = 'import importlib.util,pathlib,sys; s=importlib.util.spec_from_file_location("pinned_pack",sys.argv[1]); m=importlib.util.module_from_spec(s); s.loader.exec_module(m); m.ROOT=pathlib.Path(sys.argv[2]); sys.argv=[sys.argv[1],*sys.argv[3:]]; m.main()'
    def run(name, extra, success):
        argv = ['/usr/bin/python3','-c',script,str(source),str(ROOT),*common,'--output',str(out/name),*extra]
        done = subprocess.run(argv,capture_output=True,text=True,timeout=90)
        (out/(name+'.stdout')).write_text(done.stdout)
        (out/(name+'.stderr')).write_text(done.stderr)
        receipt['commands'].append(dict(name=name,argv=argv,exit=done.returncode,expected_success=success))
        assert (done.returncode==0)==success,(name,done.stderr)
        packed = json.loads((out/name/'manifest.json').read_text())
        assert packed['complete']==success and not packed['qualified_runtime']
        if success:
            for entry in [packed['launcher'],*packed['files']]:
                assert digest(entry['path'])==entry['sha256'] and Path(entry['path']).stat().st_size==entry['bytes']
            assert packed['required_image_bytes']==packed['launcher']['bytes']+2*sum(x['bytes'] for x in packed['files'])
        print(name,done.returncode,flush=True)
        return packed
    try:
        packed = run('elf-resource',['--dlopen','librmw_fastrtps_cpp.so','--resource',resource],True)
        assert len(packed['files'])>=30 and any(x['destination'].endswith(resource) and x['role']=='configuration' for x in packed['files'])
        traced = run('traced-lazy',['--trace',str(trace)],True)
        assert {x['destination']:x['sha256'] for x in packed['files']}=={x['destination']:x['sha256'] for x in traced['files']}
        run('missing-library',['--dlopen','lib_missing_approved_worker.so'],False)
        run('resource-traversal',['--resource','../../etc/passwd'],False)
        run('image-budget',['--max-image-bytes','4096'],False)
        run('unknown-successful-open',['--trace',str(unknown)],False)
        run('incomplete-trace',['--trace',str(partial)],False)
        run('reviewed-absent-probe',['--trace',str(unknown),'--transient-probe','/etc/passwd','Fixture parser test only; not permitted or mounted in worker'],True)
        receipt['complete']=all(digest(path)==value for path,value in pins.items())
        assert receipt['complete']
    finally:
        receipt['events_after']=(GROUP/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(receipt,indent=2)+'\n')


if __name__=='__main__':
    main()
