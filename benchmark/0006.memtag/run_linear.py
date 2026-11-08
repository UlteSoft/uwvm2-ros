#!/usr/bin/env python3
"""Native x86_64 process-level comparison; no QEMU performance claims."""
import argparse,hashlib,json,os,platform,resource,statistics,subprocess,time,fcntl
from pathlib import Path
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument("--fixtures",type=Path,required=True);p.add_argument("--vm",type=Path,required=True);p.add_argument("--wavm",type=Path,required=True);p.add_argument("--wasmtime",type=Path);p.add_argument("--out",type=Path,required=True);p.add_argument("--repeat",type=int,default=5);p.add_argument("--cpu",type=int,default=2);a=p.parse_args()
 assert platform.machine()=="x86_64" and a.cpu!=0 and a.repeat>=3
 membership=Path("/proc/self/cgroup").read_text().strip();group=Path("/sys/fs/cgroup"+membership.split("::",1)[1])
 assert (group/"memory.max").read_text().strip()==str(64<<30) and (group/"memory.swap.max").read_text().strip()=="0"
 assert a.cpu in os.sched_getaffinity(0);os.sched_setaffinity(0,{a.cpu})
 a.out.mkdir(parents=True,exist_ok=True)
 lock=(a.out/"benchmark.lock").open("a");fcntl.flock(lock,fcntl.LOCK_EX|fcntl.LOCK_NB)
 shared_lock=None
 if Path("/tmp/uwvm-hll-benchmark.lock").exists():
  shared_lock=Path("/tmp/uwvm-hll-benchmark.lock").open("rb");fcntl.flock(shared_lock,fcntl.LOCK_EX|fcntl.LOCK_NB)
 manifest=json.loads((a.fixtures/"manifest.json").read_text())
 for row in manifest:assert sha(row["file"])==row["sha256"]
 metadata=dict(scope="complete process including cold parse, initialization, JIT, kernel and shutdown; no startup subtraction",clock="CLOCK_MONOTONIC",cpu=a.cpu,cgroup=membership,boot=Path("/proc/sys/kernel/random/boot_id").read_text().strip(),vm_sha256=sha(a.vm),wavm_sha256=sha(a.wavm),fixtures=manifest)
 (a.out/"metadata.json").write_text(json.dumps(metadata,indent=2))
 def command(bits,iterations,engine):
  tagged=engine not in ["uwvm-ordinary","wavm-ordinary","wasmtime-ordinary"]
  source=next(x for x in manifest if x["bits"]==bits and x["iterations"]==iterations and x["tagged"]==tagged)["file"]
  if engine.startswith("uwvm"):
   cmd=[str(a.vm),"-Raot","--runtime-compile-threads","0","--runtime-llvm-jit-cache-path","disable"]
   if bits==64:cmd+=["--wasm-feature-enable-memory64","--wasm-feature-enable-table64"]
   if engine=="uwvm-ignored":cmd+=["--runtime-disable-memtag"]
   cmd+=["--run",source]
  elif engine.startswith("wavm"):
   cmd=[str(a.wavm),"run","--abi=bare","--function=_start","--nocache"]
   if bits==64:cmd+=["--enable","memory64","--enable","table64"]
   if engine=="wavm-basic":cmd+=["--enable","memtag-soft"]
   if engine=="wavm-full":cmd+=["--enable","memtag-soft-debug"]
   cmd+=[source]
  else:
   cmd=[str(a.wasmtime),"run","-C","cache=n","-O","opt-level=2"]
   if bits==64:cmd+=["-W","memory64=y"]
   cmd+=[source]
  return cmd
 engines=["uwvm-ordinary","uwvm-ignored","uwvm-enabled","wavm-ordinary","wavm-ignored","wavm-basic","wavm-full"]
 if a.wasmtime:engines.append("wasmtime-ordinary")
 rows=[]
 for bits in [32,64]:
  for iterations,repeats in [(0,1),(268435456,a.repeat)]:
   for repeat in range(repeats):
    order=engines if repeat%2==0 else list(reversed(engines))
    for engine in order:
     cmd=command(bits,iterations,engine);before=resource.getrusage(resource.RUSAGE_CHILDREN);start=time.monotonic_ns()
     process=subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=180)
     duration=(time.monotonic_ns()-start)/1e9;after=resource.getrusage(resource.RUSAGE_CHILDREN)
     name=f"{bits}-{iterations}-{repeat}-{engine}";(a.out/(name+".log")).write_bytes(process.stdout)
     row=dict(name=name,bits=bits,iterations=iterations,rounds=16,repeat=repeat,engine=engine,exit=process.returncode,wall_seconds=duration,user_seconds=after.ru_utime-before.ru_utime,system_seconds=after.ru_stime-before.ru_stime,involuntary_switches=after.ru_nivcsw-before.ru_nivcsw,args=cmd)
     rows.append(row);(a.out/"runs.json").write_text(json.dumps(rows,indent=2))
     print(name,process.returncode,round(duration,6),flush=True)
     if process.returncode:raise RuntimeError(process.stdout.decode(errors="replace")[-3000:])
 summary=[]
 for bits in [32,64]:
  for engine in engines:
   samples=[x["wall_seconds"] for x in rows if x["bits"]==bits and x["engine"]==engine and x["iterations"]]
   summary.append(dict(bits=bits,engine=engine,median=statistics.median(samples),minimum=min(samples),maximum=max(samples),samples=samples))
 (a.out/"summary.json").write_text(json.dumps(summary,indent=2))
if __name__=="__main__":main()
