# Unitree protocol and checksum contract

The normative upstream references below are pinned in `upstream-lock.json`.
All official source content remains an external dependency. A schema record,
checksum or request identity never supplies ROS authority or network isolation.

## Families and transport names

The selected ROS2 definitions are in
[`unitree_ros2@668d1ec5`](https://github.com/unitreerobotics/unitree_ros2/tree/668d1ec5a05d1c38d3306bdca7d59f2ba3581a88/cyclonedds_ws/src/unitree).
`unitree_go`, `unitree_hg` and `unitree_api` are distinct packages. Do not exchange
GO20-slot and HG35-slot records, even for robots sharing a brand. Go2/H1 family
examples use GO; G1/H1-2 examples use HG. The carrier's20/35 slots do not establish
an actual robot's DOF, actuator order, model torque limits or safe motion.
The selected official model and controller must independently establish those.

| ROS2 graph topic | SDK2 DDS topic | Meaning |
| --- | --- | --- |
| `/lowcmd` | `rt/lowcmd` | complete selected-family LowCmd |
| `/lowstate` | `rt/lowstate` | complete selected-family LowState |
| `/sportmodestate` | `rt/sportmodestate` | GO SportModeState where supported |
| `/api/sport/request` | `rt/api/sport/request` | unitree_api Request topic |
| `/api/sport/response` | `rt/api/sport/response` | unitree_api Response topic |

The SDK2 DDS prefix is not an extra ROS2 namespace. Publishing ROS2 `/rt/lowcmd`
does not select the same topic. Paths for other auxiliary records are selected
explicitly by a profile/controller; a `.msg` basename does not invent a topic.
Official references:
[`ros2_sport_client.h`](https://github.com/unitreerobotics/unitree_ros2/blob/668d1ec5a05d1c38d3306bdca7d59f2ba3581a88/example/src/include/common/ros2_sport_client.h),
[`SDK2 simulator bridge`](https://github.com/unitreerobotics/unitree_mujoco/blob/1eb6642e3f3fdfb7fb13a9794fd6a2dd93ea0e7d/simulate/src/unitree_sdk2_bridge.h).

ROS1
[`unitree_legged_msgs`](https://github.com/unitreerobotics/unitree_ros_to_real/tree/b989870124913091fbe75e0dcfb047eb4ca00a09/unitree_legged_msgs/msg)
uses exact camelCase, `Kp`/`Kd` and `SN` spellings. Its LowCmd layout is different
from GO ROS2. The external ROS1 UDP bridge/SDK owns its packet CRC convention;
these GO/HG helpers must not be applied by guessing a legacy layout. No CRC field
is added to any message that did not declare it. Legacy topic conventions are
an explicit external adapter selection, not assumed from the ROS2 table above.

## Portable command checksum packing

The checksum algorithm is init0xffffffff and polynomial0x04c11db7. Read every
input32-bit word in little-endian byte order, process its bits from bit31 to0,
and apply no final XOR or reflected-byte transform. Exclude the final CRC word.
The generic helper permits0..32768 bytes in multiples of4; zero bytes yields
the initial value. It uses byte loads and explicit unsigned32 operations, so
neither host endianness, alignment, `size_t` width nor object padding changes it.

The layouts follow official
[`motor_crc.h`](https://github.com/unitreerobotics/unitree_ros2/blob/668d1ec5a05d1c38d3306bdca7d59f2ba3581a88/example/src/include/common/motor_crc.h),
[`motor_crc.cpp`](https://github.com/unitreerobotics/unitree_ros2/blob/668d1ec5a05d1c38d3306bdca7d59f2ba3581a88/example/src/src/common/motor_crc.cpp),
[`motor_crc_hg.h`](https://github.com/unitreerobotics/unitree_ros2/blob/668d1ec5a05d1c38d3306bdca7d59f2ba3581a88/example/src/include/common/motor_crc_hg.h),
and SDK2 Python's explicit LE `struct.pack` formats in
[`crc.py@814556d1`](https://github.com/unitreerobotics/unitree_sdk2_python/blob/814556d15970dd2ecf1c9984e845ca02ab07e206/unitree_sdk2py/utils/crc.py).

GO LowCmd's complete compact canonical payload is749 bytes. The **separate**
checksum packet is812 bytes; exactly808 bytes/202 words enter CRC:

| Offset | Bytes | Contents |
| ---: | ---: | --- |
| 0 | 2 | head |
| 2 | 1 | level_flag |
| 3 | 1 | frame_reserve (must0) |
| 4 | 8 | sn[2], LE32 |
| 12 | 8 | version[2], LE32 |
| 20 | 2 | bandwidth, LE16 |
| 22 | 2 | zero padding |
| 24 | 720 | motor_cmd[20], stride36 |
| 744 | 4 | BMS off + reserve[3] (reserves must0) |
| 748 | 40 | wireless_remote |
| 788 | 12 | led |
| 800 | 2 | fan |
| 802 | 1 | gpio |
| 803 | 1 | zero padding |
| 804 | 4 | reserve (must0) |
| 808 | 4 | computed CRC LE32 |

Each GO motor starts at24+36*i: mode+0; zero padding+1..3; finite IEEE binary32
q+4, dq+8, tau+12, kp+16, kd+20; reserve[3]+24,+28,+32 (must0).
`head`, level, SN, version and control values are preserved, not assigned a
universal firmware meaning by this helper.

HG LowCmd's complete compact canonical payload is897 bytes. Its checksum packet
is1004 bytes; exactly1000 bytes/250 words enter CRC:

| Offset | Bytes | Contents |
| ---: | ---: | --- |
| 0 | 1 | mode_pr |
| 1 | 1 | mode_machine |
| 2 | 2 | zero padding |
| 4 | 980 | motor_cmd[35], stride28 |
| 984 | 16 | reserve[4], all must0 |
| 1000 | 4 | computed CRC LE32 |

Each HG motor starts at4+28*i: mode+0; zero padding+1..3; finite q+4,dq+8,tau+12,
kp+16,kd+20; reserve+24 (must0). GO and HG field-view structs are semantic input
containers, not packed structs. The implementation validates all fields and all
storage overlaps before committing any caller output.

There is a concrete upstream discrepancy at
[`motor_crc_hg.cpp:20`](https://github.com/unitreerobotics/unitree_ros2/blob/668d1ec5a05d1c38d3306bdca7d59f2ba3581a88/example/src/src/common/motor_crc_hg.cpp#L20):
the top-level four-word reserve copy uses a byte count of4. SDK2 Python packs
all four words. Our supported encoder space requires **every named reserved
command field to be zero**, including each motor, frame/BMS and top-level field.
Nonzero reserve is an error22 and never silently rewritten. Consequently the
two official oracle paths must agree in that supported space. No compatibility
or firmware behavior is asserted for nonzero reserves. The generic word CRC
function remains mathematically defined for all input bits.

Checksums provide no authentication, origin identity, replay prevention, actuator
limit, policy quota or safety guarantee. Native LowState CRC validation is not
implemented by the command helper and remains an independent oracle obligation.

## Topic Request/Response lifecycle

`unitree_api/Request` contains RequestHeader(identity{id,api_id}, lease{id},
policy{priority,noreply}), parameter text and binary bytes. Response contains
identity/status plus text/binary. These are ordinary topics, not `.srv` or
`.action` declarations. Opening a Service/Action alias or using service_submit
does not turn them into that protocol. Schema fields are preserved completely.
The selected canonical capacity is4096 UTF-8 bytes for parameter/data and2048
binary elements; embedded NUL/non-Unicode and excess capacity are refused.

The C/C++ helper tracks at most16 positive signed64 own request/API IDs with
caller-supplied finite deadlines. It does not allocate an ID, acquire a lease,
change priority or import a clock. Register before a single publication, retain
while waiting, validate the complete received value, then accept only exact
ID/API before its deadline. Wrong IDs, duplicates, idle and expiry cannot consume
another pending request. A publication failure requires explicit local forget.
Closing the local ledger never cancels remote work; independently close both
actual topic capabilities. A received status code must be interpreted by the
specific official API version; the helper does not invent Sport semantics.

## Simulator boundary and family coverage

The selected
[`MuJoCo configuration`](https://github.com/unitreerobotics/unitree_mujoco/blob/1eb6642e3f3fdfb7fb13a9794fd6a2dd93ea0e7d/simulate/config.yaml)
uses domain1 and interface `lo`. Those are example defaults, not an automatic
network sandbox. Root must enforce loopback/domain/owned-process constraints.
The bridge implements low-level LowCmd/LowState and relevant state publication;
it is not a Sport API walking controller. The stock C++ main uses a GLFW window;
a headless fixture must explicitly implement/select real MuJoCo stepping and
pin its model/assets/physics dependencies. The subscriber's existence alone
does not prove CRC checks; use an independent actual native CRC witness.

The pinned asset tree contains Go2/Go2W, B2/B2W, H1/H1-2, G1, H2, A2, AS2 and R1
models. An asset's presence is not a validated joint mapping/control law. The
Go2 stand example uses only its shared twelve-motor fixture order. The separate
GO20/HG35 sampled-hold example consumes a trusted explicit actual actuator→slot
map and samples current q on each step, with no locomotion/balance or persistent
initial-target claim. HG never reuses the Go2 trajectory. Exact UHL1 layout,
operating-mode check, inactive carrier values and full-state observation grammar
are in README. First-class simulation tests must separately prove each selected
model/controller family and compiled native/Guest comparison. IsaacLab's DDS robot/hand/environment command channels
are additional explicit protocols, not evidence that MuJoCo implements them.

The pinned SDK2 build selects architecture-specific archives under `lib/`,
currently x86_64/AArch64. Its Python Linux CRC loader also selects those targets.
Do not infer availability on i386, ARM32, LoongArch, RISC-V or big-endian targets.
Portable Wasm helpers can run through a separately qualified native ROS Gateway;
the native SDK2 participant remains an independently selected supported target.
