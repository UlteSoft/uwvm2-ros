//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1

package roswasm

// eventBackend is fixed by the selected import module, never inferred from data.
const eventBackend uint8 = 2
