#!/usr/bin/env python3
"""Check actual file-closure policy and refusal of the genuine wrong SDK.

Filesystem fixtures below are inert data and do not represent Python products.
No interpreter producer, freezer, native application, Wasm or ROS actor runs.
"""

import argparse
import copy
import importlib.util
import json
import os
from pathlib import Path
import sys


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--wrong-sdk', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('the original admitted Linux owner is required')
    sdk = args.source.resolve(strict=True) / 'sdk/origin'
    paths = [sdk / 'python/tools/produce.py', sdk / 'python/tools/build.py',
             sdk / 'tools/xmake/examples/managed_backend.py']
    producer, consumer, backend = [load('_closure_' + str(index), path) for index, path in enumerate(paths)]
    sources = [producer._pin(path) for path in paths]
    args.out.mkdir(parents=True, exist_ok=False)
    report = dict(status='STARTED', sources=sources, checks=[],
                  actual_python_producer_products=0, compiler_executions=0,
                  guest_executions=0, native_ros_executions=0, default_authority=[])

    def check(label, callback, accepts=True):
        accepted = True
        try:
            callback()
        except ValueError:
            accepted = False
        if accepted is not accepts:
            raise RuntimeError('file-provenance policy outcome differs: ' + label)
        report['checks'].append(dict(label=label, accepted=accepted, passed=True))

    try:
        roots = [args.out / name for name in ('data-source', 'data-sdk', 'data-home')]
        for root in roots:
            root.mkdir()
        source, wasi, native_home = roots
        (source / 'target').mkdir()
        for root, text in zip(roots, (b'not an interpreter\n', b'not a compiler\n', b'not a stdlib\n')):
            (root / 'data.bin').write_bytes(text)
        (source / 'linked-data.bin').symlink_to(wasi / 'data.bin')
        inventory = producer._product_inventory(roots)
        if (len(inventory['files']) != 3 or len(inventory['links']) != 1 or
                inventory['runtime_qualification_credit'] != 0):
            raise RuntimeError('complete data inventory lost a canonical file or symbolic role')
        report['checks'].append(dict(label='actual-deduplicated-files-and-confined-link', passed=True))
        pins = {row['path']: row for row in inventory['files']}
        data = dict(source=str(source), target=str(source / 'target'), wasi_sdk=str(wasi),
                    native_python_home=str(native_home), native_python=str(source / 'data.bin'),
                    files=inventory['files'])
        check('all-indirect-data-roles-covered', lambda: backend.python_product_closure(data, pins, roots))
        external = args.out / 'unselected-data'
        external.mkdir()
        for role in ('source', 'target', 'wasi_sdk', 'native_python_home'):
            changed = dict(data, **{role: str(external)})
            check('unselected-' + role, lambda changed=changed: backend.python_product_closure(changed, pins, roots), False)
        changed = dict(data, source='relative-data')
        check('relative-indirect-role', lambda: backend.python_product_closure(changed, pins, roots), False)
        missing = dict(pins)
        del missing[str((source / 'data.bin').resolve())]
        check('unlisted-native-role', lambda: backend.python_product_closure(data, missing, roots), False)
        changed = copy.deepcopy(data)
        changed['files'][0]['bytes'] += 1
        check('changed-declared-product-pin', lambda: backend.python_product_closure(changed, pins, roots), False)
        changed = dict(data, files='not-a-file-pin-array')
        check('nonarray-product-file-pins', lambda: backend.python_product_closure(changed, pins, roots), False)
        complete = args.out / 'complete-data-manifest.json'
        complete.write_text(json.dumps(inventory) + '\n')
        changed = dict(data, complete_product_files=producer._pin(complete))
        check('unlisted-complete-manifest', lambda: backend.python_product_closure(changed, pins, roots), False)
        with_manifest = dict(pins)
        with_manifest[str(complete)] = producer._pin(complete)
        check('independently-pinned-complete-manifest',
              lambda: backend.python_product_closure(changed, with_manifest, roots))
        (source / 'escape').symlink_to(external, target_is_directory=True)
        check('escaping-produced-link', lambda: producer._product_inventory(roots), False)
        (source / 'escape').unlink()
        (source / 'data.bin').write_bytes(b'changed owned data\n')
        check('changed-complete-offline-tree',
              lambda: backend.validate_closure(dict(files=list(pins.values()), directories=list(map(str, roots)))), False)
        wrong = consumer.pin(args.wrong_sdk / 'bin/clang')
        if wrong['sha256'] == consumer._SDK24_CLANG:
            raise ValueError('negative control requires the real different compiler, not SDK24')
        check('genuine-other-sdk-refused-before-tool-execution', lambda: consumer.sdk24_compiler(args.wrong_sdk), False)
        report['actual_wrong_compiler'] = wrong
        if any(producer._pin(Path(row['path'])) != row for row in sources):
            raise ValueError('captured production source changed')
        report['status'] = 'FILE_PROVENANCE_POLICY_AND_GENUINE_WRONG_SDK_REFUSAL_PASS_NO_PRODUCTS'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (args.out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
