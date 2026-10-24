#!/usr/bin/env python3
"""Read-only independent validation of the completed four-profile ROS run.

Remote: --workspace W --baseline W6 --alias /u19 --reuse-worker W1
        --reuse-native W2
        [--overlay INPUTS]. The original failed aggregate remains failed.
Local: --report exported-report.json --verification remote-verification.json
       --local-root REPO. Local checks cover compiled ROS production paths and
the scoped ROS test/example overlay, without claims about unrelated user edits.
Print the verification JSON to stdout; never mount, compile, run ROS, or modify
the original report, source, binaries, fixtures, or historical evidence.
"""
import argparse
import collections
import errno
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import sys

SCOPE='Four fresh Worker profiles, 18 fresh native targets and complete A15; expanded ROS2 parameter events and TF static updates'
RUNNER_SHA='97eb3002a63cdd3a25c3743e150b9bda64d7af3bb71c7feea072bfd655d07f0b'
ORIGINAL_RUNNER_SHA='26db70039301de64d05855e6396c0c4e80979f12a4ca8d9584a17dbf111ed588'
NATIVE_ORIGIN_RUNNER_SHA='91065720da465c6ed11f9568b6328c5cef084c1195d2708f231903f0e4820b06'
PROFILES=('none','ros1','ros2','dual')
SUITES=('cgroup','unit','cli','regression','native','resources','network','five-module',
        'additional-security','ingress-trap','revoke-race','flood','payload-security','performance','cgroup-after')
CG_FIELDS=('memory.max','memory.current','memory.peak','memory.events','cpu.max','pids.max','cpuset.cpus.effective')
OOM=('oom','oom_kill','oom_group_kill')


def digest(path):
    h=hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda:stream.read(1048576),b''):h.update(block)
    return h.hexdigest()


def read_json(path):
    return json.loads(Path(path).read_text())


def inventory(root):
    return {str(p.relative_to(root)):digest(p) for p in sorted(Path(root).rglob('*')) if p.is_file()}


def make_words(text):
    words,current,at=[],[],0
    while at<len(text):
        value=text[at]
        if value=='\\' and at+1<len(text):at+=1;current.append(text[at])
        elif value=='#':break
        elif value.isspace():
            if current:words.append(''.join(current));current=[]
        elif value=='$' and at+1<len(text) and text[at+1]=='$':current.append('$');at+=1
        else:current.append(value)
        at+=1
    if current:words.append(''.join(current))
    return words


def prerequisites(path):
    text=Path(path).read_text().replace('\\\r\n',' ').replace('\\\n',' ')
    result=[]
    for line in text.splitlines():
        escaped=False
        for at,value in enumerate(line):
            if escaped:escaped=False
            elif value=='\\':escaped=True
            elif value==':':result.extend(make_words(line[at+1:]));break
    return result


def production(name):
    return name.startswith(('src/ros/gateway/','sdk/','src/uwvm2/imported/ros/','src/uwvm2/uwvm/imported/ros/'))


def scoped_overlay(name):
    return production(name) or name.startswith(('test/0073.ros-tests/support/source/ros','examples/ros1/','examples/ros2/'))


class Verification:
    def __init__(self):
        self.count=0;self.groups=collections.Counter();self.section='schema';self.files={}
        self.result={'status':'FAIL','scope':SCOPE,'executed_verifier_sha256':digest(Path(__file__))}

    def check(self,value,message):
        self.count+=1;self.groups[self.section]+=1
        if not value:raise RuntimeError(message)

    def json(self,path):
        path=Path(path);self.check(path.is_file(),'missing retained JSON: '+str(path))
        self.files[str(path)]=digest(path);return read_json(path)

    def relative(self,name):
        self.check(isinstance(name,str) and not Path(name).is_absolute() and '..' not in Path(name).parts and posixpath.normpath(name)==name,
                   'unsafe project-relative path: '+str(name))
        return name

    def sha(self,value):
        self.check(isinstance(value,str) and re.fullmatch('[0-9a-f]{64}',value) is not None,'invalid SHA256 value')

    def events(self,text):
        self.check(isinstance(text,str),'missing readable memory.events')
        words=[line.split() for line in text.splitlines()]
        self.check(all(len(row)==2 and row[1].isdigit() for row in words) and len({row[0] for row in words})==len(words),'malformed memory.events')
        result={key:int(value) for key,value in words};self.check(all(key in result for key in OOM),'incomplete OOM counters');return result

    def cgroup_pair(self,before,after,label,membership):
        self.section='cgroup';check=self.check
        for snapshot in (before,after):
            check(snapshot['status']=='PASS' and snapshot['membership']==membership,'cgroup status/membership differs: '+label)
            check(snapshot['effective_memory_max']==snapshot['expected_memory_max']==68719476736,'effective 64 GiB limit differs: '+label)
            check(type(snapshot['pid']) is int and snapshot['pid']>0,'invalid historical cgroup PID: '+label)
            check(isinstance(snapshot['cgroup_ancestors'],list) and bool(snapshot['cgroup_ancestors']),'empty ancestor proof: '+label)
        first,last=before['cgroup_ancestors'],after['cgroup_ancestors']
        check(len(first)==len(last),'ancestor count changed: '+label)
        check([x['path'] for x in first]==[x['path'] for x in last],'ancestor paths changed: '+label)
        check(first[-1]['path']=='/sys/fs/cgroup','ancestor proof omits the unified cgroup root: '+label)
        finite=[];deltas=[]
        for at,(old,new) in enumerate(zip(first,last)):
            check(Path(old['path']).is_absolute() and str(Path(old['path'])).startswith('/sys/fs/cgroup'),'invalid ancestor path')
            if at:check(Path(old['path'])==Path(first[at-1]['path']).parent,'noncontiguous ancestor chain')
            for field in CG_FIELDS:
                check(field in old and field in new,'missing cgroup field: '+field)
                check((old[field] is None)==(new[field] is None),'cgroup field readable/missing state changed: '+field)
                check(old[field] is None or isinstance(old[field],str) and isinstance(new[field],str),'invalid cgroup field schema: '+field)
            check(old['memory.max']==new['memory.max'],'recorded ancestor memory limit changed')
            if old['memory.max'] not in (None,'max'):
                check(old['memory.max'].isdigit(),'invalid finite ancestor limit');finite.append(int(old['memory.max']))
            if old['memory.events'] is not None:
                a,b=self.events(old['memory.events']),self.events(new['memory.events'])
                delta={key:b[key]-a[key] for key in OOM};check(not any(delta.values()),'OOM changed at readable ancestor '+old['path'])
                deltas.append({'path':old['path'],**delta})
        check(finite and min(finite)==68719476736,'independent recorded effective limit differs')
        expected='/sys/fs/cgroup'+membership[0].split(':',2)[2]
        check(first[0]['path']==expected,'actual ancestor does not match unified membership')
        return deltas

    def local(self,args,report):
        self.section='local-scoped-source';check=self.check;root=args.local_root.resolve()
        proof=self.json(args.verification)
        check(proof['status']=='PASS' and proof['scope']==SCOPE,'remote independent verification is not PASS')
        check(proof['source_report_sha256']==digest(args.report),'remote proof binds a different original report')
        check(proof['executed_verifier_sha256']==digest(Path(__file__)),'remote/local verifier bytes differ')
        expected={name:value for name,value in proof['compiled_project_inputs'].items() if production(name)}
        expected.update({name:value for name,value in report['overlay_inputs'].items() if scoped_overlay(name)})
        check(bool(expected),'empty scoped local ROS input set')
        for name,value in expected.items():
            self.relative(name);self.sha(value);check(digest(root/name)==value,'current scoped ROS source differs: '+name)
        self.result.update(mode='Current scoped ROS production and test/example overlay only; no claim about unrelated user edits or prior deliveries',
                           current_scoped_ros_inputs=expected,scoped_files=len(expected),
                           excluded_overlay_paths=sorted(set(report['overlay_inputs'])-set(expected)))

    def remote(self,args,report):
        check=self.check;work=args.workspace.resolve();base=args.baseline.resolve();source=work/'source';reports=work/'reports';alias=str(args.alias)
        check(sys.platform=='linux','remote mode requires the supplied Linux environment')
        check(re.fullmatch('/[a-z][0-9]+',alias) is not None,'invalid short alias')
        frozen=report['candidate_source_inputs'];patches=report['overlay_inputs'];before=report['cgroup_before'];after=report['cgroup_after'];membership=before['membership']
        check(len(membership)==1 and membership[0].startswith('0::'),'unified cgroup membership schema differs')
        self.section='frozen-inputs'
        check(inventory(base/'source')==report['baseline_source_inputs'],'W6 source changed')
        check(len(report['baseline_binary_inputs'])==18 and inventory(base/'native-binaries')==report['baseline_binary_inputs'],'W6 18 native binaries changed')
        check(digest(base/'reports/report.json')==report['baseline_report_sha256'],'W6 original report changed')
        check(report['baseline_preservation']=={'source':True,'18_native_binaries':True,'report':True},'final W6 preservation guard did not pass')
        check(frozen=={**report['baseline_source_inputs'],**patches},'candidate is not exact W6 plus explicit overlay')
        check(self.json(reports/'source-manifest.json')==frozen,'frozen source manifest differs')
        check(report['executed_driver_sha256']==RUNNER_SHA==frozen['test/0073.ros-tests/support/source/ros/run_ros2_events.py']==digest(source/'test/0073.ros-tests/support/source/ros/run_ros2_events.py'),'executed runner bytes differ from the reviewed frozen runner')
        if args.overlay:check(self.json(args.overlay/'manifest.json')==patches,'reviewed overlay manifest differs')
        for name,value in patches.items():
            self.relative(name);self.sha(value);check(digest(source/name)==value,'candidate overlay bytes changed: '+name)
            if args.overlay:check(digest(args.overlay/name)==value,'reviewed overlay bytes differ: '+name)
        check(report['compiled_inputs_unchanged'] is True,'runner compiled-input immutability check missing')

        def actual(path):
            text=posixpath.normpath(str(path))
            if text==alias or text.startswith(alias+'/'):text=str(work)+text[len(alias):]
            for distro in ('noetic','jazzy'):
                root=str(work/'gateway'/(distro+'-rootfs'))+'/'
                if text.startswith(root+'tmp/'):return work/'native-temp'/distro/text[len(root+'tmp/'):]
                if text.startswith(root):return args.sdk_rootfs_workspace/'gateway'/(distro+'-rootfs')/text[len(root):]
            return Path(text)

        self.section='worker-build';product=self.json(work/'product/results.json');reuse=None;worker_origin=work;original_worker_command=None;original_alias=None
        check(product==report['worker_build'] and product['status']=='PASS','original Worker build result differs')
        check([row['profile'] for row in product['profiles']]==list(PROFILES),'four exact fresh profiles missing')
        if 'worker_reuse' in report:
            reuse=report['worker_reuse'];check(args.reuse_worker is not None,'explicit original Worker workspace is required for reuse verification')
            worker_origin=args.reuse_worker.resolve();check(worker_origin!=work and str(worker_origin)==reuse['workspace'],'Worker reuse source workspace differs')
            original=self.json(worker_origin/'reports/report.json')
            check(digest(worker_origin/'reports/report.json')==reuse['original_report_sha256'],'original failed aggregate report changed')
            check(original['status']==reuse['original_aggregate_status']=='FAIL' and original['executed_driver_sha256']==ORIGINAL_RUNNER_SHA,'original aggregate failure or runner identity was reclassified')
            check(original['scope']==SCOPE and reuse['worker_build_status']=='PASS' and reuse['scope']=='Four fresh builds from this continuation; unchanged source inputs and ELF bytes verified','Worker reuse scope differs')
            check(self.json(worker_origin/'product/results.json')==product==original['worker_build'],'reused four-profile build report differs from original successful stage')
            check(reuse['source_root']==str(worker_origin/'product/source') and reuse['source_manifest_sha256']==digest(worker_origin/'product/source-manifest.json'),'Worker reuse manifest/root binding differs')
            original_manifest=self.json(worker_origin/'product/source-manifest.json')
            check(original_manifest==self.json(work/'product/source-manifest.json') and inventory(worker_origin/'product/source')==original_manifest==inventory(work/'product/source'),'complete original/copied Worker source snapshot differs')
            for name,value in original_manifest.items():
                self.relative(name);check(value==frozen[name]==digest(source/name)==digest(worker_origin/'source'/name),'complete original Worker input differs from candidate: '+name)
            check(digest(worker_origin/'product/build-budget.json')==digest(work/'product/build-budget.json'),'reused compiler budget bytes differ')
            rows=[row for row in original['commands'] if row['name']=='four-worker-build'];check(len(rows)==1,'original Worker stage command missing or duplicated')
            original_worker_command=rows[0]
            old_aliases=[row['root'] for row in original['mount_cleanup'] if re.fullmatch('/[a-z][0-9]+',row['root'])]
            if old_aliases:
                check(len(old_aliases)==1,'original attempt short alias identity duplicated');original_alias=old_aliases[0]
                if args.reuse_worker_alias is not None:check(original_alias==str(args.reuse_worker_alias),'explicit original Worker alias differs from cleanup evidence')
            else:
                check(args.reuse_worker_alias is not None and re.fullmatch('/[a-z][0-9]+',str(args.reuse_worker_alias)) is not None,'explicit uncreated original Worker alias is required')
                original_alias=str(args.reuse_worker_alias)
                check(not any(row['name']=='complete-a15' for row in original['commands']),'missing original alias cleanup after A15 execution')
                native_rows=[row for row in original['commands'] if row['name']=='all-native-build']
                check(len(native_rows)==1 and native_rows[0]['status']=='FAIL','uncreated original alias requires pre-A15 native failure')
                check(len(original['mount_cleanup'])==2 and {row['root'] for row in original['mount_cleanup']}=={str(worker_origin/'gateway/noetic-rootfs'),str(worker_origin/'gateway/jazzy-rootfs')},'pre-alias cleanup roots differ')
                check(not any(line.split(' - ',1)[0].split()[4]==original_alias or line.split(' - ',1)[0].split()[4].startswith(original_alias+'/') for line in original['remaining_mounts']),'uncreated alias unexpectedly recorded mounted')
                self.result['original_worker_alias']={'path':original_alias,'created_in_original_attempt':False,'identity_source':'Explicit original invocation alias; failure occurred before the runner alias-creation/A15 stage'}
            check(original_alias!=alias,'original Worker alias collides with final alias')
            supplemental=self.json(worker_origin/'reports/supplemental-cleanup.json')
            check(supplemental['status']=='PASS' and supplemental['original_aggregate_report_unchanged'] is True and supplemental['unmount']['returncode']==0 and not supplemental['after_owned'] and not supplemental['remaining_mounts'],'original failed attempt supplemental cleanup did not actually pass')
            self.cgroup_pair(original['cgroup_before'],original['cgroup_after'],'original Worker reuse attempt',membership);self.section='worker-build'
            self.result['worker_reuse']={**reuse,'original_aggregate_retained_status':original['status'],'fresh_profiles_built_in_original_attempt':4,'profiles_rebuilt_in_final_attempt':0}
        else:check(args.reuse_worker is None,'unexpected requested Worker reuse without provenance')
        budget=self.json(work/'product/build-budget.json')
        check(budget['membership']==membership and budget['memory_source']=='live-cgroup-v2' and budget['memory_max']==68719476736 and budget['jobs'] in (1,2),'Worker compiler budget differs from provided cgroup')
        snap_manifest=self.json(work/'product/source-manifest.json');compiled={};worker={};compiler_pids=[]
        dependency_files=sorted((work/'product').glob('*/*.d'))
        check(len(dependency_files)==12,'expected exactly 12 actual Worker .d files')
        check(set(report['worker_compiled_dependencies'])=={str(p.relative_to(work)) for p in dependency_files},'Worker dependency-file identities differ')
        worker_deps={}
        for profile,row in zip(PROFILES,product['profiles']):
            dest=work/'product'/profile;binary=dest/'uwvm2-ros'
            check(row['status']=='PASS' and row['compile_returncodes']==[0,0,0] and row['link_returncode']==row['version_returncode']==0,'Worker build/version actually failed: '+profile)
            self.sha(row['binary_sha256']);check(binary.open('rb').read(4)==b'\x7fELF' and digest(binary)==row['binary_sha256'],'Worker artifact differs: '+profile);worker[profile]=row['binary_sha256']
            for name in ('runtime','main','host_api','link','version'):
                log=dest/(name+'.log');check(log.is_file(),'missing Worker build log: '+str(log));self.files[str(log)]=digest(log)
            if reuse:
                for name in ('uwvm2-ros','runtime.d','main.d','host_api.d','runtime.log','main.log','host_api.log','link.log','version.log','commands.json','compiler-processes.json'):
                    original_file=worker_origin/'product'/profile/name
                    check(digest(dest/name)==digest(original_file),'original/copied Worker artifact bytes differ: '+profile+'/'+name)
                    self.files[str(original_file)]=digest(original_file)
            commands=self.json(dest/'commands.json');builds=[command for command in commands if '-c' in command]
            check(len(commands)==4 and len(builds)==3,'Worker compile/link command count differs')
            units=set()
            for command in builds:
                check(all(flag in command for flag in ('-MMD','-std=c++26','-DUWVM_DISABLE_JIT')),'Worker compiler flags incomplete')
                check(('-DUWVM_IMPORT_ROS1' in command)==(profile in ('ros1','dual')) and ('-DUWVM_IMPORT_ROS2' in command)==(profile in ('ros2','dual')),'Worker profile feature defines differ')
                units.add(Path(command[command.index('-o')+1]).stem)
                check(Path(command[command.index('-c')+1]).is_relative_to(worker_origin/'product/source'),'Worker command source is outside the recorded original snapshot')
            check(units=={'main','runtime','host_api'},'Worker translation units differ')
            processes=self.json(dest/'compiler-processes.json')
            check(len(processes)==3 and {p['translation_unit'] for p in processes}==units,'Worker compiler PID count/units differ')
            for process in processes:
                check(type(process['pid']) is int and process['pid']>0 and process['membership']==membership,'Worker compiler historical membership differs');compiler_pids.append(process['pid'])
            for dep in sorted(dest.glob('*.d')):
                values={};prefix=str(worker_origin/'product/source')+'/'
                for token in prerequisites(dep):
                    if not token.startswith(prefix):continue
                    normalized=posixpath.normpath(token);check(normalized.startswith(prefix),'Worker prerequisite escapes snapshot')
                    name=self.relative(normalized[len(prefix):]);value=frozen[name]
                    check(snap_manifest[name]==value and digest(worker_origin/'product/source'/name)==value and digest(work/'product/source'/name)==value and digest(source/name)==value,'actual Worker compiler input differs: '+name)
                    values[name]=value;compiled[name]=value
                key=str(dep.relative_to(work));check(values,'empty Worker actual prerequisites: '+key)
                check(report['worker_compiled_dependencies'][key]=={'sha256':digest(dep),'inputs':values},'Worker MMD independent parse differs: '+key)
                worker_deps[key]={'sha256':digest(dep),'inputs':values}
        check(len(set(compiler_pids))==12,'Worker compiler PID records duplicated')

        self.section='native-build';native_dir=work/'gateway/final-next-r1-provenance';native_origin=work;original_native_command=None;native_alias=None
        native=self.json(native_dir/'gateway-frozen-manifest.json');build_report=self.json(native_dir/'build-report.json')
        check(native['status']=='FROZEN' and native['source_root']==str(source) and native['binaries']==report['native_binaries'],'native frozen manifest/status/root differs')
        check(build_report['status']=='PASS' and build_report['parallelism']==1 and len(build_report['builds'])==2,'native builds not two fresh serial PASS builds')
        native_reuse=report.get('native_reuse')
        if native_reuse:
            check(args.reuse_native is not None,'explicit original native workspace is required for reuse verification')
            native_origin=args.reuse_native.resolve();old_dir=native_origin/'gateway/final-next-r1-provenance'
            check(native_origin not in (work,worker_origin) and str(native_origin)==native_reuse['workspace'],'native reuse source workspace differs')
            original_native_aggregate=self.json(native_origin/'reports/report.json')
            check(digest(native_origin/'reports/report.json')==native_reuse['original_report_sha256'],'original failed native-origin aggregate changed')
            check(original_native_aggregate['status']==native_reuse['original_aggregate_status']=='FAIL' and original_native_aggregate['executed_driver_sha256']==NATIVE_ORIGIN_RUNNER_SHA,'native-origin aggregate failure or runner identity was reclassified')
            check(original_native_aggregate['scope']==SCOPE and native_reuse['native_build_status']=='PASS' and native_reuse['scope']=='18 fresh native builds from this continuation; unchanged inputs, original dependencies, installed package versions and ELF bytes verified','native reuse scope differs')
            original_native=self.json(old_dir/'gateway-frozen-manifest.json')
            check(digest(old_dir/'gateway-frozen-manifest.json')==native_reuse['original_manifest_sha256']==digest(native_dir/'gateway-reused-original-manifest.json'),'retained original native manifest bytes differ')
            check(self.json(native_dir/'gateway-reused-original-manifest.json')==original_native and original_native['binaries']==original_native_aggregate['native_binaries'],'native-origin indexed products differ')
            check(digest(old_dir/'build-report.json')==native_reuse['original_build_report_sha256']==digest(native_dir/'build-report.json') and self.json(old_dir/'build-report.json')==build_report,'original/copied native build report bytes differ')
            expected=json.loads(json.dumps(original_native));expected['source_root']=str(source)
            for key,entry in expected['binaries'].items():
                tag,name=key.split(':');version=int(tag[3:]);distro='noetic' if version==1 else 'jazzy'
                root=work/'gateway'/(distro+'-rootfs')/'tmp'
                entry['path']=str(root/(tag+'-next-r1-build')/name);entry['canonical_path']=str(root/(tag+'-build')/name)
            expected['reused_from']={'workspace':str(native_origin),'original_manifest_sha256':native_reuse['original_manifest_sha256']}
            check(native==expected,'materialized native manifest changes more than the declared paths/reuse annotation')
            check(inventory(native_dir/'source-inputs')==inventory(old_dir/'source-inputs'),'native compiler source archive copy differs')
            for name,entry in native['source_inventory'].items():check(digest(native_origin/'source'/name)==entry['sha256']==frozen[name],'native-origin actual source differs from candidate: '+name)
            rows=[row for row in original_native_aggregate['commands'] if row['name']=='all-native-build'];check(len(rows)==1,'original native build command missing or duplicated');original_native_command=rows[0]
            aliases=[row['root'] for row in original_native_aggregate['mount_cleanup'] if re.fullmatch('/[a-z][0-9]+',row['root'])]
            check(len(aliases)==1,'native-origin attempt alias identity missing');native_alias=aliases[0]
            check(len(original_native_aggregate['mount_cleanup'])==3 and all(len(row['attempts'])==1 and row['attempts'][0]['returncode']==0 for row in original_native_aggregate['mount_cleanup']) and not original_native_aggregate['remaining_mounts'] and not original_native_aggregate['remaining_owned_processes'],'native-origin failed attempt cleanup did not actually pass')
            self.cgroup_pair(original_native_aggregate['cgroup_before'],original_native_aggregate['cgroup_after'],'native-origin aggregate',membership);self.section='native-build'
            self.result['native_reuse']={**native_reuse,'original_aggregate_retained_status':'FAIL','fresh_native_targets_built_in_original_attempt':18,'native_targets_rebuilt_in_final_attempt':0}
        else:check(args.reuse_native is None,'unexpected requested native reuse without provenance')
        native_binaries=native['binaries'];check(len(native_binaries)==18,'expected all 18 fresh native targets')
        expected_names={':'.join((p.split('/')[0],Path(p).name)) for p in report['baseline_binary_inputs']}
        check(set(native_binaries)==expected_names,'native target identities differ from frozen W6 18 targets')
        native_union=set();dependencies={};native_dependency_files=0
        for version,distro in ((1,'noetic'),(2,'jazzy')):
            tag=f'ros{version}';build=work/'native-temp'/distro/(tag+'-next-r1-build');row=build_report['builds'][version-1]
            check(row['version']==version and row['status']=='PASS' and row['returncode']==0 and row['membership']==membership and type(row['pid']) is int and row['pid']>0,'native builder historical membership/exit differs')
            log=native_dir/(tag+'-build.log');check(log.is_file(),'missing native build log');self.files[str(log)]=digest(log)
            if native_reuse:check(digest(log)==digest(native_origin/'gateway/final-next-r1-provenance'/(tag+'-build.log')),'original/copied native build log bytes differ')
            chain=native['toolchains'][str(version)]
            check(re.search(r'\b'+('9' if version==1 else '13')+r'\.\d',chain['compiler_version']) is not None,'native GCC distribution differs')
            check(digest(native_dir/chain['packages_file'])==chain['packages_sha256'],'native pinned package inventory differs')
            if native_reuse:check(digest(native_dir/chain['packages_file'])==digest(native_origin/'gateway/final-next-r1-provenance'/chain['packages_file']),'original/copied native package inventory differs')
            for key,entry in chain['build_metadata'].items():
                check(digest(build/key)==entry['sha256']==digest(native_dir/entry['archive']),'native build metadata archive differs: '+key)
                if native_reuse:check(digest(native_origin/'native-temp'/distro/(tag+'-next-r1-build')/key)==entry['sha256']==digest(native_origin/'gateway/final-next-r1-provenance'/entry['archive']),'original native compiler/link metadata differs')
            files=sorted(set(build.rglob('*.o.d'))|set(build.rglob('depend.make')))
            check(bool(files),'actual native compiler prerequisite files missing')
            native_dependency_files+=len(files)
            if native_reuse:
                old_build=native_origin/'native-temp'/distro/(tag+'-next-r1-build')
                check({str(p.relative_to(build)) for p in files}=={str(p.relative_to(old_build)) for p in set(old_build.rglob('*.o.d'))|set(old_build.rglob('depend.make'))},'original/copied native dependency file identities differ')
            for dep in files:
                self.files[str(dep)]=digest(dep)
                if native_reuse:check(digest(dep)==digest(old_build/dep.relative_to(build)),'original/copied actual native MMD bytes differ')
                values={}
                for token in prerequisites(dep):
                    if not token.startswith('/work/'):continue
                    normalized=posixpath.normpath(token);check(normalized.startswith('/work/'),'native prerequisite escaped /work')
                    name=self.relative(normalized[6:]);value=frozen[name]
                    check(digest(source/name)==value,'actual native source input changed: '+name)
                    values[name]=value;compiled[name]=value;native_union.add(name)
                if values:dependencies[tag+':'+str(dep.relative_to(build))]=values
            for key,entry in native_binaries.items():
                if not key.startswith(tag+':'):continue
                self.sha(entry['sha256']);name=key.split(':',1)[1];built=build/name;snapshot=work/'native-binaries'/tag/name
                check(actual(entry['path'])==built and actual(entry['canonical_path']).resolve()==built.resolve(),'native product paths do not identify fresh build: '+key)
                check(built.open('rb').read(4)==b'\x7fELF' and digest(built)==digest(snapshot)==entry['sha256'] and built.stat().st_size==snapshot.stat().st_size==entry['bytes'],'fresh native ELF/snapshot differs: '+key)
                if native_reuse:check(digest(old_build/name)==digest(native_origin/'native-binaries'/tag/name)==entry['sha256'],'original/copied fresh native ELF bytes differ: '+key)
        check(native_dependency_files==32,'expected all 32 actual native MMD/depend.make files')
        check(dependencies==native['compiled_source_dependencies'],'actual native MMD/depend.make map differs from frozen compiler map')
        native_union.update({'src/ros/gateway/ros1/CMakeLists.txt','src/ros/gateway/ros2/CMakeLists.txt'})
        check(native_union==set(native['source_inventory']),'native source archive contains omitted or inferred prerequisites')
        for name,entry in native['source_inventory'].items():
            check(entry['sha256']==frozen[name]==digest(source/name)==digest(native_dir/'source-inputs'/name),'native immutable compiled source archive differs: '+name)
            check((source/name).stat().st_size==entry['bytes'],'native source archive byte count differs');compiled[name]=entry['sha256']
        self.cgroup_pair(build_report['cgroup_before'],build_report['cgroup_after'],'native-build',membership)

        self.section='command-proofs';commands=report['commands']
        expected_commands=([] if reuse else ['four-worker-build'])+([] if native_reuse else ['all-native-build'])+['complete-a15']
        check([row['name'] for row in commands]==expected_commands,'exact integrated command sequence differs')
        wrapper_pids=[]
        command_sources=[(row,reports) for row in commands]
        if reuse:command_sources.insert(0,(original_worker_command,worker_origin/'reports'))
        if native_reuse:command_sources.insert(1 if reuse else 0,(original_native_command,native_origin/'reports'))
        for row,command_root in command_sources:
            check(row['status']=='PASS' and row['returncode']==0 and row['seconds']>=0,'integrated command actually failed')
            log=command_root/(row['name']+'.log');check(digest(log)==row['log_sha256'],'integrated original log differs: '+row['name']);self.files[str(log)]=digest(log)
            proof=self.json(command_root/(row['name']+'-exec.json'))
            check(proof==row['pre_exec_live_PID'],'standalone pre-exec proof differs from embedded record')
            check(set(proof)=={'pid','membership','live_cgroup_procs','command'} and proof['pid']==row['wrapper_pid'] and type(proof['pid']) is int and proof['pid']>0,'pre-exec PID schema differs')
            check(proof['membership']==membership and proof['live_cgroup_procs'] is True and proof['command']==row['command'],'pre-exec command/live cgroup proof differs');wrapper_pids.append(proof['pid'])
        check(len(set(wrapper_pids))==3,'integrated wrapper PID reused')

        self.section='a15';acceptance=self.json(work/'acceptance-next/report.json')
        check(acceptance==report['acceptance'] and acceptance['status']=='PASS' and acceptance['product_sha256']==worker['dual'],'original A15/product binding differs')
        check([row['suite'] for row in acceptance['suites']]==list(SUITES),'complete A15 exact 15-suite sequence differs')
        child_reports={};suite_logs={};snapshot_checks=0
        for row in acceptance['suites']:
            check(row['status']=='PASS' and row['returncode']==0,'A15 suite actually failed: '+row['suite'])
            log=work/'acceptance-next'/(row['suite']+'.log');check(log.is_file() and 'PASS' in log.read_text(errors='replace'),'missing original A15 suite completion log')
            suite_logs[row['suite']]=digest(log);self.files[str(log)]=digest(log)
            check(isinstance(row['evidence'],list) and bool(row['evidence']),'A15 suite lacks original evidence references')
            for ref in row['evidence']:
                path=actual(ref['path']);check(path.resolve().is_relative_to(work),'A15 reference escapes candidate workspace')
                child=self.json(path);check(digest(path)==ref['sha256'] and child.get('status')==ref['status']=='PASS','A15 original referenced JSON/status differs: '+str(path))
                child_reports[str(path.relative_to(work))]=ref['sha256']
                for key in ('results','suites','checks','cases','tests'):
                    if key+'_count' in ref:check(isinstance(child.get(key),list) and len(child[key])==ref[key+'_count'],'A15 referenced result count differs')
                    if isinstance(child.get(key),list):
                        for case in child[key]:
                            if isinstance(case,dict) and 'status' in case:check(case['status']=='PASS','nested accepted suite contains failed case')
                for group in ('artifacts','binary_snapshots'):
                    for name,artifact in child.get(group,{}).items():
                        if not isinstance(artifact,dict) or 'snapshot' not in artifact:continue
                        snapshot=actual(artifact['snapshot']);check(digest(snapshot)==artifact['sha256'],'A15 immutable artifact snapshot differs: '+name);snapshot_checks+=1
                        if name=='uwvm':check(artifact['sha256']==worker['dual'],'A15 used a different Worker')
                        key=name.split(':',1)[0]+':'+Path(name.split(':',1)[1]).name if group=='binary_snapshots' else name.replace('/',':',1)
                        if key in native_binaries:check(artifact['sha256']==native_binaries[key]['sha256'],'A15 used a different native target: '+name)
                if 'cgroup_before' in child and 'cgroup_after' in child:
                    a,b=child['cgroup_before'],child['cgroup_after']
                    if 'cgroup_ancestors' in a:self.cgroup_pair(a,b,str(path),membership);self.section='a15'
                    else:
                        check(a['path']==b['path']==membership[0].split(':',2)[2] and min(a['limits'])==min(b['limits'])==68719476736,'nested A15 live cgroup differs')
                        a_events,b_events=self.events(a['memory.events']),self.events(b['memory.events'])
                        check(all(a_events[k]==b_events[k] for k in OOM),'nested A15 OOM counters increased')
        self.cgroup_pair(self.json(work/'acceptance-next/cgroup-before.json'),self.json(work/'acceptance-next/cgroup-after.json'),'A15',membership)

        self.section='historical-main-proofs';main_records=report['actual_main_live_PIDs'];check(bool(main_records),'no actual live main ELF evidence')
        unavailable={name:report.get(name,[]) for name in ('unavailable_process_samples','unavailable_cleanup_samples','unavailable_postflight_samples')}
        for name,records in unavailable.items():
            check(isinstance(records,list),'unavailable process sample collection schema differs')
            for record in records:
                expected={'pid','errno','operation','reason'} if name=='unavailable_process_samples' else {'pid','errno'}
                check(set(record)==expected and type(record['pid']) is int and record['pid']>0 and record['errno']==errno.EACCES,'unavailable /proc sample is malformed or treated as positive ELF proof')
                if name=='unavailable_process_samples':check(record['operation']=='live descendant sampling' and isinstance(record['reason'],str),'unavailable main sample reason differs')
        self.result['historical_unavailable_process_samples']=unavailable
        known={value:'worker:'+name for name,value in worker.items()};known.update({entry['sha256']:name for name,entry in native_binaries.items()})
        observed=[];pid_values=[];kinds=collections.Counter()
        for record in main_records:
            check(set(record)=={'pid','parent_pid','ancestry','exe_path','exe_device','exe_inode','exe_sha256','membership','live_cgroup_procs'},'actual main evidence schema differs')
            for field in ('pid','parent_pid','exe_device','exe_inode'):check(type(record[field]) is int and record[field]>0,'invalid actual main PID/device/inode')
            check(isinstance(record['exe_path'],str) and Path(record['exe_path']).is_absolute(),'invalid actual main ELF path schema')
            ancestry=record['ancestry'];check(isinstance(ancestry,list) and len(ancestry)>=2 and all(type(n) is int and n>0 for n in ancestry) and len(set(ancestry))==len(ancestry),'invalid actual main ancestry')
            check(ancestry[0] in wrapper_pids and ancestry[-1]==record['pid'] and ancestry[-2]==record['parent_pid'],'actual main PPid/ancestry binding differs')
            self.sha(record['exe_sha256']);check(record['membership']==membership and record['live_cgroup_procs'] is True,'actual main left supplied cgroup')
            value=record['exe_sha256'];path=actual(record['exe_path'])
            if value in known:kinds[known[value]]+=1;observed.append(value)
            else:
                check(path.is_file() and path.open('rb').read(4)==b'\x7fELF' and digest(path)==value,'unindexed actual unit/probe or SDK ELF has no retained exact artifact: '+str(path))
                self.files[str(path)]=value;kinds['additional-retained-test-or-SDK-ELF']+=1
            pid_values.append(record['pid'])
        check(len(set(pid_values))==len(pid_values),'actual main PID evidence duplicated')
        check(worker['dual'] in observed,'actual main did not execute exact fresh dual Worker')
        for version in ('ros1','ros2'):
            check(native_binaries[version+':uwvm-'+version+'-gateway']['sha256'] in observed,'actual main did not execute fresh native gateway: '+version)

        self.section='cgroup';deltas=self.cgroup_pair(before,after,'integration',membership)
        check(deltas==report['oom_deltas'],'recorded all-readable-ancestor OOM deltas differ')
        check(Path('/proc/self/cgroup').read_text().splitlines()==membership,'verifier not in original supplied cgroup')
        actual_cg=Path(before['cgroup_ancestors'][0]['path']);check(str(os.getpid()) in (actual_cg/'cgroup.procs').read_text().split(),'verifier live PID missing from supplied cgroup.procs')
        for row in after['cgroup_ancestors']:
            for field in CG_FIELDS:
                try:value=(Path(row['path'])/field).read_text().strip()
                except OSError:value=None
                check((value is None)==(row[field] is None),'current ancestor field readability differs')
                if field=='memory.max':check(value==row[field],'current ancestor memory limit differs')
                if field=='memory.events' and value is not None:
                    current=self.events(value);recorded=self.events(row[field]);check(all(current[k]==recorded[k] for k in OOM),'OOM changed after integration completion')

        self.section='cleanup';cleanup=report['mount_cleanup'];expected={alias,str(work/'gateway/noetic-rootfs'),str(work/'gateway/jazzy-rootfs')}
        check(len(cleanup)==3 and {row['root'] for row in cleanup}==expected,'three owned cleanup roots differ')
        for row in cleanup:
            attempts=row['attempts'];check(isinstance(attempts,list) and 1<=len(attempts)<=3,'unbounded cleanup attempt schema')
            for attempt in attempts:check(set(attempt)=={'returncode','stderr'} and type(attempt['returncode']) is int and isinstance(attempt['stderr'],str),'cleanup attempt schema differs')
            check(attempts[-1]['returncode']==0 and all(attempt['returncode']!=0 and 'target is busy' in attempt['stderr'] for attempt in attempts[:-1]),'cleanup masked a success or non-busy failure')
        check(not report['remaining_mounts'] and not report['remaining_owned_processes'],'runner recorded remaining owned resources')
        check(not Path(alias).exists(),'owned short alias was not removed')
        points=[line.split(' - ',1)[0].split()[4] for line in Path('/proc/self/mountinfo').read_text().splitlines()]
        owned_roots={str(work),alias}
        if reuse:owned_roots.update((str(worker_origin),original_alias));check(not Path(original_alias).exists(),'original failed attempt alias remains after supplemental cleanup')
        if native_reuse:owned_roots.update((str(native_origin),native_alias));check(not Path(native_alias).exists(),'native-origin failed attempt alias remains after cleanup')
        for prefix in owned_roots:check(not any(p==prefix or p.startswith(prefix+'/') for p in points),'kernel owned mount remains: '+prefix)
        owned=[];unreadable=[];examined=0
        for proc in Path('/proc').iterdir():
            if not proc.name.isdigit() or int(proc.name)==os.getpid():continue
            try:
                paths={name:os.readlink(proc/name) for name in ('exe','cwd','root')};examined+=1
                if any(value==prefix or value.startswith(prefix+'/') for value in paths.values() for prefix in owned_roots):owned.append({'pid':int(proc.name),**paths})
            except OSError as error:
                if error.errno not in (errno.ENOENT,errno.ESRCH):unreadable.append({'pid':int(proc.name),'errno':error.errno})
        check(not owned and not unreadable,'current kernel owned process postflight is not provable empty')
        self.result.update(mode='Read-only actual remote build, A15 evidence and kernel postflight',compiled_project_inputs=compiled,
            worker_profiles=worker,worker_dependency_files=12,compiler_PID_records=len(compiler_pids),native_targets=18,
            native_dependency_files=native_dependency_files,native_dependency_files_with_project_inputs=len(dependencies),native_compiled_source_files=len(native_union),a15_suites=15,
            a15_original_evidence=child_reports,a15_original_log_sha256=suite_logs,immutable_A15_snapshots=snapshot_checks,
            actual_main_PID_records=len(main_records),actual_main_artifact_kinds=dict(kinds),oom_deltas=deltas,
            current_kernel_postflight={'examined_processes':examined,'owned_processes':owned,'unreadable':unreadable,'owned_mounts':[]})


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace',type=Path)
    parser.add_argument('--baseline',type=Path)
    parser.add_argument('--alias',type=Path,default=Path('/u19'))
    parser.add_argument('--reuse-worker',type=Path,help='Explicit original workspace containing the four fresh successful Worker profile builds')
    parser.add_argument('--reuse-worker-alias',type=Path,help='Original attempted invocation alias; required if that attempt failed before creating its alias')
    parser.add_argument('--reuse-native',type=Path,help='Explicit original workspace containing the 18 fresh successful native target builds')
    parser.add_argument('--sdk-rootfs-workspace',type=Path,default=Path('/u8'),help='Retained original SDK rootfs workspace; only historical non-/tmp SDK ELF paths map here')
    parser.add_argument('--overlay',type=Path)
    parser.add_argument('--report',type=Path)
    parser.add_argument('--local-root',type=Path)
    parser.add_argument('--verification',type=Path)
    args=parser.parse_args()
    if args.local_root:
        if not args.report or not args.verification:parser.error('local mode needs the original exported report and remote verification')
    elif not args.workspace or not args.baseline:parser.error('remote mode needs workspace and frozen W6 baseline')
    if not args.report:args.report=args.workspace/'reports/report.json'
    verification=Verification()
    try:
        report=verification.json(args.report);verification.result['source_report_sha256']=digest(args.report)
        verification.check(report['status']=='PASS' and report['scope']==SCOPE,'actual completed integration scope/status is not PASS')
        verification.check(report['executed_driver_sha256']==RUNNER_SHA,'actual completed integration used another runner')
        if args.local_root:verification.local(args,report)
        else:verification.remote(args,report)
        verification.result['status']='PASS'
    except Exception as error:
        verification.result.update(error=str(error),error_type=type(error).__name__)
    verification.result.update(checks=verification.count,checks_by_category=dict(verification.groups),retained_file_sha256=verification.files)
    print(json.dumps(verification.result,indent=2)+'\n',end='')
    return 0 if verification.result['status']=='PASS' else 1


if __name__=='__main__':raise SystemExit(main())
