#!/usr/bin/env python3
"""Pure source/report/filesystem models for strict current-native qualification.

Every artifact/tool contains ordinary synthetic text and every ELF fact is
mocked. Raw compiler commands, depfiles, SDK trees and architecture records are
handwritten models, never actual compilation evidence. Alias tests mutate only
new temporary model SDKs. No live cgroup, compiler, native/QEMU/ROS/Wasm program
is started, and no real task SDK alias or historical evidence is changed.
"""
import copy
import hashlib
import json
from pathlib import Path
import shlex
import shutil
import tempfile
import unittest
from unittest.mock import patch

import gateway_qualification as gateway
import qualify_current_native as native
from feature_admission_models import synthetic_fact, replace_same_bytes
from rebuild_affected import (build_profile, original_compile_command, dependency_output,
                             sdk_inventory, sdk_root_aliases, custom_selection,
                             ROS2_FEATURE_SOURCES, ROS2_FEATURE_MACROS,
                             capture_generated_features, generated_feature_pin)
from runtime import cgroup_oom_deltas, MAPPING_METHOD

ROOT = Path(__file__).resolve().parents[2]


def synthetic_opened_mapping(sample, targets, membership):
    """Independent explicitly synthetic opened-FD receipts for pure admission."""
    sample.setdefault('pid',sample['ancestry'][-1])
    sample.setdefault('parent_pid',sample['ancestry'][-2] if len(sample['ancestry'])>1 else 1)
    sample.setdefault('starttime_ticks',100)
    sample['membership']=membership
    host=sample.setdefault('kernel_exe_elf',{'e_machine':62,'sha256':'SYNTHETIC_ONLY'})
    host.setdefault('device',1);host.setdefault('inode',2)
    host.setdefault('bytes',64);host.setdefault('mtime_ns',1);host.setdefault('ctime_ns',1)
    guarded={key:sample[key] for key in ('pid','parent_pid','starttime_ticks','membership','live_cgroup_procs')}
    guarded.update(state='S',host_identity={key:host[key] for key in ('device','inode','bytes','mtime_ns','ctime_ns')})
    maps=[];selected={}
    for label in sample['mapped_arm_targets']:
        image={key:value for key,value in targets[label].items() if key!='path'}
        state=Path(targets[label]['path']).stat();image.update(mtime_ns=state.st_mtime_ns,ctime_ns=state.st_ctime_ns)
        line='1000-2000 r-xp 0 ff:ff 999 SYNTHETIC_DISPLAY_ONLY';maps.append(line)
        selected[label]=[{'range':'1000-2000','raw_mapping':line,'opened_file':image,
            'hash_method':'FULL_OPENED_DESCRIPTOR_HASH','initial_hash_monotonic_ns':1,
            'initial_full_hash':{'method':'FULL_OPENED_DESCRIPTOR_HASH','opened_file':copy.deepcopy(image),'capture_monotonic_ns':1}}]
    sample['opened_target_mappings']={'format_version':1,'method':MAPPING_METHOD,'identity_before':copy.deepcopy(guarded),
        'identity_after':copy.deepcopy(guarded),'raw_maps':maps,'targets':selected}


def put(path, value):
    """Create one small synthetic model file and return its immutable byte facts."""
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(value if isinstance(value, bytes) else value.encode())
    return {'path':str(path),'sha256':hashlib.sha256(path.read_bytes()).hexdigest()}


class Dataset:
    def __init__(self, base, version):
        self.base, self.version = base, version
        self.profile = build_profile(version); prefix, target = self.profile['prefix'], self.profile['target']
        self.source, self.workspace, self.out = base/'source', base/'sdk', base/'build'
        self.sdk = self.workspace/'gateway'/(self.profile['distro']+'-rootfs')
        self.manifest = base/'source-manifest.json'
        required = ['gateways/'+prefix+'/'+name for name in ('main.cc','action.h','resources.h')]
        required += ['gateways/common/config.h']
        if version == 1: required += ['src/uwvm2/imported/ros/common/parameter_search_contract.h']
        for name in required: put(self.source/name, '// Synthetic fixture source; never compiled.\n')
        if version == 2:
            for name in ROS2_FEATURE_SOURCES:
                destination = self.source/name
                destination.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(ROOT/name, destination)
        for name in ('rebuild_affected.py','runtime.py','run_resources.py'):
            destination = self.source/'tests/ros_aarch64'/name
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT/'tests/ros_aarch64'/name, destination)
        frozen = {str(path.relative_to(self.source)):{'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'bytes':path.stat().st_size}
                  for path in self.source.rglob('*') if path.is_file()}
        put(self.manifest,json.dumps(frozen)); manifest_sha = hashlib.sha256(self.manifest.read_bytes()).hexdigest()
        compiler = self.sdk/self.profile['compiler'].lstrip('/')
        frontend = self.sdk/'usr/lib/gcc/aarch64-linux-gnu'/str(self.profile['gcc'])/'cc1plus'
        cmake = self.sdk/'usr/bin/cmake'
        for path in (compiler,frontend,cmake): put(path,'Synthetic tool; never executable.\n'+path.name)
        standard = '/usr/include/c++/'+str(self.profile['gcc'])+'/model.h'
        ros = '/opt/ros/'+self.profile['distro']+'/include/model.h'
        library = '/opt/ros/'+self.profile['distro']+'/lib/libmodel.so'
        for logical in (standard,ros,library): put(self.sdk/logical.lstrip('/'),'Synthetic SDK bytes.\n'+logical)
        inventory = sdk_inventory(self.sdk)
        aliases = sdk_root_aliases(self.sdk)
        inventory_ref = put(self.out/'sdk-static-inputs-before.json',json.dumps(inventory))
        self.binary = self.out/'owned-tmp'/Path(self.profile['build_guest']).name/target
        put(self.binary,'Synthetic compiler product; never executable.\n')
        argv = [self.profile['compiler'],'-O2','-g0','-DNDEBUG','-std=gnu++17','-MD','-MF','/tmp/gateway-origin.d',
                '-o','CMakeFiles/'+target+'.dir/main.cc.o','-c','/work/gateways/'+prefix+'/main.cc']
        database = [{'directory':self.profile['build_guest'],'file':argv[-1],'arguments':argv}]
        log = shlex.join(argv)+'\n'
        recipe = original_compile_command(self.profile,database,log)
        output = dependency_output(self.profile,recipe)
        object_guest = self.profile['build_guest']+'/CMakeFiles/'+target+'.dir/main.cc.o'
        feature_path = self.profile['build_guest']+'/uwvm_ros2_features.h'
        body = ('CMakeFiles/'+target+'.dir/main.cc.o: '+' '.join('/work/'+name for name in required)+
                ' '+standard+' '+ros+(' '+feature_path if version == 2 else '')+'\n')
        actual_dep = self.out/'owned-tmp'/output['owned_tmp_relative']; put(actual_dep,body)
        key = prefix+':'+output['archive_relative']
        dep = put(self.out/(prefix+'-metadata')/output['archive_relative'],body)
        database_ref = put(self.out/(prefix+'-metadata')/'compile_commands.json',json.dumps(database))
        log_ref = put(self.out/(prefix+'-build-gateway.log'),log)
        link_ref = put(self.out/(prefix+'-metadata')/'CMakeFiles'/(target+'.dir')/'link.txt',shlex.join([self.profile['compiler'],library,'-o',target])+'\n')
        metadata = {key:dep,prefix+':compile_commands.json':database_ref,prefix+':CMakeFiles/'+target+'.dir/link.txt':link_ref}
        # Current targeted producers explicitly report the default empty custom
        # selection. These cache bytes and configure argv remain handwritten
        # synthetic evidence; CMake is never run by this model.
        selection = custom_selection(version, None, None, [], None, self.source, self.sdk, frozen)
        cache = ('UWVM_ROS_CUSTOM_REGISTRY_HEADER:FILEPATH=\n'
                 'UWVM_ROS_CUSTOM_ROS'+str(version)+'_HEADER:FILEPATH=\n'
                 'UWVM_ROS_CUSTOM_ROS'+str(version)+'_PACKAGES:STRING=\n')
        if version == 2:
            cache += ('CMAKE_HOME_DIRECTORY:INTERNAL=/work/gateways/ros2\n'
                      'CMAKE_CXX_COMPILER:FILEPATH=/usr/bin/aarch64-linux-gnu-g++-13\n'
                      'CMAKE_BUILD_TYPE:STRING=RelWithDebInfo\nUWVM_ROS2_CXX_17:INTERNAL=1\n')
            flags = {name:name in ('qos_class','quiet_node','type_description') for name in ROS2_FEATURE_MACROS}
            cache += ''.join('UWVM_ROS2_HAVE_'+macro+'_COMPILE_LINK:INTERNAL='+('1' if flags[name] else '')+'\n'
                             for name,macro in ROS2_FEATURE_MACROS.items())
            header = (self.source/ROS2_FEATURE_SOURCES[2]).read_text().replace('@CMAKE_CXX_STANDARD@','17')
            for name,macro in ROS2_FEATURE_MACROS.items():
                header = header.replace('#cmakedefine01 UWVM_ROS2_HAVE_'+macro+'\n',
                    '#define UWVM_ROS2_HAVE_'+macro+' '+str(int(flags[name]))+'\n')
            feature_build = self.binary.parent
            put(feature_build/'uwvm_ros2_features.h',header)
            put(feature_build/'uwvm_ros2_features.json',json.dumps(
                {'format_version':1,'backend':'ros2','cxx_standard':17,'features':flags}))
            put(feature_build/'CMakeCache.txt',cache)
        metadata[prefix+':CMakeCache.txt'] = put(self.out/(prefix+'-metadata')/'CMakeCache.txt', cache)
        configure = ['/usr/bin/chroot', str(self.out/'rootfs'), '/bin/bash', '-lc',
            'source /opt/ros/'+self.profile['distro']+'/setup.bash && exec "$@"', 'uwvm-affected-build',
            'cmake', '-S', '/work/gateways/'+prefix, '-B', self.profile['build_guest'],
            '-DCMAKE_CXX_COMPILER='+self.profile['compiler'], '-DCMAKE_BUILD_TYPE=RelWithDebInfo',
            '-DCMAKE_CXX_FLAGS='+' '.join(self.profile['dependency_flags']),
            '-DCMAKE_CXX_FLAGS_RELWITHDEBINFO=-O2 -g0 -DNDEBUG', '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON']
        driver_inputs = {}
        for name in ('rebuild_affected.py','runtime.py','run_resources.py'):
            archive = self.out/'driver-inputs'/name
            ref = put(archive,(self.source/'tests/ros_aarch64'/name).read_bytes())
            driver_inputs[name] = {'archive':ref['path'],'sha256':ref['sha256']}
        identity = {'path':'/synthetic/leaf','device':29,'inode':123,'boot_id':'SYNTHETIC_MODEL_ONLY'}
        before = {'status':'PASS','effective_memory_max':68719476736,'membership':['0::/synthetic/leaf'],
                  'cgroup_ancestors':[{'path':'/synthetic/leaf','memory.max':'68719476736',
                    'memory.events':'oom 0\noom_kill 0\noom_group_kill 0'}]}
        _,deltas = cgroup_oom_deltas(before,before,True)
        emulator_path = base/'model-qemu'; put(emulator_path,'Synthetic emulator bytes; never executable.\n')
        emulator = {**synthetic_fact(emulator_path),'e_machine':62}
        self.arch = {'status':'PASS','same_cgroup_incarnation':True,'cgroup_incarnation_before':identity,
            'cgroup_incarnation_after_before_probe':identity,'cgroup_incarnation_after':identity,
            'fatal_errors':[],'remaining_owned_processes':[],'cgroup_before':before,'cgroup_after':before,
            'cgroup_ancestor_event_deltas':deltas,'expected_host_emulator_sha256':[emulator['sha256']],
            'arm_targets':{prefix+'-compiler':synthetic_fact(compiler),prefix+'-frontend':synthetic_fact(frontend),prefix+'-cmake':synthetic_fact(cmake)},
            'actual_process_samples':[{'name':prefix+'-build-gateway/','ancestry':[123,124],
                'live_cgroup_procs':True,'membership':before['membership'],
                'mapped_arm_targets':[prefix+'-compiler',prefix+'-frontend'],'kernel_exe_elf':emulator}]}
        for sample in self.arch['actual_process_samples']:
            synthetic_opened_mapping(sample,self.arch['arm_targets'],before['membership'])
        self.arch_path = self.out/'execution/architecture-evidence.json';put(self.arch_path,json.dumps(self.arch))
        headers = {logical:{'sha256':inventory[logical.lstrip('/')]['sha256'],'resolved_sdk_path':logical.lstrip('/'),
                           'bytes':(self.sdk/logical.lstrip('/')).stat().st_size} for logical in (standard,ros)}
        linked = {logical:{'sha256':inventory[logical.lstrip('/')]['sha256'],'resolved_sdk_path':logical.lstrip('/')}
                  for logical in (self.profile['compiler'],library)}
        self.row = {'status':'PASS','version':version,'profile':self.profile,'target':target,
            'source':str(self.source),'source_manifest':str(self.manifest),'source_manifest_sha256':manifest_sha,
            'sdk_workspace':str(self.workspace),'targets':{target:synthetic_fact(self.binary)},
            'source_preserved':True,'sdk_static_preserved':True,'sdk_root_aliases_preserved':True,
            'custom_selection':selection,'custom_inputs_preserved':True,
            'same_cgroup_incarnation':True,'system_headers_recorded_in_original_depfile':True,
            'post_cleanup_mountinfo':[],'mount_cleanup':[{'returncode':0}],
            'execution_evidence':str(self.arch_path),'sdk_static_inputs':inventory_ref,'sdk_root_aliases_before':aliases,
            'build_metadata':metadata,'actual_compiler_command':{**recipe,'log':log_ref['path'],'log_sha256':log_ref['sha256']},
            'actual_compiler_depfile_path':output['actual_guest_output'],'actual_compiler_depfile_host_path':str(actual_dep),
            'compiler_dependency_records':{key:{**output,'sha256':dep['sha256'],'actual_host_output':str(actual_dep),
                'dependency_output_options':recipe['options']['dependency_output_options'],
                'mf_outputs_in_argv_order':recipe['options']['mf_outputs_in_argv_order'],'compiled_object_guest_path':object_guest,
                'method':'Actual GCC'+str(self.profile['gcc'])+' compilation -MD/-MF output selected from the successful verbose compiler argv; not CMake scanner output and not preprocessing replay'}},
            'compiled_source_dependencies':{key:{name:frozen[name]['sha256'] for name in required}},
            'sdk_header_dependencies':headers,'sdk_link_inputs':linked,
            'system_header_dependency_categories':{'standard_cplusplus':1,'ros_sdk':1,'total':2},
            'driver_inputs':driver_inputs,'emulator':emulator,
            'builds':[{'name':prefix+'-build-gateway','status':'PASS','returncode':0,'pid':123},
                      {'name':prefix+'-configure','status':'PASS','returncode':0,'command':configure}]}
        if version == 2:
            put(self.out/'ros2-configure.log','Synthetic configure observations; no CMake program executed.\n')
            self.row['generated_feature_dependencies'] = capture_generated_features(self.profile,
                self.binary.parent,self.out,self.source,frozen,manifest_sha,self.row['builds'])
            self.row['generated_feature_inputs_preserved'] = True
        self.report = self.out/'build-report.json'; self.save()

    def save(self): put(self.report,json.dumps(self.row))
    def qualify(self):
        return gateway.qualify_gateway(self.binary,self.report,self.manifest,self.source,self.sdk,self.version)


class QualificationModels(unittest.TestCase):
    def test_ros2_generated_feature_records_are_exact_and_fail_closed(self):
        changes = ('absent-record','foreign-record','wrong-logical-path','missing-actual-dependency',
                   'foreign-tmp-dependency','actual-replaced','archive-replaced','header-bytes',
                   'feature-bool-as-int','wrong-cache-source','configure-failed','archive-symlink')
        for change in changes:
            with self.subTest(change=change),tempfile.TemporaryDirectory(prefix='current-feature-negative-') as temporary:
                case = Dataset(Path(temporary).resolve(),2)
                logical = case.profile['build_guest']+'/uwvm_ros2_features.h'
                records = case.row['generated_feature_dependencies']; record = records[logical]
                if change == 'absent-record': records.clear()
                elif change == 'foreign-record': records['/tmp/unrelated/features.h'] = copy.deepcopy(record)
                elif change == 'wrong-logical-path': record['guest_path'] = '/tmp/other/uwvm_ros2_features.h'
                elif change in ('missing-actual-dependency','foreign-tmp-dependency'):
                    key = next(iter(case.row['compiler_dependency_records']))
                    reference = case.row['build_metadata'][key]
                    body = Path(reference['path']).read_text()
                    body = body.replace(' '+logical,'') if change == 'missing-actual-dependency' else body.rstrip()+' /tmp/foreign.h\n'
                    reference.update(put(Path(reference['path']),body))
                    actual = Path(case.row['actual_compiler_depfile_host_path']);put(actual,body)
                    case.row['compiler_dependency_records'][key]['sha256'] = reference['sha256']
                elif change in ('actual-replaced','archive-replaced'):
                    which = 'actual' if change == 'actual-replaced' else 'archive'
                    replace_same_bytes(Path(record['inputs']['uwvm_ros2_features.h'][which]['path']))
                elif change in ('header-bytes','feature-bool-as-int','wrong-cache-source'):
                    name = ('uwvm_ros2_features.h' if change == 'header-bytes' else
                            'uwvm_ros2_features.json' if change == 'feature-bool-as-int' else 'CMakeCache.txt')
                    body = Path(record['inputs'][name]['actual']['path']).read_text()
                    if change == 'header-bytes': body += '// Unexplained generated bytes\n'
                    elif change == 'wrong-cache-source': body = body.replace('/work/gateways/ros2','/work/foreign')
                    else:
                        value = json.loads(body);value['features']['qos_class'] = 1;body = json.dumps(value)
                    for which in ('actual','archive'):
                        path = Path(record['inputs'][name][which]['path']);put(path,body)
                        record['inputs'][name][which] = generated_feature_pin(path)
                elif change == 'configure-failed': case.row['builds'][1]['returncode'] = 1
                elif change == 'archive-symlink':
                    path = Path(record['inputs']['uwvm_ros2_features.h']['archive']['path'])
                    target = path.with_name('unselected.h');path.rename(target);path.symlink_to(target.name)
                case.save()
                with patch.object(gateway,'require_arm',side_effect=synthetic_fact),self.assertRaises(RuntimeError):
                    case.qualify()

    def test_both_exact_targeted_producer_schemas_and_immutable_pins(self):
        for version in (1,2):
            with self.subTest(version=version),tempfile.TemporaryDirectory(prefix='current-gateway-model-') as temporary:
                case = Dataset(Path(temporary).resolve(),version)
                with patch.object(gateway,'require_arm',side_effect=synthetic_fact):
                    result = case.qualify()
                    self.assertEqual(result['binary'],case.row['targets'][case.profile['target']])
                    self.assertTrue(gateway.pins_preserved(result['input_pins']))
                    replace_same_bytes(case.binary)
                    self.assertFalse(gateway.pins_preserved(result['input_pins']))
                    with self.assertRaises(RuntimeError): case.qualify()

    def test_source_inventory_sdk_recipe_dependency_tool_and_arch_failures(self):
        for change in ('historical-replay','wrong-source','wrong-version','partial-targets','failed-mount',
                       'wrong-final-mf','replay-method','wrong-object','missing-header','wrong-linked-set',
                       'wrong-category','failed-compile','wrong-helper','changed-sdk','missing-source',
                       'changed-tool','missing-subtree','wrong-emulator','oom','changed-incarnation'):
            with self.subTest(change=change),tempfile.TemporaryDirectory(prefix='current-gateway-negative-model-') as temporary:
                case = Dataset(Path(temporary).resolve(),1); key = next(iter(case.row['compiler_dependency_records']))
                if change == 'historical-replay': case.row['dependency_replay'] = {}
                elif change == 'wrong-source': case.row['source_manifest_sha256'] = '0'*64
                elif change == 'wrong-version': case.row['version'] = 2
                elif change == 'partial-targets': case.row['targets'] = {}
                elif change == 'failed-mount': case.row['mount_cleanup'][0]['returncode'] = 1
                elif change == 'wrong-final-mf': case.row['compiler_dependency_records'][key]['actual_guest_output'] = '/tmp/other.d'
                elif change == 'replay-method': case.row['compiler_dependency_records'][key]['method'] = 'GCC -M replay'
                elif change == 'wrong-object': case.row['compiler_dependency_records'][key]['compiled_object_guest_path'] = '/tmp/other.o'
                elif change == 'missing-header': case.row['sdk_header_dependencies'].pop(next(iter(case.row['sdk_header_dependencies'])))
                elif change == 'wrong-linked-set': case.row['sdk_link_inputs'] = {}
                elif change == 'wrong-category': case.row['system_header_dependency_categories']['total'] = 0
                elif change == 'failed-compile': case.row['builds'][0]['returncode'] = 1
                elif change == 'wrong-helper': case.row['driver_inputs']['runtime.py']['sha256'] = '0'*64
                elif change == 'changed-sdk': put(case.sdk/'usr/include/new-unqualified.h','Unqualified new SDK input\n')
                elif change == 'missing-source': (case.source/'gateways/ros1/action.h').unlink()
                elif change == 'changed-tool': replace_same_bytes(case.sdk/case.profile['compiler'].lstrip('/'))
                elif change == 'missing-subtree': case.arch['actual_process_samples'][0]['mapped_arm_targets'] = []
                elif change == 'wrong-emulator': case.arch['actual_process_samples'][0]['kernel_exe_elf']['e_machine'] = 183
                elif change == 'oom': case.arch['cgroup_after'] = copy.deepcopy(case.arch['cgroup_after']);case.arch['cgroup_after']['cgroup_ancestors'][0]['memory.events'] = 'oom 1\noom_kill 0\noom_group_kill 0'
                else: case.arch['cgroup_incarnation_after'] = {**case.arch['cgroup_incarnation_after'],'inode':999}
                case.save();put(case.arch_path,json.dumps(case.arch))
                with patch.object(gateway,'require_arm',side_effect=synthetic_fact),self.assertRaises((RuntimeError,FileNotFoundError)):
                    case.qualify()

    def test_full_view_staging_owned_rollback_and_real_directory_refusal(self):
        for failure in (None,4,5):
            with self.subTest(failure=failure),tempfile.TemporaryDirectory(prefix='current-native-view-model-') as temporary:
                base = Path(temporary).resolve();sdk = base/'sdk'; targets = {}
                for name in native.native_names():
                    path = base/'products'/name;put(path,'Synthetic model product; never execute.\n'+name);targets[name] = synthetic_fact(path)
                for version,distro in ((1,'noetic'),(2,'jazzy')):
                    root = sdk/'gateway'/(distro+'-rootfs');put(root/'.uwvm-task-owned','Synthetic model SDK\n')
                    old = root/'tmp'/('old-'+str(version));old.mkdir(parents=True)
                    (root/'tmp'/('ros'+str(version)+'-build')).symlink_to(old.name)
                calls = []
                def guard():
                    calls.append(True)
                    if len(calls) == failure: raise RuntimeError('synthetic input guard failure')
                with patch.object(native,'require_arm',side_effect=synthetic_fact):
                    if failure:
                        with self.assertRaises(RuntimeError): native.materialize_views(sdk,targets,'model-r1',guard)
                        for version,distro in ((1,'noetic'),(2,'jazzy')):
                            alias = sdk/'gateway'/(distro+'-rootfs')/'tmp'/('ros'+str(version)+'-build')
                            self.assertEqual(alias.readlink(),Path('old-'+str(version)))
                    else:
                        result = native.materialize_views(sdk,targets,'model-r1',guard)
                        for version in (1,2):
                            for kind in native.KINDS[version]:
                                alias = Path(result['canonical_paths'][str(version)])/('uwvm-ros'+str(version)+'-'+kind)
                                self.assertEqual(alias.resolve(),Path(targets[alias.name]['path']))
                        native.rollback_views(result)
                        for version,distro in ((1,'noetic'),(2,'jazzy')):
                            alias = sdk/'gateway'/(distro+'-rootfs')/'tmp'/('ros'+str(version)+'-build')
                            self.assertEqual(alias.readlink(),Path('old-'+str(version)))
        with tempfile.TemporaryDirectory(prefix='current-native-directory-model-') as temporary:
            path = Path(temporary); path.mkdir(exist_ok=True)
            with self.assertRaises(RuntimeError): native.alias_fact(path)

    def test_fresh_base_sixteen_probe_source_and_subtree_admission(self):
        with tempfile.TemporaryDirectory(prefix='current-native-base-model-') as temporary:
            base = Path(temporary).resolve(); source = base/'source'; workspace = base/'sdk'; out = base/'base-build'
            frozen, metadata, dependencies, targets, builds, samples = {}, {}, {}, {}, [], []
            def source_file(name, content):
                ref = put(source/name,content)
                frozen[name] = {'sha256':ref['sha256'],'bytes':(source/name).stat().st_size}
            for name in ('build_gateways.py','runtime.py'):
                source_file('tests/ros_aarch64/'+name,(ROOT/'tests/ros_aarch64'/name).read_bytes())
            drivers = {}
            for name in ('build_gateways.py','runtime.py'):
                ref = put(out/'driver-inputs'/name,(source/'tests/ros_aarch64'/name).read_bytes())
                drivers[name] = {'archive':ref['path'],'sha256':ref['sha256']}
            for version,kinds in native.KINDS.items():
                for command in ('compiler-version','package-inventory','configure'):
                    builds.append({'name':f'ros{version}-{command}','status':'PASS','returncode':0,'pid':100})
                for index,kind in enumerate(kinds):
                    name = f'uwvm-ros{version}-{kind}'; path = out/'products'/name
                    put(path,'Synthetic base product; never executable.\n'+name); targets[name] = synthetic_fact(path)
                    command = f'ros{version}-build-{kind}'; pid = 200+version*20+index
                    builds.append({'name':command,'status':'PASS','returncode':0,'pid':pid})
                    samples.append({'name':command,'ancestry':[pid,pid+1],'live_cgroup_procs':True,
                                    'mapped_arm_targets':[f'ros{version}-frontend']})
                    if kind == 'gateway': continue
                    relative = native.SOURCES[kind].format(version=version)
                    source_file(relative,'// Synthetic base source; never compiled.\n'+relative)
                    key = f'ros{version}:CMakeFiles/{name}.dir/probe.cc.o.d'
                    metadata[key] = put(out/(str(version)+'-'+kind+'.d'),'probe.cc.o: /work/'+relative+'\n')
                    dependencies[key] = {relative:frozen[relative]['sha256']}
            manifest = base/'source-manifest.json';put(manifest,json.dumps(frozen))
            manifest_sha = hashlib.sha256(manifest.read_bytes()).hexdigest()
            identity = {'path':'/synthetic/leaf','device':29,'inode':123,'boot_id':'SYNTHETIC_BASE_MODEL'}
            facts = {'status':'PASS','effective_memory_max':68719476736,'membership':['0::/synthetic/leaf'],
                     'cgroup_ancestors':[{'path':'/synthetic/leaf','memory.max':'68719476736',
                                         'memory.events':'oom 0\noom_kill 0\noom_group_kill 0'}]}
            _,deltas = cgroup_oom_deltas(facts,facts,True)
            architecture = {'status':'PASS','same_cgroup_incarnation':True,'cgroup_incarnation_before':identity,
                            'cgroup_incarnation_after_before_probe':identity,'cgroup_incarnation_after':identity,
                            'fatal_errors':[],'remaining_owned_processes':[],'cgroup_before':facts,'cgroup_after':facts,
                            'cgroup_ancestor_event_deltas':deltas,'actual_process_samples':samples}
            architecture['arm_targets']={}
            for version in (1,2):
                frontend=out/('synthetic-frontend-'+str(version));put(frontend,'Synthetic model bytes; never execute.')
                architecture['arm_targets'][f'ros{version}-frontend']=synthetic_fact(frontend)
            for sample in samples:
                synthetic_opened_mapping(sample,architecture['arm_targets'],facts['membership'])
            arch_path = out/'execution/architecture-evidence.json';put(arch_path,json.dumps(architecture))
            original = {'status':'PASS','source':str(source),'sdk_workspace':str(workspace),
                        'source_manifest_sha256':manifest_sha,'targets':targets,'post_cleanup_mountinfo':[],
                        'mount_cleanup':[{'returncode':0}],'execution_evidence':str(arch_path),
                        'builds':builds,'driver_inputs':drivers,'build_metadata':metadata,
                        'compiled_source_dependencies':dependencies}
            report = out/'build-report.json';put(report,json.dumps(original))
            with patch.object(native,'require_arm',side_effect=synthetic_fact):
                result = native.qualify_base(report,manifest,source,workspace)
                self.assertEqual(len(result['targets']),16)
                self.assertTrue(all(row['method']=='FRESH_CURRENT_BASE_PROJECT_MMD_ONLY' for row in result['methods'].values()))
                for change in ('historical','old-source','missing-target','duplicate-command','missing-cleanup',
                               'missing-dependency','wrong-project-set','missing-subtree'):
                    bad = copy.deepcopy(original); arch = copy.deepcopy(architecture)
                    if change == 'historical': bad['dependency_replay'] = {}
                    elif change == 'old-source': bad['source_manifest_sha256'] = '0'*64
                    elif change == 'missing-target': bad['targets'].pop(next(iter(targets)))
                    elif change == 'duplicate-command': bad['builds'][-1] = bad['builds'][0]
                    elif change == 'missing-cleanup': bad.pop('post_cleanup_mountinfo')
                    elif change == 'missing-dependency': bad['compiled_source_dependencies'].pop(next(iter(dependencies)))
                    elif change == 'wrong-project-set': bad['compiled_source_dependencies'][next(iter(dependencies))] = {}
                    else: arch['actual_process_samples'] = []
                    put(report,json.dumps(bad));put(arch_path,json.dumps(arch))
                    with self.subTest(change=change),self.assertRaises(RuntimeError): native.qualify_base(report,manifest,source,workspace)

    def test_assembly_never_combines_different_full_source_manifests(self):
        probes = {name:{'path':'/synthetic/'+name} for name in native.native_names() if not name.endswith('-gateway')}
        base = {'targets':probes,'methods':{name:{'source_dependencies':{}} for name in probes},
                'source_manifest_binding':{'sha256':'a'*64}}
        targeted = {version:{'target':f'uwvm-ros{version}-gateway','binary':{'path':'/synthetic/gateway-'+str(version)},
                    'source_dependency_method':'ACTUAL_MD','source_dependencies':{},'report':{'sha256':'b'*64},
                    'scope':'Synthetic model only','source_manifest_binding':{'sha256':'a'*64}} for version in (1,2)}
        self.assertEqual(len(native.assemble(base,targeted)[0]),18)
        targeted[2]['source_manifest_binding']['sha256'] = 'c'*64
        with self.assertRaises(RuntimeError): native.assemble(base,targeted)


if __name__ == '__main__': unittest.main()
