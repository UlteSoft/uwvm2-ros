"""Qualify two native Fast DDS releases with the test SROS2 policy and a packet observer."""
import argparse
import fnmatch
from contextlib import ExitStack
import json
import os
from pathlib import Path
import resource
import secrets
import shutil
import socket
import subprocess
import sys
import tempfile
import time
import xml.etree.ElementTree as ET

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
from wire import DOMAIN, app_pid, receive_ports, summarize, trace_command

ENCLAVES = {'request': 'gateway', 'echo': 'witness', 'observer': 'observer'}
LIBS = ('rclcpp', 'rcl', 'rcutils', 'rmw', 'rmw_implementation', 'std_msgs__rosidl_typesupport_cpp',
        'libstatistics_collector', 'tracetools', 'rcl_interfaces__rosidl_typesupport_cpp', 'rcpputils',
        'statistics_msgs__rosidl_typesupport_cpp', 'rosgraph_msgs__rosidl_typesupport_cpp',
        'builtin_interfaces__rosidl_typesupport_cpp')


def check(ok, message):
    if not ok:
        raise ValueError(message)


def sha(path):
    import hashlib
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def save(path, obj):
    path.write_text(json.dumps(obj, indent=2) + '\n')


def build(prefix, system, path):
    inc = prefix / 'include'
    includes = [inc, *(p for p in sorted(inc.iterdir()) if (p/p.name).is_dir())]
    cmd = ['/usr/bin/clang++-22', '-std=c++20', '-O3', '-g', '-Wall', '-Wextra', '-Werror',
           *(v for p in includes for v in ('-isystem', str(p))), HERE/'participant.cpp', '-L'+str(prefix/'lib'),
           '-L'+str(system), '-Wl,-rpath-link,'+str(prefix/'lib'), '-Wl,-rpath-link,'+str(system),
           *['-l'+name for name in LIBS], '-o', path]
    with path.with_suffix('.build.log').open('wb') as log:
        subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT, check=True, timeout=90)
    return list(map(str, cmd))


def issue(rootfs, policy, stage, output):
    tree = ET.parse(policy)
    enclaves = tree.getroot().find('enclaves')
    check({e.get('path') for e in enclaves} == {'/gateway', '/witness'}, 'unexpected witness policy')
    observer = ET.fromstring('<enclave path="/observer"><profiles><profile ns="/uwvm_test" node="observer">'
                             '<topics subscribe="ALLOW"><topic>/parameter_events</topic></topics>'
                             '</profile></profiles></enclave>')
    enclaves.append(observer)
    tree.write(output/'policy.xml', encoding='utf-8', xml_declaration=True)
    cmd = ['/usr/bin/bwrap', '--die-with-parent', '--unshare-net', '--unshare-pid', '--unshare-ipc',
           '--ro-bind', rootfs, '/', '--proc', '/proc', '--dev', '/dev', '--tmpfs', '/tmp', '--dir', '/tmp/home',
           '--bind', stage, '/tmp/issued', '--ro-bind', output/'policy.xml', '/tmp/policy.xml',
           '--clearenv', '--setenv', 'PATH', '/usr/bin:/bin', '--setenv', 'HOME', '/tmp/home',
           '--setenv', 'LANG', 'C.UTF-8', '--setenv', 'ROS_DOMAIN_ID', str(DOMAIN), '/bin/bash', '-c',
           '. /opt/ros/jazzy/setup.bash && exec ros2 security generate_artifacts -k /tmp/issued/keystore -p /tmp/policy.xml']
    with (output/'issue.log').open('wb') as log:
        subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT, timeout=60, check=True)
    keys = stage/'keystore/enclaves'
    policies = {}
    for enclave in ENCLAVES.values():
        folder = keys/enclave
        entry = {'certificateSha256': sha(folder/'cert.pem')}
        for kind in ('governance', 'permissions'):
            signed = folder/(kind+'.p7s')
            plain = subprocess.check_output(['/usr/bin/openssl', 'smime', '-verify', '-text', '-in', signed, '-CAfile',
                                             folder/'permissions_ca.cert.pem', '-purpose', 'any'], stderr=subprocess.PIPE)
            doc = ET.fromstring(plain)
            if kind == 'governance':
                rules = doc.findall('.//domain_rule')
                check(len(rules) == 1, 'ambiguous domain governance')
                rule, = rules
                check(domain_allowed(rule), 'test domain outside governance')
                topics = rule.findall('topic_access_rules/topic_rule')
                check(len(topics) == 1 and topics[0].findtext('topic_expression') == '*', 'ambiguous topic governance')
                expected = {'allow_unauthenticated_participants': 'false', 'enable_join_access_control': 'true'}
                protections = {'enable_read_access_control': 'true', 'enable_write_access_control': 'true',
                               'metadata_protection_kind': 'ENCRYPT', 'data_protection_kind': 'ENCRYPT'}
                check(all(rule.findtext(k)==v for k,v in expected.items()) and
                      all(topics[0].findtext(k)==v for k,v in protections.items()), 'incomplete security governance')
                entry['governance'] = {**expected, **protections, 'rtps_protection_kind': rule.findtext('rtps_protection_kind')}
            else:
                grant, = doc.findall('.//grant')
                check(grant.findtext('default') == 'DENY', 'expected default-deny permissions')
                entry['signedGrantSubject'] = grant.findtext('subject_name')
                rules = [r for r in grant.findall('allow_rule') if domain_allowed(r)]
                allowed = {mode: sorted({t.text for r in rules for t in r.findall(mode+'/topics/topic')})
                           for mode in ('publish', 'subscribe')}
                topics = ('rt/uwvm_test/pubString', 'rt/uwvm_test/subString')
                entry['testTopicGrants'] = {mode: [topic for topic in topics
                                            if any(fnmatch.fnmatchcase(topic,t) for t in patterns)]
                                            for mode,patterns in allowed.items()}
                expected = {'gateway': {'publish':[topics[0]], 'subscribe':[topics[1]]},
                            'witness': {'publish':[topics[1]], 'subscribe':[topics[0]]},
                            'observer': {'publish':[], 'subscribe':[]}}
                check(entry['testTopicGrants'] == expected[enclave], 'wrong signed topic grants for '+enclave)
            entry[kind+'Sha256'] = sha(signed)
        runtime = stage/'runtime/enclaves'/enclave
        runtime.mkdir(mode=0o700, parents=True)
        for filename in ('key.pem', 'cert.pem', 'identity_ca.cert.pem', 'permissions_ca.cert.pem',
                         'governance.p7s', 'permissions.p7s'):
            shutil.copyfile(folder/filename, runtime/filename)
            (runtime/filename).chmod(0o600)
        policies[enclave] = entry
    check(len({p['certificateSha256'] for p in policies.values()}) == 3, 'enclave identities overlap')
    return policies


def domain_allowed(rule):
    return any(int(i.text) == DOMAIN for i in rule.findall('domains/id')) or any(
        int(r.findtext('min', '0')) <= DOMAIN <= int(r.findtext('max', '232')) for r in rule.findall('domains/id_range'))


def stop(proc):
    if proc.poll() is None:
        proc.kill()
        proc.wait()


def environment(spec, secure):
    env = dict(PATH='/usr/bin:/bin', HOME='/tmp',
               LD_LIBRARY_PATH=os.pathsep.join([str(Path(spec['prefix'])/'lib'),
                   str(Path(spec['prefix'])/'lib/x86_64-linux-gnu'), spec['system']]),
               AMENT_PREFIX_PATH=spec['prefix'], ROS_DOMAIN_ID=str(DOMAIN), RMW_IMPLEMENTATION=spec['rmw'],
               ROS_AUTOMATIC_DISCOVERY_RANGE='SYSTEM_DEFAULT',
               ROS_LOG_DIR='/tmp/ros-logs', ROS_SECURITY_ENABLE='true' if secure else 'false')
    if spec['rmw'] == 'rmw_cyclonedds_cpp':
        env['CYCLONEDDS_URI'] = ('<CycloneDDS><Domain><General><Interfaces><NetworkInterface name="lo"/>'
            '</Interfaces><AllowMulticast>false</AllowMulticast></General><Discovery><ParticipantIndex>auto</ParticipantIndex>'
            '<Peers><Peer Address="127.0.0.1"/></Peers></Discovery></Domain></CycloneDDS>')
    else:
        setting = 'FASTDDS_DEFAULT_PROFILES_FILE' if spec['release'] in ('lyrical','kilted') else 'FASTRTPS_DEFAULT_PROFILES_FILE'
        env[setting] = '/work/loopback.xml'
    if secure:
        env.update(ROS_SECURITY_STRATEGY='Enforce', ROS_SECURITY_KEYSTORE='/issued')
    return env


def missing_identity(out, cfg):
    statuses = {}
    for role in ('request', 'echo'):
        spec = cfg['peers'][role]
        path = out/(role+'-no-identity.log')
        cmd = [out/spec['release'], role, secrets.token_hex(24), spec['rmw'], '--ros-args', '--enclave', '/unissued']
        with path.open('wb') as log:
            run = subprocess.run(cmd, env=environment(spec, True), stdout=log, stderr=subprocess.STDOUT, timeout=16)
        content = path.read_text(errors='replace')
        statuses[role] = {'exit':run.returncode, 'logSha256':sha(path), 'diagnosedMissingEnclave':
                          'unissued' in content and ('security' in content.lower() or 'enclave' in content.lower()),
                          'receivedSample': any(line.startswith('received ') for line in content.splitlines())}
    return {'passed':all(s['exit'] != 0 and s['diagnosedMissingEnclave'] and not s['receivedSample']
                         for s in statuses.values()), 'peers':statuses}


def exchange(out, cfg, secure, control=False):
    case_name = 'enforce' if secure else 'plaintext'
    directory = out/(('cyclone-control-' if control else '') + case_name)
    directory.mkdir()
    token = secrets.token_hex(24)
    markers = {'request': 'secure-request-'+token, 'reply': 'secure-reply-'+token}
    roles = ('observer', 'echo', 'request') if secure and not control else ('echo', 'request')
    proc, status = {}, {}
    mapped = {r: set() for r in roles}
    listening = {r: set() for r in roles}
    with ExitStack() as stack:
        for role in roles:
            spec = cfg['peers']['echo' if role == 'echo' else 'request']
            env = environment(spec, secure)
            argv = [out/spec['release'], role, token, spec['rmw'], '--ros-args']
            if secure:
                argv.extend(['--enclave', '/'+ENCLAVES[role]])
            stream = stack.enter_context((directory/(role+'.log')).open('wb'))
            proc[role] = subprocess.Popen(trace_command(directory, role, argv), env=env, stdout=stream, stderr=subprocess.STDOUT)
            stack.callback(stop, proc[role])
            if role == 'observer':
                deadline = time.monotonic()+4
                while time.monotonic() < deadline and proc[role].poll() is None:
                    if b'observer ready' in (directory/'observer.log').read_bytes():
                        break
                    time.sleep(0.04)
        observer_during_request = False
        deadline = time.monotonic() + 17
        while time.monotonic() < deadline and any(p.poll() is None for p in proc.values()):
            if 'observer' in proc and proc['request'].poll() == 0 and proc['observer'].poll() is None:
                observer_during_request = True
            for role, process in proc.items():
                spec = cfg['peers']['echo' if role == 'echo' else 'request']
                expected = set(spec['libraries']) | (set(spec['securityPlugins']) if secure else set())
                pid = app_pid(process)
                if not pid:
                    continue
                listening[role].update(receive_ports(pid))
                try:
                    entries = Path(f'/proc/{pid}/maps').read_text().splitlines()
                except (FileNotFoundError, PermissionError, ProcessLookupError):
                    continue
                mapped[role].update(v[5] for line in entries if len(v := line.split(None, 5)) == 6
                                    and 'x' in v[1] and v[5] in expected)
            time.sleep(0.04)
        status = {r: p.returncode if p.poll() is not None else 'timeout' for r, p in proc.items()}
    observation = summarize(directory, roles, markers)
    logs = {r: (directory/(r+'.log')).read_text(errors='replace') for r in roles}
    exact = all('received '+markers[label] in logs[r].splitlines() for r,label in [('echo','request'),('request','reply')])
    ok = all(rc == 0 for rc in status.values()) and exact and all(
        mapped[r] == (set(spec['libraries']) | (set(spec['securityPlugins']) if secure else set()))
        for r in roles for spec in [cfg['peers']['echo' if r == 'echo' else 'request']])
    secured = {row['sender'] for row in observation['userPortEncryptedSubmessages']}
    paired = {r:sum(row['count'] for row in observation['userPortEncryptedSubmessages']
                    if row['sender'] == r and row['destination'] in listening[other])
              for r,other in [('request','echo'),('echo','request')]}
    denied = None
    if 'observer' in roles:
        denied = (any('rt/uwvm_test/pubString' in line and 'check_create_datareader' in line for line in logs['observer'].splitlines())
                  and 'observer reader=refused samples=0' in logs['observer'].splitlines())
        ok = ok and denied and observer_during_request
    if secure:
        unique_ports = (all(len(ports) == 1 for ports in listening.values()) and
                        len(set.union(*listening.values())) == len(roles))
        ok = ok and unique_ports and all(paired.values()) and not any(
            count for labels in observation['clearMarkers'].values() for count in labels.values())
    else:
        denied = None
        ok = ok and observation['clearMarkers']['request']['request'] > 0 and observation['clearMarkers']['echo']['reply'] > 0 and not secured
    return {'passed': ok, 'token': token, 'exits': status, 'exactCallbacks': exact,
            'observerRelease': cfg['peers']['request']['release'] if 'observer' in roles else None, 'securityEnabled': secure, 'securityStrategy': 'Enforce' if secure else None,
            'unauthorizedReaderDenied': denied, 'observerAliveAtRequestCompletion': observer_during_request if 'observer' in roles else None, 'capture': observation,
            'ownedUserUnicastPorts':{r:sorted(ports) for r,ports in listening.items()},
            'encryptedSubmessagesToAuthorizedPeer':paired, 'mappedLibraries': {r: sorted(items) for r, items in mapped.items()},
            'peers': {r: {'logSha256':sha(directory/(r+'.log')), 'rosErrors':logs[r].count('[ERROR]'),
                          'rosWarnings':logs[r].count('[WARN]'), 'rcutilsErrorReports':logs[r].count('rcutils_set_error_state()')}
                      for r in roles}}


def worker():
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    check(socket.if_nameindex() == [(1, 'lo')], 'non-loopback network interface')
    out = Path('/work')
    cfg = json.loads((out/'inputs.json').read_bytes())
    result = {'format': 'uwvm-native-cross-sros2-v1', 'inputsSha256': sha(out/'inputs.json'),
              'complete': False, 'passed': False, 'cases': {}}
    result['cases']['unissuedEnclave'] = missing_identity(out, cfg)
    save(out/'results.json', result)
    for secure in (False, True):
        result['cases']['enforce' if secure else 'plaintext'] = exchange(out, cfg, secure)
        save(out/'results.json', result)
    if cfg['peers']['echo']['rmw'] == 'rmw_cyclonedds_cpp':
        control = {'peers':{role:cfg['peers']['echo'] for role in ('request','echo')}}
        for secure in (False, True):
            name = 'cycloneControlEnforce' if secure else 'cycloneControlPlaintext'
            result['cases'][name] = exchange(out, control, secure, control=True)
            save(out/'results.json', result)
    result.update(complete=True, passed=all(c['passed'] for c in result['cases'].values()))
    save(out/'results.json', result)
    return int(not result['passed'])


def main():
    from run_scene_vm import ROOT, verify_gate
    verify_gate()
    check(os.sched_getaffinity(0).issubset({4, 5, 6, 7}), 'wrong qualification CPUs')
    p = argparse.ArgumentParser(description=__doc__)
    for name in ('issuer-rootfs','request-prefix','request-system','echo-prefix','echo-system','loopback-profile','output'):
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--cyclone-echo', action='store_true',
                   help='Use native Cyclone DDS for the echo and repeat with two Cyclone participants using the same PKI')
    p.add_argument('--cyclone-plan', type=Path, help='Signed archive package plan for the Cyclone echo')
    p.add_argument('--archive-keyring', type=Path, help='Public ROS archive keyring for the package plan')
    a = p.parse_args()
    if a.cyclone_echo and (not a.cyclone_plan or not a.archive_keyring):
        p.error('--cyclone-echo requires --cyclone-plan and --archive-keyring')
    out = a.output.resolve()
    out.mkdir(mode=0o700, parents=True, exist_ok=False)
    profile = a.loopback_profile.resolve(strict=True)
    profiles = ET.parse(profile)
    check([e.text for e in profiles.findall('.//{*}transport_descriptor/{*}type')] == ['UDPv4'] and
          [e.text for e in profiles.findall('.//{*}interfaceWhiteList/{*}address')] == ['127.0.0.1'] and
          [e.text for e in profiles.findall('.//{*}participant/{*}rtps/{*}useBuiltinTransports')] == ['false'],
          'expected the test loopback-only UDPv4 transport')
    (out/'loopback.xml').write_bytes(profile.read_bytes())
    policy = ROOT/'test/0073.ros-tests/support/source/ros_security/sros2-policy.xml'
    peers, mounts = {}, set()
    with tempfile.TemporaryDirectory(dir='/dev/shm', prefix='cross-sros2-') as temp:
        policies = issue(a.issuer_rootfs.resolve(strict=True), policy, Path(temp), out)
        for role in ('request', 'echo'):
            prefix = getattr(a, role+'_prefix').resolve(strict=True)
            system = getattr(a, role+'_system').resolve(strict=True)
            check(prefix.name in ('jazzy','humble','kilted','lyrical'), 'unsupported native release')
            command = build(prefix, system, out/prefix.name)
            cyclone = role == 'echo' and a.cyclone_echo
            rmw = 'rmw_cyclonedds_cpp' if cyclone else 'rmw_fastrtps_cpp'
            multiarch = prefix/'lib/x86_64-linux-gnu'
            vendor = multiarch/'libddsc.so' if cyclone else next(
                p for p in [prefix/'lib/libfastdds.so',prefix/'lib/libfastrtps.so'] if p.is_file())
            libs = [vendor, prefix/'lib'/('lib'+rmw+'.so'),prefix/'lib/librclcpp.so']
            plugins = [multiarch/('libdds_security_'+part+'.so') for part in ('auth','ac','crypto')] if cyclone else []
            peers[role] = {'prefix':str(prefix), 'system':str(system), 'release':prefix.name, 'build':command,
                           'rmw':rmw, 'binarySha256':sha(out/prefix.name), 'libraries':{str(p.resolve()):sha(p) for p in libs},
                           'securityPlugins':{str(p.resolve()):sha(p) for p in plugins}}
            if cyclone:
                from run_archived_dds_peer import signed_packages, match_payloads
                plan, packages = signed_packages(a.cyclone_plan.resolve(strict=True), a.archive_keyring.resolve(strict=True))
                check(plan['release'] == prefix.name, 'unexpected Cyclone package release')
                loaded = {**peers[role]['libraries'], **peers[role]['securityPlugins']}
                peers[role]['signedArchive'] = {
                    'inReleaseSha256':plan['inReleaseSha256'], 'indexSha256':plan['indexSha256'],
                    'signer':plan['fingerprint'], 'packages':{n:plan['packages'][n]['sha256'] for n in packages},
                    'members':match_payloads(prefix, packages, loaded)}
            mounts.update([prefix,system])
            mounts.update(p.resolve(strict=True) for p in system.iterdir() if p.is_symlink() and p.is_file()
                          and not p.resolve().is_relative_to(system))
        check(peers['request']['release'] != peers['echo']['release'], 'two releases required')
        issuer_manifest = a.issuer_rootfs/'opt/ros/jazzy/share/sros2/package.xml'
        inputs = {'peers':peers, 'issuer':{'release':'jazzy', 'sros2Version':ET.parse(issuer_manifest).findtext('version'),
                  'manifestSha256':sha(issuer_manifest)}, 'policySha256':sha(out/'policy.xml'), 'enclaves':policies, 'domain':DOMAIN,
                  'loopbackProfileSha256':sha(profile), 'sourceSha256':{p.name:sha(p) for p in
                  [HERE/'participant.cpp', HERE/'wire.py', HERE/'run.py',policy]}}
        save(out/'inputs.json', inputs)
        cmd = ['/usr/bin/bwrap','--die-with-parent','--new-session','--unshare-net','--unshare-ipc',
               '--unshare-pid','--cap-drop','ALL','--ro-bind','/usr','/usr',
               '--symlink','usr/lib','/lib','--symlink','usr/lib64','/lib64','--symlink','usr/bin','/bin','--dev','/dev',
               '--proc','/proc','--tmpfs','/tmp','--ro-bind',HERE,'/fixture','--bind',out,'/work','--ro-bind',Path(temp)/'runtime','/issued']
        for path in sorted(mounts):
            cmd.extend(['--ro-bind', path, path])
        cmd += ['--clearenv','--setenv','PATH','/usr/bin:/bin','--chdir','/work','/usr/bin/python3','-I','-B',
                '/fixture/run.py','--worker']
        with (out/'namespace.log').open('wb') as log:
            return subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT, timeout=135 if a.cyclone_echo else 75).returncode


if __name__ == '__main__':
    sys.path.insert(0, str(HERE.parent))
    sys.exit(worker() if sys.argv[1:] == ['--worker'] else main())
