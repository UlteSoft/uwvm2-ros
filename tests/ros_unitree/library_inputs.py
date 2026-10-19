"""Finite copied library inputs; historical preparation is not current authority.

A preparation Frame must finish successfully before its report can be consumed.
The consumer opens only actual copied leaves and the compiler/SDK/MD inputs it
really uses. It does not reclassify the producer's old whole-root observations
as observations made during the consuming build. No process starts on import.
"""
from __future__ import annotations
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import stat
import types

MIB=1024**2
PIN=frozenset(('path','bytes','sha256','device','inode','mtime_ns','ctime_ns'))
CPP_KIND='uwvm-independent-official-cpp-library-input-v1'
CPP_STATUS='PASS_INDEPENDENT_OFFICIAL_CPP_LIBRARY_INPUT_COPIED_AND_PRESERVED_NOT_EXECUTED'
POLICY='OFFICIAL_LIBCXXABI_BAREMETAL_ABORT_TU_AND_HEADER_VERBOSE_TRAP_V1'
TRAP='-D_LIBCPP_VERBOSE_ABORT(...)=__builtin_trap()'


def need(value,reason):
    if not value:raise ValueError(reason)


def closed(value,keys,reason):
    need(type(value) is dict and set(value)==set(keys),'closed '+reason+' required')


def decode(raw):
    def pairs(rows):
        result={}
        for key,value in rows:need(key not in result,'duplicate copied-input JSON member');result[key]=value
        return result
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:need(False,'nonfinite copied-input metadata'))


def body_equal(first,second):
    return all(first[key]==second[key] for key in ('bytes','sha256'))


def fields(info):
    return tuple(getattr(info,key) for key in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink'))


def selected(frame,path,*,owned=False):
    """Do not let copied-provenance metadata enlarge a consuming Frame's roots."""
    path=Path(path)
    need(path.is_absolute() and path.resolve(strict=True)==path and
         (path.is_relative_to(frame.source) or any(path.is_relative_to(r) for r in frame.roots) or
          (owned and path.is_relative_to(frame.out))),'physical selected input scope required: '+str(path))
    return frame.pin(path)


def current(frame,pin,*,owned=False):
    closed(pin,PIN,'actual library full7 pin')
    need(selected(frame,pin['path'],owned=owned)==pin,'current library input changed')
    return pin


def descriptor_copy(frame,restart,source,destination):
    """Stream one exact exclusive regular copy through held named descriptors.

    This is byte reuse, never a compilation. No hardlink, symlink, source mode
    mutation or SDK modification is permitted. The new physical identity is
    recorded only after fsync/chmod and independently reopening actual bytes.
    """
    current(frame,source,owned=True)
    destination=Path(destination)
    need(destination.is_absolute() and destination.is_relative_to(frame.out) and
         not destination.exists() and not destination.is_symlink(),'fresh owned library leaf required')
    destination.parent.mkdir(mode=0o700,parents=True,exist_ok=True)
    need(destination.parent.resolve(strict=True)==destination.parent,'physical owned copy parent required')
    source_parent,source_name=restart._parent(source['path'])
    destination_parent=None;source_fd=None;destination_fd=None
    frame._library_open_descriptors=getattr(frame,'_library_open_descriptors',0)+1
    try:
        destination_parent,destination_name=restart._parent(str(destination));frame._library_open_descriptors+=1
        source_fd=os.open(source_name,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC,dir_fd=source_parent);frame._library_open_descriptors+=1
        before=os.fstat(source_fd)
        need(stat.S_ISREG(before.st_mode) and before.st_nlink==1 and 0<=before.st_size<=64*MIB and
             (before.st_dev,before.st_ino,before.st_size,before.st_mtime_ns,before.st_ctime_ns)==
             tuple(source[k] for k in ('device','inode','bytes','mtime_ns','ctime_ns')),'exact ordinary single-name source required')
        destination_fd=os.open(destination_name,os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,
                               0o600,dir_fd=destination_parent);frame._library_open_descriptors+=1
        digest=hashlib.sha256();count=0
        while True:
            frame.guard();part=os.read(source_fd,min(MIB,before.st_size-count+1))
            if not part:break
            digest.update(part);count+=len(part);need(count<=before.st_size,'copy source grew')
            at=0
            while at<len(part):
                wrote=os.write(destination_fd,part[at:]);need(wrote>0,'short library output write');at+=wrote
        need(count==source['bytes'] and digest.hexdigest()==source['sha256'] and
             fields(before)==fields(os.fstat(source_fd))==fields(os.stat(source_name,dir_fd=source_parent,follow_symlinks=False)),
             'copy source bytes or identity changed')
        restart._verify_named_file(source['path'],source_parent,source_name,before)
        os.fsync(destination_fd);os.fchmod(destination_fd,0o400);os.fsync(destination_fd)
        after=os.fstat(destination_fd)
        need(stat.S_ISREG(after.st_mode) and after.st_nlink==1 and after.st_size==count and
             (after.st_dev,after.st_ino)!=(before.st_dev,before.st_ino),'actual independent library inode required')
        restart._verify_named_file(str(destination),destination_parent,destination_name,after)
        os.fsync(destination_parent)
    finally:
        errors=[]
        for descriptor in (destination_fd,source_fd,destination_parent,source_parent):
            if descriptor is not None:
                try:os.close(descriptor)
                except BaseException as error:errors.append(repr(error))
                finally:frame._library_open_descriptors-=1
        need(not errors,'owned copy descriptor cleanup failed: '+repr(errors))
    copied=selected(frame,destination,owned=True)
    need(body_equal(copied,source) and (copied['device'],copied['inode'])!=(source['device'],source['inode']),
         'independent copy must preserve complete source bytes')
    current(frame,source,owned=True)
    return {'source':source,'copy':copied,'relation':'EXCLUSIVE_NEW_INODE_FULL_STREAM_SHA256_EQUAL',
            'copy_mode':0o100400,'source_mode':before.st_mode,'source_nlink':before.st_nlink,'copy_nlink':1}


def leaf(frame,restart,leaves,role,old,current_pin,destination):
    need(type(role) is str and re.fullmatch('[a-z][a-z0-9_.-]{0,95}',role) and role not in leaves,
         'unique finite library leaf role required')
    closed(old,PIN,'historical library leaf pin')
    need(body_equal(old,current_pin),'historical/current library leaf content differs')
    row=descriptor_copy(frame,restart,current_pin,destination)
    leaves[role]={'historical':old,**row}
    return row['copy']


def md_spellings(raw,target,source,cwd):
    """Parse the unchanged original single MD rule without opening its names.

    Names here remain historical spellings. Stage A separately resolves each
    one against the actual original compiler cwd and records its physical pin;
    Stage B compares that exact association, then opens only selected current
    SDK files or the individually copied official source leaves.
    """
    text=raw.decode('utf-8','strict').replace('\\\n',' ')
    need(':' in text,'one actual full-MD rule required');left,right=text.split(':',1)
    need(shlex.split(left)==[target],'unchanged original MD target required')
    names=shlex.split(right)
    need(names and len(names)<=4096 and not any(name.endswith(':') for name in names),'bounded single full-MD dependency rule required')
    result=[]
    for name in names:
        path=Path(name);path=path if path.is_absolute() else Path(cwd)/path
        normalized=os.path.normpath(str(path))
        need(Path(normalized).is_absolute() and '\x00' not in normalized,'absolute historical MD spelling required')
        if normalized not in result:result.append(normalized)
    need(source in result,'historical primary source absent from MD')
    return result


def stage_cpp(frame,api,admission,selection):
    """A1: qualify the genuine object under all original58 whole-root checks."""
    closed(selection,('format_version','kind','official_cpp_report','compiler','sysroot','restart_mappings'),'CPP library selection')
    need(selection['format_version']==1 and selection['kind']=='uwvm-official-cpp-library-selection-v1',
         'explicit independent CPP library preparation required')
    need(frame.config['simulation'] is frame.config['benchmark'] is frame.config['profiler'] is None and
         frame.config['cgroup']['memory_max']==64*1024**3 and frame.config['cgroup']['memory_swap_max']==0,
         'compiler-free actual current64/swap0 scope required')
    evidence=admission.Evidence(api,frame,selection['restart_mappings'])
    support=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/command_build_support.py','cpp_library_actual_md')
    qualifier=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/independent_cpp_object.py','cpp_library_independent_original_qualification')
    compiler=frame.inputs[selection['compiler']];current(frame,compiler)
    sysroot=Path(selection['sysroot']);need(sysroot.resolve(strict=True)==sysroot and sysroot.is_dir() and
        any(sysroot==r or sysroot.is_relative_to(r) for r in frame.roots),'actual complete WASI sysroot required')
    result=qualifier.qualify(frame,evidence,selection['official_cpp_report'],
        {'WASI_CLANGXX':compiler['path'],'WASI_SYSROOT':str(sysroot)},support)
    report=evidence.selected(selection['official_cpp_report']);proof=report['overlay'];leaves={}
    # These documents are unchanged original observations. Their copied bodies
    # are preserved without rebasing any embedded path, source, cgroup or status.
    def copy(role,old,name):
        now=selected(frame,old['path'],owned=True) if Path(old['path']).is_relative_to(frame.out) else evidence.pin(old)
        return leaf(frame,evidence.restart,leaves,role,old,now,frame.out/'library'/name)
    obj=copy('object',proof['object'],'official-abort-message.o')
    dep=copy('dependency-file',proof['dependency_file'],'proof/original-compile.d')
    copy('original-report',frame.inputs[selection['official_cpp_report']],'proof/original-report.json')
    copy('qualification',result['current_qualification'],'proof/qualification.json')
    copy('compile-stdout',proof['compile']['stdout'],'proof/original-compile.stdout')
    copy('compile-stderr',proof['compile']['stderr'],'proof/original-compile.stderr')
    config_copy=copy('preparation-configuration',frame.report['configuration'],'proof/preparation-configuration.json')
    # Exactly the finite official Git/source proof needed by the unchanged
    # official_sources verifier is carried forward, not unrelated old DNS/CA.
    official=proof['official'];items=[official['lock'],official['transport_report'],*official['files'].values()]
    scope=official.get('acquisition_scope')
    if scope is not None:items.extend(scope['current_source_artifacts'])
    # Raw lock form also selects commit/tree bodies not listed in acquisition_scope.
    lock=evidence.document(official['lock'])
    for key in ('commit_document',):
        if key in lock:items.append(lock[key])
    if 'tree_documents' in lock:items.extend(lock['tree_documents'].values())
    logical={}
    for pin in items:
        closed(pin,PIN,'finite original official artifact')
        key=tuple(pin[k] for k in sorted(PIN));logical[key]=pin
    need(0<len(logical)<=128,'bounded complete official source proof leaf set required')
    for index,pin in enumerate(logical.values()):copy('official-%03d'%index,pin,'proof/official/%03d'%index)
    spellings=md_spellings(api.bounded(Path(dep['path']),MIB),proof['object']['path'],
        proof['compile']['argv'][proof['compile']['argv'].index('-c')+1],proof['compile']['cwd'])
    dependencies={pin['path']:pin for pin in proof['compile']['dependencies']}
    associations=[]
    for spelling in spellings:
        physical=str(Path(spelling).resolve(strict=True));need(physical in dependencies,'original MD name lacks actual compile dependency')
        old=dependencies[physical];now=evidence.pin(old)
        associations.append({'historical_spelling':spelling,'historical_pin':old,'preparation_current_pin':now})
    need({row['historical_pin']['path'] for row in associations}==set(dependencies),
         'every actual original MD dependency must be accounted for')
    frame.report.update(producer=selected(frame,frame.source/'tests/ros_unitree/prepare_cpp_library.py'),
        helper=selected(frame,frame.source/'tests/ros_unitree/library_inputs.py'),preparation_configuration_copy=config_copy,library={'object':obj,'leaves':leaves,'original_cpp_report':frame.inputs[selection['official_cpp_report']],
        'original_qualification':result['current_qualification'],'original_overlay':proof,
        'original_compile_dependency_spellings':associations,'preparation_actual_full_MD':result['actual_full_MD'],
        'compiler_current':compiler,'sysroot':str(sysroot),'policy':POLICY,'Guest_compile_flags':['-UNDEBUG',TRAP]},
        original_roots_observed_during_preparation=True,original_roots_claimed_current_during_later_build=False,
        SDK_modified=False,compilation_performed=False,Runtime_executed=False,actors_started=0,
        guest_authority=[],commands=[],cleanup=[],copy_descriptor_cleanup={'status':'PASS','remaining':[],
            'owned_copy_descriptors':getattr(frame,'_library_open_descriptors',0)},
        library_output_state='CONSUMABLE_ONLY_IF_ENCLOSING_REPORT_FINISHES_PASS',
        original_reports_rewritten=False,Typed14_required_or_promoted=False,
        selected_pin_bridges=evidence.bridges,scope='QUALIFIED_COPIED_LIBRARY_INPUT_ONLY_NOT_GUEST_OR_NATIVE_EXECUTION')
    need(not frame.actors and frame.report['copy_descriptor_cleanup']['owned_copy_descriptors']==0,
         'all owned copy descriptors must be closed and no actor may be created')
    return CPP_STATUS


def terminal_stage(frame,api,label,kind,status,*,producer_relative='tests/ros_unitree/prepare_cpp_library.py'):
    """Accept only a truly completed same-current-source preparation report."""
    pin=current(frame,frame.inputs[label]);raw=api.bounded(Path(pin['path']),64*MIB)
    need(len(raw)==pin['bytes'] and hashlib.sha256(raw).hexdigest()==pin['sha256'],'actual terminal stage report changed')
    report=decode(raw)
    need(report.get('kind')==kind and report.get('status')==status and not report.get('error') and
         not report.get('finalization_errors') and all(report.get(k) is True for k in
         ('source_preserved','input_pins_preserved','whole_roots_preserved')) and
         report.get('cleanup')==[] and report.get('commands')==[] and report.get('actors_started')==0 and
         report.get('Runtime_executed') is False and report.get('compilation_performed') is False and
         report.get('SDK_modified') is False and report.get('original_reports_rewritten') is False and
         report.get('guest_authority')==[],'actual terminal copied-input preparation and clean no-actor scope required')
    need(report['source']==str(frame.source) and report['source_manifest_sha256']==frame.report['source_manifest_sha256'],
         'copied-input preparation must use this actual complete current source')
    for key,relative in (('producer',producer_relative),('helper','tests/ros_unitree/library_inputs.py')):
        value=current(frame,report[key]);need(value['path']==str(frame.source/relative) and
            frame.frozen.get(relative)=={k:value[k] for k in ('bytes','sha256')},'copied-input producer/helper must be current MF-bound source')
    config_pin=report['preparation_configuration_copy'];root=Path(pin['path']).parent
    need(root in frame.roots and Path(pin['path'])==root/'report.json','complete owned library-output root required')
    # The config is provenance for its stage. Its old58 roots are not opened or
    # called current here. The exact closed current cgroup incarnation must agree.
    current(frame,config_pin)
    need(Path(config_pin['path']).is_relative_to(root/'library'),'copied preparation configuration must be an actual library leaf')
    config=decode(api.bounded(Path(config_pin['path']),16*MIB))
    need(config['cgroup']==frame.config['cgroup'],
         'actual same-current64 library stage configuration required')
    need(all(report['cgroup_before'][k]==config['cgroup'][v] for k,v in (('boot_id','expected_boot_id'),
        ('device','expected_device'),('inode','expected_inode'),('path','expected_leaf'),('cpus','expected_cpus'),
        ('memory_max','memory_max'),('memory_swap_max','memory_swap_max'))),'actual producer cgroup observations/config association differs')
    frame.common.cgroup_preserved(report['cgroup_before'],report['cgroup_after'])
    frame.common.cgroup_preserved(frame.before,frame.common.cgroup_state(frame.config['cgroup']))
    need(report.get('original_roots_observed_during_preparation') is True and
         report.get('original_roots_claimed_current_during_later_build') is False and
         report.get('copy_descriptor_cleanup')=={'status':'PASS','remaining':[],'owned_copy_descriptors':0},
         'prior whole-root observations must remain historical stage facts')
    return report,pin


def copied_leaves(frame,leaves,root):
    """Read exact finite new leaves; no historic path is opened by this method."""
    need(type(leaves) is dict and 1<=len(leaves)<=512,'finite copied leaf manifest required')
    lookup={};paths=set()
    for role,row in leaves.items():
        closed(row,('historical','source','copy','relation','copy_mode','source_mode','source_nlink','copy_nlink'),'copied library leaf')
        for key in ('historical','source','copy'):closed(row[key],PIN,'library leaf full7')
        old,source,copy=row['historical'],row['source'],row['copy']
        need(row['relation']=='EXCLUSIVE_NEW_INODE_FULL_STREAM_SHA256_EQUAL' and
             row['copy_mode']==0o100400 and row['source_nlink']==row['copy_nlink']==1 and
             body_equal(old,source) and body_equal(source,copy) and
             (source['device'],source['inode'])!=(copy['device'],copy['inode']) and
             Path(copy['path']).is_relative_to(root/'library') and copy['path'] not in paths,
             'each copied leaf must have exact bytes and a separate real owned inode')
        current(frame,copy);info=Path(copy['path']).stat(follow_symlinks=False)
        need(info.st_mode==row['copy_mode'] and info.st_nlink==1,'copied leaf mode or link topology changed')
        key=tuple(old[k] for k in sorted(PIN))
        if key in lookup:need(body_equal(lookup[key],copy),'conflicting duplicate historical proof leaf')
        lookup[key]=copy;paths.add(copy['path'])
    return lookup


def consume_cpp(frame,api,label,compiler,sysroot):
    """B: consume a terminal A1 library without importing its old58 roots.

    Original proof JSON and MD names stay immutable. A small explicit data-only
    accessor maps only exact copied official artifact keys to their real new
    leaves while rerunning the unchanged official_sources parser. It is not the
    generic Evidence accessor and cannot grant an old path or device authority.
    """
    report,report_pin=terminal_stage(frame,api,label,CPP_KIND,CPP_STATUS)
    library=report['library'];root=Path(report_pin['path']).parent
    lookup=copied_leaves(frame,library['leaves'],root)
    need(all(row['copy'] in report['input_pins'] for row in library['leaves'].values()),
         'every actual copied product must have passed normal producer pin preservation')
    need(library['object']==library['leaves']['object']['copy'] and
         library['compiler_current']==current(frame,compiler) and library['sysroot']==str(sysroot) and
         Path(sysroot).resolve(strict=True)==Path(sysroot) and Path(sysroot).is_dir() and
         any(Path(sysroot)==r or Path(sysroot).is_relative_to(r) for r in frame.roots) and
         library['policy']==POLICY and library['Guest_compile_flags']==['-UNDEBUG',TRAP],
         'same actual compiler/sysroot and unchanged isolated official object policy required')
    original=decode(api.bounded(Path(library['leaves']['original-report']['copy']['path']),16*MIB))
    qualifier=api.frozen_module(frame.source,frame.frozen,'tests/ros_unitree/independent_cpp_object.py','library_cpp_finite_official_parser')
    proof=qualifier.terminal(original)
    need(proof==library['original_overlay'] and library['leaves']['original-report']['historical']==library['original_cpp_report'],
         'unchanged original terminal CPP report/overlay association required')
    qualifier.compile_association(proof,proof['object'],proof['dependency_file'],
        proof['official']['files']['libcxxabi/src/abort_message.cpp'],compiler['path'],str(sysroot))
    class OfficialCopies:
        def __init__(self):self.api=api;self.frame=frame
        def pin(self,old):
            closed(old,PIN,'exact selected original official artifact')
            key=tuple(old[k] for k in sorted(PIN))
            if key in lookup:return current(frame,lookup[key])
            need(old in lookup.values(),'official parser requested an unselected historical artifact')
            return current(frame,old)
    finite=OfficialCopies();lock=finite.pin(proof['official']['lock']);key='copied-official-source-lock'
    context=types.SimpleNamespace(common=frame.common,input=lambda label:Path(lock['path']) if label==key else None,
        inputs={key:lock},evidence=finite)
    official,_=qualifier.official_sources(context,key)
    need(set(official)==set(proof['official']) and all(official[k]==proof['official'][k]
         for k in official if k not in ('lock','transport_report')),
         'copied unchanged Git/source proof does not reproduce the original qualification')
    dep=library['leaves']['dependency-file']['copy'];old_command=proof['compile']
    names=md_spellings(api.bounded(Path(dep['path']),MIB),proof['object']['path'],
        old_command['argv'][old_command['argv'].index('-c')+1],old_command['cwd'])
    associations=library['original_compile_dependency_spellings']
    need(type(associations) is list and [r['historical_spelling'] for r in associations]==names,
         'unchanged original MD name association required')
    original_dependencies={pin['path']:pin for pin in old_command['dependencies']};current_dependencies=[];seen=set()
    for row in associations:
        closed(row,('historical_spelling','historical_pin','preparation_current_pin'),'original MD name binding')
        old=row['historical_pin'];need(original_dependencies.get(old['path'])==old and body_equal(old,row['preparation_current_pin']),
            'each unchanged original compile dependency must retain its actual preparation association')
        key=tuple(old[k] for k in sorted(PIN))
        actual=current(frame,lookup[key]) if key in lookup else current(frame,row['preparation_current_pin'])
        need(body_equal(old,actual),'consumed compiler/SDK/official MD input changed')
        if old['path'] not in seen:current_dependencies.append(actual);seen.add(old['path'])
    need(seen==set(original_dependencies) and len(seen)==len(library['preparation_actual_full_MD']),
         'complete original dependency closure required; no unconsumed historical58 roots are implied')
    obj=current(frame,library['object'])
    return {'object':obj,'compiler':proof['compiler'],'sysroot':library['sysroot'],
        'Guest_compile_flags':['-UNDEBUG',TRAP],'policy':POLICY,'prepared_library_report':report_pin,
        'actual_consumed_MD_inputs':current_dependencies,'original_object_compilation':{
            'report':library['original_cpp_report'],'object':proof['object'],'dependency_file':proof['dependency_file'],
            'source':original['source'],'source_manifest_sha256':original['source_manifest_sha256']},
        'original_whole_root_observations_current_here':False,'Runtime_executed':False,'Guest_authority':[]}
