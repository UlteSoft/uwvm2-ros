"use strict";
(() => {
  const vscode=acquireVsCodeApi();
  let epoch=Number(document.body.dataset.viewEpoch),bytes=[],selected=[],anchor,drag,editor,pendingEdit,heldViewport,loadingMore=false;
  let wheelPixels=0,wheelRows=0,wheelRequest=0,wheelPending, wheelTimer;
  const post=message=>vscode.postMessage({...message,epoch});
  const fields=form=>Object.fromEntries(new FormData(form).entries());
  const canWrite=()=>document.body.dataset.canWrite==="true";
  const state=()=>({scroll:document.scrollingElement.scrollTop,
    selection:selected.map(byte=>byte.dataset.byte),
    expanded:[...document.querySelectorAll("details[open]")].map(node=>node.dataset.node||node.id||node.querySelector("summary")?.textContent),
    scrolls:[...document.querySelectorAll(".memory-scroll,.table-scroll,.policy-list")].map(node=>[node.scrollLeft,node.scrollTop]),
    controls:Object.fromEntries(["memory-endian","memory-rounding","memory-precision"].map(id=>[id,document.getElementById(id)?.value]))});
  function save(){vscode.setState(state());}
  function notice(message) {
    let node=document.getElementById("edit-status");
    if(!node){node=document.createElement("p");node.id="edit-status";node.className="notice";node.setAttribute("role","status");document.getElementById("edit-instructions")?.after(node);}
    node.textContent=message;
  }
  function cancelEdit(silent=true) {
    pendingEdit=undefined;
    if(!editor)return;
    editor.byte.hidden=false;editor.holder.remove();editor=undefined;
    if(!silent)notice("Edit cancelled; no bytes were written.");
  }
  function select(first,last=first,preserveText=false) {
    anchor=first;const low=Math.min(first,last),high=Math.max(first,last);selected=bytes.slice(low,high+1);
    for(const [index,byte] of bytes.entries())byte.classList.toggle("selected",index>=low&&index<=high);
    const selection=document.getElementById("memory-selection");if(!selection || !selected.length)return;
    selection.hidden=false;
    const start=BigInt(selected[0].dataset.byte).toString(16),end=BigInt(selected.at(-1).dataset.byte).toString(16);
    document.getElementById("selection-title").textContent="0x"+start+(start===end?"":"–0x"+end)+" · "+selected.length+" byte"+(selected.length===1?"":"s");
    const container=document.getElementById("memory-values"),precision=document.getElementById("memory-precision");container.replaceChildren();
    if(!precision.checkValidity()){precision.reportValidity();return;}
    const options={rounding:document.getElementById("memory-rounding").value,...(precision.value?{precision:Number(precision.value)}:{})};
    for(const row of uwvmMemoryValues.interpret(selected.map(byte=>parseInt(byte.dataset.hex,16)),document.getElementById("memory-endian").value,options)) {
      const name=document.createElement("dt"),value=document.createElement("dd");name.textContent=row.name;value.textContent=row.value;
      if(row.exact)value.title="Exact: "+row.exact;container.append(name,value);
    }
    const edit=document.getElementById("selection-edit"),form=document.getElementById("write-form");
    if(edit && form) {
      edit.hidden=selected.length===1;
      const allowed=canWrite() && selected.length<=256;
      form.elements.namedItem("writeOffset").value=selected[0].dataset.byte;
      if(!preserveText)form.elements.namedItem("hex").value=selected.map(byte=>byte.dataset.hex).join(" ");
      for(const control of form.elements)if(control.type!=="button")control.disabled=!allowed;
      form.title=allowed?"Apply one atomic write to the selected bytes":!canWrite()?"Editing requires full permission and a current stopped snapshot":"One atomic edit supports up to 256 bytes; select a smaller range";
    }
    save();
  }
  function edit(byte,draft) {
    if(!canWrite()){notice("Editing is disabled for the current permission or stop. Refresh the stopped memory view.");return;}
    cancelEdit();
    const holder=document.createElement("span"),input=document.createElement("input");holder.className="inline-byte-edit";
    input.className="byte-editor";input.maxLength=2;input.value=draft?draft.value:byte.dataset.hex;
    input.setAttribute("aria-label","Hex byte at offset "+byte.dataset.byte);input.title="Enter applies · Escape cancels";
    const apply=document.createElement("button"),cancel=document.createElement("button");
    apply.type=cancel.type="button";apply.textContent="✓";cancel.textContent="×";apply.title="Apply byte";cancel.title="Cancel edit";
    holder.append(input,apply,cancel);byte.hidden=true;byte.after(holder);editor={byte,input,holder};input.focus({preventScroll:true});
    if(draft)input.setSelectionRange(draft.start,draft.end);else input.select();
    function commit() {
      if(!canWrite()){notice("The edit is unavailable while this snapshot is being refreshed; no bytes were written.");return;}
      if(!/^[0-9a-fA-F]{2}$/.test(input.value)){input.setCustomValidity("Enter two hexadecimal digits");input.reportValidity();return;}
      const hex=input.value,address=byte.dataset.byte;cancelEdit();notice("Submitting one write at byte "+address+"…");
      wheelRows=wheelPixels=0;post({action:"write",fields:{writeOffset:address,hex}});
    }
    apply.addEventListener("click",commit);cancel.addEventListener("click",()=>cancelEdit(false));
    input.addEventListener("keydown",event=>{
      if(event.key==="Escape"){event.preventDefault();cancelEdit(false);byte.focus();}
      if(event.key==="Enter"){event.preventDefault();event.stopPropagation();commit();}
    });
    input.addEventListener("input",()=>input.setCustomValidity(""));
  }
  function restoreViewport(saved) {
    if(!saved)return;
    // Hidden retained webviews can suspend animation frames. Restore in this
    // DOM transaction: otherwise removing the old rows clamps scrollTop to
    // the temporary short document and a later refresh saves that wrong value.
    document.scrollingElement.scrollTop=saved.scroll||0;
    [...document.querySelectorAll(".memory-scroll,.table-scroll,.policy-list")].forEach((node,index)=>{
      const at=saved.scrolls?.[index];if(at){node.scrollLeft=at[0];node.scrollTop=at[1];}
    });
  }
  function setup(saved) {
    bytes=[...document.querySelectorAll("button[data-byte]")];selected=[];anchor=undefined;editor=undefined;drag=undefined;
    if(saved) {
      for(const [id,value] of Object.entries(saved.controls||{})){const node=document.getElementById(id);if(node && value!==undefined)node.value=value;}
      for(const node of document.querySelectorAll("details"))node.open=(saved.expanded||[]).includes(node.dataset.node||node.id||node.querySelector("summary")?.textContent);
      const byAddress=new Map(bytes.map((byte,index)=>[byte.dataset.byte,index]));
      const indices=(saved.selection||[]).map(address=>byAddress.get(address)).filter(index=>index!==undefined);
      if(indices.length)select(indices[0],indices.at(-1));
      restoreViewport(saved);
    }
  }
  function flushWheel() {
    if(wheelPending || !wheelRows || document.body.dataset.busy==="true")return;
    if(editor || pendingEdit || drag)return;
    const rows=wheelRows;wheelRows=0;wheelPending=++wheelRequest;
    post({action:"memory-scroll",rows,request:wheelPending});
  }
  document.addEventListener("wheel",event=>{
    if(!event.target.closest(".memory-scroll") || event.ctrlKey || event.metaKey || event.shiftKey ||
       Math.abs(event.deltaX)>Math.abs(event.deltaY) || !event.deltaY ||
       event.target.closest("input,textarea,select,[contenteditable=true]"))return;
    // Never discard an edit or steal the text field's own wheel behavior.
    if(editor || pendingEdit || drag || window.getSelection()?.toString())return;
    const row=document.querySelector(".memory tbody tr"),height=row?.getBoundingClientRect().height||24;
    const delta=event.deltaY*(event.deltaMode===1?height:event.deltaMode===2?window.innerHeight:1);
    wheelPixels+=delta;const rows=Math.trunc(wheelPixels/height);wheelPixels-=rows*height;
    wheelRows+=rows;event.preventDefault();heldViewport=undefined;
    clearTimeout(wheelTimer);wheelTimer=setTimeout(flushWheel,40);
  },{passive:false});
  window.addEventListener("message",event=>{
    const message=event.data;if(!message)return;
    if(message.action==="editorStyle") {
      const style=document.documentElement.style;
      style.setProperty("--vscode-editor-font-size",message.fontSize+"px");
      style.setProperty("--vscode-editor-font-family",message.fontFamily);
      style.setProperty("--vscode-editor-font-weight",message.fontWeight);
      const line=message.lineHeight||0;
      style.setProperty("--uwvm-code-line-height",(line===0?1.5*message.fontSize:line<8?line*message.fontSize:line)+"px");
      style.setProperty("--uwvm-code-letter-spacing",message.letterSpacing+"px");return;
    }
    if(message.action==="memory-scroll-done"){
      if(wheelPending===message.request){wheelPending=undefined;setTimeout(flushWheel,0);}return;
    }
    if(message.action!=="render" || typeof message.html!=="string")return;
    const saved=state(),active=document.activeElement,snapshot=document.body.dataset.snapshot;
    if(document.body.dataset.busy==="true" && heldViewport)Object.assign(saved,heldViewport);
    else heldViewport={scroll:saved.scroll,scrolls:saved.scrolls};
    const draft=editor?{address:editor.byte.dataset.byte,value:editor.input.value,start:editor.input.selectionStart,end:editor.input.selectionEnd}:pendingEdit;
    const focus=document.hasFocus() && active && !editor?{id:active.id,name:active.name,form:active.form?.id,start:active.selectionStart,end:active.selectionEnd}:undefined;
    const dirty=[...document.querySelectorAll("[data-dirty=true]")].map(node=>({id:node.id,name:node.name,form:node.form?.id,value:node.value}));
    const parsed=new DOMParser().parseFromString(message.html,"text/html"),body=parsed.body;
    const nextEpoch=Number(body.dataset.viewEpoch);if(!Number.isSafeInteger(nextEpoch)||nextEpoch<epoch)return;
    document.getElementById("tag-rules").textContent=parsed.getElementById("tag-rules")?.textContent||"";
    for(const script of body.querySelectorAll("script"))script.remove();
    document.body.replaceChildren(...body.childNodes);
    for(const [key,value] of Object.entries(body.dataset))document.body.dataset[key]=value;
    epoch=nextEpoch;loadingMore=false;setup(saved);
    const sameSnapshot=!!snapshot && document.body.dataset.snapshot===snapshot;
    pendingEdit=undefined;
    for(const field of dirty){if(field.name==="hex" && !sameSnapshot)continue;const node=field.id?document.getElementById(field.id):field.form?document.getElementById(field.form)?.elements.namedItem(field.name):[...document.querySelectorAll("[name]")].find(node=>node.name===field.name);if(node&&!node.disabled){node.value=field.value;node.dataset.dirty="true";}}
    if(draft) {
      const byte=bytes.find(node=>node.dataset.byte===draft.address);
      if(sameSnapshot && byte) {
        if(canWrite() && document.hasFocus())edit(byte,draft);
        else {pendingEdit=draft;notice("Uncommitted byte edit retained while this snapshot is busy; nothing has been written.");}
      } else notice("Uncommitted byte edit discarded because its snapshot or displayed range changed; nothing was written.");
    }
    if(focus){const node=focus.id?document.getElementById(focus.id):focus.form?document.getElementById(focus.form)?.elements.namedItem(focus.name):[...document.querySelectorAll("[name]")].find(node=>node.name===focus.name);
      if(node && !node.disabled){node.focus({preventScroll:true});try{node.setSelectionRange(focus.start,focus.end);}catch(_){}}}
    restoreViewport(saved);save();if(!wheelPending)setTimeout(flushWheel,0);
  });
  document.addEventListener("input",event=>{if(event.target.matches("input,textarea"))event.target.dataset.dirty="true";});
  document.addEventListener("submit",event=>{
    const form=event.target;if(!form.matches("form"))return;event.preventDefault();
    const actions={selector:"refresh","write-form":"write","payload-form":"payload","export-form":"export","dump-form":"dump","breakpoint-create":"breakpoint-create"};
    const action=form.dataset.action||actions[form.id];if(!action)return;
    if(form.id==="write-form")notice("Submitting the selected bytes…");
    for(const node of form.querySelectorAll("[data-dirty]"))delete node.dataset.dirty;
    save();post({action,fields:fields(form),id:form.dataset.breakpointId});
  });
  document.addEventListener("input",event=>{
    if(event.target.id!=="call-filter")return;
    const search=event.target.value.toLocaleLowerCase();
    for(const row of document.querySelectorAll("[data-call]"))row.hidden=!row.textContent.toLocaleLowerCase().includes(search);
  });
  document.addEventListener("change",event=>{
    const node=event.target;
    if(node.id==="memtag-toggle"){const enabled=node.checked;node.disabled=true;post({action:"toggle-tags",enabled});}
    else if(node.dataset.groupAction)post({action:node.dataset.groupAction,group:node.value});
    else if(["memory-endian","memory-rounding","memory-precision"].includes(node.id) && selected.length)select(bytes.indexOf(selected[0]),bytes.indexOf(selected.at(-1)),true);
    else if(["rows","columns","count"].includes(node.name) && node.form?.id==="selector") {
      const f=node.form;
      try {
        const columns=BigInt(f.elements.namedItem("columns").value),rows=BigInt(f.elements.namedItem("rows").value);
        if(columns>0n && rows>0n){if(node.name==="count")f.elements.namedItem("rows").value=((BigInt(node.value)+columns-1n)/columns).toString();else f.elements.namedItem("count").value=(rows*columns).toString();}
      } catch(_){ /* Native form validity and host exact-integer validation report the error. */ }
    }
  });
  document.addEventListener("pointerdown",event=>{
    const byte=event.target.closest("button[data-byte]");if(!byte || event.button!==0)return;
    event.preventDefault();cancelEdit();const index=bytes.indexOf(byte);
    drag={start:event.shiftKey&&anchor!==undefined?anchor:index,last:index,moved:event.shiftKey};select(drag.start,index);
  });
  document.addEventListener("pointerover",event=>{if(!drag)return;const byte=event.target.closest("button[data-byte]");if(!byte)return;const index=bytes.indexOf(byte);if(index!==drag.last)drag.moved=true;drag.last=index;select(drag.start,index);});
  document.addEventListener("pointerup",()=>{if(!drag)return;const current=drag;drag=undefined;if(!current.moved && canWrite())edit(bytes[current.start]);});
  window.addEventListener("blur",()=>{drag=undefined;save();});
  document.addEventListener("keydown",event=>{
    const byte=event.target.closest("button[data-byte]");if(!byte)return;const index=bytes.indexOf(byte);
    if(event.shiftKey && ["ArrowLeft","ArrowRight"].includes(event.key)){event.preventDefault();const next=Math.max(0,Math.min(bytes.length-1,index+(event.key==="ArrowRight"?1:-1)));select(anchor===undefined?index:anchor,next);bytes[next].focus();}
  });
  document.addEventListener("click",event=>{
    const byte=event.target.closest("button[data-byte]");if(byte){event.preventDefault();if(event.detail===0){select(bytes.indexOf(byte));edit(byte);}return;}
    const button=event.target.closest("button[data-action]");if(!button || button.disabled)return;
    if(button.dataset.action==="cancel-edit"){cancelEdit(false);document.getElementById("selection-edit").hidden=true;return;}
    if(button.dataset.action==="browse-export")post({action:"browse-export",fields:fields(document.getElementById("export-form"))});
    else if(button.dataset.action==="browse-dump")post({action:"browse-dump",fields:fields(document.getElementById("dump-form"))});
    else {save();post({action:button.dataset.action,kind:button.dataset.kind,group:button.dataset.group,id:button.dataset.breakpointId,nextState:button.dataset.nextState,call:button.dataset.callId});}
  });
  for(const name of ["wheel","touchmove"])document.addEventListener(name,()=>{heldViewport=undefined;},{passive:true});
  document.addEventListener("keydown",event=>{if(["PageDown","PageUp","Home","End"].includes(event.key))heldViewport=undefined;});
  let saveScheduled=false;
  document.addEventListener("scroll",()=>{
    if(saveScheduled)return;saveScheduled=true;requestAnimationFrame(()=>{saveScheduled=false;save();
      const more=document.getElementById("memory-load-more");
      if(more && !loadingMore && !more.querySelector("button").disabled && more.getBoundingClientRect().top<window.innerHeight+160){loadingMore=true;post({action:"load-more"});}
    });
  },true);
  setup(vscode.getState());post({action:"ready"});
})();
