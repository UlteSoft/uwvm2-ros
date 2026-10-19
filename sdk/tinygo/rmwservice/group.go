//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && rmw_service_v1

package rmwservice

import (
 "runtime"
 "unsafe"
 ros "uwvm2.local/roswasm"
)

// Options describes local storage, never Host authority. Every endpoint owns its
// packet/input/history before any capability open. Native DDS memory is extra.
// The finite guest-thread Group supports no concurrent goroutine access.
type Options struct {
 Endpoints, TransactionsPerEndpoint int
 MaximumRequest, MaximumResponse int
 WriterBytes uint8
 MaximumWaitNanoseconds uint64
 StorageBudgetBytes uint64
}
// Outcome distinguishes an actual zero result from a call that never happened.
type Outcome struct { Attempted bool; Code uint32 }
// Diagnostic is a value snapshot. Raw receipt bytes are preserved even when
// validation fails; their presence does not admit a native binding.
type Diagnostic struct {
 Primary uint32
 Open,Close,Revoke [3]Outcome
 Activation,Inspection,Submit,Respond,Abandon,Peek,Commit,Cancel,Available,Retire Outcome
 ActivationValidation,InspectionValidation uint32
 OriginalPresent,InspectionPresent bool
 Original,Inspected [256]byte
 LastPacketPrefix [352]byte
 LastPacketPresent bool
 TransactionSlots int
 LastTicket,LastCommittedSequence,LastReservation uint64
 Poisoned bool
}
type owner struct { alias [255]byte; length int; handle uint64; live bool; slot [8]byte }
type transaction struct { ticket uint64; state uint8; nativePresent bool; writerBytes uint8; writer [16]byte; nativeSequence int64 }
const (reserved uint8=1; admitted uint8=2; delivered uint8=3; done uint8=4; abandoned uint8=5)
type endpoint struct {
 group *Group; role uint8; owners [3]owner; ownerCount int
 requestMaximum,responseMaximum int
 receipt Receipt; associated,closed bool
 original,inspection [256]byte; ticket [8]byte; available [1]byte
 packet,input []byte; history []transaction; used int
 diagnostic Diagnostic
}
// transport is private. Production always uses actual numeric imports. Contracts
// in this package can inject calls before effects; applications cannot install a
// mock provider or change a live Group's backend.
type transport struct {
 abi func()uint32
 open func(*owner)uint32; close,revoke func(uint64)uint32
 activate func(*endpoint)uint32; inspect func(*endpoint)uint32
 submit func(*endpoint,int)uint32; respond func(*endpoint,uint64,int)uint32
 abandon func(*endpoint,uint64)uint32; peek func(*endpoint)uint32
 commit,cancel func(*endpoint,uint64,uint64)uint32
 available func(*endpoint,uint64)uint32; retire func(*endpoint)uint32
}
// Group is a non-copyable-by-contract local owner. Use only *Group; copied typed
// facades point to the same endpoint. Close/revoke/factories and recursive calls
// return17 while any decoder or operation borrows the whole Group. This finite
// SDK rule is not presented as original upstream RMW callback semantics.
type Group struct { self *Group; options Options; entries []endpoint; used int; busy,closed bool; scope Binding; scopeSet bool; io transport; cleanupCode uint32 }
func errCode(n uint32)error {if n==0{return nil};return ros.Error{Code:n}}
func codeOf(err error)uint32 {if err==nil{return 0};if e,ok:=err.(ros.Error);ok&&e.Code!=0{return e.Code};if e,ok:=err.(*ros.Error);ok&&e!=nil&&e.Code!=0{return e.Code};return 22}
func address(p unsafe.Pointer)uint64{return uint64(uintptr(p))}
func bytesAddress(b []byte)uint64 {if len(b)==0{return 0};return address(unsafe.Pointer(&b[0]))}
func actualTransport()transport{return transport{
 abi:RawABIVersion,
 open:func(o *owner)uint32{n:=ros.RawCapabilityOpen(0,address(unsafe.Pointer(&o.alias[0])),uint64(o.length),address(unsafe.Pointer(&o.slot[0])));runtime.KeepAlive(o);return n},
 close:ros.RawCapabilityClose,revoke:ros.RawCapabilityRevoke,
 activate:func(e *endpoint)uint32{var n uint32;if e.role==3{n=RawClientActivate(e.owners[0].handle,e.owners[1].handle,0,address(unsafe.Pointer(&e.original[0])))}else{n=RawServiceActivate(e.owners[0].handle,e.owners[1].handle,0,address(unsafe.Pointer(&e.original[0])))};runtime.KeepAlive(e);return n},
 inspect:func(e *endpoint)uint32{n:=RawEndpointInspect(e.owners[0].handle,e.receipt.Incarnation,0,address(unsafe.Pointer(&e.inspection[0])));runtime.KeepAlive(e);return n},
 submit:func(e *endpoint,size int)uint32{n:=RawClientSubmit(e.owners[0].handle,e.receipt.Incarnation,0,bytesAddress(e.input),uint64(size),address(unsafe.Pointer(&e.ticket[0])));runtime.KeepAlive(e);return n},
 respond:func(e *endpoint,t uint64,size int)uint32{n:=RawServiceRespond(e.owners[0].handle,e.receipt.Incarnation,e.owners[1].handle,t,0,bytesAddress(e.input),uint64(size));runtime.KeepAlive(e);return n},
 abandon:func(e *endpoint,t uint64)uint32{return RawClientAbandon(e.owners[0].handle,e.receipt.Incarnation,t)},
 peek:func(e *endpoint)uint32{n:=RawEndpointPeek(e.owners[0].handle,e.receipt.Incarnation,uint32(e.group.options.WriterBytes),0,bytesAddress(e.packet),uint64(len(e.packet)));runtime.KeepAlive(e);return n},
 commit:func(e *endpoint,s,r uint64)uint32{return RawEndpointCommit(e.owners[0].handle,e.receipt.Incarnation,s,r)},
 cancel:func(e *endpoint,s,r uint64)uint32{return RawEndpointCancel(e.owners[0].handle,e.receipt.Incarnation,s,r)},
 available:func(e *endpoint,w uint64)uint32{n:=RawClientAvailable(e.owners[0].handle,e.receipt.Incarnation,e.owners[2].handle,w,0,address(unsafe.Pointer(&e.available[0])));runtime.KeepAlive(e);return n},
 retire:func(e *endpoint)uint32{return RawEndpointRetire(e.owners[0].handle,e.receipt.Incarnation)},
}}
// NewGroup allocates only local storage and opens no permission. No empty/default
// option grants a capability. The raw optional ABI is first queried at activation.
func NewGroup(o Options)(*Group,error){if unsafe.Sizeof(uintptr(0))!=4{return nil,errCode(95)};return newGroup(o,actualTransport())}
func newGroup(o Options,t transport)(*Group,error){
 if o.Endpoints<1||o.Endpoints>256||o.TransactionsPerEndpoint<1||o.TransactionsPerEndpoint>256||o.MaximumRequest<0||o.MaximumResponse<0||o.MaximumRequest>inputMaximum||o.MaximumResponse>inputMaximum||o.WriterBytes<1||o.WriterBytes>16||o.MaximumWaitNanoseconds>60000000000||o.StorageBudgetBytes==0||o.StorageBudgetBytes>64*1024*1024{return nil,errCode(22)}
 maximum:=o.MaximumRequest;if o.MaximumResponse>maximum{maximum=o.MaximumResponse}
 // Checked in uint64 before converting lengths to wasm32 int. This is an
 // explicit payload/state allowance, not an exact Go GC/RSS accounting promise.
 per:=uint64(unsafe.Sizeof(endpoint{}))+uint64(maximum)*2+overhead+uint64(o.TransactionsPerEndpoint)*uint64(unsafe.Sizeof(transaction{}))
 total:=uint64(unsafe.Sizeof(Group{}))+uint64(o.Endpoints)*per
 if total>o.StorageBudgetBytes{return nil,errCode(90)}
 g:=&Group{options:o,entries:make([]endpoint,o.Endpoints),io:t};g.self=g
 for i:=range g.entries {e:=&g.entries[i];e.group=g;e.packet=make([]byte,overhead+maximum);e.input=make([]byte,maximum);e.history=make([]transaction,o.TransactionsPerEndpoint)}
 return g,nil
}
func(g *Group)enter()uint32{if g==nil||g.self!=g{return 22};if g.busy{return 17};if g.closed{return 125};g.busy=true;return 0}
func(g *Group)leave(){g.busy=false}
func(e *endpoint)primary(n uint32){if n!=0&&e.diagnostic.Primary==0{e.diagnostic.Primary=n}}
func(e *endpoint)retire(){if !e.associated||e.diagnostic.Retire.Attempted{return};e.diagnostic.Retire.Attempted=true;e.diagnostic.Retire.Code=e.group.io.retire(e)}
func(e *endpoint)poison(n uint32){e.primary(n);e.diagnostic.Poisoned=true}
func(e *endpoint)terminal(n uint32)uint32{e.poison(n);e.retire();return n}
func(e *endpoint)cleanup(){if e.closed{return};e.closed=true;e.retire();for i:=e.ownerCount-1;i>=0;i--{o:=&e.owners[i];if !o.live{continue};o.live=false;e.diagnostic.Close[i].Attempted=true;e.diagnostic.Close[i].Code=e.group.io.close(o.handle)}}
func(e *endpoint)live()uint32{if e.closed||e.diagnostic.Retire.Attempted{return 125};if e.diagnostic.Poisoned {if e.diagnostic.Primary!=0{return e.diagnostic.Primary};return 125};if !e.associated{return 9};return 0}
// Close once-retires every admitted native object BEFORE independently closing
// its query/peer/source raw owners in reverse order. Every outcome is retained;
// cleanup continues after failures. A repeated Close performs no imports.
func(g *Group)Close()error{if g==nil||g.self!=g{return errCode(22)};if g.busy{return errCode(17)};if g.closed{return errCode(g.cleanupCode)};g.busy=true;g.closed=true;defer g.leave();for i:=g.used-1;i>=0;i--{e:=&g.entries[i];e.cleanup();if g.cleanupCode==0{if e.diagnostic.Retire.Attempted&&e.diagnostic.Retire.Code!=0{g.cleanupCode=e.diagnostic.Retire.Code};for j:=e.ownerCount-1;j>=0;j--{if g.cleanupCode==0&&e.diagnostic.Revoke[j].Attempted&&e.diagnostic.Revoke[j].Code!=0{g.cleanupCode=e.diagnostic.Revoke[j].Code};if g.cleanupCode==0&&e.diagnostic.Close[j].Code!=0{g.cleanupCode=e.diagnostic.Close[j].Code}}}};return errCode(g.cleanupCode)}
// Diagnostic returns a fixed value copy for one reserved local row, including
// failed factories. It never returns live raw handles or borrowed byte slices.
func(g *Group)Diagnostic(row int)(Diagnostic,error){if g==nil||g.self!=g||row<0||row>=g.used{return Diagnostic{},errCode(9)};return g.entries[row].diagnostic,nil}
func(g *Group)Rows()int{if g==nil||g.self!=g{return 0};return g.used}
func validAlias(s string)bool{if len(s)==0||len(s)>255{return false};for i:=range s{if s[i]==0{return false}};return true}
func sameAlias(o *owner,s string)bool{if o.length!=len(s){return false};for i:=range s{if o.alias[i]!=s[i]{return false}};return true}
func(g *Group)open(role uint8,aliases [3]string,requested [2]QoS,qmax,smax int)(*endpoint,uint32){
 if qmax<0||smax<0||qmax>g.options.MaximumRequest||smax>g.options.MaximumResponse||!requested[0].valid()||!requested[1].valid(){return nil,22}
 count:=2;if role==3&&aliases[2]!=""{count=3};if role==4&&aliases[2]!=""{return nil,22}
 for i:=0;i<count;i++{if !validAlias(aliases[i]){return nil,22};for j:=0;j<i;j++{if aliases[j]==aliases[i]{return nil,17}};for k:=0;k<g.used;k++{for j:=0;j<g.entries[k].ownerCount;j++{if sameAlias(&g.entries[k].owners[j],aliases[i]){return nil,17}}}}
 if g.used==len(g.entries){return nil,90}
 e:=&g.entries[g.used];g.used++;e.role=role;e.ownerCount=count;e.requestMaximum=qmax;e.responseMaximum=smax
 packetMaximum:=qmax;if role==3{packetMaximum=smax};e.packet=e.packet[:overhead+packetMaximum]
 for i:=0;i<count;i++{e.owners[i].length=copy(e.owners[i].alias[:],aliases[i])}
 fail:=func(n uint32)(*endpoint,uint32){e.primary(n);e.cleanup();return nil,n}
 if g.io.abi()!=1{return fail(95)}
 for i:=0;i<count;i++ {o:=&e.owners[i];e.diagnostic.Open[i].Attempted=true;n:=g.io.open(o);e.diagnostic.Open[i].Code=n;if n!=0{return fail(n)};o.handle=u64(o.slot[:]);o.live=o.handle!=0;if !fullHandle(o.handle){return fail(22)};for j:=0;j<i;j++{if e.owners[j].handle==o.handle{return fail(13)}}}
 for i:=range e.original{e.original[i]=0xa5};e.diagnostic.Activation.Attempted=true;n:=g.io.activate(e);e.diagnostic.Activation.Code=n;if n!=0{return fail(n)}
 e.diagnostic.OriginalPresent=true;e.diagnostic.Original=e.original;r,n:=parseReceipt(e.original[:],1,role,e.owners[0].handle)
 // A structurally proved actual binding can be used only for once-cleanup even
 // if a later GID/QoS check fails. No incarnation is guessed from stale scratch.
 if r.Binding.valid()&&r.Incarnation!=0{e.receipt=r;e.associated=true}
 if n==0&&(r.Actual!=requested){n=13};if n==0&&g.scopeSet&&(g.scope.Instance!=r.Binding.Instance||g.scope.Generation!=r.Binding.Generation||g.scope.Session!=r.Binding.Session){n=13}
 e.diagnostic.ActivationValidation=n;if n!=0{return fail(n)}
 e.receipt=r;e.associated=true;if !g.scopeSet{g.scope=r.Binding;g.scopeSet=true};return e,0
}
