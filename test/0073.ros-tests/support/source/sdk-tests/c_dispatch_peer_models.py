#!/usr/bin/env python3
"""Review C dispatch checks using memory-only text mutations.

Run this portable test against a complete checkout with:
    python3 -B sdk/origin/tests/c_dispatch_peer_models.py --root PATH --report NEW.json

The report is created exclusively; existing evidence is never overwritten.
No compiler, Wasm instance, ROS process or real indirect call is executed.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--root', type=Path, required=True)
parser.add_argument('--report', type=Path, required=True)
args = parser.parse_args()
root = args.root.resolve()
if args.report.exists():
    raise FileExistsError('Preserve existing evidence: ' + str(args.report))
sys.path.insert(0, str(root/'sdk/origin/tools'))
spec = importlib.util.spec_from_file_location('dispatch_peer', root/'sdk/origin/tools/check_bindings.py')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
import abi_contract

files = ['sdk/origin/tools/check_bindings.py', 'sdk/origin/tools/abi_contract.py',
         'sdk/origin/abi/abi-v1.json', abi_contract.HOST,
         'sdk/origin/include/roswasm/roswasmc.h',
         'sdk/origin/include/roswasm/ros1.h', 'sdk/origin/include/roswasm/ros2.h']
before = {f: hashlib.sha256((root/f).read_bytes()).hexdigest() for f in files}
schema = module.extract()
assert schema == json.loads((root/'sdk/origin/abi/abi-v1.json').read_text())
rpc_types = {tuple([p['type'] for p in f['params']]+f['results'])
             for b in schema['modules'].values() for f in b['functions'] if f['form']=='rpc'}
assert len(rpc_types) == 1, 'Review model requires current uniform RPC signature'
base = {f: (root/f).read_text() for f in files if '/include/' in f}

class MemoryPath:
    """Minimal path adapter that never creates mutated files."""
    def __init__(self, content, path=''):
        self.content, self.path = content, path
    def __truediv__(self, part):
        return MemoryPath(self.content, self.path+'/'+str(part) if self.path else str(part))
    def read_text(self):
        return self.content[self.path]
    def relative_to(self, other):
        return self.path

cases = []
def check(name, changes=None, accept=False):
    """Require the real checker's decision to equal the independently selected result."""
    module.ROOT = MemoryPath({**base, **(changes or {})})
    error = None
    try:
        tables = module.check_c_dispatch(schema)
    except ValueError as caught:
        error, tables = str(caught), None
    accepted = error is None
    assert accepted == accept, (name, error, accept)
    cases.append({'name': name, 'accepted': accepted,
                  'expected': 'accept' if accept else 'reject', 'error': error})
    return tables

def parts(version):
    relative = f'sdk/origin/include/roswasm/ros{version}.h'
    text = base[relative]
    match = re.search(r'static roswasmc_backend const backend\s*=\s*\{(.*?)\};', text, re.DOTALL)
    prefix, array = match.group(1).strip().split('{', 1)
    fields = [part.strip() for part in prefix.rstrip().rstrip(',').split(',')]
    rpc = [part.strip() for part in array[:-1].split(',')]
    return relative, text, match, fields, rpc

def table(version, fixed=None, rpc=None, new=None, swap=None):
    relative, text, match, fields, requests = parts(version)
    if fixed is not None:
        fields[fixed] = new
    if rpc is not None:
        requests[rpc] = new
    if swap is not None:
        a,b = swap
        requests[a], requests[b] = requests[b], requests[a]
    initializer = '\n'+','.join(fields)+',\n{'+','.join(requests)+'}\n'
    return {relative: text[:match.start(1)]+initializer+text[match.end(1):]}

def swap_names(text, a, b):
    assert a in text and b in text
    return text.replace(a, '__PEER_SWAP__').replace(b, a).replace('__PEER_SWAP__', b)

tables = check('unchanged_actual_sources', accept=True)
assert [(t['fixed_slots'], t['RPC_slots']) for t in tables] == [(11,33),(11,33)]
for version in (1,2):
    _, _, _, fields, requests = parts(version)
    existing = [symbol for symbol in requests if symbol != '0']
    for index, value in enumerate(requests):
        other = next(symbol for symbol in existing if symbol != value)
        check(f'ros{version}_rpc_slot_{index}_wrong_same_signature_import',
              table(version, rpc=index, new=other))
    for index, value in enumerate(fields):
        other = str(3-version) if index == 0 else ('0' if value != '0' else f'uwvm_ros{version}_service_advertise')
        check(f'ros{version}_fixed_slot_{index}_wrong_value', table(version, fixed=index, new=other))
    check(f'ros{version}_close_revoke_same_signature_swap', table(version, fixed=3, new=fields[4]))
    check(f'ros{version}_parameter_events_wrong_fixed', table(version, fixed=9, new=f'uwvm_ros{version}_service_advertise'))
    check(f'ros{version}_parameter_events_in_rpc', table(version, rpc=0, new=f'uwvm_ros{version}_parameter_subscribe_events'))

relative = 'sdk/origin/include/roswasm/roswasmc.h'
source = base[relative]
swapped = swap_names(source, 'ROSWASMC_PARAMETER_GET', 'ROSWASMC_PARAMETER_DELETE')
check('enum_get_delete_swap_without_table_update', {relative: swapped})
check('enum_duplicate', {relative: source.replace('ROSWASMC_PARAMETER_DELETE,', 'ROSWASMC_PARAMETER_GET,', 1)})
check('enum_parameter_events_in_rpc_union', {relative: source.replace('ROSWASMC_PARAMETER_DELETE,', 'ROSWASMC_PARAMETER_SUBSCRIBE_EVENTS,', 1)})
check('enum_unparsed_numeric_assignment', {relative: source.replace('ROSWASMC_PARAMETER_GET,', 'ROSWASMC_PARAMETER_GET=11,', 1)})
coherent = {relative: swapped}
for version in (1,2):
    coherent.update(table(version, swap=(11,13)))
check('coherent_enum_and_table_reorder', coherent, accept=True)
check('struct_close_revoke_declarations_swapped',
      {relative: swap_names(source, 'uint32_t (*close)(uint64_t);', 'uint32_t (*revoke)(uint64_t);')})

layout = re.search(r'typedef struct roswasmc_backend\s*\{(.*?)\}\s*roswasmc_backend\s*;', source, re.DOTALL)
assert layout is not None
field_rows = list(re.finditer(r'uint32_t\s*\(\s*\*\s*(\w+)\s*\)\s*\(([^()]*)\)\s*;', layout.group(1)))
assert len(field_rows) == 10
for row in field_rows:
    member, params = row.group(1), row.group(2)
    original = row.group(0)
    check('struct_'+member+'_wrong_result_width',
          {relative: source.replace(original, original.replace('uint32_t', 'uint64_t', 1), 1)})
    pieces = [p.strip() for p in params.split(',') if p.strip() and p.strip()!='void']
    for index, width in enumerate(pieces):
        assert width in ('uint32_t','uint64_t')
        changed = list(pieces)
        changed[index] = 'uint64_t' if width=='uint32_t' else 'uint32_t'
        new = f'uint32_t (*{member})('+','.join(changed)+');'
        check(f'struct_{member}_param_{index}_wrong_width',
              {relative: source.replace(original, new, 1)})
    wrong_arity = pieces[:-1] if pieces else ['uint64_t']
    new = f'uint32_t (*{member})('+(','.join(wrong_arity) if wrong_arity else 'void')+');'
    check('struct_'+member+'_wrong_arity', {relative: source.replace(original, new, 1)})
    check('struct_'+member+'_wrong_member_name',
          {relative: source.replace(original, original.replace('(*'+member+')','(*wrong_'+member+')'),1)})

check('struct_version_wrong_width', {relative: source.replace('uint32_t version;', 'uint64_t version;', 1)})
request_decl = 'roswasmc_request_import request[ROSWASMC_OPERATION_COUNT];'
assert request_decl in source
for suffix,new in [
    ('wrong_type','uint32_t request[ROSWASMC_OPERATION_COUNT];'),
    ('wrong_name','roswasmc_request_import other[ROSWASMC_OPERATION_COUNT];'),
    ('wrong_array_bound','roswasmc_request_import request[33];'),
    ('missing_member',''),
    ('extra_member',request_decl+' uint32_t extra;')]:
    check('struct_request_'+suffix, {relative: source.replace(request_decl,new,1)})
request_type = re.search(r'typedef\s+uint32_t\s*\(\s*\*\s*roswasmc_request_import\s*\)\s*\(([^()]*)\)\s*;',source)
assert request_type is not None
original = request_type.group(0)
pieces = [part.strip() for part in request_type.group(1).split(',')]
assert len(pieces)==7
for index,width in enumerate(pieces):
    changed = list(pieces)
    changed[index] = 'uint64_t' if width=='uint32_t' else 'uint32_t'
    new = 'typedef uint32_t (*roswasmc_request_import)('+','.join(changed)+');'
    check(f'request_typedef_param_{index}_wrong_width', {relative: source.replace(original,new,1)})
for suffix,new in [
    ('wrong_result_width',original.replace('uint32_t','uint64_t',1)),
    ('signed_result',original.replace('uint32_t','int32_t',1)),
    ('wrong_name',original.replace('roswasmc_request_import','wrong_request_import')),
    ('wrong_arity','typedef uint32_t (*roswasmc_request_import)('+','.join(pieces[:-1])+');')]:
    check('request_typedef_'+suffix, {relative: source.replace(original,new,1)})

module.ROOT = root
after = {f: hashlib.sha256((root/f).read_bytes()).hexdigest() for f in files}
assert before == after, 'Inputs changed during review'
report = {
    'status': 'PURE_SOURCE_MUTATION_MODELS_PASS',
    'scope': 'In-memory textual C mutations against source checker only; no language compilation, Wasm, ROS, or indirect-call execution',
    'models': len(cases), 'expected_rejections': sum(not c['accepted'] for c in cases),
    'baseline_slots': 88, 'fixed_struct_members': 12,
    'inputs_unchanged': True, 'source_inputs_sha256': before,
    'test_script_sha256': hashlib.sha256((_CHECKOUT_ROOT / 'test/0073.ros-tests/support/source/sdk-tests/c_dispatch_peer_models.py').read_bytes()).hexdigest(),
    'previous_close_revoke_layout_gap': 'REJECTED_BY_CURRENT_CHECKER',
    'cases': cases
}
with args.report.open('x') as stream:
    json.dump(report, stream, indent=2)
    stream.write('\n')
print(json.dumps({'status':report['status'], 'models':report['models'],
                  'expected_rejections':report['expected_rejections'],
                  'report':str(args.report),
                  'report_sha256':hashlib.sha256(args.report.read_bytes()).hexdigest()}))
