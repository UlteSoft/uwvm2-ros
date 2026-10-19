#!/usr/bin/env python3
"""Generate bounded, statically registered custom ROS topic record codecs.

This is a source generator, not a native ROS generator or a type authority.
ROS native identities must come from official generation and are checked again
by the native Gateway. See sdk/custom-types/README.md for the input contract,
unsupported constructs, immutable codec identity and qualification procedure.
No generated codec performs I/O or acquires a capability.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import os
import re
import stat
import struct
import sys
import types
from dataclasses import dataclass
from pathlib import Path

PROFILE = "uwvm-ros-custom-fixed-v1"
MAX_SOURCE_BYTES = 1024 * 1024
MAX_CATALOG_BYTES = 2 * 1024 * 1024
MAX_TYPES, MAX_CELLS, MAX_WIRE, MAX_DEPTH = 128, 4096, 32768, 16
SCALARS = {
    "bool": ("boolean", 1, "uint8_t", "bool", "bool", "boolean", "bool"),
    "uint8": ("u8", 1, "uint8_t", "u8", "uint8", "number", "u8"),
    "int8": ("i8", 1, "int8_t", "i8", "int8", "number", "i8"),
    "uint16": ("u16", 2, "uint16_t", "u16", "uint16", "number", "u16"),
    "int16": ("i16", 2, "int16_t", "i16", "int16", "number", "i16"),
    "uint32": ("u32", 4, "uint32_t", "u32", "uint32", "number", "u32"),
    "int32": ("i32", 4, "int32_t", "i32", "int32", "number", "i32"),
    "uint64": ("u64", 8, "uint64_t", "u64", "uint64", "bigint", "u64"),
    "int64": ("i64", 8, "int64_t", "i64", "int64", "bigint", "i64"),
    "float32": ("f32", 4, "float", "f32", "float32", "number", "f32"),
    "float64": ("f64", 8, "double", "f64", "float64", "number", "f64"),
}
IDL_SCALARS = {"boolean": "bool", "uint8": "uint8", "int8": "int8",
    "short": "int16", "unsigned short": "uint16", "long": "int32", "unsigned long": "uint32",
    "long long": "int64", "unsigned long long": "uint64", "int16": "int16", "uint16": "uint16",
    "int32": "int32", "uint32": "uint32", "int64": "int64", "uint64": "uint64",
    "float": "float32", "double": "float64"}
NAME = re.compile(r"[A-Za-z][A-Za-z0-9_]*\Z")
PACKAGE = re.compile(r"[a-z][a-z0-9_]*\Z")
FIELD = re.compile(r"[a-z][a-z0-9]*(?:_[a-z0-9]+)*\Z")
RESERVED = set("class struct enum union template typename private public protected namespace const static inline volatile return int float double bool true false sizeof alignof default switch case for while do if else break continue new delete this type match ref self Self fn let mut pub crate mod use impl trait async await yield interface export import package func var map range defer go select chan string byte error in as from null undefined constructor prototype __proto__ valid encode decode".split())

class GenerationError(ValueError):
    """A precise refusal; unsupported syntax is never silently ignored."""

def _source_identity(info):
    return (info.st_dev, info.st_ino, info.st_size, info.st_mode,
            info.st_nlink, info.st_uid, info.st_gid, info.st_mtime_ns, info.st_ctime_ns)

def _source_unchanged(fd, path, identity):
    if _source_identity(os.fstat(fd)) != identity or _source_identity(os.lstat(path)) != identity:
        raise ValueError(f"generator source identity changed: {path}")

def _open_source_bytes(path, maximum):
    """Keep this SAME ordinary no-follow descriptor through source execution."""
    if any(not hasattr(os, flag) for flag in ('O_RDONLY', 'O_NONBLOCK', 'O_NOFOLLOW', 'O_CLOEXEC')):
        raise ValueError("actual nonblocking/no-follow/close-on-exec source admission is unavailable")
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= maximum:
            raise ValueError(f"generator source must be a bounded regular file: {path}")
        identity = _source_identity(first)
        raw = bytearray()
        while len(raw) <= first.st_size:
            part = os.read(fd, min(65536, first.st_size + 1 - len(raw)))
            if not part:
                break
            raw.extend(part)
            if len(raw) > first.st_size:
                raise ValueError(f"generator source grew during capture: {path}")
        if len(raw) != first.st_size:
            raise ValueError(f"generator source extent changed: {path}")
        _source_unchanged(fd, path, identity)
        return fd, bytes(raw), identity
    except BaseException:
        os.close(fd)
        raise

def _captured_module(name, path, expected_sha256=None, expected_identity=None):
    """Compile captured source directly; never consult or produce its pyc."""
    path = Path(os.path.abspath(os.fspath(path)))
    if expected_sha256 is not None and (not isinstance(expected_sha256, str) or not re.fullmatch(r"[0-9a-f]{64}", expected_sha256)):
        raise ValueError("an explicit lowercase full source SHA256 is required")
    fd, raw, identity = _open_source_bytes(path, MAX_SOURCE_BYTES)
    try:
        digest = hashlib.sha256(raw).hexdigest()
        if (expected_sha256 is not None and digest != expected_sha256) or (expected_identity is not None and identity != expected_identity):
            raise ValueError(f"generator source differs from the admitted source: {path}")
        code = compile(raw, str(path), 'exec', dont_inherit=True)
        module = types.ModuleType(name)
        module.__file__ = str(path)
        module.__package__ = ''
        module.__loader__ = None
        module.__spec__ = None
        module.__cached__ = None
        missing = object()
        previous = sys.modules.get(name, missing)
        # Real dataclass/future annotation resolution requires this entry BEFORE
        # exec. On every exception restore the exact previous module binding.
        sys.modules[name] = module
        try:
            exec(code, module.__dict__)
            _source_unchanged(fd, path, identity)
            if sys.modules.get(name) is not module:
                raise ValueError("captured source replaced its admitted module binding")
            # Commit only the actual captured facts, never code-supplied claims.
            module.__file__ = str(path)
            module.__loader__ = None
            module.__spec__ = None
            module.__cached__ = None
            module._uwvm_source_sha256 = digest
            module._uwvm_source_identity = identity
        except BaseException:
            if previous is missing:
                sys.modules.pop(name, None)
            else:
                sys.modules[name] = previous
            raise
        return module
    finally:
        os.close(fd)

# Keys are the actual fixed/explicit source names, not catalog-controlled paths.
_CAPTURED_GENERATOR_SOURCES = {}

def _remember_source(module):
    path = os.path.abspath(os.fspath(module.__file__))
    snapshot = (module._uwvm_source_identity, module._uwvm_source_sha256)
    previous = _CAPTURED_GENERATOR_SOURCES.get(path)
    if previous is not None and previous != snapshot:
        raise GenerationError(f"generator source changed since admission: {path}")
    _CAPTURED_GENERATOR_SOURCES[path] = snapshot

def _read_captured_source(path, maximum, snapshot):
    try:
        fd, raw, identity = _open_source_bytes(path, maximum)
        try:
            if identity != snapshot[0] or hashlib.sha256(raw).hexdigest() != snapshot[1]:
                raise GenerationError(f"generator source changed since actual source compilation: {path}")
            return raw
        finally:
            os.close(fd)
    except ValueError as error:
        raise GenerationError(str(error)) from error

def bounded_profile(path: Path):
    """Select only the installed trusted bounded-v2 source generator.

    The catalog cannot name a Python module/provider. Generated outputs and
    native identities still undergo separate source/native qualification; this
    load provides no runtime registration, permission or native library load.
    The fixed-v1 code path and historical generated revisions remain intact.
    """
    raw = bounded_read(path, MAX_CATALOG_BYTES)
    catalog = strict_json(raw)
    if not isinstance(catalog,dict) or catalog.get('format') not in ('uwvm-ros-custom-bounded-v2','uwvm-ros-official-standard-bounded-v1','uwvm-ros-original-node-control-c-cpp-v1'):
        return None
    source=Path(__file__).resolve().parents[1]/'custom-types/bounded_profile.py'
    # Only this installed sibling can execute. The catalog selects a closed
    # format, never a module pathname, import hook or provider implementation.
    admitted = _CAPTURED_GENERATOR_SOURCES.get(str(source))
    try:
        module = _captured_module('_uwvm_ros_custom_bounded_profile_v2', source,
            admitted[1] if admitted else None, admitted[0] if admitted else None)
        _remember_source(module)
    except ValueError as error:
        raise GenerationError(str(error)) from error
    return module

def bounded_read(path: Path, maximum: int) -> bytes:
    """Read one pinned regular file through a nonblocking descriptor."""
    # Later provenance reads of a captured generator MUST describe the source
    # actually compiled, not a replacement with the same filename/pyc metadata.
    admitted = _CAPTURED_GENERATOR_SOURCES.get(os.path.abspath(os.fspath(path)))
    if admitted is not None:
        return _read_captured_source(path, maximum, admitted)
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or before.st_size > maximum:
            raise GenerationError(f"input must be a regular file of at most {maximum} bytes: {path}")
        raw = bytearray()
        while len(raw) <= before.st_size:
            block = os.read(fd, min(65536, before.st_size + 1 - len(raw)))
            if not block:
                break
            raw.extend(block)
        after = os.fstat(fd)
        key = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns, s.st_ctime_ns)
        if key(before) != key(after) or len(raw) != before.st_size:
            raise GenerationError(f"input changed while reading: {path}")
        return bytes(raw)
    finally:
        os.close(fd)

def strict_json(raw: bytes):
    """Reject duplicate object keys rather than letting later values win."""
    def pairs(values):
        out = {}
        for k, v in values:
            if k in out:
                raise GenerationError(f"duplicate JSON key: {k}")
            out[k] = v
        return out
    try:
        return json.loads(raw, object_pairs_hook=pairs)
    except (UnicodeError, json.JSONDecodeError) as e:
        raise GenerationError(f"invalid UTF-8 JSON: {e}") from e

def canonical_json(value) -> bytes:
    return json.dumps(value, sort_keys=True, separators=(",", ":"), ensure_ascii=True, allow_nan=False).encode()

def native_name(version: int, name: str) -> str:
    parts = name.split("/")
    if version not in (1, 2) or len(parts) != (2 if version == 1 else 3):
        raise GenerationError(f"exact ROS{version} message type required: {name}")
    if len(name)>255 or len(parts[0])>63 or not PACKAGE.fullmatch(parts[0]) or not re.fullmatch(r"[A-Z][A-Za-z0-9]*", parts[-1]):
        raise GenerationError(f"invalid package or message name: {name}")
    if version == 2 and parts[1] != "msg":
        raise GenerationError("native Service/Action registry emission is unsupported in this fixed-topic profile")
    if parts[0] in {"builtin", "std_msgs", "geometry_msgs", "sensor_msgs", "std_srvs", "lifecycle_msgs", "rcl_interfaces", "actionlib_tutorials", "action_tutorials_interfaces"}:
        raise GenerationError("existing builtin adapter packages cannot be shadowed by a custom catalog")
    return name

def field_name(name: str, ros_version: int = 2, bounded: bool = False) -> str:
    """Preserve official ROS1 spelling without relaxing ROS2 member syntax.

    Legacy ROS1 definitions use names such as ``motorCmd``, ``Kp`` and ``SN``.
    Changing their case changes the native definition/MD5, so bounded-v2 keeps
    those names verbatim.  The fixed-v1 parser retains its existing lowercase
    grammar and calls this function with the unchanged strict default.  Every
    bounded record also passes the six-emitter namespace checks below.
    """
    pattern = NAME if ros_version == 1 else FIELD
    # ``error`` is a predeclared Go type, not a keyword; the bounded emitter
    # exports it as Error and has no conflicting member.  Unitree HG declares
    # exactly this native field. ``fn`` is legal in C/C++/Go/JS/AS and the
    # bounded Rust emitter consistently spells it r#fn. Neither exception
    # renames canonical/native fields. The fixed-v1 rule stays intact.
    reserved = RESERVED - {'error', 'fn'} if bounded else RESERVED
    if ros_version not in (1, 2) or not pattern.fullmatch(name) or name in reserved:
        raise GenerationError(f"field name is invalid or reserved in a generated language: {name}")
    return name


def bounded_member_names(names) -> None:
    """Refuse generated-language collisions; never rename an official field.

    Go exports the first character, whereas the other emitters retain exact
    spelling.  Check the actual emitted method names as well as exported-field
    collisions before producing any language.  Existing snake_case records
    keep precisely the same IR and canonical identity.
    """
    go_fields = set()
    for name in names:
        exported = name[:1].upper() + name[1:]
        if exported in go_fields or exported in {'WireSize', 'Encode'}:
            raise GenerationError('field collides in generated Go namespace: ' + name)
        go_fields.add(exported)
        if name in {'validValue', 'writeBounded', 'readBounded'}:
            raise GenerationError('field collides with generated codec member: ' + name)
        # C/C++, Rust, Go and AS/JS keyword unions not covered by the original
        # fixed-profile list.  Case is significant: official Kp/Kd/SN remain
        # intact.  No keyword escape/renaming is used in canonical/native IR.
        extra = {'auto', 'extern', 'register', 'restrict', 'signed', 'unsigned',
                 'short', 'long', 'void', 'char', 'typedef', 'goto', 'union',
                 'alignas', 'alignof', 'asm', 'catch', 'constexpr', 'consteval',
                 'constinit', 'decltype', 'explicit', 'friend', 'mutable',
                 'noexcept', 'operator', 'requires', 'co_await', 'co_return',
                 'co_yield', 'try', 'throw', 'using', 'virtual', 'wchar_t',
                 'char8_t', 'char16_t', 'char32_t', 'and', 'or', 'not', 'xor',
                 'and_eq', 'or_eq', 'not_eq', 'xor_eq', 'bitand', 'bitor',
                 'compl', 'dyn', 'loop', 'move', 'super', 'unsafe', 'where',
                 'abstract', 'as', 'assert', 'become', 'box', 'final', 'macro',
                 'override', 'priv', 'typeof', 'unsized', 'virtual', 'typeof',
                 'with', 'debugger', 'extends', 'implements', 'instanceof',
                 'instanceOf', 'delete', 'get', 'set', 'of', 'keyof', 'is',
                 'readonly', 'declare', 'namespace', 'require', 'global',
                 'switch', 'fallthrough', 'const', 'null', 'false', 'true'}
        if name in extra:
            raise GenerationError('field is reserved in a generated language: ' + name)

@dataclass(frozen=True)
class Member:
    name: str
    type: str
    count: int | None = None

@dataclass
class Record:
    version: int
    name: str
    members: list[Member]
    identity: str | None
    source: dict
    cells: list = None
    wire_bytes: int = 0
    codec_identity: str = ""
    depth: int = 1

    @property
    def symbol(self):
        return f"Ros{self.version}" + "".join(p[:1].upper() + p[1:] for p in self.name.split("/") if p != "msg")

    @property
    def c_symbol(self):
        return "roswasmc_custom_" + str(self.version) + "_" + self.name.replace("/", "_")

def reference(version: int, package: str, spelling: str) -> str:
    if spelling in SCALARS:
        return spelling
    bits = spelling.replace("::", "/").strip("/").split("/")
    if len(bits) == 1:
        bits = [package, bits[0]] if version == 1 else [package, "msg", bits[0]]
    elif version == 2 and len(bits) == 2:
        bits.insert(1, "msg")
    return native_name(version, "/".join(bits))

def parse_msg(text: str, version: int, name: str) -> list[Member]:
    """Parse exactly fixed record fields; reject every unsupported declaration."""
    out = []
    for number, raw in enumerate(text.splitlines(), 1):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        if line == "---":
            raise GenerationError(".srv/.action components are not native topics; use separate .msg records")
        match = re.fullmatch(r"([A-Za-z][A-Za-z0-9_/]*)(?:\[([0-9]+)\])?\s+([a-z][a-z0-9_]*)", line)
        if not match:
            raise GenerationError(f"line {number}: only fixed fields are supported; strings, sequences, constants and defaults are unsupported")
        base, count, field = match.groups()
        if base in {"string", "wstring", "time", "duration", "Header", "byte", "char"}:
            raise GenerationError(f"line {number}: unsupported or ambiguous scalar {base}; use explicit fixed records/scalars")
        count = int(count) if count is not None else None
        if count is not None and not 1 <= count <= MAX_CELLS:
            raise GenerationError(f"line {number}: fixed array count must be 1..{MAX_CELLS}")
        out.append(Member(field_name(field), reference(version, name.split("/")[0], base), count))
    if len(out) > MAX_CELLS or len({m.name for m in out}) != len(out):
        raise GenerationError("too many fields or duplicate field names")
    return out

def parse_idl(text: str) -> dict[str, list[Member]]:
    """Parse a closed ROS2 IDL subset; no preprocessor or annotations execute."""
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    text = re.sub(r"//[^\n]*", " ", text)
    tokens = re.findall(r"::|[A-Za-z_][A-Za-z0-9_]*|[0-9]+|[{};\[\]]|\S", text)
    if len(tokens) > 32768:
        raise GenerationError("IDL token limit exceeded")
    at, records = 0, {}
    def take(expected=None):
        nonlocal at
        if at == len(tokens):
            raise GenerationError("truncated IDL declaration")
        v = tokens[at]
        at += 1
        if expected is not None and v != expected:
            raise GenerationError(f"IDL expected {expected!r}, got {v!r}; unsupported declaration")
        return v
    def body(scope):
        nonlocal at
        if len(scope) > 2:
            raise GenerationError("IDL allows only package::msg modules")
        while at < len(tokens) and tokens[at] != "}":
            kind = take()
            if kind == "module":
                module = take()
                if not NAME.fullmatch(module):
                    raise GenerationError("invalid IDL module")
                take("{"); body(scope + [module]); take("}"); take(";")
            elif kind == "struct":
                short = take()
                full = native_name(2, "/".join(scope + [short]))
                if full in records:
                    raise GenerationError("duplicate IDL record")
                take("{"); fields = []
                while at < len(tokens) and tokens[at] != "}":
                    spelling = take()
                    if spelling == "unsigned":
                        spelling += " " + take()
                    if spelling.endswith("long") and at < len(tokens) and tokens[at] == "long":
                        spelling += " " + take()
                    while at < len(tokens) and tokens[at] == "::":
                        take(); spelling += "::" + take()
                    scalar = IDL_SCALARS.get(spelling)
                    if scalar is None and not re.fullmatch(r"[A-Za-z][A-Za-z0-9_]*(?:::[A-Za-z][A-Za-z0-9_]*)*", spelling):
                        raise GenerationError(f"unsupported IDL type: {spelling}")
                    if spelling in {"string", "wstring", "sequence", "char", "wchar", "octet", "long double"}:
                        raise GenerationError(f"unsupported IDL type: {spelling}")
                    field = field_name(take()); count = None
                    if at < len(tokens) and tokens[at] == "[":
                        take(); value = take()
                        if not value.isdecimal() or not 1 <= int(value) <= MAX_CELLS:
                            raise GenerationError("invalid fixed IDL array count")
                        count = int(value); take("]")
                    take(";")
                    fields.append(Member(field, scalar or reference(2, scope[0], spelling), count))
                take("}"); take(";")
                if len(fields) > MAX_CELLS or len({m.name for m in fields}) != len(fields):
                    raise GenerationError("too many or duplicate IDL fields")
                records[full] = fields
            else:
                raise GenerationError(f"unsupported IDL construct: {kind}; only modules and fixed structs are supported")
    body([])
    if at != len(tokens):
        raise GenerationError("trailing or unmatched IDL tokens")
    return records

def load_catalog(path: Path, require_identity=True):
    """Resolve explicit dependencies and compute bounded, canonical wire schemas."""
    raw = bounded_read(path, MAX_CATALOG_BYTES)
    catalog = strict_json(raw)
    if not isinstance(catalog, dict) or set(catalog) != {"format", "types"} or catalog["format"] != PROFILE:
        raise GenerationError(f"catalog requires exactly format={PROFILE!r} and types")
    specs = catalog["types"]
    if not isinstance(specs, list) or not 1 <= len(specs) <= MAX_TYPES:
        raise GenerationError(f"catalog must contain 1..{MAX_TYPES} types")
    records, inputs, cache = {}, [], {}
    root = path.resolve().parent
    for spec in specs:
        if not isinstance(spec, dict) or set(spec) - {"ros_version", "type", "file", "native_identity"} or not {"ros_version", "type", "file"} <= set(spec):
            raise GenerationError("type entries require ros_version, type, file and optional native_identity only")
        version, name, filename = spec["ros_version"], spec["type"], spec["file"]
        if type(version) is not int or not isinstance(name, str) or not isinstance(filename, str):
            raise GenerationError("version/name/file have incorrect JSON types")
        name = native_name(version, name)
        key = (version, name)
        if key in records:
            raise GenerationError("duplicate catalog native type")
        file = (root / filename).resolve()
        if not file.is_relative_to(root):
            raise GenerationError("definition path escapes the catalog directory")
        if file.suffix not in {".msg", ".idl"}:
            raise GenerationError("only .msg and ROS2 .idl topic records can be registered; .srv/.action native adapters are unsupported")
        if file not in cache:
            content = bounded_read(file, MAX_SOURCE_BYTES)
            try:
                text = content.decode("utf-8")
            except UnicodeError as e:
                raise GenerationError("definition must be UTF-8") from e
            cache[file] = (text, {"file": filename, "bytes": len(content), "sha256": hashlib.sha256(content).hexdigest()})
        text, pin = cache[file]
        identity = spec.get("native_identity")
        pattern = r"[0-9a-f]{32}" if version == 1 else r"RIHS01_[0-9a-f]{64}"
        if identity is not None and (not isinstance(identity, str) or not re.fullmatch(pattern, identity)):
            raise GenerationError("native_identity must be exact official lowercase ROS1 MD5 or ROS2 RIHS01")
        if require_identity and identity is None:
            raise GenerationError(f"official native_identity is required for registry emission: {name}; --check permits preparation only")
        if file.suffix == ".idl":
            if version != 2:
                raise GenerationError("IDL inputs are ROS2 only")
            definitions = parse_idl(text)
            if name not in definitions:
                raise GenerationError("catalog type is absent from its IDL file")
            members = definitions[name]
        else:
            members = parse_msg(text, version, name)
        records[key] = Record(version, name, members, identity, pin)
        inputs.append(pin)
    ordered, visiting = [], set()
    def resolve(record, depth=1):
        key = (record.version, record.name)
        if depth > MAX_DEPTH:
            raise GenerationError("nested record depth exceeds 16")
        if key in visiting:
            raise GenerationError("recursive record graph is unsupported")
        if record.cells is not None:
            if depth + record.depth - 1 > MAX_DEPTH:
                raise GenerationError("nested record depth exceeds 16")
            return
        visiting.add(key); cells = []
        for member in record.members:
            if member.type in SCALARS:
                child = [((), member.type)]
            else:
                dependency = records.get((record.version, member.type))
                if dependency is None:
                    raise GenerationError(f"named dependency must be supplied explicitly in catalog: {member.type}")
                resolve(dependency, depth + 1); child = dependency.cells
                record.depth = max(record.depth, dependency.depth + 1)
            count = member.count or 1
            if len(child) * count > MAX_CELLS - len(cells):
                raise GenerationError("expanded scalar cells exceed 4096")
            for index in range(count):
                prefix = (member.name,) + ((index,) if member.count is not None else ())
                cells.extend((prefix + suffix, scalar) for suffix, scalar in child)
        visiting.remove(key)
        record.cells = cells
        record.wire_bytes = sum(SCALARS[s][1] for _, s in cells)
        if not 1 <= record.wire_bytes <= MAX_WIRE:
            raise GenerationError("topic records require 1..32768 fixed wire bytes; empty records are unsupported")
        def schema(r):
            return {"members": [{"name": m.name, "type": m.type, "array": m.count,
                **({"record": schema(records[(r.version, m.type)])} if m.type not in SCALARS else {})} for m in r.members]}
        ir = {"profile": PROFILE, "ros_version": record.version, "type": record.name,
              "schema": schema(record), "booleans": "0-or-1", "floats": "finite-ieee754", "byte_order": "little-endian"}
        record.codec_identity = "UWVMROS1_" + hashlib.sha256(canonical_json(ir)).hexdigest()
        ordered.append(record)
    for record in records.values():
        resolve(record)
    if sum(len(r.cells) for r in ordered)>16384:
        raise GenerationError("catalog expanded scalar cells exceed the 16384 source-generation budget")
    return records, ordered, {"catalog": {"bytes": len(raw), "sha256": hashlib.sha256(raw).hexdigest()},
        "definitions": sorted({p["file"]: p for p in inputs}.values(), key=lambda p: p["file"])}

def access(path, prefix="", language="c"):
    out = prefix
    for part in path:
        out += f"[{part}]" if isinstance(part, int) else ("." if out else "") + (part[:1].upper() + part[1:] if language == "go" else part)
    return out

def c_type(member, record, records):
    return SCALARS[member.type][2] if member.type in SCALARS else records[(record.version, member.type)].c_symbol

def emit_c(records, ordered):
    lines = ["/** @file Generated fixed ROS topic records; no I/O, native serialization or authority.",
        " * Encode validates the complete value before modifying output. Decode preserves", " * the destination on refusal. Caller buffers and records must not overlap. */",
        "#ifndef UWVM_ROS_CUSTOM_FIXED_RECORDS_H", "#define UWVM_ROS_CUSTOM_FIXED_RECORDS_H", "#include <stdint.h>", "#include <stddef.h>", "#include <float.h>", "#include <limits.h>",
        "#define UWVM_CUSTOM_INVALID 22u",
        "#ifdef __cplusplus", "#define UWVM_CUSTOM_ASSERT(x) static_assert(x,\"canonical scalar representation required\")", "#else", "#define UWVM_CUSTOM_ASSERT(x) _Static_assert(x,\"canonical scalar representation required\")", "#endif",
        "UWVM_CUSTOM_ASSERT(CHAR_BIT==8 && sizeof(float)==4 && sizeof(double)==8 && FLT_RADIX==2 && FLT_MANT_DIG==24 && DBL_MANT_DIG==53 && FLT_MAX_EXP==128 && DBL_MAX_EXP==1024);",
        "/** @brief Copy fixed scalar object bits; compilers optimize this intrinsic, with a portable freestanding fallback. */",
        "static inline void uwvm_custom_copy(void* out,void const* in,size_t n) {",
        "#if defined(__GNUC__) || defined(__clang__)", "__builtin_memcpy(out,in,n);", "#else", "size_t i;for(i=0;i<n;++i)((unsigned char*)out)[i]=((unsigned char const*)in)[i];", "#endif", "}",
        "/** @brief Read one already range-checked LE scalar; no allocation occurs. */",
        "static inline uint64_t uwvm_custom_get(unsigned char const* p,unsigned n) {uint64_t v=0;unsigned i;for(i=0;i<n;++i)v|=(uint64_t)p[i]<<(8*i);return v;}",
        "/** @brief Store the low n bytes of one scalar into prevalidated storage. */",
        "static inline void uwvm_custom_put(unsigned char* p,uint64_t v,unsigned n) {unsigned i;for(i=0;i<n;++i)p[i]=(unsigned char)(v>>(8*i));}"]
    for record in ordered:
        cs = record.c_symbol
        lines += [f"/** @brief Fixed value for ROS{record.version} {record.name}; arrays have exact lengths. */", f"typedef struct {cs} {{"]
        for m in record.members:
            array = f"[{m.count}]" if m.count is not None else ""
            lines += [f"    /** @brief Native field {m.name}; {'exactly '+str(m.count)+' elements' if array else 'one fixed value'}. */", f"    {c_type(m,record,records)} {m.name}{array};"]
        lines += [f"}} {cs};", f"#define {cs.upper()}_WIRE_BYTES {record.wire_bytes}u", f"#define {cs.upper()}_CODEC_IDENTITY {json.dumps(record.codec_identity)}",
            f"/** @brief Validate all booleans and finite floats without writing or allocating. */", f"static inline int {cs}_valid({cs} const* value) {{", "    if(!value) return 0;"]
        for p, s in record.cells:
            a = access(p,"value->").replace("->.","->")
            if s == "bool": lines.append(f"    if({a}>1) return 0;")
            if s.startswith("float"):
                width=SCALARS[s][1]; mask="UINT32_C(0x7f800000)" if width==4 else "UINT64_C(0x7ff0000000000000)"
                lines.append(f"    {{uint{width*8}_t bits=0;uwvm_custom_copy(&bits,&{a},{width});if((bits&{mask})=={mask})return 0;}}")
        lines += ["    return 1;", "}", "/** @brief Encode one exact record; length and output are preserved on validation failure.", " * Supply separate output storage with capacity >= WIRE_BYTES. */",
            f"static inline uint32_t {cs}_encode({cs} const* value,void* output,size_t capacity,size_t* length) {{",
            f"    unsigned char* p=(unsigned char*)output;if(!length||!output||capacity<{record.wire_bytes}u||!{cs}_valid(value))return UWVM_ROS_INVALID;"]
        offset=0
        for p,s in record.cells:
            a=access(p,"value->").replace("->.","->");width=SCALARS[s][1]
            if s.startswith("float"):
                lines.append(f"    {{uint{width*8}_t bits=0;uwvm_custom_copy(&bits,&{a},{width});uwvm_custom_put(p+{offset},bits,{width});}}")
            else: lines.append(f"    uwvm_custom_put(p+{offset},(uint64_t){a},{width});")
            offset+=width
        lines += [f"    *length={record.wire_bytes}u;return 0;", "}", "/** @brief Decode exact canonical bytes transactionally; trailing bytes are refused. */",
            f"static inline uint32_t {cs}_decode(void const* input,size_t length,{cs}* value) {{",
            f"    unsigned char const* p=(unsigned char const*)input;{cs} next;if(!value||!input||length!={record.wire_bytes}u)return UWVM_ROS_INVALID;"]
        offset=0
        for p,s in record.cells:
            a=access(p,"next");width=SCALARS[s][1]
            lines.append(f"    {{uint{width*8}_t bits=(uint{width*8}_t)uwvm_custom_get(p+{offset},{width});uwvm_custom_copy(&{a},&bits,{width});}}")
            offset+=width
        lines += [f"    if(!{cs}_valid(&next))return UWVM_ROS_INVALID;*value=next;return 0;", "}"]
    return "\n".join(lines+["#undef UWVM_CUSTOM_ASSERT", "#endif", ""]).replace("roswasmc_copy_object", "uwvm_custom_copy").replace("UWVM_ROS_INVALID", "UWVM_CUSTOM_INVALID")

def emit_cpp(ordered):
    lines=["/** @file Generated C++20 guest codecs. Include with the SDK include directory.", " * The C record aliases retain fixed arrays. Publisher/Subscription use Codec", " * specializations; codec selection creates no permission. No I/O occurs. */", "#pragma once", '#include "fixed_records.h"', '#include <roswasm/roswasmcpp.hpp>', "namespace uwvm_custom {"]
    lines += [f"/** @brief ROS{r.version} {r.name} fixed guest value. */\nusing {r.symbol} = {r.c_symbol};" for r in ordered]
    lines += ["}", "namespace roswasmcpp {"]
    for r in ordered:
        lines += [f"/** @brief Exact checked codec for {r.name}; native identity is verified by the host. */", f"template<> struct Codec<uwvm_custom::{r.symbol}> {{",
            f"    static constexpr size_t wire_bytes={r.wire_bytes};", f"    static constexpr char const* codec_identity={json.dumps(r.codec_identity)};", f"    static bool encode(uwvm_custom::{r.symbol} const& value,wire::bytes& out) {{",
            f"        unsigned char bytes[{r.wire_bytes}];size_t n;if({r.c_symbol}_encode(&value,bytes,sizeof bytes,&n))return false;wire::bytes next(bytes,bytes+n);out=std::move(next);return true;}}",
            f"    static bool decode(std::span<unsigned char const> bytes,uwvm_custom::{r.symbol}& out) {{return !{r.c_symbol}_decode(bytes.data(),bytes.size(),&out);}}", "};"]
    return "\n".join(lines+["}", ""])

def rust_type(m,r,records):
    t=SCALARS[m.type][3] if m.type in SCALARS else records[(r.version,m.type)].symbol
    return f"[{t};{m.count}]" if m.count is not None else t

def emit_rust(records,ordered):
    lines=["//! Generated fixed topic values and checked codecs; no I/O or authority.", "//! Add this file as a module to a guest using roswasm with its alloc feature.", "extern crate alloc;", "use alloc::vec::Vec;", "use roswasm::{Error,codes};", "use roswasm::typed::Canonical;"]
    for r in ordered:
        lines += [f"/// Fixed ROS{r.version} {r.name} value. All arrays have exact lengths.", "#[derive(Clone,Debug,PartialEq)]", f"pub struct {r.symbol} {{"]
        lines += [f"    /// Native fixed field {m.name}.\n    pub {m.name}: {rust_type(m,r,records)}," for m in r.members]
        lines += ["}", f"impl Default for {r.symbol} {{fn default()->Self {{Self {{"]
        lines += [f"{m.name}:"+("core::array::from_fn(|_|Default::default())" if m.count is not None else "Default::default()")+"," for m in r.members]
        lines += ["}}}", f"impl {r.symbol} {{", f"    /// Exact canonical byte count; no native serialization is implied.\n    pub const WIRE_BYTES:usize={r.wire_bytes};", f"    /// Compile-time canonical schema identity; the host independently verifies it.\n    pub const CODEC_IDENTITY:&'static str={json.dumps(r.codec_identity)};", "    /// Validate every finite field without allocating output.", "    pub fn valid(&self)->Result<(),Error> {"]
        for p,s in r.cells:
            if s.startswith("float"):lines.append(f"        if !{access(p,'self')}.is_finite() {{return Err(Error::new(codes::INVALID));}}")
        lines += ["        Ok(())", "    }",
            "    /// Encode into separate caller-owned bytes after complete validation.", "    pub fn encode_into(&self,out:&mut [u8])->Result<usize,Error> {",
            f"        if out.len()<{r.wire_bytes} {{return Err(Error::new(codes::TOO_LARGE));}}self.valid()?;"]
        offset=0
        for p,s in r.cells:
            a=access(p,"self");width=SCALARS[s][1]
            if s=="bool":lines.append(f"        out[{offset}]=u8::from({a});")
            else:lines.append(f"        out[{offset}..{offset+width}].copy_from_slice(&{a}.to_le_bytes());")
            offset+=width
        lines += [f"        Ok({r.wire_bytes})", "    }", "}", f"impl Canonical for {r.symbol} {{", "    /// Allocate exactly one fixed output after validating all fields.", f"    fn encode(&self)->Result<Vec<u8>,Error> {{self.valid()?;let mut out=alloc::vec![0u8;{r.wire_bytes}];self.encode_into(&mut out)?;Ok(out)}}",
            "    /// Decode exactly one fixed record; truncation and trailing bytes fail.", f"    fn decode(bytes:&[u8])->Result<Self,Error> {{if bytes.len()!={r.wire_bytes} {{return Err(Error::new(codes::INVALID));}}let mut next=Self::default();"]
        offset=0
        for p,s in r.cells:
            a=access(p,"next");width=SCALARS[s][1]
            if s=="bool":lines.append(f"        if bytes[{offset}]>1 {{return Err(Error::new(codes::INVALID));}}{a}=bytes[{offset}]!=0;")
            else:
                lines.append(f"        {a}={SCALARS[s][3]}::from_le_bytes(bytes[{offset}..{offset+width}].try_into().map_err(|_|Error::new(codes::INVALID))?);")
                if s.startswith("float"):lines.append(f"        if !{a}.is_finite() {{return Err(Error::new(codes::INVALID));}}")
            offset+=width
        lines += ["        Ok(next)", "    }", "}"]
    return "\n".join(lines+[""])

def js_default(record,records):
    return "{"+",".join(json.dumps(m.name)+":"+ ("["+",".join([js_value_default(m.type,record,records)]*(m.count or 1))+"]" if m.count is not None else js_value_default(m.type,record,records)) for m in record.members)+"}"

def js_value_default(t,r,records):
    return ("false" if t=="bool" else "0n" if t in {"int64","uint64"} else "0") if t in SCALARS else js_default(records[(r.version,t)],records)

def emit_js(records,ordered):
    lines=["/** Generated fixed-record topic codecs. No native serialization, I/O or authority.", " * Encoders validate exact fields/array lengths and scalar ranges before allocation.", " * Decode returns a fresh value only after consuming exact canonical bytes. */",
        "function shape(v,keys){if(v===null||typeof v!=='object'||Array.isArray(v)||Reflect.ownKeys(v).length!==keys.length||keys.some(k=>!Object.hasOwn(v,k)))throw new TypeError('Exact record fields required');}",
        "function array(v,n){if(!Array.isArray(v)||v.length!==n)throw new TypeError('Exact fixed array required');}",
        r"function scalar(v,k){if(k==='bool'){if(typeof v!=='boolean')throw new TypeError('Boolean required');return;}if(k==='int64'||k==='uint64'){const lo=k==='int64'?-(1n<<63n):0n,hi=k==='int64'?(1n<<63n)-1n:(1n<<64n)-1n;if(typeof v!=='bigint'||v<lo||v>hi)throw new RangeError('64-bit BigInt required');return;}if(k.startsWith('float')){if(typeof v!=='number'||!Number.isFinite(v)||(k==='float32'&&!Number.isFinite(Math.fround(v))))throw new RangeError('Finite float required');return;}const signed=k.startsWith('int'),bits=Number(k.replace(/\D/g,'')),lo=signed?-(2**(bits-1)):0,hi=signed?2**(bits-1)-1:2**bits-1;if(!Number.isInteger(v)||v<lo||v>hi)throw new RangeError('Scalar integer range');}"]
    for r in ordered:
        lines += [f"/** Validate complete ROS{r.version} {r.name} before encoding. */", f"function validate{r.symbol}(value){{"]
        def shapes(rec,prefix):
            lines.append(f"shape({prefix},{json.dumps([m.name for m in rec.members])});")
            for m in rec.members:
                a=prefix+"."+m.name
                if m.count is not None:lines.append(f"array({a},{m.count});")
                if m.type not in SCALARS:
                    for i in range(m.count or 1): shapes(records[(rec.version,m.type)],a+(f"[{i}]" if m.count is not None else ""))
        shapes(r,"value")
        lines += [f"scalar({access(p,'value')},{json.dumps(s)});" for p,s in r.cells]
        lines += ["}", f"/** Codec accepted by roswasm Node.createPublisher/createSubscription; wire bytes={r.wire_bytes}. */", f"export const {r.symbol}Codec=Object.freeze({{wireBytes:{r.wire_bytes},codecIdentity:{json.dumps(r.codec_identity)},",
            f"encode(value){{validate{r.symbol}(value);const bytes=new Uint8Array({r.wire_bytes}),view=new DataView(bytes.buffer);"]
        offset=0
        for p,s in r.cells:
            a=access(p,"value");width=SCALARS[s][1]
            method="setUint8" if s=="bool" else "setFloat"+str(width*8) if s.startswith("float") else "set"+("Big" if width==8 else "")+("Int" if s.startswith("int") else "Uint")+str(width*8)
            lines.append(f"view.{method}({offset},{'Number('+a+')' if s=='bool' else a},true);");offset+=width
        lines += ["return bytes;},", f"decode(bytes){{if(!(bytes instanceof Uint8Array)||bytes.length!=={r.wire_bytes})throw new RangeError('Exact canonical length required');const view=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength),value={js_default(r,records)};"]
        offset=0
        for p,s in r.cells:
            a=access(p,"value");width=SCALARS[s][1]
            method="getUint8" if s=="bool" else "getFloat"+str(width*8) if s.startswith("float") else "get"+("Big" if width==8 else "")+("Int" if s.startswith("int") else "Uint")+str(width*8)
            if s=="bool":lines.append(f"if(view.getUint8({offset})>1)throw new RangeError('Noncanonical Bool');{a}=view.getUint8({offset})!==0;")
            else:lines.append(f"{a}=view.{method}({offset},true);")
            offset+=width
        lines += [f"validate{r.symbol}(value);return value;}}", "});"]
    return "\n".join(lines+[""])

def emit_go(records,ordered):
    lines=["// Code generated by custom_types.py; fixed topic codecs perform no I/O.", "// Copy into a separate package; values implement the SDK Message interface.", "package customtypes", 'import ("encoding/binary";"math";"errors")', 'var invalid=errors.New("invalid fixed canonical ROS record")']
    for r in ordered:
        lines += [f"// {r.symbol}CodecIdentity is the immutable canonical schema identity, not authority.",f"const {r.symbol}CodecIdentity={json.dumps(r.codec_identity)}",f"// {r.symbol} represents ROS{r.version} {r.name}, with exact fixed arrays.",f"type {r.symbol} struct {{"]
        for m in r.members:
            t=SCALARS[m.type][4] if m.type in SCALARS else records[(r.version,m.type)].symbol
            lines += [f"    // {m.name} is the native fixed field; no authority is carried here.",f"    {m.name[:1].upper()+m.name[1:]} {('['+str(m.count)+']') if m.count is not None else ''}{t}"]
        lines += ["}", f"// Encode validates fields before allocating exactly {r.wire_bytes} bytes.", f"func (value {r.symbol}) Encode() ([]byte,error) {{"]
        for p,s in r.cells:
            if s.startswith("float"):
                a=access(p,"value","go");lines.append(f"    if math.IsNaN(float64({a}))||math.IsInf(float64({a}),0){{return nil,invalid}}")
        lines.append(f"    out:=make([]byte,{r.wire_bytes})");offset=0
        for p,s in r.cells:
            a=access(p,"value","go");width=SCALARS[s][1]
            if s=="bool":lines.append(f"    if {a}{{out[{offset}]=1}}")
            elif width==1:lines.append(f"    out[{offset}]=byte({a})")
            else:
                rhs=f"math.Float{width*8}bits({a})" if s.startswith("float") else f"uint{width*8}({a})"
                lines.append(f"    binary.LittleEndian.PutUint{width*8}(out[{offset}:{offset+width}],{rhs})")
            offset+=width
        lines += ["    return out,nil", "}", f"// Decode{r.symbol} returns no value after malformed or trailing bytes.", f"func Decode{r.symbol}(bytes []byte) ({r.symbol},error) {{var value {r.symbol};if len(bytes)!={r.wire_bytes}{{return {r.symbol}{{}},invalid}}"]
        offset=0
        for p,s in r.cells:
            a=access(p,"value","go");width=SCALARS[s][1]
            if s=="bool":lines.append(f"    if bytes[{offset}]>1{{return {r.symbol}{{}},invalid}};{a}=bytes[{offset}]!=0")
            else:
                bits=f"bytes[{offset}]" if width==1 else f"binary.LittleEndian.Uint{width*8}(bytes[{offset}:{offset+width}])"
                rhs=f"math.Float{width*8}frombits({bits})" if s.startswith("float") else f"{SCALARS[s][4]}({bits})"
                lines.append(f"    {a}={rhs}")
                if s.startswith("float"):lines.append(f"    if math.IsNaN(float64({a}))||math.IsInf(float64({a}),0){{return {r.symbol}{{}},invalid}}")
            offset+=width
        lines += ["    return value,nil", "}"]
    # Force imports used even when a catalog contains only one scalar category.
    lines += ["var _=binary.LittleEndian", "var _=math.IsNaN", ""]
    return "\n".join(lines).replace("roswasmc_copy_object", "uwvm_custom_copy")

def as_default(t,r,records):
    return "false" if t=="bool" else "0" if t in SCALARS else "new "+records[(r.version,t)].symbol+"()"

def emit_as(records,ordered):
    lines=["/** Generated fixed topic values; no I/O, native serialization or authority.", " * Import path assumes output is sdk/custom-types/generated; adjust only the", " * module path if moving it. Codec.valid must succeed before exposing values. */",
        'import {Codec,Reader,Writer} from "../../assemblyscript/codecs";']
    for r in ordered:
        lines += [f"/** Fixed ROS{r.version} {r.name} value. */",f"export class {r.symbol} {{",f"/** Canonical schema identity; host grants remain separate. */\nstatic readonly codecIdentity:string={json.dumps(r.codec_identity)};"]
        for m in r.members:
            t=SCALARS[m.type][6] if m.type in SCALARS else records[(r.version,m.type)].symbol
            initial=as_default(m.type,r,records)
            if m.count is not None:
                lines += [f"/** Native field {m.name}: exactly {m.count} values. */",f"{m.name}:Array<{t}>=[{','.join([initial]*m.count)}];"]
            else:lines += [f"/** Native fixed field {m.name}. */",f"{m.name}:{t}={initial};"]
        lines += ["}", f"/** Checked SDK Codec for {r.name}; empty output signals invalid encoding. */", f"export class {r.symbol}Codec implements Codec<{r.symbol}> {{",f"encode(value:{r.symbol}):Uint8Array {{"]
        def check_arrays(rec,prefix):
            for m in rec.members:
                a=prefix+"."+m.name
                if m.count is not None:lines.append(f"if({a}.length!={m.count})return new Uint8Array(0);")
                if m.type not in SCALARS:
                    for i in range(m.count or 1):check_arrays(records[(rec.version,m.type)],a+(f"[{i}]" if m.count is not None else ""))
        check_arrays(r,"value")
        for p,s in r.cells:
            if s.startswith("float"):lines.append(f"if(!isFinite({access(p,'value')}))return new Uint8Array(0);")
        lines.append(f"const w=new Writer({r.wire_bytes});")
        for p,s in r.cells:
            a=access(p,"value");width=SCALARS[s][1]
            if s=="bool":lines.append(f"w.u8({a}?1:0);")
            elif s.startswith("float"):lines.append(f"w.integer(<u64>reinterpret<u{width*8}>({a}),{width});")
            else:lines.append(f"w.integer(<u64>{a},{width});")
        lines += ["return w.finish();}","/** Reject malformed bytes before decode is used by the subscription. */", f"valid(bytes:Uint8Array):bool {{if(bytes.length!={r.wire_bytes})return false;const r=new Reader(bytes);"]
        for p,s in r.cells:
            width=SCALARS[s][1]
            if s=="bool":lines.append("if(r.u8()>1)return false;")
            elif s.startswith("float"):lines.append(f"if(!isFinite(reinterpret<f{width*8}>(<u{width*8}>r.integer({width}))))return false;")
            else:lines.append(f"r.integer({width});")
        lines += ["return r.done();}","/** Decode previously validated bytes; malformed input returns a default value. */", f"decode(bytes:Uint8Array):{r.symbol} {{const value=new {r.symbol}();if(!this.valid(bytes))return value;const r=new Reader(bytes);"]
        for p,s in r.cells:
            a=access(p,"value");width=SCALARS[s][1]
            rhs="r.u8()!=0" if s=="bool" else f"reinterpret<f{width*8}>(<u{width*8}>r.integer({width}))" if s.startswith("float") else f"<{SCALARS[s][6]}>r.integer({width})"
            lines.append(f"{a}={rhs};")
        lines += ["return value;}","}"]
    return "\n".join(lines+[""])

def emit_manifest(ordered):
    lines=["/** @file Generated immutable C++17 fixed-topic registry.", " * Include ONLY through UWVM_ROS_CUSTOM_TOPIC_MANIFEST after descriptor types.", " * Official native identities are supplied, never calculated by this tool. */", "#pragma once", "namespace uwvm2::imported::ros::ipc {"]
    for r in ordered:
        layout=[]
        for _,s in r.cells:
            kind=SCALARS[s][0]
            if layout and layout[-1][0]==kind:layout[-1][1]+=1
            else:layout.append([kind,1])
        lines += [f"inline constexpr custom_scalar_layout custom_layout_{r.symbol}[]={{"+",".join("{custom_scalar_kind::"+k+","+str(n)+"}" for k,n in layout)+"};"]
    lines += ["inline constexpr custom_topic_descriptor custom_generated_topics[]={"]
    lines += [f"{{{r.version},{json.dumps(r.name)},{json.dumps(r.identity)},{json.dumps(r.codec_identity)},{r.wire_bytes},custom_layout_{r.symbol},sizeof(custom_layout_{r.symbol})/sizeof(custom_layout_{r.symbol}[0])}}," for r in ordered]
    lines += ["};", "inline constexpr custom_topic_registry custom_topic_entries() noexcept {return {custom_generated_topics,sizeof(custom_generated_topics)/sizeof(custom_generated_topics[0])};}", "}", ""]
    return "\n".join(lines)

def ros2_header(name):
    # Match official rosidl camel_to_snake convention, including acronym boundaries.
    return re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", re.sub(r"(.)([A-Z][a-z]+)", r"\1_\2", name)).lower()

def emit_native(ordered,version):
    selected=[r for r in ordered if r.version==version]
    lines=[f"/** @file Generated ROS{version} fixed-topic native conversion and dispatch.", " * Native generation/type support remains official. valid() allocates nothing;", " * encode validates all fields before modifying caller storage. decode commits", " * a complete temporary native record only after strict canonical validation.", " * Exact native field types/array shapes are asserted at compile time. Full", " * official native field/dependency descriptions MUST match native_schema(). */", "#pragma once", '#include "fixed_records.h"', "#include <vector>", "#include <utility>", "#include <array>", "#include <type_traits>", "#include <string_view>", "#include <uwvm2/imported/ros/ipc/custom_type.h>"]
    if version==1:lines.append("#include <boost/array.hpp>")
    for r in selected:
        parts=r.name.split("/");head=parts[0]+"/"+(parts[1]+"/" if version==2 else "")+(ros2_header(parts[-1])+".hpp" if version==2 else parts[-1]+".h")
        lines.append(f"#include <{head}>")
    lines += ["#ifndef UWVM_CUSTOM_NATIVE_SCHEMA_TRAITS", "#define UWVM_CUSTOM_NATIVE_SCHEMA_TRAITS", "namespace uwvm_custom_native {",
        "/** @brief Normalized ROS1 definition subsection, preserving every fixed field. */", "struct definition_entry {std::string_view type,fields;};",
        "/** @brief Exact fixed array shape; unsupported containers have size zero. */", "template<class T> struct fixed_array_shape {using value_type=void;static constexpr size_t size=0;};",
        "template<class T,size_t N> struct fixed_array_shape<std::array<T,N>> {using value_type=T;static constexpr size_t size=N;};", "}", "#endif"]
    if version==1:
        lines += ["#ifndef UWVM_CUSTOM_NATIVE_BOOST_ARRAY_TRAIT", "#define UWVM_CUSTOM_NATIVE_BOOST_ARRAY_TRAIT", "namespace uwvm_custom_native {", "template<class T,size_t N> struct fixed_array_shape<boost::array<T,N>> {using value_type=T;static constexpr size_t size=N;};", "}", "#endif"]
    lines.append("namespace uwvm_custom_native {")
    by_name={(r.version,r.name):r for r in ordered}
    for r in selected:
        native="::"+r.name.replace("/","::");cs=r.c_symbol;codec=r.symbol+"Codec"
        closure=[]
        def visit(rec):
            if rec in closure:return
            closure.append(rec)
            for m in rec.members:
                if m.type not in SCALARS:visit(by_name[(rec.version,m.type)])
        visit(r)
        # Keep traits checks at field granularity, including nested fixed shapes.
        for index,rec in enumerate(closure):
            owner="::"+rec.name.replace("/","::")
            for field_index,m in enumerate(rec.members):
                alias=f"{codec}_field_{index}_{field_index}"
                declared=SCALARS[m.type][2] if m.type in SCALARS else "::"+m.type.replace("/","::")
                if m.type=="bool" and version==2:declared="bool"
                lines.append(f"using {alias}=std::remove_cv_t<std::remove_reference_t<decltype(std::declval<{owner}&>().{m.name})>>;")
                if m.count is not None:
                    lines.append(f"static_assert(fixed_array_shape<{alias}>::size=={m.count},\"native fixed array length differs from generated schema\");")
                    lines.append(f"static_assert(std::is_same<typename fixed_array_shape<{alias}>::value_type,{declared}>::value,\"native fixed array element type differs from generated schema\");")
                else:
                    lines.append(f"static_assert(std::is_same<{alias},{declared}>::value,\"native scalar/nested type differs from generated schema\");")
        lines += [f"/** @brief Fixed converter for official {r.name}; native identities require runtime verification. */", f"struct {codec} {{", f"    using Native={native};", f"    static constexpr char const* type={json.dumps(r.name)};", f"    static constexpr char const* native_identity={json.dumps(r.identity)};", f"    static constexpr char const* codec_identity={json.dumps(r.codec_identity)};", f"    static constexpr size_t wire_bytes={r.wire_bytes};", "    static bool valid(Native const& value) noexcept {"]
        for p,s in r.cells:
            a=access(p,"value");width=SCALARS[s][1]
            if s=="bool":lines.append(f"        if({a}!=0&&{a}!=1)return false;")
            if s.startswith("float"):
                mask="UINT32_C(0x7f800000)" if width==4 else "UINT64_C(0x7ff0000000000000)"
                lines.append(f"        {{uint{width*8}_t bits=0;uwvm_custom_copy(&bits,&{a},{width});if((bits&{mask})=={mask})return false;}}")
        lines += ["        return true;", "    }", "    static bool validate(Native const& value) noexcept {return valid(value);}",
            "    /** @brief Full normalized native root/dependency definitions; constants/defaults are unsupported. */", "    inline static constexpr definition_entry definitions[]={"]
        for rec in closure:
            definition="".join(m.type+(f"[{m.count}]" if m.count is not None else "")+" "+m.name+"\n" for m in rec.members)
            lines.append(f"        {{{json.dumps(rec.name)},{json.dumps(definition)}}},")
        lines += ["    };", "    static constexpr size_t definition_count=sizeof(definitions)/sizeof(definitions[0]);"]
        for index,rec in enumerate(closure):
            lines.append(f"    inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_{index}[]={{")
            lines += [f"        {{{json.dumps(m.name)},{json.dumps(m.type)},{m.count or 0},{'true' if m.count is not None else 'false'}}}," for m in rec.members]
            lines.append("    };")
        lines.append("    inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_record schema_records[]={")
        lines += [f"        {{{json.dumps(rec.name)},schema_fields_{index},sizeof(schema_fields_{index})/sizeof(schema_fields_{index}[0])}}," for index,rec in enumerate(closure)]
        lines += ["    };", "    /** @brief Exact ordered fields for every native record, including the transitive closure. */", "    static constexpr ::uwvm2::imported::ros::ipc::custom_native_schema native_schema() noexcept {return {schema_records,sizeof(schema_records)/sizeof(schema_records[0])};}",
            "    static bool encode(Native const& value,uint8_t* output,size_t capacity) {", f"        if(!output||capacity<wire_bytes||!valid(value))return false;{cs} record{{}};"]
        for p,s in r.cells:lines.append(f"        {access(p,'record')}={access(p,'value')};")
        lines += [f"        size_t length;return !{cs}_encode(&record,output,capacity,&length);", "    }", "    static bool decode(uint8_t const* bytes,size_t size,Native& out) {", f"        {cs} record{{}};if({cs}_decode(bytes,size,&record))return false;Native next{{}};"]
        for p,s in r.cells:lines.append(f"        {access(p,'next')}={access(p,'record')};")
        lines += ["        out=std::move(next);return true;", "    }", "    static bool encode(Native const& value,std::vector<uint8_t>& out) {if(!valid(value))return false;std::vector<uint8_t> next(wire_bytes);if(!encode(value,next.data(),next.size()))return false;out=std::move(next);return true;}", "};"]
    lines += ["}",f"#define UWVM_ROS_CUSTOM_ROS{version}_TYPES(M)"+(" \\" if selected else "")]
    for i,r in enumerate(selected):
        lines.append(f"    M(::{r.name.replace('/', '::')},{json.dumps(r.name)},{json.dumps(r.identity)},{json.dumps(r.codec_identity)},{r.wire_bytes},::uwvm_custom_native::{r.symbol}Codec)"+(" \\" if i+1<len(selected) else ""))
    lines += [f"#define UWVM_ROS_CUSTOM_ROS{version}_TOPIC_TYPES(M) UWVM_ROS_CUSTOM_ROS{version}_TYPES(M)", ""]
    return "\n".join(lines).replace("roswasmc_copy_object", "uwvm_custom_copy")

def generate(path: Path,output: Path,guest_only=False):
    records,ordered,pins=load_catalog(path,not guest_only)
    products={"fixed_records.h":emit_c(records,ordered),"fixed_records.hpp":emit_cpp(ordered),"fixed_records.rs":emit_rust(records,ordered),
        "fixed_records.mjs":emit_js(records,ordered),"fixed_records.ts":emit_as(records,ordered),"fixed_records.go":emit_go(records,ordered)}
    if not guest_only:
        products.update({"manifest.hpp":emit_manifest(ordered),"native_ros1.hpp":emit_native(ordered,1),"native_ros2.hpp":emit_native(ordered,2)})
    proof={"format":PROFILE,"status":"GENERATED_GUEST_SOURCE_ONLY" if guest_only else "GENERATED_SOURCE_ONLY","native_registry_emitted":not guest_only,"native_identity_provenance":"not required for guest-only source; otherwise caller-supplied official identities requiring native verification",
        "native_execution":"NOT_RUN","compiler_execution":"NOT_RUN","source_inputs":pins,
        "generator_sha256":hashlib.sha256(bounded_read(Path(__file__),MAX_SOURCE_BYTES)).hexdigest(),
        "types":[{"ros_version":r.version,"type":r.name,"native_identity":r.identity,"adapter_codec_identity":r.codec_identity,"wire_bytes":r.wire_bytes,"scalar_cells":len(r.cells),"native_operations":["topic.publish","topic.subscribe"]} for r in ordered],
        "packages":{"ros1":sorted({r.name.split('/')[0] for r in ordered if r.version==1}),"ros2":sorted({r.name.split('/')[0] for r in ordered if r.version==2})},
        "outputs":{k:{"bytes":len(v.encode()),"sha256":hashlib.sha256(v.encode()).hexdigest()} for k,v in products.items()}}
    if output.exists():
        raise GenerationError("output path already exists; choose a fresh directory to preserve prior artifacts")
    output.mkdir(parents=True)
    for name,text in products.items():
        (output/name).write_text(text,encoding="utf-8")
    (output/"generation.json").write_text(json.dumps(proof,indent=2)+"\n",encoding="utf-8")
    return proof

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("catalog",type=Path,help="bounded fixed-topic catalog JSON; paths are relative to its directory")
    parser.add_argument("--check",action="store_true",help="parse/resolve only, permitting absent native identities; never emit native registration")
    parser.add_argument("--guest-only",action="store_true",help="emit Guest source only (explicit control profile: C/C++ only), without native identities or native registry/provider headers")
    parser.add_argument("--out",type=Path,help="fresh output directory; existing artifacts are never overwritten")
    args=parser.parse_args()
    try:
        bounded=bounded_profile(args.catalog)
        if args.check:
            if args.guest_only:raise GenerationError("--check and --guest-only are mutually exclusive")
            if args.out:raise GenerationError("--check never writes outputs; omit --out")
            if bounded:
                result=bounded.check(sys.modules[__name__],args.catalog)
            else:
                _,ordered,pins=load_catalog(args.catalog,False)
                result={"status":"SCHEMA_PREPARATION_ONLY","types":[{"type":r.name,"ros_version":r.version,"wire_bytes":r.wire_bytes,"scalar_cells":len(r.cells),"adapter_codec_identity":r.codec_identity} for r in ordered],"source_inputs":pins,"native_execution":"NOT_RUN"}
        else:
            if args.out is None:raise GenerationError("registry emission requires --out pointing to a fresh directory")
            result=bounded.generate(sys.modules[__name__],args.catalog,args.out,args.guest_only) if bounded else generate(args.catalog,args.out,args.guest_only)
        print(json.dumps(result,indent=2))
    except (GenerationError,OSError,TypeError,RecursionError) as e:
        print(f"custom_types: {e}",file=sys.stderr);return 2
    return 0

if __name__=="__main__":
    raise SystemExit(main())
