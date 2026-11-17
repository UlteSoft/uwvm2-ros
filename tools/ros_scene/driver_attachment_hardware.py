#!/usr/bin/env python3
"""Audit explicit user-only hardware samples and import them into VTune.

This is source/assembly hardware evidence, not VTune top-down microarchitecture
qualification or a fixed wall-clock sampling promise. Client samples are
selected by their recorded PID; the separate receiver is not a native baseline.
Run serially through in_existing_cgroup.py, after a matched baseline benchmark.
"""
import argparse,json,platform,subprocess,sys
from pathlib import Path
from driver_attachment_performance import ROOT,VTUNE
from run_scene_vm import verify_gate,sha,GATE
from performance_build_policy import vm_policy
from hardware_sampling import audit_samples,reference_frequency

def main():
 parser=argparse.ArgumentParser(description=__doc__)
 for name in ('runtime','native','output','reference-context'):parser.add_argument('--'+name,required=True,type=Path)
 parser.add_argument('--iterations',type=int,default=300000)
 parser.add_argument('--reference-period',type=int,default=24000,help='Explicit REF_TSC sample-after count; larger fallback periods never qualify as 10 us')
 parser.add_argument('--instruction-period',type=int,default=240000)
 parser.add_argument('--tls',action='store_true')
 parser.add_argument('--cpu',type=int,default=2);parser.add_argument('--peer-cpu',type=int,default=4)
 a=parser.parse_args();verify_gate();a.output.mkdir(parents=True,exist_ok=False);out=a.output
 assert 0<a.reference_period<=2**31 and 0<a.instruction_period<=2**31
 hz=reference_frequency(a.reference_context,platform.node())
 build=json.loads((a.runtime.parent/'full-build-results.json').read_text());assert build['complete'] and build['binary_sha256']==sha(a.runtime)
 report=dict(complete=False,scope=__doc__,runtime_sha256=sha(a.runtime),native_sha256=sha(a.native),build_policy=vm_policy(a.runtime),
  reference_context=dict(path=str(a.reference_context),sha256=sha(a.reference_context),hz=hz),
  cgroup=str(GATE),events_before=(GATE/'memory.events').read_text(),commands=[],profiles=[])
 def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
 def run(label,argv):
  verify_gate();argv=list(map(str,argv));p=subprocess.run(argv,cwd=ROOT,capture_output=True,timeout=300)
  (out/(label+'.stdout')).write_bytes(p.stdout);(out/(label+'.stderr')).write_bytes(p.stderr)
  report['commands'].append(dict(name=label,argv=argv,exit=p.returncode));save()
  if p.returncode:raise RuntimeError(label+': '+p.stderr.decode(errors='replace')[-3000:])
  print('PASS '+label,flush=True);return p.stdout.decode()
 events={'CPU_CLK_UNHALTED.REF_TSC':('umask=0x3',a.reference_period),'INST_RETIRED.ANY':('event=0xc0',a.instruction_period)}
 config=','.join(f'cpu_core/{encoding},period={period},name={name}/u' for name,(encoding,period) in events.items())
 try:
  for kind in ('native','wasm'):
   trace=out/(kind+'.perf');target=out/(kind+'-vtune')
   stdout=run(kind+'-collect',['/usr/bin/perf','record','-o',trace,'-N','-B','-T','--sample-cpu','-e',config,'--',sys.executable,
    ROOT/'tools/ros_scene/driver_attachment_performance.py','--one',kind,'--runtime',a.runtime,'--native',a.native,'--output',out/(kind+'-run'),'--iterations',a.iterations,'--cpu',a.cpu,'--peer-cpu',a.peer_cpu]+(['--tls'] if a.tls else []))
   trial=json.loads(stdout.splitlines()[-1]);pid=trial['client_pid']
   attrs=run(kind+'-attributes',['/usr/bin/perf','evlist','-v','-i',trace])
   samples=run(kind+'-samples',['/usr/bin/perf','script','--ns','-F','comm,pid,tid,time,event,period,ip','-i',trace])
   raw=run(kind+'-raw',['/usr/bin/perf','script','-D','-i',trace])
   # taskset execs the target in the same PID; perf can sample both images.
   # Select the launched executable explicitly, never the first sampled comm.
   comm=(a.native if kind=='native' else a.runtime).name[:15]
   lines=[]
   for line in samples.splitlines():
    fields=line.split(maxsplit=2)
    if len(fields)==3 and fields[0]==comm and fields[1].partition('/')[0]==str(pid):
     lines.append(line)
   if not lines:raise ValueError(f'No samples for {comm} at client PID {pid}')
   selected='\n'.join(lines)+'\n';(out/(kind+'-client-samples.txt')).write_text(selected)
   audit=audit_samples(attrs,selected,raw,{name:period for name,(_,period) in events.items()},hz,target_comm=comm)
   row=dict(kind=kind,client_pid=pid,client_comm=comm,receiver_pid=trial['receiver_pid'],trace_sha256=sha(trace),audit=audit,source_mapping=False,assembly_mapping=False,report_filter='process-id='+str(pid))
   report['profiles'].append(row);save()
   run(kind+'-import',[VTUNE,'-import',trace,'-result-dir',target,'-source-search-dir',build['source_root'],'-source-search-dir',ROOT,'-finalization-mode','full'])
   for suffix,group in [('functions','function'),('source','function,source-file,source-line')]:
    run(kind+'-'+suffix,[VTUNE,'-report','hw-events','-result-dir',target,'-filter',row['report_filter'],'-group-by',group,'-format','csv','-report-output',out/(kind+'-'+suffix+'.tsv')])
   source=(out/(kind+'-source.tsv')).read_text();row['source_mapping']='driver_' in source
   functions=['uwvm2::imported::ros::scene::drivers::channel::receive']
   if a.tls:
    functions=(['uwvm2::imported::ros::scene::attachment_tls::session::receive'] if kind=='native' else
               ['uwvm2::imported::ros::scene::driver_dispatch::preflight'])
   elif kind=='wasm':functions.append('uwvm2::uwvm::imported::scene::invoke_driver<(uwvm2::imported::ros::scene::drivers::operation)2>')
   for index,function in enumerate(functions):
    dest=out/(kind+'-assembly-'+str(index)+'.tsv')
    run(kind+'-assembly-'+str(index),[VTUNE,'-report','hw-events','-result-dir',target,'-filter',row['report_filter'],'-source-object','function='+function,'-group-by','address','-column','Assembly,Source Line,Hardware Event Count','-format','csv','-report-output',dest])
    if '0x' in dest.read_text():row['assembly_mapping']=True
   save()
  report['complete']=all(row['audit']['periods_verified'] and row['source_mapping'] and row['assembly_mapping'] for row in report['profiles'])
  report['ten_us_reference_period_verified']=all(row['audit']['ten_us_reference_period_verified'] for row in report['profiles'])
  report['microarchitecture_analysis_complete']=False
 finally:report['events_after']=(GATE/'memory.events').read_text();save()
 if not report['complete']:raise SystemExit('Hardware evidence incomplete; inspect retained audit, no success inferred')
if __name__=='__main__':main()
