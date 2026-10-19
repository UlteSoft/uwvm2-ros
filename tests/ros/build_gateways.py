#!/usr/bin/env python3
"""Build and freeze prepared official ROS rootfs targets in the provided cgroup."""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import signal
import subprocess
import sys
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path, required=True)
    parser.add_argument('--tag', default='v5-scalar')
    args = parser.parse_args()
    if not re.fullmatch(r'[a-z0-9]+(?:-[a-z0-9]+)*', args.tag):
        parser.error('tag must contain lowercase letters, digits and single hyphens')
    root = args.workspace.resolve()
    source = root/'source'
    sys.path.insert(0, str(source/'tests/ros_cgroup'))
    from probe import probe
    before = probe()
    if before['status'] != 'PASS':
        print('BLOCKED provided64GiB cgroup not proven'); return 77
    out = root/'gateway'/('final-'+args.tag+'-provenance')
    out.mkdir()
    digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
    # Files consumed by these targets are frozen before the first compiler starts.
    def inputs():
        return {str(p.relative_to(source)):digest(p)
                for directory in ('gateways','src/uwvm2/imported/ros','tests')
                for p in (source/directory).rglob('*')
                if p.is_file() and p.suffix in ('.h','.hpp','.cc','.cpp','.txt','.cmake')}
    initial = inputs()
    rows, toolchains, compiled, binaries = [], {}, {}, {}
    start = time.monotonic()
    try:
        for version, distribution in ((1,'noetic'),(2,'jazzy')):
            fs = root/'gateway'/(distribution+'-rootfs')
            build = fs/f'tmp/ros{version}-{args.tag}-build'
            canonical = fs/f'tmp/ros{version}-build'
            if build.exists() or not canonical.is_symlink():
                raise RuntimeError('fresh build and preexisting canonical symlink required')
            if digest(fs/'work/gateways'/f'ros{version}'/'main.cc') != digest(source/'gateways'/f'ros{version}'/'main.cc'):
                raise RuntimeError('prepared /work source mount mismatch')
            script = (f'source /opt/ros/{distribution}/setup.bash && '
                      f'cmake -S /work/gateways/ros{version} -B /tmp/{build.name} -DCMAKE_BUILD_TYPE=RelWithDebInfo && '
                      f'cmake --build /tmp/{build.name} --parallel 1')
            # The Worker compiler belongs to the host, outside this ROS rootfs.
            # Select the distribution's native compiler and library environment.
            command = ['chroot',str(fs),'/usr/bin/env','-u','CC','-u','CXX','-u','LD_LIBRARY_PATH','/bin/bash','-lc',script]
            row = {'version':version,'command':command,'canonical_previous':os.readlink(canonical)}
            rows.append(row)
            with (out/f'ros{version}-build.log').open('wb') as log:
                process = subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
                row.update(pid=process.pid,membership=Path(f'/proc/{process.pid}/cgroup').read_text().splitlines())
                if row['membership'] != before['membership']:
                    os.killpg(process.pid,signal.SIGKILL);process.wait();raise RuntimeError('builder cgroup mismatch')
                try: code = process.wait(timeout=600)
                except subprocess.TimeoutExpired:
                    os.killpg(process.pid,signal.SIGKILL);process.wait(timeout=5);raise
            row.update(returncode=code,status='PASS' if code==0 else 'FAIL')
            print(row['status'],f'ROS{version} native targets',flush=True)
            if code: raise RuntimeError(f'ROS{version} native build failed')
            packages = subprocess.check_output(['chroot',str(fs),'/usr/bin/dpkg-query','-W','-f=${Package}\t${Version}\n'],timeout=30)
            (out/f'ros{version}-packages.txt').write_bytes(packages)
            metadata = {}
            for p in [build/'CMakeCache.txt',*build.rglob('flags.make'),*build.rglob('link.txt')]:
                dest = out/f'ros{version}-build-metadata'/p.relative_to(build)
                dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(p.read_bytes())
                metadata[str(p.relative_to(build))] = {'sha256':digest(p),'archive':str(dest.relative_to(out))}
            toolchains[str(version)] = {'rootfs':str(fs),'compiler_version':subprocess.check_output(['chroot',str(fs),'/usr/bin/g++','--version'],text=True,timeout=15),
                'packages_file':f'ros{version}-packages.txt','packages_sha256':digest(out/f'ros{version}-packages.txt'),'build_metadata':metadata}
            # No inferred inventory: archive project paths recorded by the native compiler.
            for dep in [*build.rglob('*.o.d'),*build.rglob('depend.make')]:
                names = set(posixpath.normpath(x.rstrip(':'))[len('/work/'):] for x in re.findall(r'/work/[^\s\\]+',dep.read_text()) if not x.rstrip(':').endswith('.o'))
                if names:
                    values = {}
                    for name in sorted(names):
                        p = source/name
                        if not p.resolve().is_relative_to(source) or name not in initial:
                            raise RuntimeError('compiler input outside frozen source inventory: '+name)
                        actual = fs/'work'/name
                        if not actual.is_file() or digest(actual)!=initial[name] or digest(p)!=initial[name]:
                            raise RuntimeError('actual compiler /work dependency mismatch: '+name)
                        values[name] = initial[name]
                    compiled[f'ros{version}:{dep.relative_to(build)}'] = values
            for p in build.glob(f'uwvm-ros{version}-*'):
                if p.is_file(): binaries[f'ros{version}:{p.name}'] = {'path':str(p),'canonical_path':str(canonical/p.name),'sha256':digest(p),'bytes':p.stat().st_size}
        if len(binaries) != 18:
            raise RuntimeError('expected18 native targets')
        if inputs()!=initial: raise RuntimeError('source inventory changed during build')
        after = probe()
        if after['status'] != 'PASS': raise RuntimeError('final cgroup verification failed')
        def events(snapshot):
            return dict(line.split() for line in snapshot['cgroup_ancestors'][0]['memory.events'].splitlines())
        if any(events(before).get(key)!=events(after).get(key) for key in ('oom','oom_kill')):
            raise RuntimeError('OOM counter increased')
        names = {name for values in compiled.values() for name in values}
        names.update(f'gateways/ros{v}/CMakeLists.txt' for v in (1,2))
        inventory = {}
        for name in sorted(names):
            p = source/name;dest = out/'source-inputs'/name
            dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(p.read_bytes())
            inventory[name] = {'sha256':digest(p),'bytes':p.stat().st_size}
        # Publish the canonical paths only once every native target and input check passed.
        for version,distribution in ((1,'noetic'),(2,'jazzy')):
            parent = root/'gateway'/(distribution+'-rootfs')/'tmp'
            temporary = parent/f'ros{version}-build-{args.tag}-new'
            temporary.symlink_to(f'ros{version}-{args.tag}-build')
            temporary.replace(parent/f'ros{version}-build')
        status = 'FROZEN'
        manifest = {'created_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'status':status,'source_root':str(source),
            'source_inventory':inventory,'compiled_source_dependencies':compiled,'binaries':binaries,'toolchains':toolchains,
            'limits':'Compiler-recorded project dependencies and CMake files. Installed middleware pinned by package inventories; native ROS libraries are not sanitizer instrumented.'}
        (out/'gateway-frozen-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    except Exception as error:
        status = 'FAIL';rows.append({'status':'FAIL','error':str(error)})
    after = probe()
    report = {'status':'PASS' if status=='FROZEN' else 'FAIL','seconds':round(time.monotonic()-start,3),
        'builds':rows,'parallelism':1,'cgroup_before':before,'cgroup_after':after,'manifest':str(out/'gateway-frozen-manifest.json')}
    (out/'build-report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],out,flush=True)
    return 0 if report['status']=='PASS' else 1


if __name__ == '__main__': raise SystemExit(main())
