/** ROS 1 lesson 3: Node, typed publisher/subscription and guest-driven executor.
 * The callback is ordinary guest code. Native ROS never receives its address.
 * Grant pub and sub separately; default policy grants neither.
 */
import { NativeApi, Node, Executor, StringCodec } from '../../../assemblyscript/ros1/roswasm';
export { abortGuest } from '../../../assemblyscript/api';
let received: string = '';
let status: u32 = 0;

/** Guest-local callback run only when Executor.spinOnce obtains a complete event. */
function onGreeting(value: string): void { received = value; }

/** Run one bounded iteration. Applications explicitly supply repeated scheduling when needed. */
export function _start(): void {
  const node = new Node(new NativeApi(), 'greeting');
  const publisher = node.createPublisher<string>('pub', new StringCodec());
  const subscriber = node.createSubscription<string>('sub', new StringCodec(), onGreeting, 4096);
  status = publisher.capability.openStatus; if(subscriber.status && !status) status=subscriber.status;
  if (!status) status = publisher.publish('hello from roswasm');
  const executor = new Executor(); const registered=executor.add(subscriber); if(registered && !status) status=registered; if(!status) { const outcomes=executor.spinOnce(); for(let i=0;i<outcomes.length;++i) if(outcomes[i] && outcomes[i]!=11 && !status) status=outcomes[i]; } const stopped=executor.close(); if(stopped && !status) status=stopped;
  const cleanup=node.close(); for(let i=0;i<cleanup.length;++i) if(cleanup[i] && !status) status=cleanup[i];
}
/** Retrieve the publish/open diagnostic status without adding a log capability. */
export function lastStatus(): u32 { return status; }
/** Retrieve whether a copied event reached the guest callback. */
export function receivedBytes(): i32 { return String.UTF8.byteLength(received); }
