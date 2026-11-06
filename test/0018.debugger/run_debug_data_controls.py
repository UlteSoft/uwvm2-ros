#!/usr/bin/env python3
"""Actual product tests; all output stays in .tmp and runs under the required cgroup."""
from pathlib import Path
import sys,os,json,subprocess,importlib.util,tempfile,re,hashlib,time
from debug_data_control_harness import configuration, compile_source_fixture
cfg=configuration('Wasm data');R=cfg.repo;S=cfg.source_root
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
(memory (export "memory") 2)
(global $value (mut i32) (i32.const 0))
(func $main (local $x i32) (local $d f64) (local $v v128)
 i32.const 17 local.set $x
 i32.const 100 local.get $x i32.store
 (loop $again
  local.get $x i32.const 1 i32.add local.set $x
  i32.const 100 local.get $x i32.store
  local.get $x global.set $value
  local.get $x i32.const 1000 i32.lt_u br_if $again)
 i32.const 35 call $checksum_bias i32.const 42 i32.ne if unreachable end)
'''+math+'(func (export "_start") (loop $outer call $main br $outer)))'
module=out/'features.wasm';module.with_suffix('.wat').write_text(wat)
subprocess.run([cfg.wat2wasm,'--debug-names',str(module.with_suffix('.wat')),'-o',str(module)],check=True)
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
   '--wasip1-single-create','main','--wasip1-single-enable','main','--run',str(module)],out)
 c=server.connect('wasm');send('pause');p=ident(send('bw 0 0 0'));send('continue');v=wait()
 t=int(re.search(r'thread (\d+) module=0 function=0',v)[1]);send(f'delete {p}')
 step(t);step(t)
 v=send(f'set wasm local {t} 0 bits i32 29');check('local-write-ack','applied=1' in v,v)
 v=send(f'locals wasm {t} 0 0 1');check('local-read-after-write','i32 = 41' in v or 'i32=41' in v,v)
 v=send(f'set wasm local {t} 1 bits f64 3ff8000000000000');check('f64-write','applied=1' in v,v)
 v=send(f'set wasm local {t} 2 bits v128 123456789abcdef0 8877665544332211');check('v128-write','applied=1' in v,v)
 for command in [f'set wasm local {t} 0 bits f32 1',f'set wasm local {t} 999999 bits i32 1',f'set wasm local {t+100} 0 bits i32 1']:
  v=send(command);check('deny:'+command,'applied=0' in v,v)
 step(t);step(t)
 v=send(f'operands {t} 0 0 2');check('local-reaches-real-execution','41' in v,v)
 v=send(f'set wasm operand {t} 1 bits i32 4d');check('operand-write-ack','applied=1' in v,v)
 step(t)
 v=send('memory 0 0 100 4');check('operand-write-reaches-memory','4d' in v.lower(),v)
 w=ident(send('watch memory 0 0 100 4'));send('continue');v=wait()
 check('value-watch-hit','stopped: data watchpoint' in v and f'watchpoint {w}' in v,v)
 v=send('memory 0 0 100 4');check('loop-uses-edited-local','2a' in v.lower(),v)
 send(f'watchpoints disable {w}')
 current=m.actual_location(c);offset=current['offset']
 p=ident(send(f'bw 0 0 {offset}'))
 temporary=ident(send(f'tbreak bw 0 0 {offset} if false'))
 send('continue');v=wait();check('false-temp-kept',f'breakpoint {temporary} ' in send('info breakpoints'),v)
 send(f'condition {temporary} true');send('continue');v=wait()
 points=send('info breakpoints')
 check('true-temp-deleted',f'breakpoint {temporary} ' not in points,points)
 check('persistent-same-location-retained',f'breakpoint {p} ' in points,points)
 send(f'commands {p} echo first;set wasm local {t} 0 bits i32 31')
 send('continue');v=wait();check('callback-stops','stopped: breakpoint' in v,v)
 logs=send('callbacks');check('callback-ordered-log','first' in logs and 'applied=1' in logs,logs)
 v=send(f'locals wasm {t} 0 0 1');check('callback-state-write','49' in v,v)
 send(f'commands {p} set wasm local {t} 0 bits f32 1;continue')
 send('continue');v=wait();check('failed-callback-keeps-stop','stopped: breakpoint' in v,v)
 logs=send('callbacks');check('callback-error-reported','callback failed; execution remains stopped' in logs,logs)
 send(f'commands {p} echo continue-callback;continue')
 for command in [f'commands {p} echo ok;continue;echo unreachable',f'commands {p} replace 0 0 1 /host/path',f'commands {p} echo ok;run']:
  c.request('evaluate',{'expression':command,'context':'repl'},success=False)
  check('reject-callback:'+command,'continue-callback' in send(f'commands {p} list'))
 sentinel=ident(send(f'tbreak bw 0 0 {offset} ignore 2'))
 send('continue');v=wait()
 check('callback-continue-respects-other-stop','stopped: breakpoint' in v,v)
 check('ignored-temp-eventually-deleted',f'breakpoint {sentinel} ' not in send('info breakpoints'))
 check('callback-continue-recorded',send('callbacks').count('continue-callback')>=3)
 send(f'commands {p} clear');send(f'delete {p}');send(f'watchpoints enable {w}')
 send('continue');v=wait();check('reenabled-watch-hit','stopped: data watchpoint' in v,v)
 send(f'watchpoints delete {w}')
 check('watchpoints-deleted','watchpoint '+str(w)+' ' not in send('watchpoints'))
 for command in ['watch memory 0 0 131072 1','watch memory 0 0 18446744073709551615 2','watch memory 999 0 0 1']:
  response=c.request('evaluate',{'expression':command,'context':'repl'},success=False)
  check('deny:'+command,True,response)
 # Standard DAP editing and data breakpoints use current scope handles.
 def local_scope():
  frame=c.request('stackTrace',{'threadId':t})['body']['stackFrames'][0]
  scopes=c.request('scopes',{'frameId':frame['id']})['body']['scopes']
  return next(x['variablesReference'] for x in scopes if x['name']=='Typed Wasm locals')
 ref=local_scope()
 old=c.request('variables',{'variablesReference':ref})['body']['variables'][0]
 check('DAP-editable-local-hint','readOnly' not in old.get('presentationHint',{}).get('attributes',[]),old)
 response=c.request('setVariable',{'variablesReference':ref,'name':'local 0','value':'123'})['body']
 check('DAP-local-edit',response['value']=='123',response)
 c.request('setVariable',{'variablesReference':ref,'name':'local 0','value':'124'},success=False)
 check('DAP-stale-edit-denied','123' in send(f'locals wasm {t} 0 0 1'))
 ref=local_scope()
 c.request('setVariable',{'variablesReference':ref,'name':'local 0','value':'4294967296'},success=False)
 check('DAP-width-overflow-does-not-commit','123' in send(f'locals wasm {t} 0 0 1'))
 ref=local_scope()
 info=c.request('dataBreakpointInfo',{'variablesReference':ref,'name':'local 0'})['body']
 check('DAP-typed-watch-info',info['accessTypes']==['write'] and not info['canPersist'],info)
 points=c.request('setDataBreakpoints',{'breakpoints':[{'dataId':info['dataId']},{'dataId':info['dataId']}]})['body']
 check('DAP-typed-watch-install',all(x['verified'] for x in points['breakpoints']) and points['breakpoints'][0]['id']==points['breakpoints'][1]['id'],points)
 repeated=c.request('setDataBreakpoints',{'breakpoints':[{'dataId':info['dataId']}]})['body']
 check('DAP-retained-watch-is-actual',repeated['breakpoints'][0]['verified'] and repeated['breakpoints'][0]['id']==points['breakpoints'][0]['id'],repeated)

 duplicate=c.request('setDataBreakpoints',{'breakpoints':[{'dataId':info['dataId']},{'dataId':info['dataId']}]})['body']
 check('DAP-duplicate-selector-has-one-watch',len(duplicate['breakpoints'])==2 and all(x['verified'] and x['id']==points['breakpoints'][0]['id'] for x in duplicate['breakpoints']),duplicate)
 send('continue');v=wait();check('DAP-typed-watch-hit','stopped: data watchpoint' in v,v)
 send(f"watchpoints delete {points['breakpoints'][0]['id']}")
 removed=c.request('setDataBreakpoints',{'breakpoints':[{'dataId':info['dataId']}]})['body']
 check('DAP-console-deleted-watch-not-retained',not removed['breakpoints'][0]['verified'],removed)
 c.request('setDataBreakpoints',{'breakpoints':[]})
 info=c.request('dataBreakpointInfo',{'name':'wasm-memory:0:0:100:4'})['body']
 points=c.request('setDataBreakpoints',{'breakpoints':[{'dataId':info['dataId'],'accessType':'read'}]})['body']
 check('DAP-read-watch-explicit-refusal',not points['breakpoints'][0]['verified'],points)
 points=c.request('setDataBreakpoints',{'breakpoints':[{'dataId':info['dataId'],'accessType':'write'}]})['body']
 check('DAP-memory-watch-install',points['breakpoints'][0]['verified'],points)
 send('continue');v=wait();check('DAP-memory-watch-hit','stopped: data watchpoint' in v,v)
 c.request('setDataBreakpoints',{'breakpoints':[]})
 stale=c.request('dataBreakpointInfo',{'name':'wasm-memory:0:0:100:4'})['body']['dataId']
 step(t)
 points=c.request('setDataBreakpoints',{'breakpoints':[{'dataId':stale}]})['body']
 check('DAP-uninstalled-stale-data-id-denied',not points['breakpoints'][0]['verified'],points)
 w=ident(send('watch memory 0 0 0 70000'));send('continue');v=wait()
 check('large-watch-range-retained','preview-only total-bytes=70000' in v and len(v.encode())<4096,v)
 before=send(f'watchpoints old {w} 100 4')
 after=send(f'watchpoints new {w} 100 4')
 check('large-watch-difference-pages',before!=after and 'total=70000' in before and 'total=70000' in after,[before,after])
 end=send(f'watchpoints new {w} 69999 256')
 check('large-watch-final-page','first=69999 total=70000 bytes=00 ' in end,end)
 c.request('evaluate',{'expression':f'watchpoints new {w} 70001 1','context':'repl'},success=False)
 send(f'watchpoints delete {w}')
 current=m.actual_location(c);temp=ident(send(f'tbreak bw 0 {current["function"]} {current["offset"]}'))
 for i in range(120):
  send(f'commands {temp} '+('append ' if i else '')+'echo '+('测'*150)+str(i))
 check('large-command-list-page','callback-command-next=120 total=120' in send(f'commands {temp} list 100 20'))
 send('continue');wait()
 first=offset=0;pieces=[]
 for _ in range(20):
  packet=send(f'callbacks {first} 64 {offset} 32768')
  marker=re.search(r'callback-next=(\d+) byte=(\d+) total=(\d+)\n$',packet)
  check('history-page-cursor',marker is not None,packet[-90:])
  first,offset,total=map(int,marker.groups());pieces.append(packet[:marker.start()])
  check('history-page-wire-budget',len(packet.encode())<65536)
  if first==total and offset==0:break
 else:raise AssertionError('callback history paging did not complete')
 text=''.join(pieces)
 check('large-callback-history-no-loss',len(text.encode())>54000 and text.count('测')==18000)
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
