#!/usr/bin/env python3
"""Qualify a successful ARM compilation whose MMD system-header guard failed."""
import argparse
import copy
import json
from pathlib import Path
import posixpath
import re
import sys

sys.dont_write_bytecode = True

from runtime import digest, require_arm
from rebuild_affected import rooted


def reference(path):
    path = Path(path).resolve(strict=True)
    return {'path': str(path), 'sha256': digest(path)}


def inputs(path, prefix):
    text = Path(path).read_text().replace('\\\n', ' ')
    return {posixpath.normpath(name.rstrip(':'))[len(prefix):]
            for name in re.findall(re.escape(prefix)+r'[^\s\\]+', text)
            if not name.rstrip(':').endswith('.o')}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('raw-build-report', 'dependency-replay', 'out'):
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    raw = json.loads(args.raw_build_report.read_text())
    replay = json.loads(args.dependency_replay.read_text())
    if raw['status'] != 'FAIL' or not raw.get('error', '').endswith(
            'RuntimeError: incomplete Gateway actual compiler dependency closure\n'):
        raise RuntimeError('only the preserved MMD system-header guard failure is eligible')
    if replay['status'] != 'PASS' or not replay['project_set_matches_original_MMD'] or not replay['no_object_outputs']:
        raise RuntimeError('successful original-flags dependency-only replay required')
    for document in (raw, replay):
        if not all(document.get(name) is True for name in
                   ('source_preserved', 'sdk_static_preserved', 'same_cgroup_incarnation')):
            raise RuntimeError('source/SDK/cgroup preservation required')
        if document['post_cleanup_mountinfo'] or any(row['returncode'] for row in document['mount_cleanup']):
            raise RuntimeError('private mount cleanup incomplete')
        evidence = json.loads(Path(document['execution_evidence']).read_text())
        if evidence['fatal_errors'] or evidence['remaining_owned_processes']:
            raise RuntimeError('actual ARM execution evidence failed')
        for row in evidence['cgroup_ancestor_event_deltas']:
            if any(row.get(name, 0) for name in ('oom', 'oom_kill', 'oom_group_kill')):
                raise RuntimeError('inherited cgroup OOM counters increased')
    if not replay['original_report_binary_metadata_preserved']:
        raise RuntimeError('dependency replay modified an original artifact')
    if any(row.get('returncode') != 0 or row['status'] != 'PASS' for row in raw['builds']):
        raise RuntimeError('original actual ARM compiler commands failed')
    compilation = next(row for row in raw['builds'] if row['name'] == 'ros2-build-gateway')
    actual_evidence = json.loads(Path(raw['execution_evidence']).read_text())
    subtree = [row for row in actual_evidence['actual_process_samples']
               if row['name'] == compilation['name']+'/' and row['ancestry'][0] == compilation['pid']]
    positive = [row for row in subtree if row['mapped_arm_targets']]
    mapped = {name for row in positive for name in row['mapped_arm_targets']}
    if {'ros2-compiler', 'ros2-frontend'}-mapped:
        raise RuntimeError('exact original compile PID subtree lacks ARM GCC13/frontend mapping')
    if any(row.get('live_cgroup_procs') is not True for row in positive):
        raise RuntimeError('positive compiler samples lack live cgroup.procs evidence')
    source = Path(raw['source']).resolve(strict=True)
    manifest_path = Path(raw['source_manifest'])
    manifest = json.loads(manifest_path.read_text())
    if digest(manifest_path) != raw['source_manifest_sha256']:
        raise RuntimeError('original frozen source manifest changed')
    for name, row in manifest.items():
        if digest(source/name) != row['sha256']:
            raise RuntimeError('original compilation source changed: '+name)
    target = 'uwvm-ros2-gateway'
    dependency = replay['dependencies'][target]
    record = raw['build_metadata']['ros2:CMakeFiles/'+target+'.dir/main.cc.o.d']
    if digest(record['path']) != record['sha256'] or record['sha256'] != dependency['original_MMD_sha256']:
        raise RuntimeError('original actual MMD record changed')
    project = dependency['source_dependencies']
    if inputs(record['path'], '/work/') != set(project) or inputs(dependency['path'], '/work/') != set(project):
        raise RuntimeError('original compilation and replay project dependency sets differ')
    for name, value in project.items():
        if value != manifest[name]['sha256'] or digest(source/name) != value:
            raise RuntimeError('project dependency byte mismatch')
    sdk = Path(raw['sdk_workspace'])/'gateway/jazzy-rootfs'
    for logical, row in dependency['sdk_header_dependencies'].items():
        path = rooted(sdk, logical)
        if str(path.relative_to(sdk)) != row['resolved_sdk_path'] or digest(path) != row['sha256']:
            raise RuntimeError('replayed SDK header changed: '+logical)
    if not dependency['sdk_header_dependencies']:
        raise RuntimeError('system SDK header closure missing')
    for row in raw['build_metadata'].values():
        if digest(row['path']) != row['sha256']:
            raise RuntimeError('original compiler metadata changed')
    binary = require_arm(raw['targets'][target]['path'])
    if binary != raw['targets'][target] or binary != replay['original_binary']:
        raise RuntimeError('original compiled ARM binary changed')
    result = copy.deepcopy(raw)
    result.update(status='PASS',
        scope='Qualified compiled artifact/source/SDK dependencies only; original raw report remains FAIL '
              'because -MMD omits system headers. Original-flags GCC -M replay supplies SDK header closure. No ROS behavior claimed.',
        raw_build_report=reference(args.raw_build_report), dependency_replay=reference(args.dependency_replay),
        raw_failure_classification={'kind': 'MMD excludes -isystem SDK headers', 'original_error': raw['error']},
        compiled_source_dependencies={'ros2:CMakeFiles/'+target+'.dir/main.cc.o.d': project},
        sdk_header_dependencies=dependency['sdk_header_dependencies'],
        original_compilation_subtree_sample_count=len(subtree),
        original_compilation_arm_sample_count=len(positive),
        qualification_driver=reference(__file__))
    result.pop('error', None)
    args.out.mkdir(mode=0o700, parents=True, exist_ok=False)
    report = args.out/'build-report.json'
    report.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps({'status': 'PASS', 'compiled_project_inputs': len(project),
                      'SDK_header_inputs': len(dependency['sdk_header_dependencies']), 'report': str(report)}))


if __name__ == '__main__':
    main()
