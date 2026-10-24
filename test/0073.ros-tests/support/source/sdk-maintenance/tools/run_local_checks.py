#!/usr/bin/env python3
"""Run serial SDK checks and Wasm compilation from a fresh /tmp snapshot.

This local developer runner complements the Linux cgroup acceptance drivers.
It never changes their admission rules, starts ROS, connects to a Gateway or
grants a Wasm capability. Every command, build, cache and log lives beneath the
caller-selected fresh /tmp directory. Host mock models verify SDK contracts;
they do not establish native ROS communication or a sandboxed JS embedding.

Use --groups source native after the source authors have finished. Native
compilers run one translation unit at a time; Cargo and Go use one build job.
The aggregate RSS watchdog covers the command's process group, including
compiler children, rather than checking only a short-lived shell launcher.

Use --resume-from /tmp/PRIOR_RUN with --groups source wasm after an interrupted
local run. Only independently admitted successful Wasm compiler products can be
copied; every copied product receives current import inspection and a new full
validator execution. Source checks and native models are never reused. Original
failed/interrupted results remain in the prior directory and are bound as such.
Historical artifact/tool hashes absent from that run are never invented: the
retained product and current tools are hashed at resume admission, and external
compiler packages, target libraries and historic environments stay unqualified.
"""
from __future__ import annotations

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path
from stage_sdk_tests import overlay as _stage_sdk_tests
from resource_contract import validate_resource_owner


import argparse
import datetime
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import signal
import stat
import subprocess
import sys
import time
from urllib.parse import unquote, urlsplit

os.environ['PYTHONDONTWRITEBYTECODE'] = '1'
sys.dont_write_bytecode = True
PROJECT = _CHECKOUT_ROOT


def write_json_atomic(path: Path, value) -> None:
    """Keep the preceding complete receipt if a new write fails or fills disk.

    The temporary sibling is disposable new-run output. An interrupted/failed
    write may leave that sibling for diagnosis; it never truncates the committed
    progress/result record. No path in a prior run is written by this helper.
    """
    temporary = path.with_name(path.name + '.tmp')
    flags = os.O_WRONLY | os.O_CREAT | os.O_TRUNC | getattr(os, 'O_NOFOLLOW', 0)
    descriptor = os.open(temporary, flags, 0o600)
    with os.fdopen(descriptor, 'w') as output:
        json.dump(value, output, indent=2)
        output.write('\n')
        output.flush()
        os.fsync(output.fileno())
    os.replace(temporary, path)


def regular_binding(path: Path, limit: int = 64 * 1024 * 1024,
                    capture: bool = True, allow_symlink: bool = False) -> tuple[bytes | None, dict]:
    """Bind a bounded opened regular file, rejecting truncation and mutation.

    Artifacts, manifests and logs forbid symlinks. Explicit external compiler
    commands may themselves be symlinks; their current opened target is recorded
    separately and is not a historical compiler/package-closure attestation.
    Large tool files are streamed without retaining their bodies in memory.
    """
    flags = os.O_RDONLY | os.O_NONBLOCK
    if not allow_symlink:
        if path.is_symlink():
            raise ValueError('resume input must not be a symlink: ' + str(path))
        flags |= getattr(os, 'O_NOFOLLOW', 0)
    descriptor = os.open(path, flags)
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode) or not 0 <= before.st_size <= limit:
            raise ValueError('resume input must be a bounded regular file: ' + str(path))
        digest, chunks, total = hashlib.sha256(), [], 0
        while total <= before.st_size:
            chunk = os.read(descriptor, min(1024 * 1024, before.st_size + 1 - total))
            if not chunk:
                break
            total += len(chunk)
            digest.update(chunk)
            if capture:
                chunks.append(chunk)
        after = os.fstat(descriptor)
        fields = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
        if total != before.st_size or any(getattr(before, key) != getattr(after, key) for key in fields):
            raise ValueError('resume input changed while being read: ' + str(path))
    finally:
        os.close(descriptor)
    return (b''.join(chunks) if capture else None), {
        'path': str(path), 'bytes': total, 'sha256': digest.hexdigest(),
        'device': after.st_dev, 'inode': after.st_ino,
        'mtime_ns': after.st_mtime_ns, 'ctime_ns': after.st_ctime_ns}


class ResumeInputs:
    """Admit prior Wasm compiler products without promoting the interrupted run.

    All original SDK files, including the recipe/import checker, must match the
    frozen prior manifest. Current SDK names/body/dependencies must be identical
    except these two explicitly administrative Python entry points. Prior copied
    compiler work trees must also match freshly copied work inputs for that row.
    Source/native failures are preserved and never reused by this class.
    """
    ADMINISTRATIVE = {'tools/run_local_checks.py', 'tools/verify_sources.py'}

    def __init__(self, prior: Path, run, current_inputs: dict):
        if not prior.is_absolute() or prior.is_symlink():
            raise ValueError('--resume-from requires an absolute regular /tmp run directory')
        self.prior, self.run = prior.resolve(), run
        if (not self.prior.is_relative_to(Path('/tmp').resolve()) or
            self.prior == Path('/tmp').resolve() or not self.prior.is_dir() or
            self.prior == run.out or run.out.is_relative_to(self.prior) or self.prior.is_relative_to(run.out)):
            raise ValueError('prior/current runs must be distinct independent directories below /tmp')
        self.tools, self.work, self.by_name = {}, {}, {}
        self.records = {}
        for name in ('source-manifest.json', 'context-manifest.json', 'progress.json'):
            body, binding = regular_binding(self.bound_path(name))
            self.records[name] = {'value': json.loads(body), 'binding': binding}
        held = self.records['source-manifest.json']['value']
        if not isinstance(held, dict) or not held or manifest(self.prior / 'source/sdk') != held:
            raise ValueError('prior frozen SDK no longer matches its original source manifest')
        if set(held) != set(current_inputs):
            raise ValueError('resume cannot add/remove SDK inputs')
        changed = {name: {'prior': held[name], 'current': current_inputs[name]}
                   for name in held if held[name] != current_inputs[name]}
        if set(changed) - self.ADMINISTRATIVE:
            raise ValueError('resume SDK body/dependencies changed outside administrative tools: ' + ', '.join(sorted(set(changed) - self.ADMINISTRATIVE)))
        context = self.records['context-manifest.json']['value']
        current_context = json.loads((run.out / 'context-manifest.json').read_text())
        if not isinstance(context, dict):
            raise ValueError('prior context manifest must be an object')
        for name, expected in context.items():
            _, actual = regular_binding(self.bound_path('source/' + name))
            if {key: actual[key] for key in ('bytes', 'sha256')} != expected or current_context.get(name) != expected:
                raise ValueError('prior/current retained repository context differs: ' + name)
        progress = self.records['progress.json']['value']
        if not isinstance(progress, list) or not progress:
            raise ValueError('prior progress must contain actual command records')
        for row in progress:
            if not isinstance(row, dict) or not isinstance(row.get('name'), str) or row['name'] in self.by_name:
                raise ValueError('prior progress has malformed/duplicate command names')
            self.by_name[row['name']] = row
        report = self.prior / 'report.json'
        final_report = ({'status': 'NOT_WRITTEN_INTERRUPTED_RUN', 'sha256': None} if not report.exists()
                        else regular_binding(self.bound_path('report.json'), capture=False)[1])
        self.admission = {'scope': 'Reuse of recorded successful Wasm compiler products only; current signature inspection and validator required; no native ROS/delivery or full toolchain closure',
            'prior_run': str(self.prior), 'prior_final_report': final_report,
            'prior_records': {name: record['binding'] for name, record in self.records.items()},
            'prior_sdk_files_preserved': True, 'current_sdk_names_identical': True,
            'administrative_source_differences': changed,
            'original_nonpass_commands': [row for row in progress if row.get('status') != 'PASS'],
            'original_run_status': 'NOT_PROMOTED',
            'historical_artifact_sha256': 'NOT_RECORDED_BY_ORIGINAL_PROGRESS_FORMAT',
            'historical_tool_fingerprints': 'NOT_RECORDED_BY_ORIGINAL_PROGRESS_FORMAT',
            'historical_compiler_environment': 'NOT_RECORDED_BY_ORIGINAL_PROGRESS_FORMAT',
            'compiler_package_and_target_stdlib_closure': 'UNQUALIFIED',
            'added_current_context_inputs': {name: value for name, value in current_context.items() if name not in context},
            'observed_current_tool_files': self.tools, 'reused_rows': []}
        self.save()

    def bound_path(self, relative: str) -> Path:
        """Refuse path traversal/symlink substitution in the read-only prior run."""
        part = Path(relative)
        if part.is_absolute() or any(name in ('', '.', '..') for name in part.parts):
            raise ValueError('invalid prior-run relative path')
        path = self.prior
        for name in part.parts:
            path /= name
            if path.is_symlink():
                raise ValueError('prior-run source/product path contains a symlink: ' + str(path))
        return path

    def normalize(self, value: str) -> str:
        """Rebase only the prior output prefix; preserve every other argv byte."""
        if not isinstance(value, str):
            raise ValueError('prior argv/cwd must contain strings')
        aliases = {str(self.prior), str(Path('/tmp') / self.prior.relative_to(Path('/tmp').resolve()))}
        for prefix in sorted(aliases, key=len, reverse=True):
            if value == prefix:
                return str(self.run.out)
            value = value.replace(prefix + '/', str(self.run.out) + '/')
        return value

    def current_tool(self, path: Path) -> dict:
        """Record current tool bytes, never claiming unavailable historical pins."""
        key = str(path)
        if key not in self.tools:
            _, binding = regular_binding(path, 512 * 1024 * 1024, capture=False, allow_symlink=True)
            self.tools[key] = binding
        else:
            now = path.stat()
            old = self.tools[key]
            if (now.st_dev, now.st_ino, now.st_size, now.st_mtime_ns, now.st_ctime_ns) != (
                old['device'], old['inode'], old['bytes'], old['mtime_ns'], old['ctime_ns']):
                raise ValueError('current explicitly selected tool changed during resume')
        return self.tools[key]

    def check_work(self, language: str) -> bool:
        """Qualify copied local Cargo/Go sources, not unrecorded compiler caches."""
        if language not in ('rust', 'tinygo'):
            return True
        if language not in self.work:
            old = self.bound_path('wasm/work/' + language + '/sdk')
            new = self.run.out / 'wasm/work' / language / 'sdk'
            expected, observed = manifest(new), manifest(old)
            allowed = {'examples/rust/Cargo.lock', 'custom-types/application/Cargo.lock'} if language == 'rust' else set()
            self.work[language] = (old.is_dir() and all(observed.get(name) == item for name, item in expected.items())
                                   and set(observed) - set(expected) <= allowed)
        return self.work[language]

    def copy_product(self, row: dict, argv: list[str], validation_argv: list[str]) -> tuple[dict | None, str | None]:
        """Copy only an admitted prior compile/validator pair to a new artifact.

        Missing/failed/unmatched candidates are freshly compiled. A recorded
        successful log whose retained bytes changed is an integrity error. The
        artifact's first available hash is explicitly NOW at admission, rather
        than an invented historic pre/post hash. No new compile command record
        is appended: original command provenance remains nested in reused_compile.
        """
        if not re.fullmatch(r'[A-Za-z0-9_-]+', row['id']):
            raise ValueError('unsafe compiler row ID')
        compile_row = self.by_name.get(row['id'] + '-compile')
        validate_row = self.by_name.get(row['id'] + '-validate')
        for candidate in (compile_row, validate_row):
            if (candidate is None or candidate.get('status') != 'PASS' or
                type(candidate.get('returncode')) is not int or candidate['returncode'] != 0 or candidate.get('error')):
                return None, 'PRIOR_COMMAND_MISSING_FAILED_OR_PARTIAL'
        for candidate, expected, cwd in ((compile_row, argv, row['cwd']),
                                         (validate_row, validation_argv, str(self.run.snapshot))):
            if ([self.normalize(value) for value in candidate.get('argv', [])] != expected or
                self.normalize(candidate.get('cwd', '')) != cwd):
                return None, 'PRIOR_EXACT_ARGV_OR_CWD_MISMATCH'
        if not self.check_work(row['language']):
            return None, 'PRIOR_COPIED_COMPILER_INPUTS_MISSING_OR_DIFFERENT'
        logs = []
        for candidate in (compile_row, validate_row):
            expected_path = self.bound_path('logs/' + candidate['name'] + '.log')
            if Path(candidate.get('log', '')).resolve() != expected_path or not expected_path.exists():
                return None, 'PRIOR_BOUND_LOG_MISSING_OR_WRONG_PATH'
            _, binding = regular_binding(expected_path, capture=False)
            if binding['bytes'] != candidate.get('log_bytes') or binding['sha256'] != candidate.get('log_sha256'):
                raise ValueError('successful prior command log no longer matches progress pin: ' + candidate['name'])
            logs.append(binding)
        old_path = self.bound_path('wasm/' + row['id'] + '/guest.wasm')
        if not old_path.exists():
            return None, 'PRIOR_COMPILER_PRODUCT_NOT_RETAINED'
        data, old_binding = regular_binding(old_path)
        destination = Path(row['artifact'])
        if destination.exists() or not destination.is_relative_to(self.run.out):
            raise ValueError('resume copy destination must be a fresh new-run artifact')
        tools = [self.current_tool(Path(argv[0])), self.current_tool(Path(validation_argv[0]))]
        if row['language'] == 'assemblyscript':
            tools.append(self.current_tool(Path(argv[1])))
        if row['language'] == 'rust' and self.run.args.rustc:
            tools.append(self.current_tool(self.run.args.rustc))
        if row['language'] == 'tinygo' and self.run.args.go:
            tools.append(self.current_tool(self.run.args.go))
        descriptor = os.open(destination, os.O_WRONLY | os.O_CREAT | os.O_EXCL | getattr(os, 'O_NOFOLLOW', 0), 0o600)
        with os.fdopen(descriptor, 'wb') as output:
            output.write(data)
            output.flush()
            os.fsync(output.fileno())
        _, copied = regular_binding(destination, capture=False)
        _, old_after = regular_binding(old_path, capture=False)
        if old_after != old_binding or (copied['bytes'], copied['sha256']) != (old_binding['bytes'], old_binding['sha256']):
            raise ValueError('retained/copied compiler product changed during admission')
        proof = {'status': 'PRIOR_COMPILE_PRODUCT_COPIED', 'new_compiler_execution': 'NOT_PERFORMED',
                 'original_compile_command': compile_row, 'original_validator_command': validate_row,
                 'original_progress_binding': self.records['progress.json']['binding'],
                 'original_source_manifest_binding': self.records['source-manifest.json']['binding'],
                 'original_context_manifest_binding': self.records['context-manifest.json']['binding'],
                 'original_final_report': self.admission['prior_final_report'], 'original_log_bindings': logs,
                 'artifact_at_resume_admission': old_binding, 'copied_artifact_binding': copied,
                 'historic_artifact_sha256': 'NOT_RECORDED', 'current_tool_fingerprints_at_admission': tools,
                 'historic_tool_fingerprints': 'NOT_RECORDED', 'external_toolchain_closure': 'UNQUALIFIED'}
        self.admission['reused_rows'].append({'id': row['id'], 'artifact_sha256': copied['sha256']})
        self.save()
        return proof, None

    def save(self) -> None:
        """Persist admission provenance only in the new run, atomically."""
        write_json_atomic(self.run.out / 'resume-admission.json', self.admission)

    def finish(self) -> dict:
        """Recheck the frozen prior evidence after all current checks finish."""
        for name, record in self.records.items():
            _, now = regular_binding(self.bound_path(name), capture=False)
            if now != record['binding']:
                raise ValueError('prior resume evidence changed during the current run: ' + name)
        if manifest(self.prior / 'source/sdk') != self.records['source-manifest.json']['value']:
            raise ValueError('prior frozen SDK changed during the current run')
        self.admission['prior_records_still_preserved'] = True
        self.admission['copied_compiler_products'] = len(self.admission['reused_rows'])
        self.save()
        _, binding = regular_binding(self.run.out / 'resume-admission.json', capture=False)
        return binding


def manifest(root: Path) -> dict:
    """Hash regular SDK inputs; preserve symlink text without following it."""
    result = {}
    for path in sorted(root.rglob('*')):
        relative = path.relative_to(root)
        if any(part in ('__pycache__', 'target', 'node_modules', '.git', 'build')
               for part in relative.parts):
            continue
        if path.is_symlink():
            result[str(relative)] = {'symlink': os.readlink(path)}
        elif path.is_file():
            data = path.read_bytes()
            result[str(relative)] = {'bytes': len(data),
                                    'sha256': hashlib.sha256(data).hexdigest()}
    return result


def copy_inputs(out: Path, policy: bool = False) -> tuple[Path, dict]:
    """Freeze SDK and its read-only registry context before running any check."""
    before = manifest(PROJECT / 'sdk')
    snapshot = out / 'source'
    shutil.copytree(PROJECT / 'sdk', snapshot / 'sdk', symlinks=True,
                    ignore=shutil.ignore_patterns('__pycache__', 'target',
                                                 'node_modules', '.git', 'build'))
    for name in ('LICENSE.md', 'src/uwvm2/uwvm/imported/ros/host.h',
                 'test/0073.ros-tests/support/source/ros_cgroup/probe.py'):
        source = PROJECT / name
        destination = snapshot / name
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, destination)
    support='test/0073.ros-tests/support'
    shutil.copytree(PROJECT/support/'source',snapshot/support/'source',
        ignore=shutil.ignore_patterns('__pycache__','target','node_modules','.git','build'),dirs_exist_ok=True)
    for name in ('source_paths.py','stage_sdk_tests.py','resource_contract.py','owner_guards.py',
            'sdk-test-source-map.json','sdk-inline-test-extraction.json'):
        shutil.copyfile(PROJECT/support/name,snapshot/support/name)
    shutil.copytree(PROJECT/support/'records/sdk-test-migration',
        snapshot/support/'records/sdk-test-migration')
    # Resolve repository-relative documentation links against frozen real files.
    # Never invent placeholders or let this verifier read the live checkout.
    for guide in sorted((PROJECT / 'sdk').rglob('*.md')):
        body = re.sub(r'^```[^\n]*\n.*?^```[^\n]*$', '', guide.read_text(),
                      flags=re.MULTILINE | re.DOTALL)
        for link in re.findall(r'\[[^\]\n]+\]\(([^\)\n]+)\)', body):
            parsed = urlsplit(link.strip().strip('<>'))
            if parsed.scheme or not parsed.path or parsed.path.startswith('/'):
                continue
            linked = (guide.parent / unquote(parsed.path)).resolve()
            if linked.is_relative_to(PROJECT / 'sdk') or not linked.is_relative_to(PROJECT):
                continue
            if linked.is_file():
                destination = snapshot / linked.relative_to(PROJECT)
                destination.parent.mkdir(parents=True, exist_ok=True)
                if not destination.exists():shutil.copyfile(linked, destination)
    if policy:
        for relative in ('src/uwvm2/uwvm/imported/ros', 'third-parties/fast_io/include', 'src/ros/gateway/common'):
            source, destination = PROJECT / relative, snapshot / relative
            held = manifest(source)
            shutil.copytree(source, destination, symlinks=True,dirs_exist_ok=True)
            if manifest(source) != held or manifest(destination) != held:
                raise RuntimeError('policy validation context changed while copying')
    for source in sorted((PROJECT / 'sdk').rglob('*')):
        if source.suffix not in ('.c', '.h', '.cpp', '.hpp'):
            continue
        for name in re.findall(r'^\s*#include\s+"([^"]+)"', source.read_text(), re.MULTILINE):
            if (source.parent / name).exists():
                continue
            linked = (PROJECT / name).resolve()
            if linked.is_file() and linked.is_relative_to(PROJECT) and not linked.is_relative_to(PROJECT / 'sdk'):
                destination = snapshot / linked.relative_to(PROJECT)
                destination.parent.mkdir(parents=True, exist_ok=True)
                if not destination.exists():
                    shutil.copyfile(linked, destination)
    relay_models = PROJECT / 'test/0073.ros-tests/support/source/ros_sdk_relay'
    for source in sorted(relay_models.glob('javascript_*models.mjs')):
        destination = snapshot / 'test/0073.ros-tests/support/source/ros_sdk_relay' / source.name
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, destination)
    if manifest(snapshot / 'sdk') != before or manifest(PROJECT / 'sdk') != before:
        raise RuntimeError('SDK changed while copying; finish source edits first')
    (out / 'source-manifest.json').write_text(json.dumps(before, indent=2) + '\n')
    context = {str(path.relative_to(snapshot)): {
        'bytes': path.stat().st_size,
        'sha256': hashlib.sha256(path.read_bytes()).hexdigest()}
        for path in sorted(snapshot.rglob('*'))
        if path.is_file() and not path.is_relative_to(snapshot / 'sdk')}
    (out / 'context-manifest.json').write_text(json.dumps(context, indent=2) + '\n')
    return snapshot, before


def group_rss(group: int) -> tuple[int, list[int]]:
    """Read one process-group inventory, in KiB, on macOS or Linux."""
    result = subprocess.run(['ps', '-axo', 'pid=,pgid=,rss='], check=True,
                            capture_output=True, text=True, timeout=5)
    rss, pids = 0, []
    for line in result.stdout.splitlines():
        fields = line.split()
        if len(fields) == 3 and int(fields[1]) == group:
            pids.append(int(fields[0]))
            rss += int(fields[2])
    return rss, pids


def stop_group(child: subprocess.Popen) -> None:
    """Retire only the fresh command group owned by this runner."""
    for sig, seconds in ((signal.SIGTERM, 2), (signal.SIGKILL, 2)):
        try:
            os.killpg(child.pid, sig)
        except ProcessLookupError:
            break
        try:
            child.wait(timeout=seconds)
            break
        except subprocess.TimeoutExpired:
            continue


class Runner:
    """Execute one bounded command at a time and retain every failure log."""

    def __init__(self, out: Path, snapshot: Path, args: argparse.Namespace):
        self.out, self.snapshot, self.args = out, snapshot, args
        proof=validate_resource_owner()
        if not out.resolve().is_relative_to(Path(proof['run'])/'work'):
            raise ValueError('test runner output must remain below its actual finite owner work directory')
        self.resume = None
        self.rows = []
        self.env = {
            'PATH': os.defpath, 'HOME': str(out / 'home'), 'TMPDIR': str(out / 'tmp'),
            'LC_ALL': 'C', 'PYTHONDONTWRITEBYTECODE': '1',
            'UWVM_RESOURCE_RUN':proof['run'],
            'UWVM_RESOURCE_HEAVY_LOCK_HELD':'1',
            'CARGO_HOME': str(out / 'cargo-home'),
            'CARGO_TARGET_DIR': str(out / 'cargo-target'), 'CARGO_BUILD_JOBS': '1',
            'RUSTC': str(args.rustc or ''), 'RUSTC_WRAPPER': '',
            'RUSTDOC': str(args.rustc.with_name('rustdoc') if args.rustc else ''),
            'RUSTC_WORKSPACE_WRAPPER': '', 'RUST_TEST_THREADS': '1',
            'GOMAXPROCS': '1', 'GOMEMLIMIT': '192MiB', 'GOENV': 'off',
            'GOTOOLCHAIN': 'local', 'GOPROXY': 'off', 'GOSUMDB': 'off',
            'GOCACHE': str(out / 'go-cache'), 'GOTMPDIR': str(out / 'tmp'),
            'CGO_ENABLED': '0', 'UV_THREADPOOL_SIZE': '1',
            'NODE_OPTIONS': '--max-old-space-size=96 --v8-pool-size=1',
            'XDG_CACHE_HOME': str(out / 'cache'),
            'GOMODCACHE': str(out / 'go-mod-cache'),
        }
        for name in ('home', 'tmp', 'cargo-home', 'cargo-target', 'go-cache', 'logs',
                     'cache', 'go-mod-cache'):
            (out / name).mkdir()

    def run(self, name: str, argv: list[str], cwd: Path | None = None) -> bool:
        """Fail on timeout, RSS excess, nonzero exit or surviving children."""
        selected = cwd or self.snapshot
        if not selected.resolve().is_relative_to(self.out):
            raise RuntimeError('command working directory escapes /tmp run')
        log = self.out / 'logs' / (name + '.log')
        row = {'name': name, 'argv': argv, 'cwd': str(selected), 'log': str(log),
               'status': 'RUNNING', 'peak_group_rss_kib': 0}
        if self.env.get('CARGO_ENCODED_RUSTFLAGS'):
            row['explicit_rustc_tokens'] = self.env['CARGO_ENCODED_RUSTFLAGS'].split('\x1f')
        self.rows.append(row)
        self.save()
        print(json.dumps({'start': name}), flush=True)
        child = None
        try:
            with log.open('xb') as output:
                child = subprocess.Popen(argv, cwd=selected, env=self.env,
                                         stdout=output, stderr=subprocess.STDOUT,
                                         start_new_session=True)
                begun = time.monotonic()
                last_saved = begun
                while child.poll() is None:
                    rss, _ = group_rss(child.pid)
                    row['peak_group_rss_kib'] = max(row['peak_group_rss_kib'], rss)
                    if rss > self.args.rss_limit_mib * 1024:
                        raise RuntimeError('aggregate command RSS limit exceeded')
                    if time.monotonic() - begun > self.args.timeout_seconds:
                        raise RuntimeError('command deadline exceeded')
                    if time.monotonic() - last_saved >= 2:
                        self.save()
                        last_saved = time.monotonic()
                    time.sleep(.1)
                row['returncode'] = child.returncode
                row['seconds'] = round(time.monotonic() - begun, 3)
                _, survivors = group_rss(child.pid)
                if survivors:
                    raise RuntimeError('command left owned child processes')
                row['status'] = 'PASS' if child.returncode == 0 else 'FAIL'
        except (OSError, subprocess.SubprocessError, RuntimeError) as error:
            row['status'], row['error'] = 'FAIL', str(error)
        finally:
            if child is not None:
                stop_group(child)
                row['returncode'] = child.returncode
            if log.exists():
                _, binding = regular_binding(log, capture=False)
                row['log_bytes'], row['log_sha256'] = binding['bytes'], binding['sha256']
            self.save()
        print(json.dumps({'finish': name, 'status': row['status'],
                          'peak_rss_mib': round(row['peak_group_rss_kib'] / 1024, 1)}), flush=True)
        return row['status'] == 'PASS'

    def save(self) -> None:
        """Persist incremental rows; an interrupted plan cannot look complete."""
        write_json_atomic(self.out / 'progress.json', self.rows)


def source_checks(run: Runner) -> None:
    """Check registry/bindings/vectors and pure JS contracts serially."""
    sdk = run.snapshot / 'sdk/origin'
    scripts = ('tools/abi_contract.py', 'tools/check_bindings.py', 'tools/wire_vectors.py',
               'tests/test_import_inspector.py', 'tests/artifact_read_models.py',
               'rust/tests/check_sources.py')
    for relative in scripts:
        run.run('source-' + Path(relative).stem,
                [sys.executable, '-B', str(_sdk_path(sdk,relative))])
    if run.args.node:
        run.env['PATH'] = str(run.args.node.parent) + os.pathsep + os.defpath
        # Parse every shipped JS entry, including examples and generated codecs,
        # without running a host setup module, Worker or ROS operation.
        for entry in sorted(sdk.rglob('*.mjs')):
            relative = entry.relative_to(sdk).as_posix()
            run.run('js-syntax-' + relative.replace('/', '_').removesuffix('.mjs'),
                    [str(run.args.node), '--check', str(entry)])
        run.run('source-complete-verification', [sys.executable, '-B',
                str(_sdk_path(sdk,'tools/verify_sources.py')), '--node', str(run.args.node),
                '--report', str(run.out / 'complete-source-verification.json')])
        run.run('source-custom-codec-models', [sys.executable, '-B',
                str(_sdk_path(sdk,'custom-types/tests/source_models.py')),
                '--report', str(run.out / 'custom-codec-models.json')])
        example = str(sdk / 'custom-types/examples/use_generated.mjs')
        run.run('js-custom-example-roundtrip', [str(run.args.node), '--input-type=module', '--eval',
            'import assert from "node:assert/strict"; import {pathToFileURL} from "node:url"; '
            'const example=await import(pathToFileURL(' + json.dumps(example) + ').href); '
            'assert.deepEqual(example.localRoundtrip(),example.makePacket()); '
            'console.log("PASS handwritten custom JS example roundtrip; no ROS imports invoked");'])
        for relative in ('javascript/tests/test.mjs',
                         'javascript/tests/factory_transactions.mjs',
                         'javascript/tests/executor_contracts.mjs',
                         'javascript/tests/unified_events_models.mjs',
                 'javascript/tests/event_dispatch_contracts.mjs',
                         'javascript/tests/run_pure_wasm_options.mjs'):
            run.run('js-' + Path(relative).stem, [str(run.args.node), str(_sdk_path(sdk,relative))])
        for name in ('javascript_protocol_models', 'javascript_api_models'):
            # The complete model outcome is retained in this owner's command
            # log. These entry points accept an optional legacy receipt path.
            run.run('js-' + name, [str(run.args.node),
                    str(run.snapshot / 'test/0073.ros-tests/support/source/ros_sdk_relay' / (name + '.mjs'))])
    else:
        run.rows.append({'name': 'js-contract-models', 'status': 'BLOCKED',
                         'reason': 'explicit Node required for complete source group'})


def native_checks(run: Runner, selected: tuple[str, ...] = ('c', 'cpp', 'rust', 'go')) -> None:
    """Compile contract fixtures without optimization or parallel build jobs."""
    origin=run.snapshot/'sdk/origin'
    sdk = getattr(run, 'installed_sdk', origin)
    for language, compiler, standard in (('c', run.args.cc, 'c11'),
                                          ('cpp', run.args.cxx, 'c++20')):
        if language not in selected:
            continue
        sources = sorted(_sdk_path(origin,'tests',language).glob('*.' + ('c' if language == 'c' else 'cpp')))
        if not compiler or not sources:
            run.rows.append({'name': language + '-models', 'status': 'BLOCKED',
                             'reason': 'explicit compiler and nonempty fixtures required'})
            continue
        for source in sources:
            name = language + '-' + source.stem
            binary = run.out / name
            includes=([sdk/'include',*[sdk/'include/platform'/p/l for p in ('ros','ros1','ros2')for l in ('c','cpp')]]
                if hasattr(run,'installed_sdk')else[origin/'include'])
            argv = [str(compiler), '-std=' + standard, '-O0', '-g0', '-UNDEBUG',
                    *[arg for path in includes for arg in ('-I',str(path))],str(source), '-o', str(binary)]
            if run.run(name + '-compile', argv):
                run.run(name + '-execute', [str(binary)])
    if 'rust' in selected and run.args.cargo and run.args.rustc:
        work = run.out / 'native-rust-work/sdk'
        shutil.copytree(origin / 'rust', work / 'rust')
        _stage_sdk_tests('rust',work/'rust',test_root=run.snapshot/'test/0073.ros-tests')
        shutil.copyfile(origin / 'LICENSE', work / 'LICENSE')
        run.run('rust-host-models', [str(run.args.cargo), 'test', '--offline', '-j1',
                                    '--no-default-features', '--features', 'alloc',
                                    '--', '--test-threads=1'], work / 'rust')
    elif 'rust' in selected:
        run.rows.append({'name': 'rust-host-models', 'status': 'BLOCKED',
                         'reason': 'explicit cargo/rustc required'})
    if 'go' in selected and run.args.go:
        work = run.out / 'native-go-work'
        shutil.copytree(origin / 'tinygo', work / 'tinygo')
        _stage_sdk_tests('tinygo',work/'tinygo',test_root=run.snapshot/'test/0073.ros-tests')
        run.run('go-host-models', [str(run.args.go), 'test', '-v', '-count=1',
                                  '-p', '1', '-parallel', '1', './...'], work / 'tinygo')
    elif 'go' in selected:
        run.rows.append({'name': 'go-host-models', 'status': 'BLOCKED',
                         'reason': 'explicit Go required'})


def cmake_checks(run: Runner) -> None:
    """Install the whole SDK, relocate it, and verify a fresh consumer package."""
    if not run.args.cmake:
        run.rows.append({'name': 'cmake-relocation', 'status': 'BLOCKED',
                         'reason': 'explicit CMake required'})
        return
    tool, build = str(run.args.cmake), run.out / 'cmake-build'
    first, second = run.out / 'prefix-a', run.out / 'prefix-b'
    if not run.run('cmake-configure', [tool, '-S', str(run.snapshot / 'sdk'), '-B', str(build),
        '-DCMAKE_INSTALL_PREFIX=' + str(first), '-DCMAKE_INSTALL_DATADIR=share', '-DCMAKE_INSTALL_LIBDIR=lib']):
        return
    if not run.run('cmake-install', [tool, '--install', str(build)]):
        return
    first.rename(second)
    installed = second
    before = manifest(installed)
    consumer = run.out / 'cmake-consumer'
    consumer.mkdir()
    (consumer / 'CMakeLists.txt').write_text('''# Local relocation model; no compiler or native ROS selected.
cmake_minimum_required(VERSION 3.16)
project(roswasm_relocation LANGUAGES NONE)
find_package(roswasm CONFIG REQUIRED)
foreach(name c cpp cpp11 cpp20)
  if(NOT TARGET roswasm::${name})
    message(FATAL_ERROR "Missing imported SDK target")
  endif()
endforeach()
get_target_property(paths roswasm::c INTERFACE_INCLUDE_DIRECTORIES)
file(REAL_PATH "${EXPECTED_PREFIX}/include/platform/ros/c" expected)
foreach(entry IN LISTS paths)
  file(REAL_PATH "${entry}" actual)
  if(NOT actual STREQUAL "${EXPECTED_PREFIX}/include" AND NOT actual MATCHES "^${EXPECTED_PREFIX}/include/platform/(ros|ros1|ros2)/c$")
    message(FATAL_ERROR "SDK include path did not relocate")
  endif()
endforeach()
foreach(header IN ITEMS c/roswasm_unitree/crc.h cpp/roswasm_unitree/crc.hpp c/roswasm_unitree/topic_rpc.h)
  if(NOT EXISTS "${EXPECTED_PREFIX}/include/platform/unitree/${header}")
    message(FATAL_ERROR "Missing installed Unitree header ${header}")
  endif()
endforeach()
get_target_property(features roswasm::cpp20 INTERFACE_COMPILE_FEATURES)
if(NOT "cxx_std_20" IN_LIST features)
  message(FATAL_ERROR "Missing C++20 interface requirement")
endif()
add_library(consumer INTERFACE)
target_link_libraries(consumer INTERFACE roswasm::cpp)
''')
    if run.run('cmake-relocated-consumer', [tool, '-S', str(consumer), '-B', str(run.out / 'cmake-consumer-build'),
        '-Droswasm_DIR=' + str(second / 'lib/cmake/roswasm'), '-DEXPECTED_PREFIX=' + str(second)]):
        if manifest(installed) != before:
            run.rows.append({'name': 'cmake-installed-inputs', 'status': 'FAIL'})
        else:
            run.installed_sdk = installed
            run.installed_manifest = before
            (run.out / 'installed-source-manifest.json').write_text(json.dumps(before, indent=2) + '\n')


def policy_checks(run: Runner) -> None:
    """Generate config bytes and check actual policy parsers without applying rights."""
    if not run.args.cxx:
        run.rows.append({'name': 'policy-contracts', 'status': 'BLOCKED',
                         'reason': 'explicit native C++ compiler required'})
        return
    sdk, out = run.snapshot / 'sdk/origin', run.out / 'policy-models'
    if not run.run('policy-generator-cli', [sys.executable, '-B',
            str(_sdk_path(sdk,'tests/policy/prepare.py')), str(out)]):
        return
    binary = run.out / 'policy-contract'
    # These GCC-only settings collect the compiler's garbage more often and
    # omit its optional macro-expansion diagnostic history. The actual policy
    # parsers, assertions, exception paths and generated code are unchanged.
    # Resolve symlinks before recognizing an explicitly selected GCC driver;
    # Clang/WASI and other compiler families keep their ordinary argv.
    memory_flags = (['--param=ggc-min-expand=5', '--param=ggc-min-heapsize=16384',
                     '-ftrack-macro-expansion=0']
                    if re.search(r'(?:^|-)g\+\+(?:-\d+)?$', run.args.cxx.resolve().name) else [])
    if run.run('policy-contract-compile', [str(run.args.cxx), '-std=c++20', '-O0', '-g0', '-UNDEBUG',
        *memory_flags,
        '-I', str(run.snapshot / 'src'), '-I', str(run.snapshot / 'third-parties/fast_io/include'),
        '-I', str(run.snapshot), str(_sdk_path(sdk,'tests/policy/contract.cpp')), '-o', str(binary)]):
        configs = json.loads((out / 'configs.json').read_text())
        run.run('policy-contract-execute', [str(binary), *configs])


def scheduling_checks(run: Runner) -> None:
    """Execute compiled local lessons with basic-I/O mathematical imports."""
    if not run.args.node or not run.args.compiler_reports:
        run.rows.append({'name': 'compiled-scheduling-models', 'status': 'BLOCKED',
            'reason': 'explicit Node and current local compiler reports required'})
        return
    selected, bindings = [], []
    for report in run.args.compiler_reports:
        if not report.is_absolute():raise ValueError('explicit absolute compiled-model report required')
        data,report_binding=regular_binding(report)
        content = json.loads(data)
        if content['status'] != 'LOCAL_SDK_CHECKS_PASS':
            raise ValueError('compiled model source report is not a pass')
        bindings.append(report_binding)
        for row in content['wasm_compilation_rows']:
            name = Path(row['source']).stem
            if ((row['language'] in ('rust', 'tinygo') and row['id'].endswith('-scheduling')) or
                (row['language'] == 'assemblyscript' and name == '07-scheduling') or
                (row['language'] in ('c', 'cpp') and name == '08_context_future_timer' and row['id'].endswith('-defer0'))):
                selected.append(row)
    path = run.out / 'scheduling-inputs.json'
    path.write_text(json.dumps(selected, indent=2) + '\n')
    (run.out / 'compiler-report-bindings.json').write_text(json.dumps(bindings, indent=2) + '\n')
    run.run('compiled-scheduling-models', [str(run.args.node),
        str(_sdk_path(run.snapshot/'sdk/origin','tools/run_local_scheduling_models.mjs')),
        str(path), str(run.out / 'scheduling-models.json')])


def wasm_checks(run: Runner) -> list[dict]:
    """Compile real example Wasm serially, inspect imports and fully validate it.

    Only the existing compiler driver's source-only recipe constructor is
    reused. Its actual Linux/cgroup execution entrypoint is never invoked.
    Local output receives a separate scope and cannot qualify that driver.
    """
    sdk, args = run.snapshot / 'sdk/origin', run.args
    spec = importlib.util.spec_from_file_location('local_sdk_recipe_source', _sdk_path(sdk,'tools/build_cgroup_matrix.py'))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    base = run.out / 'wasm'
    base.mkdir()
    rows = module.recipes(args, base)
    if args.example_variants:
        # Conditional tutorial modes must compile too, not merely their
        # default main. Each variant retains an independent artifact and log.
        cases = {'04_service_action_resources': 7, '04_services_actions': 7,
                 '05_queries_and_events': 26, '06_queries_and_control': 26,
                 '07_service_action_servers': 3, '05_service_action_servers': 3}
        expanded = []
        for row in rows:
            name = Path(row['source']).stem
            variants = None
            if row['language'] in ('c', 'cpp'):
                if name in cases:
                    variants = [('case' + str(i), '-DROSWASM_EXAMPLE_CASE=' + str(i)) for i in range(cases[name])]
                elif name == '08_context_future_timer':
                    variants = [('defer' + str(i), '-DROSWASM_EXAMPLE_DEFER_RPC=' + str(i)) for i in range(2)]
                elif name == '09_session_ownership':
                    variants = [('revoke' + str(i), '-DROSWASM_EXAMPLE_REVOKE=' + str(i)) for i in range(2)]
                elif name == '06_runtime_delivery':
                    variants = [('denied' + str(i), '-DROSWASM_EXAMPLE_DEFAULT_DENIED=' + str(i)) for i in range(2)]
                elif name == '10_action_client_bundle':
                    # Each explicit optional authority and cleanup mode must
                    # compile with every owned-goal / local-Future flow.
                    variants = [('case' + str(case) + '-any' + str(any_role) + '-revoke' + str(revoke),
                                 ('-DROSWASM_EXAMPLE_CASE=' + str(case),
                                  '-DROSWASM_EXAMPLE_CANCEL_ANY=' + str(any_role),
                                  '-DROSWASM_EXAMPLE_REVOKE=' + str(revoke)))
                                for case in range(5) for any_role in range(2) for revoke in range(2)]
            if variants is None:
                expanded.append(row)
                continue
            for suffix, define in variants:
                variant = dict(row)
                variant['id'] = row['id'] + '-' + suffix
                variant['rowdir'] = str(base / variant['id'])
                variant['artifact'] = str(base / variant['id'] / 'guest.wasm')
                variant['argv'] = [variant['artifact'] if arg == row['artifact'] else arg for arg in row['argv']]
                defines = list(define) if isinstance(define, tuple) else [define]
                variant['argv'][1:1] = defines
                variant['variant'] = defines
                expanded.append(variant)
        rows = expanded
    schema = json.loads((sdk / 'abi/abi-v1.json').read_text())
    for backend in args.backends or ['1', '2']:
        names = ['uwvm_ros' + backend + '_' + f['name']
                 for f in schema['modules']['uwvm_ros' + backend + '_v1']['functions']]
        (base / ('allowed-ros' + backend + '-symbols.txt')).write_text('\n'.join(names) + '\n')
    for language in ('rust', 'tinygo'):
        if any(row['language'] == language for row in rows):
            target = base / 'work' / language / 'sdk'
            shutil.copytree(sdk / language, target / language)
            shutil.copytree(sdk / 'examples' / language, target / 'examples' / language)
            shutil.copyfile(sdk / 'LICENSE', target / 'LICENSE')
            if any(row.get('custom_example') and row['language'] == language for row in rows):
                module.copy_custom_example_work(sdk, target, language)
    if args.go:
        # TinyGo invokes its selected Go tool by name; supply only that trusted
        # tool directory ahead of the ordinary system tools, never user PATH.
        run.env['PATH'] = str(args.go.parent) + os.pathsep + os.defpath
    if args.tinygo:
        run.env['TINYGOROOT'] = str(args.tinygo.resolve().parent.parent)
    results = []
    results_path = run.out / 'wasm-results.json'
    for row in rows:
        result = dict(row, status='BLOCKED', actual_ROS_delivery='NOT_EXECUTED')
        results.append(result)
        write_json_atomic(results_path, results)
        Path(row['rowdir']).mkdir()
        if row['missing_configuration'] or not Path(row['source']).is_file():
            result['reason'] = 'explicit existing tools and required source needed'
            write_json_atomic(results_path, results)
            continue
        argv = list(row['argv'])
        if row['language'] == 'rust':
            if '-j1' not in argv:
                argv.insert(2, '-j1')
            # Cargo's exact output path is copied only after a successful build.
            try:
                flags, archive = module.rust_wasi_link_inputs(args.wasi_sdk,
                    coverage=Path(row['source']).stem == 'coverage')
                run.env.update(RUSTFLAGS='', CARGO_ENCODED_RUSTFLAGS='\x1f'.join(flags))
                result.update(rust_target_libc=archive, explicit_rustc_tokens=flags)
            except (OSError, ValueError) as error:
                result['status'], result['error'] = 'FAIL_TARGET_LIBC_INPUT', str(error)
                write_json_atomic(results_path, results)
                continue
        else:
            run.env.pop('RUSTFLAGS', None)
            run.env.pop('CARGO_ENCODED_RUSTFLAGS', None)
        if row['language'] in ('c', 'cpp', 'javascript_bridge'):
            argv = ['-O0' if value == '-O2' else value for value in argv]
            argv.insert(1, '-Wl,--threads=1')
        if row['language'] == 'tinygo':
            argv[2:2] = ['-p', '1', '-opt=0']
        result['planned_compile_argv'] = argv
        validation_argv = [str(args.wasm_tools), 'validate', row['artifact']]
        reused = None
        if run.resume is not None:
            try:
                reused, refusal = run.resume.copy_product(row, argv, validation_argv)
                if reused is not None:
                    result['reused_compile'] = reused
                    result['current_compiler_execution'] = 'NOT_PERFORMED_PRIOR_PRODUCT_REUSED'
                else:
                    result['resume_reuse_refusal'] = refusal
            except (OSError, ValueError) as error:
                result['status'], result['error'] = 'FAIL_RESUME_INTEGRITY_OR_COPY', str(error)
                write_json_atomic(results_path, results)
                break
        if reused is None:
            result['current_compiler_execution'] = 'PERFORMED'
            result['actual_argv'] = argv
            compiled = run.run(row['id'] + '-compile', argv, Path(row['cwd']))
            if row['language'] == 'rust':
                try:
                    _, result['rust_target_libc_after_compile'] = module.rust_wasi_link_inputs(args.wasi_sdk)
                    if result['rust_target_libc_after_compile'] != result['rust_target_libc']:
                        raise ValueError('Rust target libc changed during compilation')
                except (OSError, ValueError) as error:
                    result['status'], result['error'] = 'FAIL_TARGET_LIBC_BINDING', str(error)
                    write_json_atomic(results_path, results)
                    continue
            if not compiled:
                result['status'] = 'FAIL_COMPILE'
                write_json_atomic(results_path, results)
                continue
        artifact = Path(row['artifact'])
        if row['language'] == 'rust' and reused is None:
            built = base / ('cargo-target-ros' + row['backend']) / args.rust_target / 'release' / (Path(row['source']).stem + '.wasm')
            if not built.is_file():
                result['status'] = 'FAIL_EXPECTED_CARGO_ARTIFACT'
                write_json_atomic(results_path, results)
                continue
            shutil.copyfile(built, artifact)
        data = None
        try:
            data, artifact_binding = module.read_artifact(artifact)
            result['artifact_binding'] = artifact_binding
            if reused is not None:
                copied = reused['copied_artifact_binding']
                expected_binding = {name: copied['bytes' if name == 'size' else name]
                                    for name in ('device', 'inode', 'size', 'mtime_ns', 'ctime_ns', 'sha256')}
                if artifact_binding != expected_binding:
                    raise ValueError('copied resume product changed before current import inspection')
            if row['expects_ros_imports']:
                contract = module.INSPECTOR.check(data, row['backend'], ('wasi_snapshot_preview1',), row['require_all_imports'])
            else:
                imports = module.INSPECTOR.inspect(data)
                if row['language'] == 'assemblyscript' and imports:
                    raise ValueError('pure model must have no imports')
                if any(item['module'] != 'wasi_snapshot_preview1' for item in imports):
                    raise ValueError('local scheduling example imports unexpected authority')
                contract = {'status': 'NO_ROS_IMPORTS', 'imports': imports}
            if row['language'] == 'javascript_bridge' and Path(row['source']).stem == 'relay':
                module.check_relay_imports(contract)
            result['import_contract'] = contract
            result['artifact_bytes'] = len(data)
            result['artifact_sha256'] = hashlib.sha256(data).hexdigest()
        except (OSError, ValueError, RuntimeError) as error:
            result['status'], result['error'] = 'FAIL_IMPORT_CONTRACT', str(error)
            write_json_atomic(results_path, results)
            continue
        finally:
            # The next compiler must not overlap the previous Wasm payload's
            # lifetime. Its exact size/hash/import contract is already retained.
            data = None
        try:
            result['artifact_before_full_validation'] = module.verify_artifact(artifact, artifact_binding)
            valid = run.run(row['id'] + '-validate', validation_argv)
            result['artifact_after_full_validation'] = module.verify_artifact(artifact, artifact_binding)
            if row['language'] == 'rust':
                _, result['rust_target_libc_after'] = module.rust_wasi_link_inputs(args.wasi_sdk)
                if result['rust_target_libc_after'] != result['rust_target_libc']:
                    raise ValueError('Rust target libc changed around compilation/validation')
        except (OSError, ValueError, RuntimeError) as error:
            result['status'], result['error'] = 'FAIL_VALIDATED_ARTIFACT_BINDING', str(error)
            write_json_atomic(results_path, results)
            continue
        if not valid:
            result['status'] = 'FAIL_FULL_VALIDATION'
            write_json_atomic(results_path, results)
            continue
        result['status'] = 'COMPILE_IMPORT_FULL_VALIDATION_PASS'
        write_json_atomic(results_path, results)
    write_json_atomic(results_path, results)
    if args.execute_pure_models:
        if args.node and any(row['language'] == 'assemblyscript' and row['backend'] == 'none' for row in results):
            run.run('assemblyscript-local-pure-models', [str(args.node),
                str(sdk / 'tools/run_local_pure_wasm.mjs'),
                str(run.out / 'wasm-results.json'), str(run.out / 'pure-models.json')])
        else:
            run.rows.append({'name': 'assemblyscript-local-pure-models', 'status': 'BLOCKED',
                'reason': 'explicit Node and complete AssemblyScript model inventory required'})
    if args.execute_relay_models:
        relays = {row['backend']: row for row in results
                  if row['language'] == 'javascript_bridge' and Path(row['source']).stem == 'relay'}
        if args.node and set(relays) == {'1', '2'} and all(
                row['status'] == 'COMPILE_IMPORT_FULL_VALIDATION_PASS' for row in relays.values()):
            run.run('javascript-local-compiled-relay-models', [str(args.node),
                str(run.snapshot / 'test/0073.ros-tests/support/source/ros_sdk_relay/javascript_wasm_relay_models.mjs'),
                relays['1']['artifact'], relays['2']['artifact'],
                str(Path('/tmp') / run.out.relative_to(Path('/tmp').resolve()) / 'compiled-relay-models.json')])
        else:
            run.rows.append({'name': 'javascript-local-compiled-relay-models', 'status': 'BLOCKED',
                'reason': 'explicit Node and both successfully compiled real relays required'})
    return results


def main() -> int:
    """Require fresh output beneath the independently validated finite owner."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--groups', nargs='+', choices=('source', 'native', 'c', 'cpp', 'rust', 'go', 'wasm', 'cmake', 'policy', 'scheduling'),
                        default=['source', 'native'])
    for name in ('cc', 'cxx', 'cargo', 'rustc', 'go', 'node', 'wasi-sdk', 'tinygo', 'asc-js', 'wasm-tools', 'cmake'):
        parser.add_argument('--' + name, type=Path)
    parser.add_argument('--languages', nargs='+', choices=('c', 'cpp', 'rust', 'tinygo', 'assemblyscript', 'javascript_bridge'))
    parser.add_argument('--backends', nargs='+', choices=('1', '2'))
    parser.add_argument('--rust-target', default='wasm32-wasip1', choices=('wasm32-wasip1',))
    parser.add_argument('--execute-pure-models', action='store_true')
    parser.add_argument('--execute-relay-models', action='store_true')
    parser.add_argument('--example-variants', action='store_true')
    parser.add_argument('--resume-from', type=Path,
        help='Absolute prior /tmp local run; only byte-admitted successful Wasm compiler products may be reused, with fresh inspection/validation')
    parser.add_argument('--compiler-reports', nargs='+', type=Path)
    parser.add_argument('--rss-limit-mib', type=int, default=768)
    parser.add_argument('--timeout-seconds', type=int, default=180)
    args = parser.parse_args()
    try:proof=validate_resource_owner()
    except (OSError,ValueError,KeyError)as error:parser.error(str(error))
    out = args.out.resolve()
    if not out.is_relative_to(Path(proof['run'])/'work') or out==Path(proof['run'])/'work':
        parser.error('test outputs must be a fresh directory below the active owner work directory')
    if out.exists() or args.rss_limit_mib <= 0 or args.timeout_seconds <= 0:
        parser.error('fresh output and positive resource limits required')
    if args.resume_from is not None and (not args.resume_from.is_absolute() or 'wasm' not in args.groups):
        parser.error('--resume-from requires an absolute prior /tmp run and a current wasm group')
    out.mkdir(parents=True)
    snapshot, inputs = copy_inputs(out, policy='policy' in args.groups)
    run = Runner(out, snapshot, args)
    if args.resume_from is not None:
        run.resume = ResumeInputs(args.resume_from, run, inputs)
    wasm = None
    for group in args.groups:
        if group == 'source':
            source_checks(run)
        elif group == 'wasm':
            wasm = wasm_checks(run)
        elif group == 'cmake':
            cmake_checks(run)
        elif group == 'policy':
            policy_checks(run)
        elif group == 'scheduling':
            scheduling_checks(run)
        else:
            native_checks(run, ('c', 'cpp', 'rust', 'go') if group == 'native' else (group,))
    snapshot_stable = manifest(snapshot / 'sdk') == inputs
    # Cargo may create a new lock file in this disposable copy; it is recorded,
    # not allowed to silently replace an existing lock input or source file.
    observed = manifest(snapshot / 'sdk')
    added = {k: v for k, v in observed.items() if k not in inputs}
    stable = all(observed.get(k) == v for k, v in inputs.items())
    allowed = set(added) <= {'rust/Cargo.lock'}
    passed = bool(run.rows) and all(r['status'] == 'PASS' for r in run.rows) and stable and allowed
    installed_stable = not hasattr(run, 'installed_sdk') or manifest(run.installed_sdk) == run.installed_manifest
    passed = passed and installed_stable
    if wasm is not None:
        passed = passed and bool(wasm) and all(row['status'] == 'COMPILE_IMPORT_FULL_VALIDATION_PASS' for row in wasm)
    resume_binding = run.resume.finish() if run.resume is not None else None
    report = {'status': 'LOCAL_SDK_CHECKS_PASS' if passed else 'LOCAL_SDK_CHECKS_INCOMPLETE',
            'scope': 'Selected local SDK source/host models and/or serial Wasm compilation, import inspection and full validation; no native ROS/Gateway/QEMU delivery',
              'utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'sdk_source_manifest_sha256': hashlib.sha256((out / 'source-manifest.json').read_bytes()).hexdigest(),
              'source_inputs_preserved': stable, 'snapshot_exactly_unchanged': snapshot_stable,
              'installed_inputs_preserved': installed_stable,
              'new_disposable_work_files': added, 'only_expected_new_work_files': allowed,
              'compile_jobs': 1, 'rss_limit_mib': args.rss_limit_mib, 'rows': run.rows,
              'wasm_compilation_rows': wasm,
              'resume_admission_binding': resume_binding,
              'reused_compiler_products': sum('reused_compile' in row for row in wasm or []),
              'fresh_compiler_commands': sum(row.get('current_compiler_execution') == 'PERFORMED' for row in wasm or []),
              'original_task_complete': False, 'actual_ROS_delivery': 'NOT_EXECUTED'}
    write_json_atomic(out / 'report.json', report)
    print(report['status'], str(out / 'report.json'), flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    raise SystemExit(main())
