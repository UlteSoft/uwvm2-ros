#!/usr/bin/env python3
"""No-host-import WasmMTE VM regression cases; run inside the task resource cgroup.
Custom opcodes are encoded directly, so no WAT tool silently strips extensions.
"""
import argparse,json,struct,subprocess,hashlib
from pathlib import Path
def u(n):
 b=bytearray()
 while True:
  t=n&127;n>>=7;b.append(t|(128 if n else 0))
  if not n:return bytes(b)
def s(n,bits):
 n&=(1<<bits)-1
 if n&(1<<(bits-1)):n-=1<<bits
 b=bytearray()
 while True:
  t=n&127;n>>=7;done=(n==0 and not(t&64)) or (n==-1 and t&64)
  b.append(t|(0 if done else 128))
  if done:return bytes(b)
def c(n,bits=32):return bytes([0x41 if bits==32 else 0x42])+s(n,bits)
def sec(i,b):return bytes([i])+u(len(b))+b
def op(n,m=0):return b'\xfc'+u(n)+u(m)
def mem(n,offset=0,index=0):
 return bytes([n])+u(64 if index else 0)+(u(index) if index else b'')+u(offset)
def simd(n,offset=0,lane=None):
 return b'\xfd'+u(n)+u(0)+u(offset)+(u(lane) if lane is not None else b'')
def assertion(expression,expected,bits=32):
 return expression+c(expected,bits)+bytes([0x47 if bits==32 else 0x52,0x04,0x40,0x00,0x0b])
def module(code,wide=False,passive=None,shared=False,memories=1):
 body=b'\x01\x01'+bytes([0x7e if wide else 0x7f])+code+b'\x0b'
 result=b'\0asm\x01\0\0\0'+sec(1,b'\x01\x60\0\0')+sec(3,b'\x01\0')
 result+=sec(5,u(memories)+b''.join(u(1+(4 if wide else 0)+(2 if shared else 0))+u(1)+u(2) for _ in range(memories)))
 result+=sec(7,b'\x01\x06_start\0\0')
 if passive is not None:result+=sec(12,b'\x01')
 result+=sec(10,b'\x01'+u(len(body))+body)
 if passive is not None:result+=sec(11,b'\x01\x01'+u(len(passive))+passive)
 return result
class Program:
 def __init__(self,wide=False):self.wide=wide;self.bits=64 if wide else 32;self.code=b''
 def n(self,n):return c(n,self.bits)
 def tagged(self,p,tag=3,m=0):return self.n(p)+self.n(tag)+op(0x22,m)
 def tags(self,p=0,length=512,tag=3,m=0):return self.tagged(p,tag,m)+self.n(length)+op(0x2b,m)
 def add(self,code):self.code+=code
 def eq(self,expression,n,bits=32):self.add(assertion(expression,n,bits))
 def bytes(self,position,values):
  for i,v in enumerate(values):self.add(self.tagged(position+i)+c(v)+mem(0x3a))
 def check_bytes(self,position,values):
  for i,v in enumerate(values):self.eq(self.tagged(position+i)+mem(0x2d),v)
def markers(wide,enabled):
 p=Program(wide);n=p.n;t=p.tagged;bits=p.bits
 p.eq(op(0x20),2 if enabled else 0,bits)
 p.eq(op(0x23),(8 if wide else 4) if enabled else 0,bits)
 p.eq(op(0x24),(56 if wide else 28) if enabled else bits,bits)
 p.eq(t(17)+op(0x21),3 if enabled else 0,bits)
 p.eq(t(17)+op(0x28),17,bits)
 p.eq(t(17)+t(5,7)+op(0x26),12,bits)
 p.eq(n(17)+t(0,7)+op(0x25)+op(0x21),7 if enabled else 0,bits)
 p.eq(n(17)+t(0,7)+n(2)+op(0x36)+op(0x21),9 if enabled else 0,bits)
 p.add(p.tags());p.eq(n(17)+op(0x27)+op(0x21),3 if enabled else 0,bits)
 p.add(t(0)+c(77)+mem(0x36));p.add(t(0)+n(32)+op(0x2c));p.eq(t(0)+mem(0x28),0)
 for opcode in [0x29,0x2a]:
  p.add(p.tags());p.add(t(0)+c(77)+mem(0x36))
  p.add(t(0)+n(32)+op(opcode)+b'\x21\0')
  p.eq(b'\x20\0'+mem(0x28),0 if opcode==0x2a else 77)
  p.eq(b'\x20\0'+op(0x21),0,bits)
 p.eq(n(17)+op(0x2d)+op(0x28),17,bits)
 for opcode in [0x2e,0x2f]:
  p.add(p.tags());p.add(t(0)+c(77)+mem(0x36))
  p.add(n(0)+n(32)+op(opcode)+b'\x21\0')
  p.eq(b'\x20\0'+mem(0x28),0 if opcode==0x2f else 77)
  p.eq(b'\x20\0'+op(0x28),0,bits)
 for opcode in [0x37,0x38]:
  p.add(p.tags());p.add(t(0)+c(77)+mem(0x36))
  p.add(n(0)+n(32)+t(0,7)+n(2)+op(opcode)+b'\x21\0')
  p.eq(b'\x20\0'+mem(0x28),0 if opcode==0x38 else 77)
  p.eq(b'\x20\0'+op(0x21),9 if enabled else 0,bits)
 return module(p.code,wide)
def scalars(wide):
 p=Program(wide);p.add(p.tags());data=bytes([0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88])
 loads=[(0x28,32,4,False,False),(0x29,64,8,False,False),(0x2a,32,4,False,True),(0x2b,64,8,False,True),
 (0x2c,32,1,True,False),(0x2d,32,1,False,False),(0x2e,32,2,True,False),(0x2f,32,2,False,False),
 (0x30,64,1,True,False),(0x31,64,1,False,False),(0x32,64,2,True,False),(0x33,64,2,False,False),
 (0x34,64,4,True,False),(0x35,64,4,False,False)]
 p.bytes(15,data)
 for opcode,bits,width,sign,floating in loads:
  expression=p.tagged(0)+mem(opcode,15)+(bytes([0xbc if bits==32 else 0xbd]) if floating else b'')
  p.eq(expression,int.from_bytes(data[:width],'little',signed=sign),bits)
 stores=[(0x36,32,4,False),(0x37,64,8,False),(0x38,32,4,True),(0x39,64,8,True),
 (0x3a,32,1,False),(0x3b,32,2,False),(0x3c,64,1,False),(0x3d,64,2,False),(0x3e,64,4,False)]
 for opcode,bits,width,floating in stores:
  value=0x887766554433eeff&((1<<bits)-1)
  p.add(p.tagged(0)+c(value,bits)+(bytes([0xbe if bits==32 else 0xbf]) if floating else b'')+mem(opcode,15))
  p.check_bytes(15,value.to_bytes(bits//8,'little')[:width])
 return module(p.code,wide)
SIMD=[(0,16,0,False,False),(1,8,0,False,False),(2,8,0,False,False),(3,8,0,False,False),(4,8,0,False,False),
 (5,8,0,False,False),(6,8,0,False,False),(7,1,0,False,False),(8,2,0,False,False),(9,4,0,False,False),
 (10,8,0,False,False),(11,16,0,True,True),(84,1,15,True,False),(85,2,7,True,False),(86,4,3,True,False),
 (87,8,1,True,False),(88,1,15,True,True),(89,2,7,True,True),(90,4,3,True,True),(91,8,1,True,True),
 (92,4,0,False,False),(93,8,0,False,False)]
def simds(wide):
 p=Program(wide);p.add(p.tags());source=bytes(range(0x80,0x90));vector=bytes(range(0x40,0x50))
 for opcode,width,lane,consumes,store in SIMD:
  p.bytes(15,source);p.bytes(128,bytes(16));expected=bytearray(vector if consumes else bytes(16))
  if store:expected=bytearray(16);expected[:width]=vector[lane*width:(lane+1)*width]
  elif opcode in [0,92,93]:expected[:width]=source[:width]
  elif 84<=opcode<=87:expected[lane*width:(lane+1)*width]=source[:width]
  elif 7<=opcode<=10:expected=bytearray(source[i%width] for i in range(16))
  else:
   input_width=1<<((opcode-1)//2)
   for j in range(8//input_width):
    expected[j*2*input_width:j*2*input_width+input_width]=source[j*input_width:(j+1)*input_width]
    expected[j*2*input_width+input_width:(j+1)*2*input_width]=bytes([255 if opcode&1 else 0])*input_width
  if not store:p.add(p.tagged(128))
  p.add(p.tagged(128 if store else 15))
  if consumes:p.add(b'\xfd\x0c'+vector)
  p.add(simd(opcode,lane=lane if 84<=opcode<=91 else None))
  if not store:p.add(simd(11))
  p.check_bytes(128,expected)
 return module(p.code,wide)
def bulks(wide):
 p=Program(wide);p.add(p.tags());p.bytes(0,range(128))
 p.add(p.tagged(256)+p.tagged(0)+p.n(128)+op(10)+u(0));p.check_bytes(256,range(128))
 p.add(p.tagged(17)+p.tagged(0)+p.n(128)+op(10)+u(0));p.check_bytes(17,range(128))
 p.add(p.tagged(300)+c(0x55)+p.n(64)+op(11));p.check_bytes(300,bytes([0x55])*64)
 p.add(p.tagged(15)+c(0)+c(32)+op(8)+u(0));p.check_bytes(15,range(32))
 p.add(op(9));p.add(p.tagged(0)+c(0)+c(0)+op(8)+u(0))
 return module(p.code,wide,bytes(range(32)))
def atomics(wide):
 p=Program(wide);p.add(p.tags())
 for opcode in range(0x10,0x4f):
  i=(opcode-0x10)%7;bits=64 if i in [1,4,5,6] else 32;width=[4,8,1,2,1,2,4][i]
  p.add(p.tagged(0)+c(7,64)+mem(0x37))
  expression=p.tagged(0)
  if opcode>=0x48:expression+=c(7,bits)
  if opcode>=0x17:expression+=c(3,bits)
  expression+=b'\xfe'+u(opcode)+u(width.bit_length()-1)+u(0)
  if 0x17<=opcode<0x1e:p.add(expression);expected=3
  else:
   p.eq(expression,7,bits)
   expected=7 if opcode<0x17 else [10,4,3,7,4,3,3][(opcode-0x1e)//7]
  p.eq(p.tagged(0)+mem(0x2d),expected)
 p.add(b'\xfe\x03\0')
 return module(p.code,wide)
def denial(wide,kind):
 p=Program(wide);p.add(p.tags());bad=16 if kind in ['scalar','simd'] else 64
 p.add(p.tags(bad,16,4))
 if kind=='scalar':p.add(p.tagged(0)+c(17,64)+mem(0x37,15))
 elif kind=='simd':p.add(p.tagged(0)+b'\xfd\x0c'+bytes(16)+simd(11,15))
 elif kind=='fill':p.add(p.tagged(0)+c(55)+p.n(128)+op(11))
 elif kind=='copy':p.add(p.tagged(128)+p.tagged(0)+p.n(128)+op(10)+u(0))
 elif kind=='atomic':p.add(p.tagged(64)+c(55)+b'\xfe\x17\x02\0')
 elif kind=='bounds':p.add(p.tagged(65535)+mem(0x28)+b'\x1a')
 return module(p.code,wide)
def main():
 a=argparse.ArgumentParser();a.add_argument('--vm',type=Path);a.add_argument('--out',type=Path,required=True)
 a.add_argument('--engines',default='int,aot');a.add_argument('--generate-only',action='store_true');args=a.parse_args()
 args.out.mkdir(parents=True,exist_ok=True);cases=[]
 for wide in [False,True]:
  for enabled in [False,True]:
   for name,make in [('markers',lambda w:markers(w,enabled)),('scalars',scalars),('simd',simds),('bulk',bulks),('atomic',atomics)]:
    cases.append((f'{name}-{64 if wide else 32}-{int(enabled)}',make(wide),enabled,'success'))
   for kind in ['scalar','simd','fill','copy','atomic','bounds']:
    expected='bounds' if kind=='bounds' else 'memtag' if enabled else 'success'
    cases.append((f'deny-{kind}-{64 if wide else 32}-{int(enabled)}',denial(wide,kind),enabled,expected))
 manifest=[]
 for name,wasm,enabled,expected in cases:
  file=args.out/(name+'.wasm');file.write_bytes(wasm)
  manifest.append(dict(name=name,file=str(file),enabled=enabled,expected=expected,sha256=hashlib.sha256(wasm).hexdigest()))
 (args.out/'manifest.json').write_text(json.dumps(manifest,indent=2))
 if args.generate_only:return
 if args.vm is None:a.error('--vm is required without --generate-only')
 rows=[]
 for case in manifest:
  for engine in args.engines.split(','):
   command=[str(args.vm),f'--runtime-{engine}','--runtime-llvm-jit-cache-path','disable','--wasm-feature-enable-memory64','--wasm-feature-enable-table64','--wasm-feature-enable-threads','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasm-set-start-func','0']
   if not case['enabled']:command+=['--runtime-disable-memtag']
   command+=['--',case['file']]
   completed=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=90)
   output=completed.stdout.decode('utf8','replace');(args.out/(case['name']+'-'+engine+'.log')).write_text(output)
   passed=completed.returncode==0 if case['expected']=='success' else completed.returncode==({'memtag':122,'bounds':121}[case['expected']]) and (
    'memory tag mismatch' in output.lower() if case['expected']=='memtag' else 'out of bounds' in output.lower())
   row=dict(name=case['name'],engine=engine,exit=completed.returncode,expected=case['expected'],passed=passed,command=command)
   rows.append(row);(args.out/'report.json').write_text(json.dumps(rows,indent=2))
   print(case['name'],engine,completed.returncode,'PASS' if passed else 'FAIL',flush=True)
 raise SystemExit(any(not row['passed'] for row in rows))
if __name__=='__main__':main()
