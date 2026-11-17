"use strict";

// A session-bound disassembly editor. Display data never becomes a native
// pointer or an execution capability. Every live operation is rebound by DAP.
const path=require("path"),fs=require("fs"),crypto=require("crypto");
const language=require("./wasm_language"),models=require("./wasm_view_model");
const editorStyle=require("./editor_style");
const navigation=require("./navigation_state");
const VIEW="uwvm.wasmInstructions";
function register(vscode,context,sessions) {
  if(!vscode.window?.registerCustomEditorProvider)return;
  const history=require("./navigation_history").get(sessions);
  const panels=new Set(),selectedThreads=new Map(),stacks=new Map(),reveals=new Map(),restores=new Map(),followedStops=new Map();
  const add=x=>context.subscriptions.push(x);
  const send=(m,message)=>{if(m.ready&&!m.disposed)void m.panel.webview.postMessage({...message,revision:m.revision});};
  const live=(m,stamp)=>!m.disposed && m.revision===stamp && sessions.get(m.id.session)===m.session;
  function retire(m,reason) {
    ++m.revision;m.current=undefined;m.next=undefined;m.error=reason||"Stopped snapshot retired; waiting for the current frame.";
    state(m);
  }
  function fail(m,error) {m.error=String(error?.message||error);send(m,{action:"notice",message:m.error,error:true});}
  function state(m) {
    send(m,{action:"state",valid:!!m.current&&!m.current.native,message:m.error,permission:m.permission,sessionName:m.session.name,
      module:m.id.module,function:m.id.function,name:m.functionName,epoch:m.id.epoch,generation:m.id.generation,
      next:m.next,movable:m.movable,follow:!m.historyRestore,...m.current});
    points(m);
  }
  // Positions from the adapter. At a cooperative stop in this function the
  // Wasm pointer is EXACT (yellow, refreshed from the stop itself). At an
  // assembly stop the runtime identifies whether this is the start (yellow)
  // or inside (green). Native projections are display-only, never edit handles.
  function positions(session,pos) {
    for(const m of panels) {
      if(m.disposed||m.id.session!==session.id)continue;
      const w=pos&&pos.state==="stopped"?pos.wasm:undefined;
      const here=!!w&&w.module===m.id.module&&w.function===m.id.function;
      m.movable=pos&&pos.movable?{wasm:!!pos.movable.wasm,reason:(pos.moveReason||{}).wasm||""}:undefined;
      if(here){if(!pos.native)m.next=undefined;void refresh(m,pos.threadId);continue;}
      let next;
      if(here&&pos.native&&m.layout) {
        const row=m.layout.rows.find(r=>r.offset===String(w.offset));
        if(row&&w.provenance==="exact"&&String(w.epoch)===String(m.id.epoch)&&String(w.generation)===String(m.id.generation))next={line:row.line,exact:w.exact===true,provenance:w.provenance,stopId:pos.stopId,threadId:pos.threadId,nativePc:pos.nativePc};
      }
      if(pos&&pos.native){m.current=undefined;m.error=next?"":"The stop is in assembly code outside this function.";}
      m.next=next;state(m);
    }
    // Follow the stop into another function when a Wasm view of this session
    // is open but shows a different function. VS Code itself does this at the
    // Wasm level; this covers the source and assembly levels. A view the user
    // closed stays closed.
    if(!pos||pos.state!=="stopped"||!pos.wasm)return;
    const followKey=JSON.stringify([pos.stopId,pos.threadId,pos.wasm.module,pos.wasm.function]);
    const followed=followedStops.get(session.id);followedStops.set(session.id,followKey);
    if(followed===followKey)return;
    const open=[...panels].filter(m=>!m.disposed&&m.id.session===session.id);
    if(!open.length||open.some(m=>pos.wasm&&m.id.module===pos.wasm.module&&m.id.function===pos.wasm.function))return;
    const visible=open.find(m=>m.panel.visible);
    if(!visible)return;
    const isCurrent=navigation.current(session);
    void (async()=>{
      try {
        const reply=await session.customRequest("uwvm/currentWasmDocument",{});
        if(!isCurrent()||!visible.panel.visible||!reply?.source?.path||sessions.get(session.id)!==session)return;
        await vscode.commands.executeCommand("vscode.openWith",vscode.Uri.parse(reply.source.path),VIEW,{preview:true,preserveFocus:true,viewColumn:visible.panel.viewColumn});
      } catch(_) { /* following is a convenience; the stop itself is unaffected */ }
    })();
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
      if(!m.layout) {
        const layout=await m.session.customRequest("uwvm/wasmDocumentLayout",{reference:m.id.reference});
        if(!live(m,stamp))return;
        if(layout.reference!==m.id.reference||layout.module!==m.id.module||layout.function!==m.id.function||layout.epoch!==m.id.epoch||layout.generation!==m.id.generation||layout.digest!==crypto.createHash("sha256").update(m.document.getText(),"utf8").digest("hex"))throw new Error("Instruction document identity differs.");
        m.layout=models.layout(m.document);for(const row of m.layout.rows)row.safe=false;
        send(m,{action:"load",...m.layout,addressBits:layout.addressBits});
      }
      const reply=await m.session.customRequest("uwvm/wasmDocument",{reference:m.id.reference,includeLayout:true,
        ...(threadId===undefined?{}:{threadId})});
      if(!live(m,stamp))return;
      if(!reply || reply.reference!==m.id.reference || reply.module!==m.id.module || reply.function!==m.id.function ||
         reply.epoch!==m.id.epoch || reply.generation!==m.id.generation || !Number.isSafeInteger(reply.threadId) || reply.threadId<1 ||
         !/^[1-9][0-9]{0,19}$/.test(reply.stopId) || BigInt(reply.stopId)>=1n<<64n ||
         reply.digest!==crypto.createHash("sha256").update(m.document.getText(),"utf8").digest("hex"))
        throw new Error("The instruction document differs from the current owned generation.");
      {
        const data=m.layout,safe=new Set(reply.safeLines);
        if(!Array.isArray(reply.safeLines) || safe.size!==reply.safeLines.length || reply.safeLines.some(line=>!Number.isSafeInteger(line)||line<2||line>data.rows.length+1))
          throw new Error("Executable instruction boundaries are unavailable.");
        for(const row of data.rows)row.safe=safe.has(row.line);
        m.layout=data;
        send(m,{action:"safe-lines",lines:reply.safeLines});
      }
      const row=m.layout.rows[reply.line-2];
      if(!row || !row.safe || row.offset!==reply.offset)throw new Error("The current instruction is not an executable boundary.");
      m.current={stopId:reply.stopId,threadId:reply.threadId,line:reply.line,offset:reply.offset,native:reply.native===true};
      m.next=reply.native?{line:reply.line,exact:reply.exact===true,provenance:"exact",stopId:reply.stopId,threadId:reply.threadId}:undefined;
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
      const requested=reveals.get(m.document.uri.toString());
      if(requested&&m.layout&&m.ready){reveals.delete(m.document.uri.toString());send(m,{action:"reveal",line:requested});}
      if(m.historyRestore&&m.layout&&m.ready){send(m,{action:"restore-viewport",...m.historyRestore});m.viewport=m.historyRestore;m.historyRestore=undefined;}
      if(m.viewport===undefined&&m.layout){m.viewport={line:m.current?.line||2,top:0,left:0};if(m.panel.active)m.navigation?.visit(m.viewport);}
      if(m.refreshAgain!==undefined){const next=m.refreshAgain;m.refreshAgain=undefined;void refresh(m,next);}
    }
  }
  async function fresh(m,message,write=false) {
    const stamp=m.revision,current=m.current;
    if(message.revision!==stamp || !current || !live(m,stamp))throw new Error("This action belongs to a retired stopped snapshot.");
    if(write&&m.permission!=="full")throw new Error("Changing execution or breakpoints requires full permission.");
    if(write&&current.native)throw new Error("This is a code projection at a native instruction stop; select a cooperative Wasm stop to edit Wasm state.");
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
    const mutation=message.action==="breakpoint"||message.action==="jump"||["continue","next","stepIn","stepOut"].includes(message.action);
    if(mutation&&m.mutating){fail(m,new Error("An execution or breakpoint action is still pending."));return;}
    if(mutation)m.mutating=true;
    try {
      if(["navigate-back","navigate-forward"].includes(message.action)){await history?.move(message.action==="navigate-back"?-1:1,m.session);return;}
      if(message.action==="viewport") {
        const p=message.position;
        if(m.layout&&p&&Number.isSafeInteger(p.line)&&p.line>=2&&p.line<=m.layout.rows.length+1&&Number.isFinite(p.top)&&p.top>=0&&p.top<=m.layout.rows.length&&Number.isFinite(p.left)&&p.left>=0&&p.left<1e8) {
          m.viewport={line:p.line,top:p.top,left:p.left};
          if(m.panel.active)m.navigation?.visit(m.viewport,true);
        }return;
      }
      if(message.action==="navigate-line") {
        const line=message.line;
        if(!m.layout||!Number.isSafeInteger(line)||line<2||line>m.layout.rows.length+1)throw new Error("Destination is outside this Wasm function.");
        if(m.viewport)m.navigation?.visit(m.viewport,true);
        const place={line,top:Math.max(0,line-2),left:0};m.navigation?.visit(place);
        m.viewport=place;send(m,{action:"reveal",line});return;
      }
      if(message.action==="navigate-call") {
        if(!["full","read-only"].includes(m.permission))throw new Error("Instruction inspection requires full or read-only permission.");
        const row=m.layout.rows[message.line-2],token=row?.tokens[message.token];
        if(!token||token.meaning?.kind!=="object"||token.meaning.key!=="function")throw new Error("Select a function operand.");
        const isCurrent=navigation.begin(m.session);
        const target=await m.session.customRequest("uwvm/wasmFunctionDocument",{reference:m.id.reference,index:token.meaning.value});
        if(!isCurrent()||m.disposed)return;
        if(m.viewport)m.navigation?.visit(m.viewport,true);
        await open({...target,sessionId:m.session.id,isCurrent,preview:false});return;
      }
      if(message.action==="ready") {m.ready=true;send(m,{action:"navigation",...history?.state(m.session)});send(m,{action:"editorStyle",...editorStyle.read(vscode,m.document.uri)});await refresh(m,selectedThreads.get(m.id.session));return;}
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
      if(message.action==="jump") {
        const {current}=await fresh(m,message,true);
        const row=m.layout.rows[message.line-2];
        if(!row?.safe)throw new Error("Execution can move only to an instruction with a stop point.");
        if(row.line===current.line)return;
        try {
          await m.session.customRequest("uwvm/jump",{level:"wasm",target:row.offset,ifStop:current.stopId});
          send(m,{action:"notice",message:"Execution moved to "+row.address+"."});
        } catch(error) {
          throw new Error("The pointer stays at "+m.layout.rows[current.line-2].address+": "+String(error?.message||error).replace(/^jump unavailable: /,""));
        }
        return;
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
    const restoring=restores.get(document.uri.toString());
    const m={id,document,panel,session,revision:0,ready:false,disposed:false,historyRestore:restoring?.place};
    restores.delete(document.uri.toString());panels.add(m);
    if(history) {
      const binding=restoring?.binding||{};binding.target=m;
      if(!binding.entry)binding.entry=history.register(session,async(place,valid)=>{
        if(!valid())return;
        const target=binding.target;
        if(target.disposed){await open({sessionId:session.id,source:{path:document.uri.toString()},line:place.line,viewport:place,historyBinding:binding,isCurrent:valid});return;}
        target.panel.reveal(target.panel.viewColumn,false);target.viewport=place;send(target,{action:"restore-viewport",...place});
      },()=>binding.target.viewport);
      m.navigation=binding.entry;
      add(history.subscribe(()=>send(m,{action:"navigation",...history.state(session)})));
      add(panel.onDidChangeViewState(()=>{if(panel.active)m.navigation.visit(m.viewport||{line:m.current?.line||2,top:0,left:0});}));
    }
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
  add(vscode.debug.onDidTerminateDebugSession(s=>{selectedThreads.delete(s.id);stacks.delete(s.id);followedStops.delete(s.id);for(const m of panels)if(m.id.session===s.id)retire(m,"Debug session ended; retained code is historical only.");}));
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
        // Source-backed frames retain native source-marker ownership. Their
        // authenticated Wasm document remains a separate code projection.
        const frame=packet.body?.stackFrames?.map(frame=>frame.uwvmWasmLocation||frame)
          .find(frame=>typeof frame.source?.path==="string"&&frame.source.path.startsWith("uwvm-wasm:"));
        if(!frame)return;selectedThreads.set(session.id,thread);stacks.set(session.id,frame.source.path);
        for(const m of panels)if(m.id.session===session.id && vscode.Uri.parse(frame.source.path).toString()===m.document.uri.toString())void refresh(m,thread);
      }
    };
  }}));
  async function open(selection) {
    const session=vscode.debug.activeDebugSession;
    if(!session||sessions.get(session.id)!==session)throw new Error("Select a live UWVM debug session.");
    const isCurrent=selection?.isCurrent||navigation.begin(session);
    const live=()=>isCurrent()&&vscode.debug.activeDebugSession===session&&sessions.get(session.id)===session;
    await session.customRequest("uwvm/wasmView",{visible:true});
    if(!live())return;
    let source;
    if(selection && selection.sessionId===session.id && selection.source?.path?.startsWith("uwvm-wasm:") && Number.isSafeInteger(selection.line) && selection.line>0) source=selection.source;
    else {
      const result=await session.customRequest("uwvm/codeLocation",{level:"wasm"});source=result.source;selection=result;
    }
    if(!live())return;
    if(!source?.path?.startsWith("uwvm-wasm:"))throw new Error("A current Wasm document is required.");
    const uri=vscode.Uri.parse(source.path);
    // Reveal the instance that is already open (never a second copy), and
    // open new ones in the code group, not in the inspector group beside it.
    const existing=[...panels].find(p=>!p.disposed&&p.document.uri.toString()===uri.toString());
    if(existing){existing.panel.reveal(existing.panel.viewColumn,selection?.preserveFocus===true);if(selection?.line){existing.navigation?.visit({line:selection.line,top:Math.max(0,selection.line-2),left:0});send(existing,{action:"reveal",line:selection.line});}return;}
    if(selection?.viewport)restores.set(uri.toString(),{place:selection.viewport,binding:selection.historyBinding});
    else if(selection?.line)reveals.set(uri.toString(),selection.line);
    return vscode.commands.executeCommand("vscode.openWith",uri,VIEW,{preview:selection?.preview!==false,preserveFocus:selection?.preserveFocus===true,viewColumn:codeColumn()});
  }
  function codeColumn() {
    const groups=vscode.window.tabGroups?vscode.window.tabGroups.all:[];
    const inspector=tab=>tab.input&&typeof tab.input.viewType==="string"&&/uwvm\.(memory|registers|wasip1|ros|scene|diagnostics|breakpoints|watch|signedUpdate)\b/.test(tab.input.viewType);
    const group=groups.find(g=>g.tabs.some(t=>!inspector(t)))||groups.find(g=>!g.tabs.length);
    return group?group.viewColumn:vscode.ViewColumn.One;
  }
  add(vscode.commands.registerCommand("uwvm.openWasmInstructions",open));
  return {open,provider,panels,positions};
}
module.exports={register,VIEW};
