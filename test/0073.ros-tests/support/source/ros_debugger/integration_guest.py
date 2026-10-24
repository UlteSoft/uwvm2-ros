"""Real-import application for the authenticated Runtime debugger integration.

Generation is not execution. The only imports are real ROS functions and the
two explicitly required WASIp1 stdout/exit functions. Safe-point loops read a
single application byte; Root advances it with the existing authenticated
debugger memory-edit command, separately recorded from read-only ROS queries.
No Guest clock, socket, environment inheritance or debug channel is imported.
"""

# Deliberately valid host-console words, emitted only through Guest fd1. An
# actual integration run must still reach its application marker and observe a
# running authenticated controller. These bytes are never sent to Control.ask.
MANAGEMENT_TEXT = ('pause\nquit\nreplace 0 0 1 /guest/must-not-execute.body\n'
                   'wasm-script status; replace 0 0 1 /guest/must-not-execute.body\n')

def guest(versions, mode):
    if tuple(versions) not in ((1,), (2,), (1, 2)) or mode not in ('default-deny', 'retained'):
        raise ValueError('closed backend/mode required')
    lines=['(module']
    for v in versions:
        signatures={'open':('capability_open','i32 i64 i64 i64'),'close':('capability_close','i64'),
                    'publish':('topic_publish','i64 i32 i64 i64'),'subscribe':('topic_subscribe','i64'),
                    'receive':('event_poll','i64 i32 i64 i64 i64'),'watch':('event_watch','i64 i64 i64'),
                    'poll':('event_poll_v1','i64 i32 i64 i64 i64')}
        for short,(name,sig) in signatures.items():
            lines.append(f'(import "uwvm_ros{v}_v1" "{name}" (func ${short}{v} (param {sig}) (result i32)))')
    lines += ['(import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))',
              '(import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))','(memory (export "memory") 1 1)']
    data={128:b'pub',144:b'sub',160:b'events',176:b'ungranted',
          256:b'\x0f\0\0\0warmup-js-relay',288:b'\x14\0\0\0roswasm-js-relay-out',
          352:b'\x13\0\0\0roswasm-js-relay-in',1024:MANAGEMENT_TEXT.encode('ascii')}
    markers=('DEBUG_DEFAULT_DENIED','DEBUG_ENDPOINTS','DEBUG_POLL_EMPTY','DEBUG_DELIVERED',
             'DEBUG_RETAINED','PASS_ROS_DEBUG_GUEST')
    for i,name in enumerate(markers):data[512+64*i]=(name+'\n').encode()
    for offset,value in data.items():
        encoded=''.join('\\%02x'%byte for byte in value)
        lines.append(f'(data (i32.const {offset}) "{encoded}")')
    def emit(name):return f'(call $emit (i32.const {512+64*markers.index(name)}) (i32.const {len(name)+1}))'
    def handle(v,role):return f'(i64.load (i32.const {64+(v-1)*32+role*8}))'
    lines += ['''
      (func $assert (param $ok i32) (if (i32.eqz (local.get $ok)) (then (call $exit (i32.const 201)) unreachable)))
      (func $ok (param $error i32) (call $assert (i32.eqz (local.get $error))))
      (func $emit (param $p i32) (param $n i32)
        (i32.store (i32.const 0) (local.get $p)) (i32.store (i32.const 4) (local.get $n))
        (i32.store (i32.const 8) (i32.const -1))
        (call $ok (call $write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 8)))
        (call $assert (i32.eq (i32.load (i32.const 8)) (local.get $n))))
      ;; An actual Wasm safe-point loop. It does not block in a native import.
      ;; Root learns the real module/thread IDs from status before editing this
      ;; byte through the already-authenticated debugger memory command.
      (func $gate (param $expected i32)
        (loop $again (br_if $again (i32.ne (i32.load8_u (i32.const 2048)) (local.get $expected)))))
      (func $bytes (local $i i32)
        (loop $body (call $assert (i32.eq (i32.load8_u (i32.add (i32.const 8192) (local.get $i))) (i32.const 165)))
          (local.set $i (i32.add (local.get $i) (i32.const 1))) (br_if $body (i32.lt_u (local.get $i) (i32.const 128)))))
      (func $untouched
        (call $assert (i64.eq (i64.load (i32.const 4096)) (i64.const -1))) (call $bytes))
      (func $tail (local $i i32)
        (local.set $i (i32.const 64))
        (loop $tail (call $assert (i32.eq (i32.load8_u (i32.add (i32.const 8192) (local.get $i))) (i32.const 165)))
          (local.set $i (i32.add (local.get $i) (i32.const 1))) (br_if $tail (i32.lt_u (local.get $i) (i32.const 128)))))
      (func (export "_start") (local $e i32) (local $attempt i32)''']
    for v in versions:lines.append(f'(local $received{v} i32)')
    lines.append(f'(call $emit (i32.const 1024) (i32.const {len(MANAGEMENT_TEXT)}))')
    for v in versions:
        lines += ['(i64.store (i32.const 4096) (i64.const -1))','(memory.fill (i32.const 8192) (i32.const 165) (i32.const 128))',
                  f'(call $assert (i32.eq (call $open{v} (i32.const 0) (i64.const 176) (i64.const 9) (i64.const 4096)) (i32.const 13)))',
                  '(call $untouched)']
    if mode=='default-deny':
        lines += [emit('DEBUG_DEFAULT_DENIED'),'(call $gate (i32.const 200))',emit('PASS_ROS_DEBUG_GUEST'),
                  '(call $exit (i32.const 0)) unreachable))']
        return '\n'.join(lines)+'\n'
    for v in versions:
        for role,offset,length in ((0,128,3),(1,144,3),(2,160,6)):
            lines.append(f'(call $ok (call $open{v} (i32.const 0) (i64.const {offset}) (i64.const {length}) (i64.const {64+(v-1)*32+role*8})))')
        lines += [f'(call $ok (call $subscribe{v} {handle(v,1)}))',
                  f'(call $ok (call $watch{v} {handle(v,2)} {handle(v,1)} (i64.const 0)))',
                  f'(call $ok (call $publish{v} {handle(v,0)} (i32.const 0) (i64.const 256) (i64.const 19)))']
    lines += [emit('DEBUG_ENDPOINTS'),'(call $gate (i32.const 1))']
    for v in versions:lines.append(f'(call $ok (call $publish{v} {handle(v,0)} (i32.const 0) (i64.const 288) (i64.const 24)))')
    lines += ['(local.set $attempt (i32.const 2))','(block $done (loop $polling (call $gate (local.get $attempt))']
    for v in versions:
        lines += [f'(if (i32.eqz (local.get $received{v})) (then',
                  '(memory.fill (i32.const 8192) (i32.const 165) (i32.const 128))','(i64.store (i32.const 4096) (i64.const -1))',
                  f'(local.set $e (call $receive{v} {handle(v,1)} (i32.const 0) (i64.const 8192) (i64.const 128) (i64.const 4096)))',
                  '(if (i32.eqz (local.get $e)) (then','(call $assert (i64.eq (i64.load (i32.const 4096)) (i64.const 23)))']
        for i in range(23):lines.append(f'(call $assert (i32.eq (i32.load8_u (i32.const {8192+i})) (i32.load8_u (i32.const {352+i}))))')
        lines += [f'(local.set $received{v} (i32.const 1))) (else (call $assert (i32.eq (local.get $e) (i32.const 11))) (call $untouched)))))']
    condition=f'(local.get $received{versions[0]})'
    if len(versions)==2:condition=f'(i32.and {condition} (local.get $received{versions[1]}))'
    lines += [f'(br_if $done {condition})',emit('DEBUG_POLL_EMPTY'),'(local.set $attempt (i32.add (local.get $attempt) (i32.const 1)))',
              '(call $assert (i32.lt_u (local.get $attempt) (i32.const 150))) (br $polling)))',emit('DEBUG_DELIVERED')]
    for v in versions:lines.append(f'(call $ok (call $close{v} {handle(v,1)}))')
    lines += [emit('DEBUG_RETAINED'),'(call $gate (i32.const 200))']
    for v in versions:
        lines += ['(memory.fill (i32.const 8192) (i32.const 165) (i32.const 128))','(i64.store (i32.const 4096) (i64.const -1))',
                  f'(call $assert (i32.eq (call $poll{v} {handle(v,2)} (i32.const 0) (i64.const 8192) (i64.const 1) (i64.const 4096)) (i32.const 90)))',
                  '(call $assert (i64.eq (i64.load (i32.const 4096)) (i64.const 64))) (call $bytes)',
                  f'(call $ok (call $poll{v} {handle(v,2)} (i32.const 0) (i64.const 8192) (i64.const 128) (i64.const 4096)))',
                  '(call $assert (i64.eq (i64.load (i32.const 4096)) (i64.const 64)))',
                  '(call $assert (i32.eq (i32.load (i32.const 8192)) (i32.const 1163024213)))',
                  '(call $assert (i32.eq (i32.load16_u (i32.const 8196)) (i32.const 1)))',
                  '(call $assert (i32.eq (i32.load16_u (i32.const 8198)) (i32.const 7)))',
                  f'(call $assert (i32.eq (i32.load8_u (i32.const 8200)) (i32.const {v})))',
                  '(call $assert (i32.eq (i32.load8_u (i32.const 8201)) (i32.const 3)))',
                  '(call $assert (i32.eqz (i32.load16_u (i32.const 8202))))','(call $assert (i32.eqz (i32.load (i32.const 8204))))',
                  f'(call $assert (i64.eq (i64.load (i32.const 8208)) {handle(v,1)}))',
                  '(call $assert (i64.eqz (i64.load (i32.const 8216))))','(call $assert (i64.eq (i64.load (i32.const 8224)) (i64.const 1)))',
                  '(call $assert (i64.ne (i64.load (i32.const 8232)) (i64.const 0)))','(call $assert (i64.ne (i64.load (i32.const 8240)) (i64.const 0)))',
                  '(call $assert (i64.eqz (i64.load (i32.const 8248)))) (call $tail)',
                  '(i64.store (i32.const 4096) (i64.const -1))','(memory.fill (i32.const 8192) (i32.const 165) (i32.const 128))',
                  f'(call $assert (i32.eq (call $poll{v} {handle(v,2)} (i32.const 0) (i64.const 8192) (i64.const 128) (i64.const 4096)) (i32.const 11)))',
                  '(call $untouched)',f'(call $ok (call $close{v} {handle(v,2)}))',f'(call $ok (call $close{v} {handle(v,0)}))']
    lines += [emit('PASS_ROS_DEBUG_GUEST'),'(call $exit (i32.const 0)) unreachable))']
    return '\n'.join(lines)+'\n'
