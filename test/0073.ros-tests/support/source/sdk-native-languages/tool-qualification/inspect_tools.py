#!/usr/bin/env python3
"""Admitted finite tool inventory; version output is not compiler/Guest proof."""
from pathlib import Path
import json
import os
import subprocess

NAMES={'rustc','cargo','zig','go','tinygo','swift','swiftc','ghc','wasm32-wasi-ghc',
       'nim','ldc2','dmd','odin','clang','clang-21','wasm-ld','wasm-validate','uwvm2-ros'}
ROOTS=('/host/home/macromodel/.local/share/uwvm-ros-extension',
       '/host/home/macromodel/.rustup/toolchains','/host/root/.rustup/toolchains',
       '/host/home/macromodel/Documents/MacroModel','/host/opt','/host/usr/bin')


def main():
    rows=[];visited=0; found={}; budget=200000
    for spelling in ROOTS:
        root=Path(spelling)
        if not root.is_dir(): continue
        for directory, dirs, files in os.walk(root,followlinks=False):
            visited+=1
            depth=len(Path(directory).relative_to(root).parts)
            dirs[:]=sorted(d for d in dirs if d not in {'.git','node_modules','records','vendor','upstream','target'}) if depth<8 and visited<budget else []
            for name in sorted(files):
                if name in NAMES:
                    p=Path(directory)/name
                    if p.is_file() and os.access(p,os.X_OK): found[str(p)]=name
            if visited>=budget: break
    env=os.environ.copy()
    env['LD_LIBRARY_PATH']='/host/usr/lib/x86_64-linux-gnu:/host/lib/x86_64-linux-gnu'
    for spelling,name in sorted(found.items()):
        args=[spelling, 'version' if name in ('go','odin','tinygo') else '--version']
        row={'path':spelling,'name':name,'argv':args}
        try:
            result=subprocess.run(args,env=env,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=10)
            row.update(exit=result.returncode,stdout=result.stdout[:4096].decode(errors='replace'),stderr=result.stderr[:2048].decode(errors='replace'))
        except Exception as error: row.update(error=str(error))
        rows.append(row)
    report={'status':'INVENTORY_ONLY','visited_directories':visited,'bounded_search':budget,
            'default_authority':[],'tools':rows,'compiler_test_runs':0,'Guest_runs':0}
    Path('tool-inventory03.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))


if __name__=='__main__':main()
