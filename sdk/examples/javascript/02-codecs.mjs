/** Lesson 2: canonical message bytes can be produced without any ROS authority.
 * This is a runnable pure-codec lesson: node sdk/examples/javascript/02-codecs.mjs.
 * Its output is a specification fixture, not a native ROS delivery claim.
 */
import { StringMessage, TwistMessage, ServiceResponse, Parameter, parameterArray } from '../../javascript/codecs.mjs';

const greeting = StringMessage.encode('hello from JavaScript');
console.log('String bytes:', [...greeting]);
console.log('Decoded:', StringMessage.decode(greeting));

// Six float64 fields use linear xyz first, angular xyz second. No host layout is used.
const twist = TwistMessage.encode([0.1, 0, 0, 0, 0, 0.2]);
console.log('Twist bytes:', twist.length);

// Opaque i64 parameter values stay BigInt across encoding and decoding.
const counter = Parameter.Int64.encode(9223372036854775807n);
console.log('Int64:', Parameter.Int64.decode(counter).toString());
console.log('Float64 array:', parameterArray('Float64').decode(parameterArray('Float64').encode([1.25, -0])));
console.log('Service response:', ServiceResponse.decode(ServiceResponse.encode({ success: true, message: 'ready' })));
