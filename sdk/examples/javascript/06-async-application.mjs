/**
 * @file 06-async-application.mjs
 * @brief Assemble every supported client-library family from explicit aliases.
 *
 * The caller supplies an already attached AsyncSession and a literal plan.
 * An empty plan opens no authority. Native resource names, type identities,
 * QoS, quotas, deadlines and allowed lifecycle transitions stay in trusted CLI
 * configuration. This lesson performs no process launch or automatic spin.
 */
import { AsyncContext, AsyncExecutor, AsyncTimer, CallbackGroup, Status } from '../../javascript/async.mjs';
import { StringMessage, TriggerRequest, ServiceResponse, Parameter, TypedParameterEvent } from '../../javascript/codecs.mjs';

/** Build a local application transaction. Every requested factory is awaited;
 * if a later factory fails, Context cleanup attempts every earlier acquisition.
 * Callback groups are local exclusion guards, not native ROS callback groups.
 *
 * @param {AsyncSession} session Explicit qualified relay attachment.
 * @param {object} plan Optional pub/sub/client/service/actionClient/actionServer/
 * parameters/graph/clocks/tf/lifecycle/logger aliases. Supply only granted ones.
 * @param {object} hooks Local callbacks; timerReady is explicit and may receive
 * an already granted Clock. No implicit Clock alias or host timer is created.
 * @returns {Promise<object>} Context, Node, Executor and selected typed objects.
 */
export async function createApplication(session,plan={},hooks={}) {
  const context=new AsyncContext(session),node=context.createNode('async-application');
  const executor=new AsyncExecutor({maxPending:32,maxTasksPerPass:1}),objects={};
  const group=new CallbackGroup();
  try {
    if(plan.pub)objects.publisher=await node.createPublisher(plan.pub,StringMessage);
    if(plan.sub) {
      objects.subscription=await node.createSubscription(plan.sub,StringMessage,
        (value,result)=>group.runAsync(hooks.message??(()=>{}),value,result));
      executor.add(objects.subscription);
    }
    if(plan.client)objects.client=await node.createClient(plan.client,TriggerRequest,ServiceResponse);
    if(plan.service) {
      objects.service=await node.createService(plan.service.advertise,plan.service.respond,
        request=>group.runAsync(hooks.service??(()=>({success:true,message:'explicit local handler'})),request));
      executor.add(objects.service);
    }
    if(plan.actionClient)objects.actionClient=await node.createActionClient(plan.actionClient);
    if(plan.actionServer) {
      // The built-in server handles bounded Fibonacci goals. A production
      // handler chooses kind1/2/3/4 and a <=48 nonnegative i32 sequence.
      objects.actionServer=await node.createActionServer(plan.actionServer.advertise,plan.actionServer.respond,
        goal=>group.runAsync(hooks.action??(()=>({kind:3,values:[]})),goal));
      executor.add(objects.actionServer);
    }
    if(plan.parameters) {
      objects.parameters=await node.createParameters(plan.parameters,session.version===1?Parameter.Int32:Parameter.Int64);
      if(hooks.parameterEvent)objects.parameters.onEvent(event=>group.runAsync(hooks.parameterEvent,event));
      if(plan.parameters.events) {
        const status=await objects.parameters.subscribeEvents();if(status)throw new Error(`Event activation status ${status}`);
        executor.add({async spinOnce(){return (await objects.parameters.pollEvent(TypedParameterEvent)).status;}});
      }
    }
    if(plan.graph)objects.graph=await node.createGraph(plan.graph.alias,plan.graph.operation);
    if(plan.clocks) {
      objects.clocks=[];
      for(const clock of plan.clocks)objects.clocks.push(await node.createClock(clock.alias,clock.operation));
    }
    if(plan.tf)objects.tf=await node.createTf(plan.tf.lookup??null,plan.tf.broadcast??null);
    if(plan.lifecycle)objects.lifecycle=await node.createLifecycle(plan.lifecycle.get??null,plan.lifecycle.change??null);
    if(plan.logger)objects.logger=await node.createLogger(plan.logger);
    if(plan.resource)objects.resource=await node.createResource(plan.resource.alias,plan.resource.operation);
    if(hooks.timerReady) {
      objects.timer=new AsyncTimer(hooks.timerReady,hooks.timer??(()=>{}),{clock:objects.clocks?.[0]??null,group});
      executor.add(objects.timer);
    }
    return {context,node,executor,objects};
  } catch(error) {
    error.cleanup=await context.shutdown();executor.close();throw error;
  }
}

/** Schedule a service call as local deferred work, then execute exactly one
 * pass. Awaiting the Future alone does not drive this executor. The original
 * native service_call remains a bounded synchronous operation in the Worker.
 * If the plan grants no client, no service call is silently substituted.
 */
export async function callInOnePass(application) {
  if(!application.objects.client)throw new TypeError('Explicit client required');
  const future=application.objects.client.callDeferred(application.executor);
  const statuses=await application.executor.spinOnce(),reply=await future.promise;
  return {statuses,reply};
}

/** Inspect every cleanup item, including thrown failures. Transport shutdown
 * remains an explicit caller choice; actual Worker exit belongs to its owner.
 */
export async function closeApplication(application,{transport=false}={}) {
  application.executor.close();const result=await application.context.shutdown({transport});
  const clean=result.nodes.every(node=>!node.error&&node.results.every(item=>!item.error&&item.status===Status.SUCCESS))&&
    (!transport||!!result.transport&&!result.transport.error&&result.transport.status===Status.SUCCESS);
  return {clean,result};
}
