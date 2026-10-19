//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && rmw_service_v1

package rmwservice

// Codec must be bound to the actual generated complete canonical record. Decode
// must return an independently owned value (no aliases into its input); it must
// check exact extent, all fields/bounds/default/raw-byte domains. Encode/Decode
// may allocate the typed value before ACK; process OOM/traps require the external
// finite Source owner and cannot be turned into a fabricated cleanup success.
type Codec[T any] struct { Maximum int; Encode func(T)([]byte,error); Decode func([]byte)(T,error) }
// Definition carries source-selected root identity, never a permission. Use the
// generator's SERVICE-root CodecIdentity, not a Request/Response ID. UMRE has no
// type/hash strings: operator policy must independently pair these aliases with
// this full Native type/canonical identity. This SDK never claims that nonempty
// local strings authenticate an export/hash/TypeSupport graph.
type Definition[Q,S any] struct { NativeType,CanonicalIdentity string; Request Codec[Q]; Response Codec[S] }
type ClientAliases struct { Call,Observer,Availability string }
type ServerAliases struct { Advertise,Responder string }
// Delivery exposes a value only after complete private decoding and commit0.
// A validated control has HasValue=false and actual metadata/error. Abandoned
// data is fully decoded and ACKed, then discarded with Abandoned=true/code125.
type Delivery[T any] struct { Value T; HasValue,Abandoned bool; Metadata Metadata }
type Client[Q,S any] struct { e *endpoint; definition Definition[Q,S] }
type Server[Q,S any] struct { e *endpoint; definition Definition[Q,S] }
func validIdentity(s string)bool{if len(s)!=73||s[:9]!="UWVMROS1_"{return false};for i:=9;i<len(s);i++{if !((s[i]>='0'&&s[i]<='9')||(s[i]>='a'&&s[i]<='f')){return false}};return true}
func validDefinition[Q,S any](d Definition[Q,S])bool{return validAlias(d.NativeType)&&validIdentity(d.CanonicalIdentity)&&d.Request.Maximum>=0&&d.Response.Maximum>=0&&d.Request.Encode!=nil&&d.Request.Decode!=nil&&d.Response.Encode!=nil&&d.Response.Decode!=nil}
// NewClient opens exactly Call/Observer and optionally a third independent
// Availability owner before activation. No query alias means Available95 and no
// query import; no probe client, ambient time grant or graph-list approximation.
func NewClient[Q,S any](g *Group,d Definition[Q,S],a ClientAliases,actual [2]QoS)(*Client[Q,S],error){
 if !validDefinition(d){return nil,errCode(22)};c:=&Client[Q,S]{definition:d}
 if n:=g.enter();n!=0{return nil,errCode(n)};defer g.leave();e,n:=g.open(3,[3]string{a.Call,a.Observer,a.Availability},actual,d.Request.Maximum,d.Response.Maximum);if n!=0{return nil,errCode(n)};c.e=e;return c,nil
}
// NewServer opens Advertise/Responder independently. It never opens an observer,
// advertises via the legacy ABI or hides an ungranted responder in activation.
func NewServer[Q,S any](g *Group,d Definition[Q,S],a ServerAliases,actual [2]QoS)(*Server[Q,S],error){
 if !validDefinition(d){return nil,errCode(22)};s:=&Server[Q,S]{definition:d}
 if n:=g.enter();n!=0{return nil,errCode(n)};defer g.leave();e,n:=g.open(4,[3]string{a.Advertise,a.Responder,""},actual,d.Request.Maximum,d.Response.Maximum);if n!=0{return nil,errCode(n)};s.e=e;return s,nil
}
func(c *Client[Q,S])endpoint()*endpoint{if c==nil{return nil};return c.e}
func(s *Server[Q,S])endpoint()*endpoint{if s==nil{return nil};return s.e}
func enterEndpoint(e *endpoint)uint32{if e==nil||e.group==nil{return 9};if n:=e.group.enter();n!=0{return n};if n:=e.live();n!=0{e.group.leave();return n};return 0}
func(c *Client[Q,S])Submit(request Q)(uint64,error){
 e:=c.endpoint();if n:=enterEndpoint(e);n!=0{return 0,errCode(n)};defer e.group.leave()
 bytes,err:=c.definition.Request.Encode(request);if err!=nil{return 0,err};if len(bytes)>c.definition.Request.Maximum{return 0,errCode(90)}
 ticket,n:=e.submit(bytes);return ticket,errCode(n)
}
func(c *Client[Q,S])Abandon(ticket uint64)error{e:=c.endpoint();if n:=enterEndpoint(e);n!=0{return errCode(n)};defer e.group.leave();return errCode(e.abandon(ticket))}
// Available forwards one finite wait to THIS real retained client. Zero performs
// one actual query. A positive bound does not infer a local clock or grant; Host
// independently admits query deadline/clock/GraphDomain. There is no SDK loop.
func(c *Client[Q,S])Available(waitNanoseconds uint64)(bool,error){e:=c.endpoint();if n:=enterEndpoint(e);n!=0{return false,errCode(n)};defer e.group.leave();v,n:=e.availableNow(waitNanoseconds);return v,errCode(n)}
func(c *Client[Q,S])Inspect()(Receipt,error){e:=c.endpoint();if n:=enterEndpoint(e);n!=0{return Receipt{},errCode(n)};defer e.group.leave();if n:=e.inspect();n!=0{return Receipt{},errCode(n)};return e.receipt,nil}
func(s *Server[Q,S])Inspect()(Receipt,error){e:=s.endpoint();if n:=enterEndpoint(e);n!=0{return Receipt{},errCode(n)};defer e.group.leave();if n:=e.inspect();n!=0{return Receipt{},errCode(n)};return e.receipt,nil}
// Receive performs one actual peek, not a blocking/future driver. Again11 is an
// idle observation. Decode runs under the whole Group lease. A decoder panic
// cancels this exact borrow before unwinding; it is never swallowed/replayed.
func(c *Client[Q,S])Receive()(result Delivery[S],err error){
 e:=c.endpoint();if n:=enterEndpoint(e);n!=0{return result,errCode(n)};defer e.group.leave()
 d,n:=e.borrow();if n!=0{return result,errCode(n)}
 borrowed:=true;defer func(){if borrowed{e.cancel(d,22);e.terminal(22)}}()
 if d.Kind==6||d.Kind==7 {reason:=d.Error;if reason==0{reason=125};prepared:=Delivery[S]{Metadata:d.Metadata};preparedError:=errCode(reason);borrowed=false;if n=e.takeControl(d);n!=0{return result,errCode(n)};return prepared,preparedError}
 value,decodeError:=c.definition.Response.Decode(d.body)
 if decodeError!=nil{borrowed=false;n=e.cancel(d,codeOf(decodeError));if n!=90&&n!=95{e.terminal(n)};return result,decodeError}
 // Even an abandoned/tombstone response must pass the SAME complete decoder.
 t:=e.find(d.Ticket);wasAbandoned:=t.state==abandoned
 prepared:=Delivery[S]{Metadata:d.Metadata};var preparedError error;if wasAbandoned{prepared.Abandoned=true;preparedError=errCode(125)}else{prepared.Value=value;prepared.HasValue=true}
 borrowed=false;if n=e.commit(d);n!=0{return result,errCode(n)};t.state=done;e.diagnostic.LastTicket=d.Ticket
 return prepared,preparedError
}
// TakeRequest returns the full owned request only after commit0. Ticket remains
// opaque; native writer/sequence/timestamps are separate actual Metadata fields.
func(s *Server[Q,S])TakeRequest()(result Delivery[Q],err error){
 e:=s.endpoint();if n:=enterEndpoint(e);n!=0{return result,errCode(n)};defer e.group.leave()
 d,n:=e.borrow();if n!=0{return result,errCode(n)};borrowed:=true;defer func(){if borrowed{e.cancel(d,22);e.terminal(22)}}()
 if d.Kind==6||d.Kind==7{reason:=d.Error;if reason==0{reason=125};prepared:=Delivery[Q]{Metadata:d.Metadata};preparedError:=errCode(reason);borrowed=false;if n=e.takeControl(d);n!=0{return result,errCode(n)};e.poison(reason);return prepared,preparedError}
 value,decodeError:=s.definition.Request.Decode(d.body)
 if decodeError!=nil{borrowed=false;n=e.cancel(d,codeOf(decodeError));if n!=90&&n!=95{e.terminal(n)};return result,decodeError}
 // A fixed history slot is already available; all state assignments after ACK
 // are non-allocating. Values were completely constructed before consuming it.
 prepared:=Delivery[Q]{Value:value,HasValue:true,Metadata:d.Metadata};t:=&e.history[e.used];borrowed=false;if n=e.commit(d);n!=0{return result,errCode(n)}
 t.ticket=d.Ticket;t.state=delivered;t.nativePresent=true;t.writerBytes=d.WriterBytes;t.writer=d.Writer;t.nativeSequence=d.RequestSequence;e.used++;e.diagnostic.TransactionSlots=e.used;e.diagnostic.LastTicket=d.Ticket
 return prepared,nil
}
func(s *Server[Q,S])Respond(ticket uint64,response S)error{
 e:=s.endpoint();if n:=enterEndpoint(e);n!=0{return errCode(n)};defer e.group.leave();t:=e.find(ticket);if t==nil||t.state!=delivered{return errCode(9)}
 bytes,err:=s.definition.Response.Encode(response);if err!=nil{return err};if len(bytes)>s.definition.Response.Maximum{return errCode(90)};return errCode(e.respond(ticket,bytes))
}
func(c *Client[Q,S])Close()error{return c.endpoint().close(false)}
func(s *Server[Q,S])Close()error{return s.endpoint().close(false)}
// Revoke once-retires the native object, independently attempts all granted raw
// revocations, then closes every raw owner. Diagnostic retains each outcome.
func(c *Client[Q,S])Revoke()error{return c.endpoint().close(true)}
func(s *Server[Q,S])Revoke()error{return s.endpoint().close(true)}
func endpointDiagnostic(e *endpoint)(Diagnostic,error){if e==nil{return Diagnostic{},errCode(9)};return e.diagnostic,nil}
func(c *Client[Q,S])Diagnostic()(Diagnostic,error){return endpointDiagnostic(c.endpoint())}
func(s *Server[Q,S])Diagnostic()(Diagnostic,error){return endpointDiagnostic(s.endpoint())}
