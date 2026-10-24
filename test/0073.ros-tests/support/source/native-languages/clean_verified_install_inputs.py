#!/usr/bin/env python3
"""Remove only archived Task03 redundant Rust installer extraction inputs.

Run through the shared owner. Keep the real installed Rust/Go/Zig tools, all
receipts/downloads and both immutable evidence archives. No other task is touched.
"""
from pathlib import Path
import hashlib
import json
import os
import shutil


def sha(path):
    with path.open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()


root=Path('/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-toolchains-r1')
receipt=json.loads((root/'resource-archives/verified-copies.json').read_bytes())
if receipt.get('copies')!=2 or len(receipt.get('verified_paths',[]))!=2:raise ValueError('two exact verified original archives required')
for value in receipt['verified_paths']:
    # The original acquisition owner ran in the container namespace; the
    # current identical owner runs in chroot /host with native path spellings.
    path=Path(value[5:] if value.startswith('/host/') else value)
    expected={root/'resource-archives/evidence.tar',
        Path('/home/macromodel/Documents/uwvm-test-261018/agents/director-20261021/infrastructure/archives/03-20261021-toolchains-r1.tar')}
    if path not in expected:raise ValueError('unexpected original archive location')
    if path.stat().st_size!=receipt['bytes'] or sha(path)!=receipt['sha256']:raise ValueError('original archive read-back verification failed')
tools=root/'work/toolchains'
names=('cargo-x86_64-unknown-linux-gnu-input','rustc-x86_64-unknown-linux-gnu-input',
       'rust-std-x86_64-unknown-linux-gnu-input','rust-std-wasm32-wasip1-input','rust-std-wasm32-unknown-unknown-input')
selected=[tools/name for name in names]
for path in selected:
    if path.is_symlink() or not path.is_dir() or path.stat().st_uid!=os.getuid():raise ValueError('ordinary task-owned installer extraction required')
    if path.resolve(strict=True).parent!=tools.resolve(strict=True):raise ValueError('installer extraction path escaped')
for path in (tools/'rust').rglob('*'):
    if path.is_symlink() and any(path.resolve(strict=True).is_relative_to(target) for target in selected):
        raise ValueError('installed tool still refers to a selected extraction')
pinned={str(path):sha(path) for path in (tools/'rust/bin/rustc',tools/'rust/bin/cargo')}
removed=[]
for path in selected:shutil.rmtree(path);removed.append(str(path))
if any(sha(Path(path))!=value for path,value in pinned.items()):raise ValueError('retained installed tool changed')
(Path.cwd()/'verified-cleanup.json').write_text(json.dumps({'removed_only_redundant_extractions':removed,
    'archive_sha256':receipt['sha256'],'both_original_archives_read_back':True,'retained_program_sha256':pinned,
    'compiler_runs':0,'Guest_runs':0,'default_authority':[]},indent=2)+'\n')
