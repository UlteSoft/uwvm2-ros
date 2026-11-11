"use strict";
(() => {
  const vscode=acquireVsCodeApi();let epoch=Number(document.body.dataset.viewEpoch);
  const post=value=>vscode.postMessage({...value,epoch});
  const form=()=>document.getElementById("update-form");
  const fields=()=>Object.fromEntries(new FormData(form()).entries());
  document.addEventListener("change",event=>{
    if(event.target.name!=="mode")return;
    for(const field of form().querySelectorAll("[data-transports]"))field.hidden=!field.dataset.transports.split(" ").includes(event.target.value);
  });
  document.addEventListener("click",event=>{
    const button=event.target.closest("button");if(!button || button.disabled)return;
    if(button.dataset.action==="new-window")post({action:"new-window"});
    if(button.dataset.browse){const value=fields();for(const control of form().querySelectorAll("input,select,button"))control.disabled=true;
      post({action:"browse",field:button.dataset.browse,fields:value});}
  });
  document.addEventListener("submit",event=>{
    if(event.target.id!=="update-form")return;event.preventDefault();
    const value=fields();for(const control of form().querySelectorAll("input,select,button"))control.disabled=true;
    post({action:"submit",fields:value});
  });
  window.addEventListener("message",event=>{
    const value=event.data;if(!value || value.action!=="render" || typeof value.html!=="string")return;
    const parsed=new DOMParser().parseFromString(value.html,"text/html"),next=Number(parsed.body.dataset.viewEpoch);
    if(!Number.isSafeInteger(next)||next<epoch)return;
    const scroll=document.scrollingElement.scrollTop,active=document.activeElement,name=active?.name;
    const selection=[active?.selectionStart,active?.selectionEnd],open=[...document.querySelectorAll("details[open]")].map(n=>n.dataset.node);
    for(const script of parsed.body.querySelectorAll("script"))script.remove();
    document.body.replaceChildren(...parsed.body.childNodes);document.body.dataset.viewEpoch=String(epoch=next);
    for(const detail of document.querySelectorAll("details[data-node]"))detail.open=open.includes(detail.dataset.node);
    if(name){const node=form().elements.namedItem(name);if(node && !node.disabled){node.focus({preventScroll:true});try{node.setSelectionRange(...selection);}catch(_){}}}
    // Retained background webviews may not deliver animation frames.
    document.scrollingElement.scrollTop=scroll;
  });
  post({action:"ready"});
})();
