// Stage2 demonstrates owned capability wrappers and checked canonical codecs.
// The explicit pub grant fixes the native String topic and all quotas.
package main

import ros "uwvm2.local/roswasm"

func run() (first error) {
	publisher, err := ros.Open("pub")
	if err != nil {
		return err
	}
	defer func() { if err := publisher.Close(); err != nil && first == nil { first = err } }() // Always inspect checked retirement.
	var scratch [64]byte
	n, err := ros.EncodeString("hello from TinyGo", scratch[:])
	if err != nil {
		return err
	}
	// EncodeString returns exactly the initialized prefix. Capacity is bounded
	// by this application and does not enlarge a host message quota.
	return publisher.Publish(scratch[:n])
}
func main() {
	if err := run(); err != nil {
		println(err.Error())
		panic("checked ROS example failed")
	}
}
