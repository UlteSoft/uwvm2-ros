// Lesson9: actual ROS1/ROS2 TF lookup/broadcast, filtered graph and ROS clock.
// transform-lookup/broadcast are independent grants for the same configured
// frame pair with finite translation/quaternion/cache/byte/rate/QoS limits.
// nodes-list and ros-clock need separate grants. Native peers provide frames;
// a SDK factory never chooses a new native frame, identity, clock or authority.
package main
import ros "uwvm2.local/roswasm"
func run()(first error){
	session,err:=ros.NewSession(4); if err!=nil { return err }
	defer func(){ for _,outcome:=range session.Close(){ if outcome!=nil && first==nil { first=outcome } } }()
	node:=session.NewNode("tinygo_tf")
	lookup,err:=node.CreateTf("transform-lookup"); if err!=nil { return err }; var scratch [4096]byte
	transform,err:=lookup.LookupTransform(0,scratch[:]); if err!=nil { return err }
	broadcast,err:=node.CreateTf("transform-broadcast"); if err!=nil { return err }; if err=broadcast.BroadcastTransform(transform); err!=nil { return err }
	graph,err:=node.CreateGraph("nodes-list"); if err!=nil { return err }
	rows,err:=graph.Snapshot(ros.OpGraphListNodes,scratch[:]); if err!=nil { return err }; println("filtered nodes",rows)
	clock,err:=node.CreateClock("ros-clock"); if err!=nil { return err }
	now,err:=clock.Read(ros.OpTimeReadRos); if err!=nil { return err }; println("exact ROS nanoseconds",now)
	return nil
}
func main(){ if err:=run(); err!=nil { println(err.Error()); panic("checked TF/graph/clock example failed") } }
