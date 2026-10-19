/**
 * @file relay.mjs
 * @brief Copied, serialized transport to one explicitly supplied command relay.
 *
 * This module does not instantiate Wasm, provide ROS imports, open Gateway IPC,
 * create a process, or grant authority. The supervisor binds the actual Worker
 * image/PID/Instance/policy and supplies exclusive binary Node streams. Keep
 * Runtime stderr separate from the readable binary stdout stream. See RELAY.md.
 */
import { specification, u32, u64, registerCallPreparation, delegateCallEntry, callEntryAttempted } from './raw.mjs';

export const RelayProtocol = Object.freeze({ magic:0x524a5755, version:1, headerBytes:24,
  hello:1, call:2, result:3, shutdown:4, fatal:5, noFunction:65535,
  helloBytes:80, guardBytes:16, defaultCapacity:4080, outputFill:0xa5 });
const U64_MAX=0xffffffffffffffffn;
// Exact generated canonical typed-table digests are checked below. They are
// compatibility metadata, not a claim about the process reading the pipe.
const TABLE_DIGESTS = Object.freeze({
  1:'83e55c0aefe30aa6625447e95f88a2ffad601a686924196090df4df38a5617dd', 2:'f1f2ef80fee1a40b3de7daa584d497431ef0439f3bdd3553a2c384c69139ec68'
});

/** Local protocol/lifetime failure, never a fabricated Runtime errno. */
export class RelayError extends Error {
  constructor(message) { super(message); this.name='RelayError'; }
}
/** Reject a concurrent borrow before writing a frame; callers serialize awaits. */
export class RelayBusyError extends RelayError {
  constructor() { super('One relay request may be outstanding'); this.name='RelayBusyError'; }
}
function require(condition,message) { if(!condition) throw new RelayError(message); }
function bytes(value) {
  if(!(value instanceof Uint8Array)) throw new TypeError('Uint8Array bytes required');
  return value;
}
function view(value) { return new DataView(value.buffer,value.byteOffset,value.byteLength); }
function hex(value) { return Array.from(value,b=>b.toString(16).padStart(2,'0')).join(''); }
function form(entry) {
  const count=entry.parameters.length;
  if(entry.name==='event_watch') return 'watch';
  if(entry.name==='event_ready') return 'ready';
  if(entry.name==='service_submit') return 'submit';
  if(count===0) return 'version';
  if(count===1) return 'handle';
  if(count===4) return entry.name==='capability_open'?'open':'write';
  if(count===5) return 'poll';
  if(count===7) return 'rpc';
  throw new RelayError('Unsupported original ABI form');
}

/** Encode only the closed little-endian header. All i64 values remain BigInt. */
export function encodeRelayFrame({version,kind,sequence,index,arity},body=new Uint8Array()) {
  specification(version);u64(sequence);bytes(body);
  if(!Number.isInteger(kind)||kind<1||kind>5||!Number.isInteger(index)||index<0||index>65535||
     !Number.isInteger(arity)||arity<0||arity>7||body.length>0xffffffff) throw new RangeError('Invalid relay header');
  const result=new Uint8Array(24+body.length),data=view(result);
  data.setUint32(0,RelayProtocol.magic,true);data.setUint16(4,1,true);
  result[6]=version;result[7]=kind;data.setBigUint64(8,sequence,true);
  data.setUint16(16,index,true);result[18]=arity;result[19]=0;
  data.setUint32(20,body.length,true);result.set(body,24);return result;
}

/** Incremental bounded decoder with one allocation per body, not concatenation
 * on every fragment. A chunk may end anywhere in a header or body. Complete
 * frames are copied owned records. No unbounded queue or UTF-8 line scanner is
 * involved. The owner enforces HELLO/request/sequence ordering separately.
 */
export class RelayFrameDecoder {
  constructor(version,maxBody,onFrame) {
    specification(version);
    if(!Number.isSafeInteger(maxBody)||maxBody<80||maxBody>4194324||typeof onFrame!=='function')
      throw new TypeError('Explicit bounded frame decoder required');
    this.version=version;this.maxBody=maxBody;this.onFrame=onFrame;
    this.header=new Uint8Array(24);this.headerUsed=0;this.body=null;this.bodyUsed=0;this.fields=null;this.failed=false;
  }
  feed(chunk) {
    bytes(chunk);require(!this.failed,'Retired frame decoder');
    let offset=0;
    try {
      while(offset<chunk.length) {
        if(this.headerUsed<24) {
          const count=Math.min(24-this.headerUsed,chunk.length-offset);
          this.header.set(chunk.subarray(offset,offset+count),this.headerUsed);this.headerUsed+=count;offset+=count;
          if(this.headerUsed<24) continue;
          const data=view(this.header),length=data.getUint32(20,true);
          require(data.getUint32(0,true)===RelayProtocol.magic&&data.getUint16(4,true)===1&&
            this.header[6]===this.version&&this.header[19]===0,'Closed relay magic/version/backend/reserved required');
          require(this.header[7]>=1&&this.header[7]<=5&&this.header[18]<=7&&length<=this.maxBody,'Bounded relay kind/arity/body required');
          this.fields={version:this.version,kind:this.header[7],sequence:data.getBigUint64(8,true),
            index:data.getUint16(16,true),arity:this.header[18]};
          this.body=new Uint8Array(length);this.bodyUsed=0;
        }
        const count=Math.min(this.body.length-this.bodyUsed,chunk.length-offset);
        this.body.set(chunk.subarray(offset,offset+count),this.bodyUsed);this.bodyUsed+=count;offset+=count;
        if(this.bodyUsed===this.body.length) {
          const frame={...this.fields,body:this.body};
          this.headerUsed=0;this.body=null;this.bodyUsed=0;this.fields=null;
          this.onFrame(frame);
        }
      }
    } catch(error) { this.failed=true;throw error; }
  }
  /** EOF is complete only at a frame boundary; lifetime rules still apply. */
  finish() { require(!this.failed&&this.headerUsed===0&&this.body===null,'Incomplete or failed relay frame at EOF'); }
}

function layoutFromHello(frame,version,inputCapacity,outputCapacity) {
  require(frame.kind===RelayProtocol.hello&&frame.sequence===0n&&frame.index===65535&&frame.arity===0&&frame.body.length===80,
    'Exactly one initial HELLO required');
  const data=view(frame.body),spec=specification(version);
  require(data.getUint32(0,true)===1&&data.getUint32(4,true)===spec.functions.length&&
    hex(frame.body.subarray(8,40))===TABLE_DIGESTS[version],'Exact selected ABI table required');
  const memoryBytes=data.getBigUint64(40,true),input=data.getBigUint64(48,true),slot=data.getBigUint64(56,true),output=data.getBigUint64(64,true);
  const incoming=data.getUint32(72,true),outgoing=data.getUint32(76,true);
  require(incoming===inputCapacity&&outgoing===outputCapacity&&memoryBytes>0n&&memoryBytes<=0x100000000n&&input>=16n,
    'Explicit wasm32 scratch capacities and bounds required');
  require(slot===input+BigInt(incoming)&&output===slot+8n&&output<=memoryBytes&&BigInt(outgoing)+16n<=memoryBytes-output,
    'Disjoint reserved scratch and guards required');
  return Object.freeze({memoryBytes,input,slot,output,inputCapacity:incoming,outputCapacity:outgoing});
}

/** Validate an original ABI call against exclusively reserved scratch, then copy
 * its input. Arbitrary memory addresses and unbounded payloads are not exposed.
 * Local validation throws before transport dispatch and is not a Runtime errno.
 */
function callBody(entry,args,input,slotSeed,snapshot,layout) {
  if(!Array.isArray(args)||args.length!==entry.parameters.length) throw new TypeError('Exact ABI arity required');
  bytes(input);u64(slotSeed);
  if(!Number.isSafeInteger(snapshot)||snapshot<0||snapshot>layout.outputCapacity||input.length>layout.inputCapacity)
    throw new RangeError('Reserved scratch exceeded');
  for(let i=0;i<args.length;i++) (entry.parameters[i].type==='i64'?u64:u32)(args[i]);
  const f=form(entry),length=BigInt(input.length);
  if(f==='version'||f==='handle') require(input.length===0,'Scalar call has no transferred input');
  if(f==='open') require(args[0]===0&&args[1]===layout.input&&args[2]===length&&args[3]===layout.slot,'Reserved open ranges required');
  if(f==='write') require(args[1]===0&&args[2]===layout.input&&args[3]===length,'Reserved write range required');
  if(f==='poll') require(input.length===0&&args[1]===0&&args[2]===layout.output&&args[3]<=BigInt(layout.outputCapacity)&&args[4]===layout.slot,'Reserved poll ranges required');
  if(f==='rpc') require(args[1]===0&&args[2]===layout.input&&args[3]===length&&args[4]===layout.output&&args[5]<=BigInt(layout.outputCapacity)&&args[6]===layout.slot,'Reserved request/response ranges required');
  if(f==='watch') require(input.length===0&&snapshot===0,'Observer association transfers no payload');
  if(f==='ready') require(input.length===0&&args[1]===0&&args[2]===layout.output&&snapshot===32&&layout.outputCapacity>=32,'Exact reserved readiness output required');
  if(f==='submit') require(args[2]===0&&args[3]===layout.input&&args[4]===length&&args[5]===layout.slot&&snapshot===0,'Exact service admission input/ID slot required');
  const copied=input.slice(),body=new Uint8Array(8*args.length+12+copied.length),data=view(body);
  for(let i=0;i<args.length;i++) data.setBigUint64(8*i,typeof args[i]==='bigint'?args[i]:BigInt(args[i]),true);
  data.setBigUint64(8*args.length,slotSeed,true);data.setUint32(8*args.length+8,snapshot,true);body.set(copied,8*args.length+12);
  return body;
}

/** Explicit trusted-stream attachment; never starts a Worker or grants rights.
 * The writable stream must be the sole request writer and readable the sole
 * binary response reader. The supervisor owns process/PID/policy/cgroup proofs.
 */
export class RelayChannel {
  static async attach({version,readable,writable,inputCapacity=4080,outputCapacity=4080,timeoutMs=10000}) {
    const channel=new RelayChannel(version,readable,writable,inputCapacity,outputCapacity,timeoutMs);
    await channel.ready;return channel;
  }
  constructor(version,readable,writable,inputCapacity,outputCapacity,timeoutMs) {
    this.spec=specification(version);this.version=version;
    require(readable&&typeof readable.on==='function'&&typeof readable.pause==='function'&&writable&&typeof writable.write==='function',
      'Exclusive readable/writable Node streams required');
    require(Number.isSafeInteger(inputCapacity)&&inputCapacity>0&&inputCapacity<=4193770&&
      Number.isSafeInteger(outputCapacity)&&outputCapacity>0&&outputCapacity<=4194288&&
      Number.isSafeInteger(timeoutMs)&&timeoutMs>0&&timeoutMs<=600000,'Explicit bounded capacities/deadline required');
    this.readable=readable;this.writable=writable;this.inputCapacity=inputCapacity;this.outputCapacity=outputCapacity;
    this.timeoutMs=timeoutMs;this.sequence=0n;this.pending=null;this.state='hello';this.failure=null;this.layout=null;
    this.ready=new Promise((resolve,reject)=>{this.resolveReady=resolve;this.rejectReady=reject;});
    this.timer=setTimeout(()=>this.fail(new RelayError('Relay HELLO deadline exceeded')),timeoutMs);
    this.decoder=new RelayFrameDecoder(version,Math.max(80,20+outputCapacity),frame=>this.accept(frame));
    readable.on('data',chunk=>{try {this.decoder.feed(chunk);} catch(error) {this.fail(error);}});
    readable.on('error',error=>this.fail(new RelayError('Relay input stream failed')));
    writable.on('error',error=>this.fail(new RelayError('Relay output stream failed')));
    readable.on('end',()=>{
      try {this.decoder.finish();require(this.state==='closed'||
        (this.state==='settling'&&this.pending?.shutdown&&this.pending.responded),'Relay EOF without acknowledged shutdown');}
      catch(error) {this.fail(error);}
    });
  }
  fail(error) {
    if(this.failure) return;
    this.failure=error instanceof RelayError?error:new RelayError('Relay transport failed');this.state='failed';
    clearTimeout(this.timer);this.readable.pause();
    this.rejectReady(this.failure);
    if(this.pending) {clearTimeout(this.pending.timer);this.pending.reject(this.failure);this.pending.rejectWrite?.(this.failure);this.pending=null;}
  }
  accept(frame) {
    require(!this.failure,'Retired relay transport');
    if(this.state==='hello') {
      this.layout=layoutFromHello(frame,this.version,this.inputCapacity,this.outputCapacity);
      clearTimeout(this.timer);this.state='ready';this.resolveReady(this);return;
    }
    const pending=this.pending;
    require(pending&&this.state==='waiting','Unsolicited or duplicate relay response');
    if(frame.kind===RelayProtocol.fatal) {
      require(frame.index===65535&&frame.arity===0&&frame.body.length===4,'Closed fatal transport record required');
      const code=view(frame.body).getUint32(0,true);require(code>=1&&code<=7,'Fixed public fatal code required');
      throw new RelayError('Relay refused protocol code '+code);
    }
    require(frame.kind===RelayProtocol.result&&frame.sequence===pending.sequence&&frame.index===pending.index&&frame.arity===pending.arity,
      'Exact matching relay result required');
    require(frame.body.length===20+pending.snapshot,'Exact bounded result snapshot required');
    const data=view(frame.body),status=data.getUint32(0,true),slot=data.getBigUint64(4,true);
    require(data.getUint32(12,true)===pending.snapshot&&data.getUint32(16,true)===1,'Exact snapshot count and guards required');
    const result=Object.freeze({status,slot,slotBytes:frame.body.slice(4,12),snapshot:frame.body.slice(20),sequence:frame.sequence});
    if(pending.shutdown) require(status===0&&slot===0n&&pending.snapshot===0,'Exact shutdown acknowledgement required');
    pending.responded=true;this.state='settling';pending.resolve(result);
  }
  async send(kind,index,arity,body,snapshot,shutdown=false) {
    return prepareRelaySend(this,kind,index,arity,body,snapshot,shutdown)(null);
  }
  /** Invoke any one of the complete selected ABI functions. Slot/snapshot seeds
   * support explicit sentinel testing; they are copied owned transport data.
   */
  async call(name,args,options) { return prepareRelayCall(this,name,args,options)(null); }
  /** Explicit transport shutdown after reporting all SDK handle cleanup results.
   * This awaits the protocol acknowledgement; only the supervisor can attest
   * the actual Worker exit/owned cleanup. Accidental EOF is never success.
   */
  async shutdown() { return this.send(RelayProtocol.shutdown,65535,0,new Uint8Array(),0,true); }
}

/** All form/scalar/capacity checks and the real writable delegate are captured
 * before checked retirement may enter. A busy refusal has sent no frame.
 */
function prepareRelayCall(channel,name,args,{input=new Uint8Array(),slotSeed=U64_MAX,snapshot=0}={}) {
  require(channel instanceof RelayChannel,'Actual relay channel required');
  if(channel.pending||channel.state==='waiting') throw new RelayBusyError();
  const index=channel.spec.functions.findIndex(entry=>entry.name===name);
  if(index<0) throw new RelayError('Function absent from selected ROS backend');
  require(channel.layout&&channel.state==='ready','Attached relay required');
  const entry=channel.spec.functions[index],body=callBody(entry,args,input,slotSeed,snapshot,channel.layout);
  return prepareRelaySend(channel,RelayProtocol.call,index,entry.parameters.length,body,snapshot,false);
}
function prepareRelaySend(channel,kind,index,arity,body,snapshot,shutdown=false) {
  require(channel instanceof RelayChannel,'Actual relay channel required');
  if(channel.pending||channel.state==='waiting') throw new RelayBusyError();
  require(channel.state==='ready'&&!channel.failure,'Ready relay required');
  require(channel.sequence<U64_MAX,'Relay sequence exhausted');
  const previous=channel.sequence,sequence=previous+1n;
  const frame=encodeRelayFrame({version:channel.version,kind,sequence,index,arity},body);
  const writable=channel.writable,write=writable?.write;
  require(typeof write==='function','Actual relay writable delegate required');
  const writeArgs=[frame,null];
  return async ticket=>{
    if(channel.pending||channel.state==='waiting') throw new RelayBusyError();
    require(channel.state==='ready'&&!channel.failure&&channel.sequence===previous,'Unchanged ready relay reservation required');
    let resolve,reject,resolveWrite,rejectWrite;
    const answer=new Promise((yes,no)=>{resolve=yes;reject=no;});
    const outgoing=new Promise((yes,no)=>{resolveWrite=yes;rejectWrite=no;});
    // Install handlers for both outcomes before attempting the captured write.
    const complete=Promise.all([outgoing,answer]);
    const pending={sequence,index,arity,snapshot,shutdown,resolve,reject,rejectWrite,responded:false,timer:null};
    writeArgs[1]=error=>{if(error)rejectWrite(new RelayError('Relay frame write failed'));else resolveWrite();};
    pending.timer=setTimeout(()=>channel.fail(new RelayError('Relay call deadline exceeded; outcome unknown')),channel.timeoutMs);
    try {
      channel.sequence=sequence;channel.pending=pending;channel.state='waiting';
      delegateCallEntry(ticket,write,writable,writeArgs);
      const [,result]=await complete;
      if(channel.failure) throw channel.failure;
      require(channel.pending===pending&&pending.responded,'Retained complete request required');
      clearTimeout(pending.timer);channel.pending=null;channel.state=shutdown?'closed':'ready';return result;
    } catch(error) {
      // A checked ticket decides from its private delegate record, not from the
      // error class. The same RelayBusyError thrown by write is already unknown.
      if(ticket!==null&&!callEntryAttempted(ticket)) {
        clearTimeout(pending.timer);
        if(channel.pending===pending){channel.pending=null;channel.sequence=previous;channel.state='ready';}
        rejectWrite(error);reject(error);try{await complete;}catch{}throw error;
      }
      channel.fail(error);try{await complete;}catch{}throw channel.failure;
    }
  };
}
registerCallPreparation(RelayChannel.prototype.call,(channel,[name,args,options])=>prepareRelayCall(channel,name,args,options));
registerCallPreparation(RelayChannel.prototype.send,(channel,[kind,index,arity,body,snapshot,shutdown=false])=>prepareRelaySend(channel,kind,index,arity,body,snapshot,shutdown));
