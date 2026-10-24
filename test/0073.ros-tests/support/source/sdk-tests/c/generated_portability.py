#!/usr/bin/env python3
"""Check C export candidates under the admitted owner; never publish an SDK."""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tarfile


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--test-sources', type=Path, default=Path(__file__).resolve().parent)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--adapter', type=Path,
        help='explicit current export adapter; defaults to sdk/tools/c_profiles.py')
    parser.add_argument('--portability-header', type=Path,
        help='explicit current canonical C scalar header')
    parser.add_argument('--stream', action='store_true',
        help='compile each transformed header before retiring its temporary copy; retain hashes and diagnostics')
    parser.add_argument('--generated-input-map', type=Path,
        help='explicit relative-header to absolute-source/hash map, allowing read-only pinned corpus reuse')
    parser.add_argument('--generated-archive', type=Path)
    parser.add_argument('--generated-archive-sha256')
    args=parser.parse_args()
    if args.generated_archive and not args.generated_input_map:
        parser.error('--generated-archive requires an explicit current input map')
    if sys.platform!='linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD')!='1':
        parser.error('shared Linux finite owner required')
    sdk=args.sdk.resolve(strict=True);sources=args.test_sources.resolve(strict=True);out=args.output.absolute();out.mkdir()
    adapter = (args.adapter or sdk/'tools/c_profiles.py').resolve(strict=True)
    portability = (args.portability_header or sdk/'include/platform/ros/c/roswasm/detail/common/c_portability.h').resolve(strict=True)
    spec=importlib.util.spec_from_file_location('task02_c_profiles', adapter)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    include=out/'include/platform/ros/c/roswasm/detail/common';include.mkdir(parents=True)
    shutil.copyfile(portability, include/'c_portability.h')
    roots=[sdk/'include/platform']
    headers=sorted({p for root in roots if root.is_dir() for p in root.rglob('*.h') if p.name in module.NAMES})
    header_paths = {header: header.relative_to(sdk) for header in headers}
    selected = None
    outer_archive = None
    source_archive = None
    if args.generated_archive:
        value = hashlib.sha256()
        with args.generated_archive.open('rb') as stream:
            while block := stream.read(1 << 20):
                value.update(block)
        if value.hexdigest() != args.generated_archive_sha256:
            raise ValueError('immutable source archive changed')
        outer_archive = tarfile.open(args.generated_archive)
        source_archive = tarfile.open(fileobj=outer_archive.extractfile('inputs/source.tar'), mode='r:')
    if args.generated_input_map:
        selected = json.loads(args.generated_input_map.read_bytes())
        header_paths = {}
        for relative, entry in selected.items():
            if Path(relative).is_absolute() or '..' in Path(relative).parts:
                raise ValueError('confined relative generated input name required')
            header = out/'source-identities'/relative
            if header.name not in module.NAMES:
                raise ValueError('unknown generated input header: '+relative)
            header_paths[header] = Path(relative)
        headers = sorted(header_paths, key=lambda header: header_paths[header].as_posix())
    if not headers:
        parser.error('at least one actual generated C source is required')
    pins=[];rows=[];index=0
    def command(label, argv):
        nonlocal index
        index+=1;stem=f'{index:04d}-'+label
        result=subprocess.run([str(x) for x in argv], stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=60, check=False)
        (out/(stem+'.stdout')).write_bytes(result.stdout);(out/(stem+'.stderr')).write_bytes(result.stderr)
        record=dict(label=label, argv=[str(x) for x in argv], exit_code=result.returncode, stdout=stem+'.stdout', stderr=stem+'.stderr', status='PASS' if result.returncode==0 else 'FAIL')
        rows.append(record);return result.returncode
    portable=out/'portable'
    def read_header(header):
        relative = header_paths[header].as_posix()
        if selected is None:
            return header.read_bytes()
        entry = selected[relative]
        if 'path' in entry:
            raw = Path(entry['path']).resolve(strict=True).read_bytes()
        else:
            if source_archive is None or entry.get('archive_member') != relative:
                raise ValueError('explicit confined immutable member required')
            try:
                member = source_archive.getmember(relative)
            except KeyError:
                member = source_archive.getmember('./'+relative)
            if not member.isfile() or member.size > 16*1024*1024:
                raise ValueError('ordinary bounded generated header required')
            raw = source_archive.extractfile(member).read()
        if hashlib.sha256(raw).hexdigest() != entry['sha256']:
            raise ValueError('generated source input changed: '+relative)
        return raw

    def stage(header, seen=None):
        seen = set() if seen is None else seen
        if header in seen:
            return
        seen.add(header)
        relative = header_paths[header]
        destination = portable/relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        raw = read_header(header)
        body, transformation = module.transform(header, 'include/platform/candidate/c/'+relative.as_posix(), raw)
        if module._DOCUMENTATION.document_generated(body.decode()).encode() != body:
            raise ValueError('generated documentation is not byte-idempotent: '+relative.as_posix())
        destination.write_bytes(body)
        for name in re.findall(r'^#include\s+"([^"\n]+)"', raw.decode(), re.M):
            dependency = header.parent/name
            if selected is None:
                dependency = dependency.resolve(strict=True)
            if dependency in header_paths:
                stage(dependency, seen)
        return raw, body, transformation
    for header in headers:
        relative=header_paths[header]
        destination=portable/relative;destination.parent.mkdir(parents=True, exist_ok=True)
        raw,body,transformation=stage(header)
        pins.append(dict(path=relative.as_posix(), before_sha256=hashlib.sha256(raw).hexdigest(), after_sha256=hashlib.sha256(body).hexdigest(), transformation=transformation))
        if args.stream:
            source=out/'header.c'
            source.write_text('#include '+json.dumps(str(destination))+'\nint main(void){return 0;}\n')
            for compiler,mode in (('/usr/bin/gcc-15','c89'),('/usr/bin/clang-22','c23')):
                command('header-'+mode, [compiler,'-std='+mode,'-pedantic-errors','-fsyntax-only','-I',sdk/'include','-I',out/'include',source])
            for temporary in destination.parent.iterdir():
                if temporary.name in module.NAMES:
                    temporary.unlink()
    for pin in pins:
        if args.stream:
            break
        header=portable/pin['path'];source=out/'header.c'
        source.write_text('#include '+json.dumps(str(header))+'\nint main(void){return 0;}\n')
        for compiler,mode in (('/usr/bin/gcc-15','c89'),('/usr/bin/clang-22','c23')):
            command('header-'+mode, [compiler,'-std='+mode,'-pedantic-errors','-fsyntax-only','-I',sdk/'include','-I',out/'include',source])
    fixture=sources/'portable_generated_input.h'
    portable_fixture=out/'fixture';portable_fixture.mkdir()
    body,report=module.transform(Path('bounded_records.h'), 'include/platform/fixture/c/bounded_records.h', fixture.read_bytes())
    (portable_fixture/fixture.name).write_bytes(body)
    shutil.copyfile(sources/'portable_generated_main.c', portable_fixture/'main.c')
    original=out/'original';original.mkdir()
    shutil.copyfile(fixture, original/fixture.name)
    shutil.copyfile(sources/'portable_generated_main.c', original/'main.c')
    for directory,mode in [(original,'c11')]+[(portable_fixture,mode) for mode in ('c89','c90','iso9899:199409','c99','c11','c17','c23','c2x','c2y')]:
        exe=out/(directory.name+'-'+mode.replace(':','-'))
        result=command('wire-build-'+directory.name+'-'+mode.replace(':','-'), ['/usr/bin/gcc-15','-std='+mode,'-pedantic-errors','-O2','-I',sdk/'include','-I',out/'include',directory/'main.c','-o',exe])
        if not result:command('wire-run-'+directory.name+'-'+mode.replace(':','-'),[exe])
    report=dict(schema='shared-c-generated-export-portability-v1', source_header_denominator=len(headers), header_compile_denominator=2*len(headers), rows=rows, files=pins,
        adversarial_fixture=report,
        original_wire_fixture_runs=sum(row['label'].startswith('wire-run-original-') and row['status']=='PASS' for row in rows),
        native_ros_runs=0, gateway_runs=0, wasm_guest_runs=0, upstream_official_suite_credit=0,
        public_sdk_published=False, fields_projected=0, streamed=args.stream,
        generated_documentation_token_preservation=True, generated_documentation_byte_idempotence=True,
        physical_field_semantics_scope='Reviewed native reference contracts with explicit source hashes; per-vendor/version binding and all other physical meanings remain separate review gates',
        reviewed_native_reference_record_types=len(module._DOCUMENTATION.GENERATED_FIELD_SEMANTICS),
        adapter_sha256=hashlib.sha256(adapter.read_bytes()).hexdigest(),
        generated_source_archive_sha256=args.generated_archive_sha256,
        portability_header_sha256=hashlib.sha256(portability.read_bytes()).hexdigest(),
        status='FAIL' if any(row['status']=='FAIL' for row in rows) else 'PASS_EXECUTED_SCOPE')
    (out/'generated-c.json').write_text(json.dumps(report,indent=2,sort_keys=True)+'\n')
    print(json.dumps(dict(status=report['status'], headers=len(headers), passed=sum(row['status']=='PASS' for row in rows), failed=sum(row['status']=='FAIL' for row in rows))), flush=True)
    if source_archive is not None:
        source_archive.close()
    if outer_archive is not None:
        outer_archive.close()
    return int(report['status']=='FAIL')


if __name__=='__main__':sys.exit(main())
