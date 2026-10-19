/**
 * @file javascript_wasm_relay_models.mjs
 * @brief Guest-local execution models of the actual compiled C relay command.
 *
 * Owner-run usage, from a pinned fresh /tmp source snapshot:
 * node --max-old-space-size=96 javascript_wasm_relay_models.mjs \
 *   /tmp/ros1-relay.wasm /tmp/ros2-relay.wasm /tmp/fresh-receipt.json
 *
 * Exactly40 named cases run serially for each backend,80 total. Every case
 * instantiates the supplied real WebAssembly.Module, uses its actual exported
 * memory and invokes its actual _start. No fake Memory/Instance is supplied.
 * The mathematical ROS stubs grant no aliases: ABI1, open13 and unknown handle9.
 * Two guard injections and a bad successful output-length injection model a
 * broken import contract; they are not permission or delivery successes.
 *
 * The only other stubs are bounded in-memory fd_read/fd_write/proc_exit. This
 * program starts no Runtime, native ROS, Gateway, process, socket or thread.
 * A pass establishes compiled guest-local framing/dispatch/error behavior only.
 * It does not authenticate an Instance, exercise uwvm policy, prove native
 * effects or claim a sandboxed Node host. Root owns RSS/deadline supervision.
 */
import assert from 'node:assert/strict';
import { open } from 'node:fs/promises';
import { constants } from 'node:fs';
import { isAbsolute } from 'node:path';
import { createHash } from 'node:crypto';
import { fileURLToPath } from 'node:url';

const MAGIC=0x524a5755,HEADER=24,HELLO=1,CALL=2,RESULT=3,SHUTDOWN=4,FATAL=5;
const NONE=65535,GUARD=16,CAPACITY=4080,MEMORY_BYTES=131072;
const MAX_FRAME_IO=16384,MAX_IO_CALLS=4096,MAX_ARTIFACT=1048576;
const HI=0xfedcba9876543210n,SEED=0x8877665544332211n,MAX=0xffffffffffffffffn;
const decoder=new TextDecoder('utf-8',{fatal:true,ignoreBOM:true});
function view(bytes){return new DataView(bytes.buffer,bytes.byteOffset,bytes.byteLength);}
function hash(bytes){return createHash('sha256').update(bytes).digest('hex');}
function u64(value){assert.equal(typeof value,'bigint');assert.ok(value>=0n&&value<=MAX);return value;}
function bits(value){return BigInt.asUintN(64,value);}

/** Bounded descriptor read, with before/after identity and byte checks. It is
 * ordinary Node host input, not a filesystem capability provided to the Wasm.
 * Never replace a receipt, read a FIFO, or allocate from an unbounded pathname.
 */
async function inputFile(path,maximum=MAX_ARTIFACT) {
  assert.ok(isAbsolute(path),'explicit absolute input path');
  assert.ok(Number.isInteger(constants.O_NONBLOCK)&&constants.O_NONBLOCK>0,'nonblocking descriptor support');
  const file=await open(path,constants.O_RDONLY|constants.O_NONBLOCK);
  try {
    const before=await file.stat({bigint:true});
    assert.ok(before.isFile()&&before.size>0n&&before.size<=BigInt(maximum),'bounded regular input');
    const bytes=new Uint8Array(Number(before.size));let offset=0;
    while(offset<bytes.length){const r=await file.read(bytes,offset,bytes.length-offset,offset);assert.ok(r.bytesRead>0,'input must not truncate');offset+=r.bytesRead;}
    const after=await file.stat({bigint:true});
    assert.ok(after.isFile());for(const field of ['dev','ino','size','mtimeNs','ctimeNs'])assert.equal(after[field],before[field],'input identity preserved');
    return {bytes,pin:{path,bytes:bytes.length,sha256:hash(bytes),device:before.dev.toString(),inode:before.ino.toString(),mtime_ns:before.mtimeNs.toString(),ctime_ns:before.ctimeNs.toString()}};
  } finally {await file.close();}
}

/** The independent mathematical encoder deliberately accepts malformed header
 * fields for negative cases. It does not reuse relay.mjs's encoder or decoder.
 */
function frame(version,{kind=CALL,sequence=1n,index=0,arity=0,reserved=0,magic=MAGIC,protocol=1,bodyLength},body=new Uint8Array()) {
  assert.ok(body.length<=MAX_FRAME_IO);const bytes=new Uint8Array(HEADER+body.length),d=view(bytes);
  d.setUint32(0,magic,true);d.setUint16(4,protocol,true);bytes[6]=version;bytes[7]=kind;
  d.setBigUint64(8,u64(sequence),true);d.setUint16(16,index,true);bytes[18]=arity;bytes[19]=reserved;
  d.setUint32(20,bodyLength??body.length,true);bytes.set(body,HEADER);return bytes;
}
function together(frames) {
  const size=frames.reduce((n,b)=>n+b.length,0);assert.ok(size<=MAX_FRAME_IO);
  const bytes=new Uint8Array(size);let offset=0;for(const b of frames){bytes.set(b,offset);offset+=b.length;}return bytes;
}
function body(args,{input=new Uint8Array(),seed=SEED,snapshot=64}={}) {
  const bytes=new Uint8Array(args.length*8+12+input.length),d=view(bytes);
  args.forEach((arg,index)=>d.setBigUint64(index*8,u64(typeof arg==='bigint'?arg:BigInt(arg)),true));
  d.setBigUint64(args.length*8,u64(seed),true);d.setUint32(args.length*8+8,snapshot,true);bytes.set(input,args.length*8+12);return bytes;
}
function command(version,spec,name,args,options={}) {
  const index=spec.functions.findIndex(f=>f.name===name);assert.ok(index>=0,'selected schema entry');
  return frame(version,{index,arity:spec.functions[index].params.length,sequence:options.sequence??1n},body(args,options));
}
function shutdown(version,sequence=1n){return frame(version,{kind:SHUTDOWN,sequence,index:NONE});}
/** Construct one independent mathematical call from the closed ABI form.
 * Arity alone cannot distinguish WATCH/READY from one another, nor SUBMIT
 * from a request/response RPC. Check every declared scalar type before making
 * the frame. Observer/source handles and correlation retain their full i64
 * bits; the mathematical imports still grant no authority and return9.
 * WATCH and SUBMIT expose no output snapshot. READY has one exact32-byte
 * output record; its protected length/ID slot remains seeded and untouched.
 */
function mathematicalCall(entry,layout) {
  const signatures={
    version:[],handle:['i64'],open:['i32','i64','i64','i64'],
    write:['i64','i32','i64','i64'],poll:['i64','i32','i64','i64','i64'],
    rpc:['i64','i32','i64','i64','i64','i64','i64'],
    watch:['i64','i64','i64'],ready:['i64','i32','i64'],
    submit:['i64','i64','i32','i64','i64','i64']
  };
  assert.ok(Object.hasOwn(signatures,entry.form),'closed selected ABI form');
  assert.deepEqual(entry.params.map(p=>p.type),signatures[entry.form],'exact selected form scalar signature');
  assert.deepEqual(entry.results,['i32'],'one status/version scalar result');
  const empty=new Uint8Array(),payload=Uint8Array.of(1,0,0,0,65);
  switch(entry.form) {
    case 'version':return {args:[],input:empty,snapshot:64};
    case 'handle':return {args:[HI],input:empty,snapshot:64};
    case 'open': {
      assert.equal(entry.name,'capability_open','only alias-open has this form');
      const input=new TextEncoder().encode('ungranted');
      return {args:[0,layout.input,BigInt(input.length),layout.slot],input,snapshot:64};
    }
    case 'write':return {args:[HI,0,layout.input,BigInt(payload.length)],input:payload,snapshot:64};
    case 'poll':return {args:[HI,0,layout.output,64n,layout.slot],input:empty,snapshot:64};
    case 'rpc':return {args:[HI,0,layout.input,BigInt(payload.length),layout.output,64n,layout.slot],input:payload,snapshot:64};
    case 'watch':return {args:[HI,HI-1n,SEED],input:empty,snapshot:0};
    case 'ready':return {args:[HI,0,layout.output],input:empty,snapshot:32};
    case 'submit':return {args:[HI,HI-1n,0,layout.input,BigInt(payload.length),layout.slot],input:payload,snapshot:0};
    default:assert.fail('unimplemented selected ABI form');
  }
}
function canonical(value) {
  if(Array.isArray(value))return '['+value.map(canonical).join(',')+']';
  if(value&&typeof value==='object')return '{'+Object.keys(value).sort().map(k=>JSON.stringify(k)+':'+canonical(value[k])).join(',')+'}';
  return JSON.stringify(value);
}

/** Strict independent output parser. It checks fixed fields and full body
 * consumption. Output storage is bounded once; no growing binary queue exists.
 */
function outputFrames(bytes,version) {
  const rows=[];let offset=0;
  while(offset<bytes.length) {
    assert.ok(bytes.length-offset>=HEADER,'complete output header');const d=view(bytes.subarray(offset));
    assert.equal(d.getUint32(0,true),MAGIC);assert.equal(d.getUint16(4,true),1);assert.equal(bytes[offset+6],version);assert.equal(bytes[offset+19],0);
    const length=d.getUint32(20,true);assert.ok(length<=20+CAPACITY&&length<=bytes.length-offset-HEADER,'bounded complete output body');
    rows.push({kind:bytes[offset+7],sequence:d.getBigUint64(8,true),index:d.getUint16(16,true),arity:bytes[offset+18],body:bytes.slice(offset+HEADER,offset+HEADER+length)});
    offset+=HEADER+length;
  }
  return rows;
}
function result(row,{status,slot=SEED,snapshot=64}) {
  assert.equal(row.kind,RESULT);assert.equal(row.body.length,20+snapshot);const d=view(row.body);
  assert.equal(d.getUint32(0,true),status);assert.equal(d.getBigUint64(4,true),slot);
  assert.equal(d.getUint32(12,true),snapshot);assert.equal(d.getUint32(16,true),1);
  assert.ok(row.body.subarray(20).every(b=>b===0xa5),'default refusal output sentinel');
}
function fatal(run,code,{hello=true,priorResults=0}={}) {
  assert.equal(run.exit,1);assert.equal(run.frames.length,(hello?1:0)+priorResults+1);
  const row=run.frames.at(-1);assert.equal(row.kind,FATAL);assert.equal(row.index,NONE);assert.equal(row.arity,0);assert.equal(row.body.length,4);assert.equal(view(row.body).getUint32(0,true),code);
}
function acknowledged(run,{results=0}={}) {
  assert.equal(run.exit,0);assert.equal(run.frames.length,results+2);const row=run.frames.at(-1);
  assert.equal(row.index,NONE);assert.equal(row.arity,0);result(row,{status:0,slot:0n,snapshot:0});
}

/** A thrown host token models proc_exit without calling process.exit or
 * confusing an arbitrary trap/assertion with the command's explicit exit.
 */
class ExitToken {constructor(code){this.code=code;}}

/** Instantiate the actual artifact afresh for one serial case. All Wasm
 * pointers/iovecs are checked against its actual exported memory. Preloaded
 * input, output, host call counts and short-transfer/error plans are bounded.
 * No numerical result is fabricated for HELLO or dispatcher output: those bytes
 * are written by the real compiled C command through the fd_write stub.
 */
function execute(module,version,spec,{input=(_layout)=>shutdown(version),readPlan=[],writePlan=[],abi=1,fault=null}={}) {
  let memory,layout,instance,inputBytes,inputOffset=0,outputOffset=0,readCalls=0,writeCalls=0,ioCalls=0,exit=null;
  const output=new Uint8Array(MAX_FRAME_IO),calls=[],imports={},moduleName='uwvm_ros'+version+'_v1';
  const next=(plan,index)=>plan[index]??{};
  function range(pointer,size) {
    assert.ok(memory instanceof WebAssembly.Memory,'use actual exported memory');
    assert.ok(Number.isInteger(pointer)&&Number.isInteger(size)&&pointer>=0&&size>=0&&pointer<=memory.buffer.byteLength&&size<=memory.buffer.byteLength-pointer,'bounded wasm32 range');
    return new Uint8Array(memory.buffer,pointer,size);
  }
  function vector(fd,expected,ptr,count,out) {
    assert.equal(fd>>>0,expected);assert.equal(count>>>0,1);assert.ok(++ioCalls<=MAX_IO_CALLS,'bounded host transfer work');
    const d=view(range(ptr>>>0,8)),address=d.getUint32(0,true),length=d.getUint32(4,true);
    assert.ok(length>0);range(address,length);range(out>>>0,4);return {address,length,out:out>>>0};
  }
  function finishCount(v,count){view(range(v.out,4)).setUint32(0,count,true);}
  const wasi={
    fd_read(fd,ptr,count,out) {
      const v=vector(fd,0,ptr,count,out),step=next(readPlan,readCalls++);
      if(step.errno){finishCount(v,0);return step.errno;}
      if(step.oversize){finishCount(v,v.length+1);return 0;}
      const n=step.zero?0:Math.min(v.length,inputBytes.length-inputOffset,step.take??v.length);
      if(n)range(v.address,n).set(inputBytes.subarray(inputOffset,inputOffset+n));inputOffset+=n;finishCount(v,n);return 0;
    },
    fd_write(fd,ptr,count,out) {
      const v=vector(fd,1,ptr,count,out),step=next(writePlan,writeCalls++);
      assert.ok(readCalls+writeCalls<=MAX_IO_CALLS);
      if(step.errno){finishCount(v,0);return step.errno;}
      if(step.oversize){finishCount(v,v.length+1);return 0;}
      const n=step.zero?0:Math.min(v.length,step.take??v.length);assert.ok(n<=output.length-outputOffset,'bounded stdout');
      output.set(range(v.address,n),outputOffset);outputOffset+=n;finishCount(v,n);return 0;
    },
    proc_exit(code){assert.equal(arguments.length,1);throw new ExitToken(code>>>0);}
  };
  imports[moduleName]={};imports.wasi_snapshot_preview1=wasi;
  for(const f of spec.functions)imports[moduleName][f.name]=(...raw)=>{
    assert.equal(raw.length,f.params.length,'actual selected import arity');
    const args=raw.map((value,index)=>{
      assert.equal(typeof value,f.params[index].type==='i64'?'bigint':'number','actual selected import argument type');
      return typeof value==='bigint'?bits(value):value>>>0;
    });
    calls.push({name:f.name,args});assert.ok(calls.length<=128,'bounded mathematical ROS calls');
    if(f.name==='abi_version')return abi;
    if(fault)fault({name:f.name,args,range,layout});
    if(f.name==='capability_open') {
      assert.equal(args[0],0);assert.equal(args[1],layout.input);assert.equal(args[3],layout.slot);
      assert.equal(decoder.decode(range(Number(args[1]),Number(args[2]))),'ungranted');return 13;
    }
    return fault?.operation===f.name&&fault.status!==undefined?fault.status:9;
  };
  instance=new WebAssembly.Instance(module,imports);
  assert.equal(calls.length,0,'no hidden Wasm start invocation');assert.equal(readCalls+writeCalls,0);
  memory=instance.exports.memory;assert.ok(memory instanceof WebAssembly.Memory);assert.equal(memory.buffer.byteLength,MEMORY_BYTES);
  assert.throws(()=>memory.grow(1),RangeError,'compiled maximum is exactly2pages');assert.equal(memory.buffer.byteLength,MEMORY_BYTES);
  for(const name of ['_start','roswasm_scratch_offset','roswasm_input_capacity','roswasm_output_capacity'])assert.equal(typeof instance.exports[name],'function');
  assert.equal(instance.exports.roswasm_input_capacity(),4096);assert.equal(instance.exports.roswasm_output_capacity(),4096);
  const scratch=bits(instance.exports.roswasm_scratch_offset());layout={input:scratch+16n,slot:scratch+4096n,output:scratch+4104n,inputCapacity:CAPACITY,outputCapacity:CAPACITY};
  assert.ok(layout.input>=16n&&layout.output+BigInt(CAPACITY+GUARD)<=BigInt(MEMORY_BYTES));
  inputBytes=input(layout);assert.ok(inputBytes instanceof Uint8Array&&inputBytes.length<=MAX_FRAME_IO);
  try {instance.exports._start();assert.fail('noreturn command must call proc_exit');}
  catch(error){if(!(error instanceof ExitToken))throw error;exit=error.code;}
  const frames=outputFrames(output.subarray(0,outputOffset),version);
  if(abi===1&&frames[0]?.kind===HELLO) {
    const row=frames[0],d=view(row.body);assert.equal(row.sequence,0n);assert.equal(row.index,NONE);assert.equal(row.arity,0);assert.equal(row.body.length,80);
    assert.equal(d.getUint32(0,true),1);assert.equal(d.getUint32(4,true),spec.functions.length);
    const canonicalTable={abi_version:1,module:moduleName,functions:spec.functions.map(f=>({name:f.name,parameters:f.params.map(p=>p.type),result:'i32'}))};
    assert.equal(Buffer.from(row.body.subarray(8,40)).toString('hex'),hash(canonical(canonicalTable)));
    assert.equal(d.getBigUint64(40,true),BigInt(MEMORY_BYTES));
    for(const [at,name] of [[48,'input'],[56,'slot'],[64,'output']])assert.equal(d.getBigUint64(at,true),layout[name]);
    assert.equal(d.getUint32(72,true),CAPACITY);assert.equal(d.getUint32(76,true),CAPACITY);
  }
  return {exit,frames,calls,layout,inputOffset,readCalls,writeCalls,outputBytes:outputOffset};
}

/** Forty fixed per-backend cases. Full-dispatch is one case with every selected
 * entry checked, without claiming native effects. Each refusal asserts that no
 * unintended ROS import was dispatched after the ABI preflight.
 */
function cases(version,spec) {
  const list=[],add=(name,run)=>list.push({name,run});
  const abiInput=layout=>together([command(version,spec,'abi_version',[],{}),shutdown(version,2n)]);
  const rejected=(name,input,code,options={})=>add(name,module=>{
    const r=execute(module,version,spec,{input,...options});fatal(r,code);assert.equal(r.calls.length,1,'invalid call dispatches no ROS operation');return r;
  });
  add('HELLO exact layout/table and acknowledged shutdown',module=>{const r=execute(module,version,spec);acknowledged(r);assert.equal(r.calls.length,1);assert.equal(r.inputOffset,HEADER);return r;});
  add('ABI query uses actual compiled scalar dispatch',module=>{const r=execute(module,version,spec,{input:abiInput});acknowledged(r,{results:1});result(r.frames[1],{status:1});assert.deepEqual(r.calls.map(c=>c.name),['abi_version','abi_version']);return r;});
  add('default-denied open retains exact BigInt slot and output',module=>{
    const r=execute(module,version,spec,{input:l=>together([command(version,spec,'capability_open',[0,l.input,9n,l.slot],{input:new TextEncoder().encode('ungranted')}),shutdown(version,2n)])});
    acknowledged(r,{results:1});result(r.frames[1],{status:13});assert.equal(r.calls[1].name,'capability_open');return r;
  });
  add('every selected ABI dispatch preserves all i64 bits',module=>{
    const expected=[],snapshots=[],r=execute(module,version,spec,{input:l=>{
      const frames=spec.functions.map((f,index)=>{
        const {args,input,snapshot}=mathematicalCall(f,l);
        expected.push({name:f.name,args});snapshots.push(snapshot);
        return command(version,spec,f.name,args,{input,snapshot,sequence:BigInt(index+1)});
      });return together([...frames,shutdown(version,BigInt(frames.length+1))]);
    }});
    acknowledged(r,{results:spec.functions.length});assert.deepEqual(r.calls.slice(1),expected);
    spec.functions.forEach((f,index)=>{result(r.frames[index+1],{status:f.name==='abi_version'?1:f.name==='capability_open'?13:9,snapshot:snapshots[index]});assert.equal(r.frames[index+1].sequence,BigInt(index+1));assert.equal(r.frames[index+1].index,index);});return r;
  });
  add('one-byte short reads and writes complete without replay',module=>{
    const r=execute(module,version,spec,{input:abiInput,readPlan:Array.from({length:MAX_IO_CALLS},()=>({take:1})),writePlan:Array.from({length:MAX_IO_CALLS},()=>({take:1}))});acknowledged(r,{results:1});assert.equal(r.calls.length,2);assert.ok(r.readCalls>2&&r.writeCalls>4);return r;
  });
  add('sixteen consecutive EINTRs permit progress',module=>{const r=execute(module,version,spec,{readPlan:Array.from({length:16},()=>({errno:27})),writePlan:Array.from({length:16},()=>({errno:27}))});acknowledged(r);assert.equal(r.readCalls,17);assert.equal(r.calls.length,1);return r;});
  rejected('seventeenth read EINTR is terminal input failure',l=>shutdown(version),3,{readPlan:Array.from({length:17},()=>({errno:27}))});
  add('seventeenth write EINTR is terminal output failure',module=>{const r=execute(module,version,spec,{writePlan:Array.from({length:17},()=>({errno:27}))});fatal(r,4,{hello:false});assert.equal(r.calls.length,1);return r;});
  add('EINTR counter resets after short successful progress',module=>{
    const plan=[...Array.from({length:16},()=>({errno:27})),{take:1},...Array.from({length:16},()=>({errno:27}))];
    const r=execute(module,version,spec,{readPlan:plan,writePlan:plan});acknowledged(r);assert.equal(r.readCalls,34);assert.equal(r.calls.length,1);return r;
  });
  rejected('EOF before request header is terminal',()=>new Uint8Array(),3);
  rejected('truncated request header is terminal',()=>shutdown(version).slice(0,17),3);
  rejected('truncated declared request body is terminal',()=>frame(version,{bodyLength:12},new Uint8Array(11)),3);
  rejected('wrong magic rejects before dispatch',()=>frame(version,{magic:0},body([])),1);
  rejected('wrong protocol version rejects before dispatch',()=>frame(version,{protocol:2},body([])),1);
  rejected('wrong selected backend rejects before dispatch',()=>frame(version===1?2:1,{},body([])),1);
  rejected('nonzero reserved header byte rejects',()=>frame(version,{reserved:1},body([])),1);
  rejected('skipped sequence rejects without request replay',()=>frame(version,{sequence:2n},body([])),7);
  add('duplicate sequence rejects after exactly one prior query',module=>{const r=execute(module,version,spec,{input:l=>together([command(version,spec,'abi_version',[]),shutdown(version,1n)])});fatal(r,7,{priorResults:1});assert.equal(r.calls.length,2);return r;});
  rejected('unknown frame kind rejects',()=>frame(version,{kind:99},body([])),1);
  rejected('out-of-range ABI index rejects',()=>frame(version,{index:spec.functions.length},body([])),1);
  rejected('wrong original ABI arity rejects',()=>frame(version,{arity:1},body([HI])),1);
  rejected('body shorter than argument metadata rejects',()=>frame(version,{},new Uint8Array(11)),1);
  rejected('unbounded declared body refuses before reading it',()=>frame(version,{bodyLength:0xffffffff}),1);
  rejected('i32 argument high bits refuse narrowing',l=>command(version,spec,'capability_open',[0x100000000n,l.input,9n,l.slot],{input:new TextEncoder().encode('ungranted')}),2);
  rejected('nonzero memory index refuses',l=>command(version,spec,'capability_open',[1,l.input,9n,l.slot],{input:new TextEncoder().encode('ungranted')}),2);
  rejected('nonreserved input address refuses',l=>command(version,spec,'capability_open',[0,l.input+1n,9n,l.slot],{input:new TextEncoder().encode('ungranted')}),2);
  rejected('transferred input length mismatch refuses',l=>command(version,spec,'capability_open',[0,l.input,8n,l.slot],{input:new TextEncoder().encode('ungranted')}),2);
  rejected('nonreserved output slot refuses',l=>command(version,spec,'capability_open',[0,l.input,9n,l.slot+1n],{input:new TextEncoder().encode('ungranted')}),2);
  rejected('nonreserved poll output refuses',l=>command(version,spec,'event_poll',[HI,0,l.input,64n,l.slot]),2);
  rejected('poll capacity exceeding reserved bytes refuses',l=>command(version,spec,'event_poll',[HI,0,l.output,4081n,l.slot]),2);
  rejected('snapshot exceeding reserved bytes refuses',l=>command(version,spec,'abi_version',[],{snapshot:4081}),2);
  rejected('scalar call refuses transferred payload',l=>command(version,spec,'abi_version',[],{input:Uint8Array.of(0)}),2);
  rejected('transferred input exceeding reserved capacity refuses',l=>command(version,spec,'abi_version',[],{input:new Uint8Array(4081)}),2);
  add('broken success length greater than capacity refuses',module=>{
    const fault=({name,args,range})=>{if(name==='event_poll')view(range(Number(args[4]),8)).setBigUint64(0,65n,true);};fault.operation='event_poll';fault.status=0;
    const r=execute(module,version,spec,{input:l=>command(version,spec,'event_poll',[HI,0,l.output,64n,l.slot]),fault});fatal(r,2);assert.equal(r.calls.length,2);return r;
  });
  add('input-front guard detects broken import write',module=>{
    const fault=({layout,range})=>{range(Number(layout.input-1n),1)[0]^=1;};
    const r=execute(module,version,spec,{input:l=>command(version,spec,'capability_close',[HI]),fault});fatal(r,5);assert.equal(r.calls.length,2);return r;
  });
  add('output-tail guard detects broken import write',module=>{
    const fault=({layout,range})=>{range(Number(layout.output+BigInt(CAPACITY)),1)[0]^=1;};
    const r=execute(module,version,spec,{input:l=>command(version,spec,'capability_close',[HI]),fault});fatal(r,5);assert.equal(r.calls.length,2);return r;
  });
  add('zero-progress fd_write causes explicit failure',module=>{const r=execute(module,version,spec,{writePlan:[{zero:true}]});fatal(r,4,{hello:false});assert.equal(r.calls.length,1);return r;});
  add('oversized fd_write count causes explicit failure',module=>{const r=execute(module,version,spec,{writePlan:[{oversize:true}]});fatal(r,4,{hello:false});assert.equal(r.calls.length,1);return r;});
  rejected('oversized fd_read count causes explicit failure',()=>shutdown(version),3,{readPlan:[{oversize:true}]});
  add('ABI mismatch fails before HELLO or frame input',module=>{const r=execute(module,version,spec,{abi:2});fatal(r,6,{hello:false});assert.equal(r.readCalls,0);assert.equal(r.calls.length,1);return r;});
  assert.equal(list.length,40,'closed40 case inventory');return list;
}

/** Closed artifact CLI. Binary inspection/signature/full-validator receipts are
 * independently supplied by Root; this model also refuses unknown/duplicate/
 * version-mixed module imports before constructing any Instance.
 */
const [ros1Path,ros2Path,receiptPath,...extra]=process.argv.slice(2);
assert.equal(extra.length,0);assert.ok(typeof ros1Path==='string'&&typeof ros2Path==='string'&&typeof receiptPath==='string');
assert.ok(isAbsolute(receiptPath)&&receiptPath.startsWith('/tmp/')&&!receiptPath.split('/').includes('..'),'fresh explicit /tmp receipt');
const schemaInput=await inputFile(fileURLToPath(new URL('../../sdk/abi/abi-v1.json',import.meta.url)));
const modelInput=await inputFile(fileURLToPath(import.meta.url));
const schema=JSON.parse(decoder.decode(schemaInput.bytes)),results=[],artifacts=[];
for(const [version,path] of [[1,ros1Path],[2,ros2Path]]) {
  const artifact=await inputFile(path),spec=schema.modules['uwvm_ros'+version+'_v1'];artifacts.push({...artifact.pin,backend:version});
  const module=new WebAssembly.Module(artifact.bytes),imports=WebAssembly.Module.imports(module);
  const expected=new Set([...spec.functions.map(f=>'uwvm_ros'+version+'_v1/'+f.name),...['fd_read','fd_write','proc_exit'].map(n=>'wasi_snapshot_preview1/'+n)]);
  assert.equal(imports.length,expected.size);for(const item of imports){assert.equal(item.kind,'function');assert.ok(expected.delete(item.module+'/'+item.name),'exact selected unique import');}assert.equal(expected.size,0);
  for(const item of cases(version,spec)) {
    const observed=item.run(module);results.push({backend:version,name:item.name,status:'PASS',exit:observed.exit,ROS_stub_calls:observed.calls.length,read_calls:observed.readCalls,write_calls:observed.writeCalls,output_bytes:observed.outputBytes});
  }
  const after=await inputFile(path);assert.deepEqual(after.pin,artifact.pin,'artifact bytes/identity preserved across serial cases');
}
assert.equal(results.length,80);
assert.deepEqual((await inputFile(schemaInput.pin.path)).pin,schemaInput.pin,'schema input preserved');
assert.deepEqual((await inputFile(modelInput.pin.path)).pin,modelInput.pin,'model source input preserved');
const receipt={format_version:1,status:'PASS_COMPILED_WASM_RELAY_GUEST_LOCAL_MODELS_ONLY',cases:80,cases_per_backend:40,
  scope:'Actual supplied C relay Wasm modules/Instances/exported memory with bounded mathematical ROS and WASI stubs only. No uwvm Runtime policy/authentication/native ROS delivery/side-effect result.',
  actual_Wasm_execution:true,actual_Runtime_execution:false,actual_native_ROS_execution:false,actual_process_or_socket_created:false,
  owner_supervision:'Root serial96MiB Node heap and owned process-group RSS/deadline controls; model itself creates no process.',
  artifacts,schema:schemaInput.pin,model:modelInput.pin,results};
const out=await open(receiptPath,'wx');try{await out.writeFile(JSON.stringify(receipt,null,2)+'\n');}finally{await out.close();}
console.log(JSON.stringify({status:receipt.status,cases:80,cases_per_backend:40,receipt:receiptPath}));
