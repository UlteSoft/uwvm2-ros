;; Alternating arms, lexical ends, skipped ends and a real nested call. Compare
;; full source-offset cycles between interpreters and LLVM, not display labels.
(module
  (import "env" "memory" (memory 1 1 shared))
  (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
  (func (export "wasi_thread_start") (param $tid i32) (param i32)
    local.get $tid i32.const 1 i32.ne if unreachable end
    i32.const 60000 i32.const 1 i32.atomic.store
    i32.const 60000 i32.const -1 memory.atomic.notify drop
    i32.const 60016 i32.const 0 i64.const -1 memory.atomic.wait32 drop
    i32.const 60008 i32.const -1 i32.atomic.store unreachable)
  (func $helper (param i32) (result i32)
    block $out
      local.get 0 br_if $out
      i32.const 7 return
    end
    i32.const 9)
  (func (export "_start") (local $toggle i32)
    i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
    block $ready loop $wait
      i32.const 60000 i32.atomic.load br_if $ready
      i32.const 60000 i32.const 0 i64.const -1 memory.atomic.wait32 drop br $wait
    end end
    loop $main
      local.get $toggle i32.const 1 i32.xor local.tee $toggle
      if
        i32.const 60004 i32.const 1 i32.atomic.rmw.add drop
      else
        i32.const 60004 i32.const 2 i32.atomic.rmw.add drop
      end
      local.get $toggle if nop end
      block $skip i32.const 1 br_if $skip unreachable end
      local.get $toggle call $helper drop
      br $main
    end))
