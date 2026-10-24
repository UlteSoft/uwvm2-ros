#!/usr/bin/env python3
"""Record only files owned by the ROS extension, preserving unrelated dirty work."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--baseline',type=Path)
    args=parser.parse_args()
    root=Path(__file__).resolve().parents[5]
    files=set()
    directories=['src/uwvm2/imported/ros','src/uwvm2/uwvm/imported/ros',
                 'src/ros/gateway','sdk','examples/ros1','examples/ros2','examples/ros_dual','examples/ros_multimodule','documents/ros']
    directories.extend(str(p.relative_to(root)) for p in (root/'ros-tests').iterdir() if p.is_dir() and p.name.startswith('ros'))
    for directory in directories:
        files.update(p for p in (root/directory).rglob('*') if p.is_file() and '__pycache__' not in p.parts)
    files.update((root/'ros-tests').glob('test_ros*.sh'))
    files.update((root/'src/uwvm2/uwvm/cmdline/params').glob('ros*.h'))
    files.update((root/'src/uwvm2/uwvm/cmdline/callback').glob('ros*.h'))
    modified=['README.md','src/uwvm2/utils/cmdline/handle.h','xmake.lua','xmake/option.lua',
              'src/uwvm2/uwvm/run/run.h','src/uwvm2/uwvm/cmdline/parser.h',
              'src/uwvm2/uwvm/cmdline/params.h','src/uwvm2/uwvm/cmdline/callback/impl.h',
              'src/uwvm2/uwvm/cmdline/callback/help.h','src/uwvm2/uwvm/cmdline/params/impl.h',
              'src/uwvm2/uwvm/cmdline/params/help.h','src/uwvm2/runtime/lib/uwvm_runtime.default.cpp',
              'src/uwvm2/runtime/lib/uwvm_runtime.h','src/uwvm2/runtime/lib/uwvm_stub_runtime.default.cpp']
    files.update(root/path for path in modified)
    files.update(root/path for path in ['documents/command-line/ros1.md','documents/command-line/ros2.md','.github/workflows/ros-extension.yml'])
    tracked=set(subprocess.check_output(['git','ls-files','-z'],cwd=root).decode().split('\0'))
    rows=[]
    for path in sorted(files):
        if path.resolve()==args.output.resolve():continue
        name=str(path.relative_to(root));digest=hashlib.sha256(path.read_bytes()).hexdigest()
        row={'path':name,'kind':'modified' if name in tracked else 'new','sha256':digest,'bytes':path.stat().st_size}
        if args.baseline and (args.baseline/name).is_file():row['task_start_sha256']=hashlib.sha256((args.baseline/name).read_bytes()).hexdigest()
        rows.append(row)
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_text(json.dumps({'scope':'ROS extension owned paths; unrelated working-tree changes excluded',
        'baseline_limitation':'Runtime core files were dirty at task start and have no saved pre-task copies. Their complete HEAD diffs must not be attributed to this task.',
        'modified_count':sum(r['kind']=='modified' for r in rows),'new_count':sum(r['kind']=='new' for r in rows),'files':rows},indent=2)+'\n')
    print('PASS task file manifest',len(rows),args.output)


if __name__=='__main__':main()
