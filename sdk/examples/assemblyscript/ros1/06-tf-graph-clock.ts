/** ROS1 lesson6 uses its actual TF, filtered graph and wall-clock imports.
 * ROS2 parameter events/lifecycle/steady clock are absent from ROS1 ABI v1;
 * this lesson does not replace them with an unrelated operation. Every alias
 * below needs an independent configured operation/type/frame/limit grant.
 */
import { NativeApi, Session, Operation, Status } from '../../../assemblyscript/ros1/roswasm';
export { abortGuest } from '../../../assemblyscript/api';
let status:u32=0, nanoseconds:u64=0;
function remember(value:u32):void { if(value && !status) status=value; }
/** Make each bounded operation at most once and inspect all checked close results. */
export function _start():void {
  const session=new Session(new NativeApi(),3), node=session.node('as_ros1_resources'); remember(session.status);
  const tf=node.createTf('transform-lookup',Operation.tf_lookup); if(!status) { const r=tf.lookup(0); remember(r.status); if(!r.status && !tf.transform(r).valid) remember(Status.Invalid); }
  const graph=node.createGraph('nodes-list',Operation.graph_list_nodes); if(!status) remember(graph.readText().status);
  const clock=node.createClock('wall-clock',Operation.time_read_wall); if(!status) { const r=clock.now(); remember(r.status); if(!r.status) { if(r.bytes.length!=8) remember(Status.Invalid); else nanoseconds=clock.nanoseconds(r); } }
  const cleanup=session.close(); for(let i=0;i<cleanup.length;++i) remember(cleanup[i]);
}
/** Exact first operation/cleanup error; no logger authority is obtained. */
export function lastStatus():u32 { return status; }
/** Preserve all64 time bits, without treating zero as an error. */
export function lastTime():u64 { return nanoseconds; }
