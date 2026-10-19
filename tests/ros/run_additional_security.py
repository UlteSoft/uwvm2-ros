#!/usr/bin/env python3
"""Malicious operation-substitution guests with real native read/graph/cancel counters."""
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import signal
import subprocess
import threading
import time
from socketserver import ThreadingMixIn
from xmlrpc.server import SimpleXMLRPCServer
import xmlrpc.client
import run_network as network
import run_rpc

class CountingMaster(ThreadingMixIn,SimpleXMLRPCServer):
    daemon_threads=True
    block_on_close=False
    def __init__(self,upstream,resource):
        super().__init__(('127.0.0.1',0),allow_none=True,logRequests=False)
        self.upstream=upstream;self.resource=resource;self.reads=0;self.lock=threading.Lock()
    def _dispatch(self,method,params):
        if method=='getParam' and len(params)>=2 and params[1]==self.resource:
            with self.lock:self.reads+=1
        with xmlrpc.client.ServerProxy(self.upstream,allow_none=True) as upstream:
            return getattr(upstream,method)(*params)

class AdditionalRunner(network.Runner):
    def global_group_no_inheritance(self):
        connection,grants=network.policy(1,['publish'])
        connection={**connection,'node-name':'uwvm_ros1_global_noinherit_witness'}
        # Keep the actual Unix socket path below sockaddr_un.sun_path's107-byte ceiling.
        cap=grants[0];name='global-group';directory=self.run/name
        original_start=self.start;auxiliary=[]
        def start_with_global(command,label,env=None):
            if label!=name+'-uwvm':return original_start(command,label,env)
            prefix='--ros1-global-';options=[prefix+'enable']
            fields={**connection,'gateway-socket':str(directory/'global.sock'),'gateway-uid':str(os.getuid()),'connect-timeout-ms':'1000','audit-deny-max-rate-hz':'100','audit-deny-burst':'100','audit-deny-max-records':'10000','audit-deny-max-bytes':'1048576'}
            for key,value in fields.items():options += [prefix+key,value]
            options += [prefix+'grant',*[cap[key] for key in ('alias','operation','resource','type','identity')]]
            for key,value in cap['limits'].items():options += [prefix+'set',cap['alias'],key,value]
            command=[*command[:5],*options,*command[5:]]
            (directory/'uwvm-argv.json').write_text(json.dumps(command[4:],indent=2)+'\n')
            worker=original_start(command,label,env)
            try:
                lines=['backend ros1',f'worker-uid {os.getuid()}',f'worker-pid {worker.pid}',f'socket /uwvm-ros-test/{name}/global.sock','timeout-ms 10000','max-sessions 16','audit-max-records-per-second 100','audit-max-records 10000','audit-max-bytes 1048576']
                lines += [f'connection {key} {value}' for key,value in connection.items()]
                lines.append('grant '+' '.join(cap[key] for key in ('alias','operation','resource','type','identity')))
                lines += [f"set {cap['alias']} {key} {value}" for key,value in cap['limits'].items()]
                config=directory/'global.txt';config.write_text('\n'.join(lines)+'\n');os.chmod(config,0o600)
                gateway=original_start(self.native_command(1,['/tmp/ros1-build/uwvm-ros1-gateway',f'/uwvm-ros-test/{name}/global.txt']),name+'-global-gateway');auxiliary.append(gateway)
                for _ in range(200):
                    if gateway.poll() is not None:raise RuntimeError('FAIL enabled Global Gateway startup')
                    if (directory/'global.sock').exists():break
                    time.sleep(.01)
                else:raise RuntimeError('FAIL enabled Global Gateway readiness deadline')
                return worker
            except Exception:
                self.stop(worker)
                for gateway in auxiliary:self.stop(gateway)
                raise
        try:
            self.start=start_with_global
            self.case(name,'examples/ros_dual/wasi_group_confusion.wasm',{1:[]},[(1,'--absent','/uwvm_ros_test/output')],group='sensors',preload='examples/ros_dual/group_dummy.wasm',extra=('--ros1-group-add-module','sensors','main','--wasip1-global-noinherit-system-environment'))
            self.results[-1].update(name='global-group-no-inheritance',expected_error=13,global_enabled=True,global_grant_exists=True,global_gateway_bound=True,selected_group_enabled=True,selected_group_grants=0,ros_output_handle_unchanged=True)
        finally:
            self.start=original_start
            for gateway in auxiliary:self.stop(gateway)
    def group_confusion(self):
        self.global_group_no_inheritance()
        self.case('backend-group-confusion','examples/ros_dual/backend_group_confusion.wasm',{1:['publish'],2:[]},[(1,'--absent','/uwvm_ros_test/output'),(2,'--absent','/uwvm_ros_test/output')],group='same',preload='examples/ros_dual/group_dummy.wasm',extra=('--ros1-group-add-module','same','main','--ros2-group-add-module','same','main','--wasip1-global-noinherit-system-environment'))
        self.results[-1].update(expected_error=13,ros1_open_succeeded=True,ros2_output_handle_unchanged=True,same_group_name='same')
        self.case('wasi-group-confusion','examples/ros_dual/wasi_group_confusion.wasm',{1:[]},[(1,'--absent','/uwvm_ros_test/output')],group='same',preload='examples/ros_dual/group_dummy.wasm',extra=('--ros1-group-add-module','same','main','--wasip1-group-create','same','--wasip1-group-add-module','same','main','--wasip1-group-enable','same','--wasip1-group-noinherit-system-environment','same','--wasip1-global-noinherit-system-environment'))
        self.results[-1].update(expected_error=13,wasi_proc_exit_succeeded=True,ros_output_handle_unchanged=True,same_group_name='same')
    def startup_family(self,version):
        connection,grants=network.policy(version,['publish'])
        original=grants[0]
        tests=[]
        for name,resource in [('normalized-resource','/uwvm_ros_test/output/../forbidden'),('relative-resource','uwvm_ros_test/output'),('private-resource','~output'),('remapping-resource','/uwvm_ros_test/output:=/forbidden'),('overlong-resource','/'+('a'*1024))]:
            cap=copy.deepcopy(original);cap['resource']=resource
            tests.append((name,cap,{},[], 'invalid target, alias, resource or operation'))
        tests.append(('namespace-bypass',original,{'namespace':'/uwvm_ros_test/../forbidden'},[],'namespace must be canonical'))
        cap=copy.deepcopy(original);cap['identity']='0'*32 if version==1 else 'RIHS01_'+'0'*64
        tests.append(('wrong-md5' if version==1 else 'wrong-typesupport',cap,{},[],'type identity differs from trusted adapter'))
        cap=copy.deepcopy(original);cap['type']='std_msgs/Untrusted' if version==1 else 'std_msgs/msg/Untrusted'
        tests.append(('wrong-ros-type',cap,{},[],'operation/type has no implemented trusted backend adapter'))
        cap=copy.deepcopy(original);cap['type']='std_msgs/msg/String' if version==1 else 'std_msgs/String'
        tests.append(('cross-version-type',cap,{},[],'operation/type has no implemented trusted backend adapter'))
        prefix=f'--ros{version}-'
        tests.append(('forged-module-name',original,{},[prefix+'group-create','sensors',prefix+'group-add-module','sensors','forged'],'absent module'))
        tests.append(('multiple-module-groups',original,{},[prefix+'group-create','first',prefix+'group-add-module','first','main',prefix+'group-create','second',prefix+'group-add-module','second','main'],'already explicitly bound'))
        probe=[f'/tmp/ros{version}-build/uwvm-ros{version}-native-probe','--absent',original['resource'],'hello','String']
        native=self.start(self.native_command(version,probe),f'ros{version}-startup-absence')
        rows=[]
        try:
            time.sleep(.3)
            for name,cap,changes,extra,diagnostic in tests:
                if native.poll() is not None:raise RuntimeError('FAIL startup absence witness ended before all attacks')
                actual={**connection,**changes,'gateway-socket':str(self.run/f'never-created-ros{version}.sock'),'gateway-uid':str(os.getuid()),'connect-timeout-ms':'1000','audit-deny-max-rate-hz':'100','audit-deny-burst':'100','audit-deny-max-records':'10000','audit-deny-max-bytes':'1048576'}
                cli=[prefix+'global-enable']
                for key,value in actual.items():cli += [prefix+'global-'+key,value]
                cli += [prefix+'global-grant',*[cap[key] for key in ('alias','operation','resource','type','identity')]]
                for key,value in cap['limits'].items():cli += [prefix+'global-set',cap['alias'],key,value]
                guest=self.source/'examples/ros_dual/startup_sentinel.wasm'
                command=[self.product,*cli,*extra,'--wasip1-global-noinherit-system-environment','--wasm-set-main-module-name','main','--run',str(guest)]
                result=subprocess.run(command,env=self.env,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=8)
                text=(result.stdout+result.stderr).decode(errors='replace')
                (self.run/f'ros{version}-startup-{name}.log').write_text(text)
                (self.run/f'ros{version}-startup-{name}-argv.json').write_text(json.dumps(command,indent=2)+'\n')
                if result.returncode!=126 or diagnostic not in text:
                    raise RuntimeError(f'FAIL startup {name} expected126/intended diagnostic, got{result.returncode}: {text[:500]}')
                rows.append({'name':f'ros{version}-startup-'+name,'status':'PASS','expected_exit':126,'wasm_exit':result.returncode,'guest_start_sentinel':201,'guest_start_entered':False,'intended_diagnostic':diagnostic,'native_witness':probe,'guest_sha256':hashlib.sha256(guest.read_bytes()).hexdigest(),'product_sha256':self.artifacts['uwvm']['sha256']})
            if native.wait(timeout=8):raise RuntimeError('FAIL startup attacks had actual unauthorized native network side effects')
            self.results.extend(rows)
            for row in rows:print('PASS startup rejection plus actual native absence '+row['name'],flush=True)
        finally:self.stop(native)
    def operation(self,version,name,grant,witness,ready=True,connection_changes=None,proxy=None):
        native=self.start(self.native_command(version,witness),name+'-native-witness')
        original=network.policy
        try:
            log=self.run/(name+'-native-witness.log')
            if ready:
                for _ in range(250):
                    if native.poll() is not None:raise RuntimeError('FAIL native witness exited before malicious Guest '+name)
                    if b'READY' in log.read_bytes():break
                    time.sleep(.02)
                else:raise RuntimeError('FAIL native readiness deadline '+name)
            else:
                time.sleep(.7)
                if native.poll() is not None:raise RuntimeError('FAIL native absence witness exited before Guest '+name)
            def exact(v,role,message_type):
                connection,_=original(v,[])
                connection.update(connection_changes or {})
                return connection,[grant]
            network.policy=exact
            self.case(name,f'examples/ros{version}/'+name.removeprefix(f'ros{version}-')+'.wasm',{version:[]},[])
            record=self.results.pop()
            if proxy:
                time.sleep(.2)
                if proxy.reads:raise RuntimeError('FAIL denied ROS1 parameter read reached real Master proxy')
                record['native_counters']={'getParam_requests_for_exact_resource':proxy.reads}
                record['master_proxy_upstream']=proxy.upstream
            else:
                if native.wait(timeout=12):raise RuntimeError('FAIL native zero-side-effect counter '+name)
            record.update(native_witness=witness,native_log=str(log),expected_error=13,output_sentinels_unchanged=True)
            self.results.append(record)
            print('PASS exact denial and independent native counter '+name,flush=True)
        finally:
            network.policy=original
            self.stop(native)
    def families(self,version):
        original=network.policy
        _,grants=original(version,['publish'])
        self.operation(version,f'ros{version}-topic_subscribe_denied',grants[0],[f'/tmp/ros{version}-build/uwvm-ros{version}-native-probe','--absent-subscription',grants[0]['resource'],'hello','String'],False)
        parameter=run_rpc.grant(version,'param.set')
        if version==1:
            proxy=CountingMaster(self.env['ROS_MASTER_URI'],'/uwvm_resources/value')
            thread=threading.Thread(target=proxy.serve_forever,daemon=True);thread.start()
            try:
                self.operation(version,'ros1-parameter_get_denied',parameter,['/tmp/ros1-build/uwvm-ros1-resources-probe','--witness'],connection_changes={'master-uri':f'http://127.0.0.1:{proxy.server_address[1]}'},proxy=proxy)
            finally:
                proxy.shutdown();proxy.server_close();thread.join(timeout=2)
                if thread.is_alive():raise RuntimeError('FAIL Master counting proxy cleanup')
        else:
            self.operation(version,'ros2-parameter_get_denied',parameter,['/tmp/ros2-build/uwvm-ros2-resources-probe','--observe-read-denied'])
        self.operation(version,f'ros{version}-tf_lookup_denied',run_rpc.grant(version,'tf.broadcast'),[f'/tmp/ros{version}-build/uwvm-ros{version}-tf-probe','--absent-lookup','/tf'])
        _,grants=original(version,['action-result'])
        self.operation(version,f'ros{version}-action_cancel_denied',grants[0],[f'/tmp/ros{version}-build/uwvm-ros{version}-action-probe','--active-native',grants[0]['resource']])

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace',default='/tmp/uwvm-ros-extension-20261012')
    parser.add_argument('--source',default='/tmp/uwvm-ros-extension-20261012/source')
    parser.add_argument('--uwvm',default='/tmp/uwvm-ros-extension-20261012/product-final-v6-event/dual/uwvm')
    parser.add_argument('--llvm-libs',default=os.environ.get('LD_LIBRARY_PATH',''))
    parser.add_argument('--version',choices=['1','2','both'],default='both')
    parser.add_argument('--mode',choices=['operations','startup','groups','all'],default='all')
    args=parser.parse_args();runner=AdditionalRunner(args)
    runner.run=runner.base/'additional-security'/time.strftime('%Y%m%d-%H%M%S');runner.run.mkdir(parents=True,mode=0o700)
    status='PASS';before={};after={}
    def interrupted(signum,frame):raise RuntimeError(f'FAIL interrupted by signal {signum}; bounded cleanup follows')
    signal.signal(signal.SIGTERM,interrupted);signal.signal(signal.SIGINT,interrupted)
    try:
        before=network.cgroup_snapshot();versions=(1,2) if args.version=='both' else (int(args.version),)
        runner.prepare(set(versions))
        for version in versions:
            if args.mode in ('operations','all'):runner.families(version)
            if args.mode in ('startup','all'):runner.startup_family(version)
        if args.mode in ('groups','all'):
            if set(versions)!={1,2}:raise RuntimeError('FAIL backend Group proof requires both real backends')
            runner.group_confusion()
    except (RuntimeError,OSError,subprocess.SubprocessError) as error:
        status='BLOCKED' if 'BLOCKED' in str(error) else 'FAIL';runner.results.append({'status':status,'reason':str(error)});print(status,error,flush=True)
    finally:
        try:runner.cleanup()
        except (RuntimeError,OSError,subprocess.SubprocessError) as error:
            status='FAIL';runner.results.append({'status':'FAIL','reason':'cleanup failure '+str(error)})
    try:after=network.cgroup_snapshot()
    except RuntimeError as error:status='FAIL';runner.results.append({'status':'FAIL','reason':str(error)})
    def events(snapshot):return dict(line.split() for line in snapshot.get('memory.events','').splitlines())
    if before and after and any(events(before).get(k)!=events(after).get(k) for k in ('oom','oom_kill')):
        status='FAIL';runner.results.append({'status':'FAIL','reason':'OOM counters increased'})
    (runner.run/'report.json').write_text(json.dumps({'status':status,'artifacts':runner.artifacts,'results':runner.results,'cgroup_before':before,'cgroup_after':after},indent=2)+'\n')
    print(status,runner.run,flush=True);return 0 if status=='PASS' else 77 if status=='BLOCKED' else 1
if __name__=='__main__':raise SystemExit(main())
