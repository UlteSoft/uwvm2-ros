//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && rmw_service_v1
package main
import (
 "bytes"
 "encoding/json"
 "io"
 "unicode/utf8"
 records "uwvm2.local/groot-example/customtypes"
)
// Variable shape slices exist only during private JSON admission: decoding JSON
// straight into a fixed Go array can silently discard excess elements. These
// explicit source fields prevent projection/truncation before canonical encode.
type imageInput struct{Key string `json:"key"`;Shape []uint32 `json:"shape"`;Pixels []uint32 `json:"pixels"`}
type tensorInput struct{Key string `json:"key"`;Shape []uint32 `json:"shape"`;Values []float32 `json:"values"`}
type textInput struct{Key string `json:"key"`;Shape []uint32 `json:"shape"`;Texts []string `json:"texts"`}
type requestInput struct{Nonce uint64 `json:"application_nonce"`;Video []imageInput `json:"video"`;State []tensorInput `json:"state"`;Language []textInput `json:"language"`}
func jsonValue(d *json.Decoder,depth int,nodes *int)error{
 *nodes = *nodes + 1;if depth>16||*nodes>16384{return failure(90)};token,err:=d.Token();if err!=nil{return err}
 delimiter,ok:=token.(json.Delim);if !ok{return nil}
 switch delimiter{
 case '{':seen:=map[string]bool{};for d.More(){key,err:=d.Token();if err!=nil{return err};name,ok:=key.(string);if !ok||seen[name]||!exactKey(name){return failure(22)};seen[name]=true;if err=jsonValue(d,depth+1,nodes);err!=nil{return err}};last,err:=d.Token();if err!=nil||last!=json.Delim('}'){return failure(22)}
 case '[':for d.More(){if err=jsonValue(d,depth+1,nodes);err!=nil{return err}};last,err:=d.Token();if err!=nil||last!=json.Delim(']'){return failure(22)}
 default:return failure(22)
 };return nil
}
func decodeInput(raw []byte)(out records.Ros2Uwvm_groot_bridgeSrvInferAction_Request,err error){
 if len(raw)>1048576{return out,failure(90)};if !strictText(raw){return out,failure(22)}
 scan:=json.NewDecoder(bytes.NewReader(raw));scan.UseNumber();nodes:=0;if err=jsonValue(scan,0,&nodes);err!=nil{return out,err};if _,err=scan.Token();err!=io.EOF{return out,failure(22)}
 d:=json.NewDecoder(bytes.NewReader(raw));d.DisallowUnknownFields();var input requestInput
 if err=d.Decode(&input);err!=nil{return out,err};var trailing any;if err=d.Decode(&trailing);err!=io.EOF{return out,failure(22)}
 if len(input.Video)>3||len(input.State)>4||len(input.Language)>2{return out,failure(90)}
 next:=records.Ros2Uwvm_groot_bridgeSrvInferAction_Request{Application_nonce:input.Nonce}
 next.Video=make([]records.Ros2Uwvm_groot_bridgeMsgRGBFrame,len(input.Video))
 for i,value:=range input.Video{if len(value.Shape)!=5{return out,failure(22)};if len(value.Pixels)>768{return out,failure(90)};if !shapeCount(value.Shape,len(value.Pixels),768)||value.Shape[0]!=1||value.Shape[4]!=3{return out,failure(22)};v:=&next.Video[i];v.Key=value.Key;copy(v.Shape[:],value.Shape);v.Pixels=make([]uint8,len(value.Pixels));for j,x:=range value.Pixels{if x>255{return out,failure(22)};v.Pixels[j]=uint8(x)}}
 next.State=make([]records.Ros2Uwvm_groot_bridgeMsgFloatTensor,len(input.State))
 for i,value:=range input.State{if len(value.Shape)!=3{return out,failure(22)};if len(value.Values)>320{return out,failure(90)};if !shapeCount(value.Shape,len(value.Values),320)||value.Shape[0]!=1{return out,failure(22)};v:=&next.State[i];v.Key=value.Key;copy(v.Shape[:],value.Shape);v.Values=value.Values}
 next.Language=make([]records.Ros2Uwvm_groot_bridgeMsgTextSequence,len(input.Language))
 for i,value:=range input.Language{if len(value.Shape)!=2{return out,failure(22)};if len(value.Texts)>4{return out,failure(90)};if !shapeCount(value.Shape,len(value.Texts),4)||value.Shape[0]!=1||value.Shape[1]!=1{return out,failure(22)};v:=&next.Language[i];v.Key=value.Key;copy(v.Shape[:],value.Shape);v.Texts=value.Texts}
 if _,err=next.Encode();err!=nil{return out,err};return next,nil
}

func exactKey(s string)bool{switch s{case "application_nonce","video","state","language","key","shape","pixels","values","texts":return true};return false}
func hex4(b []byte)(uint16,bool){if len(b)<4{return 0,false};var n uint16;for _,c:=range b[:4]{var v byte;switch{case c>='0'&&c<='9':v=c-'0';case c>='a'&&c<='f':v=c-'a'+10;case c>='A'&&c<='F':v=c-'A'+10;default:return 0,false};n=n*16+uint16(v)};return n,true}
func strictText(raw []byte)bool{
 if !utf8.Valid(raw){return false};inside:=false
 for i:=0;i<len(raw);i++{c:=raw[i];if c=='"'{inside=!inside;continue};if !inside||c!='\\'{continue};i++;if i>=len(raw){return false};if raw[i]!='u'{continue};if i+4>=len(raw){return false};v,ok:=hex4(raw[i+1:]);if !ok{return false};i+=4
  if v>=0xd800&&v<=0xdbff{if i+6>=len(raw)||raw[i+1]!='\\'||raw[i+2]!='u'{return false};low,ok:=hex4(raw[i+3:]);if !ok||low<0xdc00||low>0xdfff{return false};i+=6}else if v>=0xdc00&&v<=0xdfff{return false}
 };return !inside
}

func shapeCount(shape []uint32,count int,maximum uint64)bool{n:=uint64(1);for _,d:=range shape{if d==0||n>maximum/uint64(d){return false};n*=uint64(d)};return n==uint64(count)}
