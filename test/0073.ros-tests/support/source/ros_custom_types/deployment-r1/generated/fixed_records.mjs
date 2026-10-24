/** Generated fixed-record topic codecs. No native serialization, I/O or authority.
 * Encoders validate exact fields/array lengths and scalar ranges before allocation.
 * Decode returns a fresh value only after consuming exact canonical bytes. */
function shape(v,keys){if(v===null||typeof v!=='object'||Array.isArray(v)||Reflect.ownKeys(v).length!==keys.length||keys.some(k=>!Object.hasOwn(v,k)))throw new TypeError('Exact record fields required');}
function array(v,n){if(!Array.isArray(v)||v.length!==n)throw new TypeError('Exact fixed array required');}
function scalar(v,k){if(k==='bool'){if(typeof v!=='boolean')throw new TypeError('Boolean required');return;}if(k==='int64'||k==='uint64'){const lo=k==='int64'?-(1n<<63n):0n,hi=k==='int64'?(1n<<63n)-1n:(1n<<64n)-1n;if(typeof v!=='bigint'||v<lo||v>hi)throw new RangeError('64-bit BigInt required');return;}if(k.startsWith('float')){if(typeof v!=='number'||!Number.isFinite(v)||(k==='float32'&&!Number.isFinite(Math.fround(v))))throw new RangeError('Finite float required');return;}const signed=k.startsWith('int'),bits=Number(k.replace(/\D/g,'')),lo=signed?-(2**(bits-1)):0,hi=signed?2**(bits-1)-1:2**bits-1;if(!Number.isInteger(v)||v<lo||v>hi)throw new RangeError('Scalar integer range');}
/** Validate complete ROS1 uwvm_custom_ros1_fixture/Packet before encoding. */
function validateRos1Uwvm_custom_ros1_fixturePacket(value){
shape(value,["sequence", "active", "measurement", "tag", "samples"]);
array(value.tag,4);
array(value.samples,3);
scalar(value.sequence,"int32");
scalar(value.active,"bool");
scalar(value.measurement,"float64");
scalar(value.tag[0],"uint8");
scalar(value.tag[1],"uint8");
scalar(value.tag[2],"uint8");
scalar(value.tag[3],"uint8");
scalar(value.samples[0],"int16");
scalar(value.samples[1],"int16");
scalar(value.samples[2],"int16");
}
/** Codec accepted by roswasm Node.createPublisher/createSubscription; wire bytes=23. */
export const Ros1Uwvm_custom_ros1_fixturePacketCodec=Object.freeze({wireBytes:23,codecIdentity:"UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544",
encode(value){validateRos1Uwvm_custom_ros1_fixturePacket(value);const bytes=new Uint8Array(23),view=new DataView(bytes.buffer);
view.setInt32(0,value.sequence,true);
view.setUint8(4,Number(value.active),true);
view.setFloat64(5,value.measurement,true);
view.setUint8(13,value.tag[0],true);
view.setUint8(14,value.tag[1],true);
view.setUint8(15,value.tag[2],true);
view.setUint8(16,value.tag[3],true);
view.setInt16(17,value.samples[0],true);
view.setInt16(19,value.samples[1],true);
view.setInt16(21,value.samples[2],true);
return bytes;},
decode(bytes){if(!(bytes instanceof Uint8Array)||bytes.length!==23)throw new RangeError('Exact canonical length required');const view=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength),value={"sequence":0,"active":false,"measurement":0,"tag":[0,0,0,0],"samples":[0,0,0]};
value.sequence=view.getInt32(0,true);
if(view.getUint8(4)>1)throw new RangeError('Noncanonical Bool');value.active=view.getUint8(4)!==0;
value.measurement=view.getFloat64(5,true);
value.tag[0]=view.getUint8(13,true);
value.tag[1]=view.getUint8(14,true);
value.tag[2]=view.getUint8(15,true);
value.tag[3]=view.getUint8(16,true);
value.samples[0]=view.getInt16(17,true);
value.samples[1]=view.getInt16(19,true);
value.samples[2]=view.getInt16(21,true);
validateRos1Uwvm_custom_ros1_fixturePacket(value);return value;}
});
/** Validate complete ROS2 uwvm_ros_custom_test_msgs/msg/FixedSample before encoding. */
function validateRos2Uwvm_ros_custom_test_msgsFixedSample(value){
shape(value,["sequence", "enabled", "temperature", "acceleration", "tag", "thresholds"]);
array(value.acceleration,3);
array(value.tag,8);
array(value.thresholds,2);
scalar(value.sequence,"uint32");
scalar(value.enabled,"bool");
scalar(value.temperature,"float32");
scalar(value.acceleration[0],"float64");
scalar(value.acceleration[1],"float64");
scalar(value.acceleration[2],"float64");
scalar(value.tag[0],"uint8");
scalar(value.tag[1],"uint8");
scalar(value.tag[2],"uint8");
scalar(value.tag[3],"uint8");
scalar(value.tag[4],"uint8");
scalar(value.tag[5],"uint8");
scalar(value.tag[6],"uint8");
scalar(value.tag[7],"uint8");
scalar(value.thresholds[0],"int16");
scalar(value.thresholds[1],"int16");
}
/** Codec accepted by roswasm Node.createPublisher/createSubscription; wire bytes=45. */
export const Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec=Object.freeze({wireBytes:45,codecIdentity:"UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516",
encode(value){validateRos2Uwvm_ros_custom_test_msgsFixedSample(value);const bytes=new Uint8Array(45),view=new DataView(bytes.buffer);
view.setUint32(0,value.sequence,true);
view.setUint8(4,Number(value.enabled),true);
view.setFloat32(5,value.temperature,true);
view.setFloat64(9,value.acceleration[0],true);
view.setFloat64(17,value.acceleration[1],true);
view.setFloat64(25,value.acceleration[2],true);
view.setUint8(33,value.tag[0],true);
view.setUint8(34,value.tag[1],true);
view.setUint8(35,value.tag[2],true);
view.setUint8(36,value.tag[3],true);
view.setUint8(37,value.tag[4],true);
view.setUint8(38,value.tag[5],true);
view.setUint8(39,value.tag[6],true);
view.setUint8(40,value.tag[7],true);
view.setInt16(41,value.thresholds[0],true);
view.setInt16(43,value.thresholds[1],true);
return bytes;},
decode(bytes){if(!(bytes instanceof Uint8Array)||bytes.length!==45)throw new RangeError('Exact canonical length required');const view=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength),value={"sequence":0,"enabled":false,"temperature":0,"acceleration":[0,0,0],"tag":[0,0,0,0,0,0,0,0],"thresholds":[0,0]};
value.sequence=view.getUint32(0,true);
if(view.getUint8(4)>1)throw new RangeError('Noncanonical Bool');value.enabled=view.getUint8(4)!==0;
value.temperature=view.getFloat32(5,true);
value.acceleration[0]=view.getFloat64(9,true);
value.acceleration[1]=view.getFloat64(17,true);
value.acceleration[2]=view.getFloat64(25,true);
value.tag[0]=view.getUint8(33,true);
value.tag[1]=view.getUint8(34,true);
value.tag[2]=view.getUint8(35,true);
value.tag[3]=view.getUint8(36,true);
value.tag[4]=view.getUint8(37,true);
value.tag[5]=view.getUint8(38,true);
value.tag[6]=view.getUint8(39,true);
value.tag[7]=view.getUint8(40,true);
value.thresholds[0]=view.getInt16(41,true);
value.thresholds[1]=view.getInt16(43,true);
validateRos2Uwvm_ros_custom_test_msgsFixedSample(value);return value;}
});
