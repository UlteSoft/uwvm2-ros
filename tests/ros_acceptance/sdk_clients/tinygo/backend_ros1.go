//go:build ros1 && !ros2

// This explicit compile selection does not grant any Runtime capability.
package main
const selectedBackend uint32 = 1
