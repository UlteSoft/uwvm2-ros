/** Lesson8: independently granted Fibonacci client operations and typed arrays.
 * Grant fibonacci-send/cancel-own/feedback/result for the same official Action.
 * Send does not grant cancellation or reads. This lesson requests no cancel-any
 * right and never cancels another owner's goals. array-get/set are BoolArray
 * parameter aliases with explicit positive element/value/byte limits; parameter
 * list metadata uses a third alias. No native type or parameter name comes from
 * a message. Poll/read each operation only once and preserve non-idle errors.
 */
import { NativeApi, Session, Status, Operation, ParameterKind, ParameterValue }
  from '../../../assemblyscript/ros1/roswasm';
export { abortGuest } from '../../../assemblyscript/api';
let status:u32=0, ownedGoal:u64=0;
function remember(value:u32):void { if(value && value!=Status.Again && !status) status=value; }
/** Complete entry with checked acquisition, typed calls and all-handle cleanup. */
export function _start():void {
  const session=new Session(new NativeApi(),7), node=session.node('as_action_parameters'); remember(session.status);
  const action=node.createActionClient('fibonacci-send','fibonacci-cancel-own','fibonacci-feedback','fibonacci-result'); remember(action.status);
  if(!status) { const sent=action.sendGoal(5); remember(sent.status); if(!sent.status) { ownedGoal=action.goalId(sent); if(!ownedGoal) remember(Status.Invalid); } }
  if(!status) { const reply=action.feedback(ownedGoal); remember(reply.status); if(!reply.status && !action.feedbackValue(ownedGoal,reply).valid) remember(Status.Invalid); }
  // Cancel before consuming a terminal result, which retires this ID in the
  // Runtime. A later cancel would be stale9. False is valid if work finished
  // but its result remains unread; cancellation never rolls back admitted work.
  if(!status) { const reply=action.cancelOwn(ownedGoal); remember(reply.status); if(!reply.status && (reply.bytes.length!=1 || reply.bytes[0]>1)) remember(Status.Invalid); }
  if(!status) { const reply=action.result(ownedGoal); remember(reply.status); if(!reply.status && !action.resultValue(ownedGoal,reply).valid) remember(Status.Invalid); }
  const get=node.createParameters('array-get',Operation.parameter_get,ParameterKind.BoolArray);
  if(!status) { const reply=get.getReply(); remember(reply.status); if(!reply.status && !get.decode(reply).valid) remember(Status.Invalid); }
  const set=node.createParameters('array-set',Operation.parameter_set,ParameterKind.BoolArray);
  const value=new ParameterValue(ParameterKind.BoolArray); value.boolArray=[true,false]; if(!status) remember(set.set(value).status);
  const list=node.createParameters('parameter-list',Operation.parameter_list,ParameterKind.BoolArray); if(!status) remember(list.list().status);
  const cleanup=session.close(); for(let i=0;i<cleanup.length;++i) remember(cleanup[i]);
}
/** First actual error;11 indicates normal idle and never triggers a blind retry. */
export function lastStatus():u32 { return status; }
/** Exact Gateway-issued ID, never a permission or a fabricated sequence number. */
export function goalId():u64 { return ownedGoal; }
