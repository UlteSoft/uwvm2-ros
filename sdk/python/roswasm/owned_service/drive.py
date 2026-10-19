"""Bounded stdin caller for the two actual frozen control applications.

Configuration selects existing granted aliases and full canonical components;
it cannot add Host rights. One input command drives at most one take/send.
Server responses are explicit subsequent commands, after take/ACK has returned.
"""
import json
import sys
from ..raw import Backend
from ..client import Session
from .applications import TypeQueryApplication, ParametersApplication
from .wire import uint

TYPE_QUERY = ('Ros2Type_description_interfacesSrvGetTypeDescription',)
PARAMETERS = ('Ros2Rcl_interfacesSrvDescribeParameters', 'Ros2Rcl_interfacesSrvGetParameterTypes',
              'Ros2Rcl_interfacesSrvGetParameters', 'Ros2Rcl_interfacesSrvListParameters',
              'Ros2Rcl_interfacesSrvSetParameters', 'Ros2Rcl_interfacesSrvSetParametersAtomically')
application = None


def line():
    value = sys.stdin.readline(131073)
    if not value:
        return None
    if len(value) > 131072 or not value.endswith('\n'):
        raise ValueError('one complete bounded caller line required')
    def pairs(rows):
        out = {}
        for key, item in rows:
            if key in out:
                raise ValueError('duplicate caller JSON key')
            out[key] = item
        return out
    return json.loads(value, object_pairs_hook=pairs, parse_constant=lambda _: (_ for _ in ()).throw(ValueError('nonfinite caller JSON')))


def wire(text, maximum):
    if type(text) is not str or len(text) > 2 * maximum or len(text) % 2 or any(c not in '0123456789abcdef' for c in text):
        raise ValueError('bounded lowercase complete canonical hex required')
    return bytes.fromhex(text)


def main(kind):
    global application
    from ..generated_control import control_services
    config = line()
    if (type(config) is not dict or set(config) != {'role','plans','history_entries'}
            or config['role'] not in ('client','server') or type(config['plans']) is not list):
        raise ValueError('explicit caller role/plans/history configuration required')
    symbols = TYPE_QUERY if kind == 'type_query' else PARAMETERS if kind == 'parameters' else ()
    if len(config['plans']) != len(symbols):
        raise ValueError('all selected original service plans required')
    plans = []
    for row, symbol in zip(config['plans'], symbols):
        if type(row) is not dict or set(row) != {'source','peer','availability','qos'} or type(row['qos']) is not list or len(row['qos']) != 2:
            raise ValueError('exact separate aliases and two QoS rows required')
        if config['role'] == 'server' and row['availability'] is not None:
            raise ValueError('availability belongs to a client third grant')
        plans.append({'source':row['source'],'peer':row['peer'],'availability':row['availability'],
                      'qos':tuple(wire(value,80) for value in row['qos']),
                      'definition':getattr(control_services,symbol)})
    session = Session(Backend(2), maximum_owners=32)
    app_type = TypeQueryApplication if kind == 'type_query' else ParametersApplication
    application = app_type(session, tuple(plans), server=config['role']=='server', history_entries=config['history_entries'])
    failure = application.command_failure
    # Bound cleanup methods exist before first open. Cleanup/status diagnostics
    # remain separate from the exact original raised exception.
    close, shutdown = application.close, session.shutdown
    primary = None
    try:
        application.open()
        while True:
            command = line()
            if command is None:
                break
            if type(command) is not dict or 'operation' not in command:
                raise ValueError('explicit command required')
            op = command['operation']
            if op == 'close' and set(command) == {'operation'}:
                break
            if set(command) - {'operation','service','wire','ticket','wait_ns'} or type(command.get('service')) is not int or not 0 <= command['service'] < len(plans):
                raise ValueError('bounded selected service required')
            index = command['service']
            endpoint = application.endpoints[index]
            definition = plans[index]['definition']
            if op == 'submit' and set(command) == {'operation','service','wire'} and not application.server:
                request = definition.request.decode(wire(command['wire'],definition.request.maximum))
                result = endpoint.submit(request)
                failure.record(op, index, result.status, result)
                print(json.dumps({'status':result.status,'ticket':result.ticket}))
            elif op == 'take' and set(command) == {'operation','service'}:
                result = endpoint.take()
                failure.record(op, index, result.status, result)
                item = result.delivery
                report = {'status':result.status,'discarded':result.discarded,'delivery':None}
                if item is not None:
                    codec = definition.request if application.server else definition.response
                    report['delivery'] = {'ticket':item.ticket,'sequence':item.sequence,'kind':item.kind,
                        'control':item.control,'control_flags':item.control_flags,'error':item.error,
                        'wire':None if item.control else codec.encode(item.value).hex(),
                        'information':None if item.information is None else {
                            'source_timestamp':item.information.source_timestamp,
                            'received_timestamp':item.information.received_timestamp,
                            'native_sequence':item.information.native_sequence,'writer':item.information.writer.hex()}}
                print(json.dumps(report))
            elif op == 'respond' and set(command) == {'operation','service','ticket','wire'} and application.server:
                uint(command['ticket'])
                response = definition.response.decode(wire(command['wire'],definition.response.maximum))
                status = endpoint.respond(command['ticket'],response)
                failure.record(op, index, status, status)
                print(json.dumps({'status':status}))
            elif op == 'abandon' and set(command) == {'operation','service','ticket'} and not application.server:
                status = endpoint.abandon(command['ticket'])
                failure.record(op, index, status, status)
                print(json.dumps({'status':status}))
            elif op == 'available' and set(command) == {'operation','service','wait_ns'} and not application.server:
                result = endpoint.available(command['wait_ns'])
                failure.record(op, index, result.status, result)
                print(json.dumps({'status':result.status,'available':result.available}))
            else:
                raise ValueError('unknown/wrong-role command; no fallback operation')
            if failure.failed:
                break
    except BaseException as error:
        primary = error
        application.primary = error
        failure.failed = True
        if failure.exception is None:
            failure.exception = error
        raise
    finally:
        # All cleanup is attempted independently; no printing, exception-attribute
        # growth or new exception wrapper masks the original primary here.
        try:
            failure.cleanup_status = close()
            if failure.cleanup_status != 0:
                failure.failed = True
        except BaseException as error:
            failure.cleanup_error = error
            failure.failed = True
        try:
            shutdown()
        except BaseException as error:
            failure.shutdown_error = error
            failure.failed = True
    # Retain the full original errno/objects on application; the real frozen
    # embedding accepts0..255, so use an unambiguous small process failure1.
    return 1 if failure.failed else 0
