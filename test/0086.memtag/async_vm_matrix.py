#!/usr/bin/env python3
"""Actual Wasm execution; signals and timeouts never satisfy this oracle."""
import argparse,json,subprocess,shlex,hashlib,re
from pathlib import Path
import vm_matrix as m
import threaded_matrix as threaded
import core3_linear_matrix as linear
from vm_diagnostics import fault_matches

def expressions(raw):
 # These fixtures declare only numeric locals. Return expression-relative
 # byte spans, exactly the origin coordinate used by both compiler backends.
 at=8
 while at<len(raw):
  kind=raw[at];size,start=threaded.leb(raw,at+1);end=start+size
  if kind==10:
   count,pos=threaded.leb(raw,start);result=[]
   for _ in range(count):
    length,pos=threaded.leb(raw,pos);body_end=pos+length
    groups,pos=threaded.leb(raw,pos)
    for _ in range(groups):
     _,pos=threaded.leb(raw,pos);assert raw[pos] in (0x7f,0x7e,0x7d,0x7c,0x7b);pos+=1
    result.append(raw[pos:body_end]);pos=body_end
   assert pos==end;return result
  at=end
 raise AssertionError('fixture has no code section')

def expected_origin(name,raw):
 # Match one complete generator-owned operand sequence, never an arbitrary
 # opcode byte inside an immediate. This fixes the exact failing instruction
 # before execution and prevents a next-instruction/stale origin from passing.
 wide=name.startswith('64-');p=m.Program(wide);function=0;local=0;memory=0;granule=32
 if name.startswith('multi-'):
  _,first,second=name.split('-');bits=int(second);memory=1
  prefix=linear.pointer(32,bits,1,4)+m.c(77);instruction=linear.memory(0x36,1)
 elif name=='64-upper-tag':prefix=p.tagged(32,0xb3)+m.c(77);instruction=m.mem(0x36)
 elif name in ('thread-wrong-tag','thread-tag-zero'):
  function=3;local=1;granule=128
  prefix=(m.c(128) if name=='thread-tag-zero' else p.tagged(128,4))+m.c(77);instruction=m.mem(0x36)
 elif name=='thread-before-infinite-wait':
  function=1;prefix=p.tagged(32,4)+m.c(77);instruction=m.mem(0x36)
 else:
  kind=name.split('-',1)[1]
  if kind=='scalar':prefix=p.tagged(0)+m.c(17,64);instruction=m.mem(0x37,15);granule=16
  elif kind=='simd':prefix=p.tagged(0)+b'\xfd\x0c'+bytes(16);instruction=m.simd(11,15);granule=16
  elif kind=='fill':prefix=p.tagged(0)+m.c(55)+p.n(128);instruction=m.fc(11);granule=64
  elif kind=='copy':prefix=p.tagged(128)+p.tagged(0)+p.n(128);instruction=m.fc(10)+m.u(0);granule=64
  elif kind=='atomic':prefix=p.tagged(64)+m.c(55);instruction=b'\xfe\x17\x02\0';granule=64
  else:
   assert kind in ('spin','host-exit','grow')
   function=1 if kind=='host-exit' else 0
   prefix=p.tagged(32,4)+m.c(77);instruction=m.mem(0x36)
 expression=expressions(raw)[local];anchor=prefix+instruction;assert expression.count(anchor)==1
 # These entry fixtures execute the main module (runtime module id zero),
 # including children which retain that main source and import only memory.
 return dict(module=0,function=function,offset=expression.index(anchor)+len(prefix),memory=memory,granule=granule)

def source_matches(output,expected):
 fields={}
 for label in ('Wasm module','Wasm function','Wasm byte offset','Memory index','Granule'):
  values=re.findall(r'^  '+re.escape(label)+r': ([0-9]+)$',output,re.MULTILINE)
  if len(values)!=1:return False
  fields[label]=int(values[0])
 return (fields['Wasm module']==expected['module'] and fields['Wasm function']==expected['function'] and fields['Wasm byte offset']==expected['offset'] and
         fields['Memory index']==expected['memory'] and fields['Granule']==expected['granule'])

def with_exit(code,wide):
 body=b'\0'+code+m.c(0)+b'\x10\0\x0b'
 return b'\0asm\x01\0\0\0'+m.sec(1,bytes.fromhex('02 60 00 00 60 01 7f 00'))+m.sec(2,b'\x01\x16wasi_snapshot_preview1\x09proc_exit\0\x01')+m.sec(3,b'\x01\0')+m.sec(5,b'\x01'+m.u(5 if wide else 1)+b'\x01\x02')+m.sec(7,b'\x01\x06_start\0\x01')+m.sec(10,b'\x01'+m.u(len(body))+body)
def main():
 p=argparse.ArgumentParser();p.add_argument('--vm',required=True);p.add_argument('--engine',required=True)
 p.add_argument('--out',type=Path,required=True);p.add_argument('--runner',default='');p.add_argument('--modes',default='sync,software-async')
 p.add_argument('--threads',action='store_true');a=p.parse_args();a.out.mkdir(parents=True,exist_ok=False)
 cases=[]
 for wide in (False,True):
  for kind in ('scalar','simd','fill','copy','atomic','bounds'):
   cases.append((f'{32 if not wide else 64}-{kind}',m.denial(wide,kind),121 if kind=='bounds' else 122,False))
  p=m.Program(wide);p.add(p.tags());p.add(p.tagged(32,4)+m.c(77)+m.mem(0x36))
  cases.append((f'{p.bits}-spin',m.module(p.code+b'\x03\x40\x0c\0\x0b',wide),122,False))
  cases.append((f'{p.bits}-valid',m.scalars(wide),0,False))
  cases.append((f'{p.bits}-host-exit',with_exit(p.code,wide),122,False))
  cases.append((f'{p.bits}-grow',m.module(p.code+p.n(1)+b'\x40\0\x1a',wide),122,False))
 p=m.Program(True);p.add(p.tags(0,512,0xa3));p.add(p.tagged(32,0xb3)+m.c(77)+m.mem(0x36))
 cases.append(('64-upper-tag',m.module(p.code,True),122,False))
 for widths in ((32,64),(64,32)):
  code=linear.mark(0,widths[0])+linear.mark(1,widths[1])+linear.pointer(32,widths[1],1,4)+m.c(77)+linear.memory(0x36,1)
  cases.append((f'multi-{widths[0]}-{widths[1]}',linear.module(code,widths),122,False))
 if a.threads:
  for kind in ('good','wrong-tag','tag-zero','grow'):
   cases.append(('thread-'+kind,threaded.spawned(kind),122 if kind in ('wrong-tag','tag-zero') else 0,True))
  p=m.Program();p.add(p.tags());p.add(p.tagged(32,4)+m.c(77)+m.mem(0x36))
  p.add(m.c(4096)+m.c(0)+m.c(-1,64)+b'\xfe\x01\x02\0\x1a')
  cases.append(('thread-before-infinite-wait',threaded.shared_module(m.module(p.code)),122,True))
 owner=a.out/'owner.wasm';owner.write_bytes(b'\0asm\x01\0\0\0'+m.sec(5,b'\x01\x03\x01\x02')+m.sec(7,b'\x01\x06memory\x02\0'))
 rows=[]
 for name,raw,status,threads in cases:
  path=a.out/(name+'.wasm');path.write_bytes(raw)
  origin=expected_origin(name,raw) if status==122 else None
  for mode in a.modes.split(','):
   for workers in (1,):
    cmd=shlex.split(a.runner)+[a.vm,'-R'+a.engine,'--runtime-compile-threads','0','-WFE-threads','--wasm-feature-enable-memory64','--wasm-feature-enable-multi-memory',*m.mode_flags(mode),]
    if a.engine=='aot':cmd+=['--runtime-llvm-jit-cache-path','disable']
    if name.endswith('host-exit'):
     cmd+=['--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-interface','main','proc_exit','allow','--wasip1-single-grant','main','proc-exit']
    if threads:
     cmd+=['--wasi-threads','3','3','1048576','16777216','67108864','131072','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-interface','main','proc_exit','allow','--wasip1-single-grant','main','proc-exit','--wasip1-single-interface','main','thread-spawn','allow','--wasip1-single-grant','main','thread-spawn','--wasm-preload-library',str(owner),'env']
     if a.engine=='aot':cmd+=['--runtime-llvm-jit-policy','debug']
    cmd+=['--run',str(path)]
    try:r=subprocess.run(cmd,capture_output=True,timeout=30);code=r.returncode;output=(r.stdout+r.stderr).decode(errors='replace')
    except subprocess.TimeoutExpired as e:code='timeout';output=str(e)
    passed=code==0 if status==0 else fault_matches(code,output,status,'out of bounds' if status==121 else 'memory tag mismatch')
    if passed and status==122 and mode.endswith('async'):passed='Delivery: asynchronous' in output
    if passed and status==122 and mode=='software-async':passed=source_matches(output,origin)
    # The finite threaded interpreter has a persistent source map even in
    # synchronous mode. It must not publish the absolute file offset as an
    # expression-relative byte coordinate. Other synchronous backends may
    # explicitly report an unavailable position and are not invented here.
    if passed and status==122 and mode=='sync' and threads and a.engine=='int':passed=source_matches(output,origin)
    if passed and status==122 and name.startswith('multi-') and not mode.startswith('hardware-'):passed='Memory index: 1' in output and 'Granule: 32' in output
    (a.out/(f'{name}-{mode}-{workers}.log')).write_text(output)
    rows.append(dict(name=name,mode=mode,workers=workers,command=cmd,sha256=hashlib.sha256(raw).hexdigest(),exit=code,expected=status,expected_origin=origin,passed=passed))
    (a.out/'report.json').write_text(json.dumps(rows,indent=2)+'\n');print(name,mode,workers,code,passed,flush=True)
 raise SystemExit(any(not x['passed'] for x in rows))
if __name__=='__main__':main()
