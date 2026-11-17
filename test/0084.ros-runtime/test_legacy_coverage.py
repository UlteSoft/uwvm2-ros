"""Fail-closed checks for source coverage; no native-equivalence claim."""
import copy
import importlib.util
import json
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location('coverage_check', ROOT / 'tools/ros_scene/check_legacy_coverage.py')
check = importlib.util.module_from_spec(spec)
spec.loader.exec_module(check)


class LegacyCoverage(unittest.TestCase):
    def setUp(self):
        self.mapping = json.loads((ROOT / 'tools/ros_scene/legacy-coverage.json').read_text())

    def test_real_source_sets(self):
        result = check.audit(ROOT, self.mapping)
        self.assertTrue(result['mapping_complete'])
        self.assertFalse(result['all_targets_registered'])
        self.assertFalse(result['native_equivalence_proven'])
        self.assertFalse(result['package_portability_proven'])

    def test_missing_extra_duplicate_and_target_drift(self):
        for mutation in ('missing', 'extra', 'duplicate', 'target'):
            with self.subTest(mutation=mutation):
                altered = copy.deepcopy(self.mapping)
                rows = altered['imports']
                if mutation == 'missing':
                    rows.pop()
                elif mutation == 'extra':
                    rows.append(dict(rows[0], legacy='uwvm_ros1_v1.invented'))
                elif mutation == 'duplicate':
                    rows.append(copy.deepcopy(rows[0]))
                else:
                    rows[0]['cpp_target'] = 'host::different_operation'
                with self.assertRaises(ValueError):
                    check.audit(ROOT, altered)

    def test_undefined_target_missing_rights_and_false_proof(self):
        for field, value in [('targets', ['invented.operation']), ('authority', []),
                             ('verification_obligations', []), ('differential_qualification', 'qualified')]:
            with self.subTest(field=field):
                altered = copy.deepcopy(self.mapping)
                altered['profiles']['abi'][field] = value
                with self.assertRaises(ValueError):
                    check.audit(ROOT, altered)


if __name__ == '__main__':
    if Path('/proc/self/cgroup').read_text().strip() != '0::/' + str(check.GROUP.relative_to('/sys/fs/cgroup')):
        raise SystemExit('must run in existing 64 GiB cgroup')
    if (check.GROUP / 'memory.max').read_text().strip() != '68719476736' or (check.GROUP / 'memory.swap.max').read_text().strip() != '0':
        raise SystemExit('test gate limits changed')
    unittest.main()
