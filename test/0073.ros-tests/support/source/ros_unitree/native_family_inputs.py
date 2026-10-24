"""A2: a terminal copied GO24 input, independent of Typed14's main build.

Only the genuine terminal C52 package/export record is an original build root.
The later Native6 failure is never upgraded. A2 reopens the original complete
source, every original selected input root, execution SDK and GO derived tree,
then reparses actual full-MD/Make/link/map evidence. Exclusive new-inode copies
carry forward just the consumed catalog, definitions, generated provider bytes
and unchanged proof documents. A2 executes no compiler, exporter or Guest.

B accepts A2 only after its normal Frame.finish preservation and cleanup. Its
old roots remain observations made by A2; B does not open unused historical
host/SDK roots or pretend to have observed them again. B really runs the current
emitter and requires all eleven message-only outputs to be byte-identical.
"""
from __future__ import annotations
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import shlex
import types

MIB=1024**2
PIN=frozenset(('path','bytes','sha256','device','inode','mtime_ns','ctime_ns'))
KIND='uwvm-independent-native-go24-library-input-v1'
STATUS='PASS_INDEPENDENT_NATIVE_GO24_LIBRARY_INPUT_COPIED_AND_PRESERVED_NOT_EXECUTED'
ORIGINAL_SHA='c52bc5ed98e52904f52e1f8dac7031cd65a53f94d5128375d8de2eea23ae4d73'
ORIGINAL_BYTES=40555414
# Actual original pin retained by the genuine Native6/C52 export observation.
# This is an immutable historical role, not a prediction of a future device.
ORIGINAL_PIN={'path':'/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/unitree-native-humble-build-export-20261016-r6/report.json',
    'bytes':40555414,'sha256':ORIGINAL_SHA,'device':68,'inode':326925,
    'mtime_ns':1792099813587285821,'ctime_ns':1792099813604286079}
OUTPUTS=frozenset(('bounded_records.h','bounded_records.hpp','bounded_records.rs','bounded_records.go',
    'bounded_records.ts','bounded_records.mjs','bounded_interfaces.h','bounded_interfaces.hpp',
    'manifest.hpp','native_ros1.hpp','native_ros2.hpp'))
COMMIT='668d1ec5a05d1c38d3306bdca7d59f2ba3581a88'


def need(value,reason):
    if not value:raise ValueError(reason)


def terminal_original(report,pin):
    """The original success is fixed actual data, never an inferred status."""
    need(pin['bytes']==ORIGINAL_BYTES and pin['sha256']==ORIGINAL_SHA and
        report.get('kind')=='unitree-native-amd64-package-export' and
        report.get('status')=='PREPARED_NATIVE_INTERFACES_IDENTITIES_AND_SOURCE_PROVIDERS' and
        all(report.get(key) is True for key in ('source_preserved','inputs_preserved','products_preserved',
            'execution_sdk_preserved','derived_products_preserved','preparation_complete')) and
        report.get('reuse') is False and report.get('runtime_or_robot_qualification') is False and
        report.get('cleanup')=={'status':'PASS','remaining':[]} and
        report.get('mount_cleanup_processes')=={'status':'PASS','remaining':[]} and
        report.get('remaining_mounts')==[] and report.get('finalization_errors')==[],
        'genuine terminal C52 native-interface preparation and cleanup required')
    commands=report['commands']
    need(type(commands) is list and len(commands)==40 and len({row['label'] for row in commands})==40 and
        all(row['status']=='PASS' and row['returncode']==0 and row['expected_returncode']==0 for row in commands),
        'all forty original actual commands must have completed successfully')
    families=report['families'];need(type(families) is list and
        {row['family'] for row in families}=={'unitree_go','unitree_hg','unitree_api'} and len(families)==3 and
        all(row['status']=='PREPARED_NATIVE_PACKAGE_IDENTITIES_AND_SOURCE_PROVIDER' for row in families),
        'the unchanged three-family successful original report is required')
    return next(row for row in families if row['family']=='unitree_go')


def without_dependencies(argv):
    """Remove only actual -MD/-MF/-MT for compile-database bijection."""
    result=[];at=0
    while at<len(argv):
        token=argv[at]
        if token=='-MD':at+=1;continue
        if token in ('-MF','-MT'):
            need(at+1<len(argv) and argv[at+1] and not argv[at+1].startswith('-'),'actual dependency operand required')
            at+=2;continue
        need(not token.startswith(('-MF','-MT')),'joined dependency options outside original scope')
        result.append(token);at+=1
    return tuple(result)


def make_witness(database,log,logical_build,expected_events):
    """Reconstruct actual argv/cwd from the retained GNU Make verbose log."""
    configured={};actual={};stack={};events=[]
    for row in database:
        argv=row.get('arguments') or shlex.split(row['command']);key=without_dependencies(argv)
        need(argv[0] in ('/usr/bin/gcc','/usr/bin/g++') and row['directory']==logical_build and key not in configured,
            'one configured original compiler row per translation unit required')
        configured[key]=row
    for number,line in enumerate(log.splitlines(),1):
        text=line.strip();match=re.fullmatch(r"(?:make|gmake)(?:\[(\d+)\])?: (Entering|Leaving) directory '([^']+)'",text)
        if match:
            level=int(match[1] or 0);directory=posixpath.normpath(match[3])
            need(directory==logical_build or directory.startswith(logical_build+'/'),'Make cwd outside original build')
            if match[2]=='Entering':
                need(level not in stack and (not stack or level==max(stack)+1),'ambiguous Make entry');stack[level]=directory
            else:
                need(stack and level==max(stack) and stack[level]==directory,'unmatched Make exit');del stack[level]
            events.append({'line_number':number,'raw':line,'level':level,'operation':match[2],'cwd':directory});continue
        if not text.startswith(('/usr/bin/gcc ','/usr/bin/g++ ','cd ')):continue
        argv=shlex.split(text)
        if argv[0]=='cd':
            need(not(len(argv)>=4 and argv[2]=='&&' and argv[3] in ('/usr/bin/gcc','/usr/bin/g++')),
                'unreviewed cd-prefixed compile');continue
        if argv[0] not in ('/usr/bin/gcc','/usr/bin/g++') or '-c' not in argv:continue
        need(stack and stack[max(stack)]==logical_build and not any(v in ('&&',';','|','>','>>','<') for v in argv),
            'actual compiler lacks retained Make cwd witness')
        key=without_dependencies(argv);need(key in configured and key not in actual,'extra, duplicate or unmatched actual compile')
        actual[key]={'argv':argv,'cwd':logical_build,'make_level':max(stack),'raw':line,'line_number':number,
            'cwd_basis':'ACTUAL_RETAINED_MAKE_ENTER_LEAVE_CURRENT_DIRECTORY'}
    need(not stack and set(actual)==set(configured) and events==expected_events,
        'complete original Make/compiler database bijection required')
    return [actual[without_dependencies(row.get('arguments') or shlex.split(row['command']))] for row in database]


def message_contract(generation,catalog,profile,metadata,binding):
    """Compare all native IDs, bounds and codec identities, without inventing IDs."""
    wanted={row['type']:row for row in profile['definitions']}
    need(profile['family']=='unitree_go' and profile['ros_version']==2 and profile['default_authority']==[] and
        profile['repository']=='unitreerobotics/unitree_ros2' and profile['commit']==COMMIT and len(wanted)==24,
        'the complete official GO24 ROS2 profile required')
    need(generation['format']=='uwvm-ros-custom-bounded-v2' and generation['status']=='GENERATED_SOURCE_ONLY' and
        generation['native_registry_emitted'] is True and generation['implicit_permissions']==[] and
        generation['packages']=={'ros1':[],'ros2':['unitree_go']} and set(generation['outputs'])==OUTPUTS and
        catalog['format']=='uwvm-ros-custom-bounded-v2' and len(catalog['types'])==24 and
        metadata['format']=='uwvm-ros-official-bounded-native-v2' and metadata['ros_version']==2 and
        set(metadata['types'])==set(wanted),'complete original native-bound message-only provider required')
    types={row['type']:row for row in generation['types']};records={row['type']:row for row in generation['records']}
    bound={row['type']:row for row in catalog['types']}
    need(len(types)==len(records)==len(bound)==24 and set(types)==set(records)==set(bound)==set(wanted),
        'exact GO24 interface/record/catalog inventory required')
    for name,row in bound.items():
        native=metadata['types'][name];identity=native['native_identity'];component=native['components']['message']
        need(row['ros_version']==2 and row['bounds']==wanted[name]['bounds'] and Path(row['file']).suffix=='.msg' and
            re.fullmatch(r'RIHS01_[0-9a-f]{64}',identity) and row['native_identity']==identity==component['native_identity']==
                types[name]['native_identity']==records[name]['native_identity'] and
            set(native['components'])=={'message'} and component['official_type']==name and
            types[name]['kind']=='message' and types[name]['ros_version']==records[name]['ros_version']==2 and
            types[name]['adapter_codec_identity']==records[name]['adapter_codec_identity'] and
            re.fullmatch(r'UWVMROS1_[0-9a-f]{64}',records[name]['adapter_codec_identity']),
            'official native identity, component, bounds or codec association differs: '+name)
    need(binding['status']=='NATIVE_IDENTITY_BINDING_SOURCE_ONLY' and binding['native_registration']=='NOT_ESTABLISHED' and
        binding['implicit_permissions']==[],'the genuine source-only binder receipt required')
    return wanted


class NativeFamily:
    """Qualify actual old build records against current physical selected roots."""
    def __init__(self,frame,api,admission,selection,library):
        self.frame,self.api,self.admission,self.library=frame,api,admission,library
        self.evidence=admission.Evidence(api,frame,selection['restart_mappings'])
        self.raw_pin=frame.inputs[selection['native_report']]
        need(self.evidence.pin(ORIGINAL_PIN)==self.raw_pin,'actual current original C52 report differs from its separately retained historical pin')
        self.report=self.evidence.selected(selection['native_report']);self.family=terminal_original(self.report,self.raw_pin)
        self.source,self.frozen=self.evidence.source(self.report)
        self.root=Path(self.raw_pin['path']).parent;self.work=self.root/'work';self.rootfs=Path(self.report['execution_sdk_after']['root'])
        self.upstream=Path(frame.config['upstreams'][selection['upstream']]['root'])
        self.derived=Path(self.family['derived_tree']['root'])
        need(self.derived==self.work/'unitree_go' and self.rootfs==self.root/'rootfs' and
            self.root in frame.roots and self.rootfs in frame.roots and
            self.family['derived_tree']['symlinks']==self.family['native_prefix']['symlinks']==0,
            'complete original C52 output and exact saved execution-SDK root required; GO subtrees have no symlinks')
        self.native=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/noetic_native_support.py','A2_actual_native_ELF_reader')
        self.catalogs=api.frozen_module(frame.source,frame.frozen,'test/0073.ros-tests/support/source/ros_unitree/independent_cpp_object.py','A2_actual_catalog_comparator')
        self.products={};self.old_rows={};self.leaves={};self.elf_cache={}
        self.catalog_scope=[];self.observed_targets={}
        frame.common.cgroup_preserved(self.report['cgroup_before'],self.report['cgroup_after'])
        old=self.report['cgroup_before'];cg=frame.config['cgroup']
        need(old['memory_max']==64*1024**3 and old['memory_swap_max']==0 and old['path']==cg['expected_leaf'] and
            old['cpus']==cg['expected_cpus'],'original64/swap0 leaf is historical provenance, not current continuity')
        frame.common.cgroup_preserved(frame.before,frame.common.cgroup_state(cg))

    def body(self,old,maximum=64*MIB):
        actual=self.evidence.pin(old);raw=self.api.bounded(Path(actual['path']),maximum)
        need(len(raw)==actual['bytes'] and hashlib.sha256(raw).hexdigest()==actual['sha256'],'retained proof bytes changed');return raw

    def document(self,old,maximum=64*MIB):return self.library.decode(self.body(old,maximum))

    def original(self,path):
        path=str(path);rows=[p for p in self.report['input_pins']+self.report['product_pins'] if p['path']==path]
        if path in self.old_rows:rows.append(self.old_rows[path])
        need(rows and all(p==rows[0] for p in rows),'one authentic full7 required for consumed original file: '+path)
        return rows[0]

    def translate(self,name,cwd='/owned/unitree_go'):
        need(type(name) is str and name and '\0' not in name and '\n' not in name,'clean original compiler pathname required')
        logical=posixpath.normpath(name if name.startswith('/') else posixpath.join(cwd,name))
        for prefix,physical in ((str(self.source),self.source),(str(self.work),self.work),(str(self.rootfs),self.rootfs),
                                ('/source',self.source),('/owned',self.work),('/upstream',self.upstream)):
            if logical==prefix or logical.startswith(prefix+'/'):
                path=physical/logical[len(prefix):].lstrip('/');break
        else:
            need(any(logical==p or logical.startswith(p+'/') for p in ('/usr','/opt','/lib','/lib64','/bin','/sbin','/etc')),
                'compiler pathname outside original explicit chroot mappings')
            path=self.native.rooted(self.rootfs,logical)
        actual=path.resolve(strict=True)
        need(any(actual==r or actual.is_relative_to(r) for r in (self.source,self.work,self.rootfs,self.upstream)) and
            any(actual.is_relative_to(r) for r in self.frame.roots),'resolved original dependency outside selected roots')
        return actual

    def elf(self,old,kinds):
        actual_pin=self.evidence.pin(old['input']);key=actual_pin['path']
        if key not in self.elf_cache:
            self.elf_cache[key]=self.native.elf(self.frame.common,Path(key),kinds,guard=self.frame.guard)
        actual=self.elf_cache[key]
        need(actual['input']==actual_pin and actual['type'] in kinds and
            all(actual[k]==old[k] for k in ('machine','bits','endian','type')),
            'actual retained ELF/header differs from genuine original summary')
        # C52 recorded only these header fields, not a fabricated old PT_INTERP.
        return actual

    def tree(self,old):
        root=Path(old['root']);need(any(root==r or root.is_relative_to(r) for r in self.frame.roots),
            'complete original selected root must remain inventoried')
        rows=[row for row in self.frame.catalogs if row['root']==str(root)]
        if not rows:
            rows=[self.frame.common.catalog(root,self.frame.out/('A2-tree-%03d.jsonl'%len(self.catalog_scope)),self.frame.roots)]
            self.frame.pin(rows[0]['catalog']['path'])
        need(len(rows)==1,'one complete current catalog per original root required')
        proof=self.catalogs.catalog_pair(self.frame,self.evidence,old,rows[0],observed_targets=self.observed_targets)
        self.catalog_scope.append(proof)
        if root==self.derived:
            for raw in self.body(old['catalog'],16*MIB).splitlines():
                need(len(raw)<=65536,'bounded original family catalog row required');row=self.library.decode(raw)
                if row['kind']=='file':
                    pin={'path':str(root/row['relative']),**{k:row[k] for k in PIN-{'path'}}}
                    need(pin['path'] not in self.old_rows,'duplicate original GO file name');self.old_rows[pin['path']]=pin
        return proof

    def inventories(self):
        before,after=self.report['input_roots_before'],self.report['input_roots_after']
        need(len(before)==len(after)==23 and len({r['root'] for r in before})==23 and
            [r['root'] for r in before]==[r['root'] for r in after],'all original twenty-three selected root catalogs required')
        for a,b in zip(before,after):
            need(all(a[k]==b[k] for k in ('root','files','bytes','symlinks','content_sha256')),
                'original selected root changed during original build')
            need(Path(b['root']) in self.frame.roots,'every original input root must be a whole A2 input')
            self.evidence.pin(a['catalog']);self.tree(b)
        a,b=self.report['execution_sdk_before'],self.report['execution_sdk_after']
        need(all(a[k]==b[k] for k in ('root','files','bytes','symlinks','content_sha256')),
            'original independent execution SDK preservation differs')
        self.evidence.pin(a['catalog']);self.tree(b)
        later=[r for r in self.report['derived_roots_after'] if r['root']==str(self.derived)]
        need(len(later)==1 and all(later[0][k]==self.family['derived_tree'][k] for k in
            ('root','files','bytes','symlinks','content_sha256')),'complete genuine GO derived-tree preservation required')
        self.tree(later[0]);self.tree(self.family['native_prefix'])
        # No bind mount may currently cover the old ordinary-file evidence.
        for line in Path('/proc/self/mountinfo').read_text().splitlines():
            target=Path(re.sub(r'\\([0-7]{3})',lambda m:chr(int(m[1],8)),line.split()[4]))
            need(not(target==self.rootfs or target.is_relative_to(self.rootfs) or target==self.work or target.is_relative_to(self.work)),
                'original SDK/build evidence must be at rest without selected active mounts')

    def compile(self,role,section,fresh,count):
        database=self.document(section['compile_database']);need(type(database) is list and len(database)==count and
            len(section['compile_records'])==count and fresh['compile_database']==section['compile_database'],
            'complete real GO package/export compilation inventory required')
        logical=section['compile_records'][0]['actual_compile']['cwd'];build=self.translate(logical)
        need(Path(section['compile_database']['path']).parent==build and build.is_relative_to(self.derived),
            'original database/build ownership differs')
        log=self.body(section['verbose_build_log']).decode('utf-8','strict')
        witnesses=make_witness(database,log,logical,section['actual_make_directory_events'])
        objects=set();deps={};proofs=[]
        for full,configured,witness in zip(section['compile_records'],database,witnesses):
            self.frame.guard();argv=witness['argv']
            need(full['actual_compile']==witness and full['configured_compile_database_row']==configured and
                full['physical_compile_cwd']==str(build) and '-UNDEBUG' in argv and
                not any(x.startswith('-DNDEBUG') for x in argv),'original actual compile/assertion association changed')
            for flag in (('-O3','-g0','-fno-fast-math','-ffp-contract=off') if role=='official-package' else ('-O1','-g0')):
                need(flag in argv,'original package/export optimization flag absent')
            flags=lambda option:[argv[i+1] for i,t in enumerate(argv) if t==option]
            need(argv.count('-o')==argv.count('-c')==1 and '-MD' in argv and '-MMD' not in argv and
                flags('-MF') and flags('-MT'),'real full-MD source/object/target operands required')
            obj=self.translate(flags('-o')[0],logical);source=self.translate(flags('-c')[0],logical)
            need(source==self.translate(configured['file'],logical) and
                self.translate(flags('-MF')[-1],logical)==Path(full['depfile']['path']),
                'last-MF actual dependency file or primary source differs')
            raw=self.body(full['depfile'],16*MIB).decode('utf-8').replace('\\\n','')
            need('$' not in raw and raw.count(':')==1,'closed original compiler Make dependency grammar required')
            targets,names=raw.split(':',1)
            emitted=[self.translate(n,logical) for n in shlex.split(targets)]
            requested=[self.translate(n,logical) for value in flags('-MT') for n in shlex.split(value)]
            need(obj in emitted and requested and all(n in emitted for n in requested),'complete accumulated MD targets differ')
            options={'requested':{'MD_count':argv.count('-MD'),'MF':flags('-MF'),'MT':flags('-MT')},
                'effective':{'MF_last_argument':flags('-MF')[-1],'MF_physical_depfile':str(Path(full['depfile']['path'])),
                    'MT_accumulated_arguments':flags('-MT'),'MT_last_argument':flags('-MT')[-1],
                    'MT_requested_physical_targets':[str(n) for n in requested],
                    'emitted_physical_targets':[str(n) for n in emitted]},
                'semantics':'LAST_MF_FILENAME_ALL_MT_TARGET_OPERANDS_ACCUMULATE'}
            need(options==full['dependency_options'],'original recorded dependency option semantics differ')
            actual=[self.frame.pin(self.translate(n,logical)) for n in shlex.split(names)]
            need(actual==[self.evidence.pin(p) for p in full['dependencies']] and
                self.frame.pin(source)==self.evidence.pin(full['source']) and self.frame.pin(source) in actual and
                full['object']['input']['path']==str(obj) and str(obj) not in objects,
                'every actual ordered MD input and original source/object must match')
            self.elf(full['object'],{1});objects.add(str(obj))
            for pin in actual:deps[pin['path']]=pin
            proofs.append({'source':self.evidence.pin(full['source']),'object':self.evidence.pin(full['object']['input']),
                'dependency_file':self.evidence.pin(full['depfile']),'dependency_count':len(actual)})
        fresh_objects=fresh['objects']
        need(len(fresh_objects)==len(objects) and {r['physical'] for r in fresh_objects}==objects and
            all(r['existed_before_build'] is False and str(self.translate(r['logical']))==r['physical'] for r in fresh_objects),
            'every genuine original object must have original fresh-before-build evidence')
        trace=[];resolved={}
        for line in log.splitlines():
            if re.fullmatch(r'[^\s]+\.(?:o|a|so(?:\.[0-9]+)*)',line.strip()):
                pin=self.frame.pin(self.translate(line.strip(),logical));trace.append({'raw':line,'physical_input':pin});resolved[pin['path']]=pin
        need(len(trace)==len(section['actual_link_trace']) and all(a['raw']==b['raw'] and
            a['physical_input']==self.evidence.pin(b['physical_input']) for a,b in zip(trace,section['actual_link_trace'])) and
            list(resolved.values())==[self.evidence.pin(p) for p in section['resolved_link_inputs']] and objects<=set(resolved) and
            any(Path(p).is_relative_to(self.rootfs) for p in resolved),'complete actual original GNU link trace differs')
        outputs={}
        for script in section['link_commands']:
            commands=self.body(script['link_script']).decode('utf-8').splitlines()
            need(commands==script['actual_command_lines'] and commands and all(not x.strip() or x.strip() in log for x in commands),
                'actual original link script and retained verbose log differ')
            for line in commands:
                if not line.strip():continue
                argv=shlex.split(line)
                need(argv[0] in ('/usr/bin/gcc','/usr/bin/g++') and argv.count('-o')==1 and '-Wl,--trace' in argv and
                    not any(v in ('&&',';','|','>','>>','<') for v in argv),'original direct native linker argv required')
                output=self.translate(argv[argv.index('-o')+1],logical)
                need(output.is_relative_to(build) and str(output) not in outputs,'unique owned actual native link output required')
                old=self.original(output);actual=self.native.elf(self.frame.common,output,{2,3},guard=self.frame.guard)
                need(actual['input']==self.evidence.pin(old),'actual linked original ELF identity differs')
                outputs[str(output)]=actual
        need(len(outputs)==(11 if role=='official-package' else 1),'complete original linked package/export output set required')
        return {'role':role,'translation_units':count,'complete_MD':proofs,'actual_link_input_count':len(resolved),
            'linked_outputs':list(outputs.values()),'compile_database':self.evidence.pin(section['compile_database']),
            'verbose_build_log':self.evidence.pin(section['verbose_build_log']),
            'file_api':'NOT_CAPTURED_BY_ORIGINAL_C52_ROLE_NO_FILE_API_QUALIFICATION_CLAIM',
            'scope':'ORIGINAL_COMPLETE_FULL_MD_AND_LINK_REOPENED_NOT_A_NEW_COMPILATION'}

    def mapped_export(self):
        rows=[r for r in self.report['commands'] if r['label']=='unitree_go-actual-mapped-export']
        need(len(rows)==1,'one original real mapped GO exporter command required');row=rows[0];proof=row['mapped_proof']
        need(row['argv'][-3:]==['/owned/unitree_go/prefix','/owned/unitree_go/export-build/unitree-native-identities','--mapped-stop'] and
            row['argv'][0]=='/usr/bin/chroot' and row['argv'][1]==str(self.rootfs) and row['resume_count']==1 and row['gate_bytes_hex']=='49' and
            proof['status']=='VERIFIED_ACTUAL_OPENED_NATIVE_MAPS' and proof['cooperative_barrier'] is True and
            proof['source']=='ACTUAL_MAP_FILES_DESCRIPTOR_FSTAT_AND_FULL_SHA256_TO_PHYSICAL_KNOWN_CATALOG; RAW_MAPS_DEVICE_KEPT_SEPARATE',
            'real stopped original exporter mapping/gate evidence required')
        binary=self.family['binary'];actual=self.elf(binary,{2,3});selected=proof['stopped']
        need(selected['state']=='T' and selected['pid']==proof['selected_pid'] and selected['starttime']>0 and
            proof['stopped_after_mapping_proof']==selected and
            all(proof[k]['pid']==selected['pid'] and proof[k]['starttime']==selected['starttime']
                for k in ('first_ancestry','after_pidfd')) and
            proof['popen_root_pid']==row['pid']==row['first_identity']['pid'] and
            (proof['is_popen_root'] is True or selected['parent_pid']==row['pid']) and
            proof['actual_exe']==proof['actual_exe_after'] and
            proof['actual_exe']['target_facts']=={k:binary['input'][k] for k in PIN-{'path'}},
            'same original stopped process birth/executable evidence required')
        maps={}
        for item in proof['map_files']:
            need(item['status']=='OPENED_FULL_HASH_AND_PHYSICAL_PIN_VERIFIED' and
                all(item[k]['state']=='T' and item[k]['pid']==selected['pid'] and item[k]['starttime']==selected['starttime']
                    for k in ('stopped_before','stopped_after')) and
                item['opened_target_facts']=={k:item['physical_input'][k] for k in PIN-{'path'}},
                'actual original map descriptor/full-SHA/process witness differs')
            fields=item['raw'].split(maxsplit=5)
            need(len(fields)==6 and fields[3]==item['raw_maps_device'] and int(fields[4])==item['raw_maps_inode']==item['physical_input']['inode'] and
                fields[5]==item['link_text'] and item['raw'] in proof['raw_maps'],
                'original maps raw-device/inode/path association differs')
            old=item['physical_input'];current=self.evidence.pin(old)
            need(any(Path(current['path']).is_relative_to(r) for r in (self.derived,self.rootfs)),
                'original mapped leaf outside selected GO/SDK physical closure')
            if current['path'] not in maps:
                value=self.native.elf(self.frame.common,Path(current['path']),{2,3},guard=self.frame.guard)
                need(value['input']==current,'current mapped leaf ELF/full7 changed');maps[current['path']]=value
        need(actual['input']['path'] in maps and len(actual['PT_INTERP'])==1 and
            str(self.translate(actual['PT_INTERP'][0])) in maps,'actual retained exporter and loader must appear in original maps')
        # DT_NEEDED is a SONAME: a real 'libstdc++.so.6' alias may map the
        # physical 'libstdc++.so.6.0.30' inode. Resolve only finite directories
        # actually represented by this process's retained selected mappings;
        # never compare basenames as a substitute for actual physical proof.
        parents={Path(path).parent for path in maps};needed=[]
        for value in maps.values():
            for name in value['DT_NEEDED']:
                need('/' not in name and name not in ('.','..'),'bare bounded actual DT_NEEDED required')
                matches={}
                for parent in parents:
                    try:
                        if parent.is_relative_to(self.rootfs):
                            resolved=self.native.rooted(self.rootfs,'/'+str((parent/name).relative_to(self.rootfs)))
                        else:resolved=(parent/name).resolve(strict=True)
                    except FileNotFoundError:continue
                    need(any(resolved.is_relative_to(r) for r in (self.rootfs,self.derived)),
                        'selected needed-library alias escaped original physical closure')
                    if str(resolved) in maps:matches[str(resolved)]=maps[str(resolved)]['input']
                need(len(matches)==1,'each actual needed library must resolve uniquely to a retained mapped physical file')
                needed.append({'requester':value['input'],'DT_NEEDED':name,'mapped_physical':next(iter(matches.values()))})
        output=row['stdout'];need(output['path']==str(self.derived/'native-metadata.json') and row['stderr']['bytes']==0,
            'actual original native metadata stdout and empty stderr required')
        self.evidence.pin(row['stderr']);self.evidence.pin(output)
        return {'original_command_label':row['label'],'original_stdout':output,'original_binary':binary['input'],
            'original_stopped_process':selected,'current_at_rest_ELFs':list(maps.values()),
            'raw_device_equated_with_physical_st_dev':False,'original_process_reexecuted':False,'current_needed_alias_bindings':needed}

    def copy(self,role,old,relative):
        now=self.evidence.pin(old)
        return self.library.leaf(self.frame,self.evidence.restart,self.leaves,role,old,now,self.frame.out/'library'/relative)


def stage(frame,api,admission,selection,library):
    """A2 performs only finite physical qualification and exclusive byte copies."""
    library.closed(selection,('format_version','kind','native_report','upstream','restart_mappings'),'GO library selection')
    need(selection['format_version']==1 and selection['kind']=='uwvm-native-go24-library-selection-v1' and
        frame.config['simulation'] is frame.config['benchmark'] is frame.config['profiler'] is None and
        frame.config['cgroup']['memory_max']==64*1024**3 and frame.config['cgroup']['memory_swap_max']==0,
        'explicit compiler-free current64/swap0 GO24 library preparation required')
    native=NativeFamily(frame,api,admission,selection,library);native.inventories()
    report,family,evidence=native.report,native.family,native.evidence
    # Frame.validate_upstreams already reopened this exact complete official
    # Git tree and all original lock-selected blobs. A bare commit string or a
    # definition-only directory cannot replace that whole-root qualification.
    upstream=frame.config['upstreams'][selection['upstream']]
    need(upstream['repository'].removeprefix('https://github.com/').removesuffix('.git')=='unitreerobotics/unitree_ros2' and
        upstream['commit']==COMMIT and frame.inputs[upstream['lock_input']]==evidence.pin(report['upstream_proof']),
        'actual complete official Unitree upstream proof must be a selected Frame role')
    metadata_input=native.document(report['metadata_input'])
    sdk_parent=native.document(report['sdk_parent'])
    acquisition=metadata_input['acquisition']
    need(sdk_parent.get('status')=='PREPARED_AUTHENTICATED_EXTRACTED_SYSROOT' and
        acquisition['status']==sdk_parent['status'] and all(sdk_parent.get(k) is True for k in
        ('source_preserved','inputs_preserved','products_preserved')) and sdk_parent.get('cleanup')=={'status':'PASS','remaining':[]} and
        metadata_input['report']=={'path':report['sdk_parent']['path'],'sha256':report['sdk_parent']['sha256']} and
        acquisition['sdk']==sdk_parent['sdk'] and acquisition['source']==sdk_parent['source'] and
        acquisition['source_manifest_sha256']==sdk_parent['source_manifest_sha256'],
        'actual original authenticated SDK report/metadata association required')
    # Complete original SDK source bytes were already checked in the23-root
    # comparison. Acquisition Source27 is preserved as an original report role;
    # only the actual C52 Source30 compilation is asserted current above.
    original_tools=report['selected_original_sdk_tools'];execution_tools=report['selected_execution_sdk_tools']
    need(len(original_tools)==len(execution_tools)==12 and
        [r['logical'] for r in original_tools]==[r['logical'] for r in execution_tools],
        'complete twelve actual original/execution SDK tool associations required')
    tool_proofs=[]
    for original,execution in zip(original_tools,execution_tools):
        old=evidence.pin(original['original']);new=evidence.pin(execution['copy']['input'])
        need(original['original']==execution['original'] and library.body_equal(old,new) and
            (old['device'],old['inode'])!=(new['device'],new['inode']) and
            str(native.translate(execution['logical']))==new['path'],
            'original SDK tool and independent execution copy association differs')
        tool_proofs.append(native.elf(execution['copy'],{2,3}))
    upstream_root=Path(upstream['root'])
    bind=[row for row in report['commands'] if row['argv']==['/usr/bin/mount','--bind',str(upstream_root),str(native.rootfs/'upstream')]]
    need(len(bind)==1,'actual original immutable upstream bind mapping required')
    configured=next(row for row in report['commands'] if row['label']=='unitree_go-package-configure')['argv']
    need(configured[configured.index('-S')+1]=='/upstream/cyclonedds_ws/src/unitree/unitree_go' and
        configured[configured.index('-B')+1]=='/owned/unitree_go/package-build' and
        '-DCMAKE_INSTALL_PREFIX=/owned/unitree_go/prefix' in configured,
        'actual official package source/build/install command association differs')
    for name in ('CMakeLists.txt','package.xml'):
        physical=upstream_root/'cyclonedds_ws/src/unitree/unitree_go'/name
        selected=metadata_input['package_sources'][str(physical)];raw=selected['text'].encode('utf-8')
        actual=frame.pin(physical)
        need(len(raw)==actual['bytes'] and hashlib.sha256(raw).hexdigest()==actual['sha256']==selected['sha256'],
            'retained actual official CMake/package source snapshot differs')
    source_profile=native.source/'sdk/origin/robots/unitree/profiles/unitree_go.json'
    old_profile=native.original(native.derived/'selection/profile.json')
    current_profile=library.selected(frame,frame.source/'sdk/origin/robots/unitree/profiles/unitree_go.json')
    need(library.body_equal(evidence.pin(old_profile),current_profile) and
        frame.pin(source_profile)['sha256']==current_profile['sha256'] and
        frame.frozen.get('sdk/origin/robots/unitree/profiles/unitree_go.json')=={k:current_profile[k] for k in ('bytes','sha256')},
        'original and current complete GO profile bytes must agree')
    profile=native.document(old_profile)
    prepared=native.document(family['source_selection']);material=native.document(family['materialization'])
    need(prepared['kind']=='unitree-native-export-preparation' and prepared['status']=='PREPARED_SOURCE_ONLY' and
        prepared['source']==str(native.source) and prepared['source_manifest_sha256']==report['source_manifest_sha256'] and
        prepared['family']=='unitree_go' and prepared['ros_version']==2 and prepared['inputs_preserved'] is True and
        prepared['native_identity_export_executed'] is prepared['compilers_generators_executed'] is False and
        prepared['runtime_permissions']==[],'original exact source-selection role required')
    evidence.retained(prepared['input_pins']);evidence.retained(prepared['outputs'])
    need(material['status']=='PREPARED_NOT_EXECUTED' and material['scope']=='SOURCE_CATALOG_ONLY' and
        material['family']=='unitree_go' and material['repository']==profile['repository'] and material['commit']==COMMIT and
        material['inputs_preserved'] is True and material['native_identity']=='NOT_ACQUIRED' and material['runtime_permissions']==[] and
        material['generators_compilers_tests_executed'] is False and len(material['inputs'])==26,
        'genuine lock-checked materialization of all24 original definitions required')
    evidence.retained(material['inputs']);evidence.retained(material['outputs'])
    build_proofs=[native.compile('official-package',family['official_package_build'],family['fresh_package_objects'],195),
                  native.compile('identity-exporter',family['export_build'],family['fresh_export_objects'],1)]
    mapping=native.mapped_export()
    gen_pin=family['generation'];generation=native.document(gen_pin)
    catalog_pin=native.original(native.derived/'bound/catalog.json');catalog=native.document(catalog_pin)
    metadata_pin=native.original(native.derived/'native-metadata.json');metadata=native.document(metadata_pin)
    binding=native.document(family['binding_receipt'])
    wanted=message_contract(generation,catalog,profile,metadata,binding)
    def body_pin(pin):return {k:pin[k] for k in ('bytes','sha256')}
    need(generation['source_inputs']['catalog']==body_pin(catalog_pin) and binding['output']=={
        'path':catalog_pin['path'],**body_pin(catalog_pin)} and binding['official_export']=={
        'path':metadata_pin['path'],**body_pin(metadata_pin)} and mapping['original_stdout']==metadata_pin,
        'actual binder output/native-export/generation catalog association differs')
    template_pin=native.original(native.derived/'selection/catalog/catalog.preparation.json')
    need(binding['template']=={'path':template_pin['path'],**body_pin(template_pin)},'actual original binder template differs')
    template=native.document(template_pin)
    expected_bound=json.loads(json.dumps(template))
    for row in expected_bound['types']:row['native_identity']=metadata['types'][row['type']]['native_identity']
    need(expected_bound==catalog,'bound catalog differs beyond exactly exported native identities')
    rows=generation['source_inputs']['definitions'];need(len(rows)==24 and
        {row['file'] for row in rows}==set(binding['definition_copies'])=={row['file'] for row in catalog['types']},
        'all24 exact copied definitions must belong to original binder/generator')
    definitions={};upstream_root=Path(upstream['root'])
    for index,row in enumerate(rows):
        relative=Path(row['file']);need(not relative.is_absolute() and '..' not in relative.parts and str(relative)==row['file'],
            'clean catalog-relative complete definition required')
        old=native.original(Path(catalog_pin['path']).parent/relative)
        name=next(value['type'] for value in catalog['types'] if value['file']==row['file'])
        declared=wanted[name];official=frame.pin(upstream_root/declared['path'])
        need(body_pin(old)==body_pin(official)=={k:row[k] for k in ('bytes','sha256')}==binding['definition_copies'][row['file']] and
            any(p['path']==official['path'] and library.body_equal(p,official) for p in material['inputs']),
            'actual original official definition/lock/binder/generator byte closure differs')
        definitions[row['file']]=native.copy('definition-%03d'%index,old,'catalog/'+row['file'])
    output_pins={}
    for index,name in enumerate(sorted(OUTPUTS)):
        old=native.original(Path(gen_pin['path']).parent/name)
        need(generation['outputs'][name]==body_pin(old),'all11 actual generated provider bytes required')
        output_pins[name]=native.copy('generated-%03d'%index,old,'generated/'+name)
    copied={}
    for role,pin,name in (('original-report',ORIGINAL_PIN,'proof/original-c52-report.json'),
        ('original-generation',gen_pin,'generated/generation.json'),('native-catalog',catalog_pin,'catalog/catalog.json'),
        ('profile',old_profile,'proof/profile.json'),('official-native-metadata',metadata_pin,'proof/native-metadata.json'),
        ('binding-receipt',family['binding_receipt'],'proof/binding-receipt.json'),
        ('source-selection',family['source_selection'],'proof/source-selection.json'),
        ('materialization',family['materialization'],'proof/materialization.json'),
        ('unbound-template',template_pin,'proof/unbound-template.json'),
        ('original-source-manifest',report['source_before']['manifest'],'proof/original-source-manifest.json')):
        copied[role]=native.copy(role,pin,name)
    # Historical generator digests are tied to the complete original MF. B
    # genuinely runs its current emitter; this is never relabelled as current.
    for role,relative,key in (('original-generator','sdk/origin/tools/custom_types.py','generator_sha256'),
        ('original-bounded-generator','sdk/origin/custom-types/bounded_profile.py','bounded_generator_sha256')):
        old=frame.pin(native.source/relative)
        need(native.frozen[relative]==body_pin(old) and generation[key]==old['sha256'],'actual original emitter/MF provenance differs')
        copied[role]=native.copy(role,old,'proof/'+role+'.py')
    config=library.leaf(frame,evidence.restart,native.leaves,'preparation-configuration',frame.report['configuration'],
        frame.report['configuration'],frame.out/'library/proof/preparation-configuration.json')
    # These are exactly catalog-observed external regular aliases, not selected
    # MD/tool/library inputs. The reviewed comparator owns the finite held-FD
    # ledger and reopens every source alias/target; normal Frame.finish then
    # independently repeats all actual saved complete source-root catalogs.
    catalog_observations=native.catalogs.preserve_catalog_observations(frame,evidence,native.observed_targets)
    frame.report.update(producer=library.selected(frame,frame.source/'test/0073.ros-tests/support/source/ros_unitree/prepare_native_family_library.py'),
        helper=library.selected(frame,frame.source/'test/0073.ros-tests/support/source/ros_unitree/library_inputs.py'),
        native_family_helper=library.selected(frame,frame.source/'test/0073.ros-tests/support/source/ros_unitree/native_family_inputs.py'),
        preparation_configuration_copy=config,library={'leaves':native.leaves,'original_report':ORIGINAL_PIN,'preparation_original_report_current':native.raw_pin,
            'original_source':str(native.source),'original_source_manifest_sha256':report['source_manifest_sha256'],
            'original_generation':gen_pin,'generation':copied['original-generation'],'catalog':copied['native-catalog'],
            'profile':copied['profile'],'native_metadata':copied['official-native-metadata'],'binding_receipt':copied['binding-receipt'],
            'outputs':output_pins,'definitions':definitions,'all24_native_types':sorted(wanted),
            'preparation_complete_original_root_observations':native.catalog_scope,
            'preparation_catalog_observed_external_regular_targets':catalog_observations,
            'preparation_retained_compilation':build_proofs,'preparation_retained_exporter_mapping':mapping,
            'preparation_original_SDK_report':evidence.pin(report['sdk_parent']),
            'preparation_actual_execution_SDK_tools':tool_proofs,
            'generated_native_provider_compilation':'NOT_CLAIMED_C52_EMITTED_SOURCE_ONLY_SUBSEQUENT_GATEWAY_QUALIFIER_REQUIRED',
            'other_family_current_requalification':'NOT_SELECTED_NOT_CLAIMED'},
        original_roots_observed_during_preparation=True,original_roots_claimed_current_during_later_build=False,
        SDK_modified=False,compilation_performed=False,Runtime_executed=False,actors_started=0,guest_authority=[],commands=[],cleanup=[],
        copy_descriptor_cleanup={'status':'PASS','remaining':[],'owned_copy_descriptors':getattr(frame,'_library_open_descriptors',0)},
        library_output_state='CONSUMABLE_ONLY_IF_ENCLOSING_REPORT_FINISHES_PASS',original_reports_rewritten=False,
        Typed14_required_or_promoted=False,Native6_failed_batch_promoted=False,selected_pin_bridges=evidence.bridges,
        scope='ORIGINAL_C52_NATIVE_PACKAGE_AND_EXPORT_QUALIFIED_COPIED_INPUT_NOT_NEW_COMPILATION_OR_RUNTIME')
    need(not frame.actors and frame.report['copy_descriptor_cleanup']['owned_copy_descriptors']==0,
        'all owned copy descriptors must close and no actor may start')
    # Preserve every distinct actual input exactly once. Shared headers occur
    # in many complete MD rules; conflicting pins still fail, and Frame.finish
    # really reopens every unique full7 plus all complete roots after this step.
    unique={}
    for pin in frame.pins:
        need(pin['path'] not in unique or unique[pin['path']]==pin,'conflicting repeated actual A2 input pin')
        unique[pin['path']]=pin
    frame.pins[:]=unique.values()
    return STATUS


def consume(frame,api,label,library,*,selected_pin=None):
    """B reads terminal A2's copied leaves, not its unused original whole roots."""
    if selected_pin is not None:
        # A receipt is an explicit finite current input, even when a physics
        # caller obtains it through the actual main receipt rather than a CLI
        # label. This view does not mutate Frame.inputs or enlarge its roots.
        library.current(frame,selected_pin)
        class SelectedView:
            def __init__(self,parent):self.parent=parent;self.inputs={'exact-A2-report':selected_pin}
            def __getattr__(self,name):return getattr(self.parent,name)
        frame=SelectedView(frame);label='exact-A2-report'
    report,pin=library.terminal_stage(frame,api,label,KIND,STATUS,
        producer_relative='test/0073.ros-tests/support/source/ros_unitree/prepare_native_family_library.py')
    helper=library.current(frame,report['native_family_helper'])
    need(helper['path']==str(frame.source/'test/0073.ros-tests/support/source/ros_unitree/native_family_inputs.py') and
        frame.frozen['test/0073.ros-tests/support/source/ros_unitree/native_family_inputs.py']=={k:helper[k] for k in ('bytes','sha256')},
        'A2 producer/consumer helper must be current complete-MF source')
    value=report['library'];root=Path(pin['path']).parent
    library.copied_leaves(frame,value['leaves'],root)
    need(all(row['copy'] in report['input_pins'] for row in value['leaves'].values()) and
        report['Native6_failed_batch_promoted'] is False and report['Typed14_required_or_promoted'] is False,
        'every copied A2 leaf must have passed actual normal terminal preservation')
    copies=value['leaves']
    def doc(role):
        actual=copies[role]['copy'];library.current(frame,actual)
        return library.decode(api.bounded(Path(actual['path']),64*MIB))
    original=doc('original-report');family=terminal_original(original,copies['original-report']['historical'])
    need(value['original_report']==ORIGINAL_PIN==copies['original-report']['historical'] and
        value['original_generation']==family['generation']==copies['original-generation']['historical'] and
        value['original_source']==original['source'] and value['original_source_manifest_sha256']==original['source_manifest_sha256'],
        'A2 immutable original C52/generation/source association differs')
    generation,catalog,profile,metadata,binding=(doc(key) for key in
        ('original-generation','native-catalog','profile','official-native-metadata','binding-receipt'))
    wanted=message_contract(generation,catalog,profile,metadata,binding)
    need(len(value['preparation_complete_original_root_observations'])==26 and
        [(x['role'],x['translation_units']) for x in value['preparation_retained_compilation']]==
            [('official-package',195),('identity-exporter',1)] and
        value['generated_native_provider_compilation']==
            'NOT_CLAIMED_C52_EMITTED_SOURCE_ONLY_SUBSEQUENT_GATEWAY_QUALIFIER_REQUIRED',
        'completed original A2 scope must retain true package/export and source-provider distinction')
    need(value['generation']==copies['original-generation']['copy'] and value['catalog']==copies['native-catalog']['copy'] and
        value['profile']==copies['profile']['copy'] and value['native_metadata']==copies['official-native-metadata']['copy'] and
        value['binding_receipt']==copies['binding-receipt']['copy'] and set(value['outputs'])==OUTPUTS and
        set(value['definitions'])=={row['file'] for row in generation['source_inputs']['definitions']},
        'finite copied provider role association differs')
    current_profile=library.selected(frame,frame.source/'sdk/origin/robots/unitree/profiles/unitree_go.json')
    need(library.body_equal(current_profile,value['profile']) and
        generation['source_inputs']['catalog']=={k:value['catalog'][k] for k in ('bytes','sha256')},
        'actual copied catalog and current complete profile bytes differ')
    old_manifest=doc('original-source-manifest')
    for role,relative,key in (('original-generator','sdk/origin/tools/custom_types.py','generator_sha256'),
        ('original-bounded-generator','sdk/origin/custom-types/bounded_profile.py','bounded_generator_sha256')):
        old=copies[role]['copy'];need(old_manifest[relative]=={k:old[k] for k in ('bytes','sha256')} and
            generation[key]==old['sha256'],'copied actual original emitter/complete MF relation differs')
    for name,actual in value['outputs'].items():
        library.current(frame,actual);need(Path(actual['path'])==Path(value['generation']['path']).parent/name and
            generation['outputs'][name]=={k:actual[k] for k in ('bytes','sha256')},'all11 current copied provider bytes required')
    for row in generation['source_inputs']['definitions']:
        relative=Path(row['file']);actual=value['definitions'][row['file']]
        need(not relative.is_absolute() and '..' not in relative.parts and
            Path(actual['path'])==Path(value['catalog']['path']).parent/relative and
            {k:actual[k] for k in ('bytes','sha256')}=={k:row[k] for k in ('bytes','sha256')}==binding['definition_copies'][row['file']],
            'all24 finite copied complete definition bytes required')
        library.current(frame,actual)
    return {'catalog':value['catalog'],'historical_generation':value['generation'],
        'original_generation':value['original_generation'],'original_generation_body':generation,
        'original_complete_family':{'family':'unitree_go','original_report':ORIGINAL_PIN,
            'original_generation':family['generation'],
            'scope':'ORIGINAL_C52_SOURCE_PROVIDER_ROW_FULL_BUILD_RECORDS_RETAINED_IN_UNCHANGED_REPORT'},'profile':current_profile,'native_family_library_report':pin,
        'original_native_report':value['original_report'],'original_source':value['original_source'],
        'original_manifest_sha256':value['original_source_manifest_sha256'],
        'library_scope':'TERMINAL_A2_COPIED_INPUTS_ONLY_ORIGINAL_ROOTS_NOT_CURRENT_B_OBSERVATIONS'}
