#!/usr/bin/env python3
"""Rewrite only retired source-layout spellings in the current Task06 recipes.

This one-time migration runs on an inert snapshot under the finite Linux owner.
Every rewritten module must be an actual file in the canonical SDK. No runtime
resolver accepts the retired spellings, and no executable module is evaluated.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex


KITS = {'elephant': 'elephant/mycobot', 'ufactory': 'ufactory/xarm',
        'dobot': 'dobot', 'flexiv': 'flexiv', 'realman': 'realman'}


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    p.add_argument('--sdk', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('original finite owner/common lock required')
    a.out.mkdir()
    sdk = a.sdk.resolve(strict=True)
    rows, before, after, edits = [], [], [], []

    def canonical(spelling):
        path = spelling.removeprefix('sdk/')
        if path.startswith('javascript/'):
            path = 'include/platform/ros/js/' + path.removeprefix('javascript/')
        elif path.startswith('custom-types/'):
            family, tail = path.removeprefix('custom-types/').split('/', 1)
            vendor = family.split('-', 1)[0]
            found = [sdk / f'include/platform/{vendor}/js/{dialect}/{family}/{tail}' for dialect in ('ros1', 'ros2')]
            found = [item for item in found if item.is_file() and not item.is_symlink()]
            if len(found) != 1:
                raise ValueError('exact full language/dialect family missing: ' + spelling)
            path = found[0].relative_to(sdk).as_posix()
        elif path.startswith('robots/arms/'):
            for vendor, kit in KITS.items():
                prefix = f'robots/arms/{kit}/examples/'
                if path.startswith(prefix):
                    path = f'example/platform/{vendor}/js/' + path.removeprefix(prefix)
                    break
        target = sdk / path
        if not target.is_file() or target.is_symlink() or not target.resolve().is_relative_to(sdk):
            raise ValueError('recipe dependency is not an actual SDK-local file: ' + spelling)
        return path

    for vendor in KITS:
        for source in sorted((sdk / f'example/platform/{vendor}/js').glob('javascript-*.json')):
            raw = source.read_bytes()
            recipe = json.loads(raw)
            if recipe['schema'] != 'uwvm-javascript-guest-recipes-v1':
                raise ValueError('unknown recipe grammar')
            for selected in [recipe['common'], *recipe.get('variants', {}).values()]:
                if 'entry' in selected:
                    selected['entry'] = canonical(selected['entry'])
                for module in selected.get('modules', []):
                    module['source'] = canonical(module['source'])
                    module['name'] = canonical(module['name'])
                    if module['source'] != module['name']:
                        raise ValueError('source/module identity differs')
            body = (json.dumps(recipe, indent=2, sort_keys=True) + '\n').encode()
            relative = source.relative_to(sdk).as_posix()
            file = a.out / 'changes/sdk' / relative
            file.parent.mkdir(parents=True, exist_ok=True)
            file.write_bytes(body)
            before.append(sha(raw) + '  sdk/' + relative + '\n')
            after.append(sha(body) + '  sdk/' + relative + '\n')
            rows.append(dict(path='sdk/' + relative, before_sha256=sha(raw), sha256=sha(body),
                             actual_modules=len(recipe['common'].get('modules', []))))
            edits.append('cp -p "$task06_recipes_root/changes/sdk/' + relative + '" ' + shlex.quote('sdk/' + relative))
    (a.out / 'before.sha256').write_text(''.join(before))
    (a.out / 'after.sha256').write_text(''.join(after))
    (a.out / 'apply-recipe-edits.sh').write_text('\n'.join(['#!/bin/sh', 'set -eu',
        'test "$PWD" = /Users/liyinan/Documents/MacroModel/src/uwvm2-ros',
        'task06_recipes_root=/tmp/task06-single-source-20261022-r1/recipes',
        'shasum -a 256 -c "$task06_recipes_root/before.sha256" >/dev/null', *edits,
        'shasum -a 256 -c "$task06_recipes_root/after.sha256" >/dev/null']) + '\n')
    (a.out / 'summary.json').write_text(json.dumps(dict(status='CANONICAL_RECIPE_PATHS_VALIDATED',
        recipes=rows, sdk_builds=0, guest_cases=0, native_ros_cases=0), indent=2) + '\n')
    print(json.dumps(dict(status='CANONICAL_RECIPE_PATHS_VALIDATED', recipes=len(rows))), flush=True)


if __name__ == '__main__':
    main()
