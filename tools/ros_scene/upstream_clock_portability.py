#!/usr/bin/env python3
"""Compare an actual pinned Jazzy time-source slice natively and through rosv2.

All compilers/tests must inherit the existing 64 GiB zero-swap gate. Upstream
archives are checksum-verified against acquisition receipts and safely extracted
under SDK .tmp. Upstream C logic remains unmodified. Only the POSIX clock read
boundary uses a narrow guest adapter with exact scene grants. This is source
slice qualification, not an rcl/rmw/rcutils package or ROS middleware port.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tarfile
import time

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
SOURCES = {
    'rcl': ('cfe0693454c5bdddceba24a5', 'rcl-jazzy/rcl'),
    'rmw': ('f874b47691ccd0b3e8bc952d', 'rmw-jazzy/rmw'),
    'rcutils': ('bc07f0c0c038874d9c7bd440', 'rcutils-jazzy'),
    'rosidl': ('b5446b59a66b3f67efff941b', 'rosidl-jazzy/rosidl_runtime_c'),
}


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def finalize(record, output, runtime, checks_passed, source_root=ROOT):
    """Publish success only after every recorded source/artifact hash still matches.

    Missing or unreadable inputs fail just like content changes. Save the failed
    receipt before raising, so the process status and reviewable evidence agree.
    A pre-existing test exception remains the primary failure when checks did
    not finish; integrity failures still prevent a completed qualification.
    """
    groups = {
        'fixture_sources_unchanged': [(source_root/name,digest) for name,digest in record['fixture_sha256'].items()],
        'runtime_unchanged': [(runtime,record['runtime']['sha256'])],
        'fixture_snapshots_unchanged': [(output/name,digest) for name,digest in record.get('fixture_snapshot_sha256',{}).items()],
        'upstream_sources_unchanged': [(output/name,digest) for name,digest in record.get('compiled_upstream_sha256',{}).items()],
        'upstream_headers_unchanged': [(output/name,digest) for name,digest in record.get('upstream_header_sha256',{}).items()],
        'guest_artifacts_unchanged': [(output/name,digest) for name,digest in record.get('guest_sha256',{}).items()],
    }
    if 'logging_header' in record:
        groups['generated_logging_header_unchanged'] = [(output/'include/rcutils/logging_macros.h',record['logging_header']['generated_sha256'])]
    failures = []
    for group, entries in groups.items():
        valid = True
        for path, expected in entries:
            try:
                unchanged = sha(path) == expected
            except OSError:
                unchanged = False
            if not unchanged:
                valid = False
                failures.append({'group':group,'path':str(path)})
        record[group] = valid
    record['integrity_failures'] = failures
    record['complete'] = checks_passed and not failures
    record['events_after'] = (GROUP/'memory.events').read_text()
    (output/'results.json').write_text(json.dumps(record, indent=2) + '\n')
    if checks_passed and failures:
        raise RuntimeError('Qualification inputs changed: '+', '.join(item['path'] for item in failures))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--corpus', type=Path, required=True)
    parser.add_argument('--sysroot', type=Path, required=True)
    parser.add_argument('--builtins', type=Path, required=True)
    parser.add_argument('--empy-module', type=Path, required=True, help='Real upstream/packaged em.py; not a generated-header stub')
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--clang', default='/usr/bin/clang-22')
    parser.add_argument('--wasm-ld', default='/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/wasm-ld')
    parser.add_argument('--wasm2wat', default='wasm2wat')
    args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736'
    assert (GROUP/'memory.swap.max').read_text().strip() == '0'
    assert os.sched_getaffinity(0).issubset(set(range(8, 16))), 'Reserve CPUs 2–3 for performance and 4–7 for module tests'
    out = args.output.resolve();out.mkdir(parents=True, exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve()), 'Artifacts belong in SDK .tmp'
    assert not (out/'sources').exists(), 'Use a fresh output directory to preserve previous source evidence'
    inputs = [ROOT/'test/0084.ros-runtime/upstream_clock_semantics.c', ROOT/'test/0084.ros-runtime/upstream_clock_adapter.c']
    record = {'complete': False, 'scope': 'Unmodified Jazzy time-source slice; not whole ROS packages or middleware',
              'cgroup': str(GROUP), 'memory_max': 68719476736, 'memory_swap_max': 0,
              'cpu_affinity': sorted(os.sched_getaffinity(0)), 'events_before': (GROUP/'memory.events').read_text(),
              'runtime': {'path': str(args.runtime.resolve()), 'sha256': sha(args.runtime)}, 'commands': [],
              'fixture_sha256': {str(path.relative_to(ROOT)): sha(path) for path in inputs}, 'upstream': {}}
    checks_passed = False

    def save():
        (out/'results.json').write_text(json.dumps(record, indent=2) + '\n')

    def run(label, argv, env=None):
        started = time.monotonic()
        result = subprocess.run([str(item) for item in argv], cwd=ROOT, capture_output=True, text=True, timeout=120, env=env)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append({'name': label, 'argv': [str(item) for item in argv], 'exit': result.returncode,
                                   'wall_seconds': time.monotonic()-started})
        save()
        if result.returncode:
            raise RuntimeError(label+': '+result.stderr[-5000:])
        return result.stdout

    try:
        roots = {}
        for name, (identity, subpath) in SOURCES.items():
            receipt = args.corpus/'receipts'/(identity+'.json')
            received = json.loads(receipt.read_text());archive = args.corpus/'archives'/(identity+'.tar.gz')
            assert sha(archive) == received['archive']['sha256'], 'Archive differs from acquisition receipt'
            destination = out/'sources'/name;destination.mkdir(parents=True)
            with tarfile.open(archive) as bundle:
                bundle.extractall(destination, filter='data')
            roots[name] = destination/subpath
            record['upstream'][name] = {'source': received['source'], 'archive': received['archive'],
                                        'receipt_sha256': sha(receipt), 'local_root': str(roots[name])}
            shutil.copyfile(receipt, out/(name+'.acquisition.json'))
        generated = out/'include/rcutils/logging_macros.h';generated.parent.mkdir(parents=True)
        sys.path.insert(0, str(args.empy_module.resolve().parent))
        import em
        assert Path(em.__file__).resolve() == args.empy_module.resolve()
        em.invoke(['-o', str(generated), '-D', 'rcutils_module_path='+repr(str(roots['rcutils'])),
                   str(roots['rcutils']/'resource/logging_macros.h.em')])
        record['logging_header'] = {'generator_sha256': sha(args.empy_module), 'generated_sha256': sha(generated)}
        for source in inputs:
            shutil.copyfile(source, out/source.name)
        record['fixture_snapshot_sha256'] = {source.name:sha(out/source.name) for source in inputs}
        sources = [roots['rcl']/'src/rcl/time.c', roots['rmw']/'src/time.c'] + [
            roots['rcutils']/('src/'+name+'.c') for name in ('time_unix','allocator','error_handling','strdup','snprintf')]
        record['compiled_upstream_sha256'] = {str(path.relative_to(out)): sha(path) for path in sources}
        record['upstream_header_sha256'] = {str(path.relative_to(out)): sha(path) for root in roots.values()
                                           for tree in ('include','src') for path in (root/tree).rglob('*.h')}
        record['toolchain'] = {'clang_sha256': sha(shutil.which(args.clang)), 'wasm_ld_sha256': sha(args.wasm_ld),
                                'builtins_sha256': sha(args.builtins),
                                'libc_sha256': sha(args.sysroot/'lib/wasm32-wasip1/libc.a')}
        record['compiler_version'] = run('compiler-version', [args.clang, '--version'])
        flags = ['-std=c11','-D_POSIX_C_SOURCE=200809L','-O1','-g','-ffunction-sections','-fdata-sections',
                 '-DRCUTILS_REPORT_ERROR_HANDLING_ERRORS=0','-DRCUTILS_WARN_ON_TRUNCATION=0']
        flags += ['-I'+str(root/'include') for root in [*roots.values(),out]]
        common = [args.clang,*flags,*sources,out/'upstream_clock_semantics.c']
        run('compile-native', [*common,'-Wl,--gc-sections','-o',out/'native'])
        baseline = run('native', [out/'native'])
        assert baseline.endswith('checks:215\n')
        run('compile-native-sanitized', [*common,'-fsanitize=address,undefined','-fno-omit-frame-pointer',
                                        '-Wl,--gc-sections','-o',out/'native-sanitized'])
        assert run('native-sanitized', [out/'native-sanitized'], dict(os.environ,ASAN_OPTIONS='detect_leaks=1')) == baseline
        guest = [*common,'--target=wasm32-wasip1','--sysroot='+str(args.sysroot.resolve()),'-nostartfiles',
                 '-fuse-ld='+str(args.wasm_ld),'-Wl,--no-entry','-Wl,--export=_start','-Wl,--max-memory=16777216',
                 '-Dclock_gettime=scene_clock_gettime',out/'upstream_clock_adapter.c','-nodefaultlibs','-lc',args.builtins.resolve()]
        run('compile-wasm', [*guest,'-o',out/'guest.wasm'])
        run('compile-wasm-denied', [*guest,'-DEXPECT_CLOCK_DENIAL','-o',out/'denied.wasm'])
        record['guest_sha256'] = {name: sha(out/name) for name in ('guest.wasm','denied.wasm')}
        imports = {}
        for name in ('guest','denied'):
            run(name+'-disassemble', [args.wasm2wat,'--enable-all',out/(name+'.wasm'),'-o',out/(name+'.wat')])
            imports[name] = re.findall(r'\(import "([^"]+)" "([^"]+)"', (out/(name+'.wat')).read_text())
            assert all(module=='uwvm_ros_v2' and function in ('capability.open','clock.create','clock.read') or
                       module=='wasi_snapshot_preview1' and function in ('fd_write','clock_time_get','fd_close','fd_seek')
                       for module,function in imports[name]), 'Unexpected effect in upstream slice'
        record['imports'] = imports

        def policy(grants=True, read=True, attenuate=False):
            argv = ['--ros-scene-create','upstream','--ros-scene-bind','upstream','main']
            for name in ('capability.open','clock.create','clock.read'):
                argv += ['--ros-scene-interface','upstream',name,'allow']
            if grants:
                for alias,purpose in (('steady','clock.steady'),('system','clock.system')):
                    argv += ['--ros-scene-grant','upstream',alias,purpose,alias,'local',
                             'clock.create,clock.read' if read else 'clock.create']
                    for field in ('max-handles','max-resources'):
                        argv += ['--ros-scene-limit','upstream',alias,field,'1']
                    if attenuate:
                        for key,value in (('delivery','may-reject'),('calls-per-window','1'),('window-ns','60000000000')):
                            argv += ['--ros-scene-limit','upstream',alias,key,value]
            argv += ['--wasip1-single-create','main','--wasip1-single-enable','main',
                     '--wasip1-single-interface','main','fd_write','allow','--wasip1-single-grant','main','stdout.write']
            for name in ('clock_time_get','fd_close','fd_seek'):
                argv += ['--wasip1-single-interface','main',name,'deny']
            return argv

        vm = [args.runtime.resolve(),'--wasm-set-main-module-name','main','--mode','run','--runtime-aot',
              '--runtime-llvm-jit-cache-path','disable','--runtime-compile-threads','1']
        assert run('vm-stable', [*vm,*policy(),'--run',out/'guest.wasm']) == baseline
        expected = 'upstream-physical-clock-denial-preserved\nchecks:10\n'
        for label,configuration in [('zero-grant',policy(grants=False)),('read-denied',policy(read=False)),
                                     ('attenuated',policy(attenuate=True))]:
            assert run('vm-'+label, [*vm,*configuration,'--run',out/'denied.wasm']) == expected
        record['native_wasm_transcript_equal'] = True
        checks_passed = True
    finally:
        finalize(record,out,args.runtime,checks_passed)


if __name__ == '__main__':
    main()
