# Exercise typed constants independently of the serialized field graph.
int32 NEGATIVE=-17
float64 NEGATIVE_ZERO=-0.0
string LABEL="source-constant"
bool ENABLED=true

# Preserve both the presence and value of each declaration, including empty
# text and the sign bit of floating zero. This is an owned test interface.
int32 number -17
string empty ""
string text "native-default"
float64 signed_zero -0.0
bool enabled true
