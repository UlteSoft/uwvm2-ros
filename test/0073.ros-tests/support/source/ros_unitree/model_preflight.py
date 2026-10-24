#!/usr/bin/env python3
"""Observe actual MuJoCo model mappings and bounded zero-control physics.

Run only through the Root-owned serial simulation view with its exact frozen
manifest, complete package/model inventories and current 64-GiB/swap0 cgroup.
The actor never imports SDK2, initializes DDS/ROS, opens a viewer, downloads an
asset or selects a model from a robot name. An explicit motor_sensors map is a
candidate until actual loaded IDs, transmission joints and sensor objects are
checked below. Zero-control steps qualify model/physics plumbing only; they
are not a standing, balance, walking, native/Wasm or physical-robot result.

There is no stdin protocol. Records appear in this order: loaded mapping and
initial full numeric snapshot, one full snapshot per actual step, optional
bounded offscreen RGB, then one terminal model-preflight-complete record. A
missing terminal record or nonzero process status is a failure, regardless of
earlier records. Parent deadlines and owned pidfds remain authoritative.
"""
import argparse
import base64
import hashlib
import json
import math
import os
import sys

from common import child_arguments, exact, finite, require, write_record
from preflight_support import admit, mapped_native_file, official_file, package_files, preserve


def numbers(values, bound, label):
    """Copy bounded actual numeric arrays before emitting strict JSON."""
    require(len(values) <= 16384, "preflight numeric array exceeds bound: " + label)
    result = [float(value) for value in values]
    require(all(math.isfinite(value) and abs(value) <= bound for value in result),
            "nonfinite or out-of-bound actual numeric data: " + label)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    child_arguments(parser)
    parser.add_argument("--steps", required=True, type=int)
    parser.add_argument("--render", action="store_true")
    args = parser.parse_args()
    require(1 <= args.steps <= 32, "explicit finite one-to-32 actual steps required")
    config, guard, pins, roots = admit(args, "model_preflight.py")
    mappings = config["native_mappings"]
    require(type(mappings) is list and 1 <= len(mappings) <= 16 and
            all(type(label) is str and label in pins for label in mappings) and len(set(mappings)) == len(mappings),
            "explicit distinct selected native mapping input labels required")
    spec = config["simulation"]
    require(type(spec) is dict and spec["family"] in ("go", "hg"), "explicit simulation family required")
    require(spec["model_input"] in pins and spec["model_upstream"] in config["upstreams"],
            "explicit admitted official model input/upstream required")
    from pathlib import Path
    upstream = config["upstreams"][spec["model_upstream"]]
    model_path = Path(pins[spec["model_input"]]["path"])
    require(model_path.is_relative_to(Path(upstream["root"]).resolve(strict=True)),
            "selected model is outside the admitted official upstream")
    model_source = official_file(config, pins, spec["model_upstream"], model_path)
    capacity = 20 if spec["family"] == "go" else 35
    require(type(spec["motor_sensors"]) is list and 1 <= len(spec["motor_sensors"]) <= capacity,
            "complete bounded explicit motor map required")
    bound = finite(spec["state_abs_limit"], 1, 1E6, "preflight state absolute bound")
    nanos = spec["dt_ns"]
    require(type(nanos) is int and 100000 <= nanos <= 20000000, "bounded integer timestep required")
    guard()
    import mujoco
    import numpy
    model = mujoco.MjModel.from_xml_path(str(model_path))
    require(model.nu == len(spec["motor_sensors"]) and 0 < model.nq <= 4096 and
            0 < model.nv <= 4096 and 0 <= model.nsensordata <= 16384 and
            0 <= model.nsensor <= 4096 and 0 <= model.nkey <= 64 and 0 < model.njnt <= 4096,
            "loaded model dimensions differ from finite admitted fixture")
    model.opt.timestep = nanos / 1E9
    data = mujoco.MjData(model)

    def name(kind, index):
        value = mujoco.mj_id2name(model, kind, int(index))
        require(value is not None and mujoco.mj_name2id(model, kind, value) == int(index),
                "actual object name/id round trip failed")
        return value

    def sensor(selected, size, sensor_type, objtype, objid):
        require(type(selected) is str and selected, "explicit sensor name required")
        index = mujoco.mj_name2id(model, mujoco.mjtObj.mjOBJ_SENSOR, selected)
        require(index >= 0 and name(mujoco.mjtObj.mjOBJ_SENSOR, index) == selected,
                "actual named sensor absent: " + selected)
        address, dimension = int(model.sensor_adr[index]), int(model.sensor_dim[index])
        require(dimension == size and 0 <= address <= model.nsensordata - dimension and
                int(model.sensor_type[index]) == int(sensor_type) and
                int(model.sensor_objtype[index]) == int(objtype) and int(model.sensor_objid[index]) == objid,
                "actual sensor kind/object/dimension/address differs: " + selected)
        return {"name": selected, "id": index, "address": address, "dimension": dimension,
                "type": int(model.sensor_type[index]), "object_type": int(model.sensor_objtype[index]),
                "object_id": int(model.sensor_objid[index]), "reference_type": int(model.sensor_reftype[index]),
                "reference_id": int(model.sensor_refid[index]), "datatype": int(model.sensor_datatype[index])}

    rows, used_slots, used_joints, used_sensors = [], set(), set(), set()
    for index, selected in enumerate(spec["motor_sensors"]):
        guard()
        exact(selected, ("actuator", "slot", "q", "dq", "tau"), "candidate motor map")
        slot = selected["slot"]
        require(type(slot) is int and 0 <= slot < capacity and slot not in used_slots,
                "unique explicit carrier slot required")
        actuator = mujoco.mj_name2id(model, mujoco.mjtObj.mjOBJ_ACTUATOR, selected["actuator"])
        require(actuator == index and name(mujoco.mjtObj.mjOBJ_ACTUATOR, actuator) == selected["actuator"] and
                int(model.actuator_trntype[actuator]) == int(mujoco.mjtTrn.mjTRN_JOINT),
                "actual ordered actuator is not the selected joint transmission")
        joint = int(model.actuator_trnid[actuator][0])
        require(0 <= joint < model.njnt and joint not in used_joints and
                int(model.jnt_type[joint]) in (int(mujoco.mjtJoint.mjJNT_HINGE), int(mujoco.mjtJoint.mjJNT_SLIDE)),
                "fixture requires one unique scalar physical joint per actuator")
        qadr, vadr = int(model.jnt_qposadr[joint]), int(model.jnt_dofadr[joint])
        require(0 <= qadr < model.nq and 0 <= vadr < model.nv, "actual joint address outside MjData")
        sensors = {field: sensor(selected[field], 1, kind, mujoco.mjtObj.mjOBJ_JOINT, joint)
                   for field, kind in (("q", mujoco.mjtSensor.mjSENS_JOINTPOS),
                                       ("dq", mujoco.mjtSensor.mjSENS_JOINTVEL),
                                       ("tau", mujoco.mjtSensor.mjSENS_JOINTACTFRC))}
        ids = {value["id"] for value in sensors.values()}
        require(len(ids) == 3 and not used_sensors.intersection(ids), "motor sensor IDs must be distinct")
        used_slots.add(slot); used_joints.add(joint); used_sensors.update(ids)
        rows.append({"actuator_id": actuator, "actuator": selected["actuator"], "carrier_slot": slot,
                     "transmission_type": int(model.actuator_trntype[actuator]),
                     "transmission_ids": [int(value) for value in model.actuator_trnid[actuator]],
                     "joint_id": joint, "joint": name(mujoco.mjtObj.mjOBJ_JOINT, joint),
                     "joint_type": int(model.jnt_type[joint]), "qpos_address": qadr, "dof_address": vadr,
                     "gear": numbers(model.actuator_gear[actuator], 1E12, "actuator gear"),
                     "control_limited": bool(model.actuator_ctrllimited[actuator]),
                     "control_range": numbers(model.actuator_ctrlrange[actuator], 1E12, "control range"),
                     "sensors": sensors})
    exact(spec["imu"], ("quaternion", "gyroscope", "accelerometer"), "candidate IMU")
    first_imu = mujoco.mj_name2id(model, mujoco.mjtObj.mjOBJ_SENSOR, spec["imu"]["quaternion"])
    require(first_imu >= 0, "actual selected IMU quaternion sensor absent")
    site = int(model.sensor_objid[first_imu])
    require(0 <= site < model.nsite, "actual selected IMU site outside model")
    imu = {field: sensor(spec["imu"][field], size, kind, mujoco.mjtObj.mjOBJ_SITE, site)
           for field, size, kind in (("quaternion", 4, mujoco.mjtSensor.mjSENS_FRAMEQUAT),
                                     ("gyroscope", 3, mujoco.mjtSensor.mjSENS_GYRO),
                                     ("accelerometer", 3, mujoco.mjtSensor.mjSENS_ACCELEROMETER))}
    require(not used_sensors.intersection(value["id"] for value in imu.values()),
            "IMU must not alias motor sensor IDs")
    keyframes = []
    for index in range(model.nkey):
        keyframes.append({"id": index, "name": name(mujoco.mjtObj.mjOBJ_KEY, index),
                          "time": finite(float(model.key_time[index]), -1E6, 1E6, "key time"),
                          "qpos": numbers(model.key_qpos[index], bound, "key qpos"),
                          "qvel": numbers(model.key_qvel[index], bound, "key qvel"),
                          "act": numbers(model.key_act[index], bound, "key act"),
                          "ctrl": numbers(model.key_ctrl[index], bound, "key ctrl"),
                          "mocap_position": numbers(model.key_mpos[index].reshape(-1), bound, "key mocap position"),
                          "mocap_quaternion": numbers(model.key_mquat[index].reshape(-1), bound, "key mocap quaternion")})
    # Retain every actual joint and sensor, including additional IMUs and
    # unnamed free joints. Only the selected motor/IMU map above is promoted to
    # a carrier assignment; unrelated sensors are never silently substituted.
    joints, all_sensors = [], []
    for index in range(model.njnt):
        joints.append({"id": index, "name": mujoco.mj_id2name(model, mujoco.mjtObj.mjOBJ_JOINT, index),
                       "type": int(model.jnt_type[index]), "body_id": int(model.jnt_bodyid[index]),
                       "qpos_address": int(model.jnt_qposadr[index]), "dof_address": int(model.jnt_dofadr[index]),
                       "limited": bool(model.jnt_limited[index]),
                       "range": numbers(model.jnt_range[index], 1E12, "joint range")})
    for index in range(model.nsensor):
        address, dimension = int(model.sensor_adr[index]), int(model.sensor_dim[index])
        require(dimension > 0 and 0 <= address <= model.nsensordata - dimension,
                "actual sensor table address/dimension outside sensordata")
        all_sensors.append({"id": index, "name": mujoco.mj_id2name(model, mujoco.mjtObj.mjOBJ_SENSOR, index),
                            "type": int(model.sensor_type[index]), "address": address, "dimension": dimension,
                            "object_type": int(model.sensor_objtype[index]), "object_id": int(model.sensor_objid[index]),
                            "reference_type": int(model.sensor_reftype[index]), "reference_id": int(model.sensor_refid[index]),
                            "datatype": int(model.sensor_datatype[index])})
    selected_key = None
    if spec["keyframe"] is not None:
        selected_key = mujoco.mj_name2id(model, mujoco.mjtObj.mjOBJ_KEY, spec["keyframe"])
        require(selected_key >= 0, "explicit selected keyframe missing from loaded model")
        mujoco.mj_resetDataKeyframe(model, data, selected_key)
    data.ctrl[:] = 0
    mujoco.mj_forward(model, data)
    initial_time = float(data.time)
    state_spec = mujoco.mjtState.mjSTATE_FULLPHYSICS
    state_size = mujoco.mj_stateSize(model, state_spec)
    require(0 < state_size <= 16384, "bounded actual FULLPHYSICS state size required")
    state_buffer = numpy.empty(state_size, dtype=numpy.float64)

    def snapshot(step):
        values = {key: numbers(getattr(data, key), bound, key)
                  for key in ("qpos", "qvel", "ctrl", "act", "sensordata")}
        mujoco.mj_getState(model, data, state_buffer, state_spec)
        values.update(fullphysics_state=numbers(state_buffer, bound, "actual FULLPHYSICS state"),
                      fullphysics_state_spec=int(state_spec))
        values.update(time=finite(float(data.time), -1E6, 1E6, "actual simulation time"), step=step)
        encoded = json.dumps(values, allow_nan=False, sort_keys=True, separators=(",", ":")).encode()
        values["numeric_json_sha256"] = hashlib.sha256(encoded).hexdigest()
        return values

    write_record({"kind": "model-preflight-loaded", "protocol": "unitree-model-preflight-v1",
                  "scope": "Actual model mapping and zero-control physics only; no DDS/ROS/controller qualification",
                  "robot": spec["robot"], "family": spec["family"], "model": pins[spec["model_input"]],
                  "upstream": upstream, "model_source": model_source, "mujoco_package_version": mujoco.__version__,
                  "mujoco_native_version": mujoco.mj_versionString(), "dt_ns": nanos,
                  "dimensions": {key: int(getattr(model, key)) for key in
                                 ("nq", "nv", "nu", "na", "njnt", "nsensor", "nsensordata", "nkey")},
                  "motor_map": rows, "all_joints": joints, "all_sensors": all_sensors,
                  "imu_site": {"id": site, "name": name(mujoco.mjtObj.mjOBJ_SITE, site)},
                  "imu": imu, "keyframes": keyframes, "selected_keyframe_id": selected_key,
                  "snapshot": snapshot(0)})
    for step in range(1, args.steps + 1):
        guard(); data.ctrl[:] = 0
        mujoco.mj_step(model, data)
        # Recompute sensors/derived positions at the exact resulting state.
        mujoco.mj_forward(model, data)
        require(abs(float(data.time) - initial_time - step * model.opt.timestep) <= 1E-8,
                "actual fixed-step time differs from its finite step count")
        write_record({"kind": "model-preflight-step", "snapshot": snapshot(step)})
    rendered = False
    if args.render:
        render = spec["render"]
        require(type(render) is dict and os.environ.get("MUJOCO_GL") == "osmesa",
                "explicit selected CPU OSMesa rendering required")
        width, height = render["width"], render["height"]
        require(type(width) is int and type(height) is int and 16 <= width <= 320 and 16 <= height <= 240,
                "bounded explicit RGB dimensions required")
        camera = render["camera"]
        require(camera == -1 or (type(camera) is str and
                mujoco.mj_name2id(model, mujoco.mjtObj.mjOBJ_CAMERA, camera) >= 0), "actual camera selection required")
        guard()
        with mujoco.Renderer(model, height=height, width=width) as renderer:
            renderer.update_scene(data, camera=camera); image = renderer.render()
            raw = image.tobytes(order="C")
            require(image.shape == (height, width, 3) and len(raw) == width * height * 3,
                    "actual rendered RGB dimensions differ")
            write_record({"kind": "model-preflight-render", "renderer": "osmesa", "camera": camera,
                          "width": width, "height": height, "encoding": "rgb8",
                          "rgb_base64": base64.b64encode(raw).decode("ascii"), "sha256": hashlib.sha256(raw).hexdigest()})
        rendered = True
    require(not any(key == "rclpy" or key == "cyclonedds" or key.startswith(("rclpy.", "cyclonedds.", "unitree_sdk2py."))
                    for key in sys.modules), "noDDS model preflight unexpectedly loaded ROS/SDK2/DDS")
    modules = package_files(("mujoco", "numpy", "OpenGL"), roots)
    # These are exact Root-selected physical library inputs, not inferred
    # package names. Rendering cells select the actual OSMesa DSO in addition
    # to wheel libmujoco; observe after the real context/render has been used.
    native_mappings = [{"input": label, "physical_pin": pins[label],
                        "opened_mapping": mapped_native_file(args, pins[label], guard)} for label in mappings]
    preserve(pins, guard)
    write_record({"kind": "model-preflight-complete", "status": "PASS_MODEL_PHYSICS_PREFLIGHT_ONLY",
                  "actual_mujoco_steps": args.steps, "dds_initialized": False, "ros_initialized": False,
                  "rendered": rendered, "module_files": modules, "native_mappings": native_mappings,
                  "selected_inputs_preserved": True})
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
