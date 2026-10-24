#!/usr/bin/env python3
"""Fetch pinned Rust/Go/Zig toolchains in an admitted finite owner workload.

All archives are verified against captured official manifest SHA-256 values.
No installer changes system paths, startup files, trust stores or policy.
"""
from pathlib import Path
import hashlib
import json
import os
import subprocess
import tarfile
import tomllib
import urllib.request

ROOT=Path.cwd()
RECEIPTS=[]


def fetch(url,destination,limit,expected=None):
    if not url.startswith('https://'):raise ValueError('HTTPS source required')
    digest=hashlib.sha256();size=0
    with urllib.request.urlopen(url,timeout=60) as response, destination.open('xb') as output:
        while True:
            part=response.read(1024*1024)
            if not part:break
            size+=len(part)
            if size>limit:raise ValueError('download byte bound exceeded')
            digest.update(part);output.write(part)
    actual=digest.hexdigest()
    if expected is not None and actual!=expected:raise ValueError('official manifest digest mismatch')
    RECEIPTS.append({'url':url,'path':str(destination.relative_to(ROOT)),
                     'bytes':size,'sha256':actual,'manifest_verified':expected is not None})
    return destination.read_bytes() if expected is None else None


def unpack(path,destination):
    destination.mkdir()
    with tarfile.open(path) as archive:
        entries=archive.getmembers()
        if len(entries)>100000 or sum(m.size for m in entries)>2*1024**3:
            raise ValueError('archive expansion bound exceeded')
        for m in entries:
            p=Path(m.name)
            if p.is_absolute() or '..' in p.parts or m.isdev() or m.isfifo():raise ValueError('unsafe archive member')
        archive.extractall(destination,filter='data')


def main():
    downloads=ROOT/'downloads';downloads.mkdir()
    tools=ROOT/'toolchains';tools.mkdir()
    results=[]
    jobs=['rust','go','zig']
    for name in jobs:
        try:
            if name=='rust':
                manifest=fetch('https://static.rust-lang.org/dist/channel-rust-1.85.0.toml',downloads/'rust-1.85.0.toml',2*1024**2)
                spec=tomllib.loads(manifest.decode())
                prefix=tools/'rust';prefix.mkdir()
                for package,target in [('rustc','x86_64-unknown-linux-gnu'),('cargo','x86_64-unknown-linux-gnu'),
                                       ('rust-std','x86_64-unknown-linux-gnu'),('rust-std','wasm32-wasip1'),('rust-std','wasm32-unknown-unknown')]:
                    row=spec['pkg'][package]['target'][target]
                    if not row['available']:raise ValueError('requested Rust component unavailable')
                    stem=package+'-'+target;archive=downloads/(stem+'.tar.xz')
                    fetch(row['xz_url'],archive,512*1024**2,row['xz_hash'])
                    destination=tools/(stem+'-input');unpack(archive,destination)
                    entries=list(destination.iterdir())
                    if len(entries)!=1:raise ValueError('one compiler archive root required')
                    subprocess.run(['/bin/sh',str(entries[0]/'install.sh'),'--prefix='+str(prefix),'--disable-ldconfig'],check=True,timeout=120)
            elif name=='go':
                manifest=fetch('https://go.dev/dl/?mode=json&include=all',downloads/'go-releases.json',8*1024**2)
                versions=json.loads(manifest)
                row=next(f for v in versions if v['version']=='go1.24.2' for f in v['files'] if f['os']=='linux' and f['arch']=='amd64' and f['kind']=='archive')
                path=downloads/row['filename'];fetch('https://go.dev/dl/'+row['filename'],path,256*1024**2,row['sha256'])
                unpack(path,tools/'go-input')
            else:
                manifest=fetch('https://ziglang.org/download/index.json',downloads/'zig-index.json',2*1024**2)
                row=json.loads(manifest)['0.14.1']['x86_64-linux']
                path=downloads/'zig-0.14.1.tar.xz';fetch(row['tarball'],path,256*1024**2,row['shasum'])
                unpack(path,tools/'zig-input')
            results.append({'language':name,'status':'ACQUIRED_NOT_TESTED'})
        except Exception as error:
            results.append({'language':name,'status':'ACQUISITION_FAILED','error':str(error)})
        (ROOT/'acquisition03.json').write_text(json.dumps({'results':results,'sources':RECEIPTS,
            'default_authority':[],'compiler_runs':0,'Guest_runs':0},indent=2)+'\n')
    print(json.dumps(results))


if __name__=='__main__':main()
