"""Prepare real rosbuild-era ROS 1 stacks without preserving entire Debian archives."""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tarfile
import time
from urllib.request import urlopen

from historical_packages import SNAPSHOT_KEY, index, sha
from run import gate

SUITES = {'fuerte':'precise','electric':'oneiric','diamondback':'maverick','cturtle':'lucid','boxturtle':'lucid'}
STACKS = ('ros','ros-comm','std-msgs','genpy','genmsg','roscpp-core','rospack','common-msgs')
PACKAGES = {'rospy','roscpp','rosgraph','rosmaster','roslib','roslang','rosgraph_msgs','std_msgs','genpy','genmsg',
            'rosconsole','rosbuild','catkin','rospack','rosunit','std_srvs','rostime','rosparam','roslaunch','rostopic'}


def wanted(path):
    parts=path.parts
    if not parts or parts[0]!='opt' or any(name in ('test','tests','testing','tutorials','.svn','.git') for name in parts):
        return False
    name=path.name
    if name in ('stack.xml','manifest.xml','package.xml'):
        return True
    if name in ('rosmaster','rospack','rospack_nosubdirs','python_logging.conf','rosconsole.config','rosdep.yaml'):
        return True
    if name.startswith(('librospack.so','libroslib.so','librosstack.so','libtinyxml.so')):
        return True
    if not any(part in PACKAGES for part in parts):
        return False
    return path.suffix in ('.py','.msg','.srv') or name in ('env.sh','roscore.xml')


def archive(row,base):
    size=int(row['Size'])
    relative=row['Filename']
    if not 0<size<=80*1024*1024 or not relative.startswith('pool/') or '..' in relative.split('/'):
        raise ValueError('unexpected signed package extent or path')
    url=base+relative
    for attempt in range(3):
        fd=os.memfd_create('rosbuild-signed-archive',os.MFD_CLOEXEC)
        try:
            count=0
            h=hashlib.sha256()
            with urlopen(url,timeout=25) as response:
                while count<size:
                    data=response.read(min(size-count,256*1024))
                    if not data:
                        raise EOFError('short archive '+url)
                    h.update(data)
                    if os.write(fd,data)!=len(data):
                        raise OSError('short memory-file write')
                    count+=len(data)
                if response.read(1):
                    raise ValueError('archive extent exceeded')
            if h.hexdigest()!=row['SHA256']:
                raise ValueError('archive digest mismatch')
            os.lseek(fd,0,os.SEEK_SET)
            return fd
        except (OSError,EOFError):
            os.close(fd)
            if attempt==2:
                raise
            time.sleep(attempt+.25)
        except BaseException:
            os.close(fd)
            raise


def extract(row,base,root,remaining):
    fd=archive(row,base)
    files={}
    names=[]
    links={}
    try:
        tar=subprocess.Popen(['dpkg-deb','--fsys-tarfile','/proc/self/fd/'+str(fd)],
                             pass_fds=(fd,),stdout=subprocess.PIPE)
        try:
            with tarfile.open(fileobj=tar.stdout,mode='r|') as entries:
                for item in entries:
                    path=Path(item.name.removeprefix('./'))
                    if path.is_absolute() or '..' in path.parts:
                        raise ValueError('unexpected archive path')
                    if item.isfile() or item.issym() or item.islnk():
                        names.append(str(path))
                    if not wanted(path):
                        continue
                    if item.issym() or item.islnk():
                        links[str(path)] = item.linkname
                        continue
                    if not item.isfile():
                        continue
                    if item.size>16*1024*1024 or item.size>remaining[0]:
                        raise ValueError('selected runtime exceeds cap')
                    data=entries.extractfile(item).read()
                    dest=root/path
                    if dest.exists() and dest.read_bytes()!=data:
                        raise ValueError('package collision: '+str(path))
                    if not dest.exists():
                        dest.parent.mkdir(parents=True,exist_ok=True)
                        dest.write_bytes(data)
                        dest.chmod(0o755 if item.mode&0o111 else 0o644)
                        remaining[0]-=len(data)
                    files[str(path)]=sha(data)
            if tar.wait(timeout=10):
                raise ValueError('Debian tar extraction failed')
        finally:
            tar.stdout.close()
            if tar.poll() is None:
                tar.kill()
                tar.wait()
    finally:
        os.close(fd)
    # Archive links are recorded for inspection. External references are never
    # installed, and Python source files are selected directly.
    indexpath=root.parent/(row['Package']+'.paths.gz')
    indexpath.write_bytes(gzip.compress(('\n'.join(names)+'\n').encode(),mtime=0))
    return {'package':row['Package'],'version':row['Version'],'size':int(row['Size']),'url':base+row['Filename'],
            'sha256':row['SHA256'],'files':files,'archiveLinks':links,'pathsSHA256':sha(indexpath.read_bytes())}


def acquire(args):
    out=args.output
    out.mkdir(parents=True)
    remaining=[args.max_output_mib*1024*1024]
    result={'complete':False,'sources':{},'packages':{}}
    try:
        for distro in args.distro:
            root=out/distro
            root.mkdir()
            base='http://snapshots.ros.org/'+distro+'/final/ubuntu/'
            records,rows=index(base,SUITES[distro],args.ros_keyring,SNAPSHOT_KEY,'main','gz',out)
            result['sources'][distro]=records
            result['packages'][distro]=[]
            for name in STACKS:
                key='ros-'+distro+'-'+name
                if key in rows:
                    result['packages'][distro].append(extract(rows[key],base,root,remaining))
            if not any('rospy' in path.split('/') for package in result['packages'][distro] for path in package['files']):
                raise ValueError('missing original rospy')
            print(distro, args.max_output_mib*1024*1024-remaining[0],flush=True)
        result['complete']=True
    finally:
        (out/'stacks.json').write_text(json.dumps(result,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--ros-keyring',required=True,type=Path)
    p.add_argument('--distro',required=True,choices=SUITES,action='append')
    p.add_argument('--output',required=True,type=Path)
    p.add_argument('--max-output-mib',default=24,type=int)
    args=p.parse_args()
    gate()
    if '.tmp' not in args.output.resolve().parts or not 1<=args.max_output_mib<=64:
        p.error('use .tmp output and an explicit bounded runtime footprint')
    acquire(args)
