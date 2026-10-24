#!/usr/bin/env python3
"""Serialized Task 03 source/compiler/wire verification, never native ROS credit.

Execute only as a workload of the shared finite resource owner. Every subprocess
inherits that owner's cgroup/limits; no daemon, container or background process
is launched. Failures and NOT_RUN cells remain separate from successful rows.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import os
import shutil
import subprocess
import sys
import tarfile


def stage_inputs(origin,explicit):
    root=explicit or origin.parents[1]/'test/0073.ros-tests'
    if not(root/'support/stage_sdk_tests.py').is_file():
        root=next((p for p in Path(__file__).resolve().parents
            if(p/'support/stage_sdk_tests.py').is_file()),None)if explicit is None else None
    if root is None:raise ValueError('separate test artifact required; supply --test-artifact-root')
    sys.path.insert(0,str(root/'support'))
    from stage_sdk_tests import native_languages_tools
    return native_languages_tools(origin,test_root=root)


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source-archive',type=Path,required=True)
    ap.add_argument('--test-artifact-root',type=Path,help='Separate unified test artifact when absent from the source archive')
    ap.add_argument('--tools',type=Path,required=True)
    ap.add_argument('--runtime',type=Path)
    ap.add_argument('--skip-rust',action='store_true',help='Retain prior exact-source Rust matrix receipts; do not count them as new passes')
    ap.add_argument('--classify-go-startup-refusal',action='store_true')
    args=ap.parse_args()
    root=Path.cwd(); source=root/'source';source.mkdir()
    with tarfile.open(args.source_archive) as archive:archive.extractall(source,filter='data')
    origin=source/'sdk/origin';helper=origin/'tools/native_languages'
    staging=stage_inputs(origin,args.test_artifact_root)
    rows=[];logs=root/'logs';logs.mkdir()
    env=os.environ.copy()
    host='' if Path('/usr/bin/clang-21').is_file() else '/host'
    env.update(LD_LIBRARY_PATH=host+'/usr/lib/x86_64-linux-gnu:'+host+'/lib/x86_64-linux-gnu',
       PYTHONDONTWRITEBYTECODE='1',CARGO_BUILD_JOBS='1',GOMAXPROCS='1',
       GOCACHE=str(root/'go-cache'),GOMODCACHE=str(root/'go-mod-cache'),GOTOOLCHAIN='local',GOWORK='off')

    def save():
        (root/'validation03.json').write_text(json.dumps({'kind':'task03-layered-validation-v1',
          'source_archive_sha256':hashlib.sha256(args.source_archive.read_bytes()).hexdigest(),
          'test_source_staging':staging,
          'rows':rows,'default_authority':[],
          'actual_uwvm_Guest_runs':sum(r['layer']=='actual_guest_default_denial' for r in rows),
          'actual_uwvm_Guest_passes':sum(r['layer']=='actual_guest_default_denial' and r['status']=='PASS' for r in rows),
          'native_ROS_runs':0,'Gateway_integration_runs':0,
          'upstream_suite_runs':0,'QEMU_runs':0,'VTune_hardware_runs':0},indent=2)+'\n')
    def run(name,layer,command,cwd=None,environment=None,expected=0,timeout=240,marker=None):
        row={'name':name,'layer':layer,'argv':[str(v) for v in command]}
        log=logs/(name+'.log');row['log']=str(log.relative_to(root))
        try:
            with log.open('wb') as output:
                result=subprocess.run(row['argv'],cwd=cwd or root,env=environment or env,
                    stdout=output,stderr=subprocess.STDOUT,timeout=timeout)
            row['exit']=result.returncode
            row['status']='PASS' if (result.returncode==expected if expected is not None else result.returncode!=0) else 'FAIL'
            if row['status']=='PASS' and marker is not None and marker not in log.read_text(errors='replace'):
                row.update(status='FAIL',reason='required Guest application marker absent')
        except Exception as error:row.update(status='FAIL',error=str(error))
        rows.append(row);save();print(name,row['status'],flush=True)
        return row['status']=='PASS'
    def skip(name,layer,reason):rows.append({'name':name,'layer':layer,'status':'NOT_RUN','reason':reason});save()
    runtime=args.runtime
    if runtime is not None:
        runtime_hash=hashlib.sha256(runtime.read_bytes()).hexdigest()
        if runtime_hash!='0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556':
            raise ValueError('Task10 runtime-denial-r2 immutable binary pin differs')
        (root/'runtime-pin.json').write_text(json.dumps({'path':str(runtime),'sha256':runtime_hash,
            'scope':'Task10 dual builtins; no vendor registry; no native ROS or size/performance credit'},indent=2)+'\n')

    numeric=root/'numeric'
    run('native-profile-correctness','configuration_unit',[sys.executable,'-I','-B',helper/'test_profile.py'])
    run('native-argument-correctness','configuration_unit',[sys.executable,'-I','-B',helper/'test_arguments.py'])
    if not run('numeric-generation','source',[sys.executable,'-I','-B',helper/'numeric.py','--schema',origin/'abi/abi-v1.json','--out',numeric]):return 1
    for language in ('swift','haskell','nim','d','odin'):
        shutil.copytree(numeric/language,origin/language,dirs_exist_ok=True)
    generated=root/'generated'
    if not run('full-field-generation','source',[sys.executable,'-I','-B',helper/'emit.py','--origin',origin,
        '--catalog',helper/'fixtures/catalog.json','--guest-only','--out',generated]):return 1

    clang=Path(host+'/usr/bin/clang-21');validator=Path(host+'/usr/bin/wasm-validate')
    checker=origin/'tools/check_wasm_imports.py'
    for backend in (1,2):
        out=root/f'numeric-ros{backend}.wasm';bridge=numeric/'nim/bridge'
        if run(f'bridge-ros{backend}','compiler',[clang,'--target=wasm32-unknown-unknown','-ffreestanding','-nostdlib',
               '-fuse-ld='+host+'/usr/lib/llvm-21/bin/wasm-ld','-O2',f'-DROSWASM_C_FFI_BACKEND={backend}',
               bridge/'shim.c','-I',bridge,'-Wl,--no-entry','-Wl,--export=roswasm_c_ffi_invoke','-o',out]):
            run(f'bridge-ros{backend}-validate','wasm_validation',[validator,out])
            run(f'bridge-ros{backend}-imports','binary_imports',[sys.executable,'-I','-B',checker,out,'--backend',str(backend),'--require-all'])
    run('bridge-no-backend-refused','negative_compiler',[clang,'--target=wasm32-unknown-unknown','-ffreestanding','-c',numeric/'nim/bridge/shim.c','-o',root/'refused.o'],expected=None)
    run('bridge-wasm64-refused','negative_compiler',[clang,'--target=wasm64-unknown-unknown','-ffreestanding','-DROSWASM_C_FFI_BACKEND=2','-c',numeric/'nim/bridge/shim.c','-o',root/'refused64.o'],expected=None)

    rust=args.tools/'rust/bin/rustc'
    if rust.is_file() and not args.skip_rust:
        run('rust-version','inventory',[rust,'--version','--verbose'])
        cargo=args.tools/'rust/bin/cargo'
        if cargo.is_file():
            cenv=env.copy();cenv.update(RUSTC=str(rust),CARGO_NET_OFFLINE='true',RUSTFLAGS='',CARGO_ENCODED_RUSTFLAGS='-C\x1fpanic=abort')
            run('rust-cargo-offline','compiler',[cargo,'check','--offline','--frozen','--jobs','1','--manifest-path',origin/'rust/Cargo.toml',
                '--target','wasm32-wasip1','--target-dir',root/'cargo-target','--no-default-features','--features','alloc,ros2'],environment=cenv)
        rustout=root/'rust';rustout.mkdir()
        for edition in ('2015','2018','2021','2024'):
            for mode in ('core','alloc'):
                for backend in ('none','ros1','ros2','both'):
                    features=[]
                    if mode=='alloc':features+=['--cfg','feature="alloc"']
                    if backend in ('ros1','both'):features+=['--cfg','feature="ros1"']
                    if backend in ('ros2','both'):features+=['--cfg','feature="ros2"']
                    output=rustout/f'libroswasm-{edition}-{mode}-{backend}.rlib'
                    run(f'rust-{edition}-{mode}-{backend}','compiler',[rust,'--crate-name','roswasm','--crate-type','rlib',
                        '--edition='+edition,'--target=wasm32-wasip1','-Cpanic=abort',*features,origin/'rust/src/lib.rs','-o',output])
        for backend in ('ros1','ros2'):
            library=rustout/f'libroswasm-{backend}.rlib'
            if not run('rust-guest-library-'+backend,'compiler',[rust,'--crate-name','roswasm','--crate-type','rlib','--edition=2021',
                  '--target=wasm32-wasip1','-Cpanic=abort','--cfg','feature="'+backend+'"',origin/'rust/src/lib.rs','-o',library]):continue
            app=root/('rust-denial-'+backend+'.rs')
            variant='Ros1' if backend=='ros1' else 'Ros2'
            app.write_text('extern crate roswasm;\nfn main(){ let b=roswasm::Backend::'+variant+'; if b.abi_version()!=1 {std::process::exit(95);} match roswasm::Capability::open(b,"lesson.ungranted") {Err(e)=>{if e.code!=13{std::process::exit(2);}},Ok(mut o)=>{let _=o.close();std::process::exit(3);}}println!("PASS_TASK03_RUST_DENIED");}\n')
            for edition in ('2015','2018','2021','2024'):
                out=root/f'rust-{edition}-{backend}.wasm'
                if run(f'rust-command-{edition}-{backend}','compiler',[rust,'--edition='+edition,'--target=wasm32-wasip1','-Cpanic=abort',
                        app,'--extern','roswasm='+str(library),'-o',out]):
                    run(f'rust-command-{edition}-{backend}-validate','wasm_validation',[validator,out])
                    if runtime is not None:
                        run(f'rust-command-{edition}-{backend}-Guest','actual_guest_default_denial',[runtime,'--run',out],timeout=20,marker='PASS_TASK03_RUST_DENIED')
        generatedlib=root/'generated-rust.rs'
        generatedlib.write_text('extern crate roswasm;\n#[path='+json.dumps(str(generated/'rust/bounded_records.rs'))+'] pub mod bounded_records;\n'+
            '#[path='+json.dumps(str(generated/'rust/bounded_interfaces.rs'))+'] pub mod bounded_interfaces;\n')
        for edition in ('2015','2018','2021','2024'):
            run(f'rust-full-field-{edition}','compiler',[rust,'--edition='+edition,'--crate-type','rlib','--crate-name','fullfield',
                '--target=wasm32-wasip1','-Cpanic=abort',generatedlib,'--extern','roswasm='+str(rustout/'libroswasm-2021-alloc-ros2.rlib'),'-o',root/f'generated-{edition}.rlib'])
    else:skip('rust-matrix','compiler','Not repeated in this run; use exact-source r3 receipts, or pinned rustc unavailable')

    go=args.tools/'go-input/go/bin/go'
    if go.is_file():
        run('go-version','inventory',[go,'version'])
        project=root/'go-app';project.mkdir()
        (project/'go.mod').write_text('module uwvm-native.local/probe\n\ngo 1.24\n\nrequire uwvm2.local/roswasmgo v0.0.0\nreplace uwvm2.local/roswasmgo => '+str(origin/'go')+'\n')
        (project/'main.go').write_text('package main\nimport (ros "uwvm2.local/roswasmgo"; "os";"fmt")\nfunc main(){if ros.AbiVersion()!=1{os.Exit(95)};o,c:=ros.Open("lesson.ungranted");if c!=13{if c==0{o.Close()};os.Exit(2)};fmt.Println("PASS_TASK03_GO_DENIED")}\n')
        genv=env.copy();genv.update(GOOS='wasip1',GOARCH='wasm',CGO_ENABLED='0')
        for backend in ('ros1','ros2'):
            out=root/('go-'+backend+'.wasm')
            if run('go-command-'+backend,'compiler',[go,'build','-p=1','-tags',backend,'-o',out,'.'],cwd=project,environment=genv):
                run('go-command-'+backend+'-validate','wasm_validation',[validator,out])
                if runtime is not None:
                    passed=run('go-command-'+backend+'-Guest','actual_guest_default_denial',[runtime,'--run',out],timeout=30,marker='PASS_TASK03_GO_DENIED')
                    if not passed and args.classify_go_startup_refusal:
                        row=rows[-1]
                        if row.get('exit',0)!=0 and 'clock_time_get failed' in (root/row['log']).read_text(errors='replace'):
                            row.update(status='UNAVAILABLE',layer='actual_guest_startup_refusal',
                                reason='No extra authority: official Go runtime failed before main; no ROS-denial/application credit')
                            save()
                    run('go-command-'+backend+'-Guest-explicit-monotonic','actual_guest_ROS_denial_explicit_clock',
                        [runtime,'--wasip1-global-grant','monotonic-clock','--run',out],timeout=30,marker='PASS_TASK03_GO_DENIED')
        codec=root/'go-codec';codec.mkdir();shutil.copy2(generated/'go/bounded_records.go',codec/'records.go')
        (codec/'go.mod').write_text('module uwvm-native.local/codec\n\ngo 1.24\n')
        (codec/'records_test.go').write_text('''package customtypes
import("testing";"math";"bytes")
func TestFullField(t *testing.T){v:=DefaultRos2Language_fixtureMsgAllValues();v.Title="robot";v.Child.Label="child";v.Children=[]Ros2Language_fixtureMsgSub{{Counter:math.MaxUint64,Label:"nested"}};v.Fixed_strings=[2]string{"a","b"};v.Strings=[]string{"","robot"};v.Flags=[]bool{true,false};v.Signed_values=[]int64{math.MinInt64,math.MaxInt64};v.Real_values=[]float64{-0.0,0.125};b,e:=v.Encode();if e!=nil{t.Fatal(e)};x,e:=DecodeRos2Language_fixtureMsgAllValues(b);if e!=nil{t.Fatal(e)};c,e:=x.Encode();if e!=nil||!bytes.Equal(b,c){t.Fatal("roundtrip",e)};if x.U64_value!=math.MaxUint64||x.I64_value!=math.MinInt64||x.Children[0].Counter!=math.MaxUint64{t.Fatal("64-bit narrowing")};for i:=range b{if _,e:=DecodeRos2Language_fixtureMsgAllValues(b[:i]);e==nil{t.Fatal("truncation",i)}};if _,e:=DecodeRos2Language_fixtureMsgAllValues(append(b,0));e==nil{t.Fatal("trailing")};bad:=append([]byte(nil),b...);bad[0]=2;if _,e:=DecodeRos2Language_fixtureMsgAllValues(bad);e==nil{t.Fatal("bool")};v.Title=string([]byte{0xff});if _,e:=v.Encode();e==nil{t.Fatal("utf8")};v.Title="";v.F32_value=float32(math.Inf(1));if _,e:=v.Encode();e==nil{t.Fatal("nonfinite")}}
''')
        run('go-full-field-wire','host_wire_only',[go,'test','-p=1','-count=1','-v','.'],cwd=codec)
        facade=root/'go-facades';facade.mkdir()
        shutil.copy2(generated/'go/bounded_records.go',facade/'records.go')
        shutil.copy2(generated/'go/bounded_interfaces.go',facade/'interfaces.go')
        (facade/'go.mod').write_text('module uwvm-native.local/facades\n\ngo 1.24\n\nrequire uwvm2.local/roswasmgo v0.0.0\nreplace uwvm2.local/roswasmgo => '+str(origin/'go')+'\n')
        run('go-full-field-facades','compiler',[go,'test','-p=1','-tags','ros2','-c','-o',root/'go-facades.wasm','.'],cwd=facade,environment=genv)
    else:skip('go-matrix','compiler','Pinned official Go unavailable')

    zigpaths=sorted((args.tools/'zig-input').glob('*/zig')) if (args.tools/'zig-input').is_dir() else []
    if zigpaths:
        zig=zigpaths[0];run('zig-version','inventory',[zig,'version'])
        probe=root/'zig-probe.zig'
        probe.write_text('''const std=@import("std");const records=@import("records");
test "complete fields" {var value: records.Ros2Language_fixtureMsgAllValues=.{};value.field_title="robot";var buffer: [32768]u8=undefined;const b=try value.encode(&buffer);var arena=std.heap.ArenaAllocator.init(std.testing.allocator);defer arena.deinit();const decoded=try records.Ros2Language_fixtureMsgAllValues.decode(b,arena.allocator());try std.testing.expectEqual(@as(u64,18446744073709551615),decoded.field_u64_value);try std.testing.expectEqual(@as(i64,-9223372036854775808),decoded.field_i64_value);for(0..b.len) |i| {try std.testing.expectError(error.Invalid,records.Ros2Language_fixtureMsgAllValues.decode(b[0..i],arena.allocator()));}}
''')
        run('zig-full-field-wire','host_wire_only',[zig,'test','--cache-dir',root/'zig-cache','--global-cache-dir',root/'zig-global',
            '--dep','records','-Mroot='+str(probe),'--dep','roswire','-Mrecords='+str(generated/'zig/bounded_records.zig'),'-Mroswire='+str(origin/'zig/wire.zig')])
        facade=root/'zig-facades.zig'
        facade.write_text('const std=@import("std");const interfaces=@import("interfaces");const node=@import("rosnode");test "facades" { std.testing.refAllDeclsRecursive(interfaces);_ = node.Node(4);_ = node.Executor(4); }\n')
        run('zig-full-field-facades','compiler',[zig,'test','--cache-dir',root/'zig-cache','--global-cache-dir',root/'zig-global',
             '-target','wasm32-wasi','--test-no-exec','-femit-bin='+str(root/'zig-facades.wasm'),
             '--dep','interfaces','--dep','rosnode','-Mroot='+str(facade),
             '--dep','records','--dep','client','--dep','rosnode','-Minterfaces='+str(generated/'zig/bounded_interfaces.zig'),
             '--dep','client','--dep','roswire','-Mrosnode='+str(origin/'zig/node.zig'),
             '--dep','ros','-Mclient='+str(origin/'zig/client.zig'),'-Mros='+str(origin/'zig/ros2.zig'),
             '--dep','roswire','-Mrecords='+str(generated/'zig/bounded_records.zig'),'-Mroswire='+str(origin/'zig/wire.zig')])
        for backend in ('ros1','ros2'):
            out=root/('zig-'+backend+'.wasm')
            if run('zig-command-'+backend,'compiler',[zig,'build-exe','--cache-dir',root/'zig-cache','--global-cache-dir',root/'zig-global',
                '-target','wasm32-wasi','-O','ReleaseSmall','-femit-bin='+str(out),'--dep','ros',
                '-Mroot='+str(source/'sdk/origin/examples/zig/main.zig'),'-Mros='+str(origin/'zig'/ (backend+'.zig'))]):
                run('zig-command-'+backend+'-validate','wasm_validation',[validator,out])
                if runtime is not None:
                    run('zig-command-'+backend+'-Guest','actual_guest_default_denial',[runtime,'--run',out,'deny'],timeout=20,marker='PASS_ROS_DEFAULT_DENIED')
    else:skip('zig-matrix','compiler','Pinned Zig unavailable')
    for language in ('tinygo','swift','haskell','nim','d','odin'):
        skip(language+'-guest-native','Guest/native','Actual matching compiler/runtime/bridge and current Gateway acceptance have not run in this revision')
    save()
    return 1 if any(r['status']=='FAIL' for r in rows) else 0


if __name__=='__main__':sys.exit(main())
