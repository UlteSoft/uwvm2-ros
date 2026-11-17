#!/usr/bin/env python3
"""Join artifact VTune source-line samples to exact preserved build inputs.

Rows without one unambiguous preserved source remain unattributed. Guest JIT
addresses and unresolved module labels are never assigned to a parser function
by inference. This qualifies host/native source locations,
not complete source coverage or costs hidden by unresolved symbols.
"""
import argparse
import csv
import hashlib
import json
from pathlib import Path

GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')

def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--profile',type=Path,required=True)
    parser.add_argument('--runtime',type=Path,required=True)
    parser.add_argument('--providers',choices=['wasm','native'],nargs='+',default=['wasm','native'])
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    out=args.profile.resolve();profile=json.loads((out/'profile.json').read_text());assert profile['complete']
    assert profile['runtime_sha256']==sha(args.runtime)
    build=json.loads((args.runtime.parent/'full-build-results.json').read_text());assert build['binary_sha256']==sha(args.runtime)
    manifest=Path(build['full_source_manifest']);assert sha(manifest)==build['full_source_manifest_sha256']
    frozen=Path(build['source_root']);vm_inputs={str(frozen/name):digest for name,digest in json.loads(manifest.read_text()).items()}
    prepared=json.loads((out/'prepare.json').read_text());native_inputs=prepared['inputs']
    record=dict(complete=False,scope=__doc__,runtime_sha256=sha(args.runtime),profile_sha256=sha(out/'profile.json'),
                full_source_manifest_sha256=sha(manifest),runner_sha256=sha(__file__),
                source_location_policy='First Source File column belongs to Source Line; repeated Source File is function metadata',
                guest_jit_source_mapping_qualified=False,reports=[])
    for provider in args.providers:
        inputs=vm_inputs if provider=='wasm' else native_inputs
        by_name={}
        for name,digest in inputs.items():
            path=Path(name)
            if path.suffix in ['.cc','.cpp','.h','.hpp','.cppm']:by_name.setdefault(path.name,[]).append((path,digest))
        cache={}
        for analysis in ['hotspots','uarch-exploration']:
            source=out/f'lines-{provider}-{analysis}.stdout'
            # VTune repeats "Source File": the first column belongs to the
            # grouped sampled source line, while the later column belongs to
            # the containing function. DictReader would silently overwrite
            # the sampled file and could join an inline-header line against
            # an unrelated line in the function's implementation file.
            with source.open() as stream:
                reader=csv.reader(stream,delimiter='\t');headers=next(reader)
                assert headers[:4]==['Function','Source File','Source Line','CPU Time'],headers
                columns=[]
                for index,name in enumerate(headers):
                    if name in columns:
                        name='Function Source File' if name=='Source File' else name+' (column '+str(index+1)+')'
                    assert name not in columns
                    columns.append(name)
                rows=[]
                for cells in reader:
                    assert len(cells)==len(columns),(source,cells)
                    rows.append(dict(zip(columns,cells)))
            joined=[];qualified_cpu=0.;total_cpu=0.
            for row in rows:
                value=dict(row);sample=float(row.get('CPU Time','0'));total_cpu+=sample
                candidates=by_name.get(Path(row.get('Source File','')).name,[])
                line=row.get('Source Line','')
                if len(candidates)==1 and line.isdecimal() and int(line)>0:
                    path,digest=candidates[0]
                    if path not in cache:
                        assert sha(path)==digest,'Changed preserved source: '+str(path)
                        cache[path]=path.read_text(errors='replace').splitlines()
                    number=int(line)
                    if number<=len(cache[path]):
                        value['preserved_source']=str(path);value['source_sha256']=digest
                        value['source_text']=cache[path][number-1];qualified_cpu+=sample
                joined.append(value)
            target=out/f'source-lines-{provider}-{analysis}.json';target.write_text(json.dumps(joined,indent=2)+'\n')
            record['reports'].append(dict(provider=provider,analysis=analysis,report_sha256=sha(source),rows=len(rows),
                preserved_rows=sum('preserved_source' in row for row in joined),sampled_cpu_seconds=total_cpu,
                preserved_source_cpu_seconds=qualified_cpu,joined_path=str(target),joined_sha256=sha(target)))
    record['complete']=True;(out/'source-line-attribution.json').write_text(json.dumps(record,indent=2)+'\n')
    print(json.dumps(record,indent=2))

if __name__=='__main__':main()
