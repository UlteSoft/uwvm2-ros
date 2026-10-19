"""Explicit deletion-only grants and independent version-2 tag-0 wire oracles."""
import struct

NODE = '/parameter_deletion/native'
NAME = 'value'
PRIOR_KINDS = ('bool', 'int64', 'float64', 'string', 'byte-array', 'bool-array',
               'int64-array', 'float64-array', 'string-array')
REJECTIONS = ('new-not-set', 'changed-not-set', 'concrete-delete', 'wrong-node',
              'wrong-name', 'stamp', 'fields', 'bytes')


def envelope(stamp=9):
    def field(value):
        return struct.pack('<I', len(value)) + value
    return struct.pack('<IIBBB', 123, stamp, 3, 2, 0) + field(NODE.encode()) + field(NAME.encode()) + struct.pack('<I', 0)


def expected_events(kind, scenario):
    if scenario in REJECTIONS and scenario != 'bytes':
        # The first valid notification is an ordered barrier on the same writer
        # as rejected inputs. The second comes from official SDK deletion.
        return [envelope(8), envelope(9)]
    return [envelope(9)]


def cases():
    rows = [{'kind': kind, 'scenario': 'prior-delete',
             'name': 'parameter_deletion_prior_' + kind.replace('-', '_')}
            for kind in PRIOR_KINDS]
    rows += [{'kind': 'bool', 'scenario': scenario,
              'name': 'parameter_deletion_' + scenario.replace('-', '_')}
             for scenario in (*REJECTIONS, 'permission-denied', 'no-capability')]
    return rows


def grant(kind='bool', scenario='prior-delete', alias='cap'):
    name = 'quota_value' if scenario == 'bytes' and alias == 'cap' else NAME
    return {'alias': alias, 'operation': 'param.subscribe-events',
            'resource': NODE + '/' + name, 'type': 'builtin/param-event-deletion-v1',
            'identity': 'builtin:param-event-deletion-v1',
            'limits': {'audit': 'allow-and-deny', 'audit-max-rate-hz': '100', 'audit-burst': '20',
                       'audit-max-records': '10000', 'audit-max-bytes': '1048576', 'timeout-ms': '2000',
                       'max-inflight': '2', 'max-rate-hz': '100', 'burst': '8',
                       'max-bytes-per-second': '1048576', 'max-parameter-bytes': '1' if name == 'quota_value' else '4096',
                       'validator': 'none', 'parameter-node': NODE, 'parameter-name': name,
                       'max-subscribers': '1', 'max-event-fields': '1' if scenario == 'fields' else '8',
                       'max-event-queue-depth': '8', 'max-event-queue-bytes': '32768', 'overflow-policy': 'reject',
                       'qos.reliability': 'reliable', 'qos.durability': 'volatile', 'qos.history': 'keep-last',
                       'qos.depth': '32', 'qos.deadline': 'infinite', 'qos.lifespan': 'infinite',
                       'qos.liveliness': 'automatic', 'qos.liveliness-lease-duration': 'infinite'}}


def grants(description, scalar_get_grant):
    scenario = description['scenario']
    if scenario == 'no-capability':
        return []
    if scenario == 'permission-denied':
        return [scalar_get_grant(2, 'param.get', 'bool')]
    rows = [grant(description['kind'], scenario)]
    if scenario == 'bytes':
        rows.append(grant(description['kind'], scenario, 'marker'))
    return rows
