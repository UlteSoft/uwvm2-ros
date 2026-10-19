"""Injected-data tests of real ROS2 configuration feature admission.

The guarded Linux caller loads the complete frozen tools/ros_distro/probe.py,
then calls suite(module.validate_ros2_action_features). This factory imports no
project code, starts no process and never treats these data models as native SDK
or ROS delivery tests. Run only in the owner's original 64 GiB cgroup.
"""
import unittest


def suite(validate):
    class Policy(unittest.TestCase):
        def flags(self, core=False, builtin=False, custom=False):
            return {"action_core":core,"action":builtin,"custom_action":custom}
        def provider(self):
            return {"registry":"/selected/manifest.hpp","native":"/selected/native_ros2.hpp","packages":["owned"]}
        def empty(self):
            return {"registry":None,"native":None,"packages":[]}
        def test_no_action_is_valid_unavailable(self):
            validate(self.flags(),self.empty())
        def test_core_without_tutorial_is_valid(self):
            validate(self.flags(core=True),self.empty())
        def test_custom_without_tutorial_is_valid(self):
            validate(self.flags(core=True,custom=True),self.provider())
        def test_builtin_without_custom_is_valid(self):
            validate(self.flags(core=True,builtin=True),self.empty())
        def test_both_concrete_routes_are_independent(self):
            validate(self.flags(core=True,builtin=True,custom=True),self.provider())
        def test_concrete_route_cannot_outlive_core(self):
            for name in ("action","custom_action"):
                values=self.flags();values[name]=True
                with self.subTest(name=name),self.assertRaises((ValueError,RuntimeError)):validate(values,self.provider())
        def test_custom_requires_every_selected_input(self):
            for name in ("registry","native","packages"):
                values=self.provider();values[name]=[] if name=="packages" else None
                with self.subTest(name=name),self.assertRaises((ValueError,RuntimeError)):validate(self.flags(core=True,custom=True),values)
        def test_missing_non_boolean_and_closed_selection_refused(self):
            for name in ("action_core","action","custom_action"):
                for bad in (None,0,1,"false"):
                    values=self.flags();values[name]=bad
                    with self.subTest(name=name,bad=bad),self.assertRaises((ValueError,RuntimeError)):validate(values,self.empty())
                values=self.flags();del values[name]
                with self.assertRaises((ValueError,RuntimeError)):validate(values,self.empty())
            values=self.provider();values["inferred_packages"]=["owned"]
            with self.assertRaises((ValueError,RuntimeError)):validate(self.flags(core=True,custom=True),values)
        def test_policy_never_modifies_observed_false_or_selection(self):
            values=self.flags(core=True);selected=self.provider()
            before=(dict(values),{**selected,"packages":list(selected["packages"])})
            validate(values,selected)
            self.assertEqual((values,selected),before)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policy)
