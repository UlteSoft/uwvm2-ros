#!/usr/bin/env python3
"""Stream retained host-call observations to a private, readable JSON file.

Only authenticated, stop-bound broker queries are used. No guest permission is
changed, no recording is enabled, and no native pointer is dereferenced.
"""
import argparse
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import re
import tempfile

FAMILIES = {1: "wasip1", 2: "ros1", 4: "ros2"}
U64 = (1 << 64) - 1
HEADER = {"enabled", "epoch", "cleared-through", "newest", "allocation-failures",
          "invalid-ranges", "sequence-failures"}
CALL = {"id", "family", "namespace", "function", "module-name", "module-name-bytes",
        "module-name-truncated", "module", "participant", "generation", "environment",
        "args", "returned", "has-result", "result", "timed", "duration-ns", "segments", "incomplete"}
TOKEN = re.compile(r'([a-z-]+)=("(?:[\x20-\x21\x23-\x5b\x5d-\x7e]|\\x[0-9a-f]{2})*"|[^ "\r\n]*)')


def fields(line, prefix, keys):
    if not line.startswith(prefix + " "):
        raise ValueError("unexpected host trace row")
    text, result, pos = line[len(prefix) + 1:], {}, 0
    while pos < len(text):
        match = TOKEN.match(text, pos)
        if match is None or match[1] in result:
            raise ValueError("malformed or duplicate host trace field")
        result[match[1]] = match[2]
        pos = match.end()
        if pos < len(text):
            if text[pos] != " ":
                raise ValueError("invalid host trace field separator")
            pos += 1
            if pos == len(text):
                raise ValueError("trailing host trace separator")
    if result.keys() != keys:
        raise ValueError("missing or unknown host trace field")
    return result


def number(text, maximum=U64):
    if re.fullmatch(r"0|[1-9][0-9]{0,19}", text) is None or int(text) > maximum:
        raise ValueError("invalid host trace integer")
    return int(text)


def quoted(text):
    if not text.startswith('"') or not text.endswith('"'):
        raise ValueError("quoted host trace bytes required")
    body, result, pos = text[1:-1], bytearray(), 0
    while pos < len(body):
        if body[pos] == "\\":
            if re.fullmatch(r"\\x[0-9a-f]{2}", body[pos:pos+4]) is None:
                raise ValueError("invalid byte escape")
            result.append(int(body[pos+2:pos+4], 16))
            pos += 4
        else:
            if not 32 <= ord(body[pos]) < 127 or body[pos] == '"':
                raise ValueError("unescaped host trace byte")
            result.append(ord(body[pos]))
            pos += 1
    return bytes(result)


def readable(data):
    try:
        return data.decode("utf-8", "strict")
    except UnicodeDecodeError:
        return None


def encoded(data):
    return {"hex": data.hex(), "utf8IfValid": readable(data)}


def packet(reply):
    if not isinstance(reply, str) or len(reply) > 65536 or not reply.endswith("\n"):
        raise ValueError("incomplete host trace reply")
    lines = reply[:-1].split("\n")
    h = {k: number(v) for k, v in fields(lines[0], "host-trace", HEADER).items()}
    if h["enabled"] > 7 or h["cleared-through"] > h["newest"]:
        raise ValueError("inconsistent host trace journal")
    return h, lines[1:]


def call_row(line):
    row = fields(line, "host-call", CALL)
    for key in CALL - {"namespace", "function", "module-name", "args"}:
        row[key] = number(row[key])
    if not row["id"] or row["family"] not in FAMILIES:
        raise ValueError("invalid host call identity")
    for key in ("module-name-truncated", "returned", "has-result", "timed", "incomplete"):
        if row[key] > 1:
            raise ValueError("invalid host call Boolean")
    for key in ("namespace", "function", "module-name"):
        row[key] = quoted(row[key])
    length = row["module-name-bytes"]
    if (len(row["module-name"]) != min(255, length) or
            bool(row["module-name-truncated"]) != (length > 255)):
        raise ValueError("inconsistent host module label")
    arguments = []
    for arg in row["args"].split(",") if row["args"] else []:
        parts = arg.split(":")
        if len(parts) != 2 or parts[0] not in ("32", "64"):
            raise ValueError("invalid ABI argument")
        width = int(parts[0])
        value = number(parts[1], (1 << width) - 1)
        arguments.append({"widthBits": width, "unsigned": str(value),
                          "hex": f"0x{value:0{width//4}x}"})
    if len(arguments) > 16:  # Current ABI record schema, not a history limit.
        raise ValueError("invalid ABI argument count")
    row["args"] = arguments
    return row


def save_host_trace(request, checkpoint, path, family="all"):
    """request(str)->str, checkpoint()->stable stop identity ending in stop ID.

    Export memory is bounded by protocol pages, independent of retained history
    or buffer length. Publish a new file only after the final stop/journal check.
    Existing files and symlinks are never overwritten.
    """
    if family not in ("all", *FAMILIES.values()):
        raise ValueError("family must be all, wasip1, ros1 or ros2")
    if not isinstance(path, str) or not path or "\0" in path:
        raise ValueError("a host output file path is required")
    destination = Path(os.path.abspath(path))
    identity = checkpoint()
    stop = identity[-1]
    if type(stop) is not int or not 0 < stop <= U64:
        raise ValueError("a current cooperative stop is required")
    journal = None

    def query(command):
        nonlocal journal
        h, rows = packet(request(command + f" if-stop={stop}"))
        if journal is None:
            journal = h
        elif h != journal:
            raise ValueError("host trace changed during export; file not published")
        return rows

    rows = query(f"trace host read family={family} after=0 count=8")
    descriptor, temporary = tempfile.mkstemp(prefix=".uwvm-host-trace-", suffix=".tmp",
                                            dir=destination.parent)
    summary = {"calls": 0, "incompleteCalls": 0, "unreturnedCalls": 0, "payloadBytes": 0}
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="\n") as output:
            def write(value, indent):
                text = json.dumps(value, ensure_ascii=False, indent=2, allow_nan=False)
                output.write(text.replace("\n", "\n" + " " * indent))

            metadata = {
                "schema": "uwvm.host-trace.v1",
                "exportedAtUtc": datetime.now(timezone.utc).isoformat(),
                "stopId": str(stop), "familyFilter": family,
                "journal": {k: str(v) for k, v in journal.items()},
                "semantics": {
                    "coverage": "Retained observations only, from explicitly enabled recording intervals.",
                    "wasip1": "Before/after snapshots, not proof of kernel consumption under shared-memory races.",
                    "ros": "Copied Host transfers, not proof of remote delivery; inspect the result.",
                    "integers": "64-bit quantities and ABI values are decimal strings to avoid JSON reader rounding.",
                    "bytes": "Concatenate chunks in offset order. Hex is lossless; utf8IfValid is per chunk.",
                    "timing": "durationNs is monotonic elapsed time, not a wall-clock timestamp.",
                },
            }
            output.write(json.dumps(metadata, ensure_ascii=False, indent=2)[:-2])
            output.write(',\n  "calls": [')
            after, first_call = 0, True
            while True:
                if not rows:
                    raise ValueError("missing host call pagination")
                cursor = fields(rows[-1], "host-calls", {"next", "more"})
                next_id, more = number(cursor["next"]), number(cursor["more"], 1)
                if len(rows) > 9:
                    raise ValueError("oversized host call page")
                for line in rows[:-1]:
                    call = call_row(line)
                    if not journal["cleared-through"] < call["id"] <= journal["newest"] or call["id"] <= after:
                        raise ValueError("out-of-order or retired host call")
                    if family != "all" and FAMILIES[call["family"]] != family:
                        raise ValueError("host call family differs from request")
                    after = call["id"]
                    output.write("\n    " if first_call else ",\n    ")
                    first_call = False
                    public = {"id": str(call["id"]), "family": FAMILIES[call["family"]],
                              "namespace": readable(call["namespace"]), "function": readable(call["function"]),
                              "namespaceBytes": encoded(call["namespace"]), "functionBytes": encoded(call["function"]),
                              "module": str(call["module"]), "participant": str(call["participant"]),
                              "generation": str(call["generation"]), "environment": str(call["environment"]),
                              "arguments": call["args"], "returned": bool(call["returned"]),
                              "hasResult": bool(call["has-result"]), "resultUnsigned": str(call["result"]),
                              "timed": bool(call["timed"]), "durationNs": str(call["duration-ns"]),
                              "incomplete": bool(call["incomplete"])}
                    text = json.dumps(public, ensure_ascii=False, indent=2)
                    output.write(text[:-2].replace("\n", "\n    "))
                    output.write(',\n      "moduleName": {"byteLength": "' + str(call["module-name-bytes"]) + '", "chunks": [')
                    offset, total = 0, call["module-name-bytes"]
                    while True:
                        if not call["module-name-truncated"]:
                            data, following = call["module-name"], total
                        else:
                            page = query(f"trace host bytes call={after} segment={U64} offset={offset} count=4096")
                            if len(page) != 1:
                                raise ValueError("incomplete module label page")
                            label = fields(page[0], "host-label", {"call", "size", "offset", "next", "value"})
                            data = quoted(label["value"])
                            following = number(label["next"])
                            if (number(label["call"]) != after or number(label["size"]) != total or
                                    number(label["offset"]) != offset or following != offset + len(data) or
                                    len(data) != min(4096, total - offset)):
                                raise ValueError("module label page identity changed")
                            if offset == 0 and data[:255] != call["module-name"]:
                                raise ValueError("module label prefix changed")
                        output.write("\n        " if offset == 0 else ",\n        ")
                        write({"offset": str(offset), **encoded(data)}, 8)
                        offset = following
                        if offset == total:
                            break
                    output.write('\n      ]},\n      "segments": [')
                    for segment in range(call["segments"]):
                        offset, segment_identity = 0, None
                        output.write("\n        " if segment == 0 else ",\n        ")
                        while True:
                            page = query(f"trace host bytes call={after} segment={segment} offset={offset} count=4096")
                            if len(page) != 1:
                                raise ValueError("incomplete host byte page")
                            b = fields(page[0], "host-bytes", {"call", "segment", "memory", "guest-offset",
                                       "direction", "observation", "size", "offset", "next", "hex"})
                            for k in b.keys() - {"direction", "observation", "hex"}:
                                b[k] = number(b[k])
                            if re.fullmatch(r"(?:[0-9a-f]{2}){0,4096}", b["hex"]) is None:
                                raise ValueError("invalid host buffer bytes")
                            data = bytes.fromhex(b["hex"])
                            if (b["call"] != after or b["segment"] != segment or b["offset"] != offset or
                                    offset > b["size"] or len(data) != min(4096, b["size"]-offset) or
                                    b["next"] != offset+len(data) or b["memory"] >= 1 << 32 or
                                    b["direction"] not in ("input", "output") or
                                    b["observation"] not in ("snapshot", "transfer")):
                                raise ValueError("host buffer page differs from request")
                            current = {k: b[k] for k in ("memory", "guest-offset", "direction", "observation", "size")}
                            if segment_identity is None:
                                segment_identity = current
                                text = json.dumps({"index": str(segment), "memory": str(b["memory"]),
                                        "guestOffset": str(b["guest-offset"]), "direction": b["direction"],
                                        "observation": b["observation"], "byteLength": str(b["size"])}, indent=2)
                                output.write(text[:-2].replace("\n", "\n        "))
                                output.write(',\n          "chunks": [')
                            elif current != segment_identity:
                                raise ValueError("host buffer identity changed")
                            output.write("\n            " if offset == 0 else ",\n            ")
                            write({"offset": str(offset), **encoded(data)}, 12)
                            summary["payloadBytes"] += len(data)
                            offset = b["next"]
                            if offset == b["size"]:
                                break
                        output.write("\n          ]\n        }")
                    output.write("\n      ]\n    }")
                    summary["calls"] += 1
                    summary["incompleteCalls"] += call["incomplete"]
                    summary["unreturnedCalls"] += not call["returned"]
                if next_id != after or (more and len(rows) != 9):
                    raise ValueError("non-progressing or inconsistent host trace cursor")
                if not more:
                    break
                rows = query(f"trace host read family={family} after={after} count=8")
            final = query(f"trace host read family={family} after={journal['newest']} count=1")
            if final != [f"host-calls next={journal['newest']} more=0"]:
                raise ValueError("incomplete final host trace cursor")
            if checkpoint() != identity:
                raise ValueError("guest stop changed during export; file not published")
            output.write('\n  ],\n  "summary": ')
            summary["observationLossReported"] = any(journal[k] for k in
                ("allocation-failures", "invalid-ranges", "sequence-failures"))
            write({k: str(v) if type(v) is int else v for k, v in summary.items()}, 2)
            output.write("\n}\n")
            output.flush()
            os.fsync(output.fileno())
        # Atomic exclusive publication: no partial JSON and no symlink following.
        os.link(temporary, destination)
    finally:
        os.unlink(temporary)
    return {"path": str(destination), **summary, "stopId": str(stop)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--socket-dir", required=True, help="Existing private full or read-only debugger broker directory")
    parser.add_argument("--output", required=True, help="New host JSON file; existing files are refused")
    parser.add_argument("--family", choices=("all", *FAMILIES.values()), default="all")
    args = parser.parse_args()
    # Imported here to keep the exporter independent for adapter/unit-test use.
    from dap_adapter import Adapter, UnixBroker
    adapter = Adapter(None)
    try:
        adapter.broker = UnixBroker(args.socket_dir)
        result = save_host_trace(adapter.command, adapter.cooperative_memory_stop, args.output, args.family)
        print(json.dumps(result, ensure_ascii=True))
        return 0
    except (OSError, ValueError, RuntimeError) as error:
        parser.exit(1, f"export refused: {error}\n")
    finally:
        if adapter.broker is not None:
            adapter.broker.close()


if __name__ == "__main__":
    raise SystemExit(main())
