#!/usr/bin/env python3
"""Independently verify one frozen ROS delivery without extracting its archive."""
import argparse, codecs, collections, gzip, hashlib, json, re, tarfile
from pathlib import Path, PurePosixPath

CONTRACT = {'archive_name': 'evidence-20261013-contracts.tgz', 'index_name': 'contracts-20261013-evidence-index.json', 'archive': {'sha256': 'a365b22b2eb1c6ebd01f1f12eddd8c4f97c9bc88a69cd03ba7bf319ed2ce8076', 'bytes': 24496705}, 'index': {'sha256': 'a9a1c310932f0e4a24cb5b79a4829ac02a158cbf02c5c6b0210cfdfc497e4b50', 'bytes': 3315084}, 'members': 4383, 'mappings': 4383, 'payload_bytes': 148693483, 'required': {'main-report': '3ff576750879c0aa0c96e6bf6c528a63502ba6718507e4df45cdbf023ca53885', 'main-independent': 'd7bbae8d617bda8a59468f06c6d9416f794f516c64dbe702a8ae76515e16973d', 'main-verifier': 'f5447e9ef0db41ff57ebc0918df38cb8d4bcb06b3325b0593ca0ca97512e3a2d', 'main-runner': 'cd98b90322ce024c0d3239f3885d3880dbed5703a559ad4f432f9ced4194b884', 'service-independent': '0fea5b56407560538d7a3f23e82015edee0decad70a46121afc7ddbb8f9b3172', 'string-independent': '7f00f42bd8e6a2c02d5216de5909a23acb81bbf969928d2366b3b015d0bf1923', 'string-sampler-independent': '842657fc20913134a6c0d990af486c4a02a1ccc1cce93fe33dcf106c4835445c', 'typed70-report': 'ae3afc809df1f23c4162065497c5fabc1995fbea3a633c65c523f6ccba0a5525', 'typed70-independent': 'ca45c1c797c939620079700be6413db5315cde85fe39f7d628450651482cfd27', 'deletion19-report': 'a2d7e6eccfbb5f0822b930c4b55dc6d9855968aa73acd31e3d4eae0b9c1a6773', 'deletion-independent': '7dc36035346d4681364d5526897c6233c996f81a7ac96287fbb30b1e03aae778', 'audit-permanent-report': 'f6a9001ab2755753de17366349385e0a6168fb84751c11ece034b6e177f65600', 'audit-independent': 'df853bbcc39619161a05b6d6d6a21ab24c1ec31f7fc1e46e65ca39c7012d496e', 'sdk-permanent-report': '105f862e2f1b8b0c4e8ccbb0b91e7618791f053c8799e83346c1679f58dd7517', 'sdk-independent': '9f7811cff6616be92391d209873669013779cfe6a4b1f9bb302b64ceeb98efaf', 'validation-wrapper': 'c3df08850e1865f2f624c0d84780dc854d88a59dea72f6b6ac7085f10f76c22f', 'current-source-proof': 'e2b5e572619456323d934eeeaa0ef7f9b8c13fde932c40982c90f412eefb0bdc', 'current-verifier': 'a41fade72d4a53425c76112e5388e6c9a197fadf0d2aa049c9c21592a4e35d05', 'packer': '9e029778760cc3826c3cc9632637019551ad970e62c8ff63f2c060ddf9459af3', 'current78': '2424c0be1f680beeff8cbbea63b27a7ebbe03a68a2917153278e07ef0007a7e4', 'protected39': '0eabf6fbbc739506432591096a31e0ba713682b54ded7ab26d641661881343aa', 'aliases': 'a49089d2b46fa828f495e58ef550003e96f97d61bb0627efa55ecab0f19b20fd', 'string-bootstrap-original-fail': '89a9211a5a7271a3c7775eb1f31f07c12c96ec02b842f120b3b025b102ab2eb0', 'deletion-unit-original-fail': '22bb8bc85ce099898f548dce6cf507691b1f3ea266a2339a1d739a212ee0e4b1', 'deletion-native-original-fail': '5f3e19150f9d86fa3e675870b4d43f63313f0f52c4b06660ca9ec8f0b82c20b4'}, 'manifest_name': 'task-files-contracts-20261013.json', 'manifest_sha256': '2424c0be1f680beeff8cbbea63b27a7ebbe03a68a2917153278e07ef0007a7e4', 'source_files': 78, 'protected_sha256': '0eabf6fbbc739506432591096a31e0ba713682b54ded7ab26d641661881343aa', 'aliases_sha256': 'a49089d2b46fa828f495e58ef550003e96f97d61bb0627efa55ecab0f19b20fd', 'alias_references': 12764}
CHUNK = 1048576
PRIVATE = re.compile(rb'(?m)^[ \t]*(?:-----BEGIN (?:[A-Z0-9 ]* )?PRIVATE KEY-----|---- BEGIN SSH2 ENCRYPTED PRIVATE KEY ----)[ \t]*\r?$')


class PrivateDetector:
    def __init__(self):
        self.carry=b''
        self.found=False
    def feed(self,block):
        joined=self.carry+block
        end=joined.rfind(b'\n')
        if end>=0:
            self.found=self.found or PRIVATE.search(joined[:end+1]) is not None
            self.carry=joined[end+1:]
        else:
            self.carry=joined
        if len(self.carry)>256:
            self.carry=b'x'+self.carry[-256:]
    def finish(self):
        return self.found or PRIVATE.search(self.carry) is not None

def digest(path):
    h=hashlib.sha256()
    with Path(path).open('rb') as f:
        for b in iter(lambda:f.read(CHUNK),b''):h.update(b)
    return h.hexdigest()

def safe(name):
    return (bool(name) and not name.startswith('/') and not re.match(r'^[A-Za-z]:',name) and '\\' not in name
            and all(x not in ('','.','..') for x in name.split('/'))
            and all(ord(c)>=32 and ord(c)!=127 for c in name)
            and str(PurePosixPath(name))==name)

def private_path(name):
    return any(part.lower().endswith(('.pem','.key')) or part.lower()=='key'
               or any(word in part.lower() for word in ('private_key','private-key','privatekey'))
               for part in PurePosixPath(name).parts)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path,required=True)
    parser.add_argument('--protected',type=Path,required=True)
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args();assert not args.out.exists()
    checks=collections.Counter()
    def check(value,group,description):
        checks[group]+=1
        if not value:raise RuntimeError(description)
    root=args.root.resolve();docs=root/'documents/ros'
    archive=docs/CONTRACT['archive_name'];index_path=docs/CONTRACT['index_name']
    for p,c in ((archive,CONTRACT['archive']),(index_path,CONTRACT['index'])):
        check(p.stat().st_size==c['bytes'] and digest(p)==c['sha256'],'artifact','artifact bytes differ: '+p.name)
    index=json.loads(index_path.read_bytes())
    check(index['status']=='PASS' and index['archive']['sha256']==CONTRACT['archive']['sha256'],'schema','index archive binding differs')
    check(index['selection']['explicit_inputs_only'] and index['selection']['private_keys_excluded']
          and index['selection']['default_payloads_require_utf8'],'schema','selection policy differs')
    payloads=index['payloads'];expected={v['arcname']:v for v in payloads}
    check(len(expected)==len(payloads)==CONTRACT['members'],'schema','index member count differs')
    check(index['summary']['binary_nested_archives']==0 and index['summary']['all_payloads_utf8'],'schema','unexpected non-text evidence')
    check(len(index['inputs'])==CONTRACT['mappings'],'schema','explicit input mapping count differs')
    for v in payloads:
        check(safe(v['arcname']) and not private_path(v['arcname']) and v['kind']=='utf8-text' and v['isutf8'],'schema','unsafe or non-UTF8 index record')
    seen=set();total=0;sha_members=collections.defaultdict(list)
    with gzip.open(archive,'rb') as zipped:
        with tarfile.open(fileobj=zipped,mode='r|') as t:
            for m in t:
                check(m.name in expected and m.name not in seen,'member','unexpected/duplicate archive member')
                check(m.isfile() and safe(m.name),'member','nonregular/unsafe archive member')
                check((m.uid,m.gid,m.mtime,m.uname,m.gname,m.mode)==(0,0,0,'','',0o644),'member','noncanonical tar metadata')
                v=expected[m.name];check(m.size==v['bytes'],'member','member size differs')
                h=hashlib.sha256();n=0;decoder=codecs.getincrementaldecoder('utf-8')('strict');first=True;private=PrivateDetector()
                with t.extractfile(m) as f:
                    for b in iter(lambda:f.read(CHUNK),b''):
                        if first:check(not b.startswith(b'\x7fELF'),'text','ELF mislabeled as UTF8');first=False
                        decoder.decode(b,final=False);h.update(b);n+=len(b)
                        private.feed(b)
                decoder.decode(b'',final=True)
                check(not private.finish(),'text','private key content')
                check(n==v['bytes'] and h.hexdigest()==v['sha256'],'member','member digest differs')
                seen.add(m.name);total+=n;sha_members[v['sha256']].append(m.name)
        while zipped.read(CHUNK):pass
    check(seen==set(expected),'schema','exact member set differs')
    check(total==CONTRACT['payload_bytes']==index['summary']['payload_bytes'],'schema','payload byte total differs')
    check(not any(v.endswith(CONTRACT['index_name']) for v in seen),'coverage','external index was archived')
    required={}
    for label,sha in CONTRACT['required'].items():
        check(bool(sha_members[sha]),'coverage','mandatory actual evidence missing: '+label)
        required[label]={'sha256':sha,'members':sha_members[sha]}
    # The source table excludes its own two metadata members to avoid cycles.
    with tarfile.open(archive,'r:gz') as t:
        data=t.extractfile('planning/evidence-aliases.json').read()
    check(hashlib.sha256(data).hexdigest()==CONTRACT['aliases_sha256'],'aliases','alias document differs')
    aliases=json.loads(data);selected=aliases['selected_payloads']
    check(set(selected)==set(expected)-{'planning/evidence-aliases.json','planning/executed-collector.py'},'aliases','unique source member set differs')
    check(len(aliases['aliases'])==CONTRACT['alias_references'],'aliases','original reference count differs')
    for name,value in selected.items():
        check(safe(name) and (value['sha256'],value['bytes'])==(expected[name]['sha256'],expected[name]['bytes']),'aliases','selected source differs')
    for value in aliases['aliases']:
        name=value['payload_archive']
        check(safe(value['requested_archive']) and safe(name) and name in expected,'aliases','unsafe/missing alias')
        check((value['sha256'],value['bytes'])==(expected[name]['sha256'],expected[name]['bytes']),'aliases','alias content differs')
    manifest=docs/CONTRACT['manifest_name']
    check(digest(manifest)==CONTRACT['manifest_sha256'],'current-source','current source manifest differs')
    records=json.loads(manifest.read_bytes())['files'];check(len(records)==CONTRACT['source_files'],'current-source','source scope count differs')
    coverage=[]
    for v in records:
        check(safe(v['path']),'current-source','unsafe current source path')
        p=root/v['path'];check(p.stat().st_size==v['bytes'] and digest(p)==v['sha256'],'current-source','current source changed: '+v['path'])
        check(bool(sha_members[v['sha256']]),'coverage','current source not archived: '+v['path'])
        coverage.append({**v,'members':sha_members[v['sha256']]})
    check(digest(args.protected)==CONTRACT['protected_sha256'],'preservation','prior protection manifest changed')
    protected=json.loads(args.protected.read_bytes());check(len(protected)==39,'preservation','prior artifact count differs')
    for name,sha in protected.items():check(digest(root/name)==sha,'preservation','prior formal artifact changed: '+name)
    check(digest(archive)==CONTRACT['archive']['sha256'] and digest(index_path)==CONTRACT['index']['sha256'],'artifact','delivery changed during verification')
    result={'status':'PASS','scope':'Downloaded archive, external index, all UTF8 members, current scoped files and 39 protected prior artifacts; no new ROS execution claim',
            'executed_verifier_sha256':digest(__file__),'checks':sum(checks.values()),'check_groups':dict(checks),
            'archive':{'path':str(archive),**CONTRACT['archive']},'external_index':{'path':str(index_path),**CONTRACT['index'],'is_archive_member':False},
            'members':{'total':len(seen),'utf8_text':len(seen),'binary_nested_archive':0,'payload_bytes':total,'exact_set':True,'all_sha256_and_sizes_matched':True,'fixed_metadata':True,'full_gzip_trailer_checked':True},
            'mandatory_evidence':required,'original_evidence_references':CONTRACT['alias_references'],'source_manifest':{'sha256':CONTRACT['manifest_sha256'],'files':coverage},
            'protected_prior_files':{'manifest_sha256':CONTRACT['protected_sha256'],'files':len(protected),'all_sha256_unchanged':True}}
    args.out.write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
    print(json.dumps({'status':'PASS','checks':result['checks'],'members':len(seen),'out':str(args.out)},ensure_ascii=False))

if __name__=='__main__':main()
