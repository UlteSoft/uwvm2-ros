#!/usr/bin/env python3
"""Pure filesystem/metadata models for current-feature parent admission.

The files below contain plain synthetic text and are never executable. ELF
facts are explicitly mocked, so these tests prove admission of complete exact
facts, changed identities, and the 30-product pool inventory only. They do not
establish actual native architecture, compilation, QEMU, cgroup, ROS or Wasm
behavior. Fixture-contract admission is mocked only where necessary to isolate
the parent's separate compiled-product identity check.
"""
import copy
import hashlib
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import artifacts
import run_all as acceptance


def synthetic_fact(path):
    """Return independently measured file identity plus explicitly fake ELF facts."""
    path = Path(path)
    state = path.stat()
    return {'path': str(path), 'sha256': hashlib.sha256(path.read_bytes()).hexdigest(),
            'bytes': state.st_size, 'device': state.st_dev, 'inode': state.st_ino,
            'elf_class': 2, 'elf_data': 1, 'e_machine': 183}


def replace_same_bytes(path):
    """Replace a synthetic file at a new inode without changing its payload."""
    path = Path(path)
    before = synthetic_fact(path)
    temporary = path.with_name(path.name + '.replacement')
    temporary.write_bytes(path.read_bytes())
    temporary.replace(path)
    after = synthetic_fact(path)
    assert before['sha256'] == after['sha256'] and before['inode'] != after['inode']


class FeatureAdmissionModels(unittest.TestCase):
    def test_prepared_action_products_require_exact_complete_facts(self):
        with tempfile.TemporaryDirectory(prefix='feature-preparation-admission-model-') as temporary:
            root = Path(temporary).resolve()
            probes = {}
            for label in acceptance.ACTION_QOS_PROBES:
                path = root/label; path.write_text('Synthetic metadata model only. Never execute.\n')
                probes[label] = {'binary': synthetic_fact(path)}
            original = {'probes': probes}
            report = root/'prepared.json'
            first = next(iter(probes))
            with patch.object(acceptance, 'validate_prepared_contract'), patch.object(
                    acceptance, 'require_arm', side_effect=synthetic_fact):
                report.write_text(json.dumps(original))
                accepted = acceptance.validate_action_qos_inputs(report, 'synthetic-manifest')
                self.assertEqual(accepted['products'], {name: row['binary'] for name, row in probes.items()})
                for field in ('device', 'inode', 'bytes', 'elf_class', 'elf_data', 'e_machine'):
                    bad = copy.deepcopy(original); bad['probes'][first]['binary'][field] += 1
                    report.write_text(json.dumps(bad))
                    with self.subTest(field=field), self.assertRaises(RuntimeError):
                        acceptance.validate_action_qos_inputs(report, 'synthetic-manifest')
                report.write_text(json.dumps(original))
                replace_same_bytes(probes[first]['binary']['path'])
                with self.assertRaises(RuntimeError):
                    acceptance.validate_action_qos_inputs(report, 'synthetic-manifest')

    def test_canonical_gateway_products_reject_replaced_or_changed_facts(self):
        kinds = {1: ('gateway', 'native-probe', 'resources-probe', 'tf-probe', 'action-probe',
                     'perf-probe', 'ipc-service-probe', 'five-module-probe'),
                 2: ('gateway', 'native-probe', 'resources-probe', 'tf-probe', 'action-probe',
                     'lifecycle-probe', 'secure-probe', 'perf-probe', 'ipc-service-probe', 'five-module-probe')}
        with tempfile.TemporaryDirectory(prefix='feature-native-view-admission-model-') as temporary:
            sdk = Path(temporary).resolve(); targets = {}
            for version, distro in ((1, 'noetic'), (2, 'jazzy')):
                directory = sdk/'gateway'/(distro+'-rootfs')/'tmp'/('ros'+str(version)+'-build')
                directory.mkdir(parents=True)
                for kind in kinds[version]:
                    name = 'uwvm-ros'+str(version)+'-'+kind
                    path = directory/name; path.write_text('Synthetic canonical product. Never execute.\n')
                    targets[name] = synthetic_fact(path)
            report = {'targets': targets}; first = next(iter(targets))
            with patch.object(acceptance, 'require_arm', side_effect=synthetic_fact):
                self.assertEqual(len(acceptance.validate_native_views(sdk, report)), 18)
                for field in ('device', 'inode', 'bytes', 'elf_class', 'elf_data', 'e_machine'):
                    bad = copy.deepcopy(report); bad['targets'][first][field] += 1
                    with self.subTest(field=field), self.assertRaises(RuntimeError):
                        acceptance.validate_native_views(sdk, bad)
                replace_same_bytes(targets[first]['path'])
                with self.assertRaises(RuntimeError): acceptance.validate_native_views(sdk, report)

    def test_current_feature_pool_has_thirty_distinct_products(self):
        # Four Workers + eighteen original native targets + four Action QoS
        # probes + the four current ROS 1/ROS 2 feature probes form 30 products.
        with tempfile.TemporaryDirectory(prefix='feature-pool-admission-model-') as temporary:
            root = Path(temporary).resolve(); paths = []
            for index in range(30):
                path = root/('product-'+str(index)); path.write_text('Synthetic pool source '+str(index)+'\n')
                paths.append(path)
            with patch.object(artifacts, 'require_arm', side_effect=synthetic_fact), patch.object(
                    acceptance, 'require_arm', side_effect=synthetic_fact):
                manifest = artifacts.create(root/'pool', paths)
                acceptance.validate_mandatory_product_pool(manifest, paths)
                original = json.loads(manifest.read_text())
                missing = copy.deepcopy(original); missing['artifacts'].pop(str(paths[-1]))
                manifest.write_text(json.dumps(missing))
                with self.assertRaises(RuntimeError): acceptance.validate_mandatory_product_pool(manifest, paths)
                manifest.write_text(json.dumps(original))
                for values in (paths[:-1], [*paths[:-1], paths[0]]):
                    with self.assertRaises(RuntimeError): acceptance.validate_mandatory_product_pool(manifest, values)


if __name__ == '__main__':
    unittest.main()
