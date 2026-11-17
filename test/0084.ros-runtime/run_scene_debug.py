#!/usr/bin/env python3
"""Compile and verify scene console/DAP/editor contracts inside the existing gate.

Records exact input hashes, commands, stdout/stderr and cgroup state. This is
focused protocol and controller qualification; it does not launch native ROS.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def gate():
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736'
    assert (GROUP/'memory.swap.max').read_text().strip() == '0'


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',required=True,type=Path)
    args=parser.parse_args();gate();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    paths=['src/uwvm2/imported/ros/scene/debug.h','src/uwvm2/imported/ros/scene/component_fault.h','src/uwvm2/uwvm/debugger/scene_state.h',
           'tools/debug/dap_adapter.py','tools/debug/inspection_ui.js','tools/debug/inspection_model.js',
           'tools/debug/media/inspector.js','test/0084.ros-runtime/scene_debug.cc',
           'test/0084.ros-runtime/test_scene_dap.py','test/0084.ros-runtime/test_scene_ui.cjs',
           'test/0084.ros-runtime/run_scene_debug.py']
    pins=lambda:{p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in paths}
    report=dict(scope='scene console/DAP/editor contract qualification; no real VM or graphical IDE claim',
                cgroup=str(GROUP),cgroup_inode=GROUP.stat().st_ino,memory_max='68719476736',swap_max='0',
                events_before=(GROUP/'memory.events').read_text(),inputs=pins(),runs=[],complete=False)
    def save(): (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv):
        gate();begin=time.monotonic();result=subprocess.run(argv,cwd=ROOT,capture_output=True,timeout=180)
        (out/(label+'.stdout')).write_bytes(result.stdout);(out/(label+'.stderr')).write_bytes(result.stderr)
        report['runs'].append(dict(label=label,argv=argv,returncode=result.returncode,seconds=time.monotonic()-begin));save()
        if result.returncode:raise RuntimeError((label,result.stderr.decode(errors='replace')[-5000:]))
        print('PASS',label,flush=True)
    binary=str(out/'scene-debug')
    run('compile',[shutil.which('clang++'),'-std=c++2c','-Wall','-Wextra','-Werror','-O3','-g',
                   '-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread','-Isrc',
                   '-Ithird-parties/fast_io/include','test/0084.ros-runtime/scene_debug.cc','-o',binary])
    run('formatter',[binary])
    run('dap',[sys.executable,'test/0084.ros-runtime/test_scene_dap.py','--formatter',binary])
    portable=str(out/'scene-debug-gcc13')
    run('gcc13-compile',[shutil.which('g++-13'),'-std=c++23','-Wall','-Wextra','-Werror','-O3','-g','-pthread',
                         '-Isrc','-Ithird-parties/fast_io/include','test/0084.ros-runtime/scene_debug.cc','-o',portable])
    run('gcc13-formatter',[portable])
    run('scene-editor',[shutil.which('node'),'test/0084.ros-runtime/test_scene_ui.cjs'])
    run('editor-regression',[shutil.which('node'),'test/0018.debugger/test_inspector_views.cjs'])
    run('editor-lifetime',[shutil.which('node'),'test/0018.debugger/test_inspector_lifecycle.cjs'])
    report['events_after']=(GROUP/'memory.events').read_text();report['inputs_after']=pins()
    assert report['inputs']==report['inputs_after'],'inputs changed during tests'
    report['complete']=True;save()


if __name__=='__main__':main()
