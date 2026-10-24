"""Implement four complete Unitree Python families before their source tests.

Original definitions are captured from pinned vendor Git trees and checked with
both Git blob identities and the existing SHA256/extent lock. All 54 complete
records and actual per-record lessons are implemented before value contracts
run. Publication upgrades only exact previously checked, caller-owned assets.
Native Python representation checks do not replace actual Wasm/ROS execution.
"""
import argparse
import base64
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import tarfile
import urllib.request


def fact(body): return dict(bytes=len(body), sha256=hashlib.sha256(body).hexdigest())


def encoded(value): return (json.dumps(value, indent=2, sort_keys=True)+'\n').encode()


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
    kit = source/'sdk/origin/robots/unitree'; prefix = source/'sdk/origin/platform/unitree/python'
    report = dict(status='RUNNING', commands=[], profiles=[], definitions=[], default_authority=[],
        original_official_programs_ported=0, native_transport_qualified=False, real_wasm_executed=False)
    def save(): (out/'unitreefullpython.json').write_bytes(encoded(report))
    def run(label, argv):
        with (out/(label+'.stdout')).open('xb') as so, (out/(label+'.stderr')).open('xb') as se:
            value = subprocess.run(list(map(str, argv)), stdout=so, stderr=se, stdin=subprocess.DEVNULL,
                env=dict(os.environ, PYTHONDONTWRITEBYTECODE='1'), timeout=180, check=False)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=value.returncode)); save()
        assert value.returncode == 0, (label, (out/(label+'.stderr')).read_text()[-3000:])
        return (out/(label+'.stdout')).read_bytes()
    def write(path, body):
        assert not path.is_symlink() and path.parent.resolve().is_relative_to(source)
        path.parent.mkdir(parents=True, exist_ok=True)
        if path.exists(): assert path.read_bytes() == body, path
        else:
            with path.open('xb') as stream: stream.write(body)
    def get(url, limit):
        request = urllib.request.Request(url, headers={'User-Agent':'uwvm-unitree-full-python-source'})
        with urllib.request.urlopen(request, timeout=45) as response:
            body = response.read(limit+1)
        assert len(body) <= limit
        return body
    try:
        # Current views were independently checked after the root directory
        # migration. Capture their exact bytes before adding source lessons.
        prior = dict(kind='checked-public-sdk-view-source-state-v1', default_authority=[], files={}, links={})
        for top in ('language', 'platform'):
            for base, dirs, files in os.walk(source/'sdk/origin'/top, followlinks=False):
                for name in files+[n for n in dirs if (Path(base)/n).is_symlink()]:
                    path = Path(base)/name; relative = path.relative_to(source/'sdk/origin').as_posix()
                    if path.is_symlink(): prior['links'][relative] = os.readlink(path)
                    else: prior['files'][relative] = fact(path.read_bytes())
        (out/'previous-layout.json').write_bytes(encoded(prior))
        previous_raw = (prefix/'sdk.python.json').read_bytes()
        previous = json.loads(previous_raw)
        assert previous['kind']=='complete-vendor-python-sdk-v1' and len(previous['profiles'])==1
        assert previous['profiles'][0]['family']=='unitree-api-complete'
        (out/'previous-sdk.python.json').write_bytes(previous_raw)
        selections = json.loads((kit/'python/profiles.json').read_bytes())
        assert selections['kind']=='unitree-complete-python-source-selection-v1'
        assert selections['default_authority']==[] and sum(r['records'] for r in selections['profiles'])==54
        lock = json.loads((kit/'upstream-lock.json').read_bytes())
        git = module('unitree_full_python_git_tree', source/'sdk/origin/tools/git_source_tree.py')
        materializer = module('unitree_full_python_materializer', kit/'materialize_catalog.py')
        oracle = module('unitree_full_python_original_values', Path(__file__).with_name('unitree_python_vectors.py'))
        trees, originals = {}, {}
        for selected in selections['profiles']:
            profile = json.loads((kit/'profiles'/selected['profile']).read_bytes())
            repository = profile['repository']; repository_id = repository.split('/')[-1]
            pin = next(r for r in lock['repositories'] if r['repository']==repository)
            assert pin['commit']==profile['commit'] and profile['ros_version']==selected['ros_version']
            if repository not in trees:
                if repository_id=='unitree_ros2':
                    tree_raw = (kit/'upstream/example-review-r1/unitree_ros2/tree.json').read_bytes()
                else:
                    url='https://api.github.com/repos/'+repository+'/git/trees/'+pin['commit']+'?recursive=1'
                    tree_raw=get(url, 16<<20)
                tree=json.loads(tree_raw); assert git.tree_identity(tree)==pin['tree']
                trees[repository]={row['path']:row for row in tree['tree']}
                tree_path=out/(repository_id+'-tree.json'); tree_path.write_bytes(tree_raw)
                original=kit/'upstream/python-complete-r1'/repository_id/'files'
                original.mkdir(parents=True, exist_ok=True); originals[repository]=original
                if repository_id=='unitree_ros2':
                    # One small archive replaces dozens of network requests.
                    # Only exact locked .msg members are retained as source.
                    archive=out/'unitree-ros2-original.tar.gz'
                    archive.write_bytes(get('https://codeload.github.com/'+repository+'/tar.gz/'+pin['commit'],16<<20))
                    wanted={r['path'] for s in selections['profiles']
                        for q in [json.loads((kit/'profiles'/s['profile']).read_bytes())]
                        if q['repository']==repository for r in q['definitions']}
                    with tarfile.open(archive,'r:gz') as members:
                        for item in members:
                            relative='/'.join(item.name.split('/')[1:])
                            if relative not in wanted: continue
                            assert item.isfile() and 0<item.size<=65536
                            destination=original/relative; destination.parent.mkdir(parents=True,exist_ok=True)
                            write(destination,members.extractfile(item).read())
            original=originals[repository]
            for definition in profile['definitions']:
                name=definition['path']; row=trees[repository][name]
                locked=next(r for r in lock['files'] if (r['repository'],r['commit'],r['path'])==
                            (repository,profile['commit'],name))
                assert row['type']=='blob' and row['mode'] in ('100644','100755') and row['size']==locked['bytes']<=65536
                destination=original/name
                if destination.is_file(): body=destination.read_bytes()
                else:
                    url='https://api.github.com/repos/'+repository+'/git/blobs/'+row['sha']
                    blob=json.loads(get(url,1<<20)); assert blob['encoding']=='base64' and blob['size']==row['size']
                    body=base64.b64decode(blob['content']); destination.parent.mkdir(parents=True,exist_ok=True)
                    write(destination,body)
                assert fact(body)=={k:locked[k]for k in('bytes','sha256')}
                assert hashlib.sha1(b'blob '+str(len(body)).encode()+b'\0'+body).hexdigest()==row['sha']
                report['definitions'].append(dict(repository=repository,path=name,git_blob=row['sha'],**fact(body)))
            lane=out/selected['id']; lane.mkdir()
            materializer.prepare(original,kit/'profiles'/selected['profile'],kit/'upstream-lock.json',lane/'materialized',
                catalog_format='uwvm-ros-vendor-complete-bounded-v1')
            catalog=json.loads((lane/'materialized/catalog.preparation.json').read_bytes())
            (lane/'materialized/catalog.python.json').write_bytes(encoded(catalog))
            run(selected['id']+'-implement',[sys.executable,'-I','-S','-B',source/'sdk/origin/robots/vendors/python_records.py',
                '--source-root',source,'--catalog',lane/'materialized/catalog.python.json',
                '--family',source/'sdk/origin/custom-types'/selected['family'],'--out',lane/'generated'])
            proof=json.loads((lane/'generated/generation.python.json').read_bytes())
            assert len(proof['records'])==selected['records']
            vectors=oracle.build(lane/'materialized')
            assert len(vectors['cases'])==selected['records']
            (lane/'vectors.json').write_text(json.dumps(vectors,indent=2)+'\n')
            report['profiles'].append(dict(selected,lane=str(lane))); save()
        # All implementations and every authored lesson precede tests.
        examples=[]
        for selected in report['profiles']:
            lane=Path(selected['lane']); proof=json.loads((lane/'generated/generation.python.json').read_bytes())
            for record in proof['records']:
                typename=record['type']; relative='examples/python/records/ros'+str(selected['ros_version'])+'/'+typename.split('/')[0]+'/'+typename.split('/')[-1]+'.py'
                body=('"""Complete '+typename+' message lesson.\n\n'
                    'Import the actual complete generated Message class below. Its ordered\n'
                    'fields and all nested/array values match the pinned original definition.\n'
                    'Canonical encoding is bounded to '+str(record['minimum_wire_bytes'])+'..'+str(record['maximum_wire_bytes'])+' bytes; it is not CDR or a native ABI struct.\n\n'
                    'Run raw|state|command ALIAS after explicit family/type selection. Raw\n'
                    'shows numeric alias admission; state polls one complete value; command\n'
                    'opens the publisher first, then reads one u32LE extent and complete\n'
                    'canonical record from stdin and publishes exactly once. Follow the shared\n'
                    'record_example.py implementation for the direct Session/Node API steps.\n'
                    'No endpoint, topic, network, clock, retry or permission is selected here.\n'
                    'Default authority is empty. The operator supplies the exact native schema,\n'
                    'alias operation and finite quotas. Values are not robot safety approval.\n\n'
                    'For GO/HG LowCmd, command additionally requires explicit lowlevel=True\n'
                    'and zero named reserves. Legacy ROS1 uses its external bridge protocol.\n'
                    'This authored record lesson is not a complete original vendor program port.\n'
                    '"""\nfrom bounded_records import '+record['symbol']+' as Message\n'
                    'from roswasm.unitree_record_example import run\n\n\n'
                    'def main(*, lowlevel=False):\n'
                    '    """Admit one explicit alias and preserve primary/cleanup error status."""\n'
                    '    return run(Message, lowlevel=lowlevel)\n\n\n'
                    'if __name__ == "__main__":\n    raise SystemExit(main())\n').encode()
                destination=kit/relative; destination.parent.mkdir(parents=True,exist_ok=True); write(destination,body)
                examples.append(dict(family=selected['family'],ros_version=selected['ros_version'],
                    type=typename,symbol=record['symbol'],path=relative,**fact(body)))
        assert len(examples)==54 and len({r['path']for r in examples})==54
        inventory=dict(kind='complete-unitree-python-record-lessons-v1',default_authority=[],
            original_official_programs_ported=0,examples=examples)
        write(kit/'python/record_examples.json',encoded(inventory))
        report['all_source_implementations_finished_before_tests']=True; save()
        assets={};manifest_profiles=[]
        for selected in report['profiles']:
            lane=Path(selected['lane']); label=selected['id']
            run(label+'-whole-values',[sys.executable,'-I','-S','-B',Path(__file__).with_name('check_python_vendor.py'),
                '--sdk-python',source/'sdk/origin/python','--generated',lane/'generated','--vectors',lane/'vectors.json',
                '--out',lane/'verification.json'])
            selected['result']=json.loads((lane/'verification.json').read_bytes())
            selected['vendor']='unitree'
            if selected['id'] in ('unitree-go','unitree-hg'):
                common=[sys.executable,'-I','-S','-B',Path(__file__).with_name('check_unitree_lowlevel_python.py'),
                    '--source',source,'--generated',lane/'generated','--type',selected['command_type']]
                selected['lowlevel_contract_line']=run(label+'-crc-contract',common+['--mode','contract']).decode()
                run(label+'-crc-values',common+['--mode','values'])
            elif selected['ros_version']==1:
                run(label+'-legacy-crc-refusal',[sys.executable,'-I','-S','-B',Path(__file__).with_name('check_unitree_lowlevel_python.py'),
                    '--source',source,'--generated',lane/'generated','--type',selected['command_type'],'--mode','legacy-refusal'])
            suffix='ros'+str(selected['ros_version'])+'/'+selected['family']
            assets.update({'include/'+suffix+'/bounded_records.py':lane/'generated/bounded_records.py',
                'include/'+suffix+'/generation.python.json':lane/'generated/generation.python.json',
                'test/'+suffix+'/vectors.json':lane/'vectors.json','test/'+suffix+'/verification.json':lane/'verification.json'})
            manifest_profiles.append({k:selected[k]for k in('id','family','ros_version','state_type','command_type')})
        asset_manifest=dict(kind='complete-vendor-python-sdk-v1',default_authority=[],implicit_permissions=[],
            native_driver_qualified=False,official_examples_ported=0,profiles=manifest_profiles,files={})
        def install(path,body,expected=None):
            path.parent.mkdir(parents=True,exist_ok=True); assert not path.is_symlink()
            if path.exists() and path.read_bytes()!=body:
                old=path.read_bytes(); assert expected==fact(old),path
                backup=out/'previous-assets'/path.relative_to(prefix); backup.parent.mkdir(parents=True,exist_ok=True)
                backup.write_bytes(old); path.write_bytes(body)
            elif not path.exists():
                assert expected is None
                with path.open('xb')as stream:stream.write(body)
        for name,path in assets.items():
            body=path.read_bytes(); install(prefix/name,body,previous['files'].get(name)); asset_manifest['files'][name]=fact(body)
        install(prefix/'sdk.python.json',encoded(asset_manifest),fact(previous_raw))
        layout=module('unitree_full_python_public_layout',source/'sdk/origin/tools/sdk_layout.py')
        plan=layout.make_plan(source/'sdk/origin'); plan.apply(prior); report['layout']=plan.check()
        run('complete-public-layout-models',[sys.executable,'-I','-S','-B',Path(__file__).with_name('test_sdk_layout.py'),
            '--source',source,'--out',out/'layout-models'])
        report.update(status='FOUR_COMPLETE_UNITREE_PYTHON_FAMILIES_AND_FIFTY_FOUR_LESSONS_SOURCE_PASS',
            complete_records=54,record_lessons=54,portable_checksum_families=2)
        print(report['status'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error)); raise
    finally: save()


if __name__=='__main__':main()
