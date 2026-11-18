"""Audit the effective compile switches required for accepted timing evidence.

The recorded invocation is authoritative: metadata saying O3 or debug is not
enough. Response files and forwarded compiler options are rejected because this
small auditor cannot determine whether they override an earlier option.
"""
import hashlib
import json
from pathlib import Path
import re


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def compile_policy(argv):
    argv = [str(arg) for arg in argv]
    assert not any(arg.startswith('@') or arg in ('-Xclang', '-Xarch_host', '-Xarch_device')
                   for arg in argv), 'Opaque forwarded compile options require explicit expansion'
    optimization = [arg for arg in argv if arg.startswith('-O')]
    assert not any(arg.startswith('--optimize') for arg in argv), 'Expanded optimization aliases are required'
    assert optimization and optimization[-1] == '-O3', 'Final optimization must be -O3'
    debug = [arg for arg in argv if re.fullmatch(r'-g(?:[0-3]|gdb[0-3]?|line-tables-only|line-directives-only|none|full)?', arg)]
    assert '-g' in debug and debug[-1] in ('-g', '-g2', '-g3', '-ggdb', '-ggdb2', '-ggdb3', '-gfull'), \
        'An explicit -g and effective full debug information are required; line tables alone are insufficient'
    assert not any(arg in ('-s', '--strip-all', '-fno-debug-info') or
                   arg.startswith(('-debug-info-kind=', '-gmlt')) for arg in argv), 'Debug stripping or reduced debug information is forbidden'
    result = dict(optimization_options=optimization, debug_options=debug,
                effective_optimization=optimization[-1], effective_debug=debug[-1],
                explicit_full_debug=True)
    dependency = openssl_policy(argv)
    if dependency: result['openssl'] = dependency
    return result


def openssl_policy(argv):
    """A debug wrapper cannot qualify an O2/g0 cryptographic dependency."""
    assert not any(x in ('-lssl', '-lcrypto') for x in argv), 'Profiling requires exact OpenSSL archive paths'
    archives = [Path(x) for x in argv if Path(x).name in ('libssl.a', 'libcrypto.a')]
    if not archives: return None
    roots = {p.parent.resolve() for p in archives}
    assert len(roots) == 1 and all(p.is_absolute() for p in archives), 'Ambiguous OpenSSL build inputs'
    root = next(iter(roots)); receipt = root/'receipt.json'
    record = json.loads(receipt.read_text())
    flags = record['identity']['spec']['cflags']
    assert not any(Path(x).name in ('libssl.a', 'libcrypto.a') for x in flags), 'Invalid dependency compile specification'
    policy = compile_policy(flags)
    pinned = {}
    for archive in archives:
        expected = record['artifacts'][archive.name]
        assert sha(archive) == expected, 'OpenSSL archive changed after its build receipt'
        pinned[str(archive)] = expected
    return dict(receipt=str(receipt), receipt_sha256=sha(receipt), archives=pinned, compile_policy=policy)


def vm_policy(runtime):
    root = Path(runtime).resolve().parent
    report = {}
    for unit in ('main.default.cpp.o', 'uwvm_runtime.default.cpp.o', 'host_api.default.cpp.o'):
        path = root / (unit + '.argv.json')
        report[unit] = dict(argv_file=str(path), argv_sha256=sha(path),
                            **compile_policy(json.loads(path.read_text())))
    report['openssl'] = openssl_policy(json.loads((root/'link.argv.json').read_text()))
    return report


def fixture_policy(receipt):
    commands = [row for row in receipt['commands'] if row['name'].startswith(('compile-native-', 'compile-wasm-'))]
    assert {row['name'] for row in commands} == {
        'compile-native-baseline', 'compile-native-profile', 'compile-wasm-baseline', 'compile-wasm-profile'}
    return {row['name']: compile_policy(row['argv']) for row in commands}
