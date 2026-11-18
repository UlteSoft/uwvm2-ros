#!/usr/bin/env python3
"""Inspect a UWVM binary snapshot without executing code or restoring resources.

Archive integrity is not authenticity, debugger permission, restore authority or
permission to replay an external effect. This reader never interprets a native
address and never opens a saved WASI/ROS resource. Source extraction uses only
explicit caller-selected output paths and never overwrites an existing file.
"""
from __future__ import annotations

import argparse
from dataclasses import dataclass
import hashlib
import json
import os
from pathlib import Path
import struct

MAGIC = b"UWSNAP1\0"
COMMIT = b"UWSEND1\0"
KINDS = {1: "diagnostic", 2: "wasm-state", 3: "wasm-module", 4: "effective-code",
         5: "debug-source", 6: "wasip1", 7: "ros1", 8: "ros2", 9: "events", 10: "wasm-inspection",
         11: "retained-native"}
SCENE_OBSERVATION_SCHEMA = 0x52534331
CAPTURE = ("complete", "partial", "redacted", "unavailable")
ARCH = ("unknown", "x86_64", "i686", "aarch64", "arm", "armhf")
OS = ("unknown", "linux", "windows", "macos", "freebsd")


class InvalidSnapshot(ValueError):
    pass


def require(condition, message):
    if not condition:
        raise InvalidSnapshot(message)


@dataclass(frozen=True)
class Record:
    kind: int
    capture: int
    identifier: int
    schema: int
    payload: memoryview


@dataclass(frozen=True)
class Snapshot:
    data: bytes
    platform: dict
    disclosure: str
    records: tuple[Record, ...]
    scene_observation: dict | None = None

    def describe(self):
        result = {"format": "uwvm-snapshot-v1", "disclosure": self.disclosure,
                  "origin": self.platform, "bytes": len(self.data), "sha256": self.data[-32:].hex(),
                  "records": [{"kind": KINDS[r.kind], "id": r.identifier, "schema": r.schema,
                               "capture": CAPTURE[r.capture], "bytes": len(r.payload)} for r in self.records],
                  "executableRestore": False}
        if self.scene_observation is not None:
            result["sceneObservation"] = self.scene_observation
        for row in self.records:
            if row.kind == 1 and row.capture != 3:
                result["diagnostic"] = diagnostic(row.payload)
            elif row.kind == 6 and row.capture != 3:
                result.setdefault("wasip1", []).append(wasip1(row.payload))
            elif row.kind in (7, 8) and row.capture != 3:
                result.setdefault("ros", []).append(ros(row.payload))
            elif row.kind == 9 and row.schema == 0x54484a38:
                result["threadHistory"] = threaded_history(row.payload, self.records)
            elif row.kind == 11:
                require(row.capture == 1 and row.identifier == 0 and row.schema == 1 and len(row.payload) == 32,
                        "invalid retained native continuation")
                result["continuation"] = {"mode": "retained-native", "crossProcess": False,
                    "environmentObservation": "recording-origin", "requiresOriginalLiveStop": True,
                    "nativeResult": "not-serialized", "restoreAuthority": False}
        return result


# Stable-v1 local ROS environment. Schema 2 is deliberately PARTIAL: a local
# snapshot cannot rewind the external robot, DDS peer, native socket or RPC.
_ROS_ACCOUNT = "openCount burst bandwidth tokens creationTokens byteTokens byteFraction logBandwidth logTokens logFraction limitedRate limitedBytes limitedLog".split()
_ROS_AUDIT = "retainedBytes dropped attempted accepted overwritten rateLimited oversized unconfigured allocationFailed sequenceExhausted drainFailed capacityExhausted cancelled clockUnavailable saturated".split()
_ROS_QUEUE = "maxDepth maxBytes maxRecordBytes maxPendingEvents maxPendingBytes overflow mask sequence queuedBytes reservedBytes pinnedSource pinnedRequest".split()
_ROS_TOTALS = "depth bytes pendingDepth pendingBytes reservedDepth reservedBytes highWaterDepth highWaterBytes enqueued dequeued droppedOldest droppedNewest rejected backpressured admissionRefused completed closed".split()
_ROS_TYPES = {
    "field": [("name", "s"), ("value", "s")],
    "member": [("module", "Q"), ("name", "s"), ("registered", "?"), ("retired", "?")],
    "capability": [(n, "s") for n in ("alias", "operation", "resource", "type", "typeIdentity")] + [
        ("limits", ["field"]), ("revoked", "?"), ("accounting", ("Q", _ROS_ACCOUNT)),
        ("fractions", ("s", ["rate", "call", "creation"])), ("audit", ("Q", _ROS_AUDIT)), ("auditRecords", ["s"])],
    "handle": [("slot", "I"), ("generation", "I"), ("alias", "s"), ("active", "?"), ("retired", "?")],
    "event": [("location", "B"), ("present", "?"), ("request", "Q"), ("responseCredit", "Q"),
              ("ownedBytes", "Q"), ("kind", "H"), ("flags", "B"), ("error", "I"), ("correlation", "Q"), ("payload", "blob")],
    "queue": [("state", ("Q", _ROS_QUEUE)), ("configured", "?"), ("active", "?"),
              ("totals", ("Q", _ROS_TOTALS)), ("events", ["event"])],
    "observation": [(n, "Q") for n in ("handle", "sourceHandle", "correlation", "nativeSequence", "delivered")] + [
        ("observerAlias", "s"), ("sourceAlias", "s"), ("retired", "?"), ("currentSession", "?"), ("requests", ["Q"]), ("local", "queue")],
    "availability": [("handle", "Q"), ("sourceHandle", "Q"), ("availabilityAlias", "s"), ("sourceAlias", "s")] + [
        (n, "?") for n in ("retired", "sourceRetired", "availabilityRevoked", "sourceRevoked", "currentSession")],
    "group": [("scope", "B"), ("name", "s")] + [(n, "?") for n in (
        "enabled", "registered", "alive", "sessionPresent", "connected", "availabilitySealed")] + [
        ("timeout", "I"), ("nextServiceRequest", "Q"), ("sessionLabel", "Q"), ("members", ["member"]),
        ("connection", ["field"]), ("interfaces", ["field"]), ("capabilities", ["capability"]),
        ("handles", ["handle"]), ("observations", ["observation"]), ("availabilityHistory", ["availability"])],
}


def ros(data):
    require(len(data) >= 41 and bytes(data[:8]) == b"UWROS1\0\0", "invalid ROS environment header")
    require(struct.unpack_from("<II", data, 8) == (1, 0), "unsupported ROS environment version")
    label, backend = bytes(data[16:32]), data[32]
    require(any(label) and backend in (1, 2), "invalid ROS environment identity")
    at = 33

    def width(kind):
        if isinstance(kind, list) or kind in ("s", "blob"):
            return 8
        if isinstance(kind, tuple):
            return width(kind[0]) * len(kind[1])
        if kind in _ROS_TYPES:
            return sum(width(t) for _, t in _ROS_TYPES[kind])
        return struct.calcsize("<" + kind)

    def read(kind):
        nonlocal at
        if isinstance(kind, list):
            count = read("Q")
            require(count <= (len(data) - at) // width(kind[0]), "invalid ROS collection extent")
            return [read(kind[0]) for _ in range(count)]
        if isinstance(kind, tuple):
            return {name: read(kind[0]) for name in kind[1]}
        if kind in _ROS_TYPES:
            return {name: read(t) for name, t in _ROS_TYPES[kind]}
        if kind in ("s", "blob"):
            count = read("Q")
            require(count <= len(data) - at, "truncated ROS byte string")
            b = bytes(data[at:at+count]); at += count
            if kind == "blob": return b
            require(b"\0" not in b, "ROS text contains NUL")
            return b.decode("utf-8", errors="backslashreplace")
        n = struct.calcsize("<" + kind)
        require(n <= len(data) - at, "truncated ROS scalar")
        require(kind != "?" or data[at] <= 1, "invalid ROS boolean")
        value = struct.unpack_from("<" + kind, data, at)[0]; at += n
        return value

    def ordered(rows, key):
        require(all(key(a) < key(b) for a, b in zip(rows, rows[1:])), "duplicate or unordered ROS rows")

    def fields(rows):
        ordered(rows, lambda r: r["name"])
        require(all(r["name"] for r in rows), "empty ROS field name")

    groups = read(["group"])
    require(at == len(data), "trailing ROS bytes")
    ordered(groups, lambda g: (g["scope"], g["name"]))
    members = set()
    for g in groups:
        require(g["scope"] <= 2 and g["members"] and (g["scope"] != 1 or len(g["members"]) == 1), "invalid ROS group")
        ordered(g["members"], lambda m: m["module"])
        require(g["sessionPresent"] == g["registered"], "ROS session registration mismatch")
        require(g["sessionPresent"] or not (g["connected"] or g["nextServiceRequest"] or g["sessionLabel"] or
                g["handles"] or g["observations"] or g["availabilityHistory"]), "unregistered ROS state")
        for m in g["members"]:
            require(m["module"] not in members and m["name"] and (not m["registered"] or g["registered"]), "invalid ROS member")
            members.add(m["module"])
        fields(g["connection"]); fields(g["interfaces"])
        require(all(r["value"] in ("0", "1", "2") for r in g["interfaces"]), "invalid ROS interface access")
        ordered(g["capabilities"], lambda c: c["alias"])
        aliases = {c["alias"] for c in g["capabilities"]}
        for c in g["capabilities"]:
            require(c["alias"] and c["operation"], "empty ROS capability")
            fields(c["limits"])
            require(all(c["accounting"][n] <= 1 for n in ("limitedRate", "limitedBytes", "limitedLog")) and
                    c["audit"]["saturated"] <= 1, "invalid ROS accounting flags")
        ordered(g["handles"], lambda h: h["slot"])
        for h in g["handles"]:
            require(h["slot"] and h["generation"] and h["alias"] in aliases and not (h["active"] and h["retired"]), "invalid ROS handle")
        ordered(g["observations"], lambda o: o["handle"])
        for o in g["observations"]:
            q, state = o["local"], o["local"]["state"]
            require(o["handle"] and o["sourceHandle"] and o["observerAlias"] in aliases and o["sourceAlias"] in aliases and
                    state["overflow"] <= 4 and state["mask"] <= 127 and state["sequence"] and state["pinnedSource"] <= 4, "invalid ROS observation")
            ordered(o["requests"], lambda n: n)
            previous, request, optionals = 0, 0, set()
            for event in q["events"]:
                loc = event["location"]
                require(previous <= loc <= 4 and (event["present"] or loc == 4), "invalid ROS queue location")
                previous = loc
                if 0 < loc < 4:
                    require(loc not in optionals, "duplicate ROS control/pending slot"); optionals.add(loc)
                if loc == 4:
                    require(event["request"] > request, "invalid ROS reservation"); request = event["request"]
                else:
                    require(not event["request"] and not event["responseCredit"], "unexpected ROS reservation credit")
                if event["present"]:
                    kind = event["kind"]
                    require(1 <= kind <= 7 and event["ownedBytes"] >= 64 + len(event["payload"]), "invalid ROS event extent")
                    require(kind < 6 or (not event["payload"] and not event["flags"] & ~3), "invalid ROS control event")
                    require(kind != 6 or event["error"], "missing ROS error")
                    require(kind != 7 or event["flags"] & 1, "missing ROS retirement flag")
                else:
                    require(not any(event[k] for k in ("kind", "flags", "error", "correlation", "payload")), "absent ROS response has content")
        ordered(g["availabilityHistory"], lambda a: a["handle"])
        for a in g["availabilityHistory"]:
            require(a["handle"] and a["sourceHandle"] and a["availabilityAlias"] in aliases and a["sourceAlias"] in aliases, "invalid ROS availability binding")

    def describe(kind, value):
        if isinstance(kind, list): return [describe(kind[0], v) for v in value]
        if isinstance(kind, tuple): return {n: describe(kind[0], value[n]) for n in kind[1]}
        if kind in _ROS_TYPES: return {n: describe(t, value[n]) for n, t in _ROS_TYPES[kind]}
        if kind == "blob": return value.hex()
        return str(value) if kind == "Q" else value

    return {"backend": backend, "recording": label.hex(), "groups": describe(["group"], groups),
            "externalEnvironmentCaptured": False, "grantsRestoreAuthority": False}


def diagnostic(data):
    require(len(data) >= 64, "truncated diagnostic header")
    magic, version, header, stop, reason, exit_status, threads, count, flags, reserved = struct.unpack_from("<8sIIQIIQQQQ", data)
    require(magic == b"UWDIAG1\0" and version == 1 and header == 64 and stop > 0 and not flags and not reserved,
            "invalid diagnostic header")
    require(count > 0 and count == (len(data) - 64) // 48 and (len(data) - 64) % 48 == 0,
            "invalid diagnostic frame extent")
    previous, ordinal, actual_threads = 0, 0, 0
    frames = []
    for offset in range(64, len(data), 48):
        thread, index, module, function, generation, location = struct.unpack_from("<6Q", data, offset)
        require(thread > 0, "invalid diagnostic thread")
        if thread != previous:
            require(thread > previous and index == 0, "unordered diagnostic threads")
            previous, ordinal, actual_threads = thread, 0, actual_threads + 1
        require(index == ordinal, "noncontiguous diagnostic frame ordinals")
        ordinal += 1
        frames.append({"thread": str(thread), "ordinal": str(index), "module": str(module),
                       "function": str(function), "generation": str(generation) if generation else None,
                       "wasmOffset": None if location == (1 << 64) - 1 else str(location)})
    require(actual_threads == threads, "diagnostic thread census mismatch")
    return {"stopId": str(stop), "stopReason": reason, "exitStatus": exit_status, "frames": frames}


def scene_observation(record, records):
    """Decode the RSC1 first pages only in their stopped Core Dump context."""
    from dap_adapter import parse_scene_state

    require(record.identifier == 1 and record.capture == 1, "invalid scene observation record")
    diagnostic_rows = [r for r in records if r.kind == 1]
    state_rows = [r for r in records if r.kind == 2]
    core = next((r for r in records if r.kind == 10 and r.identifier == 0), None)
    require(len(diagnostic_rows) == 1 and diagnostic_rows[0].capture == 1 and
            len(state_rows) == 1 and state_rows[0].identifier == 0 and state_rows[0].capture == 3 and
            core is not None and core.capture == 1 and core.schema == 1 and
            core.payload[:8] == b"\0asm\1\0\0\0", "scene observation requires a stopped inspection Core Dump")
    data = record.payload
    require(16 <= len(data) <= 16 + 3 * (4 + 65536) and data[:8] == b"UWSCN1\0\0",
            "invalid scene observation header")
    stop, = struct.unpack_from("<Q", data, 8)
    require(str(stop) == diagnostic(diagnostic_rows[0].payload)["stopId"], "scene observation stop mismatch")
    pages = {}
    at = 16
    for selected in ("authority", "resources", "audit"):
        require(at + 4 <= len(data), "truncated scene page size")
        size, = struct.unpack_from("<I", data, at)
        at += 4
        require(0 < size <= 65536 and at + size <= len(data), "invalid scene page extent")
        packet = bytes(data[at:at + size])
        at += size
        require(all(c == 10 or 32 <= c <= 126 for c in packet), "invalid scene page encoding")
        wire = packet.decode("ascii")
        prefix = f"scene scope=process-owned stop={stop} selection={selected} status="
        if wire in (prefix + "busy\n", prefix + "bounded scan limit\n"):
            pages[selected] = {"available": False, "status": wire[len(prefix):-1]}
        else:
            try:
                pages[selected] = parse_scene_state(wire, {"selection": selected, "start": 0, "count": 8}, stop)
            except ValueError as exc:
                raise InvalidSnapshot(f"invalid scene {selected} page: {exc}") from exc
    require(at == len(data), "trailing scene observation bytes")
    return {"scope": "process-owned", "stopId": str(stop), "pageCapacity": 8, "pages": pages,
            "restoreAuthority": False, "providerStateRestorable": False}


def wasip1(data):
    """Validate canonical WASIp1 DATA; never open or resolve a saved resource."""
    require(len(data) >= 168 and data[:8] == b"UWWASI1\0", "invalid WASIp1 environment header")
    version, reserved = struct.unpack_from("<II", data, 8)
    require(version == 1 and reserved == 0, "unsupported WASIp1 environment schema")
    label = bytes(data[16:32])
    module, opens, *counts = struct.unpack_from("<9Q", data, 96)
    require(any(label) and opens <= (1 << 31), "invalid WASIp1 identity/table size")
    remaining = len(data) - 168
    for count, width in zip(counts, (8, 8, 8, 40, 24, 24, 4)):
        require(count <= remaining // width, "invalid WASIp1 collection extent")
        remaining -= count * width
    at = 168

    def integer(width):
        nonlocal at
        require(width <= len(data) - at, "truncated WASIp1 integer")
        value = int.from_bytes(data[at:at + width], "little")
        at += width
        return value

    def blob(text=False):
        nonlocal at
        length = integer(8)
        require(length <= len(data) - at, "truncated WASIp1 byte string")
        value = data[at:at + length]
        at += length
        require(not text or 0 not in value, "WASIp1 text contains NUL")
        return value

    members = [integer(8) for _ in range(counts[0])]
    require(members and module in members and all(a < b for a, b in zip(members, members[1:])),
            "invalid WASIp1 policy membership")
    def text_data(value):
        value = bytes(value)
        try:
            return {"text": value.decode("utf-8"), "encoding": "utf-8", "bytes": len(value)}
        except UnicodeDecodeError:
            return {"hex": value.hex(), "encoding": "bytes", "bytes": len(value)}

    arguments = [text_data(blob(text=True)) for _ in range(counts[1])]
    variables = [text_data(blob(text=True)) for _ in range(counts[2])]
    resources = []
    names = ("external-file", "directory", "socket", "managed-file", "stdio")
    for _ in range(counts[3]):
        kind, stdio, options, zero, flags, zero2, offset = (integer(n) for n in (1, 1, 1, 1, 2, 2, 8))
        require(kind < len(names) and options <= 7 and not zero and not zero2 and flags <= 31,
                "invalid WASIp1 resource metadata")
        offset_known, flags_known, follow = (bool(options & bit) for bit in (1, 2, 4))
        mount, path, content = blob(text=True), blob(text=True), blob()
        require((flags_known or not flags) and (offset_known or not offset), "unknown WASIp1 resource value")
        require(stdio <= 2 if kind == 4 else not stdio, "invalid WASIp1 stdio identity")
        require(not content or kind == 3, "external resource content cannot be rolled back")
        if path:
            parts = bytes(path).split(b"/")
            require(b"\\" not in bytes(path) and b":" not in bytes(path) and all(p and p not in (b".", b"..") for p in parts),
                    "noncanonical WASIp1 guest route")
        require((not path and not follow) or mount, "WASIp1 route has no guest mount")
        if kind == 3:
            require(offset_known and flags_known and not mount and not path and not follow, "invalid managed file")
        if kind in (2, 4):
            require(not offset_known and not mount and not path and not follow, "invalid external stream")
        if kind == 1:
            require(not offset_known and mount, "invalid guest directory")
        resources.append({"kind": names[kind], "contentBytes": len(content),
                          "requiresCurrentBinding": kind != 3, "offset": str(offset) if offset_known else None,
                          "flags": flags if flags_known else None, "mount": text_data(mount), "path": text_data(path),
                          "stdioIndex": stdio if kind == 4 else None, "follow": follow,
                          "contentSha256": hashlib.sha256(content).hexdigest() if kind == 3 else None,
                          "contentPreviewHex": bytes(content[:256]).hex() if kind == 3 else None})
    numbers, used, bindings, reserved, closed = [], set(), [], [], []
    for _ in range(counts[4]):
        fd, resource, base, inheriting = (integer(n) for n in (4, 4, 8, 8))
        require(fd < (1 << 31) and resource < len(resources) and not (base | inheriting) & ~0x3fffffff,
                "invalid WASIp1 descriptor binding")
        numbers.append(fd)
        used.add(resource)
        bindings.append({"descriptor": fd, "resource": resource, "baseRights": hex(base), "inheritingRights": hex(inheriting)})
    protected = 0
    for _ in range(counts[5]):
        fd, bits, zero, base, inheriting = (integer(n) for n in (4, 1, 3, 8, 8))
        require(fd < (1 << 31) and bits <= 3 and not zero and not (base | inheriting) & ~0x3fffffff,
                "invalid WASIp1 reserved descriptor")
        require(not bits & 2 or not (base | inheriting), "protected descriptor carries rights")
        protected += bool(bits & 2)
        reserved.append({"descriptor": fd, "nullResource": bool(bits & 1), "protected": bool(bits & 2),
                         "baseRights": hex(base), "inheritingRights": hex(inheriting)})
        numbers.append(fd)
    for _ in range(counts[6]):
        fd = integer(4)
        require(fd < opens, "invalid WASIp1 closed descriptor")
        numbers.append(fd)
        closed.append(fd)
    require(at == len(data) and len(used) == len(resources), "trailing/unreferenced WASIp1 data")
    numbers.sort()
    require(len(numbers) >= opens and all(a < b for a, b in zip(numbers, numbers[1:])), "duplicate/missing WASIp1 descriptor")
    require(all(fd == i for i, fd in enumerate(numbers[:opens])), "incomplete WASIp1 descriptor table")
    return {"module": str(module), "members": [str(m) for m in members], "argumentCount": counts[1],
            "environmentCount": counts[2], "resources": resources, "descriptorCount": counts[4],
            "protectedSlots": protected, "closedCount": counts[6], "restoreAuthority": False,
            "recording": label.hex(), "originalWasmSHA256": bytes(data[32:64]).hex(),
            "arguments": arguments, "variables": variables,
            "bindings": bindings, "reserved": reserved, "closed": closed}


_THREAD_KINDS = ("invalid", "origin", "enter", "instruction", "leave", "spawn", "host-begin",
                 "memory-read", "memory-write", "host-end", "wait-begin", "wait-end", "notify",
                 "entropy", "exit-begin", "exit-end", "memory-grow", "trap", "table-grow")


def threaded_history(data, records, page=None):
    """Read logical data only; no scheduler, native handle or restore authority."""
    at = 0
    def number():
        nonlocal at
        require(at <= len(data) - 8, "truncated threaded scalar")
        value, = struct.unpack_from("<Q", data, at); at += 8
        return value
    def blob(size):
        nonlocal at
        require(size <= len(data) - at, "truncated threaded payload")
        value = data[at:at+size]; at += size
        return value
    require(number() == 0x54484a38, "unsupported threaded event schema")
    count = number()
    modules = {r.identifier: r for r in records if r.kind == 3}
    require(count == len(modules) == len(records) - 1, "threaded source census mismatch")
    source_hash = hashlib.sha256(struct.pack("<Q", count)); names = set(); ids = set(); sources = []; previous_name = b""
    for _ in range(count):
        identity, size = number(), number(); name = bytes(blob(size))
        require(identity == len(sources) and identity in modules and identity not in ids and name and name not in names and previous_name < name,
                "invalid canonical threaded source identity")
        previous_name = name
        ids.add(identity); names.add(name); wasm = modules[identity].payload
        source_hash.update(struct.pack("<QQ", identity, len(name))); source_hash.update(name)
        source_hash.update(struct.pack("<Q", len(wasm))); source_hash.update(wasm)
        sources.append({"module": str(identity), "name": name.decode("utf-8", errors="backslashreplace")})
    length = number(); require(length <= len(data) - at, "invalid threaded journal extent")
    journal_end = at + length
    require(bytes(blob(8)) == b"UTHJNL7\0", "unsupported threaded journal format")
    flags, count, target = number(), number(), number()
    require(flags in (0, 1, 2) and count >= 2 and count <= (len(data)-at)//80 and target <= count,
            "invalid threaded journal header")
    workers, births, live_tids, totals, selected = {}, {}, set(), {}, []
    terminal = False
    for index in range(count):
        kind, worker, module, function, offset, a, b, c, ins, outs = [number() for _ in range(10)]
        inp, out = blob(ins), blob(outs)
        require(0 < kind < len(_THREAD_KINDS) and worker, "invalid threaded event identity")
        label = _THREAD_KINDS[kind]; totals[label] = totals.get(label, 0) + 1
        if index == 0:
            require(kind == 1 and worker == 1 and ins == 64 and bytes(inp[32:]) == source_hash.digest(),
                    "threaded source digest mismatch")
            recording = bytes(inp[:16])
            if not any(recording): recording = b"\x01" + bytes(15)
            continue
        require(kind != 1, "duplicate threaded origin")
        if kind == 2:
            require(not terminal and worker not in workers and a < (1 << 29) and not outs and not(module or function or offset) and b <= 0xffffffff,
                    "invalid threaded worker entry")
            require(a not in live_tids, "duplicate live threaded TID")
            if not workers: require(worker == 1 and a == 0, "missing threaded main entry")
            else:
                require(a and not ins and births.pop(a, None) == b, "unmatched dynamic thread creation")
            workers[worker] = {"worker": str(worker), "tid": a, "phase": 0, "enteredAt": index, "leftAt": None}
            live_tids.add(a)
        else:
            require(worker in workers and workers[worker]["phase"] != 3, "event for a departed/unknown worker")
            w = workers[worker]; phase = w["phase"]
            if kind in (3, 12, 13, 16, 18): require(not terminal and phase == 0, "instruction outside runnable phase")
            if kind == 3: require(not(ins or outs or a or b or c), "invalid instruction event")
            elif kind == 4:
                require(not phase and not(ins or b or c) and a == w["tid"], "invalid thread departure")
                w["phase"] = 3; w["leftAt"] = index; live_tids.discard(a); terminal |= a == 0
            elif kind == 5:
                require(not terminal and not phase and not(ins or c) and outs == 4 and a <= 0xffffffff, "invalid spawn event")
                tid, = struct.unpack("<i", out)
                require(tid == -1 or 0 < tid < (1 << 29), "invalid spawn result")
                if tid > 0:
                    require(tid not in live_tids and tid not in births, "duplicate pending TID")
                    births[tid] = a
            elif kind == 6:
                require(not terminal and not phase and not outs and (b >> 1) in modules, "invalid native import entry"); w["phase"] = 1
            elif kind in (7, 8):
                require(phase == 1 and not ins and outs == 1 + c * (2 if kind == 8 else 1) and out[0] in (0, 1), "invalid native memory transaction")
            elif kind == 9:
                require(phase == 1 and not(ins or b or c) and outs == a, "invalid native import result"); w["phase"] = 0
            elif kind == 10:
                require(not terminal and not phase and a in (4, 8) and ins == 8 and outs == a, "invalid atomic wait entry"); w["phase"] = 2
            elif kind == 11:
                require(phase == 2 and not(ins or a or b or c) and outs == 16, "invalid atomic wait result"); w["phase"] = 0
            elif kind == 12:
                require(not(ins or c) and outs == 4 and struct.unpack("<I", out)[0] <= b <= 0xffffffff, "invalid atomic notification")
            elif kind == 13:
                require(not(ins or a or b or c) and outs == 2 and out[0] in (0, 1), "invalid MTE entropy")
            elif kind == 14:
                require(not terminal and not phase and not(ins or b or c) and outs == 4 and a <= 0xffffffff, "invalid proc_exit entry"); w["phase"] = 4
            elif kind == 15:
                require(not terminal and phase == 4 and not(ins or outs or b or c) and a <= 0xffffffff, "invalid proc_exit completion")
                terminal = True; w["phase"] = 3; w["leftAt"] = index
            elif kind == 16:
                require(not ins and outs == 8 and struct.unpack("<Q", out)[0] in (2**64 - 1, b), "invalid memory growth")
            elif kind == 18:
                require(not ins and outs == 8 and a <= 0xffffffff and struct.unpack("<Q", out)[0] in (2**64 - 1, c), "invalid table growth")
            elif kind == 17:
                require(not terminal and not(ins or outs or b or c) and a in (120, 121, 122), "invalid terminal trap")
                terminal = True; w["phase"] = 3; w["leftAt"] = index
        if page is not None and page[0] <= index < page[0] + page[1]:
            row = {"index": index, "kind": label, "worker": str(worker), "module": str(module), "function": str(function),
                   "wasmOffset": f"0x{offset:x}", "a": str(a), "b": str(b), "c": str(c), "inputBytes": ins, "outputBytes": outs}
            if kind == 6:
                row["importReceiverModule"] = str(b >> 1)
                row["importFamily"] = "ros-v1" if b & 1 else "wasip1"
            # Payload sizes are not a display truncation of the recording. The
            # complete bytes remain in the archive; this view avoids expanding
            # an entire shared memory into an unbounded JSON response.
            for key, value in (("input", inp), ("output", out)):
                if len(value) <= 32: row[key + "Hex"] = bytes(value).hex()
                else: row[key + "SHA256"] = hashlib.sha256(value).hexdigest()
            selected.append(row)
    unknown = not terminal and any(w["phase"] in (1, 4) for w in workers.values())
    require(at == journal_end and terminal == bool(flags & 1) and unknown == bool(flags & 2), "threaded terminal/retention mismatch")
    samples, previous = [], 0
    sample_count = number()
    require(sample_count <= (len(data) - at) // 16, "invalid threaded environment count")
    for sample_index in range(sample_count):
        cursor, size = number(), number(); payload = bytes(blob(size))
        require(previous <= cursor <= count and cursor, "invalid threaded environment position")
        previous = cursor
        # Environment containers cannot recursively contain execution histories.
        require(len(payload) >= 112 and struct.unpack_from("<I", payload, 40)[0] == 0,
                "invalid historical environment container")
        saved = decode(payload)
        require(saved.disclosure == "full", "historical environment disclosure mismatch")
        environments = []
        for record in saved.records:
            require(record.kind in (6, 7, 8), "unexpected historical component")
            value = wasip1(record.payload) if record.kind == 6 else ros(record.payload)
            require(value["recording"] == recording.hex(), "historical recording identity mismatch")
            if record.kind == 6:
                identity = int(value["module"])
                require(identity in modules and value["originalWasmSHA256"] == hashlib.sha256(modules[identity].payload).hexdigest(),
                        "historical Wasm identity mismatch")
                require(all(int(m) in modules for m in value["members"]), "unknown historical WASI member")
            else:
                require(all(int(m["module"]) in modules for g in value["groups"] for m in g["members"]),
                        "unknown historical ROS member")
            environments.append({"kind": KINDS[record.kind], "id": str(record.identifier), "data": value})
        samples.append({"sample": sample_index, "cursor": cursor, "environments": environments})
    require(at == len(data), "trailing threaded archive data")
    result = {"format": "private-thread-journal-v7", "events": count, "replayUntil": target or count,
              "terminal": terminal, "retainedNativePending": unknown, "externalEffectsOnReplay": False,
              "crossProcessUnknownResult": False, "sourceModules": sources, "eventCounts": totals,
              "workers": list(workers.values()), "restoreAuthority": False, "environmentSamples": samples}
    if page is not None: result["page"] = selected
    return result


def decode(data: bytes) -> Snapshot:
    # Copy caller-mutable input once. No borrowed writable/truncatable file map.
    if not isinstance(data, bytes):
        data = bytes(data)
    require(len(data) >= 112, "truncated snapshot")
    magic, version, header, arch, system, endian, pointer_bits, disclosure, reserved = struct.unpack_from("<8sIIHH4B", data)
    require(magic == MAGIC and version == 1 and header == 64 and reserved == 0, "invalid snapshot header/version")
    require(arch < len(ARCH) and system < len(OS) and endian in (1, 2) and pointer_bits in (32, 64), "invalid origin platform")
    require(not arch or pointer_bits == (64 if arch in (1, 3) else 32), "origin pointer width mismatch")
    require(disclosure in (1, 2), "invalid snapshot disclosure")
    product = struct.unpack_from("<4I", data, 24)
    logical, continuation, features, count = struct.unpack_from("<IIQQ", data, 40)
    end = len(data) - 48
    require(data[end:end + 8] == COMMIT and struct.unpack_from("<Q", data, end + 8)[0] == len(data), "incomplete snapshot commit")
    require(hashlib.sha256(memoryview(data)[:-32]).digest() == data[-32:], "snapshot checksum mismatch")
    require(count <= (end - 64) // 32, "invalid snapshot record count")
    at, previous = 64, (0, 0)
    extents = []
    for _ in range(count):
        require(at <= end - 32, "truncated record header")
        kind, capture = struct.unpack_from("<HB", data, at)
        identifier, schema, size = struct.unpack_from("<QQQ", data, at + 8)
        require(kind in KINDS and capture < len(CAPTURE) and data[at + 3:at + 8] == b"\0" * 5 and schema > 0,
                "invalid record header")
        require((kind, identifier) > previous, "duplicate or unordered snapshot record")
        require(disclosure != 1 or kind in (1, 3, 4, 5), "diagnostic snapshot contains a dynamic component")
        at += 32
        require(size <= end - at and (capture != 3 or size == 0), "invalid snapshot record extent")
        extents.append((kind, capture, identifier, schema, at, size))
        previous, at = (kind, identifier), at + size
    require(at == end, "trailing snapshot bytes")
    records = tuple(Record(kind, capture, identifier, schema, memoryview(data)[start:start + size])
                    for kind, capture, identifier, schema, start, size in extents)
    observed_scene = None
    for record in records:
        if record.kind == 10 and record.schema == SCENE_OBSERVATION_SCHEMA:
            observed_scene = scene_observation(record, records)
        # A pending continuation is a strictly shaped observation. Reject it at
        # decoding, including extraction callers that never call describe(). It
        # must never be mistaken for a complete/absent native result.
        if record.kind == 11:
            require(record.capture == 1 and record.identifier == 0 and record.schema == 1 and len(record.payload) == 32,
                    "invalid retained native continuation")
        if record.capture == 3:
            continue
        if record.kind == 1:
            require(record.schema == 1 and record.identifier == 0, "unsupported diagnostic schema")
            diagnostic(record.payload)
        elif record.kind == 6:
            require(record.schema == 1 and record.capture == 0, "unsupported WASIp1 snapshot record")
            state = wasip1(record.payload)
            require(int(state["module"]) == record.identifier, "WASIp1 record identity mismatch")
        elif record.kind in (7, 8):
            require(record.schema == 2 and record.capture == 1 and record.identifier == 0, "unsupported ROS snapshot record")
            state = ros(record.payload)
            require(state["backend"] == record.kind - 6, "ROS backend mismatch")
        elif record.kind == 3:
            require(record.schema == 1 and record.capture == 0 and record.payload[:8] == b"\0asm\1\0\0\0", "invalid static Wasm module")
        elif record.kind == 4:
            require(record.schema == 1 and record.capture == 0 and len(record.payload) >= 32, "invalid replacement body")
            module, function, generation, size = struct.unpack_from("<4Q", record.payload)
            require(generation > 1 and size > 0 and size == len(record.payload) - 32, "invalid replacement body extent")
    require(logical not in (2, 3, 4, 5, 6), "obsolete threaded identity ABI")
    if any(r.kind == 9 and r.schema == 0x54484a38 for r in records):
        require(logical == 7, "threaded journal requires its exact identity ABI")
    if logical == 7:
        require(disclosure == 2 and continuation == 0 and system == 1 and endian == 1 and arch != 0,
                "invalid threaded archive platform/disclosure")
        require(records and records[-1].kind == 9 and records[-1].schema == 0x54484a38 and
                records[-1].capture == 0 and records[-1].identifier == 0, "missing threaded execution journal")
        threaded_history(records[-1].payload, records)
    return Snapshot(data, {"architecture": ARCH[arch], "os": OS[system], "byteOrder": "little" if endian == 1 else "big",
                          "pointerBits": pointer_bits, "productVersion": list(product), "logicalABI": logical,
                          "continuationABI": continuation, "features": str(features)},
                    "diagnostic" if disclosure == 1 else "full", records, observed_scene)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    info = sub.add_parser("info", help="inspect platform, disclosure, completeness and fault coordinates")
    info.add_argument("file", type=Path)
    extract = sub.add_parser("extract-wasm", help="extract the original static module, without executing it")
    extract.add_argument("file", type=Path)
    extract.add_argument("--module", type=int, required=True)
    extract.add_argument("--output", type=Path, required=True)
    events = sub.add_parser("thread-events", help="inspect a page of a private dynamic-thread execution journal")
    events.add_argument("file", type=Path)
    events.add_argument("--offset", type=int, default=0)
    events.add_argument("--count", type=int, default=64)
    args = parser.parse_args()
    try:
        saved = decode(args.file.read_bytes())
        if args.command == "info":
            print(json.dumps(saved.describe(), indent=2))
        elif args.command == "thread-events":
            require(args.offset >= 0 and args.count > 0, "invalid thread event page")
            found = [r for r in saved.records if r.kind == 9 and r.schema == 0x54484a38]
            require(len(found) == 1, "private threaded journal is unavailable")
            print(json.dumps(threaded_history(found[0].payload, saved.records, (args.offset, args.count)), indent=2))
        else:
            found = [r for r in saved.records if r.kind == 3 and r.identifier == args.module and r.capture == 0]
            require(len(found) == 1, "requested original Wasm module is unavailable")
            flags = os.O_WRONLY | os.O_CREAT | os.O_EXCL | getattr(os, "O_BINARY", 0) | getattr(os, "O_NOFOLLOW", 0)
            with os.fdopen(os.open(args.output, flags, 0o600), "wb") as output:
                output.write(found[0].payload)
                output.flush()
                os.fsync(output.fileno())
    except (OSError, InvalidSnapshot, MemoryError) as error:
        parser.exit(1, f"snapshot: {error}\n")


if __name__ == "__main__":
    main()
