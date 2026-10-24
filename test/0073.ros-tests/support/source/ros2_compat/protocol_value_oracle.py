#!/usr/bin/env python3
"""Emit independent full-field value/CDR witnesses for the owned SDK fixtures.

The fixed schema below is independent of the production parser and codec emitter.
Python struct supplies exact canonical bytes. Genuine generated native values,
production converters and actual rclcpp serialization must be compiled/executed
separately by the admitted Linux owner. This script runs no ROS code or robot.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct


def text(value):
    raw = value.encode('utf-8')
    return struct.pack('<I',len(raw))+raw


def child(mode):
    return (struct.pack('<i',-17+7*mode)+text(('', 'A', 'tail')[mode])+
            text(('native-default','mode-one','mode-two')[mode])+
            struct.pack('<dB',(-0.0,-1.25,2.5)[mode],mode != 1))


def expected(name, mode):
    if name == 'Child':return child(mode)
    if name == 'Defaults':
        values = () if mode == 0 else ('alpha',) if mode == 1 else ('alpha','beta','gamma')
        count = mode
        return (struct.pack('<B',7+mode)+text(('native path','short','longer-mode-two')[mode])+
                child(mode)+child((mode+1)%3)+child((mode+2)%3)+struct.pack('<I',count)+
                b''.join(child((mode+i)%3) for i in range(count))+struct.pack('<I',len(values))+
                b''.join(text(value) for value in values))
    if name == 'Transform_Request':
        return struct.pack('<i',-17+mode)+child(mode)+text(('', 'tag1', 'tag2')[mode])+struct.pack('<3d',-0.0+mode,1.5+mode,-2.25-mode)
    if name == 'Transform_Response':
        digest = bytes((mode*17+i)&255 for i in range((0,1,16)[mode]))
        return (struct.pack('<BI',mode != 1,mode)+b''.join(child((mode+i)%3) for i in range(mode))+
                text(('ok','one','two')[mode])+struct.pack('<I',len(digest))+digest)
    if name == 'Traverse_Goal':return child(mode)+struct.pack('<i',3+mode)+text(('test','one','two')[mode])
    if name == 'Traverse_Result':return struct.pack('<B',mode == 2)+child((mode+1)%3)+text(('', 'continue','done')[mode])
    if name == 'Traverse_Feedback':return struct.pack('<d',(0.0,0.5,1.0)[mode])+child((mode+2)%3)
    raise ValueError('unknown owned complete interface')


def offsets(name, mode):
    """Independent positions of a Boolean and a nonempty UTF-8 field.

    The latter also identifies its immediately preceding four-byte length.
    These positions follow the fixed original schema, not generated C layouts.
    """
    empty = ('', 'A', 'tail')
    if name == 'Child':return len(child(mode))-1,12+len(empty[mode])
    if name == 'Defaults':return 1+len(text(('native path','short','longer-mode-two')[mode]))+len(child(mode))-1,5
    if name == 'Transform_Request':return 4+len(child(mode))-1,16+len(empty[mode])
    if name == 'Transform_Response':return 0,9+sum(len(child((mode+i)%3)) for i in range(mode))
    if name == 'Traverse_Goal':return len(child(mode))-1,12+len(empty[mode])
    if name == 'Traverse_Result':return 0,13+len(empty[(mode+1)%3])
    if name == 'Traverse_Feedback':return 8+len(child((mode+2)%3))-1,20+len(empty[(mode+2)%3])
    raise ValueError('unknown original field layout')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out',required=True,type=Path)
    args = parser.parse_args()
    if not args.out.is_absolute() or args.out.exists() or args.out.parent.resolve(strict=True) != args.out.parent:
        raise ValueError('fresh physical owned oracle output required')
    pkg = '::uwvm_source_proof_msgs::'
    types = {name:pkg+category+'::'+name for category,names in (
        ('msg',('Child','Defaults')),('srv',('Transform_Request','Transform_Response')),
        ('action',('Traverse_Goal','Traverse_Result','Traverse_Feedback'))) for name in names}
    lines = ['/** Independent fixed original-field fixture oracle. No Node or grant. */',
        '#include "src/ros/gateway/ros2/custom_services.h"','#include "src/ros/gateway/ros2/custom_action_contract.h"',
        '#include <rclcpp/serialization.hpp>','#include <rclcpp/serialized_message.hpp>',
        '#include <array>','#include <cassert>','#include <csignal>','#include <cstdlib>','#include <cstring>','#include <vector>','#include <unistd.h>',
        'using Child='+types['Child']+';', 'template<class Native>struct oracle;',
        'Child child(unsigned mode){Child value;value.number=-17+7*static_cast<int>(mode);',
        'value.empty=mode==0?"":mode==1?"A":"tail";value.text=mode==0?"native-default":mode==1?"mode-one":"mode-two";',
        'value.signed_zero=mode==0?-0.0:mode==1?-1.25:2.5;value.enabled=mode!=1;return value;}']
    bodies = {
        'Child':'return child(mode);',
        'Defaults':'''Native value;value.code=7+mode;value.text_default=mode==0?"native path":mode==1?"short":"longer-mode-two";
value.nested=child(mode);value.fixed[0]=child((mode+1)%3);value.fixed[1]=child((mode+2)%3);
value.sequence.resize(mode);for(unsigned i=0;i<mode;++i)value.sequence[i]=child((mode+i)%3);
value.labels.resize(mode==0?0:mode==1?1:3);if(mode)value.labels[0]="alpha";if(mode==2){value.labels[1]="beta";value.labels[2]="gamma";}return value;''',
        'Transform_Request':'''Native value;value.count=-17+static_cast<int>(mode);value.input=child(mode);value.tag=mode==0?"":mode==1?"tag1":"tag2";
value.scale[0]=-0.0+mode;value.scale[1]=1.5+mode;value.scale[2]=-2.25-mode;return value;''',
        'Transform_Response':'''Native value;value.accepted=mode!=1;value.results.resize(mode);for(unsigned i=0;i<mode;++i)value.results[i]=child((mode+i)%3);
value.reason=mode==0?"ok":mode==1?"one":"two";value.digest.resize(mode==0?0:mode==1?1:16);
for(unsigned i=0;i<value.digest.size();++i)value.digest[i]=(mode*17+i)&255;return value;''',
        'Traverse_Goal':'''Native value;value.command=child(mode);value.steps=3+mode;value.label=mode==0?"test":mode==1?"one":"two";return value;''',
        'Traverse_Result':'''Native value;value.completed=mode==2;value.final_state=child((mode+1)%3);value.message=mode==0?"":mode==1?"continue":"done";return value;''',
        'Traverse_Feedback':'''Native value;value.fraction=mode==0?0.0:mode==1?0.5:1.0;value.state=child((mode+2)%3);return value;'''}
    scenarios = []
    for name,native in types.items():
        rows = [expected(name,mode) for mode in range(3)]
        if len({hashlib.sha256(raw).hexdigest() for raw in rows}) != 3:
            raise ValueError('three distinct complete payloads required')
        lines += ['template<>struct oracle<'+native+'>{using Native='+native+';',
                  'static Native value(unsigned mode){'+bodies[name]+'}',
                  'static std::vector<std::uint8_t> bytes(unsigned mode){switch(mode){']
        for mode,raw in enumerate(rows):
            lines.append('case '+str(mode)+':return {'+','.join(str(b) for b in raw)+'};')
            scenarios.append(dict(type=native,mode=mode,bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()))
        lines += ['default:std::abort();}}',
                  'static std::array<std::size_t,2> offsets(unsigned mode){switch(mode){']
        for mode,raw in enumerate(rows):
            boolean,utf8 = offsets(name,mode)
            if raw[boolean] not in (0,1) or not 0 < raw[utf8] < 128:
                raise ValueError('independent malformed-value positions differ from original fields')
            lines.append('case '+str(mode)+':return {'+str(boolean)+','+str(utf8)+'};')
        lines += ['default:std::abort();}}};']
    lines += ['template<class Codec>void check(){using Native=typename Codec::Native;',
        'Native defaults{},default_restored{};std::vector<std::uint8_t> default_bytes,default_again;',
        'assert(Codec::valid(defaults)&&Codec::encode(defaults,default_bytes));',
        'assert(Codec::decode(default_bytes.data(),default_bytes.size(),default_restored));assert(default_restored==defaults);',
        'assert(Codec::encode(default_restored,default_again)&&default_again==default_bytes);',
        'for(unsigned mode=0;mode!=3;++mode){auto original=oracle<Native>::value(mode);auto bytes=oracle<Native>::bytes(mode);',
        'assert((::uwvm2::ros_gateway::ros2_custom_topics::binding<Native,Codec>::native_contract()));',
        'std::vector<std::uint8_t> encoded;assert(Codec::valid(original));assert(Codec::encode(original,encoded));assert(encoded==bytes);',
        'Native decoded{};assert(Codec::decode(bytes.data(),bytes.size(),decoded));assert(decoded==original);',
        'assert(Codec::encode(decoded,encoded)&&encoded==bytes);',
        'auto sentinel=oracle<Native>::value((mode+1)%3);std::vector<std::uint8_t> before,after;assert(Codec::encode(sentinel,before));',
        'assert(!Codec::decode(bytes.data(),bytes.size()-1,sentinel));assert(Codec::encode(sentinel,after)&&after==before);',
        'auto trailing=bytes;trailing.push_back(0);assert(!Codec::decode(trailing.data(),trailing.size(),sentinel));',
        'assert(Codec::encode(sentinel,after)&&after==before);',
        'auto locations=oracle<Native>::offsets(mode);',
        'for(unsigned kind=0;kind!=3;++kind){auto malformed=bytes;',
        'if(kind==0)malformed[locations[0]]=2;else if(kind==1)malformed[locations[1]]=255;',
        'else for(unsigned i=0;i!=4;++i)malformed[locations[1]-4+i]=255;',
        'assert(!Codec::decode(malformed.data(),malformed.size(),sentinel));assert(Codec::encode(sentinel,after)&&after==before);}',
        'rclcpp::Serialization<Native> serialization;rclcpp::SerializedMessage cdr;Native restored{};',
        'serialization.serialize_message(&original,&cdr);serialization.deserialize_message(&cdr,&restored);assert(restored==original);',
        'assert(Codec::encode(restored,encoded)&&encoded==bytes);}}',
        'int main(int argc,char**argv){if(argc!=2||std::strcmp(argv[1],"--mapped-stop"))return 64;',
        '#define CHECK_MESSAGE(Native,Name,Identity,Canonical,Bytes,Codec) check<Codec>();',
        'UWVM_ROS_CUSTOM_ROS2_TYPES(CHECK_MESSAGE)','#undef CHECK_MESSAGE',
        '#define CHECK_SERVICE(Native,Name,Identity,Canonical,Q,S) check<Q>();check<S>();',
        'UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(CHECK_SERVICE)','#undef CHECK_SERVICE',
        '#define CHECK_ACTION(Native,Name,Identity,Canonical,G,R,F) check<G>();check<R>();check<F>();',
        'UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES(CHECK_ACTION)','#undef CHECK_ACTION',
        'if(::raise(SIGSTOP)!=0)return 1;char gate=0;if(::read(STDIN_FILENO,&gate,1)!=1||gate!=\'I\')return 1;return 0;}']
    raw = ('\n'.join(lines)+'\n').encode()
    args.out.mkdir(mode=0o700);(args.out/'contract.cc').write_bytes(raw)
    (args.out/'source-oracle.json').write_text(json.dumps(dict(
        scope='Independent complete owned fixture expected bytes and native value witness SOURCE, not execution or official/vendor acceptance.',
        scenarios=scenarios,contract=dict(bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()),default_authority=[]),indent=2)+'\n')


if __name__ == '__main__':
    main()
