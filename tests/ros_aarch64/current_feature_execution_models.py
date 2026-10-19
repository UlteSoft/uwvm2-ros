#!/usr/bin/env python3
"""Pure metadata models for the parent's current ROS1/ROS2 behavior admission.

All process, cgroup, architecture and mapping records are explicitly synthetic.
Product files contain ordinary text; ``require_arm`` is mocked. No compiler,
native image, QEMU, ROS, Guest or cgroup operation is executed. These models
check rejection of incomplete, mismatched and substituted evidence only.
"""
import copy
import hashlib
import json
import os
from pathlib import Path
import tempfile
import errno
import unittest
from unittest.mock import patch

import run_all as acceptance
import run_unit
import runtime
from runtime import cgroup_oom_deltas


def fact(path):
    """Measure real synthetic-file identity and add explicitly fake ELF fields."""
    path = Path(path); state = path.stat()
    return {'path':str(path),'device':state.st_dev,'inode':state.st_ino,'bytes':state.st_size,
            'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),
            'mtime_ns':state.st_mtime_ns,'ctime_ns':state.st_ctime_ns,
            'elf_class':2,'elf_data':1,'e_machine':183}


class CurrentFeatureExecutionModels(unittest.TestCase):
    def test_opened_mapping_descriptor_models(self):
        """Synthetic kernel links exercise the maintained FD reader, not ROS.

        The fake maps device is deliberately unrelated to st_dev. Only the
        actual opened regular descriptor can establish the selected target.
        These are deterministic rejection models, never actual kernel proof.
        """
        with tempfile.TemporaryDirectory(prefix='opened-map-model-') as temporary:
            base=Path(temporary);image=base/'image';other=base/'unrelated'
            header=bytearray(64);header[:6]=b'\x7fELF\x02\x01';header[18:20]=(183).to_bytes(2,'little')
            image.write_bytes(header+b'SYNTHETIC_ONLY');other.write_bytes(b'unrelated DSO; do not hash')
            target=fact(image);original_target=copy.deepcopy(target);maps=base/'maps'
            raw='1000-2000 r-xp 00000000 ff:fe 999 synthetic\n2000-3000 r--p 0 ff:fd 998 other\n'
            maps.write_text(raw)
            host={'device':1,'inode':2,'bytes':64,'mtime_ns':1,'ctime_ns':1}
            identity={'pid':424242,'parent_pid':1,'starttime_ticks':123,'state':'T',
                      'membership':['0::/MODEL_ONLY'],'live_cgroup_procs':True,'host_identity':host}
            real_path,real_open,real_read=runtime.Path,os.open,os.read
            reader,writer=os.pipe();cache={};reads=[];opened={}
            class ModelProcPath:
                def __init__(self,path):self.path=path
                def __truediv__(self,part):return ModelProcPath(self.path/part)
                def __str__(self):return str(self.path)
                def __fspath__(self):return str(self.path)
                def open(self,*args,**kwargs):
                    self_path=maps if str(self.path)=='/proc/424242/maps' else self.path
                    return self_path.open(*args,**kwargs)
            def paths(*parts):
                path=real_path(*parts)
                return ModelProcPath(path) if str(path).startswith('/proc') else path
            def opening(path,flags,*args,**kwargs):
                name=str(path)
                if name.startswith('/proc/424242/map_files/'):
                    chosen=image if name.endswith('1000-2000') else other
                    fd=real_open(chosen,flags);opened[fd]=chosen;return fd
                return real_open(path,flags,*args,**kwargs)
            def reading(fd,size):
                reads.append(opened.get(fd));return real_read(fd,size)
            try:
                with patch.object(runtime,'Path',side_effect=paths),patch.object(runtime.os,'open',side_effect=opening), \
                        patch.object(runtime.os,'read',side_effect=reading),patch.object(runtime.os,'pidfd_open',
                        create=True,side_effect=lambda _:os.dup(reader)):
                    proof=runtime.opened_target_mappings(424242,{'target':target},lambda:copy.deepcopy(identity),cache)
                    sample={**identity,'kernel_exe_elf':host,'opened_target_mappings':proof}
                    self.assertTrue(runtime.validate_mapping_receipts(sample,'target',target))
                    self.assertTrue(proof['targets']['target'])
                    self.assertNotIn(other,reads)  # Fstat only for every non-target DSO.
                    reads.clear()
                    again=runtime.opened_target_mappings(424242,{'target':target},lambda:copy.deepcopy(identity),cache)
                    self.assertEqual(again['targets']['target'][0]['hash_method'],'RETAINED_HASHED_DESCRIPTOR')
                    self.assertNotIn(image,reads)  # The initially hashed FD is still retained.
                    # Pool hardlinks change ctime without changing an admitted
                    # ELF target body. This invalidates the FD hash cache; it
                    # must perform a new full hash, never relax bytes/SHA.
                    os.link(image,base/'pool-alias')
                    # Force a distinct metadata tuple even on a coarse-clock
                    # temporary filesystem; this does not alter file bytes.
                    state=image.stat();os.utime(image,ns=(state.st_atime_ns,state.st_mtime_ns+1000000000))
                    unstamped={key:value for key,value in target.items() if key not in ('mtime_ns','ctime_ns')}
                    refreshed=runtime.opened_target_mappings(424242,{'target':unstamped},lambda:copy.deepcopy(identity),cache)
                    self.assertEqual(refreshed['targets']['target'][0]['hash_method'],'FULL_OPENED_DESCRIPTOR_HASH')
                    target=fact(image)
                    wrong=dict(target,sha256='0'*64)
                    with self.assertRaises(RuntimeError):
                        runtime.opened_target_mappings(424242,{'target':wrong},lambda:copy.deepcopy(identity),cache)
                    with patch.object(runtime.os,'open',side_effect=PermissionError(errno.EACCES,'MODELED')):
                        with self.assertRaises(PermissionError):
                            runtime.opened_target_mappings(424242,{'target':target},lambda:copy.deepcopy(identity),cache)
                    calls=[0]
                    def changed():
                        calls[0]+=1;return dict(identity,starttime_ticks=123 if calls[0]==1 else 124)
                    with self.assertRaises(RuntimeError):
                        runtime.opened_target_mappings(424242,{'target':target},changed,cache)
                    for control in ('raw-only','physical','hash','birth','host','initial-hash'):
                        bad=copy.deepcopy(sample)
                        receipt=bad['opened_target_mappings'];row=receipt['targets']['target'][0]
                        if control=='raw-only':bad.pop('opened_target_mappings')
                        elif control=='physical':row['opened_file']['inode']+=1
                        elif control=='hash':row['opened_file']['sha256']='1'*64
                        elif control=='birth':receipt['identity_after']['starttime_ticks']+=1
                        elif control=='host':receipt['identity_after']['host_identity']['inode']+=1
                        else:row['initial_full_hash']['opened_file']['bytes']+=1
                        self.assertFalse(runtime.validate_mapping_receipts(bad,'target',original_target),control)
                    image.write_bytes(header+b'CHANGED_TARGET')
                    with self.assertRaises(RuntimeError):
                        # Existing original full facts cannot admit the changed file.
                        runtime.opened_target_mappings(424242,{'target':fact(image)},lambda:copy.deepcopy(identity),
                            {tuple(fact(image)[key] for key in runtime._MAP_FACTS):next(iter(cache.values()))})
            finally:
                for item in cache.values():os.close(item['fd'])
                os.close(reader);os.close(writer)

    def test_all_thread_children_and_recursive_rss_models(self):
        """A synthetic non-main thread's forked child contributes to RSS."""
        with tempfile.TemporaryDirectory(prefix='all-thread-procfs-model-') as temporary:
            base = Path(temporary).resolve()
            for pid,rss,threads in ((20,100,{'20':'21','29':'22 21','28':None}),
                                    (21,20,{'21':''}),(22,30,{'22':'23'}),(23,40,{'23':''})):
                directory = base/str(pid);directory.mkdir();(directory/'status').write_text('VmRSS: '+str(rss)+' kB\n')
                for tid,children in threads.items():
                    task = directory/'task'/tid;task.mkdir(parents=True)
                    if children is not None:(task/'children').write_text(children)
            def redirect(value):
                return base if str(value)=='/proc' else Path(value)
            with patch.object(runtime,'Path',side_effect=redirect),patch.object(run_unit,'Path',side_effect=redirect):
                self.assertEqual(runtime.process_child_pids(20),[21,22])
                self.assertEqual(run_unit.process_tree_rss(20),190*1024)
                (base/'20/task/29/children').write_text('malformed-child-id')
                with self.assertRaises(ValueError):runtime.process_child_pids(20)

    def test_fresh_worker_objects_helper_and_original_cgroup_counters(self):
        """Synthetic raw dependency records cannot qualify a replaced object."""
        with tempfile.TemporaryDirectory(prefix='fresh-worker-admission-model-') as temporary:
            base = Path(temporary).resolve();source = base/'source';source.mkdir()
            units = {'runtime':'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp',
                     'main':'src/uwvm2/uwvm/main.default.cpp','host_api':'src/uwvm2/uwvm/host_api.default.cpp'}
            authority = {'src/uwvm2/imported/wasi/wasip1/environment/environment.h',
                'src/uwvm2/uwvm/imported/wasi/wasip1/storage/env.h',
                'src/uwvm2/uwvm/cmdline/params/impl.h','src/uwvm2/uwvm/cmdline/params/wasip1_permissions.h',
                'src/uwvm2/uwvm/cmdline/callback/impl.h','src/uwvm2/uwvm/cmdline/callback/wasip1_permissions.h'}
            frozen = {}
            for relative in set(units.values())|authority:
                path = source/relative;path.parent.mkdir(parents=True,exist_ok=True);path.write_text('Synthetic source '+relative+'\n')
                frozen[relative] = {'sha256':acceptance.digest(path),'bytes':path.stat().st_size}
            identity = {'path':'/synthetic-48gib','device':29,'inode':1,'boot_id':'synthetic'}
            group = {'status':'PASS','effective_memory_max':68719476736,'membership':['0::/synthetic-48gib'],
                'cgroup_ancestors':[{'path':'/synthetic-48gib','memory.max':'68719476736',
                    'memory.events':'oom 0\noom_kill 0\noom_group_kill 0\n'}]}
            _,deltas = cgroup_oom_deltas(group,group,True)
            report = {'status':'PASS','source_manifest_sha256':'synthetic-source-manifest',
                'builder_sha256':acceptance.digest(Path(acceptance.__file__).with_name('build_worker.py')),
                'artifact_helper_sha256':acceptance.digest(Path(acceptance.__file__).with_name('runtime.py')),
                'binding_helper_sha256':acceptance.digest(Path(acceptance.__file__).with_name('gateway_qualification.py')),
                'driver_inputs_preserved':True,'same_cgroup_incarnation':True,'memory_swap_max_before':'0','memory_swap_max_after':'0',
                'cgroup_incarnation_before':identity,'cgroup_incarnation_after_before_probe':identity,'cgroup_incarnation_after':identity,
                'cgroup_before':copy.deepcopy(group),'cgroup_after':copy.deepcopy(group),'memory_events':deltas,'profiles':[]}
            report['tools_preserved'] = True
            for label in ('compiler','qemu'):
                tool = base/('synthetic-'+label);tool.write_text('Synthetic input '+label+'; never execute.\n')
                pin = acceptance.read_file_binding(tool)
                report[label] = {'argv':[str(tool)],'sha256':pin['sha256'],'input':pin}
            for profile in ('none','ros1','ros2','dual'):
                directory = base/profile;directory.mkdir();binary = directory/'uwvm';binary.write_text('Synthetic Worker '+profile+'\n')
                row = {'profile':profile,'status':'PASS','binary':fact(binary),'dependencies':{},'objects':{},'commands':[]}
                for unit,relative in units.items():
                    path = directory/(unit+'.o');path.write_text('Synthetic object '+unit+'\n');row['objects'][unit] = fact(path)
                    names = sorted({relative}|authority);dep = directory/(unit+'.d')
                    dep.write_text(str(path)+': '+' '.join(str(source/name) for name in names)+'\n')
                    row['dependencies'][unit] = {name:frozen[name]['sha256'] for name in names}
                    row['commands'].append({'returncode':0,'command':['synthetic-compiler','-MMD','-MF',str(dep),'-c',str(source/relative),'-o',str(path)]})
                row['commands'] += [{'returncode':0,'command':['synthetic-linker']},{'returncode':0,'command':['synthetic-QEMU-version']}]
                report['profiles'].append(row)
            with patch.object(acceptance,'require_arm',side_effect=fact):
                self.assertEqual(len(acceptance.validate_fresh_worker_inputs(report,source,frozen)),4)
                for control in ('old-helper','missing-helper','old-binding-helper','unpreserved','missing-object','changed-object','reused',
                                'changed-incarnation','missing-final-bracket','swap','oom','missing-counters','wrong-limit',
                                'unpreserved-tools','wrong-tool-sha'):
                    bad = copy.deepcopy(report)
                    if control=='old-helper':bad['artifact_helper_sha256'] = 'old'
                    elif control=='missing-helper':bad.pop('artifact_helper_sha256')
                    elif control=='old-binding-helper':bad['binding_helper_sha256'] = 'old'
                    elif control=='unpreserved':bad['driver_inputs_preserved'] = False
                    elif control=='missing-object':bad['profiles'][0]['objects'].pop('main')
                    elif control=='changed-object':bad['profiles'][0]['objects']['main']['inode'] += 1
                    elif control=='reused':bad['original_worker_report'] = 'historical'
                    elif control=='changed-incarnation':bad['cgroup_incarnation_after'] = {'inode':2}
                    elif control=='missing-final-bracket':bad.pop('cgroup_incarnation_after_before_probe')
                    elif control=='swap':bad['memory_swap_max_after'] = 'max'
                    elif control=='oom':bad['cgroup_after']['cgroup_ancestors'][0]['memory.events'] = 'oom 1\noom_kill 0\noom_group_kill 0\n'
                    elif control=='missing-counters':bad['cgroup_after']['cgroup_ancestors'][0]['memory.events'] = None
                    elif control=='unpreserved-tools':bad['tools_preserved'] = False
                    elif control=='wrong-tool-sha':bad['compiler']['sha256'] = 'old'
                    else:bad['cgroup_before']['effective_memory_max'] = 1
                    with self.subTest(control=control),self.assertRaises(RuntimeError):acceptance.validate_fresh_worker_inputs(bad,source,frozen)
                obj = Path(report['profiles'][0]['objects']['main']['path'])
                replacement = obj.with_suffix('.replacement');replacement.write_bytes(obj.read_bytes());replacement.replace(obj)
                with self.assertRaises(RuntimeError):acceptance.validate_fresh_worker_inputs(report,source,frozen)
                tool = Path(report['compiler']['argv'][0]);replacement = tool.with_suffix('.replacement')
                replacement.write_bytes(tool.read_bytes());replacement.replace(tool)
                with self.assertRaises(RuntimeError):acceptance.validate_fresh_worker_inputs(report,source,frozen)

    def fixture(self, directory, module):
        """Build an independently shaped positive record for each real inventory."""
        membership = ['0::/synthetic-existing-64gib-leaf']
        counters = 'oom 0\noom_kill 0\noom_group_kill 0\n'
        group = {'status':'PASS','effective_memory_max':68719476736,'membership':membership,
                 'cgroup_ancestors':[{'path':'/synthetic-existing-64gib-leaf',
                                     'memory.max':'68719476736','memory.events':counters}]}
        identity = {'path':'/synthetic-existing-64gib-leaf','device':29,'inode':123,'boot_id':'synthetic'}
        _,deltas = cgroup_oom_deltas(group,group,True)
        architecture = {'status':'PASS','same_cgroup_incarnation':True,'cgroup_before':copy.deepcopy(group),'cgroup_after':copy.deepcopy(group),
            'cgroup_incarnation_before':identity,'cgroup_incarnation_after_before_probe':identity,
            'cgroup_incarnation_after':identity,'cgroup_ancestor_event_deltas':deltas,
            'remaining_owned_processes':[],'fatal_errors':[],
            'arm_targets':{},'actual_process_samples':[],'expected_host_emulator_sha256':['synthetic-host-emulator']}
        gateway_path = directory/'private-gateway';gateway_path.write_text('Synthetic Gateway image; never execute.\n')
        gateway = fact(gateway_path);gateway_label = 'synthetic-gateway'
        architecture['arm_targets'][gateway_label] = gateway
        document = {'status':'PASS','mode':'execute','native_behavior_status':'PASS','actual_behavior_status':'PASS',
            'actual_native_behavior_probes':2,'source_manifest_sha256':'synthetic-source-manifest',
            'probe_contracts':copy.deepcopy(module.PROBE_CONTRACTS),'probes':{},'commands':[],
            'same_cgroup_incarnation':True,'cgroup_incarnation_before':identity,'cgroup_incarnation_after':identity,
            'memory_swap_max_before':'0','memory_swap_max_after':'0','remaining_owned_mounts':[],
            'remaining_owned_processes':[],'mount_cleanup':[{'returncode':0}],'gateway_snapshot':gateway}
        for name in ('source_preserved','sdk_preserved','driver_inputs_preserved',
                     'prepared_inputs_preserved','gateway_inputs_preserved','tools_preserved'):
            document[name] = True
        prepared = {'products':{}}

        def proof(row, label, target, pid, parent, birth, ancestry, launcher):
            actual = {'pid':pid,'parent_pid':parent,'starttime_ticks':birth,'state':'T'}
            mapping = '1000-2000 r-xp 00000000 '+format(os.major(target['device']),'x')+':'+format(os.minor(target['device']),'x')+' '+str(target['inode'])+' '+target['path']
            sample = {'name':row['name']+'/','pid':pid,'parent_pid':parent,'starttime_ticks':birth,
                'ancestry':ancestry,'membership':membership,'live_cgroup_procs':True,
                'mapped_arm_targets':[label],'target_mappings':[mapping],
                'kernel_exe_elf':{'e_machine':62,'sha256':'synthetic-host-emulator',
                    'device':1,'inode':2,'bytes':64,'mtime_ns':1,'ctime_ns':1},
                'execution_model':'QEMU user host process'}
            image={key:value for key,value in target.items() if key!='path'}
            guarded=dict(actual,membership=membership,live_cgroup_procs=True,
                         host_identity={'device':1,'inode':2,'bytes':64,'mtime_ns':1,'ctime_ns':1})
            sample['opened_target_mappings']={'format_version':1,'method':runtime.MAPPING_METHOD,
                'identity_before':copy.deepcopy(guarded),'identity_after':copy.deepcopy(guarded),
                'raw_maps':[mapping],'targets':{label:[{'range':'1000-2000',
                    'raw_mapping':mapping,'opened_file':image,'hash_method':'FULL_OPENED_DESCRIPTOR_HASH',
                    'initial_hash_monotonic_ns':1,'initial_full_hash':{'method':'FULL_OPENED_DESCRIPTOR_HASH',
                        'opened_file':copy.deepcopy(image),'capture_monotonic_ns':1}}]}}
            architecture['actual_process_samples'].append(sample)
            return {'actual_stopped_ARM':actual,'launcher':launcher,'ancestry':ancestry,
                    'target_proof':sample,'current_mapping_check':True}

        for index,(label,contract) in enumerate(module.PROBE_CONTRACTS.items()):
            path = directory/label;path.write_text('Synthetic fixture '+label+'. Never execute.\n')
            product = fact(path);document['probes'][label] = {'binary':product}
            prepared['products'][label] = copy.deepcopy(product)
            architecture['arm_targets'][label] = product
            pid,birth = 100+index*10,1000+index*10
            row = {'name':'behavior-'+label,'status':'PASS','returncode':0,'pid':pid,
                   'launcher_starttime_ticks':birth}
            launcher = {'pid':pid,'parent_pid':1,'starttime_ticks':birth,'state':'S'}
            row['actual_ARM_mapping_before_resume'] = proof(row,label,product,pid+1,pid,birth+1,[pid,pid+1],launcher)
            if contract.get('gateway_required',contract.get('requires_gateway',False)):
                row['actual_gateway_ARM_mapping_before_resume'] = proof(row,gateway_label,gateway,pid+2,pid+1,birth+2,[pid,pid+1,pid+2],launcher)
            marker = contract['marker']
            complete = marker+('19' if marker.endswith('checks=') else 'synthetic declared completion' if marker.endswith(' ') else '')
            log = directory/(row['name']+'.log');log.write_text(complete+'\n')
            row['log_sha256'] = hashlib.sha256(log.read_bytes()).hexdigest()
            document['commands'].append(row)
        nested = directory/'execution/architecture-evidence.json';nested.parent.mkdir()
        nested.write_text(json.dumps(architecture));document['architecture_evidence'] = {'sha256':acceptance.digest(nested)}
        return document,architecture,prepared,{'binary':gateway},nested

    def check(self, document, directory, module, prepared, gateway):
        return acceptance.validate_feature_execution(document,directory,'synthetic-source-manifest',module,prepared,gateway)

    def test_both_exact_feature_inventories_and_negative_controls(self):
        for module in acceptance.FEATURE_DRIVERS.values():
            with self.subTest(module=module.__name__), tempfile.TemporaryDirectory(prefix='feature-execution-admission-model-') as temporary:
                directory = Path(temporary).resolve()
                original,architecture,prepared,gateway,nested = self.fixture(directory,module)
                with patch.object(acceptance,'require_arm',side_effect=fact):
                    admitted = self.check(original,directory,module,prepared,gateway)
                    self.assertEqual(admitted['mandatory_behavior_probes'],2)
                    self.assertEqual(len(admitted['input_pins']),3)
                    for control in ('missing','duplicate','manifest','source','swap','incarnation','cleanup','mount',
                                    'failed','mapping','pid','birth','host','membership','live','ancestry','state',
                                    'map-inode','missing-map','unknown-sample','gateway-parent','gateway-proof','gateway-sha'):
                        bad = copy.deepcopy(original);row = bad['commands'][0]
                        proof = row['actual_ARM_mapping_before_resume'];sample = proof['target_proof']
                        if control=='missing':bad['commands'].pop()
                        elif control=='duplicate':bad['commands'][1] = copy.deepcopy(row)
                        elif control=='manifest':bad['source_manifest_sha256'] = 'old'
                        elif control=='source':bad['source_preserved'] = False
                        elif control=='swap':bad['memory_swap_max_after'] = 'max'
                        elif control=='incarnation':bad['cgroup_incarnation_after'] = {'inode':456}
                        elif control=='cleanup':bad['remaining_owned_processes'] = [999]
                        elif control=='mount':bad['mount_cleanup'][0]['returncode'] = 1
                        elif control=='failed':row['returncode'] = 1
                        elif control=='mapping':proof['current_mapping_check'] = False
                        elif control=='pid':proof['actual_stopped_ARM']['pid'] += 1
                        elif control=='birth':row['launcher_starttime_ticks'] += 1
                        elif control=='host':sample['kernel_exe_elf']['sha256'] = 'unknown'
                        elif control=='membership':sample['membership'] = ['0::/other']
                        elif control=='live':sample['live_cgroup_procs'] = False
                        elif control=='ancestry':proof['ancestry'] = [999]
                        elif control=='state':proof['actual_stopped_ARM']['state'] = 'R'
                        elif control=='map-inode':sample['opened_target_mappings']['targets'][sample['mapped_arm_targets'][0]][0]['opened_file']['inode'] += 1
                        elif control=='missing-map':sample.pop('opened_target_mappings')
                        elif control=='unknown-sample':sample['capture_monotonic_ns'] = 999
                        elif control=='gateway-sha':bad['gateway_snapshot']['sha256'] = 'old'
                        else:
                            gateway_row = next(item for item in bad['commands'] if 'actual_gateway_ARM_mapping_before_resume' in item)
                            gw = gateway_row['actual_gateway_ARM_mapping_before_resume']
                            if control=='gateway-parent':gw['actual_stopped_ARM']['parent_pid'] = 999
                            else:gw['current_mapping_check'] = False
                        with self.subTest(control=control),self.assertRaises(RuntimeError):
                            self.check(bad,directory,module,prepared,gateway)
                    # Corrupt both the row and its nested sample consistently.
                    # These controls reach the ancestry/birth checks instead of
                    # merely failing an unequal embedded-sample comparison.
                    for control in ('fixture-before-launch','root-wrong-birth','gateway-before-fixture'):
                        bad = copy.deepcopy(original);bad_arch = copy.deepcopy(architecture)
                        row = bad['commands'][0]
                        if control=='gateway-before-fixture':
                            row = next(item for item in bad['commands'] if 'actual_gateway_ARM_mapping_before_resume' in item)
                            proof = row['actual_gateway_ARM_mapping_before_resume']
                            birth = row['actual_ARM_mapping_before_resume']['actual_stopped_ARM']['starttime_ticks']-1
                        else:
                            proof = row['actual_ARM_mapping_before_resume'];birth = row['launcher_starttime_ticks']-1
                        old_sample = copy.deepcopy(proof['target_proof'])
                        actual,sample = proof['actual_stopped_ARM'],proof['target_proof']
                        actual['starttime_ticks'] = sample['starttime_ticks'] = birth
                        if control=='root-wrong-birth':
                            actual['pid'] = sample['pid'] = row['pid']
                            actual['parent_pid'] = sample['parent_pid'] = proof['launcher']['parent_pid']
                            proof['ancestry'] = sample['ancestry'] = [row['pid']]
                        index = bad_arch['actual_process_samples'].index(old_sample)
                        bad_arch['actual_process_samples'][index] = copy.deepcopy(sample)
                        nested.write_text(json.dumps(bad_arch));bad['architecture_evidence']['sha256'] = acceptance.digest(nested)
                        with self.subTest(control=control),self.assertRaises(RuntimeError):self.check(bad,directory,module,prepared,gateway)
                    for control in ('oom','nested-incarnation','nested-missing-counter','nested-memory-limit'):
                        bad_arch = copy.deepcopy(architecture)
                        if control=='oom':bad_arch['cgroup_after']['cgroup_ancestors'][0]['memory.events'] = 'oom 1\noom_kill 0\noom_group_kill 0\n'
                        elif control=='nested-incarnation':bad_arch['cgroup_incarnation_after'] = {'inode':456}
                        elif control=='nested-missing-counter':bad_arch['cgroup_after']['cgroup_ancestors'][0]['memory.events'] = None
                        else:bad_arch['cgroup_after']['effective_memory_max'] = 1
                        nested.write_text(json.dumps(bad_arch));bad = copy.deepcopy(original)
                        bad['architecture_evidence']['sha256'] = acceptance.digest(nested)
                        with self.subTest(control=control),self.assertRaises(RuntimeError):self.check(bad,directory,module,prepared,gateway)
                    nested.write_text(json.dumps(architecture));original['architecture_evidence']['sha256'] = acceptance.digest(nested)
                    row = original['commands'][0];log = directory/(row['name']+'.log');text = log.read_text()
                    for payload in ('unrelated PASS\n',text+text,text+'FAIL synthetic\n'):
                        log.write_text(payload);bad = copy.deepcopy(original);bad['commands'][0]['log_sha256'] = acceptance.digest(log)
                        with self.assertRaises(RuntimeError):self.check(bad,directory,module,prepared,gateway)
                    log.write_text(text)
                    product = Path(next(iter(original['probes'].values()))['binary']['path'])
                    replacement = product.with_suffix('.replacement');replacement.write_bytes(product.read_bytes());replacement.replace(product)
                    with self.assertRaises(RuntimeError):self.check(original,directory,module,prepared,gateway)


if __name__=='__main__':
    unittest.main()
