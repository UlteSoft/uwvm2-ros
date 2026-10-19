#!/usr/bin/env python3
"""Five actual instances, selected policy scopes, live foreign handles, native labels."""
from pathlib import Path
import struct
ROOT=Path(__file__).resolve().parents[2]/'examples/ros_multimodule'
ROOT.mkdir(parents=True,exist_ok=True)

def literal(data):return ''.join('\\'+format(x,'02x') for x in data)
def payload(text):
    data=text.encode();return literal(struct.pack('<I',len(data))+data)
def imports(v):return f'''(import "uwvm_ros{v}_v1" "capability_open" (func $open{v} (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros{v}_v1" "topic_publish" (func $pub{v} (param i64 i32 i64 i64) (result i32)))'''
def base(extra):return f'''(module
 {imports(1)} {imports(2)}
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))
 {extra}
 (memory (export "memory") 1)
 (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))
 (func $delay (local $start i64)
  (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64))) (local.set $start (i64.load (i32.const 64)))
  (loop $again (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
   (br_if $again (i64.lt_u (i64.sub (i64.load (i32.const 72)) (local.get $start)) (i64.const 500000000)))))
'''
def opened(v,alias,offset=8):return f'  (call $check (call $open{v} (i32.const 0) (i64.const 0) (i64.const {len(alias)}) (i64.const {offset})))\n'
def denied(v,offset,length):return f'''  (i64.store (i32.const 16) (i64.const -1))
  (if (i32.ne (call $open{v} (i32.const 0) (i64.const {offset}) (i64.const {length}) (i64.const 16)) (i32.const 13)) (then (call $exit (i32.const 230))))
  (if (i64.ne (i64.load (i32.const 16)) (i64.const -1)) (then (call $exit (i32.const 231))))
'''
def published(v,text,handle='(i64.load (i32.const 8))',offset=1024):return f'  (call $check (call $pub{v} {handle} (i32.const 0) (i64.const {offset}) (i64.const {4+len(text)})))\n'
for module,v,alias,text,forbidden in [('camera_a',1,'camera_a','camera-a','control'),('camera_b',1,'camera_b','camera-b','plan'),('controller',1,'control','controller','plan'),('planner',2,'plan','planner','control')]:
    extra='(import "camera_a" "live_handle" (global $foreign (mut i64)))' if module=='camera_b' else ''
    source=base(extra)
    if module=='camera_a':source+=' (global $handle (export "live_handle") (mut i64) (i64.const 0))\n'
    source+=f' (data (i32.const 0) "{alias}") (data (i32.const 32) "{forbidden}") (data (i32.const 1024) "{payload(text)}")\n (func $start\n'+opened(v,alias)
    if module=='camera_a':
        source+='  (global.set $handle (i64.load (i32.const 8)))\n'+denied(1,32,len(forbidden))
        source+='  (i32.store (i32.const 40) (i32.const 1851878512))\n'+denied(2,40,4) # plan little-endian
    else:source+=denied(3-v,32,len(forbidden))
    if module=='camera_b':
        source+='  (if (i64.eqz (global.get $foreign)) (then (call $exit (i32.const 232))))\n'
        source+=f'  (if (i32.ne (call $pub1 (global.get $foreign) (i32.const 0) (i64.const 1024) (i64.const {4+len(text)})) (i32.const 9)) (then (call $exit (i32.const 232))))\n'
    if module in ('camera_a','camera_b'):
        source+='  (call $check (call $open1 (i32.const 0) (i64.const 256) (i64.const 5) (i64.const 24)))\n'
        source+=published(1,text,'(i64.load (i32.const 24))')+'  (call $delay)\n'+published(1,text,'(i64.load (i32.const 24))')
        source+=f'  (if (i32.ne (call $pub1 (i64.load (i32.const 24)) (i32.const 0) (i64.const 1024) (i64.const {4+len(text)})) (i32.const 11)) (then (call $exit (i32.const 233))))\n'
        source+='  (if (i64.ne (i64.load (i32.const 16)) (i64.const -1)) (then (call $exit (i32.const 234))))\n'
    source+=published(v,text)+'  (call $delay)\n'+published(v,text)+'  (call $delay))\n (start $start))\n'
    if module in ('camera_a','camera_b'):source=source.replace(' (func $start\n',' (data (i32.const 256) "quota")\n (func $start\n',1)
    (ROOT/(module+'.wat')).write_text(source)
source=base('')+' (data (i32.const 0) "hybrid1") (data (i32.const 32) "camera_a") (data (i32.const 40) "plan") (data (i32.const 48) "hybrid2")\n'
source+=f' (data (i32.const 1024) "{payload("hybrid-ros1")}") (data (i32.const 2048) "{payload("hybrid-ros2")}")\n (func (export "_start")\n'+opened(1,'hybrid1')
source+='  (call $check (call $open2 (i32.const 0) (i64.const 48) (i64.const 7) (i64.const 24)))\n'+denied(1,32,8)+denied(2,40,4)
for _ in range(2):source+=published(1,'hybrid-ros1')+published(2,'hybrid-ros2','(i64.load (i32.const 24))',2048)+'  (call $delay)\n'
source+='))\n';(ROOT/'hybrid.wat').write_text(source)
