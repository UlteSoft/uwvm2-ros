# Official ROS1 input for the fixed-layout custom topic fixture.
# Generate this message with catkin/message_generation before selecting the
# UWVM generated native provider. Its canonical IPC layout has exactly 23 bytes:
# int32 LE, canonical bool, finite float64 LE, four raw bytes, three int16 LE.
# Fixed arrays contain no serialized length prefix in this profile. This message
# intentionally contains no native string or dynamic array allocation.
int32 sequence
bool active
float64 measurement
uint8[4] tag
int16[3] samples
