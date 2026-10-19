// Lesson5: polling Trigger/SetBool and Fibonacci servers with checked group
// cleanup. Build with exactly one backend. Grant trigger-events/replies and
// fibonacci-events/replies separately for the same native service/Action.
// Types, identities, deadlines, queues, inflight and rate quotas are host policy.
// A single poll is an illustration; Again is idle, not a request or delivery.
package main

import ros "uwvm2.local/roswasm"

func run() (first error) {
	session,err:=ros.NewSession(4); if err!=nil { return err }
	defer func(){ for _,outcome:=range session.Close(){ if outcome!=nil && first==nil { first=outcome } } }()
	node:=session.NewNode("tinygo_servers")
	service,err:=node.CreateServer("trigger-events","trigger-replies"); if err!=nil { return err }
	action,err:=node.CreateActionServer("fibonacci-events","fibonacci-replies"); if err!=nil { return err }
	var input [64]byte; var output [256]byte
	delivered,err:=service.SpinOnce(input[:],output[:],func(id ros.RequestID,request,out []byte)(uint64,error){
		// This lesson's configured Trigger request has no body. SetBool would
		// decode its one byte here. Reply identity is checked before dispatch.
		if len(request)!=0 { return 0,ros.Error{Code:ros.Invalid} }
		return ros.EncodeServiceReply(id,ros.ServiceResponse{Success:true,Message:"handled"},out)
	}); if err!=nil { return err }; println("service delivered",delivered)
	handled,err:=action.SpinGoalOnce(func(id ros.GoalID,order int32)error{
		if order<0 || order>46 { return ros.Error{Code:ros.Invalid} }
		// Match the repository's tutorial: [0,1], then append for1..order-1.
		// The order bound keeps every signed int32 sum exact. Emit one result;
		// no hidden thread, sleep or feedback loop is created by this example.
		values:=[]int32{0,1}
		for index:=int32(1); index<order; index++ { values=append(values,values[index]+values[index-1]) }
		return action.RespondFibonacci(id,2,values,output[:])
	}); if err!=nil { return err }; println("goal delivered",handled)
	return nil
}
func main(){ if err:=run(); err!=nil { println(err.Error()); panic("checked server example failed") } }
