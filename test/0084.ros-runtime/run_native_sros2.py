#!/usr/bin/env python3
"""Exercise the original Jazzy SROS2 witness against a selected native gateway."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import shutil
import socket
import subprocess
import sys
from xml.etree import ElementTree

PASS = ('PASS actual FastDDS SROS2 Enforce, owned signed enclave identities, '
        'exact-resource bidirectional communication and denied native DDS publisher')
PUBLIC_ARTIFACTS = ('cert.pem', 'permissions.p7s', 'governance.p7s')


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def run_witness():
    root = Path('/tmp')
    out = root / 'evidence'
    keys = root / 'keystore'
    os.umask(0o077)
    assert socket.if_nameindex() == [(1, 'lo')]
    os.environ.update(ROS_DOMAIN_ID='74', RMW_IMPLEMENTATION='rmw_fastrtps_cpp',
                      ROS_SECURITY_ENABLE='true', ROS_SECURITY_STRATEGY='Enforce',
                      ROS_LOG_DIR='/tmp/ros-logs')
    prefix = Path('/opt/ros/jazzy')
    report = {
        'format': 'uwvm-native-sros2-enforce-v1', 'complete': False,
        'distribution': 'jazzy', 'rmw': 'rmw_fastrtps_cpp', 'network': 'isolated loopback',
        'domain': 74, 'enclaves': ['/gateway', '/witness'], 'type': 'std_msgs/msg/String',
        'packages': {}, 'inputs': {}, 'steps': [],
    }
    for pkg in ('sros2', 'rclcpp', 'rmw_fastrtps_cpp'):
        manifest = prefix / 'share' / pkg / 'package.xml'
        report['packages'][pkg] = {'version': ElementTree.parse(manifest).findtext('version'),
                                   'manifestSha256': sha(manifest)}
    report['libraries'] = {name: sha(prefix/'lib'/name) for name in
                           ('librclcpp.so', 'librmw_fastrtps_cpp.so', 'libfastrtps.so')}
    for item in ('runner.py', 'probe', 'gateway', 'policy.xml'):
        report['inputs'][item] = sha(root/item)

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2) + '\n')

    def step(name, command):
        log = out / (name + '.log')
        with log.open('wb') as stream:
            run = subprocess.run(command, stdout=stream, stderr=subprocess.STDOUT)
        report['steps'].append({'name': name, 'exit': run.returncode, 'logSha256': sha(log)})
        save()
        run.check_returncode()

    save()
    step('issue', ['ros2', 'security', 'generate_artifacts', '-k', keys, '-p', root/'policy.xml'])
    # Match the original gateway's regular-file credential checks.
    for link in (keys/'enclaves').rglob('*'):
        if link.is_symlink():
            target = link.resolve(strict=True)
            assert target.is_relative_to(keys) and target.is_file()
            link.unlink()
            shutil.copyfile(target, link)
    for key in keys.rglob('*.pem'):
        if 'key' in key.name:
            key.chmod(0o600)
    report['publicArtifacts'] = {enclave: {name: sha(keys/'enclaves'/enclave[1:]/name)
                                           for name in PUBLIC_ARTIFACTS} for enclave in report['enclaves']}
    assert report['publicArtifacts']['/witness']['cert.pem'] != report['publicArtifacts']['/gateway']['cert.pem']
    save()
    step('enforce', [root/'probe', root/'gateway', keys])
    lines = (out/'enforce.log').read_text().splitlines()
    assert PASS in lines
    denial = 'rt/uwvm_test/not_permitted topic not found in allow rule'
    assert any(denial in line and 'check_create_datawriter' in line for line in lines)
    report.update(complete=True, denialReason=denial, witnessReceived='/uwvm_test/pubString',
                  gatewayReceived='/uwvm_test/subString', witnessPublisherDenied='/uwvm_test/not_permitted')
    save()


def main():
    from run_scene_vm import ROOT, verify_gate
    p = argparse.ArgumentParser(description=__doc__)
    for name in ('rootfs', 'gateway', 'probe', 'output'):
        p.add_argument('--' + name, type=Path, required=True)
    args = p.parse_args()
    verify_gate()
    assert os.sched_getaffinity(0).issubset({4, 5, 6, 7})
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    out = args.output.resolve()
    out.mkdir(mode=0o700, parents=True, exist_ok=False)
    cmd = ['bwrap', '--die-with-parent', '--unshare-net', '--unshare-ipc', '--unshare-pid',
           '--ro-bind', args.rootfs.resolve(strict=True), '/', '--proc', '/proc', '--dev', '/dev',
           '--tmpfs', '/tmp', '--dir', '/tmp/home', '--bind', out, '/tmp/evidence']
    policy = ROOT/'test/0073.ros-tests/support/source/ros_security/sros2-policy.xml'
    for path, name in [(Path(__file__), 'runner.py'), (policy, 'policy.xml'),
                       (args.gateway, 'gateway'), (args.probe, 'probe')]:
        cmd.extend(['--ro-bind', path.resolve(strict=True), '/tmp/' + name])
    cmd.extend(['--chdir', '/tmp', '--clearenv', '--setenv', 'HOME', '/tmp/home',
                '--setenv', 'PATH', '/usr/local/bin:/usr/bin:/bin', '--setenv', 'LANG', 'C.UTF-8',
                '/bin/bash', '-c', '. /opt/ros/jazzy/setup.bash && exec /usr/bin/python3 -B /tmp/runner.py --worker'])
    subprocess.run(cmd, timeout=150, check=True)


if __name__ == '__main__':
    if sys.argv[1:] == ['--worker']:
        run_witness()
    else:
        main()
