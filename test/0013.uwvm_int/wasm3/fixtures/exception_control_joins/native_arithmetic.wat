(module
 ;; Calls with parameters keep debug-policy checks from constant-folding
 ;; the native compiler helpers that this regression must actually execute.
 (func $i64_div_s (param i64 i64) (result i64) local.get 0 local.get 1 i64.div_s)
 (func $i64_rem_s (param i64 i64) (result i64) local.get 0 local.get 1 i64.rem_s)
 (func $i64_div_u (param i64 i64) (result i64) local.get 0 local.get 1 i64.div_u)
 (func $i64_rem_u (param i64 i64) (result i64) local.get 0 local.get 1 i64.rem_u)
 (func $f64_convert_i64_s (param i64) (result f64) local.get 0 f64.convert_i64_s)
 (func $f64_convert_i64_u (param i64) (result f64) local.get 0 f64.convert_i64_u)
 (func $f32_convert_i64_s (param i64) (result f32) local.get 0 f32.convert_i64_s)
 (func $f32_convert_i64_u (param i64) (result f32) local.get 0 f32.convert_i64_u)
 (func $i64_trunc_f64_s (param f64) (result i64) local.get 0 i64.trunc_f64_s)
 (func $i64_trunc_f64_u (param f64) (result i64) local.get 0 i64.trunc_f64_u)
 (func $i64_trunc_f32_s (param f32) (result i64) local.get 0 i64.trunc_f32_s)
 (func $i64_trunc_f32_u (param f32) (result i64) local.get 0 i64.trunc_f32_u)
 (func (export "_start")
  i64.const 9223372036854775807 i64.const 3000000000 call $i64_div_s
  i64.const 3074457345 i64.ne if unreachable end
  i64.const -9223372036854775807 i64.const 3000000000 call $i64_div_s
  i64.const -3074457345 i64.ne if unreachable end
  i64.const 9223372036854775807 i64.const 3000000000 call $i64_rem_s
  i64.const 1854775807 i64.ne if unreachable end
  i64.const -1 i64.const 3000000000 call $i64_div_u
  i64.const 6148914691 i64.ne if unreachable end
  i64.const -1 i64.const 3000000000 call $i64_rem_u
  i64.const 709551615 i64.ne if unreachable end
  i64.const -4294967296 call $f64_convert_i64_s f64.const -4294967296 f64.ne if unreachable end
  i64.const -1 call $f64_convert_i64_u f64.const 18446744073709551616 f64.ne if unreachable end
  i64.const -4294967296 call $f32_convert_i64_s f32.const -4294967296 f32.ne if unreachable end
  i64.const -1 call $f32_convert_i64_u f32.const 18446744073709551616 f32.ne if unreachable end
  f64.const -4294967296 call $i64_trunc_f64_s i64.const -4294967296 i64.ne if unreachable end
  f64.const 9223372036854775808 call $i64_trunc_f64_u i64.const -9223372036854775808 i64.ne if unreachable end
  f32.const -4294967296 call $i64_trunc_f32_s i64.const -4294967296 i64.ne if unreachable end
  f32.const 9223372036854775808 call $i64_trunc_f32_u i64.const -9223372036854775808 i64.ne if unreachable end))
