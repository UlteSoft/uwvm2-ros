#!/usr/bin/env python3
"""Prepare complete explicit grants for individually selected C/C++ examples.

This source-only tool writes reviewable Worker argv and Gateway configuration;
it never executes a policy check, starts a process or applies authority. Each
--alias is intentional. Unknown/version-unsupported roles are refused before
output. Run the emitted policy-check argv yourself before admitting a Guest.
The disabled-test ROS2 profile requires explicit opt-in and is not Enforce proof.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[2]
QOS={'reliability':'reliable','durability':'volatile','history':'keep-last','depth':'4',
     'deadline':'infinite','lifespan':'infinite','liveliness':'automatic','liveliness-lease-duration':'infinite'}
AUDIT={'audit':'allow-and-deny','audit-max-rate-hz':'100','audit-burst':'10','audit-max-records':'10000','audit-max-bytes':'1048576'}
RATE={'max-rate-hz':'10','burst':'4','max-bytes-per-second':'65536'}
TYPES={
  'Trigger':{1:('std_srvs/Trigger','937c9679a518e3a18d831e57125ea522'),2:('std_srvs/srv/Trigger','RIHS01_eeff2cd6fa5ad9d27cdf4dec64818317839b62f212a91e6b5304b634b2062c5f')},
  'SetBool':{1:('std_srvs/SetBool','09fb03525b03e7ea1fd3992bafd87e16'),2:('std_srvs/srv/SetBool','RIHS01_abe9e4bb6b41b40e6789712c00ec8871923e089af3f667a79992a428cff2da0a')},
  'Fibonacci':{1:('actionlib_tutorials/Fibonacci','f59df5767bf7634684781c92598b2406'),2:('action_tutorials_interfaces/action/Fibonacci','RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0')},
  'Transform':{1:('geometry_msgs/TransformStamped','b5764a33bfeb3588febc2682852579b0'),2:('geometry_msgs/msg/TransformStamped','RIHS01_0a241f87d04668d94099cbb5ba11691d5ad32c2f29682e4eb5653424bd275206')},
}

def qos(prefix='qos.'):
    """Return every explicit bounded QoS field; never infer middleware defaults."""
    return {prefix+k:v for k,v in QOS.items()}

def grant(args,alias):
    """Build one exact selected alias; policy binding stays independent of SDK labels."""
    v=args.backend;limits=dict(AUDIT);limits.update(RATE);base=args.resource
    if alias in ('pub','sub'):
        op='topic.publish' if alias=='pub' else 'topic.subscribe'
        kind,identity=base_tool().STRINGS[v];resource=base+('/pub' if alias=='pub' else '/sub')
        limits.update({'max-message-bytes':'64','max-queue-bytes':'256','overflow-policy':'drop-oldest','validator':'none',
                       'max-publishers' if alias=='pub' else 'max-subscribers':'1'})
        if v==1:
            limits.update({'queue-depth':'4','transport':'tcpros'})
            if alias=='pub':limits['latch']='false'
        else:limits.update({'max-inflight':'1',**qos()})
    elif alias.startswith(('trigger-','set-bool-')):
        stem='trigger' if alias.startswith('trigger-') else 'set-bool';role=alias[len(stem)+1:]
        if role not in ('call','advertise','respond'):raise ValueError('unknown service alias')
        op='service.'+role;kind,identity=TYPES['Trigger' if stem=='trigger' else 'SetBool'][v];resource=base+'/'+stem.replace('-','_')
        limits.update({'timeout-ms':'3000','max-inflight':'1','max-request-bytes':'64','max-response-bytes':'512','validator':'none'})
        if role=='advertise':limits.update({'max-services':'1','max-event-queue-depth':'4','max-event-queue-bytes':'4096','overflow-policy':'reject'})
        if v==2:limits.update(qos())
    elif alias in ('goal-send','goal-feedback','goal-result','goal-cancel-own','goal-cancel-any','fib-advertise','fib-respond'):
        op='action.'+{'goal-send':'send-goal','goal-feedback':'receive-feedback','goal-result':'receive-result',
             'goal-cancel-own':'cancel-own-goal','goal-cancel-any':'cancel-any-goal','fib-advertise':'advertise','fib-respond':'respond'}[alias]
        kind,identity=TYPES['Fibonacci'][v];resource=base+'/fibonacci'
        limits.update({'max-active-goals':'1','max-actions':'1','timeout-ms':'3000','max-goal-bytes':'4','max-feedback-bytes':'512',
          'max-result-bytes':'512','fibonacci.max-order':'12','cancel-scope':'own' if alias=='goal-cancel-own' else 'any' if alias=='goal-cancel-any' else 'not-applicable',
          'max-event-queue-depth':'4','max-event-queue-bytes':'4096','overflow-policy':'reject','validator':'none'})
        if v==2:
            for channel in ('goal-service','cancel-service','result-service','feedback','status'):limits.update(qos('qos.'+channel+'.'))
    elif alias.startswith('parameter-'):
        role=alias[len('parameter-'):];op='param.'+('subscribe-events' if role=='events' else role)
        if role not in ('get','set','delete','exists','list','search','describe','declare','undeclare','events'):raise ValueError('unknown parameter alias')
        kind='builtin/'+args.parameter_kind+'-v1';identity='builtin:param-'+args.parameter_kind+'-v1'
        if role in ('delete','exists','list','search','describe'):kind='builtin/string-v1';identity='builtin:param-string-v1'
        if role=='events':kind='builtin/param-event-'+args.parameter_kind+'-v1';identity='builtin:param-event-'+args.parameter_kind+'-v1'
        resource=base+'/value' if v==1 else args.parameter_node+('/value' if role!='list' else '')
        limits.update({'max-parameter-bytes':'1024','timeout-ms':'3000','max-inflight':'1','validator':'none'})
        if args.parameter_kind=='float64' and role in ('get','set','declare','undeclare','events'):
            limits.update({'parameter-min-value':'-1000','parameter-max-value':'1000','validator':'finite'})
        if v==2:limits.update({'parameter-node':args.parameter_node,'parameter-name':'not-applicable' if role=='list' else 'value'})
        if role in ('list','search'):limits['allowed-name-prefix']=base if v==1 else args.parameter_node
        if role=='list' and v==2:limits['list-depth']='4'
        if role=='search':limits['search-parent-namespaces']='false'
        if role=='events':limits.update({'max-subscribers':'1','max-event-fields':'8','max-event-queue-depth':'4','max-event-queue-bytes':'4096','overflow-policy':'reject',**qos()})
        elif v==2 and role not in ('declare','undeclare'):limits.update(qos())
    elif alias.startswith('graph-'):
        role={'topic':'inspect-topic','node':'inspect-node','service':'inspect-service','action':'inspect-action','topics':'list-topics','nodes':'list-nodes','services':'list-services'}.get(alias[6:])
        if not role:raise ValueError('unknown graph alias')
        op='graph.'+role;kind='builtin/graph-list-v1';identity='builtin:graph-list-v1'
        resource=base if role.startswith('list-') else base+('/fibonacci' if role=='inspect-action' else '/trigger' if role=='inspect-service' else '/sdk_peer' if role=='inspect-node' else '/pub')
        limits.update({'allowed-name-prefix':base,'max-graph-result-bytes':'1024','timeout-ms':'3000','max-inflight':'1'})
    elif alias in ('ros-time','wall-time','system-time','steady-time'):
        op='time.read-'+alias.split('-')[0];kind='builtin/time-v1';identity='builtin:time-nanoseconds-v1';resource=base+'/clock'
        limits.update({'max-inflight':'1','max-response-bytes':'8'})
    elif alias=='log-info':
        op='log.write';kind='builtin/log-v1';identity='builtin:log-utf8-v1';resource=base+'/logger'
        limits.update({'max-inflight':'1','max-message-bytes':'1024','max-log-bytes-per-second':'65536','severity':'info'})
    elif alias in ('tf-lookup','tf-broadcast'):
        op='tf.'+alias[3:];kind,identity=TYPES['Transform'][v];resource='/tf'
        limits.update({'source-frame':'map','target-frame':'base_link','tf.max-translation-abs':'100','tf.quaternion-norm-tolerance':'0.001','timeout-ms':'3000',
          'max-message-bytes':'1024','max-inflight':'1','max-queue-bytes':'4096','overflow-policy':'reject','buffer-cache-ms':'10000','validator':'transform-bounds'})
        if v==1:
            limits.update({'queue-depth':'4','transport':'tcpros'})
            if alias=='tf-broadcast':limits['latch']='false'
        else:limits.update(qos())
    elif alias in ('lifecycle-get','lifecycle-change'):
        op='lifecycle.'+('get-state' if alias=='lifecycle-get' else 'change-state');resource=base+'/lifecycle_node'
        kind='lifecycle_msgs/srv/'+('GetState' if alias=='lifecycle-get' else 'ChangeState')
        identity='RIHS01_'+('800a0a5aae599782b02932de0caf563f6dc4e7e94b794eadde075ba2cbef9795' if alias=='lifecycle-get' else '356fe34f0475a43acf54542013af4167b0e729f77ea22ffb045c6ad8e20668e5')
        limits.update({'timeout-ms':'3000','max-inflight':'1','max-request-bytes':'64','max-response-bytes':'1024',**qos()})
        if alias=='lifecycle-change':limits['allowed-transition']='configure'
    else:raise ValueError('unknown alias: '+alias)
    # Check registration only. A successful policy check is still not Runtime
    # native readiness, remote declaration support or delivery evidence.
    raw=op.replace('.','_').replace('-','_').replace('param_','parameter_',1)
    spec=json.loads((ROOT/'abi/abi-v1.json').read_text())['modules'][f'uwvm_ros{v}_v1']['functions']
    if raw not in {row['name'] for row in spec}:raise ValueError('operation is absent for selected ROS version: '+op)
    return dict(alias=alias,operation=op,resource=resource,type=kind,identity=identity,limits=limits)

_BASE=None
def base_tool():
    """Read the checked tutorial's connection serializer from this installed SDK."""
    global _BASE
    if _BASE is None:
        spec=importlib.util.spec_from_file_location('roswasm_tutorial_policy',ROOT/'tools/example_policy.py')
        _BASE=importlib.util.module_from_spec(spec);spec.loader.exec_module(_BASE)
    return _BASE

def policy(args):
    """Prepare only explicitly selected aliases; perform no Runtime operation."""
    args.operation=[];result=base_tool().policy(args);prefix=f'--ros{args.backend}-global-'
    result['grants']=[grant(args,alias) for alias in args.alias]
    lines=result['Gateway_config_text'].splitlines();cli=result['Worker_arguments']
    for row in result['grants']:
        fields=[row[k] for k in ('alias','operation','resource','type','identity')]
        cli.extend([prefix+'grant',*fields]);lines.append('grant '+' '.join(fields))
        for key,value in row['limits'].items():cli.extend([prefix+'set',row['alias'],key,value]);lines.append('set '+row['alias']+' '+key+' '+value)
    result['Worker_policy_check_arguments']=[*cli,f'--ros{args.backend}-check-policy'];result['Gateway_config_text']='\n'.join(lines)+'\n'
    result.update(status='PREPARED_NOT_VALIDATED_NOT_EXECUTED',default_authority='empty except basic stdin/stdout/stderr',
        native_acceptance='NOT_PERFORMED',implicit_rights=[],limits_are_explicit_tutorial_choices=True)
    return result

def main():
    """Require actual principal/socket binding and write a fresh private folder."""
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--backend',type=int,choices=(1,2),required=True);p.add_argument('--alias',action='append',required=True)
    p.add_argument('--socket',type=Path,required=True);p.add_argument('--worker-uid',type=int,required=True);p.add_argument('--worker-pid',type=int,required=True)
    p.add_argument('--gateway-uid',type=int,required=True);p.add_argument('--node-name',required=True);p.add_argument('--namespace',required=True)
    p.add_argument('--resource',default='/uwvm_sdk_relay');p.add_argument('--parameter-node',default='/uwvm_sdk_relay/sdk_parameter_node')
    p.add_argument('--parameter-kind',choices=('string','float64'),default='string');p.add_argument('--master-uri');p.add_argument('--advertise-host')
    p.add_argument('--domain-id',type=int);p.add_argument('--disabled-security-tutorial',action='store_true');p.add_argument('--out',type=Path,required=True)
    args=p.parse_args();identifier=r'[A-Za-z_][A-Za-z0-9_]*';absolute='/'+identifier+'(?:/'+identifier+')*'
    if len(args.alias)!=len(set(args.alias)):p.error('each selected alias appears once')
    if args.worker_pid<=0 or any(not 0<=uid<=4294967294 for uid in (args.worker_uid,args.gateway_uid)):p.error('explicit valid UID/PID required')
    if not re.fullmatch(identifier,args.node_name):p.error('canonical node basename required')
    for value in (args.namespace,args.resource,args.parameter_node):
        if value!='/' and not re.fullmatch(absolute,value):p.error('canonical absolute namespace/resource required')
    if args.resource=='/' or args.parameter_node=='/':p.error('concrete resource and parameter node required')
    socket=str(args.socket)
    if not re.fullmatch(r'/[^\s\x00]+',socket) or len(socket.encode())>107 or any(part in socket.split('/') for part in ('.','..')) or '//' in socket:p.error('short canonical absolute private socket required')
    if args.domain_id is not None and not 0<=args.domain_id<=232:p.error('domain is0..232')
    for value in (args.master_uri,args.advertise_host):
        if value is not None and (not value or any(c.isspace() or c=='\0' for c in value)):p.error('connection fields are single tokens')
    if args.master_uri is not None:
        match=re.fullmatch(r'http://([^/?#@]+):(\d+)/?',args.master_uri)
        if not match or not 1<=int(match.group(2))<=65535:p.error('explicit Master http://host:port required')
    if args.advertise_host is not None and any(c in args.advertise_host for c in '/\\'):p.error('advertise host is not a path')
    try:result=policy(args)
    except ValueError as error:p.error(str(error))
    args.out.mkdir(mode=0o700,parents=True,exist_ok=False);config=args.out/'gateway.conf';config.write_text(result['Gateway_config_text']);config.chmod(0o600)
    (args.out/'policy.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],args.out/'policy.json')
if __name__=='__main__':main()
