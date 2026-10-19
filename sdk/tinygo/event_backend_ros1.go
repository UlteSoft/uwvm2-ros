//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2

package roswasm

// eventBackend is fixed by the selected import module, never inferred from data.
const eventBackend uint8 = 1
