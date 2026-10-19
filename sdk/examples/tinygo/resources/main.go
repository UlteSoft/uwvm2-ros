// Stage4 demonstrates independently granted resource operations. Every alias
// below needs an explicit host policy grant; omit unused resource calls rather
// than assuming that one topic/Node/goal grants all sibling operations.
package main

import ros "uwvm2.local/roswasm"

func run() (first error) {
	session, err := ros.NewSession(8)
	if err != nil { return err }
	defer func() { for _, outcome := range session.Close() { if outcome != nil && first == nil { first = outcome } } }()
	node := session.NewNode("tinygo_resources")
	var scratch [1024]byte
	service, err := node.CreateClient("trigger-call")
	if err != nil {
		return err
	}
	response, err := service.Trigger(scratch[:])
	if err != nil {
		return err
	}
	println("native service", response.Success, response.Message)
	send, err := node.CreateActionClient("fibonacci-send")
	if err != nil {
		return err
	}
	goal, err := send.SendFibonacciGoal(5)
	if err != nil {
		return err
	}
	println("Gateway-issued goal ID", goal.Raw())
	// Feedback/result/cancel each require additional independently granted aliases.
	get, err := node.CreateParameters("bool-get")
	if err != nil {
		return err
	}
	value, err := get.GetValue(ros.ParameterBool, scratch[:])
	if err != nil {
		return err
	}
	set, err := node.CreateParameters("bool-set")
	if err != nil {
		return err
	}
	if err = set.SetValue(ros.ParameterValue{Kind: ros.ParameterBool, Bool: !value.Bool}); err != nil {
		return err
	}
	graph, err := node.CreateGraph("topics-list")
	if err != nil {
		return err
	}
	rows, err := graph.Snapshot(ros.OpGraphListTopics, scratch[:])
	if err != nil {
		return err
	}
	println("topics", rows)
	clock, err := node.CreateClock("ros-clock")
	if err != nil {
		return err
	}
	nanos, err := clock.Read(ros.OpTimeReadRos)
	if err != nil {
		return err
	}
	println("exact uint64 ROS nanoseconds", nanos)
	logger, err := node.CreateLogger("log-write")
	if err != nil {
		return err
	}
	return logger.Write("resource example completed", scratch[:])
}
func main() {
	if err := run(); err != nil {
		println(err.Error())
		panic("checked ROS example failed")
	}
}
