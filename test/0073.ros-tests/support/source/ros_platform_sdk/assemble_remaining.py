#!/usr/bin/env python3
"""Deliver original provenance, opt-in CMake projects and English SDK guides.

This authoring phase changes no Runtime policy and executes no compiler,
Guest or upstream source. Unavailable vendor libraries remain named gaps.
Every CMake target is OFF until the operator selects an exact source domain.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

SOURCE_KEYS = {'deep_robotics': ('deep',), 'pal': ('pal', 'pal_tiago'),
               'trossen': ('interbotix', 'interbotix_models'), 'ubtech': ('ubtech',),
               'agibot': ('agibot',), 'fourier': ('fourier', 'fourier_bridge')}


def guide(vendor, profiles):
    first = profiles[0]
    display = first['display']
    macro = vendor.upper()
    lines = [f'# {display}: selected original ROS source SDK\n',
        'This SDK contains complete checked records for each explicitly admitted source domain. '
        'It is source preparation, not controller acceptance or a claim that every vendor API is available. '
        'Complete native/Wasm C/C++ data contracts and four-managed-language full-field tests '
        'have passed for the selected profiles. Application, architecture and native delivery '
        'acceptance retain their separately recorded scopes in '
        '`documents/ros/remaining-vendor-sdks-20261020.md`.\n',
        '## Select the original domain\n',
        '| Selection | ROS family | Original roots admitted / observed | Complete definitions |',
        '| --- | --- | --- | --- |']
    for row in profiles:
        lines.append(f"| `{row['id']}` | {row['ros_version']} | {row['admitted_roots']} / {row['original_roots']} | {row['complete_definitions']} |")
    lines += ['\n`catalogs/SELECTION/source-lock.json` records exact repository, commit, tree, original file '
        'hashes (or official ZIP archive/member proofs), excluded roots, core dependency provenance and storage bounds. Original bytes remain under '
        '`upstream/`. Branch names alone do not establish the ROS family. Native metadata, ROS distribution, '
        'controller, firmware, model, namespace and launch configuration must be qualified separately.\n',
        '## Start with the imports\n',
        'All six languages call the existing `ros1` or `ros2` Wasm import module. An import is a checked '
        'operation, not a native pointer, socket, SDK object or unrestricted middleware handle. '
        'Start with the raw ABI in `sdk/origin/include/roswasm/ros1.h` and `ros2.h`; the C owner API in '
        '`roswasm/codec.h` adds counted inputs, explicit handles and cleanup.\n',
        '```c\n#include <roswasm/ros2.h>\n#include <roswasm/codec.h>\n'
        '/* The operator defines this alias and its exact permitted endpoint. */\n'
        'uint32_t inspect_alias(char const *alias, size_t length) {\n'
        '    roswasmc_context context = {0};\n    roswasmc_capability cap = {0};\n'
        '    roswasmc_context_init(&context, &roswasmc_ros2_backend);\n'
        '    roswasmc_capability_init(&cap, &context);\n'
        '    uint32_t primary = roswasmc_open(&cap, alias, length);\n'
        '    uint32_t cleanup = cap.handle ? roswasmc_close(&cap) : 0;\n'
        '    return primary ? primary : cleanup;\n}\n```\n',
        'For ROS1, select `ros1.h` and `roswasmc_ros1_backend` explicitly. With no ROS grants, a well-formed '
        'ungranted alias returns 13. The examples need only basic stdin/stdout/stderr. Building a vendor '
        'bundle grants no filesystem, network or ROS rights. Guest code cannot change policy, load updates '
        'or administer the debugger. There is no implicit grant when several vendors are selected.\n',
        '## Add complete values and protocol owners\n',
        'The matching `sdk/origin/custom-types/FAMILY/bounded_records.*` supplies all original admitted fields and '
        'constants in C, C++, Rust, JavaScript, AssemblyScript and TinyGo. Protocol families also supply '
        '`bounded_interfaces.*`, including complete typed service request/response and action '
        'goal/feedback/result owners, tickets, cancellation and cleanup.\n',
        '`include/state.h` wraps one complete selected observation and an explicit caller-provided value. '
        '`state.hpp` adds borrowed C++ `Reader` and `Publisher` facades; construction opens nothing. '
        'The named sample pair in `profiles.json` illustrates checked transport; it is not evidence that '
        'these records are live robot topics. For a service or action, use its generated protocol owner '
        'instead of publishing a payload record. No zero motion, stop or retry is synthesized.\n',
        'Closing a capability releases software resources. It does not establish a physical emergency stop. '
        'A timeout or lost reply is an unknown controller outcome and must not cause an automatic motion replay.\n',
        '## Explicit storage capacities\n',
        'Catalog `bounds` entries are the exact finite canonical storage contract. They do not alter fixed '
        'arrays and never truncate data. Typical unbounded strings use 128 UTF-8 bytes, scalar sequences '
        '16 elements, nested sequences four elements, and joint vectors 64 elements. Selected image/cloud '
        'byte sequences use 4096 bytes. Larger values fail before publication or assignment. '
        'Finite IEEE754 values, 32 KiB per record, depth 16 and 4096 expanded nodes remain enforced. '
        'Source preparation explicitly selects an aggregate budget of 65536 nodes. '
        'These choices are not physical robot limits or arbitrary native camera-frame support.\n',
        '## Build the C/C++ entries\n',
        'Use an actual Linux test cgroup, a complete source checkout and fresh absolute output paths. '
        'All CMake targets are OFF by default. For example:\n',
        '```sh\nSRC=/absolute/src\nOUT=/absolute/cgroup-test-output\n'
        f'KIT="$SRC/sdk/origin/robots/platforms/{vendor}"\nPROFILE={first["id"]}\n'
        f'BUNDLE="$SRC/sdk/origin/custom-types/{first["family"]}"\n'
        'python3 "$SRC/sdk/origin/robots/humanoids/leju/tools/select_profile.py" \\\n'
        '  --kit "$KIT" --bundle "$BUNDLE" --profile "$PROFILE" --check-existing\n'
        'cmake -S "$KIT" -B "$OUT/cmake" -DCMAKE_BUILD_TYPE=MinSizeRel \\\n'
        '  -DCMAKE_TOOLCHAIN_FILE=/absolute/wasi-sdk/share/cmake/wasi-sdk.cmake \\\n'
        f'  -DUWVM_{macro}_BUILD_WASI_EXAMPLES=ON \\\n'
        f'  -DUWVM_{macro}_PROFILE="$PROFILE" -DUWVM_{macro}_SOURCE_ROOT="$SRC" \\\n'
        f'  -DUWVM_{macro}_GENERATED="$BUNDLE"\n'
        'cmake --build "$OUT/cmake" --parallel 1\n'
        'uwvm2-ros -Rint --wasip1-global-noinherit-system-environment \\\n'
        f'  --run "$OUT/cmake/{vendor}_guest_cpp.wasm" state {vendor}.observation\n```\n',
        'The last command demonstrates default refusal until explicit operator policy is supplied. '
        'CMake checks the original source lock, complete catalog, every declaration and generated product '
        'against the selected association. A different selection is refused even if some bodies coincide.\n',
        '## Use each language\n',
        '| Language | Application and full record entry |', '| --- | --- |',
        '| C / C++ | `examples/guest.c`, `guest.cc`; `include/state.h`, `state.hpp` |',
        '| Rust | `examples/rust/main.rs`; exactly one source feature; complete generated modules |',
        '| JavaScript | `examples/guest_SELECTION.mjs`; `javascript-SELECTION.json` builds QuickJS/WASI |',
        '| AssemblyScript | `examples/main_SELECTION.ts`; complete typed codecs and protocol owners |',
        '| TinyGo | `examples/go/main.go`; exactly one ROS and source-domain build tag |\n',
        '```sh\ncargo build --offline --release -j1 --target wasm32-wasip1 \\\n'
        f'  --no-default-features --features {first["id"]} \\\n'
        '  --manifest-path "$KIT/examples/rust/Cargo.toml" --target-dir "$OUT/rust"\n'
        '# Match the canonical module IDs used by the checked abort import.\n'
        'cd "$SRC"\n'
        f'asc "$KIT/examples/main_{first["id"]}.ts" --runtime incremental \\\n'
        '  --use abort=sdk/origin/assemblyscript/api/abortGuest --optimizeLevel 1 \\\n'
        '  --initialMemory 256 --maximumMemory 1024 --outFile "$OUT/assemblyscript.wasm"\n'
        '# Select the actual TinyGo module and its relative local SDK dependency.\n'
        'cd "$KIT/examples/go"\n'
        'tinygo build -target=./wasmros-wasi.json -scheduler=none -gc=leaking \\\n'
        f'  -panic=trap -p=1 -tags="ros{first["ros_version"]} {vendor}_{first["id"]}" \\\n'
        '  -o "$OUT/tinygo.wasm" .\n```\n',
        'The JavaScript recipe is consumed by `sdk/origin/javascript/guest/prepare.py`. JavaScript runs inside '
        'the existing QuickJS Wasm engine. Node/native ROS access is not a substitute. '
        'C/C++ accept `raw|state|command ALIAS`; Rust and TinyGo accept `state|command ALIAS`. '
        'AssemblyScript and JavaScript use a mode byte (0 state, 1 command), a u8 UTF-8 alias byte count, '
        'and the counted alias. Command mode reads u32LE length plus the full canonical value from stdin '
        'only after capability admission. JavaScript reports separate primary/cleanup u32LE statuses; '
        'the other entries return the effective status.\n',
        '## Runtime build selection and qualification\n',
        f'Runtime selection uses `--ros-vendor-{vendor.replace("_", "-")}=y` with the matching independently '
        'native-bound registry header. All unselected vendor options remain OFF. Multiple vendors require '
        'one coherent merged/deduplicated native registry and separate least-privilege aliases. '
        'Source-only families cannot be passed off as native-bound manifests. '
        'See `documents/ros/vendor-build-options.md` for complete A+B selections and size measurements.\n',
        'The sources here do not qualify the full rcl/rclcpp/rclpy/rcljava API, original official suites, '
        'native driver, controller, all releases/models, simulation, physical robots or hardware VTune '
        'Hotspots/Microarchitecture. These remain separate acceptance lanes. No native object or unchecked '
        'address crosses the Wasm boundary.\n']
    if vendor == 'ubtech':
        lines += ['## Original SDK gaps\n', 'The public `tk_sdk_demo` tree provides the two '
            '`grab_demo_msgs` declarations captured here. `bodyctrl_msgs` and `lyre_msgs` used by '
            'the original motor, power and voice demos are not supplied by this source tree. '
            'Their schemas and native installation must be obtained and pinned before those '
            'control APIs can be implemented or qualified. WalkerS2 model files are not a replacement SDK.\n']
    if vendor == 'agibot':
        lines += ['## Exact AimDK source distribution\n',
            'The official download manifest labels five SDK selections: v0.8.1, v0.8.2, v0.9.0, '
            'v1.0.0 and v1.1.0. The recorded archive filenames/URLs can carry a different internal '
            'release or release-candidate label; both are preserved rather than conflated. '
            'The v1.0.0 and v1.1.0 RL protocol packages are two separate additional domains. '
            'Prebuilt AArch64 copies and IDL remain source evidence and are not substituted for '
            'the original main or RL interface tree. This is not a ROS1 or x86 native SDK claim.\n',
            'Module/process resource reporting uses explicit smaller nested capacities: two modules, '
            'two channel/RPC entries and four backend names of 64 UTF-8 bytes. Joint state/command '
            'arrays admit 64 joints. All original fields remain present; larger values fail, '
            'and the 32 KiB per-record safety limit remains unchanged.\n']
    if vendor == 'fourier':
        lines += ['## Authored native SDK bridge, not a vendor ROS driver\n',
            'Fourier Aurora publicly documents a native Python client and four named models. '
            'The interface packages under `upstream/fourier_bridge/aurora_bridge_v1/files/ros1` '
            'and `ros2` are our authored business-method envelopes. They are explicitly distinct '
            'from the original Fourier DDS wire schema and any official ROS driver. '
            'Build the selected native ROS interface package independently, source its actual '
            'installation, and pin/qualify the real `fourier_aurora_client` package before '
            'starting `native/bridge.py`. Do not install or execute a downloaded example as a test.\n',
            'The bridge has 23 individually selectable services. It covers the documented '
            'FSM/name, upper-FSM/name, velocity-source/name, stand-pose, group state, Cartesian '
            'state, motor configuration, base/contact data, motion-state/wait, FSM/source '
            'changes, explicit velocity/stand/group/gain commands and composed move recipes. '
            'GetSnapshot adds a complete checked global/configured-group snapshot. '
            'MoveCommandManager construction, opaque native MoveCommand values, client '
            'construction, DDS/QoS selection and client shutdown stay on the operator-owned host.\n',
            '```sh\n# Zero selected methods: exit before importing ROS or the native vendor SDK.\n'
            'python3 "$KIT/native/bridge.py"\n'
            '# After independent native SDK/ROS qualification, select one read method explicitly.\n'
            'python3 "$KIT/native/bridge.py" --backend ros2 --model gr3 --dds-domain 23 \\\n'
            '  --namespace /uwvm/fourier/principal01 --configuration /absolute/operator-profile.json \\\n'
            '  --sdk-version 0.1.8 --allow get_fsm_state\n```\n',
            'The host profile is an ordinary single-link JSON file, at most 64 KiB, with exactly '
            '`model`, `groups`, `limits`, and `motion_complete_states`. The all-denied example '
            '`native/default-denied.json` grants no operation and contains no guessed motion '
            'limits. Each permitted write needs its own explicit scalar/state/group limits. '
            'Configured groups are limited to eight; complete numeric vectors to 64 values. '
            'The native bridge performs no arbitrary method dispatch, Guest path access, '
            'Wasm update, debugger operation or permission change.\n',
            'Each service request carries an increasing per-operation sequence. The bridge '
            'serializes admission and consumes the sequence before invoking the SDK, even when '
            'the result is unknown. Use a separate namespace per principal. Never retry an '
            'unknown physical result; reconcile it with the operator before using a fresh '
            'sequence/namespace. Restart loses the in-memory sequence horizon, so restart also '
            'requires outcome reconciliation and a fresh namespace. Capability close is not '
            'a physical stop. There is no undocumented stop/recovery opcode.\n',
            'WaitGroupsMotionComplete performs bounded read polling (at most five seconds) '
            'with operator-supplied, independently verified completion-state IDs. The bridge '
            'does not guess which numeric state means completion. This bounds its own polling; '
            'the native SDK call implementation and its transport blocking behavior still '
            'require independent qualification. A matching cached state does not prove freshness '
            'or causal completion of a particular command. Snapshot reads are sequential cached '
            'reads, not an atomic sensor frame; the public SDK does not provide a freshness '
            'witness here. ROS deadlines do not undo an applied command.\n',
            'Use the generated service owners for the actual bridge endpoints, for example '
            '`/uwvm/fourier/principal01/get_fsm_state`. The State/VelocityCommand sample pair '
            'demonstrates DTO transport and does not create a bridge topic or bypass the '
            'explicit service selection. Public API compatibility, real SDK return values, '
            'ROS delivery, controller behavior and hardware performance remain unqualified '
            'until their dedicated execution lanes pass.\n']
    return '\n'.join(lines)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', required=True, type=Path)
    p.add_argument('--out', required=True, type=Path)
    a = p.parse_args()
    inputs = json.loads((a.root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    capture = Path(inputs['remaining_capture'])
    discovery = Path(inputs['remaining_discovery'])
    template = (source / 'sdk/origin/robots/humanoids/leju/CMakeLists.txt').read_text()
    a.out.mkdir(mode=0o700)
    results = []
    for vendor, keys in SOURCE_KEYS.items():
        kit = source / 'sdk/origin/robots/platforms' / vendor
        profiles = json.loads((kit / 'profiles.json').read_bytes())['profiles']
        for key in keys:
            target = kit / 'upstream' / key
            if target.exists():
                for path in (capture / 'upstream' / key).rglob('*'):
                    if path.is_file():
                        assert (target / path.relative_to(capture / 'upstream' / key)).read_bytes() == path.read_bytes()
            else:
                shutil.copytree(capture / 'upstream' / key, target)
            if key == 'agibot':
                destination = kit / 'upstream-discovery/agibot-original-sdk-selection'
                if destination.exists():
                    for path in Path(inputs['agibot_capture']).rglob('*'):
                        if path.is_file():
                            assert (destination / path.relative_to(inputs['agibot_capture'])).read_bytes() == path.read_bytes()
                else:
                    shutil.copytree(inputs['agibot_capture'], destination)
            elif (discovery / key).exists() and not (kit / 'upstream-discovery' / key).exists():
                shutil.copytree(discovery / key, kit / 'upstream-discovery' / key)
        if vendor == 'pal' and not (kit / 'upstream/core-source-references').exists():
            shutil.copytree(inputs['remaining_extras'], kit / 'upstream/core-source-references')
        cmake = template.replace('LEJU', vendor.upper()).replace('leju', vendor)
        selections = ' '.join(row['id'] for row in profiles)
        cmake = re.sub(r'(?m)^(set_property\(CACHE UWVM_' + vendor.upper() + r'_PROFILE PROPERTY STRINGS ).*\)$',
                       lambda m: m[1] + selections + ')', cmake)
        cmake = re.sub(r'(?m)^set\(profiles .*\)$', 'set(profiles ' + selections + ')', cmake)
        start = cmake.index('set(source_count 14)')
        end = cmake.index('math(EXPR bound_count', start)
        count_setup = '\n'.join('set(source_count_' + row['id'] + ' ' + ('14' if row['has_protocols'] else '10') + ')' for row in profiles)
        cmake = cmake[:start] + count_setup + '\nset(source_count ${source_count_${UWVM_' + vendor.upper() + '_PROFILE}})\n' + cmake[end:]
        (kit / 'CMakeLists.txt').write_text(cmake)
        (kit / 'README.md').write_text(guide(vendor, profiles))
        results.append(dict(vendor=vendor, profiles=len(profiles), examples_executed=False, default_authority=[]))
    report = dict(status='ENGLISH_GUIDES_ORIGINAL_PROVENANCE_AND_DEFAULT_OFF_CMAKE_AUTHORED_EXECUTION_PENDING',
                  vendors=results, compiler_guest_or_native_runs=0, original_official_cases_run=0, default_authority=[])
    (a.out / 'assembly.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
