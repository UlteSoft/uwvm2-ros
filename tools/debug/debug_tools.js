"use strict";

// Native Run and Debug sidebar navigation. No VM call is made until a real
// session exists; every action still goes through the authenticated adapter.
function register(vscode,context,sessions,inspection,wasmView) {
  if(!vscode.window || !vscode.window.registerTreeDataProvider || !vscode.EventEmitter)return;
  const changed=new vscode.EventEmitter(),permissions=new Map(),levels=new Map();
  const publish=()=>{const session=active();void vscode.commands.executeCommand("setContext","uwvm.stepLevel",session?levels.get(session.id):undefined);changed.fire();};
  const active=()=>{const s=vscode.debug.activeDebugSession;return s&&sessions.get(s.id)===s?s:undefined;};
  const actions=[
    ["Source instructions","uwvm.levelSource","file-code","Inspect source lines and source variables","inspect"],
    ["Wasm instructions","uwvm.levelWasm","symbol-method","Inspect WAT, locals, operand and control stacks","inspect"],
    ["JIT assembly","uwvm.levelAsm","code","Only machine code owned by the current Wasm JIT function","inspect"],
    ["Memory & allocation tags","uwvm.openMemory","database","Inspect bytes, tags and values; full permission can edit","inspect"],
    ["Registers","uwvm.openRegisters","symbol-variable","Registers at a genuine owned native instruction stop","inspect"],
    ["WASIp1 environment & policy","uwvm.openWasip1","terminal","Arguments, environment, descriptors, preopens and policy","inspect"],
    ["ROS1 / ROS2 & policy","uwvm.openRos","radio-tower","Policy groups, instances, interfaces and events","inspect"],
    ["Recorded WASIp1 calls","uwvm.openWasiCalls","list-tree","Recorded calls, payload details and JSON export","inspect"],
    ["Recorded ROS calls","uwvm.openRosCalls","list-tree","Filter ROS1 or ROS2, inspect payloads and export","inspect"],
    ["Breakpoints","uwvm.openBreakpoints","debug-breakpoint","All console and GUI breakpoints","inspect"],
    ["Diagnostics & permissions","uwvm.showDiagnostics","info","Current state, stop reason and immutable permission level","diagnostics"],
    ["Save Wasm dump","uwvm.dump","save","Current stopped snapshot; limited permission redacts sensitive data","diagnostics"],
    ["Signed hot update","uwvm.submitSignedUpdate","sync","Independent update authorization; debugger permission does not grant updates","independent"]
  ];
  const provider={onDidChangeTreeData:changed.event,getChildren(){
    const session=active(),permission=session?permissions.get(session.id):undefined;
    return [{label:session?session.name:"No UWVM session",description:permission||"Not attached",status:true},
      ...actions.map(([label,command,icon,tooltip,access])=>({label,command,icon,tooltip,access,permission,session}))];
  },getTreeItem(row){
    const item=new vscode.TreeItem(row.label,vscode.TreeItemCollapsibleState.None);
    item.description=row.description;item.tooltip=row.tooltip||row.description;
    if(row.icon)item.iconPath=new vscode.ThemeIcon(row.icon);
    if(!row.status){
      const denied=row.access!=="independent"&&(!row.session || !row.permission || row.permission==="none" || row.access==="inspect"&&row.permission==="limited");
      item.command=denied?{command:"uwvm.explainUnavailable",title:row.label,arguments:[row.session?"This view requires full or read-only permission. The running VM's permission is fixed at launch.":"Start or attach a UWVM debug configuration first."]}:{command:row.command,title:row.label};
      if(denied){item.description="Unavailable";item.tooltip+=" · "+item.command.arguments[0];}
    }return item;
  }};
  context.subscriptions.push(changed,vscode.window.registerTreeDataProvider("uwvm.tools",provider));
  context.subscriptions.push(vscode.commands.registerCommand("uwvm.explainUnavailable",text=>vscode.window.showInformationMessage(text)));
  for(const [command,level] of [["uwvm.levelSource","source"],["uwvm.levelWasm","wasm"],["uwvm.levelAsm","native"]])
    context.subscriptions.push(vscode.commands.registerCommand(command,async()=>{
      const session=active();if(!session)throw new Error("Select a live UWVM debug session");
      const reply=await session.customRequest("uwvm/stepLevel",{level});
      if(reply?.level!==level)throw new Error("The adapter did not acknowledge the selected instruction level.");
      levels.set(session.id,level);publish();
      if(active()!==session)return;
      if(level==="wasm" && wasmView)await wasmView.open();
      if(level==="native")await vscode.commands.executeCommand("debug.action.openDisassemblyView");
    }));
  const switchLevel=async()=>{
    const session=active();if(!session)throw new Error("Select a live UWVM debug session");
    const current=await session.customRequest("uwvm/stepLevel",{});
    const choice=await vscode.window.showQuickPick([
      {label:"S · Source",description:"Source statements · F10 over, F11 into",level:"source",command:"uwvm.levelSource"},
      {label:"W · Wasm",description:"Wasm instructions · F10 over calls, F11 into calls",level:"wasm",command:"uwvm.levelWasm"},
      {label:"A · Assembly",description:"Owned JIT instructions only · F10 over calls, F11 into calls",level:"native",command:"uwvm.levelAsm"}
    ].map(row=>({...row,picked:row.level===current.level})),{title:"Instruction level · "+session.name,placeHolder:"Choose the level used by the standard debug toolbar"});
    if(choice && active()===session)await vscode.commands.executeCommand(choice.command);
  };
  for(const suffix of ["Source","Wasm","Asm"])context.subscriptions.push(vscode.commands.registerCommand("uwvm.switchFrom"+suffix,switchLevel));
  if(inspection){
    context.subscriptions.push(vscode.commands.registerCommand("uwvm.openWasiCalls",()=>inspection.open("wasip1",{wasiGroup:"calls"})));
    context.subscriptions.push(vscode.commands.registerCommand("uwvm.openRosCalls",()=>inspection.open("ros",{rosGroup:"journal",backend:"ros1"})));
    context.subscriptions.push(vscode.commands.registerCommand("uwvm.openBreakpoints",()=>inspection.open("breakpoints")));
  }
  const refresh=async session=>{
    publish();if(!session || sessions.get(session.id)!==session)return;
    try{const r=await session.customRequest("uwvm/permissions",{});if(sessions.get(session.id)===session)permissions.set(session.id,r.permissions);}catch(_){permissions.delete(session.id);}
    if(["full","read-only"].includes(permissions.get(session.id)))try{
      const r=await session.customRequest("uwvm/stepLevel",{});
      if(sessions.get(session.id)===session && ["source","wasm","native"].includes(r?.level))levels.set(session.id,r.level);
    }catch(_){levels.delete(session.id);}
    publish();
  };
  if(vscode.debug.onDidChangeActiveDebugSession)context.subscriptions.push(vscode.debug.onDidChangeActiveDebugSession(refresh));
  context.subscriptions.push(vscode.debug.onDidTerminateDebugSession(s=>{permissions.delete(s.id);levels.delete(s.id);publish();}));
  if(vscode.debug.registerDebugAdapterTrackerFactory)context.subscriptions.push(vscode.debug.registerDebugAdapterTrackerFactory("uwvm-llvm-full",{
    createDebugAdapterTracker(session){return {onDidSendMessage(packet){
      if(packet.type==="event" && packet.event==="uwvm/permissions"){permissions.set(session.id,packet.body.permissions);publish();}
      if(packet.type==="event" && packet.event==="uwvm/stepLevel" && ["source","wasm","native"].includes(packet.body?.level)){
        levels.set(session.id,packet.body.level);publish();
      }
    }};}
  }));
  return {refresh,provider};
}
module.exports={register};
