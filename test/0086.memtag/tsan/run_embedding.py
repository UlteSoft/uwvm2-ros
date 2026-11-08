#!/usr/bin/env python3
"""Run real B1 lifecycle controls with an owned process deadline and physical reap."""
from pathlib import Path
import argparse, ctypes, hashlib, json, os, re, resource, signal, subprocess, time

parser=argparse.ArgumentParser()
parser.add_argument("--embedding",type=Path,required=True,help="Directory produced by build_embedding.py")
parser.add_argument("--wasm-tools",type=Path,required=True)
parser.add_argument("--output",type=Path,required=True)
parser.add_argument("--race-repeats",type=int,default=10)
parser.add_argument("--case",choices=["none","control","race"],action="append")
parser.add_argument("--engine",choices=["int","llvm"],action="append")
parser.add_argument("--deadline",type=float,default=60)
parser.add_argument("--rss-limit-mib",type=int,default=2048)
a=parser.parse_args()
R=Path(__file__).resolve().parents[3];V=a.embedding.resolve();O=a.output.resolve();tool=a.wasm_tools.resolve()
assert O.is_relative_to(R/".tmp") and not O.exists(),"Use a new artifact directory under .tmp"
assert a.race_repeats>0 and 0<a.deadline<=180 and 128<=a.rss_limit_mib<=4096
assert 0 not in os.sched_getaffinity(0)
cgroup=Path("/sys/fs/cgroup")/Path("/proc/self/cgroup").read_text().split("::",1)[1].strip().lstrip("/")
assert (cgroup/"memory.max").read_text().strip()=="68719476736" and (cgroup/"memory.swap.max").read_text().strip()=="0"
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
libc=ctypes.CDLL(None,use_errno=True)
assert libc.prctl(36,1,0,0,0)==0,"PR_SET_CHILD_SUBREAPER failed"
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
receipt=json.loads((V/"receipt.json").read_text());binary=V/"embedding"
assert receipt["status"]=="built" and sha(binary)==receipt["binary_sha256"]
O.mkdir(parents=True);(O/"ir").mkdir()
fixtures=V/"inputs/test/0050.high_level_languages/threaded_profile/debug-control-b1"
for name in ["memory_provider","b1_wait_and_loop"]:
 subprocess.run([str(tool),"parse",str(fixtures/(name+".wat")),"-o",str(O/(name+".wasm"))],check=True,timeout=30)
compile_cmd=json.loads((V/"compile.argv.json").read_text());symbolizer=Path(compile_cmd[0]).parent/"llvm-symbolizer"
env=dict(os.environ,TSAN_OPTIONS="halt_on_error=1:exitcode=66:second_deadlock_stack=1:external_symbolizer_path="+str(symbolizer),UWVM_TSAN_IR_DIRECTORY=str(O/"ir"))
rows=[]
record=dict(status="running",runner_sha256=sha(__file__),embedding_receipt=str(V/"receipt.json"),embedding_receipt_sha256=sha(V/"receipt.json"),binary_sha256=sha(binary),
 wasm_tools=str(tool),wasm_tools_sha256=sha(tool),fixtures={p.name:sha(p) for p in O.glob("*.wasm")},cpu_affinity=sorted(os.sched_getaffinity(0)),
 boot_id=Path("/proc/sys/kernel/random/boot_id").read_text().strip(),cgroup=str(cgroup),cgroup_inode=cgroup.stat().st_ino,
 rss_limit_mib=a.rss_limit_mib,sanitizer_options=env["TSAN_OPTIONS"],cache="disabled explicitly for every run",rows=rows,
 limits="Native x86_64, frozen source2, real B1 shared atomic wait/loop; race mode attempts concurrent reset/terminal retire but does not witness its exact narrow scheduling window. No Guest-language race-free or logical MTE lifetime claim.")
def save():(O/"report.json").write_text(json.dumps(record,indent=2)+"\n")
def proc_info(pid):
 try:
  stat=Path("/proc")/str(pid)/"stat";raw=stat.read_text();tail=raw[raw.rfind(")")+2:].split()
  return dict(pid=pid,state=tail[0],ppid=int(tail[1]),pgid=int(tail[2]),start_ticks=int(tail[19]))
 except (OSError,ValueError):return None

def group_members(pgid):
 answer=[]
 for path in Path("/proc").iterdir():
  if path.name.isdigit():
   info=proc_info(int(path.name))
   if info and info["pgid"]==pgid:answer.append(info)
 return answer

save()
for mode in a.case or ["none","control","race"]:
 for engine in a.engine or ["int","llvm"]:
  for repeat in range(a.race_repeats if mode=="race" else 1):
   name=f"{mode}-{engine}-{repeat:02d}"
   args=[str(binary),mode,engine,"--","-R"+("aot" if engine=="llvm" else "int"),
    "--debug-permissions","full","--runtime-compile-threads","0","--runtime-llvm-jit-cache-path","disable","--runtime-llvm-jit-policy","debug",
    "-WFE-threads","--wasi-threads","3","3","1048576","16777216","67108864","131072",
    "--wasm-set-main-module-name","main","--wasip1-single-create","main","--wasip1-single-enable","main",
    "--wasip1-single-interface","main","thread-spawn","allow","--wasip1-single-grant","main","thread-spawn",
    "--wasm-preload-library",str(O/"memory_provider.wasm"),"env","--run",str(O/"b1_wait_and_loop.wasm")]
   started=time.monotonic();timed_out=False;rss_exceeded=False;maximum_rss_kib=0
   with (O/(name+".stdout")).open("wb") as stdout,(O/(name+".stderr")).open("wb") as stderr:
    child=subprocess.Popen(args,stdout=stdout,stderr=stderr,env=env,start_new_session=True)
    identity=proc_info(child.pid);pidfd=os.pidfd_open(child.pid)
    try:
     while child.poll() is None:
      try:
       status=(Path("/proc")/str(child.pid)/"status").read_text()
       match=re.search(r"^VmHWM:\s+(\d+) kB",status,re.M)
       if match:maximum_rss_kib=max(maximum_rss_kib,int(match[1]))
      except OSError:pass
      if time.monotonic()-started>a.deadline or maximum_rss_kib>a.rss_limit_mib*1024:
       timed_out=time.monotonic()-started>a.deadline
       rss_exceeded=maximum_rss_kib>a.rss_limit_mib*1024
       os.killpg(child.pid,signal.SIGTERM)
       try:child.wait(timeout=2)
       except subprocess.TimeoutExpired:os.killpg(child.pid,signal.SIGKILL);child.wait(timeout=5)
       break
      time.sleep(0.01)
     code=child.wait()
    finally:os.close(pidfd)
   # Reap adopted descendants, if any; no detached worker is accepted as success.
   for _ in range(20):
    try:reaped,_=os.waitpid(-child.pid,os.WNOHANG)
    except ChildProcessError:break
    if reaped==0:break
   remaining=group_members(child.pid)
   if remaining:
    os.killpg(child.pid,signal.SIGKILL)
    for _ in range(50):
     try:reaped,_=os.waitpid(-child.pid,os.WNOHANG)
     except ChildProcessError:break
     if reaped==0:time.sleep(0.01)
   after=group_members(child.pid)
   text=(O/(name+".stderr")).read_text(errors="replace");output=(O/(name+".stdout")).read_text(errors="replace")
   workers=[dict(zip(["native_tid","participant","incarnation","backend"],map(int,m))) for m in re.findall(r"TSAN_EMBED_WORKER native_tid=(\d+) participant=(\d+) incarnation=(\d+) backend=(\d+)",text)]
   jit=[line for line in text.splitlines() if line.startswith("TSAN_TEST_IR")]
   sanitizer=bool(re.search(r"(?:WARNING|FATAL): ThreadSanitizer:|ThreadSanitizer: (?:DEADLYSIGNAL|CHECK failed)",text))
   passed=code==0 and not timed_out and not rss_exceeded and not sanitizer and not remaining and not after and f"B1 SOURCE CASE {mode}:" in output
   if mode!="none":passed=passed and len(workers)==2 and len({w["native_tid"] for w in workers})==2
   if engine=="llvm" and mode!="none":passed=passed and len(jit)>=2
   retirement=[line for line in text.splitlines() if line.startswith("TSAN_EMBED_RETIRE")]
   result=[line for line in text.splitlines() if line.startswith("TSAN_EMBED_RESULT")]
   passed=passed and bool(retirement)
   rows.append(dict(name=name,argv=args,process_identity=identity,process_reaped=True,timeout=timed_out,exit=code,passed=passed,
    retirement_receipt_lines=retirement,result_receipt_lines=result,
    maximum_rss_kib=maximum_rss_kib,rss_exceeded=rss_exceeded,seconds=time.monotonic()-started,remaining_group_before_cleanup=remaining,remaining_group_after_cleanup=after,
    sanitizer_report=sanitizer,workers=workers,jit_receipt_lines=jit,stdout=output.strip(),
    checked_physical_join=code==0 and f"B1 SOURCE CASE {mode}:" in output))
   save();print(name,"exit",code,"passed",passed,"workers",len(workers),"jit",len(jit),flush=True)
   if not passed:
    record.update(status="failed",failed_case=name,passed=sum(row["passed"] for row in rows),total=len(rows),ir_files={p.name:sha(p) for p in (O/"ir").iterdir()})
    save();print(text[-12000:],flush=True);raise SystemExit(1)
record.update(status="complete",passed=sum(row["passed"] for row in rows),total=len(rows),ir_files={p.name:sha(p) for p in (O/"ir").iterdir()})
save();print("summary",record["passed"],"/",len(rows),flush=True)
raise SystemExit(not all(row["passed"] for row in rows))
