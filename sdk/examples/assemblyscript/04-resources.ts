/** Lesson 4: typed service and parameter calls plus an authorized clock.
 * Alias names are policy labels. The guest does not choose arbitrary native names.
 * Read sdk/docs/abi-v1.md and sdk/docs/wire-format.md for exact type identities and wire forms.
 */
import { NativeApi, Node, Parameters, ParameterKind, Clock, Operation } from '../../assemblyscript/ros2/roswasm';
export { abortGuest } from '../../assemblyscript/api';
let status: u32 = 0;
let time: u64 = 0;

/** Make each requested operation at most once, then release all opened handles. */
export function _start(): void {
  const node = new Node(new NativeApi(), 'resources');
  const client = node.createClient('trigger-call');
  const serviceReply = client.trigger(); status = serviceReply.status;
  // get() is typed; the exact parameter and native integer type come from counter-get's grant.
  const parameter = new Parameters(node.open('counter-get'), Operation.parameter_get, ParameterKind.Int64);
  const parameterReply=parameter.getReply(); if(parameterReply.status && !status) status=parameterReply.status; const value=parameter.decode(parameterReply); if(!value.valid && !status) status=22;
  const clock = new Clock(node.open('clock-ros'), Operation.time_read_ros);
  const clockReply = clock.now(); if (!clockReply.status) time = clock.nanoseconds(clockReply); if(clockReply.status && !status) status=clockReply.status;
  const cleanup=node.close(); for(let i=0;i<cleanup.length;++i) if(cleanup[i] && !status) status=cleanup[i];
}
/** Diagnostic status retained without requiring native logging authority. */
export function lastStatus(): u32 { return status; }
/** Successful ROS clock read, preserving all 64 bits. Zero can also be simulated-time zero. */
export function lastTime(): u64 { return time; }
