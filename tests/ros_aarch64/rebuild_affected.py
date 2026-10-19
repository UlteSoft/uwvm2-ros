#!/usr/bin/env python3
"""Rebuild one ROS1/ROS2 Gateway from a new frozen source tree."""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import posixpath
import re
import shlex
import shutil
import signal
import stat
import subprocess
import sys
import time
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, digest, require_arm
from run_resources import bind_frozen_work

def build_profile(version):
    if version not in (1, 2):
        raise ValueError('ROS version must be 1 or 2')
    prefix = 'ros'+str(version)
    gcc = 9 if version == 1 else 13
    return {'version': version, 'prefix': prefix,
            'distro': 'noetic' if version == 1 else 'jazzy', 'gcc': gcc,
            'target': 'uwvm-'+prefix+'-gateway',
            'compiler': '/usr/bin/aarch64-linux-gnu-g++-'+str(gcc),
            'build_guest': '/tmp/'+prefix+'-affected-build',
            'dependency_flags': ['-MD', '-MF', '/tmp/gateway-origin.d']}


def compiler_options(tokens):
    """Read dependency output and effective codegen controls in argv order."""
    dependency, outputs, object_outputs, codegen = [], [], [], []
    optimization = debug = standard = ndebug = None
    index = 0
    while index < len(tokens):
        token = tokens[index]
        if token.startswith('@') or token.startswith('-Wp,') or token == '-Xpreprocessor':
            raise RuntimeError('opaque compiler dependency options are not supported')
        if token in ('-M', '-MM', '-MMD', '-E', '-S', '-fsyntax-only', '-MG', '-MP'):
            raise RuntimeError('actual compilation must use system-inclusive -MD without '+token)
        if token == '-MD':
            dependency.append(token)
        elif token in ('-MF', '-MT', '-MQ', '-o', '-D', '-U'):
            if index+1 == len(tokens) or not tokens[index+1]:
                raise RuntimeError('missing compiler option operand: '+token)
            value = tokens[index+1]
            if token in ('-MF', '-MT', '-MQ'):
                dependency.extend((token, value))
                if token == '-MF':
                    outputs.append(value)
            else:
                codegen.extend((token, value))
                if token == '-o':
                    object_outputs.append(value)
                elif value.split('=', 1)[0] == 'NDEBUG':
                    ndebug = token == '-D' and value in ('NDEBUG', 'NDEBUG=1')
            index += 1
        elif token.startswith(('-MF', '-MT', '-MQ')):
            option, value = token[:3], token[3:]
            dependency.append(token)
            if option == '-MF':
                outputs.append(value)
        else:
            codegen.append(token)
            if token.startswith('-o') and len(token) > 2:
                object_outputs.append(token[2:])
            if token.startswith('-O'):
                optimization = token
            elif token.startswith('-g'):
                debug = token
            elif token.startswith('-std='):
                standard = token
            elif token.startswith(('-D', '-U')) and token[2:].split('=', 1)[0] == 'NDEBUG':
                ndebug = token in ('-DNDEBUG', '-DNDEBUG=1')
        index += 1
    if '-MD' not in dependency or not outputs or not outputs[-1] or outputs[-1] == '-':
        raise RuntimeError('actual GCC compilation requires -MD and an explicit file -MF')
    if optimization != '-O2' or debug != '-g0' or standard not in ('-std=c++17', '-std=gnu++17') or not ndebug:
        raise RuntimeError('effective original C++17/O2/g0/NDEBUG compilation controls differ')
    if len(object_outputs) != 1:
        raise RuntimeError('exactly one actual compilation object output required')
    return {'dependency_output_options': dependency, 'mf_outputs_in_argv_order': outputs,
            'effective_mf': outputs[-1], 'object_output': object_outputs[0],
            'effective_codegen': {'optimization': optimization, 'debug': debug,
                                  'standard': standard, 'ndebug': ndebug},
            'codegen_tokens_without_dependency_options': codegen}


def original_compile_command(profile, database, log):
    """Use the successful verbose recipe; compile_commands can omit CMake -MF."""
    source = '/work/gateways/'+profile['prefix']+'/main.cc'
    entries = [row for row in database if posixpath.normpath(row['file']) == source]
    if len(entries) != 1 or posixpath.normpath(entries[0]['directory']) != profile['build_guest']:
        raise RuntimeError('exact original Gateway compile database entry required')
    entry = entries[0]
    database_tokens = entry.get('arguments') or shlex.split(entry['command'])
    candidates = []
    for number, line in enumerate(log.splitlines(), 1):
        if profile['compiler'] not in line or source not in line:
            continue
        tokens = shlex.split(line)
        directory = entry['directory']
        if len(tokens) > 3 and tokens[0] == 'cd' and tokens[2] == '&&':
            directory, tokens = tokens[1], tokens[3:]
        if not tokens or tokens[0] != profile['compiler'] or '-c' not in tokens:
            continue
        if tokens.count('-c') != 1 or tokens[tokens.index('-c')+1:] != [source]:
            raise RuntimeError('unexpected source or command suffix in actual GCC recipe')
        if posixpath.normpath(directory) != profile['build_guest']:
            raise RuntimeError('actual GCC recipe directory differs from compile database')
        actual_options = compiler_options(tokens)
        database_options = compiler_options(database_tokens)
        if actual_options['codegen_tokens_without_dependency_options'] != database_options['codegen_tokens_without_dependency_options']:
            raise RuntimeError('actual GCC codegen argv differs from the compile database')
        candidates.append({'directory': directory, 'argv': tokens,
                           'verbose_log_line': number, 'options': actual_options})
    if len(candidates) != 1:
        raise RuntimeError('exactly one actual verbose GCC Gateway compilation required')
    return candidates[0]


def dependency_output(profile, command):
    """Select the last effective -MF, which CMake may append after CXX_FLAGS."""
    output = command['options']['effective_mf']
    guest = posixpath.normpath(posixpath.join(command['directory'], output))
    if guest == '/tmp/gateway-origin.d':
        relative = 'CMakeFiles/'+profile['target']+'.dir/gateway-origin.d'
    elif guest.startswith(profile['build_guest']+'/'):
        relative = posixpath.relpath(guest, profile['build_guest'])
        if not relative.startswith('CMakeFiles/'+profile['target']+'.dir/') or not relative.endswith('.d'):
            raise RuntimeError('actual compiler depfile is outside target metadata')
    else:
        raise RuntimeError('actual compiler depfile is outside the owned build outputs')
    return {'actual_guest_output': guest, 'archive_relative': relative,
            'owned_tmp_relative': posixpath.relpath(guest, '/tmp'),
            'requested_origin_output': '/tmp/gateway-origin.d',
            'cmake_overrode_requested_origin': guest != '/tmp/gateway-origin.d'}


# Only this configure_file product is a compiler dependency outside source/SDK.
# Feature values are observations of the selected SDK, never new Guest rights.
ROS2_FEATURE_MACROS = {
    'qos_class': 'QOS', 'complete_rmw_qos': 'COMPLETE_RMW_QOS', 'node_options': 'NODE_OPTIONS',
    'quiet_node': 'QUIET_NODE', 'parameter_declare': 'PARAMETER_DECLARE',
    'dynamic_typing': 'DYNAMIC_TYPING', 'post_set_parameters': 'POST_SET',
    'type_description': 'TYPE_DESCRIPTION', 'deferred_service_callback': 'DEFERRED_SERVICE',
    'legacy_deferred_service': 'LEGACY_DEFERRED_SERVICE', 'enclave_argument': 'ENCLAVE_ARGUMENT',
    'action': 'ACTION', 'action_core':'ACTION_CORE', 'custom_action':'CUSTOM_ACTION', 'lifecycle': 'LIFECYCLE', 'action_graph': 'ACTION_GRAPH',
    'tf_buffer': 'TF', 'actual_service_qos': 'ACTUAL_SERVICE_QOS', 'openssl_rihs': 'OPENSSL'}
ROS2_FEATURE_SOURCES = ('gateways/ros2/CMakeLists.txt', 'gateways/ros2/compat/Probe.cmake',
                        'gateways/ros2/compat/uwvm_ros2_features.h.in',
                        'gateways/ros2/compat/action_probe.h', 'gateways/ros2/compat/action_catalog.h','gateways/ros2/compat/action_client.h',
                        'gateways/ros2/compat/action_client_lease.h')


def generated_feature_pin(path, limit=16*1024*1024):
    """Generated inputs must be physical owned files, including all parents."""
    path = Path(path)
    if not path.is_absolute() or path.resolve(strict=True) != path:
        raise RuntimeError('generated-feature input must have an exact physical path')
    pin = custom_file_pin(path)
    if pin['bytes'] > limit or path.resolve(strict=True) != path:
        raise RuntimeError('generated-feature input exceeds its bound or changed path')
    return pin


def generated_feature_files(profile):
    """Closed logical build products; ordinary temporary headers remain refused."""
    return ('uwvm_ros2_features.h', 'uwvm_ros2_features.json', 'CMakeCache.txt') if profile['version'] == 2 else ()


def validate_feature_contents(source, actual_files):
    """Check the real configure output against its frozen template and cache.

    This does not run CMake or its probes.  The independent compile/link probes
    already ran during the recorded configure command; their cache results and
    companion JSON must explain every byte of the actual included header.
    """
    def pairs(items):
        result = {}
        for key, value in items:
            if key in result: raise RuntimeError('duplicate generated feature JSON key')
            result[key] = value
        return result
    features = json.loads(actual_files['uwvm_ros2_features.json'].read_bytes(), object_pairs_hook=pairs)
    if (type(features) is not dict or set(features) != {'format_version','backend','cxx_standard','features'}
            or type(features['format_version']) is not int or features['format_version'] != 1
            or features['backend'] != 'ros2' or type(features['cxx_standard']) is not int
            or features['cxx_standard'] != 17 or type(features['features']) is not dict
            or set(features['features']) != set(ROS2_FEATURE_MACROS)
            or any(type(value) is not bool for value in features['features'].values())):
        raise RuntimeError('closed C++17 ROS2 generated feature manifest required')
    cache = {}
    for line in actual_files['CMakeCache.txt'].read_text().splitlines():
        if not line or line.startswith(('#','//')): continue
        key_type, separator, value = line.partition('=')
        key, colon, kind = key_type.partition(':')
        if not separator or not colon or not key or not kind or key in cache:
            raise RuntimeError('malformed generated-feature configure cache')
        cache[key] = value
    expected = {'CMAKE_HOME_DIRECTORY':'/work/gateways/ros2',
                'CMAKE_CXX_COMPILER':'/usr/bin/aarch64-linux-gnu-g++-13',
                'CMAKE_BUILD_TYPE':'RelWithDebInfo', 'UWVM_ROS2_CXX_17':'1'}
    if any(cache.get(key) != value for key, value in expected.items()):
        raise RuntimeError('generated feature cache belongs to another source/compiler/standard')
    for name, macro in ROS2_FEATURE_MACROS.items():
        raw = cache.get('UWVM_ROS2_HAVE_'+macro+'_COMPILE_LINK')
        # OpenSSL's optional package may not exist. All other declared probes
        # run unconditionally; an absent result is not an observed false probe.
        if raw is None and not (macro == 'OPENSSL' and features['features'][name] is False):
            raise RuntimeError('generated feature lacks an actual configure probe result')
        if raw not in (None, '', '0', '1') or (raw == '1') != features['features'][name]:
            raise RuntimeError('generated feature JSON differs from actual compile/link cache')
    template = (source/ROS2_FEATURE_SOURCES[2]).read_text()
    actual_macros = re.findall(r'^#cmakedefine01 (UWVM_ROS2_HAVE_[A-Z_]+)$', template, re.M)
    if (set(actual_macros) != {'UWVM_ROS2_HAVE_'+value for value in ROS2_FEATURE_MACROS.values()}
            or len(actual_macros) != len(ROS2_FEATURE_MACROS)
            or template.count('@CMAKE_CXX_STANDARD@') != 1):
        raise RuntimeError('frozen ROS2 feature template has an unaccounted directive')
    expected_text = template.replace('@CMAKE_CXX_STANDARD@', '17')
    for name, macro in ROS2_FEATURE_MACROS.items():
        expected_text = expected_text.replace('#cmakedefine01 UWVM_ROS2_HAVE_'+macro+'\n',
            '#define UWVM_ROS2_HAVE_'+macro+' '+str(int(features['features'][name]))+'\n')
    if actual_files['uwvm_ros2_features.h'].read_bytes() != expected_text.encode():
        raise RuntimeError('actual generated feature header differs from frozen configure template/JSON')


def capture_generated_features(profile, build, out, source, frozen, manifest_sha, rows):
    """Pin configure outputs BEFORE build, retaining separate exclusive copies."""
    names = generated_feature_files(profile)
    if not names: return {}
    configure = [row for row in rows if row['name'] == profile['prefix']+'-configure']
    if (len(configure) != 1 or configure[0]['status'] != 'PASS'
            or type(configure[0]['returncode']) is not int or configure[0]['returncode'] != 0):
        raise RuntimeError('successful actual configure required before generated feature capture')
    archive = out/(profile['prefix']+'-generated-feature-inputs')
    archive.mkdir(mode=0o700)
    inputs, source_inputs = {}, {}
    for name in names:
        actual = build/name
        if actual.resolve(strict=True) != actual:
            raise RuntimeError('generated feature input must be the exact physical owned build path')
        before = generated_feature_pin(actual)
        destination = archive/name
        with destination.open('xb') as stream: stream.write(actual.read_bytes())
        copy = generated_feature_pin(destination)
        if generated_feature_pin(actual) != before or any(copy[k] != before[k] for k in ('bytes','sha256')):
            raise RuntimeError('configure feature input changed while archiving')
        inputs[name] = {'actual':before, 'archive':copy}
    for relative in ROS2_FEATURE_SOURCES:
        pin = generated_feature_pin(source/relative)
        if frozen.get(relative, {}).get('sha256') != pin['sha256'] or frozen[relative]['bytes'] != pin['bytes']:
            raise RuntimeError('generated feature source is outside current manifest')
        source_inputs[relative] = pin
    validate_feature_contents(source, {name:Path(value['actual']['path']) for name,value in inputs.items()})
    logical = profile['build_guest']+'/uwvm_ros2_features.h'
    return {logical:{'format_version':1, 'kind':'CMake-configured-ROS2-feature-header',
        'capture_phase':'after-successful-configure-before-build', 'guest_path':logical,
        'source':str(source), 'source_manifest_sha256':manifest_sha, 'inputs':inputs,
        'source_inputs':source_inputs,
        'configure':{'name':configure[0]['name'], 'command':configure[0]['command'],
                     'log':generated_feature_pin(out/(profile['prefix']+'-configure.log'), limit=64*1024*1024)}}}


def validate_generated_features(profile, records, out, source, frozen, manifest_sha, rows, pins):
    """Reopen retained original/copy pins; no producer process must remain live."""
    logical = profile['build_guest']+'/uwvm_ros2_features.h'
    if profile['version'] == 1:
        if records != {}: raise RuntimeError('ROS1 strict profile declares no generated feature header')
        return {}
    if type(records) is not dict or set(records) != {logical}:
        raise RuntimeError('exact one ROS2 generated feature header record required')
    record = records[logical]
    if (type(record) is not dict or set(record) != {'format_version','kind','capture_phase','guest_path',
            'source','source_manifest_sha256','inputs','source_inputs','configure'}
            or type(record['format_version']) is not int or record['format_version'] != 1
            or record['kind'] != 'CMake-configured-ROS2-feature-header'
            or record['capture_phase'] != 'after-successful-configure-before-build'
            or record['guest_path'] != logical or record['source'] != str(source)
            or record['source_manifest_sha256'] != manifest_sha
            or type(record['inputs']) is not dict or set(record['inputs']) != set(generated_feature_files(profile))
            or type(record['source_inputs']) is not dict or set(record['source_inputs']) != set(ROS2_FEATURE_SOURCES)):
        raise RuntimeError('generated feature source/configure provenance differs')
    actual_files = {}
    for name, value in record['inputs'].items():
        if type(value) is not dict or set(value) != {'actual','archive'}:
            raise RuntimeError('closed generated feature actual/archive pins required')
        actual = out/'owned-tmp'/Path(profile['build_guest']).name/name
        archive = out/(profile['prefix']+'-generated-feature-inputs')/name
        for label,path in (('actual',actual),('archive',archive)):
            if path.resolve(strict=True) != path:
                raise RuntimeError('generated feature physical path escaped its exact owned directory')
            pin = generated_feature_pin(path)
            if pin != value[label]: raise RuntimeError('generated feature opened pin changed')
            pins.append(pin)
        if any(value['actual'][field] != value['archive'][field] for field in ('bytes','sha256')):
            raise RuntimeError('generated feature archive differs from actual compiler input')
        actual_files[name] = actual
    for relative,expected in record['source_inputs'].items():
        pin = generated_feature_pin(source/relative)
        if pin != expected or pin['sha256'] != frozen.get(relative, {}).get('sha256') or pin['bytes'] != frozen[relative]['bytes']:
            raise RuntimeError('generated feature generator/template source changed')
        pins.append(pin)
    configure = [row for row in rows if row['name'] == profile['prefix']+'-configure']
    fact = record['configure']
    log = generated_feature_pin(out/(profile['prefix']+'-configure.log'), limit=64*1024*1024)
    if (type(fact) is not dict or set(fact) != {'name','command','log'} or len(configure) != 1
            or configure[0]['status'] != 'PASS' or type(configure[0]['returncode']) is not int
            or configure[0]['returncode'] != 0 or fact != {'name':configure[0]['name'],
                'command':configure[0]['command'],'log':log}):
        raise RuntimeError('generated feature lacks the exact successful configure command/log')
    pins.append(log)
    validate_feature_contents(source, actual_files)
    return {logical:record['inputs']['uwvm_ros2_features.h']['actual']}


def rooted(root, path):
    """Resolve SDK symlinks with absolute links rooted inside that SDK."""
    parts = list(Path(posixpath.normpath('/'+str(path).lstrip('/'))).parts[1:])
    selected = []
    followed = 0
    while parts:
        part = parts.pop(0)
        if part == '..':
            if selected:
                selected.pop()
            continue
        if part in ('', '.'):
            continue
        candidate = root.joinpath(*selected, part)
        if candidate.is_symlink():
            followed += 1
            if followed > 40:
                raise RuntimeError('SDK symlink loop: '+str(path))
            link = os.readlink(candidate)
            if link.startswith('/'):
                selected = []
            parts = list(Path(link).parts[1:] if link.startswith('/') else Path(link).parts)+parts
        else:
            selected.append(part)
    return root.joinpath(*selected)


def sdk_inventory(root):
    records = {}
    for prefix in ('usr', 'opt', 'etc', 'var/lib/dpkg'):
        for path in (root/prefix).rglob('*'):
            name = str(path.relative_to(root))
            if path.is_symlink():
                records[name] = {'symlink': os.readlink(path)}
            elif path.is_file():
                records[name] = {'sha256': digest(path), 'bytes': path.stat().st_size}
    return records


def sdk_root_aliases(root):
    records = {}
    for name in ('bin', 'lib', 'sbin', 'lib64'):
        path = root/name
        if path.is_symlink():
            records[name] = {'symlink': os.readlink(path)}
        elif path.exists():
            records[name] = {'directory': path.is_dir(), 'mode': path.stat().st_mode}
        else:
            records[name] = None
    return records


def source_state(source, frozen):
    actual = {str(path.relative_to(source)): path for path in source.rglob('*') if path.is_file()}
    if set(actual) != set(frozen):
        raise RuntimeError('frozen source inventory differs from its exact manifest')
    values = {}
    for name, row in frozen.items():
        path = actual[name]
        if not path.resolve().is_relative_to(source):
            raise RuntimeError('frozen source input escapes source: '+name)
        values[name] = digest(path)
        if values[name] != row['sha256'] or path.stat().st_size != row['bytes']:
            raise RuntimeError('frozen source bytes differ: '+name)
    return values


def custom_file_pin(path):
    """Pin one bounded regular input from one nonblocking opened descriptor."""
    selected = Path(path).resolve(strict=True)
    descriptor = os.open(selected, os.O_RDONLY | os.O_CLOEXEC | os.O_NONBLOCK | os.O_NOFOLLOW)
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode) or not 0 <= before.st_size <= 64 * 1024 * 1024:
            raise RuntimeError('custom input must be a bounded regular file')
        hasher = hashlib.sha256()
        remaining = before.st_size + 1
        length = 0
        while remaining:
            chunk = os.read(descriptor, min(1024 * 1024, remaining))
            if not chunk:
                break
            hasher.update(chunk); length += len(chunk); remaining -= len(chunk)
        after = os.fstat(descriptor)
        identity = lambda item: (item.st_dev, item.st_ino, item.st_size, item.st_mtime_ns, item.st_ctime_ns)
        if identity(before) != identity(after) or identity(after) != identity(selected.stat()) or length != after.st_size:
            raise RuntimeError('custom input changed while being pinned')
        return {'path': str(selected), 'device': after.st_dev, 'inode': after.st_ino,
                'bytes': length, 'mtime_ns': after.st_mtime_ns, 'ctime_ns': after.st_ctime_ns,
                'sha256': hasher.hexdigest()}
    finally:
        os.close(descriptor)


def custom_packages(values):
    """Parse an explicit comma/repeated package list without accepting flags."""
    packages = []
    for value in values:
        for name in value.split(','):
            if not re.fullmatch(r'[a-z][a-z0-9_]{0,62}', name) or name in packages:
                raise RuntimeError('invalid or duplicate explicit custom package')
            packages.append(name)
    if not packages or len(packages) > 128:
        raise RuntimeError('custom selection requires 1..128 explicit packages')
    return packages


def custom_header(value, source, sdk, frozen):
    """Map a header only to frozen /work or immutable SDK /opt namespace."""
    raw = str(value)
    if not raw or any(character in raw for character in ('"', ';', '\\', '\n', '\r', '\0')):
        raise RuntimeError('unsafe custom header include path')
    logical = posixpath.normpath(raw)
    path = Path(raw)
    if logical.startswith('/work/'):
        relative = logical[len('/work/'):]
        actual = (source / relative).resolve(strict=True)
        scope = 'frozen_source'
        guest = logical
    elif logical.startswith('/opt/'):
        actual = rooted(sdk, logical).resolve(strict=True)
        scope = 'immutable_sdk'
        guest = logical
    elif path.is_absolute() and path.is_relative_to(source):
        relative = str(path.relative_to(source))
        actual = path.resolve(strict=True)
        scope = 'frozen_source'
        guest = '/work/' + relative
    elif path.is_absolute() and path.is_relative_to(sdk / 'opt'):
        guest = '/' + str(path.relative_to(sdk))
        actual = rooted(sdk, guest).resolve(strict=True)
        scope = 'immutable_sdk'
    else:
        raise RuntimeError('custom header must be frozen /work or immutable SDK /opt')
    pin = custom_file_pin(actual)
    if scope == 'frozen_source':
        if not actual.is_relative_to(source):
            raise RuntimeError('custom source header escapes frozen source')
        row = frozen.get(relative, {})
        if row.get('sha256') != pin['sha256'] or row.get('bytes') != pin['bytes']:
            raise RuntimeError('custom source header is not an exact frozen manifest input')
        return {'guest_path': guest, 'scope': scope, 'source_relative': relative, 'input': pin}
    if not actual.is_relative_to((sdk / 'opt').resolve(strict=True)):
        raise RuntimeError('custom SDK header escapes immutable /opt')
    return {'guest_path': guest, 'scope': scope, 'sdk_relative': str(actual.relative_to(sdk)), 'input': pin}


def custom_selection(version, registry, native, packages, prefix, source, sdk, frozen):
    """Prepare closed paired custom CMake inputs; create no process or grant."""
    supplied = (bool(registry), bool(native), bool(packages), bool(prefix))
    if not any(supplied):
        return {'enabled': False, 'version': version, 'cmake_arguments': [], 'input_pins': []}
    if not all(supplied) or version not in (1, 2):
        raise RuntimeError('custom topics require paired headers, packages and prefix')
    prefix_text = posixpath.normpath(str(prefix))
    if prefix_text != '/opt/uwvm-custom-types':
        raise RuntimeError('custom package prefix must be /opt/uwvm-custom-types')
    resolved_prefix = rooted(sdk, prefix_text).resolve(strict=True)
    if not resolved_prefix.is_dir() or not resolved_prefix.is_relative_to((sdk / 'opt').resolve(strict=True)):
        raise RuntimeError('custom package prefix escapes immutable SDK /opt')
    names = custom_packages(packages)
    common = custom_header(registry, source, sdk, frozen)
    provider = custom_header(native, source, sdk, frozen)
    configs = []
    for package in names:
        logical = prefix_text + '/share/' + package + '/cmake/' + package + 'Config.cmake'
        actual = rooted(sdk, logical).resolve(strict=True)
        if not actual.is_relative_to(resolved_prefix):
            raise RuntimeError('custom package config escapes explicit prefix')
        configs.append({'package': package, 'guest_path': logical,
                        'sdk_relative': str(actual.relative_to(sdk)), 'input': custom_file_pin(actual)})
    distro = 'noetic' if version == 1 else 'jazzy'
    arguments = ['-DUWVM_ROS_CUSTOM_REGISTRY_HEADER=' + common['guest_path'],
                 '-DUWVM_ROS_CUSTOM_ROS' + str(version) + '_HEADER=' + provider['guest_path'],
                 '-DUWVM_ROS_CUSTOM_ROS' + str(version) + '_PACKAGES=' + ';'.join(names),
                 '-DCMAKE_PREFIX_PATH=' + prefix_text + ';/opt/ros/' + distro]
    # Exact package DIR arguments prevent another installed prefix from winning
    # CMake discovery while the report claims these selected package configs.
    arguments += ['-D' + package + '_DIR=' + prefix_text + '/share/' + package + '/cmake' for package in names]
    pins = [common['input'], provider['input'], *[item['input'] for item in configs]]
    return {'enabled': True, 'version': version, 'registry_header': common, 'native_header': provider,
            'prefix': {'guest_path': prefix_text, 'sdk_relative': str(resolved_prefix.relative_to(sdk))},
            'packages': names, 'package_configs': configs, 'cmake_arguments': arguments,
            'input_pins': pins, 'scope': 'Build support only; no implicit ROS permission or entity'}


def custom_inputs_preserved(selection):
    """Refuse replacement, mutation or deletion of every explicitly pinned input."""
    return all(custom_file_pin(pin['path']) == pin for pin in selection['input_pins'])


def load_owned_commands(source, manifest, frozen):
    """Load the one exact current-MF controller body, without ambient lookup."""
    path = source/'tests/ros_aarch64/binfmt_lease.py'
    first = custom_file_pin(path)
    if {'bytes': first['bytes'], 'sha256': first['sha256']} != frozen['tests/ros_aarch64/binfmt_lease.py']:
        raise RuntimeError('owned command controller is outside the complete current manifest')
    # The small source file is bracketed by full physical pins. Its loaded bytes
    # are hashed independently before compile(), not reopened by a spec loader.
    body = path.read_bytes()
    if len(body) != first['bytes'] or hashlib.sha256(body).hexdigest() != first['sha256'] or custom_file_pin(path) != first:
        raise RuntimeError('owned command source changed while loading')
    spec = importlib.util.spec_from_file_location('_affected_current_owned_commands', path)
    module = importlib.util.module_from_spec(spec)
    exec(compile(body, str(path), 'exec'), module.__dict__)
    return module


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'sdk-workspace', 'source', 'out', 'qemu', 'binfmt-evidence'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--manifest', type=Path)
    parser.add_argument('--container-started-at', required=True)
    parser.add_argument('--timeout', type=int, default=1200)
    parser.add_argument('--rss-limit-bytes', type=int, default=2*1024**3,
                        help='explicit total primary command tree RSS; Root separately reserves it')
    parser.add_argument('--version', type=int, choices=(1, 2), default=2)
    parser.add_argument('--custom-registry-header', type=Path)
    parser.add_argument('--custom-native-header', type=Path)
    parser.add_argument('--custom-packages', action='append', default=[], help='Explicit comma-separated package list; repeatable')
    parser.add_argument('--custom-prefix', type=Path)
    args = parser.parse_args()
    if not 60 <= args.timeout <= 3600 or not 64*1024**2 <= args.rss_limit_bytes <= 10*1024**3:
        parser.error('--timeout60..3600 and explicit RSS64MiB..10GiB required')
    source = args.source.resolve(strict=True)
    profile = build_profile(args.version)
    prefix, target = profile['prefix'], profile['target']
    sdk_workspace = args.sdk_workspace.resolve(strict=True)
    sdk = sdk_workspace/'gateway'/(profile['distro']+'-rootfs')
    out = args.out.resolve()
    out.mkdir(parents=True, mode=0o700, exist_ok=False)
    root = out/'rootfs'
    temporary = out/'owned-tmp'
    build = temporary/Path(profile['build_guest']).name
    manifest = (args.manifest or source.parent/'source-manifest.json').resolve(strict=True)
    report_file = out/'build-report.json'
    start = time.monotonic()
    mounts, rows, active, evidence = [], [], None, None
    active_row, owned_commands, owned_common, owned_admission, owned_boundary = None, None, None, None, None
    cancellation = {'finalizing': False, 'signals': [], 'committed': False}
    before, frozen, initial, sdk_before, aliases_before = None, None, None, None, None
    generated_features = None
    status = 'FAIL'
    report = {'status': 'RUNNING', 'scope': 'Fresh targeted compilation only; no reused artifact verdicts',
              'source': str(source), 'source_manifest': str(manifest),
              'source_manifest_sha256': digest(manifest), 'sdk_workspace': str(sdk_workspace),
              'parallelism': 1, 'version': args.version, 'profile': profile,
              'target': target, 'targets': {}, 'builds': rows,
              'compiled_source_dependencies': {}, 'build_metadata': {}, 'mounts': [],
              'generated_feature_dependencies': {}, 'generated_feature_inputs_preserved': False}

    def save():
        report['seconds'] = round(time.monotonic()-start, 3)
        report_file.write_text(json.dumps(report, indent=2)+'\n')

    def checked(command):
        subprocess.run([str(item) for item in command], check=True, timeout=30,
                       stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    def bind(origin, destination, readonly=False, recursive=False):
        destination.mkdir(parents=True, exist_ok=True)
        checked(['mount', '--rbind' if recursive else '--bind', origin, destination])
        mounts.append(destination)
        checked(['mount', '--make-rprivate', destination])
        if readonly:
            checked(['mount', '-o', 'remount,bind,ro', destination])
        report['mounts'].append({'source': str(origin), 'destination': str(destination),
                                 'readonly': readonly, 'recursive': recursive})

    def incarnation():
        stat = evidence.leaf.stat()
        return {'cgroup_path': str(evidence.leaf), 'device': stat.st_dev, 'inode': stat.st_ino,
                'container_started_at': args.container_started_at,
                'membership': Path('/proc/self/cgroup').read_text().splitlines()}

    environment = dict(os.environ)
    for name in ('CC', 'CXX', 'LD_LIBRARY_PATH', 'LD_PRELOAD', 'QEMU_LD_PREFIX',
                 'CFLAGS', 'CXXFLAGS', 'LDFLAGS'):
        environment.pop(name, None)
    environment.update(PATH='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin',
                       PYTHONDONTWRITEBYTECODE='1')

    def native(arguments):
        return ['/usr/bin/chroot', str(root), '/bin/bash', '-lc',
                'source /opt/ros/'+profile['distro']+'/setup.bash && exec "$@"',
                'uwvm-affected-build', *arguments]

    def stop_active():
        nonlocal active, active_row
        if active is None:
            return
        result = active.close()
        if active_row is not None:
            active_row['owned_command_cleanup'] = result
        if result['status'] != 'PASS':
            raise RuntimeError('original-pidfd command cleanup incomplete')
        active, active_row = None, None

    def execute(name, arguments, required):
        nonlocal active, active_row
        remaining = args.timeout-(time.monotonic()-start)
        if remaining <= 0:
            raise RuntimeError('targeted rebuild total deadline')
        command = native(arguments)
        row = {'name': name, 'command': command, 'status': 'RUNNING', 'timeout_seconds': remaining}
        rows.append(row)
        save()
        begun = time.monotonic()
        try:
            active = owned_commands.OwnedCommand(owned_common, out, max(1, remaining),
                args.rss_limit_bytes, owned_boundary.check,
                cleanup_guard=lambda: owned_boundary.check(finalizing=True))
            active_row = row
            with (out/(name+'.log')).open('wb') as log:
                process = active.start(command, stdout=log, stderr=subprocess.STDOUT,
                                       env=environment, start_new_session=True)
            row['owned_command_launch'] = active.launch
            row['pid'] = process.pid
            row['identity'] = active.runner.roots[process.pid]
            row['membership'] = Path(f'/proc/{process.pid}/cgroup').read_text().splitlines()
            evidence.start(process, name+'/', command)
            row['returncode'] = active.wait()
            if row['returncode']:
                raise RuntimeError('native targeted rebuild command failed: '+name)
            evidence.check(required, name+'/')
            observed = {label for sample in evidence.samples.values()
                        if sample['name'] == name+'/' and sample['ancestry'][0] == process.pid
                        for label in sample['mapped_arm_targets']}
            if set(required)-observed:
                raise RuntimeError('missing actual ARM compiler/frontend in this launch PID subtree')
            row['required_arm_targets'] = list(required)
            row['status'] = 'PASS'
        except BaseException:
            row['status'] = 'FAIL'
            raise
        finally:
            # A cancellation during cleanup is recorded but cannot skip the
            # original-pidfd kill/reap barrier or later mount cleanup.
            previous = cancellation['finalizing']
            cancellation['finalizing'] = True
            try:
                stop_active()
            except BaseException:
                row['status'] = 'FAIL'
                raise
            finally:
                cancellation['finalizing'] = previous
                row['seconds'] = round(time.monotonic()-begun, 3)
                save()
            if cancellation['signals']:
                raise InterruptedError('targeted rebuild cancelled during owned command cleanup')
        print('PASS '+name, flush=True)

    def interrupted(signum, frame):
        if cancellation['committed']:
            return
        cancellation['signals'].append(signum)
        if not cancellation['finalizing']:
            raise InterruptedError('targeted rebuild interrupted by signal '+str(signum))

    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)
    try:
        if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'):
            raise RuntimeError('launch targeted rebuild with unshare -m')
        if not (sdk/'.uwvm-task-owned').is_file():
            raise RuntimeError('task-owned ARM '+profile['distro']+' SDK required')
        frozen = json.loads(manifest.read_text())
        initial = source_state(source, frozen)
        owned_commands = load_owned_commands(source, manifest, frozen)
        owned_common, owned_admission = owned_commands.load_pidfd_support(source, manifest)
        owned_boundary = owned_commands.CommandBoundary(owned_common, out, args.timeout)
        owned_boundary.started = start  # Do not restart the existing whole active clock.
        owned_boundary.check()
        report['owned_command_source'] = owned_admission
        report['owned_tree_RSS_limit_bytes'] = args.rss_limit_bytes
        report['owned_command_cgroup_before'] = owned_boundary.before
        report['custom_selection'] = custom_selection(args.version, args.custom_registry_header,
            args.custom_native_header, args.custom_packages, args.custom_prefix, source, sdk, frozen)
        spec = importlib.util.spec_from_file_location('affected_frozen_cgroup', source/'tests/ros_cgroup/probe.py')
        probe = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(probe)
        evidence = Evidence(out/'execution', probe.probe, interval=.002)
        before = incarnation()
        report['cgroup_incarnation_before'] = before
        report['actualexecution_cgroup'] = {'before': evidence.before, 'membership': before['membership']}
        report['emulator'] = evidence.emulator(args.qemu.resolve(strict=True))
        lease = json.loads(args.binfmt_evidence.read_text())
        entry = (Path('/proc/sys/fs/binfmt_misc')/lease['name']).read_text()
        if (entry != lease['entry'] or os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY') != lease['name']
                or lease['qemu_sha256'] != digest(args.qemu)
                or 'interpreter '+str(args.qemu.resolve()) not in entry or 'flags: F' not in entry):
            raise RuntimeError('verified active temporary QEMU lease required')
        report['binfmt'] = {'path': str(args.binfmt_evidence), 'sha256': digest(args.binfmt_evidence),
                            'name': lease['name'], 'entry': entry}
        archived = out/'driver-inputs'
        archived.mkdir()
        report['driver_inputs'] = {}
        for name in ('rebuild_affected.py', 'runtime.py', 'run_resources.py'):
            original = Path(__file__).with_name(name)
            copy = archived/name
            shutil.copy2(original, copy)
            report['driver_inputs'][name] = {'archive': str(copy), 'sha256': digest(copy)}
        sdk_before = sdk_inventory(sdk)
        aliases_before = sdk_root_aliases(sdk)
        report['sdk_root_aliases_before'] = aliases_before
        inventory_file = out/'sdk-static-inputs-before.json'
        inventory_file.write_text(json.dumps(sdk_before, indent=2)+'\n')
        report['sdk_static_inputs'] = {'path': str(inventory_file), 'sha256': digest(inventory_file),
            'files': len(sdk_before), 'scope': ['usr', 'opt', 'etc', 'var/lib/dpkg']}
        checked(['mount', '--make-rprivate', '/'])
        bind(sdk, root, recursive=True)
        report['owned_source_mount'] = bind_frozen_work(root, source, sdk, out, checked)
        mounts.append(root/'work')
        temporary.mkdir()
        bind(temporary, root/'tmp')
        home = out/'owned-home'
        home.mkdir()
        bind(home, root/'root')
        for name in ('usr', 'opt', 'etc', 'var/lib/dpkg'):
            bind(sdk/name, root/name, readonly=True)
        for name in ('proc', 'sys', 'dev'):
            bind(Path('/')/name, root/name, recursive=True)
        compiler = rooted(sdk, profile['compiler'])
        frontend_inside = '/gcc/aarch64-linux-gnu/'+str(profile['gcc'])+'/cc1plus'
        frontend = next((rooted(sdk, '/'+directory+frontend_inside)
                         for directory in ('usr/lib', 'usr/libexec')
                         if rooted(sdk, '/'+directory+frontend_inside).is_file()), None)
        if frontend is None:
            raise RuntimeError('ARM GCC'+str(profile['gcc'])+' frontend unavailable')
        evidence.target(prefix+'-compiler', compiler)
        evidence.target(prefix+'-frontend', frontend)
        evidence.target(prefix+'-cmake', rooted(sdk, '/usr/bin/cmake'))
        # The actual compilation below proves the GCC driver/frontend mapping;
        # a very short --version process need not survive a sampling interval.
        execute(prefix+'-compiler-version', [profile['compiler'], '--version'], [])
        execute(prefix+'-package-inventory', ['/usr/bin/dpkg-query', '-W', '-f=${Package}\t${Version}\t${Architecture}\n'], [])
        report['package_inventory'] = {'path': str(out/(prefix+'-package-inventory.log')),
                                      'sha256': digest(out/(prefix+'-package-inventory.log'))}
        report['compiler_dependency_output_options'] = profile['dependency_flags']
        execute(prefix+'-configure', ['cmake', '-S', '/work/gateways/'+prefix, '-B', profile['build_guest'],
            '-DCMAKE_CXX_COMPILER='+profile['compiler'],
            '-DCMAKE_BUILD_TYPE=RelWithDebInfo', '-DCMAKE_CXX_FLAGS='+' '.join(profile['dependency_flags']),
            '-DCMAKE_CXX_FLAGS_RELWITHDEBINFO=-O2 -g0 -DNDEBUG', '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON',
            *report['custom_selection']['cmake_arguments']], [prefix+'-cmake'])
        # CMake generates this feature header before the actual compiler runs.
        # Pin both original and archive now, not from a post-build reconstruction.
        generated_features = capture_generated_features(profile, build, out, source, frozen,
            report['source_manifest_sha256'], rows)
        report['generated_feature_dependencies'] = generated_features
        save()
        execute(prefix+'-build-gateway', ['cmake', '--build', profile['build_guest'],
            '--target', target, '--parallel', '1', '--verbose'], [prefix+'-compiler', prefix+'-frontend'])
        report['targets'][target] = require_arm(build/target)
        if [path.name for path in build.glob('uwvm-ros*-*') if path.is_file()] != [target]:
            raise RuntimeError('targeted build produced unexpected additional native targets')
        metadata = build/'CMakeFiles'/(target+'.dir')
        flags = (metadata/'flags.make').read_text()
        values = next(line.split('=', 1)[1] for line in flags.splitlines() if line.startswith('CXX_FLAGS ='))
        tokens = shlex.split(values)
        if not set(profile['dependency_flags']+['-O2', '-g0', '-DNDEBUG']).issubset(tokens) or not any(
                token in ('-std=c++17', '-std=gnu++17') for token in tokens):
            raise RuntimeError('original C++17/O2/g0/NDEBUG or declared dependency flags missing')
        compiler_log = out/(prefix+'-build-gateway.log')
        command = original_compile_command(profile,
            json.loads((build/'compile_commands.json').read_text()), compiler_log.read_text())
        output = dependency_output(profile, command)
        dep = temporary/output['owned_tmp_relative']
        if not dep.is_file() or dep.is_symlink() or not dep.resolve().is_relative_to(temporary):
            raise RuntimeError('actual effective GCC -MF dependency output missing or escapes owned tmp')
        report['actual_compiler_command'] = {**command,
            'log': str(compiler_log), 'log_sha256': digest(compiler_log),
            'scope': 'Successful verbose GCC compilation recipe, checked against compile database codegen argv; '
                     'dependency options are taken from the verbose recipe, not assumed from CXX_FLAGS'}
        archive = out/(prefix+'-metadata')
        inputs = [build/'CMakeCache.txt', build/'compile_commands.json', metadata/'flags.make',
                  metadata/'link.txt', metadata/'build.make', *metadata.glob('*.o.d')]
        for name in ('depend.make', 'compiler_depend.make'):
            if (metadata/name).is_file():
                inputs.append(metadata/name)
        for path in inputs:
            relative = str(path.relative_to(build))
            destination = archive/relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(path, destination)
            report['build_metadata'][prefix+':'+relative] = {'path': str(destination), 'sha256': digest(destination)}
        destination = archive/output['archive_relative']
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(dep, destination)
        dependency_key = prefix+':'+output['archive_relative']
        report['build_metadata'][dependency_key] = {'path': str(destination), 'sha256': digest(destination)}
        report['actual_compiler_depfile_path'] = output['actual_guest_output']
        report['actual_compiler_depfile_host_path'] = str(dep)
        report['compiler_dependency_records'] = {dependency_key: {**output,
            'actual_host_output': str(dep), 'sha256': digest(dep),
            'method': 'Actual GCC'+str(profile['gcc'])+' compilation -MD/-MF output selected from '
                      'the successful verbose compiler argv; not CMake scanner output and not preprocessing replay',
            'dependency_output_options': command['options']['dependency_output_options'],
            'mf_outputs_in_argv_order': command['options']['mf_outputs_in_argv_order'],
            'actual_compiler_command_log': str(compiler_log),
            'actual_compiler_command_log_sha256': digest(compiler_log)}}
        target_text, body = dep.read_text().replace('\\\n', ' ').split(':', 1)
        object_guest = posixpath.normpath(posixpath.join(command['directory'], command['options']['object_output']))
        rule_targets = [posixpath.normpath(posixpath.join(command['directory'], token))
                        for token in shlex.split(target_text)]
        if rule_targets != [object_guest]:
            raise RuntimeError('actual -MF depfile target differs from the compiled Gateway object')
        report['compiler_dependency_records'][dependency_key]['compiled_object_guest_path'] = object_guest
        generated = validate_generated_features(profile, generated_features, out, source, frozen,
            report['source_manifest_sha256'], rows, [])
        project, headers, seen_generated = {}, {}, set()
        for token in shlex.split(body):
            inside = posixpath.normpath(token)
            if inside.startswith('/work/'):
                relative = inside[len('/work/'):]
                if relative not in initial or digest(source/relative) != initial[relative]:
                    raise RuntimeError('compiler input outside frozen project manifest: '+relative)
                project[relative] = initial[relative]
            elif inside in generated:
                # No other /tmp/build/include product is admitted by this branch.
                seen_generated.add(inside)
            elif inside.startswith(('/usr/', '/opt/', '/etc/', '/lib/', '/lib64/', '/bin/', '/sbin/')):
                actual = rooted(sdk, inside)
                relative = str(actual.relative_to(sdk))
                before_header = sdk_before.get(relative)
                value = digest(actual)
                if not before_header or before_header.get('sha256') != value:
                    raise RuntimeError('SDK compiler header differs from before-build inventory: '+inside)
                headers[inside] = {'sha256': value, 'resolved_sdk_path': relative, 'bytes': actual.stat().st_size}
            else:
                raise RuntimeError('unaccounted actual compiler dependency: '+inside)
        if seen_generated != set(generated):
            raise RuntimeError('configured feature header missing from actual compiler dependency closure')
        report['generated_feature_inputs_preserved'] = True
        required_project = {'gateways/'+prefix+'/main.cc'}
        if args.version == 2:
            required_project.add('gateways/ros2/action.h')
        if not required_project.issubset(project):
            raise RuntimeError('incomplete Gateway actual compiler project dependency closure')
        report['compiled_source_dependencies'][dependency_key] = project
        report['sdk_header_dependencies'] = headers
        report['sdk_header_dependency_scope'] = (
            'Headers present in the actual system-inclusive GCC -MD/-MF compilation output '
            'at its effective final -MF path; not CMake dependency scanner output or replay')
        standard_headers = [name for name in headers if name.startswith('/usr/include/c++/'+str(profile['gcc'])+'/')]
        ros_headers = [name for name in headers if name.startswith('/opt/ros/'+profile['distro']+'/include/')]
        report['system_headers_recorded_in_original_depfile'] = bool(standard_headers and ros_headers)
        report['system_header_dependency_categories'] = {'standard_cplusplus': len(standard_headers),
                                                         'ros_sdk': len(ros_headers), 'total': len(headers)}
        if not standard_headers or not ros_headers:
            raise RuntimeError('actual GCC -MD/-MF standard C++ or ROS SDK header closure empty')
        linked = {}
        for token in shlex.split((metadata/'link.txt').read_text()):
            if token.startswith(('/usr/', '/opt/')):
                actual = rooted(sdk, token)
                relative = str(actual.relative_to(sdk))
                if actual.is_file():
                    value = digest(actual)
                    if sdk_before.get(relative, {}).get('sha256') != value:
                        raise RuntimeError('SDK link input differs from frozen SDK: '+token)
                    linked[token] = {'sha256': value, 'resolved_sdk_path': relative}
        report['sdk_link_inputs'] = linked
        report['source_preserved'] = source_state(source, frozen) == initial and digest(manifest) == report['source_manifest_sha256']
        report['sdk_static_preserved'] = sdk_inventory(sdk) == sdk_before
        report['sdk_root_aliases_preserved'] = sdk_root_aliases(sdk) == aliases_before
        report['custom_inputs_preserved'] = custom_inputs_preserved(report['custom_selection'])
        if not report['source_preserved'] or not report['sdk_static_preserved'] or not report['sdk_root_aliases_preserved'] or not report['custom_inputs_preserved']:
            raise RuntimeError('frozen source or SDK input inventory changed')
        for name in ('repository-signatures.json', 'supplemental-packages.json'):
            path = sdk_workspace/'provenance'/name
            report[name.replace('.json', '').replace('-', '_')] = {'path': str(path), 'sha256': digest(path)}
        status = 'PASS'
    except BaseException:
        report['error'] = traceback.format_exc()
    finally:
        cancellation['finalizing'] = True
        try:
            stop_active()
        except BaseException:
            report['owned_command_cleanup_error'] = traceback.format_exc()
            status = 'FAIL'
        if generated_features is not None:
            report['generated_feature_inputs_preserved'] = False
        try:
            if 'custom_selection' in report:
                report['custom_inputs_preserved'] = custom_inputs_preserved(report['custom_selection'])
                if not report['custom_inputs_preserved']:
                    status = 'FAIL'
            if initial is not None:
                report['source_preserved'] = source_state(source, frozen) == initial and digest(manifest) == report['source_manifest_sha256']
                if not report['source_preserved']:
                    status = 'FAIL'
            if sdk_before is not None:
                report['sdk_static_preserved'] = sdk_inventory(sdk) == sdk_before
                if not report['sdk_static_preserved']:
                    status = 'FAIL'
            if aliases_before is not None:
                report['sdk_root_aliases_preserved'] = sdk_root_aliases(sdk) == aliases_before
                if not report['sdk_root_aliases_preserved']:
                    status = 'FAIL'
            if generated_features is not None:
                validate_generated_features(profile, generated_features, out, source, frozen,
                    report['source_manifest_sha256'], rows, [])
                report['generated_feature_inputs_preserved'] = True
        except BaseException:
            report['preservation_error'] = traceback.format_exc()
            status = 'FAIL'
        try:
            if evidence is not None:
                result = evidence.finish(status)
                status = result['status']
                report['execution_evidence'] = str(out/'execution/architecture-evidence.json')
                report['actualexecution_cgroup']['after'] = result['cgroup_after']
                report['ancestor_memory_event_deltas'] = result['cgroup_ancestor_event_deltas']
                report['cgroup_incarnation_after'] = incarnation()
                report['same_cgroup_incarnation'] = before is not None and all(
                    before[key] == report['cgroup_incarnation_after'][key] for key in ('device', 'inode', 'membership'))
                if not report['same_cgroup_incarnation']:
                    status = 'FAIL'
        except BaseException:
            report['execution_evidence_cleanup_error'] = traceback.format_exc()
            status = 'FAIL'
        cleanup = []
        for path in reversed(mounts):
            try:
                result = subprocess.run(['umount', '-R', str(path)], capture_output=True, text=True, timeout=30)
                cleanup.append({'mount': str(path), 'returncode': result.returncode, 'stderr': result.stderr})
                if result.returncode:
                    status = 'FAIL'
            except BaseException:
                cleanup.append({'mount': str(path), 'status': 'FAIL', 'error': traceback.format_exc()})
                status = 'FAIL'
        report['mount_cleanup'] = cleanup
        try:
            report['post_cleanup_mountinfo'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
                if line.split()[4] == str(root) or line.split()[4].startswith(str(root)+'/')]
            if report['post_cleanup_mountinfo']:
                status = 'FAIL'
            if owned_commands is not None:
                report['owned_command_source_preserved'] = owned_commands.recheck_pidfd_support(owned_common, owned_admission)
                report['owned_command_cgroup_after'] = owned_boundary.check(finalizing=True)
                if int((Path(owned_boundary.arguments['expected_leaf'])/'memory.current').read_text()) >= 60*1024**3:
                    raise RuntimeError('terminal current64GiB headroom exhausted after cleanup')
        except BaseException:
            report['owned_command_terminal_error'] = traceback.format_exc()
            status = 'FAIL'
        report['cancellation_signals'] = cancellation['signals']
        if cancellation['signals']:
            status = 'FAIL'
        blocked = signal.pthread_sigmask(signal.SIG_BLOCK, {signal.SIGTERM, signal.SIGINT})
        try:
            if signal.sigpending() & {signal.SIGTERM, signal.SIGINT}:
                status = 'FAIL'
                report['cancellation_pending_at_publication'] = True
            report['status'] = status
            save()
            if signal.sigpending() & {signal.SIGTERM, signal.SIGINT}:
                report['status'] = 'FAIL'
                report['cancellation_pending_at_publication'] = True
                save()
            # No actor or cleanup work remains. Publication is the terminal
            # boundary; later TERM/INT cannot mutate an already saved verdict.
            cancellation['committed'] = True
        finally:
            signal.pthread_sigmask(signal.SIG_SETMASK, blocked)
    print(report['status'], report_file, flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
