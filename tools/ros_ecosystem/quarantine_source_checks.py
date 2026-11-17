#!/usr/bin/env python3
"""Reject hostile source archives without ever materializing archive links."""
import argparse
import io
import json
from pathlib import Path
import tarfile

from capability_inventory import ROOT, guard, sha
from quarantine_source_links import extract


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();guard()
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    checks=[]
    def run(name,items,success):
        archive=out/(name+'.tar.gz');dest=out/name;dest.mkdir()
        with tarfile.open(archive,'w:gz') as writer:
            for path,kind,body in items:
                info=tarfile.TarInfo(path);info.mode=0o777;info.type=kind
                if kind in [tarfile.SYMTYPE,tarfile.LNKTYPE]:info.linkname=body
                elif kind==tarfile.REGTYPE:info.size=len(body)
                writer.addfile(info,io.BytesIO(body) if kind==tarfile.REGTYPE else None)
        try:result=extract(archive,dest)
        except (ValueError,FileExistsError,NotADirectoryError):
            assert not success,name;checks.append(name);return
        assert success,name
        assert all(not x.is_symlink() and not (x.is_file() and x.stat().st_mode&0o111) for x in dest.rglob('*'))
        assert all(not row['materialized'] and not row['referent_resolved'] for row in result['quarantined_links'])
        checks.append(name)
    for name in ['../escape','/absolute','a/../escape','a//b','./a','a\\b','C:/escape','.']:
        run('path-'+str(len(checks)),[(name,tarfile.REGTYPE,b'x')],False)
    run('duplicate',[('a',tarfile.REGTYPE,b'x'),('a',tarfile.REGTYPE,b'x')],False)
    run('device',[('a',tarfile.CHRTYPE,b'')],False)
    run('fifo',[('a',tarfile.FIFOTYPE,b'')],False)
    run('file-as-parent',[('a',tarfile.REGTYPE,b'x'),('a/b',tarfile.REGTYPE,b'x')],False)
    run('quarantine-links',[('sdk/',tarfile.DIRTYPE,b''),('sdk/absolute',tarfile.SYMTYPE,'/etc/passwd'),
        ('sdk/relative',tarfile.SYMTYPE,'../../escape'),('sdk/hard',tarfile.LNKTYPE,'/etc/shadow'),
        ('sdk/normal.h',tarfile.REGTYPE,b'int value;\n')],True)
    occupied=out/'occupied';occupied.mkdir();(occupied/'outside').symlink_to(out)
    try:extract(out/'quarantine-links.tar.gz',occupied)
    except ValueError:checks.append('reject-occupied-root')
    else:raise AssertionError('accepted occupied root')
    alias=out/'alias';alias.symlink_to(out/'quarantine-links',target_is_directory=True)
    try:extract(out/'quarantine-links.tar.gz',alias)
    except ValueError:checks.append('reject-symlink-root')
    else:raise AssertionError('accepted symlink root')
    sources={str(Path(__file__).with_name(n)):sha(Path(__file__).with_name(n)) for n in
        ['quarantine_source_checks.py','quarantine_source_links.py','platform_archive.py']}
    (out/'results.json').write_text(json.dumps(dict(complete=True,checks=checks,inputs=sources,upstream_code_executed=False),indent=2)+'\n')
    print('PASS',len(checks),'inert source quarantine boundaries')


if __name__=='__main__':main()
