"""Acquire pinned compiler and WABT for the existing four product profiles.

This step records provider bytes; the following original product and regression
steps supply execution results. It installs no system packages.
"""
from pathlib import Path
import argparse
import json
import os
import platform
import shutil
import sys

from acquire import digest, download, require, save, unpack


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--work', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    require(sys.platform == 'linux' and platform.machine() == 'x86_64', 'Linux x86_64 providers required')
    require(os.environ.get('GITHUB_ACTIONS') == 'true' and
            os.environ.get('RUNNER_ENVIRONMENT') == 'github-hosted', 'official hosted route required')
    work, out = args.work.resolve(), args.out.resolve()
    require(not work.exists() and not out.exists(), 'fresh owned provider directories required')
    out.mkdir(mode=0o700, parents=True)
    work.mkdir(mode=0o700, parents=True)
    row = {'status': 'FAIL', 'provider_execution_qualified': False, 'product_qualified': False,
           'workflow_sha': os.environ.get('GITHUB_SHA'), 'inputs': {}}
    try:
        require(shutil.disk_usage(work).free >= 3*1024**3, '3GiB provider space required')
        compiler_lock = Path(__file__).with_name('inputs.lock.json')
        wabt_lock = Path(__file__).with_name('wabt.lock.json')
        locked = json.loads(compiler_lock.read_text())
        compiler = locked['compiler']
        wabt = json.loads(wabt_lock.read_text())
        shutil.copy2(compiler_lock, out/compiler_lock.name)
        shutil.copy2(wabt_lock, out/wabt_lock.name)
        for name, spec in (('compiler', compiler), ('wabt', wabt)):
            archive = work/(name+'.tar.'+('xz' if name == 'compiler' else 'gz'))
            record = download(spec, archive, out/(name+'-download.json'))
            record['extraction'] = unpack(archive, work/name, compression='xz' if name == 'compiler' else 'gz')
            row['inputs'][name] = record
        cpp = sorted((work/'compiler').rglob('bin/clang++'))
        wat = sorted((work/'wabt').rglob('bin/wat2wasm'))
        require(len(cpp) == len(wat) == 1, 'one pinned compiler and assembler required')
        cpp, wat = cpp[0], wat[0]
        cc = cpp.with_name('clang')
        require(cpp.resolve().is_relative_to(work/'compiler') and
                cc.resolve().is_relative_to(work/'compiler') and wat.resolve().is_relative_to(work/'wabt'),
                'provider executable alias leaves its verified archive')
        require(digest(wat) == wabt['wat2wasm_sha256'], 'actual assembler image differs')
        values = {'UWVM_CI_CXX': str(cpp), 'UWVM_CI_CC': str(cc), 'UWVM_CI_WAT2WASM': str(wat)}
        sdk = work/'sdk-fixtures'
        row['sdk_fixture_scope'] = locked['sdk_fixture_scope']
        row['sdk_fixtures'] = []
        for index, spec in enumerate(locked['sdk_fixtures']):
            target = sdk/spec['path']
            target.parent.mkdir(parents=True, exist_ok=True)
            row['sdk_fixtures'].append(download(spec, target, out/('sdk-fixture-'+str(index)+'.json')))
        values['UWVM_ROS_SDK_ROOT'] = str(sdk)
        require(all('\n' not in value and '\r' not in value for value in values.values()), 'unsafe CI environment path')
        with Path(os.environ['GITHUB_ENV']).open('a') as stream:
            for key, value in values.items():
                stream.write(key+'='+value+'\n')
        row.update(status='PASS_PINNED_PROVIDER_BYTES_ONLY', executables={
            key: {'path': value, 'sha256': digest(value)} for key, value in values.items() if key != 'UWVM_ROS_SDK_ROOT'},
            standard_library='libstdc++ selected explicitly by original product driver option')
    except BaseException as error:
        row['error'] = repr(error)
        raise
    finally:
        save(out/'result.json', row)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
