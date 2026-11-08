from pathlib import Path
import sys,json,subprocess,struct,time,hashlib
R=Path(__file__).resolve().parents[3]
vm=Path(sys.argv[1]).resolve();out=Path(sys.argv[2]).resolve();out.mkdir(parents=True,exist_ok=True)
assert out.is_relative_to(R/'.tmp'), 'Logs must stay below repository .tmp'
manifest=json.loads(Path(sys.argv[3]).read_text());rows=[]
for case in manifest:
 for enabled in ([False,True] if case['extension'] else [False]):
  for engine in ['int','aot']:
   name=case['name']+'-'+str(int(enabled))+'-'+engine
   cmd=[str(vm),'-R'+engine,'--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',
    '--runtime-llvm-jit-policy','debug','-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','131072',
    '--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main']
   for interface in ['thread-spawn','fd_write','proc_exit']:cmd+=['--wasip1-single-interface','main',interface,'allow']
   for grant in ['thread-spawn','stdout.write','proc-exit']:cmd+=['--wasip1-single-grant','main',grant]
   if case['extension'] and not enabled:cmd+=['--runtime-disable-memtag']
   cmd+=['--wasm-preload-library',case['owner'],'env','--run',case['file']]
   start=time.monotonic();r=subprocess.run(cmd,capture_output=True,timeout=180)
   (out/(name+'.stderr')).write_bytes(r.stderr);(out/(name+'.stdout')).write_bytes(r.stdout)
   fault=case['kind']=='retag-stale' and enabled
   expected=122 if fault else 0
   counter=int.from_bytes(r.stdout,'little') if len(r.stdout)==4 else None
   passed=b'WARNING: ThreadSanitizer:' not in r.stderr and r.returncode==expected and (b'memory tag mismatch' in r.stderr.lower() if fault else counter is not None)
   if not fault and case['kind']!='ordinary-race':passed=passed and counter==(777 if case['kind']=='retag-stale' else 40000)
   if case['kind']=='ordinary-race':passed=passed and counter is not None and 0<counter<=40000
   rows.append(dict(name=name,argv=cmd,exit=r.returncode,expected=expected,counter=counter,passed=passed,
    seconds=time.monotonic()-start,sanitizer_report=b'WARNING: ThreadSanitizer:' in r.stderr,
    scope='deliberate guest ordinary race; host atomic lowering may hide it from TSan' if case['kind']=='ordinary-race' else 'actual Wasm thread synchronization and MTE effects',
    jit_receipt_lines=[l for l in r.stderr.decode(errors='replace').splitlines() if l.startswith('TSAN_TEST_IR')]))
   (out/'report.json').write_text(json.dumps(rows,indent=2)+'\n');print(name,r.returncode,counter,passed,flush=True)
print('summary',sum(r['passed'] for r in rows),'/',len(rows),flush=True)
raise SystemExit(not all(r['passed'] for r in rows))
