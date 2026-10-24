#!/usr/bin/env python3
"""Author separate ROS1/ROS2 envelopes for the public Aurora client methods.

This is an authored native SDK bridge, explicitly distinct from an official
Fourier ROS driver or native DDS schema. Original public method documentation,
our exact specification and our model-independent source remain separately
identifiable. Host lifecycle methods never enter the Guest protocol catalog.
No vendor SDK, ROS process, compiler or Guest executes in this authoring phase.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', required=True, type=Path)
    p.add_argument('--out', required=True, type=Path)
    a = p.parse_args()
    inputs = json.loads((a.root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    specification = source / 'sdk/origin/robots/platforms/fourier/bridge_interfaces/specification.py'
    spec = importlib.util.spec_from_file_location('fourier_authored_bridge_specification', specification)
    api = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(api)
    capture = Path(inputs['remaining_capture'])
    original = capture / 'upstream/fourier/main'
    original_lock = json.loads((original / 'source-lock.json').read_bytes())
    document = original / 'files/python/docs/API_document_EN.md'
    doc = document.read_text()
    for operation, request, response in api.SERVICES.values():
        assert operation == 'snapshot' or ('### ' + operation + '\n') in doc
    a.out.mkdir(mode=0o700)
    output_capture = a.out / 'capture'
    shutil.copytree(capture, output_capture)
    directory = output_capture / 'upstream/fourier_bridge/aurora_bridge_v1'
    files = directory / 'files'
    rows = []
    def write(name, body):
        raw = body.encode()
        path = files / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(raw)
        rows.append(dict(path=name, bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest(),
                         source_kind='AUTHORED_OPERATOR_NATIVE_BRIDGE_V1'))
    for version in (1, 2):
        prefix = 'ros' + str(version) + '/uwvm_fourier_bridge/'
        for name, body in api.MESSAGES.items():
            write(prefix + 'msg/' + name + '.msg', body)
        for name, (operation, request, response) in api.SERVICES.items():
            body = '# Authored bridge envelope for AuroraClient.' + operation + '.\n'
            body += '# Increasing per-principal sequence; no automatic retry after an unknown result.\n'
            body += 'uint64 sequence\n' + request + '---\nuint32 status\nstring error\n' + response
            write(prefix + 'srv/' + name + '.srv', body)
        xml = '<?xml version="1.0"?>\n<package format="' + ('2' if version == 1 else '3') + '">\n<name>uwvm_fourier_bridge</name>\n<version>0.1.0</version>\n'
        xml += '<description>Explicit operator-owned Aurora native SDK business-method bridge. Not a vendor ROS driver.</description>\n'
        xml += '<maintainer email="source-owner@example.invalid">Explicit source owner</maintainer>\n<license>Apache-2.0</license>\n'
        if version == 1:
            xml += '<buildtool_depend>catkin</buildtool_depend>\n<build_depend>message_generation</build_depend>\n<exec_depend>message_runtime</exec_depend>\n<exec_depend>rospy</exec_depend>\n'
            cmake = 'cmake_minimum_required(VERSION 3.10)\nproject(uwvm_fourier_bridge)\nfind_package(catkin REQUIRED COMPONENTS message_generation rospy)\n'
            cmake += 'add_message_files(FILES ' + ' '.join(n + '.msg' for n in api.MESSAGES) + ')\n'
            cmake += 'add_service_files(FILES ' + ' '.join(n + '.srv' for n in api.SERVICES) + ')\n'
            cmake += 'generate_messages()\ncatkin_package(CATKIN_DEPENDS message_runtime rospy)\n'
        else:
            xml += '<buildtool_depend>ament_cmake</buildtool_depend>\n<build_depend>rosidl_default_generators</build_depend>\n<exec_depend>rosidl_default_runtime</exec_depend>\n<exec_depend>rclpy</exec_depend>\n<member_of_group>rosidl_interface_packages</member_of_group>\n'
            xml += '<export><build_type>ament_cmake</build_type></export>\n'
            cmake = 'cmake_minimum_required(VERSION 3.10)\nproject(uwvm_fourier_bridge)\nfind_package(ament_cmake REQUIRED)\nfind_package(rosidl_default_generators REQUIRED)\n'
            cmake += 'rosidl_generate_interfaces(${PROJECT_NAME}\n' + ''.join('  "msg/' + n + '.msg"\n' for n in api.MESSAGES)
            cmake += ''.join('  "srv/' + n + '.srv"\n' for n in api.SERVICES) + ')\nament_export_dependencies(rosidl_default_runtime)\nament_package()\n'
        write(prefix + 'package.xml', xml + '</package>\n')
        write(prefix + 'CMakeLists.txt', '# Authored source package; building it starts no bridge or robot.\n' + cmake)
    proof = dict(id='aurora_bridge_v1', branch='aurora_bridge_v1', repository='AUTHORED_FOURIER_NATIVE_SDK_ROS_BRIDGE',
                 commit=None, tree=None, source_kind='AUTHORED_OPERATOR_NATIVE_BRIDGE_V1', files=rows,
                 source_proof=dict(specification_sha256=hashlib.sha256(specification.read_bytes()).hexdigest(),
                     upstream_repository=original_lock['repository'], upstream_commit=original_lock['commit'],
                     upstream_tree=original_lock['tree'], public_api_document_sha256=hashlib.sha256(document.read_bytes()).hexdigest(),
                     original_sdk_protocol_emulated=False, host_only_lifecycle=api.LIFECYCLE),
                 default_authority=[], original_official_cases_run=0, native_driver_qualified=False)
    (directory / 'source-lock.json').write_text(json.dumps(proof, indent=2) + '\n')
    report = dict(status='AUTHORED_ROS1_ROS2_NATIVE_SDK_BRIDGE_ENVELOPES_DISTINCT_FROM_VENDOR_DRIVER',
                  messages=len(api.MESSAGES), services=len(api.SERVICES), source_capture=str(output_capture),
                  default_authority=[], native_or_guest_executions=0)
    (a.out / 'bridge-source.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
