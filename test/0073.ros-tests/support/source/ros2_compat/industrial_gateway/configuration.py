"""Write explicit original-PID-bound policies for isolated synthetic peers.

This file is executed only by the external containment owner at the original
child's stopped checkpoint. Nothing here installs a Guest Runtime policy. The
empty policy grants zero capabilities; the other policy enumerates each actual
registered interface with its exact installed native and canonical identities.
"""
import json
from pathlib import Path
import re


def prepare(directory: Path, worker_pid: int, worker_uid: int, native_export: Path,
            registry: Path, selection: Path, *, socket_directory: str) -> dict:
    assert type(worker_pid) is int and worker_pid > 0 and type(worker_uid) is int and worker_uid >= 0
    assert directory.is_absolute() and directory.is_dir() and directory.stat().st_mode & 0o077 == 0
    assert Path(socket_directory).is_absolute() and str(Path(socket_directory)) == socket_directory
    assert '..' not in Path(socket_directory).parts and not any(c in socket_directory for c in '\0\n\r')
    source = json.loads(native_export.read_bytes()); table = json.loads(selection.read_bytes())
    assert source['format'] == 'uwvm-ros-industrial-native-pre-rihs-protocol-v1' and table['default_authority'] == []
    records = {}
    for name, native, codec in re.findall(r'"([a-z0-9_/A-Za-z]+)","(RIHS01_[0-9a-f]{64})","(UWVMROS1_[0-9a-f]{64})"', registry.read_text()):
        assert records.setdefault(name, (native, codec)) == (native, codec)
    qos = dict(reliability='reliable', durability='volatile', history='keep-last', depth='16',
               deadline='infinite', lifespan='infinite', liveliness='automatic',
               **{'liveliness-lease-duration': 'infinite'})
    audit = {'max-rate-hz': '10000', 'burst': '16', 'max-bytes-per-second': '16777216',
             'audit': 'allow-and-deny', 'audit-max-rate-hz': '10000', 'audit-burst': '256',
             'audit-max-records': '65536', 'audit-max-bytes': '16777216'}
    grants = []; count = 0

    def grant(root, suffix, operation, resource_suffix, name, limits):
        nonlocal count
        native, codec = records[name]
        assert native == source['types'][name]['official']['native_identity']
        alias = root + suffix; fields = dict(audit, **limits, **{'adapter-codec-identity': codec})
        grants.append('grant ' + ' '.join((alias, operation, '/uwvm_industrial/' + root + resource_suffix, name, native)))
        grants.extend(f'set {alias} {key} {value}' for key, value in sorted(fields.items())); count += 1

    for row in table['cases']:
        root, name, kind = row['alias'], row['type'], row['kind']
        assert re.fullmatch(r'v[0-9]+', root)
        if kind == 'message':
            for suffix, operation, resource_suffix in (('_pub', 'topic.publish', '_out'), ('_sub', 'topic.subscribe', '_in')):
                limits = {'max-message-bytes': '32768', 'max-queue-bytes': '1048576', 'overflow-policy': 'reject',
                          'validator': 'none', 'max-inflight': '4',
                          'max-publishers' if operation == 'topic.publish' else 'max-subscribers': '1'}
                limits.update({'qos.' + key: value for key, value in qos.items()})
                grant(root, suffix, operation, resource_suffix, name, limits)
        elif kind == 'service':
            for suffix, operation, resource_suffix in (('_call', 'service.call', '_peer'),
                    ('_adv', 'service.advertise', '_gateway'), ('_respond', 'service.respond', '_gateway')):
                limits = {'timeout-ms': '10000', 'max-inflight': '4', 'max-request-bytes': '32768',
                          'max-response-bytes': '32768', 'validator': 'none'}
                if operation == 'service.advertise':
                    limits.update({'max-services': '1', 'max-event-queue-depth': '16',
                                   'max-event-queue-bytes': '1048576', 'overflow-policy': 'reject'})
                limits.update({'qos.' + key: value for key, value in qos.items()})
                grant(root, suffix, operation, resource_suffix, name, limits)
        else:
            assert kind == 'action'
            for suffix, operation, resource_suffix in (('_send', 'action.send-goal', '_peer'),
                    ('_feedback', 'action.receive-feedback', '_peer'), ('_result', 'action.receive-result', '_peer'),
                    ('_cancel', 'action.cancel-own-goal', '_peer'), ('_adv', 'action.advertise', '_gateway'),
                    ('_respond', 'action.respond', '_gateway')):
                limits = {'max-active-goals': '2', 'max-actions': '1', 'timeout-ms': '10000',
                          'max-goal-bytes': '32768', 'max-feedback-bytes': '32768', 'max-result-bytes': '32768',
                          'cancel-scope': 'own' if operation == 'action.cancel-own-goal' else 'not-applicable',
                          'max-event-queue-depth': '16', 'max-event-queue-bytes': '1048576',
                          'overflow-policy': 'reject', 'validator': 'none'}
                for channel in ('goal-service', 'cancel-service', 'result-service', 'feedback', 'status'):
                    limits.update({'qos.' + channel + '.' + key: value for key, value in qos.items()})
                limits['qos.status.durability'] = 'transient-local'
                grant(root, suffix, operation, resource_suffix, name, limits)
    files = {}
    for variant, rows in (('empty', []), ('granted', grants)):
        common = ['backend ros2', f'worker-uid {worker_uid}', f'worker-pid {worker_pid}',
                  f'socket {socket_directory}/{variant}.sock', 'timeout-ms 10000', 'max-sessions 4',
                  'audit-max-records-per-second 10000', 'audit-max-records 65536', 'audit-max-bytes 16777216',
                  f'connection node-name gateway_{variant}', 'connection namespace /uwvm_industrial',
                  'connection domain-id 97', 'connection rmw-implementation rmw_fastrtps_cpp',
                  'connection security-mode disabled-test', 'connection keystore not-applicable',
                  'connection enclave not-applicable']
        path = directory / (variant + '.conf')
        with path.open('x') as stream:
            stream.write('\n'.join(common + rows) + '\n')
        path.chmod(0o400); files[variant] = str(path)
    return dict(files=files, worker_pid=worker_pid, worker_uid=worker_uid,
                grant_counts=dict(empty=0, granted=count), selection=table, Guest_authority=[])
