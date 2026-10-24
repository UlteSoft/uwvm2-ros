#!/usr/bin/env python3
"""Measure candidate record bounds in a private, deliberately unadmitted IR.

Only this short-lived analysis API's numerical ceilings are relaxed to inspect
all offending candidates in one pass. Nothing is emitted, registered, bound or
accepted; production code, source catalogs, runtime limits and default rights
are unchanged. A subsequent normal strict preparation must independently pass.
This avoids repeatedly emitting SDKs merely to discover the next storage bound.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    path = source / 'sdk/origin/robots/industrial/vendor_profile.py'
    spec = importlib.util.spec_from_file_location('leju_unadmitted_size_analysis', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    parent_path, bounded_path = source / 'sdk/origin/tools/custom_types.py', source / 'sdk/origin/custom-types/bounded_profile.py'
    parent, bounded, adapter = parent_path.read_bytes(), bounded_path.read_bytes(), path.read_bytes()
    templates = {n: (bounded_path.parent / n).read_bytes() for n in
                 ('interface_templates/bounded_interfaces.' + e + '.in' for e in ('rs', 'mjs', 'go', 'ts'))}
    api = module.API(parent, bounded, parent_path=parent_path, bounded_path=bounded_path,
                     parent_sha256=hashlib.sha256(parent).hexdigest(), bounded_sha256=hashlib.sha256(bounded).hexdigest(),
                     templates=templates, adapter_raw=adapter, adapter_sha256=hashlib.sha256(adapter).hexdigest())
    api.env.update(MAX_WIRE=1 << 40, MAX_NODES=1 << 30, MAX_TOTAL_NODES=1 << 40)
    profiles = json.loads((Path(inputs['catalog_source']) / 'profiles.json').read_bytes())['profiles']
    rows = []
    for profile in profiles:
        catalog = Path(inputs['catalog_source']) / 'catalogs' / profile['id'] / 'catalog.json'
        records, ordered, interfaces, pins = api.load_catalog(catalog, False)
        violations = [dict(type=r.name, bytes=r.maximum, nodes=r.nodes, depth=r.depth)
                      for r in ordered if r.maximum > 32768 or r.nodes > 4096 or r.depth > 16]
        rows.append(dict(profile=profile['id'], records=len(ordered),
                         total_schema_nodes=sum(r.nodes for r in ordered),
                         maximum_record_bytes=max(r.maximum for r in ordered), violations=violations))
    result = dict(status='DIAGNOSTIC_ONLY_NO_SCHEMAS_OR_PRODUCTS_ADMITTED', profiles=rows,
                  private_analysis_ceilings_only=True, emitted_products=0, native_or_guest_runs=0,
                  production_source_or_limits_modified=False, default_authority=[])
    (out / 'diagnosis.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result), flush=True)


if __name__ == '__main__':
    main()
