#!/usr/bin/env python3
"""Actual shared CLI execution; no synthetic ChildLease or admission provider."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import vm_matrix as m
from vm_diagnostics import fault_matches

def leb(data,at):
 value=0;shift=0
 while True:
  b=data[at];at+=1;value|=(b&127)<<shift
  if not b&128:return value,at
  shift+=7

def shared_module(original,*,memory32_adm=4):
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
   result+=m.sec(2,m.u(2)+b'\x03env\x06memory\x02'+m.memory_limits(shared=True,memory32_adm=memory32_adm)+b'\x16wasi_snapshot_preview1\x09proc_exit\x00\x02')
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

def spawn_and_wait(p):
 # Completion synchronization uses tag-zero storage outside the tagged object.
 # wasi.thread-spawn may return a negative error before admitting a child,
 # including transient registry contention. Retry only that side-effect-free
 # failure, at most 64 times; zero is not a valid TID. A persistent refusal
 # still executes unreachable, which must fail an expected-MTE (122) test.
 p.add(m.c(64)+b'\x21\0\x02\x40\x03\x40')
 p.add(m.c(0)+b'\x10\x01'+m.c(0)+b'\x4a\x0d\x01')
 p.add(b'\x20\0'+m.c(1)+b'\x6b\x22\0\x45\x04\x40\x00\x0b')
 # A distinct zero-tag word and finite 1 ms timeout let the registry owner run.
 p.add(p.n(4100)+m.c(0)+m.c(1000000,64)+b'\xfe\x01\x02\0\x1a\x0c\0\x0b\x0b')
 p.add(b'\x02\x40\x03\x40'+p.n(4096)+b'\xfe\x10\x02\0\x0d\x01')
 p.add(p.n(4096)+m.c(0)+m.c(-1,64)+b'\xfe\x01\x02\0\x1a\x0c\0\x0b\x0b')

def worker_module(p,child,limits):
 child.add(child.n(4096)+m.c(1)+b'\xfe\x17\x02\0')
 child.add(child.n(4096)+m.c(1)+b'\xfe\x00\x02\0\x1a')
 result=b'\0asm\x01\0\0\0'
 result+=m.sec(1,bytes.fromhex('04 60 00 00 60 02 7f 7f 00 60 01 7f 00 60 01 7f 01 7f'))
 result+=m.sec(2,m.u(3)+b'\x03env\x06memory\x02'+limits+
  b'\x16wasi_snapshot_preview1\x09proc_exit\x00\x02'+b'\x04wasi\x0cthread-spawn\x00\x03')
 result+=m.sec(3,b'\x02\x00\x01')
 result+=m.sec(7,b'\x03\x06_start\x00\x02\x11wasi_thread_start\x00\x03\x06memory\x02\0')
 bodies=[b'\x01\x01\x7f'+p.code+b'\x0b',b'\0'+child.code+b'\x0b']
 return result+m.sec(10,m.u(2)+b''.join(m.u(len(body))+body for body in bodies))

def spawned(kind,*,memory32_adm=4):
 p=m.Program();p.add(p.tags());spawn_and_wait(p)
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
 return worker_module(p,child,m.memory_limits(shared=True,memory32_adm=memory32_adm))

def plain_bounds(wide,failure):
 # A weaker import aliases an owner with a two-page minimum. Exchange a
 # dynamic index so both workers visit the prefix and the newly grown suffix.
 main,child=m.Program(wide),m.Program(wide)
 read_index=main.n(8)+m.mem(0x29 if wide else 0x28)
 def publish(p,index):
  p.add(p.n(8)+p.n(index)+m.mem(0x37 if wide else 0x36))
 publish(main,2*65536-9)
 main.add(read_index+m.c(0x8162435465768798,64)+m.mem(0x37,1))
 spawn_and_wait(main)
 child.eq(read_index+m.mem(0x29,1),0x8162435465768798,64)
 child.eq(child.n(1)+b'\x40\0',2,child.bits)
 publish(child,3*65536-10)
 child.add(read_index+m.c(0xcbdcedfe8f90a1b2,64)+m.mem(0x37,1))
 main.eq(b'\x3f\0',3,main.bits)
 main.eq(read_index+m.mem(0x29,1),0xcbdcedfe8f90a1b2,64)
 if failure=='wrap':publish(main,-2)
 if failure:main.add(read_index+m.c(0,64)+m.mem(0x37,3))
 return worker_module(main,child,m.memory_limits(wide,True,tagged=False,minimum=1,maximum=3))

def shared_reader_pair():
 # The accessing module selects its own checks for the same tagged mmap.
 # Main first reads through a mismatching pointer, then calls a peer's
 # exported reader with that same pointer. A checked peer must fault there.
 header=bytes.fromhex('00 61 73 6d 01 00 00 00')
 limits=m.memory_limits(shared=True)
 peer=header+m.sec(1,bytes.fromhex('01 60 01 7f 01 7f'))
 peer+=m.sec(2,b'\x01\x03env\x06memory\x02'+limits)+m.sec(3,b'\x01\0')+m.sec(7,b'\x01\x04read\0\0')
 reader=b'\0\x20\0'+m.mem(0x28)+b'\x0b'
 peer+=m.sec(10,b'\x01'+m.u(len(reader))+reader)
 main=header+m.sec(1,bytes.fromhex('03 60 00 00 60 02 7f 7f 00 60 01 7f 01 7f'))
 main+=m.sec(2,b'\x02\x03env\x06memory\x02'+limits+b'\x04peer\x04read\0\x02')
 main+=m.sec(3,b'\x02\0\x01')
 main+=m.sec(7,b'\x03\x06_start\0\x01\x11wasi_thread_start\0\x02\x06memory\x02\0')
 p=m.Program()
 p.add(p.tags())
 p.add(p.tagged(16)+m.c(8193)+m.mem(0x36))
 p.eq(m.c(16)+m.mem(0x28),8193)
 p.eq(m.c(16)+b'\x10\0',8193)
 body=b'\0'+p.code+b'\x0b'
 main+=m.sec(10,b'\x02'+m.u(len(body))+body+b'\x02\0\x0b')
 return peer,main

def main():
 parser=argparse.ArgumentParser();parser.add_argument('--vm',type=Path);parser.add_argument('--out',type=Path,required=True)
 parser.add_argument('--engines',default='int,aot');parser.add_argument('--code-budget',type=int,default=67108864)
 parser.add_argument('--generate-only',action='store_true');args=parser.parse_args()
 if args.code_budget<0:parser.error('--code-budget must not be negative')
 args.out.mkdir(parents=True,exist_ok=True)
 # The preloaded module is the actual shared memory owner for every child.
 owner=args.out/'owner.wasm';owner.write_bytes(b'\0asm\x01\0\0\0'+m.sec(5,m.u(1)+m.memory_limits(shared=True))+m.sec(7,b'\x01\x06memory\x02\0'))
 manifest=[]
 for enabled in [False,True]:
  cases=[('markers',m.markers(False),'success'),('scalars',m.scalars(False),'success'),
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
 for wide in (False,True):
  bits=64 if wide else 32
  plain_owner=args.out/f'plain-owner-{bits}.wasm'
  plain_owner.write_bytes(b'\0asm\x01\0\0\0'+m.sec(5,b'\x01'+m.memory_limits(wide,True,tagged=False,minimum=2,maximum=3))+m.sec(7,b'\x01\x06memory\x02\0'))
  for failure in ('','cross','wrap'):
   name=f'spawn-plain-{bits}-'+(failure or 'grow')
   raw=plain_bounds(wide,failure);file=args.out/(name+'.wasm');file.write_bytes(raw)
   manifest.append(dict(name=name,file=str(file),owner=str(plain_owner),memory_bytes=3*65536,wide=wide,enabled=False,
    expected='bounds' if failure else 'success',sha256=hashlib.sha256(raw).hexdigest()))
 reader_module,reader_guest=shared_reader_pair()
 peer=args.out/'reader-peer.wasm';peer.write_bytes(reader_module)
 guest=args.out/'reader-main.wasm';guest.write_bytes(reader_guest)
 for checked_peer in [False,True]:
  manifest.append(dict(name='shared-reader-peer-'+str(int(checked_peer)),file=str(guest),peer=str(peer),
   enabled=False,checked_peer=checked_peer,expected='memtag' if checked_peer else 'success',sha256=hashlib.sha256(reader_guest).hexdigest()))
 (args.out/'manifest.json').write_text(json.dumps(manifest,indent=2))
 if args.generate_only:return
 if args.vm is None:parser.error('--vm is required without --generate-only')
 rows=[]
 for case in manifest:
  for engine in args.engines.split(','):
   command=[str(args.vm),'-R'+engine,'--runtime-compile-threads','0','--wasm-policy-global', 'parser.threads', 'on','--wasm-policy-global','runtime.memtag-backend','software','--wasi-threads','3','3','1048576','16777216',str(args.code_budget),str(case.get('memory_bytes',131072)),
    '--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main',
    '--wasip1-single-interface','main','proc_exit','allow','--wasip1-single-grant','main','proc-exit',
    '--wasm-preload-library',str(case.get('owner',owner)),'env']
   if case.get('wide'):command+=['--wasm-policy-global','parser.memory64','on']
   if engine=='aot':command+=['--runtime-llvm-jit-cache-path','disable','--wasm-policy-global', 'runtime.llvm.policy','debug']
   if case['name'].startswith('spawn-'):
    command+=['--wasithread-single-create','main','--wasithread-single-enable','main','--wasithread-single-interface','main','thread-spawn','allow']
   if not case['enabled']:command+=['--wasm-policy-global', 'runtime.memtag-checks', 'off']
   if 'peer' in case:command+=['--wasm-preload-library',case['peer'],'peer']
   if case.get('checked_peer'):command+=['--wasm-policy-single','peer','runtime.memtag-checks','on']
   command+=['--run',case['file']]
   completed=subprocess.run(command,capture_output=True,timeout=90)
   output=(completed.stdout+completed.stderr).decode('utf8','replace')
   (args.out/(case['name']+'-'+engine+'.log')).write_text(output)
   passed=completed.returncode==0 if case['expected']=='success' else fault_matches(completed.returncode,output,{'memtag':122,'bounds':121}[case['expected']],
    'memory tag mismatch' if case['expected']=='memtag' else 'out of bounds')
   row=dict(name=case['name'],engine=engine,expected=case['expected'],exit=completed.returncode,passed=passed,command=command)
   rows.append(row);(args.out/'report.json').write_text(json.dumps(rows,indent=2))
   print(case['name'],engine,completed.returncode,passed,flush=True)
 raise SystemExit(any(not row['passed'] for row in rows))
if __name__=='__main__':main()
