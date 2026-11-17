"""Offline acquisition/security checks; execute only in the admitted cgroup."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import gzip
import io
import json
import tarfile
import tempfile
import unittest
from unittest.mock import patch
from urllib.error import HTTPError, URLError

import acquire
import reconcile
import scenarios


class SourceEvidenceTests(unittest.TestCase):
    def test_archive_wrapper_never_becomes_a_rosbuild_package_name(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            result = self.receipt(root, [('arbitrary-archive-branch/manifest.xml', b'<package/>', None)])
            self.assertEqual(result['packages'], [])
            self.assertEqual(len(result['unresolved_manifests']), 1)
            self.assertFalse(acquire.identified_package(dict(name='arbitrary-archive-branch', path='arbitrary-archive-branch/manifest.xml')))
            self.assertTrue(acquire.identified_package(dict(name='explicit', path='arbitrary-archive-branch/package.xml')))

    def test_sitemap_does_not_invent_sources_or_fetch_during_report(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            with patch.object(reconcile, 'fetch') as fetched:
                self.assertEqual(reconcile.sitemap_names(root), set())
            fetched.assert_not_called()
            (root / 'metadata').mkdir()
            (root / 'metadata/sitemap.xml').write_text('''<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">
              <url><loc>https://index.ros.org/p/example/</loc></url>
              <url><loc>https://foreign.invalid/p/untrusted/</loc></url>
              <url><loc>https://index.ros.org/p/..%2Fescape/</loc></url>
              <url><loc>https://index.ros.org/p/ignored/?query</loc></url>
            </urlset>''')
            self.assertEqual(reconcile.sitemap_names(root), {'example'})

    def test_concurrent_evidence_writers_publish_complete_json(self):
        with tempfile.TemporaryDirectory() as directory:
            target = Path(directory) / 'report.json'
            rows = [dict(writer=i, payload=str(i) * 10000) for i in range(12)]
            with ThreadPoolExecutor(max_workers=4) as pool:
                list(pool.map(lambda row: acquire.save(target, row), rows))
            self.assertIn(json.loads(target.read_text()), rows)
            self.assertEqual(list(Path(directory).glob('*.pending')), [])

    def test_rosbuild_dependency_attributes_are_preserved(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            result = self.receipt(root, [('legacy/sample/manifest.xml',
                b'<package><depend package="roscpp"/><rosdep name="boost"/></package>', None)])
            self.assertEqual(result['packages'][0]['name'], 'sample')
            self.assertEqual(result['packages'][0]['dependencies'], ['boost', 'roscpp'])

    def test_large_manifest_is_skipped_before_xml_parse(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'package.xml'
            with path.open('wb') as output:
                output.truncate((8 << 20) + 1)
            with patch.object(acquire.ET, 'fromstring') as parser:
                result = acquire.audit(Path(directory))
            parser.assert_not_called()
            self.assertEqual(result['packages'], [])
            self.assertEqual(len(result['skipped']), 1)

    def test_expansion_bound_rejects_declared_size_before_extracting(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            def fake_fetch(url, archive, limit):
                archive.parent.mkdir(parents=True)
                item = tarfile.TarInfo('large')
                item.size = (8 << 30) + 1
                archive.write_bytes(gzip.compress(item.tobuf() + b'\0' * 1024))
                return dict(url=url, sha256='fixture', bytes=archive.stat().st_size)
            row = dict(id='b' * 24, url='https://github.com/example/sample', vcs='git', ref='main', bindings=[])
            with patch.object(acquire, 'fetch', fake_fetch):
                result = acquire.acquire(root, row)
            self.assertEqual(result['status'], 'FAILED')
            self.assertIn('expanded archive exceeds acquisition bounds', result['error'])
            self.assertFalse((root / 'sources' / row['id'] / 'large').exists())

    def test_effects_choose_nearest_package_and_do_not_invent_shared_ownership(self):
        packages = [dict(name='outer', path='repo/package.xml'), dict(name='inner', path='repo/sub/package.xml')]
        self.assertEqual(scenarios.package_owner('repo/sub/main.cc', packages)['name'], 'inner')
        self.assertEqual(scenarios.package_owner('repo/main.cc', packages)['name'], 'outer')
        self.assertIsNone(scenarios.package_owner('shared/helpers.cc', packages))

    def test_transient_download_is_retried_with_fresh_digest(self):
        class PartialResponse(io.BytesIO):
            def read(self, size=-1):
                if self.tell():
                    raise URLError('TLS EOF after a partial body')
                return super().read(size)
        with tempfile.TemporaryDirectory() as directory:
            target = Path(directory) / 'input'
            with patch.object(acquire, 'urlopen', side_effect=[PartialResponse(b'partial'), io.BytesIO(b'complete')]) as opened:
                with patch.object(acquire.time, 'sleep'):
                    result = acquire.fetch('https://example.invalid/archive', target)
            self.assertEqual(opened.call_count, 2)
            self.assertEqual(target.read_bytes(), b'complete')
            self.assertEqual(result['bytes'], 8)
            self.assertEqual(result['sha256'], acquire.hashlib.sha256(b'complete').hexdigest())

    def test_missing_source_is_not_retried_as_transient(self):
        with tempfile.TemporaryDirectory() as directory:
            error = HTTPError('https://example.invalid/missing', 404, 'missing', {}, None)
            with patch.object(acquire, 'urlopen', side_effect=error) as opened:
                with self.assertRaises(HTTPError):
                    acquire.fetch(error.url, Path(directory) / 'input')
            self.assertEqual(opened.call_count, 1)
            self.assertFalse(reconcile.transient_failure('HTTPError: HTTP Error 404: Not Found'))

    def test_resource_bound_is_never_retried(self):
        with tempfile.TemporaryDirectory() as directory:
            with patch.object(acquire, 'urlopen', return_value=io.BytesIO(b'oversized')) as opened:
                with self.assertRaises(ValueError):
                    acquire.fetch('https://example.invalid/archive', Path(directory) / 'input', 3)
            self.assertEqual(opened.call_count, 1)

    def test_credentials_and_traversal_do_not_select_transport(self):
        for url in ('https://secret@github.com/owner/repo', 'http://github.com/owner/repo',
                    'https://github.com/owner/../repo', 'https://github.com/owner/repo?token=x'):
            with self.assertRaises(ValueError):
                acquire.archive_location(dict(url=url, vcs='git', ref='main'))

    def test_ref_remains_one_encoded_url_component(self):
        value = acquire.archive_location(dict(url='https://github.com/owner/repo.git', vcs='git', ref='release/a?b#c'))
        self.assertEqual(value, 'https://codeload.github.com/owner/repo/tar.gz/release%2Fa%3Fb%23c')
        with self.assertRaises(ValueError):
            acquire.archive_location(dict(url='https://unknown.invalid/owner/repo', vcs='git', ref='main'))

    def test_empty_rosindex_placeholder_is_not_a_source(self):
        sample = '''Repository Summary</h3><table>
        <tr><td><b>Checkout URI</b></td><td><a href=""></a></td></tr>
        <tr><td><b>VCS Type</b></td><td><span></span></td></tr>
        <tr><td><b>VCS Version</b></td><td><span></span></td></tr></table>'''
        self.assertEqual(reconcile.page_sources(sample), [])
        sample = sample.replace('href=""', 'href="https://github.com/a/b.git"').replace('<span></span>', '<span>git</span>', 1)
        sample = sample.replace('<span></span>', '<span>release/test</span>', 1)
        self.assertEqual(reconcile.page_sources(sample), [('https://github.com/a/b.git', 'git', 'release/test')])

    def receipt(self, root, members):
        def fake_fetch(url, archive, limit):
            archive.parent.mkdir(parents=True, exist_ok=True)
            with tarfile.open(archive, 'w:gz') as output:
                for name, contents, link in members:
                    item = tarfile.TarInfo(name)
                    if link is not None:
                        item.type = tarfile.SYMTYPE
                        item.linkname = link
                        output.addfile(item)
                    else:
                        item.size = len(contents)
                        output.addfile(item, io.BytesIO(contents))
            return dict(url=url, bytes=archive.stat().st_size, sha256='fixture')
        row = dict(id='a' * 24, url='https://github.com/example/sample', vcs='git', ref='main',
                   bindings=[dict(distro='fixture', repository='sample', packages=['expected'])])
        with patch.object(acquire, 'fetch', fake_fetch):
            return acquire.acquire(root, row)

    def test_archive_traversal_and_absolute_symlink_are_not_acquired(self):
        for members in ([('../escape', b'data', None)], [('root/link', b'', '/tmp/outside')]):
            with tempfile.TemporaryDirectory() as directory:
                root = Path(directory)
                result = self.receipt(root, members)
                self.assertEqual(result['status'], 'FAILED')
                self.assertFalse((root / 'escape').exists())

    def test_observed_manifest_and_missing_expected_package_are_separate(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            result = self.receipt(root, [('root/package.xml', b'<package><name>actual</name><depend>other</depend></package>', None),
                                         ('root/postinstall.sh', b'touch should_not_execute\n', None)])
            self.assertEqual(result['status'], 'ACQUIRED_AND_SCANNED')
            self.assertEqual(result['expected_packages_absent'], ['expected'])
            self.assertEqual(result['packages'][0]['name'], 'actual')
            self.assertEqual(result['semantic_review'], 'PENDING')
            self.assertFalse((root / 'should_not_execute').exists())
            self.assertIn('AUTOMATED_SOURCE_DISCOVERY', json.loads((root / 'scans' / ('a' * 24 + '.json')).read_text())['scope'])

    def test_reconciliation_does_not_infer_portability_from_observation(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            acquire.save(root / 'metadata/sitemap-packages.json', ['actual', 'placeholder'])
            acquire.save(root / 'catalog.json', dict(rosdistro_revision='a' * 40, packages={'expected': [], '': []},
                sources=[dict(id='a' * 24, bindings=[dict(packages=['expected'])])]))
            self.receipt(root, [('root/package.xml', b'<package><name>actual</name></package>', None)])
            report = reconcile.reconcile(root)
            self.assertEqual(report['indexed_packages_not_observed'], 1)
            self.assertEqual(report['invalid_index_package_names'], 1)
            self.assertEqual(report['rosindex_distribution_package_names'], 1)
            self.assertEqual(report['observed_package_names'], 1)
            self.assertFalse(report['complete_source_coverage'])
            self.assertEqual(report['portability'], 'NOT_TESTED')


if __name__ == '__main__':
    unittest.main()
