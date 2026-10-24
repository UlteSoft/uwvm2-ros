#!/usr/bin/env python3
"""Check AOT source association against genuine captured Host/header bytes.

This is source/data validation only: no .NET SDK, compiler, bitcode, object,
managed method, Wasm, Guest or native ROS execution receives credit.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import types


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if os.uname().sysname != 'Linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('the original admitted Linux owner is required')
    source = args.source.resolve(strict=True)
    names = ('sdk/origin/dotnet/mono-wasi-aot/inspect_aot.py', 'sdk/origin/tools/rmw_service_contract.py',
             'sdk/origin/abi/abi-v1.json', 'sdk/origin/abi/rmw-service-v1.json',
             'sdk/origin/include/roswasm/rmw_service_v1.h',
             'src/uwvm2/uwvm/imported/ros/host.h', 'src/uwvm2/uwvm/imported/ros/rmw_service_host.h')
    args.out.mkdir(parents=True, exist_ok=False)
    fixture = args.out / 'captured-source-binding'
    for name in names:
        target = fixture / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source / name, target)
    path = fixture / names[0]
    spec = importlib.util.spec_from_file_location('_actual_aot_source_adapter', path)
    inspector = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(inspector)
    helper = 'sdk/origin/tools/rmw_service_contract.py'
    raw = (fixture / helper).read_bytes()
    selected = types.SimpleNamespace(source={helper: (raw, None, None)})
    catalog_sha = hashlib.sha256((fixture / 'sdk/origin/abi/rmw-service-v1.json').read_bytes()).hexdigest()
    report = dict(status='STARTED', checks=[], sources=[dict(path=name,
                  sha256=hashlib.sha256((source / name).read_bytes()).hexdigest()) for name in names],
                  aot_compiler_executions=0, compiled_method_credit=0,
                  guest_executions=0, native_ros_executions=0, default_authority=[])

    def refuses(label, callback):
        rejected = False
        try:
            callback()
        except (ValueError, KeyError):
            rejected = True
        if not rejected:
            raise RuntimeError('actual source-binding refusal failed: ' + label)
        report['checks'].append(dict(label=label, rejected=True))

    try:
        catalog, association = inspector.service_source_contract(selected, fixture, catalog_sha)
        if (association['function_count'] != 12 or association['catalog_sha256'] != catalog_sha or
                association['compiler_runs'] != 0 or association['Guest_runs'] != 0):
            raise ValueError('shared source association lost its exact scope')
        report['source_association'] = association
        report['checks'].append(dict(label='actual-captured-Host-implementation-header-association', passed=True))
        refuses('different-caller-catalog', lambda: inspector.service_source_contract(selected, fixture, '0' * 64))
        refuses('missing-admitted-helper', lambda: inspector.service_source_contract(
            types.SimpleNamespace(source={}), fixture, catalog_sha))
        (fixture / helper).write_bytes(raw + b'\n# changed owned negative fixture\n')
        refuses('changed-staged-helper', lambda: inspector.service_source_contract(selected, fixture, catalog_sha))
        (fixture / helper).write_bytes(raw)
        header = fixture / catalog['raw_header']['path']
        original_header = header.read_bytes()
        header.write_bytes(original_header + b'\n/* changed owned header fixture */\n')
        refuses('changed-real-header-association', lambda: inspector.service_source_contract(selected, fixture, catalog_sha))
        header.write_bytes(original_header)
        if any(hashlib.sha256((source / row['path']).read_bytes()).hexdigest() != row['sha256']
               for row in report['sources']):
            raise ValueError('captured original source changed')
        report['status'] = 'ACTUAL_SOURCE_ASSOCIATION_AND_FOUR_REFUSALS_PASS_NO_AOT_PRODUCTS'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (args.out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
