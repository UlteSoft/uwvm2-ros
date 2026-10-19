// Stage3 demonstrates a local Node, Publisher and poll-driven Executor.
// Explicit pub/sub grants select the String topic, native QoS and queue budgets.
package main

import ros "uwvm2.local/roswasm"

func run() (first error) {
	session, err := ros.NewSession(2)
	if err != nil { return err }
	defer func() { for _, outcome := range session.Close() { if outcome != nil && first == nil { first = outcome } } }()
	node := session.NewNode("tinygo_guest") // Local label, not native identity.
	publisher, err := node.CreatePublisher("pub")
	if err != nil {
		return err
	}
	subscription, err := node.CreateSubscription("sub")
	if err != nil {
		return err
	}
	executor := ros.NewExecutor(256)
	defer func() { if err := executor.Close(); err != nil && first == nil { first = err } }()
	var callbackError error
	if err = executor.TryAddSubscription(subscription, func(bytes []byte) {
		// Callback runs on this guest thread. Scratch bytes expire at the next
		// poll; decode/copy now before storing data for later application use.
		text, err := ros.DecodeString(bytes)
		if err != nil {
			println(err.Error())
			callbackError = err
			return
		}
		println("received", text)
	}); err != nil { return err }
	var scratch [64]byte
	if err = publisher.PublishString("hello from Node", scratch[:]); err != nil {
		return err
	}
	// This single pass may deliver zero callbacks: native delivery is asynchronous.
	// Scheduling later polls belongs to the application; no unbounded busy loop
	// or automatic retry of side-effecting errors is installed.
	count, err := executor.SpinOnce()
	if err != nil {
		return err
	}
	if callbackError != nil { return callbackError }
	println("delivered", count)
	return nil
}
func main() {
	if err := run(); err != nil {
		println(err.Error())
		panic("checked ROS example failed")
	}
}
