/** Optional AssemblyScript vision preload. Only scalar values cross instances;
 * no managed string/object/reference is passed to C, C++ or Rust. Use the stub
 * runtime with explicit memory bounds; inspect the actual binary's imports.
 */
export function vision_initialize(): u32 {return 0;}
export function vision_clearance_mm(a:i32,b:i32,c:i32,d:i32):i32 {
  if(a<0 || a>5000 || b<0 || b>5000 || c<0 || c>5000 || d<0 || d>5000) return i32.MIN_VALUE+22;
  let value:i32=a<b?a:b; if(c<value) value=c; if(d<value) value=d; return value;
}
/** Retained only if the actual compiler emits an abort import. This local
 * function traps; it never adds an env.abort host import or IO authority.
 * Four arguments match the SDK's existing AssemblyScript abort adapter.
 */
export function abortGuest(message:usize,file:usize,line:u32,column:u32):void {unreachable();}
