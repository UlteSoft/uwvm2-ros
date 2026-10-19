/** Reusable typed custom RPC steps for a caller-supplied real SDK Session.
 * Generate ../generated-rpc from catalog.preparation.json first. Native use
 * additionally requires official identities and matching explicit host grants.
 * These functions do not create a bridge, thread, clock, retry or ROS permission.
 */
import {Ros2Demo_rpcSrvIncrementClient,Ros2Demo_rpcSrvIncrementServer,
  Ros2Demo_rpcActionCountClient,Ros2Demo_rpcActionCountServer} from '../generated-rpc/bounded_interfaces.mjs';

/** Explicit call + completion-observer aliases; a successful submit is not a result.
 * Keep this returned owner alive, call pollCompletion() once per caller step,
 * and always inspect closeChecked(primaryCode) in the application's finally. */
export function openIncrementClient(session,callAlias,observerAlias){return new Ros2Demo_rpcSrvIncrementClient(session,callAlias,observerAlias,8);}
/** Complete typed request; return the original full-width admitted request ID. */
export function submitIncrement(client,value){return client.submit({value});}
/** One local service callback. Again11 is normal idle and must be handled by
 * the surrounding caller; do not busy-spin or replay a refused write. */
export async function serviceStep(session,advertiseAlias,respondAlias){
  const server=new Ros2Demo_rpcSrvIncrementServer(session,advertiseAlias,respondAlias);
  let primary=0;
  try{return await server.spinOnce(request=>({value:request.value+1}));}
  catch(error){primary=error.code||22;throw error;}
  finally{const cleanup=server.closeChecked(primary);if(cleanup.cleanupError)throw Object.assign(new Error('Inspect primary and every service cleanup result'),{cleanup});}
}
/** The application supplies four distinct granted aliases and optionally
 * cancelAny. No builtin Fibonacci client or implicit stronger alias is used. */
export function openCounter(session,aliases){return new Ros2Demo_rpcActionCountClient(session,aliases,8);}
/** One action-server pass. Its callback sends typed feedback and terminal success
 * for the exact delivered ID. A callback may respond; nested poll/retire is17. */
export async function actionStep(session,advertiseAlias,respondAlias){
  const server=new Ros2Demo_rpcActionCountServer(session,advertiseAlias,respondAlias);
  let primary=0;
  try{return await server.spinOnce((goal,id)=>{server.respondFeedback(id,{progress:goal.count});server.respondResult(id,2,{total:goal.count});});}
  catch(error){primary=error.code||22;throw error;}
  finally{const cleanup=server.closeChecked(primary);if(cleanup.cleanupError)throw Object.assign(new Error('Inspect primary and every action cleanup result'),{cleanup});}
}
