/** Exact basic WASIp1 stdio only, not a ROS or management bridge.
 * Managed byte arrays stay live for each synchronous copied import. No native
 * pointer, timer, clock, environment or network right is introduced here.
 */
@external("wasi_snapshot_preview1","fd_read") declare function fd_read(fd:u32,iovs:usize,count:u32,size:usize):u32;
@external("wasi_snapshot_preview1","fd_write") declare function fd_write(fd:u32,iovs:usize,count:u32,size:usize):u32;
@external("wasi_snapshot_preview1","proc_exit") export declare function proc_exit(code:u32):void;
export function credit():u32 {
  const b=new Uint8Array(1),iov=new Uint32Array(2),n=new Uint32Array(1);
  iov[0]=<u32>b.dataStart;iov[1]=1;n[0]=0xffffffff;
  const error=fd_read(0,iov.dataStart,1,n.dataStart);if(error)return error;
  return n[0]==1&&b[0]==82?0:22;
}
export function marker(text:string):u32 {
  const b=Uint8Array.wrap(String.UTF8.encode(text)),iov=new Uint32Array(2),n=new Uint32Array(1);
  iov[0]=<u32>b.dataStart;iov[1]=<u32>b.length;n[0]=0xffffffff;
  const error=fd_write(1,iov.dataStart,1,n.dataStart);return error?error:n[0]==<u32>b.length?0:107;
}
