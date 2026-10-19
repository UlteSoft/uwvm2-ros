//go:build tinygo && (wasm || tinygo.wasm) && ((ros1 && !ros2) || (ros2 && !ros1))

package roswasm

import (
	"encoding/binary"
	"runtime"
	"strings"
	"unsafe"
)

// Capability owns one opaque host handle. Copies share retirement state; use a
// pointer and defer Close after every successful Open. No finalizer is installed.
// A handle cannot be transferred to another module, instance or ROS backend.
type Capability struct{ state *capabilityState }

func address(p unsafe.Pointer) (uint64, error) {
	if unsafe.Sizeof(uintptr(0)) != 4 {
		return 0, Error{Code: Unsupported}
	}
	return uint64(uintptr(p)), nil
}
func bytesAddress(b []byte) (uint64, error) {
	if len(b) == 0 {
		return 0, nil
	}
	return address(unsafe.Pointer(&b[0]))
}

// Open obtains a handle for an explicitly configured alias in memory zero.
// The host—not the application—selects resource/type/QoS/quotas. There are no
// default grants. This convenience layer supports only wasm32 ordinary pointers.
func Open(alias string) (*Capability, error) {
	if len(alias) == 0 || len(alias) > 255 || strings.IndexByte(alias, 0) >= 0 {
		return nil, Error{Code: Invalid}
	}
	input := []byte(alias)
	a, err := bytesAddress(input)
	if err != nil {
		return nil, err
	}
	var slot [8]byte
	z, err := address(unsafe.Pointer(&slot[0]))
	if err != nil {
		return nil, err
	}
	code := RawCapabilityOpen(0, a, uint64(len(input)), z)
	runtime.KeepAlive(input)
	runtime.KeepAlive(&slot)
	if err := status(code); err != nil {
		return nil, err
	}
	h := binary.LittleEndian.Uint64(slot[:])
	if h == 0 {
		return nil, Error{Code: Invalid}
	}
	return &Capability{state: &capabilityState{handle: h, live: true}}, nil
}

// RawHandle borrows the exact opaque ID for a reviewed raw call. Never edit its
// generation/slot bits, synthesize another ID or use it in a different backend.
func (c *Capability) RawHandle() (uint64, error) {
	if c == nil || c.state == nil || !c.state.live {
		return 0, Error{Code: BadHandle}
	}
	return c.state.handle, nil
}

// Close retires all local copies before calling the host. A transport error can
// follow host retirement, so the SDK does not reuse or automatically retry it.
// A shared active server callback returns17 without retirement or an import.
func (c *Capability) Close() error {
	if c == nil { return Error{Code:BadHandle} }
	return retireOwner(c.state,func(handle uint64)error { return status(RawCapabilityClose(handle)) })
}

// Revoke invalidates this alias's handles. A failed notification may follow host
// retirement; reopen is not an authority recovery mechanism.
func (c *Capability) Revoke() error {
	if c == nil { return Error{Code:BadHandle} }
	return retireOwner(c.state,func(handle uint64)error { return status(RawCapabilityRevoke(handle)) })
}

// Publish submits already encoded canonical bytes under this topic grant.
func (c *Capability) Publish(payload []byte) error {
	h, err := c.RawHandle()
	if err != nil {
		return err
	}
	a, err := bytesAddress(payload)
	if err != nil {
		return err
	}
	code := RawTopicPublish(h, 0, a, uint64(len(payload)))
	runtime.KeepAlive(payload)
	return status(code)
}

// Subscribe activates this explicitly authorized topic event queue.
func (c *Capability) Subscribe() error {
	h, err := c.RawHandle()
	if err != nil {
		return err
	}
	return status(RawTopicSubscribe(h))
}

// AdvertiseService activates an independently authorized service event queue.
func (c *Capability) AdvertiseService() error {
	h, err := c.RawHandle()
	if err != nil {
		return err
	}
	return status(RawServiceAdvertise(h))
}

// SubscribeParameterEvents activates a separate ROS2 parameter event grant.
// A scalar get/set capability does not grant this operation. ROS1 returns95.
func (c *Capability) SubscribeParameterEvents() error {
	h, err := c.RawHandle()
	if err != nil {
		return err
	}
	return status(subscribeParameterEvents(h))
}

// Poll copies one canonical event into an explicit bounded buffer. Again means
// empty. On TooLarge, an observed Required value is diagnostic: the event may
// already have been dequeued, so growing and retrying is not equivalent.
func (c *Capability) Poll(output []byte) (uint64, error) {
	h, err := c.RawHandle()
	if err != nil {
		return 0, err
	}
	b, err := bytesAddress(output)
	if err != nil {
		return 0, err
	}
	slot := [8]byte{255, 255, 255, 255, 255, 255, 255, 255}
	z, err := address(unsafe.Pointer(&slot[0]))
	if err != nil {
		return 0, err
	}
	code := RawEventPoll(h, 0, b, uint64(len(output)), z)
	runtime.KeepAlive(output)
	runtime.KeepAlive(&slot)
	return finish(code, slot, uint64(len(output)))
}

// Request invokes one exact RPC operation with full-width offsets and lengths.
// Input/output slices must be distinct; the host validates canonical structure,
// ownership and quotas. A Timeout never proves that external writes were undone.
func (c *Capability) Request(op Operation, request, output []byte) (uint64, error) {
	h, err := c.RawHandle()
	if err != nil {
		return 0, err
	}
	a, err := bytesAddress(request)
	if err != nil {
		return 0, err
	}
	b, err := bytesAddress(output)
	if err != nil {
		return 0, err
	}
	// Catch overlapping nonempty borrowed slices before the import; numeric
	// range checks avoid pointer arithmetic past either Go object.
	if len(request) > 0 && len(output) > 0 && a < b+uint64(len(output)) && b < a+uint64(len(request)) {
		return 0, Error{Code: Invalid}
	}
	slot := [8]byte{255, 255, 255, 255, 255, 255, 255, 255}
	z, err := address(unsafe.Pointer(&slot[0]))
	if err != nil {
		return 0, err
	}
	code := invoke(op, h, 0, a, uint64(len(request)), b, uint64(len(output)), z)
	runtime.KeepAlive(request)
	runtime.KeepAlive(output)
	runtime.KeepAlive(&slot)
	return finish(code, slot, uint64(len(output)))
}

// Log writes a canonical String under an independently authorized log grant.
func (c *Capability) Log(request []byte) error {
	h, err := c.RawHandle()
	if err != nil {
		return err
	}
	a, err := bytesAddress(request)
	if err != nil {
		return err
	}
	code := RawLogWrite(h, 0, a, uint64(len(request)))
	runtime.KeepAlive(request)
	return status(code)
}
