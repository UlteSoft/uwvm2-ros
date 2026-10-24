#!/usr/bin/env python3
"""Release one genuine stopped pre-RIHS exporter after recording its mappings.

The finite outer owner admits this process in the original Linux cgroup. This
driver adds no Guest capability, ROS entity or physical network interface. The
exporter must finish its actual typed/introspection checks before SIGSTOP; a
digest or generated document cannot substitute for that native execution.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import signal
import subprocess
import time


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--exporter',type=Path,required=True)
    parser.add_argument('--install',type=Path,required=True)
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args();args.out.mkdir()
    helper=Path(__file__).with_name('unitree_native_api_session.py')
    spec=importlib.util.spec_from_file_location('unitree_native_mapping_reader',helper)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    artifact=module.fact(args.exporter)
    report=dict(status='RUNNING',artifact=artifact,default_authority=[],ROS_entities_created=0)
    def save():(args.out/'owner.json').write_text(json.dumps(report,indent=2)+'\n')
    child=None
    try:
        namespace=os.readlink('/proc/self/ns/net')
        interfaces=[line.split(':',1)[0].strip()for line in Path('/proc/net/dev').read_text().splitlines()[2:]]
        assert interfaces==['lo']
        membership=Path('/proc/self/cgroup').read_text()
        env=dict(os.environ,LD_LIBRARY_PATH=str(args.install/'lib')+
            ':/opt/ros/humble/lib:/opt/ros/humble/lib/x86_64-linux-gnu')
        env.pop('LD_PRELOAD',None)
        with(args.out/'metadata.json').open('xb')as stdout,(args.out/'stderr').open('xb')as stderr:
            child=subprocess.Popen([str(args.exporter),'--mapped-stop'],env=env,
                stdin=subprocess.PIPE,stdout=stdout,stderr=stderr,close_fds=True)
            report['pid']=child.pid;save();deadline=time.monotonic()+90
            while True:
                assert time.monotonic()<deadline,'native source exporter stop deadline'
                pid,status=os.waitpid(child.pid,os.WUNTRACED|os.WNOHANG)
                if pid:
                    assert os.WIFSTOPPED(status)and os.WSTOPSIG(status)==signal.SIGSTOP,('unexpected exporter exit',status)
                    break
                time.sleep(.02)
            proc=Path('/proc')/str(child.pid)
            assert(proc/'cgroup').read_text()==membership
            assert os.readlink(proc/'ns/net')==namespace
            report.update(stopped_stat=(proc/'stat').read_text(),cgroup=membership,netns=namespace,
                interfaces=interfaces,actual_loaded_files=module.mapped(child.pid))
            assert report['actual_loaded_files']['files'][str(args.exporter)]['sha256']==artifact['sha256']
            save();child.stdin.write(b'I');child.stdin.flush();child.stdin.close()
            os.kill(child.pid,signal.SIGCONT)
            assert child.wait(timeout=90)==0
        assert module.fact(args.exporter)==artifact
        for path,pin in report['actual_loaded_files']['files'].items():
            assert module.fact(path)=={k:pin[k]for k in('bytes','sha256')}
            stat=Path(path).stat();assert(stat.st_dev,stat.st_ino)==(pin['device'],pin['inode'])
        raw=(args.out/'metadata.json').read_bytes();assert 0<len(raw)<=16<<20
        value=json.loads(raw);assert len(value['types'])==8
        assert value['format']=='uwvm-ros-industrial-native-pre-rihs-source-v1'
        assert not(args.out/'stderr').stat().st_size
        report.update(status='GENUINE_STOPPED_UNITREE_INSTALLED_SOURCE_EXPORT_PASS',
            exported_messages=8,metadata=module.fact(args.out/'metadata.json'),exit=child.returncode)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:
        if child is not None and child.poll()is None:
            child.kill();child.wait(timeout=5)
        if child is not None and not child.stdin.closed:child.stdin.close()
        save()
    print(report['status'],flush=True)


if __name__=='__main__':main()
