// Lesson8: exact Fibonacci operation grants plus BoolArray parameters.
// fibonacci-send/cancel-own/feedback/result are distinct aliases for the same
// native Action; no cancel-any right is acquired. array-get/set and parameter-list
// use independent exact node/parameter/type/value/element/byte/rate grants.
// Receive once; Again is idle. Native peers/discovery are explicit prerequisites.
package main
import ros "uwvm2.local/roswasm"
func idle(err error)bool { e,ok:=err.(ros.Error); return ok && e.Code==ros.Again }
func run()(first error){
	session,err:=ros.NewSession(7); if err!=nil { return err }
	defer func(){ for _,outcome:=range session.Close(){ if outcome!=nil && first==nil { first=outcome } } }()
	node:=session.NewNode("tinygo_action_parameters")
	send,err:=node.CreateActionClient("fibonacci-send"); if err!=nil { return err }
	cancel,err:=node.CreateActionClient("fibonacci-cancel-own"); if err!=nil { return err }
	feedback,err:=node.CreateActionClient("fibonacci-feedback"); if err!=nil { return err }
	result,err:=node.CreateActionClient("fibonacci-result"); if err!=nil { return err }
	goal,err:=send.SendFibonacciGoal(5); if err!=nil { return err }; var scratch [1024]byte
	sequence,err:=feedback.Feedback(goal,scratch[:]); if err!=nil { if !idle(err){ return err } } else { println("feedback elements",len(sequence.Values)) }
	// Cancel before consuming a terminal result, which retires this ID in the
	// Runtime. A later cancel would be stale9. False is valid if work finished
	// but its result remains unread; cancellation never undoes admitted work.
	accepted,err:=cancel.CancelOwn(goal); if err!=nil { return err }; println("own cancel",accepted)
	sequence,err=result.Result(goal,scratch[:]); if err!=nil { if !idle(err){ return err } } else { println("terminal kind",sequence.Kind) }
	get,err:=node.CreateParameters("array-get"); if err!=nil { return err }
	value,err:=get.GetValue(ros.ParameterBoolArray,scratch[:]); if err!=nil { return err }; println("array elements",len(value.Bools))
	set,err:=node.CreateParameters("array-set"); if err!=nil { return err }
	if err=set.SetValue(ros.ParameterValue{Kind:ros.ParameterBoolArray,Bools:[]bool{true,false}}); err!=nil { return err }
	list,err:=node.CreateParameters("parameter-list"); if err!=nil { return err }
	n,err:=list.List(nil,scratch[:]); if err!=nil { return err }; text,err:=ros.DecodeString(scratch[:n]); if err!=nil { return err }; println("filtered parameters",text)
	return nil
}
func main(){ if err:=run(); err!=nil { println(err.Error()); panic("checked Action/parameter example failed") } }
