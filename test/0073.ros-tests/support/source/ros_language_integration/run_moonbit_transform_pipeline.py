#!/usr/bin/env python3
"""Finite native ROS1/ROS2 to MoonBit to native Transform fixture for explicitly pinned products.

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
    inputs=json.loads(input_path.read_bytes());attempt=inputs.get('attempt','r1');assert re.fullmatch(r'r[1-9][0-9]*',attempt);source=root/inputs.get('source_directory','source-r1');product=json.loads((root/inputs['producer_result']).read_bytes())
    assert product['status']=='EIGHT_ACTUAL_MOONBIT_TRANSFORM_PIPELINE_WASMS_AND_TWO_NATIVE_TF_PEERS_BUILT'
    helper=root/inputs['owned_helper'];assert digest(helper)==inputs['owned_helper_sha256'];owned=module('image_owned',helper)
    policy_path=(root/inputs.get('policy_path','source-r1/sdk/origin/tools/tf_policy.py')).resolve(strict=True);assert policy_path.is_relative_to(root)
    policy=module('transform_policy',policy_path);group=Path('/sys/fs/cgroup'+inputs['cgroup']);actors=[];report={'status':'RUNNING','cases':[],'negative_cases':[],'commands':[],'events':[],'inputs':inputs,'performance_credit':False};result=root/('native-result-'+attempt+'.json')
    os.sched_setaffinity(0,{15});resource.setrlimit(resource.RLIMIT_AS,(512<<20,128<<30));resource.setrlimit(resource.RLIMIT_CORE,(0,0));ctypes.CDLL(None).prctl(36,1,0,0,0)
    events=(group/'memory.events').read_text();boot='51c27aa1-d709-4a06-ba16-2ac3003fbcbd';scratch=Path('/tmp')/('ut16-'+str(os.getpid()));scratch.mkdir(mode=0o700);(root/('native-'+attempt)).mkdir();(root/('home-'+attempt)).mkdir();held=root/('held-exec-'+attempt+'.py');held.write_text('import os,sys\nfd=int(sys.argv[1])\nif os.read(fd,1)!=b"E":raise SystemExit(125)\nos.close(fd)\nos.execvpe(sys.argv[2],sys.argv[2:],os.environ)\n')
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
            p=Path(f[5]);st=p.stat();assert p.is_relative_to(native['native_SDK_root'])or str(p).startswith(inputs['ROS1_extra_library_root']+'/')or str(p)in [native['peer']['path'],native['native_gateway']['path']]
            rows[str(p)]={'stat':[st.st_dev,st.st_ino,st.st_size,st.st_mtime_ns,st.st_ctime_ns]}
        return {'text':raw,'files':rows}

    def rpc(method,values):
        body=xmlrpc.client.dumps(values,methodname=method).encode();connection=http.client.HTTPConnection('127.0.0.1',inputs['master_port'],timeout=2)
        try:
            connection.request('POST','/',body,{'Content-Type':'text/xml'});response=connection.getresponse();assert response.status==200;raw=response.read((1<<20)+1);assert len(raw)<=1<<20
        finally:connection.close()
        values,_=xmlrpc.client.loads(raw);assert len(values)==1 and len(values[0])==3 and values[0][0]==1;return values[0][2]
    def graph():return rpc('getSystemState',('/moonbit_transform_a16_owner',))
    def endpoints(state,category,topic):
        rows=[x for x in state[category]if len(x)==2 and x[0]==topic];assert len(rows)<=1;return sorted(rows[0][1])if rows else[]
    def snapshot(ns,ops,retired=False):
        value=graph();peer=ns+'/moonbit_transform_peer';gateway=ns+'/moonbit_transform_gateway'
        assert len(value)==3 and all(type(x)is list and len(x)<128 for x in value)
        assert endpoints(value,0,'/tf')==([]if retired else[peer])
        assert endpoints(value,1,'/tf')==([gateway]if'lookup'in ops else[])
        assert endpoints(value,0,'/tf_static')==([gateway]if'broadcast'in ops else[])
        assert endpoints(value,1,'/tf_static')==([]if retired else[peer])
        assert endpoints(value,0,ns+'/forbidden')==[]and endpoints(value,1,ns+'/forbidden')==([]if retired else[peer])
        topics=rpc('getPublishedTopics',('/moonbit_transform_a16_owner','/'));assert type(topics)is list and len(topics)<=128
        expected={}if retired else{'/tf':'tf2_msgs/TFMessage'}
        if'broadcast'in ops:expected['/tf_static']='tf2_msgs/TFMessage'
        assert len(topics)==len(expected)and dict(topics)==expected
        if retired:assert value==[[],[],[]]
        return {'state':value,'published_topics':topics}
    def event(label,phase):report['events'].append({'label':label,'phase':phase,'monotonic_ns':time.monotonic_ns()});save()
    try:
        # Stricter startup margin in addition to unchanged per-command guards.
        import fcntl
        report['startup_admission']=[]
        good=0
        for index in range(50):
            cur,estimate=guard();sample=dict(report['last_guard']);report['startup_admission'].append(sample);save()
            good=good+1 if cur<int(62.5*(1<<30))and estimate<54<<30 else 0
            if good==5:break
            assert index!=49,'no five-sample raw62.5GiB functional startup margin; zero actors'
            time.sleep(.2)
        lock_fd=os.open('/tmp/uwvm-hll-benchmark.lock',os.O_RDWR|os.O_CLOEXEC|os.O_NOFOLLOW);st=os.fstat(lock_fd);assert(st.st_dev,st.st_ino)==(50,622438);fcntl.flock(lock_fd,fcntl.LOCK_EX|fcntl.LOCK_NB);report['serialization_lock']={'device':50,'inode':622438,'performance_credit':False}
        report['before_raw'],report['before_estimate']=guard();report['before_events']=events;report['scratch']=str(scratch)
        for pin in inputs['pins']:assert Path(pin['path']).stat().st_size==pin['bytes']and digest(pin['path'])==pin['sha256']
        allproducts=product['products'];assert len(allproducts)==8
        report['master_states']={}
        for backend in [1,2]:
            native=product['native_backends'][str(backend)];sdk=Path(native['native_SDK_root']);loader=native['native_loader']['path'];lib=native['native_library_path']
            if backend==1:
                origin=native['master'];port=inputs['master_port']
                native_env={'ROS_MASTER_URI':'http://127.0.0.1:'+str(port),'ROS_IP':'127.0.0.1','ROS_LOG_DIR':str(root/'ros-logs'),'PYTHONDONTWRITEBYTECODE':'1','PYTHONNOUSERSITE':'1','PYTHONHOME':str(sdk/'usr'),'PYTHONPATH':str(sdk/'opt/ros/noetic/lib/python3/dist-packages')+':'+str(sdk/'usr/lib/python3/dist-packages'),'LD_LIBRARY_PATH':lib}
                with socket.socket()as reserve:reserve.bind(('127.0.0.1',port))
                master=launch('isolated-native-master',[loader,'--inhibit-cache','--library-path',lib,origin['python']['path'],'-B',origin['master']['path'],'--core','-p',str(port),'-w','2','-t','2'],native_env,deadline=600)
                until=time.monotonic()+15
                while True:
                    observe()
                    try:
                        initial=graph();assert initial==[[],[],[]];assert rpc('getPid',('/moonbit_transform_a16_owner',))==master['p'].pid;break
                    except(ConnectionError,OSError,http.client.HTTPException):assert time.monotonic()<until and master['p'].poll()is None;time.sleep(.05)
                report['master_states']['initial']=initial;report['master_pid_checked']=master['p'].pid
            else:
                native_env={'ROS_DOMAIN_ID':str(inputs['domain_id']),'RMW_IMPLEMENTATION':'rmw_fastrtps_cpp','ROS_LOCALHOST_ONLY':'1','ROS_SECURITY_ENABLE':'false','ROS_LOG_DIR':str(root/'ros-logs'),'LD_LIBRARY_PATH':lib}
            products=[x for x in allproducts if x['backend']==backend];assert len(products)==4
            for runtime in inputs['runtimes']:
                for app in products:
                    for profile,ops in [('zero',[]),('lookup-only',['lookup']),('broadcast-only',['broadcast']),('full',['lookup','broadcast'])]:
                        has_lookup='lookup'in ops;has_broadcast='broadcast'in ops;label=f'b{backend}-c{len(report["cases"])}-'+profile;ns='/uwvm_transform_a16/'+label.replace('-','_');directory=scratch/label;directory.mkdir(mode=0o700)
                        peer_argv=[loader,'--inhibit-cache','--library-path',lib,native['peer']['path'],ns]
                        if backend==1:peer_argv.append(str(inputs['master_port']))
                        peer=launch(label+'-peer',[*peer_argv,profile],native_env,stdin=True);marker(peer,f'TF_NATIVE backend={backend} phase=READY ')
                        graphs={'before_worker':snapshot(ns,[]) }if backend==1 else{}
                        opts=argparse.Namespace(backend=backend,socket=directory/'g',gateway_uid=0,worker_uid=0,worker_pid=1,node_name='moonbit_transform_gateway',namespace=ns,domain_id=inputs['domain_id']if backend==2 else None,disabled_security_tutorial=backend==2,master_uri=native_env.get('ROS_MASTER_URI'),advertise_host='127.0.0.1'if backend==1 else None,operation=ops,zero_grants=not ops,resource='/tf',lookup_resource='/tf'if has_lookup else None,broadcast_resource='/tf_static'if has_broadcast else None,lookup_target_frame='map',lookup_source_frame='base_link',broadcast_target_frame='map',broadcast_source_frame='processed_link',translation_bound='10',quaternion_tolerance='0.000001',gateway_timeout_ms=15000)
                        plan=policy.policy(opts)
                        features=['-WFE-gc','-WFE-exceptions','-WFE-tail-call','-WFE-function-references','-WFE-reference-types','-WFE-simd','-WFE-relaxed-simd','-WFE-extended-const','-WFE-multi-memory','-WFE-memory64','-WFE-table64']
                        worker=launch(label+'-worker',[runtime['path'],'-Rint',*features,*plan['Worker_arguments'],'--wasm-set-main-module-name','moonbit_transform_pipeline','--run',app['path']],hold=True)
                        opts.worker_pid=worker['p'].pid;bound=policy.policy(opts);assert bound['Worker_arguments']==plan['Worker_arguments'];config=directory/'gateway.conf';config.write_text(bound['Gateway_config_text']);config.chmod(0o600);(directory/'policy.json').write_text(json.dumps(bound,indent=2)+'\n')
                        gateway=launch(label+'-gateway',[loader,'--inhibit-cache','--library-path',lib,native['native_gateway']['path'],config],native_env)
                        while not opts.socket.exists():observe();assert gateway['p'].poll()is None,text(gateway,'stderr')[-2000:];time.sleep(.02)
                        st=opts.socket.lstat();assert stat.S_ISSOCK(st.st_mode)and stat.S_IMODE(st.st_mode)==0o600 and st.st_uid==0
                        os.write(worker['release'],b'E');os.close(worker['release']);worker['release']=None;event(label,'worker-exec-released')
                        marker(worker,'TF_READY')
                        if has_broadcast:marker(peer,f'TF_NATIVE backend={backend} phase=OUTPUT_MATCHED ')
                        if backend==1:graphs['armed']=snapshot(ns,['broadcast']if has_broadcast else[])
                        mapping={'peer':maps(peer),'gateway':maps(gateway)}
                        credit(worker,b'S');event(label,'S-native-output-matched-lookup-permitted')
                        if has_lookup:
                            credit(worker,b'P');event(label,'P-one-timestamped-lookup-started')
                            marker(peer,f'TF_NATIVE backend={backend} phase=INPUT_MATCHED ')
                            if backend==1:graphs['input_matched']=snapshot(ns,ops)
                            credit(peer,b'I');marker(peer,f'TF_NATIVE backend={backend} phase=INPUT_SENT ');event(label,'I-native-authorized-and-forbidden-frame-batch-sent')
                            marker(worker,'TF_LOOKUP_COMPLETE');event(label,'complete-authorized-lookup-verified')
                        marker(worker,'TF_AWAIT_NATIVE_WITNESS')
                        if has_broadcast:marker(peer,f'TF_NATIVE backend={backend} phase=DELIVERED ');event(label,'native-complete-processed-transform-writer-verified')
                        credit(worker,b'A');event(label,'A-native-witness-credit')
                        finish(worker);finish(gateway)
                        if backend==1:graphs['after_guest_retirement']=snapshot(ns,[])
                        credit(peer,b'Q');event(label,'Q-worker-gateway-natural-exits');finish(peer)
                        if backend==1:graphs['after_peer_retirement']=snapshot(ns,[],True)
                        event(label,'native-two-second-quiet-retirement')
                        expected=(''if has_lookup else'TF_LOOKUP_DENIED\n')+(''if has_broadcast else'TF_BROADCAST_DENIED\n')+('TF_BROADCAST_ARMED\n'if has_broadcast else'')+'TF_READY\n'+('TF_LOOKUP_COMPLETE\n'if has_lookup else'')+('TF_BROADCAST_SUBMITTED\n'if has_broadcast else'')+'TF_AWAIT_NATIVE_WITNESS\nPASS_MOONBIT_NATIVE_TRANSFORM_PIPELINE\n'
                        assert text(worker)==expected
                        audit=[json.loads(l)for l in text(worker,'stderr').splitlines()];unresolved=[x for x in audit if x.get('event')=='unresolved-denial'];assert len(unresolved)==2+int(not has_lookup)+int(not has_broadcast)
                        assert all(x['error']==13 and x['ros']==f'ros{backend}'and x['module']=='moonbit_transform_pipeline'for x in unresolved)
                        assert all(x.get('error')in(0,13,22,110)for x in audit)
                        for code,count in [(22,int(has_broadcast)),(13,2*int(has_broadcast))]:
                            selected=[x for x in audit if x.get('event')=='deny'and x.get('error')==code];assert len(selected)==count and all(x['operation']=='tf.broadcast'and x['resource']=='/tf_static'for x in selected)
                        assert not text(peer,'stderr')and not text(gateway)
                        parsed=[]
                        for line in text(peer).splitlines():
                            fields=line.split();assert fields[0]=='TF_NATIVE';row=dict(x.split('=',1)for x in fields[1:]);assert row['backend']==str(backend)and row['namespace']==ns
                            for key in ['sent','warmup','delivered','quiet_ns']:row[key]=int(row[key])
                            assert row['sent']in(0,1)and row['warmup']in(0,1)and row['delivered']in(0,1);parsed.append(row)
                        phases=[x['phase']for x in parsed];expected_phases=['READY','PASS']+(['INPUT_MATCHED','INPUT_SENT']if has_lookup else[])+(['OUTPUT_MATCHED','DELIVERED']if has_broadcast else[])
                        assert sorted(phases)==sorted(expected_phases)and phases[0]=='READY'and phases[-1]=='PASS'
                        last=parsed[-1];assert last['sent']==int(has_lookup)and last['delivered']==int(has_broadcast)and last['quiet_ns']>=2000000000
                        if backend==1:
                            assert last['source']==ns+'/moonbit_transform_peer'and last['input_reader']==(ns+'/moonbit_transform_gateway'if has_lookup else'none')and last['output_writer']==(ns+'/moonbit_transform_gateway'if has_broadcast else'none')
                            assert int(last['connects'])==int(last['disconnects'])==int(has_lookup)
                        else:
                            assert all(re.fullmatch('[0-9a-f]{48}',x[key])for x in parsed for key in ['source','input_reader','output_writer'])
                            assert int(last['source'],16)>0 and bool(int(last['input_reader'],16))==has_lookup and bool(int(last['output_writer'],16))==has_broadcast
                            assert len({x['source']for x in parsed})==1
                        report['cases'].append({'label':label,'backend':backend,'profile':profile,'runtime':runtime,'app':app,'namespace':ns,'policy':bound,'worker_audit':audit,'native_gateway_audit':text(gateway,'stderr'),'native_protocol':parsed,'Master_graphs':graphs,'actual_executable_mappings':mapping,'complete_native_lookup':has_lookup,'complete_native_broadcast':has_broadcast,'complete_native_processing':profile=='full','ungranted_native_endpoints_and_forbidden_broadcast_absent':True,'all_actor_exits_zero':True});save()
                        print(json.dumps({'label':label,'passed':True,'backend':backend,'target':app['target'],'mode':app['mode'],'runtime':runtime['mode']}),flush=True)
            if backend==1:
                assert graph()==[[],[],[]];report['master_states']['final']=graph();report['master_shutdown_reply']=rpc('shutdown',('/moonbit_transform_a16_owner','owned finite TF scenes complete'));finish(master)
        assert len(report['cases'])==64
        verified_mapping_hashes={}
        for case in report['cases']:
            for role in case['actual_executable_mappings'].values():
                for path,x in role['files'].items():
                    st=Path(path).stat();assert[st.st_dev,st.st_ino,st.st_size,st.st_mtime_ns,st.st_ctime_ns]==x['stat'];x['sha256']=verified_mapping_hashes.setdefault(path,digest(path)) if path not in verified_mapping_hashes else verified_mapping_hashes[path]
        for pin in inputs['pins']:assert digest(pin['path'])==pin['sha256']
        report['status']='ACTUAL_MOONBIT_NATIVE_TF64_EXPLICIT_PERMISSION_SCENES_PASSED'
    except BaseException as error:report.update(status='FAILED_RETAINED',error=repr(error),traceback=traceback.format_exc())
    finally:
        for a in reversed(actors):
            try:
                if a['release']is not None:os.close(a['release']);a['release']=None
                owned.terminate_group(a['p']);a['row']['ownership']=owned.owned_process_record(a['p']);a['row']['exit']=a['p'].returncode
                if a['p'].stdin is not None and not a['p'].stdin.closed:a['p'].stdin.close()
                for name in ['stdout','stderr']:p=a['dir']/(name+'.log');a['row'][name]={'path':str(p),'bytes':p.stat().st_size,'sha256':digest(p)}
            except BaseException as error:report.setdefault('cleanup_errors',[]).append(repr(error));report['status']='FAILED_RETAINED'
        if'lock_fd'in locals():os.close(lock_fd)
        report['after_events']=(group/'memory.events').read_text();report['after_raw']=int((group/'memory.current').read_text());save();print(report['status'],flush=True)
    return 0 if report['status'].startswith('ACTUAL_')else 1


if __name__=='__main__':raise SystemExit(main())
