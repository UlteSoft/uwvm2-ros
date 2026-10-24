#!/usr/bin/env python3
"""@brief Qualify a real privately assembled, installed and moved Nim SDK example.

@details The shared producer and CMake installer consume captured source bytes.
With --public-current no source, producer or resolver overlay is applied and no
Nim source is regenerated. The legacy historical mode uses an exact-pinned
Task03 integration candidate and retains that distinct scope in its report.
The official WASIp1 C headers/libraries and genuine external compiler closure
are pinned as real files. No placeholder SDK, sysroot, manifest or Host function
is supplied. Run solely inside the installed finite Linux owner/common lock.
@see review02_acceptance.py; native_languages/XMAKE-EXAMPLES.md
"""

from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tarfile


def digest(path):
    """@brief Return the complete SHA-256 of one readable ordinary input file.

    @param[in] path Regular source, tool or product file; no contents are executed.
    @return Lowercase complete digest; I/O errors propagate.
    @see main
    """
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    """@brief Build the captured Nim example from a relocated CMake installation.

    @details A genuine selected-language assembly is produced with the real
    shared producer, installed with CMake, byte-compared and moved to a new path.
    Xmake must configure and produce a real Wasm command for both ROS backends.
    Independent validation/import checks and actual empty-authority Guests are
    separate receipt rows. These lessons give no official vendor/native credit.
    @return Zero only after every requested build/validation/Guest row succeeds.
    @failure Fail closed on drift, unsupported tool closure, resource refusal,
    missing output, wrong imports, non-idempotent formatting or failed execution.
    @thread All work is serialized; the enclosing owner supplies finite limits.
    @see digest; review03_sources.py
    """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--source-sha256', required=True)
    parser.add_argument('--public-current', action='store_true',
                        help='Use exact captured current public producer/resolver and Nim sources without overlays or regeneration')
    args = parser.parse_args()
    root = Path.cwd()
    if digest(args.source) != args.source_sha256:
        raise ValueError('Captured assembly source changed')
    source = root / 'source'
    source.mkdir()
    with tarfile.open(args.source) as archive:
        members = archive.getmembers()
        if sum(item.size for item in members) > 6 * 1024 * 1024:
            raise ValueError('Finite selected assembly source bound exceeded')
        if any(Path(item.name).is_absolute() or '..' in Path(item.name).parts
               or not (item.isfile() or item.isdir()) for item in members):
            raise ValueError('Ordinary confined selected source files required')
        archive.extractall(source, filter='data')
    sdk_source = source / 'sdk'
    origin = sdk_source / 'origin'
    helper = origin / 'tools/native_languages'
    handoff = origin / 'coordination/20261021-director/handoffs/03/review03'
    before = {str(path.relative_to(source)): digest(path)
              for path in source.rglob('*') if path.is_file()}
    rows = []
    report = {'kind': 'task03-review03-installed-nim-v1', 'status': 'RUNNING',
              'source_sha256': args.source_sha256, 'rows': rows,
              'uses_private_shared_candidate': not args.public_current, 'global_publication': False,
              'default_authority': [], 'Guest_passes': 0, 'native_ROS_runs': 0,
              'Gateway_runs': 0, 'official_upstream_credit': 0,
              'public_API_documentation_accepted': False}
    syntax_pins = {}
    for path in sorted(helper.glob('*.py')):
        compile(path.read_bytes(), str(path), 'exec', dont_inherit=True)
        syntax_pins[str(path.relative_to(source))] = digest(path)
    report['captured_helper_python_syntax'] = {
        'files': syntax_pins, 'scope': 'actual Python parsing only; no native-language or helper execution credit'}

    def save():
        (root / 'installed-nim.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(name, layer, command, cwd=root, marker=None, timeout=180):
        log = root / (name + '.log')
        row = {'name': name, 'layer': layer, 'argv': [str(item) for item in command],
               'cwd': str(cwd), 'log': log.name}
        rows.append(row)
        save()
        try:
            with log.open('wb') as output:
                result = subprocess.run(row['argv'], cwd=cwd, stdout=output,
                                        stderr=subprocess.STDOUT, timeout=timeout)
            body = log.read_text(errors='replace')
            row.update(exit=result.returncode, log_sha256=digest(log),
                       status='PASS' if result.returncode == 0 and (marker is None or marker in body) else 'FAIL')
        except Exception as error:
            row.update(status='FAIL', error=str(error))
        if row['status'] != 'PASS':
            report['status'] = 'FAILED_RETAINED'
        save()
        print(name, row['status'], flush=True)
        if row['status'] != 'PASS':
            raise ValueError('Retained failure: ' + name)
        return body

    for path in sorted(sdk_source.rglob('*.py')):
        compile(path.read_bytes(), str(path), 'exec', dont_inherit=True)
    original = origin / 'tools/wasm_profiles.py'
    if args.public_current:
        report['public_source_inputs'] = before
        report['shared_resolver_sha256'] = digest(original)
        report['nim_source_regeneration'] = False
    else:
        candidate = handoff / 'candidates/wasm_profiles.py'
        if original.read_bytes() != (handoff / 'before/wasm_profiles.py').read_bytes():
            raise ValueError('Shared resolver candidate preimage differs')
        report['shared_candidate'] = {'before_sha256': digest(original),
                                      'candidate_sha256': digest(candidate)}
        shutil.copy2(candidate, original)
        sys.path.insert(0, str(helper))
        import numeric
        products = numeric.products((origin / 'abi/abi-v1.json').read_bytes())
        for relative, body in products.items():
            if relative.startswith('nim/'):
                path = origin / relative
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(body)
    agents = Path('/home/macromodel/Documents/uwvm-test-261018/agents')
    nim_root = agents / '03/20261021-additional-tools-r2/work/toolchains/nim/nim-2.2.4'
    nim = nim_root / 'bin/nim'
    nimpretty = nim_root / 'bin/nimpretty'
    wasi = agents / '10/staged-wasi-r1/staged/wasi-sdk'
    cc = wasi / 'bin/clang'
    linker = wasi / 'bin/wasm-ld'
    report['Nim_version'] = run('nim-version', 'actual_tool_version', [nim, '--version']).strip()
    if 'Version 2.2.4' not in report['Nim_version']:
        raise ValueError('Matching Nim2.2.4 required')
    report['nimpretty_version'] = run('nimpretty-version', 'actual_formatter_version', [nimpretty, '--version']).strip()
    nim_files = sorted((origin / 'nim').rglob('*.nim'))
    command = [nimpretty, '--indent:2', '--maxLineLen:100', *nim_files]
    run('nim-format-first', 'syntax_aware_formatting', command)
    first = {str(path): digest(path) for path in nim_files}
    run('nim-format-second', 'syntax_aware_formatting', command)
    if any(digest(Path(path)) != value for path, value in first.items()):
        raise ValueError('Nim formatter second pass differs')
    if args.public_current and any(
            before[str(path.relative_to(source))] != digest(path) for path in nim_files):
        raise ValueError('Current public Nim bytes require formatting; no newer source may borrow this receipt')
    report['formatter'] = {'sha256': digest(nimpretty), 'inputs': first,
                           'idempotent': True, 'configuration': {'indent': 2, 'maxLineLen': 100}}
    changes = []
    with tarfile.open(root / 'nim-candidates.tar', 'w') as archive:
        for path in sorted((origin / 'nim').rglob('*')):
            if not path.is_file():
                continue
            relative = str(path.relative_to(source))
            if before.get(relative) != digest(path):
                changes.append({'path': relative, 'before_sha256': before.get(relative),
                                'after_sha256': digest(path), 'bytes': path.stat().st_size})
                archive.add(path, arcname=relative, recursive=False)
    report.update(changes=changes, candidate_archive_sha256=digest(root / 'nim-candidates.tar'))
    run('nim-public-sdk-check', 'actual_Nim_semantic_check', [nim, 'check', '--mm:arc', '--threads:off',
        '--exceptions:goto', '--cpu:wasm32', '--os:any', '-d:wasm', '-d:useMalloc',
        '--path:' + str(origin / 'nim/src'), origin / 'nim/src/roswasm/typed.nim'])
    staged = root / 'prepared-sdk'
    run('actual-shared-producer', 'real_selected_SDK_assembly', [sys.executable, '-I', '-S', '-B',
        sdk_source / 'tools/prepare_distribution.py', '--origin', origin, '--out', staged,
        '--platform', 'ros', '--language', 'nim', '--wasi-sysroot', wasi / 'share/wasi-sysroot'])
    report['distribution_manifest_sha256'] = digest(staged / 'share/distribution-manifest.json')
    pins = {}

    def pin_tool(path):
        relative = path.relative_to(agents).as_posix()
        if not path.resolve(strict=True).is_relative_to(agents) or not path.is_file():
            raise ValueError('Compiler closure escapes explicit tool root')
        pins[relative] = {'path': relative, 'bytes': path.stat().st_size, 'sha256': digest(path)}
        return pins[relative]

    compiler_pin = pin_tool(nim)
    linker_pin = pin_tool(linker)
    pin_tool(cc)
    for directory in (nim_root / 'lib', nim_root / 'config'):
        for path in sorted(directory.rglob('*')):
            if path.is_file():
                pin_tool(path)
    resources = sorted((wasi / 'lib/clang').glob('*/include'))
    if len(resources) != 1:
        raise ValueError('One genuine matching Clang resource directory required')
    for path in sorted(resources[0].rglob('*')):
        if path.is_file():
            pin_tool(path)
    builtin = Path(run('clang-target-builtins', 'actual_target_resource_query',
        [cc, '--target=wasm32-wasip1', '-print-libgcc-file-name']).strip())
    if not builtin.resolve(strict=True).is_relative_to(wasi / 'lib/clang'):
        raise ValueError('Target compiler-rt archive escapes the matching compiler')
    pin_tool(builtin)
    pending = [nim, cc, linker]
    inspected = set()
    system_dependencies = set()
    while pending:
        path = pending.pop()
        if path.resolve(strict=True) in inspected:
            continue
        inspected.add(path.resolve(strict=True))
        text = run('elf-' + str(len(inspected)), 'real_external_tool_dependency_query', ['/usr/bin/readelf', '-d', path])
        for name in re.findall(r'\(NEEDED\).*\[([^\]]+)\]', text):
            selected = wasi / 'lib' / name
            if selected.is_file():
                pin_tool(selected)
                pending.append(selected)
            else:
                system_dependencies.add(name)
    dependencies = [value for name, value in sorted(pins.items())
                    if name not in (compiler_pin['path'], linker_pin['path'])]
    if len(dependencies) > 1024:
        raise ValueError('Actual compiler closure exceeds the shared finite profile limit')
    files = [{'path': str(path.relative_to(staged)), 'bytes': path.stat().st_size, 'sha256': digest(path)}
             for directory in (staged / 'include', staged / 'lib')
             for path in sorted(directory.rglob('*')) if path.is_file()]
    contract = {'target': 'wasm32-wasip1', 'address_bits': 32, 'wasi': 'wasip1',
                'exceptions': False, 'threads': False, 'startup': 'command', 'core_baseline': '3.0'}
    manifest = {'format': 'uwvm-sdk-wasm-profiles-v1', 'defaults': contract, 'default_authority': [],
        'profiles': [{'id': 'task03-real-nim224-wasi33-command', 'profile': contract,
            'toolchain_id': 'genuine-nim224-and-matching-wasi-sdk33', 'sysroot': '.', 'resource_dir': None,
            'library_abi': {'address_bits': 32, 'wasi_pointer_abi': 'wasip1-memory32',
                'exceptions': False, 'threads': False, 'exception_encoding': 'none', 'thread_abi': 'none',
                'shared_memory': False, 'required_files': ['include/wasm32-wasip1/stdio.h',
                    'include/wasm32-wasip1/stdint.h', 'lib/wasm32-wasip1/libc.a',
                    'lib/wasm32-wasip1/libm.a', 'lib/wasm32-wasip1/crt1-command.o']},
            'files': files, 'runtime_requirements': [], 'languages': {'nim': {
                'version': '2.2.4', 'toolchain_id': 'genuine-nim224-and-matching-wasi-sdk33',
                'compiler': compiler_pin, 'linker': linker_pin, 'tool_dependencies': dependencies,
                'host_runtime_dependency_names': sorted(system_dependencies), 'compiler_argv': [],
                'linker_argv': [], 'source_ready': True, 'compiled': False, 'runtime_tested': False,
                'exception_library_closure_verified': False}}}]}
    manifest_path = staged / 'share/native_languages-nim-profile.json'
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
    report.update(profile_manifest_sha256=digest(manifest_path), tool_dependency_count=len(dependencies),
                  selected_artifact_count=len(files), host_runtime_dependency_names=sorted(system_dependencies))
    report['installed_nim_source_closure'] = {
        str(path.relative_to(origin / 'nim')): digest(path)
        for path in sorted((origin / 'nim').rglob('*')) if path.is_file()
    }
    installed = root / 'installed-sdk'
    run('cmake-configure', 'actual_CMake_configuration', ['/usr/bin/cmake', '-S', staged,
        '-B', root / 'cmake-build', '-DCMAKE_INSTALL_PREFIX=' + str(installed), '-DROSWASM_INSTALL_EXAMPLES=ON'])
    run('cmake-install', 'actual_CMake_installation', ['/usr/bin/cmake', '--install', root / 'cmake-build'])
    prepared_pins = {str(path.relative_to(staged)): digest(path)
                     for path in staged.rglob('*') if path.is_file()}
    if any(not (installed / name).is_file() or digest(installed / name) != value
           for name, value in prepared_pins.items()):
        raise ValueError('CMake installation differs from prepared real SDK bytes')
    # These are fresh generated copies, not historical evidence or inputs.
    # Retain their exact source/manifest pins, then remove the redundant prepared
    # copy after byte-comparing the genuine installed files to reduce owner peak.
    report['prepared_install_byte_comparison'] = {'status': 'PASS', 'files': len(prepared_pins),
                                                  'pins': prepared_pins}
    shutil.rmtree(staged)
    relocated = root / 'relocated-sdk'
    installed.rename(relocated)
    if (relocated / 'origin').exists() or (relocated / 'test').exists() or (relocated / 'tests').exists():
        raise ValueError('Installed SDK contains maintenance/test dependencies')
    if any(path.is_symlink() for path in relocated.rglob('*')):
        raise ValueError('Scoped SDK must contain ordinary materialized files')
    xmake = Path('/home/macromodel/.local/bin/xmake')
    if digest(xmake) != 'd24ec7daa11ab5007f33221ba0a883f9c7479afae2a47370773f209047152a1e':
        raise ValueError('Actual xmake executable changed')
    runtime = agents / '10/runtime-denial-r2/work/uwvm-dual'
    if digest(runtime) != '0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556':
        raise ValueError('Actual Runtime executable changed')
    sdk_pins_before = {str(path.relative_to(relocated)): digest(path)
                       for path in relocated.rglob('*') if path.is_file()}
    project = root / 'project'
    project.mkdir()

    def xmake_command(action, *arguments):
        return [xmake, action, '--project=' + str(project),
                '--file=' + str(relocated / 'xmake.lua'), *arguments]

    for backend in ('ros1', 'ros2'):
        output = root / ('example-' + backend)
        run(backend + '-xmake-configure', 'actual_installed_xmake_configuration', xmake_command('f', '--yes',
            '--ros-sdk-platforms=ros', '--ros-sdk-languages=nim', '--sdk-example-platform=ros',
            '--sdk-example-language=nim', '--sdk-example=progressive', '--sdk-ros-backend=' + backend,
            '--sdk-target=wasm32-wasip1', '--sdk-startup=command', '--sdk-exceptions=n', '--sdk-threads=n',
            '--sdk-offline=y', '--sdk-toolchain=' + str(agents), '--sdk-python=/usr/bin/python3',
            '--sdk-profile-manifest=' + str(relocated / 'share/native_languages-nim-profile.json'),
            '--sdk-native-c-compiler=' + str(cc)), cwd=project)
        run(backend + '-xmake-build', 'actual_installed_xmake_build', xmake_command('sdk-example-build',
            '--output=' + str(output)), cwd=project, timeout=420)
        built = json.loads((output / 'build-receipt.json').read_bytes())
        if len(built['artifacts']) != 1:
            raise ValueError('Exactly one real Wasm command is required')
        artifact = Path(built['artifacts'][0]['path']).resolve(strict=True)
        run(backend + '-validate', 'independent_Wasm_validator', ['/usr/bin/wasm-validate', artifact])
        run(backend + '-imports', 'actual_binary_import_contract', [sys.executable, '-I', '-S', '-B',
            relocated / 'tools/check_wasm_imports.py', artifact, '--backend', backend[-1],
            '--allow-module', 'wasi_snapshot_preview1', '--report', root / (backend + '-imports.json')])
        if not artifact.is_relative_to(output) or digest(artifact) != built['artifacts'][0]['sha256']:
            raise ValueError('Actual Wasm product differs from its receipt')
        run(backend + '-Guest', 'actual_Guest_empty_authority', xmake_command('sdk-example-run',
            '--receipt=' + str(output / 'build-receipt.json'), '--runtime=' + str(runtime)),
            cwd=project, marker='Complete UTF-8: ROS robot', timeout=60)
        report['Guest_passes'] += 1
        save()
    sdk_pins_after = {str(path.relative_to(relocated)): digest(path)
                      for path in relocated.rglob('*') if path.is_file()}
    report['SDK_unchanged_during_build'] = sdk_pins_before == sdk_pins_after
    if sdk_pins_before != sdk_pins_after:
        raise ValueError('Installed SDK changed during isolated xmake work')
    report['status'] = ('PASS_CURRENT_PUBLIC_INSTALLED_NIM_XMAKE_TWO_BACKENDS'
                        if args.public_current else 'PASS_REAL_INSTALLED_NIM_XMAKE_WITH_PRIVATE_SHARED_CANDIDATE')
    save()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
