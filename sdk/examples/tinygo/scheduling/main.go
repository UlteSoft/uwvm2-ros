// Lesson7: local Future, Context, Timer and CallbackGroup with explicit time.
// No capability/clock/native ROS call occurs. Real applications may supply time
// from an independently granted monotonic Clock; Context never opens it itself.
// A deferred Client would retain synchronous host deadline/effect semantics.
// Compile with -target=wasi -scheduler=none; this caller-driven lesson uses no
// goroutines, time.Sleep, cooperative idle loop or implicit scheduling clock.
// Inspect the generated imports: this flag is not a permission grant and does
// not remove dependencies reached by arbitrary application/library code.
// TinyGo0.43 WASIp1 startup may still need entropy through libc arc4random.
// Denied random_get can abort before main; no noncryptographic fallback is
// assumed. A required random permission is a separate explicit operator choice,
// never an SDK/ROS grant. A freestanding target needs separate qualification.
// Leave default rights empty and do not rewrite the official TinyGo runtime.
package main

import ros "uwvm2.local/roswasm"

func run()(first error){
	context,err:=ros.NewContext(2); if err!=nil { return err }
	defer func(){ if outcome:=context.Shutdown(); outcome!=nil && first==nil { first=outcome } }()
	future,err:=ros.Defer(context,func()(uint64,error){ return 42,nil }); if err!=nil { return err }
	timer,err:=context.NewTimer(10,5,&ros.CallbackGroup{},func()error{ println("one local tick"); return nil }); if err!=nil { return err }
	if _,err=context.SpinOnce(0); err!=nil { return err }
	value,err:=future.Take(); if err!=nil { return err }; println("owned deferred result",value)
	if _,err=context.SpinOnce(10); err!=nil { return err }
	return timer.Cancel()
}
func main(){ if err:=run(); err!=nil { println(err.Error()); panic("checked scheduling example failed") } }
