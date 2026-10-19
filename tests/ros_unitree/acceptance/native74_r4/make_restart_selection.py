#!/usr/bin/env python3
"""Select the exact retained native74 inputs for Root's restart requalification.

This is a metadata-only producer. It opens the actual preserved r3 selection,
its five exact historical producer reports and the Source39 manifest, then
writes a fresh closed configuration for requalify_after_restart.py. It runs no
compiler, process, namespace operation, ROS endpoint or network client. The six
old whole catalogs below are the exact trees whose semantic content native r4
consumes. Host tool roots are freshly inventoried before/after the later build;
their selected old executable pins are also explicit file inputs here.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import stat
import sys
import time
import types

sys.dont_write_bytecode = True
MIB=1024**2
GIB=1024**3
P='/var/tmp/uwvm-ros-complete-20261015-r1'
M=P+'/acceptance-storage-r1'
B='/var/tmp/uwvm-ros-unitree-20261016-r1'
SOURCE=P+'/inputs-r39/source'
MF=P+'/inputs-r39/source-manifest.json'
MF_SHA='54d6c79978e36bd13fa37272d8dd5f2cc94a9d2e62ac437b51dfc4aa4f6ae5b9'
R3_CONFIG=B+'/source39-native-gateways-physics-peer-controls-r3/native-config.json'
R3_CONFIG_SHA='41ec81cdef512a22eeacea1f704e4c6c675313741b3ad20702b4bb698f8d70da'
# These are old observations, preserved literally. Actual opens produce a
# separate new pin. Device numbers are never substituted into an old report.
REUSE={
 'inner':{'path':M+'/source39-native-gateways-physics-peer-r1/report.json','bytes':7986560,
 'sha256':'75248cdddba3ab42f0020758cec81b0388c59d45ebd46d356ad434f6207e04d2',
 'device':81,'inode':677463,'mtime_ns':1792118141739107205,'ctime_ns':1792118141748107338},
 'outer':{'path':M+'/source39-native-gateways-physics-peer-root-supervisor-r1/report.json','bytes':86618,
 'sha256':'542a61a25de916899bbaa2e3ecbd57eb5583480fe13305826e16053d489e401c',
 'device':81,'inode':677496,'mtime_ns':1792118156347323565,'ctime_ns':1792118156352323639}}
REPORT_SHAS={'view':'8c281ba2eec9ea5f7e83ae4a6eb8e1721f9f184c66b227e88894eb9b73905697',
 'native':'c52bc5ed98e52904f52e1f8dac7031cd65a53f94d5128375d8de2eea23ae4d73',
 'worker_registries':'bbbdc3c0121949fc5c30ab417d3a7f6c63e011430a632b803fe8a28e73a948a6'}
CG={'memory_max':64*GIB,'memory_swap_max':0,'expected_device':29,'expected_inode':18315,
 'expected_boot_id':'ec0d5f0f-3060-4220-bee2-2a7791ed4e72',
 'expected_leaf':'/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
 'expected_cpus':'0-31'}
PIN_KEYS={'path','bytes','sha256','device','inode','mtime_ns','ctime_ns'}


def need(value,reason):
    if not value:raise ValueError(reason)


def pairs(items):
    result={}
    for key,value in items:
        need(key not in result,'duplicate JSON key');result[key]=value
    return result


def opened(path,limit=64*MIB):
    path=Path(path)
    need(path.is_absolute() and path.resolve(strict=True)==path,'physical absolute input required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        first=os.fstat(fd)
        need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=limit,'bounded ordinary metadata required')
        data=bytearray()
        while True:
            block=os.read(fd,MIB)
            if not block:break
            data.extend(block);need(len(data)<=limit,'metadata grew')
        last=os.fstat(fd);named=path.stat(follow_symlinks=False)
        fields=('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')
        need(len(data)==first.st_size and all(getattr(first,key)==getattr(last,key)==getattr(named,key)
             for key in fields),'metadata changed during opened read')
        raw=bytes(data)
        return raw,{'path':str(path),'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),
            'device':first.st_dev,'inode':first.st_ino,'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns}
    finally:os.close(fd)


def document(path,expected_sha):
    raw,pin=opened(path)
    need(pin['sha256']==expected_sha,'selected historical metadata content changed: '+str(path))
    return json.loads(raw,object_pairs_hook=pairs),pin


def main():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    parser.add_argument('--out',type=Path,required=True,help='Fresh output directory on the current persistent filesystem')
    args=parser.parse_args()
    need(sys.platform=='linux' and hasattr(os,'sched_getaffinity'),'actual Linux metadata coordinator required')
    frozen,mf_pin=document(MF,MF_SHA)
    common_raw,common_pin=opened(Path(SOURCE)/'tools/ros_distro/common.py',MIB)
    need(frozen['tools/ros_distro/common.py']=={key:common_pin[key] for key in ('bytes','sha256')},
         'actual complete Source39 must bind the common cgroup helper')
    common=types.ModuleType('native74_restart_selection_frozen_common')
    common.__file__=common_pin['path']
    exec(compile(common_raw,common.__file__,'exec'),common.__dict__)
    before=common.cgroup_state(CG)
    started=time.monotonic()
    def guard():
        need(time.monotonic()-started<=600,'finite selection metadata deadline exceeded')
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024<=2*GIB,'selection metadata exceeds2GiB')
        common.cgroup_preserved(before,common.cgroup_state(CG))
    guard()
    config,config_pin=document(R3_CONFIG,R3_CONFIG_SHA)
    need(config['reuse_execution_view']==REUSE and set(config['reports'])==set(REPORT_SHAS),
         'exact original r3 SDK-only selection required')
    packets={};current_reports=[]
    for name,sha in REPORT_SHAS.items():
        old=config['reports'][name]
        need(old['sha256']==sha,'known native input report SHA changed')
        packet,pin=document(old['path'],sha)
        need(pin['bytes']==old['bytes'],'historical report byte size changed')
        packets[name]=packet;current_reports.append({'historical_pin':old,'current_pin':pin})
        guard()
    old,old_pin=document(REUSE['inner']['path'],REUSE['inner']['sha256'])
    outer,outer_pin=document(REUSE['outer']['path'],REUSE['outer']['sha256'])
    need(old.get('status')==outer.get('status')=='FAIL' and
         old.get('qualified_source_translation_units')==72 and old.get('execution_view_preserved') is True and
         old['source_manifest_sha256']==outer['source_manifest_sha256']==MF_SHA and
         old.get('cleanup',{}).get('status')==outer.get('cleanup',{}).get('status')=='PASS',
         'only preserved failed-r1 SDK provenance is selected; no r3 execution promotion')
    view,native,workers=(packets[key] for key in ('view','native','worker_registries'))
    families={row['family']:row for row in native['families']}
    need(set(families)=={'unitree_go','unitree_hg','unitree_api'},'exact native three-family source closure required')
    # The producer's GO+API generated catalog physically belongs to the
    # separately retained composition tree, not to one family by label.
    generation,generation_pin=document(workers['ros2_generation']['path'],workers['ros2_generation']['sha256'])
    composition,composition_pin=document(workers['composition']['path'],workers['composition']['sha256'])
    matches=[row for row in composition['outputs'] if Path(row['path']).name=='catalog.json' and
             all(row.get(key)==generation['source_inputs']['catalog'][key] for key in ('bytes','sha256'))]
    need(len(matches)==1,'one actual native GO+API generated catalog association required')
    composition_root=str(Path(matches[0]['path']).parent)
    matches_tree=[row for row in workers['fresh_provider_inputs_before'] if row['root']==composition_root]
    need(len(matches_tree)==1,'exact historical complete GO+API composition catalog required')
    trees=[('humble-source-sdk',view['execution_view_after']),
           ('preserved-r1-sdk-copy',old['execution_view_after'][0]),
           *[('native-'+name.replace('_','-'),families[name]['derived_tree'])
             for name in ('unitree_go','unitree_hg','unitree_api')],
           ('go-api-provider-composition',matches_tree[0])]
    need(len(trees)==6 and len({row['root'] for _,row in trees})==6,'six distinct exact consumed whole trees required')
    files=[];seen={}
    def add(label,pin):
        need(type(pin) is dict and set(pin)==PIN_KEYS,'explicit historical seven-field file pin required: '+label)
        if pin['path'] in seen:
            need(seen[pin['path']]==pin,'two historical identities for selected retained path')
            return
        need(len(files)<4096,'finite metadata selection exceeded')
        seen[pin['path']]=pin
        files.append({'label':label,'historical_pin':pin,'current_path':pin['path']})
    for name in REPORT_SHAS:add('producer-'+name.replace('_','-'),config['reports'][name])
    for name,pin in REUSE.items():add('failed-r1-'+name,pin)
    add('source39-manifest',old['source_before']['manifest'])
    add('original-r1-configuration',old['configuration'])
    add('original-r1-worker',outer['worker']);add('original-r1-control',outer['control'])
    add('original-host-tool-metadata',outer['host_metadata'])
    for name,row in config['host_tools'].items():add('host-tool-'+name,row['pin'])
    add('go-api-generation',workers['ros2_generation'])
    add('go-api-composition',workers['composition'])
    add('hg-generation',families['unitree_hg']['generation'])
    for at,row in enumerate(old['execution_view_before'][:1]+old['execution_view_after'][:1]):
        add('original-sdk-catalog-'+str(at),row['catalog'])
    ledger=[pin for pin in old['product_pins'] if pin['path']==old['base_COPY']['ledger']]
    need(len(ledger)==1,'one actual original COPY ledger required');add('original-sdk-copy-ledger',ledger[0])
    add('original-sdk-loader',old['execution_loader']['physical_loader']['input'])
    for label,row in trees:add(label+'-catalog',row['catalog'])
    # All known selections are populated from authenticated retained report
    # bytes. There are no guessed future physical pins in this configuration.
    selection={'format_version':1,'kind':'uwvm-ros-restart-content-selection',
        'historical_boot_id':'af121220-8c6e-4302-9853-f08bf4c50dbb','cgroup':CG,
        'limits':{'file_bytes':2*GIB,'catalog_bytes':64*MIB,'catalog_entries':100000,
                  'total_bytes':32*GIB,'seconds':3600},
        'files':files,'trees':[{'label':label,'historical_root':row['root'],
            'historical_catalog_pin':row['catalog'],'current_root':row['root']} for label,row in trees]}
    out=args.out
    need(out.is_absolute() and out.parent.resolve(strict=True)==out.parent and not out.exists() and
         not out.is_symlink() and all(not out.is_relative_to(Path(row['current_root'])) and
         not Path(row['current_root']).is_relative_to(out) for row in selection['trees']) and
         not out.is_relative_to(Path(SOURCE)),'fresh disjoint selection output required')
    out.mkdir(mode=0o700)
    guard()
    common.atomic_json(out/'restart-selection.json',selection)
    selection_pin=common.opened(out/'restart-selection.json')
    guard()
    report={'format_version':1,'kind':'native74-r4-restart-content-selection-maker',
        'status':'SELECTION_WRITTEN_NOT_REQUALIFIED','execution_qualification':'NOT_PERFORMED',
        'source_manifest_current_pin':mf_pin,'original_r3_config_current_pin':config_pin,
        'opened_historical_report_content':current_reports,
        'failed_r1_current_reports':[old_pin,outer_pin],
        'selection_current_pin':selection_pin,'selected_files':len(files),'selected_trees':len(trees),
        'selected_tree_roots':[row['current_root'] for row in selection['trees']],
        'host_input_tree_policy':'FRESH_CURRENT_BUILD_BEFORE_AFTER_CATALOGS_WITH_SELECTED_OLD_EXECUTABLE_PINS',
        'historical_r3_status':'INTERRUPTED_RUNNING_NOT_QUALIFIED_NOT_A_SOURCE_OF_SDK_OR_OBJECTS',
        'cgroup_before':before,'cgroup_after':common.cgroup_state(CG)}
    common.atomic_json(out/'report.json',report)
    print(json.dumps({'status':report['status'],'selection':selection_pin,'files':len(files),'trees':len(trees)}))


if __name__=='__main__':
    main()
