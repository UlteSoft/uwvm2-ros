"use strict";

// A session-bound disassembly editor. Display data never becomes a native
// pointer or an execution capability. Every live operation is rebound by DAP.
const path=require("path"),fs=require("fs"),crypto=require("crypto");
const language=require("./wasm_language"),models=require("./wasm_view_model");
const editorStyle=require("./editor_style");
const VIEW="uwvm.wasmInstructions";
function register(vscode,context,sessions) {
  if(!vscode.window?.registerCustomEditorProvider)return;
  const panels=new Set(),selectedThreads=new Map(),stacks=new Map();
  const add=x=>context.subscriptions.push(x);
  const send=(m,message)=>{if(m.ready&&!m.disposed)void m.panel.webview.postMessage({...message,revision:m.revision});};
  const live=(m,stamp)=>!m.disposed && m.revision===stamp && sessions.get(m.id.session)===m.session;
  function retire(m,reason) {
    ++m.revision;m.current=undefined;m.error=reason||"Stopped snapshot retired; waiting for the current frame.";
    state(m);
  }
  function fail(m,error) {m.error=String(error?.message||error);send(m,{action:"notice",message:m.error,error:true});}
  function state(m) {
    send(m,{action:"state",valid:!!m.current,message:m.error,permission:m.permission,sessionName:m.session.name,
      module:m.id.module,function:m.id.function,name:m.functionName,epoch:m.id.epoch,generation:m.id.generation,...m.current});
    points(m);
  }
  function points(m) {
    if(!m.ready||!m.layout)return;
    const rows=[];
    const same=point=>point instanceof vscode.SourceBreakpoint && point.location.uri.toString()===m.document.uri.toString();
    const local=(vscode.debug.breakpoints||[]).filter(same);
    const inventory=m.inventory,known=[];
    if(inventory?.available!==false && Array.isArray(inventory?.rows))for(const row of inventory.rows) {
      const at=inventory.locations?.[String(row.id)];
      if(!at?.source?.path || !Number.isSafeInteger(at.line))continue;
      let uri;try{uri=vscode.Uri.parse(at.source.path);}catch(_){continue;}
      if(uri.toString()===m.document.uri.toString())known.push({...row,line:at.line});
    }
    const lines=new Set([...known.map(p=>p.line),...local.map(p=>p.location.range.start.line+1)]);
    for(const line of lines) {
      const row=m.layout.rows[line-2];if(!row)continue;
      const actual=known.filter(p=>p.line===line),desired=local.filter(p=>p.location.range.start.line+1===line);
      if(!actual.length&&!desired.length)continue;
      const pending=desired.length!==actual.length || desired.some(p=>!actual.some(a=>a.enabled===p.enabled && (a.condition||"")===(p.condition||"")));
      rows.push({line:row.line,enabled:actual.some(p=>p.enabled),pending,
        ids:actual.map(p=>p.id),condition:actual.map(p=>p.condition).filter(Boolean).join("; "),
        hits:actual.map(p=>p.hits),exists:true});
    }
    send(m,{action:"breakpoints",rows});
  }
  async function refresh(m,threadId) {
    if(!m.ready || m.disposed)return;
    // Coalesce repeated stack responses. An unchanged stop must not flash
    // through an unavailable state or overwrite the user's viewport.
    if(m.refreshing){m.refreshAgain=threadId;return;}
    m.refreshing=true;
    const stamp=m.revision;
    if(!m.current){m.error="Reading the current stopped instruction…";state(m);}
    try {
      const permission=await m.session.customRequest("uwvm/permissions",{});
      if(!live(m,stamp))return;
      m.permission=permission?.permissions;
      if(!["full","read-only"].includes(m.permission))throw new Error("Instruction inspection requires full or read-only permission. Limited mode retains basic crash diagnostics.");
      const reply=await m.session.customRequest("uwvm/wasmDocument",{reference:m.id.reference,includeLayout:!m.layout,
        ...(threadId===undefined?{}:{threadId})});
      if(!live(m,stamp))return;
      if(!reply || reply.reference!==m.id.reference || reply.module!==m.id.module || reply.function!==m.id.function ||
         reply.epoch!==m.id.epoch || reply.generation!==m.id.generation || !Number.isSafeInteger(reply.threadId) || reply.threadId<1 ||
         !/^[1-9][0-9]{0,19}$/.test(reply.stopId) || BigInt(reply.stopId)>=1n<<64n ||
         reply.digest!==crypto.createHash("sha256").update(m.document.getText(),"utf8").digest("hex"))
        throw new Error("The instruction document differs from the current owned generation.");
      if(!m.layout) {
        const data=models.layout(m.document),safe=new Set(reply.safeLines);
        if(!Array.isArray(reply.safeLines) || safe.size!==reply.safeLines.length || reply.safeLines.some(line=>!Number.isSafeInteger(line)||line<2||line>data.rows.length+1))
          throw new Error("Executable instruction boundaries are unavailable.");
        for(const row of data.rows)row.safe=safe.has(row.line);
        m.layout=data;
        send(m,{action:"load",...data,addressBits:reply.addressBits});
      }
      const row=m.layout.rows[reply.line-2];
      if(!row || !row.safe || row.offset!==reply.offset)throw new Error("The current instruction is not an executable boundary.");
      m.current={stopId:reply.stopId,threadId:reply.threadId,line:reply.line,offset:reply.offset};
      m.error="";state(m);
      // Optional metadata cannot invalidate a correctly authenticated PC.
      try {
      const inventory=await m.session.customRequest("uwvm/breakpoints",{});
      if(!live(m,stamp))return;
      m.inventory=inventory;points(m);
      const name=await m.session.customRequest("uwvm/wasmHover",{reference:m.id.reference,ifStop:reply.stopId,threadId:reply.threadId,kind:"function",index:m.id.function});
      if(live(m,stamp)&&name?.stopId===reply.stopId && m.functionName!==(name.name||"")){m.functionName=name.name||"";state(m);}
      } catch(error) {if(live(m,stamp))fail(m,error);}
    } catch(error) {if(live(m,stamp)){m.current=undefined;m.error=String(error.message||error);state(m);}}
    finally {
      m.refreshing=false;
      if(m.refreshAgain!==undefined){const next=m.refreshAgain;m.refreshAgain=undefined;void refresh(m,next);}
    }
  }
  async function fresh(m,message,write=false) {
    const stamp=m.revision,current=m.current;
    if(message.revision!==stamp || !current || !live(m,stamp))throw new Error("This action belongs to a retired stopped snapshot.");
    if(write&&m.permission!=="full")throw new Error("Changing execution or breakpoints requires full permission.");
    const reply=await m.session.customRequest("uwvm/wasmDocument",{reference:m.id.reference,ifStop:current.stopId,threadId:current.threadId});
    if(!live(m,stamp)||m.current!==current||reply.stopId!==current.stopId||reply.threadId!==current.threadId||reply.digest!==crypto.createHash("sha256").update(m.document.getText(),"utf8").digest("hex"))
      throw new Error("The stopped snapshot changed before the action.");
    return {stamp,current};
  }
  async function hover(m,message) {
    const {stamp,current}=await fresh(m,message);
    const row=m.layout.rows[message.line-2];
    if(!row)throw new Error("Instruction is outside this function.");
    let result;
    if(message.part==="address")result={title:"Wasm bytecode location",description:"Module "+m.id.module+", func "+m.id.function+"\nByte offset "+row.offset+" from this function's expression start.\nGeneration "+m.id.generation+", runtime epoch "+m.id.epoch+".\n"+(row.safe?"Executable debugger stop point.":"Display-only opcode; this optimized generation has no executable stop point here.")};
    else if(message.part==="instruction")result={title:row.name,description:row.brief+(row.safe?"":"\nThis opcode has no executable stop point in the current optimized generation.")};
    else if(Number.isSafeInteger(message.token)) {
      const token=row.tokens[message.token];if(!token)return;
      result=models.branchTarget(m.layout,row,token)||models.literal(token,row);
      const meaning=token.meaning;
      if(!result && meaning?.kind==="object" && ["function","local","global","memory","table"].includes(meaning.key)) {
        const value=await m.session.customRequest("uwvm/wasmHover",{reference:m.id.reference,ifStop:current.stopId,threadId:current.threadId,kind:meaning.key,index:meaning.value});
        if(!live(m,stamp)||value?.stopId!==current.stopId||value.kind!==meaning.key||value.index!==meaning.value)return;
        result={title:(meaning.key==="function"?"func":meaning.key)+" "+meaning.value+(value.name?" · "+value.name:""),
          type:value.type,value:value.value,description:[value.imported?"Imported by module "+m.id.module:"Defined in module "+m.id.module,
            value.addressBits?value.addressBits+"-bit linear address space":"",value.unavailable?"Current value unavailable: "+value.unavailable:""].filter(Boolean).join("\n")};
      }
      if(!result && meaning?.kind==="operand" && ["offset","align","lane"].includes(meaning.key))
        result={title:meaning.key,description:meaning.key==="offset"?"Unsigned byte displacement added to the stack address before the checked memory access.":meaning.key==="align"?"Declared alignment: "+meaning.value+" bytes. This does not waive memory bounds or tag checks.":"Zero-based lane selected within the SIMD value."};
    }
    if(live(m,stamp)&&result)send(m,{action:"hover",request:message.request,...result});
  }
  async function breakpoint(m,message) {
    await fresh(m,message,true);
    const row=m.layout.rows[message.line-2];if(!row?.safe)throw new Error("This opcode has no executable stop point in the current generation.");
    const existing=vscode.debug.breakpoints.filter(p=>p instanceof vscode.SourceBreakpoint && p.location.uri.toString()===m.document.uri.toString()&&p.location.range.start.line===message.line-1);
    if(existing.length>1)throw new Error("Several breakpoints share this opcode. Select their individual IDs in the Breakpoints window.");
    let operation=message.operation,point=existing[0];
    if(operation==="menu") {
      const choices=point?[{label:point.enabled?"Disable breakpoint":"Enable breakpoint",op:"enabled"},{label:"Edit condition…",op:"condition"},{label:"Edit hit condition…",op:"hit"},{label:"Remove breakpoint",op:"remove"}]:[{label:"Add breakpoint",op:"add"},{label:"Add conditional breakpoint…",op:"condition"}];
      const choice=await vscode.window.showQuickPick(choices,{placeHolder:"Breakpoint at "+row.address});if(!choice)return;operation=choice.op;
    }
    if(operation==="toggle")operation=point?"remove":"add";
    let condition=point?.condition,hit=point?.hitCondition;
    if(operation==="condition"||operation==="hit") {
      const value=await vscode.window.showInputBox({title:operation==="condition"?"Wasm breakpoint condition":"Wasm hit condition",value:(operation==="condition"?condition:hit)||"",prompt:"Evaluated by the authorized debugger. Empty removes this condition."});
      if(value===undefined)return;if(operation==="condition")condition=value||undefined;else hit=value||undefined;
    }
    await fresh(m,message,true); // A dialog cannot retain a mutation capability.
    if(!["remove","add","enabled","condition","hit"].includes(operation))throw new Error("Unknown breakpoint action.");
    if(point)vscode.debug.removeBreakpoints([point]);
    if(operation!=="remove")vscode.debug.addBreakpoints([new vscode.SourceBreakpoint(new vscode.Location(m.document.uri,new vscode.Position(message.line-1,0)),operation==="enabled"?(point?!point.enabled:false):(point?.enabled??true),condition,hit,point?.logMessage)]);
    send(m,{action:"notice",message:"Breakpoint request submitted; waiting for runtime confirmation."});points(m);
  }
  async function request(m,message) {
    if(!message||typeof message.action!=="string"||m.disposed)return;
    const mutation=message.action==="breakpoint"||["continue","next","stepIn","stepOut"].includes(message.action);
    if(mutation&&m.mutating){fail(m,new Error("An execution or breakpoint action is still pending."));return;}
    if(mutation)m.mutating=true;
    try {
      if(message.action==="ready") {m.ready=true;send(m,{action:"editorStyle",...editorStyle.read(vscode,m.document.uri)});await refresh(m,selectedThreads.get(m.id.session));return;}
      if(message.action==="refresh") {
        const thread=m.current?.threadId||selectedThreads.get(m.id.session);
        if(thread!==undefined)await m.session.customRequest("stackTrace",{threadId:thread,startFrame:0,levels:1});
        await refresh(m,thread);return;
      }
      if(message.action==="hover"){await hover(m,message);return;}
      if(message.action==="breakpoint"){await breakpoint(m,message);return;}
      if(["continue","next","stepIn","stepOut"].includes(message.action)) {
        const {current}=await fresh(m,message,true);
        // Invoke the same workbench action as the native toolbar. Do not
        // silently force Wasm mode when the indicator says Source/Assembly.
        if(vscode.debug.activeDebugSession!==m.session)throw new Error("Select this view's debug session before executing.");
        await vscode.commands.executeCommand({continue:"workbench.action.debug.continue",next:"workbench.action.debug.stepOver",stepIn:"workbench.action.debug.stepInto",stepOut:"workbench.action.debug.stepOut"}[message.action]);return;
      }
      if(message.action==="copy") {
        if(!m.layout||!Number.isSafeInteger(message.first)||!Number.isSafeInteger(message.last))return;
        const first=Math.min(message.first,message.last),last=Math.max(message.first,message.last);
        if(first<0||last>=m.layout.rows.length)return;
        const text=m.layout.rows.slice(first,last+1).map(row=>row.address+"  "+"    ".repeat(row.depth)+row.name+(row.operands?" "+row.operands:"")).join("\n");
        await vscode.env.clipboard.writeText(text);return;
      }
    } catch(error){fail(m,error);}finally{if(mutation)m.mutating=false;}
  }
  const provider={async resolveCustomTextEditor(document,panel) {
    const id=language.identity(document.uri),session=id&&sessions.get(id.session);
    if(!id||!session||session.type!=="uwvm-llvm-full")throw new Error("Open an authenticated UWVM Wasm stack frame.");
    const m={id,document,panel,session,revision:0,ready:false,disposed:false};panels.add(m);
    panel.webview.options={enableScripts:true,localResourceRoots:[vscode.Uri.file(context.extensionPath)]};
    const nonce=crypto.randomBytes(18).toString("hex"),asset=name=>panel.webview.asWebviewUri(vscode.Uri.file(path.join(context.extensionPath,name))).toString();
    panel.webview.html=fs.readFileSync(path.join(context.extensionPath,"wasm_view.html"),"utf8")
      .replaceAll("__NONCE__",nonce).replaceAll("__CSP__",panel.webview.cspSource)
      .replaceAll("__CSS__",asset("wasm_view.css")).replaceAll("__SCRIPT__",asset("wasm_view_webview.js"));
    const received=panel.webview.onDidReceiveMessage(message=>void request(m,message));
    panel.onDidDispose(()=>{
      m.disposed=true;++m.revision;panels.delete(m);received.dispose();
      if(![...panels].some(p=>p.id.session===id.session) && sessions.get(id.session)===session)
        void session.customRequest("uwvm/wasmView",{visible:false}).catch(()=>{});
    });
  }};
  add(vscode.window.registerCustomEditorProvider(VIEW,provider,{webviewOptions:{retainContextWhenHidden:true},supportsMultipleEditorsPerDocument:true}));
  if(vscode.workspace.onDidChangeConfiguration)add(vscode.workspace.onDidChangeConfiguration(event=>{
    if(event.affectsConfiguration("editor"))for(const m of panels)send(m,{action:"editorStyle",...editorStyle.read(vscode,m.document.uri)});
  }));
  if(vscode.debug.onDidChangeBreakpoints)add(vscode.debug.onDidChangeBreakpoints(()=>{for(const m of panels)points(m);}));
  add(vscode.debug.onDidTerminateDebugSession(s=>{selectedThreads.delete(s.id);stacks.delete(s.id);for(const m of panels)if(m.id.session===s.id)retire(m,"Debug session ended; retained code is historical only.");}));
  add(vscode.debug.registerDebugAdapterTrackerFactory("uwvm-llvm-full",{createDebugAdapterTracker(session) {
    const requests=new Map(),consoleRequests=new Set();return {
      onWillReceiveMessage(packet) {
        if(packet.type!=="request")return;
        if(packet.command==="stackTrace")requests.set(packet.seq,packet.arguments?.threadId);
        const consoleCommand=packet.command==="evaluate"&&(packet.arguments?.context??"repl")==="repl";
        if(consoleCommand)consoleRequests.add(packet.seq);
        if(["continue","next","stepIn","stepOut","pause","restart","restartFrame","goto","writeMemory","setVariable","setExpression","uwvm/signedUpdate"].includes(packet.command)||consoleCommand)
          for(const m of panels)if(m.id.session===session.id)retire(m);
      },onDidSendMessage(packet) {
        if(packet.type==="event") {
          if(["stopped","continued","invalidated","terminated","exited","memory"].includes(packet.event))for(const m of panels)if(m.id.session===session.id)retire(m);
          if(packet.event==="uwvm/breakpoints")for(const m of panels)if(m.id.session===session.id){m.inventory=packet.body;points(m);}
        }
        if(packet.type!=="response")return;
        if(packet.command==="evaluate"&&consoleRequests.delete(packet.request_seq)) {
          // Console commands may change a breakpoint without moving the stop.
          // Retire before dispatch, then obtain a fresh stack response even if
          // DAP emits no new stopped event. Never resurrect the old PC locally.
          const thread=selectedThreads.get(session.id);
          if(sessions.get(session.id)===session&&Number.isSafeInteger(thread))
            void session.customRequest("stackTrace",{threadId:thread,startFrame:0,levels:1}).catch(()=>{});
        }
        if(packet.command!=="stackTrace")return;
        const thread=requests.get(packet.request_seq);requests.delete(packet.request_seq);
        if(!packet.success||!Number.isSafeInteger(thread))return;
        const frame=packet.body?.stackFrames?.find(frame=>typeof frame.source?.path==="string"&&frame.source.path.startsWith("uwvm-wasm:"));
        if(!frame)return;selectedThreads.set(session.id,thread);stacks.set(session.id,frame.source.path);
        for(const m of panels)if(m.id.session===session.id && vscode.Uri.parse(frame.source.path).toString()===m.document.uri.toString())void refresh(m,thread);
      }
    };
  }}));
  async function open() {
    const session=vscode.debug.activeDebugSession;
    if(!session||sessions.get(session.id)!==session)throw new Error("Select a live UWVM debug session.");
    await session.customRequest("uwvm/wasmView",{visible:true});
    const result=await session.customRequest("threads",{}),thread=selectedThreads.get(session.id)||result.threads?.[0]?.id;
    if(!Number.isSafeInteger(thread))throw new Error("No current Wasm participant.");
    const frames=await session.customRequest("stackTrace",{threadId:thread,startFrame:0,levels:1});
    const source=frames.stackFrames?.find(frame=>frame.source?.path?.startsWith("uwvm-wasm:"))?.source;
    if(!source)throw new Error("Select the Wasm instruction level at a cooperative Wasm stop.");
    return vscode.commands.executeCommand("vscode.openWith",vscode.Uri.parse(source.path),VIEW,{preview:true});
  }
  add(vscode.commands.registerCommand("uwvm.openWasmInstructions",open));
  return {open,provider,panels};
}
module.exports={register,VIEW};
