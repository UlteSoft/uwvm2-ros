#!/usr/bin/env python3
"""Generate a finite native-build topic registry from pinned ROS type descriptions.

This is trusted build input, never guest input. The generated worker code pins
each upstream ROS type hash, instantiates only named generated C++ types and
preloads literal type-support SONAMEs before confinement. Runtime schema shape
checks remain mandatory. No endpoint, grant or executable image is published by
this generator; the ordinary worker closure must be built, pinned and confined.
"""
import argparse,hashlib,json,re
from pathlib import Path
from make_bounded_ros2_schema import compile_description
from bounded_message_definition import unique,number

def generate(descriptions,comparison_work):
    if not 1<=number(comparison_work,65536):raise ValueError('Explicit bounded comparison work required')
    if not 1<=len(descriptions)<=1024:raise ValueError('Finite explicit native registry required')
    approved={};sources={}
    for path in descriptions:
        path=Path(path).resolve()
        with path.open('rb')as stream:raw=stream.read(4*1024*1024+1)
        if len(raw)>4*1024*1024:raise ValueError('Type description too large')
        _,_,meta=compile_description(json.loads(raw,object_pairs_hook=unique),sequence_bound=0,string_bound=0,message_bytes=16*1024*1024,work=64*1024*1024,depth=64,finite_floats=False)
        identity=meta['type_identity'];package,category,name=identity.split('/')
        if category!='msg'or identity in approved:raise ValueError('Unique message types required for topic factories')
        header=re.sub(r'([a-z0-9])([A-Z])',r'\1_\2',re.sub(r'([A-Z]+)([A-Z][a-z])',r'\1_\2',name)).lower()
        approved[identity]=(package,name,header,meta['native_type_hash']);sources[str(path)]=hashlib.sha256(raw).hexdigest()
    lines=['/** @file @brief Generated finite native topic registry; trusted build input only. */','#pragma once',
           '#include <uwvm2/imported/ros/scene/native/bounded_topics_ros2.h>','#include <uwvm2/imported/ros/scene/communication_worker_configuration.h>','#include <dlfcn.h>']
    for package,name,header,digest in approved.values():lines.append('#include <'+package+'/msg/'+header+'.hpp>')
    lines+=['namespace uwvm2::imported::ros::scene::communication::ros2::approved_bounded_topics {',
        'using factories=::uwvm2::utils::container::std_vector<std::shared_ptr<bounded_topic_factory>>;',
        '/** @brief Preload literal, digest-pinned libraries before the worker syscall fence. */',
        'inline bool preload()noexcept {']
    libraries=sorted({'lib'+package+'__rosidl_typesupport_fastrtps_cpp.so'for package,_,_,_ in approved.values()})
    for library in libraries:lines.append('if(!::dlopen("'+library+'",RTLD_NOW|RTLD_LOCAL))return false;')
    lines+=['return true;}','/** @brief Prepare every referenced bounded topic type; no endpoint or authority is created. */',
        'inline error prepare(communication_worker_configuration::specification const& manifest,factories& output)noexcept {',
        'try{factories result;',
        'for(auto const& item:manifest.definitions){',
        'if(item.kind!=communication_configuration::family::type)continue;',
        'auto const& schema=std::get<type_plan>(item.policy);if(!schema.validate.bounded())continue;',
        'bool used{};for(auto const& endpoint:manifest.definitions)if(endpoint.kind==communication_configuration::family::topic&&std::get<topic_plan>(endpoint.policy).type==schema.selected)used=true;',
        'if(!used)continue;std::shared_ptr<bounded_topic_factory> factory;error status=error::unsupported;']
    for identity,(package,name,header,digest)in approved.items():
        raw=bytes.fromhex(digest[7:]);literal=','.join('0x'+format(value,'02x')for value in raw)
        lines+=['if(schema.type_identity=="'+identity+'"){constexpr rosidl_type_hash_t expected{1,{'+literal+'}};',
            'status=bounded_types::prepare_topic<'+package+'::msg::'+name+'>(schema,expected,'+str(comparison_work)+',factory);}']
    lines+=['if(status!=error::ok)return status;result.push_back(std::move(factory));}',
        'output=std::move(result);return error::ok;}catch(std::bad_alloc const&){return error::no_memory;}catch(...){return error::io;}}','}']
    source='\n'.join(lines)+'\n'
    return source,dict(sources=sources,types={identity:digest for identity,(_,_,_,digest)in approved.items()},
        packages=sorted({package for package,_,_,_ in approved.values()}),preload=libraries,comparison_work=comparison_work,source_sha256=hashlib.sha256(source.encode()).hexdigest())

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--type-description',type=Path,action='append',required=True);p.add_argument('--comparison-work',type=int,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    source,metadata=generate(a.type_description,a.comparison_work)
    with a.output.open('x')as stream:stream.write(source)
    with a.output.with_suffix(a.output.suffix+'.json').open('x')as stream:json.dump(metadata,stream,indent=2);stream.write('\n')

if __name__=='__main__':main()
