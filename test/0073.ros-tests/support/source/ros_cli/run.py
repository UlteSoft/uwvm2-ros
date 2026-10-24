#!/usr/bin/env python3
"""Exercise actual UWVM CLI registration and static policy validation."""
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile

p = argparse.ArgumentParser()
p.add_argument('--uwvm', type=Path, required=True)
p.add_argument('--output', type=Path, required=True)
p.add_argument('--wat2wasm', default='wat2wasm')
args = p.parse_args()
rows = []
wasi_guest_inputs = {}

def wasi_guest_binding(source, artifact):
    """Bind small generated witnesses before each actual Worker invocation.

    WAT text is retained in compile rows. TemporaryDirectory may remove the
    original paths later; exact source and binary hashes remain in the report.
    A changed path or binary fails before the Worker receives that fixture.
    """
    if source.stat().st_size > 1048576 or artifact.stat().st_size > 1048576:
        raise ValueError('generated WASI witness exceeded its1MiB bound')
    text, data = source.read_bytes(), artifact.read_bytes()
    return {'source_path':str(source), 'artifact_path':str(artifact),
            'source_sha256':hashlib.sha256(text).hexdigest(),
            'artifact_sha256':hashlib.sha256(data).hexdigest(),
            'source_bytes':len(text), 'artifact_bytes':len(data)}

def run(name, options, success, fragment=None, expected_returncode=None):
    binding = None
    if '--run' in options:
        key = str(Path(options[options.index('--run')+1]).resolve())
        if key in wasi_guest_inputs:
            expected = wasi_guest_inputs[key]
            try:
                binding = wasi_guest_binding(Path(expected['source_path']), Path(expected['artifact_path']))
                if binding != expected:
                    raise ValueError('generated WASI witness changed before Worker execution')
            except (OSError, ValueError) as error:
                rows.append({'name':name, 'status':'FAIL', 'command':options,
                             'reason':str(error), 'scope':'Worker was not launched'})
                return
    r = subprocess.run([str(args.uwvm.resolve()), *options], text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=15)
    passed = ((r.returncode == 0) == success
              and (fragment is None or fragment in r.stdout + r.stderr)
              and (expected_returncode is None or r.returncode == expected_returncode))
    record = {'name': name, 'status': 'PASS' if passed else 'FAIL', 'command': options, 'returncode': r.returncode,
              'stdout': r.stdout, 'stderr': r.stderr}
    if expected_returncode is not None:
        record['expected_returncode'] = expected_returncode
    if binding:
        record['generated_fixture_before'] = binding
        try:
            after = wasi_guest_binding(Path(binding['source_path']), Path(binding['artifact_path']))
            record['generated_fixture_after'] = after
            if after != binding:
                record.update(status='FAIL', reason='generated WASI witness changed during Worker execution')
        except (OSError, ValueError) as error:
            record.update(status='FAIL', reason=str(error))
    rows.append(record)

for v in (1, 2):
    prefix = f'--ros{v}'
    run(f'help-ros{v}', ['--help', f'ros{v}'], True, prefix + '-group-grant')
    run(f'empty-policy-ros{v}', [prefix + '-check-policy'], True)
    run(f'default-deny-dump-ros{v}', [prefix + '-dump-effective-policy'], True, '"enabled":false')
    run(f'missing-connections-ros{v}', [prefix + '-global-enable', prefix + '-check-policy'], False, 'missing explicit')
    run(f'set-before-create-ros{v}', [prefix + '-single-enable', 'sensor'], False)
    run(f'duplicate-target-ros{v}', [prefix + '-group-create', 's', prefix + '-group-create', 's'], False)
    run(f'duplicate-binding-ros{v}', [prefix + '-group-create', 's', prefix + '-group-add-module', 's', 'sensor',
                                      prefix + '-single-create', 'sensor'], False)
    run(f'unknown-bound-module-ros{v}', [prefix + '-group-create', 's', prefix + '-group-add-module', 's', 'missing', prefix + '-check-policy'], False)
    run(f'duplicate-field-ros{v}', [prefix + '-global-gateway-uid', '0', prefix + '-global-gateway-uid', '0'], False)
    run(f'no-level-option-ros{v}', [prefix + '-level', '4'], False)
    run(f'relative-socket-ros{v}', [prefix + '-global-gateway-socket', 'relative.sock'], False)
    run(f'uid-overflow-ros{v}', [prefix + '-global-gateway-uid', '18446744073709551616'], False)
run('independent-group-namespaces', ['--ros1-group-create', 'same', '--ros2-group-create', 'same', '--ros1-check-policy', '--ros2-check-policy'], True)
run('missing-grant-fields', ['--ros1-global-grant', 'cap', 'topic.publish', '/x'], False)
run('relative-resource', ['--ros1-global-grant', 'cap', 'topic.publish', 'x', 'std_msgs/String', '992ce8a1687cec8c8bd883ec73ca41d1', '--ros1-check-policy'], False)
run('missing-explicit-limits', ['--ros1-global-grant', 'cap', 'topic.publish', '/x', 'std_msgs/String', '992ce8a1687cec8c8bd883ec73ca41d1', '--ros1-check-policy'], False, 'missing explicit field')
# Real CLI validation of every new mandatory DDS-service QoS field.
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'ros'))
import run_network as network
import run_rpc as rpc

def policy_options(grants,version=2,enabled=False,scope='global'):
    options=[]
    prefix=f'--ros{version}-{scope}-'
    target=[] if scope=='global' else ['cli_signed']
    if scope!='global':
        options += ['--wasm-set-main-module-name','cli_signed',prefix+'create',*target]
        if scope=='group':options += [prefix+'add-module',*target,'cli_signed']
    if enabled:
        # A complete explicit TEST profile exercises unsupported adapters while
        # check-policy exits before any IPC handshake or ROS network activity.
        connection={'gateway-socket':'/tmp/uwvm-ros-cli-unused.sock','gateway-uid':str(os.getuid()),
                    'node-name':'uwvm_cli_scalar','namespace':'/cli','connect-timeout-ms':'1000',
                    'audit-deny-max-rate-hz':'100','audit-deny-burst':'10',
                    'audit-deny-max-records':'100','audit-deny-max-bytes':'65536'}
        connection.update({'master-uri':'http://127.0.0.1:11339','advertise-host':'127.0.0.1','transport':'tcpros'} if version==1 else
                          {'domain-id':'73','rmw-implementation':'rmw_fastrtps_cpp','security-mode':'disabled-test',
                           'keystore':'not-applicable','enclave':'not-applicable'})
        options += [prefix+'enable',*target]
        for key,value in connection.items():options += [prefix+key,*target,value]
    for cap in grants:
        options += [prefix+'grant',*target,*[cap[k] for k in ('alias','operation','resource','type','identity')]]
        for key,value in cap['limits'].items():options += [prefix+'set',*target,cap['alias'],key,value]
    return [*options,f'--ros{version}-check-policy']

def scalar_policy_options(grants,version=2,scope='global'):
    return policy_options(grants,version,enabled=True,scope=scope)

service_grants=network.policy(2,['service-client-trigger','service-trigger'])[1]
dds_grants=service_grants+[rpc.grant(2,op) for op in
    ('param.get','param.set','param.describe','param.list','lifecycle.get-state','lifecycle.change-state')]
for cap in dds_grants:
    run('qos-explicit-'+cap['operation'],policy_options([cap]),True)
    for field in network.service_qos():
        missing={**cap,'limits':{key:value for key,value in cap['limits'].items() if key!=field}}
        run('qos-missing-'+cap['operation']+'-'+field,policy_options([missing]),False,'missing explicit field: '+field)
server,response=service_grants[1:]
run('qos-service-response-contract-equal',policy_options([server,response]),True)
mismatch={**response,'limits':{**response['limits'],'qos.depth':'11'}}
run('qos-service-response-contract-mismatch',policy_options([server,mismatch]),False,'service response QoS differs')
for op in ('param.declare','param.undeclare'):
    cap=rpc.grant(2,op);cap['limits'].update(network.service_qos())
    run('qos-local-'+op+'-rejects-inapplicable-fields',policy_options([cap]),False,'unknown/inapplicable capability field')

# Conditional scalar contracts are checked by the actual CLI, before any Guest
# or ROS endpoint can be created. Scalar adapter names do not imply event grants.
scalar_start=len(rows)
contract_error='invalid explicit parameter type, bounds or byte quota'
adapter_error='operation/type has no implemented trusted backend adapter'
for version in (1,2):
    for kind in rpc.scalar.KINDS[version]:
        operations=('param.get','param.set') if version==1 else ('param.get','param.set','param.declare','param.undeclare')
        for operation in operations:
            cap=rpc.scalar_grant(version,operation,kind,local=operation in ('param.declare','param.undeclare'))
            run(f'scalar-ros{version}-{kind}-{operation}-explicit',scalar_policy_options([cap],version),True)
        cap=rpc.scalar_grant(version,'param.get',kind)
        narrow=copy.deepcopy(cap)
        narrow['limits']['max-parameter-bytes']=str(rpc.scalar.VALUES[kind]['width']-1)
        run(f'scalar-ros{version}-{kind}-quota-below-width',scalar_policy_options([narrow],version),False,
            'positive integer required: max-parameter-bytes' if kind=='bool' else contract_error)
        identity=copy.deepcopy(cap);identity['identity']='builtin:param-string-v1'
        run(f'scalar-ros{version}-{kind}-wrong-identity',scalar_policy_options([identity],version),False,'type identity differs from trusted adapter')
        if kind=='bool':
            nonnumeric=copy.deepcopy(cap)
            nonnumeric['limits'].update({'parameter-min-value':'0','parameter-max-value':'1'})
            run(f'scalar-ros{version}-bool-rejects-numeric-bounds',scalar_policy_options([nonnumeric],version),False,contract_error)
            numeric_validator=copy.deepcopy(cap);numeric_validator['limits']['validator']='finite'
            run(f'scalar-ros{version}-bool-rejects-numeric-validator',scalar_policy_options([numeric_validator],version),False,'numeric validator only applies')
        else:
            for key in ('parameter-min-value','parameter-max-value'):
                missing=copy.deepcopy(cap);del missing['limits'][key]
                run(f'scalar-ros{version}-{kind}-missing-{key}',scalar_policy_options([missing],version),False,contract_error)
            inverted=copy.deepcopy(cap)
            inverted['limits'].update({'parameter-min-value':'2','parameter-max-value':'1'})
            run(f'scalar-ros{version}-{kind}-inverted-bounds',scalar_policy_options([inverted],version),False,contract_error)
            if kind.startswith('int'):
                for name,value in (('fractional','-1.5'),('leading-plus','+1'),('exponent','1e2')):
                    malformed=copy.deepcopy(cap);malformed['limits']['parameter-min-value']=value
                    run(f'scalar-ros{version}-{kind}-{name}-bound',scalar_policy_options([malformed],version),False,contract_error)
                width=rpc.scalar.VALUES[kind]['width']*8
                for key,value in (('parameter-min-value',str(-(1<<(width-1))-1)),('parameter-max-value',str(1<<(width-1)))):
                    overflow=copy.deepcopy(cap);overflow['limits'][key]=value
                    run(f'scalar-ros{version}-{kind}-overflow-{key}',scalar_policy_options([overflow],version),False,contract_error)
                extrema=copy.deepcopy(cap)
                extrema['limits'].update({'parameter-min-value':str(-(1<<(width-1))),'parameter-max-value':str((1<<(width-1))-1)})
                run(f'scalar-ros{version}-{kind}-exact-signed-extrema',scalar_policy_options([extrema],version),True)
            else:
                for name,key,value in (('nan','parameter-min-value','NaN'),('positive-infinity','parameter-max-value','inf'),
                                       ('negative-infinity','parameter-min-value','-inf'),('overflow','parameter-max-value','1e309')):
                    nonfinite=copy.deepcopy(cap);nonfinite['limits'][key]=value
                    run(f'scalar-ros{version}-float64-{name}-bound',scalar_policy_options([nonfinite],version),False,contract_error)
                exponent=copy.deepcopy(cap)
                exponent['limits'].update({'parameter-min-value':'-2e0','parameter-max-value':'2.5E+0'})
                run(f'scalar-ros{version}-float64-finite-exponent-bounds',scalar_policy_options([exponent],version),True)
    cap=rpc.grant(version,'param.get')
    cap['limits'].update({'parameter-min-value':'0','parameter-max-value':'1'})
    run(f'scalar-ros{version}-string-rejects-numeric-bounds',scalar_policy_options([cap],version),False,contract_error)
    cap=rpc.grant(version,'param.get');cap['limits']['max-parameter-bytes']='3'
    run(f'scalar-ros{version}-string-quota-below-envelope',scalar_policy_options([cap],version),False,contract_error)
    unsupported='int64' if version==1 else 'int32'
    cap=rpc.scalar_grant(version,'param.get',unsupported)
    run(f'scalar-ros{version}-unsupported-{unsupported}-width',scalar_policy_options([cap],version),False,adapter_error)
    kind='int32' if version==1 else 'int64'
    metadata=('param.delete','param.exists','param.search','param.list') if version==1 else ('param.describe','param.list')
    for operation in metadata:
        cap=rpc.scalar_grant(version,operation,kind)
        run(f'scalar-ros{version}-{operation}-rejects-typed-metadata',scalar_policy_options([cap],version),False,adapter_error)
    cap=rpc.grant(version,'param.list')
    cap['limits'].update({'parameter-min-value':'0','parameter-max-value':'1'})
    run(f'scalar-ros{version}-metadata-rejects-numeric-bounds',scalar_policy_options([cap],version),False,contract_error)
for kind in rpc.scalar.KINDS[2]:
    cap=rpc.scalar_grant(2,'param.subscribe-events',kind)
    run(f'scalar-ros2-events-rejects-{kind}',scalar_policy_options([cap]),False,adapter_error)
scalar_count=len(rows)-scalar_start
typed_event_start=len(rows)
for kind in rpc.typed_events.KINDS:
    cap=rpc.typed_events.grant(kind)
    run(f'typed-event-{kind}-explicit',scalar_policy_options([cap]),True)
    tiny=copy.deepcopy(cap);tiny['limits']['max-parameter-bytes']='1'
    run(f'typed-event-{kind}-tiny-envelope-quota-explicit',scalar_policy_options([tiny]),True)
    zero=copy.deepcopy(cap);zero['limits']['max-parameter-bytes']='0'
    run(f'typed-event-{kind}-zero-envelope-quota',scalar_policy_options([zero]),False,'positive integer required: max-parameter-bytes')
    identity=copy.deepcopy(cap);identity['identity']='builtin:param-string-v1'
    run(f'typed-event-{kind}-wrong-identity',scalar_policy_options([identity]),False,'type identity differs from trusted adapter')
    run(f'typed-event-{kind}-ros1-operation-rejected',scalar_policy_options([cap],1),False,'invalid target, alias, resource or operation')
    get=rpc.scalar_grant(2,'param.get',kind)
    get.update(type=cap['type'],identity=cap['identity'])
    run(f'typed-event-{kind}-get-operation-rejected',scalar_policy_options([get]),False,adapter_error)
    for field in network.service_qos():
        missing=copy.deepcopy(cap);del missing['limits'][field]
        run(f'typed-event-{kind}-missing-{field}',scalar_policy_options([missing]),False,'missing explicit field: '+field)
    for field in ('max-subscribers','max-event-fields','max-event-queue-bytes','parameter-node','parameter-name'):
        missing=copy.deepcopy(cap);del missing['limits'][field]
        run(f'typed-event-{kind}-missing-{field}',scalar_policy_options([missing]),False,'missing explicit field: '+field)
    unsigned=copy.deepcopy(cap);unsigned['limits']['max-parameter-bytes']='-1'
    run(f'typed-event-{kind}-unsigned-quota-negative',scalar_policy_options([unsigned]),False,'invalid parameter:')
    if kind=='bool':
        for bounds in ({'parameter-min-value':'0'}, {'parameter-min-value':'0','parameter-max-value':'1'}):
            bounded=copy.deepcopy(cap);bounded['limits'].update(bounds)
            run('typed-event-bool-rejects-'+('one-bound' if len(bounds)==1 else 'both-bounds'),scalar_policy_options([bounded]),False,contract_error)
        numeric=copy.deepcopy(cap);numeric['limits']['validator']='finite'
        run('typed-event-bool-rejects-finite-validator',scalar_policy_options([numeric]),False,'numeric validator only applies')
    else:
        for field in ('parameter-min-value','parameter-max-value'):
            missing=copy.deepcopy(cap);del missing['limits'][field]
            run(f'typed-event-{kind}-missing-{field}',scalar_policy_options([missing]),False,contract_error)
        inverted=copy.deepcopy(cap);inverted['limits'].update({'parameter-min-value':'2','parameter-max-value':'1'})
        run(f'typed-event-{kind}-inverted-bounds',scalar_policy_options([inverted]),False,contract_error)
        if kind=='int64':
            for label,value in (('fractional','-1.5'),('leading-plus','+1'),('exponent','1e2')):
                malformed=copy.deepcopy(cap);malformed['limits']['parameter-min-value']=value
                run(f'typed-event-int64-{label}-bound',scalar_policy_options([malformed]),False,contract_error)
            for field,value in (('parameter-min-value','-9223372036854775809'),('parameter-max-value','9223372036854775808')):
                overflow=copy.deepcopy(cap);overflow['limits'][field]=value
                run('typed-event-int64-overflow-'+field,scalar_policy_options([overflow]),False,contract_error)
            run('typed-event-int64-exact-signed-extrema',scalar_policy_options([cap]),True)
        else:
            for label,field,value in (('nan','parameter-min-value','NaN'),('positive-infinity','parameter-max-value','inf'),
                                      ('negative-infinity','parameter-min-value','-inf'),('overflow','parameter-max-value','1e309')):
                nonfinite=copy.deepcopy(cap);nonfinite['limits'][field]=value
                run(f'typed-event-float64-{label}-bound',scalar_policy_options([nonfinite]),False,contract_error)
            exponent=copy.deepcopy(cap);exponent['limits'].update({'parameter-min-value':'-2e0','parameter-max-value':'2.5E+0'})
            run('typed-event-float64-finite-exponent-bounds',scalar_policy_options([exponent]),True)
for unsupported in ('int32',):
    cap=rpc.typed_events.grant('bool');cap.update(type='builtin/param-event-'+unsupported+'-v1',identity='builtin:param-event-'+unsupported+'-v1')
    run('typed-event-unsupported-'+unsupported,scalar_policy_options([cap]),False,adapter_error)
# The six new event projections retain explicit array and numeric limits.
# Their legacy unsupported assertions are now missing-limit rejection cases.
for kind in ('bool-array','int64-array','float64-array'):
    cap=rpc.typed_events.grant('bool');cap.update(type='builtin/param-event-'+kind+'-v1',identity='builtin:param-event-'+kind+'-v1')
    run('typed-event-'+kind+'-missing-array-event-limits',scalar_policy_options([cap]),False,contract_error)
expanded_start=len(rows)
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'ros_parameter_events'))
import events as expanded_events
for kind in expanded_events.KINDS:
    cap=expanded_events.grant(kind)
    for scope in ('global','single','group'):
        run(f'expanded-event-{kind}-{scope}-explicit',scalar_policy_options([cap],scope=scope),True)
    tiny=copy.deepcopy(cap);tiny['limits']['max-parameter-bytes']='1'
    run(f'expanded-event-{kind}-tiny-complete-envelope-quota',scalar_policy_options([tiny]),True)
    identity=copy.deepcopy(cap);identity['identity']='builtin:param-string-v1'
    run(f'expanded-event-{kind}-wrong-identity',scalar_policy_options([identity]),False,'type identity differs from trusted adapter')
    run(f'expanded-event-{kind}-ros1-rejected',scalar_policy_options([cap],1),False,'invalid target, alias, resource or operation')
    ordinary=rpc.scalar_grant(2,'param.get','bool');ordinary.update(type=cap['type'],identity=cap['identity'])
    run(f'expanded-event-{kind}-get-adapter-rejected',scalar_policy_options([ordinary]),False,adapter_error)
    for key in (*network.service_qos(),'max-subscribers','max-event-fields','max-event-queue-depth','max-event-queue-bytes','parameter-node','parameter-name'):
        missing=copy.deepcopy(cap);del missing['limits'][key]
        run(f'expanded-event-{kind}-missing-{key}',scalar_policy_options([missing]),False,'missing explicit field: '+key)
    queue=copy.deepcopy(cap);queue['limits']['max-event-queue-bytes']='4095'
    run(f'expanded-event-{kind}-queue-cannot-fit-one-maximum-event',scalar_policy_options([queue]),False)
    if kind!='string':
        missing=copy.deepcopy(cap);del missing['limits']['max-parameter-elements']
        run(f'expanded-event-{kind}-missing-explicit-elements',scalar_policy_options([missing]),False,contract_error)
        for label,count,diagnostic in (('zero','0','positive integer required: max-parameter-elements'),('negative','-1','invalid parameter:'),('fractional','1.5','positive integer required: max-parameter-elements'),('too-many','1048577','field exceeds protocol safety ceiling: max-parameter-elements')):
            bad=copy.deepcopy(cap);bad['limits']['max-parameter-elements']=count
            run(f'expanded-event-{kind}-{label}-elements',scalar_policy_options([bad]),False,diagnostic)
        one=copy.deepcopy(cap);one['limits']['max-parameter-elements']='1'
        run(f'expanded-event-{kind}-one-explicit-element',scalar_policy_options([one]),True)
    else:
        bad=copy.deepcopy(cap);bad['limits']['max-parameter-elements']='1'
        run('expanded-event-string-rejects-inapplicable-elements',scalar_policy_options([bad]),False,contract_error)
    if kind in ('int64-array','float64-array'):
        for key in ('parameter-min-value','parameter-max-value'):
            bad=copy.deepcopy(cap);del bad['limits'][key]
            run(f'expanded-event-{kind}-missing-{key}',scalar_policy_options([bad]),False,contract_error)
        bad=copy.deepcopy(cap);bad['limits'].update({'parameter-min-value':'2','parameter-max-value':'1'})
        run(f'expanded-event-{kind}-inverted-bounds',scalar_policy_options([bad]),False,contract_error)
        if kind=='int64-array':
            exact=copy.deepcopy(cap);exact['limits'].update({'parameter-min-value':'9007199254740993','parameter-max-value':'9007199254740993'})
            run('expanded-event-int64-array-exact-beyond-2pow53',scalar_policy_options([exact]),True)
            for label,value in (('fractional','-1.5'),('plus','+1'),('overflow','9223372036854775808')):
                bad=copy.deepcopy(cap);bad['limits']['parameter-max-value']=value
                run(f'expanded-event-int64-array-{label}-bound',scalar_policy_options([bad]),False,contract_error)
        else:
            for label,value in (('nan','NaN'),('infinity','inf'),('overflow','1e309')):
                bad=copy.deepcopy(cap);bad['limits']['parameter-max-value']=value
                run(f'expanded-event-float64-array-{label}-bound',scalar_policy_options([bad]),False,contract_error)
            no_validator=copy.deepcopy(cap);no_validator['limits']['validator']='none'
            run('expanded-event-float64-array-finite-with-validator-none',scalar_policy_options([no_validator]),True)
    else:
        bad=copy.deepcopy(cap);bad['limits'].update({'parameter-min-value':'0','parameter-max-value':'1'})
        run(f'expanded-event-{kind}-rejects-numeric-bounds',scalar_policy_options([bad]),False,contract_error)
        bad=copy.deepcopy(cap);bad['limits']['validator']='finite'
        run(f'expanded-event-{kind}-rejects-inapplicable-finite-validator',scalar_policy_options([bad]),False,'numeric validator only applies')
expanded_count=len(rows)-expanded_start
legacy=rpc.grant(2,'param.subscribe-events');legacy['limits'].update({'parameter-min-value':'0','parameter-max-value':'1'})
run('typed-event-legacy-string-rejects-numeric-bounds',scalar_policy_options([legacy]),False,contract_error)
typed_event_count=len(rows)-typed_event_start
# New closed array adapters use the existing set entrypoints. Counts are a
# conditional quota; the four-byte empty envelope remains a valid value.
def array_grant(version,operation,kind):
    cap=rpc.grant(version,operation)
    cap.update(type='builtin/'+kind+'-array-v1',identity='builtin:param-'+kind+'-array-v1')
    cap['limits'].update({'max-parameter-elements':'8','max-parameter-bytes':'512',
                          'validator':'finite' if kind=='float64' else 'none'})
    bounds={'int32':('-2147483648','2147483647'),
            'int64':('-9223372036854775808','9223372036854775807'),
            'float64':('-1e308','1e308')}.get(kind)
    if bounds:
        cap['limits'].update({'parameter-min-value':bounds[0],'parameter-max-value':bounds[1]})
    return cap

array_start=len(rows)
array_kinds={1:('bool','int32','float64','string'),2:('bool','int64','float64','string','byte')}
for version,kinds in array_kinds.items():
    for kind in kinds:
        operations=('param.get','param.set') if version==1 else ('param.get','param.set','param.declare','param.undeclare')
        for operation in operations:
            cap=array_grant(version,operation,kind)
            run(f'array-ros{version}-{kind}-{operation}-explicit',scalar_policy_options([cap],version),True)
        cap=array_grant(version,'param.get',kind)
        missing=copy.deepcopy(cap);del missing['limits']['max-parameter-elements']
        run(f'array-ros{version}-{kind}-missing-elements',scalar_policy_options([missing],version),False,contract_error)
        for label,value,fragment in (
                ('zero','0','positive integer required: max-parameter-elements'),
                ('negative','-1','invalid parameter:'),
                ('leading-plus','+1','positive integer required: max-parameter-elements'),
                ('fractional','1.5','positive integer required: max-parameter-elements'),
                ('over-ceiling','1048577','field exceeds protocol safety ceiling: max-parameter-elements'),
                ('u64-overflow','18446744073709551616','positive integer required: max-parameter-elements')):
            bad=copy.deepcopy(cap);bad['limits']['max-parameter-elements']=value
            run(f'array-ros{version}-{kind}-elements-{label}',scalar_policy_options([bad],version),False,fragment)
        empty=copy.deepcopy(cap);empty['limits'].update({'max-parameter-elements':'1048576','max-parameter-bytes':'4'})
        run(f'array-ros{version}-{kind}-empty-envelope-and-max-elements',scalar_policy_options([empty],version),True)
        small=copy.deepcopy(cap);small['limits']['max-parameter-bytes']='3'
        run(f'array-ros{version}-{kind}-quota-below-envelope',scalar_policy_options([small],version),False,contract_error)
        identity=copy.deepcopy(cap);identity['identity']='builtin:param-string-v1'
        run(f'array-ros{version}-{kind}-wrong-identity',scalar_policy_options([identity],version),False,'type identity differs from trusted adapter')
        if kind in ('bool','string','byte'):
            for label,bounds in (('one-bound',{'parameter-min-value':'0'}),('both-bounds',{'parameter-min-value':'0','parameter-max-value':'1'})):
                bad=copy.deepcopy(cap);bad['limits'].update(bounds)
                run(f'array-ros{version}-{kind}-rejects-{label}',scalar_policy_options([bad],version),False,contract_error)
            finite=copy.deepcopy(cap);finite['limits']['validator']='finite'
            run(f'array-ros{version}-{kind}-rejects-numeric-validator',scalar_policy_options([finite],version),False,'numeric validator only applies')
        else:
            for field in ('parameter-min-value','parameter-max-value'):
                missing=copy.deepcopy(cap);del missing['limits'][field]
                run(f'array-ros{version}-{kind}-missing-{field}',scalar_policy_options([missing],version),False,contract_error)
            inverted=copy.deepcopy(cap);inverted['limits'].update({'parameter-min-value':'2','parameter-max-value':'1'})
            run(f'array-ros{version}-{kind}-inverted-bounds',scalar_policy_options([inverted],version),False,contract_error)
            if kind.startswith('int'):
                for label,value in (('fractional','-1.5'),('leading-plus','+1'),('exponent','1e2')):
                    bad=copy.deepcopy(cap);bad['limits']['parameter-min-value']=value
                    run(f'array-ros{version}-{kind}-{label}-bound',scalar_policy_options([bad],version),False,contract_error)
                bits=32 if kind=='int32' else 64
                for field,value in (('parameter-min-value',str(-(1<<(bits-1))-1)),('parameter-max-value',str(1<<(bits-1)))):
                    bad=copy.deepcopy(cap);bad['limits'][field]=value
                    run(f'array-ros{version}-{kind}-overflow-{field}',scalar_policy_options([bad],version),False,contract_error)
                exact=copy.deepcopy(cap)
                if kind=='int64':exact['limits'].update({'parameter-min-value':'9007199254740993','parameter-max-value':'9007199254740993'})
                run(f'array-ros{version}-{kind}-exact-integer-bounds',scalar_policy_options([exact],version),True)
            else:
                for label,field,value in (('nan','parameter-min-value','NaN'),('inf','parameter-max-value','inf'),
                                          ('negative-inf','parameter-min-value','-inf'),('overflow','parameter-max-value','1e309')):
                    bad=copy.deepcopy(cap);bad['limits'][field]=value
                    run(f'array-ros{version}-float64-{label}-bound',scalar_policy_options([bad],version),False,contract_error)
                no_validator=copy.deepcopy(cap);no_validator['limits']['validator']='none'
                run(f'array-ros{version}-float64-finite-with-validator-none',scalar_policy_options([no_validator],version),True)
        metadata=('param.delete','param.exists','param.search','param.list') if version==1 else ('param.describe','param.list','param.subscribe-events')
        for operation in metadata:
            bad=array_grant(version,operation,kind)
            run(f'array-ros{version}-{kind}-rejects-{operation}',scalar_policy_options([bad],version),False,adapter_error)
    for kind in ('string','bool','float64','int32' if version==1 else 'int64'):
        cap=rpc.grant(version,'param.get') if kind=='string' else rpc.scalar_grant(version,'param.get',kind)
        cap['limits']['max-parameter-elements']='8'
        run(f'array-ros{version}-{kind}-scalar-rejects-elements',scalar_policy_options([cap],version),False,contract_error)
for version,kind in ((1,'int64'),(1,'byte'),(2,'int32')):
    cap=array_grant(version,'param.get',kind)
    run(f'array-ros{version}-{kind}-backend-rejected',scalar_policy_options([cap],version),False,adapter_error)
for kind in rpc.typed_events.KINDS:
    cap=rpc.typed_events.grant(kind);cap['limits']['max-parameter-elements']='8'
    run('array-elements-rejected-on-typed-event-'+kind,scalar_policy_options([cap]),False,contract_error)
legacy=rpc.grant(2,'param.subscribe-events');legacy['limits']['max-parameter-elements']='8'
run('array-elements-rejected-on-legacy-event',scalar_policy_options([legacy]),False,contract_error)
array_count=len(rows)-array_start
array_set_start=len(rows)
for version in (1,2):
    for scope in ('global','single','group'):
        prefix=f'--ros{version}-{scope}-';target=[] if scope=='global' else ['cli_signed']
        kind='int32' if version==1 else 'int64'
        cap=array_grant(version,'param.get',kind);cap['limits'].update({'parameter-min-value':'-20','parameter-max-value':'-1'})
        run(f'array-set-ros{version}-{scope}-explicit-negative-bounds',scalar_policy_options([cap],version,scope),True)
        missing=copy.deepcopy(cap);del missing['limits']['max-parameter-elements']
        run(f'array-set-ros{version}-{scope}-missing-count-field',scalar_policy_options([missing],version,scope),False,contract_error)
        for label,value,fragment in (('zero','0','positive integer required: max-parameter-elements'),
                                      ('negative','-1','invalid parameter:'),
                                      ('over-ceiling','1048577','field exceeds protocol safety ceiling: max-parameter-elements')):
            bad=copy.deepcopy(cap);bad['limits']['max-parameter-elements']=value
            run(f'array-set-ros{version}-{scope}-{label}',scalar_policy_options([bad],version,scope),False,fragment)
        base=scalar_policy_options([missing],version,scope)[:-1]
        option=[prefix+'set',*target,cap['alias'],'max-parameter-elements']
        run(f'array-set-ros{version}-{scope}-missing-value',[*base,*option],False,'missing explicit argument')
        run(f'array-set-ros{version}-{scope}-empty-value',[*base,*option,'',f'--ros{version}-check-policy'],False,'empty argument')
        run(f'array-set-ros{version}-{scope}-next-option-not-value',[*base,*option,f'--ros{version}-check-policy'],False,'missing explicit argument')
array_set_count=len(rows)-array_set_start
signed_set_start=len(rows)
for version in (1,2):
    for scope in ('global','single','group'):
        prefix=f'--ros{version}-{scope}-'
        target=[] if scope=='global' else ['cli_signed']
        kind='int32' if version==1 else 'int64'
        cap=rpc.scalar_grant(version,'param.get',kind)
        cap['limits'].update({'parameter-min-value':'-20','parameter-max-value':'-1'})
        run(f'signed-bound-ros{version}-{scope}-negative-integer',scalar_policy_options([cap],version,scope),True)
        cap=rpc.scalar_grant(version,'param.get','float64')
        cap['limits'].update({'parameter-min-value':'-2e0','parameter-max-value':'-1e-1'})
        run(f'signed-bound-ros{version}-{scope}-negative-float',scalar_policy_options([cap],version,scope),True)
        del cap['limits']['parameter-min-value']
        base=scalar_policy_options([cap],version,scope)[:-1]
        value_option=[prefix+'set',*target,cap['alias'],'parameter-min-value']
        run(f'signed-bound-ros{version}-{scope}-missing-value',[*base,*value_option],False,'missing explicit argument')
        run(f'signed-bound-ros{version}-{scope}-empty-value',[*base,*value_option,'',f'--ros{version}-check-policy'],False,'empty argument')
        run(f'signed-bound-ros{version}-{scope}-next-option-not-value',[*base,*value_option,f'--ros{version}-check-policy'],False,'missing explicit argument')
        run(f'signed-bound-ros{version}-{scope}-unknown-option-not-value',[*base,*value_option,'--ros-signed-bound-bogus'],False,'invalid parameter:')
        quota=rpc.scalar_grant(version,'param.get',kind);quota['limits']['max-parameter-bytes']='-1'
        run(f'signed-bound-ros{version}-{scope}-unsigned-quota-still-invalid',scalar_policy_options([quota],version,scope),False,'invalid parameter:')
signed_set_count=len(rows)-signed_set_start
with tempfile.TemporaryDirectory(prefix='uwvm-ros-cli-') as directory:
    directory = Path(directory)
    wat = directory/'benign.wat'
    wasm = directory/'benign.wasm'
    wat.write_text('(module (import "wasi_snapshot_preview1" "proc_exit" (func (param i32))) (memory (export "memory") 1) (func (export "_start")))\n')
    subprocess.run([args.wat2wasm, str(wat), '-o', str(wasm)], check=True, timeout=15)
    benign_binding = wasi_guest_binding(wat, wasm)
    wasi_guest_inputs[str(wasm.resolve())] = benign_binding
    rows.append({'name':'benign-wasi-fixture-binding', 'status':'PASS',
                 'scope':'Compiled fixture bytes only; no Worker execution',
                 'source_text':wat.read_text(), 'binding':benign_binding})
    for v in (1, 2):
        run(f'final-disabled-ros{v}-preserves-benign-wasi',
            [f'--ros{v}-global-enable', f'--ros{v}-global-disable', '--run', str(wasm)], True)
    # L01 recipes exercise the real parser and interpreter. This complete suite
    # is launched only by the qualified Linux64GiB acceptance driver; importing
    # the recipe module itself compiles and executes nothing.
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'wasip1_default_deny'))
    import cli_cases as wasi_cli
    compiled_wasi = {}
    for width in (32, 64):
        for name, text in wasi_cli.fixtures(width).items():
            source = directory/f'wasi{width}-{name}.wat'
            artifact = source.with_suffix('.wasm')
            source.write_text(text)
            command = [args.wat2wasm, '--enable-all', str(source), '-o', str(artifact)]
            try:
                result = subprocess.run(command, capture_output=True, text=True, timeout=15)
            except (OSError, subprocess.TimeoutExpired) as error:
                rows.append({'name':f'wasi{width}-{name}-compile', 'command':command,
                             'status':'BLOCKED', 'reason':str(error),
                             'scope':'No successful fixture compilation or execution'})
                continue
            record = {'name':f'wasi{width}-{name}-compile', 'command':command,
                         'status':'PASS' if result.returncode == 0 else 'FAIL',
                         'returncode':result.returncode, 'stdout':result.stdout,
                         'stderr':result.stderr, 'scope':'Fixture compilation only',
                         'source_text':text, 'source_sha256':hashlib.sha256(text.encode()).hexdigest()}
            if result.returncode == 0:
                try:
                    binding = wasi_guest_binding(source, artifact)
                    if binding['source_sha256'] != record['source_sha256']:
                        raise ValueError('generated WAT changed during compilation')
                    record['binding'] = binding
                    wasi_guest_inputs[str(artifact.resolve())] = binding
                    compiled_wasi[(width, name)] = artifact
                except (OSError, ValueError) as error:
                    record.update(status='FAIL', reason=str(error))
            rows.append(record)
    if len(compiled_wasi) == sum(len(wasi_cli.fixtures(width)) for width in (32, 64)):
        for name, options, success in wasi_cli.cases(wasm, compiled_wasi):
            run(name, options, success)
    else:
        rows.append({'name':'wasi-current-worker-behavior-matrix', 'status':'BLOCKED',
                     'reason':'A mandatory WASI fixture was not compiled',
                     'scope':'No vacuous pass for the unexecuted CLI behavior matrix'})
    for active in (1, 2):
        disabled = 3-active
        connection = {'gateway-socket':str(directory/'unused.sock'), 'gateway-uid':str(os.getuid()),
                      'node-name':'uwvm_cli_guard', 'namespace':'/cli', 'connect-timeout-ms':'1000',
                      'audit-deny-max-rate-hz':'100', 'audit-deny-burst':'10',
                      'audit-deny-max-records':'100', 'audit-deny-max-bytes':'65536'}
        connection.update({'master-uri':'http://127.0.0.1:11339', 'advertise-host':'127.0.0.1', 'transport':'tcpros'} if active==1 else
                          {'domain-id':'73', 'rmw-implementation':'rmw_fastrtps_cpp', 'security-mode':'disabled-test',
                           'keystore':'not-applicable', 'enclave':'not-applicable'})
        options = [f'--ros{disabled}-global-enable', f'--ros{disabled}-global-disable', f'--ros{active}-global-enable']
        for key, value in connection.items():
            options += [f'--ros{active}-global-{key}', value]
        # Validate the remaining explicit target separately from starting it.
        # No Gateway owns unused.sock: an enabled target must attest its peer
        # before even a benign Wasm start function can run. Safe default WASI
        # isolation reaches that preflight; it does not make a missing peer
        # usable or authorize the Guest to skip authentication. Require the
        # precise startup status and backend-specific diagnostic, rather than
        # accepting an arbitrary failure as evidence of the isolation rule.
        run(f'disabled-ros{disabled}-preserves-active-ros{active}-policy-validation',
            [*options, f'--ros{active}-check-policy'], True)
        run(f'disabled-ros{disabled}-preserves-active-ros{active}-default-isolation-missing-peer',
            [*options, '--run', str(wasm)], False,
            f'gateway preflight failed for ros{active}/global/', expected_returncode=126)
        run(f'disabled-ros{disabled}-preserves-active-ros{active}-strict-guard',
            [*options, '--wasip1-global-inherit-system-environment', '--run', str(wasm)],
            False, 'requires WASI host environment isolation')
args.output.parent.mkdir(parents=True, exist_ok=True)
args.output.write_text(json.dumps({'status': 'PASS' if all(r['status']=='PASS' for r in rows) else 'FAIL',
                                  'scalar_contract_cases':scalar_count,'typed_parameter_event_cases':typed_event_count,
                                  'expanded_parameter_event_cases':expanded_count,
                                  'signed_set_cases':signed_set_count,'array_contract_cases':array_count,
                                  'array_set_scope_cases':array_set_count,'cases': rows}, indent=2))
for r in rows:
    print(r['status'], r['name'])
sys.exit(0 if all(r['status']=='PASS' for r in rows) else 1)
