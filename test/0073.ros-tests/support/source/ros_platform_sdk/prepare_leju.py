#!/usr/bin/env python3
"""Prepare complete six-language Leju source families from explicit catalogs.

This trusted authoring phase copies exact captured source and invokes the
source-paired industrial emitter. It executes no compiler, Guest, original
installer/controller, native type exporter or ROS operation. Completed source
may be reused only after its full original input/output correspondence passes;
no execution status is adopted. All native binding and permissions stay absent.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def copy_exact(source, destination):
    if destination.exists():
        original = {str(p.relative_to(source)): p for p in source.rglob('*') if p.is_file()}
        current = {str(p.relative_to(destination)): p for p in destination.rglob('*') if p.is_file()}
        assert original.keys() == current.keys()
        assert all(not current[n].is_symlink() and sha(current[n]) == sha(original[n]) for n in original)
    else:
        shutil.copytree(source, destination)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    catalogs = Path(inputs['catalog_source'])
    captured = Path(inputs['captured_sources'])
    core = Path(inputs['core_sources'])
    kit = source / 'sdk/origin/robots/humanoids/leju'
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    kit.mkdir(parents=True, exist_ok=True)
    for original in sorted((captured / 'upstream').iterdir()):
        assert original.is_dir() and not original.is_symlink()
        copy_exact(original, kit / 'upstream' / original.name)
    for name in ('installed', 'geographic'):
        copy_exact(core / name, kit / 'upstream/core' / name)
    copy_exact(catalogs / 'catalogs', kit / 'catalogs')
    profile_bytes = (catalogs / 'profiles.json').read_bytes()
    profile_path = kit / 'profiles.json'
    if profile_path.exists():
        assert profile_path.read_bytes() == profile_bytes
    else:
        profile_path.write_bytes(profile_bytes)
    profiles = json.loads(profile_bytes)['profiles']
    report = dict(status='RUNNING', profiles=[], default_authority=[],
                  native_registry_emitted=False, compiler_guest_or_native_runs=0)
    try:
        for row in profiles:
            key = row['id']
            family = source / 'sdk/origin/custom-types' / ('leju-' + key.replace('_', '-') + '-complete')
            catalog = kit / 'catalogs' / key / 'catalog.json'
            if family.exists():
                preparation = json.loads((family / 'preparation.vendor.json').read_bytes())
                assert preparation['mode'] == 'prepare' and preparation['original_catalog']['sha256'] == sha(catalog)
                assert preparation['max_total_nodes'] == inputs['source_max_total_nodes']
                for name, pin in preparation['original_definitions'].items():
                    assert pin['sha256'] == sha(catalog.parent / name)
                generated = json.loads((family / 'generation.vendor.json').read_bytes())
                for name, pin in generated['outputs'].items():
                    assert (family / name).stat().st_size == pin['bytes'] and sha(family / name) == pin['sha256']
                reused = True
            else:
                prepared = out / ('prepared-' + key)
                argv = [sys.executable, '-I', '-S', '-B', source / 'sdk/origin/robots/industrial/prepare.py',
                        '--source-root', source, '--catalog', catalog, '--out', prepared, '--mode', 'prepare',
                        '--max-total-nodes', str(inputs['source_max_total_nodes'])]
                for name, relative in [('parent', 'sdk/origin/tools/custom_types.py'), ('bounded', 'sdk/origin/custom-types/bounded_profile.py'),
                                       ('adapter', 'sdk/origin/robots/industrial/vendor_profile.py')]:
                    argv += ['--' + name + '-sha256', sha(source / relative)]
                result = subprocess.run(list(map(str, argv)), cwd=source, check=False, timeout=600,
                                        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                (out / (key + '.stdout')).write_bytes(result.stdout)
                (out / (key + '.stderr')).write_bytes(result.stderr)
                assert result.returncode == 0, (key, result.stderr.decode()[-3000:])
                shutil.copytree(prepared / 'generated', family)
                if not (family / 'preparation.vendor.json').exists():
                    shutil.copyfile(prepared / 'preparation.vendor.json', family / 'preparation.vendor.json')
                generated = json.loads((family / 'generation.vendor.json').read_bytes())
                reused = False
            assert not generated['native_registry_emitted'] and generated['default_authority'] == generated['implicit_permissions'] == []
            report['profiles'].append(dict(id=key, records=len(generated['records']),
                                           source_outputs=len(generated['outputs']), reused_exact_source_only=reused))
            (out / 'preparation.json').write_text(json.dumps(report, indent=2) + '\n')
        report['status'] = 'COMPLETE_SELECTED_LEJU_SIX_LANGUAGE_SOURCE_PREPARED_NO_NATIVE_OR_EXECUTION_CREDIT'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (out / 'preparation.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
