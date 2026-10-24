#!/usr/bin/env python3
"""Derive separate r6d metadata source without importing or running a candidate.

The r3 producer, extraction/model/storage helpers and restart helper remain
exact bytes. Only fresh r6d paths and the explicit fourth candidate-view
receipt/root are added to the owner and stager. This factory writes source and
an unexecuted source package; it never contacts Linux or starts the acquisition.
"""
from __future__ import annotations

import argparse
import ast
import base64
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
BASE = HERE.parent / 'noetic6_r3'
EXPECTED = {
    'owner.py': 'd8b48ff4375a1490bdf2bdd5c4d2ca4e9204673671c79e0ad89da60015cbf5cf',
    'stage.py': '7bbcb5960b21bdac948e3d3a3fef1b35f986221a94888ea4d279d6985a82ded6',
    'acquire-sdk.py': '71fb6117a82b7154422c458ceee051c1bc44cc4a203a5a1d5a368f6be79d9e26',
    'fresh_rebuild.py': '1b79407cb37fde9ed72758c273bf8cc3e46ffdd7418bc820f70e31d112cb2d91',
    'signed_tar_model.py': '227f140b7587c989ed9dbcf6d4d035cb886f2ff8b9ba6bc9112c4b31bf46f7df',
    'owned_storage.py': 'cdc84eb4b55c727d74a382b019d4faa6f7e65778ec1c07be082073d8c37e1d97',
}
FRAGMENT = '588d2b269734cf4715643988aabfd31daffd62e2bad2d27f60775563a12f1d87'


def need(value, message):
    if not value:
        raise ValueError(message)


def change(text, old, new):
    need(text.count(old) == 1, 'unique source anchor required: ' + old[:120])
    return text.replace(old, new, 1)


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    need(not args.out.exists(), 'fresh source-only output required')
    bodies = {}
    for name, sha in EXPECTED.items():
        raw = (BASE / name).read_bytes()
        need(len(raw) <= 128 * 1024 and hashlib.sha256(raw).hexdigest() == sha, 'sealed r3 source differs: ' + name)
        bodies[name] = raw
    fragment = (HERE / 'cacheview_contract.py').read_bytes()
    need(hashlib.sha256(fragment).hexdigest() == FRAGMENT, 'reviewed embedded view-contract source differs')
    ast.parse(fragment)
    # A module docstring is harmless inside the embedded source; it is not an
    # import, an ambient module lookup, or a second executable entry point.
    for name in ('owner.py', 'stage.py'):
        text = bodies[name].decode()
        need(text.count('r6c') == (3 if name == 'owner.py' else 4), 'exact fresh-path literal count changed')
        text = text.replace('r6c', 'r6d')
        text = change(text, '\ndef main():\n', '\n' + fragment.decode() + '\n\ndef main():\n')
        roots = "str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads'), '/usr/sbin']"
        text = change(text, roots, "str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads'), '/usr/sbin', str(CACHE_VIEW / 'debs')]")
        if name == 'owner.py':
            text = change(text,
                "'original_producer', 'infrastructure_pins', 'resolver_alias', 'budgets', 'stage_request', 'source_package'}",
                "'original_producer', 'infrastructure_pins', 'resolver_alias', 'budgets', 'stage_request', 'source_package', 'cache_view_receipt'}")
            text = change(text, "spec differs beyond current cgroup, four explicit roots and pinned fstrim",
                "spec differs beyond current cgroup, five explicit roots and pinned fstrim")
            text = change(text,
                "        need(documents['cache']['roots'] == [str(M / 'noetic-amd64-native-sdk-acquisition-r1/apt/downloads'), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads')] and\n             documents['cache']['required_hit_packages'] == ['ros-noetic-ros-base'], 'exact three retained DEB caches required')",
                "        view_selection = admit_cache_view(control['cache_view_receipt'], remember, guard)\n        report['cache_view_selection'] = view_selection\n        need(documents['cache']['roots'] == [str(M / 'noetic-amd64-native-sdk-acquisition-r1/apt/downloads'), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads'), str(CACHE_VIEW / 'debs')] and\n             documents['cache']['provenance'] == [continuation_control['old_report'], continuation_control['old_outer_report'], view_selection['source_reports']['inner'], view_selection['source_reports']['outer']] and\n             documents['cache']['required_hit_packages'] == ['ros-noetic-ros-base'], 'exact four retained caches and genuine failed provenance required')")
            text = change(text,
                "            need(common.source_state(S, MF) == source_before, 'frozen complete Source35 changed')\n            after = []",
                "            need(common.source_state(S, MF) == source_before, 'frozen complete Source35 changed')\n            need(admit_cache_view(control['cache_view_receipt'], remember, guard) == view_selection, 'whole42 original/whole41 view or receipt changed')\n            after = []")
        else:
            text = change(text,
                "'old_report', 'old_outer_report', 'old_ledger', 'old_partial_sdk', 'owned_storage'}",
                "'old_report', 'old_outer_report', 'old_ledger', 'old_partial_sdk', 'owned_storage', 'cache_view_receipt'}")
            text = change(text,
                "    old_report, old_outer, old_ledger = (selected(request[key]) for key in ('old_report', 'old_outer_report', 'old_ledger'))",
                "    view_selection = admit_cache_view(request['cache_view_receipt'], selected, guard)\n    old_report, old_outer, old_ledger = (selected(request[key]) for key in ('old_report', 'old_outer_report', 'old_ledger'))")
            text = change(text,
                "[str(M / 'noetic-amd64-native-sdk-acquisition-r1/apt/downloads'), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads')],\n        'provenance': [old_report, old_outer]",
                "[str(M / 'noetic-amd64-native-sdk-acquisition-r1/apt/downloads'), str(R4 / 'apt/downloads'), str(R5 / 'apt/downloads'), str(CACHE_VIEW / 'debs')],\n        'provenance': [old_report, old_outer, view_selection['source_reports']['inner'], view_selection['source_reports']['outer']]")
            text = change(text,
                "'budgets': BUDGET, 'stage_request': request_pin, 'source_package': payload_pin})",
                "'budgets': BUDGET, 'stage_request': request_pin, 'source_package': payload_pin, 'cache_view_receipt': request['cache_view_receipt']})")
            text = change(text,
                "    need(resolver_alias() == alias, 'stage current resolver alias changed')",
                "    need(resolver_alias() == alias, 'stage current resolver alias changed')\n    need(admit_cache_view(request['cache_view_receipt'], selected, guard) == view_selection, 'whole42 original/whole41 view changed during metadata stage')")
            text = change(text,
                "        'current_old_partial_sdk': sdk, 'old_status_promoted': False, 'old_SDK_qualified': False,",
                "        'current_old_partial_sdk': sdk, 'cache_view_selection': view_selection, 'old_status_promoted': False, 'old_SDK_qualified': False,")
        ast.parse(text, filename=name)
        bodies[name] = text.encode()
    restart = (HERE.parent / 'requalify_after_restart.py').read_bytes()
    need(len(restart) == 47606 and hashlib.sha256(restart).hexdigest() ==
         '0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260', 'sealed restart helper differs')
    args.out.mkdir()
    for name, body in bodies.items():
        (args.out / name).write_bytes(body)
    (args.out / 'requalify_after_restart.py').write_bytes(restart)
    names = {'producer': ('acquire-sdk.py', 'acquire-sdk.py'), 'resume': ('fresh_rebuild.py', 'fresh_rebuild.py'),
             'model': ('signed_tar_model.py', 'signed_tar_model.py'), 'storage': ('owned_storage.py', 'owned_storage.py'),
             'restart': ('requalify_after_restart.py', 'requalify_after_restart.py'), 'owner': ('owner.py', 'launcher.py')}
    files = {}
    for role, (local, remote) in names.items():
        body = restart if role == 'restart' else bodies[local]
        files[role] = {'name': remote, 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest(), 'base64': base64.b64encode(body).decode()}
    package = {'format_version': 1, 'kind': 'noetic-r6-reviewed-fresh-all513-source-package', 'files': files}
    (args.out / 'reviewed-source-package.json').write_text(json.dumps(package, indent=2, sort_keys=True) + '\n')
    receipt = {'format_version': 1, 'kind': 'noetic-r6d-source-only-metadata-derivation', 'executed': False,
               'producer_changed': False, 'SafeTree_changed': False, 'all513_fresh_authority_and_extraction_required': True,
               'input_sha256': EXPECTED, 'embedded_contract_sha256': FRAGMENT,
               'source_outputs': {p.name: {'bytes': p.stat().st_size, 'sha256': hashlib.sha256(p.read_bytes()).hexdigest()}
                                  for p in sorted(args.out.iterdir())}}
    (args.out / 'derivation.json').write_text(json.dumps(receipt, indent=2, sort_keys=True) + '\n')


if __name__ == '__main__':
    main()
