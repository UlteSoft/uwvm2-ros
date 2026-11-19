#!/usr/bin/env python3
"""Real VM self/foreign permission reports and registered message erasure.

Run through tools/ros_scene/in_existing_cgroup.py. Uses only ROSv2 imports,
with Wasm32/64, memory zero/one and interpreter/AOT. This suite does not claim
native driver, DDS, physical safety, or end-to-end timing qualification.
"""
from pathlib import Path
import argparse
import json
import struct
import subprocess
import time
from run_scene_vm import Guest, verify_gate, sha, GATE

QUERY = ['permission.report', 'module.describe', 'module.imports', 'module.exports', 'module.group', 'module.permission.report', 'module.importers', 'vm.describe']
CALLS = ['module.call.'+x for x in ('submit','take','respond','result','acknowledge','cancel')]


def record(*values):
    result = b''
    for value in values:
        if isinstance(value, str): value = value.encode()
        result += struct.pack('<Q', value if isinstance(value, int) else len(value))
        if isinstance(value, bytes): result += value
    return result


class Fixture(Guest):
    def __init__(self, names, width=32, memory=0, shared=False):
        super().__init__([dict(name=n, words=2, bytes=128) for n in names], width, memory)
        self.shared, self.extra_imports, self.extra_functions = shared, [], []
        self.open_import = False

    def blob(self, data):
        address = self.cursor; self.cursor += len(data)+8
        self.data.append(f'(data (memory $work) (i{self.width}.const {address}) "'+''.join(f'\\{c:02x}' for c in data)+'")')
        return address

    def request(self, name, data, expected=0, cap='i64.const 0', output=16384, capacity=32768, length=1024):
        address = self.blob(data)
        self.check(f'{cap} i32.const {self.memory} i64.const {address} i64.const {len(data)} i64.const {output} i64.const {capacity} i64.const {length} call ${name}', expected)

    def find(self, needle, expected=True):
        address = self.blob(needle.encode())
        self.check(f'i32.const {address} i32.const {len(needle)} call $contains', int(expected))

    def zero(self, address, size):
        for offset in range(0,size,8): self.equal(self.word(address+offset),0)

    def open(self, alias, expected=0, output=512):
        self.open_import = True
        return super().open(alias, expected, output)

    def render(self, export='_start'):
        imports = self.extra_imports + [f'(import "uwvm_ros_v2" "{row["name"]}" (func ${row["name"]} (param i64 i32 i64 i64 i64 i64 i64) (result i32)))' for row in self.operations]
        if self.open_import: imports.append('(import "uwvm_ros_v2" "capability.open" (func $open (param i32 i64 i64 i64) (result i32)))')
        memories = '(memory 1) '*self.memory+'(memory $work '+('i64 ' if self.width==64 else '')+('1 2 shared)' if self.shared else '1)')
        extend = 'i64.extend_i32_u' if self.width==64 else ''
        contains = f'''(func $contains (param $needle i32) (param $size i32) (result i32) (local $p i32) (local $j i32) (local $end i32)
            {self.word(1024)} i32.wrap_i64 i32.const 16384 i32.add local.set $end
            i32.const 16384 local.set $p
            block $missing loop $outer
              local.get $p local.get $size i32.add local.get $end i32.gt_u br_if $missing
              i32.const 0 local.set $j
              block $different loop $inner
                local.get $j local.get $size i32.eq if i32.const 1 return end
                local.get $p local.get $j i32.add {extend} i32.load8_u $work
                local.get $needle local.get $j i32.add {extend} i32.load8_u $work i32.ne br_if $different
                local.get $j i32.const 1 i32.add local.set $j br $inner
              end end
              local.get $p i32.const 1 i32.add local.set $p br $outer
            end end i32.const 0)'''
        return '(module\n'+'\n'.join(imports)+'\n'+memories+'\n'+'\n'.join(self.data)+'\n'+ \
            '\n'.join(f'(global ${name} (mut i64) (i64.const 0))' for name in sorted(self.globals))+'\n'+ \
            contains+'\n'+'\n'.join(self.checks+self.extra_functions)+'\n'+ \
            (f'(func (export "{export}")\n'+'\n'.join(self.body)+'\n)' if export else '')+'\n)\n'


class ObservationFPFixture(Fixture):
    """Check Wasm arithmetic after observation calls; no native-hook emulation."""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.fp_sites=0
        values32=[1,2**-24,-1,-(2**-24),2**-126,.5,2**-149,2,0]
        values64=[1,2**-53,-1,-(2**-53),2**-1022,.5,2**-1074,2,0]
        payload=struct.pack('<9f',*values32).ljust(64,b'\0')+struct.pack('<9d',*values64)
        encoded=''.join('\\'+format(byte,'02x') for byte in payload)
        self.data.append(f'(data (memory $work) (i{self.width}.const 8192) "{encoded}")')

    def check(self, expression, expected=0):
        super().check(expression, expected)
        if ' call $' not in ' '+expression:return
        self.fp_sites+=1;checks=[]
        for bits,offset,answers in [(32,8192,[0x3f800000,0xbf800000,0x00400000,2,0x7f800000]),
                                    (64,8256,[0x3ff0000000000000,0xbff0000000000000,0x0008000000000000,2,0x7ff0000000000000])]:
            def load(index):return f'i{self.width}.const {offset+index*(bits//8)} f{bits}.load $work'
            for left,right,operation,answer in zip([0,2,4,6,0],[1,3,5,7,8],['add','add','mul','mul','div'],answers):
                checks.append(f'{load(left)} {load(right)} f{bits}.{operation} i{bits}.reinterpret_f{bits} i{bits}.const {answer} i{bits}.ne if unreachable end')
            checks.append(f'{load(2)} f{bits}.sqrt {load(2)} f{bits}.sqrt f{bits}.eq if unreachable end')
        self.checks[-1]=self.checks[-1][:-1]+'\n'+'\n'.join(checks)+')'


def self_report(width, memory, legacy=False):
    g=Fixture(['permission.report'],width,memory)
    g.request('permission.report',record('rosv2','',0));g.equal(g.word(16384),1);g.equal(g.word(16392),1)
    g.find('main');g.find('robot');g.find('hidden-member',False)
    for family,supported in [('preloaddl',int(legacy)),('ros1v1',int(legacy)),('ros2v1',int(legacy)),('preloadwasm',1),('wasip1',1)]:
        g.request('permission.report',record(family,'',0),expected=0 if supported else 95)
        g.equal(g.word(16384),supported)
        if family=='wasip1':g.equal(g.word(16392),0)
    g.request('permission.report',record('rosv2','clock.read',0));g.equal(g.word(16408),1) # exactly one interface row
    g.request('permission.report',record('rosv2','does.not.exist',0),95)
    g.request('permission.report',record('not-a-family','',0),22)
    g.request('permission.report',record('rosv2','',65537),22)
    g.request('permission.report',record('rosv2','',0)+b'X',22)
    g.request('permission.report',record('rosv2','',0),22,cap='i64.const 1')
    g.request('permission.report',record('rosv2','',0),90,capacity=8)
    g.check(g.word(1024)+' i64.const 40 i64.le_u')
    return g


def denial(width,memory):
    g=Fixture(['permission.report'],width,memory)
    for family,function in [('wasip1',''),('wasip1','fd_write'),('preloaddl','')]:
        g.write(16384,12345);g.write(1024,6789)
        g.request('permission.report',record(family,function,0),13)
        g.equal(g.word(16384),12345);g.equal(g.word(1024),6789)
    g.request('permission.report',record('wasip1','fd_read',0));g.equal(g.word(16384),1)
    return g


def inspection(width,memory):
    g=Fixture(QUERY,width,memory)
    g.extra_imports=['(import "worker-secret" "worker_ping" (func $worker_ping))']
    for name in QUERY[1:]:
        request=record('unknown',*(('rosv2','',0) if name=='module.permission.report' else (0,)))
        g.write(16384,9001);g.write(1024,9002);g.request(name,request,13)
        g.equal(g.word(16384),9001);g.equal(g.word(1024),9002)
    g.request('module.describe',record('worker-view',0));g.find('worker-secret');g.find('preload-wasm');g.find('private-bystander',False)
    g.request('module.imports',record('worker-view',0));g.find('worker-secret',False)
    g.request('module.exports',record('worker-view',0));g.find('worker_ping');g.find('private-bystander',False)
    g.request('module.importers',record('worker-view',0));g.find('main');g.find('worker_ping');g.find('private-bystander',False)
    g.equal(g.word(16408),1);g.equal(g.word(16416),1) # exactly one declared import
    g.request('module.importers',record('worker-view',1));g.equal(g.word(16416),0)
    g.write(16384,9001);g.write(1024,9002)
    g.request('module.importers',record('worker-view',2),90)
    g.equal(g.word(16384),9001);g.equal(g.word(1024),9002)
    g.request('module.group',record('worker-view',0));g.find('worker-group');g.find('worker-secret');g.find('private-bystander');g.find('main',False)
    g.request('module.permission.report',record('worker-view','rosv2','',0));g.find('worker-secret');g.find('private-bystander',False)
    g.request('module.permission.report',record('worker-view','wasip1','',0));g.equal(g.word(16392),0)
    g.request('module.permission.report',record('worker-view','rosv2','no.such.function',0),95)
    g.request('vm.describe',record('worker-view',0),13) # module grants cannot authorize VM state
    g.request('module.importers',record('runtime-view',0),13)
    g.request('vm.describe',record('runtime-view',0));g.find('query-only');g.find('registered-declarations')
    g.find('worker-secret',False);g.find('private-bystander',False);g.find('/home/',False)
    g.equal(g.word(16408),17);g.equal(g.word(16416),17)
    g.request('vm.describe',record('runtime-view',17));g.equal(g.word(16416),0)
    g.request('vm.describe',record('runtime-view',0),90,capacity=8)
    g.request('vm.describe',record('runtime-view',0),22,cap='i64.const 1')
    g.request('permission.report',record('vm','',0),22) # self observation must not expose VM metadata
    # Self privilege report is independent of permission to list another group.
    g.request('permission.report',record('rosv2','permission.report',0));g.equal(g.word(16384),1)
    return g


def importer_self(width,memory,allowed):
    """Invoke observation inside a preload: its caller is still that preload."""
    w=Fixture(['permission.report','module.importers'],width,memory)
    w.request('permission.report',record('rosv2','',0));w.find('worker');w.find('main',False)
    w.write(16384,9221);w.write(1024,9222)
    w.request('module.importers',record('self-dependencies',0),0 if allowed else 13)
    if allowed:
        w.find('main');w.find('who');w.find('hidden-member',False)
        w.equal(w.word(16408),1);w.equal(w.word(16416),1)
    else:
        w.equal(w.word(16384),9221);w.equal(w.word(1024),9222)
    g=Fixture([],width,memory)
    g.extra_imports=['(import "worker" "who" (func $who))'];g.body=['call $who']
    return g,w


def message_pair(width,memory,shared=True,exported=False,clear=True):
    def input_state(fixture,values):
        if clear:fixture.zero(128,len(values)*8)
        else:
            for i,value in enumerate(values):
                expression=f'i64.const {value}' if isinstance(value,int) else value
                fixture.check(f'{fixture.word(128+8*i)} {expression} i64.ne')
    w=Fixture(CALLS,width,memory,shared);w.globals.add('exchange')
    w.call('module.call.take','global.get $exchange',[0,0],expected=11,output=2048,capacity=128,length=3072)
    init='\n'.join(w.body);w.body=[]
    w.extra_functions.append('(func (export "init") (param $cap i64) local.get $cap global.set $exchange '+init+')')
    w.call('module.call.take','global.get $exchange',[0,0],output=2048,capacity=128,length=3072)
    token=w.save('token',2048);w.equal(w.word(2056),8);w.equal(w.word(2064),0x12345678);input_state(w,[0,0])
    w.call('module.call.respond','global.get $exchange',[token,8,0x76543210],size=24,output=4096,capacity=16,length=5000)
    w.zero(2048,128);w.zero(3072,8);input_state(w,[token,8,0x76543210])
    w.extra_functions.append('(func (export "pump") '+'\n'.join(w.body)+')');w.body=[]
    w.call('module.call.take','global.get $exchange',[0,0],output=2048,capacity=128,length=3072)
    w.extra_functions.append('(func (export "take_only") '+'\n'.join(w.body)+')');w.body=[]
    w.zero(2048,128);w.zero(3072,8)
    w.extra_functions.append('(func (export "assert_erased") '+'\n'.join(w.body)+')');w.body=[]
    g=Fixture(CALLS,width,memory,shared)
    g.extra_imports=[f'(import "worker" "{name}" (func ${name} '+('(param i64)' if name=='init' else '')+'))' for name in ('init','pump','take_only','assert_erased')]
    cap=g.open('exchange')
    if not shared or exported:
        if exported:g.extra_functions.append('(export "memory" (memory $work))')
        g.call('module.call.submit',cap,[0,8,0x12345678],size=24,expected=95)
        return g,w
    g.body.append(cap+' call $init')
    g.call('module.call.submit',cap,[0,8,0x12345678],size=24)
    token=g.save('token');input_state(g,[0,8,0x12345678])
    g.body.append('call $pump')
    g.call('module.call.result',cap,[token,0],output=2048,capacity=128,length=3072)
    g.equal(g.word(2056),8);g.equal(g.word(2064),0x76543210);input_state(g,[token,0])
    g.call('module.call.submit',cap,[0,8,0x12345678],size=24);second=g.save('second')
    g.body.append('call $pump')
    g.call('module.call.result',cap,[second,0],output=2048,capacity=128,length=3072,expected=17)
    g.equal(g.word(2064),0x76543210) # refused overlap leaves the original result intact
    g.call('module.call.acknowledge',cap,[token,0],output=4096,capacity=0,length=5000)
    g.zero(2048,128);g.zero(3072,8);input_state(g,[token,0])
    g.call('module.call.result',cap,[second,0],output=2048,capacity=128,length=3072)
    g.call('module.call.acknowledge',cap,[second,0],output=4096,capacity=0,length=5000)
    g.zero(2048,128);g.zero(3072,8)
    g.call('module.call.take',cap,[0,0],expected=13) # exact client cannot impersonate receiver
    g.call('module.call.submit',cap,[0,8,0x12345678],size=24);token=g.save('token')
    g.body.append('call $take_only')
    g.call('module.call.cancel',cap,[token,0],capacity=0);g.body.append('call $assert_erased')
    # The access proof is local to a host call: changing the memory index must
    # never reuse a prior owned/shared declaration. Memory zero in the m1 case
    # is private; memory one in the m0 case does not exist.
    g.write(128,0);g.write(136,8);g.write(144,0x12345678)
    g.check(f'{cap} i32.const {1-memory} i64.const 128 i64.const 24 i64.const 2048 i64.const 128 i64.const 3072 call $module.call.submit',95)
    g.equal(g.word(144),0x12345678)
    # A refused out-of-bounds call cannot queue a message. Fresh access after
    # growth must resolve the new extent for both copy and delayed erasure.
    g.call('module.call.submit',cap,[0,8,0x12345678],size=24,expected=95,output=65536,capacity=128,length=5000)
    g.check(f'i{width}.const 1 memory.grow $work i{width}.const 1 i{width}.ne')
    g.call('module.call.submit',cap,[0,8,0x12345678],size=24,output=65536,capacity=128,length=131064)
    token=g.save('grown_token',65536);input_state(g,[0,8,0x12345678]);g.body.append('call $pump')
    g.call('module.call.result',cap,[token,0],output=65664,capacity=128,length=131056)
    g.equal(g.word(65680),0x76543210)
    g.call('module.call.acknowledge',cap,[token,0],output=4096,capacity=0,length=5000)
    g.zero(65664,128);g.zero(131056,8)
    return g,w


def main():
    global Fixture
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime',required=True,type=Path);parser.add_argument('--output',required=True,type=Path)
    parser.add_argument('--clear-input',choices=['default','true','false'],default='default')
    parser.add_argument('--engines',nargs='+',default=['int','aot']);parser.add_argument('--queries-only',action='store_true')
    parser.add_argument('--legacy-enabled',action='store_true',help='Expect compiled but disabled legacy ROS/preloaddl hosts')
    parser.add_argument('--fp-controls',action='store_true',help='With --queries-only, check rounding, subnormals, infinities and NaN after each observation')
    args=parser.parse_args();verify_gate();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    assert not args.fp_controls or args.queries_only,'FP observation fixtures reserve memory used by the separate message tests'
    if args.fp_controls:Fixture=ObservationFPFixture
    runtime=args.runtime.resolve()
    result=dict(complete=False,cgroup=str(GATE),memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),runtime=str(runtime),runtime_sha256=sha(runtime),runner_sha256=sha(__file__),cases=[])
    result['clear_input']=args.clear_input;result['fp_controls']=args.fp_controls;result['guests']=[]
    inputs=[Path(__file__).resolve(),Path(__file__).with_name('run_scene_vm.py'),runtime]
    build=runtime.parent/'full-build-results.json'
    if build.is_file():
        built=json.loads(build.read_text());assert built['complete'] and built['binary_sha256']==sha(runtime)
        inputs.append(build)
        if 'source_manifest' in built:
            manifest=Path(built['source_manifest'])
            assert sha(manifest)==built['source_manifest_sha256']
            inputs.append(manifest)
        else:
            inputs.append(Path(built['source_root'])/'source-manifest.json')
            result['production_source_root']=built['source_root']
    result['inputs']={str(path):sha(path) for path in inputs}
    for path in inputs[:2]:
        target=out/'fixture-source'/path.name;target.parent.mkdir(exist_ok=True);target.write_bytes(path.read_bytes())
        result['inputs'][str(target)]=sha(target)
    def save():(out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    def compile(name,fixture,export='_start'):
        wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(fixture.render(export))
        proc=subprocess.run(['wat2wasm','--enable-threads','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],capture_output=True,text=True)
        if proc.returncode:raise RuntimeError(proc.stderr)
        result['guests'].append(dict(name=name,fp_call_sites=getattr(fixture,'fp_sites',0),arithmetic_checks_per_site=12 if args.fp_controls else 0))
        return wasm
    def run(name,engine,guest,flags,worker=None,expected=True):
        verify_gate();command=[str(runtime),'--wasm-set-main-module-name','main',f'--runtime-{engine}','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags]
        if worker:command+=['--wasm-preload-library',str(worker),'worker']
        command+=['--run',str(guest)];start=time.monotonic();p=subprocess.run(command,capture_output=True,timeout=120)
        (out/(name+'.stdout')).write_bytes(p.stdout);(out/(name+'.stderr')).write_bytes(p.stderr)
        passed=(p.returncode==0)==expected
        result['cases'].append(dict(name=name,argv=command,returncode=p.returncode,passed=passed,seconds=time.monotonic()-start,guest_sha256=sha(guest),worker_sha256=sha(worker) if worker else None));save()
        if not passed:raise RuntimeError(name+': '+p.stderr.decode(errors='replace')[-6000:])
        print('PASS '+name,flush=True)
    base=['--ros-scene-create','robot','--ros-scene-bind','robot','main','--ros-scene-bind','robot','hidden-member']
    def expose(names):return [arg for name in names for arg in ['--ros-scene-interface','robot',name,'allow']]
    try:
        for width,memory in [(32,0),(32,1),(64,0),(64,1)]:
            label=f'w{width}-m{memory}'
            guest=compile('self-'+label,self_report(width,memory,args.legacy_enabled))
            denied=compile('denied-'+label,denial(width,memory))
            for engine in args.engines:
                run('self-'+label+'-'+engine,engine,guest,base)
                run('query-denial-'+label+'-'+engine,engine,denied,base+['--ros-scene','robot','permission.report','wasip1.fd_write','deny','--ros-scene','robot','permission.report','preloaddl','deny'])
            # Exact function attenuation is checked independently of the full foreign report.
            inspected=inspection(width,memory)
            # The function-denial case has a separate policy/guest to avoid intentionally denying aggregate pages.
            foreign=compile('foreign-'+label,inspected)
            worker=out/'foreign-worker.wasm';wat=out/'foreign-worker.wat';wat.write_text('(module (func (export "worker_ping")))')
            subprocess.run(['wat2wasm',str(wat),'-o',str(worker)],check=True)
            actions=','.join(n for n in QUERY[1:] if n!='vm.describe')
            foreign_flags=base+expose(QUERY[1:])+['--ros-scene-create','worker-group','--ros-scene-bind','worker-group','worker-secret','--ros-scene-bind','worker-group','private-bystander','--ros-scene-grant','robot','worker-view','module','worker-secret','metadata',actions,'--ros-scene-grant','robot','runtime-view','vm','runtime','metadata','vm.describe','--wasm-preload-library',str(worker),'worker-secret']
            for engine in args.engines:run('foreign-'+label+'-'+engine,engine,foreign,foreign_flags)
            for allowed in (False,True):
                own,peer=importer_self(width,memory,allowed)
                own=compile('importer-self-'+str(allowed)+'-'+label,own)
                peer=compile('importer-self-worker-'+str(allowed)+'-'+label,peer,'who')
                flags=base+['--ros-scene-create','worker-group','--ros-scene-bind','worker-group','worker','--ros-scene-interface','worker-group','module.importers','allow']
                if allowed:flags+=['--ros-scene-grant','worker-group','self-dependencies','module','worker','metadata','module.importers']
                for engine in args.engines:run('importer-self-'+str(allowed)+'-'+label+'-'+engine,engine,own,flags,peer)
            reduced=Fixture(['module.permission.report'],width,memory)
            for function in ('clock.read',''):
                reduced.write(16384,1221);reduced.write(1024,2112)
                reduced.request('module.permission.report',record('worker-view','rosv2',function,0),13)
                reduced.equal(reduced.word(16384),1221);reduced.equal(reduced.word(1024),2112)
            reduced.request('module.permission.report',record('worker-view','rosv2','clock.create',0))
            reduced=compile('foreign-reduced-'+label,reduced)
            for engine in args.engines:run('foreign-reduced-'+label+'-'+engine,engine,reduced,foreign_flags+['--ros-scene','robot','permission.report','rosv2.clock.read','deny'])
            if args.queries_only:continue
            main,worker_fixture=message_pair(width,memory,clear=args.clear_input!='false')
            messages=compile('messages-'+label,main);worker=compile('worker-'+label,worker_fixture,None)
            call_flags=base+['--ros-scene-bind','robot','worker']+expose(['capability.open',*CALLS])+['--ros-scene-call-budget','robot','65536','1','--ros-scene-call','robot','solver','main','worker','solve','request-octets','response-octets','64','64','2','10000000000','--ros-scene-grant','robot','exchange','module.call','solver','registered-message',','.join(CALLS)]
            if args.clear_input!='default':call_flags+=['--ros-scene-call-clear-input','robot','solver',args.clear_input]
            for engine in args.engines:run('messages-'+label+'-'+engine,engine,messages,call_flags,worker)
            for shared,exported,variant in [(False,False,'private-memory'),(True,True,'exported-memory')]:
                invalid,_=message_pair(width,memory,shared,exported);invalid=compile(variant+'-'+label,invalid)
                for engine in args.engines:run(variant+'-'+label+'-'+engine,engine,invalid,call_flags,worker)
        minimal=Fixture(['permission.report']);minimal.request('permission.report',record('rosv2','',0),13)
        guest=compile('hidden',minimal)
        for engine in args.engines:
            run('parent-hidden-'+engine,engine,guest,[],expected=False)
            run('query-hidden-'+engine,engine,guest,base+['--ros-scene-interface','robot','permission.report','hidden'],expected=False)
            run('query-function-denied-'+engine,engine,guest,base+['--ros-scene-interface','robot','permission.report','deny'])
        # A page can fill after 32 matches, but the whole consistent scan must
        # still fail atomically when its finite work budget is exceeded. The
        # nonmatching case prevents an empty result from bypassing the bound.
        worker=out/'scan-worker.wasm';wat=out/'scan-worker.wat'
        wat.write_text('(module (func (export "ping")))')
        subprocess.run(['wat2wasm',str(wat),'-o',str(worker)],check=True)
        for target in ('worker','main'):
            bounded=Fixture(['module.importers'])
            bounded.extra_imports=['(import "worker" "ping" (func))']*65536
            bounded.write(16384,1221);bounded.write(1024,2112)
            bounded.request('module.importers',record('bounded-view',0),90)
            bounded.equal(bounded.word(16384),1221);bounded.equal(bounded.word(1024),2112)
            guest=compile('bounded-scan-'+target,bounded)
            flags=base+expose(['module.importers'])+['--ros-scene-grant','robot','bounded-view','module',target,'metadata','module.importers']
            for engine in args.engines:run('bounded-scan-'+target+'-'+engine,engine,guest,flags,worker)
        result['complete']=True
    finally:
        result['inputs_unchanged']=all(sha(Path(path))==digest for path,digest in result['inputs'].items())
        result['complete'] &= result['inputs_unchanged']
        result['events_after']=(GATE/'memory.events').read_text();save()
    assert result['complete']


if __name__=='__main__':main()
