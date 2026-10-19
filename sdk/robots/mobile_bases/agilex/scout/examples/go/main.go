//go:build tinygo && (wasm || tinygo.wasm) && ((ros1 && !ros2) || (ros2 && !ros1))

// Real TinyGo WASI entry. Compile one backend; no native CAN/ROS Go package.
package main
import ("os";ros "uwvm2.local/roswasm")
// Outcome keeps complete typed values and cleanup distinct from primary errors.
type Outcome struct {Primary,Cleanup uint32;Value *Status}
func code(err error)uint32 {if err==nil{return 0};if e,ok:=err.(ros.Error);ok{return e.Code};return 22}
func readOnce(alias string)Outcome {
 var bytes [statusCapacity]byte
 cap,err:=ros.Open(alias);if err!=nil{return Outcome{Primary:code(err)}}
 var value *Status
 err=cap.Subscribe()
 if err==nil {var n uint64;n,err=cap.Poll(bytes[:]);if err==nil {
  if n>uint64(len(bytes)){err=ros.Error{Code:22}}else{
   var complete Status;complete,err=decodeStatus(bytes[:int(n)]);if err==nil{value=&complete}
  }
 }}
 primary:=code(err);cleanup:=code(cap.Close());if primary==0{primary=cleanup}
 return Outcome{Primary:primary,Cleanup:cleanup,Value:value}
}
func main(){if len(os.Args)!=2{os.Exit(22)};result:=readOnce(os.Args[1]);status:=result.Primary;if status>125{status=125};os.Exit(int(status))}
