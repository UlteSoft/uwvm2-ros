/**
 * @file 07-sync-application.mjs
 * @brief Synchronous application assembly for an authority-aware embedder.
 *
 * Pass a real Session, created from the exact admitted Wasm memory/export
 * bridge. This lesson neither creates a WebAssembly.Instance nor substitutes
 * ROS imports with JavaScript. An empty alias plan opens no capability.
 */
import { Context, Executor, Timer, CallbackGroup } from '../../javascript/roswasm.mjs';
import { StringMessage, Parameter, TypedParameterEvent } from '../../javascript/codecs.mjs';

/** Assemble all supported families with explicit caller-owned scheduling.
 * Unsupported-version entries remain status95/refusals; no fallback native
 * implementation is substituted. Handler/callback values are local only.
 */
export function createApplication(session,plan={},hooks={}) {
  const context=new Context(session),node=context.createNode('sync-application');
  const executor=new Executor({maxPending:32,maxTasksPerPass:1}),group=new CallbackGroup(),objects={};
  try {
    if(plan.pub)objects.publisher=node.createPublisher(plan.pub,StringMessage);
    if(plan.sub)executor.add(objects.subscription=node.createSubscription(plan.sub,StringMessage,value=>group.run(hooks.message??(()=>{}),value)));
    if(plan.client)objects.client=node.createClient(plan.client);
    if(plan.service)executor.add(objects.service=node.createService(plan.service.advertise,plan.service.respond,
      request=>group.run(hooks.service??(()=>({success:true,message:'explicit local handler'})),request)));
    if(plan.actionClient)objects.actionClient=node.createActionClient(plan.actionClient);
    if(plan.actionServer)executor.add(objects.actionServer=node.createActionServer(plan.actionServer.advertise,plan.actionServer.respond,
      goal=>group.run(hooks.action??(()=>({kind:3,values:[]})),goal)));
    if(plan.parameters) {
      objects.parameters=node.createParameters(plan.parameters,session.version===1?Parameter.Int32:Parameter.Int64);
      if(hooks.parameterEvent)objects.parameters.onEvent(event=>group.run(hooks.parameterEvent,event));
      if(plan.parameters.events) {
        const status=objects.parameters.subscribeEvents();if(status)throw new Error(`Event activation status ${status}`);
        executor.add({spinOnce(){return objects.parameters.pollEvent(TypedParameterEvent).status;}});
      }
    }
    if(plan.graph)objects.graph=node.createGraph(plan.graph.alias,plan.graph.operation);
    if(plan.clocks)objects.clocks=plan.clocks.map(clock=>node.createClock(clock.alias,clock.operation));
    if(plan.tf)objects.tf=node.createTf(plan.tf.lookup??null,plan.tf.broadcast??null);
    if(plan.lifecycle)objects.lifecycle=node.createLifecycle(plan.lifecycle.get??null,plan.lifecycle.change??null);
    if(plan.logger)objects.logger=node.createLogger(plan.logger);
    if(plan.resource)objects.resource=node.createResource(plan.resource.alias,plan.resource.operation);
    if(hooks.timerReady)executor.add(objects.timer=new Timer(hooks.timerReady,hooks.timer??(()=>{}),{clock:objects.clocks?.[0]??null,group}));
    return {context,node,executor,objects};
  } catch(error) {
    try {error.cleanup=context.shutdown();}catch(cleanup){error.cleanup=cleanup;}
    executor.close();throw error;
  }
}

/** A Future defers the existing bounded service call to one explicit pass.
 * This does not add a native asynchronous service ABI or background thread.
 */
export function callInOnePass(application) {
  if(!application.objects.client)throw new TypeError('Explicit client required');
  const future=application.objects.client.callDeferred(application.executor);
  return {statuses:application.executor.spinOnce(),reply:future.result()};
}
/** Close all resources explicitly. Inspect returned statuses/aggregate errors;
 * local Context shutdown does not constitute a native process exit receipt.
 */
export function closeApplication(application) {application.executor.close();return application.context.shutdown();}
