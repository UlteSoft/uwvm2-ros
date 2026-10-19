#!/usr/bin/env python3
"""Prepare or execute the separate signed ROS2 revoke/fault/reconnect fixture.

This source-only controller inherits the strong original SDK, full-MD/object,
link, source-manifest, QEMU, exact64GiB swap0 cgroup and owned-process admission
from the held G09 positive controller. Only new lifecycle source paths are
compiled. Execution is root-only; pure models do not execute native processes.

The same Wasm instance keeps two explicitly granted publisher handles. After
one alias is revoked, the controller kills and reaps only the exact owned old
Gateway, then starts its immutable replacement for the same Worker. Success
requires error9/125 before and after reconnection, error107 on the dead peer,
a different authenticated session and native publisher GID, survivor delivery,
and an independent native revoked-topic counter that never increases.

This bounded subcase does not establish every original G09/lifecycle behavior.
"""
import sys
sys.dont_write_bytecode = True
import argparse
import fnmatch
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import posixpath
import re
import shlex
import shutil
import signal
import socket
import stat
import subprocess
import tempfile
import time
import traceback
import xml.etree.ElementTree as ET

from runtime import (Evidence, cgroup_incarnation, cgroup_oom_deltas, digest, elf,
                     read_json_binding, require_arm)
from rebuild_affected import rooted, sdk_inventory, sdk_root_aliases, source_state
from gateway_qualification import (architecture_binding, pins_preserved,
                                   qualify_gateway, read_file_binding)
from run_action_qos import process_identity, owned_stopped_arm
from run_local_parameter_events import fixture_closure, validate_fixture_link
from run_ros1_search import OwnedProcesses
from run_resources import bind_frozen_work
from run_all import validate_fresh_worker_inputs
from native_runtime_environment import gateway_library_environment

KIND = 'g09-revoke-fault-reconnect-arm'
DOMAIN = 75
MEMORY_MAX = 68719476736
EXPECTED_BOOT = '518a7b26-33dd-4044-9456-93e75aac9b63'
EXPECTED_LEAF_DEVICE = 29
EXPECTED_LEAF_INODE = 18927
# Retained only by the historical pure-model compatibility predicate. Actual
# preparation/execution requires a separately opened --expected-cgroup receipt.
WITNESS = 'revoke-reconnect-witness'
WITNESS_SOURCE = 'tests/ros_lifecycle/ros2_revoke_reconnect_witness.cc'
WAT_SOURCE = 'tests/ros_lifecycle/revoke_reconnect_ros2.wat'
POLICY_SOURCE = 'tests/ros_lifecycle/revoke_reconnect_sros2-policy.xml'
KEYSTORE = '/tmp/revoke-reconnect-keys'
NATIVE_TYPE = 'std_msgs/msg/String'
NATIVE_IDENTITY = 'RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18'
ROLE_LABELS = {'worker':'revoke-worker', 'gateway_old':'revoke-gateway', 'gateway_new':'revoke-gateway', 'witness':WITNESS}
DRIVER_INPUTS = ('run_revoke_reconnect.py', 'runtime.py', 'rebuild_affected.py',
                 'gateway_qualification.py', 'run_action_qos.py',
                 'run_local_parameter_events.py', 'run_ros1_search.py',
                 'run_resources.py', 'run_all.py', 'run_unit.py',
                 'build_rpc_reader_diagnostic.py', 'native_runtime_environment.py',
                 'artifacts.py', 'run_regression.py', 'qualify_current_native.py')
BARRIER = '''#include <csignal>
__attribute__((constructor)) static void revoke_witness_preinit_stop() { if(::raise(SIGSTOP)!=0) { __builtin_trap(); } }
'''
PRIMARY_CONTRACT = {'actual_Jazzy_RMW_GID_STORAGE_SIZE':16, 'exact_marker_hex_digits':32,
    'sdk_header_sha256':'c1e7be9e420d3c32cf6b24db62c3fbd6b97d0518811f6f4677db09d835cf3830',
    'worker_authority_source':'src/uwvm2/uwvm/imported/ros/host.h',
    'transport_error_source':'src/uwvm2/imported/ros/ipc/unix.h',
    'local_revocation_source':'src/uwvm2/imported/ros/capability/manager.h'}

WITNESS_COMMON = ('pid','node','enclave','domain','rmw','type','dead_topic','live_topic','dead_sub_gid','live_sub_gid')
OLD_GIDS = ('old_dead_pub_gid','old_live_pub_gid')
WITNESS_FIELDS = {
    'READY_REVOKE_WITNESS': WITNESS_COMMON,
    'MATCHED_REVOKE_OLD': (*WITNESS_COMMON,*OLD_GIDS,'dead_publishers','live_publishers'),
    'BASELINE_REVOKE_WITNESS': (*WITNESS_COMMON,*OLD_GIDS,'dead_before','live_before'),
    'FROZEN_REVOKE_WITNESS': (*WITNESS_COMMON,*OLD_GIDS,'dead_total','live_before','unauthorized_dead'),
    'MATCHED_REVOKE_NEW': (*WITNESS_COMMON,*OLD_GIDS,'new_live_pub_gid','dead_publishers','live_publishers','dead_total','live_before','live_after'),
    'DELIVERED_REVOKE_NEW': (*WITNESS_COMMON,*OLD_GIDS,'new_live_pub_gid','dead_publishers','live_publishers','dead_total','live_before','live_after'),
    'PASS_REVOKE_RECONNECT_WITNESS': (*WITNESS_COMMON,*OLD_GIDS,'new_live_pub_gid','dead_publishers','live_publishers','dead_total','live_before','live_after',
        'unauthorized_dead','fault_live','unknown','quiet_samples','quiet_ns','warmup_dead','warmup_live','warmup_new'),
}
GUEST_MARKERS = ('REVOKE_ENDPOINTS_CREATED','BEFORE_MESSAGES_SENT','REVOKED_STALE9_REOPEN125',
                 'GATEWAY_FAULT107','RECONNECT_ENDPOINT_CREATED','AFTER_MESSAGE_SENT','PASS_REVOKE_RECONNECT_GUEST')
NATIVE_FAILURE_PHASES = ('input','initialization','own-discovery','old-discovery','baseline','revoke-gate',
                         'fresh-discovery','survivor-delivery','completion-gate','quiet-observation','cleanup')
NATIVE_ENDPOINT_FAILURE_FIELDS = ('name_unknown','name_bare_dds','name_mismatch','namespace_unknown','namespace_bare_dds',
                                 'namespace_mismatch','type','direction','hash','zero_gid','reliability','durability')
NATIVE_FAILURE_FIELDS = {
    'native_exception':('unspecified',),'contract':('unspecified',),
    'callback_origin':('implementation','zero_gid'),'callback_source':('null_or_intraprocess',),
    'dead_delivery':('warmup_rejected','baseline_rejected','unexpected_text'),
    'live_delivery':('fault_text','warmup_rejected','baseline_rejected','reconnect_rejected','after_rejected','unexpected_text'),
    'callback_counters':('forbidden_effect','positive_count','warmup_count'),'frozen_counters':('baseline_count','warmup_count'),
    'own_subscription_graph':('shape',),'old_publisher_graph':('shape',),'fresh_own_subscriptions':('identity',),
    'fresh_dead_graph':('shape','old_gid'),
    'fresh_live_graph':('shape','duplicate_old','duplicate_fresh','independent_gid','warmup_origin'),
    'fresh_snapshot':('delivery_count',),'exchange_deadline':('before_spin','after_spin','after_sleep','before_new_marker'),
    'own_subscription':NATIVE_ENDPOINT_FAILURE_FIELDS,'old_publisher':NATIVE_ENDPOINT_FAILURE_FIELDS,
    'fresh_old_dead':NATIVE_ENDPOINT_FAILURE_FIELDS,'fresh_dead_candidate':NATIVE_ENDPOINT_FAILURE_FIELDS,
    'fresh_old_live':NATIVE_ENDPOINT_FAILURE_FIELDS,'fresh_live_candidate':NATIVE_ENDPOINT_FAILURE_FIELDS,
}


def recover_popen_owners(owned, children):
    """Recover only retained direct Popen roots before failure cleanup.

    A child is ledgered before its first fallible procfs/pidfd/evidence call.
    This recovery never signals a numeric PID or assumes an argv match proves
    ownership: it requires a current direct-child relationship, an unchanged
    recorded birth when available, and stable birth/parent around pidfd_open.
    Registration precedes cgroup checking so an owned escape remains cleanup's
    responsibility. An unavailable proof is retained as metadata; the caller
    must report CLEANUP_INCOMPLETE if any retained Popen root remains live.
    """
    recovery = []
    for child, row in children:
        if child.poll() is not None:
            continue
        fd = None
        try:
            first = process_identity(child.pid)
            row["startup_cleanup_first_observation"] = first
            birth = row.get("birth")
            expected = birth.get("starttime_ticks") if isinstance(birth, dict) else row.get("starttime_ticks")
            if expected is not None and first["starttime_ticks"] != expected:
                raise RuntimeError("retained Popen birth differs before cleanup admission")
            key = (child.pid, first["starttime_ticks"])
            if key in owned.rows:
                continue
            if first["parent_pid"] != os.getpid():
                raise RuntimeError("retained live Popen root has no current caller-parent proof")
            fd = os.pidfd_open(child.pid)
            second = process_identity(child.pid)
            if (any(first[field] != second[field] for field in ("pid", "starttime_ticks"))
                    or second["parent_pid"] != os.getpid()):
                raise RuntimeError("retained direct Popen birth/parent changed around cleanup pidfd")
            record = {"identity": first, "pidfd": fd, "label": row["name"], "ancestry": [child.pid],
                      "acquisition": {"first": first, "after_pidfd": second,
                                      "parent_changed": False, "cleanup_recovery": True}}
            owned.rows[key] = record
            fd = None  # Ownership moves once to the shared tracker's cleanup.
            if hasattr(owned, "unproven"):
                owned.unproven.pop(key, None)
            row["startup_cleanup_ownership"] = {"identity": first, "after_pidfd": second,
                                                 "method": "retained direct Popen birth and pidfd"}
            owned.check(record)
        except BaseException as error:
            row["startup_cleanup_admission_error"] = repr(error)
            recovery.append({"pid": child.pid, "name": row["name"], "error": repr(error)})
        finally:
            if fd is not None:
                try:
                    os.close(fd)
                except BaseException as error:
                    row["startup_cleanup_descriptor_error"] = repr(error)
                    recovery.append({"pid": child.pid, "name": row["name"], "descriptor_error": repr(error)})
    return recovery


def retained_popen_cleanup(owned, children):
    """Audit every retained root, including one never admitted by the tracker."""
    recovery = recover_popen_owners(owned, children)
    try:
        result = owned.stop([child for child, _ in children])
    except BaseException as error:
        # A failed tracker cleanup is never evidence that its births retired.
        # Still audit the retained Popen handles below and preserve the failure.
        result = {"status": "CLEANUP_INCOMPLETE", "remaining_owned_processes": [],
                  "owned_cleanup_unverified": True, "errors": [repr(error)]}
    remaining = [{"pid": child.pid, "name": row["name"],
                  "recorded_birth": row.get("birth", row.get("starttime_ticks")),
                  "admission_error": row.get("startup_cleanup_admission_error")}
                 for child, row in children if child.poll() is None]
    result["startup_ownership_recovery"] = recovery
    result["remaining_popen_roots"] = remaining
    if remaining:
        result["status"] = "CLEANUP_INCOMPLETE"
    elif any("descriptor_error" in item for item in recovery):
        result["status"] = "FAIL"
    return result


def protocol_require(condition, message):
    if not condition:
        raise RuntimeError(message)


def terminal_command_metadata(previous_status, before_cleanup_returncode, final_returncode, success_pending):
    """Classify a retired command without treating failure cleanup as success.

    The caller samples each retained Popen immediately before owned cleanup,
    then supplies its actual terminal return code after that bounded cleanup.
    This function only produces diagnostic metadata; the existing completion,
    architecture and ownership predicates decide whether behavior is accepted.
    An already successful tool or the deliberately killed old Gateway keeps
    its established status. A still-live process stopped after a failed lane
    is distinguished from a native process that had already failed itself.
    """
    protocol_require(previous_status in ('RUNNING','PASS','FAIL','EXITED_PENDING_ARCHITECTURE','EXPECTED_OWNED_FAULT'),
                     'known pre-cleanup command status required')
    protocol_require(before_cleanup_returncode is None or type(before_cleanup_returncode) is int,
                     'exact pre-cleanup return code required')
    protocol_require(type(final_returncode) is int and type(success_pending) is bool,
                     'actual terminal return code and exact pending state required')
    protocol_require(before_cleanup_returncode is None or before_cleanup_returncode == final_returncode,
                     'terminal command result changed during cleanup')
    if previous_status in ('PASS','EXPECTED_OWNED_FAULT'):
        protocol_require(final_returncode == (0 if previous_status == 'PASS' else -9),
                         'previously accepted command result changed')
        status = previous_status
    elif before_cleanup_returncode is not None:
        status = 'FAIL' if final_returncode != 0 else 'EXITED_NOT_ACCEPTED'
    elif final_returncode in (0,-15,-9):
        status = 'OWNED_CLEANUP_PENDING_ACCEPTANCE' if success_pending else 'OWNED_ABORTED_AFTER_FAILURE'
    else:
        # An unrelated native failure can race the pre-cleanup sample. Preserve
        # its actual result rather than attributing that exit to owned signals.
        status = 'FAIL_DURING_CLEANUP'
    return {'status':status,'status_before_cleanup':previous_status,'returncode':final_returncode,
            'returncode_before_cleanup':before_cleanup_returncode,
            'exited_before_cleanup':before_cleanup_returncode is not None,
            'alive_before_cleanup':before_cleanup_returncode is None}


def reject_native_failure_line(line):
    """Expose only bounded public failure codes and always reject the line.

    The native witness emits fixed source-owned phase/site/field literals.
    This diagnostic path cannot return a successful marker or admit partial
    evidence. Malformed failure lines remain refusals without copying unknown
    text, exception bodies, payloads or credential paths into the diagnostic.
    """
    protocol_require(isinstance(line,str) and len(line) <= 320 and line.isascii(), 'bounded native failure line required')
    matched = re.fullmatch(r'FAIL_REVOKE_RECONNECT_WITNESS phase=([a-z][a-z0-9_-]{0,63}) '
                           r'site=([a-z][a-z0-9_-]{0,63}) field=([a-z][a-z0-9_-]{0,63})',line)
    protocol_require(matched is not None, 'native witness emitted a malformed failure diagnostic')
    protocol_require(matched[1] in NATIVE_FAILURE_PHASES and matched[3] in NATIVE_FAILURE_FIELDS.get(matched[2],()),
                     'native witness emitted an unknown fixed failure code')
    raise RuntimeError('native witness refused phase='+matched[1]+' site='+matched[2]+' field='+matched[3])


def parse_witness_markers(text, expected_pid, complete=True):
    """Accept the seven fixed LF lines, exact16-byte GIDs and frozen counters.

    Warmups can be lost before volatile discovery and are bounded to one per
    source site. Baseline and surviving messages must each be delivered once.
    Final endpoint counts retain the witnessed MATCHED_REVOKE_NEW snapshot;
    the Guest intentionally closes its publisher before the final quiet phase.
    """
    protocol_require(type(expected_pid) is int and expected_pid > 0, 'exact native PID required')
    protocol_require(type(complete) is bool and isinstance(text,str) and len(text) <= 16*1024**2 and text.isascii(), 'bounded ASCII native log required')
    protocol_require('\r' not in text and (not complete or bool(text) and text.endswith('\n')), 'exact complete native LF bytes required')
    lines = text.split('\n')[:-1]
    rows = {};order = tuple(WITNESS_FIELDS)
    fixed = {'pid':expected_pid,'node':'/uwvm_revoke_test/revoke_witness','enclave':'/witness','domain':DOMAIN,
             'rmw':'rmw_fastrtps_cpp','type':NATIVE_TYPE,'dead_topic':'/uwvm_revoke_test/dead','live_topic':'/uwvm_revoke_test/live'}
    numeric = {'pid','domain','dead_publishers','live_publishers','dead_before','live_before','dead_total','live_after',
               'unauthorized_dead','fault_live','unknown','quiet_samples','quiet_ns','warmup_dead','warmup_live','warmup_new'}
    for line in lines:
        parts = line.split(' ');name = parts[0]
        if name.startswith('FAIL_REVOKE_RECONNECT_WITNESS'):
            reject_native_failure_line(line)
        protocol_require(name in WITNESS_FIELDS and name not in rows and len(rows) < len(order) and name == order[len(rows)], 'native line/order/repetition differs')
        values = {}
        for token in parts[1:]:
            protocol_require(token.count('=') == 1, 'native key=value token required')
            key,value = token.split('=',1)
            protocol_require(key not in values and value != '', 'duplicate/empty native field')
            if key in numeric:
                protocol_require(re.fullmatch(r'0|[1-9][0-9]{0,19}',value) is not None, 'canonical bounded decimal required')
                value = int(value)
            values[key] = value
        protocol_require(tuple(values) == WITNESS_FIELDS[name] and all(values[key] == value for key,value in fixed.items()), 'native fixed identity/schema/order differs')
        for key,value in values.items():
            if key.endswith('_gid'):
                protocol_require(isinstance(value,str) and re.fullmatch(r'[0-9a-f]{32}',value) is not None and value != '0'*32, 'native complete nonzero16-byte GID required')
        gids = [value for key,value in values.items() if key.endswith('_gid')]
        protocol_require(len(gids) == len(set(gids)), 'native endpoint GIDs are not independent')
        if rows:
            first = rows[order[0]]
            protocol_require(all(values[key] == first[key] for key in WITNESS_COMMON), 'native own context/endpoint changed')
        if len(rows) >= 2:
            protocol_require(all(values[key] == rows[order[1]][key] for key in OLD_GIDS), 'old native publisher proof changed')
        if name == order[1]:
            protocol_require((values['dead_publishers'],values['live_publishers']) == (1,1), 'old exact-one publishers required')
        elif name == order[2]:
            protocol_require((values['dead_before'],values['live_before']) == (1,1), 'two independent baseline deliveries required')
        elif name == order[3]:
            protocol_require((values['dead_total'],values['live_before'],values['unauthorized_dead']) == (1,1,0), 'revoked count must be frozen at one')
        elif name in order[4:]:
            protocol_require((values['dead_publishers'],values['live_publishers'],values['dead_total'],values['live_before']) == (0,1,1,1), 'fresh native publisher/dead-resource snapshot differs')
            protocol_require(values['live_after'] == (0 if name == order[4] else 1), 'fresh surviving delivery count differs')
            if name in order[5:]:
                protocol_require(values['new_live_pub_gid'] == rows[order[4]]['new_live_pub_gid'], 'fresh native publisher GID changed')
            if name == order[6]:
                protocol_require((values['unauthorized_dead'],values['fault_live'],values['unknown']) == (0,0,0) and
                                 30 <= values['quiet_samples'] <= 10000 and 2000000000 <= values['quiet_ns'] <= 10000000000 and
                                 all(0 <= values[key] <= 1 for key in ('warmup_dead','warmup_live','warmup_new')), 'independent native quiet/no-side-effect contract differs')
        rows[name] = values
    protocol_require(not complete or tuple(rows) == order, 'complete native lifecycle proof missing')
    return rows


def parse_guest_markers(text, complete=True):
    """Allow ordinary diagnostics but refuse malformed reserved Guest markers."""
    protocol_require(type(complete) is bool and isinstance(text,str) and len(text) <= 16*1024**2 and '\r' not in text,
                     'bounded raw LF Guest text required')
    protocol_require(not complete or bool(text) and text.endswith('\n'), 'complete Guest log must end LF')
    lines = text.split('\n')[:-1]
    prefixes = ('REVOKE_ENDPOINT','BEFORE_MESSAGES','REVOKED_','GATEWAY_FAULT','RECONNECT_ENDPOINT','AFTER_MESSAGE','PASS_REVOKE','FAIL_REVOKE')
    selected = [line for line in lines if line.startswith(prefixes)]
    protocol_require(len(selected) <= len(GUEST_MARKERS) and tuple(selected) == GUEST_MARKERS[:len(selected)], 'Guest marker content/order/repetition differs')
    protocol_require(not complete or tuple(selected) == GUEST_MARKERS, 'complete Guest lifecycle assertions missing')
    return selected


def parse_gateway_audit(text, expected_pid, expected_uid, complete=True):
    """Keep strict native request identity and order; never infer a new session.

    Startup preflight legitimately contributes an additional resource-free
    session.bind. The actual Guest session is selected by publish and revoke
    requests, not by counting every handshake in the old Gateway log.
    """
    protocol_require(type(expected_pid) is int and expected_pid > 0 and type(expected_uid) is int and expected_uid >= 0,
                     'exact Worker principal required for audit')
    protocol_require(type(complete) is bool and isinstance(text,str) and len(text) <= 16*1024**2 and '\r' not in text,
                     'bounded raw native audit text required')
    protocol_require(not complete or bool(text) and text.endswith('\n'), 'final native audit must end LF')
    rows = []
    for line in text.split('\n')[:-1]:
        if not line.startswith('ros_audit'): continue
        parts = line.split(' ');protocol_require(parts[0] == 'ros_audit', 'malformed reserved native audit prefix')
        values = {}
        for token in parts[1:]:
            protocol_require(token.count('=') == 1, 'native audit key=value required')
            key,value = token.split('=',1);protocol_require(key not in values, 'duplicate native audit key')
            if key != 'alias': protocol_require(value != '', 'empty native audit field')
            if key in ('timestamp_ns','backend','worker_uid','worker_pid','session','error'):
                protocol_require(re.fullmatch(r'0|[1-9][0-9]{0,19}',value) is not None, 'native audit decimal differs')
                value = int(value)
            values[key] = value
        protocol_require(set(values) == {'timestamp_ns','backend','worker_uid','worker_pid','session','operation','alias','error'} and
                         (values['backend'],values['worker_pid'],values['worker_uid']) == (2,expected_pid,expected_uid) and
                         values['timestamp_ns'] > 0 and values['session'] > 0, 'native audit exact principal/session differs')
        protocol_require(values['operation'] in ('session.bind','topic.publish','capability.revoke','capability.close') and
                         (values['alias'] == 'target' if values['operation'] == 'session.bind' else values['alias'] in ('deadPub','livePub')) and
                         values['error'] == 0, 'unexpected native request/alias/error')
        rows.append(values)
        protocol_require(len(rows) <= 32, 'bounded lifecycle native request count required')
    return rows


def validate_sessions(old_text, new_text, prefix_before_A, worker_pid, worker_uid):
    """Prove old local revocation and a distinct native session before A."""
    old = parse_gateway_audit(old_text,worker_pid,worker_uid)
    new = parse_gateway_audit(new_text,worker_pid,worker_uid)
    prefix = parse_gateway_audit(prefix_before_A,worker_pid,worker_uid)
    protocol_require(new_text.startswith(prefix_before_A), 'actual before-A log is not a retained final-log prefix')
    old_requests = [row for row in old if row['operation'] != 'session.bind']
    protocol_require([(row['operation'],row['alias']) for row in old_requests] ==
                     [('topic.publish','deadPub'),('topic.publish','livePub'),('topic.publish','deadPub'),('topic.publish','livePub'),('capability.revoke','deadPub')],
                     'old native baseline/revocation request sequence differs')
    old_session = old_requests[0]['session']
    protocol_require(all(row['session'] == old_session for row in old_requests) and
                     any(row['session'] == old_session and row['operation'] == 'session.bind' for row in old), 'old Guest session bind/revoke association missing')
    protocol_require([row['operation'] for row in new] == ['session.bind','topic.publish','topic.publish','capability.close'] and
                     [row['alias'] for row in new] == ['target','livePub','livePub','livePub'] and
                     [row['operation'] for row in prefix] == ['session.bind','topic.publish'] and [row['alias'] for row in prefix] == ['target','livePub'],
                     'new Gateway revived an unauthorized alias or before-A proof differs')
    new_session = new[0]['session']
    protocol_require(new_session != old_session and all(row['session'] == new_session for row in new+prefix), 'fresh native session was not actually established')
    protocol_require(prefix == new[:len(prefix)], 'before-A session admission is not exact native log prefix')
    return {'old':old_session,'new':new_session,'old_requests':old,'new_requests':new,'before_A':prefix}


def valid_identity(value):
    return (isinstance(value,dict) and isinstance(value.get('path'),str) and value['path'].startswith('/') and
            all(type(value.get(key)) is int and value[key] > 0 for key in ('bytes','device','inode')) and
            all(type(value.get(key)) is int for key in ('elf_class','elf_data','e_machine')) and
            value.get('elf_class') == 2 and value.get('elf_data') == 1 and value.get('e_machine') == 183 and
            isinstance(value.get('sha256'),str) and re.fullmatch(r'[0-9a-f]{64}',value['sha256']) is not None)


def validate_combined_completion(row, expected_incarnation=None):
    """Fail closed on the deterministic combined acceptance record.

    This is source-model friendly. The live driver constructs it only from
    independently opened inputs, qualified products, verified public signed
    XML/certificates, actual owned mappings, exact marker parsers and cleanup.
    A synthetic row passing this predicate is not native behavior evidence.
    """
    protocol_require(isinstance(row,dict) and row.get('kind') == KIND and type(row.get('format_version')) is int and row.get('format_version') == 1 and
                     row.get('mode') == 'execute', 'exact combined evidence version/mode required')
    protocol_require(isinstance(row.get('source_manifest_sha256'),str) and
                     re.fullmatch(r'[0-9a-f]{64}',row['source_manifest_sha256']) is not None, 'current full frozen source manifest required')
    protocol_require(isinstance(row.get('qemu_sha256'),str) and re.fullmatch(r'[0-9a-f]{64}',row['qemu_sha256']) is not None,
                     'exact selected host QEMU identity required')
    for key in ('source_preserved','sdk_preserved','driver_inputs_preserved','immutable_inputs_preserved',
                'qualified_inputs_preserved','products_preserved'):
        protocol_require(row.get(key) is True, 'required preservation proof '+key)
    group = row.get('cgroup',{})
    before,after = group.get('before'),group.get('after')
    if expected_incarnation is None:
        # This default preserves the independent historical source-model row;
        # the live driver below never calls the predicate without its CLI pin.
        selected=(EXPECTED_LEAF_DEVICE,EXPECTED_LEAF_INODE,EXPECTED_BOOT)
        path_matches=isinstance(before,dict) and isinstance(before.get('path'),str)
    else:
        protocol_require(isinstance(expected_incarnation,dict) and set(expected_incarnation)=={'device','inode','boot_id','path'} and
            all(type(expected_incarnation.get(key)) is int and expected_incarnation[key]>0 for key in ('device','inode')) and
            isinstance(expected_incarnation.get('boot_id'),str) and re.fullmatch(r'[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}',expected_incarnation['boot_id']) is not None and
            isinstance(expected_incarnation.get('path'),str) and expected_incarnation['path'].startswith('/'), 'explicit complete cgroup selection required')
        selected=tuple(expected_incarnation[key] for key in ('device','inode','boot_id'))
        path_matches=isinstance(before,dict) and before.get('path')==expected_incarnation['path']
    protocol_require(isinstance(before,dict) and before == after and group.get('same_incarnation') is True and
                     (before.get('device'),before.get('inode'),before.get('boot_id')) == selected and path_matches,
                     'current exact inherited leaf/boot required')
    protocol_require(group.get('memory_max') == MEMORY_MAX and group.get('swap_max') == '0' and
                     group.get('readable_ancestors') is True and group.get('zero_oom') is True and
                     isinstance(group.get('membership'),list) and len(group['membership']) == 1,
                     'current64GiB zero-swap readable ancestor OOM proof required')
    signed = row.get('signed',{})
    protocol_require(signed.get('status') == 'PASS' and signed.get('keystore') == KEYSTORE and
                     signed.get('subjects') == {'gateway':'CN=/gateway','witness':'CN=/witness'},
                     'signed exact two-enclave certificate association required')
    for key in ('certificate_CA_verified','permissions_signatures_verified','governance_signatures_verified',
                'key_cert_public_match','default_deny','exact_topic_permissions','keystore_preserved_before_removal'):
        protocol_require(signed.get(key) is True, 'required signed proof '+key)
    protocol_require(all(isinstance(signed.get(key),str) and re.fullmatch(r'[0-9a-f]{64}',signed[key]) is not None
                         for key in ('policy_sha256','closure_sha256')), 'policy and complete owned credential closure required')
    context = row.get('native_context',{})
    protocol_require(context.get('witness') == {'attested':True,'enclave':'/witness','domain':DOMAIN,
                     'rmw':'rmw_fastrtps_cpp','security':'Enforce','root':KEYSTORE+'/enclaves/witness'},
                     'witness actual public used-context contract required')
    protocol_require(context.get('gateway') == {'source_selected':True,'enclave':'/gateway','domain':DOMAIN,
                     'rmw':'rmw_fastrtps_cpp','security':'Enforce','root':KEYSTORE+'/enclaves/gateway',
                     'environment_explicit':True,'direct_used_root_observed':False},
                     'Gateway source-selected Enforce boundary required')
    protocol_require(context['witness']['attested'] is True and context['gateway']['source_selected'] is True and
                     context['gateway']['environment_explicit'] is True and context['gateway']['direct_used_root_observed'] is False,
                     'context attestation/provenance leaves require exact booleans')
    products,roles = row.get('products',{}),row.get('roles',{})
    protocol_require(set(products) == {'worker','gateway','witness'} and set(roles) == set(ROLE_LABELS), 'three immutable products and four sequential process roles required')
    births = {}
    for role,label in ROLE_LABELS.items():
        product = products['gateway' if role.startswith('gateway_') else role]
        process = roles[role]
        protocol_require(product.get('qualified') is True and valid_identity(product.get('origin')) and valid_identity(product.get('snapshot')) and
                         product['origin']['sha256'] == product['snapshot']['sha256'] and
                         (product['origin']['device'],product['origin']['inode']) != (product['snapshot']['device'],product['snapshot']['inode']),
                         'separate exact qualified snapshot '+role)
        protocol_require(type(process.get('pid')) is int and process['pid'] > 0 and type(process.get('starttime_ticks')) is int and process['starttime_ticks'] > 0 and
                         process.get('membership') == group['membership'] and process.get('mapped_product') == product['snapshot'], 'exact mapped process birth '+role)
        protocol_require(process.get('live_before_C' if role == 'gateway_new' else 'live_before_G') is True, 'live phase-bound role '+role)
        proof = process.get('proof',{});actual,sample = proof.get('actual_stopped_ARM',{}),proof.get('target_proof',{})
        protocol_require(proof.get('current_mapping_check') is True and (actual.get('pid'),actual.get('starttime_ticks')) ==
                         (process['pid'],process['starttime_ticks']) and actual.get('state') in ('T','t') and sample.get('live_cgroup_procs') is True and
                         sample.get('membership') == group['membership'] and label in sample.get('mapped_arm_targets',[]) and
                         sample.get('kernel_exe_elf',{}).get('e_machine') == 62 and sample.get('kernel_exe_elf',{}).get('sha256') == row['qemu_sha256'] and
                         (sample.get('pid'),sample.get('starttime_ticks')) == (process['pid'],process['starttime_ticks']) and
                         proof.get('ancestry') == sample.get('ancestry') and bool(proof.get('ancestry')), 'actual stopped ARM/QEMU role proof '+role)
        births[role] = (process['pid'],process['starttime_ticks'])
    protocol_require(len(set(births.values())) == 4 and all(len({births[role][0] for role in phase}) == 3 for phase in
                     (('worker','witness','gateway_old'),('worker','witness','gateway_new'))) and
                     roles['witness'].get('pre_init_mapping') is True and all(roles[key].get('pre_init_mapping') is False for key in ('worker','gateway_old','gateway_new')),
                     'independent simultaneous actors and distinct sequential Gateway births required')
    markers = row.get('markers',{})
    parsed = parse_witness_markers(markers.get('witness_text',''),roles['witness']['pid'])
    parse_guest_markers(markers.get('guest_text',''))
    protocol_require(markers.get('witness') == parsed, 'native marker evidence differs from raw captured bytes')
    sessions = row.get('sessions',{})
    actual_sessions = validate_sessions(sessions.get('old_text',''),sessions.get('new_text',''),sessions.get('before_A_text',''),
                                       roles['worker']['pid'],row.get('worker_uid'))
    protocol_require(sessions.get('parsed') == actual_sessions, 'actual old/fresh authenticated session evidence differs')
    fault = row.get('fault',{})
    protocol_require(fault.get('intentional') is True and fault.get('signal') == 9 and type(fault.get('signal')) is int and
                     fault.get('old_pid') == roles['gateway_old']['pid'] and fault.get('old_starttime_ticks') == roles['gateway_old']['starttime_ticks'] and
                     fault.get('all_old_owned_births_retired') is True and fault.get('root_reaped') is True and fault.get('socket_removed_after_retirement') is True and
                     type(fault.get('returncode')) is int and fault['returncode'] == -9 and fault.get('same_worker_live') is True and fault.get('same_witness_live') is True,
                     'exact owned old Gateway kill/reap before dead-peer call required')
    fault_births = fault.get('observed_old_births',[])
    protocol_require(isinstance(fault_births,list) and bool(fault_births) and all(isinstance(item,dict) and type(item.get('pid')) is int and item['pid'] > 0 and
                     type(item.get('starttime_ticks')) is int and item['starttime_ticks'] > 0 and item.get('pidfd_signal_sent') is True and item.get('live_cgroup_verified') is True
                     for item in fault_births) and births['gateway_old'] in {(item['pid'],item['starttime_ticks']) for item in fault_births}, 'retained owned fault-birth proof missing')
    old_socket,new_socket = fault.get('old_socket',{}),fault.get('new_socket',{})
    protocol_require(all(type(item.get(key)) is int and item[key] > 0 for item in (old_socket,new_socket) for key in ('device','inode')) and
                     old_socket.get('path') == new_socket.get('path') and isinstance(old_socket.get('path'),str) and
                     (old_socket['device'],old_socket['inode']) != (new_socket['device'],new_socket['inode']), 'new owned IPC socket inode required')
    gates = row.get('gates',[])
    expected = [('witness','I'),('worker','G'),('worker','B'),('worker','R'),('witness','N'),('worker','F'),('worker','C'),('worker','A'),('worker','Q'),('witness','F')]
    protocol_require(isinstance(gates,list) and [(item.get('role'),item.get('byte')) for item in gates] == expected, 'closed lifecycle gate transcript required')
    predecessors = ['ARM_WITNESS_PREINIT_STOP','READY_REVOKE_WITNESS','MATCHED_REVOKE_OLD+REVOKE_ENDPOINTS_CREATED',
                    'BASELINE_REVOKE_WITNESS+BEFORE_MESSAGES_SENT','REVOKED_STALE9_REOPEN125',
                    'FROZEN_REVOKE_WITNESS+OLD_GATEWAY_REAPED','NEW_GATEWAY_READY+GATEWAY_FAULT107',
                    'MATCHED_REVOKE_NEW+RECONNECT_ENDPOINT_CREATED+NEW_SESSION_BIND',
                    'DELIVERED_REVOKE_NEW+AFTER_MESSAGE_SENT','PASS_REVOKE_RECONNECT_GUEST']
    for index,item in enumerate(gates):
        role = roles[item['role']]
        protocol_require((item.get('pid'),item.get('starttime_ticks')) == (role['pid'],role['starttime_ticks']) and
                         item.get('current_birth_mapping_verified') is True and item.get('sent') is True and item.get('prior_marker') == predecessors[index],
                         'live birth/exact prior marker gate association required')
    protocol_require(gates[5].get('old_gateway_retired') is True and gates[6].get('new_gateway_live') is True and
                     type(gates[7].get('new_session')) is int and gates[7]['new_session'] == actual_sessions['new'] and
                     gates[8].get('native_survivor_delivered') is True and
                     type(gates[9].get('guest_exit')) is int and gates[9]['guest_exit'] == 0, 'physical fault, fresh session and native final release association required')
    commands = row.get('commands',{})
    protocol_require(all(type(commands.get(key)) is int and commands[key] == 0 for key in ('worker_exit','witness_exit','generation_exit')) and
                     type(commands.get('gateway_old_exit')) is int and commands['gateway_old_exit'] == -9 and
                     bool(commands.get('signed_verification_exits')) and all(type(value) is int and value == 0 for value in commands['signed_verification_exits']) and
                     (type(commands.get('gateway_new_exit_after_worker')) is int and commands['gateway_new_exit_after_worker'] == 0 or
                      commands.get('gateway_new_exit_after_worker') == 'OWNED_CLEANUP'), 'actual bounded lifecycle subprocess results required')
    terminal = commands.get('gateway_new_terminal',{})
    log_binding = terminal.get('log_binding',{})
    protocol_require(type(terminal.get('returncode')) is int and
                     (commands['gateway_new_exit_after_worker'] == 'OWNED_CLEANUP' and terminal.get('status') == 'OWNED_CLEANUP' and terminal['returncode'] in (0,-15,-9) or
                      type(commands['gateway_new_exit_after_worker']) is int and commands['gateway_new_exit_after_worker'] == 0 and terminal.get('status') == 'PASS' and terminal['returncode'] == 0),
                     'cleanup-owned replacement must have an actual accepted terminal result')
    protocol_require(isinstance(log_binding.get('path'),str) and log_binding['path'].startswith('/') and
                     all(type(log_binding.get(key)) is int and log_binding[key] > 0 for key in ('device','inode','bytes')) and
                     log_binding['bytes'] == len(sessions['new_text'].encode('utf-8')) and
                     log_binding.get('sha256') == hashlib.sha256(sessions['new_text'].encode('utf-8')).hexdigest(),
                     'final replacement native audit log must bind complete captured bytes')
    denial = row.get('fixture_assertions',{})
    protocol_require(denial == {'stale_before':9,'reopen_before':125,'dead_peer':107,'stale_after':9,'reopen_after':125,'surviving_publish':0} and
                     all(type(value) is int for value in denial.values()), 'exact source-bound Guest errno assertions required')
    cleanup = row.get('cleanup',{})
    protocol_require(cleanup.get('owned_processes') == [] and cleanup.get('mounts') == [] and cleanup.get('socket_absent') is True and
                     cleanup.get('secrets_removed') is True and cleanup.get('domain_lock_removed') is True, 'complete bounded owned cleanup required')
    protocol_require(row.get('G09_complete') is False, 'bounded lifecycle subcase does not close every original G09 requirement')
    return True


def cmake():
    """One original source, explicit full dependencies and pre-init ARM barrier."""
    packages = ('rclcpp','rcl','rcutils','rmw','std_msgs','rosidl_typesupport_cpp')
    lines = ['cmake_minimum_required(VERSION 3.16)', 'project(revoke_signed_witness LANGUAGES CXX)',
             'set(CMAKE_CXX_STANDARD 20)', 'set(CMAKE_CXX_STANDARD_REQUIRED ON)',
             'find_package(ament_cmake REQUIRED)', 'find_package(fastrtps CONFIG REQUIRED)']
    lines += ['find_package('+name+' REQUIRED)' for name in packages]
    lines += ['add_executable('+WITNESS+' /work/'+WITNESS_SOURCE+')',
              'target_include_directories('+WITNESS+' PRIVATE /work/third-parties/fast_io/include)',
              'target_compile_options('+WITNESS+' PRIVATE -O1 -g0 -UNDEBUG -std=gnu++20 -MD -MF /tmp/build/CMakeFiles/'+WITNESS+'.dir/revoke-required-full-sdk.d -include /tmp/evidence-stop.hpp)',
              'target_link_options('+WITNESS+' PRIVATE -v)',
              'ament_target_dependencies('+WITNESS+' '+' '.join(packages)+')',
              'target_link_libraries('+WITNESS+' fastrtps pthread)']
    return '\n'.join(lines)+'\n'


def compile_recipe(database, verbose):
    """Require actual singleton GCC13 command and its final effective MF output."""
    source = '/work/'+WITNESS_SOURCE
    compiler = '/usr/bin/aarch64-linux-gnu-g++-13'
    protocol_require(isinstance(database,list) and len(database) == 1 and
                     posixpath.normpath(database[0]['file']) == source and database[0]['directory'] == '/tmp/build',
                     'exact singleton native compile database required')

    def options(argv, final_mf=True):
        protocol_require(argv and argv[0] == compiler and argv.count('-c') == 1 and
                         argv[argv.index('-c')+1:] == [source], 'original singleton source/compiler required')
        cleaned,outputs = [],[]
        standard = optimization = debug = ndebug = None
        index = 0
        while index < len(argv):
            value = argv[index]
            protocol_require(value not in ('-M','-MM','-MMD','-E','-S','-fsyntax-only','-MG','-MP','-Xpreprocessor') and
                             not value.startswith(('@','-Wp,','-imacros')), 'opaque or partial dependency recipe refused')
            if value in ('-MD','-MF','-MT','-MQ'):
                if value != '-MD':
                    index += 1
                    protocol_require(index < len(argv), 'missing native dependency operand')
                    if value == '-MF': outputs.append(argv[index])
            elif value.startswith(('-MF','-MT','-MQ')):
                if value.startswith('-MF'): outputs.append(value[3:])
            elif value in ('-D','-U'):
                cleaned.append(value);index += 1
                protocol_require(index < len(argv), 'missing macro operand')
                cleaned.append(argv[index])
                if argv[index].split('=',1)[0] == 'NDEBUG': ndebug = value == '-D'
            else:
                cleaned.append(value)
                if value.startswith('-std='): standard = value
                if value.startswith('-O'): optimization = value
                if value.startswith('-g'): debug = value
                if value.startswith(('-D','-U')) and value[2:].split('=',1)[0] == 'NDEBUG': ndebug = value.startswith('-D')
            index += 1
        explicit = '/tmp/build/CMakeFiles/'+WITNESS+'.dir/revoke-required-full-sdk.d'
        protocol_require('-MD' in argv and explicit in outputs and (not final_mf or outputs) and
                         (standard,optimization,debug,ndebug) == ('-std=gnu++20','-O1','-g0',False),
                         'effective CPP20/O1/g0/UNDEBUG/full-MD/explicit-MF required')
        protocol_require(cleaned.count('-include') == 1 and cleaned[cleaned.index('-include')+1] == '/tmp/evidence-stop.hpp',
                         'exact generated constructor barrier include required')
        return cleaned,posixpath.normpath(posixpath.join('/tmp/build',outputs[-1]))

    expected,_ = options(database[0].get('arguments') or shlex.split(database[0]['command']),False)
    selected = []
    for number,line in enumerate(verbose.splitlines(),1):
        if compiler not in line or source not in line: continue
        argv = shlex.split(line)
        if len(argv)>3 and argv[:3] == ['cd','/tmp/build','&&']: argv = argv[3:]
        if not argv or argv[0] != compiler or '-c' not in argv: continue
        actual,depfile = options(argv)
        protocol_require(actual == expected and depfile.startswith('/tmp/build/CMakeFiles/'+WITNESS+'.dir/') and
                         depfile.endswith('.d') and argv.count('-o') == 1, 'actual command/final dependency target differs')
        object_path = posixpath.normpath(posixpath.join('/tmp/build',argv[argv.index('-o')+1]))
        protocol_require(object_path.startswith('/tmp/build/CMakeFiles/'+WITNESS+'.dir/') and object_path.endswith('.o'),
                         'native object path escapes singleton metadata')
        selected.append({'argv':argv,'working_directory':'/tmp/build','verbose_line':number,
                         'effective_mf':depfile,'compiled_object_guest_path':object_path})
    protocol_require(len(selected) == 1, 'one actual successful verbose singleton compile required')
    return selected[0]


def native_link_closure(link, sdk):
    """Pin explicit native link-library inputs inside the full immutable SDK."""
    argv = shlex.split(link)
    protocol_require('-v' in argv and not any(value.startswith(('@','-Wl,@')) for value in argv),
                     'transparent verbose native link recipe required')
    libraries = {}
    for value in argv:
        if not value.startswith('/') or not re.search(r'\.(?:a|so(?:\.[0-9.]+)?)$',value): continue
        protocol_require(value.startswith(('/usr/','/lib/','/lib64/','/opt/')), 'link library is outside canonical native SDK')
        path = rooted(sdk,value)
        protocol_require(path.resolve(strict=True).is_relative_to(sdk), 'native link library escapes SDK')
        libraries[value] = read_file_binding(path)
    protocol_require(libraries and any('/libfastrtps.so' in value for value in libraries) and
                     any('/librclcpp.so' in value for value in libraries) and
                     any('/libstd_msgs__rosidl_typesupport_cpp.so' in value for value in libraries),
                     'explicit original FastDDS/rclcpp/String type-support link closure missing')
    return libraries


def topic_policy():
    """Two independent explicit internal grants; signed DDS policy grants none."""
    connection = {'node-name':'uwvm_ros2_gateway_probe','namespace':'/uwvm_revoke_test','domain-id':str(DOMAIN),
                  'rmw-implementation':'rmw_fastrtps_cpp','security-mode':'enforce',
                  'keystore':KEYSTORE,'enclave':'/gateway'}
    common = {'max-message-bytes':'64','max-rate-hz':'100','burst':'8','max-bytes-per-second':'65536',
              'max-queue-bytes':'4096','overflow-policy':'drop-oldest','validator':'none','audit':'allow-and-deny',
              'audit-max-rate-hz':'100','audit-burst':'16','audit-max-records':'10000','audit-max-bytes':'1048576',
              'max-inflight':'1','qos.reliability':'reliable','qos.durability':'volatile','qos.history':'keep-last',
              'qos.depth':'8','qos.deadline':'infinite','qos.lifespan':'infinite','qos.liveliness':'automatic',
              'qos.liveliness-lease-duration':'infinite'}
    grants = [{'alias':alias,'operation':'topic.publish','resource':'/uwvm_revoke_test/'+resource,'type':NATIVE_TYPE,
               'identity':NATIVE_IDENTITY,'limits':{**common,'max-publishers':'1'}}
              for alias,resource in (('deadPub','dead'),('livePub','live'))]
    return connection,grants


def worker_arguments(worker, connection, grants, socket_path, wasm):
    argv = [str(worker),'--ros2-single-create','main','--ros2-single-enable','main']
    for key,value in {**connection,'gateway-socket':str(socket_path),'gateway-uid':str(os.getuid()),
                      'connect-timeout-ms':'60000','audit-deny-max-rate-hz':'100','audit-deny-burst':'16',
                      'audit-deny-max-records':'10000','audit-deny-max-bytes':'1048576'}.items():
        argv += ['--ros2-single-'+key,'main',value]
    for grant in grants:
        argv += ['--ros2-single-grant','main',*[grant[key] for key in ('alias','operation','resource','type','identity')]]
        for key,value in grant['limits'].items(): argv += ['--ros2-single-set','main',grant['alias'],key,value]
    return argv+['--wasip1-single-create','main','--wasip1-single-enable','main',
                 '--wasip1-single-noinherit-system-environment','main',
                 '--wasip1-single-grant','main','monotonic-clock',
                 '--wasm-set-main-module-name','main','--run',str(wasm)]


def gateway_configuration(worker_pid, connection, grants):
    lines = ['backend ros2','worker-uid '+str(os.getuid()),'worker-pid '+str(worker_pid),
             'socket /tmp/control/ros2.sock','timeout-ms 60000','max-sessions 2',
             'audit-max-records-per-second 100','audit-max-records 10000','audit-max-bytes 1048576']
    lines += ['connection '+key+' '+value for key,value in connection.items()]
    for grant in grants:
        lines += ['grant '+' '.join(grant[key] for key in ('alias','operation','resource','type','identity'))]
        lines += ['set '+grant['alias']+' '+key+' '+value for key,value in grant['limits'].items()]
    return '\n'.join(lines)+'\n'


def native_environment(enforce=False):
    """Literal allowlist: no parent security/discovery/loader/profile inheritance."""
    result = {'PATH':'/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin',
              'LANG':'C.UTF-8','LC_ALL':'C.UTF-8','HOME':'/root','PYTHONDONTWRITEBYTECODE':'1',
              'ROS_DOMAIN_ID':str(DOMAIN),'RMW_IMPLEMENTATION':'rmw_fastrtps_cpp',
              'ROS_SECURITY_ENABLE':'true' if enforce else 'false','ROS_LOG_DIR':'/tmp/ros-logs'}
    if enforce: result.update(ROS_SECURITY_STRATEGY='Enforce',ROS_SECURITY_KEYSTORE=KEYSTORE)
    return result


def permission_contract(path, role, subject):
    """Inspect verified signed XML, never an unsigned helper's claimed verdict."""
    protocol_require(role in ('gateway','witness') and subject == 'CN=/'+role,
                     'exact signed fixture role/subject required')
    raw,binding = read_file_binding(path,limit=1024*1024,retain=True)
    protocol_require(b'<!DOCTYPE' not in raw and b'<!ENTITY' not in raw, 'signed permission XML must have no external entities')
    root = ET.fromstring(raw)
    grants = root.findall('./permissions/grant')
    protocol_require(len(grants) == 1 and grants[0].findtext('subject_name') == subject and
                     grants[0].findtext('default') == 'DENY', 'signed default-DENY subject association differs')
    grant = grants[0]
    rules = grant.findall('allow_rule')
    protocol_require(rules and all([value.text for value in rule.findall('./domains/id')] == [str(DOMAIN)] and
                         not rule.findall('./domains/id_range') for rule in rules),
                     'signed native grants must name only exact domain75')
    user_topics = {'rt/uwvm_revoke_test/dead','rt/uwvm_revoke_test/live'}
    publish = [item.text for item in grant.findall('./allow_rule/publish/topics/topic')]
    subscribe = [item.text for item in grant.findall('./allow_rule/subscribe/topics/topic')]
    expected_pub = user_topics if role == 'gateway' else set()
    expected_sub = user_topics if role == 'witness' else set()
    protocol_require(set(publish).issubset(expected_pub|{'ros_discovery_info'}) and
                     set(subscribe).issubset(expected_sub|{'rt/parameter_events','ros_discovery_info'}) and
                     expected_pub.issubset(publish) and expected_sub.issubset(subscribe) and
                     len(publish)+len(subscribe) <= 128 and all(isinstance(value,str) and len(value) <= 512 for value in publish+subscribe),
                     'signed native lifecycle role permits an unknown, wildcard or wrong-direction resource')
    return {'subject':subject,'publish':publish,'subscribe':subscribe,'verified_xml':binding}


def governance_contract(path):
    raw,binding = read_file_binding(path,limit=1024*1024,retain=True)
    protocol_require(b'<!DOCTYPE' not in raw and b'<!ENTITY' not in raw, 'signed governance XML must have no external entities')
    root = ET.fromstring(raw)
    rules = root.findall('./domain_access_rules/domain_rule')
    protocol_require(rules and all(rule.findtext('allow_unauthenticated_participants') == 'false' for rule in rules),
                     'signed governance must refuse unauthenticated participants')
    for rule in rules:
        protocol_require([value.text for value in rule.findall('./domains/id')] == [str(DOMAIN)] and
                         not rule.findall('./domains/id_range'), 'signed governance must bind only exact domain75')
        topics = rule.findall('./topic_access_rules/topic_rule')
        for name in ('rt/uwvm_revoke_test/dead','rt/uwvm_revoke_test/live'):
            selected = [value for value in topics if fnmatch.fnmatchcase(name,value.findtext('topic_expression',''))]
            protocol_require(selected and all(value.findtext('enable_read_access_control') == 'true' and
                             value.findtext('enable_write_access_control') == 'true' for value in selected),
                             'signed application Topic rules must enforce read/write access control')
    return binding


def credential_ledger(directory, materialize=False):
    """Owned full keystore closure; hash private bytes without archiving them."""
    directory = Path(directory)
    protocol_require(directory.is_dir() and not directory.is_symlink(), 'fresh owned keystore directory required')
    paths = sorted(directory.rglob('*'))
    protocol_require(len(paths) <= 512, 'bounded temporary signed keystore closure required')
    if materialize:
        directory.chmod(0o700)
        for path in paths:
            info = path.lstat()
            protocol_require(info.st_uid == os.geteuid(), 'generated credential is not task-owned')
            if stat.S_ISLNK(info.st_mode):
                selected = path.resolve(strict=True)
                protocol_require(selected.is_relative_to(directory.resolve(strict=True)) and selected.is_file(),
                                 'generated credential symlink escapes owned keystore')
                raw,binding = read_file_binding(selected,limit=2*1024**2,retain=True)
                fresh = path.with_name(path.name+'.revoke-materialized')
                with fresh.open('xb') as stream: stream.write(raw)
                protocol_require(read_file_binding(selected) == binding, 'generated public credential changed while materializing')
                fresh.chmod(0o600 if 'key' in path.name else 0o644);os.replace(fresh,path)
            elif stat.S_ISDIR(info.st_mode): path.chmod(0o700)
            elif stat.S_ISREG(info.st_mode): path.chmod(0o600 if 'key' in path.name else 0o644)
            else: raise RuntimeError('unsupported object in generated signed keystore')
    ledger = {}
    for path in sorted(directory.rglob('*')):
        info = path.lstat();relative = str(path.relative_to(directory))
        protocol_require(info.st_uid == os.geteuid() and not info.st_mode&0o022 and
                         not stat.S_ISLNK(info.st_mode), 'unsafe generated signed credential closure')
        if stat.S_ISDIR(info.st_mode):
            ledger[relative] = {'kind':'directory','device':info.st_dev,'inode':info.st_ino,'mode':stat.S_IMODE(info.st_mode)}
        else:
            protocol_require(stat.S_ISREG(info.st_mode) and ('key' not in path.name or not info.st_mode&0o077),
                             'private key must be owned regular mode0600')
            ledger[relative] = {'kind':'regular','mode':stat.S_IMODE(info.st_mode),'binding':read_file_binding(path,limit=2*1024**2)}
    required = {role+'/'+name for role in ('gateway','witness') for name in
                ('identity_ca.cert.pem','cert.pem','permissions_ca.cert.pem','governance.p7s','permissions.p7s','key.pem')}
    protocol_require({'enclaves/'+name for name in required}.issubset(ledger), 'both signed native enclaves require complete regular credential sets')
    return ledger


def growing_log(path):
    """Read a bounded append-only captured prefix without newline conversion."""
    descriptor = os.open(path,os.O_RDONLY|os.O_NONBLOCK)
    try:
        first = os.fstat(descriptor)
        protocol_require(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= 16*1024**2, 'bounded captured regular log required')
        left = first.st_size;pieces = []
        while left:
            piece = os.read(descriptor,min(left,1024*1024))
            protocol_require(bool(piece), 'captured log prefix was truncated during read')
            pieces.append(piece);left -= len(piece)
        last = os.fstat(descriptor)
        protocol_require((first.st_dev,first.st_ino) == (last.st_dev,last.st_ino) and last.st_size >= first.st_size,
                         'captured log descriptor was replaced/truncated')
        # Parent created this file once with O_EXCL and owns the only writers'
        # immutable subprocess descriptors. Active reads intentionally permit
        # appends; finalized logs use the strict same-descriptor input binder.
        return b''.join(pieces).decode('utf-8','strict')
    finally: os.close(descriptor)


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--mode',choices=('prepare','execute'),required=True)
    for name in ('source','manifest','rootfs','out','qemu','binfmt-evidence'):
        parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--container-started-at',required=True)
    parser.add_argument('--expected-cgroup',type=Path,required=True,
        help='Current independently inspected JSON with exactly path/device/inode/boot_id; historical defaults are never used for execution')
    execution = ('prepared','worker','worker-report','worker-config','gateway','gateway-report','wat2wasm')
    for name in execution: parser.add_argument('--'+name,type=Path)
    args = parser.parse_args()
    supplied = {name:getattr(args,name.replace('-','_')) is not None for name in execution}
    if args.mode == 'execute' and not all(supplied.values()): parser.error('execute requires all seven product/prepared/Worker/Gateway/WAT tool arguments')
    if args.mode == 'prepare' and any(supplied.values()): parser.error('prepare rejects execution-only arguments')
    source,sdk = args.source.resolve(strict=True),args.rootfs.resolve(strict=True)
    out = args.out.resolve()
    if any(out == path or out.is_relative_to(path) or path.is_relative_to(out) for path in (source,sdk)):
        parser.error('fresh output must be separate from frozen source/canonical SDK')
    if any(any(char.isspace() or char == '\\' for char in str(path)) for path in (source,sdk,out)):
        parser.error('source/mount/dependency proof requires unescaped absolute paths')
    out.mkdir(parents=True,mode=0o700,exist_ok=False)
    root,temporary = out/'rootfs',out/'native-temp';root.mkdir();temporary.mkdir()
    report = {'status':'RUNNING','kind':KIND,'format_version':1,'mode':args.mode,'source':str(source),'rootfs':str(sdk),
              'domain':DOMAIN,'container_started_at':args.container_started_at,'commands':[],'driver_inputs':{},
              'compilation_status':'NOT_PERFORMED','actual_guest_behavior':'NOT_PERFORMED','G09_complete':False,
              'scope':'Singleton native witness preparation' if args.mode == 'prepare' else
                      'One raw Wasm32 Guest: local revocation, owned Gateway fault, fresh authenticated session, surviving delivery and independently frozen revoked-topic count'}
    evidence = owned = None
    pins,children,mounts,tools,products = [],[],[],{},{}
    frozen = initial = sdk_before = aliases_before = before = lease_state = expected_incarnation = None
    control = domain_lock = None
    credentials = credentials_identity = None
    completion = architecture_result = None
    communication_roles = None
    status = 'FAIL'

    def save(): (out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    def file_pin(path):
        value = read_file_binding(path);pins.append(value);return value
    def json_pin(path,limit=64*1024**2):
        value,binding = read_json_binding(path,limit=limit);pins.append(binding);return value,binding
    def check_pins():
        protocol_require(pins_preserved(pins), 'immutable control/tool/product/source input changed')
    def checked(command):
        subprocess.run(list(map(str,command)),check=True,capture_output=True,timeout=30)
    def guard():
        protocol_require(cgroup_incarnation(evidence.leaf) == before and
                         (evidence.leaf/'memory.max').read_text().strip() == str(MEMORY_MAX) and
                         (evidence.leaf/'memory.swap.max').read_text().strip() == '0', 'current inherited cgroup/limit changed')
        protocol_require((evidence.leaf/'cpuset.cpus.effective').read_text().strip() == '0-31' and
                         int((evidence.leaf/'memory.current').read_text()) <= 60*1024**3, '64GiB cgroup reserve/cpuset admission failed')
        if lease_state:
            protocol_require(lease_state['path'].read_text() == lease_state['entry'] and
                             os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY') == lease_state['name'], 'parent fixed binfmt lease changed')
        owned.sample()
        if communication_roles is not None:
            active_worker,active_gateway = communication_roles
            if active_worker.poll() is None and active_gateway.poll() is not None:
                # Recheck the Worker after observing Gateway exit: its pidfd
                # can legitimately retire the Gateway concurrently with this
                # controller sample. An exit with a still-live Worker fails.
                protocol_require(active_worker.poll() is not None,
                                 'Gateway retired before its bound Guest process exited')
        for child,row in children:
            if child.poll() is not None: continue
            protocol_require(owned.rss(row['name']) <= (2 if row['name'] in ('configure','compile') else 8)*1024**3 and
                             Path(row['log']).stat().st_size <= 16*1024**2, 'owned subprocess RSS/log budget exceeded')
            protocol_require(time.monotonic() <= row['deadline'], 'owned bounded command deadline exhausted: '+row['name'])
    def native(arguments,witness=False):
        # The qualified Jazzy setup injects SUBNET even into a clean initial
        # environment. Keep the witness's strict final absence predicate and
        # remove only that known SDK default after setup, checking the result.
        # Other native tools and both Gateways retain their original script.
        script = 'source /opt/ros/jazzy/setup.bash'
        if witness:
            script += ' && unset ROS_AUTOMATIC_DISCOVERY_RANGE && [[ ! -v ROS_AUTOMATIC_DISCOVERY_RANGE ]]'
        return ['/usr/bin/chroot',str(root),'/bin/bash','--noprofile','--norc','-c',
                script+' && exec "$@"','revoke-signed-native',*map(str,arguments)]
    def start(name,command,required=(),timeout=60,stdin=False,enforce=False,environment=None):
        guard();check_pins();log = out/(name+'.log')
        with log.open('xb') as stream:
            child = subprocess.Popen(command,stdin=subprocess.PIPE if stdin else None,stdout=stream,stderr=subprocess.STDOUT,
                                     env=environment if environment is not None else native_environment(enforce),start_new_session=True)
        row = {'name':name,'command':command,'pid':child.pid,'starttime_ticks':None,
               'status':'RUNNING','log':str(log),'deadline':None,
               'required_arm_targets':list(required),'startup_status':'POPEN_RETAINED'}
        # Retain the exact Popen handle and RUNNING row before procfs/pidfd or
        # evidence calls can fail; cleanup must account for this root as well.
        children.append((child,row));report['commands'].append(row);save()
        identity = process_identity(child.pid);row['starttime_ticks'] = identity['starttime_ticks']
        owned.acquire(child.pid,name,[child.pid]);row['startup_status'] = 'OWNED_BIRTH_RECORDED'
        evidence.start(child,name+'/',command);row['startup_status'] = 'EVIDENCE_REGISTERED'
        row['deadline'] = time.monotonic()+timeout;save();return child,row
    def finish(child,row):
        while child.poll() is None: guard();time.sleep(.002)
        # Preserve the real terminal result before propagating a failed tool
        # invocation. A command that has exited must not remain RUNNING in the
        # durable report merely because the surrounding lane fails closed.
        row.update(status='FAIL' if child.returncode != 0 else 'EXITED_PENDING_ARCHITECTURE',
                   returncode=child.returncode,log_sha256=digest(row['log']))
        row['log_binding'] = file_pin(row['log'])
        if row['name'] == 'compile' and child.returncode != 0:
            report['compilation_status'] = 'FAILED'
        save()
        protocol_require(child.returncode == 0, 'actual native/tool/Guest failure: '+row['name'])
        evidence.check(row['required_arm_targets'],row['name']+'/')
        row.update(status='PASS');save()
    def execute(name,arguments,required=(),timeout=60,enforce=False):
        child,row = start(name,native(arguments),required,timeout,enforce=enforce);finish(child,row);return row
    def current_map(pid,label):
        return evidence.mapped(pid, evidence.targets[label])
    def mapped_stop(child,row,label,pre_stopped=False):
        until = time.monotonic()+20
        while child.poll() is None and time.monotonic()<until:
            guard()
            for record in list(owned.rows.values()):
                if record['label'] != row['name'] or not owned.check(record): continue
                actual = record['identity'];pid = actual['pid']
                if not current_map(pid,label): continue
                if not pre_stopped: owned.send(record,signal.SIGSTOP)
                deadline = time.monotonic()+2
                while owned.alive(record) and process_identity(pid)['state'] not in ('T','t') and time.monotonic()<deadline: time.sleep(.001)
                proof = owned_stopped_arm(evidence,child,row['name']+'/',row['starttime_ticks'],[label])
                if proof is None: continue
                stopped = proof['actual_stopped_ARM']
                protocol_require(stopped['pid'] == pid and stopped['starttime_ticks'] == actual['starttime_ticks'] and
                                 owned.check(record) and current_map(pid,label), 'stopped actual target identity changed')
                row['actual_ARM_mapping_before_resume'] = proof
                evidence.before_resume(pid, stopped)
                owned.send(record,signal.SIGCONT);save();return proof
            time.sleep(.002)
        raise RuntimeError('actual owned stopped ARM image proof missing: '+label)
    def snapshot(origin,destination,label):
        fact = require_arm(origin);file_pin(origin);shutil.copy2(origin,destination);destination.chmod(0o500)
        acquired = evidence.target(label,destination);file_pin(destination)
        protocol_require(acquired['sha256'] == fact['sha256'] and
                         (acquired['device'],acquired['inode']) != (fact['device'],fact['inode']) and require_arm(origin) == fact,
                         'immutable snapshot changed or shares compiler-output inode')
        return {'qualified':True,'origin':fact,'snapshot':acquired}
    def live_role(child,row,role,proof):
        """Recheck this retained process birth and target map, not a prior label."""
        actual = proof['actual_stopped_ARM']
        current = process_identity(actual['pid'])
        protocol_require(child.poll() is None and current['starttime_ticks'] == actual['starttime_ticks'] and
                         current_map(actual['pid'],ROLE_LABELS[role]), 'live role identity unavailable before gate')
        record = next((value for value in owned.rows.values() if value['identity']['pid'] == actual['pid'] and
                       value['identity']['starttime_ticks'] == actual['starttime_ticks'] and value['label'] == row['name']),None)
        protocol_require(record is not None and owned.check(record), 'gate recipient is not an owned live role')
        return {'pid':actual['pid'],'starttime_ticks':actual['starttime_ticks'],
                'membership':evidence.before['membership'],'current_birth_mapping_verified':True}
    def send_gate(child,row,role,byte,proof,previous,extra=None):
        guard();check_pins();actual = proof['actual_stopped_ARM'];live_role(child,row,role,proof)
        child.stdin.write(byte.encode('ascii'));child.stdin.flush()
        item = {'role':role,'byte':byte,'pid':actual['pid'],'starttime_ticks':actual['starttime_ticks'],
                'current_birth_mapping_verified':True,'sent':True,'prior_marker':previous,**(extra or {})}
        report.setdefault('gates',[]).append(item);save()
    def wait_marker(child,row,parser_function,key,pid=None):
        while child.poll() is None:
            guard();value = growing_log(row['log'])
            parsed = parser_function(value,pid,False) if pid is not None else parser_function(value,False)
            if key in parsed: return parsed
            time.sleep(.002)
        # A native process can finish before the loop samples its final line.
        # Parse that bounded raw prefix to preserve a fixed refusal diagnostic;
        # even a valid prefix cannot satisfy a gate once its owner has exited.
        value = growing_log(row['log'])
        parser_function(value,pid,False) if pid is not None else parser_function(value,False)
        raise RuntimeError('process ended before required marker: '+key)

    try:
        protocol_require(sys.platform == 'linux' and os.geteuid() == 0, 'BLOCKED root Linux private namespace required')
        protocol_require(os.readlink('/proc/self/ns/mnt') != os.readlink(f'/proc/{os.getppid()}/ns/mnt'), 'BLOCKED launch under unshare -m')
        protocol_require(Path(__file__).resolve() == source/'tests/ros_aarch64/run_revoke_reconnect.py', 'execute only exact frozen source controller')
        protocol_require((sdk/'.uwvm-task-owned').is_file(), 'BLOCKED original task-owned canonical Jazzy SDK required')
        frozen,manifest = json_pin(args.manifest);initial = source_state(source,frozen)
        report['source_manifest'] = manifest;report['source_manifest_sha256'] = manifest['sha256']
        archive = out/'driver-inputs';archive.mkdir()
        for name in DRIVER_INPUTS:
            origin = source/'tests/ros_aarch64'/name
            protocol_require(digest(origin) == initial.get('tests/ros_aarch64/'+name), 'executed helper differs from exact frozen source')
            acquired = file_pin(origin);shutil.copy2(origin,archive/name)
            copied = file_pin(archive/name);protocol_require(acquired['sha256'] == copied['sha256'], 'archived helper changed')
            report['driver_inputs'][name] = {'original':acquired,'archive':copied}
        probe_path = source/'tests/ros_cgroup/probe.py';file_pin(probe_path)
        spec = importlib.util.spec_from_file_location('g09_cgroup_probe',probe_path)
        probe = importlib.util.module_from_spec(spec);spec.loader.exec_module(probe)
        evidence = Evidence(out/'execution',probe.probe,interval=.0005);report['qemu'] = evidence.emulator(args.qemu);file_pin(args.qemu)
        before = cgroup_incarnation(evidence.leaf)
        expected_incarnation,expected_pin=json_pin(args.expected_cgroup)
        protocol_require(isinstance(expected_incarnation,dict) and set(expected_incarnation)=={'path','device','inode','boot_id'} and
                         before==expected_incarnation, 'BLOCKED exact explicitly selected current cgroup incarnation required')
        report['expected_cgroup_selection']={'value':expected_incarnation,'pin':expected_pin}
        owned = OwnedProcesses(evidence);report['cgroup_incarnation_before'] = before
        lease,lease_pin = json_pin(args.binfmt_evidence)
        protocol_require(isinstance(lease.get('name'),str) and re.fullmatch(r'[A-Za-z0-9_.-]+',lease['name']) is not None,
                         'parent binfmt lease name invalid')
        path = Path('/proc/sys/fs/binfmt_misc')/lease['name'];entry = path.read_text()
        fields = dict(line.split(None,1) for line in entry.splitlines()[1:] if ' ' in line)
        protocol_require(entry == lease['entry'] and entry.startswith('enabled\n') and 'F' in fields['flags:'] and
                         fields['interpreter'] == str(args.qemu.resolve()) and digest(args.qemu) == lease['qemu_sha256'] and
                         bytes.fromhex(fields['magic'])[18:20] == b'\xb7\x00' and bytes.fromhex(fields['mask'])[18:20] == b'\xff\xff',
                         'exact current fixed-interpreter ARM binfmt lease required')
        lease_state = {'path':path,'entry':entry,'name':lease['name']};guard();report['binfmt'] = {**lease_pin,'entry':entry}
        sdk_before,aliases_before = sdk_inventory(sdk),sdk_root_aliases(sdk)
        (out/'sdk-before.json').write_text(json.dumps(sdk_before,indent=2)+'\n');report['sdk_before'] = file_pin(out/'sdk-before.json')
        report['sdk_root_aliases'] = aliases_before
        checked(['mount','--make-rprivate','/']);checked(['mount','--rbind',sdk,root]);mounts.append(root)
        checked(['mount','--make-rprivate',root]);report['source_mount'] = bind_frozen_work(root,source,sdk,out,checked)
        checked(['mount','--bind',temporary,root/'tmp']);home = out/'owned-home';home.mkdir();checked(['mount','--bind',home,root/'root'])
        for name in ('usr','opt','etc','var/lib/dpkg'):
            checked(['mount','--bind',sdk/name,root/name]);checked(['mount','-o','remount,bind,ro',root/name])
        for name in ('proc','sys','dev'):
            checked(['mount','--rbind',Path('/')/name,root/name]);checked(['mount','--make-rprivate',root/name])
        checked(['mount','-o','remount,bind,ro',root]);checked(['mount','--make-rprivate',root]);(temporary/'ros-logs').mkdir()
        compiler = rooted(sdk,'/usr/bin/aarch64-linux-gnu-g++-13')
        frontend = next((rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus') for base in ('usr/lib','usr/libexec')
                         if rooted(sdk,'/'+base+'/gcc/aarch64-linux-gnu/13/cc1plus').is_file()),None)
        protocol_require(frontend is not None, 'canonical Jazzy GCC13 frontend unavailable')
        for label,path in (('native-gcc',compiler),('native-frontend',frontend),('native-cmake',rooted(sdk,'/usr/bin/cmake'))):
            tools[label] = evidence.target(label,path);file_pin(path)
        report['tools'] = tools
        if args.mode == 'prepare':
            generated = temporary/'cmake';generated.mkdir();(generated/'CMakeLists.txt').write_text(cmake())
            (temporary/'evidence-stop.hpp').write_text(BARRIER);file_pin(generated/'CMakeLists.txt');file_pin(temporary/'evidence-stop.hpp')
            execute('configure',['/usr/bin/cmake','-S','/tmp/cmake','-B','/tmp/build',
                    '-DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++-13','-DCMAKE_BUILD_TYPE=Release',
                    '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'],['native-cmake'],120)
            execute('compile',['/usr/bin/cmake','--build','/tmp/build','--parallel','1','--verbose'],
                    ['native-cmake','native-gcc','native-frontend'],600)
            verbose,compile_log = read_file_binding(out/'compile.log',limit=16*1024**2,retain=True);pins.append(compile_log)
            database,database_pin = json_pin(temporary/'build/compile_commands.json');recipe = compile_recipe(database,verbose.decode())
            raw = temporary/recipe['effective_mf'][5:];dep = out/'witness-full-sdk.d';shutil.copy2(raw,dep)
            inputs,dep_pin = fixture_closure(dep,source,sdk,temporary,recipe);pins.append(dep_pin)
            required = {'/work/'+WITNESS_SOURCE,'/tmp/evidence-stop.hpp'}
            protocol_require(required.issubset(inputs) and any(name.startswith('/usr/') for name in inputs) and
                             any(name.startswith('/opt/') and name.endswith('/rmw/types.h') for name in inputs) and
                             any(name.startswith('/work/third-parties/fast_io/') for name in inputs),
                             'original full-MD native fixture/public-RMW/fast_io/system/barrier closure required')
            meta = temporary/'build/CMakeFiles'/(WITNESS+'.dir');link,link_pin = read_file_binding(meta/'link.txt',retain=True)
            pins.append(link_pin);validate_fixture_link(WITNESS,recipe,link.decode(),verbose.decode())
            libraries = native_link_closure(link.decode(),sdk);pins.extend(libraries.values())
            for item in inputs.values(): file_pin(item['path'])
            origin = temporary/'build'/WITNESS
            report['witness'] = {'binary':evidence.target(WITNESS,origin),'source':WITNESS_SOURCE,'compile_recipe':recipe,
                'raw_dependencies':{**dep_pin,'actual_output':file_pin(raw),'inputs':inputs},'metadata':{},'link_libraries':libraries,
                'object':file_pin(temporary/recipe['compiled_object_guest_path'][5:]),'compile_log':compile_log,
                'primary_contract_reference':PRIMARY_CONTRACT}
            for name,path in (('database',temporary/'build/compile_commands.json'),('flags',meta/'flags.make'),('link',meta/'link.txt'),
                              ('cmake',generated/'CMakeLists.txt'),('cache',temporary/'build/CMakeCache.txt'),('barrier',temporary/'evidence-stop.hpp')):
                report['witness']['metadata'][name] = file_pin(path)
            file_pin(origin);report['compilation_status'] = 'PASS';status = 'PASS'
        else:
            original,prepared_pin = json_pin(args.prepared)
            protocol_require(original.get('status') == 'PREPARED' and original.get('kind') == KIND and
                             type(original.get('format_version')) is int and original['format_version'] == 1 and
                             original.get('mode') == 'prepare' and original.get('domain') == DOMAIN and
                             original.get('source') == str(source) and original.get('rootfs') == str(sdk) and
                             original.get('source_manifest_sha256') == manifest['sha256'] and
                             original.get('compilation_status') == 'PASS' and original.get('actual_guest_behavior') == 'NOT_PERFORMED' and
                             original.get('same_cgroup_incarnation') is True and original.get('cgroup_incarnation_before') == before and
                             original.get('cgroup_incarnation_after') == before and original.get('container_started_at') == args.container_started_at,
                             'exact successful same-source/SDK/incarnation singleton PREPARED report required')
            protocol_require(set(original.get('driver_inputs',{})) == set(DRIVER_INPUTS) and
                             all(original.get(key) is True for key in ('source_preserved','sdk_preserved','driver_inputs_preserved',
                                 'immutable_inputs_preserved','products_preserved')) and
                             original.get('process_cleanup',{}).get('status') == 'PASS' and
                             original.get('process_cleanup',{}).get('remaining_owned_processes') == [] and
                             original.get('remaining_owned_mounts') == [] and original.get('mount_cleanup') and
                             all(type(item.get('returncode')) is int and item['returncode'] == 0 for item in original['mount_cleanup']),
                             'prepared preservation/cleanup/helper closure incomplete')
            for item in original['driver_inputs'].values():
                for key in ('original','archive'):
                    protocol_require(file_pin(item[key]['path']) == item[key], 'prepared helper original/archive identity changed')
            old_sdk,old_sdk_pin = json_pin(original['sdk_before']['path'])
            protocol_require(old_sdk_pin == original['sdk_before'] and old_sdk == sdk_before and
                             original['sdk_root_aliases'] == aliases_before and original['tools'] == tools,
                             'prepared complete original SDK/keyset/tool identity differs')
            architecture,architecture_pin = architecture_binding(original['architecture_evidence']['path'],pins)
            protocol_require(architecture_pin == original['architecture_evidence'] and architecture['cgroup_incarnation_before'] == before and
                             original['qemu']['sha256'] == report['qemu']['sha256'], 'prepared raw compiler architecture/emulator differs')
            commands = original.get('commands',[])
            protocol_require(len(commands) == 2 and [item.get('name') for item in commands] == ['configure','compile'] and
                             all(item.get('status') == 'PASS' and type(item.get('returncode')) is int and item['returncode'] == 0 for item in commands),
                             'prepared actual serial configure/compile command set differs')
            build = commands[1]
            protocol_require(type(build.get('pid')) is int and build['pid'] > 0 and
                             type(build.get('starttime_ticks')) is int and build['starttime_ticks'] > 0,
                             'prepared actual native compiler launcher birth is missing')
            launcher_samples = [item for item in architecture.get('actual_process_samples',[]) if
                item.get('name') == 'compile/' and item.get('pid') == build['pid'] and
                item.get('starttime_ticks') == build['starttime_ticks'] and item.get('ancestry') == [build['pid']] and
                item.get('live_cgroup_procs') is True and item.get('membership') == evidence.before['membership']]
            protocol_require(bool(launcher_samples), 'prepared actual native compiler launcher birth/ancestry is unproven')
            samples = [item for item in architecture.get('actual_process_samples',[]) if item.get('name') == 'compile/' and
                       item.get('ancestry',[None])[0] == build['pid'] and item.get('live_cgroup_procs') is True and
                       item.get('membership') == evidence.before['membership'] and
                       type(item.get('starttime_ticks')) is int and item['starttime_ticks'] >= build['starttime_ticks'] and
                       item.get('kernel_exe_elf',{}).get('e_machine') == 62 and
                       item.get('kernel_exe_elf',{}).get('sha256') == report['qemu']['sha256']]
            mapped = {label for item in samples for label in item.get('mapped_arm_targets',[])}
            protocol_require(set(tools).issubset(mapped) and all(architecture['arm_targets'].get(label) == fact for label,fact in tools.items()),
                             'prepared actual native compiler/frontend/CMake mappings missing or substituted')
            witness = original['witness'];origin = Path(witness['binary']['path'])
            protocol_require(require_arm(origin) == witness['binary'] and witness['source'] == WITNESS_SOURCE and
                             witness.get('primary_contract_reference') == PRIMARY_CONTRACT and
                             architecture['arm_targets'].get(WITNESS) == witness['binary'], 'prepared singleton witness identity/primary predicate differs')
            verbose,compile_pin = read_file_binding(witness['compile_log']['path'],retain=True);pins.append(compile_pin)
            protocol_require(compile_pin == witness['compile_log'], 'prepared actual compiler log replaced')
            metadata = witness['metadata']
            for item in metadata.values(): protocol_require(file_pin(item['path']) == item, 'prepared actual metadata inode/bytes changed')
            database,_ = json_pin(metadata['database']['path'])
            recipe = compile_recipe(database,verbose.decode())
            protocol_require(recipe == witness['compile_recipe'] and Path(metadata['cmake']['path']).read_text() == cmake() and
                             Path(metadata['barrier']['path']).read_text() == BARRIER, 'prepared original recipe/barrier/CMake changed')
            original_temp = origin.parent.parent
            dep = witness['raw_dependencies'];closure,dep_pin = fixture_closure(Path(dep['path']),source,sdk,original_temp,recipe)
            pins.append(dep_pin)
            protocol_require(dep_pin == {key:dep[key] for key in dep_pin} and closure == dep['inputs'] and
                             file_pin(dep['actual_output']['path']) == dep['actual_output'] and
                             dep['actual_output']['sha256'] == dep_pin['sha256'] and file_pin(witness['object']['path']) == witness['object'],
                             'prepared original system-inclusive actual-MF/object/source/SDK closure changed')
            for item in closure.values(): file_pin(item['path'])
            link = Path(metadata['link']['path']).read_text();validate_fixture_link(WITNESS,recipe,link,verbose.decode())
            libraries = native_link_closure(link,sdk)
            protocol_require(libraries == witness['link_libraries'], 'prepared native link inputs changed')
            pins.extend(libraries.values());report['prepared'] = prepared_pin;report['witness_qualification'] = witness
            products['witness'] = snapshot(origin,temporary/WITNESS,WITNESS)

            strict = qualify_gateway(args.gateway,args.gateway_report,args.manifest,source,sdk,2)
            protocol_require('gateways/ros2/environment.h' in strict['source_dependencies'], 'Gateway actual compiler closure omits checked enclave environment source')
            pins.extend(strict['input_pins']);report['gateway_qualification'] = strict
            gateway_loader = gateway_library_environment(strict,sdk,sdk_before)
            pins.extend(gateway_loader['input_pins']);report['gateway_library_environment'] = gateway_loader
            products['gateway'] = snapshot(args.gateway,temporary/'gateway',ROLE_LABELS['gateway_old'])
            config,config_pin = json_pin(args.worker_config);worker_report,worker_pin = json_pin(args.worker_report)
            protocol_require(worker_report.get('status') == 'PASS' and worker_report.get('source') == str(source) and
                             worker_report.get('source_manifest_sha256') == manifest['sha256'] and worker_report.get('config_sha256') == config_pin['sha256'] and
                             worker_report.get('cgroup_incarnation_before') == before and config.get('qemu_argv') and
                             config['qemu_argv'][0] == str(args.qemu.resolve()), 'fresh exact four-profile Worker/config/QEMU/incarnation required')
            worker_bindings = validate_fresh_worker_inputs(worker_report,source,frozen)
            for item in worker_bindings.values():
                for value in item['raw_compile_dependencies'].values():
                    raw_pin = file_pin(value['path']);protocol_require(raw_pin['sha256'] == value['sha256'], 'Worker actual raw dependency changed')
                    file_pin(value['object']['path'])
            for name in ('compiler','qemu'): pins.append(worker_report[name]['input'])
            dual = [value for value in worker_report['profiles'] if value.get('profile') == 'dual']
            protocol_require(len(dual) == 1 and dual[0].get('status') == 'PASS' and dual[0]['binary'] == require_arm(args.worker),
                             'selected image must be exact current qualified dual Worker')
            report['worker_qualification'] = {'report':worker_pin,'config':config_pin,'dual':worker_bindings['dual']}
            products['worker'] = snapshot(args.worker,out/'worker',ROLE_LABELS['worker'])
            report['products'] = products;check_pins()

            # Own a short exclusive socket/control directory and native domain.
            # All native writable paths are inside the existing fresh tmp bind;
            # no /keys or /guest directory is created in the immutable SDK.
            control = Path(tempfile.mkdtemp(prefix='uwvm-revoke-',dir='/tmp'));control.chmod(0o700)
            control_info = control.stat();report['control_directory'] = {'path':str(control),'device':control_info.st_dev,'inode':control_info.st_ino}
            (temporary/'control').mkdir();checked(['mount','--bind',control,root/'tmp/control']);mounts.append(root/'tmp/control')
            lock_path = Path('/tmp/uwvm-arm-ros-domain-75.lock')
            fd = os.open(lock_path,os.O_CREAT|os.O_EXCL|os.O_WRONLY,0o600)
            try: os.write(fd,str(os.getpid()).encode());identity = os.fstat(fd)
            finally: os.close(fd)
            domain_lock = (lock_path,identity.st_dev,identity.st_ino)
            file_pin(source/POLICY_SOURCE);file_pin(source/WAT_SOURCE);file_pin(args.wat2wasm)
            python = rooted(sdk,'/usr/bin/python3');openssl = rooted(sdk,'/usr/bin/openssl')
            evidence.target('native-python',python);file_pin(python);evidence.target('native-openssl',openssl);file_pin(openssl)
            credentials = temporary/'revoke-reconnect-keys';credentials.mkdir(mode=0o700,exist_ok=False)
            key_root = credentials.lstat();credentials_identity = (key_root.st_dev,key_root.st_ino,key_root.st_uid)
            generate = execute('signed-generate',['/opt/ros/jazzy/bin/ros2','security','generate_artifacts','-k',KEYSTORE,
                                '-p','/work/'+POLICY_SOURCE],['native-python'],90)
            ledger = credential_ledger(credentials,True)
            report['signed'] = {'status':'VERIFYING','keystore':KEYSTORE,'policy':read_file_binding(source/POLICY_SOURCE),
                'policy_sha256':digest(source/POLICY_SOURCE),'generation_command':generate,'ledger':ledger,
                'primary_contract_reference':PRIMARY_CONTRACT,
                'primary_contract_boundary':'Inspected immutable official source text. Actual SDK headers/libraries are separately qualified; no claim they were built from these branch-head commits.'}
            verification = [];subjects = {};signed_permissions = {};governance = {}
            for role in ('gateway','witness'):
                directory = KEYSTORE+'/enclaves/'+role
                verification.append(execute(role+'-certificate-verify',['/usr/bin/openssl','verify','-CAfile',directory+'/identity_ca.cert.pem',
                    '-no-CApath','-no-CAstore',directory+'/cert.pem'],['native-openssl']))
                subject = execute(role+'-certificate-subject',['/usr/bin/openssl','x509','-in',directory+'/cert.pem','-noout','-subject','-nameopt','RFC2253'],['native-openssl'])
                verification.append(subject);output = Path(subject['log']).read_text()
                protocol_require(output.strip() == 'subject=CN=/'+role, 'signed certificate subject differs from exact enclave')
                subjects[role] = 'CN=/'+role
                verification.append(execute(role+'-certificate-public',['/usr/bin/openssl','x509','-in',directory+'/cert.pem','-noout','-pubkey','-out','/tmp/'+role+'-cert-public.pem'],['native-openssl']))
                verification.append(execute(role+'-private-public',['/usr/bin/openssl','pkey','-in',directory+'/key.pem','-pubout','-out','/tmp/'+role+'-key-public.pem'],['native-openssl']))
                certificate_public = file_pin(temporary/(role+'-cert-public.pem'));key_public = file_pin(temporary/(role+'-key-public.pem'))
                protocol_require(certificate_public['sha256'] == key_public['sha256'], 'private signing key does not match exact enclave certificate public key')
                # Jazzy SROS2 signs text/plain MIME content (PKCS7_TEXT).
                # -text strips the verified MIME header before strict XML
                # parsing, and refuses a different signed content type. Only
                # this owned explicit CA is trusted; no SDK default CA store.
                verification.append(execute(role+'-permissions-verify',['/usr/bin/openssl','smime','-verify','-text','-in',directory+'/permissions.p7s',
                    '-CAfile',directory+'/permissions_ca.cert.pem','-no-CApath','-no-CAstore','-out','/tmp/'+role+'-permissions-verified.xml'],['native-openssl']))
                verification.append(execute(role+'-governance-verify',['/usr/bin/openssl','smime','-verify','-text','-in',directory+'/governance.p7s',
                    '-CAfile',directory+'/permissions_ca.cert.pem','-no-CApath','-no-CAstore','-out','/tmp/'+role+'-governance-verified.xml'],['native-openssl']))
                signed_permissions[role] = permission_contract(temporary/(role+'-permissions-verified.xml'),role,subjects[role])
                governance[role] = governance_contract(temporary/(role+'-governance-verified.xml'))
                pins.extend((signed_permissions[role]['verified_xml'],governance[role]))
            protocol_require(credential_ledger(credentials) == ledger, 'signed keystore changed during public cryptographic verification')
            signed = report['signed'];signed.update(status='PASS',subjects=subjects,certificate_CA_verified=True,
                permissions_signatures_verified=True,governance_signatures_verified=True,key_cert_public_match=True,
                default_deny=True,exact_topic_permissions=True,permissions=signed_permissions,governance=governance,
                verified_commands=verification,closure_sha256=hashlib.sha256(json.dumps(ledger,sort_keys=True,separators=(',',':')).encode()).hexdigest())
            # Translate only the fixed source WAT. Host-native assemblers are
            # permitted, with their complete selected image pinned and observed.
            assembler_fact = elf(args.wat2wasm)
            if assembler_fact['e_machine'] == 183: evidence.target('wat2wasm',args.wat2wasm);assembler_required = ['wat2wasm']
            elif assembler_fact['e_machine'] == 62: assembler_required = []
            else: raise RuntimeError('selected WAT translator is neither qualified ARM nor host x86_64 ELF')
            wasm = out/'guest.wasm'
            translate,translate_row = start('wat-translate',[str(args.wat2wasm),str(source/WAT_SOURCE),'-o',str(wasm)],assembler_required,60,
                environment={'PATH':'/usr/bin:/bin','LANG':'C.UTF-8'})
            finish(translate,translate_row);report['wat_translation'] = {'tool':read_file_binding(args.wat2wasm),
                'elf':assembler_fact,'source':read_file_binding(source/WAT_SOURCE),'wasm':file_pin(wasm),'command':translate_row}
            evidence.observe(translate.pid,'wat-translate/')
            with evidence._lock:
                assembler_samples = [item for item in evidence.samples.values() if item.get('name') == 'wat-translate/' and
                    item.get('ancestry',[None])[0] == translate.pid and item.get('live_cgroup_procs') is True and
                    item.get('membership') == evidence.before['membership'] and
                    (item.get('kernel_exe_elf',{}).get('sha256') == assembler_fact['sha256'] if assembler_fact['e_machine'] == 62 else
                     'wat2wasm' in item.get('mapped_arm_targets',[]))]
            protocol_require(bool(assembler_samples), 'selected actual WAT translator image was not observed in its owned live cgroup tree')
            report['wat_translation']['actual_tool_sample'] = assembler_samples[-1]

            connection,grants = topic_policy();argv = worker_arguments(out/'worker',connection,grants,control/'ros2.sock',wasm)
            worker_command = [*config['qemu_argv'],*argv]
            worker,worker_row = start('worker',['/bin/bash','--noprofile','--norc','-c','kill -STOP $$; exec "$@"','revoke-worker',*worker_command],
                [ROLE_LABELS['worker']],120,stdin=True,environment={'PATH':'/usr/bin:/bin','LANG':'C.UTF-8'})
            until = time.monotonic()+5
            while time.monotonic()<until and process_identity(worker.pid)['state'] not in ('T','t'): guard();time.sleep(.001)
            protocol_require(process_identity(worker.pid)['state'] in ('T','t'), 'reserved exact Worker launch PID did not pause')
            cfg = control/'gateway.conf';cfg.write_text(gateway_configuration(worker.pid,connection,grants));cfg.chmod(0o600)
            cfg_archive = out/'gateway.conf';shutil.copy2(cfg,cfg_archive);file_pin(cfg_archive)
            report['policy'] = {'connection':connection,'grants':grants,'worker_argv':argv,'worker_execution_argv':worker_command,
                'gateway_config':read_file_binding(cfg),'gateway_config_archive':read_file_binding(cfg_archive),'gateway_config_text':cfg.read_text()}
            gateway_env = native_environment(True);gateway_env.update(gateway_loader['environment'])
            report['gateway_test_environment'] = {'ambient_environment_inherited':False,'explicit_override':None,
                'configured_enclave':'/gateway','direct_used_root_observed':False}

            def socket_ready(child):
                deadline = time.monotonic()+30
                while time.monotonic()<deadline and not (control/'ros2.sock').exists():
                    guard();protocol_require(child.poll() is None, 'Gateway exited before signed socket readiness');time.sleep(.002)
                info = (control/'ros2.sock').lstat()
                protocol_require(stat.S_ISSOCK(info.st_mode) and info.st_uid == os.getuid() and stat.S_IMODE(info.st_mode) == 0o600,
                                 'actual owned authenticated IPC socket missing/insecure')
                return {'path':str(control/'ros2.sock'),'device':info.st_dev,'inode':info.st_ino,'worker_pid':worker.pid}

            gateway_old,old_row = start('gateway-old',native(['/tmp/gateway','/tmp/control/gateway.conf']),[ROLE_LABELS['gateway_old']],120,
                enforce=True,environment=gateway_env)
            communication_roles = (worker,gateway_old)
            old_proof = mapped_stop(gateway_old,old_row,ROLE_LABELS['gateway_old'])
            old_socket = socket_ready(gateway_old);report['old_socket'] = old_socket
            witness_child,witness_row = start('witness',native(['/tmp/'+WITNESS,KEYSTORE],witness=True),[WITNESS],100,stdin=True,enforce=True)
            witness_proof = mapped_stop(witness_child,witness_row,WITNESS,pre_stopped=True)
            send_gate(witness_child,witness_row,'witness','I',witness_proof,'ARM_WITNESS_PREINIT_STOP')
            witness_pid = witness_proof['actual_stopped_ARM']['pid']
            wait_marker(witness_child,witness_row,parse_witness_markers,'READY_REVOKE_WITNESS',witness_pid)
            reservation = next(item for item in owned.rows.values() if item['identity']['pid'] == worker.pid and item['label'] == 'worker')
            owned.send(reservation,signal.SIGCONT)
            worker_proof = mapped_stop(worker,worker_row,ROLE_LABELS['worker'])
            report['before_guest_G'] = {role:live_role(child,row,role,proof) for role,child,row,proof in
                (('worker',worker,worker_row,worker_proof),('gateway_old',gateway_old,old_row,old_proof),('witness',witness_child,witness_row,witness_proof))}
            send_gate(worker,worker_row,'worker','G',worker_proof,'READY_REVOKE_WITNESS')
            # G is the first successful release of the actual Wasm fixture.
            # Retain attempted execution even if a later marker or native
            # predicate fails; a partial run must not say NOT_PERFORMED.
            report['actual_guest_behavior'] = 'RUNNING';save()
            wait_marker(worker,worker_row,parse_guest_markers,'REVOKE_ENDPOINTS_CREATED')
            wait_marker(witness_child,witness_row,parse_witness_markers,'MATCHED_REVOKE_OLD',witness_pid)
            send_gate(worker,worker_row,'worker','B',worker_proof,'MATCHED_REVOKE_OLD+REVOKE_ENDPOINTS_CREATED')
            wait_marker(worker,worker_row,parse_guest_markers,'BEFORE_MESSAGES_SENT')
            wait_marker(witness_child,witness_row,parse_witness_markers,'BASELINE_REVOKE_WITNESS',witness_pid)
            send_gate(worker,worker_row,'worker','R',worker_proof,'BASELINE_REVOKE_WITNESS+BEFORE_MESSAGES_SENT')
            wait_marker(worker,worker_row,parse_guest_markers,'REVOKED_STALE9_REOPEN125')
            send_gate(witness_child,witness_row,'witness','N',witness_proof,'REVOKED_STALE9_REOPEN125')
            wait_marker(witness_child,witness_row,parse_witness_markers,'FROZEN_REVOKE_WITNESS',witness_pid)

            # The only intentional fault in this lane targets the old owned
            # Gateway birth. All observed descendants share its owned label;
            # no process-group, PID-name search or unrelated task is signaled.
            guard();check_pins();live_role(worker,worker_row,'worker',worker_proof)
            live_role(witness_child,witness_row,'witness',witness_proof)
            live_role(gateway_old,old_row,'gateway_old',old_proof)
            communication_roles = None
            fault = {'intentional':True,'signal':9,'old_pid':old_proof['actual_stopped_ARM']['pid'],
                'old_starttime_ticks':old_proof['actual_stopped_ARM']['starttime_ticks'],'observed_old_births':[],
                'old_socket':old_socket,'same_worker_live':True,'same_witness_live':True}
            deadline = time.monotonic()+5;sent = set()
            while True:
                guard();owned.sample();live_role(worker,worker_row,'worker',worker_proof)
                live_role(witness_child,witness_row,'witness',witness_proof)
                old_births = [item for item in owned.rows.values() if item['label'] == old_row['name']]
                for item in sorted(old_births,key=lambda value:len(value['ancestry']),reverse=True):
                    identity = item['identity'];birth = (identity['pid'],identity['starttime_ticks'])
                    if birth in sent or not owned.alive(item): continue
                    protocol_require(owned.check(item), 'old fault target lacks current live cgroup/birth ownership')
                    owned.send(item,signal.SIGKILL);sent.add(birth)
                    fault['observed_old_births'].append({'pid':birth[0],'starttime_ticks':birth[1],
                        'pidfd_signal_sent':True,'live_cgroup_verified':True})
                if gateway_old.poll() is not None and not any(owned.alive(item) for item in old_births): break
                protocol_require(time.monotonic()<deadline, 'owned old Gateway fault/reap budget exhausted');time.sleep(.002)
            protocol_require(gateway_old.wait(timeout=1) == -signal.SIGKILL, 'owned old Gateway did not retire with exact requested SIGKILL')
            fault.update(returncode=gateway_old.returncode,root_reaped=True,all_old_owned_births_retired=True)
            old_raw,old_log_pin = read_file_binding(old_row['log'],limit=16*1024**2,retain=True);pins.append(old_log_pin)
            old_text = old_raw.decode('utf-8','strict')
            old_row.update(status='EXPECTED_OWNED_FAULT',returncode=gateway_old.returncode,log_binding=old_log_pin,log_sha256=old_log_pin['sha256'])
            old_entries = parse_gateway_audit(old_text,worker.pid,os.getuid())
            protocol_require(any(item['operation'] == 'capability.revoke' and item['alias'] == 'deadPub' for item in old_entries),
                             'actual native old-session revocation was not logged')
            socket_info = (control/'ros2.sock').lstat()
            protocol_require(stat.S_ISSOCK(socket_info.st_mode) and (socket_info.st_dev,socket_info.st_ino) == (old_socket['device'],old_socket['inode']),
                             'only exact old owned socket may be removed after its writer retires')
            (control/'ros2.sock').unlink();fault['socket_removed_after_retirement'] = True
            report['fault'] = fault;save()
            send_gate(worker,worker_row,'worker','F',worker_proof,'FROZEN_REVOKE_WITNESS+OLD_GATEWAY_REAPED',{'old_gateway_retired':True})
            wait_marker(worker,worker_row,parse_guest_markers,'GATEWAY_FAULT107')

            gateway_new,new_row = start('gateway-new',native(['/tmp/gateway','/tmp/control/gateway.conf']),[ROLE_LABELS['gateway_new']],120,
                enforce=True,environment=gateway_env)
            communication_roles = (worker,gateway_new)
            new_proof = mapped_stop(gateway_new,new_row,ROLE_LABELS['gateway_new'])
            new_socket = socket_ready(gateway_new);fault['new_socket'] = new_socket
            protocol_require((new_socket['device'],new_socket['inode']) != (old_socket['device'],old_socket['inode']), 'replacement socket reused the old owned inode')
            protocol_require((new_proof['actual_stopped_ARM']['pid'],new_proof['actual_stopped_ARM']['starttime_ticks']) !=
                             (old_proof['actual_stopped_ARM']['pid'],old_proof['actual_stopped_ARM']['starttime_ticks']), 'replacement Gateway is not a fresh actual process birth')
            report['before_guest_C'] = {role:live_role(child,row,role,proof) for role,child,row,proof in
                (('worker',worker,worker_row,worker_proof),('gateway_new',gateway_new,new_row,new_proof),('witness',witness_child,witness_row,witness_proof))}
            send_gate(worker,worker_row,'worker','C',worker_proof,'NEW_GATEWAY_READY+GATEWAY_FAULT107',{'new_gateway_live':True})
            wait_marker(worker,worker_row,parse_guest_markers,'RECONNECT_ENDPOINT_CREATED')
            wait_marker(witness_child,witness_row,parse_witness_markers,'MATCHED_REVOKE_NEW',witness_pid)
            admission_deadline = time.monotonic()+2
            while True:
                guard();live_role(worker,worker_row,'worker',worker_proof)
                live_role(gateway_new,new_row,'gateway_new',new_proof)
                before_A_text = growing_log(new_row['log'])
                admission = parse_gateway_audit(before_A_text,worker.pid,os.getuid(),False)
                if len(admission) >= 2: break
                protocol_require(time.monotonic()<admission_deadline, 'fresh before-A native audit admission did not complete');time.sleep(.002)
            protocol_require([item['operation'] for item in admission] == ['session.bind','topic.publish'] and
                             [item['alias'] for item in admission] == ['target','livePub'] and admission[0]['session'] == admission[1]['session'],
                             'fresh actual native bind/publish must be admitted before final release')
            old_guest = [item for item in old_entries if item['operation'] == 'topic.publish']
            protocol_require(bool(old_guest) and admission[0]['session'] != old_guest[0]['session'], 'replacement reused old authenticated session')
            send_gate(worker,worker_row,'worker','A',worker_proof,'MATCHED_REVOKE_NEW+RECONNECT_ENDPOINT_CREATED+NEW_SESSION_BIND',
                {'new_session':admission[0]['session']})
            wait_marker(worker,worker_row,parse_guest_markers,'AFTER_MESSAGE_SENT')
            wait_marker(witness_child,witness_row,parse_witness_markers,'DELIVERED_REVOKE_NEW',witness_pid)
            live_role(gateway_new,new_row,'gateway_new',new_proof)
            send_gate(worker,worker_row,'worker','Q',worker_proof,'DELIVERED_REVOKE_NEW+AFTER_MESSAGE_SENT',{'native_survivor_delivered':True})
            worker.stdin.close();finish(worker,worker_row)
            guest_raw,guest_pin = read_file_binding(worker_row['log'],limit=16*1024**2,retain=True);pins.append(guest_pin)
            guest_text = guest_raw.decode('utf-8','strict');parse_guest_markers(guest_text)
            send_gate(witness_child,witness_row,'witness','F',witness_proof,'PASS_REVOKE_RECONNECT_GUEST',{'guest_exit':worker.returncode})
            witness_child.stdin.close();finish(witness_child,witness_row)
            witness_raw,witness_pin = read_file_binding(witness_row['log'],limit=16*1024**2,retain=True);pins.append(witness_pin)
            witness_text = witness_raw.decode('ascii','strict');native_markers = parse_witness_markers(witness_text,witness_pid)
            # The replacement may retire normally after its bound Worker. A
            # live replacement is stopped later through the same owned cleanup.
            if gateway_new.poll() is not None:
                protocol_require(gateway_new.returncode == 0, 'replacement Gateway failed after bound Worker retirement')
                new_row.update(status='PASS',returncode=0,log_sha256=digest(new_row['log']));gateway_exit = 0
            else: gateway_exit = 'OWNED_CLEANUP'
            roles = {}
            for role,proof in (('worker',worker_proof),('gateway_old',old_proof),('gateway_new',new_proof),('witness',witness_proof)):
                actual = proof['actual_stopped_ARM'];product_role = 'gateway' if role.startswith('gateway_') else role
                roles[role] = {'pid':actual['pid'],'starttime_ticks':actual['starttime_ticks'],'membership':proof['live_cgroup_membership'],
                    'proof':proof,'mapped_product':products[product_role]['snapshot'],'pre_init_mapping':role == 'witness',
                    'live_before_C' if role == 'gateway_new' else 'live_before_G':True}
            completion = {'kind':KIND,'format_version':1,'mode':'execute','source_manifest_sha256':manifest['sha256'],'qemu_sha256':report['qemu']['sha256'],
                'products':products,'roles':roles,'worker_uid':os.getuid(),'signed':signed,'fault':fault,
                'markers':{'witness_text':witness_text,'witness':native_markers,'guest_text':guest_text},'gates':report['gates'],
                'sessions':{'old_text':old_text,'before_A_text':before_A_text},
                'native_context':{'witness':{'attested':True,'enclave':'/witness','domain':DOMAIN,'rmw':'rmw_fastrtps_cpp','security':'Enforce','root':KEYSTORE+'/enclaves/witness'},
                    'gateway':{'source_selected':True,'enclave':'/gateway','domain':DOMAIN,'rmw':'rmw_fastrtps_cpp','security':'Enforce',
                        'root':KEYSTORE+'/enclaves/gateway','environment_explicit':True,'direct_used_root_observed':False}},
                'fixture_assertions':{'stale_before':9,'reopen_before':125,'dead_peer':107,'stale_after':9,'reopen_after':125,'surviving_publish':0},
                'commands':{'generation_exit':generate['returncode'],'signed_verification_exits':[item['returncode'] for item in verification],
                    'worker_exit':worker.returncode,'witness_exit':witness_child.returncode,'gateway_old_exit':gateway_old.returncode,
                    'gateway_new_exit_after_worker':gateway_exit},'G09_complete':False}
            protocol_require(credential_ledger(credentials) == ledger, 'signed keystore changed during actual lifecycle communication')
            signed['keystore_preserved_before_removal'] = True
            report['completion'] = completion;report['actual_guest_behavior'] = 'OBSERVED_PENDING_FINAL_GUARDS';status = 'PASS'
    except BaseException as error:
        report['error'] = traceback.format_exc()
        if report['actual_guest_behavior'] in ('RUNNING','OBSERVED_PENDING_FINAL_GUARDS'):
            report['actual_guest_behavior'] = 'NOT_ACCEPTED'
        status = 'BLOCKED' if 'BLOCKED' in str(error) else 'FAIL'
    finally:
        # Capture natural exits before cleanup can terminate the other owned
        # participants. The final ledger is also required for partial failures
        # that never reached the successful completion dictionary.
        terminal_before_cleanup = [(child,row,child.poll(),row['status']) for child,row in children]
        if owned is not None:
            try:
                cleanup = retained_popen_cleanup(owned,children);report['process_cleanup'] = cleanup
                if cleanup['status'] != 'PASS': status = 'FAIL'
            except BaseException: report['process_cleanup_error'] = traceback.format_exc();status = 'FAIL'
        for child,row,previous_returncode,previous_status in terminal_before_cleanup:
            try:
                final_returncode = child.poll()
                row.update(returncode=final_returncode,status_before_cleanup=previous_status,
                           returncode_before_cleanup=previous_returncode)
                if final_returncode is None:
                    row.update(status='UNRETIRED_AFTER_CLEANUP')
                row.update(terminal_command_metadata(previous_status,previous_returncode,final_returncode,status == 'PASS'))
                final_raw,final_pin = read_file_binding(row['log'],limit=16*1024**2,retain=True)
                pins.append(final_pin)
                row.update(log_binding=final_pin,log_sha256=final_pin['sha256'])
            except BaseException:
                if row['status'] != 'UNRETIRED_AFTER_CLEANUP':
                    row['status'] = 'FAIL_TERMINAL_FINALIZATION'
                row['terminal_finalization_error'] = traceback.format_exc();status = 'FAIL'
        save()
        if completion is not None:
            try:
                new_raw,new_pin = read_file_binding(new_row['log'],limit=16*1024**2,retain=True);pins.append(new_pin)
                protocol_require(type(gateway_new.returncode) is int, 'replacement Gateway remained unretired after owned cleanup')
                terminal_status = 'OWNED_CLEANUP' if completion['commands']['gateway_new_exit_after_worker'] == 'OWNED_CLEANUP' else 'PASS'
                protocol_require(gateway_new.returncode in (0,-signal.SIGTERM,-signal.SIGKILL) if terminal_status == 'OWNED_CLEANUP' else
                                 gateway_new.returncode == 0, 'replacement Gateway ended with an unrelated failure')
                new_row.update(status=terminal_status,returncode=gateway_new.returncode,log_binding=new_pin,log_sha256=new_pin['sha256'])
                completion['commands']['gateway_new_terminal'] = {'status':terminal_status,'returncode':gateway_new.returncode,'log_binding':new_pin}
                new_text = new_raw.decode('utf-8','strict')
                completion['sessions']['new_text'] = new_text
                completion['sessions']['parsed'] = validate_sessions(completion['sessions']['old_text'],new_text,
                    completion['sessions']['before_A_text'],completion['roles']['worker']['pid'],completion['worker_uid'])
                report['native_audit_logs'] = {'old':old_log_pin,'new':new_pin}
            except BaseException: report['native_audit_finalization_error'] = traceback.format_exc();status = 'FAIL'
        if credentials is not None:
            try:
                protocol_require(credentials == temporary/'revoke-reconnect-keys', 'only this intended fresh signed keystore may be removed')
                try: key_root = credentials.lstat()
                except FileNotFoundError: key_root = None
                if key_root is not None:
                    protocol_require(stat.S_ISDIR(key_root.st_mode) and not stat.S_ISLNK(key_root.st_mode) and
                                     (key_root.st_dev,key_root.st_ino,key_root.st_uid) == credentials_identity,
                                     'owned keystore root was replaced before cleanup')
                    if completion is not None:
                        protocol_require(credential_ledger(credentials) == report['signed']['ledger'], 'signed keystore changed before owned cleanup')
                    shutil.rmtree(credentials)
                report['secrets_removed'] = not credentials.exists() and not credentials.is_symlink()
            except BaseException: report['secret_cleanup_error'] = traceback.format_exc();report['secrets_removed'] = False;status = 'FAIL'
        try:
            check_pins();report['immutable_inputs_preserved'] = True
            report['source_preserved'] = initial is not None and source_state(source,frozen) == initial
            report['sdk_preserved'] = sdk_before is not None and sdk_inventory(sdk) == sdk_before and sdk_root_aliases(sdk) == aliases_before
            report['driver_inputs_preserved'] = bool(report['driver_inputs']) and all(
                read_file_binding(value['original']['path']) == value['original'] and
                read_file_binding(value['archive']['path']) == value['archive'] for value in report['driver_inputs'].values())
            report['products_preserved'] = all(require_arm(value['origin']['path']) == value['origin'] and
                require_arm(value['snapshot']['path']) == value['snapshot'] for value in products.values())
            if not all(report[key] is True for key in ('source_preserved','sdk_preserved','driver_inputs_preserved','immutable_inputs_preserved','products_preserved')):
                status = 'FAIL'
        except BaseException: report['preservation_error'] = traceback.format_exc();status = 'FAIL'
        if evidence is not None:
            try:
                guard();report['cgroup_incarnation_after'] = cgroup_incarnation(evidence.leaf)
                report['same_cgroup_incarnation'] = report['cgroup_incarnation_after'] == before
                architecture_result = evidence.finish(status);status = architecture_result['status']
                report['architecture_evidence'] = file_pin(evidence.out/'architecture-evidence.json')
            except BaseException: report['architecture_error'] = traceback.format_exc();status = 'FAIL'
        report['mount_cleanup'] = []
        for mount in reversed(mounts):
            try:
                result = subprocess.run(['umount','-R',str(mount)],capture_output=True,text=True,timeout=30)
                report['mount_cleanup'].append({'path':str(mount),'returncode':result.returncode})
                if result.returncode: status = 'FAIL'
            except BaseException: report['mount_cleanup'].append({'error':traceback.format_exc()});status = 'FAIL'
        report['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if line.split()[4] == str(root) or line.split()[4].startswith(str(root)+'/')] if sys.platform == 'linux' else []
        if report['remaining_owned_mounts']: status = 'FAIL'
        if control is not None:
            try:
                identity = control.stat();recorded = report['control_directory']
                protocol_require((identity.st_dev,identity.st_ino) == (recorded['device'],recorded['inode']), 'owned control directory identity replaced')
                for name in ('ros2.sock','gateway.conf'):
                    path = control/name
                    if path.exists() or path.is_symlink(): path.unlink()
                control.rmdir();report['socket_absent'] = not control.exists()
            except BaseException: report['socket_cleanup_error'] = traceback.format_exc();report['socket_absent'] = False;status = 'FAIL'
        if domain_lock is not None:
            try:
                path,device,inode = domain_lock;current = path.lstat()
                protocol_require(stat.S_ISREG(current.st_mode) and current.st_uid == os.getuid() and
                                 stat.S_IMODE(current.st_mode) == 0o600 and
                                 (current.st_dev,current.st_ino) == (device,inode), 'owned domain lease identity replaced')
                path.unlink();report['domain_lock_removed'] = True
            except BaseException: report['domain_lock_cleanup_error'] = traceback.format_exc();report['domain_lock_removed'] = False;status = 'FAIL'
        if completion is not None:
            try:
                completion.update({key:report.get(key) for key in ('source_preserved','sdk_preserved','driver_inputs_preserved',
                                  'immutable_inputs_preserved','products_preserved')})
                completion['qualified_inputs_preserved'] = pins_preserved(pins)
                protocol_require(architecture_result is not None, 'final actual architecture verdict unavailable')
                valid,deltas = cgroup_oom_deltas(evidence.before,architecture_result['cgroup_after'],architecture_result['same_cgroup_incarnation'])
                protocol_require(valid and deltas == architecture_result['cgroup_ancestor_event_deltas'], 'same-incarnation actual ancestor OOM evidence differs')
                completion['cgroup'] = {'before':before,'after':report['cgroup_incarnation_after'],
                    'same_incarnation':report['same_cgroup_incarnation'],'memory_max':architecture_result['cgroup_after']['effective_memory_max'],
                    'swap_max':(evidence.leaf/'memory.swap.max').read_text().strip(),'readable_ancestors':valid,'zero_oom':valid,
                    'membership':evidence.before['membership']}
                completion['cleanup'] = {'owned_processes':report['process_cleanup']['remaining_owned_processes'],
                    'mounts':report['remaining_owned_mounts'],'socket_absent':report.get('socket_absent'),
                    'secrets_removed':report.get('secrets_removed'),'domain_lock_removed':report.get('domain_lock_removed')}
                protocol_require(status == 'PASS', 'combined raw observations failed final cleanup/provenance/architecture guards')
                validate_combined_completion(completion,expected_incarnation);report['actual_guest_behavior'] = 'PASS'
            except BaseException: report['completion_error'] = traceback.format_exc();report['actual_guest_behavior'] = 'NOT_ACCEPTED';status = 'FAIL'
        if status != 'PASS' and report['actual_guest_behavior'] in ('RUNNING','OBSERVED_PENDING_FINAL_GUARDS'):
            report['actual_guest_behavior'] = 'NOT_ACCEPTED'
        report['status'] = 'PREPARED' if status == 'PASS' and args.mode == 'prepare' else status
        save()
    print(report['status'],out/'report.json',flush=True)
    return 0 if report['status'] in ('PASS','PREPARED') else 77 if report['status'] == 'BLOCKED' else 1


if __name__ == '__main__':
    raise SystemExit(main())
