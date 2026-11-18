#!/usr/bin/env python3
"""Run managed FF instructions as Wasm in the VM's ordinary int and AOT entry points."""
import argparse
import hashlib
import json
import resource
import subprocess
from pathlib import Path

from vm_diagnostics import fault_matches
from vm_matrix import Program, assertion, c, mem, module, op, sec, u


def fixtures(wide):
    program = Program(wide)
    bits = program.bits
    num = program.n
    local = b'\x20\x00'
    save = b'\x21\x00'
    drop = b'\x1a'
    eq = b'\x51' if wide else b'\x46'
    true = b'\x04\x40\x00\x0b'
    claim, claimz, reserve, activate, release = range(0x40, 0x45)
    def perform(instruction, pointer, length=16):
        return pointer + num(length) + op(instruction)
    for shared in (False, True):
        scope = 'shared' if shared else 'private'
        def fixture(name, code, expect=0, disabled=False):
            return (scope + '-' + name, module(code,wide,shared=shared), expect, disabled)
        code = num(0) + c(91) + mem(0x36) + perform(claimz,num(0)) + save
        code += assertion(local + mem(0x28),0)
        code += local + op(0x21) + num(0) + eq + true
        # The two allocations have a gap of exactly 64 bytes. Compare actual
        # runtime tags, independent of the entropy bytes chosen by the host.
        code += local + op(0x21) + perform(reserve,num(80)) + save
        code += local + op(0x21) + eq + true
        code += perform(activate,local) + save
        code += local+c(55)+mem(0x36)+assertion(local+mem(0x28),55)
        code += assertion(perform(release,local),80,bits)
        code += assertion(perform(release,num(0)+op(0x27)),0,bits)
        code += assertion(perform(release,perform(reserve,num(16))),16,bits)
        yield fixture('lifecycle',code)
        code = perform(reserve,num(16)) + mem(0x28) + drop
        yield fixture('reserved-pointer-access',code,122)
        code = perform(claim,num(16)) + save + perform(release,local) + drop + local + mem(0x28) + drop
        yield fixture('released-pointer-access',code,122)
        code = perform(claim,num(16)) + save + perform(release,local,32) + drop
        yield fixture('release-wrong-extent',code,122)
        code = perform(claim,num(16)) + save + perform(activate,local) + drop
        yield fixture('activate-active',code,122)
        code = perform(reserve,num(16)) + drop + perform(claim,num(16)) + drop
        yield fixture('double-reserve',code,122)
        code = perform(claim,num(16)) + save + local + num(16) + op(0x2b)
        yield fixture('legacy-overwrite',code,122)
        code = perform(reserve,num(16)) + save + num(96) + local + op(0x25) + num(16) + op(0x2b)
        yield fixture('legacy-same-neighbor',code,122)
        code = num(0)+c(123)+mem(0x36)
        for instruction in (claim,reserve,activate,release,claimz):
            code += assertion(perform(instruction,num(0)),0,bits)
        code += assertion(num(0)+mem(0x28),0)
        yield fixture('identity-zero',code,disabled=True)

        # Two manifest globals and one managed extent share a canonical owner.
        custom_name = b'uwvm.memtag.linear.globals'
        payload = u(2) + u(2) + u(0)+u(16)+u(16) + u(0)+u(48)+u(16)
        manifest = sec(0,u(len(custom_name))+custom_name+payload)
        def static_fixture(name,code,expect):
            return (scope+'-'+name,module(code,wide,shared=shared)+manifest,expect,False)
        color = lambda offset: num(offset)+op(0x27)+op(0x21)
        code = color(16) + color(48) + eq + true
        code += perform(claim,num(32)) + save
        code += color(16) + local + op(0x21) + eq + true
        code += color(48) + local + op(0x21) + eq + true
        code += assertion(perform(release,local),32,bits)
        yield static_fixture('manifest-neighbors',code,0)
        yield static_fixture('manifest-release',perform(release,num(16)+op(0x27))+drop,122)
        yield static_fixture('manifest-raw-overwrite',num(16)+num(16)+op(0x2b),122)


def threaded_fixture(reject):
    """A preloaded memory owner is shared by the main VM thread and its Wasm child."""
    # The child reserves/claims a distinct interval, publishes a pointer using a
    # seq-cst atomic, and exits. Completion is finite, including a failed spawn.
    guest = b'\0asm\x01\0\0\0'
    guest += sec(1,bytes.fromhex('04 60 00 00 60 02 7f 7f 00 60 01 7f 00 60 01 7f 01 7f'))
    guest += sec(2,u(3)+b'\x03env\x06memory\x02\x03\x01\x02'
                 + b'\x16wasi_snapshot_preview1\x09proc_exit\x00\x02'
                 + b'\x04wasi\x0cthread-spawn\x00\x03')
    guest += sec(3,b'\x02\x00\x01')
    guest += sec(7,b'\x03\x06_start\x00\x02\x11wasi_thread_start\x00\x03\x06memory\x02\x00')
    main = Program()
    main.add(c(0)+c(16)+op(0x40)+b'\x21\x00')
    main.add(c(64)+b'\x21\x01\x02\x40\x03\x40')
    main.add(c(0)+b'\x10\x01'+c(0)+b'\x4a\x0d\x01')
    main.add(b'\x20\x01'+c(1)+b'\x6b\x22\x01\x45\x04\x40\x00\x0b')
    main.add(c(4100)+c(0)+c(1000000,64)+b'\xfe\x01\x02\x00\x1a\x0c\x00\x0b\x0b')
    main.add(c(4096)+c(0)+c(5000000000,64)+b'\xfe\x01\x02\x00\x1a')
    main.eq(c(4096)+b'\xfe\x10\x02\x00',1)
    # Compare independently obtained logical tags. Release must go through the
    # same directory the child used when accessing the imported memory.
    main.add(b'\x20\x00'+op(0x21)+c(4112)+b'\xfe\x10\x02\x00'+op(0x21)+b'\x46\x04\x40\x00\x0b')
    main.eq(c(4112)+b'\xfe\x10\x02\x00'+c(16)+op(0x44),16)
    main.eq(b'\x20\x00'+c(16)+op(0x44),0)
    child = Program()
    if reject:
        child.add(c(0)+op(0x27)+c(32)+op(0x44)+b'\x1a')
    else:
        child.add(c(4112)+c(16)+c(16)+op(0x42)+c(16)+op(0x43)+b'\xfe\x17\x02\x00')
    child.add(c(4096)+c(1)+b'\xfe\x17\x02\x00'+c(4096)+c(1)+b'\xfe\x00\x02\x00\x1a')
    bodies=[b'\x01\x02\x7f'+main.code+b'\x0b',b'\x00'+child.code+b'\x0b']
    return guest+sec(10,u(2)+b''.join(u(len(body))+body for body in bodies))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--vm', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    out = args.output.resolve()
    out.mkdir(parents=True,exist_ok=False)
    vm = args.vm.resolve()
    with vm.open('rb') as f:
        report = {'vm_sha256':hashlib.file_digest(f,'sha256').hexdigest(),'rows':[]}
    for wide in (False,True):
        for label,wasm,expected,disabled in fixtures(wide):
            name = ('memory64-' if wide else 'memory32-') + label
            path = out / (name+'.wasm')
            path.write_bytes(wasm)
            for engine in ('int','aot'):
                command = [str(vm),'--debug-fault-action','exit','--runtime-'+engine,'--wasm-feature-enable-memory64',
                           '--wasm-feature-enable-threads','--wasm-set-start-func','0']
                if engine=='aot':
                    command += ['--runtime-llvm-jit-cache-path','disable','--runtime-compile-threads','0']
                if disabled:
                    command += ['--runtime-disable-memtag']
                else:
                    command += ['--runtime-memtag-backend','software','--runtime-memtag-mode','sync',
                                '--runtime-memtag-action','crash']
                command += ['--',str(path)]
                result = subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=90)
                output = result.stdout.decode(errors='replace')
                (out / (name+'-'+engine+'.log')).write_text(output)
                ok = result.returncode==0 if expected==0 else fault_matches(result.returncode,output,expected,
                    "memory tag mismatch" if label.endswith("-access") else "managed memory allocation rejected")
                report['rows'].append({'name':name,'engine':engine,'command':command,'expected_exit':expected,
                                       'actual_exit':result.returncode,'passed':ok,'guest_sha256':hashlib.sha256(wasm).hexdigest()})
                (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
                print(name,engine,result.returncode,ok,flush=True)
    owner = out/'thread-owner.wasm'
    owner.write_bytes(b'\0asm\x01\0\0\0'+sec(5,b'\x01\x03\x01\x02')+sec(7,b'\x01\x06memory\x02\x00'))
    for reject in (False,True):
        name = 'threaded-child-' + ('reject-release' if reject else 'shared-owner')
        guest = threaded_fixture(reject)
        path = out/(name+'.wasm')
        path.write_bytes(guest)
        for engine in ('int','aot'):
            command = [str(vm),'--debug-fault-action','exit','--runtime-'+engine,'--runtime-compile-threads','0',
                       '--wasm-feature-enable-threads','--wasi-threads','3','3','1048576','16777216','67108864','131072',
                       '--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main',
                       '--wasip1-single-interface','main','proc_exit','allow','--wasip1-single-grant','main','proc-exit',
                       '--wasm-preload-library',str(owner),'env','--wasithread-single-create','main','--wasithread-single-enable','main',
                       '--wasithread-single-interface','main','thread-spawn','allow']
            if engine=='aot':
                command += ['--runtime-llvm-jit-cache-path','disable','--runtime-llvm-jit-policy','debug']
            command += ['--run',str(path)]
            result = subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=40)
            output = result.stdout.decode(errors='replace')
            (out/(name+'-'+engine+'.log')).write_text(output)
            ok = fault_matches(result.returncode,output,122,'managed memory allocation rejected') if reject else result.returncode==0
            report['rows'].append({'name':name,'engine':engine,'command':command,'expected_exit':122 if reject else 0,
                                   'actual_exit':result.returncode,'passed':ok,'guest_sha256':hashlib.sha256(guest).hexdigest()})
            print(name,engine,result.returncode,ok,flush=True)
    report['complete'] = all(row['passed'] for row in report['rows'])
    (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    return 0 if report['complete'] else 1


if __name__=='__main__':
    raise SystemExit(main())
