#!/usr/bin/env python3
"""Acquire indexed ROS source snapshots as data, then inventory external effects.

No downloaded build, hook, package or executable is run. An automated source
inventory is not a completed security review or a package portability result.
"""
from __future__ import annotations
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import hashlib
import http.client
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import ssl
import subprocess
import tarfile
import tempfile
import threading
import time
from urllib.parse import quote, urlparse
from urllib.error import HTTPError, URLError
from urllib.request import Request, urlopen
import xml.etree.ElementTree as ET
import yaml


def archive_location(row):
    """Select a documented forge archive transport without executing VCS code.

    The requested ref is preserved exactly. Historical Mercurial Bitbucket
    repositories usually no longer exist; an HTTP failure stays an explicit
    failure, never an inferred Git migration or successful empty repository.
    """
    parsed = urlparse(row['url'])
    if parsed.scheme != 'https' or parsed.username or parsed.password or parsed.query or parsed.fragment:
        raise ValueError('source requires credential-free HTTPS with no query or fragment')
    repo = parsed.path.removeprefix('/').removesuffix('.git')
    if not repo or '..' in repo.split('/'):
        raise ValueError('source repository path is not canonical')
    reference = quote(row['ref'], safe='')
    if parsed.hostname == 'github.com' and row['vcs'] == 'git':
        return 'https://codeload.github.com/' + repo + '/tar.gz/' + reference
    if (parsed.hostname in ('gitlab.com', 'gitlab.uni-koblenz.de', 'gitlab.freedesktop.org',
                            'git.ime.uni-luebeck.de') and row['vcs'] == 'git'):
        return 'https://' + parsed.hostname + '/' + repo + '/-/archive/' + reference + '/source.tar.gz'
    if parsed.hostname == 'bitbucket.org' and row['vcs'] in ('git', 'hg'):
        return 'https://bitbucket.org/' + repo + '/get/' + reference + '.tar.gz'
    raise ValueError('source host/VCS needs its own archive adapter: ' + str(parsed.hostname) + '/' + str(row['vcs']))


def save(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    # Reconciliation can run alongside acquisition/recovery. Each writer needs
    # its own temporary path; readers only ever observe a complete JSON file.
    with tempfile.NamedTemporaryFile(mode='w', encoding='utf-8', dir=path.parent,
                                     prefix=path.name + '.', suffix='.pending', delete=False) as output:
        temporary = Path(output.name)
        try:
            output.write(json.dumps(data, ensure_ascii=False, indent=2) + '\n')
            output.close()
            temporary.replace(path)
        finally:
            temporary.unlink(missing_ok=True)


def fetch(url, path, limit=64 << 20):
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + '.part')
    for attempt in range(3):
        digest = hashlib.sha256()
        count = 0
        try:
            with urlopen(Request(url, headers={'User-Agent': 'uwvm-ros-source-inventory'}), timeout=60) as response, temporary.open('wb') as output:
                while chunk := response.read(1 << 20):
                    count += len(chunk)
                    if count > limit:
                        raise ValueError('archive exceeds acquisition byte bound')
                    output.write(chunk)
                    digest.update(chunk)
            temporary.replace(path)
            return {'url': url, 'sha256': digest.hexdigest(), 'bytes': count}
        except HTTPError as error:
            if error.code != 429 and not 500 <= error.code < 600:
                raise
            if attempt == 2:
                raise
        except (URLError, TimeoutError, ConnectionError, ssl.SSLError, http.client.IncompleteRead):
            if attempt == 2:
                raise
        time.sleep(1 + attempt * 2)


def catalog(root):
    metadata = root / 'metadata'
    revision = subprocess.check_output(['git', 'ls-remote', 'https://github.com/ros/rosdistro.git', 'HEAD'], timeout=60, text=True).split()[0]
    if not re.fullmatch('[0-9a-f]{40}', revision):
        raise ValueError('invalid rosdistro commit')
    base = 'https://raw.githubusercontent.com/ros/rosdistro/' + revision + '/'
    pins = [fetch(base + 'index-v4.yaml', metadata / 'index.yaml')]
    index = yaml.safe_load((metadata / 'index.yaml').read_text())
    rows, packages, failures = {}, {}, []

    def distro(item):
        name, entry = item
        data = {}
        identities = []
        for number, relative in enumerate(entry['distribution']):
            path = metadata / (name + '-' + str(number) + '.yaml')
            identities.append(fetch(base + relative, path))
            data.update(yaml.safe_load(path.read_text())['repositories'])
        package_path = metadata / (name + '-packages.json')
        try:
            identities.append(fetch('https://index.ros.org/search/packages/data.' + name + '.json', package_path))
            discovered = json.loads(package_path.read_text())
        except Exception as error:
            discovered = []
            failures.append({'distro': name, 'stage': 'rosindex-package-list', 'error': str(error)})
        return name, data, discovered, identities

    with ThreadPoolExecutor(max_workers=6) as executor:
        for result in executor.map(distro, index['distributions'].items()):
            name, repos, indexed, identities = result
            pins.extend(identities)
            package_by_repo = {}
            for package in indexed:
                package_by_repo.setdefault(package['repo'], set()).add(package['package'])
                packages.setdefault(package['package'], []).append({'distro': name, 'repository': package['repo'], 'index_url': package.get('url')})
            for repository, record in repos.items():
                source = record.get('source') or record.get('doc')
                source_kind = 'source' if record.get('source') else 'doc'
                expected = set(record.get('release', {}).get('packages', [])) | package_by_repo.get(repository, set())
                if not expected and record.get('release'):
                    expected.add(repository)
                if source is None and record.get('release'):
                    release = record['release']
                    release_version = (release.get('version') or '').rsplit('-', 1)[0]
                    source = {'type': 'git', 'url': release['url'], 'version': 'upstream/' + release_version if release_version else 'upstream'}
                    source_kind = 'release-upstream'
                if not source:
                    failures.append({'distro': name, 'repository': repository, 'stage': 'source-location', 'packages': sorted(expected)})
                    continue
                url = source['url']
                if url.startswith('git://github.com/'):
                    url = 'https://' + url[6:]
                version = source.get('version') or 'HEAD'
                key = hashlib.sha256((url + '\0' + version).encode()).hexdigest()[:24]
                row = rows.setdefault(key, {'id': key, 'url': url, 'ref': version, 'vcs': source['type'], 'source_kind': source_kind, 'bindings': []})
                row['bindings'].append({'distro': name, 'repository': repository, 'packages': sorted(expected)})
    output = {'rosdistro_revision': revision, 'fetched_at_utc': time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime()),
              'index_package_count': len(packages), 'source_snapshot_count': len(rows),
              'packages': packages, 'sources': list(rows.values()), 'metadata_pins': pins, 'failures': failures}
    save(root / 'catalog.json', output)
    return output


PATTERNS = {
    'network.socket': r'\b(socket|socketpair|accept4?|bind|listen|connect|sendto|recvfrom|getaddrinfo)\s*\(|\b(?:boost::)?asio::|QTcpSocket|QUdpSocket',
    'network.web': r'\b(curl_easy_|requests\.(?:get|post|put)|urllib|WebSocket|websocket|grpc\b|httplib)',
    'filesystem': r'\b(fopen|openat|opendir|mkdir|unlink|rename|mmap|fstream|ifstream|ofstream)\s*\(|\b(?:std|boost)::filesystem|\bopen\s*\(',
    'process.execution': r'\b(system|popen|execv\w*|posix_spawn\w*|fork|subprocess\.\w+)\s*\(',
    'plugin.loading': r'\b(dlopen|dlsym|LoadLibrary\w*)\s*\(|pluginlib::|class_loader::',
    'device.io': r'\b(ioctl|libusb_\w+|serial::|termios|v4l2|SocketCAN)\b|/dev/',
    'model.inference': r'\b(onnxruntime|TensorRT|torch|tensorflow|openai|llama_cpp|vllm|transformers)\b',
    'ros.wait': r'\b(rcl_wait|rmw_wait|create_(?:wall_)?timer|create_callback_group|CallbackQueue|AsyncSpinner)\b',
    'ros.parameters': r'\b(set_parameters_atomically|add_on_set_parameters_callback|dynamic_reconfigure|getCached|declare_parameter)\b',
    'ros.communication': r'\b(create_publisher|create_subscription|create_generic_subscription|create_client|create_service|advertise|subscribe)\s*[<(]',
    'ros.actions': r'\b(rclcpp_action|actionlib|ServerGoalHandle|SimpleActionServer|SimpleActionClient)\b',
    'ros.time_transform': r'\b(tf2_ros|lookupTransform|waitForTransform|use_sim_time|onTimeJump|TimeSource)\b',
}
REGEX = {name: re.compile(pattern) for name, pattern in PATTERNS.items()}
TEXT = {'.h', '.hpp', '.hxx', '.c', '.cc', '.cpp', '.cxx', '.py', '.rs', '.go', '.java', '.js', '.ts', '.lua', '.xml', '.yaml', '.yml', '.launch', '.cmake', '.sh'}


def identified_package(package):
    """Exclude ambiguous root names emitted by the initial scanner revision.

    A forge archive's wrapper directory is not a rosbuild checkout package
    name. New scans annotate actual identity evidence; old root fallbacks are
    conservatively left unresolved rather than guessed from release metadata.
    """
    if package.get('name_origin') in ('manifest_name', 'rosbuild_package_directory'):
        return True
    path = PurePosixPath(package['path'])
    return not (len(path.parts) <= 2 and package['name'] == path.parent.name)


def audit(folder):
    packages, findings, counts, skipped, unresolved_manifests = [], [], {}, [], []
    files = 0
    for path in sorted(folder.rglob('*')):
        if path.is_symlink() or not path.is_file():
            continue
        relative = path.relative_to(folder).as_posix()
        if path.stat().st_size > 8 << 20:
            skipped.append({'path': relative, 'reason': 'file exceeds scan bound'})
            continue
        if path.name in ('package.xml', 'manifest.xml'):
            try:
                tree = ET.fromstring(path.read_bytes())
                dependencies = {x.text.strip() for x in tree
                                if x.text and x.tag.endswith('depend') and x.text.strip()}
                # rosbuild manifests store dependency names in attributes.
                dependencies.update(x.attrib['package'] for x in tree.findall('depend') if x.attrib.get('package'))
                dependencies.update(x.attrib['name'] for x in tree.findall('rosdep') if x.attrib.get('name'))
                name = (tree.findtext('name') or '').strip()
                origin = 'manifest_name'
                if not name and path.name == 'manifest.xml' and len(PurePosixPath(relative).parts) > 2:
                    name, origin = path.parent.name, 'rosbuild_package_directory'
                if name:
                    packages.append({'name': name, 'path': relative, 'name_origin': origin,
                                     'dependencies': sorted(dependencies)})
                else:
                    unresolved_manifests.append({'path': relative, 'dependencies': sorted(dependencies),
                                                 'reason': 'no explicit name; archive wrapper is not a package identity'})
            except Exception as error:
                skipped.append({'path': relative, 'reason': 'package XML: ' + str(error)})
        if path.suffix.lower() not in TEXT and path.name != 'CMakeLists.txt':
            continue
        raw = path.read_bytes()
        if b'\0' in raw:
            continue
        files += 1
        for number, line in enumerate(raw.decode('utf-8', 'replace').splitlines(), 1):
            for category, pattern in REGEX.items():
                if pattern.search(line):
                    counts[category] = counts.get(category, 0) + 1
                    findings.append({'category': category, 'file': relative, 'line': number, 'text': line.strip()[:400]})
    return {'scope': 'AUTOMATED_SOURCE_DISCOVERY_REQUIRES_SEMANTIC_REVIEW', 'source_files_scanned': files,
            'packages': packages, 'categories': counts, 'findings': findings, 'skipped': skipped,
            'unresolved_manifests': unresolved_manifests,
            'submodules_declared': [str(p.relative_to(folder)) for p in folder.rglob('.gitmodules')]}


def acquire(root, row):
    receipt_path = root / 'receipts' / (row['id'] + '.json')
    if receipt_path.exists():
        previous = json.loads(receipt_path.read_text())
        if previous.get('status') == 'ACQUIRED_AND_SCANNED':
            return previous
    result = dict(source=row, status='FAILED')
    folder = root / 'sources' / row['id']
    archive = root / 'archives' / (row['id'] + '.tar.gz')
    try:
        archive_url = archive_location(row)
        result['archive'] = fetch(archive_url, archive, 4 << 30)
        if folder.exists():
            shutil.rmtree(folder)  # Only this script's content-addressed private output.
        folder.mkdir(parents=True)
        with tarfile.open(archive, 'r:gz') as source:
            members, expanded = [], 0
            for member in source:
                expanded += member.size
                if member.size < 0 or expanded > 8 << 30 or len(members) >= 1000000:
                    raise ValueError('expanded archive exceeds acquisition bounds')
                name = PurePosixPath(member.name)
                if name.is_absolute() or '..' in name.parts or not name.parts:
                    raise ValueError('archive path escapes source directory')
                if not (member.isfile() or member.isdir() or member.issym() or member.islnk()):
                    raise ValueError('archive contains a special file')
                members.append(member)
            source.extractall(folder, members=members, filter='data')
        review = audit(folder)
        save(root / 'scans' / (row['id'] + '.json'), review)
        expected = {p for binding in row['bindings'] for p in binding['packages']}
        observed = {p['name'] for p in review['packages']}
        result.update(status='ACQUIRED_AND_SCANNED', packages=review['packages'], categories=review['categories'],
                      expected_packages_absent=sorted(expected-observed), source_files_scanned=review['source_files_scanned'],
                      unresolved_manifests=review['unresolved_manifests'],
                      skipped_files=len(review['skipped']), submodules=review['submodules_declared'],
                      semantic_review='PENDING', source_directory=str(folder))
    except Exception as error:
        result['error'] = type(error).__name__ + ': ' + str(error)
    save(receipt_path, result)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--jobs', type=int, default=8)
    parser.add_argument('--catalog-only', action='store_true')
    args = parser.parse_args()
    root = args.output.resolve()
    root.mkdir(parents=True, exist_ok=True)
    data = json.loads((root/'catalog.json').read_text()) if (root/'catalog.json').exists() else catalog(root)
    print('CATALOG', data['index_package_count'], 'package names;', data['source_snapshot_count'], 'source snapshots', flush=True)
    if args.catalog_only:
        return
    results = []
    with ThreadPoolExecutor(max_workers=max(1, min(args.jobs, 16))) as executor:
        pending = {executor.submit(acquire, root, row): row for row in data['sources']}
        for future in as_completed(pending):
            results.append(future.result())
            counts = {}
            packages = set()
            for row in results:
                counts[row['status']] = counts.get(row['status'], 0) + 1
                packages.update(p['name'] for p in row.get('packages', []))
            summary = {'sources_finished': len(results), 'sources_total': len(pending), 'statuses': counts,
                       'observed_package_names': len(packages), 'indexed_package_names': data['index_package_count'],
                       'semantic_review': 'PENDING', 'complete_source_coverage': False}
            save(root/'progress.json', summary)
            if len(results) % 25 == 0 or len(results) == len(pending):
                print(json.dumps(summary), flush=True)
    save(root/'results.json', results)


if __name__ == '__main__':
    main()
