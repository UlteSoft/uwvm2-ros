#!/usr/bin/env python3
"""Compose exact old fixed-v1 and bounded-v2 generated sources without replacing either.

The final Worker/Gateway compiler must select the complete new bundle in an
immutable SourceB manifest. A path-only mutable provider is insufficient. This
preparation tool copies all retained products byte-for-byte, validates generation
output hashes and materializes macro row sets to avoid delayed macro overwrite.
It invokes no compiler, ROS generator, native library, Runtime or permission API.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import stat


def read(path,maximum=4*1024*1024):
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        before=os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or before.st_size>maximum:
            raise ValueError('bounded regular provider input required')
        raw=bytearray()
        while len(raw)<=before.st_size:
            body=os.read(fd,min(65536,before.st_size+1-len(raw)))
            if not body:break
            raw.extend(body)
        after=os.fstat(fd)
        key=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns)
        if key(before)!=key(after) or len(raw)!=before.st_size:raise ValueError('provider input changed')
        return bytes(raw)
    finally:os.close(fd)


def strict(raw):
    def pairs(items):
        result={}
        for key,value in items:
            if key in result:raise ValueError('duplicate provider JSON key')
            result[key]=value
        return result
    return json.loads(raw,object_pairs_hook=pairs)


def pin(body):return {'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest()}


def profile(root,format):
    raw=read(root/'generation.json');proof=strict(raw)
    if not isinstance(proof,dict) or proof.get('format')!=format or proof.get('native_registry_emitted') is not True or proof.get('status')!='GENERATED_SOURCE_ONLY':
        raise ValueError('complete native-emitting source preparation required')
    outputs=proof.get('outputs')
    if not isinstance(outputs,dict) or not {'manifest.hpp','native_ros1.hpp','native_ros2.hpp'}<=set(outputs):
        raise ValueError('complete native/common provider set required')
    bodies={'generation.json':raw}
    for name,expected in outputs.items():
        if not isinstance(name,str) or not re.fullmatch(r'[A-Za-z0-9_]+\.[A-Za-z0-9]+',name) or not isinstance(expected,dict) or set(expected)!={'bytes','sha256'}:
            raise ValueError('closed basename output inventory required')
        body=read(root/name)
        if pin(body)!=expected:raise ValueError('generated product differs from original receipt: '+name)
        bodies[name]=body
    # A concealed file/symlink cannot become an undocumented bundled dependency.
    if {item.name for item in root.iterdir()}!=set(bodies):raise ValueError('provider directory has undeclared files')
    return proof,bodies


def rows(body,name,required=True):
    """Extract a closed generated M(...) row list, never C++/macro evaluation."""
    text=body.decode('utf-8');lines=text.splitlines();matches=[]
    for index,line in enumerate(lines):
        match=re.fullmatch(r'#define '+re.escape(name)+r'\(M\)(?:\s*(\\))?',line)
        if match:
            result=[]
            if match.group(1):
                index+=1
                while index<len(lines):
                    row=re.fullmatch(r'\s*(M\([^()\r\n]*\))\s*(\\)?',lines[index])
                    if not row:raise ValueError('unqualified native macro row: '+name)
                    result.append(row.group(1))
                    if not row.group(2):break
                    index+=1
                else:raise ValueError('unterminated native macro rows')
            matches.append(result)
    if len(matches)!=1:
        if not required and not matches:return []
        raise ValueError('missing/duplicate provider macro: '+name)
    return matches[0]


def define(name,entries):
    return '#define '+name+'(M)'+(' \\\n'+' \\\n'.join('  '+row for row in entries) if entries else '')+'\n'


def compose(fixed,bounded,output):
    fixed_proof,fixed_bodies=profile(fixed,'uwvm-ros-custom-fixed-v1')
    bounded_proof,bounded_bodies=profile(bounded,'uwvm-ros-custom-bounded-v2')
    inventory=fixed_proof.get('types',[])+bounded_proof.get('types',[])
    if not 1<=len(inventory)<=128:raise ValueError('aggregate interface ceiling exceeded')
    charge=0
    for row in fixed_proof['types']:
        cells=row.get('scalar_cells')
        if type(cells) is not int or not 1<=cells<=4096:raise ValueError('fixed expanded scalar charge missing')
        charge+=cells
    for row in bounded_proof['types']:
        components=row.get('components')
        if not isinstance(components,dict) or not components:raise ValueError('bounded complete component charges missing')
        for part in components.values():
            cells=part.get('expanded_nodes') if isinstance(part,dict) else None
            if type(cells) is not int or not 1<=cells<=4096:raise ValueError('bounded expanded node charge missing')
            charge+=cells
    if charge>16384:raise ValueError('combined fixed/bounded expanded schema charge exceeds16384')
    names=[]
    for row in inventory:
        if not isinstance(row,dict) or type(row.get('ros_version')) is not int or row['ros_version'] not in (1,2) or not isinstance(row.get('type'),str):raise ValueError('closed native type inventory required')
        name=(row['ros_version'],row['type'])
        if name in names:raise ValueError('native type duplicate across profiles/categories')
        names.append(name)
        native=row.get('native_identity');pattern=r'[0-9a-f]{32}' if row['ros_version']==1 else r'RIHS01_[0-9a-f]{64}'
        if not isinstance(native,str) or not re.fullmatch(pattern,native) or not re.fullmatch(r'UWVMROS1_[0-9a-f]{64}',row.get('adapter_codec_identity','')):raise ValueError('official/canonical identity missing')
    products={}
    common=['/** Exact immutable fixed-v1 + bounded-v2 registry composition.',
            ' * Original product bytes/codec identities remain in separate pinned subtrees.',
            ' * This table registers supported types and grants no authority. */',
            '#pragma once','#include <array>',
            '#define custom_generated_topics uwvm_composite_fixed_topics',
            '#define custom_topic_entries uwvm_composite_fixed_entries',
            '#include "fixed/manifest.hpp"','#undef custom_topic_entries','#undef custom_generated_topics',
            '#define custom_generated_topics uwvm_composite_bounded_topics',
            '#define custom_topic_entries uwvm_composite_bounded_entries',
            '#define custom_service_entries uwvm_composite_bounded_services',
            '#define custom_action_entries uwvm_composite_bounded_actions',
            '#include "bounded/manifest.hpp"',
            '#undef custom_action_entries','#undef custom_service_entries','#undef custom_topic_entries','#undef custom_generated_topics',
            'namespace uwvm2::imported::ros::ipc {',
            'inline constexpr auto uwvm_composite_topics() noexcept {',
            'constexpr auto fixed=uwvm_composite_fixed_entries(),bounded=uwvm_composite_bounded_entries();',
            'std::array<custom_topic_descriptor,fixed.size+bounded.size> out{};',
            'for(size_t i=0;i<fixed.size;++i)out[i]=fixed.data[i];for(size_t i=0;i<bounded.size;++i)out[fixed.size+i]=bounded.data[i];return out;}',
            'inline constexpr auto uwvm_composite_all_topics=uwvm_composite_topics();',
            'inline constexpr custom_topic_registry custom_topic_entries() noexcept {return {uwvm_composite_all_topics.data(),uwvm_composite_all_topics.size()};}',
            'inline constexpr custom_service_registry custom_service_entries() noexcept {return uwvm_composite_bounded_services();}',
            'inline constexpr custom_action_registry custom_action_entries() noexcept {return uwvm_composite_bounded_actions();}','}']
    products['manifest.hpp']=('\n'.join(common)+'\n').encode()
    for version in (1,2):
        macro=f'UWVM_ROS_CUSTOM_ROS{version}'
        old=rows(fixed_bodies[f'native_ros{version}.hpp'],macro+'_TYPES')
        topic=rows(bounded_bodies[f'native_ros{version}.hpp'],macro+'_TYPES')
        service=rows(bounded_bodies[f'native_ros{version}.hpp'],macro+'_SERVICE_TYPES')
        action=rows(bounded_bodies[f'native_ros{version}.hpp'],macro+'_ACTION_TYPES')
        counts=lambda proof,kind:sum(row['ros_version']==version and row.get('kind','message')==kind for row in proof['types'])
        if len(old)!=counts(fixed_proof,'message') or len(topic)!=counts(bounded_proof,'message') or len(service)!=counts(bounded_proof,'service') or len(action)!=counts(bounded_proof,'action'):raise ValueError('physical macro rows differ from declared inventory')
        lines=['/** Materialized exact native provider row sets; never macro-overwrite substitution. */','#pragma once',f'#include "fixed/native_ros{version}.hpp"',f'#undef {macro}_TOPIC_TYPES',f'#undef {macro}_TYPES',f'#include "bounded/native_ros{version}.hpp"']
        for suffix in ('TOPIC_TYPES','TYPES','SERVICE_TYPES','ACTION_TYPES'):lines.append(f'#undef {macro}_{suffix}')
        text='\n'.join(lines)+'\n'+define(macro+'_FIXED_TYPES',old)+define(macro+'_BOUNDED_TYPES',topic)+define(macro+'_TYPES',old+topic)+define(macro+'_SERVICE_TYPES',service)+define(macro+'_ACTION_TYPES',action)+f'#define {macro}_TOPIC_TYPES(M) {macro}_TYPES(M)\n'
        products[f'native_ros{version}.hpp']=text.encode()
    for directory,bodies in [('fixed',fixed_bodies),('bounded',bounded_bodies)]:
        for name,body in bodies.items():products[directory+'/'+name]=body
    # Re-read exact inputs before creating a fresh derived directory. Originals
    # stay untouched, including old preparation receipts and all failure records.
    for directory,bodies in [(fixed,fixed_bodies),(bounded,bounded_bodies)]:
        for name,body in bodies.items():
            if read(directory/name)!=body:raise ValueError('provider changed before composition')
    if output.exists():raise ValueError('select a fresh immutable bundle directory')
    receipt={'format':'uwvm-ros-custom-composite-v2','status':'COMPOSITE_SOURCE_PREPARATION_ONLY','native_execution':'NOT_RUN','compiler_execution':'NOT_RUN','implicit_permissions':[],
        'fixed':{'path':str(fixed.resolve()),'generation':pin(fixed_bodies['generation.json'])},'bounded':{'path':str(bounded.resolve()),'generation':pin(bounded_bodies['generation.json'])},'types':inventory,'aggregate_expanded_charge':charge,'outputs':{name:pin(body) for name,body in products.items()},
        'required_qualification':'freeze complete bundle in new SourceB and bind final full-MD native provider inputs; no mutable path-only dependency acceptance'}
    output.mkdir(parents=True)
    for name,body in products.items():
        path=output/name;path.parent.mkdir(parents=True,exist_ok=True)
        with path.open('xb') as stream:stream.write(body)
    with (output/'composition.json').open('xb') as stream:stream.write((json.dumps(receipt,indent=2)+'\n').encode())
    return receipt


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--fixed',type=Path,required=True)
    parser.add_argument('--bounded',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();print(json.dumps(compose(args.fixed,args.bounded,args.output),indent=2))

if __name__=='__main__':main()
