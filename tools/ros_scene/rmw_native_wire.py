"""Check the native DDS Int32 fixture independently of any SDK or scene VM."""
import hashlib
import json
from pathlib import Path
import re


def require(ok, message):
    if not ok:
        raise ValueError(f'native wire: {message}')


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def peer_diagnostics(directory, outputs, name):
    checksum = outputs.get(name)
    if checksum is None:
        return {'status': 'no-pinned-log'}
    log = directory / name
    require(digest(log) == checksum, f'output identity differs for {name}')
    content = log.read_text()
    lines = content.splitlines()
    return {
        'status': 'examined', 'logSha256': checksum,
        'rosWarnLines': sum('[WARN]' in line for line in lines),
        'rosErrorLines': sum('[ERROR]' in line for line in lines),
        'rosFatalLines': sum('[FATAL]' in line for line in lines),
        'rcutilsErrorStateReports': content.count('] rcutils_set_error_state()'),
        'invalidDataSizeMentions': content.count('invalid data size'),
        'unterminatedStringMentions': content.count('string data is not null-terminated'),
    }


def verify(path, remaps=()):
    report = json.loads(path.read_bytes())
    require(report['scope'] == 'Native DDS wire baseline. Reliable Int32 pub/sub on isolated loopback. Plaintext.',
            'unrecognized fixture')
    steps = report['steps']
    require(isinstance(steps, list) and type(report['complete']) is bool and
            all(isinstance(s['name'], str) and type(s['exit']) is int for s in steps), 'invalid build steps')
    failed = [s['name'] for s in steps if s['exit'] != 0]
    result = {'receiptSha256': digest(path), 'steps': [{'name': s['name'], 'exit': s['exit']} for s in steps]}
    result['exchangeScope'] = 'reliable-Int32-topic-roundtrip'
    result['rosGraphInteroperability'] = 'unverified'
    result['diagnostics'] = {role: peer_diagnostics(path.parent, report.get('outputs', {}), name)
                             for role, name in [('requestPeer', 'ros.log'), ('replyPeer', 'cyclone.log')]}
    if not report['complete']:
        result['status'] = 'failed' if failed else 'incomplete'
        return result
    require(not failed, 'nonzero step in a completed run')
    client, peer = report['request_peer'], report['reply_peer']
    raw = peer['api'] == 'DDS C'
    require((client['language'], client['api'], client['rmw']) == ('C++', 'rclcpp', 'rmw_fastrtps_cpp') and
            (peer['language'], peer['api'], peer['rmw']) ==
            (('C', 'DDS C', None) if raw else ('C++', 'rclcpp', 'rmw_cyclonedds_cpp')), 'invalid peer API binding')
    require((report['transport'], report['ros_type'], report['network_security']) ==
            ('UDPv4', 'std_msgs/msg/Int32', 'plaintext'), 'invalid transport profile')
    ros, cyclone, peer_prefix = (Path(p) for p in (report['ros_prefix'], report['cyclone_prefix'], peer['prefix']))
    require(Path(client['prefix']) == ros and (not raw or peer_prefix == cyclone), 'mismatched peer prefix')
    for binding in (client, peer):
        prefix = Path(binding['prefix'])
        require(prefix.is_absolute() and re.fullmatch('[a-z][a-z0-9_]*', binding['distribution']) and
                prefix.name == binding['distribution'], 'mismatched peer distribution')
    expected = ['idlc', 'build-cyclone', 'build-ros-client', *([] if raw else ['build-ros-echo']), 'isolated-network']
    require([s['name'] for s in steps] == expected, 'missing or unexpected build/run step')
    args = steps[-1]['argv']
    require('--unshare-net' in args and '--run-peers' in args and args[args.index('--ros-prefix')+1] == str(ros) and
            args[args.index('--cyclone-prefix')+1] == str(cyclone), 'missing network or prefix binding')
    require(('--peer-ros-prefix' not in args) if raw else ('--peer-ros-prefix' in args and
            args[args.index('--peer-ros-prefix')+1] == str(peer_prefix)), 'mismatched echo process')
    require(all('-O3' in s['argv'] and '-g' in s['argv'] for s in steps if s['name'].startswith('build-')),
            'unrecognized native build flags')

    inputs = {Path(p): checksum for p, checksum in report['inputs'].items()}
    runners = [p for p in inputs if p.name == 'run.py' and p.parent.name == 'dds_interop']
    require(len(runners) == 1 and str(runners[0]) in args, 'missing source binding')
    source = runners[0].parent
    libraries = [p for p in inputs if p.is_relative_to(cyclone/'lib') and p.name == 'libddsc.so']
    fast = {ros/'lib'/n for n in ('libfastdds.so', 'libfastrtps.so')} & inputs.keys()
    require(len(libraries) == 1 and fast, 'missing DDS vendor libraries')
    required = {source / n for n in ('run.py', 'cyclone.c', 'ros_client.cpp', 'Int32.idl', 'fastdds-loopback.xml')}
    required.update([cyclone/'bin/idlc', *libraries, *fast, ros/'lib/librclcpp.so',
                     ros/'lib/librmw_fastrtps_cpp.so', ros/'share/std_msgs/msg/Int32.idl'])
    if not raw:
        required.update([peer_prefix/'lib/librclcpp.so', peer_prefix/'lib/librmw_cyclonedds_cpp.so',
                         peer_prefix/'share/std_msgs/msg/Int32.idl'])
    require(required <= inputs.keys(), 'incomplete ROS/IDL/compiler inputs')
    for original in required:
        require(original.is_absolute() and '..' not in original.parts, 'invalid input location')
        actual = original
        for old, new in sorted(remaps, key=lambda pair: -len(pair[0].parts)):
            if original.is_relative_to(old):
                actual = new / original.relative_to(old)
                break
        require(digest(actual) == inputs[original], f'input identity differs for {original.name}')

    outputs = ['ros-client', 'cyclone-echo', 'Int32.c', 'Int32.h', 'ros.log', 'cyclone.log', 'exchange.json']
    if not raw:
        outputs.append('ros-echo')
    for name in outputs:
        require(report['outputs'].get(name) == digest(path.parent/name), f'output identity differs for {name}')
    exchange = json.loads((path.parent/'exchange.json').read_bytes())
    require(exchange['domain'] == 61 and exchange['network'] == 'isolated loopback' and
            type(exchange['challenge']) is int and 1 <= exchange['challenge'] <= 2**29 and
            exchange['expected_response'] == exchange['challenge'] + 1 and
            exchange['ros_exit'] == 0 and exchange['cyclone_exit'] == 0, 'failed or inconsistent reply')
    client_status = 'rclcpp rmw_fastrtps_cpp request completed'
    peer_status = 'Cyclone DDS received challenge and sent reply' if raw else 'rclcpp rmw_cyclonedds_cpp echo completed'
    for name, status in [('ros.log', client_status), ('cyclone.log', peer_status)]:
        require(status in (path.parent/name).read_text().splitlines(), 'peer completion missing')
    result.update(status='passed', rosType=report['ros_type'], network=exchange['network'],
                  networkSecurity=report['network_security'], transport=report['transport'],
                  requestPeer={k: client[k] for k in ('api', 'language', 'rmw', 'distribution')},
                  replyPeer={k: peer[k] for k in ('api', 'language', 'rmw', 'distribution')},
                  exchangeSha256=report['outputs']['exchange.json'], challenge=exchange['challenge'],
                  response=exchange['expected_response'])
    return result


def verify_service(path, remaps=()):
    """Verify a correlated GetParameters exchange or retain a failed build/run observation."""
    report = json.loads(path.read_bytes())
    require((report['scope'], report['ros_type'], report['transport'], report['network_security']) ==
            ('Native GetParameters service on isolated loopback. Plaintext.', 'rcl_interfaces/srv/GetParameters',
             'UDPv4', 'plaintext'), 'unrecognized service fixture')
    steps = report['steps']
    expected = ['build-ros-client', 'build-ros-echo', 'isolated-network']
    require(type(report['complete']) is bool and [s['name'] for s in steps] == expected[:len(steps)] and
            len(steps) <= len(expected) and all(type(s['exit']) is int for s in steps), 'invalid service steps')
    failed = any(s['exit'] != 0 for s in steps)
    result = dict(receiptSha256=digest(path), exchangeScope='correlated-GetParameters-service',
                  networkSecurity='plaintext', control=report.get('control'),
                  steps=[{'name':s['name'], 'exit':s['exit']} for s in steps])
    if not report['complete']:
        result['status'] = 'failed' if failed else 'incomplete'
        return result
    require(len(steps) == 3 and not failed, 'service run did not complete')
    args = steps[-1]['argv']
    require('--unshare-net' in args and '--service' in args and '--run-peers' in args,
            'missing service/network selection')
    def opt(key, default=None):
        return args[args.index(key)+1] if key in args else default
    control = opt('--service-control')
    require(control == result['control'] and control in (None, 'request', 'reply'), 'bad control binding')
    left, right = (Path(opt(k)) for k in ('--ros-prefix', '--peer-ros-prefix'))
    server_rmw = opt('--service-peer-rmw', 'rmw_cyclonedds_cpp')
    require(server_rmw in ('rmw_fastrtps_cpp','rmw_cyclonedds_cpp'), 'unexpected service middleware')
    settings = [(left, 'rmw_fastrtps_cpp'), (right, server_rmw)]
    if control:
        settings = [settings[0 if control == 'request' else 1]] * 2
    inputs = {Path(p): h for p,h in report['inputs'].items()}
    source, = [p.parent for p in inputs if p.name == 'run.py' and p.parent.name == 'dds_interop']
    required = {source/'run.py', source/'ros_service.cpp', source/'fastdds-loopback.xml'}
    require(str(source/'run.py') in args and all(str(source/'ros_service.cpp') in step['argv'] and
            '-O3' in step['argv'] and '-g' in step['argv'] for step in steps[:2]), 'unbound service build')
    for key, (prefix, rmw) in zip(('request_peer','reply_peer'),settings):
        peer = report[key]
        require(prefix.is_absolute() and re.fullmatch('[a-z][a-z0-9_]*',prefix.name) and
                (peer['language'],peer['api'],peer['prefix'],peer['distribution'],peer['rmw']) ==
                ('C++','rclcpp',str(prefix),prefix.name,rmw), 'wrong service peer')
        required.update((prefix/'share/rcl_interfaces/srv/GetParameters.idl', prefix/'lib/librclcpp.so',
                         prefix/'lib/librcl_interfaces__rosidl_typesupport_cpp.so',prefix/('lib/lib'+rmw+'.so')))
        names = {'libfastdds.so','libfastrtps.so'} if rmw == 'rmw_fastrtps_cpp' else {'libddsc.so'}
        vendor = [p for p in inputs if p.name in names and p.is_relative_to(prefix/'lib')]
        require(vendor,'missing service vendor library')
        required.update(vendor)
    require(required <= inputs.keys(),'incomplete service inputs')
    for p in required:
        require('..' not in p.parts and p.is_absolute(),'invalid service input')
        actual = p
        for old,new in sorted(remaps,key=lambda pair: -len(pair[0].parts)):
            if p.is_relative_to(old):
                actual = new/p.relative_to(old)
                break
        require(digest(actual) == inputs[p],f'service input identity differs for {p.name}')
    for name in ('ros-client','ros-echo','ros.log','cyclone.log','exchange.json'):
        require(report['outputs'].get(name)==digest(path.parent/name),f'service output differs for {name}')
    exchange = json.loads((path.parent/'exchange.json').read_bytes())
    token = exchange['challenge']
    require(type(token) is int and 1<=token<=2**29 and exchange['domain']==61 and
            exchange['network']=='isolated loopback' and exchange['ros_exit']==exchange['cyclone_exit']==0,
            'invalid service exchange')
    names = [f'{token}-a', f'{token}-b']
    require(exchange['request_names']==names and exchange['response_values']==['reply/'+n for n in names],
            'service request/reply correlation differs')
    for log,role,(_,rmw) in zip(('ros.log','cyclone.log'),('request','echo'),settings):
        line = f'GetParameters {rmw} {role} {names[0]} {names[1]} completed'
        require((path.parent/log).read_text().splitlines().count(line)==1,'missing service witness')
    for role, field in [('requestPeer','request_peer'),('replyPeer','reply_peer')]:
        result[role] = {k:report[field][k] for k in ('distribution','api','language','rmw')}
    return {**result,'status':'passed','rosType':report['ros_type'],'responseValues':exchange['response_values']}
