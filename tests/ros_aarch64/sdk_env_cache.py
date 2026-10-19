"""Capture one real SDK login-shell environment and reuse it for native startup.

This optional test adapter removes repeated setup work from the observer's
unchanged data deadline. No native binary, Guest workload or QoS is changed.
Environment values outside SDK/runtime settings are hashed in reports.
"""
import hashlib
import json
import os
from pathlib import Path
import resource
import signal
import subprocess
import threading
import time

from runtime import digest, require_arm
from rebuild_affected import rooted


CAPTURE = """import sys
sys.dont_write_bytecode=True
import json,os,resource,time
mask=os.umask(0);os.umask(mask)
limits={name:list(resource.getrlimit(getattr(resource,name))) for name in dir(resource) if name.startswith('RLIMIT_')}
print(json.dumps({'environment':dict(os.environ),'cwd':os.getcwd(),'umask':mask,'limits':limits},sort_keys=True),flush=True)
time.sleep(.10)
"""


def fingerprint(value):
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(',', ':')).encode()).hexdigest()


def limits():
    return {name: list(resource.getrlimit(getattr(resource, name)))
            for name in dir(resource) if name.startswith('RLIMIT_')}


def environment_record(values):
    public = {'PATH', 'LD_LIBRARY_PATH', 'PYTHONPATH', 'AMENT_PREFIX_PATH', 'CMAKE_PREFIX_PATH',
              'COLCON_PREFIX_PATH', 'PKG_CONFIG_PATH', 'ROS_PACKAGE_PATH', 'ROSLISP_PACKAGE_DIRECTORIES',
              'PWD', 'OLDPWD', 'SHLVL', '_', 'LANG', 'LC_ALL', 'HOME',
              'ROS_VERSION', 'ROS_DISTRO', 'ROS_PYTHON_VERSION', 'ROS_MASTER_URI', 'ROS_IP',
              'ROS_DOMAIN_ID', 'ROS_SECURITY_ENABLE', 'ROS_SECURITY_STRATEGY', 'ROS_SECURITY_KEYSTORE',
              'RMW_IMPLEMENTATION', 'FASTRTPS_DEFAULT_PROFILES_FILE', 'FASTDDS_DEFAULT_PROFILES_FILE',
              'RMW_FASTRTPS_USE_QOS_FROM_XML', 'RMW_FASTRTPS_PUBLICATION_MODE', 'ROS_LOCALHOST_ONLY',
              'ROS_DISCOVERY_SERVER', 'FASTDDS_BUILTIN_TRANSPORTS'}
    return {'sha256': fingerprint(values), 'keys': sorted(values),
            'sdk_values': {key: value for key, value in values.items()
                           if key in public},
            'other_value_sha256': {key: hashlib.sha256(value.encode()).hexdigest()
                                   for key, value in values.items()
                                   if key not in public}}


def setup_inputs(root, version):
    distro = 'noetic' if version == 1 else 'jazzy'
    base = root/'opt/ros'/distro
    candidates = {path for path in base.rglob('*')
                  if path.suffix in ('.bash', '.sh', '.py', '.dsv') and '__pycache__' not in path.parts}
    candidates.update(root/name for name in ('etc/profile', 'etc/bash.bashrc', 'root/.bash_profile',
                                             'root/.bash_login', 'root/.profile', 'root/.bashrc'))
    candidates.update((root/'etc/profile.d').glob('*'))
    inputs = {}
    for path in sorted(candidates):
        relative = str(path.relative_to(root))
        if path.is_symlink():
            resolved = rooted(root, '/'+relative)
            inputs[relative] = {'symlink': os.readlink(path),
                                'resolved_sha256': digest(resolved) if resolved.is_file() else None}
        elif path.is_file():
            inputs[relative] = {'sha256': digest(path), 'bytes': path.stat().st_size}
    if 'opt/ros/'+distro+'/setup.bash' not in inputs:
        raise RuntimeError('missing exact SDK setup input')
    return inputs


class SdkEnvironmentCache:
    def __init__(self, runner, evidence, original_popen):
        self.runner, self.evidence, self.popen = runner, evidence, original_popen
        self.entries, self.rows, self.launches, self.monitors = {}, [], [], []
        self.expected_environment = {}
        self.out = evidence.out/'sdk-environment-cache'
        self.out.mkdir(mode=0o700, exist_ok=False)

    def capture(self, version):
        root = self.runner.roots[version]
        distro = 'noetic' if version == 1 else 'jazzy'
        before = setup_inputs(root, version)
        caller = dict(self.runner.env)
        caller_limits = limits()
        bash = self.evidence.target('sdk-cache'+str(version)+':bash', rooted(root, '/bin/bash'))
        python = self.evidence.target('sdk-cache'+str(version)+':python', rooted(root, '/usr/bin/python3'))
        name = 'sdk-environment-cache-ros'+str(version)+'/'
        command = ['chroot', str(root), '/bin/bash', '-lc',
                   'source /opt/ros/'+distro+'/setup.bash && exec "$@"',
                   'uwvm-ros-test', '/usr/bin/python3', '-c', CAPTURE]
        row = {'version': version, 'root': str(root), 'command': command, 'status': 'RUNNING',
               'caller_environment': environment_record(caller), 'bash': bash, 'python': python,
               'caller_resource_limits': caller_limits,
               'setup_inputs': before, 'setup_inputs_sha256': fingerprint(before)}
        self.rows.append(row)
        begun = time.monotonic()
        process = self.popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                            env=caller, start_new_session=True)
        row['pid'] = process.pid
        self.evidence.start(process, name, command)
        try:
            output, error = process.communicate(timeout=30)
            row['returncode'] = process.returncode
            if process.returncode:
                raise RuntimeError('real SDK setup environment capture failed')
            parsed = json.loads(output.decode())
            values = parsed['environment']
            if not isinstance(values, dict) or not all(isinstance(k, str) and isinstance(v, str) for k, v in values.items()):
                raise RuntimeError('SDK setup did not return an exact string environment')
            if parsed['cwd'] != '/' or setup_inputs(root, version) != before:
                raise RuntimeError('SDK setup source or chroot cwd changed during environment capture')
            required = ['sdk-cache'+str(version)+':bash', 'sdk-cache'+str(version)+':python']
            self.evidence.check(required, name)
            observed = {target for sample in self.evidence.samples.values()
                        if sample['name'] == name and sample['ancestry'][0] == process.pid
                        for target in sample['mapped_arm_targets']}
            if set(required)-observed:
                raise RuntimeError('SDK environment capture lacks actual ARM bash/Python mappings')
            row.update(status='PASS', captured_environment=environment_record(values),
                       cwd=parsed['cwd'], child_umask=parsed['umask'], resource_limits=parsed['limits'],
                       stderr_sha256=hashlib.sha256(error).hexdigest(),
                       scope='Same native SDK login shell/setup and caller environment; captured by held ARM Python')
            self.entries[version] = {'root': root, 'caller': caller, 'captured': values,
                'inputs': before, 'umask': parsed['umask'], 'row': row}
        finally:
            if process.poll() is None:
                os.killpg(process.pid, signal.SIGTERM)
                try:
                    process.wait(timeout=3)
                except subprocess.TimeoutExpired:
                    os.killpg(process.pid, signal.SIGKILL);process.wait(timeout=3)
            row['seconds'] = round(time.monotonic()-begun, 6)
            (self.out/('ros'+str(version)+'-capture.json')).write_text(json.dumps(row, indent=2)+'\n')

    def transform(self, command, keyword):
        argv = list(command)
        if len(argv) < 7 or argv[0] != 'chroot':
            return argv, keyword, None
        version = next((v for v, entry in self.entries.items() if argv[1] == str(entry['root'])), None)
        if version is None:
            return argv, keyword, None
        entry = self.entries[version]
        distro = 'noetic' if version == 1 else 'jazzy'
        expected = ['/bin/bash', '-lc', 'source /opt/ros/'+distro+'/setup.bash && exec "$@"', 'uwvm-ros-test']
        if argv[2:6] != expected or not argv[6].startswith('/'):
            return argv, keyword, None
        caller = dict(os.environ if keyword.get('env') is None else keyword['env'])
        row = {'version': version, 'original_command': argv, 'status': 'FALLBACK',
               'caller_environment_sha256': fingerprint(caller), 'original_native_argv': argv[6:]}
        self.launches.append(row)
        if caller != entry['caller']:
            row['reason'] = 'caller environment differs from actual capture'
            row['changed_environment_keys'] = sorted(key for key in set(caller)|set(entry['caller'])
                                                   if caller.get(key) != entry['caller'].get(key))
            return argv, keyword, row
        if keyword.get('preexec_fn') is not None or keyword.get('executable') is not None or 'umask' in keyword:
            row['reason'] = 'caller changes native process launch state'
            return argv, keyword, row
        if limits() != entry['row']['caller_resource_limits'] or entry['row']['resource_limits'] != limits():
            row['reason'] = 'SDK setup or caller changes native resource limits'
            return argv, keyword, row
        if setup_inputs(entry['root'], version) != entry['inputs']:
            row['reason'] = 'SDK setup input key set or bytes differ from capture'
            return argv, keyword, row
        native = require_arm(rooted(entry['root'], argv[6]))
        env = dict(entry['captured'])
        # Bash's exec builtin exports the selected executable in this variable.
        env['_'] = argv[6]
        replacement = ['chroot', argv[1], *argv[6:]]
        updated = dict(keyword, env=env, umask=entry['umask'])
        row.update(status='CACHED', command=replacement, native_artifact=native,
                   actual_environment=environment_record(env), child_umask=entry['umask'],
                   setup_inputs_sha256=entry['row']['setup_inputs_sha256'],
                   scope='Native ELF/argv unchanged; repeated SDK shell setup omitted; all data assertions unchanged')
        self.expected_environment[id(row)] = env
        return replacement, updated, row

    def observe_launch(self, process, row, name, required):
        if row['status'] != 'CACHED':
            return

        def observe():
            while process.poll() is None:
                with self.evidence._lock:
                    samples = list(self.evidence.samples.values())
                for sample in samples:
                    if (sample['name'] != name or sample['ancestry'][0] != process.pid
                            or not set(required).intersection(sample['mapped_arm_targets'])):
                        continue
                    pid = sample['pid']
                    try:
                        proc = Path('/proc')/str(pid)
                        before = (proc/'stat').read_text().rsplit(')', 1)[1].split()
                        tokens = (proc/'environ').read_bytes().split(b'\0')
                        after = (proc/'stat').read_text().rsplit(')', 1)[1].split()
                        if (int(before[19]) != sample['starttime_ticks'] or before[19] != after[19]
                                or before[1] != after[1] or after[0] in ('Z', 'X')):
                            continue
                        values = dict(token.decode().split('=', 1) for token in tokens if b'=' in token)
                        row['actual_arm_initial_environment'] = {'pid': pid,
                            'starttime_ticks': int(after[19]), 'mapped_arm_targets': sample['mapped_arm_targets'],
                            'environment': environment_record(values),
                            'equals_cached_environment': values == self.expected_environment[id(row)],
                            'scope': 'Initial exec environment of actual QEMU process mapping the native ARM ELF; later CPP setenv is separate'}
                        return
                    except (OSError, UnicodeError, ValueError):
                        continue
                time.sleep(.002)

        thread = threading.Thread(target=observe, daemon=True)
        self.monitors.append(thread)
        thread.start()

    def check(self):
        for thread in self.monitors:
            thread.join(timeout=1)
        for row in self.launches:
            if row['status'] == 'CACHED' and not row.get('actual_arm_initial_environment', {}).get('equals_cached_environment'):
                raise RuntimeError('cached native launch lacks matching real ARM process environment: '+str(row.get('pid')))

    def report(self):
        return {'scope': 'Startup environment caching only; native deadlines/count/QoS/Guest pace unchanged',
                'captures': self.rows, 'launches': self.launches,
                'full_non_sdk_environment_values_archived': False}
