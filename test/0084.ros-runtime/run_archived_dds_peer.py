"""Run the native DDS fixture and tie the executing peer libraries to signed ROS packages."""
import argparse
from email.parser import BytesHeaderParser
import gzip
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tarfile
import time

from run_scene_vm import sha, verify_gate

sys.path.insert(0, str(Path(__file__).resolve().parents[2]/'tools/ros_scene'))
from rmw_native_wire import peer_diagnostics

SIGNER = 'C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654'
LIBRARIES = re.compile(r'lib(?:ddsc|fastdds|fastrtps|rclcpp|rmw_fastrtps_cpp|rmw_cyclonedds_cpp)\.so(?:\..*)?$')


def require(ok, message):
    if not ok:
        raise ValueError(message)


def signed_packages(plan_path, keyring):
    plan = json.loads(plan_path.read_bytes())
    folder = plan_path.parent
    release = folder/'InRelease'
    index = folder/'Packages.gz'
    status = subprocess.run(['/usr/bin/gpgv','--keyring',keyring,'--status-fd','1',release],
                            capture_output=True, check=True)
    require(('[GNUPG:] VALIDSIG '+SIGNER+' ').encode() in status.stdout, 'ROS archive signature mismatch')
    require(sha(release) == plan['inReleaseSha256'] and plan['fingerprint'] == SIGNER, 'release identity mismatch')
    body = release.read_bytes()
    require(('\nCodename: '+plan['suite']+'\n').encode() in body and b'\nOrigin: ROS\n' in body, 'wrong ROS suite')
    table = body.split(b'\nSHA256:\n',1)[1].split(b'\nSHA512:',1)[0]
    h,n = re.search(rb'^ ([0-9a-f]{64})\s+(\d+) main/binary-amd64/Packages\.gz$',table,re.M).groups()
    require(index.stat().st_size == int(n) and sha(index) == h.decode() == plan['indexSha256'], 'index differs')
    prefix = 'ros-'+plan['release']+'-'
    names = {prefix+name for name in ['cyclonedds','rclcpp','rmw-cyclonedds-cpp']}
    selected = {}
    for entry in gzip.decompress(index.read_bytes()).split(b'\n\n'):
        package = entry.partition(b'\n')[0].removeprefix(b'Package: ').decode(errors='replace')
        if package not in names:
            continue
        fields = BytesHeaderParser().parsebytes(entry)
        p = plan['packages'][package]
        require(fields['SHA256'] == p['sha256'] and fields['Version'] == p['version'] and
                fields['Filename'] == p['filename'] and fields['Architecture'] == 'amd64', 'signed package differs')
        archive = folder/plan['release']/Path(p['filename']).name
        require(archive.stat().st_size == int(fields['Size']) and sha(archive)==fields['SHA256'], 'archive bytes differ')
        selected[package] = (archive, p)
    require(names == selected.keys(), 'incomplete signed Cyclone/rclcpp input set')
    return plan, selected


def monitor(command, out, expected):
    observed = {name: {} for name in expected.values()}
    with (out/'runner.log').open('wb') as log:
        process = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
        try:
            while process.poll() is None:
                pending = [process.pid]
                seen = set()
                while pending:
                    pid = pending.pop()
                    if pid in seen:
                        continue
                    seen.add(pid)
                    p = Path('/proc')/str(pid)
                    try:
                        pending.extend(map(int,(p/'task'/str(pid)/'children').read_text().split()))
                        path = os.readlink(p/'exe')
                        role = expected.get(path)
                        if role:
                            for line in (p/'maps').read_text().splitlines():
                                fields = line.split(None,5)
                                if len(fields) == 6 and 'x' in fields[1]:
                                    lib = Path(fields[5])
                                    if lib.is_absolute() and LIBRARIES.fullmatch(lib.name):
                                        observed[role][str(lib)] = None
                    except (OSError, ValueError):
                        continue
                time.sleep(0.015)
        finally:
            if process.poll() is None:
                process.terminate()
                process.wait()
    return process.returncode, {role:{path:sha(path) for path in sorted(libs)} for role,libs in observed.items()}


def match_payloads(prefix, packages, libraries):
    """Check bytes of mapped files directly against native Debian package members."""
    paths = {str(Path(path).relative_to(prefix.parents[2])):digest for path,digest in libraries.items()
             if Path(path).is_relative_to(prefix)}
    found = {}
    for name,(archive, _) in packages.items():
        with subprocess.Popen(['/usr/bin/dpkg-deb','--fsys-tarfile',archive], stdout=subprocess.PIPE) as extract:
            with tarfile.open(fileobj=extract.stdout,mode='r|') as stream:
                for member in stream:
                    rel = member.name.removeprefix('./')
                    if member.isfile() and rel in paths:
                        digest = hashlib.file_digest(stream.extractfile(member),'sha256').hexdigest()
                        require(paths[rel] == digest, 'loaded native library differs from ROS Debian package')
                        found[rel] = {'package':name,'sha256':digest}
            extract.stdout.read()
            require(extract.wait()==0, 'unreadable native package')
    require(paths.keys()==found.keys(), 'loaded native library missing from selected Debian archives')
    return found


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('runner','plan','keyring','ros-prefix','ros-system-libs','peer-prefix','peer-system-libs','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--auxiliary-cyclone',type=Path,
                        help='Auxiliary IDL compiler and C build for rclcpp runs on older installations')
    parser.add_argument('--raw-cyclone',action='store_true')
    parser.add_argument('--peer-cxx-standard',choices=('17','20'),help='Explicit language mode for older rclcpp headers')
    a = parser.parse_args()
    verify_gate()
    require(os.sched_getaffinity(0).issubset({4,5,6,7}), 'unexpected qualification CPUs')
    out = a.output.resolve()
    out.mkdir(parents=True,exist_ok=False)
    result = out/'results.json'
    prefix = a.peer_prefix.resolve(strict=True)
    report = {'format':'uwvm-archived-native-dds-peer-v1','complete':False,'nativeOnly':True,'stages':[],
              'rosGraphInteroperability':'unverified','sourcePlanSha256':sha(a.plan),'fixtureSha256':sha(a.runner),
              'observerSha256':sha(__file__),'keyringSha256':sha(a.keyring)}
    def save():
        result.write_text(json.dumps(report,indent=2)+'\n')
    save()
    try:
        plan,packages = signed_packages(a.plan,a.keyring.resolve(strict=True))
        require(prefix.name==plan['release'] and a.ros_prefix.name=='jazzy', 'prefix does not match release')
        require(not (a.raw_cyclone and a.auxiliary_cyclone), 'raw peer must use its own installed Cyclone library')
        report.update(release=plan['release'],suite=plan['suite'],api='DDS C' if a.raw_cyclone else 'rclcpp',
                      auxiliaryCyclone=str((a.auxiliary_cyclone or prefix).resolve()), signer=SIGNER,
                      indexSha256=plan['indexSha256'],inReleaseSha256=plan['inReleaseSha256'],
                      packages={name:{k:p[k] for k in ('version','filename','sha256')} for name,(_,p) in packages.items()})
        report['stages'].append('verified-signed-packages')
        save()
        wire = out/'wire'
        command = [sys.executable,'-B',a.runner.resolve(),'--ros-prefix',a.ros_prefix,'--ros-system-libs',a.ros_system_libs,
                   '--cyclone-prefix',a.auxiliary_cyclone or prefix,'--output',wire]
        if not a.raw_cyclone:
            command.extend(['--peer-ros-prefix',prefix,'--peer-system-libs',a.peer_system_libs])
        if a.peer_cxx_standard:
            require(not a.raw_cyclone, 'C++ language mode applies to an rclcpp peer')
            command.extend(['--peer-cxx-standard',a.peer_cxx_standard])
        if a.raw_cyclone:
            command.extend(['--cyclone-system-libs',a.peer_system_libs])
        code,loaded = monitor(command,out,{str(wire/'ros-client'):'request',str(wire/('cyclone-echo' if a.raw_cyclone else 'ros-echo')):'reply'})
        report.update(exit=code, loadedLibraries=loaded, logSha256=sha(out/'runner.log'))
        if (wire/'results.json').is_file():
            report['wireReceiptSha256'] = sha(wire/'results.json')
            wire_report = json.loads((wire/'results.json').read_bytes())
            report['wireSteps'] = [{'name':step['name'],'exit':step['exit']} for step in wire_report['steps']]
        report['wireLogs'] = {log.name:sha(log) for log in sorted(wire.glob('*.log'))}
        report['diagnostics'] = {peer:peer_diagnostics(wire,report['wireLogs'],log) for peer,log in
                                 [('request','ros.log'),('reply','cyclone.log')]}
        save()
        require(code==0,'native DDS fixture failed')
        src = json.loads((wire/'results.json').read_bytes())
        require(src['complete'] and src['reply_peer']['prefix']==str(prefix), 'incorrect native peer receipt')
        required = {'libddsc'} if a.raw_cyclone else {'libddsc','librclcpp','librmw_cyclonedds_cpp'}
        scoped = {Path(path).name.split('.so')[0] for path in loaded['reply'] if Path(path).is_relative_to(prefix)}
        require(scoped>=required,'missing execution proof for release-specific libraries')
        request_libs = {Path(path).name.split('.so')[0] for path in loaded['request'] if Path(path).is_relative_to(a.ros_prefix)}
        require(request_libs >= {'librclcpp','librmw_fastrtps_cpp'} and request_libs & {'libfastdds','libfastrtps'},
                'missing execution proof for Jazzy Fast DDS')
        report['nativePackageMembers'] = match_payloads(prefix,packages,loaded['reply'])
        ex = json.loads((wire/'exchange.json').read_bytes())
        require(ex['ros_exit']==ex['cyclone_exit']==0 and ex['expected_response']==1+ex['challenge'],'unexpected exchange')
        report['exchange'] = ex
        report['stages'].append('verified-mapped-libraries-and-int32-roundtrip')
        report['complete'] = True
    except (OSError,ValueError,KeyError,subprocess.SubprocessError) as error:
        report['error'] = str(error)
        raise
    finally:
        save()


if __name__=='__main__':
    main()
