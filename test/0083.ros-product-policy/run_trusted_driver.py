#!/usr/bin/env python3
"""Production optional driver verification: local CMS fixtures, no PKI network.

Build only two small components and DSOs. --runtime additionally checks the
public VM CLI, including option order, ROS composition and default host-API
denial. Generated certificates are ephemeral test identities, never trust data.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import shutil
import subprocess


def u(value):
    result = bytearray()
    while value >= 128:
        result.append((value & 127) | 128)
        value >>= 7
    return bytes(result + bytes([value]))


def section(kind, value):
    return bytes([kind]) + u(len(value)) + value


def name(value):
    value = value.encode()
    return u(len(value)) + value


def guest(api=0):
    """No external WAT assembler; call both published fixture functions."""
    imports = b"".join(name("driver") + name(n) + b"\0\0"
                       for n in ("value", "host_api_present"))
    body = b"\0\x10\0\x41\x2a\x47\x04\x40\0\x0b"
    body += b"\x10\x01\x41" + u(api) + b"\x47\x04\x40\0\x0b\x0b"
    return (b"\0asm\x01\0\0\0" + section(1, b"\x02\x60\0\x01\x7f\x60\0\0")
            + section(2, u(2) + imports) + section(3, b"\x01\x01")
            + section(7, b"\x01" + name("_start") + b"\0\x02")
            + section(10, b"\x01" + u(len(body)) + body))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cxx", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--runtime", type=Path)
    parser.add_argument("--engines", nargs="+", default=["int", "aot"])
    parser.add_argument("--verify-only", action="store_true",
                        help="Explicitly omit sealed dlopen proof if this test host has no /proc/self/fd")
    parser.add_argument("--runtime-preflight", action="store_true",
                        help="Check actual CLI default-off/denial/WSEA independence, without claiming sealed loads")
    parser.add_argument("--expect-legacy-driver-api-hidden", action="store_true",
                        help="Before-only evidence: expect the old ROS product to reject the explicit Single API switch")
    args = parser.parse_args()
    if args.verify_only and args.runtime and not args.runtime_preflight:
        parser.error("--verify-only does not qualify actual VM driver loading; omit --runtime")
    if args.runtime_preflight and not args.runtime:
        parser.error("--runtime-preflight requires --runtime")
    repo = Path(__file__).resolve().parents[2]
    source = Path(__file__).resolve().parent
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, TMPDIR=str(out))
    report = {"scope": "native Linux OpenSSL CMS component", "commands": [], "cases": [],
              "sealed_dlopen_tested": not args.verify_only,
              "legacy_driver_api_hidden": args.expect_legacy_driver_api_hidden}
    def command(argv, check=True, extra_env=None):
        argv = list(map(str, argv))
        # The safe product sink must not toggle a caller's blocking pipe flags.
        # A regular-file sink also works on qualification hosts without procfs.
        index = len(report["commands"])
        stdout_path, stderr_path = out / f"command-{index}.stdout", out / f"command-{index}.stderr"
        with stdout_path.open("wb") as output, stderr_path.open("wb") as errors:
            result = subprocess.run(argv, stdout=output, stderr=errors, timeout=90,
                                    env=env if extra_env is None else dict(env, **extra_env))
        result.stdout, result.stderr = stdout_path.read_bytes(), stderr_path.read_bytes()
        report["commands"].append({"argv": argv, "exit": result.returncode,
                                   "stdout": result.stdout.decode(errors="replace"),
                                   "stderr": result.stderr.decode(errors="replace")})
        if check and result.returncode:
            (out / "report.json").write_text(json.dumps(report, indent=2))
            raise RuntimeError(report["commands"][-1])
        return result
    base = [args.cxx, "-std=c++26", "-O2", "-I" + str(repo / "src")]
    command(base + ["-DUWVM_WSEA_OPENSSL", source / "trusted_driver.cc", "-lcrypto", "-ldl", "-o", out / "verify"])
    command(base + [source / "trusted_driver.cc", "-ldl", "-o", out / "unavailable"])
    for suffix, flags in [("good", []), ("other", ["-DDRIVER_VALUE=99", "-DDRIVER_MARKER=66"])]:
        command([args.cxx, "-x", "c", "-std=c17", "-D_GNU_SOURCE", "-O2", "-fPIC", "-shared", *flags,
                 source / "trusted_driver_fixture.c", "-o", out / (suffix + ".so")])
    for identity in ("approved", "other"):
        command(["openssl", "req", "-new", "-x509", "-newkey", "rsa:2048", "-nodes", "-days", "1",
                 "-subj", "/CN=uwvm-driver-component-" + identity, "-keyout", out / (identity + ".key"),
                 "-out", out / (identity + ".pem")])
    marker = b"~Module signature appended~\n"
    def sign(label, payload, identity="approved", digest="sha256", attached=False, embed=False):
        data = out / (label + ".payload")
        data.write_bytes(payload)
        signature = out / (label + ".cms")
        command(["openssl", "cms", "-sign", "-binary", "-in", data, "-signer", out / (identity + ".pem"),
                 "-inkey", out / (identity + ".key"), "-outform", "DER", "-out", signature, "-md", digest,
                 "-nosmimecap", *([] if embed else ["-nocerts"]), *(["-nodetach"] if attached else [])])
        cms = signature.read_bytes()
        result = payload + cms + b"\0\0\x02" + b"\0" * 5 + len(cms).to_bytes(4, "big") + marker
        path = out / (label + ".so")
        path.write_bytes(result)
        return path
    payload = (out / "good.so").read_bytes()
    good = sign("signed", payload)
    sha512 = sign("sha512", payload, digest="sha512")
    embedded = sign("embedded-approved", payload, embed=True)
    wrong = sign("wrong-signer", payload, identity="other", embed=True)
    weak = sign("sha1", payload, digest="sha1")
    attached = sign("attached", payload, attached=True)
    wrong_arch_bytes = bytearray(payload)
    wrong_arch_bytes[18:20] = b"\xff\xff"
    wrong_arch = sign("wrong-arch", wrong_arch_bytes)
    not_elf = sign("not-elf", b"signed text is not an ELF image\n" * 4)
    invalid = {}
    for label, contents in {
        "unsigned": payload,
        "tampered-payload": bytes([payload[0] ^ 1]) + good.read_bytes()[1:],
        "trailing-junk": good.read_bytes() + b"!",
        "truncated": good.read_bytes()[:-1],
        "trailer-algorithm": good.read_bytes()[:-len(marker)-10] + b"\x01" + good.read_bytes()[-len(marker)-9:],
        "zero-signature": good.read_bytes()[:-len(marker)-4] + b"\0" * 4 + marker,
        "oversize-signature": good.read_bytes()[:-len(marker)-4] + b"\xff" * 4 + marker,
    }.items():
        path = out / (label + ".so")
        path.write_bytes(contents)
        invalid[label] = path
    invalid.update({"wrong-signer": wrong, "sha1": weak, "attached": attached,
                    "wrong-arch": wrong_arch, "not-elf": not_elf,
                    "directory": out, "missing": out / "missing.so"})
    fifo = out / "fifo"
    if fifo.exists(): fifo.unlink()
    os.mkfifo(fifo)
    invalid["fifo"] = fifo
    cert = out / "approved.pem"
    def case(label, mode, path, expected_marker, executable="verify", certificate=cert, extra=()):
        witness = out / (label + ".marker")
        witness.unlink(missing_ok=True)
        command([out / executable, mode, certificate, path, *extra],
                extra_env={"UWVM_DRIVER_MARKER": str(witness)})
        observed = witness.read_bytes() if witness.exists() else b""
        row = {"name": label, "pass": observed == expected_marker,
               "constructor_marker": observed.decode(errors="replace")}
        report["cases"].append(row)
        assert row["pass"], row
    case("default-off-unsigned", "off", out / "good.so", b"A")
    case("backend-unavailable", "unavailable", good, b"", executable="unavailable")
    case("invalid-certificate", "select-reject", good, b"", certificate=out / "good.so")
    for label, path in [("valid", good), ("valid-sha512", sha512), ("valid-embedded", embedded)]:
        case(label, "prepare" if args.verify_only else "verify", path, b"" if args.verify_only else b"A")
    for label, path in invalid.items():
        case(label, "reject", path, b"")
    swapped = out / "replace-source.so"
    replacement = out / "replace-with.so"
    shutil.copyfile(good, swapped)
    shutil.copyfile(out / "other.so", replacement)
    case("sealed-after-path-replacement", "replace-prepare" if args.verify_only else "replace", swapped,
         b"" if args.verify_only else b"A", extra=(replacement,))
    if args.runtime:
        report["runtime"] = str(args.runtime.resolve())
        report["runtime_sha256"] = hashlib.sha256(args.runtime.read_bytes()).hexdigest()
        guest_path = out / "driver.wasm"
        guest_path.write_bytes(guest())
        api_guest = out / "driver-api.wasm"
        api_guest.write_bytes(guest(api=1))
        report["runtime_scope"] = "preflight and unsigned opt-out only" if args.runtime_preflight else "full sealed loading and ROS composition"
        if args.runtime_preflight:
            for engine in args.engines:
                engine_flags = ["--runtime-" + engine]
                if engine == "aot": engine_flags += ["--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable"]
                prefix = [args.runtime, *engine_flags, "--wasm-set-main-module-name", "main"]
                load = ["--wasm-register-dl", out / "good.so", "driver"]
                verify = ["--wasm-driver-verify", cert]
                probes = [
                    ("no-implicit-driver", ["--run", guest_path], False, b"", b"Missing module dependency"),
                    ("default-off-api-hidden", [*load, "--run", guest_path], True, b"A", b""),
                    ("explicit-driver-api", ["--wasip1-single-create", "driver", "--wasip1-single-expose-host-api", "driver",
                                             *load, "--run", api_guest], True, b"A", b""),
                    ("guest-tail-is-not-host-policy", [*load, "--run", guest_path, "--wasm-driver-verify", out / "missing.pem"], True, b"A", b""),
                    ("bad-certificate-before", ["--wasm-driver-verify", out / "missing.pem", *load, "--run", guest_path], False, b"", b"driver verification"),
                    ("bad-certificate-after", [*load, "--wasm-driver-verify", out / "missing.pem", "--run", guest_path], False, b"", b"driver verification"),
                    ("unsigned-selected-before", [*verify, *load, "--run", guest_path], False, b"", b"trusted driver rejected before loading"),
                    ("unsigned-selected-after", [*load, *verify, "--run", guest_path], False, b"", b"trusted driver rejected before loading"),
                    ("wrong-signer-after", ["--wasm-register-dl", wrong, "driver", *verify, "--run", guest_path], False, b"", b"trusted driver rejected before loading"),
                    ("tampered-after", ["--wasm-register-dl", invalid["tampered-payload"], "driver", *verify, "--run", guest_path], False, b"", b"trusted driver rejected before loading"),
                    ("default-wsea-off", ["--secure", "status"], True, b"", b"WSEA: disabled"),
                    ("driver-pin-does-not-enable-wsea", [*verify, "--secure", "status"], True, b"", b"WSEA: disabled"),
                    ("wsea-refusal-before-constructor", [*load, "--secure", "module", guest_path, out / "missing.policy", "--run", guest_path], False, b"", b"DENIED"),
                    ("driver-pin-does-not-bypass-wsea", [*load, *verify, "--secure", "module", guest_path, out / "missing.policy", "--run", guest_path], False, b"", b"DENIED"),
                ]
                for label, argv, success, ctor, diagnostic in probes:
                    if label == "explicit-driver-api" and args.expect_legacy_driver_api_hidden:
                        success, ctor, diagnostic = False, b"", b"invalid parameter: --wasip1-single-expose-host-api"
                    label = "cli-" + label + "-" + engine
                    witness = out / (label + ".marker")
                    witness.unlink(missing_ok=True)
                    result = command([*prefix, *argv], check=False, extra_env={"UWVM_DRIVER_MARKER": str(witness)})
                    observed = witness.read_bytes() if witness.exists() else b""
                    row = {"name": label, "pass": (result.returncode == 0) == success and observed == ctor and diagnostic in result.stderr,
                           "exit": result.returncode, "constructor_marker": observed.decode(errors="replace")}
                    report["cases"].append(row)
                    if not row["pass"]:
                        (out / "report.json").write_text(json.dumps(report, indent=2))
                        raise AssertionError(row)
            report["complete"] = True
            (out / "report.json").write_text(json.dumps(report, indent=2))
            print(json.dumps({"passed": len(report["cases"]), "report": str(out / "report.json")}))
            return
        for engine in args.engines:
            engine_flags = ["--runtime-" + engine]
            if engine == "aot": engine_flags += ["--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable"]
            for ros in (None, "ros1", "ros2"):
                ros_flags = [] if ros is None else ["--" + ros + "-single-create", "main", "--" + ros + "-single-enable", "main"]
                for label, selected, verify_flags, success, ctor in [
                    ("unsigned-default-off", out / "good.so", [], True, b"A"),
                    ("signed-before", good, ["--wasm-driver-verify", cert], True, b"A"),
                    ("signed-after", good, ["--wasm-driver-verify", cert], True, b"A"),
                    ("bad-signature-after", invalid["tampered-payload"], ["--wasm-driver-verify", cert], False, b""),
                    ("unsigned-after", out / "good.so", ["--wasm-driver-verify", cert], False, b""),
                    ("wrong-signer-after", wrong, ["--wasm-driver-verify", cert], False, b"")]:
                    label = (ros or "no-ros") + "-" + label + "-" + engine
                    witness = out / (label + ".marker")
                    witness.unlink(missing_ok=True)
                    load = ["--wasm-register-dl", selected, "driver"]
                    flags = verify_flags + load if "signed-before" in label else load + verify_flags
                    argv = [args.runtime, *engine_flags, "--wasm-set-main-module-name", "main", *ros_flags,
                            *flags, "--run", guest_path]
                    result = command(argv, check=False, extra_env={"UWVM_DRIVER_MARKER": str(witness)})
                    observed = witness.read_bytes() if witness.exists() else b""
                    row = {"name": label, "pass": (result.returncode == 0) == success and observed == ctor,
                           "exit": result.returncode, "constructor_marker": observed.decode(errors="replace")}
                    if not success: row["pass"] &= b"trusted driver rejected before loading" in result.stderr
                    report["cases"].append(row)
                    if not row["pass"]:
                        (out / "report.json").write_text(json.dumps(report, indent=2))
                        raise AssertionError(row)
    report["complete"] = True
    (out / "report.json").write_text(json.dumps(report, indent=2))
    print(json.dumps({"passed": len(report["cases"]), "report": str(out / "report.json")}))


if __name__ == "__main__":
    main()
