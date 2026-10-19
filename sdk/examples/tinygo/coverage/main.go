// Compile/import-inspection fixture for all raw imports of one chosen backend.
// main performs no ROS call. Never invoke roswasm_abi_coverage with a valid
// index: its zero arguments are placeholders, not real buffers/handles/grants.
package main

// InvokeRaw retains every raw import for binary signature inspection only.
// The export directive requires a TinyGo release with go:wasmexport support.
// This export must not be executed as a production or native ROS test.
//
//go:wasmexport roswasm_abi_coverage
func InvokeRaw(index uint32) uint32 { return invokeRaw(index) }

func main() { /* Compilation/import inspection only; no ROS probe execution. */ }
