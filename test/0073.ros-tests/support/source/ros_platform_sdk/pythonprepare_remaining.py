#!/usr/bin/env python3
"""Implement all selected remaining-vendor Python record sources before tests.

Only the paired source parser/emitter runs. No compiled Guest, native driver,
upstream installer, schema substitution or authority change is performed.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess
import sys


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    root, out = a.root.resolve(strict=True), a.out
    inputs = json.loads((root/'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    assert out.parent == root/'out' and not out.exists()
    out.mkdir(mode=0o700)
    report = dict(status='RUNNING', commands=[], profiles=[], default_authority=[],
                  actual_guest_native_or_official_cases_run=0)
    def save():
        (out/'pythonprepare.json').write_text(json.dumps(report, indent=2)+'\n')
    try:
        spec = importlib.util.spec_from_file_location('prior_sdk_layout', source/'sdk/origin/tools/sdk_layout.py')
        layout = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(layout)
        plan = layout.make_plan(source/'sdk/origin')
        plan.check()
        prior = dict(kind='checked-public-sdk-view-source-state-v1', default_authority=[],
            files={n:dict(bytes=len(body), sha256=hashlib.sha256(body).hexdigest())for n,body in plan.files.items()},
            links=plan.links)
        (out/'previous-layout.json').write_text(json.dumps(prior, indent=2, sort_keys=True)+'\n')
        for vendor in ('deep_robotics','pal','trossen','agibot','ubtech','fourier'):
            kit = source/'sdk/origin/robots/platforms'/vendor
            for profile in json.loads((kit/'profiles.json').read_bytes())['profiles']:
                name = vendor+'-'+profile['id']
                destination = out/name
                argv = [sys.executable, '-I', '-S', '-B', source/'sdk/origin/robots/vendors/python_records.py',
                    '--source-root', source, '--catalog', kit/'catalogs'/profile['id']/'catalog.json',
                    '--family', source/'sdk/origin/custom-types'/profile['family'], '--out', destination,
                    '--max-total-nodes', str(profile['source_max_total_nodes'])]
                with (out/(name+'.stdout')).open('xb')as stdout,(out/(name+'.stderr')).open('xb')as stderr:
                    process = subprocess.run(list(map(str,argv)), stdout=stdout, stderr=stderr, check=False, timeout=300)
                report['commands'].append(dict(argv=list(map(str,argv)),exit=process.returncode))
                save()
                assert process.returncode==0,(name,(out/(name+'.stderr')).read_text()[-2600:])
                proof = json.loads((destination/'generation.python.json').read_bytes())
                assert len(proof['records'])>=profile['complete_definitions']
                report['profiles'].append(dict(vendor=vendor,profile=profile['id'],family=profile['family'],
                    ros_version=profile['ros_version'],records=len(proof['records']),output=str(destination)))
                save()
        report.update(status='TWENTY_SIX_FULL_PYTHON_VENDOR_PROFILE_SOURCES_PREPARED',
                      full_records=sum(v['records']for v in report['profiles']))
        assert len(report['profiles'])==26 and report['full_records']==1729
        print(report['status'],report['full_records'])
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error))
        raise
    finally:
        save()


if __name__=='__main__':
    main()
