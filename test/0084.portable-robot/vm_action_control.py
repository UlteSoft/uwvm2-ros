"""Real Wasm32/64 Action imports against an exact-PID synthetic IPC peer.

Run only in the admitted shared 64 GiB, zero-swap queue. No native ROS server,
official simulator, robot, or production enrollment is claimed by this test.
"""
from pathlib import Path
import hashlib
import importlib.util
import json
import os
import socket
import struct
import subprocess
import sys
import time

HEADER = struct.Struct('<IHHB3xQQQIII')
UUID = bytes(range(1, 17))
STAMP = struct.pack('<iI', -1, 999999999)
ACCEPTED = b'\1' + struct.pack('<Q', 42) + b'\1' + UUID + STAMP + b'\2'
STATUS = b'\1' + struct.pack('<Q', 42) + UUID + STAMP + b'\2'
CANCEL = b'\1' + struct.pack('<Q', 42) + b'\0' + struct.pack('<I', 1) + UUID + STAMP


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def write(path, value):
    path.write_text(json.dumps(value, indent=2) + '\n')


def receive(connection, size):
    data = b''
    while len(data) < size:
        part = connection.recv(size - len(data))
        if not part:
            if not data:
                return None
            raise EOFError('partial checked IPC frame')
        data += part
    return data


def peer(folder, address):
    folder = Path(folder)
    transcript = []
    counts = {}
    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as listener:
        listener.bind(address)
        os.chmod(address, 0o600)
        listener.listen(2)
        listener.settimeout(60)
        write(folder / 'ready.json', {'pid': os.getpid(), 'scope': 'synthetic IPC peer only'})
        for ordinal in range(1, 5):
            connection, _ = listener.accept()
            with connection:
                connection.settimeout(15)
                credentials = struct.unpack('3i', connection.getsockopt(socket.SOL_SOCKET, socket.SO_PEERCRED, 12))
                deadline = time.monotonic() + 5
                while not (folder / 'expected.json').exists():
                    if time.monotonic() > deadline:
                        raise TimeoutError('owned VM birth publication')
                    time.sleep(.001)
                expected = json.loads((folder / 'expected.json').read_text())
                stat = Path('/proc', str(credentials[0]), 'stat').read_text().rsplit(') ', 1)[1].split()
                assert credentials[1] == os.getuid() and int(stat[2]) == expected['pgrp']
                assert sha(Path('/proc', str(credentials[0]), 'exe')) == expected['sha256']
                session = 0x701000 + ordinal
                previous = 0
                while True:
                    raw = receive(connection, 48)
                    if raw is None:
                        break
                    magic, version, kind, backend, request, response, selected, size, error, timeout = HEADER.unpack(raw)
                    assert (magic, version, backend) == (0x32524f55, 1, 2)
                    assert request > previous and not response and not error and size <= 65536
                    assert 0 < timeout <= 60000
                    previous = request
                    body = receive(connection, size) if size else b''
                    operation, alias, data, reply, code = 'hello', '', b'', b'', 0
                    if kind == 1:
                        assert not selected and not body
                    else:
                        assert kind == 2 and selected == session and len(body) >= 8
                        n, a, d = struct.unpack_from('<HHI', body)
                        assert 8+n+a+d == len(body)
                        operation = body[8:8+n].decode()
                        alias = body[8+n:8+n+a].decode()
                        data = body[8+n+a:]
                        counts[operation] = counts.get(operation, 0) + 1
                        count = counts[operation]
                        if operation == 'session.bind':
                            assert alias == 'target' and data.startswith(b'UWRP\1')
                        elif operation in ('capability.close', 'capability.revoke'):
                            assert alias in ('send', 'cancel') and not data
                        elif operation == 'action.server-is-ready':
                            assert alias == 'send' and not data
                            reply = b'\1' if count == 1 else b'\2'
                        elif operation == 'action.send-begin':
                            assert alias == 'send' and data == struct.pack('<i', 3)
                            reply = struct.pack('<Q', 42)
                        elif operation == 'action.goal-response':
                            assert alias == 'send' and data == struct.pack('<Q', 42)
                            if count == 1:
                                code = 11
                            else:
                                reply = ACCEPTED
                        elif operation == 'action.goal-status':
                            assert alias == 'send' and data == struct.pack('<Q', 42)
                            reply = STATUS
                            if count == 3:
                                reply = STATUS[:-1]
                            elif count == 4:
                                reply = STATUS[:1] + struct.pack('<Q', 43) + STATUS[9:]
                            elif count == 5:
                                code, reply = 110, b''
                        elif operation == 'action.cancel-begin-own':
                            assert alias == 'cancel' and data == struct.pack('<Q', 42)
                            reply = data
                        elif operation == 'action.cancel-response':
                            assert alias == 'cancel' and data == struct.pack('<Q', 42)
                            if count == 1:
                                code = 11
                            elif count == 2:
                                reply = CANCEL
                            else:
                                reply = CANCEL[:10] + struct.pack('<I', 2) + (UUID + STAMP)*2
                        else:
                            raise AssertionError('unexpected native operation '+operation)
                    response_header = HEADER.pack(magic, version, 3, backend, 0, request, session, len(reply), code, timeout)
                    transcript.append({'connection': ordinal, 'operation': operation, 'alias': alias,
                        'request_id': request, 'session': session, 'peer_credentials': credentials,
                        'request_header_hex': raw.hex(), 'request_body_hex': body.hex(),
                        'input_hex': data.hex(), 'output_hex': reply.hex(), 'error': code,
                        'response_header_hex': response_header.hex()})
                    write(folder / 'transcript.json', transcript)
                    connection.sendall(response_header + reply)
                    if operation == 'capability.revoke':
                        return


GUEST = r'''
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned char u8;
typedef __UINTPTR_TYPE__ uptr;
#define IMP(n) __attribute__((import_module("uwvm_ros2_v1"),import_name(#n)))
#define RPC(n) IMP(n) u32 n(u64,u32,u64,u64,u64,u64,u64);
IMP(capability_open) u32 capability_open(u32,u64,u64,u64);
IMP(capability_close) u32 capability_close(u64);
IMP(capability_revoke) u32 capability_revoke(u64);
RPC(action_server_is_ready)
RPC(action_send_begin)
RPC(action_goal_response)
RPC(action_goal_status)
RPC(action_cancel_begin_own)
RPC(action_cancel_response)
__attribute__((import_module("wasi_snapshot_preview1"),import_name("proc_exit"),noreturn)) void quit(u32);
static u64 send, cancel, used, goal=42;
static u8 output[128];
static u32 order=3;
static u32 step;
static void check(int ok){++step;if(!ok)quit(step);}
#define P(p) ((u64)(uptr)(p))
#define CALL(n,h,p,l,c) n(h,0,P(p),l,P(output),c,P(&used))
static void reset(void){used=0x12345678;for(unsigned i=0;i<128;++i)output[i]=0x5a;}
static void unchanged(void){check(used==0x12345678);for(unsigned i=0;i<128;++i)if(output[i]!=0x5a)quit(240);}
static u64 read64(u8 const*p){u64 r=0;for(unsigned i=0;i<8;++i)r|=(u64)p[i]<<(i*8);return r;}
void _start(void){
 check(capability_open(0,P("missing"),7,P(&send))==13);
 check(capability_open(0,P("send"),4,P(&send))==0);
 check(capability_open(0,P("cancel"),6,P(&cancel))==0);
 reset();check(action_server_is_ready(send,1,0,0,P(output),128,P(&used))==22);unchanged();
 check(CALL(action_server_is_ready,send+12345,0,0,128)==9);
 check(action_server_is_ready(send,0,0,0,P(output),128,P(output))==22);unchanged();
 check(action_server_is_ready(send,0,0,0,0xfffffffffffffff0ULL,128,P(&used))==22);unchanged();
 check(CALL(action_cancel_begin_own,send,&goal,8,128)==13);unchanged();
 check(CALL(action_goal_response,send,&goal,7,128)==22);unchanged();
 check(CALL(action_server_is_ready,send,&goal,8,128)==22);unchanged();
 order=6;check(CALL(action_send_begin,send,&order,4,128)==13);unchanged();order=3;
 check(CALL(action_server_is_ready,send,0,0,128)==0);check(used==1&&output[0]==1);
 check(CALL(action_send_begin,send,&order,4,128)==0);check(used==8&&read64(output)==42);
 reset();check(CALL(action_goal_response,send,&goal,8,128)==11);unchanged();
 check(CALL(action_goal_response,send,&goal,8,128)==0);check(used==35&&output[0]==1&&output[9]==1&&read64(output+1)==42);
 for(unsigned i=0;i<16;++i)check(output[10+i]==i+1);
 check(output[26]==255&&output[34]==2);
 check(CALL(action_goal_status,send,&goal,8,128)==0);check(used==34&&output[33]==2);
 check(CALL(action_cancel_begin_own,cancel,&goal,8,128)==0);check(used==8&&read64(output)==42);
 reset();check(CALL(action_cancel_response,cancel,&goal,8,128)==11);unchanged();
 check(CALL(action_cancel_response,cancel,&goal,8,128)==0);check(used==38&&output[9]==0&&output[10]==1);
 for(unsigned i=0;i<16;++i)check(output[14+i]==i+1);
 reset();check(CALL(action_goal_status,send,&goal,8,4)==90);check(used==34);for(unsigned i=0;i<128;++i)check(output[i]==0x5a);
 reset();check(CALL(action_goal_status,send,&goal,8,128)==22);unchanged();
 check(CALL(action_goal_status,send,&goal,8,128)==22);unchanged();
 check(CALL(action_goal_status,send,&goal,8,128)==110);unchanged();
 check(CALL(action_cancel_response,cancel,&goal,8,128)==22);unchanged();
 check(CALL(action_server_is_ready,send,0,0,128)==22);unchanged();
 check(capability_close(cancel)==0);check(CALL(action_cancel_begin_own,cancel,&goal,8,128)==9);
 // Revoke invalidates the live handle; stale-handle calls are EBADF (9).
 check(capability_revoke(send)==0);check(CALL(action_server_is_ready,send,0,0,128)==9);
 quit(0);
}
'''


def main():
    vm, sdk, out = Path(sys.argv[1]), Path(sys.argv[3]), Path(os.environ['UWVM_SDK_OUTPUT'])
    assert sha(vm) == sys.argv[2]
    assert '/uwvmros.slice/' in Path('/proc/self/cgroup').read_text()
    cg = Path('/sys/fs/cgroup/uwvmros.slice')
    assert (cg/'memory.max').read_text().strip() == '68719476736'
    assert (cg/'memory.swap.max').read_text().strip() == '0'
    sys.path.insert(0, str(sdk/'tools/shared/python'))
    from owned_processes import OwnedProcesses, process
    spec = importlib.util.spec_from_file_location('regression_deployment', sdk/'tools/platform/ros/deployment.py')
    deployment = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(deployment)
    common = {'fibonacci.max-order':'5','max-active-goals':'1','max-actions':'1','timeout-ms':'8000',
        'max-goal-bytes':'4','max-feedback-bytes':'64','max-result-bytes':'64','max-rate-hz':'1000',
        'burst':'128','max-bytes-per-second':'65536','max-event-queue-depth':'16',
        'max-event-queue-bytes':'8192','overflow-policy':'reject','validator':'none','audit':'allow-and-deny',
        'audit-max-rate-hz':'1000','audit-burst':'256','audit-max-records':'512','audit-max-bytes':'524288'}
    qos = {'reliability':'reliable','durability':'volatile','history':'keep-last','depth':'16',
        'deadline':'infinite','lifespan':'infinite','liveliness':'automatic','liveliness-lease-duration':'infinite'}
    for channel in ['goal-service','cancel-service','result-service','feedback','status']:
        common.update({'qos.'+channel+'.'+key:value for key,value in qos.items()})
    grants = [{'alias':alias,'operation':op,'resource':'/action_regression/fibonacci',
        'type':'action_tutorials_interfaces/action/Fibonacci',
        'identity':'RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0',
        'limits':dict(common,**{'cancel-scope':scope})}
        for alias,op,scope in [('send','action.send-goal','not-applicable'),('cancel','action.cancel-own-goal','own')]]
    report = {'scope':'actual LLVM-AOT Wasm imports; synthetic protocol peer, NOT native ROS/official simulation',
        'vm_sha256':sha(vm),'cases':[],'status':'RUNNING'}
    write(out/'report.json',report)
    for width in (32,64):
        folder=out/('wasm'+str(width));folder.mkdir()
        source=folder/'guest.c';source.write_text(GUEST)
        wasm=folder/'guest.wasm'
        compile_argv=['/usr/bin/clang-22','--target=wasm'+str(width)+'-unknown-unknown','-O1','-nostdlib',
            '-fno-builtin','-fuse-ld=/usr/lib/llvm-21/bin/wasm-ld','-Wl,--no-entry','-Wl,--export=_start',
            '-Wl,--initial-memory=131072',str(source),'-o',str(wasm)]
        with (folder/'compile.stdout').open('xb') as stdout,(folder/'compile.stderr').open('xb') as stderr:
            subprocess.run(compile_argv,stdout=stdout,stderr=stderr,check=True)
        config={'format':'uwvm-native-ros-run-v2','runtime':str(vm),'gateway':'synthetic-regression-peer',
            'backend':'ros2','distribution':'humble','connection':{'node-name':'action_regression','namespace':'/',
            'domain-id':os.environ['ROS_DOMAIN_ID'],'rmw-implementation':'rmw_cyclonedds_cpp',
            'security-mode':'disabled-test','keystore':'not-applicable','enclave':'not-applicable'},
            'examples':{'fixture':{'arguments':[],'main_module':'action_repro','preloads':[],
                'modules':{'action_repro':{'ros_enabled':True,'grants':grants,'wasi_grants':[]}}}}}
        write(folder/'deployment.json',config)
        plan=deployment.prepare(config,'fixture',wasm,folder,'wasm'+str(width)+'-wasip1','ros2','humble')
        ledger=OwnedProcesses();roots=[];audit_fd=None
        directory_fd=os.open(folder,os.O_RDONLY|os.O_DIRECTORY|os.O_CLOEXEC)
        address=f'/proc/{os.getpid()}/fd/{directory_fd}/peer.sock'
        row={'width':width,'compile_argv':compile_argv,'guest_sha256':sha(wasm),'status':'RUNNING'}
        report['cases'].append(row)
        try:
            with (folder/'peer.stdout').open('xb') as stdout,(folder/'peer.stderr').open('xb') as stderr:
                actor=subprocess.Popen([sys.executable,'-B',str(Path(__file__).resolve()),'--peer',str(folder),address],
                    stdout=stdout,stderr=stderr,start_new_session=True)
            roots.append({'pid':actor.pid,'birth':process(actor.pid)['birth'],'process':actor})
            deadline=time.monotonic()+10
            while not (folder/'ready.json').exists():
                if actor.poll() is not None or time.monotonic()>deadline:raise RuntimeError('peer startup failure')
                time.sleep(.01)
            audit_fd=os.open(folder/'journal.jsonl',os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_CLOEXEC,0o600)
            run_id='action-'+os.urandom(16).hex()
            argv=deployment.command(plan,[str(vm),'--ros2-buffer-audit',run_id,str(audit_fd),
                '--runtime-compile-threads','1'],{'action_repro':address},os.getuid())
            row['argv']=argv
            with (folder/'vm.stdout').open('xb') as stdout,(folder/'vm.stderr').open('xb') as stderr:
                guest=subprocess.Popen(argv,stdout=stdout,stderr=stderr,start_new_session=True,pass_fds=(audit_fd,))
            roots.append({'pid':guest.pid,'birth':process(guest.pid)['birth'],'process':guest})
            os.close(audit_fd);audit_fd=None
            write(folder/'expected.json',{'pgrp':guest.pid,'sha256':sha(vm)})
            guest.wait(timeout=120);row['guest_exit']=guest.returncode
            assert guest.returncode==0, ('actual guest check/VM failure',guest.returncode)
            actor.wait(timeout=10);assert actor.returncode==0
            rows=json.loads((folder/'transcript.json').read_text())
            expected={'action.server-is-ready':2,'action.send-begin':1,'action.goal-response':2,
                'action.goal-status':5,'action.cancel-begin-own':1,'action.cancel-response':3,
                'capability.close':1,'capability.revoke':1}
            for op,count in expected.items():assert sum(x['operation']==op for x in rows)==count,(op,rows)
            journal=[json.loads(line) for line in (folder/'journal.jsonl').read_text().splitlines()]
            assert journal[0]['kind']=='begin' and journal[-1]['kind']=='end'
            assert journal[-1]['dropped_records']==0 and journal[-1]['suppressed_records']==0
            assert all(r['run_id']==run_id for r in journal)
            operations=[r for r in journal if r['kind']=='operation']
            matched=0
            for r in operations:
                assert r['module_sha256']==sha(wasm) and r['module']=='action_repro'
                for direction in ('input','output'):
                    raw=bytes.fromhex(r['canonical_'+direction+'_hex'])
                    assert sha_bytes(raw)==r['canonical_'+direction+'_sha256']
                if r['response_checked']:
                    candidates=[p for p in rows if p['peer_credentials'][0]==r['pid'] and p['request_id']==r['request_id']
                        and p['operation']==r['operation'] and p['alias']==r['alias']]
                    assert len(candidates)==1
                    p=candidates[0]
                    tag=b'uwvm-sdk-ros-native-session-tag-v1\0'+struct.pack('<H',len(run_id))+run_id.encode()+bytes([2])+struct.pack('<Q',p['session'])
                    assert sha_bytes(tag)==r['session_tag'] and r['peer_pid']==actor.pid
                    assert r['canonical_input_hex']==p['input_hex']
                    if r['guest_memory_written']:assert r['canonical_output_hex']==p['output_hex']
                    matched+=1
            assert matched==14,(matched,operations)
            short=[r for r in operations if r['error']==90]
            assert len(short)==1 and short[0]['length_slot_written'] and not short[0]['guest_memory_written'] and short[0]['response_size']==34
            assert sum(r['error']==11 for r in operations)==2
            assert sum(r['error']==110 for r in operations)==1
            row.update(status='PASS',checked_joins=matched,journal_records=len(journal),journal_sha256=sha(folder/'journal.jsonl'))
        finally:
            if audit_fd is not None:os.close(audit_fd)
            row['retirement']=ledger.retire(roots,grace_seconds=1,kill_seconds=5)
            os.close(directory_fd)
            write(out/'report.json',report)
            assert row['retirement']['status']=='RETIRED'
    report['status']='PASS_ACTUAL_VM_SYNTHETIC_PEER_ONLY'
    write(out/'report.json',report)


def sha_bytes(data):
    return hashlib.sha256(data).hexdigest()


if __name__=='__main__':
    if len(sys.argv)>1 and sys.argv[1]=='--peer':
        peer(sys.argv[2],sys.argv[3])
    else:
        main()
