#!/usr/bin/env python3
"""Materialize full Spot5.0.1 C nanopb and C++ Lite SDK source.
This is an explicit SOURCE producer. It does not compile, test, run a Guest,
contact a robot, infer native TypeIdentity or grant a capability.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile

PROTOC_VERSION = "libprotoc 3.21.12"
NANOPB_COMMIT = "cad3c18ef15a663e30e3e43e3a752b66378adec1"
MAX_SOURCE_BYTES = 16 * 1024 * 1024
COMMANDS = []

# The fixed nanopb release exposes NamingStyle.var_name, but no field-rename
# option. Extend that actual generator strategy rather than editing original
# descriptors or doing search/replace on emitted C. Preserve all wire tags.
C_KEYWORDS = frozenset('''alignas alignof and and_eq asm atomic_cancel atomic_commit
atomic_noexcept auto bitand bitor bool break case catch char char8_t char16_t
char32_t class compl concept const consteval constexpr constinit const_cast
continue co_await co_return co_yield decltype default delete do double
dynamic_cast else enum explicit export extern false final float for friend
goto if import inline int long module mutable namespace new noexcept not
not_eq nullptr operator or or_eq override private protected public register
reinterpret_cast requires return short signed sizeof static static_assert
static_cast struct switch synchronized template this thread_local throw true
try typedef typeid typename typeof typeof_unqual union unsigned using virtual
void volatile wchar_t while xor xor_eq _Alignas _Alignof _Atomic _BitInt _Bool
_Complex _Decimal32 _Decimal64 _Decimal128 _Generic _Imaginary _Noreturn
_Static_assert _Thread_local'''.split())


def c_identifiers(names):
    """Return a collision-free, minimal C/C++ member spelling map.

    Account for every original field/oneof and generated discriminator/count
    helper globally. Original identifiers remain in the protobuf descriptor and
    field map. This policy changes only generated C member spelling.
    """
    names = set(names)
    if len(names) > 16384 or any(not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]{0,255}', n) for n in names):
        raise ValueError('bounded original member identifiers required')
    occupied = set(names)
    for name in names:
        occupied.update(('which_' + name, 'has_' + name, name + '_count', name + '_size'))
    result = {}
    for name in sorted(names):
        candidate = name
        if name in C_KEYWORDS:
            candidate += '_'
            def helpers(value):
                return {value, 'which_' + value, 'has_' + value, value + '_count', value + '_size'}
            while helpers(candidate) & occupied:
                candidate += '_'
                if len(candidate) > 512:
                    raise ValueError('C identifier escape budget exceeded')
            occupied.update(helpers(candidate))
        result[name] = candidate
    if len(set(result.values())) != len(result):
        raise ValueError('C member spelling collision')
    return result

def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()

def checked_file(path: Path, expected: str) -> Path:
    if not re.fullmatch(r"[a-f0-9]{64}", expected):
        raise ValueError("a full independently recorded SHA256 is required")
    path = path.resolve(strict=True)
    if not path.is_file() or sha(path) != expected:
        raise ValueError(f"file identity differs: {path}")
    return path

def run(argv: list[str], *, cwd: Path, env: dict[str, str], limit: int = 120):
    result = subprocess.run(argv, cwd=cwd, env=env, check=False, timeout=limit,
                            stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    observation = {"argv": argv, "cwd": str(cwd), "returncode": result.returncode,
                   "stdout": result.stdout.decode("utf-8", "replace"),
                   "stderr": result.stderr.decode("utf-8", "replace")}
    COMMANDS.append(observation)
    if result.returncode:
        raise RuntimeError(json.dumps(observation, ensure_ascii=True))
    return result

def original_inputs(root: Path, lock: dict) -> list[str]:
    rows = lock["protos"]
    if len(rows) != 157 or len({x["path"] for x in rows}) != 157:
        raise ValueError("expected149 SDK+2 official examples+6 WKT inputs")
    names = []
    total = 0
    for row in rows:
        name = row["path"]
        if not re.fullmatch(r"(?:bosdyn/api|google/protobuf|python/examples/self_registration)/[A-Za-z0-9_/]+\.proto", name):
            raise ValueError("noncanonical original path")
        p = root / name
        if p.is_symlink() or not p.is_file() or sha(p) != row["sha256"]:
            raise ValueError(f"original proto identity differs: {name}")
        total += p.stat().st_size
        names.append(name)
    if total > MAX_SOURCE_BYTES:
        raise ValueError("source byte bound exceeded")
    if sorted(str(p.relative_to(root)) for p in root.rglob("*.proto")) != sorted(names):
        raise ValueError("unlisted/missing proto")
    return sorted(n for n in names if not n.startswith("python/examples/"))

def descriptor_metadata(descriptor, output: Path):
    # Called only in the explicitly selected, hashed Python producer runtime.
    # Original descriptor order is not trusted for reference resolution.
    messages = {}
    source_fields = {}
    file_for_name = {}
    def visit(file, scope, message):
        name = ".".join(filter(None, (scope, message.name)))
        if name in messages:
            raise ValueError("duplicate fully qualified message")
        messages[name] = message
        file_for_name[name] = file.name
        for child in message.nested_type:
            visit(file, name, child)
    for file in descriptor.file:
        if file.syntax != "proto3":
            raise ValueError("this complete profile requires actual proto3 inputs")
        for message in file.message_type:
            visit(file, file.package, message)
    names = sorted(messages)
    if len(names) > 4096:
        raise ValueError("message schema bound exceeded")
    indices = {n: i for i, n in enumerate(names)}
    c_names = c_identifiers([field.name for msg in messages.values() for field in msg.field] +
                            [oneof.name for msg in messages.values() for oneof in msg.oneof_decl])
    (output/'c-identifiers.json').write_text(json.dumps({
        'kind': 'original-protobuf-to-C-member-identifiers-v1',
        'policy': 'Escape C/C++ keywords with a collision-free underscore suffix; original descriptors and tags are unchanged.',
        'renamed': {name: value for name, value in c_names.items() if name != value},
        'all_members': c_names}, sort_keys=True, indent=2)+'\n')
    schema = ['/* Generated from ORIGINAL157 file descriptors; do not edit. */',
              '#include "schema.h"']
    nano = ['/* Generated C representation binding; do not edit. */',
            '#include "document.h"']
    for file in sorted({file_for_name[n] for n in names}):
        nano.append('#include "' + file[:-6] + '.pb.h"')
    # Complete exact-field external options: no declaration or original file
    # changes. Pointer mode has no max_count/max_size truncation. Original
    # string fields use length-preserving bytes rather than C NUL strings.
    options = {file.name: ["* type:FT_POINTER"] for file in descriptor.file}
    for i, name in enumerate(names):
        msg = messages[name]
        if msg.extension_range or msg.extension:
            raise ValueError("unexpected proto2 extension")
        tags = set()
        fields = []
        for field in msg.field:
            if field.number in tags or field.type == 10:
                raise ValueError("duplicate tag/proto2 group")
            tags.add(field.number)
            child = indices[field.type_name.lstrip(".")] if field.type == 11 else 0xffffffff
            fields.append((field.number, child, field.type, int(field.label == 3)))
            source_fields[name + "." + field.name] = {
                "tag": field.number, "original_type": field.type,
                "type_name": field.type_name, "label": field.label,
                "nanopb_type_override": ("TYPE_BYTES" if field.type == 9 else "TYPE_INT32" if field.type == 14 else None)}
            source_fields[name + "." + field.name]['c_member_name'] = c_names[field.name]
            source_fields[name + "." + field.name]['c_oneof_name'] = (
                c_names[msg.oneof_decl[field.oneof_index].name] if field.HasField('oneof_index') else None)
            if field.type in (9,14):
                override = "TYPE_BYTES" if field.type == 9 else "TYPE_INT32"
                options[file_for_name[name]].append(
                    f"{name}.{field.name} type:FT_POINTER type_override:{override}")
        if fields:
            schema.append(f"static const uwvm_spot_proto_field fields_{i}[] = {{")
            schema += [f"  {{{tag}u,{child}u,{typ}u,{repeat}u}}," for tag,child,typ,repeat in fields]
            schema.append("};")
    schema.append("const uwvm_spot_proto_schema uwvm_spot_proto_schemas[] = {")
    for i,name in enumerate(names):
        count = len(messages[name].field)
        schema.append(f'  {{"{name}",' + (f"fields_{i}" if count else "NULL") + f",{count}u}},")
    schema += ["};", f"const size_t uwvm_spot_proto_schema_count={len(names)}u;"]
    nano.append("const uwvm_spot_proto_type uwvm_spot_proto_types[] = {")
    for i,name in enumerate(names):
        # The default nanopb C naming algorithm concatenates Names(parts)
        # with underscore; casing and ORIGINAL field spelling remain intact.
        c = name.replace(".", "_")
        nano.append(f"  {{&uwvm_spot_proto_schemas[{i}],&{c}_msg,sizeof({c})}},")
    nano += ["};", f"const size_t uwvm_spot_proto_type_count={len(names)}u;"]
    (output/"schema.c").write_text("\n".join(schema)+"\n")
    (output/"nanopb_types.c").write_text("\n".join(nano)+"\n")
    (output/"field-map.json").write_text(json.dumps(source_fields,sort_keys=True,indent=2)+"\n")
    (output/"messages.json").write_text(json.dumps(names,indent=2)+"\n")
    for name,lines in options.items():
        p=output/"options"/(name[:-6]+".options")
        p.parent.mkdir(parents=True,exist_ok=True)
        p.write_text("\n".join(lines)+"\n")
    return names

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument("--protoc", type=Path, required=True)
    p.add_argument("--protoc-sha256", required=True)
    p.add_argument("--python", type=Path, required=True)
    p.add_argument("--python-sha256", required=True)
    p.add_argument("--protobuf-root", type=Path, required=True)
    p.add_argument("--nanopb-root", type=Path, required=True)
    p.add_argument("--source-lock", type=Path, required=True)
    p.add_argument("--output", type=Path, required=True)
    a=p.parse_args()
    here=Path(__file__).resolve().parent
    lock=json.loads(a.source_lock.read_text())
    protoc=checked_file(a.protoc,a.protoc_sha256)
    python=checked_file(a.python,a.python_sha256)
    nano=a.nanopb_root.resolve(strict=True)
    protobuf=a.protobuf_root.resolve(strict=True)
    # Every selected generator/runtime source is independently pinned. In
    # particular a system grpc_tools module cannot hijack invoke_protoc().
    for row in lock["nanopb"]:
        checked_file(nano/row["path"],row["sha256"])
    for row in lock["python_runtime"]:
        checked_file(protobuf/row["path"],row["sha256"])
    if not lock["python_runtime"]:
        raise ValueError("actual Python protobuf runtime source inventory is required")
    names=original_inputs(here/"upstream/protos",lock)
    out=a.output.absolute()
    if out.exists():
        raise ValueError("fresh output required")
    out.mkdir(parents=False,mode=0o700)
    env={"PATH": str(protoc.parent), "LANG":"C", "LC_ALL":"C",
         "PYTHONNOUSERSITE":"1", "PYTHONDONTWRITEBYTECODE":"1",
         "PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION":"python",
         "NANOPB_PB2_NO_REBUILD":"1"}
    version=run([str(protoc),"--version"],cwd=here,env=env).stdout.decode().strip()
    if version!=PROTOC_VERSION:
        raise ValueError("protoc/runtime3.21.12 exact-version coupling required")
    for sub in ("cpp","c","tooling","options"):
        (out/sub).mkdir(mode=0o700)
    # Never import an accepted-source directory's pre-existing bytecode.
    # Copy ONLY the thirteen admitted official sources into a fresh private
    # plugin tree. Its real inventory must match before any helper import.
    import shutil
    nano_original=nano
    nano=out/"tooling"/"nanopb"
    nano.mkdir(mode=0o700)
    for row in lock["nanopb"]:
        target=nano/row["path"]
        target.parent.mkdir(parents=True,exist_ok=True,mode=0o700)
        shutil.copyfile(nano_original/row["path"],target)
    expected_nano={row["path"] for row in lock["nanopb"]}
    actual_nano={str(f.relative_to(nano)) for f in nano.rglob("*") if f.is_file()}
    if len(expected_nano)!=13 or actual_nano!=expected_nano or any(
        "__pycache__" in f.parts or f.suffix==".pyc" for f in nano.rglob("*")):
        raise ValueError("fresh plugin tree must contain exactly13 source inputs and no bytecode")
    for row in lock["nanopb"]:
        checked_file(nano/row["path"],row["sha256"])
    run([str(protoc),"-I"+str(here/"upstream/protos"),
         "--include_imports","--descriptor_set_out="+str(out/"original.pb")]+names,
        cwd=here/"upstream/protos",env=env)
    runtime=out/"tooling"/"runtime"
    runtime.mkdir()
    import shutil
    for row in lock["python_runtime"]:
        relative=Path(row["path"]).relative_to("python")
        target=runtime/relative
        target.parent.mkdir(parents=True,exist_ok=True)
        shutil.copyfile(protobuf/row["path"],target)
    # These real original tool schemas bootstrap descriptor_pb2/plugin_pb2.
    # No handwritten Python descriptor class substitutes for protoc.
    run([str(protoc),"-I"+str(protobuf/"src"),
         "--python_out="+str(runtime),
         "google/protobuf/descriptor.proto","google/protobuf/compiler/plugin.proto"],
        cwd=protobuf/"src",env=env)
    # A separate explicitly selected Python process reads descriptor.pb, emits
    # metadata/options and invokes the official plugin. No tool imported into
    # the caller's arbitrary ambient environment.
    worker=out/"tooling"/"worker.py"
    worker.write_text(
        "import sys\nsys.dont_write_bytecode=True\nimport runpy\n"
        f"sys.path[:0]=[{str(runtime)!r}]\n"
        "from google.protobuf import descriptor_pb2\n"
        "import google.protobuf\n"
        "assert google.protobuf.__version__=='4.21.12'\n"
        f"ns=runpy.run_path({str(Path(__file__).resolve())!r},run_name='spot_source_library')\n"
        f"d=descriptor_pb2.FileDescriptorSet.FromString(open({str(out/'original.pb')!r},'rb').read())\n"
        f"ns['descriptor_metadata'](d,ns['Path']({str(out)!r}))\n")
    run([str(python),"-I","-B",str(worker)],cwd=out,env=env)
    run([str(protoc),"-I"+str(nano/"generator/proto"),
         "-I"+str(here/"upstream/protos"),"-I"+str(protobuf/"src"),
         "--python_out="+str(out/"tooling"),str(nano/"generator/proto/nanopb.proto")],
        cwd=nano/"generator/proto",env=env)
    launcher=out/"tooling"/"protoc-gen-nanopb"
    launcher.write_text(
        "#!"+str(python)+" -I\n"
        "import sys\nsys.dont_write_bytecode=True\nimport json\n"
        f"sys.path[:0]=[{str(runtime)!r},{str(nano/'generator')!r},{str(out/'tooling')!r}]\n"
        "import nanopb_generator as generator\n"
        f"with open({str(out/'c-identifiers.json')!r}, encoding='utf-8') as source: names=json.load(source)['renamed']\n"
        "class CompleteCNames(generator.NamingStyle):\n"
        "    def var_name(self, name):\n"
        "        original=super().var_name(name)\n"
        "        return names.get(original, original)\n"
        "generator.Globals.naming_style=CompleteCNames()\n"
        "generator.main_plugin()\n")
    launcher.chmod(0o700)
    # All155 production original files are true generator inputs; the two
    # unrelated official self_registration examples remain unchanged Source; no hand-made schema,
    # regex proto parser or output fixture replaces the compiler descriptor.
    run([str(protoc),"-I"+str(here/"upstream/protos"),
         "--cpp_out=lite:"+str(out/"cpp")]+names,cwd=here/"upstream/protos",env=env)
    run([str(protoc),"-I"+str(here/"upstream/protos"),
         "--plugin=protoc-gen-nanopb="+str(launcher),
         "--nanopb_out=-I"+str(out/"options")+":"+str(out/"c")]+names,
        cwd=here/"upstream/protos",env=env)
    products=[]
    for f in sorted(out.rglob("*")):
        if f.is_file():
            products.append({"path":str(f.relative_to(out)),"bytes":f.stat().st_size,"sha256":sha(f)})
    (out/"source-generation.json").write_text(json.dumps({
        "kind":"ACTUAL_SOURCE_GENERATION_ONLY","protoc_version":version,
        "protoc_sha256":sha(protoc),"python_sha256":sha(python),
        "source_lock_sha256":sha(a.source_lock),"inputs":names,"products":products,
        "source_producer_sha256":sha(Path(__file__)),
        "C_member_identifier_policy":str(out/"c-identifiers.json"),
        "commands":COMMANDS,"plugin_path":str(launcher),"plugin_sha256":sha(launcher),
        "plugin_original_sources":lock["nanopb"],
        "python_protobuf_runtime_version":"4.21.12",
        "python_protobuf_runtime_source":lock["python_runtime"],
        "fresh_plugin_initial_inventory":sorted(actual_nano),
        "compiler_tests_native_guest_ssh":0},indent=2)+"\n")

if __name__=="__main__":
    main()
