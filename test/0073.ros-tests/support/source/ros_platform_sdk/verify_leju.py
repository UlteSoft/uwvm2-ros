#!/usr/bin/env python3
"""Independently read actual Leju command/product/refusal/QEMU evidence.

This verifier executes no compiler, Wasm, QEMU or ROS operation. It checks raw
closed run data and file contents; native controller/official/performance credit
remains zero. Run it under the same finite Linux test owner before retention.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def evidence(path):
    return json.loads(path.read_bytes())


def check_pin(pin):
    p = Path(pin['path'])
    assert p.is_file() and p.stat().st_size == pin['bytes'] and sha(p) == pin['sha256'], p


def check_memory(path):
    """Independently inspect the actual core memory declaration, not a receipt."""
    raw = Path(path).read_bytes()
    assert raw[:8] == b'\0asm\1\0\0\0'
    def number(at):
        value = 0
        for n in range(5):
            assert at < len(raw)
            b = raw[at]
            at += 1
            assert n != 4 or b < 16
            value |= (b & 127) << (7*n)
            if b < 128:
                return value, at
        raise AssertionError('overlong core length')
    at, memories = 8, []
    while at < len(raw):
        section = raw[at]
        extent, begin = number(at + 1)
        end = begin + extent
        assert end <= len(raw)
        if section == 5:
            count, cursor = number(begin)
            assert count == 1
            flags, cursor = number(cursor)
            assert flags == 1  # One nonshared 32-bit defined memory with max.
            minimum, cursor = number(cursor)
            maximum, cursor = number(cursor)
            assert cursor == end and minimum == 256 and maximum == 1024
            memories.append((minimum, maximum))
        at = end
    assert memories == [(256, 1024)]


def check_commands(report):
    names = set()
    for row in report['commands']:
        assert row['label'] not in names
        names.add(row['label'])
        assert row['exit'] == row.get('expected_exit', 0), row['label']
        for name in ('stdout', 'stderr'):
            if name in row:
                check_pin(row[name])
    return len(names)


def verify(root, direct, managed, finish, cross):
    root = Path(root).resolve(strict=True)
    inputs = evidence(root / 'inputs.json')
    source = Path(inputs['source_root'])
    profiles = evidence(source / 'sdk/origin/robots/humanoids/leju/profiles.json')['profiles']
    assert len(profiles) == 9
    nprofiles=len(profiles)
    peaks,owners = {},{}
    for phase,attempt in [('qualify',direct),('managed',managed),('finish',finish),('cross',cross)]:
        owner = evidence(root / 'out' / ('leju-'+phase+'-'+attempt+'.json'))
        assert owner['status'] == 'PASS' and owner['all_admitted_source_and_tool_pins_preserved']
        for name in ('events_before','events_after'):
            events = dict(line.split() for line in owner[name].splitlines())
            assert all(events[k]=='0' for k in ('oom','oom_kill','oom_group_kill'))
        assert all(r['exit']==0 and r['forced_signal'] is None for r in owner['commands'])
        peaks[phase] = max(r['max_sampled_owned_rss'] for r in owner['commands'])
        assert peaks[phase] <= owner['owned_aggregate_rss_limit_bytes'] == 1536 << 20
        owners[phase] = owner
    results = {}
    q = evidence(root / 'out' / ('qualify-'+direct) / 'qualification.json')
    m = evidence(root / 'out' / ('managed-'+managed) / 'managed.json')
    c = evidence(root / 'out' / ('cross-'+cross) / 'cross.json')
    f = evidence(root / 'out' / ('finish-'+finish) / 'finish.json')
    assert q['status']=='LEJU_C_CPP_COMPLETE_DATA_AND_DEFAULT_REFUSAL_PASS'
    assert m['status']=='LEJU_FOUR_MANAGED_LANGUAGE_PRODUCTS_AND_EXACT_REFUSALS_PASS'
    assert c['status']=='LEJU_FOUR_ARCH_USER_QEMU_COMPLETE_DATA_CONTRACTS_PASS'
    assert f['status']=='LEJU_EXACT_SOURCE_PROFILE_CMAKE_AND_REPRODUCTION_PASS'
    counts = {name:check_commands(r) for name,r in [('qualify',q),('managed',m),('cross',c),('finish',f)]}
    record_count,fields,vector_programs = 0,0,0
    for p in profiles:
        key = p['id']
        vector = evidence(root / 'out' / ('qualify-'+direct) / ('golden-vectors-'+key) / 'vectors.json')
        family = source / 'sdk/origin/custom-types' / ('leju-'+key.replace('_','-')+'-complete')
        gen = evidence(family / 'generation.vendor.json')
        assert vector['records']==len(gen['records'])
        record_count += vector['records']
        fields += vector['scalar_or_string_field_instances']
        vector_programs += len(vector['products'])
        assert gen['default_authority']==gen['implicit_permissions']==[] and not gen['native_registry_emitted']
        for name,pin in gen['outputs'].items():
            assert (family / name).stat().st_size==pin['bytes'] and sha(family / name)==pin['sha256']
        lock = evidence(source / 'sdk/origin/robots/humanoids/leju/catalogs' / key / 'source-lock.json')
        results[key] = dict(definitions=len(lock['definitions']),original_roots=len(lock['original_custom_roots']),records=vector['records'],scalar_or_string_instances=vector['scalar_or_string_field_instances'])
    assert sum(r["original_roots"] for r in results.values())==1101
    assert len(q['profiles'])==nprofiles and sum(r['complete_records'] for r in q['profiles'])==record_count
    assert sum(r['scalar_or_string_field_instances'] for r in q['profiles'])==fields
    assert len(q['artifacts'])==4*nprofiles+2*vector_programs
    assert len(m['artifacts'])==4*nprofiles
    for row in q['artifacts']+m['artifacts']:
        check_pin(row)
        check_memory(row['path'])
    sizes = {}
    for row in q['artifacts']+m['artifacts']:
        if row['label'].endswith('-app') or row in m['artifacts']:
            sizes[row['label']] = dict(bytes=row['bytes'],sha256=row['sha256'])
    alias = b'leju.observation'
    for report,expected in [(q,6*nprofiles),(m,8*nprofiles)]:
        denied = [r for r in report['commands'] if '-denied-' in r['label']]
        assert len(denied)==expected
        for row in denied:
            assert Path(row['stderr']['path']).read_bytes()==b''
            if 'javascript' in row['label']:
                assert row['exit']==0 and Path(row['stdout']['path']).read_bytes()==struct.pack('<II',13,0)
            else:
                assert row['exit']==13 and Path(row['stdout']['path']).read_bytes()==b''
            if 'javascript' in row['label'] or 'assemblyscript' in row['label']:
                data = bytes([int(row['label'].rsplit('-',1)[1]),len(alias)])+alias
                assert row['stdin_bytes']==len(data) and row['stdin_sha256']==hashlib.sha256(data).hexdigest()
    assert f['accepted_profiles']==nprofiles and f['wrong_domain_refusals']==nprofiles and f['default_off_confirmed']
    finish_denied = 0
    for row in f['commands']:
        directory = root / 'out' / ('finish-'+finish) / row['label']
        for name in ('stdout','stderr'):
            assert sha(directory / name)==row[name+'_sha256']
        if '-denied-' in row['label']:
            finish_denied += 1
            assert row['exit']==13 and all((directory / n).read_bytes()==b'' for n in ('stdout','stderr'))
    assert finish_denied==6*nprofiles
    # Inspect the real CMake products as well as their closed command logs.
    # A reused build directory is data, not an adopted earlier test result.
    cmake_root = Path(f['closed_build_cache_reused']) if f['closed_build_cache_reused'] else root / 'out' / ('finish-'+finish)
    assert cmake_root.parent == root / 'out'
    cmake_products = []
    for profile in profiles:
        for name in ('leju_contract_c', 'leju_contract_cpp', 'leju_guest_c', 'leju_guest_cpp'):
            actual = cmake_root / ('wasi-'+profile['id']) / (name+'.wasm')
            check_memory(actual)
            cmake_products.append(dict(profile=profile['id'], name=name,
                                       bytes=actual.stat().st_size, sha256=sha(actual)))
    machines = {'x86_64':(2,62),'i686':(1,3),'aarch64':(2,183),'armhf':(1,40)}
    assert {r['architecture'] for r in c['targets']}==set(machines)
    qemu_runs = 0
    for row in c['targets']:
        assert row['contracts']==2*nprofiles and row['independent_vector_programs']==vector_programs
        assert row['registry_boundary_programs'] == 2
        assert len(row['artifacts'])==2*nprofiles+vector_programs+2
        check_pin(row['emulator'])
        check_pin(row['production_cpp_runtime'])
        for pin in row['artifacts']:
            check_pin(pin)
            header = Path(pin['path']).read_bytes()[:64]
            assert header[:4]==b'\x7fELF' and header[5]==1
            assert (header[4],int.from_bytes(header[18:20],'little'))==machines[row['architecture']]
        qemu_runs += len(row['artifacts'])
    # The real SDK bodies consumed by every closed phase must match delivery.
    for phase,owner in owners.items():
        prior = Path(owner['inputs']['source_root'])
        pins = {str(Path(r['path']).relative_to(prior)):r for r in owner['inputs']['pins'] if Path(r['path']).is_relative_to(prior)}
        for prefix in ['sdk/origin/robots/humanoids/leju/include','sdk/origin/robots/humanoids/leju/examples'] + ['sdk/origin/custom-types/leju-'+p['id'].replace('_','-')+'-complete' for p in profiles]:
            for path in (source / prefix).rglob('*'):
                if not path.is_file() or path.suffix=='.md' or path.name in ('Cargo.lock','profile-selection.json'):
                    continue
                # Direct and cross lanes consume C/C++ source only. Later
                # recipe-only input quota corrections do not change those
                # already verified C/C++ bodies or their execution results.
                if phase in ('qualify','cross') and path.suffix in ('.json','.ts','.mjs','.go','.rs'):
                    continue
                relative = str(path.relative_to(source))
                assert relative in pins,(phase,relative)
                assert path.stat().st_size==pins[relative]['bytes'] and sha(path)==pins[relative]['sha256'],(phase,relative)
    delivery_rows=evidence(root/'out/source-delivery-final.json')['files']
    delivered=set()
    for row in delivery_rows:
        relative=Path(row['path'])
        assert not relative.is_absolute()and '..'not in relative.parts
        assert row['path']not in delivered
        delivered.add(row['path'])
        actual=source/relative
        assert actual.is_file()and not actual.is_symlink()
        assert actual.stat().st_size==row['bytes']and sha(actual)==row['sha256']
    expected=set()
    roots=[source/'sdk/origin/robots/humanoids/leju',
           *[source/'sdk/origin/custom-types'/('leju-'+p['id'].replace('_','-')+'-complete')for p in profiles]]
    for selected in roots:
        expected.update(str(p.relative_to(source))for p in selected.rglob('*')
                        if p.is_file()and p.name!='Cargo.lock')
    assert delivered==expected,'Current SDK delivery omitted or added a source body'
    empty_count = sum(len(evidence(source/'sdk/origin/robots/humanoids/leju/catalogs'/p['id']/'source-lock.json')['source_only_empty_topics'])for p in profiles)
    return dict(status='INDEPENDENT_LEJU_SOURCE_PRODUCTS_REFUSALS_AND_FOUR_ARCH_DATA_PASS',
        profiles=results,complete_records=record_count,populated_scalar_or_string_instances=fields,
        commands=counts,managed_products=4*nprofiles,managed_refusals=8*nprofiles,c_cpp_refusals=6*nprofiles,cmake_refusals=6*nprofiles,
        qemu_program_runs=qemu_runs,qemu_architectures=list(machines),sampled_owned_peak_rss_bytes=peaks,
        inspected_cmake_wasm_products=cmake_products,
        application_products=sizes,runtime=dict(bytes=Path(inputs['runtime']).stat().st_size,sha256=sha(inputs['runtime'])),
        source_only_empty_native_topics=empty_count,
        current_delivered_sdk_files_matching_executed_snapshot=len(delivery_rows),
        default_authority=[],original_official_cases_run=0,native_robot_delivery=False,
        scope='Selected authored SDK/data/default-refusal qualification; native ROS/driver/controller, whole official suites, other releases, cross-host Runtime and hardware counters remain separate')


if __name__=='__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    for name in ('direct','managed','finish','cross'):
        p.add_argument('--'+name)
    a = p.parse_args()
    attempts = evidence(a.root / 'inputs.json')['qualification_attempts']
    result = verify(a.root, *[getattr(a, name) or attempts[name] for name in ('direct','managed','finish','cross')])
    assert a.out.parent == a.root / 'out' and not a.out.exists()
    a.out.mkdir(mode=0o700)
    (a.out / 'verification.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], flush=True)


