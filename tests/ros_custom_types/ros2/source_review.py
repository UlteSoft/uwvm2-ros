#!/usr/bin/env python3
"""Review prepared ROS2 custom-topic source and the independent fixed wire model.

This script performs text/XML/source-model checks only. It never invokes CMake,
a compiler, a native executable, a Guest or ROS. Its result cannot establish
native API compatibility, DDS behavior, QoS admission, or sandbox acceptance.
Write fresh reports so earlier failed or successful evidence remains immutable.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import math
from pathlib import Path
import struct
import xml.etree.ElementTree as ET


def review(root: Path) -> dict:
    """Bind reviewed inputs and report limited, independently derived checks."""
    folder = root / 'tests/ros_custom_types/ros2'
    files = [root / 'gateways/ros2/custom_topics.h', root / 'gateways/ros2/main.cc',
             root / 'gateways/ros2/custom_topics_impl.h', root / 'gateways/ros2/custom_topics_dispatch.cc',
             root / 'gateways/ros2/custom_topics_shard.cc.in', root / 'gateways/ros2/custom_topics.cmake',
             root / 'gateways/ros2/custom_topics_build.md',
             root / 'gateways/ros2/CMakeLists.txt', folder / 'native_topics.cc',
             folder / 'CMakeLists.txt', folder / 'catalog.preparation.json',
             folder / 'interfaces/msg/FixedSample.msg', folder / 'interfaces/CMakeLists.txt',
             folder / 'interfaces/package.xml', folder / 'README.md', Path(__file__).resolve()]
    inputs = {str(path.relative_to(root)): path.read_bytes() for path in files}
    text = {name: data.decode('utf-8') for name, data in inputs.items()}
    checks = []

    def check(name: str, condition: bool) -> None:
        checks.append({'name': name, 'pass': bool(condition)})

    header = text['gateways/ros2/custom_topics.h']
    implementation = text['gateways/ros2/custom_topics_impl.h']
    check('paired build-selected native and common providers',
          '#if defined(UWVM_ROS_CUSTOM_ROS2_HEADER) != defined(UWVM_ROS_CUSTOM_TOPIC_MANIFEST)' in header)
    check('orphan externally defined native table rejected',
          '#if defined(UWVM_ROS_CUSTOM_ROS2_TYPES)\n#error' in header)
    check('empty default native table', '#define UWVM_ROS_CUSTOM_ROS2_TYPES(X)\n#endif' in header)
    check('fixed generated native message required',
          'rosidl_generator_traits::is_message<Native>::value' in header and
          'rosidl_generator_traits::has_fixed_size<Native>::value' in header)
    check('independent codec identity required', 'cap.limits.find("adapter-codec-identity")' in header)
    check('official type name and native hash checked',
          'rosidl_generator_traits::name<Native>()' in header and 'hash->version!=1' in header and
          'native_io::matches_hexadecimal' in header)
    check('official description name checked with complete type header',
          'type_description/type_description__struct.h' in header and
          'native_text(actual.type_name,expected.type)' in header)
    check('exactly one native row qualifies', 'return matches==1&&qualified;' in header)
    check('complete generated native schema compared before admission',
          'matches_native_schema(*description,Codec::native_schema(),name)' in header and
          'actual.referenced_type_descriptions.size!=expected.record_count-1' in header)
    check('native field order type bounds nested names and defaults compared',
          all(token in header for token in ['actual.fields.data[i]', 'native.type.type_id!=type',
              'native.type.capacity!=(field.is_array?field.array_size:field.is_sequence?field.sequence_bound:0)',
              'native.type.string_capacity!=field.string_bound',
              'native.default_value.data', 'native.type.nested_type_name']))
    check('native schema cycles orphan definitions and alternate deep paths refused',
          'colors[record]==1' in header and 'if(colors[i]!=2)' in header and
          'depth+heights[record]-1<=16' in header)
    callback = implementation[implementation.index('auto callback{[weak,verified,descriptor]'):implementation.index('auto configured{ros2_qos::configured(cap)};', implementation.index('auto callback{[weak,verified,descriptor]'))]
    check('native finite validation precedes canonical allocation',
          callback.index('!Codec::valid(*message)') < callback.index('std::vector<std::uint8_t> data'))
    check('encoded bytes receive common structural scan',
          'Codec::encode(*message,data)' in callback and
          'ipc::validate_custom_topic(*descriptor,data.data(),data.size())' in callback)
    check('actual native QoS and exact resource admission in both directions',
          implementation.count('ros2_qos::entity(*entity,configured)') == 2 and
          implementation.count('actual_name&&std::string_view{actual_name}==cap.resource') == 2)
    check('callback activates only after retained admitted entity',
          implementation.index('subscribers_.emplace(cap.alias') < implementation.index('verified->store(true,std::memory_order_release)'))
    check('retirement disables callbacks and queue',
          'found->second.verified->store(false,std::memory_order_release)' in header and
          'found->second.queue.lock()) { queue->deactivate(); }' in header)
    main = text['gateways/ros2/main.cc']
    check('main has custom native startup publish subscribe and close hooks',
          all(token in main for token in ['ros2_custom_topics::known(c)', 'custom_topics_.publish(c,d)',
                                         'custom_topics_.subscribe(c,std::move(q))', 'custom_topics_.close(alias)']))
    cmake = text['gateways/ros2/CMakeLists.txt']
    check('native packages explicit and registry default empty',
          'set(UWVM_ROS_CUSTOM_REGISTRY_HEADER "" CACHE' in cmake and
          'foreach(custom_package IN LISTS UWVM_ROS_CUSTOM_ROS2_PACKAGES)' in cmake)
    package = ET.fromstring(text['tests/ros_custom_types/ros2/interfaces/package.xml'])
    check('official fixture interface generation', package.findtext('name') == 'uwvm_ros_custom_test_msgs' and
          'rosidl_generate_interfaces(${PROJECT_NAME} "msg/FixedSample.msg")' in
          text['tests/ros_custom_types/ros2/interfaces/CMakeLists.txt'])
    catalog = json.loads(text['tests/ros_custom_types/ros2/catalog.preparation.json'])
    check('preparation has no invented native identity', catalog == {
        'format': 'uwvm-ros-custom-fixed-v1', 'types': [{'ros_version': 2,
        'type': 'uwvm_ros_custom_test_msgs/msg/FixedSample', 'file': 'interfaces/msg/FixedSample.msg'}]})
    fields = [line.split('#', 1)[0].strip() for line in
              text['tests/ros_custom_types/ros2/interfaces/msg/FixedSample.msg'].splitlines()]
    fields = [line.split() for line in fields if line]
    check('official sample fixed layout independently matched', fields == [
        ['uint32', 'sequence'], ['bool', 'enabled'], ['float32', 'temperature'],
        ['float64[3]', 'acceleration'], ['uint8[8]', 'tag'], ['int16[2]', 'thresholds']])
    golden = struct.pack('<IBf3d8B2h', 0x12345678, 1, 21.25, 1.5, -2.25, 3.125,
                         0, 1, 2, 127, 128, 253, 254, 255, -32768, 32767)
    check('independent canonical golden length and primitive offsets',
          len(golden) == 45 and golden[:5].hex() == '7856341201' and
          struct.unpack_from('<f', golden, 5)[0] == 21.25 and
          struct.unpack_from('<d', golden, 9)[0] == 1.5)

    def valid(data: bytes) -> bool:
        return (len(data) == 45 and data[4] <= 1 and math.isfinite(struct.unpack_from('<f', data, 5)[0]) and
                all(math.isfinite(value) for value in struct.unpack_from('<3d', data, 9)))

    check('independent golden scanner accepts finite primitive record', valid(golden))
    check('independent scanner rejects truncated and trailing records', not valid(golden[:-1]) and not valid(golden + b'\0'))
    bad = bytearray(golden);bad[4] = 2
    check('independent scanner rejects noncanonical bool', not valid(bytes(bad)))
    bad = bytearray(golden);struct.pack_into('<I', bad, 5, 0x7f800000)
    check('independent scanner rejects infinite float', not valid(bytes(bad)))
    bad = bytearray(golden);struct.pack_into('<Q', bad, 9, 0x7ff8000000000001)
    check('independent scanner rejects NaN double', not valid(bytes(bad)))
    fixture = text['tests/ros_custom_types/ros2/native_topics.cc']
    check('prepared native fixture observes actual malformed-publish side effects',
          'observed==before' in fixture and 'peer->get_topic_names_and_types().count(changed.resource)' in fixture)
    check('prepared native fixture checks both custom directions and retirement',
          all(token in fixture for token in ['received==expected', 'event.payload==bytes',
                                           'queue->poll().error==125', 'nonfinite.acceleration[1]']))
    check('prepared native nonfinite ingress requires independent DDS witness',
          fixture.count('invalid_native_observed>invalid_before') == 2 and
          'if(!Codec::valid(*message)) { ++invalid_native_observed; }' in fixture)
    return {'status': 'SOURCE_AND_MODEL_PASS' if all(row['pass'] for row in checks) else 'FAIL',
            'scope': 'text/XML/source-model checks; no compilation or native/Guest/ROS execution',
            'actual_behavior_status': 'NOT_EXECUTED', 'checks': checks, 'checks_count': len(checks),
            'independent_golden_hex': golden.hex(),
            'inputs': [{'path': name, 'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest()}
                       for name, data in inputs.items()]}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, required=True)
    arguments = parser.parse_args()
    root = Path(__file__).resolve().parents[3]
    report = review(root)
    with arguments.out.open('x', encoding='utf-8') as output:
        json.dump(report, output, indent=2);output.write('\n')
    print(f"{report['status']} ROS2 custom source/models checks={report['checks_count']}; actual behavior NOT_EXECUTED")
    return 0 if report['status'] == 'SOURCE_AND_MODEL_PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
