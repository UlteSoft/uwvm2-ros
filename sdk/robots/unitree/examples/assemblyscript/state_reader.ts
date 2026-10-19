// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Generic helper contract: the concrete GO/HG entry supplies its complete
// generated Codec<T>, exact family number/carrier count, actual maximum wire
// size and a noncapturing callback. initialize validates those bounded inputs
// before subscription. motorWord observes only a delivered finite state; it
// never parses native layout or manufactures an actuator/model mapping.
// Ordered caller protocol: validate the real binary and explicitly initialize
// its selected reactor runtime first; use one fresh instance for mode0 denial
// and another for mode1 positive behavior. Then initialize_case once, explicitly
// submit for topic RPC, poll_once at most4096 times, inspect a received value,
// and close_case after the polling/callback stack has returned. No retry loop,
// clock, thread, goroutine or native endpoint is created by this source itself.
// A successful factory or codec operation does not prove native transport.
// The trusted external Runtime policy supplies rights; this code grants none.
// Status convention:0 means the documented operation succeeded;11 is an idle
// poll;9 rejects unused/closed/repeated lifecycle operations;22 rejects invalid
// configuration or message data;90 is a capacity bound;95 rejects the selected
// ABI when checked;110 exhausts the finite poll budget. Original SDK errors are
// preserved. Rust can additionally return17 for a conflicting state borrow.
// Expected mode0 alias denial13 is translated to0 ONLY with denied_verified=1.
// All acquired owners are explicitly retired on ordinary completion/failure,
// attempting later closes after an earlier close error. Traps/panics/kills still
// require the external process owner to finish actual Runtime/Gateway cleanup.
// AssemblyScript build note: the generated complete family resolves through
// sdk/custom-types/typed-family/bounded_records in the owned stage. Bind the
// actual SDK abortGuest and call the actual _initialize export explicitly once.

/** Shared typed factory logic; the concrete GO/HG entry supplies its actual
 * generated Codec and noncapturing guest handler. Never parse native offsets. */
import { Codec } from '../../../../assemblyscript/codecs';
import { Life } from './lifecycle';
export class StateReader<T> {
  life:Life=new Life();words:Uint32Array=new Uint32Array(105);tick:u32=0;crc:u32=0;
  // Construct local codec and lifecycle owners without compiling a callback
  // that reads the concrete reader global while its initializer is pending.
  constructor(private family:u32,private motors:u32,private codec:Codec<T>,private maximum:i32){}
  // The concrete scalar entry binds its noncapturing handler only after the
  // reader global is declared. The real Subscription retains that handler.
  initialize(mode:u32,family:u32,reserved:u32,handler:(value:T)=>void):u32 {
    if(family!=this.family||reserved)return this.life.close(22);
    const first=this.life.begin(mode);if(first)return first;
    if(this.maximum<1||this.maximum>32768)return this.life.close(90);
    if(mode==0){const cap=this.life.node.open('unitree-lowstate');return cap.openStatus==13&&!cap.isOpen()?this.life.deny(13):this.life.close(cap.openStatus?cap.openStatus:22);}
    const sub=this.life.node.createSubscription<T>('unitree-lowstate',this.codec,handler,this.maximum);
    if(sub.status)return this.life.close(sub.status);
    const added=this.life.executor.add(sub);return added?this.life.close(added):0;
  }
  motorWord(slot:u32,field:u32):u32 {if(!this.life.received||slot>=this.motors||field>=3){this.life.close(22);return 0;}return this.words[slot*3+field];}
}
