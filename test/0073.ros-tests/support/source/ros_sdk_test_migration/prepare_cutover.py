"""Prepare a hash-checked source-only cutover; never mutate a checkout."""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import shlex
import sys


def require(value,message):
    if not value:raise ValueError(message)


def sha(path):
    with path.open('rb')as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--inventory',type=Path,required=True);p.add_argument('--sha256',required=True)
    p.add_argument('--tools',type=Path,required=True);a=p.parse_args()
    sys.path.insert(0,str(a.tools.resolve(strict=True)))
    from resource_contract import validate_resource_owner
    proof=validate_resource_owner();require(sha(a.inventory)==a.sha256,'exact reviewed inventory required')
    document=json.loads(a.inventory.read_bytes())
    require(document['existing_case_ids_to_preserve']==431,'431 pinned existing cases required')
    moves=[];aliases=[];links=[];sources={};destinations={}
    for row in document['files']:
        if row['disposition']=='REMOVE_GENERATED_TEST_ALIAS_AT_CUTOVER':links.append(row);continue
        if not row.get('destination'):continue
        for key in('source','destination'):
            path=PurePosixPath(row[key]);require(not path.is_absolute()and'..'not in path.parts,'confined source edit required')
        require(row['source'].startswith('sdk/')and row['destination'].startswith('test/0073.ros-tests/'),'source edit ownership escaped')
        require(row['kind']=='file','review non-file test alias separately')
        if row['disposition']=='EXACT_CONTENT_AND_PROVENANCE_ALIAS':aliases.append(row)
        elif row['destination']in destinations:
            require(destinations[row['destination']]['sha256']==row['sha256'],'distinct canonical collision')
            aliases.append(row)
        else:moves.append(row);destinations[row['destination']]=row
        sources[row['source']]=dict(destination=row['destination'],original_sha256=row['sha256'],
            original_bytes=row['bytes'],disposition=row['disposition'])
    require(all(row['destination']in destinations for row in aliases),'an alias has no actual canonical source')
    mapping=dict(kind='relocated-sdk-test-source-map-v1',status='PREPARED_NOT_APPLIED',
        inventory_sha256=a.sha256,source_capture_sha256=document['archive_sha256'],
        existing_case_ids=document['existing_case_ids'],existing_case_ids_preserved=431,
        historical_archives_modified=False,default_authority=[],sources=sources)
    Path('sdk-test-source-map.json').write_text(json.dumps(mapping,indent=2,sort_keys=True)+'\n')
    lines=['#!/bin/sh','set -eu',': "${1:?absolute checkout root required}"','cd "$1"',
        'test -d sdk/origin','test -f test/0073.ros-tests/index.json',
        '# Preflight every original and destination before the first source edit.',
        'pin_file() { test -f "$1" && test ! -L "$1" && test "$(/usr/bin/shasum -a 256 -- "$1" | cut -d " " -f 1)" = "$2"; }',
        'plain_parents() { p=$(dirname "$1"); while test "$p" != .; do test ! -L "$p"; p=$(dirname "$p"); done; }']
    q=shlex.quote
    for row in moves+aliases:
        lines.append('plain_parents '+q(row['source']))
        lines.append('plain_parents '+q(row['destination']))
        lines.append('pin_file '+q(row['source'])+' '+q(row['sha256']))
        lines.append('if test -e '+q(row['destination'])+'; then pin_file '+q(row['destination'])+' '+q(row['sha256'])+'; fi')
    for row in links:
        lines.append('test -L '+q(row['source'])+' && test "$(readlink '+q(row['source'])+')" = '+q(row['target']))
    lines.append('# Move authored sources and unchanged historical records; preserve distinct bytes.')
    for row in moves:
        lines.append('mkdir -p -- '+q(str(PurePosixPath(row['destination']).parent)))
        lines.append('if test -e '+q(row['destination'])+'; then rm -- '+q(row['source'])+'; else mv -- '+q(row['source'])+' '+q(row['destination'])+'; fi')
        lines.append('rmdir -- '+q(str(PurePosixPath(row['source']).parent))+' 2>/dev/null || :')
    for row in aliases:
        lines.append('pin_file '+q(row['destination'])+' '+q(row['sha256'])+' && rm -- '+q(row['source']))
        lines.append('rmdir -- '+q(str(PurePosixPath(row['source']).parent))+' 2>/dev/null || :')
    for row in links:lines.append('rm -- '+q(row['source']))
    directories=set()
    for row in moves+aliases+links:
        parent=PurePosixPath(row['source']).parent
        while parent!=PurePosixPath('sdk')and any(n.casefold()in('test','tests','qualification')for n in parent.parts):
            directories.add(str(parent));parent=parent.parent
    for directory in sorted(directories,key=lambda name:(-name.count('/'),name)):
        lines.append('if test -d '+q(directory)+'; then rmdir -- '+q(directory)+' 2>/dev/null || :; fi')
    lines.append('test ! -e sdk/test')
    Path('apply-source-moves.sh').write_text('\n'.join(lines)+'\n')
    result=dict(kind='sdk-test-source-cutover-plan-v1',status='PREPARED_NOT_APPLIED',resource=proof,
        inventory_sha256=a.sha256,source_map_sha256=sha(Path('sdk-test-source-map.json')),
        source_edit_script_sha256=sha(Path('apply-source-moves.sh')),ordinary_moves=len(moves),
        exact_aliases=len(aliases),generated_links_removed=len(links),existing_case_ids_preserved=431,
        historical_archives_modified=False,ready_for_live_cutover=False,
        pending=['rebind active source consumers','apply exact inline extraction','catalog and relocated acceptance'],
        actual_tests_executed=0,default_authority=[])
    Path('cutover-plan.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    print(json.dumps(result),flush=True)


if __name__=='__main__':main()
