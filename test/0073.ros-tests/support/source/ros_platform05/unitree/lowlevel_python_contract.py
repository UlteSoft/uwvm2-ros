"""Finite complete-value CRC contracts; never physical motion or ROS authority.

The four synthetic carriers exercise every motor and original GO trailer field.
Their values are deliberately unrelated to a safe actuator configuration.
The C mathematical-layout producer independently supplies packet comparison;
neither implementation substitutes for original middleware/controller tests.
This module is frozen only with the explicit lowlevel test option.
"""
import struct
from roswasm.unitree_lowlevel import LowLevelCommands, crc_words_le


SEEDS = (0, 1, 17, 243)


def command(codec, seed):
    """Build a fully populated synthetic value, with every named reserve zero."""
    value = codec()
    if codec.NATIVE_TYPE == 'unitree_go/msg/LowCmd':
        value.head = [254, 239]
        value.level_flag = (seed+3) & 255
        value.sn = [0x01020304+seed, 0xfedcba98-seed]
        value.version = [0x10203040+seed, 0xa1b2c3d4-seed]
        value.bandwidth = 0x1234+seed
        value.bms_cmd.off = (seed+7) & 255
        value.wireless_remote = [(seed+i*3) & 255 for i in range(40)]
        value.led = [(seed+i*5) & 255 for i in range(12)]
        value.fan = [(seed+11) & 255, (seed+13) & 255]
        value.gpio = (seed+19) & 255
    else:
        value.mode_pr = (seed+3) & 255
        value.mode_machine = (seed+7) & 255
    for i, motor in enumerate(value.motor_cmd):
        motor.mode = (seed+i) & 255
        motor.q = (seed-i)/8.0
        motor.dq = (3*i-seed)/16.0
        motor.tau = (i+seed)/32.0
        motor.kp = (seed+2*i)/64.0
        motor.kd = (seed-5*i)/128.0
    value.crc = 0xbadc0ffe
    return value


def values(codec, stream):
    """Emit four u32LE-framed complete padded packets, without ROS imports."""
    helper = LowLevelCommands(codec)
    for seed in SEEDS:
        packet = helper.packet(command(codec, seed))
        stream.write(struct.pack('<I', len(packet))+packet)
    stream.flush()


def check(codec):
    """Validate copy isolation, every reserve, all float fields and CRC bounds."""
    helper = LowLevelCommands(codec)
    count = 0
    def require(value):
        nonlocal count
        assert value
        count += 1
    def refuse(call):
        try: call()
        except (ValueError, TypeError, OverflowError): require(True)
        else: raise AssertionError('invalid complete LowCmd accepted')
    for seed in SEEDS:
        original = command(codec, seed)
        saved = original.encode()
        prepared = helper.prepare(original)
        packet = helper.packet(original)
        require(original.encode() == saved)
        require(prepared is not original and prepared.motor_cmd is not original.motor_cmd)
        require(all(a is not b for a, b in zip(prepared.motor_cmd, original.motor_cmd)))
        require(helper.verify(prepared))
        require(prepared.crc == int.from_bytes(packet[-4:], 'little'))
        require(len(packet) == (812 if helper.go else 1004))
        require(prepared.encode()[:-4] == saved[:-4])
        prepared.crc ^= 1
        require(not helper.verify(prepared))
        prepared.motor_cmd[0].q += 1.0
        require(original.encode() == saved)
    original = command(codec, 17)
    saved = original.encode()
    def bad_value(mutate):
        value = codec.decode(saved)
        mutate(value)
        # Invalid floats cannot themselves be encoded. Validate the original
        # complete source value after every failed operation instead.
        refuse(lambda: helper.prepare(value))
        require(original.encode() == saved)
    for i in range(len(original.motor_cmd)):
        indices = range(3) if helper.go else (None,)
        for j in indices:
            def mutate(value, i=i, j=j):
                if j is None: value.motor_cmd[i].reserve = 1
                else: value.motor_cmd[i].reserve[j] = 1
            bad_value(mutate)
    if helper.go:
        for member in ('frame_reserve', 'reserve'):
            bad_value(lambda v, member=member: setattr(v, member, 1))
        for i in range(3):
            bad_value(lambda v, i=i: v.bms_cmd.reserve.__setitem__(i, 1))
    else:
        for i in range(4):
            bad_value(lambda v, i=i: v.reserve.__setitem__(i, 1))
    for member in ('q', 'dq', 'tau', 'kp', 'kd'):
        for value in (float('nan'), float('inf'), -float('inf'), 1e300):
            bad_value(lambda v, member=member, value=value: setattr(v.motor_cmd[-1], member, value))
    bad_value(lambda v: setattr(v.motor_cmd[0], 'mode', 256))
    bad_value(lambda v: v.motor_cmd.pop())
    refuse(lambda: helper.prepare(object()))
    refuse(lambda: LowLevelCommands(object))
    require(crc_words_le(b'') == 0xffffffff)
    require(crc_words_le(bytes(range(8))) == _independent_crc(bytes(range(8))))
    require(crc_words_le(bytes(32768)) == _independent_crc(bytes(32768)))
    for data in (b'x', bytes(32772), bytearray(4), memoryview(bytes(4)), None):
        refuse(lambda data=data: crc_words_le(data))
    return count


def _independent_crc(data):
    """Bit-at-a-time xor-first reference, independent from the byte LUT."""
    value = 0xffffffff
    for offset in range(0, len(data), 4):
        value ^= int.from_bytes(data[offset:offset+4], 'little')
        for _ in range(32):
            value = ((value << 1) ^ (0x04c11db7 if value & 0x80000000 else 0)) & 0xffffffff
    return value
