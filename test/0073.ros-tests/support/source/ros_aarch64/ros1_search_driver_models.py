#!/usr/bin/env python3
"""Pure Python admission models for the ROS1 ARM feature driver.

Only selected definitions are extracted from the current driver. Compiler argv,
procfs, ELF facts, cgroup facts and prepared receipts are explicitly synthetic.
Small temporary text files exercise opened-descriptor identity checks. No
compiler, native executable, ROS, Wasm, QEMU, mount or live cgroup is invoked.
These checks cannot establish native compilation or middleware behavior.
"""
import argparse
import ast
import copy
import hashlib
import json
import os
from pathlib import Path
import posixpath
import shlex
import stat
import sys
import tempfile
import types

sys.dont_write_bytecode = True
from gateway_qualification import read_file_binding
SOURCE = Path(__file__).with_name('run_ros1_search.py')
DEFINITIONS = {'pin_file','pins_preserved','OwnedProcesses','fixture_recipe',
               'dependency_rule','validate_fixture_link','validate_prepared_contract',
               'validate_compilation_architecture','cmake'}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report',type=Path)
    args=parser.parse_args()
    if args.report and args.report.exists():parser.error('report already exists')
    raw=SOURCE.read_bytes();tree=ast.parse(raw);nodes=[];constants={}
    for node in tree.body:
        if isinstance(node,(ast.FunctionDef,ast.ClassDef)) and node.name in DEFINITIONS:nodes.append(node)
        elif isinstance(node,ast.Assign) and any(isinstance(target,ast.Name) and target.id in
                ('PROBES','PROBE_CONTRACTS','DRIVER_INPUTS') for target in node.targets):
            nodes.append(node)
    scope={'__file__':str(SOURCE),'Path':Path,'os':os,'stat_module':stat,'read_file_binding':read_file_binding,
           'shlex':shlex,'posixpath':posixpath,'signal':types.SimpleNamespace(),
           'time':types.SimpleNamespace(),'digest':lambda path:'synthetic-helper-'+Path(path).name}
    exec(compile(ast.Module(body=nodes,type_ignores=[]),str(SOURCE)+'<pure-admission-model>','exec'),scope)
    assert DEFINITIONS<=set(scope)
    rows=[]

    def check(name,condition):
        assert condition,name
        rows.append({'name':name,'status':'PASS'})

    def refuse(name,operation):
        try:operation()
        except (RuntimeError,ValueError,IndexError,KeyError,OSError):
            rows.append({'name':name,'status':'PASS'});return
        raise AssertionError(name+' was accepted')

    # The model builds independent expected command tokens. The real function
    # then parses the synthetic compile database and verbose transcript.
    compiler='/usr/bin/aarch64-linux-gnu-g++-9'
    probes=scope['PROBES'];first=next(iter(probes));relative=probes[first]
    expected_object='/tmp/build/CMakeFiles/'+first+'.dir/work/'+relative+'.o'
    # Generate source only. Catkin testing must be disabled before discovery;
    # the strict database admission still refuses framework or extra sources.
    generated=scope['cmake']().splitlines()
    testing='set(CATKIN_ENABLE_TESTING OFF CACHE BOOL "Owned ROS1 regression fixtures only" FORCE)'
    catkin='find_package(catkin REQUIRED COMPONENTS roscpp actionlib actionlib_msgs actionlib_tutorials)'
    check('owned fixtures disable catkin testing before discovery',generated.index(testing)<generated.index(catkin))
    check('generated executable sources are exactly the two owned probes',
        {line for line in generated if line.startswith('add_executable(')}==
        {'add_executable('+label+' /work/'+value+')' for label,value in probes.items()})
    def tokens(label,relative):
        return [compiler,'-DNDEBUG','-O1','-g0','-UNDEBUG','-std=gnu++17','-MD',
                '-MF','/tmp/build/CMakeFiles/'+label+'.dir/fixture-explicit.o.d',
                '-include','/tmp/evidence-stop.hpp','-o','CMakeFiles/'+label+'.dir/work/'+relative+'.o',
                '-c','/work/'+relative]
    database=[{'directory':'/tmp/build','file':'/work/'+value,'arguments':tokens(label,value)}
              for label,value in probes.items()]
    # Noetic CMake 3.16 contributes no later -MF. Independently reconstruct the
    # full compile argv from each emitted target's flags and retain its actual
    # object rule, C++17, enabled assertions, barrier and system-inclusive -MD.
    for label,value in probes.items():
        option=next(line for line in generated if line.startswith('target_compile_options('+label+' PRIVATE '))
        flags=shlex.split(option[len('target_compile_options('+label+' PRIVATE '):-1])
        emitted=[compiler,'-DNDEBUG',*flags,'-o','CMakeFiles/'+label+'.dir/work/'+value+'.o','-c','/work/'+value]
        own=scope['fixture_recipe'](label,value,database,'cd /tmp/build && '+shlex.join(emitted)+'\n')
        check('Noetic explicit MF admitted without CMake-added output '+label,
            own['effective_dependency_output']=='/tmp/build/CMakeFiles/'+label+'.dir/fixture-explicit.o.d'
            and own['controls']['mf_outputs']==['/tmp/build/CMakeFiles/'+label+'.dir/fixture-explicit.o.d'])
    base=tokens(first,relative);actual=base[:-2]+['-MT',expected_object,'-MF','discarded.d',
        '-MF','CMakeFiles/'+first+'.dir/work/'+relative+'.o.d']+base[-2:]
    transcript='cd /tmp/build && '+shlex.join(actual)+'\n'
    recipe=scope['fixture_recipe'](first,relative,database,transcript)
    check('final MF overrides prior output',recipe['effective_dependency_output']==expected_object+'.d')
    check('exact compiled object retained',recipe['controls']['object_guest_path']==expected_object)
    check('both MF spellings retained in order',recipe['controls']['mf_outputs']==[
        '/tmp/build/CMakeFiles/'+first+'.dir/fixture-explicit.o.d',
        'discarded.d','CMakeFiles/'+first+'.dir/work/'+relative+'.o.d'])
    scope['dependency_rule'](expected_object+': /work/'+relative+' \\\n /tmp/evidence-stop.hpp\n',recipe)
    check('exact depfile object accepted',True)
    for target in ('/tmp/build/other.o',expected_object+' other.o','../outside.o',''):
        refuse('depfile target refused '+repr(target),lambda target=target:scope['dependency_rule'](target+': /work/'+relative,recipe))
    link=[compiler,'-O1',expected_object,'-o',first,'/opt/ros/noetic/lib/libroscpp.so','-lpthread']
    scope['validate_fixture_link'](first,recipe,shlex.join(link),shlex.join(link)+'\n')
    check('actual exact link accepted',True)
    for name,value,verbose in (
            ('unobserved link',link,''),('duplicate actual link',link,(shlex.join(link)+'\n')*2),
            ('wrong compiler',['/usr/bin/g++',*link[1:]],shlex.join(link)),
            ('wrong object',[compiler,'wrong.o',*link[3:]],shlex.join(link)),
            ('wrong output',[compiler,*link[1:4],'other',*link[5:]],shlex.join(link))):
        refuse(name,lambda value=value,verbose=verbose:scope['validate_fixture_link'](first,recipe,shlex.join(value),verbose))
    for name,change in (
            ('no MD',lambda value:[item for item in value if item!='-MD']),
            ('MMD only',lambda value:['-MMD' if item=='-MD' else item for item in value]),
            ('C++20',lambda value:['-std=gnu++20' if item=='-std=gnu++17' else item for item in value]),
            ('disabled asserts',lambda value:['-DNDEBUG=0' if item=='-UNDEBUG' else item for item in value]),
            ('wrong barrier',lambda value:['/tmp/other.hpp' if item=='/tmp/evidence-stop.hpp' else item for item in value]),
            ('escaped object',lambda value:['/tmp/outside.o' if item==base[base.index('-o')+1] else item for item in value]),
            ('opaque response file',lambda value:[*value[:-2],'@opaque.args',*value[-2:]]),
            ('opaque preprocessor',lambda value:[*value[:-2],'-Wp,-MD,other.d',*value[-2:]]),
            ('syntax only',lambda value:[*value[:-2],'-fsyntax-only',*value[-2:]]),
            ('final MF escapes',lambda value:[*value[:-2],'-MF','/tmp/outside.d',*value[-2:]])):
        modified=change(actual)
        refuse(name,lambda modified=modified:scope['fixture_recipe'](first,relative,database,shlex.join(modified)))
    refuse('duplicate successful compiler recipe',lambda:scope['fixture_recipe'](first,relative,database,transcript*2))
    for name,value in (('extra source',[*database,database[0]]),('missing source',database[:-1])):
        refuse(name,lambda value=value:scope['fixture_recipe'](first,relative,value,transcript))
    framework=[{'directory':'/tmp/build','file':'/usr/src/googletest/'+name,'arguments':[]}
               for name in ('googletest/src/gtest-all.cc','googletest/src/gtest_main.cc',
                            'googlemock/src/gmock-all.cc','googlemock/src/gmock_main.cc')]
    refuse('unused catkin framework database entries remain refused',
        lambda:scope['fixture_recipe'](first,relative,[*database,*framework],transcript))
    for macro in ('NDEBUG','NDEBUG=0','NDEBUG=1'):
        modified=actual[:-2]+['-D',macro]+actual[-2:]
        matching=copy.deepcopy(database);matching[0]['arguments']=base[:-2]+['-D',macro]+base[-2:]
        refuse('matching database separated assertion define '+macro,
            lambda modified=modified,matching=matching:scope['fixture_recipe'](first,relative,matching,shlex.join(modified)))
    modified=actual[:-2]+['-D','NDEBUG','-U','NDEBUG']+actual[-2:]
    matching=copy.deepcopy(database);matching[0]['arguments']=base[:-2]+['-D','NDEBUG','-U','NDEBUG']+base[-2:]
    scope['fixture_recipe'](first,relative,matching,shlex.join(modified))
    check('last separated undefine enables assertions',True)

    receipt={'status':'PREPARED','mode':'prepare','kind':'ros1-search-arm','format_version':1,
        'compilation_status':'PASS','actual_native_behavior_probes':0,'actual_behavior_status':'NOT_PERFORMED',
        'probes':{name:{} for name in probes},'probe_contracts':copy.deepcopy(scope['PROBE_CONTRACTS']),
        'source_manifest_sha256':'synthetic-full-source-manifest','same_cgroup_incarnation':True,
        'cgroup_incarnation_before':{'memory.swap.max':'0','device':29,'inode':91},
        'cgroup_incarnation_after':{'memory.swap.max':'0','device':29,'inode':91},
        'process_cleanup':{'status':'PASS','remaining_owned_processes':[]},
        'remaining_owned_processes':[],'remaining_owned_mounts':[],
        'mount_cleanup':[{'returncode':0}],'memory_swap_max_before':'0','memory_swap_max_after':'0',
        'commands':[{'name':name,'status':'PASS','returncode':0,'peak_observed_subtree_RSS_bytes':1024}
            for name in ('configure','compile')],
        'driver_inputs':{name:{'sha256':'synthetic-helper-'+name} for name in scope['DRIVER_INPUTS']}}
    flags=('source_preserved','sdk_preserved','driver_inputs_preserved','prepared_inputs_preserved',
           'tools_preserved','gateway_inputs_preserved')
    receipt.update({name:True for name in flags})
    scope['validate_prepared_contract'](receipt,'synthetic-full-source-manifest')
    check('complete zero-behavior preparation admitted',True)
    for name,value in [('status','PASS'),('mode','all'),('kind','other'),('format_version',0),
            ('compilation_status','REUSED_QUALIFIED_PREPARATION'),('actual_native_behavior_probes',2),
            ('actual_behavior_status','PASS'),('same_cgroup_incarnation',False),
            ('memory_swap_max_before','max'),('memory_swap_max_after','1'),
            ('remaining_owned_processes',[123]),('remaining_owned_mounts',['mount']),
            ('mount_cleanup',[]),('source_manifest_sha256','old-source')]+[(name,False) for name in flags]:
        bad=copy.deepcopy(receipt);bad[name]=value
        refuse('prepared field '+name,lambda bad=bad:scope['validate_prepared_contract'](bad,'synthetic-full-source-manifest'))
    changes={
        'missing product':lambda value:value['probes'].pop(first),
        'extra product':lambda value:value['probes'].update({'extra':{}}),
        'weakened fixture contract':lambda value:value['probe_contracts'][first].update({'requires_gateway':False}),
        'changed incarnation':lambda value:value['cgroup_incarnation_after'].update({'inode':92}),
        'nonzero prepared swap':lambda value:[value[key].update({'memory.swap.max':'1'}) for key in ('cgroup_incarnation_before','cgroup_incarnation_after')],
        'failed cleanup':lambda value:value['process_cleanup'].update({'status':'FAIL'}),
        'remaining cleanup birth':lambda value:value['process_cleanup'].update({'remaining_owned_processes':[123]}),
        'failed unmount':lambda value:value['mount_cleanup'][0].update({'returncode':1}),
        'missing compile':lambda value:value['commands'].pop(),
        'failed compile':lambda value:value['commands'][1].update({'returncode':1}),
        'RSS excess':lambda value:value['commands'][1].update({'peak_observed_subtree_RSS_bytes':2*1024**3+1}),
        'missing RSS':lambda value:value['commands'][1].pop('peak_observed_subtree_RSS_bytes'),
        'extra helper':lambda value:value['driver_inputs'].update({'extra.py':{'sha256':'synthetic-helper-extra.py'}}),
        'changed helper':lambda value:value['driver_inputs'][scope['DRIVER_INPUTS'][0]].update({'sha256':'old-helper'}),
    }
    for name,change in changes.items():
        bad=copy.deepcopy(receipt);change(bad)
        refuse(name,lambda bad=bad:scope['validate_prepared_contract'](bad,'synthetic-full-source-manifest'))
    for name,field in (('boolean format version','format_version'),('boolean behavior count','actual_native_behavior_probes')):
        bad=copy.deepcopy(receipt);bad[field]=field=='format_version'
        refuse(name,lambda bad=bad:scope['validate_prepared_contract'](bad,'synthetic-full-source-manifest'))
    bad=copy.deepcopy(receipt);bad['commands'][1]['peak_observed_subtree_RSS_bytes']=True
    refuse('boolean RSS refused',lambda:scope['validate_prepared_contract'](bad,'synthetic-full-source-manifest'))

    # These are ordinary text files. Architecture values are explicitly fake;
    # the real validator's selection rules operate on independent metadata.
    with tempfile.TemporaryDirectory(prefix='ros1-arm-pure-tool-model-') as temporary:
        sdk=Path(temporary)
        def rooted(root,logical):return Path(root)/logical.lstrip('/')
        paths={'native-gcc':'/usr/bin/aarch64-linux-gnu-g++-9',
               'native-frontend':'/usr/lib/gcc/aarch64-linux-gnu/9/cc1plus','native-cmake':'/usr/bin/cmake'}
        def fake_arm(path):
            path=Path(path);state=path.stat()
            return {'path':str(path),'device':state.st_dev,'inode':state.st_ino,'bytes':state.st_size,
                'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'elf_class':2,'elf_data':1,'e_machine':183}
        for logical in paths.values():
            path=rooted(sdk,logical);path.parent.mkdir(parents=True,exist_ok=True);path.write_text('Never execute synthetic '+logical+'\n')
        facts={name:fake_arm(rooted(sdk,path)) for name,path in paths.items()}
        product_facts={label:{'path':'/synthetic/'+label,'sha256':'fake-'+label,'e_machine':183} for label in probes}
        incarnation={'path':'/synthetic/leaf','device':9,'inode':91,'boot_id':'synthetic-boot'}
        arch={'arm_targets':{**facts,**product_facts},'cgroup_before':{'membership':['synthetic::/leaf']},
            'cgroup_incarnation_before':copy.deepcopy(incarnation),
            'expected_host_emulator_sha256':['synthetic-qemu'],
            'actual_process_samples':[{'name':'compile/','pid':221,'starttime_ticks':11,'ancestry':[220,221],'live_cgroup_procs':True,
                'membership':['synthetic::/leaf'],'kernel_exe_elf':{'e_machine':62,'sha256':'synthetic-qemu'},
                'mapped_arm_targets':list(paths)},
                {'name':'compile/','pid':220,'starttime_ticks':10,'ancestry':[220],'live_cgroup_procs':True,
                 'membership':['synthetic::/leaf'],'kernel_exe_elf':{'e_machine':62,'sha256':'synthetic-chroot'},'mapped_arm_targets':[]}]}
        original={'compiler':facts['native-gcc'],'frontend':facts['native-frontend'],
            'cgroup_incarnation_before':copy.deepcopy(incarnation),
            'probes':{name:{'binary':value} for name,value in product_facts.items()},
            'commands':[{'name':'compile','pid':220,'launcher_starttime_ticks':10}]}
        scope.update(rooted=rooted,require_arm=fake_arm)
        pins=[];scope['validate_compilation_architecture'](original,arch,sdk,pins)
        check('actual compile subtree requires all three tool labels',len(pins)==3)
        mutations={
            'wrong build subtree':lambda value:value['actual_process_samples'][0].update({'ancestry':[999,221]}),
            'wrong launcher birth':lambda value:value['actual_process_samples'][1].update({'starttime_ticks':9}),
            'raw architecture references another cgroup':lambda value:value['cgroup_incarnation_before'].update({'inode':92}),
            'missing root birth proof':lambda value:value['actual_process_samples'].pop(),
            'mapped task predates launcher':lambda value:value['actual_process_samples'][0].update({'starttime_ticks':9}),
            'unavailable live membership':lambda value:value['actual_process_samples'][0].update({'live_cgroup_procs':False}),
            'different membership':lambda value:value['actual_process_samples'][0].update({'membership':['synthetic::/other']}),
            'unrecognized host image':lambda value:value['actual_process_samples'][0]['kernel_exe_elf'].update({'sha256':'other-qemu'}),
            'mislabelled host architecture':lambda value:value['actual_process_samples'][0]['kernel_exe_elf'].update({'e_machine':183}),
            'compiler mapping missing':lambda value:value['actual_process_samples'][0].update({'mapped_arm_targets':['native-cmake','native-frontend']}),
            'frontend mapping missing':lambda value:value['actual_process_samples'][0].update({'mapped_arm_targets':['native-cmake','native-gcc']}),
            'CMake mapping missing':lambda value:value['actual_process_samples'][0].update({'mapped_arm_targets':['native-frontend','native-gcc']}),
            'tool inode changed':lambda value:value['arm_targets']['native-gcc'].update({'inode':0}),
            'product identity changed':lambda value:value['arm_targets'][first].update({'sha256':'other-product'}),
        }
        for name,mutation in mutations.items():
            changed=copy.deepcopy(arch);mutation(changed)
            refuse(name,lambda changed=changed:scope['validate_compilation_architecture'](original,changed,sdk,[]))
        for name in ('compiler','frontend'):
            changed=copy.deepcopy(original);changed[name]['sha256']='other-tool'
            refuse('reported '+name+' differs',lambda changed=changed:scope['validate_compilation_architecture'](changed,arch,sdk,[]))

    # procfs belongs to synthetic worker threads. No real /proc path is read.
    identities={100:{'pid':100,'parent_pid':1,'starttime_ticks':10,'state':'S'}}
    known={'identity':identities[100],'label':'synthetic-command','ancestry':[100]}
    children={100:['200'],101:['201','200']}
    class ProcPath:
        def __init__(self,value):self.value=str(value)
        @property
        def name(self):return self.value.rsplit('/',1)[-1]
        def __truediv__(self,value):return ProcPath(self.value+'/'+value)
        def iterdir(self):
            assert self.value=='/proc/100/task'
            return [ProcPath('/proc/100/task/'+str(number)) for number in children]
        def read_text(self):
            task=int(self.value.split('/')[-2]);return ' '.join(children[task])
    owner=scope['OwnedProcesses'].__new__(scope['OwnedProcesses']);owner.rows={(100,10):known}
    owner.alive=lambda row:True;owner.check=lambda row:True;acquired=[]
    owner.acquire=lambda pid,label,ancestry,parent:acquired.append((pid,label,ancestry,parent))
    saved_path=scope['Path'];scope['Path']=ProcPath
    try:owner.sample()
    finally:scope['Path']=saved_path
    check('all-thread child enumeration includes worker fork',set(row[0] for row in acquired)=={200,201})
    check('duplicate child acquired only once',len(acquired)==2)
    check('every child carries exact observed parent birth',all(row[3]==identities[100] for row in acquired))
    check('every child carries observed root ancestry',all(row[2]==[100,row[0]] for row in acquired))
    owner.check=lambda row:False;acquired.clear();scope['Path']=ProcPath
    try:owner.sample()
    finally:scope['Path']=saved_path
    check('unavailable live membership cannot discover/admit descendants',acquired==[])
    class RecursiveProcPath(ProcPath):
        def __truediv__(self,value):return RecursiveProcPath(self.value+'/'+value)
        def iterdir(self):
            pid=int(self.value.split('/')[2]);return [RecursiveProcPath(self.value+'/'+str(pid))]
        def read_text(self):
            pid=int(self.value.split('/')[2]);return {100:'200 201',200:'',201:'301',301:''}[pid]
    owner.rows={(100,10):known};owner.check=lambda row:True;acquired.clear()
    def acquire_recursive(pid,label,ancestry,parent):
        identity={'pid':pid,'parent_pid':parent['pid'],'starttime_ticks':pid,'state':'S'}
        row={'identity':identity,'label':label,'ancestry':ancestry}
        owner.rows[(pid,pid)]=row;acquired.append(row);return row
    owner.acquire=acquire_recursive;scope['Path']=RecursiveProcPath
    try:owner.sample()
    finally:scope['Path']=saved_path
    check('new descendants recursively discovered in the same pass',set(row['identity']['pid'] for row in acquired)=={200,201,301})
    check('grandchild carries the entire observed root chain',owner.rows[(301,301)]['ancestry']==[100,201,301])
    saved_os=scope['os'];closed=[]
    scope['os']=types.SimpleNamespace(pidfd_open=lambda pid:701,close=lambda fd:closed.append(fd))
    first_identity={'pid':201,'state':'R','parent_pid':100,'starttime_ticks':20}
    def adoption_case(second):
        values=iter([first_identity,second])
        def identity(pid):
            value=next(values)
            if isinstance(value,BaseException):raise value
            return value
        scope['process_identity']=identity
        record=scope['OwnedProcesses'].__new__(scope['OwnedProcesses']);record.rows={};record.check=lambda row:True
        return record,lambda:record.acquire(201,'modeled-command',[100,201],identities[100])
    try:
        record,operation=adoption_case({**first_identity,'state':'S'})
        operation();check('pidfd acquisition permits transient state changes',len(record.rows)==1 and closed==[])
        record,operation=adoption_case({**first_identity,'starttime_ticks':21})
        refuse('pidfd acquisition rejects changed birth',operation)
        check('changed-birth acquisition closes unregistered descriptor',closed==[701] and record.rows=={})
        closed.clear();record,operation=adoption_case(FileNotFoundError('synthetic vanished task'))
        refuse('vanished second identity refuses adoption',operation)
        check('vanished acquisition closes unregistered descriptor',closed==[701] and record.rows=={})
    finally:scope['os']=saved_os

    with tempfile.TemporaryDirectory(prefix='ros1-arm-pure-binding-model-') as temporary:
        root=Path(temporary);path=root/'input.txt';path.write_bytes(b'Synthetic metadata only. Never execute.\n')
        pin=scope['pin_file'](path)
        check('descriptor-bound SHA matches independent bytes',pin['sha256']==hashlib.sha256(path.read_bytes()).hexdigest())
        check('same input identity accepted',scope['pins_preserved']([pin]))
        replacement=root/'replacement.txt';replacement.write_bytes(path.read_bytes());replacement.replace(path)
        check('same-byte inode replacement refused',not scope['pins_preserved']([pin]))
        refuse('directory input refused before reading',lambda:scope['pin_file'](root))
        if hasattr(os,'mkfifo'):
            fifo=root/'input.fifo';os.mkfifo(fifo)
            refuse('FIFO input refused by nonblocking regular-file admission',lambda:scope['pin_file'](fifo))

    # Source guards protect the integration paths outside the extracted helpers.
    text=raw.decode()
    guards={
        'shared strict Gateway admission':'qualify_current_gateway(binary,report_path,source.parent/',
        'shared independent zero-OOM architecture admission':'architecture_binding(architecture_path,prepared_pins)',
        'qualified Gateway complete identity after copy':'require_arm(args.gateway)!=gateway_facts',
        'qualified Gateway private copy owns inode':'qualified Gateway copy must own a distinct file identity',
        'canonical Gateway stopped proof':'actual_gateway_ARM_mapping_before_resume',
        'pidfd-owned cleanup':'signal.pidfd_send_signal',
        'all-thread descendant scan':"Path(f'/proc/{pid}/task').iterdir()",
        'system-inclusive original compilation':'-std=gnu++17 -MD -MF /tmp/build/CMakeFiles/{label}.dir/fixture-explicit.o.d -include /tmp/evidence-stop.hpp',
        'serial compiler build':"'--parallel','1'",
        'exact prepared original binary identity':'facts!=item[\'binary\']',
        'full generated barrier preservation':'barrier.read_text()!=BARRIER',
        'same-read strict Gateway report reference':"report['gateway_report'] = gateway_binding['report']",
        'Gateway control binding final preservation':'pins_preserved(gateway_pins)',
        'explicit zero swap preflight':"before['memory.swap.max']!='0'",
        'bounded fixture command':"run('behavior-'+label,arguments,required,120",
        'Master XMLRPC PID bound to owned birth':"readiness Master PID is not an observed live owned birth",
        'Master XMLRPC PID actual ARM mapping':'actual_readiness_master_mapping',
        'both Master readiness methods':"client.getPid('/uwvm_arm_ros1_search_driver')",
        'separate preparation verdict':"status = 'PREPARED' if args.mode=='prepare' else 'PASS'",
    }
    for name,fragment in guards.items():check(name,fragment in text)
    check('no broad process-group cleanup','killpg(' not in text)
    check('exact two native probes',len(probes)==2 and len(scope['PROBE_CONTRACTS'])==2)
    result={'status':'SOURCE_AND_MODEL_PASS','scope':'Pure Python synthetic admission/argv/procfs and bounded temporary-file models only; no native compilation or ROS execution',
        'checks':len(rows),'rows':rows,'inputs':{str(path.relative_to(SOURCE.parents[2])):{
            'bytes':len(path.read_bytes()),'sha256':hashlib.sha256(path.read_bytes()).hexdigest()}
            for path in [SOURCE,Path(__file__),SOURCE.with_name('README.ros1-search.md'),
                *[SOURCE.with_name(name) for name in scope['DRIVER_INPUTS'] if name!=SOURCE.name],
                *[SOURCE.parents[2]/relative for relative in probes.values()]]},
        'native_compilation_status':'NOT_PERFORMED','actual_native_behavior_probes':0}
    if args.report:args.report.write_text(json.dumps(result,indent=2)+'\n')
    print('SOURCE_AND_MODEL_PASS',len(rows),'checks; native compilation and ROS behavior NOT_PERFORMED')


if __name__=='__main__':main()
