#!/usr/bin/env python3
"""Qualify unchanged compiler inputs and affected rebuilt ARM Gateways."""
import argparse
import copy
import json
import os
from pathlib import Path
import posixpath
import re
import shlex
import sys

sys.dont_write_bytecode = True

from runtime import digest, require_arm
from rebuild_affected import rooted


def reference(path):
    path = Path(path).resolve(strict=True)
    return {'path': str(path), 'sha256': digest(path)}


def verify_metadata(rows):
    for row in rows.values():
        if digest(row['path']) != row['sha256']:
            raise RuntimeError('compiler metadata changed: '+row['path'])


def check_dependencies(source, manifest, dependencies):
    if not dependencies:
        raise RuntimeError('empty compiler dependency closure')
    for name, value in dependencies.items():
        path = (source/name).resolve(strict=True)
        if not path.is_relative_to(source) or name not in manifest:
            raise RuntimeError('compiler dependency outside declared source: '+name)
        if value != manifest[name]['sha256'] or digest(path) != value:
            raise RuntimeError('changed compiler input cannot be reused: '+name)
    return dict(dependencies)


def recorded_dependencies(path, prefix):
    text = Path(path).read_text().replace('\\\n', ' ')
    return {posixpath.normpath(value.rstrip(':'))[len(prefix):]
            for value in re.findall(re.escape(prefix)+r'[^\s\\]+', text)
            if not value.rstrip(':').endswith('.o')}


def native_dependencies(report, target):
    method = report['qualified_source_dependency_methods'][target]
    if 'source_dependencies' in method:
        values = method['source_dependencies']
    else:
        replay = method['replayed_gcc_dependencies']
        if digest(replay['path']) != replay['sha256']:
            raise RuntimeError('original GCC replay dependency record changed')
        values = replay['source_dependencies']
        if recorded_dependencies(replay['path'], '/work/') != set(values):
            raise RuntimeError('GCC replay parsed project dependencies differ from raw bytes')
    keys = method.get('original_gcc_dependency_keys', [])+method.get('original_cmake_dependency_keys', [])
    if not keys:
        raise RuntimeError('original compilation dependency records required')
    for key in keys:
        row = report['build_metadata'][key]
        if digest(row['path']) != row['sha256']:
            raise RuntimeError('original compiler dependency record changed')
        # Bind the parsed inventory to the original compiler record, not just
        # an independently asserted list of filenames.
        actual_names = recorded_dependencies(row['path'], '/work/')
        if not actual_names:
            raise RuntimeError('original dependency record does not name project inputs')
        if key in method.get('original_gcc_dependency_keys', []) and actual_names != set(values):
            raise RuntimeError('original GCC parsed project dependencies differ from raw bytes')
    return values, copy.deepcopy(method)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'manifest', 'base-native', 'base-workers', 'sdk-workspace', 'sdk-binding-report', 'out'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--rebuilt-native', type=Path, nargs='+', required=True)
    parser.add_argument('--view-tag', default='action-close-r2-qualified')
    args = parser.parse_args()
    if not re.fullmatch('[a-z0-9]+(?:-[a-z0-9]+)*', args.view_tag) or len(args.view_tag) > 64:
        raise RuntimeError('invalid task-owned native view tag')
    source = args.source.resolve(strict=True)
    manifest = json.loads(args.manifest.read_text())
    native = json.loads(args.base_native.read_text())
    fresh_reports = [(path, json.loads(path.read_text())) for path in args.rebuilt_native]
    fresh_targets = {}
    for path, report in fresh_reports:
        for target in report['targets']:
            if target not in ('uwvm-ros1-gateway', 'uwvm-ros2-gateway') or target in fresh_targets:
                raise RuntimeError('unique affected native Gateways required')
            fresh_targets[target] = (path, report)
    workers = json.loads(args.base_workers.read_text())
    if any(row['status'] != 'PASS' for row in [native, workers, *[row for _, row in fresh_reports]]):
        raise RuntimeError('qualified original products and successful fresh rebuild required')
    if len(native['targets']) != 18 or not fresh_targets:
        raise RuntimeError('eighteen original native targets and affected rebuilt Gateways required')
    for _, rebuilt in fresh_reports:
        compiled_manifest = Path(rebuilt['source']).parent/'source-manifest.json'
        if digest(compiled_manifest) != rebuilt['source_manifest_sha256']:
            raise RuntimeError('rebuilt Gateway frozen source manifest changed')
    old_manifest_path = Path(native['source']).parent/'source-manifest.json'
    old_manifest = json.loads(old_manifest_path.read_text())
    if digest(old_manifest_path) != native['source_manifest_sha256'] or set(old_manifest) != set(manifest):
        raise RuntimeError('original immutable source manifest or file set changed')
    changed = [name for name in manifest if old_manifest[name] != manifest[name]]
    if not changed or set(changed)-{'gateways/ros2/action.h', 'gateways/ros1/main.cc'}:
        raise RuntimeError('this qualification permits only the Action and service-discovery fixes')
    for name, row in manifest.items():
        if digest(source/name) != row['sha256'] or (source/name).stat().st_size != row['bytes']:
            raise RuntimeError('new frozen source changed: '+name)
    verify_metadata(native['build_metadata'])
    for _, rebuilt in fresh_reports:
        verify_metadata(rebuilt['build_metadata'])
    binding = json.loads(args.sdk_binding_report.read_text())
    if binding['status'] != 'PASS' or {row['kind'] for row in binding['sdks']} != {'ros1', 'ros2', 'worker-cross'}:
        raise RuntimeError('all three complete SDK byte bindings required')
    sdk_bindings = []
    roots = {'ros1': args.sdk_workspace/'gateway/noetic-rootfs',
             'ros2': args.sdk_workspace/'gateway/jazzy-rootfs',
             'worker-cross': args.base_workers.parent.parent/'cross-deps/sdk'}
    for row in binding['sdks']:
        sdk_root = roots[row['kind']].resolve(strict=True)
        if Path(row['root']).resolve(strict=True) != sdk_root or row['mismatch_count']:
            raise RuntimeError('SDK binding root or comparison mismatch')
        baseline = Path(row['baseline']['path'])
        if digest(baseline) != row['baseline']['sha256']:
            raise RuntimeError('SDK baseline changed')
        entries = json.loads(baseline.read_text())
        if 'regular_files' in entries:
            entries = {item['path']: {'sha256': item['sha256'], 'bytes': item['bytes']}
                       for item in entries['regular_files']} | {
                           item['path']: {'symlink': item['target']} for item in entries['symlinks']}
        if len(entries) != row['baseline_record_count']:
            raise RuntimeError('SDK baseline record count differs')
        prefixes = [sdk_root] if row['kind'] == 'worker-cross' else [
            sdk_root/name for name in ('usr', 'opt', 'etc', 'var/lib/dpkg')]
        actual_names = {str(path.relative_to(sdk_root)) for prefix in prefixes
                        for path in prefix.rglob('*') if path.is_symlink() or path.is_file()}
        if actual_names != set(entries):
            raise RuntimeError('SDK compiler/header/library file set differs: '+row['kind']+
                               ' added='+str(sorted(actual_names-set(entries))[:8])+
                               ' missing='+str(sorted(set(entries)-actual_names)[:8]))
        for name, entry in entries.items():
            if name.startswith('/') or '..' in Path(name).parts:
                raise RuntimeError('SDK baseline path escapes its guest root')
            literal, actual = sdk_root/name, rooted(sdk_root, name)
            if not actual.is_relative_to(sdk_root):
                raise RuntimeError('SDK resolved path escapes its guest root')
            if 'symlink' in entry:
                if not literal.is_symlink() or os.readlink(literal) != entry['symlink']:
                    raise RuntimeError('SDK baseline symlink changed: '+name)
            elif digest(actual) != entry['sha256'] or actual.stat().st_size != entry['bytes']:
                raise RuntimeError('SDK baseline compiler/header/library bytes changed: '+name)
        sdk_bindings.append({'kind': row['kind'], 'root': str(sdk_root),
                             'baseline': reference(baseline), 'verified_records': len(entries)})
    alias_path = args.sdk_binding_report.parent/'root-alias-binding-report.json'
    aliases = json.loads(alias_path.read_text())
    if aliases['status'] != 'PASS' or aliases['mismatches'] or len(aliases['rows']) != 8:
        raise RuntimeError('official native SDK root alias bindings required')
    for item in aliases['expected_inputs']:
        if digest(item['path']) != item['sha256']:
            raise RuntimeError('SDK root alias provenance changed')
    for row in aliases['rows']:
        sdk_root = roots['ros1' if row['distro'] == 'noetic' else 'ros2'].resolve(strict=True)
        if Path(row['root']).resolve(strict=True) != sdk_root:
            raise RuntimeError('SDK root alias binding uses another root')
        path = sdk_root/row['guest_path'].lstrip('/')
        actual = {'symlink': os.readlink(path)} if path.is_symlink() else None
        if actual != row['expected'] or (row['expected'] is None and path.exists()):
            raise RuntimeError('SDK root alias changed')
    def flags(report, target):
        version = 1 if target.startswith('uwvm-ros1-') else 2
        flags_key = f'ros{version}:CMakeFiles/{target}.dir/flags.make'
        text = Path(report['build_metadata'][flags_key]['path']).read_text()
        return {name: re.search(r'^'+name+r' = (.*)$', text, re.M).group(1)
                for name in ('CXX_DEFINES', 'CXX_INCLUDES', 'CXX_FLAGS')}
    def codegen_tokens(tokens):
        values, index = [], 0
        while index < len(tokens):
            token = tokens[index]
            if token in ('-MF', '-MT', '-MQ'):
                if index+1 == len(tokens):
                    raise RuntimeError('dependency output flag lacks its operand')
                index += 2
            elif token in ('-MD', '-MMD', '-MP'):
                index += 1
            else:
                values.append(token); index += 1
        return values
    def codegen_flags(report, target):
        values = flags(report, target)
        values['CXX_FLAGS'] = ' '.join(codegen_tokens(shlex.split(values['CXX_FLAGS'])))
        return values
    def link(report, target):
        version = 1 if target.startswith('uwvm-ros1-') else 2
        link_key = f'ros{version}:CMakeFiles/{target}.dir/link.txt'
        tokens = shlex.split(Path(report['build_metadata'][link_key]['path']).read_text())
        if not tokens or not tokens[0].startswith('/usr/bin/'):
            raise RuntimeError('recorded native compiler link driver required')
        compiler = rooted(roots[f'ros{version}'].resolve(strict=True), tokens[0])
        tokens[0] = 'compiler-sha256:'+digest(compiler)
        return codegen_tokens(tokens)
    compiler_comparisons = {}
    for target, (_, rebuilt) in fresh_targets.items():
        if codegen_flags(native, target) != codegen_flags(rebuilt, target) or link(native, target) != link(rebuilt, target):
            raise RuntimeError('fresh Gateway code generation/link inputs differ: '+target)
        compiler_comparisons[target] = {'original': flags(native, target), 'fresh': flags(rebuilt, target),
            'equivalent_codegen_flags': codegen_flags(rebuilt, target), 'equivalent_link_inputs': link(rebuilt, target),
            'comparison_scope': 'Exact defines/includes/optimization/language/warnings/link inputs; only dependency-output options excluded'}
    methods, closure, targets, reused = {}, {}, {}, []
    for target, original in native['targets'].items():
        values, method = native_dependencies(native, target)
        actual = require_arm(original['path'])
        if actual['sha256'] != original['sha256']:
            raise RuntimeError('original ARM product changed: '+target)
        affected = any(manifest[name]['sha256'] != value for name, value in values.items())
        if affected != (target in fresh_targets):
            raise RuntimeError('fresh targets must exactly match changed original compiler inputs: '+target)
        if affected:
            rebuilt_path, rebuilt = fresh_targets[target]
            dependencies = {}
            for key, entries in rebuilt['compiled_source_dependencies'].items():
                if '/'+target+'.dir/' in key:
                    raw = rebuilt['build_metadata'][key]
                    if recorded_dependencies(raw['path'], '/work/') != set(entries):
                        raise RuntimeError('fresh GCC parsed project dependencies differ from raw bytes')
                    dependencies.update(entries)
            closure[target] = check_dependencies(source, manifest, dependencies)
            if any(name not in closure[target] for name in changed if name in values):
                raise RuntimeError('fresh compiler must consume each affected fixed source')
            targets[target] = require_arm(rebuilt['targets'][target]['path'])
            if targets[target]['sha256'] != rebuilt['targets'][target]['sha256']:
                raise RuntimeError('fresh ARM product changed')
            methods[target] = {'method': 'Fresh native GCC compilation with original code generation options',
                               'source_dependencies': closure[target], 'build_report': reference(rebuilt_path)}
        else:
            closure[target] = check_dependencies(source, manifest, values)
            targets[target] = actual
            methods[target] = method
            reused.append(target)
    if len(workers['profiles']) != 4 or {row['profile'] for row in workers['profiles']} != {'dual', 'ros1', 'ros2', 'none'}:
        raise RuntimeError('all four original Worker profiles required')
    worker_result = copy.deepcopy(workers)
    worker_closure = {}
    worker_records = {}
    for row in worker_result['profiles']:
        if row['status'] != 'PASS' or require_arm(row['binary']['path'])['sha256'] != row['binary']['sha256']:
            raise RuntimeError('original qualified Worker changed')
        for unit, dependencies in row['dependencies'].items():
            key = row['profile']+'/'+unit
            worker_closure[key] = check_dependencies(source, manifest, dependencies)
            dep = args.base_workers.parent/row['profile']/(unit+'.d')
            # Original Worker records use absolute source paths; preserve raw
            # bytes and confirm every normalized compiler input is present.
            if recorded_dependencies(dep, str(Path(workers['source']).resolve())+'/') != set(dependencies):
                raise RuntimeError('original Worker dependency record incomplete: '+key)
            worker_records[key] = reference(dep)
    result = {'status': 'PASS', 'scope': 'Affected rebuilt ARM Gateways plus unchanged original native products; '
              'each reused original compiler input matches the new frozen source. This report does not assert ROS behavior.',
              'source': str(source), 'source_manifest_sha256': digest(args.manifest),
              'source_files': len(manifest), 'targets': targets,
              'sdk_binding_report': reference(args.sdk_binding_report), 'sdk_byte_bindings': sdk_bindings,
              'sdk_root_alias_report': reference(alias_path),
              'source_manifest': reference(args.manifest), 'original_source_manifest': reference(old_manifest_path),
              'changed_source_paths': changed, 'compiler_comparisons': compiler_comparisons,
              'qualified_source_dependency_methods': methods, 'source_dependencies': closure,
              'base_native_report': reference(args.base_native), 'rebuilt_native_reports': [reference(p) for p, _ in fresh_reports],
              'reused_targets': reused, 'fresh_targets': sorted(fresh_targets),
              'build_metadata': {'original:'+key: row for key, row in native['build_metadata'].items()} |
                                {'fresh:'+str(index)+':'+key: row for index, (_, rebuilt) in enumerate(fresh_reports)
                                 for key, row in rebuilt['build_metadata'].items()},
              'driver': reference(__file__)}
    worker_result.update(source=str(source), source_manifest_sha256=digest(args.manifest),
                         scope='Four original Workers reused after checking all original compiler inputs; no fresh Worker compilation claimed.',
                         original_worker_report=reference(args.base_workers), reuse_dependency_closure=worker_closure,
                         original_compiler_dependency_records=worker_records,
                         sdk_binding_report=reference(args.sdk_binding_report), sdk_byte_bindings=sdk_bindings,
                         qualification_driver=reference(__file__))
    # These new task-owned views retain the original compiler outputs intact.
    # Validate the complete view before replacing the two canonical symlinks.
    views = {}
    for version, distro in ((1, 'noetic'), (2, 'jazzy')):
        root = args.sdk_workspace/'gateway'/(distro+'-rootfs')
        if not (root/'.uwvm-task-owned').is_file():
            raise RuntimeError('task-owned SDK root required')
        view = root/'tmp'/f'ros{version}-{args.view_tag}'
        if view.exists():
            raise RuntimeError('fresh native view path required')
        view.mkdir(mode=0o700)
        for target, row in targets.items():
            if target.startswith(f'uwvm-ros{version}-'):
                (view/target).symlink_to(row['path'])
                if require_arm(view/target)['sha256'] != row['sha256']:
                    raise RuntimeError('native view mismatch')
        views[version] = view
    args.out.mkdir(mode=0o700, parents=True, exist_ok=False)
    result['canonical_paths'] = {}
    result['native_views'] = {str(v): str(p) for v, p in views.items()}
    for version, view in views.items():
        canonical = view.parent/f'ros{version}-build'
        if not canonical.is_symlink():
            raise RuntimeError('canonical native path must be a task-owned symlink')
        result['canonical_paths'][str(version)] = str(canonical)
        result.setdefault('previous_canonical_paths', {})[str(version)] = str(canonical.resolve(strict=True))
    for version, view in views.items():
        pending = view.parent/f'ros{version}-{args.view_tag}.pending'
        pending.symlink_to(view.name)
        pending.replace(view.parent/f'ros{version}-build')
    (args.out/'build-report.json').write_text(json.dumps(result, indent=2)+'\n')
    (args.out/'workers').mkdir()
    (args.out/'workers/report.json').write_text(json.dumps(worker_result, indent=2)+'\n')
    print(json.dumps({'status': 'PASS', 'fresh_native': len(fresh_targets), 'reused_native': len(reused),
                      'reused_worker_profiles': 4, 'report': str(args.out/'build-report.json')}))


if __name__ == '__main__':
    main()
