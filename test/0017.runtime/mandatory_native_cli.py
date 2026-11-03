"""Test-driver routing for the ROS product's mandatory checked native stack.

Ordinary-upstream selectors are retained only for independently built upstream
executables. These functions never translate a requested instruction-mode test
into a native-mode pass.
"""
import sys


def require_linux_ros(parser, ros):
    if ros and sys.platform != 'linux':
        parser.error('ROS product qualification requires Linux; use an ordinary-upstream executable for this platform')


def product_policy(parser, ros, requested, ordinary_default='instruction', required=False):
    if ros:
        if requested is not None:
            parser.error('ROS has no stack-policy selector; omit it to test mandatory checked native unwind')
        return 'mandatory-native'
    if required and requested is None:
        parser.error('ordinary-upstream component comparison requires an explicit stack policy')
    return ordinary_default if requested is None else requested


def product_policies(parser, ros, requested, ordinary_default=('instruction', 'unwind')):
    if ros:
        if requested:
            parser.error('ROS has no stack-policy selector; omit it to test mandatory checked native unwind')
        return ('mandatory-native',)
    return requested or ordinary_default
