#!/usr/bin/env python3
"""Join every acquired package and SDK platform to deployment review obligations.

This is a complete join over the supplied inventories, not complete semantic
review. Findings never create grants. Unknown effects, missing sources, absent
findings and unqualified native hosts remain explicit. Read acquired build
declarations as data only; do not configure, build or execute vendor sources.
"""
import argparse
from collections import Counter, defaultdict
import json
import os
from pathlib import Path
import re
import subprocess

from capability_inventory import ROOT, GATE, guard, sha, valid_output


OBLIGATIONS = {
    'filesystem': ['artifact.identity', 'artifact.transaction', 'storage.total-reservation', 'posix.compatibility-review'],
    'process.execution': ['external-process.image-closure', 'external-process.tree-supervision', 'external-process.restart-authority'],
    'ros.parameters': ['parameter.keys-and-types', 'parameter.transaction', 'parameter.events'],
    'ros.time_transform': ['clock.domain', 'transform.frames', 'state.freshness'],
    'ros.communication': ['communication.endpoint-direction', 'communication.type-and-qos', 'communication.peer-security'],
    'network.socket': ['transport.purpose-and-peers', 'transport.direction', 'external-process.syscalls'],
    'ros.wait': ['scheduling.deadline', 'scheduling.cancellation', 'scheduling.reserved-capacity'],
    'device.io': ['device.command-semantics', 'control.ownership', 'safety.independent-stop', 'external-process.device-access'],
    'ros.actions': ['action.transitions', 'action.cancellation', 'action.unknown-outcomes'],
    'model.inference': ['model.input-disclosure', 'model.output-bounds', 'proposal.separate-execution-authority'],
    'network.web': ['external-service.purpose', 'external-service.credentials', 'data.export-authority'],
    'plugin.loading': ['external-process.plugin-host', 'external-process.abi-and-dependencies', 'native.no-vm-loading'],
}

# These are bounded lexical observations, never an interpretation of CMake.
DECLARATIONS = {
    'shared_library': re.compile(r'\badd_library\s*\(\s*[^\s()]+\s+SHARED\b', re.I),
    'plugin_library': re.compile(r'\badd_library\s*\(\s*[^\s()]+\s+MODULE\b', re.I),
    'static_library': re.compile(r'\badd_library\s*\(\s*[^\s()]+\s+STATIC\b', re.I),
    'executable': re.compile(r'\badd_executable\s*\(', re.I),
    'pluginlib_export': re.compile(r'\bpluginlib_export_plugin_description_file\s*\(', re.I),
    'hardware_plugin': re.compile(r'base_class_type\s*=\s*[\"\']hardware_interface::[^\"\']+[\"\']'),
    'controller_plugin': re.compile(r'base_class_type\s*=\s*[\"\']controller_interface::[^\"\']+[\"\']'),
}


def package_obligations(inventory):
    """Preserve every instance; a missing finding must never become 'no authority needed'."""
    packages = {}
    for row in inventory:
        item = packages.setdefault(row['package'], dict(name=row['package'], instances=[], effects=set(),
            obligations=set(), unknown_effects=set(), source_path_reviews=[], semantic_review='PENDING',
            deployment_qualified=False, compatibility_decision='PER_COMPONENT_REVIEW_REQUIRED'))
        effects = row['effects']
        item['effects'].update(effects)
        for effect in effects:
            item['obligations'].update(OBLIGATIONS.get(effect, []))
            if effect not in OBLIGATIONS:
                item['unknown_effects'].add(effect)
        item['instances'].append({k: row[k] for k in ('source_id', 'manifest', 'source_url', 'source_ref', 'archive_sha256')})
    for item in packages.values():
        for key in ('effects', 'obligations', 'unknown_effects'):
            item[key] = sorted(item[key])
        item['finding_status'] = 'OBSERVED_EFFECTS' if item['effects'] else 'NO_EFFECT_OBSERVED_NOT_PROOF_OF_PURITY'
    return packages


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--corpus', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    guard()
    corpus, out = args.corpus.resolve(), args.output.resolve()
    temporary = (ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve()
    assert corpus.is_relative_to(temporary) and valid_output(out)
    out.mkdir(parents=True, exist_ok=False)
    pins = {}

    def read(path):
        pins[str(path)] = sha(path)
        return json.loads(path.read_text())

    def save(name, value):
        (out/name).write_text(json.dumps(value, indent=2)+'\n')

    inventory = read(corpus/'reconciliation/package-effect-inventory.json')
    coverage = read(corpus/'reconciliation/summary.json')
    previous = read(corpus/'linux-five-capability-inventory/results.json')
    packages = package_obligations(inventory)
    assert len(packages) == previous['package_names'] and len(inventory) == previous['package_instances']
    reviews = read(ROOT/'tools/ros_ecosystem/review-cases.json')
    review_paths = []
    for case in reviews['cases']:
        for evidence in case['evidence']:
            candidates = [base/'sources'/case['source_id']/evidence['path']
                          for base in (corpus, corpus/'supplemental', corpus/'recovery')]
            matches = [p for p in candidates if p.is_file() and sha(p) == evidence['sha256']]
            assert matches, ('Review evidence missing or changed', case['id'], evidence['path'])
            path = matches[-1]
            pins[str(path)] = evidence['sha256']
            owners = [row for row in inventory if row['source_id'] == case['source_id'] and
                      Path(evidence['path']).is_relative_to(Path(row['manifest']).parent)]
            if owners:
                depth = max(len(Path(row['manifest']).parts) for row in owners)
                owners = [row for row in owners if len(Path(row['manifest']).parts) == depth]
            names = sorted({row['package'] for row in owners})
            for name in names:
                if case['id'] not in packages[name]['source_path_reviews']:
                    packages[name]['source_path_reviews'].append(case['id'])
            review_paths.append(dict(case=case['id'], file=str(path), sha256=evidence['sha256'],
                packages=names, scope=case['reviewed_scope'], package_qualified=False))

    platform_root = corpus/'platform-protocol-sources'
    catalog = read(platform_root/'catalog.json')
    receipts = {p.stem: read(p) for p in sorted((platform_root/'receipts').glob('*.json'))}
    allowed = {key for key, receipt in receipts.items() if receipt.get('status') == 'ACQUIRED_AND_SCANNED'}
    result = subprocess.run(['rg', '--files', '--hidden', '--no-ignore', '-0',
        '-g', 'CMakeLists.txt', '-g', '*.cmake', '-g', '*.xml', str(platform_root/'sources')],
        capture_output=True, check=True)
    observations, refused = [], []
    by_source = defaultdict(list)
    examined = 0
    for raw in result.stdout.split(b'\0'):
        if not raw:
            continue
        path = Path(os.fsdecode(raw))
        relative = path.relative_to(platform_root/'sources')
        source_id = relative.parts[0]
        if source_id not in allowed:
            continue
        guard() if examined % 1000 == 0 else None
        source_root = platform_root/'sources'/source_id
        if not path.resolve().is_relative_to(source_root.resolve()) or path.stat().st_size > 4 << 20:
            refused.append(dict(file=str(path), reason='escaped-or-oversized'))
            continue
        data = path.read_bytes()
        try:
            text = data.decode('utf-8-sig')
        except UnicodeError:
            refused.append(dict(file=str(path), reason='encoding-review'))
            continue
        examined += 1
        pins[str(path)] = sha(path)
        # Mask ordinary comments without changing line coordinates. A block
        # comment or disabled branch can still match: keep the lexical label.
        masked = re.sub(r'(?m)^[ \t]*#[^\n]*', lambda m: ' '*len(m[0]), text)
        for kind, pattern in DECLARATIONS.items():
            for found in pattern.finditer(masked):
                index = len(observations)
                observations.append(dict(source_id=source_id, file=str(relative), sha256=pins[str(path)],
                    line=text.count('\n', 0, found.start())+1, kind=kind,
                    status='LEXICAL_DECLARATION_REQUIRES_REVIEW', executable_qualified=False))
                by_source[source_id].append(index)

    platforms = []
    for row in catalog['platforms']:
        ids = row['sources']
        platforms.append(dict(platform=row['platform'], sources=ids,
            unavailable_sources=[key for key in ids if key not in allowed],
            declaration_observations=[index for key in ids for index in by_source[key]],
            no_source_declared=not ids, extension_policy_only=True,
            in_vm_native_driver_allowed=False, native_host_qualification='PENDING'))
    save('packages.json', [packages[key] for key in sorted(packages)])
    save('source-path-reviews.json', review_paths)
    save('platforms.json', platforms)
    save('native-build-observations.json', observations)
    save('refused-build-files.json', refused)
    pins[str(Path(__file__).resolve())] = sha(__file__)
    guard()
    report = dict(scan_complete=True, scope=__doc__, package_names=len(packages), package_instances=len(inventory),
        packages_without_effect_findings=sum(not item['effects'] for item in packages.values()),
        packages_with_source_path_examples=sum(bool(item['source_path_reviews']) for item in packages.values()),
        effects={key: sum(key in item['effects'] for item in packages.values()) for key in OBLIGATIONS},
        unknown_effects=sorted({value for item in packages.values() for value in item['unknown_effects']}),
        platforms=len(platforms), platform_sources_declared=len(catalog['sources']),
        platform_sources_acquired=len(allowed), native_build_files_examined=examined,
        native_declarations=dict(Counter(row['kind'] for row in observations)), refused_build_files=len(refused),
        sros2_packages=sorted(name for name in packages if 'sros2' in name.lower()),
        esros2_packages=sorted(name for name in packages if 'esros2' in name.lower()),
        acquisition_coverage=coverage, full_corpus_semantics_complete=False,
        robot_deployment_qualified=False, grants_generated=0,
        cgroup=str(GATE), memory_max=68719476736, swap_max=0, inputs=pins,
        inputs_unchanged=all(sha(p) == digest for p, digest in pins.items()),
        artifacts={p.name: sha(p) for p in out.glob('*.json')})
    assert report['inputs_unchanged']
    save('results.json', report)
    print(json.dumps({key: report[key] for key in ('scan_complete', 'package_names', 'package_instances',
        'platforms', 'native_build_files_examined', 'native_declarations', 'packages_without_effect_findings',
        'full_corpus_semantics_complete', 'robot_deployment_qualified')}))


if __name__ == '__main__':
    main()
