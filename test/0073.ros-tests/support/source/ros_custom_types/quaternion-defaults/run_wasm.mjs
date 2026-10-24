/** Finite value-only Wasm fixture entry, invoked ONLY by an external owned lane.
 * The caller pins Node, this source, full validated artifact and cgroup before
 * launch; owns its birth/pidfd, memory, time and normal terminal preservation.
 * There is no ROS bridge, filesystem preopen, native identity or Guest grant.
 */
import fs from 'node:fs';
import crypto from 'node:crypto';
import {WASI} from 'node:wasi';
const [mode,file,sha]=process.argv.slice(2);
if(process.argv.length!==5||!['c','cpp','tinygo','assemblyscript'].includes(mode)||!file?.startsWith('/')||!/^[0-9a-f]{64}$/.test(sha))throw new Error('exact owned fixture mode/path/SHA required');
let bytes;const fd=fs.openSync(file,fs.constants.O_RDONLY|fs.constants.O_NOFOLLOW|fs.constants.O_NONBLOCK);
try{
 const before=fs.fstatSync(fd,{bigint:true});if(!before.isFile()||before.size<8n||before.size>16777216n)throw new Error('bounded ordinary Wasm required');
 bytes=Buffer.alloc(Number(before.size));let at=0;while(at<bytes.length){const n=fs.readSync(fd,bytes,at,bytes.length-at,null);if(!n)throw new Error('short Wasm');at+=n;}
 const after=fs.fstatSync(fd,{bigint:true}),named=fs.lstatSync(file,{bigint:true});
 for(const key of ['dev','ino','size','mtimeNs','ctimeNs','mode','nlink'])if(before[key]!==after[key]||before[key]!==named[key])throw new Error('Wasm changed');
 if(crypto.createHash('sha256').update(bytes).digest('hex')!==sha)throw new Error('actual artifact SHA differs');
}finally{fs.closeSync(fd);}
const module=new WebAssembly.Module(bytes); // Complete independent validator is a caller prerequisite too.
const imports=WebAssembly.Module.imports(module);
if(mode==='assemblyscript'){
 if(imports.length)throw new Error('pure AS fixture must have ZERO imports');
 const instance=new WebAssembly.Instance(module,{});
 if(typeof instance.exports._start!=='function'||typeof instance.exports.completedChecks!=='function')throw new Error('complete fixture exports required');
 instance.exports._start();if(instance.exports.completedChecks()!==8)throw new Error('all eight actual AS checks required');
}else{
 const allowed=new Set(['args_get','args_sizes_get','environ_get','environ_sizes_get','fd_write','fd_fdstat_get','proc_exit','clock_time_get','random_get','fd_seek','fd_close','poll_oneoff','sched_yield']);
 if(imports.some(row=>row.kind!=='function'||row.module!=='wasi_snapshot_preview1'||!allowed.has(row.name)))throw new Error('closed basic-WASI-only value fixture imports required');
 const wasi=new WASI({version:'preview1',args:[],env:{},preopens:{},returnOnExit:true});
 const instance=new WebAssembly.Instance(module,{wasi_snapshot_preview1:wasi.wasiImport});
 if(wasi.start(instance)!==0)throw new Error('actual value fixture returned failure');
}
process.stdout.write(JSON.stringify({kind:'actual-generated-value-fixture',mode,sha256:sha,imports,ROS_executed:false,native_identity_qualified:false})+'\n');
