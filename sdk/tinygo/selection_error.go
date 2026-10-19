//go:build tinygo && (wasm || tinygo.wasm) && ((!ros1 && !ros2) || (ros1 && ros2))

package roswasm

// Select exactly one backend. This intentional unresolved name gives a clear
// compile-time failure instead of silently choosing the wrong import module.
var _ = select_exactly_one_of_ros1_or_ros2
