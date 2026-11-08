from pathlib import Path
import sys,subprocess,json,hashlib
R=Path(__file__).resolve().parents[3];O=Path(sys.argv[2]).resolve();O.mkdir(parents=True,exist_ok=True)
assert O.is_relative_to(R/'.tmp'), 'Generated fixtures must stay below repository .tmp'
sys.path.insert(0,str(R/'test/0086.memtag'));import vm_matrix as m
tool=Path(sys.argv[1]).resolve()
def parse(name,wat):
 p=O/(name+'.wat');p.write_text(wat);b=p.with_suffix('.wasm')
 subprocess.run([str(tool),'parse',str(p),'-o',str(b)],check=True)
 return b
def patch_tags(source):
 raw=source.read_bytes();result=raw[:8];at=8
 while at<len(raw):
  kind=raw[at];size,pos=leb(raw,at+1);payload=raw[pos:pos+size];at=pos+size
  if kind==10:
   count,pos=leb(payload,0);bodies=[]
   for i in range(count):
    length,start=leb(payload,pos);body=payload[start:start+length];pos=start+length;bodies.append(body)
   assert bodies[:3]==[b'\0\x20\0\x0b',b'\0\x01\x0b',b'\0\x01\x0b'],[b.hex() for b in bodies[:3]]
   p=m.Program()
   bodies[0]=b'\0\x20\0'+m.c(3)+m.op(0x22)+b'\x0b'
   bodies[1]=b'\0'+p.tags(0,16,3)+b'\x0b'
   bodies[2]=b'\0'+p.tagged(0)+m.c(16)+m.op(0x29)+b'\x1a\x0b'
   payload=m.u(count)+b''.join(m.u(len(body))+body for body in bodies)
  result+=m.sec(kind,payload)
 target=source.with_suffix('.mte.wasm');target.write_bytes(result);return target
def leb(data,at):
 value=0;shift=0
 while True:
  b=data[at];at+=1;value|=(b&127)<<shift
  if not b&128:return value,at
  shift+=7
owner=parse('owner','(module (memory (export "memory") 1 2 shared))')
rows=[]
for kind in ['atomic','ordinary-race','growth','retag-stale']:
 operation='(drop (i32.atomic.rmw.add (call $ptr (i32.const 0)) (i32.const 1)))'
 if kind=='ordinary-race':
  operation='(i32.store (call $ptr (i32.const 0)) (i32.add (i32.load (call $ptr (i32.const 0))) (i32.const 1)))'
 grow='''(if (i32.ne (memory.grow (i32.const 1)) (i32.const 1)) (then unreachable))
 (i32.store (i32.const 65536) (i32.const 91))''' if kind=='growth' else ''
 checkgrow='''(if (i32.ne (memory.size) (i32.const 2)) (then unreachable))
 (if (i32.ne (i32.load (i32.const 65536)) (i32.const 91)) (then unreachable))''' if kind=='growth' else ''
 checkcount='' if kind=='ordinary-race' else '(if (i32.ne (i32.load (call $ptr (i32.const 0))) (i32.const 40000)) (then unreachable))'
 stale='(call $retire) (i32.store (call $ptr (i32.const 0)) (i32.const 777))' if kind=='retag-stale' else ''
 wat='''(module
(import "env" "memory" (memory 1 2 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
(import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32)(result i32)))
(import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
(export "memory" (memory 0))
;; The standard fixture is parsed/validated first. The separate .mte.wasm
;; replaces only these three bodies with the pinned extension encodings.
(func $ptr (param i32) (result i32) (local.get 0))
(func $tag (nop))
(func $retire (nop))
(func $work (local $i i32)
 (loop $again
 '''+operation+'''
 (local.set $i (i32.add (local.get $i) (i32.const 1)))
 (br_if $again (i32.lt_u (local.get $i) (i32.const 20000)))))
(func (export "wasi_thread_start") (param i32 i32)
 (block $ready (loop $wait
  (br_if $ready (i32.atomic.load (i32.const 4096)))
  (drop (memory.atomic.wait32 (i32.const 4096) (i32.const 0) (i64.const -1)))
  (br $wait)))
 '''+grow+'''
 (call $work)
 (i32.atomic.store (i32.const 4100) (i32.const 1))
 (drop (memory.atomic.notify (i32.const 4100) (i32.const 1))))
(func (export "_start")
 (call $tag)
 (if (i32.lt_s (call $spawn (i32.const 0)) (i32.const 0)) (then unreachable))
 (i32.atomic.store (i32.const 4096) (i32.const 1))
 (drop (memory.atomic.notify (i32.const 4096) (i32.const 1)))
 (call $work)
 (block $joined (loop $wait
  (br_if $joined (i32.atomic.load (i32.const 4100)))
  (drop (memory.atomic.wait32 (i32.const 4100) (i32.const 0) (i64.const -1)))
  (br $wait)))
 '''+checkcount+checkgrow+stale+'''
 (i32.store (i32.const 4200) (call $ptr (i32.const 0)))
 (i32.store (i32.const 4204) (i32.const 4))
 (if (call $write (i32.const 1) (i32.const 4200) (i32.const 1) (i32.const 4216)) (then unreachable))
 (call $exit (i32.const 0))))'''
 standard=parse(kind,wat); tagged=patch_tags(standard)
 for file in [standard,tagged]:
  rows.append(dict(name=file.stem,kind=kind,file=str(file),owner=str(owner),sha256=hashlib.sha256(file.read_bytes()).hexdigest(),extension=file==tagged))
(O/'manifest.json').write_text(json.dumps(rows,indent=2)+'\n');print('generated',len(rows),'Wasm-origin concurrent controls')
