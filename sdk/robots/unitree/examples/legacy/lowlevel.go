//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2

// Actual ROS1 TinyGo SDK owners and complete generated records. No goroutine,
// native ROS, timer, external endpoint, packet struct or implicit permission.
package main
import (
 roswasm "uwvm2.local/roswasm"
 vendor "example.local/uwvm-unitree-legacy/customtypes"
)
var node *roswasm.Node
var publisher *roswasm.Publisher
var subscription *roswasm.Subscription
var started,closed,submitted bool
var lastStatus,polls,received,denials uint32
var scratch [1536]byte
var observed []byte
func code(err error) uint32 {if err==nil{return 0};if e,ok:=err.(roswasm.Error);ok{return e.Code};return 22}
func closeCase(first uint32) uint32 {
 if closed {if first!=0{return first};return 9};closed=true
 if subscription!=nil {c:=code(subscription.Close());if first==0{first=c};subscription=nil}
 if publisher!=nil {c:=code(publisher.Close());if first==0{first=c};publisher=nil}
 if node!=nil {for _,err:=range node.Close(){if first==0{first=code(err)}}}
 lastStatus=first;return first
}
func initializeCase(mode,family,reserved uint32) uint32 {
 if started||closed{return 9};started=true;if mode>1||family!=4||reserved!=0{return closeCase(22)}
 node=roswasm.NewNode("legacy-typed-example")
 if mode==0 {
  for _,alias:=range []string{"legacy-lowcmd","legacy-lowstate"}{
   owner,err:=roswasm.Open(alias);if code(err)!=13||owner!=nil {if owner!=nil{_ =owner.Close()};if err!=nil{return closeCase(code(err))};return closeCase(22)};denials++
  };return closeCase(0)
 }
 var err error;publisher,err=node.CreatePublisher("legacy-lowcmd");if err!=nil{return closeCase(code(err))}
 // CreateSubscription invokes actual topic_subscribe before parent M.
 subscription,err=node.CreateSubscription("legacy-lowstate");if err!=nil{return closeCase(code(err))};return 0
}
func pollOnce() uint32 {
 if !started||closed||received!=0{return 9};if polls>=4096{return closeCase(110)};polls++
 n,err:=subscription.Poll(scratch[:]);if code(err)==11{return 11};if err!=nil{return closeCase(code(err))};if n>uint64(len(scratch)){return closeCase(22)}
 value,err:=vendor.DecodeRos1Unitree_legged_msgsLowState(scratch[:int(n)]);if err!=nil{return closeCase(code(err))}
 actual,err:=value.Encode();if err!=nil{return closeCase(code(err))};expected,err:=expectedState().Encode();if err!=nil{return closeCase(code(err))}
 if len(actual)==0||len(actual)>1536||len(actual)!=len(expected)||len(actual)!=int(n){return closeCase(22)}
 for i:=range actual {if actual[i]!=expected[i]||actual[i]!=scratch[i]{return closeCase(22)}}
 observed=actual;received=1;return 0
}
func submitOnce() uint32 {if !started||closed||received!=1||submitted{return 9};if err:=publisher.PublishMessage(expectedCommand());err!=nil{return closeCase(code(err))};submitted=true;return 0}
func observedByte(index uint32) uint32 {if received==0||index>=uint32(len(observed)){lastStatus=22;return 0};return uint32(observed[index])}
