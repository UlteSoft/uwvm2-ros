/** Real typed AssemblyScript SDK delivery shared by four explicit entry files.
 * The controller owns all clocks, native matching, process gates and policy.
 * Each R performs one real typed spin; wrong bytes, duplicate callbacks, default
 * denial sentinel changes or unsuccessful cleanup prevent the PASS marker.
 */
import { Api, Status } from '../../../../sdk/assemblyscript/api';
import { Node } from '../../../../sdk/assemblyscript/roswasm';
import { StringCodec } from '../../../../sdk/assemblyscript/codecs';
@external('wasi_snapshot_preview1','fd_read') declare function fdRead(fd:u32,vector:usize,count:u32,actual:usize):u32;
@external('wasi_snapshot_preview1','fd_write') declare function fdWrite(fd:u32,vector:usize,count:u32,actual:usize):u32;
@external('wasi_snapshot_preview1','proc_exit') declare function exit(code:u32):void;
function check(value:bool):void {if(!value){exit(Status.Invalid);unreachable();}}
function ok(status:u32):void {if(status){exit(status);unreachable();}}
function emit(text:string):void {
    const bytes=String.UTF8.encode(text),vector=new Uint32Array(2),count=new Uint32Array(1);vector[0]=<u32>changetype<usize>(bytes);vector[1]=<u32>bytes.byteLength;count[0]=u32.MAX_VALUE;
    ok(fdWrite(1,vector.dataStart,1,count.dataStart));check(count[0]==vector[1]);
}
function gate(expected:u8):void {
    const bytes=new Uint8Array(1),vector=new Uint32Array(2),count=new Uint32Array(1);vector[0]=<u32>bytes.dataStart;vector[1]=1;count[0]=u32.MAX_VALUE;
    ok(fdRead(0,vector.dataStart,1,count.dataStart));check(count[0]==1&&bytes[0]==expected);
}
function denied(api:Api,alias:string):void {
    const text=String.UTF8.encode(alias),slot=new Uint8Array(8);store<u64>(slot.dataStart,0x8877665544332211);
    check(api.open(0,<u64>changetype<usize>(text),<u64>text.byteLength,<u64>slot.dataStart)==Status.Denied&&load<u64>(slot.dataStart)==0x8877665544332211);
}
function refusal(api:Api,handle:u64,expected:u32):void {
    const output=new Uint8Array(64),slot=new Uint8Array(8);output.fill(0xa5);store<u64>(slot.dataStart,u64.MAX_VALUE);
    check(api.poll(handle,0,<u64>output.dataStart,64,<u64>slot.dataStart)==expected&&load<u64>(slot.dataStart)==u64.MAX_VALUE);for(let i=0;i<64;++i)check(output[i]==0xa5);
}
let delivered:u32=0;
/** Copied typed callback runs on the Guest poll thread, never native DDS. */
function receive(value:string):void {check(value=='roswasm-js-relay-in'&&delivered==0);delivered++;}
/** Executable flow: default empty policy or one exact bounded pub/sub exchange. */
export function run(api:Api,absent:bool):void {
    check(api.version()==1);if(absent){denied(api,'pub');denied(api,'sub');denied(api,'ungranted');emit('PASS_SDK_DEFAULT_DENIED\n');exit(0);return;}
    const node=new Node(api,'sdk-delivery'),pub=node.createPublisher<string>('pub',new StringCodec()),sub=node.createSubscription<string>('sub',new StringCodec(),receive,64);
    ok(pub.capability.openStatus);ok(sub.status);denied(api,'ungranted');refusal(api,pub.capability.handle,Status.Denied);
    ok(pub.publish('warmup-js-relay'));emit('SDK_ENDPOINTS_CREATED\n');gate(80);ok(pub.publish('roswasm-js-relay-out'));emit('SDK_FINAL_SENT\n');
    for(let i=0;i<200&&delivered==0;++i){gate(82);const status=sub.spinOnce();if(status==Status.Again)emit('SDK_POLL_AGAIN\n');else ok(status);}
    check(delivered==1);emit('SDK_DELIVERED\n');const oldPub=pub.capability.handle,oldSub=sub.capability.handle;ok(sub.close());ok(pub.close());
    refusal(api,oldPub,Status.BadHandle);refusal(api,oldSub,Status.BadHandle);const cleanup=node.close();for(let i=0;i<cleanup.length;++i)ok(cleanup[i]);
    emit('PASS_SDK_DELIVERY\n');exit(0);
}
