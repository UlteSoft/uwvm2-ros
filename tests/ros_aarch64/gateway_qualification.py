"""Qualify a fresh current-source targeted Gateway compilation, read-only.

Feature runners and the 18-product wrapper share this validator. It accepts only
the original system-inclusive compilation, never historical-source reuse or a
dependency replay. It performs no compilation, process launch, mount or alias
mutation. The returned input_pins are complete opened-file bindings that callers
must recheck after using the qualified product.
"""
import hashlib
import os
from pathlib import Path
import posixpath
import shlex
import stat

from runtime import read_json_binding, require_arm, cgroup_oom_deltas, validate_architecture_mappings
from rebuild_affected import (build_profile, original_compile_command, dependency_output,
                             rooted, sdk_inventory, sdk_root_aliases, source_state,
                             custom_selection, custom_inputs_preserved, validate_generated_features)

HISTORICAL_KEYS = {'raw_build_report', 'dependency_replay', 'original_build_report',
                   'reuse_dependency_closure', 'original_compiler_dependency_records',
                   'raw_failure_classification'}


def read_file_binding(path, limit=512*1024*1024, retain=False):
    """Open nonblocking, admit a regular file, hash the same bounded descriptor.

    Binary/tool pins stream in 1 MiB chunks. Text callers explicitly retain bytes;
    oversized or concurrently edited inputs fail, including same-path changes.
    A pathname FIFO never reaches a blocking read-only open.
    """
    path = Path(path)
    flag = getattr(os, 'O_NONBLOCK', None)
    if not isinstance(flag, int) or flag <= 0:
        raise RuntimeError('input binding requires O_NONBLOCK')
    fd = os.open(path, os.O_RDONLY | flag)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= limit:
            raise RuntimeError('input must be an opened bounded regular file: '+str(path))
        value, chunks, count, left = hashlib.sha256(), [], 0, first.st_size+1
        while left:
            piece = os.read(fd, min(left, 1024*1024))
            if not piece: break
            value.update(piece); count += len(piece); left -= len(piece)
            if retain: chunks.append(piece)
        last = os.fstat(fd)
        if (not stat.S_ISREG(last.st_mode) or count != first.st_size or any(
                getattr(first, key) != getattr(last, key) for key in
                ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns'))):
            raise RuntimeError('input changed during descriptor binding: '+str(path))
    finally:
        os.close(fd)
    binding = {'path':str(path), 'device':last.st_dev, 'inode':last.st_ino,
               'bytes':count, 'mtime_ns':last.st_mtime_ns, 'ctime_ns':last.st_ctime_ns,
               'sha256':value.hexdigest()}
    return (b''.join(chunks), binding) if retain else binding


def pins_preserved(pins):
    """Reject disappeared, modified or same-byte inode-replaced control inputs."""
    return all(read_file_binding(item['path']) == item for item in pins)


def architecture_binding(path, pins):
    """Independently recompute same-incarnation zero OOM from raw architecture facts."""
    report, binding = read_json_binding(path, limit=64*1024*1024); pins.append(binding)
    identity = report.get('cgroup_incarnation_before')
    if (report.get('status') != 'PASS' or report.get('same_cgroup_incarnation') is not True
            or not identity or identity != report.get('cgroup_incarnation_after_before_probe')
            or identity != report.get('cgroup_incarnation_after')
            or report.get('fatal_errors') != [] or report.get('remaining_owned_processes') != []):
        raise RuntimeError('actual compilation architecture/incarnation/cleanup evidence incomplete')
    before, after = report['cgroup_before'], report['cgroup_after']
    if (before.get('status') != 'PASS' or after.get('status') != 'PASS'
            or before.get('effective_memory_max') != 68719476736
            or after.get('effective_memory_max') != 68719476736
            or before.get('membership') != after.get('membership')):
        raise RuntimeError('actual compiler inherited 64 GiB membership is unavailable')
    valid, deltas = cgroup_oom_deltas(before, after, True)
    if not valid or deltas != report.get('cgroup_ancestor_event_deltas'):
        raise RuntimeError('actual compilation OOM counter evidence differs or failed')
    validate_architecture_mappings(report)
    return report, binding


def custom_compiler_definitions(argv):
    """Read the actual compiler's custom macros without effective-value shortcuts.

    Duplicate definitions, later undefinitions and indirect preprocessor inputs
    cannot establish the paired generated-provider selection. Other ordinary
    compiler definitions remain subject to the original recipe validator.
    """
    definitions = {}
    index = 0
    while index < len(argv):
        token = argv[index]
        if (token.startswith('@') or token.startswith('-Wp,') or token == '-Xpreprocessor'
                or token.startswith('-include') or token.startswith('-imacros')):
            raise RuntimeError('indirect custom preprocessor selection is unsupported')
        if token in ('-D', '-U'):
            if index+1 == len(argv):
                raise RuntimeError('missing custom macro option operand')
            option, value = token, argv[index+1]
            index += 1
        elif token.startswith(('-D', '-U')):
            option, value = token[:2], token[2:]
        else:
            index += 1
            continue
        name, separator, body = value.partition('=')
        if name.startswith('UWVM_ROS_CUSTOM_'):
            if option != '-D' or not separator or name in definitions:
                raise RuntimeError('custom macro is duplicate, unquoted or undefined')
            definitions[name] = body
        index += 1
    return definitions


def custom_cache_entries(body):
    """Parse bounded archived CMake cache entries, rejecting duplicate keys."""
    entries = {}
    for line in body.decode('utf-8').splitlines():
        if not line or line.startswith(('#', '//')):
            continue
        key_type, separator, value = line.partition('=')
        key, colon, kind = key_type.partition(':')
        if not separator or not colon or not key or not kind or key in entries:
            raise RuntimeError('malformed or duplicate CMake cache entry')
        entries[key] = (kind, value)
    return entries


def qualify_custom_selection(row, report_path, source, sdk, frozen, profile, recipe,
                             project, headers, pins):
    """Bind optional generated topics to the same frozen inputs actually compiled.

    This creates no build, native entity or capability. Even a default build
    must explicitly report the producer's empty selection and carry empty cache
    options with no custom compiler macro. An enabled build must preserve the
    exact paired header/config descriptors, configure arguments, package prefix,
    native-package headers and the system-inclusive compiler dependencies.
    """
    version, prefix = profile['version'], profile['prefix']
    reported = row.get('custom_selection')
    if (not isinstance(reported, dict) or type(reported.get('enabled')) is not bool
            or type(reported.get('version')) is not int or reported['version'] != version
            or row.get('custom_inputs_preserved') is not True):
        raise RuntimeError('explicit preserved custom-selection report required')
    if reported['enabled']:
        try:
            packages = reported['packages']
            if not isinstance(packages, list) or not all(isinstance(item, str) for item in packages):
                raise RuntimeError('explicit custom package list required')
            selected = custom_selection(version, reported['registry_header']['guest_path'],
                reported['native_header']['guest_path'], packages,
                reported['prefix']['guest_path'], source, sdk, frozen)
        except (KeyError, TypeError) as error:
            raise RuntimeError('incomplete paired custom selection') from error
    else:
        selected = custom_selection(version, None, None, [], None, source, sdk, frozen)
    if selected != reported or not custom_inputs_preserved(selected):
        raise RuntimeError('custom selection differs from current exact pinned inputs')
    for item in selected['input_pins']:
        pin = read_file_binding(item['path'], limit=64*1024*1024)
        if pin != item:
            raise RuntimeError('selected custom header/config descriptor changed')
        pins.append(pin)

    cache_ref = row['build_metadata'][prefix+':CMakeCache.txt']
    cache_body, cache_pin = read_file_binding(cache_ref['path'], limit=64*1024*1024, retain=True)
    pins.append(cache_pin)
    if cache_pin['sha256'] != cache_ref['sha256']:
        raise RuntimeError('custom selection CMake cache archive changed')
    cache = custom_cache_entries(cache_body)
    native_key = 'UWVM_ROS_CUSTOM_ROS'+str(version)+'_HEADER'
    packages_key = 'UWVM_ROS_CUSTOM_ROS'+str(version)+'_PACKAGES'
    expected_cache = {'UWVM_ROS_CUSTOM_REGISTRY_HEADER':('FILEPATH', ''),
                      native_key:('FILEPATH', ''), packages_key:('STRING', '')}
    expected_macros = {}
    if selected['enabled']:
        expected_cache['UWVM_ROS_CUSTOM_REGISTRY_HEADER'] = ('FILEPATH', selected['registry_header']['guest_path'])
        expected_cache[native_key] = ('FILEPATH', selected['native_header']['guest_path'])
        expected_cache[packages_key] = ('STRING', ';'.join(selected['packages']))
        required_prefix = selected['prefix']['guest_path']+';/opt/ros/'+profile['distro']
        if cache.get('CMAKE_PREFIX_PATH', (None, None))[1] != required_prefix:
            raise RuntimeError('custom CMake package prefix differs')
        for item in selected['package_configs']:
            key = item['package']+'_DIR'
            expected_dir = posixpath.dirname(item['guest_path'])
            if cache.get(key) not in (('PATH', expected_dir), ('UNINITIALIZED', expected_dir)):
                raise RuntimeError('custom package config was discovered from another directory')
        for name, descriptor in (('UWVM_ROS_CUSTOM_TOPIC_MANIFEST', selected['registry_header']),
                                 (native_key, selected['native_header'])):
            guest = descriptor['guest_path']
            # The ROS1 CMake recipe resolves its provider paths before defining
            # include macros. ROS2 retains the selected guest path verbatim.
            if version == 1:
                guest = ('/work/'+str(Path(descriptor['input']['path']).relative_to(source))
                         if descriptor['scope'] == 'frozen_source'
                         else '/'+descriptor['sdk_relative'])
            expected_macros[name] = '"'+guest+'"'
            if guest.startswith('/work/'):
                relative = guest[6:]
                if project.get(relative) != descriptor['input']['sha256']:
                    raise RuntimeError('selected custom source header absent from actual -MD dependencies')
                # Bind the logical selected pathname too: a same-byte symlink
                # retarget must not survive the final opened-file pin recheck.
                pins.append(read_file_binding(source/descriptor['source_relative'], limit=64*1024*1024))
            else:
                expected_header = {'sha256':descriptor['input']['sha256'],
                                   'resolved_sdk_path':descriptor['sdk_relative'],
                                   'bytes':descriptor['input']['bytes']}
                if headers.get(guest) != expected_header:
                    raise RuntimeError('selected custom SDK header absent from actual -MD dependencies')
        include_prefix = selected['prefix']['sdk_relative']+'/include/'
        for package in selected['packages']:
            if not any(item['resolved_sdk_path'].startswith(include_prefix+package+'/')
                       for item in headers.values()):
                raise RuntimeError('selected custom package has no actual native header dependency')
    elif any(name.startswith('/opt/uwvm-custom-types/')
             or item['resolved_sdk_path'].startswith('opt/uwvm-custom-types/')
             for name, item in headers.items()):
        raise RuntimeError('default Gateway unexpectedly compiled selected-prefix custom headers')
    if any(cache.get(key) != value for key, value in expected_cache.items()):
        raise RuntimeError('custom paired cache options differ from selected inputs')
    if any(key.startswith('UWVM_ROS_CUSTOM_') and key not in expected_cache and value[1]
           for key, value in cache.items()):
        raise RuntimeError('unselected custom CMake option is enabled')
    definitions = custom_compiler_definitions(recipe['argv'])
    if definitions != expected_macros:
        raise RuntimeError('actual compiler custom definitions differ from paired selection')

    configure = [item for item in row.get('builds', []) if item.get('name') == prefix+'-configure']
    expected_command = ['/usr/bin/chroot', str(Path(report_path).parent/'rootfs'), '/bin/bash', '-lc',
        'source /opt/ros/'+profile['distro']+'/setup.bash && exec "$@"', 'uwvm-affected-build',
        'cmake', '-S', '/work/gateways/'+prefix, '-B', profile['build_guest'],
        '-DCMAKE_CXX_COMPILER='+profile['compiler'], '-DCMAKE_BUILD_TYPE=RelWithDebInfo',
        '-DCMAKE_CXX_FLAGS='+' '.join(profile['dependency_flags']),
        '-DCMAKE_CXX_FLAGS_RELWITHDEBINFO=-O2 -g0 -DNDEBUG', '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON',
        *selected['cmake_arguments']]
    if (len(configure) != 1 or configure[0].get('status') != 'PASS'
            or configure[0].get('returncode') != 0 or configure[0].get('command') != expected_command):
        raise RuntimeError('actual configure argv differs from exact paired custom selection')
    return {'selection':selected, 'cmake_cache_binding':cache_pin,
            'actual_compiler_definitions':definitions,
            'scope':'Build support selection only; no ROS permission or runtime behavior claim'}


def qualify_gateway(path, report_path, manifest_path, source, sdk, version):
    """Return one strict current compiler product and all immutable evidence pins.

    The source/keyset, static SDK/keyset/root aliases, actual final -MF, depfile
    object rule, exact project/system sets, codegen controls, link inputs, tools,
    compiler PID-subtree mappings and executed helper archives are rechecked.
    This qualifies compilation only; it does not establish ROS behavior.
    """
    source, sdk, path = (Path(item).resolve(strict=True) for item in (source, sdk, path))
    manifest_path, report_path = Path(manifest_path).resolve(strict=True), Path(report_path).resolve(strict=True)
    frozen, manifest_pin = read_json_binding(manifest_path)
    row, report_pin = read_json_binding(report_path, limit=64*1024*1024)
    pins = [manifest_pin, report_pin, read_file_binding(path)]
    profile = build_profile(version); prefix, target = profile['prefix'], profile['target']
    facts = require_arm(path)
    if (HISTORICAL_KEYS & set(row) or row.get('status') != 'PASS' or row.get('version') != version
            or row.get('profile') != profile or row.get('target') != target
            or row.get('source') != str(source) or row.get('source_manifest_sha256') != manifest_pin['sha256']
            or set(row.get('targets', {})) != {target} or row['targets'][target] != facts
            or Path(row.get('source_manifest', '')).resolve() != manifest_path
            or Path(row.get('sdk_workspace', '')).resolve()/'gateway'/(profile['distro']+'-rootfs') != sdk):
        raise RuntimeError('exact fresh current-source targeted Gateway report required')
    initial = source_state(source, frozen)
    expected = ('source_preserved', 'sdk_static_preserved', 'sdk_root_aliases_preserved',
                'same_cgroup_incarnation', 'system_headers_recorded_in_original_depfile')
    if not all(row.get(name) is True for name in expected):
        raise RuntimeError('Gateway source/SDK/system-header/cgroup preservation incomplete')
    if (row.get('post_cleanup_mountinfo') != [] or not row.get('mount_cleanup')
            or any(item.get('returncode') != 0 for item in row['mount_cleanup'])):
        raise RuntimeError('Gateway compilation mount cleanup incomplete')
    arch, architecture_pin = architecture_binding(row['execution_evidence'], pins)
    inventory_ref = row['sdk_static_inputs']
    inventory, inventory_pin = read_json_binding(inventory_ref['path'], limit=256*1024*1024); pins.append(inventory_pin)
    if (inventory_pin['sha256'] != inventory_ref['sha256'] or inventory != sdk_inventory(sdk)
            or row.get('sdk_root_aliases_before') != sdk_root_aliases(sdk)):
        raise RuntimeError('Gateway complete static SDK keyset/aliases differ')
    aliases = row['sdk_root_aliases_before']
    metadata, records = row['build_metadata'], row['compiler_dependency_records']
    if len(records) != 1: raise RuntimeError('one actual system-inclusive Gateway depfile required')
    key, record = next(iter(records.items()))
    if not key.startswith(prefix+':CMakeFiles/'+target+'.dir/'):
        raise RuntimeError('Gateway depfile belongs to another target')
    dep = metadata[key]; body, dep_pin = read_file_binding(dep['path'], retain=True); pins.append(dep_pin)
    database_ref = metadata[prefix+':compile_commands.json']
    database, database_pin = read_json_binding(database_ref['path']); pins.append(database_pin)
    command_ref = row['actual_compiler_command']
    log, log_pin = read_file_binding(command_ref['log'], limit=64*1024*1024, retain=True); pins.append(log_pin)
    if (dep_pin['sha256'] != dep['sha256'] or dep_pin['sha256'] != record['sha256']
            or database_pin['sha256'] != database_ref['sha256'] or log_pin['sha256'] != command_ref['log_sha256']):
        raise RuntimeError('Gateway raw compilation dependency/recipe bytes changed')
    recipe = original_compile_command(profile, database, log.decode('utf-8'))
    if any(recipe[name] != command_ref[name] for name in ('directory', 'argv', 'options', 'verbose_log_line')):
        raise RuntimeError('Gateway actual verbose compiler recipe differs')
    output = dependency_output(profile, recipe)
    expected_method = ('Actual GCC'+str(profile['gcc'])+' compilation -MD/-MF output selected from '
                       'the successful verbose compiler argv; not CMake scanner output and not preprocessing replay')
    if (key != prefix+':'+output['archive_relative']
            or row.get('actual_compiler_depfile_path') != output['actual_guest_output']
            or any(record.get(name) != value for name, value in output.items())
            or record.get('dependency_output_options') != recipe['options']['dependency_output_options']
            or record.get('mf_outputs_in_argv_order') != recipe['options']['mf_outputs_in_argv_order']
            or record.get('method') != expected_method):
        raise RuntimeError('Gateway recorded final effective -MF differs from actual compiler argv')
    actual_dep = report_path.parent/'owned-tmp'/output['owned_tmp_relative']
    actual_dep_pin = read_file_binding(actual_dep); pins.append(actual_dep_pin)
    if (record.get('actual_host_output') != str(actual_dep)
            or row.get('actual_compiler_depfile_host_path') != str(actual_dep)
            or actual_dep_pin['sha256'] != dep_pin['sha256']):
        raise RuntimeError('original actual compiled depfile differs from archived record')
    target_text, input_text = body.decode('utf-8').replace('\\\n', ' ').split(':', 1)
    object_path = posixpath.normpath(posixpath.join(recipe['directory'], recipe['options']['object_output']))
    if ([posixpath.normpath(posixpath.join(recipe['directory'], item)) for item in shlex.split(target_text)] != [object_path]
            or record.get('compiled_object_guest_path') != object_path):
        raise RuntimeError('Gateway actual depfile rule does not identify compiled object')
    generated = validate_generated_features(profile, row.get('generated_feature_dependencies', {}),
        report_path.parent, source, frozen, manifest_pin['sha256'], row.get('builds', []), pins)
    if version == 2 and row.get('generated_feature_inputs_preserved') is not True:
        raise RuntimeError('Gateway generated configure inputs were not preserved')
    if version == 2 and (metadata[prefix+':CMakeCache.txt']['sha256'] !=
            row['generated_feature_dependencies'][profile['build_guest']+'/uwvm_ros2_features.h']
                ['inputs']['CMakeCache.txt']['actual']['sha256']):
        raise RuntimeError('Gateway archived configure cache differs from generated-feature provenance')
    project, headers, seen_generated = {}, {}, set()
    for token in shlex.split(input_text):
        logical = posixpath.normpath(token)
        if logical.startswith('/work/'):
            relative = logical[6:]
            if relative not in frozen or read_file_binding(source/relative)['sha256'] != frozen[relative]['sha256']:
                raise RuntimeError('Gateway dependency is outside current exact frozen source')
            project[relative] = frozen[relative]['sha256']
        elif logical in generated:
            seen_generated.add(logical)
        elif logical.startswith(('/usr/', '/opt/', '/etc/', '/lib/', '/lib64/', '/bin/', '/sbin/')):
            actual = rooted(sdk, logical); relative = str(actual.relative_to(sdk))
            pin = read_file_binding(actual); before = inventory.get(relative, {})
            if before.get('sha256') != pin['sha256'] or before.get('bytes') != pin['bytes']:
                raise RuntimeError('Gateway SDK dependency differs from complete before-build snapshot')
            headers[logical] = {'sha256':pin['sha256'], 'resolved_sdk_path':relative, 'bytes':pin['bytes']}
        else: raise RuntimeError('unclassified Gateway actual compiler dependency')
    if seen_generated != set(generated):
        raise RuntimeError('Gateway actual depfile omits its configured feature header')
    required = {'gateways/'+prefix+'/main.cc', 'gateways/'+prefix+'/action.h',
                'gateways/'+prefix+'/resources.h', 'gateways/common/config.h'}
    if version == 1: required.add('src/uwvm2/imported/ros/common/parameter_search_contract.h')
    if (not required <= set(project) or row.get('compiled_source_dependencies') != {key:project}
            or headers != row.get('sdk_header_dependencies')
            or not any(name.startswith('/usr/include/c++/'+str(profile['gcc'])+'/') for name in headers)
            or not any(name.startswith('/opt/ros/'+profile['distro']+'/include/') for name in headers)):
        raise RuntimeError('Gateway actual project/system header exactset incomplete or changed')
    custom = qualify_custom_selection(row, report_path, source, sdk, frozen, profile,
                                      recipe, project, headers, pins)
    link_ref = metadata[prefix+':CMakeFiles/'+target+'.dir/link.txt']
    link, link_pin = read_file_binding(link_ref['path'], retain=True); pins.append(link_pin)
    linked = {}
    for token in shlex.split(link.decode('utf-8')):
        if token.startswith(('/usr/', '/opt/')):
            selected = rooted(sdk, token)
            if selected.is_file():
                pin = read_file_binding(selected); relative = str(selected.relative_to(sdk))
                if inventory.get(relative, {}).get('sha256') != pin['sha256']:
                    raise RuntimeError('Gateway scalar SDK link input changed')
                linked[token] = {'sha256':pin['sha256'], 'resolved_sdk_path':relative}
    if linked != row.get('sdk_link_inputs'): raise RuntimeError('Gateway scalar link input exactset differs')
    categories = {'standard_cplusplus':sum(name.startswith('/usr/include/c++/'+str(profile['gcc'])+'/') for name in headers),
                  'ros_sdk':sum(name.startswith('/opt/ros/'+profile['distro']+'/include/') for name in headers),
                  'total':len(headers)}
    if row.get('system_header_dependency_categories') != categories:
        raise RuntimeError('Gateway system-header categories differ from actual depfile')
    for value in metadata.values():
        pin = read_file_binding(value['path']); pins.append(pin)
        if pin['sha256'] != value['sha256']: raise RuntimeError('Gateway codegen/link metadata changed')
    drivers = ('rebuild_affected.py', 'runtime.py', 'run_resources.py')
    if set(row.get('driver_inputs', {})) != set(drivers): raise RuntimeError('Gateway executed helper inventory differs')
    for name in drivers:
        item = row['driver_inputs'][name]; pin = read_file_binding(item['archive']); pins.append(pin)
        frozen_name = 'tests/ros_aarch64/'+name
        if pin['sha256'] != item['sha256'] or frozen.get(frozen_name, {}).get('sha256') != pin['sha256']:
            raise RuntimeError('Gateway executed helper is not the current frozen recipe')
    compiler = rooted(sdk, profile['compiler'])
    frontend = next((rooted(sdk, '/'+base+'/gcc/aarch64-linux-gnu/'+str(profile['gcc'])+'/cc1plus')
                     for base in ('usr/lib', 'usr/libexec') if rooted(sdk, '/'+base+'/gcc/aarch64-linux-gnu/'+str(profile['gcc'])+'/cc1plus').is_file()), None)
    if frontend is None: raise RuntimeError('current SDK compiler frontend unavailable')
    tools = {prefix+'-compiler':compiler, prefix+'-frontend':frontend, prefix+'-cmake':rooted(sdk, '/usr/bin/cmake')}
    for label, tool in tools.items():
        pin = read_file_binding(tool); pins.append(pin)
        if arch.get('arm_targets', {}).get(label) != require_arm(tool): raise RuntimeError('compiled tool image identity changed')
    compile_rows = [item for item in row.get('builds', []) if item.get('name') == prefix+'-build-gateway']
    if len(compile_rows) != 1 or compile_rows[0].get('status') != 'PASS' or compile_rows[0].get('returncode') != 0:
        raise RuntimeError('actual successful Gateway compile launch missing')
    launch = compile_rows[0]
    samples = [item for item in arch.get('actual_process_samples', [])
               if item.get('name') == launch['name']+'/' and item.get('ancestry', [None])[0] == launch['pid']]
    emulator = row['emulator']; emulator_pin = read_file_binding(emulator['path']); pins.append(emulator_pin)
    if (emulator.get('e_machine') != 62 or emulator_pin['sha256'] != emulator.get('sha256')
            or emulator.get('sha256') not in arch.get('expected_host_emulator_sha256', [])):
        raise RuntimeError('actual compilation host emulator bytes differ')
    positive = [item for item in samples if item.get('mapped_arm_targets')]
    if not positive or any(item.get('kernel_exe_elf', {}).get('e_machine') != 62
                           or item.get('kernel_exe_elf', {}).get('sha256') != emulator['sha256'] for item in positive):
        raise RuntimeError('compiler mapping samples do not identify qualified x86_64 QEMU')
    mapped = {name for item in samples if item.get('live_cgroup_procs') is True
              and item.get('membership') == arch['cgroup_before']['membership']
              for name in item.get('mapped_arm_targets', [])}
    if not {prefix+'-compiler', prefix+'-frontend'} <= mapped:
        raise RuntimeError('actual Gateway compiler/frontend PID-subtree mapping proof incomplete')
    if (source_state(source, frozen) != initial or sdk_inventory(sdk) != inventory
            or sdk_root_aliases(sdk) != aliases or not custom_inputs_preserved(custom['selection'])
            or not pins_preserved(pins)):
        raise RuntimeError('qualification source/SDK/control inputs changed during review')
    return {'target':target, 'version':version, 'binary':facts, 'report':report_pin,
            'source_manifest_binding':manifest_pin, 'source_dependencies':project,
            'sdk_header_dependencies':headers, 'sdk_link_inputs':linked,
            'generated_feature_dependencies':generated,
            'sdk_root':str(sdk), 'sdk_static_input_binding':inventory_pin, 'sdk_root_aliases':aliases,
            'source_dependency_method':'ACTUAL_GCC_SYSTEM_INCLUSIVE_MD_FINAL_MF_CURRENT_FULL_SOURCE',
            'architecture_evidence':architecture_pin, 'input_pins':pins,
            'custom_selection_binding':custom,
            'scope':'Current targeted compilation qualification only; no ROS behavior claim'}
