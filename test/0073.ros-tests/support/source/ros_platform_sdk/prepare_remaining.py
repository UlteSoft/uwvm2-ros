#!/usr/bin/env python3
"""Implement selected source SDKs and six-language applications before testing.

This Linux authoring phase runs only the paired source emitter and documented
facade authors. No compiler, Guest, native driver, simulator or upstream
installer executes. Every output remains unqualified until the subsequent
unified execution lanes independently build and exercise it.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    catalog_root = Path(inputs['remaining_catalogs'])
    catalogs = json.loads((catalog_root / 'catalogs.json').read_bytes())
    assert catalogs['status'].startswith('SEPARATE_EXACT_ROS_SOURCE_CATALOGS_AUTHORED_')
    out.mkdir(mode=0o700)
    report = dict(status='RUNNING', commands=[], profiles=[], default_authority=[],
                  compiler_guest_or_native_runs=0, original_official_cases_run=0)
    def run(label, argv):
        result = subprocess.run(list(map(str, argv)), cwd=source, timeout=600,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=False)
        (out / (label + '.stdout')).write_bytes(result.stdout)
        (out / (label + '.stderr')).write_bytes(result.stderr)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=result.returncode))
        (out / 'preparation.json').write_text(json.dumps(report, indent=2) + '\n')
        assert result.returncode == 0, (label, result.stderr.decode()[-3000:])
    try:
        for vendor in sorted({r['vendor'] for r in catalogs['profiles']}):
            kit = source / 'sdk/origin/robots/platforms' / vendor
            original_kit = catalog_root / 'sdk/origin/robots/platforms' / vendor
            if kit.exists() and not (kit / 'profiles.json').exists():
                # A newly authored native bridge can already contain its host
                # implementation/specification. Add only the fresh catalog
                # paths; retain and never overwrite those authored sources.
                for path in original_kit.rglob('*'):
                    if path.is_file():
                        destination = kit / path.relative_to(original_kit)
                        destination.parent.mkdir(parents=True, exist_ok=True)
                        assert not destination.exists()
                        destination.write_bytes(path.read_bytes())
            changed = kit.exists() and any(p.is_file() and
                (not (kit / p.relative_to(original_kit)).is_file() or
                 (kit / p.relative_to(original_kit)).read_bytes() != p.read_bytes()) for p in original_kit.rglob('*'))
            if changed:
                assert vendor in inputs.get('replace_untested_source_drafts', []), 'Changed draft source needs explicit authoring selection'
                history = out / ('prior-untested-' + vendor)
                history.mkdir()
                old_profiles = json.loads((kit / 'profiles.json').read_bytes())['profiles']
                kit.rename(history / 'kit')
                for row in old_profiles:
                    prior_family = source / 'sdk/origin/custom-types' / row['family']
                    if prior_family.exists():
                        prior_family.rename(history / row['family'])
                report.setdefault('retained_replaced_untested_source', []).append(dict(vendor=vendor, history=str(history)))
            if not kit.exists():
                shutil.copytree(original_kit, kit)
            else:
                for path in (catalog_root / 'sdk/origin/robots/platforms' / vendor).rglob('*'):
                    if path.is_file():
                        assert (kit / path.relative_to(catalog_root / 'sdk/origin/robots/platforms' / vendor)).read_bytes() == path.read_bytes()
            for row in [r for r in catalogs['profiles'] if r['vendor'] == vendor]:
                key = row['id']
                family = source / 'sdk/origin/custom-types' / row['family']
                catalog = kit / 'catalogs' / key / 'catalog.json'
                if family.exists():
                    prep = json.loads((family / 'preparation.vendor.json').read_bytes())
                    assert prep['mode'] == 'prepare' and prep['original_catalog']['sha256'] == sha(catalog)
                    for name, pin in prep['original_definitions'].items():
                        assert sha(catalog.parent / name) == pin['sha256']
                    gen = json.loads((family / 'generation.vendor.json').read_bytes())
                    for name, pin in gen['outputs'].items():
                        assert (family / name).stat().st_size == pin['bytes'] and sha(family / name) == pin['sha256']
                else:
                    prepared = out / (vendor + '-' + key)
                    argv = [sys.executable, '-I', '-S', '-B', source / 'sdk/origin/robots/industrial/prepare.py',
                        '--source-root', source, '--catalog', catalog, '--out', prepared,
                        '--mode', 'prepare', '--max-total-nodes', str(row['source_max_total_nodes'])]
                    for name, relative in [('parent', 'sdk/origin/tools/custom_types.py'), ('bounded', 'sdk/origin/custom-types/bounded_profile.py'),
                                           ('adapter', 'sdk/origin/robots/industrial/vendor_profile.py')]:
                        argv += ['--' + name + '-sha256', sha(source / relative)]
                    run(vendor + '-' + key, argv)
                    shutil.copytree(prepared / 'generated', family)
                    shutil.copyfile(prepared / 'preparation.vendor.json', family / 'preparation.vendor.json')
                    gen = json.loads((family / 'generation.vendor.json').read_bytes())
                assert not gen['native_registry_emitted'] and gen['default_authority'] == gen['implicit_permissions'] == []
                selector = source / 'sdk/origin/robots/humanoids/leju/tools/select_profile.py'
                run(vendor + '-' + key + '-associate', [sys.executable, '-I', '-S', '-B', selector,
                    '--kit', kit, '--bundle', family, '--profile', key] +
                    (['--check-existing'] if (family / 'profile-selection.json').exists() else []))
                report['profiles'].append(dict(vendor=vendor, profile=key, records=len(gen['records']),
                                              generated_sources=len(gen['outputs']), executed=False))
                print(json.dumps(report['profiles'][-1]), flush=True)
            for tool in ('emit_examples.py', 'emit_contracts.py'):
                directory = out / (vendor + '-' + tool.removesuffix('.py'))
                argv = [sys.executable, '-I', '-S', '-B', source / 'sdk/origin/robots/humanoids/leju/tools' / tool,
                        '--source', source, '--out', directory, '--vendor', vendor,
                        '--kit-relative', str(kit.relative_to(source))]
                if tool == 'emit_examples.py':
                    argv += ['--display', next(r['display'] for r in catalogs['profiles'] if r['vendor'] == vendor)]
                run(vendor + '-' + tool, argv)
                target = kit / 'tests' if tool == 'emit_contracts.py' else kit
                for path in sorted(directory.rglob('*')):
                    if path.is_file():
                        destination = target / path.relative_to(directory)
                        destination.parent.mkdir(parents=True, exist_ok=True)
                        if destination.exists():
                            assert not destination.is_symlink() and destination.read_bytes() == path.read_bytes()
                        else:
                            with destination.open('xb') as stream:
                                stream.write(path.read_bytes())
        report['status'] = 'SELECTED_SIX_LANGUAGE_SDKS_AND_REAL_APPLICATION_SOURCES_IMPLEMENTED_EXECUTION_PENDING'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (out / 'preparation.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
