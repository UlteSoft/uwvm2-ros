#!/usr/bin/env python3
"""Compile the real B1 embedding main against an already instrumented frozen VM."""
from pathlib import Path
import argparse, difflib, hashlib, json, os, resource, shutil, subprocess, time

parser=argparse.ArgumentParser()
parser.add_argument("--instrumented-vm",type=Path,required=True)
parser.add_argument("--output",type=Path,required=True)
parser.add_argument("--prepare-only",action="store_true")
a=parser.parse_args()
R=Path(__file__).resolve().parents[3];D=Path(__file__).resolve().parent
V=a.instrumented_vm.resolve();O=a.output.resolve()
assert O.is_relative_to(R/".tmp") and not O.exists(), "Use a new artifact directory under .tmp"
assert 0 not in os.sched_getaffinity(0)
cgroup=Path("/sys/fs/cgroup")/Path("/proc/self/cgroup").read_text().split("::",1)[1].strip().lstrip("/")
assert (cgroup/"memory.max").read_text().strip()=="68719476736"
assert (cgroup/"memory.swap.max").read_text().strip()=="0"
assert os.uname().machine=="x86_64"
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
previous=json.loads((V/"receipt.json").read_text());S=Path(previous["source_root"])
assert previous["status"]=="built"
snapshot=previous["source"]
for relative,digest in snapshot.items():assert sha(S/relative)==digest,(relative,"snapshot no longer matches VM receipt")
O.mkdir(parents=True)
fixture=R/"test/0050.high_level_languages/threaded_profile/debug-control-b1"
inputs=[D/"embedding_lifecycle.cc",fixture/"after_join_shared_memory.h",fixture/"b1_native_embedding.cc",fixture/"b1_wait_and_loop.wat",fixture/"memory_provider.wat"]
for src in inputs:
 dest=O/"inputs"/src.relative_to(R);dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(src,dest)
source=O/"inputs"/(D/"embedding_lifecycle.cc").relative_to(R)
diff="".join(difflib.unified_diff((fixture/"b1_native_embedding.cc").read_text().splitlines(keepends=True),source.read_text().splitlines(keepends=True),fromfile="original-b1-native-embedding.cc",tofile="tsan-embedding-lifecycle.cc"))
(O/"original-to-test.patch").write_text(diff)
cmd=json.loads((V/"main.default.cpp.argv.json").read_text())
assert "-fsanitize=thread" in cmd and "-DUWVM_TEST_TSAN_JIT=1" in cmd
main=next(x for x in cmd if x.endswith("/src/uwvm2/uwvm/main.default.cpp"))
compile_root=str(Path(main).parents[3])
cmd=[x.replace(compile_root,str(S)) for x in cmd]
cmd[cmd.index(str(S/"src/uwvm2/uwvm/main.default.cpp"))]=str(source)
cmd[cmd.index("-o")+1]=str(O/"main.o");cmd[cmd.index("-MF")+1]=str(O/"main.d")
(O/"compile.argv.json").write_text(json.dumps(cmd,indent=2)+"\n")
reused={}
for name in ["host_api.default.cpp","uwvm_runtime.default.cpp"]:
 entry=previous["project_objects"][name];obj=V/(name+".o")
 assert entry["instrumented"] and entry["exit"]==0 and sha(obj)==entry["sha256"]
 reused[name]={"object":str(obj),"sha256":sha(obj),"instrumented":True}
link=json.loads((V/"link.argv.json").read_text())
assert "-fsanitize=thread" in link
assert link.count(str(V/"main.default.cpp.o"))==1
link=[str(O/"main.o") if x==str(V/"main.default.cpp.o") else x for x in link]
link[link.index("-o")+1]=str(O/"embedding")
link+= ["-Wl,-Map="+str(O/"link.map")]
(O/"link.argv.json").write_text(json.dumps(link,indent=2)+"\n")
record=dict(status="prepared",source_root=str(S),vm_receipt=str(V/"receipt.json"),vm_receipt_sha256=sha(V/"receipt.json"),
 builder_sha256=sha(__file__),test_adaptation_patch_sha256=sha(O/"original-to-test.patch"),source=str(source),inputs={str(p.relative_to(R)):sha(p) for p in inputs},source_sha256=sha(source),reused_instrumented_objects=reused,
 cpu_affinity=sorted(os.sched_getaffinity(0)),cgroup=str(cgroup),cgroup_inode=cgroup.stat().st_ino,
 boot_id=Path("/proc/sys/kernel/random/boot_id").read_text().strip(),
 limits={"memory.max":68719476736,"memory.swap.max":0},
 coverage="Real B1 owned-source/debug-stop/reset/retire/drain APIs; original B1 sources remain unexecuted outside this private adaptation.",
 limitations=previous.get("third_party_instrumentation",previous.get("providers","External providers not fully instrumented; no suppression.")))
def save():(O/"receipt.json").write_text(json.dumps(record,indent=2)+"\n")
save()
if a.prepare_only:print("PREPARED",O);raise SystemExit(0)
for phase,arguments in [("compile",cmd),("link",link)]:
 began=time.time()
 with (O/(phase+".log")).open("w") as log:
  proc=subprocess.Popen(arguments,cwd=S,stdout=log,stderr=subprocess.STDOUT)
  record.update(status=phase+"_running",active={"pid":proc.pid,"phase":phase,"started":began});save()
  print("START",phase,"pid",proc.pid,flush=True);code=proc.wait()
 record.pop("active",None);record[phase]={"exit":code,"seconds":time.time()-began};save()
 if code:
  record["status"]=phase+"_failed";save();print((O/(phase+".log")).read_text()[-16000:]);raise SystemExit(code)
 assert sha(source)==record["source_sha256"]
 for relative,digest in record["inputs"].items():assert sha(O/"inputs"/relative)==digest,(relative,"frozen test input mutated")
 for relative,digest in snapshot.items():assert sha(S/relative)==digest,(relative,"snapshot mutated during build")
 for name,entry in reused.items():assert sha(entry["object"])==entry["sha256"]
print("LINKED",O/"embedding",flush=True)
record.update(status="built",binary=str(O/"embedding"),binary_sha256=sha(O/"embedding"),main_sha256=sha(O/"main.o"),maximum_child_rss_kib=resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss)
save()
