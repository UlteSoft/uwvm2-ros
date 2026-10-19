#!/usr/bin/env python3
"""Compile explicit Unitree fixtures serially and emit physical build receipts.

Root supplies a pinned recipe through the normal input map. Commands are direct
compiler/linker argv arrays, with full -MD system dependencies and original
source/object/depfile associations. No compiler is discovered, downloaded, or
run during import. This producer must be executed only after the complete
implementation freeze; successful compilation is not simulation acceptance.
"""
from pathlib import Path
import shlex

from common import Actors, Frame, arguments, exact, require


def relative(root, value):
    path = Path(value)
    require(type(value) is str and not path.is_absolute() and ".." not in path.parts and value,
            "safe nonempty relative product required")
    return root / path


def dependencies(frame, filename, output, source):
    raw, pin = frame.common.body_pin(filename)
    text = raw.decode("utf-8", "strict").replace("\\\n", " ")
    require(":" in text, "complete compiler dependency rule required")
    target, tail = text.split(":", 1)
    require(shlex.split(target) == [str(output)], "actual dependency target differs from owned object")
    names = shlex.split(tail)
    require(names and not any(name.endswith(":") for name in names), "one full dependency rule required; -MP is not admitted")
    result = []
    for name in names:
        item = Path(name)
        require(item.is_absolute(), "absolute full compiler dependency paths required")
        item = item.resolve(strict=True)
        require(item.is_relative_to(frame.source) or any(item.is_relative_to(root) for root in frame.roots),
                "compiler dependency outside all frozen source/SDK/tool roots")
        value = frame.pin(item)
        if value not in result:
            result.append(value)
    require(frame.pin(source) in result, "actual translation unit absent from full dependencies")
    return frame.pin(filename), result


def link_dependencies(frame, row, objects):
    """Reopen actual --trace libraries, objects and archive-member containers."""
    values = []
    for stream_name in ("stdout", "stderr"):
        raw = Path(row[stream_name]).read_bytes()
        require(len(raw) <= frame.limits["log_bytes"], "bounded actual linker trace required")
        for line in raw.decode("utf-8", "strict").splitlines():
            # GNU ld and LLD report archive members as /path/lib.a(member.o).
            # Resolve the actual archive rather than an invented member path.
            name = line.strip().split("(", 1)[0]
            if not name.startswith("/"):
                continue
            path = Path(name)
            if not path.is_file():
                continue  # A diagnostic containing an absolute path is not a trace input.
            path = path.resolve(strict=True)
            require(path in objects or path.is_relative_to(frame.source) or
                    any(path.is_relative_to(root) for root in frame.roots),
                    "actual linker input outside frozen source/SDK/tool roots")
            pin = frame.pin(path)
            if pin not in values:
                values.append(pin)
    require(values and all(frame.pin(path) in values for path in objects),
            "actual linker trace must include every compiled object")
    return values


def main():
    parser = arguments(__doc__)
    parser.add_argument("--recipe-input", required=True)
    args = parser.parse_args()
    frame = None
    try:
        frame = Frame(args, "unitree-fixture-preparation")
        recipe, _ = frame.common.document(frame.input(args.recipe_input))
        exact(recipe, ("format_version", "targets"), "fixture recipe")
        require(recipe["format_version"] == 1 and type(recipe["targets"]) is list and 1 <= len(recipe["targets"]) <= 64,
                "bounded explicit fixture target list required")
        owner, names, products = Actors(frame), set(), []
        for target in recipe["targets"]:
            exact(target, ("id", "output", "kind", "definitions", "steps", "compiles", "validation"), "fixture target")
            name = target["id"]
            require(type(name) is str and name.replace("-", "").isalnum() and 1 <= len(name) <= 64 and name not in names,
                    "unique safe fixture target id required")
            names.add(name)
            folder = frame.out / name
            folder.mkdir(mode=0o700)
            output = relative(folder, target["output"])
            require(target["kind"] in ("native", "wasm") and type(target["definitions"]) is dict and
                    type(target["steps"]) is list and 1 <= len(target["steps"]) <= 32 and
                    type(target["compiles"]) is list and target["compiles"], "actual bounded compile/link recipe required")
            compile_map = {}
            for item in target["compiles"]:
                exact(item, ("step", "source", "object", "dependency"), "compile association")
                require(type(item["step"]) is int and 0 <= item["step"] < len(target["steps"]) and item["step"] not in compile_map,
                        "one original translation unit per compile command required")
                compile_map[item["step"]] = item
            require(len(target["steps"]) >= 2 and len(compile_map) == len(target["steps"]) - 1 and
                    len(target["steps"]) - 1 not in compile_map,
                    "one or more compile commands followed by one separate final link required")
            require(all(type(key) is str and key.isidentifier() and type(value) is int
                        for key, value in target["definitions"].items()), "explicit integral compile definitions required")
            commands, sources, objects = [], [], []
            for index, job in enumerate(target["steps"]):
                command = frame.command(job, {"target_out": str(folder)})
                require(not any(value in ("-ffast-math", "-Ofast", "-DNDEBUG", "-MMD", "-MP") for value in command),
                        "checked full-MD non-fast-math recipe required")
                if index in compile_map:
                    record = compile_map[index]
                    source = relative(frame.source, record["source"]).resolve(strict=True)
                    obj, dep = relative(folder, record["object"]), relative(folder, record["dependency"])
                    require(all(value in command for value in ("-c", "-MD", "-MF", "-O3", "-ffp-contract=off", str(source), str(obj), str(dep))),
                            "actual source/object/full-MD/optimized flags association required")
                    require(command[command.index("-MF") + 1] == str(dep) and command[command.index("-o") + 1] == str(obj),
                            "exact owned object/depfile output selection required")
                    for key, value in target["definitions"].items():
                        require("-D" + key + "=" + str(value) in command,
                                "recorded fixture macro absent from actual compile argv")
                else:
                    require(index == len(target["steps"]) - 1 and all(str(obj) in command for obj in objects) and
                            "-o" in command and command[command.index("-o") + 1] == str(output) and
                            any(flag in command for flag in ("--trace", "-Wl,--trace")),
                            "final direct link must consume all actual compiled objects")
                child, row = owner.start(name + "-%02d" % index, command, data=b"", json_output=False)
                owner.finish(child)
                deps = []
                if index in compile_map:
                    dependency_pin, deps = dependencies(frame, dep, obj, source)
                    frame.pin(obj)
                    objects.append(obj)
                    for value in deps:
                        if Path(value["path"]).is_relative_to(frame.source) and value not in sources:
                            sources.append(value)
                else:
                    deps = link_dependencies(frame, row, objects)
                commands.append({"argv": command, "returncode": row["returncode"],
                    "stdout": frame.pin(row["stdout"]), "stderr": frame.pin(row["stderr"]), "dependencies": deps})
            output_pin = frame.pin(output)
            with output.open("rb") as stream:
                prefix = stream.read(8)
            require((target["kind"] == "wasm" and prefix == b"\0asm\x01\0\0\0") or
                    (target["kind"] == "native" and prefix[:4] == b"\x7fELF"), "actual linked output kind differs")
            if target["kind"] == "wasm":
                validator = frame.command(target["validation"], {"target_out": str(folder)})
                require(validator[1:] == ["validate", str(output)], "explicit actual wasm-tools validation job required")
                child, row = owner.start(name + "-actual-validator", validator, data=b"", json_output=False)
                owner.finish(child)
                commands.append({"argv": validator, "returncode": row["returncode"], "stdout": frame.pin(row["stdout"]),
                                 "stderr": frame.pin(row["stderr"]), "dependencies": [output_pin]})
            else:
                require(target["validation"] is None, "native binary validation is its actual later ELF/architecture admission")
            proof = {"format_version": 1, "kind": "unitree-fixture-build", "status": "PASS",
                "source_manifest_sha256": frame.report["source_manifest_sha256"], "output": output_pin,
                "source_inputs": sources, "definitions": target["definitions"], "commands": commands,
                "full_dependencies_verified": True}
            path = folder / "build-receipt.json"
            frame.common.atomic_json(path, proof)
            products.append({"id": name, "output": output_pin, "receipt": frame.pin(path)})
        frame.report["products"] = products
        frame.report["scope"] = "Actual serial fixture compilation/linking only; no simulator, Runtime acceptance or performance executed"
        return frame.finish("PASS_ACTUAL_FIXTURE_PREPARATION")
    except BaseException as error:
        if frame:
            return frame.finish("FAIL", repr(error))
        raise


if __name__ == "__main__":
    raise SystemExit(main())
