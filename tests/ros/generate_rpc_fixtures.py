#!/usr/bin/env python3
"""Generate single-operation Wasm guests for real ROS resource witnesses."""
from pathlib import Path
import struct
root=Path(__file__).resolve().parents[2]
u32=lambda n:struct.pack('<I',n)
text=lambda s:u32(len(s))+s
transform=u32(0)+u32(123)+u32(456)+text(b'map')+text(b'base')+struct.pack('<7d',1.25,2.5,3.75,0,0,0,1)
operations={
 'parameter_get':(b'', 'hello'), 'parameter_set':(text(b'hello'),'empty'),
 'parameter_delete':(b'','empty'),'parameter_exists':(b'','bool'),
 'parameter_list':(b'','string'),'parameter_search':(b'','string'),
 'parameter_describe':(b'','string'),'parameter_declare':(text(b'hello'),'empty'),
 'parameter_undeclare':(b'','empty'),
 'graph_list_topics':(b'','string'),'graph_list_nodes':(b'','string'),'graph_list_services':(b'','string'),
 'graph_inspect_topic':(b'','string'),'graph_inspect_node':(b'','string'),'graph_inspect_service':(b'','string'),'graph_inspect_action':(b'','string'),
 'time_read_ros':(b'','time'),'time_read_wall':(b'','time'),'time_read_system':(b'','time'),'time_read_steady':(b'','time'),
 'lifecycle_get_state':(b'','state'),'lifecycle_change_state':(b'','bool'),
 'tf_broadcast':(transform,'empty'),'tf_lookup':(bytes(8),'transform'),
}

def literal(value):return ''.join('\\'+format(v,'02x') for v in value)

def verify(kind):
 checks=[]
 def reject(expression): checks.append(f'    (if {expression} (then (call $exit (i32.const 201))))')
 if kind=='empty':reject('(i64.ne (i64.load (i32.const 16)) (i64.const 0))')
 elif kind=='bool':
  reject('(i64.ne (i64.load (i32.const 16)) (i64.const 1))')
  reject('(i32.ne (i32.load8_u (i32.const 128)) (i32.const 1))')
 elif kind=='time':
  reject('(i64.ne (i64.load (i32.const 16)) (i64.const 8))')
  reject('(i64.eqz (i64.load (i32.const 128)))')
 elif kind in ('string','hello','state'):
  shift=1 if kind=='state' else 0
  reject(f'(i64.lt_u (i64.load (i32.const 16)) (i64.const {4+shift}))')
  reject(f'(i64.ne (i64.extend_i32_u (i32.load (i32.const {128+shift}))) (i64.sub (i64.load (i32.const 16)) (i64.const {4+shift})))')
  if kind=='state':
   reject('(i32.ne (i32.load8_u (i32.const 128)) (i32.const 1))')
   reject('(i64.ne (i64.load (i32.const 16)) (i64.const 17))')
   reject('(i64.ne (i64.load (i32.const 133)) (i64.const 7451599682962419317))')
  if kind=='hello':
   reject('(i64.ne (i64.load (i32.const 16)) (i64.const 9))')
   reject('(i32.ne (i32.load (i32.const 132)) (i32.const 1819043176))')
   reject('(i32.ne (i32.load8_u (i32.const 136)) (i32.const 111))')
 elif kind=='transform':
  reject('(i64.ne (i64.load (i32.const 16)) (i64.const 83))')
  reject('(i32.ne (i32.load (i32.const 140)) (i32.const 3))')
  reject('(i32.ne (i32.load (i32.const 147)) (i32.const 4))')
  reject('(f64.ne (f64.load (i32.const 155)) (f64.const 1.25))')
  reject('(f64.ne (f64.load (i32.const 163)) (f64.const 2.5))')
  reject('(f64.ne (f64.load (i32.const 171)) (f64.const 3.75))')
  reject('(f64.ne (f64.load (i32.const 203)) (f64.const 1))')
 return '\n'.join(checks)

for version in (1,2):
 unsupported=['parameter_describe','parameter_declare','parameter_undeclare','graph_inspect_action','time_read_system','time_read_steady','lifecycle_get_state','lifecycle_change_state'] if version==1 else ['parameter_delete','parameter_exists','parameter_search','time_read_wall']
 for operation,(request,kind) in operations.items():
  if operation in unsupported:continue
  guest=f'''(module
  (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros{version}_v1" "{operation}" (func $rpc (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (memory (export "memory") 1) (data (i32.const 0) "cap")
  (data (i32.const 8192) "{literal(request)}")
  (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))
  (func (export "_start")
    (call $check (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)))
    (call $check (call $rpc (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const {len(request)}) (i64.const 128) (i64.const 4096) (i64.const 16)))
{verify(kind)}))
'''
  needle=None
  if operation=='parameter_list':needle=b'/uwvm_resources/value' if version==1 else b'/uwvm_resources/native/value'
  elif operation=='parameter_describe':needle=b'name=value'
  elif operation=='parameter_search':needle=b'/uwvm_resources/value'
  elif operation.startswith('graph_'):
   if 'topic' in operation:needle=b'/uwvm_resources/topic'
   elif 'node' in operation:needle=b'/uwvm_resources/native'
   elif 'service' in operation:needle=b'/uwvm_resources/trigger' if version==1 else b'/uwvm_resources/native/get_parameters'
   elif 'action' in operation:needle=b'/uwvm_ros_test/action_native'
  if needle:
   guest=guest.replace('  (func $check',f'  (data (i32.const 9000) "{literal(needle)}")\n  (func $check')
   contains=f'''  (func $contains (result i32) (local $at i32) (local $i i32)
    (block $absent (loop $scan
      (br_if $absent (i32.gt_u (i32.add (local.get $at) (i32.const {len(needle)})) (i32.load (i32.const 128))))
      (local.set $i (i32.const 0))
      (block $mismatch (loop $compare
        (br_if $mismatch (i32.ne (i32.load8_u (i32.add (i32.const 132) (i32.add (local.get $at) (local.get $i)))) (i32.load8_u (i32.add (i32.const 9000) (local.get $i)))))
        (local.set $i (i32.add (local.get $i) (i32.const 1)))
        (br_if $compare (i32.lt_u (local.get $i) (i32.const {len(needle)})))
        (return (i32.const 1))))
      (local.set $at (i32.add (local.get $at) (i32.const 1))) (br $scan)))
    (i32.const 0))
'''
   guest=guest.replace('  (func (export "_start")',contains+'  (func (export "_start")')
   guest=guest[:-3]+'    (if (i32.eqz (call $contains)) (then (call $exit (i32.const 207))))) )\n'
  if operation.startswith('graph_'):
   guest=guest.replace('  (memory (export "memory")','  (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))\n  (memory (export "memory")')
   call=f'(call $rpc (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const {len(request)}) (i64.const 128) (i64.const 4096) (i64.const 16))'
   retry=f'''  (func $snapshot (local $n i32) (local $e i32) (local $start i64)
    (loop $again
      (local.set $e {call})
      (if (i32.and (i32.eqz (local.get $e)) (i32.gt_u (i32.load (i32.const 128)) (i32.const 0))) (then (return)))
      (if (i32.and (local.get $e) (i32.ne (local.get $e) (i32.const 2))) (then (call $exit (local.get $e))))
      (local.set $n (i32.add (local.get $n) (i32.const 1)))
      (if (i32.ge_u (local.get $n) (i32.const 100)) (then (call $exit (i32.const 110))))
      (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64)))
      (local.set $start (i64.load (i32.const 64)))
      (loop $delay (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
        (br_if $delay (i64.lt_u (i64.sub (i64.load (i32.const 72)) (local.get $start)) (i64.const 50000000))))
      (br $again)))
'''
   guest=guest.replace('  (func (export "_start")',retry+'  (func (export "_start")')
   guest=guest.replace('(call $check '+call+')','(call $snapshot)',1)
  if operation=='tf_broadcast':
   guest=guest.replace('  (memory (export "memory")', '  (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))\n  (memory (export "memory")')
   marker='  (func (export "_start")'
   delay='''  (func $delay (local $start i64)
    (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64)))
    (local.set $start (i64.load (i32.const 64)))
    (loop $wait (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
      (br_if $wait (i64.lt_u (i64.sub (i64.load (i32.const 72)) (local.get $start)) (i64.const 1000000000)))))
'''
   guest=guest.replace(marker,delay+marker)
   guest=guest[:-3]+'''    (call $delay)
    (call $check (call $rpc (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const 83) (i64.const 128) (i64.const 4096) (i64.const 16)))
    (call $delay)))
'''
  if version==2 and operation in ('parameter_declare','parameter_undeclare'):
   imports='  (import "uwvm_ros2_v1" "parameter_get" (func $verify (param i64 i32 i64 i64 i64 i64 i64) (result i32)))\n'
   if operation=='parameter_undeclare':imports+='  (import "uwvm_ros2_v1" "parameter_declare" (func $setup (param i64 i32 i64 i64 i64 i64 i64) (result i32)))\n'
   guest=guest.replace('  (memory (export "memory")',imports+'  (memory (export "memory")')
   guest=guest.replace('  (func $check', '  (data (i32.const 4096) "setup") (data (i32.const 4104) "verify") (data (i32.const 8208) "\\05\\00\\00\\00hello")\n  (func $check')
   before='    (call $check (call $open (i32.const 0) (i64.const 4104) (i64.const 6) (i64.const 40)))\n'
   if operation=='parameter_undeclare':
    before+='''    (call $check (call $open (i32.const 0) (i64.const 4096) (i64.const 5) (i64.const 24)))
    (call $check (call $setup (i64.load (i32.const 24)) (i32.const 0) (i64.const 8208) (i64.const 9) (i64.const 128) (i64.const 4096) (i64.const 32)))
'''
   guest=guest.replace('    (call $check (call $rpc',before+'    (call $check (call $rpc',1)
   after='''    (call $check (call $verify (i64.load (i32.const 40)) (i32.const 0) (i64.const 8192) (i64.const 0) (i64.const 128) (i64.const 4096) (i64.const 16)))
'''+verify('hello') if operation=='parameter_declare' else '''    (if (i32.ne (call $verify (i64.load (i32.const 40)) (i32.const 0) (i64.const 8192) (i64.const 0) (i64.const 128) (i64.const 4096) (i64.const 16)) (i32.const 2)) (then (call $exit (i32.const 206))))'''
   guest=guest[:-3]+after+'))\n'
  (root/f'examples/ros{version}/{operation}.wat').write_text(guest)
  if operation in ('parameter_set','tf_broadcast','lifecycle_change_state'):
   denied_request=text(b'unauthorized') if operation=='parameter_set' else request
   denied=f'''(module
  (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros{version}_v1" "{operation}" (func $rpc (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (memory (export "memory") 1) (data (i32.const 0) "cap") (data (i32.const 8192) "{literal(denied_request)}")
  (func (export "_start")
   (if (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)) (then (call $exit (i32.const 204))))
   (if (i32.ne (call $rpc (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const {len(denied_request)}) (i64.const 128) (i64.const 4096) (i64.const 16)) (i32.const 13)) (then (call $exit (i32.const 205))))))
'''
   (root/f'examples/ros{version}/{operation}_denied.wat').write_text(denied)
 guest=f'''(module
  (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros{version}_v1" "log_write" (func $log (param i64 i32 i64 i64) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (memory (export "memory") 1) (data (i32.const 0) "cap") (data (i32.const 32) "\\05\\00\\00\\00hello")
  (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))
  (func (export "_start")
   (call $check (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)))
   (call $check (call $log (i64.load (i32.const 8)) (i32.const 0) (i64.const 32) (i64.const 9)))))
'''
 (root/f'examples/ros{version}/log_write.wat').write_text(guest)
