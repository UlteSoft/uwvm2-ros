#!/usr/bin/env python3
"""Own serial SDK layout preparation/verification in the original 64 GiB group."""
import argparse
import importlib.util
import json
from pathlib import Path
import resource
import sys


def load(name,path):
    spec=importlib.util.spec_from_file_location(name,path)
    value=importlib.util.module_from_spec(spec);spec.loader.exec_module(value);return value


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--phase',choices=('prepare','verify','probe','package','catalog','projects-prepare','projects-verify','projects-frontends','projects-catalog','unitree-prepare','unitree-verify','unitree-catalog','unitree-composition','unitree-views','sport-prepare','sport-verify','sport-catalog','native-reproduce','native-package','native-peer','native-session','native-audit','native-capture','native-catalog','native-proof'),required=True)
    p.add_argument('--attempt',required=True)
    a=p.parse_args();root=a.root.resolve(strict=True);inputs=json.loads((root/'inputs.json').read_bytes())
    source=Path(inputs['source_root']);base=source/'test/0073.ros-tests/support/source/ros_language_integration'
    helper=load('sdk_layout_finite_owner',base/'native_owner.py');budget=load('sdk_layout_budget',base/'cgroup_budget.py')
    class Owner(helper.Owner):
        owned_rss_limit=(1536 if a.phase in('native-peer','native-proof')else 768)<<20
        def guard(self):
            super().guard();sample=budget.sample(self.group);self.report['last_accounting']=sample
            return sample['admission_bytes']
        @staticmethod
        def limits():
            # Linear memory uses unmapped guard windows. Bound virtual address
            # space separately from the unchanged 768 MiB process-tree RSS
            # allowance and original hard64GiB/swap0 physical cgroup limit.
            resource.setrlimit(resource.RLIMIT_AS,(32<<30,)*2)
            resource.setrlimit(resource.RLIMIT_CPU,(1800,)*2)
            resource.setrlimit(resource.RLIMIT_CORE,(0,0))
            resource.setrlimit(resource.RLIMIT_FSIZE,(256<<20,)*2)
            resource.setrlimit(resource.RLIMIT_NOFILE,(1024,)*2)
    name='sdk-layout-'+a.phase+'-'+a.attempt
    assert not(root/'out'/(name+'.json')).exists()
    report=dict(status='RUNNING',phase=a.phase,attempt=a.attempt,default_authority=[],
        official_suites_run=0,current_Runtime_rebuilt=False,resource_limits_modified=False,
        owned_process_rss_limit_bytes=Owner.owned_rss_limit)
    owner=Owner(root,inputs,report,name+'.json')
    try:
        owner.admit()
        for pin in inputs['pins']:
            path=Path(pin['path']);assert path.stat().st_size==pin['bytes']and helper.sha(path)==pin['sha256']
        script=('check_unitree_native.py'if a.phase.startswith('native-')else'check_unitree_sport.py'if a.phase.startswith('sport-')else'check_unitree_public.py'if a.phase.startswith('unitree-')else
            'check_public_projects.py'if a.phase.startswith('projects-')else'layout_distribution_phase.py')
        phase=a.phase.removeprefix('projects-').removeprefix('unitree-').removeprefix('sport-').removeprefix('native-')
        actor=owner.launch(name,[sys.executable,'-I','-S','-B',Path(__file__).with_name(script),
            '--root',root,'--phase',phase,'--out',root/'out'/name],deadline=1800)
        owner.wait(actor)
        for pin in inputs['pins']:
            path=Path(pin['path']);assert path.stat().st_size==pin['bytes']and helper.sha(path)==pin['sha256']
        report['status']='PASS';print('SDK_LAYOUT_OWNED_PHASE_PASS',a.phase,a.attempt,flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:owner.close();owner.save()


if __name__=='__main__':main()
