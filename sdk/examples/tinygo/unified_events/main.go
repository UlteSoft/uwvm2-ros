// Complete caller-driven unified event application; compile once with tags ros1
// and once with ros2, -scheduler=none. Explicit sub String and subEvents observer
// grants are mandatory; default open13 is a checked failure. No goroutine/sleep,
// automatic entropy grant or permission is inferred. See docs/unified-events.md.
package main

import ros "uwvm2.local/roswasm"

func run() (primary error) {
	source, err := ros.Open("sub")
	if err != nil {
		return err
	}
	defer func() {
		if _, owned := source.RawHandle(); owned == nil {
			if e := source.Close(); e != nil {
				println("source cleanup", e.Error())
				if primary == nil {
					primary = e
				}
			}
		}
	}()
	observer, err := ros.Open("subEvents")
	if err != nil {
		return err
	}
	defer func() {
		if e := observer.Close(); e != nil {
			println("observer cleanup", e.Error())
			if primary == nil {
				primary = e
			}
		}
	}()
	stream, err := ros.NewEventStream(observer, source, 0)
	if err != nil {
		return err
	}
	executor, err := ros.NewEventExecutor(256)
	if err != nil {
		return err
	}
	var last ros.EventKind
	if err = executor.Add(stream, func(event ros.UnifiedEvent) error {
		last = event.Kind // Body borrows fixed executor scratch until return.
		return nil        // Choose the actual source codec before inspecting Body.
	}); err != nil {
		return err
	}
	if _, err = stream.Readiness(); err != nil {
		return err
	}
	if _, err = executor.SpinOnce(); err != nil {
		return err
	}
	// Remove before changing lifetime; executor no longer borrows this stream.
	if _, err = executor.Remove(0); err != nil {
		return err
	}
	if err = executor.Close(); err != nil {
		return err
	}
	if err = source.Close(); err != nil {
		return err
	}
	var storage [256]byte
	done, err := stream.SpinOnce(storage[:], func(event ros.UnifiedEvent) error { last = event.Kind; return nil })
	if err != nil {
		return err
	}
	if !done || last != ros.EventResourceClosed {
		return ros.Error{Code: ros.Invalid}
	}
	return nil
}
func main() {
	if err := run(); err != nil {
		println(err.Error())
		panic("checked unified event application failed")
	}
}
