"""Refresh relocated case pins and retire test delivery declarations."""
import argparse
import ast
from collections import Counter
import gzip
import hashlib
import io
import json
import os
from pathlib import Path
import re
import sys
import tarfile

PREFIX='test/0073.ros-tests/'
SOURCE=PREFIX+'support/source/'
def require(ok,message):
    if not ok:raise ValueError(message)
def sha(raw):return hashlib.sha256(raw).hexdigest()
def encoded(value):return (json.dumps(value,indent=2,sort_keys=True)+'\n').encode()
def pure(source,names,namespace):
    tree=ast.parse(source)
    selected=[n for n in tree.body if isinstance(n,ast.FunctionDef)and n.name in names
        or isinstance(n,ast.Assign)and any(isinstance(t,ast.Name)and t.id in names for t in n.targets)]
    exec(compile(ast.fix_missing_locations(ast.Module(body=selected,type_ignores=[])),'selected-current-producer','exec'),namespace)
    return namespace

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--archive',type=Path,required=True)
    p.add_argument('--sha256',required=True);p.add_argument('--tools',type=Path,required=True)
    p.add_argument('--baseline-cases',type=int,required=True)
    p.add_argument('--expected-added',type=int,required=True)
    p.add_argument('--catalog-only',action='store_true',help='Refresh case/source-map pins only; SDK assembly metadata remains pending')
    p.add_argument('--register-source',action='append',default=[],metavar='SUPPORT_SOURCE',
        help='Explicit reviewed new support/source leaf; accepts actual Python or compiled test entry points')
    a=p.parse_args();sys.path.insert(0,str(a.tools))
    from resource_contract import validate_resource_owner
    proof=validate_resource_owner();require(Path.cwd()==Path(proof['run'])/'work','owned work required')
    require(sha(a.archive.read_bytes())==a.sha256,'exact inert metadata capture required')
    data={}
    with tarfile.open(a.archive,'r:*')as source:
        for item in source:
            path=Path(item.name)
            require(not path.is_absolute()and'..'not in path.parts,'confined metadata capture required')
            if item.isdir():continue
            require(item.isfile()and item.name not in data and item.size<=8<<20,'bounded unique ordinary source required')
            data[item.name]=source.extractfile(item).read()
    output={};links=[];changes=[]
    index_path=PREFIX+'index.json'
    baseline=json.loads(data[index_path]);cases=baseline['cases'];original_ids={r['id']for r in cases}
    registered=set(a.register_source)
    require(len(registered)==len(a.register_source),'unique explicit source registrations required')
    require(all(name.startswith('support/source/')and PREFIX+name in data
        and Path(name).as_posix()==name and '..'not in Path(name).parts
        and name not in{row['source']for row in cases}for name in registered),
        'exact captured new support source registrations required')
    require(len(cases)==len(original_ids)==a.baseline_cases,'exact current catalog baseline required')
    catalog=pure(data['sdk/tools/prepare_test_catalog.py'],
        {'LANG','ENTRYPOINTS','SHARED_PLATFORMS','VENDORS','test_role','platform'},
        dict(Path=Path,ast=ast,re=re,hashlib=hashlib))
    by_source={r['source']:r for r in cases};last={}
    for row in cases:
        key=(row['platform'],row['language'])
        last[key]=max(last.get(key,0),int(Path(row['id']).name.split('.',1)[0]))
    for name,raw in sorted(data.items()):
        if name.endswith('.py'):ast.parse(raw,filename=name)
        if not name.startswith(SOURCE):continue
        identity=name[len(PREFIX):];relative=Path(name[len(SOURCE):])
        if identity in by_source:
            row=by_source[identity]
            if row['source_sha256']!=sha(raw):
                changes.append(dict(source=identity,id=row['id'],before=row['source_sha256'],after=sha(raw)))
                row['source_sha256']=sha(raw);row['source_bytes']=len(raw)
                output[PREFIX+row['id']+'/case.json']=encoded(row)
        else:
            language=catalog['LANG'].get(relative.suffix)
            if registered and identity not in registered:continue
            if not registered and language!='py':continue
            if language is None:continue
            text=raw.decode();role=catalog['test_role'](relative,text)
            if role is None or not re.search(catalog['ENTRYPOINTS'][language],text):
                require(identity not in registered,'explicit registration must be an actual recognized test entry point')
                continue
            platform=catalog['platform'](relative);key=(platform,language);last[key]=last.get(key,0)+1
            stem=re.sub('[^a-z0-9._-]+','-',str(relative.with_suffix('')).lower().replace('/','-')).strip('-')
            folder=Path('platform')/platform/'language'/language/('%04d.%s'%(last[key],stem))
            row=dict(kind='numbered-ros-source-test-case-v1',id=folder.as_posix(),platform=platform,language=language,
                role=role,source=identity,source_bytes=len(raw),source_sha256=sha(raw),
                default_authority=[],execution_status='NOT_RUN_BY_CATALOG_PREPARATION',
                official_upstream_suite_credit=0,guest_language_qualification=False)
            cases.append(row);by_source[identity]=row;output[PREFIX+row['id']+'/case.json']=encoded(row)
            for filename,target in (('run.py','support/case_entry.py'),('README.md','support/CASE-GUIDE.md'),('source'+relative.suffix,identity)):
                links.append(dict(path=PREFIX+row['id']+'/'+filename,target=os.path.relpath(target,folder)))
    require(not registered or registered<={row['source']for row in cases},'all reviewed source registrations required')
    require(original_ids<={r['id']for r in cases}and len(cases)==a.baseline_cases+a.expected_added,
        'all existing IDs and exact declared additions required')
    counts=Counter((r['platform'],r['language'])for r in cases)
    baseline['cases']=sorted(cases,key=lambda r:r['source'])
    baseline['groups']=[dict(platform=p,language=l,cases=n)for(p,l),n in sorted(counts.items())]
    baseline['cases_prepared']=len(cases)
    baseline['source_pin_refresh']=dict(captured_metadata_sha256=a.sha256,changed_sources=changes,
        added_sources=[r['source']for r in cases if r['id']not in original_ids],actual_tests_executed=0)
    output[index_path]=encoded(baseline)
    mapping_path=PREFIX+'support/sdk-test-source-map.json'
    mapping=json.loads(data[mapping_path]);historical=json.loads(gzip.decompress(data[PREFIX+'support/records/sdk-test-migration/historical-record-index.json.gz']))
    require(set(mapping['existing_case_ids'])<={r['id']for r in cases},'all431 original identities required')
    for old,row in mapping['sources'].items():
        relative=row['destination'][len(PREFIX):]
        selected=by_source.get(relative)
        row['numbered_case_id']=selected['id']if selected else None
        if row['destination']in data:
            row['current_source_sha256']=sha(data[row['destination']])
            row['current_source_bytes']=len(data[row['destination']])
        elif selected:row['current_source_sha256']=selected['source_sha256']
    mapping['current_catalog']=dict(path=index_path,sha256=sha(output[index_path]),cases=len(cases),existing_ids_preserved=431)
    mapping['status']='PHYSICALLY_RELOCATED_NUMBERED_SOURCE_PINS_REFRESHED_DISTRIBUTION_PENDING'
    output[mapping_path]=encoded(mapping)
    retired=[];changed_docs=[]
    if not a.catalog_only:
        predicate=pure(data['sdk/tools/prepare_distribution.py'],{'TEST_SOURCE_PREFIXES','TEST_SOURCE_FILES','test_source'},
            dict(Path=Path,re=re,frozenset=frozenset))['test_source']
        manifest_path='sdk/share/distribution-manifest.json'
        manifest=json.loads(data[manifest_path]);old_entries=manifest['entries'];retired=[]
        def retired_entry(row):
            path=row['path'];origin='sdk/origin/'+row.get('original',{}).get('path','')
            return predicate(path)or 'sdk/'+path in mapping['sources']or origin in mapping['sources']or origin in historical
        manifest['entries']=[]
        for row in old_entries:
            if retired_entry(row):retired.append(row)
            else:manifest['entries'].append(row)
        retirement=manifest.get('test_delivery_retirement')
        if retirement is None:
            retirement=dict(retired_entries=len(retired),original_manifest_sha256=sha(data[manifest_path]),
                original_bytes_retained_in_capture=a.sha256,production_entries_regenerated=False)
        elif retired:
            retirement=dict(retirement)
            retirement['retired_entries']+=len(retired)
            retirement.setdefault('additional_retirement_batches',[]).append(dict(entries=len(retired),
                manifest_sha256=sha(data[manifest_path]),capture_sha256=a.sha256))
        manifest.update(tests_installed=False,numbered_test_cases_installed=0,
            external_test_catalog=index_path,global_sdk_assembly_complete=False,historical_entries_current=False,
            test_delivery_retirement=retirement)
        require(not any(retired_entry(r)for r in manifest['entries']),'test delivery declarations remain')
        output[manifest_path]=encoded(manifest)
        sdk_index_path='sdk/share/sdk-index.json';sdk_index=json.loads(data[sdk_index_path])
        sdk_index.update(tests_installed=False,numbered_test_cases_installed=0,external_test_catalog=index_path,
            global_sdk_assembly_complete=False,historical_distribution_entries_current=False)
        output[sdk_index_path]=encoded(sdk_index)
        replacements={old:row['destination']for old,row in mapping['sources'].items()}
        replacements.update({
            'sdk/origin/tests/c/':SOURCE+'sdk-tests/c/','sdk/origin/tests/cpp/':SOURCE+'sdk-tests/cpp/',
            'sdk/origin/tests/rust/':SOURCE+'sdk-native-languages/rust/',
            'sdk/origin/tests/':SOURCE+'sdk-tests/'})
        changed_docs=[]
        for name,raw in data.items():
            if not name.endswith('.md')or not name.startswith(('sdk/origin/docs/','sdk/origin/README.md','sdk/README.md')):continue
            text=raw.decode()
            for old,new in sorted(replacements.items(),key=lambda r:len(r[0]),reverse=True):text=text.replace(old,new)
            if name=='sdk/README.md':
                text=text.replace('  test/platform/<platform>/<language>/    # SDK contracts\n','')
                text=text.replace('Application examples and tests are excluded by default. Enable\n'
                    '`ROSWASM_INSTALL_EXAMPLES` and `ROSWASM_INSTALL_TESTS` explicitly when needed.',
                    'Application examples are installed by default; set `ROSWASM_INSTALL_EXAMPLES=OFF`\n'
                    'to omit them. SDK tests are a separate repository artifact under\n'
                    '`test/0073.ros-tests` and are never installed.')
                text=text.replace('The distribution manifest records the\nexact installed files and any header include-path transformation.',
                    'The distribution manifest identifies file provenance and transformations.\n'
                    'Its assembly/currentness flags distinguish a regenerated SDK from historical entries.')
            if name in('sdk/origin/docs/build-matrix.md','sdk/origin/docs/model-matrix.md','sdk/origin/docs/local-testing.md'):
                heading=text.find('\n')
                note='\n\nCurrent maintained execution uses the independently validated shared finite owner.\n'
                note+='Put every new output below `$UWVM_RESOURCE_RUN/work/`. Current drivers bind\n'
                note+='the actual child identity and original parent hard64GiB/swap0 automatically;\n'
                note+='the child has no delegated memory limit. Raw charged memory and approved\n'
                note+='inactive/nonshared/unmapped clean-file accounting are recorded separately.\n'
                note+='Old explicit leaf-limit recipes below describe historical runs. See\n'
                note+='`maintained-tests.md` for the current invocation and evidence boundaries.\n'
                if 'See\n`maintained-tests.md`'not in text:text=text[:heading]+note+text[heading:]
                text=text.replace('prefix-b/share/roswasm/'+SOURCE+'sdk-tests/',
                    SOURCE+'sdk-tests/ (with separately installed headers under prefix-b/include)')
            if text.encode()!=raw:output[name]=text.encode();changed_docs.append(name)
        output['sdk/origin/docs/maintained-tests.md']=b'''# Maintained source tests after SDK relocation

Test implementations and fixtures live in the separate repository artifact
`test/0073.ros-tests/support/source/`. Numbered manifests preserve exact source
hashes. `run.py` and per-case guides share support links; the invoking path
selects the actual case. The installed SDK contains application examples and
production modules, and does not require repository tests to build an example.

Inspect a numbered manifest with `python3 run.py --describe`. Actual execution
requires `--execute --owner /absolute/active/resource-evidence/result.json`.
Native/Wasm cases additionally require an explicit reviewed compiler/run recipe.
The recipe binds the source SHA256 and a fresh artifact under the live owner's
work directory. No filename supplies a compiler, endpoint, registry or grant.

Execute maintained matrix controllers only through the existing SSH Linux
finite owner. The runner supplies `UWVM_RESOURCE_RUN`; the controller verifies
the live root receipt, original parent/child identity, supervisor ancestry and
common kernel flock. Place `--out` below `$UWVM_RESOURCE_RUN/work/`. Omitted
incarnation pins bind to this actual child. The kernel hard64GiB/swap0 bound
belongs to the original parent; the child uses sampled RSS/process/disk bounds
and inherited finite per-process rlimits. Parent raw charge, conservative
clean-file accounting and OOM counters remain separately recorded.

The native driver is
`test/0073.ros-tests/support/source/sdk-maintenance/tools/run_cgroup_models.py`.
Select `--groups c cpp` with actual `--cc`, `--cxx`, and `--cmake` paths. It
installs and relocates the public SDK, then compiles external canonical fixtures
against those installed headers. Rust/Go groups stage private checks into owned
temporary production copies; tests are never added back to the installed SDK.

The serial source/compiler driver is
`test/0073.ros-tests/support/source/sdk-maintenance/tools/run_local_checks.py`.
Its outputs, snapshot, logs and caches remain under the actual owner. JS source
models retain their result in owned command logs. Historical compiler reuse
requires exact original inputs; the migration does not promote older products.

The approved owner exposes `--address-mib`; a measured Runtime composition may
need an explicit finite49152MiB virtual-address allowance. This changes no
physical memory limit. It is not a new Guest permission.

Catalog preparation executes no case. Current evidence distinguishes source
contracts, two host C11/C++20 import models, actual Wasm validation, actual Guest
runs and native ROS delivery. A host import model does not establish a ROS peer,
ARM/QEMU execution, a complete vendor example, or all compiler standards.
'''
    output[SOURCE+'ros_sdk_test_migration/prepare_current_state.py']=Path(__file__).read_bytes()
    packed=Path('current-migration-state.tar.gz')
    with tarfile.open(packed,'w:gz')as archive:
        for name,raw in sorted(output.items()):
            member=tarfile.TarInfo(name);member.mode=0o644;member.size=len(raw);archive.addfile(member,io.BytesIO(raw))
        for row in links:
            member=tarfile.TarInfo(row['path']);member.type=tarfile.SYMTYPE;member.linkname=row['target'];archive.addfile(member)
    Path('published-file-sha256.txt').write_text(''.join(sha(raw)+'  '+name+'\n'for name,raw in sorted(output.items())))
    source_pins=''.join(r['source_sha256']+'  '+PREFIX+r['source']+'\n'for r in baseline['cases'])
    if a.catalog_only:
        Path('current-case-source-sha256.txt.gz').write_bytes(gzip.compress(source_pins.encode(),mtime=0))
    else:Path('current-case-source-sha256.txt').write_text(source_pins)
    result=dict(status='CURRENT_SOURCE_PINS_AND_TEST_DELIVERY_METADATA_PREPARED',resource=proof,
        original_case_ids_preserved=len(original_ids),original_historical_baseline_preserved=431,
        cases=len(cases),changed_numbered_sources=changes,added_case_ids=[r['id']for r in cases if r['id']not in original_ids],
        sdk_test_delivery_entries_retired=len(retired),sdk_metadata_updated=not a.catalog_only,
        sdk_production_assembly_regenerated=False,
        source_map_sha256=sha(output[mapping_path]),catalog_sha256=sha(output[index_path]),
        archive_sha256=sha(packed.read_bytes()),archive_bytes=packed.stat().st_size,
        changed_docs=changed_docs,actual_tests_executed_by_metadata_preparation=0,guest_cases=0,native_ros_cases=0,default_authority=[])
    Path('current-state-result.json').write_bytes(encoded(result))
    print(json.dumps({k:v for k,v in result.items()if k not in('resource','changed_docs','changed_numbered_sources')}),flush=True)

if __name__=='__main__':main()
