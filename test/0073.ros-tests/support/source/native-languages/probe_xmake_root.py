#!/usr/bin/env python3
"""@brief Diagnose description-scope SDK-root capture with real xmake.

@details This small host-only reproduction imports the exact installed common
profile module. It creates no SDK substitute and earns no installed-build or
Guest credit. Every new file is under the active finite owner.
@see sdk/origin/tools/xmake/languages/native_languages.lua
"""
from pathlib import Path
import hashlib
import json
import subprocess


def main():
    """@brief Print the common request's actual root and its expected install root.

    @return Zero after actual xmake configuration and request construction.
    @failure Preserve logs and propagate configuration/task failures.
    @see sdk/origin/tools/xmake/common/profiles.lua
    """
    root = Path.cwd()
    sdk = Path('/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-round03-installed-nim-r1/work/relocated-sdk')
    module = sdk / 'tools/xmake/common/profiles.lua'
    xmake = Path('/home/macromodel/.local/bin/xmake')
    native = Path(__file__).parent / 'native_languages.lua'
    source = ('includes(' + json.dumps(str(module)) + ')\n' +
              'local api=uwvm_sdk_common()\napi.register_options()\n' +
              'task("native_languages-root-probe")\n set_menu{usage="xmake native_languages-root-probe",description="Read actual SDK-root request",options={}}\n on_run(function()\n' +
              '  import("core.project.config").load()\n  api.bind(import)\n' +
              '  local request=api.request("nim",{})\n' +
              '  print("ACTUAL_REQUEST_ROOT="..request.sdk_root)\n' +
              '  print("NORMALIZED_REQUEST_ROOT="..path.normalize(path.absolute(request.sdk_root)))\n' +
              '  print("EXPECTED_INSTALL_ROOT="..' + json.dumps(str(sdk)) + ')\n' +
              '  local m=import("native_languages",{rootdir=' + json.dumps(str(native.parent)) + ',anonymous=true})\n' +
              '  local n=m.uwvm_sdk_native_languages()\n' +
              '  n.request(api,"nim",' + json.dumps(str(sdk)) + ',{version="2.2.4",config_get=function(k) if k=="sdk-ros-backend" then return "ros2" end return import("core.project.config").get(k) end})\n' +
              '  print("PASS_NORMALIZED_NATIVE_LANGUAGES_REQUEST")\n' +
              ' end)\ntask_end()\n')
    (root / 'xmake.lua').write_text(source)
    rows = []
    for name, arguments in (
            ('configure', ['f', '--yes', '--sdk-toolchain=' + str(sdk)]),
            ('request', ['native_languages-root-probe'])):
        log = root / (name + '.log')
        with log.open('wb') as output:
            result = subprocess.run([str(xmake), *arguments], cwd=root,
                                    stdout=output, stderr=subprocess.STDOUT, timeout=60)
        rows.append({'name': name, 'argv': [str(xmake), *arguments], 'exit': result.returncode,
                     'log': log.name, 'text': log.read_text(errors='replace')})
        (root / 'probe.json').write_text(json.dumps({'rows': rows,
            'common_module_sha256': hashlib.sha256(module.read_bytes()).hexdigest(),
            'installed_SDK_build_credit': 0, 'Guest_credit': 0}, indent=2) + '\n')
        if result.returncode:
            return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
