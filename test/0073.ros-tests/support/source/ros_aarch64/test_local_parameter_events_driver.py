#!/usr/bin/env python3
"""Pure metadata/source/ownership models; no compiler, process or ROS execution.

Prepared reports and verbose compiler commands below are handwritten negative
controls. Fake PID records and descriptors are never passed to a kernel API.
These checks establish driver rejection behavior only, not native acceptance.
"""
import ast
import argparse
import copy
import json
from pathlib import Path
import sys
import tempfile
import types
import unittest
from unittest.mock import patch

sys.dont_write_bytecode = True
import run_local_parameter_events as driver
from runtime import digest
from gateway_qualification import read_file_binding


def prepared():
    identity = {'path':'/modeled/leaf','device':9,'inode':11,'boot_id':'modeled-boot','membership':['0::/modeled/leaf'],
                'memory.swap.max':'0','parent_recorded_container_started_at':'modeled-container-start'}
    return {'status':'PREPARED','kind':'ros2-local-parameter-events-arm','format_version':1,'mode':'prepare','domain':203,
        'compilation_status':'PASS','native_behavior_status':'NOT_PERFORMED','actual_native_behavior_probes':0,
        'probes':{name:{} for name in driver.PROBES},'probe_contracts':copy.deepcopy(driver.PROBE_CONTRACTS),
        'source_manifest_sha256':'modeled-sha','same_cgroup_incarnation':True,'memory_swap_max_before':'0','memory_swap_max_after':'0',
        'cgroup_incarnation_before':identity,'cgroup_incarnation_after':copy.deepcopy(identity),
        'remaining_owned_processes':[],'remaining_owned_mounts':[],'source_preserved':True,'sdk_preserved':True,
        'driver_inputs_preserved':True,'tools_preserved':True,'prepared_inputs_preserved':True,
        'gateway_inputs_preserved':True,'immutable_inputs_preserved':True,
        'process_cleanup':[{'errors':[],'remaining':[]} for _ in range(2)],
        'mount_cleanup':[{'returncode':0}],
        'driver_inputs':{name:{'sha256':digest(Path(driver.__file__).with_name(name))} for name in driver.DRIVER_INPUTS},
        'commands':[{'name':name,'status':'PASS','returncode':0,'peak_observed_subtree_RSS_bytes':128} for name in ('configure','compile')]}


def compiler_data(label):
    source = '/work/'+driver.PROBES[label]
    binary = '/usr/bin/aarch64-linux-gnu-g++-13'
    common = f'{binary} -DNDEBUG -O1 -g0 -UNDEBUG -std=gnu++20 -MD -include /tmp/evidence-stop.hpp -o CMakeFiles/{label}.dir/probe.o'
    # CMake's database may omit rule-generated -MF; the successful verbose
    # recipe, rather than a guessed file, must bind the actual final output.
    database = []
    for name,relative in driver.PROBES.items():
        command = f'{binary} -DNDEBUG -O1 -g0 -UNDEBUG -std=gnu++20 -MD -include /tmp/evidence-stop.hpp -o CMakeFiles/{name}.dir/probe.o -c /work/{relative}'
        database.append({'file':'/work/'+relative,'directory':'/tmp/build','command':command})
    actual = common+f' -MF /tmp/ignored-origin.d -MD -MT CMakeFiles/{label}.dir/probe.o -MF CMakeFiles/{label}.dir/probe.o.d -c '+source
    return database,actual


class DriverModels(unittest.TestCase):
    def test_exact_current_preparation(self):
        driver.validate_prepared_contract(prepared(),'modeled-sha')

    def test_incomplete_prepared_controls(self):
        controls = [('status','PASS'),('mode','all'),('domain',74),('compilation_status','NOT_PERFORMED'),
            ('actual_native_behavior_probes',1),('native_behavior_status','PASS'),('same_cgroup_incarnation',False),
            ('memory_swap_max_before','max'),('memory_swap_max_after','1'),('remaining_owned_processes',[17]),
            ('remaining_owned_mounts',['/leftover']),('source_preserved',False),('sdk_preserved',False),
            ('driver_inputs_preserved',False),('tools_preserved',False),('probes',{}),('commands',[]),
            ('prepared_inputs_preserved',False),('gateway_inputs_preserved',False),('immutable_inputs_preserved',False),
            ('mount_cleanup',[]),('mount_cleanup',[{'returncode':1}]),('process_cleanup',[]),
            ('process_cleanup',[{'errors':['leak'],'remaining':[]},{'errors':[],'remaining':[]}]),
            ('format_version',True),('actual_native_behavior_probes',False),('domain',True)]
        for key,value in controls:
            with self.subTest(key=key):
                document = prepared();document[key]=value
                with self.assertRaises(RuntimeError):driver.validate_prepared_contract(document,'modeled-sha')

    def test_recipe_or_manifest_substitution(self):
        for mutation in ('source','inventory','contract','helper','incarnation','duplicate-command'):
            with self.subTest(mutation=mutation):
                document = prepared()
                if mutation=='source':document['source_manifest_sha256']='other'
                elif mutation=='inventory':document['probes']['unknown']={}
                elif mutation=='contract':document['probe_contracts']['local-parameter-events-ipc']['gateway_required']=False
                elif mutation=='helper':document['driver_inputs']['runtime.py']['sha256']='other'
                elif mutation=='incarnation':document['cgroup_incarnation_after']['inode']+=1
                else:document['commands'][1]['name']='configure'
                with self.assertRaises(RuntimeError):driver.validate_prepared_contract(document,'modeled-sha')

    def test_actual_last_mf_is_selected(self):
        for label in driver.PROBES:
            database,actual = compiler_data(label)
            result = driver.compile_recipe(label,database,actual)
            self.assertEqual(result['effective_mf'],f'/tmp/build/CMakeFiles/{label}.dir/probe.o.d')

    def test_serial_command_count_rss_and_types(self):
        for change in ('duplicate','rss-over','rss-negative','rss-bool','returncode-bool'):
            document = prepared()
            if change=='duplicate':document['commands'].append(copy.deepcopy(document['commands'][0]))
            elif change=='rss-over':document['commands'][0]['peak_observed_subtree_RSS_bytes']=2*1024**3+1
            elif change=='rss-negative':document['commands'][0]['peak_observed_subtree_RSS_bytes']=-1
            elif change=='rss-bool':document['commands'][0]['peak_observed_subtree_RSS_bytes']=False
            else:document['commands'][0]['returncode']=False
            with self.subTest(change=change),self.assertRaises(RuntimeError):driver.validate_prepared_contract(document,'modeled-sha')

    def test_compile_controls_refuse_bad_real_recipe(self):
        label = next(iter(driver.PROBES));database,actual = compiler_data(label)
        controls = [actual.replace('-std=gnu++20','-std=gnu++17'),actual.replace('-UNDEBUG','-DNDEBUG'),
            actual.replace('-MD','-MMD'),actual.replace('-O1','-O3'),actual.replace('-g0','-g3'),actual+'\n'+actual,
            actual.replace(' -c ',' @opaque.rsp -c '),actual.replace('probe.o.d','../../outside.d'),
            actual.replace('/tmp/evidence-stop.hpp','/tmp/wrong-barrier.hpp')]
        for text in controls:
            with self.subTest(text=text):
                with self.assertRaises(RuntimeError):driver.compile_recipe(label,database,text)

    def test_source_compiler_database_disagreement(self):
        label = next(iter(driver.PROBES));database,actual = compiler_data(label)
        for change in ('source','directory','macro'):
            value = copy.deepcopy(database)
            if change=='source':value[0]['file']='/work/other.cc'
            elif change=='directory':value[0]['directory']='/tmp/other'
            else:value[0]['command']=value[0]['command'].replace('-DNDEBUG','-DOTHER')
            with self.subTest(change=change):
                with self.assertRaises(RuntimeError):driver.compile_recipe(label,value,actual)

    def test_effective_separated_macros_and_exact_database(self):
        label = next(iter(driver.PROBES));database,actual = compiler_data(label)
        for macro in ('-D NDEBUG','-DNDEBUG=0','-D NDEBUG=0'):
            text = actual.replace(' -c ',f' {macro} -c ')
            changed = copy.deepcopy(database);changed[0]['command']=changed[0]['command'].replace(' -c ',f' {macro} -c ')
            with self.subTest(macro=macro),self.assertRaises(RuntimeError):driver.compile_recipe(label,changed,text)
        changed=copy.deepcopy(database);changed[0]['command']=changed[0]['command'].replace('-UNDEBUG','-U NDEBUG')
        driver.compile_recipe(label,changed,actual.replace('-UNDEBUG','-U NDEBUG'))
        for changed in (database[:1],database+[copy.deepcopy(database[0])]):
            with self.assertRaises(RuntimeError):driver.compile_recipe(label,changed,actual)

    def test_actual_link_requires_one_exact_object_and_command(self):
        label=next(iter(driver.PROBES));database,actual=compiler_data(label);recipe=driver.compile_recipe(label,database,actual)
        link=f'/usr/bin/aarch64-linux-gnu-g++-13 CMakeFiles/{label}.dir/probe.o -o {label} /opt/ros/jazzy/lib/librclcpp.so -pthread'
        driver.validate_fixture_link(label,recipe,link,actual+'\n'+link)
        for changed,verbose in ((link,actual),(link,link+'\n'+link),(link.replace('-13','-9'),link),
                (link.replace('probe.o','other.o'),link),(link+' other.o',link+' other.o'),
                (link.replace('-o '+label,'-o other'),link)):
            with self.subTest(changed=changed),self.assertRaises(RuntimeError):driver.validate_fixture_link(label,recipe,changed,verbose)

    def test_full_unique_semantic_lines(self):
        for label,contract in driver.PROBE_CONTRACTS.items():
            marker=contract['marker'];driver.validate_semantic_marker(label,'native logging\n'+marker+'\n')
            for text in (marker+'\n'+marker,marker.split(' actual ',1)[0] if ' actual ' in marker else marker[:-1],
                    marker+' extra',marker.replace('observed_gateway_dds_events=0','observed_gateway_dds_events=1') if 'dds_events=' in marker else ''):
                with self.subTest(label=label,text=text),self.assertRaises(RuntimeError):driver.validate_semantic_marker(label,text)

    def test_complete_arm_identity_rejects_every_substitution(self):
        original = {'path':'/fake','sha256':'fake-sha','bytes':64,'device':2,'inode':3,'elf_class':2,'elf_data':1,'e_machine':183}
        self.assertTrue(driver.complete_identity(original,copy.deepcopy(original)))
        for key in original:
            changed = copy.deepcopy(original);changed[key] = 'different'
            with self.subTest(key=key):self.assertFalse(driver.complete_identity(original,changed))

    def test_all_native_threads_children_are_considered(self):
        with tempfile.TemporaryDirectory() as directory:
            proc = Path(directory);(proc/'task/1').mkdir(parents=True);(proc/'task/2').mkdir()
            (proc/'task/1/children').write_text('11 12');(proc/'task/2/children').write_text('12 13')
            with patch.object(driver,'Path',lambda path:proc/'task'):
                self.assertEqual(driver.process_children(100),[11,12,13])

    def test_escaped_child_is_registered_before_scope_refusal(self):
        evidence = types.SimpleNamespace(before={'membership':['modeled']},leaf=Path('/modeled'))
        scope = driver.OwnedProcesses(types.SimpleNamespace(pid=100),evidence)
        identity = {'pid':101,'state':'S','parent_pid':100,'starttime_ticks':200}
        with patch.object(driver,'process_identity',return_value=identity),patch.object(driver.os,'pidfd_open',return_value=701,create=True),patch.object(scope,'check_scope',side_effect=RuntimeError('modeled escape')):
            with self.assertRaises(RuntimeError):scope.adopt(101,{'pid':100,'birth':199})
        self.assertEqual(scope.records[101]['pidfd'],701)

    def test_pidfd_pins_only_stable_identity_and_closes_failed_pin(self):
        identity={'pid':101,'state':'R','parent_pid':100,'starttime_ticks':200}
        for change in ('state','birth','vanished'):
            scope=driver.OwnedProcesses(types.SimpleNamespace(pid=100),types.SimpleNamespace())
            second={**identity,'state':'S'} if change=='state' else {**identity,'starttime_ticks':201} if change=='birth' else FileNotFoundError('modeled exit')
            with patch.object(driver,'process_identity',side_effect=[identity,second]),patch.object(driver.os,'getpid',return_value=100),patch.object(driver.os,'pidfd_open',return_value=701,create=True),patch.object(driver.os,'close') as close,patch.object(scope,'check_scope',return_value=True):
                if change=='state':scope.adopt(101,None);close.assert_not_called();self.assertIn(101,scope.records)
                else:
                    with self.assertRaises((RuntimeError,FileNotFoundError)):scope.adopt(101,None)
                    close.assert_called_once_with(701);self.assertEqual(scope.records,{})

    def test_exit_membership_is_unavailable_but_live_escape_refuses(self):
        with tempfile.TemporaryDirectory() as directory:
            base=Path(directory);(base/'leaf').mkdir();(base/'leaf/cgroup.procs').write_text('')
            (base/'task.cgroup').write_text('0::/modeled')
            evidence=types.SimpleNamespace(before={'membership':['0::/modeled']},leaf=base/'leaf')
            scope=driver.OwnedProcesses(types.SimpleNamespace(pid=100),evidence)
            scope.records[101]={'pid':101,'birth':200,'pidfd':701,'parent':100}
            identity={'pid':101,'state':'S','parent_pid':100,'starttime_ticks':200}
            with patch.object(driver,'process_identity',return_value=identity),patch.object(driver,'Path',lambda path:base/'task.cgroup'):
                self.assertFalse(scope.check_scope(101))
                (base/'task.cgroup').write_text('0::/other')
                with self.assertRaises(RuntimeError):scope.check_scope(101)
                (base/'task.cgroup').unlink();self.assertFalse(scope.check_scope(101))

    def test_owned_rss_includes_pinned_worker_children_and_rejects_reuse(self):
        scope=driver.OwnedProcesses(types.SimpleNamespace(pid=100),types.SimpleNamespace())
        scope.records={pid:{'pid':pid,'birth':birth,'pidfd':pid+600,'parent':7} for pid,birth in ((100,10),(101,11))}
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory)/'status';path.write_text('Name:\tmodeled\nVmRSS:\t8 kB\n')
            def identity(pid):return {'pid':pid,'state':'S','parent_pid':7,'starttime_ticks':10 if pid==100 else 11}
            with patch.object(driver,'Path',return_value=path),patch.object(driver,'process_identity',side_effect=identity):
                self.assertEqual(scope.rss(),16*1024)
            calls=[]
            def reused(pid):
                calls.append(pid)
                return {'pid':pid,'state':'S','parent_pid':7,'starttime_ticks':10 if pid==100 else 12}
            with patch.object(driver,'Path',return_value=path),patch.object(driver,'process_identity',side_effect=reused):
                self.assertEqual(scope.rss(),8*1024)

    def test_compilation_architecture_binds_tools_products_and_owned_root(self):
        with tempfile.TemporaryDirectory() as directory:
            sdk=Path(directory)
            paths={label:sdk/relative for label,relative in {'native-gcc':'usr/bin/aarch64-linux-gnu-g++-13',
                'native-frontend':'usr/lib/gcc/aarch64-linux-gnu/13/cc1plus','native-cmake':'usr/bin/cmake'}.items()}
            for path in paths.values():path.parent.mkdir(parents=True,exist_ok=True);path.write_text('synthetic metadata only')
            facts={label:{'path':str(path),'sha256':label,'bytes':23,'device':9,'inode':index+1,
                          'elf_class':2,'elf_data':1,'e_machine':183} for index,(label,path) in enumerate(paths.items())}
            document=prepared();document['tools']=facts;document['compiler']=facts['native-gcc'];document['frontend']=facts['native-frontend']
            document['commands'][1].update(pid=100,launcher_starttime_ticks=200)
            document['probes']={name:{'binary':{'path':'/modeled/'+name,'sha256':name}} for name in driver.PROBES}
            membership=['0::/modeled/leaf']
            root={'pid':100,'name':'compile/','ancestry':[100],'starttime_ticks':200,'membership':membership,
                'live_cgroup_procs':True,'kernel_exe_elf':{'e_machine':62,'sha256':'host-chroot'},'mapped_arm_targets':[]}
            worker={**root,'pid':101,'ancestry':[100,101],'starttime_ticks':201,
                'kernel_exe_elf':{'e_machine':62,'sha256':'modeled-qemu'},'mapped_arm_targets':list(facts)}
            architecture={'cgroup_incarnation_before':copy.deepcopy(document['cgroup_incarnation_before']),
                'cgroup_before':{'membership':membership},'arm_targets':{**facts,**{name:item['binary'] for name,item in document['probes'].items()}},
                'actual_process_samples':[root,worker],'expected_host_emulator_sha256':['modeled-qemu']}
            with (patch.object(driver,'rooted',side_effect=lambda base,logical:base/logical.lstrip('/')),
                    patch.object(driver,'require_arm',side_effect=lambda path:next(fact for fact in facts.values() if fact['path']==str(path)))):
                driver.validate_compilation_architecture(document,architecture,sdk,[])
                for change in ('root-absent','root-stale','worker-predates','wrong-host','wrong-cgroup','wrong-ancestry',
                               'tool-missing','product-changed','raw-incarnation','declared-tool'):
                    original=copy.deepcopy(document);proof=copy.deepcopy(architecture)
                    if change=='root-absent':proof['actual_process_samples'].pop(0)
                    elif change=='root-stale':proof['actual_process_samples'][0]['starttime_ticks']=199
                    elif change=='worker-predates':proof['actual_process_samples'][1]['starttime_ticks']=199
                    elif change=='wrong-host':proof['actual_process_samples'][1]['kernel_exe_elf']['sha256']='other'
                    elif change=='wrong-cgroup':proof['actual_process_samples'][1]['membership']=['0::/other']
                    elif change=='wrong-ancestry':proof['actual_process_samples'][1]['ancestry']=[99,101]
                    elif change=='tool-missing':proof['actual_process_samples'][1]['mapped_arm_targets'].pop()
                    elif change=='product-changed':proof['arm_targets'][next(iter(driver.PROBES))]['sha256']='other'
                    elif change=='raw-incarnation':proof['cgroup_incarnation_before']['inode']=999
                    else:original['compiler']['sha256']='other'
                    with self.subTest(change=change),self.assertRaises(RuntimeError):driver.validate_compilation_architecture(original,proof,sdk,[])

    def test_same_descriptor_raw_dependency_rule_and_generated_closure(self):
        label=next(iter(driver.PROBES));database,actual=compiler_data(label);recipe=driver.compile_recipe(label,database,actual)
        with tempfile.TemporaryDirectory() as directory:
            base=Path(directory).resolve();source=base/'source';sdk=base/'sdk';temporary=base/'tmp'
            for path in (source/'fixture.cc',sdk/'usr/include/header.h',temporary/'evidence-stop.hpp'):
                path.parent.mkdir(parents=True,exist_ok=True);path.write_text('synthetic metadata only')
            dep=base/'actual.d';rule=recipe['compiled_object_guest_path']+': /work/fixture.cc /usr/include/header.h /tmp/evidence-stop.hpp\n';dep.write_text(rule)
            with patch.object(driver,'rooted',side_effect=lambda root,logical:root/logical.lstrip('/')):
                inputs,binding=driver.fixture_closure(dep,source,sdk,temporary,recipe)
                self.assertEqual(set(inputs),{'/work/fixture.cc','/usr/include/header.h','/tmp/evidence-stop.hpp'})
                self.assertEqual(binding,read_file_binding(dep))
                for target in ('/tmp/build/wrong.o',recipe['compiled_object_guest_path']+' second.o'):
                    dep.write_text(rule.replace(recipe['compiled_object_guest_path'],target))
                    with self.assertRaises(RuntimeError):driver.fixture_closure(dep,source,sdk,temporary,recipe)
                outside=base/'outside.h';outside.write_text('outside');(source/'escaped.h').symlink_to(outside)
                dep.write_text(recipe['compiled_object_guest_path']+': /work/escaped.h\n')
                with self.assertRaises(RuntimeError):driver.fixture_closure(dep,source,sdk,temporary,recipe)

    def test_reused_numeric_pid_never_signals_new_process(self):
        process = types.SimpleNamespace(pid=100,wait=lambda timeout:0)
        scope = driver.OwnedProcesses(process,types.SimpleNamespace())
        scope.records[100]={'pid':100,'birth':10,'pidfd':701,'parent':7}
        current={'pid':100,'state':'S','parent_pid':7,'starttime_ticks':11}
        with patch.object(driver,'process_identity',return_value=current),patch.object(driver.signal,'pidfd_send_signal',create=True) as send,patch.object(driver.os,'close'):
            result = scope.cleanup()
        send.assert_not_called();self.assertEqual(result['remaining'],[]);self.assertTrue(result['errors'])

    def test_gateway_requires_successful_current_source_and_complete_product(self):
        with patch.object(driver,'require_arm',return_value={'path':'/fake','e_machine':183}):
            for document in ({},{'status':'PASS','source_manifest_sha256':'wrong'},
                {'status':'PASS','source_manifest_sha256':'modeled','targets':{driver.GATEWAY_TARGET:{'path':'/different'}}}):
                with self.assertRaises(RuntimeError):driver.require_current_gateway(document,'/fake','modeled')

    def test_source_guards_are_present_in_real_control_flow(self):
        body = Path(driver.__file__).read_text();tree = ast.parse(body)
        imports = {node.module for node in tree.body if isinstance(node,ast.ImportFrom)}
        self.assertIn('gateway_qualification',imports)
        names = {node.name for node in tree.body if isinstance(node,(ast.ClassDef,ast.FunctionDef))}
        self.assertTrue({'OwnedProcesses','process_children','gateway_stop_proof','validate_prepared_contract','compile_recipe'}<=names)
        self.assertNotIn('os.killpg(',body);self.assertNotIn('os.kill(',body)
        for token in ['strict[\'input_pins\']','scope.adopt(process.pid,None)','--parallel\',\'1','scope.check_scope(child)',
            'native_behavior_status\'] = \'PASS\'','memory.swap.max','actual_gateway_ARM_mapping_before_resume',
            'source/\'test/0073.ros-tests/support/source/ros_aarch64\'/name','read_file_binding(path)','pins_preserved(pins)']:
            with self.subTest(token=token):self.assertIn(token,body)


if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report',type=Path)
    args=parser.parse_args()
    if args.report and args.report.exists():parser.error('report already exists')
    source=Path(driver.__file__).resolve().parents[2]
    paths=[Path(driver.__file__).with_name(name) for name in driver.DRIVER_INPUTS]
    paths += [Path(__file__),Path(driver.__file__).with_name('local-parameter-events.md')]
    paths += [source/'test/0073.ros-tests/support/source/ros_local_parameter_events'/name for name in
              ('contract_ros2.cc','native_ros2.cc','common.h','CMakeLists.txt','README.md','source_review.py')]
    paths += [source/'src/ros/gateway/ros2/resources.h',source/'src/ros/gateway/ros2/main.cc',source/'sdk/origin/common/codec.hpp']
    before=[read_file_binding(path) for path in paths]
    result=unittest.TextTestRunner(verbosity=1).run(unittest.defaultTestLoader.loadTestsFromTestCase(DriverModels))
    after=[read_file_binding(path) for path in paths]
    stable=before==after
    document={'status':'SOURCE_AND_MODEL_PASS' if result.wasSuccessful() and stable else 'FAIL',
        'scope':'Pure Python source, handwritten metadata and mocked ownership admission controls only',
        'tests_run':result.testsRun,'errors':len(result.errors),'failures':len(result.failures),
        'inputs_before':before,'inputs_after':after,'inputs_preserved':stable,
        'native_compilation':'NOT_PERFORMED','actual_native_behavior_probes':0,'native_ros_behavior':'NOT_PERFORMED'}
    if args.report:
        with args.report.open('x') as stream:json.dump(document,stream,indent=2);stream.write('\n')
    print(document['status'],result.testsRun,'test methods; native compilation and ROS behavior NOT_PERFORMED')
    raise SystemExit(0 if document['status']=='SOURCE_AND_MODEL_PASS' else 1)
