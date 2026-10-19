#!/usr/bin/env python3
"""Extract and validate the public guest ABI against the actual host registry.

This program reads source declarations only. It never compiles or executes a
Worker, Gateway, Wasm guest, ROS process or network client. Run ``--write`` after
an intentional ABI change, then review the JSON diff; ordinary CI uses the
default check mode and fails on registry drift. A known optional client catalog
may separately attest the whole new Host while preserving the historical81
catalog bytes/hash; original module declarations and metadata must remain exact.
The extractor deliberately
rejects unknown declaration forms instead of guessing their Wasm signature.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
HOST = 'src/uwvm2/uwvm/imported/ros/host.h'

# Parameter names describe the binary ABI, independently of abbreviated C++
# implementation variable names. Every pointer-like value is an i64 offset.
FORMS = {
    'version': [],
    'open': [('memory', 'i32'), ('alias_offset', 'i64'), ('alias_length', 'i64'), ('handle_out_offset', 'i64')],
    'handle': [('handle', 'i64')],
    'write': [('handle', 'i64'), ('memory', 'i32'), ('payload_offset', 'i64'), ('payload_length', 'i64')],
    'poll': [('handle', 'i64'), ('memory', 'i32'), ('output_offset', 'i64'), ('capacity', 'i64'), ('length_out_offset', 'i64')],
    'watch': [('observer_handle','i64'), ('source_handle','i64'), ('correlation','i64')],
    'ready': [('observer_handle','i64'), ('memory','i32'), ('output_offset','i64')],
    'submit': [('source_handle','i64'), ('observer_handle','i64'), ('memory','i32'), ('request_offset','i64'), ('request_length','i64'), ('request_id_out_offset','i64')],
    'rpc': [('handle', 'i64'), ('memory', 'i32'), ('request_offset', 'i64'), ('request_length', 'i64'),
            ('response_offset', 'i64'), ('capacity', 'i64'), ('length_out_offset', 'i64')],
}

DESCRIPTIONS = {
    'abi_version': 'Return ABI version 1. This does not open a capability or authorize a resource.',
    'capability_open': 'Open one explicitly granted alias in the caller Instance and selected backend. Write an opaque eight-byte little-endian handle on success.',
    'capability_close': 'Retire this handle generation. The handle may already be retired if later Gateway cleanup returns an error.',
    'capability_revoke': 'Revoke the alias for this Instance/backend, invalidating all its open sibling handles. Reopening cannot restore the revoked grant.',
    'topic_publish': 'Publish one canonical generated message through a separately granted publisher alias.',
    'topic_subscribe': 'Activate a separately granted subscription queue. Receive messages through event_poll on the guest execution thread.',
    'event_poll': 'Poll a topic, service-request, incoming-goal or ROS2 parameter-event queue. Error 11 means no event; no native callback enters Wasm.',
    'event_watch': 'Bind separately granted event.observe and source owners. Correlation is nonzero only when an owned outgoing Action data kind is selected; metadata-only watches use zero and activate/consume no native payload. No aliases or rights are created.',
    'event_poll_v1': 'Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.',
    'event_ready': 'Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.',
    'service_submit': 'Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.',
    'service_call': 'Call the explicitly selected service using its canonical request/response type and configured deadline. Do not automatically replay a timed-out call.',
    'service_advertise': 'Activate the explicitly granted service server. Poll owned request IDs through event_poll.',
    'service_respond': 'Respond to an owned live request using a separate response grant and u64 request-ID-prefixed canonical response.',
    'action_send_goal': 'Send the selected trusted Action goal codec and receive an opaque u64 Goal ID. Builtin Fibonacci uses one canonical i32 order. Generated bounded Actions require their registered goal codec; all bounds remain explicit.',
    'action_cancel_own_goal': 'Request cancellation of one owned Goal using its u64 ID and a separately authorized cancel capability.',
    'action_cancel_any_goal': 'Use the separately granted any-cancel operation. Goal ID zero is valid only for this independently authorized operation.',
    'action_receive_feedback': 'Receive ID8 plus the selected trusted Action feedback codec for an owned Goal. Builtin Fibonacci appends count32/int32 values; an empty queue returns 11.',
    'action_receive_result': 'Receive ID8/outcome8 plus the selected trusted terminal result codec. Builtin Fibonacci appends count32/int32 values. Successful terminal consumption retires ownership.',
    'action_advertise': 'Activate the explicitly granted Action server with an empty command. Poll ID8 plus the selected trusted goal codec; builtin Fibonacci uses i32 order. Responses require another grant.',
    'action_respond': 'Send ID8/kind8 plus the selected trusted feedback/result codec. Builtin Fibonacci appends count32/int32 values. Advertiser and responder require the same explicit ROS2 QoS contract.',
    'tf_lookup': 'Request the separately configured source/target frame pair at u64 nanoseconds; zero selects the latest admitted transform.',
    'tf_broadcast': 'Broadcast one canonical TransformStamped for the separately configured frame pair. Do not normalize its quaternion implicitly.',
    'parameter_get': 'Read the value fixed by the capability type. An empty request does not select a different node, name or value kind.',
    'parameter_set': 'Set a canonical value of the explicitly granted parameter type. Numeric and element bounds remain enforced by the host.',
    'parameter_delete': 'Delete the explicitly granted ROS1 parameter using an empty request; no implicit parent-name authority is added.',
    'parameter_exists': 'Query existence of the explicitly granted ROS1 parameter; decode one canonical Bool response.',
    'parameter_list': 'List the separately authorized parameter scope; decode the bounded canonical String result.',
    'parameter_search': 'Search the explicitly granted ROS1 resource with an empty request. Parent namespaces are included only with search-parent-namespaces=true, for the same final key and within allowed-name-prefix before every native query.',
    'parameter_describe': 'Read the separately authorized ROS2 parameter metadata as a canonical String.',
    'parameter_declare': 'Declare a typed parameter on the explicitly selected local ROS2 Gateway node. Remote declaration is unsupported.',
    'parameter_undeclare': 'Undeclare a typed parameter on the explicitly selected local ROS2 Gateway node. A missing remote operation is not emulated.',
    'parameter_subscribe_events': 'Activate a separate ROS2 parameter-event projection grant. This takes only one handle, not the seven RPC arguments.',
    'graph_inspect_topic': 'Inspect the configured topic filter with an empty request and decode sorted canonical String rows.',
    'graph_inspect_node': 'Inspect the configured node filter with an empty request and decode sorted canonical String rows.',
    'graph_inspect_service': 'Inspect the configured service filter with an empty request and decode sorted canonical String rows.',
    'graph_inspect_action': 'Inspect the separately configured ROS2 Action filter; ROS1 does not export this function.',
    'graph_list_topics': 'List topics within the explicit exact/prefix graph filter; decode sorted canonical String rows.',
    'graph_list_nodes': 'List nodes within the explicit graph filter; this is not unrestricted discovery authority.',
    'graph_list_services': 'List services within the explicit graph filter; decode sorted canonical String rows.',
    'lifecycle_get_state': 'Read the separately selected ROS2 lifecycle node state as u8 state ID followed by canonical String label.',
    'lifecycle_change_state': 'Apply the single ROS2 transition fixed by policy using an empty request; the guest cannot override that transition.',
    'time_read_ros': 'Read the separately authorized ROS clock domain as little-endian u64 nanoseconds.',
    'time_read_wall': 'Read the independently authorized ROS1 wall clock; another time grant does not imply this one.',
    'time_read_system': 'Read the independently authorized ROS2 system clock as u64 nanoseconds.',
    'time_read_steady': 'Read the independently authorized ROS2 steady clock as u64 nanoseconds.',
    'log_write': 'Write one canonical String to the separately granted ROS logger. This does not open a file or socket.',
}


def module_body(text, name):
    """Select exactly one complete known module declaration, never global entries."""
    rows = re.findall(r'    struct '+re.escape(name)+r'\n    \{(.*?)\n    \};', text, re.S)
    if len(rows) != 1:
        raise ValueError('exact builtin module declaration required: '+name)
    return rows[0]


def preserved_authority(root, text, actual):
    """Verify the known optional extension without rewriting historical v1 bytes.

    A Host source hash mismatch is NEVER just ignored. The separately reviewed
    optional catalog must bind the full actual Host, whole original catalog,
    exact original module declaration hashes, and both actual new registries.
    The enclosing current MF remains source admission, not this data checker.
    """
    optional = root/'sdk/abi/service-client-v1.json'
    if not optional.exists():
        return actual
    extension = json.loads(optional.read_text())
    if (extension.get('schema_version') != 1 or extension.get('extension_version') != 1 or
            extension.get('kind') != 'optional-service-client-v1' or
            extension.get('authority_source') != actual['authority_source']):
        raise ValueError('optional client catalog does not bind the complete actual Host')
    raw = (root/'sdk/abi/abi-v1.json').read_bytes()
    old = json.loads(raw)
    preserved = extension.get('legacy_preservation', {})
    if (set(preserved) != {'catalog_path','catalog_sha256','original_authority_source','original_module_declaration_sha256','scope'} or
            preserved['catalog_path'] != 'sdk/abi/abi-v1.json' or
            preserved['catalog_sha256'] != hashlib.sha256(raw).hexdigest() or
            preserved['original_authority_source'] != old.get('authority_source')):
        raise ValueError('optional client legacy catalog provenance differs')
    hashes = {f'uwvm_ros{number}_v1':hashlib.sha256(module_body(text,f'ros{number}_local_imported_module_t').encode()).hexdigest()
              for number in (1,2)}
    if preserved['original_module_declaration_sha256'] != hashes:
        raise ValueError('an original81 module declaration changed')
    if set(extension.get('modules', {})) != {'uwvm_ros1_client_v1','uwvm_ros2_client_v1'}:
        raise ValueError('closed optional module pair required')
    declarations = dict(re.findall(r'inline\s+::std::uint32_t\s+(\w+)\(([^)]*)\)\s+noexcept', text))
    names = dict(re.findall(r'inline constexpr char8_t (\w+)_name\[\]\s*=\s*u8"([^"]+)";', text))
    expected = [('abi_version','abi_version',[]),('client_features','features',['i64','i64','i32','i64']),
                ('client_service_available','service_available',['i64','i64','i64','i32','i64'])]
    for number in (1,2):
        body = module_body(text,f'ros{number}_client_local_imported_module_t')
        namespace_names = re.findall(r'module_name\{u8"([^"]+)"\}',body)
        if namespace_names != [f'uwvm_ros{number}_client_v1']:
            raise ValueError('optional actual module namespace differs')
        entries = re.findall(r'wrapper::native_function<&host::(\w+)<core::version::ros([12])>,wrapper::(\w+)_name>', body)
        if len(entries) != 3:
            raise ValueError('exact three-function optional registry required')
        functions=[]
        for (function, backend, label),(wanted,name,widths) in zip(entries,expected):
            declaration=declarations.get(function)
            if function != wanted or int(backend) != number or names.get(label) != name or declaration is None:
                raise ValueError('optional registry function/backend/name differs')
            actual_widths=['i'+n for n in re.findall(r'::std::uint(32|64)_t\s+\w+',declaration)]
            if actual_widths != widths or declaration and len(actual_widths) != declaration.count(',')+1:
                raise ValueError('optional registry numeric signature differs')
            functions.append({'name':name,'params':widths,'results':['i32']})
        if extension['modules'][f'uwvm_ros{number}_client_v1'] != {'backend':number,'function_count':3,'functions':functions}:
            raise ValueError('optional machine-readable numeric catalog differs')
    result=dict(actual)
    result['authority_source']=old['authority_source']
    if result != old:
        raise ValueError('original ABI metadata or81 signatures changed')
    return result


def extract(root=None):
    """Extract81 unchanged tuples plus verified optional-source preservation."""
    root = ROOT if root is None else Path(root)
    path = root/HOST
    raw_host = path.read_bytes()
    text = raw_host.decode('utf-8')
    names = dict(re.findall(r'inline constexpr char8_t (\w+)_name\[\]\s*=\s*u8"([^"]+)";', text))
    declarations = dict(re.findall(r'inline\s+::std::uint32_t\s+(\w+)\(([^)]*)\)\s+noexcept', text))
    entries = []
    for number in (1,2):
        body = module_body(text,f'ros{number}_local_imported_module_t')
        if re.findall(r'module_name\{u8"([^"]+)"\}',body) != [f'uwvm_ros{number}_v1']:
            raise ValueError('original actual module namespace differs')
        entries += re.findall(r'wrapper::native_function<&host::(\w+)<core::version::ros([12])>,wrapper::(\w+)_name>', module_body(text,f'ros{number}_local_imported_module_t'))
    modules = {}
    for version in (1, 2):
        functions = []
        for function, backend, label in entries:
            if int(backend) != version:
                continue
            name = names[label]
            if function != name or name not in DESCRIPTIONS:
                raise ValueError('unrecognized registered function: '+name)
            declaration = declarations[function]
            types = re.findall(r'::std::uint(32|64)_t\s+\w+', declaration)
            if declaration and len(types) != declaration.count(',')+1:
                raise ValueError('unrecognized parameter declaration: '+name)
            widths = ['i'+n for n in types]
            candidates = [key for key, parameters in FORMS.items() if widths == [value for _, value in parameters]]
            if len(candidates) != 1:
                raise ValueError('unknown or ambiguous signature: '+name)
            form = candidates[0]
            functions.append({'name': name, 'form': form, 'params': [
                {'name': n, 'type': t} for n, t in FORMS[form]], 'results': ['i32'],
                'description': DESCRIPTIONS[name], 'return_semantics': 'ABI version' if form == 'version' else 'error code'})
        if len({row['name'] for row in functions}) != len(functions):
            raise ValueError('duplicate registered names')
        modules[f'uwvm_ros{version}_v1'] = {'backend': version, 'function_count': len(functions), 'functions': functions}
    actual = {'schema_version': 1, 'abi_version': 1,
        'authority_source': {'path': HOST, 'sha256': hashlib.sha256(raw_host).hexdigest()},
        'memory_contract': {'memory_index': 'i32', 'offset_length_handle': 'i64', 'endianness': 'little',
            'output_slot_bytes': 8, 'pointer_wrappers_memory': 0,
            'maximum_IPC_payload_bytes': 4194304,
            'maximum_request_data_bytes': 4193786,
            'length_error_contract': 'Error 90 may leave the output-length slot untouched. Read it only if a UINT64_MAX sentinel was overwritten. An insufficient legacy receive buffer may already have consumed the event; event_poll_v1 retains its pinned event on capacity90.'},
        'authority_contract': 'All ROS resources start without authority. Opening an alias uses an explicit trusted grant; SDK factories, codecs and node objects grant no rights.',
        'errors': {'0':'success','2':'missing resource','9':'bad/stale/foreign handle','11':'empty queue or quota refusal',
            '13':'permission denied','17':'contract conflict','22':'invalid argument/memory/canonical value','90':'byte/capacity quota',
            '95':'unsupported','107':'disconnected','110':'timeout; admitted effects may persist','125':'revoked alias'},
        'modules': modules}
    return preserved_authority(root, text, actual)


def main():
    """Write reviewed metadata on request, otherwise verify committed bytes."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write', action='store_true')
    args = parser.parse_args()
    expected = extract()
    destination = ROOT/'sdk/abi/abi-v1.json'
    body = json.dumps(expected, indent=2)+'\n'
    if args.write:
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(body)
    elif destination.read_text() != body:
        raise SystemExit('ABI metadata drift; review host registry and regenerate intentionally')
    print('PASS source ABI contract: '+', '.join(f'{name}={m["function_count"]}' for name,m in expected['modules'].items()))


if __name__ == '__main__':
    main()
