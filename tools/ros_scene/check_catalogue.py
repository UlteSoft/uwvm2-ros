#!/usr/bin/env python3
"""Validate scene design references without claiming provider implementation.

Run through in_existing_cgroup.py. The check rejects duplicate/ambiguous names,
undefined record types, planned exports mislabelled as implemented, drift from
the concrete local operation table, and source evidence whose bytes changed.
It never executes downloaded ROS code or starts a native provider.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import check_operation_design


ROOT = Path(__file__).resolve().parents[2]
GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--corpus', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    require(Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GATE.relative_to('/sys/fs/cgroup')), 'must use existing cgroup runner')
    require((GATE/'memory.max').read_text().strip() == '68719476736', 'memory limit changed')
    require((GATE/'memory.swap.max').read_text().strip() == '0', 'swap limit changed')
    path = Path(__file__).with_name('interface-catalogue.json')
    data = json.loads(path.read_text())
    require(data['abi_module'] == 'uwvm_ros_v2', 'unexpected module boundary')
    schemas = data['schemas']
    primitives = {'u32', 'u64', 'i64', 'f64', 'bool', 'text', 'bytes'}

    def check_type(value):
        if value in schemas or value in primitives:
            return
        generic = re.fullmatch(r'(?:array|optional)<(.+)>', value)
        require(generic is not None, 'undefined record type: ' + value)
        check_type(generic.group(1))

    def check_fields(fields):
        names = set()
        for name, kind in fields:
            require(name not in names, 'duplicate field: ' + name)
            names.add(name)
            check_type(kind)

    for name, record in schemas.items():
        require(bool(record.get('fields') is not None) != bool(record.get('variant') is not None), 'schema needs fields XOR variant: ' + name)
        check_fields(record.get('fields', []))
        if 'variant' in record:
            tags = record.get('variant_tags', {})
            require(set(tags) == set(record['variant']), 'variant tags missing: ' + name)
            require(len(set(tags.values())) == len(tags) and all(type(x) is int and 0 <= x < 2**64 for x in tags.values()), 'invalid/duplicate variant tag: ' + name)
        for fields in record.get('variant', {}).values():
            check_fields(fields)
    operations = data['operations']
    names = [row['name'] for row in operations]
    require(len(names) == len(set(names)), 'duplicate operation')
    allowed_actions = set(names) | {item['name'] for item in data['grant_only_actions']}
    for op in operations:
        name = op['name']
        require(re.fullmatch(r'[a-z][a-z0-9]*(?:\.[a-z][a-z0-9]*)*', name), 'ambiguous operation spelling: ' + name)
        require(not re.search(r'(?:^|\.)v[0-9]+(?:\.|$)', name), 'internal version name: ' + name)
        require(op['action'] == name and op['contract'], 'missing exact admission action/contract: ' + name)
        require(set(op['secondary_actions']) <= allowed_actions, 'undefined secondary action: ' + name)
        require(set(op.get('additional_action_by_tag', {}).values()) <= allowed_actions, 'undefined tagged transition action: ' + name)
        require(op['implementation'] in {'registered_local', 'planned_provider'}, 'bad implementation status')
        if op['implementation'] == 'planned_provider':
            require(op['wire'] == 'proposed_record', 'planned provider labelled as registered wire: ' + name)
            require(all(op['verification'][key] == 'not_qualified' for key in ('native_semantics', 'wasm_execution', 'performance')), 'design row claims execution: ' + name)
        check_fields([(item['name'], item['type']) for item in op['request']])
        check_fields([(item['name'], item['type']) for item in op['result']])

    header = ROOT/'src/uwvm2/imported/ros/runtime/contract.h'
    concrete = {n: (int(w), int(b)) for n, w, b in re.findall(r' X\(\w+,"([^"]+)","[^"]+",(\d+),(-?\d+)\)', header.read_text())}
    artifact_header = ROOT/'src/uwvm2/imported/ros/scene/artifact_contract.h'
    artifacts = {n: (int(w), int(b)) for n, w, b in re.findall(r' X\(\w+,\w+,"([^"]+)",(\d+),(-?\d+)\)', artifact_header.read_text())}
    concrete.update(artifacts)
    revocation_header=ROOT/'src/uwvm2/imported/ros/scene/revocation_contract.h'
    concrete.update({n:(int(w),int(b)) for n,w,b in re.findall(r' X\(\w+,\s*\w+,\s*"([^"]+)",\s*(\d+),\s*(\d+)\)',revocation_header.read_text())})
    output_header=ROOT/'src/uwvm2/imported/ros/scene/output_contract.h'
    outputs={n:(int(w),72) for n,w in re.findall(r' X\(\w+,\w+,"([^"]+)",(-?\d+)\)',output_header.read_text())}
    concrete.update(outputs)
    managed_header=ROOT/'src/uwvm2/imported/ros/scene/managed_lifecycle_contract.h'
    concrete.update({n:(int(w),int(b)) for n,w,b in re.findall(r' X\(\w+,\w+,"([^"]+)",(\d+),(\d+)\)',managed_header.read_text())})
    buffer_header=ROOT/'src/uwvm2/imported/ros/scene/buffer_contract.h'
    buffers=set(re.findall(r' X\(\w+,\w+,"([^"]+)"\)',buffer_header.read_text()))
    model_header=ROOT/'src/uwvm2/imported/ros/scene/model_contract.h'
    models=set(re.findall(r' X\(\w+,\s*\w+,\s*"([^"]+)"\)',model_header.read_text()))
    transform_header=ROOT/'src/uwvm2/imported/ros/scene/transform_contract.h'
    transforms=set(re.findall(r' X\(\w+,\s*\w+,\s*"([^"]+)"\)',transform_header.read_text()))
    log_diagnostic_header=ROOT/'src/uwvm2/imported/ros/scene/log_diagnostic_contract.h'
    log_diagnostics=set(re.findall(r' X\(\w+,\s*\w+,\s*"([^"]+)"\)',log_diagnostic_header.read_text()))
    additional_headers={family:ROOT/f'src/uwvm2/imported/ros/scene/{family}_contract.h' for family in ('recording','sensor','proposal')}
    additional={family:set(re.findall(r' X\(\w+,\s*\w+,\s*"([^"]+)"\)',header.read_text())) for family,header in additional_headers.items()}
    communication_headers={family:ROOT/f'src/uwvm2/imported/ros/scene/{family}_contract.h' for family in ('communication','communication_receive','communication_creation_control')}
    communications=set().union(*(set(re.findall(r'X\(\w+,\s*\w+,\s*"([^"]+)"\)',header.read_text())) for header in communication_headers.values()))
    parameter_header=ROOT/'src/uwvm2/imported/ros/scene/parameter_contract.h'
    parameter_operations=set(re.findall(r'X\(\w+,\s*\w+,\s*"([^"]+)"\)',parameter_header.read_text()))
    action_header=ROOT/'src/uwvm2/imported/ros/scene/communication_action_contract.h'
    action_operations=set(re.findall(r'X\(\w+,\s*\w+,\s*"([^"]+)"\)',action_header.read_text()))
    observation_headers={family:ROOT/('src/uwvm2/imported/ros/scene/'+header) for family,header in [('graph','communication_graph_contract.h'),('event','event_contract.h')]}
    observation_operations={family:set(re.findall(r'X\(\w+,\s*\w+,\s*"([^"]+)"\)',header.read_text())) for family,header in observation_headers.items()}
    robot_header = ROOT/'src/uwvm2/imported/ros/scene/robot_provider_contract.h'
    robot_registered = {n for n in re.findall(r'X\(\w+,\s*\w+,\s*"([^"]+)"\)', robot_header.read_text())}
    host_text=(ROOT/'src/uwvm2/uwvm/imported/scene/host.h').read_text()
    robot_registered={n for n in robot_registered if not n.startswith('component.')}
    for family in ('DEVICE','CONTROL','MEDIA','BRIDGE'):
        wrapper='DEVICE' if family=='CONTROL' else family
        require('UWVM_ROS_'+family+'_FUNCTIONS(UWVM_'+wrapper+'_FUNCTION)' in host_text, 'robot registration missing: '+family)
    module_operations=set().union(*(set(re.findall(r'X\(\w+,\s*\w+,\s*"([^"]+)"\)', (ROOT/'src/uwvm2/imported/ros/scene'/header).read_text())) for header in ('permission_report.h','module_exchange_contract.h')))
    driver_operations=set(re.findall(r'X\(\w+,\s*\w+,\s*"([^"]+)"\)', (ROOT/'src/uwvm2/imported/ros/scene/driver_contract.h').read_text()))
    require('UWVM_ROS_DRIVER_FUNCTIONS(UWVM_DRIVER_FUNCTION)' in host_text,'driver registration missing')
    implemented = {op['name']: op for op in operations if op['implementation'] == 'registered_local'}
    require(set(implemented) == set(concrete) | driver_operations | module_operations | robot_registered | buffers | models | transforms | log_diagnostics | communications | parameter_operations | action_operations | set().union(*observation_operations.values()) | set().union(*additional.values()) | {'abi', 'capability.open', 'capability.close'}, 'implemented inventory differs from concrete local imports')
    for name, (words, size) in concrete.items():
        row = implemented[name]
        require(row['request_words'] == words and row['result_bytes'] == size, 'wire extent drift: ' + name)
        require(len(row['request']) == words or (name=='artifact.append' and words==-3 and len(row['request'])==4 and row['request'][-1]==dict(name='data',type='bytes')) or (name=='artifact.publish' and words==-2 and row['request']==[dict(name='key',type='u64'),dict(name='size',type='u64'),dict(name='data',type='bytes')]) or (name == 'artifact.open' and row['request'] == [dict(name='generation', type='u64'), dict(name='sha256', type='bytes')]), 'request field/word mismatch: ' + name)
        require(row['wasm_signature'] == dict(parameters=['i64', 'i32', 'i64', 'i64', 'i64', 'i64', 'i64'], results=['i32']), 'common signature drift')
    for name in robot_registered:
        row=implemented[name]
        require(row['wire']=='canonical_record' and row['request_words']==-1, 'robot codec drift: '+name)
        require(row['result_bytes']==(8 if name in ('device.open','media.open','bridge.create','component.open') else 32 if name=='control.open' else -1), 'robot result drift: '+name)
        require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']), 'robot signature drift: '+name)
    for name in buffers:
        row=implemented[name]
        require(row['wire']=='canonical_record' and row['request_words']==-1,'buffer codec drift: '+name)
        require(row['result_bytes']=={'buffer.create':8,'buffer.write':8,'buffer.seal':16,'buffer.read':-1,'buffer.describe':-1}[name],'buffer result drift: '+name)
        require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']),'buffer signature drift')
    for name, parameters in [('abi', []), ('capability.open', ['i32', 'i64', 'i64', 'i64']), ('capability.close', ['i64'])]:
        require(implemented[name]['wasm_signature'] == dict(parameters=parameters, results=['i32']), 'special signature mismatch: ' + name)
    host = ROOT/'src/uwvm2/uwvm/imported/scene/host.h'
    require('module_name{u8"uwvm_ros_v2"}' in host.read_text(), 'concrete module changed')
    require({name for name in re.findall(r'\w+_name\[\]=u8"([^"]+)"', host.read_text())} == {'abi', 'capability.open', 'capability.close'}, 'concrete special import names changed')

    require('UWVM_ROS_REVOCATION_FUNCTIONS(UWVM_REVOCATION_FUNCTION)' in host.read_text(), 'revocation registration missing')
    require('UWVM_ROS_ARTIFACT_FUNCTIONS(UWVM_ARTIFACT_FUNCTION)' in host.read_text(), 'artifact registration missing')

    require('UWVM_ROS_OUTPUT_FUNCTIONS(UWVM_OUTPUT_FUNCTION)' in host.read_text(), 'output registration missing')
    require('UWVM_ROS_MANAGED_LIFECYCLE_FUNCTIONS(UWVM_MANAGED_FUNCTION)' in host.read_text(), 'lifecycle registration missing')
    require('UWVM_ROS_BUFFER_FUNCTIONS(UWVM_BUFFER_FUNCTION)' in host.read_text(), 'buffer registration missing')
    require('UWVM_ROS_MODEL_FUNCTIONS(UWVM_MODEL_FUNCTION)' in host.read_text() and
            'UWVM_ROS_REQUEST_FUNCTIONS(UWVM_MODEL_FUNCTION)' in host.read_text(), 'model/request registration missing')
    for name in models:
        row=implemented[name]
        require(row['wire']=='canonical_record' and row['request_words']==-1,'model codec drift: '+name)
        require(row['result_bytes']==(8 if name in ('model.open','request.release') else -1),'model result drift: '+name)
        require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']),'model signature drift')
        expected=['model.language','model.vision','model.world']+(['transform','arbiter','node','topic','service','parameter','action'] if name.startswith('request.') else [])
        require(row['implemented_purposes']==expected,'model/request scope drift')
    require('UWVM_ROS_TRANSFORM_FUNCTIONS(UWVM_TRANSFORM_FUNCTION)' in host.read_text(),'transform registration missing')
    require(len(transforms)==9,'transform operation count drift')
    require(log_diagnostics=={'log.emit','diagnostic.read'},'log/diagnostic operation drift')
    require('UWVM_ROS_LOG_DIAGNOSTIC_FUNCTIONS(UWVM_LOG_DIAGNOSTIC_FUNCTION)'in host.read_text(),'log/diagnostic registration missing')
    for name in log_diagnostics:
        row=implemented[name]
        require(row['wire']=='canonical_record'and row['request_words']==-1,'log/diagnostic codec drift: '+name)
        require(row['result_bytes']==(48 if name=='log.emit'else -1),'log/diagnostic result drift: '+name)
        require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']),'log/diagnostic signature drift')
        require(row['implemented_purposes']==(['log']if name=='log.emit'else['diagnostic']),'log/diagnostic scope drift')
    require(implemented['diagnostic.read']['secondary_actions']==['resource.observe'],'diagnostic source observation missing')
    sizes={'recording.open':8,'recording.append':104,'recording.checkpoint':104,'recording.stop':104,
           'replay.open':8,'replay.read':-1,'replay.seek':8,'sensor.open':8,'sensor.take':-1,'sensor.describe':-1,
           'proposal.capture':-1,'proposal.publish':8,'proposal.read':-1,'proposal.accept':-1}
    require(set().union(*additional.values())==set(sizes),'recording/sensor/proposal operation drift')
    for family,names in additional.items():
        require(f'UWVM_ROS_{family.upper()}_FUNCTIONS(UWVM_{family.upper()}_FUNCTION)' in host.read_text(),family+' registration missing')
        for name in names:
            row=implemented[name]
            require(row['wire']=='canonical_record' and row['request_words']==-1,'canonical codec drift: '+name)
            require(row['result_bytes']==sizes[name],'result extent drift: '+name)
            require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']),'signature drift: '+name)
            expected='arbiter' if name=='proposal.accept' else name.split('.')[0]
            require(row['implemented_purposes']==[expected],'purpose drift: '+name)
    require(implemented['sensor.take']['secondary_actions']==['data.export'],'sensor disclosure authority missing')
    for name in transforms:
        row=implemented[name]
        require(row['wire']=='canonical_record' and row['request_words']==-1,'transform codec drift: '+name)
        require(row['result_bytes']==(8 if name=='transform.open' else 48 if name=='transform.velocity' else -1),'transform result drift: '+name)
        require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']),'transform signature drift')
        require(row['implemented_purposes']==['transform'],'transform scope drift')

    for family in ('COMMUNICATION','COMMUNICATION_RECEIVE','COMMUNICATION_CREATION'):
        require(f'UWVM_ROS_{family}_FUNCTIONS(UWVM_{family}_FUNCTION)' in host.read_text(),'communication registration missing')
    for name in communications:
        row=implemented[name]
        require(row['wire']=='canonical_record' and row['request_words']==-1,'communication codec drift: '+name)
        require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']),'communication signature drift: '+name)
        require(set(row['implemented_purposes'])<=set(row['purposes'])&{'context','node','type','topic','service','action'},'communication scope drift: '+name)
    require(len(parameter_operations)==12 and 'UWVM_ROS_PARAMETER_FUNCTIONS(UWVM_PARAMETER_FUNCTION)' in host.read_text(),'parameter registration missing')
    for name in parameter_operations:
        row=implemented[name]
        require(row['wire']=='canonical_record' and row['request_words']==-1,'parameter codec drift: '+name)
        require(row['result_bytes']==(8 if name=='parameter.open' else 24 if name=='parameter.exists' else -1),'parameter extent drift: '+name)
        require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']),'parameter signature drift')
        require(row['implemented_purposes']==['parameter'],'parameter scope drift')
    require(len(action_operations)==7 and 'UWVM_ROS_ACTION_FUNCTIONS(UWVM_ACTION_FUNCTION)' in host.read_text(),'action registration missing')
    for name in action_operations:
        row=implemented[name]
        require(row['wire']=='canonical_record' and row['request_words']==-1,'action codec drift: '+name)
        require(row['result_bytes']==(48 if name=='action.open' else -1),'action extent drift: '+name)
        require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']),'action signature drift')
        require(row['implemented_purposes']==['action'],'action scope drift')
    for family,names in observation_operations.items():
        require('UWVM_ROS_'+family.upper()+'_FUNCTIONS(UWVM_'+family.upper()+'_FUNCTION)' in host.read_text(),'observation registration missing: '+family)
        require(len(names)==(2 if family=='graph' else 3),'observation inventory drift')
        for name in names:
            row=implemented[name]
            require(row['wire']=='canonical_record' and row['request_words']==-1,'observation codec drift: '+name)
            require(row['result_bytes']==(-1 if name in {'graph.snapshot','event.stream.poll'} else 8),'observation extent drift: '+name)
            require(row['wasm_signature']==dict(parameters=['i64','i32','i64','i64','i64','i64','i64'],results=['i32']),'observation signature drift')
            require(row['implemented_purposes']==(['discovery'] if family=='graph' else ['event']),'observation scope drift')
    evidence = []
    for record in data['reviewed_source_records']:
        relative = Path(record['path'])
        require(not relative.is_absolute() and '..' not in relative.parts, 'unsafe evidence path')
        source = args.corpus/'sources'/record['source_id']/relative
        require(digest(source) == record['file_sha256'], 'source evidence changed: ' + str(relative))
        receipt = json.loads((args.corpus/'receipts'/(record['source_id'] + '.json')).read_text())
        require(receipt['archive']['sha256'] == record['archive_sha256'], 'archive identity mismatch')
        require(receipt['source']['url'] == record['url'] and receipt['source']['ref'] == record['ref'], 'source ref mismatch')
        require(1 <= record['line_start'] <= record['line_end'] <= len(source.read_text().splitlines()), 'source range outside file')
        evidence.append(record['id'])
    prior = json.loads((ROOT/data['prior_review_cases_file']).read_text())
    require(set(data['prior_review_case_ids']) <= {case['id'] for case in prior['cases']}, 'unknown earlier source review')
    design_paths = ['tools/ros_scene/operation-contracts.json', 'tools/ros_scene/robot-provider-contracts.json']
    designs = [check_operation_design.load(ROOT/name) for name in design_paths]
    design_report = check_operation_design.audit(data, *designs)
    for name, expected in zip([check_operation_design.DOCUMENT, check_operation_design.HEADER],
                              check_operation_design.render(data, *designs)):
        require((ROOT/name).read_text() == expected, 'generated operation design drift: ' + name)
    report = dict(status='PASS', scope='catalogue and source-evidence consistency only; no provider execution',
                  cgroup=str(GATE), cgroup_inode=GATE.stat().st_ino,
                  memory_max=(GATE/'memory.max').read_text().strip(), memory_swap_max=(GATE/'memory.swap.max').read_text().strip(),
                  catalogue_sha256=digest(path), header_sha256=digest(header), artifact_header_sha256=digest(artifact_header), output_header_sha256=digest(output_header), managed_header_sha256=digest(managed_header), buffer_header_sha256=digest(buffer_header), model_header_sha256=digest(model_header),
                  registered_local=len(implemented), planned_provider=len(operations)-len(implemented),
                  transform_header_sha256=digest(transform_header),
                  log_diagnostic_header_sha256=digest(log_diagnostic_header),
                  additional_header_sha256={family:digest(header) for family,header in additional_headers.items()},
                  communication_header_sha256={family:digest(header) for family,header in communication_headers.items()},
                  parameter_header_sha256=digest(parameter_header),
                  revocation_header_sha256=digest(revocation_header), schemas=len(schemas), source_evidence_verified=evidence,
                  operation_design=design_report, design_sha256={name:digest(ROOT/name) for name in design_paths})
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PASS: catalogue consistency; %d registered local, %d planned, %d source evidence records' % (len(implemented), len(operations)-len(implemented), len(evidence)))


if __name__ == '__main__':
    main()
