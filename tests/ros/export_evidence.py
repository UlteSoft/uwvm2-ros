#!/usr/bin/env python3
"""Export bounded reports, logs and explicit test policies; exclude keystores/binaries."""
import argparse
import hashlib
import io
import json
from pathlib import Path
import tarfile


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    root=args.workspace.resolve();output=args.output.resolve()
    candidates=set()
    directories=('reports','rpc','network','additional-security','ingress','revoke-race',
                 'multimodule','five-module','native-ultimate-frozen-all','native-final-gateway-review-v2',
                 'native-final-audit-v3','acceptance-final','acceptance-final-v3-audit',
                 'acceptance-final-v4-service-qos','acceptance-final-v4-service-qos-final',
                 'acceptance-final-v5-scalars','acceptance-final-v5-scalars-final','acceptance-final-v5-scalars-before-library-path',
                 'acceptance-final-v6-events','acceptance-final-v6-events-final',
                 'acceptance-final-v7-arrays','acceptance-final-v7-arrays-final','acceptance-final-v8-io-final',
                 'build/unit-final-sanitized',
                 'build/unit-v4-sanitized','build/schema-v4','build/schema-v4-before-docscope',
                 'build/unit-v5-sanitized','build/schema-v5','build/schema-v5-before-docscope',
                 'build/unit-v6-event-sanitized','build/schema-v6',
                 'build/unit-v7-array-sanitized','build/schema-v7','build/schema-v7-before-final-docscope',
                 'build/unit-v8-io-initial-sanitized','build/unit-v8-format-corrected-sanitized','build/unit-v8-io-sanitized','build/schema-v8',
                 'gateway/explicit-service-qos-native-20261012-190419')
    for directory in directories:
        for path in (root/directory).rglob('*'):
            if path.is_file() and path.suffix in ('.json','.log','.txt','.tsv'):candidates.add(path)
    for product in (root/'product-final-matrix',root/'product-final-v2',root/'product-final-v3',root/'product-final-v4',root/'product-final-v5',root/'product-final-v5-final',root/'product-final-v6-event',root/'product-final-v7-array',root/'product-final-v8-io'):
        for path in product.glob('*.json'):candidates.add(path)
        for profile in ('none','ros1','ros2','dual'):
            for path in (product/profile).glob('*'):
                if path.is_file() and path.suffix in ('.json','.log'):candidates.add(path)
    for path in (root/'build').glob('environment-*.json'):candidates.add(path)
    for path in (root/'gateway').glob('*native-ultimate.log'):candidates.add(path)
    for path in (root/'gateway').glob('*final-frozen-build.log'):candidates.add(path)
    for path in (root/'gateway').glob('*acceptance-witness-build.log'):candidates.add(path)
    for path in (root/'gateway').glob('*explicit_service_qos_build*.log'):candidates.add(path)
    for provenance in ('final-frozen-provenance','final-acceptance-witness-provenance',
                       'final-v4-service-qos-provenance','final-v5-scalar-provenance','final-v5-scalar-final-provenance','final-v5-scalar-release-provenance','final-v5-scalar-ack-release-provenance',
                       'final-v6-event-provenance','final-v6-event-final-provenance','final-v6-event-release-provenance','final-v7-array-provenance','final-v7-array-final-provenance','final-v8-io-provenance','final-v8-io-before-native-environment-provenance'):
        for path in (root/'gateway'/provenance).rglob('*'):
            if path.is_file() and path.suffix in ('.json','.txt','.make','.log'):candidates.add(path)
        for path in (root/'gateway'/provenance/'source-inputs').rglob('*'):
            if path.is_file() and path.suffix in ('.h','.cc','.cpp','.txt'):candidates.add(path)
    for benchmark in ('capability-benchmark-v4','capability-benchmark-v5','capability-benchmark-v6','capability-benchmark-v7','capability-benchmark-v8'):
        for path in (root/'reports'/benchmark).rglob('*'):
            if path.is_file() and path.suffix in ('.h','.cc','.cpp','.d'):candidates.add(path)
    for path in (root/'reports/final-v6-fixture-inputs').glob('*.wat'):
        if path.is_file():candidates.add(path)
    for path in (root/'reports/final-v6-core-inputs').rglob('*'):
        if path.is_file() and path.suffix in ('.h','.cpp','.cc'):candidates.add(path)
    for path in (root/'reports/final-v7-fixture-inputs').glob('*.wat'):
        if path.is_file():candidates.add(path)
    for path in (root/'reports/final-v7-core-inputs').rglob('*'):
        if path.is_file() and path.suffix in ('.h','.cpp','.cc'):candidates.add(path)
    for path in (root/'gateway').glob('*v5*build*.log'):candidates.add(path)
    for directory in ('build/unit-v5-sanitized','reports/scalar-v5-portability','build/unit-v6-event-sanitized','reports/event-v6-portability','build/unit-v7-array-sanitized','reports/array-v7-portability','build/unit-v8-io-sanitized','reports/array-v8-portability'):
        for path in (root/directory).rglob('*'):
            if path.is_file() and path.suffix in ('.h','.hpp','.cc','.cpp','.d'):candidates.add(path)
    for path in (root/'reports/final-v8-fixture-inputs').glob('*.wat'):
        if path.is_file():candidates.add(path)
    for path in (root/'reports/final-v8-core-inputs').rglob('*'):
        if path.is_file():candidates.add(path)
    for directory in ('reports/io-golden-v8','reports/io-golden-v8-final','reports/native-io-v8-golden'):
        for path in (root/directory).rglob('*'):
            if path.is_file() and path.suffix in ('.h','.hpp','.cc','.cpp','.d','.b64','.out','.err'):candidates.add(path)
    entries=[];total=0;output.parent.mkdir(parents=True,exist_ok=True)
    with tarfile.open(output,'w:gz') as archive:
        for path in sorted(candidates):
            if path.resolve()==output:continue
            if path.is_symlink() or not path.resolve().is_relative_to(root):
                raise RuntimeError('evidence must be a regular owned workspace file: '+str(path))
            before=path.stat()
            if before.st_size>8*1024*1024:raise RuntimeError('individual evidence file exceeds8MiB: '+str(path))
            data=path.read_bytes();after=path.stat()
            if b'PRIVATE KEY-----' in data:
                raise RuntimeError('private key marker refused in evidence: '+str(path))
            if (before.st_size,before.st_mtime_ns)!=(after.st_size,after.st_mtime_ns):
                raise RuntimeError('evidence changed during export: '+str(path))
            total+=len(data)
            if total>256*1024*1024:raise RuntimeError('bounded evidence export exceeds256MiB')
            relative=str(path.relative_to(root));digest=hashlib.sha256(data).hexdigest()
            entries.append({'path':relative,'bytes':len(data),'sha256':digest})
            info=tarfile.TarInfo(relative);info.size=len(data);info.mode=0o600
            archive.addfile(info,io.BytesIO(data))
        index=json.dumps({'files':entries,'uncompressed_bytes':total,
                          'includes_private_keys':False,'includes_binaries':False},indent=2).encode()+b'\n'
        info=tarfile.TarInfo('evidence-index.json');info.size=len(index);info.mode=0o600
        archive.addfile(info,io.BytesIO(index))
    result={'status':'PASS','archive':str(output),'files':len(entries),
            'uncompressed_bytes':total,'archive_bytes':output.stat().st_size,
            'archive_sha256':hashlib.sha256(output.read_bytes()).hexdigest()}
    output.with_suffix(output.suffix+'.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2));return 0


if __name__=='__main__':raise SystemExit(main())
