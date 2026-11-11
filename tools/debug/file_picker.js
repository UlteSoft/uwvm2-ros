"use strict";

const path=require("path");
const fs=require("fs/promises");

async function localProfile(profile) {
  if(typeof profile!=="string" || !path.isAbsolute(profile) || /[\x00-\x1f]/.test(profile))
    throw new Error("Choose an absolute transport profile path first");
  const data=JSON.parse(await fs.readFile(profile,"utf8"));
  if(!data || data.format!=="uwvm-debug-project-v1" || !["local","ssh"].includes(data.transport?.kind))
    throw new Error("The transport profile does not identify a supported VM host");
  return data.transport.kind==="local";
}
async function localSession(session) {
  if(!session)throw new Error("Select a live debug session first");
  return session.configuration.transportProfile===undefined || await localProfile(session.configuration.transportProfile);
}
function requireLocal(local) {
  if(!local)throw new Error("This path belongs to the SSH VM host. Enter its absolute path directly; the IDE file picker browses the extension host.");
}
async function pick(vscode,{title,value,directory=false,save=false,filters}) {
  const defaultUri=typeof value==="string" && path.isAbsolute(value) && !/[\x00-\x1f]/.test(value)?vscode.Uri.file(value):undefined;
  const options={title,...(defaultUri?{defaultUri}:{}),...(filters?{filters}:{})};
  const selected=save?await vscode.window.showSaveDialog({...options,saveLabel:"Use path"}):
    (await vscode.window.showOpenDialog({...options,canSelectMany:false,canSelectFiles:!directory,canSelectFolders:directory,openLabel:directory?"Use directory":"Use file"}))?.[0];
  if(selected===undefined)return;
  if(selected.scheme!=="file" || typeof selected.fsPath!=="string" || !path.isAbsolute(selected.fsPath) || /[\x00-\x1f]/.test(selected.fsPath))
    throw new Error("Choose a filesystem path on the IDE extension host");
  return selected.fsPath;
}
module.exports={localProfile,localSession,requireLocal,pick};
