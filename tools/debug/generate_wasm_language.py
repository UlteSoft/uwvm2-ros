#!/usr/bin/env python3
"""Regenerate IDE opcode metadata from the VM's actual instruction display table.
No network/toolchain dependency. --check verifies the checked-in editor assets.
"""
import argparse
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "tools/debug"


def leb(value):
    out = []
    while True:
        byte = value & 127
        value >>= 7
        out.append(byte | (128 if value else 0))
        if not value:
            return out


def describe(name, prefix):
    if prefix == 0xfe:
        if name == "atomic.fence":
            return "Threads", "Sequentially consistent atomic fence. No stack operands; the binary includes a reserved zero byte."
        if ".wait" in name:
            return "Threads", "Compare the shared-memory value and wait when equal. Stack: address, expected value, i64 timeout in nanoseconds; result i32 is 0 (notified), 1 (not equal), or 2 (timed out)."
        if name.endswith(".notify"):
            return "Threads", "Notify up to the requested number of waiters at a shared-memory address. Stack: address, i32 count; result is the i32 number woken."
        action = "Atomically read and modify memory, returning the previous value."
        if ".load" in name:
            action = "Atomically load a value; narrow loads zero-extend to the result integer type."
        elif ".store" in name:
            action = "Atomically store a value; narrow stores retain the low bits. No result."
        elif "cmpxchg" in name:
            action = "Atomically compare with the expected value and store the replacement on equality; return the previous value."
        return "Threads", action + " Sequentially consistent. The effective address must meet natural alignment and bounds. A selected memory64 uses an i64 address. Bounds checks and configured MTE checks apply independently."
    if prefix == 0xfb:
        if name.startswith(("struct.", "array.")):
            detail = "Typed GC aggregate operation. Type, field, element and segment immediates are validated against the selected module; null/type/bounds traps remain enforced."
        else:
            detail = "Typed GC/reference operation. Cast and nullability behavior follows the validated reference types; it does not turn a reference into a native pointer."
        return "GC", detail + " GC objects have no linear-memory allocation tags. An i32/i64 tagged linear address carried in a GC field must retain its tag when later used to access linear memory."
    if prefix == 0xfd:
        detail = "128-bit SIMD operation; the mnemonic identifies the lane type, lane width and operation."
        if "relaxed" in name:
            detail += " Relaxed SIMD permits the standard's specified alternative numerical results."
        if "load" in name or "store" in name:
            detail += " Memory index, offset, alignment and lane are shown where present; memory64 address types and MTE checks apply."
        return "SIMD", detail
    if name.startswith(("local.", "global.")):
        what, op = name.split(".")
        return "Variables", f"Module/frame {what} index immediate. " + {"get":"Push the current value.", "set":"Pop and assign the value.", "tee":"Assign the value and retain it on the operand stack."}[op]
    if name.startswith(("memory.", "data.")) or re.match(r"^[if](32|64)\.(load|store)", name):
        return "Memory", "Linear-memory operation. Memory indices are module-local, including multi-memory. Memory64 changes the address/size operand type; offsets remain exact integers. Bounds and configured MTE checks apply independently."
    if name.startswith(("table.", "elem.")):
        return "Tables", "Typed reference table/element-segment operation. Indices identify module-local tables or segments; element type and table bounds remain checked."
    if name.startswith("ref.") or name.startswith("br_on_"):
        return "References", "Typed reference operation or reference-conditional branch. Reference type/nullability and label arity are validated; references are not exposed as native addresses."
    controls = {
        "unreachable":"Unconditionally trap; also used by compiled abort/assert paths.",
        "nop":"No stack or memory effect.",
        "block":"Enter a structured block with its validated block type.",
        "loop":"Enter a structured loop; its branch label denotes the loop continuation.",
        "if":"Pop an i32 condition and select a structured branch.",
        "else":"Begin the alternative arm of a structured if.",
        "end":"End the current structured control construct.",
        "br":"Unconditionally branch to the selected enclosing label.",
        "br_if":"Pop an i32 condition and branch when nonzero.",
        "br_table":"Pop an i32 selector and branch through the immediate label vector, using the default label when out of range.",
        "return":"Return the current function's declared results.",
        "throw":"Throw the selected module-local exception tag with its validated payload.",
        "throw_ref":"Throw an existing typed exception reference.",
        "try_table":"Enter a typed exception-handling region with an immediate catch/label table.",
        "call":"Call the module-local function index with its declared signature.",
        "call_indirect":"Call through a typed table element after checking table bounds and the expected function type.",
        "call_ref":"Call a typed function reference after its required null/type checks.",
        "return_call":"Tail-call the module-local function; transfer the current continuation.",
        "return_call_indirect":"Tail-call a typed table element after the required bounds and type checks.",
        "return_call_ref":"Tail-call a typed function reference.",
        "drop":"Discard one operand-stack value.",
        "select":"Pop two values and an i32 condition; choose the first value when the condition is nonzero.",
        "select_t":"Typed select. The binary includes an explicit value-type vector.",
    }
    if name in controls:
        return "Control", controls[name]
    return "Numeric", "Scalar numeric instruction. The mnemonic specifies the value type and operation; _s/_u distinguish signed/unsigned interpretation. Constant immediates are values, not host addresses."



def brief(name, family, description):
    """Only the operation's useful meaning; representation/security details stay in docs."""
    if family == "Threads":
        if name == "atomic.fence": return "Sequentially consistent fence."
        if ".wait" in name: return "Wait if equal; 0 = woken, 1 = unequal, 2 = timeout."
        if name.endswith(".notify"): return "Wake up to count waiters; return the number woken."
        if ".load" in name: return "Atomic load (seq_cst)."
        if ".store" in name: return "Atomic store (seq_cst)."
        if "cmpxchg" in name: return "Compare and exchange; return the previous value (seq_cst)."
        action = name.rsplit(".", 1)[-1].removesuffix("_u")
        return "Atomic " + action + "; return the previous value (seq_cst)."
    if family == "GC":
        op = name.split(".")[-1]
        if name == "array.len": return "Read the array length."
        if name.startswith("struct."):
            if op.startswith("new"): return "Create a typed struct."
            if op.startswith("get"): return "Read a struct field."
            if op == "set": return "Write a struct field."
        if name.startswith("array."):
            if op.startswith("new"): return "Create a typed array."
            if op.startswith("get"): return "Read an array element."
            if op == "set": return "Write an array element."
            if op == "fill": return "Fill an array range."
            if op == "copy": return "Copy an array range."
            if op.startswith("init_"): return "Initialize an array range from a segment."
        if name.startswith("ref.test"): return "Test the reference type."
        if name.startswith("ref.cast"): return "Cast the reference; trap on a type mismatch."
        if name == "br_on_cast": return "Branch when the reference cast succeeds."
        if name == "br_on_cast_fail": return "Branch when the reference cast fails."
        return ""
    if family == "Variables":
        return {"get":"Read the variable.", "set":"Assign the variable.", "tee":"Assign and keep the value."}[name.split(".")[-1]]
    if family == "Memory":
        if ".load" in name: return "Read from linear memory."
        if ".store" in name: return "Write to linear memory."
        return {"memory.size":"Read the memory size in pages.", "memory.grow":"Grow memory; return its previous size, or -1 on failure.",
                "memory.copy":"Copy a memory range.", "memory.fill":"Fill a memory range with a byte.",
                "memory.init":"Copy a data segment into memory.", "data.drop":"Release a passive data segment."}.get(name,"")
    if family == "Tables":
        return {"table.get":"Read a table entry.", "table.set":"Write a table entry.", "table.size":"Read the table length.",
                "table.grow":"Grow the table; return its previous size, or -1 on failure.", "table.fill":"Fill a table range.",
                "table.copy":"Copy a table range.", "table.init":"Copy an element segment into a table.",
                "elem.drop":"Release a passive element segment."}.get(name,"")
    if family == "References":
        return {"ref.null":"Push a null reference.", "ref.is_null":"Test for null.", "ref.func":"Produce a function reference.",
                "ref.eq":"Compare reference identity.", "ref.as_non_null":"Require a non-null reference.",
                "br_on_null":"Branch on a null reference.", "br_on_non_null":"Branch on a non-null reference."}.get(name,"")
    if family == "Control": return description.split(". ", 1)[0].rstrip(".") + "."
    op = name.split(".", 1)[-1]
    actions={"add":"Add", "sub":"Subtract", "mul":"Multiply", "div":"Divide", "div_s":"Signed division",
             "div_u":"Unsigned division", "rem_s":"Signed remainder", "rem_u":"Unsigned remainder",
             "and":"Bitwise AND", "or":"Bitwise OR", "xor":"Bitwise XOR", "not":"Bitwise NOT",
             "shl":"Shift left", "shr_s":"Arithmetic shift right", "shr_u":"Logical shift right",
             "rotl":"Rotate left", "rotr":"Rotate right", "clz":"Count leading zero bits", "ctz":"Count trailing zero bits",
             "popcnt":"Count set bits", "eqz":"Test for zero", "eq":"Compare for equality", "ne":"Compare for inequality",
             "abs":"Absolute value", "neg":"Negate", "sqrt":"Square root", "ceil":"Round toward positive infinity",
             "floor":"Round toward negative infinity", "trunc":"Round toward zero", "nearest":"Round to nearest, ties to even",
             "min":"Minimum", "max":"Maximum", "copysign":"Copy the sign bit", "const":"Push a constant"}
    if op in actions: return actions[op] + (" per lane." if family == "SIMD" and op != "const" and not name.startswith("v128.") else ".")
    if family == "SIMD":
        if op.startswith("load"): return "Load from linear memory into a vector."
        if op.startswith("store"): return "Store vector data to linear memory."
        if "extract_lane" in op: return "Read the selected vector lane."
        if op == "replace_lane": return "Replace the selected vector lane."
        if op == "splat": return "Replicate the value into every lane."
    # The mnemonic already identifies the operation; don't substitute generic filler.
    return ""

def build():
    text = (ROOT / "src/uwvm2/uwvm/debugger/wasm_listing.h").read_text()
    entries = {}
    prefix = None
    in_table = False
    for line in text.splitlines():
        if "switch(op)" in line:
            in_table = True
            continue
        if not in_table:
            continue
        found = re.search(r"case 0x([0-9a-f]+)u: switch\(extension\)", line)
        if found:
            prefix = int(found[1], 16)
            continue
        found = re.search(r'case 0x([0-9a-f]+)u: return "([^"]+)";', line)
        if found:
            code, name = int(found[1], 16), found[2]
            family, detail = describe(name, prefix)
            wire = [code] if prefix is None else [prefix] + leb(code)
            entries[name] = dict(family=family, description=detail, brief=brief(name, family, detail), encoding=" ".join(f"{b:02x}" for b in wire),
                                 primary=code if prefix is None else prefix, subopcode=None if prefix is None else code)
        if line.strip().startswith('default: return "extended-opcode"'):
            prefix = None
        if line.strip() == 'default: return "opcode";':
            break

    mt = (ROOT / "src/uwvm2/extension/memtag/instruction.h").read_text()
    enum = mt[mt.index("enum class opcode"):mt.index("[[nodiscard]]")]
    codes = {key:int(value,16) for key,value in re.findall(r"(\w+)\s*=\s*0x([0-9a-f]+)u",enum)}
    rows = re.findall(r'\{opcode::(\w+), u8"([^"]+)", (\d+)u, (\d+)u, \{([^}]*)\}, (true|false), (true|false), (true|false), (true|false)\}',mt)
    assert len(rows) == 20
    meanings = {
        "status":"Return the selected runtime's MTE implementation status (0 when disabled).",
        "extract":"Extract the pointer tag as an integer.",
        "insert":"Replace the input address's pointer tag with the supplied tag.",
        "tagbits":"Return the selected memory's pointer-tag width (0 when disabled).",
        "startbit":"Return the first pointer-tag bit position (the address width when disabled).",
        "copy":"Copy the second address's pointer tag into the first address, preserving the first address's location.",
        "sub":"Subtract the two untagged address locations, returning the address-width difference.",
        "load":"Read the allocation tag at the addressed granule and return the input address retagged with that allocation tag.",
        "untag":"Return the address with its pointer-tag bits cleared.",
        "untagstore":"Clear the pointer tag and store zero allocation tags over the byte range; return the untagged address.",
        "untagstorez":"Clear/store zero tags over the byte range, zero its bytes and return the untagged address.",
        "store":"Store the input address's pointer tag as allocation tags over the byte range.",
        "storez":"Store the pointer tag as allocation tags over the byte range and zero its bytes.",
        "random":"Return the input address with a newly selected random pointer tag.",
        "randomstore":"Select a random pointer tag, store it over the byte range and return the retagged address.",
        "randomstorez":"Select/store a random tag, zero the byte range and return the retagged address.",
        "hint":"Return the input address retagged with tag_source's tag plus tag_offset, masked to the tag width.",
        "hintstore":"Select the hinted tag, store it over the byte range and return the retagged address.",
        "hintstorez":"Select/store the hinted tag, zero the byte range and return the retagged address.",
        "add":"Unspecified arithmetic placeholder; not an executable instruction.",
    }
    mte_briefs = {
        "status":"Read MTE status.", "extract":"Extract the pointer tag.", "insert":"Replace the pointer tag.",
        "tagbits":"Read the pointer-tag width.", "startbit":"Read the first pointer-tag bit.",
        "copy":"Copy a pointer tag without changing the address.", "sub":"Subtract untagged addresses.",
        "load":"Read the allocation tag into the pointer.", "untag":"Clear the pointer tag.",
        "untagstore":"Clear pointer and allocation tags.", "untagstorez":"Clear tags and zero the range.",
        "store":"Set allocation tags from the pointer tag.", "storez":"Set allocation tags and zero the range.",
        "random":"Choose a random pointer tag.", "randomstore":"Choose and store a random tag.",
        "randomstorez":"Choose/store a random tag and zero the range.",
        "hint":"Select tag_source + tag_offset, masked to the tag width.",
        "hintstore":"Select a hinted tag and store it.", "hintstorez":"Select/store a hinted tag and zero the range.",
        "add":"Reserved; cannot execute.",
    }
    for op,name,inputs,outputs,roles,reads,writes,zeroes,random in rows:
        role_names = re.findall(r"operand_role::(\w+)", roles)
        description = ("With MTE execution enabled: " + meanings[op] + " Experimental UWVM linear-memory MTE v1, domain 1; not a standard Wasm opcode. "
                       "One unsigned LEB32 memory-index immediate. Stack operands (deepest first): "
                       + (", ".join(role_names) or "none") + f"; results: {outputs}. "
                       "All operands/results use the selected memory's address type (i32 or i64); lengths and address offsets are bytes. ")
        if reads == "true": description += "Reads allocation tags. "
        if writes == "true": description += "Writes allocation tags. "
        if zeroes == "true": description += "Also zeroes the covered memory. "
        if random == "true": description += "Chooses a random pointer tag. "
        description += "Granule: 16 bytes. memory32 pointer tag: 4 bits; memory64: 8 bits. "
        if op == "add":
            description += "Reserved descriptor only: execution/validation is rejected because the reference arithmetic semantics are unspecified."
        word = 0x01010000 | codes[op]
        entries["uwvm.linear.v1::" + name] = dict(family="MTE", description=description, brief=mte_briefs[op], encoding=" ".join(f"{b:02x}" for b in [255]+leb(word)),
            primary=255, subopcode=word, stackOperands=role_names, results=int(outputs), supported=op!="add")
    assert len([e for e in entries.values() if e["family"] == "Threads"]) == 67
    # Targets and operands have their own scopes, including coloured code
    # snippets inside compact hovers. No theme/user settings are overwritten.
    pattern = [
        {"match":r"\b(memory|table|type)(=)([0-9]+)",
         "captures":{"1":{"name":"entity.name.type.target.uwvm-wasm"},"2":{"name":"keyword.operator.assignment.uwvm-wasm"},"3":{"name":"entity.name.type.target.uwvm-wasm"}}},
        {"match":r"\b(function)(=)([0-9]+)",
         "captures":{"1":{"name":"entity.name.function.target.uwvm-wasm"},"2":{"name":"keyword.operator.assignment.uwvm-wasm"},"3":{"name":"entity.name.function.target.uwvm-wasm"}}},
        {"match":r"\b(local|global|label)(=)([0-9]+)",
         "captures":{"1":{"name":"variable.other.target.uwvm-wasm"},"2":{"name":"keyword.operator.assignment.uwvm-wasm"},"3":{"name":"variable.other.target.uwvm-wasm"}}},
        {"match":r"\b(offset|align|lane)(=)([0-9]+)",
         "captures":{"1":{"name":"variable.parameter.operand.uwvm-wasm"},"2":{"name":"keyword.operator.assignment.uwvm-wasm"},"3":{"name":"constant.numeric.operand.uwvm-wasm"}}},
    ]
    indexed = [
        (r"uwvm\.linear\.v1::memtag\.[a-z]+", "entity.name.function.memtag", "entity.name.type.target"),
        (r"local\.(?:get|set|tee)|global\.(?:get|set)", "variable.language", "variable.other.target"),
        (r"call|return_call", "keyword.control", "entity.name.function.target"),
        (r"ref\.func", "storage.type.reference", "entity.name.function.target"),
        (r"table\.(?:get|set)", "support.function.table", "entity.name.type.target"),
        (r"br|br_if", "keyword.control", "entity.name.label.target"),
    ]
    for names, instruction_scope, target_scope in indexed:
        pattern.append({"match":r"(?<![A-Za-z0-9_.:])("+names+r")([ \t]+)([0-9]+)(?![A-Za-z0-9_])",
                        "captures":{"1":{"name":instruction_scope+".uwvm-wasm"},"3":{"name":target_scope+".uwvm-wasm"}}})
    scopes = {"MTE":"entity.name.function.memtag", "Threads":"keyword.control.atomic", "GC":"entity.name.type.gc",
              "SIMD":"support.function.simd", "Memory":"support.function.memory", "Variables":"variable.language",
              "Tables":"support.function.table", "References":"storage.type.reference", "Control":"keyword.control",
              "Numeric":"support.function.numeric"}
    for family, scope in scopes.items():
        names = sorted((n for n,e in entries.items() if e["family"] == family), key=lambda n:(-len(n), n))
        # Regex.escape's backslashes are accepted by Oniguruma. The namespace is
        # part of the sole MTE spelling; historical FC encodings are not aliases.
        pattern.append({"match":r"(?<![A-Za-z0-9_.:])(?:"+"|".join(re.escape(n) for n in names)+r")(?![A-Za-z0-9_.:])",
                        "name":scope+".uwvm-wasm"})
    pattern.extend([
        {"match":r"(?<![A-Za-z0-9_])(?:0x[0-9a-fA-F]+|-?[0-9]+)(?![A-Za-z0-9_])", "name":"constant.numeric.uwvm-wasm"},
        {"match":r";.*$", "name":"comment.line.semicolon.uwvm-wasm"},
    ])
    grammar={"name":"UWVM Wasm Disassembly", "scopeName":"source.uwvm-wasm-disassembly",
        "patterns":[{"begin":r"^(;;[ \t]+)(0x(?:[0-9a-f]{16}|[0-9a-f]{8}))([ \t]+)", "end":"$",
          "beginCaptures":{"1":{"name":"punctuation.definition.comment.uwvm-wasm"},
                           "2":{"name":"comment.line.byte-offset.uwvm-wasm"}}, "patterns":pattern},
         {"match":r"\b(?:i32|i64|f32|f64|v128|ref|func|extern|struct|array|any|eq|i31)\b", "name":"storage.type.uwvm-wasm"},
         {"match":r";;.*$", "name":"comment.line.semicolon.uwvm-wasm"}, *pattern]}
    return {"wasm_opcodes.json":entries, "syntaxes/uwvm-wasm.tmLanguage.json":grammar}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check",action="store_true")
    args=parser.parse_args()
    for name,value in build().items():
        target=OUT/name
        expected=json.dumps(value,indent=2,ensure_ascii=False)+"\n"
        if args.check:
            if not target.is_file() or target.read_text()!=expected:
                raise SystemExit(f"stale generated Wasm language asset: {target}")
        else:
            target.parent.mkdir(exist_ok=True,parents=True)
            target.write_text(expected)
    print("Wasm language metadata and grammar match the VM opcode table")


if __name__ == "__main__":
    main()
