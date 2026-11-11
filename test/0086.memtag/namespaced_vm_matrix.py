#!/usr/bin/env python3
"""Execute the sole FF packed-u32 MTE wire; reject both obsolete encodings.

Ordinary FC numeric/bulk instructions remain standard. An unused or unreachable
body must not retain a compatibility path for FC MTE or FF vendor headers.
"""
import argparse
import hashlib
import json
from pathlib import Path
import resource
import shlex
import subprocess

import vm_matrix as vm
from vm_diagnostics import fault_matches


def encode(operation, memory=0, *, domain=1, version=1, padded=False, padded_memory=False):
    def integer(value, padding):
        if not padding:
            return vm.u(value)
        return bytes(((value >> (7*i)) & 127) | (128 if i != 4 else 0) for i in range(5))
    word = (version << 24) | (domain << 16) | operation
    return b"\xff" + integer(word,padded) + integer(memory,padded_memory)


def two_functions(entry, unused):
    first, second = b"\0" + entry + b"\x0b", b"\0" + unused + b"\x0b"
    return (b"\0asm\x01\0\0\0" + vm.sec(1,b"\x01\x60\0\0") + vm.sec(3,b"\x02\0\0")
            + vm.sec(5,b"\x01\x01\x01\x01") + vm.sec(7,b"\x01\x06_start\0\0")
            + vm.sec(10,b"\x02" + vm.u(len(first)) + first + vm.u(len(second)) + second))


def cases():
    original = vm.op
    try:
        for encoding in ("packed", "packed-padded", "packed-padded-memory"):
            vm.op = lambda op,memory=0: encode(op,memory,padded=encoding!="packed",padded_memory=encoding.endswith("memory"))
            for wide in (False,True):
                for enabled in (False,True):
                    name=f"{encoding}-{64 if wide else 32}-{'enabled' if enabled else 'disabled'}"
                    options=["--wasm-feature-enable-multi-memory"] if encoding.endswith("memory") else []
                    yield name,vm.markers(wide,enabled),"ok",options,not enabled
                    if encoding.endswith("memory"):
                        yield name+"-without-multi-memory",vm.markers(wide,enabled),"multi-memory",[],not enabled
                yield f"{encoding}-{64 if wide else 32}-feature-off",vm.module(vm.op(0x20)+b"\x1a",wide),"feature",["--wasm-feature-disable-memtag-extension"],False
    finally:
        vm.op=original
    for label,wire in (
        ("gc-domain-reserved",encode(0x20,domain=2)),
        ("domain-zero",encode(0x20,domain=0)),
        ("unknown-domain",encode(0x20,domain=255)),
        ("version-zero",encode(0x20,version=0)),
        ("version-two",encode(0x20,version=2)),
        ("version-max",encode(0x20,version=255)),
        ("standard-subopcode",encode(0x10)),
        ("operation-gap",encode(0x30)),
        ("reserved-add",encode(0x33)),
        ("operation-max",encode(0xffff)),
        ("word-overflow",b"\xff\x80\x80\x80\x80\x10\0"),
        ("word-truncated",b"\xff\x80\x80\x80\x80"),
    ):
        yield "reject-"+label,vm.module(wire+b"\x1a"),"namespace",[],False
        yield "dead-reject-"+label,vm.module(b"\0"+wire+b"\x1a"),"namespace",[],False
        yield "unused-reject-"+label,two_functions(b"",wire+b"\x1a"),"namespace",[],False
    # All operation identities are tested under both discarded wire grammars.
    for operation in (*range(0x20,0x30),0x33,0x36,0x37,0x38):
        old_fc=b"\xfc"+vm.u(operation)+b"\0"
        old_ff=b"\xff"+b"".join(vm.u(value) for value in (0x554C5445,1,1,operation,0))
        for label,wire,expected in (("obsolete-fc",old_fc,"obsolete-fc"),("obsolete-ff-vendor",old_ff,"namespace")):
            yield f"{label}-{operation:02x}",vm.module(wire+b"\x1a"),expected,[],False
            yield f"dead-{label}-{operation:02x}",vm.module(b"\0"+wire+b"\x1a"),expected,[],False
            yield f"unused-{label}-{operation:02x}",two_functions(b"",wire+b"\x1a"),expected,[],False
    yield "unused-packed-op-enables-owner",two_functions(vm.c(0x30000000)+vm.mem(0x28)+b"\x1a",encode(0x20)+b"\x1a"),"memtag",[],False
    for wide in (False,True):
        yield f"standard-fc-bulk-{64 if wide else 32}",vm.bulks(wide),"ok",[],False


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm",type=Path)
    parser.add_argument("--engines",default="int,aot")
    parser.add_argument("--runner",default="",help="Optional shlex-parsed emulator command prefix.")
    parser.add_argument("--out",type=Path,required=True)
    parser.add_argument("--generate-only",action="store_true")
    args=parser.parse_args()
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    args.out.mkdir(parents=True,exist_ok=True)
    manifest=[]
    for name,wasm,expected,options,disabled in cases():
        path=(args.out/(name+".wasm")).resolve();path.write_bytes(wasm)
        manifest.append(dict(name=name,file=str(path),expected=expected,options=options,
                             disabled=disabled,sha256=hashlib.sha256(wasm).hexdigest()))
    (args.out/"manifest.json").write_text(json.dumps(manifest,indent=2)+"\n")
    if args.generate_only:return
    if args.vm is None:parser.error("--vm is required without --generate-only")
    rows=[]
    for case in manifest:
        for engine in args.engines.split(","):
            command=shlex.split(args.runner)+[str(args.vm.resolve()),f"--runtime-{engine}","-WFE-memory64","--wasm-set-start-func","0"]
            if engine=="aot":command += ["--runtime-llvm-jit-cache-path","disable","--runtime-compile-threads","0"]
            if case["disabled"]:command += ["--runtime-disable-memtag"]
            command += case["options"]+["--",case["file"]]
            result=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=90)
            text=result.stdout.decode("utf-8","replace")
            (args.out/(case["name"]+"-"+engine+".log")).write_text(text)
            rejection=result.returncode==127 and "Validation error" in text
            if case["expected"]=="ok":passed=result.returncode==0
            elif case["expected"]=="namespace":passed=rejection and "uwvm.memtag namespace" in text
            elif case["expected"]=="obsolete-fc":passed=rejection and "obsolete FC memtag encoding" in text and "regenerate" in text
            elif case["expected"]=="multi-memory":passed=rejection and "wasm-feature-enable-multi-memory" in text
            elif case["expected"]=="memtag":passed=fault_matches(result.returncode,text,122,"memory tag mismatch")
            else:passed=rejection and "memtag" in text.lower() and "feature" in text.lower()
            rows.append(dict(name=case["name"],engine=engine,exit=result.returncode,expected=case["expected"],passed=passed,command=command))
            (args.out/"report.json").write_text(json.dumps(rows,indent=2)+"\n")
            print(case["name"],engine,result.returncode,"PASS" if passed else "FAIL",flush=True)
    raise SystemExit(any(not row["passed"] for row in rows))


if __name__=="__main__":main()
