#!/usr/bin/env python3
"""Prepare data-only inputs for a fresh finite Python native TF owner.

This selected recovery profile preserves original source/product pins, requires
the original ROS Runtime bytes and copies pinned source as data. It neither
runs a compiler/loader/Guest nor grants native execution or SDK parity credit.
Historical dependency pins stay recorded separately from active execution pins.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import os
import stat


ORIGINAL = Path('/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1')
PYTHON_COHORT = ORIGINAL / 'root-language-python-transform-20261020-a17'
HELPER_SHA256 = '09940a0f6395fdbb13563d533384af1edc85d87e5d8bd1ea0145b1157432e42d'


def observed(path, tick=lambda: None):
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW)
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or before.st_size > 512 << 20:
            raise ValueError('bounded ordinary input file required')
        digest = hashlib.sha256()
        while chunk := os.read(fd, 65536):
            tick()
            digest.update(chunk)
        after = os.fstat(fd)
        identity = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns, s.st_ctime_ns)
        if identity(before) != identity(after):
            raise ValueError('input changed while hashing')
        return dict(path=str(path), bytes=before.st_size, sha256=digest.hexdigest())
    finally:
        os.close(fd)


def expect(path, reference, tick):
    row = observed(path, tick)
    if any(row[key] != reference[key] for key in ('bytes', 'sha256')):
        raise ValueError('original file pin mismatch: ' + str(path))
    return row


def inside(root, path):
    path = path.resolve(strict=True)
    if not path.is_relative_to(root):
        raise ValueError('recovered input escapes selected root')
    return path


def prepare(source, native_root, runtimes_path, loader_report, driver, out, tick=lambda: None):
    source = source.resolve(strict=True)
    native_root = native_root.resolve(strict=True)
    driver = driver.resolve(strict=True)
    out = out.absolute()
    out.mkdir(mode=0o700)  # A partial failed preparation is never overwritten.
    for name in ('tmp', 'ros-logs'):
        (out / name).mkdir(mode=0o700)
    old_inputs = json.loads((source / 'native-inputs-r2.json').read_bytes())
    old_products = json.loads((source / 'native-products-r1.json').read_bytes())
    if old_products['status'] != 'PYTHON_TRANSFORM_PRODUCTS_AND_PINNED_NATIVE_WITNESSES_SELECTED':
        raise ValueError('expected the frozen Python Transform producer')
    restored = json.loads(runtimes_path.read_bytes())
    runtime_rows = {row['sha256']: row for row in restored['runtimes']}
    diagnostic = json.loads(loader_report.read_bytes())
    if (diagnostic['status'] != 'FIVE_PINNED_NATIVE_LOADER_LIST_DIAGNOSTICS_MATCH_EXPLICIT_STATIC_CLOSURES_NO_APPLICATION_ENTRY'
            or len(diagnostic['commands']) != 5 or not all(row['passed'] for row in diagnostic['commands'])):
        raise ValueError('five qualified loader-list diagnostics required')
    copies = {row['original']['sha256']: row['copy'] for row in diagnostic['loaders']}
    active = {}

    def add(row):
        previous = active.setdefault(row['path'], row)
        if previous != row:
            raise ValueError('conflicting active input pins')

    def write(name, value):
        path = out / name
        path.parent.mkdir(parents=True, exist_ok=True)
        with path.open('x') as stream:
            json.dump(value, stream, indent=2)
            stream.write('\n')
        return path

    copied_source = []
    for old in old_inputs['pins']:
        path = Path(old['path'])
        if not path.is_relative_to(PYTHON_COHORT / 'source-r2'):
            continue
        relative = path.relative_to(PYTHON_COHORT)
        original = inside(source, source / relative)
        expect(original, old, tick)
        target = out / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        with original.open('rb') as src, target.open('xb') as dst:
            while chunk := src.read(65536):
                tick()
                dst.write(chunk)
        add(expect(target, old, tick))
        copied_source.append(str(relative))
    if len(copied_source) != 149:
        raise ValueError('the selected producer requires149 original source roles')
    current_driver = out / 'native-controller-r1.py'
    current_driver.write_bytes(driver.read_bytes())
    add(observed(current_driver, tick))
    products = copy.deepcopy(old_products)
    for app in products['products']:
        original = app['path']
        app['path'] = str(inside(source, source / Path(original).relative_to(PYTHON_COHORT)))
        add(expect(Path(app['path']), app, tick))
        app['original_path'] = original
    if len(products['products']) != 4:
        raise ValueError('four original ROS Python Wasm products required')
    for backend, native in products['native_backends'].items():
        native['native_SDK_root'] = str(inside(native_root, native_root / Path(native['native_SDK_root']).relative_to(ORIGINAL)))
        native['native_library_path'] = ':'.join(str(inside(native_root, native_root / Path(p).relative_to(ORIGINAL)))
                                                 for p in native['native_library_path'].split(':'))
        principals = [native[name] for name in ('native_gateway', 'native_loader', 'peer')]
        if 'master' in native:
            principals += [row for row in native['master'].values() if isinstance(row, dict) and 'path' in row]
        for row in principals:
            old_path = row['path']
            recovered = inside(native_root, native_root / Path(old_path).relative_to(ORIGINAL))
            add(expect(recovered, row, tick))
            row['path'] = str(recovered)
            row['original_path'] = old_path
        loader = native['native_loader']
        copied = copies[loader['sha256']]
        add(expect(Path(copied['path']), loader, tick))
        loader['path'] = copied['path']
        native['historical_build_dependencies_not_reconstructed'] = True
    inputs = {key: copy.deepcopy(value) for key, value in old_inputs.items()
              if key not in ('pins', 'runtimes', 'controller')}
    inputs.update(attempt='r1', source_directory='source-r2', producer_result='native-products-r1.json',
                  policy_path='source-r2/sdk/origin/tools/tf_policy.py',
                  owned_helper='source-r2/test/0050.high_level_languages/owned_process.py',
                  ROS1_extra_library_root=str(inside(native_root, native_root / Path(old_inputs['ROS1_extra_library_root']).relative_to(ORIGINAL))))
    inputs['runtimes'] = []
    for row in old_inputs['runtimes']:
        runtime = copy.deepcopy(row)
        runtime['original_path'] = runtime['path']
        recovered = runtime_rows[runtime['sha256']]
        runtime['path'] = recovered['path']
        add(expect(Path(runtime['path']), runtime, tick))
        inputs['runtimes'].append(runtime)
    helper = next(row for row in active.values() if row['path'].endswith('/owned_process.py'))
    if helper['sha256'] != HELPER_SHA256:
        raise ValueError('owned-process helper differs')
    products_path = write('native-products-r1.json', products)
    add(observed(products_path, tick))
    inputs['controller'] = observed(current_driver, tick)
    inputs['pins'] = list(active.values())
    write('native-inputs-r1.json', inputs)
    write('historical-input-association.json', dict(original_inputs=old_inputs, original_products=old_products))
    result = dict(status='RECOVERED_PYTHON_TF_OWNER_INPUTS_PREPARED_AS_DATA_NO_EXECUTION',
                  source_roles=149, active_pins=len(active), runtime_products=len(inputs['runtimes']),
                  Wasm_products=4, out=str(out), application_entry_executions=0,
                  native_delivery_credit=False, execution_ready=False,
                  remaining='Actual Python imports, dynamic plugin/resource discovery, mapped paths and32 native scenes require separate admission')
    write('preparation-result-r1.json', result)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'native-root', 'runtime-report', 'loader-report', 'driver', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    result = prepare(args.source, args.native_root, args.runtime_report, args.loader_report, args.driver, args.out)
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
