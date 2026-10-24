"""Finite Linux process owner shared by actual portable-language native tests.

All authority is supplied explicitly by independently generated policy. This
test helper never changes cgroup limits, installs software, retries an external
effect or runs an archived controller. Root/input/source admission is explicit.
"""
import hashlib
import json
import os
from pathlib import Path
import resource
import signal
import subprocess
import time


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


class Owner:
    # A trusted derived fixture may explicitly choose a finite native compiler
    # budget. The default remains unchanged for all existing language lanes.
    owned_rss_limit = 768 << 20
    def __init__(self, root, inputs, report, result_name):
        self.root = root
        self.inputs = inputs
        self.report = report
        self.result = root / 'out' / result_name
        self.group = Path(inputs['cgroup']['path'])
        self.events_before = (self.group / 'memory.events').read_text()
        self.oom_before = self.oom()
        self.actors = []
        self.report.update(commands=[], protocol_events=[], events_before=self.events_before)
        for name in ('commands', 'home', 'tmp'):
            (root / 'out' / name).mkdir(exist_ok=True)
        (root / 'control').mkdir(exist_ok=True)
        self.held = root / 'control/held-exec-ros2.py'
        self.held.write_text('import os,sys\nfd=int(sys.argv[1])\nif os.read(fd,1)!=b"E":raise SystemExit(125)\nos.close(fd)\nos.execvpe(sys.argv[2],sys.argv[2:],os.environ)\n')
        os.sched_setaffinity(0, {20})
        assert root.is_mount() and root.stat().st_dev != root.parent.stat().st_dev
        self.guard()
        self.save()

    def oom(self):
        return {key: value for key, value in map(str.split, (self.group / 'memory.events').read_text().splitlines()) if key.startswith('oom')}

    def guard(self):
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == self.inputs['boot_id']
        assert Path('/proc/self/cgroup').read_text().strip() == '0::' + self.inputs['cgroup']['membership']
        info = self.group.stat()
        assert [info.st_dev, info.st_ino] == self.inputs['cgroup']['identity']
        assert (self.group / 'memory.max').read_text().strip() == str(64 << 30)
        assert (self.group / 'memory.swap.max').read_text().strip() == '0'
        assert self.oom() == self.oom_before, 'shared OOM/kill counter changed'
        return int((self.group / 'memory.current').read_text())

    def save(self):
        self.result.write_text(json.dumps(self.report, indent=2) + '\n')

    def event(self, language, phase):
        self.report['protocol_events'].append({'language': language, 'phase': phase, 'monotonic_ns': time.monotonic_ns()})
        self.save()

    def admit(self):
        end = time.monotonic() + 60
        while self.guard() >= 61 << 30:
            assert time.monotonic() < end, '3GiB shared admission margin unavailable'
            time.sleep(0.5)

    @staticmethod
    def descendants(pid):
        found, pending = {pid}, [pid]
        while pending:
            parent = pending.pop()
            try:
                values = (Path('/proc') / str(parent) / 'task' / str(parent) / 'children').read_text().split()
            except (FileNotFoundError, ProcessLookupError):
                continue
            for child in map(int, values):
                if child not in found:
                    found.add(child)
                    pending.append(child)
        return found

    @staticmethod
    def rss(pid):
        try:
            return int((Path('/proc') / str(pid) / 'statm').read_text().split()[1]) * os.sysconf('SC_PAGE_SIZE')
        except (FileNotFoundError, ProcessLookupError):
            return 0

    def observe(self):
        assert self.guard() < 63 << 30, 'shared 1GiB running margin'
        total = 0
        for actor in self.actors:
            if actor['process'].poll() is not None:
                continue
            used = 0
            for pid in self.descendants(actor['process'].pid):
                used += self.rss(pid)
                try:
                    assert (Path('/proc') / str(pid) / 'cgroup').read_text().strip() == '0::' + self.inputs['cgroup']['membership']
                except (FileNotFoundError, ProcessLookupError):
                    pass
            total += used
            actor['row']['max_sampled_owned_rss'] = max(actor['row']['max_sampled_owned_rss'], used)
            assert time.monotonic() - actor['started'] < actor['row']['deadline_seconds'], 'actor deadline'
            assert sum((actor['directory'] / name).stat().st_size for name in ('stdout', 'stderr')) <= 8 << 20, 'output bound'
        assert total <= self.owned_rss_limit, 'aggregate sampled owned RSS bound'

    @staticmethod
    def limits():
        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        resource.setrlimit(resource.RLIMIT_AS, (16 << 30, 16 << 30))
        resource.setrlimit(resource.RLIMIT_CPU, (180, 180))
        resource.setrlimit(resource.RLIMIT_FSIZE, (64 << 20, 64 << 20))
        resource.setrlimit(resource.RLIMIT_NOFILE, (1024, 1024))

    def launch(self, label, argv, extra=None, held=False, stdin=False, deadline=45):
        self.guard()
        directory = self.root / 'out/commands' / label
        directory.mkdir()
        env = {'PATH': '/usr/bin:/bin', 'HOME': str(self.root / 'out/home'),
               'TMPDIR': str(self.root / 'out/tmp'), 'LANG': 'C.UTF-8', 'LC_ALL': 'C.UTF-8',
               'OMP_NUM_THREADS': '1', 'OPENBLAS_NUM_THREADS': '1'}
        env.update(extra or {})
        argv = list(map(str, argv))
        read_fd = write_fd = None
        actual = argv
        if held:
            read_fd, write_fd = os.pipe()
            actual = ['/usr/bin/python3', '-I', '-S', '-B', str(self.held), str(read_fd), *argv]
        with (directory / 'stdout').open('xb') as out, (directory / 'stderr').open('xb') as err:
            process = subprocess.Popen(actual, env=env, stdin=subprocess.PIPE if held or stdin else subprocess.DEVNULL,
                stdout=out, stderr=err, start_new_session=True, pass_fds=(read_fd,) if held else (), preexec_fn=self.limits)
        if held:
            os.close(read_fd)
        row = {'label': label, 'argv': argv, 'launch_argv': actual, 'env': env, 'pid': process.pid,
               'exit': None, 'forced_signal': None, 'max_sampled_owned_rss': 0,
               'started_monotonic_ns': time.monotonic_ns(), 'deadline_seconds': deadline}
        actor = {'process': process, 'directory': directory, 'row': row, 'release': write_fd,
                 'pidfd': os.pidfd_open(process.pid), 'started': time.monotonic()}
        self.actors.append(actor)
        self.report['commands'].append(row)
        self.save()
        return actor

    def text(self, actor, stream='stdout'):
        return (actor['directory'] / stream).read_text()

    def marker(self, actor, prefix):
        while True:
            self.observe()
            if any(line.startswith(prefix) for line in self.text(actor).splitlines()):
                assert actor['process'].poll() is None, 'stale marker from an already ended actor'
                return
            assert actor['process'].poll() is None, (actor['row']['label'], prefix, self.text(actor, 'stderr')[-1800:])
            time.sleep(0.02)

    def release(self, actor):
        os.write(actor['release'], b'E')
        os.close(actor['release'])
        actor['release'] = None

    def credit(self, actor, value):
        actor['process'].stdin.write(value)
        actor['process'].stdin.flush()

    def wait(self, actor):
        while actor['process'].poll() is None:
            self.observe()
            time.sleep(0.02)
        actor['row']['exit'] = actor['process'].wait()
        actor['row']['finished_monotonic_ns'] = time.monotonic_ns()
        self.save()
        assert actor['row']['exit'] == 0, (actor['row']['label'], actor['row']['exit'], self.text(actor, 'stderr')[-2200:])

    def maps(self, actor, prefixes):
        raw = (Path('/proc') / str(actor['process'].pid) / 'maps').read_text()
        records = {}
        for line in raw.splitlines():
            fields = line.split(None, 5)
            if len(fields) != 6 or 'x' not in fields[1] or not fields[5].startswith('/'):
                continue
            path = Path(fields[5])
            assert any(path == prefix or path.is_relative_to(prefix) for prefix in prefixes), str(path)
            item = path.stat()
            records[str(path)] = {'identity': [item.st_dev, item.st_ino, item.st_size, item.st_mtime_ns, item.st_ctime_ns]}
        return {'text': raw, 'files': records}

    @staticmethod
    def finish_maps(mapping):
        for role in mapping.values():
            for path, row in role['files'].items():
                info = Path(path).stat()
                assert [info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns] == row['identity']
                row['sha256'] = sha(path)

    def close(self):
        for actor in reversed(self.actors):
            if actor['release'] is not None:
                os.close(actor['release'])
            if actor['process'].poll() is None:
                try:
                    os.killpg(actor['process'].pid, signal.SIGKILL)
                except ProcessLookupError:
                    pass
                actor['row']['forced_signal'] = 'SIGKILL owned process group after failed or expired fixture'
            actor['row']['exit'] = actor['process'].wait()
            os.close(actor['pidfd'])
            if actor['process'].stdin is not None and not actor['process'].stdin.closed:
                actor['process'].stdin.close()
            for stream in ('stdout', 'stderr'):
                actor['row'][stream + '_sha256'] = sha(actor['directory'] / stream)
        self.report['events_after'] = (self.group / 'memory.events').read_text()
        self.save()
