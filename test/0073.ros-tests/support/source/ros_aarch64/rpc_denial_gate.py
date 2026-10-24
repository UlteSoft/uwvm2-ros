"""Test-owned post-denial stdin gate; original assertions and native code stay intact."""
import json
import os
from pathlib import Path
import subprocess
import threading
import time

from runtime import digest

DENIALS = ('permission-denied', 'no-capability')
MARKER = b'DENIAL_ASSERTIONS_PASSED\n'
NATIVE_PASS = b'PASS denied typed ParameterEvent subscription\n'
IMPORTS = ''' (import "wasi_snapshot_preview1" "fd_write" (func $denial_gate_write (param i32 i32 i32 i32) (result i32)))
 (import "wasi_snapshot_preview1" "fd_read" (func $denial_gate_read (param i32 i32 i32 i32) (result i32)))
'''
HELPER = ''' (data (i32.const 10272) "DENIAL_ASSERTIONS_PASSED\\0a")
 (func $denial_stdio_gate
  (i32.store (i32.const 10240) (i32.const 10272))
  (i32.store (i32.const 10244) (i32.const 25))
  (call $check (call $denial_gate_write (i32.const 1) (i32.const 10240) (i32.const 1) (i32.const 10256)))
  (if (i32.ne (i32.load (i32.const 10256)) (i32.const 25)) (then (call $exit (i32.const 230))))
  (i32.store (i32.const 10240) (i32.const 10264))
  (i32.store (i32.const 10244) (i32.const 1))
  (call $check (call $denial_gate_read (i32.const 0) (i32.const 10240) (i32.const 1) (i32.const 10256)))
  (if (i32.ne (i32.load (i32.const 10256)) (i32.const 1)) (then (call $exit (i32.const 231))))
  (if (i32.ne (i32.load8_u (i32.const 10264)) (i32.const 71)) (then (call $exit (i32.const 232)))))
'''


def derive(original, scenario):
    """Insert only imports/helper/post-assert call, and invert every insertion."""
    if scenario not in DENIALS or not original.startswith('(module\n'):
        raise RuntimeError('exact original denial module/scenario required')
    if original.count('(memory (export "memory") 1)') != 1 or '$denial_stdio_gate' in original:
        raise RuntimeError('original denial memory/gate contract differs')
    if original.count(' (func (export "_start")\n') != 1 or not original.endswith('\n ))\n'):
        raise RuntimeError('exact original denial function boundary required')
    # The original assertions touch handle0, length64, clock128/136,
    # payload256..4351 and strings8192/8208. The new gate uses10240..10296,
    # wholly within the same one-page memory. No original memory is rewritten.
    if any('(i32.const '+str(offset)+')' in original for offset in (10240, 10244, 10256, 10264, 10272)):
        raise RuntimeError('original denial scratch overlaps post-assert gate')
    after = '  (call $denial_stdio_gate)\n'
    after += ('  (call $unchanged)\n' if scenario == 'permission-denied' else
        '  (if (i64.ne (i64.load (i32.const 0)) (i64.const 1234605616436508552)) (then (call $exit (i32.const 207))))\n')
    result = original.replace('(module\n', '(module\n'+IMPORTS, 1)
    result = result.replace(' (func (export "_start")\n', HELPER+' (func (export "_start")\n', 1)
    result = result[:-4]+after+' ))\n'
    restored = result.replace('(module\n'+IMPORTS, '(module\n', 1)
    restored = restored.replace(HELPER+' (func (export "_start")\n', ' (func (export "_start")\n', 1)
    if not restored.endswith(after+' ))\n'):
        raise RuntimeError('post-assert gate derivation cannot be reversed')
    restored = restored[:-len(after+' ))\n')]+' ))\n'
    if restored != original:
        raise RuntimeError('post-assert gate modified an original assertion byte')
    return result


def process_identity(process):
    proc = Path('/proc')/str(process.pid)
    fields = (proc/'stat').read_text().rsplit(')', 1)[1].split()
    return {'pid': process.pid, 'starttime': int(fields[19]),
            'membership': (proc/'cgroup').read_text().splitlines()}


class PostDenialGate:
    """Compile derived denial Guests and release each only after its native PASS."""

    def __init__(self, network, rpc):
        self.network, self.rpc = network, rpc
        self.original_compile = rpc.RpcRunner.compile_typed_events
        self.original_event = rpc.RpcRunner.typed_event
        self.original_start = network.Runner.start
        self.contexts, self.rows, self.compilations = {}, [], []
        self.installed = False

    def prepare(self, receiver):
        self.original_compile(receiver)
        directory = receiver.run/'post-denial-gate';directory.mkdir(mode=0o700)
        for row in receiver.typed_event_compilations:
            scenario = next((value for value in DENIALS if row['name'].endswith(value.replace('-', '_'))), None)
            if scenario is None:continue
            original_path = Path(row['source'])
            original_sha = digest(original_path)
            if original_sha != row['source_sha256']:
                raise RuntimeError('original denial fixture changed after original compilation')
            original_output = receiver.run/(row['name']+'.wasm')
            if digest(original_output) != row['binary_sha256']:
                raise RuntimeError('original denial compilation output changed')
            original_row = json.loads(json.dumps(row))
            original_row['actual_original_output'] = str(original_output)
            derived = directory/(row['name']+'.wat')
            derived.write_text(derive(original_path.read_text(), scenario))
            binary = directory/(row['name']+'.wasm')
            command = [receiver.args.wat2wasm, '--enable-all', str(derived), '-o', str(binary)]
            result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=30)
            log = directory/(row['name']+'-build.log');log.write_bytes(result.stdout)
            if result.returncode or digest(original_path) != original_sha:
                raise RuntimeError('post-denial gate compile/input preservation failed')
            # run.py's RpcGeneratedOutputs redirects this logical path into
            # its task-owned output directory. Never write the frozen source.
            logical_binary = Path(row['binary'])
            if not logical_binary.resolve().is_relative_to(receiver.source):
                raise RuntimeError('post-denial gate expects the original logical fixture output before adapter qualification')
            logical_binary.write_bytes(binary.read_bytes())
            record = {'name': row['name'], 'scenario': scenario,
                'original_source': {'path': str(original_path), 'sha256': original_sha},
                'original_compilation': original_row,
                'derived_source': {'path': str(derived), 'sha256': digest(derived)},
                'derived_binary': {'path': str(binary), 'sha256': digest(binary), 'bytes': binary.stat().st_size},
                'command': command, 'returncode': result.returncode, 'log_sha256': digest(log),
                'memory_scratch': {'begin': 10240, 'end_exclusive': 10297, 'memory_bytes': 65536,
                    'original_payload_sentinel_range': [256, 4352], 'original_handle_offset': 0,
                    'original_length_sentinel_offset': 64, 'original_alias_offset': 8192},
                'reverse_derivation_exact': True,
                'scope': 'Every original denial/sentinel assertion retained; stdin gate follows assertions; original Guest20s/native5s budgets'}
            self.compilations.append(record)
            row.update(command=command, source=str(derived), source_sha256=digest(derived),
                binary_sha256=digest(binary), bytes=binary.stat().st_size,
                post_denial_gate=record)

    def install(self):
        if self.installed:raise RuntimeError('post-denial gate already installed')
        owner = self

        def compile(receiver):
            return owner.prepare(receiver)

        def start(receiver, command, name, env=None):
            context = owner.contexts.get(receiver)
            if context is None or name != context['name']+'-uwvm':
                process = owner.original_start(receiver, command, name, env)
                if context is not None and name == context['name']+'-typed-event-witness':
                    context['native'] = process
                    context['row']['native_identity'] = process_identity(process)
                return process
            if context.get('native') is None:
                raise RuntimeError('post-denial gate has no original native witness')
            log = receiver.run/(name+'.log')
            with log.open('wb') as stream:
                process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=stream,
                    stderr=subprocess.STDOUT, env=env or receiver.env, start_new_session=True)
            receiver.processes.append(process)
            context['worker'] = process;context['row']['worker_identity'] = process_identity(process)
            context['row']['worker_log'] = str(log)
            context['thread'] = threading.Thread(target=owner.observe, args=(receiver, context), daemon=True)
            context['thread'].start()
            return process

        def event(receiver, description):
            if description['scenario'] not in DENIALS:
                return owner.original_event(receiver, description)
            if receiver in owner.contexts:raise RuntimeError('nested post-denial gate context')
            row = {'name': description['name'], 'scenario': description['scenario'], 'status': 'WAITING',
                'scope': 'Post-assert Gueststdin gate; original native5s observation completes before Workerexit/Gatewaypidfd shutdown'}
            context = {'name': description['name'], 'row': row, 'cancel': threading.Event()}
            owner.rows.append(row);owner.contexts[receiver] = context
            try:
                result = owner.original_event(receiver, description)
                if row['status'] != 'RELEASED':raise RuntimeError('post-denial gate lacks successful native release')
                return result
            finally:
                context['cancel'].set()
                thread = context.get('thread')
                cleanup_error = None
                if thread is not None:
                    thread.join(timeout=1)
                    row['observer_thread_joined'] = not thread.is_alive()
                    if thread.is_alive():cleanup_error = RuntimeError('post-denial gate observer survived original cleanup')
                worker = context.get('worker')
                if worker is not None and worker.stdin is not None:worker.stdin.close()
                owner.contexts.pop(receiver)
                if cleanup_error is not None:raise cleanup_error

        self.rpc.RpcRunner.compile_typed_events = compile
        self.rpc.RpcRunner.typed_event = event
        self.network.Runner.start = start
        self.installed = True

    def observe(self, receiver, context):
        row, worker, native = context['row'], context['worker'], context['native']
        worker_log = Path(row['worker_log'])
        native_log = receiver.run/(context['name']+'-typed-event-witness.log')
        deadline = time.monotonic()+20  # Guard only; original Workerwait20s remains.
        try:
            while not context['cancel'].is_set():
                now = time.monotonic()
                guest = worker_log.read_bytes() if worker_log.is_file() else b''
                contents = native_log.read_bytes() if native_log.is_file() else b''
                if MARKER in guest and 'guest_assertions_observed_at' not in row:
                    if NATIVE_PASS in contents:
                        raise RuntimeError('native absence observation completed before Guestdenial marker')
                    row['guest_assertions_observed_at'] = now
                if NATIVE_PASS in contents and 'native_pass_observed_at' not in row:
                    row['native_pass_observed_at'] = now
                result = native.poll()
                if result is not None:
                    if (result != 0 or 'guest_assertions_observed_at' not in row or NATIVE_PASS not in contents or
                            row['native_pass_observed_at'] <= row['guest_assertions_observed_at']):
                        raise RuntimeError('original native witness did not pass after Guestdenial assertions')
                    if process_identity(worker) != row['worker_identity'] or worker.poll() is not None:
                        raise RuntimeError('post-denial Worker PID/cgroup changed before release')
                    worker.stdin.write(b'G');worker.stdin.flush()
                    row.update(status='RELEASED', released_at_monotonic=time.monotonic(),
                        native_returncode=result, native_log=str(native_log), native_log_sha256=digest(native_log),
                        guest_marker=MARKER.decode().strip(), release_byte='G')
                    return
                if worker.poll() is not None:raise RuntimeError('post-denial Guest exited before native completion')
                if now >= deadline:raise RuntimeError('post-denial gate bounded outer observation deadline')
                time.sleep(.01)
            row['status'] = 'CANCELLED'
        except BaseException as error:
            row.update(status='FAIL', error=str(error))
            if worker.poll() is None:
                try:
                    if process_identity(worker) != row['worker_identity']:
                        row['cleanup_error'] = 'Worker PID identity changed; release refused'
                        return
                except (OSError, ValueError):
                    return
            try:worker.stdin.write(b'F');worker.stdin.flush()
            except (OSError, ValueError):pass

    def evidence(self):
        if self.contexts:raise RuntimeError('post-denial gate has live contexts')
        return {'rows': self.rows, 'derived_compilations': self.compilations,
            'original_guest_wait_seconds': 20, 'original_native_wait_seconds': 15,
            'original_native_absence_observation_seconds': 5, 'original_native_baseline_deadline_seconds': 8,
            'scope': 'Derived post-assert Gueststdio synchronization; original Wasmdenial predicates/nativebinary/Gateway/Worker unchanged'}

    def restore(self):
        if self.contexts:raise RuntimeError('post-denial gate still has active contexts')
        if self.installed:
            self.rpc.RpcRunner.compile_typed_events = self.original_compile
            self.rpc.RpcRunner.typed_event = self.original_event
            self.network.Runner.start = self.original_start
            self.installed = False
