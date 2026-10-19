// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Helper ownership: Life owns actual NativeApi/Node and one-registration
// Executor. Constructing their local objects opens no alias. begin verifies
// ABI compatibility before factories. close retires registrations then every
// Node-owned capability and preserves the first error. Entry owns exports.
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

/** One explicit Guest-local owner. No native ROS setup occurs during module
 * evaluation. Runtime aliases remain the only authority; every close is tried
 * after a failed operation, preserving its first error. No retry or timer. */
import { Node, NativeApi, Executor } from '../../../../assemblyscript/ros2/roswasm';
export class Life {
  node:Node=new Node(new NativeApi(),'unitree-typed-example');
  executor:Executor=new Executor(1);
  started:bool=false;closed:bool=false;denied:bool=false;
  status:u32=0;polls:u32=0;received:u32=0;
  begin(mode:u32):u32 {
    if(this.started||this.closed)return 9;
    if(mode>1)return this.close(22);this.started=true;
    if(this.node.api.version()!=1)return this.close(95);return 0;
  }
  close(first:u32=0):u32 {
    if(this.closed)return first?first:9;this.closed=true;
    const local=this.executor.close();if(!first)first=local;
    const results=this.node.close();for(let i=0;i<results.length;++i)if(!first&&results[i])first=results[i];
    this.status=first;return first;
  }
  deny(code:u32):u32 {this.denied=code==13;return this.close(this.denied?0:code);}
  poll():u32 {
    if(!this.started||this.closed)return 9;if(this.polls++>=4096)return this.close(110);
    const before=this.received,results=this.executor.spinOnce();
    for(let i=0;i<results.length;++i)if(results[i]&&results[i]!=11)return this.close(results[i]);
    if(this.status)return this.close(this.status);return this.received>before?0:11;
  }
}
