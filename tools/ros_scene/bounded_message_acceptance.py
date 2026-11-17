#!/usr/bin/env python3
"""Join exact-source bounded-message evidence without expanding its qualification scope.

Every selected receipt must be complete and use the existing 64 GiB zero-swap
cgroup. Core/native/corpus/fuzz/benchmark sources must match the final VM's
bounded-message implementation, and actual VM/DAP receipts must name that exact
binary. Named-module receipts remain focused host-export tests, not full graph
claims. This is no industrial certification, generic endpoint registration,
complete platform implementation or whole-ROS native-performance assertion.
"""
import argparse
import json
from pathlib import Path
from platform_agilex_checks import ROOT,GROUP,guard,sha

KEYS=('build','core','linux','native','corpus','fuzz','deployment','benchmark','instructions',
      'modules_llvm','modules_interpreter','allocators','vm_types','vm_robot','vm_cli','dap')
HEADER='src/uwvm2/imported/ros/scene/bounded_message.h'

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--plan',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();guard();temporary=(ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve();plan_path=a.plan.resolve();paths=json.loads(plan_path.read_text())
    assert set(paths)==set(KEYS);out=a.output.resolve();assert out.is_relative_to(temporary)and not out.exists()
    receipts={};pins={str(plan_path):sha(plan_path),str(Path(__file__).resolve()):sha(Path(__file__))}
    for key,path in paths.items():
        file=Path(path).resolve();assert file.is_relative_to(temporary);value=json.loads(file.read_text());assert value['complete'],key
        assert value['cgroup']==str(GROUP)and value['memory_max']==68719476736 and value.get('swap_max',value.get('memory_swap_max'))==0,key
        receipts[key]=value;pins[str(file)]=sha(file)
    build=receipts['build'];source=Path(build['source_root']);manifest=source/'source-manifest.json'
    assert build['optimization']=='O3'and build['debug']=='full -g'and build['sources_unchanged']
    assert sha(manifest)==build['full_source_manifest_sha256'];assert build['sources']==json.loads(manifest.read_text())
    assert all(sha(source/name)==digest for name,digest in build['sources'].items())
    binary=Path(paths['build']).parent/'uwvm2-ros';assert sha(binary)==build['binary_sha256']
    digest=build['sources'][HEADER];assert sha(ROOT/HEADER)==digest
    pins[str(binary)]=sha(binary);pins[str(manifest)]=sha(manifest)
    for key in ('core','linux','native','corpus','fuzz','deployment','benchmark'):
        value=receipts[key];assert value['snapshots'][HEADER]==digest,key
        snapshot=Path(paths[key]).parent/'source'
        assert all(sha(snapshot/name)==expected for name,expected in value['snapshots'].items()),key
        for name,expected in value['snapshots'].items():
            if name.startswith('src/uwvm2/imported/ros/scene/'):
                assert build['sources'][name]==expected,(key,name)
    for key,mode in [('modules_llvm','llvm'),('modules_interpreter','interpreter')]:
        value=receipts[key];assert value['host_fp_mode']==mode and value['isolated_host_export']
        assert value['full_source_manifest_sha256']==sha(manifest)and value['tracked_scene_sources_unchanged']
    allocator=receipts['allocators'];assert not allocator['findings']and allocator['inputs'][str(source/HEADER)]==digest
    for key in ('vm_types','vm_robot','dap'):
        value=receipts[key];assert value['runtime_sha256']==sha(binary),key
        assert value['inputs_unchanged'],key
    assert receipts['vm_cli']['inputs'][str(binary)]==sha(binary)
    assert all(row['passed']for key in ('vm_types','vm_robot','vm_cli')for row in receipts[key]['cases'])
    assert all(row['complete']for row in receipts['dap']['cases'])
    assert {'x86_64','i686','aarch64','arm','armhf'}==set(receipts['linux']['elf'])
    assert any('-runs=100000'in row['argv']for row in receipts['fuzz']['commands'])
    benchmark=Path(paths['benchmark']).parent/'benchmark';assert sha(benchmark)==receipts['benchmark']['binary_sha256']
    assert receipts['instructions']['inputs'][str(benchmark)]==sha(benchmark)
    assert receipts['instructions']['inputs'][str(Path(paths['benchmark']).resolve())]==sha(Path(paths['benchmark']))
    assert all(row['sampled_source_joined']>0 for row in receipts['instructions']['functions'])
    for key in ('hotspots','uarch-exploration'):assert key in receipts['benchmark']['profiles']
    result=dict(complete=True,scope=__doc__,receipts=paths,inputs=pins,source_manifest_sha256=sha(manifest),binary_sha256=sha(binary),
        bounded_message_sha256=digest,corpus_cases=receipts['corpus']['cases'],vm_cases={key:len(receipts[key]['cases'])for key in ('vm_types','vm_robot','vm_cli','dap')},
        performance=receipts['benchmark']['summary'],cgroup=str(GROUP),memory_max=68719476736,swap_max=0,
        generic_native_endpoints_qualified=False,production_vendor_modules_registered=0,industrial_certification=False)
    assert all(sha(Path(path))==expected for path,expected in pins.items())
    out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(result,indent=2)+'\n');print('PASS exact-source bounded-message evidence:',result['binary_sha256'])

if __name__=='__main__':main()
