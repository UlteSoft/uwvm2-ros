#!/usr/bin/env python3
"""Read digest-pinned vendor definition archives as data; never load SDK code."""
import json
from pathlib import Path, PurePosixPath
import shutil
import stat
import zipfile
from urllib.parse import urlparse

from acquire import audit, fetch, save
from capability_inventory import sha


def relative(value):
    path=PurePosixPath(value)
    if (not isinstance(value,str) or not value or '\\' in value or '\x00' in value or ':' in value or
            path.is_absolute() or not path.parts or '..' in path.parts or path.as_posix()!=value.removesuffix('/')):
        raise ValueError('noncanonical vendor archive path')
    return path


def extract(archive, destination, roots):
    selected=[relative(root) for root in roots]
    if not selected or len(set(selected))!=len(selected):raise ValueError('missing or duplicate definition roots')
    observed=set();names=set();expanded=0;files=0
    with zipfile.ZipFile(archive) as source:
        entries=source.infolist()
        if len(entries)>100000:raise ValueError('vendor archive member limit exceeded')
        for entry in entries:
            path=relative(entry.filename)
            chosen=[root for root in selected if path.is_relative_to(root)]
            if not chosen:continue
            if path in names:raise ValueError('duplicate vendor definition member')
            names.add(path)
            mode=entry.external_attr>>16
            if stat.S_ISLNK(mode) or (stat.S_IFMT(mode) and not (stat.S_ISREG(mode) or stat.S_ISDIR(mode))):
                raise ValueError('definition archive contains a link or special file')
            if entry.flag_bits&1:raise ValueError('encrypted definition is not reviewable')
            target=destination/str(path)
            if entry.is_dir():target.mkdir(parents=True,exist_ok=True);continue
            expanded+=entry.file_size
            if entry.file_size<0 or entry.file_size>8<<20 or expanded>512<<20:raise ValueError('definition expansion limit exceeded')
            target.parent.mkdir(parents=True,exist_ok=True)
            count=0
            with source.open(entry) as incoming,target.open('xb') as output:
                while data:=incoming.read(65536):
                    count+=len(data)
                    if count>entry.file_size:raise ValueError('definition expands beyond declared size')
                    output.write(data)
            if count!=entry.file_size:raise ValueError('truncated vendor definition')
            target.chmod(0o644);observed.update(chosen);files+=1
    if observed!=set(selected):raise ValueError('a pinned definition root is absent')
    return dict(files=files,expanded_bytes=expanded,roots=[str(p) for p in selected])


def acquire_archive(root, row):
    receipt=root/'receipts'/(row['id']+'.json');result=dict(source=row,status='FAILED')
    archive=root/'archives'/(row['id']+'.zip');folder=root/'sources'/row['id']
    try:
        url=urlparse(row['url'])
        if (url.scheme!='https' or url.hostname!='x2-aimdk.agibot.com' or url.username or url.password or url.query or url.fragment or
                not url.path.startswith('/downloads/') or not url.path.endswith('.zip')):raise ValueError('unreviewed vendor archive transport')
        if not 0<row['archive_bytes']<=1<<30:raise ValueError('vendor archive byte limit exceeded')
        if not archive.exists() or archive.stat().st_size!=row['archive_bytes'] or sha(archive)!=row['archive_sha256']:
            result['archive']=fetch(row['url'],archive,row['archive_bytes'])
        if archive.stat().st_size!=row['archive_bytes'] or sha(archive)!=row['archive_sha256']:raise ValueError('vendor archive identity mismatch')
        result['archive']=dict(url=row['url'],sha256=sha(archive),bytes=archive.stat().st_size)
        if folder.exists():shutil.rmtree(folder)
        folder.mkdir(parents=True)
        result['extraction']=extract(archive,folder,row['package_roots'])
        review=audit(folder);save(root/'scans'/(row['id']+'.json'),review)
        result.update(status='ACQUIRED_AND_SCANNED',source_directory=str(folder),packages=review['packages'],
            categories=review['categories'],source_files_scanned=review['source_files_scanned'],semantic_review='PENDING',
            selected_definitions_only=True,native_libraries_extracted=False,
            files={str(p.relative_to(folder)):sha(p) for p in folder.rglob('*') if p.is_file()})
    except Exception as error:result['error']=type(error).__name__+': '+str(error)
    save(receipt,result);return result
