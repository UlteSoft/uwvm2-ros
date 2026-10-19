"""Real ROS import/function-reference fixtures; generation is not execution.

No policy, native callback DTO, caller identity or Runtime handle is fabricated.
A same-module malformed open returns22 AFTER actual identity, whereas a crossed
module chain returns13 BEFORE memory validation. Valid ungranted opens must
still return13 with their output sentinels unchanged. No endpoint is created.
"""
import argparse
import json
from pathlib import Path


def fixtures(version):
    if type(version) is not int or version not in (1, 2):
        raise ValueError('one actual ROS backend required')
    ros=f'uwvm_ros{version}_v1'
    raw=f'(import "{ros}" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))'
    own='(call $open (i32.const 0) (i64.const 0) (i64.const 0) (i64.const 16))'
    tail='(return_call $open (i32.const 0) (i64.const 0) (i64.const 0) (i64.const 16))'
    callback=f"""(module
 {raw}
 (memory (export "memory") 1 1)
 (func (export "probe") (result i32) {own})
 (func (export "tail_host") (result i32) {tail})
 (func (export "_start")
   (if (i32.ne {own} (i32.const 22)) (then unreachable))))
"""
    library=f"""(module
 (type $probe (func (result i32)))
 {raw}
 (import "callbacks" "probe" (func $callback (type $probe)))
 (import "callbacks" "tail_host" (func $callback_tail (type $probe)))
 (memory (export "memory") 1 1)
 (table 1 1 funcref)
 (tag $problem (export "problem") (param i32))
 (func $own (export "own") (type $probe) {own})
 (func (export "tail_host") (type $probe) {tail})
 ;; The exact root funcref is an ordinary Wasm value. Its owner is resolved by
 ;; the real Runtime; neither this i32 table selector nor a Guest token mints it.
 (func (export "callback_ref") (param $f (ref $probe)) (result i32)
   (call_ref $probe (local.get $f)))
 (func (export "callback_tail_ref") (param $f (ref $probe)) (result i32)
   (return_call_ref $probe (local.get $f)))
 (func (export "callback_indirect") (param $f funcref) (result i32)
   (table.set (i32.const 0) (local.get $f))
   (call_indirect (type $probe) (i32.const 0)))
 (func (export "callback_tail_indirect") (param $f funcref) (result i32)
   (table.set (i32.const 0) (local.get $f))
   (return_call_indirect (type $probe) (i32.const 0)))
 (func (export "other_library") (type $probe) (call $callback))
 (func (export "tail_other_library") (type $probe) (return_call $callback_tail))
 (func $recursive (export "recursive") (param $n i32) (result i32)
   (if (result i32) (i32.eqz (local.get $n)) (then {own})
     (else (return_call $recursive (i32.sub (local.get $n) (i32.const 1))))))
 (func $throw (export "throw") (i32.const 37) (throw $problem))
 (func (export "tail_throw") (return_call $throw))
 (func (export "tail_trap") unreachable)
 (func (export "_start")
   (if (i32.ne {own} (i32.const 22)) (then unreachable))))
"""
    imports="""
 (import "library" "own" (func $library (type $probe)))
 (import "library" "tail_host" (func $library_tail (type $probe)))
 (import "library" "callback_ref" (func $callback_ref (param (ref $probe)) (result i32)))
 (import "library" "callback_tail_ref" (func $callback_tail_ref (param (ref $probe)) (result i32)))
 (import "library" "callback_indirect" (func $callback_indirect (param funcref) (result i32)))
 (import "library" "callback_tail_indirect" (func $callback_tail_indirect (param funcref) (result i32)))
 (import "library" "other_library" (func $other (type $probe)))
 (import "library" "tail_other_library" (func $other_tail (type $probe)))
 (import "library" "recursive" (func $library_recursive (param i32) (result i32)))
 (import "library" "throw" (func $throw))
 (import "library" "tail_throw" (func $tail_throw))
 (import "library" "tail_trap" (func $tail_trap))
 (import "library" "problem" (tag $problem (param i32)))
"""
    main=f"""(module
 (type $probe (func (result i32)))
 {raw}
 {imports}
 (memory (export "memory") 1 1)
 (data (i32.const 0) "x")
 (func $assert (param $ok i32) (if (i32.eqz (local.get $ok)) (then unreachable)))
 (func $probe (export "probe") (type $probe) {own})
 (elem declare func $probe)
 (func $tail_host (type $probe) {tail})
 (func $tail_library (type $probe) (return_call $library))
 (func $recursive (param $n i32) (result i32)
   (if (result i32) (i32.eqz (local.get $n)) (then (call $probe))
     (else (return_call $recursive (i32.sub (local.get $n) (i32.const 1))))))
 (func $default_deny
   (i64.store (i32.const 16) (i64.const -1))
   (call $assert (i32.eq (call $open (i32.const 0) (i64.const 0) (i64.const 1) (i64.const 16)) (i32.const 13)))
   (call $assert (i64.eq (i64.load (i32.const 16)) (i64.const -1))))
 (func $restored
   (call $assert (i32.eq (call $probe) (i32.const 22))) (call $default_deny))
 (func (export "_start")
   (call $restored)
   (call $assert (i32.eq (call $tail_host) (i32.const 22)))
   (call $assert (i32.eq (call $recursive (i32.const 10000)) (i32.const 22)))
"""
    edges=[('direct','(call $library)'),('host-tail','(call $library_tail)'),
           ('typed-direct-tail','(call $tail_library)'),
           ('callback-ref','(call $callback_ref (ref.func $probe))'),
           ('callback-tail-ref','(call $callback_tail_ref (ref.func $probe))'),
           ('callback-indirect','(call $callback_indirect (ref.func $probe))'),
           ('callback-tail-indirect','(call $callback_tail_indirect (ref.func $probe))'),
           ('other-library','(call $other)'),('tail-other-library','(call $other_tail)'),
           ('recursive-library','(call $library_recursive (i32.const 128))')]
    for label,expression in edges:
        main+=f'   ;; {label}: cross-module calls never acquire the callee\'s policy.\n'
        main+=f'   (call $assert (i32.eq {expression} (i32.const 13))) (call $restored)\n'
    for target in ('$throw','$tail_throw'):
        main+=f"""   (call $assert (i32.eq
     (block $caught (result i32)
       (try_table (catch $problem $caught) (call {target}) unreachable)
       unreachable) (i32.const 37))) (call $restored)
"""
    main+='))\n'
    trap=main[:main.index(' (func (export "_start")')]+""" (func (export "_start")
   (call $restored) (call $tail_trap) unreachable))
"""
    # The real host reentry fixture has fixed import/defined indices consumed
    # by ancestry_reentry.cc. These are actual Wasm indices, never principal IDs.
    reentry_root=f"""(module
 {raw}
 (import "reentry-library" "run" (func $library (result i32)))
 (import "ancestry-host" "reenter" (func $reenter (result i32)))
 (memory (export "memory") 1 1)
 (func (export "probe") (result i32) {own}) ;; index3
 (func (export "_start")                   ;; index4
   (if (i32.ne (call $reenter) (i32.const 22)) (then unreachable))
   (if (i32.ne (call $library) (i32.const 13)) (then unreachable))
   (if (i32.ne {own} (i32.const 22)) (then unreachable))))
"""
    reentry_library="""(module
 (import "ancestry-host" "reenter" (func $reenter (result i32)))
 (func (export "run") (result i32) (return_call $reenter)))
"""
    return {'callbacks':callback,'library':library,'main':main,'trap':trap,
            'reentry-root':reentry_root,'reentry-library':reentry_library}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ros-version',type=int,choices=(1,2),required=True)
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args()
    args.out.mkdir(parents=True,exist_ok=False)
    rows=[]
    for name,text in fixtures(args.ros_version).items():
        path=args.out/(name+'.wat');path.write_text(text,encoding='utf-8')
        rows.append({'id':name,'wat':str(path.resolve()),'status':'SOURCE_GENERATED_NOT_VALIDATED_OR_EXECUTED'})
    (args.out/'fixtures.json').write_text(json.dumps({'ros_version':args.ros_version,'fixtures':rows,
        'runtime_execution':'NOT_EXECUTED','capability_grants':[]},indent=2)+'\n')
    return 0


if __name__=='__main__':
    raise SystemExit(main())
