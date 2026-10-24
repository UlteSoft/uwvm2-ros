#!/usr/bin/env python3
"""@brief Build the genuine installed Nim SDK with isolated xmake project output.

@details Reuse the previously CMake-installed and relocated SDK as an explicit
read-only input. Every SDK file is hashed before and after; fresh project,
configuration, compiler outputs and logs belong to this finite owner. The one
owned adapter overlay is pinned explicitly and preserved with the receipt.
No SDK copy, fake profile, alternate builder or Host substitute is created.
@see review03_installed_nim.py; probe_xmake_root.py
"""
from pathlib import Path
import hashlib
import json
import shutil
import subprocess
import sys


def digest(path):
    """@brief Compute the complete SHA-256 of an ordinary input/product file.

    @param[in] path File borrowed for the duration of the synchronous read.
    @return Lowercase digest; I/O errors propagate.
    @see main
    """
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    """@brief Build/validate/run both ROS backends through real installed xmake.

    @details The existing SDK's source and profile closure are unchanged during
    work. Xmake's explicit project/file options direct all new state to this
    owner's fresh project. Nim jsondoc additionally produces a real frontend
    declaration inventory; its content still needs independent semantic review.
    @return Zero only after both requested xmake/Wasm/import/Guest paths pass.
    @failure SDK/tool drift, unexpected project writes, failed compilation or
    execution retains evidence and returns failure. No effect is retried.
    @see digest; review03_installed_nim.main
    """
    root = Path.cwd()
    agents = Path('/home/macromodel/Documents/uwvm-test-261018/agents')
    previous = agents / '03/20261021-round03-installed-nim-r1/work'
    sdk = previous / 'relocated-sdk'
    parent_result = previous / 'installed-nim.json'
    original = json.loads(parent_result.read_bytes())
    adapter = sdk / 'tools/xmake/languages/native_languages.lua'
    if digest(adapter) != 'dae6b4ddd1a340e2262e7f2e0b5cb99dc8247c0ded53de2257b3687b4f6af7e1':
        raise ValueError('Exact owned normalized-root adapter overlay required')
    before = {str(path.relative_to(sdk)): digest(path) for path in sdk.rglob('*') if path.is_file()}
    for name, expected in original['prepared_install_byte_comparison']['pins'].items():
        if name != 'tools/xmake/languages/native_languages.lua' and before.get(name) != expected:
            raise ValueError('Original installed SDK input changed: ' + name)
    shutil.copy2(adapter, root / 'native_languages-overlay.lua')
    rows = []
    report = {'kind': 'task03-real-installed-nim-resume-v1', 'status': 'RUNNING', 'rows': rows,
              'sdk_root': str(sdk), 'SDK_pins_before': before,
              'parent_install_receipt_sha256': digest(parent_result),
              'uses_private_shared_candidate': True,
              'private_overlay': {'path': 'tools/xmake/languages/native_languages.lua',
                  'before_sha256': original['prepared_install_byte_comparison']['pins']['tools/xmake/languages/native_languages.lua'],
                  'after_sha256': digest(adapter)},
              'Guest_passes': 0, 'native_ROS_runs': 0, 'Gateway_runs': 0,
              'official_upstream_credit': 0, 'public_API_documentation_accepted': False,
              'default_authority': []}

    def save():
        (root / 'installed-nim-resume.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(name, layer, command, cwd=root, marker=None, required=True, timeout=180):
        log = root / (name + '.log')
        row = {'name': name, 'layer': layer, 'argv': [str(item) for item in command],
               'cwd': str(cwd), 'log': log.name}
        rows.append(row)
        save()
        try:
            with log.open('wb') as output:
                result = subprocess.run(row['argv'], cwd=cwd, stdout=output,
                                        stderr=subprocess.STDOUT, timeout=timeout)
            text = log.read_text(errors='replace')
            row.update(exit=result.returncode, log_sha256=digest(log),
                       status='PASS' if result.returncode == 0 and (marker is None or marker in text) else 'FAIL')
        except Exception as error:
            row.update(status='FAIL', error=str(error))
        if required and row['status'] != 'PASS':
            report['status'] = 'FAILED_RETAINED'
        save()
        print(name, row['status'], flush=True)
        if required and row['status'] != 'PASS':
            raise ValueError('Retained failure: ' + name)
        return row

    xmake = Path('/home/macromodel/.local/bin/xmake')
    runtime = agents / '10/runtime-denial-r2/work/uwvm-dual'
    cc = agents / '10/staged-wasi-r1/staged/wasi-sdk/bin/clang'
    nim = agents / '03/20261021-additional-tools-r2/work/toolchains/nim/nim-2.2.4/bin/nim'
    if digest(xmake) != 'd24ec7daa11ab5007f33221ba0a883f9c7479afae2a47370773f209047152a1e':
        raise ValueError('Xmake program drift')
    if digest(runtime) != '0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556':
        raise ValueError('Runtime program drift')
    project = root / 'project'
    project.mkdir()

    def xmake_command(action, *arguments):
        return [xmake, action, '--project=' + str(project), '--file=' + str(sdk / 'xmake.lua'), *arguments]

    for backend in ('ros1', 'ros2'):
        output = root / ('example-' + backend)
        run(backend + '-configure', 'actual_installed_xmake_configuration', xmake_command('f', '--yes',
            '--ros-sdk-platforms=ros', '--ros-sdk-languages=nim', '--sdk-example-platform=ros',
            '--sdk-example-language=nim', '--sdk-example=progressive', '--sdk-ros-backend=' + backend,
            '--sdk-target=wasm32-wasip1', '--sdk-startup=command', '--sdk-exceptions=n', '--sdk-threads=n',
            '--sdk-offline=y', '--sdk-toolchain=' + str(agents), '--sdk-python=/usr/bin/python3',
            '--sdk-profile-manifest=' + str(sdk / 'share/native_languages-nim-profile.json'),
            '--sdk-native-c-compiler=' + str(cc)), cwd=project)
        run(backend + '-build', 'actual_installed_xmake_build',
            xmake_command('sdk-example-build', '--output=' + str(output)), cwd=project, timeout=420)
        receipt = json.loads((output / 'build-receipt.json').read_bytes())
        if len(receipt['artifacts']) != 1:
            raise ValueError('One real Wasm command expected')
        artifact = Path(receipt['artifacts'][0]['path']).resolve(strict=True)
        if not artifact.is_relative_to(output) or digest(artifact) != receipt['artifacts'][0]['sha256']:
            raise ValueError('Actual example output differs from its receipt')
        run(backend + '-validator', 'independent_Wasm_validation', ['/usr/bin/wasm-validate', artifact])
        run(backend + '-imports', 'actual_binary_import_contract', [sys.executable, '-I', '-S', '-B',
            sdk / 'tools/check_wasm_imports.py', artifact, '--backend', backend[-1],
            '--allow-module', 'wasi_snapshot_preview1', '--report', root / (backend + '-imports.json')])
        run(backend + '-Guest', 'actual_Guest_empty_authority',
            xmake_command('sdk-example-run', '--receipt=' + str(output / 'build-receipt.json'),
                           '--runtime=' + str(runtime)), cwd=project,
            marker='Complete UTF-8: ROS robot', timeout=60)
        report['Guest_passes'] += 1
        save()
    docs = root / 'frontend-docs'
    docs.mkdir()
    package = sdk / 'lib/platform/ros/nim/source/src'
    report['frontend_inventories'] = []
    for name, path in [('owner', package / 'roswasm.nim'), ('wire', package / 'roswasm/wire.nim'),
                       ('typed', package / 'roswasm/typed.nim'), ('raw', package / 'roswasm/raw.nim')]:
        out = docs / (name + '.json')
        row = run(name + '-frontend-jsondoc', 'actual_Nim_frontend_declaration_inventory',
            [nim, 'jsondoc', '--mm:arc', '--threads:off', '--exceptions:goto', '--cpu:wasm32',
             '--os:any', '-d:wasm', '-d:useMalloc', '--path:' + str(package), '--out:' + str(out), path],
            required=False)
        report['frontend_inventories'].append({'module': name, 'source_sha256': digest(path),
            'status': row['status'], 'output_sha256': digest(out) if out.exists() else None})
        save()
    after = {str(path.relative_to(sdk)): digest(path) for path in sdk.rglob('*') if path.is_file()}
    report['SDK_unchanged_during_build'] = before == after
    if before != after:
        report['status'] = 'FAILED_SDK_INPUT_MUTATION'
        save()
        raise ValueError('Read-only installed SDK changed during isolated xmake work')
    report['status'] = 'PASS_INSTALLED_NIM_XMAKE_TWO_BACKENDS_WITH_PINNED_PRIVATE_OVERLAY'
    save()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
