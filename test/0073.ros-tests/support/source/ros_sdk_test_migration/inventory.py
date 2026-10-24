"""Inventory SDK test provenance without moving files or executing suites."""
import argparse
import ast
import collections
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import sys
import tarfile

BASE='test/0073.ros-tests/support/source/'

def require(value,message):
    if not value:raise ValueError(message)

def sha(path):
    with path.open('rb')as stream:return hashlib.file_digest(stream,'sha256').hexdigest()

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive',type=Path,required=True);p.add_argument('--sha256',required=True)
    p.add_argument('--tools',type=Path,required=True)
    a=p.parse_args();sys.path.insert(0,str(a.tools.resolve(strict=True)))
    from resource_contract import validate_resource_owner
    proof=validate_resource_owner();require(sha(a.archive)==a.sha256,'exact inert capture required')
    entries={};documents={}
    with tarfile.open(a.archive,'r:')as archive:
        for item in archive:
            path=PurePosixPath(item.name)
            require(not path.is_absolute()and'..'not in path.parts,'confined capture required')
            if item.isdir()or path.name.startswith('._')or path.name=='.DS_Store':continue
            require(item.name not in entries,'duplicate capture member: '+item.name)
            require(len(entries)<30000 and 0<=item.size<=32<<20,'finite source inventory required')
            row=dict(source=item.name,bytes=item.size)
            if item.issym():row.update(kind='symbolic-link',target=item.linkname)
            else:
                require(item.isfile(),'ordinary file/link capture required')
                stream=archive.extractfile(item);digest=hashlib.sha256();parts=[]
                retain=item.size<=16<<20 and path.name not in('vectors.json','verification.json')
                while chunk:=stream.read(1<<20):
                    digest.update(chunk)
                    if retain:parts.append(chunk)
                row.update(kind='file',sha256=digest.hexdigest())
                if parts:documents[item.name]=b''.join(parts)
            entries[item.name]=row
    index=json.loads(documents['test/0073.ros-tests/index.json'])
    require(len(index['cases'])==431,'complete current 431-case baseline required')
    dist=json.loads(documents['sdk/share/distribution-manifest.json'])
    provenance={row['path']:row.get('original')for row in dist['entries']if row.get('original')}
    proposal=json.loads(documents['sdk/origin/coordination/20261021-director/handoffs/04/TEST-MIGRATION-MAP.json'])
    rules=sorted(proposal['rules'],key=lambda row:len(row['source']),reverse=True)
    proposal09=json.loads(documents['sdk/origin/coordination/20261021-director/handoffs/09/TEST-MIGRATION-MAP.json'])
    rules09=sorted([row for row in proposal09['maps']if row['to'].startswith(BASE)],key=lambda row:len(row['from']),reverse=True)
    proposal08=json.loads(documents['sdk/origin/coordination/20261021-director/handoffs/08/followup-02/TEST-MIGRATION-MAP.json'])
    rules08={row['current']:row['proposed']for row in proposal08['items']
        if row['kind']=='maintained-authored-test-or-fixture'}
    vendors={}
    for node in ast.parse(documents['sdk/origin/tools/sdk_layout.py']).body:
        if isinstance(node,ast.Assign)and any(isinstance(t,ast.Name)and t.id=='PLATFORMS'for t in node.targets):
            vendors=ast.literal_eval(node.value)
    require(vendors,'actual maintained platform source mapping required')
    def canonical(name):
        if not name.startswith('sdk/origin/'):return None
        relative=name[len('sdk/origin/'):]
        if name in rules08:return rules08[name]
        if relative=='rust/owned_service/src/contracts.rs':
            return BASE+'sdk-native-languages/rust/owned_service/contracts.rs'
        if relative.startswith('platform/')and'/test/'in relative:
            vendor,language,tail=relative[len('platform/'):].split('/',2)
            if tail.startswith('test/'):return BASE+'platform-sdk/'+vendor+'/'+language+'/'+tail[len('test/'):]
        native_languages=(('rust/tests/','rust/tests/'),('tests/rust/','rust/'),
            ('tools/native_languages/fixtures/','fixtures/'))
        for prefix,destination in native_languages:
            if relative.startswith(prefix):return BASE+'sdk-native-languages/'+destination+relative[len(prefix):]
        if relative in('go/client_test.go',)or relative.startswith('tinygo/')and re.search(r'_test\.go$',relative):
            return BASE+'sdk-native-languages/'+relative
        if relative.startswith('tools/native_languages/'):
            leaf=PurePosixPath(relative).name
            if leaf in('test_profile.py','test_arguments.py'):
                return BASE+'sdk-native-languages/configuration/'+leaf
            if leaf in('validate.py','validate_additional.py','validate_subset.py','validate_go_values.py','run_extended.py'):
                return BASE+'sdk-native-languages/drivers/'+leaf
            if leaf in('fetch_tools.py','fetch_additional.py','inspect_tools.py'):
                return BASE+'sdk-native-languages/tool-qualification/'+leaf
        if relative in('tools/run_local_checks.py','tools/run_cgroup_models.py','tools/run_moonbit_models.py',
                'tools/verify_sources.py','tools/go_inventory_models.py','tools/build_cgroup_matrix.py',
                'robots/vendors/check_python_values.py'):
            return BASE+'sdk-maintenance/'+relative
        for rule in rules09:
            prefix=rule['from']
            if name==prefix or prefix.endswith('/')and name.startswith(prefix):return rule['to']+name[len(prefix):]
        for vendor,kit in (('unitree','robots/unitree'),('leju','robots/humanoids/leju'),
                ('deep_robotics','robots/platforms/deep_robotics'),('agibot','robots/platforms/agibot'),
                ('ubtech','robots/platforms/ubtech'),('fourier','robots/platforms/fourier')):
            prefix=kit+'/tests/'
            if relative.startswith(prefix):return BASE+'ros_platform05/'+vendor+'/'+relative[len(prefix):]
        for vendor,kit in (('elephant','robots/arms/elephant/mycobot'),('ufactory','robots/arms/ufactory/xarm'),
                ('dobot','robots/arms/dobot'),('flexiv','robots/arms/flexiv'),('realman','robots/arms/realman')):
            if relative.startswith(kit+'/')and any(n in('test','tests')for n in PurePosixPath(relative[len(kit)+1:]).parts):
                return BASE+'platform-sdk/'+vendor+'/authored/'+relative[len(kit)+1:]
        for rule in rules:
            selected=rule['source']
            if relative==selected or selected.endswith('/')and relative.startswith(selected):
                return BASE+'shared-language-sdk/'+rule['destination']+relative[len(selected):]
        if relative.startswith('tests/'):return BASE+'sdk-tests/'+relative[len('tests/'):]
        for prefix in ('compat/rclc/tests/','compat/rclcpp/tests/','compat/rclcpp/services/tests/'):
            if relative.startswith(prefix):return BASE+'sdk-compat/'+relative[len('compat/'):]
        for vendor,kit in sorted(vendors.items(),key=lambda pair:len(pair[1]),reverse=True):
            if relative.startswith(kit+'/')and any(n.casefold()in('test','tests')for n in PurePosixPath(relative[len(kit)+1:]).parts):
                return BASE+'sdk-platforms/'+vendor+'/'+relative[len(kit)+1:]
        if relative.startswith('custom-types/')and PurePosixPath(relative).name in('vectors.json','verification.json'):
            return BASE+'sdk-generated/'+relative[len('custom-types/'):]
        if any(n.casefold()in('test','tests')for n in PurePosixPath(relative).parts)or re.search(r'(_test\.|_wbtest\.|/test_)',relative):
            return BASE+'sdk-native/'+relative
        return None
    counts=collections.Counter();rows=[];references=[]
    for name,row in sorted(entries.items()):
        path=PurePosixPath(name)
        if name.startswith('sdk/origin/coordination/'):row['disposition']='HISTORICAL_HANDOFF_REFERENCE'
        elif name.startswith('sdk/origin/debug/'):row['disposition']='EXCLUDED_SEPARATELY_OWNED_DEBUGGER'
        elif name.startswith('sdk/origin/')and any(n=='upstream'or n.startswith('upstream_')for n in path.parts):
            row['disposition']='PRESERVE_IMMUTABLE_UPSTREAM_AUDIT'
        elif row['kind']=='symbolic-link'and'/test/'in name and name.startswith(('sdk/origin/platform/','sdk/origin/language/')):
            row['disposition']='REMOVE_GENERATED_TEST_ALIAS_AT_CUTOVER'
        elif name.startswith('sdk/')and(name.startswith('sdk/test/')or name[len('sdk/'):]in provenance and canonical('sdk/origin/'+provenance[name[len('sdk/'):]]['path'])):
            original=provenance.get(name[len('sdk/'):]);row['original_provenance']=original
            navigation=False
            if original is None and path.name=='case.json':
                try:navigation=json.loads(documents.get(name,b'{}')).get('kind')=='numbered-sdk-source-test-v1'
                except (ValueError,AttributeError):pass
            if original is None and path.name=='README.md':
                navigation=b'One original authored source test is installed here.'in documents.get(name,b'')
            if navigation:
                row.update(disposition='RETAIN_LEGACY_NAVIGATION_RECORD',destination='test/0073.ros-tests/support/records/sdk-test-migration/legacy-layout/'+name[len('sdk/'):])
            else:
                old='sdk/origin/'+original['path']if original else None
                current=entries.get(old);target=canonical(old)if old else None
                if target and current and row.get('sha256')==current.get('sha256')and row.get('sha256')==original.get('sha256'):
                    row.update(disposition='EXACT_CONTENT_AND_PROVENANCE_ALIAS',destination=target,canonical_original=old)
                else:row.update(disposition='RETAIN_DISTINCT_DELIVERED_VARIANT',destination=BASE+'sdk-delivery/'+name[len('sdk/'):])
        else:
            target=canonical(name)
            if target:
                historical=bool(re.match(r'(verification-|validation-|prepared-review|build-matrix-prepared-review)',path.name))
                historical |= path.name=='README.md'and name.startswith(('sdk/origin/platform/','sdk/origin/language/'))and'/test/'in name
                if historical:target=target.replace('/support/source/','/support/records/',1)
                row.update(disposition='MOVE_HISTORICAL_RECEIPT_UNCHANGED'if historical else'MOVE_AUTHORED_TEST_OR_FIXTURE',destination=target)
            else:row['disposition']='REFERENCE_OR_PRODUCER_INPUT_ONLY'
        counts[row['disposition']]+=1;rows.append(row)
        if name in documents and path.suffix in('.py','.lua','.md','.json','.toml','.cmake','.txt','.csproj','.xml','.c','.cc','.cpp','.h','.hpp','.rs','.zig','.mbt','.d','.odin'):
            try:text=documents[name].decode('utf-8')
            except UnicodeDecodeError:continue
            if re.search(r'#\[cfg\(test\)\]|#\[test\]|(?m:^test\s+")|(?m:^unittest\s*\{)|@\(test\)',text):
                row['inline_test_candidate']=True
                if path.suffix in('.rs','.zig','.d','.odin')and row['disposition']=='REFERENCE_OR_PRODUCER_INPUT_ONLY':
                    row['disposition']='INLINE_TEST_EXTRACTION_REQUIRES_LANGUAGE_SOURCE_REVIEW'
                    counts['REFERENCE_OR_PRODUCER_INPUT_ONLY']-=1
                    counts[row['disposition']]+=1
            for line_number,line in enumerate(text.splitlines(),1):
                if 'sdk/test'in line or 'sdk/origin/tests'in line or 'test/platform/'in line or re.search(r"['\"](?:test|tests)/",line):
                    references.append(dict(source=name,line=line_number,text=line[:1000]))
    destinations={}
    for row in rows:
        if row.get('destination')and row['disposition']!='EXACT_CONTENT_AND_PROVENANCE_ALIAS':
            key=row['destination'];previous=destinations.get(key)
            require(previous is None or previous.get('sha256')==row.get('sha256'),'distinct source collision: '+key)
            destinations[key]=row
    report=dict(kind='sdk-test-migration-inventory-v1',status='INVENTORIED_NOT_MOVED',resource=proof,
        archive_sha256=a.sha256,source_sha256=sha(Path(__file__)),source_capture_entries=len(rows),dispositions=dict(counts),platform_source_roots=len(vendors),
        existing_case_ids=[row['id']for row in index['cases']],existing_case_ids_to_preserve=431,
        actual_case_workloads_executed=0,new_guest_or_upstream_credit=0,ready_for_cutover=False,
        default_authority=[],files=rows,references=references)
    Path('test-migration-inventory.json').write_text(json.dumps(report,indent=2,sort_keys=True)+'\n')
    print(json.dumps({key:value for key,value in report.items()if key not in('files','references','existing_case_ids')}),flush=True)

if __name__=='__main__':main()
