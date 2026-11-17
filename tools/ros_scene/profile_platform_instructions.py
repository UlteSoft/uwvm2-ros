#!/usr/bin/env python3
"""Join sampled platform instructions to exact frozen C++ and fast_io sources.

Consumes a completed -O3 -g benchmark cohort and its real VTune hardware
collections. DWARF inline stacks independently resolve instruction addresses.
Only manifest-matching source receives text. PMU samples may skid; event counts
do not measure the latency of an individual instruction or prove real time.
"""
import argparse
import csv
import json
from pathlib import Path
import re
import shutil
import subprocess
import time
from device_dispatch_checks import ROOT, GROUP, guard, sha


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cohort',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--function',action='append',default=[],metavar='LABEL=PREFIX',
                        help='Select exact sampled functions instead of the default platform motion set')
    args=parser.parse_args();guard();cohort=args.cohort.resolve();out=args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    evidence=cohort/'results.json';proof=json.loads(evidence.read_text());binary=cohort/'benchmark';source=cohort/'source'
    assert proof['complete'] and proof['binary_sha256']==sha(binary)
    assert all(sha(source/p)==h for p,h in proof['snapshots'].items())
    vtune='/opt/intel/oneapi/vtune/latest/bin64/vtune'
    resolver=shutil.which('llvm-addr2line-22') or shutil.which('llvm-addr2line') or shutil.which('addr2line');assert resolver
    nm=shutil.which('nm');assert nm
    record=dict(complete=False,scope=__doc__,commands=[],functions=[],
        inputs={str(p):sha(p) for p in [evidence,binary,Path(vtune),Path(resolver),Path(nm),Path(__file__)]},
        cgroup=str(GROUP),memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv):
        guard();start=time.monotonic();done=subprocess.run(list(map(str,argv)),capture_output=True,text=True,timeout=120)
        (out/(label+'.stdout')).write_text(done.stdout);(out/(label+'.stderr')).write_text(done.stderr)
        record['commands'].append(dict(label=label,argv=list(map(str,argv)),exit=done.returncode,seconds=time.monotonic()-start));save()
        assert done.returncode==0,(label,done.stderr[-5000:]);return done.stdout
    symbols={}
    for line in run('symbols',[nm,'-S','-C','--defined-only',binary]).splitlines():
        row=line.split(maxsplit=3)
        if len(row)==4 and re.fullmatch('[0-9a-fA-F]+',row[0]) and re.fullmatch('[0-9a-fA-F]+',row[1]):
            symbols[int(row[0],16)]=max(symbols.get(int(row[0],16),0),int(row[1],16))
    table=cohort/'functions-uarch-exploration.stdout';record['inputs'][str(table)]=sha(table)
    rows=list(csv.DictReader(table.read_text().splitlines(),delimiter='\t'))
    targets=[('hash','fast_io::details::sha256::sha256_runtime_routine'),
             ('decoder','uwvm2::imported::ros::scene::devices::scalar_decoder::decode'),
             ('receiver','uwvm2::imported::ros::scene::devices::protocol::receiver::command_translate<'),
             ('codec','uwvm2::imported::ros::scene::platform_extensions::agilex::motion_codec::encode')]
    if args.function:
        targets=[]
        for value in args.function:
            label,separator,prefix=value.partition('=')
            assert separator and re.fullmatch(r'[a-z][a-z0-9_-]*',label) and prefix
            assert label not in [key for key,_ in targets]
            targets.append((label,prefix))
    pins={str(source/p):h for p,h in proof['snapshots'].items()};cache={}
    for key,prefix in targets:
        matches=[r for r in rows if r['Module']=='benchmark' and r['Function'].startswith(prefix)];assert len(matches)==1,matches
        selected=matches[0];begin=int(selected['Start Address'],16);end=begin+symbols[begin]
        raw=run(key+'-assembly',[vtune,'-report','hotspots','-result-dir',cohort/'vtune-uarch-exploration',
            '-source-object','function='+selected['Function'],'-source-search-dir',source,'-group-by','address',
            '-column','Assembly,Source Line,Clockticks,Instructions Retired,CPI Rate','-format','csv'])
        instructions=[]
        for row in csv.DictReader(raw.splitlines(),delimiter='\t'):
            if not row.get('Assembly') or row['Assembly'].startswith('Block '):continue
            address=int(row['Address'],16);assert begin<=address<end,(key,address,begin,end)
            instructions.append(dict(address=row['Address'],assembly=row['Assembly'],
                clockticks=float(row.get('Clockticks') or 0),instructions_retired=float(row.get('Instructions Retired') or 0)))
        assert instructions
        raw=run(key+'-dwarf',[resolver,'-a','-f','-C','-i','-e',binary,*[r['address'] for r in instructions]])
        frames={};current=None;pending=[]
        def finish():
            if current is None:return
            assert len(pending)%2==0,(key,pending)
            values=[]
            for i in range(0,len(pending),2):
                frame=dict(function=pending[i],location=pending[i+1])
                match=re.match(r'^(.*?):([0-9]+)(?::[0-9]+)?(?:\s.*)?$',pending[i+1])
                # DWARF preserves include spellings such as native/../foo.h.
                # Resolve to the exact frozen manifest entry before hashing;
                # a basename match must never attribute external source.
                path=str(Path(match[1]).resolve()) if match else None
                if match and path in pins and int(match[2])>0:
                    line=int(match[2]);digest=pins[path]
                    if path not in cache:
                        assert sha(path)==digest;cache[path]=Path(path).read_text().splitlines()
                    if line<=len(cache[path]):frame.update(source=path,line=line,source_sha256=digest,text=cache[path][line-1])
                values.append(frame)
            frames[current]=values
        for line in raw.splitlines():
            if re.fullmatch(r'0x[0-9a-fA-F]+',line):finish();current=int(line,16);pending=[]
            else:pending.append(line)
        finish()
        for row in instructions:row['inline_frames']=frames[int(row['address'],16)]
        sampled=[r for r in instructions if r['clockticks'] or r['instructions_retired']]
        joined=[r for r in sampled if any('source_sha256' in f for f in r['inline_frames'])]
        assert joined,(key,'No actual sample with manifest-matching source')
        artifact=out/(key+'-instructions.json');artifact.write_text(json.dumps(instructions,indent=2)+'\n')
        record['functions'].append(dict(function=selected['Function'],address=hex(begin),bytes=end-begin,
            sampled_instructions=len(sampled),sampled_source_joined=len(joined),artifact=str(artifact),sha256=sha(artifact)))
        save();print('PASS sampled assembly and frozen source:',key,len(joined),flush=True)
    record['inputs_unchanged']=all(sha(p)==h for p,h in record['inputs'].items())
    record['snapshots_unchanged']=all(sha(source/p)==h for p,h in proof['snapshots'].items())
    record['events_after']=(GROUP/'memory.events').read_text();record['complete']=record['inputs_unchanged'] and record['snapshots_unchanged'];save()
    assert record['complete']


if __name__=='__main__':main()
