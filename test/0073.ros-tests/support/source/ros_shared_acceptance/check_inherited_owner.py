#!/usr/bin/env python3
"""Exercise maintained guards and two numbered host models in the real owner."""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
from types import SimpleNamespace

ROOT=next(p for p in Path(__file__).resolve().parents if(p/'sdk/origin').is_dir())
TESTS=ROOT/'test/0073.ros-tests';SUPPORT=TESTS/'support'
sys.path.insert(0,str(SUPPORT))
from resource_contract import validate_resource_owner
import owner_guards


def require(condition,message):
    if not condition:raise AssertionError(message)


def sha(path):
    with Path(path).open('rb')as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    proof=validate_resource_owner();work=Path.cwd();cases=[]
    require(work==Path(proof['run'])/'work','owned work directory required')
    def passed(name):cases.append(dict(name=name,status='PASS'))
    path=SUPPORT/'source/sdk-maintenance/tools/build_cgroup_matrix.py'
    spec=importlib.util.spec_from_file_location('maintained_guard',path)
    guards=importlib.util.module_from_spec(spec);spec.loader.exec_module(guards)
    args=SimpleNamespace(out=work/'maintained',expected_leaf=None,expected_device=None,expected_inode=None,
        expected_boot=None,expected_memory_max=64<<30,expected_swap_max='0',expected_cpus='0-31',
        rss_limit_mib=128,cgroup_current_limit_mib=61440,timeout_seconds=15)
    before=guards.cgroup_snapshot(args)
    require(before['leaf_memory_max']is None and before['limit_owner']==proof['parent']
        and before['probe']['effective_memory_max']==64<<30,'truthful inherited-parent proof required')
    require(before['events_scope']=='original-shared-parent'
        and before['accounting']['limits_or_cache_modified']is False,'actual parent counters/accounting required')
    passed('inherited-parent-and-actual-child-proof')
    for field,value in (('expected_leaf',Path(proof['parent'])),('expected_boot','wrong-boot'),
            ('expected_swap_max','max'),('out',ROOT)):
        bad=SimpleNamespace(**vars(args));setattr(bad,field,value)
        try:owner_guards.bind_arguments(bad)
        except ValueError:passed('refuse-'+field)
        else:raise AssertionError('wrong explicit control accepted: '+field)
    row=work/'held-command';row.mkdir()
    command=[sys.executable,'-I','-S','-B','-c','import time;time.sleep(.2);print("owned-child-ok")']
    step=guards.guarded_command(command,work,os.environ.copy(),row,'python-child',args)
    require(step['returncode']==0 and not step['guard_error']and step['cleanup']['status']=='PASS'
        and step['target_executable_observed'],'actual held child guard/cleanup failed')
    require((row/'python-child.log').read_text().strip()=='owned-child-ok','actual child result missing')
    passed('actual-held-exec-membership-rss-and-cleanup')
    after=guards.cgroup_snapshot(args)
    require(all(before[key]==after[key]for key in ('leaf','device','inode','boot_id','limit_owner_identity','swap_max')),
        'original scope/limits changed')
    require(not any(after['events'].get(key,0)-before['events'].get(key,0)
        for key in ('oom','oom_kill','oom_group_kill')),'shared parent OOM changed')
    passed('unchanged-original-limits-and-oom-counters')
    index=json.loads((TESTS/'index.json').read_bytes());native=[]
    for language,tool,standard,suffix in (('c',Path('/usr/bin/cc'),'c11','.c'),
            ('cpp',Path('/usr/bin/c++'),'c++20','.cpp')):
        selected=next(r for r in index['cases']if r['source']=='support/source/sdk-tests/'+language+'/wrapper_contract'+suffix)
        executable=work/('native-'+language)
        includes=[ROOT/'sdk/include',*[ROOT/'sdk/include/platform'/p/k for p in ('ros','ros1','ros2')for k in ('c','cpp')]]
        recipe=dict(kind='explicit-numbered-ros-test-recipe-v1',source_sha256=selected['source_sha256'],
            artifact=str(executable),compile_argv=[str(tool),'-std='+standard,'-O0','-g0','-UNDEBUG',
                *[arg for path in includes for arg in ('-I',str(path))],'{source}','-o','{artifact}'],run_argv=['{artifact}'])
        recipe_path=work/(language+'-recipe.json');recipe_path.write_text(json.dumps(recipe,indent=2)+'\n')
        entry=TESTS/selected['id']/'run.py'
        outcome=subprocess.run([sys.executable,'-I','-S','-B',str(entry),'--execute','--owner',proof['evidence'],
            '--recipe',str(recipe_path)],stdin=subprocess.DEVNULL,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=45)
        log=work/(language+'-native.log');log.write_bytes(outcome.stdout)
        row=dict(language=language,case=selected['id'],returncode=outcome.returncode,
            source_sha256=selected['source_sha256'],compiler_sha256=sha(tool),log_sha256=sha(log))
        if executable.is_file():row['artifact_sha256']=sha(executable)
        native.append(row)
    pins=[dict(path=p.relative_to(ROOT).as_posix(),sha256=sha(p))for p in sorted(ROOT.rglob('*'))if p.is_file()and not p.is_symlink()]
    result=dict(kind='maintained-driver-inherited-owner-v1',status='PASS'if all(r['returncode']==0 for r in native)else'FAILED_RETAINED',
        resource=proof,guard_cases=cases,guard_denominator=7,native_host_models=native,
        native_host_model_denominator=2,actual_numbered_case_workloads=2,source_pins=pins,
        full_SDK_install_qualified=False,complete_source_checkout=False,guest_cases=0,native_ros_cases=0,
        qemu_cases=0,official_upstream_credit=0,default_authority=[])
    (work/'inherited-owner-result.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    require(len(cases)==7 and result['status']=='PASS','maintained driver/native model checks failed; retained logs identify the layer')
    print(json.dumps(dict(status='PASS',guard_cases=7,native_host_models=2,guest_cases=0,native_ros_cases=0)),flush=True)


if __name__=='__main__':main()
