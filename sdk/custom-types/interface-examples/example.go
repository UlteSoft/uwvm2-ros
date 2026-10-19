//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1

// Package customrpc demonstrates caller-driven generated typed RPCs. Copy only
// the two generated Go modules into ./customtypes before building this package.
// The go.mod selects the real SDK; exactly ros2 is enabled by build tags.
package customrpc

import("uwvm2.local/customrpc/customtypes")

// SubmitStep admits one copied request. Keep both the client and its explicit
// observer live, and call PollCompletion once per caller scheduling step.
func SubmitStep(client *customtypes.Ros2Demo_rpcSrvIncrementClient,value int32)(uint64,error){
    return client.Submit(customtypes.Ros2Demo_rpcSrvIncrement_Request{Value:value})
}
// ServiceStep runs a local callback under the owner's lease. Nested polling or
// CloseChecked refuses Conflict before an import. A handler must return a value
// rather than also responding manually. No goroutine/native callback is started.
func ServiceStep(server *customtypes.Ros2Demo_rpcSrvIncrementServer)error{
    return server.SpinOnce(func(_ uint64,request customtypes.Ros2Demo_rpcSrvIncrement_Request)(customtypes.Ros2Demo_rpcSrvIncrement_Response,error){
        return customtypes.Ros2Demo_rpcSrvIncrement_Response{Value:request.Value},nil
    })
}
// ActionStep sends exact-ID feedback then terminal success. Refusals do not
// silently delete a local goal or replay a write after possible external effect.
func ActionStep(server *customtypes.Ros2Demo_rpcActionCountServer)error{
    id,goal,err:=server.PollOnce();if err!=nil{return err}
    if err=server.RespondFeedback(id,customtypes.Ros2Demo_rpcActionCount_Feedback{Progress:goal.Count});err!=nil{return err}
    return server.RespondResult(id,2,customtypes.Ros2Demo_rpcActionCount_Result{Total:goal.Count})
}
// FinishClient preserves the primary operation error and every cleanup result.
// The application must inspect Results even if a close notification fails107/110.
func FinishClient(client *customtypes.Ros2Demo_rpcSrvIncrementClient,primary error)customtypes.RPCCleanup{return client.CloseChecked(primary)}
