#!/usr/bin/env python3
"""Reconcile ROS source evidence without equating discovery with qualification.

This tool never builds or executes fetched sources. The primary acquisition can
continue concurrently: additional receipts, archives and scans live exclusively
under ``supplemental``. Existing successful snapshots are reused by identity.
"""
from __future__ import annotations

import argparse
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed
import hashlib
import html
import json
from pathlib import Path
import re
import time
from urllib.parse import quote, unquote, urlparse
import xml.etree.ElementTree as ET
import yaml

from acquire import acquire, archive_location, fetch, identified_package, save


def identity(url, ref):
    return hashlib.sha256((url + '\0' + ref).encode()).hexdigest()[:24]


def source_url(url):
    """Only upgrade known public forge schemes; never add credentials."""
    parsed = urlparse(url)
    if parsed.hostname in ('github.com', 'gitlab.com', 'bitbucket.org') and parsed.scheme in ('http', 'git'):
        return 'https://' + url.split('://', 1)[1]
    return url


def page_sources(text):
    """Read explicit repository-summary triples, skipping empty placeholders."""
    result = set()
    sections = re.split(r'Repository Summary</h3>', text)[1:]
    for section in sections:
        section = section.split('</table>', 1)[0]
        uri = re.search(r'Checkout URI</b></td>\s*<td>\s*<a[^>]*href="([^"]*)"', section)
        vcs = re.search(r'VCS Type</b></td>\s*<td>\s*<span[^>]*>([^<]*)', section)
        ref = re.search(r'VCS Version</b></td>\s*<td>\s*<span[^>]*>([^<]*)', section)
        if uri and vcs and uri[1].strip() and vcs[1].strip():
            result.add((source_url(html.unescape(uri[1].strip())), html.unescape(vcs[1].strip()),
                        html.unescape(ref[1].strip()) if ref and ref[1].strip() else 'HEAD'))
    return sorted(result)


def primary_receipts(root):
    return {p.stem: json.loads(p.read_text()) for p in (root / 'receipts').glob('*.json')}


def sitemap_names(root, fetch_missing=False):
    """Capture package page identities only; page URLs confer no source trust."""
    names_path = root / 'metadata/sitemap-packages.json'
    if names_path.exists():
        return set(json.loads(names_path.read_text()))
    sitemap_path = root / 'metadata/sitemap.xml'
    if not sitemap_path.exists():
        if not fetch_missing:
            return set()
        receipt = fetch('https://index.ros.org/sitemap.xml', sitemap_path, 64 << 20)
        save(root / 'metadata/sitemap-receipt.json', receipt)
    names = set()
    for entry in ET.fromstring(sitemap_path.read_bytes()).iter():
        if entry.tag.rsplit('}', 1)[-1] != 'loc' or not entry.text:
            continue
        url = urlparse(entry.text.strip())
        if url.scheme != 'https' or url.netloc != 'index.ros.org' or url.query or url.fragment:
            continue
        parts = url.path.split('/')
        if len(parts) == 4 and parts[0] == parts[3] == '' and parts[1] == 'p' and parts[2]:
            name = unquote(parts[2])
            if '/' not in name and name not in ('.', '..'):
                names.add(name)
    save(names_path, sorted(names))
    return names


def transient_failure(message):
    return any(word in message for word in ('URLError:', 'TimeoutError:', 'IncompleteRead:',
                                            'HTTP Error 429', 'HTTP Error 500', 'HTTP Error 502',
                                            'HTTP Error 503', 'HTTP Error 504', 'ConnectionError:'))


def distribution_records(root):
    result = {}
    for path in sorted((root / 'metadata').glob('*-[0-9].yaml')):
        distro = path.stem.rsplit('-', 1)[0]
        result.setdefault(distro, {}).update(yaml.safe_load(path.read_text()).get('repositories', {}))
    return result


def candidates(root, inspect_pages=False, jobs=6):
    catalog = json.loads((root / 'catalog.json').read_text())
    primary = primary_receipts(root)
    existing = {row['id']: row for row in catalog['sources']}
    supplemental = root / 'supplemental'
    rows, page_reports = {}, []

    def add(url, vcs, ref, kind, bindings, reason):
        url = source_url(url)
        key = identity(url, ref)
        if key in existing:
            return
        row = rows.setdefault(key, dict(id=key, url=url, ref=ref, vcs=vcs,
                                        source_kind=kind, bindings=[], recovery_reasons=[]))
        for binding in bindings:
            if binding not in row['bindings']:
                row['bindings'].append(binding)
        if reason not in row['recovery_reasons']:
            row['recovery_reasons'].append(reason)

    records = distribution_records(root)
    for key, receipt in primary.items():
        # An acquired tree can still omit expected packages or have submodules.
        if receipt['status'] == 'ACQUIRED_AND_SCANNED' and not receipt.get('expected_packages_absent'):
            continue
        row = receipt['source']
        if receipt['status'] != 'ACQUIRED_AND_SCANNED':
            # Retry newly supported forges in a separate output tree. Never
            # rewrite a primary receipt while its acquisition is running.
            try:
                archive_location(row)
                if 'adapter' in receipt.get('error', '') or 'non-HTTPS or non-Git' in receipt.get('error', ''):
                    rows.setdefault(key, dict(row, recovery_reasons=['new-archive-adapter']))
            except ValueError:
                pass
        for binding in row['bindings']:
            record = records.get(binding['distro'], {}).get(binding['repository'], {})
            for selection in ('source', 'doc'):
                src = record.get(selection)
                if src:
                    add(src['url'], src['type'], src.get('version') or 'HEAD',
                        'alternate-' + selection, [binding], key)
            release = record.get('release')
            if release and release.get('version'):
                version = release['version'].rsplit('-', 1)[0]
                add(release['url'], 'git', 'upstream/' + version, 'release-upstream-recovery', [binding], key)

    names = sitemap_names(root, fetch_missing=inspect_pages)
    missing = sorted(names - set(catalog['packages']))
    page_dir = supplemental / 'pages'

    def page(name):
        key = hashlib.sha256(name.encode()).hexdigest()[:24]
        report_path = page_dir / (key + '.json')
        if report_path.exists():
            previous = json.loads(report_path.read_text())
            if previous['status'] != 'UNRESOLVED':
                return previous
        result = dict(package=name, url='https://index.ros.org/p/' + quote(name, safe='') + '/',
                      status='UNRESOLVED', candidates=[])
        try:
            path = page_dir / (key + '.html')
            result['download'] = fetch(result['url'], path, 16 << 20)
            result['candidates'] = page_sources(path.read_text())
            result['status'] = 'EXPLICIT_SOURCE_METADATA' if result['candidates'] else 'NO_SOURCE_IN_REPOSITORY_SUMMARIES'
        except Exception as error:
            result['error'] = type(error).__name__ + ': ' + str(error)
        save(report_path, result)
        return result

    if inspect_pages:
        with ThreadPoolExecutor(max_workers=jobs) as pool:
            for result in pool.map(page, missing):
                page_reports.append(result)
                for url, vcs, ref in result['candidates']:
                    add(url, vcs, ref, 'rosindex-page-recovery',
                        [dict(distro='unspecified-by-page', repository=urlparse(url).path.rstrip('/').split('/')[-1],
                              packages=[result['package']])], result['url'])
    else:
        page_reports = [json.loads(p.read_text()) for p in page_dir.glob('*.json')]
        for result in page_reports:
            for url, vcs, ref in result['candidates']:
                add(url, vcs, ref, 'rosindex-page-recovery',
                    [dict(distro='unspecified-by-page', repository=urlparse(url).path.rstrip('/').split('/')[-1],
                          packages=[result['package']])], result['url'])

    output = dict(rosdistro_revision=catalog['rosdistro_revision'], primary_receipts_observed=len(primary),
                  candidates=list(rows.values()), pages=page_reports)
    save(supplemental / 'catalog.json', output)
    return output


def reconcile(root):
    """Produce auditable rows for each package name and source snapshot."""
    catalog = json.loads((root / 'catalog.json').read_text())
    primary = primary_receipts(root)
    supplemental = {p.stem: json.loads(p.read_text()) for p in (root / 'supplemental/receipts').glob('*.json')}
    recovered = {p.stem: json.loads(p.read_text()) for p in (root / 'recovery/receipts').glob('*.json')}
    all_receipts = dict(primary)
    for key, row in list(supplemental.items()) + list(recovered.items()):
        if key not in all_receipts or row['status'] == 'ACQUIRED_AND_SCANNED':
            all_receipts[key] = row
    expected, observed = defaultdict(set), defaultdict(set)
    unresolved_manifests = []
    for row in catalog['sources']:
        for binding in row['bindings']:
            for name in binding['packages']:
                expected[name].add(row['id'])
    for key, row in all_receipts.items():
        if row['status'] == 'ACQUIRED_AND_SCANNED':
            for package in row.get('packages', []):
                if identified_package(package):
                    observed[package['name']].add(key)
                else:
                    unresolved_manifests.append(dict(source_id=key, manifest=package['path'],
                        previous_scanner_name=package['name'], reason='archive wrapper is not a confirmed package identity'))
            unresolved_manifests.extend(dict(source_id=key, manifest=p['path'], reason=p['reason'])
                                        for p in row.get('unresolved_manifests', []))
    sitemap = sitemap_names(root)
    invalid_indexed = {name for name in catalog['packages'] if not name.strip()}
    indexed = set(catalog['packages']) - invalid_indexed
    names = sorted(indexed | sitemap | set(expected) | set(observed))
    rows = []
    for name in names:
        expected_sources = sorted(expected.get(name, ()))
        observed_sources = observed.get(name, set())
        direct_sources = sorted(set(expected_sources) & observed_sources)
        rows.append(dict(name=name, rosindex_distribution_entry=name in indexed, sitemap_page=name in sitemap,
                         expected_sources=expected_sources, observed_sources=sorted(observed_sources),
                         direct_expected_sources_observed=direct_sources,
                         status='SOURCE_OBSERVED' if observed_sources else 'SOURCE_NOT_YET_OBSERVED',
                         semantic_review='PENDING', portability='NOT_TESTED'))
    unresolved = [row for row in rows if row['status'] != 'SOURCE_OBSERVED']
    failures = [dict(source_id=k, url=v['source']['url'], ref=v['source']['ref'], vcs=v['source']['vcs'],
                     error=v.get('error')) for k, v in all_receipts.items() if v['status'] != 'ACQUIRED_AND_SCANNED']
    summary = dict(rosdistro_revision=catalog['rosdistro_revision'],
                   generated_at_utc=time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime()),
                   primary_source_snapshots_total=len(catalog['sources']), primary_source_receipts=len(primary),
                   primary_source_statuses=dict(Counter(r['status'] for r in primary.values())),
                   supplemental_source_receipts=len(supplemental),
                   supplemental_source_statuses=dict(Counter(r['status'] for r in supplemental.values())),
                   transient_recovery_receipts=len(recovered),
                   transient_recovery_statuses=dict(Counter(r['status'] for r in recovered.values())),
                   rosindex_distribution_package_names=len(indexed), sitemap_package_pages=len(sitemap),
                   rosindex_raw_distinct_name_entries=len(catalog['packages']),
                   invalid_index_package_names=len(invalid_indexed),
                   sitemap_captured=(root / 'metadata/sitemap-packages.json').exists(),
                   rosdistro_expected_package_names=len(expected), observed_package_names=len(observed),
                   indexed_packages_observed=len(indexed & observed.keys()),
                   indexed_packages_not_observed=len(indexed - observed.keys()),
                   sitemap_pages_without_distribution_entry=len(sitemap-indexed),
                   all_name_union=len(names), source_not_yet_observed=len(unresolved),
                   snapshots_with_submodule_declarations=sum(bool(v.get('submodules')) for v in all_receipts.values()),
                   unresolved_package_manifests=len(unresolved_manifests),
                   complete_source_coverage=False, semantic_review='PENDING', portability='NOT_TESTED',
                   notes=['Sitemap contains dependency-only/empty package pages; its count is not a runnable package count.',
                          'A package name observed in one snapshot does not qualify all distro variants or dependencies.',
                          'Automated regex scanning is not a semantic/security review.'])
    save(root / 'reconciliation/summary.json', summary)
    save(root / 'reconciliation/packages.json', rows)
    save(root / 'reconciliation/unresolved-packages.json', unresolved)
    save(root / 'reconciliation/source-failures.json', failures)
    save(root / 'reconciliation/unresolved-manifests.json', unresolved_manifests)
    save(root / 'reconciliation/invalid-index-entries.json', sorted(invalid_indexed))
    return summary


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--inspect-pages', action='store_true')
    parser.add_argument('--recover', action='store_true')
    parser.add_argument('--jobs', type=int, default=6)
    args = parser.parse_args()
    root = args.root.resolve()
    jobs = max(1, min(args.jobs, 8))
    if args.inspect_pages or args.recover:
        additional = candidates(root, args.inspect_pages, jobs)
        print('SUPPLEMENTAL_CANDIDATES', len(additional['candidates']), flush=True)
        if args.recover:
            with ThreadPoolExecutor(max_workers=jobs) as pool:
                for number, _ in enumerate(pool.map(lambda row: acquire(root / 'supplemental', row), additional['candidates']), 1):
                    if number % 25 == 0:
                        print('SUPPLEMENTAL_FINISHED', number, flush=True)
    print(json.dumps(reconcile(root), indent=2), flush=True)


if __name__ == '__main__':
    main()
