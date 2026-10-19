#!/usr/bin/env python3
"""Actual fixed-step, headless MuJoCo with direct or official SDK2 DDS input.

Exactly one thread owns MjData. Native DDS callbacks only copy commands into a
bounded single-slot handoff; they never advance physics or mutate MjData. The
supervisor's stdin barrier is test instrumentation, not a ROS service or Guest
authority. A DDS step consumes exactly one real LowCmd and publishes one real
LowState. Rendering is optional actual MuJoCo offscreen RGB and is identified
separately from the unrendered physics fixture.
"""
import argparse
import base64
import hashlib
import math
import os
from pathlib import Path
import threading
import time

from common import child_arguments, child_guard, exact, finite, read_record, require, write_record
from messages import checked_command, sdk


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    child_arguments(parser)
    parser.add_argument("--mode", choices=("direct", "dds"), required=True)
    parser.add_argument("--bootstrap", action="store_true")
    parser.add_argument("--steps", type=int)
    parser.add_argument("--quiet-only", action="store_true")
    args = parser.parse_args()
    config, guard = child_guard(args)
    spec = config["simulation"]
    require(spec is not None, "explicit simulation selection required")
    steps = spec["steps"] if args.steps is None else args.steps
    require(type(steps) is int and 1 <= steps <= spec["steps"], "bounded selected physics step count required")
    require(spec["interface"] == "lo" and type(spec["domain"]) is int and 1 <= spec["domain"] <= 232,
            "nonphysical domain and loopback-only interface required")
    # Admission happens before the real native simulator or DDS libraries load.
    import mujoco
    model_path = Path(config["inputs"][spec["model_input"]]["path"]).resolve(strict=True)
    model = mujoco.MjModel.from_xml_path(str(model_path))
    data = mujoco.MjData(model)
    model.opt.timestep = finite(spec["dt_ns"], 100000, 20000000, "timestep ns") / 1E9
    require(model.nu == len(spec["motor_sensors"]) and 0 < model.nu <= (20 if spec["family"] == "go" else 35),
            "actual actuator count differs from selected sensor mapping")
    if spec["keyframe"] is not None:
        key = mujoco.mj_name2id(model, mujoco.mjtObj.mjOBJ_KEY, spec["keyframe"])
        require(key >= 0, "selected actual keyframe missing")
        mujoco.mj_resetDataKeyframe(model, data, key)
    mujoco.mj_forward(model, data)
    initial_time = float(data.time)
    require(math.isfinite(initial_time), "finite actual initial simulation time required")
    def sensor(name, size):
        index = mujoco.mj_name2id(model, mujoco.mjtObj.mjOBJ_SENSOR, name)
        require(index >= 0 and int(model.sensor_dim[index]) == size, "selected sensor absent or dimension differs: " + name)
        start = int(model.sensor_adr[index])
        return [float(value) for value in data.sensordata[start:start + size]]
    addresses = []
    slots = set()
    capacity = 20 if spec["family"] == "go" else 35
    for row in spec["motor_sensors"]:
        exact(row, ("actuator", "slot", "q", "dq", "tau"), "native motor sensor map")
        actuator = mujoco.mj_name2id(model, mujoco.mjtObj.mjOBJ_ACTUATOR, row["actuator"])
        require(actuator == len(addresses), "native motor/actuator ordering differs")
        require(type(row["slot"]) is int and 0 <= row["slot"] < capacity and row["slot"] not in slots,
                "unique explicitly mapped native carrier slot required")
        slots.add(row["slot"])
        addresses.append(row)
        for field in ("q", "dq", "tau"):
            sensor(row[field], 1)
    renderer = None
    if spec["render"] is not None:
        require(os.environ.get("MUJOCO_GL") == "osmesa", "explicit CPU offscreen renderer required")
        render = spec["render"]
        require(16 <= render["width"] <= 320 and 16 <= render["height"] <= 240, "bounded rendered RGB dimensions required")
        require(render["camera"] == -1 or (type(render["camera"]) is str and
                mujoco.mj_name2id(model, mujoco.mjtObj.mjOBJ_CAMERA, render["camera"]) >= 0),
                "select an actual named model camera or explicit derived default free camera -1")
        renderer = mujoco.Renderer(model, height=render["height"], width=render["width"])
    selected = sdk(spec["family"]) if args.mode == "dds" else None
    publisher = subscriber = None
    mailbox, failures, mutex = [], [], threading.Lock()
    if selected:
        from unitree_sdk2py.core.channel import ChannelFactoryInitialize, ChannelPublisher, ChannelSubscriber
        ChannelFactoryInitialize(spec["domain"], "lo")
        def receive(message):
            try:
                values = checked_command(selected, message)
                with mutex:
                    require(not mailbox, "duplicated or unconsumed DDS motor command")
                    mailbox.append((values, int(message.crc), time.monotonic_ns()))
            except BaseException as error:
                with mutex:
                    if not failures:
                        failures.append(repr(error))
        subscriber = ChannelSubscriber("rt/lowcmd", selected["LowCmd"])
        subscriber.Init(receive, 0)  # No hidden lossy BQueue/thread handoff.
        publisher = ChannelPublisher("rt/lowstate", selected["LowState"])
        publisher.Init()
    write_record({"kind": "ready", "protocol": "unitree-physics-v1", "mode": args.mode,
                  "robot": spec["robot"], "family": spec["family"], "actuators": model.nu,
                  "dt_ns": spec["dt_ns"], "renderer": "osmesa-rgb" if renderer else None})
    def publish_state(tick, motor_state):
        require(selected is not None, "actual native publisher required")
        state = selected["state"]()
        state.tick = tick
        if spec["family"] == "go":
            state.head[0], state.head[1], state.level_flag = 0xFE, 0xEF, 0xFF
        else:
            state.mode_machine = spec["mode_machine"]
        for index, (target, value) in enumerate(zip(state.motor_state, motor_state)):
            target.mode, target.q, target.dq, target.tau_est = int(index in slots), value["q"], value["dq"], value["tau"]
        for field, name, size in (("quaternion", spec["imu"]["quaternion"], 4),
                                  ("gyroscope", spec["imu"]["gyroscope"], 3),
                                  ("accelerometer", spec["imu"]["accelerometer"], 3)):
            setattr(state.imu_state, field, sensor(name, size))
        state.crc = selected["crc"].Crc(state)
        require(publisher.Write(state, timeout=spec["step_timeout_seconds"]), "actual LowState publish failed")
        return int(state.crc)
    def snapshot():
        # Preserve the complete actual DDS carrier. Inactive slots remain the
        # official zero default; actuator order is distinct from carrier order.
        values = [{"q": 0.0, "dq": 0.0, "tau": 0.0} for _ in range(capacity)]
        for mapping in addresses:
            values[mapping["slot"]] = {field: sensor(mapping[field], 1)[0] for field in ("q", "dq", "tau")}
        return values
    try:
        if args.quiet_only:
            require(args.mode == "dds" and not args.bootstrap, "actual command subscriber required for quiet witness")
            packet = read_record()
            exact(packet, ("op", "seconds"), "quiet native barrier")
            require(packet["op"] == "quiet" and .1 <= packet["seconds"] <= 2, "bounded quiet observation required")
            began, samples = time.monotonic_ns(), 0
            until = time.monotonic() + packet["seconds"]
            while time.monotonic() < until:
                guard()
                with mutex:
                    require(not mailbox and not failures, "denied Guest caused native command activity")
                samples += 1
                time.sleep(.001)
            write_record({"kind": "quiet", "samples": samples, "elapsed_ns": time.monotonic_ns() - began,
                          "native_dds_commands": 0, "actual_mujoco_steps": 0})
            require(read_record() == {"op": "finish"}, "explicit quiet finish required")
            write_record({"kind": "complete", "steps": 0, "actual_mujoco_steps": 0, "native_dds_commands": 0})
            return 0
        if args.bootstrap:
            require(args.mode == "dds" and read_record() == {"op": "bootstrap"}, "explicit actual initial-state barrier required")
            initial = snapshot()
            initial_crc = publish_state(1, initial)
            write_record({"kind": "bootstrap", "tick": 1, "state_crc": initial_crc,
                          "motors": initial, "actual_mujoco_steps": 0})
        for step in range(1, steps + 1):
            guard()
            packet = read_record()
            require(packet.get("op") == "step" and packet.get("step") == step, "strict current step barrier required")
            began = time.monotonic_ns()
            if args.mode == "direct":
                from messages import motors
                values = motors(packet["motors"], 20 if spec["family"] == "go" else 35)
                crc, received = None, began
            else:
                until = time.monotonic() + spec["step_timeout_seconds"]
                while True:
                    guard()
                    with mutex:
                        require(not failures, "native DDS callback failure: " + repr(failures))
                        if mailbox:
                            values, crc, received = mailbox.pop()
                            break
                    require(time.monotonic() < until, "actual DDS command deadline exhausted")
                    time.sleep(.001)
            for index, mapping in enumerate(addresses):
                motor = values[mapping["slot"]]
                data.ctrl[index] = (motor["tau"] + motor["kp"] * (motor["q"] - sensor(mapping["q"], 1)[0])
                                    + motor["kd"] * (motor["dq"] - sensor(mapping["dq"], 1)[0])) if motor["mode"] else 0
            physics_start = time.monotonic_ns()
            mujoco.mj_step(model, data)
            physics_end = time.monotonic_ns()
            qpos, qvel = [float(v) for v in data.qpos], [float(v) for v in data.qvel]
            require(all(math.isfinite(v) for v in qpos + qvel) and
                    max(map(abs, qpos + qvel), default=0) <= spec["state_abs_limit"], "nonfinite or unbounded actual physics state")
            motor_state = snapshot()
            require(all(math.isfinite(value) and abs(value) <= spec["state_abs_limit"]
                        for motor in motor_state for value in motor.values()), "actual motor sensors are nonfinite/unbounded")
            require(abs(float(data.time) - initial_time - step * model.opt.timestep) <= 1E-8,
                    "actual MuJoCo time does not equal exactly one selected step per barrier")
            state_crc = None
            if selected:
                state_crc = publish_state(step + int(args.bootstrap), motor_state)
            rendered = None
            if renderer:
                renderer.update_scene(data, camera=spec["render"]["camera"])
                image = renderer.render()
                raw = image.tobytes(order="C")
                width, height = spec["render"]["width"], spec["render"]["height"]
                require(image.shape == (height, width, 3) and len(raw) == width * height * 3, "actual RGB image shape differs")
                rendered = {"width": width, "height": height, "encoding": "rgb8", "step": width * 3,
                            "camera": spec["render"]["camera"], "camera_scope": "derived-free-camera" if spec["render"]["camera"] == -1 else "named-model-camera",
                            "sha256": hashlib.sha256(raw).hexdigest(), "rgb_base64": base64.b64encode(raw).decode("ascii")}
            write_record({"kind": "state", "step": step, "tick": step + int(args.bootstrap), "simulation_time": float(data.time),
                "qpos": qpos, "qvel": qvel, "motors": motor_state, "applied_command": values,
                "command_crc": crc, "state_crc": state_crc,
                "command_received_monotonic_ns": received, "barrier_monotonic_ns": began,
                "physics_ns": physics_end - physics_start, "rendered": rendered})
        require(read_record() == {"op": "finish"}, "explicit final barrier required")
        with mutex:
            require(not mailbox and not failures, "late/duplicate command after final step")
        write_record({"kind": "complete", "steps": steps, "actual_mujoco_steps": steps,
                      "mode": args.mode, "native_dds_commands": steps if selected else 0})
    finally:
        if subscriber:
            subscriber.Close()
        if publisher:
            publisher.Close()
        if renderer:
            renderer.close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
