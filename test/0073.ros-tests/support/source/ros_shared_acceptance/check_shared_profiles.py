#!/usr/bin/env python3
"""Use actual sysroot/tool binaries to test common resolution and argv behavior."""
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--tools', type=Path, required=True)
    parser.add_argument('--artifact-sdk', type=Path, required=True)
    parser.add_argument('--toolchain', type=Path, required=True)
    parser.add_argument('--wasm-tools', type=Path, required=True)
    parser.add_argument('--xmake', type=Path)
    parser.add_argument('--runtime', type=Path)
    args = parser.parse_args()
    sys.path.insert(0, str(args.tools.resolve(strict=True)))
    from resource_contract import validate_resource_owner
    from wasm_profiles import DEFAULT_PROFILE, export_clang_configs, inspect_user_argv, invoke, prepare_toolchain, resolve
    from wasi_sysroot import make_profile_manifest
    root = Path.cwd()
    report = dict(status='RUNNING', resource=validate_resource_owner(), cases=[],
        default_authority=[], actual_guest_cases=0, native_ros_cases=0, gateway_cases=0,
        upstream_cases=0, qemu_cases=0, vtune_hardware_cases=0)
    report['source_pins'] = [dict(path=str(path), bytes=path.stat().st_size,
        sha256=hashlib.sha256(path.read_bytes()).hexdigest()) for path in
        [Path(__file__), *sorted(args.tools.rglob('*.py')), *sorted(args.tools.rglob('*.lua'))]]
    result = root / 'shared-profiles.json'
    def save():
        result.write_text(json.dumps(report, indent=2, sort_keys=True) + '\n')
    def case(name, scope):
        report['cases'].append(dict(name=name, status='PASS', scope=scope))
        save()
    def refused(name, change, message=None):
        request = copy.deepcopy(base)
        change(request)
        try:
            resolve(request)
        except (ValueError, OSError) as error:
            if message and message not in str(error):
                raise
        else:
            raise RuntimeError('unsupported/conflicting profile accepted: ' + name)
        case(name, 'configuration-refusal')
    try:
        sdk = root / 'SDK with spaces-路径'
        shutil.copytree(args.artifact_sdk, sdk)
        metadata = json.loads((sdk / 'share/platform/wasi/sysroot.json').read_bytes())
        manifest = make_profile_manifest(metadata, args.toolchain, {'c','cpp'})
        manifest_path = sdk / 'share/wasm-profiles.json'
        manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
        base = dict(format='uwvm-sdk-build-request-v1', sdk_root=str(sdk),
            toolchain_root=str(args.toolchain.resolve(strict=True)), profile_manifest=str(manifest_path),
            offline=True, language='cpp', profile=dict(DEFAULT_PROFILE),
            language_options=dict(standard='c++20',library_mode='libc++',compiler_argv=['-std=c++20'],
                linker_argv=[],runtime={}))
        resolved = resolve(base)
        if resolved['profile'] != DEFAULT_PROFILE or resolved['language_options'] != base['language_options']:
            raise RuntimeError('configuration replaced caller language/profile options')
        case('actual-relocated-default-profile-and-caller-options', 'actual-artifact-resolution')
        source = root / 'application with spaces-中文.cpp'
        source.write_text('#include <cstdio>\n#include <vector>\nint main(){std::vector<int> v={1,2,3};'
            'std::printf("%u\\n",unsigned(v.size()));return v.size()!=3;}\n')
        artifact = root / 'artifact with spaces-结果.wasm'
        response = root / 'arguments with spaces-参数.rsp'
        response.write_text(' '.join(shlex.quote(str(value)) for value in (source, '-Wl,--strip-debug', '-o', artifact)) + '\n')
        if invoke(resolved, 'link', ['@' + str(response)]) != 0 or not artifact.is_file():
            raise RuntimeError('real Clang response-file/path invocation failed')
        subprocess.run([str(args.wasm_tools), 'validate', str(artifact)], stdin=subprocess.DEVNULL, check=True, timeout=20)
        case('real-clang-response-file-spaces-unicode-linker-option', 'actual-clang-link-and-wasm-validation')
        configs = export_clang_configs(resolved, root / 'Clang configs-配置')
        configured = root / 'configured.wasm'
        subprocess.run([*configs['phases']['link']['command'], str(source), '-o', str(configured)],
            stdin=subprocess.DEVNULL, check=True, timeout=60)
        subprocess.run([str(args.wasm_tools), 'validate', str(configured)], stdin=subprocess.DEVNULL, check=True, timeout=20)
        case('actual-clang-config-export-and-link', 'actual-clang-link-and-wasm-validation')
        # Audit the real process-launch argv, including an empty element. Clang
        # may itself ignore an empty argument; its parser behavior must not be
        # confused with the launcher's argument-preservation contract.
        captured = inspect_user_argv(['', str(source), '-Wl,--strip-debug'], root, 'c++20')
        if captured:
            raise RuntimeError('ordinary argv unexpectedly became a response file')
        launched=[]
        def audit(event, details):
            if event=='subprocess.Popen':launched.append(list(details[1]))
        sys.addaudithook(audit)
        empty_arguments=['',str(source),'-o',str(root/'empty-argv.wasm')]
        empty_exit=invoke(resolved,'link',empty_arguments)
        expected_argv=[resolved['compiler'],*resolved['compiler_argv'],*resolved['linker_argv'],*empty_arguments]
        if not launched or launched[-1]!=expected_argv:
            raise RuntimeError('empty/Unicode argv differs at the actual process-launch boundary')
        report['empty_argument_compiler_exit']=empty_exit
        case('empty-argv-preserved-at-real-process-launch','actual-process-argv-transport')
        refused('memory64-pointer-width-conflict', lambda q:q['profile'].update(target='wasm64-wasip1'), 'pointer width conflict')
        refused('missing-wasm64-closure-no-fallback', lambda q:q['profile'].update(target='wasm64-wasip1',address_bits=64), 'no unique')
        refused('missing-thread-closure-no-fallback', lambda q:q['profile'].update(threads=True), 'no unique')
        refused('missing-eh-closure-no-fallback', lambda q:q['profile'].update(exceptions=True), 'no unique')
        refused('missing-selected-toolchain', lambda q:q.update(toolchain_root=str(root/'absent-toolchain')))
        refused('different-language-library-mode', lambda q:q['language_options'].update(library_mode='unprovided-runtime'), 'strategy is unsupported')
        refused('libcxx-standard-boundary', lambda q:q['language_options'].update(standard='c++98',compiler_argv=['-std=c++98']), 'library does not support')
        bad = root / 'conflicting.rsp'
        bad.write_text('--target=wasm64-wasip1\n')
        refused('response-file-conflicting-target', lambda q:q['language_options'].update(compiler_argv=['@'+str(bad)]), 'ABI/profile input')
        refused('caller-standard-conflict', lambda q:q['language_options'].update(compiler_argv=['-std=c++11']), 'standard conflicts')
        refused('linker-memory-feature-override', lambda q:q['language_options'].update(linker_argv=['-Wl,--shared-memory']), 'ABI override')
        old = manifest['profiles'][0]['library_abi']['address_bits']
        manifest['profiles'][0]['library_abi']['address_bits'] = 64
        manifest_path.write_text(json.dumps(manifest))
        try:
            refused('actual-library-abi-width-mismatch', lambda q:None, 'library ABI')
        finally:
            manifest['profiles'][0]['library_abi']['address_bits'] = old
            manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
        dependency = manifest['profiles'][0]['languages']['cpp']['tool_dependencies'][0]
        original_digest = dependency['sha256']
        dependency['sha256'] = '0' * 64
        manifest_path.write_text(json.dumps(manifest))
        try:
            refused('changed-native-compiler-dependency-refused', lambda q:None, 'SHA-256 differs')
        finally:
            dependency['sha256'] = original_digest
            manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
        prepared = prepare_toolchain(base)
        if prepared['status'] != 'EXISTING_TOOLCHAIN_SELECTED_NOT_EXECUTION_QUALIFIED':
            raise RuntimeError('offline existing-tool selection changed')
        case('explicit-existing-toolchain-offline-preparation', 'preparation')
        absent = copy.deepcopy(base)
        absent['toolchain_root'] = str(root / 'missing-offline-tool')
        try:
            prepare_toolchain(absent)
        except ValueError as error:
            if 'offline preparation' not in str(error):
                raise
        else:
            raise RuntimeError('offline preparation acquired an absent tool')
        case('offline-missing-tool-refused-without-download', 'preparation-refusal')
        if args.xmake is not None:
            xmake_env = dict(os.environ, XMAKE_STATS='false', XMAKE_SKIP_HISTORY='y')
            project = root / 'xmake-consumer'
            project.mkdir()
            common = args.tools / 'xmake/common/profiles.lua'
            # This is an isolated consuming project. It does not edit the
            # owner-01 sdk/xmake.lua or turn a Runtime into a header-only target.
            def lua_string(value):
                return json.dumps(str(value), ensure_ascii=False)
            lua = 'includes(' + lua_string(common) + ')\nlocal shared=uwvm_sdk_common()\nshared.register_options()\n'
            lua += 'target("scoped-profile-test")\nset_kind("phony")\non_build(function()\nshared.bind(import)\n'
            lua += 'local selected=shared.request("cpp", {standard="c++20",library_mode="libc++",compiler_argv={"-std=c++20"},linker_argv={},runtime={}})\n'
            lua += 'selected.sdk_root=' + lua_string(sdk) + '\n'
            lua += 'local resolved=shared.resolve(selected,' + lua_string(root/'xmake-resolved.json') + ')\n'
            lua += 'shared.invoke(resolved,"link",{' + ','.join(lua_string(v) for v in (source,'-o',root/'xmake-output.wasm')) + '})\nend)\ntarget_end()\n'
            (project / 'xmake.lua').write_text(lua)
            subprocess.run([str(args.xmake),'f','-y','-c','--sdk-toolchain='+str(args.toolchain),
                '--sdk-profile-manifest='+str(manifest_path)],cwd=project,env=xmake_env,check=True,timeout=60)
            subprocess.run([str(args.xmake),'-j1','scoped-profile-test'],cwd=project,env=xmake_env,check=True,timeout=120)
            subprocess.run([str(args.wasm_tools),'validate',str(root/'xmake-output.wasm')],check=True,timeout=20)
            case('real-xmake-entry-common-resolve-and-clang-build', 'actual-xmake-build-and-wasm-validation')
        if args.runtime is not None:
            for name, expected in (('c-command',b'wasi-c=10\n'),('cpp-command',b'wasi-cpp=14\n')):
                wasm=args.artifact_sdk.parent/(name+'.wasm')
                value=subprocess.run([str(args.runtime),'-Rint','--wasip1-global-noinherit-system-environment','--run',str(wasm)],
                    stdin=subprocess.DEVNULL,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=30)
                if value.returncode or value.stdout!=expected:
                    raise RuntimeError((name,value.returncode,value.stdout,value.stderr))
                case(name+'-current-runtime-basic-io-only','actual-wasm-guest')
                report['actual_guest_cases']+=1
        report.update(status='PASS',passed=len(report['cases']),denominator=18+(1 if args.xmake else 0)+(2 if args.runtime else 0),
            artifact_manifest_sha256=hashlib.sha256(manifest_path.read_bytes()).hexdigest(),
            supported_default_route_only=True, optional_profile_runtime_qualification=False,
            pinned_download_route_tested=False)
        if report['passed'] != report['denominator']:
            raise RuntimeError(('wrong scoped denominator',len(report['cases']),report['denominator']))
        save()
        print(json.dumps(dict(status='PASS',passed=report['passed'],denominator=report['denominator'])),flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error))
        save()
        raise


if __name__ == '__main__':
    main()
