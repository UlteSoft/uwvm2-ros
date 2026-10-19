/** Lesson6: ROS2 typed parameter events, TF, graph, explicit clock/lifecycle.
 * Each named alias is independent; host policy fixes parameter kind, node,
 * frames, transition, native type identities and quotas. No wildcard right is
 * created. Parameter events/lifecycle have no ROS1 equivalent in this ABI.
 */
import { NativeApi, Session, Operation, ParameterKind, Status, decodeTypedParameterEvent }
  from '../../assemblyscript/ros2/roswasm';
export { abortGuest } from '../../assemblyscript/api';
let status:u32=0, eventKind:u8=0, nanoseconds:u64=0;
function remember(value:u32):void { if(value && value!=Status.Again && !status) status=value; }
/** One explicit pass; no default clock, sleep, background polling or native callback. */
export function _start():void {
  const session=new Session(new NativeApi(),6), node=session.node('as_events'); remember(session.status);
  const events=node.createParameters('parameter-events',Operation.parameter_subscribe_events,ParameterKind.Bool,4096);
  remember(events.subscribeEvents());
  if(!status) { const r=events.pollEventReply(); remember(r.status); if(!r.status) { const value=decodeTypedParameterEvent(r.bytes); if(!value.valid) remember(Status.Invalid); else eventKind=value.kind; } }
  const tf=node.createTf('transform-lookup',Operation.tf_lookup); if(!status) { const reply=tf.lookup(0); remember(reply.status); if(!reply.status && !tf.transform(reply).valid) remember(Status.Invalid); }
  const graph=node.createGraph('nodes-list',Operation.graph_list_nodes); if(!status) remember(graph.readText().status);
  const clock=node.createClock('steady-clock',Operation.time_read_steady); if(!status) { const reply=clock.now(); remember(reply.status); if(!reply.status) { if(reply.bytes.length!=8) remember(Status.Invalid); else nanoseconds=clock.nanoseconds(reply); } }
  const state=node.createLifecycle('lifecycle-state',Operation.lifecycle_get_state); if(!status) { const reply=state.getStateReply(); remember(reply.status); }
  const change=node.createLifecycle('lifecycle-change',Operation.lifecycle_change_state); if(!status) remember(change.changeState().status);
  const cleanup=session.close(); for(let i=0;i<cleanup.length;++i) remember(cleanup[i]);
}
/** First observed error; Again is ordinary idle and does not imply event delivery. */
export function lastStatus():u32 { return status; }
/** Zero means no validated typed event was observed during this pass. */
export function lastEventKind():u8 { return eventKind; }
/** Exact explicitly granted clock result; zero is also a valid native value. */
export function lastTime():u64 { return nanoseconds; }
