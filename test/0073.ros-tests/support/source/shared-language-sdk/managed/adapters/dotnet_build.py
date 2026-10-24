#!/usr/bin/env python3
"""Consume .NET 8.0.408 and compile the actual complete F#/VB.NET adapters.

Optional acquisition is explicit and uses Microsoft's exact release metadata
and SHA512. This consumer recipe never edits a compiler/runtime or installed
target. It does not run the Mono bridge, claim AOT or connect to native ROS.
All invocations, including acquisition, belong to the shared Linux owner.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tarfile
import urllib.request
import xml.etree.ElementTree as ET

VERSION = '8.0.408'
METADATA = 'https://builds.dotnet.microsoft.com/dotnet/release-metadata/8.0/releases.json'


def fact(path):
    with path.open('rb') as stream:
        return {'path': str(path), 'bytes': path.stat().st_size,
                'sha256': hashlib.file_digest(stream, 'sha256').hexdigest()}


def acquire(out):
    with urllib.request.urlopen(METADATA, timeout=30) as response:
        raw = response.read(8 * 1024 * 1024 + 1)
    if len(raw) > 8 * 1024 * 1024:
        raise ValueError('release metadata bound')
    (out / 'microsoft-release-metadata.json').write_bytes(raw)
    sdks = [sdk for release in json.loads(raw)['releases'] for sdk in release.get('sdks', []) if sdk['version'] == VERSION]
    if len(sdks) != 1:
        raise ValueError('exact SDK release required')
    files = [row for row in sdks[0]['files'] if row['rid'] == 'linux-x64' and row['name'].endswith('.tar.gz')]
    if len(files) != 1 or not files[0]['url'].startswith('https://builds.dotnet.microsoft.com/dotnet/Sdk/' + VERSION + '/'):
        raise ValueError('exact official Linux SDK archive required')
    row = files[0]
    path = out / 'dotnet-sdk.tar.gz'
    digest = hashlib.sha512()
    size = 0
    with urllib.request.urlopen(row['url'], timeout=30) as source, path.open('xb') as destination:
        while chunk := source.read(1024 * 1024):
            size += len(chunk)
            if size > 384 * 1024 * 1024:
                raise ValueError('SDK download byte bound')
            destination.write(chunk); digest.update(chunk)
    if digest.hexdigest().lower() != row['hash'].lower():
        raise ValueError('official SDK archive SHA512 differs')
    root = out / 'dotnet'
    root.mkdir()
    with tarfile.open(path) as archive:
        if sum(max(0, m.size) for m in archive.getmembers()) > 1024 * 1024 * 1024:
            raise ValueError('SDK extraction byte bound')
        archive.extractall(root, filter='data')
    receipt = {'kind': 'official-dotnet-sdk-consumer-input-v1', 'version': VERSION,
               'metadata_url': METADATA, 'metadata': fact(out / 'microsoft-release-metadata.json'),
               'archive': fact(path), 'sha512': digest.hexdigest(), 'official_row': row,
               'executable': fact(root / 'dotnet')}
    (out / 'acquisition.json').write_text(json.dumps(receipt, indent=2) + '\n')
    return root


def project(path, sources, references=(), language='csharp', executable=False):
    root = ET.Element('Project', Sdk='Microsoft.NET.Sdk')
    properties = ET.SubElement(root, 'PropertyGroup')
    values = {'TargetFramework': 'net8.0', 'EnableDefaultCompileItems': 'false',
              'Nullable': 'enable' if language == 'csharp' else 'disable', 'RootNamespace': '',
              'OutputType': 'Exe' if executable else 'Library', 'DefineConstants': 'UWVM_ROS2',
              'NuGetAudit': 'false', 'UseSharedCompilation': 'false', 'Deterministic': 'true'}
    if language == 'vbnet':
        values['OptionStrict'] = 'On'
    for key, value in values.items():
        ET.SubElement(properties, key).text = value
    items = ET.SubElement(root, 'ItemGroup')
    for source in sources:
        ET.SubElement(items, 'Compile', Include=str(source))
    for reference in references:
        ET.SubElement(items, 'ProjectReference', Include=str(reference))
    ET.ElementTree(root).write(path, encoding='utf-8', xml_declaration=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--generated', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--trace-startup', action='store_true')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--sdk', type=Path)
    group.add_argument('--download-official-sdk', action='store_true')
    args = parser.parse_args()
    if os.uname().sysname != 'Linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('shared admitted Linux owner required')
    args.out.mkdir(exist_ok=False)
    sdk = args.sdk.resolve(strict=True) if args.sdk else acquire(args.out)
    source = args.source.resolve(strict=True) / 'sdk/origin'
    generated = args.generated.resolve(strict=True)
    work = args.out / 'projects'; work.mkdir()
    (work / 'global.json').write_text(json.dumps({'sdk': {'version': VERSION, 'rollForward': 'disable'}}))
    config = ET.Element('configuration'); feeds = ET.SubElement(config, 'packageSources')
    ET.SubElement(feeds, 'clear')
    ET.SubElement(feeds, 'add', key='pinned-sdk-fsharp', value=str(sdk / 'sdk' / VERSION / 'FSharp/library-packs'))
    ET.ElementTree(config).write(work / 'NuGet.Config', encoding='utf-8')
    for name in ('canonical', 'fsharp', 'vbnet', 'contract'):
        (work / name).mkdir()
    canonical = work / 'canonical/Canonical.csproj'
    project(canonical, sorted((source / 'csharp/RosWasm').glob('*.cs')) + [generated / 'csharp/BoundedRecords.cs'])
    fsharp = work / 'fsharp/FSharp.fsproj'; vbnet = work / 'vbnet/VisualBasic.vbproj'
    project(fsharp, [generated / 'fsharp/BoundedRecords.fs'], [canonical], 'fsharp')
    project(vbnet, [generated / 'vbnet/BoundedRecords.vb'], [canonical], 'vbnet')
    contract = work / 'contract/Contract.csproj'
    project(contract, [source / 'language/managed/test/DotnetContract.cs'], [canonical, fsharp, vbnet], executable=True)
    env = dict(os.environ, DOTNET_ROOT=str(sdk), DOTNET_CLI_TELEMETRY_OPTOUT='1',
        DOTNET_SKIP_FIRST_TIME_EXPERIENCE='1', MSBUILDDISABLENODEREUSE='1',
        DOTNET_CLI_WORKLOAD_UPDATE_NOTIFY_DISABLE='1', DOTNET_MULTILEVEL_LOOKUP='0',
        DOTNET_CLI_HOME=str(args.out / 'cli-home'), NUGET_PACKAGES=str(args.out / 'packages'),
        DOTNET_GCHeapHardLimit='0x20000000', DOTNET_PROCESSOR_COUNT='1', DOTNET_EnableDiagnostics='0',
        DOTNET_SYSTEM_GLOBALIZATION_INVARIANT='1', DOTNET_EnableWriteXorExecute='0')
    # The actual bounded startup trace retained a 2 TiB ftruncate on CLR's
    # doublemapper memfd, rejected by finite RLIMIT_FSIZE with SIGXFSZ. This
    # host-tool allocator setting avoids that backing file. Resource limits
    # and Guest/Wasm permissions/features remain unchanged.
    commands = []
    def run(label, argv):
        with (args.out / (label + '.stdout')).open('xb') as stdout, (args.out / (label + '.stderr')).open('xb') as stderr:
            done = subprocess.run([str(x) for x in argv], cwd=work, env=env, stdout=stdout, stderr=stderr, check=False)
        commands.append({'label': label, 'argv': list(map(str, argv)), 'exit': done.returncode,
                         'stdout': fact(args.out / (label + '.stdout')), 'stderr': fact(args.out / (label + '.stderr'))})
        (args.out / 'commands.json').write_text(json.dumps(commands, indent=2) + '\n')
        if done.returncode:
            raise RuntimeError('failed ' + label + '; retain exact compiler diagnostics')
    version_command = [sdk / 'dotnet', '--version']
    if args.trace_startup:
        version_command = ['/usr/bin/strace', '-f', '-e', 'trace=ftruncate,memfd_create,mmap,mprotect,pwrite64,write,openat',
                           '-o', args.out / 'sdk-startup.strace', *version_command]
    run('sdk-version', version_command)
    if (args.out / 'sdk-version.stdout').read_text().strip() != VERSION:
        raise ValueError('exact SDK version changed')
    run('build', [sdk / 'dotnet', 'build', contract, '-c', 'Release', '-m:1', '-nr:false',
                  '-p:UseSharedCompilation=false', '--configfile', work / 'NuGet.Config'])
    run('clr-value-contract', [sdk / 'dotnet', work / 'contract/bin/Release/net8.0/Contract.dll'])
    report = {'kind': 'real-dotnet-managed-adapter-build-v1', 'sdk': VERSION,
              'status': 'HOST_CLR_VALUE_CONTRACT_PASSED', 'commands': commands,
              'generated_source_receipt': fact(generated / 'generation.adapters.json'),
              'sdk_consumer_sources': [fact(p) for p in sorted((source / 'csharp/RosWasm').glob('*.cs'))],
              'contract_source': fact(source / 'language/managed/test/DotnetContract.cs'),
              'wasm': 'NOT_RUN', 'native_ros': 'NOT_RUN', 'gateway': 'NOT_RUN', 'default_authority': []}
    (args.out / 'result.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'])


if __name__ == '__main__':
    main()
