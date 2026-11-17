#!/usr/bin/env python3
"""Validate closed driver configuration without handshake, guest or native effects."""
import argparse,json,os,socket,subprocess,time
from pathlib import Path
from run_scene_vm import verify_gate,sha,GATE
from run_driver_attachment_vm import NAMES,IDS,DIGEST

def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--runtime',type=Path,required=True);parser.add_argument('--output',type=Path,required=True);a=parser.parse_args()
 verify_gate();a.output.mkdir(parents=True,exist_ok=False);report=dict(complete=False,runtime_sha256=sha(a.runtime),runner_sha256=sha(__file__),cgroup=str(GATE),events_before=(GATE/'memory.events').read_text(),cases=[])
 scenarios=['valid','missing-budget','zero-budget','missing-schema','duplicate-schema','unknown-schema','management-schema','unknown-module','unknown-alias','zero-identity','bad-hex','zero-contract','large-body','zero-duration','large-depth','bad-pid','bad-fd','stream','no-credentials','retired-import','retired-launcher']
 try:
  for mode in scenarios:
   left,right=socket.socketpair(socket.AF_UNIX,socket.SOCK_STREAM if mode=='stream' else socket.SOCK_SEQPACKET)
   try:
    if mode!='no-credentials':left.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
    flags=['--ros-scene-create','robot','--ros-scene-bind','robot','main']
    for name in ['capability.open']+NAMES:flags+=['--ros-scene-interface','robot',name,'allow']
    flags+=['--ros-scene-grant','robot','joint','driver','arm','registered-driver',','.join(NAMES)]
    budget=['--ros-scene-driver-budget','robot','0' if mode=='zero-budget' else '1','65536']
    channel=['--ros-scene-driver-kernel-channel','robot','joint','main',str(left.fileno()),str(os.getpid()),str(os.getuid()),str(os.getgid()),*[x.hex() for x in IDS],DIGEST.hex(),'32','32','60000000000',str(time.monotonic_ns()+60000000000)]
    schema=['--ros-scene-driver-schema','robot','joint','parameter.exists','64','64','8']
    if mode=='unknown-module':channel[3]='other'
    if mode=='unknown-alias':channel[2]='other'
    if mode=='zero-identity':channel[8]='0'*32
    if mode=='bad-hex':channel[8]='q'*32
    if mode=='zero-contract':channel[13]='0'*64
    if mode=='large-body':channel[14]='65537'
    if mode=='zero-duration':channel[16]='0'
    if mode=='bad-pid':channel[5]='0'
    if mode=='bad-fd':channel[4]='2147483647'
    if mode=='unknown-schema':schema[3]='unregistered.operation'
    if mode=='management-schema':schema[3]='driver.submit'
    if mode=='large-depth':schema[6]='65'
    if mode!='missing-budget':flags+=budget
    flags+=channel
    if mode!='missing-schema':flags+=schema
    if mode=='duplicate-schema':flags+=schema
    if mode=='retired-import':flags+=['--ros-scene-interface','robot','component.start','allow']
    if mode=='retired-launcher':flags+=['--ros-scene-component-budget','robot','1','65536']
    command=[str(a.runtime),*flags,'--ros-scene-check'];p=subprocess.run(command,pass_fds=(left.fileno(),),capture_output=True,timeout=10)
    (a.output/(mode+'.stderr')).write_bytes(p.stderr)
    passed=p.returncode==0 if mode=='valid' else p.returncode>0 and b'[crash]' not in p.stderr
    right.setblocking(False)
    try:packet=right.recv(1)
    except BlockingIOError:packet=b''
    assert not packet,'CLI validation sent protocol data'
    report['cases'].append(dict(name=mode,exit=p.returncode,passed=passed,argv=command));assert passed,(mode,p.stderr[-3000:]);print('PASS '+mode,flush=True)
   finally:left.close();right.close()
  report['complete']=True
 finally:report['events_after']=(GATE/'memory.events').read_text();(a.output/'results.json').write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
