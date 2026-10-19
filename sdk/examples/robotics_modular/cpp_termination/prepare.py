#!/usr/bin/env python3
"""Prepare one explicit SDK33 C++ termination-object command, without running it.

This is an offline metadata tool. It downloads nothing, changes no SDK and
never imports a compiler/helper, invokes a shell, or starts a build. The user
must authenticate the SDK distribution independently, provide the actual tool
byte pin, and execute the emitted argv under the serial limits in recipes.json.
A generated command or matching VERSION is not evidence of compiler execution,
complete dependency closure, correct Wasm imports, Runtime behavior or ROS IO.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import stat
import sys

sys.dont_write_bytecode = True
LOCK_SHA256 = '5b0bf5b8cc448ea2806dfdffd360734f9d82fc126c7d1b08bd80a653f97e59d9'
COMMIT = '4434dabb69916856b824f68a64b029c67175e532'
POLICY = 'OFFICIAL_LIBCXXABI_BAREMETAL_ABORT_TU_AND_HEADER_VERBOSE_TRAP_V1'
TRAP = '-D_LIBCPP_VERBOSE_ABORT(...)=__builtin_trap()'
SOURCE = 'libcxxabi/src/abort_message.cpp'
FILES = {SOURCE, 'libcxxabi/src/abort_message.h', 'libcxxabi/include/cxxabi.h',
         'libcxxabi/include/__cxxabi_config.h', 'LICENSE.TXT'}


def require(value, message):
    if not value:
        raise ValueError(message)


def physical_directory(path):
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path and path.is_dir() and
            path != Path('/'), 'explicit ordinary physical input directory required')
    return path


def opened(path, maximum, retain=False):
    """Stream a regular file, bound its size before allocation, retain full facts.

    Compiler bytes are hashed in bounded chunks; only small declared metadata
    and source bodies are retained. Follow neither a final symlink nor a FIFO.
    Alias admission below separately records a selected compiler invocation.
    """
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute input file required')
    descriptor = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        first = os.fstat(descriptor)
        require(stat.S_ISREG(first.st_mode) and 0 < first.st_size <= maximum, 'bounded ordinary input required')
        digest, size, body = hashlib.sha256(), 0, bytearray()
        while True:
            block = os.read(descriptor, min(65536, maximum + 1 - size))
            if not block:
                break
            size += len(block)
            require(size <= maximum, 'input grew beyond its declared bound')
            digest.update(block)
            if retain:
                body.extend(block)
        last, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        require(size == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
                for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'opened input changed')
        return bytes(body), {'path': str(path), 'device': first.st_dev, 'inode': first.st_ino,
            'bytes': size, 'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns,
            'sha256': digest.hexdigest()}
    finally:
        os.close(descriptor)


def pairs(rows):
    value = {}
    for key, item in rows:
        require(key not in value, 'duplicate input member')
        value[key] = item
    return value


def alias(path):
    """Preserve clang++ invocation spelling while pinning its physical bytes."""
    path = Path(path)
    require(path.is_absolute() and path.parent.resolve(strict=True) == path.parent and
            re.fullmatch(r'clang(?:\+\+)?(?:-[0-9.]+)?', path.name), 'explicit Clang invocation required')
    value = path.lstat()
    require(stat.S_ISREG(value.st_mode) or stat.S_ISLNK(value.st_mode), 'ordinary compiler invocation required')
    resolved = path.resolve(strict=True)
    return {'invocation': str(path), 'physical': str(resolved), 'device': value.st_dev,
        'inode': value.st_ino, 'mode': value.st_mode, 'mtime_ns': value.st_mtime_ns,
        'ctime_ns': value.st_ctime_ns, 'target': os.readlink(path) if path.is_symlink() else None}


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('sdk-root', 'official-root', 'compiler', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--compiler-sha256', required=True)
    args = parser.parse_args()
    require(re.fullmatch(r'[0-9a-f]{64}', args.compiler_sha256), 'operator-provided actual compiler SHA256 required')
    sdk, upstream = physical_directory(args.sdk_root), physical_directory(args.official_root)
    own = Path(__file__).resolve(strict=True)
    lock_path = own.parent / 'official-inputs.json'
    _, own_pin = opened(own, 65536)
    lock_body, lock_pin = opened(lock_path, 16384, True)
    require(lock_pin['sha256'] == LOCK_SHA256, 'reviewed portable official-input lock differs')
    lock = json.loads(lock_body, object_pairs_hook=pairs,
                      parse_constant=lambda _: require(False, 'nonfinite official-input metadata'))
    require(lock['format_version'] == 1 and lock['commit'] == COMMIT and lock['policy'] == POLICY and
            lock['repository'] == 'https://github.com/llvm/llvm-project' and
            len(lock['files']) == 5 and {row['path'] for row in lock['files']} == FILES,
            'exact SDK33 official input selection required')
    invocation = alias(args.compiler)
    compiler = Path(invocation['physical'])
    require(compiler.is_relative_to(sdk) and Path(invocation['invocation']).is_relative_to(sdk) and
            os.access(compiler, os.X_OK), 'selected compiler must belong to the admitted SDK')
    _, compiler_pin = opened(compiler, 512 * 1024**2)
    require(compiler_pin['sha256'] == args.compiler_sha256, 'actual selected compiler bytes differ')
    version, version_pin = opened(sdk / 'VERSION', 4096, True)
    require({key: version_pin[key] for key in ('bytes', 'sha256')} == lock['sdk_version'] and
            b'llvm: 4434dabb6991\n' in version and b'llvm-version: 22.1.0\n' in version,
            'selected SDK VERSION must match the actual supported source revision')
    sysroot = physical_directory(sdk / 'share/wasi-sysroot')
    selected, bodies = [], {}
    for row in lock['files']:
        relative = Path(row['path'])
        require(not relative.is_absolute() and '..' not in relative.parts, 'fixed safe upstream path required')
        body, pin = opened(upstream / relative, 128 * 1024, True)
        require(pin['bytes'] == row['bytes'] and pin['sha256'] == row['sha256'] and
                hashlib.sha1(b'blob ' + str(len(body)).encode() + b'\0' + body,
                             usedforsecurity=False).hexdigest() == row['git_blob_sha1'],
                'actual official source/license bytes differ from reviewed Git blob')
        selected.append(pin)
        bodies[row['path']] = body
    # The exact unchanged TU contains no assertion call. NDEBUG selects only
    # its official diagnostic-stdio guard; its final abort remains unconditional.
    # No other TU may inherit these two private compilation defines.
    text = bodies[SOURCE].decode('utf-8', 'strict')
    require(text.count('#if !defined(NDEBUG) || !defined(LIBCXXABI_BAREMETAL)') == 1 and
            len(re.findall(r'\bNDEBUG\b', text)) == 1 and
            not re.search(r'\b(?:assert|static_assert|_LIBCXXABI_ASSERT|_LIBCPP_ASSERT\w*)\s*\(', text) and
            len(re.findall(r'\babort\s*\(\s*\)\s*;', text)) == 1 and
            re.search(r'\babort\s*\(\s*\)\s*;\s*}\s*$', text),
            'official TU assertion/final-abort structure differs')
    out = args.out
    require(out.is_absolute() and out.parent.resolve(strict=True) == out.parent and
            not out.exists() and not out.is_symlink() and
            all(not out.is_relative_to(root) and not root.is_relative_to(out)
                for root in (sdk, upstream, own.parent)), 'fresh disjoint physical output required')
    obj, dep = out / 'official-abort-message.o', out / 'official-abort-message.d'
    argv = [invocation['invocation'], '--driver-mode=g++', '--target=wasm32-wasip1', '--sysroot=' + str(sysroot),
        '-std=c++20', '-O1', '-g0', '-fno-builtin', '-fno-exceptions', '-fno-rtti',
        '-DLIBCXXABI_BAREMETAL', '-DNDEBUG', '-I' + str(upstream / 'libcxxabi/include'),
        '-MD', '-MF', str(dep), '-MT', str(obj), '-c', str(upstream / SOURCE), '-o', str(obj)]
    pins = [own_pin, lock_pin, compiler_pin, version_pin, *selected]
    require(alias(args.compiler) == invocation and
            all(opened(Path(pin['path']), max(pin['bytes'], 1))[1] == pin for pin in pins),
            'selected files or compiler alias changed during metadata preparation')
    out.mkdir(mode=0o700)
    for name in ('tmp', 'home', 'cache'):
        (out / name).mkdir(mode=0o700)
    result = {'format_version': 1, 'kind': 'wasi-sdk33-cpp-termination-object-plan-v1',
        'status': 'PREPARED_COMMAND_NOT_EXECUTED', 'policy': POLICY,
        'compiler_invocation': invocation, 'input_pins': pins, 'compiler_executed': False,
        'SDK_modified': False, 'runtime_executed': False, 'native_ROS_qualified': False,
        'whole_input_catalogs_verified': False, 'dependency_closure_verified': False,
        'command': {'argv': argv, 'cwd': str(out), 'expected_returncode': 0,
            'environment': {'PATH': '/nonexistent', 'TMPDIR': str(out / 'tmp'), 'HOME': str(out / 'home'),
                'XDG_CACHE_HOME': str(out / 'cache'), 'LANG': 'C', 'LC_ALL': 'C',
                'OMP_NUM_THREADS': '1', 'OPENBLAS_NUM_THREADS': '1'},
            'rss_max_bytes': 536870912, 'timeout_seconds': 600, 'jobs': 1},
        'object': str(obj), 'dependency_file': str(dep),
        'recipe_substitutions_after_actual_compile_and_qualification': {'CPP_TERMINATION_OBJECT': str(obj)},
        'Guest_compile_flags': ['-UNDEBUG', TRAP],
        'NDEBUG_scope': 'Only the exact official no-assert abort_message.cpp; never a Guest/global build flag',
        'required_before_and_after_execution': ['reopen every actual input pin', 'whole source/SDK/tool root inventories',
            'actual declared cgroup/RSS/deadline/serial/pidfd ownership and cleanup'],
        'required_after_compile': ['successful actual compiler log/exit', 'fresh object full pin',
            'full-MD target/source/all four official compile files and complete SDK/tool closure',
            'source/SDK/tools/objects preserved before/after', 'actual link trace includes the direct object',
            'why-extract and trace exclude default libc++ verbose_abort/libc++abi abort_message archive members',
            'full Wasm validator plus exact existing import/export/memory/start checks'],
        'trust_scope': lock['origin_scope']}
    (out / 'object-plan.json').write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({'plan': str(out / 'object-plan.json'), 'status': result['status']}))


if __name__ == '__main__':
    main()
