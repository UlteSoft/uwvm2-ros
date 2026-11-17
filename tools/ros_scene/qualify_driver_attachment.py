#!/usr/bin/env python3
"""Bind attachment evidence to one executable; never promote it to ecosystem coverage."""
import argparse,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'test/0084.ros-runtime'))
from run_scene_vm import GATE,verify_gate,sha

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for field in ('runtime','checks','performance','hardware','output'):p.add_argument('--'+field,required=True,type=Path)
    p.add_argument('--receiver',required=True,type=Path,help='Separate externally compiled shared-control receiver receipt')
    p.add_argument('--receiver-performance',required=True,type=Path)
    p.add_argument('--receiver-portability',required=True,type=Path)
    a=p.parse_args();verify_gate();runtime=a.runtime.resolve();out=a.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    binary_hash=sha(runtime);receipts={};cases={}
    def read(label,path):
        path=path.resolve();d=json.loads(path.read_text());assert d.get('complete'),(label,'incomplete')
        if 'runtime_sha256' in d:assert d['runtime_sha256']==binary_hash,(label,'different executable')
        assert d.get('cgroup')==str(GATE),(label,'different resource gate')
        before=d.get('events_before',d.get('memory_events_before'));after=d.get('events_after',d.get('memory_events_after'))
        if before is not None:assert before==after,(label,'resource pressure changed')
        receipts[label]=dict(path=str(path),sha256=sha(path));return d
    build=read('build',runtime.parent/'full-build-results.json');assert build['binary_sha256']==binary_hash
    source=Path(build['source_root']);manifest=source/'source-manifest.json';assert sha(manifest)==build['full_source_manifest_sha256']
    files=json.loads(manifest.read_text());assert all(sha(source/name)==digest for name,digest in files.items())
    current_scene=[name for name in files if name.startswith(('src/uwvm2/imported/ros/scene/','src/uwvm2/uwvm/imported/scene/'))]
    assert all(sha(ROOT/name)==files[name] for name in current_scene),'production scene changed after build'
    observed={str(path.relative_to(ROOT)) for folder in ('src/uwvm2/imported/ros/scene','src/uwvm2/uwvm/imported/scene') for path in (ROOT/folder).rglob('*') if path.is_file()}
    additions=observed-set(current_scene)
    assert additions=={'src/uwvm2/imported/ros/scene/driver_control.h'},('unrecorded scene sources',sorted(additions))
    checks=read('checks',a.checks/'results.json');assert checks['inputs_unchanged'] and checks['runtime_unchanged']
    for label,count in [('driver-vm',72),('driver-cli',21),('shared-vm',4),('module-vm',82),
        ('all-import-denials',342),('mujoco-ordinary',24),('mujoco-namespace',24),('mujoco-device',128)]:
        d=read(label,a.checks/label/'results.json');assert len(d['cases'])==count and all(row['passed'] for row in d['cases']),(label,'case mismatch');cases[label]=count
    for label in ('debug','modules-llvm','modules-interpreter'):read(label,a.checks/label/'results.json')
    for label in ('catalogue','allocators'):
        path=a.checks/(label+'.json');d=json.loads(path.read_text())
        assert d.get('status')=='PASS' if label=='catalogue' else d.get('complete') and not d.get('findings')
        receipts[label]=dict(path=str(path.resolve()),sha256=sha(path))
    assert all(row['exit']==0 for row in checks['commands'])
    performance=read('performance',a.performance/'results.json');assert performance['benchmark_complete']
    hardware=read('hardware',a.hardware/'results.json');assert hardware['ten_us_reference_period_verified']
    assert hardware['native_sha256']==performance['native_sha256']
    assert len(hardware['profiles'])==2 and all(row['audit']['periods_verified'] and row['source_mapping'] and row['assembly_mapping'] and row.get('report_filter')=='process-id='+str(row['client_pid']) for row in hardware['profiles'])
    receiver=read('receiver-control',a.receiver/'results.json')
    assert receiver['inputs_unchanged'] and all(sha(path)==digest for path,digest in receiver['inputs'].items())
    assert len(receiver['cases'])==8 and all(row['passed'] for row in receiver['cases'])
    assert all(row['exit']==0 for row in receiver['commands']);cases['receiver-control']=8
    receiver_performance=read('receiver-performance',a.receiver_performance/'results.json')
    assert receiver_performance['inputs_unchanged'] and all(sha(path)==digest for path,digest in receiver_performance['inputs'].items())
    assert len(receiver_performance['trials'])==22 and all(row['exit']==0 for row in receiver_performance['commands'])
    portability=read('receiver-portability',a.receiver_portability/'results.json')
    assert portability['inputs_unchanged'] and portability['snapshots_unchanged']
    assert all(sha(path)==digest for path,digest in portability['inputs'].items())
    assert set(portability['elf'])=={'x86_64','i686','aarch64','arm','armhf'} and all(row['exit']==0 for row in portability['commands'])
    ecosystem=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-ecosystem/reconciliation/summary.json';inventory=json.loads(ecosystem.read_text())
    report=dict(complete=True,scope='Exact attachment-only VM cohort and enumerated fixtures; not all ROS providers or industrial acceptance',
        runtime=str(runtime),runtime_sha256=binary_hash,source_root=str(source),source_manifest_sha256=sha(manifest),
        cgroup=str(GATE),memory_max=68719476736,swap_max=0,receipts=receipts,cases=cases,
        current_scene_sources_match=True,source_files=len(files),build_flags=['-O3','-g'],
        receiver_control=dict(compiled_into_vm=False,evidence=receipts['receiver-control'],
            separately_qualified_sources={name:sha(ROOT/name) for name in sorted(additions)},
            performance=dict(evidence=receipts['receiver-performance'],median_ns=receiver_performance['median_ns'],
                scope='Fixed-clock local admission/completion versus native arbitration; not IPC or native/Wasm equivalence'),
            portability=dict(evidence=receipts['receiver-portability'],architectures=sorted(portability['elf']),
                scope='Receiver control core correctness; ARM emulated; not full VM, channel or driver ABI qualification'),
            scope='External receiver helper; two actual VM clients, eight MuJoCo cases and seven sanitizer security groups; not physical stop acceptance'),
        performance=dict(median_ns=performance['median_ns'],wasm_to_native_ratio=performance['ratio'],
            trials=len(performance['trials']),request_bytes=17,response_bytes=24,metric='median of roundtrip batch means; not per-call p99'),
        hardware=dict(ten_us_reference_period_verified=True,fixed_wall_interval_guaranteed=False,
            microarchitecture_analysis_complete=hardware.get('microarchitecture_analysis_complete',False),
            profiles=[dict(kind=row['kind'],audit=row['audit'],source_mapping=row['source_mapping'],assembly_mapping=row['assembly_mapping']) for row in hardware['profiles']]),
        ecosystem=dict(path=str(ecosystem.resolve()),sha256=sha(ecosystem),summary=inventory),
        providers_complete=False,native_equivalence_proven=False,package_portability_proven=False,industrial_assurance_proven=False,
        outstanding=['External provisioning migration of existing model/DDS workers','General vendor adapters and supported-ABI qualification',
            'Vendor integration of the shared receiver arbiter and physical handover qualification','Independent hardware watchdog and physical stop acceptance',
            'Deployment execution-domain attestation and durable unknown-effect reconciliation','Exhaustive ROS source/semantic review and portability'])
    (out/'results.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(complete=True,cases=cases,performance=report['performance'])))

if __name__=='__main__':main()
