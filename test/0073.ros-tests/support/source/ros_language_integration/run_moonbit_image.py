#!/usr/bin/env python3
"""Finite real ROS2 Image delivery/absence fixture for explicitly pinned products.

All actors execute on SSH Linux in the original hard64GiB/swap0 cgroup. This
functional owner uses the retained cache estimate and raw63.5GiB stop margin;
these observations never establish performance eligibility.
"""
from pathlib import Path
import argparse,ctypes,hashlib,importlib.util,json,os,re,resource,stat,subprocess,time,traceback


def digest(path):
    h=hashlib.sha256()
    with Path(path).open('rb') as stream:
        before=os.fstat(stream.fileno())
        while body:=stream.read(65536):h.update(body)
        after=os.fstat(stream.fileno())
        fields=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns)
        assert fields(before)==fields(after),'input changed while hashing'
    return h.hexdigest()


def module(name,path):
    spec=importlib.util.spec_from_file_location(name,path);value=importlib.util.module_from_spec(spec);spec.loader.exec_module(value);return value


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--root',type=Path,required=True);parser.add_argument('--inputs',type=Path,default=Path('native-inputs-r1.json'));args=parser.parse_args();root=args.root.resolve(strict=True);input_path=(root/args.inputs).resolve(strict=True);assert input_path.is_relative_to(root)
    inputs=json.loads(input_path.read_bytes());attempt=inputs.get('attempt','r1');assert re.fullmatch(r'r[1-9][0-9]*',attempt);source=root/'source-r1';product=json.loads((root/inputs['producer_result']).read_bytes())
    assert product['status']=='FOUR_ACTUAL_MOONBIT_IMAGE_PRODUCTS_POLICY_REGRESSION_AND_NATIVE_PEER_BUILD_PASSED'
    helper=root/inputs['owned_helper'];assert digest(helper)==inputs['owned_helper_sha256'];owned=module('image_owned',helper)
    policy=module('image_policy',source/'sdk/origin/tools/example_policy.py');group=Path('/sys/fs/cgroup'+inputs['cgroup']);actors=[];report={'status':'RUNNING','cases':[],'zero_grant':[],'commands':[],'events':[],'inputs':inputs,'performance_credit':False};result=root/('native-result-'+attempt+'.json')
    os.sched_setaffinity(0,{15});resource.setrlimit(resource.RLIMIT_AS,(512<<20,128<<30));resource.setrlimit(resource.RLIMIT_CORE,(0,0));ctypes.CDLL(None).prctl(36,1,0,0,0)
    events=(group/'memory.events').read_text();boot='51c27aa1-d709-4a06-ba16-2ac3003fbcbd';scratch=Path('/tmp')/('ui11-'+str(os.getpid()));scratch.mkdir(mode=0o700);(root/('native-'+attempt)).mkdir();(root/('home-'+attempt)).mkdir();held=root/('held-exec-'+attempt+'.py');held.write_text('import os,sys\nfd=int(sys.argv[1])\nif os.read(fd,1)!=b"E":raise SystemExit(125)\nos.close(fd)\nos.execvpe(sys.argv[2],sys.argv[2:],os.environ)\n')
    def save():result.write_text(json.dumps(report,indent=2)+'\n')
    def guard():
        assert Path('/proc/self/cgroup').read_text().strip()=='0::'+inputs['cgroup'];assert(group.stat().st_dev,group.stat().st_ino)==(29,19075);assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==boot
        assert(group/'memory.max').read_text().strip()==str(64<<30)and(group/'memory.swap.max').read_text().strip()=='0';assert(group/'memory.events').read_text()==events,'shared memory event drift'
        cur=int((group/'memory.current').read_text());st=dict(x.split()for x in(group/'memory.stat').read_text().splitlines());credit=max(0,min(cur,int(st['inactive_file']))-int(st['file_dirty'])-int(st['file_writeback'])-int(st['unevictable']))
        assert cur<int(63.5*(1<<30))and cur-credit<56<<30,'functional running margin';report['raw_peak']=max(report.get('raw_peak',0),cur);report['functional_estimate_peak']=max(report.get('functional_estimate_peak',0),cur-credit);return cur,cur-credit
    def observe():
        guard();total=0
        for a in actors:
            row=a['row'];record=owned.remember_process(a['p'])
            for m in record['members']:
                p=Path('/proc')/str(m['identity']['pid'])
                try:
                    f=(p/'stat').read_text().rpartition(')')[2].split()
                    if int(f[19])!=m['identity']['start']:continue
                    assert(p/'cgroup').read_text().strip()=='0::'+inputs['cgroup']
                    total+=next((int(l.split()[1])*1024 for l in(p/'status').read_text().splitlines()if l.startswith('VmRSS:')),0)
                except (FileNotFoundError,ProcessLookupError):continue
            if a['p'].poll()is None:assert time.monotonic()-a['started']<45,'actor45s deadline'
            assert sum((a['dir']/(n+'.log')).stat().st_size for n in ['stdout','stderr'])<4<<20,'output bound'
        report['owned_peak_rss']=max(report.get('owned_peak_rss',0),total);assert total<768<<20,'owned aggregate RSS'
    def limits():
        os.sched_setaffinity(0,{19});resource.setrlimit(resource.RLIMIT_AS,(128<<30,128<<30));resource.setrlimit(resource.RLIMIT_CPU,(60,60));resource.setrlimit(resource.RLIMIT_CORE,(0,0));resource.setrlimit(resource.RLIMIT_FSIZE,(8<<20,8<<20))
    def launch(label,argv,extra=None,hold=False,stdin=False):
        cur,estimate=guard();assert cur<int(63.25*(1<<30))and estimate<54<<30,'functional admission'
        d=root/('native-'+attempt)/label;d.mkdir();env={'PATH':'/usr/bin:/bin','HOME':str(root/('home-'+attempt)),'LANG':'C','LC_ALL':'C','TMPDIR':str(root/'tmp')};env.update(extra or {});argv=list(map(str,argv));actual=argv;readfd=writefd=None
        if hold:
            readfd,writefd=os.pipe();actual=['/usr/bin/python3','-I','-S','-B',str(held),str(readfd),*argv]
        with(d/'stdout.log').open('xb')as out,(d/'stderr.log').open('xb')as err:
            p=subprocess.Popen(actual,stdout=out,stderr=err,stdin=subprocess.PIPE if stdin or hold else subprocess.DEVNULL,env=env,start_new_session=True,pass_fds=(readfd,)if hold else (),preexec_fn=limits)
        if readfd is not None:os.close(readfd)
        owned.remember_process(p);row={'label':label,'argv':argv,'launch_argv':actual,'env':env,'pid':p.pid,'before_raw':cur,'before_estimate':estimate,'log_directory':str(d)};a={'p':p,'dir':d,'row':row,'started':time.monotonic(),'release':writefd};actors.append(a);report['commands'].append(row);save();return a
    def text(a,n='stdout'):return(a['dir']/(n+'.log')).read_text()
    def marker(a,prefix):
        while True:
            observe()
            if any(l.startswith(prefix)for l in text(a).splitlines()):return
            assert a['p'].poll()is None,(a['row']['label'],prefix,text(a,'stderr')[-2500:]);time.sleep(.02)
    def credit(a,b):a['p'].stdin.write(b);a['p'].stdin.flush()
    def finish(a):
        while a['p'].poll()is None:observe();time.sleep(.02)
        a['row']['exit']=a['p'].wait();owned.terminate_group(a['p']);a['row']['ownership']=owned.owned_process_record(a['p']);assert a['row']['exit']==0,(a['row']['label'],text(a,'stderr')[-2500:]);assert not a['row']['ownership']['signals'];save()
    def maps(a):
        raw=(Path('/proc')/str(a['p'].pid)/'maps').read_text();rows={}
        for line in raw.splitlines():
            f=line.split(None,5)
            if len(f)!=6 or 'x'not in f[1]or not f[5].startswith('/'):continue
            p=Path(f[5]);st=p.stat();assert p.is_relative_to(inputs['SDK_root'])or str(p)in [product['native_peer']['path'],product['native_gateway']['path']]
            rows[str(p)]={'stat':[st.st_dev,st.st_ino,st.st_size,st.st_mtime_ns,st.st_ctime_ns]}
        return {'text':raw,'files':rows}
    def event(label,phase):report['events'].append({'label':label,'phase':phase,'monotonic_ns':time.monotonic_ns()});save()
    try:
        report['before_raw'],report['before_estimate']=guard();report['before_events']=events;report['scratch']=str(scratch)
        for pin in inputs['pins']:assert Path(pin['path']).stat().st_size==pin['bytes']and digest(pin['path'])==pin['sha256']
        products=product['products'];assert len(products)==4
        native_env={'AMENT_PREFIX_PATH':inputs['SDK_root']+'/opt/ros/humble','RMW_IMPLEMENTATION':'rmw_fastrtps_cpp','ROS_DOMAIN_ID':'207','ROS_SECURITY_ENABLE':'false','ROS_LOCALHOST_ONLY':'1','ROS_LOG_DIR':str(root/'ros-logs')};loader=product['native_loader']['path'];lib=product['native_library_path']
        for runtime in inputs['runtimes']:
            for app in products:
                index=len(report['cases']);base='c'+str(index);namespace='/uwvm_image_a11/c'+str(index)
                for positive in [False,True]:
                    label=base+('-positive'if positive else'-zero');ns=namespace+('/positive'if positive else'/zero');directory=scratch/label;directory.mkdir(mode=0o700)
                    peer=launch(label+'-peer',[loader,'--inhibit-cache','--library-path',lib,product['native_peer']['path'],ns,*([]if positive else['absent'])],native_env,stdin=not positive);marker(peer,'READY_NATIVE_IMAGE ')
                    opts=argparse.Namespace(backend=2,socket=directory/'g',gateway_uid=0,worker_uid=0,worker_pid=1,node_name='moonbit_image_gateway',namespace=ns,domain_id=207,disabled_security_tutorial=True,master_uri=None,advertise_host=None,operation=['publish']if positive else[],resource=ns+'/image',message_type='image',gateway_timeout_ms=15000)
                    plan=policy.policy(opts);features=['-WFE-gc','-WFE-exceptions','-WFE-tail-call','-WFE-function-references','-WFE-reference-types','-WFE-simd','-WFE-relaxed-simd','-WFE-extended-const','-WFE-multi-memory','-WFE-memory64','-WFE-table64']
                    worker=launch(label+'-worker',[runtime['path'],'-Rint',*features,*plan['Worker_arguments'],'--wasm-set-main-module-name','moonbit_image','--run',app['path']],hold=True)
                    opts.worker_pid=worker['p'].pid;bound=policy.policy(opts);assert bound['Worker_arguments']==plan['Worker_arguments'];config=directory/'gateway.conf';config.write_text(bound['Gateway_config_text']);config.chmod(0o600);(directory/'policy.json').write_text(json.dumps(bound,indent=2)+'\n')
                    gateway=launch(label+'-gateway',[loader,'--inhibit-cache','--library-path',lib,product['native_gateway']['path'],config],native_env)
                    while not opts.socket.exists():observe();assert gateway['p'].poll()is None,text(gateway,'stderr')[-2000:];time.sleep(.02)
                    st=opts.socket.lstat();assert stat.S_ISSOCK(st.st_mode)and stat.S_IMODE(st.st_mode)==0o600 and st.st_uid==0
                    os.write(worker['release'],b'E');os.close(worker['release']);worker['release']=None;event(label,'worker-exec-released')
                    if positive:
                        marker(worker,'READY_MOONBIT_NATIVE_IMAGE');marker(peer,'MATCHED_NATIVE_IMAGE ');event(label,'actual-native-graph-type-qos-gid-matched');mapping={'gateway':maps(gateway),'peer':maps(peer)}
                        credit(worker,b'P');event(label,'P-gate');marker(worker,'AWAIT_NATIVE_IMAGE_DELIVERY');marker(peer,'DELIVERED_NATIVE_IMAGE ');event(label,'native-complete-image-and-writer-checked');credit(worker,b'A');event(label,'A-gate')
                    finish(worker);finish(gateway)
                    if not positive:credit(peer,b'Q');event(label,'zero-worker-ended-Q-gate')
                    finish(peer);event(label,'native-quiet-and-endpoint-retirement-exited')
                    out=text(worker);assert out==('READY_MOONBIT_NATIVE_IMAGE\nAWAIT_NATIVE_IMAGE_DELIVERY\nPASS_MOONBIT_NATIVE_IMAGE\n'if positive else'PASS_MOONBIT_IMAGE_DEFAULT_DENIED\n')
                    audit=[json.loads(l)for l in text(worker,'stderr').splitlines()];denied=[x for x in audit if x.get('event')=='unresolved-denial'];assert len(denied)==(1 if positive else 2)and all(x['error']==13 and x['ros']=='ros2'and x['module']=='moonbit_image'for x in denied)
                    assert all(x.get('error')in(0,13,22)for x in audit);assert not text(peer,'stderr');assert not text(gateway)
                    lines=text(peer).splitlines();phases=['READY','MATCHED','DELIVERED','PASS']if positive else['READY','ABSENT'];assert len(lines)==len(phases);parsed=[]
                    for phase,line in zip(phases,lines):
                        m=re.fullmatch(phase+r'_NATIVE_IMAGE namespace=(\S+) gateway=moonbit_image_gateway gid=([0-9a-f]{48}) warmup=([01]) delivered=([01])',line);assert m and m[1]==ns;parsed.append({'phase':phase,'gid':m[2],'warmup':int(m[3]),'delivered':int(m[4])})
                    assert parsed[0]=={'phase':'READY','gid':'0'*48,'warmup':0,'delivered':0}
                    if positive:assert parsed[1]['gid']!='0'*48 and len({p['gid']for p in parsed[1:]})==1 and [p['delivered']for p in parsed]==[0,0,1,1]
                    else:assert all(p['gid']=='0'*48 and p['delivered']==p['warmup']==0 for p in parsed)
                    row={'label':label,'runtime':runtime,'app':app,'namespace':ns,'policy':bound,'native_protocol':parsed,'worker_audit':audit,'native_gateway_audit':text(gateway,'stderr'),'positive_native_Image_delivery':positive,'forbidden_native_Image_endpoint_absent':True,'quiet_min_seconds':2,'all_actor_exits_zero':True}
                    if positive:row['actual_executable_mappings']=mapping;row['expected_Image']={'seconds':1,'nanoseconds':2,'frame_id':'camera/optical','height':2,'width':2,'encoding':'mono8','bigendian':0,'step':2,'data_hex':'4c951cff'}
                    report['cases'if positive else'zero_grant'].append(row);save();print(json.dumps({'label':label,'passed':True,'target':app['target'],'profile':app['mode'],'runtime':runtime['mode']}),flush=True)
        assert len(report['cases'])==len(report['zero_grant'])==8
        for case in report['cases']:
            for role in case['actual_executable_mappings'].values():
                for p,x in role['files'].items():
                    st=Path(p).stat();assert [st.st_dev,st.st_ino,st.st_size,st.st_mtime_ns,st.st_ctime_ns]==x['stat'];x['sha256']=digest(p)
        for pin in inputs['pins']:assert digest(pin['path'])==pin['sha256']
        report['status']='ACTUAL_MOONBIT_ROS2_IMAGE_EIGHT_DELIVERIES_AND_EIGHT_ZERO_GRANT_SCENES_PASSED'
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error),traceback=traceback.format_exc())
    finally:
        for a in reversed(actors):
            try:
                if a['release']is not None:os.close(a['release']);a['release']=None
                owned.terminate_group(a['p']);a['row']['ownership']=owned.owned_process_record(a['p']);a['row']['exit']=a['p'].returncode
                if a['p'].stdin is not None and not a['p'].stdin.closed:a['p'].stdin.close()
                for n in ['stdout','stderr']:p=a['dir']/(n+'.log');a['row'][n]={'path':str(p),'bytes':p.stat().st_size,'sha256':digest(p)}
            except BaseException as error:report.setdefault('cleanup_errors',[]).append(repr(error));report['status']='FAILED_RETAINED'
        report['after_events']=(group/'memory.events').read_text();report['after_raw']=int((group/'memory.current').read_text());save();print(report['status'],flush=True)
    return 0 if report['status'].startswith('ACTUAL_')else 1


if __name__=='__main__':raise SystemExit(main())
