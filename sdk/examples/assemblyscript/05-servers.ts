/** Lesson5: real Trigger/SetBool and Fibonacci polling servers.
 * Grant trigger-events/service.advertise and trigger-replies/service.respond
 * independently for the same official service, plus fibonacci-events and
 * fibonacci-replies for advertise/respond of the supported Fibonacci Action.
 * Native type/identity, deadlines, queue/goal/rate bounds remain host policy.
 * This is one bounded pass. Again11 is idle, not delivered or automatically retried.
 */
import { NativeApi, Session, Executor, Status, ServiceRequest, ServiceResponse,
  IncomingGoal, ActionSequence } from '../../assemblyscript/ros2/roswasm';
export { abortGuest } from '../../assemblyscript/api';
let status:u32=0;
function remember(value:u32):void { if(value && value!=Status.Again && !status) status=value; }
function serve(request:ServiceRequest):ServiceResponse {
  // This lesson grants Trigger: its body is empty. For SetBool inspect its one
  // validated byte. The SDK retains request.id when encoding the response.
  const response=new ServiceResponse(true,'handled'); response.valid=request.valid && request.request.length==0; return response;
}
function act(goal:IncomingGoal):ActionSequence {
  // Match the repository's official tutorial: [0,1], then append for1..order-1.
  // The decoded0..46 bound keeps every signed int32 sum exact. Emit one result
  // without a hidden thread, sleep or feedback loop; retain the incoming ID.
  const values:Array<i32>=[0,1]; for(let index:i32=1;index<goal.order;++index) values.push(values[index]+values[index-1]);
  return new ActionSequence(goal.id,2,values);
}
/** Construct transactionally, run once, inspect every local/native close status. */
export function _start():void {
  const session=new Session(new NativeApi(),4), node=session.node('as_servers'); remember(session.status);
  const service=node.createService('trigger-events','trigger-replies',serve,256); remember(service.status);
  const action=node.createActionServer('fibonacci-events','fibonacci-replies',act,256); remember(action.status);
  const executor=new Executor(2); remember(executor.add(service)); remember(executor.add(action));
  if(!status) { const results=executor.spinOnce(); for(let i=0;i<results.length;++i) remember(results[i]); }
  remember(executor.close()); const cleanup=session.close(); for(let i=0;i<cleanup.length;++i) remember(cleanup[i]);
}
/** Nonzero reports initialization, codec, operation or cleanup error; no logger grant is assumed. */
export function lastStatus():u32 { return status; }
