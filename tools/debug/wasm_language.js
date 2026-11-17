"use strict";

// Language assistance for authenticated, stop-scoped Wasm virtual documents.
// There is no expression evaluation, guest execution, host file access or
// address dereference in this provider.
const {URLSearchParams}=require("url");
const {createHash}=require("crypto");
const path=require("path");
const OPCODES=require("./wasm_opcodes.json");
const LANGUAGE="uwvm-wasm-disassembly";
const SESSION=/^[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}$/;

function identity(uri) {
  if(!uri || uri.scheme!=="uwvm-wasm" || uri.authority || uri.fragment)return undefined;
  const path=/^\/module-([0-9]{1,20})\/function-([0-9]{1,20})\/code-([1-9][0-9]{0,19})-([1-9][0-9]{0,19})\.uwvm-wat$/.exec(uri.path);
  if(!path)return undefined;
  const q=new URLSearchParams(uri.query),session=q.get("session"),reference=q.get("ref");
  if(!session || !SESSION.test(session) || !reference || !/^[1-9][0-9]{0,15}$/.test(reference) ||
     uri.query!=="session="+session+"&ref="+reference)return undefined;
  const ref=Number(reference);
  if(!Number.isSafeInteger(ref) || path.slice(1).some(n=>BigInt(n)>=1n<<64n || BigInt(n).toString()!==n))return undefined;
  return {module:path[1],function:path[2],epoch:path[3],generation:path[4],session,reference:ref};
}

function instructionLine(text) {
  const match=/^;;[ \t]+(0x(?:[0-9a-f]{16}|[0-9a-f]{8}))[ \t]+([A-Za-z0-9_.:]+)(.*)$/.exec(text);
  if(!match || BigInt(match[1])>=1n<<64n || !/^0x[0-9a-f]+$/.test(match[1]))return undefined;
  const opcode=Object.hasOwn(OPCODES,match[2])?OPCODES[match[2]]:undefined;
  if(!opcode)return undefined; // Unknown/new encodings never acquire invented semantics.
  const start=text.indexOf(match[2],text.indexOf(match[1])+match[1].length);
  return {offset:BigInt(match[1]).toString(),offsetText:match[1],offsetStart:text.indexOf(match[1]),name:match[2],operands:match[3].trim(),start,end:start+match[2].length,opcode};
}


// Only decoded tokens have hover meaning. Whitespace and undecoded byte tails
// never inherit the mnemonic's hover range.
function hoverToken(text,character,row=instructionLine(text)) {
  if(!row || !Number.isInteger(character) || character<0)return undefined;
  if(character>=row.offsetStart && character<row.offsetStart+row.offsetText.length)
    return {kind:"location",start:row.offsetStart,end:row.offsetStart+row.offsetText.length};
  if(character>=row.start && character<row.end)return {kind:"instruction",start:row.start,end:row.end};
  const suffix=text.slice(row.end).split(";",1)[0];
  let ordinal=0;
  for(const match of suffix.matchAll(/\S+/g)) {
    const start=row.end+match.index,end=start+match[0].length,index=ordinal++;
    if(character<start || character>=end)continue;
    const field=/^(memory|offset|align|lane)=([0-9]+)$/.exec(match[0]);
    if(field)return {kind:field[1]==="memory"?"object":"operand",key:field[1],value:field[2],start,end};
    if(!/^-?(?:0x[0-9a-fA-F]+|[0-9]+)$/.test(match[0]))return undefined;
    if(index===0) {
      let key;
      if(row.opcode.family==="MTE")key="memory";
      else if(row.name.startsWith("local."))key="local";
      else if(row.name.startsWith("global."))key="global";
      else if(["call","return_call","ref.func"].includes(row.name))key="function";
      else if(["table.get","table.set"].includes(row.name))key="table";
      else if(["br","br_if"].includes(row.name))key="label";
      if(key && /^[0-9]+$/.test(match[0]))return {kind:"object",key,value:match[0],start,end};
      if(["i32.const","i64.const"].includes(row.name))
        return {kind:"literal",value:match[0],type:row.name.split(".")[0],start,end};
    }
    return {kind:"operand",key:"immediate",value:match[0],start,end};
  }
  return undefined;
}

function hoverContent(row,token,id) {
  if(token.kind==="instruction")return {
    code:row.name,description:row.opcode.brief||"",
    detail:row.opcode.stackOperands && row.opcode.stackOperands.length?
      "Stack: "+row.opcode.stackOperands.join(", ")+" → "+(row.opcode.results?"value":"∅"):""
  };
  if(token.kind==="location")return {
    code:"@0x"+BigInt(row.offset).toString(16),
    description:"Module "+id.module+" · function "+id.function
  };
  if(token.kind==="object") {
    const descriptions={
      memory:"Linear memory in module "+id.module+".",
      table:"Reference table in module "+id.module+".",
      function:"Function in module "+id.module+".",
      local:"Local slot in function "+id.function+".",
      global:"Global in module "+id.module+".",
      label:"Branch depth in the current function."
    };
    return {code:(token.key==="function"?"func":token.key)+" "+token.value,description:descriptions[token.key]};
  }
  if(token.kind==="literal")return {code:token.value,description:token.type+" constant."};
  const descriptions={
    offset:"Byte displacement added to the address.",
    align:"Memory alignment in bytes.",
    lane:"SIMD lane index.",
    immediate:"Decoded instruction immediate."
  };
  return {code:token.key+"="+token.value,description:descriptions[token.key]};
}

function stopId(reply) {
  if(!reply || typeof reply.text!=="string")throw new Error("Unavailable: debugger stop could not be authenticated.");
  const matches=[...reply.text.matchAll(/^stop-id ([1-9][0-9]{0,19})$/gm)];
  if(matches.length!==1 || BigInt(matches[0][1])>=1n<<64n)throw new Error("Unavailable: no current stopped Wasm snapshot.");
  return matches[0][1];
}

function register(vscode,context,sessions) {
  // Old headless extension test hosts do not implement language/UI APIs.
  if(!vscode.languages || !vscode.languages.registerHoverProvider || !vscode.languages.setTextDocumentLanguage)return;
  const epochs=new Map();
  const decorations=new Map();
  const decorationRequests=new WeakMap();
  const disabledBreakpoints=new Map();
  const add=value=>context.subscriptions.push(value);
  const epoch=id=>epochs.get(id)||0;
  const icon=name=>vscode.Uri&&typeof vscode.Uri.file==="function"?
    vscode.Uri.file(path.join(context.extensionPath,"icons",name)):undefined;
  const exactOptions={
    isWholeLine:true, backgroundColor:new vscode.ThemeColor("editor.stackFrameHighlightBackground"),
    overviewRulerColor:new vscode.ThemeColor("editor.stackFrameHighlightBackground"),
    overviewRulerLane:vscode.OverviewRulerLane&&vscode.OverviewRulerLane.Full,
    rangeBehavior:vscode.DecorationRangeBehavior&&vscode.DecorationRangeBehavior.ClosedClosed,
    light:{gutterIconPath:icon("pointer-exact-light.svg")},
    dark:{gutterIconPath:icon("pointer-exact-dark.svg")},gutterIconSize:"contain"
  };
  const nextOptions={
    isWholeLine:true, backgroundColor:new vscode.ThemeColor("editor.focusedStackFrameHighlightBackground"),
    overviewRulerColor:new vscode.ThemeColor("editor.focusedStackFrameHighlightBackground"),
    overviewRulerLane:vscode.OverviewRulerLane&&vscode.OverviewRulerLane.Full,
    rangeBehavior:vscode.DecorationRangeBehavior&&vscode.DecorationRangeBehavior.ClosedClosed,
    light:{gutterIconPath:icon("pointer-next-light.svg")},
    dark:{gutterIconPath:icon("pointer-next-dark.svg")},gutterIconSize:"contain"
  };
  // The virtual Wasm document has its own authenticated exact/next markers.
  // VS Code's ordinary current-frame decoration is intentionally not reused:
  // it may be green when a source frame is focused, which loses the distinction
  // between an exact Wasm stop and a native stop inside that instruction.
  const exactPc=vscode.window.createTextEditorDecorationType(exactOptions);
  const nextPc=vscode.window.createTextEditorDecorationType(nextOptions);
  add(exactPc);add(nextPc);
  // VS Code merges the current-PC arrow and breakpoint glyph in one gutter
  // cell. Its disabled glyph can then be visually covered by that arrow.
  // Keep native gutter ownership; annotate only a confirmed disabled point at
  // the current instruction, without shifting instruction text or columns.
  const disabledAtPC=vscode.window.createTextEditorDecorationType({
    after:{color:new vscode.ThemeColor("debugIcon.breakpointDisabledForeground"),margin:"0 0 0 2em"},
    rangeBehavior:vscode.DecorationRangeBehavior.ClosedClosed
  });
  add(disabledAtPC);
  function annotateDisabledAtPC(editor,id) {
    const points=disabledBreakpoints.get(id.session)?.get(editor.document.uri.toString())?.get(id.line)||[];
    const end=editor.document.lineAt(id.line-1).text.length;
    editor.setDecorations(disabledAtPC,points.length?[{
      range:new vscode.Range(id.line-1,end,id.line-1,end),
      renderOptions:{after:{contentText:points.length===1?"● Breakpoint disabled":"● "+points.length+" breakpoints disabled"}},
      hoverMessage:"Disabled runtime breakpoint"+(points.length===1?" ":"s ")+points.map(point=>"#"+point).join(", ")+". Retained, but will not stop execution."
    }]:[]);
  }
  function breakpointInventory(session,packet) {
    if(!packet || packet.available===false || !Array.isArray(packet.rows) || !packet.locations)return;
    const paths=new Map();
    for(const row of packet.rows) {
      if(row.enabled!==false || !Number.isSafeInteger(row.id) || row.id<=0)continue;
      const location=packet.locations[String(row.id)];
      if(typeof location?.source?.path!=="string" || !Number.isSafeInteger(location.line) || location.line<2)continue;
      let uri;
      try{uri=vscode.Uri.parse(location.source.path);}catch(_){continue;}
      const id=identity(uri);
      if(!id || id.session!==session.id)continue;
      const key=uri.toString();
      if(!paths.has(key))paths.set(key,new Map());
      const lines=paths.get(key);
      if(!lines.has(location.line))lines.set(location.line,[]);
      lines.get(location.line).push(row.id);
    }
    disabledBreakpoints.set(session.id,paths);
    for(const editor of vscode.window.visibleTextEditors||[]) {
      const id=identity(editor.document.uri);
      if(id && id.session===session.id)void decorate(editor);
    }
  }
  const coordinates=vscode.window.createTextEditorDecorationType({color:new vscode.ThemeColor("descriptionForeground")});
  add(coordinates);
  function decorateCoordinates(editor) {
    const ranges=[];
    for(let line=1;line<editor.document.lineCount;++line) {
      const row=instructionLine(editor.document.lineAt(line).text);
      if(row)ranges.push(new vscode.Range(line,row.offsetStart,line,row.offsetStart+row.offsetText.length));
    }
    editor.setDecorations(coordinates,ranges);
  }
  function retire(session) {
    epochs.set(session.id,epoch(session.id)+1);
    for(const [editor,owner] of decorations)if(owner===session.id) {
      editor.setDecorations(exactPc,[]);editor.setDecorations(nextPc,[]);
      editor.setDecorations(disabledAtPC,[]);decorations.delete(editor);
    }
  }
  function live(document,id,session,stamp,version,cancel) {
    if(document.isClosed || document.version!==version || sessions.get(id.session)!==session ||
       epoch(id.session)!==stamp || cancel && cancel.isCancellationRequested)
      throw new Error("Unavailable: this Wasm document belongs to a retired stop or session.");
  }
  async function authenticate(document,cancel) {
    const id=identity(document.uri);
    if(!id)throw new Error("Unavailable: not an authenticated UWVM Wasm document.");
    const session=sessions.get(id.session),stamp=epoch(id.session),version=document.version;
    if(!session || session.type!=="uwvm-llvm-full")throw new Error("Unavailable: the debug session has ended.");
    live(document,id,session,stamp,version,cancel);
    const permission=await session.customRequest("uwvm/permissions",{});
    live(document,id,session,stamp,version,cancel);
    if(!permission || !["full","read-only"].includes(permission.permissions))
      throw new Error("Permission denied: Wasm instruction inspection requires full or read-only debugger permission.");
    const reply=await session.customRequest("uwvm/wasmDocument",{reference:id.reference});
    live(document,id,session,stamp,version,cancel);
    if(!reply || reply.reference!==id.reference || reply.module!==id.module || reply.function!==id.function ||
       reply.epoch!==id.epoch || reply.generation!==id.generation ||
       !/^[1-9][0-9]{0,19}$/.test(reply.stopId) || !Number.isInteger(reply.line) ||
       reply.line<2 || reply.line>document.lineCount ||
       reply.digest!==createHash("sha256").update(document.getText(),"utf8").digest("hex"))
      throw new Error("Unavailable: instruction document differs from the current runtime.");
    const row=instructionLine(document.lineAt(reply.line-1).text);
    if(!row || row.offset!==reply.offset)throw new Error("Unavailable: current instruction mapping differs.");
    return {...id,stop:reply.stopId,line:reply.line,offset:reply.offset,stamp,version,sessionObject:session};
  }
  function readable(md) {
    // VS Code appendText escapes spaces as &nbsp;. Keep all Markdown/HTML
    // escaping and the untrusted flag, but let long instruction descriptions
    // wrap within the editor hover instead of overflowing horizontally.
    md.value=md.value.replace(/&nbsp;/g," ");
    return md;
  }
  function message(text) {
    const md=new vscode.MarkdownString();
    md.isTrusted=false;md.supportHtml=false;md.appendText(text);
    return readable(md);
  }
  const hover={async provideHover(document,position,cancel) {
    const row=instructionLine(document.lineAt(position.line).text);
    const token=hoverToken(document.lineAt(position.line).text,position.character,row);
    if(!token)return undefined;
    const range=new vscode.Range(position.line,token.start,position.line,token.end);
    // Mouse delay/cancellation are owned by the editor (language default 350ms).
    // Do not add a second timer or turn explicit keyboard hover into a wait.
    try {
      const id=await authenticate(document,cancel);
      if(cancel && cancel.isCancellationRequested)return undefined;
      const content=hoverContent(row,token,id);
      if(token.kind==="object" && ["function","local","global","memory","table"].includes(token.key)) {
        const value=await id.sessionObject.customRequest("uwvm/wasmHover",{
          reference:id.reference,ifStop:id.stop,kind:token.key,index:token.value
        });
        live(document,id,id.sessionObject,id.stamp,id.version,cancel);
        if(!value || value.stopId!==id.stop || value.kind!==token.key || value.index!==token.value)
          throw new Error("Unavailable: value belongs to a different stop.");
        if(value.name)content.code+=("  "+value.name);
        if(value.type!==undefined)content.code+=("\n"+value.type+"  "+value.value);
        if(value.unavailable)content.detail="Value unavailable: "+value.unavailable;
        else if(value.addressBits)content.detail=value.addressBits+"-bit address space";
        if(value.imported)content.description+=" Imported by this module.";
      }
      const md=new vscode.MarkdownString();md.isTrusted=false;md.supportHtml=false;
      md.appendCodeblock(content.code,LANGUAGE);
      if(content.description)md.appendText(content.description);
      if(content.detail)md.appendText((content.description?"\n\n":"")+content.detail);
      return new vscode.Hover(readable(md),range);
    } catch(error) {
      if(cancel && cancel.isCancellationRequested)return undefined;
      const text=error && /^Permission denied:/.test(error.message)?
        "Permission denied: requires full or read-only.":
        "Unavailable: no current authorized Wasm snapshot.";
      return new vscode.Hover(message(text),range);
    }
  }};
  add(vscode.languages.registerHoverProvider({scheme:"uwvm-wasm",language:LANGUAGE},hover));

  async function decorate(editor) {
    if(!editor || !identity(editor.document.uri))return;
    const request=Symbol();decorationRequests.set(editor,request);
    try {
      const id=identity(editor.document.uri),session=sessions.get(id.session);
      if(!session)throw new Error("debug session ended");
      let exactLine, nextLine, stopId;
      // Positions are the single cross-level source of truth.  In particular,
      // a native stop is allowed to mark this document only when the runtime
      // authenticated its LLVM-JIT owner and Wasm byte offset.
      let position;
      try { position=await session.customRequest("uwvm/positions",{}); } catch(_) { position=undefined; }
      if(position?.state==="stopped" && position.wasm &&
         String(position.wasm.module)===id.module && String(position.wasm.function)===id.function &&
         String(position.wasm.epoch||id.epoch)===id.epoch && String(position.wasm.generation||id.generation)===id.generation &&
         (!position.native || position.wasm.provenance==="exact")) {
        const wanted=String(position.wasm.offset);
        for(let line=1;line<editor.document.lineCount;line++) {
          const row=instructionLine(editor.document.lineAt(line).text);
          if(row&&row.offset===wanted) { if(position.wasm.exact===true)exactLine=line; else nextLine=line; break; }
        }
        stopId=position.stopId;
      }
      // Cooperative Wasm stops are also accepted through the strict document
      // authentication path. This keeps the language provider usable with an
      // older adapter that has not implemented uwvm/positions yet.
      // An authenticated native stop without an exact Wasm provenance must
      // leave the Wasm document unmarked.  Falling back to the document's
      // own initial stop marker here would fabricate a Wasm position and make
      // an unrelated native instruction look executable.
      if(exactLine===undefined && nextLine===undefined &&
         (!position || position.state!="stopped" || !position.native)) {
        const authenticated=await authenticate(editor.document);
        if(decorationRequests.get(editor)!==request)return;
        exactLine=authenticated.line-1;stopId=authenticated.stop;
      }
      if(decorationRequests.get(editor)!==request)return;
      const list=line=>line===undefined?[]:[{range:new vscode.Range(line,0,line,editor.document.lineAt(line).text.length),hoverMessage:
        line===exactLine?"Exact Wasm instruction position (yellow).":"Execution is inside this Wasm instruction (green); this marker cannot be moved."}];
      editor.setDecorations(exactPc,list(exactLine));
      editor.setDecorations(nextPc,list(nextLine));
      if(exactLine!==undefined)annotateDisabledAtPC(editor,{...id,line:exactLine+1,stop:stopId});
      else if(nextLine!==undefined)annotateDisabledAtPC(editor,{...id,line:nextLine+1,stop:stopId});
      else editor.setDecorations(disabledAtPC,[]);
      decorations.set(editor,id.session);
    } catch(_) {if(decorationRequests.get(editor)===request){editor.setDecorations(exactPc,[]);editor.setDecorations(nextPc,[]);editor.setDecorations(disabledAtPC,[]);decorations.delete(editor);}}
  }
  async function opened(document) {
    if(!identity(document.uri))return;
    try {
      if(document.languageId!==LANGUAGE)await vscode.languages.setTextDocumentLanguage(document,LANGUAGE);
      for(const editor of vscode.window.visibleTextEditors||[])if(editor.document===document) {decorateCoordinates(editor);void decorate(editor);}
    } catch(_) { /* A retired virtual document never falls back to a host file. */ }
  }
  if(vscode.workspace.onDidOpenTextDocument)add(vscode.workspace.onDidOpenTextDocument(opened));
  if(vscode.window.onDidChangeVisibleTextEditors)add(vscode.window.onDidChangeVisibleTextEditors(editors=>{
    for(const [editor] of decorations)if(!editors.includes(editor))decorations.delete(editor);
    for(const editor of editors)void opened(editor.document);
  }));
  if(vscode.workspace.onDidCloseTextDocument)add(vscode.workspace.onDidCloseTextDocument(document=>{
    for(const [editor] of decorations)if(editor.document===document)decorations.delete(editor);
  }));
  if(vscode.debug.registerDebugAdapterTrackerFactory)add(vscode.debug.registerDebugAdapterTrackerFactory("uwvm-llvm-full",{
    createDebugAdapterTracker(session) {return {
      onWillReceiveMessage(packet) {
        if(packet.type!=="request")return;
        if(["continue","next","stepIn","stepOut","pause","restart","restartFrame","goto","writeMemory","setVariable",
            "setExpression","uwvm/signedUpdate"].includes(packet.command) ||
           packet.command==="evaluate" && packet.arguments && packet.arguments.context==="repl")retire(session);
      },
      onDidSendMessage(packet) {
        if(packet.type==="event" && packet.event==="uwvm/breakpoints")breakpointInventory(session,packet.body);
        if(packet.type==="event" && ["stopped","continued","invalidated","terminated","exited","memory"].includes(packet.event)) {
          retire(session);
          if(packet.event==="stopped")setTimeout(()=>{for(const editor of vscode.window.visibleTextEditors||[]) {
            const id=identity(editor.document.uri);if(id&&id.session===session.id)void decorate(editor);
          }},0);
        }
        if(packet.type==="response" && packet.command==="stackTrace" && packet.success)
          for(const editor of vscode.window.visibleTextEditors||[]) {
            const id=identity(editor.document.uri);if(id && id.session===session.id)void decorate(editor);
          }
      }
    };}
  }));
  add(vscode.debug.onDidTerminateDebugSession(session=>{retire(session);epochs.delete(session.id);disabledBreakpoints.delete(session.id);}));
  for(const document of vscode.workspace.textDocuments||[])void opened(document);
  return {authenticate,hover,retire,decorate};
}

module.exports={register,identity,instructionLine,hoverToken,hoverContent,LANGUAGE};
