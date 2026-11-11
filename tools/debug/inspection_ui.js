"use strict";

const path=require("path");
const crypto=require("crypto");
const modelAPI=require("./inspection_model");
const filePicker=require("./file_picker");
const editorStyle=require("./editor_style");

function createController(vscode,context,sessions) {
  const panels=new Map();
  let nextPanelId=0;
  const stopStates=new Map();
  let disabledPermission;
  const dispose=value=>context.subscriptions.push(value);
  function active() {
    const session=vscode.debug.activeDebugSession;
    return session && session.type==="uwvm-llvm-full" && sessions.get(session.id)===session?session:undefined;
  }
  function sessionFor(model) {
    const session=sessions.get(model.sessionId);
    if(!session || session.type!=="uwvm-llvm-full")throw new Error("This inspection window's debug session has ended. Choose Use active session.");
    return session;
  }
  function render(model) {
    if(model.disposed)return;
    if(!model.assets)model.assets={nonce:crypto.randomBytes(18).toString("hex"),cspSource:model.panel.webview.cspSource,
      style:model.panel.webview.asWebviewUri(vscode.Uri.file(path.join(context.extensionPath,"inspection.css"))).toString(),
      values:model.panel.webview.asWebviewUri(vscode.Uri.file(path.join(context.extensionPath,"memory_values.js"))).toString(),
      script:model.panel.webview.asWebviewUri(vscode.Uri.file(path.join(context.extensionPath,"inspection_webview.js"))).toString()};
    model.html=modelAPI.render(model,model.assets);
    if(!model.initialized){model.initialized=true;model.panel.webview.html=model.html;}
    else if(model.ready)void model.panel.webview.postMessage({action:"render",html:model.html});
  }
  function retire(model,message) {
    model.epoch++;model.payload=undefined;model.exportResult=undefined;model.diagnostics=undefined;model.stopId=undefined;model.retired=true;model.message=message||"";model.notice="";model.error="";
    model.policyHistory=[];model.groupHistory=[];model.callbackPages={};
    if(model.kind==="wasip1" || model.kind==="ros"){model.policyPage=0;model.page=0;}
    render(model);
  }
  function bind(model,session) {
    retire(model);model.data=undefined;
    model.sessionId=session && session.id;model.sessionName=session && session.name;model.sessionEverBound=!!session || !!model.sessionEverBound;
    model.permission=session?undefined:disabledPermission;model.showTags=undefined;model.notice="";model.diagnostics=undefined;model.retired=false;model.page=0;model.policyPage=0;model.policyHistory=[];model.groupHistory=[];
    model.writeError="";model.writeResult=undefined;
    model.payloadSelector=undefined;model.exportPath="";
  }
  function current(model,session,epoch) {
    if(model.disposed || model.epoch!==epoch || sessions.get(session.id)!==session || model.sessionId!==session.id)
      throw new Error("Inspection snapshot retired while the request was pending");
  }
  function stopFrom(reply) {
    if(!reply || typeof reply.text!=="string")throw new Error("Invalid debugger diagnostics response");
    const match=/\bstop-id ([0-9]+)\b/.exec(reply.text);
    return match && match[1]!=="0"?modelAPI.decimal(match[1],"Stop identifier"):undefined;
  }
  function stopState(session) {
    if(!stopStates.has(session.id))stopStates.set(session.id,{checking:false,failed:false});
    return stopStates.get(session.id);
  }
  async function checkStop(session) {
    const state=stopState(session);
    if(state.checking || state.failed)return;
    state.checking=true;
    try {
      const stop=stopFrom(await session.customRequest("uwvm/diagnostics",{}));
      if(sessions.get(session.id)!==session || !stop || state.lastStop===stop)return;
      state.lastStop=stop;
      for(const model of panels.values())if(model.sessionId===session.id && model.stopId!==stop) {
        retire(model);
        if(model.busy)model.refreshOnIdle=true;else setTimeout(()=>refresh(model),50);
      }
    } catch(_) {
      state.failed=true;
      for(const model of panels.values())if(model.sessionId===session.id)
        retire(model,"Stop status could not be authenticated. Refresh explicitly before inspecting again.");
    } finally {state.checking=false;}
  }
  async function groupRequest(session,command,args) {
    try {
      const reply=await session.customRequest(command,args);
      if(!reply || typeof reply!=="object" || !Array.isArray(reply.rows||reply.variables||reply.calls))
        throw new Error("The debugger returned an invalid group page");
      if(reply.calls)return {...reply,rows:reply.calls,status:"Retained journal epoch "+reply.journal.epoch+" · enabled family mask "+reply.journal.enabled};
      return reply;
    }
    catch(error) {return {error:String(error.message||error)};}
  }
  async function registers(session) {
    const threads=await session.customRequest("threads",{});
    if(!threads || !Array.isArray(threads.threads) || !threads.threads.length)
      throw new Error("No stopped guest thread. Native registers require a real owned native instruction stop.");
    const stack=await session.customRequest("stackTrace",{threadId:threads.threads[0].id,startFrame:0,levels:32});
    for(const frame of stack.stackFrames||[]) {
      const result=await session.customRequest("scopes",{frameId:frame.id});
      const scope=(result.scopes||[]).find(scope=>scope.presentationHint==="registers" || scope.name==="Registers");
      if(scope) {
        const values=await session.customRequest("variables",{variablesReference:scope.variablesReference,start:0,count:0});
        if(!values || !Array.isArray(values.variables))throw new Error("Invalid register values from the debugger");
        return {variables:values.variables.map(row=>({name:row.name,value:row.value,...(row.type?{type:row.type}:{})})),frameName:frame.name};
      }
    }
    throw new Error("Registers require a native instruction stop. Use native stepping.");
  }
  async function memoryRange(model,session,stop,epoch,previous) {
    const selected=model.selector,reference=`wasm-memory:${selected.module}:${selected.memory}:${selected.offset}`;
    const initial={...selected,count:0};
    let data=previous?{...previous,bytes:previous.bytes.slice(),tags:previous.tags.slice()}:
      modelAPI.decodeMemory(await session.customRequest("uwvm/memoryTags",{memoryReference:reference,count:0,includeBytes:true}),initial);
    current(model,session,epoch);
    if(data.stopId!==stop)throw new Error("Memory belongs to a different stop; refresh the window");
    const available=BigInt(data.memoryBytes)-BigInt(selected.offset);
    if(available<0n)throw new Error("Memory offset is outside the selected memory");
    data.rangeCount=Number(available<BigInt(selected.count)?available:BigInt(selected.count));
    const goal=Math.min(data.rangeCount,data.bytes.length+2048),tags=new Map(data.tags.map(tag=>[tag.offset,tag]));
    while(data.bytes.length<goal) {
      const at=(BigInt(selected.offset)+BigInt(data.bytes.length)).toString(),count=Math.min(256,goal-data.bytes.length);
      const slice={...selected,offset:at,count};
      const page=modelAPI.decodeMemory(await session.customRequest("uwvm/memoryTags",{memoryReference:`wasm-memory:${selected.module}:${selected.memory}:${at}`,count,includeBytes:true}),slice);
      current(model,session,epoch);
      if(page.stopId!==stop || page.memoryBytes!==data.memoryBytes || page.status!==data.status)throw new Error("Memory range changed during its paged copy");
      data.bytes.push(...page.bytes);
      for(const tag of page.tags){const old=tags.get(tag.offset);if(old && old.tag!==tag.tag)throw new Error("Allocation tag changed during its paged copy");tags.set(tag.offset,tag);}
    }
    if(!previous) {
      const inventory=await session.customRequest("uwvm/wasmInventory",{moduleId:Number(selected.module),kind:"memories",first:Number(selected.memory),count:1});
      current(model,session,epoch);
      if(inventory.stopId!==stop || !Array.isArray(inventory.rows) || inventory.rows.length!==1 || ![32,64].includes(inventory.rows[0].addressBits))
        throw new Error("The actual memory declaration could not be authenticated");
      data.addressBits=inventory.rows[0].addressBits;
    }
    data.tags=[...tags.values()];data.count=data.bytes.length;
    data.nextMemoryReference=BigInt(selected.offset)+BigInt(data.rangeCount)<BigInt(data.memoryBytes)?reference:undefined;
    return data;
  }
  async function loadMore(model) {
    if(model.busy || model.retired || !model.data || model.data.bytes.length>=model.data.rangeCount)return;
    const session=sessionFor(model),epoch=model.epoch,stop=model.stopId;model.busy=true;render(model);
    try {
      const data=await memoryRange(model,session,stop,epoch,model.data);
      if(stopFrom(await session.customRequest("uwvm/diagnostics",{}))!==stop)throw new Error("Memory stop retired while loading more rows");
      current(model,session,epoch);model.data=data;
    } catch(error){retire(model);model.error=String(error.message||error);}
    finally{model.busy=false;render(model);}
  }
  async function refresh(model,fields,requestedTags=model.showTags) {
    if(model.busy || model.disposed)return;
    if(fields) {
      if(model.kind==="memory") {
        const selected=modelAPI.memorySelection({...model.selector,...fields});
        if(Object.keys(selected).some(key=>model.selector[key]!==selected[key]))model.data=undefined;
        model.selector=selected;if(fields.columns!==undefined)model.columns=modelAPI.columns(fields.columns);
      }
      if(model.kind==="wasip1" && fields.module!==undefined) {
        const module=modelAPI.decimal(fields.module,"Module",32);
        if(model.module!==module){model.page=0;model.policyPage=0;model.policyHistory=[];model.payloadSelector=undefined;model.payload=undefined;model.exportResult=undefined;}
        model.module=module;
      }
      if(model.kind==="ros" && fields.backend!==undefined) {
        if(!["all","ros1","ros2"].includes(fields.backend))throw new Error("ROS backend must be all, ros1 or ros2");
        if(model.backend!==fields.backend) {model.page=0;model.policyPage=0;model.policyHistory=[];model.payloadSelector=undefined;}
        model.backend=fields.backend;
      }
      if(model.kind==="ros") {
        for(const key of ["policyScope","policyTarget","policyAlias"])if(fields[key]!==undefined) {
          const value=fields[key];
          if(typeof value!=="string" || /[\x00-\x1f]/.test(value) || Buffer.byteLength(value)>1024 ||
             key==="policyScope" && !["all","global","single","group"].includes(value) ||
             key==="policyAlias" && (value.length>255 || /[^\x21-\x7e]/.test(value)))throw new Error("Invalid ROS policy filter: "+key);
          if(model[key]!==value){model.policyPage=0;model.policyHistory=[];}
          model[key]=value;
        }
      }
    }
    model.busy=true;model.retired=true;model.error="";model.payload=undefined;model.exportResult=undefined;model.stopId=undefined;
    const epoch=++model.epoch, pending={};model.pending=pending;
    render(model);
    try {
      if(!model.sessionId) {model.permission=disabledPermission;model.data=undefined;return;}
      const session=sessionFor(model);
      stopState(session).failed=false;
      const permission=await session.customRequest("uwvm/permissions",{});
      if(!permission || !modelAPI.PERMISSIONS.includes(permission.permissions))throw new Error("Invalid authenticated debugger permission reply");
      const before=await session.customRequest("uwvm/diagnostics",{});
      current(model,session,epoch);
      // A permissions-only reply before attach is insufficient. Diagnostics
      // must also complete on the authenticated live broker before UI admission.
      model.permission=permission.permissions;model.diagnostics=before.text;
      if(model.kind==="diagnostics") {model.data=before.details||{Permissions:model.permission,Status:before.text};model.stopId=stopFrom(before);model.retired=false;return;}
      if(!["full","read-only"].includes(model.permission)) {model.data=undefined;model.retired=false;return;}
      const stop=stopFrom(before);
      if(!stop)throw new Error("Pause at a current guest stop before inspecting this window. No stopped snapshot is available.");
      let data;
      if(model.kind==="memory") {
        data=await memoryRange(model,session,stop,epoch);
      } else if(model.kind==="registers")data=await registers(session);
      else if(model.kind==="breakpoints") {
        data=await session.customRequest("uwvm/breakpoints",{});
        if(data?.available===false)throw new Error("The breakpoint inventory is unavailable while the VM is "+data.state+"; existing points were not deleted.");
      }
      else if(model.kind==="wasip1") {
        const module=Number(model.module);
        data={policy:await groupRequest(session,"uwvm/wasip1Policy",{moduleId:module,start:model.policyPage,count:32}),
          environment:model.wasiGroup==="calls"?await groupRequest(session,"uwvm/hostTrace",{family:"wasip1",after:String(model.page),count:8}):
            await groupRequest(session,"uwvm/wasip1State",{selection:model.wasiGroup,moduleId:module,start:model.page,count:32})};
      } else if(model.kind==="ros") {
        data={policy:await groupRequest(session,"uwvm/rosState",{selection:"policy",backend:model.backend,start:model.policyPage,count:4,
            ...(model.policyScope?{scope:model.policyScope}:{}),...(model.policyTarget?{target:model.policyTarget}:{}),...(model.policyAlias?{alias:model.policyAlias}:{})}),
          internal:model.rosGroup==="journal"?(model.backend==="all"?{error:"Select ROS1 or ROS2 to view recorded calls."}:
            await groupRequest(session,"uwvm/hostTrace",{family:model.backend,after:String(model.page),count:8})):
            await groupRequest(session,"uwvm/rosState",model.rosGroup==="trace"?
            {selection:"trace",backend:model.backend,after:model.page,count:8}:{selection:model.rosGroup,backend:model.backend,start:model.page,count:8})};
      }
      const after=await session.customRequest("uwvm/diagnostics",{});
      current(model,session,epoch);
      if(stopFrom(after)!==stop)throw new Error("The guest stop changed while copying the window; refresh for a coherent snapshot");
      stopState(session).lastStop=stop;
      model.data=data;model.stopId=stop;model.retired=false;model.error="";
      if(model.kind==="memory") {
        model.showTags=requestedTags!==false && data.status==="available";
        model.notice=requestedTags===true && !model.showTags?(data.status==="disabled"?
          "Runtime MTE is not enabled. Memtag remains off.":"Memtag is unavailable for this memory. The switch remains off."):"";
      }
      const journal=model.kind==="wasip1"?data.environment:model.kind==="ros"?data.internal:undefined;
      if(journal && journal.calls && journal.calls.length && !model.payloadSelector)model.payloadSelector={call:journal.calls[0].id,segment:"0",offset:"0",count:256};
    } catch(error) {
      if(!model.disposed && model.epoch===epoch) {model.data=undefined;model.stopId=undefined;model.error=String(error.message||error);}
    } finally {
      if(model.pending===pending) {model.pending=undefined;model.busy=false;render(model);
        if(model.refreshOnIdle) {model.refreshOnIdle=false;setTimeout(()=>refresh(model),50);}}
    }
  }
  async function write(model,fields) {
    if(!model || model.disposed || model.busy || model.retired || !model.data || !model.stopId)
      throw new Error("Refresh a stopped memory window before editing");
    if(model.permission!=="full")throw new Error("Writing guest memory requires full debugger permission; current permission is "+model.permission);
    // Capture the displayed authority before the first await. Another command
    // can change a window's selector/session while authentication is pending.
    const session=sessionFor(model),epoch=model.epoch,snapshot=model.data,selected=Object.freeze({...model.selector}),expected=model.stopId;
    const address=modelAPI.decimal(fields.writeOffset===undefined?selected.offset:fields.writeOffset,"Write offset"),bytes=modelAPI.editBytes(fields.hex);
    const begin=BigInt(selected.offset),end=begin+BigInt(snapshot.bytes.length),target=BigInt(address);
    if(target<begin || target+BigInt(bytes.length)>end)throw new Error("The write must fit entirely inside the displayed memory page");
    const reference=`wasm-memory:${selected.module}:${selected.memory}:${address}`;
    const sameSelection=()=>Object.keys(selected).every(key=>model.selector[key]===selected[key]);
    function displayed() {
      current(model,session,epoch);
      if(model.data!==snapshot || model.stopId!==expected || model.retired || model.permission!=="full" || !sameSelection())
        throw new Error("The displayed memory selection or session changed; refresh before editing");
    }
    model.busy=true;model.error="";model.writeError="";model.writeResult=undefined;model.hex=fields.hex;model.writeOffset=address;render(model);
    let reply,committed=false;
    try {
      const permission=await session.customRequest("uwvm/permissions",{});displayed();
      if(permission.permissions!=="full")throw new Error("The authenticated runtime does not authorize memory writes");
      const diagnostic=await session.customRequest("uwvm/diagnostics",{});displayed();
      if(stopFrom(diagnostic)!==expected)throw new Error("The displayed memory stop has retired; refresh before editing");
      // The adapter enforces ifStop itself. The frontend never upgrades the
      // permission or retries an uncertain write after a transport failure.
      reply=await session.customRequest("writeMemory",{memoryReference:reference,
        offset:0,data:bytes.toString("base64"),allowPartial:false,ifStop:expected});
      if(!reply || reply.bytesWritten!==bytes.length)throw new Error("Memory write result is incomplete; inspect memory before retrying");
      committed=true;
      if(sessions.get(session.id)!==session || model.sessionId!==session.id || !sameSelection())
        throw new Error("The original memory write was acknowledged after this window changed. Inspect the original session and selection before further writes.");
      model.message="Committed "+bytes.length+" byte(s) at offset "+address+"; checking read-back…";
      const checked=modelAPI.decodeMemory(await session.customRequest("uwvm/memoryTags",{memoryReference:reference,count:bytes.length,includeBytes:true}),{...selected,offset:address,count:bytes.length});
      if(checked.stopId!==reply.stopId || !Buffer.from(checked.bytes).equals(bytes))
        throw new Error("The commit was acknowledged, but current read-back does not match its stopped snapshot");
      model.message="";
      // Late stop/invalidated events retire values, not this already observed
      // transaction result. Keep its original target and acknowledgement.
      model.writeResult={status:"verified",text:"Wrote "+bytes.length+" byte(s) at offset "+address+"; read-back verified.",stopId:reply.stopId,module:selected.module,memory:selected.memory};
      model.hex="";
    } catch(error) {
      retire(model);model.writeError=(committed?"Commit acknowledged; do not retry automatically. ":"")+String(error.message||error);
      if(!committed)throw error;
    } finally {model.busy=false;render(model);}
    await refresh(model);
    return reply;
  }
  async function message(model,value) {
    if(!value || typeof value!=="object" || Array.isArray(value) || typeof value.action!=="string")return;
    try {
      if(value.action==="ready"){model.ready=true;void model.panel.webview.postMessage({action:"editorStyle",...editorStyle.read(vscode)});if(model.html)void model.panel.webview.postMessage({action:"render",html:model.html});return;}
      if(!Number.isSafeInteger(value.epoch) || value.epoch!==model.epoch) {
        if(value.action==="memory-scroll")void model.panel.webview.postMessage({action:"memory-scroll-done",request:value.request});
        throw new Error("This window has changed. Use the current view and retry.");
      }
      if(value.action==="memory-scroll" && model.kind==="memory") {
        if(!Number.isSafeInteger(value.rows) || !Number.isSafeInteger(value.request))throw new Error("Exact row displacement required");
        try {
          if(model.busy || model.retired || !model.data || !["full","read-only"].includes(model.permission))
            throw new Error("Refresh a current memory snapshot before scrolling its address range");
          const memory=BigInt(model.data.memoryBytes),columns=BigInt(model.columns||16);
          const last=memory?((memory-1n)/columns)*columns:0n;
          let offset=BigInt(model.selector.offset)+BigInt(value.rows)*columns;
          offset=offset<0n?0n:offset>last?last:offset;
          if(offset!==BigInt(model.selector.offset))await refresh(model,{...model.selector,offset:offset.toString()});
        } finally {
          if(!model.disposed)void model.panel.webview.postMessage({action:"memory-scroll-done",request:value.request});
        }
        return;
      }
      if(value.action==="load-more" && model.kind==="memory"){await loadMore(model);return;}
      if(value.action==="new-window") {
        await open(model.kind,model.kind==="memory"?{...model.selector,showTags:model.showTags,columns:model.columns}:
          model.kind==="wasip1"?{module:model.module}:model.kind==="ros"?{backend:model.backend}:undefined,
          sessions.get(model.sessionId)||null);return;
      }
      if(value.action==="columns" && model.kind==="memory") {
        model.columns=modelAPI.columns(value.value);render(model);return;
      }
      if(value.action==="toggle-tags" && model.kind==="memory") {
        if(model.busy)return;
        if(typeof value.enabled!=="boolean")throw new Error("Invalid Memtag switch value");
        if(!value.enabled) {model.showTags=false;model.notice="";render(model);return;}
        if(!["full","read-only"].includes(model.permission) || !model.data || !model.stopId || model.retired)
          throw new Error("Memtag requires a current full or read-only memory snapshot");
        await refresh(model,undefined,true);return;
      }
      if(value.action==="bind") {bind(model,active());await refresh(model);return;}
      if(model.kind==="breakpoints" && ["breakpoint-create","breakpoint-condition","breakpoint-ignore","breakpoint-set-callbacks","breakpoint-callbacks","breakpoint-callback-next"].includes(value.action)) {
        if(model.retired || model.busy || !model.stopId)throw new Error("Refresh the current stopped breakpoint view first");
        const session=sessionFor(model),epoch=model.epoch,reading=["breakpoint-callbacks","breakpoint-callback-next"].includes(value.action);
        if(!reading && model.permission!=="full")throw new Error("Changing breakpoints requires full permission");
        if(!["full","read-only"].includes(model.permission))throw new Error("Inspecting breakpoints requires full or read-only permission");
        const id=Number(value.id);
        if(value.action!=="breakpoint-create" && (!Number.isSafeInteger(id) || !model.data?.rows?.some(row=>row.id===id)))throw new Error("Select a breakpoint in the current inventory");
        model.busy=true;render(model);
        try {
          if(reading) {
            const start=value.action==="breakpoint-callback-next"?model.callbackPages?.[id]?.next:0;
            if(!Number.isSafeInteger(start))throw new Error("Read the first callback page before continuing");
            const result=await session.customRequest("uwvm/breakpointCallbacks",{id,start});current(model,session,epoch);
            model.callbackPages??={};model.callbackPages[id]=result;
          } else if(value.action==="breakpoint-create") {
            const fields=value.fields||{};
            await session.customRequest("uwvm/breakpointCreate",{module:fields.module,function:fields.function,offset:fields.offset,temporary:fields.temporary==="on"});
          } else {
            const action={"breakpoint-condition":"condition","breakpoint-ignore":"ignore","breakpoint-set-callbacks":"callbacks"}[value.action];
            await session.customRequest("uwvm/breakpointControl",{id,action,value:value.fields?.value||"",mode:value.fields?.mode||"replace"});
          }
          model.message=reading?"Callback page read.":"Breakpoint change acknowledged by the runtime.";
        } finally {model.busy=false;render(model);}
        if(!reading)await refresh(model);return;
      }
      if(model.kind==="breakpoints" && ["breakpoint-toggle","breakpoint-delete"].includes(value.action)) {
        if(model.permission!=="full" || model.retired || model.busy)throw new Error("A current full-permission stop is required to change breakpoints");
        const id=Number(value.id);
        if(!Number.isSafeInteger(id) || !model.data?.rows?.some(row=>row.id===id))throw new Error("The breakpoint is no longer in this view");
        const action=value.action==="breakpoint-delete"?"delete":value.nextState;
        if(!["enable","disable","delete"].includes(action))throw new Error("Unknown breakpoint action");
        await sessionFor(model).customRequest("uwvm/breakpointControl",{id,action});await refresh(model);return;
      }
      if(value.action==="diagnostics") {await vscode.commands.executeCommand("uwvm.showDiagnostics");return;}
      if(model.kind==="diagnostics" && value.action==="browse-dump") {
        const session=sessionFor(model);filePicker.requireLocal(await filePicker.localSession(session));
        model.picking=true;render(model);
        try {const path=await filePicker.pick(vscode,{title:"Wasm dump on VM host",value:value.fields?.dumpPath||model.dumpPath,save:true});
          if(path!==undefined && !model.disposed)model.dumpPath=path;
        } finally {model.picking=false;render(model);}return;
      }
      if(model.kind==="diagnostics" && value.action==="dump") {
        const session=sessionFor(model),epoch=model.epoch,stop=model.stopId;
        if(!stop || model.retired || model.busy || !["full","read-only","limited"].includes(model.permission))
          throw new Error("Refresh a current permitted stop before saving a dump");
        if(typeof value.fields?.dumpPath!=="string" || !value.fields.dumpPath)throw new Error("Choose an output file on the VM host");
        model.dumpPath=value.fields.dumpPath;model.busy=true;model.error="";render(model);
        try {const reply=await session.customRequest("uwvm/dump",{path:model.dumpPath,stopId:stop});current(model,session,epoch);model.dumpReply=reply;}
        finally {model.busy=false;render(model);}return;
      }
      if(value.action==="refresh") {model.message="";model.writeError="";await refresh(model,value.fields);return;}
      if(value.action==="write" && (model.kind==="memory")) {await write(model,value.fields||{});return;}
      if(model.busy)return;
      if(["browse-export","payload","export","trace-on","trace-off","trace-clear"].includes(value.action)) {
        if(!["wasip1","ros"].includes(model.kind) || !["full","read-only"].includes(model.permission) || !model.data || !model.stopId || model.retired)
          throw new Error("Refresh a current full or read-only stopped group before accessing recorded calls");
        const family=model.kind==="wasip1"?"wasip1":model.backend;
        if(!["wasip1","ros1","ros2"].includes(family))throw new Error("Choose one explicit ROS backend for host-call controls");
        const session=sessionFor(model),expected=model.stopId,epoch=model.epoch;
        const diagnostic=await session.customRequest("uwvm/diagnostics",{});current(model,session,epoch);
        if(model.stopId!==expected || model.retired || stopFrom(diagnostic)!==expected)throw new Error("The displayed stop retired; refresh before accessing retained recordings");
        model.error="";
        if(value.action==="browse-export") {
          if(model.picking)return;
          model.picking=true;
          try {
            filePicker.requireLocal(await filePicker.localSession(session));current(model,session,epoch);
            const target=await filePicker.pick(vscode,{title:"Recorded calls JSON on VM host",value:value.fields?.exportPath||model.exportPath,save:true,filters:{JSON:["json"]}});
            current(model,session,epoch);
            if(target!==undefined)model.exportPath=target;
          } finally {model.picking=false;render(model);}
          return;
        }
        if(value.action==="call-payload") {
          if(typeof value.call!=="string" || !/^[1-9][0-9]{0,19}$/.test(value.call))throw new Error("Invalid recorded call ID");
          model.payloadSelector={call:value.call,segment:"0",offset:"0",count:256};render(model);return;
        }
        if(value.action==="payload") {
          const f=value.fields||{},selector={call:modelAPI.decimal(f.call,"Call ID"),segment:modelAPI.decimal(f.segment,"Segment"),offset:modelAPI.decimal(f.offset,"Payload offset"),count:modelAPI.page(f.count,4096)};
          if(selector.call==="0")throw new Error("Choose a nonzero retained call ID");
          model.payloadSelector=selector;
          const copied=await session.customRequest("uwvm/hostTraceBytes",selector);
          current(model,session,epoch);
          if(copied.stopId!==expected || stopFrom(await session.customRequest("uwvm/diagnostics",{}))!==expected)throw new Error("Retained payload belongs to a different stop; refresh");
          model.payload=copied;render(model);return;
        }
        if(value.action==="export") {
          const target=value.fields && value.fields.exportPath;
          if(typeof target!=="string" || !target.startsWith("/") || /[\x00-\x1f]/.test(target))throw new Error("Enter an absolute new JSON file path on the VM host");
          model.exportPath=target;
          const copied=await session.customRequest("uwvm/exportHostTrace",{family,path:target});
          current(model,session,epoch);model.exportResult=copied;render(model);return;
        }
        if(model.permission!=="full")throw new Error("Changing recording requires full debugger permission");
        const permission=await session.customRequest("uwvm/permissions",{});
        current(model,session,epoch);
        if(permission.permissions!=="full")throw new Error("The runtime does not authorize recording changes");
        const verb=value.action.slice(6),expression="trace host "+verb+(verb==="clear"?"":" family="+family)+" if-stop="+expected;
        await session.customRequest("evaluate",{context:"repl",expression});
        model.message="Recording: "+verb+".";await refresh(model);return;
      }
      if(value.action==="previous" || value.action==="next") {
        const step=BigInt(model.selector.count)*(value.action==="previous"?-1n:1n),offset=BigInt(model.selector.offset)+step;
        await refresh(model,{offset:(offset<0n?0n:offset).toString()});return;
      }
      if(value.action==="wasi-group" && Object.hasOwn(modelAPI.WASI_GROUPS,value.group)) {model.wasiGroup=value.group;model.page=0;model.groupHistory=[];model.payloadSelector=undefined;await refresh(model);return;}
      if(value.action==="ros-group" && Object.hasOwn(modelAPI.ROS_GROUPS,value.group)) {model.rosGroup=value.group;model.page=0;model.groupHistory=[];model.payloadSelector=undefined;await refresh(model);return;}
      if(value.action==="policy-next" && model.data && model.data.policy && model.data.policy.more && Number.isSafeInteger(model.data.policy.next)) {
        if(model.data.policy.next<=model.policyPage)throw new Error("The backend policy page did not advance");
        (model.policyHistory||=[]).push(model.policyPage);
        model.policyPage=model.data.policy.next;await refresh(model);return;
      }
      if(value.action==="policy-previous" && model.policyHistory && model.policyHistory.length) {model.policyPage=model.policyHistory.pop();await refresh(model);return;}
      if(value.action==="group-previous" && model.groupHistory?.length) {model.page=model.groupHistory.pop();await refresh(model);return;}
      if(value.action==="group-next" && model.data) {
        const data=model.kind==="wasip1"?model.data.environment:model.data.internal;
        if(data && data.more && (Number.isSafeInteger(data.next) && data.next>=0 || typeof data.next==="string" && /^(0|[1-9][0-9]{0,19})$/.test(data.next))) {(model.groupHistory||=[]).push(model.page);model.page=data.next;await refresh(model);}
      }
    } catch(error) {model.error=String(error.message||error);render(model);}
  }
  async function open(kind,initial,session=active()) {
    if(!Object.hasOwn(modelAPI.KINDS,kind))throw new Error("Unknown UWVM inspection window");
    const selector=kind==="memory"?modelAPI.memorySelection(initial||{}):modelAPI.memorySelection();
    const columns=modelAPI.columns(initial && typeof initial==="object"?initial.columns:undefined);
    const id=++nextPanelId;
    // Separate webview origins also avoid stale service-worker reuse after a same-kind window closes.
    const panel=vscode.window.createWebviewPanel("uwvm."+kind+"."+id,"UWVM · "+modelAPI.KINDS[kind]+" "+id,vscode.ViewColumn.Active,
      {enableScripts:true,retainContextWhenHidden:true,localResourceRoots:[vscode.Uri.file(context.extensionPath)]});
    const model={id,kind,panel,selector,columns,module:"0",backend:"all",wasiGroup:"args",rosGroup:"sessions",page:0,policyPage:0,epoch:0,busy:false,showTags:undefined};
    panels.set(id,model);bind(model,session);
    if(initial && typeof initial==='object') {
      if(Object.hasOwn(modelAPI.WASI_GROUPS,initial.wasiGroup))model.wasiGroup=initial.wasiGroup;
      if(Object.hasOwn(modelAPI.ROS_GROUPS,initial.rosGroup))model.rosGroup=initial.rosGroup;
    }
    dispose(panel);dispose(panel.webview.onDidReceiveMessage(value=>message(model,value)));
    dispose(panel.onDidDispose(()=>{model.disposed=true;model.epoch++;panels.delete(id);}));
    await refresh(model,kind==="ros"||kind==="wasip1"?initial:undefined,kind==="memory" && initial && typeof initial.showTags==="boolean"?initial.showTags:undefined);
    return panel;
  }
  for(const [command,kind] of Object.entries({"uwvm.openMemory":"memory","uwvm.openRegisters":"registers","uwvm.openWasip1":"wasip1","uwvm.openRos":"ros"}))
    dispose(vscode.commands.registerCommand(command,initial=>open(kind,initial)));
  dispose(vscode.commands.registerCommand("uwvm.writeMemory",async fields=>{
    const model=Array.from(panels.values()).find(item=>item.kind==="memory" && item.panel.active && !item.disposed);
    if(!model)throw new Error("Focus the memory window to edit first");
    if(fields===undefined) {
      const epoch=model.epoch,sessionId=model.sessionId;
      const value=await vscode.window.showInputBox({prompt:"Hex bytes to write at the selected offset "+(model.writeOffset||model.selector.offset),placeHolder:"41 42 43 00"});
      if(value===undefined)return;
      if(model.disposed || model.epoch!==epoch || model.sessionId!==sessionId)throw new Error("The memory window changed while entering bytes. Refresh before editing.");
      fields={hex:value,writeOffset:model.writeOffset||model.selector.offset};
    }
    return write(model,fields);
  }));
  if(vscode.debug.registerDebugAdapterTrackerFactory)dispose(vscode.debug.registerDebugAdapterTrackerFactory("uwvm-llvm-full",{
    createDebugAdapterTracker(session) {return {onWillReceiveMessage(packet) {
      if(packet.type!=="request")return;
      const changes=["continue","next","stepIn","stepOut","pause","restart","restartFrame","goto","writeMemory","setVariable","setExpression","uwvm/signedUpdate"];
      if(changes.includes(packet.command) || packet.command==="evaluate" && packet.arguments && packet.arguments.context==="repl")
        {stopState(session).failed=false;for(const model of panels.values())if(model.sessionId===session.id)retire(model);}
    },onDidSendMessage(packet) {
      if(packet.type!=="event")return;
      for(const model of panels.values())if(model.sessionId===session.id) {
        if(["continued","invalidated","terminated","exited","memory"].includes(packet.event))retire(model);
      }
      // Compare authenticated stop identifiers, never reason/thread labels.
      // Repeated stopped notifications must not cause a refresh feedback loop.
      if(packet.event==="stopped")void checkStop(session);
      if(packet.event==="uwvm/breakpoints")for(const model of panels.values())if(model.sessionId===session.id && model.kind==="breakpoints") {
        if(model.busy)model.refreshOnIdle=true;else void refresh(model);
      }
    }};}
  }));
  if(vscode.workspace.onDidChangeConfiguration)dispose(vscode.workspace.onDidChangeConfiguration(event=>{
    if(event.affectsConfiguration("editor"))for(const model of panels.values())if(model.ready && !model.disposed)
      void model.panel.webview.postMessage({action:"editorStyle",...editorStyle.read(vscode)});
  }));
  dispose(vscode.debug.onDidTerminateDebugSession(session=>{
    stopStates.delete(session.id);
    for(const model of panels.values())if(model.sessionId===session.id) {retire(model);model.data=undefined;model.sessionId=undefined;model.sessionName="Session ended";render(model);}
  }));
  if(vscode.debug.onDidStartDebugSession)dispose(vscode.debug.onDidStartDebugSession(session=>{
    if(session.type!=="uwvm-llvm-full" || sessions.get(session.id)!==session)return;disabledPermission=undefined;
    for(const model of panels.values())if(!model.sessionId && !model.sessionEverBound) {bind(model,session);setTimeout(()=>refresh(model),50);}
  }));
  return {open,noteDisabled(permission) {disabledPermission=permission;for(const model of panels.values())if(!model.sessionId) {model.permission=permission;render(model);}},
    dispose() {for(const model of panels.values())model.panel.dispose();panels.clear();}};
}
module.exports={createController};
