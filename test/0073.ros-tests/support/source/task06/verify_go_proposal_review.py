#!/usr/bin/env python3
"""Produce the exact two-function Go review diff without loading a generator.

This deliberately separate review-only workload does not change the existing
32 MiB complete-generation plan. It executes no generator, formatter, compiler,
SDK, Guest or native peer. The approved finite Linux owner is mandatory.
"""
import argparse
import ast
import difflib
import hashlib
import json
import os
from pathlib import Path
import sys


def pin(path):
    raw = path.read_bytes()
    return dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('proposal', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same finite Linux owner/common lock required')
    args.out.mkdir()
    changes, patch, source_pins = [], [], [pin(Path(__file__))]
    targets = [('sdk/origin/custom-types/bounded_profile.py', 'emit_control_go', None),
               ('sdk/origin/robots/industrial/vendor_profile.py', 'records', 'API')]
    for relative, function, container in targets:
        before, candidate = (args.proposal / folder / relative for folder in ('before', 'candidate'))
        source_pins.extend([pin(before), pin(candidate)])
        old, new = before.read_text(), candidate.read_text()
        first, second = ast.parse(old), ast.parse(new)
        left, right = first, second
        if container:
            left = next(node for node in first.body if isinstance(node, ast.ClassDef) and node.name == container)
            right = next(node for node in second.body if isinstance(node, ast.ClassDef) and node.name == container)
        index = next(at for at, node in enumerate(left.body) if isinstance(node, ast.FunctionDef) and node.name == function)
        left.body[index] = next(node for node in right.body if isinstance(node, ast.FunctionDef) and node.name == function)
        if ast.dump(first, include_attributes=False) != ast.dump(second, include_attributes=False):
            raise RuntimeError('changes outside the single reviewed function: ' + relative)
        if old == new:
            raise RuntimeError('expected an actual changed helper: ' + relative)
        patch.extend(difflib.unified_diff(old.splitlines(keepends=True), new.splitlines(keepends=True),
                                         fromfile='a/' + relative, tofile='b/' + relative))
        changes.append(dict(path=relative, exact_function=(container + '.' if container else '') + function,
                            before=pin(before), candidate=pin(candidate),
                            whole_ast_outside_reviewed_function_unchanged=True))
    target = args.out / 'standard-go-current.patch'
    target.write_text(''.join(patch))
    for row in source_pins:
        if pin(Path(row['path'])) != row:
            raise RuntimeError('source changed during review capture')
    manifest = dict(schema='task06-owned-shared-proposal-v1',
                    status='PASS_EXACT_DIFF_AND_TWO_FUNCTION_AST_CONFINEMENT_ONLY',
                    changes=changes, patch=pin(target), test=pin(Path(__file__)),
                    live_shared_edited=False, generator_loaded=False,
                    generated_families=0, formatter_runs=0, sdk_builds=0, guest=0, native_ros=0,
                    complete_generation_plan_disk_mib=32, publication_ready=False)
    (args.out / 'proposal-manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    (args.out / 'source-pins.json').write_text(json.dumps(source_pins, indent=2) + '\n')
    print(json.dumps(dict(status=manifest['status'], exact_function_checks=2,
                         generated_families=0, sdk_builds=0, guest=0, native_ros=0)), flush=True)


if __name__ == '__main__':
    main()
