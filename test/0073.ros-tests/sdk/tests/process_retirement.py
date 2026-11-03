"""Real Linux orphan/setsid/stopped-child regressions; no simulation qualification.

Run inside the campaign's admitted shared cgroup. Each supervisor is childless
before installing its subreaper, and a sibling sentinel must survive cleanup.
"""
from pathlib import Path
import json
import os
import signal
import subprocess
import sys
import time


def main():
    mode, sdk, output = sys.argv[1:4]
    output = Path(output)
    output.mkdir(parents=True, exist_ok=True)
    assert '/uwvmros.slice/' in Path('/proc/self/cgroup').read_text()
    if mode == 'actor':
        kind = sys.argv[4]
        sys.stdin.buffer.read(1)
        if kind == 'stopped':
            os.kill(os.getpid(), signal.SIGSTOP)
            return
        child = os.fork()
        if child:
            while not (output / 'orphan.json').exists():
                time.sleep(0.01)
            os._exit(0)
        os.setsid()
        if kind == 'double-fork':
            child = os.fork()
            if child:
                os._exit(0)
            os.setsid()
        signal.signal(signal.SIGTERM, signal.SIG_IGN)
        (output / 'orphan.json').write_text(json.dumps({'pid': os.getpid(), 'birth': Path('/proc/self/stat').read_text().rsplit(') ', 1)[1].split()[19]}))
        while True:
            time.sleep(1)
    if mode == 'example-supervisor':
        sys.path.insert(0, str(Path(sdk) / 'tools/shared/python'))
        from example_process import child_command, digest
        from owned_processes import process
        # The fixture is only an adversarial lifecycle subject, never a build
        # or simulator PASS. Actual Xmake selection is tested separately.
        subject = output / 'early-exit-subject'
        subject.write_text('#!' + sys.executable + '\n'
            'import os,signal,time,json\nfrom pathlib import Path\n'
            'if os.fork(): os._exit(0)\n'
            'os.setsid()\n'
            'if os.fork(): os._exit(0)\n'
            'os.setsid()\nsignal.signal(signal.SIGTERM,signal.SIG_IGN)\n'
            'p=Path(os.environ["RETIREMENT_FIXTURE_OUTPUT"])/"orphan.json"\n'
            'p.write_text(json.dumps({"pid":os.getpid(),"birth":Path("/proc/self/stat").read_text().rsplit(") ",1)[1].split()[19]}))\n'
            'while True: time.sleep(1)\n')
        subject.chmod(0o700)
        env = dict(os.environ, PWD=str(output), RETIREMENT_FIXTURE_OUTPUT=str(output))
        request = {'xmake': str(subject), 'xmake_sha256': digest(subject),
                   'controller_cgroup': Path('/proc/self/cgroup').read_text().strip(), 'timeout_seconds': 5}
        row = child_command(request, 'build', output, output, env, None)
        assert row['status'] == 'BLOCKED', 'Fixture must not qualify as actual Xmake'
        assert row['diagnostic'] == 'BLOCKED_ACTUAL_CHILD_XMAKE_EXEC_IDENTITY_NOT_OBSERVED'
        receipt = row['retirement']
        assert receipt['status'] == 'RETIRED' and receipt['subreaper_restored'], receipt
        assert not receipt['same_birth_remaining'] and not receipt['uncollected_direct_children']
        for owned in receipt['processes']:
            live = process(owned['pid'])
            assert live is None or live['birth'] != owned['birth']
        (output / 'retirement.json').write_text(json.dumps(row, indent=2)+'\n')
        print('PASS example_process adopted descendant retirement; build qualification correctly BLOCKED')
        return
    if mode == 'supervisor':
        sys.path.insert(0, str(Path(sdk) / 'tools/shared/python'))
        from owned_processes import OwnedProcesses, process
        from simulation_example import Context
        context = Context.__new__(Context)
        context.lifecycle = OwnedProcesses()
        with (output / 'actor.stdout').open('w') as out, (output / 'actor.stderr').open('w') as err:
            child = subprocess.Popen([sys.executable, '-B', __file__, 'actor', sdk, str(output), sys.argv[4]], stdin=subprocess.PIPE, stdout=out, stderr=err)
            birth = process(child.pid)['birth']
            context.actors = [{'pid': child.pid, 'birth': birth, 'process': child}]
            child.stdin.write(b'1'); child.stdin.flush(); child.stdin.close()
            if sys.argv[4] != 'stopped':
                assert child.wait(timeout=5) == 0  # Leader gone before first census.
            else:
                deadline = time.monotonic() + 5
                while process(child.pid)['state'] != 'T':
                    assert time.monotonic() < deadline
                    time.sleep(0.01)
            context.retire()
            row = context.retirement
            assert row['status'] == 'RETIRED' and not row['same_birth_remaining'] and not row['uncollected_direct_children'], row
            assert row['subreaper_restored']
            if (output / 'orphan.json').exists():
                orphan = json.loads((output / 'orphan.json').read_text())
                current = process(orphan['pid'])
                assert current is None or current['birth'] != orphan['birth']
            (output / 'retirement.json').write_text(json.dumps(row, indent=2)+'\n')
        print('RETIRED', sys.argv[4])
        return
    assert mode == 'suite'
    sentinel = subprocess.Popen(['/bin/sleep', '60'])
    try:
        for kind in ('early-exit', 'double-fork', 'stopped'):
            subprocess.run([sys.executable, '-B', __file__, 'supervisor', sdk, str(output / kind), kind], check=True, timeout=20)
            assert sentinel.poll() is None, 'Unrelated sibling sentinel was killed'
        subprocess.run([sys.executable, '-B', __file__, 'example-supervisor', sdk, str(output / 'example-process')], check=True, timeout=20)
        assert sentinel.poll() is None, 'Unrelated sibling sentinel was killed'
        print(json.dumps({'status': 'PASS', 'cases': ['early-exit', 'double-fork-setsid', 'stopped-gate'], 'unrelated_sentinel_alive': True, 'sentinel_pid': sentinel.pid}))
    finally:
        sentinel.terminate(); sentinel.wait(timeout=5)


if __name__ == '__main__':
    main()
