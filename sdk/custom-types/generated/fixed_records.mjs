/** Generated fixed-record topic codecs. No native serialization, I/O or authority.
 * Encoders validate exact fields/array lengths and scalar ranges before allocation.
 * Decode returns a fresh value only after consuming exact canonical bytes. */
function shape(v,keys){if(v===null||typeof v!=='object'||Array.isArray(v)||Reflect.ownKeys(v).length!==keys.length||keys.some(k=>!Object.hasOwn(v,k)))throw new TypeError('Exact record fields required');}
function array(v,n){if(!Array.isArray(v)||v.length!==n)throw new TypeError('Exact fixed array required');}
function scalar(v,k){if(k==='bool'){if(typeof v!=='boolean')throw new TypeError('Boolean required');return;}if(k==='int64'||k==='uint64'){const lo=k==='int64'?-(1n<<63n):0n,hi=k==='int64'?(1n<<63n)-1n:(1n<<64n)-1n;if(typeof v!=='bigint'||v<lo||v>hi)throw new RangeError('64-bit BigInt required');return;}if(k.startsWith('float')){if(typeof v!=='number'||!Number.isFinite(v)||(k==='float32'&&!Number.isFinite(Math.fround(v))))throw new RangeError('Finite float required');return;}const signed=k.startsWith('int'),bits=Number(k.replace(/\D/g,'')),lo=signed?-(2**(bits-1)):0,hi=signed?2**(bits-1)-1:2**bits-1;if(!Number.isInteger(v)||v<lo||v>hi)throw new RangeError('Scalar integer range');}
/** Validate complete ROS1 example_interfaces_custom/Packet before encoding. */
function validateRos1Example_interfaces_customPacket(value){
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
export const Ros1Example_interfaces_customPacketCodec=Object.freeze({wireBytes:23,codecIdentity:"UWVMROS1_e68d0934f545cd81105ac13682379dc84f023c860c070c046e69c7f70e94d407",
encode(value){validateRos1Example_interfaces_customPacket(value);const bytes=new Uint8Array(23),view=new DataView(bytes.buffer);
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
validateRos1Example_interfaces_customPacket(value);return value;}
});
/** Validate complete ROS1 example_interfaces_custom/ScalarRecord before encoding. */
function validateRos1Example_interfaces_customScalarRecord(value){
shape(value,["enabled", "unsigned_small", "signed_small", "unsigned_medium", "signed_medium", "unsigned_word", "signed_word", "unsigned_large", "signed_large", "single_value", "double_value"]);
scalar(value.enabled,"bool");
scalar(value.unsigned_small,"uint8");
scalar(value.signed_small,"int8");
scalar(value.unsigned_medium,"uint16");
scalar(value.signed_medium,"int16");
scalar(value.unsigned_word,"uint32");
scalar(value.signed_word,"int32");
scalar(value.unsigned_large,"uint64");
scalar(value.signed_large,"int64");
scalar(value.single_value,"float32");
scalar(value.double_value,"float64");
}
/** Codec accepted by roswasm Node.createPublisher/createSubscription; wire bytes=43. */
export const Ros1Example_interfaces_customScalarRecordCodec=Object.freeze({wireBytes:43,codecIdentity:"UWVMROS1_dfab9988d24ce356a6a914d4ae6f6a7c8c44dc8118cfc768e41fa8bf47f386c6",
encode(value){validateRos1Example_interfaces_customScalarRecord(value);const bytes=new Uint8Array(43),view=new DataView(bytes.buffer);
view.setUint8(0,Number(value.enabled),true);
view.setUint8(1,value.unsigned_small,true);
view.setInt8(2,value.signed_small,true);
view.setUint16(3,value.unsigned_medium,true);
view.setInt16(5,value.signed_medium,true);
view.setUint32(7,value.unsigned_word,true);
view.setInt32(11,value.signed_word,true);
view.setBigUint64(15,value.unsigned_large,true);
view.setBigInt64(23,value.signed_large,true);
view.setFloat32(31,value.single_value,true);
view.setFloat64(35,value.double_value,true);
return bytes;},
decode(bytes){if(!(bytes instanceof Uint8Array)||bytes.length!==43)throw new RangeError('Exact canonical length required');const view=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength),value={"enabled":false,"unsigned_small":0,"signed_small":0,"unsigned_medium":0,"signed_medium":0,"unsigned_word":0,"signed_word":0,"unsigned_large":0n,"signed_large":0n,"single_value":0,"double_value":0};
if(view.getUint8(0)>1)throw new RangeError('Noncanonical Bool');value.enabled=view.getUint8(0)!==0;
value.unsigned_small=view.getUint8(1,true);
value.signed_small=view.getInt8(2,true);
value.unsigned_medium=view.getUint16(3,true);
value.signed_medium=view.getInt16(5,true);
value.unsigned_word=view.getUint32(7,true);
value.signed_word=view.getInt32(11,true);
value.unsigned_large=view.getBigUint64(15,true);
value.signed_large=view.getBigInt64(23,true);
value.single_value=view.getFloat32(31,true);
value.double_value=view.getFloat64(35,true);
validateRos1Example_interfaces_customScalarRecord(value);return value;}
});
/** Validate complete ROS1 example_interfaces_custom/NestedRecord before encoding. */
function validateRos1Example_interfaces_customNestedRecord(value){
shape(value,["readings", "positions"]);
array(value.readings,2);
shape(value.readings[0],["enabled", "unsigned_small", "signed_small", "unsigned_medium", "signed_medium", "unsigned_word", "signed_word", "unsigned_large", "signed_large", "single_value", "double_value"]);
shape(value.readings[1],["enabled", "unsigned_small", "signed_small", "unsigned_medium", "signed_medium", "unsigned_word", "signed_word", "unsigned_large", "signed_large", "single_value", "double_value"]);
array(value.positions,3);
scalar(value.readings[0].enabled,"bool");
scalar(value.readings[0].unsigned_small,"uint8");
scalar(value.readings[0].signed_small,"int8");
scalar(value.readings[0].unsigned_medium,"uint16");
scalar(value.readings[0].signed_medium,"int16");
scalar(value.readings[0].unsigned_word,"uint32");
scalar(value.readings[0].signed_word,"int32");
scalar(value.readings[0].unsigned_large,"uint64");
scalar(value.readings[0].signed_large,"int64");
scalar(value.readings[0].single_value,"float32");
scalar(value.readings[0].double_value,"float64");
scalar(value.readings[1].enabled,"bool");
scalar(value.readings[1].unsigned_small,"uint8");
scalar(value.readings[1].signed_small,"int8");
scalar(value.readings[1].unsigned_medium,"uint16");
scalar(value.readings[1].signed_medium,"int16");
scalar(value.readings[1].unsigned_word,"uint32");
scalar(value.readings[1].signed_word,"int32");
scalar(value.readings[1].unsigned_large,"uint64");
scalar(value.readings[1].signed_large,"int64");
scalar(value.readings[1].single_value,"float32");
scalar(value.readings[1].double_value,"float64");
scalar(value.positions[0],"int32");
scalar(value.positions[1],"int32");
scalar(value.positions[2],"int32");
}
/** Codec accepted by roswasm Node.createPublisher/createSubscription; wire bytes=98. */
export const Ros1Example_interfaces_customNestedRecordCodec=Object.freeze({wireBytes:98,codecIdentity:"UWVMROS1_b0856889a47eb923055e44730a21c04d498472c373a1b9539a8f213a7c09ae5c",
encode(value){validateRos1Example_interfaces_customNestedRecord(value);const bytes=new Uint8Array(98),view=new DataView(bytes.buffer);
view.setUint8(0,Number(value.readings[0].enabled),true);
view.setUint8(1,value.readings[0].unsigned_small,true);
view.setInt8(2,value.readings[0].signed_small,true);
view.setUint16(3,value.readings[0].unsigned_medium,true);
view.setInt16(5,value.readings[0].signed_medium,true);
view.setUint32(7,value.readings[0].unsigned_word,true);
view.setInt32(11,value.readings[0].signed_word,true);
view.setBigUint64(15,value.readings[0].unsigned_large,true);
view.setBigInt64(23,value.readings[0].signed_large,true);
view.setFloat32(31,value.readings[0].single_value,true);
view.setFloat64(35,value.readings[0].double_value,true);
view.setUint8(43,Number(value.readings[1].enabled),true);
view.setUint8(44,value.readings[1].unsigned_small,true);
view.setInt8(45,value.readings[1].signed_small,true);
view.setUint16(46,value.readings[1].unsigned_medium,true);
view.setInt16(48,value.readings[1].signed_medium,true);
view.setUint32(50,value.readings[1].unsigned_word,true);
view.setInt32(54,value.readings[1].signed_word,true);
view.setBigUint64(58,value.readings[1].unsigned_large,true);
view.setBigInt64(66,value.readings[1].signed_large,true);
view.setFloat32(74,value.readings[1].single_value,true);
view.setFloat64(78,value.readings[1].double_value,true);
view.setInt32(86,value.positions[0],true);
view.setInt32(90,value.positions[1],true);
view.setInt32(94,value.positions[2],true);
return bytes;},
decode(bytes){if(!(bytes instanceof Uint8Array)||bytes.length!==98)throw new RangeError('Exact canonical length required');const view=new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength),value={"readings":[{"enabled":false,"unsigned_small":0,"signed_small":0,"unsigned_medium":0,"signed_medium":0,"unsigned_word":0,"signed_word":0,"unsigned_large":0n,"signed_large":0n,"single_value":0,"double_value":0},{"enabled":false,"unsigned_small":0,"signed_small":0,"unsigned_medium":0,"signed_medium":0,"unsigned_word":0,"signed_word":0,"unsigned_large":0n,"signed_large":0n,"single_value":0,"double_value":0}],"positions":[0,0,0]};
if(view.getUint8(0)>1)throw new RangeError('Noncanonical Bool');value.readings[0].enabled=view.getUint8(0)!==0;
value.readings[0].unsigned_small=view.getUint8(1,true);
value.readings[0].signed_small=view.getInt8(2,true);
value.readings[0].unsigned_medium=view.getUint16(3,true);
value.readings[0].signed_medium=view.getInt16(5,true);
value.readings[0].unsigned_word=view.getUint32(7,true);
value.readings[0].signed_word=view.getInt32(11,true);
value.readings[0].unsigned_large=view.getBigUint64(15,true);
value.readings[0].signed_large=view.getBigInt64(23,true);
value.readings[0].single_value=view.getFloat32(31,true);
value.readings[0].double_value=view.getFloat64(35,true);
if(view.getUint8(43)>1)throw new RangeError('Noncanonical Bool');value.readings[1].enabled=view.getUint8(43)!==0;
value.readings[1].unsigned_small=view.getUint8(44,true);
value.readings[1].signed_small=view.getInt8(45,true);
value.readings[1].unsigned_medium=view.getUint16(46,true);
value.readings[1].signed_medium=view.getInt16(48,true);
value.readings[1].unsigned_word=view.getUint32(50,true);
value.readings[1].signed_word=view.getInt32(54,true);
value.readings[1].unsigned_large=view.getBigUint64(58,true);
value.readings[1].signed_large=view.getBigInt64(66,true);
value.readings[1].single_value=view.getFloat32(74,true);
value.readings[1].double_value=view.getFloat64(78,true);
value.positions[0]=view.getInt32(86,true);
value.positions[1]=view.getInt32(90,true);
value.positions[2]=view.getInt32(94,true);
validateRos1Example_interfaces_customNestedRecord(value);return value;}
});
/** Validate complete ROS2 example_interfaces_custom/msg/FixedSample before encoding. */
function validateRos2Example_interfaces_customFixedSample(value){
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
export const Ros2Example_interfaces_customFixedSampleCodec=Object.freeze({wireBytes:45,codecIdentity:"UWVMROS1_b794a6eb1863626cf279af1fbad1f071fce945d6585f3fdb22979c8f24e32663",
encode(value){validateRos2Example_interfaces_customFixedSample(value);const bytes=new Uint8Array(45),view=new DataView(bytes.buffer);
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
validateRos2Example_interfaces_customFixedSample(value);return value;}
});
