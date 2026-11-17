#!/usr/bin/env python3
"""Exercise acquired ROS interface shapes without mixing ambiguous distributions.

This is structural portability testing, not a semantic or safety audit of a
package. Every chosen source hash is rechecked. References resolve inside the
same source first, then within exactly one recorded ROS distribution. Different
source contents are never silently merged. Explicit small test-only bounds
replace unbounded strings/sequences; these are not deployment defaults.
"""
import argparse
from collections import Counter,defaultdict
import hashlib
import json
from pathlib import Path
import struct
import sys

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/ros_scene'))
from platform_agilex_checks import guard,GROUP,sha
from bounded_message_definition import compile_schema

ROS1={'cturtle','diamondback','electric','fuerte','groovy','hydro','indigo','jade','kinetic','lunar','melodic','noetic'}
ROS2={'ardent','bouncy','crystal','dashing','eloquent','foxy','galactic','humble','iron','jazzy','kilted','rolling','lyrical'}
PRIMITIVES={'bool','int8','uint8','int16','uint16','int32','uint32','int64','uint64','float32','float64','string','wstring','byte','char','time','duration'}

class Refused(Exception):pass

def message(nodes,layout,big=False,nonempty=False):
    data=bytearray(b'\x00\x00\x00\x00' if big else b'\x00\x01\x00\x00') if layout=='cdr' else bytearray()
    origin=len(data)
    def number(width,value):
        if layout=='cdr':data.extend(b'\0'*((-(len(data)-origin))&(width-1)))
        data.extend(value.to_bytes(width,'big' if big else 'little'))
    def visit(index):
        node=nodes[index];kind=node['kind'];bound=node['bound']
        if kind in ('bool','int8','uint8'):number(1,1 if nonempty else 0)
        elif kind in ('int16','uint16'):number(2,257 if nonempty else 0)
        elif kind in ('int32','uint32','float32'):number(4,0x3f800000 if kind=='float32' and nonempty else 7 if nonempty else 0)
        elif kind in ('int64','uint64','float64'):number(8,0x3ff0000000000000 if kind=='float64' and nonempty else 9 if nonempty else 0)
        elif kind in ('string','wstring'):
            count=min(2,bound) if nonempty else 0
            number(4,count+(1 if kind=='string' and layout=='cdr' else 0))
            if kind=='wstring':
                for _ in range(count):number(2,65)
            else:data.extend(b'a'*count+(b'\0' if layout=='cdr' else b''))
        elif kind=='record':
            if not node['members'] and layout=='cdr':number(1,0)
            for child in node['members']:visit(child['type'])
        else:
            count=bound if kind=='array' else min(2,bound) if nonempty else 0
            if kind=='sequence':number(4,count)
            for _ in range(count):visit(node['members'][0]['type'])
        if len(data)>16*1024*1024:raise Refused('GENERATED_MESSAGE_BYTE_LIMIT')
    visit(len(nodes)-1)
    return bytes(data)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inventory',type=Path,required=True);parser.add_argument('--ecosystem',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True);parser.add_argument('--sequence-bound',type=int,required=True)
    parser.add_argument('--string-bound',type=int,required=True);args=parser.parse_args();guard()
    assert 0<=args.sequence_bound<=256 and 0<=args.string_bound<=4096
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    inventory=args.inventory.resolve();records=json.loads((inventory/'definitions.json').read_text())
    metadata=json.loads((inventory/'results.json').read_text());assert metadata['complete']
    ecosystem=args.ecosystem.resolve();receipt_pins={};distros={};pins={}
    for folder in (ecosystem,ecosystem/'supplemental',ecosystem/'recovery'):
        for path in sorted((folder/'receipts').glob('*.json')):
            value=json.loads(path.read_text())
            if value.get('status')=='ACQUIRED_AND_SCANNED':
                distros[path.stem]=sorted({binding['distro']for binding in value['source']['bindings']})
                receipt_pins[str(path)]=sha(path)
    local=defaultdict(list);distributed=defaultdict(list)
    for index,record in enumerate(records):
        if record['kind']=='msg':
            name=record.get('type_name',Path(record['file']).stem);namespace=record.get('type_namespace','msg')
            local[record['source_id'],record['package'],namespace,name].append(index)
            for distro in distros.get(record['source_id'],[]):distributed[distro,record['package'],namespace,name].append(index)
    outcomes=[];cases=[];used_sources={};compiled=set();totals=Counter()
    def pick(candidates):
        hashes={records[index]['sha256']for index in candidates}
        if not hashes:raise Refused('MISSING_DEPENDENCY')
        if len(hashes)!=1:raise Refused('AMBIGUOUS_DEPENDENCY')
        return candidates[0]
    bundle=out/'cases.bin'
    with bundle.open('xb') as stream:
        def blob(value):stream.write(struct.pack('<Q',len(value)));stream.write(value)
        for index,record in enumerate(records):
            if index%1000==0:guard()
            if record['status']!='SHAPES_EXTRACTED':
                outcomes.append(dict(index=index,status=record['status']));totals[record['status']]+=1;continue
            variants=distros.get(record['source_id'],[])
            if not variants:
                outcomes.append(dict(index=index,status='MISSING_DISTRIBUTION'));totals['MISSING_DISTRIBUTION']+=1;continue
            for distro in variants:
                layout='ros1' if distro in ROS1 else 'cdr' if distro in ROS2 else None
                for section in range(1,record['sections']+1):
                    row=dict(index=index,distro=distro,section=section)
                    try:
                        if layout is None:raise Refused('UNKNOWN_DISTRIBUTION_LAYOUT')
                        nodes=[];cache={};active=set();closure={};built={}
                        def node(kind,bound=0,members=()):
                            value=dict(kind=kind,bound=bound,flags=[],members=[dict(name=name,type=child)for name,child in members]);key=json.dumps(value,sort_keys=True)
                            if key in cache:return cache[key]
                            if len(nodes)>=4096:raise Refused('NODE_LIMIT')
                            nodes.append(value);cache[key]=len(nodes)-1;return len(nodes)-1
                        def build(current,part=1):
                            identity=(current,part)
                            if identity in active:raise Refused('RECURSIVE_TYPE')
                            if identity in built:return built[identity]
                            if len(active)>=64:raise Refused('DEPTH_LIMIT')
                            source=records[current]
                            if source['status']!='SHAPES_EXTRACTED':raise Refused('DEPENDENCY_REQUIRES_SYNTAX_REVIEW')
                            path=Path(source['file'])
                            if str(path) not in pins:
                                digest=sha(path)
                                if digest!=source['sha256']:raise Refused('SOURCE_CHANGED')
                                pins[str(path)]=digest
                            closure[str(path)]=source['sha256'];active.add(identity);members=[]
                            for field in source['fields']:
                                if field['section']!=part:continue
                                kind=field['type']
                                if kind in ('string','wstring'):
                                    if kind=='wstring' and layout=='ros1':raise Refused('ROS1_WIDE_STRING')
                                    bound=args.string_bound if field['string_bound'] is None else min(args.string_bound,field['string_bound'])
                                    child=node(kind,bound)
                                elif kind in ('time','duration'):
                                    if layout!='ros1':raise Refused('ROS2_LEGACY_TIME_ALIAS')
                                    primitive=node('uint32' if kind=='time' else 'int32');child=node('record',members=[('sec',primitive),('nsec',primitive)])
                                elif kind in PRIMITIVES:
                                    kind={'byte':'int8' if layout=='ros1' else 'uint8','char':'uint8'}.get(kind,kind);child=node(kind)
                                else:
                                    name=kind.split('/')
                                    namespace='msg'
                                    if kind=='Header':package,typename='std_msgs','Header'
                                    elif len(name)==1:package,typename=source['package'],kind
                                    elif len(name)==2:package,typename=name
                                    elif len(name)==3 and name[1]in ('msg','srv','action'):package,namespace,typename=name
                                    else:raise Refused('UNSUPPORTED_TYPE_REFERENCE')
                                    candidates=local.get((source['source_id'],package,namespace,typename),[])
                                    if not candidates:candidates=distributed.get((distro,package,namespace,typename),[])
                                    child=build(pick(candidates))
                                collection=field['array']
                                if collection!='scalar':
                                    if collection=='fixed_array':bound=field['array_bound'];kind='array'
                                    elif collection in ('sequence','bounded_sequence'):
                                        kind='sequence';bound=args.sequence_bound if field['array_bound'] is None else min(args.sequence_bound,field['array_bound'])
                                    else:raise Refused('UNSUPPORTED_COLLECTION')
                                    child=node(kind,bound,members=[('element',child)])
                                members.append((field['name'],child))
                            active.remove(identity);result=node('record',members=members);built[identity]=result;return result
                        root=build(index,section)
                        # Canonical deduplication can reuse the root; retain only
                        # its reachable graph and remap all prior references.
                        reached=set()
                        def reach(current):
                            if current in reached:return
                            reached.add(current)
                            for member in nodes[current]['members']:reach(member['type'])
                        reach(root);indices=sorted(reached);remap={old:new for new,old in enumerate(indices)}
                        nodes=[{**nodes[old],'members':[dict(name=member['name'],type=remap[member['type']])for member in nodes[old]['members']]}for old in indices]
                        document=dict(layout=layout,message_bytes=16*1024*1024,work=64*1024*1024,depth=64,nodes=nodes)
                        descriptor=compile_schema(document);digest=hashlib.sha256(descriptor).hexdigest()
                        if digest not in compiled:
                            pending=[]
                            for nonempty in (False,True):
                                data=message(nodes,layout,nonempty=nonempty)
                                target=message(nodes,'cdr',big=True,nonempty=nonempty)
                                identity=record.get('type_identity',f'{record["package"]}/{Path(record["file"]).name}')
                                name=f'{identity}:{section}:{distro}:{int(nonempty)}'
                                pending.append((name,data,target))
                            # Commit both variants only after complete generation.
                            # A refused shape must never poison deduplication or
                            # publish a partial bundle and later appear covered.
                            for name,data,target in pending:
                                blob(name.encode());blob(descriptor);blob(data);blob(target)
                                cases.append(dict(name=name,descriptor_sha256=digest,bytes=len(data),target_bytes=len(target)))
                            compiled.add(digest)
                        used_sources.update(closure);row.update(status='STRUCTURAL_CASE_PREPARED',descriptor_sha256=digest,closure_files=len(closure))
                    except Refused as failure:row['status']=str(failure)
                    except (ValueError,RecursionError) as failure:row.update(status='DESCRIPTION_REFUSED',reason=str(failure))
                    outcomes.append(row);totals[row['status']]+=1
    pins_to_check={**receipt_pins,**pins,str(inventory/'definitions.json'):sha(inventory/'definitions.json'),str(inventory/'results.json'):sha(inventory/'results.json'),str(Path(__file__).resolve()):sha(Path(__file__))}
    report=dict(complete=True,scope=__doc__,cgroup=str(GROUP),memory_max=68719476736,swap_max=0,
        test_only_bounds=dict(sequence=args.sequence_bound,string=args.string_bound),counts=dict(totals),unique_descriptors=len(compiled),
        cases=len(cases),source_files=len(used_sources),inputs=pins_to_check,bundle_sha256=sha(bundle),native_qualification=False,semantic_audit=False)
    for name,value in [('outcomes.json',outcomes),('cases.json',cases),('source-pins.json',used_sources),('results.json',report)]:
        (out/name).write_text(json.dumps(value,indent=2)+'\n')
    print(json.dumps({key:report[key]for key in ('counts','unique_descriptors','cases','source_files')}),flush=True)

if __name__=='__main__':main()
