"use strict";

const {Buffer} = require("buffer");
const {ROUNDING} = require("./memory_values");
const PERMISSIONS = ["full", "read-only", "limited", "none"];
const KINDS = {memory:"Memory", registers:"Registers", wasip1:"WASIp1", ros:"ROS", diagnostics:"Diagnostics & Dump", breakpoints:"Breakpoints"};
const WASI_GROUPS = {args:"Arguments", env:"Environment", fds:"Descriptors", preopens:"Preopens",calls:"Recorded calls"};
const ROS_GROUPS = {capabilities:"Capabilities",sessions:"Sessions", queues:"Queues", services:"Services", actions:"Actions",
  parameters:"Parameters", tf:"Transforms", graph:"Graph", stats:"Statistics",calls:"Interface statistics",trace:"Call metadata",journal:"Recorded calls"};
function escape(value) {return String(value === undefined || value === null ? "" : value).replace(/[&<>"']/g, c => ({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[c]));}
function decimal(value, label, bits=64) {
  if (typeof value === "number" && Number.isSafeInteger(value)) value=String(value);
  if (typeof value !== "string" || !/^(0|[1-9][0-9]*)$/.test(value) || value.length>20 || BigInt(value)>=(1n<<BigInt(bits)))
    throw new Error(label+" must be an unsigned "+bits+"-bit decimal integer");
  return BigInt(value).toString();
}
function page(value, max=256) {
  const text=decimal(value,"Page size",32), count=Number(text);
  if(count<1 || count>max)throw new Error("Page size must be 1.."+max);
  return count;
}
function extent(value,label) {
  const text=decimal(value,label),n=Number(text);
  if(!Number.isSafeInteger(n) || n<1)throw new Error(label+" must be a positive exact integer");
  return n;
}
function columns(value=16) {return extent(value,"Row bytes");}
function memorySelection(input={}) {
  if(typeof input === "string") {
    const parts=input.split(" ");
    if(parts.length!==4)throw new Error("Use decimal MODULE MEMORY OFFSET COUNT");
    input={module:parts[0],memory:parts[1],offset:parts[2],count:parts[3]};
  }
  if(!input || typeof input!=="object" || Array.isArray(input))throw new Error("Memory selection must contain module, memory, offset and count");
  const selected={module:decimal(input.module===undefined?"0":input.module,"Module",32),
    memory:decimal(input.memory===undefined?"0":input.memory,"Memory",32),
    offset:decimal(input.offset===undefined?"0":input.offset,"Offset"),count:extent(input.count===undefined?64:input.count,"Range bytes")};
  if(BigInt(selected.offset)+BigInt(selected.count)>=(1n<<64n))throw new Error("Memory page exceeds the unsigned 64-bit address range");
  return selected;
}
function decodeMemory(reply, selected) {
  const reference=`wasm-memory:${selected.module}:${selected.memory}:${selected.offset}`;
  if(!reply || reply.memoryReference!==reference || reply.offset!==selected.offset || reply.count!==selected.count ||
      !/^[1-9][0-9]{0,19}$/.test(reply.stopId) || BigInt(reply.stopId)>=(1n<<64n) ||
      !["available","disabled","unsupported-memory"].includes(reply.status) || !Array.isArray(reply.tags) || typeof reply.data!=="string")
    throw new Error("Invalid memory/tag snapshot from the debugger");
  const bytes=Buffer.from(reply.data,"base64");
  if(bytes.length!==selected.count || bytes.toString("base64")!==reply.data)throw new Error("Incomplete memory bytes from the debugger");
  const start=BigInt(selected.offset), granules=selected.count?Number((start+BigInt(selected.count)-1n)/16n-start/16n+1n):0;
  if(reply.tags.length!==(reply.status==="available"?granules:0))throw new Error("Incomplete allocation tag coverage");
  const tags=reply.tags.map((row,index)=>{
    const first=(start/16n+BigInt(index))*16n;
    if(!row || row.offset!==first.toString() || !Number.isInteger(row.tag) || row.tag<0 || row.tag>255 ||
       row.hex!=="0x"+row.tag.toString(16).padStart(2,"0") || !/^#[0-9a-f]{6}$/.test(row.color))throw new Error("Invalid allocation tag row");
    return {offset:row.offset,tag:row.tag,hex:row.hex,color:row.color};
  });
  return {...reply,bytes:Array.from(bytes),tags};
}
function editBytes(value) {
  if(typeof value!=="string" || value.length>2048)throw new Error("Enter 1..256 bytes as hexadecimal pairs");
  const compact=value.replace(/\s/g,"");
  if(!/^(?:[0-9a-fA-F]{2}){1,256}$/.test(compact))throw new Error("Enter 1..256 complete hexadecimal byte pairs");
  return Buffer.from(compact,"hex");
}
function reason(permission, connected=true) {
  if(permission==="none")return "Debugger disabled (none).";
  if(!connected)return "No active UWVM debug session.";
  if(permission==="limited")return "Limited: diagnostics only. State inspection is not permitted.";
  if(!PERMISSIONS.includes(permission))return "Checking permissions…";
  return "";
}
function textValue(value) {
  if(value===null)return "null";
  if(value===undefined)return "not provided";
  if(typeof value==="object") {
    if(typeof value.text==="string")return value.text+(value.truncated?" [truncated]":"");
    return Array.isArray(value)?"Array ("+value.length+")":"Object ("+Object.keys(value).length+")";
  }
  return String(value);
}
function structure(value,key="value") {
  if(value===null || value===undefined || typeof value!=="object") {
    const kind=value===null?"null":value===undefined?"missing":typeof value;
    const displayed=typeof value==="string" && value.length===0?'""':textValue(value);
    if(typeof value==="string" && value.length>160)
      return '<details class="structured" data-node="'+escape(key)+'"><summary>Text · '+value.length+' characters · '+escape(value.slice(0,120))+'…</summary><span class="value-string">'+escape(value)+'</span></details>';
    return '<span class="value-'+kind+'">'+escape(displayed)+'</span>';
  }
  const entries=Array.isArray(value)?value.map((item,index)=>[String(index),item]):Object.entries(value);
  return '<details class="structured" data-node="'+escape(key)+'"><summary>'+escape((Array.isArray(value)?'Array':'Object')+' ('+entries.length+')')+'</summary>'+
    (entries.length?'<dl>'+entries.map(([name,item])=>'<dt>'+escape(name)+'</dt><dd>'+structure(item,key+"/"+name)+'</dd>').join("")+'</dl>':'<span class="empty">Empty '+(Array.isArray(value)?"array":"object")+'</span>')+'</details>';
}
function table(rows) {
  if(!Array.isArray(rows)||!rows.length)return '<p class="empty">No records.</p>';
  const internal=new Set(["variablesReference","presentationHint","memoryReference","evaluateName","indexedVariables","namedVariables"]);
  const columns=Array.from(new Set(rows.flatMap(row=>row && typeof row==="object"?Object.keys(row):["value"]))).filter(key=>!internal.has(key));
  return '<div class="table-scroll"><table><thead><tr>'+columns.map(key=>'<th>'+escape(key)+'</th>').join("")+'</tr></thead><tbody>'+rows.map((row,index)=>
    '<tr>'+columns.map(key=>{const value=row && typeof row==="object"?row[key]:row;
      return '<td>'+structure(value,"row/"+index+"/"+key)+'</td>';}).join("")+'</tr>').join("")+'</tbody></table></div>';
}
function input(name,label,value,extra="") {return '<label>'+escape(label)+'<input name="'+name+'" value="'+escape(value)+'" '+extra+'></label>';}
function policyRows(rows) {
  if(!Array.isArray(rows)||!rows.length)return '<p class="empty">No policy records.</p>';
  return '<div class="policy-list">'+rows.map(row=>{
    if(!row || typeof row!=="object" || !Object.hasOwn(row,"key") || !Object.hasOwn(row,"value"))return table([row]);
    const context=[row.kind,row.backend,row.scope,row.target,row.module,row.alias].filter(v=>v!==undefined&&v!==null).map(textValue).join(" · ");
    const flags=[row.configured!==undefined?'Configured '+textValue(row.configured):null,row.enabled!==undefined?'Enabled '+textValue(row.enabled):null,
      row.redacted!==undefined?'Redacted '+textValue(row.redacted):null].filter(Boolean);
    if(row.value_offset!==undefined && row.value_total!==undefined) {
      const raw=typeof row.valueBytesBase64==="string"?row.valueBytesBase64:row.value && typeof row.value==="object"?row.value.bytesBase64:undefined;
      const length=typeof raw==="string"?Buffer.from(raw,"base64").length:Buffer.byteLength(textValue(row.value));
      flags.push('Fragment bytes '+textValue(row.value_offset)+' + '+length+' of '+textValue(row.value_total));
    }
    const metadata=Object.fromEntries(Object.entries(row).filter(([key])=>!["key","value"].includes(key)));
    return '<article class="policy-item">'+(context?'<p class="muted">'+escape(context)+'</p>':"")+
      '<dl><dt>'+escape(textValue(row.key))+'</dt><dd'+(typeof row.valueBytesBase64==="string"?' title="Exact value bytes (base64): '+escape(row.valueBytesBase64)+'"':'')+'>'+structure(row.value,'policy/'+textValue(row.key))+'</dd></dl>'+
      (flags.length?'<p class="muted">'+escape(flags.join(" · "))+'</p>':"")+
      (Object.keys(metadata).length?'<details><summary>Details</summary>'+table([metadata])+'</details>':"")+'</article>';
  }).join("")+'</div>';
}
function button(action,label,disabled=false,extra="") {return '<button data-action="'+action+'" '+(disabled?'disabled ':'')+extra+'>'+escape(label)+'</button>';}
function callRecords(rows) {
  if(!rows.length)return '<p class="empty">No recorded calls in this page.</p>';
  return '<label>Filter this page<input type="search" id="call-filter" placeholder="Function, result or module"></label><div id="call-records">'+rows.map(row=>
    '<details class="card" data-call="'+escape(row.id)+'" data-node="call/'+escape(row.id)+'"><summary>#'+escape(row.id)+' · '+escape(textValue(row.namespace))+'.'+escape(textValue(row.function))+' · result '+escape(row.result)+'</summary>'+
    structure(row,'call/'+row.id+'/details')+button('call-payload','Inspect payload',false,'data-call-id="'+escape(row.id)+'"')+'</details>').join('')+'</div>';
}
function group(title,data,actions="",policy=false) {
  if(!data)return '<section class="card"><h2>'+escape(title)+'</h2><p class="empty">Refresh to load.</p></section>';
  if(data.error)return '<section class="card"><h2>'+escape(title)+'</h2><p class="error">'+escape(data.error)+'</p></section>';
  const rows=data.rows||data.variables||[];
  const metadata=[data.status,data.total!==undefined?"Total "+data.total:null,data.more?"More records":null].filter(x=>x!==null&&x!==undefined).join(" · ");
  return '<section class="card"><h2>'+escape(title)+'</h2><p class="muted">'+escape(metadata)+'</p>'+
    (data.available===false?'<p class="notice">Unavailable: '+escape(data.status||"runtime state is not available")+'</p>':policy?policyRows(rows):data.journal?callRecords(rows):table(rows))+actions+'</section>';
}
function groupSelect(action,groups,selected,disabled) {
  return '<label class="group-select">View<select data-group-action="'+action+'" '+(disabled?'disabled':'')+'>'+Object.entries(groups).map(([key,title])=>
    '<option value="'+key+'" '+(selected===key?'selected':'')+'>'+escape(title)+'</option>').join("")+'</select></label>';
}
function render(model,assets) {
  const connected=!!model.sessionId, inspect=["full","read-only"].includes(model.permission), snapshot=model.data;
  const denied=model.kind==="diagnostics" && connected && model.permission==="limited"?"":reason(model.permission,connected), blocked=!!denied, canWrite=model.permission==="full" && snapshot && !model.retired && !model.busy;
  const selector=model.selector||memorySelection(),rowBytes=columns(model.columns);
  const selectionForm=(model.kind==="memory")?
    input("module","Module",selector.module,'inputmode="numeric"')+input("memory","Memory",selector.memory,'inputmode="numeric"')+
    input("offset","Byte offset",selector.offset,'inputmode="numeric"')+input("count","Range bytes",selector.count,'type="number" min="1"')+
    input("rows","Rows",Math.ceil(selector.count/rowBytes),'type="number" min="1"')+
    input("columns","Bytes per row",rowBytes,'id="memory-columns" type="number" min="1"'):
    model.kind==="wasip1"?input("module","Module",model.module||"0",'inputmode="numeric"'):
    model.kind==="ros"?'<label>Backend<select name="backend">'+["all","ros1","ros2"].map(key=>'<option value="'+key+'" '+(model.backend===key?'selected':'')+'>'+key+'</option>').join("")+'</select></label>'+
      '<label>Scope<select name="policyScope">'+["all","global","single","group"].map(key=>'<option value="'+key+'" '+((model.policyScope||"all")===key?'selected':'')+'>'+key+'</option>').join("")+'</select></label>'+
      input("policyTarget","Target",model.policyTarget||"")+input("policyAlias","Alias",model.policyAlias||""):"";
  let body="",colors=[];
  if(model.kind==="memory") {
    if(snapshot) {
      const first=BigInt(selector.offset), tags=new Map((model.showTags?snapshot.tags:[]).map(t=>[t.offset,t]));
      colors=model.showTags?snapshot.tags.map(t=>t.color):[];
      body='<section class="card memory-card"><div class="memory-scroll"><table class="memory"><thead><tr><th>Offset</th><th>Hex</th><th>Characters</th>'+(model.showTags?'<th>Allocation tags</th>':'')+'</tr></thead><tbody>';
      for(let index=0;index<snapshot.bytes.length;index+=rowBytes) {
        const slice=snapshot.bytes.slice(index,index+rowBytes), offset=first+BigInt(index);
        const cells=slice.map((value,n)=>{const address=offset+BigInt(n),tag=tags.get((address/16n*16n).toString());
          const tooltip='Byte 0x'+value.toString(16).padStart(2,"0")+' · offset '+address+(tag?' · Tag '+tag.hex+' · granule 0x'+BigInt(tag.offset).toString(16):'');
          const cls=model.showTags&&tag?' tag-'+tag.color.slice(1):'';
          return '<button class="byte'+cls+'" type="button" data-byte="'+address+'" data-hex="'+value.toString(16).padStart(2,"0")+'" title="'+escape(tooltip)+'">'+value.toString(16).padStart(2,"0")+'</button>';}).join("");
        const ascii=slice.map(n=>n>=32&&n<127?String.fromCharCode(n):"·").join("");
        const rowTags=[];
        for(let address=offset/16n*16n;address<offset+BigInt(slice.length);address+=16n) {
          const tag=tags.get(address.toString());
          if(tag)rowTags.push('<span class="tag-chip tag-'+tag.color.slice(1)+'" title="16-byte allocation granule">'+escape(tag.hex)+' · 0x'+address.toString(16)+'–0x'+(address+15n).toString(16)+'</span>');
        }
        body+='<tr><th><code>0x'+offset.toString(16).padStart(snapshot.addressBits===64 || snapshot.tagBits===8?16:8,"0")+'</code></th><td class="bytes">'+cells+'</td><td class="ascii">'+escape(ascii)+'</td>'+(model.showTags?'<td class="row-tags">'+rowTags.join("")+'</td>':'')+'</tr>';
      }
      body+='</tbody></table></div>';
      body+='<p class="muted">Loaded '+snapshot.bytes.length+' of '+(snapshot.rangeCount===undefined?snapshot.bytes.length:snapshot.rangeCount)+' bytes in this range.</p>'+
        (snapshot.bytes.length<snapshot.rangeCount?'<div id="memory-load-more">'+button("load-more","Load more rows",model.busy)+'</div>':'')+
        '<div class="pager">'+button("previous","Previous range",model.busy||BigInt(selector.offset)===0n)+button("next","Next range",model.busy||!snapshot.nextMemoryReference)+'</div></section>';
    }
    body+='<section class="card" id="memory-selection" hidden><div class="section-heading"><h2 id="selection-title">Selection</h2><label>Byte order<select id="memory-endian"><option value="little">Little-endian</option><option value="big">Big-endian (.be)</option><option value="pdp">PDP (.pdp)</option></select></label><label>Rounding<select id="memory-rounding">'+ROUNDING.map(mode=>'<option value="'+mode+'">'+mode.replaceAll("_"," ")+'</option>').join("")+'</select></label><label>Digits<input id="memory-precision" type="number" min="1" max="36" placeholder="Auto"></label></div><dl id="memory-values"></dl></section>';
    body+='<section class="card selection-edit" id="selection-edit" hidden><h2>Edit selected bytes</h2>'+ 
      '<form id="write-form">'+input("writeOffset","Byte offset",model.writeOffset||selector.offset,'readonly '+(!canWrite?'disabled':''))+
      '<label class="hex-input">Hex bytes<textarea name="hex" rows="2" placeholder="41 42 43 00" '+(!canWrite?'disabled':'')+'>'+escape(model.hex||"")+'</textarea></label>'+
      '<button type="submit" '+(!canWrite?'disabled':'')+'>Apply</button><button type="button" data-action="cancel-edit">Cancel</button></form></section>'+
      '<p class="muted" id="edit-instructions">'+escape(canWrite?'Select bytes to inspect; click a byte or press Enter to edit. Enter applies, Escape cancels.':
        model.permission!=="full"?'Editing disabled: full debugger permission is required.':'Editing disabled: refresh a current stopped snapshot.')+'</p>';
  } else if(model.kind==="breakpoints") {
    body='<section class="card"><h2>All breakpoint sources</h2><p class="muted">Console and editor breakpoints share the VM inventory. Instruction offsets are relative to their Wasm or JIT function.</p>';
    if(snapshot && Array.isArray(snapshot.rows))body+=snapshot.rows.length?snapshot.rows.map(row=>
      '<details class="card" data-node="breakpoint/'+escape(row.id)+'"><summary>#'+escape(row.id)+' · '+escape(row.kind)+' · func '+escape(row.function)+' · '+(row.enabled?'Enabled':'Disabled')+'</summary>'+
      structure(row,'breakpoint/'+row.id+'/details')+'<div class="toolbar">'+
      button('breakpoint-toggle',row.enabled?'Disable':'Enable',!canWrite,'data-breakpoint-id="'+escape(row.id)+'" data-next-state="'+(row.enabled?'disable':'enable')+'"')+
      button('breakpoint-delete','Delete',!canWrite,'data-breakpoint-id="'+escape(row.id)+'"')+
      button('breakpoint-callbacks','View callbacks',model.busy||model.retired,'data-breakpoint-id="'+escape(row.id)+'"')+'</div>'+
      '<form id="condition-'+escape(row.id)+'" data-action="breakpoint-condition" data-breakpoint-id="'+escape(row.id)+'" class="toolbar">'+
      input('value','Condition (empty clears)',row.condition||'','maxlength="256" '+(!canWrite?'disabled':''))+'<button '+(!canWrite?'disabled':'')+'>Set condition</button></form>'+
      '<form id="ignore-'+escape(row.id)+'" data-action="breakpoint-ignore" data-breakpoint-id="'+escape(row.id)+'" class="toolbar">'+
      input('value','Skip next hits',row.ignore,'inputmode="numeric" '+(!canWrite?'disabled':''))+'<button '+(!canWrite?'disabled':'')+'>Set count</button></form>'+
      '<details data-node="breakpoint/'+escape(row.id)+'/callbacks"><summary>Callback actions</summary>'+
      (model.callbackPages?.[row.id]?structure(model.callbackPages[row.id],'breakpoint/'+row.id+'/callback-page'):'<p class="muted">Use View callbacks to read the installed actions.</p>')+
      (model.callbackPages?.[row.id]?.more?button('breakpoint-callback-next','Next callback page',model.busy||model.retired,'data-breakpoint-id="'+escape(row.id)+'"'):'')+
      '<form id="callbacks-'+escape(row.id)+'" data-action="breakpoint-set-callbacks" data-breakpoint-id="'+escape(row.id)+'">'+
      '<label>Action list<textarea name="value" rows="3" '+(!canWrite?'disabled':'')+' placeholder="echo reached; continue"></textarea></label>'+
      '<label>Change<select name="mode" '+(!canWrite?'disabled':'')+'><option value="replace">Replace actions</option><option value="append">Append actions</option><option value="clear">Clear actions</option></select></label>'+
      '<p class="muted">Semicolon-separated debugger actions; no shell or host function calls. The runtime validates the complete packet before installation. Append supports additional packets.</p>'+
      '<button '+(!canWrite?'disabled':'')+'>Apply callback actions</button></form></details></details>').join(''):'<p class="empty">No breakpoints.</p>';
    body+='</section><section class="card"><h2>Add Wasm instruction breakpoint</h2><p class="muted">Use the editor gutter for source lines and the disassembly gutter for native instructions. These coordinates select a validated Wasm instruction.</p>'+
      '<form id="breakpoint-create" class="toolbar">'+input('module','Module index','0',!canWrite?'disabled':'')+
      input('function','Function index','0',!canWrite?'disabled':'')+input('offset','Instruction offset','0x0',!canWrite?'disabled':'')+
      '<label><input type="checkbox" name="temporary" '+(!canWrite?'disabled':'')+'>Remove after first hit</label><button '+(!canWrite?'disabled':'')+'>Add breakpoint</button></form></section>';
  } else if(model.kind==="diagnostics") {
    body='<section class="card"><h2>Current runtime</h2>'+structure(snapshot||{},'diagnostics')+'</section>'+
      '<section class="card"><h2>Wasm dump</h2><p>'+escape(model.permission==="limited"?"Limited permission saves non-sensitive diagnostic information.":"Full and read-only permission can save a complete diagnostic dump.")+'</p>'+
      '<form id="dump-form" class="toolbar">'+input("dumpPath","New dump file on VM host",model.dumpPath||"")+
      '<button type="button" data-action="browse-dump" '+(model.busy||model.picking?'disabled':'')+'>Browse…</button>'+
      '<button type="submit" '+(model.busy||!model.stopId||model.permission==="none"?'disabled':'')+'>Save dump</button></form>'+
      (!model.stopId?'<p class="notice">A current stopped process is required to save a dump.</p>':"")+
      (model.dumpReply?structure(model.dumpReply,'dump-result'):"")+'</section>';
  } else if(model.kind==="registers") {
    if(snapshot)body=group("Registers",{rows:snapshot.variables,total:snapshot.variables.length})+'<p class="muted">'+escape(snapshot.frameName)+'</p>';
  } else if(model.kind==="wasip1") {
    body='<div class="split"><div>'+group("Policy",snapshot&&snapshot.policy,
      '<div class="pager">'+button("policy-previous","Previous",model.busy||!model.policyHistory||!model.policyHistory.length)+button("policy-next","Next",model.busy||!snapshot||!snapshot.policy||!snapshot.policy.more)+'</div>',true)+
      '</div><div>'+groupSelect("wasi-group",WASI_GROUPS,model.wasiGroup,model.busy||blocked)+
      group(WASI_GROUPS[model.wasiGroup],snapshot&&snapshot.environment,button("group-previous","Previous",model.busy||!model.groupHistory?.length)+button("group-next","Next",model.busy||!snapshot||!snapshot.environment||!snapshot.environment.more))+ '</div></div>';
  } else if(model.kind==="ros") {
    body='<div class="split"><div>'+group("Policy",snapshot&&snapshot.policy,
      '<div class="pager">'+button("policy-previous","Previous",model.busy||!model.policyHistory||!model.policyHistory.length)+button("policy-next","Next",model.busy||!snapshot||!snapshot.policy||!snapshot.policy.more)+'</div>',true)+
      '</div><div>'+groupSelect("ros-group",ROS_GROUPS,model.rosGroup,model.busy||blocked)+
      group(ROS_GROUPS[model.rosGroup],snapshot&&snapshot.internal,button("group-previous","Previous",model.busy||!model.groupHistory?.length)+button("group-next","Next",model.busy||!snapshot||!snapshot.internal||!snapshot.internal.more))+'</div></div>';
  }
  const colorRules=Array.from(new Set(colors)).map(c=>'.tag-'+c.slice(1)+'{background-color:'+c+'26;border-bottom-color:'+c+'}').join("\n");
  if(!blocked && snapshot && (model.kind==="wasip1" && model.wasiGroup==="calls" || model.kind==="ros" && model.rosGroup==="journal")) {
    const selector=model.payloadSelector||{call:"",segment:"0",offset:"0",count:256},disabled=model.busy;
    const family=model.kind==="wasip1"?"wasip1":model.backend;
    body+='<section class="card"><h2>Recorded payload</h2>'+
      '<form id="payload-form" class="toolbar">'+input("call","Call ID",selector.call)+input("segment","Segment",selector.segment)+input("offset","Byte offset",selector.offset)+input("count","Bytes (1–4096)",selector.count)+
      '<button type="submit" '+(disabled?'disabled':'')+'>Read</button></form>'+
      (model.payload?structure(model.payload,'recorded-payload'):"")+
      '<form id="export-form" class="toolbar">'+input("exportPath","New JSON file on VM host",model.exportPath||"")+'<button type="button" data-action="browse-export" '+(disabled||model.picking?'disabled':'')+'>Browse…</button><button type="submit" '+(disabled?'disabled':'')+'>Export JSON</button></form>'+
      (model.exportResult?structure(model.exportResult,'export-result'):"")+
      '<h2>Recording</h2>'+(model.permission!=="full"?'<p class="muted">Read-only: recording controls disabled.</p>':"")+
      '<div class="toolbar">'+button("trace-on","Start "+family,disabled||model.permission!=="full")+button("trace-off","Stop "+family,disabled||model.permission!=="full")+
      button("trace-clear","Clear all families",disabled||model.permission!=="full")+'</div></section>';
  }
  return '<!doctype html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">'+
    '<meta http-equiv="Content-Security-Policy" content="default-src \'none\'; style-src '+escape(assets.cspSource)+' \'nonce-'+assets.nonce+'\'; script-src \'nonce-'+assets.nonce+'\';">'+
    '<link rel="stylesheet" href="'+escape(assets.style)+'"><style id="tag-rules" nonce="'+assets.nonce+'">'+colorRules+'</style></head><body data-window-id="'+escape(model.id)+'" data-window-kind="'+escape(model.kind)+'" data-view-epoch="'+escape(model.epoch)+'" data-can-write="'+(canWrite?'true':'false')+'" data-busy="'+(model.busy?'true':'false')+'" data-snapshot="'+escape(!model.retired && model.stopId?[model.sessionId,model.kind,model.stopId,model.selector?.module,model.selector?.memory].join(":"):"")+'">'+
    '<header><h1>'+KINDS[model.kind]+'</h1><div class="window-actions"><span class="badge permission" title="'+escape({full:"Inspect and edit","read-only":"Inspect only",limited:"Diagnostics only",none:"Debugger disabled"}[model.permission]||"Checking permissions")+'">'+escape(model.permission||"Not connected")+'</span>'+button("new-window","New window")+'</div></header>'+
    '<div class="session-bar"><span>'+escape(model.sessionName||"No active session")+(model.stopId?' · stop '+escape(model.stopId):'')+'</span><details><summary>Session</summary><div class="toolbar">'+button("bind","Use active session")+button("diagnostics","Diagnostics")+'</div></details></div>'+
    '<form id="selector" class="toolbar">'+selectionForm+(model.kind==="memory"?'<label class="toggle"><input id="memtag-toggle" type="checkbox" '+(model.showTags && snapshot && snapshot.status==="available"?'checked ':'')+(model.busy||blocked||!snapshot?'disabled ':'')+'>Memtag</label>':"")+'<button type="submit" '+(model.busy?'disabled':'')+'>Refresh</button></form>'+
    (denied?'<p class="notice" role="status">'+escape(denied)+'</p>':"")+
    (model.retired?'<p class="notice" role="status">Snapshot expired. Refresh at a stopped guest.</p>':"")+
    (model.notice?'<p class="notice" role="status">'+escape(model.notice)+'</p>':"")+
    (model.error?'<p class="error" role="alert">'+escape(model.error)+'</p>':"")+
    (model.writeResult?'<p class="success" role="status">'+escape(model.writeResult.text)+' <span class="muted">module '+escape(model.writeResult.module)+', memory '+escape(model.writeResult.memory)+', committed stop '+escape(model.writeResult.stopId)+'</span></p>':"")+
    (model.writeError?'<p class="error" role="alert"><strong>Memory write: </strong>'+escape(model.writeError)+'</p>':"")+
    (model.message?'<p class="success" role="status">'+escape(model.message)+'</p>':"")+
    (model.busy?'<p class="muted" role="status">Reading the stopped runtime…</p>':"")+body+
    (model.diagnostics&&blocked?'<section class="card"><h2>Diagnostics</h2><pre>'+escape(model.diagnostics)+'</pre></section>':"")+
    '<script nonce="'+assets.nonce+'" src="'+escape(assets.values)+'"></script><script nonce="'+assets.nonce+'" src="'+escape(assets.script)+'"></script></body></html>';
}
module.exports={PERMISSIONS,KINDS,WASI_GROUPS,ROS_GROUPS,decimal,page,extent,columns,memorySelection,decodeMemory,editBytes,reason,render,structure};
