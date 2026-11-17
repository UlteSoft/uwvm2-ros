#!/usr/bin/env python3
"""Match generated Jazzy type descriptions and introspection, then test actual RMW codecs.

This is codec qualification within the mandatory shared 64 GiB gate. It does
not create an endpoint or qualify arbitrary native worker images. Every local
source and external native header/library is frozen or hashed; failures retain
an incomplete receipt. All local C++ builds use -O3 -g.
"""
import argparse,hashlib,json,os,shlex,shutil,subprocess,sys,time
from pathlib import Path
from platform_agilex_checks import ROOT,GROUP,guard,sha
from make_bounded_ros2_schema import compile_description
sys.path.insert(0,str(ROOT/'tools/ros_ecosystem'))
from bounded_message_corpus import message

TYPES={'std_msgs':['Bool','Byte','Char','Empty','String','Float32','Float64','Int8','Int16','Int32','Int64','UInt8','UInt16','UInt32','UInt64','ColorRGBA','Header','Float64MultiArray'],
       'geometry_msgs':['Point','Quaternion','Pose','PoseStamped','PoseWithCovariance','TransformStamped','Twist','Polygon'],
       'sensor_msgs':['JointState','LaserScan','Image','Imu','PointCloud2']}

def fixtures(out,ros,inputs):
    out.mkdir();headers=[];calls=[];reports=[]
    import re
    for package,names in TYPES.items():
        for name in names:
            source=ros/'share'/package/'msg'/(name+'.json');raw=source.read_bytes();inputs[str(source)]=sha(source)
            schema,binary,meta=compile_description(json.loads(raw),sequence_bound=8,string_bound=32,message_bytes=65536,work=65536,depth=32,finite_floats=True)
            stem=package+'_msg_'+name;meta['source_sha256']=sha(source)
            (out/(stem+'.schema')).write_bytes(binary);(out/(stem+'.json')).write_text(json.dumps(dict(schema=schema,**meta),indent=2)+'\n')
            (out/(stem+'.hash')).write_bytes(bytes([1])+bytes.fromhex(meta['native_type_hash'][7:]))
            for suffix,big in [('little',False),('big',True)]:
                (out/(stem+'.'+suffix)).write_bytes(message(schema['nodes'],'cdr',big=big,nonempty=True))
            header=re.sub(r'([a-z0-9])([A-Z])',r'\1_\2',re.sub(r'([A-Z]+)([A-Z][a-z])',r'\1_\2',name)).lower()
            headers.append('#include <'+package+'/msg/'+header+'.hpp>');calls.append('run<'+package+'::msg::'+name+'>(directory,"'+stem+'");');reports.append(meta)
    (out/'bounded-native-types.inc').write_text('#if !defined(UWVM_BOUNDED_TYPE_INSTANTIATIONS)\n'+'\n'.join(headers)+'\n#else\n'+'\n'.join(calls)+'\n#endif\n')
    return reports

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();guard()
    temporary=(ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve();out=a.output.resolve();assert out.is_relative_to(temporary);out.mkdir(parents=True,exist_ok=False)
    ros=temporary/'ros-scene-security/communication-ros-sysroot/opt/ros/jazzy';system=ros.parents[2]/'system-libs';assert system.is_dir()
    source=out/'source';fixture=Path('test/0084.ros-runtime/bounded_type_support.cpp');flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread']
    record=dict(complete=False,scope=__doc__,cgroup=str(GROUP),memory_max=68719476736,swap_max=0,commands=[],inputs={},snapshots={},external={},flags=flags,events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv):
        guard();start=time.monotonic();result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=300,
            env=dict(os.environ,LD_LIBRARY_PATH=str(ros/'lib')+':'+str(system),RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ASAN_OPTIONS='detect_leaks=1'))
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=argv,exit=result.returncode,seconds=time.monotonic()-start));save();print(label,result.returncode,flush=True)
        if result.returncode:raise RuntimeError(label+': '+result.stderr[-7000:])
        return result.stdout
    success=False
    try:
        record['types']=fixtures(out/'fixtures',ros,record['external']);record['fixtures']={str(path):sha(path)for path in (out/'fixtures').iterdir()}
        includes=['-isystem',str(ros/'include')]
        for directory in sorted((ros/'include').iterdir()):
            if directory.is_dir():includes+=['-isystem',str(directory)]
        for directory in (ros/'include',ros/'lib',system):
            for path in sorted(directory.rglob('*')):
                if path.is_file():record['external'][str(path)]=sha(path)
        libraries=['rmw_implementation','rmw','rcutils','rosidl_runtime_c','rosidl_typesupport_cpp','rosidl_typesupport_introspection_cpp',
            *[package+'__rosidl_typesupport_'+suffix for package in TYPES for suffix in ('cpp','introspection_cpp')]]
        link=['-L'+str(ros/'lib'),'-L'+str(system),'-Wl,-rpath-link,'+str(ros/'lib'),'-Wl,-rpath-link,'+str(system),*['-l'+name for name in libraries]]
        dependencies={ROOT/fixture,Path(__file__).resolve(),ROOT/'tools/ros_scene/make_bounded_ros2_schema.py',ROOT/'tools/ros_scene/bounded_message_definition.py',ROOT/'tools/ros_ecosystem/bounded_message_corpus.py'}
        variants=[('sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('gcc','/usr/bin/g++-13',[])]
        for name,compiler,extra in variants:
            record['inputs'][compiler]=sha(compiler)
            result=run(name+'-dependencies',[compiler,*flags,*extra,'-I'+str(ROOT/'src'),'-I'+str(ROOT/'third-parties/fast_io/include'),'-I'+str(out/'fixtures'),*includes,'-MM','-MT','codec',str(ROOT/fixture)])
            entries=shlex.split(result.replace('\\\n',' '));assert entries[0]=='codec:'
            for value in entries[1:]:
                path=Path(value).resolve()
                if path.is_relative_to(ROOT):dependencies.add(path)
                else:assert path.is_relative_to(out/'fixtures')
        for original in sorted(dependencies):
            name=original.relative_to(ROOT);target=source/name;target.parent.mkdir(parents=True,exist_ok=True);digest=sha(original);shutil.copyfile(original,target);assert sha(target)==digest
            record['inputs'][str(original)]=digest;record['snapshots'][str(name)]=digest
        for name,compiler,extra in variants:
            binary=out/name;run(name+'-compile',[compiler,*flags,*extra,'-I'+str(source/'src'),'-I'+str(source/'third-parties/fast_io/include'),'-I'+str(out/'fixtures'),*includes,str(source/fixture),*link,'-o',str(binary)])
            text=run(name,[str(binary),str(out/'fixtures')]);assert 'PASS bounded native type support: 31 types,'in text
            record.setdefault('binaries',{})[str(binary)]=sha(binary)
        success=True
    finally:
        record['inputs_unchanged']=all(sha(path)==value for path,value in record['inputs'].items());record['external_unchanged']=all(sha(path)==value for path,value in record['external'].items())
        record['snapshots_unchanged']=all(sha(source/path)==value for path,value in record['snapshots'].items());record['fixtures_unchanged']=all(sha(path)==value for path,value in record.get('fixtures',{}).items())
        record['events_after']=(GROUP/'memory.events').read_text();record['complete']=success and all(record[key]for key in ('inputs_unchanged','external_unchanged','snapshots_unchanged','fixtures_unchanged'));save()
    assert record['complete']

if __name__=='__main__':main()
