"""Check a fixed String relay between isolated Fast DDS and Cyclone trust domains."""
import argparse
from contextlib import ExitStack
import json
import os
from pathlib import Path
import resource
import secrets
import signal
import socket
import subprocess
import sys
import tempfile
import time

HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(HERE.parent/'sros2_cross'))
import run as cross
import wire
sys.path.insert(0,str(HERE))
from credentials import issue_dds,issue_tls
from tls_records import Transport

SIDES={'fast':('request',74),'cyclone':('echo',75)}
CASES=('missing','untrusted-client','untrusted-server','connected')
UNTRUSTED={'untrusted-client':'cyclone','untrusted-server':'fast'}


def cleanup(proc):
    if proc.poll() is None:
        os.killpg(proc.pid,signal.SIGKILL)
        proc.wait()


def message_pair(challenge,response):
    return {'request':'Q'+challenge,'reply':'A'+challenge+response}


def leg_worker(side,case,challenge,response):
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    cross.check(socket.if_nameindex()==[(1,'lo')],'unexpected network interface')
    spec=json.loads(Path('/spec.json').read_bytes())
    app_role,domain=SIDES[side]
    roles=['app'] if case=='missing' else ['app','bridge']
    logs=Path('/evidence')
    ports={r:set() for r in roles}
    mappings={r:set() for r in roles}
    markers=message_pair(challenge,response)
    expected=set(spec['libraries']) | set(spec['securityPlugins'])
    status={}
    with ExitStack() as stack:
        processes={}
        for role in roles:
            args=['/native',app_role if role=='app' else side+'-bridge',spec['rmw'],
                  challenge if role=='app' else '-',response if role=='app' else '-','--ros-args','--enclave','/'+role]
            env=cross.environment(spec,True)
            env.update(ROS_DOMAIN_ID=str(domain),ROS_SECURITY_KEYSTORE='/authority')
            env['FASTDDS_DEFAULT_PROFILES_FILE']='/loopback.xml'
            env['FASTRTPS_DEFAULT_PROFILES_FILE']='/loopback.xml'
            readable=['/usr','/dev','/proc/self','/proc/sys','/proc/net','/tmp','/native','/loopback.xml',
                      '/authority/enclaves/'+role,spec['prefix'],spec['system'],*spec['externalTargets']]
            if role=='bridge':
                readable.append('/tls')
            policy=['/usr/bin/setpriv','--no-new-privs','--landlock-access','fs:read-file']
            for path in readable:
                policy+=['--landlock-rule','path-beneath:read-file:'+path]
            cmd=wire.trace_command(logs,role,[*policy,*args])
            log=stack.enter_context((logs/(role+'.log')).open('wb'))
            proc=subprocess.Popen(cmd,env=env,stdin=0 if role=='bridge' else subprocess.DEVNULL,
                                  stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
            stack.callback(cleanup,proc)
            processes[role]=proc
        deadline=time.monotonic()+23
        while time.monotonic()<deadline and any(p.poll() is None for p in processes.values()):
            for r,p in processes.items():
                pid=wire.app_pid(p)
                if not pid: continue
                ports[r].update(wire.receive_ports(pid,domain))
                try: lines=Path('/proc/'+pid+'/maps').read_text().splitlines()
                except (FileNotFoundError,PermissionError,ProcessLookupError): continue
                mappings[r].update(v[5] for l in lines if len(v:=l.split(None,5))==6 and 'x' in v[1] and v[5] in expected)
            time.sleep(0.04)
        status={r:p.returncode if p.poll() is not None else 'timeout' for r,p in processes.items()}
    capture=wire.summarize(logs,roles,markers,domain)
    reads={r:(logs/(r+'.log')).read_text(errors='replace') for r in roles}
    confirmations={r:[line for line in txt.splitlines() if ' received ' in line] for r,txt in reads.items()}
    expected_msg=markers['reply' if side=='fast' else 'request']
    matched=(app_role+' received '+expected_msg) in confirmations['app']
    paired={r:sum(row['count'] for row in capture['userPortEncryptedSubmessages']
                  if row['sender']==r and row['destination'] in ports[other])
            for r,other in [('app','bridge'),('bridge','app')] if other in ports and r in ports}
    tls=[line for line in reads.get('bridge','').splitlines() if line.startswith('verified TLSv1.3 ')]
    no_clear=not any(n for counts in capture['clearMarkers'].values() for n in counts.values())
    ok=no_clear and mappings['app']==expected and 'DDS ready' in reads['app'].splitlines()
    if case=='connected':
        peer='cyclone-bridge.test' if side=='fast' else 'fast-bridge.test'
        bridge_msg=markers['request' if side=='fast' else 'reply']
        bridge_received=side+'-bridge received '+bridge_msg
        ok=bool(ok and matched and bridge_received in confirmations['bridge'] and all(v==0 for v in status.values())
                and mappings['bridge']==expected and len(ports['app'])==len(ports['bridge'])==1
                and ports['app'].isdisjoint(ports['bridge']) and all(paired.values())
                and len(tls)==1 and tls[0].endswith('peer='+peer))
    else:
        ok=ok and status['app']==4 and 'no matching sample' in reads['app'].splitlines() and not confirmations['app']
        if case in UNTRUSTED:
            text=reads['bridge']
            ok=bool(ok and status['bridge']==5 and not tls and 'DDS ready' not in text and not confirmations['bridge']
                    and (('TLS handshake failed; verify=' in text and 'certificate verify failed' in text.lower())
                         if side!=UNTRUSTED[case] else ('TLS read failed' in text or 'TLS handshake refused' in text)))
    leg={'passed':ok,'side':side,'domain':domain,'mode':case,'processExit':status,'applicationMatched':matched,
         'confirmations':confirmations,'tls':tls,'userPorts':{r:sorted(v) for r,v in ports.items()},
         'encryptedToLocalPeer':paired,'nativeMappings':{r:sorted(v) for r,v in mappings.items()},
         'diagnostics':{r:{'rosWarnings':text.count('[WARN]'),'rosErrors':text.count('[ERROR]'),
                   'rcutilsReports':text.count('rcutils_set_error_state()'),
                   'secureParticipantNotAllowed':sum('Remote secure participant' in line and 'not allowed' in line
                                                      for line in text.splitlines())} for r,text in reads.items()},
         'logSha256':{r:cross.sha(logs/(r+'.log')) for r in roles},'udp':capture}
    cross.save(logs/'leg.json',leg)
    return int(not ok)


def native_specs(baseline):
    base=json.loads(baseline.read_bytes())
    inputs=baseline.parent/'inputs.json'
    cross.check(base['complete'] and base['inputsSha256']==cross.sha(inputs) and
                base['cases']['cycloneControlEnforce']['passed'],'expected a completed original Cyclone control')
    inp=json.loads(inputs.read_bytes()); result={}
    for side,role in [('fast','request'),('cyclone','echo')]:
        spec=inp['peers'][role]
        cross.check((spec['release'],spec['rmw'])==(('jazzy','rmw_fastrtps_cpp') if side=='fast' else
                    ('lyrical','rmw_cyclonedds_cpp')),'wrong native peer')
        checks={**spec['libraries'],**spec['securityPlugins']}
        cross.check(all(cross.sha(path)==h for path,h in checks.items()),'native input identity changed')
        if side=='cyclone':
            cross.check(len(spec['securityPlugins'])==3 and bool(spec['signedArchive']['members']),'missing signed plugin bindings')
        system=Path(spec['system'])
        result[side]={**spec,'externalTargets':sorted({str(p.resolve()) for p in system.iterdir() if p.is_symlink()
                              and p.is_file() and not p.resolve().is_relative_to(system)})}
    return result


def launch_leg(side,case,spec,out,stage,challenge,response,stream,log):
    case_dir=out/case/side
    case_dir.mkdir(parents=True)
    suite=HERE.parent
    cmd=['/usr/bin/bwrap','--die-with-parent','--new-session','--unshare-net','--unshare-pid','--unshare-ipc',
         '--cap-drop','ALL','--ro-bind','/usr','/usr','--symlink','usr/bin','/bin','--symlink','usr/lib','/lib',
         '--symlink','usr/lib64','/lib64','--proc','/proc','--dev','/dev','--tmpfs','/tmp',
         '--ro-bind',HERE,'/suite/sros2_tls_relay','--ro-bind',suite/'sros2_cross','/suite/sros2_cross',
         '--ro-bind',out/(side+'.bin'),'/native','--ro-bind',out/(side+'.json'),'/spec.json',
         '--ro-bind',out/'loopback.xml','/loopback.xml','--ro-bind',stage/side/'runtime','/authority',
         '--ro-bind',stage/'tls'/('untrusted-'+side if UNTRUSTED.get(case)==side else side),'/tls',
         '--bind',case_dir,'/evidence','--chdir','/tmp']
    for path in sorted({spec['prefix'],spec['system'],*spec['externalTargets']}):
        cmd+=['--ro-bind',path,path]
    cmd+=['--clearenv','--setenv','PATH','/usr/bin:/bin','/usr/bin/python3','-I','-B',
          '/suite/sros2_tls_relay/run.py','--leg',side,case,challenge,response]
    return subprocess.Popen(cmd,stdin=stream,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)


def main():
    sys.path.insert(0,str(HERE.parent))
    from run_scene_vm import verify_gate
    verify_gate()
    cross.check(os.sched_getaffinity(0).issubset({4,5,6,7}),'unexpected qualification CPUs')
    p=argparse.ArgumentParser(description=__doc__)
    for n in ('baseline','issuer-rootfs','output'):
        p.add_argument('--'+n,type=Path,required=True)
    args=p.parse_args()
    out=args.output.resolve(); out.mkdir(mode=0o700,exist_ok=False,parents=True)
    baseline=args.baseline.resolve(strict=True)
    specs=native_specs(baseline)
    profile=baseline.parent/'loopback.xml'
    previous=json.loads((baseline.parent/'inputs.json').read_bytes())
    cross.check(cross.sha(profile)==previous['loopbackProfileSha256'],'changed loopback profile')
    (out/'loopback.xml').write_bytes(profile.read_bytes())
    result={'format':'uwvm-sros2-tls13-single-string-relay-v1','complete':False,'passed':False,
            'baselineSha256':cross.sha(baseline),'dds':{},'cases':{},
            'sourceSha256':{str(p.relative_to(HERE.parent)):cross.sha(p) for p in [
                HERE/'peer.cpp',HERE/'run.py',HERE/'credentials.py',HERE/'tls_records.py',
                HERE.parent/'sros2_cross/run.py',HERE.parent/'sros2_cross/wire.py']}}
    with tempfile.TemporaryDirectory(dir='/dev/shm',prefix='sros2-relay-') as tmp:
        stage=Path(tmp)
        result['tlsCredentials']=issue_tls(cross,stage/'tls')
        for side,spec in specs.items():
            certs=issue_dds(cross,args.issuer_rootfs.resolve(strict=True),stage/side,out/(side+'-policy'),SIDES[side][1])
            build=cross.build(Path(spec['prefix']),Path(spec['system']),out/(side+'.bin'),HERE/'peer.cpp',('ssl','crypto'))
            cross.save(out/(side+'.json'),spec)
            result['dds'][side]={'security':certs,'specSha256':cross.sha(out/(side+'.json')),
                                 'binarySha256':cross.sha(out/(side+'.bin')),'build':build}
        for ca in ('identity_ca.cert.pem','permissions_ca.cert.pem'):
            cross.check(result['dds']['fast']['security']['enclaves']['app'][ca]!=
                        result['dds']['cyclone']['security']['enclaves']['app'][ca],'overlapping DDS trust roots')
        cross.save(out/'results.json',result)
        for case in CASES:
            challenge,response=secrets.token_hex(16),secrets.token_hex(16)
            with ExitStack() as stack:
                transport=Transport(message_pair(challenge,response)) if case!='missing' else None
                pair=transport.start() if transport else [None,None]
                if transport:stack.callback(transport.finish)
                procs={}
                for side,sock in zip(SIDES,pair):
                    log=stack.enter_context((out/(case+'-'+side+'.log')).open('wb'))
                    procs[side]=launch_leg(side,case,specs[side],out,stage,challenge,response,
                                           sock if case!='missing' else subprocess.DEVNULL,log)
                    stack.callback(cleanup,procs[side])
                for s in pair:
                    if s:s.close()
                status={}
                for side,proc in procs.items():
                    try:status[side]=proc.wait(timeout=38)
                    except subprocess.TimeoutExpired:status[side]='timeout'
            legs={}
            for side in SIDES:
                path=out/case/side/'leg.json'
                legs[side]={'sha256':cross.sha(path),'passed':json.loads(path.read_bytes())['passed']} if path.is_file() else {'passed':False}
            tls_wire=transport.receipts if transport else {}
            wire_ok=(not transport or (len(tls_wire)==2 and all(row['bytes']>0 and row['completeRecords']
                     and row['records'].get('22',0)>0 and (row['records'].get('23',0)>0 or
                         (case in UNTRUSTED and row['records'].get('21',0)>0))
                     and not any(row['clearMarkers'].values())
                     and (case in UNTRUSTED or row['error'] is None) for row in tls_wire.values())))
            result['cases'][case]={'passed':all(leg['passed'] for leg in legs.values()) and
                                   all(v==0 for v in status.values()) and wire_ok,'tlsWire':tls_wire,
                                  'challenge':challenge,'response':response,'legs':legs,'workerExit':status}
            cross.save(out/'results.json',result)
    result.update(complete=True,passed=all(row['passed'] for row in result['cases'].values()))
    cross.save(out/'results.json',result)
    return int(not result['passed'])


if __name__=='__main__':
    sys.exit(leg_worker(*sys.argv[2:]) if sys.argv[1:2]==['--leg'] else main())
