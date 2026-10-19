#!/usr/bin/env python3
"""Generate a reviewable, explicit String tutorial policy without executing it.

Every output grant fixes its resource, trusted type identity and all applicable
limits. The tutorial profile is intentionally visible in this source and the
generated JSON; it is not a Runtime default. This tool never starts a Gateway,
changes permissions, reads ROS environment variables or infers credentials.
The caller supplies the actual Worker PID/UID, Gateway UID and private socket.
"""
import argparse
import json
from pathlib import Path
import re

STRINGS={1:('std_msgs/String','992ce8a1687cec8c8bd883ec73ca41d1'),
         2:('std_msgs/msg/String','RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18')}


def gateway_connection(version,connection):
    """Serialize only native backend fields, as Runtime's binding manifest does.

    Socket/peer authentication, connection timeout and unresolved-denial audit
    budgets belong to the Worker. The native Gateway has separate top-level
    socket, Worker identity, timeout and audit fields. Passing Worker transport
    fields as native ROS connection identity makes the real Gateway refuse it.
    """
    fields={'node-name','namespace'}
    if version==1:fields.update(('master-uri','advertise-host','transport'))
    elif version==2:fields.update(('domain-id','rmw-implementation','security-mode','keystore','enclave'))
    else:raise ValueError('explicit ROS backend must be1 or2')
    if not fields.issubset(connection):raise ValueError('incomplete native backend connection')
    return {key:value for key,value in connection.items() if key in fields}


def policy(args):
    """Return explicit CLI arguments and trusted Gateway configuration bytes."""
    version=args.backend;prefix=f'--ros{version}-global-'
    connection={'gateway-socket':str(args.socket),'gateway-uid':str(args.gateway_uid),
        'node-name':args.node_name,'namespace':args.namespace,'connect-timeout-ms':'3000',
        'audit-deny-max-rate-hz':'100','audit-deny-burst':'10','audit-deny-max-records':'10000',
        'audit-deny-max-bytes':'1048576'}
    if version==1:
        if args.master_uri is None or args.advertise_host is None:
            raise ValueError('ROS1 needs explicit --master-uri and --advertise-host')
        if args.domain_id is not None:raise ValueError('ROS1 does not select a DDS domain')
        connection.update({'master-uri':args.master_uri,'advertise-host':args.advertise_host,'transport':'tcpros'})
    else:
        if args.domain_id is None:raise ValueError('ROS2 needs explicit --domain-id')
        if args.master_uri is not None or args.advertise_host is not None:
            raise ValueError('ROS2 does not select a ROS1 Master connection')
        if not args.disabled_security_tutorial:
            raise ValueError('this tutorial emits disabled-test security only; opt in explicitly with --disabled-security-tutorial')
        connection.update({'domain-id':str(args.domain_id),'rmw-implementation':'rmw_fastrtps_cpp',
            'security-mode':'disabled-test','keystore':'not-applicable','enclave':'not-applicable'})
    grants=[]
    for operation,alias in (('publish','pub'),('subscribe','sub')):
        if operation not in args.operation:continue
        limits={'max-message-bytes':'4096','max-rate-hz':'10','burst':'4','max-bytes-per-second':'65536',
            'max-queue-bytes':'16384','overflow-policy':'drop-oldest','validator':'none',
            'audit':'allow-and-deny','audit-max-rate-hz':'100','audit-burst':'10',
            'audit-max-records':'10000','audit-max-bytes':'1048576',
            'max-publishers' if operation=='publish' else 'max-subscribers':'1'}
        if version==1:
            limits.update({'queue-depth':'4','transport':'tcpros'})
            if operation=='publish':limits['latch']='false'
        else:
            limits.update({'max-inflight':'1','qos.reliability':'reliable','qos.durability':'volatile',
                'qos.history':'keep-last','qos.depth':'4','qos.deadline':'infinite','qos.lifespan':'infinite',
                'qos.liveliness':'automatic','qos.liveliness-lease-duration':'infinite'})
        grants.append({'alias':alias,'operation':'topic.'+operation,'resource':args.resource,
                       'type':STRINGS[version][0],'identity':STRINGS[version][1],'limits':limits})
    if getattr(args,'observe_sub',False):
        if 'subscribe' not in args.operation:
            raise ValueError('--observe-sub requires separately selected --operation subscribe')
        # This optional profile is an explicit new grant. It never adds a source
        # operation, and obtaining the SDK does not enable it by default.
        grants.append({'alias':'subEvents','operation':'event.observe','resource':args.resource,
            'type':'builtin/event-v1','identity':'builtin:ros-event-envelope-le64-v1','limits':{
                'event-source-alias':'sub','event-kinds':'topic-message,resource-error,resource-closed',
                'max-event-bytes':'4160','max-event-queue-depth':'8','max-event-queue-bytes':'32768',
                'max-inflight':'2','max-rate-hz':'10','burst':'4','max-bytes-per-second':'65536',
                'overflow-policy':'reject','audit':'allow-and-deny','audit-max-rate-hz':'100',
                'audit-burst':'10','audit-max-records':'10000','audit-max-bytes':'1048576'}})
    cli=['--wasip1-global-noinherit-system-environment',prefix+'enable']
    for key,value in connection.items():cli.extend((prefix+key,value))
    lines=[f'backend ros{version}',f'worker-uid {args.worker_uid}',f'worker-pid {args.worker_pid}',
        # Runtime preflight closes its authenticated connection before Guest
        # start, but the native session thread may still be retiring when the
        # first business connection arrives. Bound both slots explicitly;
        # neither slot adds a ROS operation, resource or different principal.
        'socket '+str(args.socket),'timeout-ms 3000','max-sessions 2',
        'audit-max-records-per-second 100','audit-max-records 10000','audit-max-bytes 1048576']
    native_connection=gateway_connection(version,connection)
    lines.extend('connection '+key+' '+value for key,value in native_connection.items())
    for grant in grants:
        fields=[grant[key] for key in ('alias','operation','resource','type','identity')]
        cli.extend((prefix+'grant',*fields));lines.append('grant '+' '.join(fields))
        for key,value in grant['limits'].items():
            cli.extend((prefix+'set',grant['alias'],key,value))
            lines.append('set '+grant['alias']+' '+key+' '+value)
    return {'backend':version,'connection':connection,'Gateway_connection':native_connection,'grants':grants,'Worker_arguments':cli,
            'Worker_policy_check_arguments':[*cli,f'--ros{version}-check-policy'],
            'Gateway_config_text':'\n'.join(lines)+'\n'}


def main():
    """Require explicit caller binding and create a new private output folder."""
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--backend',type=int,choices=(1,2),required=True)
    parser.add_argument('--socket',type=Path,required=True)
    parser.add_argument('--worker-uid',type=int,required=True);parser.add_argument('--worker-pid',type=int,required=True)
    parser.add_argument('--gateway-uid',type=int,required=True)
    parser.add_argument('--node-name',required=True);parser.add_argument('--namespace',required=True)
    parser.add_argument('--resource',required=True);parser.add_argument('--master-uri');parser.add_argument('--advertise-host')
    parser.add_argument('--operation',choices=('publish','subscribe'),action='append',required=True,
                        help='Grant only these explicitly selected String topic operations; repeat to select both')
    parser.add_argument('--observe-sub',action='store_true',
                        help='Explicitly add subEvents unified data/error/closed observation; requires separately selected subscribe')
    parser.add_argument('--domain-id',type=int);parser.add_argument('--disabled-security-tutorial',action='store_true')
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args()
    if len(args.operation)!=len(set(args.operation)):parser.error('select each tutorial operation only once')
    for value in (args.worker_uid,args.gateway_uid):
        if not 0<=value<=4294967294:parser.error('UID must be in the public explicit range')
    if args.worker_pid<=0:parser.error('a positive actual Worker PID is required')
    if args.domain_id is not None and not 0<=args.domain_id<=232:parser.error('DDS domain is0..232')
    socket=str(args.socket)
    if not args.socket.is_absolute() or len(socket.encode())>107 or any(c.isspace() for c in socket) or '\0' in socket or any(part in socket for part in ('/../','/./','//')) or socket.endswith(('/..','/.')):
        parser.error('use a short canonical absolute private Unix socket path')
    identifier=r'[A-Za-z_][A-Za-z0-9_]*'
    if not re.fullmatch(identifier,args.node_name):parser.error('invalid explicit node basename')
    for name in (args.namespace,args.resource):
        if name!='/' and not re.fullmatch('/'+identifier+'(?:/'+identifier+')*',name):parser.error('invalid canonical absolute namespace/resource')
    if args.resource=='/':parser.error('a concrete String topic is required')
    for value in (args.master_uri,args.advertise_host):
        if value is not None and (not value or any(c.isspace() for c in value) or '\0' in value):
            parser.error('connection fields must be nonempty single tokens')
    if args.master_uri is not None:
        match=re.fullmatch(r'http://([^/?#@]+):(\d+)/?',args.master_uri)
        if not match or not 1<=int(match.group(2))<=65535:parser.error('Master URI requires explicit http://host:port')
    if args.advertise_host is not None and any(c in args.advertise_host for c in '/\\'):
        parser.error('advertise host is an explicit host identity, not a path')
    try:result=policy(args)
    except ValueError as error:parser.error(str(error))
    args.out.mkdir(mode=0o700,parents=True,exist_ok=False)
    config=args.out/'gateway.conf';config.write_text(result['Gateway_config_text']);config.chmod(0o600)
    result.update(status='PREPARED_POLICY_NOT_VALIDATED_OR_EXECUTED',
        scope='Only explicitly selected String tutorial grants; no authority applied, no Worker/Gateway or ROS process started',
        Gateway_config=str(config.resolve()),tutorial_limits_require_review=True)
    (args.out/'policy.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],args.out/'policy.json')


if __name__=='__main__':main()
