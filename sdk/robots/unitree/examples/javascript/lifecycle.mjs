// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Helper ownership: Lifecycle owns the actual embedded Guest session, Node
// and Executor. begin creates local SDK objects, not an alias or native node.
// Node owns successful factories; Executor.close drops local registrations.
// close tries both even after an exception. The selected entry owns exports.
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
// Embedded-JS note: execute this only in the selected real QuickJS-in-Wasm Guest
// with the closed source module manifest and UTF-8 bootstrap. Host Node or a mock
// bridge cannot qualify these examples or their native transport.

/** Finite Guest-local lifecycle for the typed Unitree reactors.
 * Importing this module opens nothing. Only initialize_case calls the actual
 * embedded-Wasm SDK. Node/Executor are local owners, not native participants.
 * A single instance is single-use: reinitialization and double close return9.
 */
import { createGuestSession, Node, Executor } from '../../../../javascript/guest.mjs';
import { Status } from '../../../../javascript/raw.mjs';
export class Lifecycle {
  constructor() { this.started=false; this.closed=false; this.status=0; this.polls=0; this.received=0; this.denied=false; this.node=null; this.executor=null; this.session=null; }
  begin(mode) {
    if(this.started || this.closed) return 9;
    if(mode!==0 && mode!==1) return this.fail(22);
    this.started=true;
    try { this.session=createGuestSession(); if(this.session.version!==2)return this.fail(95); this.node=new Node(this.session,'unitree-typed-example'); this.executor=new Executor(); return 0; }
    catch(error) { return this.fail(error); }
  }
  code(error) { return Number.isInteger(error?.code)?error.code:typeof error==='number'?error:22; }
  finishDenied(error) { const code=this.code(error); this.denied=code===Status.DENIED; return this.close(this.denied?0:code); }
  fail(error) { return this.close(this.code(error)||22); }
  close(first=0) {
    if(this.closed)return first||9;
    this.closed=true;
    // Attempt every close even when an earlier operation/close failed. Node
    // owns successful factories; an unsuccessful SDK transaction rolled back.
    try { if(this.executor)this.executor.close(); } catch(error) { if(!first)first=this.code(error); }
    try { if(this.node)for(const code of this.node.close())if(!first && code)first=code; } catch(error) { if(!first)first=this.code(error); }
    this.status=first;return first;
  }
  poll() {
    if(!this.started || this.closed)return 9;
    if(this.polls++>=4096)return this.fail(110);
    try { const before=this.received,results=this.executor.spinOnce(); for(const code of results)if(code && code!==11)return this.fail(code); return this.received>before?0:11; }
    catch(error) { return this.fail(error); }
  }
}
