"use strict";
(() => {
  const api=acquireVsCodeApi(),code=document.getElementById("code"),rail=document.getElementById("rail-rows"),body=document.getElementById("code-rows"),space=document.getElementById("space"),tip=document.getElementById("hover");
  let rows=[],state={valid:false},revision=0,rowHeight=24,charWidth=8,codeInset=0,base=0,segment=1,points=new Map(),drawn=new Map(),selection=0,anchor=0,lastFollow,hoverSerial=0,hoverTimer,pointer;
  let mnemonicWidth=12,operandWidth=8,maxColumns=1,manualSaved=api.getState(),editorStyle={};
  const post=message=>api.postMessage({...message,revision});
  const element=(name,className,text)=>{const e=document.createElement(name);if(className)e.className=className;if(text!==undefined)e.textContent=text;return e;};
  const canWrite=()=>state.valid&&state.permission==="full";
  function notice(text,error=false){const n=document.getElementById("notice");n.textContent=text||"";n.classList.toggle("error",error);}
  function hideHover(){clearTimeout(hoverTimer);++hoverSerial;tip.hidden=true;pointer=undefined;}
  function save(){api.setState({top:base+code.scrollTop/rowHeight,left:code.scrollLeft,selection,anchor});}
  function measure() {
    const probe=element("span","","0000000000");probe.style.cssText="position:absolute;visibility:hidden;white-space:pre";code.append(probe);
    charWidth=probe.getBoundingClientRect().width/10||8;probe.remove();
    const font=parseFloat(getComputedStyle(code).fontSize)||14,line=editorStyle.lineHeight||0;rowHeight=Math.round(line===0?font*1.5:line<8?line*font:line);document.documentElement.style.setProperty("--row-height",rowHeight+"px");
    // Bound browser layout coordinates, never the function length. The segment
    // rolls as the user scrolls; exact offset navigation reaches every row.
    segment=Math.max(1,Math.floor(8_000_000/rowHeight));
    space.style.height=Math.min(segment,Math.max(0,rows.length-base))*rowHeight+"px";
    codeInset=charWidth;
    space.style.width=Math.max(code.clientWidth,codeInset+(maxColumns+2)*charWidth)+"px";
  }
  function setBase(first) {
    base=Math.max(0,Math.min(first,Math.max(0,rows.length-segment)));drawn.clear();body.replaceChildren();rail.replaceChildren();
    space.style.height=Math.min(segment,rows.length-base)*rowHeight+"px";
  }
  function go(index,follow=false) {
    if(!rows.length)return;index=Math.max(0,Math.min(index,rows.length-1));
    if(index<base||index>=base+segment)setBase(Math.max(0,index-Math.floor(segment/2)));
    const top=(index-base)*rowHeight,bottom=top+rowHeight;
    if(top<code.scrollTop||bottom>code.scrollTop+code.clientHeight)code.scrollTop=Math.max(0,top-code.clientHeight*.25);
    if(follow) {
      const x=codeInset+rows[index].depth*4*charWidth,visible=x-code.scrollLeft,width=code.clientWidth;
      // Safe zone is measured in the actual instruction viewport, excluding
      // the fixed address rail. Follow both growing and shrinking indentation.
      if(visible<0)code.scrollLeft=Math.max(0,x-width*.25);
      else if(visible>=width)code.scrollLeft=Math.max(0,x-width*.75);
    }
    selection=anchor=index;draw();save();
  }
  function styleRow(index,pair) {
    const row=rows[index],point=points.get(row.line),isCurrent=state.valid&&state.line===row.line;
    for(const node of [pair.text,pair.gutter]){node.classList.toggle("current",isCurrent);node.classList.toggle("selected",index>=Math.min(anchor,selection)&&index<=Math.max(anchor,selection));}
    pair.pc.setAttribute("aria-hidden",String(!isCurrent));pair.pc.title=isCurrent?"Next Wasm instruction to execute · thread "+state.threadId+" · stop "+state.stopId:"";
    const b=pair.breakpoint;b.className="breakpoint "+(!point?"empty":point.pending?"pending":point.enabled?"enabled":"disabled");b.textContent=point?.pending?"○":"●";
    b.disabled=!canWrite()||!row.safe;
    b.title=point?(point.pending?"Awaiting runtime confirmation":(point.enabled?"Enabled":"Disabled")+" breakpoint "+point.ids.map(id=>"#"+id).join(", "))+(point.condition?"\nCondition: "+point.condition:""):
      !row.safe?"Display-only opcode; no executable stop point in this generation":!canWrite()?"Breakpoint changes require full permission and a current stop":"Click to add a breakpoint; right-click for options";
    b.setAttribute("aria-label",b.title+" at "+row.address);b.setAttribute("aria-pressed",point?"true":"false");
    pair.text.setAttribute("aria-selected",String(index===selection));
  }
  function createRow(index) {
    const row=rows[index],text=element("div","row"),gutter=element("div","rail-row"),bp=element("button","breakpoint"),pc=element("span","pc"),address=element("span","address token",row.address);
    text.dataset.index=gutter.dataset.index=String(index);text.setAttribute("role","row");text.setAttribute("aria-rowindex",String(index+1));text.id="instruction-"+index;
    bp.type="button";bp.dataset.breakpoint=String(row.line);address.dataset.hover="address";address.dataset.line=String(row.line);
    // VS Code Codicons: debug-stackframe. See icons/CODICONS-NOTICE.md.
    // Static SVG avoids Unicode/font-dependent geometry and never changes on hover.
    const glyph=document.createElementNS("http://www.w3.org/2000/svg","svg"),
      outline=document.createElementNS("http://www.w3.org/2000/svg","path");
    glyph.setAttribute("viewBox","0 0 16 16");glyph.setAttribute("aria-hidden","true");
    glyph.setAttribute("focusable","false");
    outline.setAttribute("d","M8.879 14H5c-1.103 0-2-.897-2-2V4c0-1.103.897-2 2-2h3.879a2.01 2.01 0 0 1 1.414.586l4.212 4.212c.321.321.498.748.498 1.202 0 .454-.177.881-.498 1.202l-4.212 4.212A2.01 2.01 0 0 1 8.879 14ZM5 3c-.551 0-1 .449-1 1v8c0 .551.449 1 1 1h3.879c.263 0 .521-.107.707-.293l4.212-4.212a.696.696 0 0 0 0-.99L9.586 3.293A1.008 1.008 0 0 0 8.879 3H5Z");
    glyph.append(outline);pc.append(glyph);
    gutter.append(bp,pc,address);
    const guides=element("span","guides");guides.style.width=row.depth*4+"ch";guides.setAttribute("aria-hidden","true");
    const name=element("span","mnemonic token",row.name);name.dataset.hover="instruction";name.dataset.line=String(row.line);
    // Same families/scopes as generate_wasm_language.py's TextMate grammar.
    // Numeric instructions (including *.const) are support.function, NOT
    // constant.numeric. Their immediate values have their own token style.
    const category={Control:"control",Variables:"variable",Numeric:"numeric",
      Memory:"memory",GC:"gc",SIMD:"vector",References:"reference",Tables:"table",
      Threads:"atomic",MTE:"memtag"}[row.family]||"numeric";
    name.classList.add(category);
    const operands=element("span","operands");
    row.tokens.forEach((token,n)=>{if(n)operands.append(document.createTextNode(" "));const key=token.meaning?.kind==="object"?token.meaning.key:undefined;
      const category=key==="function"?"function":key==="memory"||key==="table"?"type":key==="label"?"label":key?"index":
        /^(?:i32|i64|f32|f64|v128|ref|func|extern|struct|array|any|eq|i31|externref|funcref)$/.test(token.text)?"type":"number";
      const span=element("span","token "+category,token.text);span.dataset.token=String(n);span.dataset.line=String(row.line);operands.append(span);});
    text.append(guides,name,operands);if(row.comment)text.append(element("span","comment","; "+row.comment));
    return {text,gutter,breakpoint:bp,pc};
  }
  function draw() {
    if(!rows.length)return;
    const first=Math.max(base,base+Math.floor(code.scrollTop/rowHeight)-8),last=Math.min(rows.length,base+segment,base+Math.ceil((code.scrollTop+code.clientHeight)/rowHeight)+8);
    for(const [index,pair] of drawn)if(index<first||index>=last){pair.text.remove();pair.gutter.remove();drawn.delete(index);}
    for(let i=first;i<last;++i) {
      let pair=drawn.get(i);if(!pair){pair=createRow(i);drawn.set(i,pair);body.append(pair.text);rail.append(pair.gutter);}
      pair.text.style.top=pair.gutter.style.top=(i-base)*rowHeight+"px";styleRow(i,pair);
    }
    rail.style.transform="translateY("+(-code.scrollTop)+"px)";code.setAttribute("aria-activedescendant","instruction-"+selection);
  }
  code.addEventListener("scroll",()=>{
    const absolute=base+code.scrollTop/rowHeight;
    if(rows.length>segment && (base>0&&code.scrollTop<code.clientHeight*2 || base+segment<rows.length&&code.scrollTop>segment*rowHeight-code.clientHeight*3)) {
      setBase(Math.floor(absolute)-Math.floor(segment/2));code.scrollTop=(absolute-base)*rowHeight;
    }
    hideHover();draw();save();
  });
  document.getElementById("rail").addEventListener("wheel",event=>{code.scrollTop+=event.deltaY;code.scrollLeft+=event.deltaX;event.preventDefault();},{passive:false});
  code.addEventListener("wheel",event=>{if(event.shiftKey&&event.deltaY&&!event.deltaX){code.scrollLeft+=event.deltaY;event.preventDefault();}},{passive:false});
  document.addEventListener("click",event=>{
    const button=event.target.closest("button"),action=button?.dataset.action;
    if(action) {if(action==="follow")go((state.line||2)-2,true);else post({action});return;}
    const point=event.target.closest("[data-breakpoint]");
    if(point&&!point.disabled){post({action:"breakpoint",operation:event.shiftKey?"enabled":"toggle",line:Number(point.dataset.breakpoint)});return;}
    const row=event.target.closest("[data-index]");if(row){selection=Number(row.dataset.index);if(!event.shiftKey)anchor=selection;draw();code.focus({preventScroll:true});save();}
  });
  document.addEventListener("contextmenu",event=>{const point=event.target.closest("[data-breakpoint]");if(point&&!point.disabled){event.preventDefault();post({action:"breakpoint",operation:"menu",line:Number(point.dataset.breakpoint)});}});
  document.getElementById("jump").addEventListener("submit",event=>{
    event.preventDefault();const input=document.getElementById("address"),raw=input.value.trim();
    if(!/^(?:0x[0-9a-f]+|[0-9]+)$/i.test(raw)){notice("Enter a hexadecimal or decimal byte offset.",true);return;}
    const target=BigInt(raw);let low=0,high=rows.length;
    while(low<high){const mid=Math.floor((low+high)/2);if(BigInt(rows[mid].offset)<target)low=mid+1;else high=mid;}
    if(low===rows.length||BigInt(rows[low].offset)!==target){notice("That offset is not an instruction boundary in this function.",true);return;}
    notice("");go(low,true);code.focus({preventScroll:true});
  });
  document.addEventListener("keydown",event=>{
    if(event.target instanceof HTMLInputElement)return;
    const command=event.key==="F5"?"continue":event.key==="F10"?"next":event.key==="F11"?(event.shiftKey?"stepOut":"stepIn"):undefined;
    if(command){event.preventDefault();event.stopPropagation();if(canWrite())post({action:command});else notice("Execution control requires full permission and a current stop.",true);return;}
    if(event.key==="Escape"){hideHover();return;}
    if(event.key==="F9"){event.preventDefault();if(canWrite())post({action:"breakpoint",operation:event.shiftKey?"enabled":"toggle",line:rows[selection]?.line});return;}
    if((event.ctrlKey||event.metaKey)&&event.key.toLowerCase()==="c" && !window.getSelection()?.toString()){event.preventDefault();post({action:"copy",first:anchor,last:selection});return;}
    if(event.key==="ArrowDown"||event.key==="ArrowUp"){event.preventDefault();go(selection+(event.key==="ArrowDown"?1:-1));}
    if(event.key==="ArrowRight"||event.key==="ArrowLeft"){event.preventDefault();code.scrollLeft+=(event.key==="ArrowRight"?1:-1)*4*charWidth;}
  });
  document.addEventListener("pointerover",event=>{
    const token=event.target.closest(".token");if(!token)return;hideHover();pointer={x:event.clientX,y:event.clientY,target:token};
    const request=++hoverSerial;
    hoverTimer=setTimeout(()=>{if(state.valid&&pointer?.target===token)post({action:"hover",request,line:Number(token.dataset.line),part:token.dataset.hover,token:token.dataset.token===undefined?undefined:Number(token.dataset.token)});},350);
  });
  document.addEventListener("pointerout",event=>{if(event.target.closest(".token"))hideHover();});
  window.addEventListener("message",event=>{
    const message=event.data;if(!message||!Number.isSafeInteger(message.revision)||message.revision<revision)return;
    revision=message.revision;
    if(message.action==="editorStyle") {
      const top=base+code.scrollTop/rowHeight,left=code.scrollLeft;
      editorStyle=message;
      const root=document.documentElement.style;
      root.setProperty("--vscode-editor-font-size",message.fontSize+"px");
      root.setProperty("--vscode-editor-font-family",message.fontFamily);
      root.setProperty("--vscode-editor-font-weight",message.fontWeight);
      code.style.letterSpacing=rail.style.letterSpacing=message.letterSpacing+"px";
      measure();code.scrollTop=(top-base)*rowHeight;code.scrollLeft=left;draw();return;
    }
    if(message.action==="load") {
      rows=message.rows;mnemonicWidth=message.mnemonicWidth+2;operandWidth=message.operandWidth;
      document.documentElement.style.setProperty("--mnemonic-width",mnemonicWidth+"ch");document.documentElement.style.setProperty("--operand-width",operandWidth+"ch");
      const addressColumns=rows.reduce((n,row)=>Math.max(n,row.address.length),message.addressBits/4+2);
      document.documentElement.style.setProperty("--address-width",addressColumns+"ch");code.setAttribute("aria-rowcount",String(rows.length));
      maxColumns=rows.reduce((n,row)=>Math.max(n,row.depth*4+mnemonicWidth+operandWidth+row.comment.length+4),0);
      drawn.clear();body.replaceChildren();rail.replaceChildren();measure();
      if(manualSaved){setBase(Math.floor(manualSaved.top/segment)*segment);code.scrollTop=(manualSaved.top-base)*rowHeight;code.scrollLeft=manualSaved.left;selection=manualSaved.selection||0;anchor=manualSaved.anchor||selection;manualSaved=undefined;}
      draw();return;
    }
    if(message.action==="state") {
      const old=state;state=message;if(old.stopId!==state.stopId||old.threadId!==state.threadId||old.valid!==state.valid)hideHover();
      document.getElementById("status").textContent=state.valid?"Paused · thread "+state.threadId+" · stop "+state.stopId+" · "+state.permission+" · arrow marks the next instruction":state.message||"Historical instruction view; execution controls are disabled.";
      document.getElementById("identity").textContent=(state.sessionName?state.sessionName+" · ":"")+"module "+(state.module??"?")+" · func "+(state.function??"?")+(state.name?" · "+state.name:"");
      for(const button of document.querySelectorAll("button[data-action]"))if(["continue","next","stepIn","stepOut"].includes(button.dataset.action))button.disabled=!canWrite();
      const key=state.valid?state.stopId+":"+state.threadId+":"+state.line:undefined;
      if(key&&key!==lastFollow){lastFollow=key;go(state.line-2,true);}else draw();
      if(old.valid&&!state.valid)notice("");return;
    }
    if(message.action==="breakpoints"){points=new Map(message.rows.map(row=>[row.line,row]));draw();return;}
    if(message.action==="notice"){notice(message.message,message.error);return;}
    if(message.action==="hover"&&message.request===hoverSerial&&pointer&&state.valid) {
      tip.replaceChildren(element("strong","",message.title||""));
      if(message.type!==undefined)tip.append(element("span","type",message.type+" "));
      if(message.value!==undefined)tip.append(element("span","value",message.value+"\n"));
      if(message.description)tip.append(element("div","",message.description));
      tip.hidden=false;const width=tip.offsetWidth,height=tip.offsetHeight;
      tip.style.left=Math.max(4,Math.min(pointer.x,window.innerWidth-width-8))+"px";tip.style.top=Math.max(4,Math.min(pointer.y+18,window.innerHeight-height-8))+"px";
    }
  });
  new ResizeObserver(()=>{measure();draw();}).observe(code);
  post({action:"ready"});
})();
