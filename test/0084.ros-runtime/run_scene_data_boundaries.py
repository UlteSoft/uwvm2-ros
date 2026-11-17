#!/usr/bin/env python3
"""Exercise all buffer/publish wrappers with actual FP and software-MTE boundaries.

Every target checks normal, short-output and exact replay behavior. Real tagged
memory faults separately target request/output/length; diagnostic alias cases
prove normalized overlap rejection without state changes. Guest arithmetic
checks follow every FP return under both engines and memory32/64 index zero/one.
This qualifies the VM boundary, not hardware MTE or native robotics providers.
All tests inherit the existing 64 GiB zero-swap cgroup.
"""
import argparse,json,resource,signal,subprocess,sys,time
from pathlib import Path
sys.dont_write_bytecode=True
import run_scene_vm as base
from run_scene_fp import FPGuest
from run_scene_memtag import TaggedGuest,add_private_instructions
from run_scene_buffers import layout,plan,PAYLOAD,PAYLOAD_SHA,u64,blob
ROOT,GATE=base.ROOT,base.GATE
TARGETS=['buffer.'+name for name in ['create','write','seal','read','describe']]+['artifact.publish']
OPERATIONS=[dict(name=name,words=0,bytes=0) for name in TARGETS]
class DataCalls:
    def address(self,value,tag=None):return self.pointer(value,tag) if isinstance(self,TaggedGuest) else value
    def raw(self,name,cap,payload,patches=(),expected=0,capacity=512,output=512,length=1024,request_tag=None,output_tag=None,length_tag=None):
        for offset,byte in enumerate(payload):
            self.body.append(f'i{self.width}.const {self.address(128+offset)} i32.const {byte} i32.store8 $work')
        for offset,value in patches:self.write(128+offset,value)
        self.check(f'{cap} i32.const {self.memory} i64.const {self.address(128,request_tag)} i64.const {len(payload)} '
                   f'i64.const {self.address(output,output_tag)} i64.const {capacity} i64.const {self.address(length,length_tag)} call ${name}',expected)
class DataFP(DataCalls,FPGuest):pass
class DataTagged(DataCalls,TaggedGuest):pass
def configured():
    return [*base.policy(OPERATIONS),'--ros-scene-grant','robot','data','buffer','sensor','sealed-buffer',
        ','.join(TARGETS[:-1]),'--ros-scene-buffer-budget','robot','64','2','32','2',*plan('data'),
        '--ros-scene-grant','robot','cache','cache','cache-data','volatile-output',
        'artifact.publish,artifact.begin,artifact.append,artifact.commit',
        '--ros-scene-interface','robot','artifact.begin','allow','--ros-scene-interface','robot','artifact.append','allow',
        '--ros-scene-interface','robot','artifact.commit','allow','--ros-scene-output-budget','robot','32','1','112',
        '--ros-scene-output','robot','cache','32','1','32','16','--ros-scene-publication-budget','robot','512','1',
        '--ros-scene-publication','robot','cache','512','1']
def guest(width,memory,scenario):
    tagged=scenario not in ('fp','denied','no-grants');g=(DataTagged if tagged else DataFP)(OPERATIONS,width,memory)
    if tagged:
        g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
        g.check('call $tag.status'+(' i32.wrap_i64' if width==64 else ''),2)
    if scenario in ('denied','no-grants'):
        for name in TARGETS:g.raw(name,'i64.const 1',bytes(16) if name=='artifact.publish' else b'',expected=13 if scenario=='denied' else 9)
        return g
    cap=g.open('data');cache=g.open('cache');mismatch=scenario.split('-')[1:] if scenario.startswith('mismatch-') else []
    def invoke(name,cap,payload,patches=(),required=8,replay=False):
        if mismatch and name==mismatch[0]:
            g.raw(name,cap,payload,patches,expected=22,capacity=required,**{mismatch[1]+'_tag':5});return False
        if scenario=='aliases':
            for slot in (512,128):
                g.write(512,0x123456789abcdef);g.write(1024,0x3456789)
                g.raw(name,cap,payload,patches,expected=22,capacity=required,length=slot,length_tag=0)
                g.equal(g.word(512),0x123456789abcdef);g.equal(g.word(1024),0x3456789)
        g.write(512,0x123456789abcdef)
        g.raw(name,cap,payload,patches,expected=90,capacity=required-1)
        g.equal(g.word(1024),required);g.equal(g.word(512),0x123456789abcdef)
        g.raw(name,cap,payload,patches,capacity=required);g.equal(g.word(1024),required)
        if replay:
            saved=[g.save('replay-'+name+'-'+str(n),512+8*n) for n in range(required//8)]
            g.raw(name,cap,payload,patches,capacity=required)
            for n,value in enumerate(saved):g.check(g.word(512+8*n)+' '+value+' i64.ne')
        return True
    if not invoke('buffer.create',cap,layout()):return g
    handle=g.save('object')
    if not invoke('buffer.write',cap,u64(0)+u64(0)+blob(PAYLOAD),[(0,handle)],replay=True):return g
    g.equal(g.word(512),8)
    if not invoke('buffer.seal',cap,u64(0)+blob(PAYLOAD_SHA),[(0,handle)],16,True):return g
    g.check(g.word(512)+' '+cap+' i64.ne');g.check(g.word(520)+' '+handle+' i64.ne')
    if not invoke('buffer.read',cap,u64(0)+u64(0)+u64(8),[(0,handle)],16,True):return g
    g.equal(g.word(512),8);g.equal(g.word(520),int.from_bytes(PAYLOAD,'little'))
    if not invoke('buffer.describe',cap,u64(0),[(0,handle)],len(layout())+21):return g
    if not invoke('artifact.publish',cache,u64(1)+u64(8)+PAYLOAD,required=72,replay=True):return g
    for n,value in [(0,1),(1,8),(3,0),(4,8),(5,8),(6,8),(7,2),(8,1)]:g.equal(g.word(512+n*8),value)
    if scenario=='aliases':
        g.raw('buffer.read',cap,u64(0)+u64(0)+u64(8),[(0,handle)],capacity=16,length_tag=0)
        g.equal(g.word(520),int.from_bytes(PAYLOAD,'little'))
    g.check(cap+' call $close');g.check(cache+' call $close');return g

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--prepare-only', action='store_true')
    parser.add_argument('--source-root', type=Path, default=ROOT, help='Frozen production source tree used to build the qualified executable')
    args = parser.parse_args(); base.verify_gate()
    if not args.prepare_only and args.runtime is None:
        parser.error('--runtime is required without --prepare-only')
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    out = args.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    paths = [Path(__file__), *[ROOT/'test/0084.ros-runtime'/name for name in
             ('run_scene_fp.py', 'run_scene_memtag.py', 'run_scene_vm.py', 'run_scene_buffers.py')]]
    if not args.prepare_only:
        paths += [ROOT/'src/uwvm2/uwvm/imported/scene/host.h',
                  *[ROOT/'src/uwvm2/imported/ros/scene'/name for name in
                    ('output_contract.h', 'outputs.h', 'buffer_contract.h', 'buffers.h', 'broker.h', 'deployment.h')]]
    runtime = args.runtime.resolve() if args.runtime else None
    if runtime: paths.append(runtime)
    production_root = args.source_root.resolve(); assert (production_root/'src').is_dir()
    paths = [production_root/path.resolve().relative_to(ROOT) if path.resolve().is_relative_to(ROOT/'src') else path.resolve() for path in paths]
    pins = lambda: {str(path): base.sha(path) for path in paths}
    report = dict(complete=False, prepare_only=args.prepare_only, inputs=pins(),
                  scope=__doc__,
                  cgroup=str(GATE), inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
                  core_limit=0, events_before=(GATE/'memory.events').read_text(), guests=[], cases=[])

    report['production_source_root'] = str(production_root)
    report['fixture_source_root'] = str(ROOT)

    def save():
        report['events_after'] = (GATE/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    scenarios = ['fp', 'denied', 'no-grants', 'valid', 'aliases']
    scenarios += ['mismatch-'+name+'-'+slot for name in TARGETS for slot in ('request','output','length')]
    try:
        for width, memory in ((32, 0), (64, 0), (32, 1), (64, 1)):
            for scenario in scenarios:
                tagged = scenario not in ('fp', 'denied', 'no-grants')
                g = guest(width, memory, scenario); name = f'{scenario}-memory{width}-index{memory}'
                wat, wasm = out/(name+'.wat'), out/(name+'.wasm'); wat.write_text(g.render())
                argv = ['wat2wasm', '--enable-memory64', '--enable-multi-memory', '--debug-names', str(wat), '-o', str(wasm)]
                result = subprocess.run(argv, capture_output=True, timeout=30)
                (out/(name+'-compile.stderr')).write_bytes(result.stderr)
                assert result.returncode == 0, result.stderr.decode(errors='replace')
                if tagged: wasm.write_bytes(add_private_instructions(wasm.read_bytes(), len(g.checks), memory))
                report['guests'].append(dict(name=name, argv=argv, sha256=base.sha(wasm), assertions=len(g.checks),
                                            fp_sites=getattr(g, 'fp_sites', 0), arithmetic_checks_per_site=0 if tagged else 12))
                save()
                if args.prepare_only: continue
                flags = (base.policy(OPERATIONS, 'deny' if scenario == 'denied' else 'allow')
                         if scenario in ('denied', 'no-grants') else configured())
                if scenario == 'no-grants':
                    for dependency in ('artifact.begin','artifact.append','artifact.commit'):
                        flags += ['--ros-scene-interface','robot',dependency,'allow']
                for engine in ('int', 'aot'):
                    base.verify_gate(); run_name = name+'-'+engine
                    argv = [str(runtime), '--wasm-set-main-module-name', 'main', '--runtime-'+engine,
                            '--runtime-compile-threads', '1', '--log-color', 'disable', *flags]
                    if engine == 'aot':
                        argv += ['--runtime-llvm-jit-policy', 'max', '--runtime-llvm-jit-cache-path', 'disable']
                    if tagged:
                        argv += ['--runtime-memtag-backend', 'software', '--runtime-memtag-mode', 'sync']
                        if scenario == 'aliases': argv += ['--runtime-memtag-action', 'silent-log', 'err']
                    argv += ['--run', str(wasm)]
                    start = time.monotonic(); result = subprocess.run(argv, capture_output=True, timeout=120)
                    (out/(run_name+'.stdout')).write_bytes(result.stdout); (out/(run_name+'.stderr')).write_bytes(result.stderr)
                    errors = result.stderr.decode(errors='replace'); crash = scenario.startswith('mismatch-')
                    expected = -signal.SIGILL if crash else 0; passed = result.returncode == expected; violations = []
                    if crash:
                        slot = {'request': 128, 'output': 512, 'length': 1024}[scenario.split('-')[2]]
                        reason = 'Reason: host interface memory tag mismatch' if engine == 'int' else 'Reason: memory tag mismatch'
                        passed = passed and all(token in errors for token in
                            ('[crash] memory-protection', reason, 'Exit status: 122', 'Fault action: crash (OS core-dump policy applies)',
                             f'Memory index: {memory}', f'Pointer: {g.pointer(slot, 5)}', 'Pointer tag: 5', 'Allocation tag: 3'))
                    if scenario == 'aliases':
                        violations = [json.loads(line) for line in errors.splitlines() if line.startswith('{"schema":"uwvm.memtag.violation.')]
                        passed = passed and len(violations) >= 13 and all(row['pointer_tag'] == 0 and row['allocation_tag'] == 3 and
                            row['memory'] == memory and row['pointer'] in (128, 512, 1024) for row in violations)
                    report['cases'].append(dict(name=run_name, argv=argv, exit=result.returncode, expected=expected, passed=passed,
                                                seconds=time.monotonic()-start, violations=violations))
                    save(); assert passed, (run_name, result.returncode, errors[-5000:])
                    print('PASS', run_name, flush=True)
        report['inputs_after'] = pins()
        assert report['inputs_after'] == report['inputs'], 'qualification inputs changed'
        report['complete'] = True
    finally:
        save()


if __name__ == '__main__':
    main()
