#!/usr/bin/env python3
"""Verify the frozen deadline delivery without extracting or executing archive contents."""
import argparse,os,codecs,collections,gzip,hashlib,json,re,sys,tarfile,traceback
from pathlib import Path,PurePosixPath

CONTRACT = {'archive': {'sha256': '8c27029245831fdf3f520a2f731ee677d281d568354499cafeae68b4b9881759', 'bytes': 15796737}, 'index': {'sha256': 'c98be73c402e4a4ddad162dd75f9c2ffd2acd43fa093d480850dfd99512f68e9', 'bytes': 1889331}, 'members': 4013, 'data_members': 4011, 'aliases': 19730, 'excluded': 1124, 'payload_bytes': 87497538, 'source_base': '/home/macromodel/.local/share/uwvm-ros-extension', 'manifest_name': 'task-files-deadlines-20261013.json', 'protected_name': 'deadlines-20261013-protected-prior.json', 'self_metadata': {'bundle/planning/aliases.json': {'sha256': '71797deb922738f45a03ecef8a750431e1cb626be050caac87223bdf60bba19f', 'bytes': 5623534}, 'bundle/planning/source-table.json': {'sha256': 'f8557bb4247e10b8270d1d9e9304a451f3bc1f95df4a6d1e94d3971ec5e4436c', 'bytes': 1547921}}, 'anchors': {'20261013-client-deadline-before-r1/results/report.json': {'member': 'bundle/evidence/objects/9757f61ceeaeffa5cfc3132860b1e3b290d13d8a8a550ca09c315d9bb59c905d.json', 'sha256': '9757f61ceeaeffa5cfc3132860b1e3b290d13d8a8a550ca09c315d9bb59c905d', 'bytes': 8317, 'status': 'FAIL'}, '20261013-client-deadline-before-r2/results/report.json': {'member': 'bundle/evidence/objects/45c329015fdfca95214382121b634b6934b7cc673fd0482f5f52770656d671cb.json', 'sha256': '45c329015fdfca95214382121b634b6934b7cc673fd0482f5f52770656d671cb', 'bytes': 17117, 'status': 'CONFIRMED_BEFORE_SEMANTIC_FAILURES'}, '20261013-client-deadline-after-r1/results/report.json': {'member': 'bundle/evidence/objects/0eaf32ea36d1e330fa8f8b6a66f4fa98578b7c4afa0c01e985e749a7856e901d.json', 'sha256': '0eaf32ea36d1e330fa8f8b6a66f4fa98578b7c4afa0c01e985e749a7856e901d', 'bytes': 14676, 'status': 'FAIL'}, '20261013-client-deadline-after-r2/results/report.json': {'member': 'bundle/evidence/objects/a58afe2b208ddc30182ed2c2959fd7d1a31da7e9c2b58f84bd79c271be271420.json', 'sha256': 'a58afe2b208ddc30182ed2c2959fd7d1a31da7e9c2b58f84bd79c271be271420', 'bytes': 37647, 'status': 'FAIL'}, '20261013-client-deadline-after-r3/results/report.json': {'member': 'bundle/evidence/objects/cde9ca5a3249e023744b1a7baa9955af7aa869fdb77609febe0b0349eb379ba4.json', 'sha256': 'cde9ca5a3249e023744b1a7baa9955af7aa869fdb77609febe0b0349eb379ba4', 'bytes': 58204, 'status': 'PASS'}, '20261013-client-deadline-unit-entry-r1/results/results.json': {'member': 'bundle/evidence/objects/0b87dd87b05a656dcd6c14cd33f1223158ee78690cb50e6fb2c66dbbef670106.json', 'sha256': '0b87dd87b05a656dcd6c14cd33f1223158ee78690cb50e6fb2c66dbbef670106', 'bytes': 219, 'status': 'PASS'}, '20261013-client-deadline-workers-r1/report.json': {'member': 'bundle/evidence/objects/a2507652dee90bd612a9017b78f2d9c06d139c6d21cc224083e0e0461e8332b3.json', 'sha256': 'a2507652dee90bd612a9017b78f2d9c06d139c6d21cc224083e0e0461e8332b3', 'bytes': 6221, 'status': 'FAIL'}, '20261013-client-deadline-workers-r1/build/results.json': {'member': 'bundle/evidence/objects/3194cf3d18e2ebad29b923b18a972b86a221cfa3500042b90968dedacaed2e46.json', 'sha256': '3194cf3d18e2ebad29b923b18a972b86a221cfa3500042b90968dedacaed2e46', 'bytes': 1279, 'status': 'PASS'}, '20261013-client-deadline-workers-r2/report.json': {'member': 'bundle/evidence/objects/4cf78b94cdf67ae93fb323ef56c909295bb8c9811d39913fcc456de068f2c6f3.json', 'sha256': '4cf78b94cdf67ae93fb323ef56c909295bb8c9811d39913fcc456de068f2c6f3', 'bytes': 2567949, 'status': 'PASS'}, '20261013-client-deadline-native-before-r1/reports/report.json': {'member': 'bundle/evidence/objects/2bf38f0f0e51a9490b65f3454dc43e50b94262a8c16a4842781e987692a7880e.json', 'sha256': '2bf38f0f0e51a9490b65f3454dc43e50b94262a8c16a4842781e987692a7880e', 'bytes': 614939, 'status': 'CONFIRMED_BEFORE_SEMANTIC_FAILURES'}, '20261013-client-deadline-native-after-r1/reports/report.json': {'member': 'bundle/evidence/objects/79da03ea6fe8fa683a5b28a491db3c4a7d7f22fcdfc041e2990c87bca093a632.json', 'sha256': '79da03ea6fe8fa683a5b28a491db3c4a7d7f22fcdfc041e2990c87bca093a632', 'bytes': 615235, 'status': 'PASS'}, '20261013-client-deadline-independent-r1/independent-validation.json': {'member': 'bundle/evidence/objects/d7cef183fcd0feaec49af4eae179dc7bee62753e11888c52331266b7e63d25b7.json', 'sha256': 'd7cef183fcd0feaec49af4eae179dc7bee62753e11888c52331266b7e63d25b7', 'bytes': 1784067, 'status': 'FAIL', 'checks': 7698, 'executed_verifier_sha256': '6a188932f7968b0a1d048f8dd7f88e429ebd8e845fb6f35c276e0c124bfe8a18'}, '20261013-client-deadline-independent-r2/independent-validation.json': {'member': 'bundle/evidence/objects/0c9251f4caf40099d58c10ea7f060dd12b475f05a6eb40566bbf3c1e1826e343.json', 'sha256': '0c9251f4caf40099d58c10ea7f060dd12b475f05a6eb40566bbf3c1e1826e343', 'bytes': 5262856, 'status': 'PASS', 'checks': 91725, 'executed_verifier_sha256': 'c31c75f6fba58de6951f3bb03534d1ad68800883f534735fab8706d681b6d510'}, '20261013-deadline-audit-native-before-r1/report.json': {'member': 'bundle/evidence/objects/d3195eda83f1c0a11c5144bc8e1024f381c3157cf126b1a654ab8dcf2ea0d558.json', 'sha256': 'd3195eda83f1c0a11c5144bc8e1024f381c3157cf126b1a654ab8dcf2ea0d558', 'bytes': 11427, 'status': 'PASS'}, '20261013-deadline-audit-native-after-r1/report.json': {'member': 'bundle/evidence/objects/ee927c4608f3480eab925727f422f755756101fc8be26dcdecd3de6d41527e1e.json', 'sha256': 'ee927c4608f3480eab925727f422f755756101fc8be26dcdecd3de6d41527e1e', 'bytes': 232288, 'status': 'PASS'}, '20261013-deadline-audit-native-verification-r2/independent-validation.json': {'member': 'bundle/evidence/objects/310dd543b8b8c0d54b2830e7cdc7a0c0031b3fd065380a02d350f6ccec23a51e.json', 'sha256': '310dd543b8b8c0d54b2830e7cdc7a0c0031b3fd065380a02d350f6ccec23a51e', 'bytes': 16209, 'status': 'PASS', 'checks': 1195, 'executed_verifier_sha256': 'd1179368b0e1aad33358a5307cfc7526b620b9f068add7747c00c193f865bf59'}, '20261013-deadline-audit-noetic-r1/reports/bootstrap-failure.json': {'member': 'bundle/evidence/objects/64658584052d241c922cb07566ec27d57bdb20da22dfd43f41d2f9c2e58f0fed.json', 'sha256': '64658584052d241c922cb07566ec27d57bdb20da22dfd43f41d2f9c2e58f0fed', 'bytes': 571, 'status': 'FAIL'}, '20261013-deadline-audit-noetic-r2/reports/report.json': {'member': 'bundle/evidence/objects/3873a8ebc5fd16e050d8486ce8438d7b535f2c62404d48bbec24da29604edd30.json', 'sha256': '3873a8ebc5fd16e050d8486ce8438d7b535f2c62404d48bbec24da29604edd30', 'bytes': 26447, 'status': 'PASS'}, '20261013-deadline-audit-noetic-verification-r1/independent-validation.json': {'member': 'bundle/evidence/objects/dda3c709dc52f64fba277f41c2c135511e7da448472f5e7cdef376770cc93e1f.json', 'sha256': 'dda3c709dc52f64fba277f41c2c135511e7da448472f5e7cdef376770cc93e1f', 'bytes': 17023, 'status': 'PASS', 'checks': 379, 'executed_verifier_sha256': '2c0180fd87f2176c418b159329425ebd8f124081ce27e87b20d24640a514503e'}, '20261013-parameter-exception-before-r1/reports/report.json': {'member': 'bundle/evidence/objects/398468fc84b8b515a07e082153e34ae341c3fc92a669e591e670346e743491fe.json', 'sha256': '398468fc84b8b515a07e082153e34ae341c3fc92a669e591e670346e743491fe', 'bytes': 123304, 'status': 'FAIL'}, '20261013-parameter-exception-before-r2/reports/report.json': {'member': 'bundle/evidence/objects/ba45a9d7c479378db1111b2eceb71b304fe124d5354f28ce797226d3c0443a33.json', 'sha256': 'ba45a9d7c479378db1111b2eceb71b304fe124d5354f28ce797226d3c0443a33', 'bytes': 138583, 'status': 'FAIL'}, '20261013-parameter-exception-after-r1/reports/report.json': {'member': 'bundle/evidence/objects/afaf64c915d26778d7a566eacabb19146577197f1339a11ddaa9ace09bb57dce.json', 'sha256': 'afaf64c915d26778d7a566eacabb19146577197f1339a11ddaa9ace09bb57dce', 'bytes': 130657, 'status': 'PASS'}, '20261013-parameter-exception-verification-r1/independent-validation.json': {'member': 'bundle/evidence/objects/17829f669920c560468e5a7287e19500878819b115413fdd0bd5ce8395289243.json', 'sha256': '17829f669920c560468e5a7287e19500878819b115413fdd0bd5ce8395289243', 'bytes': 4390, 'status': 'PASS', 'checks': 1318, 'executed_verifier_sha256': 'c489efd341295cafaa6b8bbbdbe67d084444c0d9229732fd514dbf4508e32e3b'}, '20261013-deadline-local-protection-r1/proof.json': {'member': 'bundle/evidence/objects/8d65fcc45207c9129a6110830439068f716a140aa0b4b93e3ba15bb05bf956d5.json', 'sha256': '8d65fcc45207c9129a6110830439068f716a140aa0b4b93e3ba15bb05bf956d5', 'bytes': 19566, 'status': 'PASS', 'checks': 147, 'executed_verifier_sha256': '6a188932f7968b0a1d048f8dd7f88e429ebd8e845fb6f35c276e0c124bfe8a18'}, '20261013-deadline-local-protection-r1/protected-manifest.json': {'member': 'bundle/evidence/objects/920375c92235c5fe2c663311c2348f199a50aa5e6019b66ce2cab39e00a785ee.json', 'sha256': '920375c92235c5fe2c663311c2348f199a50aa5e6019b66ce2cab39e00a785ee', 'bytes': 7665}, '20261013-deadline-current-inputs-r2/task-files.json': {'member': 'bundle/metadata/task-files.json', 'sha256': 'd1e75ccb22232f870bc9dd30fde192a312af60d95e2b3c9cd53d9f495cbc93b4', 'bytes': 3847}, '20261013-deadline-audit-noetic-r1/reports/bootstrap-executed-driver.py': {'member': 'bundle/evidence/objects/e9c04769cd0274232706b9b49d3f26ee4316dcd8f9bcf0d378c38883fe651f5e.py', 'sha256': 'e9c04769cd0274232706b9b49d3f26ee4316dcd8f9bcf0d378c38883fe651f5e', 'bytes': 11619}, '20261013-deadline-audit-native-before-r1/run_before.py': {'member': 'bundle/evidence/objects/43fc1ab43f5dc86b9fe4379b4be2e13835b6db5acdd740bb513b99eea351f42b.py', 'sha256': '43fc1ab43f5dc86b9fe4379b4be2e13835b6db5acdd740bb513b99eea351f42b', 'bytes': 6928}, '20261013-deadline-audit-native-after-r1/executed-driver.py': {'member': 'bundle/current-source/tests/ros_native_audit_io/run.py', 'sha256': '13a8e1f01462ea47c956ab08e2d3a9cf84a2833a9160d2e1e7ecfe9b011f66e9', 'bytes': 15376}, '20261013-deadline-audit-native-verification-r2/verify_native_audit.py': {'member': 'bundle/evidence/objects/d1179368b0e1aad33358a5307cfc7526b620b9f068add7747c00c193f865bf59.py', 'sha256': 'd1179368b0e1aad33358a5307cfc7526b620b9f068add7747c00c193f865bf59', 'bytes': 9857}, '20261013-deadline-audit-noetic-verification-r1/verify_noetic.py': {'member': 'bundle/evidence/objects/2c0180fd87f2176c418b159329425ebd8f124081ce27e87b20d24640a514503e.py', 'sha256': '2c0180fd87f2176c418b159329425ebd8f124081ce27e87b20d24640a514503e', 'bytes': 10798}}}
CHUNK=1048576
PRIVATE=re.compile(rb'(?m)^[ \t]*(?:-----BEGIN (?:[A-Z0-9 ]* )?PRIVATE KEY-----|---- BEGIN SSH2 ENCRYPTED PRIVATE KEY ----)[ \t]*\r?$')
class PrivateDetector:
    def __init__(self):self.carry=b'';self.found=False
    def feed(self,block):
        joined=self.carry+block;end=joined.rfind(b'\n')
        if end>=0:self.found=self.found or PRIVATE.search(joined[:end+1]) is not None;self.carry=joined[end+1:]
        else:self.carry=joined
        if len(self.carry)>256:self.carry=b'x'+self.carry[-256:]
    def finish(self):return self.found or PRIVATE.search(self.carry) is not None

def digest(path):
    h=hashlib.sha256()
    with Path(path).open('rb') as f:
        for block in iter(lambda:f.read(CHUNK),b''):h.update(block)
    return h.hexdigest()
def safe(name):
    return isinstance(name,str) and bool(name) and not name.startswith('/') and not re.match(r'^[A-Za-z]:',name) and '\\' not in name and all(x not in ('','.','..') for x in name.split('/')) and all(ord(c)>=32 and ord(c)!=127 for c in name) and str(PurePosixPath(name))==name
def source_alias_allowed(name):
    # Compiler paths retain their original ../ spelling. Only archive member
    # names must be canonical. A source alias must still stay below source_base.
    if not isinstance(name,str) or not name or name.startswith('/') or re.match(r'^[A-Za-z]:',name) or '\\' in name or any(ord(c)<32 or ord(c)==127 for c in name):return False
    return safe(os.path.normpath(name))

def private_path(name):
    return any(x.lower().endswith(('.pem','.key')) or x.lower() in ('key','id_rsa','id_dsa','id_ecdsa','id_ed25519') or any(word in x.lower() for word in ('private_key','private-key','privatekey')) for x in PurePosixPath(name).parts)
def json_bytes(raw):
    def pairs(items):
        out={}
        for k,v in items:
            if k in out:raise ValueError('duplicate JSON key: '+k)
            out[k]=v
        return out
    return json.loads(raw,object_pairs_hook=pairs)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('root','archive','index','out'):parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args()
    if args.out.exists():parser.error('output must be new; previous proof/failure remains unchanged')
    counts=collections.Counter();result={'status':'FAIL','scope':'Frozen local archive, all UTF8 members, explicit source aliases, current24 and prior48 preservation; no ROS execution or new A15 claim','argv':sys.argv,'executed_verifier_sha256':digest(__file__)}
    def check(c,g,m):
        counts[g]+=1
        if not c:raise AssertionError(m)
    try:
        root=args.root.resolve(strict=True);archive=args.archive.resolve(strict=True);index_path=args.index.resolve(strict=True)
        check(args.out.resolve() not in (archive,index_path),'artifact','output/input collision')
        for path,key in ((archive,'archive'),(index_path,'index')):
            c=CONTRACT[key];check(path.is_file() and not path.is_symlink() and path.stat().st_size==c['bytes'] and digest(path)==c['sha256'],'artifact','artifact SHA/size differs: '+str(path))
        index=json_bytes(index_path.read_bytes());check(index['schema']=='uwvm-ros-next-evidence-index-v1' and index['status']=='PASS','index','index schema/status')
        check(index['archive']['sha256']==CONTRACT['archive']['sha256'] and index['archive']['bytes']==CONTRACT['archive']['bytes'] and index['archive']['verified_members']==CONTRACT['members'],'index','archive index binding')
        check(index['archive']['all_member_sha256_matched'] is True and index['archive']['exact_index_match'] is True,'index','producer second-pass declaration')
        selection=index['selection'];check(selection['explicit_inputs_only'] is True and selection['default_payloads_require_utf8'] is True and selection['private_keys_excluded'] is True and selection['directory_symlinks_followed'] is False,'index','selection contract')
        check(index['summary']=={'all_payloads_utf8':True,'binary_nested_archives':0,'payload_bytes':CONTRACT['payload_bytes'],'payload_files':CONTRACT['members'],'skipped_entries':0,'utf8_text_files':CONTRACT['members']} and index['skipped']==[],'index','exact text/count summary')
        check(index['verification']=={'exact_member_set':True,'frozen_source_stream_hash_checked':True,'read_only_second_pass':True,'tar_metadata':{'gid':0,'mode':'0644','mtime':0,'uid':0}},'index','producer verification declarations')
        payloads=index['payloads'];expected={v['arcname']:v for v in payloads};check(len(expected)==len(payloads)==CONTRACT['members'],'index','duplicate or missing payload records')
        check(len(index['inputs'])==1 and index['inputs'][0]['name']=='bundle' and index['inputs'][0]['is_directory'] is True and index['inputs'][0]['selection']=='utf8-input','index','one explicit staged input')
        stage=index['inputs'][0]['source'].rstrip('/')
        for name,v in expected.items():
            check(safe(name) and name.startswith('bundle/') and not private_path(name),'index','unsafe/private member')
            check(v['kind']=='utf8-text' and v['isutf8'] is True and type(v['bytes']) is int and v['bytes']>=0 and re.fullmatch('[0-9a-f]{64}',v['sha256']) is not None,'index','payload types')
            check(v['source']==stage+'/'+name[len('bundle/'):],'index','staged index source relationship')
        for name,hard in CONTRACT['self_metadata'].items():check(all(expected[name][k]==hard[k] for k in ('sha256','bytes')),'index','self metadata anchor')
        capture=set(CONTRACT['self_metadata'])|{v['member'] for v in CONTRACT['anchors'].values() if v['member'].endswith('.json')}
        data={};seen=set();total=0
        with gzip.open(archive,'rb') as zipped:
            with tarfile.open(fileobj=zipped,mode='r|',encoding='utf-8',errors='strict') as tar:
                for member in tar:
                    name=member.name;check(name in expected and name not in seen,'member','unknown/duplicate member')
                    check(member.type==tarfile.REGTYPE and member.isfile() and safe(name) and not private_path(name),'member','unsafe/nonregular member')
                    check((member.uid,member.gid,member.mtime,member.uname,member.gname,member.mode,member.linkname,member.devmajor,member.devminor)==(0,0,0,'','',0o644,'',0,0) and not member.pax_headers,'member','noncanonical tar metadata')
                    v=expected[name];check(member.size==v['bytes'],'member','member size')
                    h=hashlib.sha256();n=0;decoder=codecs.getincrementaldecoder('utf-8')('strict');private=PrivateDetector();first=True;kept=[]
                    if name in capture:check(member.size<=8*1024*1024,'member','captured JSON exceeds8MiB')
                    with tar.extractfile(member) as stream:
                        for block in iter(lambda:stream.read(CHUNK),b''):
                            if first:check(not block.startswith((b'\x7fELF',b'\x00asm')),'text','ELF/Wasm payload');first=False
                            decoder.decode(block,final=False);private.feed(block);h.update(block);n+=len(block)
                            if name in capture:kept.append(block)
                    decoder.decode(b'',final=True);check(not private.finish(),'text','private key content')
                    check(n==v['bytes'] and h.hexdigest()==v['sha256'],'member','digest/bytes mismatch');seen.add(name);total+=n
                    if name in capture:data[name]=json_bytes(b''.join(kept))
            while zipped.read(CHUNK):pass
        check(seen==set(expected) and total==CONTRACT['payload_bytes'],'member','exact member set/total bytes')
        check(not any(PurePosixPath(n).name in (index_path.name,Path(__file__).name) for n in seen),'member','external index/verifier was archived')
        aliases=data['bundle/planning/aliases.json'];table=data['bundle/planning/source-table.json'];files=table['files'];self_names=set(CONTRACT['self_metadata']);actual_data=seen-self_names
        check(aliases['source_base']==CONTRACT['source_base'] and aliases['columns']==['relative_source','payload_archive','sha256','bytes'],'aliases','source-base/columns')
        check(aliases['data_payload_count']==CONTRACT['data_members'] and aliases['alias_count']==CONTRACT['aliases'] and len(aliases['aliases'])==CONTRACT['aliases'],'aliases','alias/data counts')
        check(len(files)==CONTRACT['data_members'] and set(files)==actual_data,'table','data table exact set excludes self metadata')
        alias_by_source={};referenced=set()
        for row in aliases['aliases']:
            check(isinstance(row,list) and len(row)==4,'aliases','four alias columns');source,target,sha,n=row
            check(source_alias_allowed(source) and not private_path(os.path.normpath(source)) and source not in alias_by_source,'aliases','unsafe/private/duplicate source alias')
            check(target in files and expected[target]['sha256']==sha and expected[target]['bytes']==n,'aliases','alias/member relationship');alias_by_source[source]=row;referenced.add(target)
        check(referenced==actual_data,'aliases','data member without source alias')
        for name,v in files.items():
            check(v['sha256']==expected[name]['sha256'] and v['bytes']==expected[name]['bytes'],'table','table/index hashes/sizes')
            original=v['original_source'];prefix=CONTRACT['source_base']+'/'
            check(original.startswith(prefix) and original[len(prefix):] in alias_by_source,'table','table source outside explicit aliases')
            row=alias_by_source[original[len(prefix):]];check(row[1:]==[name,v['sha256'],v['bytes']],'table','table canonical source alias')
        excluded=aliases['excluded'];check(len(excluded)==CONTRACT['excluded'],'aliases','excluded count')
        for row in excluded:
            check(source_alias_allowed(row['source']) and row['source'] not in alias_by_source and isinstance(row['reason'],str) and bool(row['reason']),'aliases','excluded/source collision or missing reason')
            check(type(row['bytes']) is int and row['bytes']>=0 and re.fullmatch('[0-9a-f]{64}',row['sha256']) is not None,'aliases','excluded digest/size')
        mandatory={}
        for source,hard in CONTRACT['anchors'].items():
            check(alias_by_source.get(source)==[source,hard['member'],hard['sha256'],hard['bytes']],'anchors','mandatory source alias changed: '+source)
            check(expected[hard['member']]['sha256']==hard['sha256'] and expected[hard['member']]['bytes']==hard['bytes'],'anchors','mandatory payload changed')
            if hard['member'] in data:
                doc=data[hard['member']]
                for field in ('status','checks','executed_verifier_sha256'):
                    if field in hard:check(doc.get(field)==hard[field],'anchors','actual status/proof identity changed: '+source)
            mandatory[source]=hard
        manifest_source='20261013-deadline-current-inputs-r2/task-files.json';manifest_hard=CONTRACT['anchors'][manifest_source];manifest=data[manifest_hard['member']];current=manifest['files']
        manifest_path=root/'documents/ros'/CONTRACT['manifest_name'];check(manifest_path.stat().st_size==manifest_hard['bytes'] and digest(manifest_path)==manifest_hard['sha256'],'current','external task manifest')
        check(len(current)==24 and {n for n in actual_data if n.startswith('bundle/current-source/')}=={'bundle/current-source/'+n for n in current},'current','exact canonical current24')
        coverage=[]
        for name,v in current.items():
            check(safe(name) and not private_path(name),'current','unsafe current path');path=root/name;check(path.resolve().is_relative_to(root) and not path.is_symlink() and path.is_file(),'current','nonregular/outside current path')
            check(path.stat().st_size==v['bytes'] and digest(path)==v['sha256'],'current','actual current file changed: '+name)
            member='bundle/current-source/'+name;check(expected[member]['bytes']==v['bytes'] and expected[member]['sha256']==v['sha256'],'current','canonical current member differs');coverage.append({'path':name,**v,'member':member})
        protected_source='20261013-deadline-local-protection-r1/protected-manifest.json';hard=CONTRACT['anchors'][protected_source];protected=data[hard['member']];protected_path=root/'documents/ros'/CONTRACT['protected_name']
        check(protected_path.stat().st_size==hard['bytes'] and digest(protected_path)==hard['sha256'],'protected','external prior manifest changed');check(len(protected)==48,'protected','prior protection count')
        for name,v in protected.items():
            check(safe(name),'protected','unsafe prior path');path=root/name;check(path.resolve().is_relative_to(root) and path.is_file() and not path.is_symlink(),'protected','nonregular/outside prior path')
            check(path.stat().st_size==v['bytes'] and digest(path)==v['sha256'],'protected','prior formal artifact changed: '+name)
        check(digest(archive)==CONTRACT['archive']['sha256'] and digest(index_path)==CONTRACT['index']['sha256'],'artifact','artifact changed during verification')
        result.update(status='PASS',archive={'path':str(archive),**CONTRACT['archive']},external_index={'path':str(index_path),**CONTRACT['index'],'in_archive':False},members={'all':len(seen),'data':len(actual_data),'self_metadata':len(self_names),'utf8':len(seen),'payload_bytes':total,'all_digests_sizes_fixed_metadata':True,'full_gzip_trailer_checked':True,'nested_archives':0},aliases={'source_base':CONTRACT['source_base'],'count':len(alias_by_source),'excluded':len(excluded),'every_data_member_referenced':True},mandatory_evidence=mandatory,current24=coverage,protected48={'manifest_sha256':hard['sha256'],'all_actual_sha256_and_sizes_matched':True,'count':48})
    except Exception as error:result.update(error=str(error),traceback=traceback.format_exc())
    result['checks']=sum(counts.values());result['check_groups']=dict(counts);args.out.parent.mkdir(parents=True,exist_ok=True)
    with args.out.open('x',encoding='utf8') as stream:stream.write(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'checks':result['checks'],'out':str(args.out),'proof_sha256':digest(args.out),'error':result.get('error')},ensure_ascii=False));return 0 if result['status']=='PASS' else 1
if __name__=='__main__':sys.exit(main())
