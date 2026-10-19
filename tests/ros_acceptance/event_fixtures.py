"""Source-bound real-import Wasm for all seven Native ROS event kinds.

The translator is invoked only by the unified testing stage. All ROS pointers
select memory1; default WASI stdio selects memory0. The complete128-byte
sentinel in memory0 survives every ROS import. Each poll is explicitly paced
by the controller's R byte, with at most200 attempts per expected event.
Every kind first uses capacity1 and proves errno90 retains the exact record;
the following full copy validates all header fields and every payload byte.
"""
from fixtures import escaped

ALIASES=("topic","topic-events","advertise","request-events","respond","call","response-events",
         "missing","error-events","goal","feedback","feedback-events","result","result-events")


def seven_event_guest(version):
    if version not in (1,2):raise ValueError("explicit backend required")
    external=(('open','capability_open','i32 i64 i64 i64'),('close','capability_close','i64'),
              ('watch','event_watch','i64 i64 i64'),('poll','event_poll_v1','i64 i32 i64 i64 i64'),
              ('ready','event_ready','i64 i32 i64'),('submit','service_submit','i64 i64 i32 i64 i64 i64'),
              ('respond','service_respond','i64 i32 i64 i64 i64 i64 i64'),
              ('goal','action_send_goal','i64 i32 i64 i64 i64 i64 i64'))
    lines=[';; Finite real Wasm imports; no fabricated callbacks or native RPC client.', '(module']
    lines += [f'(import "uwvm_ros{version}_v1" "{name}" (func ${alias} (param {sig}) (result i32)))'
              for alias,name,sig in external]
    lines += ['(import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32) (result i32)))',
              '(import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))',
              '(import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))',
              '(memory (export "memory") 1 1)','(memory 1 1)']
    def handle(alias):return f'(i64.load 1 (i32.const {1024+8*ALIASES.index(alias)}))'
    for index,alias in enumerate(ALIASES):lines.append(f'(data (memory 1) (i32.const {512+32*index}) "{alias}")')
    records=((2048,b'\1'),(2056,(5).to_bytes(4,'little')),(2100,b'\x0c\0\0\0native-topic'),
             (2140,b'\1\x0f\0\0\0native-response'),(2180,b'\1\x0e\0\0\0guest-response'),
             (2220,(6).to_bytes(4,'little')+b''.join(value.to_bytes(4,'little') for value in (0,1,1,2,3,5))))
    for offset,data in records:lines.append(f'(data (memory 1) (i32.const {offset}) "{escaped(data)}")')
    markers=('EVENTS_READY','EVENT_TOPIC_DONE','EVENT_REQUEST_DONE','EVENT_RESPONSE_DONE','EVENT_ERROR_DONE',
             'EVENT_GOAL_WATCHED','EVENT_FEEDBACK_DONE','EVENT_RESULT_DONE','EVENT_CLOSED_DONE','PASS_WASM_SEVEN_EVENTS','EVENT_POLL_AGAIN')
    for index,name in enumerate(markers):lines.append(f'(data (i32.const {512+64*index}) "{name}\\0a")')
    def emit(name):return f'(call $emit (i32.const {512+64*markers.index(name)}) (i32.const {len(name)+1}))'
    lines += ['''
      (func $assert (param $ok i32) (if (i32.eqz (local.get $ok)) (then (call $exit (i32.const 201)) unreachable)))
      (func $check (param $error i32) (call $assert (i32.eqz (local.get $error))))
      (func $gate (param $expected i32)
        (i32.store (i32.const 0) (i32.const 32)) (i32.store (i32.const 4) (i32.const 1))
        (call $check (call $read (i32.const 0) (i32.const 0) (i32.const 1) (i32.const 8)))
        (call $assert (i32.and (i32.eq (i32.load (i32.const 8)) (i32.const 1))
          (i32.eq (i32.load8_u (i32.const 32)) (local.get $expected)))))
      (func $emit (param $p i32) (param $n i32)
        (i32.store (i32.const 0) (local.get $p)) (i32.store (i32.const 4) (local.get $n))
        (call $check (call $write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 8)))
        (call $assert (i32.eq (i32.load (i32.const 8)) (local.get $n))))
      (func $protected (local $i i32)
        (loop $s (call $assert (i32.eq (i32.load8_u (i32.add (i32.const 4096) (local.get $i))) (i32.const 165)))
          (local.set $i (i32.add (local.get $i) (i32.const 1))) (br_if $s (i32.lt_u (local.get $i) (i32.const 128)))))
      (func $same (param $left i32) (param $right i32) (param $n i32) (local $i i32)
        (if (local.get $n) (then (loop $s
          (call $assert (i32.eq (i32.load8_u 1 (i32.add (local.get $left) (local.get $i)))
            (i32.load8_u 1 (i32.add (local.get $right) (local.get $i)))))
          (local.set $i (i32.add (local.get $i) (i32.const 1))) (br_if $s (i32.lt_u (local.get $i) (local.get $n)))))))
      (func $untouched (local $i i32)
        (loop $s (call $assert (i32.eq (i32.load8_u 1 (i32.add (i32.const 8192) (local.get $i))) (i32.const 165)))
          (local.set $i (i32.add (local.get $i) (i32.const 1))) (br_if $s (i32.lt_u (local.get $i) (i32.const 256)))))
      ;; Exact front record is retained after90. R pacing is outside WASI rights.
      (func $receive (param $observer i64) (param $source i64) (param $kind i32) (param $corr i64)
        (param $body i64) (param $error i32) (param $flags i32) (param $seq i64)
        (local $e i32) (local $attempts i32)
        (block $available (loop $retry
          (call $gate (i32.const 82))
          (memory.fill 1 (i32.const 8192) (i32.const 165) (i32.const 256))
          (i64.store 1 (i32.const 9000) (i64.const -1))
          (call $check (call $ready (local.get $observer) (i32.const 1) (i64.const 9024)))
          (local.set $e (call $poll (local.get $observer) (i32.const 1) (i64.const 8192) (i64.const 1) (i64.const 9000)))
          (call $protected)
          (if (i32.eq (local.get $e) (i32.const 90)) (then
            (call $assert (i64.eq (i64.load 1 (i32.const 9000)) (i64.add (i64.const 64) (local.get $body))))
            (call $untouched)
            (br $available)))
          (call $assert (i32.eq (local.get $e) (i32.const 11)))
          (call $assert (i64.eq (i64.load 1 (i32.const 9000)) (i64.const -1)))
          (call $untouched)
          (local.set $attempts (i32.add (local.get $attempts) (i32.const 1)))
          (call $assert (i32.lt_u (local.get $attempts) (i32.const 200)))''',emit('EVENT_POLL_AGAIN'),'''(br $retry)))
        (call $check (call $poll (local.get $observer) (i32.const 1) (i64.const 8192) (i64.const 256) (i64.const 9000)))
        (call $protected)
        (call $assert (i64.eq (i64.load 1 (i32.const 9000)) (i64.add (i64.const 64) (local.get $body))))
        (call $assert (i32.eq (i32.load 1 (i32.const 8192)) (i32.const 1163024213)))
        (call $assert (i32.eq (i32.load16_u 1 (i32.const 8196)) (i32.const 1)))
        (call $assert (i32.eq (i32.load16_u 1 (i32.const 8198)) (local.get $kind)))
        (call $assert (i32.eq (i32.load8_u 1 (i32.const 8200)) (i32.const VERSION)))
        (call $assert (i32.eq (i32.load8_u 1 (i32.const 8201)) (local.get $flags)))
        (call $assert (i32.eqz (i32.load16_u 1 (i32.const 8202))))
        (call $assert (i32.eq (i32.load 1 (i32.const 8204)) (local.get $error)))
        (call $assert (i64.eq (i64.load 1 (i32.const 8208)) (local.get $source)))
        (if (i64.ne (local.get $corr) (i64.const -1)) (then
          (call $assert (i64.eq (i64.load 1 (i32.const 8216)) (local.get $corr)))))
        (call $assert (i64.eq (i64.load 1 (i32.const 8224)) (local.get $seq)))
        (call $assert (i64.ne (i64.load 1 (i32.const 8232)) (i64.const 0)))
        (call $assert (i64.ne (i64.load 1 (i32.const 8240)) (i64.const 0)))
        (if (i64.eqz (i64.load 1 (i32.const 2024))) (then
          (i64.store 1 (i32.const 2024) (i64.load 1 (i32.const 8232)))
          (i64.store 1 (i32.const 2032) (i64.load 1 (i32.const 8240)))))
        (call $assert (i64.eq (i64.load 1 (i32.const 8232)) (i64.load 1 (i32.const 2024))))
        (call $assert (i64.eq (i64.load 1 (i32.const 8240)) (i64.load 1 (i32.const 2032))))
        (call $assert (i64.eq (i64.load 1 (i32.const 8248)) (local.get $body))))
      (func (export "_start")
        (memory.fill (i32.const 4096) (i32.const 165) (i32.const 128)) (call $gate (i32.const 71))
    '''.replace('VERSION',str(version))]
    for index,alias in enumerate(ALIASES):lines += [f'(call $check (call $open (i32.const 1) (i64.const {512+32*index}) (i64.const {len(alias)}) (i64.const {1024+8*index})))','(call $protected)']
    for observer,source in (('topic-events','topic'),('request-events','advertise'),('response-events','call'),('error-events','missing')):
        lines.append(f'(call $check (call $watch {handle(observer)} {handle(source)} (i64.const 0)))')
    lines += [emit('EVENTS_READY')]
    def receive(observer,source,kind,corr,body,error=0,flags=0,seq=1):
        return f'(call $receive {handle(observer)} {handle(source)} (i32.const {kind}) {corr} (i64.const {body}) (i32.const {error}) (i32.const {flags}) (i64.const {seq}))'
    lines += [receive('topic-events','topic',1,'(i64.const 0)',16),
              '(call $same (i32.const 8256) (i32.const 2100) (i32.const 16))',emit('EVENT_TOPIC_DONE'),
              receive('request-events','advertise',2,'(i64.const -1)',9),
              '(call $assert (i64.ne (i64.load 1 (i32.const 8216)) (i64.const 0)))',
              '(call $assert (i64.eq (i64.load 1 (i32.const 8216)) (i64.load 1 (i32.const 8256))))',
              '(call $assert (i32.eq (i32.load8_u 1 (i32.const 8264)) (i32.const 1)))',
              '(i64.store 1 (i32.const 2300) (i64.load 1 (i32.const 8256)))',
              '(memory.copy 1 1 (i32.const 2308) (i32.const 2180) (i32.const 19))',
              f'(call $check (call $respond {handle("respond")} (i32.const 1) (i64.const 2300) (i64.const 27) (i64.const 8192) (i64.const 256) (i64.const 9000)))',
              '(call $assert (i64.eq (i64.load 1 (i32.const 9000)) (i64.const 0)))',emit('EVENT_REQUEST_DONE'),
              f'(call $check (call $submit {handle("call")} {handle("response-events")} (i32.const 1) (i64.const 2048) (i64.const 1) (i64.const 2000)))',
              '(call $assert (i64.ne (i64.load 1 (i32.const 2000)) (i64.const 0)))',
              receive('response-events','call',3,'(i64.load 1 (i32.const 2000))',20),
              '(call $same (i32.const 8256) (i32.const 2140) (i32.const 20))',emit('EVENT_RESPONSE_DONE'),
              f'(call $check (call $submit {handle("missing")} {handle("error-events")} (i32.const 1) (i64.const 2048) (i64.const 1) (i64.const 2008)))',
              '(call $assert (i32.and (i64.ne (i64.load 1 (i32.const 2008)) (i64.const 0)) (i64.ne (i64.load 1 (i32.const 2008)) (i64.load 1 (i32.const 2000)))))',
              receive('error-events','missing',6,'(i64.load 1 (i32.const 2008))',0,110,2),emit('EVENT_ERROR_DONE'),
              f'(call $check (call $goal {handle("goal")} (i32.const 1) (i64.const 2056) (i64.const 4) (i64.const 2016) (i64.const 8) (i64.const 9000)))',
              '(call $assert (i32.and (i64.eq (i64.load 1 (i32.const 9000)) (i64.const 8)) (i64.ne (i64.load 1 (i32.const 2016)) (i64.const 0))))']
    for observer,source in (('feedback-events','feedback'),('result-events','result')):
        lines.append(f'(call $check (call $watch {handle(observer)} {handle(source)} (i64.load 1 (i32.const 2016))))')
    lines += [emit('EVENT_GOAL_WATCHED'),receive('feedback-events','feedback',4,'(i64.load 1 (i32.const 2016))',36),
              '(call $assert (i64.eq (i64.load 1 (i32.const 8256)) (i64.load 1 (i32.const 2016))))',
              '(call $same (i32.const 8264) (i32.const 2220) (i32.const 28))',emit('EVENT_FEEDBACK_DONE'),
              receive('result-events','result',5,'(i64.load 1 (i32.const 2016))',37),
              '(call $assert (i64.eq (i64.load 1 (i32.const 8256)) (i64.load 1 (i32.const 2016))))',
              '(call $assert (i32.eq (i32.load8_u 1 (i32.const 8264)) (i32.const 2)))',
              '(call $same (i32.const 8265) (i32.const 2220) (i32.const 28))',emit('EVENT_RESULT_DONE'),
              f'(call $check (call $close {handle("topic")}))',
              # Closing an activated Source conservatively discloses possible
              # already-started native effects: retired1|effects-may-remain2.
              receive('topic-events','topic',7,'(i64.const 0)',0,0,3,2),emit('EVENT_CLOSED_DONE'),
              '(memory.fill 1 (i32.const 8192) (i32.const 165) (i32.const 256))',
              '(i64.store 1 (i32.const 9000) (i64.const -1))',
              f'(call $assert (i32.eq (call $poll {handle("topic-events")} (i32.const 1) (i64.const 8192) (i64.const 256) (i64.const 9000)) (i32.const 11)))',
              '(call $assert (i64.eq (i64.load 1 (i32.const 9000)) (i64.const -1)))','(call $untouched)']
    for alias in reversed(ALIASES):
        if alias!='topic':lines.append(f'(call $check (call $close {handle(alias)}))')
    lines += ['(call $protected)',emit('PASS_WASM_SEVEN_EVENTS'),'(call $exit (i32.const 0)) unreachable))']
    return '\n'.join(lines)+'\n'
