#!/usr/bin/env python3
"""Build a private native x86_64 host + threaded-JIT TSan snapshot, never production."""
from pathlib import Path
import argparse, difflib, hashlib, json, os, resource, shutil, subprocess, time

P=argparse.ArgumentParser()
P.add_argument('--baseline-vm', type=Path, required=True, help='Existing qualified native VM directory with receipt and three argv files')
P.add_argument('--output', type=Path, required=True, help='New artifact directory below repository .tmp')
P.add_argument('--patch', type=Path, action='append', default=[], help='Explicit additional production fixes applied before the test-only patch; recorded in receipt')
P.add_argument('--prepare-only', action='store_true')
a=P.parse_args()
R=Path(__file__).resolve().parents[3]; D=Path(__file__).resolve().parent
out=a.output.resolve(); baseline=a.baseline_vm.resolve()
assert out.is_relative_to(R/'.tmp') and not out.exists(), 'Use a new artifact directory under .tmp'
assert 0 not in os.sched_getaffinity(0), 'Run through the resource cgroup and taskset excluding CPU0'
cgroup=Path('/sys/fs/cgroup')/Path('/proc/self/cgroup').read_text().split('::',1)[1].strip().lstrip('/')
assert (cgroup/'memory.max').read_text().strip()=='68719476736' and (cgroup/'memory.swap.max').read_text().strip()=='0', 'Use the existing 64 GiB / zero-swap cgroup'
assert os.uname().machine=='x86_64', 'This private JIT binding ABI is qualified only on native x86_64'
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
previous=json.loads((baseline/'receipt.json').read_text()); origin=Path(previous['source_root'])
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
S=out/'source'; V=out/'vm'; V.mkdir(parents=True); (out/'ir').mkdir()
shutil.copytree(origin/'src',S/'src'); (S/'third-parties').symlink_to((R/'third-parties').resolve(),target_is_directory=True)
patches=[]
for patch in [*a.patch,D/'threaded_jit_instrumentation.patch']:
 patch=patch.resolve()
 subprocess.run(['git','apply','--check',str(patch)],cwd=S,check=True)
 subprocess.run(['git','apply',str(patch)],cwd=S,check=True)
 patches.append(dict(path=str(patch),sha256=sha(patch)))
shutil.copy2(D/'uwvm_runtime_tsan_test.h',S/'src/uwvm2/runtime/lib/uwvm_runtime_tsan_test.h')
fingerprint=lambda:{str(p.relative_to(S)):sha(p) for p in (S/'src').rglob('*') if p.is_file()}
source=fingerprint(); started=time.time()
receipt=dict(source_root=str(S),source=source,baseline_receipt=str(baseline/'receipt.json'),baseline_receipt_sha256=sha(baseline/'receipt.json'),patches=patches,
 architecture='native-x86_64',cpu_affinity=sorted(os.sched_getaffinity(0)),project_objects={},status='prepared',started=started,
 instrumentation='All three project translation units plus threaded JIT Function TSan pass; ordinary non-threaded JIT is not instrumented.',
 providers='Existing LLVM, libc++, libc++abi, libunwind, libc and OpenSSL providers are not fully instrumented. No suppression or ignore_noninstrumented_modules option is enabled.')
def save(): (V/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n')
save()
if a.prepare_only: print(S);raise SystemExit(0)
for unit in ['uwvm_runtime.default.cpp','main.default.cpp','host_api.default.cpp']:
 cmd=json.loads((baseline/(unit+'.argv.json')).read_text())
 assert not any(x.startswith('-fsanitize=') for x in cmd), 'Reference build must be unsanitized, not recursively instrumented'
 cmd=[x.replace(str(origin),str(S)) for x in cmd];cmd[cmd.index('-o')+1]=str(V/(unit+'.o'))
 cmd[1:1]=['-I'+str(S/'src')]
 cmd+=['-O1','-fsanitize=thread','-fno-omit-frame-pointer','--rtlib=compiler-rt','-DUWVM_TEST_TSAN_JIT=1','-MD','-MF',str(V/(unit+'.d'))]
 (V/(unit+'.argv.json')).write_text(json.dumps(cmd,indent=2)+'\n');began=time.time()
 with (V/(unit+'.log')).open('w') as log:
  proc=subprocess.Popen(cmd,cwd=S,stdout=log,stderr=subprocess.STDOUT)
  receipt.update(status='building',active=dict(unit=unit,pid=proc.pid,started=began));save();print('START',unit,proc.pid,flush=True);code=proc.wait()
 receipt.pop('active',None)
 receipt['project_objects'][unit]=dict(object=str(V/(unit+'.o')),instrumented=True,exit=code,seconds=time.time()-began)
 if code: receipt['status']='compile_failed';save();raise SystemExit(code)
 receipt['project_objects'][unit]['sha256']=sha(V/(unit+'.o'));save()
link=json.loads((baseline/'link.argv.json').read_text())
for unit,entry in previous['objects'].items(): link=[str(V/(unit+'.o')) if value==entry['object'] else value for value in link]
link[link.index('-o')+1]=str(V/'uwvm2-ros');link[1:1]=['-fsanitize=thread','--rtlib=compiler-rt','-Wl,--threads=4','-Wl,--export-dynamic']
(V/'link.argv.json').write_text(json.dumps(link,indent=2)+'\n')
with (V/'link.log').open('w') as log: result=subprocess.run(link,cwd=S,stdout=log,stderr=subprocess.STDOUT)
if result.returncode: receipt['status']='link_failed';save();raise SystemExit(result.returncode)
assert source==fingerprint(), 'Snapshot changed during qualification build'
compiler=Path(json.loads((V/'uwvm_runtime.default.cpp.argv.json').read_text())[0]); symbolizer=compiler.parent/'llvm-symbolizer'
wrapper=(D/'run_vm.py').read_text().replace('@SYMBOLIZER@',str(symbolizer))
(V/'run-vm').write_text(wrapper);(V/'run-vm').chmod(0o755)
receipt.update(status='built',binary=str(V/'uwvm2-ros'),binary_sha256=sha(V/'uwvm2-ros'),end=time.time(),maximum_child_rss_kib=resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss);save()
print('BUILT',V/'run-vm',flush=True)
