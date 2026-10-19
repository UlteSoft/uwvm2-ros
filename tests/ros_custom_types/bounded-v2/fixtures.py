"""Real-import bounded Topic/negative Guests; source generation is not execution.

Every memory/output slot is explicit, each stream result/EOF/short write is
checked and positive polls are caller-paced at most200 times. No clock/random,
filesystem/socket/environment authority is added. Rejections invoke the actual
Runtime, rather than replacing native effects with mathematical callback stubs.
"""

def escaped(data):return ''.join('\\%02x'%value for value in data)


def sample():
    """Independent declaration-order golden record (61 bytes, not native CDR)."""
    text=lambda value:len(value).to_bytes(4,'little')+value
    return (text(b'bounded-native')+(3).to_bytes(4,'little')+b'\x03\x00\x05\x00\x08\x00'
        +(2).to_bytes(4,'little')+(-7).to_bytes(4,'little',signed=True)+b'\x01'
        +(11).to_bytes(4,'little',signed=True)+b'\x00'+text(b'first')+text(b'second'))


def bounded_guest(version,mode):
    if version not in (1,2) or mode not in ('topics','denied','rejection'):raise ValueError('closed bounded backend/mode required')
    module='uwvm_ros'+str(version)+'_v1'
    imports=[('open','capability_open','i32 i64 i64 i64'),('close','capability_close','i64'),
        ('publish','topic_publish','i64 i32 i64 i64'),('subscribe','topic_subscribe','i64'),
        ('poll','event_poll','i64 i32 i64 i64 i64'),('service','service_call','i64 i32 i64 i64 i64 i64 i64'),
        ('send','action_send_goal','i64 i32 i64 i64 i64 i64 i64')]
    lines=['(module',*[f'(import "{module}" "{name}" (func ${local} (param {params}) (result i32)))' for local,name,params in imports],
        '(import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32) (result i32)))',
        '(import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))',
        '(import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))','(memory (export "memory") 1 1)']
    aliases=('bounded-pub','bounded-sub','bounded-echo','bounded-send','bounded-empty',
             'bounded-cancel','bounded-feedback','bounded-result','bounded-service-events','bounded-action-events','ungranted')
    lines += [f'(data (i32.const {2048+64*i}) "{escaped(alias.encode())}")' for i,alias in enumerate(aliases)]
    golden=sample();lines += [f'(data (i32.const 128) "{escaped(golden)}")']
    mutants=[]
    for at,value in ((36,b'\x02'),(18,b'\x09\0\0\0'),(0,b'\x21\0\0\0'),(4,b'\xff')):
        raw=bytearray(golden);raw[at:at+len(value)]=value;mutants.append(bytes(raw))
    mutants.append(golden+b'\0')
    for i,raw in enumerate(mutants):
        lines.append(f'(data (i32.const {256+128*i}) "{escaped(raw)}")')
        lines.append(f'(data (i32.const {1024+128*i}) "{escaped(raw+(32).to_bytes(4,"little"))}")')
    names=('BOUNDED_TOPIC_READY','BOUNDED_AGAIN','PASS_BOUNDED_TOPICS','PASS_BOUNDED_DENIED','PASS_BOUNDED_REJECTION')
    lines += [f'(data (i32.const {4096+128*i}) "{escaped((name+chr(10)).encode())}")' for i,name in enumerate(names)]
    lines += ['''
(func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)) unreachable)))
(func $assert (param $ok i32) (if (i32.eqz (local.get $ok)) (then (call $exit (i32.const 201)) unreachable)))
(func $gate (param $expected i32)
 (i32.store (i32.const 0) (i32.const 32)) (i32.store (i32.const 4) (i32.const 1)) (i32.store (i32.const 8) (i32.const -1))
 (call $check (call $read (i32.const 0) (i32.const 0) (i32.const 1) (i32.const 8)))
 (call $assert (i32.and (i32.eq (i32.load (i32.const 8)) (i32.const 1)) (i32.eq (i32.load8_u (i32.const 32)) (local.get $expected)))))
(func $emit (param $p i32) (param $n i32)
 (i32.store (i32.const 0) (local.get $p)) (i32.store (i32.const 4) (local.get $n)) (i32.store (i32.const 8) (i32.const -1))
 (call $check (call $write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 8))) (call $assert (i32.eq (i32.load (i32.const 8)) (local.get $n))))
(func $seed (memory.fill (i32.const 8192) (i32.const 165) (i32.const 128)) (i64.store (i32.const 56) (i64.const -1)))
(func $unchanged (local $i i32)
 (call $assert (i64.eq (i64.load (i32.const 56)) (i64.const -1)))
 (loop $scan (call $assert (i32.eq (i32.load8_u (i32.add (i32.const 8192) (local.get $i))) (i32.const 165)))
 (local.set $i (i32.add (local.get $i) (i32.const 1))) (br_if $scan (i32.lt_u (local.get $i) (i32.const 128)))))
(func (export "_start") (local $e i32) (local $passes i32) (local $i i32)
(call $gate (i32.const 71))''']
    def emit(name):return f'(call $emit (i32.const {4096+128*names.index(name)}) (i32.const {len(name)+1}))'
    def denied(i):return [f'(i64.store (i32.const 64) (i64.const -1))',f'(call $assert (i32.eq (call $open (i32.const 0) (i64.const {2048+64*i}) (i64.const {len(aliases[i])}) (i64.const 64)) (i32.const 13)))','(call $assert (i64.eq (i64.load (i32.const 64)) (i64.const -1)))']
    lines+=denied(10)
    if mode=='denied':
        for i in range(10):lines+=denied(i)
        lines += [emit('PASS_BOUNDED_DENIED')]
    else:
        selected=(0,1) if mode=='topics' else (0,2,3,4)
        for i in selected:lines.append(f'(call $check (call $open (i32.const 0) (i64.const {2048+64*i}) (i64.const {len(aliases[i])}) (i64.const {72+8*i})))')
        if mode=='rejection':
            for i,raw in enumerate(mutants):
                lines += [f'(call $assert (i32.eq (call $publish (i64.load (i32.const 72)) (i32.const 0) (i64.const {256+128*i}) (i64.const {len(raw)})) (i32.const 22)))','(call $seed)',
                    f'(call $assert (i32.eq (call $service (i64.load (i32.const 88)) (i32.const 0) (i64.const {1024+128*i}) (i64.const {len(raw)+4}) (i64.const 8192) (i64.const 128) (i64.const 56)) (i32.const 22)))','(call $unchanged)','(call $seed)',
                    f'(call $assert (i32.eq (call $send (i64.load (i32.const 96)) (i32.const 0) (i64.const {256+128*i}) (i64.const {len(raw)}) (i64.const 8192) (i64.const 128) (i64.const 56)) (i32.const 22)))','(call $unchanged)']
            # Empty native components remain zero canonical bytes; trailing input
            # is invalid and must not dispatch an otherwise valid Trigger-like RPC.
            lines += ['(call $seed)','(call $assert (i32.eq (call $service (i64.load (i32.const 104)) (i32.const 0) (i64.const 8192) (i64.const 1) (i64.const 8192) (i64.const 128) (i64.const 56)) (i32.const 22)))','(call $unchanged)']
        else:
            lines += ['(call $check (call $subscribe (i64.load (i32.const 80))))',emit('BOUNDED_TOPIC_READY'),'(call $gate (i32.const 80))',
                '(call $check (call $publish (i64.load (i32.const 72)) (i32.const 0) (i64.const 128) (i64.const 61)))',
                '(block $done (loop $again (call $gate (i32.const 82)) (call $seed)',
                '(local.set $e (call $poll (i64.load (i32.const 80)) (i32.const 0) (i64.const 8192) (i64.const 128) (i64.const 56)))',
                '(if (i32.eqz (local.get $e)) (then (call $assert (i64.eq (i64.load (i32.const 56)) (i64.const 61)))',
                '(loop $exact (call $assert (i32.eq (i32.load8_u (i32.add (i32.const 8192) (local.get $i))) (i32.load8_u (i32.add (i32.const 128) (local.get $i))))) (local.set $i (i32.add (local.get $i) (i32.const 1))) (br_if $exact (i32.lt_u (local.get $i) (i32.const 61))))',
                '(loop $tail (call $assert (i32.eq (i32.load8_u (i32.add (i32.const 8192) (local.get $i))) (i32.const 165))) (local.set $i (i32.add (local.get $i) (i32.const 1))) (br_if $tail (i32.lt_u (local.get $i) (i32.const 128)))) (br $done)))',
                '(call $assert (i32.eq (local.get $e) (i32.const 11))) (call $unchanged) (local.set $passes (i32.add (local.get $passes) (i32.const 1))) (call $assert (i32.lt_u (local.get $passes) (i32.const 200)))',emit('BOUNDED_AGAIN'),'(br $again)))']
        for i in reversed(selected):lines.append(f'(call $check (call $close (i64.load (i32.const {72+8*i}))))')
        lines+=['(call $assert (i32.eq (call $publish (i64.load (i32.const 72)) (i32.const 0) (i64.const 128) (i64.const 61)) (i32.const 9)))',emit('PASS_BOUNDED_TOPICS' if mode=='topics' else 'PASS_BOUNDED_REJECTION')]
    lines += ['(call $exit (i32.const 0)) unreachable))'];return '\n'.join(lines)+'\n'
