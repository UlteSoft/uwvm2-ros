/** Pure JavaScript execution of generated guest-local codecs.
 * Synthetic native identities are not native ROS evidence. No Wasm, compiler,
 * ROS node, network socket or capability import is executed in these models.
 */
import assert from 'node:assert/strict';
import {pathToFileURL} from 'node:url';
const generated=await import(pathToFileURL(process.argv[2]).href);
const scalar=generated.Ros1Test_customScalarRecordCodec;
const nested=generated.Ros1Test_customNestedRecordCodec;
let checks=0;
function check(fn){fn();++checks;}
const value={enabled:true,unsigned_small:255,signed_small:-128,unsigned_medium:65535,signed_medium:-32768,unsigned_word:4294967295,signed_word:-2147483648,unsigned_large:18446744073709551615n,signed_large:-9223372036854775808n,single_value:1.5,double_value:-2.25};
const wire=scalar.encode(value);
check(()=>assert.equal(wire.length,43));
check(()=>assert.deepEqual(scalar.decode(wire),value));
// The golden vector is independently calculated with Python struct.pack using
// <BBbHhIiQqfd, rather than taking expected bytes from this generated encoder.
check(()=>assert.equal(Buffer.from(wire).toString('hex'),'01ff80ffff0080ffffffff00000080ffffffffffffffff00000000000000800000c03f00000000000002c0'));
const outer={readings:[value,{...value,enabled:false}],positions:[-1,0,2147483647]};
check(()=>assert.deepEqual(nested.decode(nested.encode(outer)),outer));
for(let length=0;length<wire.length;++length)check(()=>assert.throws(()=>scalar.decode(wire.subarray(0,length))));
check(()=>assert.throws(()=>scalar.decode(new Uint8Array(wire.length+1))));
check(()=>assert.throws(()=>scalar.decode(Array.from(wire))));
const corrupt=wire.slice();corrupt[0]=2;check(()=>assert.throws(()=>scalar.decode(corrupt)));
for(const [field,bad] of [['unsigned_small',256],['unsigned_small',-1],['signed_small',128],['signed_small',-129],['unsigned_medium',65536],['signed_medium',32768],['unsigned_word',4294967296],['signed_word',-2147483649],['unsigned_large',18446744073709551616n],['unsigned_large',-1n],['signed_large',9223372036854775808n],['signed_large',-9223372036854775809n],['unsigned_large',123],['enabled',2],['single_value',NaN],['single_value',Infinity],['single_value',1e100],['double_value',NaN],['double_value',-Infinity]])check(()=>assert.throws(()=>scalar.encode({...value,[field]:bad})));
check(()=>assert.throws(()=>scalar.encode({...value,extra:1})));
const missing={...value};delete missing.enabled;check(()=>assert.throws(()=>scalar.encode(missing)));
check(()=>assert.throws(()=>nested.encode({...outer,readings:[value]})));
check(()=>assert.throws(()=>nested.encode({...outer,readings:[value,value,value]})));
check(()=>assert.throws(()=>nested.encode({...outer,positions:[1,2]})));
const shifted=new Uint8Array(wire.length+6);shifted.set(wire,3);check(()=>assert.deepEqual(scalar.decode(shifted.subarray(3,3+wire.length)),value));
const float32NaN=wire.slice();new DataView(float32NaN.buffer).setUint32(31,0x7fc00000,true);check(()=>assert.throws(()=>scalar.decode(float32NaN)));
const float64Inf=wire.slice();new DataView(float64Inf.buffer).setFloat64(35,Infinity,true);check(()=>assert.throws(()=>scalar.decode(float64Inf)));
console.log(JSON.stringify({status:'PURE_JAVASCRIPT_MODEL_PASS',checks,native_ros:'NOT_RUN',wasm:'NOT_RUN'}));
