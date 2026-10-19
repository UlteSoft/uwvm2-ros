"""Closed explicit Runtime/native architecture selections; no execution on import.

Runtime and native ROS architectures are independent. The supplied native SDK
can be x86_64 or AArch64 while the actual interpreter is another selected ELF.
No distribution label supplies a library, permission or successful test result.
"""
from pathlib import Path
import re

ARCHES = {"x86_64": (62, 64, "little"), "aarch64": (183, 64, "little"),
          "arm": (40, 32, "little"), "i386": (3, 32, "little"),
          "riscv64": (243, 64, "little"), "loongarch64": (258, 64, "little"),
          "ppc64le": (21, 64, "little"), "s390x": (22, 64, "big")}
LANGUAGES = ("c", "cpp", "rust", "tinygo", "assemblyscript", "javascript")
SDK_CASES = tuple("sdk-"+language+"-"+mode for language in LANGUAGES for mode in ("positive", "absent"))
CASES = (*SDK_CASES, "seven-events")


def require(ok, why):
    if not ok:
        raise ValueError(why)


def exact(value, fields, label):
    require(type(value) is dict and set(value) == set(fields), "closed "+label+" fields required")


def path(value):
    require(type(value) is str and Path(value).is_absolute() and
            not any(c in value for c in ("\0", "\n", "\r", ";")), "explicit safe absolute path required")


def tokens(value):
    require(type(value) is list and len(value) <= 128 and all(type(x) is str and x and
            not any(c in x for c in ("\0", "\n", "\r")) for x in value), "bounded explicit argv required")


def execution(value, architecture):
    exact(value, ("mode", "qemu", "sysroot", "loader", "environment"), "execution")
    require(value["mode"] in ("native", "qemu"), "execution must be native or explicit user-QEMU")
    if value["mode"] == "qemu":
        for key in ("qemu", "sysroot", "loader"):
            path(value[key])
    else:
        require(value["qemu"] is None, "native execution cannot select an implicit emulator")
        for key in ("sysroot", "loader"):
            if value[key] is not None:
                path(value[key])
        require(architecture == "x86_64", "this lane's native host must be actual ELF62; use QEMU otherwise")
    require(type(value["environment"]) is dict and set(value["environment"]) <=
            {"LD_LIBRARY_PATH", "AMENT_PREFIX_PATH", "CMAKE_PREFIX_PATH", "PYTHONPATH", "ROS_PACKAGE_PATH"},
            "only explicit selected library/package paths allowed")
    for item in value["environment"].values():
        require(type(item) is str and item and "\0" not in item and "\n" not in item,
                "nonempty explicit environment value required")
        for component in item.split(":"):
            path(component)


def validate(value):
    exact(value, ("schema_version", "id", "backend", "runtime", "native", "compiler", "cmake",
                  "make", "compile_flags", "link_flags", "compiler_environment", "native_cmake_flags",
                  "roots", "source_lock", "cgroup", "limits", "cases", "guests", "wat2wasm", "wasm_validate",
                  "node", "master"), "multiarch profile")
    require(value["schema_version"] == 1 and type(value["backend"]) is int and value["backend"] in (1, 2),
            "exact schema1/backend required")
    require(type(value["id"]) is str and re.fullmatch(r"[a-z0-9][a-z0-9_-]{0,63}", value["id"]), "safe profile id required")
    for name in ("runtime", "native"):
        item = value[name]
        exact(item, ("architecture", "execution"), name)
        require(item["architecture"] in ARCHES, "unknown target ELF identity")
        execution(item["execution"], item["architecture"])
    require(value["native"]["architecture"] in ("x86_64", "aarch64"),
            "native ROS fixture is independently qualified host62 or AArch64183, never relabelled as Runtime arch")
    for key in ("compiler", "cmake", "make", "source_lock", "guests", "wat2wasm", "wasm_validate", "node"):
        path(value[key])
    for key in ("compile_flags", "link_flags", "native_cmake_flags"):
        tokens(value[key])
    require(not any(x in ("-DNDEBUG", "-MMD") for x in value["compile_flags"]), "assertions/full system MD must remain enabled")
    require(type(value["compiler_environment"]) is dict and set(value["compiler_environment"]) <= {"LD_LIBRARY_PATH"},
            "compiler-only explicit loader selection required")
    for item in value["compiler_environment"].values():
        for component in item.split(":"):
            path(component)
    require(type(value["roots"]) is list and 1 <= len(value["roots"]) <= 32 and
            len(set(value["roots"])) == len(value["roots"]), "whole SDK/tool roots must be explicit and distinct")
    for root in value["roots"]:
        path(root)
    require(type(value["cases"]) is list and value["cases"] and len(set(value["cases"])) == len(value["cases"])
            and set(value["cases"]) <= set(CASES), "closed unique case selection required")
    require(all("sdk-"+language+"-positive" in value["cases"] and "sdk-"+language+"-absent" in value["cases"]
                for language in LANGUAGES) and "seven-events" in value["cases"],
            "each profile requires all six actual language positive/empty-grant pairs plus seven events")
    exact(value["limits"], ("jobs", "compile_seconds", "actor_seconds", "compiler_rss", "worker_rss", "native_rss", "log_bytes"), "limits")
    limit = value["limits"]
    require(limit["jobs"] == 1 and all(type(limit[k]) is int and limit[k] > 0 for k in limit), "serial positive finite limits required")
    require(limit["compile_seconds"] <= 3600 and limit["actor_seconds"] <= 300 and
            limit["compiler_rss"] <= 8*1024**3 and limit["worker_rss"] <= 8*1024**3 and
            limit["native_rss"] <= 2*1024**3 and limit["log_bytes"] <= 16*1024**2, "leaf bounds exceed maintained lane")
    exact(value["cgroup"], ("memory_max", "memory_swap_max", "expected_boot_id", "expected_leaf", "expected_device", "expected_inode", "expected_cpus"), "cgroup")
    require(value["cgroup"]["memory_max"] == 68719476736 and value["cgroup"]["memory_swap_max"] == 0, "current64GiB/swap0 required")
    path(value["cgroup"]["expected_leaf"])
    master = value["master"]
    require((value["backend"] == 2 and master is None) or (value["backend"] == 1 and type(master) is dict), "ROS1 needs an explicit owned Master")
    if master is not None:
        exact(master, ("python", "script"), "Master")
        path(master["python"]);path(master["script"])
    return value


def available(value):
    """Missing user provision is a BLOCKED prerequisite, never a skipped PASS."""
    names=[value[k] for k in ("compiler","cmake","make","source_lock","guests","wat2wasm","wasm_validate","node")]
    names+=value["roots"]
    for name in ("runtime","native"):
        selection=value[name]["execution"]
        names += [selection[k] for k in ("qemu","sysroot","loader") if selection[k] is not None]
    if value["master"]:
        names+=list(value["master"].values())
    for name in names:
        Path(name).resolve(strict=True)
