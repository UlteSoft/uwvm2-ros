// Optional reactor source, not an automatically qualified zero-right preload.
// A supervisor must first qualify the selected TinyGo reactor's _initialize
// imports/startup. It must not grant entropy/clock/filesystem as a workaround.
// The separately supplied WAT adapter calls _initialize exactly once before
// delegating scalar calls; neither Go pointers nor a goroutine crosses it.
package main

//go:wasmexport vision_clearance_mm
func visionClearanceMM(a, b, c, d int32) int32 {
	if a < 0 || a > 5000 || b < 0 || b > 5000 || c < 0 || c > 5000 || d < 0 || d > 5000 {return -2147483626}
	value := a
	if b < value {value = b}; if c < value {value = c}; if d < value {value = d}
	return value
}

func main() {} // Reactor entry is _initialize, never a claimed ROS application.
