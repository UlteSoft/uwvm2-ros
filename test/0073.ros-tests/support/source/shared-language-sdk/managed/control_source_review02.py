#!/usr/bin/env python3
"""Actual full CONTROL source successor and optional CLR compile baseline.

No application, native ROS import, Wasm or AOT compiler is executed. The full
canonical source producer and complete C# operator execute under the common
finite owner; every original/control/helper/output SHA remains inspectable.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
from xml.sax.saxutils import escape


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--dotnet', type=Path)
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('shared admitted Linux finite owner required')
    source = args.source.resolve(strict=True)
    sdk = source / 'sdk/origin'
    args.out.mkdir(parents=True, exist_ok=False)
    report = dict(format='uwvm-control-source-successor-v1', status='STARTED', commands=[],
        canonical_records=0, canonical_roots=0, complete_csharp_records=0, source_routes={},
        clr_compile_cases=0, aot_compile_cases=0, guest_cases=0, native_ros_cases=0, default_authority=[])
    env = dict(os.environ, HOME=str(args.out / 'home'), TMPDIR=str(args.out / 'tmp'),
        DOTNET_CLI_HOME=str(args.out / 'home'), NUGET_PACKAGES=str(args.out / 'packages'),
        DOTNET_CLI_TELEMETRY_OPTOUT='1', DOTNET_SKIP_FIRST_TIME_EXPERIENCE='1',
        DOTNET_CLI_WORKLOAD_UPDATE_NOTIFY_DISABLE='true', DOTNET_CLI_USE_MSBUILD_SERVER='0',
        DOTNET_EnableWriteXorExecute='0', DOTNET_EnableDiagnostics='0',
        DOTNET_GCHeapHardLimit='0x20000000', DOTNET_PROCESSOR_COUNT='1',
        DOTNET_SYSTEM_GLOBALIZATION_INVARIANT='1', MSBUILDDISABLENODEREUSE='1')
    for name in ('home', 'tmp', 'commands'):
        (args.out / name).mkdir()

    def save():
        (args.out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(name, argv):
        folder = args.out / 'commands' / name
        folder.mkdir()
        with (folder / 'stdout').open('xb') as stdout, (folder / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, argv)), cwd=args.out, env=env, stdin=subprocess.DEVNULL,
                stdout=stdout, stderr=stderr, check=False)
        report['commands'].append(dict(name=name, argv=list(map(str, argv)), exit=result.returncode,
            stdout_sha256=digest(folder / 'stdout'), stderr_sha256=digest(folder / 'stderr')))
        save()
        if result.returncode:
            raise RuntimeError('failed ' + name + '; retained raw output')

    try:
        canonical = args.out / 'canonical'
        run('canonical', [sys.executable, '-I', '-S', '-B', sdk / 'tools/custom_types.py',
            sdk / 'rmw_wasm/default_control/catalog.preparation.json', '--out', canonical, '--guest-only'])
        for name, operator in [('ordinary', sdk / 'csharp/owned/generate_control.py'),
                               ('aot', sdk / 'dotnet/mono-wasi-aot/generate_control.py')]:
            target = args.out / name
            run('complete-csharp-' + name, [sys.executable, '-I', '-S', '-B', operator,
                '--source-root', source, '--canonical', canonical, '--out', target,
                '--operator-sha256', digest(operator), '--emitter-sha256', digest(operator.with_name('control_csharp_emitter.py')),
                '--explicit-owned-ros2'])
            proof = json.loads((target / 'generation.control-csharp.json').read_bytes())
            if len(proof['complete_record_names']) != 32 or len(proof['complete_record_contracts']) != 32:
                raise ValueError('complete original32 source contracts required')
            if proof['native_registry_emitted'] is not False or proof['implicit_permissions'] != []:
                raise ValueError('source producer cannot grant native identity or authority')
            if len(proof['emitter']['services']) != 7:
                raise ValueError('all seven original complete service roots required')
            report['source_routes'][name] = dict(records=32, service_roots=7,
                manifest_sha256=digest(target / 'generation.control-csharp.json'),
                outputs=proof['outputs'])
            save()
        for name in ('BoundedRecords.cs', 'ControlRoots.cs'):
            if (args.out / 'ordinary' / name).read_bytes() != (args.out / 'aot' / name).read_bytes():
                raise ValueError('ordinary and AOT routes must consume the same whole record bodies')
        report.update(canonical_records=32, canonical_roots=7, complete_csharp_records=32)
        if args.dotnet:
            run('dotnet-version', [args.dotnet, '--version'])
            if (args.out / 'commands/dotnet-version/stdout').read_bytes() != b'8.0.408\n':
                raise ValueError('exact retained SDK8.0.408 required; no roll-forward')
            (args.out / 'global.json').write_text('{"sdk":{"version":"8.0.408","rollForward":"disable","allowPrerelease":false}}\n')
            config = args.out / 'NuGet.Config'
            config.write_text('<configuration><packageSources><clear/></packageSources></configuration>\n')
            files = sorted((sdk / 'csharp/RosWasm').glob('*.cs'))
            files += [sdk / 'csharp/owned' / (name + '.cs') for name in
                ('NativeOwned', 'PreparedSession', 'OwnedWire', 'OwnedGroup', 'ControlWire')]
            files += [args.out / 'aot' / name for name in ('BoundedRecords.cs', 'ControlRoots.cs')]
            body = '<Project Sdk="Microsoft.NET.Sdk"><PropertyGroup><TargetFramework>net8.0</TargetFramework>'
            body += '<EnableDefaultCompileItems>false</EnableDefaultCompileItems><Nullable>enable</Nullable>'
            body += '<DefineConstants>UWVM_ROS2;UWVM_ROSWASM_OWNED_SERVICE</DefineConstants></PropertyGroup><ItemGroup>'
            body += ''.join('<Compile Include="' + escape(str(path), {'"': '&quot;'}) + '"/>' for path in files)
            body += '</ItemGroup></Project>'
            project = args.out / 'CompleteControl.csproj'
            project.write_text(body)
            run('offline-restore', [args.dotnet, 'restore', project, '--configfile', config, '--disable-parallel', '-p:NuGetAudit=false'])
            run('compile-whole-control', [args.dotnet, 'build', project, '--no-restore', '--disable-build-servers',
                '-m:1', '-nr:false', '-p:UseSharedCompilation=false', '-p:NuGetAudit=false'])
            assembly = args.out / 'bin/Debug/net8.0/CompleteControl.dll'
            report.update(clr_compile_cases=1, clr_assembly=dict(bytes=assembly.stat().st_size, sha256=digest(assembly)))
        run('installed-source-kit', [sys.executable, '-I', '-S', '-B', sdk / 'dotnet/mono-wasi-aot/source_kit.py',
            '--source-root', source, '--out', args.out / 'source-kit'])
        kit = json.loads((args.out / 'source-kit/source-kit.json').read_bytes())
        if kit['active_sdk_tests_included'] is not False or any('/tests/' in row['target'] for row in kit['sources']):
            raise ValueError('SDK source kit must exclude active tests')
        report['installed_source_kit'] = dict(files=kit['source_files'], objects=kit['unique_objects'],
            manifest_sha256=digest(args.out / 'source-kit/source-kit.json'))
        report['status'] = 'FULL_CONTROL_SOURCE_SUCCESSOR_PASS_NO_AOT_OR_GUEST_CREDIT'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
