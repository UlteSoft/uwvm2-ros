#!/usr/bin/env python3
"""Finite native ROS1 to MoonBit to native Image processing fixture for explicitly pinned products.

All actors execute on SSH Linux in the original hard64GiB/swap0 cgroup. This
functional owner uses the retained cache estimate and raw63.5GiB stop margin;
these observations never establish performance eligibility.
"""
from pathlib import Path
import http.client,socket,xmlrpc.client
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
    assert product['status']=='FOUR_ACTUAL_MOONBIT_ROS1_PIPELINE_PRODUCTS_AND_NATIVE_PEER_BUILD_PASSED'
    helper=root/inputs['owned_helper'];assert digest(helper)==inputs['owned_helper_sha256'];owned=module('image_owned',helper)
    policy=module('image_policy',source/'sdk/origin/tools/example_policy.py');group=Path('/sys/fs/cgroup'+inputs['cgroup']);actors=[];report={'status':'RUNNING','cases':[],'negative_cases':[],'commands':[],'events':[],'inputs':inputs,'performance_credit':False};result=root/('native-result-'+attempt+'.json')
    os.sched_setaffinity(0,{15});resource.setrlimit(resource.RLIMIT_AS,(512<<20,128<<30));resource.setrlimit(resource.RLIMIT_CORE,(0,0));ctypes.CDLL(None).prctl(36,1,0,0,0)
    events=(group/'memory.events').read_text();boot='51c27aa1-d709-4a06-ba16-2ac3003fbcbd';scratch=Path('/tmp')/('ui14-'+str(os.getpid()));scratch.mkdir(mode=0o700);(root/('native-'+attempt)).mkdir();(root/('home-'+attempt)).mkdir();held=root/('held-exec-'+attempt+'.py');held.write_text('import os,sys\nfd=int(sys.argv[1])\nif os.read(fd,1)!=b"E":raise SystemExit(125)\nos.close(fd)\nos.execvpe(sys.argv[2],sys.argv[2:],os.environ)\n')
    def save():result.write_text(json.dumps(report,indent=2)+'\n')
    def guard():
        assert Path('/proc/self/cgroup').read_text().strip()=='0::'+inputs['cgroup'];assert(group.stat().st_dev,group.stat().st_ino)==(29,19075);assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==boot
        assert(group/'memory.max').read_text().strip()==str(64<<30)and(group/'memory.swap.max').read_text().strip()=='0';assert(group/'memory.events').read_text()==events,'shared memory event drift'
        cur=int((group/'memory.current').read_text());st=dict(x.split()for x in(group/'memory.stat').read_text().splitlines());credit=max(0,min(cur,int(st['inactive_file']))-int(st['file_dirty'])-int(st['file_writeback'])-int(st['unevictable']))
        report['last_guard']={'memory_current':cur,'recoverable_file_estimate':credit,'functional_estimate':cur-credit,'monotonic_ns':time.monotonic_ns()}
        report['raw_peak']=max(report.get('raw_peak',0),cur);report['functional_estimate_peak']=max(report.get('functional_estimate_peak',0),cur-credit)
        if cur>=int(63.5*(1<<30))or cur-credit>=56<<30:
            report['guard_rejection']=dict(report['last_guard']);save();raise AssertionError('functional running margin')
        return cur,cur-credit
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
            if a['p'].poll()is None:assert time.monotonic()-a['started']<a['deadline'],'actor deadline'
            assert sum((a['dir']/(n+'.log')).stat().st_size for n in ['stdout','stderr'])<4<<20,'output bound'
        report['owned_peak_rss']=max(report.get('owned_peak_rss',0),total);assert total<768<<20,'owned aggregate RSS'
    def limits():
        os.sched_setaffinity(0,{19});resource.setrlimit(resource.RLIMIT_AS,(128<<30,128<<30));resource.setrlimit(resource.RLIMIT_CPU,(60,60));resource.setrlimit(resource.RLIMIT_CORE,(0,0));resource.setrlimit(resource.RLIMIT_FSIZE,(8<<20,8<<20))
    def launch(label,argv,extra=None,hold=False,stdin=False,deadline=50):
        cur,estimate=guard();assert cur<int(63.25*(1<<30))and estimate<54<<30,'functional admission'
        d=root/('native-'+attempt)/label;d.mkdir();env={'PATH':'/usr/bin:/bin','HOME':str(root/('home-'+attempt)),'LANG':'C','LC_ALL':'C','TMPDIR':str(root/'tmp')};env.update(extra or {});argv=list(map(str,argv));actual=argv;readfd=writefd=None
        if hold:
            readfd,writefd=os.pipe();actual=['/usr/bin/python3','-I','-S','-B',str(held),str(readfd),*argv]
        with(d/'stdout.log').open('xb')as out,(d/'stderr.log').open('xb')as err:
            p=subprocess.Popen(actual,stdout=out,stderr=err,stdin=subprocess.PIPE if stdin or hold else subprocess.DEVNULL,env=env,start_new_session=True,pass_fds=(readfd,)if hold else (),preexec_fn=limits)
        if readfd is not None:os.close(readfd)
        owned.remember_process(p);row={'label':label,'argv':argv,'launch_argv':actual,'env':env,'pid':p.pid,'before_raw':cur,'before_estimate':estimate,'log_directory':str(d),'deadline_seconds':deadline};a={'p':p,'dir':d,'row':row,'started':time.monotonic(),'release':writefd,'deadline':deadline};actors.append(a);report['commands'].append(row);save();return a
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
            p=Path(f[5]);st=p.stat();assert p.is_relative_to(inputs['SDK_root'])or p.is_relative_to(inputs['gateway_library_root'])or str(p)in [product['native_peer']['path'],product['native_gateway']['path']]
            rows[str(p)]={'stat':[st.st_dev,st.st_ino,st.st_size,st.st_mtime_ns,st.st_ctime_ns]}
        return {'text':raw,'files':rows}
    def rpc(method,values):
        body=xmlrpc.client.dumps(values,methodname=method).encode();connection=http.client.HTTPConnection('127.0.0.1',inputs['master_port'],timeout=2)
        try:
            connection.request('POST','/',body,{'Content-Type':'text/xml'});response=connection.getresponse();assert response.status==200;raw=response.read((1<<20)+1);assert len(raw)<=1<<20
        finally:connection.close()
        values,_=xmlrpc.client.loads(raw);assert len(values)==1 and len(values[0])==3 and values[0][0]==1;return values[0][2]
    def graph():
        value=rpc('getSystemState',('/moonbit_image_a14_owner',));assert len(value)==3 and all(type(x)is list and len(x)<128 for x in value);return value
    def endpoints(state,category,topic):
        rows=[x for x in state[category]if len(x)==2 and x[0]==topic];assert len(rows)<=1;return sorted(rows[0][1])if rows else[]
    def snapshot(ns,stage):
        value=graph();active=stage!='retired';peer=ns+'/moonbit_image_peer';gateway=ns+'/moonbit_image_gateway'
        assert endpoints(value,0,ns+'/input')==([peer]if active else[])
        assert endpoints(value,1,ns+'/input')==([gateway]if stage in ['input','full']else[])
        assert endpoints(value,0,ns+'/output')==([gateway]if stage=='full'else[])
        assert endpoints(value,1,ns+'/output')==([peer]if active else[])
        assert endpoints(value,0,ns+'/forbidden')==[]and endpoints(value,1,ns+'/forbidden')==([peer]if active else[])
        topics=rpc('getPublishedTopics',('/moonbit_image_a14_owner',ns));assert type(topics)is list and len(topics)<=128
        expected={ns+'/input':'sensor_msgs/Image'}if active else{}
        if stage=='full':expected[ns+'/output']='sensor_msgs/Image'
        assert len(topics)==len(expected)and dict(topics)==expected
        if not active:assert value==[[],[],[]]
        return {'state':value,'published_topics':topics}
    def event(label,phase):report['events'].append({'label':label,'phase':phase,'monotonic_ns':time.monotonic_ns()});save()
    try:
        report['before_raw'],report['before_estimate']=guard();report['before_events']=events;report['scratch']=str(scratch)
        for pin in inputs['pins']:assert Path(pin['path']).stat().st_size==pin['bytes']and digest(pin['path'])==pin['sha256']
        products=product['products'];assert len(products)==4
        origin=product['native_origin'];sdk=Path(inputs['SDK_root']);loader=product['native_loader']['path'];lib=product['native_library_path'];port=inputs['master_port']
        native_env={'ROS_MASTER_URI':'http://127.0.0.1:'+str(port),'ROS_IP':'127.0.0.1','ROS_LOG_DIR':str(root/'ros-logs'),'PYTHONDONTWRITEBYTECODE':'1','PYTHONNOUSERSITE':'1','PYTHONHOME':str(sdk/'usr'),'PYTHONPATH':str(sdk/'opt/ros/noetic/lib/python3/dist-packages')+':'+str(sdk/'usr/lib/python3/dist-packages'),'LD_LIBRARY_PATH':lib}
        with socket.socket()as reserve:reserve.bind(('127.0.0.1',port))
        master=launch('isolated-native-master',[loader,'--inhibit-cache','--library-path',lib,origin['python']['path'],'-B',origin['master']['path'],'--core','-p',str(port),'-w','2','-t','2'],native_env,deadline=300)
        until=time.monotonic()+15
        while True:
            observe()
            try:
                initial=graph();assert initial==[[],[],[]];assert rpc('getPid',('/moonbit_image_a14_owner',))==master['p'].pid;break
            except (ConnectionError,OSError,http.client.HTTPException):assert time.monotonic()<until and master['p'].poll()is None;time.sleep(.05)
        report['initial_master_graph']=initial;report['master_pid_checked']=master['p'].pid
        for runtime in inputs['runtimes']:
            for app in products:
                index=len(report['cases']);base='c'+str(index)
                for profile,ops in [('zero',[]),('publish-only',['publish']),('subscribe-only',['subscribe']),('full',['publish','subscribe'])]:
                    positive=profile=='full';has_sub='subscribe'in ops;label=base+'-'+profile;ns='/uwvm_image_pipe_a14/'+base+'/'+profile.replace('-','_');directory=scratch/label;directory.mkdir(mode=0o700)
                    peer=launch(label+'-peer',[loader,'--inhibit-cache','--library-path',lib,product['native_peer']['path'],ns,str(port),profile],native_env,stdin=True);marker(peer,'READY_ROS1_IMAGE_PIPELINE ');graphs={'before_worker':snapshot(ns,'initial')}
                    opts=argparse.Namespace(backend=1,socket=directory/'g',gateway_uid=0,worker_uid=0,worker_pid=1,node_name='moonbit_image_gateway',namespace=ns,domain_id=None,disabled_security_tutorial=False,master_uri=native_env['ROS_MASTER_URI'],advertise_host='127.0.0.1',operation=ops,resource=ns+'/output',subscribe_resource=ns+'/input'if has_sub else None,message_type='image',gateway_timeout_ms=15000)
                    plan=policy.policy(opts);features=['-WFE-gc','-WFE-exceptions','-WFE-tail-call','-WFE-function-references','-WFE-reference-types','-WFE-simd','-WFE-relaxed-simd','-WFE-extended-const','-WFE-multi-memory','-WFE-memory64','-WFE-table64']
                    worker=launch(label+'-worker',[runtime['path'],'-Rint',*features,*plan['Worker_arguments'],'--wasm-set-main-module-name','moonbit_image_pipeline','--run',app['path']],hold=True)
                    opts.worker_pid=worker['p'].pid;bound=policy.policy(opts);assert bound['Worker_arguments']==plan['Worker_arguments'];config=directory/'gateway.conf';config.write_text(bound['Gateway_config_text']);config.chmod(0o600);(directory/'policy.json').write_text(json.dumps(bound,indent=2)+'\n')
                    gateway=launch(label+'-gateway',[loader,'--inhibit-cache','--library-path',lib,product['native_gateway']['path'],config],native_env)
                    while not opts.socket.exists():observe();assert gateway['p'].poll()is None,text(gateway,'stderr')[-2000:];time.sleep(.02)
                    st=opts.socket.lstat();assert stat.S_ISSOCK(st.st_mode)and stat.S_IMODE(st.st_mode)==0o600 and st.st_uid==0
                    os.write(worker['release'],b'E');os.close(worker['release']);worker['release']=None;event(label,'worker-exec-released')
                    if has_sub:
                        marker(worker,'INPUT_OPEN_IMAGE_PIPELINE');marker(peer,'INPUT_MATCHED_ROS1_IMAGE_PIPELINE ');graphs['input_matched']=snapshot(ns,'input');event(label,'native-input-reader-identity-TCPROS-Master-matched');credit(worker,b'S');event(label,'S-setup-gate')
                    pulses=0
                    if positive:
                        marker(worker,'READY_RECEIVE_NATIVE_IMAGE');marker(peer,'OUTPUT_MATCHED_ROS1_IMAGE_PIPELINE ');graphs['both_matched']=snapshot(ns,'full');event(label,'native-output-writer-identity-TCPROS-Master-matched');mapping={'gateway':maps(gateway),'peer':maps(peer)}
                        credit(worker,b'P');pulses=1;event(label,'P-empty-before-native-send');marker(worker,'IMAGE_PIPELINE_EMPTY');event(label,'actual-empty-poll11-observed')
                        credit(peer,b'I');event(label,'I-native-input-gate');marker(peer,'INPUT_SENT_ROS1_IMAGE_PIPELINE ');event(label,'native-RGB-publication-completed')
                        for pulse in range(1,32):
                            old_empty=text(worker).count('IMAGE_PIPELINE_EMPTY\n');credit(worker,b'P');pulses+=1;event(label,'P-input-poll-'+str(pulse))
                            while True:
                                observe();out=text(worker)
                                if 'AWAIT_PIPELINE_NATIVE_DELIVERY\n'in out:break
                                if out.count('IMAGE_PIPELINE_EMPTY\n')>old_empty:break
                                assert worker['p'].poll()is None,text(worker,'stderr')[-2000:];time.sleep(.01)
                            if 'AWAIT_PIPELINE_NATIVE_DELIVERY\n'in text(worker):break
                            time.sleep(.02)
                        marker(worker,'AWAIT_PIPELINE_NATIVE_DELIVERY');marker(peer,'DELIVERED_ROS1_IMAGE_PIPELINE ');event(label,'native-complete-processed-image-and-writer-verified');credit(worker,b'A');event(label,'A-delivery-gate')
                    finish(worker);finish(gateway);graphs['after_guest_retirement']=snapshot(ns,'initial')
                    if not positive:credit(peer,b'Q');event(label,'Q-after-worker-gateway-natural-exits')
                    finish(peer);graphs['after_peer_retirement']=snapshot(ns,'retired');event(label,'native-endpoints-quiet-and-owner-retired')
                    out=text(worker);empty=out.count('IMAGE_PIPELINE_EMPTY\n')
                    expected=('INPUT_OPEN_IMAGE_PIPELINE\nREADY_RECEIVE_NATIVE_IMAGE\n'+'IMAGE_PIPELINE_EMPTY\n'*empty+'AWAIT_PIPELINE_NATIVE_DELIVERY\nPASS_MOONBIT_NATIVE_IMAGE_PIPELINE\n'if positive else 'INPUT_OPEN_IMAGE_PIPELINE\nPASS_PIPELINE_PUB_DENIED\n'if has_sub else'PASS_PIPELINE_SUB_DENIED\n')
                    assert out==expected and (1<=empty<32 and pulses==empty+1 if positive else empty==pulses==0)
                    audit=[json.loads(l)for l in text(worker,'stderr').splitlines()];denied=[x for x in audit if x.get('event')=='unresolved-denial'];assert len(denied)==(2 if positive else 3)and all(x['error']==13 and x['ros']=='ros1'and x['module']=='moonbit_image_pipeline'for x in denied)
                    assert all(x.get('error')in(0,11,13,22)for x in audit);assert not text(peer,'stderr')and not text(gateway)
                    lines=text(peer).splitlines();phases=['READY','INPUT_MATCHED','OUTPUT_MATCHED','INPUT_SENT','DELIVERED','PASS']if positive else['READY','INPUT_MATCHED','ABSENT']if has_sub else['READY','ABSENT'];assert len(lines)==len(phases);parsed=[]
                    for phase,line in zip(phases,lines):
                        m=re.fullmatch(phase+r'_ROS1_IMAGE_PIPELINE namespace='+re.escape(ns)+r' source='+re.escape(ns)+r'/moonbit_image_peer input_reader=(none|'+re.escape(ns)+r'/moonbit_image_gateway) output_writer=(none|'+re.escape(ns)+r'/moonbit_image_gateway) transport=tcpros type=sensor_msgs/Image md5=060021388200f6f0f447d0fcd9c64743 latch=0 input_subscribers=([01]) output_publishers=([01]) connects=([01]) disconnects=([01]) sent=([01]) warmup=([01]) delivered=([01])',line);assert m;parsed.append(dict(phase=phase,input_reader=m[1],output_writer=m[2],input_subscribers=int(m[3]),output_publishers=int(m[4]),connects=int(m[5]),disconnects=int(m[6]),sent=int(m[7]),warmup=int(m[8]),delivered=int(m[9])))
                    assert parsed[0]==dict(phase='READY',input_reader='none',output_writer='none',input_subscribers=0,output_publishers=0,connects=0,disconnects=0,sent=0,warmup=0,delivered=0)
                    if has_sub:assert all(p['input_reader']==ns+'/moonbit_image_gateway'and p['connects']==1 for p in parsed[1:])and parsed[1]['input_subscribers']==1 and parsed[-1]['disconnects']==1
                    else:assert all(p['input_reader']=='none'and p['input_subscribers']==p['connects']==p['disconnects']==0 for p in parsed)
                    if positive:assert all(p['output_writer']==ns+'/moonbit_image_gateway'for p in parsed[2:])and [p['sent']for p in parsed]==[0,0,0,1,1,1]and [p['delivered']for p in parsed]==[0,0,0,0,1,1]and parsed[4]['input_subscribers']==parsed[4]['output_publishers']==1 and parsed[-1]['input_subscribers']==parsed[-1]['output_publishers']==0
                    else:assert all(p['output_writer']=='none'and p['output_publishers']==p['sent']==p['warmup']==p['delivered']==0 for p in parsed)
                    row={'label':label,'profile':profile,'runtime':runtime,'app':app,'namespace':ns,'policy':bound,'native_protocol':parsed,'worker_audit':audit,'native_gateway_audit':text(gateway,'stderr'),'complete_native_input_processing_output':positive,'ungranted_native_endpoints_absent':True,'quiet_min_seconds':2,'all_actor_exits_zero':True,'P_poll_credits':pulses,'empty_polls':empty,'Master_graphs':graphs}
                    if positive:row['actual_executable_mappings']=mapping;row['expected_input']={'sequence':0,'seconds':1,'nanoseconds':2,'frame_id':'camera/optical','height':2,'width':2,'encoding':'rgb8','bigendian':0,'step':8,'data_hex':'ff000000ff00aabb0000ffffffffccdd'};row['expected_output']=dict(row['expected_input'],sequence=1,encoding='mono8',step=2,data_hex='4c951cff')
                    report['cases'if positive else'negative_cases'].append(row);save();print(json.dumps({'label':label,'passed':True,'target':app['target'],'Guest_profile':app['mode'],'runtime':runtime['mode'],'P_credits':pulses}),flush=True)
        assert len(report['cases'])==8 and len(report['negative_cases'])==24
        assert graph()==[[],[],[]];report['final_master_graph']=graph();event('master','administrative-XMLRPC-shutdown');report['master_shutdown_reply']=rpc('shutdown',('/moonbit_image_a14_owner','owned finite scenes complete'));finish(master)
        for case in report['cases']:
            for role in case['actual_executable_mappings'].values():
                for p,x in role['files'].items():
                    st=Path(p).stat();assert [st.st_dev,st.st_ino,st.st_size,st.st_mtime_ns,st.st_ctime_ns]==x['stat'];x['sha256']=digest(p)
        for pin in inputs['pins']:assert digest(pin['path'])==pin['sha256']
        report['status']='ACTUAL_MOONBIT_NATIVE_ROS1_IMAGE_PIPELINE_EIGHT_PROCESSING_AND24_PERMISSION_SCENES_PASSED'
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
