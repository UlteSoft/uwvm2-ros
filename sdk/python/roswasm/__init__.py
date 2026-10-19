"""Python ROS Wasm SDK: explicit capabilities, copied bytes, local callbacks.

Importing codecs/events needs no native extension and opens nothing. Construct
Backend(1 or 2) only inside an interpreter with the real built-in ROS bridge.
Use the separately documented rclpy compatibility package for migration; this
package's capability-oriented API does not claim complete rclpy equivalence.
"""
from .raw import Backend, Result, RosError
from .client import (Session, Node, Publisher, Subscription, ServiceClient,
                     ServiceServer, ActionClient, Observer, Executor, Cleanup)
from . import codecs, events

__all__ = ['Backend', 'Result', 'RosError', 'Session', 'Node', 'Publisher',
           'Subscription', 'ServiceClient', 'ServiceServer', 'ActionClient',
           'Observer', 'Executor', 'Cleanup', 'codecs', 'events']
