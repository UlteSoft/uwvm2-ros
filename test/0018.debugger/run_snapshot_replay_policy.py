#!/usr/bin/env python3
"""Actual VM rejection of execution permissions and dump disclosure limits."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import time
from run_dump_launch_policy import VM, require


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--binary',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    p.add_argument('--runner-json',type=Path)
    args=p.parse_args();args.out=args.out.resolve();args.out.mkdir(parents=True,exist_ok=False)
    require('/docker-' in Path('/proc/self/cgroup').read_text(),'resource cgroup required')
    prefix=json.loads(args.runner_json.read_text()) if args.runner_json else []
    cases=[('read-only',['--debug-permissions','read-only','--runtime-disable-memtag'],False),
           ('limited',['--debug-permissions','limited','--runtime-disable-memtag'],False),
           ('disabled',['--debug-dump-force','none','--runtime-disable-memtag'],False),
           ('ceiling',['--debug-dump-max','diagnostic','--runtime-disable-memtag'],False),
           ('forced',['--debug-dump-force','diagnostic','--runtime-disable-memtag'],False)]
    rows=[]
    for name,flags,shared in cases:
        folder=args.out/name;folder.mkdir();wat=folder/'program.wat';wasm=wat.with_suffix('.wasm')
        wat.write_text('(module (memory 1'+(' 1 shared' if shared else '')+') (func (export "_start") (loop br 0)))')
        subprocess.run(['/usr/bin/wat2wasm','--enable-threads',str(wat),'-o',str(wasm)],check=True)
        vm=None;row={'case':name,'passed':False}
        try:
            vm=VM([str(args.binary.resolve()),'-Raot','-Rdbg','-Rct','0','-Rllvm-cache-path','disable',*flags,'--run',str(wasm)],folder,prefix)
            if name not in ('read-only','limited'):
                require(not vm.request('breakwasm 0 0 0').startswith('error:'),'loop breakpoint')
                vm.request('continue')
            deadline=time.monotonic()+120
            while True:
                state=vm.request('status');matched=re.search(r'stop-id ([1-9][0-9]*)',state)
                if name in ('read-only','limited') or matched and re.search(r'^thread [1-9]',state,re.M):break
                require(time.monotonic()<deadline,state);time.sleep(.01)
            stop=matched[1] if matched else '1'
            refused=vm.request('dump record start '+stop)
            require(refused.startswith('error:'),refused)
            after=vm.request('status');require(after==state,'rejection changed the execution state')
            output=folder/'forbidden.uwsnap'
            refused_save=vm.request(f'dump record save {stop} {output}')
            require(refused_save.startswith('error:') and not output.exists(),'failed recording exported a journal')
            row.update(passed=True,start=refused,save=refused_save)
        except Exception as error:row['error']=repr(error)
        finally:
            if vm:row['commands']=vm.commands;vm.close()
            rows.append(row);(args.out/'results.json').write_text(json.dumps(rows,indent=2))
        print(name,row['passed'],row.get('error',''),flush=True)
    require(all(x['passed'] for x in rows),'record policy failures; inspect results.json')

if __name__=='__main__':main()
