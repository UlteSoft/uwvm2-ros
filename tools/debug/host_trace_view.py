"""Paged IDE views of retained observations; no collection or resource grants."""
import re
from export_host_trace import FAMILIES, U64, packet, fields, number, quoted, encoded, call_row, readable


def argument(value, default=None):
    if value is None:
        value = default
    if type(value) is int and 0 <= value <= U64:
        return value
    if isinstance(value, str):
        return number(value)
    raise ValueError("canonical unsigned integer or decimal string required")


def query(request, checkpoint, arguments, *, byte_page=False):
    allowed = {"call", "segment", "offset", "count"} if byte_page else {"family", "after", "count"}
    if not isinstance(arguments, dict) or arguments.keys() - allowed:
        raise ValueError("unknown host observation query field")
    identity = checkpoint()
    stop = identity[-1]
    if byte_page:
        call = argument(arguments.get("call"))
        segment = argument(arguments.get("segment"), 0)
        offset = argument(arguments.get("offset"), 0)
        count = argument(arguments.get("count"), 4096)
        if not call or not 1 <= count <= 4096:
            raise ValueError("host byte page requires a call and count 1..4096")
        command = f"trace host bytes call={call} segment={segment} offset={offset} count={count}"
    else:
        family = arguments.get("family", "all")
        after = argument(arguments.get("after"), 0)
        count = argument(arguments.get("count"), 8)
        if family not in ("all", *FAMILIES.values()) or not 1 <= count <= 8:
            raise ValueError("host call page requires a family and count 1..8")
        command = f"trace host read family={family} after={after} count={count}"
    journal, rows = packet(request(command + f" if-stop={stop}"))
    result = {"stopId": str(stop), "journal": {k: str(v) for k, v in journal.items()}}
    if byte_page:
        if len(rows) != 1:
            raise ValueError("exactly one host byte row required")
        if segment == U64:
            row = fields(rows[0], "host-label", {"call", "size", "offset", "next", "value"})
            data = quoted(row.pop("value"))
        else:
            row = fields(rows[0], "host-bytes", {"call", "segment", "memory", "guest-offset",
                         "direction", "observation", "size", "offset", "next", "hex"})
            if (re.fullmatch(r"(?:[0-9a-f]{2}){0,4096}", row["hex"]) is None or
                    row["direction"] not in ("input", "output") or
                    row["observation"] not in ("snapshot", "transfer") or
                    number(row["segment"]) != segment or number(row["memory"]) >= 1 << 32):
                raise ValueError("invalid host byte metadata")
            number(row["guest-offset"])
            data = bytes.fromhex(row.pop("hex"))
        size, following = number(row["size"]), number(row["next"])
        if (number(row["call"]) != call or number(row["offset"]) != offset or offset > size or
                len(data) != min(count, size-offset) or following != offset+len(data)):
            raise ValueError("host byte page differs from requested identity/range")
        result.update(row, **encoded(data), more=following < size)
    else:
        if not 1 <= len(rows) <= count+1:
            raise ValueError("invalid host call page size")
        cursor = fields(rows[-1], "host-calls", {"next", "more"})
        following, more = number(cursor["next"]), number(cursor["more"], 1)
        calls, previous = [], after
        for line in rows[:-1]:
            row = call_row(line)
            if (not max(previous, journal["cleared-through"]) < row["id"] <= journal["newest"] or
                    family != "all" and FAMILIES[row["family"]] != family):
                raise ValueError("host call order/family differs from request")
            previous = row["id"]
            public = {k: str(v) if type(v) is int else v for k, v in row.items()
                      if k not in ("namespace", "function", "module-name", "family")}
            public.update(family=FAMILIES[row["family"]],
                          namespace=readable(row["namespace"]), function=readable(row["function"]),
                          namespaceBytes=encoded(row["namespace"]), functionBytes=encoded(row["function"]),
                          moduleName=encoded(row["module-name"]))
            calls.append(public)
        if following != previous or (more and len(calls) != count):
            raise ValueError("incomplete/non-progressing host call page")
        result.update(calls=calls, next=str(following), more=bool(more))
    if checkpoint() != identity:
        raise ValueError("host observation stop changed during copy")
    return result
