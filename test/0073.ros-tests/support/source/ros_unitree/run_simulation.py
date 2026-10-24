#!/usr/bin/env python3
"""Compare actual native/Wasm control, real MuJoCo, and independent DDS peers.

Four separate scopes are retained: controller byte/numeric agreement, direct
fixed-step physics replay, native-relayed DDS physics, and actual typed Guest
ROS2 publish/subscribe. The last scope uses the real SDK direct_go2 example;
neither a native relay nor a std_msgs/String marker can qualify that path.
"""
import math
import base64
import hashlib
import os
from pathlib import Path
import re
import struct
import time
import types

from common import Actors, Frame, actor_control_arguments, arguments, current_loopback, exact, require
from messages import controller_bytes
from benchmark import image, percentiles, receipt


def validate(frame):
    spec = frame.config["simulation"]
    exact(spec, ("robot", "family", "model_input", "model_upstream", "keyframe", "dt_ns", "steps",
                 "domain", "interface", "expected_network_namespace", "motor_sensors", "imu", "render",
                 "state_abs_limit", "state_tolerance", "command_tolerance", "mode_machine", "step_timeout_seconds",
                 "python_input", "peer_transport", "controllers", "direct_runtime"), "simulation selection")
    require(spec["family"] in ("go", "hg") and type(spec["robot"]) is str and
            type(spec["steps"]) is int and 1 <= spec["steps"] <= 2000 and
            type(spec["dt_ns"]) is int and 100000 <= spec["dt_ns"] <= 20000000, "bounded robot/family/time selection required")
    require(spec["model_upstream"] in frame.config["upstreams"] and
            frame.input(spec["model_input"]).is_relative_to(Path(frame.config["upstreams"][spec["model_upstream"]]["root"])),
            "model must be an actual admitted official upstream asset")
    require(spec["interface"] == "lo" and type(spec["domain"]) is int and 1 <= spec["domain"] <= 232 and
            spec["peer_transport"] in ("sdk2", "ros2"), "explicit loopback SDK2/ROS2 simulation transport required")
    frame.report["actual_loopback_namespace"] = current_loopback(spec["expected_network_namespace"])
    require(type(spec["motor_sensors"]) is list and 1 <= len(spec["motor_sensors"]) <= (20 if spec["family"] == "go" else 35),
            "complete explicit motor map required")
    capacity, slots = (20 if spec["family"] == "go" else 35), []
    for row in spec["motor_sensors"]:
        exact(row, ("actuator", "slot", "q", "dq", "tau"), "actual actuator/carrier map")
        require(type(row["slot"]) is int and 0 <= row["slot"] < capacity and row["slot"] not in slots and
                all(type(row[key]) is str and row[key] for key in ("actuator", "q", "dq", "tau")),
                "unique actual carrier slot and nonempty native model names required")
        slots.append(row["slot"])
    exact(spec["imu"], ("quaternion", "gyroscope", "accelerometer"), "actual IMU mapping")
    for name in ("state_tolerance", "command_tolerance"):
        require(type(spec[name]) in (int, float) and math.isfinite(spec[name]) and 0 <= spec[name] <= .01,
                "explicit finite narrow numeric tolerance required")
    require(type(spec["state_abs_limit"]) in (int, float) and 1 <= spec["state_abs_limit"] <= 1E6 and
            type(spec["step_timeout_seconds"]) in (int, float) and .01 <= spec["step_timeout_seconds"] <= 10,
            "finite state/step bounds required")
    require(type(spec["mode_machine"]) is int and 0 <= spec["mode_machine"] <= 255, "explicit HG mode required")
    if spec["render"] is not None:
        exact(spec["render"], ("camera", "width", "height", "max_pixel_error"), "actual rendered camera")
        require(type(spec["render"]["max_pixel_error"]) is int and 0 <= spec["render"]["max_pixel_error"] <= 32,
                "explicit bounded pixel comparison tolerance required")
    return spec


def python_command(frame, script, tail=()):
    selected = getattr(frame, "simulation_python_command", None)
    if selected is not None:
        return selected(script, tail)
    spec = frame.config["simulation"]
    return [str(frame.input(spec["python_input"])), "-B", "-s", str(frame.source / "test/0073.ros-tests/support/source/ros_unitree" / script),
            *actor_control_arguments(frame), *tail]


def physics(owner, frame, label, mode, steps, extra=()):
    child, _ = owner.start(label, python_command(frame, "mujoco_driver.py", ["--mode", mode, "--steps", str(steps), *extra]),
                           environment=getattr(frame, "simulation_python_environment", None))
    ready = owner.receive(child)
    require(ready.get("kind") == "ready" and ready.get("protocol") == "unitree-physics-v1" and ready.get("mode") == mode,
            "actual physics readiness differs")
    hook = getattr(frame, "simulation_actor_ready", None)
    if hook is not None:
        hook(owner, child, label, "mujoco_driver.py", ready)
    return child


def peer(owner, frame, label, observer=False):
    tail = ["--transport", frame.config["simulation"]["peer_transport"]]
    if observer:
        tail.append("--observer-only")
    child, _ = owner.start(label, python_command(frame, "native_peer.py", tail),
                           environment=getattr(frame, "simulation_python_environment", None))
    ready = owner.receive(child)
    require(ready.get("kind") == "ready" and ready.get("protocol") == "unitree-native-peer-v1", "actual native peer readiness differs")
    hook = getattr(frame, "simulation_actor_ready", None)
    if hook is not None:
        hook(owner, child, label, "native_peer.py", ready)
    return child


def close_physics(owner, child, count):
    owner.send(child, {"op": "finish"})
    complete = owner.receive(child)
    require(complete.get("kind") == "complete" and complete.get("actual_mujoco_steps") == count,
            "actual physics completion count differs")
    owner.finish(child)


def close_peer(owner, child, count, published):
    owner.send(child, {"op": "finish"})
    complete = owner.receive(child)
    require(complete == {"kind": "complete", "published": published, "observed": count}, "native peer exact count differs")
    owner.finish(child)


def verify_state(physics_row, native_row, tick):
    require(physics_row["tick"] == native_row["tick"] == tick and
            physics_row["state_crc"] == native_row["state_crc"] and
            len(physics_row["motors"]) == len(native_row["motors"]), "independent native state tick/CRC/carrier size differs")
    for actual, native in zip(physics_row["motors"], native_row["motors"]):
        require(all(struct.pack("<f", actual[key]) == struct.pack("<f", native[key]) for key in ("q", "dq", "tau")),
                "independent native motor binary32 observations differ")


def compare_replay(left, right, tolerance, pixel_tolerance=0):
    require(len(left) == len(right), "physics replay step count differs")
    error = 0.0
    for a, b in zip(left, right):
        require(a["step"] == b["step"] and len(a["qpos"]) == len(b["qpos"]) and len(a["qvel"]) == len(b["qvel"]),
                "physics replay state dimensions differ")
        require(a["command_crc"] == b["command_crc"], "native/Wasm independently received full-command CRC differs")
        for key in ("qpos", "qvel"):
            for value, expected in zip(a[key], b[key]):
                require(math.isfinite(value) and math.isfinite(expected), "nonfinite replay state")
                error = max(error, abs(value - expected))
        require(len(a["applied_command"]) == len(b["applied_command"]), "native/Wasm actual command carrier dimensions differ")
        for motor, expected in zip(a["applied_command"], b["applied_command"]):
            require(motor["mode"] == expected["mode"] and all(
                    struct.pack("<f", motor[key]) == struct.pack("<f", expected[key])
                    for key in ("q", "dq", "kp", "kd", "tau")),
                    "native/Wasm independently received command binary32 fields differ")
        if a["rendered"] is not None or b["rendered"] is not None:
            require(a["rendered"] is not None and b["rendered"] is not None,
                    "rendered replay mode differs")
            images = []
            for row in (a["rendered"], b["rendered"]):
                raw = base64.b64decode(row["rgb_base64"], validate=True)
                require(len(raw) == row["width"] * row["height"] * 3 and row["encoding"] == "rgb8" and
                        hashlib.sha256(raw).hexdigest() == row["sha256"], "actual rendered RGB bytes/digest differ")
                images.append(raw)
            require(len(images[0]) == len(images[1]) and
                    max((abs(x - y) for x, y in zip(*images)), default=0) <= pixel_tolerance,
                    "actual rendered native/Wasm pixels exceed declared tolerance")
    require(error <= tolerance, "actual native/Wasm physics replay exceeded declared tolerance")
    return error


def direct_contract(spec, selected):
    """Bind a selected control policy to real source/macro/protocol contracts."""
    language = selected.get("language", "cpp")
    require(language in ("c", "cpp", "javascript"), "explicit real direct Guest language required")
    require(type(selected["modular"]) is bool and type(selected["libraries"]) is list,
            "explicit modular selection and artifact list required")
    if selected["controller_mode"] == "sampled-hold":
        require(language == "cpp", "sampled hold retains its separately compiled typed C++ policy")
        require(not selected["modular"] and not selected["libraries"], "sampled hold uses its separate typed direct build")
        family = 1 if spec["family"] == "go" else 2
        sources = ["sdk/origin/robots/unitree/examples/direct_hold.cpp", "sdk/origin/robots/unitree/include/roswasm_unitree/simulation_hold.h"]
        enabled = {"UWVM_UNITREE_HOLD_ENABLED": 1, "UWVM_UNITREE_HOLD_FAMILY": family}
        denied = {**enabled, "UWVM_UNITREE_HOLD_ENABLED": 0}
        markers = {"denied": "PASS_UNITREE_HOLD_DEFAULT_DENIED", "wasm_ready": "READY_UNITREE_HOLD_V1",
                   "wasm_complete": "PASS_UNITREE_HOLD", "native_ready": "READY_UNITREE_HOLD_NATIVE_V1",
                   "native_complete": "PASS_UNITREE_HOLD_NATIVE"}
    else:
        shared = "sdk/origin/robots/unitree/include/roswasm_unitree/simulation_control.h"
        if language == "cpp":
            sources = ["sdk/origin/robots/unitree/examples/direct_go2.cpp", shared]
            enabled = {"UWVM_UNITREE_DIRECT_ENABLED": 1, "UWVM_UNITREE_MODULAR_MOTION": int(selected["modular"])}
            denied = {"UWVM_UNITREE_DIRECT_ENABLED": 0, "UWVM_UNITREE_MODULAR_MOTION": 0}
        elif language == "c":
            sources = ["sdk/origin/robots/unitree/examples/direct_go2.c", "sdk/origin/robots/unitree/examples/direct_go2_output.cpp", shared]
            enabled, denied = {"UWVM_UNITREE_DIRECT_ENABLED": 1}, {"UWVM_UNITREE_DIRECT_ENABLED": 0}
        else:
            sources = ["sdk/origin/robots/unitree/examples/javascript/direct_go2.mjs",
                       "sdk/origin/robots/unitree/examples/javascript/control_kernel.c", shared]
            enabled = {"UWVM_UNITREE_DIRECT_ENABLED": 1, "UWVM_JS_UNITREE_CONTROL": 1}
            denied = {"UWVM_UNITREE_DIRECT_ENABLED": 0, "UWVM_JS_UNITREE_CONTROL": 1}
        if selected["modular"]:
            graph=[row.get("name") for row in selected["libraries"]]
            complete=graph==["callbacks","motion","perception"]
            require(complete or (language=="cpp" and graph==["callbacks","motion"]),
                    "explicit complete owned graph or historical two-provider C++ graph required")
            enabled["UWVM_UNITREE_MODULAR_MOTION"]=1;denied["UWVM_UNITREE_MODULAR_MOTION"]=0
            if complete:
                enabled["UWVM_UNITREE_MODULAR_PERCEPTION"]=1;denied["UWVM_UNITREE_MODULAR_PERCEPTION"]=0
                sources += ["sdk/origin/examples/robotics_modular/unitree_motion_abi.h",
                            "sdk/origin/examples/robotics_modular/unitree_perception_abi.h",
                            "sdk/origin/examples/robotics_modular/unitree_modular_control.h"]
            else:sources.append("sdk/origin/examples/robotics_modular/unitree_motion_abi.h")
        else:
            require(not selected["libraries"], "inline stand has no library artifact selection")
        markers = {"denied": "PASS_UNITREE_DIRECT_DEFAULT_DENIED", "wasm_ready": "READY_UNITREE_DIRECT_V1",
                   "wasm_complete": "PASS_UNITREE_DIRECT", "native_ready": "READY_UNITREE_NATIVE_V1",
                   "native_complete": "PASS_UNITREE_NATIVE"}
    native = {"UWVM_UNITREE_NATIVE_HOLD": int(selected["controller_mode"] == "sampled-hold"),
              "UWVM_UNITREE_NATIVE_HG": int(spec["family"] == "hg")}
    return sources, enabled, denied, native, markers


def runtime_backends(frame, selected):
    """Read compiled features from an actual full-MD Worker producer receipt.

    Existing configurations without this optional receipt retain the original
    dual-backend CLI requirements. A ros2-only Worker cannot parse ROS1 policy
    flags: admitting their absence therefore requires its actual compile rows,
    exact executable, complete original source and consumed-source byte bridge.
    The original receipt/source labels remain unchanged.
    """
    label = selected.get("runtime_build_input")
    if label is None:
        return {1, 2}
    build, build_pin = frame.common.document(frame.input(label))
    require(frame.pin(frame.input(label)) == build_pin and
            build.get("kind") == "native-interpreter-four-profile-build" and
            build.get("status") == "PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED" and
            build.get("runtime_backend") == "uwvm-interpreter" and build.get("no_reuse") is True and
            build.get("reused_products") == 0 and all(build.get(key) is True for key in
            ("source_preserved", "inputs_preserved", "products_preserved")) and
            not build.get("error") and not build.get("errors") and not build.get("finalization_errors"),
            "actual complete retained interpreter build receipt required")
    cleanup = build.get("cleanup")
    cleanup = [cleanup] if type(cleanup) is dict else cleanup
    require(type(cleanup) is list and cleanup and all(row["status"] == "PASS" and
            not row.get("remaining") and not row.get("errors") for row in cleanup),
            "retained Worker producer must have completed owned cleanup")
    executable = frame.pin(frame.input(selected["runtime_input"]))
    matches = [row for row in build["profiles"] if row.get("binary") == executable]
    require(len(matches) == 1, "selected Runtime must be one exact compiled profile")
    profile = matches[0]
    features = {"none": set(), "ros1": {1}, "ros2": {2}, "dual": {1, 2}}
    require(profile["profile"] in features and profile["status"] == "BUILT_NOT_EXECUTED" and
            profile["full_dependencies_verified"] is True and profile["no_reuse"] is True and
            profile["reused_products"] == 0 and len(profile["objects"]) == 3,
            "actual three-translation-unit profile proof required")
    old_source = Path(build["source"])
    require(old_source.is_absolute() and old_source.resolve(strict=True) == old_source and
            any(old_source == root or old_source.is_relative_to(root) for root in frame.roots),
            "complete original Worker source must remain separately inventoried")
    original_manifest = build["source_before"]["manifest"]
    require(original_manifest["path"] == str(old_source.parent / "source-manifest.json") and
            original_manifest["sha256"] == build["source_manifest_sha256"] and
            frame.common.source_state(old_source, Path(original_manifest["path"])) == build["source_before"],
            "original Worker source/full manifest differs from actual compilation")
    # This exact sibling manifest is metadata, not a new whole parent root or
    # an executable input label. Its actual full7 is retained through finish.
    require(frame.pin(original_manifest["path"]) == original_manifest,
            "original Worker manifest physical identity changed")
    current_frozen, current_manifest = frame.common.document(frame.manifest)
    old_frozen, old_manifest = frame.common.document(Path(original_manifest["path"]))
    require(old_manifest == original_manifest and current_manifest == frame.pin(frame.manifest),
            "current/original complete source manifests changed")
    def retained(value):
        if type(value) is dict:
            if {"path", "bytes", "sha256"} <= set(value):
                actual = frame.pin(value["path"])
                require(all(actual[key] == value[key] for key in actual if key in value),
                        "retained actual Worker input/product changed")
            for item in value.values():
                retained(item)
        elif type(value) is list:
            for item in value:
                retained(item)
    retained(build)
    observed = []
    for item in profile["objects"]:
        command = build["commands"][item["command_index"]]
        argv = command["argv"]
        require(command["status"] == "PASS" and command["returncode"] == 0 and
                all(flag in argv for flag in ("-MD", "-MF", "-MT", "-c", "-o", "-UNDEBUG")) and
                "-MMD" not in argv and argv[argv.index("-c") + 1] == item["source"]["path"] and
                argv[argv.index("-o") + 1] == item["object"]["path"] and
                argv[argv.index("-MF") + 1] == item["dependency_file"]["path"] and
                argv[argv.index("-MT") + 1] == item["object"]["path"],
                "actual full-MD compile/source/object association required")
        macros = {version for version in (1, 2) if "-DUWVM_IMPORT_ROS" + str(version) in argv}
        require(macros == features[profile["profile"]] and
                not any(token.startswith(("-DUWVM_IMPORT_ROS1=", "-DUWVM_IMPORT_ROS2=",
                                          "-UUWVM_IMPORT_ROS", "@")) for token in argv),
                "actual unambiguous compiled backend macros required")
        for pin in item["dependencies"]:
            path = Path(pin["path"])
            if path.is_relative_to(old_source):
                relative = str(path.relative_to(old_source))
                value = {key: pin[key] for key in ("bytes", "sha256")}
                counterpart = frame.pin(frame.source / relative)
                require(old_frozen.get(relative) == current_frozen.get(relative) == value ==
                        {key: counterpart[key] for key in ("bytes", "sha256")},
                        "consumed Worker source changed; fresh compilation is required")
        observed.append({"command_index": item["command_index"], "source": item["source"],
                         "compiled_backends": sorted(macros)})
    frame.report.setdefault("direct_runtime_compiled_profiles", []).append({
        "original_build": build_pin, "original_source": str(old_source),
        "original_manifest": original_manifest, "selected_binary": executable,
        "profile": profile["profile"], "compiled_backends": sorted(features[profile["profile"]]),
        "actual_compile_rows": observed, "raw_producer_rewritten": False})
    frame.save()
    return features[profile["profile"]]


def pure_provider_imports(frame, label):
    """Read actual import sections with the MF-bound SDK binary inspector."""
    relative = "sdk/origin/tools/check_wasm_imports.py"
    raw, pin = frame.common.body_pin(frame.source / relative, 1024**2)
    frozen, _ = frame.common.document(frame.manifest)
    require(frame.pin(frame.source / relative) == pin and
            frozen.get(relative) == {key: pin[key] for key in ("bytes", "sha256")},
            "actual current frozen import inspector required")
    inspector = types.ModuleType("unitree_direct_provider_import_inspector")
    inspector.__file__ = str(frame.source / relative)
    exec(compile(raw, inspector.__file__, "exec"), inspector.__dict__)
    body, binary_pin = frame.common.body_pin(frame.input(label), 64 * 1024**2)
    require(frame.pin(frame.input(label)) == binary_pin, "actual pure provider changed")
    imports = inspector.inspect(body)
    require(not any(row["module"].startswith("uwvm_ros1") for row in imports),
            "ros2-only Runtime requires actual providers without any ROS1 imports")
    frame.report.setdefault("direct_provider_imports", []).append({"binary": binary_pin, "imports": imports})
    frame.save()


def bind_modular_libraries(frame, selected):
    """Bind pure providers and their empty module grants to actual root argv."""
    owned = getattr(frame, "direct_modular_bind", None)
    if owned is not None:
        owned(selected)
        return
    require(len(selected["libraries"]) == 2, "actual Unitree modular graph has callbacks then motion")
    command = frame.command(selected["job"])
    backends = runtime_backends(frame, selected)
    require(2 in backends, "actual direct controller needs a compiled ROS2 backend")
    prefixes = ("--wasip1-single", "--ros2-single") + (("--ros1-single",) if 1 in backends else ())
    globals_required = ("--wasip1-global-disable", "--ros2-global-disable") + (("--ros1-global-disable",) if 1 in backends else ())
    if 1 not in backends:
        require(not any(token.startswith("--ros1-") for token in command),
                "compiled-out ROS1 has no CLI surface; it cannot receive policy flags")
        pure_provider_imports(frame, selected["guest_input"])
    expected_preloads = []
    required = {
        "callbacks": ["sdk/origin/examples/robotics_modular/unitree_callback.c", "sdk/origin/examples/robotics_modular/unitree_motion_abi.h"],
        "motion": ["sdk/origin/examples/robotics_modular/unitree_motion.c", "sdk/origin/examples/robotics_modular/unitree_motion_abi.h",
                   "sdk/origin/robots/unitree/include/roswasm_unitree/simulation_control.h"]}
    for item, name in zip(selected["libraries"], ("callbacks", "motion")):
        exact(item, ("name", "guest_input", "build_input"), "actual pure Unitree preload")
        require(item["name"] == name, "provider order/name differs from actual import graph")
        receipt(frame, item["build_input"], item["guest_input"], required[name], {})
        expected_preloads.append((str(frame.input(item["guest_input"])), name))
        if 1 not in backends:
            pure_provider_imports(frame, item["guest_input"])
        for prefix in prefixes:
            require(any(command[index:index + 2] == [prefix + "-disable", name] for index in range(len(command) - 1)),
                    "pure Unitree provider must have explicit disabled WASI/ROS1/ROS2 policy")
            require(not any(command[index:index + 2] == [prefix + suffix, name]
                            for suffix in ("-enable", "-grant") for index in range(len(command) - 1)),
                    "pure Unitree provider cannot receive native authority")
    actual_preloads = [(command[index + 1], command[index + 2]) for index, token in enumerate(command)
                       if token == "--wasm-preload-library" and index + 2 < len(command)]
    require(actual_preloads == expected_preloads and all(token in command for token in
            globals_required),
            "actual root must use exactly the bound pure providers and disabled global authority")


def direct_runtime(owner, frame, spec):
    selected = spec["direct_runtime"]
    exact(selected, ("job", "denied_job", "runtime_input", "guest_input", "denied_guest_input",
                     "guest_build_input", "denied_build_input", "steps", "native_job", "native_binary_input",
                     "native_build_input", "warmup_steps", "slowdown_trigger", "controller_mode", "hold",
                     "modular", "libraries",
                     *(["language"] if "language" in selected else []),
                     *(["runtime_build_input"] if "runtime_build_input" in selected else [])), "actual direct Runtime selection")
    count = selected["steps"]
    require(type(count) is int and 1 <= count <= min(32, spec["steps"]) and spec["domain"] == 1,
            "direct matched controllers require bounded steps and their explicit isolated domain 1")
    require(selected["controller_mode"] in ("go2-stand", "sampled-hold"), "explicit direct control policy required")
    if selected["controller_mode"] == "go2-stand":
        require(spec["robot"] == "go2" and spec["family"] == "go" and selected["hold"] is None and
                [row["slot"] for row in spec["motor_sensors"]] == list(range(12)),
                "the Go2 stand trajectory requires its actual twelve ordered Go2 motor slots")
    else:
        exact(selected["hold"], ("kp", "kd", "mode_pr"), "explicit simulation hold gains/mode")
        require(all(type(selected["hold"][key]) in (int, float) and math.isfinite(selected["hold"][key]) and
                    0 < selected["hold"][key] <= maximum for key, maximum in (("kp", 1000), ("kd", 100))) and
                type(selected["hold"]["mode_pr"]) is int and 0 <= selected["hold"]["mode_pr"] <= 255,
                "finite positive bounded sampled-hold gains and explicit mode_pr required")
        require(spec["family"] != "go" or (selected["hold"]["mode_pr"] == 0 and spec["mode_machine"] == 0),
                "GO carriers have no HG mode_pr/mode_machine fields")
    require(spec["peer_transport"] == "ros2", "direct Guest needs actual ROS2 endpoint discovery before volatile initial state")
    require(selected["job"]["executable"] == selected["denied_job"]["executable"] == selected["runtime_input"],
            "direct actual Runtime invocation required")
    image(frame, selected["runtime_input"])
    require(image(frame, selected["native_binary_input"]) == image(frame, selected["runtime_input"]) and
            selected["native_job"]["executable"] == selected["native_binary_input"],
            "matched compiled native C++ and Runtime physical architecture required")
    require(type(selected["warmup_steps"]) is int and 0 <= selected["warmup_steps"] <= 2 and
            selected["warmup_steps"] < count, "bounded explicit initial control warmup required")
    require(type(selected["slowdown_trigger"]) in (int, float) and 1 <= selected["slowdown_trigger"] <= 10,
            "explicit finite direct-control slowdown trigger required")
    # Compile definitions are bound below to the actual producer receipt. The
    # hold policy has separate typed GO/HG builds rather than a Go2 joint table
    # silently reused for a humanoid or wheeled model.
    sources, enabled, denied, native_definitions, markers = direct_contract(spec, selected)
    receipt(frame, selected["guest_build_input"], selected["guest_input"], sources, enabled)
    # A denied stand root is compiled without modular imports, while positive
    # modular source includes the explicitly source-pinned scalar ABI header.
    denied_sources = [name for name in sources if not name.startswith("sdk/origin/examples/robotics_modular/")]
    receipt(frame, selected["denied_build_input"], selected["denied_guest_input"], denied_sources, denied)
    native_header = "simulation_hold.h" if selected["controller_mode"] == "sampled-hold" else "simulation_control.h"
    receipt(frame, selected["native_build_input"], selected["native_binary_input"],
            ["test/0073.ros-tests/support/source/ros_unitree/native_ros2_controller.cpp", "sdk/origin/robots/unitree/include/roswasm_unitree/" + native_header], native_definitions)
    if selected["modular"]:
        bind_modular_libraries(frame, selected)
    # Independent native subscriber is live during the empty-grant case.
    quiet = physics(owner, frame, "denied-physics", "dds", 1, ["--quiet-only"])
    denied, _ = start_direct(owner, frame, "direct-default-denied", selected["denied_job"], selected, "denied")
    require(direct_line(owner, frame, denied) == markers["denied"], "actual Guest default-denial marker differs")
    owner.finish(denied)
    finish_direct(owner, frame, denied, "denied")
    owner.send(quiet, {"op": "quiet", "seconds": .2})
    witness = owner.receive(quiet)
    require(witness.get("kind") == "quiet" and witness.get("native_dds_commands") == witness.get("actual_mujoco_steps") == 0 and
            witness.get("samples", 0) >= 30 and witness.get("elapsed_ns", 0) >= 200000000,
            "actual independent zero-command observation missing")
    close_physics(owner, quiet, 0)
    results = {}
    for role, job, ready, complete in (("native", selected["native_job"], markers["native_ready"], markers["native_complete"]),
            ("wasm", selected["job"], markers["wasm_ready"], markers["wasm_complete"])):
        results[role] = run_direct_controller(owner, frame, spec, selected, role, job, ready, complete)
    maximum = compare_replay(results["native"]["actual_physics_states"], results["wasm"]["actual_physics_states"],
                             spec["state_tolerance"], spec["render"]["max_pixel_error"] if spec["render"] else 0)
    timing = {role: percentiles(value["raw_control_roundtrip_ns"][selected["warmup_steps"]:])
              for role, value in results.items()}
    ratio = timing["wasm"]["p50_ns"] / timing["native"]["p50_ns"]
    return {"controller_mode": selected["controller_mode"], "Guest_language": selected.get("language", "cpp"),
            "default_denial_witness": witness, "actual_matched_controllers": results,
            "native_wasm_physics_max_abs_error": maximum, "control_roundtrip_statistics": timing,
            "performance_sample_qualification": "AT_LEAST_30_PER_ROLE" if count - selected["warmup_steps"] >= 30 else "INSUFFICIENT_FOR_PERFORMANCE_QUALIFICATION",
            "slowdown_p50": ratio, "slowdown_trigger": selected["slowdown_trigger"],
            "profiling_required": count - selected["warmup_steps"] >= 30 and ratio > selected["slowdown_trigger"],
            "scope": "Matched compiled C++ native ROS2 versus actual typed " + selected.get("language", "cpp") +
                     " Guest ROS2, shared C kernel/inputs/CRC, separate simulator and native state witness; caller-paced control/barrier/physics/DDS timing includes supervisor overhead"}


def start_direct(owner, frame, label, job, selected, role):
    """An explicit Root runner may hold/map a new standalone main before G.

    Original ungated fixture recipes retain their existing entry behavior.
    The controlled-physics runner installs this hook only after actual binary,
    Worker, Gateway, complete native catalog and policy admission. No hook may
    replace a Guest callback, native observation or a simulator step.
    """
    hook = getattr(frame, "direct_start", None)
    return hook(owner, label, job, selected, role) if hook is not None else owner.start(label, frame.command(job))


def finish_direct(owner, frame, child, role):
    hook = getattr(frame, "direct_finished", None)
    if hook is not None:
        hook(owner, child, role)


def direct_line(owner, frame, child):
    """Preserve binary exec-gate offsets for the separately controlled runner."""
    hook = getattr(frame, "direct_line", None)
    return hook(owner, child) if hook is not None else owner.receive_line(child)


def run_direct_controller(owner, frame, spec, selected, role, job, ready_marker, complete_marker, profile_hooks=None):
    count = selected["steps"]
    # A collection prefix changes owned filenames only. Logical role, source,
    # native messages, control gates and physics semantics remain unchanged.
    label_prefix = profile_hooks.get("label", "") if profile_hooks else ""
    require(label_prefix in ("", "hotspots-", "uarch-exploration-"), "closed collection label prefix required")
    simulator = physics(owner, frame, label_prefix + "direct-" + role + "-physics", "dds", count, ["--bootstrap"])
    witness_peer = peer(owner, frame, label_prefix + "direct-" + role + "-native-state-witness", True)
    guest, _ = start_direct(owner, frame, label_prefix + "direct-" + role + "-controller", job, selected, role)
    if selected["controller_mode"] == "sampled-hold":
        capacity = 20 if spec["family"] == "go" else 35
        slots = [row["slot"] for row in spec["motor_sensors"]]
        hold = selected["hold"]
        setup = (b"UHL1" + struct.pack("<II4Bff", count, spec["dt_ns"], capacity, len(slots),
                 hold["mode_pr"], spec["mode_machine"], hold["kp"], hold["kd"]) + bytes(slots + [255] * (35 - len(slots))))
        require(len(setup) == 59, "exact bounded hold setup required")
    else:
        setup = b"UCK1" + struct.pack("<II", count, spec["dt_ns"])
    owner.write(guest, setup)
    require(direct_line(owner, frame, guest) == ready_marker, "actual typed controller endpoint readiness differs")
    hook = getattr(frame, "direct_ready", None)
    if hook is not None:
        hook(owner, guest, role)
    # Native C++ constructs its publisher eagerly. The Guest/Gateway opens
    # the real LowCmd publisher on the first genuine callback publication;
    # demanding it before the first LowState would deadlock discovery.
    wasm_role = job["executable"] == selected["runtime_input"]
    expected_publishers = 0 if wasm_role else 1
    owner.send(witness_peer, {"op": "match", "state_subscribers": 2, "command_publishers": expected_publishers})
    matched = owner.receive(witness_peer)
    require(matched == {"kind": "matched", "state_subscribers": 2, "command_publishers": expected_publishers},
            "actual pre-bootstrap subscriber/lazy-publisher discovery differs")
    first_publish_match = None if wasm_role else matched
    profile_actor = profile_hooks["start"](guest) if profile_hooks else None
    if profile_hooks and "check" in profile_hooks:
        profile_hooks["check"](profile_actor)
    owner.send(simulator, {"op": "bootstrap"})
    previous = owner.receive(simulator)
    require(previous.get("kind") == "bootstrap" and previous["actual_mujoco_steps"] == 0,
            "actual initial native physics state missing")
    owner.send(witness_peer, {"op": "receive", "step": 1})
    observed = owner.receive(witness_peer)
    verify_state(previous, observed, 1)
    states, guest_rows, timings, polls = [], [], [], 0
    for step in range(1, count + 1):
        if profile_hooks and "check" in profile_hooks:
            profile_hooks["check"](profile_actor)
        began = time.monotonic_ns()
        while True:
            polls += 1
            require(polls <= 4096, "actual typed Guest poll budget exhausted")
            owner.write(guest, b"R")
            line = direct_line(owner, frame, guest)
            if line != "UNITREE_AGAIN":
                break
            time.sleep(.001)
        match = re.fullmatch(r"UNITREE_STEP ([1-9][0-9]*) ([1-9][0-9]*) ([0-9]+) ([0-9]+)", line)
        require(match is not None, "actual typed Guest publication marker differs")
        published_step, tick, command_crc, state_crc = map(int, match.groups())
        require(published_step == tick == step and state_crc == previous["state_crc"], "actual Guest used a stale/different LowState")
        words = direct_line(owner, frame, guest).split()
        observed_count = (20 if spec["family"] == "go" else 35) if selected["controller_mode"] == "sampled-hold" else 12
        require(len(words) == 3 + 3 * observed_count and words[:3] == ["UNITREE_STATE", str(step), str(step)] and
                all(word.isascii() and word.isdecimal() and int(word) <= 0xFFFFFFFF for word in words[3:]),
                "complete exact Guest motor-state bit records required")
        expected_bits = [struct.unpack("<I", struct.pack("<f", motor[key]))[0]
                         for motor in observed["motors"][:observed_count] for key in ("q", "dq", "tau")]
        require(list(map(int, words[3:])) == expected_bits, "Guest canonical decode differs from independent native state")
        if first_publish_match is None:
            require(step == 1 and wasm_role, "one genuine first-publication discovery barrier required")
            owner.send(witness_peer, {"op": "match", "state_subscribers": 2, "command_publishers": 1})
            first_publish_match = owner.receive(witness_peer)
            require(first_publish_match == {"kind": "matched", "state_subscribers": 2, "command_publishers": 1},
                    "real LowCmd publisher must be discovered after first Guest publish and before physics step")
        owner.send(simulator, {"op": "step", "step": step})
        actual = owner.receive(simulator)
        require(actual.get("kind") == "state" and actual["command_crc"] == command_crc,
                "Guest publish marker lacks independent actual LowCmd receipt")
        owner.send(witness_peer, {"op": "receive", "step": step + 1})
        observed = owner.receive(witness_peer)
        verify_state(actual, observed, step + 1)
        timings.append(observed["received_monotonic_ns"] - began)
        states.append(actual)
        guest_rows.append({"step": step, "input_tick": tick, "state_crc": state_crc, "command_crc": command_crc,
                           "state_float32_words": expected_bits})
        previous = actual
        if profile_hooks and "check" in profile_hooks:
            profile_hooks["check"](profile_actor)
    owner.write(guest, b"Q")
    require(direct_line(owner, frame, guest) == complete_marker + " steps=" + str(count), "actual typed controller close/count differs")
    guest.stdin.close()
    owner.finish(guest)
    finish_direct(owner, frame, guest, role)
    if profile_hooks:
        profile_hooks["finish"](profile_actor)
    close_physics(owner, simulator, count)
    close_peer(owner, witness_peer, count + 1, 0)
    return {"kind": "actual-" + spec["robot"] + "-" + selected["controller_mode"] + "-" + role + "-DDS-MuJoCo", "steps": count, "polls": polls,
            "pre_bootstrap_endpoint_match": matched, "actual_endpoint_match": first_publish_match,
            "raw_control_roundtrip_ns": timings,
            "guest_callbacks": guest_rows, "actual_physics_states": states,
            "scope": ("Real Guest imports and generated canonical codecs" if role.endswith("wasm") else "Compiled native C++ ROS2 and actual native messages") +
                     ", same shared control kernel and real separate simulator/CRC/state witness"}


def native_model(owner, frame, spec):
    """Real Go/HG state-feedback hold; no Go2 trajectory or Wasm equivalence claim."""
    simulator = physics(owner, frame, "native-model-physics", "dds", spec["steps"], ["--bootstrap"])
    native = peer(owner, frame, "native-model-peer")
    owner.send(simulator, {"op": "bootstrap"})
    initial = owner.receive(simulator)
    owner.send(native, {"op": "receive", "step": 1})
    observed = owner.receive(native)
    verify_state(initial, observed, 1)
    capacity = 20 if spec["family"] == "go" else 35
    active = {mapping["slot"] for mapping in spec["motor_sensors"]}
    # Desired positions come from an actual native state sample, never a
    # fabricated controller state or Go2 joint table applied to a humanoid.
    states = []
    for step in range(1, spec["steps"] + 1):
        command = [{"mode": int(index in active), "q": observed["motors"][index]["q"] if index in active else 0, "dq": 0,
                    "kp": 20 if index in active else 0, "kd": 1 if index in active else 0, "tau": 0}
                   for index in range(capacity)]
        owner.send(native, {"op": "publish", "step": step, "motors": command})
        published = owner.receive(native)
        owner.send(simulator, {"op": "step", "step": step})
        actual = owner.receive(simulator)
        require(actual.get("kind") == "state" and actual["command_crc"] == published["command_crc"],
                "actual native-model command receipt differs")
        owner.send(native, {"op": "receive", "step": step + 1})
        observed = owner.receive(native)
        verify_state(actual, observed, step + 1)
        states.append(actual)
    close_physics(owner, simulator, spec["steps"])
    close_peer(owner, native, spec["steps"] + 1, spec["steps"])
    return {"robot": spec["robot"], "family": spec["family"], "active_motor_slots": sorted(active),
            "capacity": capacity, "steps": spec["steps"], "actual_states": states,
            "scope": "Actual official native message/CRC and model-specific sensor/actuator mapping with sampled-position hold; no balance, locomotion, Wasm or firmware qualification"}


def main():
    parser = arguments(__doc__)
    parser.add_argument("--scope", choices=("all", "replay-relay", "direct-runtime", "native-model"), default="all")
    args = parser.parse_args()
    frame = None
    try:
        frame = Frame(args, "unitree-actual-mujoco-ros-simulation")
        spec = validate(frame)
        owner = Actors(frame)
        if args.scope == "native-model":
            frame.report["actual_native_model"] = native_model(owner, frame, spec)
        if args.scope in ("all", "replay-relay"):
            selection = spec["controllers"]
            exact(selection, ("native", "wasm", "native_binary_input", "runtime_input", "wasm_binary_input",
                              "native_build_input", "wasm_build_input"), "actual matched controllers")
            require(spec["family"] == "go" and spec["robot"] == "go2", "built-in stand controller is Go2, never HG")
            require(selection["native"]["executable"] == selection["native_binary_input"] and
                    selection["wasm"]["executable"] == selection["runtime_input"], "direct actual executables required")
            require(image(frame, selection["native_binary_input"]) == image(frame, selection["runtime_input"]),
                    "controller/native Runtime must use the same physical architecture")
            sources = ["test/0073.ros-tests/support/source/ros_unitree/controller.c", "test/0073.ros-tests/support/source/ros_unitree/controller_kernel.h",
                       "sdk/origin/robots/unitree/include/roswasm_unitree/simulation_control.h"]
            receipt(frame, selection["native_build_input"], selection["native_binary_input"], sources, {})
            receipt(frame, selection["wasm_build_input"], selection["wasm_binary_input"], sources, {})
            commands = {}
            for role in ("native", "wasm"):
                child, row = owner.start("controller-" + role, frame.command(selection[role]),
                    data=b"UCK1" + struct.pack("<II", spec["steps"], spec["dt_ns"]), json_output=False)
                owner.finish(child)
                commands[role] = controller_bytes(Path(row["stdout"]).read_bytes(), spec["steps"], spec["dt_ns"])
            maximum = 0.0
            for left, right in zip(commands["native"], commands["wasm"]):
                for a, b in zip(left, right):
                    require(a["mode"] == b["mode"], "controller motor modes differ")
                    maximum = max(maximum, *(abs(a[key] - b[key]) for key in ("q", "dq", "kp", "kd", "tau")))
            require(maximum <= spec["command_tolerance"], "actual native/Wasm motor commands differ")
            replays, live = {}, {}
            for mode in ("direct", "dds"):
                for role in ("native", "wasm"):
                    simulator = physics(owner, frame, mode + "-" + role + "-physics", mode, spec["steps"])
                    native = peer(owner, frame, mode + "-" + role + "-native") if mode == "dds" else None
                    states, timings = [], []
                    for step, motors in enumerate(commands[role], 1):
                        published = None
                        if native:
                            owner.send(native, {"op": "publish", "step": step, "motors": motors})
                            published = owner.receive(native)
                            require(published.get("kind") == "published" and published["step"] == step, "actual native publish acknowledgement differs")
                        owner.send(simulator, {"op": "step", "step": step, **({"motors": motors} if not native else {})})
                        actual = owner.receive(simulator)
                        require(actual.get("kind") == "state" and actual["step"] == step, "actual physics step differs")
                        if native:
                            require(actual["command_crc"] == published["command_crc"], "actual DDS command differed from computed input")
                            owner.send(native, {"op": "receive", "step": step})
                            observation = owner.receive(native)
                            verify_state(actual, observation, step)
                            timings.append(observation["received_monotonic_ns"] - published["published_monotonic_ns"])
                        states.append(actual)
                    close_physics(owner, simulator, spec["steps"])
                    if native:
                        close_peer(owner, native, spec["steps"], spec["steps"])
                        live[role] = {"statistics": percentiles(timings), "raw_ns": timings,
                            "scope": "actual native DDS publish through host step barrier/real MuJoCo to independent LowState receipt; excludes controller compute"}
                    replays[mode + "-" + role] = states
            errors = {mode: compare_replay(replays[mode + "-native"], replays[mode + "-wasm"], spec["state_tolerance"],
                                          spec["render"]["max_pixel_error"] if spec["render"] else 0)
                      for mode in ("direct", "dds")}
            frame.report.update(controller_max_abs_error=maximum, replay_max_abs_errors=errors,
                                actual_physics_replays=replays, actual_dds_measurements=live)
        if args.scope in ("all", "direct-runtime"):
            require(spec["direct_runtime"] is not None, "actual typed Runtime/Guest selection is required")
            frame.report["actual_direct_roswasm"] = direct_runtime(owner, frame, spec)
        frame.report["selected_scope"] = args.scope
        return frame.finish("PASS_ACTUAL_UNITREE_" + args.scope.upper().replace("-", "_"))
    except BaseException as error:
        if frame:
            return frame.finish("FAIL", repr(error))
        raise


if __name__ == "__main__":
    raise SystemExit(main())
