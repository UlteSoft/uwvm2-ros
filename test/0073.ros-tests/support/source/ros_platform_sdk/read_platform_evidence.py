#!/usr/bin/env python3
"""Read back a streamed inert checkpoint without saving or executing its objects.

This is an archive-integrity reader, not a local ROS/compiler/Runtime test.
Supply the independently retained compressed byte count and SHA256. Every
regular object, its mode, manifest association and gzip EOF are checked with a
one-MiB content buffer. No archive path or symlink is materialized locally.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import sys
import tarfile


class Counted:
    def __init__(self,stream):
        self.stream=stream
        self.digest=hashlib.sha256()
        self.count=0

    def read(self,n=-1):
        assert 0<=n<=16<<20,'bounded compressed read required'
        body=self.stream.read(n)
        self.digest.update(body)
        self.count+=len(body)
        return body


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--expected-sha256',required=True)
    p.add_argument('--expected-bytes',required=True,type=int)
    p.add_argument('--out',required=True,type=Path)
    a=p.parse_args()
    assert re.fullmatch('[0-9a-f]{64}',a.expected_sha256)
    assert 0<a.expected_bytes<=2<<30 and not a.out.exists()
    raw=Counted(sys.stdin.buffer)
    seen=set()
    with gzip.GzipFile(fileobj=raw,mode='rb') as zipped:
        with tarfile.open(fileobj=zipped,mode='r|') as archive:
            members=iter(archive)
            member=next(members)
            assert member.name=='manifest.json' and member.isfile() and member.size<=16<<20
            body=archive.extractfile(member).read()
            manifest=json.loads(body)
            assert manifest['format']=='uwvm-owned-inert-objects-v1'
            objects={}
            paths=set()
            for row in manifest['roles']:
                role=PurePosixPath(row['path'])
                assert not role.is_absolute()and '..'not in role.parts and str(role)!='.'
                assert row['path']not in paths
                paths.add(row['path'])
                assert type(row['mode'])is int and 0<=row['mode']<=0o7777
                if row['type']=='regular':
                    assert re.fullmatch('[0-9a-f]{64}',row['sha256'])
                    assert row['object']=='objects/'+row['sha256']+'-'+oct(row['mode'])[2:]
                    assert type(row['bytes'])is int and 0<=row['bytes']<=128<<20
                    prior=objects.setdefault(row['object'],row)
                    assert all(prior[k]==row[k]for k in ('bytes','sha256','mode'))
                else:
                    assert row['type']in ('directory-metadata','symlink-metadata')
            for member in members:
                assert member.name in objects and member.name not in seen and member.isfile()
                row=objects[member.name]
                assert member.mode==row['mode'] and member.size==row['bytes']
                stream=archive.extractfile(member)
                digest=hashlib.sha256()
                while block:=stream.read(1<<20):digest.update(block)
                assert digest.hexdigest()==row['sha256']
                seen.add(member.name)
            assert seen==set(objects)
        while zipped.read(1<<20):pass
    assert raw.count==a.expected_bytes and raw.digest.hexdigest()==a.expected_sha256
    result=dict(status='CLIENT_STREAMED_INERT_OBJECTS_MODES_AND_GZIP_EOF_PASS',
        label=manifest['label'],bytes=raw.count,sha256=raw.digest.hexdigest(),
        manifest_sha256=hashlib.sha256(body).hexdigest(),roles=len(paths),objects=len(seen),
        archive_objects_extracted_or_executed=False,compiler_or_ros_tests_run=0)
    a.out.parent.mkdir(parents=True,exist_ok=True)
    with a.out.open('x')as stream:stream.write(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result),flush=True)


if __name__=='__main__':main()
