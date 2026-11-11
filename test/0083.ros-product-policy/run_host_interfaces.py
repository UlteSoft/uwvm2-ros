"""Exercise the public CLI and actual VM host gates with assembled Wasm.

Run inside the admitted 64 GiB/0 swap cgroup, using --output under uwvm2-ros/.tmp.
An architecture launcher may invoke QEMU for correctness, never performance.
"""
from pathlib import Path
import argparse
import hashlib
import json
import subprocess
import time
import os

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--runtime",required=True,type=Path)
    parser.add_argument("--output",required=True,type=Path)
    parser.add_argument("--sdk",required=True,type=Path)
    parser.add_argument("--engines",nargs="+",default=["interpreter","aot"])
    parser.add_argument("--threaded-engines",nargs="+",help="Explicitly qualified threaded backends; ordinary engines are always tested")
    parser.add_argument("--thread-start-repetitions",type=int,default=1,help="Repeat real threaded starts to detect admission/management lock races")
    args=parser.parse_args()
    if args.thread_start_repetitions < 1:parser.error("--thread-start-repetitions must be positive")
    args.threaded_engines=args.threaded_engines or args.engines;out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    rows=json.loads(Path(__file__).with_name("host-interface-signatures.json").read_text())
    sdk=args.sdk/"example/language/wat/library_kinds/static_math.wat"
    text=sdk.read_text();arithmetic=text[text.index("  (func"):text.rindex(")")]
    reports=[]
    def family(row):
        return "wasip1" if row["ns"] in ("wasi_snapshot_preview1","wasi") else "ros1" if row["ns"].startswith("uwvm_ros1_") else "ros2"
    def key(row):
        return row["name"] if family(row)=="wasip1" else row["ns"]+"."+row["name"]
    def declare(row,n):
        return '(import "'+row["ns"]+'" "'+row["name"]+'" (func $f'+str(n)+\
            (" (param "+" ".join(row["params"])+")" if row["params"] else "")+\
            (" (result "+" ".join(row["results"])+")" if row["results"] else "")+"))"
    def invoke(row,n):
        return " ".join(t+".const "+str(v) for t,v in zip(row["params"],row["args"]))+" call $f"+str(n)
    def compile(name,body):
        wat=out/(name+".wat");wasm=out/(name+".wasm")
        wat.write_text(body);subprocess.run(["wat2wasm","--enable-threads",str(wat),"-o",str(wasm)],check=True)
        return wasm
    def policy(selected,state):
        result=[]
        for group in sorted({family(row) for row in selected}):
            result+=["--"+group+"-single-create","main","--"+group+"-single-enable","main"]
        for row in selected:result+=["--"+family(row)+"-single-interface","main",key(row),state]
        return result
    def run(label,engine,flags,wasm,success=True,contains=(),absent=()):
        command=[str(args.runtime),"--wasm-set-main-module-name","main","--runtime-"+("int" if engine=="interpreter" else engine),*flags,"--run",str(wasm)]
        start=time.monotonic()
        value=subprocess.run(command,capture_output=True,timeout=120)
        stderr=value.stderr.decode(errors="replace")
        (out/(label+"-"+engine+".stderr")).write_bytes(value.stderr)
        row={"case":label,"engine":engine,"exit":value.returncode,"seconds":time.monotonic()-start,
             "argv":command,"stderr_sha256":hashlib.sha256(value.stderr).hexdigest()}
        reports.append(row)
        (out/"results.json").write_text(json.dumps({"cases":reports,"complete":False},indent=2))
        assert (value.returncode==0)==success,(label,engine,value.returncode,stderr[-2000:])
        for expected in contains:assert expected in stderr,(label,engine,expected,stderr[-2000:])
        for forbidden in absent:assert forbidden not in stderr,(label,engine,forbidden,stderr[-2000:])
        return value,row
    returning=[r for r in rows if r["results"] and r["ns"]!="wasi"]
    namespaces=set()
    for index,row in enumerate(rows):
        file=compile("api-"+str(index),"(module "+declare(row,0)+' (memory (export "memory") 1) (func (export "_start")))')
        group=family(row)
        if row["ns"] not in namespaces:
            run("namespace-absent-"+group+"-"+str(index),"interpreter",[],file,False,
                ("namespace permission is disabled","--"+group+("-global-enable" if group in ("ros1","ros2") else "-single-enable")))
            if row["ns"] != "wasi":
                run("validate-disabled-"+str(index),"interpreter",["--mode","validation"],file,False,
                    ("namespace permission is disabled",))
                run("validate-hidden-"+str(index),"interpreter",["--mode","validation",
                    "--"+group+"-single-create","main","--"+group+"-single-enable","main"],file,False,
                    ("interface exposure permission is hidden",))
                run("validate-deny-"+str(index),"interpreter",["--mode","validation",*policy([row],"deny")],file,
                    True,absent=("[warning]",))
                run("interface-without-namespace-"+str(index),"interpreter",["--"+group+"-single-create","main",
                    "--"+group+"-single-interface","main",key(row),"allow"],file,False,
                    ("namespace permission is disabled",))
            namespaces.add(row["ns"])
        # Each registered API is hidden even after the family is enabled.
        run("hidden-"+str(index),"interpreter",["--"+group+"-single-create","main","--"+group+"-single-enable","main"],file,
            False,("interface exposure permission is hidden","--"+group+"-single-interface 'main' "+key(row)))
    checks=[]
    for index,row in enumerate(returning):
        expected=76 if family(row)=="wasip1" else 13
        checks.append(invoke(row,index)+" "+row["results"][0]+".const "+str(expected)+" "+
                      row["results"][0]+".ne if unreachable end")
    all_denied=compile("all-denied","(module "+" ".join(declare(r,i) for i,r in enumerate(returning))+
        ' (memory (export "memory") 1) '+arithmetic+' (func (export "_start") i32.const 0 i64.const 123456789 i64.store '+
        " ".join(checks)+' i32.const 0 i64.load i64.const 123456789 i64.ne if unreachable end '+
        'i32.const 35 call $checksum_bias i32.const 42 i32.ne if unreachable end))')
    denied=policy(returning,"deny")
    for engine in args.engines:
        result,_=run("deny-all",engine,denied,all_denied,True,("wasip1.random_get denied","ros1.uwvm_ros1_v1.abi_version denied","ros2.uwvm_ros2_v1.abi_version denied"))
        assert result.stderr.count(b"[warning]")==len(returning),(engine,result.stderr.count(b"[warning]"))
        run("deny-silent",engine,denied+["--disable-warning","all_host_denied"],all_denied,True,absent=("[warning]",))
        result,_=run("deny-one",engine,denied+["--disable-warning","all_host_calls","--enable-warning","host_denied_wasip1.random_get"],all_denied,True,("wasip1.random_get denied",))
        assert result.stderr.count(b"[warning]")==1
        run("deny-fatal",engine,denied+["--warning-as-error","all_host_denied"],all_denied,False,("[error]",))
        for family_name in ("wasip1","ros1","ros2"):
            result,_=run("deny-"+family_name,engine,denied+["--disable-warning","all_host_calls","--enable-warning","all_host_denied_"+family_name],all_denied)
            assert result.stderr.count(b"[warning]")==sum(family(r)==family_name for r in returning)
    random=next(r for r in rows if r["name"]=="random_get")
    random["args"]=[32,16]
    for expected in (0,1,76):
        compile("random-"+str(expected),"(module "+declare(random,0)+' (memory (export "memory") 1) (func (export "_start") '+
            invoke(random,0)+" i32.const "+str(expected)+" i32.ne if unreachable end))")
    for engine in args.engines:
        allowed=policy([random],"allow")
        run("operation-denied",engine,allowed,out/"random-76.wasm",True,("--wasip1-single-grant 'main' random",))
        allowed+=["--wasip1-single-grant","main","random"]
        run("operation-unlimited",engine,allowed,out/"random-0.wasm",True,absent=("[warning]",))
        limited=allowed+["--wasip1-single-limit","main","random.request-bytes","8"]
        run("quota-silent",engine,limited,out/"random-1.wasm",True,absent=("[warning]",))
        run("quota-warning",engine,limited+["--enable-warning","all_host_limited_wasip1"],out/"random-1.wasm",True,("quota rejected: random.request-bytes",))
        run("quota-error",engine,limited+["--warning-as-error","host_limited_wasip1.random_get"],out/"random-1.wasm",False,("[error]",))
        twice=compile("random-twice","(module "+declare(random,0)+' (memory (export "memory") 1) (func (export "_start") '+
            (invoke(random,0)+" if unreachable end ")*2+"))")
        waiting=allowed+["--wasip1-single-limit","main","random.window-calls","1",
                         "--wasip1-single-limit-action","main","random.window-calls","wait",
                         "--enable-warning","host_limited_wasip1.random_get"]
        _,elapsed=run("quota-wait",engine,waiting,twice,True,("synchronously waiting for quota random.window-calls",))
        assert elapsed["seconds"]>=0.5,elapsed

    # Forwarded Wasm exports must resolve the immediate caller's contract.
    for sample in [random, next(r for r in rows if r["ns"]=="uwvm_ros1_v1" and r["name"]=="abi_version"),
                    next(r for r in rows if r["ns"]=="uwvm_ros2_v1" and r["name"]=="abi_version")]:
        group=family(sample);expected=76 if group=="wasip1" else 13
        worker=compile(group+"-worker","(module "+declare(sample,0)+
            ' (memory (export "memory") 1) (func (export "forward") (result i32) '+invoke(sample,0)+"))")
        caller=compile(group+"-caller",'(module (import "worker" "forward" (func $forward (result i32))) '+
            '(func (export "_start") call $forward i32.const '+str(expected)+' i32.ne if unreachable end))')
        own=policy([sample],"allow")
        if group=="wasip1":own+=["--wasip1-single-grant","main","random"]
        preload=["--wasm-preload-library",str(worker),"worker"]
        bound=["--"+group+"-single-create","worker","--"+group+"-single-enable","worker"]
        for engine in args.engines:
            run(group+"-no-caller-inheritance",engine,own+preload+bound,caller,False,
                ("interface exposure permission is hidden","'worker'"))
            worker_policy=bound+["--"+group+"-single-interface","worker",key(sample),"deny"]
            run(group+"-worker-denied",engine,own+preload+worker_policy,caller,True,("denied for single 'worker'",))
            group_policy=["--"+group+"-group-create","operators","--"+group+"-group-enable","operators",
                "--"+group+"-group-add-module","operators","worker",
                "--"+group+"-group-interface","operators",key(sample),"deny"]
            run(group+"-worker-group",engine,own+preload+group_policy,caller,True,("denied for group 'operators'",))
            if group in ("ros1","ros2"):
                global_policy=["--"+group+"-global-enable","--"+group+"-global-interface",key(sample),"deny"]
                run(group+"-global-default-member",engine,own+preload+global_policy,caller,True,
                    ("denied for global",))
                run(group+"-global-bound",engine,own+preload+global_policy+
                    ["--"+group+"-global-add-module","worker"],caller,True,("denied for global",))
            if group=="wasip1":
                worker_policy[-1]="allow"
                run("wasip1-resource-not-inherited",engine,own+preload+worker_policy,caller,True,("--wasip1-single-grant 'worker' random",))
    thread_memory=compile("thread-memory",'(module (memory (export "memory") 4 256 shared))')
    thread_flags=["--wasm-feature-enable-threads", "--wasi-threads", "1", "1", "1048576", "16777216", "8388608", "16777216",
                  "--wasm-preload-library",str(thread_memory),"env"]
    for row in (r for r in rows if not r["results"] or r["ns"]=="wasi"):
        body=invoke(row,0)+(" i32.const -1 i32.ne if unreachable end" if row["results"] else "")
        memory=' (memory (export "memory") 1) ' if row["ns"]!="wasi" else ' (import "env" "memory" (memory 4 256 shared)) (export "memory" (memory 0)) (func (export "wasi_thread_start") (param i32 i32)) '
        file=compile("special-"+row["name"],"(module "+declare(row,0)+memory+' (func (export "_start") '+body+"))")
        for engine in (args.threaded_engines if row["ns"]=="wasi" else args.engines):
            for iteration in range(args.thread_start_repetitions if row["ns"]=="wasi" else 1):
                label="denied-"+row["name"]+("-start-"+str(iteration) if iteration else "")
                run(label,engine,policy([row],"deny")+ (thread_flags if row["ns"]=="wasi" else []),file,bool(row["results"]),
                    ("wasip1."+row["name"]+" denied","'main'"))
    # The separately admitted thread adapter must preserve exposed-deny semantics.
    threaded_random=compile("thread-random","(module "+declare(random,0)+
        ' (import "env" "memory" (memory 4 256 shared)) (export "memory" (memory 0)) (func (export "wasi_thread_start") (param i32 i32))'+
        ' (func (export "_start") '+invoke(random,0)+' i32.const 76 i32.ne if unreachable end))')
    for engine in args.threaded_engines:
        run("threaded-wasi-denied",engine,thread_flags+policy([random],"deny"),threaded_random,True,("wasip1.random_get denied",))
    # Logging is observational. A closed/full sink must not turn EACCES into a host crash.
    for mode in ("closed-pipe","full-pipe"):
        readfd,writefd=os.pipe2(os.O_NONBLOCK)
        if mode=="closed-pipe":os.close(readfd)
        else:
            try:
                while True:os.write(writefd,b"x"*4096)
            except BlockingIOError:pass
        command=[str(args.runtime),"--wasm-set-main-module-name","main",*policy([random],"deny"),
                 "--run",str(out/"random-76.wasm")]
        value=subprocess.run(command,stdout=subprocess.DEVNULL,stderr=writefd,timeout=30)
        os.close(writefd)
        if mode!="closed-pipe":os.close(readfd)
        assert value.returncode==0,(mode,value.returncode)
        reports.append({"case":mode,"exit":value.returncode})
    logfile=out/"configured-diagnostic.log"
    value,_=run("configured-log-output","interpreter",policy([random],"deny")+["--log-output","file",str(logfile)],
        out/"random-76.wasm")
    assert b"[warning]" not in value.stderr and "wasip1.random_get denied" in logfile.read_text()
    (out/"results.json").write_text(json.dumps({"complete":True,"cases":reports,"registered":len(rows),
        "engines":args.engines,"threaded_engines":args.threaded_engines,"thread_start_repetitions":args.thread_start_repetitions,"sdk_example":str(sdk),"sdk_example_sha256":hashlib.sha256(sdk.read_bytes()).hexdigest()},indent=2))
    print(json.dumps({"complete":True,"cases":len(reports),"registered":len(rows)}))
if __name__=="__main__":main()
