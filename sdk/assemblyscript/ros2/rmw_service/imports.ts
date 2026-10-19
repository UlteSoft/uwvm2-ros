/** Explicit import adapter. Model substitutions are local test inputs only. */
import * as raw from './raw';

export abstract class ServiceImports {
  abstract version():u32;
  abstract activate(client:bool,source:u64,peer:u64,memory:u32,receipt:u64):u32;
  abstract inspect(source:u64,incarnation:u64,memory:u32,receipt:u64):u32;
  abstract submit(source:u64,incarnation:u64,memory:u32,input:u64,length:u64,ticket:u64):u32;
  abstract abandon(source:u64,incarnation:u64,ticket:u64):u32;
  abstract respond(source:u64,incarnation:u64,peer:u64,ticket:u64,memory:u32,input:u64,length:u64):u32;
  abstract peek(source:u64,incarnation:u64,writer:u32,memory:u32,packet:u64,capacity:u64):u32;
  abstract commit(source:u64,incarnation:u64,sequence:u64,reservation:u64):u32;
  abstract cancel(source:u64,incarnation:u64,sequence:u64,reservation:u64):u32;
  abstract available(source:u64,incarnation:u64,owner:u64,waitNs:u64,memory:u32,output:u64):u32;
  abstract retire(source:u64,incarnation:u64):u32;
}

/** Every production method calls the actual optional numeric namespace once. */
export class NativeServiceImports extends ServiceImports {
  version():u32{return raw.abi_version();}
  activate(client:bool,s:u64,p:u64,m:u32,o:u64):u32{return client?raw.client_activate(s,p,m,o):raw.service_activate(s,p,m,o);}
  inspect(s:u64,i:u64,m:u32,o:u64):u32{return raw.endpoint_inspect(s,i,m,o);}
  submit(s:u64,i:u64,m:u32,p:u64,n:u64,o:u64):u32{return raw.client_submit(s,i,m,p,n,o);}
  abandon(s:u64,i:u64,t:u64):u32{return raw.client_abandon(s,i,t);}
  respond(s:u64,i:u64,p:u64,t:u64,m:u32,b:u64,n:u64):u32{return raw.service_respond(s,i,p,t,m,b,n);}
  peek(s:u64,i:u64,w:u32,m:u32,p:u64,n:u64):u32{return raw.endpoint_peek(s,i,w,m,p,n);}
  commit(s:u64,i:u64,q:u64,r:u64):u32{return raw.endpoint_commit(s,i,q,r);}
  cancel(s:u64,i:u64,q:u64,r:u64):u32{return raw.endpoint_cancel(s,i,q,r);}
  available(s:u64,i:u64,a:u64,w:u64,m:u32,o:u64):u32{return raw.client_available(s,i,a,w,m,o);}
  retire(s:u64,i:u64):u32{return raw.endpoint_retire(s,i);}
}
