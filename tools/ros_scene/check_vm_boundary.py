#!/usr/bin/env python3
"""Check ROSv2's source ownership boundary and one optional exact VM artifact.

This is a structural regression check, not a complete syscall sandbox proof.
Traverse ROSv2's actual local header closure, reject historical process owners
and native vendor adapters, and bind the reviewed files to SHA-256 digests.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT/'test/0084.ros-runtime'))
from deployment_fixture import check_gate, GATE


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, default=ROOT)
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    check_gate()
    root = args.source_root.resolve()
    scene = root/'src/uwvm2/imported/ros/scene'
    removed = {'component_dispatch.h', 'component_policy.h', 'model_worker.h', 'model_worker_manifest.h',
               'model_sandbox.h', 'communication_worker_sandbox.h', 'communication_worker_manifest.h',
               'linux_process_domain.h', 'execution_placement.h'}
    report = dict(complete=False, scope=__doc__, cgroup=str(GATE), source_root=str(root),
                  files={}, failures=[], removed_process_owners=sorted(removed))
    for name in removed:
        if (scene/name).exists():
            report['failures'].append('historical process owner remains in production: '+name)
    pending = [scene/'deployment.h', scene/'broker.h']
    while pending:
        path = pending.pop().resolve()
        relative = str(path.relative_to(root))
        if relative in report['files']:
            continue
        text = path.read_text()
        report['files'][relative] = sha(path)
        if path.is_relative_to(scene/'native'):
            report['failures'].append('native ROS implementation enters VM closure: '+relative)
        if re.search(r'::(?:fork|vfork|execve|execv|execvp|posix_spawn|setns|unshare|kill|waitpid)\s*\(|\bSYS_(?:clone3?|execve|pidfd_send_signal)\b|cgroup\.(?:procs|kill)', text):
            report['failures'].append('process/namespace authority in ROSv2 closure: '+relative)
        for kind, name in re.findall(r'#\s*include\s*(["<])([^">]+)[">]', text):
            target = (path.parent/name if kind == '"' else root/'src'/name).resolve()
            if target.is_relative_to(scene) and target.is_file():
                pending.append(target)
    if args.runtime:
        runtime = args.runtime.resolve()
        receipt = runtime.parent/'full-build-results.json'
        build = json.loads(receipt.read_text())
        if not build['complete'] or build['binary_sha256'] != sha(runtime) or Path(build['source_root']) != root:
            report['failures'].append('runtime does not match complete build receipt')
        for command in build['commands']:
            if any(arg.startswith(('-DUWVM_ENABLE_PRELOAD_DL', '-DUWVM_SUPPORT_PRELOAD_DL', '-DUWVM_SUPPORT_WEAK_SYMBOL')) for arg in command['argv']):
                report['failures'].append('unsafe native provider is compiled in default qualification build')
        dynamic = subprocess.run(['readelf', '-d', str(runtime)], capture_output=True, text=True, check=True).stdout
        needed = re.findall(r'\(NEEDED\).*?\[(.*?)\]', dynamic)
        if any(re.search(r'(?:rclcpp|librcl\.|rmw_|fastrtps|fastdds|ddsc|mujoco)', name) for name in needed):
            report['failures'].append('external provider linked into VM')
        report.update(runtime_sha256=sha(runtime), build_receipt_sha256=sha(receipt), needed_libraries=needed)
    report['complete'] = not report['failures']
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(dict(complete=report['complete'], files=len(report['files']), failures=report['failures'])))
    if not report['complete']:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
