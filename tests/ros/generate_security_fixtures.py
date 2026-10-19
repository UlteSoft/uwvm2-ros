#!/usr/bin/env python3
"""Security guests assert exact host denials before any external ROS operation."""
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def rpc(v,name):return f'(import "uwvm_ros{v}_v1" "{name}" (func $op (param i64 i32 i64 i64 i64 i64 i64) (result i32)))'
def base(v,imports,alias='pub',memory64=False):
 a='i64' if memory64 else 'i32'
 return f'''(module
 (import "uwvm_ros{v}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 {imports}
 (memory (export "memory") {'i64 ' if memory64 else ''}1)
 (data ({a}.const 0) "{alias}") (data ({a}.const 8192) "\\05\\00\\00\\00hello")
 (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))
 (func (export "_start")
  (call $check (call $open (i32.const 0) (i64.const 0) (i64.const {len(alias)}) (i64.const 8)))
'''
def deny(call,expected):return f'  (if (i32.ne {call} (i32.const {expected})) (then (call $exit (i32.const 210))))\n'
for v in (1,2):
 pub=f'(import "uwvm_ros{v}_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))'
 p=lambda h,offset=8192,length=9:f'(call $publish {h} (i32.const 0) (i64.const {offset}) (i64.const {length}))'
 for name,alias,imp,call in [('topic_denied','sub',pub,p('(i64.load (i32.const 8))')),
  ('service_denied','server',rpc(v,'service_call'),'(call $op (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const 0) (i64.const 128) (i64.const 4096) (i64.const 16))'),
  ('graph_denied','cap',rpc(v,'graph_inspect_node'),'(call $op (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const 0) (i64.const 128) (i64.const 4096) (i64.const 16))'),
  ('action_denied','result',rpc(v,'action_send_goal'),'(call $op (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const 4) (i64.const 128) (i64.const 4096) (i64.const 16))')]:
  s=base(v,imp,alias)+'  (i64.store (i32.const 16) (i64.const -1)) (i64.store (i32.const 128) (i64.const -1))\n'+deny(call,13)
  s+='  (if (i32.or (i64.ne (i64.load (i32.const 16)) (i64.const -1)) (i64.ne (i64.load (i32.const 128)) (i64.const -1))) (then (call $exit (i32.const 211))))) )\n'
  (ROOT/f'examples/ros{v}/{name}.wat').write_text(s)
 imports=pub+f'\n (import "uwvm_ros{v}_v1" "capability_close" (func $close (param i64) (result i32)))\n (import "uwvm_ros{v}_v1" "capability_revoke" (func $revoke (param i64) (result i32)))'
 s=base(v,imports)+deny(p('(i64.const 9223372036854775807)'),9)+'  (call $check (call $close (i64.load (i32.const 8))))\n'+deny(p('(i64.load (i32.const 8))'),9)+deny('(call $close (i64.load (i32.const 8)))',9)
 s+='  (call $check (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)))\n  (call $check (call $revoke (i64.load (i32.const 8))))\n'+deny(p('(i64.load (i32.const 8))'),9)+deny('(call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8))',125)+'))\n'
 (ROOT/f'examples/ros{v}/handle_security.wat').write_text(s)
 s=base(v,pub,memory64=True)+'  (i64.store (i64.const 16) (i64.const -1))\n'
 h='(i64.load (i64.const 8))'
 s+=deny(p(h,-1,9),22)+deny(p(h,65535,9),22)+deny(p(h,8192,-1),90)
 s+=deny('(call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const -1))',22)
 s+='  (if (i64.ne (i64.load (i64.const 16)) (i64.const -1)) (then (call $exit (i32.const 211))))) )\n'
 (ROOT/f'examples/ros{v}/memory64_bounds.wat').write_text(s)
 imports=pub+f'\n (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))'
 s=base(v,imports,memory64=True)
 s=s.replace(' (func (export "_start")',''' (func $delay (local $start i64)
  (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64))) (local.set $start (i64.load (i64.const 64)))
  (loop $again (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
   (br_if $again (i64.lt_u (i64.sub (i64.load (i64.const 72)) (local.get $start)) (i64.const 1000000000)))))
 (func (export "_start")''')
 s+='  (if (i64.ne (memory.grow (i64.const 1)) (i64.const 1)) (then (call $exit (i32.const 212))))\n  (i32.store (i64.const 65536) (i32.const 5)) (i32.store (i64.const 65540) (i32.const 1819043176)) (i32.store8 (i64.const 65544) (i32.const 111))\n  (call $check '+p(h,65536,9)+') (call $delay) (call $check '+p(h,65536,9)+') (call $delay)))\n'
 (ROOT/f'examples/ros{v}/memory64_growth.wat').write_text(s)
s=base(1,'''(import "uwvm_ros2_v1" "topic_publish" (func $publish2 (param i64 i32 i64 i64) (result i32)))
 (import "uwvm_ros2_v1" "capability_open" (func $open2 (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros1_v1" "topic_publish" (func $publish1 (param i64 i32 i64 i64) (result i32)))''')
s+=deny('(call $publish2 (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const 9))',9)
s+='  (call $check (call $open2 (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 24)))\n'
s+=deny('(call $publish1 (i64.load (i32.const 24)) (i32.const 0) (i64.const 8192) (i64.const 9))',9)+'))\n'
(ROOT/'examples/ros_dual/cross_backend_handle.wat').write_text(s)
for recovery in (False,True):
 s='''(module
 (import "uwvm_ros1_v1" "capability_open" (func $open1 (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros2_v1" "capability_open" (func $open2 (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros1_v1" "topic_subscribe" (func $subscribe (param i64) (result i32)))
 (import "uwvm_ros1_v1" "event_poll" (func $poll (param i64 i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros2_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))
 (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
 (memory (export "memory") 1)
 (data (i32.const 0) "sub") (data (i32.const 4096) "pub") (data (i32.const 8192) "\05\00\00\00hello")
 (data (i32.const 1024) "READY_DISCONNECT\0a") (data (i32.const 1056) "READY_RECOVER\0a")
 (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))
 (func $say (param $p i32) (param $n i32)
  (i32.store (i32.const 1008) (local.get $p)) (i32.store (i32.const 1012) (local.get $n))
  (call $check (call $write (i32.const 1) (i32.const 1008) (i32.const 1) (i32.const 1000))))
 (func $delay (param $duration i64) (local $start i64)
  (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64))) (local.set $start (i64.load (i32.const 64)))
  (loop $again (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
   (br_if $again (i64.lt_u (i64.sub (i64.load (i32.const 72)) (local.get $start)) (local.get $duration)))))
 (func (export "_start") (local $e i32) (local $n i32)
  (call $check (call $open1 (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)))
  (call $check (call $open2 (i32.const 0) (i64.const 4096) (i64.const 3) (i64.const 24)))
  (call $check (call $subscribe (i64.load (i32.const 8))))
  (call $say (i32.const 1024) (i32.const 17)) (call $delay (i64.const 2000000000))
  (i64.store (i32.const 16) (i64.const -1)) (i64.store (i32.const 128) (i64.const -1))
  (if (i32.ne (call $poll (i64.load (i32.const 8)) (i32.const 0) (i64.const 128) (i64.const 4096) (i64.const 16)) (i32.const 107)) (then (call $exit (i32.const 213))))
  (if (i32.or (i64.ne (i64.load (i32.const 16)) (i64.const -1)) (i64.ne (i64.load (i32.const 128)) (i64.const -1))) (then (call $exit (i32.const 214))))
  (call $check (call $publish (i64.load (i32.const 24)) (i32.const 0) (i64.const 8192) (i64.const 9)))
  (call $delay (i64.const 1000000000))
  (call $check (call $publish (i64.load (i32.const 24)) (i32.const 0) (i64.const 8192) (i64.const 9)))
  (call $delay (i64.const 1000000000))
'''
 # Python non-raw string above needs escaped backslashes retained in the WAT data segments.
 s=s.replace('"\x05\x00\x00\x00hello"','"\\05\\00\\00\\00hello"').replace('READY_DISCONNECT\x00a','READY_DISCONNECT\\0a').replace('READY_RECOVER\x00a','READY_RECOVER\\0a')
 if recovery:s+='''  (call $say (i32.const 1056) (i32.const 14)) (call $delay (i64.const 2000000000))
  (call $check (call $subscribe (i64.load (i32.const 8))))
  (loop $event
   (local.set $e (call $poll (i64.load (i32.const 8)) (i32.const 0) (i64.const 128) (i64.const 4096) (i64.const 16)))
   (if (local.get $e) (then
    (if (i32.ne (local.get $e) (i32.const 11)) (then (call $exit (local.get $e))))
    (local.set $n (i32.add (local.get $n) (i32.const 1)))
    (if (i32.ge_u (local.get $n) (i32.const 200)) (then (call $exit (i32.const 110))))
    (call $delay (i64.const 50000000)) (br $event))))
  (if (i64.ne (i64.load (i32.const 16)) (i64.const 9)) (then (call $exit (i32.const 215))))
  (if (i32.or (i32.ne (i32.load (i32.const 128)) (i32.const 5)) (i32.ne (i32.load (i32.const 132)) (i32.const 1819043176))) (then (call $exit (i32.const 216))))
  (if (i32.ne (i32.load8_u (i32.const 136)) (i32.const 111)) (then (call $exit (i32.const 216))))
'''
 s+='))\n'
 (ROOT/('examples/ros_dual/disconnect_recover.wat' if recovery else 'examples/ros_dual/disconnect_survive.wat')).write_text(s)
 # Swap only import namespaces; trusted backend identity remains host-selected.
 reverse=s.replace('uwvm_ros1_v1','uwvm_ros_swap_v1').replace('uwvm_ros2_v1','uwvm_ros1_v1').replace('uwvm_ros_swap_v1','uwvm_ros2_v1')
 (ROOT/('examples/ros_dual/disconnect_2_recover.wat' if recovery else 'examples/ros_dual/disconnect_2_survive.wat')).write_text(reverse)
