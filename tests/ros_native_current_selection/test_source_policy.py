#!/usr/bin/env python3
"""Authored Source/DATA policies; no generator, compiler or Native SDK models.

Uses actual historical products as NEGATIVE data and actual current source.
A positive complete fresh native-bound bundle is never manufactured here.
"""
from pathlib import Path
import hashlib
import copy
import json
import os
import stat
import subprocess
import tempfile
import types
import unittest

ROOT=Path(__file__).resolve().parents[2]
TOOL=ROOT/'sdk/custom-types/native_current/select_bundle.py'
# Capture the fixed trusted source helper itself; never a cached pyc/provider.
fd=os.open(TOOL,os.O_RDONLY|os.O_NONBLOCK|os.O_NOFOLLOW|os.O_CLOEXEC)
try:
    before=os.fstat(fd)
    if not stat.S_ISREG(before.st_mode)or before.st_size>1024*1024:raise ValueError('bounded source helper required')
    pieces=[];count=0
    while True:
        part=os.read(fd,min(65536,1024*1024-count+1))
        if not part:break
        count+=len(part)
        if count>1024*1024:raise ValueError('source helper grew')
        pieces.append(part)
    after=os.fstat(fd);named=os.lstat(TOOL)
    key=lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mode,s.st_mtime_ns,s.st_ctime_ns)
    if key(before)!=key(after)or key(before)!=key(named)or count!=before.st_size:raise ValueError('source helper changed')
    raw=b''.join(pieces)
finally:os.close(fd)
selection=types.ModuleType('actual_current_native_source_selection');selection.__file__=str(TOOL)
exec(compile(raw,str(TOOL),'exec'),selection.__dict__)


class CurrentNativeSourcePolicies(unittest.TestCase):
    def historical(self):
        return ROOT/'sdk/custom-types/generated-bounded-v2'

    def test_actual_historical_directory_cannot_be_retagged_fresh(self):
        with self.assertRaisesRegex(ValueError,'historical delivered'):
            selection.admit(ROOT,ROOT/'sdk/compat/rclcpp/services/catalogs/catalog.preparation.json',self.historical(),2,['example_interfaces/srv/AddTwoInts'])

    def test_actual_old_header_has_no_required_service_action_trait(self):
        generated=self.historical()
        proof=json.loads((generated/'generation.json').read_bytes())
        native=(generated/'native_ros2.hpp').read_bytes()
        self.assertTrue(any(row['kind']=='service'and row['ros_version']==2 for row in proof['types']))
        with self.assertRaisesRegex(ValueError,'protocol schema trait'):
            selection.protocol_traits(native,proof['types'])

    def test_real_historical_receipt_does_not_name_current_generator(self):
        proof=json.loads((self.historical()/'generation.json').read_bytes())
        current=hashlib.sha256((ROOT/'sdk/custom-types/bounded_profile.py').read_bytes()).hexdigest()
        self.assertNotEqual(proof['bounded_generator_sha256'],current)
        self.assertEqual(proof['native_registry_emitted'],True)
        # Source history stays intact, including its actual old provider hash.
        self.assertEqual(proof['outputs']['native_ros2.hpp'],selection.content((self.historical()/'native_ros2.hpp').read_bytes()))

    def test_duplicate_nonfinite_and_exponent_overflow_json_refuse(self):
        for raw in (b'{"a":1,"a":2}',b'{"a":NaN}',b'{"a":Infinity}',b'{"a":1e999}',b'{"a":-1e999}'):
            with self.subTest(raw=raw),self.assertRaises(ValueError):selection.strict(raw)

    def test_unknown_parent_traversal_absolute_and_backslash_definition_refuse(self):
        for path in ('../native.hpp','/tmp/native.hpp','a//b','a/./b','a/../b','a\\b',''):
            with self.subTest(path=path),self.assertRaises(ValueError):selection.relative(path)

    def test_fifo_symlink_and_oversized_source_refuse_before_read(self):
        with tempfile.TemporaryDirectory(prefix='uwvm-native-source-input-')as directory:
            base=Path(directory);regular=base/'regular';regular.write_bytes(b'actual input data')
            fifo=base/'fifo';os.mkfifo(fifo);link=base/'link';link.symlink_to(regular.name)
            for path in(fifo,link):
                with self.subTest(path=path),self.assertRaises((ValueError,OSError)):selection.read(path)
            large=base/'large'
            with large.open('wb')as output:output.truncate(selection.LIMIT+1)
            with self.assertRaises(ValueError):selection.read(large)

    def test_relative_paths_unknown_backend_and_duplicate_interface_refuse(self):
        for root,catalog,generated,backend,names in(
            (Path('relative'),Path('/catalog'),Path('/generated'),2,['x']),
            (ROOT,Path('/catalog'),Path('/generated'),True,['x']),
            (ROOT,Path('/catalog'),Path('/generated'),2,['x','x'])):
            with self.subTest(backend=backend),self.assertRaises(ValueError):selection.admit(root,catalog,generated,backend,names)

    def test_both_real_contracts_let_gateway_load_paired_provider(self):
        for lane in('rclcpp','rcljava'):
            source=(ROOT/f'sdk/compat/{lane}/services/native/codec_contract.cc').read_text()
            self.assertNotIn('#include "native_ros2.hpp"',source)
            self.assertIn('UWVM_NATIVE_BOUND_CONTRACT_SELECTION',source)
            self.assertIn('#include "gateways/ros2/custom_topics.h"',source)
            self.assertIn('ipc::find_custom_service(2,Type)',source)
            self.assertIn('common->request->native_identity==Request::native_identity',source)
            self.assertIn('n::native_interface_contract<Native>(support,Type,Identity)',source)
            self.assertIn('q.a=std::numeric_limits<std::int64_t>::min()',source)
            self.assertIn('r2.sum==99',source)

    def test_native_exporters_and_gateways_do_not_lose_modern_refusal(self):
        for lane in('rclcpp','rcljava'):
            source=(ROOT/f'sdk/compat/{lane}/services/native/identity_export.cc').read_text()
            self.assertIn('!support->get_type_hash_func||!support->get_type_description_func',source)
            self.assertLess(source.index('warm(rosidl_typesupport_cpp::get_service_type_support_handle<S>());'),source.index('::raise(SIGSTOP)'))
            self.assertIn("bounded_native_io::gate('I')",source)
        gateway=(ROOT/'gateways/ros2/custom_topics.h').read_text()
        self.assertIn('#if defined(UWVM_ROS_CUSTOM_ROS2_HEADER) != defined(UWVM_ROS_CUSTOM_TOPIC_MANIFEST)',gateway)
        self.assertIn('#if defined(UWVM_ROS_CUSTOM_ROS2_TYPES)',gateway)

    def test_actual_helper_selects_full_paired_includes_and_executable_link(self):
        source=(ROOT/'sdk/custom-types/native_current/SelectedNative.cmake').read_text()
        self.assertIn('set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)',source)
        self.assertIn('unset(UWVM_CURRENT_NATIVE_PAIRED_LINK CACHE)',source)
        self.assertIn('UWVM_ROS_CUSTOM_TOPIC_MANIFEST=',source)
        self.assertIn('UWVM_ROS_CUSTOM_ROS2_HEADER=',source)
        self.assertIn('"${source}/src"',source)
        self.assertIn('"${source}/third-parties/fast_io/include"',source)
        self.assertIn('rosidl_typesupport_introspection_cpp::rosidl_typesupport_introspection_cpp',source)
        self.assertIn('get_message_type_support_handle<S::Request>()',source)
        self.assertIn('get_message_type_support_handle<S::Response>()',source)
        self.assertIn('uwvm_current_native_link_environment(${target} ${actual_libraries})',source)
        self.assertIn('INTERFACE_LINK_OPTIONS',source)
        self.assertIn('IMPORTED_LINK_INTERFACE_LIBRARIES',source)
        self.assertIn('CMAKE_CXX_STANDARD_LIBRARIES',source)
        self.assertNotIn('try_run(',source)
        self.assertNotIn('add_test(',source)

    def test_actual_catalog_kind_and_component_owner_cannot_be_erased_by_proof(self):
        actual = selection.strict((ROOT / 'tests/ros_custom_types/deployment-final/derivation/combined/catalog.json').read_bytes())
        originals = selection.catalog_interfaces(actual)
        types = selection.strict((self.historical() / 'generation.json').read_bytes())['types']
        selection.validate_types(originals, types)  # Actual preserved SOURCE data, not Native qualification.
        index = next(i for i, row in enumerate(types) if row['ros_version'] == 2 and row['kind'] == 'service')
        for failure in ('kind', 'floating-backend', 'bool-backend', 'component-owner'):
            changed = copy.deepcopy(types)
            row = changed[index]
            if failure == 'kind':
                row['kind'] = 'message'
                row['components'] = {'message': {'type': row['type'], 'native_identity': row['native_identity']}}
            elif failure == 'floating-backend': row['ros_version'] = 2.0
            elif failure == 'bool-backend': row['ros_version'] = True
            else: row['components']['request']['type'] = row['type'] + '_Other'
            with self.subTest(failure=failure), self.assertRaises(ValueError):
                selection.validate_types(originals, changed)

    def test_actual_catalog_definition_suffix_and_type_category_are_paired(self):
        actual = selection.strict((ROOT / 'sdk/compat/rclcpp/services/catalogs/catalog.preparation.json').read_bytes())
        self.assertEqual(selection.catalog_interfaces(actual)[(2, 'example_interfaces/srv/AddTwoInts')][0], 'service')
        for failure in ('suffix', 'category', 'stem', 'floating-backend'):
            changed = copy.deepcopy(actual)
            row = changed['types'][0]
            if failure == 'suffix': row['file'] = row['file'].replace('.srv', '.msg')
            elif failure == 'category': row['type'] = row['type'].replace('/srv/', '/msg/')
            elif failure == 'stem': row['file'] = row['file'].replace('AddTwoInts.srv', 'Different.srv')
            else: row['ros_version'] = 2.0
            with self.subTest(failure=failure), self.assertRaises(ValueError): selection.catalog_interfaces(changed)

    def link_mode(self, flags, environment_variable=None):
        # A future pure CMake input-policy model only: no language, SDK target,
        # source generation, compiler, linker, executable or Native call.
        cmake = os.environ.get('UWVM_NATIVE_POLICY_CMAKE')
        if not cmake: self.skipTest('Supply one explicit actual CMake executable for the local input-policy contract')
        path = Path(cmake)
        if not path.is_absolute() or not path.is_file(): self.fail('Explicit actual absolute CMake executable required')
        with tempfile.TemporaryDirectory(prefix='uwvm-native-link-policy-') as directory:
            script = Path(directory) / 'policy.cmake'
            helper = ROOT / 'sdk/custom-types/native_current/SelectedNative.cmake'
            if environment_variable is None:
                invocation = 'uwvm_current_native_link_mode_guard([==[' + flags + ']==])\n'
            else:
                self.assertIn(environment_variable, ('CMAKE_C_FLAGS', 'CMAKE_C_FLAGS_DEBUG',
                    'CMAKE_C_FLAGS_RELEASE', 'CMAKE_C_FLAGS_RELWITHDEBINFO',
                    'CMAKE_C_FLAGS_MINSIZEREL', 'CMAKE_C_STANDARD_LIBRARIES',
                    'CMAKE_C_LINK_FLAGS', 'CMAKE_C_LINK_EXECUTABLE'))
                invocation = ('set(' + environment_variable + ' [==[' + flags + ']==])\n' +
                    'uwvm_current_native_link_environment()\n')
            script.write_text('cmake_minimum_required(VERSION 3.16)\ninclude([==[' + str(helper) + ']==])\n' + invocation)
            return subprocess.run([str(path), '-P', str(script)], cwd=directory,
                stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                timeout=10, check=False, env={'PATH': '/usr/bin:/bin', 'LC_ALL': 'C'})

    def test_real_link_mode_guard_keeps_normal_cross_sysroot_and_stdlib_flags(self):
        cross = '--target=aarch64-linux-gnu --sysroot=/owned/actual-sysroot -stdlib=libc++ -O2 -g -Wl,--no-undefined -Wl,-rpath,/owned/lib'
        for normal in (cross, cross + ';-Wl,-z;-Wl,defs', cross + ';-Xlinker;-z;-Xlinker;defs',
                cross + ';LINKER:-z;LINKER:defs',
                cross + ';-I;/owned/include;-Wl,-R,/owned/lib',
                cross + ';$<BUILD_INTERFACE:-Wl,-z>;$<BUILD_INTERFACE:-Wl,defs>',
                cross + ';$<LINK_ONLY:-Xlinker>;$<LINK_ONLY:-z>;$<LINK_ONLY:-Xlinker>;$<LINK_ONLY:defs>',
                cross + ';$<INSTALL_INTERFACE:LINKER:-z>;$<INSTALL_INTERFACE:LINKER:defs>'):
            with self.subTest(flags=normal):
                result = self.link_mode(normal)
                self.assertEqual(result.returncode, 0, result.stderr.decode(errors='replace'))

    def test_real_link_mode_guard_refuses_permissive_symbol_and_partial_link_inputs(self):
        for flags in ('-Wl,--unresolved-symbols=ignore-all', '-Xlinker --unresolved-symbols=ignore-in-shared-libs',
                '-Wl,--warn-unresolved-symbols', '-Wl,--allow-shlib-undefined', '-Wl,-z,undefs',
                '-Wl,-undefined,dynamic_lookup', '-Xlinker -undefined -Xlinker suppress',
                '-Wl,--noinhibit-exec', '/FORCE:UNRESOLVED', '-Wl,-r', '--relocatable',
                '-Wl,-z;-Wl,undefs', '-Xlinker;-undefined;-Xlinker;dynamic_lookup',
                '-Xlinker;-z;-Xlinker;undefs', 'LINKER:-z;LINKER:undefs',
                'SHELL:-undefined;SHELL:suppress', '-Wl,-i', '-Xlinker;-Ur', 'LINKER:-i', 'SHELL:-Ur',
                '$<BUILD_INTERFACE:-Wl,-z>;$<BUILD_INTERFACE:-Wl,undefs>',
                '$<BUILD_INTERFACE:-Wl,-z,undefs>',
                '$<LINK_ONLY:-Xlinker>;$<LINK_ONLY:-undefined>;$<LINK_ONLY:-Xlinker>;$<LINK_ONLY:dynamic_lookup>',
                '$<INSTALL_INTERFACE:LINKER:-z>;$<INSTALL_INTERFACE:LINKER:undefs>',
                '$<BUILD_INTERFACE:-Wl,-i>', '$<LINK_ONLY:-Xlinker>;$<LINK_ONLY:-Ur>'):
            with self.subTest(flags=flags):
                result = self.link_mode(flags)
                self.assertNotEqual(result.returncode, 0)
                self.assertIn(b'refuses', result.stderr)

    def test_real_link_oracle_refuses_wasm_undefined_symbol_suppression(self):
        # These actual wasm-ld modes can import unresolved library functions.
        # Checking the real CMake function is an input-policy model, not a
        # compiler/Native/import-census or Runtime permission qualification.
        for mode in ('--allow-undefined', '--allow-undefined-file=/owned/symbols',
                '--import-undefined'):
            for flags in (mode, '-Wl,' + mode, '-Xlinker;' + mode,
                    'LINKER:' + mode, 'SHELL:' + mode,
                    '$<BUILD_INTERFACE:-Wl,' + mode + '>',
                    '$<LINK_ONLY:-Xlinker>;$<LINK_ONLY:' + mode + '>',
                    "-Wl,'" + mode + "'", '-Wl,"' + mode + '"',
                    '-Wl,--' + '"' + mode[2:5] + '"' + mode[5:],
                    '-Wl,--' + chr(92) + mode[2:]):
                with self.subTest(flags=flags):
                    result = self.link_mode(flags)
                    self.assertNotEqual(result.returncode, 0)
                    self.assertIn(b'refuses', result.stderr)

    def test_real_link_oracle_inspects_actual_C_driver_inputs(self):
        variables = ('CMAKE_C_FLAGS', 'CMAKE_C_FLAGS_DEBUG',
            'CMAKE_C_FLAGS_RELEASE', 'CMAKE_C_FLAGS_RELWITHDEBINFO',
            'CMAKE_C_FLAGS_MINSIZEREL', 'CMAKE_C_STANDARD_LIBRARIES',
            'CMAKE_C_LINK_FLAGS', 'CMAKE_C_LINK_EXECUTABLE')
        for variable in variables:
            with self.subTest(variable=variable):
                refused = self.link_mode("-Wl,'--import-undefined'", variable)
                self.assertNotEqual(refused.returncode, 0)
                self.assertIn(b'refuses', refused.stderr)
                allowed = self.link_mode('--target=wasm32-wasi --sysroot="/owned/path with spaces" -pthread -Wl,--import-memory -Wl,--export-memory', variable)
                self.assertEqual(allowed.returncode, 0, allowed.stderr.decode(errors='replace'))

    def test_real_recipe_keeps_generation_a_separate_explicit_stage(self):
        recipe=json.loads((ROOT/'sdk/custom-types/native_current/recipes.json').read_bytes())
        phases={row['stage']:row for row in recipe['phases']}
        self.assertEqual(phases['fresh_bind']['argv'][2],'${SOURCE}/tests/ros_custom_types/bounded-v2/bind_catalog.py')
        self.assertEqual(phases['fresh_current_generate']['argv'][2],'${SOURCE}/sdk/tools/custom_types.py')
        self.assertNotIn('--guest-only',phases['fresh_current_generate']['argv'])
        self.assertIn('-DUWVM_BOUND_CATALOG=${FRESH_BIND_PARENT}/catalog.json',phases['actual_contract_configure']['argv'])
        self.assertEqual(recipe['authority'],[])


if __name__=='__main__':unittest.main(verbosity=2)
