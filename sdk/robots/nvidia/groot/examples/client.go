//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && rmw_service_v1

// Complete future Wasm application: exact generated records + owned raw12.
package main
import (
 "io"

 "os"
 "strings"
 ros "uwvm2.local/roswasm"
 owned "uwvm2.local/roswasm/rmwservice"
 records "uwvm2.local/groot-example/customtypes"
)
func failure(n uint32)error{return ros.Error{Code:n}}
func decimal(s string)(uint64,error){var v uint64;if len(s)==0{return 0,failure(22)};for i:=range s{c:=s[i];if c<'0'||c>'9'||v>(^uint64(0)-uint64(c-'0'))/10{return 0,failure(22)};v=v*10+uint64(c-'0')};return v,nil}
func qos(text string)(out owned.QoS,err error){
 fields:=strings.Split(text,",");if len(fields)!=12{return out,failure(22)};var n [12]uint64
 for i,s:=range fields{n[i],err=decimal(s);if err!=nil{return out,err}}
 if n[0]<1||n[0]>2||n[1]<1||n[1]>2||n[2]<1||n[2]>2||n[3]<1||n[3]>2||n[11]>1{return out,failure(22)}
 return owned.QoS{History:uint32(n[0]),Reliability:uint32(n[1]),Durability:uint32(n[2]),Liveliness:uint32(n[3]),Depth:n[4],Deadline:owned.Duration{Seconds:n[5],Nanoseconds:n[6]},Lifespan:owned.Duration{Seconds:n[7],Nanoseconds:n[8]},Lease:owned.Duration{Seconds:n[9],Nanoseconds:n[10]},AvoidNamespace:n[11]!=0},nil
}
var lastCleanup owned.Diagnostic
func run()(primary error){
 // Arguments are existing aliases, exact independently prepared SERVICE-root
 // canonical identity, two actual QoS expectations and a complete typed JSON
 // request file. There is no model/device/path or permission-management import.
 if len(os.Args)!=8{return failure(22)}
 first,err:=qos(os.Args[5]);if err!=nil{return err};second,err:=qos(os.Args[6]);if err!=nil{return err}
 f,err:=os.Open(os.Args[7]);if err!=nil{return err}
 raw,readErr:=io.ReadAll(io.LimitReader(f,1048577));closeErr:=f.Close();if readErr!=nil{return readErr};if closeErr!=nil{return closeErr};if len(raw)>1048576{return failure(90)}
 request,err:=decodeInput(raw);if err!=nil{return err}
 // Encoder validates every scalar, full count and wire bound before any open.
 if _,err=request.Encode();err!=nil{return err}
 if request.Application_nonce==0{return failure(22)}
 definition:=owned.Definition[records.Ros2Uwvm_groot_bridgeSrvInferAction_Request,records.Ros2Uwvm_groot_bridgeSrvInferAction_Response]{
  NativeType:"uwvm_groot_bridge/srv/InferAction",CanonicalIdentity:os.Args[4],
  Request:owned.Codec[records.Ros2Uwvm_groot_bridgeSrvInferAction_Request]{Maximum:records.Ros2Uwvm_groot_bridgeSrvInferAction_RequestMaxWireBytes,Encode:func(q records.Ros2Uwvm_groot_bridgeSrvInferAction_Request)([]byte,error){return q.Encode()},Decode:records.DecodeRos2Uwvm_groot_bridgeSrvInferAction_Request},
  Response:owned.Codec[records.Ros2Uwvm_groot_bridgeSrvInferAction_Response]{Maximum:records.Ros2Uwvm_groot_bridgeSrvInferAction_ResponseMaxWireBytes,Encode:func(q records.Ros2Uwvm_groot_bridgeSrvInferAction_Response)([]byte,error){return q.Encode()},Decode:records.DecodeRos2Uwvm_groot_bridgeSrvInferAction_Response},
 }
 g,err:=owned.NewGroup(owned.Options{Endpoints:1,TransactionsPerEndpoint:2,MaximumRequest:32768,MaximumResponse:32768,WriterBytes:16,MaximumWaitNanoseconds:1000000,StorageBudgetBytes:1<<20});if err!=nil{return err}
 defer func(){cleanup:=g.Close();lastCleanup,_=g.Diagnostic(0);if primary==nil{primary=cleanup}}()
 c,err:=owned.NewClient(g,definition,owned.ClientAliases{Call:os.Args[1],Observer:os.Args[2],Availability:os.Args[3]},[2]owned.QoS{first,second});if err!=nil{return err}
 ready,err:=c.Available(1000000);if err!=nil{return err};if !ready{return failure(11)}
 ticket,err:=c.Submit(request);if err!=nil{return err} // exactly ONE submit
 for i:=0;i<64;i++{
  result,err:=c.Receive();if e,ok:=err.(ros.Error);ok&&e.Code==11{continue};if err!=nil{return err}
  if !result.HasValue||result.Metadata.Ticket!=ticket||result.Value.Application_nonce!=request.Application_nonce{return failure(22)}
  if result.Value.Status!=0{return failure(result.Value.Status)};if !result.Value.Info_is_empty{return failure(95)}
  // Actual typed diagnostics enumerate EVERY returned tensor shape and value.
  // These prints do not assert Go DWARF or current produced artifact qualification.
  for _,tensor:=range result.Value.Action{println(tensor.Key);for _,d:=range tensor.Shape{println(d)};for _,v:=range tensor.Values{println(v)}}
  return nil
 }
 abandon:=c.Abandon(ticket);if abandon!=nil{return abandon};return failure(90)
}
func main(){if err:=run();err!=nil{panic(err)}}
