"""Verify complete inert Python SDK upgrade, reuse and foreign-edit refusal.

The private fixture retains old view documents from a read-back checkpoint.
Only transport/path/hash logic runs; no transported SDK or application executes.
The fixture is a duplicate and is retired after the checked transport results.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys
import tarfile


def sha(path):
    with Path(path).open('rb') as stream: return hashlib.file_digest(stream,'sha256').hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True); p.add_argument('--out',type=Path,required=True)
    a=p.parse_args(); root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes()); source=Path(inputs['source_root'])
    assert out.parent==root/'out'and not out.exists(); out.mkdir(mode=0o700)
    prepared=root/inputs['unitree_python_source']
    prior=json.loads((prepared/'previous-layout.json').read_bytes())
    assert prior['kind']=='checked-public-sdk-view-source-state-v1'
    report=dict(status='RUNNING',commands=[],default_authority=[],transported_code_executed=False,
                scope='Inert SDK delivery and identical static-link input capture')
    def save(): (out/'unitreepythondelivery.json').write_text(json.dumps(report,indent=2)+'\n')
    clone=out/'prior-source'
    try:
        # Actual linker trace resolves every consumed ordinary archive/object.
        # Re-linking emits identical bytes to the already executed products;
        # the Guest/application itself is not executed by this closure step.
        accepted=root/'out/unitreepythonwasm-r1'
        acceptance=json.loads((accepted/'unitreepythonwasm.json').read_bytes())
        assert acceptance['status']=='COMPLETE_UNITREE_PYTHON_SPORT_ASYNC_AND_TWENTY_SEVEN_PROFILE_REAL_WASM_PASS'
        dependencies={}; links=[]
        for version in (1,2):
            build=json.loads((accepted/('ros'+str(version))/'build.json').read_bytes())
            link=next(row for row in build['commands']if row['label']=='link')
            # wasm-ld records the output basename in the module name section.
            # Preserve that basename when verifying byte-identical output.
            product=out/('ros'+str(version))/'vendor-python.wasm'
            product.parent.mkdir()
            argv=list(link['argv']); assert argv[-2]=='-o'; argv[-1]=str(product)
            argv.insert(1,'-Wl,--trace')
            def trace(suffix):
                label='link-ros'+str(version)+'-'+suffix
                with(out/(label+'.stdout')).open('xb')as stdout,(out/(label+'.stderr')).open('xb')as stderr:
                    result=subprocess.run(argv,cwd=link['cwd'],stdin=subprocess.DEVNULL,stdout=stdout,stderr=stderr,timeout=180,check=False)
                report['commands'].append(dict(label=label,argv=argv,cwd=link['cwd'],exit=result.returncode)); save()
                assert result.returncode==0,(out/(label+'.stderr')).read_text()[-2000:]
                assert product.stat().st_size==build['artifact']['bytes']and sha(product)==build['artifact']['sha256']
                return(out/(label+'.stdout')).read_text()
            trace_text=trace('resolve')
            product.unlink()
            compiler=Path(argv[0]).resolve(strict=True)
            tool=dict(path=str(compiler),bytes=compiler.stat().st_size,sha256=sha(compiler))
            paths=set()
            for line in trace_text.splitlines():
                match=re.fullmatch(r'(.+?\.(?:o|a))(?:\(.*\))?',line.strip())
                assert match,repr(line)
                path=Path(match[1]); paths.add((Path(link['cwd'])/path).resolve(strict=True))
            for role in ('entry','bridge'):
                dep=accepted/('ros'+str(version))/(role+'.d')
                raw=dep.read_text().replace('\\\n',' ')
                for word in shlex.split(raw.partition(':')[2]):
                    paths.add((source/word).resolve(strict=True))
            captured=[]
            for path in sorted(paths):
                assert path.is_file()and path.stat().st_size<=128<<20
                before=path.stat(); digest=sha(path)
                # The existing owned source/object/frozen header is already
                # retained by its explicit role. Copy only external sysroot
                # dependencies, avoiding a second copy of large frozen arrays.
                destination=path if path.is_relative_to(root)else out/'dependency-objects'/digest
                if destination!=path:
                    destination.parent.mkdir(exist_ok=True)
                    if not destination.exists(): shutil.copyfile(path,destination)
                assert sha(destination)==digest
                after=path.stat()
                assert all(getattr(before,k)==getattr(after,k)for k in('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns'))
                row=dict(path=str(path),bytes=before.st_size,sha256=digest,copy=str(destination))
                dependencies[str(path)]=row; captured.append(row)
            assert trace('recheck')==trace_text
            assert all(Path(row['path']).stat().st_size==row['bytes']and sha(row['path'])==row['sha256']for row in captured)
            assert compiler.stat().st_size==tool['bytes']and sha(compiler)==tool['sha256']
            links.append(dict(ros_version=version,identical_artifact=build['artifact'],inputs=len(captured),compiler_tool_identity=tool,
                duplicate_trace_product_retired_after_hash_match=True))
            product.unlink()
        report.update(link_closure=links,ordinary_compiler_and_linker_inputs=list(dependencies.values()),
            copied_external_dependency_objects=len(list((out/'dependency-objects').iterdir())))
        save()
        def copy(old,new):
            if Path(old).relative_to(source/'sdk/origin').parts[0]in('platform','language'):
                return shutil.copyfile(old,new)
            return os.link(old,new)
        shutil.copytree(source/'sdk/origin',clone/'sdk',symlinks=True,copy_function=copy)
        evidence=root.parent.parent/'recovery/ros-remaining-r60'
        retained=json.loads((evidence/'unitree-complete-sport-source-and-acceptance-r1.json').read_bytes())
        assert sha(retained['primary'])==retained['sha256']
        wanted={}
        with tarfile.open(retained['primary'],'r|gz')as archive:
            first=next(iter(archive)); raw=archive.extractfile(first).read()
            assert hashlib.sha256(raw).hexdigest()==retained['manifest_sha256']
            for row in json.loads(raw)['roles']:
                if row['type']!='regular'or not row['path'].startswith('source/sdk/'): continue
                name=row['path'][len('source/sdk/'):]
                if name not in prior['files']: continue
                path=clone/'sdk'/name
                if sha(path)==row['sha256']: continue
                wanted.setdefault(row['object'],[]).append((path,row))
            restored=0
            for member in archive:
                if member.name not in wanted: continue
                assert member.isfile()and member.size<=16<<20
                data=archive.extractfile(member).read()
                for path,row in wanted.pop(member.name):
                    assert len(data)==row['bytes']and hashlib.sha256(data).hexdigest()==row['sha256']
                    path.write_bytes(data); restored+=1
        assert not wanted and restored>0
        for name,pin in prior['files'].items():
            path=clone/'sdk'/name
            assert path.stat().st_size==pin['bytes']and sha(path)==pin['sha256']
        report['checked_prior_view_documents_restored']=restored; save()
        tool=source/'test/0073.ros-tests/support/source/ros_platform_sdk/io_layout_delivery.py'
        def transfer(label,expected=0):
            export=[sys.executable,'-I','-S','-B',tool,'export','--source',source,'--receipt',out/(label+'-export.json')]
            receive=[sys.executable,'-I','-S','-B',tool,'receive','--source',clone,'--receipt',out/(label+'-receive.json')]
            with(out/(label+'-export.stderr')).open('xb')as export_error,(out/(label+'-receive.stdout')).open('xb')as stdout,(out/(label+'-receive.stderr')).open('xb')as stderr:
                actor=subprocess.Popen(list(map(str,export)),stdout=subprocess.PIPE,stderr=export_error)
                try:
                    result=subprocess.run(list(map(str,receive)),stdin=actor.stdout,stdout=stdout,stderr=stderr,timeout=180,check=False)
                    actor.stdout.close(); exit_code=actor.wait(timeout=180)
                finally:
                    if actor.poll()is None: actor.kill(); actor.wait()
            report['commands'] += [dict(label=label+'-export',exit=exit_code),dict(label=label+'-receive',exit=result.returncode)]; save()
            if expected==0:
                assert exit_code==result.returncode==0,(out/(label+'-receive.stderr')).read_text()[-2000:]
                sent,received=(json.loads((out/(label+'-'+name+'.json')).read_bytes())for name in('export','receive'))
                assert all(sent[k]==received[k]for k in('manifest_sha256','rows','ordinary_files','relative_links','ordinary_bytes'))
                return received
            assert result.returncode!=0 and b'AssertionError'in(out/(label+'-receive.stderr')).read_bytes()
        received=transfer('prior-upgrade')
        assert transfer('identical')['manifest_sha256']==received['manifest_sha256']
        foreign=clone/'sdk/origin/platform/unitree/python/include/README.md'
        assert foreign.is_file()
        foreign.write_text('independent user edit\n')
        absent=clone/'sdk/origin/platform/unitree/python/include/ros2/unitree-api-complete/bounded_records.py'
        assert absent.is_file(); absent.unlink()
        transfer('foreign',1)
        assert foreign.read_text()=='independent user edit\n'and not absent.exists()
        shutil.rmtree(clone)
        report.update(status='UNITREE_PYTHON_INERT_PRIOR_UPGRADE_IDENTICAL_AND_FOREIGN_REFUSAL_PASS',
            transport=received,private_fixture_retired=True)
        print(report['status'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error)); raise
    finally: save()


if __name__=='__main__': main()
