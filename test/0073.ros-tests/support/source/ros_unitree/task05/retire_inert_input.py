#!/usr/bin/env python3
"""Retire one explicitly owned redundant input after two archive readbacks.

No immutable archive, source repository, toolchain, result or log is removed.
Recovery is an ordinary extraction into a fresh owned reference directory.
"""
import hashlib
import json
import os
from pathlib import Path
import shutil
import tarfile


def digest(path):
    with path.open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    run=Path(os.environ['UWVM_RESOURCE_RUN']).resolve(strict=True)
    assert os.environ['UWVM_RESOURCE_HEAVY_LOCK_HELD']=='1'
    root=Path('/home/macromodel/Documents/uwvm-test-261018/agents')
    old=root/'05/20261021-platform-source-audit-r1'
    source=old/'inputs/source';assert source.is_dir() and not source.is_symlink()
    copies=[old/'resource-archives/evidence.tar',root/'director-20261021/infrastructure/archives/05-20261021-platform-source-audit-r1.tar']
    expected='b540c29936ffa71a1fb1f3f5bc0e2a3269ce07742b849a4afcdfbcb38402b27c'
    report=dict(status='RUNNING',removed=None,archives=[dict(path=str(path),sha256=digest(path)) for path in copies],
        files_verified=0,bytes_verified=0,archives_removed=False,repository_modified=False)
    result=run/'work/cleanup.json'
    try:
        assert all(row['sha256']==expected for row in report['archives'])
        with tarfile.open(copies[0],'r:') as archive:
            names=set()
            for member in archive:
                relative=Path(member.name)
                if relative.parts[:2]!=('inputs','source'):continue
                assert '..' not in relative.parts and not relative.is_absolute()
                local=old/relative;names.add(local)
                if member.isdir():assert local.is_dir() and not local.is_symlink()
                elif member.issym():assert local.is_symlink() and os.readlink(local)==member.linkname
                else:
                    assert member.isfile() and local.is_file() and not local.is_symlink()
                    assert local.resolve().is_relative_to(source.resolve())
                    assert member.size==local.stat().st_size
                    stream=archive.extractfile(member)
                    assert hashlib.file_digest(stream,'sha256').hexdigest()==digest(local)
                    report['files_verified']+=1;report['bytes_verified']+=member.size
            assert set(source.rglob('*'))<=names and report['files_verified']>0
        assert all(digest(path)==expected for path in copies)
        shutil.rmtree(source)
        report.update(status='PASS',removed=str(source),recovery_archive=str(copies[0]),recovery_prefix='inputs/source/')
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:result.write_text(json.dumps(report,indent=2)+'\n')


if __name__=='__main__':main()
