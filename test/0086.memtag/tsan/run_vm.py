#!/usr/bin/env python3
from pathlib import Path
import os,sys,resource
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
arguments=sys.argv[1:]
key="--runtime-llvm-jit-cache-path"
found=False
for i,value in enumerate(arguments):
    if value.startswith(key+"="):
        raise SystemExit("TSan qualification requires separate --runtime-llvm-jit-cache-path disable arguments")
    if value==key:
        if i+1==len(arguments) or arguments[i+1]!="disable":
            raise SystemExit("TSan qualification rejects persistent JIT cache")
        found=True
if not found:arguments=[key,"disable"]+arguments
base=Path(__file__).resolve().parent
os.environ["TSAN_OPTIONS"]="halt_on_error=1:exitcode=66:second_deadlock_stack=1:external_symbolizer_path=@SYMBOLIZER@"
os.environ["UWVM_TSAN_IR_DIRECTORY"]=str(base.parent/"ir")
print("TSAN_TEST persistent_cache=disabled host_project_tus=all guest_shared_jit=function-pass",file=sys.stderr,flush=True)
os.execv(str(base/"uwvm2-ros"),[str(base/"uwvm2-ros"),*arguments])
