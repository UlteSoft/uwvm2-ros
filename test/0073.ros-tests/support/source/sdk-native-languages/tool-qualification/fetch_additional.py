#!/usr/bin/env python3
"""Explicit selected compiler acquisition from captured upstream digest manifests.

An unavailable release/digest is a retained failure, never a source-built shim
substituted for the requested language compiler. Run under the shared owner.
"""
from pathlib import Path
import argparse
import hashlib
import json
import re
import tarfile
import urllib.request

ROOT=Path.cwd();receipts=[]


def get(url,path,limit,expected=None):
    request=urllib.request.Request(url,headers={'User-Agent':'uwvm2-sdk-source-qualification/03','Accept':'application/vnd.github+json' if 'api.github.com/' in url else '*/*'})
    size=0;digest=hashlib.sha256()
    with urllib.request.urlopen(request,timeout=60) as response,path.open('xb') as stream:
        while True:
            b=response.read(1024*1024)
            if not b:break
            size+=len(b)
            if size>limit:raise ValueError('download bound exceeded')
            digest.update(b);stream.write(b)
    actual=digest.hexdigest()
    if expected and actual!=expected:raise ValueError('captured upstream digest mismatch')
    receipts.append({'url':url,'path':str(path.relative_to(ROOT)),'bytes':size,'sha256':actual,'manifest_verified':expected is not None})
    return path.read_bytes() if expected is None else None


def unpack(path,target,expanded_limit=2*1024**3):
    target.mkdir()
    with tarfile.open(path) as archive:
        members=archive.getmembers()
        if len(members)>100000 or sum(m.size for m in members)>expanded_limit:raise ValueError('archive bound exceeded')
        archive.extractall(target,filter='data')


def acquire_tinygo_go(version,downloads,tools):
    """Retain an exact official Go root; acquisition executes no tool binary.

    TinyGo0.43's original builder/config.go admits Go1.25..1.27 and separately
    checks the Go minor used to build TinyGo. Only a later actual compilation
    can qualify the selected pair. Never reuse the retained Go1.24 root here.
    """
    raw=get('https://go.dev/dl/?mode=json&include=all',
            downloads/'tinygo-go-releases.json',16*1024**2)
    releases=[row for row in json.loads(raw) if row.get('version')=='go'+version]
    if len(releases)!=1:raise ValueError('one exact official matching Go release required')
    name='go'+version+'.linux-amd64.tar.gz'
    assets=[row for row in releases[0]['files'] if row.get('filename')==name
            and row.get('os')=='linux' and row.get('arch')=='amd64'
            and row.get('kind')=='archive']
    if len(assets)!=1:raise ValueError('one exact official matching Go Linux archive required')
    asset=assets[0];expected=asset.get('sha256')
    if not isinstance(expected,str) or not re.fullmatch('[0-9a-f]{64}',expected):
        raise ValueError('official Go archive digest unavailable')
    archive=downloads/name
    get('https://go.dev/dl/'+name,archive,128*1024**2,expected)
    if archive.stat().st_size!=asset.get('size'):raise ValueError('official Go archive extent changed')
    target=tools/'tinygo-go';unpack(archive,target,768*1024**2)
    goroot=target/'go'
    if (goroot/'VERSION').read_text().splitlines()[0]!='go'+version or not (goroot/'bin/go').is_file():
        raise ValueError('extracted actual Go root/version missing')
    return {'version':version,'root':str(goroot.relative_to(ROOT)),
            'archive_sha256':expected,'compiler_runs':0,
            'pair_qualification':'ACQUIRED_ONLY_ACTUAL_TINYGO_BUILD_REQUIRED'}


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--language',action='append',choices=('tinygo','nim','d','odin'),required=True)
    ap.add_argument('--tinygo-version',choices=('0.38.0','0.43.0'),default='0.38.0')
    ap.add_argument('--tinygo-go-version',help='Exact official Go release for the TinyGo0.43 reactor, e.g.1.25.0')
    args=ap.parse_args()
    if len(set(args.language))!=len(args.language):raise ValueError('unique explicit language selection required')
    if args.tinygo_go_version is not None and (
            'tinygo' not in args.language or args.tinygo_version!='0.43.0'
            or not re.fullmatch(r'1\.(?:25|26|27)\.\d+',args.tinygo_go_version)):
        raise ValueError('exact Go1.25..1.27 selection belongs only to the explicit TinyGo0.43 reactor acquisition')
    if 'tinygo' in args.language and args.tinygo_version=='0.43.0' and args.tinygo_go_version is None:
        raise ValueError('TinyGo0.43 acquisition requires its explicitly selected matching Go archive')
    downloads=ROOT/'downloads';downloads.mkdir();tools=ROOT/'toolchains';tools.mkdir();rows=[]
    tinygo_pattern=r'tinygo'+re.escape(args.tinygo_version)+r'\.linux-amd64\.tar\.gz'
    specifications={'tinygo':('tinygo-org/tinygo','v'+args.tinygo_version,tinygo_pattern),
                    'd':('ldc-developers/ldc','v1.40.1',r'ldc2-1\.40\.1-linux-x86_64\.tar\.xz'),
                    'odin':('odin-lang/Odin','dev-2025-10',r'.*linux.*(?:amd64|x86_64).*\.tar\.(?:gz|xz)')}
    for language in args.language:
        row={'language':language}
        try:
            if language=='nim':
                name='nim-2.2.4-linux_x64.tar.xz';url='https://nim-lang.org/download/'+name
                raw=get(url+'.sha256',downloads/(name+'.sha256'),65536)
                matches=re.findall(rb'\b[0-9a-fA-F]{64}\b',raw)
                if len(matches)!=1:raise ValueError('one exact official Nim SHA256 required')
                expected=matches[0].decode().lower()
            else:
                repository,tag,pattern=specifications[language]
                raw=get('https://api.github.com/repos/'+repository+'/releases/tags/'+tag,downloads/(language+'-release.json'),4*1024**2)
                manifest=json.loads(raw)
                if manifest.get('tag_name')!=tag or manifest.get('draft'):raise ValueError('exact published release required')
                assets=[a for a in manifest['assets'] if re.fullmatch(pattern,a['name'])]
                if len(assets)!=1:raise ValueError('one exact Linux x86_64 release archive required')
                selected=assets[0];name=selected['name'];url=selected['browser_download_url'];digest=selected.get('digest')
                if isinstance(digest,str) and re.fullmatch('sha256:[0-9a-f]{64}',digest):
                    expected=digest.split(':')[1]
                else:
                    sums=[a for a in manifest['assets'] if a['name'].endswith('.sha256sums.txt')]
                    if len(sums)!=1:raise ValueError('upstream release asset SHA256/checksum manifest unavailable; no unverified execution')
                    text=get(sums[0]['browser_download_url'],downloads/(language+'-sha256sums.txt'),1024*1024).decode('utf-8')
                    values=[]
                    for line in text.splitlines():
                        match=re.fullmatch(r'([0-9a-fA-F]{64})\s+\*?(.+)',line.strip())
                        if match and match[2]==name:values.append(match[1].lower())
                    if len(values)!=1:raise ValueError('one exact upstream checksum for selected archive required')
                    expected=values[0]
                row['release']={'repository':repository,'tag':tag,'id':manifest['id'],'target_commitish':manifest['target_commitish'],'asset_id':selected['id']}
            archive=downloads/name;get(url,archive,512*1024**2,expected)
            unpack(archive,tools/language)
            row.update(status='ACQUIRED_NOT_COMPILED',archive_sha256=expected)
            if language=='tinygo' and args.tinygo_version=='0.43.0':
                row['matching_Go']=acquire_tinygo_go(args.tinygo_go_version,downloads,tools)
        except Exception as error:row.update(status='ACQUISITION_FAILED',error=str(error))
        rows.append(row)
        (ROOT/'acquisition-additional03.json').write_text(json.dumps({'rows':rows,'sources':receipts,'default_authority':[],'compiler_runs':0,'Guest_runs':0},indent=2)+'\n')
        print(json.dumps(row),flush=True)
    return 1 if any(row['status']=='ACQUISITION_FAILED' for row in rows) else 0


if __name__=='__main__':raise SystemExit(main())
