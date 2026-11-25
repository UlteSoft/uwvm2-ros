#!/usr/bin/env python3
# v7 SOURCE successor; this is DATA ABI separation, not authenticated native cache replay.
"""Check the mmap backend identity in the standalone cache DATA codec."""
import argparse,hashlib,json,subprocess
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--mmap',type=Path,required=True)
p.add_argument('--out',type=Path,required=True);a=p.parse_args()
subprocess.run(['bash',str(Path(__file__).resolve().parents[2]/'tools/ci/require_wasm3_test_cgroup.sh')],check=True)
a.out.mkdir(parents=True,exist_ok=False);rows=[]
def run(*args):
 command=[str(a.mmap),*map(str,args)];r=subprocess.run(command,capture_output=True,text=True,timeout=30)
 rows.append(dict(command=command,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr))
 (a.out/'results.json').write_text(json.dumps(rows,indent=2)+'\n')
 assert r.returncode==0,rows[-1]
 assert 'codec_data_only=1' in r.stdout and 'native_cache_replay=0' in r.stdout
 if args:assert 'standalone_persistent_refused=1' in r.stdout
 return dict(line.split('=',1) for line in r.stdout.splitlines() if '=' in line)
model=run()
abi=bytes.fromhex(model['abi-hex'])
assert b'native-memory-backend' in abi and b'mmap' in abi
for mode in ['native-signed','native-unsigned']:
 record=run('write',a.out/mode,mode)
 assert not Path(record['path']).exists() and record['blob-hex']
(a.out/'summary.json').write_text(json.dumps(dict(passed=True,codec_data_only=True,native_cache_replay=False,invocations=len(rows),
 product=model['product'],binary_sha256=hashlib.file_digest(a.mmap.open('rb'),'sha256').hexdigest()),indent=2)+'\n')
print('PASS mmap identity and signed/unsigned cache DATA encoding')
