#!/usr/bin/env python3
"""Serial measured C++ source/link/Guest candidates; execute only in the finite owner.

No native model test is credited as Guest, ROS, Gateway, upstream, QEMU or
performance evidence. Every requested compiler mode is probed independently.
"""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tarfile
import time

p = argparse.ArgumentParser()
p.add_argument('--archive', type=Path, required=True)
p.add_argument('--output', type=Path, required=True)
p.add_argument(
    '--test-source-root',
    type=Path,
    help='Explicit canonical C++ test-source root; sibling c/ supplies shared fixtures',
)
p.add_argument(
    '--historical-layout',
    action='store_true',
    help='Explicitly reproduce a frozen archive predating test relocation',
)
p.add_argument('--vendors', action='store_true')
p.add_argument('--vendor-standards', default='98,3,11,14,17,20,23,26')
p.add_argument('--wasm', action='store_true')
p.add_argument('--integrations', action='store_true')
p.add_argument('--generators', action='store_true')
a = p.parse_args()
assert os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') == '1'
run = Path(os.environ['UWVM_RESOURCE_RUN'])
scope = Path(os.environ['UWVM_RESOURCE_SCOPE'])
assert ('0::' + str(scope).split('/sys/fs/cgroup', 1)[-1]) in Path('/proc/self/cgroup').read_text()
root = a.output
root.mkdir()
source = root / 'source'
source.mkdir()
with tarfile.open(a.archive) as archive:
    archive.extractall(source, filter='data')
sdk = root / 'sdk'
include = sdk / 'include'
sdk.mkdir()
for part in ('include', 'src', 'lib', 'tools', 'share', 'toolchains', 'example'):
    canonical = source / 'sdk' / part
    if canonical.is_dir():
        shutil.copytree(canonical, sdk / part)
for name in ('xmake.lua', 'CMakeLists.txt', 'README.md'):
    if (source / 'sdk' / name).is_file():
        shutil.copy2(source / 'sdk' / name, sdk / name)
if not include.is_dir():
    raise ValueError('The canonical SDK include tree is required')
if a.historical_layout:
    raise ValueError('Replay a closed archive with its retained original recipe; current tests require the canonical SDK')
commands = []
cases = []
n = 0
report = dict(
    kind='cpp-standard-matrix-v2',
    default_authority=[],
    native_model=[],
    cases=cases,
    commands=commands,
    source_archive_sha256=hashlib.sha256(a.archive.read_bytes()).hexdigest(),
    source_only=False,
    coverage=dict(
        native_ros=0, gateway=0, upstream_official=0, qemu=0, vtune_hardware=0, physical=0
    ),
)


def save():
    (root / 'matrix.json').write_text(json.dumps(report, indent=2) + '\n')


def cmd(label, argv, expected=0, timeout=120, diagnostic_all=(), diagnostic_forbidden=()):
    global n
    n += 1
    start = time.monotonic()
    out = root / (f'{n:04d}-' + label + '.stdout')
    err = out.with_suffix('.stderr')
    try:
        with out.open('wb') as stdout, err.open('wb') as stderr:
            r = subprocess.run(
                list(map(str, argv)), stdout=stdout, stderr=stderr, timeout=timeout, check=False
            )
        code = r.returncode
    except (OSError, subprocess.TimeoutExpired) as error:
        err.write_text(repr(error))
        code = -999
    exit_matched = code == expected if expected is not None else code > 0
    diagnostic = (
        out.read_text(errors='replace') + err.read_text(errors='replace')
        if diagnostic_all or diagnostic_forbidden
        else ''
    )
    diagnostic_matched = all(re.search(pattern, diagnostic) for pattern in diagnostic_all) and not any(
        re.search(pattern, diagnostic) for pattern in diagnostic_forbidden
    )
    good = exit_matched and diagnostic_matched
    commands.append(
        dict(
            label=label,
            argv=list(map(str, argv)),
            exit_code=code,
            expected=expected,
            exit_matched=exit_matched,
            diagnostic_required=list(diagnostic_all),
            diagnostic_forbidden=list(diagnostic_forbidden),
            diagnostic_matched=diagnostic_matched,
            matched=good,
            elapsed_seconds=time.monotonic() - start,
            stdout=out.name,
            stderr=err.name,
        )
    )
    save()
    return good


def case(kind, label, status, **extra):
    cases.append(dict(layer=kind, label=label, status=status, **extra))
    save()
    print(json.dumps(cases[-1]), flush=True)


# Consume the canonical SDK directly; this owned temporary copy is a build input,
# never a second maintained SDK or an authored-source fallback.
foundation_include = include / 'wasm/language/c++/include'
inc = ['-I' + str(include), '-I' + str(foundation_include)]
report['historical_public_include_search'] = False
libsource = sdk / 'src/platform/ros/cpp/library_profile.cpp'
canonical_tests = source / 'test/0073.ros-tests/support/source/sdk-tests/cpp'
tests = a.test_source_root.resolve(strict=True) if a.test_source_root else canonical_tests
profile = tests / 'profiles'
if not profile.is_dir() or not (tests.parent / 'c/mock_backend.h').is_file():
    raise ValueError('Complete canonical C++ tests and sibling C fixtures are required')
report['test_source_root'] = str(tests)
report['test_source_layout'] = 'explicit-canonical' if a.test_source_root else 'canonical'
report['test_input_pins'] = [
    dict(path=str(f), sha256=hashlib.sha256(f.read_bytes()).hexdigest())
    for directory in (tests, tests.parent / 'c')
    for f in sorted(directory.rglob('*'))
    if f.is_file()
]
if not a.historical_layout:
    # Keep the shared C model bodies immutable. Only this private candidate copy
    # rebinds their SDK includes, so C++ tests need no legacy SDK search roots.
    selected_tests = tests
    tests = root / 'model-tests/cpp'
    shutil.copytree(selected_tests, tests)
    fixture_pins = []
    for name in ('mock_backend.h', 'ownership_backend.h'):
        original = selected_tests.parent / 'c' / name
        before = original.read_bytes()

        def fixture_include(match):
            spelling = match.group(1)
            if spelling.startswith('roswasm/'):
                leaf = Path(spelling).name
                backend = 'ros1' if 'ros1' in leaf else 'ros2' if 'ros2' in leaf else 'ros'
                language = 'cpp' if leaf.endswith('.hpp') else 'c'
                dependency = Path('platform') / backend / language / spelling
                if not (include / dependency).is_file():
                    raise ValueError('Missing canonical fixture dependency: ' + str(dependency))
                return '#include <' + dependency.as_posix() + '>'
            return match.group(0)

        after = re.sub(r'#include\s*["<]([^"<>\n]+)[">]', fixture_include, before.decode()).encode()
        destination = tests.parent / 'c' / name
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(after)
        fixture_pins.append(dict(source=str(original), staged=str(destination),
                                 before_sha256=hashlib.sha256(before).hexdigest(),
                                 after_sha256=hashlib.sha256(after).hexdigest(),
                                 only_sdk_include_spelling_rebound=True))
    report['private_c_fixture_include_rebinding'] = fixture_pins
    report['staged_test_source_root'] = str(tests)
    profile = tests / 'profiles'
# Tests include their frozen C model by relative path; every public SDK include
# resolves only within the standalone candidate include tree above.
profile_modes = [
    (98, 'c++98'),
    (3, 'c++03'),
    (11, 'c++11'),
    (14, 'c++14'),
    (17, 'c++17'),
    (20, 'c++20'),
    (23, 'c++23'),
    (26, 'c++2c'),
]
compilers = [c for c in ('/usr/bin/g++-15', '/usr/bin/clang++-22') if Path(c).is_file()]
for cc in compilers:
    cmd('version', [cc, '--version'])
if not compilers:
    case('compiler', 'native-tools', 'UNAVAILABLE')
archives = {}
for cc in compilers:
    for selector, mode in profile_modes:
        label = Path(cc).name + '-' + mode
        work = root / label
        work.mkdir()
        flags = [
            cc,
            '-std=' + mode,
            '-DROSWASM_CPP_STANDARD=' + str(selector),
            '-pedantic-errors',
            '-O0',
        ] + inc
        probe = work / 'probe.cpp'
        probe.write_text(
            '#include <platform/ros/cpp/roswasm/cpp_profile.hpp>\nint main(){return 0;}\n'
        )
        if not cmd(label + '-probe', flags + [probe, '-o', work / 'probe']):
            case('compiler', label, 'UNAVAILABLE', requested=selector)
            continue
        cmd(label + '-predefines', flags + ['-dM', '-E', probe])
        definitions = (root / commands[-1]['stdout']).read_text()
        cplusplus = re.search(r'^#define __cplusplus (\d+)', definitions, re.M)
        draft = re.search(r'^#define ROSWASM_CPP_DRAFT_MODE (\d+)', definitions, re.M)
        case(
            'compiler-mode',
            label,
            'PASS',
            cplusplus=int(cplusplus.group(1)) if cplusplus else None,
            draft=bool(int(draft.group(1))) if draft else None,
        )
        obj = work / 'library.o'
        archive = work / 'profile.a'
        ok = cmd(label + '-source', flags + ['-c', libsource, '-o', obj])
        if ok:
            ok = cmd(label + '-archive', ['/usr/bin/ar', 'rcs', archive, obj])
        if ok:
            archives[(cc, selector)] = archive
        if ok:
            ok = cmd(
                label + '-link',
                flags
                + [
                    profile / 'link_main.cpp',
                    profile / 'link_peer.cpp',
                    archive,
                    '-o',
                    work / 'link',
                ],
            )
        if ok:
            ok = cmd(label + '-link-run', [work / 'link'])
        case(
            'native-library',
            label,
            'PASS' if ok else 'FAIL',
            translation_units=3,
            standard=selector,
        )
        if not ok:
            continue
        protocol_ok = cmd(
            label + '-protocol_shape',
            flags + [profile / 'protocol_shape.cpp', archive, '-o', work / 'protocol_shape'],
        )
        if protocol_ok:
            protocol_ok = cmd(label + '-protocol_shape-run', [work / 'protocol_shape'])
        case(
            'native-protocol-model',
            label + '-protocol_shape',
            'PASS' if protocol_ok else 'FAIL',
            actual_guest=False,
            native_ros=False,
        )
        if selector in (98, 3, 11, 14, 17):
            for test_name in ('portable_contract', 'portable_resources'):
                ok = cmd(
                    label + '-' + test_name,
                    flags + [profile / (test_name + '.cpp'), archive, '-o', work / test_name],
                )
                if ok:
                    ok = cmd(label + '-' + test_name + '-run', [work / test_name])
                case(
                    'native-model',
                    label + '-' + test_name,
                    'PASS' if ok else 'FAIL',
                    actual_guest=False,
                )
        else:
            for test in sorted(tests.glob('*.cpp')):
                if test.name in ('cpp11_profile.cpp', 'c_header_compatibility.cpp'):
                    continue
                ok = cmd(label + '-' + test.stem, flags + [test, archive, '-o', work / test.stem])
                if ok:
                    ok = cmd(label + '-' + test.stem + '-run', [work / test.stem])
                case(
                    'native-model',
                    label + '-' + test.stem,
                    'PASS' if ok else 'FAIL',
                    actual_guest=False,
                )
        bad = work / 'bad.cpp'
        bad.write_text('#include <platform/ros/cpp/roswasm/cpp_profile.hpp>\nint main(){return 0;}\n')
        case(
            'negative-compile',
            label + '-nonexistent13',
            (
                'PASS'
                if cmd(
                    label + '-reject13',
                    [cc, '-std=' + mode, '-DROSWASM_CPP_STANDARD=13']
                    + inc
                    + [bad, '-o', work / 'bad'],
                    expected=None,
                    diagnostic_all=(r'ROSWASM_CPP_STANDARD must be', r'C\+\+13 does not exist'),
                    diagnostic_forbidden=(r'No such file or directory|file not found',),
                )
                else 'FAIL'
            ),
        )
        if selector in (98, 3):
            copy = work / 'copy.cpp'
            copy.write_text(
                '#include <platform/ros/cpp/roswasm/roswasm.hpp>\nint main(){roswasmcpp::Capability a;roswasmcpp::Capability b(a);}\n'
            )
            case(
                'negative-compile',
                label + '-copy',
                (
                    'PASS'
                    if cmd(
                        label + '-rejectcopy', flags + [copy, '-o', work / 'copy'], expected=None,
                        diagnostic_all=(r'Capability', r'private|inaccessible'),
                        diagnostic_forbidden=(r'No such file or directory|file not found',),
                    )
                    else 'FAIL'
                ),
            )
    available = [s for s, _ in profile_modes if (cc, s) in archives]
    for selector in available:
        wrong = next((s for s in available if s != selector), None)
        if wrong is None:
            continue
        mode = dict(profile_modes)[selector]
        dest = root / (Path(cc).name + '-mismatch-' + str(selector))
        ok = cmd(
            dest.name,
            [cc, '-std=' + mode, '-DROSWASM_CPP_STANDARD=' + str(selector)]
            + inc
            + [
                profile / 'link_main.cpp',
                profile / 'link_peer.cpp',
                archives[(cc, wrong)],
                '-o',
                dest,
            ],
            expected=None,
            diagnostic_all=(r'undefined reference|undefined symbol',
                            'roswasm_cpp' + str(selector) + '_library_profile'),
            diagnostic_forbidden=(r'No such file or directory|file not found',),
        )
        case('negative-link', dest.name, 'PASS' if ok else 'FAIL', consumer=selector, library=wrong)
    if (cc, 98) in archives:
        bridge = root / (Path(cc).name + '-fast-io.o')
        client_binary = root / (Path(cc).name + '-fast-io-client')
        ok = cmd(
            Path(cc).name + '-fast-io-compile',
            [cc, '-std=c++20'] + inc + ['-c', sdk / 'src/platform/ros/cpp/fast_io_bridge.cpp', '-o', bridge],
        )
        if ok:
            ok = cmd(
                Path(cc).name + '-fast-io-link',
                [cc, '-std=c++98', '-DROSWASM_CPP_STANDARD=98']
                + inc
                + [profile / 'fast_io_client.cpp', archives[(cc, 98)], bridge, '-o', client_binary],
            )
        if ok:
            ok = cmd(Path(cc).name + '-fast-io-run', [client_binary])
        case(
            'c-abi-bridge',
            Path(cc).name + '-cpp98-cpp20-fast-io',
            'PASS' if ok else 'FAIL',
            standard_library_objects_crossed=False,
        )

# Generator behavior is a separate, explicitly requested current-tool scope.
if a.generators or a.vendors:
    generator_ok = cmd(
        'generator-composition',
        [sys.executable, '-I', '-S', '-B', profile / 'generator_contract.py',
         source, include, root / 'generator-contract', source, tests.parent / 'c'],
        timeout=600,
    )
    case('source-generator-contract', 'composition-and-exact-defaults',
         'PASS' if generator_ok else 'FAIL', native_ros=False)

if a.integrations:
    build = root / 'cmake-all'
    ok = cmd(
        'cmake-configure',
        [
            '/usr/bin/cmake',
            '-S',
            sdk,
            '-B',
            build,
            '-DCMAKE_CXX_COMPILER=/usr/bin/clang++-22',
            '-DROSWASM_BUILD_CPP_LIBRARIES=98;3;11;14;17;20;23;26',
            '-DROSWASM_BUILD_FAST_IO_BRIDGE=ON',
        ],
    )
    if ok:
        ok = cmd('cmake-build', ['/usr/bin/cmake', '--build', build, '-j1'], timeout=600)
    case('consumer-build', 'cmake-all-profile-libraries', 'PASS' if ok else 'FAIL')
    if ok:
        installed = root / 'installed'
        ok = cmd(
            'cmake-install',
            ['/usr/bin/cmake', '--install', build, '--prefix', installed],
            timeout=180,
        )
        case('consumer-install', 'cmake', 'PASS' if ok else 'FAIL')
    xmake = Path('/home/macromodel/.local/bin/xmake')
    if xmake.is_file():
        os.environ['XMAKE_PROGRAM_DIR'] = '/home/macromodel/.local/share/xmake'
        cmd('xmake-version', [xmake, '--version'])
        ok = cmd(
            'xmake-configure',
            [
                xmake,
                'f',
                '-P',
                sdk,
                '--ros-sdk-platforms=ros,ros1,ros2',
                '--ros-sdk-languages=cpp',
                '--ros-sdk-cpp-standard=98',
                '--sdk-toolchain=/explicitly-unavailable-toolchain',
            ],
        )
        case('consumer-configure', 'xmake', 'PASS' if ok else 'FAIL')
        previous = Path.cwd()
        os.chdir(sdk)
        try:
            rejected = cmd('xmake-reject-unavailable', [xmake, 'sdk-cpp-inspect'], expected=None)
        finally:
            os.chdir(previous)
        diagnostic = (root / commands[-1]['stdout']).read_text() + (
            root / commands[-1]['stderr']
        ).read_text()
        expected_reason = (
            'explicitly-unavailable-toolchain' in diagnostic
            or 'sdk-toolchain must' in diagnostic
            or 'wasm-profiles.json' in diagnostic
        )
        case(
            'unsupported-profile',
            'xmake-missing-profile-or-toolchain',
            'PASS' if rejected and expected_reason else 'FAIL',
            expected_reason_observed=expected_reason,
        )
    else:
        case('consumer-configure', 'xmake', 'UNAVAILABLE')

if a.vendors:
    module_path = source / 'sdk/tools/cpp_profiles.py'
    spec = importlib.util.spec_from_file_location('cpp_profiles', module_path)
    cpp = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(cpp)
    delivery = source / 'sdk/include/platform'
    families = []
    pins = []
    # All selected helper dependencies remain within this isolated include
    # candidate. Preserve the freshly assembled shared foundation above.
    for original in sorted(delivery.rglob('*')):
        if not original.is_file() or original.name.startswith('._'):
            continue
        rel = original.relative_to(source / 'sdk/include')
        dest = include / rel
        if dest.exists():
            continue
        if rel.parts[2] not in ('c', 'cpp'):
            continue
        body = original.read_bytes()
        if original.suffix == '.hpp' and rel.parts[2] == 'cpp':
            body, _ = cpp.transform(original, 'include/' + rel.as_posix(), body)
        if original.suffix == '.h' and rel.parts[2] == 'c':
            old = b'#if defined(__cplusplus)\n#define UWVM_BOUNDED_ASSERT(x) static_assert(x,"canonical representation required")'
            new = b'#if defined(__cplusplus) && __cplusplus >= 201103L\n#define UWVM_BOUNDED_ASSERT(x) static_assert(x,"canonical representation required")\n#elif defined(__cplusplus)\n#define UWVM_BOUNDED_ASSERT(x) typedef char uwvm_bounded_representation_required[(x)?1:-1]'
            body = body.replace(old, new)
            body = cpp.portable_c_float_defaults(body.decode()).encode()
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(body)
    for original in sorted(delivery.glob('*/cpp/**/bounded_records.hpp')):
        rel = original.relative_to(source / 'sdk/include')
        text = original.read_bytes()
        body, change = cpp.transform(original, 'include/' + rel.as_posix(), text)
        dest = include / rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(body)
        families.append(dest)
        for original2 in (original.with_name('bounded_interfaces.hpp'),):
            if original2.is_file():
                rel2 = original2.relative_to(source / 'sdk/include')
                body2, _ = cpp.transform(
                    original2, 'include/' + rel2.as_posix(), original2.read_bytes()
                )
                (include / rel2).write_bytes(body2)
        # Copy all C inputs for this actual selected family. Only the obsolete
        # C++ assertion branch changes, exactly as in the generator candidate.
        cdir = delivery / rel.parts[1] / 'c' / Path(*rel.parts[3:-1])
        for cfile in cdir.glob('*.h'):
            ctext = cfile.read_text()
            old = '#if defined(__cplusplus)\n#define UWVM_BOUNDED_ASSERT(x) static_assert(x,"canonical representation required")'
            new = '#if defined(__cplusplus) && __cplusplus >= 201103L\n#define UWVM_BOUNDED_ASSERT(x) static_assert(x,"canonical representation required")\n#elif defined(__cplusplus)\n#define UWVM_BOUNDED_ASSERT(x) typedef char uwvm_bounded_representation_required[(x)?1:-1]'
            cbody = cpp.portable_c_float_defaults(ctext.replace(old, new))
            cdest = include / cfile.relative_to(source / 'sdk/include')
            cdest.parent.mkdir(parents=True, exist_ok=True)
            cdest.write_text(cbody)
            pins.append(
                dict(
                    path=str(cfile.relative_to(source)),
                    before_sha256=hashlib.sha256(cfile.read_bytes()).hexdigest(),
                    candidate_sha256=hashlib.sha256(cbody.encode()).hexdigest(),
                    only_cpp_assertion_changed=cbody != ctext,
                )
            )
    (root / 'vendor-c-inputs.json').write_text(json.dumps(pins, indent=2) + '\n')
    report['vendor_families_denominator'] = len(families)
    save()
    cc = '/usr/bin/clang++-22'
    selected_vendor_standards = [int(value) for value in a.vendor_standards.split(',')]
    if len(set(selected_vendor_standards)) != len(selected_vendor_standards) or set(
        selected_vendor_standards
    ) - set(dict(profile_modes)):
        raise ValueError('explicit unique actual C++ standards required')
    report['vendor_standards_requested'] = selected_vendor_standards
    for selector, mode in profile_modes:
        if selector not in selected_vendor_standards:
            continue
        work = root / ('vendor-' + str(selector))
        work.mkdir()
        flags = [
            cc,
            '-std=' + mode,
            '-DROSWASM_CPP_STANDARD=' + str(selector),
            '-pedantic-errors',
            '-O0',
        ] + inc
        prefix = work / 'prefix.hpp'
        prefix.write_text(
            '#include <platform/ros/cpp/roswasm/roswasm.hpp>\n#include <platform/ros/cpp/roswasm/vendor.hpp>\n#include <cassert>\n'
        )
        pch = work / 'prefix.pch'
        if not cmd(
            'vendor-' + str(selector) + '-pch', flags + ['-x', 'c++-header', prefix, '-o', pch]
        ):
            case('vendor-compiler', str(selector), 'UNAVAILABLE')
            continue
        for i, header in enumerate(families):
            family = header.parent
            label = family.relative_to(include).as_posix()
            text = header.read_text()
            test = work / (str(i) + '.cpp')
            exe = work / str(i)
            rows = re.findall(r'typedef (roswasmc_bounded_\w+) (\w+);', text)
            lines = [
                '#include <' + header.relative_to(include).as_posix() + '>',
                '#include "' + str(tests.parent / 'c/mock_backend.h') + '"',
            ]
            interface = family / 'bounded_interfaces.hpp'
            if interface.is_file():
                lines.append('#include <' + interface.relative_to(include).as_posix() + '>')
            lines.append('int main(){')
            for c, name in rows:
                init = (
                    'roswasmcpp::Codec<uwvm_custom::' + name + '>::default_value()'
                    if ('static uwvm_custom::' + name + ' default_value()') in text
                    else 'uwvm_custom::' + name + '()'
                )
                lines += [
                    '{typedef uwvm_custom::'
                    + name
                    + ' T;T* value=new T('
                    + init
                    + ');T* out=new T();roswasmcpp::wire::bytes bytes;',
                    'assert(roswasmcpp::Codec<T>::encode(*value,bytes));assert(roswasmcpp::Codec<T>::decode(bytes,*out));roswasmcpp::wire::bytes again;',
                    'assert(roswasmcpp::Codec<T>::encode(*out,again)&&again==bytes);bytes.push_back(0);assert(!roswasmcpp::Codec<T>::decode(bytes,*out));delete out;delete value;}',
                ]
            if interface.is_file():
                itext = interface.read_text()
                lines.append('roswasmc_context context={model_backend()};')
                for binding, name in re.findall(
                    r'typedef ::roswasmcpp::vendor_detail::ServiceClient<(\w+)> (\w+)Client;', itext
                ):
                    lines += [
                        '{uwvm_custom::'
                        + name
                        + 'Client client(context,"denied");assert(client.error()==13);uwvm_custom::'
                        + name
                        + 'Server server(context,"denied.a","denied.b");assert(server.error()==13);}'
                    ]
                for binding, name in re.findall(
                    r'typedef ::roswasmcpp::vendor_detail::ActionClient<(\w+)> (\w+)Client;', itext
                ):
                    lines += [
                        '{roswasmc_action_aliases a={{"send",4},{"own",3},{"feedback",8},{"result",6},{0,0}};uwvm_custom::'
                        + name
                        + 'Client client(context,a);assert(client.error()==13);uwvm_custom::'
                        + name
                        + 'Server server(context,"denied.a","denied.b");assert(server.error()==13);}'
                    ]
            lines += ['return 0;}']
            test.write_text('\n'.join(lines) + '\n')
            ok = cmd(
                'vendor-' + str(selector) + '-' + str(i),
                flags + ['-include-pch', pch, test, '-o', exe],
            )
            if ok:
                ok = cmd('vendor-' + str(selector) + '-' + str(i) + '-run', [exe])
            case(
                'native-vendor-model',
                label + '-' + str(selector),
                'PASS' if ok else 'FAIL',
                records=len(rows),
                actual_guest=False,
            )

if a.wasm:
    wasi = Path(
        '/home/macromodel/Documents/uwvm-test-261018/agents/10/staged-wasi-r1/staged/wasi-sdk'
    )
    runtime = Path('/home/macromodel/Documents/uwvm-test-261018/agents/02/c-matrix-r2/inputs/uwvm2-ros')
    report['wasm_toolchain'] = str(wasi)
    report['runtime'] = str(runtime)

    def guest_command(exe):
        result = [
            runtime,
            '--runtime-int',
            '-Rct',
            '0',
            '--wasm-set-main-module-name',
            'main',
            '--wasip1-global-disable',
            '--wasip1-global-noinherit-system-environment',
            '--wasip1-single-create',
            'main',
            '--wasip1-single-enable',
            'main',
            '--wasip1-single-hide-host-api',
            'main',
            '--wasip1-single-noinherit-system-environment',
            'main',
        ]
        for right in (
            'realtime-clock',
            'monotonic-clock',
            'process-clock',
            'thread-clock',
            'random',
            'proc-raise',
            'sched-yield',
            'stdio-full-rights',
        ):
            result += ['--wasip1-single-deny', 'main', right]
        return result + ['--run', exe]

    for selector, mode in profile_modes:
        work = root / ('wasm-' + str(selector))
        work.mkdir()
        exe = work / 'denial.wasm'
        flags = [
            wasi / 'bin/clang++',
            '--target=wasm32-wasip1',
            '-std=' + mode,
            '-DROSWASM_CPP_STANDARD=' + str(selector),
            '-fno-exceptions',
            '-fno-rtti',
            '-O2',
        ] + inc
        ok = cmd(
            'wasm-' + str(selector), flags + [sdk / 'example/platform/ros2/cpp/profiles/default_denial.cpp', '-o', exe]
        )
        case(
            'wasm-compile',
            'typed-denial-' + str(selector),
            'PASS' if ok else 'FAIL',
            address_width=32,
            exceptions=False,
            threads=False,
        )
        if ok:
            report.setdefault('artifact_sizes', []).append(
                dict(
                    path=str(exe.relative_to(root)),
                    bytes=exe.stat().st_size,
                    sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),
                )
            )
            valid = cmd('wasm-validate-' + str(selector), ['/usr/bin/wasm-validate', exe])
            case('wasm-validation', str(selector), 'PASS' if valid else 'FAIL')
            if runtime.is_file():
                good = cmd('guest-denial-' + str(selector), guest_command(exe))
                case(
                    'actual-guest',
                    'default-denial-' + str(selector),
                    'PASS' if good else 'FAIL',
                    native_ros=False,
                )
            cmd('wasm-imports-' + str(selector), ['/usr/bin/wasm-objdump', '-x', exe])
        raw = work / 'raw-denial.wasm'
        raw_ok = cmd(
            'wasm-raw-' + str(selector),
            flags + ['-nostdlib++', sdk / 'example/platform/ros2/cpp/profiles/raw_default_denial.cpp', '-o', raw],
        )
        case(
            'wasm-raw-compile',
            'raw-denial-' + str(selector),
            'PASS' if raw_ok else 'FAIL',
            typed_cpp_coverage=False,
        )
        if raw_ok:
            raw_valid = cmd('wasm-raw-validate-' + str(selector), ['/usr/bin/wasm-validate', raw])
            case(
                'wasm-raw-validation',
                str(selector),
                'PASS' if raw_valid else 'FAIL',
                typed_cpp_coverage=False,
            )
            if runtime.is_file():
                raw_good = cmd('guest-raw-denial-' + str(selector), guest_command(raw))
                case(
                    'actual-raw-guest',
                    'default-denial-' + str(selector),
                    'PASS' if raw_good else 'FAIL',
                    typed_cpp_coverage=False,
                    native_ros=False,
                )
        if ok:
            modules = {}
            for name in ('callbacks', 'codec', 'client'):
                module = work / (name + '.wasm')
                built = cmd(
                    'preload-' + str(selector) + '-' + name,
                    flags
                    + [
                        '-mexec-model=reactor',
                        sdk / 'example/platform/ros/cpp/profiles/preload' / (name + '.cpp'),
                        '-o',
                        module,
                    ],
                )
                if built:
                    modules[name] = module
                    cmd(
                        'preload-imports-' + str(selector) + '-' + name,
                        ['/usr/bin/wasm-objdump', '-x', module],
                    )
                    cmd(
                        'preload-validate-' + str(selector) + '-' + name,
                        ['/usr/bin/wasm-validate', module],
                    )
            main = work / 'preload-main.wasm'
            built = cmd(
                'preload-' + str(selector) + '-main',
                flags + [sdk / 'example/platform/ros/cpp/profiles/preload/main.cpp', '-o', main],
            )
            case(
                'wasm-preload-compile',
                str(selector),
                'PASS' if built and len(modules) == 3 else 'FAIL',
                independent_modules=4,
            )
            if built and len(modules) == 3 and runtime.is_file():
                invocation = guest_command(main)
                preloads = []
                for name in ('callbacks', 'codec', 'client'):
                    preloads += [
                        '--wasip1-single-create',
                        name,
                        '--wasip1-single-enable',
                        name,
                        '--wasip1-single-hide-host-api',
                        name,
                        '--wasip1-single-noinherit-system-environment',
                        name,
                    ]
                    for right in (
                        'realtime-clock',
                        'monotonic-clock',
                        'process-clock',
                        'thread-clock',
                        'random',
                        'proc-raise',
                        'sched-yield',
                        'stdio-full-rights',
                    ):
                        preloads += ['--wasip1-single-deny', name, right]
                    preloads += ['--wasm-preload-library', modules[name], name]
                invocation = invocation[:-2] + preloads + invocation[-2:]
                good = cmd('preload-guest-' + str(selector), invocation)
                case(
                    'actual-preload-guest',
                    str(selector),
                    'PASS' if good else 'FAIL',
                    modules=4,
                    native_ros=False,
                )
report['status'] = (
    'FAIL'
    if any(c['status'] == 'FAIL' for c in cases)
    else (
        'PASS_WITH_EXPLICIT_UNAVAILABLE'
        if any(c['status'] == 'UNAVAILABLE' for c in cases)
        else 'PASS'
    )
)
save()
print(
    json.dumps(dict(status=report['status'], cases=len(cases), path=str(root / 'matrix.json'))),
    flush=True,
)
sys.exit(1 if report['status'] == 'FAIL' else 0)
