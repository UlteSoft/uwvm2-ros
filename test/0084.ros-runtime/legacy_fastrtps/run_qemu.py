"""Boot a NIC-less Linux guest to compare signed Fast RTPS String examples with Jazzy."""
import argparse
import base64
import io
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tarfile
import tempfile

from packages import SUITES, archives, signed_plan
from run import MODES, write_json
from run_scene_vm import sha, verify_gate

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
HOST_TOOLS = ('/usr/sbin/ip', '/usr/bin/setpriv', '/usr/bin/dpkg-deb', '/usr/bin/gpgv', '/usr/bin/tar')


def copy(source, target):
    target.parent.mkdir(parents=True, exist_ok=True)
    if source.is_dir():
        omit = shutil.ignore_patterns('__pycache__', '*.pyc', 'tests', 'test', 'site-packages', 'dist-packages')
        shutil.copytree(source, target, dirs_exist_ok=True, symlinks=True, ignore=omit)
    else:
        shutil.copy2(source, target)


def runtime_libraries(root, native_inputs=("/fixture/jazzy-string",)):
    cache = {}
    for line in subprocess.check_output(['/sbin/ldconfig', '-p'], text=True).splitlines():
        item = re.match(r'\s*(\S+) \(libc6,x86-64[^)]*\) => (\S+)', line)
        if item:
            cache.setdefault(item[1], Path(item[2]))
    inspect = [root / name.lstrip('/') for name in (*HOST_TOOLS, '/usr/bin/python3', *native_inputs)]
    inspect.extend(root.glob('usr/lib/python3.*/lib-dynload/*.so'))
    inspect.extend(root.glob('opt/ros/jazzy/lib/*.so*'))
    inspect.extend(root.glob('syslib/*/*.so*'))
    needed = {'libstdc++.so.6', 'libgcc_s.so.1', 'librt.so.1', 'libpthread.so.0', 'libdl.so.2',
              'libresolv.so.2', 'libnss_files.so.2', 'libnss_dns.so.2', 'libsqlite3.so.0'}
    seen = set()
    while inspect:
        output = subprocess.run(['/usr/bin/readelf', '-d', *inspect], capture_output=True, text=True).stdout
        needed.update(re.findall(r'\(NEEDED\)\s+Shared library: \[([^]]+)\]', output))
        inspect = []
        for name in sorted(needed - seen):
            if name in cache:
                target = root / 'usr/lib/x86_64-linux-gnu' / name
                copy(cache[name], target)
                inspect.append(target)
        seen.update(needed)


def guest_files(args, root):
    for name in ('bin', 'usr/bin', 'usr/lib', 'lib64', 'dev', 'sys', 'proc', 'tmp', 'etc'):
        (root / name).mkdir(parents=True, exist_ok=True)
    (root / 'lib').symlink_to('usr/lib')
    (root / 'etc/passwd').write_text('root:x:0:0:root:/tmp:/bin/sh\n')
    (root / 'etc/group').write_text('root:x:0:\n')
    (root / 'etc/hosts').write_text('127.0.0.1 localhost\n')
    copy(args.busybox, root / 'bin/busybox')
    for app in ('sh', 'xz', 'gzip', 'zstd', 'cat', 'uname', 'hostname', 'dirname', 'true'):
        (root / 'bin' / app).symlink_to('busybox')
    (root / 'bin/tar').symlink_to('/usr/bin/tar')
    for name in (*HOST_TOOLS, '/lib64/ld-linux-x86-64.so.2', '/usr/bin/python3'):
        copy(Path(name), root / name.lstrip('/'))
    stdlib = Path('/usr/lib') / ('python' + '.'.join(map(str, sys.version_info[:2])))
    copy(stdlib, root / stdlib.relative_to('/'))
    if args.isolated_ethernet_module:
        copy(args.isolated_ethernet_module, root / 'devices/e1000.ko')
    copy(HERE / 'guest_init.sh', root / 'init')
    (root / 'init').chmod(0o755)
    fixture = root / 'fixture'
    files = [HERE / name for name in ('guest.py', 'run.py', 'packages.py')]
    files.extend((HERE.parent / 'run_scene_vm.py', HERE.parent / 'run_archived_dds_peer.py',
                  ROOT / 'tools/ros_scene/rmw_native_wire.py'))
    for path in files:
        copy(path, fixture / path.relative_to(ROOT))
    copy(args.witness, fixture / 'jazzy-string')
    copy(args.keyring, root / 'input/keyring.gpg')
    copy(args.jazzy_profile, root / 'input/loopback.xml')
    for name in ('share', 'lib'):
        copy(args.jazzy_prefix / name, root / 'opt/ros/jazzy' / name)
    system = {'xenial': args.xenial_system, 'bionic': args.bionic_system, 'jazzy': args.jazzy_system}
    for suite in {SUITES[release] for release in args.release} | {'jazzy'}:
        for path in system[suite].glob('*.so*'):
            if path.is_file():
                copy(path, root / 'syslib' / suite / path.name)
    inputs = {}
    for release in args.release:
        suite = SUITES[release]
        index = args.indexes / ('ros-' + suite)
        plan_path = args.inputs / release / 'signed-plan.json'
        plan = signed_plan(release, index, args.keyring)
        if json.loads(plan_path.read_bytes()) != plan:
            raise ValueError('supplied plan disagrees with signed index: ' + release)
        for pkg, _ in archives(plan, plan_path.parent / 'debs').values():
            copy(pkg, root / 'input' / release / 'debs' / pkg.name)
        for name in ('InRelease', 'Packages.gz'):
            copy(index / name, root / 'input' / suite / name)
        copy(plan_path, root / 'input' / release / 'signed-plan.json')
        inputs[release] = sha(plan_path)
    write_json(root / 'input/releases.json', args.release)
    runtime_libraries(root)
    return inputs


def pack(root, archive, log):
    with archive.open('wb') as output, log.open('wb') as errors:
        with subprocess.Popen(['/usr/bin/find', '.', '-print0'], cwd=root, stdout=subprocess.PIPE) as names:
            cmd = ['/usr/bin/cpio', '--null', '-o', '-H', 'newc', '--owner=0:0']
            with subprocess.Popen(cmd, cwd=root, stdin=names.stdout, stdout=subprocess.PIPE, stderr=errors) as cpio:
                names.stdout.close()
                subprocess.run(['/usr/bin/gzip', '-6c'], stdin=cpio.stdout, stdout=output, stderr=errors, check=True)
                cpio.stdout.close()
                if cpio.wait() or names.wait():
                    raise RuntimeError('initramfs packing failed')


def extract_reports(serial, target, releases):
    text = serial.read_text(errors='replace').replace('\r', '')
    start = 'UWVM_REPORTS_BEGIN\n'
    if start not in text:
        raise ValueError('guest did not write a report; see serial.log')
    data = text.split(start, 1)[1].split('\nUWVM_REPORTS_END\n', 1)
    if len(data) != 2:
        raise ValueError('guest did not finish writing results')
    archive = base64.b64decode(''.join(data[0].split()), validate=True)
    allowed = {Path(release) / name for release in releases for name in ('environment.json', 'worker.log', 'results.json')}
    allowed.update(Path(release) / mode / name for release in releases for mode in MODES
                   for name in ('case.json', 'subscriber.log', 'publisher.log'))
    with tarfile.open(fileobj=io.BytesIO(archive), mode='r:gz') as tar:
        for member in tar:
            path = Path(member.name)
            if member.isdir():
                continue
            if not member.isfile() or path not in allowed or member.size > 4 * 1024 * 1024:
                raise ValueError(f'unexpected guest output {member.name}')
            output = target / path
            output.parent.mkdir(parents=True, exist_ok=True)
            with output.open('xb') as stream:
                shutil.copyfileobj(tar.extractfile(member), stream)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('release', choices=SUITES, nargs='+')
    for name in ('inputs', 'indexes', 'keyring', 'xenial-system', 'bionic-system', 'jazzy-system', 'jazzy-prefix',
                 'jazzy-profile', 'witness', 'qemu', 'kernel', 'busybox', 'bios', 'firmware-dir', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--accel', choices=('kvm', 'tcg'), default='tcg')
    parser.add_argument('--isolated-ethernet-module', type=Path,
                        help='Matching trusted e1000.ko for a single guest-only QEMU hub port')
    parser.add_argument('--keep-initramfs', action='store_true')
    args = parser.parse_args()
    verify_gate()
    if not os.sched_getaffinity(0).issubset({4, 5, 6, 7}) or Path(sys.executable).resolve() != Path('/usr/bin/python3').resolve():
        raise RuntimeError('use the system Python under the designated serial qualification gate')
    if len(set(args.release)) != len(args.release):
        parser.error('duplicate release')
    args.output.mkdir(mode=0o700, parents=True, exist_ok=False)
    report = {'format': 'uwvm-legacy-fastrtps-qemu-v1', 'complete': False, 'nativeOnly': True, 'releases': args.release,
              'scope': 'x86_64 Linux, native String on /topic', 'outcomes': {}, 'rosGraphInteroperability': 'unverified'}
    report['network'] = 'isolated-guest-ethernet-hub' if args.isolated_ethernet_module else 'guest-loopback-only'
    receipt = args.output / 'receipt.json'
    write_json(receipt, report)
    with tempfile.TemporaryDirectory(dir='/dev/shm', prefix='legacy-ros-') as scratch:
        root = Path(scratch) / 'root'
        report['plans'] = guest_files(args, root)
        archive = Path(scratch) / 'initrd.cpio.gz'
        pack(root, archive, args.output / 'pack.log')
        report['inputs'] = {name: sha(getattr(args, name)) for name in
                            ('qemu', 'kernel', 'busybox', 'bios', 'keyring', 'jazzy_profile', 'witness')}
        report['fixtureSha256'] = sha(__file__)
        report['initramfsSha256'] = sha(archive)
        if args.keep_initramfs:
            shutil.copyfile(archive, args.output / archive.name)
        network = ['-nic', 'none']
        if args.isolated_ethernet_module:
            report['inputs']['e1000Module'] = sha(args.isolated_ethernet_module)
            network = ['-netdev', 'hubport,id=fixture,hubid=0', '-device', 'e1000,netdev=fixture,mac=52:54:00:00:00:01']
        command = [args.qemu, '-nodefaults', '-machine', 'q35', '-accel', args.accel, '-smp', '2', '-m', '2048',
                   *network, '-display', 'none', '-no-reboot', '-serial', 'stdio', '-monitor', 'none',
                   '-bios', args.bios, '-L', args.firmware_dir, '-kernel', args.kernel, '-initrd', archive,
                   '-append', 'console=ttyS0 loglevel=2 rdinit=/init panic=-1']
        report['argv'] = list(map(str, command))
        write_json(receipt, report)
        serial = args.output / 'serial.log'
        try:
            with serial.open('wb') as log:
                vm = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=90 + 80 * len(args.release))
            report['exit'] = vm.returncode
            extract_reports(serial, args.output / 'reports', args.release)
            for release in args.release:
                result = args.output / 'reports' / release / 'results.json'
                observation = json.loads(result.read_bytes())
                report['outcomes'][release] = {key: observation[key] for key in
                                               ('complete', 'nativePairPassed', 'crossReleaseBothDirections')}
                report['outcomes'][release]['receiptSha256'] = sha(result)
            report['complete'] = vm.returncode == 0 and all(r['complete'] for r in report['outcomes'].values())
        except (ValueError, IndexError, OSError, KeyError, subprocess.TimeoutExpired, tarfile.TarError) as error:
            report['error'] = str(error)
        report['serialSha256'] = sha(serial)
        write_json(receipt, report)
    return 0 if report['complete'] and all(r['nativePairPassed'] and r['crossReleaseBothDirections']
                                            for r in report['outcomes'].values()) else 1


if __name__ == '__main__':
    sys.exit(main())
