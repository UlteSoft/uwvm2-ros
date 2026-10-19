#!/usr/bin/env python3
"""Root-only preserved-r1 native SDK reuse and 74 fresh target-TU builds.

Source authoring does not run this worker. It never installs packages, downloads,
creates ROS/DDS endpoints or configures Runtime grants. Only the bounded native
String metadata exporter executes behind an actual mapped-stop gate.
"""
from __future__ import annotations
import argparse
import errno
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import posixpath
import shlex
import signal
import subprocess
import stat
import sys
import time
import traceback
import types

sys.dont_write_bytecode=True
MIB=1024**2;GIB=1024**3
PIN_KEYS={"path","bytes","sha256","device","inode","mtime_ns","ctime_ns"}
CG={"memory_max":64*GIB,"memory_swap_max":0,"expected_device":29,"expected_inode":18315,"expected_boot_id":"ec0d5f0f-3060-4220-bee2-2a7791ed4e72","expected_leaf":"/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope","expected_cpus":"0-31"}
NATIVE="c52bc5ed98e52904f52e1f8dac7031cd65a53f94d5128375d8de2eea23ae4d73"
WHEELS="4f21f666e8ef2d99354aa97696fed105809bb549c1f334eeeb4d5587ab066192"
OSMESA="251ec9868b33bba922a6df930933c53814de1130257d941d3e1fd483fd5a4f0a"


# Historical source/SDK bytes are admitted separately after this exact reboot.
# The support module never rewrites raw reports or re-labels historical actors.
HISTORICAL_CG = dict(CG, expected_inode=18151, expected_boot_id='af121220-8c6e-4302-9853-f08bf4c50dbb')
RESTART = None
RESTART_SUPPORT_SHA = '22492fdfadb84b5016cc0624229283f34289a144764a46fde4dfa95cdf94a485'


def install_restart(config, common, source, manifest_pin):
    global RESTART
    selected = config['restart_requalification']
    need(type(selected) is dict and set(selected) == {'helper','parallel_helper','support','completed_receipt'},
         'explicit reviewed restart support/helper/completed receipt required')
    raw, pin = read(Path(selected['support']['path']), MIB)
    need(pin == selected['support'] and pin['sha256'] == RESTART_SUPPORT_SHA,
         'reviewed physical restart support bytes changed')
    module = types.ModuleType('native74_r4_explicit_restart_support')
    module.__file__ = pin['path']
    exec(compile(raw, module.__file__, 'exec'), module.__dict__)
    RESTART = module.RestartAdmission(selected, common, read, CG, source, manifest_pin)


def same_pin(actual, previous):
    if actual == previous:
        return True
    need(RESTART is not None, 'post-restart selected pin lacks explicit content admission')
    return RESTART.compare(actual, previous)



def need(value,reason):
    if not value:raise ValueError(reason)

def pairs(items):
    result={}
    for key,value in items:
        need(key not in result,'duplicate JSON member');result[key]=value
    return result

def read(path,limit=64*MIB):
    path=Path(path);need(path.is_absolute(),'absolute input required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=limit,'bounded regular input required')
        raw=bytearray()
        while True:
            part=os.read(fd,MIB)
            if not part:break
            raw.extend(part);need(len(raw)<=limit,'input grew')
        last=os.fstat(fd);named=path.stat(follow_symlinks=False)
        facts=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns)
        need(facts(first)==facts(last)==facts(named) and len(raw)==first.st_size,'input changed')
        return bytes(raw),{'path':str(path),'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),'device':first.st_dev,
            'inode':first.st_ino,'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns}
    finally:os.close(fd)

def document(path,limit=64*MIB):
    raw,pin=read(path,limit)
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda v:need(False,'nonfinite JSON')),pin

def elf(common,path,kind=None):
    raw,pin=read(path,64*MIB)
    need(len(raw)>=64 and raw[:4]==b'\x7fELF' and raw[4:6]==b'\x02\x01' and int.from_bytes(raw[18:20],'little')==62,'actual fresh ELF64LE62 required')
    if kind is not None:need(int.from_bytes(raw[16:18],'little') in kind,'wrong ELF file type')
    # Decode actual kernel/linker layout from retained bytes. A flag in the
    # CMake recipe alone does not establish PT_INTERP, DT_RPATH or dependencies.
    phoff=int.from_bytes(raw[32:40],'little');entry=int.from_bytes(raw[54:56],'little');count=int.from_bytes(raw[56:58],'little')
    need(count<=1024 and (count==0 or entry==56) and phoff+entry*count<=len(raw),'bounded actual ELF program headers required')
    loads=[];interpreters=[];dynamic=[]
    for at in range(count):
        header=raw[phoff+entry*at:phoff+entry*(at+1)];tag=int.from_bytes(header[:4],'little')
        offset=int.from_bytes(header[8:16],'little');address=int.from_bytes(header[16:24],'little');size=int.from_bytes(header[32:40],'little')
        need(offset+size<=len(raw),'actual ELF segment outside file')
        if tag==1:loads.append((address,offset,size))
        if tag==3:
            value=raw[offset:offset+size];need(1<size<=4096 and value[-1:]==b'\0' and b'\0' not in value[:-1],'one bounded actual interpreter string required')
            interpreters.append(value[:-1].decode('ascii'))
        if tag==2:dynamic.append((offset,size))
    need(len(interpreters)<=1 and len(dynamic)<=1,'ambiguous ELF interpreter/dynamic segment')
    tags=[];strings={};needed=[];rpath=[];runpath=[]
    if dynamic:
        offset,size=dynamic[0];need(size%16==0 and size<=MIB,'bounded actual dynamic table required')
        terminated=False
        for at in range(offset,offset+size,16):
            tag=int.from_bytes(raw[at:at+8],'little',signed=True);value=int.from_bytes(raw[at+8:at+16],'little')
            if tag==0:terminated=True;break
            tags.append((tag,value))
        need(terminated,'unterminated ELF dynamic table')
        strtab=[v for t,v in tags if t==5];strsz=[v for t,v in tags if t==10]
        need(len(strtab)==len(strsz)==1 and 0<strsz[0]<=16*MIB,'actual bounded ELF dynamic string table required')
        matches=[offset+strtab[0]-address for address,offset,size in loads if address<=strtab[0] and strtab[0]+strsz[0]<=address+size]
        need(len(matches)==1,'ELF string virtual address has no unique file-backed segment')
        table=raw[matches[0]:matches[0]+strsz[0]]
        for tag,value in tags:
            if tag not in (1,14,15,29):continue
            need(value<len(table),'ELF string index out of bounds');end=table.find(b'\0',value);need(end>=value,'unterminated ELF string')
            strings.setdefault(tag,[]).append(table[value:end].decode('ascii'))
        needed=strings.get(1,[]);rpath=strings.get(15,[]);runpath=strings.get(29,[])
        need(len(rpath)<=1 and len(runpath)<=1,'duplicate ELF search-path tag')
    return {'input':pin,'machine':62,'bits':64,'endian':'little','type':int.from_bytes(raw[16:18],'little'),
            'PT_INTERP':interpreters,'DT_NEEDED':needed,'DT_RPATH':rpath,'DT_RUNPATH':runpath,
            'layout_basis':'ACTUAL_OPENED_ELF64_PROGRAM_HEADERS_AND_DYNAMIC_STRING_TABLE'}


REUSE_REPORTS = {'inner': {'path': '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source39-native-gateways-physics-peer-r1/report.json', 'bytes': 7986560, 'sha256': '75248cdddba3ab42f0020758cec81b0388c59d45ebd46d356ad434f6207e04d2', 'device': 81, 'inode': 677463, 'mtime_ns': 1792118141739107205, 'ctime_ns': 1792118141748107338}, 'outer': {'path': '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source39-native-gateways-physics-peer-root-supervisor-r1/report.json', 'bytes': 86618, 'sha256': '542a61a25de916899bbaa2e3ecbd57eb5583480fe13305826e16053d489e401c', 'device': 81, 'inode': 677496, 'mtime_ns': 1792118156347323565, 'ctime_ns': 1792118156352323639}}


def admit_reused_sdk(config, source, mf_pin):
    """Admit only Root's preserved failed-r1 COPY, never any old target object."""
    expected = REUSE_REPORTS
    need(type(config['reuse_execution_view']) is dict and config['reuse_execution_view'] == expected,
         'only the two exact Root-observed failed-r1 report full7s may select SDK reuse')
    docs = {}; pins = []
    for name, selected in expected.items():
        doc, pin = document(Path(selected['path']), 64*MIB)
        need(same_pin(pin, selected), 'original failed-r1 report full7 changed: '+name)
        docs[name] = doc; pins.append(pin)
    old, outer = docs['inner'], docs['outer']
    for name, doc in docs.items():
        need(doc.get('status') == 'FAIL' and doc.get('cleanup',{}).get('status') == 'PASS' and
             doc['cleanup'].get('remaining') == [] and doc.get('finalization_errors') == [] and
             all(doc.get(key) is True for key in ('source_preserved','inputs_preserved','products_preserved')),
             'only terminal failed-r1 reports with complete preservation and cleanup are admitted: '+name)
        need(doc['source'] == str(source) and doc['source_manifest_sha256'] == mf_pin['sha256'] and
             doc['source_before'] == doc['source_after'] and same_pin(mf_pin, doc['source_before']['manifest']),
             'failed-r1 source identity must remain its actual unchanged Source39')
    need(old.get('execution_view_preserved') is True and old.get('remaining_mounts') == [] and
         old.get('qualified_source_translation_units') == 72 and old.get('reuse') is False and
         old.get('error') == "ValueError('native launcher exited before cooperative mapped stop')",
         'exact original failed exporter attempt required; 72 old TUs are provenance only')
    need(outer['native_configuration'] == old['configuration'] and outer['worker']['sha256'] ==
         '9a083274f67ed2a91b74fe7ccf38bbbb249f29730c7e6bd4e738537ff8a2cbc4' and
         len(outer['commands']) == 1 and outer['commands'][0]['status'] == 'FAIL' and
         outer['commands'][0].get('returncode') == 1,
         'original outer must bind the actual r1 worker/configuration and failed direct child')
    original_out = Path(expected['inner']['path']).parent
    argv = outer['commands'][0]['argv']
    for option, value in (('--out',str(original_out)),('--config',old['configuration']['path']),
                          ('--config-sha256',old['configuration']['sha256']),('--source',str(source)),
                          ('--manifest',mf_pin['path']),('--manifest-sha256',mf_pin['sha256'])):
        need(argv.count(option) == 1 and argv[argv.index(option)+1] == value,
             'original failed child argument association changed: '+option)
    need(old['producer_reports'] == config['reports'], 'reused COPY must retain the same three admitted SDK/provider reports')
    original_config, pin = document(Path(old['configuration']['path']),4*MIB)
    need(same_pin(pin, old['configuration']) and set(original_config) ==
         {'format_version','kind','cgroup','reports','host_tools','host_input_roots','control_roots'},
         'original r1 configuration full7/schema changed')
    need(original_config['cgroup'] == HISTORICAL_CG and config['cgroup'] == CG, 'separate exact historical/current cgroup selections required')
    for key in ('format_version','kind','reports','host_tools','host_input_roots'):
        need(original_config[key] == config[key], 'SDK reuse cannot change original input selection: '+key)
    pins.append(pin)
    for selected in (outer['worker'],outer['control']):
        _, pin = read(Path(selected['path']),4*MIB);need(same_pin(pin, selected),'original r1 control/source changed');pins.append(pin)
    before, after = old['execution_view_before'], old['execution_view_after']
    need(len(before) == len(after) == 4 and all(
         {key:value for key,value in a.items() if key != 'catalog'} ==
         {key:value for key,value in b.items() if key != 'catalog'} for a,b in zip(before,after)),
         'original complete execution-view before/after identity differs')
    root = original_out/'rootfs'; catalog = after[0]
    need(catalog['root'] == str(root) and root.resolve(strict=True) == root and root.is_dir() and
         (catalog['files'],catalog['symlinks'],catalog['bytes'],catalog['content_sha256']) ==
         (32047,1265,1266016500,'3ef3dc72e08aa1341f6ce054b49c9065059c4831c8bd76fca9f4ab719ab334ca'),
         'only the complete actual preserved r1 physical rootfs may be reused')
    for row in (before[0],after[0]):
        _, pin = read(Path(row['catalog']['path']),64*MIB)
        need(same_pin(pin, row['catalog']),'original r1 SDK catalog full7 changed');pins.append(pin)
    ledger = Path(old['base_COPY']['ledger'])
    matches = [pin for pin in old['product_pins'] if pin['path'] == str(ledger)]
    need(len(matches) == 1 and ledger == original_out/'base-copy-bindings.jsonl' and
         old['base_COPY'] == {'file_bytes':1266016500,'files':32046,'ledger':str(ledger),'symlinks':1265},
         'complete original independent SDK COPY ledger required')
    _, pin = read(ledger,64*MIB);need(same_pin(pin, matches[0]),'original COPY ledger full7 changed');pins.append(pin)
    loader = old['execution_loader']['physical_loader']['input']
    need(Path(loader['path']).is_relative_to(root), 'original physical SDK loader escaped rootfs')
    _, pin = read(Path(loader['path']),64*MIB);need(same_pin(pin, loader),'original physical SDK loader changed');pins.append(pin)
    return {'scope':'PRESERVED_R1_SDK_COPY_ONLY_NO_COMPILED_OBJECT_OR_ELF_REUSE',
            'reports':expected,'original_status':'FAIL','original_qualified_TUs_provenance_only':72,
            'source_before':old['source_before'],'source_after':old['source_after'],
            'original_configuration':old['configuration'],'original_worker':outer['worker'],
            'original_COPY':old['base_COPY'],'original_catalog':catalog,
            'original_loader':loader,'root':str(root),'pins':pins,
            'compiled_object_reuse':False,'all_74_TUs_must_be_fresh':True}


def verify_reused_COPY(common, selection, base, guard):
    """Reopen every original source/COPY full7 through the exact bound ledger."""
    ledger = Path(selection['original_COPY']['ledger']);root = Path(selection['root'])
    selected = next(pin for pin in selection['pins'] if pin['path'] == str(ledger))
    need(common.opened(ledger,64*MIB) == selected,'original COPY ledger changed before streaming')
    names = set();files = links = total = 0
    with ledger.open('rb') as stream:
        while True:
            guard();raw = stream.readline(65537)
            if not raw:break
            need(len(raw) <= 65536 and raw.endswith(b'\n'),'bounded complete COPY ledger record required')
            row = json.loads(raw,object_pairs_hook=pairs)
            relative = row['relative'];part = Path(relative)
            need(type(relative) is str and not part.is_absolute() and '..' not in part.parts and
                 str(part) == relative and relative not in names,'ordinary distinct original COPY path required')
            names.add(relative);need(len(names) <= 65536,'COPY ledger record bound exceeded')
            a,b = base/relative,root/relative
            need(a.parent.resolve(strict=True).is_relative_to(base) and b.parent.resolve(strict=True).is_relative_to(root),
                 'original COPY input escaped admitted SDK/rootfs')
            if row['kind'] == 'file':
                need(set(row) == {'relative','kind','source','copy','identical_existing'} and
                     row['source']['path'] == str(a) and row['copy']['path'] == str(b) and
                     row['identical_existing'] is False,'closed original independent file COPY record required')
                need(same_pin(common.opened(a), row['source']) and same_pin(common.opened(b), row['copy']),
                     'original SDK source or independent COPY physical full7 changed')
                need(all(row['source'][key] == row['copy'][key] for key in ('bytes','sha256')) and
                     (row['source']['device'],row['source']['inode']) !=
                     (row['copy']['device'],row['copy']['inode']),'original COPY was not independent identical bytes')
                files += 1;total += row['source']['bytes']
            else:
                need(row['kind'] == 'symlink' and set(row) ==
                     {'relative','kind','target','source_device','source_inode','identical_existing'} and
                     row['identical_existing'] is False and a.is_symlink() and b.is_symlink() and
                     os.readlink(a) == os.readlink(b) == row['target'] and
                     RESTART.link_source(a,row['source_device'],row['source_inode'],row['target']),
                     'original source/COPY alias identity changed')
                links += 1
    need(common.opened(ledger,64*MIB) == selected and
         (files,links,total) == (32046,1265,1266016500),'complete original SDK COPY ledger changed')
    return {'ledger':selected,'source_root':str(base),'reused_root':str(root),
            'files':files,'symlinks':links,'file_bytes':total,
            'every_original_source_and_COPY_full7_reopened':True,'no_old_target_object_admitted':True}


def reused_directory_state(root, output, guard):
    """Retain current directory metadata too; mounting must not mutate old dirs."""
    count = 0;digest = hashlib.sha256()
    with output.open('xb') as stream:
        for directory,dirs,names in os.walk(root,followlinks=False):
            guard();dirs.sort();names.sort();path = Path(directory);info = path.lstat()
            need(stat.S_ISDIR(info.st_mode) and not path.is_symlink(),'ordinary reused SDK directory required')
            row = {'relative':str(path.relative_to(root)),'device':info.st_dev,'inode':info.st_ino,
                   'mode':info.st_mode,'nlink':info.st_nlink,'bytes':info.st_size,
                   'mtime_ns':info.st_mtime_ns,'ctime_ns':info.st_ctime_ns}
            raw = (json.dumps(row,sort_keys=True,separators=(',',':'))+'\n').encode()
            stream.write(raw);digest.update(raw);count += 1
            need(count <= 65536,'reused SDK directory metadata bound exceeded')
        stream.flush();os.fsync(stream.fileno())
    return {'root':str(root),'directories':count,'content_sha256':digest.hexdigest()}


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for key in ('source','manifest','config','out'):parser.add_argument('--'+key,type=Path,required=True)
    for key in ('manifest-sha256','config-sha256','parent-mount-namespace'):parser.add_argument('--'+key,required=True)
    args=parser.parse_args()
    need(sys.platform=='linux' and os.geteuid()==0 and os.uname().machine=='x86_64','actual Root native amd64 producer required')
    need(all(getattr(args,k).is_absolute() for k in ('source','manifest','config','out')) and all(re.fullmatch('[0-9a-f]{64}',getattr(args,k)) for k in ('manifest_sha256','config_sha256')),'absolute controls and actual SHA256 required')
    source=args.source.resolve(strict=True);manifest=args.manifest.resolve(strict=True)
    frozen,mf_pin=document(manifest,16*MIB);need(mf_pin['sha256']==args.manifest_sha256,'current actual complete source MF required')
    bodies={};helper_pins=[]
    for relative in ('tools/ros_distro/common.py','tests/ros_cgroup/probe.py','tools/ros_distro/acquire_sdk.py'):
        raw,pin=read(source/relative,2*MIB);need(frozen.get(relative)=={k:pin[k] for k in ('bytes','sha256')},'frozen helper differs');bodies[relative]=raw;helper_pins.append(pin)
    module=types.ModuleType('humble_gateway_frozen_acquire');module.__file__=str(source/'tools/ros_distro/acquire_sdk.py');sys.modules[module.__name__]=module
    exec(compile(bodies['tools/ros_distro/acquire_sdk.py'],module.__file__,'exec'),module.__dict__)
    acquire=module;common=acquire.load_common(source,manifest,args.manifest_sha256);cancelled=common.install_interrupts()
    need(callable(getattr(common,'pidfd_terminal',None)),'poll-based pidfd completion required')
    config,config_pin=document(args.config.resolve(strict=True),4*MIB)
    need(config_pin['sha256']==args.config_sha256 and set(config)=={'format_version','kind','cgroup','reports','host_tools','host_input_roots','control_roots','reuse_execution_view','restart_requalification'} and type(config['format_version']) is int and config['format_version']==1 and config['kind']=='uwvm-humble-gateway-portable-peer-build-inputs-v1' and config['cgroup']==CG,'closed current64 native build config required')
    install_restart(config,common,source,mf_pin)
    own_ns=os.readlink('/proc/self/ns/mnt');parent_ns=os.readlink('/proc/%d/ns/mnt'%os.getppid())
    need(parent_ns==args.parent_mount_namespace and own_ns!=parent_ns and re.fullmatch(r'mnt:\[\d+\]',parent_ns),'actual distinct direct-parent mount namespace required')
    need(set(config['reports'])=={'view','native','worker_registries'},'three actual producer reports required')
    packets={};packet_pins={}
    for key,value in config['reports'].items():
        need(set(value)==PIN_KEYS,'actual full7 report pin required');packet,pin=document(Path(value['path']),64*MIB);need(same_pin(pin,value),'actual report full pin changed');packets[key]=packet;packet_pins[key]=pin
    view,native,workers=(packets[k] for k in ('view','native','worker_registries'))
    need(packet_pins['view']['sha256']=='8c281ba2eec9ea5f7e83ae4a6eb8e1721f9f184c66b227e88894eb9b73905697' and view['status']=='COMPLETED_EXPLICIT_NATIVE_PREFLIGHTS_ONLY' and view.get('execution_view_preserved') is True and not view['remaining_mounts'],'actual preserved successful complete native SDK view required')
    need(packet_pins['native']['sha256']==NATIVE and native['status']=='PREPARED_NATIVE_INTERFACES_IDENTITIES_AND_SOURCE_PROVIDERS' and native.get('execution_sdk_preserved') is True and native.get('derived_products_preserved') is True,'actual three native families required')
    need(packet_pins['worker_registries']['sha256']=='bbbdc3c0121949fc5c30ab417d3a7f6c63e011430a632b803fe8a28e73a948a6' and workers['status']=='PREPARED_FOUR_NATIVE_INTERPRETER_WORKERS_WITH_EXPLICIT_REGISTRIES_NOT_EXECUTED','actual historical Source33 registry generation required')
    for key,packet in packets.items():
        need(packet['cleanup']['status']=='PASS' and not packet['cleanup']['remaining'] and not packet.get('remaining_mounts') and not packet.get('finalization_errors') and all(packet.get(k) is True for k in ('source_preserved','inputs_preserved','products_preserved')),'actual producer not fully clean/preserved: '+key)
    reused=admit_reused_sdk(config,source,mf_pin)
    rootfs=Path(reused['root'])
    base=Path(view['execution_view_after']['root']).resolve(strict=True)
    family_rows={row['family']:row for row in native['families']};need(set(family_rows)=={'unitree_go','unitree_hg','unitree_api'},'exact three native families required')
    family_roots={key:Path(row['derived_tree']['root']).resolve(strict=True) for key,row in family_rows.items()}
    generation_pins={'go-api':workers['ros2_generation'],'hg':family_rows['unitree_hg']['generation']}
    generated={key:Path(value['path']).resolve(strict=True).parent for key,value in generation_pins.items()}
    controls=[Path(p).resolve(strict=True) for p in config['control_roots']]
    roots=list(dict.fromkeys([Path(p).resolve(strict=True) for p in config['host_input_roots']]+[base,rootfs,*family_roots.values(),*generated.values(),*controls]))
    need(1<=len(roots)<=64 and all(p.is_dir() and p!=Path('/') for p in roots),'complete finite immutable input directories required')
    out=args.out;need(out.parent.resolve(strict=True)==out.parent and not out.exists() and not out.is_symlink() and all(not out.is_relative_to(p) and not p.is_relative_to(out) for p in [source,*roots]),'fresh canonical disjoint native output required')
    out.mkdir(mode=0o700);work=out/'work';staged=out/'controls'
    for p in (work,staged,out/'home',out/'tmp',out/'host-home',out/'host-tmp'):p.mkdir(mode=0o700)
    RESTART.start(out)
    report={'format_version':1,'schema':'uwvm-embedded-js-native-products-v1','kind':'humble-three-gateways-and-portable-peer','status':'RUNNING','source':str(source),'source_manifest_sha256':args.manifest_sha256,'configuration':config_pin,'producer_reports':packet_pins,'historical_producer_manifests':{k:p['source_manifest_sha256'] for k,p in packets.items()},'mount_namespace':{'current':own_ns,'actual_parent':parent_ns},'commands':[],'products':[],'qualified_source_translation_units':0,'installation_performed':False,'downloads_performed':False,'Runtime_executed':False,'DDS_initialized_by_worker':False,'ROS_WASI_grants_changed':False,'reuse':True,'reuse_scope':'SDK_COPY_ONLY_ALL74_TARGET_TUS_FRESH','compiled_object_reuse':False,'SDK_COPY_performed':False,'parallelism_policy':{'maximum_jobs':4,'per_heavy_job_reservation_bytes':5*GIB,'compile_command_aggregate_rss_bytes':20*GIB},'build_job_plans':[],'effective_rss_limits':{'coordinator_bytes':2*GIB,'configure_bytes':2*GIB,'native_compile_build_bytes':20*GIB,'identity_execution_bytes':2*GIB,'cleanup_command_bytes':512*MIB},'scope':'FRESH_NATIVE_GATEWAY_PEER_TYPED_WITNESS_AND_GO2_STAND_CONTROLLER_COMPILE_AND_STRING_METADATA_ONLY_NO_DDS_DELIVERY_RUNTIME_OR_PHYSICS_QUALIFICATION'}
    report['reused_execution_sdk']=reused
    report['host_only_control_roots']=[str(path) for path in controls]
    inputs={};products={};aliases={};before_roots=[];before_cg=source_before=copy_before=reused_dirs_before=None;runner=cleanup_runner=None;mounted=[]
    started=time.monotonic();last_guard=0.;finalizing=False
    def remember(path,expected=None):
        pin=common.opened(Path(path).resolve(strict=True),max(64*MIB,expected['bytes']) if expected else 128*MIB)
        need(expected is None or same_pin(pin,expected),'input full pin changed: '+pin['path'])
        need(pin['path'] not in inputs or inputs[pin['path']]==pin,'input changed');inputs[pin['path']]=pin;return pin
    def product(path,limit=64*MIB):
        pin=common.opened(path,limit);need(pin['path'] not in products or products[pin['path']]==pin,'product changed');products[pin['path']]=pin;return pin
    def guard(force=False):
        nonlocal last_guard
        now=time.monotonic();need(finalizing or not cancelled['cancelled'],'cooperative cancellation requested')
        need(finalizing or now-started<14400,'four-hour view/probe deadline exhausted')
        if not force and now-last_guard<.1:return
        last_guard=now
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024<=2*GIB,'coordinator exceeds2GiB')
        if before_cg:common.cgroup_preserved(before_cg,common.cgroup_state(CG))
        for path in (out,*out.parents):
            fs=os.statvfs(path);need(fs.f_bavail*fs.f_frsize>=8*GIB,'output/backing ancestor reserve below8GiB')
        need(os.readlink('/proc/self/ns/mnt')==own_ns,'owned private namespace changed')
    def save():
        report['restart_content_requalification']=RESTART.summary()
        report['input_pins']=list(inputs.values());report['product_pins']=list(products.values())
        if runner:report['commands']=runner.commands
        need(len(json.dumps(report,sort_keys=True).encode())<=64*MIB,'native product report exceeds bounded64MiB consumer contract')
        common.atomic_json(out/'report.json',report)
    hostenv={'PATH':'/usr/bin:/bin','LANG':'C.UTF-8','LC_ALL':'C.UTF-8','HOME':str(out/'host-home'),'TMPDIR':str(out/'host-tmp'),'PYTHONDONTWRITEBYTECODE':'1'}
    tools={}
    def run(label,argv,limits=None,environment=None):
        runner.limits=limits or {'rss_bytes':512*MIB,'command_seconds':30,'log_bytes':16*MIB}
        try:
            row=runner.run(label,argv,out,environment or hostenv);runner.discover()
            need(all(common.pidfd_terminal(item['fd']) for item in runner.owned.values()),'owned descendant remains after serial command')
            for key,item in list(runner.owned.items()):os.close(item['fd']);del runner.owned[key]
            for key,item in runner.unproven.items():
                now=common.identity(item['pid']);need(common.birth(now)!=key or now['state'] in ('Z','X'),'live unproven descendant remains')
            row['owned_terminal_barrier']={'all_observed_pidfds_terminal':True,'retained_Popen_roots_and_unproven_ledger':True}
            common.atomic_json(out/'commands.json',runner.commands)
            product(row['log']['path'],runner.limits['log_bytes']);return row
        finally:save()
    def mountpoint(logical,file=False):
        # No mkdir/touch/chmod in the preserved rootfs. All logical destinations
        # must already have been created by the admitted original r1 producer.
        path=Path(logical);need(path.is_absolute() and '..' not in path.parts and '\0' not in logical,'clean absolute existing mountpoint required')
        target=rootfs/path.relative_to('/')
        need(target.parent.resolve(strict=True).is_relative_to(rootfs) and not target.is_symlink(),
             'reused mountpoint must have an ordinary existing parent inside old rootfs')
        info=target.lstat()
        need(stat.S_ISREG(info.st_mode) if file else stat.S_ISDIR(info.st_mode),
             'reused SDK lacks an original compatible bind destination; do not create it')
        return target
    binds=[]
    def plan_bind(origin,logical,file=False,readonly=True):
        origin=Path(origin);target=mountpoint(str(logical),file)
        need(not any(target==row['destination'] for row in binds),'duplicate logical bind destination')
        binds.append({'origin':origin,'destination':target,'logical':str(logical),'readonly':readonly})
    def bind(row):
        mounted.append(row['destination'])
        run('bind-%03d'%len(mounted),[tools['mount'],'--bind',str(row['origin']),str(row['destination'])])
        if row['readonly']:run('readonly-%03d'%len(mounted),[tools['mount'],'-o','remount,bind,ro',str(row['destination'])])
    known={};derived_catalogs=[];execution_prefix_catalogs=[];loader_pin=None;native_libdirs=[]
    def native_product(path):
        actual=elf(common,path,{2,3});need(loader_pin is not None and actual['PT_INTERP']==[loader_pin['path']],'actual native PT_INTERP differs from selected COPY loader')
        need(actual['DT_RPATH']==[':'.join(native_libdirs)] and actual['DT_RUNPATH']==[],'actual native DT_RPATH must exactly select immutable COPY/prefix libraries, without RUNPATH fallback semantics')
        need(actual['DT_NEEDED'] and all('/' not in name and name not in ('.','..') for name in actual['DT_NEEDED']),'bounded ordinary dynamic SONAME dependencies required')
        product(actual['input']['path']);return actual
    host_env=hostenv
    def retire(row):
        runner.discover();row['terminal_owned_pidfds']=[]
        for key,item in list(runner.owned.items()):
            need(common.pidfd_terminal(item['fd']),'earlier native actor descendant remains live')
            row['terminal_owned_pidfds'].append({'identity':item['identity'],'identity_after':item['identity_after'],'pidfd_terminal':True});os.close(item['fd']);del runner.owned[key]
        for key,item in runner.unproven.items():
            now=common.identity(item['pid']);need(common.birth(now)!=key or now['state']=='Z','unproven live actor cannot be ignored')
    def native(prefix,argv):
        script='source /opt/ros/humble/setup.bash && if [[ -n "$1" ]]; then export LD_LIBRARY_PATH="$1/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"; fi && shift && exec "$@"'
        return [tools['chroot'],str(rootfs),'/bin/bash','--noprofile','--norc','-c',script,'humble-native-build',prefix,*argv]
    def translate(name,cwd='/owned'):
        logical=posixpath.normpath(name if name.startswith('/') else posixpath.join(cwd,name))
        maps=[(str(rootfs),rootfs),(str(source),source),*[(str(value),value) for value in family_roots.values()],*[(str(value),value) for value in generated.values()],('/source',source),*[("/owned/"+key,value) for key,value in family_roots.items()],*[("/providers/"+key,value) for key,value in generated.items()],('/owned',work),('/tmp',out/'tmp'),('/root',out/'home')]
        for prefix,physical in maps:
            if logical==prefix or logical.startswith(prefix+'/'):path=physical/logical[len(prefix):].lstrip('/');break
        else:
            need(any(logical==p or logical.startswith(p+'/') for p in ('/usr','/opt','/lib','/lib64','/bin','/sbin','/etc')),'unadmitted logical compiler input: '+logical);path=rootfs/logical.lstrip('/')
        resolved=path.resolve(strict=True);need(any(resolved==p or resolved.is_relative_to(p) for p in (source,work,rootfs,*family_roots.values(),*generated.values(),out/'tmp',out/'home')),'logical compiler input escaped immutable/owned roots');return resolved
    def add_known(catalog):
        for line in Path(catalog['catalog']['path']).open():
            row=json.loads(line)
            if row['kind']=='file':
                p=common.opened(Path(catalog['root'])/row['relative']);need(p['sha256']==row['sha256'] and p['bytes']==row['bytes'],'catalog changed while admitting maps');known[(p['device'],p['inode'])]=p

    def fresh_objects(build):
        # Configure is real, but its commands have not compiled these target
        # objects yet. Compiler-ID scratch outputs are deliberately not inputs.
        rows,pin=document(build/'compile_commands.json');objects=[]
        for row in rows:
            argv=row.get('arguments') or shlex.split(row['command']);need('-o' in argv,'actual object output missing')
            name=argv[len(argv)-1-list(reversed(argv)).index('-o')+1]
            logical=posixpath.normpath(name if name.startswith('/') else posixpath.join(row['directory'],name))
            need(logical.startswith('/owned/'),'target object outside fresh owned work')
            physical=work/logical[len('/owned/'):]
            need(not physical.exists() and not physical.is_symlink(),'configured object already existed before actual build')
            objects.append({'logical':logical,'physical':str(physical),'existed_before_build':False})
        need(objects and len({r['physical'] for r in objects})==len(objects),'actual distinct target object inventory missing')
        return {'compile_database':product(build/'compile_commands.json'),'objects':objects}

    def configure_query(build):
        # Query only configuration metadata; the actual compiler evidence below
        # must independently match every configured source/object bijectively.
        query=build/'.cmake/api/v1/query/codemodel-v2'
        put(query,'')

    def file_api_evidence(build,target_name):
        reply=build/'.cmake/api/v1/reply'
        indexes=list(reply.glob('index-*.json'))
        need(len(indexes)==1,'exactly one actual fresh CMake File API index required')
        index,ip=document(indexes[0]);product(indexes[0])
        selected=index.get('reply',{}).get('codemodel-v2',{})
        need(selected.get('kind')=='codemodel' and selected.get('version',{}).get('major')==2 and
             Path(selected.get('jsonFile','')).name==selected.get('jsonFile'),'actual codemodel-v2 reply required')
        model_path=reply/selected['jsonFile'];model,mp=document(model_path);product(model_path)
        need(len(model['configurations'])==1 and model['configurations'][0]['name']=='Release','one real Release codemodel configuration required')
        matches=[x for x in model['configurations'][0]['targets'] if x['name']==target_name]
        need(len(matches)==1 and Path(matches[0]['jsonFile']).name==matches[0]['jsonFile'],'exact native target File API selection')
        target_path=reply/matches[0]['jsonFile'];target,tp=document(target_path);product(target_path)
        need(target['name']==target_name and target['type']=='EXECUTABLE','actual executable target metadata required')
        source_base=model['paths']['source'];sources=[]
        for row in target['sources']:
            if 'compileGroupIndex' not in row:continue
            need(target['compileGroups'][row['compileGroupIndex']]['language']=='CXX','only actual CXX target sources admitted')
            physical=translate(row['path'],source_base)
            sources.append({'path':str(physical),'input':common.opened(physical),'generated':row.get('isGenerated',False)})
        rows,_=document(build/'compile_commands.json')
        configured=[str(translate(row['file'],row['directory'])) for row in rows]
        need(sources and len({r['path'] for r in sources})==len(sources) and
             sorted(configured)==sorted(r['path'] for r in sources),'File API and raw compile database are not a complete source bijection')
        return {'index':ip,'codemodel':mp,'target':tp,'sources':sources,
                'scope':'ACTUAL_CONFIGURED_SOURCE_SET_NOT_COMPILER_EXECUTION'}

    def selected_build_plan(label,maximum=4):
        # The existing aggregate compiler ceiling is now an explicit20GiB.
        # Four5GiB reservations are the upper bound, not CPU-only fanout.
        # Current cgroup/host memory and CPU quota can reduce every fresh batch.
        selection=RESTART.build_plan(maximum=maximum)
        selection.update(label=label,status='ADMITTED')
        report['build_job_plans'].append(selection);save()
        return selection

    def parallel_cmake_build(label,logical,target_name,maximum=1):
        selection=selected_build_plan(label,maximum)
        row=run(label,native('',[
            '/usr/bin/cmake','--build',logical,'--target',target_name,'--verbose','--',
            '-j'+str(selection['jobs'])]),
            {'rss_bytes':20*GIB,'command_seconds':600,'log_bytes':16*MIB})
        row['memory_parallelism']=selection
        return row

    def bounded_target_build(label,build,target_name,fresh):
        # Use the real generated target-specific Make rules. A bounded batch
        # contains at most four exact fresh object targets. GNU output-sync
        # keeps each target's raw compiler line and Make cwd markers together;
        # the unchanged parser still requires an exact complete compiler/MD
        # bijection and refuses ambiguous or interleaved evidence.
        logical='/owned/'+str(build.relative_to(work));parts=[];batches=[]
        build_make='CMakeFiles/'+target_name+'.dir/build.make'
        need((build/build_make).is_file(),'actual target-specific Make rule file missing')
        product(build/build_make)
        at=0
        while at<len(fresh['objects']):
            selection=selected_build_plan(label+'-objects-%03d'%at,
                                          min(4,len(fresh['objects'])-at))
            batch=fresh['objects'][at:at+selection['jobs']];targets=[]
            for item in batch:
                physical=Path(item['physical'])
                need(not physical.exists() and not physical.is_symlink() and physical.is_relative_to(build),
                     'fresh exact target object required')
                relative=str(physical.relative_to(build))
                need(relative.startswith('CMakeFiles/'+target_name+'.dir/') and relative.endswith('.o'),
                     'exact configured target object path required')
                targets.append(relative)
            row=run(label+'-objects-%03d-to-%03d'%(at,at+len(batch)-1),native('',[
                '/usr/bin/make','--print-directory','--output-sync=target','-C',logical,'-f',build_make,
                *targets,'VERBOSE=1','-j'+str(selection['jobs'])]),
                {'rss_bytes':20*GIB,'command_seconds':600,'log_bytes':16*MIB})
            need(all(Path(item['physical']).is_file() for item in batch),
                 'successful actual object batch omitted a selected target')
            row['memory_parallelism']=selection
            batches.append({'objects':batch,'plan':selection,'command_log':row['log']})
            parts.append(row['log']);at+=len(batch)
        # All target objects already exist. This command performs the original
        # CMake dependency/link step with a one-job cap, not a second compile.
        row=parallel_cmake_build(label+'-link',logical,target_name,1)
        parts.append(row['log'])
        joined=build/'actual-memory-bounded-build.log'
        with joined.open('xb') as outlog:
            size=0
            for part in parts:
                raw,pin=read(Path(part['path']),16*MIB);need(pin==part,'retained actual compiler/link raw log changed')
                need(raw.endswith(b'\n'),'complete actual per-job raw log line boundary required')
                size+=len(raw);need(size<=64*MIB,'combined actual build logs exceed64MiB')
                outlog.write(raw)
        return {'log':product(joined,64*MIB),'actual_raw_parts':parts,'object_batches':batches,
                'scope':'BYTE_EXACT_ORDERED_CONCATENATION_OF_SUCCESSFUL_MEMORY_BOUNDED_MAKE_BATCH_AND_CMAKE_LINK_LOGS'}

    def compile_evidence(build,build_log,optimization,strict_fp):
        database=build/'compile_commands.json';rows,pin=document(database);product(database);evidence=[];all_dependencies={}
        raw_log,log_pin=read(Path(build_log),64*MIB)
        text=raw_log.decode('utf-8','strict');logical_build='/owned/'+str(build.relative_to(work))
        # CMake's compilation database may omit the Make generator's implicit
        # -MT/-MF options. It is configuration evidence, NOT the executed full
        # argv. Match every actual verbose compiler command bijectively to its
        # database row after removing ONLY dependency-emission bookkeeping.
        # All assertions/optimization/FP/includes/source/object tokens stay exact.
        def without_dependency_options(argv):
            result=[];at=0
            while at<len(argv):
                token=argv[at]
                if token=='-MD':at+=1;continue
                if token in ('-MT','-MF'):
                    need(at+1<len(argv) and argv[at+1] and not argv[at+1].startswith('-'),'missing dependency-option operand')
                    at+=2;continue
                need(not token.startswith(('-MT','-MF')),'unsupported joined dependency-option spelling')
                result.append(token);at+=1
            return tuple(result)
        configured={}
        for index,row in enumerate(rows):
            argv=row.get('arguments') or shlex.split(row['command'])
            need(argv and argv[0] in ('/usr/bin/gcc','/usr/bin/g++'),'explicit selected SDK compiler required')
            need(posixpath.normpath(row['directory'])==logical_build,'database cwd differs from actual CMake build root')
            key=without_dependency_options(argv)
            need(key not in configured,'ambiguous duplicate configured compiler recipe')
            configured[key]=(index,row,argv)
        actual={};make_stack={};make_events=[]
        for number,line in enumerate(text.splitlines(),1):
            stripped=line.strip()
            event=re.fullmatch(r"(?:make|gmake)(?:\[(\d+)\])?: (Entering|Leaving) directory '([^']+)'",stripped)
            if event:
                level=int(event[1] or 0);directory=posixpath.normpath(event[3])
                need(directory.startswith('/') and (directory==logical_build or directory.startswith(logical_build+'/')),
                     'retained Make cwd outside exact build tree')
                if event[2]=='Entering':
                    need(level not in make_stack and (not make_stack or level==max(make_stack)+1),'ambiguous Make directory-enter sequence')
                    make_stack[level]=directory
                else:
                    need(make_stack and level==max(make_stack) and make_stack[level]==directory,'unmatched Make directory-leave sequence')
                    del make_stack[level]
                make_events.append({'line_number':number,'raw':line,'level':level,'operation':event[2],'cwd':directory})
                continue
            if not stripped.startswith(('/usr/bin/gcc ','/usr/bin/g++ ','cd ')):continue
            tokens=shlex.split(stripped)
            if tokens and tokens[0]=='cd':
                # Observed cd/generator/cmake_depends commands execute in an
                # isolated recipe shell; they do NOT change the Make process's
                # cwd. No unobserved cd+compiler syntax is silently admitted.
                need(not (len(tokens)>=4 and tokens[2]=='&&' and tokens[3] in ('/usr/bin/gcc','/usr/bin/g++')),
                     'unobserved cd-prefixed actual compiler command')
                continue
            if not tokens or tokens[0] not in ('/usr/bin/gcc','/usr/bin/g++') or '-c' not in tokens:continue
            need(not any(t in ('&&',';','|','>','>>','<') for t in tokens),'unsupported actual compiler shell operation')
            need(make_stack and make_stack[max(make_stack)]==logical_build,'actual compiler lacks retained current Make cwd proof')
            key=without_dependency_options(tokens)
            need(key in configured and key not in actual,'extra, repeated or unmatched actual verbose compiler command')
            actual[key]={'argv':tokens,'cwd':make_stack[max(make_stack)],'make_level':max(make_stack),
                         'raw':line,'line_number':number,'cwd_basis':'ACTUAL_RETAINED_MAKE_ENTER_LEAVE_CURRENT_DIRECTORY'}
        need(not make_stack,'retained successful Make build has an unclosed directory context')
        need(configured and set(actual)==set(configured),'actual verbose commands and configured target objects are not a complete bijection')
        for index,row in enumerate(rows):
            configured_argv=row.get('arguments') or shlex.split(row['command'])
            witness=actual[without_dependency_options(configured_argv)];cwd=witness['cwd'];argv=witness['argv']
            need('-c' in argv and '-MD' in argv and '-MMD' not in argv and argv.count('-MF')>=1 and argv.count('-MT')>=1 and '-o' in argv,'actual complete compiler MD/target argv required')
            # The explicit target options and CMake's implicit Make options can
            # legitimately repeat. GCC uses the last -MF output filename, while
            # every -MT operand contributes targets (it is NOT last-wins).
            # Preserve all requested values as well as the effective filename
            # and the actual emitted target list; no executed token is lost.
            mf_values=[argv[at+1] for at,token in enumerate(argv) if token=='-MF']
            mt_values=[argv[at+1] for at,token in enumerate(argv) if token=='-MT']
            need('-UNDEBUG' in argv and not any(a.startswith('-DNDEBUG') for a in argv),'native assertion mode was overridden')
            opts=[a for a in argv if re.fullmatch(r'-O(?:[0-3sgz]|fast)',a)]
            need(opts and opts[-1]==optimization,'actual selected native optimization mismatch')
            if strict_fp:
                need('-fno-fast-math' in argv and '-ffast-math' not in argv and '-Ofast' not in argv,'native package fast-math contract mismatch')
                modes=[a for a in argv if a.startswith('-ffp-contract=')]
                need(modes and modes[-1]=='-ffp-contract=off','native package floating-point contraction differs from matched controller')
            value=lambda flag:argv[len(argv)-1-list(reversed(argv)).index(flag)+1]
            obj=translate(value('-o'),cwd);dep=translate(value('-MF'),cwd)
            source_path=translate(row['file'],cwd);body,dep_pin=read(dep,16*MIB)
            need(translate(value('-MT'),cwd)==obj,'actual explicit compiler MD target differs from object')
            # Actual current GCC/CMake emits one make rule without -MP. Refuse
            # variables/unhandled extra rules rather than silently drop inputs.
            raw=body.decode().replace('\\\n','');need('$' not in raw and raw.count(':')==1,'unsupported compiler make dependency grammar')
            target,names=raw.split(':',1);dependencies=[]
            emitted_targets=[translate(name,cwd) for name in shlex.split(target)]
            requested_targets=[translate(name,cwd) for operand in mt_values for name in shlex.split(operand)]
            need(obj in emitted_targets and requested_targets and all(name in emitted_targets for name in requested_targets),
                 'actual dependency targets omit the object or an accumulated explicit MT target')
            dependency_options={'requested':{'MD_count':argv.count('-MD'),'MF':mf_values,'MT':mt_values},
                                'effective':{'MF_last_argument':mf_values[-1],'MF_physical_depfile':str(dep),
                                             'MT_accumulated_arguments':mt_values,'MT_last_argument':mt_values[-1],
                                             'MT_requested_physical_targets':[str(path) for path in requested_targets],
                                             'emitted_physical_targets':[str(path) for path in emitted_targets]},
                                'semantics':'LAST_MF_FILENAME_ALL_MT_TARGET_OPERANDS_ACCUMULATE'}
            for name in shlex.split(names):
                p=translate(name,cwd);dependencies.append(common.opened(p))
            need(dependencies and any(p['path']==str(source_path) for p in dependencies),'compiler source absent from actual MD')
            object_elf=elf(common,obj,{1});product(obj);product(dep)
            full_record={'configured_compile_database_row':row,'actual_compile':witness,
                         'physical_compile_cwd':str(translate(cwd)),'object':object_elf,'depfile':dep_pin,'dependency_options':dependency_options,
                         'source':common.opened(source_path),'dependencies':dependencies,
                         'source_of_argv':'ACTUAL_RETAINED_VERBOSE_COMPILER_LINE_BIJECTIVELY_MATCHED_TO_RAW_DATABASE'}
            # Keep the complete independently reopened full-MD header closure in
            # an immutable per-TU receipt. The bounded main report references
            # its actual full7 pin rather than duplicating huge SDK lists.
            receipt=build/('qualified-compile-%03d.json'%index)
            common.atomic_json(receipt,full_record)
            evidence.append({k:v for k,v in full_record.items() if k!='dependencies'})
            evidence[-1].update(full_MD_receipt=product(receipt,32*MIB),dependency_count=len(dependencies))
            for dependency in dependencies:
                old=all_dependencies.setdefault(dependency['path'],dependency)
                need(old==dependency,'shared actual compiler dependency changed between TUs')
        links=[]
        for path in sorted(build.rglob('link.txt')):
            raw,p=read(path,4*MIB)
            for line in raw.decode().splitlines():
                if line.strip():need(line.strip() in text,'configured link command not witnessed in actual verbose build log')
            links.append({'link_script':p,'actual_command_lines':raw.decode().splitlines()});product(path)
        need(links,'no actual native link command evidence')
        # GNU ld --trace prints one actual input pathname per line. Resolve
        # relative inputs against the real CMake build directory, never obj.parent.
        logical_build='/owned/'+str(build.relative_to(work));traced={};raw_rows=[]
        for line in text.splitlines():
            name=line.strip()
            if not re.fullmatch(r'[^\s]+\.(?:o|a|so(?:\.[0-9]+)*)',name):continue
            path=translate(name,logical_build);p=common.opened(path);traced[p['path']]=p
            raw_rows.append({'raw':line,'physical_input':p})
        object_names={row['object']['input']['path'] for row in evidence}
        need(object_names<=set(traced),'actual --trace omitted a compiled target object')
        need(any(p['path'].startswith(str(rootfs)+'/') for p in traced.values()),'actual SDK linker input trace absent')
        report['qualified_source_translation_units']+=len(evidence)
        return {'compile_database':pin,'compile_records':evidence,'link_commands':links,'actual_link_trace':raw_rows,
                'resolved_link_inputs':list(traced.values()),'actual_make_directory_events':make_events,'verbose_build_log':product(build_log,64*MIB),
                'compiled_dependency_paths':sorted(all_dependencies),
                'scope':'Actual full-MD, witnessed verbose link commands and physical --trace inputs. Runtime mappings checked separately; no configuration counted as executed.'}

    def export(family,path,prefix,selected):
        # Build tools run inside native()'s logical chroot. This linked target
        # instead names the exact physical COPY loader in its real PT_INTERP
        # and DT_RPATH. Execute that same physical ELF outside chroot; invoking
        # the loader as argv[0] would lose the actual target /proc/exe proof.
        need(family=='string-identity' and prefix=='' and
             path==work/'string-identity/export-build/unitree-native-identities',
             'only the closed String metadata exporter uses physical execution')
        path=path.resolve(strict=True);launch_image=native_product(path)
        need(common.opened(Path(loader_pin['path']))==loader_pin,
             'actual selected physical COPY interpreter changed before launch')
        export_environment={**host_env,**report['execution_environment_base']}
        need(export_environment['LD_LIBRARY_PATH']==':'.join(native_libdirs),
             'actual exporter library search must use the recorded COPY directories')
        argv=[str(path),'--mapped-stop']
        stdout=work/family/'native-metadata.json';stderr=work/family/'native-metadata.stderr'
        # Popen's child resolves this intentional kernel FD path while the
        # selected ordinary ELF FD is explicitly inherited. No shell, pathname
        # fallback, chroot launcher or foreign loader executes this target.
        executable_fd=os.open(path,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW|os.O_CLOEXEC)
        try:
            first=os.fstat(executable_fd)
            need(stat.S_ISREG(first.st_mode) and first.st_mode&0o111 and
                 all(getattr(first,name)==launch_image['input'][key] for name,key in
                   (('st_dev','device'),('st_ino','inode'),('st_size','bytes'),
                    ('st_mtime_ns','mtime_ns'),('st_ctime_ns','ctime_ns'))),
                 'opened physical exporter ELF differs from actual linked target')
            digest=hashlib.sha256();size=0
            while True:
                guard();block=os.read(executable_fd,MIB)
                if not block:break
                size+=len(block);need(size<=launch_image['input']['bytes'],'opened exporter grew')
                digest.update(block)
            last=os.fstat(executable_fd);named=path.stat(follow_symlinks=False)
            need(size==launch_image['input']['bytes'] and digest.hexdigest()==launch_image['input']['sha256'] and
                 all(getattr(first,name)==getattr(last,name)==getattr(named,name) for name in
                   ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')),
                 'full opened exporter bytes or physical identity changed')
            row={'label':family+'-actual-mapped-export','argv':argv,'cwd':str(out),
                 'environment':export_environment,'status':'RUNNING','expected_returncode':0,
                 'execution_method':'OPENED_PHYSICAL_ELF_WITH_RECORDED_COPY_PT_INTERP_AND_LIBRARY_PATH',
                 'opened_executable':launch_image['input'],'opened_executable_fd':executable_fd,
                 'Popen_executable':'/proc/self/fd/%d'%executable_fd,'pass_fds':[executable_fd],
                 'actual_ELF':launch_image,'actual_loader':loader_pin,
                 'stdout_path':str(stdout),'stderr_path':str(stderr),
                 'started_monotonic_ns':time.monotonic_ns(),'peak_owned_rss_bytes':0}
            runner.commands.append(row);save()
            with stdout.open('xb') as output,stderr.open('xb') as errors:
                child=subprocess.Popen(argv,executable='/proc/self/fd/%d'%executable_fd,
                    pass_fds=(executable_fd,),cwd=out,env=export_environment,
                    stdin=subprocess.PIPE,stdout=output,stderr=errors)
                # Retain Popen before log closure or any fallible ownership read.
                runner.children.append(child);row['pid']=child.pid
        finally:os.close(executable_fd)
        first=common.identity(child.pid);runner.roots[child.pid]=first;row['first_identity']=first
        runner.acquire(child.pid,os.getpid());need(any(k[0]==child.pid for k in runner.owned) or child.poll() is not None,'exporter launcher owned birth unproven')
        deadline=time.monotonic()+60;stop_until=time.monotonic()+20
        def check():
            guard();runner.discover();rss=runner.rss();row['peak_owned_rss_bytes']=max(row['peak_owned_rss_bytes'],rss)
            need(rss<=2*GIB and time.monotonic()<deadline,'export actor RSS/deadline exceeded')
            need(stdout.stat().st_size<=4*MIB and stderr.stat().st_size<=16*MIB,'export stdout/stderr bound exceeded')

        def opened_image(pid):
            proc=Path('/proc/%d/exe'%pid);link=os.readlink(proc)
            fd=os.open(proc,os.O_RDONLY|os.O_NONBLOCK)
            try:
                first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=64*MIB,'bounded actual native image required')
                digest=hashlib.sha256();size=0
                while True:
                    check();block=os.read(fd,MIB)
                    if not block:break
                    size+=len(block);need(size<=64*MIB,'actual native image exceeds64MiB');digest.update(block)
                last=os.fstat(fd);facts=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns)
                need(facts(first)==facts(last) and size==first.st_size,'actual native image changed while opening')
                return {'opened_path':str(proc),'link_text':link,'target_facts':{'device':first.st_dev,'inode':first.st_ino,'bytes':size,
                    'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns,'sha256':digest.hexdigest()}}
            finally:os.close(fd)

        def same_stopped(pid,key,item):
            check();now=common.identity(pid)
            need(runner.owned.get(key) is item and common.birth(now)==key and now['state'] in ('T','t') and
                 not common.pidfd_terminal(item['fd']) and child.poll() is None,'selected owned stopped birth/root changed')
            need(Path('/proc/%d/cgroup'%pid).read_text()==Path('/proc/self/cgroup').read_text(),'selected stopped process escaped current cgroup')
            return now

        def select_stopped(expected):
            observed=set();row['stopped_selection_observations']=[]
            while True:
                check();need(child.poll() is None,'native launcher exited before cooperative mapped stop');matches=[]
                # The trusted chroot may keep a waiting parent. Only already admitted
                # owned births can be candidates; no name/PID/ancestry fallback exists.
                for key,item in list(runner.owned.items()):
                    now=common.identity(key[0])
                    if common.birth(now)!=key or now['state'] not in ('T','t'):continue
                    image=opened_image(now['pid']);after=same_stopped(now['pid'],key,item)
                    mark=(key,image['target_facts']['sha256'])
                    if mark not in observed:
                        need(len(observed)<64,'bounded stopped candidate inventory exceeded');observed.add(mark)
                        row['stopped_selection_observations'].append({'first_ancestry':item['identity'],'after_pidfd':item['identity_after'],
                            'stopped':now,'after_image':after,'actual_exe':image,'is_popen_root':now['pid']==child.pid});save()
                    if image['target_facts']=={k:v for k,v in expected.items() if k!='path'}:matches.append((key,item,after,image))
                need(len(matches)<=1,'ambiguous multiple owned stopped processes match the exact native image')
                if matches:return matches[0]
                need(time.monotonic()<stop_until,'no exact owned cooperative mapped stop');time.sleep(.005)

        def opened_maps(key,item,stopped,image,expected):
            pid=key[0];same_stopped(pid,key,item)
            maps_path=Path('/proc/%d/maps'%pid);fd=os.open(maps_path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
            try:
                chunks=[];size=0
                while True:
                    block=os.read(fd,min(65536,16*MIB+1-size))
                    if not block:break
                    size+=len(block);need(size<=16*MIB,'bounded actual native maps required');chunks.append(block)
                raw=b''.join(chunks)
            finally:os.close(fd)
            text=raw.decode('utf-8','strict');map_files=[]
            # Persist the exact candidate and raw maps before ANY mapping assertion.
            # A failed proof must retain the physical evidence that failed, as r2 did.
            proof={'status':'MAPPING_PROOF_IN_PROGRESS','first_ancestry':item['identity'],'after_pidfd':item['identity_after'],
                'popen_root_pid':child.pid,'selected_pid':pid,'is_popen_root':pid==child.pid,'stopped':stopped,
                'actual_exe':image,'raw_maps':text,'map_files':map_files,'cooperative_barrier':False,
                'source':'ACTUAL_MAP_FILES_DESCRIPTOR_FSTAT_AND_FULL_SHA256_TO_PHYSICAL_KNOWN_CATALOG; RAW_MAPS_DEVICE_KEPT_SEPARATE'}
            row['mapped_proof']=proof;save();seen=set();mapped=[]
            for line in text.splitlines():
                fields=line.split(None,5);need(len(fields)>=5,'malformed actual maps row')
                if not int(fields[4]):continue
                need(re.fullmatch('[0-9a-f]+-[0-9a-f]+',fields[0]),'invalid actual mapping address range')
                record={'raw':line,'raw_maps_device':fields[3],'raw_maps_inode':int(fields[4]),
                    'map_files_path':'/proc/%d/map_files/%s'%(pid,fields[0]),'status':'OPENING'}
                map_files.append(record);fd=None
                try:
                    record['stopped_before']=same_stopped(pid,key,item)
                    record['link_text']=os.readlink(record['map_files_path'])
                    # A deliberate kernel magic-link open, under retained birth/T/
                    # cgroup/pidfd proof. O_NOFOLLOW would refuse the actual mapping.
                    fd=os.open(record['map_files_path'],os.O_RDONLY|os.O_NONBLOCK)
                    first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=64*MIB,'actual mapped object must be regular and<=64MiB')
                    digest=hashlib.sha256();size=0
                    while True:
                        same_stopped(pid,key,item);block=os.read(fd,MIB)
                        if not block:break
                        size+=len(block);need(size<=64*MIB,'actual mapped object exceeds64MiB');digest.update(block)
                    last=os.fstat(fd);facts=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns)
                    need(facts(first)==facts(last) and size==first.st_size,'actual mapped descriptor changed')
                    actual={'device':first.st_dev,'inode':first.st_ino,'bytes':size,'mtime_ns':first.st_mtime_ns,
                        'ctime_ns':first.st_ctime_ns,'sha256':digest.hexdigest()};record['opened_target_facts']=actual
                    # Raw maps displays a Btrfs superblock device on the observed host,
                    # while opened fstat identifies the subvolume. Never guess a
                    # conversion. Only the opened full facts identify a physical input.
                    physical_key=(actual['device'],actual['inode']);need(physical_key in known,'opened mapped object has no qualified physical identity')
                    pin=known[physical_key];need(actual=={k:v for k,v in pin.items() if k!='path'},'opened mapped object full facts/hash differ from physical input')
                    need(common.opened(Path(pin['path']),64*MIB)==pin,'physical known mapped input changed')
                    record['physical_input']=pin;record['stopped_after']=same_stopped(pid,key,item)
                    record['status']='OPENED_FULL_HASH_AND_PHYSICAL_PIN_VERIFIED';seen.add(physical_key)
                    mapped.append({'raw':line,'physical_input':pin,'map_files':record})
                except BaseException as error:
                    record.update(status='FAILED_MAP_FILES_PROOF',error=repr(error),error_type=type(error).__name__)
                    if isinstance(error,OSError):record.update(errno=error.errno,strerror=error.strerror)
                    save();raise
                finally:
                    if fd is not None:os.close(fd)
            need(map_files and (expected['device'],expected['inode']) in seen,'opened actual executable mapping absent')
            after=same_stopped(pid,key,item);last_image=opened_image(pid)
            need(last_image['target_facts']=={k:v for k,v in expected.items() if k!='path'},'stopped native image changed after mappings')
            after=same_stopped(pid,key,item)
            fd=os.open(maps_path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
            try:
                chunks=[];size=0
                while True:
                    block=os.read(fd,min(65536,16*MIB+1-size))
                    if not block:break
                    size+=len(block);need(size<=16*MIB,'final actual maps size bound');chunks.append(block)
                need(b''.join(chunks)==raw,'stopped native maps changed during opened mapping proof')
            finally:os.close(fd)
            proof.update(mapped=mapped,stopped_after_mapping_proof=after,actual_exe_after=last_image)
            return proof,seen

        wanted=common.opened(path);need(wanted==launch_image['input'],'actual exporter changed after opened-FD launch')
        key,record,now,actual=select_stopped(wanted)
        need(stdout.stat().st_size==0,'native metadata emitted before mapped proof')
        proof,seen=opened_maps(key,record,now,actual,wanted)
        need((loader_pin['device'],loader_pin['inode']) in seen,'actual selected COPY interpreter absent from opened mappings')
        for library in selected:
            pin=common.opened(library);identity=(pin['device'],pin['inode'])
            need(identity in seen and known.get(identity)==pin,'actual selected type-support library absent or changed')
        proof['stopped_after_mapping_proof']=same_stopped(key[0],key,record)
        proof.update(status='VERIFIED_ACTUAL_OPENED_NATIVE_MAPS',cooperative_barrier=True)
        row['mapped_proof']=proof;save()
        need(row.get('resume_count',0)==0,'cooperative exporter may resume only once')
        same_stopped(key[0],key,record)
        signal.pidfd_send_signal(record['fd'],signal.SIGCONT);row['resume_count']=1
        need(child.stdin.write(b'I')==1,'short explicit export gate');child.stdin.flush();child.stdin.close();row['gate_bytes_hex']='49'
        while child.poll() is None:check();time.sleep(.005)
        check();row['returncode']=child.returncode;row['stdout']=product(stdout,4*MIB);row['stderr']=product(stderr,16*MIB)
        need(child.returncode==0,'actual exporter nonzero');retire(row);row['status']='PASS';row['finished_monotonic_ns']=time.monotonic_ns();save()
        exported,_=document(stdout,4*MIB);need(set(exported)=={'format','ros_version','types'} and exported['format']=='uwvm-ros-official-bounded-native-v2' and exported['ros_version']==2,'actual native export shape invalid')
        return exported,row


    def put(path,text):
        path.parent.mkdir(parents=True,exist_ok=True)
        with path.open('x') as stream:stream.write(text)
        return product(path)
    def features(build,configure):
        paths={name:build/name for name in ('uwvm_ros2_features.json','uwvm_ros2_features.h','CMakeCache.txt')}
        data,pin=document(paths['uwvm_ros2_features.json']);cache={}
        need(set(data)=={'format_version','backend','cxx_standard','features'} and data['format_version']==1 and data['backend']=='ros2' and data['cxx_standard'] in (17,20,23) and all(type(v) is bool for v in data['features'].values()),'actual closed ROS2 capability JSON required')
        raw=read(paths['CMakeCache.txt'],16*MIB)[0].decode()
        for line in raw.splitlines():
            if not line or line.startswith(('#','//')):continue
            pair,sep,value=line.partition('=');key,colon,kind=pair.partition(':');need(sep and colon and key not in cache,'actual configure cache grammar/unique keys');cache[key]=value
        need(cache.get('CMAKE_HOME_DIRECTORY')=='/source/gateways/ros2' and cache.get('CMAKE_CXX_COMPILER')=='/usr/bin/g++' and cache.get('CMAKE_BUILD_TYPE')=='Release' and cache.get('UWVM_ROS2_CXX_STANDARD')=='AUTO' and cache.get('UWVM_ROS2_BUILD_PROBES')=='OFF' and cache.get('UWVM_ROS2_CXX_'+str(data['cxx_standard']))=='1','actual AUTO SDK compile/link standard/source/compiler evidence required')
        probe=source/'gateways/ros2/compat/Probe.cmake';template=source/'gateways/ros2/compat/uwvm_ros2_features.h.in'
        names=re.findall(r'"([a-z_]+);([A-Z_]+)"',read(probe,MIB)[0].decode())
        need(len(names)==16 and len(set(names))==16 and set(data['features'])=={name for name,macro in names},'all exact current16 capability fields required')
        for name,macro in names:
            value=cache.get('UWVM_ROS2_HAVE_'+macro+'_COMPILE_LINK');need(value in ('','0','1',None) and (value=='1')==data['features'][name] and (value is not None or macro=='OPENSSL' and data['features'][name] is False),'actual compile/link cache/feature mismatch')
        expected=read(template,MIB)[0].decode();need(expected.count('@CMAKE_CXX_STANDARD@')==1,'actual feature template standard directive')
        expected=expected.replace('@CMAKE_CXX_STANDARD@',str(data['cxx_standard']))
        for name,macro in names:expected=expected.replace('#cmakedefine01 UWVM_ROS2_HAVE_'+macro+'\n','#define UWVM_ROS2_HAVE_'+macro+' '+str(int(data['features'][name]))+'\n')
        need(read(paths['uwvm_ros2_features.h'],MIB)[0]==expected.encode() and '#cmake' not in expected and data['features']['quiet_node'] and data['features']['openssl_rihs'],'exact current generated feature bytes and required actual APIs/Crypto')
        return {'json':product(paths['uwvm_ros2_features.json']),'header':product(paths['uwvm_ros2_features.h']),'cache':product(paths['CMakeCache.txt']),'source_probe':remember(probe),'source_template':remember(template),'configure':configure,'actual':data,'scope':'ACTUAL_CONFIGURE_COMPILE_LINK_PROBES_NOT_TARGET_EXECUTION'}
    try:
        before_cg=common.cgroup_state(CG);report['cgroup_before']=before_cg
        need(all(row['events'].get(key,0)==0 for row in before_cg['ancestors'] for key in ('oom','oom_kill','oom_group_kill')),'initial ancestor OOM counters nonzero')
        report['retained_producer_content_provenance']=[RESTART.producer_provenance(k,p['cgroup_after'],own_ns,parent_ns) for k,p in packets.items()]
        source_before=common.source_state(source,manifest);need(source_before['manifest']==mf_pin,'initial MF full pin changed');report['source_before']=source_before
        for pin in (mf_pin,config_pin,*helper_pins,*packet_pins.values(),*RESTART.inputs):remember(pin['path'],pin)
        remember(Path(__file__))
        for pin in reused['pins']:remember(pin['path'],pin)
        report['reused_COPY_before']=verify_reused_COPY(common,reused,base,guard)
        reused_dirs_before=reused_directory_state(rootfs,out/'reused-sdk-directory-before.jsonl',guard)
        reused_dirs_before['catalog']=product(out/'reused-sdk-directory-before.jsonl')
        report['reused_sdk_directories_before']=reused_dirs_before
        for pin in view['input_pins']+view['product_pins']:remember(pin['path'],pin)
        need(set(config['host_tools'])=={'python','mount','umount','chroot'},'four actual host tools required')
        for key,value in config['host_tools'].items():
            need(set(value)=={'invocation','pin'} and set(value['pin'])==PIN_KEYS,'actual closed tool alias/full pin')
            path=Path(value['invocation']);need(path.is_absolute() and path.resolve(strict=True)==Path(value['pin']['path']) and any(path.resolve(strict=True).is_relative_to(p) for p in roots),'actual inventoried tool alias required')
            pin=remember(value['pin']['path'],value['pin']);header=read(Path(pin['path']),max(pin['bytes'],64*MIB))[0][:64];need(header[:6]==b'\x7fELF\x02\x01' and int.from_bytes(header[18:20],'little')==62,'native host ELF62 tool required')
            aliases[str(path)]=acquire.alias_binding(path);tools[key]=str(path)
        need(Path(tools['python']).resolve(strict=True)==Path(sys.executable).resolve(strict=True) and tools['chroot']=='/usr/bin/chroot','actual selected coordinator and chroot multicall spelling')
        args.chroot=Path(tools['chroot'])
        expected={base:view['execution_view_after'],rootfs:reused['original_catalog'],**{family_roots[k]:v['derived_tree'] for k,v in family_rows.items()}}
        for key,directory in generated.items():
            proof,pin=document(directory/'generation.json');need(same_pin(pin,generation_pins[key]),'actual native registry generation full pin changed')
            labels=('unitree_go','unitree_api') if key=='go-api' else ('unitree_hg',);wanted=set()
            for name in labels:
                profile,_=document(source/'sdk/robots/unitree/profiles'/(name+'.json'));wanted.update(row['type'] for row in profile['definitions'])
            need(proof['status']=='GENERATED_SOURCE_ONLY' and proof['native_registry_emitted'] is True and proof['implicit_permissions']==[] and proof['generator_sha256']==remember(source/'sdk/tools/custom_types.py')['sha256'] and proof['bounded_generator_sha256']==remember(source/'sdk/custom-types/bounded_profile.py')['sha256'],'current actual native-bound source generation required')
            need({row['type'] for row in proof['records']}==wanted and len(proof['records'])==len(wanted)==(32 if key=='go-api' else 11) and sum(row['expanded_nodes'] for row in proof['records'])==(14058 if key=='go-api' else 2353) and all(row['ros_version']==2 and re.fullmatch('RIHS01_[0-9a-f]{64}',row['native_identity']) for row in proof['records']),'complete actual family registry/identities/unchanged cell limits required')
            remember(pin['path'],pin)
            for name,value in proof['outputs'].items():
                need(Path(name).name==name,'closed generated native filename');actual=remember(directory/name);need(all(actual[k]==value[k] for k in ('bytes','sha256')),'actual native provider output changed')
            if key=='go-api':
                composition,composition_pin=document(Path(workers['composition']['path']));remember(composition_pin['path'],workers['composition'])
                need(composition['status']=='COMPOSED_SOURCE_ONLY' and composition['inputs_preserved'] is True and composition['selected_families']==['unitree_api','unitree_go'] and composition['runtime_permissions']==[],'actual Source33 GO+API composition required')
                candidates={row['path']:row for row in composition['outputs'] if all(row.get(k)==proof['source_inputs']['catalog'][k] for k in ('bytes','sha256')) and Path(row['path']).name=='catalog.json'}
                need(len(candidates)==1,'one actual composition-bound native source catalog required')
                catalog_pin=next(iter(candidates.values()));catalog_path=Path(catalog_pin['path']);catalog,_=document(catalog_path);remember(catalog_path,catalog_pin)
            else:
                # r7 preserves the entire derived tree, not a separate historical
                # full7 catalog pin. Admit its current physical identity only
                # after checking the generator's exact historical body digest;
                # the original complete tree is checked against its old catalog.
                catalog_path=family_roots['unitree_hg']/'bound/catalog.json';catalog,catalog_pin=document(catalog_path)
                need(all(catalog_pin[k]==proof['source_inputs']['catalog'][k] for k in ('bytes','sha256')),'actual HG catalog body differs from retained native generation');remember(catalog_path,catalog_pin)
                report['HG_catalog_pin_scope']='CURRENT_PHYSICAL_PIN_WITH_HISTORICAL_GENERATION_BODY_AND_WHOLE_DERIVED_TREE_ASSOCIATION_NOT_A_FABRICATED_HISTORICAL_PIN'
            need({row['type']:row['native_identity'] for row in catalog['types']}=={row['type']:row['native_identity'] for row in proof['types']},'actual native catalog/generation identities differ')
            catalog_root=catalog_path.parent.resolve(strict=True)
            if catalog_root not in roots:
                need(not out.is_relative_to(catalog_root) and not catalog_root.is_relative_to(out),'native catalog root/output overlap');roots.append(catalog_root)
            if key=='go-api':
                matches=[row for row in workers['fresh_provider_inputs_before'] if row['root']==str(catalog_root)]
                need(len(matches)==1,'actual original complete GO+API composition tree catalog required');expected[catalog_root]=matches[0]
            for definition in proof['source_inputs']['definitions']:
                path=(catalog_root/definition['file']).resolve(strict=True);need(path.is_relative_to(catalog_root),'native definition escaped actual catalog');actual=remember(path);need(all(actual[k]==definition[k] for k in ('bytes','sha256')),'actual native definition changed')
        need(len(roots)<=64,'too many complete actual roots')
        for at,path in enumerate(roots):
            guard(True);row=common.catalog(path,out/('input-before-%02d.jsonl'%at),roots);before_roots.append(row);product(row['catalog']['path'])
            if path in expected:need(RESTART.same_tree(row,expected[path]),'complete actual input tree differs from producer')
        report['input_roots_before']=before_roots
        report['base_COPY']={**reused['original_COPY'],'scope':'ORIGINAL_R1_COPY_PROVENANCE_ONLY_NO_NEW_COPY'}
        for name in family_roots:(work/name).mkdir(mode=0o700)
        plan_bind(work,'/owned',readonly=False);plan_bind(source,'/source');plan_bind(source,str(source));plan_bind(manifest,str(manifest),True)
        plan_bind(out/'tmp','/tmp',readonly=False);plan_bind(out/'home','/root',readonly=False)
        for key,path in family_roots.items():plan_bind(path,'/owned/'+key);plan_bind(path,str(path))
        for key,path in generated.items():plan_bind(path,'/providers/'+key);plan_bind(path,str(path))
        # Fresh r4 controls are host-only metadata. No compiler command needs
        # their new absolute paths inside the old immutable rootfs.
        plan_bind(Path('/proc'),'/proc');plan_bind(Path('/sys/fs/cgroup'),'/sys/fs/cgroup')
        for name in ('null','zero','urandom','random'):
            path=Path('/dev')/name;need(stat.S_ISCHR(path.stat().st_mode),'actual nonhardware character device required');plan_bind(path,'/dev/'+name,True)
        # Mirror the copied SDK's two real top-level library trees at their
        # absolute physical names inside the chroot. The GNU linker records the
        # actual COPY loader path as PT_INTERP, so the same ELF executes directly
        # outside chroot and still names itself in /proc/PID/exe. No loader argv
        # wrapper, ELF rewrite, host libc substitution or install occurs.
        for name in ('usr','opt'):
            origin=rootfs/name;need(origin.is_dir() and not origin.is_symlink(),'actual real SDK mirror root required');plan_bind(origin,str(origin))
        compiler=elf(common,translate('/usr/bin/g++'),{2,3})
        need(len(compiler['PT_INTERP'])==1 and Path(compiler['PT_INTERP'][0]).is_absolute(),'actual SDK compiler must identify its native loader')
        loader=translate(compiler['PT_INTERP'][0]);need(loader.is_relative_to(rootfs),'SDK loader escaped complete COPY')
        loader_elf=elf(common,loader,{2,3});need(loader_elf['PT_INTERP']==[],'actual native loader must not itself request another interpreter');loader_pin=loader_elf['input'];remember(loader,loader_pin)
        prefixes=[(family_roots[key]/'prefix').resolve(strict=True) for key in ('unitree_go','unitree_api','unitree_hg')]
        native_libdirs=list(dict.fromkeys(str(path.resolve(strict=True)) for path in [rootfs/'opt/ros/humble/lib',rootfs/'usr/lib/x86_64-linux-gnu',*[path/'lib' for path in prefixes]]))
        need(Path(loader_pin['path']).is_file() and all(Path(path).is_dir() for path in [*native_libdirs,*map(str,prefixes)]) and all(re.fullmatch(r'/[A-Za-z0-9_./+-]+',path) for path in [loader_pin['path'],*native_libdirs,*map(str,prefixes)]),'selected actual loader/library/prefix type or supported linker/CMake path differs')
        report['execution_environment_base']={'LD_LIBRARY_PATH':':'.join(native_libdirs),'AMENT_PREFIX_PATH':':'.join(map(str,[*prefixes,rootfs/'opt/ros/humble']))}
        report['execution_loader']={'compiler':compiler,'logical_compiler_PT_INTERP':compiler['PT_INTERP'][0],'physical_loader':loader_elf,'scope':'ACTUAL_COPIED_GLIBC_LOADER_NOT_HOST_LOADER_NO_ELF_REWRITE'}
        for index,prefix in enumerate(prefixes):
            row=common.catalog(prefix,out/('execution-prefix-before-%02d.jsonl'%index),roots);product(row['catalog']['path']);execution_prefix_catalogs.append(row)
        copy_before=common.catalog(rootfs,out/'execution-sdk-before.jsonl',[rootfs]);product(copy_before['catalog']['path']);report['execution_view_before']=[copy_before,*execution_prefix_catalogs]
        need(RESTART.same_tree(copy_before,reused['original_catalog']),'whole reused SDK no longer equals preserved original r1')
        add_known(copy_before)
        for row in execution_prefix_catalogs:add_known(row)
        for path in ('/usr/bin/gcc','/usr/bin/g++','/usr/bin/cmake','/usr/bin/make','/usr/bin/python3.10','/bin/bash'):
            actual=elf(common,translate(path),{2,3});report.setdefault('native_tools',[]).append({'logical':path,'physical':actual});remember(actual['input']['path'],actual['input'])
        for name,value in family_rows.items():add_known(value['derived_tree'])
        runner=common.SerialRunner(out,{'rss_bytes':2*GIB,'command_seconds':600,'log_bytes':16*MIB},guard)
        # Reuse the original physical SDK at its original path, private and
        # read-only. Child bind mounts exist only in this new mount namespace.
        # /owned, /tmp and /root select fresh r4 output directories.
        bind({'origin':rootfs,'destination':rootfs,'logical':'/','readonly':True})
        for row in binds:bind(row)
        link_flags='-Wl,--trace -Wl,--dynamic-linker='+loader_pin['path']+' -Wl,--disable-new-dtags -Wl,-rpath,'+':'.join(native_libdirs)
        # CMake-generated /owned build RPATHs are not usable in the host view.
        # Suppress only those automatic paths; the explicit GNU DT_RPATH above
        # remains in each actual link argv and is verified from the ELF bytes.
        common_flags=['-G','Unix Makefiles','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_COMPILER=/usr/bin/g++','-DCMAKE_MAKE_PROGRAM=/usr/bin/make','-DCMAKE_EXPORT_COMPILE_COMMANDS=ON','-DCMAKE_SKIP_RPATH=TRUE','-DCMAKE_EXE_LINKER_FLAGS='+link_flags,'-DPython3_EXECUTABLE=/usr/bin/python3','-DPYTHON_EXECUTABLE=/usr/bin/python3']
        report['native_link_contract']={'actual_link_flags':link_flags,'automatic_CMake_RPATH':False,'required_PT_INTERP':loader_pin,'required_DT_RPATH':native_libdirs,'exec_gate_environment':'HOST_WORKER_ENVIRONMENT_ONLY_NATIVE_ENVIRONMENT_IS_NEVER_MERGED_INTO_THE_GATE_OR_WORKER'}
        cmake_prefix='/owned/unitree_go/prefix;/owned/unitree_api/prefix;/owned/unitree_hg/prefix;/opt/ros/humble'
        report['gateway_profiles']=[]
        for key in ('builtin','go-api','hg'):
            home=work/'gateways'/key;home.mkdir(parents=True);logical='/owned/gateways/'+key
            selected=[] if key=='builtin' else ['-DUWVM_ROS_CUSTOM_REGISTRY_HEADER=/providers/'+key+'/manifest.hpp','-DUWVM_ROS_CUSTOM_ROS2_HEADER=/providers/'+key+'/native_ros2.hpp','-DUWVM_ROS_CUSTOM_ROS2_PACKAGES='+('unitree_go;unitree_api' if key=='go-api' else 'unitree_hg'),'-DUWVM_ROS2_CUSTOM_TOPIC_SHARDS=32']
            flags='-O3 -g0 -UNDEBUG -fno-fast-math -ffp-contract=off'
            argv=['/usr/bin/cmake','-S','/source/gateways/ros2','-B',logical+'/build',*common_flags,'-DCMAKE_CXX_FLAGS='+flags+' -MD','-DCMAKE_CXX_FLAGS_RELEASE='+flags,'-DCMAKE_PREFIX_PATH='+cmake_prefix,'-DUWVM_ROS2_CXX_STANDARD=AUTO','-DUWVM_ROS2_BUILD_PROBES=OFF',*selected]
            configure_query(home/'build')
            configured=run(key+'-gateway-configure',native('',argv),{'rss_bytes':2*GIB,'command_seconds':600,'log_bytes':16*MIB});captured=features(home/'build',configured);fresh=fresh_objects(home/'build')
            api=file_api_evidence(home/'build','uwvm-ros2-gateway')
            expected_TUs=1 if key=='builtin' else 34
            need(len(api['sources'])==len(fresh['objects'])==expected_TUs,'complete builtin or32-shard custom target source/object count differs')
            expected_sources={str(source/'gateways/ros2/main.cc')}
            if key!='builtin':
                expected_sources.add(str(source/'gateways/ros2/custom_topics_dispatch.cc'))
                template=source/'gateways/ros2/custom_topics_shard.cc.in'
                template_pin=remember(template)
                need(frozen.get('gateways/ros2/custom_topics_shard.cc.in')=={k:template_pin[k] for k in ('bytes','sha256')},'current shard template differs from actual complete manifest')
                template_raw=read(template,MIB)[0]
                need(template_raw.count(b'@UWVM_ROS2_CUSTOM_TOPIC_SHARD_INDEX@')==1 and
                     template_raw.count(b'@UWVM_ROS2_CUSTOM_TOPIC_SHARDS@')==1,'exact bounded maintained configure template required')
                for shard in range(32):
                    generated_source=home/'build/custom-topic-shards'/('custom_topics_shard_%d.cc'%shard)
                    actual_raw,_=read(generated_source,MIB)
                    expected_raw=template_raw.replace(b'@UWVM_ROS2_CUSTOM_TOPIC_SHARD_INDEX@',str(shard).encode()).replace(b'@UWVM_ROS2_CUSTOM_TOPIC_SHARDS@',b'32')
                    need(actual_raw==expected_raw,'actual configured shard body differs from maintained template/index/count')
                    product(generated_source);expected_sources.add(str(generated_source))
            need({r['path'] for r in api['sources']}==expected_sources,'File API configured source set drops or adds a typed shard/dispatcher')
            save()
            built=bounded_target_build(key+'-gateway-build',home/'build','uwvm-ros2-gateway',fresh)
            evidence=compile_evidence(home/'build',built['log']['path'],'-O3',True)
            evidence.update(file_api=api,actual_memory_bounded_build=built)
            need(len(evidence['compile_records'])==expected_TUs and
                 {r['source']['path'] for r in evidence['compile_records']}==expected_sources,
                 'every configured native source must have actual full-MD compiler evidence')
            if key!='builtin':
                for record in evidence['compile_records']:
                    definitions=[a for a in record['actual_compile']['argv'] if a.startswith('-DUWVM_ROS2_CUSTOM_TOPIC_SHARDS=')]
                    need(definitions==['-DUWVM_ROS2_CUSTOM_TOPIC_SHARDS=32'],'actual typed shard count definition differs across TUs')
                need(str(source/'gateways/ros2/custom_topics_impl.h') in evidence['compiled_dependency_paths'],
                     'actual custom shard MD must consume the heavy out-of-line implementation')
            binary=native_product(home/'build/uwvm-ros2-gateway')
            all_deps=set(evidence['compiled_dependency_paths'])
            need(str(source/'gateways/common/server.h') in all_deps and str(source/'src/uwvm2/imported/ros/ipc/event.h') in all_deps and str(source/'src/uwvm2/imported/ros/ipc/event_queue.h') in all_deps,'actual built Gateway must consume the full unified-event implementation')
            need(common.opened(home/'build/uwvm_ros2_features.h')==captured['header'] and common.opened(home/'build/uwvm_ros2_features.json')==captured['json'],'generated configure artifacts changed during compiler build')
            if key!='builtin':need(all(str(generated[key]/name) in all_deps for name in ('manifest.hpp','native_ros2.hpp','bounded_records.h')),'actual Gateway MD must consume complete selected native provider and registry')
            report['gateway_profiles'].append({'profile':key,'backend':2,'binary':binary['input'],'actual_elf':binary,'features':captured,'fresh_objects':fresh,'compile':evidence,'status':'BUILT_NOT_EXECUTED'});save()
        dependency_function='''function(selected_deps target)
  if(COMMAND ament_target_dependencies)
    ament_target_dependencies(${target} ${ARGN})
  else()
    foreach(package IN LISTS ARGN)
      if(TARGET ${package}::${package})
        target_link_libraries(${target} ${package}::${package})
      elseif(DEFINED ${package}_TARGETS AND NOT "${${package}_TARGETS}" STREQUAL "")
        target_link_libraries(${target} ${${package}_TARGETS})
      elseif(DEFINED ${package}_LIBRARIES AND NOT "${${package}_LIBRARIES}" STREQUAL "")
        target_include_directories(${target} SYSTEM PRIVATE ${${package}_INCLUDE_DIRS})
        target_link_libraries(${target} ${${package}_LIBRARIES})
      else()
        message(FATAL_ERROR "Actual selected SDK has no explicit link interface")
      endif()
    endforeach()
  endif()
endfunction()
'''
        peer_home=work/'peer';peer_project=peer_home/'project';peer_build=peer_home/'build';flags='-O1 -g0 -UNDEBUG -fno-fast-math -ffp-contract=off'
        put(peer_project/'CMakeLists.txt','''cmake_minimum_required(VERSION 3.16)
project(actual_humble_portable_peer LANGUAGES CXX)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rmw REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_typesupport_cpp REQUIRED)
'''+dependency_function+'''add_executable(portable-native-peer /source/tests/ros_javascript_guest/native_peer.cpp)
set_target_properties(portable-native-peer PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_include_directories(portable-native-peer PRIVATE /source/third-parties/fast_io/include)
target_compile_definitions(portable-native-peer PRIVATE UWVM_JS_NATIVE_BACKEND=2)
selected_deps(portable-native-peer rclcpp rmw std_msgs rosidl_typesupport_cpp)
target_link_libraries(portable-native-peer pthread)
''')
        argv=['/usr/bin/cmake','-S','/owned/peer/project','-B','/owned/peer/build',*common_flags,'-DCMAKE_CXX_FLAGS='+flags+' -MD','-DCMAKE_CXX_FLAGS_RELEASE='+flags,'-DCMAKE_PREFIX_PATH=/opt/ros/humble']
        run('portable-peer-configure',native('',argv),{'rss_bytes':2*GIB,'command_seconds':600,'log_bytes':16*MIB});fresh_peer=fresh_objects(peer_build)
        built=parallel_cmake_build('portable-peer-build','/owned/peer/build','portable-native-peer')
        peer_evidence=compile_evidence(peer_build,built['log']['path'],'-O1',True);peer=native_product(peer_build/'portable-native-peer')
        need(len(peer_evidence['compile_records'])==1 and peer_evidence['compile_records'][0]['source']==remember(source/'tests/ros_javascript_guest/native_peer.cpp') and '-DUWVM_JS_NATIVE_BACKEND=2' in peer_evidence['compile_records'][0]['actual_compile']['argv'] and '-std=c++20' in peer_evidence['compile_records'][0]['actual_compile']['argv'],'actual current portable peer, selected backend and C++20 required')
        gate_home=work/'exec-gate';gate_project=gate_home/'project';gate_build=gate_home/'build'
        put(gate_project/'CMakeLists.txt','''cmake_minimum_required(VERSION 3.16)
project(actual_host_environment_exec_gate LANGUAGES CXX)
add_executable(js-exec-gate /source/tests/ros_javascript_guest/exec_gate.cpp)
set_target_properties(js-exec-gate PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_include_directories(js-exec-gate PRIVATE /source/third-parties/fast_io/include)
target_link_libraries(js-exec-gate pthread)
''')
        argv=['/usr/bin/cmake','-S','/owned/exec-gate/project','-B','/owned/exec-gate/build',*common_flags,'-DCMAKE_CXX_FLAGS='+flags+' -MD','-DCMAKE_CXX_FLAGS_RELEASE='+flags]
        run('standalone-exec-gate-configure',native('',argv),{'rss_bytes':2*GIB,'command_seconds':600,'log_bytes':16*MIB});fresh_gate=fresh_objects(gate_build)
        built=parallel_cmake_build('standalone-exec-gate-build','/owned/exec-gate/build','js-exec-gate')
        gate_evidence=compile_evidence(gate_build,built['log']['path'],'-O1',True);gate=native_product(gate_build/'js-exec-gate')
        need(len(gate_evidence['compile_records'])==1 and gate_evidence['compile_records'][0]['source']==remember(source/'tests/ros_javascript_guest/exec_gate.cpp') and '-std=c++20' in gate_evidence['compile_records'][0]['actual_compile']['argv'],'actual current standalone execution gate C++20/full-MD required')
        report.update(exec_gate=gate['input'],exec_gate_source='tests/ros_javascript_guest/exec_gate.cpp',exec_gate_full_dependencies_verified=True,exec_gate_compile_commands=[gate_evidence],exec_gate_elf=gate,fresh_exec_gate_objects=fresh_gate)
        identity_home=work/'string-identity';selection=identity_home/'selection';export_build=identity_home/'export-build'
        put(selection/'selection.hpp','''#pragma once
#include <std_msgs/msg/string.hpp>
#include <uwvm2/imported/ros/ipc/event.h>
static_assert(uwvm2::imported::ros::ipc::event_version==1 && uwvm2::imported::ros::ipc::event_header_bytes==64);
#define UWVM_UNITREE_NATIVE_MESSAGES(X) X(std_msgs::msg::String,"std_msgs/msg/String")
''')
        put(selection/'CMakeLists.txt','''cmake_minimum_required(VERSION 3.16)
project(actual_humble_string_identity LANGUAGES CXX)
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_typesupport_cpp REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS Crypto)
'''+dependency_function+'''add_executable(unitree-native-identities /source/sdk/integrations/unitree/native_identity.cpp)
set_target_properties(unitree-native-identities PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_include_directories(unitree-native-identities PRIVATE /owned/string-identity/selection /source /source/src /owned/gateways/builtin/build)
target_compile_definitions(unitree-native-identities PRIVATE UWVM_UNITREE_ROS_VERSION=2)
selected_deps(unitree-native-identities std_msgs rosidl_typesupport_cpp rosidl_typesupport_introspection_cpp)
target_link_libraries(unitree-native-identities OpenSSL::Crypto pthread)
''')
        argv=['/usr/bin/cmake','-S','/owned/string-identity/selection','-B','/owned/string-identity/export-build',*common_flags,'-DCMAKE_CXX_FLAGS='+flags+' -MD','-DCMAKE_CXX_FLAGS_RELEASE='+flags,'-DCMAKE_PREFIX_PATH=/opt/ros/humble']
        run('actual-string-identity-configure',native('',argv),{'rss_bytes':2*GIB,'command_seconds':600,'log_bytes':16*MIB});fresh_identity=fresh_objects(export_build)
        built=parallel_cmake_build('actual-string-identity-build','/owned/string-identity/export-build','unitree-native-identities')
        identity_evidence=compile_evidence(export_build,built['log']['path'],'-O1',True);identity_binary=native_product(export_build/'unitree-native-identities');known[(identity_binary['input']['device'],identity_binary['input']['inode'])]=identity_binary['input']
        selected_libraries=[translate('/opt/ros/humble/lib/libstd_msgs'+suffix) for suffix in ('__rosidl_typesupport_cpp.so','__rosidl_typesupport_introspection_cpp.so')]
        exported,observation=export('string-identity',export_build/'unitree-native-identities','',selected_libraries)
        need(set(exported['types'])=={'std_msgs/msg/String'},'closed actual String native witness required');actual_identity=exported['types']['std_msgs/msg/String'];actual_hash=actual_identity['native_identity'];need(re.fullmatch('RIHS01_[0-9a-f]{64}',actual_hash),'actual native String identity malformed')
        message=read(source/'src/uwvm2/imported/ros/common/message.h',MIB)[0].decode();selector=re.search(r'if\(type == "std_msgs/msg/String"\) \{ return "(RIHS01_[0-9a-f]{64})"; \}',message);need(selector and selector[1]==actual_hash,'actual native String identity disagrees with frozen built-in contract')
        identity_doc=identity_home/'actual-interface-identity.json';put(identity_doc,json.dumps({'backend':2,'type':'std_msgs/msg/String','identity':actual_hash,'identity_source':actual_identity['identity_source'],'native_export':product(identity_home/'native-metadata.json'),'native_schema':actual_identity['components']['message']['schema'],'native_binary':identity_binary,'actual_mapping':observation,'source_cpp':remember(source/'sdk/integrations/unitree/native_identity.cpp'),'scope':'ACTUAL_NATIVE_INTROSPECTION_METADATA_NOT_DDS_DELIVERY'},indent=2)+'\n')
        for gateway in report['gateway_profiles']:
            feature_doc=work/'gateways'/gateway['profile']/'actual-native-features.json';feature_values=gateway['features']['actual']
            put(feature_doc,json.dumps({'backend':2,'topic_string':True,'unified_events_v1':True,'runtime_type_hash':actual_identity['identity_source']=='OFFICIAL_GET_TYPE_HASH','gateway_capabilities':feature_values,'actual_configure':gateway['features'],'topic_string_basis':product(identity_doc),'unified_event_basis':'ACTUAL_GATEWAY_FULL_MD_AND_LINKED_EXPORTER_EVENT_VERSION_STATIC_ASSERT','scope':'ACTUAL_COMPILE_LINK_AND_METADATA_NOT_ROS_DELIVERY'},indent=2)+'\n')
            report['products'].append({'backend':2,'profile':gateway['profile'],'gateway':gateway['binary'],'gateway_elf':gateway['actual_elf'],'peer':peer['input'],'peer_elf':peer,'features':product(feature_doc),'interface_identity':product(identity_doc),'peer_source':'tests/ros_javascript_guest/native_peer.cpp','definitions':{'UWVM_JS_NATIVE_BACKEND':2},'full_dependencies_verified':True,'compile_commands':[gateway['compile'],peer_evidence,identity_evidence],'fresh_peer_objects':fresh_peer,'fresh_identity_objects':fresh_identity,'source_scope':'CURRENT_GATEWAY_AND_PORTABLE_PEER_ONLY_HISTORICAL_WORKERS_NOT_RELABELLED'})
        typed_home=work/'typed-peer';typed_project=typed_home/'project';typed_build=typed_home/'build'
        typed_cmake=source/'tests/ros_unitree/native_typed_ros2_CMakeLists.txt'
        typed_source=source/'tests/ros_unitree/native_typed_ros2_peer.cpp'
        for path in (typed_cmake,typed_source):
            actual=remember(path)
            need(frozen.get(str(path.relative_to(source)))=={k:actual[k] for k in ('bytes','sha256')},'actual current typed native witness source differs from MF')
        put(typed_project/'CMakeLists.txt',read(typed_cmake,MIB)[0].decode('utf-8','strict'))
        configure_query(typed_build)
        flags='-O3 -g0 -UNDEBUG -fno-fast-math -ffp-contract=off'
        argv=['/usr/bin/cmake','-S','/owned/typed-peer/project','-B','/owned/typed-peer/build',*common_flags,
              '-DCMAKE_CXX_FLAGS='+flags+' -MD','-DCMAKE_CXX_FLAGS_RELEASE='+flags,
              '-DCMAKE_PREFIX_PATH='+cmake_prefix,'-DUWVM_SOURCE=/source','-DUWVM_UNITREE_NATIVE_CXX_STANDARD=20']
        run('typed-peer-configure',native('',argv),{'rss_bytes':2*GIB,'command_seconds':600,'log_bytes':16*MIB})
        typed_api=file_api_evidence(typed_build,'unitree-native-typed-peer');typed_fresh=fresh_objects(typed_build)
        need(len(typed_api['sources'])==len(typed_fresh['objects'])==1 and typed_api['sources'][0]['path']==str(typed_source),'exact real typed witness TU required')
        typed_built=bounded_target_build('typed-peer-build',typed_build,'unitree-native-typed-peer',typed_fresh)
        typed_evidence=compile_evidence(typed_build,typed_built['log']['path'],'-O3',True)
        typed_evidence.update(file_api=typed_api,actual_memory_bounded_build=typed_built)
        need(len(typed_evidence['compile_records'])==1 and typed_evidence['compile_records'][0]['source']==remember(typed_source) and
             '-std=c++20' in typed_evidence['compile_records'][0]['actual_compile']['argv'],'actual strict C++20 typed native witness compilation required')
        typed=native_product(typed_build/'unitree-native-typed-peer')
        report.update(typed_peer=typed,typed_peer_compile=typed_evidence,typed_peer_fresh_objects=typed_fresh,
                      typed_peer_protocol='UTN1/M/Q/P/A/Z/F',typed_peer_executed=False,typed_example_cases_qualified=False)
        # Compile the frozen native Go2 stand baseline as a separate ordinary
        # ROS2 artifact. This stage never starts it, initializes its Context,
        # publishes LowCmd, or claims a matched physics/DDS observation.
        physics_home=work/'physics-controller';physics_project=physics_home/'project';physics_build=physics_home/'build'
        physics_cmake=source/'tests/ros_unitree/native_ros2_CMakeLists.txt'
        physics_source=source/'tests/ros_unitree/native_ros2_controller.cpp'
        physics_shared=[source/'sdk/robots/unitree/include/roswasm_unitree'/name for name in
                        ('simulation_control.h','simulation_hold.h','crc.hpp')]
        for path in (physics_cmake,physics_source,*physics_shared):
            actual=remember(path)
            need(frozen.get(str(path.relative_to(source)))=={k:actual[k] for k in ('bytes','sha256')},
                 'actual frozen native physics controller source/kernel differs from MF')
        physics_cmake_raw=read(physics_cmake,MIB)[0]
        put(physics_project/'CMakeLists.txt',physics_cmake_raw.decode('utf-8','strict'))
        need(read(physics_project/'CMakeLists.txt',MIB)[0]==physics_cmake_raw,
             'native controller project must stage the exact maintained CMake bytes')
        configure_query(physics_build)
        flags='-O3 -g0 -UNDEBUG -fno-fast-math -ffp-contract=off'
        argv=['/usr/bin/cmake','-S','/owned/physics-controller/project','-B','/owned/physics-controller/build',*common_flags,
              '-DCMAKE_CXX_FLAGS='+flags+' -MD','-DCMAKE_CXX_FLAGS_RELEASE='+flags,
              '-DCMAKE_PREFIX_PATH='+cmake_prefix,'-DUWVM_SOURCE=/source','-DUWVM_UNITREE_NATIVE_CXX_STANDARD=20',
              '-DUWVM_UNITREE_NATIVE_FAMILY=unitree_go','-DUWVM_UNITREE_NATIVE_MODE=go2-stand']
        physics_configured=run('physics-controller-configure',native('',argv),
                               {'rss_bytes':2*GIB,'command_seconds':600,'log_bytes':16*MIB})
        physics_api=file_api_evidence(physics_build,'unitree-native-ros2-controller')
        physics_fresh=fresh_objects(physics_build)
        need(len(physics_api['sources'])==len(physics_fresh['objects'])==1 and
             physics_api['sources'][0]['path']==str(physics_source),'exact real native physics controller TU required')
        physics_built=bounded_target_build('physics-controller-build',physics_build,'unitree-native-ros2-controller',physics_fresh)
        physics_evidence=compile_evidence(physics_build,physics_built['log']['path'],'-O3',True)
        physics_evidence.update(file_api=physics_api,actual_memory_bounded_build=physics_built)
        need(len(physics_evidence['compile_records'])==1 and
             physics_evidence['compile_records'][0]['source']==remember(physics_source),
             'actual strict native physics controller compilation required')
        physics_argv=physics_evidence['compile_records'][0]['actual_compile']['argv']
        need('-std=c++20' in physics_argv and
             [a for a in physics_argv if a.startswith('-DUWVM_UNITREE_NATIVE_HG=')]==['-DUWVM_UNITREE_NATIVE_HG=0'] and
             [a for a in physics_argv if a.startswith('-DUWVM_UNITREE_NATIVE_HOLD=')]==['-DUWVM_UNITREE_NATIVE_HOLD=0'],
             'actual native Go2 stand/C++20 carrier and policy selection required')
        need({str(path) for path in physics_shared}<=set(physics_evidence['compiled_dependency_paths']),
             'actual physics controller full MD must consume the shared simulation kernels and CRC')
        physics=native_product(physics_build/'unitree-native-ros2-controller')
        physics_contract=physics_build/'native-controller-contract.txt'
        expected_physics_contract=('status=CONFIGURED_NOT_EXECUTED\nfamily=unitree_go\nmode=go2-stand\n'
            'cxx_standard=20\nshared_headers=/source/sdk/robots/unitree/include/roswasm_unitree\n'
            'artifact=/owned/physics-controller/build/unitree-native-ros2-controller\n'
            'protocol=UCK1-stand-or-UHL1-hold/R/Q\nrole=NATIVE_ROS2_NOT_WASM\n')
        need(read(physics_contract,MIB)[0].decode('utf-8','strict')==expected_physics_contract,
             'actual selected native controller configure contract differs')
        report.update(physics_controller=physics,physics_controller_compile=physics_evidence,
                      physics_controller_fresh_objects=physics_fresh,
                      physics_controller_contract={'configured':product(physics_contract),
                          'configure_command':physics_configured,'maintained_CMake':remember(physics_cmake),
                          'source':remember(physics_source),'family':'unitree_go','mode':'go2-stand',
                          'cxx_standard':20,'protocol':'UCK1/R/Q','shared_kernel_inputs':[remember(path) for path in physics_shared],
                          'scope':'FRESH_ORDINARY_NATIVE_ROS2_CONTROLLER_COMPILE_ONLY_NOT_WASM_OR_PHYSICS_DDS_QUALIFICATION'},
                      physics_controller_executed=False,physics_controller_DDS_initialized=False,
                      matched_physics_baseline_qualified=False)
        need(len(report['products'])==3 and report['qualified_source_translation_units']==74,
             'original73-TU products plus the separate native Go2 stand controller must be physically qualified')
        # Build a complete finite ordinary-file catalog for all native maps,
        # not just a few expected DSOs. Non-ELF mmap data must also be admitted
        # by exact opened facts; a pathname label never proves loaded bytes.
        execution_files={};execution_elfs={}
        for catalog in [copy_before,*execution_prefix_catalogs]:
            for line in Path(catalog['catalog']['path']).open():
                guard();need(len(line.encode())<=16*1024,'catalog row too long');row=json.loads(line,object_pairs_hook=pairs)
                if row['kind']!='file':continue
                path=(Path(catalog['root'])/row['relative']).resolve(strict=True);pin=common.opened(path)
                need(all(pin[key]==row[key] for key in ('bytes','sha256')),'execution file differs from whole native view catalog')
                need(pin['path'] not in execution_files or execution_files[pin['path']]==pin,'execution file duplicate changed');execution_files[pin['path']]=pin
        for pin in [*[row['binary'] for row in report['gateway_profiles']],peer['input'],identity_binary['input'],gate['input'],typed['input'],physics['input']]:execution_files[pin['path']]=pin
        ledger=out/'execution-files.jsonl';ledger_bytes=0
        with ledger.open('xb') as stream:
            for path,pin in sorted(execution_files.items()):
                guard();fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
                try:
                    before=os.fstat(fd);header=os.read(fd,64);after=os.fstat(fd);named=Path(path).stat(follow_symlinks=False)
                    facts=lambda item:(item.st_dev,item.st_ino,item.st_size,item.st_mtime_ns,item.st_ctime_ns)
                    need(facts(before)==facts(after)==facts(named)==tuple(pin[key] for key in ('device','inode','bytes','mtime_ns','ctime_ns')),'execution file changed during ELF classification')
                    if header[:4]==b'\x7fELF':
                        need(header[4:6]==b'\x02\x01' and len(header)>=64 and int.from_bytes(header[18:20],'little')==62,'non-native ELF in selected execution SDK/install closure')
                        execution_elfs[path]=pin
                finally:os.close(fd)
                raw=(json.dumps(pin,sort_keys=True,separators=(',',':'))+'\n').encode();need(len(raw)<=16*1024,'execution full7 line exceeds16KiB');ledger_bytes+=len(raw);need(ledger_bytes<=512*MIB,'execution file ledger exceeds512MiB');stream.write(raw)
        need(0<len(execution_files)<=1000000 and 0<len(execution_elfs)<=8192 and loader_pin['path'] in execution_elfs,'finite complete native file/ELF closure including actual loader required')
        report['execution_file_catalog']=product(ledger,512*MIB);report['execution_file_catalog_rows']=len(execution_files);report['execution_elf_catalog']=list(execution_elfs.values())
        report['preexisting_native_interface_libraries']={'actual_producer':packet_pins['native'],'prefixes':[str(path) for path in prefixes],'scope':'REUSED_ADMITTED_NATIVE_INTERFACE_LIBRARIES_ONLY_ALL_GATEWAY_PEER_GATE_IDENTITY_TYPED_AND_PHYSICS_CONTROLLER_OBJECTS_ARE_FRESH'}
        report['execution_mapping_scope']='ALL_ORDINARY_COPY_AND_THREE_NATIVE_INSTALL_FILES_PLUS_SELECTED_FRESH_EXECUTABLES_REQUIRE_CURRENT_OPENED_MAP_FD_MATCH_NO_UNKNOWN_HOST_ELF_OR_DATA_FALLBACK'
        for at,path in enumerate((work/'gateways',peer_home,gate_home,identity_home,typed_home,physics_home)):
            row=common.catalog(path,out/('derived-before-%02d.jsonl'%at),[rootfs,work,*roots]);product(row['catalog']['path']);derived_catalogs.append(row)
        report['derived_roots_before']=derived_catalogs;report['preparation_complete']=True
    except BaseException as error:
        report.update(status='FAIL',error=repr(error),traceback=traceback.format_exc())
    finally:
        finalizing=True;errors=[]
        def finish(label,action):
            try:action()
            except BaseException as error:report['status']='FAIL';errors.append({'stage':label,'error':repr(error)})
        def stop():
            report['cleanup']=runner.close() if runner else {'status':'PASS','remaining':[],'scope':'NO_ACTOR_STARTED'}
            need(report['cleanup']['status']=='PASS' and not report['cleanup']['remaining'],'owned process cleanup incomplete')
        finish('owned-processes',stop)
        if runner:
            for row in runner.commands:
                if 'log' in row:finish('retain-final-log',lambda row=row:product(row['log']['path'],16*MIB))
            if (out/'commands.json').exists():finish('retain-actor-command-ledger',lambda:product(out/'commands.json',64*MIB))
        def mountpoints():return {re.sub(r'\\([0-7]{3})',lambda m:chr(int(m[1],8)),line.split()[4]) for line in Path('/proc/self/mountinfo').read_text().splitlines()}
        try:
            cleanup_out=out/'mount-cleanup';cleanup_out.mkdir(mode=0o700)
            cleanup_runner=common.SerialRunner(cleanup_out,{'rss_bytes':512*MIB,'command_seconds':30,'log_bytes':16*MIB},guard)
        except BaseException as error:report['status']='FAIL';errors.append({'stage':'cleanup-owner','error':repr(error)})
        report['mount_cleanup']=[]
        if cleanup_runner:
            for index,target in enumerate(reversed(mounted)):
                def unmount(index=index,target=target):
                    if str(target) not in mountpoints():return
                    row=cleanup_runner.run('unmount-%03d'%index,[tools['umount'],str(target)],out,hostenv);report['mount_cleanup'].append(row)
                finish('unmount-'+str(target),unmount)
            def close_mount_owner():
                report['mount_cleanup_processes']=cleanup_runner.close();need(report['mount_cleanup_processes']['status']=='PASS' and not report['mount_cleanup_processes']['remaining'],'unmount actors remain')
            finish('unmount-processes',close_mount_owner)
            for row in cleanup_runner.commands:
                if 'log' in row:finish('retain-unmount-log',lambda row=row:product(row['log']['path'],16*MIB))
            report['mount_cleanup_commands']=cleanup_runner.commands
            if (cleanup_out/'commands.json').exists():finish('retain-mount-command-ledger',lambda:product(cleanup_out/'commands.json',64*MIB))
        def remaining_mounts():
            report['remaining_mounts']=[str(path) for path in mounted if str(path) in mountpoints()]
            need(not report['remaining_mounts'],'owned mounts remain')
        finish('remaining-mounts',remaining_mounts)
        def preserve():
            need(source_before is not None and len(before_roots)==len(roots),'complete initial preservation baselines missing')
            report['source_after']=common.source_state(source,manifest);need(report['source_after']==source_before,'current frozen source changed')
            after=[]
            for index,root in enumerate(roots):
                guard(True);row=common.catalog(root,out/('input-after-%02d.jsonl'%index),roots);product(row['catalog']['path']);after.append(row)
                need(row['content_sha256']==before_roots[index]['content_sha256'],'complete original input root changed')
            report['input_roots_after']=after
            current_derived=[]
            for index,old in enumerate(derived_catalogs):
                now=common.catalog(Path(old['root']),out/('derived-after-%02d.jsonl'%index),[rootfs,work,*roots]);product(now['catalog']['path']);need(now['content_sha256']==old['content_sha256'],'fresh native build tree changed');current_derived.append(now)
            report['derived_roots_after']=current_derived
            need(all(common.opened(Path(pin['path']),max(64*MIB,pin['bytes']))==pin for pin in inputs.values()),'old/control input pin changed')
            need(all(acquire.alias_binding(name)==value for name,value in aliases.items()),'selected host invocation alias changed')
            need(copy_before is not None and reused_dirs_before is not None,'complete reused SDK baselines missing')
            current=common.catalog(rootfs,out/'view-copy-after.jsonl',[rootfs]);product(current['catalog']['path'])
            need(current['content_sha256']==copy_before['content_sha256'],'new read-only SDK view changed')
            execution_after=[current]
            for index,old in enumerate(execution_prefix_catalogs):
                now=common.catalog(Path(old['root']),out/('execution-prefix-after-%02d.jsonl'%index),roots);product(now['catalog']['path']);need(now['content_sha256']==old['content_sha256'],'complete immutable native install prefix changed');execution_after.append(now)
            report['execution_view_after']=execution_after
            report['reused_COPY_after']=verify_reused_COPY(common,reused,base,guard)
            need(report['reused_COPY_after']==report['reused_COPY_before'],'original SDK/COPY provenance changed')
            dirs=reused_directory_state(rootfs,out/'reused-sdk-directory-after.jsonl',guard)
            need(dirs=={k:v for k,v in reused_dirs_before.items() if k!='catalog'},'old SDK directory metadata changed')
            dirs['catalog']=product(out/'reused-sdk-directory-after.jsonl');report['reused_sdk_directories_after']=dirs
            need(admit_reused_sdk(config,source,mf_pin)==reused,'old failed-r1 report/control/catalog/source pins changed')
            report['reused_execution_sdk_preserved']=True
            need(all(common.opened(Path(pin['path']),max(64*MIB,pin['bytes']))==pin for pin in products.values()),'new metadata/log product changed')
            report.update(source_preserved=True,inputs_preserved=True,products_preserved=True,execution_view_preserved=True)
        finish('preservation',preserve)
        def current_cgroup():
            report['cgroup_after']=common.cgroup_state(CG);need(before_cg is not None,'initial cgroup baseline missing');common.cgroup_preserved(before_cg,report['cgroup_after'])
        finish('current-cgroup',current_cgroup)
        def seal_restart():
            report['restart_content_requalification']=RESTART.finish()
            if RESTART.terminal_pin:product(RESTART.terminal_pin['path'],128*MIB)
            for tree in RESTART.trees.values():product(tree['current_catalog']['path'],64*MIB)
        finish('restart-selected-content-preservation',seal_restart)
        report['finalization_errors']=errors
        if report.get('preparation_complete') and report['status']=='RUNNING' and not errors:
            report['status']='PREPARED_NATIVE_PRODUCTS_NOT_EXECUTED'
        save()
    return 0 if report['status']=='PREPARED_NATIVE_PRODUCTS_NOT_EXECUTED' else 1


if __name__=='__main__':raise SystemExit(main())

