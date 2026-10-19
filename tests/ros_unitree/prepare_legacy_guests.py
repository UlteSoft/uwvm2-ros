#!/usr/bin/env python3
"""Compile six fresh same-main ROS1 legacy commands; execute no Guest or ROS.

The complete current source, all native-bound eleven-message outputs, selected
compiler packages, sysroots and the official C++ termination object are explicit
inputs. Every real compiler/link/validator is serial, bounded and pidfd-owned.
Recipes are expanded as argv arrays, never interpreted by a shell. No tool is
downloaded or discovered from a distribution label, PATH or a prior PASS name.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import stat
import sys
import time
import traceback
import types

sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[2]
MIB=1024**2;GIB=1024**3
KIND='uwvm-unitree-legacy-same-main-command-products-v1'
STATUS='PREPARED_SIX_LEGACY_COMMAND_PRODUCTS_BINARY_VALIDATED_NOT_EXECUTED'
ROW_STATUS='COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED'
RECIPE='sdk/robots/unitree/recipes/legacy-command-recipes.json'
EXAMPLES='sdk/robots/unitree/examples/legacy'
ENTRY='tests/ros_unitree/legacy_command'
FAMILY='unitree_legged_msgs'
LANGUAGES=('c','cpp','rust','tinygo','assemblyscript','javascript')
OUTPUTS={*('bounded_records.'+x for x in ('h','hpp','rs','go','ts','mjs')),
         'bounded_interfaces.h','bounded_interfaces.hpp','manifest.hpp','native_ros1.hpp','native_ros2.hpp'}
TRAP='-D_LIBCPP_VERBOSE_ABORT(...)=__builtin_trap()'
CG_LEAF='/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'


def need(value,reason):
    if not value:raise ValueError(reason)


def pairs(rows):
    result={}
    for key,value in rows:need(key not in result,'duplicate JSON key');result[key]=value
    return result


def bounded(path,maximum):
    """Size-first stable ordinary bootstrap bytes, before any project import."""
    path=Path(path);need(path.is_absolute() and path.resolve(strict=True)==path,'physical bootstrap path required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
    try:
        first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=maximum,'bounded regular bootstrap required')
        raw=bytearray()
        while True:
            part=os.read(fd,min(65536,maximum+1-len(raw)))
            if not part:break
            raw.extend(part);need(len(raw)<=maximum,'bootstrap grew')
        last,named=os.fstat(fd),path.stat(follow_symlinks=False)
        need(len(raw)==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k)
             for k in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')),'bootstrap changed')
        return bytes(raw)
    finally:os.close(fd)


def decode(raw):
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:need(False,'nonfinite JSON'))


def javascript_prepared_file(frame,original,maximum=128*MIB):
    """Associate one genuine SDK closed8 observation with the Frame's full7.

    The JavaScript preparer deliberately retains raw ``st_mode`` in addition
    to the seven physical/stat/hash fields used by Frame. Neither dictionary
    substitutes for the other: retain the original eight fields unchanged,
    independently stream an opened ordinary file including its raw mode, and
    only then ask the normal Frame pin admission to observe the same full7.
    This grants no new path authority: this helper requires an existing source,
    owned-output or whole-input root before opening the selected file.
    """
    keys={'path','bytes','sha256','device','inode','mtime_ns','ctime_ns','mode'}
    need(type(original) is dict and set(original)==keys,'exact JavaScript SDK closed8 file observation required')
    need(type(original['path']) is str and type(original['sha256']) is str and
         re.fullmatch('[0-9a-f]{64}',original['sha256']) and
         all(type(original[key]) is int and original[key]>=0 for key in keys-{'path','sha256'}) and
         original['inode']>0 and stat.S_ISREG(original['mode']),
         'typed JavaScript SDK ordinary-file identity and raw mode required')
    need(type(maximum) is int and 0<maximum<=128*MIB and original['bytes']<=maximum,
         'bounded JavaScript SDK source/input required')
    path=Path(original['path'])
    frame.guard()
    need(path.is_absolute() and path.resolve(strict=True)==path,'physical JavaScript SDK source/input required')
    need(any(path.is_relative_to(Path(root)) for root in (frame.source,frame.out,*frame.roots)),
         'JavaScript SDK input is outside the actual selected Frame scope')
    descriptor=os.open(path,os.O_RDONLY|os.O_CLOEXEC|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        first=os.fstat(descriptor)
        need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=maximum and first.st_mode==original['mode'],
             'opened JavaScript SDK raw mode or file extent differs')
        digest=hashlib.sha256();size=0
        while True:
            frame.guard();block=os.read(descriptor,min(MIB,maximum+1-size))
            if not block:break
            size+=len(block);need(size<=maximum,'JavaScript SDK input grew beyond its streamed bound');digest.update(block)
        actual={'path':str(path),'bytes':size,'sha256':digest.hexdigest(),'device':first.st_dev,
                'inode':first.st_ino,'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns}
        expected={key:original[key] for key in keys-{'mode'}}
        need(actual==expected,'opened JavaScript SDK full7 differs from its original closed8 record')
        # Keep Frame's normal full-file observation and terminal ledger. The
        # explicit scope check above adds no ``owned=True`` or input root.
        need(frame.pin(path)==actual,'JavaScript SDK observation differs from the actual Frame file')
        frame.guard();last=os.fstat(descriptor);named=path.stat(follow_symlinks=False)
        fields=('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')
        need(size==first.st_size and path.resolve(strict=True)==path and
             all(getattr(first,key)==getattr(last,key)==getattr(named,key) for key in fields),
             'JavaScript SDK descriptor/name changed during Frame association')
        return {'original_sdk_pin':dict(original),'current_file':actual,'descriptor_mode':first.st_mode}
    finally:os.close(descriptor)


def frozen_module(source,frozen,relative,name):
    """Load only complete-manifest-selected helper bytes; never its main."""
    path=source/relative;raw=bounded(path,MIB)
    need(frozen.get(relative)=={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()},'helper absent from current complete freeze: '+relative)
    value=types.ModuleType(name);value.__file__=str(path);sys.modules[name]=value
    exec(compile(raw,str(path),'exec'),value.__dict__)
    return value


def bootstrap(args):
    need(sys.platform=='linux' and os.geteuid()==0 and hasattr(os,'pidfd_open'),'Root-owned actual Linux cgroup lane required')
    need(args.source.resolve(strict=True)==ROOT and all(getattr(args,k).is_absolute()
         for k in ('source','manifest','config','out')),'invoke actual complete frozen source with absolute paths')
    for name in ('manifest_sha256','config_sha256'):need(re.fullmatch('[0-9a-f]{64}',getattr(args,name)),'actual control SHA required')
    raw=bounded(args.manifest.resolve(strict=True),16*MIB)
    need(hashlib.sha256(raw).hexdigest()==args.manifest_sha256,'complete source manifest changed')
    frozen=decode(raw);own='tests/ros_unitree/prepare_legacy_guests.py'
    need(Path(__file__).absolute()==ROOT/own,'actual frozen producer entry required')
    body=bounded(ROOT/own,MIB)
    need(type(frozen) is dict and frozen.get(own)=={'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest()},'producer absent from complete manifest')
    raw=bounded(args.config.resolve(strict=True),16*MIB)
    need(hashlib.sha256(raw).hexdigest()==args.config_sha256,'actual configuration changed')
    cg=decode(raw)['cgroup'];keys={'memory_max','memory_swap_max','expected_leaf','expected_device','expected_inode','expected_boot_id','expected_cpus'}
    need(type(cg) is dict and set(cg)==keys and type(cg['memory_max']) is int and cg['memory_max']==64*GIB and
         type(cg['memory_swap_max']) is int and cg['memory_swap_max']==0 and cg['expected_leaf']==CG_LEAF and
         cg['expected_cpus']=='0-31','unchanged original64GiB/swap0/CPU leaf required')
    leaf=Path(cg['expected_leaf']);info=leaf.stat()
    need(leaf.resolve(strict=True)==leaf and (info.st_dev,info.st_ino)==(cg['expected_device'],cg['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip()==cg['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines()==['0::'+str(leaf).removeprefix('/sys/fs/cgroup')] and
         (leaf/'memory.max').read_text().strip()==str(64*GIB) and (leaf/'memory.swap.max').read_text().strip()=='0' and
         (leaf/'cpuset.cpus.effective').read_text().strip()==cg['expected_cpus'],'actual current cgroup incarnation differs')
    api=types.SimpleNamespace(frozen_module=frozen_module,bounded=bounded)
    unitree=frozen_module(ROOT,frozen,'tests/ros_unitree/common.py','legacy_guest_current_owners')
    return frozen,api,unitree


def owned_frame_type(unitree,api,frozen):
    """Keep the exact selected Noetic CHROOT inventory literal, per Frame.

    Absolute symlink spellings inside the retained Focal SDK are CHROOT data,
    not host file selections. Only this report's complete immutable parent gets
    the independently reviewed descriptor walker. All other root catalogs,
    physical input pins and executable/mapping authority retain normal rules.
    """
    class LegacyGuestFrame(unitree.Frame):
        def __init__(self,args):
            self.frozen=frozen;self.selection_label=args.selection_input
            self.selected_manifest_sha=args.manifest_sha256
            super().__init__(args,KIND)
        def validate_upstreams(self):
            super().validate_upstreams()
            selection,pin=self.common.document(self.input(self.selection_label))
            need(pin==self.inputs[self.selection_label],'actual selected compiler metadata changed before root inventory')
            native_pin=self.inputs[selection['native_products']]
            raw=bounded(Path(native_pin['path']),64*MIB)
            need(len(raw)==native_pin['bytes'] and hashlib.sha256(raw).hexdigest()==native_pin['sha256'] and
                 self.common.opened(Path(native_pin['path']))==native_pin,'actual native report changed before literal inventory')
            report=decode(raw);root=Path(native_pin['path']).parent
            need(report.get('schema')=='uwvm-unitree-noetic-native-products-v1' and
                 report.get('status')=='PREPARED_NOETIC_LEGACY_NATIVE_PRODUCTS_NOT_TCPROS_EXECUTED' and
                 report.get('source')==str(self.source) and report.get('source_manifest_sha256')==self.selected_manifest_sha and
                 root in self.roots and native_pin['path']==str(root/'report.json'),
                 'exact complete current-source Noetic producer root required for literal inventory')
            support=api.frozen_module(self.source,frozen,'tests/ros_unitree/noetic_native_support.py','legacy_guest_literal_noetic_inventory')
            original=self.common.catalog
            def catalog(selected,output,allowed=None):
                if Path(selected)==root:return support.tree(self.common,root,Path(output),self.guard)
                return original(selected,output,allowed)
            self.common.catalog=catalog;self.literal_native_root=root
    return LegacyGuestFrame


def current_family(frame,api,selection):
    """Admit only the new actual Noetic eleven-message family, never GO/HG."""
    admission=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/typed_transport_inputs.py','legacy_guest_finite_evidence')
    evidence=admission.Evidence(api,frame,selection['restart_mappings'])
    native=evidence.selected(selection['native_products'])
    admission.clean_build(native,'PREPARED_NOETIC_LEGACY_NATIVE_PRODUCTS_NOT_TCPROS_EXECUTED',
        ('source_preserved','input_pins_preserved','whole_roots_preserved','execution_view_preserved',
         'original_sdk_preserved','original_official_upstream_preserved','official_source_copy_unmodified',
         'acquisition_source_preserved'))
    need(native.get('schema')=='uwvm-unitree-noetic-native-products-v1' and native.get('qualified_primary_translation_units')==3 and
         native.get('source')==str(frame.source) and native.get('source_manifest_sha256')==frame.report['source_manifest_sha256'],
         'fresh complete-current-source Noetic producer required')
    family=native['family'];evidence.retained(family)
    need(family['name']==FAMILY and family['ros_version']==1 and family['message_count']==11,'complete original ROS1 family required')
    profile=frame.source/'sdk/robots/unitree/profiles/unitree_legged_msgs.json'
    need(evidence.pin(family['profile'])==frame.pin(profile),'actual current original eleven-type profile required')
    declared,_=frame.common.document(profile)
    need(declared['family']==FAMILY and declared['ros_version']==1 and declared['default_authority']==[] and
         declared['commit']=='b989870124913091fbe75e0dcfb047eb4ca00a09' and len(declared['definitions'])==11,'unchanged official legacy profile required')
    expected={row['type'] for row in declared['definitions']}
    exported=evidence.document(family['native_metadata'])
    need(len(expected)==11 and exported['format']=='uwvm-ros-official-bounded-native-v2' and exported['ros_version']==1 and
         set(exported['types'])==expected,'actual native exporter must cover all eleven messages')
    identities={name:row['native_identity'] for name,row in exported['types'].items()}
    need(all(re.fullmatch('[0-9a-f]{32}',value) for value in identities.values()),'actual complete MD5 identities required')
    mapping=family['native_export_mapping']
    need(mapping['pidfd_owned'] is True and mapping['returncode']==0 and mapping['all_file_mappings'] and
         mapping['gate']=='COOPERATIVE_SIGSTOP_SAME_BIRTH_PIDFD_CONT_THEN_SINGLE_I' and
         mapping['actual_executable']==native['products']['identity_exporter']['binary'] and mapping['stdout']==family['native_metadata'],
         'genuine stopped owned native exporter result required')
    generation=evidence.document(family['generation']);directory=Path(family['generated_directory'])
    need(directory.resolve(strict=True)==directory and any(directory==r or directory.is_relative_to(r) for r in frame.roots),
         'complete actual generated directory must be an inventoried input')
    need(Path(family['generation']['path']).parent==directory and generation['format']=='uwvm-ros-custom-bounded-v2' and
         generation['status']=='GENERATED_SOURCE_ONLY' and generation['native_registry_emitted'] is True and
         generation['implicit_permissions']==[] and len(generation['records'])==len(generation['types'])==11 and
         {row['type']:row['native_identity'] for row in generation['records']}==identities and
         {row['type']:row['native_identity'] for row in generation['types']}==identities and
         all(row['ros_version']==1 and 0<=row['minimum_wire_bytes']<=row['maximum_wire_bytes']<=1536 for row in generation['records']) and
         generation['packages']=={'ros1':[FAMILY],'ros2':[]},'complete native-bound ROS1 generator contract required')
    codecs={row['type']:row['adapter_codec_identity'] for row in generation['records']}
    need(all(re.fullmatch('UWVMROS1_[0-9a-f]{64}',value) for value in codecs.values()) and
         all(row['adapter_codec_identity']==codecs[row['type']] for row in generation['types']),'complete canonical codec identities required')
    for key,relative in (('generator_sha256','sdk/tools/custom_types.py'),('bounded_generator_sha256','sdk/custom-types/bounded_profile.py')):
        need(generation[key]==frame.pin(frame.source/relative)['sha256'],'actual current unchanged emitter required')
    catalog=evidence.document(family['catalog']);binding=evidence.document(family['binding_receipt'])
    need(catalog['format']=='uwvm-ros-custom-bounded-v2' and
         {row['type']:row['native_identity'] for row in catalog['types']}==identities and
         all(row['ros_version']==1 for row in catalog['types']) and
         binding['status']=='NATIVE_IDENTITY_BINDING_SOURCE_ONLY' and binding['implicit_permissions']==[] and
         binding['official_export']=={k:family['native_metadata'][k] for k in ('path','bytes','sha256')} and
         binding['output']=={k:family['catalog'][k] for k in ('path','bytes','sha256')},'actual native catalog/binding receipt differs')
    need(generation['source_inputs']['catalog']=={k:family['catalog'][k] for k in ('bytes','sha256')},'generation must bind the actual complete catalog')
    definitions=generation['source_inputs']['definitions'];need(len(definitions)==11,'all eleven actual official message bodies required')
    for item in definitions:
        rel=Path(item['file']);need(not rel.is_absolute() and '..' not in rel.parts,'safe official definition path required')
        pin=frame.pin(Path(family['catalog']['path']).parent/rel)
        need(all(pin[k]==item[k] for k in ('bytes','sha256')),'actual official definition changed')
    need(set(generation['outputs'])==set(family['all_generated_outputs'])==OUTPUTS,'all eleven generated language/provider files required')
    outputs={}
    for name in sorted(OUTPUTS):
        pin=evidence.pin(family['all_generated_outputs'][name]);need(pin==frame.pin(directory/name) and
             generation['outputs'][name]=={k:pin[k] for k in ('bytes','sha256')},'actual complete generated output changed')
        outputs[name]=pin
    need(family['registry']==family['all_generated_outputs']['manifest.hpp'] and
         family['native_header']==family['all_generated_outputs']['native_ros1.hpp'],'native provider file association differs')
    return evidence,{'native_products':frame.inputs[selection['native_products']],'generation':evidence.pin(family['generation']),
        'profile':evidence.pin(family['profile']),'catalog':evidence.pin(family['catalog']),
        'binding_receipt':evidence.pin(family['binding_receipt']),'native_metadata':evidence.pin(family['native_metadata']),
        'all_generated_outputs':outputs,'identities':identities,'codec_identities':codecs,'directory':str(directory)}


def official_cpp(frame,evidence,label,values,support):
    """Reuse only the independently qualified official object, without GO data.

    The successful Typed14 receipt is provenance for this explicit library
    input. Its original source, cgroup and qualification remain unchanged. No
    other Typed14 product or historical network metadata becomes a build input.
    """
    packet=evidence.selected(label)
    need(packet.get('status')=='PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY' and
         packet.get('compiled_command_products')==14 and packet.get('retained_command_products')==9 and
         packet.get('fresh_command_products')==5 and packet.get('actual_Runtime_cases_executed')==0 and
         all(packet.get(k) is True for k in ('source_preserved','input_pins_preserved','whole_roots_preserved')) and
         not packet.get('finalization_errors'),'actual completed binary-only Typed14 prerequisite required')
    cleanup=packet.get('cleanup');cleanup=cleanup if type(cleanup) is list else [cleanup]
    need(cleanup and all(row.get('status')=='PASS' and not row.get('remaining') and not row.get('errors') for row in cleanup),'actual Typed14 cleanup required')
    qualification=packet['official_cpp_object']['qualification'];obj=evidence.pin(qualification['object'])
    report=evidence.document(qualification['report']);proof=report['overlay']
    need(report['kind']=='wasi-cpp-official-termination-overlay' and report['status']=='PASS_OFFICIAL_CPP_TERMINATION_OBJECT_PREPARATION_ONLY' and
         all(report.get(k) is True for k in ('source_preserved','input_pins_preserved','whole_roots_preserved','overlay_products_preserved')) and
         not report.get('finalization_errors') and qualification['policy']==proof['policy']=='OFFICIAL_LIBCXXABI_BAREMETAL_ABORT_TU_AND_HEADER_VERBOSE_TRAP_V1' and
         qualification['Guest_compile_flags']==proof['Guest_compile_flags']==['-UNDEBUG',TRAP] and
         qualification['official']['commit']==proof['official']['commit']=='4434dabb69916856b824f68a64b029c67175e532' and
         proof['official']['no_assertion_site_in_compiled_TU'] is True and proof['runtime_executed'] is False and proof['SDK_modified'] is False,
         'actual unchanged official-source termination object qualification required')
    need(evidence.pin(proof['object'])==obj and proof['compiler']['physical']==str(Path(values['WASI_CLANGXX']).resolve(strict=True)) and
         proof['sysroot']==values['WASI_SYSROOT'],'official object must match actual selected WASI compiler/sysroot')
    producer=evidence.pin(report['producer'])
    need(producer==evidence.pin(qualification['producer']) and producer['bytes']==27601 and
         producer['sha256']=='31340e874df8848a9e7710d824483ae4588fbb9c320fe79ca12f056a33407981',
         'unchanged actual independently qualified official-object producer required')
    evidence.retained(proof['products']);evidence.pin(qualification['consumer_qualifier'])
    evidence.retained(proof['official']['files']);evidence.pin(proof['official']['lock'])
    command=proof['compile'];argv=command['argv'];dep=evidence.pin(proof['dependency_file'])
    need(command['returncode']==0 and argv.count('-MD')==argv.count('-MF')==argv.count('-MT')==argv.count('-c')==argv.count('-o')==1 and
         '-DNDEBUG' in argv and '-DLIBCXXABI_BAREMETAL' in argv and '-UNDEBUG' not in argv and
         argv[argv.index('-MF')+1]==dep['path'] and argv[argv.index('-MT')+1]==obj['path'] and argv[argv.index('-o')+1]==obj['path'],
         'actual isolated official full-MD compile association required')
    source=evidence.pin(proof['official']['files']['libcxxabi/src/abort_message.cpp'])
    need(argv[argv.index('-c')+1]==source['path'],'official abort primary source differs')
    actual=support.full_md(frame,Path(dep['path']),Path(obj['path']),Path(source['path']),Path(command['cwd']))
    need(actual==[evidence.pin(pin) for pin in command['dependencies']],'actual official object dependency closure changed')
    return {'object':obj,'original_qualification':qualification,'original_overlay_report':evidence.pin(qualification['report']),
            'actual_full_MD':actual,'current_Typed14_receipt':frame.inputs[label],
            'scope':'QUALIFIED_OFFICIAL_LIBRARY_INPUT_ONLY_NO_GO_OR_HG_GUEST_REUSE'}



def wasi_cpp_archive_profile(sysroot, archives):
    """Validate an explicitly selected C++ pair, never discover a preferred one.

    WASI SDK releases may place C++ libraries directly in the nonthread WASIp1
    library directory or in its noeh/eh subdirectory. These are distinct linker
    profiles: both explicitly supplied physical archives must share one exact
    directory. LTO, thread, WASIp2, mixed and foreign-SDK pairs are refused.
    The profile name describes library layout, not the C++ language exception
    flags. The real driver/link trace must independently consume this pair.
    """
    sysroot=Path(sysroot)
    need(sysroot.is_absolute() and sysroot.resolve(strict=True)==sysroot,
         'physical selected WASI sysroot required')
    need(type(archives) is dict and set(archives)=={'libcxx','libcxxabi'},
         'two explicit selected C++ archive roles required')
    paths={key:Path(value) for key,value in archives.items()}
    need(all(path.is_absolute() and path.resolve(strict=True)==path and path.is_file()
             for path in paths.values()),'physical ordinary C++ archives required')
    need(paths['libcxx'].name=='libc++.a' and paths['libcxxabi'].name=='libc++abi.a' and
         paths['libcxx'].parent==paths['libcxxabi'].parent,'same-profile C++ archive pair required')
    base=sysroot/'lib/wasm32-wasip1';parent=paths['libcxx'].parent
    layouts={base:'flat',base/'noeh':'noeh',base/'eh':'eh'}
    need(parent in layouts,'C++ archives outside exact non-LTO nonthread WASIp1 profile')
    return layouts[parent]


def selected_tools(frame,selection,support):
    tools=('wasi_clang','wasi_clangxx','wasm_tools','python','rustc','tinygo','go','node','asc_js',
           'wasi_libc','wasi_libm','wasi_full_printscan','wasi_builtins','wasi_libcxx','wasi_libcxxabi')
    paths=('wasi_sysroot','wasi_resource_dir','wasm_ld_alias','rust_sysroot','tinygo_root','go_root','quickjs_root')
    need(type(selection['tools']) is dict and set(selection['tools'])==set(tools) and
         type(selection['paths']) is dict and set(selection['paths'])==set(paths),'complete closed compiler/archive/path selection required')
    values={key.upper():str(frame.input(label)) for key,label in selection['tools'].items()}
    for key,token in selection['paths'].items():
        path=Path(token);need(path.is_absolute() and path.parent.resolve(strict=True)==path.parent,'canonical explicit SDK path required')
        physical=support.contained(frame,path);need(physical.is_file() if key=='wasm_ld_alias' else physical.is_dir(),'actual SDK directory/tool required')
        values[key.upper()]=str(path if key=='wasm_ld_alias' else physical)
    values['WASM_LD']=values['WASM_LD_ALIAS'];values['QUICKJS']=values['QUICKJS_ROOT']
    aliases=[]
    for key in ('WASI_CLANG','WASI_CLANGXX','WASM_TOOLS','PYTHON','RUSTC','TINYGO','GO','NODE','WASM_LD'):
        alias=support.alias(values[key]);need(os.access(values[key],os.X_OK),'actual executable tool required')
        support.contained(frame,alias['physical']);frame.pin(alias['physical']);aliases.append(alias)
    need(re.fullmatch('wasm-ld(?:-[0-9.]+)?',Path(values['WASM_LD']).name),'actual wasm-ld multicall spelling required')
    sysroot=Path(values['WASI_SYSROOT'])
    # C archives retain their exact direct nonthread WASIp1 paths. C++ has
    # a separately selected pair because SDK33 partitions it into noeh/eh.
    for key,name in (('WASI_LIBC','libc.a'),('WASI_LIBM','libm.a'),('WASI_FULL_PRINTSCAN','libc-printscan-long-double.a')):
        need(Path(values[key])==(sysroot/'lib/wasm32-wasip1'/name).resolve(strict=True),'actual C archives must match selected WASI SDK')
    values['WASI_CPP_ARCHIVE_PROFILE']=wasi_cpp_archive_profile(sysroot,
        {'libcxx':values['WASI_LIBCXX'],'libcxxabi':values['WASI_LIBCXXABI']})
    for key in ('WASI_LIBCXX','WASI_LIBCXXABI'):
        path=Path(values[key]);need(support.contained(frame,path)==path and
             frame.pin(path)==frame.inputs[selection['tools'][key.lower()]],
             'explicit C++ archive must retain its actual whole-SDK input full-seven')
    need(Path(values['RUSTC']).resolve(strict=True)==Path(values['RUST_SYSROOT'])/'bin/rustc' and
         (Path(values['RUST_SYSROOT'])/'lib/rustlib/wasm32-unknown-unknown/lib').is_dir(),
         'complete actual unknown-target Rust SDK required; no rustup/network/WASI substitution')
    need(Path(values['TINYGO']).resolve(strict=True)==(Path(values['TINYGO_ROOT'])/'bin/tinygo').resolve(strict=True) and
         Path(values['GO']).resolve(strict=True)==(Path(values['GO_ROOT'])/'bin/go').resolve(strict=True),
         'actual complete offline TinyGo and Go packages required')
    return values,aliases


def stage_product(frame,support,row,cell,family):
    """Copy full SDK/application trees with their real relative include paths."""
    stage=cell/'source';stage.mkdir(mode=0o700);associations=[];language=row['language']
    roots=['sdk/include',ENTRY]
    roots+=['sdk/rust'] if language=='rust' else ['sdk/tinygo'] if language=='tinygo' else ['sdk/assemblyscript'] if language=='assemblyscript' else ['sdk/javascript','sdk/abi','sdk/ros1','sdk/common'] if language=='javascript' else []
    for relative in roots:support.copy_tree(frame,frame.frozen,relative,stage,associations)
    suffixes={'c':('lowlevel.c','values_c.h'),'cpp':('lowlevel.cpp','values_c.h'),
              'rust':('lowlevel.rs','values.rs'),'tinygo':('lowlevel.go','values.go','go.mod'),
              'assemblyscript':('lowlevel.ts','values.ts'),'javascript':('lowlevel.mjs','values.mjs')}
    for name in suffixes[language]:
        associations.append(support.copy_file(frame,frame.source/EXAMPLES/name,stage/EXAMPLES/name,frame.frozen))
    # The complete native-bound family remains available in every language
    # stage. A compiler may eliminate unused records, but source staging does
    # not replace their complete definitions with a two-message projection.
    destination=stage/EXAMPLES
    for name,pin in family['all_generated_outputs'].items():
        associations.append(support.copy_file(frame,Path(pin['path']),stage/'native-family'/name))
    local={'c':('bounded_records.h','bounded_interfaces.h'),
           'cpp':('bounded_records.h','bounded_records.hpp','bounded_interfaces.h','bounded_interfaces.hpp'),
           'rust':('bounded_records.rs',),'javascript':('bounded_records.mjs',)}
    for name in local.get(language,()):
        associations.append(support.copy_file(frame,Path(family['all_generated_outputs'][name]['path']),destination/name))
    if language=='tinygo':
        for name in ('command.go','protocol.go'):
            associations.append(support.copy_file(frame,frame.source/ENTRY/name,destination/name,frame.frozen))
        associations.append(support.copy_file(frame,Path(family['all_generated_outputs']['bounded_records.go']['path']),destination/'customtypes/bounded_records.go'))
    elif language=='assemblyscript':
        associations.append(support.copy_file(frame,Path(family['all_generated_outputs']['bounded_records.ts']['path']),stage/'sdk/custom-types/legacy-family/bounded_records.ts'))
    return stage,associations


def binary_boundary(frame,support,inspector,schema,artifact,entry):
    """Check actual imports, owned memory and local entry after full validation."""
    raw,pin=support.read(artifact,64*MIB);need(frame.pin(artifact)==pin,'actual Wasm changed')
    table=support.wasm_structure(raw,inspector)
    expected={r['name']:([p['type'] for p in r['params']],r['results']) for r in schema['modules']['uwvm_ros1_v1']['functions']}
    wasi={'fd_read':(['i32']*4,['i32']),'fd_write':(['i32']*4,['i32']),'proc_exit':(['i32'],[])}
    seen=set();ros=set();stdio=set()
    for item in table['imports']:
        key=item['module'],item['name'];need(item['kind']==0 and key not in seen,'unique actual function-only imports required');seen.add(key)
        signature=item['params'],item['results']
        if key[0]=='uwvm_ros1_v1':need(expected.get(key[1])==signature,'actual ROS1 ABI differs');ros.add(key[1])
        else:need(key[0]=='wasi_snapshot_preview1' and wasi.get(key[1])==signature,'foreign or optional WASI import forbidden');stdio.add(key[1])
    need('capability_open' in ros and stdio==set(wasi),'actual SDK and basic command stdio required')
    need(table['start_function'] is None and len(table['memories'])==1 and table['memories'][0]['imported'] is False and
         table['memories'][0]['maximum']==1024 and 0<=table['memories'][0]['minimum']<=1024 and
         table['exports'].get('memory')=={'kind':2,'index':0},'one own64MiB memory0 and no implicit start required')
    function=table['exports'].get(entry);count=len(table['imports'])
    need(function and function['kind']==0 and function['params']==function['results']==[] and function['index']>=count,'actual same-module local void entry required')
    return {'artifact':pin,'imports':table['imports'],'memories':table['memories'],'entry_export':entry,
            'entry':function,'local_entry':function['index']-count,'implicit_start':False,'default_authority':[]}


def build(frame,api,unitree,selection):
    independent=None
    if type(selection) is dict and type(selection.get('format_version')) is int and selection['format_version']==2:
        # The independent object's own terminal receipt is sufficient library
        # provenance. No Typed14 main report or main product is read on this route.
        unitree.exact(selection,('format_version','kind','native_products','official_cpp_report','tools','paths','restart_mappings','build_seconds'),'independent-object legacy compiler selection')
        need(selection['kind']=='uwvm-unitree-legacy-command-build-selection-v2' and
             type(selection['official_cpp_report']) is str and selection['official_cpp_report'] and
             type(selection['build_seconds']) is int and 600<=selection['build_seconds']<=14400,
             'closed finite independent-object six-command selection required')
        independent=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/independent_cpp_object.py','legacy_independent_cpp_library')
    else:
        unitree.exact(selection,('format_version','kind','native_products','typed_build','tools','paths','restart_mappings','build_seconds'),'legacy compiler selection')
        need(selection['format_version']==1 and selection['kind']=='uwvm-unitree-legacy-command-build-selection-v1' and
             type(selection['build_seconds']) is int and 600<=selection['build_seconds']<=14400,'closed finite fresh six-command selection required')
    need(not frame.config['upstreams'] and set(frame.config['environment'])<={'PATH','LD_LIBRARY_PATH'} and
         all(frame.config[k] is None for k in ('simulation','benchmark','profiler')) and frame.limits['jobs']==1 and
         frame.limits['rss_bytes']<=2*GIB,'serial compiler-only selected package environment required')
    support=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/command_build_support.py','legacy_fresh_command_support')
    original_guard=frame.guard;clock={'start':time.monotonic(),'cleanup':None};limit=selection['build_seconds'];aliases=[]
    def guard():
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024<=GIB,'compiler coordinator RSS exceeds1GiB')
        if clock['cleanup'] is not None:
            need(time.monotonic()-clock['cleanup']<600,'finite owned cleanup grace exhausted')
            frame.common.cgroup_preserved(frame.before,frame.common.cgroup_state(frame.config['cgroup']))
        else:
            need(time.monotonic()-clock['start']<limit,'finite six-language build/preservation deadline exhausted');original_guard()
        need(all(support.alias(row['path'])==row for row in aliases),'selected compiler/linker alias changed')
    frame.guard=guard
    inspector=api.frozen_module(frame.source,frame.frozen,'sdk/tools/check_wasm_imports.py','legacy_actual_binary_reader')
    values,aliases=selected_tools(frame,selection,support)
    evidence,family=current_family(frame,api,selection)
    overlay=official_cpp(frame,evidence,selection['typed_build'],values,support) if independent is None else \
        independent.qualify(frame,evidence,selection['official_cpp_report'],values,support)
    recipes,rp=support.doc(frame.source/RECIPE);support.source_pin(frame,frame.frozen,frame.source/RECIPE)
    need(recipes['kind']=='uwvm-unitree-legacy-same-main-command-recipes-v1' and recipes['status']=='SOURCE_RECIPES_NOT_EXECUTED' and
         recipes['default_authority']==[] and len(recipes['products'])==6 and tuple(r['language'] for r in recipes['products'])==LANGUAGES,
         'exact maintained six-language recipe inventory required')
    for row in recipes['products']:
        need(row['id']==row['language']+'-legacy-lowlevel-command' and row['family']==FAMILY and row['backend']==1 and
             row['family_code']==4 and row['complete_record_count']==11 and row['actual_example']==EXAMPLES+'/lowlevel.'+
             {'c':'c','cpp':'cpp','rust':'rs','tinygo':'go','assemblyscript':'ts','javascript':'mjs'}[row['language']] and
             row['entry_export']==('legacy_command_entry' if row['language']=='assemblyscript' else '_start') and
             row['ROS_authority']==row['optional_WASI']==[] and row['own_memory'] is True and row['implicit_start'] is False and
             row['maximum_memory_pages']==1024 and row['poll_limit']==4096,'actual full same-main recipe association differs')
        expected_jobs=['entry','example','link'] if row['language'] in ('c','cpp') else ['sdk','command'] if row['language']=='rust' else ['prepare'] if row['language']=='javascript' else ['command']
        need([job['id'] for job in row['commands']]==expected_jobs,'exact real recipe job roles required')
        if row['language']=='cpp':need(row['commands'][0]['argv']==recipes['products'][0]['commands'][0]['argv'],'C/C++ must share the identical real C entry recipe')
    schema,_=support.doc(frame.source/'sdk/abi/abi-v1.json');support.source_pin(frame,frame.frozen,frame.source/'sdk/abi/abi-v1.json')
    owner=frame.common.SerialRunner(frame.out,frame.limits,guard)
    def close():
        clock['cleanup']=time.monotonic()
        try:return owner.close()
        finally:clock['cleanup']=None
    frame.actors.append(types.SimpleNamespace(close=close))
    env={**frame.environment,'CARGO_NET_OFFLINE':'true','RUSTUP_AUTO_INSTALL':'0','GOTOOLCHAIN':'local','GOPROXY':'off',
         'GOSUMDB':'off','GOENV':'off','GOWORK':'off','GOFLAGS':'-mod=readonly','GOMAXPROCS':'1','GOMEMLIMIT':'192MiB',
         'GOROOT':values['GO_ROOT'],'TINYGOROOT':values['TINYGO_ROOT'],'BINARYEN_CORES':'1','RAYON_NUM_THREADS':'1','MAKEFLAGS':'-j1'}
    env['PATH']=':'.join(dict.fromkeys([str(Path(values[k]).parent) for k in ('GO','TINYGO','RUSTC','WASM_LD','WASI_CLANG','NODE')]+env['PATH'].split(':')))
    for key in ('PATH','LD_LIBRARY_PATH'):
        for token in env.get(key,'').split(':') if key in env else []:
            path=Path(token);need(token and path.is_absolute() and path.is_dir(),'clean physical offline tool search path required');support.contained(frame,path)
    for key,name in (('GOCACHE','go-cache'),('GOPATH','go-path'),('GOTMPDIR','go-tmp')):
        path=frame.out/name;path.mkdir(mode=0o700);env[key]=str(path)
    frame.report.update(kind=KIND,products=[],commands=owner.commands,planned_products=6,compiled_command_products=0,
        actual_Runtime_cases_executed=0,granted_rights=[],default_ROS_authority=[],optional_WASI_rights=[],
        full_native_family=family,official_cpp_object=overlay,source_recipes=rp,tool_invocation_aliases=aliases,
        scope='SIX_FRESH_ROS1_SAME_MAIN_COMMAND_COMPILATIONS_AND_BINARY_VALIDATION_ONLY')
    def run(label,argv,cwd,dependencies=None,**extra):
        guard();memory=int((Path(frame.config['cgroup']['expected_leaf'])/'memory.current').read_text())
        need(memory+frame.limits['rss_bytes']+2*GIB<60*GIB,'compiler RSS and parent reserve require actual aggregate headroom below60GiB')
        available={line.split(':',1)[0]:line.split(':',1)[1].strip() for line in Path('/proc/meminfo').read_text().splitlines()}
        need(int(available['MemAvailable'].split()[0])*1024>=frame.limits['rss_bytes']+10*GIB,'host must retain explicit8GiB reserve plus compiler/parent budget')
        actual=support.run(frame,owner,label,argv,cwd,env)
        return {**actual,'dependencies':dependencies or [],'cwd':str(cwd),**extra}
    def compiled(label,argv,cwd,source,obj,dep):
        record=run(label,argv,cwd);deps=support.full_md(frame,dep,obj,source,cwd)
        record.update(source=frame.pin(source),object=frame.pin(obj),dependency_file=frame.pin(dep),dependencies=deps,full_MD=deps)
        return record
    for row in recipes['products']:
        guard();cell=frame.out/row['id'];cell.mkdir(mode=0o700);stage,associations=stage_product(frame,support,row,cell,family)
        language=row['language'];artifact=cell/'guest.wasm';commands=[];entry=None;details={};validation=None
        mapping={**values,'SOURCE':str(frame.source),'OUT':str(cell),'STAGE':str(stage),'GENERATED':family['directory'],
                 'OFFICIAL_CPP_ABORT_OBJECT':overlay['object']['path'],'WHY_EXTRACT':str(cell/'why-extract.tsv'),
                 'ACTUAL_GO_ROOT':values['GO_ROOT'],'ACTUAL_TINYGO_ROOT':values['TINYGO_ROOT']}
        if language in ('c','cpp','rust'):
            original=recipes['products'][0]['commands'][0];obj,dep=cell/'entry.o',cell/'entry.d'
            local={**mapping,'OBJECT':str(obj),'DEP':str(dep)};argv=[support.expand(x,local) for x in original['argv']]
            entry=compiled(row['id']+'-entry',argv,cell,frame.source/ENTRY/'entry.c',obj,dep);commands.append(entry)
            mapping['ENTRY_OBJECT']=str(obj)
        if language in ('c','cpp'):
            obj,dep=cell/'example.o',cell/'example.d';mapping.update(OBJECT=str(obj),DEP=str(dep),EXAMPLE_OBJECT=str(obj))
            source=stage/row['actual_example'];argv=[support.expand(x,mapping) for x in row['commands'][1]['argv']]
            if language=='cpp':need(argv.count('--target=wasm32-wasip1')==1,
                'selected C++ archive pair requires the exact nonthread WASIp1 compile target')
            record=compiled(row['id']+'-example',argv,cell,source,obj,dep);commands.append(record)
            generated=stage/EXAMPLES/('bounded_records.hpp' if language=='cpp' else 'bounded_records.h')
            need(frame.pin(generated) in record['dependencies'],'actual example MD omitted full newly generated module')
            argv=[support.expand(x,mapping) for x in row['commands'][2]['argv']]
            if language=='cpp':need(argv.count('--target=wasm32-wasip1')==1,
                'selected C++ archive pair requires the exact nonthread WASIp1 link target')
            linked=run(row['id']+'-link',argv,cell)
            links=support.trace_inputs(frame,linked,cell)
            need(frame.pin(obj) in links and entry['object'] in links,'actual link trace omitted example or same-main entry')
            linked['dependencies']=links;commands.append(linked);details['link_inputs']=links
            if language=='cpp':
                pair={key:frame.pin(values[key]) for key in ('WASI_LIBCXX','WASI_LIBCXXABI')}
                need(all(pin in links for pin in pair.values()),
                     'actual C++ link trace must contain both explicitly selected same-profile archives')
                details['selected_cpp_archives']={'profile':values['WASI_CPP_ARCHIVE_PROFILE'],
                    'target':'wasm32-wasip1','archives':pair,'actual_link_trace_contains_both':True,
                    'selection':'EXPLICIT_INPUT_PAIR_NOT_DISCOVERED_OR_SUBSTITUTED'}
                details['official_termination']=support.cpp_link_evidence(frame,support,linked,cell/'why-extract.tsv',overlay,links)
        elif language=='rust':
            linker=support.prepare_rust_linker(frame,cell,values,entry,support)
            mapping['OWNED_REAL_LINKER_TRACE_WRAPPER']=linker['wrapper']['path'];dependencies=[]
            for job in row['commands']:
                argv=[support.expand(x,mapping) for x in job['argv']];record=run(row['id']+'-'+job['id'],argv,cell)
                dep=cell/('sdk.d' if job['id']=='sdk' else 'command.d');target=cell/('libroswasm.rlib' if job['id']=='sdk' else 'guest.wasm')
                src=frame.source/'sdk/rust/src/lib.rs' if job['id']=='sdk' else stage/row['actual_example']
                proof=support.rust_dependencies(frame,dep,target,src,cell);record['dependencies']=proof['dependencies'];record['rust_dep_info']=proof
                commands.append(record);dependencies.append(proof)
                if job['id']=='command':details['actual_rust_link']=support.qualify_rust_linker(frame,linker,support)
            details.update(sdk_rlib=frame.pin(cell/'libroswasm.rlib'),rust_dependency_proofs=dependencies)
        elif language=='tinygo':
            target,proof=support.derive_tinygo_target(frame,values,cell);mapping['OWNED_BOUNDED_WASM_UNKNOWN_TARGET']=str(target)
            job=row['commands'][0];argv=[support.expand(x,mapping) for x in job['argv']]
            need(argv.count('-buildmode=default')==1 and argv.count('-tags=ros1')==1,'actual ROS1 TinyGo command startup required')
            commands.append(run(row['id']+'-command',argv,Path(support.expand(job['cwd'],mapping))))
            need(frame.pin(target)==proof['owned'] and frame.pin(proof['original']['path'])==proof['original'],'derived target or original SDK changed')
            details.update(derived_target=proof,language_dependency_scope='COMPLETE_OFFLINE_GO_TINYGO_AND_STAGED_SOURCE_CATALOGS_NOT_CLANG_MD')
        elif language=='assemblyscript':
            argv=[support.expand(x,mapping) for x in row['commands'][0]['argv']]
            need(argv[argv.index('--exportStart')+1]=='legacy_command_entry','actual AssemblyScript command initializer required')
            commands.append(run(row['id']+'-command',argv,stage))
            details['language_dependency_scope']='COMPLETE_NODE_ASSEMBLYSCRIPT_PACKAGES_AND_STAGED_SOURCE_CATALOGS_NOT_CLANG_MD'
        else:
            job=row['commands'][0]
            # The actual preparer checks HERE == source-root/sdk/javascript/guest.
            # Execute its exact copied body from that stage and use the equally
            # copied original manifest; every later prepared argv is unchanged.
            argv=[support.expand(x,{**mapping,'SOURCE':str(stage)}) for x in job['argv']]
            commands.append(run(row['id']+'-prepare',argv,cell));prepared=cell/'prepared';metadata,mp=support.doc(prepared/'report.json')
            manifest,_=support.doc(frame.source/ENTRY/'javascript-manifest.json')
            need(metadata['schema']=='uwvm-embedded-javascript-preparation-v1' and metadata['status']=='PREPARED_NOT_COMPILED' and
                 metadata['compiler_executed'] is False and metadata['implicit_permissions']==[] and metadata['manifest']==manifest and
                 manifest['backend']==1 and manifest['mode']=='command' and manifest['exports']==[] and len(manifest['modules'])==15 and
                 [job['id'] for job in metadata['jobs']]==['compile-'+str(i) for i in range(13)]+['link','validate'],
                 'actual closed backend1 command preparation with13 TUs required')
            objects=[]
            for job in metadata['jobs']:
                need(job['cwd']==str(prepared),'actual prepared working directory differs')
                if 'source' in job:
                    source=Path(job['source']['path']);obj=Path(job['artifact']);dep=Path(job['dependencies']);argv=job['argv']
                    need(source.is_relative_to(stage) or source.is_relative_to(prepared),'prepared C source outside selected stage')
                    source_qualification=javascript_prepared_file(frame,job['source'])
                    need(obj.parent==prepared/'objects' and dep==obj.with_suffix('.d') and
                         argv[0]==values['WASI_CLANG'] and '-MD' in argv and '-MMD' not in argv and '-DUWVM_JS_BACKEND=1' in argv and
                         all(x in argv for x in ('-UNDEBUG','-fno-fast-math','-ffp-contract=off')) and
                         argv[argv.index('-c')+1]==str(source) and argv[argv.index('-o')+1]==str(obj) and
                         argv[argv.index('-MF')+1]==str(dep) and argv[argv.index('-MT')+1]==str(obj),'actual JS source/full-MD/SDK association required')
                    record=compiled(row['id']+'-'+job['id'],argv,prepared,source,obj,dep)
                    record['javascript_prepared_source']=source_qualification
                    objects.append(obj);commands.append(record)
                elif job['id']=='link':
                    argv=job['argv'];need(len(objects)==13 and argv[0]==values['WASI_CLANG'] and
                         all(str(obj) in argv for obj in objects) and all(values[key] in argv for key in ('WASI_LIBM','WASI_FULL_PRINTSCAN','WASI_LIBC','WASI_BUILTINS')) and
                         argv.index(values['WASI_FULL_PRINTSCAN'])<argv.index(values['WASI_LIBC']) and '-Wl,--trace' in argv and
                         argv[argv.index('-o')+1]==str(prepared/'guest.wasm'),'actual full engine/archive command link required')
                    record=run(row['id']+'-link',argv,prepared);links=support.trace_inputs(frame,record,prepared)
                    need(all(frame.pin(obj) in links for obj in objects),'actual JS linker trace omitted engine object')
                    record['dependencies']=links;commands.append(record);details['link_inputs']=links
                else:
                    need(job['id']=='validate' and job['argv']==[values['WASM_TOOLS'],'validate',str(prepared/'guest.wasm')],'actual prepared full validator required')
                    # Exactly this prepared job is executed below, once. No
                    # invented validation row substitutes for real exit zero.
                    validation=run(row['id']+'-validate',job['argv'],prepared)
            artifact=prepared/'guest.wasm';details.update(embedded_preparation=frame.pin(mp['path']),
                module_table=frame.pin(prepared/'module_table.c'),scalar_trampolines=frame.pin(prepared/'scalar_trampolines.c'),
                preparation_inputs=metadata['inputs'],javascript_manifest=frame.pin(stage/ENTRY/'javascript-manifest.json'),
                recipe_adaptation={'SOURCE_for_preparer':str(stage),'reason':'Exact staged preparer HERE/source-root invariant; original source bytes unchanged',
                                   'prepared_compiler_link_validator_argv_modified':False})
            # Inputs include the exact preparer/engine/embedded modules and all
            # generated C sources. Reopen every original closed8 observation
            # after the real compile/link/validator jobs, preserving raw mode.
            details['javascript_preparation_input_qualifications']=[
                javascript_prepared_file(frame,pin) for pin in metadata['inputs']]
        if language!='javascript':
            argv=[support.expand(x,{**mapping,'OUT':str(artifact.parent)}) for x in row['validation']]
            need(argv==[values['WASM_TOOLS'],'validate',str(artifact)],'actual exact full validator required')
            validation=run(row['id']+'-validate',argv,artifact.parent)
        need(validation is not None and validation['status']=='PASS' and validation['returncode']==0 and
             validation['argv']==[values['WASM_TOOLS'],'validate',str(artifact)],'actual exact full validation must complete successfully')
        boundary=binary_boundary(frame,support,inspector,schema,artifact,row['entry_export'])
        source_bindings=[];generated_bindings=[];consumed={}
        for association in associations:
            old,new=association['source'],association['copy'];path=Path(old['path'])
            need(all(old[k]==new[k] for k in ('bytes','sha256')),'actual source-copy byte relation changed')
            if path.is_relative_to(frame.source):
                relative=str(path.relative_to(frame.source))
                need(frame.frozen.get(relative)=={k:old[k] for k in ('bytes','sha256')},'original staged source absent from its complete manifest')
                source_bindings.append({'relative':relative,'original':old,'compiled':new})
            else:
                matches=[name for name,pin in family['all_generated_outputs'].items() if pin==old]
                need(len(matches)==1,'staged external source lacks exact complete-family association')
                generated_bindings.append({'name':matches[0],'generation':family['generation'],'original':old,'compiled':new})
        by_copy={item['compiled']['path']:item for item in source_bindings}
        for command in commands:
            for pin in command.get('dependencies',[]):
                if Path(pin['path']).is_relative_to(frame.source):consumed[pin['path']]=pin
                elif pin['path'] in by_copy:
                    association=by_copy[pin['path']];need(pin==association['compiled'],'actual compiler input differs from staged source binding')
                    consumed[association['original']['path']]=association['original']
        basis='ACTUAL_CLANG_FULL_MD_OR_RUST_DEP_INFO_WITH_EXACT_STAGED_SOURCE_COUNTERPARTS'
        if language in ('tinygo','assemblyscript','javascript'):
            # These compilers/embedded module tables do not provide Clang MD
            # for Go/AS/JS language imports. Retain the complete explicit staged
            # source package as conservative input scope, without claiming that
            # every unused staged SDK file appeared in a compiler dependency.
            for association in source_bindings:consumed[association['original']['path']]=association['original']
            basis='COMPLETE_EXPLICIT_STAGED_LANGUAGE_PACKAGE_AND_MODULE_SCOPE_PLUS_ACTUAL_CLANG_MD_WHERE_AVAILABLE'
        actual_example=support.source_pin(frame,frame.frozen,frame.source/row['actual_example'])
        need(consumed.get(actual_example['path'])==actual_example,'actual application must be in the original compiler/module input closure')
        product={'id':row['id'],'language':language,'family':FAMILY,'backend':1,'entry_export':row['entry_export'],
            'artifact':frame.pin(artifact),'generation':family['generation'],'status':ROW_STATUS,
            'same_main_source_control':True,'preloaded_application':False,'cross_module_guard_modified':False,'granted_rights':[],
            'source':actual_example,
            'compilation_provenance':{'mode':'SIX_FRESH_CURRENT_SOURCE_LEGACY_COMMAND_BUILDS','producer':frame.pin(Path(__file__)),
                'source_manifest_sha256':frame.report['source_manifest_sha256'],'original_source':str(frame.source),
                'original_manifest':frame.pin(frame.manifest),'complete_original_source_state':frame.source_before,
                'original_recipe':rp,'native_family':family,'consumed_project_inputs':list(consumed.values()),'consumed_input_basis':basis,
                'language_module_scope':'Actual full Clang MD or Rust dep-info where available; otherwise the complete explicit staged language SDK/application/generated modules and complete selected tool package catalogs, without invented compiler MD.'},
            'validation':validation,'commands':commands,'owned_source_associations':associations,'source_bindings':source_bindings,
            'generated_bindings':generated_bindings,'binary_boundary':boundary,**details}
        receipt={**product,'format':'uwvm-unitree-legacy-same-main-command-guest-build-receipt-v1',
            'observed_execution':'NOT_EXECUTED','source_manifest_sha256':frame.report['source_manifest_sha256'],'current_source':str(frame.source)}
        product['receipt']=support.put(frame,cell/'build-receipt.json',json.dumps(receipt,indent=2)+'\n')
        frame.report['products'].append(product);frame.report['compiled_command_products']=len(frame.report['products']);frame.save()
    need(len(frame.report['products'])==6 and all(row['status']==ROW_STATUS for row in frame.report['products']) and
         len(owner.commands)==32 and all(row['status']=='PASS' and row['returncode']==0 for row in owner.commands),
         'all six fresh real commands and32 actual compiler/preparer/link/validator jobs required')
    products={}
    for pin in frame.pins:
        if Path(pin['path']).is_relative_to(frame.out):
            need(pin['path'] not in products or products[pin['path']]==pin,'owned product changed during build')
            products[pin['path']]=pin
    frame.report['product_pins']=list(products.values())
    frame.report['product_preservation_scope']='Every selected owned product above is also retained in Frame.input_pins and fully reopened by its terminal input_pins_preserved check.'
    frame.report['selected_input_bridges']=evidence.bridges
    return STATUS


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for name in ('source','manifest','config','out'):parser.add_argument('--'+name,type=Path,required=True)
    for name in ('manifest-sha256','config-sha256','selection-input'):parser.add_argument('--'+name,required=True)
    args=parser.parse_args();frame=None
    try:
        frozen,api,unitree=bootstrap(args);frame=owned_frame_type(unitree,api,frozen)(args)
        selection,pin=frame.common.document(frame.input(args.selection_input))
        need(pin==frame.inputs[args.selection_input],'actual selected compiler metadata changed')
        status=build(frame,api,unitree,selection)
        # Frame publishes its ordinary preservation/cleanup fields unchanged.
        # Its legacy exit helper recognizes PASS_* only; this lane's distinct
        # PREPARED_* contract therefore checks the actual published status.
        frame.finish(status)
        frame.report['products_preserved']=frame.report.get('input_pins_preserved') is True
        frame.save()
        return 0 if frame.report['status']==STATUS else 1
    except BaseException as error:
        if frame is not None:
            frame.report['traceback']=traceback.format_exc();frame.finish('FAIL',repr(error))
        else:print(traceback.format_exc(),file=sys.stderr)
        return 1


if __name__=='__main__':raise SystemExit(main())
