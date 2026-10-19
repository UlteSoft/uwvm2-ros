#!/usr/bin/env python3
"""Freeze explicit deployment DATA for the original-default Node example.

No aliases are derived from names, and no right is created by this file. Actual
Host policy and same-native endpoint inspection still enforce all permissions,
types and effective QoS. Input values are deployment declarations, not queried
native evidence. This operator has no subprocess/compiler/native execution.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import stat

ENDPOINTS = ('rosout', 'parameter_events', 'describe_parameters', 'get_parameters',
             'get_parameter_types', 'list_parameters', 'set_parameters', 'set_parameters_atomically', 'get_type_description', 'parameter_events_subscription')
LOGGERS = ('default_control_node', 'rcl', 'rclcpp')

def read(path):
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or first.st_size > 65536:
            raise ValueError('bounded ordinary deployment DATA required')
        parts, count = [], 0
        while True:
            part = os.read(fd, 65537-count)
            if not part:
                break
            count += len(part)
            if count > 65536:
                raise ValueError('deployment grew beyond bound')
            parts.append(part)
        after, named = os.fstat(fd), os.lstat(path)
        def key(s):
            return s.st_dev, s.st_ino, s.st_size, s.st_mode, s.st_mtime_ns, s.st_ctime_ns, s.st_nlink
        if key(first) != key(after) or key(first) != key(named) or count != first.st_size:
            raise ValueError('deployment identity or extent drift')
        return b''.join(parts)
    finally:
        os.close(fd)

def run(path, expected_sha, output):
    raw = read(path)
    if hashlib.sha256(raw).hexdigest() != expected_sha:
        raise ValueError('explicit reviewed deployment SHA differs')
    def pairs(rows):
        result = {}
        for k, v in rows:
            if k in result:
                raise ValueError('duplicate deployment key')
            result[k] = v
        return result
    d = json.loads(raw, object_pairs_hook=pairs)
    if set(d) != {'format_version', 'domain_id', 'system_clock_alias', 'steady_clock_alias', 'loggers', 'endpoints'} or type(d['format_version']) is not int or d['format_version'] != 1:
        raise ValueError('closed explicit deployment schema required')
    def integer(value, maximum):
        if type(value) is not int or not 0 <= value <= maximum:
            raise ValueError('exact bounded nonnegative integer required')
        return value
    integer(d['domain_id'], 232)
    aliases = set()
    def alias(value):
        if not isinstance(value, str) or not 1 <= len(value.encode('utf-8')) <= 255 or '\0' in value or value in aliases:
            raise ValueError('distinct explicit nonempty UTF-8 alias required')
        aliases.add(value)
        # UTF-8 bytes, not JSON surrogate escapes which are invalid C++ UCNs.
        return '"' + ''.join('\\x' + format(b, '02x') for b in value.encode('utf-8')) + '"'
    system, steady = alias(d['system_clock_alias']), alias(d['steady_clock_alias'])
    if set(d['loggers']) != set(LOGGERS) or set(d['endpoints']) != set(ENDPOINTS):
        raise ValueError('complete exact original default endpoint/logger selection required')
    logger_rows = []
    for name in LOGGERS:
        values = d['loggers'][name]
        if not isinstance(values, list) or len(values) != 5:
            raise ValueError('five explicit severity aliases required per selected logger')
        logger_rows.append('{' + json.dumps(name) + ',{' + ','.join(alias(v) for v in values) + '}}')
    endpoint_rows = []
    for i, name in enumerate(ENDPOINTS):
        row = d['endpoints'][name]
        if not isinstance(row, dict) or set(row) != {'aliases', 'effective_qos'} or not isinstance(row['aliases'], list) or len(row['aliases']) != (1 if i < 2 or i == 9 else 2):
            raise ValueError('actual publisher or service alias pair required')
        selected = [alias(v) for v in row['aliases']]
        if len(selected) == 1:
            selected.append('nullptr')
        q = row['effective_qos']
        keys = {'history', 'depth', 'reliability', 'durability', 'deadline', 'lifespan', 'liveliness', 'liveliness_lease_duration', 'avoid_ros_namespace_conventions'}
        if not isinstance(q, dict) or set(q) != keys:
            raise ValueError('complete effective QoS required')
        for field in ('history', 'reliability', 'durability'):
            if integer(q[field], 2) not in (1, 2):
                raise ValueError('effective policy may not be system-default or unknown')
        if integer(q['liveliness'], 3) not in (1, 2, 3):
            raise ValueError('actual effective liveliness required')
        integer(q['depth'], 2**32-1)
        if type(q['avoid_ros_namespace_conventions']) is not bool:
            raise ValueError('exact namespace-conventions bool required')
        body = '[]{rmw_qos_profile_t v{};'
        for field, ctype in [('history', 'rmw_qos_history_policy_t'), ('reliability', 'rmw_qos_reliability_policy_t'), ('durability', 'rmw_qos_durability_policy_t'), ('liveliness', 'rmw_qos_liveliness_policy_t')]:
            body += 'v.' + field + '=static_cast<' + ctype + '>(' + str(q[field]) + ');'
        body += 'v.depth=' + str(q['depth']) + 'u;'
        for field in ('deadline', 'lifespan', 'liveliness_lease_duration'):
            value = q[field]
            if not isinstance(value, dict) or set(value) != {'sec', 'nsec'}:
                raise ValueError('complete duration required; normalization is not silently inferred')
            body += 'v.' + field + '={UINT64_C(' + str(integer(value['sec'], 2**64-1)) + '),UINT64_C(' + str(integer(value['nsec'], 2**64-1)) + ')};'
        body += 'v.avoid_ros_namespace_conventions=' + str(q['avoid_ros_namespace_conventions']).lower() + ';return v;}()'
        endpoint_rows.append('{{' + ','.join(selected) + '},' + body + '}')
    header = '// Exact reviewed deployment declarations, never native query evidence or grants.\n#pragma once\n#include <uwvm_rmw/bridge.h>\n#include <uwvm_rmw_platform.h>\n#include <array>\nnamespace uwvm_default_control_deployment {\n'
    header += 'inline constexpr std::size_t domain_id=' + str(d['domain_id']) + ';\ninline constexpr char source_sha256[]="' + expected_sha + '";\n'
    header += 'inline constexpr char system_clock_alias[]=' + system + ';\ninline constexpr char steady_clock_alias[]=' + steady + ';\n'
    header += 'inline constexpr std::array<uwvm_rmw_platform_logger,3>loggers{{' + ','.join(logger_rows) + '}};\n'
    header += 'struct Endpoint{char const*aliases[2];rmw_qos_profile_t effective_qos;};\ninline const std::array<Endpoint,10>endpoints{{' + ','.join(endpoint_rows) + '}};\n}\n'
    if read(path) != raw:
        raise ValueError('terminal deployment drift')
    if not path.is_absolute() or not output.is_absolute() or output.exists():
        raise ValueError('absolute input and fresh output required')
    output.mkdir(parents=True)
    (output/'uwvm_default_control_deployment.hpp').write_text(header, encoding='utf-8')
    (output/'deployment-input.json').write_text(json.dumps(dict(status='FROZEN_DECLARATIONS_NOT_RIGHTS_OR_NATIVE_EVIDENCE', input_sha256=expected_sha, aliases=len(aliases), output=dict(bytes=len(header.encode()), sha256=hashlib.sha256(header.encode()).hexdigest())), indent=2)+'\n')

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', type=Path, required=True); parser.add_argument('--input-sha256', required=True); parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    try:
        run(args.input, args.input_sha256, args.out)
    except (OSError, ValueError, TypeError, KeyError) as error:
        parser.error(str(error))

if __name__ == '__main__':
    main()
