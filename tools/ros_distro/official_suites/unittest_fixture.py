"""Operator-only real observer fixture, never an official ROS test universe.

Root may copy this exact source into a fresh owned fixture checkout and invoke
the SDK observer in a separate bounded process. Do not add it to an upstream
source lock or count these cases as ROS coverage. It exercises actual original
unittest callbacks without network, providers, subprocesses or external files.
"""
import unittest


class ActualLifecycle(unittest.TestCase):
    def test_pass(self):
        self.assertEqual(2+2,4)

    @unittest.skip('deliberate observer regression fixture')
    def test_skip(self):
        raise AssertionError('must not execute skipped body')

    @unittest.expectedFailure
    def test_expected_failure(self):
        self.assertEqual(1,2)

    @unittest.expectedFailure
    def test_unexpected_success(self):
        self.assertEqual(1,1)
