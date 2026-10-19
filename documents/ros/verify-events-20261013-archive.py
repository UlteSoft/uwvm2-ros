#!/usr/bin/env python3
"""Independently verify one frozen ROS delivery without extracting its archive."""
import argparse, codecs, collections, gzip, hashlib, json, re, tarfile
from pathlib import Path, PurePosixPath

CONTRACT = {'archive_name': 'evidence-20261013-events.tgz', 'index_name': 'events-20261013-evidence-index.json', 'archive': {'sha256': '6934f28d18a7916acdcc1d37adf160b3149683b416a1a90cff5c25aa20f5e86c', 'bytes': 47784834}, 'index': {'sha256': 'b6a7dec42e69a31f9390d47f77bb13e39fbfe99fb6aebc7b268b8de8e063939e', 'bytes': 8817136}, 'members': 13840, 'mappings': 3686, 'payload_bytes': 349432206, 'required': {'main-report': '219823d30475e8316517a88bc32b9b3a0318e347b53b4bb3fa478677fe71e1bf', 'main-independent': 'b18f78a10f30f2d312fb925781508ebf9534e5f697b96e6a0ae228ea8137c4c2', 'main-independent-driver': 'b1966c8eb0ad92fe335dcc229986eaa95bb7744c9ccd6879d6bbbe2b4691c49a', 'entry-final-report': '5defd6a36c76dd43feed82b20341bd1562624f46b41b2f7c64c792aa898cb55e', 'entry-final-driver': 'f1d91404733b8702e90b010c92cb838ec46e444efdba03903e56c5c5ad6f0913', 'entry-original-fail': 'f237b17a217a6fab1b308cea2bd659876e30c355885b815a2cddaf88eb31df82', 'entry-original-driver': '34b4685471148bd03b4be35fbef08bec112782cfb5b716f65d01a93d5568711f', 'ipc-unit-final': 'a01f3d87f2c91cacaa1eb6b524c5731a3500f4bb927567c73e775276f770034b', 'ipc-unit-independent': '77a77222dce75b3c18ce9b90b707c87a375cc4e40e4ec5ad2136dd258a9ebb9c', 'sdk-final': 'e9747552fa384f601b812ac6c3bfe54f3779a16201e9467e25fa5121b2a7c3d8', 'sdk-independent': '60ef56fc1360616b4cebc5c34819216de52f9ee6c3895a5624c5571d8d331e03', 'sdk-independent-driver': 'a5340a3397f0012bd36394c06e1e9d595e84eb2a0d3ad05e2a4a42f81749d40e', 'request-independent': '75e64a71bc4804d087cb960ed87dc07b33d4f1146da179fa0ecd58c4ce7d9be4', 'request-independent-driver': 'e35d77496fcd3d68af47260f7ddeb7a76088038c3a4194ba6422289b6c731730', 'unit-independent-driver': '2eeb3cc4b9f939ed52d9538e8476e3adbad4b41e0973dabe555c25c326175111', 'tf-independent': '470d1601f9532d953377656a8cfe706a8b6054e4a8013a68785bdbef88d15858', 'tf-independent-driver': 'c132c1a543be4080033cab294407551bbe75aa3da34b4538c911f569f34a05e6', 'request-before': '12177ab19f9aae2290921724d631bb0847ade6def50038729c4c648720f879fa', 'request-final': '9d5f87b731cc18be0780b85017c94797e8ebb40d4971149eca510f7dd55c09f4', 'tf-original-fail': '4e86d481fed23e435b88345f830a9b125c6802b2934b3433156fce087686780f', 'tf-final': '7cd03d8357f475f5a2f3466a852b2a0795191aa0171eb8f81ad05f1b28cdb48f', 'integration-original-crash': '83bfdfe4bd0616c948f35ba5e670a4c22a96a5f5a64a5964841be07d3da59a35', 'integration-crash-cpp': 'c0cff6881f06b62572d5e5e08b4e0a8a73f1c908c7a70014adf42b3a6bb19d80', 'native-nul-r2-original-fail': 'baf84abef0c981409bc1db943ff86ef7566cc40771569dcd21662bd4a1a7f37c', 'native-nul-r3-original-fail': '17287ec1bef139cbb103cdcfa067c84465c96baf048a4a42865ae38d55ddfad4', 'current-42-manifest': '6df72eff628ac136271c93763fadf0b891cd59ccc3cd0cd93ea836326506cd72', 'candidate3-source-manifest': 'e82a70c651f6e2edcaa8534df3bb7b020996044452fdf4914b6a042180d5c408', 'protected-31-manifest': '182c16cb0e1d4ff13be07169942c52e56be1f87b23c917b13388e8ffc0664368', 'union-remote': '99cbaad6f1e4b981b9795e8d14b390d16f85f58e53d6aa18ce7c9a60f0717914', 'union-local': 'a98842317cc6e557f5d7eb95c96b72c93676b6bc708e6edc7d71f65865d5f58c', 'union-driver': '21461b18bdedca1a766175fd5346594e760cda9a8ad7f0066552ce0642310dc2', 'union-original-fail': 'b801a91d1d08afad225b97b0647585b19294520ce8088b5465a22a3c8cbcad02', 'current-42-independent-binding': 'dc72394566e0131c585bf738bd2ad54e41ed2247abad6294796ecd50a13e1877', 'union-closed-evidence-index': 'ad2c5c2050734de4369290563454cb701ac3edd1ba7847a89a40f582f920c2f3'}, 'manifest_name': 'task-files-events-20261013.json', 'manifest_sha256': '6df72eff628ac136271c93763fadf0b891cd59ccc3cd0cd93ea836326506cd72', 'source_files': 42, 'protected_sha256': '182c16cb0e1d4ff13be07169942c52e56be1f87b23c917b13388e8ffc0664368'}
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
    protected=json.loads(args.protected.read_bytes());check(len(protected)==31,'preservation','prior artifact count differs')
    for name,sha in protected.items():check(digest(root/name)==sha,'preservation','prior formal artifact changed: '+name)
    check(digest(archive)==CONTRACT['archive']['sha256'] and digest(index_path)==CONTRACT['index']['sha256'],'artifact','delivery changed during verification')
    result={'status':'PASS','scope':'Downloaded archive, external index, all UTF8 members, current scoped files and 31 protected prior artifacts; no new ROS execution claim',
            'executed_verifier_sha256':digest(__file__),'checks':sum(checks.values()),'check_groups':dict(checks),
            'archive':{'path':str(archive),**CONTRACT['archive']},'external_index':{'path':str(index_path),**CONTRACT['index'],'is_archive_member':False},
            'members':{'total':len(seen),'utf8_text':len(seen),'binary_nested_archive':0,'payload_bytes':total,'exact_set':True,'all_sha256_and_sizes_matched':True,'fixed_metadata':True,'full_gzip_trailer_checked':True},
            'mandatory_evidence':required,'source_manifest':{'sha256':CONTRACT['manifest_sha256'],'files':coverage},
            'protected_prior_files':{'manifest_sha256':CONTRACT['protected_sha256'],'files':len(protected),'all_sha256_unchanged':True}}
    args.out.write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
    print(json.dumps({'status':'PASS','checks':result['checks'],'members':len(seen),'out':str(args.out)},ensure_ascii=False))

if __name__=='__main__':main()
