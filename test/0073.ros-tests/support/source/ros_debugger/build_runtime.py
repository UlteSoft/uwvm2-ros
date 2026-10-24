#!/usr/bin/env python3
"""Build fresh, native LLVM-full ROS debugger Workers under the original cgroup.

This producer performs compilation only. It does not run a Worker, import a
Guest, contact ROS, download an SDK, reuse an object, or grant any authority.
Each of the three profiles has its own three full-MD objects and executable.
The separate integration controller must qualify and execute these receipts.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import shlex
import stat
import sys
import types

sys.dont_write_bytecode = True
PROFILES = ("ros1", "ros2", "dual")
UNITS = (("runtime", "src/uwvm2/runtime/lib/uwvm_runtime.default.cpp"),
         ("main", "src/uwvm2/uwvm/main.default.cpp"),
         ("host_api", "src/uwvm2/uwvm/host_api.default.cpp"))
LIMITS = {"jobs": 1, "rss_bytes": 2147483648, "command_seconds": 600,
          "log_bytes": 33554432}
# The dependency preparer imports LIMITS and retains its 2-GiB command cap.
# Real Runtime translation units need a separate 8-GiB owned-tree budget;
# this does not alter compiler flags, seriality, deadlines or cgroup reserve.
WORKER_LIMITS = {**LIMITS, "rss_bytes": 8589934592}
MEMORY_MAX = 68719476736
CONSUMER_FILE_MAX = 512 * 1024**2
UNSAFE_FP = frozenset(("-ofast", "-ffast-math", "-ffp-model=fast", "-ffp-model=aggressive",
    "-ffinite-math-only", "-funsafe-math-optimizations", "-fassociative-math",
    "-fno-signed-zeros", "-freciprocal-math", "-fno-honor-nans", "-fno-honor-infinities",
    "-fapprox-func", "/fp:fast"))


def require(ok, message):
    if not ok:
        raise ValueError(message)


def closed(value, keys, label):
    require(type(value) is dict and set(value) == set(keys), label + " has an unexpected schema")


def path(value, directory=False):
    require(type(value) is str and Path(value).is_absolute() and "\0" not in value,
            "explicit absolute input path required")
    result = Path(value).resolve(strict=True)
    require(result.is_dir() if directory else result.is_file(), "input kind differs: " + value)
    return result


def strings(value, label):
    require(type(value) is list and all(type(item) is str and item and "\0" not in item
                                      for item in value), label + " must be explicit argv strings")
    return value


def inside(value, roots):
    return any(value == root or root in value.parents for root in roots)


def safe_fp(tokens):
    for token in tokens:
        normalized = re.sub(r"^(?:/clang:|-clang:|-xclang=)", "", token.lower())
        require(normalized not in UNSAFE_FP, "unsafe floating-point compilation option: " + token)


def no_extra_llvm(tokens, label):
    """Keep all LLVM providers exclusively in the verified CMake response.

    Inspect forwarded linker operands as well as ordinary driver arguments.
    Generic response/configuration/full-script indirection is not a safe place
    to hide a second provider; normal stdlib/crypto/system flags remain valid.
    The sole LLVM response is added later from the qualified package itself.
    """
    operands = []
    for token in tokens:
        if token.startswith("-Wl,"):
            operands.extend(token[4:].split(","))
        elif token.startswith("-Xlinker="):
            operands.append(token.split("=", 1)[1])
        elif token != "-Xlinker":
            operands.append(token)
    previous = None
    for value in operands:
        lower = value.lower()
        require(not lower.lstrip('-').startswith('print-archive-stats'),
                label + ' cannot replace the owned linker archive-statistics output')
        section_address = re.fullmatch(r"-T(?:text(?:-segment)?|data|bss)(?:=(?:0x[0-9a-fA-F]+|[0-9]+))?", value) is not None
        require(not value.startswith("@") and not lower.startswith(("--config", "--options-file", "--script", "--default-script"))
                and value != "-T" and not (value.startswith("-T") and len(value) > 2 and not section_address),
                label + " cannot use unqualified response/config/full-linker-script indirection")
        library = (lower[2:] if value.startswith("-l") else
                   lower.split("=", 1)[1] if lower.startswith("--library=") else
                   lower if previous in ("-l", "--library") else "")
        name = Path(value.split("=", 1)[-1]).name.lower()
        require("llvm" not in library and not name.startswith(("libllvm", "llvm")),
                label + " cannot add LLVM search libraries, DSOs, plugins or archives outside the verified response")
        previous = value


def load_common(source, manifest):
    """Admit the opened helper against the supplied frozen manifest before exec.

    Only bounded standard-library metadata processing occurs before this
    comparison. The complete source inventory is then verified by that exact
    admitted helper and again after all commands.
    """
    def bounded_body(filename, limit):
        fd = os.open(filename, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
        try:
            info = os.fstat(fd)
            require(stat.S_ISREG(info.st_mode) and 0 <= info.st_size <= limit,
                    "bounded regular bootstrap input required")
            body = bytearray()
            while True:
                part = os.read(fd, min(65536, limit + 1 - len(body)))
                if not part:
                    break
                body.extend(part)
                require(len(body) <= limit, "bootstrap input exceeded bound")
            after = os.fstat(fd); now = Path(filename).stat(follow_symlinks=False)
            require(len(body) == info.st_size and all(getattr(info, key) == getattr(after, key) ==
                    getattr(now, key) for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")),
                    "bootstrap input changed while opening")
            return bytes(body)
        finally:
            os.close(fd)
    def pairs(values):
        result = {}
        for key, value in values:
            require(key not in result, "duplicate frozen manifest key")
            result[key] = value
        return result
    entries = json.loads(bounded_body(manifest, 16 * 1024**2), object_pairs_hook=pairs)
    require(type(entries) is dict, "complete frozen manifest required")
    helper = source / "tools/ros_distro/common.py"
    body = bounded_body(helper, 256 * 1024)
    helper_sha = hashlib.sha256(body).hexdigest()
    for relative, actual in (("tools/ros_distro/common.py", body),
                             ("test/0073.ros-tests/support/source/ros_debugger/build_runtime.py", bounded_body(Path(__file__).resolve(), 256 * 1024))):
        expected = entries.get(relative)
        require(type(expected) is dict and expected.get("sha256") == hashlib.sha256(actual).hexdigest() and
                expected.get("bytes") == len(actual), "bootstrap implementation differs from frozen manifest: " + relative)
    module = types.ModuleType("uwvm_debug_runtime_build_common")
    module.__file__ = str(helper)
    exec(compile(body, str(helper), "exec"), module.__dict__)
    return module, helper_sha


def load_execution_adapter(common, source, manifest):
    """Load only the helper bound by this actual complete source manifest."""
    frozen, _ = common.document(manifest)
    relative = "test/0073.ros-tests/support/source/ros_debugger/target_execution.py"
    body, pin = common.body_pin(source / relative, 256 * 1024)
    require(frozen.get(relative) == {key: pin[key] for key in ("bytes", "sha256")},
            "foreign execution adapter differs from complete source")
    module = types.ModuleType("uwvm_debugger_target_execution")
    module.__file__ = str(source / relative)
    exec(compile(body, module.__file__, "exec"), module.__dict__)
    return module


def configuration(config, source, adapter=None):
    """Admit explicit existing packages; an LLVM version label is never proof."""
    keys = ("format_version", "compiler", "linker", "target", "stdlib", "sysroot", "roots",
            "include_directories", "compile_flags", "link_flags", "library_inputs",
            "profile_compile_flags", "environment", "llvm", "cgroup")
    foreign = type(config.get("format_version")) is int and config["format_version"] == 2
    closed(config, keys + (("execution",) if foreign else ()), "build configuration")
    require(type(config["format_version"]) is int and config["format_version"] in (1, 2),
            "unsupported build configuration version")
    require(len(strings(config["compiler"], "compiler")) == 1,
            "one explicit clang++ executable required; shell/env wrappers are not admitted")
    config["compiler"][0] = str(path(config["compiler"][0]))
    linker_spelling = Path(config["linker"])
    path(config["linker"])
    # ld.lld may be a symlink to the multicall lld binary. Keep that proven
    # basename for execution; the whole-root catalog binds the symlink and its
    # target separately. Canonicalizing it to plain 'lld' changes its driver.
    config["linker"] = str(linker_spelling.parent.resolve(strict=True) / linker_spelling.name)
    closed(config["target"], ("triple", "machine", "bits", "endian", "host_machine"), "target")
    target = config["target"]
    require(type(target["triple"]) is str and re.fullmatch(r"[A-Za-z0-9_.+-]+", target["triple"]),
            "explicit compiler target triple required")
    known = {"x86_64": (62, 64, "little"), "amd64": (62, 64, "little"),
             "aarch64": (183, 64, "little"), "arm64": (183, 64, "little"),
             "riscv64": (243, 64, "little"), "loongarch64": (258, 64, "little"),
             "ppc64le": (21, 64, "little"), "s390x": (22, 64, "big")}
    actual_host = platform.machine().lower()
    if foreign:
        require(adapter is not None, "v2 requires the source-bound foreign execution adapter")
        adapter.validate_selection(target, config["execution"], actual_host)
    else:
        require(target["host_machine"] == actual_host and actual_host in known and
                (target["machine"], target["bits"], target["endian"]) == known[actual_host],
                "v1 requires the matching native host; foreign targets require explicit v2 selection")
    closed(config["roots"], ("toolchain", "llvm", "sdk", "stdlib"), "complete input roots")
    roots = {}
    for name, values in config["roots"].items():
        require(type(values) is list and values, "nonempty explicit " + name + " roots required")
        roots[name] = [path(item, True) for item in values]
        require(len(set(roots[name])) == len(roots[name]), "duplicate selected roots")
    all_roots = sorted(set(root for items in roots.values() for root in items))
    require(not any(root == source or source in root.parents for root in all_roots),
            "source is independently frozen; do not inventory the output/source as an SDK")
    require(all(inside(path(value), roots["toolchain"]) for value in
                (config["compiler"][0], config["linker"])), "compiler/linker must belong to selected whole tool roots")
    if foreign:
        require(type(config["sysroot"]) is str and config["sysroot"] == config["execution"]["sysroot"] and
                str(Path(config["sysroot"]).resolve(strict=True)) == config["sysroot"],
                "v2 compile/runtime sysroot must use the same literal canonical physical directory")
    if config["sysroot"] is not None:
        sysroot = path(config["sysroot"], True)
        require(inside(sysroot, all_roots), "sysroot must have complete selected-root inventory")
        config["sysroot"] = str(sysroot)
    includes = [path(item, True) for item in strings(config["include_directories"], "include directories")]
    require(all(inside(item, all_roots) or inside(item, [source]) for item in includes),
            "include directory is outside frozen source/whole package roots")
    config["include_directories"] = [str(item) for item in includes]
    for name in ("compile_flags", "link_flags"):
        strings(config[name], name)
        safe_fp(config[name])
        require(not any('print-archive-stats' in value.lower() for value in config[name]),
                'configuration cannot replace the owned linker archive-statistics output')
        require(not any(value.startswith(("@", "-o", "-MF", "-MT", "-MMD", "-MD", "-std=",
                      "--sysroot", "--target", "-fuse-ld", "-DUWVM", "-UUWVM")) or
                      value in ("-c", "-E", "-S", "-shared", "-fsyntax-only", "-D", "-U") or
                      "UWVM" in value for value in config[name]),
                "configuration may not override owned outputs, backend, target or dependency flags")
    no_extra_llvm(config["link_flags"], "generic link_flags")
    require(config["stdlib"] in ("libc++", "libstdc++") and all(
        [value for value in config[name] if value.startswith("-stdlib=")] == ["-stdlib=" + config["stdlib"]]
        for name in ("compile_flags", "link_flags")), "one matching explicit standard library is required for compile and link")
    closed(config["profile_compile_flags"], PROFILES, "profile compile flags")
    for values in config["profile_compile_flags"].values():
        strings(values, "profile compile flags")
        require(all(value.startswith("-DUWVM_ROS") and "IMPORT_ROS" not in value and
                    "\n" not in value for value in values), "only explicit custom ROS registry defines are allowed")
    config["library_inputs"] = [str(path(item)) for item in strings(config["library_inputs"], "library inputs")]
    no_extra_llvm(config["library_inputs"], "generic library_inputs")
    require(config["library_inputs"] and all(inside(Path(item), all_roots) and
            (Path(item).suffix == ".a" or re.search(r"\.so(?:\.[A-Za-z0-9_.+-]+)?$", Path(item).name))
            for item in config["library_inputs"]),
            "explicit crypto/system-library inputs must have whole-root coverage")
    environment = config["environment"]
    require(type(environment) is dict and set(environment) <= {"PATH", "LD_LIBRARY_PATH", "LANG", "LC_ALL"}
            and "PATH" in environment and all(type(value) is str and "\0" not in value for value in environment.values()),
            "closed explicit compiler-only environment required; no ambient environment inheritance")
    for name in ("PATH", "LD_LIBRARY_PATH"):
        if name in environment:
            require(environment[name] and all(value and Path(value).is_absolute() and
                    inside(path(value, True), all_roots) for value in environment[name].split(":")),
                    name + " must contain only nonempty inventoried absolute directories")
    closed(config["cgroup"], ("memory_max", "memory_swap_max", "expected_leaf", "expected_device",
                              "expected_inode", "expected_boot_id", "expected_cpus"), "original cgroup")
    require(config["cgroup"]["memory_max"] == MEMORY_MAX and config["cgroup"]["memory_swap_max"] == 0,
            "current original64GiB/zero-swap identity required")
    closed(config["llvm"], ("mode", "package_manifest", "package_sha256", "source_lock", "contract",
                            "generated_include", "public_include", "configuration", "consumer_response_file",
                            "shared_library", "build_root", "source_root"), "LLVM package selection")
    require(config["llvm"]["mode"] == "static-response" and config["llvm"]["shared_library"] is None,
            "current frozen LLVM contract requires static archives; shared/stock LLVM is unsupported")
    return roots, all_roots


def elf(common, filename, target, kinds=(2, 3)):
    pin = common.opened(filename, CONSUMER_FILE_MAX)
    with Path(filename).open("rb") as stream:
        header = stream.read(64)
    require(common.opened(filename) == pin and header[:4] == b"\x7fELF" and len(header) >= 52,
            "stable complete ELF input required: " + str(filename))
    endian = {1: "little", 2: "big"}.get(header[5])
    bits = {1: 32, 2: 64}.get(header[4])
    require(endian == target["endian"] and bits == target["bits"] and
            int.from_bytes(header[18:20], endian) == target["machine"] and
            int.from_bytes(header[16:18], endian) in kinds, "ELF architecture/type differs from actual native selection")
    return pin


def archive(common, filename, target):
    """Check every actual archive member header; thin/external/bitcode objects refuse."""
    pin = common.opened(filename, CONSUMER_FILE_MAX)
    members = 0
    with Path(filename).open("rb") as stream:
        require(stream.read(8) == b"!<arch>\n", "only complete native regular archives are supported")
        while stream.tell() < pin["bytes"]:
            header = stream.read(60)
            require(len(header) == 60 and header[58:] == b"`\n", "malformed archive member")
            raw_name = header[:16].decode("ascii").strip()
            require(re.fullmatch(rb" *[0-9]+ *", header[48:58]) is not None, "invalid archive member length")
            size = int(header[48:58]); start = stream.tell()
            require(start + size <= pin["bytes"], "archive member exceeds input")
            if raw_name not in ("/", "//", "/SYM64/") and not raw_name.startswith("__.SYMDEF"):
                prefix = int(raw_name[3:]) if raw_name.startswith("#1/") else 0
                require(0 <= prefix <= size, "invalid BSD archive filename")
                stream.seek(start + prefix)
                body = stream.read(min(64, size - prefix))
                require(len(body) >= 52 and body[:4] == b"\x7fELF", "archive member is not a native ELF object")
                endian = {1: "little", 2: "big"}.get(body[5])
                require(endian == target["endian"] and {1: 32, 2: 64}.get(body[4]) == target["bits"] and
                        int.from_bytes(body[18:20], endian) == target["machine"] and
                        int.from_bytes(body[16:18], endian) == 1, "archive contains a different target object")
                members += 1
            stream.seek(start + size + size % 2)
    require(members > 0 and common.opened(filename) == pin, "empty/changed native archive")
    return pin, members


def llvm_archive_statistics(common, filename, *, archives, member_counts, trace_paths,
                            expected_archive_operands, roots, target, guard=lambda: None):
    """Replay real LLD statistics without claiming unused members were linked.

    LLD's -t output names extracted archive members, whereas its independent
    --print-archive-stats output records every offered archive occurrence. The
    first occurrence of each raw spelling carries its extracted count; later
    occurrences of that raw spelling carry zero. Keep both observations and the
    exact ordered/repeated response operands, not a synthesized trace line.

    This actor-free helper opens files but runs no tool. Callers supply the
    already qualified provider full-seven pins/native member counts, actual
    canonical trace paths and original whole roots. Additional non-LLVM driver
    archives must have the same native/full-file/root proof. An exactly empty
    ordinary ar is recorded separately without an architecture claim; it can
    never replace a nonempty selected LLVM/package archive.
    """
    from collections import Counter
    pin_keys = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
    require(type(archives) is list and archives and type(member_counts) is dict and
            type(trace_paths) in (list, set, tuple, dict) and
            type(expected_archive_operands) is list and expected_archive_operands,
            'explicit selected archives, actual counts/trace and ordered operands required')
    selected = {}
    for pin in archives:
        require(type(pin) is dict and set(pin) == pin_keys and type(pin['path']) is str and
                type(pin['sha256']) is str and re.fullmatch('[0-9a-f]{64}', pin['sha256']) and
                all(type(pin[key]) is int and pin[key] >= 0 for key in pin_keys - {'path', 'sha256'}) and
                pin['inode'] > 0 and pin['path'] not in selected, 'distinct closed selected archive full-seven pins required')
        selected[pin['path']] = pin
    require(set(member_counts) == set(selected) and all(type(value) is int and value > 0
            for value in member_counts.values()), 'actual nonempty native counts must match every selected archive')
    require(roots and all(isinstance(root, Path) and root.is_absolute() and root.resolve(strict=True) == root
            and root.is_dir() and root != Path('/') for root in roots), 'original complete physical roots required')

    def spelling(value):
        """Record actual parent/alias identities without inventing raw text.

        Directory time/nlink changes from unrelated names do not change path
        resolution authority. Directory device/inode/mode and every symlink's
        full naming facts/target must remain stable across archive admission.
        """
        pending = list(Path(value).parts[1:]); current = Path('/'); rows = []; links = 0
        while pending:
            part = pending.pop(0)
            if part == '..':
                current = current.parent
                continue
            candidate = current / part; info = candidate.lstat()
            row = {'path': str(candidate), 'device': info.st_dev, 'inode': info.st_ino, 'mode': info.st_mode}
            if stat.S_ISLNK(info.st_mode):
                links += 1; require(links <= 40, 'finite implicit driver alias chain exceeded')
                target_name = os.readlink(candidate)
                row.update(target=target_name, bytes=info.st_size, mtime_ns=info.st_mtime_ns,
                           ctime_ns=info.st_ctime_ns, nlink=info.st_nlink)
                target_path = Path(target_name)
                if target_path.is_absolute():
                    current = Path('/'); pending = list(target_path.parts[1:]) + pending
                else:
                    pending = list(target_path.parts) + pending
            else:
                current = candidate
            rows.append(row)
        return {'canonical': str(current), 'components': rows}

    def physical(value, implicit=False):
        require(type(value) is str and '\0' not in value and '\n' not in value and '\r' not in value and '\t' not in value,
                'clean absolute physical archive path required')
        reported = Path(value)
        require(reported.is_absolute(), 'absolute linker archive spelling required')
        observation = spelling(value); item = Path(observation['canonical'])
        require((implicit or str(item) == value) and item.resolve(strict=True) == item and
                item.suffix == '.a' and inside(item, roots) and stat.S_ISREG(item.lstat().st_mode),
                'archive statistics path escapes its canonical ordinary whole-root input')
        return item, observation

    for name in selected:
        physical(name)
    for name in expected_archive_operands:
        physical(name)
    expected = Counter(expected_archive_operands)
    require(set(selected) <= set(expected), 'ordered response operands omit a selected package archive')
    require(all(type(value) is str and Path(value).is_absolute() and str(Path(value)) == value and
                Path(value).resolve(strict=True) == Path(value) and stat.S_ISREG(Path(value).lstat().st_mode)
                for value in trace_paths),
            'actual canonical trace paths required')
    traced = set(trace_paths)
    guard()
    statistics_path = Path(filename)
    require(statistics_path.is_absolute() and statistics_path.resolve(strict=True) == statistics_path and
            stat.S_ISREG(statistics_path.lstat().st_mode), 'ordinary physical owned archive-statistics sidecar required')
    raw, statistics_pin = common.body_pin(statistics_path, 1024**2)
    require(type(raw) is bytes and raw.endswith(b'\n') and b'\r' not in raw and b'\0' not in raw,
            'complete LF-terminated archive-statistics body required')
    lines = raw.decode('utf-8', errors='strict').splitlines()
    require(lines and lines[0] == 'members\textracted\tarchive' and 1 < len(lines) <= 16385,
            'exact LLD tab-separated archive-statistics header and finite rows required')
    rows, aggregates, reported_extraction = [], {}, {}
    for line_number, line in enumerate(lines[1:], 2):
        guard()
        parts = line.split('\t')
        require(len(parts) == 3 and all(re.fullmatch(r'0|[1-9][0-9]{0,8}', value) for value in parts[:2]),
                'exact nonnegative decimal members/extracted/archive row required')
        members, extracted = int(parts[0]), int(parts[1]); name = parts[2]
        require(extracted <= members, 'archive extracted count exceeds actual member count')
        item, path_before = physical(name, implicit=True)
        reported_name = name; name = str(item)
        is_llvm = any(value.lower().startswith(('libllvm', 'llvm')) for value in (item.name, Path(reported_name).name))
        require(not is_llvm or (name in selected and reported_name == name),
                'statistics introduced an extra, aliased or shared LLVM provider')
        if name not in aggregates:
            if members == 0:
                require(name not in selected and extracted == 0, 'selected LLVM/package archive cannot be empty')
                empty, current = common.body_pin(item, 8)
                require(empty == b'!<arch>\n' and current['bytes'] == 8,
                        'zero-member system archive must be exactly the ordinary empty ar header')
            else:
                current, actual_members = archive(common, item, target)
                require(actual_members == members, 'linker member count differs from actual native archive')
            if name in selected:
                require(current == selected[name] and members == member_counts[name],
                        'actual provider archive full-seven/member association changed')
            aggregates[name] = {'archive': current, 'members': members, 'occurrences': 0, 'extracted': 0,
                'trace_present': name in traced, 'selected_LLVM': name in selected,
                'scope': 'selected-provider' if name in selected else 'explicit-system' if name in expected else 'implicit-driver-system',
                'native_architecture_qualified': members > 0}
        else:
            entry = aggregates[name]
            require(members == entry['members'], 'physical archive occurrence changed its actual member count')
        require(reported_name not in reported_extraction or extracted == 0,
                'repeated raw archive spelling must report zero extracted after its first occurrence')
        reported_extraction[reported_name] = extracted if reported_name not in reported_extraction else reported_extraction[reported_name]
        entry = aggregates[name]; entry['occurrences'] += 1
        entry['extracted'] += extracted
        named = Path(reported_name).stat()
        require(stat.S_ISREG(named.st_mode) and all(getattr(named, field) == entry['archive'][key]
                for field, key in (('st_dev', 'device'), ('st_ino', 'inode'), ('st_size', 'bytes'),
                                   ('st_mtime_ns', 'mtime_ns'), ('st_ctime_ns', 'ctime_ns'))),
                'raw linker archive spelling no longer names the qualified physical file')
        require(spelling(reported_name) == path_before, 'linker archive spelling changed during qualification')
        rows.append({'line': line_number, 'archive': name, 'reported_archive': reported_name, 'members': members,
                     'extracted': extracted, 'occurrence': entry['occurrences'], 'path_observation': path_before})
    actual_selected = [row['archive'] for row in rows if row['archive'] in selected]
    expected_selected = [name for name in expected_archive_operands if name in selected]
    require(actual_selected == expected_selected, 'LLD selected archive order/repetitions differ from the actual response')
    require(set(expected) <= set(aggregates) and all(aggregates[name]['occurrences'] >= count
            for name, count in expected.items()), 'statistics omit an explicit offered archive occurrence')
    require({name for name in traced if Path(name).suffix == '.a'} <= set(aggregates),
            'real traced archive is absent from the complete statistics table')
    for name, entry in aggregates.items():
        require(entry['extracted'] == 0 or name in traced, 'extracted archive is absent from the real member trace')
        require(entry['extracted'] != 0 or name not in traced, 'zero-extracted archive contradicts the actual member trace')
        guard(); require(common.opened(Path(name), CONSUMER_FILE_MAX) == entry['archive'], 'statistics archive changed during qualification')
    for row in rows:
        guard(); require(spelling(row['reported_archive']) == row['path_observation'], 'archive parent/alias naming changed')
    guard(); require(common.opened(statistics_path, 1024**2) == statistics_pin, 'raw archive-statistics sidecar changed')
    return {'format_version': 1, 'kind': 'llvm-archive-statistics-v1', 'statistics': statistics_pin,
            'offered_archive_operands': list(expected_archive_operands), 'rows': rows,
            'archives': list(aggregates.values()), 'selected_archive_count': len(selected),
            'selected_archives_missing_from_trace_only_zero_extracted': [name for name in selected if name not in traced]}


def dependency_inputs(common, filename, output, source, roots):
    """Decode the one explicit -MD/-MF/-MT rule, including system headers."""
    body, dep_pin = common.body_pin(filename)
    text = body.decode().replace("\\\n", "")
    require(":" in text, "missing full compiler dependency rule")
    target, tail = text.split(":", 1)
    require(shlex.split(target) == [str(output)], "dependency target differs from exact owned object")
    names = shlex.split(tail)
    require(names and not any(name.endswith(":") for name in names), "one complete dependency rule required; -MP is not admitted")
    unique = {}
    for name in names:
        item = Path(name)
        require(item.is_absolute(), "compiler must retain absolute full-MD dependency paths")
        item = item.resolve(strict=True)
        require(inside(item, [source] + roots), "dependency is outside all source/SDK/tool closures: " + str(item))
        unique[str(item)] = common.opened(item)
    return dep_pin, list(unique.values())


def llvm_inputs(common, config, source, roots, remember, adapter=None):
    """Replay the current static CMake file-API contract, not a hand-written graph.

    An explicit package certificate binds previously built dependency bytes;
    this builder still compiles every Worker object itself. The source lock is
    checked against its separate complete LLVM source SDK, so a stock LLVM
    installation cannot become a patched package by changing a version label.
    """
    llvm = config["llvm"]
    package_path = path(llvm["package_manifest"])
    package, package_pin = common.document(package_path)
    require(re.fullmatch(r"[0-9a-f]{64}", llvm["package_sha256"] or "") and
            package_pin["sha256"] == llvm["package_sha256"], "reviewed LLVM package manifest SHA differs")
    require(type(package) is dict, "actual LLVM package certificate required")
    source_lock = path(llvm["source_lock"])
    contract = path(llvm["contract"])
    require(source_lock == source / "third-parties/llvm/sources.sha256" and
            contract == source / "xmake/llvm/contract.cmake", "current frozen LLVM source contract required")
    contract_body, _ = common.body_pin(contract)
    versions = re.findall(rb'if\(NOT PACKAGE_VERSION STREQUAL "([^"\n]+)"\)', contract_body)
    require(len(versions) == 1, "one explicit current LLVM contract version required")
    required_version = versions[0].decode()
    require(package.get("version") == required_version, "package version differs from current frozen patched contract")
    require(package.get("stdlib") == config["stdlib"], "LLVM package and Worker standard-library selections differ")
    source_sdk = path(llvm["source_root"], True)
    build = path(llvm["build_root"], True)
    require(inside(source_sdk, roots) and inside(build, roots), "complete LLVM source/build SDK inventories required")
    lock_pin, contract_pin = remember(source_lock), remember(contract)
    require(package.get("source_manifest_sha256") == lock_pin["sha256"] and
            package.get("contract_sha256") == contract_pin["sha256"], "LLVM source/package contract differs")
    external_lock = remember(source_sdk / "sources.sha256")
    require(external_lock["sha256"] == lock_pin["sha256"], "separate complete LLVM source SDK has a different lock")
    # Iterate the lock without retaining source bodies or the multi-GB source
    # tree. Whole-root before/after catalogs separately retain physical facts.
    names = set()
    with source_lock.open("r", encoding="utf-8") as stream:
        for line in stream:
            match = re.fullmatch(r"([0-9a-f]{64})  ([^\n]+)\n?", line)
            require(match is not None, "malformed current LLVM source lock")
            relative = Path(match[2])
            require(not relative.is_absolute() and ".." not in relative.parts and str(relative) not in names,
                    "unsafe/duplicate LLVM source-lock entry")
            names.add(str(relative))
            selected = (source_sdk / relative).resolve(strict=True)
            require(inside(selected, [source_sdk]) and common.opened(selected)["sha256"] == match[1],
                    "actual full LLVM source SDK differs from its patched lock: " + str(relative))
    require(names, "empty LLVM source lock")
    actual = {str(item.relative_to(source_sdk)) for subtree in ("cmake", "libc", "llvm", "third-party")
              for item in (source_sdk / subtree).rglob("*") if item.is_file()}
    require(actual <= names, "unrecorded LLVM source SDK inputs")
    generated = path(llvm["generated_include"], True)
    public = path(llvm["public_include"], True)
    require(generated == build / "include" and public == source / "third-parties/llvm/llvm/include",
            "actual build-generated configuration and frozen public headers required")
    configurations = [path(item) for item in strings(llvm["configuration"], "LLVM generated configuration")]
    version_header = generated / "llvm/Config/llvm-config.h"
    require(version_header in configurations, "actual generated LLVM version/configuration required")
    version_body, _ = common.body_pin(version_header)
    def define(name, quoted=True):
        pattern = rb'^#define ' + name.encode() + (rb' "([^"\n]+)"$' if quoted else rb' ([A-Za-z0-9_]+)$')
        values = re.findall(pattern, version_body, re.M)
        require(len(values) == 1, "one actual generated LLVM define required: " + name)
        return values[0].decode()
    native_arch = define("LLVM_NATIVE_ARCH", False)
    if config.get("format_version", 1) == 2:
        require(adapter is not None and native_arch == adapter.LLVM_ARCHES[config["execution"]["architecture"]],
                "foreign LLVM native target macro differs from selected Runtime architecture")
    require(define("LLVM_VERSION_STRING") == required_version and
            define("LLVM_HOST_TRIPLE") == define("LLVM_DEFAULT_TARGET_TRIPLE") == config["target"]["triple"],
            "generated LLVM version/native default triple differs from selected target")
    for macro, suffix in (("LLVM_NATIVE_TARGET", "Target"), ("LLVM_NATIVE_ASMPARSER", "AsmParser"),
                          ("LLVM_NATIVE_DISASSEMBLER", "Disassembler")):
        require(define(macro, False) == "LLVMInitialize" + native_arch + suffix,
                "actual native LLVM parser/target/disassembler is absent")
    require(package.get("generated_version_header_sha256") == remember(version_header)["sha256"],
            "generated header differs from package certificate")
    cache_body, cache_pin = common.body_pin(build / "CMakeCache.txt")
    cache = {}
    for line in cache_body.decode().splitlines():
        if line and not line.startswith(("#", "//")) and "=" in line and ":" in line.split("=", 1)[0]:
            key, value = line.split("=", 1); cache[key.split(":", 1)[0]] = value
    for key, value in cache.items():
        if re.fullmatch(r"CMAKE_C(?:XX)?_FLAGS(?:_[A-Za-z0-9_]+)?", key):
            safe_fp(shlex.split(value))
    require(cache.get("CMAKE_BUILD_TYPE") == "Release" and
            Path(cache.get("CMAKE_HOME_DIRECTORY", "")).resolve() == source_sdk / "llvm",
            "LLVM SDK must retain its actual Release source CMake origin")
    llvm_cxx = shlex.split(cache.get("CMAKE_CXX_FLAGS", ""))
    require([value for value in llvm_cxx if value.startswith("-stdlib=")] == ["-stdlib=" + config["stdlib"]] and
            path(cache.get("CMAKE_CXX_COMPILER")) == path(config["compiler"][0]),
            "LLVM and Worker must use the same explicit compiler/standard-library ABI provider")
    abi = lambda flags: [value for value in flags if "_GLIBCXX_USE_CXX11_ABI" in value]
    require(abi(llvm_cxx) == abi(config["compile_flags"]), "LLVM/Worker explicit libstdc++ string ABI flags differ")
    configured_contract = path(cache.get("CMAKE_PROJECT_LLVM_INCLUDE"))
    require(common.opened(configured_contract)["sha256"] == contract_pin["sha256"] and
            (inside(configured_contract, roots) or inside(configured_contract, [source])),
            "actual LLVM configuration did not use current frozen contract bytes")
    # Same file-API selection and flag filtering as bundled_llvm.lua and
    # utility.llvm_link_arguments: retain order and repeated archive arguments.
    reply = build / ".cmake/api/v1/reply"
    indexes = sorted(reply.glob("index-*.json"))
    require(indexes, "actual CMake file-API index missing")
    index, _ = common.document(indexes[-1]); remember(indexes[-1])
    entry = index.get("reply", {}).get("codemodel-v2")
    require(type(entry) is dict and Path(entry["jsonFile"]).name == entry["jsonFile"], "closed codemodel reply required")
    model, _ = common.document(reply / entry["jsonFile"]); remember(reply / entry["jsonFile"])
    require(len(model["configurations"]) == 1 and model["configurations"][0]["name"] == "Release",
            "one actual Release codemodel required")
    targets = model["configurations"][0]["targets"]
    selected = [row for row in targets if row["name"] == "uwvm_ros_llvm_contract"]
    require(len(selected) == 1, "actual ROS LLVM consumer target missing")
    def target_document(row):
        require(Path(row["jsonFile"]).name == row["jsonFile"], "unsafe file-API target document")
        value, _ = common.document(reply / row["jsonFile"]); remember(reply / row["jsonFile"])
        return value
    consumer = target_document(selected[0])
    require(consumer.get("type") == "EXECUTABLE" and consumer.get("link", {}).get("language") == "CXX" and
            len(consumer.get("compileGroups", [])) == 1, "complete actual CXX consumer contract required")
    group = consumer["compileGroups"][0]
    for fragment in group.get("compileCommandFragments", []):
        safe_fp(shlex.split(fragment["fragment"]))
    compiler_inputs = []
    for item in group.get("includes", []):
        include = path(item["path"], True)
        require(inside(include, roots), "LLVM consumer include escapes selected package roots")
        compiler_inputs += ["-isystem", str(include)]
    for item in group.get("defines", []):
        require(type(item["define"]) is str and "\n" not in item["define"] and "UWVM" not in item["define"],
                "consumer cannot override Runtime/backend definitions")
        compiler_inputs.append("-D" + item["define"])
    fragments = consumer["link"].get("commandFragments")
    require(type(fragments) is list and fragments, "actual ordered consumer link fragments missing")
    tokens = []
    for fragment in fragments:
        if fragment.get("role") == "flags" and "backtrace" not in fragment:
            continue
        arguments = shlex.split(fragment["fragment"])
        safe_fp(arguments)
        for token in arguments:
            require('print-archive-stats' not in token.lower(),
                    'CMake response cannot replace the owned archive-statistics output')
            require(not token.startswith(("@", "-lLLVM", "-l:libLLVM")), "LLVM search/nested-response substitution refused")
            if not token.startswith("-") and (token.endswith((".a", ".so", ".lib")) or ".so." in token):
                candidate = Path(token)
                token = str((candidate if candidate.is_absolute() else build / candidate).resolve(strict=True))
            tokens.append(token)
    response = path(llvm["consumer_response_file"])
    data, response_pin = common.body_pin(response)
    require(response_pin["sha256"] == package.get("consumer_link_rsp_sha256") and
            shlex.split(data.decode()) == tokens, "response must equal ordered actual CMake consumer fragments")
    listed = package.get("archives")
    require(type(listed) is list and listed, "complete reviewed package archive inventory required")
    expected = {str(path(item["path"])): item for item in listed}
    require(len(expected) == len(listed), "duplicate archive identities in package")
    archives, libraries, member_counts, seen = [], [], {}, set()
    for token in tokens:
        if token.startswith("-"):
            continue
        item = path(token)
        require(inside(item, roots), "consumer library escapes complete package roots")
        is_llvm = item.name.startswith(("libLLVM", "LLVM"))
        require(not is_llvm or (item.suffix == ".a" and inside(item, [build])),
                "current contract permits only actual static LLVM archives")
        if item.suffix == ".a":
            if str(item) in seen:
                continue  # Keep repeats in tokens; pin/check each archive once.
            seen.add(str(item))
            pin, count = archive(common, item, config["target"])
            require(str(item) in expected and expected[str(item)].get("sha256") == pin["sha256"] and
                    expected[str(item)].get("size", expected[str(item)].get("bytes")) == pin["bytes"],
                    "LLVM archive differs from package certificate")
            if is_llvm:
                target_name = item.name[3:-2] if item.name.startswith("lib") else item.stem
                rows = [row for row in targets if row["name"] == target_name]
                require(len(rows) == 1, "static archive has no exact actual CMake target")
                actual_target = target_document(rows[0])
                require(actual_target.get("type") == "STATIC_LIBRARY" and any(
                    (build / value["path"]).resolve() == item for value in actual_target.get("artifacts", [])),
                    "LLVM archive must be the actual STATIC_LIBRARY target artifact")
            archives.append(remember(item)); member_counts[str(item)] = count
        else:
            libraries.append(remember(item))
    require(seen == set(expected) and seen, "consumer/package archive set differs")
    required_native = {"LLVM" + native_arch + "AsmParser", "LLVM" + native_arch + "Disassembler", "LLVMDebugInfoDWARF"}
    require(required_native <= {Path(item).name.removeprefix("lib").removesuffix(".a") for item in seen},
            "actual native parser/disassembler/DWARF component closure missing")
    version, _ = common.document(build / "uwvm-llvm-version.json")
    require(version == {"version": required_version, "host_target": config["target"]["triple"]},
            "actual configured LLVM version/host metadata differs")
    configs = [remember(item) for item in configurations] + [remember(package_path), lock_pin, contract_pin,
                external_lock, remember(build / "CMakeCache.txt"), remember(build / "uwvm-llvm-version.json"),
                remember(configured_contract)]
    return ({"headers": [], "libraries": libraries, "archives": archives,
             "response_files": [remember(response)], "configuration": configs}, ["@" + str(response)],
            {"version": required_version, "package": package_pin, "native_archive_members": member_counts,
             "source_sdk_files_verified": len(names), "file_api_consumer": remember(reply / selected[0]["jsonFile"]),
             "scope": "CURRENT_PATCHED_STATIC_SOURCE_CONTRACT_AND_ACTUAL_LINK_INPUTS_NOT_ROS_BEHAVIOR"}, compiler_inputs)


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--config", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    require(sys.platform == "linux", "Linux native pidfd/cgroup builder required")
    source = args.source.resolve(strict=True)
    require(Path(__file__).resolve() == source / "test/0073.ros-tests/support/source/ros_debugger/build_runtime.py",
            "execute the builder from the complete frozen source, never a mutable workspace")
    manifest = args.manifest.resolve(strict=True)
    common, helper_sha = load_common(source, manifest)
    common.install_interrupts()
    config_path = args.config.resolve(strict=True)
    config, config_pin = common.document(config_path)
    adapter = load_execution_adapter(common, source, manifest) if config.get("format_version") == 2 else None
    _roots_by_kind, roots = configuration(config, source, adapter)
    out = args.out.resolve()
    require(not inside(out, [source] + roots), "outputs cannot mutate source or selected packages")
    out.mkdir(parents=True, mode=0o700, exist_ok=False)
    state = {"format_version": 1, "kind": "ros-debugger-runtime-build-evidence", "status": "FAIL",
             "source": str(source), "config": config_pin, "parallelism": 1, "limits": WORKER_LIMITS,
             "no_reuse": True, "reused_products": 0, "runtime_integration_executed": False,
             "profiles": list(PROFILES), "commands": [], "cleanup": {"status": "NOT_PERFORMED"}}
    runner = None
    before = None
    inputs = {}
    products = {}
    profile_reports = []
    snapshots = []

    def remember(item):
        pin = common.opened(Path(item), CONSUMER_FILE_MAX)
        previous = inputs.get(pin["path"])
        require(previous is None or previous == pin, "input changed during build: " + pin["path"])
        inputs[pin["path"]] = pin
        return pin

    def guard():
        current = common.cgroup_state(config["cgroup"])
        if before is not None:
            common.cgroup_preserved(before, current)
        return current

    try:
        before = guard(); state["cgroup_before"] = before
        state["source_before"] = common.source_state(source, manifest)
        state["source_manifest_sha256"] = state["source_before"]["manifest"]["sha256"]
        require(remember(source / "tools/ros_distro/common.py")["sha256"] == helper_sha,
                "executed helper differs from frozen source")
        for item in (Path(__file__).resolve(), manifest, config_path):
            remember(item)
        for index, root in enumerate(roots):
            guard()
            snapshots.append(common.catalog(root, out / ("inputs-before-%03d.jsonl" % index), roots))
        state["input_roots_before"] = snapshots
        compiler = remember(config["compiler"][0]); remember(path(config["linker"]))
        tool_target = config["target"] if adapter is None else adapter.host_target()
        elf(common, config["compiler"][0], tool_target)
        elf(common, path(config["linker"]), tool_target)
        execution = None
        if adapter is not None:
            remember(source / "test/0073.ros-tests/support/source/ros_debugger/target_execution.py")
            execution = adapter.qualify(config, roots, remember)
            state["runtime_execution"] = execution
        llvm, llvm_argv, state["llvm_package"], llvm_compile = llvm_inputs(common, config, source, roots, remember, adapter)
        libraries = []
        for item in config["library_inputs"]:
            if Path(item).suffix == ".a":
                archive(common, item, config["target"])
            else:
                elf(common, item, config["target"], (3,))
            libraries.append(remember(item))
        runner = common.SerialRunner(out, WORKER_LIMITS, guard)
        base = ["--driver-mode=g++", "-std=c++26", "-O0", "-g0", "-pthread", "-fno-rtti", "-fasynchronous-unwind-tables",
                "-Wno-undefined-inline", "-fconstexpr-steps=8388608", "--target=" + config["target"]["triple"],
                "-DUWVM=2", "-DUWVM_USE_LLVM_JIT", "-DUWVM_DISABLE_INT", "-DUWVM_USE_THREAD_LOCAL",
                "-DUWVM_RUNTIME_LLVM_JIT_CACHE_USE_OPENSSL_ED25519",
                "-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64", "-DUWVM_ENABLE_WASIP1_SOCKET",
                "-DUWVM_VERSION_X=2", "-DUWVM_VERSION_Y=0", "-DUWVM_VERSION_Z=4", "-DUWVM_VERSION_S=0",
                '-DUWVM2_BUILD_SOURCE_ID=u8"' + state["source_manifest_sha256"] + '"']
        if config["sysroot"]:
            base.append("--sysroot=" + config["sysroot"])
        include_paths = [Path(config["llvm"]["generated_include"]), Path(config["llvm"]["public_include"])] + [
            source / item for item in ("src", "third-parties/fast_io/include", "third-parties/bizwen/include",
                                      "third-parties/boost_unordered/include")] + [Path(item) for item in config["include_directories"]]
        for include in include_paths:
            base.extend(["-I", str(include)])
        for profile in PROFILES:
            destination = out / profile
            destination.mkdir(mode=0o700)
            commands, records, objects = [], [], []
            flags = base + config["compile_flags"] + llvm_compile + config["profile_compile_flags"][profile]
            flags += ["-DUWVM_IMPORT_ROS" + str(v) for v in ((1,) if profile == "ros1" else (2,) if profile == "ros2" else (1, 2))]
            for unit, relative in UNITS:
                obj = destination / (unit + ".o"); dep = destination / (unit + ".d")
                argv = config["compiler"] + flags + ["-MD", "-MF", str(dep), "-MT", str(obj),
                                                      "-c", str(source / relative), "-o", str(obj)]
                row = runner.run(profile + "-" + unit, argv, destination, config["environment"])
                commands.append({"role": "compile", "argv": argv, "cwd": str(destination), "returncode": row["returncode"],
                                 "stdout": row["log"], "stderr": row["log"]})
                dep_pin, deps = dependency_inputs(common, dep, obj, source, roots)
                require(str((source / relative).resolve()) in {item["path"] for item in deps},
                        "full-MD dependencies omit the actual translation unit")
                for item in deps:
                    remember(item["path"])
                obj_pin = elf(common, obj, config["target"], (1,))
                products[obj_pin["path"]] = obj_pin; products[dep_pin["path"]] = dep_pin
                objects.append(obj_pin)
                records.append({"command_index": len(commands) - 1, "source": remember(source / relative),
                                "object": obj_pin, "dependency_file": dep_pin, "dependencies": deps})
            binary = destination / "uwvm2-ros"
            statistics_path = destination / "archive-statistics.tsv"
            require(not statistics_path.exists() and not statistics_path.is_symlink(),
                    'fresh owned linker archive-statistics output required')
            argv = config["compiler"] + ["--driver-mode=g++", "--target=" + config["target"]["triple"], "-fuse-ld=" + config["linker"],
                "-pthread", "-Wl,--threads=1", "-Wl,-t", "-Wl,--no-undefined",
                "-Wl,--print-archive-stats=" + str(statistics_path)]
            if config["sysroot"]:
                argv.append("--sysroot=" + config["sysroot"])
            argv += config["link_flags"] + [item["path"] for item in objects] + llvm_argv + config["library_inputs"] + ["-ldl", "-o", str(binary)]
            row = runner.run(profile + "-link", argv, destination, config["environment"])
            commands.append({"role": "link", "argv": argv, "cwd": str(destination), "returncode": row["returncode"],
                             "stdout": row["log"], "stderr": row["log"]})
            trace, _ = common.body_pin(Path(row["log"]["path"]), LIMITS["log_bytes"])
            linked = {}
            for line in trace.decode(errors="strict").splitlines():
                if not line.startswith("/"):
                    continue
                item = Path(line.split("(", 1)[0]).resolve(strict=True)
                require(item.is_file() and (inside(item, roots) or inside(item, [destination])),
                        "link trace names an unqualified external input: " + str(item))
                if item.name.lower().startswith(("libllvm", "llvm")):
                    require(item.suffix == ".a" and str(item) in {entry["path"] for entry in llvm["archives"]},
                            "actual linker trace introduced an extra/shared LLVM provider")
                pin = common.opened(item)
                linked[str(item)] = pin
                if not inside(item, [destination]):
                    remember(item)
            required_link = {item['path'] for item in objects + libraries + llvm['libraries']
                             if Path(item['path']).suffix != '.a'}
            require(required_link <= linked.keys(), 'actual linker trace omits explicit objects/shared libraries')
            response_body, _ = common.body_pin(Path(llvm['response_files'][0]['path']))
            offered_archives = [token for token in shlex.split(response_body.decode())
                                if not token.startswith('-') and Path(token).suffix == '.a']
            offered_archives += [item['path'] for item in libraries if Path(item['path']).suffix == '.a']
            archive_statistics = llvm_archive_statistics(common, statistics_path, archives=llvm['archives'],
                member_counts=state['llvm_package']['native_archive_members'], trace_paths=linked,
                expected_archive_operands=offered_archives, roots=roots, target=config['target'], guard=guard)
            products[archive_statistics['statistics']['path']] = archive_statistics['statistics']
            for entry in archive_statistics['archives']:
                require(remember(entry['archive']['path']) == entry['archive'], 'archive-statistics actual input changed')
            binary_pin = elf(common, binary, config["target"])
            if adapter is not None:
                adapter.check_image(binary, config["target"], execution, lambda item: common.opened(Path(item), CONSUMER_FILE_MAX))
            require(os.access(binary, os.X_OK), "fresh Worker is not executable")
            products[binary_pin["path"]] = binary_pin
            header_pins = {item["path"]: item for record in records for item in record["dependencies"]
                           if inside(Path(item["path"]), [Path(config["llvm"]["public_include"]), Path(config["llvm"]["generated_include"])])}
            require(header_pins, "actual compiles did not consume real LLVM headers")
            profile_llvm = dict(llvm, compiler=compiler, headers=list(header_pins.values()))
            profile_reports.append({"format_version": 1, "kind": "ros-debugger-runtime-build", "status": "PASS",
                "source": str(source), "source_manifest_sha256": state["source_manifest_sha256"], "profile": profile,
                "runtime_backend": "llvm-full", "binary": binary_pin,
                "elf": {key: config["target"][key] for key in ("machine", "bits", "endian")},
                "input_pins": [], "commands": commands, "compile_records": records,
                "link": {"command_index": len(commands) - 1, "objects": objects,
                         "explicit_inputs": list(linked.values()), "response_files": llvm["response_files"],
                         "archive_statistics": archive_statistics['statistics'],
                         "llvm_archive_statistics": archive_statistics},
                "llvm_closure": profile_llvm, "full_dependencies_verified": True,
                "source_preserved": False, "inputs_preserved": False, "products_preserved": False,
                "no_reuse": True, "reused_products": 0, "compilation_executed": True, "runtime_integration_executed": False})
            if adapter is not None:
                profile_reports[-1].update(format_version=2, build_configuration=config_pin, runtime_execution=execution)
        require(len(profile_reports) == 3, "all three independent profiles are required")
        state["compilation_status"] = "PASS"
    except BaseException as error:
        state["error"] = repr(error)
        if runner is None or not runner.commands:
            state["status"] = "BLOCKED"
    finally:
        if runner is not None:
            try:
                state["cleanup"] = runner.close()
            except BaseException as error:
                state["cleanup"] = {"status": "CLEANUP_INCOMPLETE", "remaining": [], "error": repr(error)}
            state["commands"] = runner.commands
        else:
            state["cleanup"] = {"status": "PASS", "remaining": [], "scope": "NO_ACTOR_STARTED"}
        try:
            require(state["cleanup"]["status"] == "PASS" and
                    not state["cleanup"]["remaining"], "owned compiler cleanup incomplete")
            for command in state["commands"]:
                if "log" in command:
                    pin = command["log"]
                    require(common.opened(Path(pin["path"])) == pin, "original raw command log changed")
                    products[pin["path"]] = pin
            if runner is not None:
                ledger_pin = common.opened(out / "commands.json")
                products[ledger_pin["path"]] = ledger_pin
            state["source_after"] = common.source_state(source, manifest)
            require(state["source_after"] == state["source_before"], "complete frozen source changed")
            after_catalogs = []
            for index, old in enumerate(snapshots):
                guard()
                root = roots[index]
                after = common.catalog(root, out / ("inputs-after-%03d.jsonl" % index), roots)
                require({key: value for key, value in old.items() if key != "catalog"} ==
                        {key: value for key, value in after.items() if key != "catalog"}, "complete package root changed")
                after_catalogs.append(after)
            state["input_roots_after"] = after_catalogs
            for snapshot in snapshots + after_catalogs:
                require(snapshot["catalog"]["bytes"] <= CONSUMER_FILE_MAX and
                        common.opened(Path(snapshot["catalog"]["path"]), CONSUMER_FILE_MAX) == snapshot["catalog"],
                        "input catalog changed or exceeds the consumer's fixed file bound")
            for pin in list(inputs.values()) + list(products.values()):
                require(common.opened(Path(pin["path"])) == pin, "physical input/product changed before finalization")
            if adapter is not None:
                require(adapter.qualify(config, roots, remember) == execution,
                        "foreign QEMU/loader/sysroot selection changed during compilation")
            state["cgroup_after"] = guard()
            state["source_preserved"] = state["inputs_preserved"] = state["products_preserved"] = True
            if state.get("compilation_status") == "PASS" and "error" not in state:
                state["status"] = "PASS"
        except BaseException as error:
            state["finalization_error"] = repr(error)
            if state["status"] != "BLOCKED" or state["cleanup"]["status"] != "PASS":
                state["status"] = "FAIL"
        state["scope"] = ("FRESH_NATIVE_LLVM_FULL_COMPILE_ONLY_NO_RUNTIME_OR_ROS_DELIVERY_ACCEPTANCE" if adapter is None else
                          "FRESH_FOREIGN_LLVM_FULL_COMPILE_ONLY_NO_QEMU_RUNTIME_OR_DEBUGGER_ACCEPTANCE")
        state["input_pins"] = list(inputs.values())
        state["products"] = list(products.values())
        common.atomic_json(out / "build-evidence.json", state)
        evidence_pin = common.opened(out / "build-evidence.json")
        if state["status"] == "PASS":
            catalog_pins = [item["catalog"] for item in snapshots + state["input_roots_after"]]
            for report in profile_reports:
                report.update(source_preserved=True, inputs_preserved=True, products_preserved=True,
                              input_pins=list(inputs.values()) + catalog_pins + [evidence_pin])
                if adapter is not None:
                    report["build_evidence"] = evidence_pin
                common.atomic_json(out / report["profile"] / "report.json", report)
    print(json.dumps({"status": state["status"], "evidence": str(out / "build-evidence.json"),
                      "profiles": [str(out / profile / "report.json") for profile in PROFILES]
                      if state["status"] == "PASS" else []}), flush=True)
    return 0 if state["status"] == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
