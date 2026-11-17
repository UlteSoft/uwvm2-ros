#!/usr/bin/env python3
"""Offline provenance and vendor archive boundary tests; no upstream execution."""
import argparse
import json
from pathlib import Path
import stat
import warnings
import zipfile

from capability_inventory import ROOT, guard, sha
from platform_sources import repository, entries
from platform_archive import extract, relative


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();guard();out=args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    checks=[]
    for url in ['http://github.com/a/b','https://user@github.com/a/b','https://github.com/a/b?token=x',
                'https://github.com/a/b#main','https://github.com/a/../b','https://github.com/a/b/tree/main','https://example.com/a/b']:
        assert repository(url) is None;checks.append('reject '+url)
    assert repository('org/repo')=='https://github.com/org/repo'
    assert repository('https://github.com/org/repo.git')=='https://github.com/org/repo';checks.append('exact github normalization')
    revision='a'*40
    assert list(entries(dict(source_proof=dict(upstream_repository='org/repo',upstream_commit=revision))))[0]['ref']==revision
    assert list(entries(dict(repository='org/repo',branch='main')))[0]['ref'] is None
    checks.append('preserve exact revisions and unresolved branches')
    for path in ['', '.', '../escape','/absolute','a/../b','a//b','a\\b','C:/a','a//']:
        try:relative(path)
        except ValueError:checks.append('reject path '+path)
        else:raise AssertionError(path)
    def archive(name,records,success):
        target=out/(name+'.zip');destination=out/name;destination.mkdir()
        with zipfile.ZipFile(target,'w',compression=zipfile.ZIP_DEFLATED) as writer,warnings.catch_warnings():
            warnings.simplefilter('ignore',UserWarning)
            for path,data,mode in records:
                entry=zipfile.ZipInfo(path);entry.create_system=3;entry.external_attr=mode<<16
                writer.writestr(entry,data,compress_type=zipfile.ZIP_DEFLATED)
        try:
            result=extract(target,destination,['sdk/src/msgs'])
        except (ValueError,FileExistsError):
            assert not success,name;checks.append(name);return
        assert success,(name,result);checks.append(name)
        assert not (destination/'sdk/bin/native').exists()
        assert all(not p.stat().st_mode&0o111 for p in destination.rglob('*') if p.is_file())
    regular=stat.S_IFREG|0o755
    archive('only-declared-definitions',[('sdk/src/msgs/State.msg',b'float64 value\n',regular),('sdk/bin/native',b'ELF-not-executed',regular)],True)
    archive('missing-root',[('sdk/other/value',b'x',regular)],False)
    archive('parent-traversal',[('../escape',b'x',regular)],False)
    archive('duplicate',[('sdk/src/msgs/State.msg',b'x',regular),('sdk/src/msgs/State.msg',b'y',regular)],False)
    archive('symlink',[('sdk/src/msgs/link',b'/etc/passwd',stat.S_IFLNK|0o777)],False)
    archive('special-device',[('sdk/src/msgs/device',b'x',stat.S_IFCHR|0o600)],False)
    archive('expanded-file-bound',[('sdk/src/msgs/Huge.msg',b'x'*(8*1024*1024+1),regular)],False)
    sources={str(Path(__file__).with_name(p)):sha(Path(__file__).with_name(p)) for p in
             ['platform_sources_checks.py','platform_sources.py','platform_archive.py','platform-source-adapters.json']}
    report=dict(complete=True,checks=checks,inputs=sources,network_requests=0,upstream_code_executed=False)
    (out/'results.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(checks),'platform acquisition boundary checks')


if __name__=='__main__':main()
