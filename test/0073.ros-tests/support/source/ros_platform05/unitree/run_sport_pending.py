#!/usr/bin/env python3
"""Build all new local ledger contracts, then run actual Wasm and host-JS lanes.

Invoke only as an admitted task-05 workload of the published shared resource
owner. The frozen SDK/toolchains stay read-only. This tests an exact maintenance
overlay, not a newly published SDK or a native/Gateway/controller path.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys


def fact(path):
    path = Path(path)
    with path.open('rb') as stream:
        digest = hashlib.file_digest(stream, 'sha256').hexdigest()
    return dict(path=str(path), bytes=path.stat().st_size, sha256=digest)


def mapped(path):
    path = Path(path)
    if not path.exists():
        path = Path('/host' + str(path))
    return path.resolve(strict=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--frozen-root', type=Path, required=True)
    parser.add_argument('--overlay', type=Path, required=True)
    parser.add_argument('--test-source-root', type=Path,
        help='Separate canonical Unitree tests; select archived embedded inputs explicitly')
    parser.add_argument('--toolchains', type=Path,
        help='Optional inert readable copy of the original locked toolchains')
    parser.add_argument('--node', type=Path, help='Explicit separately locked standalone Node producer')
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    run = Path(os.environ['UWVM_RESOURCE_RUN']).resolve(strict=True)
    scope = os.environ['UWVM_RESOURCE_SCOPE']
    assert os.environ['UWVM_RESOURCE_HEAVY_LOCK_HELD'] == '1'
    assert Path('/proc/self/cgroup').read_text().strip().endswith('/' + Path(scope).name)
    out = args.out.absolute()
    assert out.is_relative_to(run / 'work') and not out.exists()
    out.mkdir()
    frozen = args.frozen_root.resolve(strict=True)
    overlay = args.overlay.resolve(strict=True)
    tests = (args.test_source_root or Path(__file__).resolve().parent).resolve(strict=True)
    sdk = frozen / 'out/sdk-layout-sport-prepare-r3/sdk'
    inputs = json.loads((frozen / 'inputs.json').read_bytes())
    tc = args.toolchains.resolve(strict=True) if args.toolchains else mapped(inputs['toolchains'])
    # An inert byte-preserving reference copy may be necessary because the
    # original root-owned evidence is deliberately unreadable by the workload
    # UID. Do not chmod/mutate that historical evidence to make a test pass.
    runtime = (frozen/'tools/uwvm2-ros').resolve(strict=True)
    validator = tc/'wasm-tools-1.262.0-x86_64-linux/wasm-tools'
    tools = dict(cargo=tc/'rust-1.90.0/bin/cargo', rustc=tc/'rust-1.90.0/bin/rustc',
        tinygo=tc/'tinygo/bin/tinygo', goroot=tc/'go', node=args.node.resolve(strict=True) if args.node else Path('/usr/bin/node') if Path('/usr/bin/node').is_file() else Path('/host/usr/bin/node'),
        asc=tc/'node_modules/assemblyscript/bin/asc.js')
    # The reference container has no Node installation. The retained host Node
    # uses its original loader and library tree, all read-only, under the same
    # admitted child cgroup. No chroot, privileged child or native ROS is used.
    node_prefix = [tools['node']]
    if tools['node']==Path('/host/usr/bin/node'):
        node_prefix = [Path('/host/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2'),
            '--library-path','/host/lib/x86_64-linux-gnu:/host/usr/lib/x86_64-linux-gnu',tools['node']]
    report = dict(status='RUNNING', commands=[], products=[], guest_executions=[],
        host_executions=[], inputs=[], default_authority=[],
        source_kind='maintenance overlay over immutable public SDK',
        current_public_SDK_qualified=False, native_ROS_cases=0, Gateway_cases=0,
        physical_cases=0, official_suite_cases=0, QEMU_cases=0, VTune_cases=0,
        toolchain_shared_dependency_closure_qualified=False)
    original = {}
    def pin(path):
        path = Path(path).resolve(strict=True)
        if path not in original:
            original[path] = fact(path)
            report['inputs'].append(original[path])
        return path
    def save():
        (out/'result.json').write_text(json.dumps(report, indent=2)+'\n')
    def command(label, argv, cwd=out, env=None):
        directory = out/'commands'/label
        directory.mkdir(parents=True)
        row = dict(label=label, argv=list(map(str, argv)), cwd=str(cwd), exit=None)
        report['commands'].append(row)
        save()
        with (directory/'stdout').open('xb') as so, (directory/'stderr').open('xb') as se:
            result = subprocess.run(row['argv'], cwd=cwd, env=env, stdin=subprocess.DEVNULL,
                stdout=so, stderr=se, timeout=600)
        row.update(exit=result.returncode, stdout=fact(directory/'stdout'), stderr=fact(directory/'stderr'))
        save()
        if result.returncode:
            raise RuntimeError((label, result.returncode, (directory/'stderr').read_text()[-4000:]))
        return (directory/'stdout').read_bytes()
    def stage(source, target, transform=None):
        source = pin(source)
        body = source.read_bytes()
        if transform:
            body = transform(body.decode()).encode()
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(body)
        return target
    def rebase(source, target):
        def change(match):
            old = match[2]
            if not old.startswith('.'):
                return match[0]
            value = os.path.relpath((source.parent/old).resolve(), target.parent)
            if not value.startswith('.'):
                value = './'+value
            return 'from '+match[1]+value+match[1]
        return lambda text: re.sub(r'from\s+([\'\"])([^\'\"]+)\1', change, text)
    try:
        pin(__file__)
        pin(frozen/'inputs.json')
        pin(sdk/'share/distribution-manifest.json')
        pin(sdk/'share/abi/abi-v1.json')
        pin(sdk/'tools/check_wasm_imports.py')
        review=pin(tests/'review_local_imports.py')
        for value in tools.values():
            if value.is_file():
                pin(value)
        for value in (runtime, validator, tc/'go/bin/go', tc/'node_modules/assemblyscript/dist/asc.js',
                tc/'node_modules/assemblyscript/dist/assemblyscript.js'):
            pin(value)
        if len(node_prefix)>1:
            pin(node_prefix[0])
        command('version-rustc',[tools['rustc'],'--version','--verbose'])
        command('version-cargo',[tools['cargo'],'--version'])
        command('version-node',node_prefix+['--max-old-space-size=256','--disable-wasm-trap-handler','--version'])
        go_environment=dict(os.environ,GOROOT=str(tools['goroot']),GOTOOLCHAIN='local',
            PATH=str(tools['goroot']/'bin')+':'+os.environ['PATH'])
        command('version-go',[tools['goroot']/'bin/go','version'],env=go_environment)
        command('version-tinygo',[tools['tinygo'],'version'],env=go_environment)
        # Pin all source inputs in the four borrowed language SDK modules. This
        # is source preservation, not a claim about every compiler shared DSO.
        for language in ('rs','as','tinygo','js'):
            for path in sorted((sdk/'include/platform/ros'/language).rglob('*')):
                if path.is_file():
                    pin(path)
        artifacts = {}
        for language, suffix in (('rs','rs'), ('as','ts'), ('tinygo','go'), ('js','mjs')):
            project = out/language
            project.mkdir()
            record = sdk/'include/platform/unitree'/language/'ros2/unitree-api-complete'/('bounded_records.'+suffix)
            target = project/('records/bounded_records.go' if language=='tinygo' else 'bounded_records.'+suffix)
            stage(record, target, rebase(record,target) if language in ('as','js') else None)
            sport_target = project/('records/sport.go' if language=='tinygo' else 'sport.'+suffix)
            stage(overlay/'include'/('sport.'+suffix), sport_target,
                (lambda text:text.replace('../../../custom-types/unitree-api-complete/bounded_records','./bounded_records'))
                if language in ('as','js') else None)
            entry = stage(tests/('sport_pending.'+suffix), project/('main.'+suffix))
            if language == 'rs':
                (project/'Cargo.toml').write_text('[package]\nname="unitree-sport-pending"\nversion="0.1.0"\nedition="2021"\npublish=false\n'
                    '[dependencies]\nroswasm={path='+json.dumps(str(sdk/'include/platform/ros/rs'))+',default-features=false,features=["ros2","alloc"]}\n'
                    '[[bin]]\nname="unitree-sport-pending"\npath="main.rs"\n')
                env = dict(os.environ, RUSTC=str(tools['rustc']), CARGO_HOME=str(project/'cargo-home'), CARGO_BUILD_JOBS='1')
                flags = ['-C','codegen-units=1','-C','debuginfo=0','-C','link-arg=--threads=1',
                    '-C','link-arg='+str(pin(sdk/'lib/wasm32-wasip1/libc.a'))]
                env['CARGO_ENCODED_RUSTFLAGS'] = '\x1f'.join(flags)
                command('build-rs',[tools['cargo'],'build','--offline','--release','-j1','--target','wasm32-wasip1',
                    '--target-dir',project/'target'],project,env)
                artifacts[language] = project/'target/wasm32-wasip1/release/unitree-sport-pending.wasm'
            elif language == 'as':
                # Pure record contracts need not otherwise import the API
                # module. Call its real local-trap handler from the entry
                # and select that actual module identity, as the SDK documents.
                api = os.path.relpath(sdk/'include/platform/ros/as/api',project)
                if not api.startswith('.'):
                    api = './'+api
                entry.write_text(entry.read_text()+'\nimport {abortGuest as sdkAbortGuest} from '+json.dumps(api)+';\n'
                    'export function task05Abort(message:usize,fileName:usize,line:u32,column:u32):void{sdkAbortGuest(message,fileName,line,column);}\n')
                artifacts[language] = project/'pending.wasm'
                command('build-as',node_prefix+['--max-old-space-size=256','--disable-wasm-trap-handler',tools['asc'],'./main.ts',
                    '--outFile',artifacts[language],'--runtime','incremental','--initialMemory','128',
                    '--maximumMemory','512','--optimizeLevel','0','--shrinkLevel','0','--use','abort=main/task05Abort'],project)
            elif language == 'tinygo':
                (project/'go.mod').write_text('module example.local/unitree-sport\n\ngo 1.23\nrequire uwvm2.local/roswasm v0.0.0\n'
                    'replace uwvm2.local/roswasm => '+json.dumps(str(sdk/'include/platform/ros/tinygo'))+'\n')
                env = dict(os.environ, GOMAXPROCS='1', GOFLAGS='-p=1', GOTOOLCHAIN='local', GOPROXY='off', GOSUMDB='off',
                    GOWORK='off', CGO_ENABLED='0', GOCACHE=str(project/'go-cache'), GOMODCACHE=str(project/'go-modcache'),
                    GOROOT=str(tools['goroot']), PATH=str(tools['goroot']/'bin')+':'+os.environ['PATH'])
                artifacts[language] = project/'pending.wasm'
                command('build-tinygo',[tools['tinygo'],'build','-p','1','-target=wasip1','-scheduler=none','-opt=0',
                    '-panic=trap','-tags=ros2','-o',artifacts[language],'.'],project,env)
            else:
                (project/'run.mjs').write_text("import {localPending} from './main.mjs';\nconsole.log(localPending());\n")
        for language, path in artifacts.items():
            command('validate-'+language,[validator,'validate',path])
            command('imports-'+language,[sys.executable,'-I','-S','-B',review,path,
                '--checker',sdk/'tools/check_wasm_imports.py','--report',out/language/'imports.json'])
            report['products'].append(dict(language=language, artifact=fact(path)))
        # All builds and validation finish before the first Guest execution.
        expected = b'SPORT_PENDING_OK operations=39 capacity=16 identity_bits=64\n'
        for language, path in artifacts.items():
            body = command('guest-'+language,[runtime,'-Rint','--wasip1-global-noinherit-system-environment','--run',path])
            assert body == expected, (language,body)
            report['guest_executions'].append(dict(language=language,operations=39,scope='local correlation only'))
        body = command('host-js',node_prefix+[out/'js/run.mjs'])
        assert body == expected
        report['host_executions'].append(dict(language='js',operations=39,scope='host-only local correlation'))
        assert all(fact(path)==row for path,row in original.items())
        report.update(status='PASS',input_pins_preserved=True,wasm_products=3,guest_runs=3,host_js_runs=1)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
