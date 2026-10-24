#!/usr/bin/env python3
"""Operation substitution guests preserve output sentinels on each exact denial."""
from pathlib import Path
ROOT=Path(__file__).resolve().parents[5]

def rpc(version,name,identifier='op'):
    return f'(import "uwvm_ros{version}_v1" "{name}" (func ${identifier} (param i64 i32 i64 i64 i64 i64 i64) (result i32)))'
def base(version,imports,alias):
    return f'''(module
 (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 {imports}
 (memory (export "memory") 1) (data (i32.const 0) "{alias}")
 (func (export "_start")
  (if (call $open (i32.const 0) (i64.const 0) (i64.const {len(alias)}) (i64.const 8)) (then (call $exit (i32.const 220))))
  (i64.store (i32.const 16) (i64.const -1)) (i64.store (i32.const 128) (i64.const -1))
'''
def deny(call):
    return f'  (if (i32.ne {call} (i32.const 13)) (then (call $exit (i32.const 221))))\n'
def rpc_call(identifier,request_length=0):
    return f'(call ${identifier} (i64.load (i32.const 8)) (i32.const 0) (i64.const 4096) (i64.const {request_length}) (i64.const 128) (i64.const 4096) (i64.const 16))'
def end():
    return '''  (if (i32.or (i64.ne (i64.load (i32.const 16)) (i64.const -1)) (i64.ne (i64.load (i32.const 128)) (i64.const -1))) (then (call $exit (i32.const 222))))) )
'''
for version in (1,2):
    subscribe=f'(import "uwvm_ros{version}_v1" "topic_subscribe" (func $subscribe (param i64) (result i32)))'
    path=ROOT/f'examples/ros{version}'
    (path/'topic_subscribe_denied.wat').write_text(base(version,subscribe,'pub')+deny('(call $subscribe (i64.load (i32.const 8)))')+end())
    (path/'parameter_get_denied.wat').write_text(base(version,rpc(version,'parameter_get'),'cap')+deny(rpc_call('op'))+end())
    (path/'tf_lookup_denied.wat').write_text(base(version,rpc(version,'tf_lookup'),'cap')+'  (i64.store (i32.const 4096) (i64.const 0))\n'+deny(rpc_call('op',8))+end())
    imports=rpc(version,'action_cancel_own_goal','own')+'\n '+rpc(version,'action_cancel_any_goal','any')
    body=base(version,imports,'result')+'  (i64.store (i32.const 4096) (i64.const 1))\n'+deny(rpc_call('own',8))
    body+='  (i64.store (i32.const 4096) (i64.const 0))\n'+deny(rpc_call('any',8))+end()
    (path/'action_cancel_denied.wat').write_text(body)

(ROOT/'examples/ros_dual/group_dummy.wat').write_text('(module (func (export "unused")))\n')
(ROOT/'examples/ros_dual/backend_group_confusion.wat').write_text('''(module
 (import "uwvm_ros1_v1" "capability_open" (func $open1 (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros1_v1" "capability_close" (func $close1 (param i64) (result i32)))
 (import "uwvm_ros2_v1" "capability_open" (func $open2 (param i32 i64 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1) (data (i32.const 0) "pub")
 (func (export "_start")
  (if (call $open1 (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)) (then (call $exit (i32.const 223))))
  (i64.store (i32.const 16) (i64.const -1))
  (if (i32.ne (call $open2 (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 16)) (i32.const 13)) (then (call $exit (i32.const 224))))
  (if (i64.ne (i64.load (i32.const 16)) (i64.const -1)) (then (call $exit (i32.const 225))))
  (if (call $close1 (i64.load (i32.const 8))) (then (call $exit (i32.const 226))))))
''')
(ROOT/'examples/ros_dual/wasi_group_confusion.wat').write_text('''(module
 (import "uwvm_ros1_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1) (data (i32.const 0) "pub")
 (func (export "_start")
  (i64.store (i32.const 8) (i64.const -1))
  (if (i32.ne (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)) (i32.const 13)) (then (call $exit (i32.const 224))))
  (if (i64.ne (i64.load (i32.const 8)) (i64.const -1)) (then (call $exit (i32.const 225))))
  (call $exit (i32.const 0))))
''')
