#!/usr/bin/env python3
"""Execute the pinned selected SDK products on the newly built dual-ROS Runtime.

Retained authored value products are restored only after exact checkpoint,
role, extent and content checks. Their old status is not a current pass: every
product executes again under the independent bounded owner. Application cases
verify explicit zero authority; value cases exercise complete independent
original-field references. No native middleware or controller is credited.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import subprocess
import tarfile


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def retained_roles(root, checkpoint, names):
    """Read exact ordinary object bodies as data; never extract archive paths."""
    proof = json.loads(checkpoint.read_bytes())
    client = json.loads((root / 'control/client-readbacks' /
                         (checkpoint.stem + '-client-readback.json')).read_bytes())
    assert proof['status'] == 'TWO_FULL_OBJECT_CONTENT_MODE_AND_GZIP_EOF_READBACKS_PASS'
    assert client['status'] == 'CLIENT_STREAMED_INERT_OBJECTS_MODES_AND_GZIP_EOF_PASS'
    assert all(proof[key] == client[key] for key in ('bytes', 'sha256', 'manifest_sha256'))
    archive_path = Path(proof['primary'])
    assert archive_path.stat().st_size == proof['bytes'] and sha(archive_path) == proof['sha256']
    found = {}
    with archive_path.open('rb') as raw, gzip.GzipFile(fileobj=raw) as zipped:
        with tarfile.open(fileobj=zipped, mode='r|') as archive:
            members = iter(archive)
            first = next(members)
            assert first.name == 'manifest.json' and first.isfile() and first.size <= 16 << 20
            body = archive.extractfile(first).read()
            assert hashlib.sha256(body).hexdigest() == proof['manifest_sha256']
            manifest = json.loads(body)
            assert manifest['original_root'] == str(root)
            selected = {row['path']: row for row in manifest['roles'] if row['path'] in names}
            assert set(selected) == set(names) and all(row['type'] == 'regular' for row in selected.values())
            needed = {}
            for name, row in selected.items():
                needed.setdefault(row['object'], []).append(name)
            for member in members:
                if member.name not in needed:
                    continue
                assert member.isfile() and member.size <= 16 << 20
                data = archive.extractfile(member).read((16 << 20) + 1)
                for name in needed[member.name]:
                    row = selected[name]
                    assert len(data) == row['bytes'] and hashlib.sha256(data).hexdigest() == row['sha256']
                    found[name] = data
                assert sum(len(value) for value in found.values()) <= 128 << 20
            assert set(found) == set(names)
        while zipped.read(1 << 20):
            pass
    assert sha(archive_path) == proof['sha256']
    return found


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    build = Path(inputs.get('runtime_build_result', str(root / 'out/build-r1')))
    built = json.loads((build / 'build.json').read_bytes())
    assert built['status'] == 'FROZEN_CURRENT_SOURCE_FOUR_COMPACT_INTERPRETER_SIZE_BUILDS_PASS'
    runtime = build / 'dual/uwvm.stripped'
    identity = next(row['stripped'] for row in built['profiles'] if row['profile'] == 'dual')
    assert runtime.stat().st_size == identity['bytes'] and sha(runtime) == identity['sha256']
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    (out / 'commands').mkdir()
    (out / 'retained-value-products').mkdir()
    report = dict(status='RUNNING', commands=[], application_products=0, value_products=0,
                  runtime=identity, default_authority=[], original_official_cases_run=0,
                  prior_execution_status_adopted=False, native_delivery_qualified=False)

    def save():
        (out / 'runtimecheck.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(label, artifact, *, argv=(), data=None, expected=0, output=b''):
        directory = out / 'commands' / label
        directory.mkdir()
        command = [runtime, '-Rint', '--wasip1-global-noinherit-system-environment', '--run', artifact, *argv]
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            result = subprocess.run(list(map(str, command)), input=data, stdout=stdout,
                                    stderr=stderr, check=False, timeout=90)
        report['commands'].append(dict(label=label, argv=list(map(str, command)), exit=result.returncode,
                                      expected_exit=expected, stdout_sha256=sha(directory / 'stdout'),
                                      stderr_sha256=sha(directory / 'stderr')))
        save()
        assert result.returncode == expected and (directory / 'stdout').read_bytes() == output, label

    def checked_artifact(row):
        path = Path(row['path'])
        assert path.is_relative_to(root) and path.stat().st_size == row['bytes'] and sha(path) == row['sha256']
        return path

    try:
        vendors = ['deep_robotics', 'pal', 'trossen', 'agibot', 'ubtech', 'fourier']
        for vendor in vendors:
            name = vendor.replace('_', '-')
            direct_name = 'deep' if vendor == 'deep_robotics' else name
            direct = root / 'out' / ('qualify-' + direct_name + ('-r2' if vendor == 'agibot' else '-r1'))
            qualification = json.loads((direct / 'qualification.json').read_bytes())
            assert qualification['status'] == 'SELECTED_VENDOR_C_CPP_COMPLETE_DATA_AND_DEFAULT_REFUSAL_PASS'
            for row in qualification['artifacts']:
                if not row['label'].endswith(('-c-app', '-cpp-app')):
                    continue
                artifact = checked_artifact(row)
                for mode in ['raw', 'state', 'command']:
                    run(vendor + '-' + row['label'] + '-' + mode, artifact,
                        argv=[mode, vendor + '.observation'], expected=13)
                report['application_products'] += 1
            managed = json.loads((root / 'out' / ('managed-' + name + '-r1') / 'managed.json').read_bytes())
            assert managed['status'] == 'SELECTED_VENDOR_FOUR_MANAGED_LANGUAGE_PRODUCTS_AND_EXACT_REFUSALS_PASS'
            alias = (vendor + '.observation').encode()
            for row in managed['artifacts']:
                artifact = checked_artifact(row)
                language = row['label'].rsplit('-', 1)[1]
                for mode in [0, 1]:
                    label = vendor + '-' + row['label'] + '-' + str(mode)
                    if language in ('rust', 'tinygo'):
                        run(label, artifact, argv=['state' if mode == 0 else 'command', alias.decode()], expected=13)
                    else:
                        run(label, artifact, data=bytes([mode, len(alias)]) + alias,
                            expected=0 if language == 'javascript' else 13,
                            output=b'\r\0\0\0\0\0\0\0' if language == 'javascript' else b'')
                report['application_products'] += 1
        primary = Path('/home/macromodel/Documents/uwvm-test-261018/recovery/ros-remaining-r60')
        for checkpoint, phases in [
            ('managed-values-first-three-r1', ['values-agibot-r1', 'values-pal-r1', 'values-fourier-r6']),
            ('managed-values-other-three-r1', ['values-deep-robotics-r1', 'values-ubtech-r1', 'values-trossen-r1'])]:
            proof = primary / (checkpoint + '.json')
            metadata = retained_roles(root, proof, ['out/' + phase + '/values.json' for phase in phases])
            artifacts = []
            for body in metadata.values():
                # This is identity metadata only. Old statuses are not used to
                # pass a product: its exact bytes execute below on the new VM.
                artifacts += json.loads(body)['artifacts']
            names = [str(Path(row['path']).relative_to(root)) for row in artifacts]
            objects = retained_roles(root, proof, names)
            for index, row in enumerate(artifacts):
                body = objects[str(Path(row['path']).relative_to(root))]
                assert len(body) == row['bytes'] and hashlib.sha256(body).hexdigest() == row['sha256']
                artifact = out / 'retained-value-products' / (checkpoint + '-' + str(index) + '.wasm')
                artifact.write_bytes(body)
                run('value-' + str(report['value_products']), artifact)
                report['value_products'] += 1
        assert report['application_products'] == 156 and report['value_products'] == 104
        assert runtime.stat().st_size == identity['bytes'] and sha(runtime) == identity['sha256']
        report['status'] = 'CURRENT_COMPACT_DUAL_RUNTIME_156_APPLICATIONS_104_VALUE_PRODUCTS_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
