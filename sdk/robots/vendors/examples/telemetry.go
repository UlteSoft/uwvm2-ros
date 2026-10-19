//go:build tinygo && (wasm || tinygo.wasm) && ((ros1 && !ros2) || (ros2 && !ros1))

// Package vendorlesson is source-only complete typed telemetry infrastructure.
// The actual Guest module must select one backend and stage the original SDK
// under uwvm2.local/roswasm; no host Go substitute or native ROS binding is used.
package vendorlesson

import ros "uwvm2.local/roswasm"

// T is the complete generated record, never a projected native struct. Decode
// must be the actual generated decoder. Observe compares/exposes model values
// for an independent native oracle; a local count is not transport evidence.
type Telemetry[T any] struct {
 session *ros.Session
 node *ros.Node
 executor *ros.Executor
 decode func([]byte)(T,error)
 observe func(T)error
 started,closed,denied,polling bool
 status,cleanup,polls,received uint32
}
func code(err error)uint32{if err==nil{return 0};if e,ok:=err.(ros.Error);ok{return e.Code};return 22}
func NewTelemetry[T any](maximum uint32,decode func([]byte)(T,error),observe func(T)error)(*Telemetry[T],error){
 if maximum==0||maximum>32768||decode==nil||observe==nil{return nil,ros.Error{Code:22}}
 group,err:=ros.NewSession(1);if err!=nil{return nil,err}
 executor,err:=ros.NewCheckedExecutor(maximum);if err!=nil{_ = group.Close();return nil,err}
 return &Telemetry[T]{session:group,node:group.NewNode("vendor-typed-telemetry"),executor:executor,decode:decode,observe:observe},nil
}
func (t *Telemetry[T]) Initialize(mode uint32,alias string)uint32{
 if t.started||t.closed{return 9};if mode>1||len(alias)==0||len(alias)>64{return 22};t.started=true
 if mode==0{cap,err:=ros.Open(alias);if err!=nil{t.denied=code(err)==13;if t.denied{return t.finish(0)};return t.finish(code(err))};if err=cap.Close();err!=nil{t.cleanup=code(err)};return t.finish(22)}
 sub,err:=t.node.CreateSubscription(alias);if err!=nil{return t.finish(code(err))}
 err=t.executor.TryAddSubscription(sub,func(raw []byte){
  value,err:=t.decode(raw);if err==nil{err=t.observe(value)}
  if err!=nil{if t.status==0{t.status=code(err)};return};t.received++
 })
 if err!=nil{return t.finish(code(err))};return 0
}
// PollOnce does not sleep, spawn a goroutine, replay a request or grow buffers.
func (t *Telemetry[T]) PollOnce()uint32{
 if t.polling{return 17};if !t.started||t.closed{return 9};if t.polls>=4096{return t.finish(110)};t.polls++;t.polling=true;defer func(){t.polling=false}()
 count,err:=t.executor.SpinOnce();if err!=nil{return t.finish(code(err))}
 if t.status!=0{return t.finish(t.status)};if count==0{return 11};return 0
}
func (t *Telemetry[T]) finish(first uint32)uint32{
 if t.closed{if first!=0{return first};return 9};t.closed=true
 if err:=t.executor.Close();err!=nil&&t.cleanup==0{t.cleanup=code(err)}
 for _,err:=range t.node.Close(){if err!=nil&&t.cleanup==0{t.cleanup=code(err)}}
 for _,err:=range t.session.Close(){if err!=nil&&t.cleanup==0{t.cleanup=code(err)}}
 if first!=0{t.status=first}else{t.status=t.cleanup};return t.status
}
func (t *Telemetry[T]) Close()uint32{if t.polling{return 17};return t.finish(t.status)}
func (t *Telemetry[T]) Received()uint32{return t.received}
func (t *Telemetry[T]) DeniedVerified()bool{return t.denied}
func (t *Telemetry[T]) Status()uint32{return t.status}
func (t *Telemetry[T]) CleanupError()uint32{return t.cleanup}
