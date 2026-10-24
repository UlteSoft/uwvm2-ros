# Preserve all64 bits of this constant without signed narrowing.
uint64 MAXIMUM=18446744073709551615
uint8 code 7

# Use a default accepted by the selected SDK's original MSG-to-IDL adapter.
# Escape decoding belongs to that adapter; raw source spelling is not its value.
string text_default "native path"
Child nested
Child[2] fixed
Child[<=2] sequence
string<=12[<=3] labels
