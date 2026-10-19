#!/usr/bin/env python3
"""Small real-kernel checks for the independently owned CP310 CoW view.

Run only through Root's existing Linux 64 GiB/zero-swap owner, with the exact
reviewed helper SHA and a fresh fixture directory on the owned Btrfs artifact
filesystem. These tests never execute the copied interpreter or any ROS,
Runtime, compiler or simulator. Passing them proves only the file-copy policy.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import stat
import sys
import types
import unittest

GIB=1024**3
LEAF=Path('/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope')
BOOT='501c54bd-af24-4d22-900c-8e24de274464'
MODULE=OUT=None


def require(value,message):
    if not value:raise ValueError(message)


def guard():
    info=LEAF.stat()
    require(sys.platform=='linux' and os.geteuid()==0 and (info.st_dev,info.st_ino)==(29,18358) and
        Path('/proc/sys/kernel/random/boot_id').read_text().strip()==BOOT and
        Path('/proc/self/cgroup').read_text().splitlines()==['0::'+str(LEAF).removeprefix('/sys/fs/cgroup')] and
        (LEAF/'memory.max').read_text().strip()==str(64*GIB) and
        (LEAF/'memory.swap.max').read_text().strip()=='0' and
        (LEAF/'cpuset.cpus.effective').read_text().strip()=='0-31' and
        int((LEAF/'memory.current').read_text())<60*GIB and
        resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024<512*1024**2,
        'exact current64GiB zero-swap cgroup and bounded test RSS required')
    for parent in (LEAF,*[p for p in LEAF.parents if p.is_relative_to('/sys/fs/cgroup')]):
        path=parent/'memory.events'
        if path.exists():
            events=dict(line.split() for line in path.read_text().splitlines())
            require(all(int(events.get(key,'0'))==0 for key in ('oom','oom_kill','oom_group_kill')),
                'ancestor OOM invalidates the copy-policy test')


def opened(path):
    """Return actual bounded source bytes and full7 without following aliases."""
    require(path.is_absolute() and path.resolve(strict=True)==path,'physical test source required')
    descriptor=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_CLOEXEC|os.O_NONBLOCK)
    try:
        first=os.fstat(descriptor)
        require(stat.S_ISREG(first.st_mode) and 0<first.st_size<=1024**2,'bounded ordinary test source required')
        body=bytearray()
        while True:
            guard();part=os.read(descriptor,65536)
            if not part:break
            body.extend(part);require(len(body)<=1024**2,'source grew')
        last,named=os.fstat(descriptor),path.stat(follow_symlinks=False)
        require(len(body)==first.st_size and all(getattr(first,key)==getattr(last,key)==getattr(named,key)
            for key in ('st_dev','st_ino','st_size','st_mode','st_mtime_ns','st_ctime_ns','st_nlink')),'test source changed')
        return bytes(body),{'path':str(path),'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest(),
            'device':first.st_dev,'inode':first.st_ino,'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns}
    finally:os.close(descriptor)


class CopyPolicy(unittest.TestCase):
    def setUp(self):
        guard();self.home=OUT/self.id().rsplit('.',1)[-1];self.home.mkdir(mode=0o700)
        self.source=self.home/'source';self.source.mkdir(mode=0o700)
        self.destination=self.home/'copy';self.ledger=self.home/'copy.jsonl'

    def tearDown(self):guard()

    def copy(self):
        return MODULE.stream_copy(self.source,self.destination,self.ledger,guard,max_bytes=16*1024**2)

    def test_complete_regular_empty_and_relative_alias_copy(self):
        payload=b'bounded nonaligned whole-file reflink payload\n'*60000
        (self.source/'data').write_bytes(payload);(self.source/'data').chmod(0o640)
        (self.source/'empty').write_bytes(b'');os.symlink('data',self.source/'alias')
        result=self.copy()
        self.assertEqual((result['files'],result['symlinks'],result['file_bytes']),(2,1,len(payload)))
        self.assertEqual(result['FICLONE_successful_files'],1);self.assertTrue(result['reflinks_used'])
        self.assertFalse(result['hardlinks_used']);self.assertEqual((self.destination/'data').read_bytes(),payload)
        self.assertEqual((self.destination/'empty').stat().st_size,0)
        self.assertEqual(os.readlink(self.destination/'alias'),'data')
        self.assertEqual((self.destination/'data').stat().st_mode & 0o777,0o640)
        rows=[json.loads(line) for line in self.ledger.read_bytes().splitlines()]
        files=[row for row in rows if row['kind']=='file'];self.assertEqual(len(files),2)
        for row in files:
            self.assertNotEqual((row['source_current']['device'],row['source_current']['inode']),
                (row['copy_actual']['device'],row['copy_actual']['inode']))
            self.assertEqual(row['source_current']['sha256'],row['copy_actual']['sha256'])

    def test_clone_mutation_is_private_to_the_new_inode(self):
        payload=b'original immutable input\n'*50000;(self.source/'data').write_bytes(payload)
        source_info=(self.source/'data').stat();self.copy()
        self.assertNotEqual(source_info.st_ino,(self.destination/'data').stat().st_ino)
        with (self.destination/'data').open('r+b') as stream:
            stream.write(b'changed clone');stream.flush();os.fsync(stream.fileno())
        self.assertEqual((self.source/'data').read_bytes(),payload)
        self.assertNotEqual((self.destination/'data').read_bytes(),payload)
        now=(self.source/'data').stat()
        self.assertEqual((source_info.st_ino,source_info.st_size,source_info.st_mtime_ns,source_info.st_ctime_ns),
            (now.st_ino,now.st_size,now.st_mtime_ns,now.st_ctime_ns))
        self.assertEqual(now.st_nlink,1);self.assertEqual((self.destination/'data').stat().st_nlink,1)

    def test_hardlinked_source_is_refused(self):
        (self.source/'data').write_bytes(b'one inode with two names');os.link(self.source/'data',self.source/'other')
        with self.assertRaisesRegex(ValueError,'ordinary source file with one name'):self.copy()

    def test_absolute_symlink_is_refused(self):
        os.symlink('/etc/passwd',self.source/'alias')
        with self.assertRaisesRegex(ValueError,'bounded relative source aliases'):self.copy()

    def test_escaping_relative_symlink_is_refused(self):
        os.symlink('../outside',self.source/'alias')
        with self.assertRaisesRegex(ValueError,'alias escapes SDK root'):self.copy()


def main():
    global MODULE,OUT
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    parser.add_argument('--helper',type=Path,required=True);parser.add_argument('--helper-sha256',required=True)
    parser.add_argument('--out',type=Path,required=True);args=parser.parse_args();guard()
    body,pin=opened(args.helper);require(pin['sha256']==args.helper_sha256,'complete reviewed COPY helper SHA required')
    MODULE=types.ModuleType('actual_cp310_small_copy_policy');MODULE.__file__=str(args.helper)
    sys.modules[MODULE.__name__]=MODULE;exec(compile(body,str(args.helper),'exec'),MODULE.__dict__)
    OUT=args.out;artifact=Path('/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1')
    require(OUT.is_absolute() and OUT.parent==artifact and artifact.stat().st_dev==81 and
        not OUT.exists() and not OUT.is_symlink(),'fresh fixture root on the exact owned Btrfs mount required')
    OUT.mkdir(mode=0o700)
    result=unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(CopyPolicy))
    require(opened(args.helper)[1]==pin,'COPY helper changed during tests');guard()
    report={'kind':'cp310-cow-copy-small-real-filesystem-regressions','status':'PASS' if result.wasSuccessful() and
        result.testsRun==5 and not result.skipped else 'FAIL','tests_run':result.testsRun,'helper':pin,
        'Runtime_cases_executed':0,'CP310_executed':False,'native_ROS_executed':False,
        'scope':'SMALL_BTRFS_FICLONE_AND_ALIAS_POLICY_ONLY_NOT_COMPLETE_VIEW_OR_ROS_QUALIFICATION'}
    with (OUT/'report.json').open('x') as stream:
        json.dump(report,stream,indent=2);stream.write('\n');stream.flush();os.fsync(stream.fileno())
    print(json.dumps({'status':report['status'],'report':str(OUT/'report.json')}),flush=True)
    return 0 if report['status']=='PASS' else 1


if __name__=='__main__':raise SystemExit(main())
