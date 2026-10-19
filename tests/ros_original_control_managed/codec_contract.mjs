/** Authored local Node codec contracts. No bridge/ROS endpoint is made.
 * They have not run. Actual Guest/native/producer checks remain separate. */
import assert from 'node:assert/strict';
import * as r from '../../sdk/custom-types/generated-control-managed/bounded_records.mjs';
const source=r.Ros2Type_description_interfacesMsgTypeSourceCodec;
const response=r.Ros2Type_description_interfacesSrvGetTypeDescription_ResponseCodec;
function raw(n){const s=source.defaultValue();s.type_name='rcl_interfaces/msg/Log';s.encoding='msg';s.raw_file_contents=new Uint8Array(n).fill(255);if(n)s.raw_file_contents[n-1]=0;return s;}
function code(n,fn){assert.throws(fn,e=>e.code===n);}
assert.equal(r.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodec.defaultValue().include_type_sources,true);
assert.equal(r.Ros2Rcl_interfacesMsgParameterTypeCodec.constants.PARAMETER_STRING,4);
assert.deepEqual(source.decode(source.encode(raw(6162))),raw(6162));
code(22,()=>source.encode(raw(6163)));
const full=response.defaultValue();full.successful=true;full.type_sources=Array.from({length:12},()=>raw(16));
const bytes=response.encode(full),copy=response.decode(bytes);bytes.fill(0);assert.deepEqual(copy,full);
const large=response.defaultValue();large.type_sources=Array.from({length:12},()=>raw(3000));code(90,()=>response.encode(large));
code(90,()=>response.decode(new Uint8Array(32769)));
for(const text of ['bad\0text','\ud800','\udc00']){const value=raw(1);value.type_name=text;code(22,()=>source.encode(value));}
const withBom=raw(0);withBom.type_name='\ufeffname';assert.equal(source.decode(source.encode(withBom)).type_name,withBom.type_name);
const pv=r.Ros2Rcl_interfacesMsgParameterValueCodec.defaultValue();Object.assign(pv,{type:4,bool_value:true,integer_value:-(1n<<63n),double_value:-1.25,string_value:'text',byte_array_value:[0,255],bool_array_value:[true,false],integer_array_value:[-(1n<<63n),(1n<<63n)-1n],double_array_value:[2.5,-3.25],string_array_value:['a','b']});assert.deepEqual(r.Ros2Rcl_interfacesMsgParameterValueCodec.decode(r.Ros2Rcl_interfacesMsgParameterValueCodec.encode(pv)),pv);
const event=r.Ros2Service_msgsMsgServiceEventInfoCodec.defaultValue();event.client_gid.pop();code(22,()=>r.Ros2Service_msgsMsgServiceEventInfoCodec.encode(event));
let reads=0;const once=raw(3);Object.defineProperty(once,'encoding',{get(){++reads;if(reads>1)throw Error('re-read getter');return 'msg';}});source.encode(once);assert.equal(reads,1);
class Misleading extends Uint8Array {get length(){return 1;}static get [Symbol.species](){throw Error('species must not be consulted');}}
const spoof=raw(0);spoof.raw_file_contents=new Misleading(6163);code(22,()=>source.encode(spoof));
const good=source.encode(raw(1));for(let n=0;n<good.length;++n)assert.equal(source.valid(good.subarray(0,n)),false);
const trailing=new Uint8Array(good.length+1);trailing.set(good);assert.equal(source.valid(trailing),false);

const malformed=source.encode(raw(0));malformed[4]=255;code(22,()=>source.decode(malformed));
assert.notEqual(r.Ros2Type_description_interfacesSrvGetTypeDescriptionIdentity.codecIdentity,r.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodec.codecIdentity);
