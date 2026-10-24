#!/usr/bin/env python3
"""Prepare or run a serial SDK compiler/import/full-validator matrix in Linux64GiB.

Default mode only emits recipes. --run requires an explicitly pinned live cgroup
leaf, existing toolchains and a fresh output directory. No guest, Worker, Gateway,
ROS node or JavaScript host application is executed. Compiler success, binary ROS
import checking and full Wasm validation remain separate recorded steps.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path
from owner_guards import bind_arguments as _bind_owner_arguments, snapshot as _owner_snapshot, accounted_memory as _owner_memory

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import platform
import shutil
import signal
import stat
import struct
import subprocess
import sys
import time
import tomllib

sys.dont_write_bytecode = True
SDK = _CHECKOUT_ROOT / 'sdk/origin'
ROOT = _CHECKOUT_ROOT
LIMIT = 68719476736
LIMIT_MIB = 65536
DEFAULT_CURRENT_LIMIT_MIB = 61440
HELD_EXEC = "import json,os,signal,sys; a=json.loads(sys.argv[1]); os.kill(os.getpid(),signal.SIGSTOP); os.execvpe(a[0],a,os.environ)"


def load_python(name, path):
    """Read the existing repository probe/inspector, without generating caches."""
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


PROBE = load_python('roswasm_existing_cgroup_probe', ROOT / 'test/0073.ros-tests/support/source/ros_cgroup/probe.py')
INSPECTOR = load_python('roswasm_binary_import_checker', _sdk_path(SDK, 'tools/check_wasm_imports.py'))


def digest(path):
    """Hash file bytes in bounded chunks, including compiler and Wasm artifacts."""
    result = hashlib.sha256()
    with Path(path).open('rb') as stream:
        while block := stream.read(1024 * 1024):
            result.update(block)
    return result.hexdigest()


def rust_wasi_link_inputs(wasi_sdk, rust_sysroot=None, coverage=False):
    """Select the target C archive explicitly, including for no_std cdylibs.

    Rust may lower slice operations to compiler memory intrinsics such as
    memcmp. no_std does not supply their C definitions. Use exactly the selected
    WASIp1 archive, never a host -lc lookup or a handwritten implementation.
    Return individual rustc tokens for CARGO_ENCODED_RUSTFLAGS so an absolute
    package path containing spaces remains one linker operand. This adds no
    WASI permission; the unchanged binary inspector still rejects env imports.
    """
    if wasi_sdk is None or not Path(wasi_sdk).is_absolute():
        raise ValueError('Rust Wasm compilation requires explicit absolute --wasi-sdk')
    root = Path(wasi_sdk).resolve(strict=True)
    archive = root / 'share/wasi-sysroot/lib/wasm32-wasip1/libc.a'
    if archive.resolve(strict=True) != archive:
        raise ValueError('selected WASIp1 libc archive must have a physical non-aliased path')
    fd = os.open(archive, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW)
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or not 8 <= before.st_size <= 64 * 1024**2:
            raise ValueError('selected WASIp1 libc must be a bounded regular archive')
        value, count, header = hashlib.sha256(), 0, b''
        while count <= before.st_size:
            block = os.read(fd, min(1024**2, before.st_size + 1 - count))
            if not block:
                break
            if not header:
                header = block[:8]
            count += len(block); value.update(block)
        after = os.fstat(fd); current = archive.stat(follow_symlinks=False)
        fields = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
        if (header != b'!<arch>\n' or count != before.st_size or
                any(getattr(before, key) != getattr(after, key) or
                    getattr(before, key) != getattr(current, key) for key in fields)):
            raise ValueError('selected WASIp1 libc archive changed or has invalid framing')
    finally:
        os.close(fd)
    binding = {'path': str(archive), 'device': after.st_dev, 'inode': after.st_ino,
               'bytes': count, 'mtime_ns': after.st_mtime_ns, 'ctime_ns': after.st_ctime_ns,
               'sha256': value.hexdigest()}
    flags = ([] if rust_sysroot is None else ['--sysroot=' + str(rust_sysroot)])
    flags += ['-C', 'codegen-units=1', '-C', 'link-arg=--threads=1',
              '-C', 'link-arg=' + str(archive)]
    if coverage:
        flags += ['-C', 'link-arg=--export=roswasm_abi_coverage']
    return flags, binding


def manifest():
    """Bind every SDK regular file/symlink; output and compiler caches live elsewhere."""
    files = []
    for path in sorted(SDK.rglob('*')):
        if path.is_symlink():
            target = os.readlink(path)
            files.append({'path': str(path.relative_to(SDK)), 'kind': 'symlink', 'target': target})
        elif path.is_file():
            files.append({'path': str(path.relative_to(SDK)), 'kind': 'file', 'size': path.stat().st_size, 'sha256': digest(path)})
    encoded = json.dumps(files, sort_keys=True, separators=(',', ':')).encode()
    return {'sha256': hashlib.sha256(encoded).hexdigest(), 'files': files}


class CommandGuardFailure(RuntimeError):
    """An unsafe/unprovable command scope aborts the matrix with available facts."""
    def __init__(self, message, capsule):
        super().__init__(message)
        self.capsule = capsule


class ArtifactMutation(RuntimeError):
    """A compiled output path/bytes changed between inspection and validation."""


def read_artifact(path):
    """Read one regular file at most64MiB and bind the opened identity/bytes.

    O_NONBLOCK is required before opening: checking a FIFO after an ordinary
    blocking open would already let it stall the controller outside a command
    deadline. The opened descriptor is checked before any read, so a raced path
    replacement cannot substitute a device or pipe for a prechecked pathname.
    A mutation during the bounded read is rejected. Hash and import inspection
    use this exact returned byte string rather than independent mutable reads.
    This is a same-descriptor consistency check, not a filesystem-wide lock.
    """
    nonblocking = getattr(os, 'O_NONBLOCK', None)
    if not isinstance(nonblocking, int) or nonblocking <= 0:
        raise ValueError('bounded regular-file reads require O_NONBLOCK support')
    descriptor = os.open(os.fspath(path), os.O_RDONLY | nonblocking)
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode):
            raise ValueError('artifact must be an opened regular file')
        limit = 64 * 1024 * 1024
        if not 0 <= before.st_size <= limit:
            raise ValueError('artifact declared size is outside the64MiB bound')
        parts = []
        remaining = before.st_size + 1  # One extra byte detects unexpected growth.
        while remaining:
            block = os.read(descriptor, min(1024 * 1024, remaining))
            if not block:
                break
            parts.append(block)
            remaining -= len(block)
        data = b''.join(parts)
        after = os.fstat(descriptor)
    finally:
        os.close(descriptor)
    fields = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
    if not stat.S_ISREG(after.st_mode) or any(getattr(before, key) != getattr(after, key) for key in fields):
        raise ArtifactMutation('compiled artifact changed during bounded read')
    if len(data) > 64 * 1024 * 1024 or len(data) != after.st_size:
        raise ValueError('missing, truncated or over64MiB compiled artifact')
    binding = {'device': after.st_dev, 'inode': after.st_ino, 'size': after.st_size,
               'mtime_ns': after.st_mtime_ns, 'ctime_ns': after.st_ctime_ns,
               'sha256': hashlib.sha256(data).hexdigest()}
    return data, binding


def verify_artifact(path, expected):
    """Require one unchanged output file before/after the full validator."""
    try:
        _, current = read_artifact(path)
    except OSError as error:
        raise ArtifactMutation('compiled artifact path disappeared: ' + str(error)) from error
    if current != expected:
        raise ArtifactMutation('compiled artifact file identity/bytes changed between verification steps')
    return current


def abort_after_row(result):
    """Stop after source mutation or any observed process/memory/cleanup guard failure.

    Ordinary missing toolchains, compiler errors and unsupported import modules
    remain independent rows; no command runs after a failed ownership guard.
    """
    if result.get('fatal_guard_exception') or result.get('fatal_row_finalization') or result['status'] in ('FAIL_SOURCE_MUTATION', 'FAIL_ARTIFACT_MUTATION', 'FAIL_TOOL_INPUT_MUTATION'):
        return True
    for name in ('compile', 'full_validation'):
        step = result.get(name, {})
        if step.get('guard_error') or step.get('cleanup', {}).get('status') == 'FAIL' or step.get('cleanup', {}).get('remaining_live_owned'):
            return True
    return False


def write_json(path, value):
    """Write one result inside the already fresh run directory."""
    Path(path).write_text(json.dumps(value, indent=2) + '\n')



def finalize_row(result, rowdir, args, source_before):
    """Persist available row facts and abort on secondary binding/evidence errors.

    Secondary compiler inputs and the SDK source manifest are rechecked before
    saving a row. A failed check/write returns an explicit fatal row, preserving
    earlier command capsules. If normal persistence fails, a separate fallback
    file is attempted; an unavailable fallback is recorded and emitted to stderr
    best-effort, never described as successful on-disk evidence.
    """
    stage = 'secondary_input_binding'
    try:
        if 'asc_script' in result:
            stage = 'asc_script_after_binding'
            result['asc_script_after_sha256'] = digest(args.asc_js)
            if result['asc_script_after_sha256'] != result['asc_script']['sha256']:
                result['status'] = 'FAIL_TOOL_INPUT_MUTATION'
        if 'rustc' in result:
            stage = 'rustc_after_binding'
            result['rustc_after'] = executable(args.rustc)
            if result['rustc_after'] != result['rustc']:
                result['status'] = 'FAIL_TOOL_INPUT_MUTATION'
        if 'rust_target_libc' in result:
            stage = 'rust_target_libc_after_binding'
            _, result['rust_target_libc_after'] = rust_wasi_link_inputs(args.wasi_sdk)
            if result['rust_target_libc_after'] != result['rust_target_libc']:
                result['status'] = 'FAIL_TOOL_INPUT_MUTATION'
        stage = 'source_after_binding'
        source_now = manifest()
        result['row_source_after_sha256'] = source_now['sha256']
        if source_now != source_before:
            result['status'] = 'FAIL_SOURCE_MUTATION'
            result['reason'] = 'SDK source manifest changed during the matrix'
        stage = 'row_report_persistence'
        write_json(rowdir / 'report.json', result)
    except Exception as error:
        capsule = {'stage': stage, 'exception_type': type(error).__name__, 'reason': str(error),
                   'scope': 'Available row facts only; failed binding/persistence is not successful proof'}
        result['status'] = 'FAIL_FATAL_ROW_FINALIZATION'
        result['fatal_row_finalization'] = capsule
        try:
            write_json(rowdir / 'fatal-row-finalization.json', result)
        except Exception as persistence_error:
            capsule['fallback_persistence_error'] = str(persistence_error)
            try:
                print(json.dumps({'progress': 'fatal-row-fallback-unavailable', 'id': result.get('id'),
                                  'fatal_row_finalization': capsule}), file=sys.stderr, flush=True)
            except OSError as stderr_error:
                capsule['stderr_persistence_error'] = str(stderr_error)
    return result

def events(text):
    """Parse numeric cgroup memory events, preserving counters across each observation."""
    return {key: int(value) for key, value in (line.split() for line in text.splitlines())}


def cgroup_snapshot(args):
    """Pin the actual child and the unchanged original parent's kernel controls."""
    return _owner_snapshot(args)


def executable(path):
    """Bind the supplied compiler/interpreter/validator bytes without running it."""
    path = Path(path)
    if not path.is_absolute() or not path.is_file() or not os.access(path, os.X_OK):
        raise ValueError('tool must be an existing absolute executable path: ' + str(path))
    stat = path.stat()
    with path.open('rb') as stream:
        header = stream.read(64)
    row = {'path': str(path), 'resolved': str(path.resolve()), 'sha256': digest(path),
           'device': stat.st_dev, 'inode': stat.st_ino, 'size': stat.st_size}
    if header[:4] == b'\x7fELF':
        endian = '<' if header[5] == 1 else '>' if header[5] == 2 else None
        if endian is None:
            raise ValueError('invalid ELF byte order')
        row.update(kind='ELF', machine=struct.unpack(endian + 'H', header[18:20])[0], elf_class=header[4])
    else:
        raise ValueError('tool is not a directly bound ELF executable; supply the actual interpreter/compiler')
    return row


def process(pid):
    """Read one live PID's birth, parent/group, membership, RSS and executable inode."""
    base = Path('/proc') / str(pid)
    fields = (base / 'stat').read_text().rsplit(')', 1)[1].split()
    status = (base / 'status').read_text().splitlines()
    rss = next((int(line.split()[1]) * 1024 for line in status if line.startswith('VmRSS:')), 0)
    membership = (base / 'cgroup').read_text().splitlines()
    result = {'pid': pid, 'state': fields[0], 'ppid': int(fields[1]), 'pgrp': int(fields[2]),
              'birth': int(fields[19]), 'rss': rss, 'cgroup': membership}
    try:
        stat = (base / 'exe').stat()
        result.update(exe=os.readlink(base / 'exe'), exe_device=stat.st_dev, exe_inode=stat.st_ino)
    except OSError:
        result['exe'] = None
    again = (base / 'stat').read_text().rsplit(')', 1)[1].split()
    if int(again[19]) != result['birth']:
        raise ProcessLookupError('PID birth changed during observation')
    return result


def owned_processes(parent, known):
    """Discover current descendants plus still-live previously observed owned births."""
    rows = {}
    for name in os.listdir('/proc'):
        if name.isdecimal():
            try:
                rows[int(name)] = process(int(name))
            except (OSError, ValueError, IndexError):
                continue  # A raced exit is never retained as positive live evidence.
    owners = {parent} if parent in rows and known.get(parent) == rows[parent]['birth'] else set()
    owners.update(pid for pid, birth in known.items() if pid in rows and rows[pid]['birth'] == birth)
    changed = True
    while changed:
        prior = len(owners)
        owners.update(pid for pid, row in rows.items() if row['ppid'] in owners)
        changed = len(owners) != prior
    return [rows[pid] for pid in sorted(owners) if pid in rows]


def stop_owned(child, known):
    """Kill only verified observed births through pidfds, including orphan children.

    The root may already be exited. No reused process group is signaled; each
    pidfd is checked against the recorded birth before sending an owned signal.
    """
    signals = []
    for pid, birth in list(known.items()):
        descriptor = None
        try:
            descriptor = os.pidfd_open(pid)
            current = process(pid)
            if current['birth'] != birth or current['state'] == 'Z':
                continue
            signal.pidfd_send_signal(descriptor, signal.SIGKILL)
            signals.append({'pid': pid, 'birth': birth, 'signal': 'SIGKILL', 'method': 'birth-checked pidfd'})
        except (FileNotFoundError, ProcessLookupError):
            pass
        finally:
            if descriptor is not None:
                os.close(descriptor)
    try:
        child.wait(timeout=5)
    except subprocess.TimeoutExpired:
        return {'signals': signals, 'status': 'FAIL', 'reason': 'owned launcher did not terminate'}
    deadline = time.monotonic() + 5
    remaining = []
    while time.monotonic() < deadline:
        remaining = []
        for pid, birth in known.items():
            try:
                current = process(pid)
                if current['birth'] == birth and current['state'] != 'Z':
                    remaining.append(current)
            except (FileNotFoundError, ProcessLookupError):
                pass
        if not remaining:
            break
        time.sleep(0.02)
    return {'signals': signals, 'status': 'PASS' if not remaining else 'FAIL', 'remaining_live_owned': remaining}


def guarded_command(argv, cwd, env, rowdir, label, args):
    """Convert launched/scope exceptions into an explicit fatal row capsule.

    Missing executable preflight is ordinary blocked configuration. Once launch
    occurs, after-proof, executable-binding, cleanup or evidence errors cannot
    escape as an ordinary blocked row that would permit another compiler.
    """
    state = {'argv': argv, 'cwd': str(cwd), 'label': label, 'launched': False,
             'scope': 'Available command facts only; unknown after/cleanup facts are not successful proof'}
    try:
        return guarded_command_impl(argv, cwd, env, rowdir, label, args, state)
    except Exception as error:
        if not state['launched'] and not state.get('cgroup_guard_failure'):
            raise
        state.update(status='FATAL_GUARD_EXCEPTION', exception_type=type(error).__name__, reason=str(error))
        try:
            write_json(rowdir / (label + '-fatal-guard.json'), state)
        except OSError as persistence_error:
            state['capsule_persistence_error'] = str(persistence_error)
        raise CommandGuardFailure(str(error), state) from error


def guarded_command_impl(argv, cwd, env, rowdir, label, args, state):
    """Run one tool in an owned session with held launch, live cgroup/RSS sampling.

    Samples establish observed membership, not exhaustive instruction tracing or
    ARM execution. Very short internal children can exit between samples; this
    limit is recorded and never promoted to an actual architecture qualification.
    """
    try:
        before = cgroup_snapshot(args)
    except Exception as error:
        state['cgroup_guard_failure'] = str(error)
        raise
    state['before'] = before
    tool = executable(argv[0])
    state['tool'] = tool
    known = {}
    observed = {}
    peak_rss = 0
    actual_tool_observed = False
    start = time.monotonic()
    child = None
    birth = None
    held = None
    cleanup = {"status": "NOT_NEEDED", "signals": []}
    raced_exits = []
    failure = None
    with (rowdir / (label + '.log')).open('wb') as log:
        child = subprocess.Popen([sys.executable, '-B', '-c', HELD_EXEC, json.dumps(argv)], cwd=cwd,
                                 env=env, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT,
                                 start_new_session=True)
        state.update(launched=True, launcher_pid=child.pid, raw_log=str(rowdir / (label + '.log')))
        try:
            while time.monotonic() - start < 10:
                current = process(child.pid)
                if current['state'] in ('T', 't'):
                    break
                if child.poll() is not None:
                    raise RuntimeError('held compiler launcher exited before its guard')
                time.sleep(0.01)
            else:
                raise RuntimeError('held compiler launcher did not stop')
            birth = current['birth']
            known[child.pid] = birth
            held = current
            state['held_launch'] = held
            state['known_owned_births'] = dict(known)
            if current['ppid'] != os.getpid() or current['pgrp'] != child.pid or current['cgroup'] != before['probe']['membership']:
                raise RuntimeError('held child parent/session/membership mismatch')
            leaf = Path(before['leaf'])
            limits = Path(before['limit_owner'])
            if (Path('/proc')/str(child.pid)/'cgroup').read_text().splitlines()!=before['probe']['membership']:
                raise RuntimeError('held child departed its actual pinned cgroup')
            held = current
            os.kill(child.pid, signal.SIGCONT)
            while True:
                current_identity = leaf.stat()
                if ((current_identity.st_dev, current_identity.st_ino) != (before['device'], before['inode'])
                        or Path('/proc/sys/kernel/random/boot_id').read_text().strip() != args.expected_boot
                        or [limits.stat().st_dev,limits.stat().st_ino] != before['limit_owner_identity']
                        or (limits/'memory.max').read_text().strip() != str(args.expected_memory_max)):
                    raise RuntimeError('cgroup leaf/boot/exact64GiB limit changed during command')
                if (limits / 'memory.swap.max').read_text().strip() != args.expected_swap_max or (limits / 'cpuset.cpus.effective').read_text().strip() != args.expected_cpus:
                    raise RuntimeError('cgroup swap/cpuset changed during command')
                rows = owned_processes(child.pid, known)
                rss = 0
                for current in rows:
                    known[current['pid']] = current['birth']  # ancestry/birth proved; retain even on an ensuing departure guard
                    state['known_owned_births'] = dict(known)
                    if current['state'] == 'Z':
                        continue
                    if current['cgroup'] != before['probe']['membership']:
                        raise RuntimeError('owned compiler process departed the actual pinned cgroup')
                    # process() read this PID's actual kernel cgroup together
                    # with its birth/ancestry. The outer root owner separately
                    # samples cgroup.procs, which is intentionally root-only.
                    key = (current['pid'], current['birth'], current.get('exe_device'), current.get('exe_inode'))
                    observed[key] = current
                    rss += current['rss']
                    if (current.get('exe_device'), current.get('exe_inode')) == (tool['device'], tool['inode']):
                        actual_tool_observed = True
                peak_rss = max(peak_rss, rss)
                if rss > args.rss_limit_mib * 1024 * 1024:
                    raise RuntimeError('owned compiler aggregate sampled RSS exceeded its configured guard')
                accounting = _owner_memory(limits)
                state['last_memory_accounting'] = accounting
                if accounting['admission_memory_bytes'] > args.cgroup_current_limit_mib * 1024 * 1024:
                    raise RuntimeError('approved shared-parent accounted memory exceeded its configured safety guard')
                if child.poll() is not None:
                    break
                if time.monotonic() - start > args.timeout_seconds:
                    raise RuntimeError('compiler/validator command deadline exceeded')
                time.sleep(0.02)
            child.wait()
            leftovers = [row for row in owned_processes(child.pid, known) if row['state'] != 'Z' and row['pid'] != child.pid]
            if leftovers:
                raise RuntimeError('compiler left observed live owned children after exit')
            cleanup = {'status': 'PASS', 'signals': [], 'remaining_live_owned': []}
        except Exception as error:
            failure = str(error)
            state['primary_guard_error'] = failure
            if birth is None:
                try:
                    known[child.pid] = process(child.pid)['birth']
                except (FileNotFoundError, ProcessLookupError):
                    pass
            state.update(cleanup_attempted=True, known_owned_births=dict(known))
            cleanup = stop_owned(child, known)
            state['cleanup_result'] = cleanup
            if cleanup['status'] != 'PASS':
                failure += '; owned cleanup failed'
    state['cleanup_result'] = cleanup
    state['command_returncode'] = child.returncode
    after = cgroup_snapshot(args)
    state['after'] = after
    delta = {key: after['events'].get(key, 0) - before['events'].get(key, 0) for key in set(before['events']) | set(after['events'])}
    if before['boot_id'] != after['boot_id'] or any(value < 0 for value in delta.values()) or any(delta.get(key, 0) for key in ('oom', 'oom_kill', 'oom_group_kill')):
        failure = failure or 'cgroup incarnation/counter/OOM guard failed'
    tool_after = executable(argv[0])
    state['tool_after'] = tool_after
    if tool_after != tool:
        failure = failure or 'supplied executable bytes/identity changed during execution'
    result = {'argv': argv, 'cwd': str(cwd), 'tool': tool, 'tool_after': tool_after, 'cleanup': cleanup, 'raced_exits': raced_exits, 'returncode': child.returncode,
              'guard_error': failure, 'held_launch': held,
              'seconds': time.monotonic() - start, 'aggregate_sampled_peak_rss': peak_rss,
              'target_executable_observed': actual_tool_observed, 'observed_processes': list(observed.values()),
              'sampling_limit': '20ms process sampling; raced exits are not positive architecture evidence',
              'before': before, 'after': after, 'memory_events_delta': delta,
              'log': str(rowdir / (label + '.log'))}
    write_json(rowdir / (label + '.json'), result)
    return result


def complete_abi_required(language, name):
    """Select dedicated whole-ABI probes, never ordinary typed applications.

    C/C++ lesson 04 has seven independently runnable typed service/action cases;
    each uses a legitimate ABI subset. Its name is not full-import evidence.
    The explicit 00 probe exports every raw function for compiler inspection
    while its main calls none. Existing other-language whole-ABI probes retain
    their exact completeness requirement.
    """
    return (language == 'javascript_bridge' or
            (language in ('rust', 'tinygo') and name == 'coverage') or
            (language in ('c', 'cpp') and name == '00_abi_coverage'))


def recipes(args, out):
    """Construct real per-language/per-backend compilation commands without executing them."""
    selected = args.languages or ['c', 'cpp', 'rust', 'tinygo', 'assemblyscript', 'javascript_bridge']
    rows = []
    for backend in args.backends or ['1', '2']:
        symbols = out / ('allowed-ros' + backend + '-symbols.txt')
        for language in selected:
            sources = []
            missing = []
            if language in ('c', 'cpp'):
                suffix = '.c' if language == 'c' else '.cpp'
                required = (['00_abi_coverage.c', '01_raw_imports.c', '02_encoded_resource.c', '03_node_executor.c', '04_service_action_resources.c', '05_queries_and_events.c',
                             '06_runtime_delivery.c', '07_service_action_servers.c', '08_context_future_timer.c',
                             '09_session_ownership.c', '10_action_client_bundle.c']
                            if language == 'c' else ['00_abi_coverage.cpp', '01_raw_imports.cpp', '02_encoded_resource.cpp', '03_node_executor.cpp',
                                                   '04_services_actions.cpp', '05_service_action_servers.cpp', '06_queries_and_control.cpp',
                                                   '06_runtime_delivery.cpp', '08_context_future_timer.cpp',
                                                   '09_session_ownership.cpp', '10_action_client_bundle.cpp'])
                base = _sdk_path(SDK, 'examples', language)
                sources = sorted(set(base.glob('*' + suffix)) | {base / name for name in required})
                compiler = args.wasi_sdk / 'bin' / ('clang' if language == 'c' else 'clang++') if args.wasi_sdk else None
                if not compiler:
                    missing.append('--wasi-sdk')
            elif language == 'javascript_bridge':
                sources = [_sdk_path(SDK, 'javascript/bridge.c'), _sdk_path(SDK, 'javascript/relay.c')]
                compiler = args.wasi_sdk / 'bin/clang' if args.wasi_sdk else None
                if not compiler:
                    missing.append('--wasi-sdk')
            elif language == 'rust':
                # Retain the required ABI/tutorial baseline and include every
                # declared executable application, including later lessons.
                declared = tomllib.loads((_sdk_path(SDK, 'examples/rust/Cargo.toml')).read_text()).get('bin', [])
                names = {'raw', 'typed', 'node', 'resources', 'coverage', 'servers',
                         'action_parameters', 'events_tf_lifecycle', 'tf_graph_clock', 'scheduling'} | {row['name'] for row in declared}
                paths = {row['name']: row.get('path', row['name'] + '.rs') for row in declared}
                sources = [_sdk_path(SDK, 'examples/rust', paths.get(name, name + '.rs')) for name in sorted(names)]
                compiler = args.cargo
                if not args.cargo or not args.rustc:
                    missing.append('--cargo and --rustc')
                if not args.wasi_sdk:
                    missing.append('--wasi-sdk for explicit target libc.a')
            elif language == 'tinygo':
                base = _sdk_path(SDK, 'examples/tinygo')
                required = {base / name / 'main.go' for name in ('raw', 'typed', 'node', 'resources', 'coverage',
                            'servers', 'action_parameters', 'events_tf_lifecycle', 'tf_graph_clock', 'scheduling')}
                sources = sorted(required | set(base.glob('*/main.go')))
                compiler = args.tinygo
                if not compiler:
                    missing.append('--tinygo')
            else:
                base = _sdk_path(SDK, 'examples/assemblyscript')
                if backend == '1':
                    base /= 'ros1'
                required = ['01-raw.ts', '02-codecs.ts', '03-node.ts', '04-resources.ts', '05-servers.ts',
                            '06-tf-graph-clock.ts' if backend == '1' else '06-events-tf-lifecycle.ts',
                            '07-scheduling.ts', '08-action-parameters.ts']
                sources = sorted(set(base.glob('[0-9]*.ts')) | {base / name for name in required})
                compiler = args.node
                if not args.node or not args.asc_js:
                    missing.append('--node and --asc-js')
            if not args.wasm_tools:
                missing.append('--wasm-tools')
            for source in sources:
                name = source.parent.name if language == 'tinygo' else source.stem
                # These lessons use the ROS2-only event/lifecycle ABI. A ROS1
                # compiler invocation is not an unsupported application row.
                if backend == '1' and language in ('rust', 'tinygo') and name == 'events_tf_lifecycle':
                    continue
                rowdir = out / (language + '-ros' + backend + '-' + name)
                artifact = rowdir / 'guest.wasm'
                argv = []
                cwd = ROOT
                if compiler:
                    if language in ('c', 'cpp'):
                        argv = [str(compiler), '--target=wasm32-wasip1', '--sysroot=' + str(args.wasi_sdk / 'share/wasi-sysroot'),
                                '-std=' + ('c11' if language == 'c' else 'c++20'), '-O2', '-I', str(_sdk_path(SDK, 'include')),
                                '-DROSWASM_EXAMPLE_ROS_VERSION=' + backend, '-Wl,--export-dynamic',
                                '-Wl,--allow-undefined-file=' + str(symbols), str(source), '-o', str(artifact)]
                        if language == 'cpp':
                            argv.insert(4, '-stdlib=libc++')
                            # These WASIp1 tutorials use numeric SDK refusals and
                            # nonthrowing callbacks; do not require a Wasm EH ABI.
                            # Native exception-enabled models keep their flags.
                            argv.insert(5, '-fno-exceptions')
                            argv.insert(6, '-fno-rtti')
                        if name == '06_runtime_delivery':
                            # This gated basic-stdio fixture supplies _start
                            # itself. Keep libc/C++ libraries for the SDK while
                            # omitting CRT argument/environment initialization.
                            argv[1:1] = ['-nostartfiles', '-ffreestanding', '-fno-builtin',
                                         '-Wl,--entry=_start', '-Wl,--export=_start']
                        if name == '00_abi_coverage':
                            # The whole-ABI selector must remain externally
                            # reachable after optimization and dead stripping.
                            # main is empty; this is compile/import coverage.
                            argv.insert(1, '-Wl,--export=roswasm_abi_coverage')
                    elif language == 'javascript_bridge':
                        argv = [str(compiler), '--target=wasm32', '-std=c11', '-O2', '-ffreestanding', '-nostdlib',
                                '-DROSWASM_BRIDGE_ROS_VERSION=' + backend, '-Wl,--no-entry', '-Wl,--export-memory',
                                '-Wl,--allow-undefined-file=' + str(symbols), str(source), '-o', str(artifact)]
                        if name == 'relay':
                            # relay.c includes the original bridge exactly once.
                            # Its only additional host interface is basic I/O.
                            argv.insert(5, '-fno-builtin')
                            argv[argv.index('-Wl,--no-entry')] = '-Wl,--entry=_start'
                            argv.insert(-4, '-Wl,--export=_start')
                            argv[1:1] = ['-Wl,--initial-memory=131072', '-Wl,--max-memory=131072',
                                         '-Wl,-z,stack-size=65536']
                    elif language == 'rust':
                        work = out / 'work/rust/sdk/origin/examples/rust'
                        features = 'ros' + backend + (',alloc' if name not in ('raw', 'typed', 'coverage') else '')
                        argv = [str(compiler), 'build', '--offline', '-j1', '--release', '--manifest-path', str(work / 'Cargo.toml'),
                                '--target', args.rust_target, '--target-dir', str(out / ('cargo-target-ros' + backend)),
                                '--no-default-features', '--features', features, '--bin', name]
                    elif language == 'tinygo':
                        cwd = out / 'work/tinygo/sdk/origin/examples/tinygo'
                        # One compiler package at a time; keep this full matrix's
                        # existing optimization level rather than changing it.
                        argv = [str(compiler), 'build', '-p', '1', '-target=wasi', '-scheduler=none', '-tags=ros' + backend, '-o', str(artifact), './' + name]
                    else:
                        argv = [str(compiler), str(args.asc_js), str(source), '--runtime', 'incremental', '--exportRuntime',
                                '--use', 'abort=sdk/origin/assemblyscript/api/abortGuest', '--outFile', str(artifact)]
                rows.append({'id': rowdir.name, 'language': language, 'backend': backend, 'source': str(source),
                             'source_sha256': digest(source) if source.is_file() else None, 'argv': argv, 'cwd': str(cwd),
                             'artifact': str(artifact), 'rowdir': str(rowdir), 'missing_configuration': missing,
                             'require_all_imports': complete_abi_required(language, name), 'expects_ros_imports': not ((language in ('rust', 'tinygo') and name == 'scheduling') or (language == 'assemblyscript' and name == '07-scheduling'))})
    if 'assemblyscript' in selected:
        fixtures = [('coverage_ros' + backend, backend, True, True) for backend in args.backends or ['1', '2']]
        fixtures += [(name, 'none', False, False) for name in ('highlevel_coverage', 'codec_vectors', 'adapter_contracts', 'factory_transactions', 'executor_mutation', 'resource_roles', 'unicode_strings', 'local_schedule', 'server_callbacks', 'unified_events', 'event_dispatch')]
        for name, backend, require_all, expects_ros in fixtures:
            source = _sdk_path(SDK, 'assemblyscript/tests', name + '.ts')
            rowdir = out / ('assemblyscript-' + ('ros' + backend if backend != 'none' else 'pure') + '-' + name)
            artifact = rowdir / 'guest.wasm'
            missing = []
            if not args.node or not args.asc_js:
                missing.append('--node and --asc-js')
            if not args.wasm_tools:
                missing.append('--wasm-tools')
            argv = [str(args.node), str(args.asc_js), str(source), '--runtime', 'incremental', '--exportRuntime',
                    '--use', 'abort=sdk/origin/assemblyscript/api/abortGuest', '--outFile', str(artifact)] if args.node and args.asc_js else []
            rows.append({'id': rowdir.name, 'language': 'assemblyscript', 'backend': backend, 'source': str(source),
                         'source_sha256': digest(source) if source.is_file() else None, 'argv': argv, 'cwd': str(ROOT),
                         'artifact': str(artifact), 'rowdir': str(rowdir), 'missing_configuration': missing,
                         'require_all_imports': require_all, 'expects_ros_imports': expects_ros, 'fixture': True})
    return rows + custom_example_recipes(args, out, rows)


def custom_example_recipes(args, out, ordinary):
    """Compile the five handwritten fixed-record ROS1 guest applications too.

    Reuse each selected language's genuine recipe and target flags. These
    guests require independently registered native identities at execution;
    compiling guest-only generated codecs never fabricates that registration.
    JavaScript's sixth example uses the supplied bridge and pure source models.
    """
    suffixes = {'c': '.c', 'cpp': '.cpp', 'rust': '.rs',
                'tinygo': '.go', 'assemblyscript': '.ts'}
    results = []
    for language, suffix in suffixes.items():
        baseline = next((row for row in ordinary
                         if row['language'] == language and row['backend'] == '1' and
                         not row['require_all_imports']), None)
        if baseline is None:
            continue
        source = _sdk_path(SDK, 'custom-types/examples', 'use_generated' + suffix)
        rowdir = out / (language + '-ros1-custom-packet')
        artifact = rowdir / 'guest.wasm'
        row = dict(baseline, id=rowdir.name, source=str(source),
                   source_sha256=digest(source) if source.is_file() else None,
                   artifact=str(artifact), rowdir=str(rowdir),
                   require_all_imports=False, expects_ros_imports=True,
                   custom_example=True)
        row['argv'] = [str(source) if value == baseline['source'] else
                       str(artifact) if value == baseline['artifact'] else value
                       for value in baseline['argv']]
        application = out / 'work' / language / 'sdk/origin/custom-types/application'
        if row['argv'] and language == 'rust':
            row['argv'][row['argv'].index('--manifest-path') + 1] = str(application / 'Cargo.toml')
            row['argv'][row['argv'].index('--features') + 1] = 'ros1,alloc'
            row['argv'][row['argv'].index('--bin') + 1] = 'use_generated'
        elif row['argv'] and language == 'tinygo':
            row['cwd'] = str(application)
            row['argv'][-1] = '.'
        results.append(row)
    return results


def copy_custom_example_work(sdk, target, language):
    """Prepare explicit local Cargo/Go modules beneath a fresh owned work tree.

    All code is copied from the frozen SDK; only package manifests/layout are
    supplied here. No package is fetched, generated native identity supplied,
    source file overwritten or existing generated revision modified.
    """
    custom = target / 'custom-types'
    shutil.copytree(sdk / 'custom-types/generated-r3', custom / 'generated-r3')
    examples = custom / 'examples'
    examples.mkdir()
    suffix = '.rs' if language == 'rust' else '.go'
    source = sdk / 'custom-types/examples' / ('use_generated' + suffix)
    shutil.copyfile(source, examples / source.name)
    application = custom / 'application'
    application.mkdir()
    if language == 'rust':
        (application / 'Cargo.toml').write_text(
            '[package]\nname = "roswasm-custom-example"\nversion = "0.1.0"\nedition = "2021"\n'
            '[dependencies]\nroswasm = { path = "../../rust", default-features = false }\n'
            '[features]\ndefault = []\nros1 = ["roswasm/ros1"]\nalloc = ["roswasm/alloc"]\n'
            '[[bin]]\nname = "use_generated"\npath = "../examples/use_generated.rs"\n')
    else:
        shutil.copyfile(source, application / 'main.go')
        package = application / 'customtypes'
        package.mkdir()
        shutil.copyfile(sdk / 'custom-types/generated-r3/fixed_records.go', package / 'fixed_records.go')
        (application / 'go.mod').write_text(
            'module example.local/guest\n\ngo 1.23\n\n'
            'require uwvm2.local/roswasm v0.0.0\n'
            'replace uwvm2.local/roswasm => ../../tinygo\n')


def check_relay_imports(contract):
    """Require precisely the relay's three basic WASIp1 function imports."""
    expected = {'fd_read': (['i32'] * 4, ['i32']),
                'fd_write': (['i32'] * 4, ['i32']),
                'proc_exit': (['i32'], [])}
    rows = [item for item in contract['imports'] if item['module'] == 'wasi_snapshot_preview1']
    if len(rows) != len(expected) or {item['name'] for item in rows} != set(expected):
        raise ValueError('relay must import only fd_read, fd_write and proc_exit in addition to ROS')
    for item in rows:
        if item['kind'] != 0 or (item['params'], item['results']) != expected[item['name']]:
            raise ValueError('relay has an incompatible basic WASIp1 signature')
    return contract


def matrix_status(rows, results):
    """Never accept an empty/vacuous matrix as a compiler success."""
    return ('COMPILER_MATRIX_PASS' if rows and len(results) == len(rows)
            and all(row['status'] == 'COMPILER_IMPORT_FULL_VALIDATION_PASS' for row in results)
            else 'COMPILER_MATRIX_INCOMPLETE')


def main():
    """Default to source-only planning; actual commands require all cgroup pins."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run', action='store_true')
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--languages', nargs='+', choices=('c', 'cpp', 'rust', 'tinygo', 'assemblyscript', 'javascript_bridge'))
    parser.add_argument('--backends', nargs='+', choices=('1', '2'))
    parser.add_argument('--wasi-sdk', type=Path)
    parser.add_argument('--cargo', type=Path)
    parser.add_argument('--rustc', type=Path)
    parser.add_argument('--rust-target', default='wasm32-wasip1', choices=('wasm32-wasip1',))
    parser.add_argument('--tinygo', type=Path)
    parser.add_argument('--node', type=Path)
    parser.add_argument('--asc-js', type=Path)
    parser.add_argument('--wasm-tools', type=Path)
    parser.add_argument('--expected-leaf', type=Path)
    parser.add_argument('--expected-device', type=int)
    parser.add_argument('--expected-inode', type=int)
    parser.add_argument('--expected-boot')
    parser.add_argument('--expected-memory-max', type=int, default=LIMIT, choices=(LIMIT,))
    parser.add_argument('--expected-swap-max', default='0', choices=('0',))
    parser.add_argument('--expected-cpus', default='0-31', choices=('0-31',))
    parser.add_argument('--rss-limit-mib', type=int, default=2048)
    parser.add_argument('--cgroup-current-limit-mib', type=int, default=DEFAULT_CURRENT_LIMIT_MIB)
    parser.add_argument('--timeout-seconds', type=int, default=180)
    args = parser.parse_args()
    for name in ('wasi_sdk', 'cargo', 'rustc', 'tinygo', 'node', 'asc_js', 'wasm_tools', 'expected_leaf'):
        path = getattr(args, name)
        if path is not None and not path.is_absolute():
            parser.error('--' + name.replace('_', '-') + ' must be absolute')
    if not args.out.is_absolute():
        parser.error('--out must be absolute')
    out = args.out.resolve()
    if out.is_relative_to(SDK):
        parser.error('compiler outputs/caches must be outside the immutable SDK tree')
    if args.languages:
        args.languages = list(dict.fromkeys(args.languages))
    if args.backends:
        args.backends = list(dict.fromkeys(args.backends))
    rows = recipes(args, out)
    if not rows:
        parser.error('selected matrix has no required compilation rows')
    if not args.run:
        print(json.dumps({'status': 'PREPARED_PLAN_ONLY', 'scope': 'No compiler, validator, guest or ROS execution',
                          'expected_memory_max': args.expected_memory_max, 'expected_swap_max': args.expected_swap_max,
                          'rss_limit_mib': args.rss_limit_mib, 'cgroup_current_limit_mib': args.cgroup_current_limit_mib,
                          'rows': rows}, indent=2))
        return 0
    try:_bind_owner_arguments(args)
    except (OSError,ValueError,KeyError)as error:parser.error(str(error))
    if platform.system() != 'Linux' or any(value is None for value in (args.expected_leaf, args.expected_device, args.expected_inode, args.expected_boot)):
        parser.error('--run requires Linux and --expected-leaf/--expected-device/--expected-inode/--expected-boot')
    if not 0 < args.rss_limit_mib <= args.cgroup_current_limit_mib < LIMIT_MIB or args.timeout_seconds <= 0:
        parser.error('require 0 < RSS guard <= cgroup-current guard < 65536 MiB and positive deadline')
    if not hasattr(os, 'pidfd_open') or not hasattr(signal, 'pidfd_send_signal'):
        parser.error('actual execution requires Linux pidfd ownership-safe cleanup support')
    before = cgroup_snapshot(args)
    source_before = manifest()
    out.parent.mkdir(parents=True, exist_ok=True)
    out.mkdir(mode=0o700, exist_ok=False)
    write_json(out / 'source-before.json', source_before)
    schema = json.loads((_sdk_path(SDK, 'abi/abi-v1.json')).read_text())
    for backend in args.backends or ['1', '2']:
        names = ['uwvm_ros' + backend + '_' + row['name'] for row in schema['modules']['uwvm_ros' + backend + '_v1']['functions']]
        (out / ('allowed-ros' + backend + '-symbols.txt')).write_text('\n'.join(names) + '\n')
    selected = {row['language'] for row in rows}
    for language in ('rust', 'tinygo'):
        if language in selected:
            target = out / ('work/' + language + '/sdk')
            shutil.copytree(_sdk_path(SDK, language), target / language)
            shutil.copytree(_sdk_path(SDK, 'examples', language), target / 'examples' / language)
            shutil.copyfile(_sdk_path(SDK, 'LICENSE'), target / 'LICENSE')
            if any(row.get('custom_example') and row['language'] == language for row in rows):
                copy_custom_example_work(SDK, target, language)
    environment = os.environ.copy()
    environment.pop('CARGO_ENCODED_RUSTFLAGS', None)
    # The Go heap cap does not cover native LLVM allocations. The original
    # aggregate RSS/deadline/owned-process guards remain authoritative.
    environment.update(RUSTC_WRAPPER='', RUSTC_WORKSPACE_WRAPPER='', CARGO_BUILD_RUSTC_WRAPPER='', CARGO_BUILD_RUSTC_WORKSPACE_WRAPPER='',
                       PYTHONDONTWRITEBYTECODE='1', CARGO_NET_OFFLINE='true', RUSTUP_AUTO_INSTALL='0', CARGO_BUILD_JOBS='1',
                       GOPROXY='off', GOSUMDB='off', GOTOOLCHAIN='local', GOENV='off', GOMAXPROCS='1', GOMEMLIMIT='192MiB',
                       GOCACHE=str(out / 'go-cache'), GOMODCACHE=str(out / 'go-mod-cache'), XDG_CACHE_HOME=str(out / 'cache'))
    if args.rustc:
        environment['RUSTC'] = str(args.rustc)
    results = []
    for row in rows:
        print(json.dumps({'progress': 'row-start', 'id': row['id']}), flush=True)
        rowdir = Path(row['rowdir'])
        rowdir.mkdir()
        result = dict(row, status='BLOCKED', ros_execution='NOT_PERFORMED', guest_execution='NOT_PERFORMED')
        try:
            if not Path(row['source']).is_file():
                raise ValueError('required SDK example/fixture source is missing: ' + row['source'])
            if row['missing_configuration']:
                raise ValueError('missing existing toolchain inputs: ' + ', '.join(row['missing_configuration']))
            if args.asc_js and row['language'] == 'assemblyscript':
                result['asc_script'] = {'path': str(args.asc_js), 'sha256': digest(args.asc_js)}
            if args.rustc and row['language'] == 'rust':
                result['rustc'] = executable(args.rustc)
            if row['language'] in ('c', 'cpp') and not (args.wasi_sdk / 'share/wasi-sysroot').is_dir():
                raise ValueError('configured WASI standard-library sysroot is unavailable')
            row_environment = environment.copy()
            if row['language'] == 'rust':
                flags, archive = rust_wasi_link_inputs(args.wasi_sdk,
                    coverage=Path(row['source']).stem == 'coverage')
                row_environment.update(RUSTFLAGS='', CARGO_ENCODED_RUSTFLAGS='\x1f'.join(flags))
                result.update(rust_target_libc=archive, explicit_rustc_tokens=flags)
            result['compiler_environment_sha256'] = hashlib.sha256(json.dumps(sorted(row_environment.items()), separators=(',', ':')).encode()).hexdigest()
            relevant = ('RUSTC', 'RUSTFLAGS', 'CARGO_ENCODED_RUSTFLAGS', 'RUSTC_WRAPPER', 'RUSTC_WORKSPACE_WRAPPER', 'CARGO_BUILD_RUSTC_WRAPPER',
                        'CARGO_BUILD_RUSTC_WORKSPACE_WRAPPER', 'CARGO_HOME', 'RUSTUP_HOME', 'PATH', 'RUSTC_BOOTSTRAP', 'GOMAXPROCS', 'GOMEMLIMIT')
            result['relevant_environment'] = {key: {'present': key in row_environment,
                'length': len(row_environment.get(key, '')),
                'sha256': hashlib.sha256(row_environment.get(key, '').encode()).hexdigest()} for key in relevant}
            result['explicit_environment_controls'] = {'RUSTC': str(args.rustc) if args.rustc else None,
                'RUSTC_WRAPPER': '', 'RUSTC_WORKSPACE_WRAPPER': '', 'CARGO_BUILD_RUSTC_WRAPPER': '',
                'CARGO_BUILD_RUSTC_WORKSPACE_WRAPPER': '', 'CARGO_ENCODED_RUSTFLAGS': 'explicit token list for Rust rows; inherited override removed',
                'CARGO_NET_OFFLINE': 'true', 'RUSTUP_AUTO_INSTALL': '0', 'GOPROXY': 'off', 'GOSUMDB': 'off', 'GOTOOLCHAIN': 'local',
                'GOMAXPROCS': '1', 'GOMEMLIMIT': '192MiB'}
            result['toolchain_sdk_closure'] = 'UNQUALIFIED: system headers/libraries, target stdlib, global config and compiler package dependencies require separate verified closure'

            compile_result = guarded_command(row['argv'], Path(row['cwd']), row_environment, rowdir, 'compile', args)
            result['compile'] = compile_result
            if compile_result['guard_error'] or compile_result['returncode']:
                result['status'] = 'FAIL_COMPILE'
            else:
                artifact = Path(row['artifact'])
                if row['language'] == 'rust':
                    built = out / ('cargo-target-ros' + row['backend']) / args.rust_target / 'release' / (Path(row['source']).stem + '.wasm')
                    shutil.copyfile(built, artifact)
                result['status'] = 'FAIL_ARTIFACT_OUTPUT'
                data, binding = read_artifact(artifact)
                result['artifact_binding'] = binding
                result['artifact_sha256'] = binding['sha256']
                result['status'] = 'FAIL_IMPORT_CONTRACT'
                try:
                    if row['expects_ros_imports']:
                        result['import_contract'] = INSPECTOR.check(data, row['backend'], ('wasi_snapshot_preview1',), row['require_all_imports'])
                        if row['language'] == 'javascript_bridge' and Path(row['source']).stem == 'relay':
                            check_relay_imports(result['import_contract'])
                    else:
                        imports = INSPECTOR.inspect(data)
                        if any(item['module'] != 'wasi_snapshot_preview1' for item in imports):
                            raise ValueError('non-ROS import module in pure fixture needs compatible runtime: ' + str(imports))
                        result['import_contract'] = {'status': 'PURE_FIXTURE_BINARY_INSPECTED', 'scope': 'No ROS imports expected; no execution',
                                                     'sha256': binding['sha256'], 'imports': imports, 'ROS_functions_checked': 0} 
                except ValueError as error:
                    result['import_error'] = str(error)
                    try:
                        result['actual_imports_on_refusal'] = INSPECTOR.inspect(data)
                    except ValueError as parse_error:
                        result['binary_parse_error'] = str(parse_error)
                    if 'non-ROS import module' in str(error):
                        result['status'] = 'BLOCKED_UNSUPPORTED_IMPORT_MODULE'
                if 'import_contract' in result:
                    result['status'] = 'BLOCKED_FULL_VALIDATOR'
                    result['artifact_before_full_validation'] = verify_artifact(artifact, binding)
                    validation = guarded_command([str(args.wasm_tools), 'validate', str(artifact)], ROOT, environment, rowdir, 'full-validate', args)
                    result['full_validation'] = validation
                    result['artifact_after_full_validation'] = verify_artifact(artifact, binding)
                    result['status'] = 'COMPILER_IMPORT_FULL_VALIDATION_PASS' if not validation['guard_error'] and validation['returncode'] == 0 else 'FAIL_FULL_VALIDATION'
        except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
            result['reason'] = str(error)
            if isinstance(error, CommandGuardFailure):
                result['status'] = 'FAIL_FATAL_GUARD_EXCEPTION'
                result['fatal_guard_exception'] = error.capsule
            elif isinstance(error, ArtifactMutation):
                result['status'] = 'FAIL_ARTIFACT_MUTATION'
        finalize_row(result, rowdir, args, source_before)
        results.append(result)
        print(json.dumps({'progress': 'row-finish', 'id': row['id'], 'status': result['status']}), flush=True)
        if abort_after_row(result):
            break
    after = cgroup_snapshot(args)
    source_after = manifest()
    write_json(out / 'source-after.json', source_after)
    status = matrix_status(rows, results)
    run_delta = {key: after['events'].get(key, 0) - before['events'].get(key, 0) for key in set(before['events']) | set(after['events'])}
    if before['boot_id'] != after['boot_id'] or any(value < 0 for value in run_delta.values()) or any(run_delta.get(key, 0) for key in ('oom', 'oom_kill', 'oom_group_kill')):
        status = 'FAIL_CGROUP_OOM_OR_INCARNATION'
    if source_before != source_after:
        status = 'FAIL_SOURCE_MUTATION'
    report = {'status': status, 'task_complete': False, 'run_memory_events_delta': run_delta,
              'expected_memory_max': args.expected_memory_max, 'expected_swap_max': args.expected_swap_max,
              'rss_limit_mib': args.rss_limit_mib, 'cgroup_current_limit_mib': args.cgroup_current_limit_mib,
              'scope': 'Compilation/import/full validation only; no Guest/ROS/native/ARM execution',
              'before': before, 'after': after, 'source_before_sha256': source_before['sha256'], 'source_after_sha256': source_after['sha256'],
              'rows': results, 'native_C_CPP_model_tests': 'REQUIRED_SEPARATE_CGROUP_DRIVER_WITH_CROSS_CONFIG_AND_QEMU',
              'actual_ROS_behavior': 'REQUIRED_SEPARATE_QUALIFIED_ARM_WORKER_GATEWAY_MAINLINE',
              'toolchain_sdk_closure': 'UNQUALIFIED',
              'reuse_qualification': 'NOT_ESTABLISHED: matrix records commands and produced binaries, not complete immutable compiler inputs'}
    write_json(out / 'report.json', report)
    return 0 if status == 'COMPILER_MATRIX_PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
