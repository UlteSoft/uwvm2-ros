#!/usr/bin/env python3
"""Assemble 18 fresh current-source native products and materialize owned views.

Default mode emits a plan only. --run requires the explicitly pinned existing
Linux 64 GiB cgroup, zero swap, one boot/leaf incarnation and zero OOM deltas.
The 16 base probes retain project-dependency qualification only; two Gateway
products come from fresh system-inclusive targeted builds of the same source.
No compiler, binary, QEMU, Guest or ROS process is executed by this tool.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import re
import shlex
import sys

sys.dont_write_bytecode = True
from runtime import read_json_binding, require_arm, cgroup_incarnation, cgroup_oom_deltas
from gateway_qualification import (HISTORICAL_KEYS, architecture_binding, qualify_gateway,
                                   read_file_binding, pins_preserved)
from rebuild_affected import source_state, sdk_inventory, sdk_root_aliases

KINDS = {1: ('gateway','native-probe','resources-probe','tf-probe','action-probe',
             'perf-probe','ipc-service-probe','five-module-probe'),
         2: ('gateway','native-probe','resources-probe','tf-probe','action-probe',
             'lifecycle-probe','secure-probe','perf-probe','ipc-service-probe','five-module-probe')}
SOURCES = {'native-probe':'test/0073.ros-tests/support/source/ros{version}/native_probe.cc',
           'resources-probe':'test/0073.ros-tests/support/source/ros_resources/native_ros{version}.cc',
           'tf-probe':'test/0073.ros-tests/support/source/ros_tf/native_ros{version}.cc',
           'action-probe':'test/0073.ros-tests/support/source/ros_action/native_ros{version}.cc',
           'lifecycle-probe':'test/0073.ros-tests/support/source/ros_lifecycle/native_ros2.cc',
           'secure-probe':'test/0073.ros-tests/support/source/ros_security/native_secure_ros2.cc',
           'perf-probe':'test/0073.ros-tests/support/source/ros_perf/native_ros{version}.cc',
           'ipc-service-probe':'test/0073.ros-tests/support/source/ros_perf/ipc_service_ros{version}.cc',
           'five-module-probe':'test/0073.ros-tests/support/source/ros_multimodule/native_ros{version}.cc'}


def native_names():
    return {'uwvm-ros'+str(version)+'-'+kind for version, kinds in KINDS.items() for kind in kinds}


def qualify_base(path, manifest_path, source, sdk_workspace):
    """Retain 16 freshly compiled probe images, without inventing system closure."""
    row, binding = read_json_binding(path, limit=64*1024*1024)
    frozen, manifest_pin = read_json_binding(manifest_path)
    pins = [binding, manifest_pin]
    initial = source_state(source, frozen)
    if (HISTORICAL_KEYS & set(row) or row.get('status') != 'PASS'
            or row.get('source') != str(source) or row.get('sdk_workspace') != str(sdk_workspace)
            or row.get('source_manifest_sha256') != manifest_pin['sha256']
            or set(row.get('targets', {})) != native_names()):
        raise RuntimeError('fresh 18-product base report for exact current full source required')
    if (row.get('post_cleanup_mountinfo') != [] or not row.get('mount_cleanup')
            or any(item.get('returncode') != 0 for item in row['mount_cleanup'])):
        raise RuntimeError('base native private mounts did not retire')
    arch, architecture_pin = architecture_binding(row['execution_evidence'], pins)
    expected_commands = {f'ros{version}-{name}' for version in KINDS
                         for name in ('compiler-version','package-inventory','configure')}
    expected_commands |= {f'ros{version}-build-{kind}' for version,kinds in KINDS.items() for kind in kinds}
    builds = row.get('builds', [])
    if (len(builds) != len(expected_commands) or {item.get('name') for item in builds} != expected_commands
            or any(item.get('status') != 'PASS' or item.get('returncode') != 0 for item in builds)):
        raise RuntimeError('base native 24 fresh compile/configure commands incomplete')
    if set(row.get('driver_inputs', {})) != {'build_gateways.py','runtime.py'}:
        raise RuntimeError('base builder/helper inventory differs')
    for name, item in row['driver_inputs'].items():
        pin = read_file_binding(item['archive']); pins.append(pin)
        if pin['sha256'] != item['sha256'] or frozen.get('test/0073.ros-tests/support/source/ros_aarch64/'+name, {}).get('sha256') != pin['sha256']:
            raise RuntimeError('base executed recipe is not the current frozen helper')
    for item in row.get('build_metadata', {}).values():
        pin = read_file_binding(item['path']); pins.append(pin)
        if pin['sha256'] != item['sha256']: raise RuntimeError('base raw compiler metadata changed')
    targets, methods = {}, {}
    for version, kinds in KINDS.items():
        for kind in kinds:
            name = 'uwvm-ros'+str(version)+'-'+kind; facts = row['targets'][name]
            if require_arm(facts['path']) != facts: raise RuntimeError('base product image identity changed: '+name)
            pins.append(read_file_binding(facts['path']))
            if kind == 'gateway': continue  # Replaced only by a strict fresh targeted product.
            keys = [key for key in row.get('compiled_source_dependencies', {})
                    if key.startswith('ros'+str(version)+':CMakeFiles/'+name+'.dir/') and key.endswith('.d')]
            if len(keys) != 1: raise RuntimeError('one original project-MMD probe record required: '+name)
            key = keys[0]; metadata = row['build_metadata'][key]
            raw, dep_pin = read_file_binding(metadata['path'], retain=True); pins.append(dep_pin)
            body = raw.decode('utf-8').replace('\\\n', ' ').split(':', 1)[1]
            names = {os.path.normpath(item)[6:] for item in shlex.split(body) if item.startswith('/work/')}
            recorded = row['compiled_source_dependencies'][key]
            required = SOURCES[kind].format(version=version)
            if names != set(recorded) or required not in names:
                raise RuntimeError('base probe actual project dependency exactset differs: '+name)
            for relative, sha in recorded.items():
                if frozen.get(relative, {}).get('sha256') != sha or read_file_binding(source/relative)['sha256'] != sha:
                    raise RuntimeError('base probe dependency differs from current full source')
            command = next(item for item in builds if item['name'] == f'ros{version}-build-{kind}')
            observed = {label for sample in arch.get('actual_process_samples', [])
                        if sample.get('name') == command['name'] and sample.get('ancestry', [None])[0] == command['pid']
                        and sample.get('live_cgroup_procs') is True for label in sample.get('mapped_arm_targets', [])}
            if f'ros{version}-frontend' not in observed:
                raise RuntimeError('fresh base probe lacks actual compiler frontend subtree proof: '+name)
            targets[name] = facts
            methods[name] = {'method':'FRESH_CURRENT_BASE_PROJECT_MMD_ONLY', 'source_dependencies':recorded,
                             'raw_dependency':dep_pin, 'scope':'No system-header/full-SDK claim from -MMD'}
    if len(targets) != 16 or source_state(source,frozen) != initial or not pins_preserved(pins):
        raise RuntimeError('base native qualification inputs changed')
    return {'report':binding,'targets':targets,'methods':methods,'input_pins':pins,
            'architecture_evidence':architecture_pin,'source_manifest_binding':manifest_pin}


def assemble(base, targeted):
    """Combine exactly 16 base probes with two strict compiler Gateway products."""
    targets, methods = dict(base['targets']), dict(base['methods'])
    if set(targeted) != {1,2}: raise RuntimeError('both fresh targeted Gateway qualifications required')
    for version, item in targeted.items():
        if item.get('source_manifest_binding', {}).get('sha256') != base.get('source_manifest_binding', {}).get('sha256'):
            raise RuntimeError('base and targeted builds do not identify the same complete source manifest')
        name = 'uwvm-ros'+str(version)+'-gateway'
        if item['target'] != name: raise RuntimeError('targeted Gateway version/target differs')
        targets[name] = item['binary']
        methods[name] = {'method':item['source_dependency_method'], 'source_dependencies':item['source_dependencies'],
                         'targeted_report':item['report'], 'scope':item['scope']}
    if set(targets) != native_names() or len(targets) != 18:
        raise RuntimeError('complete current 18 native product inventory differs')
    return targets, methods


def alias_fact(path):
    """Bind an existing canonical symlink; refuse overwriting a real directory."""
    if path.is_symlink():
        state = path.lstat()
        return {'target':os.readlink(path),'device':state.st_dev,'inode':state.st_ino}
    if path.exists(): raise RuntimeError('canonical native view must be an owned symlink or absent')
    return None


def materialize_views(sdk_workspace, targets, tag, guard):
    """Stage both owned view directories, then atomically switch each symlink.

    The two filesystems cannot provide one cross-filesystem atomic transaction.
    Both views are fully qualified first; a failed switch rolls back only aliases
    still identified as this function's own new symlinks. Raw compiler products
    and reports are never copied over, changed or removed.
    """
    aliases, views, before, switched = {}, {}, {}, {}
    for version,distro in ((1,'noetic'),(2,'jazzy')):
        root = sdk_workspace/'gateway'/(distro+'-rootfs')
        if not (root/'.uwvm-task-owned').is_file(): raise RuntimeError('task-owned SDK marker required before view mutation')
        alias = root/'tmp'/('ros'+str(version)+'-build')
        aliases[version], before[version] = alias, alias_fact(alias)
        view = alias.parent/('ros'+str(version)+'-current-native-'+tag)
        if view.exists() or view.is_symlink(): raise RuntimeError('fresh native view tag required')
        views[version] = view
    try:
        for version, view in views.items():
            guard(); view.mkdir(mode=0o700)
            (view/'.uwvm-task-owned').write_text('Current-source native compilation view. No behavior verdict.\n')
            for kind in KINDS[version]:
                name = 'uwvm-ros'+str(version)+'-'+kind
                if require_arm(targets[name]['path']) != targets[name]: raise RuntimeError('qualified product changed before staging')
                (view/name).symlink_to(targets[name]['path'])
        for version, alias in aliases.items():
            guard()
            if alias_fact(alias) != before[version]: raise RuntimeError('canonical view changed before switch')
            stage = alias.with_name(alias.name+'-current-native-'+tag+'-pending')
            stage.symlink_to(views[version].name)
            stage.replace(alias); switched[version] = alias_fact(alias)
        guard()
        return {'canonical_paths':{str(v):str(path) for v,path in aliases.items()},
                'views':{str(v):str(path) for v,path in views.items()},
                'aliases_before':{str(v):value for v,value in before.items()},
                'aliases_after':{str(v):value for v,value in switched.items()}}
    except BaseException:
        for version in reversed(list(switched)):
            alias = aliases[version]
            if alias_fact(alias) != switched[version]: raise RuntimeError('owned canonical alias changed; rollback refused')
            previous = before[version]
            if previous is None: alias.unlink()
            else:
                rollback = alias.with_name(alias.name+'-current-native-'+tag+'-rollback')
                rollback.symlink_to(previous['target']); rollback.replace(alias)
        raise


def rollback_views(state):
    """Restore only still-owned new aliases after a later guard/write failure."""
    restored = []
    for version in reversed(list(state['canonical_paths'])):
        alias = Path(state['canonical_paths'][version])
        if alias_fact(alias) != state['aliases_after'][version]:
            raise RuntimeError('canonical alias no longer belongs to this qualification; rollback refused')
        previous = state['aliases_before'][version]
        if previous is None: alias.unlink()
        else:
            staging = alias.with_name(alias.name+'-qualification-rollback')
            if staging.exists() or staging.is_symlink(): raise RuntimeError('fresh rollback symlink path required')
            staging.symlink_to(previous['target']); staging.replace(alias)
        restored.append({'path':str(alias),'restored_target':previous['target'] if previous else None})
    return restored


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run',action='store_true')
    for name in ('source','manifest','base-report','ros1-report','ros2-report','sdk-workspace','out'):
        parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--view-tag',required=True)
    parser.add_argument('--expected-leaf',type=Path)
    parser.add_argument('--expected-device',type=int)
    parser.add_argument('--expected-inode',type=int)
    parser.add_argument('--expected-boot')
    args = parser.parse_args()
    if not re.fullmatch('[a-z0-9]+(?:-[a-z0-9]+)*',args.view_tag): parser.error('view-tag must be lowercase letters/digits/hyphens')
    if not args.run:
        print(json.dumps({'status':'PREPARED_PLAN_ONLY','scope':'No qualification PASS, compiler/ROS execution, output or alias mutation',
            'required_reports':['fresh current build_gateways 18-product report','fresh strict ROS 1 target','fresh strict ROS 2 target'],
            'native_products':sorted(native_names()),'view_tag':args.view_tag})); return 0
    if sys.platform != 'linux' or any(value is None for value in (args.expected_leaf,args.expected_device,args.expected_inode,args.expected_boot)):
        parser.error('--run requires Linux and explicitly pinned current cgroup leaf/device/inode/boot')
    source,sdk,out = args.source.resolve(strict=True),args.sdk_workspace.resolve(strict=True),args.out.resolve()
    if out.is_relative_to(source) or out.is_relative_to(sdk): parser.error('fresh output must remain outside source/SDK')
    frozen, manifest_pin = read_json_binding(args.manifest.resolve(strict=True))
    initial = source_state(source,frozen)
    spec = importlib.util.spec_from_file_location('current_native_probe',source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py')
    probe = importlib.util.module_from_spec(spec);spec.loader.exec_module(probe)
    leaf = args.expected_leaf.resolve(strict=True)
    expected = {'path':str(leaf),'device':args.expected_device,'inode':args.expected_inode,'boot_id':args.expected_boot}
    def current():
        if cgroup_incarnation(leaf) != expected: raise RuntimeError('current cgroup/boot identity differs from explicit pin')
        facts = probe.probe()
        if (facts.get('status') != 'PASS' or facts.get('effective_memory_max') != 68719476736
                or facts['cgroup_ancestors'][0]['path'] != str(leaf)
                or cgroup_incarnation(leaf) != expected
                or (leaf/'memory.swap.max').read_text().strip() != '0'):
            raise RuntimeError('current existing 64 GiB zero-swap cgroup not proved')
        return facts
    before = current(); all_pins = [manifest_pin]
    def guard():
        after = current(); valid,_ = cgroup_oom_deltas(before,after,True)
        if (not valid or before['membership'] != after['membership'] or not pins_preserved(all_pins)
                or source_state(source,frozen) != initial): raise RuntimeError('qualification input/cgroup/OOM guard failed')
    out.mkdir(mode=0o700,parents=True,exist_ok=False)
    report = {'status':'FAIL','scope':'Compilation qualification only; no native ROS/Guest behavior executed',
              'source':str(source),'source_manifest_sha256':manifest_pin['sha256'],'source_manifest_binding':manifest_pin,
              'sdk_workspace':str(sdk),'cgroup_before':before,'cgroup_incarnation':expected}
    views = None
    try:
        base = qualify_base(args.base_report.resolve(strict=True),args.manifest.resolve(strict=True),source,sdk)
        all_pins.extend(base['input_pins']); targeted = {}
        for version,path in ((1,args.ros1_report),(2,args.ros2_report)):
            target_report,_ = read_json_binding(path,limit=64*1024*1024)
            name = 'uwvm-ros'+str(version)+'-gateway'
            target_sdk = sdk/'gateway'/('noetic-rootfs' if version==1 else 'jazzy-rootfs')
            targeted[version] = qualify_gateway(target_report['targets'][name]['path'],path,args.manifest,source,target_sdk,version)
            all_pins.extend(targeted[version]['input_pins'])
        targets,methods = assemble(base,targeted);guard()
        def sdk_guard():
            for item in targeted.values():
                inventory,pin = read_json_binding(item['sdk_static_input_binding']['path'],limit=256*1024*1024)
                if (pin != item['sdk_static_input_binding'] or sdk_inventory(Path(item['sdk_root'])) != inventory
                        or sdk_root_aliases(Path(item['sdk_root'])) != item['sdk_root_aliases']):
                    raise RuntimeError('complete static SDK qualification inputs changed around view publication')
        sdk_guard()
        # Persist concrete qualification facts before publishing aliases. This
        # document is deliberately not PASS; a final guard/write failure must
        # leave reviewable facts without a successful materialization verdict.
        with (out/'qualified-inputs.json').open('x') as stream:
            stream.write(json.dumps({'status':'QUALIFIED_INPUTS_NOT_MATERIALIZED','targets':targets,
                'base_report':base['report'],'targeted_reports':{str(v):item['report'] for v,item in targeted.items()},
                'source_manifest_binding':manifest_pin,'qualified_source_dependency_methods':methods},indent=2)+'\n')
        views = materialize_views(sdk,targets,args.view_tag,guard)
        report.update(views,targets=targets,base_report=base['report'],
                      targeted_reports={str(v):item['report'] for v,item in targeted.items()},
                      qualified_source_dependency_methods=methods,input_pins=all_pins,
                      source_dependencies={name:item['source_dependencies'] for name,item in methods.items()})
        guard(); sdk_guard(); after = current(); valid,deltas = cgroup_oom_deltas(before,after,True)
        if not valid: raise RuntimeError('qualification final OOM proof failed')
        report.update(status='PASS',cgroup_after=after,same_cgroup_incarnation=True,ancestor_memory_event_deltas=deltas,
                      source_preserved=True,sdk_static_inputs_preserved=True,input_pins_preserved=True,actual_native_behavior='NOT_PERFORMED')
    except BaseException as error:
        report['error'] = str(error)
        if views is not None:
            try: report['alias_rollback'] = rollback_views(views)
            except BaseException as rollback_error: report['alias_rollback_error'] = str(rollback_error)
    try:
        with (out/'build-report.json').open('x') as stream: stream.write(json.dumps(report,indent=2)+'\n')
    except BaseException:
        if views is not None and report['status'] == 'PASS': rollback_views(views)
        raise
    print(json.dumps({'status':report['status'],'report':str(out/'build-report.json')}))
    return 0 if report['status']=='PASS' else 1


if __name__ == '__main__': sys.exit(main())
