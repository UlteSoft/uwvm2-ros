#!/usr/bin/env python3
"""Linux-only parse/helper checks for the immutable migration test packet.

These are source/routing helper checks. No product, Wasm or native fixture is
compiled or executed, and no execution evidence is inferred from these checks.
"""
from __future__ import annotations
import argparse
import ast
import importlib.util
import json
from pathlib import Path
import sys
from qualification_stack_support import guard, run, save, sha, verify_manifest


def load(path: Path, name: str):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class Refusal(Exception):
    pass


class Parser:
    def error(self, message):
        raise Refusal(message)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--manifest', type=Path, required=True)
    parser.add_argument('--manifest-sha256', required=True)
    parser.add_argument('--lease', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    admission = guard(args.lease)
    root = args.root.resolve(strict=True)
    manifest = verify_manifest(root, args.manifest, args.manifest_sha256)
    args.out.mkdir(parents=True, exist_ok=False)
    report = {'source_parse': 'UNTESTED', 'helper_checks': 'UNTESTED',
              'build': 'UNTESTED', 'wasm_execution': 'UNTESTED', 'native_ros': 'UNTESTED',
              'qemu': 'UNTESTED', 'performance': 'UNTESTED', 'admission': admission,
              'manifest_sha256': sha(args.manifest), 'parsed': [], 'helper_cases': []}
    try:
        for row in manifest['files']:
            path = root / row['path']
            if path.suffix == '.py' and row.get('role') == 'changed':
                ast.parse(path.read_bytes(), filename=str(path))
                report['parsed'].append(row['path'])
            elif path.suffix == '.sh' and row.get('role') == 'changed':
                run(['/usr/bin/bash', '-n', str(path)], root,
                    args.out / (path.name + '.syntax.log'), 30, args.lease)
        report['source_parse'] = 'PASS'
        policy = load(root / 'test/0017.runtime/mandatory_native_cli.py', '_migration_policy')
        audit = load(root / 'tools/ci/qualification_ros_route_audit.py', '_migration_route_audit')
        fake = Parser()
        def check(name, function, expected=None, refusal=False):
            try:
                result = function()
            except Refusal:
                if not refusal:
                    raise
            else:
                if refusal or result != expected:
                    raise AssertionError((name, result, expected, refusal))
            report['helper_cases'].append({'case': name, 'status': 'PASS'})
        check('ROS omitted scalar selector', lambda: policy.product_policy(fake, True, None), 'mandatory-native')
        check('ROS omitted matrix', lambda: policy.product_policies(fake, True, None), ('mandatory-native',))
        for token in ('instruction', 'unwind', 'unwind-uncheck', 'none', 'auto'):
            check('ROS rejects scalar ' + token, lambda token=token: policy.product_policy(fake, True, token), refusal=True)
            check('ROS rejects matrix ' + token, lambda token=token: policy.product_policies(fake, True, [token]), refusal=True)
        check('upstream scalar default', lambda: policy.product_policy(fake, False, None), 'instruction')
        check('upstream selected unwind', lambda: policy.product_policy(fake, False, 'unwind'), 'unwind')
        check('upstream matrix retained', lambda: policy.product_policies(fake, False, None), ('instruction', 'unwind'))
        check('upstream explicit none matrix', lambda: policy.product_policies(fake, False, ['none']), ['none'])
        check('upstream required scalar refusal', lambda: policy.product_policy(fake, False, None, required=True), refusal=True)
        old_platform = policy.sys.platform
        try:
            policy.sys.platform = 'darwin'
            check('non-Linux ROS refusal', lambda: policy.require_linux_ros(fake, True), refusal=True)
            check('non-Linux ordinary capability', lambda: policy.require_linux_ros(fake, False), None)
        finally:
            policy.sys.platform = old_platform
        table = "def modes(ros):\n if ros:\n  return {'native':['-Raot']}\n return {'int':['-Rint']}\n"
        assert audit.verify_table(audit.ros_table(table, 'modes', 'branch'))['rows'] == 1
        for token in ('-Rint', '-Rllvm-call-stack', '-Rcc', '--runtime-tiered'):
            bad = table.replace("['-Raot']", "['-Raot'," + repr(token) + "]")
            try:
                audit.verify_table(audit.ros_table(bad, 'modes', 'branch'))
            except ValueError:
                report['helper_cases'].append({'case': 'positive ROS audit rejects ' + token, 'status': 'PASS'})
            else:
                raise AssertionError('source audit accepted ' + token)
        consumer = load(root / 'tools/ci/qualification_run_stack_fixtures.py', '_migration_consumer')
        old = ['clang++', '-c', consumer.RUNTIME, '-o', '/product/immutable.o',
               '-MF', '/product/immutable.d', '-MMD', '-fexceptions',
               '-DUWVM_ROS_PAIRED_RUNTIME_HEADERS=1']
        rewritten = consumer.consumer_compile(old, Path('/consumer/fixture.cc'), Path('/consumer/fixture.o'))
        assert '/product/immutable.o' not in rewritten and '/product/immutable.d' not in rewritten
        assert rewritten[rewritten.index('-o') + 1] == '/consumer/fixture.o'
        assert '-fexceptions' in rewritten and '-DUWVM_ROS_PAIRED_RUNTIME_HEADERS=1' in rewritten
        assert consumer.RUNTIME not in rewritten and '/consumer/fixture.cc' in rewritten
        report['helper_cases'].append({'case': 'consumer replay preserves EH/paired ABI and isolates output/dependency files', 'status': 'PASS'})
        report['route_authorities'] = audit.audit(root)
        report['helper_checks'] = 'PASS'
        verify_manifest(root, args.manifest, args.manifest_sha256)
        report['final_admission'] = guard(args.lease)
    except BaseException as error:
        report['error'] = repr(error)
        raise
    finally:
        save(args.out / 'source-helper-results.json', report)


if __name__ == '__main__':
    main()
