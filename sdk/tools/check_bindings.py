#!/usr/bin/env python3
"""Check complete language import declarations against the source ABI schema.

This is a source contract check, not a compiler or ROS behavior test. It checks
exact module/field sets, parameter widths and result widths in C, Rust, TinyGo,
AssemblyScript and JavaScript metadata. C++ uses the checked C declarations.
It also checks every C backend dispatch slot against the Runtime's operation
inventory, so matching declarations cannot conceal a wrong function-table slot.
The parser accepts only the current explicit declaration syntax; it fails
closed when a binding changes shape rather than silently omitting functions.
"""
import argparse
import json
from pathlib import Path
import re

from abi_contract import ROOT, extract


def widths(parameters, pattern, mapping):
    """Convert a complete comma-separated parameter list to Wasm widths."""
    parts=[part.strip() for part in parameters.split(',') if part.strip()]
    if parts==['void']:
        return []
    values=[]
    for part in parts:
        match=re.fullmatch(pattern,part)
        if match is None:
            raise ValueError('unrecognized explicit parameter: '+part)
        values.append(mapping[match.group(1)])
    return values


def parse_c(version):
    """Read the unchanged import macros used by the legacy/public C headers."""
    path=ROOT/f'sdk/ros{version}/abi.h';text=path.read_text()
    module=f'uwvm_ros{version}_v1'
    if f'import_module("{module}")' not in text:
        raise ValueError('C module attribute missing: '+module)
    rows={}
    for name,symbol,params in re.findall(r'UWVM_ROS[12]_IMPORT\("(\w+)"\)\s+uint32_t\s+(\w+)\(([^)]*)\)\s*;',text):
        if symbol!=f'uwvm_ros{version}_{name}' or name in rows:
            raise ValueError('C symbol/name mismatch or duplicate: '+name)
        rows[name]=(widths(params,r'(uint32_t|uint64_t)\s+\w+',{'uint32_t':'i32','uint64_t':'i64'}),['i32'])
    return path,rows


def parse_rust(version):
    """Read fixed-width extern declarations in the selected Rust raw module."""
    path=ROOT/f'sdk/rust/src/raw/ros{version}.rs';text=path.read_text()
    if f'wasm_import_module = "uwvm_ros{version}_v1"' not in text:
        raise ValueError('Rust module attribute missing')
    rows={}
    for name,params,result in re.findall(r'pub\s+fn\s+(\w+)\s*\(([^)]*)\)\s*->\s*(u32)\s*;',text):
        if name in rows:raise ValueError('duplicate Rust import')
        rows[name]=(widths(params,r'\w+\s*:\s*(u32|u64)',{'u32':'i32','u64':'i64'}),['i32'])
    return path,rows


def parse_tinygo(version):
    """Read each go:wasmimport directive and its adjacent fixed-width stub."""
    path=ROOT/f'sdk/tinygo/raw_ros{version}.go';text=path.read_text();rows={}
    for module,name,symbol,params in re.findall(r'//go:wasmimport\s+(\w+)\s+(\w+)\s*\nfunc\s+(\w+)\(([^)]*)\)\s+uint32',text):
        if module!=f'uwvm_ros{version}_v1' or name in rows:
            raise ValueError('TinyGo module or duplicate import mismatch')
        rows[name]=(widths(params,r'\w+\s+(uint32|uint64)',{'uint32':'i32','uint64':'i64'}),['i32'])
    return path,rows


def parse_assemblyscript(version):
    """Read real @external declarations, excluding application-side wrappers."""
    path=ROOT/f'sdk/assemblyscript/ros{version}/raw.ts';text=path.read_text();rows={}
    for module,name,symbol,params in re.findall(r'@external\("(\w+)",\s*"(\w+)"\)\s*export declare function\s+(\w+)\(([^)]*)\)\s*:\s*u32\s*;',text):
        if module!=f'uwvm_ros{version}_v1' or symbol!=name or name in rows:
            raise ValueError('AssemblyScript module/symbol mismatch')
        rows[name]=(widths(params,r'\w+\s*:\s*(u32|u64)',{'u32':'i32','u64':'i64'}),['i32'])
    return path,rows


def parse_javascript(version):
    """Read JSON ABI metadata without executing a JavaScript module."""
    path=ROOT/'sdk/javascript/abi.mjs';text=path.read_text()
    declaration=re.search(r'\bconst inventory\s*=\s*',text)
    if declaration is not None:
        if not re.search(r'export const ABI\s*=\s*freeze\(inventory\)\s*;',text):
            raise ValueError('JavaScript ABI export does not select the reviewed inventory')
        start=declaration.end()
    else:
        declaration=re.search(r'export const ABI\s*=\s*Object\.freeze\(\s*',text)
        if declaration is None:raise ValueError('unrecognized JavaScript ABI metadata declaration')
        start=declaration.end()
    value,_=json.JSONDecoder().raw_decode(text[start:].lstrip())
    module=value[str(version)]
    if module['module']!=f'uwvm_ros{version}_v1':
        raise ValueError('JavaScript bridge metadata module mismatch')
    rows={}
    for row in module['functions']:
        if row['name'] in rows:raise ValueError('duplicate JavaScript metadata')
        rows[row['name']]=([p['type'] for p in row['parameters']],[row['result']])
    return path,rows


def check_c_dispatch(schema):
    """Bind positional C resource dispatch to actual registered RPC names.

    The public operation enum defines indices; each selected backend table must
    use the matching import or zero for an absent backend operation. Fixed
    non-RPC slots are checked separately, particularly handle-only parameter
    events. This is a source-order check, not indirect-call execution evidence.
    """
    def uncomment(body):
        return re.sub(r'/\*.*?\*/|//[^\n]*', '', body, flags=re.DOTALL)
    enum_path=ROOT/'sdk/include/roswasm/roswasmc.h'
    enum_text=uncomment(enum_path.read_text())
    layout=re.search(r'typedef struct roswasmc_backend\s*\{(.*?)\}\s*roswasmc_backend\s*;',enum_text,re.DOTALL)
    if layout is None:raise ValueError('unrecognized C backend struct')
    declarations=[part.strip() for part in layout.group(1).split(';') if part.strip()]
    members=['version','abi_version','open','close','revoke','publish','subscribe','poll',
             'advertise_service','subscribe_parameter_events','log','request']
    functions=['abi_version','capability_open','capability_close','capability_revoke',
               'topic_publish','topic_subscribe','event_poll','service_advertise',
               'parameter_subscribe_events','log_write']
    inventory={f['name']:f for module in schema['modules'].values() for f in module['functions']}
    if len(declarations)!=len(members) or declarations[0]!='uint32_t version':
        raise ValueError('C backend version/member count mismatch')
    for declaration,member,function in zip(declarations[1:-1],members[1:-1],functions):
        field=re.fullmatch(r'uint32_t\s*\(\s*\*\s*(\w+)\s*\)\s*\(([^()]*)\)',declaration)
        expected=[p['type'] for p in inventory[function]['params']]
        if (field is None or field.group(1)!=member or
                widths(field.group(2),r'(uint32_t|uint64_t)',{'uint32_t':'i32','uint64_t':'i64'})!=expected):
            raise ValueError('C backend member order/signature mismatch: '+member)
    if not re.fullmatch(r'roswasmc_request_import\s+request\s*\[\s*ROSWASMC_OPERATION_COUNT\s*\]',declarations[-1]):
        raise ValueError('C backend request-array member mismatch')
    request_type=re.search(r'typedef\s+uint32_t\s*\(\s*\*\s*roswasmc_request_import\s*\)\s*\(([^()]*)\)\s*;',enum_text)
    rpc=next(f for f in inventory.values() if f['form']=='rpc')
    if request_type is None or widths(request_type.group(1),r'(uint32_t|uint64_t)',{'uint32_t':'i32','uint64_t':'i64'})!=[p['type'] for p in rpc['params']]:
        raise ValueError('C backend request-pointer signature mismatch')
    match=re.search(r'typedef enum roswasmc_operation\s*\{([^}]+)\}',enum_text)
    if match is None:raise ValueError('unrecognized C operation enum')
    enum=[]
    for item in match.group(1).split(','):
        item=item.strip()
        if not re.fullmatch(r'ROSWASMC_\w+',item):raise ValueError('unrecognized explicit C operation index')
        enum.append(item)
    if not enum or enum[-1]!='ROSWASMC_OPERATION_COUNT':raise ValueError('missing C operation count')
    operations=[item.removeprefix('ROSWASMC_').lower() for item in enum[:-1]]
    registered={f['name'] for module in schema['modules'].values()
                for f in module['functions'] if f['form']=='rpc'}
    if len(set(operations))!=len(operations) or set(operations)!=registered:
        raise ValueError('C operation enum does not match the union of registered RPC operations')
    results=[]
    for version in (1,2):
        path=ROOT/f'sdk/include/roswasm/ros{version}.h'
        text=uncomment(path.read_text())
        match=re.search(r'static roswasmc_backend const backend\s*=\s*\{(.*?)\};',text,re.DOTALL)
        if match is None:raise ValueError('unrecognized C backend initializer')
        prefix,array=match.group(1).strip().split('{',1)
        if not array.endswith('}') or '{' in array:raise ValueError('unrecognized C request table shape')
        fields=[field.strip() for field in prefix.rstrip().rstrip(',').split(',')]
        requests=[field.strip() for field in array[:-1].split(',')]
        names={f['name'] for f in schema['modules'][f'uwvm_ros{version}_v1']['functions']}
        fixed=['abi_version','capability_open','capability_close','capability_revoke',
               'topic_publish','topic_subscribe','event_poll','service_advertise',
               'parameter_subscribe_events','log_write']
        expected_fields=[str(version)]+[f'uwvm_ros{version}_{name}' if name in names else '0' for name in fixed]
        expected_requests=[f'uwvm_ros{version}_{name}' if name in names else '0' for name in operations]
        if fields!=expected_fields or requests!=expected_requests:
            raise ValueError(f'C ROS{version} backend dispatch slot mismatch')
        results.append({'backend':version,'file':str(path.relative_to(ROOT)),
                        'fixed_slots':len(fields),'RPC_slots':len(requests),
                        'struct_member_order_and_signatures':'PASS','status':'PASS'})
    return results


def check():
    """Validate every backend-qualified function in each supported binding."""
    schema=extract();committed=json.loads((ROOT/'sdk/abi/abi-v1.json').read_text())
    if schema!=committed:raise ValueError('committed ABI schema differs from Runtime source')
    results=[]
    for version in (1,2):
        expected={f['name']:([p['type'] for p in f['params']],f['results'])
                  for f in schema['modules'][f'uwvm_ros{version}_v1']['functions']}
        for language,parser in [('C/C++',parse_c),('Rust',parse_rust),('TinyGo',parse_tinygo),
                                ('AssemblyScript',parse_assemblyscript),('JavaScript bridge metadata',parse_javascript)]:
            path,actual=parser(version)
            if actual!=expected:
                raise ValueError(f'{language} ROS{version} declaration mismatch: missing={sorted(set(expected)-set(actual))}, extra={sorted(set(actual)-set(expected))}, wrong_types={sorted(n for n in expected.keys() & actual.keys() if expected[n]!=actual[n])}')
            results.append({'language':language,'backend':version,'file':str(path.relative_to(ROOT)),
                            'functions':len(actual),'status':'PASS'})
    tables=check_c_dispatch(schema)
    return {'status':'SOURCE_CONTRACT_PASS','scope':'Exact declarations and C dispatch slots only; no language compilation, Wasm or ROS execution',
            'backend_qualified_declarations_checked':sum(r['functions'] for r in results),
            'bindings':results,'C_backend_tables':tables}


def main():
    """Emit an optional source-only report in a caller-selected output path."""
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--report',type=Path)
    args=parser.parse_args();result=check()
    if args.report:
        args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(result,indent=2)+'\n')
    print('PASS complete source declarations: '+str(result['backend_qualified_declarations_checked'])+' across ten backend/language sets')


if __name__=='__main__':
    main()
