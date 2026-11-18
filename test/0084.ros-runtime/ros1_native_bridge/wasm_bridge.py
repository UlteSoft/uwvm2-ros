"""A registered ROSv2 driver carries one fixed ROS String through the native bridge.

The VM has no network imports. Only the test deployer can start ROS processes.
The deployment explicitly opts in to legacy plaintext in a private network.
"""
import argparse
from contextlib import ExitStack
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import select
import socket
import struct
import subprocess
import sys
import time
import xmlrpc.client

from run import CYCLONE, HERE, URI, digest, gate

NAMES = ['driver.' + op for op in ('connect', 'submit', 'result', 'disconnect')]
IDS = [bytes([n]) + bytes(15) for n in range(1, 6)]
ROLES = ('publish', 'take')
MAGIC = 0x44534f52564d5755
TYPE = b'std_msgs/msg/String'
ENCODING = b'scene-canonical'


def require(ok, reason):
    if not ok:
        raise RuntimeError(reason)


def u64(value):
    return struct.pack('<Q', value)


def blob(value):
    return u64(len(value)) + value


def contract(role):
    return hashlib.sha256(b'wros-test:ros1_bridge:/chatter:std_msgs/msg/String:'
                          b'scene-canonical:topic.' + role.encode()).digest()


def publish(token, deadline):
    return u64(1)*3 + blob(TYPE) + blob(ENCODING) + blob(blob(token)) + u64(deadline)


def delivery(token):
    return (u64(1)*3 + u64(0)*5 + blob(b'topic.message') + blob(TYPE) +
            blob(ENCODING) + blob(blob(token)))


def accepted():
    return u64(1) + blob(b'admitted') + blob(b'locally_accepted') + u64(1) + blob(b'none')


def build(args, output):
    src, ros2, ssl = args.vm_source, args.humble_prefix, args.openssl_root
    compiler = '/usr/bin/clang++-22'
    flags = ['-O3', '-g', '-Wall', '-Wextra', '-Werror']
    tls_source = src/'test/0084.ros-runtime/attachment_peer.cpp'
    tls = [compiler, '-std=c++23', *flags, '-shared', '-fPIC', '-pthread',
           '-DUWVM_ROS_ATTACHMENT_OPENSSL', '-I'+str(ssl/'include'), '-I'+str(src/'src'),
           '-isystem', src/'third-parties/fast_io/include', tls_source, ssl/'libssl.a',
           ssl/'libcrypto.a', '-ldl', '-o', output/'tls.so']
    includes = [ros2/'include', *[p for p in sorted((ros2/'include').iterdir()) if (p/p.name).is_dir()]]
    libs = ['rclcpp','rcl','rcutils','rmw','rmw_implementation','std_msgs__rosidl_typesupport_cpp',
            'libstatistics_collector','tracetools','rcl_interfaces__rosidl_typesupport_cpp','rcpputils',
            'statistics_msgs__rosidl_typesupport_cpp','rosgraph_msgs__rosidl_typesupport_cpp',
            'builtin_interfaces__rosidl_typesupport_cpp']
    ros = [compiler, '-std=c++17', *flags, *[arg for p in includes for arg in ('-isystem',p)],
           HERE/'ros2_string_io.cpp', *[arg for p in (ros2/'lib',args.humble_system_libs)
           for arg in ('-L'+str(p),'-Wl,-rpath-link,'+str(p))], *['-l'+n for n in libs], '-o', output/'ros2_io']
    commands = []
    for name, argv in [('tls',tls), ('ros',ros)]:
        with (output/(name+'.build.log')).open('wb') as log:
            subprocess.run(argv, check=True, timeout=120, stdout=log, stderr=subprocess.STDOUT)
        commands.append(list(map(str,argv)))
    return commands


def endpoint(args):
    from attachment_peer import SecurePeer
    role, fd, key = args.role, args.fd, args.key
    binding = b''.join(IDS) + contract(role)
    peer = SecurePeer(args.output/'tls.so', fd, key, int(sys.stdin.readline()), 1, binding, 176+512)
    requests = 0
    active = False
    header = struct.Struct('<22Q')
    try:
        while True:
            packet = peer.recv()
            fields = list(header.unpack_from(packet))
            body = packet[header.size:]
            require(fields[:2] == [MAGIC,176] and fields[3] == 0 and packet[32:144] == binding
                    and fields[20:] == [len(body),0], 'registration, extent or status')
            kind = fields[2]
            if kind in (1,5):
                require(not any(fields[18:]) and active == (kind == 5), 'control state')
                if kind == 5:
                    break
                active = True
                fields[2] = 2
                peer.sendall(header.pack(*fields))
                continue
            require(active and kind == 3 and requests == 0 and fields[18] == 1, 'one admitted request')
            remaining = (fields[19] - time.monotonic_ns()) / 1e9
            require(0 < remaining <= 30, 'bounded submission deadline')
            requests += 1
            if role == 'publish':
                # Check canonical nested lengths, endpoint, request identity and deadline.
                token = body[-40:-8]
                require(re.fullmatch(b'[a-f0-9]{32}', token) and body == publish(token, fields[19]),
                        'fixed publish request and type')
            else:
                require(body == u64(1), 'fixed take endpoint')
                token = b''
            # This process is a test deployment-owned adapter. DDS is unreachable
            # from the guest and this fixture creates no ROS endpoint before submit.
            require(os.write(args.io_request,b'S'+token) == 1+len(token), 'bounded adapter request')
            ready,_,_ = select.select([args.io_response],[],[],min(remaining,7))
            require(ready,'DDS adapter timeout')
            answer = os.read(args.io_response,64)
            if role == 'take':
                require(re.fullmatch(b'[a-f0-9]{32}', answer), 'one typed DDS sample')
                result = delivery(answer)
            else:
                require(answer == b'P', 'one typed DDS write')
                result = accepted()
            require(time.monotonic_ns() < fields[19], 'late result')
            fields[2], fields[20] = 4, len(result)
            peer.sendall(header.pack(*fields)+result)
    finally:
        peer.close()
    print('authenticatedRequests='+str(requests),flush=True)


def guest(role, token, until, denied):
    from run_module_security_vm import Fixture
    g = Fixture(NAMES)
    cap = g.open('chatter')
    def poll(name, body=b'', expected=0):
        address = g.blob(body)
        label = 'wait'+str(len(g.extra_functions))
        g.extra_functions.append(f'''(func ${label} (result i32) (local $status i32)
            (local $count i32) block $done loop $wait
            {cap} i32.const 0 i64.const {address} i64.const {len(body)}
            i64.const 16384 i64.const 1024 i64.const 1024 call ${name}
            local.tee $status i32.const 11 i32.ne br_if $done
            local.get $count i32.const 1 i32.add local.tee $count
            i32.const 20000000 i32.gt_u if unreachable end br $wait end end local.get $status)''')
        g.check('call $'+label,expected)
    poll('driver.connect')
    body = publish(token,until) if role == 'publish' else u64(1)
    poll('driver.submit', u64(1)+u64(until)+blob(body), 13 if denied else 0)
    if not denied:
        poll('driver.result',u64(1))
        result = accepted() if role == 'publish' else delivery(token)
        g.equal(g.word(1024),len(result)+16)
        expected = u64(0)+blob(result)
        for index in range(0,len(expected),8):
            chunk = expected[index:index+8]
            if len(chunk)==8:
                g.equal(g.word(16384+index),int.from_bytes(chunk,'little'))
            else:
                for pos,value in enumerate(chunk):
                    g.check(f'i32.const {16384+index+pos} i32.load8_u $work',value)
    poll('driver.disconnect')
    return g.render()


def compile_sdk_guest(args, output, role, denied, token, deadline):
    """Compile the case with the site's signed release and preserve its Wasm receipt."""
    payload = publish(token, deadline) if role == 'publish' else u64(1)
    result = accepted() if role == 'publish' else delivery(token)
    header = output/'case_values.h'
    rows = []
    for name, contents in [('submit_request', u64(1)+u64(deadline)+blob(payload)),
                           ('expected_reply', u64(0)+blob(result))]:
        rows.append('static const uint8_t '+name+'[] = {'+', '.join(map(str,contents))+'};')
    rows.append('enum { expected_submit = '+str(13 if denied else 0)+' };')
    header.write_text('\n'.join(rows)+'\n')
    wasm = output/'application.wasm'
    env = dict(os.environ, TMPDIR=str(output), PATH='/usr/bin:/bin')
    command = [args.sdk/'bin/wrc', '-O3', '-g', '--reactor', '--export', '_start', '--build-receipt',
               '-I'+str(output), HERE/'driver_guest.c', '-o', wasm]
    with (output/'sdk.log').open('wb') as log:
        subprocess.run(command, env=env, cwd=output, stdout=log, stderr=subprocess.STDOUT,
                       check=True, timeout=40)
    receipt = Path(str(wasm)+'.wros-build.json')
    build_data = json.loads(receipt.read_bytes())
    require(build_data['mode']=='strict' and build_data['profile']['mte']
            and build_data['vm_sha256']==digest(args.runtime), 'signed SDK build configuration')
    sys.path.insert(0,str(args.sdk/'install'))
    from imports import inspect
    imports = inspect(wasm.read_bytes())
    expected = {'capability.open','driver.connect','driver.submit','driver.disconnect'}
    if not denied:
        expected.add('driver.result')
    require(all(i['kind']==0 and i['module']=='uwvm_ros_v2' for i in imports)
            and {i['name'] for i in imports}==expected, 'unexpected guest authority surface')
    return digest(receipt)


def case(args):
    from attachment_peer import issue
    names = [s.split(':')[0].strip() for s in Path('/proc/net/dev').read_text().splitlines()[2:]]
    require(names == ['lo'] and args.allow_plaintext_loopback,'explicit private legacy network')
    role, engine, denied = args.role, args.engine, args.denied
    output = args.output/(role+'-'+engine+('-denied' if denied else '-granted'))
    if args.sdk:
        prepared = json.loads((output/'sdk-case.json').read_bytes())
        require(digest(output/'application.wasm')==prepared['wasm'], 'SDK guest changed after build')
        token = prepared['token']
    else:
        output.mkdir()
        token = secrets.token_hex(16)
    focal = args.noetic_root/'usr'
    noetic = args.noetic_root/'opt/ros/noetic'
    ros2 = args.humble_prefix
    focal_lib = focal/'lib/x86_64-linux-gnu'
    ros1_args = [focal_lib/'ld-2.31.so','--library-path',os.pathsep.join(map(str,(focal_lib,noetic/'lib'))),
                 focal/'bin/python3.8','-B','-s']
    ros1_env = {'PATH':'/usr/bin:/bin','HOME':str(output),'ROS_HOME':str(output),'ROS_MASTER_URI':URI,
                'ROS_IP':'127.0.0.1','ROS_DISTRO':'noetic','ROS_VERSION':'1','ROS_PACKAGE_PATH':str(noetic/'share'),
                'PYTHONHOME':str(focal),'PYTHONPATH':os.pathsep.join(map(str,(noetic/'lib/python3/dist-packages',
                                                                            focal/'lib/python3/dist-packages')))}
    master_program = noetic/'bin/rosmaster'
    witness_program = HERE/'ros1_witness.py'
    if args.historical_distro:
        release = args.historical_distro
        distro = args.historical_packages/release
        prefix = distro/'opt/ros'/release
        pyroot = args.historical_packages/'python2/usr'
        site = prefix/'lib/python2.7/dist-packages'
        master_program = prefix/'bin/rosmaster'
        witness_program = HERE/'historical_witness.py'
        ros1_args = [pyroot/'bin/python2.7','-B','-S']
        ros1_env.update(ROS_DISTRO=release,PYTHONHOME=str(pyroot),ROS_PACKAGE_PATH=str(prefix/'share'),
                        WROS_ROS1_PYTHON_PREFIX=str(site),PYTHONPATH=os.pathsep.join(map(str,(
                            site,distro/'usr/lib/python2.7/dist-packages',pyroot/'lib/python2.7/dist-packages'))))
    ros_env = {'PATH':'/usr/bin:/bin','HOME':str(output),'ROS_MASTER_URI':URI,'ROS_IP':'127.0.0.1',
               'ROS_DOMAIN_ID':'61','AMENT_PREFIX_PATH':str(ros2),'RMW_IMPLEMENTATION':'rmw_cyclonedds_cpp',
               'ROS_DISTRO':'humble','ROS_HOME':str(output),'ROS_LOG_DIR':str(output/'ros-log'),'CYCLONEDDS_URI':CYCLONE,
               'LD_LIBRARY_PATH':os.pathsep.join(map(str,(ros2/'lib',ros2/'lib/x86_64-linux-gnu',args.humble_system_libs,
                                                         noetic/'lib',args.native_dir/'focal-dependencies')))}
    def stop(process):
        if process.poll() is None:
            process.terminate()
            try:
                process.wait(timeout=2)
            except subprocess.TimeoutExpired:
                process.kill()
                process.wait()
    with ExitStack() as owned:
        def start(name,argv,**options):
            log = owned.enter_context((output/(name+'.log')).open('wb'))
            options.setdefault('stdout',log)
            proc = subprocess.Popen(list(map(str,argv)),stderr=log,**options)
            owned.callback(stop,proc)
            return proc
        if not denied:
            master = start('master',[*ros1_args,master_program,'--core','-p','11311'],env=ros1_env)
            master_api = xmlrpc.client.ServerProxy(URI)
            for _ in range(80):
                require(master.poll() is None,'master exited')
                try:
                    if master_api.getPid('/wros_bridge_test')[0] == 1:
                        break
                except OSError:
                    time.sleep(.05)
            else:
                raise TimeoutError('ROS master')
            direction = '2_to_1' if role=='publish' else '1_to_2'
            bridge = start('bridge',[args.native_dir/direction],env=ros_env)
            witness = start('noetic',[*ros1_args,witness_program,'receive' if role=='publish' else 'send',
                                      token,'10'],env=ros1_env)
            # ROS master registration alone is earlier than TCPROS connection.
            for _ in range(100):
                require(bridge.poll() is None and witness.poll() is None,'ROS endpoint exited')
                node = master_api.lookupNode('/wros_bridge_test','/wros_ros1_witness')
                if node[0] == 1:
                    reply = xmlrpc.client.ServerProxy(node[2]).getBusInfo('/wros_bridge_test')
                    if reply[0] == 1 and any(len(row)>=6 and row[3]=='TCPROS' and row[4]=='/chatter' and row[5]
                                              for row in reply[2]):
                        break
                time.sleep(.05)
            else:
                raise TimeoutError('Noetic witness TCPROS connection')
        until = prepared['deadline'] if args.sdk else time.monotonic_ns()+20_000_000_000
        wat, wasm = output/'application.wat', output/'application.wasm'
        sdk_receipt = None
        if args.sdk:
            sdk_receipt = prepared['receipt']
            require(sdk_receipt==digest(Path(str(wasm)+'.wros-build.json')), 'SDK receipt changed')
        else:
            wat.write_text(guest(role,token.encode(),until,denied))
            subprocess.run(['wat2wasm','--enable-multi-memory','--debug-names',wat,'-o',wasm],check=True,timeout=5)
        # One deployment-created native client waiting on a bounded local pipe.
        io = start('dds',[args.output/'ros2_io',role],env=ros_env,stdin=subprocess.PIPE,stdout=subprocess.PIPE)
        left, right = socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
        for sock in (left,right):
            owned.callback(sock.close)
            sock.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
        vm_key, peer_key = issue(owned)
        peer = start('adapter',[sys.executable,'-B',__file__,*sys.argv[1:],
                     '--fd',right.fileno(),'--key',peer_key,'--io-request',io.stdin.fileno(),
                     '--io-response',io.stdout.fileno()],stdin=subprocess.PIPE,
                     pass_fds=(right.fileno(),peer_key,io.stdin.fileno(),io.stdout.fileno()))
        right.close()
        if denied or role=='take':
            io.stdin.close()
        io.stdout.close()
        clock_fd=os.open(f'/proc/{peer.pid}/ns/time',os.O_RDONLY|os.O_CLOEXEC)
        owned.callback(os.close,clock_fd)
        flags=['--ros-scene-create','robot','--ros-scene-bind','robot','main']
        for name in ['capability.open',*NAMES]:
            flags+=['--ros-scene-interface','robot',name,'allow']
        grant = [name for name in NAMES if name!='driver.submit' or not denied]
        flags+=['--ros-scene-grant','robot','chatter','driver','bridge-chatter-'+role,'registered-driver',','.join(grant),
                '--ros-scene-driver-budget','robot','1','1048576','--ros-scene-driver-channel','robot','chatter','main',
                str(left.fileno()),str(peer.pid),str(os.getuid()),str(os.getgid()),*[x.hex() for x in IDS],
                contract(role).hex(),'512','512','30000000000',str(until+1000000000),str(vm_key),str(clock_fd),
                '--ros-scene-driver-schema','robot','chatter','topic.'+role,'128','128','16']
        cmd=[args.runtime,'--wasm-set-main-module-name','main','--runtime-'+engine,'--runtime-compile-threads','0',
             '--runtime-llvm-jit-cache-path','disable',*flags,'--run',wasm]
        vm = start('vm',cmd,pass_fds=(left.fileno(),vm_key,clock_fd))
        left.close()
        peer.stdin.write((str(vm.pid)+'\n').encode())
        peer.stdin.close()
        require(vm.wait(timeout=25)==0,'VM guest assertion or CLI failed')
        require(peer.wait(timeout=3)==0,'typed adapter failed')
        requests=0 if denied else 1
        require('authenticatedRequests='+str(requests) in (output/'adapter.log').read_text(),'driver operation count')
        require(('activating DDS endpoint' in (output/'dds.log').read_text()) == (not denied),'DDS activation count')
        if not denied:
            require(witness.wait(timeout=12)==0 and bridge.poll() is None,'Noetic witness or upstream bridge')
            text=(output/'noetic.log').read_text()
            expected=(r'role=receive sent=0 received=1 unexpected=0 value=' if role=='publish'
                      else r'role=send sent=[1-9][0-9]* received=0 unexpected=0 value=')
            require(re.search(expected+token,text) and ('Passing along: ['+token+']') in (output/'bridge.log').read_text(),
                    'ROS 1 witness and upstream bridge failed exact value check')
            if role=='publish':
                # The publisher must remain live until the independent ROS 1
                # subscriber and the upstream bridge have both observed the value.
                require(io.poll() is None,'DDS publisher exited before external delivery confirmation')
                io.stdin.write(b'A')
                io.stdin.close()
        require(io.wait(timeout=3)==0,'DDS endpoint failed')
        observed=dict(role=role,engine=engine,denied=denied,authenticatedRequests=requests,
                      value=None if denied else token,interfaces=names,argv=list(map(str,cmd)),
                      guestSHA256=digest(wasm),sdkBuildReceiptSHA256=sdk_receipt,logs={p.name:digest(p) for p in output.glob('*.log')})
        (output/'observed.json').write_text(json.dumps(observed,indent=2)+'\n')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for arg in ('vm-source','runtime','runtime-receipt','native-dir','noetic-root','humble-prefix',
                'humble-system-libs','openssl-root','output'):
        parser.add_argument('--'+arg,type=lambda value:Path(value).resolve(),required=True)
    parser.add_argument('--sdk',type=lambda value:Path(value).resolve(),
                        help='Compile C guests with this site-configured development installation')
    parser.add_argument('--historical-packages',type=lambda p:Path(p).resolve())
    parser.add_argument('--historical-distro',choices=('melodic','kinetic','lunar','indigo','jade','hydro','groovy'))
    parser.add_argument('--adapter-baseline',type=lambda p:Path(p).resolve(),
                        help='Reuse the two hashed adapter binaries from a successful prior run.')
    parser.add_argument('--allow-plaintext-loopback',action='store_true')
    parser.add_argument('--role',choices=ROLES,help=argparse.SUPPRESS)
    parser.add_argument('--engine',choices=('int','aot'),help=argparse.SUPPRESS)
    parser.add_argument('--denied',action='store_true',help=argparse.SUPPRESS)
    for field in ('fd','key','io-request','io-response'):
        parser.add_argument('--'+field,type=int,help=argparse.SUPPRESS)
    args=parser.parse_args()
    gate()
    require(args.allow_plaintext_loopback and '.tmp' in args.output.parts,
            'explicit --allow-plaintext-loopback and fresh .tmp output directory required')
    require(bool(args.historical_packages) == bool(args.historical_distro),'historical package directory and distro must be paired')
    sys.path.insert(0,str(args.vm_source/'test/0084.ros-runtime'))
    if args.fd is not None:
        endpoint(args)
        return
    if args.role:
        case(args)
        return
    args.output.mkdir(parents=True)
    native=json.loads((args.native_dir/'results.json').read_bytes())
    runtime=json.loads(args.runtime_receipt.read_bytes())
    require(runtime['complete'] and digest(args.runtime)==runtime['vm_sha256'],'runtime receipt mismatch')
    require(native['complete'] and all(digest(args.native_dir/p)==native['executables'][p]
                                        for p in ('1_to_2','2_to_1')), 'native baseline receipt mismatch')
    manifest={'complete':False,'scope':'Wasm registered TLS driver, fixed /chatter String; legacy plaintext local network.',
              'cgroup':Path('/proc/self/cgroup').read_text().strip(),'nativeReceipt':digest(args.native_dir/'results.json'),
              'runtimeSHA256':runtime['vm_sha256'],'cases':[]}
    try:
        if args.historical_distro:
            receipt_path=args.historical_packages/'packages.json'
            packages=json.loads(receipt_path.read_bytes())
            distro=args.historical_distro
            from historical_packages import PYTHON_PACKAGES, ROS_PACKAGES
            # A different release can still be downloading. The two exact
            # package sets needed by this test must already be in the receipt.
            python_names={p['package'] for p in packages['packages'].get('python2',[])}
            ros_names={p['package'] for p in packages['packages'].get(distro,[])}
            required={'ros-'+distro+'-'+name for name in ROS_PACKAGES}
            require(set(PYTHON_PACKAGES)==python_names and required<=ros_names and
                    any(name.startswith('python-rospkg') for name in ros_names) and
                    any(name.startswith('python-catkin-pkg') for name in ros_names),'historical package selection')
            manifest['archiveBatchComplete']=packages['complete']
            def verified(tier):
                metadata=[]
                for package in packages['packages'][tier]:
                    files=package['files']
                    require(len(files)==package['selectedFiles'],'file manifest extent')
                    for relative,expected in files.items():
                        path=Path(relative)
                        require(not path.is_absolute() and '..' not in path.parts,'local package path')
                        require(digest(args.historical_packages/tier/path)==expected,'historical file changed: '+relative)
                    metadata.append({k:v for k,v in package.items() if k!='files'})
                return metadata
            manifest['historicalClient']={'distro':distro,'receipt':digest(receipt_path),
                                          'archive':packages['sources'][distro],'packages':verified(distro),
                                          'python':verified('python2')}
        if args.adapter_baseline:
            baseline=json.loads((args.adapter_baseline/'results.json').read_bytes())
            require(baseline['complete'] and baseline['runtimeSHA256']==runtime['vm_sha256'],
                    'adapter baseline must match VM')
            helper_hashes=[value for path,value in baseline['inputs'].items() if Path(path).name=='ros2_string_io.cpp']
            require(helper_hashes==[digest(HERE/'ros2_string_io.cpp')],'adapter source has changed; build without --adapter-baseline')
            for name in ('tls.so','ros2_io'):
                binary=args.adapter_baseline/name
                require(baseline['inputs'][str(binary)]==digest(binary),'adapter binary digest mismatch')
                (args.output/name).symlink_to(binary)
            manifest['adapterReceipt']=digest(args.adapter_baseline/'results.json')
        else:
            manifest['commands']=build(args,args.output)
        files = [Path(__file__).resolve(),HERE/'ros2_string_io.cpp',HERE/'run.py',HERE/'ros1_witness.py',
                 args.output/'tls.so',args.output/'ros2_io', args.openssl_root/'libssl.a',args.openssl_root/'libcrypto.a']
        if args.sdk:
            files.extend((HERE/'driver_guest.c',args.sdk/'bin/wrc',args.sdk/'share/abi/ros.json'))
        if args.historical_distro:
            files.append(HERE/'historical_witness.py')
        for leaf in ('attachment_peer.cpp','attachment_peer.py','run_module_security_vm.py','run_scene_vm.py'):
            files.append(args.vm_source/'test/0084.ros-runtime'/leaf)
        manifest['inputs']={str(path):digest(path) for path in files}
        for role in ROLES:
            for engine,denied in [('int',True),('int',False),('aot',False)]:
                casepath=args.output/(role+'-'+engine+('-denied' if denied else '-granted'))
                if args.sdk:
                    casepath.mkdir()
                    token = secrets.token_hex(16)
                    deadline = time.monotonic_ns()+30_000_000_000
                    receipt = compile_sdk_guest(args,casepath,role,denied,token.encode(),deadline)
                    (casepath/'sdk-case.json').write_text(json.dumps(dict(token=token,deadline=deadline,
                        receipt=receipt,wasm=digest(casepath/'application.wasm'))))
                cmd=['/usr/bin/bwrap','--die-with-parent','--unshare-net','--ro-bind','/','/','--bind',
                     args.output,args.output,'--proc','/proc','--dev','/dev',sys.executable,'-B',Path(__file__).resolve(),
                     *sys.argv[1:],'--role',role,'--engine',engine,*(['--denied'] if denied else [])]
                subprocess.run(cmd,check=True,timeout=45)
                manifest['cases'].append(json.loads((casepath/'observed.json').read_bytes()))
        manifest['complete']=len(manifest['cases'])==6
    finally:
        (args.output/'results.json').write_text(json.dumps(manifest,indent=2)+'\n')


if __name__=='__main__':
    main()
