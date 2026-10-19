//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && rmw_service_v1

// This real future Guest uses generated original control records, not hand-made
// native layouts or business-ABI RPC. Generation and this build are unexecuted.
package main

import (
 "bytes"
 "os"
 "strings"
 ros "uwvm2.local/roswasm"
 owned "uwvm2.local/roswasm/rmwservice"
 records "uwvm2.local/owned-rmw-service-example/customtypes"
)
// Each comma-separated profile is an explicit independently observed normalized
// expectation, not guessed from the words "default QoS". Native implementations
// can resolve middleware-default duration/liveliness values differently.
func serviceQoS(args []string)(out [2]owned.QoS,err error){
 if len(args)!=2{return out,fail(22)}
 for i,text:=range args{
  if len(text)>512{return out,fail(22)};fields:=strings.Split(text,",");if len(fields)!=12{return out,fail(22)};var n [12]uint64
  for j,field:=range fields{if len(field)==0{return out,fail(22)};for k:=range field{c:=field[k];if c<'0'||c>'9'||n[j]>(^uint64(0)-uint64(c-'0'))/10{return out,fail(22)};n[j]=n[j]*10+uint64(c-'0')}}
  if n[0]<1||n[0]>2||n[1]<1||n[1]>2||n[2]<1||n[2]>2||n[3]<1||n[3]>2||n[11]>1{return out,fail(22)}
  out[i]=owned.QoS{History:uint32(n[0]),Reliability:uint32(n[1]),Durability:uint32(n[2]),Liveliness:uint32(n[3]),Depth:n[4],Deadline:owned.Duration{Seconds:n[5],Nanoseconds:n[6]},Lifespan:owned.Duration{Seconds:n[7],Nanoseconds:n[8]},Lease:owned.Duration{Seconds:n[9],Nanoseconds:n[10]},AvoidNamespace:n[11]!=0}
 };return out,nil
}
func fail(n uint32)error{return ros.Error{Code:n}}
func gtdDefinition()owned.Definition[records.Ros2Type_description_interfacesSrvGetTypeDescription_Request,records.Ros2Type_description_interfacesSrvGetTypeDescription_Response]{
 return owned.Definition[records.Ros2Type_description_interfacesSrvGetTypeDescription_Request,records.Ros2Type_description_interfacesSrvGetTypeDescription_Response]{
  NativeType:records.Ros2Type_description_interfacesSrvGetTypeDescriptionNativeType,
  CanonicalIdentity:records.Ros2Type_description_interfacesSrvGetTypeDescriptionCodecIdentity,
  Request:owned.Codec[records.Ros2Type_description_interfacesSrvGetTypeDescription_Request]{Maximum:records.Ros2Type_description_interfacesSrvGetTypeDescription_RequestMaxWireBytes,Encode:func(v records.Ros2Type_description_interfacesSrvGetTypeDescription_Request)([]byte,error){return v.Encode()},Decode:records.DecodeRos2Type_description_interfacesSrvGetTypeDescription_Request},
  Response:owned.Codec[records.Ros2Type_description_interfacesSrvGetTypeDescription_Response]{Maximum:records.Ros2Type_description_interfacesSrvGetTypeDescription_ResponseMaxWireBytes,Encode:func(v records.Ros2Type_description_interfacesSrvGetTypeDescription_Response)([]byte,error){return v.Encode()},Decode:records.DecodeRos2Type_description_interfacesSrvGetTypeDescription_Response},
 }
}
func typesDefinition()owned.Definition[records.Ros2Rcl_interfacesSrvGetParameterTypes_Request,records.Ros2Rcl_interfacesSrvGetParameterTypes_Response]{
 return owned.Definition[records.Ros2Rcl_interfacesSrvGetParameterTypes_Request,records.Ros2Rcl_interfacesSrvGetParameterTypes_Response]{
  NativeType:records.Ros2Rcl_interfacesSrvGetParameterTypesNativeType,CanonicalIdentity:records.Ros2Rcl_interfacesSrvGetParameterTypesCodecIdentity,
  Request:owned.Codec[records.Ros2Rcl_interfacesSrvGetParameterTypes_Request]{Maximum:records.Ros2Rcl_interfacesSrvGetParameterTypes_RequestMaxWireBytes,Encode:func(v records.Ros2Rcl_interfacesSrvGetParameterTypes_Request)([]byte,error){return v.Encode()},Decode:records.DecodeRos2Rcl_interfacesSrvGetParameterTypes_Request},
  Response:owned.Codec[records.Ros2Rcl_interfacesSrvGetParameterTypes_Response]{Maximum:records.Ros2Rcl_interfacesSrvGetParameterTypes_ResponseMaxWireBytes,Encode:func(v records.Ros2Rcl_interfacesSrvGetParameterTypes_Response)([]byte,error){return v.Encode()},Decode:records.DecodeRos2Rcl_interfacesSrvGetParameterTypes_Response},
 }
}
func client(g *owned.Group,args []string,qos [2]owned.QoS)error{
 if len(args)!=5{return fail(22)}
 // All aliases are independent operator-configured existing grants. Native type
 // name/hash are actual externally exported inputs; no fixture IDs are supplied.
 c,err:=owned.NewClient(g,gtdDefinition(),owned.ClientAliases{Call:args[0],Observer:args[1],Availability:args[2]},qos);if err!=nil{return err}
 ready,err:=c.Available(1000000);if err!=nil{return err};if !ready{return fail(11)}
 q:=records.DefaultRos2Type_description_interfacesSrvGetTypeDescription_Request();q.Type_name=args[3];q.Type_hash=args[4]
 if len(q.Type_hash)!=71||q.Type_hash[:7]!="RIHS01_"{return fail(22)}
 if !q.Include_type_sources{return fail(22)} // preserve original true default
 ticket,err:=c.Submit(q);if err!=nil{return err}
 // Explicit finite observation budget. Iterations are NOT elapsed nanoseconds,
 // readiness waits, sleeps or a fake upstream executor. Each11 may spend quota.
 for i:=0;i<64;i++{
  result,err:=c.Receive();if e,ok:=err.(ros.Error);ok&&e.Code==11{continue};if err!=nil{return err}
  if !result.HasValue||result.Metadata.Ticket!=ticket||!result.Value.Successful||len(result.Value.Type_sources)==0{return fail(22)}
  // Re-encode/decode every field, including all counted raw bytes/final NUL.
  // This checks complete local retention; it does not invent Native provenance.
  first,err:=result.Value.Encode();if err!=nil{return err};again,err:=records.DecodeRos2Type_description_interfacesSrvGetTypeDescription_Response(first);if err!=nil{return err};second,err:=again.Encode();if err!=nil{return err};if !bytes.Equal(first,second){return fail(22)};return nil
 }
 // Local abandonment is independent from true native cancellation. It is never
 // followed by another submit/replay. Group cleanup still retires/closes all.
 if err:=c.Abandon(ticket);err!=nil{return err};return fail(90)
}
func server(g *owned.Group,args []string,qos [2]owned.QoS)error{
 if len(args)!=2{return fail(22)}
 s,err:=owned.NewServer(g,typesDefinition(),owned.ServerAliases{Advertise:args[0],Responder:args[1]},qos);if err!=nil{return err}
 for i:=0;i<64;i++{
  request,err:=s.TakeRequest();if e,ok:=err.(ros.Error);ok&&e.Code==11{continue};if err!=nil{return err};if !request.HasValue{return fail(22)}
  // Explicit example business semantics: this provider has no set parameters.
  // Original GetParameterTypes requires one result per requested name, in order.
  // All zero values mean genuine PARAMETER_NOT_SET, never a Native identity.
  response:=records.Ros2Rcl_interfacesSrvGetParameterTypes_Response{Types:make([]uint8,len(request.Value.Names))}
  return s.Respond(request.Metadata.Ticket,response)
 }
 return fail(90)
}
func run()(primary error){
 if len(os.Args)<4{return fail(22)};qos,err:=serviceQoS(os.Args[2:4]);if err!=nil{return err}
 g,err:=owned.NewGroup(owned.Options{Endpoints:1,TransactionsPerEndpoint:16,MaximumRequest:32768,MaximumResponse:32768,WriterBytes:16,MaximumWaitNanoseconds:1000000,StorageBudgetBytes:1<<20});if err!=nil{return err}
 // Primary error is never overwritten by cleanup. This example deliberately
 // retains the fixed cleanup diagnostic snapshot separately for a real harness.
 defer func(){cleanup:=g.Close();for i:=0;i<g.Rows();i++{lastCleanup[i],_=g.Diagnostic(i)};if primary==nil{primary=cleanup}}()
 switch os.Args[1]{case "gtd-client":return client(g,os.Args[4:],qos);case "types-server":return server(g,os.Args[4:],qos);default:return fail(22)}
}
var lastCleanup [1]owned.Diagnostic
func main(){if err:=run();err!=nil{panic(err)}}
