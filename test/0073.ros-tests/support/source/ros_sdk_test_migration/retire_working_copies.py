"""Retire only verified Task10 duplicate work trees, preserving both archives."""
import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import shutil
import stat
import sys
import tarfile


def require(value,message):
    if not value:raise ValueError(message)


def digest(stream):return hashlib.file_digest(stream,'sha256').hexdigest()


def sha(path):
    with Path(path).open('rb')as stream:return digest(stream)


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--tools',type=Path,required=True)
    a=p.parse_args();sys.path.insert(0,str(a.tools.resolve(strict=True)))
    from resource_contract import validate_resource_owner
    proof=validate_resource_owner();task=Path(proof['run']).parent
    require(task.name=='10','only Task10 owned duplicate data may be retired')
    infrastructure=task.parent/'director-20261021/infrastructure'
    selections=[('shared-profiles-r'+str(i),'work/SDK with spaces-路径')for i in range(1,8)]
    selections += [('catalog-r1','work/source-snapshot'),('catalog-r1','work/catalog/support/source'),
        ('reference-wasip1-r1','work/reference'),('runtime-denial-r2','work/source'),
        ('catalog-r1','inputs'),('reference-wasip1-r1','inputs'),('runtime-denial-r2','inputs'),
        ('sdk-test-inventory-r1','inputs'),('runtime-denial-r1','inputs')]
    selections += [('catalog-r1','work/catalog/platform')]
    selections += [('runtime-denial-r2',p.relative_to(task/'runtime-denial-r2').as_posix())
        for p in sorted((task/'runtime-denial-r2/work').glob('*.o'))]
    report=dict(kind='verified-task10-working-copy-retirement-v1',status='RUNNING',resource=proof,
        source_sha256=sha(__file__),before_disk_free=shutil.disk_usage(task).free,retired=[],skipped=[],
        immutable_archives_deleted=0,evidence_logs_deleted=0,compiled_test_or_runtime_executables_deleted=0,
        protected_shared_runs=['staged-wasi-r1','wasi-sysroot-r4','shared-profiles-r8'],
        protected_runtime_artifact='runtime-denial-r2/work/uwvm-dual',
        default_authority=[])
    result=Path('working-copy-retirement.json')
    def save():result.write_text(json.dumps(report,indent=2,sort_keys=True)+'\n')
    verified={}
    for run_id,prefix in selections:
        root=task/run_id;candidate=root/prefix
        if not candidate.exists():report['skipped'].append(dict(path=str(candidate),reason='already absent'));continue
        require(root.resolve(strict=True)==root and candidate.resolve(strict=True)==candidate,'ordinary owned working directory required')
        require((candidate.is_dir()or candidate.is_file())and candidate.is_relative_to(task),'confined owned working data required')
        object_file=candidate.is_file()
        if object_file:
            with candidate.open('rb')as stream:header=stream.read(20)
            require(run_id=='runtime-denial-r2'and candidate.suffix=='.o'and header[:4]==b'\x7fELF'
                and header[5]in(1,2)and int.from_bytes(header[16:18],'little'if header[5]==1 else'big')==1,
                'only an exact archived relocatable object, never an executable, may be retired')
        require((root/'resource-evidence/result.json').stat().st_uid==0,'original root-owned result required')
        original=json.loads((root/'resource-evidence/result.json').read_bytes())
        require(original['status']in('PASS','FAILED_RETAINED'),'working run is not retired')
        if original.get('scope'):
            require(original.get('cleanup',{}).get('scope_removed')is True,'original child must be fully retired')
            require(not Path(original['scope']['path']).exists(),'original child scope is still live')
        else:
            refused_reasons={
                'sdk-test-inventory-r1':"Refused('disk cannot admit workload, two evidence copies and reserve')",
                'runtime-denial-r1':"Refused('shared charged memory cannot admit RSS allowance and reserve')"}
            require(run_id in refused_reasons and original.get('admitted')is None
                and original['error']==refused_reasons[run_id]
                and all(original.get(key)==0 for key in('peak_sampled_disk_bytes','peak_sampled_rss_bytes','peak_sampled_processes')),
                'only the exact unadmitted Task10 inventory input duplicate is eligible')
            require(original['parent']['boot_id']==Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                'original refused owner boot changed')
            require(not(Path(original['parent']['path'])/('uwvm-director-10-'+run_id)).exists(),
                'refused original child unexpectedly exists')
        if run_id not in verified:
            record_path=root/'resource-archives/verified-copies.json'
            require(record_path.stat().st_uid==0,'original root-owned copy receipt required')
            record=json.loads(record_path.read_bytes())
            first=root/'resource-archives/evidence.tar'
            second=infrastructure/'archives'/('10-'+run_id+'.tar')
            require(record['copies']==2 and set(record['verified_paths'])=={str(first),str(second)},'both exact original archive locations required')
            for archive in (first,second):
                require(not archive.is_symlink()and archive.is_file()and archive.stat().st_uid==0,'ordinary immutable root-owned archive required')
                require(archive.stat().st_size==record['bytes']and sha(archive)==record['sha256'],'original retained archive differs')
            require(sha(root/'resource-evidence/result.json')==record['result_sha256'],'original result differs from retained receipt')
            verified[run_id]=(first,second,record)
        first,second,record=verified[run_id];expected={}
        with tarfile.open(first,'r:')as archive:
            for item in archive:
                if object_file:
                    if item.name!=prefix:continue
                    relative=''
                else:
                    if not item.name.startswith(prefix+'/'):continue
                    relative=item.name[len(prefix)+1:]
                path=PurePosixPath(relative)
                require(not path.is_absolute()and'..'not in path.parts,'confined archived member required')
                if item.isdir():continue
                require((item.isfile()or item.issym())and relative not in expected,'ordinary or exact-link unique archived source required')
                expected[relative]=('symlink',item.linkname)if item.issym()else(item.size,digest(archive.extractfile(item)))
        actual={};before_identity=candidate.stat()
        if object_file:actual['']=(candidate.stat().st_size,sha(candidate))
        for base,dirs,files in os.walk(candidate,followlinks=False):
            require(all(not(Path(base)/name).is_symlink()for name in dirs),'no linked working directories may be retired')
            for name in files:
                path=Path(base)/name;info=path.lstat()
                if stat.S_ISLNK(info.st_mode):
                    actual[path.relative_to(candidate).as_posix()]=('symlink',os.readlink(path));continue
                require(stat.S_ISREG(info.st_mode),'only ordinary duplicate files may be retired')
                actual[path.relative_to(candidate).as_posix()]=(info.st_size,sha(path))
        require(actual==expected and actual,'working copy differs from its complete original archived bytes')
        current=candidate.stat()
        require((current.st_dev,current.st_ino)==(before_identity.st_dev,before_identity.st_ino),'working directory identity changed')
        row=dict(path=str(candidate),files=len(actual),bytes=sum(value[0]for value in actual.values()if isinstance(value[0],int)),
            original_archives=[str(first),str(second)],archive_sha256=record['sha256'],
            archive_members_prefix=prefix+'/',all_file_bytes_verified=True,
            tree_pin_sha256=hashlib.sha256(json.dumps(actual,sort_keys=True).encode()).hexdigest())
        report['next_verified_retirement']=row;save()
        if object_file:candidate.unlink()
        else:shutil.rmtree(candidate)
        require(not candidate.exists(),'duplicate retirement incomplete')
        report['retired'].append(row);report.pop('next_verified_retirement',None);save()
    report.update(status='VERIFIED_DUPLICATE_WORK_COPIES_RETIRED',after_disk_free=shutil.disk_usage(task).free,
        retired_bytes=sum(row['bytes']for row in report['retired']))
    save();print(json.dumps(report),flush=True)


if __name__=='__main__':main()
