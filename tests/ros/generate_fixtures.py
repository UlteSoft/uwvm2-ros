#!/usr/bin/env python3
"""Regenerate canonical Bool/Twist/Image Wasm source fixtures from String examples."""
from pathlib import Path
import struct

root = Path(__file__).resolve().parents[2]
u32 = lambda n: struct.pack('<I', n)
string = lambda value: u32(len(value)) + value
payloads = {
    'bool': b'\x01',
    'twist': struct.pack('<6d', 1.25, -2, 3.5, 0.1, 0.2, 0.3),
    'image': u32(7)+u32(123)+u32(456)+string(b'camera_frame')+u32(2)+u32(2)+string(b'rgb8')+b'\x00'+u32(6)+u32(12)+bytes(range(12)),
}

def literal(payload):
    return ''.join('\\'+format(x, '02x') for x in payload)

def check(length, length_offset, ignore_sequence=False):
    start = 4 if ignore_sequence else 0
    return f'''(if (i64.ne (i64.load (i32.const {length_offset})) (i64.const {length})) (then (call $exit (i32.const 201))))
    (local.set $i (i32.const {start}))
    (block $verified (loop $compare
      (br_if $verified (i32.ge_u (local.get $i) (i32.const {length})))
      (if (i32.ne (i32.load8_u (i32.add (i32.const 128) (local.get $i))) (i32.load8_u (i32.add (i32.const 8192) (local.get $i)))) (then (call $exit (i32.const 202))))
      (local.set $i (i32.add (local.get $i) (i32.const 1))) (br $compare)))'''

for kind, payload in payloads.items():
    for version in (1, 2):
        directory = root/f'examples/ros{version}'
        talker = (directory/'talker.wat').read_text()
        talker = talker.replace('(data (i32.const 32) "\\05\\00\\00\\00hello")', f'(data (i32.const 8192) "{literal(payload)}")')
        talker = talker.replace('(i64.const 32) (i64.const 9)', f'(i64.const 8192) (i64.const {len(payload)})')
        (directory/f'talker_{kind}.wat').write_text(talker)
        listener = (directory/'listener.wat').read_text()
        listener = listener.replace('(data (i32.const 0) "sub")', f'(data (i32.const 0) "sub")\n  (data (i32.const 8192) "{literal(payload)}")')
        listener = listener.replace('(local $start i64)', '(local $start i64) (local $i i32)')
        marker = '    (if (i32.or (i64.ne (i64.load (i32.const 16))'
        listener = listener[:listener.index(marker)] + '    ' + check(len(payload), 16, kind=='image') + '))\n'
        (directory/f'listener_{kind}.wat').write_text(listener)
    for direction in ('1_to_2', '2_to_1'):
        directory = root/'examples/ros_dual'
        hybrid = (directory/f'hybrid_{direction}.wat').read_text()
        hybrid = hybrid.replace('(data (i32.const 512) "\\05\\00\\00\\00hello")', f'(data (i32.const 8192) "{literal(payload)}")')
        hybrid = hybrid.replace('(i64.const 512) (i64.const 9)', f'(i64.const 8192) (i64.const {len(payload)})')
        hybrid = hybrid.replace('(local $e i32) (local $start i64)', '(local $e i32) (local $start i64) (local $i i32)')
        first = hybrid.index('  ;; Decode the incoming')
        last = hybrid.index('  (call $check (call $publish', first)
        hybrid = hybrid[:first] + '  ;; Verify the incoming value and construct the fixed outgoing canonical value.\n  ' + check(len(payload), 24, kind=='image') + '\n' + hybrid[last:]
        (directory/f'hybrid_{direction}_{kind}.wat').write_text(hybrid)
