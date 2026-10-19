#!/usr/bin/env python3
"""Physical evidence primitives for the fresh Noetic native preparation.

This module has no import-time process, mount, compiler, network or package
installation side effect. The frozen producer supplies its live Root guard.
All path translation is explicit: a chroot spelling is never silently opened
against the host filesystem. SDK copies receive new inodes and are completely
rehash-checked; hardlinks are not used to save storage.
"""
from __future__ import annotations
import fcntl
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import shlex
import signal
import stat
import time

MIB, GIB = 1024**2, 1024**3
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}


def need(value, reason):
    if not value:
        raise ValueError(reason)


def facts(info):
    return tuple(getattr(info, key) for key in ('st_dev', 'st_ino', 'st_size',
        'st_mtime_ns', 'st_ctime_ns', 'st_mode', 'st_nlink'))


def rooted(root, logical):
    """Resolve a bounded SDK path with actual chroot absolute-link semantics.

    pathlib.resolve on SDK/usr/bin/ALIAS can otherwise follow an absolute
    target into the HOST /etc or /usr. Absolute symlink targets here restart
    at the selected SDK root. The returned leaf must be an ordinary file or
    directory. No selected SDK symlink grants access to a host target.
    """
    root = Path(root)
    need(root.is_absolute() and root.resolve(strict=True) == root and
         type(logical) is str and logical.startswith('/') and '\0' not in logical,
         'explicit physical SDK root and logical absolute spelling required')
    pending = logical.split('/'); parts = []; links = 0
    while pending:
        value = pending.pop(0)
        if value in ('', '.'):
            continue
        if value == '..':
            need(parts, 'SDK alias escapes selected root'); parts.pop(); continue
        path = root.joinpath(*parts, value)
        info = path.lstat()
        if stat.S_ISLNK(info.st_mode):
            links += 1; need(links <= 40, 'SDK alias depth exhausted')
            target = os.readlink(path)
            need(target and len(target.encode()) <= 4096 and '\0' not in target,
                 'bounded literal SDK alias required')
            need(facts(info) == facts(path.lstat()), 'SDK alias changed')
            if target.startswith('/'):
                parts = []
            pending = target.split('/') + pending
        else:
            need(stat.S_ISDIR(info.st_mode) or stat.S_ISREG(info.st_mode),
                 'SDK logical path crosses special node')
            parts.append(value)
    result = root.joinpath(*parts)
    need(result.resolve(strict=True) == result, 'resolved SDK physical spelling is not canonical')
    return result


def tree(common, root, output, guard):
    """Inventory every literal directory/link and every opened ordinary file.

    Links are not followed or promoted to host targets. Content digest omits
    inode/timestamps so a new independent copy can be compared to its original;
    the separate physical digest/catalog records every actual current pin.
    Empty directories and executable modes are included in the content proof.
    """
    root, output = Path(root), Path(output)
    need(root.resolve(strict=True) == root and root.is_dir(), 'canonical complete tree required')
    content = hashlib.sha256(); physical = hashlib.sha256(); files = links = directories = total = 0
    with output.open('xb') as stream:
        for folder, dirs, names in os.walk(root, followlinks=False):
            guard(); dirs.sort(); names.sort()
            for name in sorted(dirs + names):
                guard(); path = Path(folder) / name; first = path.lstat()
                relative = str(path.relative_to(root)); mode = stat.S_IMODE(first.st_mode)
                need(first.st_dev == root.stat().st_dev and not mode & 0o7000,
                     'ordinary single-filesystem non-set-id tree required')
                value = {'relative': relative, 'mode': mode}
                if stat.S_ISLNK(first.st_mode):
                    target = os.readlink(path); links += 1
                    need(target and len(target.encode()) <= 4096 and '\0' not in target,
                         'bounded literal tree alias required')
                    value.update(kind='symlink', target=target)
                    row = {**value, 'device': first.st_dev, 'inode': first.st_ino,
                        'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns}
                elif stat.S_ISDIR(first.st_mode):
                    directories += 1; value['kind'] = 'directory'
                    row = {**value, 'device': first.st_dev, 'inode': first.st_ino}
                else:
                    need(stat.S_ISREG(first.st_mode), 'special tree node refused')
                    pin = common.opened(path); files += 1; total += pin['bytes']
                    value.update(kind='file', bytes=pin['bytes'], sha256=pin['sha256'])
                    row = {**value, 'pin': pin, 'nlink': first.st_nlink}
                need(facts(first) == facts(path.lstat()), 'tree entry changed during inventory')
                need(files + links + directories <= 262144 and total <= 8 * GIB,
                     'complete tree size bound exhausted')
                body = (json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode()
                semantic = (json.dumps(value, sort_keys=True, separators=(',', ':')) + '\n').encode()
                stream.write(body); physical.update(body); content.update(semantic)
        stream.flush(); os.fsync(stream.fileno())
    return {'root': str(root), 'files': files, 'symlinks': links, 'directories': directories,
        'bytes': total, 'semantic_sha256': content.hexdigest(), 'physical_sha256': physical.hexdigest(),
        'catalog': common.opened(output), 'symlink_targets': 'LITERAL_CHROOT_NAMES_NOT_HOST_TARGET_QUALIFICATION'}


def clone_tree(common, source, destination, ledger, guard):
    """Make an independent same-filesystem FICLONE copy and hash both sides.

    Source aliases and any source hardlink multiplicity remain source facts.
    Every destination regular name receives its own nlink-1 inode. No source
    object is linked, renamed, chmod'ed or edited, and no ordinary-copy fallback
    is available when the actual filesystem rejects FICLONE.
    """
    source, destination = Path(source), Path(destination)
    need(source.resolve(strict=True) == source and source.is_dir() and
         destination.parent.resolve(strict=True) == destination.parent and
         not destination.exists() and not destination.is_symlink() and
         source != destination and source not in destination.parents and destination not in source.parents,
         'fresh disjoint independent SDK destination required')
    root_info = source.stat(); destination.mkdir(mode=0o700)
    need(destination.stat().st_dev == root_info.st_dev, 'same-filesystem required FICLONE copy')
    files = links = total = 0
    with Path(ledger).open('xb') as records:
        for folder, dirs, names in os.walk(source, followlinks=False):
            guard(); dirs.sort(); names.sort(); original_dir = Path(folder)
            output_dir = destination / original_dir.relative_to(source)
            old_dir = original_dir.lstat(); old_names = sorted(os.listdir(original_dir))
            need(stat.S_ISDIR(old_dir.st_mode) and old_dir.st_dev == root_info.st_dev,
                 'source directory is not a stable ordinary selected filesystem')
            for name in sorted(dirs + names):
                guard(); original = original_dir / name; copied = output_dir / name
                first = original.lstat(); relative = str(original.relative_to(source))
                need(first.st_dev == root_info.st_dev and not first.st_mode & 0o7000,
                     'cross-filesystem/set-id SDK entry refused')
                if stat.S_ISDIR(first.st_mode):
                    copied.mkdir(mode=stat.S_IMODE(first.st_mode)); row = {'kind': 'directory', 'relative': relative}
                elif stat.S_ISLNK(first.st_mode):
                    target = os.readlink(original)
                    need(target and len(target.encode()) <= 4096 and '\0' not in target, 'bounded SDK link required')
                    os.symlink(target, copied); links += 1
                    row = {'kind': 'symlink', 'relative': relative, 'target': target,
                        'source_device': first.st_dev, 'source_inode': first.st_ino}
                else:
                    need(stat.S_ISREG(first.st_mode) and first.st_size <= 2 * GIB,
                         'bounded ordinary SDK file required')
                    old = os.open(original, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
                    new = os.open(copied, os.O_RDWR | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o600)
                    try:
                        need(facts(first) == facts(os.fstat(old)), 'source SDK file changed before clone')
                        if first.st_size:
                            fcntl.ioctl(new, 0x40049409, old)  # Linux x86_64 FICLONE; no fallback.
                        os.fchmod(new, stat.S_IMODE(first.st_mode)); os.fsync(new)
                        new_info = os.fstat(new)
                        need(new_info.st_nlink == 1 and (new_info.st_dev, new_info.st_ino) !=
                             (first.st_dev, first.st_ino), 'SDK copy must have an independent nlink-1 inode')
                        need(facts(first) == facts(os.fstat(old)), 'source SDK changed while cloning')
                    finally:
                        os.close(old); os.close(new)
                    before = common.opened(original); after = common.opened(copied)
                    need((before['bytes'], before['sha256']) == (after['bytes'], after['sha256']) and
                         facts(first) == facts(original.lstat()), 'full source/clone bytes differ')
                    files += 1; total += before['bytes']
                    row = {'kind': 'file', 'relative': relative, 'source_current': before, 'copy_actual': after,
                        'source_nlink': first.st_nlink, 'copy_nlink': 1,
                        'method': 'FICLONE_FULL_BOTH_SIDE_REHASH' if first.st_size else 'EMPTY_NEW_ORDINARY_FILE'}
                need(facts(first) == facts(original.lstat()), 'source named SDK entry changed')
                need(files + links <= 262144 and total <= 8 * GIB, 'SDK copy budget exhausted')
                records.write((json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n').encode())
            need(facts(old_dir) == facts(original_dir.lstat()) and old_names == sorted(os.listdir(original_dir)),
                 'complete source directory changed while cloning')
        records.flush(); os.fsync(records.fileno())
    return {'source': str(source), 'copy': str(destination), 'files': files, 'symlinks': links,
        'bytes': total, 'ledger': common.opened(Path(ledger)), 'ordinary_copy_fallback': False,
        'hardlinks_created': False, 'both_full_stream_hashes_compared': True}


def preserve_sdk_copy(common, previous, current, guard):
    """Keep every original copied SDK row exact; allow only owned mount leaves.

    Mount operations need two empty directories and, if absent, one ordinary
    null placeholder. Their existence is a fresh producer artifact, not a
    claim that a device node was part of the signed ordinary SDK. No compiler,
    catkin generator or install rule may otherwise edit the copied SDK.
    """
    def rows(catalog):
        pin = catalog['catalog']
        need(common.opened(Path(pin['path'])) == pin, 'SDK at-rest catalog changed')
        result = {}
        with Path(pin['path']).open('rb') as stream:
            for raw in stream:
                guard(); need(len(raw) <= 16384 and len(result) <= 262144, 'bounded SDK preservation catalog required')
                value = json.loads(raw); name = value['relative']
                need(name not in result, 'duplicate complete SDK name'); result[name] = value
        return result
    before, after = rows(previous), rows(current)
    need(before.keys() <= after.keys() and all(after[name] == row for name, row in before.items()),
         'an original copied SDK file/link/directory changed during native preparation')
    extra = sorted(after.keys() - before.keys())
    need(set(extra) <= {'work', 'owned', 'dev', 'dev/null'}, 'unselected new SDK entry introduced')
    for name in extra:
        row = after[name]
        if name == 'dev/null':
            need(row['kind'] == 'file' and row['bytes'] == 0 and row['pin']['bytes'] == 0,
                 'only a fresh zero-byte ordinary null placeholder may be retained')
        else: need(row['kind'] == 'directory', 'owned mount parent must remain an ordinary directory')
    return {'original_copy_catalog': previous['catalog'], 'actual_at_rest_catalog': current['catalog'],
        'all_original_rows_exact': True, 'new_owned_mount_names_only': extra,
        'special_device_observation_promoted': False}


def translate(root, source, work, name, cwd):
    """Translate one original compiler/linker spelling through finite mounts."""
    logical = posixpath.normpath(name if name.startswith('/') else posixpath.join(cwd, name))
    need(logical.startswith('/') and '\0' not in logical, 'absolute normalized compiler input required')
    for prefix, physical in (('/work', source), ('/owned', work)):
        if logical == prefix or logical.startswith(prefix + '/'):
            path = Path(physical) / logical.removeprefix(prefix).lstrip('/')
            need(path.resolve(strict=True).is_relative_to(Path(physical)), 'build input escapes selected mount')
            return path.resolve(strict=True)
    return rooted(root, logical)


def build_proof(frame, root, work, build, log_row, expected_source, binary, elf_reader, *, compiled_source=None):
    """Reconcile actual compileDB, verbose Make cwd, full-MD and link trace.

    Exactly one target TU is expected here; compiler/configure probes are not
    silently counted as target TUs. The original complete raw logs and files
    stay ordinary products, and every actual source/header/object/archive is
    opened. No -MMD, response file, preprocessing-only or missing-file fallback
    is accepted as full dependency/link evidence.
    """
    common = frame.common; build = Path(build); physical_binary = Path(binary)
    selected_source = frame.source if compiled_source is None else Path(compiled_source)
    need(selected_source.resolve(strict=True) == selected_source, 'explicit original compiled source root required')
    database, database_pin = common.document(build / 'compile_commands.json')
    need(type(database) is list and len(database) == 1, 'one actual primary TU per closed target required')
    db = database[0]; original = db.get('arguments') or shlex.split(db['command'])
    source_logical = db['file']; logical_build = '/owned/' + str(build.relative_to(work))
    need(db['directory'] == logical_build and translate(root, selected_source, work, source_logical, logical_build) ==
         Path(expected_source).resolve(strict=True), 'actual compileDB target source/cwd differs')
    raw = Path(log_row['stdout']).read_bytes() + b'\n' + Path(log_row['stderr']).read_bytes()
    need(len(raw) <= 2 * frame.limits['log_bytes'], 'bounded full verbose build log required')
    matches = []; links = []; cwd = logical_build
    for line in raw.decode('utf-8', 'strict').splitlines():
        event = re.search(r"(?:g?make)(?:\[[0-9]+\])?: (Entering|Leaving) directory ['`](.+)['`]", line)
        if event:
            if event[1] == 'Entering': cwd = posixpath.normpath(event[2])
            continue
        try: argv = shlex.split(line)
        except ValueError: continue
        actual_cwd = cwd
        if argv[:1] == ['cd']:
            need(len(argv) >= 4 and argv[2] == '&&', 'closed verbose Make cd command required')
            actual_cwd = posixpath.normpath(argv[1]); argv = argv[3:]
        if not argv or argv[0] != original[0]: continue
        if '-c' in argv and source_logical in argv:
            matches.append((argv, actual_cwd))
        elif '-o' in argv and source_logical not in argv:
            links.append((argv, actual_cwd))
    need(len(matches) == 1, 'one original actual verbose primary compiler command required')
    argv, cwd = matches[0]
    forbidden = ('-MMD', '-MM', '-M', '-MP', '-MG', '-E', '-S', '-fsyntax-only', '-ffast-math', '-Ofast')
    need(not any(token in forbidden or token.startswith(('@', '-Wp,')) for token in argv) and '-MD' in argv and
         argv.count('-c') == argv.count('-o') == 1 and '-UNDEBUG' in argv and '-fno-fast-math' in argv and
         '-ffp-contract=off' in argv, 'actual full-MD/assertion/non-fast-math command required')
    assertion_flags = [token for token in argv if token in ('-UNDEBUG', '-DNDEBUG') or token.startswith('-DNDEBUG=')]
    standards = [token for token in argv if token.startswith('-std=')]
    optimization = [token for token in argv if re.fullmatch('-O[0-9szgfast]+', token)]
    expected_optimization = '-O3' if Path(expected_source).name == 'native_legacy_ros1_peer.cpp' else '-O1'
    need(assertion_flags[-1:] == ['-UNDEBUG'] and standards[-1:] == ['-std=c++17'] and
         optimization[-1:] == [expected_optimization], 'actual effective assertions/C++17/selected optimization differ')
    def normalized(tokens):
        result = []; i = 0
        while i < len(tokens):
            token = tokens[i]
            if token in ('-MF', '-MT', '-MQ'):
                need(i + 1 < len(tokens), 'missing dependency operand'); i += 2; continue
            if token != '-MD' and not token.startswith(('-MF', '-MT', '-MQ')): result.append(token)
            i += 1
        return result
    need(normalized(argv) == normalized(original), 'compileDB differs from original verbose compiler argv')
    def operands(flag):
        result = []
        for i, token in enumerate(argv):
            if token == flag:
                need(i + 1 < len(argv), 'missing original compiler operand'); result.append(argv[i + 1])
            elif token.startswith(flag) and token != flag: result.append(token[len(flag):])
        return result
    deps, targets = operands('-MF'), operands('-MT')
    need(deps and targets, 'actual full -MD/-MF/-MT operands required')
    obj = translate(root, selected_source, work, argv[argv.index('-o') + 1], cwd)
    dependency = translate(root, selected_source, work, deps[-1], cwd)
    body, dependency_pin = common.body_pin(dependency, 16 * MIB)
    text = body.decode().replace('\\\n', ' '); need(':' in text, 'one complete dependency rule required')
    target, tail = text.split(':', 1); names = shlex.split(tail)
    need(shlex.split(target) == [targets[-1]] and names and not any(value.endswith(':') for value in names),
         'original emitted dependency targets/complete rule differ')
    dependencies = []
    for name in names:
        pin = frame.pin(translate(root, selected_source, work, name, cwd))
        if pin not in dependencies: dependencies.append(pin)
    need(frame.pin(expected_source) in dependencies, 'primary source missing from actual full-MD')
    link_files = list(build.glob('CMakeFiles/*.dir/link.txt'))
    need(len(link_files) == 1, 'one actual selected target link.txt required')
    link_raw, link_pin = common.body_pin(link_files[0]); link_argv = shlex.split(link_raw.decode())
    need(link_argv[0] == argv[0] and '-o' in link_argv and
         translate(root, selected_source, work, link_argv[link_argv.index('-o') + 1], logical_build) == physical_binary and
         any(value in link_argv for value in ('-Wl,--trace', '--trace')) and
         any(translate(root, selected_source, work, value, logical_build) == obj for value in link_argv if value.endswith('.o')),
         'actual direct link/object/output/trace association required')
    need(sum(value == link_argv and where == logical_build for value, where in links) == 1,
         'actual verbose link differs from singleton link.txt')
    trace = []
    for line in raw.decode('utf-8', 'strict').splitlines():
        match = re.fullmatch(r'([^\s()]+)(?:\([^\r\n()]+\))?', line.strip())
        if not match: continue
        name = match[1]
        if not (name.startswith('/') or '/' in name and
                (name.endswith(('.o', '.a', '.so')) or '.so.' in name)):
            continue
        selected = translate(root, selected_source, work, name, logical_build)
        pin = frame.pin(selected)
        if pin not in trace: trace.append(pin)
    need(frame.pin(obj) in trace and trace, 'real linker trace must include compiled object and selected files')
    image = elf_reader(common, physical_binary, (2, 3))
    return {'primary_source': frame.pin(expected_source), 'compile_database': frame.pin(build / 'compile_commands.json'),
        'compiler_argv': argv, 'compiler_cwd': cwd, 'dependency_file': dependency_pin,
        'dependency_targets': targets, 'dependencies': dependencies, 'object': frame.pin(obj),
        'link_argv': link_argv, 'link_file': link_pin, 'link_inputs': trace,
        'verbose_stdout': frame.pin(log_row['stdout']), 'verbose_stderr': frame.pin(log_row['stderr']),
        'binary': frame.pin(physical_binary), 'ELF': image, 'primary_translation_units': 1,
        'compiled_source_root': str(selected_source), 'effective_cxx_standard': 'c++17',
        'effective_optimization': expected_optimization, 'effective_assertions': '-UNDEBUG',
        'configure_probe_translation_units': 'SEPARATE_CMAKE_CONFIGURE_ARTIFACTS_NOT_INCLUDED'}


def stopped_export(frame, owner, image, catalog, environment):
    """Admit actual opened exporter/maps while its own SIGSTOP gate is held."""
    child, row = owner.start('legacy-native-identity-export', [image['path'], '--mapped-stop'],
        environment=environment, json_output=False)
    common = frame.common; until = time.monotonic() + 30
    while True:
        owner.check(); identity = common.identity(child.pid)
        need(child.poll() is None and time.monotonic() <= until, 'exporter did not reach its own bounded stop')
        if identity['state'] in ('T', 't'): break
        time.sleep(.005)
    birth = common.birth(identity); entries = [value for key, value in owner.owner.owned.items() if key == birth]
    need(len(entries) == 1 and identity['parent_pid'] == os.getpid() and not common.pidfd_terminal(entries[0]['fd']),
         'actual direct exporter birth/pidfd must remain held')
    by_identity = {tuple(pin[key] for key in ('device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns')): pin for pin in catalog}
    by_identity[tuple(image[key] for key in ('device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns'))] = image
    def live():
        owner.check(); now = common.identity(child.pid)
        need(common.birth(now) == birth and now['state'] in ('T', 't') and
             Path('/proc/%d/cgroup' % child.pid).read_text() == Path('/proc/self/cgroup').read_text() and
             not common.pidfd_terminal(entries[0]['fd']), 'stopped exporter identity/cgroup/pidfd changed')
    def opened(descriptor):
        before = os.fstat(descriptor); key = tuple(getattr(before, name) for name in
            ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns'))
        need(stat.S_ISREG(before.st_mode) and key in by_identity, 'actual exporter mapping outside complete selected closure')
        selected = by_identity[key]; digest = hashlib.sha256(); count = 0
        while True:
            live(); data = os.read(descriptor, MIB)
            if not data: break
            count += len(data); need(count <= selected['bytes'], 'mapped exporter file grew'); digest.update(data)
        need(count == selected['bytes'] and digest.hexdigest() == selected['sha256'] and
             facts(before) == facts(os.fstat(descriptor)), 'full opened mapped bytes differ')
        return selected
    exe = os.open('/proc/%d/exe' % child.pid, os.O_RDONLY | os.O_CLOEXEC)
    try: need(opened(exe) == image, 'stopped exporter actual /proc/exe differs')
    finally: os.close(exe)
    live(); raw = Path('/proc/%d/maps' % child.pid).read_bytes()
    need(len(raw) <= 4 * MIB, 'bounded stopped exporter maps required'); observations = []
    for line in raw.decode('utf-8', 'surrogateescape').splitlines():
        words = line.split(None, 5)
        if len(words) < 5 or not words[4].isdecimal() or int(words[4]) == 0: continue
        need(re.fullmatch('[0-9a-f]+-[0-9a-f]+', words[0]), 'actual map range invalid')
        fd = os.open('/proc/%d/map_files/%s' % (child.pid, words[0]), os.O_RDONLY | os.O_NONBLOCK | os.O_CLOEXEC)
        try: pin = opened(fd)
        finally: os.close(fd)
        observations.append({'display_only_map': line, 'actual_opened_file': pin})
    need(observations, 'actual stopped exporter mappings required'); live()
    # Release only the exact still-held birth once. No numeric-PID signal or
    # process-group ownership guess is used, including the cooperative resume.
    signal.pidfd_send_signal(entries[0]['fd'], signal.SIGCONT)
    owner.write(child, b'I'); child.stdin.close(); owner.finish(child)
    need(not Path(row['stderr']).read_bytes(), 'native metadata exporter emitted stderr')
    body, output_pin = common.body_pin(Path(row['stdout']), 4 * MIB)
    return body, output_pin, {'birth': identity, 'pidfd_owned': True, 'actual_executable': image,
        'all_file_mappings': observations, 'gate': 'COOPERATIVE_SIGSTOP_SAME_BIRTH_PIDFD_CONT_THEN_SINGLE_I',
        'stdout': output_pin, 'stderr': frame.pin(row['stderr']), 'returncode': row['returncode']}
