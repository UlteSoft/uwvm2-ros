"""Overlay canonical checks only into an owned temporary production copy."""
import hashlib
import json
from pathlib import Path
import shutil
from resource_contract import validate_resource_owner


def require(value,message):
    if not value:raise ValueError(message)


def overlay(language,target,*,test_root=None):
    """Keep private language tests in their real original module scope."""
    proof=validate_resource_owner();target=Path(target).resolve(strict=True)
    require(target.is_dir()and target.is_relative_to(Path(proof['run'])/'work'),
        'test overlays require an existing production copy under the active owner work directory')
    require(language in('rust','go','tinygo','zig','moonbit'),'explicit supported module staging language required')
    root=Path(test_root).resolve(strict=True)if test_root else Path(__file__).resolve().parent.parent
    mapping=json.loads((root/'support/sdk-test-source-map.json').read_bytes())['sources']
    prefix='test/0073.ros-tests/';copied=[];written=set()
    for original,row in mapping.items():
        if not original.startswith('sdk/origin/'+language+'/'):continue
        relative=Path(original[len('sdk/origin/'+language+'/'):])
        selected=(language=='rust'and relative.parts[0]=='tests'and relative.suffix=='.rs'
            or language=='rust'and relative.as_posix()=='owned_service/src/contracts.rs'
            or language in('go','tinygo')and relative.name.endswith('_test.go')
            or language=='zig'and relative.name.startswith('test_')and relative.suffix=='.zig'
            or language=='moonbit'and relative.name.endswith(('_test.mbt','_wbtest.mbt')))
        if not selected:continue
        source=(root/row['destination'][len(prefix):]).resolve(strict=True)
        require(row['destination'].startswith(prefix)and source.is_relative_to(root),'canonical fixture escaped its artifact')
        destination=target/relative
        require(not destination.exists()and destination not in written,'production copy already contains test source')
        destination.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,destination);written.add(destination)
        raw=destination.read_bytes();copied.append(dict(source=str(source),destination=str(destination),sha256=hashlib.sha256(raw).hexdigest()))
    if language in('rust','zig'):
        plan=json.loads((root/'support/sdk-inline-test-extraction.json').read_bytes())
        for row in plan['blocks']:
            source_prefix='sdk/origin/'+language+'/'
            if not row['source'].startswith(source_prefix):continue
            relative=Path(row['source'][len(source_prefix):]);destination=target/relative
            fixture=(root/row['destination'][len(prefix):]).resolve(strict=True)
            require(fixture.is_relative_to(root),'inline fixture escaped its artifact')
            block=fixture.read_bytes()
            require(len(block)==row['bytes']and hashlib.sha256(block).hexdigest()==row['sha256'],'exact extracted test block changed')
            require(destination.is_file()and block not in destination.read_bytes(),'actual copied module required without embedded tests')
            with destination.open('ab')as stream:stream.write(block)
            copied.append(dict(source=str(fixture),destination=str(destination),sha256=row['sha256'],operation='append original private test scope'))
    if language=='moonbit':
        fixture=root/'support/source/sdk-native-languages/moonbit/test-link-reservation.moon.pkg'
        block=fixture.read_bytes()
        require(hashlib.sha256(block).hexdigest()=='c64f3273585589f6c595ea2cdd59e522a905427ac34d02fbbc66c8b9be60d2d5',
            'exact external MoonBit test linker reservation required')
        for package in ('linear','ros1','ros2','wire','owned_service'):
            destination=target/package/'moon.pkg'
            require(destination.is_file()and block not in destination.read_bytes(),
                'test-free copied MoonBit package required')
            with destination.open('ab')as stream:stream.write(block)
            copied.append(dict(source=str(fixture),destination=str(destination),
                sha256=hashlib.sha256(block).hexdigest(),operation='append external test linker reservation'))
    return dict(kind='canonical-sdk-test-overlay-v1',language=language,resource=proof,files=copied,
        installed_sdk_modified=False,default_authority=[],execution_credit=0)


def native_languages_tools(origin,*,test_root):
    """Rehydrate removed checks only in the owner's extracted source copy."""
    proof=validate_resource_owner();origin=Path(origin).resolve(strict=True)
    require(origin.is_dir()and origin.is_relative_to(Path(proof['run'])/'work'),
        'native checks require an owned temporary production source copy')
    root=Path(test_root).resolve(strict=True);helper=origin/'tools/native_languages'
    require(helper.is_dir()and helper.resolve()==helper,'ordinary production helper required')
    old=(helper/'fixtures/catalog.json',helper/'test_profile.py',helper/'test_arguments.py')
    receipt=dict(kind='canonical-native_languages-test-stage-v1',resource=proof,files=[],overlays=[],
        installed_sdk_modified=False,execution_credit=0,default_authority=[])
    if all(path.exists()for path in old):
        receipt['mode']='embedded-source-tests-retained-without-current-overlay'
        for path in(helper/'fixtures',*old[1:]):
            for source in sorted(path.rglob('*'))if path.is_dir()else(path,):
                if source.is_file():
                    require(not source.is_symlink(),'ordinary frozen test source required')
                    receipt['files'].append(dict(destination=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
        return receipt
    require(not any(path.exists()for path in old),'partial legacy/current test mixture refused')
    require((root/'support/sdk-test-source-map.json').is_file(),'complete separate test artifact required')
    canonical=root/'support/source/sdk-native-languages'
    selected=[*(canonical/'fixtures').rglob('*'),canonical/'configuration/test_profile.py',
        canonical/'configuration/test_arguments.py',*(canonical/'drivers').glob('*.py')]
    for source in sorted(selected):
        if source.is_dir():continue
        require(source.is_file()and not source.is_symlink()and source.resolve().is_relative_to(root),
            'ordinary confined canonical test source required')
        relative=source.relative_to(canonical)
        if relative.parts[0]=='fixtures':destination=helper/relative
        else:destination=helper/relative.name
        require(not destination.exists(),'production helper unexpectedly contains a test consumer')
        destination.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,destination)
        receipt['files'].append(dict(source=str(source),destination=str(destination),
            sha256=hashlib.sha256(destination.read_bytes()).hexdigest()))
    # The copied configuration checks locate their selected production helper
    # beside sdk/origin, while resolving shared support through this test link.
    test_link=origin.parents[1]/'test/0073.ros-tests'
    if test_link.exists():require(test_link.resolve()==root,'selected test artifact differs from extracted source')
    else:
        test_link.parent.mkdir(parents=True,exist_ok=True);test_link.symlink_to(root,target_is_directory=True)
    for language in('go','zig'):
        if(origin/language).is_dir():receipt['overlays'].append(overlay(language,origin/language,test_root=root))
    receipt['mode']='separate-canonical-tests-in-owned-private-copy'
    return receipt
