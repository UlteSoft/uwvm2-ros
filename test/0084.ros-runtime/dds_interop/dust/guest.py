"""Run the four local pairings in distinct domains inside the isolated guest."""
import json
import os
from pathlib import Path
import secrets
import subprocess
import sys
import time


def command(endpoint, role, domain, token, rust):
    if endpoint == "fast":
        return ["/fixture/ros-client", str(token), role, "rmw_fastrtps_cpp"]
    if rust:
        return ["/fixture/rust-peer", role, str(domain), str(token)]
    return [sys.executable, "-B", "/fixture/peer.py", role, str(domain), str(token)]


def collect(process):
    try:
        output, _ = process.communicate(timeout=18)
    except subprocess.TimeoutExpired:
        process.kill()
        output, _ = process.communicate()
    if len(output) >= 8192:
        raise ValueError("unexpected DDS fixture log size")
    return {"code": process.returncode, "output": output.decode(errors="replace")}


def main():
    if sorted(p.name for p in Path("/sys/class/net").iterdir()) != ["eth0", "lo"]:
        raise RuntimeError("expected one virtual Ethernet interface and loopback")
    if any(row.split()[1] == "00000000" for row in Path("/proc/net/route").read_text().splitlines()[1:]):
        raise RuntimeError("unexpected default route in isolated guest")
    rust = Path("/fixture/rust-peer").is_file()
    pairs = [("dust", "dust"), ("fast", "fast"), ("fast", "dust"), ("dust", "fast")]
    for index, (requester, responder) in enumerate(pairs):
        token = secrets.randbelow(2**28) + 1
        domain = 61 + index
        env = {**os.environ, "PATH": "/bin:/usr/bin", "ROS_DOMAIN_ID": str(domain),
               "AMENT_PREFIX_PATH": "/opt/ros/jazzy", "LD_LIBRARY_PATH": "/opt/ros/jazzy/lib:/syslib/jazzy",
               "RMW_IMPLEMENTATION": "rmw_fastrtps_cpp", "FASTRTPS_DEFAULT_PROFILES_FILE": "/fixture/fastdds.xml",
               "ROS_HOME": "/tmp/ros-home", "ROS_LOG_DIR": "/tmp/ros-log", "PYTHONPATH": "/fixture/dust",
               "PYTHONDONTWRITEBYTECODE": "1", "HOME": "/tmp"}
        if rust:
            env["ROS_AUTOMATIC_DISCOVERY_RANGE"] = "SUBNET"
        options = {"env": env, "stdout": subprocess.PIPE, "stderr": subprocess.STDOUT}
        with subprocess.Popen(command(responder, "echo", domain, token, rust), **options) as echo:
            time.sleep(.4)
            with subprocess.Popen(command(requester, "request", domain, token, rust), **options) as ask:
                request = collect(ask)
            response = collect(echo)
        case = dict(requester=requester, responder=responder, token=token, domain=domain,
                    request=request, response=response, passed=request["code"] == response["code"] == 0)
        print("DUST_CASE " + json.dumps(case, separators=(",", ":")), flush=True)
    print("DUST_DONE " + str(len(pairs)), flush=True)


if __name__ == "__main__":
    main()
