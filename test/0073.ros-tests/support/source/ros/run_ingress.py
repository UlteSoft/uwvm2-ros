#!/usr/bin/env python3
"""Real network proof of startup socket/DL rejection and Wasm trap cleanup."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import signal
import socket
import subprocess
import time
import run_network as network


class IngressRunner(network.Runner):
    def command(self, version, grants, directory, guest, extra=(), isolate_wasi=True):
        connection, _ = network.policy(version, [])
        cli = [f'--ros{version}-global-enable']
        for key, value in {**connection, 'gateway-socket':str(directory/f'ros{version}.sock'),
                'gateway-uid':str(os.getuid()), 'connect-timeout-ms':'1000',
                'audit-deny-max-rate-hz':'100', 'audit-deny-burst':'100',
                'audit-deny-max-records':'10000', 'audit-deny-max-bytes':'1048576'}.items():
            cli += [f'--ros{version}-global-'+key, value]
        for cap in grants:
            cli += [f'--ros{version}-global-grant', *[cap[k] for k in ('alias','operation','resource','type','identity')]]
            for key, value in cap['limits'].items():
                cli += [f'--ros{version}-global-set', cap['alias'], key, value]
        self.snapshot(self.source/guest, directory/'guest.wasm', directory.name+'/guest')
        # Host inheritance is now denied by default. The negative ingress case
        # must explicitly request inheritance to exercise the ROS startup guard.
        isolation=['--wasip1-global-noinherit-system-environment'] if isolate_wasi else ['--wasip1-global-inherit-system-environment']
        command = [self.product, *cli, *extra, *isolation,
                   '--wasip1-global-grant', 'monotonic-clock',
                   '--wasm-set-main-module-name', 'main', '--run', str(directory/'guest.wasm')]
        (directory/'uwvm-argv.json').write_text(json.dumps(command, indent=2)+'\n')
        return connection, command

    def start_input(self, command, name, descriptor=None):
        with open(self.run/(name+'.log'), 'wb') as log:
            process = subprocess.Popen(command, stdin=descriptor, stdout=log, stderr=subprocess.STDOUT,
                                       env=self.env, start_new_session=True)
        self.processes.append(process)
        return process

    def startup(self, version, kind, plugin):
        name = f'ros{version}-'+kind
        directory = self.run/name; directory.mkdir(mode=0o700)
        _, grants = network.policy(version, ['publish'])
        peer = inherited = listener = None
        extra = []
        diagnostic = 'strict ROS worker cannot admit additional native DL plugins'
        guest = 'examples/ros_dual/plugin_ingress.wasm'
        if kind == 'native-plugin-ingress':
            extra = ['--wasm-register-dl', str(plugin), 'dl.example',
                     '--wasm-set-preload-module-attribute', 'dl.example', 'copy', 'all',
                     '--wasip1-global-expose-host-api']
        elif kind == 'missing-environment-isolation':
            guest='examples/ros_dual/socket_ingress.wasm'
            diagnostic='ROS worker requires WASI host environment isolation for main'
        elif kind == 'directory-preopen':
            guest='examples/ros_dual/socket_ingress.wasm'
            credentials=directory/'credentials';credentials.mkdir()
            (credentials/'canary.txt').write_text('test-only ROS credential canary\n')
            extra=['--wasip1-global-mount-dir','/credentials',str(credentials)]
            diagnostic='ROS worker rejects WASI directory preopens for main'
        else:
            guest = 'examples/ros_dual/socket_ingress.wasm'
            if kind == 'socket-standard-input':
                inherited, peer = socket.socketpair()
                diagnostic = 'strict ROS worker rejects socket standard descriptors'
            else:
                listener = socket.socket(); listener.bind(('127.0.0.1', 0)); listener.listen(1); listener.settimeout(.2)
                address=f'127.0.0.1:{listener.getsockname()[1]}'
                if kind == 'socket-single-preopen':
                    extra=['--wasip1-single-create','main','--wasip1-single-enable','main',
                           '--wasip1-single-socket-tcp-connect','main','3',address]
                elif kind == 'socket-group-preopen':
                    extra=['--wasip1-group-create','ingress','--wasip1-group-add-module','ingress','main',
                           '--wasip1-group-socket-tcp-connect','ingress','3',address]
                else: extra=['--wasip1-global-socket-tcp-connect','3',address]
                diagnostic = 'ROS worker rejects WASI socket preopens for main'
        _, command = self.command(version, grants, directory, guest, extra, kind != 'missing-environment-isolation')
        native = self.start(self.native_command(version, [f'/tmp/ros{version}-build/uwvm-ros{version}-native-probe',
                            '--absent', '/uwvm_ros_test/output', 'hello', 'String']), name+'-native')
        worker = None
        try:
            time.sleep(.3)
            if native.poll() is not None: raise RuntimeError('FAIL native absence witness exited before ingress attempt')
            worker = self.start_input(command, name+'-uwvm', inherited)
            if inherited: inherited.close(); inherited = None
            code = worker.wait(timeout=8)
            text = (self.run/(name+'-uwvm.log')).read_text(errors='replace')
            if code != 126 or diagnostic not in text:
                raise RuntimeError(f'FAIL {name} expected startup126 and intended diagnostic, got {code}: {text[:600]}')
            if listener:
                try: peer, _ = listener.accept()
                except socket.timeout: pass
                else: raise RuntimeError('FAIL WASI preopen established a socket before startup rejection')
            peer_bytes = 0
            if peer:
                peer.settimeout(1)
                try: data = peer.recv(4096)
                except socket.timeout: raise RuntimeError('FAIL rejected worker retained ingress socket')
                peer_bytes = len(data)
                if peer_bytes: raise RuntimeError('FAIL Guest wrote network bytes before startup rejection')
            if native.wait(timeout=8): raise RuntimeError('FAIL startup denial created actual ROS publisher/message')
            row = {'name':name, 'status':'PASS', 'wasm_exit':code, 'guest_start_sentinel':201,
                   'guest_start_entered':False, 'intended_diagnostic':diagnostic,
                   'native_ros_publisher_count':0, 'native_ros_message_count':0,
                   'guest_sha256':self.artifacts[name+'/guest']['sha256'],
                   'product_sha256':self.artifacts['uwvm']['sha256'],
                   'native_witness_sha256':self.artifacts[f'ros{version}/uwvm-ros{version}-native-probe']['sha256']}
            if peer: row['socket_peer_received_bytes'] = peer_bytes
            if listener: row['socket_peer_connections'] = 0
            if kind == 'native-plugin-ingress': row['plugin_sha256'] = self.artifacts['trusted-dl-fixture']['sha256']
            self.results.append(row); print('PASS '+name, flush=True)
        finally:
            for value in (peer, inherited, listener):
                if value: value.close()
            for process in (worker, native):
                if process: self.stop(process)

    def trap(self, version):
        name = f'ros{version}-wasm-trap-resource-cleanup'
        directory = self.run/name; directory.mkdir(mode=0o700)
        connection, grants = network.policy(version, ['publish','subscribe','service-denied'])
        _, command = self.command(version, grants, directory, f'examples/ros{version}/trap_cleanup.wasm')
        worker = self.start(['/bin/bash','-c','kill -STOP $$; exec "$@"','uwvm-ros-worker',*command], name+'-uwvm')
        gateway = native = None
        try:
            lines = [f'backend ros{version}', f'worker-uid {os.getuid()}', f'worker-pid {worker.pid}',
                     f'socket /uwvm-ros-test/{name}/ros{version}.sock', 'timeout-ms 10000', 'max-sessions 16',
                     'audit-max-records-per-second 100', 'audit-max-records 10000', 'audit-max-bytes 1048576']
            lines += [f'connection {key} {value}' for key,value in connection.items()]
            for cap in grants:
                lines.append('grant '+' '.join(cap[key] for key in ('alias','operation','resource','type','identity')))
                lines += [f"set {cap['alias']} {key} {value}" for key,value in cap['limits'].items()]
            config = directory/f'ros{version}.txt'; config.write_text('\n'.join(lines)+'\n'); os.chmod(config,0o600)
            gateway = self.start(self.native_command(version, [f'/tmp/ros{version}-build/uwvm-ros{version}-gateway',
                                 f'/uwvm-ros-test/{name}/ros{version}.txt']), name+'-gateway')
            deadline = time.monotonic()+4
            while not (directory/f'ros{version}.sock').exists():
                if gateway.poll() is not None or time.monotonic() > deadline: raise RuntimeError('FAIL trap gateway startup')
                time.sleep(.02)
            native = self.start(self.native_command(version, [f'/tmp/ros{version}-build/uwvm-ros{version}-native-probe',
                                '--observe-cleanup']), name+'-native')
            log = self.run/(name+'-native.log')
            deadline = time.monotonic()+5
            while b'READY' not in log.read_bytes():
                if native.poll() is not None or time.monotonic() > deadline: raise RuntimeError('FAIL trap native readiness')
                time.sleep(.02)
            os.kill(worker.pid, signal.SIGCONT)
            code = worker.wait(timeout=15)
            text = (self.run/(name+'-uwvm.log')).read_text(errors='replace')
            if code == 0 or 'READY_TRAP' not in text or 'unreachable' not in text.lower():
                raise RuntimeError(f'FAIL expected actual Wasm unreachable trap after resource creation, exit {code}: {text[-600:]}')
            if gateway.poll() not in (None,0): raise RuntimeError('FAIL gateway crashed before native cleanup observation')
            if native.wait(timeout=15): raise RuntimeError('FAIL native trap cleanup witness')
            native_text = log.read_text(errors='replace')
            if 'OBSERVED_RESOURCES' not in native_text or 'PASS trap cleanup' not in native_text:
                raise RuntimeError('FAIL missing actual resource presence/absence evidence')
            gateway_exit=gateway.poll()
            if gateway_exit not in (None,0): raise RuntimeError('FAIL gateway crashed during native cleanup observation')
            if gateway_exit==0 and (directory/f'ros{version}.sock').exists():
                raise RuntimeError('FAIL worker-lifetime gateway exit retained its socket')
            self.results.append({'name':name, 'status':'PASS', 'wasm_exit':code, 'actual_unreachable_trap':True,
                'native_before_trap':['publisher','subscriber','Trigger service','exact hello'],
                'native_after_trap':{'publishers':0,'subscribers':0,'services':0,'consecutive_graph_samples':10},
                'gateway_alive_after_cleanup':gateway_exit is None,
                'gateway_unassisted_exit':gateway_exit,
                'gateway_socket_removed':not (directory/f'ros{version}.sock').exists(),
                'test_termination_sent_before_native_observation':False,
                'guest_sha256':self.artifacts[name+'/guest']['sha256'],
                'product_sha256':self.artifacts['uwvm']['sha256'],
                'gateway_sha256':self.artifacts[f'ros{version}/uwvm-ros{version}-gateway']['sha256'],
                'native_witness_sha256':self.artifacts[f'ros{version}/uwvm-ros{version}-native-probe']['sha256']})
            print('PASS '+name, flush=True)
        finally:
            for process in (worker,native,gateway):
                if process: self.stop(process)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace',default='/tmp/uwvm-ros-extension-20261012')
    parser.add_argument('--source',default='/tmp/uwvm-ros-extension-20261012/source')
    parser.add_argument('--uwvm',default='/tmp/uwvm-ros-extension-20261012/product-final-v6-event/dual/uwvm2-ros')
    parser.add_argument('--llvm-libs',default=os.environ.get('LD_LIBRARY_PATH',''))
    parser.add_argument('--version',choices=['1','2','both'],default='both')
    parser.add_argument('--mode',choices=['startup','trap','all'],default='all')
    parser.add_argument('--cc',default='/usr/bin/cc')
    args=parser.parse_args()
    if os.name!='posix' or not Path('/proc/self/cgroup').exists():
        print('BLOCKED Linux cgroup v2 and prepared native ROS gateways are required',flush=True);return 77
    runner=IngressRunner(args)
    runner.run=runner.base/'ingress'/time.strftime('%Y%m%d-%H%M%S'); runner.run.mkdir(parents=True,mode=0o700)
    before={}; after={}; status='PASS'
    def interrupted(signum,frame): raise RuntimeError(f'FAIL interrupted by signal {signum}; bounded cleanup follows')
    signal.signal(signal.SIGTERM,interrupted); signal.signal(signal.SIGINT,interrupted)
    try:
        before=network.cgroup_snapshot(); versions=(1,2) if args.version=='both' else (int(args.version),)
        runner.prepare(versions)
        plugin=runner.run/'libregdl.so'
        if args.mode in ('startup','all'):
            compiled=subprocess.run([args.cc,'-std=c17','-shared','-fPIC',str(runner.source/'examples/0002.dl/regdl.c'),'-o',str(plugin)],
                                    stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=30)
            (runner.run/'plugin-compile.log').write_bytes(compiled.stdout+compiled.stderr)
            if compiled.returncode: raise RuntimeError('FAIL real Trusted-DL fixture compile')
            runner.snapshot(plugin,runner.run/'snapshots/libregdl.so','trusted-dl-fixture')
            plugin=runner.run/'snapshots/libregdl.so'
        for version in versions:
            if args.mode in ('startup','all'):
                for kind in ('socket-standard-input','socket-preopen','socket-single-preopen','socket-group-preopen',
                             'missing-environment-isolation','directory-preopen','native-plugin-ingress'):
                    runner.startup(version,kind,plugin)
            if args.mode in ('trap','all'): runner.trap(version)
    except (RuntimeError,OSError,subprocess.SubprocessError) as error:
        status='BLOCKED' if str(error).startswith('BLOCKED') else 'FAIL'
        runner.results.append({'status':status,'reason':str(error)}); print(status,error,flush=True)
    finally:
        try: runner.cleanup()
        except (RuntimeError,OSError,subprocess.SubprocessError) as error:
            status='FAIL'; runner.results.append({'status':'FAIL','reason':'cleanup failure '+str(error)})
    try: after=network.cgroup_snapshot()
    except RuntimeError as error: status='FAIL'; runner.results.append({'status':'FAIL','reason':str(error)})
    def events(value): return dict(line.split() for line in value.get('memory.events','').splitlines())
    if before and after and any(events(before).get(key)!=events(after).get(key) for key in ('oom','oom_kill')):
        status='FAIL'; runner.results.append({'status':'FAIL','reason':'OOM counters increased'})
    (runner.run/'report.json').write_text(json.dumps({'status':status,'artifacts':runner.artifacts,'results':runner.results,
         'cgroup_before':before,'cgroup_after':after},indent=2)+'\n')
    print(status,runner.run,flush=True); return 0 if status=='PASS' else 77 if status=='BLOCKED' else 1

if __name__=='__main__': raise SystemExit(main())
