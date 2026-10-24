"""Complete real Python record contract for an isolated frozen WASI command.

This module imports actual generated values and SDK codecs. It creates no
Backend, native identity, transport model or ROS resource. Run its main only
inside the real selected CPython/WASI interpreter for Guest qualification.
"""
import struct
import sys
import bounded_records as records
import joint_ros1_records
import joint_ros2_records

checks = 0


def need(value):
    global checks
    checks += 1
    if not value:
        raise AssertionError('complete Python record contract')


def refuses(operation):
    refused = False
    try:
        operation()
    except (TypeError, ValueError, OverflowError, UnicodeError):
        refused = True
    need(refused)


def scalar():
    return records.Ros2Managed_fixtureMsgScalars(
        flag=True, u8=255, i8=-128, u16=65535, i16=-32768,
        u32=4294967295, i32=-2147483648, u64=18446744073709551615,
        i64=-9223372036854775808, f32=-0.0, f64=1.25, text='π')


def complete_scalar_golden():
    expected = bytes.fromhex('01ff80ffff0080ffffffff00000080ffffffffffffffff'
                             '000000000000008000000080000000000000f43f02000000cf80')
    need(scalar().encode() == expected)
    need(expected == struct.pack('<BBbHhIiQqfdI', 1, 255, -128, 65535, -32768,
                                4294967295, -2147483648, 18446744073709551615,
                                -9223372036854775808, -0.0, 1.25, 2) + b'\xcf\x80')
    value = records.Ros2Managed_fixtureMsgScalars.decode(expected)
    need(value.u64 == 18446744073709551615 and value.i64 == -9223372036854775808)
    need(value.u32 == 4294967295 and value.i32 == -2147483648)
    need(struct.pack('<f', value.f32) == b'\0\0\0\x80')


def signed_width_bits():
    value = scalar()
    value.i8 = value.i16 = value.i32 = value.i64 = -1
    wire = value.encode()
    result = type(value).decode(wire)
    need((result.i8, result.i16, result.i32, result.i64) == (-1, -1, -1, -1))
    need(wire[2:3] == b'\xff' and wire[5:7] == b'\xff\xff')
    need(wire[11:15] == b'\xff' * 4 and wire[23:31] == b'\xff' * 8)


def complete_unsigned64():
    value = scalar()
    value.u64 = 1 << 63
    need(type(value).decode(value.encode()).u64 == 1 << 63)
    value.u64 = (1 << 64) - 1
    need(type(value).decode(value.encode()).u64 == (1 << 64) - 1)


def exact_integer_types_and_widths():
    for name, invalid in (('u8', 256), ('u16', 65536), ('u32', 1 << 32),
                          ('u64', -1), ('i8', -129), ('i16', 32768),
                          ('i32', 1 << 31), ('i64', 1 << 63),
                          ('flag', 1), ('u8', True)):
        value = scalar()
        setattr(value, name, invalid)
        refuses(value.encode)


def finite_and_signed_zero():
    for name, invalid in (('f32', float('inf')), ('f32', 1e40), ('f64', float('nan'))):
        value = scalar()
        setattr(value, name, invalid)
        refuses(value.encode)
    value = scalar()
    value.f64 = -0.0
    need(struct.pack('<d', type(value).decode(value.encode()).f64) == b'\0' * 7 + b'\x80')


def strict_text():
    value = scalar()
    value.text = '🙂' * 4
    need(type(value).decode(value.encode()).text == value.text)
    for invalid in ('🙂' * 5, 'bad\0text', '\ud800'):
        value.text = invalid
        refuses(value.encode)


def exact_decode():
    cls = records.Ros2Managed_fixtureMsgScalars
    wire = scalar().encode()
    for invalid in (wire[:-1], wire + b'\0', b'\2' + wire[1:], wire[:-2] + b'\xc0\x80'):
        refuses(lambda: cls.decode(invalid))


def nested_arrays():
    value = records.Ros2Managed_fixtureMsgOuter()
    value.inner.scalar = scalar()
    value.fixed[0].tags = ['first', 'π']
    value.fixed[1].pair = [3.0, -2.0]
    value.items = [records.Ros2Managed_fixtureMsgInner()]
    value.items[0].scalar.i64 = -7
    result = type(value).decode(value.encode())
    need(result.inner.scalar.u64 == (1 << 64) - 1)
    need(result.fixed[1].pair == [3.0, -2.0])
    need(result.fixed[0].tags == ['first', 'π'])
    need(result.items[0].scalar.i64 == -7)


def array_bounds():
    value = records.Ros2Managed_fixtureMsgOuter()
    value.fixed = []
    refuses(value.encode)
    value = records.Ros2Managed_fixtureMsgOuter()
    value.items = [records.Ros2Managed_fixtureMsgInner() for _ in range(3)]
    refuses(value.encode)
    value.items = []
    value.inner.tags[0] = '🙂' * 3
    refuses(value.encode)
    value.inner.tags[0] = ''
    value.fixed[1] = None
    refuses(value.encode)


def independent_defaults():
    first, second = records.Ros2Managed_fixtureMsgOuter(), records.Ros2Managed_fixtureMsgOuter()
    need(first.rotation.w == 1.0 and first.pose.orientation.w == 1.0)
    need([v.w for v in first.rotations] == [1.0, 1.0])
    need(first.dynamic == [] and first.items == [])
    first.rotations[0].w = 0.0
    first.fixed[0].pair[0] = 9.0
    need(first.rotations[1].w == 1.0 and second.rotations[0].w == 1.0)
    need(second.fixed[0].pair[0] == 0.0)


def no_missing_default_wire():
    value = records.Ros2Geometry_msgsMsgQuaternion()
    value.w = 0.0
    wire = value.encode()
    need(len(wire) == 32 and type(value).decode(wire).w == 0.0)
    refuses(lambda: type(value).decode(wire[:-1]))


def complete_components():
    request = records.Ros2Managed_fixtureSrvProbe_Request()
    need(request.encode() == b'')
    need(isinstance(type(request).decode(b''), type(request)))
    refuses(lambda: type(request).decode(b'\0'))
    goal = records.Ros2Managed_fixtureActionTrajectory_Goal()
    goal.step = 17
    need(type(goal).decode(goal.encode()).step == 17)


def normalized_ros1_primitives():
    value = records.Ros1Managed_fixtureTimes()
    value.stamp.nsec = 1000000000
    refuses(value.encode)
    value.stamp.nsec = 999999999
    value.duration_value.sec = value.duration_value.nsec = -1
    refuses(value.encode)
    value.duration_value.nsec = 1
    need(type(value).decode(value.encode()).duration_value.sec == -1)


def input_ownership():
    cls = records.Ros2Managed_fixtureMsgScalars
    wire = scalar().encode()
    value = cls.decode(wire)
    wire = b'\0' * len(wire)
    need(value.u64 == (1 << 64) - 1 and value.text == 'π')
    need(cls.NATIVE_IDENTITY is None)
    refuses(lambda: cls.decode(bytearray(scalar().encode())))
    need('_roswasm_native' not in sys.modules and 'rclpy' not in sys.modules)


def fixed_index_fields():
    strings = records.Ros2Managed_fixtureMsgStringIndex()
    need(strings.i == ['', ''])
    strings.i = ['a', 'π']
    expected = bytes.fromhex('010000006102000000cf80')
    need(strings.encode() == expected)
    need(type(strings).decode(expected).i == ['a', 'π'])
    values = records.Ros2Managed_fixtureMsgRecordIndex()
    values.i[0].pair[0] = 3.0
    need(values.i[1].pair[0] == 0.0 and type(values).decode(values.encode()).i[0].pair[0] == 3.0)


def class_scope_fields():
    value = records.Ros2Managed_fixtureMsgClassScope(
        classmethod=17, list='a', str='π', values=[0, 255], record=1, sealed=2, permits=3)
    expected = bytes.fromhex('11010000006102000000cf800200000000ff010203')
    need(value.encode() == expected)
    result = type(value).decode(expected)
    need((result.classmethod, result.list, result.str, result.record, result.sealed, result.permits)
         == (17, 'a', 'π', 1, 2, 3))


def complete_jointstate_values():
    for version in (1, 2):
        module = joint_ros1_records if version == 1 else joint_ros2_records
        cls = getattr(module, 'Ros1Sensor_msgsJointState' if version == 1 else 'Ros2Sensor_msgsMsgJointState')
        value = cls()
        if version == 1:
            value.header.seq = 19
            value.header.stamp.sec = 42
            value.header.stamp.nsec = 300000000
            prefix = struct.pack('<III', 19, 42, 300000000)
        else:
            value.header.stamp.sec = -2
            value.header.stamp.nanosec = 300000000
            prefix = struct.pack('<iI', -2, 300000000)
        value.header.frame_id = 'base'
        value.name = ['left', 'right']
        value.position = [1.25, -2.5]
        value.velocity = []
        value.effort = [0.5, -0.25]
        expected = (prefix + struct.pack('<I', 4) + b'base' + struct.pack('<II', 2, 4) + b'left'
                    + struct.pack('<I', 5) + b'right' + struct.pack('<IddII', 2, 1.25, -2.5, 0, 2)
                    + struct.pack('<dd', 0.5, -0.25))
        need(value.encode() == expected)
        result = cls.decode(expected)
        need(result.header.frame_id == 'base' and result.name == ['left', 'right'])
        need(result.position == [1.25, -2.5] and result.velocity == [] and result.effort == [0.5, -0.25])
        need((result.header.seq, result.header.stamp.sec, result.header.stamp.nsec) == (19, 42, 300000000)
             if version == 1 else (result.header.stamp.sec, result.header.stamp.nanosec) == (-2, 300000000))
        need(cls.NATIVE_IDENTITY is None)
        refuses(lambda: cls.decode(expected[:-1]))
        refuses(lambda: cls.decode(expected + b'\0'))
        value.position = [1.25]
        refuses(value.encode)


def main():
    cases = (complete_scalar_golden, signed_width_bits, complete_unsigned64,
             exact_integer_types_and_widths, finite_and_signed_zero, strict_text,
             exact_decode, nested_arrays, array_bounds, independent_defaults,
             no_missing_default_wire, complete_components, normalized_ros1_primitives,
             input_ownership, fixed_index_fields, class_scope_fields, complete_jointstate_values)
    for case in cases:
        case()
    need(len(cases) == 17)
    sys.stdout.write('managed Python record cases=17 checks=' + str(checks)
                     + ' native/ROS=false host-execution=false\n')
    sys.stdout.flush()
    return 0
