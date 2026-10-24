#!/usr/bin/env python3
"""Qualify the real integrated or historical complete-record standard-Go branch.

The admitted finite Linux owner is mandatory. This is generation plus real
gofmt syntax/idempotence, not a Go app typecheck/build/Guest/native result.
Legacy TinyGo output must remain byte-identical for all seven full families.
"""
import argparse
import ast
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def need(value, message):
    if not value:
        raise RuntimeError(message)


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'application', 'go', 'gofmt', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    emitter = parser.add_mutually_exclusive_group(required=True)
    emitter.add_argument('--proposal', type=Path)
    emitter.add_argument('--published-emitter', action='store_true')
    args = parser.parse_args()
    need(sys.platform == 'linux' and os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') == '1',
         'same finite Linux owner/common lock required')
    need(sha(args.gofmt) == 'acb1f5c293c3f0e2032676b72a288f7f18022653aedb465e3c1bd0ea78eab6e5',
         'actual pinned Go1.24.2 formatter required')
    args.out.mkdir()
    pins = {}

    def capture(path):
        raw = path.read_bytes()
        row = dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())
        need(pins.setdefault(str(path), row) == row, 'source changed during capture')
        return raw

    for path in (Path(__file__), args.go, args.gofmt):
        capture(path)
    ast.parse(Path(__file__).read_bytes())
    source = args.source.resolve(strict=True)
    private = args.out / ('captured-standard-go-source' if args.published_emitter else 'paired-standard-go-source')
    rows = []
    required = ['sdk/origin/tools/custom_types.py', 'sdk/origin/custom-types/bounded_profile.py',
                'sdk/origin/robots/industrial/vendor_profile.py', 'sdk/tools/cpp_profiles.py',
                'sdk/origin/tools/native_languages/records.py', 'sdk/origin/tools/native_languages/documentation.py']
    required += ['sdk/origin/custom-types/interface_templates/bounded_interfaces.' + ext + '.in'
                 for ext in ('rs', 'mjs', 'go', 'ts')]
    for relative in required:
        target = private / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(capture(source / relative))
    changes = [('sdk/origin/custom-types/bounded_profile.py', 'emit_control_go', None),
               ('sdk/origin/robots/industrial/vendor_profile.py', 'records', 'API')]
    for relative, name, container in changes:
        if args.published_emitter:
            rows.append(dict(file=relative, source_sha256=sha(source / relative),
                             exact_function=(container + '.' if container else '') + name,
                             source_mode='INTEGRATED_PUBLIC_EMITTER_NO_PRIVATE_OVERRIDE', live_shared_edited=False))
            continue
        before = args.proposal / 'before' / relative
        candidate = args.proposal / 'candidate' / relative
        original = capture(before)
        changed = capture(candidate)
        need(capture(source / relative) == original, 'exact captured proposal preimage differs: ' + relative)
        first, second = ast.parse(original), ast.parse(changed)
        parent_before = first if container is None else next(node for node in first.body if isinstance(node, ast.ClassDef) and node.name == container)
        parent_after = second if container is None else next(node for node in second.body if isinstance(node, ast.ClassDef) and node.name == container)
        index = next(index for index, node in enumerate(parent_before.body) if isinstance(node, ast.FunctionDef) and node.name == name)
        replacement = next(node for node in parent_after.body if isinstance(node, ast.FunctionDef) and node.name == name)
        parent_before.body[index] = replacement
        need(ast.dump(first, include_attributes=False) == ast.dump(second, include_attributes=False),
             'proposal changes code outside exactly the reviewed function: ' + name)
        (private / relative).write_bytes(changed)
        rows.append(dict(file=relative, exact_function=(container + '.' if container else '') + name,
                         before_sha256=sha(before), candidate_sha256=sha(candidate), live_shared_edited=False))
    (args.out / 'emitter-source-mode.json').write_text(json.dumps(dict(
        source_mode='PUBLIC_NO_OVERRIDE' if args.published_emitter else 'HISTORICAL_PRIVATE_PROPOSAL',
        inputs=rows), indent=2) + '\n')

    def api(root, suffix):
        origin = root / 'sdk/origin'
        parent, bounded, adapter_path = (origin / relative for relative in (
            'tools/custom_types.py', 'custom-types/bounded_profile.py', 'robots/industrial/vendor_profile.py'))
        adapter_raw = adapter_path.read_bytes()
        adapter = load('_task06_go_adapter_' + suffix, adapter_path)
        templates = {name: (bounded.parent / name).read_bytes() for name in (
            'interface_templates/bounded_interfaces.' + ext + '.in' for ext in ('rs', 'mjs', 'go', 'ts'))}
        return adapter.API(parent.read_bytes(), bounded.read_bytes(), parent_path=parent, bounded_path=bounded,
                           parent_sha256=sha(parent), bounded_sha256=sha(bounded), templates=templates,
                           adapter_raw=adapter_raw, adapter_sha256=sha(adapter_path)), adapter

    original_api, _ = api(source, 'before')
    selected_api, adapter = api(private, 'candidate')
    docs_path = private / 'sdk/origin/tools/native_languages/documentation.py'
    previous = sys.modules.get('documentation')
    sys.modules['documentation'] = load('_task06_actual_go_documentation', docs_path)
    try:
        records_helper = load('_task06_actual_go_record_documenter', private / 'sdk/origin/tools/native_languages/records.py')
    finally:
        if previous is None:
            del sys.modules['documentation']
        else:
            sys.modules['documentation'] = previous
    environment = dict(os.environ, GOTOOLCHAIN='local', GOTELEMETRY='off', GOENV='off', GOWORK='off', GOPROXY='off', GOSUMDB='off')
    version = subprocess.run([args.go, 'version'], env=environment, capture_output=True, timeout=30, check=True)
    need(b'go1.24.2 ' in version.stdout, 'actual Go formatter runtime differs')
    (args.out / 'go-version.stdout').write_bytes(version.stdout)
    results = []
    for profile in ('ros2_humble_v1', 'ros2_jazzy_v1', 'ros2_lyrical_v1', 'ros2_humble_v2',
                    'ros2_jazzy_v2', 'ros2_humble_v2_2', 'ros2_jazzy_v2_2'):
        catalog = source / 'sdk/origin/robots/arms/flexiv/catalogs' / profile / 'catalog.json'
        family = 'flexiv-' + profile.replace('_', '-') + '-complete'
        # Family spelling uses the original literal v2-2 source revision.
        family_path = source / 'sdk/origin/custom-types' / family
        capture(catalog)
        for row in json.loads(catalog.read_bytes())['types']:
            capture(catalog.parent / row['file'])
        original_records, original_order, _, original_pins = original_api.load_catalog(catalog, False)
        records, ordered, _, pins_from_loader = selected_api.load_catalog(catalog, False)
        need(original_pins == pins_from_loader, 'standard-Go branch changed admitted source IR')
        legacy = original_api.records(original_records, original_order)['bounded_records.go']
        candidate_legacy = selected_api.records(records, ordered)['bounded_records.go']
        historical = capture(family_path / 'bounded_records.go')
        need(legacy == candidate_legacy and legacy.encode() == historical,
             'unchanged TinyGo default must match actual complete original family bytes: ' + profile)
        prepared = selected_api.records(records, ordered, go_foundation='go')['bounded_records.go']
        need('//go:build wasip1 && wasm && ros2 && !ros1' in prepared and
             'uwvm2.local/roswasmgo' in prepared and 'ros.APIError{' in prepared and
             'tinygo' not in prepared and 'ros.Error{' not in prepared,
             'real standard-Go foundation branch was not selected')
        documented = records_helper.document_records('go', prepared, ordered)
        folder = args.out / 'generated' / family
        folder.mkdir(parents=True)
        raw = folder / 'bounded_records.before-gofmt.go'
        raw.write_text(documented)
        first = subprocess.run([args.gofmt, raw], env=environment, capture_output=True, timeout=30, check=True)
        target = folder / 'bounded_records.go'
        target.write_bytes(first.stdout)
        second = subprocess.run([args.gofmt, target], env=environment, capture_output=True, timeout=30, check=True)
        need(first.stdout == second.stdout, 'actual standard-Go source formatter is not idempotent')
        results.append(dict(profile=profile, family=family, whole_records=len(ordered),
                            native_registry_emitted=False, source_inputs=pins_from_loader,
                            tinygo_default_unchanged=True, output=dict(bytes=target.stat().st_size, sha256=sha(target))))
        (folder / 'generation.standard-go.json').write_text(json.dumps(results[-1], indent=2) + '\n')
    application = args.out / 'application/main.go'
    application.parent.mkdir()
    capture(args.application)
    first = subprocess.run([args.gofmt, args.application], env=environment, capture_output=True, timeout=30, check=True)
    application.write_bytes(first.stdout)
    second = subprocess.run([args.gofmt, application], env=environment, capture_output=True, timeout=30, check=True)
    need(first.stdout == second.stdout, 'actual standard-Go application formatting differs on second pass')
    for path, row in pins.items():
        need(sha(Path(path)) == row['sha256'], 'original/proposal/tool/source changed')
    # API source captures include the exact real C++ helper and four templates.
    for observed_api in (original_api, selected_api):
        for path, body in observed_api.captured.items():
            need(adapter.read_source(Path(path)) == body, 'captured authoritative helper changed')
    summary = dict(status='PASS_COMPLETE_STANDARD_GO_SOURCE_ONLY', profiles=7,
                   emitter_source_mode='PUBLIC_NO_OVERRIDE' if args.published_emitter else 'HISTORICAL_PRIVATE_PROPOSAL',
                   whole_records=sum(row['whole_records'] for row in results),
                   tinygo_default_byte_comparisons=7, real_gofmt_idempotent=True,
                   application=dict(bytes=application.stat().st_size, sha256=sha(application)),
                   sdk_builds=0, application_typechecks=0, wasm_guest=0, native_ros=0,
                   public_api_parser_inventory='NOT_RUN', publication_ready=False)
    (args.out / 'source-pins.json').write_text(json.dumps(list(pins.values()), indent=2) + '\n')
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
