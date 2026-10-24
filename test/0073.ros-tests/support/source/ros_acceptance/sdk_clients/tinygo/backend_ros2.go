//go:build ros2 && !ros1

// This explicit compile selection does not grant any Runtime capability.
package main
const selectedBackend uint32 = 2
