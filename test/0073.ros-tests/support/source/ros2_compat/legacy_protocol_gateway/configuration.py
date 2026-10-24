"""Prepare two explicit trusted-process witness policies, without any actor.

The external containment owner calls ``prepare`` only after observing the real
stopped original child's PID. Identities are taken from an original installed
source export and the exact corresponding generated registry, then checked
again by the unmodified production Gateway. These fixture-local policies are
never installed into a Guest Runtime or inferred from a ROS graph.
"""
from __future__ import annotations

import json
from pathlib import Path
import re


def prepare(directory: Path, worker_pid: int, worker_uid: int,
            native_export: Path, registry: Path, *, socket_directory: str | None = None) -> dict:
    assert type(worker_pid) is int and worker_pid > 0
    assert type(worker_uid) is int and worker_uid >= 0
    assert directory.is_absolute() and directory.is_dir()
    assert directory.stat().st_mode & 0o077 == 0
    # The physical owner writes outside the SDK chroot. Its socket name must
    # resolve inside the actors' namespace, rather than naming the host alias.
    socket_directory = str(directory) if socket_directory is None else socket_directory
    assert Path(socket_directory).is_absolute() and str(Path(socket_directory)) == socket_directory
    assert ".." not in Path(socket_directory).parts and not any(c in socket_directory for c in "\0\n\r")
    source = json.loads(native_export.read_bytes())
    assert source["format"] == "uwvm-ros-industrial-native-pre-rihs-protocol-v1"
    records = {}
    for name, native, codec in re.findall(r'"([a-z0-9_/A-Za-z]+)","(RIHS01_[0-9a-f]{64})","(UWVMROS1_[0-9a-f]{64})"', registry.read_text()):
        assert records.setdefault(name, (native, codec)) == (native, codec)
    roots = ("uwvm_source_proof_msgs/msg/Child", "uwvm_source_proof_msgs/srv/Transform",
             "uwvm_source_proof_msgs/action/Traverse")
    for name in roots:
        assert records[name][0] == source["types"][name]["official"]["native_identity"]
    qos = {"reliability": "reliable", "durability": "volatile", "history": "keep-last",
           "depth": "16", "deadline": "infinite", "lifespan": "infinite",
           "liveliness": "automatic", "liveliness-lease-duration": "infinite"}
    audit = {"max-rate-hz": "10000", "burst": "16", "max-bytes-per-second": "1048576",
             "audit": "allow-and-deny", "audit-max-rate-hz": "10000", "audit-burst": "256",
             "audit-max-records": "8192", "audit-max-bytes": "1048576"}
    grants = []

    def grant(alias, operation, resource, name, limits):
        native, codec = records[name]
        fields = dict(audit, **limits, **{"adapter-codec-identity": codec})
        grants.append("grant " + " ".join((alias, operation, resource, name, native)))
        grants.extend(f"set {alias} {key} {value}" for key, value in sorted(fields.items()))

    for alias, operation, resource in (("topic-publish", "topic.publish", "from_gateway"),
                                       ("topic-subscribe", "topic.subscribe", "to_gateway")):
        limits = {"max-message-bytes": "256", "max-queue-bytes": "4096", "overflow-policy": "reject",
                  "validator": "none", "max-inflight": "4",
                  "max-publishers" if operation == "topic.publish" else "max-subscribers": "1"}
        limits.update({"qos." + key: value for key, value in qos.items()})
        grant(alias, operation, "/uwvm_legacy/" + resource, roots[0], limits)
    for alias, operation, resource in (("service-call", "service.call", "peer_service"),
                                       ("service-advertise", "service.advertise", "gateway_service"),
                                       ("service-respond", "service.respond", "gateway_service")):
        limits = {"timeout-ms": "10000", "max-inflight": "4", "max-request-bytes": "512",
                  "max-response-bytes": "512", "validator": "none"}
        if operation == "service.advertise":
            limits.update({"max-services": "1", "max-event-queue-depth": "16",
                           "max-event-queue-bytes": "8192", "overflow-policy": "reject"})
        limits.update({"qos." + key: value for key, value in qos.items()})
        grant(alias, operation, "/uwvm_legacy/" + resource, roots[1], limits)
    for alias, operation, resource in (("action-send", "action.send-goal", "peer_action"),
                                       ("action-feedback", "action.receive-feedback", "peer_action"),
                                       ("action-result", "action.receive-result", "peer_action"),
                                       ("action-cancel", "action.cancel-own-goal", "peer_action"),
                                       ("action-advertise", "action.advertise", "gateway_action"),
                                       ("action-respond", "action.respond", "gateway_action")):
        limits = {"max-active-goals": "2", "max-actions": "1", "timeout-ms": "10000",
                  "max-goal-bytes": "512", "max-feedback-bytes": "512", "max-result-bytes": "512",
                  "cancel-scope": "own" if operation == "action.cancel-own-goal" else "not-applicable",
                  "max-event-queue-depth": "16", "max-event-queue-bytes": "8192",
                  "overflow-policy": "reject", "validator": "none"}
        for channel in ("goal-service", "cancel-service", "result-service", "feedback", "status"):
            limits.update({"qos." + channel + "." + key: value for key, value in qos.items()})
            # Match the original native Action client's durable status reader;
            # a volatile writer cannot satisfy a transient-local reader.
            if channel == "status":
                limits["qos.status.durability"] = "transient-local"
        grant(alias, operation, "/uwvm_legacy/" + resource, roots[2], limits)
    result = {}
    for selection, rows in (("empty", []), ("granted", grants)):
        common = ["backend ros2", f"worker-uid {worker_uid}", f"worker-pid {worker_pid}",
                  f"socket {socket_directory}/{selection}.sock", "timeout-ms 10000", "max-sessions 4",
                  "audit-max-records-per-second 10000", "audit-max-records 8192", "audit-max-bytes 1048576",
                  f"connection node-name gateway_{selection}", "connection namespace /uwvm_legacy",
                  "connection domain-id 97", "connection rmw-implementation rmw_fastrtps_cpp",
                  "connection security-mode disabled-test", "connection keystore not-applicable",
                  "connection enclave not-applicable"]
        path = directory / (selection + ".conf")
        with path.open("x") as stream:
            stream.write("\n".join(common + rows) + "\n")
        path.chmod(0o400)
        result[selection] = str(path)
    return {"files": result, "worker_pid": worker_pid, "worker_uid": worker_uid,
            "grant_counts": {"empty": 0, "granted": 11}, "Guest_authority": []}
