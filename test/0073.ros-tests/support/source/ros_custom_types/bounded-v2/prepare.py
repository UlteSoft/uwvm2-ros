#!/usr/bin/env python3
"""Trusted two-phase bounded custom-interface preparation in the guarded lane.

``derive`` builds official packages in a copied, task-owned prefix, exports their
actual native identities/schema and derives the fixed-v1+bounded-v2 bundle.
``native`` recompiles peers/exporters against the separately frozen SourceB and
its separately derived complete SDK. Neither mode launches a ROS node/Guest.
Reports remain PREPARED, never delivery evidence. Root runs all commands serially.
"""
from __future__ import annotations
import argparse
import os
from pathlib import Path
import posixpath
import shlex
import shutil
import sys
import traceback
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(HERE.parents[1]/'ros_acceptance'))
from lane import Lane,add_arguments,atomic_json
from protocol import require
from runtime import digest,require_arm
from rebuild_affected import rooted,sdk_inventory,sdk_root_aliases
from gateway_qualification import architecture_binding,read_file_binding

PREFIX='/opt/uwvm-custom-types'
TARGETS={'bounded-native-identities':'native_identity.cc','bounded-native-peer':'native_peer.cc'}


def inventory(root):
    """Pin the complete derived prefix; internal links require pinned targets."""
    root=Path(root).resolve(strict=True);result={}
    for path in sorted(root.rglob('*')):
        name=str(path.relative_to(root))
        if path.is_symlink():
            target=path.resolve(strict=True);require(target.is_relative_to(root),'derived prefix link escapes')
            result[name]={'link':os.readlink(path),'target':str(target.relative_to(root))}
        elif path.is_file():result[name]={'bytes':path.stat().st_size,'sha256':digest(path)}
        else:require(path.is_dir(),'unexpected derived prefix input kind')
    require(result,'nonempty complete native prefix required');return result


def native(lane,version,argv):
    """Activate exact base SDK and closed package paths, with no ambient loader.

    A standalone official CMake install need not create colcon's root setup.bash.
    No optional arbitrary hook is evaluated. The base setup is immutable SDK
    input; its nonempty loader entries are validated before the one custom path
    is prepended. CMake/ament/ROS package discovery is explicitly closed here.
    """
    distro='noetic' if version==1 else 'jazzy'
    script=('source /opt/ros/'+distro+'/setup.bash && '
        '[[ -n "${LD_LIBRARY_PATH:-}" && "$LD_LIBRARY_PATH" != :* && "$LD_LIBRARY_PATH" != *: && "$LD_LIBRARY_PATH" != *::* ]] && '
        'export LD_LIBRARY_PATH="'+PREFIX+'/lib:$LD_LIBRARY_PATH" && '
        'export CMAKE_PREFIX_PATH="'+PREFIX+':/opt/ros/'+distro+'" && '
        'export AMENT_PREFIX_PATH="'+PREFIX+':/opt/ros/'+distro+'" && '
        'export ROS_PACKAGE_PATH="'+PREFIX+'/share:/opt/ros/'+distro+'/share" && exec "$@"')
    return ['/usr/bin/chroot',str(lane.root(version)),'/bin/bash','-lc',script,'bounded-native',*map(str,argv)]


def logical(lane,version,path,prefix=None):
    """Resolve only classified frozen-source, owned-output or selected SDK paths."""
    path=posixpath.normpath(path);temporary=lane.out/('ros'+str(version))/'tmp'
    if path.startswith('/work/'):root=lane.source;file=root/path[6:]
    elif path.startswith('/tmp/'):root=temporary;file=root/path[5:]
    elif prefix is not None and (path==PREFIX or path.startswith(PREFIX+'/')):root=prefix;file=root/path[len(PREFIX)+1:]
    else:root=lane.sdks[version];file=rooted(root,path)
    require(file.resolve(strict=True).is_relative_to(root.resolve(strict=True)),'native input path escapes classified root')
    return file


def recipes(lane,version,temporary,compile_row,prefix=None):
    """Reconstruct both successful actual object recipes and final full-MD files.

    Generated makefiles alone are insufficient. The effective last MD/MF and
    original argv must occur exactly once in the archived verbose build output.
    Both objects, original link command/files and explicit SDK/prefix link inputs
    are retained. This does not attest a transitive DT_NEEDED graph or each short
    official generator interpreter instruction.
    """
    database,pin=lane.json(temporary/'native-build/compile_commands.json')
    require(len(database)==2,'exact two native C++ translation units required')
    verbose=lane.read(compile_row);records=[]
    for target,filename in TARGETS.items():
        source='/work/test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/'+filename
        entries=[r for r in database if r['file']==source];require(len(entries)==1,'native source inventory differs')
        entry=entries[0];require(entry['directory']=='/tmp/native-build','native compile cwd differs')
        expected=entry.get('arguments') or shlex.split(entry['command']);matches=[]
        def clean(argv,original=False):
            out=[];mf=[];md=[];i=0
            while i<len(argv):
                t=argv[i];require(t not in ('-M','-MM','-MMD','-E','-S','-fsyntax-only','-MG','-MP','-Xpreprocessor') and not t.startswith(('@','-Wp,')),'opaque or partial native recipe')
                if t in ('-MF','-MT','-MQ'):
                    i+=1;require(i<len(argv),'missing dependency operand')
                    if t=='-MF':mf.append(argv[i])
                elif t=='-MD':md.append(t)
                else:out.append(t)
                i+=1
            require(not original or md and mf,'original system-inclusive MD/final MF required')
            require([v for v in out if v.startswith('-O')][-1:]==['-O1'] and [v for v in out if v.startswith('-g')][-1:]==['-g0'] and '-UNDEBUG' in out,'native optimization/assertion controls differ')
            require([v for v in out if v.startswith('-std=')][-1:] in (['-std=gnu++17'],['-std=c++17']),'native C++17 required')
            return out,posixpath.normpath(posixpath.join(entry['directory'],mf[-1])) if mf else None
        wanted,_=clean(expected)
        for line in verbose.splitlines():
            if source not in line or ' -c ' not in line:continue
            argv=shlex.split(line)
            if argv[:3]==['cd','/tmp/native-build','&&']:argv=argv[3:]
            if argv[0]!=expected[0]:continue
            actual,dep=clean(argv,True);require(actual==wanted,'database differs from actual original recipe');matches.append((argv,dep))
        require(len(matches)==1,'one original verbose compile per native target required')
        argv,dep=matches[0];obj=posixpath.normpath(posixpath.join('/tmp/native-build',argv[argv.index('-o')+1]))
        require(obj=='/tmp/native-build/CMakeFiles/'+target+'.dir/'+filename+'.o' and dep==obj+'.d','native object/final MF output differs')
        depfile=logical(lane,version,dep,prefix);raw=depfile.read_text().replace('\\\n',' ');rule,body=raw.split(':',1)
        require([posixpath.normpath(posixpath.join('/tmp/native-build',t)) for t in shlex.split(rule)]==[obj],'original depfile object rule differs')
        inputs={}
        for token in shlex.split(body):
            guest=posixpath.normpath(posixpath.join('/tmp/native-build',token));inputs[guest]=lane.pin(logical(lane,version,guest,prefix))
        require(source in inputs,'native source absent from original MD')
        linkfile=temporary/('native-build/CMakeFiles/'+target+'.dir/link.txt');link=shlex.split(linkfile.read_text())
        require(link[0]==expected[0] and link.count('-o')==1 and [posixpath.normpath(posixpath.join('/tmp/native-build',t)) for t in link if t.endswith('.o')]==[obj],'native link object/driver differs')
        require(posixpath.normpath(posixpath.join('/tmp/native-build',link[link.index('-o')+1]))=='/tmp/native-build/'+target,'native linked product differs')
        found=[]
        for line in verbose.splitlines():
            if link[0] not in line:continue
            a=shlex.split(line)
            if a[:3]==['cd','/tmp/native-build','&&']:a=a[3:]
            if a==link:found.append(line)
        require(len(found)==1,'one original verbose native link required')
        linked={t:lane.pin(logical(lane,version,t,prefix)) for t in link if t.startswith(('/usr/','/opt/','/lib/','/tmp/')) and not t.startswith('-')}
        destination=lane.out/('ros'+str(version))/target
        product=lane.snapshot(temporary/('native-build/'+target),destination,target+'-'+str(version))
        records.append({'target':target,'version':version,'status':'PREPARED','source':source,'source_pin':inputs[source],
            'original_compile_argv':argv,'working_directory':'/tmp/native-build','effective_mf':dep,'compiled_object_guest_path':obj,
            'dependency_file':lane.pin(depfile),'dependencies':inputs,'object':lane.pin(logical(lane,version,obj,prefix)),
            'compile_database':pin,'compile':compile_row,'link':{'file':lane.pin(linkfile),'argv':link,'scalar_inputs':linked},'product':product})
    return records


def qualify_peer(lane,path,version):
    """Admit only current SourceB native preparation, never SourceA products."""
    report,pin=lane.json(path)
    require(report.get('kind')=='bounded-custom-native-preparation' and report.get('mode')=='native' and report.get('status')=='PREPARED' and report.get('source')==str(lane.source) and report.get('source_manifest_sha256')==lane.report['source_manifest_sha256'],'fresh SourceB native preparation required')
    require(report.get('container_started_at')==lane.args.container_started_at,'native preparation container incarnation differs')
    require(all(report.get(k) is True for k in ('source_preserved','sdk_preserved','inputs_preserved','products_preserved')),'native preparation preservation incomplete')
    architecture,archpin=architecture_binding(report['architecture_evidence']['path'],lane.pins);require(archpin==report['architecture_evidence'] and architecture.get('cgroup_incarnation_before')==lane.before,'preparation architecture/current leaf changed')
    state,statepin=lane.json(report['sdk_before'][str(version)]['path']);require(statepin==report['sdk_before'][str(version)] and state=={'inventory':sdk_inventory(lane.sdks[version]),'aliases':sdk_root_aliases(lane.sdks[version])},'whole current derived SDK changed')
    rows=[r for r in report['cases'] if r['version']==version];require(len(rows)==2,'both current native targets required')
    replay=recipes(lane,version,Path(report['temporary'][str(version)]),rows[0]['compile'])
    # recipes creates new exact immutable snapshots for this output and checks all
    # recorded original dependencies; compare fields independent of copy inode.
    for old,new in zip(rows,replay):
        for field in ('target','source','source_pin','original_compile_argv','working_directory','effective_mf','compiled_object_guest_path','dependency_file','dependencies','object','compile_database','compile','link'):require(old[field]==new[field],'native original recipe/input replay differs: '+field)
        require(require_arm(old['product']['snapshot']['path'])==old['product']['snapshot'],'old prepared peer product changed')
    lane.report['bounded_native_qualification']={'report':pin,'architecture':archpin,'full_sdk':statepin,'rows':rows}
    return next(r['product']['snapshot']['path'] for r in replay if r['target']=='bounded-native-peer')


def main():
    parser=argparse.ArgumentParser(description=__doc__);add_arguments(parser,workers=False)
    parser.add_argument('--mode',choices=('derive','native'),required=True)
    parser.add_argument('--fixed-provider',type=Path)
    args=parser.parse_args();lane=None;status='FAIL'
    try:
        lane=Lane(args,'bounded-custom-native-preparation');require(Path(__file__).resolve()==lane.source/'test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/prepare.py','run exact frozen preparation driver')
        if args.mode=='native':require((lane.source/'test/0073.ros-tests/support/source/ros_custom_types/deployment-final/generated/composition.json').is_file(),'native phase requires complete new SourceB composite')
        lane.report.update(mode=args.mode,actual_ROS_delivery='NOT_EXECUTED',cases=[],temporary={})
        for version in (1,2):
            root=lane.root(version);temporary=lane.out/('ros'+str(version))/'tmp';sdk=lane.sdks[version];prefix=None
            if args.mode=='derive':
                original=rooted(sdk,PREFIX);require(original.is_dir(),'legacy official package prefix required')
                prefix=lane.out/('ros'+str(version))/'derived-prefix';shutil.copytree(original,prefix,symlinks=True)
                lane.bind(prefix,root/PREFIX[1:]);lane.report.setdefault('prefix_before',{})[str(version)]=inventory(prefix)
                package='/work/test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/ros'+str(version)+'/uwvm_ros_bounded_v2'
                compiler='/usr/bin/aarch64-linux-gnu-g++-'+('9' if version==1 else '13')
                configure=['/usr/bin/cmake','-S',package,'-B','/tmp/package-build','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_INSTALL_PREFIX='+PREFIX,'-DCMAKE_CXX_COMPILER='+compiler,'-DCATKIN_ENABLE_TESTING=OFF','-DBUILD_TESTING=OFF']
                for name,command in (('official-configure',configure),('official-build',['/usr/bin/cmake','--build','/tmp/package-build','--parallel','1','--verbose']),('official-install',['/usr/bin/cmake','--install','/tmp/package-build'])):
                    child,row=lane.start(name+'-'+str(version),native(lane,version,command),seconds=1200);lane.finish(child,row)
            compiler='/usr/bin/aarch64-linux-gnu-g++-'+('9' if version==1 else '13')
            frontend=next((rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/'+('9' if version==1 else '13')+'/cc1plus') for base in ('usr/lib','usr/libexec') if rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/'+('9' if version==1 else '13')+'/cc1plus').is_file()),None)
            require(frontend is not None,'selected native SDK frontend absent')
            for label,path in (('cmake',rooted(sdk,'/usr/bin/cmake')),('compiler',rooted(sdk,compiler)),('frontend',frontend)):
                lane.pin(path);lane.evidence.target(label+'-'+str(version),path)
            configured,crow=lane.start('native-configure-'+str(version),native(lane,version,['/usr/bin/cmake','-S','/work/test/0073.ros-tests/support/source/ros_custom_types/bounded-v2','-B','/tmp/native-build','-DUWVM_SOURCE=/work','-DUWVM_CUSTOM_NATIVE_ROS_VERSION='+str(version),'-DCMAKE_CXX_COMPILER='+compiler,'-DCMAKE_EXPORT_COMPILE_COMMANDS=ON']),seconds=180);lane.finish(configured,crow)
            child,row=lane.start('native-build-'+str(version),native(lane,version,['/usr/bin/cmake','--build','/tmp/native-build','--parallel','1','--verbose']),seconds=600);lane.finish(child,row)
            lane.evidence.check(['cmake-'+str(version),'compiler-'+str(version),'frontend-'+str(version)],'native-build-'+str(version)+'/')
            records=recipes(lane,version,temporary,row,prefix);lane.report['cases']+=records;lane.report['temporary'][str(version)]=str(temporary)
            export=temporary/'exporter';selected=next(r for r in records if r['target']=='bounded-native-identities');lane.snapshot(selected['product']['snapshot']['path'],export,'exporter-'+str(version))
            child,row=lane.start('official-export-'+str(version),native(lane,version,['/tmp/exporter']),seconds=30)
            lane.stopped(child,row,'exporter-'+str(version),cooperative=True);lane.send(child,row,b'I','actual native exporter stopped map');lane.finish(child,row)
            metadata,pin=lane.json(Path(row['stdout']));require(metadata.get('format')=='uwvm-ros-official-bounded-native-v2' and metadata.get('ros_version')==version,'actual official metadata export differs')
            lane.report.setdefault('official_metadata',{})[str(version)]={'output':pin,'command':row,'source_manifest':lane.report['source_manifest_sha256']}
            if prefix is not None:
                path=lane.out/('ros'+str(version)+'-derived-prefix.json');atomic_json(path,inventory(prefix));lane.report.setdefault('derived_prefix',{})[str(version)]={'root':str(prefix),'inventory':lane.pin(path),'requires_new_complete_sdk':'copy unchanged original SDK then overlay this exact prefix at '+PREFIX}
                bound=lane.out/('bound'+str(version))/'catalog.native.json'
                argv=[sys.executable,'-B',str(lane.source/'test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/bind_catalog.py'),'--template',str(lane.source/('test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/ros'+str(version)+'/catalog.preparation.json')),'--native-metadata',pin['path'],'--output',str(bound)]
                child,row=lane.start('bind-'+str(version),argv,seconds=30,rss=256*1024**2);lane.finish(child,row)
        if args.mode=='derive':
            require(args.fixed_provider and args.fixed_provider.resolve()==lane.source/'test/0073.ros-tests/support/source/ros_custom_types/deployment-r1/generated','exact original frozen fixed-v1 provider required')
            commands=[('combine',[sys.executable,'-B',str(lane.source/'sdk/origin/custom-types/combine_catalogs.py'),'--input',str(lane.out/'bound1/catalog.native.json'),'--input',str(lane.out/'bound2/catalog.native.json'),'--output',str(lane.out/'combined')]),
                ('generate',[sys.executable,'-B',str(lane.source/'sdk/origin/tools/custom_types.py'),str(lane.out/'combined/catalog.json'),'--out',str(lane.out/'bounded')]),
                ('compose',[sys.executable,'-B',str(lane.source/'sdk/origin/custom-types/compose_profiles.py'),'--fixed',str(args.fixed_provider),'--bounded',str(lane.out/'bounded'),'--output',str(lane.out/'composite')])]
            for name,argv in commands:child,row=lane.start(name,argv,seconds=60,rss=256*1024**2);lane.finish(child,row)
            composed=inventory(lane.out/'composite');require(len(composed)==26,'closed complete26 provider bundle required')
            path=lane.out/'composite-inventory.json';atomic_json(path,composed);lane.report['composite']={'root':str(lane.out/'composite'),'inventory':lane.pin(path),'next_source_relative':'test/0073.ros-tests/support/source/ros_custom_types/deployment-final/generated','required_next_phase':'freeze complete new SourceB plus actual catalogs/export receipts before any final Worker/Gateway/native/Guest compilation'}
        lane.report['new_tests_executed']=False;status='PREPARED'
    except BaseException as error:
        if lane is None:raise
        lane.report.update(error=repr(error),traceback=traceback.format_exc())
    finally:
        if lane is not None:
            actual=lane.close('PASS' if status=='PREPARED' else 'FAIL');status='PREPARED' if status=='PREPARED' and actual=='PASS' else 'FAIL';lane.report['status']=status;lane.save()
    return 0 if status=='PREPARED' else 1

if __name__=='__main__':raise SystemExit(main())
