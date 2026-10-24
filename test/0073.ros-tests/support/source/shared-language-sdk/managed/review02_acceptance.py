#!/usr/bin/env python3
"""Current SDK consumer source checks; no language compiler or Guest credit.

Run only under the original common Linux finite owner. Task 10 owns numbered
entries and migration; this newly authored reusable source is already outside
the SDK. No existing source, fixture or historical evidence is moved here.
"""
import argparse
import copy
import json
import os
from pathlib import Path
import sys
import types
import unittest


def load(name, path):
    module = types.ModuleType(name)
    module.__file__ = str(path)
    sys.modules[name] = module
    exec(compile(path.read_bytes(), str(path), 'exec', dont_inherit=True), module.__dict__)
    return module


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('shared admitted Linux owner required')
    source = args.source.resolve(strict=True)
    sdk = source / 'sdk/origin'
    args.out.mkdir(parents=True, exist_ok=False)
    python = load('_review02_python_consumer', sdk / 'python/tools/build.py')
    js = load('_review02_js_consumer', sdk / 'examples/javascript_telemetry/build.py')
    backend = load('_review02_managed_backend', sdk / 'tools/xmake/examples/managed_backend.py')
    aot = load('_review02_mono_consumer', sdk / 'dotnet/mono-wasi-aot/producer.py')
    grammar = load('_review02_managed_grammar', sdk / 'tools/shared_managed04/profile.py')

    class CurrentSources(unittest.TestCase):
        def directory(self, name):
            p = args.out / name
            p.mkdir()
            return p

        def test_changed_python_tools_parse(self):
            paths = ['python/tools/build.py', 'examples/python_telemetry/build.py',
                'examples/python_transform_pipeline/build.py', 'examples/java_telemetry/build.py',
                'examples/kotlin/build.py', 'examples/csharp_telemetry/build.py',
                'examples/javascript_telemetry/build.py', 'javascript/guest/prepare.py',
                'tools/xmake/examples/managed_backend.py', 'dotnet/mono-wasi-aot/producer.py']
            paths += ['dotnet/mono-wasi-aot/source_kit.py', 'dotnet/mono-wasi-aot/generate_control.py',
                      'csharp/owned/generate_control.py', 'lua/tools/stdlib_profile.py',
                      'tools/shared_managed04/profile.py']
            for name in paths:
                compile((sdk / name).read_bytes(), name, 'exec', dont_inherit=True)

        def test_custom_section_removal_preserves_noncustom_bytes(self):
            header = b'\0asm\x01\0\0\0'
            source = header + b'\x00\x04\x01xAB' + b'\x01\x01\x00' + b'\x00\x02\x01z' + b'\x03\x01\x00'
            result, count = python.strip_custom(source)
            self.assertEqual(result, header + b'\x01\x01\x00\x03\x01\x00')
            self.assertEqual(count, 2)

        def test_truncated_overwide_and_noncore_binaries_refuse(self):
            for body in (b'', b'\0asm\x0d\0\x01\0', b'\0asm\x01\0\0\0\x00',
                         b'\0asm\x01\0\0\0\x00\x80\x80\x80\x80\x10',
                         b'\0asm\x01\0\0\0\x01\x04\0'):
                with self.assertRaises(ValueError):
                    python.strip_custom(body)

        def test_real_maintenance_python_layout(self):
            paths = python.layout(sdk)
            self.assertEqual(paths['python'], sdk / 'python')
            self.assertTrue(all(paths[k].is_file() for k in ('entry', 'bridge', 'freeze', 'recipe')))

        def test_installed_python_layout_has_no_checkout_dependency(self):
            root = self.directory('sdk with Unicode-λ')
            marker = root / 'include/platform/ros/py/roswasm/raw.py'
            marker.parent.mkdir(parents=True)
            marker.write_bytes(b'')
            paths = python.layout(root)
            self.assertEqual(paths['entry'], root / 'lib/platform/ros/py/source/python/embedding/main.c')
            self.assertEqual(paths['recipe'], root / 'tools/platform/ros/py/python_link_recipe.py')
            self.assertTrue(all(p.is_relative_to(root) for k, p in paths.items() if k != 'includes'))

        def test_real_javascript_closed_sdk_imports(self):
            rows = js.closed_modules(sdk, source, sdk / 'examples/javascript_telemetry/telemetry.mjs',
                sdk / 'javascript/guest/bootstrap.mjs')
            names = {r['name'] for r in rows}
            self.assertIn('sdk/origin/javascript/guest.mjs', names)
            self.assertIn('sdk/origin/javascript/codecs.mjs', names)
            self.assertIn('sdk/origin/javascript/guest/bootstrap.mjs', names)
            self.assertEqual(len(rows), len(names))

        def test_all_current_closed_javascript_lessons(self):
            for entry in sorted((sdk / 'examples/javascript_guest').glob('[0-9][0-9]-*.mjs')):
                if entry.name.startswith('10-'):
                    continue  # This real independent-provider composition is explicit pending scope.
                rows = js.closed_modules(sdk, source, entry, sdk / 'javascript/guest/bootstrap.mjs')
                self.assertIn(entry.relative_to(source).as_posix(), {r['name'] for r in rows})

        def test_relocated_javascript_names_and_unicode_paths(self):
            root = self.directory('installed-λ')
            library = root / 'include/platform/ros/js'
            entry = root / 'example/platform/ros/js/application/main.mjs'
            entry.parent.mkdir(parents=True)
            library.mkdir(parents=True)
            entry.write_text("import { value } from '../../../../../include/platform/ros/js/value.mjs';\nexport function main(){return value;}\n")
            (library / 'value.mjs').write_text('export const value=0;\n')
            boot = library / 'bootstrap.mjs'
            boot.write_text("import { encode } from 'roswasm:utf8';\n")
            rows = js.closed_modules(root, root, entry, boot)
            self.assertEqual({r['name'] for r in rows}, {
                'example/platform/ros/js/application/main.mjs', 'include/platform/ros/js/value.mjs',
                'include/platform/ros/js/bootstrap.mjs'})

        def test_host_js_and_missing_provider_refuse(self):
            root = self.directory('invalid-js')
            boot = root / 'bootstrap.mjs'
            boot.write_text("import {} from 'roswasm:utf8';\n")
            for index, text in enumerate(("import fs from 'node:fs';\n",
                    "import {identity32} from 'roswasm:library_imports';\n")):
                entry = root / ('bad' + str(index) + '.mjs')
                entry.write_text(text)
                with self.assertRaises(ValueError):
                    js.closed_modules(root, root, entry, boot)

        def test_dynamic_refusal_probe_cannot_expand_the_closed_map(self):
            root = self.directory('dynamic-js')
            boot, entry = root / 'bootstrap.mjs', root / 'entry.mjs'
            boot.write_text("import {} from 'roswasm:utf8';\n")
            entry.write_text("export async function main(){try{await import('node:fs');}catch{}}\n")
            rows = js.closed_modules(root, root, entry, boot)
            self.assertEqual({row['name'] for row in rows}, {'bootstrap.mjs', 'entry.mjs'})

        def test_confined_source_rejects_escape_and_symlink(self):
            root = self.directory('confined')
            outside = args.out / 'outside.txt'
            outside.write_bytes(b'not SDK input')
            (root / 'link').symlink_to(outside)
            for name in ('../outside.txt', str(outside), 'link'):
                with self.assertRaises(ValueError):
                    backend.confined(root, name)

        def test_exact_offline_input_tree_and_changed_file(self):
            root = self.directory('tools')
            path = root / 'tool'
            path.write_bytes(b'actual input bytes')
            inputs = dict(files=[backend.pin(path)], directories=[str(root)])
            self.assertEqual(len(backend.validate_closure(inputs)[0]), 1)
            path.write_bytes(b'changed actual bytes')
            with self.assertRaises(ValueError):
                backend.validate_closure(inputs)

        def test_unlisted_offline_dependency_refuses(self):
            root = self.directory('unlisted-tools')
            path = root / 'tool'
            path.write_bytes(b'tool')
            inputs = dict(files=[backend.pin(path)], directories=[str(root)])
            (root / 'unlisted.so').write_bytes(b'missing dependency pin')
            with self.assertRaises(ValueError):
                backend.validate_closure(inputs)

        def test_duplicate_json_keys_refuse(self):
            path = args.out / 'duplicate.json'
            path.write_bytes(b'{"format":"a","format":"b"}')
            with self.assertRaises(ValueError):
                backend.document(path)

        def managed_selection(self, name):
            root = self.directory(name)
            inputs = root / 'input closure Unicode-λ.json'
            inputs.write_bytes(b'{"actual":"source-only grammar fixture"}\n')
            selection = dict(format='uwvm-managed-example-selection-v1', sdk_root=str(sdk),
                builder='mono-aot', backend=2, entry='', pure=False, inputs=str(inputs),
                inputs_sha256=backend.pin(inputs)['sha256'])
            options = dict(consumer=selection)
            argv = ['-I', '-S', '-B', str(sdk / 'tools/xmake/examples/managed_backend.py'),
                '--sdk-root', str(sdk), '--resolved', str(root / 'resolved.json'),
                '--inputs', str(inputs), '--inputs-sha256', selection['inputs_sha256'],
                '--builder', 'mono-aot', '--language', 'fsharp', '--backend', '2',
                '--entry', '', '--out', str(root / 'output with Unicode-λ')]
            return root, options, argv

        def test_managed_closed_argv_preserves_empty_unicode_and_spaces(self):
            root, options, argv = self.managed_selection('closed-argv')
            original = list(argv)
            self.assertEqual(grammar.validate_arguments('fsharp', {}, options, argv, root), {})
            self.assertEqual(argv, original)
            self.assertEqual(grammar.validate_arguments('fsharp', {}, {}, [], root), {})

        def test_managed_changed_selector_raw_response_and_duplicate_refuse(self):
            root, options, argv = self.managed_selection('changed-argv')
            changed = list(argv)
            changed[changed.index('--backend') + 1] = '1'
            for words in (changed, argv + ['--entry', ''], ['@unqualified.rsp'],
                          argv + ['--property', 'RuntimeIdentifier=linux-x64']):
                with self.assertRaises(ValueError):
                    grammar.validate_arguments('fsharp', {}, options, words, root)
            Path(options['consumer']['inputs']).write_bytes(b'changed bytes')
            with self.assertRaises(ValueError):
                grammar.validate_arguments('fsharp', {}, options, argv, root)

        def test_managed_provider_requires_exact_runtime_abi_and_one_dialect(self):
            profile = dict(target='wasm32-wasip1', address_bits=32, startup='command',
                exceptions=False, threads=False, wasi='wasip1', core_baseline='3.0')
            runtime = dict(source_ready=True, name='Mono', version='8.0.15', sdk_version='8.0.408',
                route='fsharp-mono8', mode='interpreter', language='fsharp',
                target='wasm32-wasip1', address_bits=32, startup='command', exceptions=False)
            options = dict(runtime=runtime, standard='8.0')
            provider = dict(driver='uwvm-managed-sdk-consumer-v1',
                runtime_identity=grammar.runtime_identity(runtime), compiler_argv=[], linker_argv=[])
            self.assertIs(grammar.validate_provider('fsharp', profile, options, provider), provider)
            changes = [dict(provider, driver='dotnet'), dict(provider, compiler_argv=['@raw.rsp']),
                dict(provider, runtime_identity=dict(provider['runtime_identity'], sdk_version='9.0.100'))]
            for row in changes:
                with self.assertRaises(ValueError):
                    grammar.validate_provider('fsharp', profile, options, row)
            for changed in (dict(profile, address_bits=64), dict(profile, threads=True)):
                with self.assertRaises(ValueError):
                    grammar.validate_provider('fsharp', changed, options, provider)
            for dialect in ('8.0;RuntimeIdentifier=linux-x64', ''):
                with self.assertRaises(ValueError):
                    grammar.validate_provider('fsharp', profile, dict(options, standard=dialect), provider)
            aot_options = copy.deepcopy(options)
            aot_options['runtime']['aot_modes'] = dict(LLVMOnly=True)
            with self.assertRaises(ValueError):
                grammar.validate_provider('fsharp', profile, aot_options, provider)

        def test_real_owner_child_and_original_parent_guard(self):
            group = Path('/proc/self/cgroup').read_text().strip().removeprefix('0::')
            parent = '/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope'
            boot = Path('/proc/sys/kernel/random/boot_id').read_text().strip()
            def identity(name):
                stat = (Path('/sys/fs/cgroup') / name.lstrip('/')).stat()
                return dict(path=name, dev=str(stat.st_dev), ino=str(stat.st_ino), boot_id=boot)
            consumer = aot.Admission.__new__(aot.Admission)
            consumer.guard_pin, consumer.parent_pin = identity(group), identity(parent)
            result = consumer.guard()
            self.assertEqual(result['execution_group'], group)
            self.assertEqual(result['parent_group'], parent)
            consumer.parent_pin = dict(consumer.parent_pin, ino='0')
            with self.assertRaises(ValueError):
                consumer.guard()

    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(CurrentSources))
    report = dict(format='uwvm-managed-review02-source-acceptance-v1', tests=result.testsRun,
        failures=len(result.failures), errors=len(result.errors), compiler_cases=0, guest_cases=0,
        native_ros_cases=0, default_authority=[])
    (args.out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')
    return 0 if result.wasSuccessful() else 1


if __name__ == '__main__':
    raise SystemExit(main())
