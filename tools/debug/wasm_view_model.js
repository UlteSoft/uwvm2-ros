"use strict";

// Cold, immutable presentation of a validated generation. No runtime handle or
// guest value is retained here; dynamic hovers are rebound to an exact stop.
const language=require("./wasm_language");
function layout(document) {
  const rows=[],scopes=[{name:"func",start:-1,end:undefined,parent:-1}],stack=[0];
  let mnemonicWidth=0,operandWidth=0,maxDepth=0;
  for(let line=1;line<document.lineCount;++line) {
    const text=document.lineAt(line).text;
    if(!text.trim() && line===document.lineCount-1)continue;
    const parsed=language.instructionLine(text);
    if(!parsed)throw new Error("The function contains an unavailable instruction display.");
    const [code,...comments]=parsed.operands.split(";"),operands=code.trim(),comment=comments.join(";").trim();
    let scope=stack.at(-1),depth=stack.length-1;
    if(parsed.name==="end" || parsed.name==="else") {
      if(scope===undefined || parsed.name==="else" && scopes[scope].name!=="if")throw new Error("Incomplete Wasm control structure.");
      depth=Math.max(0,depth-1);
      if(parsed.name==="end") {scopes[scope].end=rows.length;stack.pop();}
    }
    const row={line:line+1,offset:parsed.offset,address:parsed.offsetText,name:parsed.name,operands,comment,
      depth,scope,family:parsed.opcode.family,brief:parsed.opcode.brief||"",encoding:parsed.opcode.encoding,tokens:[]};
    let cursor=text.indexOf(code,parsed.end),ordinal=0;
    // Retain exact operand spans and attach semantics only to decoded tokens.
    for(const token of operands.matchAll(/\S+/g)) {
      const start=text.indexOf(token[0],cursor),meaning=language.hoverToken(text,start,parsed);
      row.tokens.push({text:token[0],...(meaning?{meaning}:{}),ordinal:ordinal++});cursor=start+token[0].length;
    }
    rows.push(row);mnemonicWidth=Math.max(mnemonicWidth,row.name.length);operandWidth=Math.max(operandWidth,operands.length);maxDepth=Math.max(maxDepth,depth);
    if(["block","loop","if","try_table"].includes(parsed.name)) {
      if(scope===undefined)throw new Error("Instructions follow the final function end.");
      const id=scopes.length;scopes.push({name:parsed.name,start:rows.length-1,end:undefined,parent:scope});stack.push(id);
    }
    if(!stack.length && line<document.lineCount-2)throw new Error("Instructions follow the final function end.");
  }
  if(stack.length || !rows.length)throw new Error("The complete function end is unavailable.");
  return {rows,scopes,mnemonicWidth,operandWidth,maxDepth};
}
function branchTarget(model,row,token) {
  if(!["br","br_if","br_on_null","br_on_non_null","br_table"].includes(row.name) || !/^[0-9]+$/.test(token.text))return;
  const depth=BigInt(token.text);if(depth>BigInt(model.scopes.length))return;
  let id=row.scope;
  for(let n=0n;n<depth && id>=0;++n)id=model.scopes[id].parent;
  if(id<0)return;
  const scope=model.scopes[id],target=model.rows[scope.name==="loop"?scope.start:scope.end];
  if(!target)return;
  return {title:scope.name==="loop"?"Loop back-edge":"Exit "+scope.name,
    description:"Depth "+token.text+" targets "+target.address+" ("+target.name+")."+
      (row.name==="br_if"?" Taken when the popped i32 condition is nonzero.":""),line:target.line};
}
function literal(token,row) {
  if(!/^(i32|i64)\.const$/.test(row.name) || !/^-?[0-9]+$/.test(token.text))return;
  const bits=row.name.startsWith("i32")?32:64,value=BigInt(token.text),unsigned=BigInt.asUintN(bits,value);
  return {title:bits+"-bit integer constant",description:"Signed: "+BigInt.asIntN(bits,value)+"\nUnsigned: "+unsigned+"\nBits: 0x"+unsigned.toString(16).padStart(bits/4,"0")};
}
module.exports={layout,branchTarget,literal};
