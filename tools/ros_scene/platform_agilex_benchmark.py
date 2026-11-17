#!/usr/bin/env python3
"""Compare pinned native motion conversion with bounded ROSv2 conversion.

Codec comparisons include extra validation only in ROSv2. Framed comparisons
share the common authority/lease/framing receiver and differ only in the vendor
conversion wrapper. Isolated comparisons use the same frames over inherited
private pipes, same CPU pair, buffers and request order. No CAN device, real
time guarantee, Wasm carrier or complete vendor-driver qualification is claimed.
"""
import argparse
import csv
import json
import os
from pathlib import Path
import shlex
import shutil
import statistics
import subprocess
import time
from device_dispatch_checks import ROOT,GROUP,guard,sha


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--upstream',type=Path,required=True);p.add_argument('--profile',action='store_true');a=p.parse_args();guard()
    assert {8,10}.issubset(os.sched_getaffinity(0));out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    upstream=a.upstream.resolve();assert upstream.name=='ugv_sdk-f2704eacdc90357078cd93ec60aae08bb4baab35'
    source=ROOT/'test/0084.ros-runtime/platform_agilex_benchmark.cpp';snapshot=out/'source';compiler='/usr/bin/clang++-22';cc='/usr/bin/clang-22'
    flags=['-std=c++23','-O3','-g','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-pthread']
    record=dict(complete=False,scope=__doc__,commands=[],inputs={},snapshots={},observations=[],
        cgroup=str(GROUP),inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,
        events_before=(GROUP/'memory.events').read_text(),affinity=sorted(os.sched_getaffinity(0)),
        topology={str(cpu):{name:(Path(f'/sys/devices/system/cpu/cpu{cpu}/topology')/name).read_text().strip()
            for name in ['core_id','physical_package_id','thread_siblings_list']} for cpu in [8,10]},
        numa={str(cpu):[x.name for x in Path(f'/sys/devices/system/cpu/cpu{cpu}').glob('node*')] for cpu in [8,10]},
        memory_policy='inherited, default first-touch; no NUMA binding claimed',load_before=Path('/proc/loadavg').read_text(),profiles={})
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,timeout=240):
        guard();start=time.monotonic();result=subprocess.run([str(x)for x in argv],capture_output=True,text=True,timeout=timeout)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=[str(x)for x in argv],exit=result.returncode,seconds=time.monotonic()-start));save()
        assert not result.returncode,(label,result.stderr[-5000:]);print('PASS',label,flush=True);return result.stdout
    def includes(repo,vendor):return ['-I'+str(repo/'src'),'-I'+str(repo/'third-parties/fast_io/include'),'-I'+str(vendor/'src'),'-I'+str(vendor/'include')]
    deps=set()
    for label,tool,options,path in [('cpp',compiler,flags,source),('c',cc,['-O3','-g'],upstream/'src/protocol_v2/agilex_msg_parser_v2.c')]:
        result=run(label+'-dependencies',[tool,*options,*includes(ROOT,upstream),'-MM','-MT','benchmark',path]);parts=shlex.split(result.replace('\\\n',' '));assert parts[0]=='benchmark:'
        deps.update(Path(x).resolve()for x in parts[1:]);record['inputs'][tool]=sha(tool)
    for original in sorted(deps):
        if original.is_relative_to(ROOT):relative=original.relative_to(ROOT)
        else:assert original.is_relative_to(upstream);relative=Path('upstream')/original.relative_to(upstream)
        dest=snapshot/relative;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(original,dest)
        record['inputs'][str(original)]=sha(original);record['snapshots'][str(relative)]=sha(dest)
    run('compile-upstream',[cc,'-std=c11','-O3','-g','-fno-omit-frame-pointer',*includes(snapshot,snapshot/'upstream'),'-c',snapshot/'upstream/src/protocol_v2/agilex_msg_parser_v2.c','-o',out/'upstream.o'])
    binary=out/'benchmark';run('compile-comparison',[compiler,*flags,*includes(snapshot,snapshot/'upstream'),snapshot/source.relative_to(ROOT),out/'upstream.o','-o',binary])
    record['binary_sha256']=sha(binary)
    modes=['native-codec','platform-codec','native-framed','platform-framed','native-isolated','platform-isolated']
    for round in range(3):
        for mode in modes if round%2==0 else reversed(modes):
            raw=run(f'measure-{round}-{mode}',['taskset','-c','8',binary,mode,'31','2'if 'isolated'in mode else'32'])
            rows=[line.split() for line in raw.splitlines()];assert len(rows)==31 and all(row[0]==mode for row in rows)
            record['observations'].append(dict(round=round,mode=mode,ns_per_operation=[int(row[1])/int(row[2])for row in rows],checksum=rows[-1][3]));save()
    record['summary']={}
    for mode in modes:
        values=sorted(value for row in record['observations']if row['mode']==mode for value in row['ns_per_operation'])
        record['summary'][mode]=dict(batch_median_ns=statistics.median(values),batch_p95_ns=values[int(.95*(len(values)-1))],batch_p99_ns=values[int(.99*(len(values)-1))])
    for family in ['codec','framed','isolated']:
        record['summary'][family+'_ratio']=record['summary']['platform-'+family]['batch_median_ns']/record['summary']['native-'+family]['batch_median_ns']
    record['request_latency']={}
    # Two steady-clock calls instrument each request equally. These numbers
    # include that overhead; batch quantiles above are not request tails.
    for mode in ['native-framed','platform-framed','native-isolated','platform-isolated']:
        values=[]
        for iteration in range(3):
            raw=run(f'latency-{iteration}-{mode}',['taskset','-c','8',binary,mode+'-latency','1','32'])
            rows=[line.split() for line in raw.splitlines() if line.startswith('sample ')]
            assert len(rows)==8192 and all(row[1]=='0' and int(row[2])==i for i,row in enumerate(rows))
            values.extend(int(row[3]) for row in rows)
        values.sort()
        record['request_latency'][mode]=dict(samples=len(values),median_ns=statistics.median(values),
            p95_ns=values[int(.95*(len(values)-1))],p99_ns=values[int(.99*(len(values)-1))],
            p999_ns=values[int(.999*(len(values)-1))],maximum_ns=values[-1],
            instrumentation='two steady-clock calls per request; no subtraction or discarded outliers')
        save()
    record['paired_latency']={}
    for mode in ['paired-framed','paired-isolated']:
        pairs=[]
        for iteration in range(3):
            raw=run(f'paired-{iteration}-{mode}',['taskset','-c','8',binary,mode,'1','32'])
            rows=[line.split() for line in raw.splitlines()];assert len(rows)==8192
            assert all(row[0]=='pair' and row[1]=='0' and int(row[2])==i for i,row in enumerate(rows))
            pairs.extend((int(row[3]),int(row[4])) for row in rows)
        result=dict(pairs=len(pairs),order='native/platform order reverses every pair on the same receiver process and channels',
            instrumentation='two steady-clock calls per request; all samples retained')
        for index,name in enumerate(['native','platform']):
            values=sorted(pair[index] for pair in pairs)
            result[name]=dict(median_ns=statistics.median(values),p95_ns=values[int(.95*(len(values)-1))],
                p99_ns=values[int(.99*(len(values)-1))],p999_ns=values[int(.999*(len(values)-1))],maximum_ns=values[-1])
        result['paired_median_difference_ns']=statistics.median(platform-native for native,platform in pairs)
        record['paired_latency'][mode]=result;save()
    if a.profile:
        vtune='/opt/intel/oneapi/vtune/latest/bin64/vtune';record['inputs'][vtune]=sha(vtune)
        for analysis in ['hotspots','uarch-exploration']:
            result=out/('vtune-'+analysis);knobs=['-knob','sampling-mode=hw','-knob','enable-stack-collection=true']if analysis=='hotspots'else['-knob','pmu-collection-mode=detailed','-knob','dram-bandwidth-limits=false']
            run('collect-'+analysis,[vtune,'-collect',analysis,*knobs,'-knob','sampling-interval=1','-no-analyze-system','-data-limit=100','-return-app-exitcode','-result-dir',result,'-source-search-dir',snapshot,'--','taskset','-c','8',binary,'platform-framed','1','12000'],timeout=120)
            run('summary-'+analysis,[vtune,'-report','summary','-result-dir',result,'-format','csv'],timeout=90)
            raw=run('functions-'+analysis,[vtune,'-report','hotspots','-result-dir',result,'-format','csv'],timeout=90)
            run('lines-'+analysis,[vtune,'-report','hotspots','-result-dir',result,'-group-by','function,source-file,source-line','-format','csv'],timeout=90)
            rows=list(csv.DictReader(raw.splitlines(),delimiter='\t'));candidates=[r for r in rows if 'platform_common' in r.get('Function (Full)','')]
            assert len(candidates)==1,candidates
            selector=candidates[0]['Function'];options=['-group-by','address','-column','Assembly,Source Line,Clockticks,Instructions Retired,CPI Rate']if analysis=='uarch-exploration'else[]
            assembly=run('detail-'+analysis,[vtune,'-report','hotspots','-result-dir',result,'-source-object','function='+selector,'-source-search-dir',snapshot,*options,'-format','csv'],timeout=90)
            if analysis=='uarch-exploration':
                instructions=list(csv.DictReader(assembly.splitlines(),delimiter='\t'));sampled=[r for r in instructions if r.get('Assembly')and(float(r.get('Clockticks')or 0)>0 or float(r.get('Instructions Retired')or 0)>0)]
                assert sampled,'No sampled assembly';record['profiles'][analysis]=dict(sampled_instruction_rows=len(sampled),function=selector)
            else:record['profiles'][analysis]=dict(function=selector,source_search=str(snapshot))
            save()
    record['inputs_unchanged']=all(sha(p)==h for p,h in record['inputs'].items());record['snapshots_unchanged']=all(sha(snapshot/p)==h for p,h in record['snapshots'].items())
    record['events_after']=(GROUP/'memory.events').read_text();record['load_after']=Path('/proc/loadavg').read_text();record['runner_sha256']=sha(__file__)
    record['complete']=record['inputs_unchanged']and record['snapshots_unchanged'];save();assert record['complete'];print(json.dumps(record['summary'],indent=2))


if __name__=='__main__':main()
