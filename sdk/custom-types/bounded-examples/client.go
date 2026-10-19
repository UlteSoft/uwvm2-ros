//go:build tinygo && wasm && ((ros1 && !ros2) || (ros2 && !ros1))

// Complete caller-driven TinyGo custom Service/Action application. Copy the
// generated bounded_records.go into example.local/bounded/customtypes and build
// this file with exactly one aliases_ros1.go/aliases_ros2.go tag-selected file.
// The existing SDK Capability API owns every explicitly opened alias. Typed
// component codecs are complete; builtin Trigger/Fibonacci clients are unused.
// External basic stdin R controls at most200 receives; no clock/random/env
// capability is acquired. Inspect actual WASI startup imports before execution.
package main

import (
 "encoding/binary"
 "io"
 "os"
 ros "uwvm2.local/roswasm"
)

func invalid()error{return ros.Error{Code:22}}
// Check both write count and error; never turn a failed basic stream into PASS.
func marker(text string)error{n,err:=os.Stdout.Write([]byte(text));if err!=nil||n!=len(text){return ros.Error{Code:107}};return nil}
func sample()Envelope{return Envelope{Label:"bounded-native",Samples:[]uint16{3,5,8},Points:[]Point{{Value:-7,Enabled:true},{Value:11,Enabled:false}},Labels:[2]string{"first","second"}}}
func code(err error)uint32{if err==nil{return 0};if value,ok:=err.(ros.Error);ok{return value.Code};return 22}
func execute(caps []*ros.Capability)error{
 request:=Request{Sample:sample(),Limit:32};bytes,err:=request.Encode();if err!=nil{return err};output:=make([]byte,responseMax)
 n,err:=caps[0].Request(ros.OpServiceCall,bytes,output);if err!=nil{return err};response,err:=decodeResponse(output[:int(n)]);if err!=nil{return err}
 expected,err:=sample().Encode();if err!=nil{return err};actual,err:=response.Echoed.Encode();if err!=nil||!response.Accepted||string(actual)!=string(expected){return invalid()}
 bytes,err=(EmptyRequest{}).Encode();if err!=nil{return err};n,err=caps[1].Request(ros.OpServiceCall,bytes,nil);if err!=nil{return err};if _,err=decodeEmptyResponse(nil);err!=nil||n!=0{return invalid()}
 bytes,err=(Goal{Sample:sample()}).Encode();if err!=nil{return err};var id[8]byte;n,err=caps[2].Request(ros.OpActionSendGoal,bytes,id[:]);if err!=nil{return err};if n!=8||binary.LittleEndian.Uint64(id[:])==0{return invalid()}
 if err=marker("SDK_BOUNDED_GOAL_SENT\n");err!=nil{return err};gotFeedback:=false
 for pass:=0;pass<200;pass++{
  var gate[1]byte;if _,err=io.ReadFull(os.Stdin,gate[:]);err!=nil||gate[0]!='R'{return invalid()}
  if !gotFeedback{output=make([]byte,feedbackMax+8);n,err=caps[4].Request(ros.OpActionReceiveFeedback,id[:],output);if code(err)==11{if err=marker("SDK_BOUNDED_AGAIN\n");err!=nil{return err};continue};if err!=nil{return err};if n<8||binary.LittleEndian.Uint64(output[:8])!=binary.LittleEndian.Uint64(id[:]){return invalid()};value,e:=decodeFeedback(output[8:int(n)]);if e!=nil||value.Processed!=3||value.Phase!="accepted"{return invalid()};gotFeedback=true
   // Exact validated feedback is acknowledged once. The external controller
   // releases E before terminal delivery; no call, Goal or response is replayed.
   if err=marker("SDK_BOUNDED_FEEDBACK_DELIVERED\n");err!=nil{return err}
   if _,err=io.ReadFull(os.Stdin,gate[:]);err!=nil||gate[0]!='E'{return invalid()}
  }
  output=make([]byte,resultMax+9);n,err=caps[5].Request(ros.OpActionReceiveResult,id[:],output);if code(err)==11{if err=marker("SDK_BOUNDED_AGAIN\n");err!=nil{return err};continue};if err!=nil{return err};if n<9||binary.LittleEndian.Uint64(output[:8])!=binary.LittleEndian.Uint64(id[:])||output[8]!=2{return invalid()};value,e:=decodeResult(output[9:int(n)]);if e!=nil||!value.Accepted||value.Sum!=16||value.Note!="bounded-native-result"{return invalid()};return nil
 }
 return ros.Error{Code:110}
}
// Reverse checked cleanup retains the first error and attempts every owned role.
func run()(primary error){
 var caps []*ros.Capability
 defer func(){for i:=len(caps)-1;i>=0;i--{cleanup:=caps[i].Close();if primary==nil{primary=cleanup}}}()
 for _,alias:=range []string{"bounded-echo","bounded-empty","bounded-send","bounded-cancel","bounded-feedback","bounded-result"}{cap,err:=ros.Open(alias);if err!=nil{return err};caps=append(caps,cap)}
 return execute(caps)
}
func main(){err:=run();if err==nil{err=marker("PASS_BOUNDED_CUSTOM_CLIENT\n")};if err!=nil{os.Exit(int(code(err)))}}
