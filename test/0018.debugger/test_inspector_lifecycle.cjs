"use strict";
const assert = require("node:assert/strict");
const path = require("node:path");
const {createController} = require("../../tools/debug/inspection_ui");
const tick = () => new Promise(resolve => setTimeout(resolve, 40));
const disposable = {dispose() {}};
let trackerFactory, stackChanged, stop = 41, pendingDump, restorePending = false, restoreResponse;
let threaded = false, joined = false, native = false, pendingRecord, pendingHistory;
let hostBuilds = {"u2rosv1-ros1": true, "u2rosv1-ros2": true, u2rosv2: true, preloaddl: false};
const queries = [], panels = [];
const session = {id:"live", type:"uwvm-llvm-full", name:"fixture", async customRequest(command, args) {
  queries.push({command,args});
  if(command==="uwvm/permissions")return {permissions:"full"};
  if(command==="uwvm/hostInterfaces")return {builds:{...hostBuilds},libraries:[],first:"0",total:"0",next:"0",more:false};
  if(command==="uwvm/diagnostics")return restorePending ? {restorePending:true,text:"Snapshot restoration is pending.\n",details:{State:"restoring"}} :
    joined ? {threaded,text:"guest exited: 0\n",details:{State:"exited"}} : {threaded,cooperative:!native,text:`stopped: pause\nstop-id ${stop}\n`};
  if(command==="uwvm/recordSnapshot")return pendingRecord || {threaded,text:"recorded"};
  if(command==="uwvm/snapshotHistory")return pendingHistory || {historical:true,restoreAuthority:false};
  if(command==="uwvm/restoreSnapshot") { restorePending=true; return restoreResponse(); }
  if(command==="uwvm/dumpPolicy")return {kind:"max",configured:"full",effective:"full",allowed:["diagnostic","full"],immutable:true};
  if(command==="uwvm/dump")return pendingDump || {text:"wasm-dump saved",disclosure:args.disclosure};
  if(command==="uwvm/rosState") {
    if(args.selection!=="policy")return {rows:[],total:0,more:false};
    assert.equal(args.count,4);
    assert.equal(args.ifStop,String(stop));
    const next=Math.min(args.start+args.count,126);
    return {rows:Array.from({length:next-args.start},(_,i)=>({key:`uwvm_ros2_v1.fn${args.start+i}`,value:"allow",backend:"ros2",scope:"group",target:"robots"})),
      total:126,next,more:next<126,stopId:String(stop)};
  }
  throw Error(command);
}};
const vscode={
  Uri:{file:p=>({toString:()=>p})}, ViewColumn:{One:1,Active:-1,Beside:-2},ThemeIcon:class{},
  workspace:{getConfiguration:()=>({get:(_key,fallback)=>fallback})},
  commands:{registerCommand:()=>disposable},
  window:{tabGroups:{activeTabGroup:{tabs:[],viewColumn:1}},createWebviewPanel(_kind,_title,column){
    const p={viewColumn:column,active:true,visible:true,messages:[],reveals:[],
      webview:{asWebviewUri:x=>x,cspSource:"test",postMessage:x=>p.messages.push(x),onDidReceiveMessage:fn=>(p.receive=fn,disposable)},
      onDidChangeViewState:fn=>(p.show=fn,disposable),onDidDispose:fn=>(p.end=fn,disposable),
      reveal:(column,preserve)=>p.reveals.push({column,preserve}),dispose:()=>p.end?.()};
    panels.push(p);return p;
  }},
  debug:{activeDebugSession:session,activeStackItem:{session,frameId:10},
    onDidChangeActiveStackItem:fn=>(stackChanged=fn,disposable),
    onDidTerminateDebugSession:()=>disposable,
    registerDebugAdapterTrackerFactory:(_name,factory)=>(trackerFactory=factory,disposable)}
};
(async()=>{
  const controller=createController(vscode,{extensionPath:path.resolve(__dirname,"../../tools/debug"),subscriptions:[]},new Map([[session.id,session]]));
  const panel=await controller.open("ros");panel.receive({action:"ready"});
  const view=()=>panel.messages.filter(x=>x.action==="view").at(-1).view;
  assert.equal(view().ros.policyRows.length,64);
  assert.equal(view().ros.policy.total,126);
  panel.receive({action:"policy-next",epoch:view().epoch});await tick();
  assert.equal(view().ros.policyRows.length,62);
  assert.equal(view().ros.policy.start,64);
  assert.equal(view().ros.policy.more,false);

  const tracker=trackerFactory.createDebugAdapterTracker(session);
  panel.visible=false;panel.active=false;stop++;
  const policyReads=()=>queries.filter(q=>q.command==="uwvm/rosState").length;
  const before=policyReads();
  tracker.onDidSendMessage({type:"event",event:"stopped",body:{}});await tick();await tick();
  assert.equal(policyReads(),before,"hidden inspectors do not fetch all policy rows on every step");
  panel.visible=true;panel.active=true;panel.show();await tick();
  assert.equal(view().session.stopId,String(stop));
  assert.equal(view().ros.policyRows.length,64);

  tracker.onWillReceiveMessage({type:"request",command:"setVariable",seq:1});
  stop++;
  tracker.onDidSendMessage({type:"event",event:"stopped",body:{uwvmStateMutation:true}});
  stackChanged({session,frameId:10});assert.equal(panel.reveals.length,0);
  stackChanged({session,frameId:11});assert.deepEqual(panel.reveals,[{column:panel.viewColumn,preserve:true}]);
  stackChanged({session,frameId:12});assert.equal(panel.reveals.length,1,"later navigation is never pulled back");

  tracker.onWillReceiveMessage({type:"request",command:"setVariable",seq:2});
  tracker.onDidSendMessage({type:"response",request_seq:2,success:false});
  stackChanged({session,frameId:13});assert.equal(panel.reveals.length,1,"failed edits do not schedule navigation");
  tracker.onWillReceiveMessage({type:"request",command:"uwvm/nativeEntry",seq:3});
  tracker.onDidSendMessage({type:"event",event:"stopped",body:{uwvmInspectionStop:true}});
  stackChanged({session,frameId:14});assert.equal(panel.reveals.length,2,"native capture keeps its inspector visible");
  await tick();
  const diagnostics=await controller.open("diagnostics");diagnostics.receive({action:"ready"});
  const dumpView=()=>diagnostics.messages.filter(x=>x.action==="view").at(-1).view;
  assert.equal(dumpView().diagnostics.canDump,true);
  assert.equal(dumpView().diagnostics.disclosure,"full");
  diagnostics.receive({action:"dump-disclosure",epoch:dumpView().epoch,fields:{disclosure:"diagnostic",dumpPath:"/private/robot.uwsnap"}});await tick();
  assert.equal(dumpView().diagnostics.disclosure,"diagnostic");
  diagnostics.receive({action:"dump",epoch:dumpView().epoch,fields:{disclosure:"diagnostic",dumpPath:"/private/robot.uwsnap"}});await tick();
  assert.deepEqual(queries.findLast(q=>q.command==="uwvm/dump").args,{path:"/private/robot.uwsnap",stopId:String(stop),disclosure:"diagnostic"});
  assert.equal(dumpView().diagnostics.dumpReply.disclosure,"diagnostic");
  let firstDone, secondDone;
  pendingDump=new Promise(resolve=>{firstDone=resolve;});
  diagnostics.receive({action:"dump",epoch:dumpView().epoch,fields:{disclosure:"diagnostic",dumpPath:"/private/old.uwsnap"}});await tick();
  assert.equal(dumpView().busy,true);
  diagnostics.receive({action:"bind",epoch:dumpView().epoch});await tick();
  assert.equal(dumpView().diagnostics.dumpReply,undefined);
  assert.equal(dumpView().busy,false,"rebind releases the retired export UI operation");
  pendingDump=new Promise(resolve=>{secondDone=resolve;});
  diagnostics.receive({action:"dump",epoch:dumpView().epoch,fields:{disclosure:"full",dumpPath:"/private/new.uwsnap"}});await tick();
  firstDone({text:"old export finished",disclosure:"diagnostic"});await tick();
  assert.equal(dumpView().diagnostics.dumpReply,undefined,"old export does not publish into a rebound panel");
  assert.equal(dumpView().busy,true,"old completion cannot unlock the new export");
  secondDone({text:"new export finished",disclosure:"full"});await tick();
  assert.equal(dumpView().diagnostics.dumpReply.text,"new export finished");
  assert.equal(dumpView().busy,false);
  restoreResponse=()=>{throw new Error("transport timeout after dispatch");};
  diagnostics.receive({action:"restore-snapshot",epoch:dumpView().epoch,
    fields:{restorePath:"/private/full.uwsnap",restoreEnvironment:"current"}});await tick();
  assert.equal(dumpView().diagnostics.restorePending,true,"uncertain transport retains the continuation action");
  assert.equal(dumpView().session.stopId,undefined,"old stop is not exposed while retirement is pending");
  assert.equal(dumpView().diagnostics.canDump,false);
  diagnostics.receive({action:"bind",epoch:dumpView().epoch});await tick();
  assert.equal(dumpView().diagnostics.restorePending,true,"rebound panel learns pending state from the adapter");
  restoreResponse=()=>{restorePending=false;stop++;return {applied:true,pending:false};};
  diagnostics.receive({action:"continue-restore",epoch:dumpView().epoch});await tick();
  assert.equal(dumpView().diagnostics.restorePending,false);
  assert.equal(dumpView().session.stopId,String(stop));
  assert.equal(dumpView().diagnostics.canDump,true);
  threaded=true; joined=true;
  tracker.onDidSendMessage({type:"event",event:"uwvm/threadReplay",body:{state:"joined",exitCode:0}});await tick();
  assert.equal(dumpView().session.stopId,"0","joined cohort retains a host management stop");
  assert.equal(dumpView().diagnostics.canRecord,true);
  assert.equal(dumpView().diagnostics.canRestore,true);
  assert.equal(dumpView().diagnostics.restoreEnvironment,"replay");
  assert.equal(dumpView().diagnostics.threaded,true);
  restoreResponse=()=>{restorePending=false;joined=false;stop++;return {applied:true,pending:false,threaded:true};};
  diagnostics.receive({action:"restore-snapshot",epoch:dumpView().epoch,
    fields:{restorePath:"/private/threads.uwsnap",restoreEnvironment:"replay",replayCursor:"7"}});await tick();
  assert.deepEqual(queries.findLast(q=>q.command==="uwvm/restoreSnapshot").args,
    {path:"/private/threads.uwsnap",environment:"replay",stopId:"0",bindings:[],cursor:"7"});
  for(const kind of ["history","recording"]) {
    let finish;
    const promise=new Promise(resolve=>{finish=resolve;});
    if(kind==="history")pendingHistory=promise;else pendingRecord=promise;
    diagnostics.receive({action:kind==="history"?"snapshot-history":"record-snapshot",epoch:dumpView().epoch,
      fields:kind==="history"?{}:{action:"status"}});await tick();
    assert.equal(dumpView().busy,true);
    diagnostics.receive({action:"bind",epoch:dumpView().epoch});await tick();
    assert.equal(dumpView().busy,false,"rebinding releases pending "+kind);
    finish({text:"stale",historical:true});await tick();
    assert.equal(dumpView().diagnostics[kind==="history"?"historyReply":"recordingReply"],undefined);
    pendingHistory=pendingRecord=undefined;
  }
  let recordDone;
  pendingRecord=new Promise(resolve=>{recordDone=resolve;});
  diagnostics.receive({action:"record-snapshot",epoch:dumpView().epoch,fields:{action:"status"}});await tick();
  joined=true;tracker.onDidSendMessage({type:"event",event:"uwvm/threadReplay",body:{state:"joined"}});
  recordDone({text:"recorded"});await tick();await tick();pendingRecord=undefined;
  assert.equal(dumpView().session.stopId,"0","joined event during a request is refreshed on completion");
  tracker.onWillReceiveMessage({type:"request",command:"uwvm/threadReplay",arguments:{action:"replay"},seq:51});
  diagnostics.receive({action:"ready"});
  assert.equal(dumpView().diagnostics.canRestore,false,"an external replay immediately retires old controls");
  tracker.onDidSendMessage({type:"response",request_seq:51,success:false});await tick();
  assert.equal(dumpView().session.stopId,"0","a rejected replay reauthenticates the actual cohort");
  joined=false; native=true; stop++;
  const readsAtNative=policyReads();
  const nativeRos=await controller.open("ros");nativeRos.receive({action:"ready"});
  const nativeView=()=>nativeRos.messages.filter(x=>x.action==="view").at(-1).view;
  assert.equal(policyReads(),readsAtNative,"native inspector never attempts unsafe host snapshot reads");
  assert(nativeView().banners.some(b=>b.text.startsWith("Assembly stop:")));
  assert(!nativeView().banners.some(b=>b.level==="error"));
  assert.equal(nativeView().session.stopId,undefined);
  native=false;stop++;nativeRos.receive({action:"refresh",epoch:nativeView().epoch});await tick();
  assert.equal(nativeView().ros.policyRows.length,64,"cooperative stop restores the inspector");
  native=true;stop++;nativeRos.receive({action:"refresh",epoch:nativeView().epoch});await tick();
  assert.equal(nativeView().ros.policyRows.length,64,"native stop retains the matching historical selection");
  assert(nativeView().banners.some(b=>b.text.includes("not live")));
  assert.equal(nativeView().permission.canWrite,false);
  native=false;
  const readsBeforeDisabled=policyReads();
  hostBuilds={"u2rosv1-ros1":false,"u2rosv1-ros2":false,u2rosv2:false,preloaddl:false};
  joined=true;stop++;
  const disabledRos=await controller.open("ros");disabledRos.receive({action:"ready"});
  const disabledView=disabledRos.messages.filter(x=>x.action==="view").at(-1).view;
  assert.match(disabledView.ros.unavailable,/not included in this VM build/);
  assert.equal(policyReads(),readsBeforeDisabled,"compiled-out u2rosv1 never queries legacy runtime state");
  const disabledScene=await controller.open("scene");disabledScene.receive({action:"ready"});
  assert.match(disabledScene.messages.filter(x=>x.action==="view").at(-1).view.scene.unavailable,/not included/);
  assert.equal(queries.filter(q=>q.command==="uwvm/sceneState").length,0,"compiled-out u2rosv2 does not query its providers");
  const preload=await controller.open("preloaddl");preload.receive({action:"ready"});
  assert.equal(preload.messages.filter(x=>x.action==="view").at(-1).view.preloaddl.builds.preloaddl,false);
  assert(!preload.messages.filter(x=>x.action==="view").at(-1).view.banners.some(row=>row.level==="error"),
    "launch inventory is available after the cohort joined");
  controller.dispose();
  console.log("PASS complete policy pagination, hidden refresh, fresh frame restoration and no later focus theft");
})().catch(error=>{console.error(error);process.exitCode=1;});
