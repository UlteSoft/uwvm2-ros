#!/usr/bin/env python3
"""Reject direct default allocator spellings in VM-owned scene implementation.

This lexical check complements actual malloc-family failure injection. It does
not prove allocation provenance through templates, external ROS/LLVM ABIs or
linked libraries. Owning scene containers must use the project's recoverable
fast_io adapters; views, arrays and shared_ptr references are not allocations.
Run within the existing 64 GiB zero-swap cgroup. This never executes a provider.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')

def code_only(text):
    """Blank comments/literals while preserving source positions and line numbers."""
    pattern=re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|(?:u8|u|U|L)?R"([^ ()\\\t\r\n]{0,16})\([\s\S]*?\)\1"|(?:u8|u|U|L)?"(?:\\[\s\S]|[^"\\])*"|(?:u8|u|U|L)?\'(?:\\[\s\S]|[^\'\\])*\'')
    return pattern.sub(lambda match: ''.join('\n' if c=='\n' else ' ' for c in match.group()),text)

RULES={
    'default standard owning container': re.compile(r'\bstd\s*::\s*(?:vector|deque|list|forward_list|map|multimap|set|multiset|unordered_map|unordered_multimap|unordered_set|unordered_multiset|basic_string|basic_stringbuf|basic_ostringstream|basic_istringstream|string|wstring|u8string|u16string|u32string|function|packaged_task|pmr)\b'),
    'default standard allocation factory': re.compile(r'\bstd\s*::\s*(?:make_shared|make_unique|make_unique_for_overwrite|make_shared_for_overwrite|allocator)\b'),
    'direct allocation function': re.compile(r'(?<![\w])(?:malloc|calloc|realloc|aligned_alloc|posix_memalign)\s*\('),
    # Ordinary placement construction is required by the fast_io object
    # adapter and does not itself allocate. Parenthesized type-id spellings
    # and custom allocation macros require the accompanying manual review.
    'direct C++ allocation expression': re.compile(r'\bnew\b\s*(?:[A-Za-z_:]|\(\s*(?:::)?std\s*::\s*nothrow\b)'),
}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root',type=Path,default=ROOT)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    source=args.source_root.resolve();output=args.output.resolve()
    assert output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve()) and not output.exists()
    paths=sorted([*source.glob('src/uwvm2/imported/ros/scene/**/*.h'),*source.glob('src/uwvm2/uwvm/imported/scene/*.h'),source/'src/uwvm2/uwvm/cmdline/params/scene.h'])
    assert paths and all(p.is_file() for p in paths)
    findings=[];inputs={}
    for path in paths:
        original=path.read_bytes();inputs[str(path)]=hashlib.sha256(original).hexdigest()
        text=original.decode();code=code_only(text)
        for rule,regex in RULES.items():
            for match in regex.finditer(code):
                line=code.count('\n',0,match.start())+1
                findings.append(dict(path=str(path.relative_to(source)),line=line,rule=rule,source=text.splitlines()[line-1].strip()))
    result=dict(complete=not findings,scope=__doc__,source_root=str(source),inputs=inputs,files=len(paths),findings=findings,
        cgroup=str(GROUP),cgroup_inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,
        runner_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
    output.parent.mkdir(parents=True,exist_ok=True);output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(complete=not findings,files=len(paths),findings=findings),indent=2))
    if findings:raise SystemExit(1)

if __name__=='__main__':main()
