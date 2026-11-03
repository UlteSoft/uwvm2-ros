#!/usr/bin/env python3
"""Freeze literal include closure and changed audit inputs. Never compiles/runs tests."""
import argparse, hashlib, json, pathlib, re, shutil
p=argparse.ArgumentParser();p.add_argument('--source',type=pathlib.Path,required=True);p.add_argument('--out',type=pathlib.Path,required=True);a=p.parse_args()
s=a.source.resolve();out=a.out.resolve();out.mkdir(parents=True,exist_ok=False)
search=[s/'src',s/'third-parties/fast_io/include',s/'third-parties/bizwen/include',s/'third-parties/boost_unordered/include',s]
queue=list((s/'test/0082.audit-qualification').glob('*'))
queue += [s/v for v in ('src/uwvm2/imported/ros/common/audit.h','src/uwvm2/imported/ros/common/audit_io.h','src/uwvm2/imported/ros/common/debug.h','src/uwvm2/imported/ros/capability/manager.h','src/uwvm2/uwvm/debugger/wasip1_calls.cppm','src/uwvm2/uwvm/debugger/wasip1_state.md','src/uwvm2/uwvm/debugger/ros_state.md','src/uwvm2/uwvm/imported/wasi/wasip1/init/init_env.h','src/uwvm2/runtime/lib/uwvm_runtime_cache_origin.h')]
seen=set();include=re.compile(r'^\s*#\s*include\s*[<"]([^>"\n]+)[>"]',re.M)
while queue:
    f=queue.pop().resolve()
    if f in seen or not f.is_file() or not f.is_relative_to(s):continue
    seen.add(f)
    try:text=f.read_text(encoding='utf-8-sig')
    except UnicodeDecodeError:continue
    for name in include.findall(text):
        for base in [f.parent,*search]:
            candidate=(base/name).resolve()
            if candidate.is_file() and candidate.is_relative_to(s):queue.append(candidate);break
# Preserve FastIO's complete platform-selective include tree; a Linux compiler
# can select a different literal path than the macOS source packager.
seen.update(f.resolve() for f in (s/'third-parties/fast_io/include').rglob('*') if f.is_file())
manifest={}
for f in sorted(seen):
    rel=f.relative_to(s);target=out/rel;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(f,target)
    b=target.read_bytes();manifest[str(rel)]={'sha256':hashlib.sha256(b).hexdigest(),'bytes':len(b)}
(out/'source-manifest.json').write_text(json.dumps({'format':1,'scope':'audit components and actual builtin WASIp1 wrapper IO; no native ROS/full Runtime acceptance claim','files':manifest},indent=2)+'\n')
print(json.dumps({'status':'SOURCE_PACKET_ONLY','files':len(manifest),'bytes':sum(v['bytes'] for v in manifest.values()),'path':str(out)}))
