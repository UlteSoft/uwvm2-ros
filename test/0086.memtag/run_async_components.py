#!/usr/bin/env python3
"""Native TSan and optional QEMU functional tests. No emulated timing claims."""
from pathlib import Path
import argparse, hashlib, itertools, json, os, resource, subprocess, time

ROOT=Path(__file__).resolve().parents[2]
HERE=Path(__file__).resolve().parent
TARGETS={
 'i686':('i686-linux-gnu',['-march=i686','-mno-sse','-mno-sse2'],'qemu-i386-static','pentium3'),
 'aarch64':('aarch64-linux-gnu',['-march=armv8-a'],'qemu-aarch64-static','cortex-a53'),
 'arm':('arm-linux-gnueabi',['-march=armv7-a','-mfloat-abi=soft','-mno-unaligned-access'],'qemu-arm-static','cortex-a15'),
 'armhf':('arm-linux-gnueabihf',['-march=armv7-a','-mfpu=vfpv3-d16','-mfloat-abi=hard','-mno-unaligned-access'],'qemu-arm-static','cortex-a15'),
}
def main():
 p=argparse.ArgumentParser();p.add_argument('--cxx',required=True);p.add_argument('--out',required=True,type=Path)
 p.add_argument('--cross-root',type=Path);a=p.parse_args();out=a.out.resolve();out.mkdir(parents=True,exist_ok=False);(out/'tmp').mkdir()
 compiler=Path(a.cxx).absolute();resource.setrlimit(resource.RLIMIT_CORE,(0,0))
 env={**os.environ,'TMPDIR':str(out/'tmp'),'TSAN_OPTIONS':'halt_on_error=1:exitcode=66:second_deadlock_stack=1'}
 sources=[HERE/'deferred_delivery.cc',HERE/'async_access.cc',HERE/'tsan/detector.c',
  *[ROOT/'src/uwvm2'/name for name in ('runtime/memtag/deferred.h','runtime/memtag/bridge.h',
   'runtime/diagnostic/crash_policy.h','extension/memtag/execution_policy.h',
   'extension/memtag/hardware_mte.h','extension/memtag/shadow.h','extension/memtag/storage.h',
   'object/memory/linear/mmap.h','object/memory/signal/signal.h')]]
 def source_hashes():return {str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources}
 report={'scope':'native TSan; cross profiles are QEMU user-mode functional tests','rows':[],
  'source_sha256_before':source_hashes()}
 def run(name,args,expected=0,required=None):
  began=time.monotonic();r=subprocess.run(list(map(str,args)),env=env,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=180)
  text=r.stdout.decode(errors='replace');(out/(name+'.log')).write_text(text)
  passed=r.returncode==expected and (required is None or required in text)
  if expected!=66 and 'WARNING: ThreadSanitizer:' in text:passed=False
  report['rows'].append({'name':name,'args':list(map(str,args)),'exit':r.returncode,'expected':expected,'passed':passed,'seconds':time.monotonic()-began})
  (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
  if not passed:raise RuntimeError(name+' failed: '+text[-4000:])
 includes=['-I'+str(ROOT/'src'),*['-I'+str(ROOT/'third-parties'/x/'include') for x in ('fast_io','bizwen','boost_unordered')]]
 for profile in ['native',*(TARGETS if a.cross_root else [])]:
  flags=[];runner=[]
  if profile=='native':flags=['-fsanitize=thread','-g']
  else:
   cross=a.cross_root.resolve();triple,arch,qemu,cpu=TARGETS[profile]
   flags=['--target='+triple,'--sysroot='+str(cross),'--gcc-install-dir='+str(cross/'usr/lib/gcc-cross'/triple/'13'),
    '-idirafter',str(cross/'usr'/triple/'include'),'-L'+str(cross/'usr'/triple/'lib'),'--ld-path='+str(compiler.with_name('ld.lld')),*arch]
   runner=[cross/'usr/bin'/qemu,'-cpu',cpu,'-L',cross/'usr'/triple]
  for unit in ('deferred_delivery','async_access'):
   binary=out/(profile+'-'+unit)
   run(profile+'-compile-'+unit,[compiler,*flags,'-std=c++26','-O1','-pthread',*includes,HERE/(unit+'.cc'),'-latomic','-ldl','-o',binary])
   if unit=='deferred_delivery':
    run(profile+'-queue',[*runner,binary],required='262144 copied fault records')
    run(profile+'-queue-terminal',[*runner,binary,'fault'],122,'Exit status: 122')
    run(profile+'-first-fault-status',[*runner,binary,'prior-fault'],120,'Exit status: 120')
   else:
    for mode,owner,bits,shared in itertools.product(('sync','async','valid','bounds','host'),('mmap','moving'),('32','64'),('private','shared')):
     run('-'.join((profile,mode,owner,bits,shared)),[*runner,binary,mode,owner,bits,shared],122 if mode=='async' else 0,'PASS:')
 detector=out/'tsan-detector'
 run('detector-compile',[compiler.with_name('clang'),'-std=c11','-D_GNU_SOURCE','-O1','-g','-pthread','-fsanitize=thread',HERE/'tsan/detector.c','-o',detector])
 run('detector-safe',[detector],0,'count=20000');run('detector-race',[detector,'race'],66,'WARNING: ThreadSanitizer: data race')
 report['passed']=all(x['passed'] for x in report['rows'])
 report['source_sha256']=source_hashes()
 report['sources_unchanged']=report['source_sha256']==report['source_sha256_before']
 report['passed']=report['passed'] and report['sources_unchanged']
 (out/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'passed':report['passed'],'rows':len(report['rows'])}))
 if not report['passed']:raise SystemExit('sources changed during qualification')
if __name__=='__main__':main()
