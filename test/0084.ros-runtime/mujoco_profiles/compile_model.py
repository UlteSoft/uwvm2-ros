"""Prepare an individually pinned vendor simulation scene with the official MuJoCo compiler."""
import json
import math
from pathlib import Path, PurePosixPath
import re
import tempfile
import xml.etree.ElementTree as ET

from run_scene_vm import sha


def urdf_mesh_mounts(profile, snapshot, output):
    """Resolve original ROS package mesh URIs inside MuJoCo's URDF basename lookup."""
    model = PurePosixPath(profile['model'])
    original = snapshot/model
    if model.suffix != '.urdf' or profile['actuator'] is not None or original.stat().st_size > 1 << 20:
        raise ValueError('expected an original bounded URDF without a native actuator')
    data = original.read_bytes()
    if b'<!DOCTYPE' in data or b'<!ENTITY' in data:
        raise ValueError('URDF entity declarations are outside this fixture')
    robot = ET.fromstring(data)
    matches = [j for j in robot.findall('joint') if j.get('name') == profile['joint']]
    if robot.tag != 'robot' or len(matches) != 1 or matches[0].get('type') not in ('revolute', 'continuous'):
        raise ValueError('expected one declared rotary URDF joint')
    joint = matches[0]
    if joint.find('mimic') is not None:
        raise ValueError('the selected fixture drive cannot actuate a dependent mimic joint')
    limit = joint.find('limit')
    effort = float(limit.get('effort', 'nan')) if limit is not None else math.nan
    if not math.isfinite(effort) or effort < 1:
        raise ValueError('URDF joint must allow the one newton-metre fixture limit')
    velocity = float(limit.get('velocity', 'nan'))
    if not math.isfinite(velocity) or velocity <= 0:
        raise ValueError('URDF joint has no positive rated velocity')
    if joint.get('type') == 'revolute':
        lower, upper = (float(limit.get(key, 'nan')) for key in ('lower', 'upper'))
        if not (math.isfinite(lower) and math.isfinite(upper) and lower <= -.5 and .5 <= upper):
            raise ValueError('both fixture target angles must lie within the URDF joint limits')
    child = joint.find('child')
    links = [link for link in robot.findall('link') if child is not None and link.get('name') == child.get('link')]
    mass = links[0].find('inertial/mass') if len(links) == 1 else None
    mass_kg = float(mass.get('value', 'nan')) if mass is not None else math.nan
    tensor = links[0].find('inertial/inertia') if len(links) == 1 else None
    if not math.isfinite(mass_kg) or mass_kg <= 0 or tensor is None:
        raise ValueError('selected URDF link requires a positive mass and original inertia')
    xx, yy, zz, xy, xz, yz = (float(tensor.get('i'+key, 'nan')) for key in ('xx','yy','zz','xy','xz','yz'))
    determinant = xx*yy*zz + 2*xy*xz*yz - xx*yz*yz - yy*xz*xz - zz*xy*xy
    if not (all(map(math.isfinite, (xx,yy,zz,xy,xz,yz,determinant))) and
            xx > 0 and xx*yy > xy*xy and determinant > 0):
        raise ValueError('selected URDF link requires a positive definite inertia tensor')
    # Package-root profiles omit the ROS package prefix from their pin names.
    # Repository-root profiles pin each mesh using its complete package path.
    repo_root = profile['sourceDirectory'] == '.'
    package = PurePosixPath(profile['sourceDirectory']).name
    prefix = 'package://' if repo_root else 'package://' + package + '/'
    aliases = {}
    mounts = []
    for mesh in robot.iter('mesh'):
        uri = mesh.get('filename', '')
        if not uri.startswith(prefix) or uri[len(prefix):] not in profile['files']:
            raise ValueError('URDF mesh must be a pinned member of the selected ROS package')
        source = PurePosixPath(uri[len(prefix):])
        # MuJoCo normally strips URDF mesh directories. Visual and collision
        # meshes often share a basename, so give each pinned payload its own name.
        alias = 'wros_mesh_' + profile['files'][str(source)] + source.suffix.lower()
        target = model.parent/alias
        if str(target) in profile['files']:
            raise ValueError('URDF fixture mesh name collides with a declared input')
        if alias not in aliases:
            mounts.extend(('--ro-bind', snapshot/source, '/model/' + str(target)))
            aliases[alias] = str(source)
        mesh.set('filename', alias)
    if 'mobileBase' in profile:
        root_link = profile['mobileBase']
        link_names = {link.get('name') for link in robot.findall('link')}
        children = {child.get('link') for child in robot.findall('joint/child')}
        if (not isinstance(root_link, str) or link_names - children != {root_link}
                or root_link not in link_names or any(
                    e.get('name', '').startswith('wros_fixture_') for e in robot.iter())):
            raise ValueError('mobile scene requires an unambiguous original root link')
        # A bare mobile URDF would weld the chassis to world. Add a separate
        # fixed floor and free the original root; preserve every robot member.
        floor = ET.SubElement(robot, 'link', name='wros_fixture_world')
        collision = ET.SubElement(floor, 'collision', name='wros_fixture_floor')
        ET.SubElement(collision, 'origin', xyz='0 0 -0.06')
        geometry = ET.SubElement(collision, 'geometry')
        ET.SubElement(geometry, 'box', size='4 4 0.1')
        free = ET.SubElement(robot, 'joint', name='wros_fixture_chassis', type='floating')
        ET.SubElement(free, 'parent', link='wros_fixture_world')
        ET.SubElement(free, 'child', link=root_link)
    with tempfile.NamedTemporaryFile(dir=snapshot, suffix='.urdf', delete=False) as f:
        ET.ElementTree(robot).write(f, encoding='utf-8', xml_declaration=True)
        normalized = Path(f.name)
    mounts.extend(('--ro-bind', normalized, '/model/' + str(model)))
    (output/'urdf-intake.json').write_text(json.dumps({
        'originalSha256': profile['files'][str(model)], 'normalizedSha256': sha(normalized),
        'meshes': aliases, 'joint': profile['joint'], 'sourceEffortNm': effort, 'appliedLimitNm': 1,
        'sourceVelocityRadPerSecond': velocity, 'selectedLinkMassKg': mass_kg,
    }, indent=2) + '\n')
    return mounts


def prepare(profile_path, source, mujoco, output, platforms, run, maximum_bytes=64 << 20):
    if not (1 << 20) <= maximum_bytes <= (512 << 20):
        raise ValueError('invalid compiled simulation model budget')
    profile = json.loads(profile_path.read_bytes())
    platform = profile['platform']
    inputs = profile['files']
    if profile.get('actuation', 'actuator') not in ('actuator', 'urdf-joint-effort'):
        raise ValueError('unsupported simulation actuation')
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
        if profile.get('actuation') == 'urdf-joint-effort':
            args.extend(urdf_mesh_mounts(profile, Path(private), output))
        output_dir = Path(compiler_output)
        args.extend(('--bind',output_dir,'/output'))
        run('compile-vendor-model', ['/usr/bin/prlimit', '--fsize='+str(maximum_bytes)+':'+str(maximum_bytes),'--',*args,'/tool/compile',profile['model'],'/output/physics.mjb'])
        candidate = output_dir/'physics.mjb'
        if candidate.is_symlink() or not candidate.is_file() or not 0 < candidate.stat().st_size <= maximum_bytes:
            raise ValueError('compiled simulation model exceeds the receiver limit')
        binary = staging/'physics.mjb'
        candidate.replace(binary)
    return profile,binary
