#!/usr/bin/env python3
"""Install/verify/run actual Intel VTune in the admitted serial Linux lane.

This program changes no sysctl, driver, cgroup, service, container, or global
package state. Installation uses an owned copy and explicit unprivileged UID,
so the installer cannot load a kernel sampling driver as a side effect. Actual
profiling uses the caller's existing supported Perf/PMU capabilities. Hardware
Hotspots and microarchitecture are separate serial collections, with actual
raw counters and reports retained. QEMU targets are refused for this native
microarchitecture comparison; their host counters would measure translation.
"""
import csv
import io
import os
from pathlib import Path
import re
import shutil
import stat
import sys
import time

from common import Actors, Frame, arguments, exact, require
from benchmark import image


def run(owner, frame, label, argv, data=b"", environment=None, credentials=None):
    child, row = owner.start(label, argv, environment, data=data, json_output=False, credentials=credentials)
    owner.finish(child)
    return row


def pmu_metadata():
    """Read actual host facts; absence is evidence, never an invented PMU."""
    values = {}
    for name in ("perf_event_paranoid", "perf_event_mlock_kb", "perf_event_max_sample_rate", "nmi_watchdog"):
        path = Path("/proc/sys/kernel") / name
        values[name] = path.read_text().strip() if path.exists() else None
    cpu = Path("/proc/cpuinfo").read_text()
    values["cpu_description"] = [line for line in cpu.splitlines() if line.startswith(
        ("vendor_id", "cpu family", "model\t", "model name", "stepping", "microcode", "flags"))][:12]
    values["effective_capabilities"] = [line for line in Path("/proc/self/status").read_text().splitlines()
                                       if line.startswith(("CapEff:", "CapBnd:", "CapAmb:", "Seccomp:"))]
    values["affinity_cpus"] = sorted(os.sched_getaffinity(0))
    values["event_sources"] = sorted(path.name for path in Path("/sys/bus/event_source/devices").iterdir())
    return values


def hardware_counts(path):
    """Require positive actual raw core-event columns in VTune's CSV report.

    Metric names come from the installed collector/physical PMU. We accept
    recognizable cycles/instructions event families rather than hardcoding
    model-specific event numbers. Unsupported/absent columns fail closed and
    retain raw CSV for review; software CPU-time samples cannot satisfy this.
    """
    require(path.stat().st_size <= 16 * 1024**2, "hardware-event CSV exceeds bounded parsing budget")
    with path.open(encoding="utf-8-sig", newline="") as stream:
        text = stream.read(16 * 1024**2 + 1)
    require(len(text) <= 16 * 1024**2, "hardware-event CSV grew beyond parsing budget")
    rows = list(csv.reader(io.StringIO(text)))
    require(1 < len(rows) <= 100000, "bounded nonempty actual hardware-event CSV required")
    evidence = []
    for index, header in enumerate(rows):
        columns = [position for position, name in enumerate(header) if any(event in name.upper()
            for event in ("CPU_CLK_UNHALTED", "INST_RETIRED", "CPU_CYCLES", "CPU-CYCLES", "INSTRUCTIONS_RETIRED"))]
        if not columns:
            continue
        for row in rows[index + 1:]:
            for position in columns:
                if position >= len(row):
                    continue
                value = row[position].strip().replace(",", "")
                if re.fullmatch(r"[0-9]+(?:\.[0-9]+)?", value) and float(value) > 0:
                    evidence.append({"event_column": header[position], "actual_count": value})
                    if len(evidence) >= 32:
                        return evidence
    require(evidence, "actual positive raw cycles/instructions hardware counts unavailable")
    return evidence


def install(owner, frame, spec):
    selected = spec["installation"]
    exact(selected, ("shell_input", "installer_input", "official_input_receipt", "uid", "gid"), "owned installation")
    proof, _ = frame.common.document(frame.input(selected["official_input_receipt"]))
    require(proof.get("status") == "VERIFIED_OFFICIAL_INSTALLER_INPUT" and
            proof.get("vendor") == "Intel" and proof.get("artifact") == frame.inputs[selected["installer_input"]] and
            proof.get("download_url", "").startswith("https://registrationcenter-download.intel.com/") and
            proof.get("documentation_url", "").startswith("https://www.intel.com/"),
            "actual official installer acquisition receipt required")
    credentials = {"uid": selected["uid"], "gid": selected["gid"]}
    require(all(type(value) is int and value >= 1000 for value in credentials.values()), "installation must not run as root")
    root = frame.out / "owned-installer"
    root.mkdir(mode=0o700)
    os.chown(root, credentials["uid"], credentials["gid"])
    # Traverse only the fresh output; contents remain owner-only. No input
    # ownership/permissions or system account/group definitions are changed.
    frame.out.chmod(0o711)
    for name in ("home", "tmp", "cache", "logs", "extract"):
        path = root / name
        path.mkdir(mode=0o700)
        os.chown(path, credentials["uid"], credentials["gid"])
    original = frame.input(selected["installer_input"])
    copy = root / "installer.sh"
    with original.open("rb") as source, copy.open("xb") as target:
        shutil.copyfileobj(source, target, 1024**2)
    copy.chmod(0o500)
    os.chown(copy, credentials["uid"], credentials["gid"])
    actual = frame.pin(copy)
    require(actual["sha256"] == frame.inputs[selected["installer_input"]]["sha256"], "owned installer copy differs")
    environment = {**frame.environment, "HOME": str(root / "home"), "TMPDIR": str(root / "tmp"),
                   "XDG_CACHE_HOME": str(root / "cache"), "XDG_CONFIG_HOME": str(root / "home")}
    shell = str(frame.input(selected["shell_input"]))
    run(owner, frame, "installer-help", [shell, str(copy), "--help"], environment=environment, credentials=credentials)
    run(owner, frame, "installer-options", [shell, str(copy), "-a", "--help"], environment=environment, credentials=credentials)
    argv = [shell, str(copy), "-f", str(root / "extract"), "-r", "no", "-a", "--silent", "--eula", "accept",
        "--install-dir", str(root / "installation"), "--log-dir", str(root / "logs"),
        "--download-cache", str(root / "cache"), "--intel-sw-improvement-program-consent", "decline"]
    run(owner, frame, "actual-owned-user-install", argv, environment=environment, credentials=credentials)
    candidates = [path for path in (root / "installation").rglob("vtune") if path.is_file() and path.parent.name == "bin64"]
    require(candidates, "actual VTune executable absent after installer completion")
    frame.report["installed_executables"] = [frame.pin(path) for path in candidates]
    frame.report["scope"] = "Actual owned unprivileged installation only; no self-check or PMU qualification"
    return "PASS_VTUNE_USER_INSTALL_ONLY"


def direct3_collect(frame, spec):
    """Replay one fully measured Wasm case under the explicit host profile policy.

    The CP310 owner has already created a NEW private mount/loopback namespace.
    Its baseline report/config/selection remain immutable original inputs. Only
    the actual namespace and owned mutable output paths are renewed. Hardware
    collection attaches after actual Worker/Gateway maps and endpoint readiness,
    before bootstrap, the first real callback publication and physics step1.
    """
    import copy
    import types

    recipe = spec['direct3']
    require(type(recipe) is dict, 'closed Direct3 profile recipe object required')
    exact(recipe, ('language', 'selection_input', 'baseline_config_input', 'baseline_selection_input',
                  *(['collection_policy'] if 'collection_policy' in recipe else [])), 'Direct3 profile recipe')
    require(spec['installation'] is spec['self_checker_input'] is spec['target_stdin_input'] is None and
            spec['target_role'] == 'wasm' and type(spec['seconds']) is int and 1 <= spec['seconds'] <= 120 and
            spec['benchmark_report_input'] == 'direct3-profile-baseline' and spec['vtune_input'] == 'direct3-profile-vtune' and
            recipe['selection_input'] == 'physics_selection' and
            recipe['baseline_config_input'] == 'direct3-profile-config' and
            recipe['baseline_selection_input'] == 'direct3-profile-selection',
            'explicit hardware-only selected Wasm profile with parent-paced control required')
    frozen, _ = frame.common.document(frame.manifest)
    def source_module(relative, name):
        frame.guard(); body, pin = frame.common.body_pin(frame.source / relative, 2 * 1024**2)
        require(frozen.get(relative) == {key: pin[key] for key in ('bytes', 'sha256')}, 'current complete-manifest profile helper differs')
        module = types.ModuleType(name); module.__file__ = pin['path']; sys.modules[name] = module
        exec(compile(body, module.__file__, 'exec'), module.__dict__)
        frame.pin(Path(pin['path'])); return module
    direct = source_module('test/0073.ros-tests/support/source/ros_unitree/run_direct_physics.py', 'vtune_actual_direct3_coordinator')
    rules = source_module('test/0073.ros-tests/support/source/ros_unitree/direct3_profile.py', 'vtune_actual_direct3_data_predicates')
    args = types.SimpleNamespace(source=frame.source, manifest=frame.manifest,
                                 manifest_sha256=frame.report['source_manifest_sha256'])
    _, frozen, api, unitree = direct.bootstrap(args)
    frame.unitree, frame.frozen, frame.workers, frame.native_aliases = unitree, frozen, {}, {}
    frame.report.update(kind=rules.PROFILE_KIND, execution_bindings=[], physical_robot_access=False,
                        default_ROS_authority=[], optional_WASI_rights=[], hardware_collection_requested=True,
                        physical_hardware_access=True, hardware_scope='HOST_PMU_ONLY_NO_ROBOT',
                        hardware_privilege_setup_performed=False, profiling_changes_timing=True, cleanup=[])
    def document(label):
        body, pin = frame.common.body_pin(frame.input(label), 64 * 1024**2)
        require(pin == frame.inputs[label], 'actual profile document changed')
        return unitree.decode(body), pin
    baseline, baseline_pin = document(spec['benchmark_report_input'])
    collection_policy = rules.profile_policy(recipe)
    case = rules.select_case(baseline, recipe['language'], collection_policy=collection_policy)
    frame.report.update(hardware_collection_policy=collection_policy,
        hardware_profile_authorization=('EXPLICIT_REQUIRED_HARDWARE_REPLAY' if collection_policy == rules.REQUIRED_HARDWARE_POLICY
                                        else 'ACTUAL_SELECTED_LANGUAGE_SLOWDOWN'),
        original_baseline_profiling_required=case['actual_control']['profiling_required'])
    require(baseline['source'] == str(frame.source) and baseline['source_manifest_sha256'] == frame.report['source_manifest_sha256'],
            'profile requires an actual baseline from this same complete frozen source')
    baseline_config, config_pin = document(recipe['baseline_config_input'])
    require(config_pin == baseline['configuration'], 'baseline configuration full7 must be the actual original report selection')
    original_selection, selection_pin = document(recipe['baseline_selection_input'])
    thin = baseline_config['inputs']['physics_selection']
    require({key: selection_pin[key] for key in ('path', 'bytes', 'sha256')} == thin and
            any(pin == selection_pin for pin in baseline['input_pins']), 'baseline physics selection needs its original current full7')
    selection, _ = document(recipe['selection_input'])
    rules.same_configuration(baseline_config, frame.config)
    rules.same_physics(original_selection, selection)
    require(original_selection['expected_network_namespace'] != selection['expected_network_namespace'],
            'profiling requires a new explicitly observed private namespace, not old process continuity')
    for pin in baseline['input_pins']:
        require(frame.pin(pin['path']) == pin, 'an original measured input/artifact changed before profiling')
    require(baseline_config['cgroup'] == frame.config['cgroup'], 'same selected actual task cgroup required; new namespace is not process continuity')
    frame.report['original_cgroup_observation'] = baseline['cgroup_before']
    admitted = direct.prepare(frame, api, unitree, selection, profiling=True)
    simulation = admitted['simulation']; sim = copy.deepcopy(simulation.validate(frame))
    require(sim['robot'] == 'go2' and sim['family'] == 'go' and sim['domain'] == 1 and sim['peer_transport'] == 'ros2',
            'same actual Go2/domain1/ROS2 model workload required')
    selected = case['derived_selection']
    measure = selection['measurement']; chosen = selection['guests'][recipe['language']]
    expected = {'language': recipe['language'], 'job': {'executable': selection['workers']['ros2'], 'arguments': []},
        'denied_job': {'executable': selection['workers']['ros2'], 'arguments': []}, 'runtime_input': selection['workers']['ros2'],
        'guest_input': chosen['positive'], 'denied_guest_input': chosen['denied'],
        'guest_build_input': chosen['positive_receipt'], 'denied_build_input': chosen['denied_receipt'],
        'native_job': {'executable': selection['native_controller'], 'arguments': []},
        'native_binary_input': selection['native_controller'], 'native_build_input': selection['reports']['native_products'],
        'steps': measure['steps'], 'warmup_steps': measure['warmup_steps'], 'slowdown_trigger': measure['slowdown_trigger'],
        'controller_mode': 'go2-stand', 'hold': None, 'modular': admitted['modular'], 'libraries': admitted['libraries']}
    require(selected == expected and spec['target'] == selected['job'] and spec['target_input'] == selected['runtime_input'],
            'selected language/artifact/control workload must equal the actual measured original case')
    physical = image(frame, spec['target_input'])
    require(not Path(frame.command(spec['target'])[0]).name.startswith('qemu-'), 'hardware microarchitecture cannot profile a QEMU surrogate')
    connection, grants = direct.policy(admitted['chosen'], True, admitted['records'])
    bindings = [row for row in baseline['execution_bindings'] if row.get('role') == 'wasm' and row.get('language') == recipe['language']]
    require(len(bindings) == 1 and bindings[0]['grants'] == grants and bindings[0]['default_authority'] == [] and
            bindings[0]['optional_WASI_rights'] == [], 'one real measured positive Worker binding with unchanged exact grants required')
    argv = bindings[0]['actual_command']; indexes = [i for i, token in enumerate(argv) if token == '--ros2-single-gateway-socket']
    require(len(indexes) == 1 and argv[indexes[0] + 1] == 'main', 'original exact positive socket operand required')
    old_socket = Path(argv[indexes[0] + 2])
    require(old_socket.parent == Path(baseline_pin['path']).parent and re.fullmatch(r's[0-9]+', old_socket.name),
            'original Gateway socket must have belonged to the actual measured output')
    product = admitted['binaries'][recipe['language'], 'positive']
    require(direct.runtime_argv(admitted, frame, product, True, connection, grants, old_socket) == argv,
            'original actual Runtime argv/policy no longer matches the current pinned workload')
    sources, enabled, denied, native_definitions, markers = simulation.direct_contract(sim, selected)
    from benchmark import receipt
    receipt(frame, selected['guest_build_input'], selected['guest_input'], sources, enabled)
    receipt(frame, selected['denied_build_input'], selected['denied_guest_input'], sources, denied)
    receipt(frame, selected['native_build_input'], selected['native_binary_input'],
            ['test/0073.ros-tests/support/source/ros_unitree/native_ros2_controller.cpp', 'sdk/origin/robots/unitree/include/roswasm_unitree/simulation_control.h'], native_definitions)
    frame.report.update(baseline_report=baseline_pin, baseline_configuration=config_pin, baseline_physics_selection=selection_pin,
        selected_language=recipe['language'], original_measured_case=case, physical_architecture=physical,
        baseline_namespace=original_selection['expected_network_namespace'], current_namespace=selection['expected_network_namespace'],
        scope='Actual selected Wasm Worker host PMU events during the same caller-paced ROS/Gateway/MuJoCo control; profiler perturbs timing; no native-speed or Guest privilege claim')
    vtune = str(frame.input(spec['vtune_input'])); diagnostics = Actors(frame)
    run(diagnostics, frame, 'direct3-vtune-version', [vtune, '--version'])
    for analysis in ('hotspots', 'uarch-exploration'):
        run(diagnostics, frame, 'direct3-' + analysis + '-help', [vtune, '-help', 'collect', analysis])
    diagnostic_cleanup = diagnostics.close()
    require(diagnostic_cleanup['status'] == 'PASS', 'owned VTune diagnostics cleanup required')
    frame.report['cleanup'].append(diagnostic_cleanup); frame.actors.remove(diagnostics)
    counts = {}
    for analysis in ('hotspots', 'uarch-exploration'):
        owner = Actors(frame); hooks = direct.Hooks(frame, admitted, recipe['language'], label_prefix=analysis + '-')
        frame.simulation_python_command = hooks.python_command
        frame.simulation_python_environment = selection['cp310']['environment']
        frame.simulation_actor_ready = hooks.python_ready; frame.direct_start = hooks.start
        frame.direct_ready = hooks.ready; frame.direct_finished = hooks.finished; frame.direct_line = hooks.line
        result = frame.out / ('result-' + analysis)
        knobs = (['-knob', 'sampling-mode=hw', '-knob', 'enable-stack-collection=true', '-knob', 'stack-size=2048']
                 if analysis == 'hotspots' else ['-knob', 'pmu-collection-mode=detailed', '-knob', 'collect-memory-bandwidth=false'])
        prefix = [vtune, '-collect', analysis, *knobs, '-no-analyze-system', '-follow-child', '-duration', str(spec['seconds']),
                  '-data-limit', '256', '-result-dir', str(result)]
        def start_profile(child):
            owner.check(); held = [value for key, value in owner.owner.owned.items() if key[0] == child.pid]
            require(len(held) == 1 and child.pid in hooks.bindings, 'actual mapped Worker with one retained pidfd required')
            record = held[0]; current = frame.common.identity(child.pid)
            require(current is not None and frame.common.birth(current) == frame.common.birth(record['identity']) and
                    child.poll() is None, 'same actual live Worker birth required before PMU attachment')
            binding = hooks.bindings[child.pid]
            require(binding['role'] == 'wasm' and binding['language'] == recipe['language'] and
                    'live_Runtime_before_main' in binding and 'Gateway_after_real_subscription' in binding,
                    'real exec gate, Runtime maps and initialized Gateway maps must precede PMU attachment')
            profiler, row = owner.start(analysis + '-actual-same-birth-Worker-collection', [*prefix, '-target-pid', str(child.pid)], data=b'', json_output=False)
            deadline = time.monotonic() + 30
            while True:
                owner.check(); raw = b''
                for label in ('stdout', 'stderr'):
                    # These are live owned collector logs, not immutable inputs.
                    # Appends may race this bounded readiness observation. Hold
                    # an ordinary no-follow descriptor and stable identity; full
                    # immutable final log pins remain the actor owner's duty.
                    path = Path(row[label]); fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
                    try:
                        first = os.fstat(fd); limit = frame.limits['log_bytes']
                        require(stat.S_ISREG(first.st_mode) and first.st_size <= limit, 'bounded ordinary owned collector log required')
                        chunks = bytearray()
                        while True:
                            block = os.read(fd, min(65536, limit + 1 - len(chunks)))
                            if not block: break
                            chunks.extend(block); require(len(chunks) <= limit, 'owned collector log grew beyond bound')
                        last, named = os.fstat(fd), path.stat(follow_symlinks=False)
                        require((first.st_dev, first.st_ino) == (last.st_dev, last.st_ino) == (named.st_dev, named.st_ino) and
                                last.st_size <= limit, 'owned collector log identity/limit changed')
                        raw += chunks
                    finally: os.close(fd)
                if b'Collection started' in raw: break
                require(profiler.poll() is None and child.poll() is None and time.monotonic() < deadline,
                        'hardware collector must start before actual bootstrap/first callback/physics step')
                time.sleep(.01)
            now = frame.common.identity(child.pid)
            require(now is not None and frame.common.birth(now) == frame.common.birth(record['identity']), 'Worker birth changed during collector startup')
            frame.report.setdefault('actual_attach_targets', []).append({'analysis': analysis, 'language': recipe['language'],
                'birth': record['identity'], 'pidfd_admitted_birth': record['identity_after'], 'actual_command': binding['actual_command'],
                'membership': Path('/proc', str(child.pid), 'cgroup').read_text(), 'before_first_physics_step': True,
                'Runtime_and_Gateway_mapping_proofs': binding})
            return profiler
        def check_profile(profiler):
            # A short collection that ended before the last real state cannot
            # qualify this replay, even if it collected idle PMU samples.
            owner.check(); require(profiler.poll() is None, 'hardware collection ended before the final actual physics state')
        def finish_profile(profiler): owner.finish(profiler)
        observed = simulation.run_direct_controller(owner, frame, sim, selected, 'wasm', selected['job'],
            markers['wasm_ready'], markers['wasm_complete'], {'start': start_profile, 'check': check_profile, 'finish': finish_profile, 'label': analysis + '-'})
        error = simulation.compare_replay(case['actual_control']['actual_matched_controllers']['native']['actual_physics_states'],
            observed['actual_physics_states'], sim['state_tolerance'], sim['render']['max_pixel_error'] if sim['render'] else 0)
        summary = frame.out / (analysis + '-summary.txt'); events = frame.out / (analysis + '-raw-hardware-events.csv')
        run(owner, frame, analysis + '-actual-summary', [vtune, '-report', 'summary', '-result-dir', str(result), '-report-output', str(summary)])
        run(owner, frame, analysis + '-actual-events', [vtune, '-report', 'hw-events', '-result-dir', str(result), '-format', 'csv', '-csv-delimiter', 'comma', '-report-output', str(events)])
        counts[analysis] = hardware_counts(events); frame.pin(summary); frame.pin(events)
        if analysis == 'hotspots':
            hotspots = frame.out / 'actual-hotspots.csv'
            run(owner, frame, 'actual-hotspots-functions', [vtune, '-report', 'hotspots', '-result-dir', str(result), '-format', 'csv', '-csv-delimiter', 'comma', '-report-output', str(hotspots)])
            frame.pin(hotspots)
        else:
            text, _ = frame.common.body_pin(summary, 16 * 1024**2)
            require(all(label.encode() in text for label in ('Retiring', 'Bad Speculation', 'Front-End Bound')),
                    'actual hardware top-down metrics required; no software-only fallback')
        api.terminal(frame, owner); cleanup = owner.close()
        require(cleanup['status'] == 'PASS' and not hooks.gateways, 'all actual profile/physics/Gateway actors must retire normally')
        frame.actors.remove(owner); frame.report['cleanup'].append(cleanup)
        frame.report.setdefault('direct3_profiled_controls', {})[analysis] = {'actual_control': observed, 'actual_processes': hooks.processes,
            'maximum_error_vs_original_native': error, 'cleanup': cleanup, 'new_performance_acceptance': False}
        frame.report['actual_hardware_event_evidence'] = counts; frame.save()
    frame.report.update(optimization_status='Hardware profiles only; source optimization and new unprofiled matched measurements remain required',
                        original_baseline_status_preserved=baseline['status'], performance_qualified=False)
    return frame.finish(rules.PROFILE_STATUS)


def main():
    parser = arguments(__doc__)
    parser.add_argument("--action", choices=("install", "verify", "collect", "simulation-collect", "direct3-collect"), required=True)
    args = parser.parse_args()
    frame = None
    try:
        frame = Frame(args, "unitree-native-vtune-diagnostics")
        spec = frame.config["profiler"]
        exact(spec, ("installation", "vtune_input", "self_checker_input", "benchmark_report_input",
                     "target", "target_input", "target_stdin_input", "target_role", "seconds",
                     *(["direct3"] if args.action == "direct3-collect" else [])), "profiler selection")
        # This route admits the complete measured language BEFORE even running
        # the selected VTune diagnostic executable. Existing routes are unchanged.
        if args.action == "direct3-collect":
            frame.report["actual_host_pmu_metadata"] = pmu_metadata()
            return direct3_collect(frame, spec)
        owner = Actors(frame)
        frame.report["actual_host_pmu_metadata"] = pmu_metadata()
        if args.action == "install":
            return frame.finish(install(owner, frame, spec))
        vtune = str(frame.input(spec["vtune_input"]))
        run(owner, frame, "vtune-version", [vtune, "--version"])
        for analysis in ("hotspots", "uarch-exploration"):
            run(owner, frame, analysis + "-help", [vtune, "-help", "collect", analysis])
        if args.action == "verify":
            run(owner, frame, "actual-self-checker", [str(frame.input(spec["self_checker_input"]))])
            frame.report["scope"] = "Actual self-checker/version/help diagnostics retained; individual capability outcomes require raw summary review"
            return frame.finish("PASS_RETAINED_VTUNE_CAPABILITY_DIAGNOSTICS")
        baseline, baseline_pin = frame.common.document(frame.input(spec["benchmark_report_input"]))
        modular = baseline.get("kind") == "unitree-native-wasm-modular-performance"
        simulation = baseline.get("kind") == "unitree-actual-mujoco-ros-simulation"
        measured = baseline if modular else baseline.get("actual_direct_roswasm", {})
        require(((modular and args.action == "collect" and baseline.get("status") == "PASS_ACTUAL_MATCHED_MODULAR_MEASUREMENTS") or
                 (simulation and args.action == "simulation-collect" and baseline.get("status") in
                    ("PASS_ACTUAL_UNITREE_ALL", "PASS_ACTUAL_UNITREE_DIRECT_RUNTIME"))) and
                baseline.get("source_manifest_sha256") == frame.report["source_manifest_sha256"] and
                measured.get("profiling_required") is True and baseline.get("input_pins_preserved") is True and
                baseline.get("source_preserved") is True and baseline.get("whole_roots_preserved") is True,
                "closed current actual measured slowdown is required before profiling")
        for pin in baseline["input_pins"]:
            require(frame.pin(pin["path"]) == pin, "measured baseline input/artifact changed before profiling")
        frame.common.cgroup_preserved(baseline["cgroup_before"], frame.before)
        baseline_config, _ = frame.common.document(baseline["configuration"]["path"])
        require(baseline_config["environment"] == frame.config["environment"] and
                baseline_config["simulation" if simulation else "benchmark"] == frame.config["simulation" if simulation else "benchmark"],
                "profile workload/environment differs from actual measured source selection")
        require(spec["target_role"] in ("native", "wasm") and 1 <= spec["seconds"] <= 120 and
                spec["target"]["executable"] == spec["target_input"], "bounded direct physical target required")
        physical = image(frame, spec["target_input"])
        target = frame.command(spec["target"])
        require(not Path(target[0]).name.startswith("qemu-"), "QEMU translation is not a native target microarchitecture comparison")
        require(any(row.get("argv") == target and row.get("status") == "PASS" for row in baseline["commands"]),
                "profile target argv differs from measured actual artifact")
        require(any(pin == frame.inputs[spec["target_input"]] for pin in baseline["input_pins"]),
                "profile target physical input differs from measured artifact")
        data = frame.input(spec["target_stdin_input"]).read_bytes() if spec["target_stdin_input"] is not None else b""
        require(len(data) <= 1024**2, "bounded explicit profile target stdin required")
        frame.report.update(baseline_report=baseline_pin, physical_architecture=physical,
            scope="Hardware events of the actual native physical host target; profiling perturbs timing; no emulated ISA microarchitecture claim")
        counts = {}
        for analysis in ("hotspots", "uarch-exploration"):
            result = frame.out / ("result-" + analysis)
            knobs = (["-knob", "sampling-mode=hw", "-knob", "enable-stack-collection=true", "-knob", "stack-size=2048"]
                if analysis == "hotspots" else ["-knob", "pmu-collection-mode=detailed", "-knob", "collect-memory-bandwidth=false"])
            prefix = [vtune, "-collect", analysis, *knobs, "-no-analyze-system", "-follow-child",
                      "-duration", str(spec["seconds"]), "-data-limit", "256", "-result-dir", str(result)]
            if args.action == "simulation-collect":
                from run_simulation import direct_contract, run_direct_controller, validate
                selection = validate(frame)
                selected = selection["direct_runtime"]
                role = spec["target_role"]
                job = selected["native_job" if role == "native" else "job"]
                require(frame.command(job) == target and not data, "actual selected direct controller and parent-driven stdin required")
                def start_profile(child):
                    owner.check()
                    record = next(value for key, value in owner.owner.owned.items() if key[0] == child.pid)
                    first = record["identity"]
                    require(frame.common.birth(frame.common.identity(child.pid)) == frame.common.birth(first),
                            "actual attach target birth changed")
                    profiler, row = owner.start(analysis + "-actual-pid-collection", [*prefix, "-target-pid", str(child.pid)],
                                               data=b"", json_output=False)
                    deadline = time.monotonic() + 30
                    while True:
                        owner.check()
                        text = Path(row["stdout"]).read_bytes() + Path(row["stderr"]).read_bytes()
                        if b"Collection started" in text:
                            break
                        require(profiler.poll() is None and time.monotonic() < deadline,
                                "actual VTune attach did not report collection start before controller release")
                        time.sleep(.01)
                    frame.report.setdefault("actual_attach_targets", []).append({"analysis": analysis,
                        "birth": first, "pidfd_admitted_birth": record["identity_after"], "argv": target,
                        "membership": Path("/proc", str(child.pid), "cgroup").read_text()})
                    return profiler
                def finish_profile(profiler):
                    owner.finish(profiler)
                # Each collection owns a new simulator/controller/observer
                # group. All earlier actors have retired before the next run.
                markers = direct_contract(selection, selected)[4]
                ready = markers["native_ready" if role == "native" else "wasm_ready"]
                complete = markers["native_complete" if role == "native" else "wasm_complete"]
                observations = run_direct_controller(owner, frame, selection, selected,
                    analysis + "-" + role, job, ready, complete,
                    {"start": start_profile, "finish": finish_profile})
                frame.report.setdefault("profiled_actual_control_observations", {})[analysis] = observations
            else:
                argv = [*prefix, "-return-app-exitcode", "--", *target]
                run(owner, frame, analysis + "-actual-hardware-collection", argv, data=data)
            summary = frame.out / (analysis + "-summary.txt")
            events = frame.out / (analysis + "-raw-hardware-events.csv")
            run(owner, frame, analysis + "-actual-summary", [vtune, "-report", "summary", "-result-dir", str(result),
                                                           "-report-output", str(summary)])
            run(owner, frame, analysis + "-actual-events", [vtune, "-report", "hw-events", "-result-dir", str(result),
                "-format", "csv", "-csv-delimiter", "comma", "-report-output", str(events)])
            counts[analysis] = hardware_counts(events)
            frame.pin(summary)
            frame.pin(events)
            if analysis == "hotspots":
                hotspots = frame.out / "actual-hotspots.csv"
                run(owner, frame, "actual-hotspots-functions", [vtune, "-report", "hotspots", "-result-dir", str(result),
                    "-format", "csv", "-csv-delimiter", "comma", "-report-output", str(hotspots)])
                frame.pin(hotspots)
            else:
                require(summary.stat().st_size <= 16 * 1024**2, "microarchitecture summary exceeds bound")
                text = summary.read_text()
                require(all(name in text for name in ("Retiring", "Bad Speculation", "Front-End Bound")),
                        "actual top-down microarchitecture metrics absent; retain unsupported result")
            frame.report["actual_hardware_event_evidence"] = counts
            frame.save()
        frame.report["optimization_status"] = "Profiles collected; actual source optimization and matching reruns are separate required work"
        return frame.finish("PASS_OBSERVED_ACTUAL_HARDWARE_PROFILES")
    except BaseException as error:
        if frame:
            return frame.finish("FAIL", repr(error))
        raise


if __name__ == "__main__":
    raise SystemExit(main())
