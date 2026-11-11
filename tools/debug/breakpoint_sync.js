"use strict";

// DAP has no enabled field on Breakpoint events. Use the editor's supported
// SourceBreakpoint API for that display state, and send its complete enabled /
// disabled inventory over an authenticated, session-bound request.
function register(vscode,context,sessions) {
  const api=vscode.debug;
  if(!api?.onDidChangeBreakpoints || !api.addBreakpoints || !api.removeBreakpoints || !vscode.SourceBreakpoint)return;
  const states=new Map();let mirroring=0;
  const state=session=>{if(!states.has(session.id))states.set(session.id,{session,paths:new Map(),registered:new Set(),permission:undefined,bound:new Set(),queue:Promise.resolve(),pushing:0,echo:new Map(),runtime:new Map(),scheduled:new Map(),flushQueued:false});return states.get(session.id);};
  const uriFor=path=>path.startsWith("uwvm-wasm:")?vscode.Uri.parse(path):vscode.Uri.file(path);
  const keyFor=path=>uriFor(path).toString();
  const pointsAt=uri=>api.breakpoints.filter(point=>point instanceof vscode.SourceBreakpoint && point.location.uri.toString()===uri.toString());
  const rowsAt=uri=>pointsAt(uri).map(point=>({line:point.location.range.start.line+1,enabled:point.enabled,
    condition:point.condition||"",hitCondition:point.hitCondition||"",logMessage:point.logMessage||""})).sort((a,b)=>a.line-b.line);
  const fingerprint=rows=>JSON.stringify(rows);
  const alive=s=>sessions.get(s.session.id)===s.session;
  function remember(s,path) {
    if(typeof path!=="string" || !path || /[\x00-\x1f]/.test(path))return;
    const uri=uriFor(path);
    if(uri.scheme==="uwvm-wasm" && new URLSearchParams(uri.query).get("session")!==s.session.id)return;
    s.paths.set(uri.toString(),{path,uri});
  }
  function publish(s,entry) {
    if(!alive(s) || s.permission!=="full")return;
    // Capture the user edit now, then serialize writes. Do not retry a failed
    // or uncertain mutation, and do not send another session's private URI.
    const rows=rowsAt(entry.uri);s.pushing++;
    s.queue=s.queue.then(async()=>{
      if(!alive(s))return;
      const result=await s.session.customRequest("uwvm/editorBreakpoints",{source:{path:entry.path},breakpoints:rows});
      const refused=result?.breakpoints?.find(point=>!point.verified);
      if(refused)throw new Error(refused.message||"The runtime rejected a breakpoint");
      // A first standard request may arrive before this complete inventory and
      // receive a pending response. Re-register that API point once, so the
      // editor associates its own ID with the now-existing runtime ID. Never
      // create a second point through a DAP 'new' event for the same location.
      const pending=(result?.breakpoints||[]).filter(point=>point.verified && !s.bound.has(point.id));
      const replace=pointsAt(entry.uri).filter(point=>pending.some(row=>row.line===point.location.range.start.line+1));
      if(replace.length) {
        mirroring++;
        try {
          api.removeBreakpoints(replace);
          api.addBreakpoints(replace.map(point=>new vscode.SourceBreakpoint(point.location,point.enabled,point.condition,point.hitCondition,point.logMessage)));
          s.echo.set(entry.uri.toString(),fingerprint(rowsAt(entry.uri)));
        } finally {mirroring--;}
      }
    }).catch(error=>{
      if(alive(s))void vscode.window?.showWarningMessage("UWVM breakpoint change: "+String(error.message||error));
    }).finally(async()=>{
      s.pushing--;
      if(!s.pushing && alive(s)) {
        s.pending=undefined;
        // Earlier inventory events may precede a successful mutation. Fetch a
        // fresh read-only inventory; never replay that earlier enabled state.
        try {const packet=await s.session.customRequest("uwvm/breakpoints",{});if(!s.pushing)mirror(s,packet);}catch(_){}
      }
    });
  }
  function schedulePublish(s,entry) {
    // SourceBreakpoint properties are immutable. A custom-editor edit uses
    // remove + add in the same turn; publishing the intermediate empty list
    // would delete the runtime point, its hit count and its callbacks. Observe
    // the final inventory once the complete synchronous API edit has finished.
    s.scheduled.set(entry.uri.toString(),entry);
    if(s.flushQueued)return;
    s.flushQueued=true;
    queueMicrotask(()=>{
      s.flushQueued=false;
      const pending=[...s.scheduled];s.scheduled.clear();
      if(!alive(s))return;
      for(const [key,current] of pending) {
        const mark=fingerprint(rowsAt(current.uri));
        // Mirrored events may arrive asynchronously after remove + add. Keep
        // their final fingerprint until a genuinely different user edit.
        if(s.echo.get(key)===mark)continue;
        s.echo.delete(key);publish(s,current);
      }
    });
  }
  function mirror(s,packet) {
    if(!alive(s) || !packet || packet.available===false || !Array.isArray(packet.rows) || !packet.locations)return;
    if(Object.keys(packet.locations).some(id=>!packet.rows.some(row=>String(row.id)===id)))return;
    if(s.pushing){s.pending=packet;return;}
    const next=new Map();
    for(const row of packet.rows) {
      const location=packet.locations[String(row.id)];
      if(!location?.source?.path || !Number.isSafeInteger(location.line) || location.line<1 || typeof row.enabled!=="boolean")continue;
      remember(s,location.source.path);
      const key=keyFor(location.source.path),entry=s.paths.get(key);if(!entry)continue;
      // A real source URI is workspace-global in VS Code. Never use one VM's
      // console update to overwrite another VM's source breakpoint switches.
      // Per-session WAT documents and the Breakpoints window remain distinct.
      if(entry.uri.scheme!=="uwvm-wasm" && [...states.values()].some(other=>other!==s && alive(other) && other.paths.has(key)))continue;
      next.set(String(row.id),{row,location,entry});
    }
    const byPath=new Map();
    for(const item of [...s.runtime.values(),...next.values()])byPath.set(item.entry.uri.toString(),item.entry);
    mirroring++;
    try {
      for(const [key,entry] of byPath) {
        if(entry.uri.scheme!=="uwvm-wasm" && [...states.values()].some(other=>other!==s && alive(other) && other.paths.has(key)))continue;
        const wanted=[...next.values()].filter(item=>item.entry.uri.toString()===key);
        const previous=[...s.runtime.values()].filter(item=>item.entry.uri.toString()===key);
        const remove=[],add=[],existing=pointsAt(entry.uri);
        for(const item of previous)if(!next.has(String(item.row.id)) && !wanted.some(p=>p.location.line===item.location.line))
          remove.push(...existing.filter(p=>p.location.range.start.line+1===item.location.line));
        for(const item of wanted) {
          const {row,location}=item,at=existing.filter(p=>p.location.range.start.line+1===location.line);
          const hit=Number(row.ignore)>0?">"+row.ignore:"",condition=row.condition||"";
          const desiredHit=at.length===1 && row.enabled===at[0].enabled && condition===(at[0].condition||"")?at[0].hitCondition||"":hit;
          if(at.length===1 && at[0].enabled===row.enabled && (at[0].condition||"")===condition && (at[0].hitCondition||"")===desiredHit)continue;
          if(wanted.filter(p=>p.location.line===location.line).length!==1)continue;
          remove.push(...at);
          add.push(new vscode.SourceBreakpoint(new vscode.Location(entry.uri,new vscode.Position(location.line-1,0)),row.enabled,condition||undefined,desiredHit||undefined));
        }
        if(remove.length)api.removeBreakpoints([...new Set(remove)]);
        if(add.length)api.addBreakpoints(add);
        s.echo.set(key,fingerprint(rowsAt(entry.uri)));
      }
    } finally {mirroring--;}
    s.runtime=next;
  }
  context.subscriptions.push(api.onDidChangeBreakpoints(event=>{
    if(mirroring)return;
    const changed=new Set([...event.added,...event.changed,...event.removed].filter(p=>p instanceof vscode.SourceBreakpoint).map(p=>p.location.uri.toString()));
    for(const s of states.values())for(const key of changed) {
      const entry=s.paths.get(key);if(!entry)continue;
      schedulePublish(s,entry);
    }
  }));
  context.subscriptions.push(api.onDidTerminateDebugSession(session=>states.delete(session.id)));
  if(api.registerDebugAdapterTrackerFactory)context.subscriptions.push(api.registerDebugAdapterTrackerFactory("uwvm-llvm-full",{
    createDebugAdapterTracker(session) {const s=state(session);return {
      onWillReceiveMessage(packet) {
        if(packet.type!=="request" || packet.command!=="setBreakpoints")return;
        const path=packet.arguments?.source?.path;if(typeof path!=="string")return;
        const key=keyFor(path);remember(s,path);
        // First registration also binds disabled points omitted by DAP.
        if(!s.registered.has(key) && s.paths.has(key)){s.registered.add(key);publish(s,s.paths.get(key));}
      },onDidSendMessage(packet) {
        if(packet.type==="response" && packet.command==="setBreakpoints" && packet.success)
          for(const point of packet.body?.breakpoints||[])if(point.verified && Number.isSafeInteger(point.id))s.bound.add(point.id);
        if(packet.type!=="event")return;
        if(packet.event==="uwvm/permissions"){s.permission=packet.body?.permissions;}
        if(packet.event==="uwvm/breakpoints")mirror(s,packet.body);
      }
    };}
  }));
  return true;
}
module.exports={register};
