#!/usr/bin/env python3
"""Assemble symbol-only frozen-VM qualification, including explicit limitations."""
import argparse,csv,hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--base',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    b=a.base.resolve();names=['intel-jit-process-owner-second/results.json','intel-symbol-source-qualified/intel-jit-qualification.json',
        'vm-intel-symbol-qualified/full-build-results.json','intel-symbol-vm-qualified/results.json','intel-symbol-profile-qualified/profile.json',
        'intel-symbol-profile-qualified/source-line-attribution.json','intel-symbol-host-assembly/results.json','intel-symbol-host-instructions/results.json']
    receipts={n:json.loads((b/n).read_text()) for n in names};assert all(r['complete'] for r in receipts.values())
    build=receipts[names[2]];vm=b/'vm-intel-symbol-qualified/uwvm2-ros';assert sha(vm)==build['binary_sha256']
    frozen=Path(build['source_root']);manifest=Path(build['full_source_manifest']);assert sha(manifest)==build['full_source_manifest_sha256']
    assert all(sha(frozen/n)==digest for n,digest in json.loads(manifest.read_text()).items())
    profile=receipts[names[4]];source=receipts[names[5]];assembly=receipts[names[6]];instructions=receipts[names[7]]
    assert profile['runtime_sha256']==source['runtime_sha256']==assembly['runtime_sha256']==instructions['runtime_sha256']==sha(vm)
    assert source['profile_sha256']==assembly['profile_sha256']==instructions['profile_sha256']==sha(b/names[4])
    directory=b/'intel-symbol-profile-qualified'
    with (directory/'report-wasm-hotspots-hotspots.stdout').open() as stream:rows=list(csv.DictReader(stream,delimiter='\t'))
    total=sum(float(x['CPU Time']) for x in rows);guest=[x for x in rows if x['Function'].startswith('uwvm_m_')]
    known=[x for x in rows if not x['Function'].startswith('uwvm_m_') and 'Outside any known module' not in x['Function']]
    unknown=sum(float(x['CPU Time'])for x in rows if 'Outside any known module' in x['Function'])
    labels={x['symbol']:x['wasm_name_label'] for x in profile['guest_name_labels']}
    def summarize(values):return [dict(function=x['Function'],guest_name=labels.get(x['Function']),seconds=float(x['CPU Time']),fraction=float(x['CPU Time'])/total)for x in values[:5]]
    with (directory/'report-wasm-uarch-exploration-summary.stdout').open() as stream:metrics=list(csv.DictReader(stream,delimiter='\t'))
    frequency=float(next(x['Metric Value'] for x in metrics if x['Metric Name']=='Average CPU Frequency'))
    max_frequency=int(Path('/sys/devices/system/cpu/cpu8/cpufreq/cpuinfo_max_freq').read_text())*1000
    assert frequency>max_frequency
    window=[json.loads(line)for line in (directory/'window.jsonl').read_text().splitlines()]
    runtime=frozen/'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp';text=runtime.read_text().splitlines()
    chain={n:text[n-1]for n in [10558,10563,12440,12455,13643,13689]}
    assert 'publish_finalized' in chain[10558] and 'return false' in chain[10563] and 'fast_terminate' in chain[12455]
    report=dict(complete=True,scope='Linux x86_64 trusted frozen ordinary-full LLVM profiling qualification only; no production integration',
        runtime=str(vm),runtime_sha256=sha(vm),full_source_manifest_sha256=sha(manifest),source_files=build['full_source_files'],
        receipts={str(b/n):sha(b/n)for n in names},runner_sha256=sha(__file__),
        helper_commands=len(receipts[names[0]]['runs']),actual_vm_commands=len(receipts[names[3]]['commands']),
        owner_observations=receipts[names[3]]['observations'],official_collections=2,
        hotspot_fractions=dict(guest=sum(float(x['CPU Time'])for x in guest)/total,known_host=sum(float(x['CPU Time'])for x in known)/total,unknown=unknown/total),
        top_guest=summarize(guest),top_host=summarize(known),
        guest_instructions=sum(x['instructions']for x in profile['guest_instructions']),
        guest_event_sampled_instructions=sum(x['sampled_instructions']for x in profile['guest_instructions']),
        guest_instruction_reports=profile['guest_instructions'],host_assembly_artifacts=len(assembly['assembly']),
        host_instructions=sum(x['instructions']for x in instructions['functions']),
        host_event_sampled_instructions=sum(x['sampled_instructions']for x in instructions['functions']),
        host_instructions_with_frozen_source=sum(x['source_qualified_instructions']for x in instructions['functions']),
        source_rows=source['reports'],original_guest_source_mapping=False,near_native_performance_qualified=False,
        uarch_cpu_time_metrics=dict(qualified=False,reported_average_frequency_hz=frequency,kernel_max_frequency_hz=max_frequency,
            reason='Reported frequency exceeds the physical maximum; derived CPU-time/frequency/utilization are withheld. Address/event observations are not exact instruction latency.'),
        profiling_failure=dict(graceful_startup_qualified=False,actual_exit=-4,guest_entered=False,
            source=str(runtime),source_sha256=sha(runtime),fatal_chain=chain),
        window=dict(samples=len(window),memory_current_min=min(x['current']for x in window),memory_current_max=max(x['current']for x in window),
            events_before=profile['events_before'],events_after=profile['events_after'],quiet_baseline_qualified=False,
            trace_sha256=sha(directory/'window.jsonl'),processes_before_sha256=sha(directory/'processes-before.txt'),processes_after_sha256=sha(directory/'processes-after.txt')),
        retained_incomplete_attempts=[str(b/n)for n in ['intel-symbol-vm-first/results.json','intel-symbol-vm-second/results.json',
            'intel-symbol-vm-third/results.json','intel-symbol-vm-final/results.json']])
    assert all(not json.loads(Path(n).read_text())['complete'] for n in report['retained_incomplete_attempts'])
    out=a.output.resolve();assert not out.exists();out.write_text(json.dumps(report,indent=2)+'\n')
    print(out);print('PASS frozen symbol/assembly qualification; production/guest-source/near-native/graceful-startup remain unqualified')
if __name__=='__main__':main()
