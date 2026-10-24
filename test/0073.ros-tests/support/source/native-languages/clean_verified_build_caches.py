#!/usr/bin/env python3
"""Remove only byte-verified archived Task03 compiler caches, under its owner.

Keep logs, Wasm/rlib products, tools, inputs, downloads and both archives.
Fixed candidates only; the ordinary common reserve/admission still applies.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import shutil
import tarfile


def sha(path):
    with path.open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--batch',choices=('original','go-values-r4'),required=True)
batch=parser.parse_args().batch
base=Path('/home/macromodel/Documents/uwvm-test-261018/agents')
runs={
    '20261021-validation-r1':(165089280,'8104bfcdc8864caac7b9dc87219876f74e73b87316ddfa33d995b0176536b9a2',('go-cache','zig-cache','zig-global')),
    '20261021-validation-r3':(200785920,'84f6cd45e8d7ad5d790a2954f6c620c84efb3743c7abfd14f7a90ebceabe294d',('go-cache','zig-cache','zig-global','cargo-target')),
    '20261021-validation-r5':(146247680,'d1eda1604e0e82e256de809939b2cf295388c4f0200c61eb96a765c53804ae25',('go-cache','zig-cache','zig-global')),
    '20261021-go-values-r1':(40663040,'441c9b5d25f0c48b69bde2b1550552e8c6ac6b356cd95cef6907924771052bb7',('go-cache',)),
}
if batch=='go-values-r4':
    runs={'20261021-go-values-r4':(40232960,'28359fabbaa7939d98f80b2763fc8b551e11f8385c6326c2dc3f41377fe6db51',('go-cache',))}
report={'kind':'task03-verified-build-cache-cleanup-v2','status':'VERIFYING','rows':[],
    'removed':[],'compiler_runs':0,'Guest_runs':0,'default_authority':[]}
out=Path.cwd()/'verified-cache-cleanup.json'


def save():out.write_text(json.dumps(report,indent=2)+'\n')


save();selected=[]
for run,(expected_bytes,expected_sha,names) in runs.items():
    root=base/'03'/run
    if root.resolve(strict=True)!=root:raise ValueError('ordinary fixed Task03 run root required')
    receipt=json.loads((root/'resource-archives/verified-copies.json').read_bytes())
    expected_paths={root/'resource-archives/evidence.tar',
        base/'director-20261021/infrastructure/archives'/('03-'+run+'.tar')}
    actual_paths={Path(value[5:] if value.startswith('/host/') else value) for value in receipt.get('verified_paths',[])}
    if receipt.get('copies')!=2 or actual_paths!=expected_paths or receipt.get('bytes')!=expected_bytes or receipt.get('sha256')!=expected_sha:
        raise ValueError('original two-copy receipt/pin differs: '+run)
    for path in sorted(actual_paths):
        if path.is_symlink() or path.stat().st_size!=expected_bytes or sha(path)!=expected_sha:
            raise ValueError('original immutable archive read-back failed: '+str(path))
    row={'run':run,'both_original_archives_read_back':True,'archive_sha256':expected_sha,'caches':[]}
    report['rows'].append(row);save()
    with tarfile.open(root/'resource-archives/evidence.tar') as archive:
        members={member.name.rstrip('/'):member for member in archive.getmembers()}
        for name in names:
            directory=root/'work'/name;prefix='work/'+name
            if directory.is_symlink() or not directory.is_dir() or directory.resolve(strict=True)!=directory or directory.stat().st_uid!=os.getuid():
                raise ValueError('ordinary task-owned cache required: '+str(directory))
            actual={path.relative_to(directory).as_posix():path for path in directory.rglob('*')}
            expected={key[len(prefix)+1:]:member for key,member in members.items() if key.startswith(prefix+'/')}
            if set(actual)!=set(expected):raise ValueError('cache entries differ from original archive: '+str(directory))
            files=0;total=0
            for relative,path in actual.items():
                member=expected[relative]
                if path.is_symlink() or path.stat().st_uid!=os.getuid():raise ValueError('cache ownership/link changed')
                if path.is_dir():
                    if not member.isdir():raise ValueError('archived cache kind differs')
                    continue
                if not path.is_file() or not member.isfile() or path.stat().st_size!=member.size:
                    raise ValueError('cache file size/kind differs')
                with archive.extractfile(member) as stream:archived=hashlib.file_digest(stream,'sha256').hexdigest()
                if sha(path)!=archived:raise ValueError('cache bytes differ from original archive: '+str(path))
                files+=1;total+=member.size
            row['caches'].append({'path':str(directory),'verified_files':files,'verified_bytes':total})
            selected.append(directory);save()

# No deletion happens until every archive and every candidate byte matches.
report['status']='ALL_CANDIDATES_VERIFIED';save()
for directory in selected:
    shutil.rmtree(directory);report['removed'].append(str(directory));save()
report['status']='PASS';save()
