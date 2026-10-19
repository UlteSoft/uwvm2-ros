//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1

// Authored canonical contracts, not executed or a native delivery witness.
// Copy the actual generated bounded_records.go to this module's customtypes
// subdirectory. No SDK stub, generated replacement or Native identity is made.
package main
import(ros "uwvm2.local/roswasm";r "example.local/control-record-contracts/customtypes";"bytes";"math")
const minI64 int64 = -1 << 63
const maxI64 int64 = 1 << 63 - 1
const maxU64 uint64 = 1 << 64 - 1
func need(v bool){if !v{panic("control record contract refused")}}
func status(err error)uint32{if err==nil{return 0};if e,ok:=err.(ros.Error);ok{return e.Code};panic("foreign codec error")}
func source(n int)r.Ros2Type_description_interfacesMsgTypeSource{v:=r.Ros2Type_description_interfacesMsgTypeSource{Type_name:"rcl_interfaces/msg/Log",Encoding:"msg",Raw_file_contents:make([]byte,n)};for i:=range v.Raw_file_contents{v.Raw_file_contents[i]=255};if n>0{v.Raw_file_contents[n-1]=0};return v}
func main(){
 need(r.DefaultRos2Type_description_interfacesSrvGetTypeDescription_Request().Include_type_sources)
 need(r.Ros2Rcl_interfacesMsgParameterType_PARAMETER_STRING==4)
 need(r.Ros2Type_description_interfacesSrvGetTypeDescriptionNativeType=="type_description_interfaces/srv/GetTypeDescription"&&r.Ros2Type_description_interfacesSrvGetTypeDescriptionCodecIdentity!=r.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodecIdentity)
 s:=source(6162);b,err:=s.Encode();need(err==nil);d,err:=r.DecodeRos2Type_description_interfacesMsgTypeSource(b);need(err==nil&&s.Type_name==d.Type_name&&s.Encoding==d.Encoding&&bytes.Equal(s.Raw_file_contents,d.Raw_file_contents)&&len(b)==12+len(s.Type_name)+len(s.Encoding)+6162)
 _,err=source(6163).Encode();need(status(err)==22)
 v:=r.Ros2Type_description_interfacesSrvGetTypeDescription_Response{Successful:true};for i:=0;i<12;i++{v.Type_sources=append(v.Type_sources,source(16))};b,err=v.Encode();need(err==nil);out,err:=r.DecodeRos2Type_description_interfacesSrvGetTypeDescription_Response(b);need(err==nil);for i:=range b{b[i]=0};need(out.Successful&&len(out.Type_sources)==12&&out.Type_sources[0].Type_name==v.Type_sources[0].Type_name&&bytes.Equal(out.Type_sources[0].Raw_file_contents,v.Type_sources[0].Raw_file_contents))
 v.Type_sources=nil;for i:=0;i<12;i++{v.Type_sources=append(v.Type_sources,source(3000))};b,err=v.Encode();need(status(err)==90&&b==nil)
 _,err=r.DecodeRos2Type_description_interfacesSrvGetTypeDescription_Response(make([]byte,32769));need(status(err)==90)
 s=source(1);s.Type_name="bad\x00text";_,err=s.Encode();need(status(err)==22)
 pv:=r.Ros2Rcl_interfacesMsgParameterValue{Type:4,Bool_value:true,Integer_value:minI64,Double_value:-1.25,String_value:"text",Byte_array_value:[]uint8{0,255},Bool_array_value:[]bool{true,false},Integer_array_value:[]int64{minI64,maxI64},Double_array_value:[]float64{2.5,-3.25},String_array_value:[]string{"a","b"}};b,err=pv.Encode();need(err==nil);p,err:=r.DecodeRos2Rcl_interfacesMsgParameterValue(b);need(err==nil&&p.Type==pv.Type&&p.Bool_value==pv.Bool_value&&p.Integer_value==pv.Integer_value&&p.Double_value==pv.Double_value&&p.String_value==pv.String_value&&bytes.Equal(p.Byte_array_value,pv.Byte_array_value)&&len(p.Bool_array_value)==2&&p.Bool_array_value[0]&&!p.Bool_array_value[1]&&p.Integer_array_value[0]==minI64&&p.Integer_array_value[1]==maxI64&&p.Double_array_value[1]==-3.25&&p.String_array_value[1]=="b")
 e:=r.Ros2Service_msgsMsgServiceEventInfo{Sequence_number:minI64};e.Client_gid[15]=255;b,err=e.Encode();need(err==nil);ed,err:=r.DecodeRos2Service_msgsMsgServiceEventInfo(b);need(err==nil&&ed==e)
 ir:=r.Ros2Rcl_interfacesMsgIntegerRange{From_value:minI64,To_value:maxI64,Step:maxU64};b,err=ir.Encode();need(err==nil);id,err:=r.DecodeRos2Rcl_interfacesMsgIntegerRange(b);need(err==nil&&id==ir)
 fr:=r.Ros2Rcl_interfacesMsgFloatingPointRange{Step:math.Inf(1)};_,err=fr.Encode();need(status(err)==22)
}
