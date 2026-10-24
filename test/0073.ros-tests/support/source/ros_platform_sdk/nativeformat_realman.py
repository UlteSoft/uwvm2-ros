#!/usr/bin/env python3
"""Create one explicitly recorded CMake format-only downstream source overlay.

The immutable vendor capture remains untouched. The original Humble suite
failed on two trailing spaces at CMakeLists.txt:84. This tool removes only
those two bytes in a fresh private copy. No declaration, test registration,
test command, filter, linter configuration or native SDK is changed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    args=p.parse_args();root=args.root.resolve(strict=True);out=args.out
    inputs=json.loads((root/'inputs.json').read_bytes())
    original=Path(inputs['source_root'])/'sdk/origin/robots/arms/realman/upstream/ros2_humble/files/rm_ros_interfaces'
    prior=json.loads((root/'out/native-humble-r1/native.json').read_bytes())
    assert prior['status']=='ORIGINAL_NATIVE_INTERFACE_BUILD_PASS_UPSTREAM_TEST_FAILURES_RETAINED'
    assert prior['original_cases_run']==2 and prior['original_case_failures']==1
    assert out.parent==root/'out'and not out.exists()
    assert not any(p.is_symlink()for p in original.rglob('*'))
    before=(original/'CMakeLists.txt').read_bytes();lines=before.splitlines(keepends=True)
    assert lines[83]==b'  msg/Mastername.msg  \n'
    lines[83]=b'  msg/Mastername.msg\n';after=b''.join(lines)
    assert len(before)-len(after)==2
    out.mkdir(mode=0o700);target=out/'rm_ros_interfaces';shutil.copytree(original,target)
    (target/'CMakeLists.txt').write_bytes(after)
    rows=[]
    for path in sorted(original.rglob('*')):
        if not path.is_file():continue
        relative=path.relative_to(original);old=path.read_bytes();new=(target/relative).read_bytes()
        assert new==(after if str(relative)=='CMakeLists.txt'else old)
        rows.append(dict(path=str(relative),before_bytes=len(old),after_bytes=len(new),
            before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest(),
            changed=old!=new))
    result=dict(status='EXPLICIT_CMAKE_WHITESPACE_ONLY_OVERLAY_CREATED',original=str(original),
        overlay=str(target),changed_line=84,removed_ascii_space_bytes=2,files=rows,
        original_failure_report=str(root/'out/native-humble-r1/native.json'),default_authority=[],
        tests_or_declarations_modified=False)
    (out/'overlay.json').write_text(json.dumps(result,indent=2)+'\n')
    (out/'cmake-whitespace-only.patch').write_text('--- a/CMakeLists.txt\n+++ b/CMakeLists.txt\n@@ -84 +84 @@\n-  msg/Mastername.msg  \n+  msg/Mastername.msg\n')
    print(result['status'],flush=True)


if __name__=='__main__':main()
