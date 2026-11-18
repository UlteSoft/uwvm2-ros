"use strict";
// Exercise the real editor controller with a bounded metadata transport fixture.
// This verifies routing/lifetime; it is not a graphical VS Code acceptance run.
const assert = require("node:assert/strict");
const path = require("node:path");
const {createController} = require("../../tools/debug/inspection_ui");
const {view: modelView} = require("../../tools/debug/inspection_model");
const tick = () => new Promise(resolve => setTimeout(resolve, 25));
const disposable = {dispose() {}};
let trackerFactory, stop = "41", permission = "read-only", wrongStop = false;
const queries = [], commands = new Map();
const label = text => ({text, bytesBase64: Buffer.from(text).toString("base64"), truncated:false});
const session = {id:"scene", type:"uwvm-llvm-full", name:"robot", async customRequest(command, args) {
  queries.push({command,args});
  if (command === "uwvm/permissions") return {permissions:permission};
  if (command === "uwvm/diagnostics") return {text:`stopped: cooperative pause\nstop-id ${stop}\n`};
  if (command === "uwvm/hostInterfaces") return {builds:{u2rosv2:true}};
  if (command === "uwvm/hostTrace") {
    const expected={family:"rosv2",after:"0",ifStop:stop,count:8};
    if (args.result !== undefined) Object.assign(expected,{module:"main",result:"13"});
    assert.deepEqual(args,expected);
    return {stopId:stop,journal:{enabled:"8","rosv2-dropped":"3"},
      calls:[{id:"9",function:"abi",result:"2",segments:"0",args:[]}],next:"9",more:false};
  }
  if (command !== "uwvm/sceneState") throw Error("unexpected side effect route: " + command);
  assert.deepEqual(Object.keys(args).sort(), ["count","ifStop","selection","start"]);
  assert.equal(args.count,8); assert.equal(args.ifStop,stop);
  if (args.selection === "policy") {
    return {selection:"policy",available:true,status:"available",scope:"process-owned",stopId:stop,
      start:0,total:1,next:1,more:false,payloadsRead:false,nativeOperationsIssued:false,
      rows:[{kind:"policy",environment:label("robot"),operation:"permission.report",configured:"unset",effective:"allow",origin:"self-default"}]};
  }
  if (args.selection === "audit") {
    return {selection:"audit",available:true,status:"available",scope:"process-owned",stopId:stop,
      rows:[{configured:true,admitted:"1",limited:"18446744073709551615",busy:"0",clock_failed:"0",sink_failed:"1"}],
      total:1,next:1,more:false,start:0};
  }
  assert.ok(["authority","resources"].includes(args.selection));
  assert.ok(Number.isInteger(args.start) && args.start >= 0 && args.start <= 4096);
  const total = args.selection === "authority" ? 10 : 9, next = Math.min(total,args.start+8);
  return {selection:args.selection,available:true,status:"available",scope:"process-owned",stopId:wrongStop ? "40" : stop,
    rows:Array.from({length:Math.max(0,next-args.start)},(_,i)=>({kind:args.selection==="authority"?"grant":"resource",
      alias:label("signal"+(args.start+i)), id:args.selection==="resources"&&args.start+i>=3?"0":"18446744073709551615", environment:label("robot"),
      binding:label(args.selection==="resources"&&args.start+i===8?"local-diagnostic":args.selection==="resources"&&args.start+i===7?"private-log":args.selection==="resources"&&args.start+i===6?"local-transform":args.selection==="resources"&&args.start+i===5?"local-model":args.selection==="resources"&&args.start+i===4?"sealed-buffer":args.selection==="resources"&&args.start+i===3?"local-lifecycle":"private"),
      members:args.selection==="resources"&&args.start+i===6?"3":args.selection==="resources"&&args.start+i===4?"2":"0",
      open_handles:args.selection==="resources"&&args.start+i===6?"2":args.selection==="resources"&&args.start+i===4?"1":"0",
      type:label(args.selection==="authority"?"model-weights":["artifact","artifact.output","artifact.receipt","lifecycle","buffer","model","transform","log","diagnostic"][args.start+i]),
      state:label(args.selection==="authority"?"":["","staged","removed","configuring","","","","",""][args.start+i]),
      size_bytes:args.selection==="resources"&&args.start+i===0?"18446744073709551615":"0",
      immutable:args.selection==="resources"&&args.start+i===0,
      accepted_bytes:args.selection==="resources"&&[1,2].includes(args.start+i)?"9007199254740993":"0",
      reserved_bytes:args.selection==="resources"&&[1,4,6].includes(args.start+i)?"18446744073709551615":"0",
      retained_receipts:args.selection==="resources"&&[1,2].includes(args.start+i)?"1":"0",published:false,durable:false,
      generation:args.selection==="resources"&&args.start+i===3?"9007199254740993":"0",
      transition:args.selection==="resources"&&args.start+i===3?"1":"0",
      deadline_ns:args.selection==="resources"&&args.start+i===3?"9223372036854775807":"0",status_code:"0",
      lease_count:args.selection==="resources"&&args.start+i===4?"18446744073709551615":"0",
      workspace_bytes:args.selection==="resources"&&[4,6].includes(args.start+i)?"9007199254740993":"0",
      input_bytes:args.selection==="resources"&&args.start+i===5?"9007199254740993":"0",
      output_bytes:args.selection==="resources"&&args.start+i===5?"9223372036854775807":"0",
      output_items:args.selection==="resources"&&args.start+i===5?"18446744073709551615":"0",
      sample_count:args.selection==="resources"&&args.start+i===6?"9007199254740993":"0",
      query_count:args.selection==="resources"&&args.start+i===6?"18446744073709551615":"0",
      pending_queries:args.selection==="resources"&&args.start+i===6?"9007199254740993":"0",
      query_bytes:args.selection==="resources"&&args.start+i===6?"9223372036854775807":"0",
      carrier_bytes:args.selection==="resources"&&args.start+i===6?"18446744073709551615":"0",
      emitted_records:args.selection==="resources"&&args.start+i===7?"18446744073709551615":"0",
      exported_records:args.selection==="resources"&&args.start+i===7?"9007199254740993":"0",
      lost_records:args.selection==="resources"&&args.start+i===7?"9007199254740993":"0",
      retained_bytes:args.selection==="resources"&&args.start+i===7?"1024":"0",
      copy_failures:args.selection==="resources"&&args.start+i===7?"18446744073709551615":"0",
      counters_saturated:args.selection==="resources"&&args.start+i===7,
      pending_page:args.selection==="resources"&&args.start+i===8,busy:args.selection==="resources"&&args.start+i>=7,
      callback_pending:args.selection==="resources"&&args.start+i===3,callback_delivered:false})),
    total,next,more:next<total,start:args.start,payloadsRead:false,nativeOperationsIssued:false};
}};
const vscode = {
  Uri:{file:p=>({toString:()=>p})}, ViewColumn:{One:1,Active:-1,Beside:-2}, ThemeIcon:class{},
  workspace:{getConfiguration:()=>({get:(_key,fallback)=>fallback})},
  commands:{registerCommand:(name,handler)=>(commands.set(name,handler),disposable)},
  window:{tabGroups:{activeTabGroup:{tabs:[],viewColumn:1}},createWebviewPanel(_kind,_title,column){
    const panel={viewColumn:column,active:true,visible:true,messages:[],
      webview:{asWebviewUri:x=>x,cspSource:"test",postMessage:x=>panel.messages.push(x),onDidReceiveMessage:fn=>(panel.receive=fn,disposable)},
      onDidChangeViewState:fn=>(panel.show=fn,disposable),onDidDispose:fn=>(panel.end=fn,disposable),
      reveal(){},dispose:()=>panel.end?.()};return panel;
  }},
  debug:{activeDebugSession:session, onDidTerminateDebugSession:()=>disposable,
    registerDebugAdapterTrackerFactory:(_name,factory)=>(trackerFactory=factory,disposable)}
};
(async()=>{
  const controller=createController(vscode,{extensionPath:path.resolve(__dirname,"../../tools/debug"),subscriptions:[]},new Map([[session.id,session]]));
  assert.equal(typeof commands.get("uwvm.openScene"),"function");
  const panel=await commands.get("uwvm.openScene")(); panel.receive({action:"ready"});
  const view=()=>panel.messages.filter(x=>x.action==="view").at(-1).view;
  assert.equal(view().scene.view.rows.length,8);
  assert.equal(view().scene.view.rows[0].id,"18446744073709551615");
  panel.receive({action:"group-next",epoch:view().epoch});await tick();
  assert.equal(view().scene.view.rows.length,2);assert.equal(view().scene.view.start,8);
  panel.receive({action:"group-previous",epoch:view().epoch});await tick();
  assert.equal(view().scene.view.start,0);
  panel.receive({action:"scene-group",group:"resources",epoch:view().epoch});await tick();
  assert.equal(view().scene.active,"resources");assert.equal(view().scene.view.rows.length,8);
  assert.equal(view().scene.view.rows[0].size_bytes,"18446744073709551615","artifact sizes retain exact uint64 strings");
  assert.equal(view().scene.view.rows[0].immutable,true);
  assert.ok(view().scene.view.columns.includes("size_bytes"));assert.ok(view().scene.view.columns.includes("immutable"));
  assert.equal(view().scene.view.rows[1].accepted_bytes,"9007199254740993","accepted bytes are not rounded by JavaScript");
  assert.equal(view().scene.view.rows[1].reserved_bytes,"18446744073709551615");
  assert.equal(view().scene.view.rows[1].state.text,"staged");assert.equal(view().scene.view.rows[2].state.text,"removed");
  assert.equal(view().scene.view.rows[2].retained_receipts,"1");assert.equal(view().scene.view.rows[2].reserved_bytes,"0");
  for(const field of ["state","accepted_bytes","reserved_bytes","retained_receipts","published","durable"])
    assert.ok(view().scene.view.columns.includes(field));
  assert.equal(view().scene.view.rows[3].state.text,"configuring");
  assert.equal(view().scene.view.rows[3].generation,"9007199254740993","lifecycle generations retain exact uint64 strings");
  assert.equal(view().scene.view.rows[3].deadline_ns,"9223372036854775807");
  assert.equal(view().scene.view.rows[3].callback_pending,true);
  assert.equal(view().scene.view.rows[3].callback_delivered,false);
  for(const field of ["generation","transition","deadline_ns","status_code","callback_pending","callback_delivered"])
    assert.ok(view().scene.view.columns.includes(field));
  assert.equal(view().scene.view.rows[4].type.text,"buffer");
  assert.equal(view().scene.view.rows[4].id,"0");assert.equal(view().scene.view.rows[4].members,"2");
  assert.equal(view().scene.view.rows[4].open_handles,"1");assert.equal(view().scene.view.rows[4].reserved_bytes,"18446744073709551615");
  assert.equal(view().scene.view.rows[4].binding.text,"sealed-buffer");
  assert.equal(view().scene.view.rows[4].lease_count,"18446744073709551615");
  assert.equal(view().scene.view.rows[4].workspace_bytes,"9007199254740993");
  for(const field of ["lease_count","workspace_bytes"])assert.ok(view().scene.view.columns.includes(field));
  assert.equal(view().scene.view.rows[5].type.text,"model");
  assert.equal(view().scene.view.rows[5].binding.text,"local-model");
  assert.equal(view().scene.view.rows[5].input_bytes,"9007199254740993");
  assert.equal(view().scene.view.rows[5].output_items,"18446744073709551615");
  for(const field of ["input_bytes","output_bytes","output_items"])assert.ok(view().scene.view.columns.includes(field));
  assert.equal(view().scene.view.rows[6].type.text,"transform");
  assert.equal(view().scene.view.rows[6].binding.text,"local-transform");
  assert.equal(view().scene.view.rows[6].sample_count,"9007199254740993");
  assert.equal(view().scene.view.rows[6].query_count,"18446744073709551615");
  assert.equal(view().scene.view.rows[6].pending_queries,"9007199254740993");
  assert.equal(view().scene.view.rows[6].query_bytes,"9223372036854775807");
  assert.equal(view().scene.view.rows[6].carrier_bytes,"18446744073709551615");
  for(const field of ["sample_count","query_count","pending_queries","query_bytes","carrier_bytes"])
    assert.ok(view().scene.view.columns.includes(field));
  assert.equal(view().scene.view.rows[7].type.text,"log");
  assert.equal(view().scene.view.rows[7].emitted_records,"18446744073709551615");
  assert.equal(view().scene.view.rows[7].lost_records,"9007199254740993");
  assert.equal(view().scene.view.rows[7].counters_saturated,true);
  for(const field of ["emitted_records","exported_records","lost_records","retained_bytes","copy_failures","counters_saturated","pending_page","busy"])
    assert.ok(view().scene.view.columns.includes(field));
  panel.receive({action:"group-next",epoch:view().epoch});await tick();
  assert.equal(view().scene.view.rows[0].type.text,"diagnostic");
  assert.equal(view().scene.view.rows[0].pending_page,true);
  assert.equal(view().scene.view.rows[0].busy,true);
  panel.receive({action:"group-previous",epoch:view().epoch});await tick();

  assert.ok(view().footer.some(item => item.text === "Cached transform counts; inspection does not execute or expire queries"));
  const count=queries.length;
  for (const action of ["write","export","trace-on","trace-off","trace-clear","call-payload","dump","breakpoint-create","revoke",
    "artifact.open","artifact.read","artifact.describe","artifact.begin","artifact.append","artifact.commit",
    "artifact.abort","artifact.remove","artifact.status","artifact.receipt","receipt-ack","receipt-clear",
    "model.open","model.submit","model.take","model.cancel","model.describe","request.describe","request.release",
    "transform.open","transform.publish","transform.lookup","transform.available","transform.velocity","transform.submit","transform.take","transform.cancel","transform.clear",
    "buffer.create","buffer.write","buffer.seal","buffer.read","buffer.describe","artifact.publish",
    "recording.open","recording.append","recording.checkpoint","recording.stop","replay.open","replay.read","replay.seek",
    "sensor.open","sensor.take","sensor.describe","proposal.capture","proposal.publish","proposal.read","proposal.accept",
    "lifecycle.open","lifecycle.state","lifecycle.transitions","lifecycle.submit","lifecycle.take","lifecycle.respond"])
    panel.receive({action,epoch:view().epoch,fields:{hex:"00",dumpPath:"/tmp/not-issued"}});
  await tick();assert.equal(queries.length,count,"scene messages cannot reach payload or effect requests");
  panel.receive({action:"scene-group",group:"policy",epoch:view().epoch});await tick();
  assert.equal(view().scene.view.rows[0].origin,"self-default");
  assert.equal(queries.filter(q=>q.command==="uwvm/sceneState").at(-1).args.selection,"policy");
  assert.ok(view().scene.view.columns.includes("effective"));
  panel.receive({action:"scene-group",group:"audit",epoch:view().epoch});await tick();
  assert.equal(queries.filter(q=>q.command==="uwvm/sceneState").at(-1).args.selection,"audit");
  assert.equal(view().scene.view.rows[0].__raw.limited,"18446744073709551615");
  panel.receive({action:"scene-group",group:"journal",epoch:view().epoch});await tick();
  assert.equal(view().scene.active,"journal");
  assert.equal(view().scene.view.journal["rosv2-dropped"],"3");
  assert.equal(view().scene.view.calls[0].result,"2");
  panel.receive({action:"scene-trace-filter",fields:{module:"main",result:"13"},epoch:view().epoch});await tick();
  assert.deepEqual(view().scene.filters,{module:"main",result:"13"});
  assert.equal(queries.filter(q=>q.command==="uwvm/hostTrace").at(-1).args.result,"13");
  const beforePayload=queries.length;
  panel.receive({action:"payload",fields:{call:"9",segment:"0",offset:"0",count:"8"},epoch:view().epoch});await tick();
  assert.equal(queries.length,beforePayload,"metadata-only calls have no payload read route");
  panel.receive({action:"scene-group",group:"resources",epoch:view().epoch});await tick();
  const beforeGroup=queries.length;
  panel.receive({action:"scene-group",group:"__proto__",epoch:view().epoch});await tick();
  assert.equal(queries.length,beforeGroup);
  wrongStop=true;panel.receive({action:"refresh",epoch:view().epoch});await tick();
  assert.ok(view().banners.some(b=>/retired stop/.test(b.text)));
  assert.equal(view().session.stopId,undefined);
  wrongStop=false;stop="42";panel.receive({action:"refresh",epoch:view().epoch});await tick();
  assert.equal(view().session.stopId,"42");
  const tracker=trackerFactory.createDebugAdapterTracker(session);
  panel.visible=false;panel.active=false;
  const beforeHidden=queries.filter(q=>q.command==="uwvm/sceneState").length;
  tracker.onDidSendMessage({type:"event",event:"stopped",body:{}});await tick();await tick();
  assert.equal(queries.filter(q=>q.command==="uwvm/sceneState").length,beforeHidden,"hidden scene inspectors do not issue metadata queries");
  panel.visible=true;panel.active=true;panel.show();await tick();
  permission="none";
  const beforeDenied=queries.filter(q=>q.command==="uwvm/sceneState").length;
  panel.receive({action:"refresh",epoch:view().epoch});await tick();
  assert.equal(queries.filter(q=>q.command==="uwvm/sceneState").length,beforeDenied);
  assert.equal(modelView({id:3,kind:"scene",epoch:1,sceneGroup:"authority"}).session.connected,false);
  const privateRows=["recording","replay","sensor","model.proposal"].map((type,index)=>({
    type:label(type),binding:label(["private-recording","private-replay","bounded-sensor","model-proposals"][index]),
    state:label("active"),id:"0",members:"2",open_handles:index===3?"0":"2",sample_count:"9007199254740993",
    lease_count:"18446744073709551615",workspace_bytes:"9007199254740993"}));
  const privateView=modelView({id:4,kind:"scene",epoch:1,sceneGroup:"resources",data:{internal:{
    rows:privateRows,total:4,next:4,start:0,more:false,available:true,status:"available"}}});
  assert.deepEqual(privateView.scene.view.rows.map(row=>row.type.text),["recording","replay","sensor","model.proposal"]);
  assert.equal(privateView.scene.view.rows[2].lease_count,"18446744073709551615");
  assert.equal(privateView.scene.view.rows[3].workspace_bytes,"9007199254740993");
  for(const text of ["does not read messages or advance replay cursors","does not read a device or consume frames","executes no proposed operation"])
    assert.ok(privateView.footer.some(item=>item.text.includes(text)));
  const communicationView=modelView({id:5,kind:"scene",epoch:1,sceneGroup:"resources",data:{internal:{
    rows:[{type:label("communication"),binding:label("native-communication"),state:label("ready"),
      id:"0",members:"1",open_handles:"1",retained_receipts:"1"}],
    total:1,next:1,start:0,more:false,available:true,status:"available"}}});
  assert.equal(communicationView.scene.view.rows[0].type.text,"communication");
  assert.ok(communicationView.footer.some(item=>item.text.includes("viewing performs no transport polling")));
  const parameterView=modelView({id:6,kind:"scene",epoch:1,sceneGroup:"resources",data:{internal:{
    rows:[{type:label("parameter"),binding:label("local-parameters"),state:label(""),
      id:"0",members:"2",open_handles:"1",sample_count:"5",query_count:"3",retained_bytes:"9007199254740993",
      workspace_bytes:"18446744073709551615",input_bytes:"1024",output_items:"4"}],
    total:1,next:1,start:0,more:false,available:true,status:"available"}}});
  assert.equal(parameterView.scene.view.rows[0].retained_bytes,"9007199254740993");
  assert.equal(parameterView.scene.view.rows[0].workspace_bytes,"18446744073709551615");
  assert.ok(parameterView.footer.some(item=>item.text.includes("expires no validation and advances no cursor")));
  const platformView=modelView({id:7,kind:"scene",epoch:1,sceneGroup:"authority",data:{internal:{
    rows:[{type:label("device"),binding:label("framed-scalar"),platform:label("uwvm_ros_v2.platform_extension.fixture.v1"),
      platform_enabled:true,revoked:true,open_handles:"0"}],total:1,next:1,start:0,more:false,available:true,status:"available"}}});
  assert.equal(platformView.scene.view.rows[0].platform.text,"uwvm_ros_v2.platform_extension.fixture.v1");
  assert.equal(platformView.scene.view.rows[0].platform_enabled,true);
  assert.equal(platformView.scene.view.rows[0].revoked,true);
  assert.ok(platformView.footer.some(item=>item.text.includes("enabling a module does not grant device authority")));
  const componentView=modelView({id:8,kind:"scene",epoch:1,sceneGroup:"resources",data:{internal:{
    rows:[{type:label("component"),binding:label("isolated-roles"),state:label("failed"),
      status_code:"4",exit_observed:true,members:"0"}],total:1,next:1,start:0,more:false,available:true,status:"available"}}});
  assert.equal(componentView.scene.view.rows[0].status_code,"4");
  assert.ok(componentView.footer.some(item=>item.text.includes("Stop and exit never clear it")));
  const driverView=modelView({id:9,kind:"scene",epoch:1,sceneGroup:"resources",data:{internal:{
    rows:[{type:label("driver"),binding:label("registered-driver"),state:label("failed"),
      sequence:"9007199254740993",query_count:"1",status_code:"7",armed:false}],
    available:true,status:"available",selection:"resources",total:1,start:0,next:1,more:false}}});
  assert.equal(driverView.scene.view.rows[0].sequence,"9007199254740993");
  assert.equal(driverView.scene.view.rows[0].query_count,"1");
  assert.ok(driverView.footer.some(item=>item.text.includes("VM disconnect never stops the shared driver")));
  for (const clear of [true,false]) {
    const exchangeView=modelView({id:9,kind:"scene",epoch:1,sceneGroup:"resources",data:{internal:{
      rows:[{type:label("module.call"),binding:label("registered-message"),clear_after_copy:clear}],
      total:1,next:1,start:0,more:false,available:true,status:"available"}}});
    assert.equal(exchangeView.scene.view.rows[0].clear_after_copy,clear);
    assert.equal(exchangeView.footer.some(item=>item.text.includes("Input clearing is disabled")),!clear);
  }
  controller.dispose();
  console.log("PASS scene IDE routing, exact IDs, pagination, stale stop, hidden refresh and side-effect exclusion");
})().catch(error=>{console.error(error);process.exitCode=1;});
