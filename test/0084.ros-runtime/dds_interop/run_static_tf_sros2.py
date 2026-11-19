"""Qualify native static TF under the official Jazzy SROS2 Enforce stack."""

import argparse
from contextlib import ExitStack
import fnmatch
import json
import os
from pathlib import Path
import resource
import secrets
import select
import socket
import subprocess
import sys
import time
from types import SimpleNamespace
import xml.etree.ElementTree as ET

from run_image import COMPILER, RMW, compile_peer, digest, gate, require
from run_static_tf import LIVE_COUNT, check_case, stop, wait_for

HERE = Path(__file__).resolve().parent
PREFIX = Path("/opt/ros/jazzy")
KEYSTORE = Path("/tmp/tf-keystore")
DOMAIN = 74
ENCLAVES = {role: "/tf_" + role for role in ("writer", "reader", "denied")}
GRANTS = {
    "writer": {"publish": {"rt/tf_static"}, "subscribe": {"rt/parameter_events"}},
    "reader": {"publish": {"rt/parameter_events"}, "subscribe": {"rt/tf", "rt/tf_static"}},
    "denied": {"publish": {"rt/parameter_events"}, "subscribe": {"rt/tf"}},
}
TOPICS = {"rt/tf", "rt/tf_static", "rt/parameter_events"}


def domain_allowed(rule, field):
    """Check whether this fixed test domain is covered by a signed rule."""
    return any(int(v.text) == DOMAIN for v in rule.findall(field + "/id")) or any(
        int(r.findtext("min", "0")) <= DOMAIN <= int(r.findtext("max", "232"))
        for r in rule.findall(field + "/id_range"))


def verify_signed(folder, name):
    """Verify the signed SROS2 XML using the generated permissions CA."""
    signed = folder / (name + ".p7s")
    command = ["/usr/bin/openssl", "smime", "-verify", "-text", "-in", signed, "-CAfile",
               folder / "permissions_ca.cert.pem", "-purpose", "any"]
    return ET.fromstring(subprocess.check_output(command, stderr=subprocess.PIPE, timeout=10))


def issue(out):
    """Issue independent ephemeral enclaves and verify their enforcement inputs."""
    with (out / "issue.log").open("wb") as log:
        subprocess.run(["ros2", "security", "generate_artifacts", "-k", KEYSTORE, "-p", HERE / "static-tf-policy.xml"],
                       check=True, timeout=35, stdout=log, stderr=subprocess.STDOUT)
    entries = {}
    for role, enclave in ENCLAVES.items():
        folder = KEYSTORE / "enclaves" / enclave[1:]
        governance = verify_signed(folder, "governance")
        rule, = governance.findall(".//domain_rule")
        require(domain_allowed(rule, "domains") and rule.findtext("allow_unauthenticated_participants") == "false"
                and rule.findtext("enable_join_access_control") == "true", "missing authenticated domain governance")
        topic, = rule.findall("topic_access_rules/topic_rule")
        expected = {"topic_expression": "*", "enable_read_access_control": "true",
                    "enable_write_access_control": "true", "metadata_protection_kind": "ENCRYPT",
                    "data_protection_kind": "ENCRYPT"}
        require(all(topic.findtext(k) == v for k, v in expected.items()), "incomplete topic protection")

        grant, = verify_signed(folder, "permissions").findall(".//grant")
        require(grant.findtext("default") == "DENY", "permissions default must be DENY")
        rules = [row for row in grant.findall("allow_rule") if domain_allowed(row, "domains")]
        allowed = {}
        for access in ("publish", "subscribe"):
            patterns = {entry.text for row in rules for entry in row.findall(access + "/topics/topic")}
            allowed[access] = {name for name in TOPICS if any(fnmatch.fnmatchcase(name, p) for p in patterns)}
        require(allowed == GRANTS[role], f"unexpected static TF grants for {role}")
        entries[role] = {
            "enclave": enclave,
            "default": "DENY",
            "signedSubject": grant.findtext("subject_name"),
            "topicGrants": {access: sorted(topics) for access, topics in allowed.items()},
            "topicGovernance": expected,
            "publicArtifactSha256": {name: digest(folder / name) for name in
                                     ("cert.pem", "governance.p7s", "permissions.p7s")},
        }
    require(len({e["publicArtifactSha256"]["cert.pem"] for e in entries.values()}) == 3, "shared identity")
    return entries


def denied_reader():
    """Try the exact protected data reader using an issued, narrower enclave."""
    import rclpy
    from rclpy.parameter import Parameter
    from rclpy.qos import DurabilityPolicy, QoSProfile
    from rclpy.utilities import get_rmw_implementation_identifier
    from tf2_msgs.msg import TFMessage

    rclpy.init(args=[])
    require(get_rmw_implementation_identifier() == RMW["jazzy"], "observer RMW changed")
    node = rclpy.create_node("wros_static_tf_denied", enable_rosout=False, start_parameter_services=False,
                             parameter_overrides=[Parameter("start_type_description_service", value=False)])
    callbacks = []

    def receive(message):
        callbacks.append(message)

    try:
        allowed = node.create_subscription(TFMessage, "/tf", receive, 4)
        require(allowed.topic_name == "/tf", "unrelated allowed subscription failed")
        print("ALLOWED /tf", flush=True)
        qos = QoSProfile(depth=100, durability=DurabilityPolicy.TRANSIENT_LOCAL)
        try:
            node.create_subscription(TFMessage, "/tf_static", receive, qos)
        except RuntimeError:
            print("DENIED /tf_static", flush=True)
        else:
            raise RuntimeError("ungranted static TF reader created")
        until = time.monotonic() + 20
        while time.monotonic() < until:
            rclpy.spin_once(node, timeout_sec=0.01)
            if select.select([sys.stdin], [], [], 0)[0]:
                require(sys.stdin.readline() == "x\n", "observer lost its coordinator")
                break
        else:
            raise RuntimeError("observer timed out")
        require(not callbacks, "denied observer was delivered a sample")
        print("COMPLETE 0", flush=True)
    finally:
        node.destroy_node()
        rclpy.shutdown()


def endpoint(stack, out, role, argv):
    """Start one real participant with a distinct signed enclave."""
    env = dict(os.environ, ROS_SECURITY_ENABLE="true", ROS_SECURITY_STRATEGY="Enforce",
               ROS_SECURITY_KEYSTORE=str(KEYSTORE), ROS_SECURITY_ENCLAVE_OVERRIDE=ENCLAVES[role])
    return stack.enter_context(subprocess.Popen(
        argv, env=env, stdin=subprocess.PIPE,
        stdout=stack.enter_context((out / (role + ".stdout")).open("wb")),
        stderr=stack.enter_context((out / (role + ".stderr")).open("wb"))))


def worker():
    """Hold the denied participant alongside the authorized late join and all updates."""
    gate()
    require(socket.if_nameindex() == [(1, "lo")], "secure participants require private loopback")
    os.umask(0o077)
    out = Path("/tmp/evidence")
    os.environ.update(ROS_DOMAIN_ID=str(DOMAIN), RMW_IMPLEMENTATION=RMW["jazzy"],
                      FASTRTPS_DEFAULT_PROFILES_FILE=str(HERE / "fastdds-image-loopback.xml"),
                      ROS_LOG_DIR="/tmp/ros-logs", ROS_AUTOMATIC_DISCOVERY_RANGE="SYSTEM_DEFAULT",
                      RMW_FASTRTPS_USE_QOS_FROM_XML="1")
    report = {"format": "uwvm-native-static-tf-sros2-v1", "complete": False,
              "scope": "Native Jazzy rclcpp publisher and Jazzy Python TF2 under SROS2",
              "transport": "isolated IPv4 loopback", "strategy": "Enforce", "domain": DOMAIN,
              "packages": {}}

    def save():
        (out / "witness.json").write_text(json.dumps(report, indent=2) + "\n")

    for name in ("sros2", "rclcpp", "rclpy", "rmw_fastrtps_cpp", "tf2_ros_py", "tf2_msgs"):
        manifest = PREFIX / "share" / name / "package.xml"
        report["packages"][name] = {"version": ET.parse(manifest).findtext("version"), "sha256": digest(manifest)}
    save()
    report["signedPolicies"] = issue(out)
    save()
    token = 1 + secrets.randbelow(4095)
    with ExitStack() as stack:
        writer = endpoint(stack, out, "writer", ["/tmp/writer", RMW["jazzy"], str(token), "transient"])
        stack.callback(stop, writer)
        wait_for(writer, out / "writer.stdout", "PUBLISHED 0 ", 8)
        denied = endpoint(stack, out, "denied", [sys.executable, "-B", __file__, "--observer"])
        stack.callback(stop, denied)
        wait_for(denied, out / "denied.stdout", "DENIED /tf_static", 8)

        reader = endpoint(stack, out, "reader", [sys.executable, "-B", HERE / "ros_static_tf.py",
                                                RMW["jazzy"], "transient", str(token)])
        stack.callback(stop, reader)
        wait_for(reader, out / "reader.stdout", "HISTORY 1", 12)
        for sequence in range(1, LIVE_COUNT + 1):
            require(denied.poll() is None, "denied participant exited before live delivery")
            writer.stdin.write(b"p\n")
            writer.stdin.flush()
            wait_for(reader, out / "reader.stdout", f"RECEIVED {sequence} ", 3)
        require(writer.wait(timeout=5) == reader.wait(timeout=5) == 0, "authorized participant failed")
        denied.stdin.write(b"x\n")
        denied.stdin.flush()
        require(denied.wait(timeout=5) == 0, "observer did not complete")
    (out / "processes.json").write_text(json.dumps({"writer": writer.returncode, "reader": reader.returncode,
                                                   "denied": denied.returncode, "token": token}) + "\n")

    stdout = (out / "denied.stdout").read_text().splitlines()
    diagnostics = stdout + (out / "denied.stderr").read_text().splitlines()
    denial = "rt/tf_static topic not found in allow rule"
    require(any(denial in line and "check_create_datareader" in line for line in diagnostics),
            "no DDS access-control rejection")
    require([line for line in stdout if line.startswith(("ALLOWED ", "DENIED ", "COMPLETE "))] ==
            ["ALLOWED /tf", "DENIED /tf_static", "COMPLETE 0"], "unexpected denied participant outcome")
    for role, enclave in ENCLAVES.items():
        loaded = str(KEYSTORE / "enclaves" / enclave[1:])
        activations = [line.partition("Found security directory: ")[2] for line in
                       (out / (role + ".stderr")).read_text().splitlines() if "Found security directory: " in line]
        require(activations == [loaded], f"unexpected runtime enclave for {role}")
        report["signedPolicies"][role]["runtimeEnclaveVerified"] = True
    checked = SimpleNamespace(jazzy_prefix=PREFIX, sanitized=False)
    report["authorized"] = check_case(checked, out, "jazzy", "transient")
    report["denied"] = {"subscribe": "/tf_static", "allowedSubscribe": "/tf", "samples": 0,
                        "ddsReason": denial, "onlineDuringAuthorizedDelivery": True,
                        "outputSha256": {name: digest(out / name) for name in ("denied.stdout", "denied.stderr")}}
    report["complete"] = True
    save()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rootfs", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    member = gate()
    out = args.output.resolve()
    require(".tmp" in out.parts, "output must be below a project .tmp")
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    resource.setrlimit(resource.RLIMIT_FSIZE, (16 << 20, 16 << 20))
    out.mkdir(mode=0o700)
    root = args.rootfs.resolve(strict=True)
    prefix = root / PREFIX.relative_to("/")
    inputs = [Path(__file__).resolve(), COMPILER,
              *(HERE / file for file in ("ros_static_tf.cpp", "ros_static_tf.py", "run_static_tf.py", "run_image.py",
                                         "run.py", "static-tf-policy.xml", "fastdds-image-loopback.xml")),
              prefix / "share/tf2_msgs/msg/TFMessage.idl"]
    report = {"complete": False, "cgroup": member, "cpu_set": sorted(os.sched_getaffinity(0)),
              "inputsSha256": {str(item): digest(item) for item in inputs}}
    destination = out / "results.json"
    destination.write_text(json.dumps(report, indent=2) + "\n")
    report["build"] = compile_peer(prefix, root / "usr/lib/x86_64-linux-gnu", "jazzy", out, False,
                                   witness="static_tf", message_package="tf2_msgs")
    binary = out / "jazzy-static_tf"
    report["binarySha256"] = digest(binary)
    destination.write_text(json.dumps(report, indent=2) + "\n")
    cmd = ["/usr/bin/bwrap", "--die-with-parent", "--unshare-net", "--unshare-ipc", "--unshare-pid",
           "--ro-bind", root, "/", "--proc", "/proc", "--dev", "/dev", "--ro-bind", "/sys", "/sys",
           "--tmpfs", "/tmp", "--dir", "/tmp/home", "--bind", out, "/tmp/evidence",
           "--ro-bind", HERE, "/tmp/fixture", "--ro-bind", binary, "/tmp/writer",
           "--chdir", "/tmp", "--clearenv", "--setenv", "HOME", "/tmp/home",
           "--setenv", "PATH", "/usr/bin:/bin", "--setenv", "LANG", "C.UTF-8",
           "/bin/bash", "-c", ". /opt/ros/jazzy/setup.bash && exec /usr/bin/python3 -B "
           "/tmp/fixture/run_static_tf_sros2.py --worker"]
    with (out / "sandbox.log").open("wb") as log:
        subprocess.run(cmd, check=True, timeout=90, stdout=log, stderr=subprocess.STDOUT)
    report.update(json.loads((out / "witness.json").read_bytes()))
    require(report["complete"], "incomplete SROS2 witness")
    require(all(digest(Path(path)) == sha for path, sha in report["inputsSha256"].items()), "fixture inputs changed")
    report["issueLogSha256"] = digest(out / "issue.log")
    destination.write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    if sys.argv[1:] == ["--worker"]:
        worker()
    elif sys.argv[1:] == ["--observer"]:
        denied_reader()
    else:
        main()
