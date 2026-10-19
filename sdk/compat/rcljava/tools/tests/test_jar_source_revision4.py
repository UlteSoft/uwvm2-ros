#!/usr/bin/env python3
"""Unexecuted Source/real-reader fixtures; run only on the admitted Linux owner.

No compiler, producer, JVM application, Wasm Guest or ROS endpoint is launched.
The keeper admits this entry/interpreter/stdlib before first candidate instruction.
Captured tool bytes below avoid candidate pyc/sys.modules selection; they do not
make the Python interpreter or standard library hermetic.
"""
import copy
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import tempfile
import types
import unittest

ROOT = Path(__file__).resolve().parents[5]
TOOLS = ROOT / 'sdk/compat/rcljava/tools'


def captured(path, maximum=4 << 20):
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= maximum:
            raise ValueError('bounded actual Source required')
        parts = []; count = 0
        while count <= first.st_size:
            part = os.read(fd, min(65536, first.st_size + 1 - count))
            if not part:
                break
            parts.append(part); count += len(part)
        raw = b''.join(parts)
        def key(row):
            return tuple(getattr(row, 'st_' + field) for field in
                         ('dev', 'ino', 'mode', 'nlink', 'size', 'mtime_ns', 'ctime_ns'))
        if len(raw) != first.st_size or key(first) != key(os.fstat(fd)) or key(first) != key(os.lstat(path)):
            raise ValueError('Source changed during fixture admission')
        return raw, stat.S_IMODE(first.st_mode)
    finally:
        os.close(fd)


def u32(value):
    result = bytearray()
    while True:
        part = value & 127; value >>= 7
        result.append(part | (128 if value else 0))
        if not value:
            return bytes(result)


def name(value):
    value = value.encode(); return u32(len(value)) + value


def imports(rows):
    """Actual tiny Core function type/import bytes; never instantiated."""
    types_data = u32(len(rows)); imports_data = u32(len(rows))
    for ordinal, (module, function) in enumerate(rows):
        types_data += b'\x60\x00\x01\x7f'
        imports_data += name(module) + name(function) + b'\x00' + u32(ordinal)
    return (b'\0asm\1\0\0\0' + b'\x01' + u32(len(types_data)) + types_data
            + b'\x02' + u32(len(imports_data)) + imports_data)


class Revision4SourceContracts(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        manifest_raw, unused = captured(TOOLS / 'jar-input-source-v1.json')
        cls.manifest = json.loads(manifest_raw)
        cls.modules = []; cls.names = []
        sources = []
        # Capture/pin both complete candidate tools before executing either.
        for relative in ('sdk/compat/rcljava/tools/plan_jar.py',
                         'sdk/compat/rcljava/tools/check_jar_wasm.py'):
            role = next(row for row in cls.manifest['support_sources'] if row['relative'] == relative)
            raw, mode = captured(ROOT / relative)
            if (len(raw), hashlib.sha256(raw).hexdigest(), mode) != (role['bytes'], role['sha256'], role['mode']):
                raise ValueError('actual tool differs from coherent Source contract')
            sources.append((relative, raw))
        try:
            for relative, raw in sources:
                private = '_uwvm_jar_revision4_fixture_' + os.urandom(16).hex()
                if private in sys.modules:
                    raise ValueError('private fixture module already occupied')
                module = types.ModuleType(private); module.__file__ = str(ROOT / relative)
                module.__package__ = ''; module.__loader__ = module.__spec__ = module.__cached__ = None
                sys.modules[private] = module; cls.names.append((private, module))
                exec(compile(raw, module.__file__, 'exec', dont_inherit=True), module.__dict__)
                if sys.modules.get(private) is not module:
                    raise ValueError('fixture module ownership changed')
                cls.modules.append(module)
            cls.plan, cls.gate = cls.modules
            cls.reader, unused, cls.abi = cls.gate.load_fixed_reader(ROOT, cls.manifest)
        except BaseException:
            cls.tearDownClass(); raise

    @classmethod
    def tearDownClass(cls):
        for private, module in reversed(cls.names):
            if sys.modules.get(private) is module:
                del sys.modules[private]

    def refused_by_both(self, mutate):
        for tool in self.modules:
            document = copy.deepcopy(self.manifest); mutate(document)
            with self.assertRaises(ValueError):
                tool.validate_source_contract(document)

    def empty_tree(self, root):
        for role in self.manifest['sdk_sources']:
            path = root / role['relative']; path.parent.mkdir(parents=True, exist_ok=True); path.touch()

    def test_actual_25_plus_36_source_and_top_level_closure(self):
        for tool in self.modules:
            self.assertEqual(len(tool.validate_source_contract(self.manifest)), 61)
            self.assertEqual(len(tool.closed_java_tree(ROOT, self.manifest)), 61)
        self.assertEqual(len(self.gate.admit_sdk_source_cohort(ROOT, self.manifest)), 69)

    def test_old_revision_and_changed_count_are_not_current(self):
        self.refused_by_both(lambda document: document.update(source_contract_revision=3, sdk_source_count=56))
        self.refused_by_both(lambda document: document.update(sdk_source_count=60))

    def test_missing_or_duplicate_owned_source_cannot_pass_the_count(self):
        target = 'sdk/java/src/main/java/org/uwvm/roswasm/OwnedService.java'
        def duplicate(document):
            index = next(i for i, row in enumerate(document['sdk_sources']) if row['relative'] == target)
            document['sdk_sources'][index] = copy.deepcopy(document['sdk_sources'][0])
        self.refused_by_both(duplicate)
        self.refused_by_both(lambda document: document['sdk_sources'].pop())

    def test_renamed_source_with_same_count_is_refused(self):
        self.refused_by_both(lambda document: document['sdk_sources'][0].update(
            relative='sdk/java/src/main/java/org/uwvm/roswasm/NotTheReviewedClass.java'))

    def test_class_shadow_protection_cannot_drop_or_add_a_class(self):
        self.refused_by_both(lambda document: document['sdk_top_level_classes'].pop())
        self.refused_by_both(lambda document: document['sdk_top_level_classes'].append('forged/Extra.class'))

    def test_missing_and_extra_actual_java_paths_are_refused(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder); self.empty_tree(root)
            path = root / 'sdk/java/src/main/java/org/uwvm/roswasm/OwnedService.java'
            path.unlink()
            for tool in self.modules:
                with self.assertRaises(ValueError): tool.closed_java_tree(root, self.manifest)
            path.touch(); extra = path.with_name('Unreviewed.java'); extra.touch()
            for tool in self.modules:
                with self.assertRaises(ValueError): tool.closed_java_tree(root, self.manifest)

    def test_complete_source_admission_rejects_changed_owned_body(self):
        roles = [*self.manifest['sdk_sources'], *self.manifest['support_sources']]
        roles += [{key: row[key] for key in ('relative', 'bytes', 'sha256', 'mode')}
                  for row in self.manifest['bootstraps'].values()]
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            for role in roles:
                raw, unused = captured(ROOT / role['relative'])
                path = root / role['relative']; path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(raw); path.chmod(role['mode'])
            self.assertEqual(len(self.gate.admit_sdk_source_cohort(root, self.manifest)), 69)
            (root / 'sdk/java/src/main/java/org/uwvm/roswasm/OwnedService.java').write_bytes(b'changed Source')
            with self.assertRaises(ValueError): self.gate.admit_sdk_source_cohort(root, self.manifest)

    def test_source_json_duplicate_key_refused(self):
        for tool in self.modules:
            with self.assertRaises(ValueError): tool.strict_source_json(b'{"sdk_source_count":61,"sdk_source_count":61}')

    def test_compatibility_route_never_enables_raw12(self):
        self.refused_by_both(lambda document: document['owned_rmw_service'].update(enabled=True))
        self.assertEqual(self.manifest['owned_rmw_service']['producer_arguments'],
                         ['--ros-rmw-service-enabled', '0'])
        raw = imports([('uwvm_ros2_v1', 'abi_version'), ('uwvm_ros2_rmw_srv_v1', 'abi_version')])
        with self.assertRaises(ValueError):
            self.gate.check_with_optional_client(raw, '2', self.abi, self.reader.inspect)
        client_raw, unused = captured(ROOT / 'sdk/abi/service-client-v1.json')
        client = self.gate.client_abi_contract(json.loads(client_raw), '2')
        with self.assertRaises(ValueError):
            self.gate.check_with_optional_client(raw, '2', self.abi, self.reader.inspect, client)

    def test_optional_client_pair_keeps_defaultoff_and_half_pair_refusal(self):
        for tool in self.modules:
            self.assertEqual(tool.client_abi_flags(None, None, '2'), [])
            with self.assertRaises(ValueError):
                tool.client_abi_flags(ROOT / 'sdk/abi/service-client-v1.json', None, '2')

    def test_historical_source_contract_is_not_retagged(self):
        self.assertEqual(self.manifest['historical_source_contract']['source_set'], 52)
        self.assertIs(self.manifest['historical_source_contract']['retagged'], False)
        self.assertEqual(self.manifest['previous_active_source_contract']['revision'], 3)
        self.assertEqual(self.manifest['previous_active_source_contract']['sdk_source_count'], 56)


if __name__ == '__main__':
    unittest.main()
