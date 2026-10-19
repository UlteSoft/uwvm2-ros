#!/usr/bin/env python3
"""Run six genuine same-main ROS1 commands in twelve owned TCPROS lifecycles.

Use a complete frozen source and the ordinary Unitree closed configuration.
This executable admits actual native/Guest/Worker build results first, then
creates its own loopback-only Linux network namespace in this SAME owned PID.
It changes no cgroup or mount namespace, downloads/builds nothing, and grants
no optional WASI rights. Default-denied instances never start a Gateway.

Every success requires a real Focal ROS master, native generated-message
witness, interpreter, complete process-descriptor mappings, exact full bytes,
normal process exits and final input/source/cgroup preservation. Source and
preparation status alone cannot satisfy this runtime test.
"""
from __future__ import annotations
import argparse
import fcntl
import hashlib
import json
import os
from pathlib import Path
import platform
import signal
import socket
import stat
import struct
import sys
import time
import types

ROOT = Path(__file__).resolve().parents[2]
sys.dont_write_bytecode = True

def require(value, reason):
    if not value:
        raise ValueError(reason)

def bounded(path, limit):
    """Read one stable ordinary bootstrap file without importing project code."""
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute bootstrap file required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= limit, 'bounded bootstrap file required')
        chunks, size = [], 0
        while True:
            block = os.read(descriptor, min(65536, limit + 1 - size))
            if not block:
                break
            chunks.append(block)
            size += len(block)
            require(size <= limit, 'bootstrap file grew')
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        require(size == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
                for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'bootstrap file changed')
        return b''.join(chunks)
    finally:
        os.close(descriptor)

def bootstrap(args):
    """Bind this entrypoint and first helper before any project helper executes."""
    source = args.source.resolve(strict=True)
    require(source == ROOT, 'invoke actual complete frozen source entrypoint')
    raw = bounded(args.manifest, 16 * 1024**2)
    require(hashlib.sha256(raw).hexdigest() == args.manifest_sha256, 'explicit current source manifest hash differs')
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, 'duplicate source manifest key')
            result[key] = value
        return result
    frozen = json.loads(raw, object_pairs_hook=pairs)
    require(type(frozen) is dict and frozen, 'complete frozen source manifest required')
    helper = None
    for relative in ('tests/ros_unitree/run_legacy_transport.py', 'tests/ros_javascript_guest/run.py'):
        body = bounded(source / relative, 1024**2)
        require(frozen.get(relative) == {'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest()},
                'actual entry/helper missing from complete frozen manifest')
        if relative.endswith('ros_javascript_guest/run.py'):
            helper = body
    api = types.ModuleType('legacy_transport_low_level_execution')
    api.__file__ = str(source / 'tests/ros_javascript_guest/run.py')
    sys.modules[api.__name__] = api
    exec(compile(helper, api.__file__, 'exec'), api.__dict__)
    unitree = api.frozen_module(source, frozen, 'tests/ros_unitree/common.py', 'legacy_transport_unitree_owners')
    return source, frozen, api, unitree


def private_loopback(frame, expected_parent):
    """Create this owner's actual network namespace and bring only lo UP.

    CLONE_NEWNET and SIOC{G,S}IFFLAGS are Linux UAPI constants. The fixed40-byte
    ifreq is restricted to native x86-64 Linux; namespace/interface identity is
    checked from the current kernel socket APIs, never inherited sysfs. This
    creates no actor, packet, mount, cgroup or Guest network capability.
    """
    frame.guard()
    require(sys.platform == 'linux' and platform.machine().lower() in ('x86_64', 'amd64') and
            hasattr(os, 'unshare') and not frame.actors and
            os.readlink('/proc/self/ns/net') == expected_parent, 'direct native owner/actual parent namespace required')
    old = Path('/proc/self/ns/net').stat()
    os.unshare(0x40000000)  # CLONE_NEWNET, include/uapi/linux/sched.h.
    actual = os.readlink('/proc/self/ns/net')
    new = Path('/proc/self/ns/net').stat()
    require(actual != expected_parent and (old.st_dev, old.st_ino) != (new.st_dev, new.st_ino) and
            [name for _, name in socket.if_nameindex()] == ['lo'], 'fresh private loopback-only namespace required')
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as channel:
        request = struct.pack('16s24x', b'lo')
        before = fcntl.ioctl(channel.fileno(), 0x8913, request)  # SIOCGIFFLAGS.
        require(len(before) == 40, 'complete native Linux ifreq required')
        flags = struct.unpack_from('H', before, 16)[0]
        require(flags & 0x8, 'new loopback must have kernel LOOPBACK flag')
        desired = bytearray(before)
        struct.pack_into('H', desired, 16, flags | 0x1)
        fcntl.ioctl(channel.fileno(), 0x8914, bytes(desired))  # SIOCSIFFLAGS: only IFF_UP added.
    proof = frame.unitree.current_loopback(actual)
    frame.guard()
    return {'network_namespace': actual, 'parent_network_namespace': expected_parent,
        'parent_identity': {'device': old.st_dev, 'inode': old.st_ino},
        'actual_identity': {'device': new.st_dev, 'inode': new.st_ino}, 'owner_pid': os.getpid(),
        'created_by_same_owned_coordinator': True, 'current_interface_proof': proof,
        'sysfs_remounted': False, 'cgroup_changed': False, 'Guest_network_rights': []}


def owned_frame_type(unitree, api, frozen):
    """Keep a full600-second cooperative cleanup budget even on low disk.

    Active work retains all ordinary eight-GiB ancestor reserve checks. During
    failed-run cleanup/preservation only that write-admission check is omitted;
    current cgroup identity,64GiB/swap0, OOM counters, owned RSS and the finite
    cleanup deadline still apply. The ordinary pidfd owner performs terminal
    discovery and forced fallback after this cooperative interval.
    """
    class LegacyFrame(unitree.Frame):
        def __init__(self, args):
            self.finalizing = False
            self.started = time.monotonic()
            self.unitree, self.frozen = unitree, frozen
            self.selected_input = args.selection_input
            super().__init__(args, 'unitree-six-same-main-legacy-native-TCPROS')

        def validate_upstreams(self):
            """Inventory only the exact native producer root as literal CHROOT.

            Its preserved Focal SDK contains absolute *guest-root* symlinks.
            Following those as host names would mislabel a host file as SDK
            authority. This per-Frame module adapter changes exactly one fully
            selected immutable producer root; every other inventory retains
            the original common.catalog implementation and trust boundary.
            Complete current physical file/map_files proofs remain separate.
            """
            super().validate_upstreams()
            native = api.frozen_module(self.source, frozen, 'tests/ros_javascript_guest/native.py',
                                       'legacy_bootstrap_native_document')
            selection, _ = native.document(api, self, self.input(self.selected_input), 1024**2)
            label = selection['reports']['native_products']
            report, pin = native.document(api, self, self.input(label), 64 * 1024**2)
            require(report.get('schema') == 'uwvm-unitree-noetic-native-products-v1' and
                    report.get('status') == 'PREPARED_NOETIC_LEGACY_NATIVE_PRODUCTS_NOT_TCPROS_EXECUTED' and
                    type(report.get('source')) is str and Path(report['source']).is_absolute() and
                    (Path(report['source']) == self.source or any(Path(report['source']) == item or
                        Path(report['source']).is_relative_to(item) for item in self.roots)),
                    'actual complete original native source must be selected before literal inventory')
            root = Path(pin['path']).parent
            require(root in self.roots and pin['path'] == str(root / 'report.json'),
                    'the exact whole native producer output must be selected, not only its report or SDK child')
            support = api.frozen_module(self.source, frozen, 'tests/ros_unitree/noetic_native_support.py',
                                        'legacy_literal_native_input_inventory')
            original = self.common.catalog
            def catalog(selected, output, allowed=None):
                if Path(selected) == root:
                    return support.tree(self.common, root, Path(output), self.guard)
                return original(selected, output, allowed)
            self.common.catalog = catalog
            self.literal_native_root = root

        def guard(self):
            require(time.monotonic() - self.started <= (7800 if self.finalizing else 7200),
                    'legacy active/finalization deadline exhausted')
            if self.finalizing:
                self.common.cgroup_preserved(self.before, self.common.cgroup_state(self.config['cgroup']))
            else:
                super().guard()

        def finish(self, status, error=None):
            self.finalizing = True
            cooperative_errors, signals = [], []
            until = time.monotonic() + 600
            for actor in reversed(self.actors):
                try:
                    actor.owner.discover()
                    for key, item in reversed(list(actor.owner.owned.items())):
                        if not self.common.pidfd_terminal(item['fd']):
                            require(self.common.birth(self.common.identity(key[0])) == key,
                                    'cooperative cleanup owned birth changed')
                            signal.pidfd_send_signal(item['fd'], signal.SIGTERM)
                            signals.append({'pid': key[0], 'birth': list(key), 'signal': 'SIGTERM', 'method': 'OWNED_PIDFD'})
                except BaseException as caught:
                    cooperative_errors.append(repr(caught))
            while self.actors and time.monotonic() < until:
                live = False
                try:
                    self.guard()
                    for actor in self.actors:
                        actor.owner.discover()
                        actor.pump()
                        require(actor.owner.rss() <= self.limits['rss_bytes'], 'cleanup owned RSS exhausted')
                        live |= any(not self.common.pidfd_terminal(item['fd']) for item in actor.owner.owned.values())
                        for child, _ in actor.rows:
                            child.poll()
                            live |= child.poll() is None
                except BaseException as caught:
                    cooperative_errors.append(repr(caught))
                    break  # Still invoke the original bounded pidfd fallback.
                if not live:
                    break
                time.sleep(.02)
            self.report['cooperative_cleanup'] = {'budget_seconds': 600, 'signals': signals, 'errors': cooperative_errors,
                'disk_reserve_checked_during_active_work': True, 'low_disk_cannot_skip_owned_cleanup': True}
            if cooperative_errors:
                status, error = 'FAIL', repr({'original': error, 'cooperative_cleanup': cooperative_errors})
            result = super().finish(status, error)
            # A successful result must also meet the active disk reserve at the
            # terminal barrier. Failed preservation never waits for free disk.
            if result == 0:
                try:
                    self.finalizing = False
                    self.guard()
                except BaseException as caught:
                    self.report.update(status='FAIL', error=repr(caught))
                    self.save()
                    result = 1
            return result
    return LegacyFrame


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--manifest', type=Path, required=True)
    parser.add_argument('--manifest-sha256', required=True)
    parser.add_argument('--config', type=Path, required=True)
    parser.add_argument('--config-sha256', required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--selection-input', required=True)
    args = parser.parse_args()
    frame = None
    try:
        source, frozen, api, unitree = bootstrap(args)
        frame = owned_frame_type(unitree, api, frozen)(args)
        frame.unitree, frame.frozen, frame.workers = unitree, frozen, {}
        require(all(frame.config[key] is None for key in ('simulation', 'benchmark', 'profiler')) and
                frame.config['environment'] == {}, 'legacy transport has no unrelated workload/native host environment')
        require(args.selection_input in frame.inputs and len(os.fsencode(frame.out / 's11')) < 104,
                'explicit selected legacy input and short owned socket prefix required')
        admission = api.frozen_module(source, frozen, 'tests/ros_unitree/legacy_transport_inputs.py', 'legacy_transport_admission')
        session = api.frozen_module(source, frozen, 'tests/ros_unitree/legacy_transport_session.py', 'legacy_transport_sessions')
        native = api.frozen_module(source, frozen, 'tests/ros_javascript_guest/native.py', 'legacy_transport_descriptor_proofs')
        selection, selection_pin = native.document(api, frame, frame.input(args.selection_input), 1024**2)
        frame.report.update(selection=selection_pin, cases=[], actual_Runtime_cases_executed=0, actual_Runtime_launches_released=0,
            main_principal='main', optional_WASI_rights=[], default_ROS_authority=[],
            scope='Actual six-language same-main ROS1 TCPROS only; no ROS2, physical robot, firmware CRC, simulator or performance qualification')
        frame.save()
        admitted = admission.admit(frame, api, native, selection)
        frame.report['isolation'] = private_loopback(frame, selection['parent_network_namespace'])
        frame.report['admission']['transport_execution'] = 'RUNNING_ACTUAL_OWNED_TCPROS_CASES'
        frame.save()
        for product in admitted['products']:
            for positive in (False, True):
                session.run_case(api, native, frame, admitted, product, positive)
                frame.report['completed_actual_cases'] = len(frame.report['cases'])
                frame.save()
        cases = frame.report['cases']
        require(len(cases) == frame.report['actual_Runtime_cases_executed'] == frame.report['actual_Runtime_launches_released'] == 12 and
                len({row['case'] for row in cases}) == 12 and sum(row['positive'] for row in cases) == 6 and
                all(row['status'] == 'PASS_ACTUAL_LEGACY_TCPROS_CASE' and
                    row['worker_exit'] == row['native_exit'] == row['master_exit'] == 0 and
                    row['cleanup']['status'] == 'PASS' for row in cases), 'all twelve fresh actual legacy cases required')
        frame.unitree.current_loopback(frame.report['isolation']['network_namespace'])
        frame.report['admission']['transport_execution'] = 'COMPLETED_12_ACTUAL_OWNED_TCPROS_CASES'
        return frame.finish('PASS_UNITREE_SIX_SAME_MAIN_COMMANDS_TWELVE_ACTUAL_LEGACY_TCPROS_CASES')
    except BaseException as error:
        if frame is not None:
            return frame.finish('FAIL', repr(error))
        print(repr(error), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())

