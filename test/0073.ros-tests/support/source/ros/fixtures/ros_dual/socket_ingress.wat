(module
 (import "wasi_snapshot_preview1" "sock_send" (func $send (param i32 i32 i32 i32 i32) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory 1) (data (i32.const 128) "\50\4f\53\54\20\2f\52\50\43\32\20\48\54\54\50\2f\31\2e\30\0d\0a\43\6f\6e\74\65\6e\74\2d\4c\65\6e\67\74\68\3a\20\30\0d\0a\0d\0a")
 (func (export "_start")
  (i32.store (i32.const 8) (i32.const 128)) (i32.store (i32.const 12) (i32.const 42))
  (drop (call $send (i32.const 0) (i32.const 8) (i32.const 1) (i32.const 0) (i32.const 16)))
  (drop (call $send (i32.const 3) (i32.const 8) (i32.const 1) (i32.const 0) (i32.const 16)))
  (call $exit (i32.const 201))))
