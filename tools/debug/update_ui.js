"use strict";

const path=require("path");
const crypto=require("crypto");
const {spawn}=require("child_process");
const filePicker=require("./file_picker");
const escape=value=>String(value===undefined?"":value).replace(/[&<>"']/g,c=>({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[c]));

function absolute(value,label) {
  if(typeof value!=="string" || !path.isAbsolute(value) || /[\x00-\x1f]/.test(value))throw new Error(label+" must be an absolute path without control characters");
  return value;
}
function result(value) {
  if(!value || typeof value!=="object" || Array.isArray(value) || typeof value.status!=="string" ||
      typeof value.replyHex!=="string" || !/^(?:[0-9a-f]{2}){48,}$/.test(value.replyHex))
    throw new Error("The update transport returned an invalid status response; the outcome may be unknown. Inspect the deployment coordinator before any further decision.");
  return value;
}
function cli(python,args) {
  return new Promise((resolve,reject)=>{
    const child=spawn(python,args,{shell:false,windowsHide:true,stdio:["ignore","pipe","pipe"]});
    const chunks=[],errors=[];let bytes=0,exceeded=false;
    child.stdout.on("data",data=>{bytes+=data.length;if(bytes>8*1024*1024){exceeded=true;child.kill();}else chunks.push(data);});
    child.stderr.on("data",data=>{if(errors.reduce((sum,part)=>sum+part.length,0)<65536)errors.push(data);});
    child.on("error",reject);
    child.on("close",(code,signal)=>{
      if(exceeded)return reject(new Error("Update reply exceeded the display limit. The outcome may be unknown; do not retry a deployment decision blindly."));
      const stderr=Buffer.concat(errors).toString("utf8").trim();
      if(code!==0)return reject(new Error((stderr||"Update transport exited "+String(code)+ (signal?" ("+signal+")":""))+". No request was retried."));
      try{resolve(result(JSON.parse(Buffer.concat(chunks).toString("utf8"))));}catch(error){reject(error);}
    });
  });
}
function createController(vscode,context,sessions) {
  let model,nextPanelId=0;
  const history=require("./navigation_history").get(sessions);
  function active(){const value=vscode.debug.activeDebugSession;return value && value.type==="uwvm-llvm-full" && sessions.get(value.id)===value?value:undefined;}
  function view(model){
    return {kind:"update",windowId:model.id,epoch:model.epoch,busy:!!model.busy,picking:!!model.picking,mode:model.mode,
      fields:{pythonPath:model.pythonPath,socketDir:model.socketDir,profilePath:model.profilePath,framePath:model.framePath},
      sessionName:(active()||{}).name,sessionHasUpdate:!!(active()&&active().configuration.updateSocketDir),
      error:model.error||"",reply:model.reply,platform:process.platform};
  }
  function render(target=model){
    if(!target || target.disposed)return;
    const model=target,webview=model.panel.webview;
    model.epoch=(model.epoch||0)+1;
    const state=view(model);
    if(!model.initialized){
      model.initialized=true;
      const nonce=crypto.randomBytes(18).toString("hex");
      const asset=(name,folder="media")=>webview.asWebviewUri(vscode.Uri.file(path.join(context.extensionPath,folder,name))).toString();
      webview.html='<!doctype html><html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">'+
        '<meta http-equiv="Content-Security-Policy" content="default-src \'none\'; style-src '+escape(webview.cspSource)+'; script-src \'nonce-'+nonce+'\';">'+
        '<link rel="stylesheet" href="'+escape(asset("ui.css"))+'"><link rel="stylesheet" href="'+escape(asset("inspector.css"))+'"></head>'+
        '<body data-initial="'+escape(JSON.stringify(state))+'"><div id="app"></div>'+
        '<script nonce="'+nonce+'" src="'+escape(asset("icons.js"))+'"></script><script nonce="'+nonce+'" src="'+escape(asset("ui.js"))+'"></script>'+
        '<script nonce="'+nonce+'" src="'+escape(asset("update.js"))+'"></script></body></html>';
    } else if(model.ready)void webview.postMessage({action:"view",view:state});
  }
  function readFields(fields,model) {
    if(!fields || typeof fields!=="object" || Array.isArray(fields) || !["local","profile","session"].includes(fields.mode))throw new Error("Select an update transport");
    for(const key of ["mode","pythonPath","socketDir","profilePath","framePath"]) {
      if(fields[key]!==undefined && typeof fields[key]!=="string")throw new Error("Invalid update field: "+key);
      model[key]=fields[key]||"";
    }
  }
  async function browse(field,fields,model) {
    if(model.busy || model.picking || model.disposed)return;
    const names={pythonPath:"Python executable",socketDir:"Update socket directory",profilePath:"Transport profile",framePath:"Frame file on VM host"};
    if(!Object.hasOwn(names,field))throw new Error("Unknown path field");
    readFields(fields,model);
    if(field==="socketDir" && model.mode!=="local" || field==="profilePath" && model.mode!=="profile" || field==="pythonPath" && model.mode==="session")throw new Error("This path is not used by the selected transport");
    const session=active();model.picking=true;model.error="";render(model);
    try {
      if(field==="framePath")filePicker.requireLocal(model.mode==="local" || (model.mode==="profile"?await filePicker.localProfile(model.profilePath):await filePicker.localSession(session)));
      if(model.disposed)return;
      const value=await filePicker.pick(vscode,{title:names[field],value:model[field],directory:field==="socketDir",...(field==="profilePath"?{filters:{"JSON profiles":["json"]}}:{})});
      if(model.disposed || value===undefined)return;
      if(field==="framePath" && model.mode==="session" && active()!==session)throw new Error("The active debug session changed; select the VM-host file again");
      model[field]=value;
    } finally {model.picking=false;render(model);}
  }
  async function submit(fields,target=model){
    const model=target;
    if(model.busy || model.picking || model.disposed)return;
    if(!vscode.workspace.isTrusted)throw new Error("Trust this workspace before submitting a host-authorized update request");
    readFields(fields,model);
    const frame=absolute(model.framePath,"VM-host frame path");
    let request;
    if(model.mode==="session"){
      const session=active();
      if(!session || !session.configuration.updateSocketDir)throw new Error("The active debug session has no independent updateSocketDir. Choose an independent host or imported profile transport.");
      request=()=>session.customRequest("uwvm/signedUpdate",{framePath:frame});
    } else {
      const python=absolute(model.pythonPath,"Local Python executable");
      if(model.mode==="profile")request=()=>cli(python,[path.join(context.extensionPath,"import_ide.py"),"update","--profile",absolute(model.profilePath,"Transport profile"),"--frame",frame]);
      else {
        if(process.platform!=="linux")throw new Error("The independent host relay currently requires Linux. Use an imported verified SSH profile for a Linux VM host.");
        request=()=>cli(python,[path.join(context.extensionPath,"signed_update.py"),"request","--socket-dir",absolute(model.socketDir,"Update socket directory"),"--frame",frame]);
      }
    }
    model.busy=true;model.reply=undefined;model.error="";render(model);
    try{model.reply=result(await request());return model.reply;}
    catch(error){model.error=String(error.message||error);}
    finally{model.busy=false;render(model);}
  }
  async function open(initial={}){
    {
      const config=vscode.workspace.getConfiguration("uwvm.update"),session=active();
      const id=++nextPanelId;
      const panel=vscode.window.createWebviewPanel("uwvm.signedUpdate."+id,"Hot Update"+(id>1?" ("+id+")":""),vscode.ViewColumn.Active,{enableScripts:true,retainContextWhenHidden:true,localResourceRoots:[vscode.Uri.file(path.join(context.extensionPath,"media"))]});
      if(vscode.ThemeIcon)panel.iconPath=new vscode.ThemeIcon("cloud-upload");
      model={panel,id,mode:session&&session.configuration.updateSocketDir?"session":"local",pythonPath:config.get("pythonPath","") || (typeof session?.configuration.pythonPath==="string" && path.isAbsolute(session.configuration.pythonPath)?session.configuration.pythonPath:process.platform==="win32"?"":"/usr/bin/python3"),socketDir:config.get("socketDir",""),profilePath:config.get("transportProfile",""),framePath:"",busy:false};
      const owned=model;
      if(history&&session) {
        const navigation=history.register(session,async(_,valid)=>{
          if(!valid())return;
          if(owned.disposed){await open();return;}
          owned.panel.reveal();
        });
        navigation.visit({});
        context.subscriptions.push(history.subscribe(()=>{if(owned.ready&&!owned.disposed)void panel.webview.postMessage({action:"navigation",...history.state(session)});}),panel.onDidChangeViewState(()=>{if(panel.active)navigation.visit({});}));
      }
      context.subscriptions.push(panel,panel.onDidDispose(()=>{owned.disposed=true;}),panel.webview.onDidReceiveMessage(async value=>{
        if(!value)return;
        if(value.action==="navigate-back"||value.action==="navigate-forward"){
          try {await history?.move(value.action==="navigate-back"?-1:1,session);}
          catch(error){owned.error=String(error.message||error);render(owned);}return;
        }
        if(value.action==="ready"){owned.ready=true;void panel.webview.postMessage({action:"navigation",...history?.state(session)});void panel.webview.postMessage({action:"view",view:view(owned)});return;}
        if(value.action==="submit" && value.epoch!==owned.epoch){owned.error="This window changed while you were editing; review the fields and submit again.";render(owned);return;}
        if(value.action==="new-window"){await open();return;}
        try {
          if(value.action==="browse")await browse(value.field,value.fields,owned);
          else if(value.action==="submit")await submit(value.fields,owned);
        }catch(error){owned.error=String(error.message||error);render(owned);}
      }));
    }
    if(initial && typeof initial==="object" && !model.busy)for(const key of ["mode","pythonPath","socketDir","profilePath","framePath"])if(typeof initial[key]==="string")model[key]=initial[key];
    const owned=model;owned.panel.reveal();
    render(owned);return owned.panel;
  }
  context.subscriptions.push(vscode.commands.registerCommand("uwvm.submitSignedUpdate",open));
  return {open};
}
module.exports={createController};
