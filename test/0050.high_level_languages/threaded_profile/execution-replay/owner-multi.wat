(module
 (memory (export "memory") 1 1 shared)
 (memory $wide (export "wide") (export "wide-alias") i64 1 4 shared)
 (memory (export "small") 1 4 shared))
