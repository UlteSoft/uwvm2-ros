"""Build complete Unitree and existing Python families, then test real Wasm.

Reuses the admitted genuine CPython3.14.8 static producer. All application,
payload, record and correlation checks run inside uwvm2. Original C++ source
drives an independent complete-frame oracle on the resulting bytes. There are
no fake ROS backends, native Python application substitutes or Host relays.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import subprocess
import sys


def pin(path):
    path = Path(path)
    with path.open('rb') as stream: digest = hashlib.file_digest(stream, 'sha256').hexdigest()
    return dict(path=str(path), bytes=path.stat().st_size, sha256=digest)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, required=True); p.add_argument('--out', type=Path, required=True)
    a = p.parse_args(); root, out = a.root.resolve(strict=True), a.out
    inputs = json.loads((root/'inputs.json').read_bytes()); source = Path(inputs['source_root'])
    prepared = root/inputs.get('unitree_python_source','out/unitreepython-r1')
    preparation = json.loads((prepared/'unitreepython.json').read_bytes())
    assert preparation['status'] == 'COMPLETE_UNITREE_API_PYTHON_RECORDS_SPORT_AND_ASYNC_SOURCE_VIEWS_PASS'
    assert out.parent == root/'out' and not out.exists(); out.mkdir(mode=0o700); (out/'commands').mkdir()
    report = dict(status='RUNNING', commands=[], products=[], complete_values=[], default_authority=[],
        runtime_rebuilt=False, native_controller_or_transport_qualified=False, official_suite_cases_run=0)
    def save(): (out/'unitreepythonwasm.json').write_text(json.dumps(report, indent=2)+'\n')
    def run(label, argv, expected=0, timeout=360):
        directory = out/'commands'/label; directory.mkdir()
        row = dict(label=label, argv=list(map(str,argv)), expected_exit=expected, exit=None)
        report['commands'].append(row); save()
        with (directory/'stdout').open('xb') as stdout, (directory/'stderr').open('xb') as stderr:
            result = subprocess.run(row['argv'], cwd=source, env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1'),
                stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, timeout=timeout, check=False)
        row.update(exit=result.returncode, stdout=pin(directory/'stdout'), stderr=pin(directory/'stderr')); save()
        assert result.returncode == expected, (label,result.returncode,(directory/'stderr').read_text()[-2600:])
        return (directory/'stdout').read_bytes(), (directory/'stderr').read_bytes()
    try:
        producer = root/'out/pythonbuild-r2/pythonbuild.json'
        builder = source/'sdk/origin/robots/vendors/build_python_guest.py'
        base = [sys.executable,'-I','-S','-B',builder,'--producer',producer,
            '--producer-sha256',pin(producer)['sha256'],'--validator',inputs['validator']]
        verification = json.loads((root/'out/pythonverify-r3/pythonverify.json').read_bytes())
        assert verification['status'] == 'TWENTY_SIX_VENDOR_PYTHON_FULL_VALUE_AND_REFUSAL_PROFILES_PASS'
        profiles = verification['profiles'] + [dict(vendor='unitree', profile='unitree-api',
            family='unitree-api-complete', ros_version=2, records=8, result=preparation['verification'])]
        kit = source/'sdk/origin/robots/unitree'
        upstream = kit/'upstream/example-review-r1/unitree_ros2/files/example/src'
        spec = importlib.util.spec_from_file_location('unitree_wasm_original_sport',Path(__file__).with_name('unitreesport_remaining.py'))
        oracle = importlib.util.module_from_spec(spec); spec.loader.exec_module(oracle)
        original = oracle.original_operations((upstream/'include/common/ros2_sport_client.h').read_text(),
                                              (upstream/'src/common/ros2_sport_client.cpp').read_text())
        ordered = [(name,original[name]) for name in re.findall(r'^UWVM_UNITREE_SPORT\((\w+),',
            (kit/'include/roswasm_unitree/sport_operations.inc').read_text(),re.M)]
        assert len(ordered) == len(original) == 39
        family = 'unitree-api-complete'
        for version in (2,1):
            rows = [row for row in profiles if row['ros_version'] == version]
            argv = base + ['--out',out/('ros'+str(version))]
            if version == 2: argv += ['--unitree-sport','--unitree-sport-tests']
            for row in rows:
                prefix = source/'sdk/origin/platform'/row['vendor']/'python'
                suffix = 'ros'+str(version)+'/'+row['family']
                argv += ['--example',prefix/'example'/suffix,'--vectors',prefix/'test'/suffix/'vectors.json']
            run('ros'+str(version)+'-public-build',argv)
            build = json.loads((out/('ros'+str(version))/'build.json').read_bytes())
            assert build['status'] == 'REAL_MULTI_PROFILE_CPYTHON_VENDOR_WASM_BUILT_NOT_EXECUTED'
            assert len(build['profiles']) == len(rows) and build['backend'] == version
            artifact = Path(build['artifact']['path']); assert pin(artifact) == build['artifact']
            prefix = [inputs['runtime'],'-Rint','--wasip1-global-noinherit-system-environment','--run',artifact]
            expected = 'PASS_ALL_VENDOR_PYTHON_VALUES %d %d\n' % (
                sum(row['records'] for row in rows),
                sum(row['result']['populated_scalar_or_string_instances'] for row in rows))
            output,error = run('ros'+str(version)+'-all-values',prefix+['all-values'])
            assert output == expected.encode() and not error
            for row in rows:
                selected = row['family']
                output,error = run(selected+'-values',prefix+[selected,'values'])
                assert output == ('PASS_COMPLETE_VENDOR_PYTHON_VALUES %d %d\n' % (
                    row['records'],row['result']['populated_scalar_or_string_instances'])).encode() and not error
                output,error = run(selected+'-corrupt',prefix+[selected,'values-corrupt'],1)
                assert not output and b'/roswasm/vendor_check.py' in error
                for mode in ('raw','state','command'):
                    output,error = run(selected+'-denied-'+mode,prefix+[selected,mode,row['vendor']+'.observation'],13)
                    assert not output and not error
                report['complete_values'].append(dict(family=selected, records=row['records'],
                    populated_scalar_or_string_instances=row['result']['populated_scalar_or_string_instances'],
                    ros_version=version, corrupt_control_refused=True, default_denials=3)); save()
            if version == 2:
                output,error = run('unitree-sport-full-values',prefix+[family,'sport-values'])
                assert not error; oracle.check_frames(output,ordered)
                contract,error = run('unitree-sport-async-contract',prefix+[family,'sport-contract'])
                assert contract.decode() == preparation['contract_line'] and not error
                report['sport_contract_line'] = contract.decode()
                for name,operation in original.items():
                    kind = operation['kind']
                    args = ['1.25','-2.5','.0625'] if kind == 'xyz' else ['-7'] if kind == 'integer' else ['1'] if kind == 'boolean' else []
                    output,error = run('unitree-sport-denied-'+name,
                        prefix+[family,'sport-send','unitree.sport.request',name,*args],13)
                    assert not output and not error
                output,error = run('unitree-query-default-denied',prefix+[family,'sport-query',
                    'unitree.sport.request','unitree.sport.response','1','10','2'],13)
                assert not output and not error
                invalid = [[], ['sport-send'], ['sport-send','alias','Unknown'],
                    ['sport-send','','Damp'], ['sport-send','alias','Move','nan','0','0'],
                    ['sport-send','alias','Move','1e300','0','0'], ['sport-send','alias','Move','1','2'],
                    ['sport-send','alias','Pose','2'], ['sport-send','alias','SpeedLevel','2147483648'],
                    ['sport-query','a','b','1','0','2'], ['sport-query','a','b','1','10','0'],
                    ['sport-query','a','b','0',str(1 << 64),'1']]
                for index,args in enumerate(invalid):
                    output,error = run('unitree-invalid-'+str(index),prefix+[family,*args],22)
                    assert not output and not error
                corrupt = bytearray((out/'commands/unitree-sport-full-values/stdout').read_bytes())
                corrupt[12] ^= 1
                try: oracle.check_frames(corrupt,ordered)
                except AssertionError: report['independent_sport_corrupt_control_refused'] = True
                else: raise AssertionError('corrupted complete Sport API identity accepted')
            else:
                output,error = run('no-implicit-unitree-plugin',prefix+[rows[0]['family'],'sport-send','a','Damp'],22)
                assert not output and not error
            report['products'].append(build['artifact']); save()
        wrong = out/'wrong-backend-sport'
        row = next(row for row in profiles if row['ros_version'] == 1)
        example = source/'sdk/origin/platform'/row['vendor']/'python/example/ros1'/row['family']
        _,error = run('sport-wrong-backend-refused',base+['--example',example,'--unitree-sport','--out',wrong],1)
        assert b'complete Unitree API source family and ROS2 required for Sport' in error
        assert json.loads((wrong/'build.json').read_bytes())['commands'] == []
        bad = out/'implicit-contracts'
        _,error = run('sport-test-option-refused',base+['--example',example,'--unitree-sport-tests','--out',bad],2)
        assert b'explicit --unitree-sport required' in error and not bad.exists()
        report.update(status='COMPLETE_UNITREE_PYTHON_SPORT_ASYNC_AND_TWENTY_SEVEN_PROFILE_REAL_WASM_PASS',
            full_records=sum(row['records'] for row in profiles), profiles=len(profiles), sport_operations=39,
            complete_sport_frames=78, default_denials=3*len(profiles)+40,
            runtime_reused=pin(inputs['runtime']))
        assert report['full_records'] == 1737 and report['profiles'] == 27
        print(report['status'],report['default_denials'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error)); raise
    finally: save()


if __name__ == '__main__': main()
