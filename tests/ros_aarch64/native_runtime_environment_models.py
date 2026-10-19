#!/usr/bin/env python3
"""Exercise native loader qualification with synthetic SDK bytes only.

The model creates small fake ARM ELF headers, regular text and symlink records
inside a temporary directory. None is executed or loaded. Production functions
are isolated from their source AST, including the shared read-only descriptor
binder and SDK-rooted symlink resolver; launch controllers are never imported.
These cases prove finite source/metadata behavior, not real SDK ABI compatibility,
dynamic dependency resolution, ROS behavior, cgroup execution or ARM execution.

Run with ``PYTHONDONTWRITEBYTECODE=1 python3 -B ... --out fresh-report.json``.
The optional report must be fresh. It preserves exact input hashes and lists
each assertion, while expressly leaving all native/runtime execution untested.
"""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import stat
import sys
import tempfile

sys.dont_write_bytecode = True


def isolated_functions(source):
    """Load only the reviewed functions/constants; no module import side effects."""
    directory = source / 'tests/ros_aarch64'
    namespace = {'os': os, 'Path': Path, 'posixpath': posixpath, 're': re,
                 'stat': stat, 'hashlib': hashlib}
    inputs = {}
    for name, selected in (('gateway_qualification.py', {'read_file_binding'}),
                           ('rebuild_affected.py', {'rooted'}),
                           ('native_runtime_environment.py', None)):
        path = directory / name
        raw = path.read_bytes(); inputs[str(path.relative_to(source))] = hashlib.sha256(raw).hexdigest()
        tree = ast.parse(raw)
        nodes = [node for node in tree.body if
                 isinstance(node, ast.FunctionDef) and (selected is None or node.name in selected) or
                 selected is None and isinstance(node, ast.Assign)]
        if selected is not None and {node.name for node in nodes} != selected:
            raise RuntimeError('pure AST function inventory differs: ' + name)
        exec(compile(ast.Module(body=nodes, type_ignores=[]), str(path), 'exec'), namespace)
    return namespace, inputs


def arm_shared(machine=183, elf_class=2, byte_order=1, elf_type=3, size=64):
    """Make data sufficient for header classification, never a runnable library."""
    raw = bytearray(max(64, size)); raw[:7] = b'\x7fELF' + bytes((elf_class, byte_order, 1))
    raw[16:18] = elf_type.to_bytes(2, 'little'); raw[18:20] = machine.to_bytes(2, 'little')
    return bytes(raw[:size])


class SyntheticSDK:
    """Own the entire fake SDK and its independently measured inventory."""
    def __init__(self, root, version=2, enabled=True, linked=True):
        self.root = root
        self.directory = root / 'opt/uwvm-custom-types/lib'; self.directory.mkdir(parents=True)
        self.library = self.directory / 'libselected.so'
        self.library.write_bytes(arm_shared())
        self.dependency = self.directory / 'libdependency.so.1'
        self.dependency.write_bytes(arm_shared(size=65))
        (self.directory / 'package-data.txt').write_text('Synthetic non-ELF SDK data.\n')
        self.inventory = self.measure()
        selection = {'enabled': enabled, 'version': version}
        if enabled:
            selection['prefix'] = {'guest_path': '/opt/uwvm-custom-types', 'sdk_relative': 'opt/uwvm-custom-types'}
        self.qualification = {'version': version, 'target': 'uwvm-ros' + str(version) + '-gateway',
                              'sdk_root': str(root), 'custom_selection_binding': {'selection': selection},
                              'sdk_link_inputs': {}}
        if linked:
            self.qualification['sdk_link_inputs']['/opt/uwvm-custom-types/lib/libselected.so'] = {
                'resolved_sdk_path': 'opt/uwvm-custom-types/lib/libselected.so',
                'sha256': hashlib.sha256(self.library.read_bytes()).hexdigest()}

    def measure(self):
        result = {}
        for path in self.root.rglob('*'):
            relative = str(path.relative_to(self.root))
            if path.is_symlink(): result[relative] = {'symlink': os.readlink(path)}
            elif path.is_file(): result[relative] = {'sha256': hashlib.sha256(path.read_bytes()).hexdigest(), 'bytes': path.stat().st_size}
        return result


def run_models(source):
    namespace, inputs = isolated_functions(source)
    helper = namespace['gateway_library_environment']; checks = []

    def check(name, assertion):
        if not assertion: raise AssertionError(name)
        checks.append(name)

    def case(name, change=None, version=2, enabled=True, linked=True, accept=False, verify=None):
        with tempfile.TemporaryDirectory(prefix='uwvm-native-loader-model-') as temporary:
            fixture = SyntheticSDK(Path(temporary).resolve(), version, enabled, linked)
            if change: change(fixture)
            try:
                result = helper(fixture.qualification, fixture.root, fixture.inventory)
            except (RuntimeError, OSError, KeyError, TypeError, ValueError):
                check(name, not accept); return
            check(name, accept)
            if verify: verify(fixture, result)

    def valid(fixture, result):
        check('literal loader entry has no ambient/empty component', result['environment'] == {'LD_LIBRARY_PATH': '/opt/uwvm-custom-types/lib'})
        check('non-scalar type-support dependency is pinned', '/opt/uwvm-custom-types/lib/libdependency.so.1' in result['libraries'])
        check('complete regular directory files are pinned', len(result['input_pins']) == 3)
        check('metadata result grants no Guest permission', 'no' in result['scope'] and 'Guest permission' in result['scope'])
        prior = result['input_pins'][0]; selected = Path(prior['path']); data = selected.read_bytes()
        replacement = selected.with_name(selected.name + '.replacement'); replacement.write_bytes(data); replacement.replace(selected)
        check('caller pin preservation rejects same-byte inode replacement', namespace['read_file_binding'](selected) != prior)

    case('qualified ROS2 complete library directory accepted', accept=True, verify=valid)
    case('qualified ROS1 custom shared libraries accepted', version=1, accept=True)
    for version in (1, 2):
        case('disabled ROS' + str(version) + ' gets no native loader path', version=version, enabled=False, linked=False, accept=True,
             verify=lambda f, r: check('disabled selection returns closed empty result ROS' + str(f.qualification['version']),
                                        r['environment'] == {} and r['input_pins'] == [] and not r['enabled']))
        case('disabled ROS' + str(version) + ' refuses custom scalar library', version=version, enabled=False)
    case('ROS1 header-only selection gets explicit scope', version=1, linked=False, accept=True,
         verify=lambda f, r: check('header-only has no added library permission/path', r['enabled'] and r['environment'] == {} and 'HEADER_ONLY' in r['scope']))
    case('ROS2 enabled selection with no custom links refused', linked=False)
    def missing_prefix(f):
        import shutil
        shutil.rmtree(f.root / 'opt/uwvm-custom-types')
        (f.root / 'opt/uwvm-custom-types').write_text('not a package directory')
    case('ROS1 header-only prefix must remain a directory', missing_prefix, version=1, linked=False)

    def alias(f):
        (f.directory / 'libselected-alias.so').symlink_to('libselected.so'); f.inventory = f.measure()
        fact = f.qualification['sdk_link_inputs'].pop('/opt/uwvm-custom-types/lib/libselected.so')
        f.qualification['sdk_link_inputs']['/opt/uwvm-custom-types/lib/libselected-alias.so'] = fact
    case('recorded internal shared-library alias accepted', alias, accept=True,
         verify=lambda f, r: check('symlink alias deduplicates regular pins', len(r['input_pins']) == 3 and '/opt/uwvm-custom-types/lib/libselected-alias.so' in r['libraries']))
    case('absolute SDK-rooted internal alias accepted', lambda f: ((f.directory / 'libabsolute.so').symlink_to('/opt/uwvm-custom-types/lib/libselected.so'), setattr(f, 'inventory', f.measure())), accept=True)

    mutations = {
        'missing strict custom selection': lambda f: f.qualification.pop('custom_selection_binding'),
        'raw selection without strict binding': lambda f: f.qualification.update(custom_selection_binding=f.qualification['custom_selection_binding']['selection']),
        'missing full scalar link inventory': lambda f: f.qualification.pop('sdk_link_inputs'),
        'mismatched canonical SDK root': lambda f: f.qualification.update(sdk_root=str(f.root) + '/elsewhere'),
        'mismatched qualification version': lambda f: f.qualification.update(version=1),
        'mismatched qualification target': lambda f: f.qualification.update(target='other-gateway'),
        'boolean ROS version refused': lambda f: f.qualification['custom_selection_binding']['selection'].update(version=True),
        'unsupported ROS version refused': lambda f: f.qualification['custom_selection_binding']['selection'].update(version=3),
        'string enabled flag refused': lambda f: f.qualification['custom_selection_binding']['selection'].update(enabled='true'),
        'guest custom prefix differs': lambda f: f.qualification['custom_selection_binding']['selection']['prefix'].update(guest_path='/opt/other'),
        'resolved custom prefix differs': lambda f: f.qualification['custom_selection_binding']['selection']['prefix'].update(sdk_relative='opt/other'),
        'missing selected prefix refused': lambda f: f.qualification['custom_selection_binding']['selection'].pop('prefix'),
        'qualified linked hash mismatch': lambda f: next(iter(f.qualification['sdk_link_inputs'].values())).update(sha256='0' * 64),
        'qualified linked size mismatch': lambda f: next(iter(f.qualification['sdk_link_inputs'].values())).update(bytes=63),
        'qualified resolved path mismatch': lambda f: next(iter(f.qualification['sdk_link_inputs'].values())).update(resolved_sdk_path='opt/uwvm-custom-types/lib/libdependency.so.1'),
        'inventory linked hash mismatch': lambda f: f.inventory['opt/uwvm-custom-types/lib/libselected.so'].update(sha256='1' * 64),
        'inventory linked size mismatch': lambda f: f.inventory['opt/uwvm-custom-types/lib/libselected.so'].update(bytes=63),
        'boolean inventory size refused': lambda f: f.inventory['opt/uwvm-custom-types/lib/libselected.so'].update(bytes=True),
        'non-scalar dependency hash mismatch': lambda f: f.inventory['opt/uwvm-custom-types/lib/libdependency.so.1'].update(sha256='2' * 64),
        'mutated selected library after inventory': lambda f: f.library.write_bytes(arm_shared(size=66)),
        'mutated transitive library after inventory': lambda f: f.dependency.write_bytes(arm_shared(size=67)),
        'unrecorded library addition': lambda f: (f.directory / 'libextra.so').write_bytes(arm_shared()),
        'recorded library deletion': lambda f: f.dependency.unlink(),
        'invented closure inventory key': lambda f: f.inventory.update({'opt/uwvm-custom-types/lib/libmissing.so': {'sha256': '3' * 64, 'bytes': 64}}),
        'regular inventory entry replaced by symlink claim': lambda f: f.inventory.update({'opt/uwvm-custom-types/lib/libselected.so': {'symlink': 'libdependency.so.1'}}),
        'malformed inventory regular record': lambda f: f.inventory.update({'opt/uwvm-custom-types/lib/libselected.so': []}),
        'malformed inventory key': lambda f: f.inventory.update({1: {}}),
        'special FIFO fails before descriptor read': lambda f: os.mkfifo(f.directory / 'never-open-fifo'),
        'nested directory symlink closure refused': lambda f: (f.directory / 'nested').symlink_to(f.directory, target_is_directory=True),
        'dangling library symlink refused': lambda f: ((f.directory / 'libgone.so').symlink_to('no-such-file'), setattr(f, 'inventory', f.measure())),
    }
    for name, change in mutations.items(): case(name, change)
    for label, values in (('x86 ELF', {'machine': 62}), ('32-bit ELF', {'elf_class': 1}),
                          ('big-endian ELF', {'byte_order': 2}), ('ET_EXEC ELF', {'elf_type': 2}),
                          ('truncated ELF', {'size': 20})):
        for dependency in (False, True):
            def change(f, values=values, dependency=dependency):
                (f.dependency if dependency else f.library).write_bytes(arm_shared(**values)); f.inventory = f.measure()
                if not dependency: next(iter(f.qualification['sdk_link_inputs'].values()))['sha256'] = hashlib.sha256(f.library.read_bytes()).hexdigest()
            case(label + (' transitive object refused' if dependency else ' scalar object refused'), change)
    case('non-ELF scalar .so refused', lambda f: (f.library.write_bytes(b'synthetic linker text'), setattr(f, 'inventory', f.measure()), next(iter(f.qualification['sdk_link_inputs'].values())).update(sha256=hashlib.sha256(f.library.read_bytes()).hexdigest())))

    for logical in ('/opt/uwvm-custom-types/lib/../lib/libselected.so', '/opt/uwvm-custom-types/lib/libselected.so:evil',
                    '/opt/uwvm-custom-types/lib/libselected.so\n', '/opt/uwvm-custom-types/lib/libselected.a',
                    '/opt/elsewhere/libselected.so', '/opt/uwvm-custom-types/lib/subdir/libselected.so'):
        def change(f, logical=logical): f.qualification['sdk_link_inputs'] = {logical: next(iter(f.qualification['sdk_link_inputs'].values()))}
        case('alternate or out-of-directory scalar path refused ' + repr(logical), change)
    case('disabled selection refuses resolved custom alias', lambda f: f.qualification.update(sdk_link_inputs={'/usr/lib/libalias.so': {'resolved_sdk_path': 'opt/uwvm-custom-types/lib/libselected.so', 'sha256': '4' * 64}}), enabled=False, linked=False)
    def escape(f):
        other = f.root / 'opt/other'; other.mkdir(); (other / 'liboutside.so').write_bytes(arm_shared())
        (f.directory / 'libescape.so').symlink_to('../../other/liboutside.so'); f.inventory = f.measure()
    case('custom library alias escaping closed directory refused', escape)
    def changed_alias(f):
        alias(f); (f.directory / 'libselected-alias.so').unlink(); (f.directory / 'libselected-alias.so').symlink_to('libdependency.so.1')
    case('same-path custom symlink target mutation refused', changed_alias)
    helper_tree = ast.parse((source / 'tests/ros_aarch64/native_runtime_environment.py').read_text())
    check('production helper has no launch or library-loader call', not any(isinstance(node, ast.Call) and
          isinstance(node.func, ast.Attribute) and node.func.attr in ('Popen', 'run', 'system', 'CDLL', 'PyDLL', 'LoadLibrary') for node in ast.walk(helper_tree)))
    check('production module exposes no command execution entry point', not any(isinstance(node, ast.FunctionDef) and node.name == 'main' for node in helper_tree.body))
    return {'status': 'PURE_NATIVE_LOADER_MODELS_PASS', 'checks': len(checks), 'assertions': checks,
            'inputs': inputs, 'models': 'Synthetic file/header/metadata qualification only',
            'native_compilation': 'NOT_PERFORMED', 'shared_library_load': 'NOT_PERFORMED',
            'ROS_execution': 'NOT_PERFORMED', 'ARM_execution': 'NOT_PERFORMED', 'task_complete': False}


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--source', type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument('--out', type=Path)
    args = parser.parse_args()
    report = run_models(args.source.resolve(strict=True))
    if args.out:
        with args.out.open('x') as stream: stream.write(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'status': report['status'], 'checks': report['checks']}))


if __name__ == '__main__':
    main()
