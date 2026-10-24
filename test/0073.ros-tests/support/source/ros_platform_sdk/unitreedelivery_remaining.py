#!/usr/bin/env python3
"""Check the finite ordinary Unitree source stream before client delivery.

This runs only our inert file-transport code. Transported original/sdk programs
are not imported, compiled or executed. A changed existing destination must be
refused before any new source file is installed.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import sys


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    a=p.parse_args();root,out=a.root.resolve(strict=True),a.out
    inputs=json.loads((root/'inputs.json').read_bytes());source=Path(inputs['source_root'])
    assert out.parent==root/'out'and not out.exists();out.mkdir(mode=0o700)
    report=dict(status='RUNNING',commands=[],default_authority=[],transported_code_executed=False)
    def save():(out/'unitreedelivery.json').write_text(json.dumps(report,indent=2)+'\n')
    try:
        tool=source/'test/0073.ros-tests/support/source/ros_platform_sdk/io_remaining_delivery.py'
        clone=out/'fresh-source';clone.mkdir()
        def transfer(label,expected):
            export_argv=[sys.executable,'-I','-S','-B',tool,'export','--unitree-only','--source',source,'--receipt',out/(label+'-export.json')]
            receive_argv=[sys.executable,'-I','-S','-B',tool,'receive','--source',clone,'--receipt',out/(label+'-receive.json')]
            with(out/(label+'-export.stderr')).open('xb')as export_error,(out/(label+'-receive.stdout')).open('xb')as stdout,(out/(label+'-receive.stderr')).open('xb')as stderr:
                export=subprocess.Popen(list(map(str,export_argv)),stdout=subprocess.PIPE,stderr=export_error)
                try:
                    receive=subprocess.run(list(map(str,receive_argv)),stdin=export.stdout,stdout=stdout,stderr=stderr,timeout=120,check=False)
                    export.stdout.close();exit_code=export.wait(timeout=120)
                finally:
                    if export.poll()is None:export.kill();export.wait()
            report['commands'] += [dict(label=label+'-export',argv=list(map(str,export_argv)),exit=exit_code),
                                   dict(label=label+'-receive',argv=list(map(str,receive_argv)),exit=receive.returncode)]
            save()
            if expected==0:
                assert exit_code==receive.returncode==0,(out/(label+'-receive.stderr')).read_text()[-1800:]
                sent,received=(json.loads((out/(label+'-'+name+'.json')).read_bytes())for name in('export','receive'))
                assert sent==received
                return sent
            assert receive.returncode!=0
            assert 'AssertionError'in(out/(label+'-receive.stderr')).read_text()
        sent=transfer('fresh',0)
        assert sent==transfer('identical',0)
        relative='sdk/origin/robots/unitree/SPORT.md'
        (clone/relative).write_text('independent user edit\n')
        absent=clone/'sdk/origin/robots/unitree/sport.cpp'
        assert absent.is_file();absent.unlink()
        transfer('foreign',1)
        assert(clone/relative).read_text()=='independent user edit\n'and not absent.exists()
        # Only a private byte-copy fixture is retired after successful checks.
        shutil.rmtree(clone)
        report.update(status='UNITREE_INERT_SOURCE_FRESH_IDENTICAL_AND_FOREIGN_REFUSAL_PASS',
            files=len(sent['files']),ordinary_bytes=sent['logical_bytes'],private_fixture_retired=True)
        print(report['status'],flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED',error=repr(error));raise
    finally:save()


if __name__=='__main__':main()
