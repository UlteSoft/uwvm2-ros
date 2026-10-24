// Stage as main.go beside records/{bounded_records.go,sport.go}. This checks
// actual-Wasm local state only; it grants no ROS rights and runs no native peer.
package main
import (
    "fmt"
    vendor "example.local/unitree-sport/records"
    ros "uwvm2.local/roswasm"
)
var apis=[]int64{1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1015,1016,1017,1020,1022,1023,1027,1028,1029,1030,1031,1032,1036,1061,1062,1063,2041,2043,2044,2045,2046,2047,2048,2049,2050,2051,2054,2055,2058}
func require(ok bool){if !ok{panic("Sport pending invariant")}}
func status(err error)uint32{if err==nil{return 0};if value,ok:=err.(ros.Error);ok{return value.Code};return 0xffffffff}
func request(id,api int64)vendor.Ros2Unitree_apiMsgRequest{
    var value vendor.Ros2Unitree_apiMsgRequest;value.Header.Identity.Id=id;value.Header.Identity.Api_id=api
    value.Header.Lease.Id=-9007199254740993;value.Header.Policy.Priority=-101
    value.Parameter="{\"complete\":\"λ\"}";value.Binary=[]byte{0,255,1,128};return value
}
func response(id,api int64)vendor.Ros2Unitree_apiMsgResponse{
    var value vendor.Ros2Unitree_apiMsgResponse;value.Header.Identity.Id=id;value.Header.Identity.Api_id=api
    value.Header.Status.Code=-2147483647;value.Data="{\"data\":false,\"full\":\"λ\"}";value.Binary=[]int8{0,-1,1,-128};return value
}
func main(){
    var pending vendor.SportPending
    for offset,api:=range apis{
        id:=int64(9007199254740993)+int64(offset);req:=request(id,api)
        require(pending.Register(req,7,11)==nil);require(pending.PendingCount()==1)
        require(status(pending.Register(req,7,11))==17)
        copied:=pending;require(status(copied.Register(request(id+1,api),7,11))==9)
        reply:=response(id,api);require(status(pending.Check(response(id,api+90000),8))==9)
        require(pending.Check(reply,10)==nil);_,err:=pending.Accept(reply,11);require(status(err)==110)
        invalid:=reply;invalid.Data=string(make([]byte,4097));_,err=pending.Accept(invalid,10);require(status(err)==22)
        require(pending.PendingCount()==1);accepted,err:=pending.Accept(reply,10);require(err==nil)
        require(accepted.Header==reply.Header&&accepted.Data==reply.Data&&len(accepted.Binary)==4)
        require(accepted.Binary[1]==-1&&accepted.Binary[3]==-128);reply.Binary[0]=7;require(accepted.Binary[0]==0)
        _,err=pending.Accept(accepted,10);require(status(err)==9);require(status(pending.Register(req,7,11))==17)
    }
    const max=^uint64(0)
    for id:=int64(9007199254741032);id<9007199254741048;id++{require(pending.Register(request(id,1003),0,max)==nil)}
    next:=request(9007199254741048,1003);require(status(pending.Register(next,0,max))==11)
    require(pending.PendingCount()==16);require(pending.Expire(9007199254741032,1003,max-1)==11)
    require(pending.Expire(9007199254741032,1003,max)==110);require(pending.Register(next,0,max)==nil)
    require(pending.Forget(next.Header.Identity.Id,1003)==nil)
    invalid:=request(9223372036854775807,1003);invalid.Header.Policy.Noreply=true
    require(status(pending.Register(invalid,0,1))==22);invalid.Header.Policy.Noreply=false
    invalid.Header.Identity.Api_id=65536+1003;require(status(pending.Register(invalid,0,1))==22)
    invalid.Header.Identity.Api_id=1003;require(status(pending.Register(invalid,1,1))==22)
    require(pending.Register(invalid,max-1,max)==nil);require(pending.Forget(invalid.Header.Identity.Id,1003)==nil)
    require(status(pending.Register(invalid,0,1))==17)
    pending.Close();require(pending.PendingCount()==0);require(status(pending.Register(invalid,0,1))==9)
    require(pending.Expire(9007199254741033,1003,max)==9)
    fmt.Println("SPORT_PENDING_OK operations=39 capacity=16 identity_bits=64")
}
