#!/usr/bin/env python3
"""Freeze, compare and hardware-profile bounded CDR processing against Fast-CDR.

The independent Fast-CDR archive comes from a completed retained -O3 -g native
oracle build. Comparisons process the same serialized inputs with preallocated
storage. Raw native lacks the finite-float check; checked native includes it.
Neither native path promises atomic output for all invalid input. No IPC,
endpoint, physical-device or whole-ROS performance qualification is implied.
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
from platform_agilex_checks import ROOT,GROUP,guard,sha
from bounded_message_definition import compile_schema

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--native-cohort',type=Path,required=True);p.add_argument('--profile',action='store_true');a=p.parse_args();guard()
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    native=a.native_cohort.resolve();receipt=native/'results.json';proof=json.loads(receipt.read_text());assert proof['complete']
    archive=native/'fastcdr-build/src/cpp/libfastcdr.a';assert sha(archive)==proof['binaries']['fastcdr']
    assert all(sha(native/'source'/path)==digest for path,digest in proof['snapshots'].items())
    assert all(sha(path)==digest for path,digest in proof['generated'].items())
    compiler='/usr/bin/clang++-22';snapshot=out/'source';source=Path('test/0084.ros-runtime/bounded_message_benchmark.cpp')
    record=dict(complete=False,scope=__doc__,commands=[],inputs={str(receipt):sha(receipt),str(archive):sha(archive),compiler:sha(compiler)},snapshots={},observations=[],profiles={},
        cgroup=str(GROUP),memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text(),
        affinity=sorted(os.sched_getaffinity(0)),memory_policy='inherited, default first-touch',load_before=Path('/proc/loadavg').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,timeout=240):
        guard();start=time.monotonic();done=subprocess.run(list(map(str,argv)),capture_output=True,text=True,timeout=timeout)
        (out/(label+'.stdout')).write_text(done.stdout);(out/(label+'.stderr')).write_text(done.stderr)
        record['commands'].append(dict(label=label,argv=list(map(str,argv)),exit=done.returncode,seconds=time.monotonic()-start));save()
        assert not done.returncode,(label,done.stderr[-6000:]);print('PASS',label,flush=True);return done.stdout
    flags=['-std=c++23','-O3','-g','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-pthread']
    includes=[ROOT/'src',ROOT/'third-parties/fast_io/include',native/'source/vendor/fastcdr/include',native/'fastcdr-build/include']
    result=run('dependencies',[compiler,*flags,*['-I'+str(x)for x in includes],'-MM','-MT','benchmark',ROOT/source]);parts=shlex.split(result.replace('\\\n',' '));assert parts.pop(0)=='benchmark:'
    dependencies={Path(x).resolve()for x in parts}|{Path(__file__).resolve(),ROOT/'tools/ros_scene/bounded_message_definition.py'}
    def mapped(path):
        if path.is_relative_to(ROOT):return snapshot/path.relative_to(ROOT)
        assert path.is_relative_to(native);return snapshot/'native'/path.relative_to(native)
    for path in sorted(dependencies):
        target=mapped(path);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
        record['inputs'][str(path)]=sha(path);record['snapshots'][str(target.relative_to(snapshot))]=sha(target)
    fixtures=out/'fixtures';fixtures.mkdir()
    for name,kind,bound in [('finite','float64',65536),('octets','uint8',1048576)]:
        document=dict(layout='cdr',message_bytes=2<<20,work=bound+2,depth=3,nodes=[
            dict(kind=kind,bound=0,flags=['finite']if name=='finite'else[],members=[]),
            dict(kind='sequence',bound=bound,flags=[],members=[dict(name='element',type=0)]),
            dict(kind='record',bound=0,flags=[],members=[dict(name='values',type=1)])])
        path=fixtures/(name+'.bin');path.write_bytes(compile_schema(document));record['inputs'][str(path)]=sha(path)
    binary=out/'benchmark';run('compile',[compiler,*flags,*['-I'+str(mapped(x))for x in includes],mapped(ROOT/source),archive,'-o',binary]);record['binary_sha256']=sha(binary)
    for iteration in range(3):
        raw=run('measure-'+str(iteration),['taskset','-c','8',binary,fixtures,'measure'])
        rows=[line.split()for line in raw.splitlines()];assert len(rows)==744
        for kind,count,method,sample,elapsed,iterations,checksum in rows:
            record['observations'].append(dict(round=iteration,kind=kind,count=int(count),method=int(method),sample=int(sample),ns_per_operation=int(elapsed)/int(iterations),checksum=checksum))
        save()
    record['summary']={}
    for kind,count in sorted({(r['kind'],r['count'])for r in record['observations']}):
        row={}
        for method,name in enumerate(('native-raw','native-checked','bounded')):
            values=sorted(r['ns_per_operation']for r in record['observations']if(r['kind'],r['count'],r['method'])==(kind,count,method))
            row[name]=dict(batch_median_ns=statistics.median(values),batch_p99_ns=values[int(.99*(len(values)-1))])
        row['ratio_to_checked']=row['bounded']['batch_median_ns']/row['native-checked']['batch_median_ns'];record['summary'][kind+'-'+str(count)]=row
    if a.profile:
        vtune=Path('/opt/intel/oneapi/vtune/latest/bin64/vtune');record['inputs'][str(vtune)]=sha(vtune)
        for analysis in ('hotspots','uarch-exploration'):
            result=out/('vtune-'+analysis);knobs=['-knob','sampling-mode=hw','-knob','enable-stack-collection=true']if analysis=='hotspots'else['-knob','pmu-collection-mode=detailed','-knob','dram-bandwidth-limits=false']
            run('collect-'+analysis,[vtune,'-collect',analysis,*knobs,'-knob','sampling-interval=1','-no-analyze-system','-data-limit=100','-return-app-exitcode','-result-dir',result,'-source-search-dir',snapshot,'--','taskset','-c','8',binary,fixtures,'profile'],120)
            run('summary-'+analysis,[vtune,'-report','summary','-result-dir',result,'-format','csv'],90)
            raw=run('functions-'+analysis,[vtune,'-report','hotspots','-result-dir',result,'-format','csv'],90)
            run('lines-'+analysis,[vtune,'-report','hotspots','-result-dir',result,'-group-by','function,source-file,source-line','-format','csv'],90)
            rows=list(csv.DictReader(raw.splitlines(),delimiter='\t'));candidates=[r for r in rows if 'bounded_message::schema::visit' in r.get('Function','')];assert len(candidates)==1,candidates
            selector=candidates[0]['Function'];options=['-group-by','address','-column','Assembly,Source Line,Clockticks,Instructions Retired,CPI Rate']if analysis=='uarch-exploration'else[]
            detail=run('detail-'+analysis,[vtune,'-report','hotspots','-result-dir',result,'-source-object','function='+selector,'-source-search-dir',snapshot,*options,'-format','csv'],90)
            sampled=[]
            if analysis=='uarch-exploration':
                sampled=[r for r in csv.DictReader(detail.splitlines(),delimiter='\t')if r.get('Assembly')and(float(r.get('Clockticks')or 0)>0 or float(r.get('Instructions Retired')or 0)>0)];assert sampled
            record['profiles'][analysis]=dict(function=selector,sampled_instruction_rows=len(sampled));save()
    record['inputs_unchanged']=all(sha(p)==h for p,h in record['inputs'].items());record['snapshots_unchanged']=all(sha(snapshot/p)==h for p,h in record['snapshots'].items())
    record['events_after']=(GROUP/'memory.events').read_text();record['load_after']=Path('/proc/loadavg').read_text();record['complete']=record['inputs_unchanged']and record['snapshots_unchanged'];save();assert record['complete']
    print(json.dumps(record['summary'],indent=2))

if __name__=='__main__':main()
