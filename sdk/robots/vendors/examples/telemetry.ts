/** Source-only AssemblyScript complete-record component.
 * ./backend reexports the REAL current SDK ROS1 OR ROS2 Node/Session/Executor/
 * NativeApi declarations. It is a source-bound one-line selection, never a mock.
 * ./sdk/codecs is the unchanged current SDK codec module. No model type/codec
 * stub is shipped: the entry must supply the real generated complete Codec<T>.
 * The callback must be noncapturing in AssemblyScript: a model-specific scalar
 * entry can refer to its actual global reader and call delivered after its full
 * value/native-oracle checks. No controller/state/actuator layout is guessed.
 */
import {Node,Session,Executor,NativeApi} from './backend';
import {Codec} from './sdk/codecs';
export class Telemetry<T> {
  session:Session=new Session(new NativeApi(),1);
  node:Node=this.session.node('vendor-typed-telemetry');
  executor:Executor=new Executor(1);
  started:bool=false;closed:bool=false;denied:bool=false;polling:bool=false;
  status:u32=0;cleanup:u32=0;polls:u32=0;received:u32=0;
  constructor(private codec:Codec<T>,private maximum:i32){}
  /** Only separately source-reviewed binding code calls delivered, after the
   * generated Subscription has validated/decode the entire typed message. */
  delivered():void{this.received++;}
  initialize(mode:u32,alias:string,handler:(value:T)=>void):u32 {
    if(this.started||this.closed)return 9;
    if(mode>1||!alias.length||alias.length>64||this.maximum<1||this.maximum>32768)return 22;
    this.started=true;if(this.session.status)return this.finish(this.session.status);
    if(mode==0){const cap=this.session.open(alias);this.denied=cap.openStatus==13&&!cap.isOpen();return this.finish(this.denied?0:cap.openStatus?cap.openStatus:22);}
    const sub=this.node.createSubscription<T>(alias,this.codec,handler,this.maximum);
    if(sub.status)return this.finish(sub.status);
    const code=this.executor.add(sub);return code?this.finish(code):0;
  }
  /** Local caller drives at most4096 passes;11 is idle, never a callback. */
  pollOnce():u32 {
    if(this.polling)return 17;if(!this.started||this.closed)return 9;
    if(this.polls++>=4096)return this.finish(110);
    this.polling=true;const before=this.received,codes=this.executor.spinOnce();this.polling=false;
    for(let i=0;i<codes.length;++i)if(codes[i]&&codes[i]!=11)return this.finish(codes[i]);
    if(this.status)return this.finish(this.status);return this.received>before?0:11;
  }
  /** Call only after the dispatch stack has returned; attempt every organizer.
   * A trap still requires original external process/Runtime cleanup. */
  close():u32{return this.polling?17:this.finish(this.status);}
  private finish(first:u32=0):u32 {
    if(this.closed)return first?first:9;this.closed=true;
    const local=this.executor.close();if(!this.cleanup&&local)this.cleanup=local;
    const nodes=this.node.close();for(let i=0;i<nodes.length;++i)if(!this.cleanup&&nodes[i])this.cleanup=nodes[i];
    const group=this.session.close();for(let i=0;i<group.length;++i)if(!this.cleanup&&group[i])this.cleanup=group[i];
    this.status=first?first:this.cleanup;return this.status;
  }
}
