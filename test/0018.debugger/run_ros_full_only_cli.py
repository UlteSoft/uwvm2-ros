#!/usr/bin/env python3
"""Actual ROS full-only CLI surfaces and interpreter/LLVM dispatch, Linux cgroup only."""
from pathlib import Path
import argparse, hashlib, json, os, re, socket, subprocess, sys


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ["source-root","binary","wasm-tools","out"]:parser.add_argument("--"+name,type=Path,required=True)
    a=parser.parse_args();assert sys.platform=="linux"
    subprocess.run(["bash",str(a.source_root/"tools/ci/require_wasm3_test_cgroup.sh")],check=True)
    a.out.mkdir(parents=True,exist_ok=False);rows=[]
    def run(name,argv,pass_fds=()):
        wrapper=[sys.executable,"-c","import os,sys,time;time.sleep(.15);os.execvpe(sys.argv[1],sys.argv[1:],os.environ)",*map(str,argv)]
        result=subprocess.run(wrapper,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,pass_fds=pass_fds,timeout=30)
        (a.out/(name+".log")).write_bytes(result.stdout)
        rows.append({"name":name,"argv":list(map(str,argv)),"exit":result.returncode})
        return result
    def check(result,predicate):
        rows[-1]["passed"]=bool(predicate);assert predicate,(rows[-1],result.stdout[-2000:])
    try:
        help_result=run("help",[a.binary,"--help","all"])
        text=re.sub(rb"\x1b\[[0-9;]*[A-Za-z]",b"",help_result.stdout)
        check(help_result,help_result.returncode==0 and all(x in text for x in [b"--runtime-aot",b"-Raot",b"--runtime-int",b"-Rint",b"-Rdbg"]))
        removed=["-Rjit","-Rcm","-Rcc","-Rtiered","-RDint","-Rllvm-lazy-policy"]
        for option in removed:
            assert not re.search(rb"(?<![A-Za-z0-9_-])"+option.encode()+rb"(?![A-Za-z0-9_-])",text),option
            result=run("reject-"+option[1:],[a.binary,option])
            check(result,result.returncode==126 and b"invalid parameter:" in result.stdout and option.encode() in result.stdout)
        wat=a.out/"full-only.wat";wasm=a.out/"full-only.wasm"
        wat.write_text('(module (func (export "_start") i32.const 17 i32.const 19 i32.add i32.const 36 i32.ne if unreachable end))\n')
        result=run("parse",[a.wasm_tools,"parse",wat,"-o",wasm]);check(result,result.returncode==0)
        result=run("validate",[a.wasm_tools,"validate","--features","all",wasm]);check(result,result.returncode==0)
        for option in ["-Rint","-Raot"]:
            result=run("execute-"+option[1:],[a.binary,option,"-Rct","0","-Rllvm-cache-path","disable","--run",wasm])
            check(result,result.returncode==0)
        for name, options in (
            ("int-run-full-permission", ["-Rint", "--debug-permissions", "full"]),
            ("aot-debug-limited", ["-Raot", "-Rdbg", "--debug-permissions", "limited"]),
            ("aot-debug-none", ["-Raot", "-Rdbg", "--debug-permissions", "none"]),
        ):
            result = run(name, [a.binary, *options, "-Rct", "0", "-Rllvm-cache-path", "disable", "--run", wasm])
            check(result, result.returncode == 0)
        unsupported = b"debug-jit is unsupported in the current mode: uwvm-int/full"
        for name, options in (
            ("debug-int-refusal", ["-Rdbg", "-Rint"]),
            ("int-debug-limited", ["-Rint", "-Rdbg", "--debug-permissions", "limited"]),
            ("debug-int-none", ["--debug-permissions", "none", "--runtime-debug", "--runtime-int"]),
            ("mode-debug-int-read-only", ["-m", "debug", "--runtime-int", "--debug-permissions", "read-only"]),
            ("int-mode-debug-full", ["-Rint", "--mode", "debug", "--debug-permissions", "full"]),
        ):
            result = run(name, [a.binary, *options, "--run", wasm])
            check(result, result.returncode == 126 and unsupported in result.stdout)

        profiles = [("ordinary", [])]
        if b"--wasi-threads" in text:
            profiles.append(("threads", ["--wasi-threads", "1", "2", "1048576", "16777216", "67108864", "65536"]))
        for profile, options in profiles:
            for request, mode, expected in (
                ("explicit", ["-Rdbg"], unsupported),
                ("late", [], b"debug-jit control FD is unsupported in the current mode"),
            ):
                parent, child = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
                with parent, child:
                    child.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
                    # A missing main also proves backend rejection precedes guest loading.
                    result = run(f"int-{profile}-{request}-fd", [
                        a.binary, "-Rint", *mode, *options, "--debug-permissions", "full",
                        "--debug-jit-control-fd", str(child.fileno()), "--run", a.out / "missing.wasm",
                    ], pass_fds=(child.fileno(),))
                    check(result, result.returncode == 126 and expected in result.stdout)
    finally:
        (a.out/"results.json").write_text(json.dumps({"binary_sha256":hashlib.sha256(a.binary.read_bytes()).hexdigest(),"harness_sha256":hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),"cgroup":Path("/proc/self/cgroup").read_text(),"rows":rows},indent=2)+"\n")
    print(json.dumps({"passed":True,"checks":len(rows)}))


if __name__=="__main__":main()
