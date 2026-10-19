#!/usr/bin/env python3
"""Scoped actual SDK encoder bytes/allocation validation, four C++20/26 SAN profiles."""
import argparse,hashlib,json,os,pathlib,shlex,signal,subprocess,sys,time,traceback,shutil
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source',type=pathlib.Path,default=pathlib.Path(__file__).resolve().parents[2])
parser.add_argument('--work',type=pathlib.Path,required=True,help='Fresh output directory; existing paths are refused')
parser.add_argument('--compiler',default='clang++')
parser.add_argument('--compiler-library-path',help='Optional LD_LIBRARY_PATH for the selected compiler/runtime')
args=parser.parse_args();S=args.source.resolve();W=args.work.resolve();W.mkdir(exist_ok=False)
C=shutil.which(args.compiler) or str(pathlib.Path(args.compiler).resolve())
ENV=dict(os.environ)
if args.compiler_library_path is not None:ENV['LD_LIBRARY_PATH']=args.compiler_library_path
P=pathlib.Path(__file__).resolve().parent
H=lambda p:hashlib.sha256(pathlib.Path(p).read_bytes()).hexdigest()
ENV.update(ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
copies={'event_encode_probe.cc':P/'unit.cc','before-encoder.inc':P/'before-encoder.inc','executed-driver.py':pathlib.Path(__file__),'source/sdk/common/codec.hpp':S/'sdk/common/codec.hpp'}
for rel,source in copies.items():
    p=W/rel;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(source.read_bytes())
manifest={'phase':'after','scope':'SDK typed parameter event encoder only; postfreeze permanent test driver','files':{rel:{'sha256':H(W/rel),'bytes':(W/rel).stat().st_size} for rel in copies}}
(W/'input-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
def cg(pid):
    membership=pathlib.Path('/proc',str(pid),'cgroup').read_text().splitlines();group=pathlib.Path('/sys/fs/cgroup')/next(v.split(':',2)[2] for v in membership if v.startswith('0::')).lstrip('/')
    rows=[];q=group
    while True:
        row={'path':str(q),'memory.max':None,'memory.events':None}
        for key in ['memory.max','memory.events']:
            try:row[key]=(q/key).read_text().strip()
            except (FileNotFoundError,PermissionError):pass
        rows.append(row)
        if q==pathlib.Path('/sys/fs/cgroup'):break
        q=q.parent
    maximum=min(int(v['memory.max']) for v in rows if v['memory.max'] not in (None,'max'))
    return {'pid':pid,'membership':membership,'effective_memory_max':maximum,'ancestors':rows,'memory_current':int((group/'memory.current').read_text()),'live_cgroup_procs':str(pid) in (group/'cgroup.procs').read_text().split()}
def inputs(dep):
    raw=pathlib.Path(dep).read_text();paths=shlex.split(raw.replace('\\\n',' ').split(':',1)[1]);result={}
    for name in paths:
        q=pathlib.Path(os.path.normpath(name));assert q.is_file(),name
        result[str(q)]={'sha256':H(q),'bytes':q.stat().st_size}
    assert result
    return {'raw_lexical_count':len(paths),'normalized_unique_count':len(result),'files':result}
def run(name,argv,timeout=240):
    log=W/(name+'.log');start=time.monotonic()
    with log.open('wb') as out:
        process=subprocess.Popen(argv,env=ENV,stdout=out,stderr=subprocess.STDOUT);children.append(process)
        proof=None;deadline=time.monotonic()+2
        while process.poll() is None and time.monotonic()<deadline:
            try:
                candidate=cg(process.pid);exe=os.readlink('/proc/'+str(process.pid)+'/exe')
                if os.path.realpath(exe)==os.path.realpath(argv[0]):proof={**candidate,'exe':exe};break
            except FileNotFoundError:pass
            time.sleep(.001)
        try:code=process.wait(timeout=timeout)
        except subprocess.TimeoutExpired:process.kill();process.wait();raise
    row={'name':name,'argv':argv,'returncode':code,'seconds':time.monotonic()-start,'log':str(log),'log_sha256':H(log),'compiler_actual_pid':proof};r['commands'].append(row)
    assert code==0,(name,code,log.read_text()[-1000:]);return row
phase=json.loads((W/'input-manifest.json').read_text())['phase'];after=phase=='after'
r={'status':'FAIL','scope':'SDK typed parameter event encoder only; no Worker/Gateway/ROS network execution claim','phase':phase,'executed_driver_sha256':H(__file__),'cgroup_before':cg(os.getpid()),'commands':[],'profiles':[],'input_manifest_sha256':H(W/'input-manifest.json'),'source_root':str(S)};children=[]
try:
    assert phase in ['before','after'] and r['cgroup_before']['effective_memory_max']==68719476736 and r['cgroup_before']['live_cgroup_procs']
    manifest=json.loads((W/'input-manifest.json').read_text())
    for rel,item in manifest['files'].items():assert H(W/rel)==item['sha256'] and (W/rel).stat().st_size==item['bytes'],rel
    version=run('compiler-version',[C,'--version'])
    plans=[('cpp20-std',20,False),('cpp26-std',26,False),('cpp20-libcxx',20,True),('cpp26-libcxx',26,True)]
    for name,standard,libcxx in plans:
        live=cg(os.getpid());budget={'effective_memory_max':live['effective_memory_max'],'memory_current':live['memory_current'],'compiler_gib':8,'reserve_gib':16,'jobs':1}
        (W/(name+'-build-budget.json')).write_text(json.dumps(budget,indent=2)+'\n');assert live['memory_current']+(8+16)*1024**3<=live['effective_memory_max'],budget
        binary=W/name;dep=W/(name+'.d')
        argv=[C,'-std=c++'+str(standard),'-O1','-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie','-pthread','-DSDK_AFTER='+str(int(after)),'-I',str(W/'source/sdk/common'),'-I',str(W),'-I',str(S/'third-parties/fast_io/include'),'-MMD','-MF',str(dep),str(W/'event_encode_probe.cc'),'-o',str(binary)]
        if libcxx:argv.insert(2,'-stdlib=libc++')
        build=run(name+'-build',argv);assert build['compiler_actual_pid'] and build['compiler_actual_pid']['effective_memory_max']==68719476736
        actual_inputs=inputs(dep);elf=binary.read_bytes();assert elf[:4]==b'\x7fELF' and int.from_bytes(elf[16:18], 'little')==2
        binary_sha=hashlib.sha256(elf).hexdigest()
        profile={'name':name,'standard':standard,'libcxx':libcxx,'binary':str(binary),'binary_sha256':binary_sha,'ELF_type':2,'raw_mmd':str(dep),'raw_mmd_sha256':H(dep),'compiled_inputs':actual_inputs};r['profiles'].append(profile)
        process=subprocess.Popen([str(binary)],env=ENV,stdout=subprocess.PIPE,stderr=subprocess.PIPE);children.append(process);deadline=time.monotonic()+5
        while True:
            assert process.poll() is None,'native exited before actual-main control point'
            status=pathlib.Path('/proc',str(process.pid),'status').read_text()
            if '\nState:\tT ' in status:break
            assert time.monotonic()<deadline,'actual-main control point timed out';time.sleep(.001)
        live=cg(process.pid);parent=int(next(v.split()[1] for v in status.splitlines() if v.startswith('PPid:')));exe=os.readlink('/proc/'+str(process.pid)+'/exe')
        with open('/proc/'+str(process.pid)+'/exe','rb') as f:stat=os.fstat(f.fileno());same=hashlib.sha256(f.read()).hexdigest()
        assert same==binary_sha and parent==os.getpid() and live['live_cgroup_procs'] and live['effective_memory_max']==68719476736 and exe==str(binary)
        main={**live,'parent_pid':parent,'exe':exe,'device':stat.st_dev,'inode':stat.st_ino,'sha256':same,'sampled_native_state':'T (stopped)','live_identity_captured_before_fd_hash':True}
        start=time.monotonic();os.kill(process.pid,signal.SIGCONT);stdout,stderr=process.communicate(timeout=30)
        (W/(name+'-run.log')).write_bytes(stdout);(W/(name+'-san.log')).write_bytes(stderr)
        row={'name':name+'-run','argv':[str(binary)],'returncode':process.returncode,'seconds':time.monotonic()-start,'log':str(W/(name+'-run.log')),'log_sha256':H(W/(name+'-run.log')),'san_log':str(W/(name+'-san.log')),'san_log_sha256':H(W/(name+'-san.log')),'actual_main':main};r['commands'].append(row)
        assert process.returncode==0 and not stderr,(name,process.returncode,stderr.decode()[-1000:])
        lines=stdout.decode().splitlines();summary=dict(v.split('=',1) for v in lines[-1].split() if '=' in v)
        assert lines[-1].startswith('PASS SDK event encode summary ') and summary=={'cases':'110','checks':'756' if after else '702','phase':phase},summary
        cases=[]
        for line in lines[:-1]:
            assert line.startswith('PASS case=');item=dict(v.split('=',1) for v in line.split()[1:]);cases.append(item)
            if after:assert int(item['allocations'])==(1 if item['accepted']=='1' else 0)
            elif item['accepted']=='1':assert int(item['allocations'])>0
            else:assert int(item['allocations'])==0
        assert len(cases)==110 and len({v['case'] for v in cases})==110
        assert sum(v['accepted']=='1' for v in cases)==54
        profile.update(cases=cases,case_count=110,checks=int(summary['checks']),actual_main=main)
    assert len(r['commands'])==9 and all(v['returncode']==0 for v in r['commands']);r['status']='PASS'
except Exception as e:r.update(error=str(e),traceback=traceback.format_exc())
finally:
    for process in children:
        if process.poll() is None:process.kill();process.wait(timeout=5)
    r['cgroup_after']=cg(os.getpid());r['remaining_owned_processes']=[v.pid for v in children if v.poll() is None];r['oom_deltas']=[]
    for x,y in zip(r['cgroup_before']['ancestors'],r['cgroup_after']['ancestors']):
        assert x['path']==y['path'] and x['memory.max']==y['memory.max']
        if x['memory.events'] is None:continue
        a,b=[dict(line.split() for line in z['memory.events'].splitlines()) for z in [x,y]]
        r['oom_deltas'].append({'path':x['path'],**{k:int(b[k])-int(a[k]) for k in ['oom','oom_kill','oom_group_kill']}})
    r['compiled_inputs_preserved']=all(H(path)==v['sha256'] for p in r['profiles'] for path,v in p['compiled_inputs']['files'].items())
    if r['remaining_owned_processes'] or not r['compiled_inputs_preserved'] or any(any(v[k] for k in ['oom','oom_kill','oom_group_kill']) for v in r['oom_deltas']):r['status']='FAIL'
    (W/'report.json').write_text(json.dumps(r,indent=2)+'\n')
    print(json.dumps({'status':r['status'],'phase':phase,'profiles':len(r['profiles']),'cases':sum(v.get('case_count',0) for v in r['profiles']),'report':str(W/'report.json'),'sha256':H(W/'report.json'),'error':r.get('error'),'oom_deltas':r['oom_deltas']}));raise SystemExit(0 if r['status']=='PASS' else 1)
