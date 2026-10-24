"""Capture exact API definitions, implement Python records and publish SDK views.

All source preparation precedes tests. Full native Python and source-view
checks follow implementation. Real Wasm execution belongs to the next lane.
This lane never executes original vendor programs or grants ROS authority.
"""
import argparse
import base64
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tarfile
import time
import urllib.error
import urllib.request


def facts(data): return dict(bytes=len(data), sha256=hashlib.sha256(data).hexdigest())


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, required=True); p.add_argument('--out', type=Path, required=True)
    a = p.parse_args(); root, out = a.root.resolve(strict=True), a.out
    inputs = json.loads((root/'inputs.json').read_bytes()); source = Path(inputs['source_root'])
    assert out.parent == root/'out' and not out.exists(); out.mkdir(mode=0o700)
    report = dict(status='RUNNING', commands=[], definitions=[], default_authority=[], official_cases_run=0,
                  native_driver_qualified=False, real_wasm_executed=False)
    def save(): (out/'unitreepython.json').write_text(json.dumps(report, indent=2)+'\n')
    def run(label, argv):
        with (out/(label+'.stdout')).open('xb') as stdout, (out/(label+'.stderr')).open('xb') as stderr:
            value = subprocess.run(list(map(str, argv)), stdin=subprocess.DEVNULL, stdout=stdout,
                stderr=stderr, env=dict(os.environ, PYTHONDONTWRITEBYTECODE='1'), timeout=180, check=False)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=value.returncode)); save()
        assert value.returncode == 0, (label, (out/(label+'.stderr')).read_text()[-2600:])
        return (out/(label+'.stdout')).read_bytes()
    def write(path, data):
        path.parent.mkdir(parents=True, exist_ok=True)
        assert not path.is_symlink() and path.parent.resolve().is_relative_to(source)
        if path.exists(): assert path.read_bytes() == data
        else:
            with path.open('xb') as stream: stream.write(data)
    try:
        # Recover only exact already-reviewed public view documents from the
        # retained, independently read-back prior source checkpoint.
        label = 'unitree-complete-sport-source-and-acceptance-r1'
        evidence = root.parent.parent/'recovery/ros-remaining-r60'
        retained = json.loads((evidence/(label+'.json')).read_bytes())
        client = json.loads((root/'control/client-readbacks'/(label+'-client-readback.json')).read_bytes())
        assert retained['status'] == 'TWO_FULL_OBJECT_CONTENT_MODE_AND_GZIP_EOF_READBACKS_PASS'
        assert client['status'] == 'CLIENT_STREAMED_INERT_OBJECTS_MODES_AND_GZIP_EOF_PASS'
        assert all(retained[k] == client[k] for k in ('bytes','sha256','manifest_sha256'))
        with Path(retained['primary']).open('rb') as stream:
            assert hashlib.file_digest(stream, 'sha256').hexdigest() == retained['sha256']
        with tarfile.open(retained['primary'], 'r|gz') as archive:
            first = next(iter(archive)); assert first.name == 'manifest.json' and first.size <= 16 << 20
            raw = archive.extractfile(first).read(); assert facts(raw)['sha256'] == retained['manifest_sha256']
            manifest = json.loads(raw)
        prior = dict(kind='checked-public-sdk-view-source-state-v1', default_authority=[], files={}, links={})
        for row in manifest['roles']:
            rel = Path(row['path'])
            if rel.parts[:3] not in (('source','sdk','platform'),('source','sdk','language')): continue
            dst = root/rel; name = rel.relative_to('source/sdk').as_posix()
            if row['type'] == 'regular':
                assert not dst.is_symlink() and facts(dst.read_bytes()) == {k:row[k] for k in ('bytes','sha256')}
                prior['files'][name] = {k:row[k] for k in ('bytes','sha256')}
            elif row['type'] == 'symlink-metadata':
                assert dst.is_symlink() and os.readlink(dst) == row['target']
                prior['links'][name] = row['target']
        (out/'previous-layout.json').write_text(json.dumps(prior, indent=2, sort_keys=True)+'\n')
        kit = source/'sdk/origin/robots/unitree'
        profile = json.loads((kit/'profiles/unitree_api.json').read_bytes())
        lock = json.loads((kit/'upstream-lock.json').read_bytes())
        tree = json.loads((kit/'upstream/example-review-r1/unitree_ros2/tree.json').read_bytes())
        git = module('unitree_python_complete_tree', source/'sdk/origin/tools/git_source_tree.py')
        expected = next(r for r in lock['repositories'] if r['repository'] == profile['repository'])
        assert git.tree_identity(tree) == expected['tree'] and expected['commit'] == profile['commit']
        original = kit/'upstream/python-api-r1/unitree_ros2/files'
        for row in profile['definitions']:
            locked = next(r for r in lock['files'] if (r['repository'],r['commit'],r['path']) ==
                          (profile['repository'],profile['commit'],row['path']))
            entry = next(r for r in tree['tree'] if r['path'] == row['path'])
            assert entry['type'] == 'blob' and entry['mode'] in ('100644','100755') and entry['size'] == locked['bytes'] <= 65536
            url = 'https://api.github.com/repos/'+profile['repository']+'/git/blobs/'+entry['sha']
            if (original/row['path']).is_file():
                body = (original/row['path']).read_bytes()
            for attempt in (() if (original/row['path']).is_file() else range(3)):
                try:
                    req = urllib.request.Request(url, headers={'User-Agent':'uwvm-unitree-complete-python'})
                    with urllib.request.urlopen(req, timeout=30) as response:
                        assert response.geturl() == url
                        encoded = response.read((1 << 20)+1)
                    assert len(encoded) <= 1 << 20
                    blob = json.loads(encoded)
                    assert blob['encoding'] == 'base64' and blob['size'] == entry['size']
                    body = base64.b64decode(blob['content'])
                    break
                except (urllib.error.URLError, TimeoutError, ConnectionError):
                    if attempt == 2: raise
                    time.sleep(.5*(attempt+1))
            assert facts(body) == {k:locked[k] for k in ('bytes','sha256')}
            assert hashlib.sha1(b'blob '+str(len(body)).encode()+b'\0'+body).hexdigest() == entry['sha']
            write(original/row['path'], body)
            report['definitions'].append(dict(path=row['path'], git_blob=entry['sha'], original_git_mode=entry['mode'], url=url, **facts(body))); save()
        materializer = module('unitree_python_materializer', kit/'materialize_catalog.py')
        materializer.prepare(original, kit/'profiles/unitree_api.json', kit/'upstream-lock.json', out/'materialized',
            catalog_format='uwvm-ros-vendor-complete-bounded-v1')
        # Reproduce the existing family's canonical JSON spelling without
        # altering either the retained preparation output or source semantics.
        catalog = json.loads((out/'materialized/catalog.preparation.json').read_bytes())
        (out/'materialized/catalog.python.json').write_text(json.dumps(catalog,indent=2,sort_keys=True)+'\n')
        run('complete-record-emission', [sys.executable,'-I','-S','-B',source/'sdk/origin/robots/vendors/python_records.py',
            '--source-root',source,'--catalog',out/'materialized/catalog.python.json',
            '--family',source/'sdk/origin/custom-types/unitree-api-complete','--out',out/'generated'])
        oracle = module('unitree_python_original_vectors', Path(__file__).with_name('unitree_python_vectors.py'))
        vectors = oracle.build(out/'materialized')
        (out/'vectors.json').write_text(json.dumps(vectors, indent=2)+'\n')
        run('complete-record-values', [sys.executable,'-I','-S','-B',Path(__file__).with_name('check_python_vendor.py'),
            '--sdk-python',source/'sdk/origin/python','--generated',out/'generated','--vectors',out/'vectors.json',
            '--out',out/'verification.json'])
        checker = Path(__file__).with_name('check_unitree_python.py')
        report['contract_line'] = run('sport-contract',[sys.executable,'-I','-S','-B',checker,'--source',source,
            '--generated',out/'generated','--mode','contract']).decode()
        raw = run('sport-values',[sys.executable,'-I','-S','-B',checker,'--source',source,
            '--generated',out/'generated','--mode','values'])
        sport_oracle = module('unitree_python_original_sport', Path(__file__).with_name('unitreesport_remaining.py'))
        upstream = kit/'upstream/example-review-r1/unitree_ros2/files/example/src'
        operations = sport_oracle.original_operations((upstream/'include/common/ros2_sport_client.h').read_text(),
                                                    (upstream/'src/common/ros2_sport_client.cpp').read_text())
        ordered = [(name,operations[name]) for name in re.findall(r'^UWVM_UNITREE_SPORT\((\w+),',
            (kit/'include/roswasm_unitree/sport_operations.inc').read_text(),re.M)]
        assert len(ordered) == len(operations) == 39
        sport_oracle.check_frames(raw, ordered)
        prefix = source/'sdk/origin/platform/unitree/python'; family = 'unitree-api-complete'; suffix = 'ros2/'+family
        assets = {'include/'+suffix+'/bounded_records.py':out/'generated/bounded_records.py',
            'include/'+suffix+'/generation.python.json':out/'generated/generation.python.json',
            'test/'+suffix+'/vectors.json':out/'vectors.json', 'test/'+suffix+'/verification.json':out/'verification.json'}
        asset_manifest = dict(kind='complete-vendor-python-sdk-v1', default_authority=[], implicit_permissions=[],
            native_driver_qualified=False, official_examples_ported=0,
            profiles=[dict(id='unitree-api', family=family, ros_version=2,
                state_type='unitree_api/msg/Response',command_type='unitree_api/msg/Request')], files={})
        for name, path in assets.items():
            body = path.read_bytes(); write(prefix/name, body); asset_manifest['files'][name] = facts(body)
        write(prefix/'sdk.python.json', (json.dumps(asset_manifest, indent=2, sort_keys=True)+'\n').encode())
        layout = module('unitree_python_public_layout', source/'sdk/origin/tools/sdk_layout.py')
        plan = layout.make_plan(source/'sdk/origin'); plan.apply(prior); report['layout'] = plan.check()
        run('source-layout-models',[sys.executable,'-I','-S','-B',Path(__file__).with_name('test_sdk_layout.py'),
            '--source',source,'--out',out/'layout-models'])
        report.update(status='COMPLETE_UNITREE_API_PYTHON_RECORDS_SPORT_AND_ASYNC_SOURCE_VIEWS_PASS',
            complete_records=8, sport_operations=39, complete_sport_frames=78,
            verification=json.loads((out/'verification.json').read_bytes()),
            previous_source_checkpoint=retained['sha256'])
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error)); raise
    finally: save()


if __name__ == '__main__': main()
