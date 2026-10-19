/**
 * @file run.mjs
 * @brief Actual application entry for the English-commented SDK lessons.
 *
 * Usage: node run.mjs sync|relay 1|2 /absolute/plan.json [absolute-setup.mjs]
 * sync: explicit setup module exports createAdmittedInstance(version), returning
 * the real authority-aware WebAssembly.Instance. No JS ROS mocks are accepted.
 * relay: supervisor supplies fd3 Worker binary output and fd4 binary input.
 * This program spawns no actors, configures no Runtime rights and reads no
 * ambient environment. Node host file/module/stdio privileges are independent
 * of the strictly configured Wasm principal. Use only trusted explicit inputs.
 */
import { createReadStream,createWriteStream } from 'node:fs';
import { open as openFile } from 'node:fs/promises';
import { pathToFileURL } from 'node:url';
import { isAbsolute } from 'node:path';
import { fromGuestBridge } from '../../javascript/bridge.mjs';
import { Session } from '../../javascript/roswasm.mjs';
import { AsyncSession } from '../../javascript/async.mjs';
import { StringMessage } from '../../javascript/codecs.mjs';
import { memoryRange } from '../../javascript/raw.mjs';
import { openAlias,closeAlias } from './01-raw.mjs';
import { greetingOnce } from './03-node.mjs';
import { resourceOnce } from './04-resources.mjs';
import * as Sync from './07-sync-application.mjs';
import * as Async from './06-async-application.mjs';
import { customTopics,customTopicsAsync } from './08-custom-codecs.mjs';
import {runUnifiedEvents} from './09-unified-events.mjs';

function require(condition,text) {if(!condition)throw new TypeError(text);}
function output(value) {process.stdout.write(JSON.stringify(value,(_,v)=>typeof v==='bigint'?v.toString():v instanceof Uint8Array?Array.from(v):v)+'\n');}
function statusResult(result) {
  const status=typeof result==='number'?result:result?.status;
  if(Number.isInteger(status)&&status!==0)process.exitCode=1;return result;
}
/** Read a bounded explicit host plan. Fixed storage and one extra byte detect
 * overlong files before JSON parsing; no unbounded readFile allocation occurs.
 */
async function readPlan(path) {
  require(isAbsolute(path),'Explicit absolute plan path required');const file=await openFile(path,'r'),bytes=new Uint8Array(65537);let count=0;
  try {
    require((await file.stat()).isFile(),'Regular plan file required');
    while(count<bytes.length) {const r=await file.read(bytes,count,bytes.length-count,null);if(!r.bytesRead)break;count+=r.bytesRead;}
    require(count<=65536,'Plan maximum64KiB');
    const plan=JSON.parse(new TextDecoder('utf-8',{fatal:true}).decode(bytes.subarray(0,count)));
    require(plan&&typeof plan==='object'&&!Array.isArray(plan),'Explicit plan object required');
    const keys=['lesson','aliases','actions','steps','handlers','timer','customCodecModule','rawAlias'];
    require(Object.keys(plan).every(k=>keys.includes(k)),'Unknown plan key');
    require(['raw','node','resources','application','custom','events'].includes(plan.lesson),'Explicit lesson required');
    require(Number.isSafeInteger(plan.steps??0)&&(plan.steps??0)>=0&&(plan.steps??0)<=200,'Bounded explicit0..200 steps');
    require(Array.isArray(plan.actions??[])&&(plan.actions??[]).length<=64,'At most64 explicit actions');
    return plan;
  } finally {await file.close();}
}
/** JSON cannot represent BigInt. Only this exact tagged unsigned/signed64-bit
 * argument becomes a BigInt; plain numbers are never silently widened.
 */
function argument(value) {
  if(value&&typeof value==='object'&&!Array.isArray(value)) {
    if(Object.keys(value).length===1&&Array.isArray(value.$bytes)) {require(value.$bytes.length<=4080&&value.$bytes.every(b=>Number.isInteger(b)&&b>=0&&b<=255),'Bounded explicit byte argument');return Uint8Array.from(value.$bytes);}
    if(Object.keys(value).length===1&&typeof value.$i64==='string') {require(/^-?(?:0|[1-9][0-9]{0,18})$/.test(value.$i64),'Exact i64 decimal string');const n=BigInt(value.$i64);require(n>=-0x8000000000000000n&&n<=0x7fffffffffffffffn,'i64 range');return n;}
    if(Object.keys(value).length===1&&typeof value.$u64==='string') {require(/^(?:0|[1-9][0-9]{0,19})$/.test(value.$u64),'Exact u64 decimal string');const n=BigInt(value.$u64);require(n<=0xffffffffffffffffn,'u64 range');return n;}
    return Object.fromEntries(Object.entries(value).map(([k,v])=>[k,argument(v)]));
  }
  return Array.isArray(value)?value.map(argument):value;
}
const METHODS={publisher:['publish'],client:['call'],parameters:['get','set','list','describe','declare','undeclare','delete','exists','search','subscribeEvents','pollEvent'],
  actionClient:['sendGoal','cancelOwn','cancelAny','cancelAll','feedback','result'],graph:['read'],clock:['now'],tf:['lookup','broadcast'],lifecycle:['getState','changeState'],logger:['write'],resource:['call']};
/** Execute only closed public methods, never arbitrary reflection. Each action
 * reports its status; a nonzero Runtime result is not automatically retried or
 * hidden. A deferred client action is run only by an explicit later pass.
 */
async function actions(application,plan) {
  const futures=[];
  for(const action of plan.actions??[]) {
    require(action&&typeof action==='object'&&Object.keys(action).every(k=>['object','method','args','index'].includes(k)),'Closed action object');
    const args=argument(action.args??[]);require(Array.isArray(args)&&args.length<=4,'Bounded method arguments');
    if(action.object==='executor') {require(action.method==='spinOnce'&&args.length===0,'Explicit executor step required');output({operation:'spinOnce',statuses:await application.executor.spinOnce()});continue;}
    const object=action.object==='clock'?application.objects.clocks?.[action.index??0]:application.objects[action.object];
    require(object,'Action requires an explicitly created resource');
    if(action.object==='client'&&action.method==='callDeferred') {futures.push(object.callDeferred(application.executor,...args));output({operation:'callDeferred',future:futures.length-1,state:'pending'});continue;}
    require(METHODS[action.object]?.includes(action.method),'Unknown action method');output({operation:action.object+'.'+action.method,result:statusResult(await object[action.method](...args))});
  }
  for(let pass=0;pass<(plan.steps??0);pass++)output({operation:'spinOnce',statuses:await application.executor.spinOnce()});
  for(let index=0;index<futures.length;index++) {
    const future=futures[index];output({future:index,state:future.state,...(future.done?{result:statusResult(future.result())}: {})});
  }
}

const [mode,versionText,planPath,setupPath,...extra]=process.argv.slice(2),version=Number(versionText);
let readable,writable,session,application,transportClosed=false;
try {
  require(extra.length===0&&(mode==='sync'||mode==='relay')&&(versionText==='1'||versionText==='2'),'Explicit mode/backend/plan arguments');
  require(mode==='sync'?typeof setupPath==='string'&&isAbsolute(setupPath):setupPath===undefined,'Sync setup module required; relay accepts no setup module');
  const plan=await readPlan(planPath);let bridge,scratch;
  if(mode==='sync') {
    const setup=await import(pathToFileURL(setupPath).href);require(typeof setup.createAdmittedInstance==='function','Trusted setup must export createAdmittedInstance');
    ({bridge,scratch}=fromGuestBridge(version,await setup.createAdmittedInstance(version)));session=new Session(version,bridge,scratch);
  } else {
    readable=createReadStream(null,{fd:3,autoClose:false,highWaterMark:8192});writable=createWriteStream(null,{fd:4,autoClose:false,highWaterMark:8192});
    session=await AsyncSession.attach({version,readable,writable,timeoutMs:10000});
  }
  if(plan.lesson==='raw') {
    require(typeof plan.rawAlias==='string'&&plan.rawAlias.length,'Explicit rawAlias required');
    if(mode==='sync') {
      const input=StringMessage.encode(plan.rawAlias).subarray(4);require(input.length<=scratch.inputCapacity,'Reserved alias limit');
      memoryRange(bridge.memory,scratch.offset,BigInt(input.length)).set(input);
      const r=openAlias(bridge,version,scratch.offset,BigInt(input.length),scratch.offset+BigInt(scratch.inputCapacity));statusResult(r);output({operation:'raw.open',...r});
      if(r.status===0)output({operation:'raw.close',status:statusResult(closeAlias(bridge,version,r.handle))});
    } else {
      const input=StringMessage.encode(plan.rawAlias).subarray(4),s=session.layout;
      const r=await session.raw('capability_open',[0,s.input,BigInt(input.length),s.slot],{input,slotSeed:0n});statusResult(r);output({operation:'raw.open',status:r.status,handle:r.status?null:r.slot});
      if(r.status===0)output({operation:'raw.close',status:statusResult(await session.handle('capability_close',r.slot))});
    }
  } else if(plan.lesson==='events') {
    require(plan.aliases&&typeof plan.aliases.source==='string'&&typeof plan.aliases.observer==='string','Explicit source/observer aliases required');
    output(statusResult(await runUnifiedEvents(session,plan.aliases.source,plan.aliases.observer)));
  } else if(plan.lesson==='node'||plan.lesson==='resources') {
    require(mode==='sync'&&version===2,'Legacy recipe lessons require explicit sync ROS2 setup; use application for both transports/versions');
    output(plan.lesson==='node'?greetingOnce(bridge,scratch,value=>output({received:value})):resourceOnce(bridge,scratch));
  } else {
    const hooks={};
    if(plan.handlers?.serviceResponse)hooks.service=()=>argument(plan.handlers.serviceResponse);
    if(plan.handlers?.actionResponse)hooks.action=()=>argument(plan.handlers.actionResponse);
    hooks.message=value=>output({received:value});hooks.parameterEvent=event=>output({parameterEvent:event});
    if(plan.timer) {require(typeof plan.timer.ready==='boolean','Timer requires caller-supplied readiness boolean');hooks.timerReady=()=>plan.timer.ready;hooks.timer=()=>output({localTimerReady:true});}
    const library=mode==='sync'?Sync:Async;application=await library.createApplication(session,plan.aliases??{},hooks);
    if(plan.lesson==='custom') {
      require(typeof plan.customCodecModule==='string'&&isAbsolute(plan.customCodecModule),'Explicit generated codec module required');
      const generated=await import(pathToFileURL(plan.customCodecModule).href);require(generated.codec&&plan.aliases?.custom,'Generated codec export and custom aliases required');
      const resources=mode==='sync'?customTopics(application.node,generated.codec,plan.aliases.custom,hooks.message):await customTopicsAsync(application.node,generated.codec,plan.aliases.custom,hooks.message);
      application.objects.publisher=resources.publisher;application.objects.subscription=resources.subscription;application.executor.add(resources.subscription);
    }
    await actions(application,plan);
    const cleanup=await library.closeApplication(application,mode==='relay'?{transport:true}:undefined);output({cleanup});
    if(mode==='relay'?!cleanup.clean:cleanup.some(statuses=>statuses.some(status=>status!==0)))process.exitCode=1;
    transportClosed=mode==='relay';application=null;
  }
  if(mode==='relay'&&!transportClosed){await session.shutdown();transportClosed=true;output({transport:'acknowledged'});}
} catch(error) {
  // Report fixed public code/operation only. No policy/key/body/setup exception
  // is echoed. Expected default denial remains13 and unsupported remains95.
  output({status:'APPLICATION_REFUSED',code:Number.isInteger(error?.code)?error.code:null,operation:typeof error?.operation==='string'?error.operation:null});process.exitCode=1;
} finally {
  if(application)try {output({cleanup:await (mode==='sync'?Sync:Async).closeApplication(application)});}catch {output({cleanup:'FAILED'});}
  readable?.destroy();writable?.destroy();
}
