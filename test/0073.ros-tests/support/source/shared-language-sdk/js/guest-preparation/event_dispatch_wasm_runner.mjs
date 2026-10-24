/** Run only the authored Rust production Stream/Executor cdylib contract.
 * Finite mocked ROS2 imports grant no real ROS rights and reach no middleware.
 * Root must provide an external pidfd/deadline/RSS/cgroup owner: synchronous
 * Wasm traps/loops cannot be bounded by an event-loop timer in this process.
 */
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
const filename=process.argv[2];assert.equal(process.argv.length,3);assert.ok(path.isAbsolute(filename));
const fd=fs.openSync(filename,fs.constants.O_RDONLY|fs.constants.O_NOFOLLOW|fs.constants.O_NONBLOCK);
let raw;
try{const before=fs.fstatSync(fd,{bigint:true});assert.ok(before.isFile()&&before.nlink===1n&&before.size>0n&&before.size<=67108864n);
  raw=fs.readFileSync(fd);const after=fs.fstatSync(fd,{bigint:true});
  for(const key of ['dev','ino','size','mtimeNs','ctimeNs','mode','nlink'])assert.equal(after[key],before[key]);assert.equal(BigInt(raw.length),before.size);
}finally{fs.closeSync(fd);}
const module=new WebAssembly.Module(raw),names=new Set(['abi_version','capability_open','capability_close','event_watch','event_poll_v1']);
for(const entry of WebAssembly.Module.imports(module))assert.ok(entry.module==='uwvm_ros2_v1'&&entry.kind==='function'&&names.has(entry.name));
let instance,opens=0,closes=0,watches=0,polls=0;const live=new Set();
function range(offset,length){assert.equal(typeof offset,'bigint');assert.equal(typeof length,'bigint');const memory=instance.exports.memory;
  assert.ok(memory instanceof WebAssembly.Memory&&offset>=0n&&length>=0n&&offset+length<=BigInt(memory.buffer.byteLength));return new Uint8Array(memory.buffer,Number(offset),Number(length));}
function put(offset,value){const bytes=range(offset,8n);new DataView(bytes.buffer,bytes.byteOffset,8).setBigUint64(0,value,true);}
instance=new WebAssembly.Instance(module,{uwvm_ros2_v1:{
  abi_version(){return 1;},
  capability_open(memory,alias,length,slot){assert.equal(memory,0);const name=new TextDecoder('utf-8',{fatal:true}).decode(range(alias,length));
    assert.equal(name,opens===0?'observer':'source');const id=BigInt(++opens);live.add(id);put(slot,id);return 0;},
  capability_close(id){assert.ok(live.delete(id));closes++;return 0;},
  event_watch(observer,source,correlation){assert.deepEqual([observer,source,correlation],[1n,2n,0n]);watches++;return 0;},
  event_poll_v1(observer,memory,out,capacity,slot){assert.equal(observer,1n);assert.equal(memory,0);assert.ok(capacity>=64n);polls++;put(slot,64n);
    const bytes=range(out,64n);bytes.fill(0);const view=new DataView(bytes.buffer,bytes.byteOffset,64);view.setUint32(0,0x45525755,true);view.setUint16(4,1,true);view.setUint16(6,1,true);bytes[8]=2;
    for(const [at,n] of [[16,2n],[32,1n],[40,17n],[48,19n]])view.setBigUint64(at,n,true);return 0;}
}});
assert.equal(opens,0);assert.equal(typeof instance.exports.contract,'function');assert.equal(instance.exports.contract(),0);
assert.deepEqual([opens,closes,watches,polls,live.size],[2,2,1,1,0]);
console.log(JSON.stringify({status:'PASS',scope:'actual Rust SDK Stream/Executor Wasm with finite fake imports; no ROS acceptance',opens,closes,watches,polls}));
