"""Exchange a typed sample using additional ROS installations' native RMW plugins."""
import argparse
from contextlib import ExitStack
import json
import os
import re
from pathlib import Path
import secrets
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT/'tools/ros_scene'))
from rmw_native_wire import digest, verify
sys.path.insert(0, str(ROOT/'test/0084.ros-runtime'))
from run_scene_vm import GATE, verify_gate

VENDORS = {
    'rmw_fastrtps_dynamic_cpp': 'libfastrtps.so',
    'rmw_opensplice_cpp': 'libddskernel.so',
    'rmw_int2dds_cpp': 'libint2dds_ffi.so',
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--baseline', type=Path, required=True)
    parser.add_argument('--overlay', type=Path, required=True)
    parser.add_argument('--rmw', required=True, help='Exact rmw implementation identifier')
    parser.add_argument('--local-library', nargs=3, metavar=('SONAME', 'PLUGIN_SHA256', 'LIBRARY_SHA256'),
                        help='Require these exact original library digests for a site-provided RMW')
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--worker', action='store_true', help=argparse.SUPPRESS)
    a = parser.parse_args()
    verify_gate()
    receipt, overlay, out = (p.resolve() for p in (a.baseline, a.overlay, a.output))
    checked = verify(receipt)
    if checked['status'] != 'passed' or checked['replyPeer']['api'] != 'rclcpp':
        raise ValueError('requires an executed two-rclcpp baseline')
    if '.tmp' not in out.parts:raise ValueError('choose a fresh qualification output')
    if not re.fullmatch(r'rmw_[a-z0-9_]{1,100}', a.rmw):
        parser.error('invalid ROS middleware identifier')
    if a.local_library:
        soname, plugin_hash, library_hash = a.local_library
        if (not re.fullmatch(r'lib[a-zA-Z0-9_+.-]+[.]so(?:[.][0-9]+)*', soname)
                or not all(re.fullmatch(r'[a-f0-9]{64}', h) for h in (plugin_hash, library_hash))):
            parser.error('provide a vendor library basename and two SHA-256 digests')
    elif a.rmw in VENDORS:
        soname = VENDORS[a.rmw]
    else:
        parser.error('a site-provided RMW requires --local-library')
    baseline = json.loads(receipt.read_bytes())
    args = baseline['steps'][-1]['argv']
    left, right = (Path(baseline[k]['prefix']) for k in ('request_peer','reply_peer'))
    if overlay.name != right.name:
        raise ValueError('RMW overlay must match the reply distribution')
    if not a.worker:
        # Supply only the ROS packages, the evidence inputs and the system runtime.
        out.mkdir(parents=True)
        mounts = {
            left, right, overlay, Path(baseline['cyclone_prefix']), receipt.parent,
            Path(args[args.index('--ros-system-libs')+1]), Path(args[args.index('--peer-system-libs')+1]),
            Path(__file__).resolve(), ROOT/'tools/ros_scene/rmw_native_wire.py',
            ROOT/'test/0084.ros-runtime/run_scene_vm.py',
            ROOT/'test/0084.ros-runtime/dds_interop/fastdds-loopback.xml',
            GATE/'memory.max', GATE/'memory.swap.max',
        }
        if a.rmw == 'rmw_opensplice_cpp':
            mounts.add(Path(__file__).with_name('opensplice-loopback.xml'))
        mounts.update(Path(p) for p in baseline['inputs'] if '/dds_interop/' in p)
        cmd = ['/usr/bin/bwrap', '--die-with-parent', '--unshare-net', '--unshare-ipc', '--unshare-pid',
               '--new-session', '--cap-drop', 'ALL', '--clearenv', '--setenv', 'PATH', '/usr/bin:/bin',
               '--ro-bind', '/usr', '/usr', '--symlink', 'usr/bin', '/bin', '--symlink', 'usr/lib', '/lib',
               '--symlink', 'usr/lib64', '/lib64', '--proc', '/proc', '--dev', '/dev', '--tmpfs', '/tmp']
        for path in sorted(mounts):
            if any(parent in mounts for parent in path.parents):
                continue
            cmd.extend(('--ro-bind', path, path))
        cmd.extend(('--bind', out, out, '--chdir', out, sys.executable, '-I', '-B', Path(__file__).resolve(),
                    '--worker', '--baseline', receipt, '--overlay', overlay, '--rmw', a.rmw, '--output', out))
        if a.local_library:
            cmd.extend(('--local-library', *a.local_library))
        return subprocess.run(cmd, timeout=55).returncode
    interfaces = [line.split(':',1)[0].strip() for line in Path('/proc/net/dev').read_text().splitlines()[2:]]
    if interfaces != ['lo']:
        raise RuntimeError('only loopback may be present')
    xml = ROOT/'test/0084.ros-runtime/dds_interop/fastdds-loopback.xml'
    libs = [overlay/'lib',right/'lib',right/'lib/x86_64-linux-gnu',Path(args[args.index('--peer-system-libs')+1])]
    right_rmw = (libs[0]/('lib'+a.rmw+'.so')).resolve(strict=True)
    vendor_lib = next(((p/soname).resolve() for p in libs if (p/soname).is_file()), None)
    if vendor_lib is None:
        raise ValueError('required site library is absent: '+soname)
    if a.local_library and (digest(right_rmw) != plugin_hash or digest(vendor_lib) != library_hash):
        raise ValueError('site-provided middleware identity differs before peer startup')
    left_rmw = (left/'lib/librmw_fastrtps_cpp.so').resolve(strict=True)
    fast = next((left/'lib'/n).resolve() for n in ('libfastrtps.so','libfastdds.so') if (left/'lib'/n).exists())
    left_env = dict(PATH='/usr/bin:/bin',HOME=str(out),ROS_LOG_DIR=str(out/'log'),
                    LD_LIBRARY_PATH=f"{left}/lib:{args[args.index('--ros-system-libs')+1]}",
                    AMENT_PREFIX_PATH=str(left),RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_DOMAIN_ID='61',
                    ROS_AUTOMATIC_DISCOVERY_RANGE='SYSTEM_DEFAULT',FASTRTPS_DEFAULT_PROFILES_FILE=str(xml))
    right_env = dict(left_env, LD_LIBRARY_PATH=os.pathsep.join(map(str,libs)),AMENT_PREFIX_PATH=f'{overlay}:{right}',
                     RMW_IMPLEMENTATION=a.rmw)
    if a.rmw == 'rmw_opensplice_cpp':
        ospl = Path(__file__).with_name('opensplice-loopback.xml')
        metaconfig = overlay/'etc/ospl_metaconfig.xml'
        if not metaconfig.is_file():
            raise ValueError('place the original OpenSplice configuration schema at '+str(metaconfig))
        right_env.update(OSPL_HOME=str(overlay), OSPL_URI=ospl.as_uri(), OSPL_INFOFILE='<stdout>',
                         OSPL_ERRORFILE='<stderr>', ROS_HOME=str(out))
    expected = {'request':[left_rmw,fast],'echo':[right_rmw,vendor_lib]}
    report = dict(complete=False,baseline=checked['receiptSha256'], transport='isolated loopback, plaintext Int32',
                  rmw=a.rmw,localLibrary=a.local_library,libraryHashes={str(p):digest(p) for files in expected.values() for p in files},
                  source=digest(Path(__file__)), profile=digest(xml), processes={})
    if a.rmw == 'rmw_opensplice_cpp':
        report.update(openSpliceProfileSha256=digest(ospl), openSpliceMetaconfigSha256=digest(metaconfig))
    challenge = 1+secrets.randbelow(2**29)
    report.update(challenge=challenge,expectedReply=challenge+1)
    try:
        with ExitStack() as resources:
            processes={}
            for role,name,env in [('echo','ros-echo',right_env),('request','ros-client',left_env)]:
                log = resources.enter_context((out/(role+'.log')).open('wb'))
                err = resources.enter_context((out/(role+'.stderr')).open('wb'))
                cmd = [str(receipt.parent/name),str(challenge),role,env['RMW_IMPLEMENTATION']]
                process = subprocess.Popen(cmd,env=env,cwd=out,stdout=log,stderr=err)
                processes[role]=process
                resources.callback(process.wait)
                resources.callback(process.kill)
                report['processes'][role] = dict(argv=cmd,binary=digest(Path(cmd[0])),observed=[])
            end=time.monotonic()+20
            while any(p.poll() is None for p in processes.values()):
                if time.monotonic()>=end:raise ValueError('native middleware peers timed out')
                for role,proc in processes.items():
                    row=report['processes'][role]
                    if len(row['observed'])==len(expected[role]):continue
                    try:maps=Path(f'/proc/{proc.pid}/maps').read_text()
                    except (FileNotFoundError,ProcessLookupError):continue
                    mapped={line.split(maxsplit=5)[-1] for line in maps.splitlines() if len(line.split(maxsplit=5))==6}
                    row['observed']=sorted(set(row['observed'])|{str(p) for p in expected[role] if str(p) in mapped})
                time.sleep(.01)
            for role,p in processes.items():
                row=report['processes'][role];row['exit']=p.returncode
                log=out/(role+'.log');row['logSha256']=digest(log)
                text=log.read_text(errors='replace')
                marker = 'rclcpp '+row['argv'][-1]+' '+role+' completed'
                lines = text.splitlines(keepends=True)
                row['reportedCompletion'] = lines.count(marker+'\n') == 1
                err = out/(role+'.stderr')
                row['stderrSha256'] = digest(err)
                if row['reportedCompletion']:
                    lines.remove(marker+'\n')
                diagnostics = ''.join(lines) + err.read_text(errors='replace')
                row['diagnostics'] = diagnostics.strip().splitlines()
                row['rcutilsErrorReports']=diagnostics.count('] rcutils_set_error_state()')
            report['complete']=all(r['exit']==0 and r['reportedCompletion'] and
                                   set(r['observed'])==set(map(str,expected[k])) for k,r in report['processes'].items())
    finally:
        (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    return int(not report['complete'])


if __name__=='__main__':
    sys.exit(main())
