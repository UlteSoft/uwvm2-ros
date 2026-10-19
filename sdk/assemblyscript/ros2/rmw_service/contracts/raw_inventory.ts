/** Compile/import inventory only. Do not invoke: numeric values grant nothing. */
import * as r from '../raw';
export function version():u32{return r.abi_version();}
export function activateClient(s:u64,p:u64,o:u64):u32{return r.client_activate(s,p,0,o);}
export function activateService(s:u64,p:u64,o:u64):u32{return r.service_activate(s,p,0,o);}
export function inspect(s:u64,i:u64,o:u64):u32{return r.endpoint_inspect(s,i,0,o);}
export function submit(s:u64,i:u64,p:u64,n:u64,o:u64):u32{return r.client_submit(s,i,0,p,n,o);}
export function abandon(s:u64,i:u64,t:u64):u32{return r.client_abandon(s,i,t);}
export function respond(s:u64,i:u64,p:u64,t:u64,b:u64,n:u64):u32{return r.service_respond(s,i,p,t,0,b,n);}
export function peek(s:u64,i:u64,p:u64,n:u64):u32{return r.endpoint_peek(s,i,16,0,p,n);}
export function commit(s:u64,i:u64,q:u64,n:u64):u32{return r.endpoint_commit(s,i,q,n);}
export function cancel(s:u64,i:u64,q:u64,n:u64):u32{return r.endpoint_cancel(s,i,q,n);}
export function available(s:u64,i:u64,a:u64,w:u64,o:u64):u32{return r.client_available(s,i,a,w,0,o);}
export function retire(s:u64,i:u64):u32{return r.endpoint_retire(s,i);}
