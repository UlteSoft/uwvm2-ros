#!/usr/bin/env python3
"""Check explicit aggregate budget admission with unchanged Leju declarations.

The default API must refuse this large source graph; the explicitly selected
larger source budget accepts it while every individual record remains within
the original wire, node and depth limits. No output or native identity is made.
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
    source = Path(json.loads((root / 'inputs.json').read_bytes())['source_root'])
    path = source / 'sdk/origin/robots/industrial/vendor_profile.py'
    spec = importlib.util.spec_from_file_location('leju_explicit_budget_case', path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    parent_path, bounded_path = source / 'sdk/origin/tools/custom_types.py', source / 'sdk/origin/custom-types/bounded_profile.py'
    parent, bounded, adapter = parent_path.read_bytes(), bounded_path.read_bytes(), path.read_bytes()
    kwargs = dict(parent_path=parent_path, bounded_path=bounded_path,
                  parent_sha256=hashlib.sha256(parent).hexdigest(), bounded_sha256=hashlib.sha256(bounded).hexdigest(),
                  templates={n: (bounded_path.parent / n).read_bytes() for n in
                             ('interface_templates/bounded_interfaces.' + e + '.in' for e in ('rs', 'mjs', 'go', 'ts'))},
                  adapter_raw=adapter, adapter_sha256=hashlib.sha256(adapter).hexdigest())
    catalog = source / 'sdk/origin/robots/humanoids/leju/catalogs/ros1_main_platform/catalog.json'
    default = module.API(parent, bounded, **kwargs)
    assert default.max_total_nodes == 16384
    try:
        default.load_catalog(catalog, False)
    except default.parent.GenerationError as error:
        assert 'total expanded generation nodes exceed explicit source budget 16384' in str(error)
    else:
        raise AssertionError('Implicit aggregate budget expansion')
    explicit = module.API(parent, bounded, **kwargs, max_total_nodes=65536)
    records, ordered, interfaces, pins = explicit.load_catalog(catalog, False)
    total = sum(r.nodes for r in ordered)
    assert 16384 < total <= 65536
    assert all(r.maximum <= 32768 and r.nodes <= 4096 and r.depth <= 16 for r in ordered)
    refused = []
    for value in (0, -1, 1048577, True, 65536.0, '65536'):
        try:
            module.API(parent, bounded, **kwargs, max_total_nodes=value)
        except ValueError as error:
            assert 'explicit aggregate source node budget' in str(error)
            refused.append(repr(value))
        else:
            raise AssertionError('Invalid explicit source budget accepted')
    out.mkdir(mode=0o700)
    (out / 'budget.json').write_text(json.dumps(dict(
        status='DEFAULT_REFUSAL_EXPLICIT_AGGREGATE_ADMISSION_AND_RECORD_LIMITS_PASS',
        default_nodes=16384, explicit_nodes=65536, actual_nodes=total,
        invalid_inputs_refused=refused, emitted_products=0, default_authority=[]), indent=2) + '\n')


if __name__ == '__main__':
    main()
