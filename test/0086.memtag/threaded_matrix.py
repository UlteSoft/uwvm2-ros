#!/usr/bin/env python3
"""Actual shared CLI execution; no synthetic ChildLease or admission provider."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import vm_matrix as m

def leb(data,at):
 value=0;shift=0
 while True:
  b=data[at];at+=1;value|=(b&127)<<shift
  if not b&128:return value,at
  shift+=7

def shared_module(original):
 # Input comes only from the single-function regression builders. Preserve its
 # validated code/data exactly; add a genuine imported owner and thread entry.
 sections=[];at=8
 while at<len(original):
  kind=original[at];size,start=leb(original,at+1);sections.append((kind,original[start:start+size]));at=start+size
 result=original[:8]
 for kind,payload in sections:
  if kind==1:
   count,pos=leb(payload,0);assert count==1
   result+=m.sec(1,m.u(3)+payload[pos:]+bytes.fromhex('60 02 7f 7f 00 60 01 7f 00'))
   result+=m.sec(2,m.u(2)+b'\x03env\x06memory\x02\x03\x01\x02'+b'\x16wasi_snapshot_preview1\x09proc_exit\x00\x02')
  elif kind==3:
   assert payload==b'\x01\x00';result+=m.sec(3,b'\x02\x00\x01')
  elif kind==5:pass
  elif kind==7:
   result+=m.sec(7,b'\x03\x06_start\x00\x01\x11wasi_thread_start\x00\x02\x06memory\x02\x00')
  elif kind==10:
   count,pos=leb(payload,0);assert count==1
   result+=m.sec(10,m.u(2)+payload[pos:]+b'\x02\x00\x0b')
  else:result+=m.sec(kind,payload)
 return result

def waits():
 p=m.Program();p.add(p.tags());p.add(p.tagged(0)+m.c(7,64)+m.mem(0x37))
 for opcode,bits,width in [(1,32,4),(2,64,8)]:
  for expected,want in [(8,1),(7,2)]:
   p.eq(p.tagged(0)+m.c(expected,bits)+m.c(0,64)+b'\xfe'+m.u(opcode)+m.u(width.bit_length()-1)+b'\0',want)
 p.eq(p.tagged(0)+m.c(20)+b'\xfe\x00\x02\0',0)
 return m.module(p.code)
def growth():
 p=m.Program();p.add(p.tags())
 p.eq(m.c(1)+b'\x40\0',1);p.eq(b'\x3f\0',2)
 # Fresh suffix allocation tags are zero. The pre-existing tagged prefix is unchanged.
 p.add(m.c(65536)+m.c(91)+m.mem(0x36));p.eq(m.c(65536)+m.mem(0x28),91)
 p.add(p.tagged(0)+m.c(37)+m.mem(0x36));p.eq(p.tagged(0)+m.mem(0x28),37)
 return m.module(p.code)

def spawned(kind):
 p=m.Program();p.add(p.tags())
 # Completion synchronization uses tag-zero storage outside the tagged object.
 p.add(m.c(0)+b'\x10\x01'+m.c(0)+b'\x48\x04\x40\x00\x0b')
 p.add(b'\x02\x40\x03\x40'+m.c(4096)+b'\xfe\x10\x02\0\x0d\x01')
 p.add(m.c(4096)+m.c(0)+m.c(-1,64)+b'\xfe\x01\x02\0\x1a\x0c\0\x0b\x0b')
 p.eq(p.tagged(128)+m.mem(0x28),77)
 if kind=='grow':p.eq(p.tagged(65536,5)+m.mem(0x28),119)
 child=m.Program()
 if kind=='tag-zero':pointer=m.c(128)
 else:pointer=child.tagged(128,4 if kind=='wrong-tag' else 3)
 child.add(pointer+m.c(77)+m.mem(0x36))
 if kind=='grow':
  child.eq(m.c(1)+b'\x40\0',1)
  child.add(child.tags(65536,16,5))
  child.add(child.tagged(65536,5)+m.c(119)+m.mem(0x36))
 child.add(m.c(4096)+m.c(1)+b'\xfe\x17\x02\0')
 child.add(m.c(4096)+m.c(1)+b'\xfe\x00\x02\0\x1a')
 result=b'\0asm\x01\0\0\0'
 result+=m.sec(1,bytes.fromhex('04 60 00 00 60 02 7f 7f 00 60 01 7f 00 60 01 7f 01 7f'))
 result+=m.sec(2,m.u(3)+b'\x03env\x06memory\x02\x03\x01\x02'+
  b'\x16wasi_snapshot_preview1\x09proc_exit\x00\x02'+b'\x04wasi\x0cthread-spawn\x00\x03')
 result+=m.sec(3,b'\x02\x00\x01')
 result+=m.sec(7,b'\x03\x06_start\x00\x02\x11wasi_thread_start\x00\x03\x06memory\x02\0')
 bodies=[b'\0'+body+b'\x0b' for body in [p.code,child.code]]
 result+=m.sec(10,m.u(2)+b''.join(m.u(len(body))+body for body in bodies))
 return result

def main():
 parser=argparse.ArgumentParser();parser.add_argument('--vm',type=Path);parser.add_argument('--out',type=Path,required=True)
 parser.add_argument('--engines',default='int,aot');parser.add_argument('--code-budget',type=int,default=67108864)
 parser.add_argument('--generate-only',action='store_true');args=parser.parse_args()
 if args.code_budget<0:parser.error('--code-budget must not be negative')
 args.out.mkdir(parents=True,exist_ok=True)
 # The preloaded module is the actual shared memory owner for every child.
 owner=args.out/'owner.wasm';owner.write_bytes(b'\0asm\x01\0\0\0'+m.sec(5,b'\x01\x03\x01\x02')+m.sec(7,b'\x01\x06memory\x02\0'))
 manifest=[]
 for enabled in [False,True]:
  cases=[('markers',m.markers(False,enabled),'success'),('scalars',m.scalars(False),'success'),
   ('simd',m.simds(False),'success'),('bulk',m.bulks(False),'success'),('atomic',m.atomics(False),'success'),
   ('wait-notify',waits(),'success'),('growth',growth(),'success')]
  for kind in ['scalar','simd','fill','copy','atomic','bounds']:
   cases.append(('deny-'+kind,m.denial(False,kind),'bounds' if kind=='bounds' else 'memtag' if enabled else 'success'))
  for name,raw,expected in cases:
   name+='-'+str(int(enabled));raw=shared_module(raw);file=args.out/(name+'.wasm');file.write_bytes(raw)
   manifest.append(dict(name=name,file=str(file),enabled=enabled,expected=expected,sha256=hashlib.sha256(raw).hexdigest()))
 for enabled in [False,True]:
  for kind in ['good','wrong-tag','tag-zero','grow']:
   name='spawn-'+kind+'-'+str(int(enabled));raw=spawned(kind);file=args.out/(name+'.wasm');file.write_bytes(raw)
   expected='memtag' if enabled and kind in ['wrong-tag','tag-zero'] else 'success'
   manifest.append(dict(name=name,file=str(file),enabled=enabled,expected=expected,sha256=hashlib.sha256(raw).hexdigest()))
 (args.out/'manifest.json').write_text(json.dumps(manifest,indent=2))
 if args.generate_only:return
 if args.vm is None:parser.error('--vm is required without --generate-only')
 rows=[]
 for case in manifest:
  for engine in args.engines.split(','):
   command=[str(args.vm),'-R'+engine,'--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',
    '--runtime-llvm-jit-policy','debug','-WFE-threads','--wasi-threads','3','3','1048576','16777216',str(args.code_budget),'131072',
    '--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main',
    '--wasip1-single-interface','main','proc_exit','allow','--wasip1-single-grant','main','proc-exit',
    '--wasm-preload-library',str(owner),'env']
   if case['name'].startswith('spawn-'):
    command+=['--wasip1-single-interface','main','thread-spawn','allow','--wasip1-single-grant','main','thread-spawn']
   if not case['enabled']:command+=['--runtime-disable-memtag']
   command+=['--run',case['file']]
   completed=subprocess.run(command,capture_output=True,timeout=90)
   output=(completed.stdout+completed.stderr).decode('utf8','replace')
   (args.out/(case['name']+'-'+engine+'.log')).write_text(output)
   passed=completed.returncode==0 if case['expected']=='success' else completed.returncode==({'memtag':122,'bounds':121}[case['expected']]) and 'uwvm: [crash] ' in output
   if case['expected']=='memtag':passed=passed and 'memory tag mismatch' in output.lower()
   row=dict(name=case['name'],engine=engine,expected=case['expected'],exit=completed.returncode,passed=passed,command=command)
   rows.append(row);(args.out/'report.json').write_text(json.dumps(rows,indent=2))
   print(case['name'],engine,completed.returncode,passed,flush=True)
 raise SystemExit(any(not row['passed'] for row in rows))
if __name__=='__main__':main()
