#!/usr/bin/env python3
"""Exercise current 64 GiB probe/SDK admission with wholly synthetic files.

Only named Python functions are extracted from current source. No live /proc or
cgroup files, compiler, validator, native test, Wasm, ROS or QEMU is accessed or
executed. Existing 61-row compiler and 17-row model recipe bodies are unchanged.
The legacy filename is retained for existing script paths. Its current
expected budget is 64 GiB; all historical receipts remain immutable.
"""
import sys
sys.dont_write_bytecode = True
import argparse, ast, copy, hashlib, importlib.util, json, os
from pathlib import Path
import stat, types

ROOT = Path(__file__).resolve().parents[2]
PROBE = ROOT/'tests/ros_cgroup/probe.py'
BUILDER = ROOT/'sdk/tools/build_cgroup_matrix.py'
MODELS = ROOT/'sdk/tools/run_cgroup_models.py'


def selected(path, names, scope):
    """Load definitions only; their module CLI/import/launch code is excluded."""
    nodes = [node for node in ast.parse(path.read_text()).body
             if isinstance(node, ast.FunctionDef) and node.name in names]
    assert {node.name for node in nodes} == set(names)
    exec(compile(ast.Module(body=nodes, type_ignores=[]), str(path)+'<synthetic48>', 'exec'), scope)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    if args.report and args.report.exists(): parser.error('fresh report path required')
    spec = importlib.util.spec_from_file_location('root_counter_source_fixture', ROOT/'tests/ros_cgroup/root_counter_models.py')
    fixtures = importlib.util.module_from_spec(spec);spec.loader.exec_module(fixtures)
    rows = []

    def exercise(name, change=lambda fs: None, expected=True, pins=None, hook=None, sdk=False):
        fs = fixtures.Filesystem();change(fs);FakePath = fs.path_type()
        FakePath.resolve = lambda self: self
        if hook:
            original = FakePath.read_text
            counts = {}
            def read(self):
                value = original(self);counts[self.value] = counts.get(self.value, 0)+1
                hook(fs, self.value, counts[self.value]);return value
            FakePath.read_text = read
        mocked_os = types.SimpleNamespace(getpid=lambda:123,cpu_count=lambda:32,major=os.major,minor=os.minor)
        scope = {'Path':FakePath, 'errno':fixtures.errno, 'stat':stat, 'os':mocked_os,
                 'platform':types.SimpleNamespace(system=lambda:'Linux',uname=lambda:types.SimpleNamespace(_asdict=lambda:{'synthetic':True}))}
        selected(PROBE, ('controllerless_root','probe'), scope)
        if not sdk:
            result = scope['probe'](**(pins or {}));accepted = result['status']=='PASS'
        else:
            arguments = types.SimpleNamespace(expected_leaf=FakePath(fs.leaf),expected_device=os.makedev(0,29),expected_inode=18927,
                expected_boot='synthetic-current-64-boot',expected_memory_max=68719476736,expected_swap_max='0',expected_cpus='0-31')
            for key,value in (pins or {}).items():setattr(arguments,key,value)
            sdk_scope = {'Path':FakePath,'PROBE':types.SimpleNamespace(probe=scope['probe']), 'LIMIT':68719476736,'os':mocked_os}
            selected(BUILDER, ('events','cgroup_snapshot'), sdk_scope)
            try:result = sdk_scope['cgroup_snapshot'](arguments);accepted = True
            except RuntimeError as error:result={'reason':str(error)};accepted=False
        assert accepted is expected, (name, result)
        rows.append({'name':name,'status':'PASS','admitted':accepted})

    exercise('exact actual64GiB leaf with zero swap')
    full = {'expected_leaf':'/sys/fs/cgroup/system.slice/test.scope','expected_device':os.makedev(0,29),
            'expected_inode':18927,'expected_boot':'synthetic-current-64-boot'}
    exercise('complete explicit current leaf/device/inode/boot binding',pins=full)
    for value in ('51539607552','34359738368','max'):
        exercise('wrong actual leaf limit '+value,lambda fs,v=value:fs.files.update({fs.leaf+'/memory.max':v}),False)
    exercise('old48GiB leaf hidden by inherited64GiB limit',lambda fs:fs.files.update({fs.leaf+'/memory.max':'51539607552',fs.parent+'/memory.max':'68719476736'}),False)
    exercise('inherited64GiB limit cannot replace exact native leaf',lambda fs:fs.files.update({fs.leaf+'/memory.max':'max',fs.parent+'/memory.max':'68719476736'}),False)
    exercise('ancestor32GiB limit is rejected',lambda fs:fs.files.update({fs.parent+'/memory.max':'34359738368'}),False)
    exercise('old48GiB expected limit is rejected',expected=False,pins={'expected_memory_max':51539607552})
    exercise('arbitrary expected32GiB limit is rejected',expected=False,pins={'expected_memory_max':34359738368})
    exercise('partial expected identity is rejected',expected=False,pins={'expected_leaf':full['expected_leaf']})
    for key,value in [('expected_device',os.makedev(0,30)),('expected_inode',18926),('expected_boot','old-boot'),('expected_leaf','/sys/fs/cgroup/other.scope')]:
        exercise('stale explicit '+key,expected=False,pins={**full,key:value})
    for value in ('max','1'):
        exercise('nonzero or unlimited swap '+value,lambda fs,v=value:fs.files.update({fs.leaf+'/memory.swap.max':v}),False)
    exercise('missing swap proof',lambda fs:fs.files.pop(fs.leaf+'/memory.swap.max'),False)
    exercise('missing boot proof',lambda fs:fs.files.pop('/proc/sys/kernel/random/boot_id'),False)
    exercise('empty boot proof',lambda fs:fs.files.update({'/proc/sys/kernel/random/boot_id':''}),False)
    for label,target,value in [('boot','/proc/sys/kernel/random/boot_id','changed-boot'),('membership','/proc/self/cgroup','0::/system.slice/other.scope\n'),('PID','/sys/fs/cgroup/system.slice/test.scope/cgroup.procs','124\n'),('limit','/sys/fs/cgroup/system.slice/test.scope/memory.max','51539607552')]:
        def mutate(fs,path,count,t=target,v=value):
            if path==fs.leaf+'/memory.events' and count==1:fs.files[t]=v
        exercise('mid-probe '+label+' replacement rejected',expected=False,hook=mutate)
    exercise('SDK snapshot requires complete current64GiB proof',sdk=True)
    for key,value in [('expected_boot','old-boot'),('expected_memory_max',51539607552),('expected_swap_max','max'),('expected_inode',18926),('expected_cpus','0-15')]:
        exercise('SDK refuses stale '+key,expected=False,pins={key:value},sdk=True)
    current = {'leaf':'/sys/fs/cgroup/system.slice/test.scope','device':29,'inode':18927,'boot_id':'new-boot','swap_max':'0','cpus_effective':'0-31',
               'expected_memory_max':68719476736,'leaf_memory_max':68719476736,'probe':{'status':'PASS','effective_memory_max':68719476736,'expected_memory_max':68719476736}}
    module_scope = {'GUARDS':types.SimpleNamespace(LIMIT=68719476736)}
    selected(MODELS, ('require','same_scope'), module_scope);module_scope['same_scope'](current,current)
    rows.append({'name':'model admission exact64GiB scope','status':'PASS','admitted':True})
    for key,value in [('expected_memory_max',51539607552),('leaf_memory_max',51539607552),('boot_id','old-boot'),('swap_max','max')]:
        old = copy.deepcopy(current);old[key]=value
        try:module_scope['same_scope'](old,current)
        except ValueError:pass
        else:raise AssertionError('old model scope incorrectly admitted: '+key)
        rows.append({'name':'model scope refuses '+key,'status':'PASS','admitted':False})
    report = {'status':'PURE_CURRENT64_CGROUP_SDK_MODEL_PASS','checks':len(rows),'rows':rows,
              'expected_memory_max':68719476736,'expected_swap_max':'0','default_current_limit_mib':61440,'default_rss_limit_mib':2048,
              'source_inputs':{str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (PROBE,BUILDER,MODELS)},
              'scope':'Synthetic files and named extracted functions only; no live cgroup/Node/Wasm/compiler/native/ROS/QEMU execution'}
    if args.report:
        args.report.parent.mkdir(parents=True,exist_ok=True)
        with args.report.open('x') as stream:json.dump(report,stream,indent=2);stream.write('\n')
    print(json.dumps({k:v for k,v in report.items() if k!='rows'}));return 0

if __name__=='__main__':raise SystemExit(main())
