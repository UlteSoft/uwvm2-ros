//go:build tinygo && (wasm || tinygo.wasm) && ((ros1 && !ros2) || (ros2 && !ros1))

// Actual TinyGo WASI command, no host Go/native ROS substitute.
package main
import("io";"os";ros "uwvm2.local/roswasm")
var received uint32
var denied bool
func status(e error)uint32{if e==nil{return 0};if value,ok:=e.(ros.Error);ok{return value.Code};return 22}
// The complete decoder has already validated bounds/Unicode/finite values.
func full(v CIOState)bool{
 if !header(v)||v.Header.Frame_id!="xarm-gpio"||v.State!=1||v.Code!=0||v.Input_digitals!=([2]uint16{165,32769})||v.Output_digitals!=([2]uint16{90,16386})||v.Input_analogs!=([2]float32{1.25,2.5})||v.Output_analogs!=([2]float32{3.75,0.5}){return false}
 for i,x:=range v.Input_conf{if x!=uint8(i)||v.Output_conf[i]!=uint8(15-i){return false}};return true
}
func run()(first uint32){
 group,e:=ros.NewSession(1);if e!=nil{return status(e)}
 var cap *ros.Capability
 defer func(){
  if cap!=nil{code:=status(cap.Close());if first==0{first=code}}
  for _,e:=range group.Close(){if first==0{first=status(e)}}
 }()
 // Session validates exact ABI and pointer model. The one direct capability
 // is explicitly owned here; it is not falsely reported as a group registry.
 cap,e=ros.Open("xarm-cio-state")
 if mode==0{denied=cap==nil&&status(e)==13;if denied{return 0};if e!=nil{return status(e)};return 22}
 if e!=nil{return status(e)};if maximum<1||maximum>32768{return 90}
 if e=cap.Subscribe();e!=nil{return status(e)}
 scratch:=make([]byte,maximum)
 for pass:=0;pass<64;pass++{
  var gate [1]byte;if _,e=io.ReadFull(os.Stdin,gate[:]);e!=nil||gate[0]!='R'{return 22}
  n,e:=cap.Poll(scratch);if status(e)==11{continue};if e!=nil{return status(e)};if n>uint64(len(scratch)){return 22}
  value,e:=decode(scratch[:int(n)]);if e!=nil||!full(value){return 22};received++;return 0
 }
 return 110
}
func main(){
 code:=run();if code==0&&((mode==0&&(!denied||received!=0))||(mode==1&&received!=1)){code=22}
 if code==0{marker:="PASS_XARM_CIO_STATE_DEFAULT_DENIED\n";if mode==1{marker="PASS_XARM_CIO_STATE_FULL_VALUE\n"};n,e:=io.WriteString(os.Stdout,marker);if e!=nil||n!=len(marker){code=107}}
 if code!=0{os.Exit(int(code))}
}
