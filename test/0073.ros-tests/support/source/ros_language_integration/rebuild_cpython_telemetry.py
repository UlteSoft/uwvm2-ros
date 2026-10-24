#!/usr/bin/env python3
"""Rebuild deleted CPython/WASI inputs in the original bounded Linux owner.

This new reviewed controller executes only explicitly selected official source
helpers and trusted compiler tools. Old archived controllers are never run.
Building an interpreter does not qualify any ROS application or performance.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import resource
import tarfile
import time


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs-toolchain.json').read_bytes())
    controls = root / 'source/test/0073.ros-tests/support/source/ros_language_integration'
    helper = module('reviewed_python_build_owner', controls / 'native_owner.py')
    budget = module('reviewed_python_build_budget', controls / 'cgroup_budget.py')

    class BuildOwner(helper.Owner):
        def guard(self):
            super().guard()
            current = budget.sample(self.group)
            accounting = self.report.setdefault('memory_accounting', {})
            accounting.setdefault('first', current)
            accounting['last'] = current
            if current['admission_bytes'] > accounting.get('highest_admission', {}).get('admission_bytes', -1):
                accounting['highest_admission'] = current
            if current['charged_bytes'] > accounting.get('highest_charged', {}).get('charged_bytes', -1):
                accounting['highest_charged'] = current
            assert os.statvfs(root.parent).f_bfree * os.statvfs(root.parent).f_frsize > 8 << 30
            return current['admission_bytes']

        def admit(self):
            end = time.monotonic() + 60
            while self.guard() >= 60 << 30:
                assert time.monotonic() < end, '4GiB conservative admission margin unavailable'
                time.sleep(.5)

        @staticmethod
        def limits():
            # Trusted interpreter archives exceed the ordinary small-app file
            # limit. Only this compiler lane raises that per-file ceiling;
            # the 3GiB tmpfs, 768MiB RSS and original cgroup bounds remain.
            resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
            resource.setrlimit(resource.RLIMIT_AS, (16 << 30, 16 << 30))
            resource.setrlimit(resource.RLIMIT_CPU, (180, 180))
            resource.setrlimit(resource.RLIMIT_FSIZE, (768 << 20, 768 << 20))
            resource.setrlimit(resource.RLIMIT_NOFILE, (1024, 1024))

    report = {'status': 'RUNNING', 'scope': 'Fresh official CPython3.14.8 native/WASI toolchain build only; no ROS application execution',
              'inputs': inputs, 'make_jobs': 1}
    owner = BuildOwner(root, inputs, report, 'toolchain-rebuild-results.json')
    cwd_entry = root / 'control/build-in-cwd.py'
    cwd_entry.write_text('import os,sys\nos.chdir(sys.argv[1])\nos.execvpe(sys.argv[2],sys.argv[2:],os.environ)\n')

    def run(label, argv, cwd, env=None, deadline=1200):
        owner.admit()
        actor = owner.launch(label, ['/usr/bin/python3', '-I', '-S', '-B', cwd_entry, cwd, *argv],
                             extra=env, deadline=deadline)
        owner.wait(actor)
        print('COMPLETED_CPYTHON_BUILD_STAGE', label, flush=True)
        return actor

    try:
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256']
        total, members = 0, 0
        for archive in inputs['archives']:
            owner.admit()
            with tarfile.open(archive['path'], mode='r|*') as stream:
                for entry in stream:
                    relative = Path(entry.name)
                    assert not relative.is_absolute() and '..' not in relative.parts
                    assert relative.parts[0] == archive['prefix']
                    assert entry.size <= 768 << 20
                    total += entry.size
                    members += 1
                    assert total <= 2 << 30 and members <= 40000
                    owner.guard()
                    stream.extract(entry, path=root / 'tools', filter='data')
        report['extracted_source_bytes'] = total
        report['extracted_members'] = members
        source = root / 'tools/Python-3.14.8'
        sdk = root / 'tools/wasi-sdk-24.0-x86_64-linux'
        assert helper.sha(sdk / 'bin/clang') == '64280d1cf3e55fe73bffe55deed0e61936c84a1b538a5435e50ad8e028eb2e3b'
        env = {'PATH': str(root / 'tools/reference-bin') + ':/usr/bin:/bin',
               'WASI_SDK_PATH': str(sdk), 'SOURCE_DATE_EPOCH': '1790726400',
               'CFLAGS': '-O2 -g', 'PYTHONHASHSEED': '0', 'PYTHONDONTWRITEBYTECODE': '1',
               'MAKEFLAGS': '-j1'}
        wasi_helper = source / 'Tools/wasm/wasi'
        run('configure-native', ['/usr/bin/python3', '-I', '-S', '-B', wasi_helper,
            'configure-build-python', '--', '--without-ensurepip', '--disable-test-modules'], source, env)
        native_roots = [path for path in (source / 'cross-build').iterdir()
                        if path.is_dir() and path.name != 'wasm32-wasip1']
        assert len(native_roots) == 1
        native = native_roots[0]
        run('make-native', ['/usr/bin/make', '-C', native, '-j1', 'all'], source, env)
        actor = run('native-version', [native / 'python', '--version'], source, env, deadline=45)
        assert owner.text(actor) == 'Python 3.14.8\n'
        run('wasi-sdk-version', [sdk / 'bin/clang', '--version'], source, env, deadline=45)
        wasi_env = dict(env, LDFLAGS='-Wl,--threads=1')
        run('configure-wasi', ['/usr/bin/python3', '-I', '-S', '-B', wasi_helper,
            'configure-host', '--wasi-sdk', sdk, '--', '--without-ensurepip', '--disable-test-modules'], source, wasi_env)
        target = source / 'cross-build/wasm32-wasip1'
        run('make-wasi', ['/usr/bin/make', '-C', target, '-j1', 'all'], source, wasi_env)
        products = {'source': str(source), 'target': str(target), 'native_python': str(native / 'python'),
                    'wasi_sdk': str(sdk), 'optimization': 'New O2-g native/target interpreter, serial make and one WASI linker thread',
                    'files': []}
        for path in (native / 'python', native / 'pyconfig.h', target / 'pyconfig.h',
                     target / 'Makefile', target / 'libpython3.14.a', target / 'python.wasm'):
            assert path.is_file()
            products['files'].append({'path': str(path), 'bytes': path.stat().st_size, 'sha256': helper.sha(path)})
        (root / 'out/toolchain-products.json').write_text(json.dumps(products, indent=2) + '\n')
        for pin in inputs['pins']:
            path = Path(pin['path'])
            assert path.stat().st_size == pin['bytes'] and helper.sha(path) == pin['sha256']
        report['source_and_archive_inputs_preserved'] = True
        report['status'] = 'FRESH_CPYTHON_3148_NATIVE_WASI_TOOLCHAIN_BUILT_NO_ROS_EXECUTED'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        owner.close()


if __name__ == '__main__':
    main()
