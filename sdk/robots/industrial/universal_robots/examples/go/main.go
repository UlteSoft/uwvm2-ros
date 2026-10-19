//go:build tinygo && (wasm || tinygo.wasm) && ((urros1 && ros1 && !ros2 && !urros2 && !polyscopex) || (urros2 && ros2 && !ros1 && !urros1 && !polyscopex) || (polyscopex && ros2 && !ros1 && !urros1 && !urros2))

// Real TinyGo WASI entry. Compile one backend; no native CAN/ROS Go package.
package main
import ("os";ros "uwvm2.local/roswasm")
// Outcome keeps complete typed values and cleanup distinct from primary errors.
type Outcome struct {Primary,Cleanup uint32;Value *IOState}
func code(err error)uint32 {if err==nil{return 0};if e,ok:=err.(ros.Error);ok{return e.Code};return 22}
func readOnce(alias string)Outcome {
 var bytes [ioCapacity]byte
 cap,err:=ros.Open(alias);if err!=nil{return Outcome{Primary:code(err)}}
 var value *IOState
 err=cap.Subscribe()
 if err==nil {var n uint64;n,err=cap.Poll(bytes[:]);if err==nil {
  if n>uint64(len(bytes)){err=ros.Error{Code:22}}else{
   var complete IOState;complete,err=decodeIOState(bytes[:int(n)]);if err==nil{value=&complete}
  }
 }}
 primary:=code(err);cleanup:=code(cap.Close());
 return Outcome{Primary:primary,Cleanup:cleanup,Value:value}
}
func main(){if len(os.Args)!=2{os.Exit(22)};result:=readOnce(os.Args[1]);status:=result.Primary;if status==0{status=result.Cleanup};if status>125{status=125};os.Exit(int(status))}
