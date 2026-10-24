#!/usr/bin/env python3
"""Generate inert finite codec inputs, never an expected acceptance decision.

The selected C++ Host decoder supplies the independent expected output. No ROS
transport, permission grant or hardware/performance qualification occurs here.
Run under the external SSH Linux process/resource owner.
"""
from pathlib import Path
import argparse, hashlib, json, random, struct


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project',type=Path,required=True)
    parser.add_argument('--corpus',type=Path,required=True)
    args=parser.parse_args(); project=args.project.resolve(strict=True)
    directory=project/'vectors';directory.mkdir();cases=[]
    def pack(sequence=0,seconds=2,nanoseconds=3,parent=b'map',child=b'base_link',values=(1.25,-2.5,3.,0.,0.,0.,1.)):
        return struct.pack('<III',sequence,seconds,nanoseconds)+struct.pack('<I',len(parent))+parent+struct.pack('<I',len(child))+child+struct.pack('<7d',*values)
    def add(label,body,backends=(1,2)):
        assert len(body)<=587
        for backend in backends:cases.append({'label':label,'backend':backend,'hex':body.hex()})
    base=pack();assert len(base)==88
    add('baseline',base)
    add('signed-zero-and-quaternion-sign',pack(values=(-0.,0.,0.,0.,0.,0.,-1.)))
    add('last-u32-sequence',pack(sequence=0xffffffff))
    add('last-timestamp',pack(seconds=0x7fffffff,nanoseconds=999999999))
    add('maximum-complete-extent',pack(parent=b'a'*255,child=b'b'*255))
    add('minimum-complete-extent',pack(parent=b'a',child=b'b'))
    add('valid-nested-frame',pack(parent=b'map2/_camera0',child=b'_base0'))
    add('within-baseline-norm-tolerance',pack(values=(1.,2.,3.,0.,0.,0.,1.00025)))
    add('finite-translation-is-not-a-policy-bound',pack(values=(1e308,-1e308,0.,0.,0.,0.,1.)))
    add('nontrivial-unit-quaternion',pack(values=(1.,2.,3.,.6,0.,0.,.8)))
    for size in range(len(base)):add('truncated-'+str(size),base[:size])
    add('trailing-byte',base+b'\0')
    add('extent77',bytes(77));add('extent587',bytes(587))
    for offset in (12,19):
        for length in (0,1,255,0xffffffff):
            b=bytearray(base);b[offset:offset+4]=struct.pack('<I',length);add('forged-length-'+str(offset)+'-'+str(length),bytes(b))
    for parent in (b'',b'/map',b'map/',b'map//base',b'1map',b'map/1base',b'map-base',b'map\0',b'\xff',b'a'*256):
        add('bad-parent-'+parent.hex(),pack(parent=parent))
        add('bad-child-'+parent.hex(),pack(child=parent))
    add('equal-frame-pair',pack(child=b'map'))
    for seconds in (0x80000000,0xffffffff):add('invalid-seconds-'+str(seconds),pack(seconds=seconds))
    for nanos in (1000000000,0xffffffff):add('invalid-nanos-'+str(nanos),pack(nanoseconds=nanos))
    for slot in range(7):
        for bits in (0x7ff8000000000000,0x7ff0000000000000,0xfff0000000000000):
            b=bytearray(base);b[32+8*slot:40+8*slot]=struct.pack('<Q',bits);add('nonfinite-'+str(slot)+'-'+hex(bits),bytes(b))
    for q in ((0.,0.,0.,0.),(0.,0.,0.,2.),(1e308,0.,0.,0.),(0.,0.,0.,1.001)):
        add('bad-quaternion-'+repr(q),pack(values=(0.,0.,0.,*q)))
    add('invalid-backend0',base,(0,));add('invalid-backend3',base,(3,))
    rng=random.Random(0x545246)
    for index in range(384):
        b=bytearray(base);offset=rng.randrange(len(b));b[offset]^=1<<rng.randrange(8)
        add('single-bit-mutation-'+str(index),bytes(b),(1+index%2,))
    assert 0<len(cases)<=4096 and not args.corpus.exists()
    args.corpus.write_text(''.join(str(c['backend'])+' '+c['hex']+'\n'for c in cases))
    (directory/'moon.pkg').write_text('')
    body='pub fn cases() -> Array[(Int,Bytes)] {\n  [\n'
    for case in cases:
        raw=bytes.fromhex(case['hex']);literal=''.join('\\x'+format(b,'02X')for b in raw)
        body+='    ('+str(case['backend'])+', b"'+literal+'"),\n'
    body+='  ]\n}\n';(directory/'data.mbt').write_text(body)
    report={'kind':'INERT_CORPUS_NOT_EXPECTED_DECISIONS','cases':cases,'case_count':len(cases),'corpus_sha256':hashlib.sha256(args.corpus.read_bytes()).hexdigest(),'generated_source_sha256':hashlib.sha256(body.encode()).hexdigest()}
    args.corpus.with_suffix('.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k]for k in ['kind','case_count','corpus_sha256','generated_source_sha256']}))


if __name__=='__main__':main()
