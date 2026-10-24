# Fixed-layout telemetry used by the custom topic acceptance fixture.
#
# Official rosidl generation owns this interface's ROS2 native type identity.
# The uwvm custom generator separately defines a 45-byte canonical record:
# uint32 LE, canonical bool 0/1, finite float32 LE, three finite float64 LE,
# eight octets, and two signed int16 LE values. It never forwards native CDR.
# Fixed arrays and primitive fields are deliberate: strings and dynamic
# sequences are outside the first custom topic profile.

# Monotonic fixture sequence. It is data, never a capability or request ID.
uint32 sequence
# A canonical boolean; the Gateway rejects wire values other than 0 and 1.
bool enabled
# Temperature must be finite in every Guest and native conversion direction.
float32 temperature
# Three finite acceleration coordinates in x/y/z order.
float64[3] acceleration
# Eight uninterpreted bytes, including zero; this field is not a C string.
uint8[8] tag
# Two signed calibration thresholds; all int16 values are structurally valid.
int16[2] thresholds
