#!/usr/bin/env python3
"""Generate dependency-free Rust and Go codec checks from shared ABI vectors.

This preparation tool does not compile code or execute a ROS guest. The checked-in
expected bytes come from sdk/origin/abi/wire-vectors-v1.json, independently of the codecs.
Run language compilation/tests in the authorized cgroup environment.
"""
from pathlib import Path
import argparse
import json
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--fixtures',type=Path,required=True,help='Exact copied language-neutral ABI JSON')
parser.add_argument('--sdk-root',type=Path,required=True,help='Installed/source SDK root that must remain free of generated tests')
parser.add_argument('--out',type=Path,required=True,help='Fresh owned unified-test source/artifact directory outside the SDK')
args=parser.parse_args()
sdk_root=args.sdk_root.resolve(strict=True)
out=args.out.absolute()
if out.exists() or out.is_relative_to(sdk_root) or out.parent.resolve(strict=True)!=out.parent:
    parser.error('fresh ordinary output outside the SDK required; tests belong to test/0073.ros-tests')
data=json.loads(args.fixtures.read_text())
def rq(s):return json.dumps(s,ensure_ascii=False).replace('\\u0000','\\u{0}')
def gq(s):return json.dumps(s,ensure_ascii=False)
def rf(v):return '-0.0f64' if v=='negative-zero' else str(float(v))+'f64'
def gf(v):return 'math.Copysign(0, -1)' if v=='negative-zero' else str(float(v))
kinds={'bool':('Bool','Bool'),'int32':('Int32','Int32'),'int64':('Int64','Int64'),'float64':('Float64','Float64'),'string':('String','Text'),'byte-array':('ByteArray','Bytes'),'bool-array':('BoolArray','Bools'),'int32-array':('Int32Array','Int32s'),'int64-array':('Int64Array','Int64s'),'float64-array':('Float64Array','Float64s'),'string-array':('StringArray','Strings')}
tagkinds={1:'bool',2:'int64',3:'float64',4:'string',5:'byte-array',6:'bool-array',7:'int64-array',8:'float64-array',9:'string-array'}
def rp(k,v):
 a,_=kinds[k]
 if k=='bool':x=str(v).lower()
 elif k in ('int32','int64'):x=str(v)+('i32' if k=='int32' else 'i64')
 elif k=='float64':x=rf(v)
 elif k=='string':x='String::from('+rq(v)+')'
 elif k.endswith('array'):
  scalar=k[:-6]
  if scalar in ('int32','int64'):items=[str(x)+('i32' if scalar=='int32' else 'i64') for x in v]
  elif scalar=='float64':items=[rf(x) for x in v]
  elif scalar=='string':items=['String::from('+rq(x)+')' for x in v]
  elif scalar=='bool':items=[str(x).lower() for x in v]
  else:items=[str(x)+'u8' for x in v]
  x='vec!['+', '.join(items)+']'
 return 'ParameterValue::'+a+'('+x+')'
def gp(k,v):
 a,f=kinds[k]
 if k=='bool':x=str(v).lower()
 elif k in ('int32','int64'):x=str(v)
 elif k=='float64':x=gf(v)
 elif k=='string':x=gq(v)
 elif k.endswith('array'):
  scalar=k[:-6];ty={'byte':'byte','bool':'bool','int32':'int32','int64':'int64','float64':'float64','string':'string'}[scalar]
  if scalar=='float64':items=[gf(x) for x in v]
  elif scalar=='string':items=[gq(x) for x in v]
  elif scalar=='bool':items=[str(x).lower() for x in v]
  else:items=[str(x) for x in v]
  x='[]'+ty+'{'+', '.join(items)+'}'
 return 'ParameterValue{Kind: Parameter'+a+', '+f+': '+x+'}'
r=['//! Generated from language-neutral wire fixtures; compilation/execution is a separate verdict.','#![cfg(feature = "alloc")]','use roswasm::{codec, typed::*};','use std::string::String;','fn bytes(h: &str) -> Vec<u8> { (0..h.len()).step_by(2).map(|i| u8::from_str_radix(&h[i..i+2],16).unwrap()).collect() }','#[test]','fn shared_positive_vectors() {']
g=['// Golden codec vectors are independent expected bytes from the common ABI fixtures.','// Generating this test is not a compilation/runtime verdict.','package roswasm','import("encoding/hex";"math";"testing")','func fixture(s string)[]byte{b,err:=hex.DecodeString(s);if err!=nil{panic(err)};return b}','func assertBytes(t *testing.T,b,want []byte,err error){t.Helper();if err!=nil{t.Fatal(err)};if hex.EncodeToString(b)!=hex.EncodeToString(want){t.Fatalf("got %x want %x",b,want)}}','func TestSharedPositiveVectors(t *testing.T){']
for index,row in enumerate(data['positive']):
 k=row['codec'];v=row['value'];hexv=row['hex'];r+=['    { // '+row['name'],'        let expected = bytes('+rq(hexv)+');']
 g+=['t.Run('+gq(row['name'])+',func(t *testing.T){want:=fixture('+gq(hexv)+');']
 if k=='string':
  r+=['let value = String::from('+rq(v)+'); assert_eq!(value.encode().unwrap(),expected); assert_eq!(String::decode(&expected).unwrap(),value);']
  g+=['b,err:=String('+gq(v)+').Encode();assertBytes(t,b,want,err);decoded,err:=DecodeString(want);if err!=nil||decoded!='+gq(v)+'{t.Fatal("string decode",err)}']
 elif k=='bool' or k=='set-bool-request':
  r+=['let mut b=[0u8;1];codec::encode_bool('+str(v).lower()+', &mut b).unwrap();assert_eq!(b.as_slice(),expected);assert_eq!(codec::decode_bool(&expected).unwrap(),'+str(v).lower()+');']
  g+=['b:=make([]byte,1);_,err:=EncodeBool('+str(v).lower()+',b);assertBytes(t,b,want,err);decoded,err:=DecodeBool(want);if err!=nil||decoded!='+str(v).lower()+'{t.Fatal("bool decode",err)}']
 elif k.startswith('parameter-') and k not in ('parameter-event',):
  pk=k[10:];rn,_=kinds[pk]
  r+=['let value='+rp(pk,v)+';assert_eq!(value.encode().unwrap(),expected);assert_eq!(ParameterValue::decode(ParameterKind::'+rn+', &expected).unwrap().encode().unwrap(),expected);']
  g+=['v:='+gp(pk,v)+';b,err:=v.Encode();assertBytes(t,b,want,err);decoded,err:=DecodeParameter(Parameter'+rn+',want);if err!=nil{t.Fatal(err)};b,err=decoded.Encode();assertBytes(t,b,want,err)']
 elif k=='twist':
  r+=['let value=['+', '.join(rf(x) for x in v)+'];assert_eq!(value.encode().unwrap(),expected);assert_eq!(<[f64;6] as Canonical>::decode(&expected).unwrap().encode().unwrap(),expected);']
  g+=['v:=Twist{'+', '.join(gf(x) for x in v)+'};b,err:=v.Encode();assertBytes(t,b,want,err);decoded,err:=DecodeTwist(want);if err!=nil{t.Fatal(err)};b,err=Twist(decoded).Encode();assertBytes(t,b,want,err)']
 elif k in ('image','transform'):
  frame=v['frame_id'] if k=='image' else v['parent_frame']
  rh='Header{sequence:'+str(v['sequence'])+',seconds:'+str(v['seconds'])+',nanoseconds:'+str(v['nanoseconds'])+',frame:String::from('+rq(frame)+')}'
  gh='Header{Sequence:'+str(v['sequence'])+',Seconds:'+str(v['seconds'])+',Nanoseconds:'+str(v['nanoseconds'])+',Frame:'+gq(frame)+'}'
  if k=='image':
   rv='Image{header:'+rh+',height:'+str(v['height'])+',width:'+str(v['width'])+',encoding:String::from('+rq(v['encoding'])+'),big_endian:'+str(v['bigendian']).lower()+',step:'+str(v['step'])+',data:vec!['+','.join(map(str,v['data']))+']}'
   gv='Image{Header:'+gh+',Height:'+str(v['height'])+',Width:'+str(v['width'])+',Encoding:'+gq(v['encoding'])+',BigEndian:'+str(v['bigendian']).lower()+',Step:'+str(v['step'])+',Data:[]byte{'+','.join(map(str,v['data']))+'}}';ty='Image'
  else:
   vals=v['translation']+v['quaternion'];rv='Transform{header:'+rh+',child_frame:String::from('+rq(v['child_frame'])+'),values:['+','.join(rf(x) for x in vals)+']}'
   gv='Transform{Header:'+gh+',ChildFrame:'+gq(v['child_frame'])+',Values:[7]float64{'+','.join(gf(x) for x in vals)+'}}';ty='Transform'
  r+=['let value='+rv+';assert_eq!(value.encode().unwrap(),expected);assert_eq!('+ty+'::decode(&expected).unwrap().encode().unwrap(),expected);']
  g+=['v:='+gv+';b,err:=v.Encode();assertBytes(t,b,want,err);decoded,err:=Decode'+ty+'(want);if err!=nil{t.Fatal(err)};b,err=decoded.Encode();assertBytes(t,b,want,err)']
 elif k=='trigger-request':
  r+=['assert_eq!(codec::encode_trigger_request().as_slice(),expected);codec::decode_trigger_request(&expected).unwrap();']
  g+=['assertBytes(t,EncodeTriggerRequest(),want,nil);if err:=DecodeTriggerRequest(want);err!=nil{t.Fatal(err)}']
 elif k=='service-response':
  r+=['let value=codec::ServiceResponse{success:'+str(v['success']).lower()+',message:'+rq(v['message'])+'};let mut b=[0u8;64];let n=codec::encode_service_response(&value,&mut b).unwrap();assert_eq!(&b[..n],expected);assert_eq!(codec::decode_service_response(&expected).unwrap(),value);']
  g+=['v:=ServiceResponse{Success:'+str(v['success']).lower()+',Message:'+gq(v['message'])+'};b:=make([]byte,64);n,err:=EncodeServiceResponse(v,b);assertBytes(t,b[:n],want,err);decoded,err:=DecodeServiceResponse(want);if err!=nil||decoded!=v{t.Fatal("response decode",err)}']
 elif k=='service-request-event':
  r+=['let(id,value)=codec::decode_set_bool_event(&expected).unwrap();assert_eq!(id.raw(),'+v['request_id']+'u64);assert!(value);']
  g+=['id,v,err:=DecodeSetBoolEvent(want);if err!=nil||id.Raw()!='+v['request_id']+'||!v{t.Fatal("event decode",err)}']
 elif k=='service-response-envelope':
  r+=['let id=codec::decode_trigger_event(&expected[..8]).unwrap();let response=codec::decode_service_response(&expected[8..]).unwrap();let mut b=[0u8;64];let n=codec::encode_service_reply(id,&response,&mut b).unwrap();assert_eq!(&b[..n],expected);assert_eq!(id.raw(),'+v['request_id']+'u64);']
  g+=['id,err:=DecodeTriggerEvent(want[:8]);if err!=nil{t.Fatal(err)};response,err:=DecodeServiceResponse(want[8:]);if err!=nil{t.Fatal(err)};b:=make([]byte,64);n,err:=EncodeServiceReply(id,response,b);assertBytes(t,b[:n],want,err);if id.Raw()!='+v['request_id']+'{t.Fatal("ID precision")}']
 elif k=='fibonacci-goal':
  r+=['let mut b=[0u8;4];codec::encode_fibonacci_goal('+str(v)+',&mut b).unwrap();assert_eq!(b.as_slice(),expected);']
  g+=['b:=make([]byte,4);_,err:=EncodeFibonacciGoal('+str(v)+',b);assertBytes(t,b,want,err)']
 elif k=='goal-id':
  r+=['let id=codec::decode_goal_id(&expected).unwrap();assert_eq!(id.raw(),'+v+'u64);assert_eq!(id.bytes().as_slice(),expected);']
  g+=['id,err:=DecodeGoalID(want);if err!=nil||id.Raw()!='+v+'{t.Fatal("ID precision",err)};b:=id.Bytes();assertBytes(t,b[:],want,nil)']
 elif k=='incoming-goal':
  r+=['let(id,order)=codec::decode_fibonacci_goal_event(&expected).unwrap();assert_eq!(id.raw(),'+v['goal_id']+'u64);assert_eq!(order,'+str(v['order'])+');']
  g+=['id,order,err:=DecodeFibonacciGoalEvent(want);if err!=nil||id.Raw()!='+v['goal_id']+'||order!='+str(v['order'])+'{t.Fatal("incoming goal",err)}']
 elif k in ('action-feedback','action-result','action-response'):
  dec={'action-feedback':'feedback','action-result':'result','action-response':'reply'}[k];go={'feedback':'Feedback','result':'Result','reply':'Reply'}[dec]
  r+=['let value=codec::decode_fibonacci_'+dec+'(&expected).unwrap();assert_eq!(value.goal.raw(),'+v['goal_id']+'u64);assert_eq!(value.values().collect::<Vec<_>>(),vec!['+','.join(map(str,v['values']))+']);let mut b=[0u8;256];']
  g+=['v,err:=DecodeFibonacci'+go+'(want);if err!=nil||v.Goal.Raw()!='+v['goal_id']+'{t.Fatal("action decode",err)};b:=make([]byte,256);']
  if k=='action-feedback':
   r+=['let n=codec::encode_fibonacci_feedback(value.goal,&value.values().collect::<Vec<_>>(),&mut b).unwrap();']
   g+=['n,err:=EncodeFibonacciFeedback(v.Goal,v.Values,b);']
  else:
   r+=['let n=codec::encode_fibonacci_reply(value.goal,value.kind,&value.values().collect::<Vec<_>>(),&mut b).unwrap();']
   g+=['n,err:=EncodeFibonacciReply(v.Goal,v.Kind,v.Values,b);']
  r+=['assert_eq!(&b[..n],expected);'];g+=['assertBytes(t,b[:n],want,err)']
 elif k=='lifecycle-state':
  r+=['let value=LifecycleState{id:'+str(v['id'])+',label:String::from('+rq(v['label'])+')};assert_eq!(value.encode().unwrap(),expected);assert_eq!(LifecycleState::decode(&expected).unwrap(),value);']
  g+=['v:=LifecycleState{ID:'+str(v['id'])+',Label:'+gq(v['label'])+'};b,err:=v.Encode();assertBytes(t,b,want,err);decoded,err:=DecodeLifecycleState(want);if err!=nil||decoded!=v{t.Fatal("lifecycle decode",err)}']
 elif k=='time':
  r+=['assert_eq!(codec::decode_time(&expected).unwrap(),'+v+'u64);'];g+=['n,err:=DecodeTime(want);if err!=nil||n!='+v+'{t.Fatal("time precision",err)}']
 elif k=='parameter-event':
  rv='LegacyParameterEvent{seconds:'+str(v['seconds'])+',nanoseconds:'+str(v['nanoseconds'])+',kind:'+str(v['kind'])+',node:String::from('+rq(v['node'])+'),name:String::from('+rq(v['name'])+'),value:String::from('+rq(v['value'])+')}'
  gv='LegacyParameterEvent{Seconds:'+str(v['seconds'])+',Nanoseconds:'+str(v['nanoseconds'])+',Kind:'+str(v['kind'])+',Node:'+gq(v['node'])+',Name:'+gq(v['name'])+',Value:'+gq(v['value'])+'}'
  r+=['let value='+rv+';assert_eq!(value.encode().unwrap(),expected);assert_eq!(LegacyParameterEvent::decode(&expected).unwrap(),value);'];g+=['v:='+gv+';b,err:=v.Encode();assertBytes(t,b,want,err);decoded,err:=DecodeLegacyParameterEvent(want);if err!=nil||decoded!=v{t.Fatal("legacy decode",err)}']
 elif k=='typed-parameter-event':
  tag='Deletion' if v['tag']==0 else kinds[tagkinds[v['tag']]][0]
  rvalue='None' if v['value'] is None else 'Some('+rp(tagkinds[v['tag']],v['value'])+')'
  rv='TypedParameterEvent{seconds:'+str(v['seconds'])+',nanoseconds:'+str(v['nanoseconds'])+',kind:'+str(v['kind'])+',tag:EventTag::'+tag+',node:String::from('+rq(v['node'])+'),name:String::from('+rq(v['name'])+'),value:'+rvalue+'}'
  gv='TypedParameterEvent{Seconds:'+str(v['seconds'])+',Nanoseconds:'+str(v['nanoseconds'])+',Kind:'+str(v['kind'])+',Tag:Event'+tag+',Node:'+gq(v['node'])+',Name:'+gq(v['name'])
  if v['value'] is not None:gv+=',Value:&'+gp(tagkinds[v['tag']],v['value'])
  gv+='}'
  r+=['let value='+rv+';assert_eq!(value.encode().unwrap(),expected);assert_eq!(TypedParameterEvent::decode(&expected).unwrap().encode().unwrap(),expected);'];g+=['v:='+gv+';b,err:=v.Encode();assertBytes(t,b,want,err);decoded,err:=DecodeTypedParameterEvent(want);if err!=nil{t.Fatal(err)};b,err=decoded.Encode();assertBytes(t,b,want,err)']
 else:raise ValueError(k)
 r+=['    }'];g+=['})']
r+=['}'];g+=['}']
r+=['#[test]','fn shared_negative_vectors() {'];g+=['func TestSharedNegativeVectors(t *testing.T){']
for row in data['negative']:
 k=row['codec'];hexv=row['hex'];r+=['{let b=bytes('+rq(hexv)+');'];g+=['{b:=fixture('+gq(hexv)+');']
 if k=='bool':rd='codec::decode_bool(&b)';gd='DecodeBool(b)'
 elif k=='string':rd='codec::decode_string(&b)';gd='DecodeString(b)'
 elif k=='action-result':rd='codec::decode_fibonacci_result(&b)';gd='DecodeFibonacciResult(b)'
 elif k=='typed-parameter-event':rd='TypedParameterEvent::decode(&b)';gd='DecodeTypedParameterEvent(b)'
 else:
  name=kinds[k[10:]][0];rd='ParameterValue::decode(ParameterKind::'+name+',&b)';gd='DecodeParameter(Parameter'+name+',b)'
 r+=['assert!('+rd+'.is_err());}'];g+=['_,err:='+gd+';if err==nil{t.Fatal("expected refusal: '+row['reason'].replace('"','\\"')+'")}}']
r+=['}'];g+=['}']
out.mkdir(mode=0o700)
(out/'rust').mkdir();(out/'tinygo').mkdir()
(out/'rust/wire_vectors.rs').write_text('\n'.join(r)+'\n')
(out/'tinygo/wire_vectors_test.go').write_text('\n'.join(g)+'\n')
print(json.dumps({'status':'PREPARED_NOT_COMPILED','positive_cases':len(data['positive']),'negative_cases':len(data['negative'])}))
