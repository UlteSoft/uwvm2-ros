"""Unrun local ledger models. No bosdyn/rclpy import or Native qualification."""
import time
import unittest
import builtins
import sys
from unittest.mock import patch
from ..common.native_gate import NativeGate, SERVICES
def enabled(maximum=2):
    return NativeGate({'operations':{name:{'maximum_rpc_ns':5_000_000_000} for name in SERVICES},'maximum_history':maximum})
class GateContract(unittest.TestCase):
    def test_default_denies_without_reservation(self):
        gate=NativeGate()
        with self.assertRaises(PermissionError):gate.enter(SERVICES['state'],'state',(),time.monotonic_ns()+1_000_000_000,1,1)
        self.assertEqual(gate.audit_snapshot_native_only(),())
        from ..spot import ros_bridge
        original_import = builtins.__import__
        def no_native(name, *args, **kwargs):
            if name == 'rclpy' or name.startswith(('rclpy.', 'bosdyn')):
                self.fail('default-denied bridge attempted a native SDK import')
            return original_import(name, *args, **kwargs)
        with patch.object(sys, 'argv', ['spot-bridge']), patch.object(builtins, '__import__', no_native):
            with self.assertRaises(PermissionError) as error:ros_bridge.main()
        self.assertEqual(error.exception.errno, 13)
    def test_full_u64_and_replay(self):
        gate=enabled();value=(1<<64)-1
        with gate.enter(SERVICES['state'],'state',(),time.monotonic_ns()+1_000_000_000,value,value):pass
        with self.assertRaises(ValueError):gate.enter(SERVICES['state'],'state',(),time.monotonic_ns()+1_000_000_000,value,1)
        with self.assertRaises(ValueError):gate.enter(SERVICES['state'],'state',(),time.monotonic_ns()+1_000_000_000,1,value)
    def test_unknown_is_retained(self):
        gate=enabled()
        with self.assertRaisesRegex(RuntimeError,'unknown'):
            with gate.enter(SERVICES['command.stop'],'command.stop',(),time.monotonic_ns()+1_000_000_000,1,2):raise RuntimeError('unknown')
        row=gate.audit_snapshot_native_only()[0];self.assertEqual(row.state,'unknown');self.assertIsInstance(row.error,RuntimeError)
    def test_cleanup_does_not_borrow_full_history(self):
        gate=enabled(1)
        with gate.enter(SERVICES['state'],'state',(),time.monotonic_ns()+1_000_000_000,1,2):pass
        with self.assertRaises(OSError):gate.enter(SERVICES['state'],'state',(),time.monotonic_ns()+1_000_000_000,3,4)
        with gate.cleanup_enter_native_only('command.stop',time.monotonic_ns()+1_000_000_000) as row:self.assertNotEqual(row.token,1)
        with gate.cleanup_enter_native_only('lease.return',time.monotonic_ns()+1_000_000_000) as other:self.assertNotEqual(other.token,row.token)
    def test_cleanup_never_adds_permission(self):
        gate=NativeGate({'operations':{'state':{'maximum_rpc_ns':1_000_000}}})
        with self.assertRaises(PermissionError):gate.cleanup_enter_native_only('command.stop',time.monotonic_ns()+1)
    def test_dispatch_configuration_is_closed(self):
        for value in ({'callable':'unsafe'},{'operations':{'eval':{'maximum_rpc_ns':1}}},{'operations':{'state':{'method':'eval'}}}):
            with self.assertRaises(ValueError):NativeGate(value)
    def test_alias_mismatch_has_no_reservation(self):
        gate=enabled()
        with self.assertRaises(PermissionError):gate.enter(SERVICES['command.stop'],'state',(),time.monotonic_ns()+1_000_000_000,1,2)
        self.assertEqual(gate.audit_snapshot_native_only(),())
    def test_expired_deadline_has_no_reservation(self):
        gate=enabled()
        with self.assertRaises(TimeoutError):gate.enter(SERVICES['state'],'state',(),time.monotonic_ns()-1,1,2)
        self.assertEqual(gate.audit_snapshot_native_only(),())
