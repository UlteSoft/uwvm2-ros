#!/usr/bin/env python3
"""Rebuild one finite native/WASI CPython producer for vendor Guest acceptance.

Uses only the newly hash-verified official source and already admitted WASI
SDK. The static target library needs no target-interpreter bootstrap execution;
the declared UWVM host runner keeps zero authority and is not invoked here.
No ROS process or vendor application executes during this producer build.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def pin(path):
    with path.open('rb')as stream:h=hashlib.file_digest(stream,'sha256').hexdigest()
    return dict(path=str(path),bytes=path.stat().st_size,sha256=h)


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a=p.parse_args()
    root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes())
    recovered=json.loads((root/'out'/inputs.get('python_source_fetch_result','pythonfetch-r2')/'pythonfetch.json').read_bytes())
    assert recovered['status']=='OFFICIAL_PINNED_CPYTHON_SOURCE_RECOVERED_NO_CODE_EXECUTED'
    source=Path(recovered['source'])
    sdk=Path(inputs['toolchains'])/'wasi-sdk-33.0-x86_64-linux'
    assert out.parent==root/'out'and not out.exists()
    out.mkdir(mode=0o700);(out/'commands').mkdir()
    report=dict(status='RUNNING',commands=[],default_authority=[],make_jobs=1,
        source_archive_sha256=recovered['archive_sha256'],sdk=str(sdk),
        target_interpreter_bootstrap_runs=0,actual_ros_or_vendor_application_runs=0)
    env=dict(os.environ,CFLAGS='-O2 -g0',CXXFLAGS='-O2 -g0',MAKEFLAGS='-j1',
             SOURCE_DATE_EPOCH='1790726400',PYTHONHASHSEED='0',PYTHONDONTWRITEBYTECODE='1',
             WASI_SDK_PATH=str(sdk),PATH='/usr/bin:/bin')
    def save():(out/'pythonbuild.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,*,extra=None,timeout=1200):
        directory=out/'commands'/label;directory.mkdir()
        row=dict(label=label,argv=list(map(str,argv)),cwd=str(source),exit=None)
        report['commands'].append(row);save()
        with (directory/'stdout').open('xb')as stdout,(directory/'stderr').open('xb')as stderr:
            result=subprocess.run(row['argv'],cwd=source,env=dict(env,**(extra or {})),
                stdin=subprocess.DEVNULL,stdout=stdout,stderr=stderr,timeout=timeout,check=False)
        row.update(exit=result.returncode,stdout=pin(directory/'stdout'),stderr=pin(directory/'stderr'));save()
        assert result.returncode==0,(label,(directory/'stderr').read_text()[-2600:])
        print('CPYTHON_VENDOR_PRODUCER_STAGE_PASS',label,flush=True)
        return (directory/'stdout').read_bytes()
    try:
        for row in recovered['files']:
            path=source.parent/row['path'];assert pin(path)['sha256']==row['sha256']
        driver=source/'Tools/wasm/wasi'
        run('configure-native',[sys.executable,'-I','-S','-B',driver,'configure-build-python','--',
                                '--without-ensurepip','--disable-test-modules'])
        native=[p for p in(source/'cross-build').iterdir()if p.is_dir()and p.name!='wasm32-wasip1']
        assert len(native)==1
        native=native[0]
        run('build-native',['/usr/bin/make','-C',native,'-j1','all'])
        assert run('native-version',[native/'python','--version'],timeout=30)==b'Python 3.14.8\n'
        runner=inputs['runtime']+' -Rint --wasip1-global-noinherit-system-environment --run'
        run('configure-wasi',[sys.executable,'-I','-S','-B',driver,'configure-host','--wasi-sdk',sdk,
            '--host-runner',runner,'--','--without-ensurepip','--disable-test-modules'],
            extra=dict(LDFLAGS='-Wl,--threads=1'))
        target=source/'cross-build/wasm32-wasip1'
        run('build-static-wasi',['/usr/bin/make','-C',target,'-j1','libpython3.14.a'],
            extra=dict(LDFLAGS='-Wl,--threads=1'))
        for row in recovered['files']:
            path=source.parent/row['path'];assert pin(path)['sha256']==row['sha256']
        report.update(status='FRESH_CPYTHON_3148_NATIVE_AND_STATIC_WASI_PRODUCER_PASS',
            source=str(source),target=str(target),native_python=str(native/'python'),
            optimization='O2-g0, serial native/all and target/static-library only',
            files=[pin(p)for p in(native/'python',native/'pyconfig.h',target/'pyconfig.h',target/'Makefile',target/'libpython3.14.a')])
        print(report['status'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
