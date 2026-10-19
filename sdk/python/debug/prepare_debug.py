#!/usr/bin/env python3
"""Capture non-ROS CPython debug inputs and emit additive argv; execute NOTHING.

This operator itself requires pre-start Linux keeper admission. An externally
reviewed closure is required; JSON, passed fields and self hashes cannot prove
its authority/completeness. The keeper freezes toolchain inputs across every
later read, owns all execution, and retains NEW receipts. No CPython rebuild.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import stat
import sys

MIB = 1024 * 1024
BASE_SHA = '4eb71c6a10490a270b895cf2d66b542ab70fd2511d7dfae5d24e81065c020816'
OZ_SHA = 'f081533714842d25c78c9c4627a7c976b1c1e9484ea1e403ba0cdee65acf68be'
CONFIG_SHA = '1664efaae522119f80d2a5b0a38579482846c976a53b324afabcab7de0d64df3'
SOURCE_PINS = {
    'sdk/python/debug/frame_snapshot.c': '82a958af8ebcb14855a6a15f0ef6fdcbd38f179765b6fabf1ea4d01fe8c7bf18',
    'sdk/python/debug/frame_snapshot.h': 'd5298a312c24aeed1aa10b91878792f22cc34aec40e5e57714be2df1deba3603',
    'sdk/python/debug/non_ros_main.c': '406ae1233eba99d907d7deba46a03b11b2b5fa2d87e2c1062376831c37f33946',
    'sdk/python/debug/snapshot_anchor.c': '6d1bce91d5fafaf798e24b5a5a16d9e038130f12aea9292fd3836cfe91ac91d7',
    'sdk/python/debug/freeze_debug.py': '18e26fba581616297c45f72388ea8aa97531efb6f78c6c579eb278bab1aaeb4b',
    'sdk/python/debug/non_ros_probe.py': 'df7da142740785525435bbc504a8720f79772b418a55a9bc1c5bf3073ed7b834',
    'sdk/debug/include/uwvm_language_snapshot.h': 'bbcc3e867c42205720943bb2fe2f4cbe822175cea32a1bb5a768e9dabace4949',
    'test/0050.high_level_languages/inspect_wasm.py': 'a457050eb7a94a39ffc9ed1a451a22e3a2808c4cb8dcf5fe6ba03862a7ab1dca'}


def need(value, text):
    if not value: raise ValueError(text)


def sha(raw): return hashlib.sha256(raw).hexdigest()
def full_sha(value): return type(value) is str and re.fullmatch('[0-9a-f]{64}', value) is not None


def strict(raw):
    def pairs(rows):
        out = {}
        for key, value in rows:
            need(key not in out, 'duplicate JSON key'); out[key] = value
        return out
    def bad(value): raise ValueError('nonfinite JSON: ' + value)
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=bad)


def directory(value):
    need(type(value) is str, 'directory spelling')
    path = Path(value)
    need(path.is_absolute() and path.resolve(strict=True) == path and path.is_dir(), 'canonical existing directory')
    return path


class Capture:
    def __init__(self): self.rows, self.identities = {}, {}
    def read(self, value, maximum, expected, retain=True):
        need(full_sha(expected), 'full external SHA required')
        path = Path(value)
        need(path.is_absolute() and path.resolve(strict=True) == path, 'canonical input; symlink/path aliases refused')
        fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
        try:
            first = os.fstat(fd)
            keys = ('st_dev', 'st_ino', 'st_mode', 'st_nlink', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
            need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum, 'bounded regular input')
            identity = first.st_dev, first.st_ino
            need(identity not in self.identities or self.identities[identity] == str(path), 'hardlink/path alias refused')
            digest = hashlib.sha256(); blocks = []; size = 0
            while True:
                block = os.read(fd, min(65536, maximum - size + 1))
                if not block: break
                size += len(block); need(size <= maximum and size <= first.st_size, 'input grew')
                digest.update(block)
                if retain: blocks.append(block)
            last, named = os.fstat(fd), os.lstat(path)
            need(size == first.st_size and all(getattr(first, k) == getattr(last, k) == getattr(named, k) for k in keys), 'input drift')
            need(digest.hexdigest() == expected, 'exact admitted SHA differs: ' + str(path))
            row = {'path': str(path), 'bytes': size, 'sha256': expected,
                   'stat': {k: str(getattr(first, k)) for k in keys}}
            need(str(path) not in self.rows or row == self.rows[str(path)], 'capture changed')
            self.identities[identity] = str(path); self.rows[str(path)] = row
            return b''.join(blocks) if retain else None
        finally: os.close(fd)
    def recheck(self):
        for row in tuple(self.rows.values()): self.read(row['path'], row['bytes'], row['sha256'], False)


def prepare(args):
    cap = Capture()
    own = cap.read(Path(__file__).resolve(), 2 * MIB, args.operator_sha256)
    source_raw = cap.read(args.source_closure, 8 * MIB, args.source_closure_sha256)
    closure = strict(source_raw)
    need(type(closure) is dict and set(closure) == {'format', 'files'} and
         closure['format'] == 'python-debug-source-closure-v1' and type(closure['files']) is list and
         10 <= len(closure['files']) <= 64, 'closed complete debug Source closure')
    sources = {}; source_paths = set(); total = 0
    for row in closure['files']:
        need(type(row) is dict and set(row) == {'source', 'target', 'bytes', 'sha256'}, 'Source row schema')
        need(type(row['target']) is str and re.fullmatch(r'(?:sdk|test)/[A-Za-z0-9_./-]+', row['target']) is not None, 'Source target')
        target = Path(row['target'])
        need(not target.is_absolute() and '..' not in target.parts and target.as_posix() == row['target'] and
             row['target'] not in sources and row['source'] not in source_paths, 'unique normalized Source target/path')
        need(type(row['bytes']) is int and 0 <= row['bytes'] <= 4 * MIB, 'Source extent')
        total += row['bytes']; need(total <= 16 * MIB, 'total Source budget')
        raw = cap.read(row['source'], row['bytes'], row['sha256']); need(len(raw) == row['bytes'], 'Source size')
        sources[row['target']] = raw; source_paths.add(row['source'])
    required = set(SOURCE_PINS) | {'sdk/python/debug/prepare_debug.py', 'sdk/python/debug/check_debug_wasm.py'}
    need(set(sources) == required, 'exact debug6 + wire + maintained inspector + two tools; no ROS C dispatcher')
    for name, expected in SOURCE_PINS.items(): need(sha(sources[name]) == expected, 'Source generation changed: ' + name)
    need(sources['sdk/python/debug/prepare_debug.py'] == own, 'operator closure bytes differ')
    entry = cap.read(args.entry, MIB, args.entry_sha256)
    need(entry == sources['sdk/python/debug/non_ros_probe.py'], 'this finite profile freezes the actual reviewed probe')

    admission_raw = cap.read(args.admission, 16 * MIB, args.admission_sha256)
    admission = strict(admission_raw)
    need(type(admission) is dict and set(admission) == {'format', 'files', 'roles', 'directories', 'link_operands', 'closure_evidence', 'link_resolution_evidence'} and
         admission['format'] == 'python-debug-cpython3148-build-admission-v1', 'closed externally reviewed build admission')
    need(type(admission['files']) is list and 16 <= len(admission['files']) <= 32768, 'full bounded native/compiler/headers/sysroot/static closure')
    files = {}; paths = set(); small = {}; total = 0
    for row in admission['files']:
        need(type(row) is dict and set(row) == {'id', 'path', 'bytes', 'sha256'} and type(row['id']) is str and
             re.fullmatch('[a-z0-9][a-z0-9_.-]{0,95}', row['id']) is not None and row['id'] not in files and row['path'] not in paths, 'unique admitted input id/path')
        need(type(row['bytes']) is int and 0 <= row['bytes'] <= 512 * MIB, 'admitted input extent')
        total += row['bytes']; need(total <= 2 * 1024 * MIB, 'full input budget; larger closure requires a separately reviewed profile')
        body = cap.read(row['path'], row['bytes'], row['sha256'], row['bytes'] <= 8 * MIB)
        need(cap.rows[row['path']]['bytes'] == row['bytes'], 'admitted size')
        files[row['id']] = row; paths.add(row['path'])
        if body is not None: small[row['id']] = body
    roles = admission['roles']
    role_names = {'base_receipt', 'oz_receipt', 'native', 'clang', 'pyconfig', 'config_c', 'makefile', 'python_h', 'patchlevel', 'libpython', 'cpython_archive', 'sdk_archive', 'encodings', 'aliases', 'utf8'}
    need(type(roles) is dict and set(roles) == role_names and all(type(v) is str and v in files for v in roles.values()), 'actual complete role binding')
    def data(role): need(roles[role] in small, 'metadata role too large'); return small[roles[role]]
    def file(role): return files[roles[role]]
    need(file('base_receipt')['sha256'] == BASE_SHA and file('oz_receipt')['sha256'] == OZ_SHA, 'exact actual historical receipts required; not a passed boolean')
    base, oz = strict(data('base_receipt')), strict(data('oz_receipt'))
    need(oz['original_receipt_sha256'] == BASE_SHA and oz['config_c_sha256'] == CONFIG_SHA and
         file('config_c')['sha256'] == CONFIG_SHA and file('pyconfig')['sha256'] == oz['configuration_files']['pyconfig.h'], 'actual71-builtin/GIL configuration identities')
    need(file('native')['sha256'] == base['native_python']['sha256'] and file('clang')['sha256'] == base['sdk_clang']['sha256'], 'matching real native3.14.8 and original clang')
    need(file('makefile')['sha256'] == oz['configuration_files']['Makefile'] and
         file('cpython_archive')['sha256'] == oz['source_archive']['sha256'] == 'c2215904f02b175596dc49351585104f4bc20341e1c47378b26a2c274360ce73' and
         file('sdk_archive')['sha256'] == 'c6c38aab56e5de88adf6c1ebc9c3ae8da72f88ec2b656fb024eda8d4167a0bc5', 'actual source/config and observed SDK archive origins')
    dirs = admission['directories']
    need(type(dirs) is dict and set(dirs) == {'cpython_include', 'wasi_build', 'sysroot'}, 'exact immutable directory roles')
    dirs = {k: directory(v) for k, v in dirs.items()}
    need(len(set(dirs.values())) == 3 and Path(file('python_h')['path']) == dirs['cpython_include'] / 'Python.h' and
         Path(file('patchlevel')['path']) == dirs['cpython_include'] / 'patchlevel.h' and
         Path(file('pyconfig')['path']) == dirs['wasi_build'] / 'pyconfig.h' and
         Path(file('config_c')['path']) == dirs['wasi_build'] / 'Modules/config.c' and
         Path(file('makefile')['path']) == dirs['wasi_build'] / 'Makefile' and
         Path(file('libpython')['path']) == dirs['wasi_build'] / 'libpython3.14.a', 'actual header/config/archive directory membership')
    for key in ('closure_evidence', 'link_resolution_evidence'):
        ids = admission[key]
        need(type(ids) is list and 1 <= len(ids) <= 64 and len(ids) == len(set(ids)) and all(type(x) is str and x in files for x in ids), 'independent externally admitted evidence references required')
    # These lists bind bytes of outside review/build evidence. They cannot prove
    # their own independence, full dependency completeness or execution authority.
    selected = [x for x in oz['commands'] if type(x) is dict and type(x.get('argv')) is list and
                x['argv'] and x['argv'][-1] == oz['artifact']['path']]
    need(len(selected) == 1, 'one exact original FullOz link command; no fallback guess')
    old = selected[0]; argv = old['argv']
    need(all(type(x) is str and x and '\0' not in x for x in argv) and argv[0] == base['sdk_clang']['path'] and
         argv[-2] == '-o' and old['cwd'] == oz['wasi_build'], 'original link identity/cwd/output shape')
    mains = [i for i, x in enumerate(argv) if x.endswith('/embedded_main.c')]
    need(len(mains) == 1 and not any(x.endswith('.o') for x in argv), 'one actual original C main and no old objects')
    old_frozen = '-I' + str(Path(oz['frozen_header']['path']).parent)
    old_wasi, old_include = '-I' + oz['wasi_build'], '-I' + str(Path(oz['source']) / 'Include')
    need(all(argv.count(x) == 1 for x in (old_frozen, old_wasi, old_include)) and '-Oz' in argv and '-g' in argv and
         not any('lto' in x.lower() or 'HLL_EXTENSION' in x for x in argv), 'exact original non-LTO link form')
    operands = admission['link_operands']; need(type(operands) is list, 'actual library resolution table')
    resolved = {}; required_indices = {i for i, x in enumerate(argv) if x.startswith('-l') or x.endswith('.a')}
    for row in operands:
        need(type(row) is dict and set(row) == {'index', 'file_id'} and type(row['index']) is int and
             row['index'] in required_indices and row['index'] not in resolved and type(row['file_id']) is str and row['file_id'] in files, 'exact unique original library operand index')
        file_ = files[row['file_id']]; need(file_['path'].endswith('.a'), 'static archive resolution only')
        resolved[row['index']] = file_['path']
    need(set(resolved) == required_indices and len(required_indices) > 1, 'every actual original library operand resolved; missing is HOLD')
    libindex = [i for i in required_indices if argv[i] == str(Path(oz['wasi_build']) / 'libpython3.14.a')]
    need(len(libindex) == 1 and resolved[libindex[0]] == file('libpython')['path'], 'actual retained libpython archive, not a new library list')

    stage = args.stage
    need(stage.is_absolute() and stage.parent.resolve(strict=True) == stage.parent and stage.name not in ('', '.', '..') and
         not stage.exists() and not stage.is_symlink(), 'fresh canonical owned stage')
    for row in cap.rows.values(): need(not Path(row['path']).is_relative_to(stage), 'output/input overlap')
    for path in dirs.values(): need(not stage.is_relative_to(path) and not path.is_relative_to(stage), 'stage outside immutable toolchain directories')
    objects = [stage / (name + '.o') for name in ('frame_snapshot', 'snapshot_anchor', 'non_ros_main')]
    common = [file('clang')['path'], '--sysroot=' + str(dirs['sysroot']), '-std=c17', '-O0', '-g', '-fno-lto', '-I' + str(dirs['wasi_build']), '-I' + str(dirs['cpython_include']), '-I' + str(stage / 'frozen')]
    commands = [{'name': 'freeze-matching-native-marshal', 'cwd': str(stage), 'argv': [file('native')['path'], '-I', '-S', str(stage / 'sdk/python/debug/freeze_debug.py'), '--entry', str(stage / 'entry.py'), '--stdlib', str(stage / 'freeze-stdlib'), '--out', str(stage / 'frozen')]}]
    for name, output in zip(('frame_snapshot', 'snapshot_anchor', 'non_ros_main'), objects):
        commands.append({'name': 'compile-' + name, 'cwd': str(stage), 'argv': common + ['-c', str(stage / ('sdk/python/debug/' + name + '.c')), '-o', str(output)]})
    linked = []
    for i, value in enumerate(argv):
        if i == 0: linked.extend([file('clang')['path'], '--sysroot=' + str(dirs['sysroot'])])
        elif i == mains[0]: linked.extend(str(x) for x in objects)
        elif i in resolved: linked.append(resolved[i])
        elif value == old_frozen: linked.append('-I' + str(stage / 'frozen'))
        elif value == old_wasi: linked.append('-I' + str(dirs['wasi_build']))
        elif value == old_include: linked.append('-I' + str(dirs['cpython_include']))
        elif i == len(argv) - 1: linked.append(str(stage / 'python-debug.wasm'))
        else:
            need('/' not in value and '@' not in value, 'unrecognized file-bearing original operand; HOLD instead of guessing')
            linked.append(value)
    commands.append({'name': 'link-original-ordered-static-closure', 'cwd': str(dirs['wasi_build']), 'argv': linked})
    cap.recheck()  # Capture/recheck every input BEFORE the first stage write.
    stage.mkdir(exist_ok=False)
    staged = dict(sources); staged['entry.py'] = entry
    for role, target in [('encodings', 'encodings/__init__.py'), ('aliases', 'encodings/aliases.py'), ('utf8', 'encodings/utf_8.py')]: staged['freeze-stdlib/' + target] = data(role)
    for target, body in staged.items():
        path = stage / target; path.parent.mkdir(parents=True, exist_ok=True)
        with path.open('xb') as stream: stream.write(body)
    plan = {'format': 'python-debug-additive-build-plan-v1', 'status': 'CAPTURED_ARGV_NOT_EXECUTED',
            'original_base_receipt_sha256': BASE_SHA, 'original_oz_receipt_sha256': OZ_SHA,
            'source_closure_sha256': args.source_closure_sha256, 'build_admission_sha256': args.admission_sha256,
            'entry_sha256': args.entry_sha256, 'commands': commands,
            'staged_files': [{'path': str(stage / k), 'bytes': len(v), 'sha256': sha(v)} for k, v in sorted(staged.items())],
            'input_pins': list(cap.rows.values()), 'library_resolution_indices': operands,
            'execution_count': 0, 'runtime_ROS_DBG_qualified': False,
            'required_keeper': ['existing Linux64GiB/swap0 current membership/exclusive window', 'independently reviewed complete native/startup/compiler/helper/sysroot/header/archive/config input closure frozen through all later reads', 'finite wall/CPU/RSS/PIPE/EOF and real pidfd retirement; failure is not PASS', 'new actual freeze/compile/link/validator/checker/run receipts; preserve original products', 'canonical DWARF root/activation/controller and actual GC/debugger observations separately required'],
            'trust_boundary': 'External pinned evidence and these hashes bind bytes, not authority, independence or hermetic completeness; no passed/self-SHA field confers admission.'}
    with (stage / 'build-plan.json').open('x') as stream: json.dump(plan, stream, indent=2); stream.write('\n')
    cap.recheck()
    return plan


def main():
    need(sys.platform == 'linux' and sys.flags.isolated and sys.flags.no_site and sys.flags.safe_path, 'pre-admitted Linux Python -I -S only')
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source-closure', 'admission', 'entry', 'stage'): parser.add_argument('--' + name, type=Path, required=True)
    for name in ('source-closure', 'admission', 'entry', 'operator'): parser.add_argument('--' + name + '-sha256', required=True)
    result = prepare(parser.parse_args()); print(json.dumps(result, indent=2)); return 0


if __name__ == '__main__':
    try: raise SystemExit(main())
    except (ValueError, KeyError, TypeError, OSError) as failure:
        print(json.dumps({'status': 'HOLD', 'error_type': type(failure).__name__, 'error': str(failure), 'actual_commands_executed': 0}), file=sys.stderr)
        raise SystemExit(3)
