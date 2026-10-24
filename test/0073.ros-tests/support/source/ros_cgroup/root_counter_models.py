#!/usr/bin/env python3
"""Test controllerless hierarchy-root qualification using pure filesystem models.

The actual probe and shared counter function are extracted from current source.
All /proc, mountinfo, cgroup files and directory stats are synthetic; no live
cgroup is read or changed. Preserved historical JSON supplies only the known
root-null shape and a raw mountinfo example, never current execution authority.
No compiler, native binary, QEMU, Wasm, Guest or ROS process is started.
"""
import argparse
import ast
import copy
import errno
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import stat
import sys
import types

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[5]
PROBE = ROOT / 'test/0073.ros-tests/support/source/ros_cgroup/probe.py'
RUNTIME = ROOT / 'test/0073.ros-tests/support/source/ros_aarch64/runtime.py'
HISTORICAL = ROOT / 'documents/ros/continuation-20261013-validation.json'


def functions(path, names, scope):
    """Load only named current functions; module main/constructors never run."""
    nodes = [node for node in ast.parse(path.read_bytes()).body
             if isinstance(node, ast.FunctionDef) and node.name in names]
    assert {node.name for node in nodes} == set(names)
    exec(compile(ast.Module(body=nodes, type_ignores=[]), str(path) + '<pure-root-model>', 'exec'), scope)


class Filesystem:
    """Expose a fully synthetic cgroup2 mount and known required read errors."""
    root = '/sys/fs/cgroup'
    parent = root + '/system.slice'
    leaf = parent + '/test.scope'
    mountinfo = '1244 1242 0:29 / /sys/fs/cgroup rw,nosuid,nodev,noexec - cgroup2 cgroup2 rw'

    def __init__(self):
        self.files = {'/proc/self/cgroup': '0::/system.slice/test.scope\n',
                      '/proc/self/mountinfo': self.mountinfo + '\n',
                      '/etc/os-release': 'ID=synthetic\n', '/proc/meminfo': 'Synthetic: 0\n',
                      '/proc/sys/kernel/random/boot_id': 'synthetic-current-64-boot'}
        for path, limit in ((self.leaf, '68719476736'), (self.parent, 'max'), (self.root, None)):
            self.files[path + '/cgroup.procs'] = '123\n'
            if limit is not None:
                self.files[path + '/memory.max'] = limit
                self.files[path + '/memory.current'] = '0'
                self.files[path + '/memory.events'] = 'oom 0\noom_kill 0\noom_group_kill 0'
                self.files[path + '/memory.swap.max'] = '0'
                self.files[path + '/cpuset.cpus.effective'] = '0-31'
        self.errors, self.reads, self.stats = {}, [], []
        self.identity = dict(st_dev=os.makedev(0, 29), st_ino=1, st_mode=stat.S_IFDIR | 0o755)
        self.after_identity = self.identity

    def path_type(self):
        model = self

        class FakePath:
            """String/path arithmetic plus fake read/stat; no real filesystem API."""
            def __init__(self, path): self.value = str(PurePosixPath(str(path)))
            def __str__(self): return self.value
            def __truediv__(self, name): return FakePath(PurePosixPath(self.value) / str(name))
            @property
            def parent(self): return FakePath(PurePosixPath(self.value).parent)
            def __eq__(self, other): return isinstance(other, FakePath) and self.value == other.value
            def exists(self): return self.value in model.files
            def read_text(self):
                model.reads.append(self.value)
                code = model.errors.get(self.value)
                if code is not None: raise OSError(code, 'synthetic read error', self.value)
                if self.value not in model.files: raise FileNotFoundError(errno.ENOENT, 'synthetic absent file', self.value)
                return model.files[self.value]
            def stat(self):
                if self.value == model.leaf:
                    return types.SimpleNamespace(st_dev=os.makedev(0, 29), st_ino=18927, st_mode=stat.S_IFDIR | 0o755)
                assert self.value == model.root, 'Only actual root qualification needs directory stat'
                model.stats.append(self.value)
                identity = model.identity if len(model.stats) == 1 else model.after_identity
                if isinstance(identity, OSError): raise identity
                return types.SimpleNamespace(**identity)
        return FakePath

    def probe(self):
        scope = {'Path': self.path_type(), 'errno': errno, 'stat': stat,
                 'os': types.SimpleNamespace(getpid=lambda: 123, cpu_count=lambda: 32,
                                             major=os.major, minor=os.minor),
                 'platform': types.SimpleNamespace(system=lambda: 'Linux',
                     uname=lambda: types.SimpleNamespace(_asdict=lambda: {'model': True}))}
        functions(PROBE, ('controllerless_root', 'probe'), scope)
        return scope['probe']()


def historical_shape(value):
    """Locate one preserved ancestor list with the actual host's root-null shape."""
    if isinstance(value, dict):
        ancestors = value.get('cgroup_ancestors')
        if isinstance(ancestors, list) and any(row.get('path') == '/sys/fs/cgroup' and row.get('memory.events') is None for row in ancestors):
            return value
        children = value.values()
    elif isinstance(value, list):
        children = value
    else:
        return None
    for child in children:
        found = historical_shape(child)
        if found is not None:
            return found
    return None


def main():
    """Run current-source models and optionally write a new evidence revision."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    if args.report and args.report.exists(): parser.error('report already exists')
    source_inputs = {str(path.relative_to(ROOT)): hashlib.sha256(path.read_bytes()).hexdigest()
                     for path in (PROBE, RUNTIME, HISTORICAL)}
    scope = {'os': os}
    functions(RUNTIME, ('cgroup_oom_deltas',), scope)
    compare = scope['cgroup_oom_deltas']
    rows = []

    def result(name, before, after=None, expected=False, same=True):
        valid, deltas = compare(before, copy.deepcopy(before) if after is None else after, same)
        assert valid is expected, (name, valid, deltas)
        if not same: assert deltas == [], name
        rows.append({'name': name, 'status': 'PASS', 'admitted': valid, 'compared_rows': len(deltas)})
        return deltas

    baseline = Filesystem().probe()
    assert baseline['status'] == 'PASS'
    root = baseline['cgroup_ancestors'][-1]
    qualification = root['controllerless_hierarchy_root']
    assert qualification['mount']['root'] == '/' and qualification['missing_files'] == {
        name: 'ENOENT' for name in ('memory.max', 'memory.current', 'memory.events')}
    deltas = result('qualified unchanged controllerless root coexists with real zero counters', baseline, expected=True)
    assert deltas[-1]['comparison'] == 'NOT_APPLICABLE_QUALIFIED_CONTROLLERLESS_HIERARCHY_ROOT'
    assert all(deltas[index]['oom'] == 0 for index in (0, 1))
    changed = copy.deepcopy(baseline)
    changed['cgroup_ancestors'][-1]['controllerless_hierarchy_root']['mount']['raw_mountinfo'] = (
        Filesystem.mountinfo.replace(' - ', ' shared:8 master:6 - '))
    result('optional propagation tags may change without replacing the root', baseline, changed, expected=True)
    changed = copy.deepcopy(baseline)
    mount = changed['cgroup_ancestors'][-1]['controllerless_hierarchy_root']['mount']
    mount['mount_id'] = '1245'
    mount['raw_mountinfo'] = Filesystem.mountinfo.replace('1244 ', '1245 ', 1)
    result('changed mount identity fails even with a consistent raw record', baseline, changed)
    historical = historical_shape(json.loads(HISTORICAL.read_text()))
    assert historical is not None
    result('historical null root is never silently upgraded to current qualification', historical)
    for file in ('memory.max', 'memory.current', 'memory.events'):
        for code in (errno.EACCES, errno.EIO):
            fs = Filesystem(); fs.errors[fs.root + '/' + file] = code
            facts = fs.probe()
            assert 'controllerless_hierarchy_root' not in facts['cgroup_ancestors'][-1]
            result(file + ' unreadable root rejected errno=' + str(code), facts)
    for label, identity in [('changed-inode', {**Filesystem().identity, 'st_ino': 2}),
                            ('changed-device', {**Filesystem().identity, 'st_dev': os.makedev(0, 30)}),
                            ('not-directory', {**Filesystem().identity, 'st_mode': stat.S_IFREG}),
                            ('stat-unavailable', OSError(errno.EACCES, 'synthetic root stat failure'))]:
        fs = Filesystem(); fs.after_identity = identity
        result('root ' + label + ' fails qualification', fs.probe())
    fs = Filesystem(); fs.files['/proc/self/mountinfo'] = fs.mountinfo.replace('0:29', '0:30') + '\n'
    result('mountinfo device mismatch fails qualification', fs.probe())
    fs = Filesystem(); fs.files['/proc/self/mountinfo'] = fs.mountinfo + '\n' + fs.mountinfo + '\n'
    result('ambiguous root mounts fail qualification', fs.probe())
    fs = Filesystem(); fs.files[fs.root + '/memory.max'] = 'max'
    result('partly present root controller files are never skipped', fs.probe())
    for missing in (Filesystem.leaf, Filesystem.parent):
        fs = Filesystem(); del fs.files[missing + '/memory.events']
        result('missing nonroot counters fail: ' + missing, fs.probe())
    for field in ('device', 'inode', 'path'):
        changed = copy.deepcopy(baseline)
        q = changed['cgroup_ancestors'][-1]['controllerless_hierarchy_root']
        q[field] = q[field] + 1 if isinstance(q[field], int) else q[field] + '/other'
        result('root qualification ' + field + ' changed after run', baseline, changed)
    for label, mutation in [
            ('absent qualification', lambda row: row.pop('controllerless_hierarchy_root')),
            ('non-ENOENT missing event', lambda row: row['controllerless_hierarchy_root']['missing_files'].update({'memory.events': 'EACCES'})),
            ('wrong mount root', lambda row: row['controllerless_hierarchy_root']['mount'].update({'root': '/system.slice'})),
            ('wrong raw filesystem', lambda row: row['controllerless_hierarchy_root']['mount'].update({'raw_mountinfo': Filesystem.mountinfo.replace('cgroup2', 'tmpfs')})),
            ('wrong raw root', lambda row: row['controllerless_hierarchy_root']['mount'].update({'raw_mountinfo': Filesystem.mountinfo.replace('0:29 / ', '0:29 /system.slice ')})),
            ('missing actual required field', lambda row: row.pop('memory.current'))]:
        changed = copy.deepcopy(baseline); mutation(changed['cgroup_ancestors'][-1])
        result(label + ' is not a qualified root', changed)
    changed = copy.deepcopy(baseline)
    changed['cgroup_ancestors'][0]['controllerless_hierarchy_root'] = copy.deepcopy(qualification)
    changed['cgroup_ancestors'][0]['memory.events'] = None
    result('copied root metadata cannot qualify a nonroot path', changed)
    for index in (0, 1):
        for value in (1, -1):
            changed = copy.deepcopy(baseline)
            changed['cgroup_ancestors'][index]['memory.events'] = f'oom {value}\noom_kill 0\noom_group_kill 0'
            result('real counter increase/reset remains rejected '+str((index, value)), baseline, changed)
    only_root = {'cgroup_ancestors': [copy.deepcopy(root)]}
    result('a qualified root alone cannot form a vacuous counter pass', only_root)
    result('different incarnation never subtracts counters', baseline, same=False)
    changed = copy.deepcopy(baseline); changed['cgroup_ancestors'][0]['memory.events'] = 'oom 0\noom_kill 0'
    result('missing required real OOM key remains rejected', changed)
    for path in (PROBE, RUNTIME, HISTORICAL):
        assert hashlib.sha256(path.read_bytes()).hexdigest() == source_inputs[str(path.relative_to(ROOT))]
    report = {'status': 'SOURCE_ROOT_COUNTER_MODEL_PASS', 'checks': len(rows), 'rows': rows,
              'source_inputs': source_inputs, 'scope': 'Current extracted probe/counter helpers with synthetic filesystem and preserved JSON shape only',
              'historical_root_remains_unqualified': True, 'actual_live_cgroup': 'NOT_PERFORMED',
              'actual_compiler_native_QEMU_Wasm_ROS_execution': 'NOT_PERFORMED'}
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        with args.report.open('x') as output: output.write(json.dumps(report, indent=2) + '\n')
    print(json.dumps({key: value for key, value in report.items() if key != 'rows'}))
    return 0


if __name__ == '__main__':
    sys.exit(main())
