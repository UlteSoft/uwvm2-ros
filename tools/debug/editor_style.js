"use strict";
function read(vscode,uri) {
  const options=vscode.workspace.getConfiguration("editor",uri);
  const fontSize=Number(options.get("fontSize",14)),line=Number(options.get("lineHeight",0));
  return {fontSize:Number.isFinite(fontSize)&&fontSize>0?fontSize:14,
    fontFamily:String(options.get("fontFamily","monospace")),fontWeight:String(options.get("fontWeight","normal")),
    lineHeight:Number.isFinite(line)&&line>=0?line:0,letterSpacing:Number(options.get("letterSpacing",0))||0};
}
module.exports={read};
