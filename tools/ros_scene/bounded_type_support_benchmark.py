#!/usr/bin/env python3
"""Measure actual RMW round trips and bounded adapter overhead, with official hardware VTune.

Both paths deserialize into a fresh generated native object and serialize into
the same preallocated output capacity. Raw native lacks finite validation;
checked native scans the object once. Bounded checks both wire boundaries and
strict extents. Packaged ROS dependency build flags are not asserted. Local
benchmark and adapter code use -O3 -g. This is not IPC, Wasm or device latency.
"""
import argparse,csv,json,os,shlex,shutil,statistics,subprocess,time
from pathlib import Path
from platform_agilex_checks import ROOT,GROUP,guard,sha
from make_bounded_ros2_schema import compile_description

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--profile',action='store_true');a=p.parse_args();guard()
    temporary=(ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve();out=a.output.resolve();assert out.is_relative_to(temporary);out.mkdir(parents=True,exist_ok=False)
    ros=temporary/'ros-scene-security/communication-ros-sysroot/opt/ros/jazzy';system=ros.parents[2]/'system-libs';source=out/'source'
    local=Path('test/0084.ros-runtime/bounded_type_support_benchmark.cpp');compiler='/usr/bin/clang++-22';flags=['-std=c++23','-O3','-g','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-pthread']
    env=dict(os.environ,LD_LIBRARY_PATH=str(ros/'lib')+':'+str(system),RMW_IMPLEMENTATION='rmw_fastrtps_cpp')
    record=dict(complete=False,scope=__doc__,cgroup=str(GROUP),memory_max=68719476736,swap_max=0,commands=[],inputs={compiler:sha(compiler)},external={},snapshots={},profiles={},observations=[],
        affinity=sorted(os.sched_getaffinity(0)),benchmark_cpu=8,memory_policy='inherited; first-touch; no NUMA isolation claim',load_before=Path('/proc/loadavg').read_text(),events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,timeout=300):
        guard();start=time.monotonic();result=subprocess.run(list(map(str,argv)),cwd=ROOT,capture_output=True,text=True,timeout=timeout,env=env)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr);record['commands'].append(dict(label=label,argv=list(map(str,argv)),exit=result.returncode,seconds=time.monotonic()-start));save();print(label,result.returncode,flush=True)
        assert result.returncode==0,(label,result.stderr[-6000:]);return result.stdout
    success=False
    try:
        original=ros/'share/std_msgs/msg/Float64MultiArray.json';record['external'][str(original)]=sha(original)
        _,binary,metadata=compile_description(json.loads(original.read_text()),sequence_bound=65536,string_bound=64,message_bytes=2*1024*1024,work=64*1024*1024,depth=32,finite_floats=True)
        fixtures=out/'fixtures';fixtures.mkdir();(fixtures/'schema.bin').write_bytes(binary);(fixtures/'hash.bin').write_bytes(bytes([1])+bytes.fromhex(metadata['native_type_hash'][7:]))
        for path in fixtures.iterdir():record['inputs'][str(path)]=sha(path)
        includes=[]
        for folder in sorted((ros/'include').iterdir()):
            if folder.is_dir():includes+=['-isystem',str(folder)]
        for directory in (ros/'include',ros/'lib',system):
            for path in directory.rglob('*'):
                if path.is_file():record['external'][str(path)]=sha(path)
        libraries=['rmw_implementation','rmw','rcutils','rosidl_runtime_c','rosidl_typesupport_cpp','rosidl_typesupport_introspection_cpp','std_msgs__rosidl_typesupport_cpp','std_msgs__rosidl_typesupport_introspection_cpp']
        link=['-L'+str(ros/'lib'),'-L'+str(system),'-Wl,-rpath-link,'+str(ros/'lib'),'-Wl,-rpath-link,'+str(system),*['-l'+name for name in libraries]]
        result=run('dependencies',[compiler,*flags,'-I'+str(ROOT/'src'),'-I'+str(ROOT/'third-parties/fast_io/include'),*includes,'-MM','-MT','benchmark',ROOT/local])
        parts=shlex.split(result.replace('\\\n',' '));assert parts.pop(0)=='benchmark:'
        dependencies={Path(value).resolve()for value in parts}|{Path(__file__).resolve(),ROOT/'tools/ros_scene/make_bounded_ros2_schema.py',ROOT/'tools/ros_scene/bounded_message_definition.py'}
        for original in dependencies:
            name=original.relative_to(ROOT);target=source/name;target.parent.mkdir(parents=True,exist_ok=True);digest=sha(original);shutil.copyfile(original,target);record['inputs'][str(original)]=digest;record['snapshots'][str(name)]=digest
        binary=out/'benchmark';run('compile',[compiler,*flags,'-I'+str(source/'src'),'-I'+str(source/'third-parties/fast_io/include'),*includes,source/local,*link,'-o',binary]);record['binary_sha256']=sha(binary)
        for round in range(3):
            lines=run('measure-'+str(round),['taskset','-c','8',binary,fixtures,'measure']).splitlines();assert len(lines)==279
            for line in lines:
                count,method,sample,elapsed,iterations,checksum=map(int,line.split());record['observations'].append(dict(round=round,count=count,method=method,sample=sample,ns_per_operation=elapsed/iterations,checksum=checksum))
        record['summary']={}
        for count in (6,1024,65536):
            entry={}
            for method,name in enumerate(('native-raw','native-checked','bounded')):
                values=sorted(row['ns_per_operation']for row in record['observations']if(row['count'],row['method'])==(count,method));entry[name]=dict(batch_median_ns=statistics.median(values),batch_p99_ns=values[int(.99*(len(values)-1))])
            entry['ratio_to_raw']=entry['bounded']['batch_median_ns']/entry['native-raw']['batch_median_ns'];record['summary'][str(count)]=entry
        if a.profile:
            vtune=Path('/opt/intel/oneapi/vtune/latest/bin64/vtune');record['inputs'][str(vtune)]=sha(vtune)
            for analysis in ('hotspots','uarch-exploration'):
                result=out/('vtune-'+analysis);knobs=['-knob','sampling-mode=hw','-knob','enable-stack-collection=true']if analysis=='hotspots'else['-knob','pmu-collection-mode=detailed','-knob','dram-bandwidth-limits=false']
                run('collect-'+analysis,[vtune,'-collect',analysis,*knobs,'-knob','sampling-interval=1','-no-analyze-system','-data-limit=100','-return-app-exitcode','-result-dir',result,'-source-search-dir',source,'--','taskset','-c','8',binary,fixtures,'profile'],120)
                run('summary-'+analysis,[vtune,'-report','summary','-result-dir',result,'-format','csv'],90)
                raw=run('functions-'+analysis,[vtune,'-report','hotspots','-result-dir',result,'-format','csv'],90)
                run('lines-'+analysis,[vtune,'-report','hotspots','-result-dir',result,'-group-by','function,source-file,source-line','-format','csv'],90)
                rows=list(csv.DictReader(raw.splitlines(),delimiter='\t'));matches=[row for row in rows if row.get('Module')=='benchmark'and row.get('Function')=='bounded_roundtrip'];assert len(matches)==1
                record['profiles'][analysis]=dict(function=matches[0]['Function'])
        success=True
    finally:
        record['inputs_unchanged']=all(sha(path)==digest for path,digest in record['inputs'].items());record['external_unchanged']=all(sha(path)==digest for path,digest in record['external'].items());record['snapshots_unchanged']=all(sha(source/path)==digest for path,digest in record['snapshots'].items())
        record['load_after']=Path('/proc/loadavg').read_text();record['events_after']=(GROUP/'memory.events').read_text();record['complete']=success and all(record[key]for key in ('inputs_unchanged','external_unchanged','snapshots_unchanged'));save()
    assert record['complete'];print(json.dumps(record['summary'],indent=2))

if __name__=='__main__':main()
