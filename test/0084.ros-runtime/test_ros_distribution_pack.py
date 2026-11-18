"""Check release selection independently of middleware runtime qualification."""
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[2]/'tools/ros_scene'))
from pack_communication_worker import ros2_prefix


class ReleaseSelection(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def hooks(self, release, legacy=False):
        prefix = self.root/'opt/ros'/release
        hooks = prefix/'share/ros_environment/environment'
        hooks.mkdir(parents=True)
        for key, value in [('ROS_DISTRO', release), ('ROS_VERSION', '2')]:
            if legacy:
                (hooks/('1.'+key.lower()+'.sh')).write_text('# generated upstream\nexport '+key+'='+value+'\n')
            else:
                (hooks/('1.'+key.lower()+'.dsv')).write_text('set;'+key+';'+value+'\n')
        return prefix, hooks

    def test_independent_prefixes(self):
        for name, legacy in [('ardent',True),('bouncy',True),('crystal',True),('dashing',True),('humble',False),('jazzy',False)]:
            prefix, _ = self.hooks(name, legacy)
            selected, digests = ros2_prefix(self.root, name)
            self.assertEqual(selected,prefix)
            self.assertEqual(len(digests),2)

    def test_mismatched_or_ambiguous_identity(self):
        _, hooks = self.hooks('humble')
        for field, value in [('ros_version','set;ROS_VERSION;1'),('ros_distro','set;ROS_DISTRO;jazzy'),
                             ('ros_distro','set;ROS_DISTRO;humble\nset;ROS_DISTRO;jazzy')]:
            with self.subTest(field=field,value=value):
                file = hooks/('1.'+field+'.dsv')
                previous = file.read_bytes()
                file.write_text(value)
                with self.assertRaises(ValueError):
                    ros2_prefix(self.root,'humble')
                file.write_bytes(previous)

    def test_missing_or_escaping_prefix(self):
        selected, hooks = self.hooks('humble')
        (self.root/'opt/ros/rolling').symlink_to(selected, target_is_directory=True)
        for name in ['../humble','aaa/bbb','rolling','jazzy','Humble','']:
            with self.subTest(name=name), self.assertRaises((ValueError,FileNotFoundError)):
                ros2_prefix(self.root,name)
        outside = self.root/'untrusted'
        outside.write_text('set;ROS_DISTRO;humble')
        file = hooks/'1.ros_distro.dsv'
        file.unlink()
        file.symlink_to(outside)
        with self.assertRaises(ValueError):
            ros2_prefix(self.root,'humble')

    def test_shell_hooks_are_read_as_data(self):
        _, hooks = self.hooks('dashing',legacy=True)
        marker = self.root/'was-executed'
        script = hooks/'1.ros_distro.sh'
        script.write_text('touch '+str(marker)+'\nexport ROS_DISTRO=dashing\n')
        ros2_prefix(self.root,'dashing')
        self.assertFalse(marker.exists())
        script.write_text('export ROS_DISTRO='+chr(36)+'(hostname)\n')
        with self.assertRaises(ValueError):
            ros2_prefix(self.root,'dashing')
        script.write_text('export ROS_DISTRO=dashing\n')
        # A present but invalid modern declaration cannot fall back to the legacy hook.
        (hooks/'1.ros_distro.dsv').write_text('set;ROS_DISTRO;humble')
        with self.assertRaises(ValueError):
            ros2_prefix(self.root,'dashing')


if __name__ == '__main__':
    unittest.main()
