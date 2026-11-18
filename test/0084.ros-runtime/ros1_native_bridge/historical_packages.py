"""Acquire signed historical ROS 1 Python packages for the isolated bridge tests."""
import argparse
import gzip
import hashlib
import json
import lzma
import os
from pathlib import Path
import re
import subprocess
import tarfile
import tempfile
import time
from urllib.request import urlopen

from run import gate

SUITES = {'groovy':'precise','hydro':'precise','indigo':'trusty','jade':'trusty',
          'lunar':'xenial','kinetic':'xenial','melodic':'bionic'}
SNAPSHOT_KEY = '4B63CF8FDE49746E98FA01DDAD19BAB3CBF125EA'
PYTHON_PACKAGES = ('python2.7-minimal','libpython2.7-minimal','libpython2.7-stdlib',
                   'python-yaml','python-six','python-pkg-resources','python-dateutil',
                   'python-netifaces','python-defusedxml','python-docutils','python-pyparsing')
ROS_PACKAGES = ('rospy','roscpp','catkin','rosgraph','rosgraph-msgs','rosmaster','genpy','genmsg','roslib','std-msgs')
COMMON_PACKAGES = ('python-rospkg','python-rospkg-modules','python-catkin-pkg','python-catkin-pkg-modules')


def sha(data):
    return hashlib.sha256(data).hexdigest()


def get(url, limit):
    if limit > 32*1024*1024:
        raise ValueError('archive size exceeds the fixture ceiling')
    for attempt in range(3):
        try:
            with urlopen(url,timeout=20) as response:
                data=response.read(limit+1)
            break
        except (OSError,EOFError) as failure:
            if attempt == 2:
                raise RuntimeError('retrieval failed: '+url) from failure
            time.sleep(.25*(attempt+1))
    if len(data)>limit:
        raise ValueError('archive response too large')
    return data


def index(base,suite,keyring,signer,component,extension,temp):
    path='dists/'+suite+'/'
    signed=get(base+path+'InRelease',512*1024)
    release=temp/('release-'+suite+'-'+component+'-'+sha(signed)[:16])
    release.write_bytes(signed)
    signature=subprocess.run(['gpgv','--status-fd','2','--keyring',str(keyring),'--output','-',release],
                             capture_output=True,check=True)
    match=re.search(r'^\[GNUPG:\] VALIDSIG ([A-F0-9]{40}) ',signature.stderr.decode(),re.MULTILINE)
    if not match or (signer and match[1]!=signer):
        raise ValueError('unexpected archive signer')
    content=signature.stdout.decode()
    metadata=dict(line.split(': ',1) for line in content.splitlines() if line and line[0]!=' ' and ': ' in line)
    if metadata.get('Codename')!=suite:
        raise ValueError('unexpected archive codename')
    if signer:
        label=base.split('snapshots.ros.org/',1)[1].rstrip('/')+' '+suite
        if metadata.get('Label')!=label:
            raise ValueError('unexpected historical ROS archive')
    elif metadata.get('Origin')!='Ubuntu':
        raise ValueError('unexpected Ubuntu archive')
    entry=component+'/binary-amd64/Packages.'+extension
    section=content.split('SHA256:\n',1)[1].split('\nSHA',1)[0]
    rows=[line.split() for line in section.splitlines() if len(line.split())==3 and line.split()[-1]==entry]
    if len(rows)!=1:
        raise ValueError('missing signed Packages hash')
    digest,size,_=rows[0]
    data=get(base+path+entry,int(size))
    if len(data)!=int(size) or sha(data)!=digest:
        raise ValueError('index digest mismatch')
    text=(gzip.decompress(data) if extension=='gz' else lzma.decompress(data)).decode()
    packages={}
    for paragraph in text.split('\n\n'):
        row=dict(line.split(': ',1) for line in paragraph.splitlines() if line and line[0]!=' ' and ': ' in line)
        if 'Package' in row and 'Filename' in row:
            packages[row['Package']]=row
    return {'base':base,'suite':suite,'index':entry,'signatureFingerprint':match[1],
            'releaseSHA256':sha(signed),'indexSHA256':digest},packages


def extract(base,row,root):
    name=row['Filename']
    if not name.startswith('pool/') or '..' in name.split('/'):
        raise ValueError('unexpected pool location')
    data=get(base+name,int(row['Size']))
    if len(data)!=int(row['Size']) or sha(data)!=row['SHA256']:
        raise ValueError('binary package digest mismatch')
    # Extract only regular files needed by the historical Python processes. No
    # maintainer scripts, device files, archive links or native ROS binaries run.
    selected={}
    with tempfile.TemporaryFile(dir=root.parent) as archive:
        archive.write(data)
        archive.seek(0)
        tar=subprocess.Popen(['dpkg-deb','--fsys-tarfile','/dev/stdin'],stdin=archive,stdout=subprocess.PIPE)
        try:
            with tarfile.open(fileobj=tar.stdout,mode='r|') as entries:
                for item in entries:
                    path=item.name.removeprefix('./')
                    wanted=(path=='usr/bin/python2.7' or path.startswith(('usr/lib/python2.7/','usr/lib/python2/')) or
                            (path.startswith('opt/ros/') and ('/lib/python2.7/' in path or path.endswith('/package.xml')
                            or path.endswith('/bin/rosmaster') or '/share/rosgraph/conf/' in path)))
                    if not wanted or not item.isfile():
                        continue
                    if path.startswith('/') or '..' in path.split('/') or item.size>16*1024*1024:
                        raise ValueError('unexpected extraction path or extent')
                    dst=root/path
                    dst.parent.mkdir(parents=True,exist_ok=True)
                    payload=entries.extractfile(item).read()
                    if dst.exists() and dst.read_bytes()!=payload:
                        raise ValueError('conflicting package content')
                    dst.write_bytes(payload)
                    dst.chmod(0o755 if item.mode&0o111 else 0o644)
                    selected[path]=sha(payload)
            if tar.wait(timeout=12):
                raise ValueError('dpkg-deb extraction failed')
        finally:
            tar.stdout.close()
            if tar.poll() is None:
                tar.kill()
                tar.wait()
    return {'package':row['Package'],'version':row['Version'],'size':len(data),'url':base+name,
            'sha256':row['SHA256'],'selectedFiles':len(selected),'files':selected}


def acquire(args):
    output=args.output
    output.mkdir(parents=True,exist_ok=args.resume)
    old={}
    receipt=output/'packages.json'
    if args.resume:
        previous=json.loads(receipt.read_bytes())
        if previous['complete']:
            raise ValueError('the package receipt is already complete')
        (output/('failed-'+sha(receipt.read_bytes())[:16]+'.json')).write_bytes(receipt.read_bytes())
        for tier,rows in previous['packages'].items():
            for package in rows:
                if 'files' in package:
                    old[tier,package['package']]=package
    manifest={'complete':False,'sources':{},'packages':{}}
    try:
        if args.python_baseline:
            previous=json.loads((args.python_baseline/'packages.json').read_bytes())
            if not previous['complete']:
                raise ValueError('Python runtime baseline incomplete')
            manifest['packages']['python2']=previous['packages']['python2']
            manifest['sources'].update({key:value for key,value in previous['sources'].items() if key.startswith('ubuntu')})
            for package in manifest['packages']['python2']:
                for relative,expected in package['files'].items():
                    path=Path(relative)
                    if path.is_absolute() or '..' in path.parts:
                        raise ValueError('invalid archived file path')
                    src=args.python_baseline/'python2'/path
                    dst=output/'python2'/path
                    if sha(src.read_bytes())!=expected:
                        raise ValueError('historical Python runtime file changed')
                    dst.parent.mkdir(parents=True,exist_ok=True)
                    if not dst.exists():
                        os.link(src,dst)
            tiers=[]
        else:
            base='https://archive.ubuntu.com/ubuntu/'
            records,rows=index(base,'bionic',args.ubuntu_keyring,None,'main','xz',output)
            manifest['sources']['ubuntu']=records
            need=set(PYTHON_PACKAGES)-rows.keys()
            if need:
                records,other=index(base,'bionic',args.ubuntu_keyring,None,'universe','xz',output)
                manifest['sources']['ubuntu-universe']=records
                rows.update({p:other[p] for p in need})
            tiers=[('python2',base,[rows[p] for p in PYTHON_PACKAGES])]
        for distro in args.distro:
            base='http://snapshots.ros.org/'+distro+'/final/ubuntu/'
            records,rows=index(base,SUITES[distro],args.ros_keyring,SNAPSHOT_KEY,'main','gz',output)
            manifest['sources'][distro]=records
            selected=[rows['ros-'+distro+'-'+p] for p in ROS_PACKAGES]
            selected.extend(rows[p] for p in COMMON_PACKAGES if p in rows)
            tiers.append((distro,base,selected))
        for tier,base,packages in tiers:
            root=output/tier
            root.mkdir(exist_ok=args.resume)
            # Extraction is serialized to check duplicate paths deterministically.
            manifest['packages'][tier]=[]
            for row in packages:
                previous=old.get((tier,row['Package']))
                if (previous and previous['sha256']==row['SHA256'] and previous['size']==int(row['Size'])
                        and all(not Path(path).is_absolute() and '..' not in Path(path).parts and (root/path).is_file()
                                and sha((root/path).read_bytes())==expected for path,expected in previous['files'].items())):
                    manifest['packages'][tier].append(previous)
                else:
                    manifest['packages'][tier].append(extract(base,row,root))
            print(tier, len(packages),flush=True)
        manifest['complete']=True
    finally:
        (output/'packages.json').write_text(json.dumps(manifest,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--resume',action='store_true',help='Recheck signed indices and resume a partial local extraction.')
    p.add_argument('--ros-keyring',required=True,type=Path)
    p.add_argument('--ubuntu-keyring',default='/usr/share/keyrings/ubuntu-archive-keyring.gpg',type=Path)
    p.add_argument('--distro',action='append',choices=SUITES,required=True)
    p.add_argument('--output',required=True,type=Path)
    p.add_argument('--python-baseline',type=Path,help='Hard-link identical verified Python files from a prior qualification under the same filesystem.')
    args=p.parse_args()
    gate()
    if '.tmp' not in args.output.resolve().parts:
        p.error('use a fresh directory in project .tmp')
    acquire(args)
