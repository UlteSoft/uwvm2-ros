#!/usr/bin/env python3
"""Actual isolated C xmake acceptance; shared admitted Linux owner only.

The installed SDK input must be copied beneath this run's work directory before
entry. No global generation/publication, host substitute or implicit grant.
"""
import argparse
import hashlib
import itertools
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tarfile
import time

STANDARDS = ('89', '90', '95', '99', '11', '17', '23', '29')
RECIPES = ('abi-coverage', 'raw-imports', 'encoded-resource', 'node-executor',
    'service-action-0', 'queries-events-0', 'runtime-denied', 'server-0',
    'context-local', 'session-close', 'action-bundle-0', 'unified-events',
    'async-service', 'portable-library', 'portable-codec-library', 'preload',
    'portable-telemetry', 'service-client-metadata', 'robotics-modular',
    'robotics-modular-chain-denial', 'robotics-benchmark', 'robotics-benchmark-session')


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--toolchain', type=Path, required=True)
    parser.add_argument('--xmake', type=Path, required=True)
    parser.add_argument('--archiver', type=Path, required=True)
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--catalog-mode', choices=('full', 'recipes', 'selected'), default='full')
    parser.add_argument('--standard', choices=STANDARDS, action='append', default=[])
    parser.add_argument('--compact', action='store_true',
        help='losslessly retain each completed example in a hash-verified capsule before retiring its scratch')
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        parser.error('shared admitted Linux finite owner required')
    scope = os.environ.get('UWVM_RESOURCE_SCOPE', '')
    if not scope or Path('/proc/self/cgroup').read_text().strip() != '0::' + scope.split('/sys/fs/cgroup', 1)[-1]:
        parser.error('actual owned child cgroup identity differs')
    run = Path(os.environ['UWVM_RESOURCE_RUN']).resolve(strict=True)
    sdk = args.sdk.resolve(strict=True)
    if not sdk.is_relative_to(run/'work'):
        parser.error('--sdk must be an isolated ordinary SDK copy inside this run/work; xmake writes configuration there')
    output = args.output.absolute()
    if not output.is_relative_to(run/'work') or output.exists():
        parser.error('fresh output beneath this run/work required')
    output.mkdir()
    manifest_path = sdk/'share/wasm-profiles.json'
    manifest = json.loads(manifest_path.read_bytes())
    if manifest.get('format') != 'uwvm-sdk-wasm-profiles-v1':
        parser.error('actual shared profile manifest v1 required')
    for file in (args.xmake, args.archiver, args.toolchain/'bin/clang'):
        if not file.is_file():
            parser.error('explicit actual tool unavailable: '+str(file))
    rows, commands = [], []
    sequence = 0
    source_pins = {str(p.relative_to(sdk)): digest(p)
        for base in (sdk/'example/platform/ros/c', sdk/'tools', sdk/'include/platform/ros/c',
            sdk/'include/platform/ros1/c',sdk/'include/platform/ros2/c',sdk/'include/roswasm',sdk/'lib/platform/ros/c')
        for p in base.rglob('*') if p.is_file() and p.suffix in ('.lua', '.py', '.h', '.c')}
    for relative in ('xmake.lua', 'example/xmake.lua', 'share/wasm-profiles.json'):
        source_pins[relative] = digest(sdk/relative)
    result = dict(format='task02-c-public-xmake-qualification-v1', sdk=str(sdk), toolchain=str(args.toolchain),
        manifest_sha256=digest(manifest_path), source_sha256=source_pins, rows=rows, commands=commands,
        tools={str(p): digest(p) for p in (args.xmake, args.archiver)}, default_authority=[],
        native_ros_runs=0, gateway_runs=0, upstream_official_suite_credit=0, physical_motion_runs=0,
        qemu_runs=0, vtune_hardware_counter_runs=0, complete_profile_product=False)
    result['artifact_capsules'] = []
    result['command_scratch_capsules'] = []

    def checkpoint():
        (output/'qualification.json').write_text(json.dumps(result, indent=2, sort_keys=True)+'\n')

    def compact_completed():
        if not args.compact:
            return
        capsule_root = output/'capsules'
        capsule_root.mkdir(exist_ok=True)
        for directory in sorted(output.glob('w*-eh*-t*-c*-*')):
            if not directory.is_dir():
                continue
            files = {}
            for path in directory.rglob('*'):
                if path.is_symlink():
                    raise RuntimeError('ordinary bounded example outputs required')
                if path.is_file():
                    files[path.relative_to(directory).as_posix()] = digest(path)
            capsule = capsule_root/(directory.name+'.tar.gz')
            with tarfile.open(capsule, 'w:gz') as archive:
                archive.add(directory, arcname='example')
            with tarfile.open(capsule, 'r:gz') as archive:
                archived = {}
                for member in archive:
                    if member.isfile():
                        archived[Path(member.name).relative_to('example').as_posix()] = hashlib.sha256(archive.extractfile(member).read()).hexdigest()
                if archived != files:
                    raise RuntimeError('lossless output capsule verification failed')
            result['artifact_capsules'].append(dict(example_output=directory.name, path=str(capsule),
                sha256=digest(capsule), bytes=capsule.stat().st_size, members_sha256=files,
                verification='ALL_MEMBER_BYTES_MATCH_BEFORE_SCRATCH_RETIREMENT'))
            checkpoint()
            shutil.rmtree(directory)

    def command(label, argv, timeout=180):
        nonlocal sequence
        sequence += 1
        stem = f'{sequence:04d}-'+re.sub('[^A-Za-z0-9._-]', '-', label)
        started = time.monotonic()
        temporary = None
        environment = dict(os.environ)
        if args.compact and str(argv[0]) == str(args.xmake):
            temporary = Path(os.environ['TMPDIR'])/('xmake-command-'+str(sequence))
            temporary.mkdir()
            environment['TMPDIR'] = str(temporary)
        try:
            proc = subprocess.run([str(x) for x in argv], cwd=sdk/'example', stdin=subprocess.DEVNULL,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout, check=False, env=environment)
            code, stdout, stderr = proc.returncode, proc.stdout, proc.stderr
        except subprocess.TimeoutExpired as error:
            code, stdout, stderr = None, error.stdout or b'', error.stderr or b''
        (output/(stem+'.stdout')).write_bytes(stdout)
        (output/(stem+'.stderr')).write_bytes(stderr)
        record = dict(label=label, argv=[str(x) for x in argv], exit_code=code,
            elapsed_seconds=time.monotonic()-started, stdout=stem+'.stdout', stderr=stem+'.stderr',
            stdout_sha256=hashlib.sha256(stdout).hexdigest(), stderr_sha256=hashlib.sha256(stderr).hexdigest())
        commands.append(record)
        if temporary is not None and code is not None and code >= 0:
            files = {}
            for path in temporary.rglob('*'):
                if path.is_symlink():
                    raise RuntimeError('ordinary per-command xmake scratch required')
                if path.is_file():
                    files[path.relative_to(temporary).as_posix()] = digest(path)
            scratch_capsules = output/'command-scratch'
            scratch_capsules.mkdir(exist_ok=True)
            capsule = scratch_capsules/(stem+'.tar.gz')
            with tarfile.open(capsule, 'w:gz') as archive:
                archive.add(temporary, arcname='scratch')
            with tarfile.open(capsule) as archive:
                restored = {Path(member.name).relative_to('scratch').as_posix():
                    hashlib.sha256(archive.extractfile(member).read()).hexdigest()
                    for member in archive if member.isfile()}
            if restored != files:
                raise RuntimeError('per-command xmake scratch capture changed bytes')
            result['command_scratch_capsules'].append(dict(command=stem, path=str(capsule),
                sha256=digest(capsule), bytes=capsule.stat().st_size, members_sha256=files,
                verification='ALL_MEMBER_BYTES_MATCH_BEFORE_OWN_TEMP_RETIREMENT'))
            shutil.rmtree(temporary)
        checkpoint()
        return record, stdout

    def configure(profile, standard, example):
        return command('configure-'+standard+'-'+example, [args.xmake, 'f', '-c', '-y',
            '--ros-sdk-platforms=ros,ros1,ros2', '--ros-sdk-languages=c',
            '--sdk-example-platform=ros', '--sdk-example-language=c', '--sdk-example='+example,
            '--ros-example-c-backend=2', '--ros-sdk-c-standard='+standard,
            '--sdk-target='+profile['target'], '--sdk-startup=command',
            '--sdk-exceptions='+('y' if profile['exceptions'] else 'n'),
            '--sdk-threads='+('y' if profile['threads'] else 'n'), '--sdk-offline=y',
            '--sdk-toolchain='+str(args.toolchain.resolve(strict=True)),
            '--sdk-profile-manifest='+str(manifest_path), '--ros-example-c-archiver='+str(args.archiver),
            '--ros-example-c-archiver-sha256='+digest(args.archiver),
            '--ros-example-c-runtime-memory64='+('y' if profile['address_bits']==64 else 'n')])[0]

    profiles = {}
    for bits, exceptions, threads, startup in itertools.product((32,64), (False,True), (False,True), ('command','reactor')):
        selected = dict(target=f'wasm{bits}-wasip1', address_bits=bits, wasi='wasip1', exceptions=exceptions,
            threads=threads, startup=startup, core_baseline='3.0')
        candidates = [p for p in manifest['profiles'] if p['profile']==selected and 'c' in p['languages']]
        key = (bits,exceptions,threads,startup)
        if len(candidates)!=1 or not candidates[0]['languages']['c'].get('source_ready'):
            rows.append(dict(layer='actual-profile-availability', profile=selected, status='NOT_SUPPORTED',
                reason='no unique actual source-ready C library/toolchain closure; no fallback'))
        else:
            profiles[key] = candidates[0]
            rows.append(dict(layer='actual-profile-availability', profile=selected, status='AVAILABLE_NOT_QUALIFIED',
                profile_id=candidates[0]['id'], provider=candidates[0]['languages']['c']))
    checkpoint()
    runtime_refusal = False
    catalog_ids = []
    spec = importlib.util.spec_from_file_location('installed_wasm_inspector', sdk/'tools/check_wasm_imports.py')
    inspector = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(inspector)
    core_schema = json.loads((sdk/'share/abi/abi-v1.json').read_bytes())
    expected_core = {(module, item['name']): ([p['type'] for p in item['params']], item['results'])
        for module, entry in core_schema['modules'].items() for item in entry['functions']}
    recipes_built = set()
    for label, tool in (('compiler', args.toolchain/'bin/clang'), ('archiver', args.archiver),
                        ('xmake', args.xmake), ('cmake', Path('/usr/bin/cmake'))):
        command('version-'+label, [tool, '--version'])

    def denial_guest(example, directory):
        report = json.loads((directory/'c-build/build.json').read_bytes())
        argv = [args.runtime, '--runtime-int', '-Rct', '0', '--wasm-set-main-module-name', 'main',
            '--wasip1-global-disable', '--wasip1-global-noinherit-system-environment',
            '--wasip1-single-create', 'main', '--wasip1-single-enable', 'main',
            '--wasip1-single-hide-host-api', 'main', '--wasip1-single-noinherit-system-environment', 'main']
        for right in ('realtime-clock', 'monotonic-clock', 'process-clock', 'thread-clock', 'random',
                      'proc-raise', 'sched-yield', 'stdio-full-rights'):
            argv += ['--wasip1-single-deny', 'main', right]
        for preload in report.get('preloads', []):
            argv += ['--wasm-preload-library', preload['path'], preload['name']]
        expected = 95 if example in ('queries-events-2', 'queries-events-3',
                                     'queries-events-5', 'queries-events-18') else 13
        executed, _ = command('empty-authority-'+directory.name, argv+['--run', directory/'c-build/main.wasm'])
        return dict(layer='actual-empty-authority-denial', example=example,
                    status='PASS' if executed['exit_code']==expected else 'FAIL',
                    expected_exit=expected, command=executed, positive_native_functionality=False)
    for key, row in profiles.items():
        if key[3]!='command':
            continue
        profile = row['profile']
        for standard in args.standard or STANDARDS:
            # All source modes exercise actual hosted command/libc startup and
            # the separately compiled profile-sensitive library. C11 additionally
            # builds the whole finite catalog, including retained C11 adapters.
            selected = ['runtime-denied', 'portable-codec-library', 'preload']
            configured = configure(profile, standard, selected[0])
            if configured['exit_code']!=0:
                rows.append(dict(layer='public-xmake-configure', profile=profile, standard=standard,
                    status='FAIL', command=configured))
                continue
            if not catalog_ids:
                listed, text = command('list-c-examples', [args.xmake, 'sdk-example-list'])
                catalog_ids = re.findall(r'^([a-z][a-z0-9_.-]*)  .+$', text.decode(errors='replace'), re.M)
                rows.append(dict(layer='public-xmake-catalog', status='PASS' if listed['exit_code']==0 and len(catalog_ids)==77 and len(set(catalog_ids))==77 else 'FAIL',
                    expected_entries=77, actual_entries=len(catalog_ids), command=listed))
            if standard=='11' and args.catalog_mode=='full':
                selected = catalog_ids
            elif standard=='11' and args.catalog_mode=='recipes':
                selected = RECIPES
            for example in selected:
                compact_completed()
                if example=='preload' and (*key[:3], 'reactor') not in profiles:
                    rows.append(dict(layer='public-xmake-example', profile=profile, standard=standard, example=example,
                        status='NOT_SUPPORTED', reason='composition requires an actual reactor closure'))
                    continue
                if key[0]==64 and example.startswith('robotics-'):
                    rows.append(dict(layer='public-xmake-example', profile=profile, standard=standard, example=example,
                        status='NOT_SUPPORTED', reason='retained modular source explicitly requires wasm32'))
                    continue
                configured = configure(profile, standard, example)
                if configured['exit_code']!=0:
                    rows.append(dict(layer='public-xmake-configure', profile=profile, standard=standard, example=example,
                        status='FAIL', command=configured))
                    continue
                name = f'w{key[0]}-eh{int(key[1])}-t{int(key[2])}-c{standard}-{example}'
                built, _ = command('build-'+name, [args.xmake, 'sdk-example-build', '--output='+str(output/name)], timeout=300)
                receipt_path = output/name/'build-receipt.json'
                passed = built['exit_code']==0 and receipt_path.is_file()
                rows.append(dict(layer='public-xmake-example', profile=profile, standard=standard, example=example,
                    status='PASS' if passed else 'FAIL', command=built, receipt=str(receipt_path)))
                if not passed:
                    continue
                receipt = json.loads(receipt_path.read_bytes())
                report = json.loads((output/name/'c-build/build.json').read_bytes())
                recipes_built.add(report['example'])
                for artifact in receipt['artifacts']:
                    binary = Path(artifact['path'])
                    data = binary.read_bytes()
                    imports = inspector.inspect(data)
                    core_matches = all((item['params'], item['results']) == expected_core[(item['module'], item['name'])]
                        for item in imports if (item['module'], item['name']) in expected_core)
                    core_matches = core_matches and all((item['module'], item['name']) in expected_core
                        for item in imports if item['module'] in core_schema['modules'])
                    complete = None
                    if example == 'abi-coverage' and binary.suffix == '.wasm':
                        complete = inspector.check(data, '2', ('wasi_snapshot_preview1',), require_all=True)
                    rows.append(dict(layer='actual-imports', example=example, artifact=str(binary),
                        sha256=digest(binary), imports=imports, complete_core_coverage=complete,
                        scope='Actual inventory and core ROSv1 signatures; optional namespaces are recorded separately',
                        status='PASS' if core_matches else 'FAIL'))
                    if binary.suffix == '.wasm':
                        validated, _ = command('validate-'+name+'-'+binary.name,
                            ['/usr/bin/wasm-validate']+(['--enable-memory64'] if key[0]==64 else [])+[binary])
                        rows.append(dict(layer='actual-wasm-validation', example=example, artifact=str(binary),
                            status='PASS' if validated['exit_code']==0 else 'FAIL', command=validated))
                if receipt['execution']!='guest-empty-authority':
                    if receipt['execution']=='inspect-only':
                        rows.append(dict(layer='actual-guest', profile=profile, standard=standard, example=example,
                            status='NOT_RUN', reason='ABI retention probe is inspection only'))
                    elif args.runtime and not runtime_refusal:
                        checked = denial_guest(example, output/name)
                        checked.update(profile=profile, standard=standard)
                        rows.append(checked)
                        code = checked['command']['exit_code']
                        runtime_refusal = code is None or code < 0 or code==126
                    else:
                        rows.append(dict(layer='actual-guest', profile=profile, standard=standard, example=example,
                            status='NOT_RUN', reason='actual Runtime unavailable or retained startup refusal'))
                    continue
                if not args.runtime or runtime_refusal:
                    rows.append(dict(layer='actual-guest', profile=profile, standard=standard, example=example,
                        status='NOT_RUN', reason='actual Runtime unavailable or prior retained startup refusal'))
                    continue
                executed, _ = command('guest-'+name, [args.xmake, 'sdk-example-run', '--receipt='+str(receipt_path),
                    '--runtime='+str(args.runtime)], timeout=90)
                guest_record = output/name/'guest-run.json'
                actual = json.loads(guest_record.read_bytes()) if guest_record.is_file() else {}
                rows.append(dict(layer='actual-guest', profile=profile, standard=standard, example=example,
                    status='PASS' if executed['exit_code']==0 and actual.get('status')=='PASS_GUEST_SCOPE' else 'FAIL',
                    command=executed, guest_record=actual))
                code = actual.get('exit_code')
                runtime_refusal = code is None or code < 0 or code==126
    result['catalog_ids'] = catalog_ids
    compact_completed()
    result['actual_recipes_built'] = sorted(recipes_built)
    result['actual_recipe_denominator'] = 22
    result['profile_availability_denominator'] = 16
    standards = args.standard or STANDARDS
    expected_builds = sum((77 if args.catalog_mode=='full' else 22)
        if standard=='11' and args.catalog_mode in ('full','recipes') else 3 for standard in standards)
    actual_builds = sum(row['layer']=='public-xmake-example' and row['status']=='PASS'
        and row['profile']['address_bits']==32 and not row['profile']['exceptions']
        and not row['profile']['threads'] for row in rows)
    expected_recipes = 22 if '11' in standards and args.catalog_mode in ('full','recipes') else 3
    denominator_ok = (32,False,False,'command') in profiles and actual_builds==expected_builds and len(recipes_built)==expected_recipes
    rows.append(dict(layer='requested-default32-build-denominator', status='PASS' if denominator_ok else 'FAIL',
        expected_builds=expected_builds, actual_builds=actual_builds,
        expected_unique_recipes=expected_recipes, actual_unique_recipes=len(recipes_built),
        source='Actual successful build receipts; unavailable metadata never counts as compilation'))
    result['counts'] = {status:sum(row['status']==status for row in rows)
        for status in ('PASS','FAIL','NOT_SUPPORTED','NOT_RUN','AVAILABLE_NOT_QUALIFIED')}
    result['status'] = 'FAIL' if result['counts']['FAIL'] else 'PASS_EXECUTED_SCOPE_WITH_REMAINING_PROFILE_GAPS'
    result['eh_thread_runtime_qualification'] = 'C source/library compatibility only; C++ unwinding and real thread-spawn require independent actual feature probes'
    checkpoint()
    print(json.dumps({key:result[key] for key in ('status','counts','profile_availability_denominator')}), flush=True)
    return int(bool(result['counts']['FAIL']))


if __name__=='__main__':
    sys.exit(main())
