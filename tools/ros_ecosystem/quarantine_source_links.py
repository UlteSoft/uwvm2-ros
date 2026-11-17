#!/usr/bin/env python3
"""Recover regular source files from pinned archives without materializing links.

External and internal link targets remain inert JSON evidence. Their referents
are unresolved: this is not a complete upstream working tree or a build result.
No executable mode, device node, symlink or hard link is created.
"""
import argparse
import json
from pathlib import Path, PurePosixPath
import tarfile

from acquire import audit, save
from capability_inventory import ROOT, guard, sha
from platform_archive import relative


def extract(archive, destination):
    # A fresh empty root is mandatory. Never walk a pre-existing symlink or
    # merge an untrusted archive with an earlier, partially extracted tree.
    if destination.is_symlink() or not destination.is_dir() or any(destination.iterdir()):
        raise ValueError('source destination must be an empty ordinary directory')
    links=[];names=set();expanded=0;count=0
    with tarfile.open(archive,'r:gz') as source:
        for item in source:
            count+=1;path=relative(item.name)
            if count>1000000:
                raise ValueError('unsafe or excessive source members')
            if path in names:raise ValueError('duplicate source member')
            names.add(path)
            if item.issym() or item.islnk():
                links.append(dict(path=str(path),target=item.linkname,kind='symlink' if item.issym() else 'hardlink',
                                  materialized=False,referent_resolved=False));continue
            target=destination/str(path)
            if item.isdir():target.mkdir(parents=True,exist_ok=True);continue
            if not item.isfile():raise ValueError('special source member')
            expanded+=item.size
            if item.size<0 or expanded>8<<30:raise ValueError('source expansion limit exceeded')
            target.parent.mkdir(parents=True,exist_ok=True);incoming=source.extractfile(item)
            if incoming is None:raise ValueError('missing regular file body')
            size=0
            with incoming,target.open('xb') as output:
                while data:=incoming.read(65536):
                    size+=len(data)
                    if size>item.size:raise ValueError('source exceeds declared size')
                    output.write(data)
            if size!=item.size:raise ValueError('source body truncated')
            target.chmod(0o644)
    return dict(members=count,expanded_bytes=expanded,quarantined_links=links)


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--previous',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True);args=parser.parse_args();guard()
    previous=args.previous.resolve();out=args.output.resolve()
    assert previous.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve()) and out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True,exist_ok=False);results=[]
    for row in json.loads((previous/'results.json').read_text()):
        if not row.get('error','').startswith('AbsoluteLinkError:'):continue
        identity=row['source']['id'];assert len(identity)==24 and all(c in '0123456789abcdef' for c in identity)
        archive=previous/'archives'/(identity+'.tar.gz');assert archive.is_file() and sha(archive)==row['archive']['sha256']
        folder=out/'sources'/identity;folder.mkdir(parents=True)
        metadata=extract(archive,folder);review=audit(folder)
        save(out/'scans'/(identity+'.json'),review)
        result=dict(source=row['source'],archive=dict(path=str(archive),sha256=sha(archive)),source_directory=str(folder),
            status='REGULAR_FILES_ACQUIRED_LINKS_QUARANTINED',semantic_review='PENDING',native_qualification='NOT_TESTED',
            complete_upstream_working_tree=False,regular_files_scanned=review['source_files_scanned'],packages=review['packages'],**metadata)
        save(out/'receipts'/(identity+'.json'),result);results.append(result)
        print(identity,len(review['packages']),'packages;',len(metadata['quarantined_links']),'inert links',flush=True)
    save(out/'results.json',dict(complete=True,scope=__doc__,sources=results,runner_sha256=sha(__file__),
        previous_results_sha256=sha(previous/'results.json'),upstream_code_executed=False))


if __name__=='__main__':main()
