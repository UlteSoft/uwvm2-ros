#!/usr/bin/env python3
"""Serial C language matrix. Execute only through the shared Linux finite owner.

This is a scheduler, never C/Wasm/ROS evidence itself. A skipped toolchain mode
is retained as NOT_SUPPORTED; actual commands and layer results stay separate.
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
import time

MODES = (('c89', 89), ('c90', 90), ('iso9899:199409', 95), ('c99', 99),
         ('c11', 11), ('c17', 17), ('c23', 23), ('c2x', 23), ('c2y', 29))
RUNTIME_FAILURE = None


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--test-sources', type=Path, default=Path(__file__).resolve().parent,
        help='relocatable C fixtures; defaults to this driver directory, never an installed SDK test tree')
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--native-cc', action='append', default=[])
    parser.add_argument('--skip-native', action='store_true')
    parser.add_argument('--wasm-cc')
    parser.add_argument('--wasm-ar')
    parser.add_argument('--wasm-ld', default='/usr/bin/wasm-ld-21')
    parser.add_argument('--wasm-target', default='wasm32-unknown-unknown')
    parser.add_argument('--wasm-libc', type=Path)
    parser.add_argument('--wasm-crt', type=Path,
        help='explicit real hosted command CRT; manual CRT/libc program probe, not a compiler-rt qualification')
    parser.add_argument('--wasm-sysroot', type=Path)
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--validator')
    parser.add_argument('--import-checker', type=Path,
        help='explicit pinned binary inspector; defaults to the selected source tree tool')
    parser.add_argument('--abi-schema', type=Path,
        help='explicit pinned ABI schema for evidence; inspector must resolve this same schema')
    parser.add_argument('--runtime-extra', action='append', default=[])
    parser.add_argument('--mode', choices=[mode for mode, _ in MODES], action='append', default=[],
        help='explicit finite mode subset for lossless serial replay; omission runs every mode')
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        parser.error('admitted shared Linux finite owner required')
    scope = os.environ.get('UWVM_RESOURCE_SCOPE', '')
    if not scope:
        parser.error('owned cgroup identity missing')
    actual = Path('/proc/self/cgroup').read_text().strip()
    if actual != '0::' + scope.split('/sys/fs/cgroup', 1)[-1]:
        parser.error('scheduler escaped the admitted child cgroup')
    sdk, out = args.sdk.resolve(strict=True), args.output.absolute()
    out.mkdir(mode=0o700)
    sources = args.test_sources.resolve(strict=True)
    import_checker = (args.import_checker or sdk/'tools/check_wasm_imports.py').resolve(strict=True)
    abi_schema = (args.abi_schema or sdk/'share/abi/abi-v1.json').resolve(strict=True)
    inspector_root = import_checker.parent.parent
    inspector_abi = inspector_root/'share/abi'
    if hashlib.sha256(abi_schema.read_bytes()).digest() != hashlib.sha256((inspector_abi/'abi-v1.json').read_bytes()).digest():
        parser.error('explicit ABI schema differs from the inspector-selected schema')
    examples = sdk / 'example/platform/ros/c/c'
    rows, commands = [], []
    sequence = 0

    def run(label, command, expected=0, timeout=40):
        nonlocal sequence
        sequence += 1
        name = f'{sequence:04d}-' + ''.join(c if c.isalnum() or c in '._-' else '-' for c in label)
        if args.wasm_target.startswith('wasm64') and validator and str(command[0])==str(validator) and '--version' not in command:
            command=[command[0], '--enable-memory64', *command[1:]]
        started = time.monotonic()
        try:
            result = subprocess.run([str(c) for c in command], stdin=subprocess.DEVNULL,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout, check=False)
            code, stdout, stderr = result.returncode, result.stdout, result.stderr
        except subprocess.TimeoutExpired as error:
            code, stdout, stderr = None, error.stdout or b'', error.stderr or b''
        (out / (name + '.stdout')).write_bytes(stdout)
        (out / (name + '.stderr')).write_bytes(stderr)
        record = dict(label=label, command=[str(c) for c in command], exit_code=code,
            expected_exit=expected, matched=(code == expected), elapsed_seconds=time.monotonic()-started,
            stdout=name+'.stdout', stderr=name+'.stderr', stdout_bytes=len(stdout),
            stdout_sha256=hashlib.sha256(stdout).hexdigest(), stderr_sha256=hashlib.sha256(stderr).hexdigest())
        commands.append(record)
        return record

    def row(layer, compiler, mode, status, **extra):
        rows.append(dict(layer=layer, compiler=str(compiler), mode=mode, status=status, **extra))

    def flags(cc, mode, profile):
        return [cc, '-std='+mode, '-DROSWASM_C_STANDARD='+str(profile), '-pedantic-errors',
            '-Werror=implicit-function-declaration', '-Werror=incompatible-pointer-types', '-O2',
            '-ffunction-sections', '-fdata-sections', '-I', sdk/'include', '-I', examples]

    native = [] if args.skip_native else (args.native_cc or [p for n in ('gcc', 'clang') if (p := shutil.which(n))])
    wasm = args.wasm_cc or shutil.which('clang')
    runtime = args.runtime
    validator = args.validator or shutil.which('wasm-validate')
    tools = dict(native=native, wasm=wasm, wasm_ar=args.wasm_ar, wasm_ld=args.wasm_ld,
        wasm_target=args.wasm_target, wasm_libc=str(args.wasm_libc) if args.wasm_libc else None,
        wasm_crt=str(args.wasm_crt) if args.wasm_crt else None,
        runtime=str(runtime) if runtime else None, validator=validator)
    for cc in list(dict.fromkeys(native + ([wasm] if wasm else []))):
        run('tool-version', [cc, '--version'])
    if runtime:
        run('runtime-version', [runtime, '--version'])
    if validator:
        run('validator-version', [validator, '--version'])
    probe = out/'language-probe.c'
    probe.write_text('int main(void) {return 0;}\n')
    for cc, target in [(cc, 'native') for cc in native] + ([(wasm, 'wasm')] if wasm else []):
        for mode, profile in MODES:
            if args.mode and mode not in args.mode:
                continue
            tag = target+'-'+Path(cc).name+'-'+mode.replace(':', '-')
            base = flags(cc, mode, profile)
            if target == 'wasm':
                base += ['--target='+args.wasm_target, '-ffreestanding', '-nostdlib', '-fuse-ld='+args.wasm_ld]
                if args.wasm_sysroot:
                    base += ['--sysroot='+str(args.wasm_sysroot.resolve(strict=True))]
            supported = run(tag+'-mode', base+['-fsyntax-only', probe])
            if not supported['matched']:
                row('compiler-mode', cc, mode, 'NOT_SUPPORTED', command=supported)
                continue
            macros = run(tag+'-macros', base+['-dM', '-E', '-x', 'c', '-include', sdk/'include/platform/ros/c/roswasm/detail/common/c_portability.h', probe])
            syntax = run(tag+'-headers', base+['-fsyntax-only', sources/'portable_headers.c'])
            row('strict-headers', cc, mode, 'PASS' if syntax['matched'] else 'FAIL', command=syntax, macros=macros)
            if not syntax['matched']:
                continue
            macro_text=(out/macros['stdout']).read_text(errors='replace')
            pointer_match=re.search(r'(?m)^#define __SIZEOF_POINTER__ (\d+)$',macro_text)
            if pointer_match:
                wrong_bits=64 if int(pointer_match[1])==4 else 32
                rejected=run(tag+'-pointer-mismatch',base+['-DROSWASM_C_ADDRESS_BITS='+str(wrong_bits),'-fsyntax-only',sources/'portable_headers.c'],expected=1)
                row('pointer-abi-mismatch',cc,mode,'PASS' if rejected['matched'] else 'FAIL',command=rejected)
            extension = target=='wasm' and args.wasm_target.startswith('wasm32') and profile in (89,90,95)
            iso = run(tag+'-iso-types', base+['-DROSWASM_C_REQUIRE_ISO_TYPES=1', '-fsyntax-only', sources/'portable_headers.c'], expected=1 if extension else 0)
            row('iso-scalar-contract', cc, mode, 'PASS' if iso['matched'] else 'FAIL', command=iso,
                outcome='expected refusal of compiler i64 extension' if extension else 'accepted exact ISO scalar types')
            suffix = '.wasm' if target == 'wasm' else ''
            exe = out/(tag+'-codec'+suffix)
            link = ['-Wl,--entry=_start', '-Wl,--export=_start', '-include', sources/'guest_entry.h'] if target == 'wasm' else ['-Wl,--gc-sections']
            if target == 'wasm' and args.wasm_libc:
                link += [args.wasm_libc]
            compiled = run(tag+'-codec-build', base+link+[sources/'portable_contract.c', '-o', exe])
            row(target+'-codec-compile', cc, mode, 'PASS' if compiled['matched'] else 'FAIL', command=compiled)
            if compiled['matched']:
                if target == 'native':
                    executed = run(tag+'-codec-run', [exe])
                    row('native-codec-run', cc, mode, 'PASS' if executed['matched'] else 'FAIL', command=executed)
                else:
                    validate_and_run(exe, tag+'-codec', mode, cc, validator, runtime, run, row, args.runtime_extra)
            if target == 'wasm':
                hosted=out/(tag+'-hosted.wasm')
                hosted_flags=[flag for flag in base if flag not in ('-ffreestanding','-nostdlib')]
                hosted_libraries=[]
                if args.wasm_crt:
                    if not args.wasm_libc:
                        parser.error('explicit hosted CRT probe also requires the actual matching libc')
                    hosted_flags+=['-nostdlib']
                    hosted_libraries=[args.wasm_crt.resolve(strict=True),args.wasm_libc.resolve(strict=True)]
                built=run(tag+'-hosted-build', hosted_flags+[sources/'hosted_profile.c']+hosted_libraries+['-o',hosted])
                row('wasm-hosted-libc-crt-compile',cc,mode,'PASS' if built['matched'] else 'FAIL',command=built)
                if built['matched']:
                    validate_and_run(hosted,tag+'-hosted',mode,cc,validator,runtime,run,row,args.runtime_extra,hosted=True)
            # Separately compiled objects and actual target archive.
            archive = out/(tag+'-telemetry.a')
            objects = []
            for source in (sdk/'src/platform/ros/c/library_profile.c', examples/'portable_library/encode.c', examples/'portable_library/send.c'):
                obj = out/(tag+'-'+source.stem+'.o')
                built = run(tag+'-object-'+source.stem, base+['-c', source, '-o', obj])
                if built['matched']:
                    objects.append(obj)
            ar = (args.wasm_ar or shutil.which('llvm-ar')) if target == 'wasm' else shutil.which('ar')
            if len(objects) == 3 and ar:
                archived = run(tag+'-archive', [ar, 'rcs', archive]+objects)
                consumer = out/(tag+'-library'+suffix)
                built = run(tag+'-library-build', base+link+[examples/'portable_library/codec_main.c', archive, '-o', consumer])
                row(target+'-library-compile', cc, mode, 'PASS' if archived['matched'] and built['matched'] else 'FAIL', command=built)
                if built['matched']:
                    if target == 'native':
                        executed = run(tag+'-library-run', [consumer])
                        row('native-library-run', cc, mode, 'PASS' if executed['matched'] else 'FAIL', command=executed)
                    else:
                        validate_and_run(consumer, tag+'-library', mode, cc, validator, runtime, run, row, args.runtime_extra)
                alternate = 99 if profile in (89,90) else 90
                mismatched = [x for x in base if not str(x).startswith('-DROSWASM_C_STANDARD=')]+['-DROSWASM_C_STANDARD='+str(alternate)]
                # C90 compiler cannot select C99. A C90 library is consumed by
                # a genuine C99-mode TU for that expected link mismatch.
                if profile in (89, 90, 95):
                    mismatched = [('-std=c99' if str(x).startswith('-std=') else x) for x in mismatched]
                mismatch = run(tag+'-profile-mismatch', mismatched+link+[examples/'portable_library/codec_main.c', archive, '-o', out/(tag+'-mismatch'+suffix)], expected=1)
                stderr = (out/mismatch['stderr']).read_text(errors='replace')
                refused = mismatch['exit_code'] != 0 and mismatch['exit_code'] is not None and 'roswasmc_library_c_profile_' in stderr
                row(target+'-profile-mismatch', cc, mode, 'PASS' if refused else 'FAIL', command=mismatch)
                for dimension in ('EXCEPTIONS','THREADS'):
                    rejected=run(tag+'-'+dimension.lower()+'-library-mismatch',base+link+['-DROSWASM_C_'+dimension+'=1',examples/'portable_library/codec_main.c',archive,'-o',out/(tag+'-'+dimension.lower()+'-mismatch'+suffix)],expected=1)
                    diagnostic=(out/rejected['stderr']).read_text(errors='replace')
                    refused=rejected['exit_code'] is not None and rejected['exit_code']!=0 and 'roswasmc_library_c_profile_' in diagnostic
                    row(target+'-abi-dimension-mismatch',cc,mode,'PASS' if refused else 'FAIL',dimension=dimension,
                        scope='expected link refusal only; no EH/thread runtime qualification',command=rejected)
            else:
                row(target+'-library-compile', cc, mode, 'FAIL', reason='object compile or target archiver missing')
            if target != 'wasm':
                continue
            for backend in (1, 2):
                guest = out/(tag+'-deny-ros'+str(backend)+'.wasm')
                selected = ['-DROSWASM_EXAMPLE_ROS_VERSION='+str(backend)]
                coverage = out/(tag+'-raw-'+str(backend)+'.wasm')
                built = run(tag+'-raw-build-'+str(backend), base+selected+['-Wl,--no-entry', examples/'00_abi_coverage.c', '-o', coverage])
                row('wasm-full-abi-compile', cc, mode, 'PASS' if built['matched'] else 'FAIL', backend=backend, command=built)
                if built['matched']:
                    inspected = run(tag+'-raw-inspect-'+str(backend), [sys.executable, '-I', '-S', '-B', import_checker, coverage, '--backend', str(backend), '--require-all', '--report', out/(tag+'-raw-'+str(backend)+'.json')])
                    row('wasm-full-import-signatures', cc, mode, 'PASS' if inspected['matched'] else 'FAIL', backend=backend,
                        denominator=38 if backend==1 else 43, command=inspected)
                    if validator:
                        validated = run(tag+'-raw-validate-'+str(backend), [validator, coverage])
                        row('wasm-abi-validation', cc, mode, 'PASS' if validated['matched'] else 'FAIL', backend=backend, command=validated)
                built = run(tag+'-deny-build-'+str(backend), base+link+selected+[sources/'default_denial.c', '-o', guest])
                row('wasm-denial-compile', cc, mode, 'PASS' if built['matched'] else 'FAIL', backend=backend, command=built)
                if built['matched']:
                    validate_and_run(guest, tag+'-deny-'+str(backend), mode, cc, validator, runtime, run, row, args.runtime_extra, backend=backend)
                # Actual named preloads with independent linear memories.
                modules = {}
                for stem in ('callback', 'codec', 'client'):
                    module = out/(tag+'-ros'+str(backend)+'-'+stem+'.wasm')
                    built = run(tag+'-preload-'+str(backend)+'-'+stem, base+selected+['-Wl,--no-entry', examples/'preload'/ (stem+'.c'), '-o', module])
                    if built['matched']:
                        modules[stem] = module
                main_guest = out/(tag+'-ros'+str(backend)+'-preload-main.wasm')
                built = run(tag+'-preload-main-'+str(backend), base+link+[examples/'preload/main.c', '-o', main_guest])
                if len(modules) != 3 or not built['matched']:
                    row('wasm-preload-compile', cc, mode, 'FAIL', backend=backend)
                else:
                    row('wasm-preload-compile', cc, mode, 'PASS', backend=backend)
                    extra = ['--wasm-preload-library', modules['callback'], 'callbacks', '--wasm-preload-library', modules['codec'], 'codec', '--wasm-preload-library', modules['client'], 'client']
                    validate_and_run(main_guest, tag+'-preload-'+str(backend), mode, cc, validator, runtime, run, row, args.runtime_extra+extra, backend=backend, preload=True)
                lessons = [(name, {}) for name in ('01_raw_imports', '02_encoded_resource', '03_node_executor',
                    '04_service_action_resources', '05_queries_and_events', '07_service_action_servers',
                    '08_context_future_timer', '09_session_ownership', '10_action_client_bundle', '11_unified_events', '12_async_service')]
                lessons.append(('06_runtime_delivery', {'ROSWASM_EXAMPLE_DEFAULT_DENIED': 1}))
                lessons.append(('06_runtime_delivery', {'ROSWASM_EXAMPLE_DEFAULT_DENIED': 0}))
                if mode == 'c89':
                    for stem, count in (('04_service_action_resources', 7), ('05_queries_and_events', 26), ('07_service_action_servers', 3)):
                        lessons += [(stem, {'ROSWASM_EXAMPLE_CASE': case}) for case in range(1, count)]
                    lessons += [('08_context_future_timer', {'ROSWASM_EXAMPLE_DEFER_RPC': 1}),
                        ('09_session_ownership', {'ROSWASM_EXAMPLE_REVOKE': 1})]
                    lessons += [('10_action_client_bundle', {'ROSWASM_EXAMPLE_CASE': case, 'ROSWASM_EXAMPLE_CANCEL_ANY': any_role, 'ROSWASM_EXAMPLE_REVOKE': revoke})
                        for case in range(5) for any_role in (0, 1) for revoke in (0, 1) if (case, any_role, revoke) != (0, 0, 0)]
                for lesson_index, (stem, definitions) in enumerate(lessons):
                    label=tag+'-lesson-'+str(backend)+'-'+str(lesson_index)+'-'+stem
                    artifact=out/(label+'.wasm')
                    lesson_link=[] if stem=='06_runtime_delivery' else link
                    defined=['-D'+key+'='+str(value) for key, value in definitions.items()]
                    built=run(label+'-build', base+selected+defined+lesson_link+[examples/(stem+'.c'), '-o', artifact])
                    row('wasm-progressive-example-compile', cc, mode, 'PASS' if built['matched'] else 'FAIL', backend=backend, lesson=stem, definitions=definitions, command=built)
                    if built['matched'] and runtime:
                        expected=0 if (stem=='06_runtime_delivery' and definitions.get('ROSWASM_EXAMPLE_DEFAULT_DENIED')) or (stem=='08_context_future_timer' and not definitions.get('ROSWASM_EXAMPLE_DEFER_RPC')) else 13
                        case=definitions.get('ROSWASM_EXAMPLE_CASE', 0)
                        if stem=='05_queries_and_events' and case in ((6,7,8,9,13,19,20,24,25) if backend==1 else (2,3,5,18)):
                            expected=95
                        validate_and_run(artifact, label, mode, cc, validator, runtime, run, row, args.runtime_extra, backend=backend, lesson=stem, definitions=definitions, expected_exit=expected)
    for cc in native:
        if args.mode and 'c11' not in args.mode:
            continue
        base=flags(cc, 'c11', 11)+['-Wl,--gc-sections']
        for name in ('wrapper_contract', 'golden_vectors', 'legacy_length_contract', 'events_contract',
                     'event_dispatch_contract', 'resource_lifetimes', 'scheduler_contract', 'action_bundle_contract'):
            exe=out/(Path(cc).name+'-legacy-'+name)
            compiled=run(Path(cc).name+'-legacy-build-'+name, base+[sources/(name+'.c'), '-o', exe])
            row('native-existing-model-compile', cc, 'c11', 'PASS' if compiled['matched'] else 'FAIL', fixture=name, command=compiled)
            if compiled['matched']:
                executed=run(Path(cc).name+'-legacy-run-'+name, [exe])
                row('native-existing-model-run', cc, 'c11', 'PASS' if executed['matched'] else 'FAIL', fixture=name, command=executed)
    hashes = {(str(p.relative_to(sdk)) if p.is_relative_to(sdk) else 'test-sources/'+str(p.relative_to(sources))): hashlib.sha256(p.read_bytes()).hexdigest()
        for base in (sdk/'include/platform/ros/c/roswasm', sdk/'include/platform/ros1/c/roswasm', sdk/'include/platform/ros2/c/roswasm', sdk/'src/platform/ros/c', sources, examples)
        for p in base.rglob('*') if p.is_file() and p.suffix in ('.h', '.c', '.py', '.lua')}
    hashes['tools/check_wasm_imports.py']=hashlib.sha256(import_checker.read_bytes()).hexdigest()
    hashes['abi/abi-v1.json']=hashlib.sha256(abi_schema.read_bytes()).hexdigest()
    tools.update(import_checker=str(import_checker), abi_schema=str(abi_schema))
    summary = dict(schema='shared-c-standard-matrix-v1', tools=tools, rows=rows, commands=commands,
        source_sha256=hashes, default_authority=[], physical_motion_tests=0,
        artifact_sizes={p.name:dict(bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest())
            for p in out.iterdir() if p.is_file() and p.suffix in ('.wasm','.o','.a')},
        native_ros_runs=0, gateway_integration_runs=0, upstream_official_suite_credit=0,
        qemu_runs=0, vtune_hardware_counter_runs=0, public_profile_resolver_qualification=False,
        status='FAIL' if any(r['status']=='FAIL' for r in rows) else 'PASS_EXECUTED_SCOPE')
    (out/'matrix.json').write_text(json.dumps(summary, indent=2, sort_keys=True)+'\n')
    print(json.dumps(dict(status=summary['status'], rows=len(rows), counts={s:sum(r['status']==s for r in rows) for s in ('PASS','FAIL','NOT_SUPPORTED','NOT_RUN')})), flush=True)
    return int(summary['status']=='FAIL')


def validate_and_run(guest, label, mode, cc, validator, runtime, run, row, extra, **metadata):
    global RUNTIME_FAILURE
    expected_exit=metadata.pop('expected_exit', 0)
    if validator:
        validated = run(label+'-validate', [validator, guest])
        row('wasm-validation', cc, mode, 'PASS' if validated['matched'] else 'FAIL', command=validated, **metadata)
    else:
        row('wasm-validation', cc, mode, 'NOT_RUN', reason='validator unavailable', **metadata)
    if runtime:
        if RUNTIME_FAILURE:
            row('actual-wasm-runtime', cc, mode, 'NOT_RUN', reason='prior Runtime process/launch failure in this target matrix', blocker=RUNTIME_FAILURE, **metadata)
            return
        # No ROS enable/grant, environment, clock, random, filesystem or network
        # permission is added. Main requires only its explicit basic WASIp1 entry.
        command = [runtime, '--runtime-int', '-Rct', '0',
            '--wasm-set-main-module-name', 'main', '--wasip1-global-disable',
            '--wasip1-global-noinherit-system-environment', '--wasip1-single-create', 'main',
            '--wasip1-single-enable', 'main', '--wasip1-single-hide-host-api', 'main',
            '--wasip1-single-noinherit-system-environment', 'main']
        for right in ('realtime-clock', 'monotonic-clock', 'process-clock', 'thread-clock',
                      'random', 'proc-raise', 'sched-yield', 'stdio-full-rights'):
            command += ['--wasip1-single-deny', 'main', right]
        if '--wasm-feature-enable-memory64' in extra:
            names = ['main']
            for index, argument in enumerate(extra):
                if argument == '--wasm-preload-library':
                    names.append(str(extra[index+2]))
            for name in names:
                command += ['--wasm-set-memory-limit', name, 'all', '2', '256']
        command += extra + ['--run', guest]
        executed = run(label+'-actual-runtime', command, expected=expected_exit)
        stdout_matches=not metadata.get('hosted') or executed['stdout_sha256']==hashlib.sha256(b'C stdio profile\n').hexdigest()
        row('actual-wasm-runtime', cc, mode, 'PASS' if executed['matched'] and stdout_matches else 'FAIL', command=executed,
            expected_stdout_matched=stdout_matches, **metadata)
        if not executed['matched'] and executed['exit_code'] is not None and (executed['exit_code']==126 or executed['exit_code']<0):
            RUNTIME_FAILURE=executed
    else:
        row('actual-wasm-runtime', cc, mode, 'NOT_RUN', reason='actual Runtime unavailable', **metadata)


if __name__ == '__main__':
    sys.exit(main())
