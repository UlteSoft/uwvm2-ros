"""Verify that an invalid vendor model cannot reach the native model compiler."""
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

from mujoco_profiles.compile_model import prepare
from run_scene_vm import ROOT, verify_gate


class ModelIntake(unittest.TestCase):
    def test_compiled_size_budget_checked_before_reading_source(self):
        verify_gate()
        for size in ((1 << 20) - 1, (512 << 20) + 1):
            with self.subTest(size=size), self.assertRaisesRegex(ValueError, 'budget'):
                prepare(None, None, None, None, (), None, maximum_bytes=size)

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
