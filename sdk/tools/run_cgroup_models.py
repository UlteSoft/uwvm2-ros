#!/usr/bin/env python3
"""Prepare or execute SDK native/pure models in the existing pinned Linux cgroup.

Default mode prints recipes and writes nothing. --run uses only explicitly
supplied existing tools, a fresh output directory and build_cgroup_matrix.py's
held-launch/process/memory/cleanup guards. It never downloads packages, grants
ROS authority, starts ROS or claims aarch64 execution. Native test binaries run
on the host architecture; pure AssemblyScript fixtures receive no imports.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import platform
import re
import shutil
import signal
import sys

sys.dont_write_bytecode = True
SDK = Path(__file__).resolve().parents[1]
ROOT = SDK.parent
spec = importlib.util.spec_from_file_location('roswasm_model_guards', SDK / 'tools/build_cgroup_matrix.py')
GUARDS = importlib.util.module_from_spec(spec)
spec.loader.exec_module(GUARDS)
C_FIXTURES = ('wrapper_contract', 'golden_vectors', 'legacy_length_contract',
              'resource_lifetimes', 'scheduler_contract', 'action_bundle_contract', 'events_contract', 'event_dispatch_contract')
CPP_FIXTURES = C_FIXTURES + ('c_header_compatibility', 'poll_reentrancy')
RUST_TESTS = {
    'roswasm': ('capability::tests::output_slot_is_observed_not_assumed',
                'callback::tests::exact_service_id_and_checked_state',
                'callback::tests::reentry_and_close_guard_do_not_poll_or_overwrite',
                'callback::tests::mismatched_reply_and_malformed_body_never_respond',
                'callback::tests::idle_or_failure_does_not_dispatch_and_guard_resets',
                'callback::tests::goal_handler_keeps_identity_and_refuses_nested_poll',
                'callback::tests::shared_owner_lease_blocks_retirement_and_resets',
                'callback::tests::group_cleanup_preflights_all_live_owners_before_retirement',
                'events::tests::all_seven_kinds_preserve_full_width_and_borrow',
                'events::tests::malformed_flags_ticket_and_lengths_are_refused',
                'events::tests::readiness_is_exact_and_full_width',
                'events::binding_tests::binding_commits_only_complete_exact_source_session_sequence',
                'events::binding_tests::true_sequence_exhaustion_does_not_wrap'),
    'wire_vectors': ('shared_positive_vectors', 'shared_negative_vectors'),
    'image_timestamp': ('ros1_image_preserves_all_u32_second_bits', 'transforms_keep_their_bounded_seconds_contract'),
    'local_schedule': ('future_is_one_shot_and_never_runs_inline',
                       'local_cancellation_does_not_execute_or_revoke_native_authority',
                       'timer_budget_exact_time_and_shutdown_are_checked',
                       'group_refuses_reentry_and_preserves_callback_error',
                       'shutdown_cancels_pending_work_and_timer_overflow_never_dispatches'),
}
GO_TESTS = ('TestSharedPositiveVectors', 'TestSharedNegativeVectors', 'TestOutputSlotContract',
            'TestROS1ImagePreservesU32Seconds', 'TestTransformSecondsRemainBounded',
            'TestExecutorRefusesCallbackMutationsAndNestedSpin', 'TestExecutorRejectsNilBeforePoll',
            'TestExecutorResetsGuardAfterCallbackPanic', 'TestExecutorIdleAndErrorAccounting',
            'TestLocalFutureOneShotAndCancellation', 'TestLocalTimerBudgetTimeAndShutdown',
            'TestLocalGroupAndTimerOverflow', 'TestExecutorCheckedClosedRegistration',
            'TestActualServiceDispatchGuardAndIdentity', 'TestActualServiceRefusalNeverResponds',
            'TestActualGoalDispatchRefusesNestedPoll',
            'TestSharedOwnerBusyRefusesCloseRevokeWithoutRetirement',
            'TestGroupedCleanupBusyPreflightPreservesEarlierIdleOwners',
            'TestUnifiedEventKindsAndBorrow', 'TestUnifiedEventMalformedRefused', 'TestEventReadinessFullWidth',
            'TestEventExecutorDispatchMutationAndOwnerLease',
            'TestEventBindingRejectsForeignReplayAndLengthTransactionally',
            'TestEventExecutorIdleCapacityAndDuplicateRegistration')
AS_CHECKS = {'codec_vectors': 175, 'adapter_contracts': 14, 'executor_mutation': 9,
             'factory_transactions': 75, 'resource_roles': 25, 'unicode_strings': 8,
             'highlevel_coverage': None, 'local_schedule': 47, 'server_callbacks': 60, 'unified_events': 33, 'event_dispatch': 20}
TOOLS = ('cc', 'cxx', 'cmake', 'cargo', 'rustc', 'go', 'node', 'asc_js', 'wasm_tools')


class FatalModelGuard(RuntimeError):
    """A failed source/input/process/cleanup proof forbids another command."""


def require(value, message):
    """Reject absent or inconsistent evidence rather than accepting a skipped check."""
    if not value:
        raise ValueError(message)


def file_json(path, sha256=None):
    """Read bounded immutable JSON bytes and optionally check an explicit digest."""
    data, binding = GUARDS.read_artifact(path)
    if sha256 is not None:
        require(binding['sha256'] == sha256, 'JSON input SHA-256 differs from its explicit pin')
    return json.loads(data), binding


def host_machine():
    """Restrict direct model execution to a known host ELF architecture, never QEMU."""
    return {'x86_64': (62, 'amd64'), 'aarch64': (183, 'arm64')}.get(platform.machine())


def same_scope(snapshot, current):
    """Match the live leaf incarnation, boot and explicit swap/cpuset configuration."""
    fields = ('leaf', 'device', 'inode', 'boot_id', 'swap_max', 'cpus_effective',
              'expected_memory_max', 'leaf_memory_max')
    require(all(snapshot.get(key) == current.get(key) for key in fields),
            'recorded cgroup scope differs from the current pinned incarnation')
    require(snapshot.get('probe', {}).get('status') == 'PASS'
            and snapshot['probe'].get('effective_memory_max') == GUARDS.LIMIT
            and snapshot['probe'].get('expected_memory_max') == GUARDS.LIMIT
            and snapshot.get('expected_memory_max') == GUARDS.LIMIT
            and snapshot.get('leaf_memory_max') == GUARDS.LIMIT
            and snapshot.get('swap_max') == '0',
            'recorded cgroup proof is unavailable or has the wrong limit')


def clean_command(step, scope):
    """Admit only recorded successful commands with scope, OOM and cleanup evidence."""
    require(step.get('returncode') == 0 and not step.get('guard_error'), 'recorded command did not pass')
    require(step.get('cleanup', {}).get('status') == 'PASS'
            and not step['cleanup'].get('remaining_live_owned'), 'recorded command cleanup was not proved')
    require(step.get('tool') == step.get('tool_after'), 'recorded command tool mutated')
    for name in ('before', 'after'):
        same_scope(step.get(name, {}), scope)
    delta = step.get('memory_events_delta')
    require(isinstance(delta, dict) and all(isinstance(n, int) and n >= 0 for n in delta.values())
            and not any(delta.get(key, 0) for key in ('oom', 'oom_kill', 'oom_group_kill')),
            'recorded command has missing/reset/OOM event evidence')
    before_events, after_events = step['before'].get('events', {}), step['after'].get('events', {})
    require(delta == {key: after_events.get(key, 0) - before_events.get(key, 0)
                      for key in set(before_events) | set(after_events)}, 'recorded command event deltas are inconsistent')
    held = step.get('held_launch', {})
    require(held.get('state') in ('T', 't') and held.get('cgroup') == scope['probe']['membership'],
            'recorded held launch has no matching membership proof')


def admit_as_report(args, source, scope):
    """Bind a current compiler report and every actual validated pure artifact.

    A PASS label alone is insufficient: source manifests, compiler/script/full
    validator bytes, current cgroup incarnation and artifact file identities are
    independently checked. This establishes consistency with recorded evidence,
    not a cryptographically signed attestation or complete toolchain closure.
    """
    require(args.compiler_report and args.compiler_report_sha256,
            'AssemblyScript execution needs --compiler-report and --compiler-report-sha256')
    for name in ('node', 'asc_js', 'wasm_tools'):
        require(getattr(args, name), 'AssemblyScript evidence needs --' + name.replace('_', '-'))
    report, binding = file_json(args.compiler_report, args.compiler_report_sha256)
    directory = args.compiler_report.parent
    require(report.get('status') == 'COMPILER_MATRIX_PASS' and report.get('task_complete') is False,
            'input is not a successful actual compiler matrix report')
    manifests = {}
    for name in ('source-before.json', 'source-after.json'):
        recorded, manifest_binding = file_json(directory / name)
        require(recorded == source, 'compiler report does not bind the current complete SDK source: ' + name)
        manifests[str(directory / name)] = manifest_binding
    require(report.get('source_before_sha256') == source['sha256']
            and report.get('source_after_sha256') == source['sha256'], 'compiler source digest is stale')
    for name in ('before', 'after'):
        same_scope(report.get(name, {}), scope)
    for key in ('oom', 'oom_kill', 'oom_group_kill'):
        require(scope['events'].get(key, 0) == report['after'].get('events', {}).get(key, 0),
                'OOM counters changed since the compiler report')
    delta = report.get('run_memory_events_delta')
    require(isinstance(delta, dict) and all(isinstance(n, int) and n >= 0 for n in delta.values())
            and not any(delta.get(key, 0) for key in ('oom', 'oom_kill', 'oom_group_kill')),
            'compiler run OOM/counter evidence is missing or failed')
    node = GUARDS.executable(args.node)
    validator = GUARDS.executable(args.wasm_tools)
    require(host_machine() and all(tool.get('machine') == host_machine()[0] and tool.get('elf_class') == 2
                                  for tool in (node, validator)), 'compiler evidence tools are not direct host ELF executables')
    script = {'path': str(args.asc_js), 'sha256': GUARDS.digest(args.asc_js)}
    rows = {}
    for fixture in AS_CHECKS:
        matches = [row for row in report.get('rows', []) if row.get('id') == 'assemblyscript-pure-' + fixture]
        require(len(matches) == 1, 'missing/duplicate actual pure compiler row: ' + fixture)
        row = matches[0]
        source_path = SDK / 'assemblyscript/tests' / (fixture + '.ts')
        require(row.get('language') == 'assemblyscript' and row.get('backend') == 'none'
                and row.get('expects_ros_imports') is False and row.get('fixture') is True
                and row.get('source') == str(source_path)
                and row.get('source_sha256') == GUARDS.digest(source_path), 'wrong pure fixture source/role')
        require(row.get('status') == 'COMPILER_IMPORT_FULL_VALIDATION_PASS', 'pure compiler row did not pass')
        require(row.get('row_source_after_sha256') == source['sha256'], 'pure row source finalization is stale')
        clean_command(row.get('compile', {}), scope)
        clean_command(row.get('full_validation', {}), scope)
        require(row['compile'].get('tool') == node and row['full_validation'].get('tool') == validator,
                'current Node/validator bytes differ from compiler evidence')
        require(row.get('asc_script') == script and row.get('asc_script_after_sha256') == script['sha256'],
                'current AssemblyScript compiler script differs from compiler evidence')
        artifact = Path(row.get('artifact', ''))
        require(artifact.is_absolute() and artifact.resolve().is_relative_to(directory.resolve())
                and not artifact.resolve().is_relative_to(SDK), 'pure artifact must remain in its compiler run')
        compile_argv = row['compile'].get('argv', [])
        require(compile_argv[:3] == [str(args.node), str(args.asc_js), str(source_path)]
                and '--outFile' in compile_argv and compile_argv[compile_argv.index('--outFile') + 1] == str(artifact),
                'compiler argv does not identify the expected source/output')
        require(row['full_validation'].get('argv') == [str(args.wasm_tools), 'validate', str(artifact)],
                'full validator did not receive the expected artifact')
        bound = row.get('artifact_binding')
        require(isinstance(bound, dict) and row.get('artifact_before_full_validation') == bound
                and row.get('artifact_after_full_validation') == bound
                and row.get('artifact_sha256') == bound.get('sha256'), 'artifact validation identity is incomplete')
        data, actual = GUARDS.read_artifact(artifact)
        require(actual == bound and not GUARDS.INSPECTOR.inspect(data), 'pure artifact mutated or has imports')
        require(row.get('import_contract', {}).get('imports') == [], 'recorded pure artifact imports are not empty')
        rows[fixture] = {'artifact': artifact, 'binding': bound, 'compiler_row': row}
    return {'report': str(args.compiler_report), 'binding': binding,
            'source_manifests': manifests, 'asc_script': script, 'node': node, 'validator': validator}, rows


def recipes(args):
    """Describe fixed model inventories without creating files or starting tools."""
    selected = args.groups or ['cmake', 'c', 'cpp', 'rust', 'go', 'assemblyscript']
    installed = args.out / 'prefix-b/share/roswasm/sdk'
    rows = []
    if any(name in selected for name in ('cmake', 'c', 'cpp')):
        rows.append({'id': 'cmake-relocation', 'requires': ['--cmake'],
                     'steps': ['configure LANGUAGES NONE', 'install to prefix-a', 'move prefix-a to prefix-b',
                               'fresh consumer checks both targets and relocated include root',
                               'bind installed SDK file/symlink content before/after native models']})
    for language, names, tool in (('c', C_FIXTURES, args.cc), ('cpp', CPP_FIXTURES, args.cxx)):
        if language not in selected:
            continue
        for name in names:
            rowdir = args.out / (language + '-' + name)
            suffix = '.c' if language == 'c' else '.cpp'
            argv = ([str(tool), '-std=' + ('c11' if language == 'c' else 'c++20'), '-O2', '-UNDEBUG',
                     '-I', str(installed / 'include'), str(installed / 'tests' / language / (name + suffix)),
                     '-o', str(rowdir / 'model')] if tool else [])
            rows.append({'id': rowdir.name, 'language': language, 'compile_argv': argv,
                         'execute_argv': [str(rowdir / 'model')], 'requires': ['--cmake', '--' + ('cc' if language == 'c' else 'cxx')],
                         'source': str(installed / 'tests' / language / (name + suffix))})
    if 'rust' in selected:
        rows.append({'id': 'rust-native-tests', 'requires': ['--cargo', '--rustc'],
                     'steps': ['copy crate and license outside SDK', 'rustc -vV through held launch',
                               'cargo test --offline --no-default-features --features alloc --lib --tests --no-run --message-format=json',
                               'execute exactly four reported host ELF test binaries separately'], 'expected_tests': RUST_TESTS})
    if 'go' in selected:
        rows.append({'id': 'go-native-tests', 'requires': ['--go'],
                     'steps': ['copy common TinyGo package outside SDK', 'go test -c -o model.test',
                               'execute model.test -test.v and check exact names/no skips'], 'expected_tests': GO_TESTS,
                     'scope': 'Native Go common logic only; not TinyGo Wasm execution'})
    if 'assemblyscript' in selected:
        for fixture, count in AS_CHECKS.items():
            rows.append({'id': 'assemblyscript-' + fixture, 'requires': ['--node', '--asc-js', '--wasm-tools',
                         '--compiler-report', '--compiler-report-sha256'], 'fixture': fixture,
                         'expected_checks': count, 'scope': 'Pure import-free Wasm model only' if count else 'Semantic coverage only'})
    return rows


def tree_binding(base):
    """Bind a copied/installed tree without modifying it; relative names permit relocation."""
    records = []
    for path in sorted(base.rglob('*')):
        if path.is_symlink():
            records.append({'path': str(path.relative_to(base)), 'symlink': os.readlink(path)})
        elif path.is_file():
            records.append({'path': str(path.relative_to(base)), 'bytes': path.stat().st_size, 'sha256': GUARDS.digest(path)})
    return records


class Run:
    """Preserve exact command facts, immutable inputs and fatal guard boundaries."""
    def __init__(self, args, source, scope):
        self.args, self.source, self.scope = args, source, scope
        self.external = {str(ROOT / 'tests/ros_cgroup/probe.py'): GUARDS.digest(ROOT / 'tests/ros_cgroup/probe.py')}
        self.pins = {}
        self.artifact_inputs = {}
        self.copies = {}

    def check_inputs(self):
        """Abort before another command after source, controller or pinned tool mutation."""
        try:
            require(GUARDS.manifest() == self.source, 'complete SDK source manifest changed')
            for path, expected in self.external.items():
                require(GUARDS.digest(path) == expected, 'external controller input changed: ' + path)
            for path, expected in self.pins.items():
                require(GUARDS.executable(path) == expected, 'pinned existing tool bytes/identity changed: ' + path)
            for path, expected in self.artifact_inputs.items():
                GUARDS.verify_artifact(path, expected)
            for path, (expected, allowed) in self.copies.items():
                current = {item['path']: item for item in tree_binding(Path(path))}
                require(all(current.get(name) == item for name, item in expected.items())
                        and set(current) - set(expected) <= set(allowed), 'copied model source changed: ' + path)
            current_scope = GUARDS.cgroup_snapshot(self.args)
            same_scope(current_scope, self.scope)
            delta = {key: current_scope['events'].get(key, 0) - self.scope['events'].get(key, 0)
                     for key in set(current_scope['events']) | set(self.scope['events'])}
            require(all(n >= 0 for n in delta.values())
                    and not any(delta.get(key, 0) for key in ('oom', 'oom_kill', 'oom_group_kill')),
                    'whole-run counter/OOM guard failed before another command')
        except Exception as error:
            raise FatalModelGuard('input/scope guard failed: ' + str(error)) from error

    def copy_input(self, row, base, allowed=()):
        """Bind actual copied compiler inputs; only named lock-file additions are allowed."""
        binding = tree_binding(base)
        self.copies[str(base)] = ({item['path']: item for item in binding}, allowed)
        row.setdefault('copied_inputs', []).append({'path': str(base), 'before': binding,
                                                   'allowed_generated_files': list(allowed)})

    def pin(self, path):
        """Bind an existing explicit ELF tool before use, with no shell fallback."""
        require(path, 'required existing tool is not configured')
        bound = GUARDS.executable(path)
        require(host_machine() and bound.get('machine') == host_machine()[0] and bound.get('elf_class') == 2,
                'model tool is not a direct 64-bit host ELF executable')
        key = str(path)
        if key in self.pins and self.pins[key] != bound:
            raise FatalModelGuard('pinned executable changed before reuse')
        self.pins[key] = bound
        return bound

    def command(self, row, label, argv, cwd, env):
        """Use the shared held-exec guard and retain returned or fatal command capsules."""
        self.check_inputs()
        self.pin(Path(argv[0]))
        try:
            step = GUARDS.guarded_command(argv, cwd, env, Path(row['rowdir']), label, self.args)
            row.setdefault('commands', []).append(dict(step, label=label,
                environment_sha256=hashlib.sha256(json.dumps(sorted(env.items())).encode()).hexdigest()))
        except GUARDS.CommandGuardFailure as error:
            row.setdefault('commands', []).append(dict(error.capsule, label=label))
            raise FatalModelGuard(str(error)) from error
        if step.get('guard_error') or step.get('cleanup', {}).get('status') != 'PASS':
            raise FatalModelGuard('command process/memory/cleanup guard failed: ' + label)
        self.check_inputs()
        require(step['returncode'] == 0, 'command exited unsuccessfully: ' + label)
        with Path(step['log']).open('rb') as stream:
            output = stream.read(16 * 1024 * 1024 + 1)
        require(len(output) <= 16 * 1024 * 1024, 'command log exceeds the bounded parser budget')
        return output.decode(errors='replace')

    def execute(self, row, label, artifact, arguments, cwd, env):
        """Bind a produced host ELF around actual execution; do not infer ARM behavior."""
        data, bound = GUARDS.read_artifact(artifact)
        del data
        self.artifact_inputs[str(artifact)] = bound
        tool = self.pin(artifact)
        require(host_machine() and tool['machine'] == host_machine()[0], 'model ELF is not the direct host architecture')
        row.setdefault('artifacts', []).append({'path': str(artifact), 'binding': bound, 'ELF': tool})
        output = self.command(row, label, [str(artifact)] + arguments, cwd, env)
        try:
            GUARDS.verify_artifact(artifact, bound)
        except (OSError, ValueError, RuntimeError) as error:
            raise FatalModelGuard('executed model artifact changed: ' + str(error)) from error
        return output


def environment(out):
    """Disable automatic downloads/wrappers and isolate mutable compiler caches.

    Remaining inherited inputs are digest-recorded and unqualified. This is not
    a full system-header, standard-library or global-configuration closure.
    """
    env = os.environ.copy()
    env.pop('CARGO_ENCODED_RUSTFLAGS', None)
    env.update(PYTHONDONTWRITEBYTECODE='1', CARGO_NET_OFFLINE='true', RUSTUP_AUTO_INSTALL='0',
               RUSTC_WRAPPER='', RUSTC_WORKSPACE_WRAPPER='', CARGO_BUILD_RUSTC_WRAPPER='',
               CARGO_BUILD_RUSTC_WORKSPACE_WRAPPER='', RUSTFLAGS='', CARGO_BUILD_RUSTFLAGS='',
               GOPROXY='off', GOSUMDB='off', GOTOOLCHAIN='local', GOENV='off', GOFLAGS='',
               CGO_ENABLED='0', GOCACHE=str(out / 'go-cache'), GOMODCACHE=str(out / 'go-mod-cache'),
               XDG_CACHE_HOME=str(out / 'cache'))
    return env


def relocate(run, row, env):
    """Actually install A, move to B, and configure an independent relocated consumer."""
    args = run.args
    require(args.cmake, 'missing --cmake; installed C/C++ model inputs are unavailable')
    a, b = args.out / 'prefix-a', args.out / 'prefix-b'
    build, consumer = args.out / 'cmake-sdk-build', args.out / 'cmake-consumer'
    run.command(row, 'configure-sdk', [str(args.cmake), '-S', str(SDK), '-B', str(build),
                '-DCMAKE_INSTALL_PREFIX=' + str(a), '-DCMAKE_INSTALL_DATADIR=share',
                '-DCMAKE_INSTALL_LIBDIR=lib'], ROOT, env)
    run.command(row, 'install-sdk', [str(args.cmake), '--install', str(build)], ROOT, env)
    require(a.is_dir() and not b.exists(), 'expected fresh install A/B paths are unavailable')
    before = tree_binding(a)
    a.rename(b)
    require(tree_binding(b) == before, 'installed tree changed during relocation')
    consumer.mkdir()
    cmake_text = '''# Generated relocation consumer; no host compiler or ROS service is selected.
cmake_minimum_required(VERSION 3.16)
project(roswasm_relocation_check LANGUAGES NONE)
find_package(roswasm CONFIG REQUIRED NO_DEFAULT_PATH PATHS "${ROSWASM_TEST_PREFIX}/lib/cmake/roswasm")
foreach(name IN ITEMS roswasm::c roswasm::cpp roswasm::unitree)
  if(NOT TARGET ${name})
    message(FATAL_ERROR "Missing imported target ${name}")
  endif()
endforeach()
get_target_property(actual_include roswasm::c INTERFACE_INCLUDE_DIRECTORIES)
set(expected_include "${ROSWASM_TEST_PREFIX}/share/roswasm/sdk/include")
if(NOT actual_include STREQUAL expected_include)
  message(FATAL_ERROR "Include root did not relocate: ${actual_include}")
endif()
if(NOT ROSWASM_SDK_ROOT STREQUAL "${ROSWASM_TEST_PREFIX}/share/roswasm/sdk")
  message(FATAL_ERROR "SDK package root did not relocate")
endif()
get_target_property(unitree_include roswasm::unitree INTERFACE_INCLUDE_DIRECTORIES)
set(expected_unitree "${ROSWASM_TEST_PREFIX}/share/roswasm/sdk/robots/unitree/include")
if(NOT unitree_include STREQUAL expected_unitree)
  message(FATAL_ERROR "Unitree include root did not relocate: ${unitree_include}")
endif()
foreach(header IN ITEMS crc.h crc.hpp topic_rpc.h topic_rpc.hpp simulation_control.h simulation_hold.h)
  if(NOT EXISTS "${expected_unitree}/roswasm_unitree/${header}")
    message(FATAL_ERROR "Missing installed Unitree header ${header}")
  endif()
endforeach()
get_target_property(features roswasm::cpp INTERFACE_COMPILE_FEATURES)
if(NOT "cxx_std_20" IN_LIST features)
  message(FATAL_ERROR "Missing C++20 interface requirement")
endif()
foreach(header IN ITEMS roswasm/ros1.h roswasm/ros2.h roswasm/roswasmcpp.hpp roswasm/node.h)
  if(NOT EXISTS "${expected_include}/${header}")
    message(FATAL_ERROR "Missing installed header ${header}")
  endif()
endforeach()
add_library(consumer INTERFACE)
target_link_libraries(consumer INTERFACE roswasm::cpp)
'''
    (consumer / 'CMakeLists.txt').write_text(cmake_text)
    run.command(row, 'configure-consumer', [str(args.cmake), '-S', str(consumer), '-B', str(args.out / 'cmake-consumer-build'),
                '-DROSWASM_TEST_PREFIX=' + str(b)], ROOT, env)
    require(tree_binding(b) == before, 'installed SDK changed during consumer configuration')
    row.update(status='CMAKE_RELOCATION_PASS', installed_prefix=str(b), installed_tree=before)
    return b / 'share/roswasm/sdk', before


def native_c(run, row, installed, installed_binding, env):
    """Build/assert-run one fixture from the relocated tree, retaining its layout."""
    require(installed, 'CMake relocation did not pass; no installed fixture may be substituted')
    prefix = run.args.out / 'prefix-b'
    if tree_binding(prefix) != installed_binding:
        raise FatalModelGuard('installed model source tree changed')
    run.command(row, 'compile', row['compile_argv'], ROOT, env)
    artifact = Path(row['execute_argv'][0])
    run.execute(row, 'execute', artifact, [], ROOT, env)
    if tree_binding(prefix) != installed_binding:
        raise FatalModelGuard('installed model source tree changed during native execution')
    row.update(status='NATIVE_ASSERT_MODEL_PASS', assertions='C/C++ assert enabled by explicit -UNDEBUG',
               source_sha256=GUARDS.digest(row['source']))


def rust_models(run, row, env):
    """Compile no-backend alloc tests once, then execute exact Cargo-reported binaries."""
    args = run.args
    require(args.cargo and args.rustc, 'native Rust models require --cargo and --rustc')
    run.pin(args.rustc)
    work = args.out / 'work/rust/sdk'
    shutil.copytree(SDK / 'rust', work / 'rust')
    shutil.copyfile(SDK / 'LICENSE', work / 'LICENSE')
    run.copy_input(row, work, ('rust/Cargo.lock',))
    local = env.copy()
    local['RUSTC'] = str(args.rustc)
    version = run.command(row, 'rustc-version', [str(args.rustc), '-vV'], ROOT, local)
    host = re.findall(r'^host: ([A-Za-z0-9_.-]+)$', version, re.MULTILINE)
    require(len(host) == 1, 'rustc did not expose one concrete host triple')
    target = args.out / 'cargo-native-target'
    output = run.command(row, 'compile-tests', [str(args.cargo), 'test', '--offline', '--manifest-path', str(work / 'rust/Cargo.toml'),
             '--no-default-features', '--features', 'alloc', '--lib', '--tests', '--no-run', '--message-format=json',
             '--target', host[0], '--target-dir', str(target)], ROOT, local)
    binaries = {}
    for line in output.splitlines():
        if not line.startswith('{'):
            continue
        value = json.loads(line)
        if value.get('reason') == 'compiler-artifact' and value.get('profile', {}).get('test') and value.get('executable'):
            name = value.get('target', {}).get('name')
            require(name in RUST_TESTS and name not in binaries, 'unexpected/duplicate Cargo test executable')
            artifact = Path(value['executable'])
            require(artifact.is_absolute() and artifact.resolve().is_relative_to(target.resolve()), 'Cargo test output escaped fresh target tree')
            binaries[name] = artifact
    require(set(binaries) == set(RUST_TESTS), 'Cargo did not produce exactly the four required test executables')
    executed = {}
    for name, artifact in binaries.items():
        text = run.execute(row, 'execute-' + name, artifact, ['--test-threads=1', '--nocapture'], ROOT, local)
        passed = re.findall(r'^test ([^\n]+) \.\.\. ok$', text, re.MULTILINE)
        require(sorted(passed) == sorted(RUST_TESTS[name]) and len(passed) == len(set(passed)), 'Rust executed test inventory differs: ' + name)
        summary = re.findall(r'test result: ok\. (\d+) passed; (\d+) failed; (\d+) ignored; (\d+) measured; (\d+) filtered out;', text)
        require(summary == [(str(len(passed)), '0', '0', '0', '0')], 'Rust tests were skipped/filtered or summary is missing')
        executed[name] = passed
    row.update(status='NATIVE_RUST_MODELS_PASS', host_triple=host[0], executed_tests=executed,
               completed_tests=sum(len(names) for names in RUST_TESTS.values()),
               backend_features=[], alloc_feature=True)


def go_subtest_inventory(source):
    """Bind the closed, literal positive-vector subtests to their frozen source.

    This fixture declares each subtest directly. Refuse a changed declaration
    form instead of silently treating an unparsed subtest as optional coverage.
    No Go code is executed to construct this expected inventory.
    """
    start = 'func TestSharedPositiveVectors(t *testing.T) {'
    end = 'func TestSharedNegativeVectors(t *testing.T) {'
    require(source.count(start) == source.count(end) == 1,
            'Go positive-vector source boundaries are ambiguous')
    body = source.split(start, 1)[1].split(end, 1)[0]
    names = re.findall(r'^\tt\.Run\("([A-Za-z0-9_.-]+)", func\(t \*testing\.T\) \{$', body, re.MULTILINE)
    require(len(names) == 57 and len(set(names)) == len(names)
            and body.count('t.Run(') == len(names),
            'Go positive-vector literal subtest inventory differs')
    return tuple('TestSharedPositiveVectors/' + name for name in names)


def validate_go_inventory(text, subtests):
    """Check all nine top-level tests and every declared nested vector result.

    Go prints nested RUN lines without indentation and nested PASS lines with
    indentation. Those results are separate from the top-level inventory; all
    of them must occur once, pass, and precede the one final PASS marker.
    """
    expected = GO_TESTS + tuple(subtests)
    require(len(expected) == len(set(expected)), 'duplicate expected Go test')
    runs, outcomes, complete = [], [], False
    run_pattern = re.compile(r'=== RUN   (\S+)')
    pass_pattern = re.compile(r'([ \t]*)--- PASS: (\S+) \((\d+(?:\.\d+)?)s\)')
    for line in text.splitlines():
        if line == 'PASS':
            require(not complete and sorted(outcomes) == sorted(expected),
                    'Go final PASS is duplicated or precedes completed tests')
            complete = True
        elif line.lstrip().startswith('=== RUN'):
            match = run_pattern.fullmatch(line)
            require(match is not None and not complete, 'malformed or late Go RUN result')
            name = match[1]
            require(name in expected and name not in runs, 'unexpected or duplicate Go RUN result')
            if '/' in name:
                parent = name.split('/', 1)[0]
                require(parent in runs and parent not in outcomes, 'Go subtest RUN is outside its parent')
            runs.append(name)
        elif line.lstrip().startswith('---'):
            match = pass_pattern.fullmatch(line)
            require(match is not None and not complete, 'Go test failed, skipped or has malformed outcome')
            indentation, name = match[1], match[2]
            require(name in runs and name not in outcomes
                    and bool(indentation) == ('/' in name),
                    'Go PASS lacks its unique RUN or has the wrong nesting')
            if '/' not in name:
                # Go reports the aggregate parent PASS before the indented
                # child PASS lines, although each child RUN occurred earlier.
                require(all(child in runs for child in subtests if child.startswith(name + '/')),
                        'Go parent PASS precedes its nested RUN inventory')
            else:
                require(name.split('/', 1)[0] in outcomes,
                        'Go nested PASS precedes its aggregate parent result')
            outcomes.append(name)
        elif line.startswith(('FAIL', 'panic:', 'fatal error:', '=== ')):
            require(False, 'Go execution contains a failure or unsupported test transition')
    require(complete and sorted(runs) == sorted(expected)
            and sorted(outcomes) == sorted(expected),
            'Go top-level or nested test inventory passed incompletely')
    return {'tests': [name for name in outcomes if '/' not in name],
            'subtests': [name for name in outcomes if '/' in name]}


def go_models(run, row, env):
    """Execute nine native Go common-logic tests; do not relabel them TinyGo Wasm."""
    require(run.args.go and host_machine(), 'native Go models require --go and a supported host architecture')
    work = run.args.out / 'work/go/sdk/tinygo'
    shutil.copytree(SDK / 'tinygo', work)
    run.copy_input(row, work, ('go.sum',))
    local = dict(env, GOOS='linux', GOARCH=host_machine()[1])
    artifact = Path(row['rowdir']) / 'model.test'
    run.command(row, 'compile-tests', [str(run.args.go), 'test', '-c', '-o', str(artifact), '.'], work, local)
    text = run.execute(row, 'execute-tests', artifact, ['-test.v', '-test.run=^(' + '|'.join(GO_TESTS) + ')$'], work, local)
    inventory = validate_go_inventory(text, go_subtest_inventory((work / 'wire_vectors_test.go').read_text()))
    row.update(status='NATIVE_GO_COMMON_MODELS_PASS', executed_tests=inventory['tests'], completed_tests=len(GO_TESTS),
               executed_subtests=inventory['subtests'], completed_subtests=len(inventory['subtests']),
               tinygo_Wasm_execution='NOT_PERFORMED')


def as_model(run, row, evidence, inputs, env):
    """Execute exactly one admitted zero-import artifact through the root-owned runner."""
    require(evidence and inputs, 'current compiler report admission is unavailable')
    GUARDS.verify_artifact(run.args.compiler_report, evidence['binding'])
    fixture = row['fixture']
    input_row = inputs[fixture]
    artifact, bound = input_row['artifact'], input_row['binding']
    GUARDS.verify_artifact(artifact, bound)
    args = run.args
    argv = [str(args.node), str(SDK / 'javascript/tests/run_pure_wasm.mjs'), '--wasm', str(artifact),
            '--sha256', bound['sha256'], '--fixture', fixture, '--expected-leaf', str(args.expected_leaf),
            '--expected-device', str(args.expected_device), '--expected-inode', str(args.expected_inode),
            '--expected-boot', run.scope['boot_id'], '--expected-memory-max', str(args.expected_memory_max),
            '--expected-swap-max', args.expected_swap_max,
            '--expected-cpus', args.expected_cpus]
    text = run.command(row, 'execute-pure-wasm', argv, ROOT, env)
    values = [json.loads(line) for line in text.splitlines() if line.startswith('{')]
    require(len(values) == 1, 'pure runner did not emit one result')
    result = values[0]
    expected = AS_CHECKS[fixture]
    require(result.get('status') == ('SEMANTIC_MODEL_EXECUTED' if expected is None else 'PURE_MODEL_ASSERTIONS_PASS')
            and result.get('fixture') == fixture and result.get('sha256') == bound['sha256']
            and result.get('imports') == [] and result.get('expected_checks') == expected
            and result.get('completed_checks') == expected, 'pure runner result differs from the admitted fixture')
    try:
        GUARDS.verify_artifact(artifact, bound)
        GUARDS.verify_artifact(args.compiler_report, evidence['binding'])
    except (OSError, ValueError, RuntimeError) as error:
        raise FatalModelGuard('admitted AS input changed during execution: ' + str(error)) from error
    row.update(status=result['status'], result=result, compiler_evidence=evidence,
               artifact_binding=bound, compiler_row_id=input_row['compiler_row']['id'])


def save_row(row):
    """Persist facts; a write failure is fatal and receives a best-effort fallback."""
    try:
        GUARDS.write_json(Path(row['rowdir']) / 'report.json', row)
    except Exception as error:
        row.update(status='FAIL_FATAL_EVIDENCE_PERSISTENCE', persistence_error=str(error))
        try:
            GUARDS.write_json(Path(row['rowdir']) / 'fatal-evidence.json', row)
        except Exception as fallback:
            row['fallback_persistence_error'] = str(fallback)
            print(json.dumps(row), file=sys.stderr, flush=True)
        raise FatalModelGuard('model evidence persistence failed') from error


def main():
    """Default to prepare-only; real execution requires current Linux/cgroup/tool pins."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run', action='store_true')
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--groups', nargs='+', choices=('cmake', 'c', 'cpp', 'rust', 'go', 'assemblyscript'))
    for name in TOOLS:
        parser.add_argument('--' + name.replace('_', '-'), type=Path)
    parser.add_argument('--compiler-report', type=Path)
    parser.add_argument('--compiler-report-sha256')
    parser.add_argument('--expected-leaf', type=Path)
    parser.add_argument('--expected-device', type=int)
    parser.add_argument('--expected-inode', type=int)
    parser.add_argument('--expected-boot')
    parser.add_argument('--expected-memory-max', type=int, default=GUARDS.LIMIT, choices=(GUARDS.LIMIT,))
    parser.add_argument('--expected-swap-max', default='0', choices=('0',))
    parser.add_argument('--expected-cpus', default='0-31', choices=('0-31',))
    parser.add_argument('--rss-limit-mib', type=int, default=2048)
    parser.add_argument('--cgroup-current-limit-mib', type=int, default=GUARDS.DEFAULT_CURRENT_LIMIT_MIB)
    parser.add_argument('--timeout-seconds', type=int, default=180)
    args = parser.parse_args()
    for name in TOOLS + ('out', 'compiler_report', 'expected_leaf'):
        value = getattr(args, name)
        if value is not None and not value.is_absolute():
            parser.error('--' + name.replace('_', '-') + ' must be absolute')
    args.out = args.out.resolve()
    if args.out.is_relative_to(SDK):
        parser.error('output and caches must remain outside immutable SDK inputs')
    if args.compiler_report_sha256 and not re.fullmatch('[a-f0-9]{64}', args.compiler_report_sha256):
        parser.error('--compiler-report-sha256 must be a lower-case SHA-256 digest')
    args.groups = list(dict.fromkeys(args.groups)) if args.groups else None
    plan = recipes(args)
    if not args.run:
        print(json.dumps({'status': 'PREPARED_PLAN_ONLY', 'scope': 'No compiler/Guest/ROS execution or output writes',
                          'expected_memory_max': args.expected_memory_max, 'expected_swap_max': args.expected_swap_max,
                          'rss_limit_mib': args.rss_limit_mib, 'cgroup_current_limit_mib': args.cgroup_current_limit_mib,
                          'rows': plan}, indent=2))
        return 0
    if platform.system() != 'Linux' or any(value is None for value in
            (args.expected_leaf, args.expected_device, args.expected_inode, args.expected_boot)):
        parser.error('--run needs Linux and current --expected-leaf/--expected-device/--expected-inode/--expected-boot')
    if not 0 < args.rss_limit_mib <= args.cgroup_current_limit_mib < GUARDS.LIMIT_MIB or args.timeout_seconds <= 0:
        parser.error('require 0 < RSS guard <= current-memory guard < 65536 MiB and a positive deadline')
    if not hasattr(os, 'pidfd_open') or not hasattr(signal, 'pidfd_send_signal'):
        parser.error('actual execution needs Linux pidfd ownership-safe cleanup')
    before = GUARDS.cgroup_snapshot(args)
    require(before['boot_id'] == args.expected_boot, 'live boot differs from the explicit pin')
    source = GUARDS.manifest()
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.mkdir(mode=0o700, exist_ok=False)
    GUARDS.write_json(args.out / 'source-before.json', source)
    run = Run(args, source, before)
    env = environment(args.out)
    results = []
    installed, installed_binding = None, None
    evidence, as_inputs, as_refusal = None, None, None
    if any(row.get('fixture') in AS_CHECKS for row in plan):
        try:
            evidence, as_inputs = admit_as_report(args, source, before)
            run.pins[str(args.node)] = evidence['node']
            run.pins[str(args.wasm_tools)] = evidence['validator']
            run.external[str(args.asc_js)] = evidence['asc_script']['sha256']
            run.artifact_inputs[str(args.compiler_report)] = evidence['binding']
            run.artifact_inputs.update(evidence['source_manifests'])
            for item in as_inputs.values():
                run.artifact_inputs[str(item['artifact'])] = item['binding']
        except (OSError, ValueError, RuntimeError, IndexError, KeyError) as error:
            as_refusal = str(error)
    fatal = None
    for prepared in plan:
        row = dict(prepared, rowdir=str(args.out / prepared['id']), status='BLOCKED',
                   ROS_execution='NOT_PERFORMED', ARM_execution='NOT_PERFORMED',
                   environment_sha256=hashlib.sha256(json.dumps(sorted(env.items())).encode()).hexdigest())
        Path(row['rowdir']).mkdir()
        print(json.dumps({'progress': 'model-start', 'id': row['id']}), flush=True)
        try:
            if row['id'] == 'cmake-relocation':
                installed, installed_binding = relocate(run, row, env)
            elif row.get('language') in ('c', 'cpp'):
                require(row['compile_argv'], 'native compiler configuration is missing')
                native_c(run, row, installed, installed_binding, env)
            elif row['id'] == 'rust-native-tests':
                rust_models(run, row, env)
            elif row['id'] == 'go-native-tests':
                go_models(run, row, env)
            else:
                require(not as_refusal, 'AssemblyScript compiler evidence refused: ' + str(as_refusal))
                as_model(run, row, evidence, as_inputs, env)
            run.check_inputs()
        except FatalModelGuard as error:
            row.update(status='FAIL_FATAL_GUARD', reason=str(error))
            fatal = str(error)
        except Exception as error:
            row.update(status='FAIL_MODEL' if row.get('commands') else 'BLOCKED',
                       exception_type=type(error).__name__, reason=str(error))
        try:
            for copied in row.get('copied_inputs', []):
                copied['after'] = tree_binding(Path(copied['path']))
            run.check_inputs()
            save_row(row)
        except Exception as error:
            row.update(status='FAIL_FATAL_GUARD', finalization_guard_error=str(error))
            try:
                save_row(row)
            except FatalModelGuard:
                pass
            fatal = str(error)
        results.append(row)
        print(json.dumps({'progress': 'model-finish', 'id': row['id'], 'status': row['status']}), flush=True)
        if fatal:
            break
    report = {'status': 'MODEL_MATRIX_INCOMPLETE', 'task_complete': False,
              'expected_memory_max': args.expected_memory_max, 'expected_swap_max': args.expected_swap_max,
              'rss_limit_mib': args.rss_limit_mib, 'cgroup_current_limit_mib': args.cgroup_current_limit_mib,
              'scope': 'Host-native SDK models, relocated source package and import-free AS models only',
              'ROS_execution': 'NOT_PERFORMED', 'ARM_execution': 'NOT_PERFORMED',
              'toolchain_sdk_closure': 'UNQUALIFIED', 'source_before_sha256': source['sha256'],
              'before': before, 'rows': results, 'requested_rows': len(plan), 'fatal_guard': fatal,
              'external_inputs': run.external, 'pinned_tools': run.pins,
              'artifact_input_bindings': run.artifact_inputs,
              'sampling_limit': 'Shared 20 ms observed process guard; no exhaustive child or ARM instruction tracing'}
    try:
        run.check_inputs()
        after = GUARDS.cgroup_snapshot(args)
        source_after = GUARDS.manifest()
        GUARDS.write_json(args.out / 'source-after.json', source_after)
        delta = {key: after['events'].get(key, 0) - before['events'].get(key, 0)
                 for key in set(before['events']) | set(after['events'])}
        report.update(after=after, source_after_sha256=source_after['sha256'], run_memory_events_delta=delta)
        require(before['boot_id'] == after['boot_id'] and all(n >= 0 for n in delta.values())
                and not any(delta.get(key, 0) for key in ('oom', 'oom_kill', 'oom_group_kill')), 'whole-run OOM/incarnation/counter guard failed')
        accepted = {'CMAKE_RELOCATION_PASS', 'NATIVE_ASSERT_MODEL_PASS', 'NATIVE_RUST_MODELS_PASS',
                    'NATIVE_GO_COMMON_MODELS_PASS', 'PURE_MODEL_ASSERTIONS_PASS', 'SEMANTIC_MODEL_EXECUTED'}
        if not fatal and len(results) == len(plan) and results and all(row['status'] in accepted for row in results):
            report['status'] = 'SDK_MODEL_MATRIX_PASS'
    except Exception as error:
        report.update(status='FAIL_FINAL_GUARD', final_guard_error=str(error))
    try:
        GUARDS.write_json(args.out / 'report.json', report)
    except Exception as error:
        report.update(status='FAIL_REPORT_PERSISTENCE', persistence_error=str(error))
        try:
            GUARDS.write_json(args.out / 'fatal-report.json', report)
        except Exception as fallback:
            report['fallback_persistence_error'] = str(fallback)
            print(json.dumps(report), file=sys.stderr, flush=True)
    return 0 if report['status'] == 'SDK_MODEL_MATRIX_PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
