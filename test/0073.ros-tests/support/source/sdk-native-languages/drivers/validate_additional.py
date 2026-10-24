#!/usr/bin/env python3
"""Actual additional compiler/Guest checks; no Host substitution or ROS credit.

Run within the shared finite owner only. The input tree and generated complete
record fixture come from the same admitted source archive as the base runner.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--origin',type=Path,required=True)
    ap.add_argument('--generated',type=Path,required=True)
    ap.add_argument('--tools',type=Path,required=True)
    ap.add_argument('--go',type=Path,required=True)
    ap.add_argument('--wasi-sdk',type=Path,required=True)
    ap.add_argument('--runtime',type=Path,required=True)
    ap.add_argument('--ldc-tools',type=Path)
    ap.add_argument('--language',action='append',choices=('nim','tinygo','odin','d'),
                    help='Explicit subset for bounded serialized compiler batches')
    args=ap.parse_args();root=Path.cwd();logs=root/'additional-logs';logs.mkdir();rows=[]
    selected=set(args.language or ('nim','tinygo','odin','d'))
    origin=args.origin;generated=args.generated;env=os.environ.copy()
    env.update(GOROOT=str(args.go),GOTOOLCHAIN='local',GOMAXPROCS='1',
        GOCACHE=str(root/'tinygo-go-cache'),GOMODCACHE=str(root/'tinygo-go-mod-cache'),
        PATH=str(args.go/'bin')+':/usr/local/bin:/usr/bin:/bin')
    def save():
        (root/'additional-validation03.json').write_text(json.dumps({'rows':rows,'default_authority':[],
            'native_ROS_runs':0,'Gateway_integration_runs':0,'upstream_runs':0,'QEMU_runs':0,'VTune_runs':0},indent=2)+'\n')
    def run(name,language,layer,command,cwd=None,marker=None,timeout=240,environment=None):
        log=logs/(name+'.log');row={'name':name,'language':language,'layer':layer,'argv':[str(x) for x in command],'log':str(log.relative_to(root))}
        try:
            with log.open('wb') as stream:r=subprocess.run(row['argv'],cwd=cwd or root,env=environment or env,stdout=stream,stderr=subprocess.STDOUT,timeout=timeout)
            row.update(exit=r.returncode,status='PASS' if r.returncode==0 else 'FAIL')
            if marker and row['status']=='PASS' and marker not in log.read_text(errors='replace'):row.update(status='FAIL',reason='Guest application marker missing')
        except Exception as error:row.update(status='FAIL',error=str(error))
        rows.append(row);save();print(name,row['status'],flush=True);return row['status']=='PASS'
    def skip(language,reason):rows.append({'language':language,'status':'NOT_RUN','reason':reason});save()
    clang=args.wasi_sdk/'bin/clang';ar=args.wasi_sdk/'bin/ar';validator=Path('/usr/bin/wasm-validate')
    runtime_hash=hashlib.sha256(args.runtime.read_bytes()).hexdigest()
    if runtime_hash!='0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556':raise ValueError('current Task10 Runtime pin changed')
    nim_candidates=sorted((args.tools/'nim').glob('*/bin/nim'))
    if 'nim' not in selected:skip('nim','not selected in this bounded batch')
    elif nim_candidates:
        nim=nim_candidates[0];run('nim-version','nim','inventory',[nim,'--version'])
        common=[nim,'check','--mm:arc','--threads:off','--exceptions:goto','--cpu:wasm32','--os:any','-d:wasm','-d:useMalloc',
                '--path:'+str(origin/'nim/src'),'--path:'+str(generated/'nim')]
        run('nim-sdk-check','nim','semantic_check',[*common,origin/'nim/src/roswasm/typed.nim'])
        run('nim-fields-check','nim','semantic_check',[*common,generated/'nim/records.nim'])
        run('nim-facades-check','nim','semantic_check',[*common,generated/'nim/interfaces.nim'])
        probe=root/'nim_probe.nim'
        probe.write_text('''import roswasm
import roswasm/[wire,typed]
import records
{.compile: "'''+str(origin/'nim/bridge/shim.c')+'''".}
{.compile: "'''+str(origin/'nim/bridge/main.c')+'''".}
if raw_abi_version()!=1: quit(95)
var value=initRos2Language_fixtureMsgAllValues()
value.title="robot"
let bytes=encode(value)
let decoded=decodeRos2Language_fixtureMsgAllValues(bytes)
if decoded.u64_value!=high(uint64) or decoded.i64_value!=low(int64): quit(22)
try:
  let o=roswasm.open("lesson.ungranted")
  close(o)
  quit(17)
except RosError as error:
  if error.code!=13: quit(22)
echo "PASS_TASK03_NIM_DENIED"
''')
        for backend in (1,2):
            out=root/f'nim-ros{backend}.wasm'
            command=[nim,'c','--mm:arc','--threads:off','--exceptions:goto','--cpu:wasm32','--os:any','-d:wasm','-d:useMalloc',
                '--stackTrace:off','--lineTrace:off','-d:noSignalHandler','--parallelBuild:1','--cc:clang','--clang.exe:'+str(clang),'--clang.linkerexe:'+str(clang),
                '--passC:--target=wasm32-wasip1','--passL:--target=wasm32-wasip1',f'--passC:-DROSWASM_C_FFI_BACKEND={backend}','--passC:-Dmain=roswasm_c_ffi_nim_main',
                '--path:'+str(origin/'nim/src'),'--path:'+str(generated/'nim'),'--nimcache:'+str(root/f'nim-cache-{backend}'),'--out:'+str(out),probe]
            if run(f'nim-ros{backend}-compile','nim','compiler',command):
                run(f'nim-ros{backend}-validate','nim','wasm_validation',[validator,out])
                run(f'nim-ros{backend}-Guest','nim','actual_guest_default_denial',[args.runtime,'--run',out],marker='PASS_TASK03_NIM_DENIED',timeout=30)
    else:skip('nim','pinned compiler unavailable')

    tiny=args.tools/'tinygo/tinygo/bin/tinygo'
    if 'tinygo' not in selected:skip('tinygo','not selected in this bounded batch')
    elif tiny.is_file():
        run('tinygo-version','tinygo','inventory',[tiny,'version'])
        project=root/'tinygo-probe';project.mkdir()
        (project/'go.mod').write_text('module uwvm-native.local/tinyprobe\n\ngo 1.23\nrequire uwvm2.local/roswasm v0.0.0\nreplace uwvm2.local/roswasm => '+str(origin/'tinygo')+'\n')
        (project/'main.go').write_text('''package main
import ros "uwvm2.local/roswasm"
func main(){if ros.RawAbiVersion()!=1{panic("ABI")};o,e:=ros.Open("lesson.ungranted");if e==nil{o.Close();panic("grant")};v,ok:=e.(ros.Error);if !ok||v.Code!=13{panic("status")};println("PASS_TASK03_TINYGO_DENIED")}
''')
        for backend in ('ros1','ros2'):
            out=root/('tinygo-'+backend+'.wasm')
            if run('tinygo-'+backend+'-compile','tinygo','compiler',[tiny,'build','-p=1','-target=wasi','-scheduler=none','-panic=trap','-tags='+backend,'-o',out,'.'],cwd=project,timeout=480):
                run('tinygo-'+backend+'-validate','tinygo','wasm_validation',[validator,out])
                run('tinygo-'+backend+'-Guest','tinygo','actual_guest_default_denial',[args.runtime,'--run',out],marker='PASS_TASK03_TINYGO_DENIED',timeout=30)
    else:skip('tinygo','pinned compiler unavailable')

    odin_candidates=sorted((args.tools/'odin').glob('*/odin'))
    if 'odin' not in selected:skip('odin','not selected in this bounded batch')
    elif odin_candidates:
        odin=odin_candidates[0];run('odin-version','odin','inventory',[odin,'version'])
        odin_env=env.copy();odin_env['PATH']=str(args.wasi_sdk/'bin')+':'+env['PATH']
        for name in ('records.odin','interfaces.odin'):shutil.copy2(generated/'odin'/name,origin/'odin/roswasm'/name)
        library=root/'odin-bridge';library.mkdir();obj=library/'shim.o';archive=library/'libroswasm_c_ffi.a'
        run('odin-bridge-object','odin','compiler_bridge',[clang,'--target=wasm32-wasip1','-O2','-DROSWASM_C_FFI_BACKEND=2','-c',origin/'odin/bridge/shim.c','-o',obj])
        if run('odin-bridge-archive','odin','compiler_bridge',[ar,'rcs',archive,obj]):
            project=root/'odin-probe';project.mkdir()
            (project/'main.odin').write_text('''package main
import "core:fmt"
import ros "roswasm:roswasm"
main :: proc(){
 if ros.raw_abi_version()!=1 { panic("ABI") }
 value:=ros.init_Ros2Language_fixtureMsgAllValues()
 buffer:[32768]u8
 bytes,code:=ros.encode_Ros2Language_fixtureMsgAllValues(&value,buffer[:])
 if code!=0 || len(bytes)==0 { panic("encode") }
 state:ros.State
 owner,status:=ros.open(&state,transmute([]u8)string("lesson.ungranted"))
 if status!=13 { if status==0 { ros.close(owner) };panic("grant") }
 fmt.println("PASS_TASK03_ODIN_DENIED")
}
''')
            base=[odin,'check',project,'-target:wasi_wasm32','-thread-count:1','-collection:roswasm='+str(origin/'odin')]
            run('odin-fields-sdk-check','odin','semantic_check',base)
            out=root/'odin-ros2.wasm'
            if run('odin-ros2-compile','odin','compiler',[odin,'build',project,'-target:wasi_wasm32','-thread-count:1','-o:size',
                '-collection:roswasm='+str(origin/'odin'),'-extra-linker-flags:libroswasm_c_ffi.a','-out:'+str(out)],cwd=library,environment=odin_env):
                run('odin-ros2-validate','odin','wasm_validation',[validator,out])
                run('odin-ros2-Guest','odin','actual_guest_default_denial',[args.runtime,'--run',out],marker='PASS_TASK03_ODIN_DENIED',timeout=30)
    else:skip('odin','pinned compiler unavailable')

    ldc_candidates=sorted(args.ldc_tools.glob('d/*/bin/ldc2')) if args.ldc_tools else []
    if 'd' not in selected:skip('d','not selected in this bounded batch')
    elif ldc_candidates:
        ldc=ldc_candidates[0];run('ldc-version','d','inventory',[ldc,'--version'])
        (origin/'d/source/roswasm/records.d').write_bytes((generated/'d/records.d').read_bytes())
        (origin/'d/source/roswasm/interfaces.d').write_bytes((generated/'d/interfaces.d').read_bytes())
        for name in ('package','wire','typed','records','interfaces'):
            run('d-'+name+'-object','d','compiler',[ldc,'-betterC','-mtriple=wasm32-unknown-wasi','-c','-I='+str(origin/'d/source'),
                origin/('d/source/roswasm/'+name+'.d'),'-of='+str(root/('d-'+name+'.o'))])
    else:skip('d','exact upstream-checksum-verified LDC unavailable in selected tool run')
    save();return 1 if any(r['status']=='FAIL' for r in rows) else 0


if __name__=='__main__':sys.exit(main())
