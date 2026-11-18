"""Prepare an individually pinned vendor simulation scene with the official MuJoCo compiler."""
import json
from pathlib import Path, PurePosixPath
import re
import tempfile

from run_scene_vm import sha


def prepare(profile_path, source, mujoco, output, platforms, run, maximum_bytes=64 << 20):
    if not (1 << 20) <= maximum_bytes <= (512 << 20):
        raise ValueError('invalid compiled simulation model budget')
    profile = json.loads(profile_path.read_bytes())
    platform = profile['platform']
    inputs = profile['files']
    if (platform not in platforms or not re.fullmatch('[0-9a-f]{40}',profile['commit'])
            or not 1 <= len(inputs) <= 1024 or profile['model'] not in inputs):
        raise ValueError('invalid vendor model profile')
    for name in ('joint','actuator'):
        if name=='actuator' and profile[name] is None:
            continue
        if not re.fullmatch('[A-Za-z0-9_.-]{1,128}', profile[name]):
            raise ValueError('invalid MuJoCo object name')
    staging = output/'compiled-model'
    staging.mkdir()
    compiler = mujoco/'bin/compile'
    args = ['/usr/bin/bwrap','--die-with-parent','--unshare-net','--unshare-ipc','--unshare-pid',
            '--new-session','--cap-drop','ALL','--clearenv','--ro-bind','/usr','/usr',
            '--symlink','usr/lib','/lib','--symlink','usr/lib64','/lib64','--dev','/dev',
            '--proc','/proc','--tmpfs','/tmp','--ro-bind',compiler,'/tool/compile',
            '--ro-bind',mujoco/'lib','/tool/lib','--setenv','LD_LIBRARY_PATH','/tool/lib',
            '--dir','/model','--chdir','/model']
    source = source.resolve(strict=True)
    with tempfile.TemporaryDirectory(dir=output) as private, tempfile.TemporaryDirectory(dir=output) as compiler_output:
        total = 0
        for name, checksum in inputs.items():
            path = PurePosixPath(name)
            if (path.is_absolute() or '..' in path.parts or str(path)!=name or not re.fullmatch('[a-f0-9]{64}',checksum)):
                raise ValueError('invalid vendor asset declaration')
            asset = (source/path).resolve(strict=True)
            extent = asset.stat().st_size
            total += extent
            if not asset.is_relative_to(source) or not asset.is_file() or total>256<<20:
                raise ValueError('vendor source differs from the pinned model')
            sealed = Path(private)/path
            sealed.parent.mkdir(parents=True,exist_ok=True)
            with asset.open('rb') as reader, sealed.open('xb') as writer:
                remaining=extent
                while remaining:
                    block=reader.read(min(remaining,1<<20))
                    if not block:
                        raise ValueError('vendor asset shortened during snapshot')
                    writer.write(block)
                    remaining-=len(block)
                if reader.read(1):
                    raise ValueError('vendor asset grew during snapshot')
            if sha(sealed)!=checksum:
                raise ValueError('vendor source differs from the pinned model')
            sealed.chmod(0o400)
            args.extend(('--ro-bind',sealed,'/model/'+name))
        output_dir = Path(compiler_output)
        args.extend(('--bind',output_dir,'/output'))
        run('compile-vendor-model', ['/usr/bin/prlimit', '--fsize='+str(maximum_bytes)+':'+str(maximum_bytes),'--',*args,'/tool/compile',profile['model'],'/output/physics.mjb'])
        candidate = output_dir/'physics.mjb'
        if candidate.is_symlink() or not candidate.is_file() or not 0 < candidate.stat().st_size <= maximum_bytes:
            raise ValueError('compiled simulation model exceeds the receiver limit')
        binary = staging/'physics.mjb'
        candidate.replace(binary)
    return profile,binary
