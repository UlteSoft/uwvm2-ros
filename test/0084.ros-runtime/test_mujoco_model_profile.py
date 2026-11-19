"""Verify that an invalid vendor model cannot reach the native model compiler."""
import hashlib
import json
from pathlib import Path
import tempfile
import unittest
import xml.etree.ElementTree as ET

from mujoco_profiles.compile_model import prepare, urdf_mesh_mounts
from run_scene_vm import ROOT, verify_gate


class ModelIntake(unittest.TestCase):
    def test_compiled_size_budget_checked_before_reading_source(self):
        verify_gate()
        for size in ((1 << 20) - 1, (512 << 20) + 1):
            with self.subTest(size=size), self.assertRaisesRegex(ValueError, 'budget'):
                prepare(None, None, None, None, (), None, maximum_bytes=size)

    def test_urdf_pinned_meshes_and_rated_joint(self):
        verify_gate()
        scratch = ROOT/'.tmp'
        scratch.mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=scratch) as tmp:
            work = Path(tmp)
            robot = ET.fromstring("""<robot name="fixture">
              <link name="base"/>
              <link name="arm">
                <inertial><mass value="1"/><inertia ixx="1" iyy="1" izz="1" ixy="0" ixz="0" iyz="0"/></inertial>
                <visual><geometry><mesh filename="package://arm/visual/link.stl"/></geometry></visual>
                <collision><geometry><mesh filename="package://arm/collision/link.stl"/></geometry></collision>
              </link>
              <joint type="revolute" name="hinge"><parent link="base"/><child link="arm"/>
                <limit lower="-1" upper="1" velocity="1" effort="2"/></joint>
            </robot>""")
            src = work/'model.urdf'
            ET.ElementTree(robot).write(src)
            assets = {}
            for folder in ('visual','collision'):
                name = folder+'/link.stl'
                asset = work/name
                asset.parent.mkdir()
                asset.write_bytes(folder.encode())
                assets[name] = hashlib.sha256(asset.read_bytes()).hexdigest()
            original = src.read_bytes()
            profile = dict(model=src.name, sourceDirectory='arm', joint='hinge', actuator=None,
                           files={src.name:hashlib.sha256(original).hexdigest(), **assets})
            mounts = urdf_mesh_mounts(profile, work, work)
            staged = ET.parse(mounts[-2])
            aliases = [m.get('filename') for m in staged.iter('mesh')]
            self.assertEqual(len(set(aliases)), 2)
            self.assertEqual(set(mounts[2:-3:3]), {'/model/'+alias for alias in aliases})
            self.assertEqual(src.read_bytes(), original)
            self.assertEqual(staged.find('joint/limit').attrib, robot.find('joint/limit').attrib)
            checks = [('joint/limit','effort','0','newton-metre'), ('joint/limit','velocity','0','velocity'),
                      ('joint/limit','upper','0.2','target angles'), ('link/inertial/mass','value','0','mass'),
                      ('link/inertial/inertia','ixy','5','inertia'), ('.//mesh','filename','package://other/file','pinned')]
            for path, key, value, error in checks:
                with self.subTest(path=path, key=key):
                    edited = ET.fromstring(original)
                    edited.find(path).set(key, value)
                    ET.ElementTree(edited).write(src)
                    with self.assertRaisesRegex(ValueError, error):
                        urdf_mesh_mounts(profile, work, work)

    def test_declined_before_compiler_start(self):
        verify_gate()
        scratch = ROOT/'.tmp'
        scratch.mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=scratch) as tmp:
            work = Path(tmp)
            source = work/'source'
            source.mkdir()
            sample = b'<mujoco/>'
            checksum = hashlib.sha256(sample).hexdigest()
            (source/'scene.xml').write_bytes(sample)
            (work/'outside.xml').write_bytes(sample)
            (source/'link.xml').symlink_to(work/'outside.xml')
            profile = dict(platform='unitree', commit='0'*40, model='scene.xml',
                           joint='hinge', actuator='motor', files={'scene.xml':checksum})
            cases = [
                {'platform':'absent'},
                {'model':'undeclared.xml'},
                {'files':{'scene.xml':'0'*64}},
                {'files':{'scene.xml':checksum, '../outside.xml':checksum}},
                {'files':{'scene.xml':checksum, 'link.xml':checksum}},
                {'files':{'scene.xml':checksum, 'missing.xml':checksum}},
            ]
            for index, changes in enumerate(cases):
                with self.subTest(changes=changes):
                    path = work/'profile.json'
                    path.write_text(json.dumps(dict(profile, **changes)))
                    out = work/str(index)
                    out.mkdir()
                    with self.assertRaises((ValueError, FileNotFoundError)):
                        prepare(path, source, work/'mujoco', out, ('unitree',),
                                lambda *_: self.fail('unexpected model compiler startup'))


if __name__ == '__main__':
    unittest.main()
