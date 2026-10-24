#!/usr/bin/env python3
"""Guarded source-policy regressions; no compiler, Guest, ROS or robot runs.

Root invokes this only from a complete current source in the original64GiB
cgroup. Synthetic Wasm exercises the independent boundary parser, not the full
validator. All real copy/dependency fixtures live in fresh owned output; their
small dummy SDK bodies are explicitly policy fixtures, never native evidence.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import stat
import sys
import time
import types
import unittest

sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[5]
MIB=1024**2;GIB=1024**3
CONTEXT={}


def need(ok,reason):
    if not ok:raise ValueError(reason)


def read(path,maximum):
    path=Path(path);need(path.is_absolute() and path.resolve(strict=True)==path,'physical fixture control required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=maximum,'bounded regular fixture control required')
        body=bytearray()
        while True:
            data=os.read(fd,min(65536,maximum+1-len(body)))
            if not data:break
            body.extend(data);need(len(body)<=maximum,'fixture control grew')
        last,named=os.fstat(fd),path.stat(follow_symlinks=False)
        need(len(body)==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k)
             for k in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')),'fixture control changed')
        return bytes(body)
    finally:os.close(fd)


def u(value):
    """Encode only tiny fixture section lengths/indices, not production I/O."""
    result=bytearray()
    while True:
        byte=value&127;value>>=7;result.append(byte|(128 if value else 0))
        if not value:return bytes(result)


def name(text):
    raw=text.encode();return u(len(raw))+raw


def section(identifier,body):return bytes([identifier])+u(len(body))+body


def fixture_wasm(*,foreign=False,optional=False,bad_signature=False,imported_memory=False,
                 start=False,unbounded=False,shared=False,imported_entry=False):
    """Small real core-byte sections using the actual current ABI declaration."""
    abi=CONTEXT['abi'];cap=next(row for row in abi['modules']['uwvm_ros1_v1']['functions'] if row['name']=='capability_open')
    parameters=[p['type'] for p in cap['params']]+(['i32'] if bad_signature else [])
    shapes=[(parameters,cap['results']),(['i32']*4,['i32']),(['i32'],[]),([],[])]
    code={'i32':b'\x7f','i64':b'\x7e','f32':b'\x7d','f64':b'\x7c'}
    types_=u(len(shapes))
    for args,results in shapes:types_+=b'\x60'+u(len(args))+b''.join(code[x] for x in args)+u(len(results))+b''.join(code[x] for x in results)
    rows=[('uwvm_ros2_v1' if foreign else 'uwvm_ros1_v1','capability_open',0),
          ('wasi_snapshot_preview1','fd_read',1),('wasi_snapshot_preview1','fd_write',1),
          ('wasi_snapshot_preview1','clock_time_get' if optional else 'proc_exit',2)]
    imports=u(len(rows)+int(imported_memory))+b''.join(name(m)+name(n)+b'\0'+u(t) for m,n,t in rows)
    if imported_memory:imports+=name('env')+name('memory')+b'\x02\x01'+u(1)+u(1024)
    data=b'\0asm\x01\0\0\0'+section(1,types_)+section(2,imports)+section(3,b'\x01\x03')
    if not imported_memory:
        limits=(b'\x03'+u(1)+u(1024)) if shared else (b'\x00'+u(1)) if unbounded else b'\x01'+u(1)+u(1024)
        data+=section(5,b'\x01'+limits)
    data+=section(7,b'\x02'+name('memory')+b'\x02\x00'+name('_start')+b'\x00'+u(0 if imported_entry else 4))
    if start:data+=section(8,u(4))
    return data+section(10,b'\x01\x02\x00\x0b')


class Policy(unittest.TestCase):
    def setUp(self):
        CONTEXT['guard']();self.root=CONTEXT['out']/self._testMethodName;self.root.mkdir(mode=0o700)
        self.source=self.root/'source';self.source.mkdir();self.output=self.root/'output';self.output.mkdir()
        common=CONTEXT['common'];guard=CONTEXT['guard']
        def pin(path):guard();return common.opened(Path(path).resolve(strict=True))
        self.frame=types.SimpleNamespace(source=self.source,out=self.output,roots=[],pin=pin,frozen={})
    def original(self,relative,body=b'// policy fixture only\n'):
        path=self.source/relative;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(body)
        self.frame.frozen[relative]={'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest()};return path
    def boundary(self,**options):
        path=self.output/'guest.wasm';path.write_bytes(fixture_wasm(**options))
        return CONTEXT['producer'].binary_boundary(self.frame,CONTEXT['support'],CONTEXT['inspector'],CONTEXT['abi'],path,'_start')
    def test_real_sections_admit_only_local_bounded_command(self):
        row=self.boundary();self.assertEqual(row['local_entry'],0);self.assertEqual(row['default_authority'],[])
    def test_foreign_ros_backend_refused(self):
        with self.assertRaisesRegex(ValueError,'foreign|optional'):self.boundary(foreign=True)
    def test_optional_wasi_refused(self):
        with self.assertRaisesRegex(ValueError,'foreign|optional'):self.boundary(optional=True)
    def test_actual_ros_signature_mismatch_refused(self):
        with self.assertRaisesRegex(ValueError,'ABI differs'):self.boundary(bad_signature=True)
    def test_imported_memory_refused(self):
        with self.assertRaisesRegex(ValueError,'function-only'):self.boundary(imported_memory=True)
    def test_implicit_start_refused(self):
        with self.assertRaisesRegex(ValueError,'implicit start'):self.boundary(start=True)
    def test_unbounded_memory_refused(self):
        with self.assertRaisesRegex(ValueError,'own64MiB'):self.boundary(unbounded=True)
    def test_shared_memory_refused(self):
        with self.assertRaisesRegex(ValueError,'shared/memory64'):self.boundary(shared=True)
    def test_imported_entry_refused(self):
        with self.assertRaisesRegex(ValueError,'local void entry'):self.boundary(imported_entry=True)
    def stage(self,language):
        producer,support=CONTEXT['producer'],CONTEXT['support']
        for rel in ('sdk/origin/include/policy.h',producer.ENTRY+'/entry.c',producer.ENTRY+'/command.go',
                    producer.ENTRY+'/protocol.go',producer.ENTRY+'/command.ts',producer.ENTRY+'/protocol.ts',
                    producer.EXAMPLES+'/lowlevel.c',producer.EXAMPLES+'/lowlevel.cpp',producer.EXAMPLES+'/values_c.h',
                    producer.EXAMPLES+'/lowlevel.go',producer.EXAMPLES+'/values.go',producer.EXAMPLES+'/go.mod',
                    producer.EXAMPLES+'/lowlevel.ts',producer.EXAMPLES+'/values.ts',
                    'sdk/origin/tinygo/go.mod','sdk/origin/assemblyscript/ros1/roswasm.ts'):
            self.original(rel,('exact policy body '+rel+'\n').encode())
        generated=self.root/'generated';generated.mkdir();self.frame.roots=[generated];outputs={}
        for filename in producer.OUTPUTS:
            path=generated/filename;path.write_bytes(('full generated policy body '+filename+'\n').encode());outputs[filename]=self.frame.pin(path)
        cell=self.output/'cell';cell.mkdir()
        stage,rows=producer.stage_product(self.frame,support,{'language':language},cell,{'all_generated_outputs':outputs})
        self.assertEqual({path.name for path in (stage/'native-family').iterdir()},producer.OUTPUTS)
        self.assertTrue(all(row['source']['sha256']==row['copy']['sha256'] and
                            (row['source']['device'],row['source']['inode'])!=(row['copy']['device'],row['copy']['inode']) for row in rows))
        return producer,stage
    def test_tinygo_main_avoids_other_go_package_and_cgo_siblings(self):
        producer,stage=self.stage('tinygo');main=stage/producer.EXAMPLES
        self.assertEqual({p.name for p in main.iterdir()},{'lowlevel.go','values.go','go.mod','command.go','protocol.go','customtypes'})
        self.assertEqual((main/'lowlevel.go').read_bytes(),(self.source/producer.EXAMPLES/'lowlevel.go').read_bytes())
        self.assertTrue((main/'customtypes/bounded_records.go').is_file())
    def test_assemblyscript_keeps_original_relative_sdk_and_full_module(self):
        producer,stage=self.stage('assemblyscript')
        self.assertTrue((stage/'sdk/origin/assemblyscript/ros1/roswasm.ts').is_file())
        self.assertTrue((stage/'sdk/origin/custom-types/legacy-family/bounded_records.ts').is_file())
        self.assertEqual((stage/producer.EXAMPLES/'lowlevel.ts').read_bytes(),(self.source/producer.EXAMPLES/'lowlevel.ts').read_bytes())
    def rust(self,*,wrong_target=False,missing=False,escape=False):
        source=self.original('actual.rs',b'// actual recorded policy input\n');artifact=self.output/'guest.wasm';artifact.write_bytes(b'policy output')
        other=self.root/'external' if escape else self.source/'system.rs';other.write_bytes(b'policy dependency')
        selected=self.root/'missing.rs' if missing else other
        dep=self.output/'command.d';target=self.output/'wrong.wasm' if wrong_target else artifact
        dep.write_text(str(dep)+': '+str(source)+' '+str(selected)+'\n\n'+str(target)+': '+str(source)+' '+str(selected)+'\n\n'+str(source)+':\n')
        return CONTEXT['support'].rust_dependencies(self.frame,dep,artifact,source,self.output)
    def test_rust_actual_multiple_rules_retain_every_existing_input(self):
        row=self.rust();self.assertEqual(len(row['dependencies']),2);self.assertIn('ACTUAL_RUST_DEP_INFO',row['scope'])
    def test_rust_artifact_rule_required(self):
        with self.assertRaisesRegex(ValueError,'artifact/source rule missing'):self.rust(wrong_target=True)
    def test_rust_missing_dependency_never_skipped(self):
        with self.assertRaises(FileNotFoundError):self.rust(missing=True)
    def test_rust_external_dependency_refused(self):
        with self.assertRaisesRegex(ValueError,'complete frozen/tool/SDK'):self.rust(escape=True)
    def test_rust_retired_temporary_names_and_locations_remain_finite(self):
        # Exercise the production predicate against real created-then-retired
        # policy fixtures. A spelling alone never supplies pre-exec link proof.
        cell=self.output/'cell';cell.mkdir();temporary=self.output/'tmp';temporary.mkdir()
        outside=self.root/'other';outside.mkdir()
        rows=((cell/'unit.hash-cgu.0.rcgu.o',True),(cell/'ordinary.o',False),
              (outside/'unit.hash-cgu.0.rcgu.o',False),(cell/r'unit\.rcgu\.o',False),
              (temporary/'compiler-owned.o',True),(temporary/'compiler-owned.rlib',False))
        for path,expected in rows:
            path.write_bytes(b'created policy object; never compiled\n');path.unlink()
            self.assertFalse(path.exists())
            self.assertIs(CONTEXT['support'].rust_compiler_temporary(path,self.output,cell),expected)
        self.assertFalse(CONTEXT['support'].rust_compiler_temporary(Path('unit.rcgu.o'),self.output,cell))


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for key in ('source','manifest','config','out'):parser.add_argument('--'+key,type=Path,required=True)
    for key in ('manifest-sha256','config-sha256'):parser.add_argument('--'+key,required=True)
    args=parser.parse_args();need(sys.platform=='linux' and os.geteuid()==0 and args.source.resolve(strict=True)==ROOT,'actual frozen Root Linux policy lane required')
    raw=read(args.manifest.resolve(strict=True),16*MIB);need(hashlib.sha256(raw).hexdigest()==args.manifest_sha256,'actual MF SHA changed')
    def pairs(items):
        result={}
        for k,v in items:need(k not in result,'duplicate metadata key');result[k]=v
        return result
    frozen=json.loads(raw,object_pairs_hook=pairs)
    raw=read(args.config.resolve(strict=True),16*MIB);need(hashlib.sha256(raw).hexdigest()==args.config_sha256,'actual config SHA changed')
    config=json.loads(raw,object_pairs_hook=pairs);cg=config['cgroup'];leaf=Path(cg['expected_leaf']);info=leaf.stat()
    need(cg['memory_max']==64*GIB and cg['memory_swap_max']==0 and cg['expected_cpus']=='0-31' and
         cg['expected_leaf']=='/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope' and
         (info.st_dev,info.st_ino)==(cg['expected_device'],cg['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip()==cg['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines()==['0::'+str(leaf).removeprefix('/sys/fs/cgroup')] and
         (leaf/'memory.max').read_text().strip()==str(64*GIB) and (leaf/'memory.swap.max').read_text().strip()=='0' and
         (leaf/'cpuset.cpus.effective').read_text().strip()=='0-31','actual original cgroup required before candidate import')
    selected=[]
    def load(relative,label):
        path=ROOT/relative;data=read(path,MIB);pin={'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()}
        need(frozen.get(relative)==pin,'test/helper absent from complete current manifest');selected.append((path,data))
        module=types.ModuleType(label);module.__file__=str(path);sys.modules[label]=module;exec(compile(data,str(path),'exec'),module.__dict__);return module
    own='test/0073.ros-tests/support/source/ros_unitree/test_legacy_guest_build_policy.py';data=read(ROOT/own,MIB);own_initial=data
    need(frozen.get(own)=={'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()},'own test source absent from full manifest')
    common=load('tools/ros_distro/common.py','legacy_policy_common');common._CGROUP_PROBE=load('test/0073.ros-tests/support/source/ros_cgroup/probe.py','legacy_policy_probe')
    before=common.cgroup_state(cg);source_before=common.source_state(ROOT,args.manifest)
    out=args.out;need(out.is_absolute() and out.parent.resolve(strict=True)==out.parent and not out.exists() and not out.is_symlink() and
         not out.is_relative_to(ROOT) and not ROOT.is_relative_to(out),'fresh independent policy output required');out.mkdir(mode=0o700)
    start=time.monotonic()
    def guard():
        common.cgroup_preserved(before,common.cgroup_state(cg));need(time.monotonic()-start<600,'policy deadline exhausted')
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024<=512*MIB,'policy coordinator RSS exceeds512MiB')
        for path in (out,*out.parents):
            space=os.statvfs(path);need(space.f_bavail*space.f_frsize>=8*GIB,'eight-GiB backing reserve required')
    guard();producer=load('test/0073.ros-tests/support/source/ros_unitree/prepare_legacy_guests.py','legacy_policy_producer')
    support=load('test/0073.ros-tests/support/source/ros_unitree/command_build_support.py','legacy_policy_support')
    inspector=load('sdk/origin/tools/check_wasm_imports.py','legacy_policy_wasm_reader')
    abi,abi_pin=common.document(ROOT/'sdk/origin/abi/abi-v1.json');need(frozen['sdk/origin/abi/abi-v1.json']=={k:abi_pin[k] for k in ('bytes','sha256')},'actual ABI changed')
    CONTEXT.update(out=out,guard=guard,common=common,producer=producer,support=support,inspector=inspector,abi=abi)
    result=unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(Policy))
    guard();need(common.source_state(ROOT,args.manifest)==source_before,'complete frozen source changed')
    for path,data in selected:need(read(path,MIB)==data,'selected helper bytes changed')
    need(read(ROOT/own,MIB)==own_initial,'test source changed')
    report={'status':'PASS_LEGACY_GUEST_POLICY_ONLY' if result.wasSuccessful() else 'FAIL','tests':result.testsRun,
            'failures':len(result.failures),'errors':len(result.errors),'cgroup_before':before,'cgroup_after':common.cgroup_state(cg),
            'source_preserved':True,'candidate_compiler_executed':False,'Guest_or_ROS_executed':False,
            'scope':'Actual filesystem copying/dependency and synthetic core-byte policy tests only; no actual six-language build or Wasm validation result'}
    common.atomic_json(out/'report.json',report);return 0 if result.wasSuccessful() else 1


if __name__=='__main__':raise SystemExit(main())
