#!/usr/bin/env python3
"""Malformed malicious-Wasm publications with active native absence witnesses."""
import argparse
import json
import os
from pathlib import Path
import struct
import subprocess
import signal
import run_network as network


def guest(version,payload,error):
    escaped=''.join('\\'+format(byte,'02x') for byte in payload)
    return f'''(module
 (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros{version}_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory 1) (data (i32.const 0) "pub") (data (i32.const 8192) "{escaped}")
 (func (export "_start")
  (if (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)) (then (call $exit (i32.const 201))))
  (if (i32.ne (call $publish (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const {len(payload)})) (i32.const {error})) (then (call $exit (i32.const 202))))))
'''


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--workspace',required=True);p.add_argument('--source',default=str(Path(__file__).resolve().parents[5]));p.add_argument('--uwvm',required=True)
    p.add_argument('--llvm-libs',default=os.environ.get('LD_LIBRARY_PATH',''));p.add_argument('--wat2wasm',default='wat2wasm');args=p.parse_args()
    runner=network.Runner(args);original=network.policy;report={'status':'FAIL','results':runner.results}
    def terminated(signum,frame):
        raise RuntimeError('FAIL bounded payload runner interrupted')
    signal.signal(signal.SIGTERM,terminated)
    fields=struct.pack('<6d',1.25,-2.,3.5,.1,.2,.3)
    image=struct.pack('<III',0,123,456)+struct.pack('<I',12)+b'camera_frame'+struct.pack('<II',2,2)+struct.pack('<I',4)+b'rgb8'+b'\0'+struct.pack('<II',6,10)+bytes(range(12))
    cases=[('String','malformed-length',b'\xff\0\0\0hello',22),
           ('String','invalid-utf8',struct.pack('<I',2)+b'\xc0\xaf',22),
           ('Bool','noncanonical-bool',b'\2',22),
           ('Twist','truncated-fields',fields[:-1],22),
           ('Twist','nan',struct.pack('<6d',float('nan'),0,0,0,0,0),22),
           ('Twist','infinity',struct.pack('<6d',float('inf'),0,0,0,0,0),22),
           ('Twist','explicit-bound',fields,13),
           ('Image','malformed-image-length',image,22),
           ('String','oversized',struct.pack('<I',4093)+b'x'*4093,90)]
    try:
        report['cgroup_before']=network.cgroup_snapshot();runner.prepare({1,2})
        for version in (1,2):
            for kind,label,payload,error in cases:
                name=f'ros{version}-payload-'+label
                fixture=runner.run/(name+'.wat');fixture.write_text(guest(version,payload,error));wasm=fixture.with_suffix('.wasm')
                subprocess.run([args.wat2wasm,'--enable-all',str(fixture),'-o',str(wasm)],check=True,timeout=15)
                def configured(v,role,message_type):
                    connection,caps=original(v,role,message_type)
                    if label=='explicit-bound':caps[0]['limits'].update({'validator':'twist-bounds','twist.linear-max-abs':'1','twist.angular-max-abs':'1'})
                    return connection,caps
                network.policy=configured
                runner.case(name,str(wasm),{version:['publish']},[(version,'--absent','/uwvm_ros_test/output')],message_type=kind)
                runner.results[-1].update(expected_error=error,payload_bytes=len(payload),native_effect='active native subscriber observed0 messages; no native publisher registration')
                network.policy=original
        report['status']='PASS'
    except (RuntimeError,OSError,subprocess.SubprocessError) as error:
        report.update(status='BLOCKED' if str(error).startswith('BLOCKED') else 'FAIL',error=str(error));print(report['status'],error,flush=True)
    finally:
        network.policy=original;runner.cleanup();report.update(artifacts=runner.artifacts,cgroup_after=network.cgroup_snapshot())
        def events(snapshot):return dict(line.split() for line in snapshot['memory.events'].splitlines())
        if 'cgroup_before' in report and any(events(report['cgroup_before']).get(k)!=events(report['cgroup_after']).get(k) for k in ('oom','oom_kill')):report.update(status='FAIL',error='shared target OOM counter increased')
        (runner.run/'payload-security.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],runner.run/'payload-security.json',flush=True)
    return 0 if report['status']=='PASS' else 77 if report['status']=='BLOCKED' else 1


if __name__=='__main__':raise SystemExit(main())
