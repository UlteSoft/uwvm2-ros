#!/usr/bin/env python3
"""Bounded owned ROS1 Master forward proxy with an independent query ledger.

This test-only host actor forwards unchanged public XMLRPC method arguments
and results to the owned numeric Master11384. Only the exact Gateway caller
is counted; native witnesses/controller readiness queries are excluded. Every
accepted HTTP body is bounded before parsing, every socket has a deadline,
and at most4096 forwarded calls are admitted. It never invents native graph
results, launches a ROS node or accepts an arbitrary upstream/network target.
"""
import argparse
import os
from pathlib import Path
import select
import signal
import stat
import sys
import time
import xmlrpc.client
from xmlrpc.server import SimpleXMLRPCServer, SimpleXMLRPCRequestHandler

METHODS = frozenset(("getUri", "getPid", "getSystemState", "getTopicTypes", "getPublishedTopics", "lookupNode", "lookupService",
    "registerPublisher", "unregisterPublisher", "registerSubscriber", "unregisterSubscriber", "registerService", "unregisterService",
    "getParam", "setParam", "deleteParam", "hasParam", "getParamNames", "searchParam", "subscribeParam", "unsubscribeParam"))
QUERIES = frozenset(("getSystemState", "getTopicTypes", "getPublishedTopics", "lookupNode", "lookupService", "getParamNames", "searchParam"))


class ShortTransport(xmlrpc.client.Transport):
    def make_connection(self, host):
        connection = super().make_connection(host); connection.timeout=2; return connection


class BoundedHandler(SimpleXMLRPCRequestHandler):
    rpc_paths=("/", "/RPC2")
    def setup(self):
        super().setup(); self.connection.settimeout(2)
    def do_POST(self):
        length=self.headers.get("content-length", "")
        if not length.isdecimal() or not 1<=int(length)<=1024*1024:
            self.send_error(413); return
        super().do_POST()
    def log_message(self, *_): pass


def exact_write(fd, data):
    while data:
        try: size=os.write(fd,data)
        except InterruptedError: continue
        if size<=0: raise RuntimeError("owned ledger write did not progress")
        data=data[size:]


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--ledger",type=Path,required=True); args=parser.parse_args()
    path=args.ledger; fd=os.open(path,os.O_WRONLY|os.O_APPEND|os.O_NOFOLLOW|os.O_CLOEXEC); state=os.fstat(fd)
    if not path.is_absolute() or not stat.S_ISREG(state.st_mode) or state.st_uid!=os.geteuid() or state.st_mode&0o777!=0o600 or state.st_size:
        os.close(fd); raise RuntimeError("existing empty owned0600 ledger required")
    exact_write(fd,f"MASTER_GRAPH_LEDGER {state.st_dev} {state.st_ino}\n".encode())
    # The controller proves this actual interpreter/actor birth before G.
    os.kill(os.getpid(),signal.SIGSTOP)
    ready,_,_=select.select([0],[],[],20)
    if not ready or os.read(0,1)!=b"G": raise RuntimeError("bounded proxy initial gate differs")
    sequence=0; total=0; deadline=time.monotonic()+180
    def forward(method, parameters):
        nonlocal sequence,total
        if method not in METHODS or not isinstance(parameters,tuple) or not parameters or type(parameters[0]) is not str:
            raise RuntimeError("closed official Master method/caller required")
        total+=1
        if total>4096 or time.monotonic()>=deadline: raise RuntimeError("bounded proxy admission exhausted")
        with xmlrpc.client.ServerProxy("http://127.0.0.1:11384",transport=ShortTransport(),allow_none=True) as upstream:
            result=getattr(upstream,method)(*parameters)
        if not isinstance(result,(list,tuple)) or len(result)!=3 or type(result[0]) is not int:
            raise RuntimeError("actual Master result shape differs")
        if parameters[0]=="/uwvm_sdk_relay/sdk_gateway" and method in QUERIES:
            sequence+=1; exact_write(fd,f"MASTER_GRAPH {sequence} {method} {result[0]}\n".encode())
        return result
    with SimpleXMLRPCServer(("127.0.0.1",11386),requestHandler=BoundedHandler,logRequests=False,allow_none=True) as server:
        server.timeout=.05; server.register_instance(type("OwnedForwarder",(),{"_dispatch":staticmethod(forward)})())
        print("READY_MASTER_GRAPH_PROXY",flush=True)
        while time.monotonic()<deadline: server.handle_request()
    os.close(fd); raise TimeoutError("bounded proxy lifetime exhausted")


if __name__=="__main__":
    try: main()
    except BaseException:
        print("FAIL_MASTER_GRAPH_PROXY",file=sys.stderr,flush=True); raise
