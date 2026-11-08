from pathlib import Path
import json,subprocess,sys,os,hashlib,resource
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
R=Path(__file__).resolve().parents[3]
if len(sys.argv)<5:raise SystemExit('usage: compile_component.py SOURCE BASELINE_VM FROZEN_SOURCE OUTPUT [--build-only]')
source=Path(sys.argv[1]).resolve();base=Path(sys.argv[2]).resolve();S=Path(sys.argv[3]).resolve();out=Path(sys.argv[4]).resolve()
assert out.is_relative_to(R/'.tmp'), 'Artifacts must stay under repository .tmp'
out.mkdir(parents=True,exist_ok=True)
original=json.loads((base/'main.default.cpp.argv.json').read_text())
cmd=[original[0],'-I'+str(S/'src')]
skip=False
for x in original[1:]:
 if skip: skip=False;continue
 if x=='-o':skip=True;continue
 if x.endswith('main.default.cpp') or x=='-fno-exceptions':continue
 cmd.append(x)
cmd+=['-fexceptions','-O1','-g','-fno-omit-frame-pointer','-fsanitize=thread',
 '-MD','-MF',str(out/'deps.d'),str(source),'-o',str(out/'test.o')]
link=[x for x in json.loads((base/'link.argv.json').read_text()) if not x.endswith('.o')]
link[1:1]=[str(out/'test.o'),'-Wl,--threads=4','-fsanitize=thread','-g','--rtlib=compiler-rt']
link[link.index('-o')+1]=str(out/'test')
record=dict(source=str(source),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
            frozen_headers=str(S),uninstrumented_providers='Existing paired LLVM/libc++/libc++abi/libunwind; host bridge/header coverage only.')
env=os.environ.copy()
env['TSAN_OPTIONS']='halt_on_error=1:exitcode=66:second_deadlock_stack=1:external_symbolizer_path='+str(Path(original[0]).parent/'llvm-symbolizer')
phases=[('compile',cmd),('link',link)]
if '--build-only' not in sys.argv[5:]:phases.append(('run',[str(out/'test')]))
for name,args in phases:
 with (out/(name+'.log')).open('w') as log:
  r=subprocess.run(args,cwd=R,stdout=log,stderr=subprocess.STDOUT,env=env,timeout=600)
 record[name]=dict(args=args,exit=r.returncode)
 (out/'report.json').write_text(json.dumps(record,indent=2)+'\n')
 if r.returncode:
  print(source.stem,name,r.returncode,(out/(name+'.log')).read_text()[-10000:])
  sys.exit(r.returncode)
print(source.stem,(out/'run.log').read_text() if '--build-only' not in sys.argv[5:] else 'built',flush=True)
