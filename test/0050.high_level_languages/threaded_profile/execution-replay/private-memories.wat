(module
 (import "env" "memory" (memory $shared 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
 (memory $small 1 4)
 (memory $wide i64 1 4)
 (memory $zero i64 0 1)
 (data (memory $small) (i32.const 16) "ABCD")
 (data (memory $wide) (i64.const 16) "EFGH")
 (data $data "abcdefgh")
 (func $check

  memory.size $small i32.const 1 i32.ne if unreachable end
  memory.size $wide i64.const 1 i64.ne if unreachable end
  i32.const 16 i32.load $small i32.const 0x44434241 i32.ne if unreachable end
  i64.const 16 i32.load $wide i32.const 0x48474645 i32.ne if unreachable end
  i32.const 16 i32.const 99 i32.store $small
  i64.const 16 i64.const 0x123456789abcdef i64.store $wide
  i64.const 16 i64.load $wide i64.const 0x123456789abcdef i64.ne if unreachable end
  i64.const 32 i32.const 0 i32.const 8 memory.init $wide $data
  i64.const 32 i64.load $wide i64.const 0x6867666564636261 i64.ne if unreachable end
  i32.const 48 i64.const 32 i32.const 8 memory.copy $small $wide
  i32.const 48 i64.load $small i64.const 0x6867666564636261 i64.ne if unreachable end
  i64.const 48 i32.const 48 i32.const 8 memory.copy $wide $small
  i64.const 49 i64.const 48 i64.const 7 memory.copy $wide $wide
  i64.const 49 i64.load $wide i64.const 0x0067666564636261 i64.ne if unreachable end
  i64.const 64 i32.const 42 i64.const 32 memory.fill $wide
  i64.const 64 v128.load $wide i8x16.extract_lane_u 15 i32.const 42 i32.ne if unreachable end
  i64.const 96 v128.const i32x4 1 2 3 4 v128.store $wide
  i64.const 96 v128.load $wide i32x4.extract_lane 3 i32.const 4 i32.ne if unreachable end
  i64.const 112 i32.const 77 i32.atomic.store $wide
  i64.const 112 i32.atomic.load $wide i32.const 77 i32.ne if unreachable end
  i32.const 1 memory.grow $small i32.const 1 i32.ne if unreachable end
  i64.const 1 memory.grow $wide i64.const 1 i64.ne if unreachable end
  i64.const 0x100000000 memory.grow $wide i64.const -1 i64.ne if unreachable end
  memory.size $wide i64.const 2 i64.ne if unreachable end
  memory.size $zero i64.eqz i32.eqz if unreachable end
  i64.const 1 memory.grow $zero i64.const 0 i64.ne if unreachable end
  i64.const 0 i32.load $zero i32.eqz i32.eqz if unreachable end
  i64.const 0 i32.const 71 i32.store $zero
  data.drop $data
  i64.const 131072 i32.const 0 i32.const 0 memory.init $wide $data
)
 (func (export "wasi_thread_start") (param i32 i32)
  call $check
  i32.const 0 i32.const 1 i32.atomic.rmw.add drop
  i32.const 0 i32.const 1 memory.atomic.notify drop)
 (func (export "_start")
  call $check
  i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
  i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
  block $done loop $again
   i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
   i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
   br $again end end))
