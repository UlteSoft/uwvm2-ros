from pathlib import Path
import json,subprocess,sys,time,threading,selectors,os
R=Path(__file__).resolve().parents[3];B=Path(sys.argv[2]).resolve();B.mkdir(parents=True,exist_ok=True);vm=Path(sys.argv[1]).resolve()
assert B.is_relative_to(R/'.tmp'), 'Outputs must stay under repository .tmp'
parser=Path(sys.argv[3]).resolve()
memory=B/'memory.wat';memory.write_text('(module (memory (export "memory") 1 1 shared))')
subprocess.run([str(parser),'parse',str(memory),'-o',str(memory.with_suffix('.wasm'))],check=True)
rows=[]
for engine in ('int','aot'):
 for action in ('cancel','wait'):
  for kind in ('quota','native'):
   name=engine+'-'+kind+'-'+action;directory=B/name;directory.mkdir(exist_ok=True)
   source=directory/'main.wat'
   source.write_text('''(module
(import "env" "memory" (memory 1 1 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
(import "wasi_snapshot_preview1" "fd_renumber" (func $renumber (param i32 i32)(result i32)))
(import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32)(result i32)))
(import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
(export "memory" (memory 0))
(func (export "wasi_thread_start") (param i32 i32)
 (drop (call $write (i32.const 2)(i32.const 80)(i32.const 1)(i32.const 88)))
 (drop (memory.atomic.wait32 (i32.const 64)(i32.const 0)(i64.const 100000000)))
 (call $exit (i32.const 22)))
(func (export "_start")
 (if (call $renumber (i32.const 1)(i32.const 3))(then (call $exit (i32.const 101))))
 (i32.store (i32.const 80)(i32.const 96))
 (i32.store (i32.const 84)(i32.const 1))
 (i32.store8 (i32.const 96)(i32.const 0))
 (i32.store (i32.const 0)(i32.const 1024))
 (i32.store (i32.const 4)(i32.const '''+('1' if kind=='quota' else '60000')+'''))
 (if (i32.lt_s (call $spawn (i32.const 0))(i32.const 0))(then (call $exit (i32.const 102))))
 (loop $again
   (drop (call $write (i32.const 3)(i32.const 0)(i32.const 1)(i32.const 16)))
   (br $again))))''')
   wasm=source.with_suffix('.wasm');subprocess.run([str(parser),'parse',str(source),'-o',str(wasm)],check=True)
   args=[str(vm),'-R'+engine,'--runtime-compile-threads','0','--runtime-llvm-jit-policy','debug','--runtime-llvm-jit-cache-path','disable',
    '-WFE-threads','--wasi-threads','1','1','1048576','16777216','67108864','65536','--wasm-set-main-module-name','main',
    '--wasip1-single-create','main','--wasip1-single-enable','main',
    '--wasip1-single-mount-dir','main','/target',str(directory),'--wasip1-single-mount-dir-rights','main','/target','0','0']
   for n in ('proc-exit','stdout.write','stderr.write','thread-spawn'):args+=['--wasip1-single-grant','main',n]
   for n in ('proc_exit','fd_write','fd_renumber','thread-spawn'):args+=['--wasip1-single-interface','main',n,'allow']
   if kind=='quota':args+=['--wasip1-single-limit','main','stdout.window-calls','1','--wasip1-single-limit-action','main','stdout.window-calls','wait']
   args+=['--wasip1-single-limit-exit-action','main','stdout.'+('window-calls' if kind=='quota' else 'native-wait'),action,
    '--wasm-preload-library',str(memory.with_suffix('.wasm')),'env','--run',str(wasm)]
   start=time.monotonic();p=subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
   # Start cancellation timing after the actual guest child is running;
   # QEMU cold compilation is not a native performance measurement.
   try:
    # TSan diagnostics/IR receipts precede Guest stderr. Preserve them while
    # waiting for the actual child marker, not the first host diagnostic byte.
    prefix=bytearray();deadline=time.monotonic()+180
    with selectors.DefaultSelector() as selected:
     selected.register(p.stderr,selectors.EVENT_READ)
     while b'\0' not in prefix:
      remaining=deadline-time.monotonic()
      assert remaining>0 and selected.select(remaining),'Guest did not publish its readiness marker'
      chunk=os.read(p.stderr.fileno(),4096)
      assert chunk,'Guest exited before readiness marker'
      prefix.extend(chunk)
    ready=time.monotonic()
    # Withhold reads so native writes genuinely block on a full host pipe.
    if kind=='native' and action=='wait':
     time.sleep(.35);alive=p.poll() is None
     out,err=p.communicate(timeout=10)
    else:
     p.wait(timeout=10);alive=None;out,err=p.communicate()
   except BaseException:
    p.kill();p.communicate();raise
   elapsed=time.monotonic()-start
   after_ready=time.monotonic()-ready
   row=dict(name=name,exit=p.returncode,seconds=elapsed,seconds_after_guest_ready=after_ready,output_bytes=len(out),waiting_before_drain=alive,argv=args,stderr=(bytes(prefix)+err).decode(errors='replace'),sanitizer_report=b'WARNING: ThreadSanitizer:' in bytes(prefix)+err)
   row['pass']=not row['sanitizer_report'] and p.returncode==22 and (action!='cancel' or after_ready<.9) and (kind!='quota' or action!='wait' or elapsed>=.9) and (alive is not False)
   rows.append(row);(B/'report.json').write_text(json.dumps(rows,indent=2))
   print(name,p.returncode,round(elapsed,3),len(out),row['pass'],flush=True);assert row['pass'],row
