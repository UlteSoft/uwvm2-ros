"""Prepare exact test-block extraction without mutating a source checkout."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import tarfile

PREFIX='test/0073.ros-tests/support/source/sdk-native-languages/inline/'
PINS={
    'rust/src/capability.rs':'eb1135e63ae1f19fc6f4f8cadd5bb6464e0056d09e96b612214b81c521fbe742',
    'rust/src/callback.rs':'5936404135a0c0ed169bcd150c367cc916f679bcf65391b0ff2eead9a924974b',
    'rust/src/events.rs':'64fb1880e49a2cc1c1bd437548026901b7bfaacb3b79af462f1ce6b37f49426b',
    'zig/client.zig':'1f88ff5dd10f0895255bb258d5e8e74a13b66bf34482f8f13096f2aa40ad3423'}

def require(value,message):
    if not value:raise ValueError(message)

def digest(body):return hashlib.sha256(body).hexdigest()

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive',type=Path,required=True);p.add_argument('--sha256',required=True)
    p.add_argument('--tools',type=Path,required=True);a=p.parse_args()
    sys.path.insert(0,str(a.tools.resolve(strict=True)))
    from resource_contract import validate_resource_owner
    proof=validate_resource_owner()
    with a.archive.open('rb')as stream:require(hashlib.file_digest(stream,'sha256').hexdigest()==a.sha256,'exact inert capture required')
    report=dict(kind='sdk-inline-test-extraction-v1',status='PREPARED_NOT_APPLIED',resource=proof,
        archive_sha256=a.sha256,blocks=[],production=[],default_authority=[],actual_tests_executed=0)
    def emit(name,body):
        out=Path('overlay')/name;out.parent.mkdir(parents=True,exist_ok=True);out.write_bytes(body)
    with tarfile.open(a.archive,'r:')as archive:
        extracted={}
        for relative,expected in PINS.items():
            source='sdk/origin/'+relative;body=archive.extractfile(source).read()
            require(digest(body)==expected,'reviewed production preimage changed: '+source)
            blocks=[]
            if relative=='rust/src/events.rs':
                start=body.index(b'\n#[cfg(test)]\nmod tests {')
                end=body.index(b'\n/// Allocation-free local binding',start)
                blocks.append(body[start:end]);body=body[:start]+body[end:]
                start=body.index(b'\n#[cfg(test)]\nmod binding_tests {')
            elif relative.endswith('.rs'):start=body.index(b'\n#[cfg(test)]\nmod tests {')
            else:start=body.index(b'\ntest "length sentinel and quarantine" {')
            blocks.append(body[start:]);body=body[:start]+b'\n'
            require(b'#[cfg(test)]'not in body and b'\ntest "'not in body,'unreviewed residual test block')
            emit(source,body)
            report['production'].append(dict(source=source,before_sha256=expected,after_sha256=digest(body)))
            for number,block in enumerate(blocks,1):
                destination=PREFIX+relative+'.block'+str(number)
                emit(destination,block)
                report['blocks'].append(dict(source=source,destination=destination,bytes=len(block),sha256=digest(block),
                    operation='append in original module scope in a fresh external test-stage copy'))
            extracted[relative]=blocks
        source='sdk/origin/rust/owned_service/src/lib.rs';raw=archive.extractfile(source).read()
        marker=b'\n#[cfg(test)]\nmod contracts;\n'
        require(raw.endswith(marker)and raw.count(marker)==1,'exact owned-service test-module tail required')
        body=raw[:-len(marker)]+b'\n';emit(source,body)
        destination=PREFIX+'rust/owned_service/src/lib.rs.block1';emit(destination,marker)
        report['production'].append(dict(source=source,before_sha256=digest(raw),after_sha256=digest(body)))
        report['blocks'].append(dict(source=source,destination=destination,bytes=len(marker),sha256=digest(marker),
            operation='append module declaration and stage canonical contracts.rs beside the actual module'))
        extracted['rust/owned_service/src/lib.rs']=[marker]
        for backend in('ros','ros1','ros2'):
            for relative,blocks in extracted.items():
                language='rs'if relative.startswith('rust/')else'zig'
                tail=relative.split('/',1)[1]
                public='sdk/include/platform/'+backend+'/'+language+'/'+tail
                raw=archive.extractfile(public).read();body=raw
                for block in blocks:
                    require(body.count(block)==1,'exact extracted public test block required: '+public)
                    body=body.replace(block,b'\n',1)
                require(b'#[cfg(test)]'not in body and b'\ntest "'not in body,'unreviewed public residual test block')
                emit(public,body)
                report['production'].append(dict(source=public,before_sha256=digest(raw),after_sha256=digest(body),
                    operation='remove only exact extracted test blocks; preserve rebound production bytes'))
    Path('inline-extraction-map.json').write_text(json.dumps(report,indent=2,sort_keys=True)+'\n')
    with tarfile.open('inline-overlay.tar','w:')as out:out.add('overlay',arcname='.')
    print(json.dumps({k:v for k,v in report.items()if k not in('blocks','production')}))

if __name__=='__main__':main()
