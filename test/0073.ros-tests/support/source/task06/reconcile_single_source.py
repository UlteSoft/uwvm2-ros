#!/usr/bin/env python3
"""Inventory Task06's real source reconciliation inside the finite Linux owner.

This maintenance program reads an inert source snapshot. It neither runs an
upstream program nor changes the checkout. Exact proposed destinations and both
preimages are retained so an owner can review every divergent source before
applying ordinary source edits to the authoritative SDK.
"""
import argparse
import difflib
import hashlib
import json
import os
from pathlib import Path
import re
import shlex


VENDORS = ('elephant', 'ufactory', 'dobot', 'flexiv', 'realman')
KITS = {'elephant': 'elephant/mycobot', 'ufactory': 'ufactory/xarm',
        'dobot': 'dobot', 'flexiv': 'flexiv', 'realman': 'realman'}
LANGUAGES = {'c': 'c', 'cpp': 'cpp', 'rust': 'rs', 'javascript': 'js',
             'assemblyscript': 'as', 'tinygo': 'tinygo', 'go': 'go',
             'python': 'py', 'java': 'java', 'kotlin': 'kotlin',
             'csharp': 'csharp', 'fsharp': 'fsharp', 'vbnet': 'vbnet',
             'moonbit': 'moonbit', 'zig': 'zig'}
SUFFIX = {'.h': 'c', '.hpp': 'cpp', '.rs': 'rs', '.mjs': 'js',
          '.ts': 'as', '.go': 'tinygo', '.py': 'py'}


def sha(body):
    return hashlib.sha256(body).hexdigest()


def files(base):
    if not base.exists():
        return
    for path in sorted(base.rglob('*')):
        if path.is_symlink() or path.is_file():
            yield path


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + '\n')


def family_target(vendor, family, version, name):
    tail = f'ros{version}/{family}/{name}'
    if name in ('native_identity.cc', 'native_constants.hpp', 'manifest.hpp',
                'native_ros1.hpp', 'native_ros2.hpp'):
        return f'sdk/src/platform/{vendor}/native/{tail}', 'native-source'
    if name == 'operations.c':
        return f'sdk/example/platform/{vendor}/c/{tail}', 'application'
    language = SUFFIX.get(Path(name).suffix)
    if language:
        return f'sdk/include/platform/{vendor}/{language}/{tail}', 'public-module'
    return f'sdk/share/platform/{vendor}/{tail}', 'provenance'


def mappings(sdk):
    for vendor in VENDORS:
        platform = sdk / 'origin/platform' / vendor
        for path in files(platform):
            rel = path.relative_to(platform)
            parts = rel.parts
            if path.is_symlink():
                yield path, None, 'obsolete-link'
                continue
            if parts[0] in LANGUAGES:
                language = LANGUAGES[parts[0]]
                if len(parts) < 3:
                    raise ValueError(str(path))
                section, tail = parts[1], Path(*parts[2:])
                if section == 'include':
                    if tail.name == 'README.md':
                        dest = f'sdk/share/platform/{vendor}/language/{language}/modules/{tail}'
                    elif tail.name.endswith('.json'):
                        dest = f'sdk/share/platform/{vendor}/{tail}'
                    else:
                        dest = f'sdk/include/platform/{vendor}/{language}/{tail}'
                    yield path, dest, 'public-module' if path.suffix != '.md' else 'documentation'
                elif section == 'example':
                    if 'vendor' in tail.parts:
                        raise ValueError('unexpected ordinary source under legacy vendor link: ' + str(path))
                    yield path, f'sdk/example/platform/{vendor}/{language}/{tail}', 'application'
                else:
                    raise ValueError(str(path))
            elif parts[0] == 'example':
                yield path, f'sdk/example/platform/{vendor}/{Path(*parts[1:])}', 'application-build'
            elif parts[0] == 'source-candidates':
                if len(parts) == 2:
                    yield path, f'sdk/share/platform/{vendor}/source-candidates/{parts[1]}', 'provenance'
                else:
                    dest, role = family_target(vendor, parts[1], 1, Path(*parts[2:]).as_posix())
                    yield path, dest, role
            elif parts[0] == 'controller-core':
                if parts[1] == 'prototype':
                    name = parts[2]
                    dest = (f'sdk/example/platform/{vendor}/js/official/{name}' if name == 'recorded_controllers.mjs'
                            else f'sdk/include/platform/{vendor}/js/{name}')
                    yield path, dest, 'application' if name == 'recorded_controllers.mjs' else 'public-module'
                elif parts[1] == 'generated':
                    flavor, name = parts[2], Path(*parts[3:]).as_posix()
                    dest, role = family_target(vendor, f'flexiv-controller-core-{flavor}-complete', 2, name)
                    yield path, dest, role
                else:
                    yield path, f'sdk/share/platform/{vendor}/controller-core/{Path(*parts[1:])}', 'schema-documentation'
            elif parts[0] == 'query-core':
                if parts[1] == 'prototype':
                    name = parts[2]
                    dest = (f'sdk/example/platform/{vendor}/js/official/{name}' if name == 'api_get_arm_state.mjs'
                            else f'sdk/include/platform/{vendor}/js/{name}')
                    yield path, dest, 'application' if name == 'api_get_arm_state.mjs' else 'public-module'
                elif parts[1] == 'generated':
                    dest, role = family_target(vendor, 'realman-query-empty-source', 1, Path(*parts[2:]).as_posix())
                    yield path, dest, role
                else:
                    yield path, f'sdk/share/platform/{vendor}/query-core/{Path(*parts[1:])}', 'schema-documentation'
            else:
                yield path, f'sdk/share/platform/{vendor}/{rel}', 'documentation-provenance'

        kit = sdk / 'origin/robots/arms' / KITS[vendor]
        for path in files(kit):
            rel = path.relative_to(kit)
            parts = rel.parts
            if parts[0] == 'upstream':
                continue
            if path.is_symlink():
                yield path, None, 'obsolete-link'
            elif parts[0] == 'include':
                language = SUFFIX[path.suffix]
                yield path, f'sdk/include/platform/{vendor}/{language}/{Path(*parts[1:])}', 'public-module'
            elif parts[0] == 'catalogs':
                yield path, f'sdk/share/platform/{vendor}/{rel}', 'schema-provenance'
            elif parts[0] == 'tools':
                if path.name == 'emit_contracts.py':
                    dest = f'test/0073.ros-tests/support/source/task06/vendor/{vendor}/{path.name}'
                else:
                    dest = f'sdk/tools/platform/{vendor}/{Path(*parts[1:])}'
                yield path, dest, 'maintenance-tool'
            elif parts[0] == 'examples':
                tail = Path(*parts[1:])
                if parts[1] == 'multi_vendor':
                    yield path, f'test/0073.ros-tests/support/source/task06/vendor/{vendor}/{tail}', 'isolated-peer-fixture'
                elif parts[1] in ('go', 'rust'):
                    language = 'tinygo' if parts[1] == 'go' else 'rs'
                    yield path, f'sdk/example/platform/{vendor}/{language}/{Path(*parts[2:])}', 'application'
                elif path.suffix == '.json':
                    yield path, f'sdk/example/platform/{vendor}/js/{tail}', 'application-recipe'
                elif path.suffix in ('.c', '.h', '.cc', '.cpp', '.mjs', '.ts'):
                    language = {'.c': 'c', '.h': 'c', '.cc': 'cpp', '.cpp': 'cpp', '.mjs': 'js', '.ts': 'as'}[path.suffix]
                    if path.name == 'entry.h':
                        # Exactly one C implementation; C++ includes this file.
                        language = 'c'
                    yield path, f'sdk/example/platform/{vendor}/{language}/{tail}', 'application'
                else:
                    raise ValueError('unclassified kit example: ' + str(path))
            elif path.name == 'CMakeLists.txt':
                yield path, f'test/0073.ros-tests/support/source/task06/vendor/{vendor}/CMakeLists.txt', 'contract-build'
            else:
                name = 'source-reference.md' if path.name == 'README.md' else rel.as_posix()
                yield path, f'sdk/share/platform/{vendor}/{name}', 'documentation-provenance'

        for family in sorted((sdk / 'origin/custom-types').glob(vendor + '-*')):
            proof = json.loads((family / 'generation.vendor.json').read_bytes())
            versions = {row['ros_version'] for row in proof['records']}
            if len(versions) != 1:
                raise ValueError('mixed original dialect: ' + str(family))
            version = next(iter(versions))
            for path in files(family):
                if path.is_symlink():
                    raise ValueError('generated source symlink: ' + str(path))
                dest, role = family_target(vendor, family.name, version, path.relative_to(family).as_posix())
                yield path, dest, role


def main():
    p = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    p.add_argument('--snapshot', type=Path, required=True)
    p.add_argument('--apps', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--emit', action='store_true')
    args = p.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('original finite owner/common lock required')
    args.out.mkdir()
    sdk = args.snapshot / 'sdk'
    root = args.snapshot
    records = []
    proposals = {}
    for source, dest, role in mappings(sdk):
        relative = source.relative_to(root).as_posix()
        record = dict(old_path=relative, new_path=dest, role=role)
        if source.is_symlink():
            record.update(link=os.readlink(source), decision='remove-obsolete-link')
        else:
            raw = source.read_bytes()
            record.update(old_sha256=sha(raw), old_bytes=len(raw))
            proposals.setdefault(dest, []).append(source)
        records.append(record)

    # Existing canonical modules are also dependency anchors. No foundation
    # source is rewritten or selected from a vendor's obsolete base symlink.
    source_dest = {}
    for dest, values in proposals.items():
        for source in values:
            source_dest[source.resolve()] = dest.removeprefix('sdk/')
    foundation = {'js': 'javascript', 'rs': 'rust', 'as': 'assemblyscript',
                  'tinygo': 'tinygo', 'go': 'go'}
    for backend in ('ros', 'ros1', 'ros2'):
        base = sdk / 'include/platform' / backend
        for path in files(base):
            if path.is_symlink():
                continue
            rel = path.relative_to(base)
            language = rel.parts[0]
            if language in foundation:
                old = sdk / 'origin' / foundation[language] / Path(*rel.parts[1:])
                source_dest.setdefault(old.resolve(), path.relative_to(sdk).as_posix())
    import importlib.util
    spec = importlib.util.spec_from_file_location('task06_paths', sdk / 'tools/project_paths.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    # ProjectPaths accepts destination -> source. Preserve multiple old aliases
    # by adding them directly to its explicitly reviewed lookup afterward.
    rebinder = module.ProjectPaths(sdk / 'origin', {})
    rebinder.exports = {source: [Path(dest)] for source, dest in source_dest.items()}
    for language, old in foundation.items():
        for anchor in (sdk / 'origin' / old, sdk / 'origin/platform/ros' / language):
            rebinder.exports[anchor.resolve()] = [Path('include/platform/ros') / language]
    # These authored Go manifests already spell their final installed paths.
    # Their old positions do not make those paths relative to origin.
    for vendor in VENDORS:
        for language in ('go', 'tinygo'):
            manifest = sdk / f'origin/platform/{vendor}/{language}/include/go.mod'
            if manifest.is_file():
                anchor = sdk / f'origin/platform/{vendor}/{language}/include'
                rebinder.exports[anchor.resolve()] = [Path(f'include/platform/{vendor}/{language}')]
    include_names = {}
    for backend in ('ros', 'ros1', 'ros2'):
        for language in ('c', 'cpp'):
            for path in files(sdk / f'include/platform/{backend}/{language}'):
                if not path.is_symlink():
                    name = path.relative_to(sdk / f'include/platform/{backend}/{language}').as_posix()
                    include_names.setdefault(name, path.relative_to(sdk / 'include').as_posix())
    # These files are unchanged genuine emitter outputs relocated into a
    # supplemental source closure. Its generated imports were originally
    # relative to origin/custom-types/<family>, not the temporary capture slot.
    for dest, values in proposals.items():
        for source in values:
            if not any(part in source.parts for part in ('source-candidates', 'generated')):
                continue
            if source.suffix not in ('.mjs', '.ts'):
                continue
            for spelling in re.findall(r'\bfrom\s*["\x27]([^"\x27]+)["\x27]', source.read_text()):
                for old in ('javascript', 'assemblyscript'):
                    prefix = '../../' + old + '/'
                    if not spelling.startswith(prefix):
                        continue
                    tail = spelling.removeprefix(prefix)
                    target = sdk / 'origin' / old / tail
                    extension = '.ts' if source.suffix == '.ts' else ''
                    chosen = rebinder.destination(target, dest.removeprefix('sdk/'))
                    if chosen is None and extension:
                        chosen = rebinder.destination(Path(str(target) + extension), dest.removeprefix('sdk/'))
                        if chosen is not None:
                            chosen = chosen.with_suffix('')
                    if chosen is None:
                        raise ValueError('actual generated foundation dependency is absent: ' + str(target))
                    rebinder.exports[(source.parent / spelling).resolve()] = [chosen]
    divergent = []
    comparisons = []
    selected_outputs = {}
    decisions = {}
    for dest, values in sorted(proposals.items()):
        canonical = root / dest
        existing = canonical.read_bytes() if canonical.is_file() else None
        normalized = {}
        for source in values:
            if source.name == 'go.mod' and 'origin/platform' in source.as_posix():
                raw = source.read_bytes()
            else:
                raw = rebinder.transform(source, dest.removeprefix('sdk/'), source.read_bytes())
            # C include rebinding has no ABI/content transformation.
            if source.suffix in ('.h', '.hpp', '.c', '.cc', '.cpp'):
                text = raw.decode()
                for old_name, new_name in include_names.items():
                    text = text.replace('<' + old_name + '>', '<' + new_name + '>')
                def quoted_include(match):
                    target = (source.parent / match[2]).resolve()
                    target_dest = source_dest.get(target)
                    if target_dest is None:
                        return match[0]
                    if target_dest.startswith('include/'):
                        return match[1] + '<' + target_dest.removeprefix('include/') + '>'
                    relative = os.path.relpath(target_dest, Path(dest.removeprefix('sdk/')).parent)
                    return match[1] + '"' + relative + '"'
                text = re.sub(r'(^\s*#\s*include\s*)"([^"\n]+)"', quoted_include, text, flags=re.M)
                raw = text.encode()
            normalized[source] = raw
        by_hash = {}
        for source, raw in normalized.items():
            by_hash.setdefault(sha(raw), []).append(source.relative_to(root).as_posix())
        row = dict(path=dest, canonical_sha256=sha(existing) if existing is not None else None,
                   proposals=by_hash)
        if existing is not None and len(by_hash) == 1 and sha(existing) in by_hash:
            row['decision'] = 'identical-after-explicit-path-rebinding'
        elif existing is None and len(by_hash) == 1:
            row['decision'] = 'move-unique-source'
        else:
            row['decision'] = 'review-divergence'
            divergent.append(row)
            lines = []
            previous = existing
            previous_name = dest
            for source, raw in normalized.items():
                if previous is not None and previous != raw:
                    lines.extend(difflib.unified_diff(previous.decode().splitlines(True), raw.decode().splitlines(True),
                                                     fromfile=previous_name, tofile=source.relative_to(root).as_posix()))
                previous = raw
                previous_name = source.relative_to(root).as_posix()
            if lines:
                file = args.out / 'diffs' / (dest + '.diff')
                file.parent.mkdir(parents=True, exist_ok=True)
                file.write_text(''.join(lines))
        if args.emit:
            if len(by_hash) == 1:
                selected = next(iter(normalized.values()))
                decision = row['decision']
            elif dest.endswith('/operations.h'):
                source = next((source for source in values if 'source-candidates' in source.parts), values[0])
                selected = normalized[source]
                decision = 'merge-genuine-whole-family-with-explicit-public-includes'
            else:
                raise ValueError('unreviewed conflicting origin versions: ' + dest)
            if row['decision'] == 'review-divergence':
                allowed = (dest.endswith(('/state.h', '/guest.c', '/guest.cc', '/bounded_records.hpp', '/bounded_interfaces.h', '/operations.h'))
                           or dest.startswith('sdk/example/platform/flexiv/c/')
                           or dest.startswith('sdk/example/platform/flexiv/rs/')
                           or dest.endswith('/example-coverage.json'))
                if not allowed:
                    raise ValueError('unreviewed public/source divergence: ' + dest)
                decision = 'merge-current-source-docs-and-behavior-with-public-paths'
            selected_outputs[dest] = selected
            decisions[dest] = decision
        comparisons.append(row)
    if args.emit:
        if rebinder.missing:
            raise ValueError('required source-project dependency is unresolved')
        # Each closed app candidate contains the same latest application body
        # with explicitly canonical imports. It is consumed once, then retired.
        for name in ('five', 'dobot'):
            base = args.apps / name
            for source in files(base):
                if source.is_symlink():
                    raise ValueError('candidate symlink')
                dest = 'sdk/' + source.relative_to(base).as_posix()
                if dest.split('/')[3] not in VENDORS:
                    raise ValueError('candidate crosses owner scope')
                selected_outputs[dest] = source.read_bytes()
                decisions[dest] = 'reconcile-current-closed-app-canonical-imports'
        changes = args.out / 'changes'
        output_rows = []
        preimages = []
        postimages = []
        script = ['#!/bin/sh', 'set -eu', 'test "$PWD" = /Users/liyinan/Documents/MacroModel/src/uwvm2-ros',
                  'task06_merge_root=/tmp/task06-single-source-20261022-r1/merge',
                  'shasum -a 256 -c "$task06_merge_root/before.sha256" >/dev/null']
        for dest, body in sorted(selected_outputs.items()):
            path = root / dest
            before = path.read_bytes() if path.is_file() else None
            file = changes / dest
            file.parent.mkdir(parents=True, exist_ok=True)
            file.write_bytes(body)
            output_rows.append(dict(path=dest, before_sha256=sha(before) if before is not None else None,
                                    sha256=sha(body), bytes=len(body), decision=decisions[dest]))
            if before is not None:
                preimages.append(sha(before) + '  ' + dest + '\n')
            else:
                script.append('if test -e ' + shlex.quote(dest) + '; then cmp -s ' + shlex.quote(dest)
                              + ' "$task06_merge_root/changes/' + dest + '"; fi')
            postimages.append(sha(body) + '  ' + dest + '\n')
        for record in records:
            if record['role'] != 'obsolete-link':
                preimages.append(record['old_sha256'] + '  ' + record['old_path'] + '\n')
        for dest in sorted(selected_outputs):
            script.append('mkdir -p ' + shlex.quote(str(Path(dest).parent)))
            script.append('cp -p "$task06_merge_root/changes/' + dest + '" ' + shlex.quote(dest))
        script.append('shasum -a 256 -c "$task06_merge_root/after.sha256" >/dev/null')
        for record in records:
            script.append('rm -- ' + shlex.quote(record['old_path']))
            if record['new_path']:
                record['new_sha256'] = sha(selected_outputs[record['new_path']])
                record['decision'] = decisions[record['new_path']]
        for vendor in VENDORS:
            for old in ('sdk/origin/platform/' + vendor, 'sdk/origin/robots/arms/' + KITS[vendor]):
                script.append('find ' + shlex.quote(old) + ' -depth -type d -empty -delete')
        for family in sorted((sdk / 'origin/custom-types').iterdir()):
            if any(family.name.startswith(vendor + '-') for vendor in VENDORS):
                script.append('find ' + shlex.quote(family.relative_to(root).as_posix()) + ' -depth -type d -empty -delete')
        script.append('shasum -a 256 -c "$task06_merge_root/after.sha256" >/dev/null')
        (args.out / 'before.sha256').write_text(''.join(preimages))
        (args.out / 'after.sha256').write_text(''.join(postimages))
        (args.out / 'apply-source-moves.sh').write_text('\n'.join(script) + '\n')
        save(args.out / 'canonical-outputs.json', output_rows)
    save(args.out / 'source-map.json', records)
    save(args.out / 'comparisons.json', comparisons)
    save(args.out / 'divergent.json', divergent)
    save(args.out / 'path-rebindings.json', rebinder.changes)
    save(args.out / 'unresolved-paths.json', sorted(set(rebinder.missing)))
    counts = {decision: sum(row['decision'] == decision for row in comparisons)
              for decision in ('identical-after-explicit-path-rebinding', 'move-unique-source', 'review-divergence')}
    result = dict(status='RECONCILIATION_INVENTORY_ONLY', old_items=len(records), destinations=len(comparisons),
                  counts=counts, unresolved_paths=len(set(rebinder.missing)), default_authority=[],
                  checkout_modified=False, compiler_cases=0, guest_cases=0, native_ros_cases=0)
    save(args.out / 'summary.json', result)
    print(json.dumps(result), flush=True)


if __name__ == '__main__':
    main()
