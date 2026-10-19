#!/usr/bin/env python3
"""Prepare one current native witness, or run one signed raw-import Guest lane.

Run only from a new full frozen source in the original root-owned Linux private
mount namespace, current64GiB zero-swap cgroup and exact parent QEMU/binfmt
lease. Preparation compiles one new C++20 witness and performs no ROS behavior.
Execution requires that separate prepared product, the current strictly
qualified ROS2 Gateway and the current four-profile Worker report. It never
extends the old18-product inventory or weakens the existing qualification API.

Actual execution is parent-only. Source/model checks do not establish native
compilation, a signed DDS identity or a Guest result. The native permission
event is attested by the immutable witness predicate; its raw Log::Entry body is
deliberately not captured. A positive Enforce result still leaves the separate
original G09 revocation/failure/reconnection combination pending.
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

KIND = 'g09-guest-enforce-arm'
DOMAIN = 74
MEMORY_MAX = 68719476736
EXPECTED_BOOT = '518a7b26-33dd-4044-9456-93e75aac9b63'
EXPECTED_LEAF_DEVICE = 29
EXPECTED_LEAF_INODE = 18927
# Retained only by the historical pure-model compatibility predicate. Actual
# preparation/execution requires a separately opened --expected-cgroup receipt.
WITNESS = 'g09-secure-witness'
WITNESS_SOURCE = 'tests/ros_security/guest_enforce_witness_ros2.cc'
WAT_SOURCE = 'tests/ros_security/guest_enforce_ros2.wat'
POLICY_SOURCE = 'tests/ros_security/sros2-policy.xml'
KEYSTORE = '/tmp/g09-keys'
NATIVE_TYPE = 'std_msgs/msg/String'
NATIVE_IDENTITY = 'RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18'
ROLE_LABELS = {'worker':'g09-worker', 'gateway':'g09-gateway', 'witness':WITNESS}
DRIVER_INPUTS = ('run_g09_guest_enforce.py', 'runtime.py', 'rebuild_affected.py',
                 'gateway_qualification.py', 'run_action_qos.py',
                 'run_local_parameter_events.py', 'run_ros1_search.py',
                 'run_resources.py', 'run_all.py', 'run_unit.py',
                 'build_rpc_reader_diagnostic.py', 'native_runtime_environment.py',
                 'artifacts.py', 'run_regression.py', 'qualify_current_native.py')
BARRIER = '''#include <csignal>
__attribute__((constructor)) static void g09_witness_preinit_stop() { if(::raise(SIGSTOP)!=0) { __builtin_trap(); } }
'''
PRIMARY_CONTRACT = {
    'FastDDS_Log_public': {'url':'https://raw.githubusercontent.com/eProsima/Fast-DDS/5fb2d46b825209fb15f06eed6c2ab72baa09fa02/include/fastdds/dds/log/Log.hpp','sha256':'f9eb69630929d560e88e83a0d841b6309d9ed7705edd0beffd180dad8c68d7f5'},
    'FastDDS_Log_lifetime': {'url':'https://raw.githubusercontent.com/eProsima/Fast-DDS/5fb2d46b825209fb15f06eed6c2ab72baa09fa02/src/cpp/fastdds/log/Log.cpp','sha256':'2c965010706b3b59b4d59f293a76fa28c5318f83559b379fcde37142e610929f'},
    'FastDDS_creation_refusal': {'url':'https://raw.githubusercontent.com/eProsima/Fast-DDS/5fb2d46b825209fb15f06eed6c2ab72baa09fa02/src/cpp/rtps/security/SecurityManager.cpp','sha256':'8e773d105ba4c45d64ed9db9d0b04035e42541826b506f6a63f30e639a72185c'},
    'FastDDS_permission_predicate': {'url':'https://raw.githubusercontent.com/eProsima/Fast-DDS/5fb2d46b825209fb15f06eed6c2ab72baa09fa02/src/cpp/security/accesscontrol/Permissions.cpp','sha256':'25765d542f973e4f5c9217b0d33ee88a0e086ea82e611e0e26766428226e83ad'},
    'Jazzy_generic_writer_failure': {'url':'https://raw.githubusercontent.com/ros2/rmw_fastrtps/eb2b49a66b02c209ce836f04b3611e1ab550c134/rmw_fastrtps_cpp/src/publisher.cpp','sha256':'62d51e6a3994dc807ff672e4d94a6a1a49528a272c2c47020ab2301670f763f1'},
    'Jazzy_rcl_failure_retirement': {'url':'https://raw.githubusercontent.com/ros2/rcl/18d96c9c2b98e616324ae9a2c7701dd0b0886ff0/rcl/src/rcl/publisher.c','sha256':'bdeed60efb95cfca0509dbb70e04b53ae09aa2ea92a15398139833c97c4ae90a'},
}
WITNESS_FIELDS = {
    'READY_SECURE_WITNESS': ('pid','node','enclave','domain','rmw','type','sub_topic','pub_topic','pub_gid','out_bytes','in_bytes'),
    'READY_SECURE_MATCHED': ('pid','node','type','sub_topic','pub_topic','witness_sub_gid','witness_pub_gid',
                            'gateway_pub_gid','gateway_sub_gid','matched_publishers','matched_subscribers','received','sent'),
    'PASS_SECURE_WITNESS': ('pid','node','type','sub_topic','pub_topic','witness_sub_gid','witness_pub_gid',
                           'gateway_pub_gid','gateway_sub_gid','matched_publishers','matched_subscribers','received','sent',
                           'mismatched','denied_native','permission_events','out_bytes','in_bytes'),
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


def parse_witness_markers(text, expected_pid, complete=True):
    """Parse only exact fixed native lines; this pure function never launches IO."""
    protocol_require(type(expected_pid) is int and expected_pid > 0, 'native marker PID must be exact integer')
    protocol_require(isinstance(text,str) and len(text) <= 16*1024**2 and text.isascii(), 'bounded ASCII witness log required')
    protocol_require('\r' not in text and (not complete or bool(text) and text.endswith('\n')), 'native protocol requires complete exact LF lines')
    lines = [] if not text else text.split('\n')[:-1] if text.endswith('\n') else text.split('\n')
    if text and not text.endswith('\n'):
        lines = lines[:-1]
    rows = {}
    order = tuple(WITNESS_FIELDS)
    numeric = {'pid','domain','out_bytes','in_bytes','matched_publishers','matched_subscribers',
               'received','sent','mismatched','denied_native','permission_events'}
    for line in lines:
        protocol_require(bool(line), 'unexpected empty witness protocol line')
        parts = line.split(' ')
        name = parts[0]
        protocol_require(name in WITNESS_FIELDS and name not in rows and name == order[len(rows)], 'unexpected/repeated/out-of-order native marker')
        values = {}
        for token in parts[1:]:
            protocol_require(token.count('=') == 1, 'native marker token is not one key=value')
            key,value = token.split('=',1)
            protocol_require(key not in values and value != '', 'duplicate/empty native marker field')
            if key in numeric:
                protocol_require(re.fullmatch(r'0|[1-9][0-9]{0,19}',value) is not None, 'noncanonical native integer')
                value = int(value)
            values[key] = value
        protocol_require(set(values) == set(WITNESS_FIELDS[name]), 'native marker field keyset differs')
        protocol_require(values['pid'] == expected_pid and values['node'] == '/uwvm_test/secure_witness' and
                         values['type'] == NATIVE_TYPE and values['sub_topic'] == '/uwvm_test/pubString' and
                         values['pub_topic'] == '/uwvm_test/subString', 'native identity/topic association differs')
        for key,value in values.items():
            if key.endswith('_gid'):
                # The actual pinned Jazzy SDK declares RMW_GID_STORAGE_SIZE
                # as 16. Native code asserts that exact width and emits every
                # byte; accepting another width would hide a mismatched ABI.
                protocol_require(isinstance(value,str) and re.fullmatch(r'[0-9a-f]{32}',value) is not None and
                                 value != '0'*32, 'native GID must be nonzero complete lowercase 16-byte value')
        if name == order[0]:
            protocol_require((values['enclave'],values['domain'],values['rmw'],values['out_bytes'],values['in_bytes']) ==
                             ('/witness',DOMAIN,'rmw_fastrtps_cpp',31,23), 'native used-context/byte metadata differs')
        else:
            protocol_require(values['matched_publishers'] == 1 and values['matched_subscribers'] == 1 and
                             0 <= values['received'] <= 2, 'native exact-two-admitted-publish bound differs')
            gids = {values[key] for key in ('witness_sub_gid','witness_pub_gid','gateway_pub_gid','gateway_sub_gid')}
            protocol_require(len(gids) == 4 and values['witness_pub_gid'] == rows[order[0]]['pub_gid'], 'distinct native endpoints or local publication GID differ')
            if name == order[1]:
                protocol_require(values['sent'] == 0, 'native matching marker precedes replies')
            else:
                protocol_require(1 <= values['received'] <= 2 and 1 <= values['sent'] <= 100 and
                                 (values['mismatched'],values['denied_native'],values['permission_events'],values['out_bytes'],values['in_bytes']) ==
                                 (0,1,1,31,23), 'native final byte/counter/permission contract differs')
                previous = rows[order[1]]
                protocol_require(values['received'] >= previous['received'] and all(values[key] == previous[key]
                                 for key in ('witness_sub_gid','witness_pub_gid','gateway_pub_gid','gateway_sub_gid')),
                                 'native final endpoint/counter association changed')
        rows[name] = values
    protocol_require(not complete or tuple(rows) == order, 'complete native protocol is unavailable')
    return rows


def parse_guest_markers(text, complete=True):
    """Allow ordinary Worker diagnostics, requiring two exact Guest markers."""
    protocol_require(isinstance(text,str) and len(text) <= 16*1024**2, 'bounded Guest log required')
    protocol_require(not complete or bool(text) and text.endswith('\n'), 'complete Guest log must retain terminal LF')
    lines = text.split('\n')[:-1] if text.endswith('\n') else text.split('\n')
    if text and not text.endswith('\n'):
        lines = lines[:-1]
    expected = ('SECURE_ENDPOINTS_CREATED','PASS_SECURE_GUEST')
    selected = [line for line in lines if line.startswith(('SECURE_ENDPOINTS','PASS_SECURE','FAIL_SECURE'))]
    protocol_require(tuple(selected) == expected[:len(selected)] and len(selected) <= 2, 'Guest marker content/order/repetition differs')
    protocol_require(not complete or tuple(selected) == expected, 'complete Guest byte/sentinel/close proof missing')
    return list(selected)


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
                     'inherited_override_cleared':True,'direct_used_root_observed':False},
                     'Gateway source-selected Enforce boundary required')
    protocol_require(context['witness']['attested'] is True and context['gateway']['source_selected'] is True and
                     context['gateway']['inherited_override_cleared'] is True and context['gateway']['direct_used_root_observed'] is False,
                     'context attestation/provenance leaves require exact booleans')
    products,roles = row.get('products',{}),row.get('roles',{})
    protocol_require(set(products) == set(ROLE_LABELS) and set(roles) == set(ROLE_LABELS), 'all three independent products/processes required')
    births = set()
    for role,label in ROLE_LABELS.items():
        product,process = products[role],roles[role]
        protocol_require(product.get('qualified') is True and valid_identity(product.get('origin')) and
                         valid_identity(product.get('snapshot')) and product['origin']['sha256'] == product['snapshot']['sha256'] and
                         (product['origin']['device'],product['origin']['inode']) !=
                         (product['snapshot']['device'],product['snapshot']['inode']), 'separate immutable qualified snapshot '+role)
        protocol_require(type(process.get('pid')) is int and process['pid'] > 0 and
                         type(process.get('starttime_ticks')) is int and process['starttime_ticks'] > 0 and
                         process.get('membership') == group['membership'] and process.get('live_before_G') is True,
                         'exact live process association '+role)
        protocol_require(process.get('mapped_product') == product['snapshot'], 'mapped role product association '+role)
        proof = process.get('proof',{})
        actual,sample = proof.get('actual_stopped_ARM',{}),proof.get('target_proof',{})
        protocol_require(proof.get('current_mapping_check') is True and
                         (actual.get('pid'),actual.get('starttime_ticks')) == (process['pid'],process['starttime_ticks']) and
                         actual.get('state') in ('T','t') and sample.get('live_cgroup_procs') is True and
                         sample.get('membership') == group['membership'] and label in sample.get('mapped_arm_targets',[]) and
                         sample.get('kernel_exe_elf',{}).get('e_machine') == 62 and
                         sample.get('kernel_exe_elf',{}).get('sha256') == row.get('qemu_sha256') and
                         (sample.get('pid'),sample.get('starttime_ticks')) == (process['pid'],process['starttime_ticks']) and
                         proof.get('ancestry') == sample.get('ancestry') and bool(proof.get('ancestry')),
                         'current actual stopped ARM/QEMU proof '+role)
        births.add((process['pid'],process['starttime_ticks']))
    protocol_require(len(births) == 3 and len({value[0] for value in births}) == 3 and roles['witness'].get('pre_init_mapping') is True and
                     roles['gateway'].get('pre_init_mapping') is False, 'independent native births and honest initialization boundaries required')
    markers = row.get('markers',{})
    parsed = parse_witness_markers(markers.get('witness_text',''),roles['witness']['pid'])
    parse_guest_markers(markers.get('guest_text',''))
    protocol_require(markers.get('witness') == parsed, 'parsed native evidence differs from its raw fixed marker log')
    gates = row.get('gates',[])
    protocol_require(isinstance(gates,list) and [(item.get('role'),item.get('byte')) for item in gates] ==
                     [('witness','I'),('worker','G'),('worker','M'),('witness','F')], 'exact native/Guest gate sequence required')
    for item in gates:
        role = roles[item['role']]
        protocol_require((item.get('pid'),item.get('starttime_ticks')) == (role['pid'],role['starttime_ticks']) and
                         item.get('current_birth_mapping_verified') is True and item.get('sent') is True,
                         'gate must bind the actual current role before release')
    protocol_require(gates[1].get('prior_marker') == 'READY_SECURE_WITNESS' and
                     gates[2].get('prior_marker') == 'READY_SECURE_MATCHED+SECURE_ENDPOINTS_CREATED' and
                     gates[3].get('prior_marker') == 'PASS_SECURE_GUEST' and type(gates[3].get('guest_exit')) is int and gates[3].get('guest_exit') == 0,
                     'each gate requires its actual semantic predecessor')
    commands = row.get('commands',{})
    protocol_require(all(type(commands.get(key)) is int and commands[key] == 0 for key in ('worker_exit','witness_exit','generation_exit')) and
                     commands.get('signed_verification_exits') and
                     all(type(value) is int and value == 0 for value in commands['signed_verification_exits']) and
                     (type(commands.get('gateway_exit_after_worker')) is int and commands['gateway_exit_after_worker'] == 0 or
                      commands.get('gateway_exit_after_worker') == 'OWNED_CLEANUP'), 'successful actual generation/native/Guest commands required')
    protocol_require(row.get('native_denial') == {'attested':True,'denied_native':1,'permission_events':1,
                     'raw_error_body_captured':False,'predicate_bound_to_qualified_witness':True},
                     'native permission-specific source-bound event attestation required')
    protocol_require(type(row['native_denial']['denied_native']) is int and type(row['native_denial']['permission_events']) is int,
                     'native permission counters must be integers rather than booleans')
    protocol_require(row['native_denial']['attested'] is True and row['native_denial']['raw_error_body_captured'] is False and
                     row['native_denial']['predicate_bound_to_qualified_witness'] is True,
                     'native attestation/privacy provenance leaves require exact booleans')
    cleanup = row.get('cleanup',{})
    protocol_require(cleanup.get('owned_processes') == [] and cleanup.get('mounts') == [] and
                     cleanup.get('socket_absent') is True and cleanup.get('secrets_removed') is True and
                     cleanup.get('domain_lock_removed') is True, 'complete owned cleanup required')
    protocol_require(row.get('G09_complete') is False, 'positive Enforce does not close combined revocation/reconnect gap')
    return True


def cmake():
    """One original source, explicit full dependencies and pre-init ARM barrier."""
    packages = ('rclcpp','rcl','rcutils','rmw','std_msgs','rosidl_typesupport_cpp')
    lines = ['cmake_minimum_required(VERSION 3.16)', 'project(g09_signed_witness LANGUAGES CXX)',
             'set(CMAKE_CXX_STANDARD 20)', 'set(CMAKE_CXX_STANDARD_REQUIRED ON)',
             'find_package(ament_cmake REQUIRED)', 'find_package(fastrtps CONFIG REQUIRED)']
    lines += ['find_package('+name+' REQUIRED)' for name in packages]
    lines += ['add_executable('+WITNESS+' /work/'+WITNESS_SOURCE+')',
              'target_include_directories('+WITNESS+' PRIVATE /work/third-parties/fast_io/include)',
              'target_compile_options('+WITNESS+' PRIVATE -O1 -g0 -UNDEBUG -std=gnu++20 -MD -MF /tmp/build/CMakeFiles/'+WITNESS+'.dir/g09-required-full-sdk.d -include /tmp/evidence-stop.hpp)',
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
        explicit = '/tmp/build/CMakeFiles/'+WITNESS+'.dir/g09-required-full-sdk.d'
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
    connection = {'node-name':'uwvm_ros2_gateway_probe','namespace':'/uwvm_test','domain-id':str(DOMAIN),
                  'rmw-implementation':'rmw_fastrtps_cpp','security-mode':'enforce',
                  'keystore':KEYSTORE,'enclave':'/gateway'}
    common = {'max-message-bytes':'64','max-rate-hz':'100','burst':'8','max-bytes-per-second':'65536',
              'max-queue-bytes':'4096','overflow-policy':'drop-oldest','validator':'none','audit':'allow-and-deny',
              'audit-max-rate-hz':'100','audit-burst':'16','audit-max-records':'10000','audit-max-bytes':'1048576',
              'max-inflight':'1','qos.reliability':'reliable','qos.durability':'volatile','qos.history':'keep-last',
              'qos.depth':'8','qos.deadline':'infinite','qos.lifespan':'infinite','qos.liveliness':'automatic',
              'qos.liveliness-lease-duration':'infinite'}
    grants = [{'alias':alias,'operation':operation,'resource':'/uwvm_test/'+alias,'type':NATIVE_TYPE,
               'identity':NATIVE_IDENTITY,'limits':{**common,resource_limit:'1'}}
              for alias,operation,resource_limit in
              (('pubString','topic.publish','max-publishers'),('subString','topic.subscribe','max-subscribers'))]
    return connection,grants


def worker_arguments(worker, connection, grants, socket_path, wasm):
    argv = [str(worker),'--ros2-single-create','main','--ros2-single-enable','main']
    for key,value in {**connection,'gateway-socket':str(socket_path),'gateway-uid':str(os.getuid()),
                      'connect-timeout-ms':'10000','audit-deny-max-rate-hz':'100','audit-deny-burst':'16',
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
             'socket /tmp/control/ros2.sock','timeout-ms 10000','max-sessions 2',
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
                     'signed native grants must name only exact domain74')
    expected_pub = 'rt/uwvm_test/'+('pubString' if role == 'gateway' else 'subString')
    expected_sub = 'rt/uwvm_test/'+('subString' if role == 'gateway' else 'pubString')
    publish = [item.text for item in grant.findall('./allow_rule/publish/topics/topic')]
    subscribe = [item.text for item in grant.findall('./allow_rule/subscribe/topics/topic')]
    protocol_require(expected_pub in publish and expected_sub in subscribe and len(publish)+len(subscribe) <= 128 and
                     all(isinstance(value,str) and len(value) <= 512 for value in publish+subscribe),
                     'signed exact complementary native Topic resources missing')
    forbidden = 'rt/uwvm_test/not_permitted'
    protocol_require(not any(fnmatch.fnmatchcase(forbidden,value) for value in publish) and
                     not any(value.startswith('rt/uwvm_test/') and value != expected_pub for value in publish) and
                     not any(value.startswith('rt/uwvm_test/') and value != expected_sub for value in subscribe) and
                     not any(fnmatch.fnmatchcase(forbidden,value) for value in subscribe),
                     'signed native publication allows forbidden/asymmetric user resource')
    protocol_require(set(publish).issubset({expected_pub,'ros_discovery_info'}) and
                     set(subscribe).issubset({expected_sub,'rt/parameter_events','ros_discovery_info'}),
                     'signed fixture contains an unknown or wildcard native resource')
    # The unchanged policy explicitly subscribes to parameter_events. Jazzy
    # SROS2's FastDDS template adds only ros_discovery_info for native graph
    # discovery. Neither native resource becomes an internal Guest grant.
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
                         not rule.findall('./domains/id_range'), 'signed governance must bind only exact domain74')
        topics = rule.findall('./topic_access_rules/topic_rule')
        for name in ('rt/uwvm_test/pubString','rt/uwvm_test/subString','rt/uwvm_test/not_permitted'):
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
                fresh = path.with_name(path.name+'.g09-materialized')
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
                      'One raw Wasm32 Guest plus independent signed FastDDS witness; separate revoked reconnect gap remains pending'}
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
        # Jazzy's qualified setup hook adds the default discovery range even
        # to this literal clean environment. Keep the independent witness's
        # strict absence check: remove only that hook default after setup and
        # verify removal before exec. Ordinary tools and the Gateway retain
        # their existing launch path, including the deliberate Gateway input
        # override used to exercise production enclave selection.
        script = 'source /opt/ros/jazzy/setup.bash'
        if witness:
            script += ' && unset ROS_AUTOMATIC_DISCOVERY_RANGE && [[ ! -v ROS_AUTOMATIC_DISCOVERY_RANGE ]]'
        return ['/usr/bin/chroot',str(root),'/bin/bash','--noprofile','--norc','-c',
                script+' && exec "$@"','g09-signed-native',*map(str,arguments)]
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
        raise RuntimeError('process ended before required marker: '+key)

    try:
        protocol_require(sys.platform == 'linux' and os.geteuid() == 0, 'BLOCKED root Linux private namespace required')
        protocol_require(os.readlink('/proc/self/ns/mnt') != os.readlink(f'/proc/{os.getppid()}/ns/mnt'), 'BLOCKED launch under unshare -m')
        protocol_require(Path(__file__).resolve() == source/'tests/ros_aarch64/run_g09_guest_enforce.py', 'execute only exact frozen source controller')
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
                             any(name.startswith('/opt/') and name.endswith('/fastdds/dds/log/Log.hpp') for name in inputs) and
                             any(name.startswith('/work/third-parties/fast_io/') for name in inputs),
                             'original full-MD native fixture/FastDDS/fast_io/system/barrier closure required')
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
            products['gateway'] = snapshot(args.gateway,temporary/'gateway',ROLE_LABELS['gateway'])
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
            control = Path(tempfile.mkdtemp(prefix='uwvm-g09-',dir='/tmp'));control.chmod(0o700)
            control_info = control.stat();report['control_directory'] = {'path':str(control),'device':control_info.st_dev,'inode':control_info.st_ino}
            (temporary/'control').mkdir();checked(['mount','--bind',control,root/'tmp/control']);mounts.append(root/'tmp/control')
            lock_path = Path('/tmp/uwvm-arm-ros-domain-74.lock')
            fd = os.open(lock_path,os.O_CREAT|os.O_EXCL|os.O_WRONLY,0o600)
            try: os.write(fd,str(os.getpid()).encode());identity = os.fstat(fd)
            finally: os.close(fd)
            domain_lock = (lock_path,identity.st_dev,identity.st_ino)
            file_pin(source/POLICY_SOURCE);file_pin(source/WAT_SOURCE);file_pin(args.wat2wasm)
            python = rooted(sdk,'/usr/bin/python3');openssl = rooted(sdk,'/usr/bin/openssl')
            evidence.target('native-python',python);file_pin(python);evidence.target('native-openssl',openssl);file_pin(openssl)
            credentials = temporary/'g09-keys';credentials.mkdir(mode=0o700,exist_ok=False)
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
            worker,worker_row = start('worker',['/bin/bash','--noprofile','--norc','-c','kill -STOP $$; exec "$@"','g09-worker',*worker_command],
                [ROLE_LABELS['worker']],120,stdin=True,environment={'PATH':'/usr/bin:/bin','LANG':'C.UTF-8'})
            until = time.monotonic()+5
            while time.monotonic()<until and process_identity(worker.pid)['state'] not in ('T','t'): guard();time.sleep(.001)
            protocol_require(process_identity(worker.pid)['state'] in ('T','t'), 'reserved exact Worker launch PID did not pause')
            cfg = control/'gateway.conf';cfg.write_text(gateway_configuration(worker.pid,connection,grants));cfg.chmod(0o600)
            cfg_archive = out/'gateway.conf';shutil.copy2(cfg,cfg_archive);file_pin(cfg_archive)
            report['policy'] = {'connection':connection,'grants':grants,'worker_argv':argv,'worker_execution_argv':worker_command,
                'gateway_config':read_file_binding(cfg),'gateway_config_archive':read_file_binding(cfg_archive),'gateway_config_text':cfg.read_text()}
            gateway_env = native_environment(True)
            gateway_env.update(gateway_loader['environment'])
            gateway_env['ROS_SECURITY_ENCLAVE_OVERRIDE'] = '/witness'
            report['gateway_test_environment'] = {'explicit_override':'/witness','configured_enclave':'/gateway',
                'ambient_environment_inherited':False,'direct_used_root_observed':False}
            gateway,gateway_row = start('gateway',native(['/tmp/gateway','/tmp/control/gateway.conf']),[ROLE_LABELS['gateway']],120,
                enforce=True,environment=gateway_env)
            communication_roles = (worker,gateway)
            gateway_proof = mapped_stop(gateway,gateway_row,ROLE_LABELS['gateway'])
            until = time.monotonic()+30
            while time.monotonic()<until and not (control/'ros2.sock').exists():
                guard();protocol_require(gateway.poll() is None, 'Gateway exited before signed socket readiness');time.sleep(.002)
            socket_info = (control/'ros2.sock').lstat()
            protocol_require(stat.S_ISSOCK(socket_info.st_mode) and socket_info.st_uid == os.getuid() and stat.S_IMODE(socket_info.st_mode) == 0o600,
                             'actual owned authenticated IPC socket missing/insecure')
            report['socket'] = {'path':str(control/'ros2.sock'),'device':socket_info.st_dev,'inode':socket_info.st_ino,'worker_pid':worker.pid}
            witness_child,witness_row = start('witness',native(['/tmp/'+WITNESS,KEYSTORE],witness=True),[WITNESS],100,stdin=True,enforce=True)
            witness_proof = mapped_stop(witness_child,witness_row,WITNESS,pre_stopped=True)
            send_gate(witness_child,witness_row,'witness','I',witness_proof,'ARM_WITNESS_PREINIT_STOP')
            witness_pid = witness_proof['actual_stopped_ARM']['pid']
            wait_marker(witness_child,witness_row,parse_witness_markers,'READY_SECURE_WITNESS',witness_pid)
            reservation = next(item for item in owned.rows.values() if item['identity']['pid'] == worker.pid and item['label'] == 'worker')
            owned.send(reservation,signal.SIGCONT)
            worker_proof = mapped_stop(worker,worker_row,ROLE_LABELS['worker'])
            # All three participants must still be the owned live images at
            # this boundary. Their earlier stopped samples alone do not prove
            # that the Gateway or witness survived until the Guest's G byte.
            guard();check_pins()
            report['before_guest_G'] = {role:live_role(child,row,role,proof)
                for role,child,row,proof in (('worker',worker,worker_row,worker_proof),
                    ('gateway',gateway,gateway_row,gateway_proof),('witness',witness_child,witness_row,witness_proof))}
            send_gate(worker,worker_row,'worker','G',worker_proof,'READY_SECURE_WITNESS')
            wait_marker(worker,worker_row,parse_guest_markers,'SECURE_ENDPOINTS_CREATED')
            wait_marker(witness_child,witness_row,parse_witness_markers,'READY_SECURE_MATCHED',witness_pid)
            send_gate(worker,worker_row,'worker','M',worker_proof,'READY_SECURE_MATCHED+SECURE_ENDPOINTS_CREATED')
            worker.stdin.close();finish(worker,worker_row)
            guest_raw,guest_pin = read_file_binding(worker_row['log'],limit=16*1024**2,retain=True)
            pins.append(guest_pin);guest_text = guest_raw.decode('utf-8','strict');parse_guest_markers(guest_text)
            send_gate(witness_child,witness_row,'witness','F',witness_proof,'PASS_SECURE_GUEST',{'guest_exit':worker.returncode})
            witness_child.stdin.close();finish(witness_child,witness_row)
            witness_raw,witness_pin = read_file_binding(witness_row['log'],limit=16*1024**2,retain=True)
            pins.append(witness_pin);witness_text = witness_raw.decode('ascii','strict');native_markers = parse_witness_markers(witness_text,witness_pid)
            # Principal-bound Gateway may exit automatically once the exact
            # Worker pidfd becomes readable. It must not fail before Guest exit.
            if gateway.poll() is not None:
                protocol_require(gateway.returncode == 0, 'Gateway exited with failure after bound Worker retirement')
                gateway_row.update(status='PASS',returncode=0,log_sha256=digest(gateway_row['log']))
                gateway_exit = 0
            else: gateway_exit = 'OWNED_CLEANUP'
            roles = {}
            for role,proof in (('worker',worker_proof),('gateway',gateway_proof),('witness',witness_proof)):
                actual = proof['actual_stopped_ARM']
                roles[role] = {'pid':actual['pid'],'starttime_ticks':actual['starttime_ticks'],
                    'membership':proof['live_cgroup_membership'],'proof':proof,'mapped_product':products[role]['snapshot'],
                    'live_before_G':True,'pre_init_mapping':role == 'witness'}
            completion = {'kind':KIND,'format_version':1,'mode':'execute','source_manifest_sha256':manifest['sha256'],
                'qemu_sha256':report['qemu']['sha256'],'products':products,'roles':roles,'signed':signed,
                'markers':{'witness_text':witness_text,'witness':native_markers,'guest_text':guest_text},'gates':report['gates'],
                'native_context':{'witness':{'attested':True,'enclave':'/witness','domain':DOMAIN,'rmw':'rmw_fastrtps_cpp','security':'Enforce','root':KEYSTORE+'/enclaves/witness'},
                    'gateway':{'source_selected':True,'enclave':'/gateway','domain':DOMAIN,'rmw':'rmw_fastrtps_cpp','security':'Enforce',
                        'root':KEYSTORE+'/enclaves/gateway','inherited_override_cleared':True,'direct_used_root_observed':False}},
                'native_denial':{'attested':True,'denied_native':1,'permission_events':1,'raw_error_body_captured':False,
                    'predicate_bound_to_qualified_witness':True},
                'commands':{'generation_exit':generate['returncode'],'signed_verification_exits':[item['returncode'] for item in verification],
                    'worker_exit':worker.returncode,'witness_exit':witness_child.returncode,'gateway_exit_after_worker':gateway_exit},'G09_complete':False}
            protocol_require(credential_ledger(credentials) == ledger, 'signed keystore changed during actual combined native/Guest communication')
            signed['keystore_preserved_before_removal'] = True
            report['completion'] = completion;report['actual_guest_behavior'] = 'OBSERVED_PENDING_FINAL_GUARDS';status = 'PASS'
    except BaseException as error:
        report['error'] = traceback.format_exc()
        status = 'BLOCKED' if 'BLOCKED' in str(error) else 'FAIL'
    finally:
        if owned is not None:
            try:
                cleanup = retained_popen_cleanup(owned,children);report['process_cleanup'] = cleanup
                if cleanup['status'] != 'PASS': status = 'FAIL'
            except BaseException: report['process_cleanup_error'] = traceback.format_exc();status = 'FAIL'
        if credentials is not None:
            try:
                protocol_require(credentials == temporary/'g09-keys', 'only this intended fresh signed keystore may be removed')
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
        report['status'] = 'PREPARED' if status == 'PASS' and args.mode == 'prepare' else status
        save()
    print(report['status'],out/'report.json',flush=True)
    return 0 if report['status'] in ('PASS','PREPARED') else 77 if report['status'] == 'BLOCKED' else 1


if __name__ == '__main__':
    raise SystemExit(main())
