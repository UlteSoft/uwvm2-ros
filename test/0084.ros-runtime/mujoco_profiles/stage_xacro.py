"""Stage a digest-verified upstream Xacro expansion and its original URDF meshes."""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import tempfile
import xml.etree.ElementTree as ET

from fetch import relative


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for flag in ('profile', 'assets', 'expanded-urdf', 'output'):
        parser.add_argument('--'+flag, type=Path, required=True)
    args = parser.parse_args()
    reviewed = args.profile.read_bytes()
    profile = json.loads(reviewed)
    root = args.assets.resolve(strict=True)
    out = args.output.absolute()
    if out.is_symlink() or out.exists() or '.tmp' not in out.resolve().parts:
        parser.error('choose a new output under the project .tmp')
    package = profile['package']
    if not re.fullmatch(r'[a-z][a-z0-9_]*', package):
        parser.error('invalid ROS description package')
    up = PurePosixPath(relative(profile['model']))
    if (up.suffix != '.xacro' or up.parts[0] != package or str(up) not in profile['files']
            or profile['sourceDirectory'] != '.' or profile['actuation'] != 'urdf-joint-effort'
            or not 1 <= len(profile['files']) <= 1024):
        parser.error('expected a pinned ROS Xacro package profile')
    with args.expanded_urdf.open('rb') as stream:
        raw = stream.read((1 << 20)+1)
    if len(raw) > 1 << 20 or b'<!DOCTYPE' in raw or b'<!ENTITY' in raw:
        parser.error('unsupported URDF expansion')
    canonical = ET.canonicalize(raw, strip_text=True).encode()
    digest = hashlib.sha256(canonical).hexdigest()
    if digest != profile['expandedUrdfSha256']:
        parser.error('expanded URDF differs from the reviewed model')
    meshes = {}
    robot = ET.fromstring(canonical)
    for mesh in robot.iter('mesh'):
        uri = mesh.get('filename', '')
        path = uri.removeprefix('package://')
        if (not uri.startswith('package://'+package+'/') or path not in profile['files']
                or relative(path) != path):
            parser.error('expanded mesh URI is outside the pinned package')
        meshes[path] = str(PurePosixPath(path).relative_to(package))
    if not meshes:
        parser.error('expected a model with pinned geometry')
    out.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(dir=out.parent) as tmp:
        stage = Path(tmp)/'staging'
        tree = stage/'assets'
        tree.mkdir(parents=True)
        inputs = {}
        total = 0
        for name, checksum in profile['files'].items():
            path = (root/relative(name)).resolve(strict=True)
            if not path.is_file() or not path.is_relative_to(root):
                parser.error('invalid source asset')
            total += path.stat().st_size
            if total > 256 << 20:
                parser.error('source package exceeds the review budget')
            target = tree/meshes[name] if name in meshes else None
            if target:
                target.parent.mkdir(parents=True, exist_ok=True)
            with path.open('rb') as original:
                # Published source files have a per-file bound in the profile fetcher.
                data = original.read((64 << 20)+1)
            if len(data) > 64 << 20:
                parser.error('source asset exceeds its size limit')
            if hashlib.sha256(data).hexdigest() != checksum:
                parser.error('source differs from the pinned Xacro package: '+name)
            if target:
                with target.open('xb') as f:
                    f.write(data)
                inputs[meshes[name]] = checksum
        model = 'expanded.urdf'
        with (tree/model).open('xb') as f:
            f.write(canonical)
        inputs[model] = digest
        derived = dict(profile, sourceDirectory=package, model=model, files=inputs)
        (stage/'profile.json').write_text(json.dumps(derived, indent=2)+'\n')
        (stage/'source.json').write_bytes(reviewed)
        if out.exists() or out.is_symlink():
            parser.error('output was already published')
        stage.rename(out)


if __name__ == '__main__':
    main()
