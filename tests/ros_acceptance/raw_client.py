#!/usr/bin/env python3
"""Actual host socket attacks against one owned fixed-PID Gateway.

This is an adversarial host client, not a Wasm SDK or a forged Runtime Instance.
Wrong UID is produced only by dropping this owned child to65534. The controller
temporarily permits access to its owned test socket so denial is measured at
SO_PEERCRED rather than directory permissions. Raw corrupt/truncated/unbound
frames use the actual48-byte protocol but never construct a trusted manifest.
Every connection/transfer is bounded; only public outcome counters are printed.
Native side effects are established separately by a real native witness and
the test Graph observer, not inferred from these host refusal records.
"""
import argparse
import os
from pathlib import Path
import select
import signal
import socket
import struct
import sys
import time

HEADER=struct.Struct("<IHHB3xQQQIII")


def frame(version, kind=1, request=1, response=0, session=0, size=0, error=0, timeout=1000):
    return HEADER.pack(0x32524f55,1,kind,version,request,response,session,size,error,timeout)


def receive(sock, size):
    output=b""
    while len(output)<size:
        part=sock.recv(size-len(output))
        if not part: break
        output+=part
    return output


def refused(path, body, write_shutdown=True):
    with socket.socket(socket.AF_UNIX,socket.SOCK_STREAM) as sock:
        sock.settimeout(2); sock.connect(path); sock.sendall(body)
        if write_shutdown: sock.shutdown(socket.SHUT_WR)
        try: result=sock.recv(1)
        except (ConnectionResetError,BrokenPipeError): result=b""
        if result: raise RuntimeError("malformed/credential request received accepted protocol bytes")


def unbound(path,version):
    with socket.socket(socket.AF_UNIX,socket.SOCK_STREAM) as sock:
        sock.settimeout(2);sock.connect(path);sock.sendall(frame(version));reply=receive(sock,48)
        if len(reply)!=48: raise RuntimeError("positive raw protocol handshake failed")
        fields=HEADER.unpack(reply)
        if fields[:4]!=(0x32524f55,1,3,version) or fields[5]!=1 or not fields[6] or fields[7] or fields[8]:
            raise RuntimeError("actual raw protocol handshake metadata differs")
        operation=b"graph.list-nodes";alias=b"cap";payload=struct.pack("<HHI",len(operation),len(alias),0)+operation+alias
        sock.sendall(frame(version,kind=2,request=2,session=fields[6],size=len(payload))+payload)
        reply=receive(sock,48)
        if len(reply)!=48: raise RuntimeError("unbound request lacks actual refusal response")
        answer=HEADER.unpack(reply)
        if answer[2]!=3 or answer[3]!=version or answer[5]!=2 or answer[6]!=fields[6] or answer[7] or answer[8]!=13:
            raise RuntimeError("unbound request was not exact13 with empty output")


def control(path,version,manifest):
    """Prove the exact selected native observer active before the attack.

    This explicit trusted host-control manifest is not a Wasm Instance claim.
    It grants one bounded exact-node metadata operation, closes its connection,
    then waits A before creating any malicious/wrong-UID connection.
    """
    with socket.socket(socket.AF_UNIX,socket.SOCK_STREAM) as sock:
        sock.settimeout(12);sock.connect(path);sock.sendall(frame(version,timeout=10000));reply=receive(sock,48)
        if len(reply)!=48:raise RuntimeError("native control handshake failed")
        session=HEADER.unpack(reply)[6]
        if not session:raise RuntimeError("native control session missing")
        def call(index,operation,alias,data=b""):
            operation=operation.encode();alias=alias.encode();payload=struct.pack("<HHI",len(operation),len(alias),len(data))+operation+alias+data
            sock.sendall(frame(version,kind=2,request=index,session=session,size=len(payload),timeout=10000 if operation==b"session.bind" else 2000)+payload)
            result=receive(sock,48)
            if len(result)!=48:raise RuntimeError("native control response missing")
            fields=HEADER.unpack(result)
            if fields[2]!=3 or fields[3]!=version or fields[5]!=index or fields[6]!=session or fields[7]>128:
                raise RuntimeError("native control response binding differs")
            return fields[8],receive(sock,fields[7])
        if call(2,"session.bind","target",manifest)!=(0,b""):raise RuntimeError("native control immutable manifest rejected")
        sock.settimeout(3)
        expected=b"/uwvm_sdk_relay/sdk_peer\n";expected=struct.pack("<I",len(expected))+expected
        for index in range(3,203):
            error,body=call(index,"graph.inspect-node","ready")
            if error==0:
                if body!=expected:raise RuntimeError("native control exact node bytes differ")
                break
            if error not in (2,11) or body:raise RuntimeError("native control bounded discovery refused unexpectedly")
            time.sleep(.25)
        else:raise RuntimeError("native control discovery exhausted")
    print("RAW_GRAPH_CONTROL_DONE",flush=True)
    ready,_,_=select.select([0],[],[],20)
    if not ready or os.read(0,1)!=b"A":raise RuntimeError("post-control attack gate differs")


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument("--socket",required=True)
    parser.add_argument("--version",type=int,choices=(1,2),required=True);parser.add_argument("--mode",choices=("wrong-uid","raw-frame"),required=True)
    parser.add_argument("--manifest",type=Path,required=True)
    args=parser.parse_args();os.kill(os.getpid(),signal.SIGSTOP)
    ready,_,_=select.select([0],[],[],20)
    if not ready or os.read(0,1)!=b"G":raise RuntimeError("host attack input gate differs")
    manifest=args.manifest.read_bytes()
    if not 7<=len(manifest)<=65536 or not manifest.startswith(b"UWRP\x01"):raise RuntimeError("bounded native control manifest required")
    control(args.socket,args.version,manifest)
    if args.mode=="wrong-uid":
        if os.geteuid()!=0:raise RuntimeError("owned UID-drop actor must start as root")
        os.setgroups([]);os.setgid(65534);os.setuid(65534)
        if os.getuid()!=65534 or os.geteuid()!=65534:raise RuntimeError("actual UID drop differs")
        refused(args.socket,frame(args.version));print("PASS_RAW_CREDENTIAL uid=65534 hello_rejected=1",flush=True);return
    good=frame(args.version); mutations=[]
    mutations.append(("magic",b"BAD!"+good[4:]))
    mutations.append(("version",good[:4]+b"\x02\0"+good[6:]))
    mutations.append(("kind",frame(args.version,kind=6)))
    mutations.append(("backend",frame(3-args.version)))
    mutations.append(("reserved",good[:9]+b"\x01\0\0"+good[12:]))
    mutations.append(("oversize",frame(args.version,size=4*1024*1024+1)))
    mutations.append(("timeout-zero",frame(args.version,timeout=0)))
    mutations.append(("timeout-large",frame(args.version,timeout=60001)))
    mutations.append(("hello-session",frame(args.version,session=7)))
    mutations.append(("hello-response",frame(args.version,response=1)))
    mutations.append(("hello-error",frame(args.version,error=13)))
    mutations.append(("truncated-header",good[:47]))
    mutations.append(("truncated-payload",frame(args.version,size=1)))
    for name,body in mutations:
        refused(args.socket,body);print("RAW_FRAME_REJECT name="+name+" accepted=0",flush=True)
    unbound(args.socket,args.version)
    print("PASS_RAW_FRAME mutations=13 unbound13=1 native_authority_claimed=0",flush=True)


if __name__=="__main__":
    try:main()
    except BaseException:
        print("FAIL_RAW_ATTACK",file=sys.stderr,flush=True);raise
