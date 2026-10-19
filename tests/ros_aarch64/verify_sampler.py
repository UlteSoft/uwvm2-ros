#!/usr/bin/env python3
"""Bounded real ARM/QEMU exit-race stress plus labeled fake-proc rejection models.

No real cgroup membership, memory limit, or binfmt registration is changed.
SIGSTOP barriers establish strict live ARM positives before each short main
exits; raw fast host children and ARM fork children exercise disposal races.
"""
import argparse
import collections
import importlib.util
import json
import os
from pathlib import Path
import shlex
import signal
import subprocess
import sys
import threading
import time
import traceback

sys.dont_write_bytecode = True
import runtime
from runtime import Evidence, digest, require_arm, validate_mapping_receipts


HELLO = r'''#include <signal.h>
#include <unistd.h>
int main() {
    if (raise(SIGSTOP)) return 1;
    static char const text[] = "ARM_HELLO\n";
    return write(1, text, sizeof(text)-1) == sizeof(text)-1 ? 0 : 2;
}
'''
THREAD_FORK = r'''#include <atomic>
#include <cstdio>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
static std::atomic<unsigned> count{};
static void* worker(void*) {
    for (unsigned i=0;i!=10000;++i) count.fetch_add(1,std::memory_order_relaxed);
    return nullptr;
}
int main() {
    if (raise(SIGSTOP)) return 1;
    pthread_t threads[4];
    for (auto& thread:threads) if (pthread_create(&thread,nullptr,worker,nullptr)) return 2;
    for (auto thread:threads) if (pthread_join(thread,nullptr)) return 3;
    if (count.load()!=40000) return 4;
    // All threads are joined before fork; no post-fork C++ lock ambiguity.
    for (unsigned i=0;i!=12;++i) {
        auto child=fork();if(child<0)return 5;if(!child)_exit(0);
        int status{};if(waitpid(child,&status,0)!=child||!WIFEXITED(status)||WEXITSTATUS(status))return 6;
    }
    auto held=fork();if(held<0)return 7;
    if(!held) { if(raise(SIGSTOP))_exit(8);_exit(0); }
    int stopped{};
    if(waitpid(held,&stopped,WUNTRACED)!=held||!WIFSTOPPED(stopped))return 9;
    std::printf("FORK_BARRIER child=%ld threads=4 count=40000 rapid_forks=12\n",static_cast<long>(held));
    std::fflush(stdout);
    int final{};
    if(waitpid(held,&final,0)!=held||!WIFEXITED(final)||WEXITSTATUS(final))return 10;
    std::puts("PASS ARM_THREAD_FORK threads=4 count=40000 rapid_forks=12 held_fork=1");
    return 0;
}
'''


def save(path, value):
    Path(path).write_text(json.dumps(value, indent=2)+'\n')


def state(pid):
    return Path('/proc', str(pid), 'stat').read_text().rsplit(')', 1)[1].split()[0]


def exact_positive(evidence, name, root_pid, target, child_pid=None):
    selected = [row for row in evidence.samples.values()
                if row['name'] == name and row['ancestry'][0] == root_pid
                and target in row['mapped_arm_targets']
                and validate_mapping_receipts(row, target, evidence.targets[target])
                and (child_pid is None or row['pid'] == child_pid)]
    if not selected:
        raise RuntimeError('FAIL missing actual positive mapping in required PID subtree: '+name)
    for row in selected:
        if not row['live_cgroup_procs'] or row['membership'] != evidence.before['membership'] or row['kernel_exe_elf']['e_machine'] != 62 or row['kernel_exe_elf']['sha256'] not in evidence.emulator_shas:
            raise RuntimeError('FAIL positive sample lost strict cgroup/known-host-QEMU proof')
    return selected[0]


def fake_proc_models(out):
    """Execute observe/finish on controlled files, never real cgroup mutation."""
    root = Path(out);root.mkdir()
    original_path = runtime.Path
    rows = []
    cases = [
        ('genuine-mismatch-live', '/other', '/other', True, 'S', 123, True),
        ('genuine-mismatch-not-listed', '/other', '/other', False, 'S', 123, True),
        ('mismatch-during-resample', '/expected', '/other', False, 'S', 123, True),
        ('same-membership-not-listed', '/expected', '/expected', False, 'S', 123, False),
        ('mismatch-zombie', '/other', '/other', False, 'Z', 123, False),
        ('mismatch-pid-reused', '/other', '/other', False, 'S', 124, False),
    ]
    for label, first_group, newest_group, listed, newest_state, newest_birth, expect_fatal in cases:
        base = root/label;proc = base/'proc/424242';(proc/'task/424242').mkdir(parents=True)
        (proc/'task/424242/children').write_text('')
        cg = base/'cgroup-fixture';cg.mkdir();(cg/'cgroup.procs').write_text('424242\n' if listed else '')
        reads = {'stat': 0, 'cgroup': 0}

        def stat_text(which_state, birth):
            fields = [which_state, '1', *(['0']*18)];fields[19] = str(birth)
            return '424242 (MODELED_PROC_FIXTURE) '+' '.join(fields)+'\n'

        class ModelPath:
            def __init__(self, value):self.real = Path(value)
            def __truediv__(self, other):return ModelPath(self.real/other)
            def read_text(self, *args, **kwargs):
                if self.real == proc/'stat':
                    reads['stat'] += 1
                    return stat_text('S', 123) if reads['stat'] == 1 else stat_text(newest_state, newest_birth)
                if self.real == proc/'cgroup':
                    reads['cgroup'] += 1
                    return '0::'+(first_group if reads['cgroup'] == 1 else newest_group)+'\n'
                return self.real.read_text(*args, **kwargs)
            def __getattr__(self, key):return getattr(self.real, key)
            def __str__(self):return str(self.real)

        def path_factory(*parts):
            if parts and str(parts[0]) == '/proc':return ModelPath(base/'proc').joinpath(*parts[1:]) if len(parts)>1 else ModelPath(base/'proc')
            return original_path(*parts)

        facts = {'status': 'PASS', 'membership': ['0::/expected'],
                 'fixture_only_not_real_cgroup': True,
                 'cgroup_ancestors': [{'path': str(cg), 'memory.max': '68719476736',
                                       'memory.events': 'oom 0\noom_kill 0\noom_group_kill 0'}]}
        model = object.__new__(Evidence)
        model.out = base;model.probe = lambda: facts;model.before = facts;model.leaf = cg
        model._incarnation = lambda: {'path': str(cg), 'device': cg.stat().st_dev, 'inode': cg.stat().st_ino,
                                     'boot_id': 'SYNTHETIC_PROC_MODEL_ONLY'}
        model.incarnation_before = model._incarnation()
        model.targets = {};model.processes = [];model.samples = {};model.unavailable = []
        model.fatal_errors = [];model.emulator_shas = set();model._elf_cache = {}
        model._lock = threading.RLock();model._stop = threading.Event()
        model.thread = threading.Thread(target=lambda: None);model.thread.start()
        try:
            runtime.Path = path_factory
            model.observe(424242, 'MODELED/'+label)
        finally:
            runtime.Path = original_path
        if bool(model.fatal_errors) != expect_fatal or model.samples:
            raise RuntimeError('FAIL fake-proc rejection/skip branch: '+label)
        if label == 'same-membership-not-listed' and not any('absent from cgroup.procs' in row.get('reason', '') for row in model.unavailable):
            raise RuntimeError('FAIL same-membership missing PID was not recorded as unavailable')
        result = model.finish('PASS', {'scope': 'MODELED_PROC_FIXTURE_ONLY_NO_REAL_EXECUTION',
                                      'synthetic_fixture': True, 'model_case': label})
        if result['status'] != ('FAIL' if expect_fatal else 'PASS'):
            raise RuntimeError('FAIL fatal model did not control finish status: '+label)
        row = {'name': label, 'validation_status': 'PASS', 'synthetic_fixture': True,
               'expected_fatal': expect_fatal, 'model_finish_status': result['status'],
               'fatal_errors': model.fatal_errors, 'unavailable_samples': model.unavailable,
               'positive_samples': len(model.samples), 'reads': reads,
               'fixture_first_membership': first_group, 'fixture_latest_membership': newest_group,
               'fixture_listed': listed, 'latest_state': newest_state, 'latest_birth': newest_birth}
        rows.append(row)
    save(root/'models.json', {'status': 'PASS_MODEL_VALIDATION_ONLY', 'cases': rows,
                             'limits': 'No real process moved between cgroups and no memory/cgroup limit written; no modeled record is actual ELF execution proof'})
    return rows


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'config', 'out'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--expected-runtime-sha256', required=True)
    args = parser.parse_args()
    runtime_path = Path(runtime.__file__).resolve()
    if digest(runtime_path) != args.expected_runtime_sha256:
        raise RuntimeError('FAIL sampler differs from selected current runtime SHA')
    source = args.source.resolve();config = json.loads(args.config.read_text())
    spec = importlib.util.spec_from_file_location('sampler_cgroup', source/'tests/ros_cgroup/probe.py')
    module = importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    evidence = Evidence(args.out.resolve(), module.probe, interval=.001)
    out = evidence.out;environment = dict(os.environ);environment.update(config.get('environment', {}))
    environment.pop('LD_PRELOAD', None);environment.pop('QEMU_LD_PREFIX', None)
    evidence.emulator(config['qemu_argv'][0])
    rows, children, builds, model_rows = [], [], [], []
    selected_inputs = {'driver_sha256': digest(__file__), 'runtime_sha256': digest(runtime_path),
                       'config_sha256': digest(args.config), 'compiler_sha256': digest(config['compiler'][0]),
                       'qemu_sha256': digest(config['qemu_argv'][0]),
                       'probe_driver_sha256': digest(source/'tests/ros_cgroup/probe.py')}
    status, error, actual_result = 'FAIL', None, None

    def launch(command, name, stdin=False):
        log = out/(name.rstrip('/').replace('/', '_')+'.log')
        with log.open('xb') as stream:
            child = subprocess.Popen(command, stdin=subprocess.PIPE if stdin else None,
                                     stdout=stream, stderr=subprocess.STDOUT,
                                     env=environment, start_new_session=True)
        children.append(child);evidence.start(child, name, command)
        return child, log

    def wait_stop(child, name, target):
        deadline = time.monotonic()+10
        while child.poll() is None and time.monotonic() < deadline:
            try:
                if state(child.pid) in ('T', 't'):
                    evidence.observe(child.pid, name);evidence.check([target], name)
                    return exact_positive(evidence, name, child.pid, target)
            except FileNotFoundError:pass
            time.sleep(.001)
        raise RuntimeError('FAIL ARM positive barrier missing: '+name)

    def terminate(signum, frame):
        raise RuntimeError('FAIL interrupted bounded sampler verification: '+str(signum))

    signal.signal(signal.SIGTERM, terminate);signal.signal(signal.SIGINT, terminate)
    try:
        for label, text in (('arm-hello', HELLO), ('arm-thread-fork', THREAD_FORK)):
            cpp = out/(label+'.cc');dep = out/(label+'.d');binary = out/label;cpp.write_text(text)
            command = [*config['compiler'], *config['compile_flags'], '-std=c++20', '-O1', '-g0',
                       '-pthread', '-MMD', '-MF', str(dep), str(cpp), '-o', str(binary),
                       *config.get('link_flags', [])]
            child, log = launch(command, 'compile-'+label+'/')
            code = child.wait(timeout=120)
            if code:raise RuntimeError('FAIL real cross compiler: '+label)
            tokens = shlex.split(dep.read_text().replace('\\\n', ' ').split(':', 1)[1])
            dependencies = {str(Path(token).resolve()): digest(Path(token).resolve()) for token in tokens}
            if dependencies.get(str(cpp)) != digest(cpp):raise RuntimeError('FAIL MMD does not bind generated stress source')
            evidence.target(label, binary)
            builds.append({'name': label, 'returncode': code, 'command': command,
                           'binary': require_arm(binary), 'source': {'path': str(cpp), 'sha256': digest(cpp)},
                           'MMD': {'path': str(dep), 'sha256': digest(dep), 'inputs': dependencies},
                           'log_sha256': digest(log)})
        # Explicit QEMU never needs an ARM binfmt handler for these programs.
        for index in range(100):
            name = f'arm-hello-{index}/';command = [*config['qemu_argv'], str(out/'arm-hello')]
            child, log = launch(command, name)
            positive = wait_stop(child, name, 'arm-hello')
            os.kill(child.pid, signal.SIGCONT)
            code = child.wait(timeout=10);evidence.observe(child.pid, name)
            if code or log.read_text() != 'ARM_HELLO\n':raise RuntimeError('FAIL short ARM hello execution')
            rows.append({'name': name, 'status': 'PASS', 'kind': 'actual-ARM-short-main-after-positive-barrier',
                         'pid': child.pid, 'returncode': code, 'positive_capture_ns': positive['capture_monotonic_ns'],
                         'log_sha256': digest(log)})
        for index in range(100):
            name = f'host-fast-{index}/';child, log = launch(['/usr/bin/true'], name)
            # Repeated synchronous samples bracket disposal; missing samples
            # never count as ARM positives and normal exit never proves departure.
            evidence.observe(child.pid, name);code = child.wait(timeout=5);evidence.observe(child.pid, name)
            if code:raise RuntimeError('FAIL host fast child')
            rows.append({'name': name, 'status': 'PASS', 'kind': 'actual-host-fast-exit-only',
                         'pid': child.pid, 'returncode': code, 'log_sha256': digest(log)})
        for index in range(20):
            name = f'arm-thread-fork-{index}/';command = [*config['qemu_argv'], str(out/'arm-thread-fork')]
            child, log = launch(command, name)
            positive = wait_stop(child, name, 'arm-thread-fork');os.kill(child.pid, signal.SIGCONT)
            deadline = time.monotonic()+15;held = None
            while child.poll() is None and time.monotonic() < deadline:
                evidence.observe(child.pid, name)
                if 'FORK_BARRIER ' in log.read_text():
                    for pid in map(int, Path('/proc', str(child.pid), 'task', str(child.pid), 'children').read_text().split()):
                        if state(pid) in ('T', 't'):held = pid;break
                    if held is not None:break
                time.sleep(.001)
            if held is None:raise RuntimeError('FAIL actual ARM held fork child not observed')
            evidence.observe(child.pid, name)
            child_positive = exact_positive(evidence, name, child.pid, 'arm-thread-fork', held)
            os.kill(held, signal.SIGCONT)
            code = child.wait(timeout=10);evidence.observe(child.pid, name)
            if code or 'PASS ARM_THREAD_FORK threads=4 count=40000 rapid_forks=12 held_fork=1' not in log.read_text():
                raise RuntimeError('FAIL actual ARM pthread/fork behavior')
            rows.append({'name': name, 'status': 'PASS', 'kind': 'actual-ARM-pthread-fork', 'pid': child.pid,
                         'held_child_pid': held, 'returncode': code, 'threads': 4, 'atomic_count': 40000,
                         'rapid_forks': 12, 'held_forks': 1, 'positive_capture_ns': positive['capture_monotonic_ns'],
                         'child_positive_capture_ns': child_positive['capture_monotonic_ns'], 'log_sha256': digest(log)})
        if any(digest(Path(path)) != value for path, value in ((runtime_path, selected_inputs['runtime_sha256']),
               (args.config, selected_inputs['config_sha256']), (config['compiler'][0], selected_inputs['compiler_sha256']),
               (config['qemu_argv'][0], selected_inputs['qemu_sha256']))):
            raise RuntimeError('FAIL selected runtime/config/compiler/QEMU changed')
        status = 'PASS'
    except BaseException:
        error = traceback.format_exc()
    finally:
        for child in reversed(children):
            if child.poll() is None:
                try:os.killpg(child.pid, signal.SIGKILL)
                except ProcessLookupError:pass
                child.wait(timeout=5)
        actual_result = evidence.finish(status, {'scope': 'ACTUAL_ARM_QEMU_SAMPLER_STRESS_ONLY_NO_ROS_BEHAVIOR',
            'selected_inputs': selected_inputs, 'builds': builds, 'cases': rows, 'error': error,
            'hello_positive_barrier': 'SIGSTOP before short main; no claim all arbitrary fast programs are sampled',
            'unavailable_record_capacity': 100})
    try:
        # Actual sampler thread has finished before replacing its Path symbol.
        model_rows = fake_proc_models(out/'models')
    except BaseException:
        status = 'FAIL';error = (error or '')+'\n'+traceback.format_exc()
    status = 'PASS' if actual_result['status'] == 'PASS' and len(rows) == 220 and len(model_rows) == 6 else 'FAIL'
    counts = collections.Counter(row['kind'] for row in rows)
    unavailable = actual_result['unavailable_samples']
    report = {'status': status, 'scope': 'Real ARM/host stress and separate controlled fake-proc branch validation',
              'selected_inputs': selected_inputs, 'actual_case_counts': dict(counts),
              'actual_ARM_required_positive_launches': 120,
              'actual_ARM_required_positive_held_fork_children': 20,
              'actual_recorded_not_listed_skips': sum('absent from cgroup.procs' in row.get('reason', '') for row in unavailable),
              'actual_unavailable_records': len(unavailable), 'unavailable_capacity': 100,
              'skip_count_scope': 'Recorded bounded unavailable entries; zero does not mean the kernel race never occurs',
              'actual_fatal_errors': actual_result['fatal_errors'], 'models': model_rows, 'error': error,
              'actual_architecture_evidence_sha256': digest(out/'architecture-evidence.json')}
    save(out/'report.json', report)
    print(status, out/'report.json', flush=True)
    return 0 if status == 'PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
