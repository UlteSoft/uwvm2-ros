//go:build tinygo && wasm && ((ros1 && !ros2) || (ros2 && !ros1))

// Executable private-owner TinyGo custom typed server composition. Copy this
// entry OR client.go (never both mains) with aliases_ros1/2.go and the fresh
// generated customtypes package into a temporary serial guest module. Four
// explicit advertise/respond aliases are independently opened and retired;
// pure handlers receive only complete typed data, no capabilities or native IDs.
package main
import("encoding/binary";"io";"os";ros "uwvm2.local/roswasm")
func invalid()error{return ros.Error{Code:22}}
func code(err error)uint32{if err==nil{return 0};if value,ok:=err.(ros.Error);ok{return value.Code};return 22}
func marker(text string)error{n,err:=os.Stdout.Write([]byte(text));if err!=nil||n!=len(text){return ros.Error{Code:107}};return nil}
func sample()Envelope{return Envelope{Label:"bounded-native",Samples:[]uint16{3,5,8},Points:[]Point{{Value:-7,Enabled:true},{Value:11,Enabled:false}},Labels:[2]string{"first","second"}}}
func same(value Envelope)error{actual,err:=value.Encode();if err!=nil{return err};expected,err:=sample().Encode();if err!=nil{return err};if string(actual)!=string(expected){return invalid()};return nil}
// Validate complete handler outputs before the first native response effect.
func handleService(value Request)(Response,error){if value.Limit!=32{return Response{},invalid()};if err:=same(value.Sample);err!=nil{return Response{},err};return Response{Accepted:true,Echoed:value.Sample},nil}
func handleGoal(value Goal)(Feedback,ResultValue,error){if err:=same(value.Sample);err!=nil{return Feedback{},ResultValue{},err};return Feedback{Processed:3,Phase:"accepted"},ResultValue{Accepted:true,Sum:16,Note:"bounded-native-result"},nil}
func frame(id uint64,kind byte,terminal bool,body []byte)([]byte,error){if id==0{return nil,invalid()};prefix:=8;if terminal{prefix=9};out:=make([]byte,prefix+len(body));binary.LittleEndian.PutUint64(out,id);if terminal{out[8]=kind};copy(out[prefix:],body);return out,nil}
func respond(cap *ros.Capability,operation ros.Operation,body []byte)error{n,err:=cap.Request(operation,body,nil);if err!=nil{return err};if n!=0{return invalid()};return nil}
func execute(caps []*ros.Capability)error{
 if err:=caps[0].AdvertiseService();err!=nil{return err};if err:=respond(caps[2],ros.OpActionAdvertise,nil);err!=nil{return err};if err:=marker("SDK_BOUNDED_SERVERS_READY\n");err!=nil{return err};services,goals:=0,0
 for pass:=0;pass<200;pass++{
  var gate[1]byte;if _,err:=io.ReadFull(os.Stdin,gate[:]);err!=nil||gate[0]!='R'{return invalid()}
  if services==0{wire:=make([]byte,requestMax+8);n,err:=caps[0].Poll(wire);if code(err)!=11{if err!=nil{return err};if n<8{return invalid()};id:=binary.LittleEndian.Uint64(wire[:8]);value,err:=decodeRequest(wire[8:int(n)]);if err!=nil{return err};answer,err:=handleService(value);if err!=nil{return err};body,err:=answer.Encode();if err!=nil{return err};body,err=frame(id,0,false,body);if err!=nil{return err};if err=respond(caps[1],ros.OpServiceRespond,body);err!=nil{return err};services++}}
  if goals==0{wire:=make([]byte,goalMax+8);n,err:=caps[2].Poll(wire);if code(err)!=11{if err!=nil{return err};if n<8{return invalid()};id:=binary.LittleEndian.Uint64(wire[:8]);value,err:=decodeGoal(wire[8:int(n)]);if err!=nil{return err};feedback,result,err:=handleGoal(value);if err!=nil{return err};fb,err:=feedback.Encode();if err!=nil{return err};rp,err:=result.Encode();if err!=nil{return err};fb,err=frame(id,1,true,fb);if err!=nil{return err};rp,err=frame(id,2,true,rp);if err!=nil{return err};if err=respond(caps[3],ros.OpActionRespond,fb);err!=nil{return err}
   // Keep the exact Goal and both owners until independent native feedback
   // delivery releases E. A successful publish does not prove remote receipt.
   if err=marker("SDK_BOUNDED_FEEDBACK_SENT\n");err!=nil{return err}
   if _,err=io.ReadFull(os.Stdin,gate[:]);err!=nil||gate[0]!='E'{return invalid()}
   if err=respond(caps[3],ros.OpActionRespond,rp);err!=nil{return err};goals++}}
  if services==1&&goals==1{if err:=marker("SDK_BOUNDED_REPLIES_SENT\n");err!=nil{return err};var done[1]byte;if _,err:=io.ReadFull(os.Stdin,done[:]);err!=nil||done[0]!='D'{return invalid()};return nil};if err:=marker("SDK_BOUNDED_AGAIN\n");err!=nil{return err}
 }
 return ros.Error{Code:110}
}
func run()(primary error){var caps []*ros.Capability;defer func(){for i:=len(caps)-1;i>=0;i--{cleanup:=caps[i].Close();if primary==nil{primary=cleanup}}}();for _,alias:=range []string{"bounded-service-events","bounded-service-replies","bounded-action-events","bounded-action-replies"}{cap,err:=ros.Open(alias);if err!=nil{return err};caps=append(caps,cap)};return execute(caps)}
func main(){err:=run();if err==nil{err=marker("PASS_BOUNDED_CUSTOM_SERVER\n")};if err!=nil{os.Exit(int(code(err)))}}
