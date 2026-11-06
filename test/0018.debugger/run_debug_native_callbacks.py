#!/usr/bin/env python3
"""Actual product tests; all output stays in .tmp and runs under the required cgroup."""
from pathlib import Path
import sys,json,subprocess,importlib.util,tempfile,re,hashlib
from debug_data_control_harness import configuration
cfg=configuration('native callbacks');R=cfg.repo;S=cfg.source_root
spec=importlib.util.spec_from_file_location('real_dap',cfg.harness);m=importlib.util.module_from_spec(spec);sys.modules[spec.name]=m;spec.loader.exec_module(m)
original=tempfile.mkdtemp
def local(*args,**kwargs):
 kwargs['dir']=str(R/'.tmp');kwargs['prefix']='parity-native-';return original(*args,**kwargs)
m.tempfile.mkdtemp=local
vm=cfg.vm;out=cfg.out;arch=cfg.arch;out.mkdir(exist_ok=False)
sdk=cfg.sdk/'example/language/wat/library_kinds/static_math.wat';s=sdk.read_text();math=s[s.index('  (func'):s.rindex(')')]
wat='(module (func $window (export "_start") (local $value i32) i32.const 17 local.set $value (loop $again local.get $value i32.const 41 i32.xor local.set $value local.get $value i32.const 1 i32.rotl local.set $value i32.const 64 local.get $value i32.store br $again)) '+math+' (memory 1))'
f=out/'native.wat';f.write_text(wat);wasm=f.with_suffix('.wasm');subprocess.run([cfg.wat2wasm,'--debug-names',str(f),'-o',str(wasm)],check=True)
server=None;c=None;report={'arch':arch,'passed':False,'checks':[],'sdk_sha256':hashlib.sha256(sdk.read_bytes()).hexdigest()}
def continue_to_stop():
 # Time slicing preserves an actual owned trap. QEMU is a correctness
 # executor and may need more slices than native hardware; never interpret
 # an unsupported-instruction refusal as a timeout or a successful hit.
 import time
 deadline=time.monotonic()+30
 while True:
  packet=c.evaluate('continue').decode()
  if 'continue time slice ended;' not in packet:return packet
  if time.monotonic()>=deadline:raise AssertionError('native continue exceeded test deadline')
  current=m.actual_location(c)
  assert 'native-pc=' in current['raw'],current
def check(name,ok,data=None):
 report['checks'].append(dict(name=name,passed=bool(ok),data=data));assert ok,(name,data)
try:
 server=m.BrokerSession(vm,S/'tools/debug/secure_server.py',S/'tools/debug/dap_adapter.py',['-Raot','-Rct','1','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--run',str(wasm)],out)
 report['walk']=m.native_case(server,wasm)
 c=server.connect('native');location=m.actual_location(c);thread=location['thread'];frame=m.physical_frame(c,thread)
 scopes=c.request('scopes',{'frameId':frame['id']})['body']['scopes']
 regs=c.request('variables',{'variablesReference':scopes[0]['variablesReference']})['body']['variables']
 expected={'x86_64':48,'i686':18,'aarch64':66,'arm':49,'armhf':49}[arch]
 check('native-register-ABI',len(regs)==expected,[x['name'] for x in regs])
 for first,count in [(0,0),(0,1),(63,1),(64,1),(65,1),(66,1),((1<<64)-1,1)]:
  page=c.request('variables',{'variablesReference':scopes[0]['variablesReference'],'start':first,'count':count})['body']['variables']
  expected_page=regs[first:] if count==0 else regs[first:first+count]
  check('native-register-page-'+str(first)+'-'+str(count),page==expected_page,page)
 for command in ['info registers','info registers pc','info registers sp','info registers fp','info all-registers']:
  reply=c.evaluate(command).decode();check(command,'error:' not in reply and 'native-registers' in reply,reply)
 before=m.actual_location(c)
 for command in ['info registers not_a_register','info registers 999999999999999999999999','breakasm 65537','breakasm -1','jump asm *0x1','jump asm 18446744073709551615','set $pc = 1','set $sp = 0']:
  response=c.request('evaluate',{'expression':command,'context':'repl'},success=False)
  report['checks'].append(dict(name='deny:'+command,passed=True))
 check('rejected-controls-preserve-stop',m.actual_location(c)['stop']==before['stop'])
 dis=c.evaluate(f"disassemble-range {thread} {before['stop']} 32 0 0 1").decode();(out/'disassembly.txt').write_text(dis)
 values=[(int(a),int(b,16)) for a,b in re.findall(r'function-offset=(\d+) pc=0x([0-9a-f]+)',dis)]
 if not values:values=[(int(a),int(b,16)) for b,a in re.findall(r'pc=0x([0-9a-f]+) function-offset=(\d+)',dis)]
 check('native-relative-instructions',bool(values),dis)
 target=values[1] if len(values)>1 else values[0]
 reply=c.evaluate(f'breakasm {target[0]}').decode();point=re.search(r'breakpoint (\d+)',reply);check('native-breakpoint-create',bool(point),reply);identifier=int(point[1])
 again=c.evaluate(f'bi {target[0]}').decode();check('native-breakpoint-dedup',f'breakpoint {identifier}' in again,again)
 for command in [f'disable {identifier}',f'enable {identifier}',f'ignore {identifier} 0',f'condition {identifier}']:
  reply=c.evaluate(command).decode();check('native-'+command,'error:' not in reply,reply)
 before_condition=m.actual_location(c)
 c.evaluate(f'condition {identifier} $pc != 0')
 check('native-condition-install-keeps-stop',m.actual_location(c)['stop']==before_condition['stop'])
 for expression in ('$sp != 0','$not_a_register == 1','$pc[0] == 1','*(pc) == 1','system(1)','p$c == 1'):
  c.request('evaluate',{'expression':f'condition {identifier} {expression}','context':'repl'},success=False)
  check('native-condition-deny:'+expression,m.actual_location(c)['stop']==before_condition['stop'])

 oldref=frame.get('instructionPointerReference')
 continue_to_stop()
 for _ in range(20):
  stopped=c.evaluate('wait').decode()
  if 'stopped: native breakpoint' in stopped:break
 check('native-breakpoint-hit','stopped: native breakpoint' in stopped and f'native-breakpoint {identifier}' in stopped,stopped)
 c.request('disassemble',{'memoryReference':oldref,'instructionCount':1},success=False);report['checks'].append(dict(name='stale-native-reference-denied',passed=True))
 c.evaluate(f'delete {identifier}')
 check('post-breakpoint-still-usable',bool(m.physical_frame(c,thread)))
 # A distinct one-shot point, callback commands and a numeric memory watch
 # share the real native stop without borrowing any host address.
 current=m.actual_location(c)
 temp_reply=c.evaluate(f'tbreak bi {target[0]}').decode()
 temp=int(re.search(r'breakpoint (\d+)',temp_reply)[1])
 c.evaluate(f'commands {temp} echo native-temporary;info all-registers')
 c.evaluate(f'condition {temp} $pc == 0')
 persistent=int(re.search(r'breakpoint (\d+)',c.evaluate(f'bi {target[0]}').decode())[1])
 watch_reply=c.evaluate('watch memory 0 0 0 4').decode()
 watch=int(re.search(r'watchpoint (\d+)',watch_reply)[1])
 c.evaluate(f'condition {persistent} ($pc == 0x{target[1]:x}) && (($pc + 1) > $pc)')
 continue_to_stop()
 first=c.evaluate('wait').decode()
 points=c.evaluate('info breakpoints').decode()
 check('native-false-condition-stays-registered','stopped: native breakpoint' in first and f'breakpoint {temp} ' in points,first+points)
 check('native-false-condition-no-callback','native-temporary' not in c.evaluate('callbacks').decode())
 c.evaluate(f'condition {temp} ($pc == 0x{target[1]:x}) ? true : false')
 continue_to_stop()
 for _ in range(20):
  stopped=c.evaluate('wait').decode()
  if 'stopped: native breakpoint' in stopped:break
 check('native-temporary-hit','stopped: native breakpoint' in stopped,stopped)
 points=c.evaluate('info breakpoints').decode()
 check('native-temporary-deleted',f'breakpoint {temp} ' not in points,points)
 check('native-persistent-retained',f'breakpoint {persistent} ' in points,points)
 logs=c.evaluate('callbacks').decode()
 check('native-callback-ordered-values','native-temporary' in logs and 'known-bits=' in logs,logs)
 # A runtime arithmetic failure keeps the actual native stop and its
 # temporary point; it cannot silently continue or consume that point.
 bad=int(re.search(r'breakpoint (\d+)',c.evaluate(f'tbreak bi {target[0]}').decode())[1])
 c.evaluate(f'condition {bad} ($pc / ($pc - $pc)) != 0')
 import time
 deadline=time.monotonic()+30
 while True:
  number=c.queue([('evaluate',{'expression':'continue','context':'repl'})])[0]
  response=c.response(number)
  if not response.get('success'):break
  assert 'continue time slice ended;' in response['body']['result'],response
  assert time.monotonic()<deadline
 stopped=m.actual_location(c)
 check('native-condition-arithmetic-failure-stays-native','native-pc=' in stopped['raw'],response)
 check('native-condition-error-keeps-temporary',f'breakpoint {bad} ' in c.evaluate('info breakpoints').decode())
 c.evaluate(f'delete {bad}')
 c.evaluate(f'watchpoints disable {watch}')
 c.evaluate(f'watchpoints delete {watch}')
 c.evaluate(f'delete {persistent}')

 w=int(re.search(r'watchpoint (\d+)',c.evaluate('watch memory 0 0 64 4').decode())[1])
 continue_to_stop()
 for _ in range(20):
  stopped=c.evaluate('wait').decode()
  if 'stopped: native data watchpoint' in stopped:break
 check('native-data-write-stops','stopped: native data watchpoint' in stopped,stopped)
 page=c.evaluate(f'watchpoints new {w} 0 4').decode()
 check('native-data-write-page','first=0 total=4 bytes=' in page,page)
 c.evaluate(f'watchpoints delete {w}')
 c.close();c=None;report['passed']=True
except BaseException as e:
 report['error']=repr(e)
finally:
 if c:
  try:c.close()
  except BaseException as e:report['client_cleanup_error']=repr(e);report['passed']=False
 if server:
  try:report['cleanup']=server.close()
  except BaseException as e:report['cleanup_error']=repr(e);report['passed']=False
 (out/'report.json').write_text(json.dumps(report,indent=2));print(json.dumps({'passed':report['passed'],'error':report.get('error'),'checks':len(report['checks'])}))
raise SystemExit(not report['passed'])
