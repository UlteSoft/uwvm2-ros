/** Reusable AssemblyScript polling steps. Generate ../generated-rpc and select
 * a real ros2/roswasm NativeApi. No callback/object pointer enters native ROS. */
import {Status} from '../../assemblyscript/api';
import {RPCCleanup,RPCReply,Ros2Demo_rpcSrvIncrementClient,Ros2Demo_rpcSrvIncrementServer,
  Ros2Demo_rpcActionCountServer} from '../generated-rpc/bounded_interfaces';
import {Ros2Demo_rpcSrvIncrement_Request,Ros2Demo_rpcSrvIncrement_Response,
  Ros2Demo_rpcActionCount_Feedback,Ros2Demo_rpcActionCount_Result} from '../generated-rpc/bounded_records';

/** service_submit is real admission, not a deferred synchronous call. Keep the
 * owner and its explicit observer live, then pollCompletion() once per step. */
export function submitStep(client:Ros2Demo_rpcSrvIncrementClient,value:i32):u64{
  const request=new Ros2Demo_rpcSrvIncrement_Request();request.value=value;
  const admitted=client.submit(request);return admitted.status?0:admitted.id;
}
/** One typed local callback under a lease. Close/revoke/nested poll returns17;
 * a fatal Wasm abort cannot promise lease recovery or cleanup. */
export function serviceStep(server:Ros2Demo_rpcSrvIncrementServer):u32{
  return server.spinOnce((_ticket:u64,request:Ros2Demo_rpcSrvIncrement_Request):Ros2Demo_rpcSrvIncrement_Response=>{
    const response=new Ros2Demo_rpcSrvIncrement_Response();response.value=request.value;return response;
  });
}
/** Incoming IDs are retained after feedback and removed only on accepted terminal
 * response. Caller must handle Again11 and choose its own scheduling policy. */
export function actionStep(server:Ros2Demo_rpcActionCountServer):u32{
  const incoming=server.pollOnce();if(incoming.status)return incoming.status;
  const goal=incoming.value;if(goal===null)return Status.Invalid;
  const feedback=new Ros2Demo_rpcActionCount_Feedback();feedback.progress=goal.count;
  let status=server.respondFeedback(incoming.id,feedback);if(status)return status;
  const result=new Ros2Demo_rpcActionCount_Result();result.total=goal.count;
  return server.respondResult(incoming.id,2,result);
}
/** Keep primary separate and inspect every cleanup status before exiting Wasm. */
export function finishClient(client:Ros2Demo_rpcSrvIncrementClient,primary:u32):RPCCleanup{return client.closeChecked(primary);}
