#!/usr/bin/env python3
"""Consecutive real signed policy/code commits retain the MTE safety contract.

The first transition tightens exposed interfaces/FD capacity. The second expands
only numeric capacity; neither request manufactures a native resource. Four
DBG permission levels have no effect on production update authorization.
"""
import argparse,json,os,resource,shlex,socket,struct,subprocess
from pathlib import Path
from cryptography.hazmat.primitives.asymmetric.ed25519 import Ed25519PrivateKey
from cryptography.hazmat.primitives.serialization import Encoding,PublicFormat
from run_signed_update_boundaries import permit,request,module,sha,call_export

def policy(fd_limit):
 q=lambda *v:struct.pack('<'+'Q'*len(v),*v)
 # UWPOL6, 217 registered interface slots, 16 operation budgets, no quotas.
 return (b'UWPOL6\0\0'+q(1,0,fd_limit)+bytes(217)
         +(q(*([2**64-1]*6))+bytes(13))*16+q(1,1,1,1)
         +q(*([2**64-1]*3),0,0,0,0))

def declaration(fd_limit,generation):
 payload=policy(fd_limit)
 return (b'UUT2'+struct.pack('<III',1,1,len(payload))+sha(b'approved next policy')
         +struct.pack('<QQQ',generation,1,1)+payload)

def signed(key,facts,operation,target,transaction,base_generation,revocation,**kw):
 token=permit(key,facts,operation,target,transaction,**kw)
 body=bytearray(token[:-64]);struct.pack_into('<QQ',body,376,base_generation,revocation)
 return bytes(body)+key.sign(bytes(body))

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--vm',type=Path,required=True);p.add_argument('--runner',default='');p.add_argument('--out',required=True,type=Path);p.add_argument('--levels',default='full,read-only,limited,none');p.add_argument('--policies',default='sync:crash,sync:silent-log,software-async:crash,software-async:silent-log');a=p.parse_args();a.out.mkdir(parents=True,exist_ok=False);resource.setrlimit(resource.RLIMIT_CORE,(0,0));rows=[]
 key=Ed25519PrivateKey.from_private_bytes(bytes(range(1,33)));public=key.public_key().public_bytes(Encoding.Raw,PublicFormat.Raw)
 initial=module(False);path=a.out/'initial.wasm';path.write_bytes(initial)
 def check(label,name,condition,detail):
  rows.append(dict(scenario=label,name=name,passed=bool(condition),detail=detail));(a.out/'report.json').write_text(json.dumps(dict(passed=all(x['passed'] for x in rows),checks=len(rows),rows=rows),indent=2))
  if not condition:raise AssertionError(label+' '+name+': '+repr(detail))
 for level in a.levels.split(','):
  for execution in a.policies.split(','):
   mode,action=execution.split(':');label=level+'-'+mode+'-'+action;host,guest=socket.socketpair(socket.AF_UNIX,socket.SOCK_STREAM);guest.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1);host.settimeout(20)
   args=shlex.split(a.runner)+[str(a.vm.resolve()),'-Raot','-Rct','0','--runtime-llvm-jit-cache-path','disable','--debug-permissions',level,'--runtime-memtag-backend','hardware' if mode.startswith('hardware-') else 'software','--runtime-memtag-mode','async' if mode.endswith('async') else 'sync','--runtime-memtag-action',action,*(['err'] if action=='silent-log' else []),'--update-control-fd',str(guest.fileno()),'--update-public-key',public.hex(),'--update-contract',sha(b'test contract').hex(),'--update-scope',sha(b'test scope').hex(),'--update-call-export','value','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--run',str(path.resolve())]
   with (a.out/(label+'.stderr')).open('wb') as error,(a.out/(label+'.stdout')).open('wb') as output:
    child=subprocess.Popen(args,pass_fds=(guest.fileno(),),stdout=output,stderr=error);guest.close()
    try:
     state=request(host,0);check(label,'awaiting signed launch',state['status']==0 and state['state']==8 and not state['active'],state)
     launch=permit(key,state,4,sha(initial),sha(label.encode()+b'launch'));r=request(host,9,launch);check(label,'launch',r['status']==0 and r['active'],r)
     for index in (1,2):
      facts=request(host,0);target=module(index==1);decl=declaration(2+index,index+1);tx=sha((label+str(index)).encode());revocation=0 if index==1 else 1
      token=signed(key,facts,1,sha(target),tx,index,revocation,declaration=decl)
      r=request(host,7,token+struct.pack('<I',len(decl))+decl+target);check(label,f'prepare {index}',r['status']==0 and r['state']==3,r)
      ready=request(host,2);check(label,f'ready {index}',ready['status']==0 and ready['state']==4,ready)
      token=signed(key,facts,2,sha(target),tx,index,revocation,declaration=decl,prepared=bytes.fromhex(ready['prepared']),ready=bytes.fromhex(ready['ready']))
      r=request(host,3,token);check(label,f'commit {index}',r['status']==0 and r['active'] and r['epoch']==index+1,r)
      value=call_export(host,struct.pack('<III',1,0,4));check(label,f'call after policy {index}',value['status']==0 and value['result']==('09000000' if index==1 else '07000000'),value)
     r=request(host,6);check(label,'authorized stop',r['status']==0,r);child.wait(timeout=20);check(label,'exit',child.returncode==0,child.returncode)
    finally:
     host.close()
     if child.poll() is None:child.terminate()
     try:child.wait(timeout=10)
     except subprocess.TimeoutExpired:child.kill();child.wait()
 print(json.dumps(dict(passed=True,scenarios=len(a.levels.split(','))*len(a.policies.split(',')),checks=len(rows))))
if __name__=='__main__':main()
