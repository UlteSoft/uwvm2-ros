#!/usr/bin/env python3
"""Execute one real bounded custom-interface row against complete frozen SourceB.

Closed selections are24 six-language client/server rows plus Topics, malformed
canonical rejection and default-denied rows for each backend. No fixed-only
Gateway/Worker or SourceA-derived peer can satisfy these rows. The shared Lane
owns every process birth, current64GiB/zero-swap/ancestor-OOM check and cleanup.
Compile preparation is separate; no compiler result is relabeled as delivery.
"""
from __future__ import annotations
import argparse
import importlib.util
import os
from pathlib import Path
import signal
import stat
import shutil
import subprocess
import sys
import time
import traceback
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(HERE.parents[1]/'ros_acceptance'))
from lane import Lane,add_arguments
from protocol import LANGUAGES,require,marker_lines
from run_delivery import gateway_config,worker_argv,start_master,translate,process_state
from fixtures import sdk_gate
from runtime import digest,elf
from gateway_qualification import architecture_binding
from prepare import qualify_peer,native,inventory

COMPOSITE='tests/ros_custom_types/deployment-final/generated'
AUDIT={'audit':'allow-and-deny','audit-max-rate-hz':'100','audit-burst':'100','audit-max-records':'10000','audit-max-bytes':'1048576'}
RATE={'max-rate-hz':'100','burst':'32','max-bytes-per-second':'65536','validator':'none'}
QOS={'reliability':'reliable','durability':'volatile','history':'keep-last','depth':'4','deadline':'infinite','lifespan':'infinite','liveliness':'automatic','liveliness-lease-duration':'infinite'}


def load(path,name):
    spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec);sys.modules[name]=module;spec.loader.exec_module(module);return module


def composite(lane,version):
    """Physically bind every original and new provider byte through SourceB."""
    root=lane.source/COMPOSITE;proof,pin=lane.json(root/'composition.json');outputs=proof.get('outputs',{})
    require(proof.get('format')=='uwvm-ros-custom-composite-v2' and proof.get('implicit_permissions')==[] and len(outputs)==25,'complete fixed/bounded26 bundle required')
    require(set(inventory(root))==set(outputs)|{'composition.json'},'undeclared/missing provider file')
    for name,expected in outputs.items():
        path=root/name;binding=lane.pin(path);require({k:binding[k] for k in ('bytes','sha256')}==expected and lane.frozen.get(str(path.relative_to(lane.source)))==expected,'provider is outside exact SourceB')
    generated,gpin=lane.json(root/'bounded/generation.json')
    require(generated.get('native_registry_emitted') is True and generated.get('format')=='uwvm-ros-custom-bounded-v2' and generated.get('implicit_permissions')==[],'official bound source profile missing')
    selected={r['type'].split('/')[-1]:r for r in generated['types'] if r['ros_version']==version}
    require(set(selected)=={'Point','Envelope','Echo','Empty','Digest'},'closed official fixture inventory differs')
    provider=lane.gateway_bindings[version]['custom_selection_binding']['selection']
    require(provider.get('enabled') is True and provider['registry_header'].get('source_relative')==COMPOSITE+'/manifest.hpp' and provider['native_header'].get('source_relative')==COMPOSITE+'/native_ros'+str(version)+'.hpp' and 'uwvm_ros_bounded_v2' in provider['packages'],'Gateway did not select complete frozen composite provider')
    report,_=lane.json(lane.args.worker_report);dual=next(r for r in report['profiles'] if r['profile']=='dual')
    consumed={name for unit in dual['dependencies'].values() for name in unit}
    require({COMPOSITE+'/manifest.hpp',COMPOSITE+'/fixed/manifest.hpp',COMPOSITE+'/bounded/manifest.hpp'}<=consumed,'actual Worker dependencies omit old or new registry')
    lane.report['composite_qualification']={'composition':pin,'bounded_generation':gpin,'native_types':selected,'retained_fixed_generation':lane.pin(root/'fixed/generation.json'),'registration_is_permission':False}
    return selected


def native_types(lane,types,version):
    """Bind final SourceB descriptor identities to its fresh official exporter."""
    report=lane.report['bounded_native_qualification'];prepared,_=lane.json(report['report']['path'])
    control=prepared['official_metadata'][str(version)]['output'];metadata,pin=lane.json(control['path']);require(pin==control,'current official export changed')
    require(set(metadata['types'])=={item['type'] for item in types.values()},'current official export inventory differs')
    for item in types.values():
        actual=metadata['types'][item['type']];official=actual.get('official',actual)
        require(official['native_identity']==item['native_identity'] and set(actual['components'])==set(item['components']),'official root/component inventory differs')
        for role,expected in item['components'].items():
            part=actual['components'][role];identity=part.get('official',part)
            require(identity['native_identity']==expected['native_identity'],'official component identity differs')
            name=part.get('type') if version==1 else part['root']['type']
            require(name==expected['type'],'official component native name differs')
    lane.report['current_official_export_binding']=pin


def policy(version,kind,types):
    """Explicit capability budgets only; native metadata supplies identities, not rights."""
    connection={'node-name':'bounded_gateway','namespace':'/uwvm_bounded_v2'}
    if version==1:connection.update({'master-uri':'http://127.0.0.1:11384','advertise-host':'127.0.0.1','transport':'tcpros'})
    else:connection.update({'domain-id':'205','rmw-implementation':'rmw_fastrtps_cpp','security-mode':'disabled-test','keystore':'not-applicable','enclave':'not-applicable'})
    caps=[]
    if kind=='denied':return connection,caps
    def grant(alias,operation,resource,name,limits):
        item=types[name];caps.append({'alias':alias,'operation':operation,'resource':'/uwvm_bounded_v2/'+resource,'type':item['type'],'identity':item['native_identity'],'limits':{**limits,**AUDIT,'adapter-codec-identity':item['adapter_codec_identity']}})
    if kind in ('topics','rejection'):
        for alias,operation,resource in (('bounded-pub','topic.publish','guest_topic'),('bounded-sub','topic.subscribe','native_topic')):
            if kind=='rejection' and alias=='bounded-sub':continue
            limits={**RATE,'burst':'4','max-message-bytes':'128','max-queue-bytes':'512','overflow-policy':'reject','max-publishers' if operation=='topic.publish' else 'max-subscribers':'1'}
            if version==1:limits.update({'queue-depth':'4','transport':'tcpros',**({'latch':'false'} if operation=='topic.publish' else {})})
            else:limits.update({'max-inflight':'1',**{'qos.'+key:value for key,value in QOS.items()}})
            grant(alias,operation,resource,'Envelope',limits)
    roles=[]
    if kind in ('client','rejection'):
        roles=[('bounded-echo','service.call','native_echo','Echo'),('bounded-empty','service.call','native_empty','Empty'),('bounded-send','action.send-goal','native_digest','Digest')]
        if kind=='client':roles += [('bounded-cancel','action.cancel-own-goal','native_digest','Digest'),('bounded-feedback','action.receive-feedback','native_digest','Digest'),('bounded-result','action.receive-result','native_digest','Digest')]
    elif kind=='server':roles=[('bounded-service-events','service.advertise','guest_echo','Echo'),('bounded-service-replies','service.respond','guest_echo','Echo'),('bounded-action-events','action.advertise','guest_digest','Digest'),('bounded-action-replies','action.respond','guest_digest','Digest')]
    for alias,operation,resource,name in roles:
        limits={**RATE,'timeout-ms':'10000','max-inflight':'2','max-request-bytes':'256','max-response-bytes':'256'}
        if operation.startswith('service.'):
            if operation=='service.advertise':limits.update({'max-services':'1','max-event-queue-depth':'4','max-event-queue-bytes':'2048','overflow-policy':'reject'})
            if version==2:limits.update({'qos.'+key:value for key,value in QOS.items()})
        else:
            limits={**RATE,'timeout-ms':'10000','max-active-goals':'1','max-actions':'1','max-goal-bytes':'128','max-feedback-bytes':'128','max-result-bytes':'128','cancel-scope':'own' if operation=='action.cancel-own-goal' else 'not-applicable','max-event-queue-depth':'4','max-event-queue-bytes':'2048','overflow-policy':'reject'}
            if version==2:
                for channel in ('goal-service','cancel-service','result-service','feedback','status'):limits.update({'qos.'+channel+'.'+key:value for key,value in QOS.items()})
        grant(alias,operation,resource,name,limits)
    return connection,caps


def native_markers(text,version,mode,pid):
    common=('backend','pid','mode');effects=('services','empty','goals','feedback','result','mismatch','topic_received','topic_sent','quiet_samples','quiet_ns')
    schema={name:common for name in ('READY_BOUNDED_NATIVE','MATCHED_BOUNDED_NATIVE','FEEDBACK_SENT_BOUNDED_NATIVE','RESULT_SENT_BOUNDED_NATIVE','FEEDBACK_DELIVERED_BOUNDED_NATIVE','DELIVERED_BOUNDED_NATIVE')};schema['PASS_BOUNDED_NATIVE']=common+effects
    parsed=marker_lines(text,'',schema);names=[name for name,_ in parsed]
    expected=['READY_BOUNDED_NATIVE']+({'topics':['MATCHED_BOUNDED_NATIVE'],'client':['FEEDBACK_DELIVERED_BOUNDED_NATIVE','DELIVERED_BOUNDED_NATIVE'],'server':['FEEDBACK_SENT_BOUNDED_NATIVE','RESULT_SENT_BOUNDED_NATIVE'],'absent':[]}[mode])+['PASS_BOUNDED_NATIVE']
    require(names==expected,'native ordered marker inventory differs')
    for name,row in parsed:
        require(row['backend']==str(version) and row['pid']==str(pid) and row['mode']==mode,'native backend/birth/mode differs')
        if name=='PASS_BOUNDED_NATIVE':
            non_topic='1' if mode in ('server','client') else '0'
            require(all(row[k]==non_topic for k in ('services','goals','feedback','result')) and row['empty']==('1' if mode=='server' else '0') and row['mismatch']=='0','independent native Service/Action counts differ')
            require(row['topic_received']==row['topic_sent']==('1' if mode=='topics' else '0'),'independent native Topic counts differ')
            require(row['quiet_samples'].isdigit() and 30<=int(row['quiet_samples'])<=10000 and row['quiet_ns'].isdigit() and int(row['quiet_ns'])>=2000000000,'native quiet observation missing')
    return parsed


def advance(lane,child,row,complete,again,gateway,worker,feedback=None):
    """Drive bounded passes, releasing one terminal only after actual feedback.

    The optional (exact marker, hook) is invoked once while the current R pass
    is blocked on E. This is a causal delivery gate, not a scheduling sleep or
    permission grant. A completion without that required observation refuses.
    """
    delivered=False
    for _ in range(200):
        require(gateway.poll() is None or worker.poll()==0,'Gateway exited before its actual Worker');prior=lane.read(row).count(again+'\n');lane.send(child,row,b'R','one externally paced bounded pass; no send/call replay')
        until=time.monotonic()+12
        while time.monotonic()<until:
            lane.check();require(gateway.poll() is None or worker.poll()==0,'Gateway exited before actual Worker');text=lane.read(row)
            if feedback is not None:
                marker,release=feedback;count=text.splitlines().count(marker)
                require(count<=1,'feedback delivery marker repeated')
                if count and not delivered:release();delivered=True;text=lane.read(row)
            if complete+'\n' in text:
                require(feedback is None or delivered,'completion preceded required feedback delivery gate');return
            if text.count(again+'\n')>prior:break
            require(child.poll() is None,'Guest retired before checked completion');time.sleep(.005)
        else:raise TimeoutError('bounded custom one-pass completion missing')
        time.sleep(.02)
    raise TimeoutError('bounded custom200-pass budget exhausted')


def main():
    parser=argparse.ArgumentParser(description=__doc__);add_arguments(parser)
    parser.add_argument('--prepared-native',type=Path,required=True);parser.add_argument('--wat2wasm',type=Path,required=True)
    parser.add_argument('--version',type=int,choices=(1,2),required=True);parser.add_argument('--case',choices=('client','server','topics','denied','rejection'),required=True)
    parser.add_argument('--language',choices=LANGUAGES);parser.add_argument('--products',type=Path);parser.add_argument('--node',type=Path)
    args=parser.parse_args();require(args.case not in ('client','server') or args.language and args.products,'SDK direction requires language/product report')
    require(args.case in ('client','server') or not args.language,'raw Topic/negative case is separately scoped')
    require(args.language!='javascript' or args.node,'explicit pinned host Node required')
    lane=None;status='FAIL'
    try:
        lane=Lane(args,'bounded-custom-native-behavior');require(Path(__file__).resolve()==lane.source/'tests/ros_custom_types/bounded-v2/run_custom_bounded.py','run exact frozen behavior driver');v=args.version;lane.report.update(version=v,case=args.case,language=args.language,actual_ROS_delivery='RUNNING',G01_complete=False)
        lane.qualify_workers();lane.qualify_gateway(v)
        worker_report,_=lane.json(args.worker_report);require(worker_report.get('cgroup_incarnation_before')==lane.before,'Worker build belongs to another current leaf/boot')
        gateway_arch,_=architecture_binding(lane.gateway_bindings[v]['architecture_evidence']['path'],lane.pins);require(gateway_arch.get('cgroup_incarnation_before')==lane.before,'Gateway build belongs to another current leaf/boot')
        types=composite(lane,v);selection=policy(v,args.case,types);lane.report['explicit_policy']=selection
        root=lane.root(v);temporary=lane.out/('ros'+str(v))/'tmp';origin=qualify_peer(lane,args.prepared_native,v);native_types(lane,types,v)
        lane.snapshot(origin,temporary/'peer','bounded-peer-runtime')
        sdk_wasm=None;rights=[];host=None
        if args.case in ('client','server'):
            builder=load(lane.source/'tests/ros_custom_types/bounded-v2/build_guests.py','bounded_guest_builder')
            origin_wasm,rights=builder.qualify_product(lane,args.products,args.language,v,args.case)
            origin_pin=lane.pin(origin_wasm);sdk_wasm=lane.out/'sdk-guest.wasm';shutil.copy2(origin_wasm,sdk_wasm);sdk_wasm.chmod(0o400);copy_pin=lane.pin(sdk_wasm)
            require(all(copy_pin[k]==origin_pin[k] for k in ('bytes','sha256')),'separate immutable Guest copy differs');lane.report['selected_guest_snapshot']={'origin':origin_pin,'snapshot':copy_pin}
            if args.language=='javascript':
                products,_=lane.json(args.products);row=next(r for r in products['products'] if (r['language'],r['version'],r['role'])==(args.language,v,args.case));host=Path(row['js_host_entry']['path']);require(lane.pin(host)==row['js_host_entry'],'actual JS host input changed')
                require(args.node.resolve(strict=True)==Path(row['js_host_argv'][0]).resolve(strict=True) and row['js_host_argv'][1:]==[str(host),str(v),args.case],'host Node/source/backend/role must match actual prepared syntax/tool selection');args.node=args.node.resolve(strict=True)
            wasm=translate(lane,args.wat2wasm,'sdk-gate',sdk_gate())
        else:
            module=load(lane.source/'tests/ros_custom_types/bounded-v2/fixtures.py','bounded_wat_source');wasm=translate(lane,args.wat2wasm,'bounded-'+args.case,module.bounded_guest(v,args.case))
        if v==1:start_master(lane)
        else:lane.lock('domain-205')
        mode={'client':'server','server':'client','topics':'topics','denied':'absent','rejection':'absent'}[args.case]
        peer,peer_row=lane.start('bounded-native-peer',native(lane,v,['/tmp/peer',mode]),lane.environment(v,gateway=True),seconds=120)
        proof=lane.stopped(peer,peer_row,'bounded-peer-runtime',cooperative=True);native_pid=proof['actual_stopped_ARM']['pid']
        if args.case!='server':lane.send(peer,peer_row,b'I','actual native map before ROS initialization');lane.wait_marker(peer,peer_row,'READY_BOUNDED_NATIVE')
        argv=worker_argv(lane,(v,),wasm,sdk_wasm,rights,selections={v:selection})
        binary=args.language=='javascript';worker,wrow=lane.start('bounded-worker',['/bin/bash','-c','kill -STOP $$; exec "$@"','bounded-worker',*lane.config['qemu_argv'],*argv],stdout=subprocess.PIPE if binary else None,seconds=120,rss=8*1024**3)
        until=time.monotonic()+5
        while worker.poll() is None and process_state(worker.pid) not in ('T','t') and time.monotonic()<until:lane.check();time.sleep(.001)
        require(worker.poll() is None and process_state(worker.pid) in ('T','t'),'PID-preserving Worker shell did not stop')
        lane.report['worker_argv']=argv;config=lane.short/('ros'+str(v)+'.conf');config.write_text(gateway_config(v,worker.pid,False,selection));config.chmod(0o600)
        gateway,grow=lane.start('bounded-gateway',native(lane,v,['/tmp/gateway','/guest/'+config.name]),lane.environment(v,gateway=True),seconds=120)
        lane.stopped(gateway,grow,'gateway-'+str(v));sock=lane.short/('ros'+str(v)+'.sock');until=time.monotonic()+30
        while not sock.exists() and time.monotonic()<until:lane.check();require(gateway.poll() is None,'Gateway retired before socket readiness');time.sleep(.01)
        require(sock.exists() and stat.S_ISSOCK(sock.lstat().st_mode) and sock.lstat().st_uid==os.getuid(),'owned Gateway socket unavailable')
        owner=next(r for r in lane.owned.rows.values() if r['identity']['pid']==worker.pid);lane.owned.send(owner,signal.SIGCONT);lane.stopped(worker,wrow,'worker');lane.send(worker,wrow,b'G','mapped peer/Worker/Gateway and exact fixed-PID socket')
        sdk,srow=worker,wrow
        if binary:
            lane.pin(args.node);require(elf(args.node)['e_machine']==62,'explicit host Node ELF required')
            sdk,srow=lane.start('bounded-js-host',['/bin/bash','-c','exec 3<&"$1" 4>&"$2"; shift 2; exec "$@"','bounded-pipes',str(worker.stdout.fileno()),str(worker.stdin.fileno()),str(args.node),str(host),str(v),args.case],pass_fds=(worker.stdout.fileno(),worker.stdin.fileno()),seconds=120,rss=96*1024**2)
            lane.stopped_host(sdk,srow,args.node)
        if args.case=='client':
            lane.wait_marker(sdk,srow,'SDK_BOUNDED_GOAL_SENT');lane.send(peer,peer_row,b'F','actual typed Guest Goal submitted; permit one native feedback');lane.wait_marker(peer,peer_row,'FEEDBACK_SENT_BOUNDED_NATIVE')
            def release_native_result():
                lane.send(peer,peer_row,b'E','actual Guest matching typed feedback decoded once');lane.wait_marker(peer,peer_row,'RESULT_SENT_BOUNDED_NATIVE')
                lane.send(sdk,srow,b'E','native terminal publication follows actual Guest feedback delivery')
            advance(lane,sdk,srow,'PASS_BOUNDED_CUSTOM_CLIENT','SDK_BOUNDED_AGAIN',gateway,worker,('SDK_BOUNDED_FEEDBACK_DELIVERED',release_native_result))
        elif args.case=='server':
            lane.wait_marker(sdk,srow,'SDK_BOUNDED_SERVERS_READY');lane.send(peer,peer_row,b'I','mapped native client plus actual typed Guest advertisements');lane.wait_marker(peer,peer_row,'READY_BOUNDED_NATIVE')
            def release_guest_result():
                lane.wait_marker(peer,peer_row,'FEEDBACK_DELIVERED_BOUNDED_NATIVE')
                lane.send(sdk,srow,b'E','independent native matching typed feedback callback completed once')
            advance(lane,sdk,srow,'SDK_BOUNDED_REPLIES_SENT','SDK_BOUNDED_AGAIN',gateway,worker,('SDK_BOUNDED_FEEDBACK_SENT',release_guest_result));lane.wait_marker(peer,peer_row,'DELIVERED_BOUNDED_NATIVE');lane.send(sdk,srow,b'D','independent native exact response/feedback/result delivery before owner cleanup');lane.wait_marker(sdk,srow,'PASS_BOUNDED_CUSTOM_SERVER')
        elif args.case=='topics':
            lane.wait_marker(sdk,srow,'BOUNDED_TOPIC_READY');lane.wait_marker(peer,peer_row,'MATCHED_BOUNDED_NATIVE');lane.send(sdk,srow,b'P','exact independent native Topic counterpart counts1/1');advance(lane,sdk,srow,'PASS_BOUNDED_TOPICS','BOUNDED_AGAIN',gateway,worker)
        else:lane.wait_marker(sdk,srow,'PASS_BOUNDED_'+args.case.upper())
        lane.finish(sdk,srow)
        if sdk is not worker:lane.finish(worker,wrow)
        lines=lane.read(srow).splitlines();expected={'client':['SDK_BOUNDED_GOAL_SENT','SDK_BOUNDED_FEEDBACK_DELIVERED','PASS_BOUNDED_CUSTOM_CLIENT'],'server':['SDK_BOUNDED_SERVERS_READY','SDK_BOUNDED_FEEDBACK_SENT','SDK_BOUNDED_REPLIES_SENT','PASS_BOUNDED_CUSTOM_SERVER'],'topics':['BOUNDED_TOPIC_READY','PASS_BOUNDED_TOPICS'],'denied':['PASS_BOUNDED_DENIED'],'rejection':['PASS_BOUNDED_REJECTION']}[args.case]
        again='BOUNDED_AGAIN' if args.case=='topics' else 'SDK_BOUNDED_AGAIN';require([line for line in lines if line!=again]==expected and lines.count(again)<200,'Guest exact ordered completion transcript differs')
        lane.send(peer,peer_row,b'Q','exact typed Guest transcript and actual Worker exit0');lane.finish(peer,peer_row)
        parsed=native_markers(lane.read(peer_row),v,mode,native_pid)
        # Production Gateway exits0 when its exact fixed Worker pidfd becomes
        # readable. This terminal observation follows actual Worker0 and every
        # independent effect/quiet marker; it never admits an earlier/nonzero exit.
        require(gateway.poll() in (None,0),'Gateway terminal code differs after actual Worker0');lane.finish(gateway,grow)
        lane.report.update(new_tests_executed=True,actual_ROS_delivery='PASS' if args.case in ('client','server','topics') else 'NOT_APPLICABLE_NEGATIVE',actual_guest_behavior='PASS',cases=[{'status':'PASS','case':args.case,'language':args.language,'version':v,'native_markers':parsed,'guest_markers':lines,'actual_worker_returncode':worker.returncode,'malformed_canonical_refusals':16 if args.case=='rejection' else 0,'default_alias_denials':11 if args.case=='denied' else 1 if args.case in ('topics','rejection') else 0,'stale9':args.case in ('topics','rejection')}],scope='Recorded selected finite custom profiles/direction only; no arbitrary dynamic ROS type, full RMW heap or original-task completion promotion');status='PASS'
    except BaseException as error:
        if lane is None:raise
        lane.report.update(error=repr(error),traceback=traceback.format_exc(),actual_ROS_delivery='FAIL')
    finally:
        if lane is not None:status=lane.close(status)
    return 0 if status=='PASS' else 1

if __name__=='__main__':raise SystemExit(main())
