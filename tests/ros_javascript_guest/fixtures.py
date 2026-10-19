"""Small independent Wasm callers for actual embedded-JavaScript acceptance.

These functions only author WAT source. They neither instantiate Wasm nor run
JavaScript. The owner must translate, validate, pin and execute each product
through the guarded Runtime lane. Every check below executes inside the real
calling Wasm instance; no host Memory, JS callback or synthetic import is used.
"""

GATE = "JS_RUNTIME_GATE\n"
PURE_PASS = "PASS_JS_RUNTIME_PURE_LIFECYCLE\n"


def escaped(value):
    return ''.join('\\%02x' % byte for byte in value.encode('ascii'))


def prelude(imports, marker):
    """Provide only a finite basic-stdio gate, checked output and proc_exit.

    The main memory is independent of every preloaded library. G is sent only
    after the owner has proved the current owned Worker executable and birth.
    No clock, randomness, file, socket or ROS operation is imported here.
    """
    return '''(module
%s
 (import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32) (result i32)))
 (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1 1)
 (data (i32.const 128) "%s")
 (data (i32.const 256) "%s")
 (func $assert (param $value i32) (if (i32.eqz (local.get $value)) (then unreachable)))
 (func $emit (param $address i32) (param $length i32)
   (i32.store (i32.const 0) (local.get $address))
   (i32.store (i32.const 4) (local.get $length))
   (i32.store (i32.const 8) (i32.const -1))
   (call $assert (i32.eqz (call $write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 8))))
   (call $assert (i32.eq (i32.load (i32.const 8)) (local.get $length))))
 (func $gate
   (call $emit (i32.const 128) (i32.const %d))
   (i32.store (i32.const 0) (i32.const 64))
   (i32.store (i32.const 4) (i32.const 1))
   (i32.store (i32.const 8) (i32.const -1))
   (call $assert (i32.eqz (call $read (i32.const 0) (i32.const 0) (i32.const 1) (i32.const 8))))
   (call $assert (i32.eq (i32.load (i32.const 8)) (i32.const 1)))
   (call $assert (i32.eq (i32.load8_u (i32.const 64)) (i32.const 71))))
''' % (imports, escaped(GATE), escaped(marker), len(GATE))


def command_gate():
    """Run the exact preloaded command's real _start after ownership proof.

    A command returning instead of terminating through its declared proc_exit
    is an error202. Guest stdout and exit status otherwise remain unchanged.
    """
    return prelude('(import "application" "_start" (func $application))', '') + '''
 (func (export "_start") (call $gate) (call $application)
   (call $exit (i32.const 202)) unreachable))
'''


def pure_lifecycle():
    """Exercise four actual JS libraries, including the real callback edge.

    The integer oracle is deliberately small and independent of the JS batch.
    Negative zero, f32 rounding, Infinity and NaN are checked with real Wasm
    floating instructions. Lifecycle refusals are checked alongside scalar
    fallbacks, so a valid-looking fallback cannot hide CLOSED status. Invalid
    pump bounds return INVALID6 without poisoning an otherwise healthy engine.
    """
    imports = []
    for module in ('vision', 'callbacks', 'motion', 'floating'):
        for name, params in (('init', ''), ('status', ''), ('close', ''), ('pump', '(param i32)')):
            imports.append('(import "%s" "roswasm_js_%s" (func $%s_%s %s (result i32)))' %
                           (module, name, module, name, params))
    imports += [
        '(import "vision" "vision_initialize" (func $vinit (result i32)))',
        '(import "vision" "vision_clearance_mm" (func $clear (param i32 i32 i32 i32) (result i32)))',
        '(import "callbacks" "callback_begin" (func $begin (param i32) (result i32)))',
        '(import "callbacks" "callback_end" (func $end (param i32 i32) (result i32)))',
        '(import "callbacks" "callback_count" (func $count (param i32 i32) (result i32)))',
        '(import "callbacks" "on_motion" (func $callback (param i32 i32 i32 i32) (result i32)))',
        '(import "motion" "motion_step" (func $motion (param i32 i32 i32 i32) (result i32)))',
        '(import "floating" "identity32" (func $f32 (param f32) (result f32)))',
        '(import "floating" "identity64" (func $f64 (param f64) (result f64)))',
        '(import "floating" "sum32" (func $sum32 (param f32 f32) (result f32)))',
        '(import "floating" "sum64" (func $sum64 (param f64 f64) (result f64)))']
    body = [' (func (export "_start") (local $generation i32)', ' (call $gate)']
    for module in ('vision', 'callbacks', 'motion', 'floating'):
        body += [' (call $assert (i32.eqz (call $%s_init)))' % module,
                 ' (call $assert (i32.eqz (call $%s_status)))' % module,
                 ' (call $assert (i32.eq (call $%s_pump (i32.const 0)) (i32.const 6)))' % module,
                 ' (call $assert (i32.eqz (call $%s_status)))' % module,
                 ' (call $assert (i32.eqz (call $%s_pump (i32.const 1))))' % module]
    body += ['''
 (call $assert (i32.eqz (call $vinit)))
 (call $assert (i32.eq (call $clear (i32.const 5000) (i32.const 400) (i32.const 401) (i32.const 4999)) (i32.const 400)))
 (call $assert (i32.eq (call $clear (i32.const -1) (i32.const 1) (i32.const 2) (i32.const 3)) (i32.const -2147483626)))
 (call $assert (i32.eqz (call $begin (i32.const 0))))
 (local.set $generation (call $begin (i32.const 1297044558)))
 (call $assert (i32.ne (local.get $generation) (i32.const 0)))
 (call $assert (i32.eqz (call $begin (i32.const 1297044558))))
 (call $assert (i32.eq (call $motion (i32.const 1297044558) (local.get $generation) (i32.const 60) (i32.const 400)) (i32.const 60)))
 (call $assert (i32.eqz (call $motion (i32.const 1297044558) (local.get $generation) (i32.const 60) (i32.const 399))))
 (call $assert (i32.eq (call $count (i32.const 1297044558) (local.get $generation)) (i32.const 2)))
 (call $assert (i32.eqz (call $end (i32.const 1297044558) (local.get $generation))))
 (call $assert (i32.eq (call $end (i32.const 1297044558) (local.get $generation)) (i32.const 9)))
 (call $assert (i32.eq (call $callback (i32.const 1297044558) (local.get $generation) (i32.const 1) (i32.const 5000)) (i32.const -2147483639)))
 (call $assert (i32.eq (call $count (i32.const 1297044558) (local.get $generation)) (i32.const -1)))
 (call $assert (f32.eq (call $f32 (f32.demote_f64 (f64.const 16777217))) (f32.const 16777216)))
 (call $assert (f64.eq (call $f64 (f64.const 16777217)) (f64.const 16777217)))
 (call $assert (i32.eq (i32.reinterpret_f32 (call $f32 (f32.const -0))) (i32.const -2147483648)))
 (call $assert (i64.eq (i64.reinterpret_f64 (call $f64 (f64.const -0))) (i64.const -9223372036854775808)))
 (call $assert (f32.eq (call $sum32 (f32.const 0.5) (f32.const 0.25)) (f32.const 0.75)))
 (call $assert (f64.eq (call $sum64 (f64.const 0.5) (f64.const 0.25)) (f64.const 0.75)))
 (call $assert (f32.eq (call $f32 (f32.const inf)) (f32.const inf)))
 (call $assert (f64.eq (call $f64 (f64.const -inf)) (f64.const -inf)))
 (call $assert (f32.ne (call $f32 (f32.const nan)) (call $f32 (f32.const nan))))
 (call $assert (f64.ne (call $f64 (f64.const nan)) (call $f64 (f64.const nan))))
''']
    for module in ('motion', 'callbacks', 'vision', 'floating'):
        body += [' (call $assert (i32.eqz (call $%s_status)))' % module,
                 ' (call $assert (i32.eqz (call $%s_close)))' % module,
                 ' (call $assert (i32.eq (call $%s_status) (i32.const 5)))' % module,
                 ' (call $assert (i32.eq (call $%s_init) (i32.const 5)))' % module,
                 ' (call $assert (i32.eq (call $%s_pump (i32.const 1)) (i32.const 5)))' % module,
                 ' (call $assert (i32.eq (call $%s_close) (i32.const 5)))' % module]
    body += ['''
 (call $assert (i32.eq (call $clear (i32.const 1) (i32.const 2) (i32.const 3) (i32.const 4)) (i32.const -2147483626)))
 (call $assert (i32.eqz (call $begin (i32.const 1297044558))))
 (call $assert (i32.eq (call $motion (i32.const 1297044558) (i32.const 1) (i32.const 60) (i32.const 5000)) (i32.const -2147483626)))
 (call $assert (f32.eq (call $f32 (f32.const 1)) (f32.const 0)))
 (call $assert (f64.eq (call $f64 (f64.const 1)) (f64.const 0)))
''']
    for module in ('vision', 'callbacks', 'motion', 'floating'):
        body.append(' (call $assert (i32.eq (call $%s_status) (i32.const 5)))' % module)
    body += [' (call $emit (i32.const 256) (i32.const %d))' % len(PURE_PASS),
             ' (call $exit (i32.const 0)) unreachable))']
    return prelude('\n'.join(imports), PURE_PASS) + '\n'.join(body) + '\n'


def checksum(seed=1, iterations=10000):
    """Independent Python integer oracle for the documented synthetic inputs.

    This is not a native benchmark and never supplies a simulated ROS result.
    Every requested checksum uses explicit modulo2^32/modulo2^64 arithmetic.
    """
    if type(seed) is not int or not 0 <= seed <= 0xffffffff or type(iterations) is not int or not 1 <= iterations <= 1000000:
        raise ValueError('bounded exact seed/count required')
    result = 14695981039346656037
    for _ in range(iterations):
        samples = []
        for _ in range(4):
            seed = (seed * 1664525 + 1013904223) & 0xffffffff
            samples.append(seed % 5001)
        seed = (seed * 1664525 + 1013904223) & 0xffffffff
        requested = seed % 201 - 100
        command = 0 if min(samples) < 400 else requested
        result = ((result ^ (command & 0xffffffff)) * 1099511628211) & 0xffffffffffffffff
    return result.to_bytes(8, 'little')
