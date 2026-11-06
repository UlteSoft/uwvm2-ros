#!/usr/bin/env python3
"""Actual product tests; all output stays in .tmp and runs under the required cgroup."""
from pathlib import Path
import sys,os,json,subprocess,importlib.util,tempfile,re,hashlib
from debug_data_control_harness import configuration, compile_source_fixture
cfg=configuration('source watch');R=cfg.repo;S=cfg.source_root
spec=importlib.util.spec_from_file_location('real_dap',cfg.harness)
m=importlib.util.module_from_spec(spec);sys.modules[spec.name]=m;spec.loader.exec_module(m)
mk=tempfile.mkdtemp
def local(*a,**kw):
 kw['dir']=str(R/'.tmp');kw['prefix']='dbg-srcf-';return mk(*a,**kw)
m.tempfile.mkdtemp=local
vm=cfg.vm;out=cfg.out;out.mkdir(exist_ok=False)
source=cfg.sdk/'example/language/c/static_pointer_library/algorithm.c'
module=compile_source_fixture(cfg)
rows=[];report={'passed':False,'sdk_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'wasm_sha256':hashlib.sha256(module.read_bytes()).hexdigest(),'checks':rows};server=c=None
def check(name,ok,data=None):
 rows.append(dict(name=name,passed=bool(ok),data=data));(out/'report.json').write_text(json.dumps(report,indent=2));assert ok,(name,data)
def send(command):
 value=c.evaluate(command).decode();(out/'commands.log').open('a').write(command+'\n'+value);return value
def wait():
 for _ in range(80):
  v=send('wait')
  if 'stopped:' in v or 'guest exited' in v:return v
 raise AssertionError(('no stop',v))
def ident(reply):
 found=re.search(r'(?:breakpoint|watchpoint) (\d+)',reply);assert found,reply;return int(found[1])
def value(name):
 t=m.actual_location(c)['thread']
 frame=c.request('stackTrace',{'threadId':t})['body']['stackFrames'][0]
 return c.request('evaluate',{'expression':name,'context':'watch','frameId':frame['id']})['body']['result']
try:
 server=m.BrokerSession(vm,S/'tools/debug/secure_server.py',S/'tools/debug/dap_adapter.py',
  ['-Raot','-Rct','1','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main',
   '--wasip1-single-create','main','--wasip1-single-enable','main','--run',str(module)],out)
 c=server.connect('source');send('pause');wait()
 p=ident(send(f'tbreak bs 0 {source}:6 if i == 2'))
 send('continue');v=wait()
 check('source-temporary-condition',value('i')=='2',v)
 check('source-temporary-consumed',f'breakpoint {p} ' not in send('info breakpoints'))
 w=ident(send('watch i'));send('continue');v=wait()
 check('source-data-watch-hit','stopped: data watchpoint' in v and value('i')=='3',v)
 send(f'watchpoints disable {w}')
 p=ident(send(f'tbreak bs 0 {source}:6 if i == 4'))
 send(f'commands {p} echo source-callback;print i')
 send('continue');v=wait()
 check('source-callback-stop',value('i')=='4',v)
 log=send('callbacks')
 check('source-callback-expression','source-callback' in log and re.search(r'source value bits=0x0*4(?:\n|$)',log) is not None,log)
 check('source-callback-one-shot-consumed',f'breakpoint {p} ' not in send('info breakpoints'))
 send(f'watchpoints enable {w}')
 send('continue');v=wait();check('source-watch-reenabled',value('i')=='5' and 'stopped: data watchpoint' in v,v)
 # A watch belongs to one original activation; it must never silently bind
 # to a later function invocation whose numeric frame index happens to match.
 for _ in range(32):
  send('continue');v=wait()
  if 'watched activation retired' in v:break
 check('source-watch-activation-retirement','watched activation retired' in v,v)
 check('source-watch-auto-disabled',f'watchpoint {w} enabled=no' in send('watchpoints'))
 send(f'watchpoints delete {w}')
 report['passed']=True
except BaseException as e:report['error']=repr(e)
finally:
 if c:
  try:c.close()
  except BaseException as e:report['client_cleanup_error']=repr(e)
 if server:
  try:report['cleanup']=server.close()
  except BaseException as e:report['cleanup_error']=repr(e)
 (out/'report.json').write_text(json.dumps(report,indent=2));print(json.dumps({k:report[k] for k in ['passed','error'] if k in report}),flush=True)
raise SystemExit(not report['passed'])
