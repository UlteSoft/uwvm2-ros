#!/usr/bin/env python3
"""Stage real generated RPC examples and emit serial compiler jobs, never run them.

Supply an actual complete source manifest, its SHA256, a freshly emitted complete
12-file guest-only RPC generation, and an explicit tool/path selection. All
copies preserve source-relative imports and are pinned before/after. An external
Linux owner must enforce its cgroup, aggregate memory/reserve, deadlines and
pidfd cleanup when executing the emitted jobs. This program starts no process,
contacts no package registry, opens no ROS resource and manufactures no codec.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import resource
import stat
import time

HERE = Path(__file__).resolve().parent
START = time.monotonic()
TOOLS = ('PYTHON','CARGO','RUSTC','NODE','ASC_JS','TINYGO','WASI_CLANG','WASM_LD_ALIAS',
         'WASI_LIBC','WASI_LIBM','WASI_PRINTSCAN','WASI_BUILTINS','WASM_TOOLS')
DIRECTORIES = ('RUST_SYSROOT','TINYGO_ROOT','GO_ROOT','QUICKJS','WASI_SYSROOT','WASI_RESOURCE_DIR')


def need(value, message):
    if not value:
        raise ValueError(message)


def read(path, maximum=64 * 1024**2, *, collect=True):
    """Bound a physical ordinary input and retain actual stable full-seven facts."""
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute input required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum, 'bounded ordinary input required')
        raw = bytearray(); digest = hashlib.sha256(); total = 0
        while True:
            need(time.monotonic() - START < 180, 'finite source preparation deadline exceeded')
            need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= 256 * 1024**2,
                 'source preparation exceeded256MiB; run only in the documented Linux owner')
            part = os.read(fd, min(1024**2, first.st_size + 1 - total))
            if not part:
                break
            total += len(part); digest.update(part)
            if collect: raw.extend(part)
            need(total <= first.st_size, 'source input grew')
        last, named = os.fstat(fd), path.lstat()
        need(total == first.st_size and all(getattr(first,k) == getattr(last,k) == getattr(named,k)
             for k in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')), 'source input changed')
        return bytes(raw), {'path':str(path),'bytes':total,'sha256':digest.hexdigest(),
            'device':first.st_dev,'inode':first.st_ino,'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns}
    finally:
        os.close(fd)


def javascript_modules(stage):
    """Discover actual staged SDK modules without interpreting ancestor names.

    A legitimate owned output may live below /tmp/tests. Only a tests directory
    inside the actual SDK JavaScript tree is excluded; the outer owner path is
    not part of this module-selection policy. Returned names retain the original
    source-relative module IDs required by the real embedded engine loader.
    """
    base = stage / 'sdk/javascript'
    return [{'name': str(path.relative_to(stage)), 'source': str(path.relative_to(stage))}
            for path in sorted(base.rglob('*.mjs'))
            if 'tests' not in path.relative_to(base).parts]


def prepare(source, manifest, manifest_sha256, generated, selection, out):
    """Materialize the exact current SDK and complete generated modules in fresh work."""
    source, generated = source.resolve(strict=True), generated.resolve(strict=True)
    need(HERE == source / 'sdk/custom-types/interface-examples', 'execute the actual selected source producer')
    need(out.is_absolute() and out.parent.resolve(strict=True) == out.parent and not out.exists(), 'fresh output required')
    need(not any(out.is_relative_to(root) or root.is_relative_to(out) for root in (source,generated)), 'output overlaps an input')
    mf_raw, mf_pin = read(manifest,16*1024**2)
    need(mf_pin['sha256'] == manifest_sha256, 'actual source manifest SHA differs')
    frozen = json.loads(mf_raw)
    before = {}
    for relative,row in frozen.items():
        if not relative.startswith('sdk/'):
            continue
        need(str(Path(relative)) == relative and '..' not in Path(relative).parts, 'unsafe source path')
        raw,pin = read(source / relative)
        need(row == {key:pin[key] for key in ('bytes','sha256')}, 'current SDK source differs from complete manifest')
        before[relative] = pin
    need('sdk/custom-types/interface-examples/prepare.py' in before, 'producer absent from actual MF')
    recipe_raw = read(source / 'sdk/custom-types/interface-examples/build-recipes.json')[0]
    recipe = json.loads(recipe_raw)
    need(recipe['format'] == 'uwvm-custom-rpc-example-build-recipes-v1' and recipe['default_authority'] == [], 'closed recipe required')
    proof_raw, proof_pin = read(generated / 'generation.json',4*1024**2)
    proof = json.loads(proof_raw)
    outputs = recipe['preparation']['required_generated_outputs']
    need(proof['status'] == 'GENERATED_GUEST_SOURCE_ONLY' and proof['native_registry_emitted'] is False and
         len(outputs) == len(set(outputs)) == 12 and set(proof['outputs']) == set(outputs), 'complete real guest-only RPC generation required')
    for field,relative in (('generator_sha256','sdk/tools/custom_types.py'),('bounded_generator_sha256','sdk/custom-types/bounded_profile.py')):
        need(proof[field] == before[relative]['sha256'], 'generation used another actual generator revision')
    catalog = before['sdk/custom-types/interface-examples/catalog.preparation.json']
    need(proof['source_inputs']['catalog'] == {key:catalog[key] for key in ('bytes','sha256')}, 'actual RPC catalog differs')
    definitions = proof['source_inputs']['definitions']
    need(len(definitions) == 2 and {row['file'] for row in definitions} == {'schema/srv/Increment.srv','schema/action/Count.action'}, 'complete exact RPC definitions required')
    for row in definitions:
        pin = before['sdk/custom-types/interface-examples/' + row['file']]
        need(all(row[key] == pin[key] for key in ('bytes','sha256')), 'actual RPC definition differs')
    templates = proof['source_inputs']['facade_templates']
    need(len(templates) == 4 and {row['generator_relative'] for row in templates} ==
         {'interface_templates/bounded_interfaces.'+suffix+'.in' for suffix in ('rs','mjs','go','ts')}, 'complete exact RPC templates required')
    for row in templates:
        pin = before['sdk/custom-types/' + row['generator_relative']]
        need(all(row[key] == pin[key] for key in ('bytes','sha256')), 'actual RPC template differs')
    generated_pins = {}
    for name in outputs:
        raw,pin = read(generated / name)
        need(proof['outputs'][name] == {key:pin[key] for key in ('bytes','sha256')}, 'actual generated output differs')
        generated_pins[name] = pin
    selected_raw, selected_pin = read(selection,1024**2)
    values = json.loads(selected_raw)
    need(type(values) is dict and set(values) == set(TOOLS + DIRECTORIES), 'closed actual tool/path selection required')
    tool_pins = {}
    for name in TOOLS:
        alias = Path(values[name])
        need(alias.is_absolute() and alias.parent.resolve(strict=True) == alias.parent, 'actual selected tool alias required')
        tool_pins[name] = read(alias.resolve(strict=True),512*1024**2,collect=False)[1]
        values[name] = str(alias)
    for name in DIRECTORIES:
        path = Path(values[name]);need(path.is_absolute() and path.resolve(strict=True) == path and path.is_dir(), 'physical selected SDK directory required')
    out.mkdir(mode=0o700);stage=out/'source';stage.mkdir(mode=0o700)
    copies=[];total=0
    def copy(original, destination):
        nonlocal total
        raw,pin=read(original);total+=len(raw);need(total<=128*1024**2,'finite SDK copy budget exceeded')
        destination.parent.mkdir(parents=True,exist_ok=True)
        with destination.open('xb') as stream:
            stream.write(raw);stream.flush();os.fsync(stream.fileno())
        copied=read(destination)[1]
        need(copied['inode']!=pin['inode'] or copied['device']!=pin['device'],'independent copy inode required')
        need(all(copied[key]==pin[key] for key in ('bytes','sha256')),'copy bytes differ')
        copies.append({'original':pin,'copy':copied})
    for relative in sorted(before):
        copy(source/relative,stage/relative)
    for name in outputs:
        copy(generated/name,stage/'sdk/custom-types/generated-rpc'/name)
    for name in ('bounded_records.go','bounded_interfaces.go'):
        copy(generated/name,stage/'sdk/custom-types/interface-examples/customtypes'/name)
    modules=javascript_modules(stage)
    modules += [{'name':relative,'source':relative} for relative in (
        'sdk/custom-types/generated-rpc/bounded_records.mjs','sdk/custom-types/generated-rpc/bounded_interfaces.mjs',
        'sdk/custom-types/interface-examples/example.mjs')]
    js={'schema':'uwvm-embedded-javascript-build-v1','mode':'reactor','backend':2,
        'entry':'sdk/custom-types/interface-examples/example.mjs','modules':modules,'imports':[],'exports':[],
        'limits':{'heap_bytes':32*1024**2,'stack_bytes':1024**2,'max_jobs':4096,'interrupt_polls':1000000,
            'input_bytes':32768,'output_bytes':32768,'print_bytes':4096,'max_text_bytes':32768,'max_modules':256,'max_source_bytes':8*1024**2},
        'random_seed':1,'optimization':'-O2','initial_memory':16*1024**2,'maximum_memory':64*1024**2}
    with (out/'javascript-manifest.json').open('x') as stream:json.dump(js,stream,indent=2);stream.write('\n')
    values.update(SOURCE=str(source),GENERATED=str(generated),STAGE=str(stage),OUT=str(out))
    def expand(value):
        result=value.format_map(values);need(not re.search(r'\{[^}]+\}',result),'unknown recipe placeholder');return result
    jobs=[{**row,'argv':[expand(value) for value in row['argv']], 'cwd':expand(row['cwd']),
           'environment':{key:expand(value) for key,value in row['environment'].items()},'artifact':expand(row['artifact'])}
          for row in recipe['jobs']]
    need([row['id'] for row in jobs] == ['rust-library','assemblyscript-library','tinygo-wire-example','embedded-javascript-preparation'], 'exact four jobs required')
    for relative,pin in before.items():need(read(source/relative)[1]==pin,'original SDK input changed')
    for name,pin in generated_pins.items():need(read(generated/name)[1]==pin,'original generated input changed')
    need(read(manifest,16*1024**2)[1]==mf_pin and read(selection,1024**2)[1]==selected_pin and read(generated/'generation.json',4*1024**2)[1]==proof_pin,'original metadata changed')
    for name,pin in tool_pins.items():need(read(Path(values[name]).resolve(strict=True),512*1024**2,collect=False)[1]==pin,'selected physical tool changed')
    report={'format':'uwvm-custom-rpc-example-preparation-v1','status':'PREPARED_SOURCE_AND_JOBS_NOT_COMPILED',
            'source':str(source),'source_manifest':mf_pin,'generation':proof_pin,'selection':selected_pin,
            'copies':copies,'tools':tool_pins,'jobs':jobs,'compiler_executed':False,'Runtime_ROS_executed':False,
            'granted_rights':[],'original_selected_file_inputs_preserved':True,'scope':'SOURCE_PREPARATION_ONLY_NO_NATIVE_TYPE_REGISTRATION_OR_BEHAVIOR_CLAIM'}
    with (out/'report.json').open('x') as stream:json.dump(report,stream,indent=2);stream.write('\n');stream.flush();os.fsync(stream.fileno())
    return report


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('source','manifest','generated','selection','out'):parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--manifest-sha256',required=True)
    args=parser.parse_args()
    report=prepare(args.source,args.manifest,args.manifest_sha256,args.generated,args.selection,args.out)
    print(json.dumps({'status':report['status'],'jobs':len(report['jobs']),'report':str(args.out/'report.json')}))


if __name__=='__main__':main()
