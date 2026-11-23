(module
(import "env" "memory" (memory 1 4 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
(func $entry)
(table 1 funcref)
(elem (i32.const 1) $entry)
(func (export "wasi_thread_start") (param i32 i32))
(func (export "_start")))
