"""Locate maintained SDK checks in the separate unified test artifact."""
import json
import hashlib
import gzip
from pathlib import Path
import tarfile


def _artifact_path(root, value):
    prefix='test/0073.ros-tests/'
    if not value.startswith(prefix):raise ValueError('test map destination is outside the unified artifact')
    path=(root/value[len(prefix):]).resolve(strict=True)
    if not path.is_relative_to(root.resolve(strict=True)):raise ValueError('test map escaped its selected artifact')
    return path


def _historical_rows(root, document):
    descriptor=document.get('historical_record_index')
    if not descriptor:return {}
    path=_artifact_path(root,descriptor['path']);raw=path.read_bytes()
    if len(raw)>4<<20 or hashlib.sha256(raw).hexdigest()!=descriptor['sha256']:
        raise ValueError('historical record index changed')
    with gzip.open(path,'rb')as stream:body=stream.read((16<<20)+1)
    if len(body)>16<<20 or hashlib.sha256(body).hexdigest()!=descriptor['uncompressed_sha256']:
        raise ValueError('bounded exact historical record index required')
    rows=json.loads(body)
    if len(rows)!=descriptor['members']:raise ValueError('historical record index member count changed')
    return rows


def sdk_path(sdk_origin,*parts,test_root=None):
    """Resolve a production input or mapped test while keeping ordinary Paths."""
    root=Path(sdk_origin);relative=Path(*parts)
    if relative.is_absolute()or'..'in relative.parts:raise ValueError('SDK-relative input path required')
    original=root/relative
    if original.exists():return original
    test_root=Path(test_root)if test_root else root.parents[1]/'test/0073.ros-tests'
    manifest=test_root/'support/sdk-test-source-map.json'
    if not manifest.is_file():return original
    document=json.loads(manifest.read_bytes());name='sdk/origin/'+relative.as_posix()
    prefix='test/0073.ros-tests/'
    if name in document['sources']:
        row=document['sources'][name]
        if row.get('storage'):
            raise ValueError('historical SDK record is preserved in '+row['storage']['archive']+' member '+row['storage']['member'])
        selected=row['destination']
    else:
        historical=_historical_rows(test_root,document)
        if name in historical:
            storage=historical[name]['storage']
            raise ValueError('historical SDK record is preserved in '+storage['archive']+' member '+storage['member'])
        candidates=set()
        for source,row in document['sources'].items():
            if not source.startswith(name+'/'):continue
            suffix=source[len(name)+1:];destination=row['destination']
            if destination.endswith('/'+suffix):candidates.add(destination[:-len(suffix)-1])
        if len(candidates)!=1:return original
        selected=candidates.pop()
    relative_target=Path(selected[len(prefix):])
    if not selected.startswith(prefix)or relative_target.is_absolute()or'..'in relative_target.parts:
        raise ValueError('mapped test escaped the unified artifact')
    return test_root/relative_target


def historical_record(relative,*,test_root=None):
    """Read exact archived receipt bytes without extracting into the SDK."""
    root=Path(test_root).resolve(strict=True)if test_root else Path(__file__).resolve().parent.parent
    document=json.loads((root/'support/sdk-test-source-map.json').read_bytes())
    relative=Path(relative)
    if relative.is_absolute()or'..'in relative.parts:raise ValueError('SDK-relative historical record path required')
    name=relative.as_posix()
    if not name.startswith('sdk/'):name='sdk/origin/'+name
    rows=document['sources']
    row=rows[name]if name in rows else _historical_rows(root,document)[name]
    storage=row['storage']
    prefix='test/0073.ros-tests/'
    archive=(root/storage['archive'][len(prefix):]).resolve(strict=True)
    if not storage['archive'].startswith(prefix)or not archive.is_relative_to(root):raise ValueError('confined historical record archive required')
    with archive.open('rb')as stream:
        if hashlib.file_digest(stream,'sha256').hexdigest()!=storage['archive_sha256']:raise ValueError('historical record archive changed')
    with tarfile.open(archive,'r:gz')as source:
        member=source.getmember(storage['member'])
        if not member.isfile()or member.size!=row['original_bytes']or member.size>32<<20:raise ValueError('bounded exact historical member required')
        body=source.extractfile(member).read()
    if hashlib.sha256(body).hexdigest()!=row['original_sha256']:raise ValueError('historical record bytes changed')
    return body


def repository_root(anchor):
    path=Path(anchor).resolve()
    for parent in(path,*path.parents):
        if(parent/'sdk/origin').is_dir()and(parent/'test/0073.ros-tests').is_dir():return parent
    raise ValueError('supply a complete source checkout or an explicit test artifact root')


def sdk_test_source(sdk_origin,relative,*,test_root=None):
    sdk_origin=Path(sdk_origin).resolve(strict=True);relative=Path(relative)
    if relative.is_absolute()or'..'in relative.parts:raise ValueError('SDK-relative original test path required')
    test_root=Path(test_root).resolve(strict=True)if test_root else sdk_origin.parents[1]/'test/0073.ros-tests'
    manifest=test_root/'support/sdk-test-source-map.json'
    if manifest.is_file():
        document=json.loads(manifest.read_bytes())
        rows=document['sources'];name='sdk/origin/'+relative.as_posix()
        if name in rows:
            destination=rows[name]['destination'];prefix='test/0073.ros-tests/'
            if not destination.startswith(prefix):raise ValueError('test map destination is outside the unified artifact')
            selected=(test_root/destination[len(prefix):]).resolve(strict=True)
            if not selected.is_relative_to(test_root):raise ValueError('test map escaped its selected artifact')
            return selected
        if name in _historical_rows(test_root,document):
            raise ValueError('historical record bytes must be read through historical_record')
    # Frozen source archives and the pre-cutover read-only view remain usable.
    selected=(sdk_origin/relative).resolve(strict=True)
    if not selected.is_relative_to(sdk_origin):raise ValueError('legacy source escaped the selected SDK input')
    return selected
