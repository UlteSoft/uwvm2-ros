(module
  ;; Source-only G09 fixture: Wasm32 memory 0, raw ROS2 v1 ABI. Imports
  ;; authorize nothing. The parent must grant only pubString/subString and the
  ;; WASI monotonic clock explicitly; fd 0/1 are baseline stdin/stdout. There
  ;; are no Guest filesystem, socket, environment, process-creation or ROS clock imports.
  (import "uwvm_ros2_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros2_v1" "capability_close" (func $close (param i64) (result i32)))
  (import "uwvm_ros2_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))
  (import "uwvm_ros2_v1" "topic_subscribe" (func $subscribe (param i64) (result i32)))
  (import "uwvm_ros2_v1" "event_poll" (func $poll (param i64 i32 i64 i64 i64) (result i32)))
  (import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32) (result i32)))
  (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))
  (memory (export "memory") 1 1)

  ;; Exact trusted alias names. notGranted is a valid identifier deliberately
  ;; absent from the parent policy; its failure tests internal capability
  ;; denial independently of the native witness's signed DDS writer refusal.
  (data (i32.const 0) "pubString")
  (data (i32.const 16) "subString")
  (data (i32.const 32) "notGranted")
  ;; Canonical String bodies contain the LE byte count followed by UTF-8 only.
  ;; Outbound: 27 text bytes / 31 canonical bytes. Inbound: distinct 19 / 23.
  ;; These bytes are not TCPROS or CDR frames and include no native padding.
  (data (i32.const 4096) "\1b\00\00\00uwvm real ROS exact payload")
  (data (i32.const 4160) "\13\00\00\00native signed reply")
  ;; The parent waits for the first exact newline-terminated marker before
  ;; checking actual native matching and releasing M. PASS comes only after
  ;; byte-exact delivery, both closes and stale-handle denial.
  (data (i32.const 4352) "SECURE_ENDPOINTS_CREATED\0a")
  (data (i32.const 4416) "PASS_SECURE_GUEST\0a")

  ;; Memory layout, all in the sole fixed 64 KiB memory:
  ;; 56/80: handle guards; 64/72: pub/sub i64 outputs.
  ;; 88/104: denied-output guards; 96: protected denied i64 handle.
  ;; 120/136: length guards; 128: protected event i64 length output.
  ;; 156/172: WASI iovec/count guards; 160/164: i32 buffer/length;
  ;; 168: i32 byte count; 176..183: one gate byte and seven protected bytes.
  ;; 184/200: clock guards; 192: WASI monotonic i64 timestamp.
  ;; 248/320: output guards; 256..319: bounded 64-byte receive storage.
  (func $die (param $code i32)
    (call $exit (local.get $code))
    unreachable)
  (func $check (param $error i32)
    (if (local.get $error) (then (call $die (local.get $error)))))
  (func $require (param $ok i32) (param $code i32)
    (if (i32.eqz (local.get $ok)) (then (call $die (local.get $code)))))

  ;; fd_read and fd_write use the Wasm32 WASI iovec ABI, separately from the
  ;; ROS ABI's i64 offsets. Protect both neighboring words and the count slot.
  (func $iov (param $buffer i32) (param $length i32)
    (i32.store (i32.const 156) (i32.const 0x55667788))
    (i32.store (i32.const 160) (local.get $buffer))
    (i32.store (i32.const 164) (local.get $length))
    (i32.store (i32.const 168) (i32.const 0xdeadbeef))
    (i32.store (i32.const 172) (i32.const 0x99aabbcc)))
  (func $iov_result (param $buffer i32) (param $length i32)
    (call $require (i32.eq (i32.load (i32.const 156)) (i32.const 0x55667788)) (i32.const 201))
    (call $require (i32.eq (i32.load (i32.const 160)) (local.get $buffer)) (i32.const 201))
    (call $require (i32.eq (i32.load (i32.const 164)) (local.get $length)) (i32.const 201))
    (call $require (i32.eq (i32.load (i32.const 168)) (local.get $length)) (i32.const 202))
    (call $require (i32.eq (i32.load (i32.const 172)) (i32.const 0x99aabbcc)) (i32.const 201)))
  (func $gate (param $expected i32)
    ;; Read exactly one byte; EOF, short input and any other byte refuse the
    ;; phase. There is no implicit newline, retry or default gate value.
    (i64.store (i32.const 176) (i64.const 0xa5a5a5a5a5a5a5a5))
    (call $iov (i32.const 176) (i32.const 1))
    (call $check (call $read (i32.const 0) (i32.const 160) (i32.const 1) (i32.const 168)))
    (call $iov_result (i32.const 176) (i32.const 1))
    (call $require
      (i64.eq (i64.load (i32.const 176))
        (i64.or (i64.const 0xa5a5a5a5a5a5a500) (i64.extend_i32_u (local.get $expected))))
      (i32.const 203)))
  (func $emit (param $offset i32) (param $length i32)
    ;; A successful partial write is insufficient to release the next parent
    ;; phase. The complete exact marker, including its newline, must be written.
    (call $iov (local.get $offset) (local.get $length))
    (call $check (call $write (i32.const 1) (i32.const 160) (i32.const 1) (i32.const 168)))
    (call $iov_result (local.get $offset) (local.get $length)))

  (func $now (result i64)
    (i64.store (i32.const 184) (i64.const 0x1020304050607080))
    (i64.store (i32.const 192) (i64.const -1))
    (i64.store (i32.const 200) (i64.const 0x8070605040302010))
    ;; Clock id 1 is monotonic, precision 1 ns. No realtime clock is requested.
    (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 192)))
    (call $require (i64.eq (i64.load (i32.const 184)) (i64.const 0x1020304050607080)) (i32.const 204))
    (call $require (i64.eq (i64.load (i32.const 200)) (i64.const 0x8070605040302010)) (i32.const 204))
    (call $require (i64.ne (i64.load (i32.const 192)) (i64.const -1)) (i32.const 204))
    (i64.load (i32.const 192)))
  (func $within_deadline (param $start i64) (result i64) (local $current i64)
    (local.set $current (call $now))
    (call $require (i64.ge_u (local.get $current) (local.get $start)) (i32.const 110))
    (call $require
      (i64.lt_u (i64.sub (local.get $current) (local.get $start)) (i64.const 10000000000))
      (i32.const 110))
    (local.get $current))
  (func $pace (param $start i64) (local $previous i64) (local $current i64)
    ;; Pace empty polling with only the already granted clock. Every spin
    ;; rechecks the overall deadline; no sleep/poll/process import is added.
    (local.set $previous (call $within_deadline (local.get $start)))
    (loop $pause
      (local.set $current (call $within_deadline (local.get $start)))
      (br_if $pause
        (i64.lt_u (i64.sub (local.get $current) (local.get $previous)) (i64.const 10000000)))))

  (func $reset_output (local $i i32)
    (i64.store (i32.const 120) (i64.const 0x1122334455667788))
    (i64.store (i32.const 128) (i64.const -1))
    (i64.store (i32.const 136) (i64.const 0x8877665544332211))
    (i64.store (i32.const 248) (i64.const 0x0123456789abcdef))
    (i64.store (i32.const 320) (i64.const 0xfedcba9876543210))
    (loop $fill
      (i32.store8 (i32.add (i32.const 256) (local.get $i)) (i32.const 0xa5))
      (local.set $i (i32.add (local.get $i) (i32.const 1)))
      (br_if $fill (i32.lt_u (local.get $i) (i32.const 64)))))
  (func $output_guards
    (call $require (i64.eq (i64.load (i32.const 120)) (i64.const 0x1122334455667788)) (i32.const 205))
    (call $require (i64.eq (i64.load (i32.const 136)) (i64.const 0x8877665544332211)) (i32.const 205))
    (call $require (i64.eq (i64.load (i32.const 248)) (i64.const 0x0123456789abcdef)) (i32.const 205))
    (call $require (i64.eq (i64.load (i32.const 320)) (i64.const 0xfedcba9876543210)) (i32.const 205)))
  (func $unchanged_tail (param $begin i32) (local $i i32)
    (local.set $i (local.get $begin))
    (block $done (loop $compare
      (br_if $done (i32.ge_u (local.get $i) (i32.const 64)))
      (call $require
        (i32.eq (i32.load8_u (i32.add (i32.const 256) (local.get $i))) (i32.const 0xa5))
        (i32.const 206))
      (local.set $i (i32.add (local.get $i) (i32.const 1)))
      (br $compare))))
  (func $handles_guarded (param $pub i64) (param $sub i64)
    (call $require (i64.eq (i64.load (i32.const 56)) (i64.const 0x13579bdf2468ace0)) (i32.const 207))
    (call $require (i64.eq (i64.load (i32.const 80)) (i64.const 0x02468ace13579bdf)) (i32.const 207))
    (call $require (i64.eq (i64.load (i32.const 64)) (local.get $pub)) (i32.const 207))
    (call $require (i64.eq (i64.load (i32.const 72)) (local.get $sub)) (i32.const 207)))

  (func (export "_start") (local $pub i64) (local $sub i64) (local $error i32) (local $start i64) (local $i i32)
    ;; Initial gate G=0x47 precedes this fixture's tested ROS calls. Worker
    ;; preflight may already have authenticated IPC before entering Wasm; this
    ;; gate is a live Worker mapping/protocol point, not a pre-IPC claim.
    (call $gate (i32.const 0x47))
    (i64.store (i32.const 56) (i64.const 0x13579bdf2468ace0))
    (i64.store (i32.const 64) (i64.const -1))
    (i64.store (i32.const 72) (i64.const -1))
    (i64.store (i32.const 80) (i64.const 0x02468ace13579bdf))
    (call $check (call $open (i32.const 0) (i64.const 0) (i64.const 9) (i64.const 64)))
    (local.set $pub (i64.load (i32.const 64)))
    (call $require (i64.ne (local.get $pub) (i64.const 0)) (i32.const 207))
    (call $require (i64.ne (local.get $pub) (i64.const -1)) (i32.const 207))
    (call $handles_guarded (local.get $pub) (i64.const -1))
    (call $check (call $open (i32.const 0) (i64.const 16) (i64.const 9) (i64.const 72)))
    (local.set $sub (i64.load (i32.const 72)))
    (call $require (i64.ne (local.get $sub) (i64.const 0)) (i32.const 207))
    (call $require (i64.ne (local.get $sub) (i64.const -1)) (i32.const 207))
    (call $require (i64.ne (local.get $pub) (local.get $sub)) (i32.const 207))
    (call $handles_guarded (local.get $pub) (local.get $sub))
    (call $check (call $subscribe (local.get $sub)))

    ;; capability_open has one 8-byte handle output and no length-out argument.
    ;; Its denied output is guarded on both sides. The separate receive buffer
    ;; and its length sentinel are also protected against unrelated mutation.
    (i64.store (i32.const 88) (i64.const 0x123456789abcdef0))
    (i64.store (i32.const 96) (i64.const 0x8877665544332211))
    (i64.store (i32.const 104) (i64.const 0x0fedcba987654321))
    (call $reset_output)
    (call $require
      (i32.eq (call $open (i32.const 0) (i64.const 32) (i64.const 10) (i64.const 96)) (i32.const 13))
      (i32.const 208))
    (call $require (i64.eq (i64.load (i32.const 88)) (i64.const 0x123456789abcdef0)) (i32.const 208))
    (call $require (i64.eq (i64.load (i32.const 96)) (i64.const 0x8877665544332211)) (i32.const 208))
    (call $require (i64.eq (i64.load (i32.const 104)) (i64.const 0x0fedcba987654321)) (i32.const 208))
    (call $require (i64.eq (i64.load (i32.const 128)) (i64.const -1)) (i32.const 208))
    (call $output_guards)
    (call $unchanged_tail (i32.const 0))
    (call $handles_guarded (local.get $pub) (local.get $sub))

    ;; One bootstrap publish creates the explicitly granted Gateway publisher.
    ;; Matching is attested by the independent native/controller side. This
    ;; Guest neither infers discovery from a delay nor retransmits this sample.
    (call $check (call $publish (local.get $pub) (i32.const 0) (i64.const 4096) (i64.const 31)))
    (call $emit (i32.const 4352) (i32.const 25))
    (call $gate (i32.const 0x4d))
    ;; Only the exact M=0x4d gate permits the final publish after real matching.
    (call $check (call $publish (local.get $pub) (i32.const 0) (i64.const 4096) (i64.const 31)))
    (local.set $start (call $now))
    (block $received (loop $receive
      (drop (call $within_deadline (local.get $start)))
      (call $reset_output)
      (local.set $error (call $poll (local.get $sub) (i32.const 0) (i64.const 256) (i64.const 64) (i64.const 128)))
      ;; An import may have its own bounded IPC wait. Check the Guest deadline
      ;; again before accepting a successful result; late success is refused.
      (drop (call $within_deadline (local.get $start)))
      (call $output_guards)
      (if (i32.eqz (local.get $error)) (then (br $received)))
      (call $require (i32.eq (local.get $error) (i32.const 11)) (local.get $error))
      (call $require (i64.eq (i64.load (i32.const 128)) (i64.const -1)) (i32.const 209))
      (call $unchanged_tail (i32.const 0))
      (call $pace (local.get $start))
      (br $receive)))

    ;; Compare length and every canonical byte, including the LE String prefix.
    ;; Protect the unused 41 bytes and both length/buffer guard pairs as well.
    (call $require (i64.eq (i64.load (i32.const 128)) (i64.const 23)) (i32.const 209))
    (block $equal (loop $compare_reply
      (br_if $equal (i32.ge_u (local.get $i) (i32.const 23)))
      (call $require
        (i32.eq (i32.load8_u (i32.add (i32.const 256) (local.get $i)))
                (i32.load8_u (i32.add (i32.const 4160) (local.get $i))))
        (i32.const 209))
      (local.set $i (i32.add (local.get $i) (i32.const 1)))
      (br $compare_reply)))
    (call $unchanged_tail (i32.const 23))
    (call $output_guards)
    (call $handles_guarded (local.get $pub) (local.get $sub))
    (call $check (call $close (local.get $pub)))
    (call $check (call $close (local.get $sub)))
    ;; Retain the original opaque handle locally: stale-handle error 9 must not
    ;; accidentally test a clobbered output word or another alias's handle.
    (call $require
      (i32.eq (call $publish (local.get $pub) (i32.const 0) (i64.const 4096) (i64.const 31)) (i32.const 9))
      (i32.const 210))
    (call $emit (i32.const 4416) (i32.const 18))
    (call $die (i32.const 0))))
