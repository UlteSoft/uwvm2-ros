#!/usr/bin/env python3
"""Reproducible Wasm Action guests, checked against native Fibonacci witnesses."""
from pathlib import Path
import struct
ROOT=Path(__file__).resolve().parents[5]
SEQ=(0,1,1,2,3,5)
literal=lambda value:''.join('\\'+format(b,'02x') for b in value)

def common(v,imports,data):
 return f'''(module
 (import "uwvm_ros{v}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros{v}_v1" "capability_revoke" (func $revoke (param i64) (result i32)))
 {imports}
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))
 (memory (export "memory") 1)
 {data}
 (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))
 (func $delay (local $start i64)
  (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64)))
  (local.set $start (i64.load (i32.const 64)))
  (loop $wait (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
   (br_if $wait (i64.lt_u (i64.sub (i64.load (i32.const 72)) (local.get $start)) (i64.const 50000000)))))
'''

def rpc(v,name,local):return f'(import "uwvm_ros{v}_v1" "action_{name}" (func ${local} (param i64 i32 i64 i64 i64 i64 i64) (result i32)))'
def invoke(local,handle,offset=8192,length=8):return f'(call ${local} (i64.load (i32.const {handle})) (i32.const 0) (i64.const {offset}) (i64.const {length}) (i64.const 128) (i64.const 4096) (i64.const 16))'
def poll(name,handle,call):return f''' (func ${name} (local $start i64) (local $e i32)
  (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 80))) (local.set $start (i64.load (i32.const 80)))
  (loop $again
   (local.set $e {call})
   (if (local.get $e) (then
    (if (i32.ne (local.get $e) (i32.const 11)) (then (call $exit (local.get $e))))
    (call $delay) (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 88)))
    (if (i64.gt_u (i64.sub (i64.load (i32.const 88)) (local.get $start)) (i64.const 10000000000)) (then (call $exit (i32.const 110))))
    (br $again)))))
'''
def opened(offset,length,slot):return f'  (call $check (call $open (i32.const 0) (i64.const {offset}) (i64.const {length}) (i64.const {slot})))\n'
def reject(expr):return f'  (if {expr} (then (call $exit (i32.const 202))))\n'
for v in (1,2):
 imports='\n '.join(rpc(v,name,name) for name in ('send_goal','receive_feedback','receive_result'))
 guest=common(v,imports,'(data (i32.const 0) "goal") (data (i32.const 4096) "feedback") (data (i32.const 4104) "result") (data (i32.const 8208) "\\05\\00\\00\\00")')
 guest+=poll('feedback',24,invoke('receive_feedback',24))+poll('result',32,invoke('receive_result',32))
 guest+=' (func (export "_start") (local $n i32) (local $i i32)\n'+opened(0,4,8)+opened(4096,8,24)+opened(4104,6,32)
 guest+='  (call $check '+invoke('send_goal',8,8208,4)+')\n'+reject('(i64.ne (i64.load (i32.const 16)) (i64.const 8))')+reject('(i64.eqz (i64.load (i32.const 128)))')
 guest+='  (i64.store (i32.const 8192) (i64.load (i32.const 128)))\n  (call $feedback)\n'
 guest+=reject('(i64.ne (i64.load (i32.const 128)) (i64.load (i32.const 8192)))')
 guest+='  (local.set $n (i32.load (i32.const 136)))\n'+reject('(i32.or (i32.eqz (local.get $n)) (i32.gt_u (local.get $n) (i32.const 6)))')
 guest+=reject('(i64.ne (i64.load (i32.const 16)) (i64.add (i64.const 12) (i64.mul (i64.extend_i32_u (local.get $n)) (i64.const 4))))')
 for i,value in enumerate(SEQ):guest+=f'  (if (i32.gt_u (local.get $n) (i32.const {i})) (then (if (i32.ne (i32.load (i32.const {140+i*4})) (i32.const {value})) (then (call $exit (i32.const 203))))))\n'
 guest+='  (call $result)\n'+reject('(i64.ne (i64.load (i32.const 16)) (i64.const 37))')+reject('(i64.ne (i64.load (i32.const 128)) (i64.load (i32.const 8192)))')+reject('(i32.ne (i32.load8_u (i32.const 136)) (i32.const 2))')+reject('(i32.ne (i32.load (i32.const 137)) (i32.const 6))')
 for i,value in enumerate(SEQ):guest+=reject(f'(i32.ne (i32.load (i32.const {141+i*4})) (i32.const {value}))')
 guest+=reject('(i32.ne '+invoke('receive_result',32)+' (i32.const 9))')+'))\n'
 (ROOT/f'examples/ros{v}/action_client.wat').write_text(guest)
 imports=rpc(v,'advertise','advertise')+'\n '+rpc(v,'respond','respond')+f'\n (import "uwvm_ros{v}_v1" "event_poll" (func $event (param i64 i32 i64 i64 i64) (result i32)))'
 guest=common(v,imports,'(data (i32.const 0) "server") (data (i32.const 4096) "responder") (data (i32.const 8192) "'+literal(bytes(8)+bytes([1])+struct.pack('<I6i',6,*SEQ))+'")')
 guest+=poll('incoming',8,'(call $event (i64.load (i32.const 8)) (i32.const 0) (i64.const 128) (i64.const 4096) (i64.const 16))')
 guest+=' (func (export "_start")\n'+opened(0,6,8)+opened(4096,9,24)+'  (call $check '+invoke('advertise',8,8208,0)+')\n  (call $incoming)\n'
 guest+=reject('(i64.ne (i64.load (i32.const 16)) (i64.const 12))')+reject('(i64.eqz (i64.load (i32.const 128)))')+reject('(i32.ne (i32.load (i32.const 136)) (i32.const 5))')
 guest+='  (i64.store (i32.const 8192) (i64.load (i32.const 128)))\n  (call $check '+invoke('respond',24,8192,37)+')\n  (i32.store8 (i32.const 8200) (i32.const 2))\n  (call $check '+invoke('respond',24,8192,37)+')\n'
 guest+=reject('(i64.ne (i64.load (i32.const 16)) (i64.const 0))')+reject('(i32.ne '+invoke('respond',24,8192,37)+' (i32.const 9))')
 guest+='  (call $check (call $revoke (i64.load (i32.const 8))))\n'+reject('(i32.ne (call $event (i64.load (i32.const 8)) (i32.const 0) (i64.const 128) (i64.const 4096) (i64.const 16)) (i32.const 9))')
 guest+='  (call $delay) (call $delay)))\n'
 (ROOT/f'examples/ros{v}/action_server.wat').write_text(guest)
