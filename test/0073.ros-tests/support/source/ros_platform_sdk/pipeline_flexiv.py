#!/usr/bin/env python3
"""Serial Flexiv source generation and qualification, under owner_flexiv only.

Fresh outputs are admitted without executing original vendor source. Preserve
every failure ledger; source availability is separate from native acceptance.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, required=True)
    p.add_argument('--phase', required=True)
    p.add_argument('--attempt', required=True)
    a = p.parse_args()
    root = a.root.resolve(strict=True)
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    kit = source / 'sdk/origin/robots/arms/flexiv'
    profiles = json.loads((kit / 'profiles.json').read_bytes())['profiles']
    sha = lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    def run(argv, deadline=240):
        subprocess.run(list(map(str,argv)),cwd=source,check=True,timeout=deadline)
    python = [sys.executable,'-I','-S','-B']
    acquired = Path(inputs.get('capture_output',root / 'out/acquired'))
    if a.phase == 'capture':
        run([*python,kit / 'tools/capture_sources.py','--source',source,
             '--mirror',root / 'mirror','--out',acquired])
    elif a.phase == 'prepare':
        for name in ('upstream','catalogs'):
            shutil.copytree(acquired / name, kit / name)
        for row in profiles:
            profile = row['id']
            out = root / 'out' / ('prepared-' + profile + '-' + a.attempt)
            argv = [*python,source / 'sdk/origin/robots/industrial/prepare.py',
                '--source-root',source,'--catalog',kit / 'catalogs' / profile / 'catalog.json',
                '--out',out,'--mode','prepare']
            for role,path in [('parent','sdk/origin/tools/custom_types.py'),('bounded','sdk/origin/custom-types/bounded_profile.py'),('adapter','sdk/origin/robots/industrial/vendor_profile.py')]:
                argv += ['--' + role + '-sha256',sha(source / path)]
            run(argv)
            generated = source / 'sdk/origin/custom-types' / ('flexiv-' + profile.replace('_','-') + '-complete')
            # The preparation report owns the generated child directory.
            shutil.copytree(out / 'generated',generated)
            for name in ('preparation.vendor.json',):
                if not (generated / name).exists():
                    shutil.copyfile(out / name,generated / name)
    elif a.phase == 'assemble':
        for script,name,target in [('emit_examples.py','example-sources',kit),('emit_contracts.py','contract-sources',kit / 'tests')]:
            out = root / 'out' / (name + '-' + a.attempt)
            run([*python,kit / 'tools' / script,'--source',source,'--out',out])
            for path in sorted(out.rglob('*')):
                if path.is_file():
                    destination = target / path.relative_to(out)
                    destination.parent.mkdir(parents=True,exist_ok=True)
                    with destination.open('xb') as stream:
                        stream.write(path.read_bytes())
    elif a.phase == 'recover':
        out=root / 'out' / ('recover-' + a.attempt)
        out.mkdir(mode=0o700)
        run([*python,source / 'test/0073.ros-tests/support/source/ros_platform_sdk/recover_platform_sdk_inputs.py',
             '--root',root,'--source-cache',inputs['recovery_volume'],
             '--out',inputs['sdk_recovery_output'],'--record',out / 'recovery.json'], 900)
        for row in json.loads((out / 'recovery.json').read_bytes())['selected_roots']:
            shutil.copyfile(row['manifest_path'],out / (row['name'] + '-files.json'))
    elif a.phase == 'kernel':
        run([*python,source / 'test/0073.ros-tests/support/source/ros_platform_sdk/acquire_dobot_kernel.py',
             '--metadata',inputs['kernel_metadata'],'--out',root / 'out' / ('kernel-' + a.attempt)], 240)
    elif a.phase == 'verify':
        out = root / 'out' / ('verify-' + a.attempt)
        out.mkdir(mode=0o700)
        attempts = inputs['qualification_attempts']
        argv = [*python,source / 'test/0073.ros-tests/support/source/ros_platform_sdk/verify_flexiv.py','--root',root]
        for name in ('direct','managed','finish','cross'):
            argv += ['--'+name,attempts[name]]
        with (out/'verification.json').open('xb') as stdout,(out/'stderr').open('xb') as stderr:
            subprocess.run(list(map(str,argv)),cwd=source,stdout=stdout,stderr=stderr,check=True,timeout=180)
        report = json.loads((out/'verification.json').read_bytes())
        assert report['status']=='INDEPENDENT_FLEXIV_SOURCE_PRODUCTS_REFUSALS_AND_FOUR_ARCH_DATA_PASS'
        run([*python,kit/'tests/validate_sources.py','--source',source])
        print('INDEPENDENT_FLEXIV_VERIFICATION_PASS',flush=True)
    else:
        out = root / 'out' / (a.phase + '-' + a.attempt)
        argv = [*python,source / 'test/0073.ros-tests/support/source/ros_platform_sdk' / (a.phase + '_flexiv.py'),'--root',root,'--out',out]
        if a.phase == 'cross':
            argv += ['--vectors',inputs['vectors']]
        run(argv, 1200)


if __name__ == '__main__':
    main()
