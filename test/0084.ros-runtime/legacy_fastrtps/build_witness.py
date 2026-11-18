"""Build the String witness using the qualified Jazzy client toolchain and sysroot."""
import argparse
import json
from pathlib import Path
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from run_scene_vm import sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--jazzy-receipt', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    original = json.loads(args.jazzy_receipt.read_bytes())
    step = next(s for s in original['steps'] if s['name'] == 'build-ros-client')
    if not original['complete'] or step['exit'] or original['request_peer']['distribution'] != 'jazzy':
        raise ValueError('expected a completed fixture with a Jazzy requester')
    argv = list(step['argv'])
    source = Path(__file__).with_name('jazzy_string.cpp').resolve()
    position, = [i for i, arg in enumerate(argv) if Path(arg).name == 'ros_client.cpp']
    if argv[-2] != '-o' or '-g' not in argv or '-O3' not in argv:
        raise ValueError('unexpected compiler arguments')
    args.output.mkdir(mode=0o700, parents=True, exist_ok=False)
    exe = (args.output / 'jazzy-string').resolve()
    argv[position] = str(source)
    argv[-1] = str(exe)
    with (args.output / 'build.log').open('wb') as log:
        build = subprocess.run(argv, stdout=log, stderr=subprocess.STDOUT)
    report = {'exit': build.returncode, 'argv': argv, 'sourceSha256': sha(source),
              'witnessSha256': sha(exe) if exe.is_file() else None, 'baselineSha256': sha(args.jazzy_receipt)}
    (args.output / 'witness-build.json').write_text(json.dumps(report, indent=2) + '\n')
    return build.returncode


if __name__ == '__main__':
    sys.exit(main())
