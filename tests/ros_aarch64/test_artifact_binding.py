#!/usr/bin/env python3
"""Pure report/filesystem models; never compile or execute an ARM/ROS program."""
import copy
import hashlib
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import artifacts
import run_all


def file_model(path):
    path = Path(path)
    stat = path.stat()
    return {'path': str(path), 'sha256': hashlib.sha256(path.read_bytes()).hexdigest(),
            'device': stat.st_dev, 'inode': stat.st_ino, 'bytes': stat.st_size}


class BindingModels(unittest.TestCase):
    def test_exact_source_and_independent_profiles(self):
        expected = 'a'*64
        worker = {'status': 'PASS', 'source_manifest_sha256': expected, 'profiles': [
            {'profile': name, 'status': 'PASS'} for name in ('none', 'ros1', 'ros2', 'dual')]}
        gateway = {'status': 'PASS', 'source_manifest_sha256': expected,
                   'targets': {str(i): {} for i in range(18)}}
        run_all.validate_product_source(worker, gateway, expected)
        for side in (0, 1):
            for value in ('b'*64, None):
                with self.subTest(side=side, binding=value):
                    w, g = copy.deepcopy(worker), copy.deepcopy(gateway)
                    (w, g)[side]['source_manifest_sha256'] = value
                    with self.assertRaises(RuntimeError):
                        run_all.validate_product_source(w, g, expected)
        for change in ('missing', 'duplicate', 'failed', 'unqualified'):
            with self.subTest(profile=change):
                w = copy.deepcopy(worker)
                if change == 'missing':
                    w['profiles'].pop()
                elif change == 'duplicate':
                    w['profiles'][-1]['profile'] = 'ros2'
                elif change == 'failed':
                    w['profiles'][-1]['status'] = 'FAIL'
                else:
                    w['profiles'][-1].pop('status')
                with self.assertRaises(RuntimeError):
                    run_all.validate_product_source(w, gateway, expected)

    def test_canonical_views(self):
        kinds = ('gateway', 'native-probe', 'resources-probe', 'tf-probe', 'action-probe',
                 'perf-probe', 'ipc-service-probe', 'five-module-probe')
        with tempfile.TemporaryDirectory(prefix='uwvm-view-model-') as directory:
            root = Path(directory)
            report = {'targets': {}}
            for version, distro in ((1, 'noetic'), (2, 'jazzy')):
                canonical = root/'sdk/gateway'/(distro+'-rootfs')/'tmp'/f'ros{version}-build'
                canonical.mkdir(parents=True)
                for kind in kinds+(('lifecycle-probe', 'secure-probe') if version == 2 else ()):
                    name = f'uwvm-ros{version}-{kind}'
                    source = root/'qualified'/name
                    source.parent.mkdir(exist_ok=True)
                    source.write_text('nonexecutable filesystem model: '+name)
                    (canonical/name).symlink_to(source)
                    report['targets'][name] = file_model(source)
            with patch.object(run_all, 'require_arm', file_model):
                self.assertEqual(len(run_all.validate_native_views(root/'sdk', report)), 18)
                bad = copy.deepcopy(report)
                bad['targets']['uwvm-ros2-gateway']['sha256'] = 'c'*64
                with self.assertRaises(RuntimeError):
                    run_all.validate_native_views(root/'sdk', bad)
                bad = copy.deepcopy(report)
                bad['targets']['unexpected'] = bad['targets'].pop('uwvm-ros2-gateway')
                with self.assertRaises(RuntimeError):
                    run_all.validate_native_views(root/'sdk', bad)
                alias = root/'sdk/gateway/jazzy-rootfs/tmp/ros2-build/uwvm-ros2-gateway'
                original = Path(report['targets']['uwvm-ros2-gateway']['path'])
                identical = root/'identical-unreported-product'
                identical.write_bytes(original.read_bytes())
                alias.unlink()
                alias.symlink_to(identical)
                with self.assertRaises(RuntimeError):
                    run_all.validate_native_views(root/'sdk', report)

    def test_pool_aliases_and_nonproduct_copies(self):
        with tempfile.TemporaryDirectory(prefix='uwvm-pool-model-') as directory:
            root = Path(directory)
            sources = []
            for profile in ('dual', 'none'):
                source = root/profile/'uwvm'
                source.parent.mkdir()
                source.write_text('nonexecutable filesystem model: '+profile)
                sources.append(source)
            canonical = root/'canonical-worker'
            canonical.symlink_to(sources[0])
            native = root/'uwvm-ros2-gateway'
            native.write_text('nonexecutable filesystem model: gateway')
            sources.append(native)
            with patch.object(artifacts, 'require_arm', file_model):
                manifest = artifacts.create(root/'pool', [*sources, canonical])
                pool = artifacts.Pool(manifest)
                pool.copy(canonical, root/'good-snapshot')
                self.assertEqual((root/'good-snapshot').read_bytes(), sources[0].read_bytes())
                canonical.unlink()
                canonical.symlink_to(sources[1])
                with self.assertRaises(RuntimeError):
                    pool.copy(canonical, root/'wrong-snapshot')
                with self.assertRaises(RuntimeError):
                    artifacts.Pool(manifest)
                unreported = root/'unknown/uwvm-ros2-gateway'
                unreported.parent.mkdir()
                unreported.write_bytes(native.read_bytes())
                with self.assertRaises(RuntimeError):
                    pool.copy(unreported, root/'unknown-snapshot')
                neutral = root/'neutral'
                neutral.symlink_to(unreported)
                with self.assertRaises(RuntimeError):
                    pool.copy(neutral, root/'unknown-resolved-snapshot')
                for name in ('uwvm-ros2-topic-observer', 'flags.make'):
                    source = root/name
                    source.write_text('nonproduct fixture')
                    pool.copy(source, root/(name+'-copy'))
                    self.assertEqual((root/(name+'-copy')).read_bytes(), source.read_bytes())


if __name__ == '__main__':
    unittest.main()
