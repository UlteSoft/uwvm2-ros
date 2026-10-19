#!/usr/bin/env python3
"""Static source/schema readiness for the complete JavaScript SDK.

This reader parses JSON metadata, Python AST and literal source inventories.
It neither executes JavaScript/library models nor compiles/validates/runs Wasm,
ROS, native code or a toolchain. Its output is SOURCE_CHECKED_ONLY, never a
delivery/authentication receipt. Run with Python -B and bytecode writing disabled;
save a fresh receipt exclusively under /tmp. Historical evidence stays intact.
"""
import argparse
import ast
import hashlib
import json
import pathlib
import re
import sys

sys.dont_write_bytecode = True
ROOT = pathlib.Path(__file__).resolve().parents[2]
checks = []


def require(condition, label):
    if not condition:
        raise ValueError(label)
    checks.append(label)


def read(relative):
    return (ROOT / relative).read_text(encoding='utf-8')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', required=True)
    args = parser.parse_args()
    out = pathlib.Path(args.out)
    require(out.is_absolute() and str(out).startswith('/tmp/') and '..' not in out.parts,
            'fresh absolute temporary receipt')
    require(not out.exists(), 'never replace prior receipt')
    ast.parse(pathlib.Path(__file__).read_text(), filename=__file__)
    require(True, 'Python source-reader AST')
    schema = json.loads(read('sdk/abi/abi-v1.json'))
    abi_source = read('sdk/javascript/abi.mjs')
    inventory = json.loads(abi_source.split('const inventory = ', 1)[1].split(';', 1)[0])
    protocol = read('sdk/javascript/relay_protocol.h')
    relay = read('sdk/javascript/relay.c')
    transport = read('sdk/javascript/relay.mjs')
    bridge = read('sdk/javascript/bridge.c')
    signatures = {name: re.findall(r'uint(32|64)_t', params)
                  for name, params in re.findall(r'uint32_t roswasm_forward_(\w+)\(([^)]*)\)', bridge)}
    for version, count in [(1, 38), (2, 43)]:
        functions = schema['modules'][f'uwvm_ros{version}_v1']['functions']
        meta = inventory[str(version)]
        require(len(functions) == count and len(meta['functions']) == count,
                f'ROS{version} exact selected count')
        require([(f['name'], f['params']) for f in functions] ==
                [(f['name'], f['parameters']) for f in meta['functions']],
                f'ROS{version} immutable JS table matches authoritative schema')
        part = protocol.split('#if ROSWASM_BRIDGE_ROS_VERSION == 1', 1)[1].split('#elif ROSWASM_BRIDGE_ROS_VERSION == 2', 1)[0] if version == 1 else protocol.split('#elif ROSWASM_BRIDGE_ROS_VERSION == 2', 1)[1].split('#else', 1)[0]
        rows = re.findall(r'X\((\d+), (\w+), (\w+), (\d+), (\d+)u\)', part)
        require(len(rows) == count, f'ROS{version} closed C dispatch rows')
        for index, (function, row) in enumerate(zip(functions, rows)):
            params = function['params']
            mask = sum(1 << n for n, p in enumerate(params) if p['type'] == 'i64')
            expected_form = {'version': 'VERSION', 'open': 'OPEN', 'handle': 'HANDLE',
                             'publish': 'WRITE', 'poll': 'POLL', 'rpc': 'RPC',
                             'watch': 'WATCH', 'ready': 'READY', 'submit': 'SUBMIT'}.get(function['form'])
            # The schema's write form is named payload; classify it explicitly
            # by its four typed arguments, never infer a novel Runtime entry.
            if expected_form is None and len(params) == 4 and function['name'] != 'capability_open':
                expected_form = 'WRITE'
            require(row == (str(index), function['name'], expected_form, str(len(params)), str(mask)),
                    f'ROS{version}/{function["name"]} exact index/form/arity/i64 mask')
            require(signatures.get(function['name']) == ['64' if p['type'] == 'i64' else '32' for p in params],
                    f'ROS{version}/{function["name"]} original C forwarder typed signature')
            require(function['results'] == ['i32'], f'ROS{version}/{function["name"]} exact result type')
        canonical = {'abi_version': 1, 'module': meta['module'], 'functions': [
            {'name': f['name'], 'parameters': [p['type'] for p in f['params']], 'result': 'i32'} for f in functions]}
        digest = hashlib.sha256(json.dumps(canonical, sort_keys=True, separators=(',', ':')).encode()).hexdigest()
        constants = re.search(r'uwvm_js_relay_table_digest\[32\] = \{([^}]+)\}', part).group(1)
        require(bytes(int(x, 16) for x in re.findall(r'0x[0-9a-f]{2}', constants)).hex() == digest,
                f'ROS{version} C digest matches canonical table')
        require(digest in transport, f'ROS{version} transport digest matches canonical table')
    imports = re.findall(r'import_module\("([^"]+)"\), import_name\("([^"]+)"\)', relay)
    require(imports == [('wasi_snapshot_preview1', name) for name in ['fd_read', 'fd_write', 'proc_exit']],
            'exact three extra WASIp1 imports')
    require('#include "bridge.c"' in relay and '#include "relay_protocol.h"' in relay,
            'real original forwarders retained; no host ROS substitute')
    for form in ['VERSION', 'OPEN', 'HANDLE', 'WRITE', 'POLL', 'RPC', 'WATCH', 'READY', 'SUBMIT']:
        require(f'#define UWVM_JS_INVOKE_{form}' in relay, f'complete dispatch form {form}')
    for literal in ['actual>length-consumed', 'interruptions>UWVM_JS_RELAY_MAX_INTERRUPTS',
                    'sequence!=uwvm_js_sequence+1', 'input_bytes>UWVM_JS_INPUT_BYTES',
                    'snapshot>UWVM_JS_OUTPUT_BYTES', 'if(!uwvm_js_guards())']:
        require(literal in relay, 'C bounded/source guard ' + literal)
    require('pending.responded=true;this.state=\'settling\'' in transport and
            'Promise.all([outgoing,answer])' in transport, 'borrow covers write completion and copied result')
    sync, async_source = read('sdk/javascript/roswasm.mjs'), read('sdk/javascript/async.mjs')
    families = ['Session', 'Capability', 'Publisher', 'Subscription', 'Executor', 'Client',
                'Service', 'ActionClient', 'ActionServer', 'Resource', 'Parameters', 'Graph',
                'Clock', 'Tf', 'Lifecycle', 'Logger', 'Node', 'Context']
    for family in families:
        require(f'export class {family}' in sync, 'sync public class ' + family)
        require(f'export class Async{family}' in async_source, 'async public class ' + family)
    factories = ['Publisher', 'Subscription', 'Client', 'Service', 'ActionClient', 'ActionServer',
                 'Parameters', 'Graph', 'Clock', 'Tf', 'Lifecycle', 'Resource', 'Logger']
    for factory in factories:
        require(f'create{factory}(' in sync and f'create{factory}(' in async_source,
                'both Node factory surfaces ' + factory)
    scheduling = read('sdk/javascript/scheduling.mjs')
    for concept in ['Future', 'CallbackGroup', 'Timer', 'AsyncTimer', 'DeferredQueue', 'ParameterCallbacks']:
        require(f'export class {concept}' in scheduling, 'caller-driven local concept ' + concept)
    require('setTimeout(' not in scheduling and 'setInterval(' not in scheduling and 'Date.now(' not in scheduling,
            'local scheduling creates no hidden clock/timer')
    require("this.active.filter(task=>task.future.state==='pending').length" in scheduling and 'waiting>=this.maxPending' in scheduling and 'this.tasks.splice(0,this.maxTasksPerPass)' in scheduling,
            'bounded pending queue and per-pass snapshot')
    api_cases = re.findall(r"^test\('([^']+)'", read('tests/ros_sdk_relay/javascript_api_models.mjs'), re.M)
    require(len(api_cases) == 54 and len(set(api_cases)) == 54, 'exact54 unique serial API model cases; prior36 retained')
    ownership = read('sdk/javascript/ownership.mjs')
    require('new WeakSet()' in ownership and 'selected.delete(owner)' in ownership and
            'selected.add(owner)' in ownership, 'private local acquire/release lifetime sets')
    require('for (const owner of owners) selected.add(owner)' in ownership and
            'if (owners.some(owner => polling.has(owner) || retiring.has(owner))) return null' in ownership,
            'whole local group prechecks before any lifetime mutation')
    require(sync.count('finally { release(); this.spinning = false; }') == 3 and
            async_source.count('finally {release();this.spinning=false;}') == 3,
            'all six direct poll guards release shared owner lease in finally')
    require(sync.count('value.goalId === goalId') == 2 and async_source.count('value.goalId===goalId') == 2,
            'four typed feedback/result paths require exact requested ID')
    require(sync.count('runtimeStatus: r.status') == 2 and async_source.count('runtimeStatus:r.status') == 2,
            'local ID refusal preserves actual Runtime status separately')
    require('ownersBusy(node.owned)' in sync and 'node.busy||ownersBusy(node.owned)' in async_source,
            'both Context shutdown paths precheck shared owner borrows')
    require('acquireRetirement(owners)' in sync and 'acquireRetirement(owners)' in async_source,
            'both cleanup groups exclude a fresh poll throughout checked cleanup')
    require('callback.call(this' in sync and 'callback.call(this' in async_source and
            'handler.call(this' in sync and 'handler.call(this' in async_source,
            'callable snapshot preserves the existing resource this receiver')
    require('ownerPolling(cap) && !leased' in sync and 'ownerPolling(cap)&&!leased' in async_source and
            'pollCapability(capability, true)' in sync and 'pollCapability(advertise, true)' in sync and
            'pollCapability(capability,capacity,true)' in async_source and 'pollCapability(advertise,capacity,true)' in async_source,
            'public owner polling refuses a retained borrow; all six typed paths use private leased poll')
    package = json.loads(read('sdk/javascript/package.json'))
    for key in ['./async', './relay', './scheduling']:
        require(key in package['exports'], 'additive package export ' + key)
    for name in ['plan-empty.json', 'plan-ros1-resources.json', 'plan-ros2-resources.json']:
        plan = json.loads(read('sdk/examples/javascript/' + name))
        require(plan['lesson'] == 'application' and len(plan['actions']) <= 64 and plan['steps'] <= 200,
                'bounded example plan ' + name)
    empty = json.loads(read('sdk/examples/javascript/plan-empty.json'))
    require(empty['aliases'] == {} and empty['actions'] == [] and empty['steps'] == 0,
            'empty application plan opens no ROS authority')
    runner = read('sdk/examples/javascript/run.mjs')
    require('fromGuestBridge(version,await setup.createAdmittedInstance(version))' in runner and
            'new WebAssembly.Memory' not in runner and 'WebAssembly.instantiate' not in runner,
            'runnable sync entry receives real admitted Instance; no fake setup')
    require('fd:3' in runner and 'fd:4' in runner and 'spawn(' not in runner and 'process.env' not in runner,
            'runnable async entry attaches explicit streams; no process/env authority')
    wasm_runner = read('sdk/javascript/tests/run_pure_wasm.mjs')
    expected_text = re.search(r'export const expectedChecks = Object.freeze\(\{(.*?)\}\);', wasm_runner, re.S).group(1)
    expected = {name: None if value == 'null' else int(value)
                for name, value in re.findall(r'^\s+(\w+): (\d+|null),', expected_text, re.M)}
    require(list(expected) == ['codec_vectors', 'adapter_contracts', 'executor_mutation', 'factory_transactions',
                              'resource_roles', 'unicode_strings', 'highlevel_coverage', 'local_schedule', 'server_callbacks', 'unified_events'],
            'exact nine AS pure fixture names without aliases/skips')
    require(expected['factory_transactions'] == 75 and expected['local_schedule'] == 47 and expected['server_callbacks'] == 60 and expected['unified_events'] == 33,
            'AS prepared count pins75/47/60/33; old413 assertions retained')
    require(sum(value for value in expected.values() if value is not None) == 446,
            'AS numeric assertion total446 excludes semantic-only coverage')
    options_source = read('sdk/javascript/tests/run_pure_wasm_options.mjs')
    require('expectedChecks.server_callbacks,60' in options_source and
            'expectedChecks.unified_events,33' in options_source and 'reduce((a, b) => a + b, 0), 446' in options_source,
            'AS option assertions pin current60/33 and aggregate446')
    checked_example = read('sdk/examples/javascript/05-runtime-relay.mjs')
    for marker in ['SDK_ENDPOINTS_CREATED', 'SDK_FINAL_SENT', 'SDK_POLL_AGAIN', 'SDK_DELIVERED', 'PASS_SDK_DELIVERY', 'PASS_SDK_DEFAULT_DENIED']:
        require(marker in checked_example, 'checked delivery protocol marker ' + marker)
    sources = sorted(str(p.relative_to(ROOT)) for parent in ['sdk/javascript', 'sdk/examples/javascript']
                     for p in (ROOT / parent).iterdir() if p.is_file())
    sources += ['tests/ros_sdk_relay/javascript_protocol_models.mjs',
                'tests/ros_sdk_relay/javascript_api_models.mjs', str(pathlib.Path(__file__).relative_to(ROOT)),
                'sdk/abi/abi-v1.json', 'sdk/javascript/tests/run_pure_wasm.mjs',
                'sdk/javascript/tests/run_pure_wasm_options.mjs']
    pins = [{'path': path, 'bytes': (ROOT / path).stat().st_size,
             'sha256': hashlib.sha256((ROOT / path).read_bytes()).hexdigest()} for path in sources]
    result = {'format_version': 1, 'status': 'SOURCE_CHECKED_ONLY', 'checks': len(checks),
              'scope': 'Static source/schema/JSON/Python-AST only; no JavaScript library, compiler, validator, Runtime, Wasm, native or ROS execution.',
              'runtime_tools_executed': [], 'actual_native_or_Guest_execution': False,
              'pins': pins, 'checks_passed': checks,
              'pending_owner_serial_tests': [
                  {'entry': 'tests/ros_sdk_relay/javascript_protocol_models.mjs', 'historical_expected_checks': 824},
                  {'entry': 'tests/ros_sdk_relay/javascript_api_models.mjs', 'expected_cases': 54},
                  {'entry': 'sdk/javascript/tests/test.mjs', 'scope': 'existing synchronous regressions'},
                  {'entry': 'sdk/javascript/tests/factory_transactions.mjs', 'scope': 'existing factory regressions'},
                  {'entry': 'sdk/javascript/tests/executor_contracts.mjs', 'scope': 'existing executor regressions'}]}
    with out.open('x', encoding='utf-8') as stream:
        json.dump(result, stream, indent=2)
        stream.write('\n')
    print(json.dumps({'status': result['status'], 'checks': len(checks), 'receipt': str(out),
                      'sha256': hashlib.sha256(out.read_bytes()).hexdigest()}))


if __name__ == '__main__':
    main()
