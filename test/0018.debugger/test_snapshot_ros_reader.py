#!/usr/bin/env python3
"""Offline ROS v1 data validation; never imports or contacts a native gateway."""
import argparse
import struct
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/debug"))
import snapshot


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--fixture", type=Path, required=True, help="Binary emitted by debug_snapshot_ros_environment.cc")
    args=parser.parse_args()
    data=args.fixture.read_bytes()
    result=snapshot.ros(data)
    assert not result["externalEnvironmentCaptured"] and not result["grantsRestoreAuthority"]
    group=result["groups"][0]
    assert [m["module"] for m in group["members"]]==["0","1"]
    queue=group["observations"][0]["local"]
    assert queue["state"]["pinnedSource"]=="1"
    assert queue["events"][0]["payload"]=="01020304"
    assert queue["events"][1]["present"] and not queue["events"][2]["present"]
    checks=4
    def reject(blob):
        nonlocal checks
        try:snapshot.ros(blob)
        except snapshot.InvalidSnapshot:checks+=1
        else:raise AssertionError("accepted malformed ROS environment")
    for n in range(len(data)):reject(data[:n])
    reject(data+b"\0")
    reject(data[:33]+struct.pack("<Q",2**64-1)+data[41:])
    reject(data[:32]+b"\3"+data[33:])
    reject(data[:16]+bytes(16)+data[32:])
    assert struct.pack("<Q",0xfedcba9876543210) not in data
    print(f"PASS ROS v1 offline environment reader: {checks+1} checks")


if __name__=="__main__": main()
