/** Source-only complete typed telemetry over the genuine copied JS Guest SDK.
 * Stage ./sdk/roswasm.mjs and its entire original import closure from the actual
 * current MF. Use real QuickJS-in-Wasm CopiedTransport/Session. A host Node mock
 * or invented WebAssembly.Memory/Instance cannot qualify this component.
 */
import { Node, Session, RosError } from './sdk/roswasm.mjs';
export class Telemetry {
  constructor(session, codec, maximum, observe) {
    if (!(session instanceof Session) || !codec || typeof codec.decode!=='function' ||
        typeof codec.encode!=='function' || !Number.isInteger(maximum) || maximum<1 || maximum>32768 ||
        session.outputCapacity<maximum || typeof observe!=='function') throw new TypeError('Actual Session, complete generated codec/bound and model observer required');
    this.node=new Node(session,'vendor-typed-telemetry');this.codec=codec;this.maximum=maximum;this.observe=observe;
    this.started=false;this.closed=false;this.polling=false;this.denied=false;this.status=0;this.cleanup=[];this.polls=0;this.received=0;this.subscription=null;
  }
  initialize(mode,alias) {
    if(this.started||this.closed)return 9;
    if((mode!==0&&mode!==1)||typeof alias!=='string'||!alias.length||alias.length>64||alias.includes('\0'))return 22;
    this.started=true;
    if(mode===0){try{const cap=this.node.open(alias);const code=cap.close();if(code)this.cleanup.push(code);return this.finish(22);}catch(error){
      this.denied=error instanceof RosError&&error.code===13;return this.finish(this.denied?0:status(error));}}
    try {this.subscription=this.node.createSubscription(alias,this.codec,value=>{
      // decode is performed by the real typed Subscription before this handler.
      const code=this.observe(value);if(code!==0)throw new RosError(code,'model_observer');++this.received;
    });return 0;}catch(error){return this.finish(status(error));}
  }
  pollOnce() {
    if(this.polling)return 17;if(!this.started||this.closed)return 9;if(this.polls++>=4096)return this.finish(110);this.polling=true;
    try {const code=this.subscription.spinOnce();return code===0||code===11?code:this.finish(code);}catch(error){return this.finish(status(error));}finally{this.polling=false;}
  }
  /** Node.close attempts remaining owners even if subscription rollback failed.
   * Session storage belongs to the actual caller and must outlive this object. */
  finish(first=0) {
    if(this.closed)return first||9;this.closed=true;
    try{this.cleanup.push(...this.node.close());}catch(error){this.cleanup.push(...(Array.isArray(error.results)?error.results:[error]));}
    this.status=first||this.cleanup.map(status).find(code=>code!==0)||0;return this.status;
  }
  close(){return this.polling?17:this.finish(this.status);}
}
function status(error){if(Number.isInteger(error)&&error>=0)return error;return Number.isInteger(error?.code)&&error.code>=0?error.code:22;}
