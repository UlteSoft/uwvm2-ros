#!/usr/bin/env python3
"""Check exact source patches, actual runtime-tree pins and matching Rust format.

Run only inside the original finite owner. This creates no SDK, profile provider,
Wasm substitute or Guest. Native language frontend/content acceptance is separate.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tarfile


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--sha256',required=True)
    parser.add_argument('--integrated-shared',action='store_true',
                        help='Use exact current integrated shared source; do not repeat the old private patch applications')
    args=parser.parse_args()
    if digest(args.source)!=args.sha256:raise ValueError('Exact source-gate input changed')
    work=Path.cwd();source=work/'source';source.mkdir()
    with tarfile.open(args.source) as archive:
        members=archive.getmembers()
        if sum(row.size for row in members)>1280*1024 or any(
                not row.isfile() or Path(row.name).is_absolute() or '..' in Path(row.name).parts for row in members):
            raise ValueError('Bounded ordinary source-gate capsule required')
        archive.extractall(source,filter='data')
    before={str(path.relative_to(source)):digest(path) for path in source.rglob('*') if path.is_file()}
    rows=[]
    report={'kind':'task03-current-source-patch-runtime-pin-and-Rust-format-gate-v1',
        'status':'RUNNING','source_capsule_sha256':args.sha256,'captured_source_pins':before,
        'commands':rows,'native_language_compiler_runs':0,'native_language_frontend_runs':0,
        'SDK_assembly':False,'installed_SDK_builds':0,'xmake_builds':0,'Guest_runs':0,
        'native_ROS_runs':0,'documentation_semantic_acceptance':False,'default_authority':[]}
    def save():
        (work/'source-gate.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,layer,command,*,cwd=work,env=None):
        log=work/(name+'.log');row={'name':name,'layer':layer,'argv':[str(value) for value in command],
                                  'cwd':str(cwd),'log':log.name}
        rows.append(row);save()
        with log.open('wb') as stream:
            result=subprocess.run(row['argv'],cwd=cwd,env=env,stdout=stream,
                                  stderr=subprocess.STDOUT,timeout=120)
        row.update(exit=result.returncode,log_sha256=digest(log),status='PASS' if result.returncode==0 else 'FAIL')
        save()
        if result.returncode:raise ValueError('Actual source gate failed: '+name)
        return log.read_text(errors='replace')
    try:
        parsed={}
        for path in sorted(source.rglob('*.py')):
            compile(path.read_bytes(),str(path),'exec',dont_inherit=True)
            parsed[str(path.relative_to(source))]=digest(path)
        report['actual_Python_syntax']=parsed
        review=source/'sdk/origin/coordination/20261021-director/handoffs/03/review05'
        patches=[]
        patch_cases=() if args.integrated_shared else (
            ('shared-control','bounded_profile.py'),('shared-reactor','wasm_profiles.py'))
        for folder,filename in patch_cases:
            package=review/folder;metadata=json.loads((package/'PATCH.json').read_bytes())
            previous=package/'before'/filename;candidate=package/'candidates'/filename
            patch_files=list((package/'patches').glob('*.patch'))
            if len(patch_files)!=1:raise ValueError('One exact narrow captured patch required')
            patch=patch_files[0]
            if (digest(previous),digest(candidate),digest(patch))!=(
                    metadata['preimage_sha256'],metadata['candidate_sha256'],metadata['patch_sha256']):
                raise ValueError('Narrow captured source patch pins changed')
            tree=work/(folder+'-application');tree.mkdir()
            destination=tree/metadata['source'];destination.parent.mkdir(parents=True)
            shutil.copy2(previous,destination)
            run(folder+'-check','actual_git_apply_check',['/usr/bin/git','apply','--check',patch],cwd=tree)
            run(folder+'-apply','actual_private_git_apply',['/usr/bin/git','apply',patch],cwd=tree)
            if digest(destination)!=metadata['candidate_sha256']:
                raise ValueError('Actual narrow patch postimage differs')
            compile(destination.read_bytes(),str(destination),'exec',dont_inherit=True)
            patches.append({'source':metadata['source'],'preimage_sha256':metadata['preimage_sha256'],
                'postimage_sha256':digest(destination),'patch_sha256':digest(patch),'status':'PASS_PRIVATE_PATCH_AND_SYNTAX'})
        report['narrow_patch_applications']=patches
        if args.integrated_shared:
            report['current_integrated_shared_source']={
                name:digest(source/name) for name in ('sdk/origin/tools/wasm_profiles.py',
                                                     'sdk/origin/custom-types/bounded_profile.py')}
            report['shared_regression_scope']='Actual Python syntax only; installed source behavior belongs to the public SDK jobs'
        save()
        # Actual retained Go root data exercises a complete closure above the
        # shared executable-list bound; no Go binary or TinyGo compiler is run.
        agents=Path('/home/macromodel/Documents/uwvm-test-261018/agents/03')
        toolroot=agents/'20261021-toolchains-r1/work/toolchains'
        goroot=toolroot/'go-input/go'
        if (goroot/'VERSION').read_text().splitlines()[0]!='go1.24.2':
            raise ValueError('Actual retained Go input root changed')
        sys.path.insert(0,str(source/'sdk/origin/tools'))
        from native_languages import reactors
        tree_pins=[]
        for path in sorted(goroot.rglob('*')):
            if path.is_file():
                tree_pins.append({'path':str(path.relative_to(toolroot)),
                                  'bytes':path.stat().st_size,'sha256':digest(path)})
        if len(tree_pins)<=1024:raise ValueError('Actual large complete runtime-root control required')
        resolved={'toolchain_root':str(toolroot),'tool_pins':[]}
        variant={'runtime_tree_pins':tree_pins}
        actual_pins=reactors.runtime_pin_table(resolved,variant)
        membership=reactors.pinned_tree(goroot,resolved,actual_pins)
        refused=[]
        def refusal(name,operation):
            try:operation()
            except ValueError as error:refused.append({'name':name,'error':str(error)})
            else:raise ValueError('Actual runtime pin negative control was admitted: '+name)
        first=next(iter(actual_pins))
        missing=dict(actual_pins);missing.pop(first)
        refusal('actual-unlisted-runtime-file',lambda:reactors.pinned_tree(goroot,resolved,missing))
        corrupt=dict(actual_pins);corrupt[first]={**corrupt[first],'sha256':'0'*64}
        refusal('actual-runtime-file-wrong-digest',lambda:reactors.pinned_tree(goroot,resolved,corrupt))
        refusal('duplicate-runtime-path',lambda:reactors.runtime_pin_table(resolved,
            {'runtime_tree_pins':[tree_pins[0],tree_pins[0]]}))
        if reactors.pinned_tree(goroot,resolved,actual_pins)!=membership:
            raise ValueError('Actual retained runtime tree changed during checks')
        report['actual_runtime_tree_gate']={'status':'PASS_ACTUAL_COMPLETE_ROOT_AND_THREE_REFUSALS',
            'root':str(goroot),'file_count':len(tree_pins),'total_file_bytes':sum(row['bytes'] for row in tree_pins),
            'ordered_tree_pins_sha256':hashlib.sha256(json.dumps(tree_pins,sort_keys=True).encode()).hexdigest(),
            'negative_controls':refused,'actual_Go_binary_runs':0,'TinyGo043_pair_qualification':False,
            'scope':'Actual retained Go1.24.2 root data only; not a supported0.43 pair or compiler/SDK test'}
        fmt=agents/'20261021-round03-rustfmt-r1/work/tools/bin/rustfmt'
        if digest(fmt)!='5d90e4b57cf7ae89a83c35428a245c0de9fa050fd13c5ad7f19b2ee4b561ae6f':
            raise ValueError('Matching official Rust1.85 formatter program changed')
        library=toolroot/'rust/lib'
        tool_pins={str(path):{'bytes':path.stat().st_size,'sha256':digest(path)}
            for path in [fmt,*sorted(library.glob('*.so*'))] if path.is_file()}
        environment=dict(os.environ);environment['LD_LIBRARY_PATH']=str(library)
        version=run('rustfmt-version','actual_matching_formatter_version',[fmt,'--version'],env=environment)
        if version.strip()!='rustfmt 1.8.0-stable (4d91de4e48 2025-02-17)':
            raise ValueError('Actual matching Rust1.85 formatter version differs')
        configuration=work/'rustfmt.toml';configuration.write_text('edition = "2021"\nmax_width = 100\n')
        files=[source/'sdk/origin/rust/src'/name for name in ('capability.rs','codec.rs','events.rs','session.rs')]
        command=[fmt,'--config-path',configuration,*files]
        run('Rust-format-first','actual_Rust185_rustfmt',command,env=environment)
        first_format={str(path.relative_to(source)):digest(path) for path in files}
        run('Rust-format-second','actual_Rust185_rustfmt',command,env=environment)
        if any(digest(source/name)!=pin for name,pin in first_format.items()):
            raise ValueError('Actual matching Rust formatter is not byte-idempotent')
        changes=[]
        with tarfile.open(work/'Rust-format-candidates.tar','w') as archive:
            for path in files:
                name=str(path.relative_to(source))
                if digest(path)!=before[name]:
                    changes.append({'path':name,'before_sha256':before[name],
                                    'after_sha256':digest(path),'bytes':path.stat().st_size})
                    archive.add(path,arcname=name,recursive=False)
        if any(digest(source/name)!=pin for name,pin in before.items() if name not in first_format):
            raise ValueError('Captured non-format source changed')
        for spelling,pin in tool_pins.items():
            if Path(spelling).stat().st_size!=pin['bytes'] or digest(Path(spelling))!=pin['sha256']:
                raise ValueError('Actual formatter/shared-library input changed')
        report.update(status=('PASS_CURRENT_INTEGRATED_SOURCE_RUNTIME_PIN_AND_RUST_FORMAT_GATES'
            if args.integrated_shared else 'PASS_SCOPED_SOURCE_PATCH_RUNTIME_PIN_AND_RUST_FORMAT_GATES'),
            formatter={'actual_passes':2,'configuration_sha256':digest(configuration),
                'tool_library_pins':tool_pins,'source_after_sha256':first_format,'idempotent':True,
                'changes':changes,'candidate_archive_sha256':digest(work/'Rust-format-candidates.tar')},
            all_public_API_documentation_accepted=False)
        save();return 0
    except Exception:
        report['status']='FAILED_RETAINED';save();raise


if __name__=='__main__':raise SystemExit(main())
