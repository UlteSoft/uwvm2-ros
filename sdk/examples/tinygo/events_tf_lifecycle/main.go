//go:build tinygo && wasm && ros2 && !ros1

// Lesson6 is ROS2-only. Every alias below is independent: parameter-events,
// transform-lookup, nodes-list, steady-clock, lifecycle-state/change. Host policy
// fixes parameter kind, frame pair, transition, quotas and native identities.
// No Node construction or state read grants events, transforms or transitions.
package main

import ros "uwvm2.local/roswasm"

func run()(first error){
	session,err:=ros.NewSession(6); if err!=nil { return err }
	defer func(){ for _,outcome:=range session.Close(){ if outcome!=nil && first==nil { first=outcome } } }()
	node:=session.NewNode("tinygo_events")
	parameters,err:=node.CreateParameters("parameter-events"); if err!=nil { return err }
	queue,err:=parameters.SubscribeEvents(); if err!=nil { return err }
	executor,err:=ros.NewCheckedExecutor(4096); if err!=nil { return err }
	defer func(){ if outcome:=executor.Close(); outcome!=nil && first==nil { first=outcome } }()
	var callbackError error
	if err=executor.TryAddSubscription(queue,func(bytes []byte){
		// Decode/copy before retaining anything; scratch expires on next poll.
		value,outcome:=ros.DecodeTypedParameterEvent(bytes)
		if outcome!=nil { callbackError=outcome; return }; println("parameter event",value.Kind)
	}); err!=nil { return err }
	if _,err=executor.SpinOnce(); err!=nil { return err }; if callbackError!=nil { return callbackError }
	var scratch [4096]byte
	tf,err:=node.CreateTf("transform-lookup"); if err!=nil { return err }
	transform,err:=tf.LookupTransform(0,scratch[:]); if err!=nil { return err }; println("configured parent",transform.Header.Frame)
	graph,err:=node.CreateGraph("nodes-list"); if err!=nil { return err }
	rows,err:=graph.Snapshot(ros.OpGraphListNodes,scratch[:]); if err!=nil { return err }; println("filtered nodes",rows)
	clock,err:=node.CreateClock("steady-clock"); if err!=nil { return err }
	now,err:=clock.Read(ros.OpTimeReadSteady); if err!=nil { return err }; println("exact nanoseconds",now)
	state,err:=node.CreateLifecycle("lifecycle-state"); if err!=nil { return err }
	value,err:=state.State(scratch[:]); if err!=nil { return err }; println("state",value.ID)
	change,err:=node.CreateLifecycle("lifecycle-change"); if err!=nil { return err }
	accepted,err:=change.Change(); if err!=nil { return err }; if !accepted { return ros.Error{Code:ros.Invalid} }
	return nil
}
func main(){ if err:=run(); err!=nil { println(err.Error()); panic("checked resource example failed") } }
