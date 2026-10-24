#!/usr/bin/env python3
"""Recover pinned official CPython source as inert data inside the Linux owner.

No installer, archived controller, compiler, interpreter or Guest is run here.
Only verified regular source members are copied; tests/docs for other platforms
are excluded explicitly to keep this finite producer input small.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import tarfile
import urllib.request


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    a=p.parse_args()
    root,out=a.root.resolve(strict=True),a.out
    assert out.parent==root/'out'and not out.exists()
    out.mkdir(mode=0o700)
    url='https://www.python.org/ftp/python/3.14.8/Python-3.14.8.tar.xz'
    expected='c2215904f02b175596dc49351585104f4bc20341e1c47378b26a2c274360ce73'
    inputs=json.loads((root/'inputs.json').read_bytes())
    reused=inputs.get('python_source_fetch_reuse')
    prior=root/'out'/reused if reused else out
    assert prior.parent==root/'out'
    archive=prior/'Python-3.14.8.tar.xz'
    report=dict(status='RUNNING',url=url,expected_bytes=24130644,expected_sha256=expected,
                default_authority=[],compiler_guest_native_or_official_cases_run=0)
    def save():(out/'pythonfetch.json').write_text(json.dumps(report,indent=2)+'\n')
    try:
        h=hashlib.sha256();size=0
        if reused:
            with archive.open('rb')as stream:
                while block:=stream.read(65536):
                    size+=len(block);h.update(block)
        else:
            with urllib.request.urlopen(url,timeout=30)as response,archive.open('xb')as stream:
                while block:=response.read(65536):
                    size+=len(block)
                    assert size<=24130644
                    h.update(block);stream.write(block)
        assert size==24130644 and h.hexdigest()==expected
        source=prior/'Python-3.14.8'
        if not reused:source.mkdir()
        kept=[];skipped=[];total=0
        with tarfile.open(archive,'r|xz')as tar:
            for member in tar:
                relative=Path(member.name)
                assert not relative.is_absolute()and'..'not in relative.parts and relative.parts[0]=='Python-3.14.8'
                parts=relative.parts[1:]
                if not parts:continue
                # The official configure driver imports test.support to detect
                # Py_DEBUG. Omitting that helper makes its failure look like a
                # debug interpreter; retain exactly that support package.
                support=(parts[:3]==('Lib','test','support')or parts==('Lib','test','__init__.py'))
                if (parts[0]in('Doc','.github','Android','iOS','Mac','PC','PCbuild')or
                        parts[:2]==('Lib','test')and not support):
                    continue
                if member.isdir():continue
                if not member.isfile():
                    skipped.append(dict(path=str(relative),kind=member.type.decode('ascii','replace')))
                    continue
                assert member.size<=16<<20
                total+=member.size
                assert total<=192<<20 and len(kept)<20000
                data=tar.extractfile(member).read((16<<20)+1)
                assert len(data)==member.size
                destination=source/Path(*parts)
                destination.parent.mkdir(parents=True,exist_ok=True)
                assert destination.parent.resolve().is_relative_to(source)
                if reused and destination.exists():
                    assert destination.is_file()and not destination.is_symlink()and destination.read_bytes()==data
                else:
                    with destination.open('xb')as stream:stream.write(data)
                    os.chmod(destination,member.mode&0o777)
                kept.append(dict(path=destination.relative_to(source.parent).as_posix(),bytes=len(data),sha256=hashlib.sha256(data).hexdigest()))
        assert (source/'Tools/wasm/wasi/__main__.py').is_file()
        report.update(status='OFFICIAL_PINNED_CPYTHON_SOURCE_RECOVERED_NO_CODE_EXECUTED',
                      archive_bytes=size,archive_sha256=h.hexdigest(),source=str(source),
                      extracted_bytes=total,files=kept,skipped_nonregular=skipped)
        print(report['status'],total,len(kept))
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
