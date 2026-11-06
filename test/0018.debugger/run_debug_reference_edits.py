#!/usr/bin/env python3
"""Actual product tests; all output stays in .tmp and runs under the required cgroup."""
from pathlib import Path
import sys,os,json,subprocess,importlib.util,tempfile,re,hashlib,time
from debug_data_control_harness import configuration, compile_source_fixture
cfg=configuration('reference edits');R=cfg.repo;S=cfg.source_root
spec=importlib.util.spec_from_file_location('real_dap',cfg.harness)
m=importlib.util.module_from_spec(spec);sys.modules[spec.name]=m;spec.loader.exec_module(m)
mk=tempfile.mkdtemp
def local(*a,**kw):
 kw['dir']=str(R/'.tmp');kw['prefix']='dbg-feat-';return mk(*a,**kw)
m.tempfile.mkdtemp=local
vm=cfg.vm;out=cfg.out;out.mkdir(exist_ok=False)
sdk=cfg.sdk/'example/language/wat/library_kinds/static_math.wat'
text=sdk.read_text();math=text[text.index('  (func'):text.rindex(')')]
wat='''(module
(type $t (func (result i32)))
(memory (export "memory") 1)
(func $a (type $t) i32.const 11)
(func $b (type $t) i32.const 22)
(func $bad (param i32) (result i32) local.get 0)
(elem declare func $a $b $bad)
(func $main (export "main") (local $r (ref null $t))
 ref.func $a local.set $r
 i32.const 0 local.get $r ref.as_non_null call_ref $t i32.store
 i32.const 4 ref.func $a call_ref $t i32.store
 i32.const 35 call $checksum_bias i32.const 42 i32.ne if unreachable end)
'''+math+'(func (export "_start") (loop $outer call $main br $outer)))'
module=out/'features.wasm';module.with_suffix('.wat').write_text(wat)
subprocess.run([cfg.wasm_tools,'parse',str(module.with_suffix('.wat')),'-o',str(module)],check=True)
subprocess.run([cfg.wasm_tools,'validate','--features','all',str(module)],check=True)
rows=[];report={'passed':False,'sdk_sha256':hashlib.sha256(sdk.read_bytes()).hexdigest(),'checks':rows};server=None;c=None
def check(name,ok,data=None):
 rows.append(dict(name=name,passed=bool(ok),data=data));(out/'report.json').write_text(json.dumps(report,indent=2));assert ok,(name,data)
def send(command):
 value=c.evaluate(command).decode();(out/'commands.log').open('a').write(command+'\n'+value);return value
def wait():
 for _ in range(40):
  v=send('wait')
  if 'stopped:' in v or 'guest exited' in v:return v
 raise AssertionError(('no stop',v))
def ident(reply):
 found=re.search(r'(?:breakpoint|watchpoint) (\d+)',reply);assert found,reply;return int(found[1])
def step(t):
 v=send('step '+str(t));check('actual-step','stopped:' in v,v);return v
try:
 server=m.BrokerSession(vm,S/'tools/debug/secure_server.py',S/'tools/debug/dap_adapter.py',
  ['-Raot','-Rct','1','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main',
   '--wasip1-single-create','main','--wasip1-single-enable','main','-WFE-function-references','--run',str(module)],out)
 c=server.connect('wasm');send('pause');p=ident(send('bw 0 3 0'));send('continue');v=wait()
 t=int(re.search(r'thread (\d+) module=0 function=3',v)[1]);send(f'delete {p}')
 step(t);step(t)
 for command in [f'set wasm local {t} 0 function 0 2',f'set wasm local {t} 0 function 9 1',f'set wasm local {t} 0 bits i64 123']:
  v=send(command);check('reference-deny:'+command,'applied=0' in v,v)
 v=send(f'set wasm local {t} 0 function 0 1');check('reference-local-edit','applied=1' in v,v)
 # Set an independently validated byte breakpoint after each real call/store.
 p=ident(send('bw 0 3 14'));send('continue');v=wait()
 check('reference-local-real-call','16' in send('memory 0 0 0 4'),v)
 send(f'delete {p}')
 step(t);step(t)
 current=m.actual_location(c)
 check('direct-ref-call-site',current['function']==3 and current['offset']==18,current)
 v=send(f'set wasm operand {t} 1 function 0 1');check('reference-operand-edit','applied=1' in v,v)
 p=ident(send('bw 0 3 23'));send('continue');v=wait()
 check('direct-ref-optimization-invalidated','16' in send('memory 0 0 4 4'),v)
 send(f'delete {p}')
 report['passed']=True
except BaseException as e:
 report['error']=repr(e)
finally:
 if c:
  try:c.close()
  except BaseException as e:report['client_cleanup_error']=repr(e)
 if server:
  try:report['cleanup']=server.close()
  except BaseException as e:report['cleanup_error']=repr(e)
 (out/'report.json').write_text(json.dumps(report,indent=2));print(json.dumps({k:report[k] for k in ['passed','error'] if k in report}),flush=True)
raise SystemExit(not report['passed'])
