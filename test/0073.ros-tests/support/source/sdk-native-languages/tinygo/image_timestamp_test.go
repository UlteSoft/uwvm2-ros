// Backend timestamp-range fixtures exercise codec data without native ROS.
// These tests are prepared and require an actual cgroup compilation/run verdict.
package roswasm

import "testing"

func TestROS1ImagePreservesU32Seconds(t *testing.T) {
	value := Image{Header: Header{Seconds: ^uint32(0), Nanoseconds: 2, Frame: "camera"}, Height: 1, Width: 1, Encoding: "rgb8", Step: 3, Data: []byte{1, 2, 3}}
	wire, err := value.Encode()
	if err != nil {
		t.Fatal(err)
	}
	if wire[4] != 255 || wire[5] != 255 || wire[6] != 255 || wire[7] != 255 {
		t.Fatal("seconds narrowed")
	}
	decoded, err := DecodeImage(wire)
	if err != nil || decoded.Header.Seconds != ^uint32(0) {
		t.Fatal("ROS1 image timestamp round trip", err)
	}
	// ROS2 host validation separately refuses seconds above its signed bound.
}
func TestTransformSecondsRemainBounded(t *testing.T) {
	value := Transform{Header: Header{Seconds: ^uint32(0), Nanoseconds: 2, Frame: "map"}, ChildFrame: "base", Values: [7]float64{0, 0, 0, 0, 0, 0, 1}}
	if _, err := value.Encode(); err == nil {
		t.Fatal("Transform accepted unbounded seconds")
	}
	value.Header.Seconds = 2147483647
	wire, err := value.Encode()
	if err != nil {
		t.Fatal(err)
	}
	for i := 4; i < 8; i++ {
		wire[i] = 255
	}
	if _, err = DecodeTransform(wire); err == nil {
		t.Fatal("Transform decode accepted unbounded seconds")
	}
}
