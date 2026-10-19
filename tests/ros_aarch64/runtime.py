"""Live Linux/QEMU evidence for ARM ROS tests; never relabel /proc/exe as ARM."""
from __future__ import annotations

import hashlib
import json
import os
import select
import stat
from pathlib import Path
import threading
import time

# Current trusted test budget requested by the user. Old 64-GiB receipts retain
# their original source/limit; they cannot qualify these current controllers.
EXPECTED_MEMORY_MAX = 68719476736


def digest(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def read_json_binding(path, limit=16*1024*1024):
    """Parse and bind the same bounded bytes from one opened regular file.

    Reports are control inputs: a pathname FIFO must not block before a runner
    deadline, and a second independent hash read must not bind different JSON
    bytes. The returned binding also detects later same-byte path replacement.
    This is consistency evidence around a read, not a filesystem-wide lock.
    """
    path = Path(path)
    nonblocking = getattr(os, 'O_NONBLOCK', None)
    if not isinstance(nonblocking, int) or nonblocking <= 0:
        raise RuntimeError('bounded report reads require O_NONBLOCK')
    fd = os.open(path, os.O_RDONLY | nonblocking)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= limit:
            raise RuntimeError('report must be an opened bounded regular file')
        pieces, left = [], first.st_size+1
        while left:
            piece = os.read(fd, min(left, 1024*1024))
            if not piece:
                break
            pieces.append(piece);left -= len(piece)
        raw = b''.join(pieces)
        last = os.fstat(fd)
        keys = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
        if (not stat.S_ISREG(last.st_mode) or len(raw) != first.st_size
                or any(getattr(first,key) != getattr(last,key) for key in keys)):
            raise RuntimeError('report changed during bounded read')
    finally:
        os.close(fd)
    binding = {'path':str(path), 'device':last.st_dev, 'inode':last.st_ino,
               'bytes':len(raw), 'mtime_ns':last.st_mtime_ns, 'ctime_ns':last.st_ctime_ns,
               'sha256':hashlib.sha256(raw).hexdigest()}
    return json.loads(raw), binding


def elf(path):
    with Path(path).open('rb') as stream:
        first = os.fstat(stream.fileno())
        header = stream.read(64)
        if len(header) < 20 or header[:4] != b'\x7fELF' or header[5] not in (1, 2):
            raise RuntimeError('BLOCKED not an ELF executable: ' + str(path))
        identity = os.fstat(stream.fileno())
        stream.seek(0)
        value = hashlib.file_digest(stream, 'sha256').hexdigest()
        last = os.fstat(stream.fileno())
        if (not stat.S_ISREG(first.st_mode) or
                any(getattr(first,key) != getattr(last,key) for key in
                    ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns'))):
            raise RuntimeError('ELF changed during opened descriptor binding: '+str(path))
    return {'path': str(path), 'sha256': value, 'bytes': identity.st_size,
            'device': identity.st_dev, 'inode': identity.st_ino, 'elf_class': header[4],
            'elf_data': header[5],
            'e_machine': int.from_bytes(header[18:20], 'little' if header[5] == 1 else 'big')}


def require_arm(path):
    result = elf(path)
    if (result['elf_class'], result['elf_data'], result['e_machine']) != (2, 1, 183):
        raise RuntimeError('BLOCKED expected ELF64 little-endian AArch64 target: ' + str(path))
    return result


MAPPING_METHOD = 'OPENED_PROC_MAP_FILES_V1'
_MAP_FACTS = ('device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns')


def mapping_file_facts(state):
    return dict(zip(_MAP_FACTS, (state.st_dev, state.st_ino, state.st_size,
                                state.st_mtime_ns, state.st_ctime_ns)))


def canonical_map_files_range(value):
    """Return Linux's unpadded lowercase map_files basename, retaining no authority.

    /proc/PID/maps may pad low addresses; map_files lookup rejects redundant
    leading zeroes. Only the opened kernel FD and unchanged receipt checks can
    prove a mapping. This bounded numeric spelling never replaces raw evidence.
    """
    if type(value) is not str or not 3 <= len(value) <= 33:
        raise RuntimeError('bounded native64 mapping range required')
    bounds = value.split('-')
    if (len(bounds) != 2 or any(not item or len(item) > 16 or
            any(letter not in '0123456789abcdefABCDEF' for letter in item) for item in bounds)):
        raise RuntimeError('malformed inode-backed proc mapping range')
    start, end = (int(item, 16) for item in bounds)
    if start >= end:
        raise RuntimeError('mapping range must increase')
    return format(start, 'x') + '-' + format(end, 'x')


def opened_target_mappings(pid, targets, guard, cache=None):
    """Bind actual map_files descriptors, never the display-only maps device.

    The caller supplies a live owned-birth/cgroup/executable guard. This helper
    opens its own pidfd but never signals. Every inode-backed map is fstat-filtered
    before hashing; unrelated DSOs are not read. A target is bounded by its exact
    admitted size, with no new architecture-specific size ceiling. Optional
    cache entries retain the original hashed FD and require identical physical
    facts on that descriptor and the newly opened mapping before reuse.
    """
    own_cache = cache is None
    cache = {} if own_cache else cache
    first = guard()
    if first.get('pid') != pid or first.get('state') in ('Z', 'X'):
        raise RuntimeError('mapping guard did not establish a live owned birth')
    pidfd = os.pidfd_open(pid)
    poller = select.poll();poller.register(pidfd, select.POLLIN)
    def coherent():
        value = guard()
        keys = ('pid', 'parent_pid', 'starttime_ticks', 'membership', 'host_identity')
        if (poller.poll(0) or value.get('state') in ('Z', 'X') or
                any(value.get(key) != first.get(key) for key in keys) or
                (first.get('state') in ('T', 't') and value.get('state') not in ('T', 't'))):
            raise RuntimeError('owned birth/cgroup/executable changed during mapping proof')
        return value
    try:
        coherent()
        with (Path('/proc')/str(pid)/'maps').open('rb') as stream:
            raw = stream.read(4*1024*1024+1)
        if len(raw) > 4*1024*1024:
            raise RuntimeError('bounded proc maps exceeded 4 MiB')
        lines = raw.decode('utf-8', errors='surrogateescape').splitlines()
        matches = {label: [] for label in targets}
        for line in lines:
            parts = line.split(None, 5)
            if len(parts) < 5 or not parts[4].isdecimal() or int(parts[4]) == 0:
                continue
            map_files_range = canonical_map_files_range(parts[0])
            if poller.poll(0):raise RuntimeError('owned mapping birth became terminal')
            # This kernel magic link is followed deliberately, only for the
            # guarded owned birth. Its pathname target is not a source of trust.
            fd = os.open(Path('/proc')/str(pid)/'map_files'/map_files_range, os.O_RDONLY|os.O_NONBLOCK)
            try:
                state = os.fstat(fd);facts = mapping_file_facts(state)
                if not stat.S_ISREG(state.st_mode):
                    raise RuntimeError('inode-backed map_files descriptor is not regular')
                selected = [(label, target) for label, target in targets.items()
                            if all(facts[key] == target.get(key) for key in ('device','inode','bytes'))
                            and all(key not in target or facts[key] == target[key]
                                    for key in ('mtime_ns','ctime_ns'))]
                if not selected:
                    continue
                coherent()
                key = tuple(facts[name] for name in _MAP_FACTS)
                held = cache.get(key)
                if held is not None:
                    if mapping_file_facts(os.fstat(held['fd'])) != facts:
                        raise RuntimeError('retained mapped target descriptor changed')
                    value, header, hashed_at = held['sha256'], held['header'], held['hashed_at']
                    mode = 'RETAINED_HASHED_DESCRIPTOR'
                else:
                    header = os.read(fd, 64);os.lseek(fd, 0, os.SEEK_SET)
                    digestor = hashlib.sha256();count = 0
                    while True:
                        coherent()  # Preserve cancellation/cgroup/birth checks for large targets.
                        chunk = os.read(fd, min(1024*1024, state.st_size-count+1))
                        if not chunk:break
                        count += len(chunk)
                        if count > state.st_size:raise RuntimeError('mapped target grew beyond admitted size')
                        digestor.update(chunk)
                    if count != state.st_size:raise RuntimeError('mapped target shortened during binding')
                    value, hashed_at, mode = digestor.hexdigest(), time.monotonic_ns(), 'FULL_OPENED_DESCRIPTOR_HASH'
                    retained = os.dup(fd)
                    cache[key] = {'fd':retained, 'sha256':value, 'header':header, 'hashed_at':hashed_at}
                if (len(header) < 20 or header[:4] != b'\x7fELF' or header[5] not in (1, 2)
                        or mapping_file_facts(os.fstat(fd)) != facts):
                    raise RuntimeError('mapped target is not the same stable ELF descriptor')
                image = dict(facts, sha256=value, elf_class=header[4], elf_data=header[5],
                    e_machine=int.from_bytes(header[18:20], 'little' if header[5] == 1 else 'big'))
                for label, target in selected:
                    expected = ('device','inode','bytes','sha256','elf_class','elf_data','e_machine')
                    if any(image[key] != target.get(key) for key in
                           (*expected, *(key for key in ('mtime_ns','ctime_ns') if key in target))):
                        raise RuntimeError('opened mapped target bytes differ from admitted artifact: '+label)
                    matches[label].append({'range':parts[0], 'raw_mapping':line, 'opened_file':image,
                        'hash_method':mode, 'initial_hash_monotonic_ns':hashed_at,
                        'initial_full_hash':{'method':'FULL_OPENED_DESCRIPTOR_HASH',
                            'opened_file':dict(image), 'capture_monotonic_ns':hashed_at}})
                coherent()
            finally:
                os.close(fd)
        last = coherent()
        return {'format_version':1, 'method':MAPPING_METHOD, 'identity_before':first,
                'identity_after':last, 'raw_maps':list(dict.fromkeys(row['raw_mapping']
                    for rows in matches.values() for row in rows)), 'targets':matches}
    finally:
        os.close(pidfd)
        if own_cache:
            for item in cache.values():os.close(item['fd'])


def validate_mapping_receipts(sample, label, target):
    """Pure receipt admission; historical raw device/inode text is insufficient."""
    receipt = sample.get('opened_target_mappings', {})
    if (receipt.get('format_version') != 1 or receipt.get('method') != MAPPING_METHOD
            or not isinstance(receipt.get('raw_maps'), list)):
        return False
    first, last = receipt.get('identity_before', {}), receipt.get('identity_after', {})
    for key in ('pid', 'parent_pid', 'starttime_ticks', 'membership'):
        if first.get(key) != last.get(key) or first.get(key) != sample.get(key):return False
    if (any(type(first.get(key)) is not int or first[key] <= 0 for key in
            ('pid', 'parent_pid', 'starttime_ticks')) or not first.get('membership')
            or first.get('state') in (None, 'Z', 'X') or last.get('state') in (None, 'Z', 'X')
            or (first.get('state') in ('T','t') and last.get('state') not in ('T','t'))
            or sample.get('live_cgroup_procs') is not True
            or first.get('live_cgroup_procs') is not True or last.get('live_cgroup_procs') is not True
            or first.get('host_identity') != last.get('host_identity')
            or any(type(first.get('host_identity', {}).get(key)) is not int
                   or first['host_identity'][key] < 0 for key in _MAP_FACTS)
            or first.get('host_identity', {}).get('inode',0) <= 0
            or first.get('host_identity', {}).get('bytes',0) <= 0
            or any(first.get('host_identity', {}).get(key) != sample.get('kernel_exe_elf', {}).get(key)
                   for key in _MAP_FACTS)):
        return False
    rows = receipt.get('targets', {}).get(label)
    if not isinstance(rows, list) or not rows:return False
    if (any(type(target.get(key)) is not int or target[key] < 0 for key in ('device','inode','bytes'))
            or target['bytes'] <= 0 or target['inode'] <= 0
            or not isinstance(target.get('sha256'), str) or len(target['sha256']) != 64):return False
    for row in rows:
        image = row.get('opened_file', {})
        initial = row.get('initial_full_hash', {})
        if (row.get('raw_mapping') not in receipt['raw_maps']
                or row.get('range') != row['raw_mapping'].split(None, 1)[0]
                or row.get('hash_method') not in ('FULL_OPENED_DESCRIPTOR_HASH', 'RETAINED_HASHED_DESCRIPTOR')
                or type(row.get('initial_hash_monotonic_ns')) is not int or row['initial_hash_monotonic_ns'] <= 0
                or initial.get('method') != 'FULL_OPENED_DESCRIPTOR_HASH'
                or initial.get('opened_file') != image
                or initial.get('capture_monotonic_ns') != row['initial_hash_monotonic_ns']
                or any(type(image.get(key)) is not int or image[key] < 0 for key in _MAP_FACTS)
                or any(image.get(key) != target.get(key) for key in
                       ('device','inode','bytes','sha256','elf_class','elf_data','e_machine',
                        *(key for key in ('mtime_ns','ctime_ns') if key in target)))):
            return False
    return True


def validate_architecture_mappings(report):
    """Reject any promoted label without its current opened-descriptor proof."""
    targets = report.get('arm_targets', {})
    for sample in report.get('actual_process_samples', []):
        for label in sample.get('mapped_arm_targets', []):
            if label not in targets or not validate_mapping_receipts(sample, label, targets[label]):
                raise RuntimeError('opened map_files receipt missing or inconsistent: '+str(label))


def cgroup_incarnation(leaf):
    """Pin the real leaf and boot before accepting any OOM counter baseline."""
    leaf = Path(leaf)
    state = leaf.stat()
    boot = Path('/proc/sys/kernel/random/boot_id').read_text().strip()
    if not boot:
        raise RuntimeError('BLOCKED kernel boot identity is unavailable')
    return {'path': str(leaf), 'device': state.st_dev, 'inode': state.st_ino,
            'boot_id': boot}


def cgroup_oom_deltas(before, after, same_incarnation):
    """Counter subtraction is valid only inside the exact same pinned cgroup."""
    if not same_incarnation:
        return False, []

    def qualified_root(row):
        """Validate the probe's exact controllerless cgroup2-root evidence.

        A generic null/unavailable counter is insufficient. The recorded raw
        mountinfo must identify this path as the hierarchy root, the mount's
        device must match the directory identity, and every required controller
        file must have been absent with ENOENT. No live filesystem is read here.
        """
        root = row.get('controllerless_hierarchy_root')
        if not isinstance(root, dict) or root.get('path') != row.get('path'):
            return False
        required = ('memory.max', 'memory.current', 'memory.events')
        if (any(name not in row or row[name] is not None for name in required)
                or root.get('missing_files') != {name: 'ENOENT' for name in required}
                or type(root.get('device')) is not int or root['device'] < 0
                or type(root.get('inode')) is not int or root['inode'] <= 0):
            return False
        mount = root.get('mount')
        if (not isinstance(mount, dict) or mount.get('root') != '/'
                or mount.get('filesystem_type') != 'cgroup2'
                or mount.get('mount_point') != root['path']):
            return False
        try:
            prefix, suffix = mount['raw_mountinfo'].split(' - ')
            fields, filesystem = prefix.split(), suffix.split()
            major, minor = (int(value) for value in fields[2].split(':'))
            valid = (len(fields) >= 6 and len(filesystem) >= 3 and fields[0].isdecimal()
                    and fields[0] == mount.get('mount_id') and fields[2] == mount.get('major_minor')
                    and fields[3] == '/' and fields[4] == root['path'] and filesystem[0] == 'cgroup2'
                    and (os.major(root['device']), os.minor(root['device'])) == (major, minor))
            if valid:
                # Optional mountinfo propagation tags legitimately change when
                # the parent makes its mount namespace private. Compare actual
                # root/mount identity, after validating each raw record, rather
                # than treating raw text spelling as the cgroup incarnation.
                return {'path': root['path'], 'device': root['device'], 'inode': root['inode'],
                        'mount': {name: mount[name] for name in ('mount_id', 'major_minor', 'root',
                                                                 'mount_point', 'filesystem_type')},
                        'missing_files': root['missing_files']}
            return False
        except (AttributeError, IndexError, KeyError, TypeError, ValueError):
            return False

    deltas, valid, compared = [], True, 0
    by_path = {row['path']: row for row in after.get('cgroup_ancestors', [])}
    for first in before['cgroup_ancestors']:
        last = by_path.get(first['path'])
        if not last or first.get('memory.max') != last.get('memory.max'):
            valid = False
            continue
        if first.get('memory.events') is None or last.get('memory.events') is None:
            first_root, last_root = qualified_root(first), qualified_root(last)
            if first_root and first_root == last_root:
                deltas.append({'path': first['path'],
                               'comparison': 'NOT_APPLICABLE_QUALIFIED_CONTROLLERLESS_HIERARCHY_ROOT',
                               'controllerless_hierarchy_root': first['controllerless_hierarchy_root'],
                               'compared_root_identity': first_root})
                continue
            deltas.append({'path': first['path'], 'unavailable': True})
            valid = False
            continue
        parse = lambda text: dict((key, int(value)) for key, value in
                                  (line.split() for line in text.splitlines()))
        old, new = parse(first['memory.events']), parse(last['memory.events'])
        keys = ('oom', 'oom_kill', 'oom_group_kill')
        if any(key not in old or key not in new for key in keys):
            deltas.append({'path': first['path'], 'unavailable': True})
            valid = False
            continue
        row = {'path': first['path'], **{key: new[key]-old[key] for key in keys}}
        compared += 1
        if any(row[key] for key in keys):
            valid = False
        deltas.append(row)
    return valid and compared > 0, deltas


def process_child_pids(pid):
    """Read direct children created by any thread of one process.

    Linux exposes each thread's children separately. A thread disappearing
    during enumeration is unavailable evidence; other thread records still
    contribute. Callers must independently bracket process birth/parent and
    cgroup identity before accepting a mapping or signalling a descendant.
    """
    children = set()
    for task in (Path('/proc')/str(pid)/'task').iterdir():
        try:
            children.update(int(value) for value in (task/'children').read_text().split())
        except (FileNotFoundError, ProcessLookupError):
            continue
    return sorted(children)


class Evidence:
    """Sample host processes and prove a frozen ARM target was actually mapped.

    A target's command line or maps display device is not execution proof.
    Actual map_files descriptors bind the frozen physical file and bytes;
    host /proc/exe is separately pinned. This works with QEMU and binfmt.
    """

    def __init__(self, out, probe, interval=.002):
        self.out = Path(out)
        self.out.mkdir(parents=True, exist_ok=False)
        self.probe = probe
        discovered = probe()
        if (discovered.get('status') != 'PASS' or
                type(discovered.get('effective_memory_max')) is not int or
                discovered['effective_memory_max'] != EXPECTED_MEMORY_MAX):
            raise RuntimeError('BLOCKED inherited 64 GiB cgroup not proven')
        self.leaf = Path(discovered['cgroup_ancestors'][0]['path'])
        self._incarnation = lambda: cgroup_incarnation(self.leaf)
        self.incarnation_before = self._incarnation()
        # The first probe locates the leaf only. Capture the accepted counter
        # baseline between identity samples, so a replaced leaf/boot never
        # contributes a counter from another incarnation to this run.
        self.before = probe()
        if (self.before.get('status') != 'PASS' or
                type(self.before.get('effective_memory_max')) is not int or
                self.before['effective_memory_max'] != EXPECTED_MEMORY_MAX or
                (self.leaf/'memory.max').read_text().strip() != str(EXPECTED_MEMORY_MAX) or
                self.before['membership'] != discovered['membership'] or
                self.before['cgroup_ancestors'][0]['path'] != str(self.leaf) or
                self._incarnation() != self.incarnation_before):
            raise RuntimeError('BLOCKED cgroup incarnation changed while accepting baseline')
        self.interval = interval
        self.targets = {}
        self.processes = []
        self.samples = {}
        self.unavailable = []
        self.fatal_errors = []
        self.emulator_shas = set()
        self._elf_cache = {}
        self._mapping_cache = {}
        self._stop = threading.Event()
        self._lock = threading.RLock()
        self.thread = threading.Thread(target=self._loop, daemon=True)
        self.thread.start()

    def target(self, label, path):
        result = require_arm(path)
        with self._lock:
            self.targets[label] = result
        return result

    def emulator(self, path):
        result = elf(path)
        if result['e_machine'] != 62:
            raise RuntimeError('BLOCKED this QEMU-user run requires a verified x86_64 host emulator')
        with self._lock:
            self.emulator_shas.add(result['sha256'])
        return result

    def start(self, process, name, command):
        with self._lock:
            self.processes.append({'process': process, 'name': name, 'command': command})
        self.observe(process.pid, name)

    def mapping(self, pid, targets=None):
        """Capture a current live mapping without STOP or authority admission.

        Callers retain their existing owned ancestry/pidfd checks. The sampler
        independently brackets the same birth, parent, live original cgroup and
        host executable. The shared map helper opens an additional temporary
        pidfd so PID reuse/exit cannot turn a mapping into a positive receipt.
        """
        with self._lock:
            if not hasattr(self, '_mapping_cache'):self._mapping_cache = {}
            receipt = opened_target_mappings(pid, dict(self.targets) if targets is None else targets,
                                            lambda:self.mapping_guard(pid), self._mapping_cache)
            if not hasattr(self, '_last_mapping_guards'):self._last_mapping_guards = {}
            self._last_mapping_guards[(pid, receipt['identity_after']['starttime_ticks'])] = receipt['identity_after']
            return receipt

    def mapping_guard(self, pid):
        if self._stop.is_set():raise RuntimeError('mapping sampler is closing')
        if (self._incarnation() != self.incarnation_before or
                (self.leaf/'memory.max').read_text().strip() != str(EXPECTED_MEMORY_MAX) or
                (self.leaf/'memory.swap.max').read_text().strip() != '0'):
            raise RuntimeError('mapping original cgroup incarnation/limits changed')
        proc = Path('/proc')/str(pid)
        fields = (proc/'stat').read_text().rsplit(')', 1)[1].split()
        host = (proc/'exe').stat()
        membership = (proc/'cgroup').read_text().splitlines()
        if (membership != self.before['membership'] or
                str(pid) not in (self.leaf/'cgroup.procs').read_text().split()):
            raise RuntimeError('mapping birth lacks original live cgroup membership')
        newest = (proc/'stat').read_text().rsplit(')', 1)[1].split()
        if (fields[1], fields[19]) != (newest[1], newest[19]):
            raise RuntimeError('mapping birth/parent changed during guard')
        return {'pid':pid, 'parent_pid':int(fields[1]), 'starttime_ticks':int(fields[19]),
                'state':newest[0], 'membership':membership, 'live_cgroup_procs':True,
                'host_identity':mapping_file_facts(host)}

    def before_resume(self, pid, actual):
        """Final live birth/T/cgroup/executable check immediately before CONT."""
        current = self.mapping_guard(pid)
        previous = getattr(self, '_last_mapping_guards', {}).get((pid, actual['starttime_ticks']))
        if (current['state'] not in ('T', 't') or previous is None
                or any(current[key] != actual[key] for key in ('pid', 'parent_pid', 'starttime_ticks'))
                or any(current[key] != previous[key] for key in ('host_identity', 'membership'))):
            raise RuntimeError('stopped owned birth/cgroup/executable changed before resume')

    def mapped(self, pid, target):
        """Current descriptor proof used by existing owned stopped callers."""
        receipt = self.mapping(pid, {'selected':target})
        return bool(receipt['targets']['selected'])

    def _loop(self):
        while not self._stop.wait(self.interval):
            try:
                limits_unchanged = ((self.leaf/'memory.max').read_text().strip() == str(EXPECTED_MEMORY_MAX)
                                    and (self.leaf/'memory.swap.max').read_text().strip() == '0')
            except OSError:
                limits_unchanged = False
            if not limits_unchanged:
                with self._lock:
                    self.fatal_errors.append('current 64-GiB/zero-swap cgroup limits changed during sampling')
                return
            with self._lock:
                running = list(self.processes)
            for row in running:
                if row['process'].poll() is None:
                    self.observe(row['process'].pid, row['name'])

    def observe(self, root, name):
        pending = [(root, [root])]
        seen = set()
        while pending:
            pid, ancestry = pending.pop()
            if pid in seen:
                continue
            seen.add(pid)
            proc = Path('/proc') / str(pid)
            try:
                pending.extend((child, ancestry + [child]) for child in process_child_pids(pid))
                stat = (proc/'stat').read_text().rsplit(')', 1)[1].split()
                parent, starttime = int(stat[1]), int(stat[19])
                if len(ancestry) > 1 and parent != ancestry[-2]:
                    continue
                membership = (proc/'cgroup').read_text().splitlines()
                live = str(pid) in (self.leaf/'cgroup.procs').read_text().split()
                if membership != self.before['membership'] or not live:
                    newest = (proc/'stat').read_text().rsplit(')', 1)[1].split()
                    if int(newest[19]) != starttime or newest[0] in ('Z', 'X'):
                        continue
                    newest_membership = (proc/'cgroup').read_text().splitlines()
                    if membership == self.before['membership'] and newest_membership == self.before['membership']:
                        # cgroup.procs is not an atomic process snapshot. A
                        # process entering do_exit can disappear from that list
                        # before procfs reports Z/X. This is unavailable evidence,
                        # never a positive sample or proof of cgroup departure.
                        with self._lock:
                            if len(self.unavailable) < 100:
                                self.unavailable.append({'pid': pid, 'name': name,
                                    'reason': 'matching cgroup membership but PID absent from cgroup.procs snapshot',
                                    'live_cgroup_procs': False})
                        continue
                    with self._lock:
                        self.fatal_errors.append({'pid': pid, 'name': name,
                            'reason': 'actual process left inherited cgroup',
                            'membership': membership, 'newest_membership': newest_membership,
                            'live_cgroup_procs': live})
                    continue
                with self._lock:
                    targets = dict(self.targets)
                # Capture /proc facts before hashing the pinned host ELF FD. A
                # fast guest can exit during that hash without losing its proof.
                argv = (proc/'cmdline').read_bytes().split(b'\0')
                exe_path = os.readlink(proc/'exe')
                with (proc/'exe').open('rb') as stream:
                    identity = os.fstat(stream.fileno())
                    header = stream.read(64)
                    if len(header) < 20 or header[:4] != b'\x7fELF':
                        continue
                    cache_key = (identity.st_dev, identity.st_ino, identity.st_size,
                                 identity.st_mtime_ns, identity.st_ctime_ns)
                    # Reject PID reuse and an exec that raced this /proc sample.
                    coherent = (proc/'stat').read_text().rsplit(')', 1)[1].split()
                    latest = (proc/'exe').stat()
                    if int(coherent[19]) != starttime or int(coherent[1]) != parent or (latest.st_dev, latest.st_ino) != (identity.st_dev, identity.st_ino):
                        continue
                    with self._lock:
                        host = self._elf_cache.get(cache_key)
                    mapping_receipt = self.mapping(pid, targets)
                    captured_identity = mapping_receipt['identity_before']
                    if (captured_identity['starttime_ticks'] != starttime or captured_identity['parent_pid'] != parent
                            or captured_identity['host_identity'] != mapping_file_facts(identity)):
                        continue
                    mapped = {label for label, rows in mapping_receipt['targets'].items() if rows}
                    captured = time.monotonic_ns()
                    if host is None:
                        stream.seek(0)
                        host = {'sha256': hashlib.file_digest(stream, 'sha256').hexdigest(),
                                **mapping_file_facts(identity),
                                'elf_class': header[4], 'elf_data': header[5],
                                'e_machine': int.from_bytes(header[18:20],
                                    'little' if header[5] == 1 else 'big')}
                        if mapping_file_facts(os.fstat(stream.fileno())) != mapping_file_facts(identity):
                            raise RuntimeError('host executable changed during opened descriptor hash')
                        with self._lock:
                            self._elf_cache[cache_key] = host
                # Pin and sample identity before hashing; a fast target may exit
                # during hashing, but the captured live map and FD remain proof.
                key = (pid, starttime, identity.st_dev, identity.st_ino, tuple(sorted(mapped)),
                       mapping_receipt['identity_after']['state'] in ('T', 't'))
                record = {'name': name, 'pid': pid, 'parent_pid': parent,
                          'ancestry': ancestry, 'starttime_ticks': starttime,
                          'capture_monotonic_ns': captured, 'membership': membership,
                          'live_cgroup_procs': True, 'kernel_exe_path': exe_path,
                          'kernel_exe_elf': host,
                          'command': [x.decode(errors='replace') for x in argv if x],
                          'mapped_arm_targets': sorted(mapped),
                          'opened_target_mappings': mapping_receipt,
                          'target_mappings': mapping_receipt['raw_maps'],
                          'execution_model': 'QEMU user host process' if host['e_machine'] == 62
                                             and mapped and host['sha256'] in self.emulator_shas
                                             else 'kernel ELF as observed'}
                with self._lock:
                    self.samples.setdefault(key, record)
            except (FileNotFoundError, ProcessLookupError):
                pass
            except (OSError, RuntimeError, ValueError) as error:
                with self._lock:
                    if len(self.unavailable) < 100:
                        self.unavailable.append({'pid': pid, 'name': name, 'reason': str(error)})

    def check(self, required, name):
        # Flush current snapshots immediately before a caller closes its group.
        with self._lock:
            processes = list(self.processes)
        for row in processes:
            if row['name'].startswith(name) and row['process'].poll() is None:
                self.observe(row['process'].pid, row['name'])
        with self._lock:
            observed = {label for sample in self.samples.values()
                        if sample['name'].startswith(name) for label in sample['mapped_arm_targets']}
        missing = sorted(set(required) - observed)
        if missing:
            raise RuntimeError('FAIL missing actual ARM target mapping for ' + name + ': ' + ', '.join(missing))
        with self._lock:
            unsupported = [sample for sample in self.samples.values()
                           if sample['name'].startswith(name) and sample['mapped_arm_targets']
                           and sample['kernel_exe_elf']['e_machine'] != 183
                           and sample['execution_model'] != 'QEMU user host process']
        if unsupported:
            raise RuntimeError('FAIL mapped ARM target without a recognized QEMU host process: ' + name)

    def finish(self, status, extra=None):
        self._stop.set()
        self.thread.join(timeout=2)
        sampler_stopped = not self.thread.is_alive()
        if sampler_stopped:
            with self._lock:
                for item in getattr(self, '_mapping_cache', {}).values():os.close(item['fd'])
                self._mapping_cache = {}
        else:
            # Never close a descriptor underneath a still-running hash. A
            # stalled sampler cannot produce a successful architecture receipt.
            status = 'FAIL'
            self.fatal_errors.append('mapping sampler did not stop within its bounded join')
        identity_error = None
        try:
            finish_identity_before = self._incarnation()
        except (OSError, AttributeError, RuntimeError) as error:
            finish_identity_before = None
            identity_error = str(error)
        after = self.probe()
        try:
            finish_identity_after = self._incarnation()
        except (OSError, AttributeError, RuntimeError) as error:
            finish_identity_after = None
            identity_error = str(error)
        same_incarnation = (getattr(self, 'incarnation_before', None) is not None and
                            finish_identity_before == self.incarnation_before == finish_identity_after and
                            after.get('membership') == self.before.get('membership') and
                            bool(after.get('cgroup_ancestors')) and
                            after['cgroup_ancestors'][0]['path'] == str(self.leaf))
        if not same_incarnation:
            status = 'FAIL'
        for label, target in self.targets.items():
            try:
                current = require_arm(target['path'])
                if any(current[key] != target[key] for key in ('device','inode','bytes','sha256')):
                    status = 'FAIL'
                    self.unavailable.append({'target': label, 'reason': 'ARM artifact changed during execution'})
            except (OSError, RuntimeError) as error:
                status = 'FAIL'
                self.unavailable.append({'target': label, 'reason': str(error)})
        valid_counters, deltas = cgroup_oom_deltas(self.before, after, same_incarnation)
        if not valid_counters:
            status = 'FAIL'
        if after.get('status') != 'PASS':
            status = 'FAIL'
        if self.fatal_errors:
            status = 'FAIL'
        owned = [row['process'].pid for row in self.processes if row['process'].poll() is None]
        if owned:
            status = 'FAIL'
        result = {'status': status, 'scope': 'ARM ROS behavior under QEMU user; kernel executable architecture is reported separately',
                  'cgroup_before': self.before, 'cgroup_after': after,
                  'cgroup_incarnation_before': getattr(self, 'incarnation_before', None),
                  'cgroup_incarnation_after_before_probe': finish_identity_before,
                  'cgroup_incarnation_after': finish_identity_after,
                  'same_cgroup_incarnation': same_incarnation, 'cgroup_incarnation_error': identity_error,
                  'oom_counter_comparison': 'SAME_INCARNATION' if same_incarnation else 'NOT_COMPARED_CHANGED_OR_UNPROVEN_INCARNATION',
                  'cgroup_ancestor_event_deltas': deltas, 'arm_targets': self.targets,
                  'actual_process_samples': list(self.samples.values()),
                  'sampler_thread_stopped':sampler_stopped, 'mapping_descriptors_closed':sampler_stopped,
                  'unavailable_samples': self.unavailable, 'remaining_owned_processes': owned,
                  'fatal_errors': self.fatal_errors, 'expected_host_emulator_sha256': sorted(self.emulator_shas),
                  'commands': [{key: value for key, value in row.items() if key != 'process'}
                               for row in self.processes], **(extra or {})}
        # Caller metadata may label scope, but cannot replace this mandatory
        # verdict or turn missing/replaced-incarnation evidence into success.
        result.update(status=status, same_cgroup_incarnation=same_incarnation,
                      cgroup_incarnation_before=getattr(self, 'incarnation_before', None),
                      cgroup_incarnation_after_before_probe=finish_identity_before,
                      cgroup_incarnation_after=finish_identity_after,
                      cgroup_incarnation_error=identity_error, cgroup_ancestor_event_deltas=deltas,
                      oom_counter_comparison='SAME_INCARNATION' if same_incarnation else
                          'NOT_COMPARED_CHANGED_OR_UNPROVEN_INCARNATION')
        (self.out/'architecture-evidence.json').write_text(json.dumps(result, indent=2)+'\n')
        return result
