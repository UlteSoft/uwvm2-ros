#!/usr/bin/env python3
"""Resolve a numbered ROS source test without weakening its existing owner.

--describe performs inert metadata IO only. Execution requires an actual live
external owner whose recorded child PID is this process or an ancestor. The
owner retains deadlines, process-tree RSS/output limits and retirement. Native
compiler recipes remain explicit and are never inferred from a filename.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
sys.path.insert(0,str(Path(__file__).resolve().parent))
from resource_contract import validate_resource_owner


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--case',type=Path,required=True)
    p.add_argument('--describe',action='store_true')
    p.add_argument('--execute',action='store_true')
    p.add_argument('--owner',type=Path)
    p.add_argument('--recipe',type=Path,help='Explicit reviewed compile/run argv for a native or Wasm source case')
    p.add_argument('arguments',nargs=argparse.REMAINDER)
    a=p.parse_args();root=Path(__file__).resolve().parent.parent
    case=a.case.resolve(strict=True)
    if not case.is_relative_to(root/'platform'):p.error('numbered test manifest required')
    value=json.loads(case.read_bytes());relative=Path(value['source']);source=root/relative
    if (value['kind']!='numbered-ros-source-test-case-v1'
            or value['id']!=case.parent.relative_to(root).as_posix()
            or relative.is_absolute()or '..'in relative.parts
            or relative.parts[:2]!=('support','source')
            or value['default_authority']!=[]or value['official_upstream_suite_credit']!=0
            or not source.resolve(strict=True).is_relative_to(root/'support/source')):
        p.error('confined original test source required')
    raw=source.read_bytes()
    if len(raw)!=value['source_bytes']or hashlib.sha256(raw).hexdigest()!=value['source_sha256']:
        p.error('actual test source differs from its numbered case pin')
    if a.describe:
        print(json.dumps(value,indent=2,sort_keys=True));return
    if not a.execute or a.owner is None:p.error('explicit execution and active finite owner required')
    try:proof=validate_resource_owner()
    except (OSError,ValueError,KeyError)as error:p.error(str(error))
    if a.owner.resolve(strict=True)!=Path(proof['evidence']).resolve(strict=True):
        p.error('explicit owner must be the independently validated active resource report')
    args=a.arguments[1:]if a.arguments[:1]==['--']else a.arguments
    if a.recipe is not None:
        reviewed=json.loads(a.recipe.read_bytes())
        if reviewed.get('kind')=='explicit-numbered-ros-source-driver-v1':
            if(reviewed.get('source_sha256')!=value['source_sha256']
                    or reviewed.get('scope')!='host-source-model'
                    or value['language']not in('py','js','as','lua','ruby','php','r')):
                p.error('exact source-model recipe required; this route earns no Guest qualification')
            command=reviewed.get('run_argv')
            if(type(command)is not list or not command or command.count('{source}')!=1
                    or any(type(x)is not str or '\0'in x for x in command)):
                p.error('closed explicit source-model argv required')
            executable=Path(command[0])
            if(not executable.is_absolute()or not executable.is_file()
                    or hashlib.sha256(executable.read_bytes()).hexdigest()!=reviewed.get('interpreter_sha256')):
                p.error('actual selected source-model interpreter pin changed')
            os.execv(str(executable),[str(source)if x=='{source}'else x for x in command]+args)
    if value['language']!='py':
        if a.recipe is None:p.error('native/Wasm source case needs an explicit reviewed --recipe')
        recipe=json.loads(a.recipe.read_bytes())
        if recipe.get('kind')!='explicit-numbered-ros-test-recipe-v1'or recipe.get('source_sha256')!=value['source_sha256']:
            p.error('reviewed compiler recipe must bind this exact source')
        artifact=Path(recipe['artifact'])
        if (not artifact.is_absolute()or artifact.exists()or artifact.is_symlink()
                or not artifact.parent.is_dir()
                or not artifact.parent.resolve().is_relative_to(Path(proof['run'])/'work')):
            p.error('fresh explicit artifact path in an existing owned output directory required')
        def argv(key):
            command=recipe[key]
            if type(command)is not list or not command or any(type(x)is not str or '\0'in x for x in command):
                p.error('closed nonempty argv arrays required; no implicit shell')
            return [str(source)if x=='{source}'else str(artifact)if x=='{artifact}'else x for x in command]
        if (recipe['compile_argv'].count('{source}')!=1
                or '{artifact}'not in recipe['compile_argv']or '{artifact}'not in recipe['run_argv']):
            p.error('compiler recipe must name the exact source and artifact')
        status=subprocess.run(argv('compile_argv'),stdin=subprocess.DEVNULL,check=False).returncode
        if status:raise SystemExit(status)
        if not artifact.is_file()or artifact.is_symlink():p.error('compiler did not produce the explicit ordinary artifact')
        command=argv('run_argv')+args
        os.execv(command[0],command)
    os.execv(sys.executable,[sys.executable,'-I','-S','-B',str(source),*args])


if __name__=='__main__':main()
