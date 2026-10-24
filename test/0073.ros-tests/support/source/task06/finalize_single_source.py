#!/usr/bin/env python3
"""Retire proven redundant Task06 SDK sources after canonical reconciliation.

Only an inert, current five-vendor snapshot is read. Every deletion has a
canonical counterpart or a verified external original-source cache. Divergent
source bodies refuse removal. This program is run by the original Linux owner;
the emitted shell file performs only hash-guarded source edits in the checkout.
"""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import shutil


VENDORS = ('elephant', 'ufactory', 'dobot', 'flexiv', 'realman')
KITS = {'elephant': 'elephant/mycobot', 'ufactory': 'ufactory/xarm',
        'dobot': 'dobot', 'flexiv': 'flexiv', 'realman': 'realman'}


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + '\n')


def main():
    p = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--migration', type=Path, required=True)
    p.add_argument('--reference-cache', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    args = p.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('original finite owner/common lock required')
    args.out.mkdir()
    root, sdk = args.source, args.source / 'sdk'
    old_map = {row['old_path']: row['new_path'] for row in json.loads(args.migration.read_bytes()) if row['new_path']}
    planned, deleted, unresolved, references = {}, {}, [], []
    families = {}
    for vendor in VENDORS:
        for dialect in ('ros1', 'ros2'):
            for path in (sdk / f'include/platform/{vendor}/tinygo/{dialect}').iterdir():
                if path.is_dir():
                    families[path.name] = (vendor, dialect)

    def relative(path):
        return path.relative_to(root).as_posix()

    def replace_paths(text, output):
        vendor = output.parts[3] if output.parts[:3] in (('sdk', 'include', 'platform'), ('sdk', 'example', 'platform')) else None
        def record_module(match):
            family, name = match[1], match[2]
            owner, dialect = families[family]
            target = Path(f'sdk/include/platform/{owner}/rs/{dialect}/{family}/{name}')
            return os.path.relpath(target, output.parent)
        text = re.sub(r'(?:\.\./)+custom-types/([^/]+)/([^"\n]+\.rs)', record_module, text)
        if output.name == 'Cargo.toml':
            target = os.path.relpath('sdk/include/platform/ros/rs', output.parent)
            text = re.sub(r'(roswasm\s*=\s*\{[^}\n]*\bpath\s*=\s*")[^"]+("[^}\n]*\})',
                          lambda m: m[1] + target + m[2], text)
        if output.name == 'go.mod':
            foundation = os.path.relpath('sdk/include/platform/ros/tinygo', output.parent)
            text = re.sub(r'(replace\s+uwvm2\.local/roswasm\s*=>\s*)[^\s]+', lambda m: m[1] + foundation, text)
        if 'multi_vendor' in output.parts and output.suffix in ('.c', '.cc'):
            text = re.sub(r'(#\s*include\s*)"[^"]*entry\.h"', r'\1"entry.h"', text)
        if vendor and output.suffix == '.go':
            def go_record(match):
                profile = match[1]
                dialect = 'ros1' if profile.startswith(('ros1_', 'kinetic', 'melodic', 'noetic')) else 'ros2'
                if vendor == 'elephant':
                    family = 'elephant-mycobot-' + profile + '-complete'
                elif vendor == 'ufactory':
                    family = 'ufactory-xarm-' + profile.replace('_', '-') + '-complete'
                else:
                    family = vendor + '-' + profile.replace('_', '-') + '-complete'
                if family not in families:
                    raise ValueError('explicit complete Go family absent: ' + family)
                return f'uwvm2.local/{vendor}-records/{dialect}/{family}'
            text = re.sub(r'uwvm2\.local/' + re.escape(vendor) + r'-example/records_([A-Za-z0-9_]+)', go_record, text)
        # Literal source references, including old JSON module recipes and docs.
        if 'sdk/origin/' in text:
            for old in sorted(old_map, key=len, reverse=True):
                if old in text:
                    text = text.replace(old, old_map[old])
        text = text.replace('sdk/origin/coordination/20261021-director',
                            'test/0073.ros-tests/support/coordination/20261021-director')
        return text

    def normalized(raw, output):
        if output.suffix in ('.rs', '.go', '.toml', '.mod', '.json', '.md', '.lua', '.py', '.h', '.hpp', '.c', '.cc', '.mjs', '.ts'):
            return replace_paths(raw.decode(), output).encode()
        return raw

    def put(path, body, reason):
        name = relative(path)
        existing = path.read_bytes() if path.is_file() else None
        if existing != body:
            planned[name] = dict(body=body, before_sha256=sha(existing) if existing is not None else None, reason=reason)

    def retire(path, target, role, compare=True):
        name, output = relative(path), Path(relative(target))
        before = path.read_bytes()
        target_raw = planned.get(str(output), {}).get('body', target.read_bytes() if target.is_file() else None)
        if target_raw is None or (compare and normalized(before, output) != normalized(target_raw, output)):
            unresolved.append(dict(path=name, target=str(output), old_sha256=sha(before),
                                   target_sha256=sha(target_raw) if target_raw is not None else None, role=role))
            return
        deleted[name] = dict(old_sha256=sha(before), old_bytes=len(before), new_path=str(output),
                             new_sha256=sha(normalized(target_raw, output)), role=role,
                             decision='identical-source-after-explicit-canonical-path-rebinding')

    # Flatten obsolete distribution views. The authoritative application lives
    # at rs/ or tinygo/, and generated packages live under public include/ only.
    for vendor in VENDORS:
        examples = sdk / f'example/platform/{vendor}'
        for language, nested in (('rs', 'rust'), ('tinygo', 'go')):
            base = examples / language / nested
            for source in sorted(base.rglob('*')):
                if source.is_file():
                    target = examples / language / source.relative_to(base)
                    retire(source, target, 'obsolete-nested-application-view')
        tiny = examples / 'tinygo'
        for folder in sorted(tiny.glob('records_*')):
            if not folder.is_dir():
                continue
            profile = folder.name.removeprefix('records_')
            if vendor == 'elephant':
                family = 'elephant-mycobot-' + profile + '-complete'
            elif vendor == 'ufactory':
                family = 'ufactory-xarm-' + profile.replace('_', '-') + '-complete'
            else:
                family = vendor + '-' + profile.replace('_', '-') + '-complete'
            owner, dialect = families[family]
            for source in sorted(folder.rglob('*')):
                if source.is_file():
                    suffix = {'records.go': 'bounded_records.go', 'interfaces.go': 'bounded_interfaces.go'}[source.name]
                    target = sdk / f'include/platform/{owner}/tinygo/{dialect}/{family}/{suffix}'
                    retire(source, target, 'generated-codec-duplicate-in-example')
        # These historical Go views contain TinyGo applications. Preserve the
        # distinct new official standard-Go monitor and remove only proven copies.
        go = examples / 'go'
        for source in sorted(go.rglob('*')):
            if not source.is_file() or source.relative_to(go).parts[0] == 'official' or source.name == 'xmake.lua':
                continue
            tail = source.relative_to(go)
            if source.name == 'README.md' and (go / 'official').is_dir():
                continue
            if tail.parts[0] == 'go':
                tail = Path(*tail.parts[1:])
            target = tiny / tail
            if source.name == 'README.md':
                target = sdk / f'share/platform/{vendor}/language/go/legacy-example-notes.md'
                put(target, normalized(source.read_bytes(), Path(relative(target))), 'retain-unique-support-notes')
            retire(source, target, 'TinyGo-source-is-not-a-standard-Go-implementation')
        # One compiled C helper is shared by C and C++, rather than copied.
        for language in ('c', 'cpp'):
            support = examples / language / 'support/robots/arms' / KITS[vendor] / 'examples'
            for source in sorted(support.rglob('*')):
                if not source.is_file():
                    continue
                tail = source.relative_to(support)
                target = examples / 'c' / tail if tail.parts[0] != 'multi_vendor' else root / f'test/0073.ros-tests/support/source/task06/vendor/{vendor}' / tail
                retire(source, target, 'duplicate-application-helper')
        for language in ('c', 'cpp', 'py'):
            base = examples / language / 'multi_vendor'
            for source in sorted(base.rglob('*')):
                if source.is_file():
                    target = root / f'test/0073.ros-tests/support/source/task06/vendor/{vendor}/multi_vendor' / source.relative_to(base)
                    retire(source, target, 'isolated-peer-fixture-outside-installed-examples')

    # Canonical TinyGo module imports resolve the one public generated package.
    for vendor in VENDORS:
        module = sdk / f'include/platform/{vendor}/tinygo/go.mod'
        body = (f'module uwvm2.local/{vendor}-records\n\ngo 1.23\n\n'
                'require uwvm2.local/roswasm v0.0.0\n\n'
                'replace uwvm2.local/roswasm => ../../ros/tinygo\n').encode()
        put(module, body, 'one-public-TinyGo-record-package')
        manifest = sdk / f'example/platform/{vendor}/tinygo/go.mod'
        text = normalized(manifest.read_bytes(), Path(relative(manifest))).decode()
        if f'require uwvm2.local/{vendor}-records ' not in text:
            path = os.path.relpath(module.parent, manifest.parent)
            text += f'\nrequire uwvm2.local/{vendor}-records v0.0.0\nreplace uwvm2.local/{vendor}-records => {path}\n'
        put(manifest, text.encode(), 'application-depends-on-canonical-record-package')

    # Current sources are rebound once. Historical immutable receipts are not
    # in this snapshot and are never rewritten to claim a different old run.
    for base in (sdk / 'include/platform', sdk / 'example/platform', sdk / 'share/platform', sdk / 'tools/platform'):
        for vendor in VENDORS:
            for source in sorted((base / vendor).rglob('*')):
                if source.is_file() and relative(source) not in deleted:
                    current = planned.get(relative(source), {}).get('body', source.read_bytes())
                    body = normalized(current, Path(relative(source)))
                    if source.suffix == '.py':
                        ast.parse(body, filename=relative(source))
                    put(source, body, 'canonical-live-source-reference')

    # Original native projects are maintenance inputs in one explicit external
    # cache. The public SDK retains locks, official URLs and original licenses.
    args.reference_cache.mkdir(parents=True, exist_ok=True)
    for vendor in VENDORS:
        upstream = sdk / 'origin/robots/arms' / KITS[vendor] / 'upstream'
        cache = args.reference_cache / vendor
        cache.mkdir(exist_ok=True)
        repos, pins = [], []
        for source in sorted(upstream.rglob('*')):
            if not source.is_file() or source.is_symlink():
                continue
            tail, raw = source.relative_to(upstream), source.read_bytes()
            target = cache / tail
            target.parent.mkdir(parents=True, exist_ok=True)
            if target.is_file():
                if target.read_bytes() != raw:
                    raise ValueError('external original cache has a different unique input')
            else:
                shutil.copyfile(source, target)
            if sha(target.read_bytes()) != sha(raw):
                raise ValueError('original source cache readback mismatch')
            name = relative(source)
            deleted[name] = dict(old_sha256=sha(raw), old_bytes=len(raw), new_path=str(target), new_sha256=sha(raw),
                                 role='original-upstream-maintenance-reference', decision='verified-external-original-input-cache')
            pins.append(dict(path=tail.as_posix(), bytes=len(raw), sha256=sha(raw)))
            if source.name in ('capture.json', 'commit.json'):
                value = json.loads(raw)
                commit = value.get('commit')
                if isinstance(commit, dict):
                    commit = value.get('sha')
                repos.append(dict(path=tail.as_posix(), sha256=sha(raw), commit=commit,
                                  repository=value.get('repository'), url=value.get('url') or value.get('html_url'),
                                  tree=value.get('tree_git_sha1') or value.get('tree')))
            if source.name.casefold() in ('license', 'license.txt', 'license.md', 'copying', 'copying.txt'):
                license_path = sdk / f'share/platform/{vendor}/licenses/upstream' / tail
                put(license_path, raw, 'retain-original-upstream-license')
        reference = dict(schema='task06-upstream-reference-locks-v1', platform=vendor,
                         maintenance_cache=str(cache), ordinary_sdk_dependency=False,
                         repositories=repos, captured_files=pins,
                         source_policy='Official locked originals are inert external maintenance inputs; no native upstream program is installed or executed.')
        reference_path = sdk / f'share/platform/{vendor}/upstream-references.json'
        put(reference_path, (json.dumps(reference, indent=2, sort_keys=True) + '\n').encode(), 'compact-original-source-provenance')
        references.append(dict(platform=vendor, files=len(pins), cache=str(cache), logical_bytes=sum(pin['bytes'] for pin in pins)))

    save(args.out / 'unresolved.json', unresolved)
    if unresolved:
        save(args.out / 'summary.json', dict(status='UNIQUE_SOURCE_REQUIRES_REVIEW', unresolved=len(unresolved),
                                           originals_cached=references, checkout_modified=False))
        raise ValueError('unique canonical duplicate differences require review before source removal')

    rows, before, after, commands = [], [], [], []
    changes = args.out / 'changes'
    for name, item in sorted(planned.items()):
        target = changes / name
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(item['body'])
        if item['before_sha256']:
            before.append(item['before_sha256'] + '  ' + name + '\n')
        else:
            commands.append('test ! -e ' + shlex.quote(name))
        after.append(sha(item['body']) + '  ' + name + '\n')
        rows.append(dict(path=name, before_sha256=item['before_sha256'], sha256=sha(item['body']),
                         bytes=len(item['body']), decision=item['reason']))
    for name, item in sorted(deleted.items()):
        before.append(item['old_sha256'] + '  ' + name + '\n')
    script = ['#!/bin/sh', 'set -eu', 'test "$PWD" = /Users/liyinan/Documents/MacroModel/src/uwvm2-ros',
              'task06_finalize_root=/tmp/task06-single-source-20261022-r1/finalize',
              'shasum -a 256 -c "$task06_finalize_root/before.sha256" >/dev/null', *commands]
    for name in sorted(planned):
        script += ['mkdir -p ' + shlex.quote(str(Path(name).parent)),
                   'cp -p "$task06_finalize_root/changes/' + name + '" ' + shlex.quote(name)]
    script.append('shasum -a 256 -c "$task06_finalize_root/after.sha256" >/dev/null')
    script += ['rm -- ' + shlex.quote(name) for name in sorted(deleted)]
    for vendor in VENDORS:
        for base in (f'sdk/example/platform/{vendor}', 'sdk/origin/robots/arms/' + KITS[vendor]):
            script.append('find ' + shlex.quote(base) + ' -depth -type d -empty -delete')
    script.append('shasum -a 256 -c "$task06_finalize_root/after.sha256" >/dev/null')
    (args.out / 'before.sha256').write_text(''.join(before))
    (args.out / 'after.sha256').write_text(''.join(after))
    (args.out / 'apply-source-edits.sh').write_text('\n'.join(script) + '\n')
    save(args.out / 'changes.json', rows)
    save(args.out / 'deletions.json', [dict(old_path=name, **item) for name, item in sorted(deleted.items())])
    save(args.out / 'summary.json', dict(status='HASH_GUARDED_SOURCE_EDITS_READY', changed_files=len(rows),
                                       redundant_files=len(deleted), redundant_logical_bytes=sum(row['old_bytes'] for row in deleted.values()),
                                       original_reference_caches=references, unresolved=0,
                                       compiler_cases=0, guest_cases=0, native_ros_cases=0, default_authority=[]))
    print(json.dumps(dict(status='HASH_GUARDED_SOURCE_EDITS_READY', changed_files=len(rows), retired_files=len(deleted))), flush=True)


if __name__ == '__main__':
    main()
