#!/usr/bin/env python3
"""Build actual SDK guests with a freestanding WebAssembly-capable Clang."""
import argparse
from pathlib import Path
import subprocess
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--clang',default='clang');p.add_argument('--source',type=Path,default=Path(__file__).resolve().parents[2]);a=p.parse_args()
for version in (1,2):
 for language,suffix in (('c','c'),('c++','cpp')):
  source=a.source/f'examples/ros{version}/talker.{suffix}';output=source.with_name(f'talker_{suffix}.wasm')
  cmd=[a.clang,'--target=wasm32','-x',language,'-std=c11' if language=='c' else '-std=c++20','-ffreestanding','-nostdlib','-fno-exceptions','-fno-rtti','-O1','-Wl,--no-entry','-Wl,--export=_start','-Wl,--export-memory','-Wl,--allow-undefined',str(source),'-o',str(output)]
  subprocess.run(cmd,check=True,timeout=60);print('PASS compiled',output)
