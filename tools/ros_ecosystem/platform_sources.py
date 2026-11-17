#!/usr/bin/env python3
"""Acquire SDK-declared platform sources as inert, immutable review evidence.

SDK registration is not qualification of a platform. Preserve exact commit
identities and metadata provenance. Do not infer a repository from a vendor
name, run downloaded code or change the SDK. Platform extension registration
requires a separate reviewed semantic contract and implementation.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import hashlib
import json
from pathlib import Path
import re
from urllib.parse import urlparse

from acquire import acquire, save
from capability_inventory import ROOT, guard, sha
from platform_archive import acquire_archive

METADATA = {'provider.json', 'profile.json', 'source-lock.json', 'source-mapping.json',
            'upstream-notices.json', 'upstream.json', 'sources.json', 'source-revisions.json', 'upstream-lock.json'}
BASE_PLATFORMS = {'ros', 'ros1', 'ros2', 'wasi'}


def repository(value):
    if not isinstance(value, str):
        return None
    if re.fullmatch(r'[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+', value):
        value = 'https://github.com/'+value
    parsed = urlparse(value)
    if parsed.scheme != 'https' or parsed.username or parsed.password or parsed.query or parsed.fragment:
        return None
    if parsed.hostname != 'github.com':
        return None
    parts = parsed.path.strip('/').removesuffix('.git').split('/')
    if len(parts) != 2 or any(not re.fullmatch(r'[A-Za-z0-9_.-]+', p) or p in ('.', '..') for p in parts):
        return None
    return 'https://github.com/'+'/'.join(parts)


def entries(value, path=''):
    if isinstance(value, dict):
        url = next((normalized for k in ('url', 'repository', 'repo', 'upstream_repository') if (normalized := repository(value.get(k)))), None)
        revision = next((value[k] for k in ('commit', 'revision', 'git_commit', 'source_commit', 'upstream_commit')
                         if isinstance(value.get(k), str) and re.fullmatch('[0-9a-f]{40}', value[k])), None)
        if url:
            yield dict(url=url, ref=revision, pointer=path,
                       unresolved_ref=None if revision else {k: value[k] for k in ('branch', 'tag', 'ref', 'version') if k in value})
        if (isinstance(value.get('url'),str) and value['url'].startswith('https://x2-aimdk.agibot.com/downloads/') and
                isinstance(value.get('archive_sha256'),str) and re.fullmatch('[0-9a-f]{64}',value['archive_sha256']) and
                isinstance(value.get('archive_bytes'),int) and not isinstance(value['archive_bytes'],bool) and
                isinstance(value.get('original_package_root'),str)):
            yield dict(url=value['url'],ref=value['archive_sha256'],pointer=path,vcs='archive',
                archive_sha256=value['archive_sha256'],archive_bytes=value['archive_bytes'],package_root=value['original_package_root'])
        for key, child in value.items():
            yield from entries(child, path+'/'+str(key).replace('~', '~0').replace('/', '~1'))
    elif isinstance(value, list):
        for index, child in enumerate(value):
            yield from entries(child, path+'/'+str(index))


def catalog(sdk, output):
    sources, platforms, missing, inputs = {}, [], [], {}
    adapters_path=Path(__file__).with_name('platform-source-adapters.json')
    adapters=json.loads(adapters_path.read_text())['repositories'];inputs[str(adapters_path.resolve())]=sha(adapters_path)
    revisions=[]
    for provider in sorted((sdk/'share/platform').glob('*/provider.json')):
        platform = provider.parent.name
        if platform in BASE_PLATFORMS:
            continue
        selected = set()
        for path in sorted(provider.parent.rglob('*.json')):
            if path.name not in METADATA:
                continue
            if path.stat().st_size > 32 << 20:
                raise ValueError('metadata exceeds explicit input bound: '+str(path))
            inputs[str(path)] = sha(path)
            document = json.loads(path.read_text())
            found=list(entries(document))
            for adapter in adapters:
                if adapter['platform']==platform and path.name=='source-lock.json':
                    revision=document.get(adapter['revision_key'])
                    if isinstance(revision,str) and re.fullmatch('[0-9a-f]{40}',revision):
                        found.append(dict(url=adapter['repository'],ref=revision,pointer='/'+adapter['revision_key'],
                            repository_evidence=adapter['primary_evidence'],repository_locator=str(adapters_path.resolve())))
            if path==provider:
                for key,revision in document.get('source_revisions',{}).items():
                    if isinstance(revision,str) and re.fullmatch('[0-9a-f]{40}',revision):
                        revisions.append((platform,revision,str(path),'/source_revisions/'+key))
            for item in found:
                provenance = dict(platform=platform, metadata=str(path), metadata_sha256=inputs[str(path)], pointer=item['pointer'])
                for key in ('repository_evidence','repository_locator'):
                    if key in item:provenance[key]=item[key]
                if item['ref'] is None:
                    missing.append(dict(**item, provenance=provenance))
                    continue
                identity = hashlib.sha256((item['url']+'\0'+item['ref']).encode()).hexdigest()[:24]
                source = sources.setdefault(identity, dict(id=identity, url=item['url'], ref=item['ref'], vcs=item.get('vcs','git'),
                    source_kind='sdk-platform', bindings=[], provenance=[]))
                if source['vcs']=='archive':
                    source['archive_sha256']=item['archive_sha256'];source['archive_bytes']=item['archive_bytes']
                    roots=source.setdefault('package_roots',[])
                    if item['package_root'] not in roots:roots.append(item['package_root'])
                if platform not in [p['platform'] for p in source['bindings']]:
                    source['bindings'].append(dict(platform=platform, packages=[]))
                source['provenance'].append(provenance)
                selected.add(identity)
        platforms.append(dict(platform=platform, provider_sha256=sha(provider), sources=sorted(selected),
            extension_module_pattern='uwvm_ros_v2.platform_extension.'+platform+'.v<major>',
            extension_registered=False, semantic_review='PENDING', native_qualification='NOT_TESTED'))
    # Child NVIDIA entries share exact commits with the umbrella provenance.
    # Join only a unique pinned repository identity; never infer from its name.
    by_platform={p['platform']:p for p in platforms}
    for platform,revision,metadata,pointer in revisions:
        matches=[s for s in sources.values() if s['vcs']=='git' and s['ref']==revision]
        if len(matches)!=1:continue
        source=matches[0]
        if source['id'] not in by_platform[platform]['sources']:by_platform[platform]['sources'].append(source['id'])
        if platform not in [p['platform'] for p in source['bindings']]:source['bindings'].append(dict(platform=platform,packages=[]))
        source['provenance'].append(dict(platform=platform,metadata=metadata,metadata_sha256=inputs[metadata],pointer=pointer,exact_commit_join=True))
    for name in ('platform_sources.py','platform_archive.py','acquire.py','capability_inventory.py'):
        path=Path(__file__).with_name(name).resolve();inputs[str(path)]=sha(path)
    record = dict(scope=__doc__, sdk=str(sdk), inputs=inputs, platforms=platforms,
        sources=list(sources.values()), unresolved_source_references=missing,
        inputs_unchanged=all(sha(p) == value for p, value in inputs.items()))
    assert record['inputs_unchanged']
    save(output/'catalog.json', record)
    return record


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--fetch', action='store_true')
    parser.add_argument('--jobs', type=int, default=2)
    parser.add_argument('--reuse',type=Path,help='Retain earlier pinned acquisition receipts; verify their archive and scan digests')
    args = parser.parse_args()
    guard()
    sdk, output = args.sdk.resolve(), args.output.resolve()
    assert output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    assert 1 <= args.jobs <= 4
    output.mkdir(parents=True, exist_ok=True)
    if (output/'catalog.json').exists():
        record = json.loads((output/'catalog.json').read_text())
        assert record['sdk'] == str(sdk)
        assert all(sha(p) == value for p, value in record['inputs'].items()), 'SDK metadata changed; start a separate evidence cohort'
    else:
        record = catalog(sdk, output)
    print(json.dumps(dict(platforms=len(record['platforms']), snapshots=len(record['sources']),
        platforms_without_pinned_sources=[p['platform'] for p in record['platforms'] if not p['sources']],
        unresolved_references=len(record['unresolved_source_references']))), flush=True)
    if not args.fetch:
        return
    completed = {}
    previous={}
    if args.reuse:
        reused=args.reuse.resolve();assert reused.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
        previous={item['source']['id']:item for item in json.loads((reused/'results.json').read_text()) if item['status']=='ACQUIRED_AND_SCANNED'}
    def obtain(source):
        old=previous.get(source['id'])
        if old and all(old['source'][key]==source[key] for key in ('url','ref','vcs')):
            archive=reused/'archives'/(source['id']+'.tar.gz');receipt=reused/'receipts'/(source['id']+'.json');scan=reused/'scans'/(source['id']+'.json')
            if archive.is_file() and sha(archive)==old['archive']['sha256'] and Path(old['source_directory']).is_dir():
                result=dict(old,source=source,reused_receipt=dict(path=str(receipt),sha256=sha(receipt),scan=str(scan),scan_sha256=sha(scan),archive=str(archive)),
                            extracted_source_rescanned=False)
                save(output/'receipts'/(source['id']+'.json'),result);return result
        return acquire_archive(output,source) if source['vcs']=='archive' else acquire(output,source)
    def progress():
        result = dict(complete=len(completed) == len(record['sources']), total=len(record['sources']), finished=len(completed),
            acquired=sum(r['status'] == 'ACQUIRED_AND_SCANNED' for r in completed.values()),
            failed=[dict(id=key, error=row.get('error')) for key, row in completed.items() if row['status'] != 'ACQUIRED_AND_SCANNED'],
            platforms_without_acquired_sources=[p['platform'] for p in record['platforms'] if not any(
                completed.get(identity,{}).get('status')=='ACQUIRED_AND_SCANNED' for identity in p['sources'])],
            all_platform_sources_present=False, semantic_review='PENDING', native_qualification='NOT_TESTED',
            catalogue_sha256=sha(output/'catalog.json'))
        save(output/'progress.json', result)
        return result
    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        pending = {executor.submit(obtain, source): source['id'] for source in record['sources']}
        for future in as_completed(pending):
            completed[pending[future]] = future.result()
            value = progress()
            if len(completed) % 10 == 0 or value['complete']:
                print(json.dumps({k: value[k] for k in ('complete', 'total', 'finished', 'acquired')}), flush=True)
    save(output/'results.json', [completed[key] for key in sorted(completed)])


if __name__ == '__main__':
    main()
