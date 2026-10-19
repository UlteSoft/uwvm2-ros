#!/usr/bin/env python3
"""Run bounded source and pure-model SDK checks, without compiling a guest.

Run this tool from a complete uwvm2 source checkout. The ABI extractor needs
the actual Runtime registry outside sdk/. JavaScript tests use in-memory fake
bridges and codecs; they never instantiate a guest or call a ROS host. This
tool does not start a Worker, Gateway, compiler, package installer or network
client. A successful report is deliberately named SOURCE_AND_MODEL_PASS.

Use --report with a new path to preserve the exact checked source hashes and
raw child output. Existing reports are never overwritten. Guest compilation,
full Wasm validation and real ROS/cgroup acceptance are separate checks.
"""
import argparse
import ast
import datetime
import hashlib
import json
import math
import os
import stat
from pathlib import Path
import re
import shutil
import subprocess
import sys
from urllib.parse import unquote, urlsplit

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
SDK = ROOT / 'sdk'

# These five translation units consume external, explicitly generated build
# inputs. The complete source checker records their literal producer contracts;
# it does not claim the generated headers exist or have compiled. An unrelated
# missing include, even one with the same basename, remains an error.
GENERATED_INCLUDES = {
    ('sdk/integrations/unitree/native_identity.cpp', 'selection.hpp'):
        ('sdk/integrations/unitree/prepare.py', 'prepare',
         'sdk/integrations/unitree/CMakeLists.txt'),
    **{('sdk/robots/unitree/examples/' + name, header):
       ('sdk/custom-types/bounded_profile.py', 'generate',
        'sdk/robots/unitree/recipes/six-language-codecs.json')
       for name, header in (
           ('direct_go2.cpp', 'bounded_records.hpp'),
           ('direct_hold.cpp', 'bounded_records.hpp'),
           ('passive_command.c', 'bounded_records.h'),
           ('passive_command.cpp', 'bounded_records.hpp'))},
}

# Exactly these seven NEW typed examples consume whole externally generated
# family modules. Their original filenames/relative references stay intact;
# the closed source recipe records the owned build-work staging operation.
# This table does not admit another source with the same missing basename.
TYPED_GENERATED_RECIPE = 'sdk/robots/unitree/recipes/closedtyped-example-recipes.json'
TYPED_GENERATED_REFERENCES = {
    ('sdk/robots/unitree/examples/topic_rpc.c', 'bounded_records.h'): 'bounded_records.h',
    ('sdk/robots/unitree/examples/topic_rpc.cpp', 'bounded_records.hpp'): 'bounded_records.hpp',
    **{('sdk/robots/unitree/examples/javascript/' + name + '.mjs', './bounded_records.mjs'):
       'bounded_records.mjs' for name in ('lowstate', 'topic_rpc')},
    **{('sdk/robots/unitree/examples/assemblyscript/' + name + '.ts',
        '../../../../custom-types/typed-family/bounded_records'): 'bounded_records.ts'
       for name in ('lowstate_go', 'lowstate_hg', 'topic_rpc')},
}

# Additional actual build routes are kept separate from the original five
# includes/seven typed references, so an older exception cannot grow by basename.
ADDITIONAL_GENERATED_RECIPE = 'sdk/tools/generated-reference-contracts.json'
ADDITIONAL_GENERATED_ROUTES = {
    'custom-rpc': ('sdk/custom-types/interface-examples/prepare.py', 'prepare',
                   'sdk/custom-types/interface-examples/build-recipes.json'),
    'direct-go2': ('tests/ros_unitree/prepare_direct_guests.py', 'build',
                   'tests/ros_unitree/prepare_direct_guests.py'),
    'legacy-lowlevel': ('tests/ros_unitree/prepare_legacy_guests.py', 'stage_product',
                        'sdk/robots/unitree/recipes/legacy-command-recipes.json'),
}
ADDITIONAL_GENERATED_REFERENCES = {
    ('sdk/custom-types/interface-examples/example.mjs', '../generated-rpc/bounded_interfaces.mjs'): ('bounded_interfaces.mjs', 'custom-rpc'),
    ('sdk/custom-types/interface-examples/example.ts', '../generated-rpc/bounded_interfaces'): ('bounded_interfaces.ts', 'custom-rpc'),
    ('sdk/custom-types/interface-examples/example.ts', '../generated-rpc/bounded_records'): ('bounded_records.ts', 'custom-rpc'),
    ('sdk/robots/unitree/examples/direct_go2.c', 'bounded_records.h'): ('bounded_records.h', 'direct-go2'),
    ('sdk/robots/unitree/examples/javascript/direct_go2.mjs', './bounded_records.mjs'): ('bounded_records.mjs', 'direct-go2'),
    ('sdk/robots/unitree/examples/legacy/lowlevel.cpp', 'bounded_records.hpp'): ('bounded_records.hpp', 'legacy-lowlevel'),
    **{('sdk/robots/unitree/examples/legacy/' + name + '.mjs', './bounded_records.mjs'):
       ('bounded_records.mjs', 'legacy-lowlevel') for name in ('lowlevel', 'values')},
    **{('sdk/robots/unitree/examples/legacy/' + name + '.ts', '../../../../custom-types/legacy-family/bounded_records'):
       ('bounded_records.ts', 'legacy-lowlevel') for name in ('lowlevel', 'values')},
    ('sdk/robots/unitree/examples/legacy/values_c.h', 'bounded_records.h'): ('bounded_records.h', 'legacy-lowlevel'),
}


# The records-only operator is a separate closed route. It cannot expand the
# old eleven route exceptions or nominate an arbitrary source/output/producer.
CONTROL_GENERATED_REFERENCES = {
    ('sdk/assemblyscript/ros2/rmw_service/examples/control_client.ts', '../../../../custom-types/generated-control-managed/bounded_records'):
        ('bounded_records.ts', 'relative-module', 'sdk/custom-types/generated-control-managed/bounded_records.ts'),
    ('tests/ros_original_control_managed/codec_contract.mjs', '../../sdk/custom-types/generated-control-managed/bounded_records.mjs'):
        ('bounded_records.mjs', 'relative-module', 'sdk/custom-types/generated-control-managed/bounded_records.mjs'),
    ('tests/ros_original_control_managed/codec_contract.ts', '../../sdk/custom-types/generated-control-managed/bounded_records'):
        ('bounded_records.ts', 'relative-module', 'sdk/custom-types/generated-control-managed/bounded_records.ts'),
    ('tests/ros_original_control_managed/rust/contract.rs', '../../../sdk/custom-types/generated-control-managed/bounded_records.rs'):
        ('bounded_records.rs', 'rust-path', 'sdk/custom-types/generated-control-managed/bounded_records.rs'),
    ('tests/ros_original_control_managed/go/codec_contract.go', 'example.local/control-record-contracts/customtypes'):
        ('bounded_records.go', 'go-module', 'tests/ros_original_control_managed/go/customtypes/bounded_records.go'),
    ('sdk/rust/owned_service/examples/control_bindings.rs', '../../../custom-types/generated-control-managed/bounded_records.rs'):
        ('bounded_records.rs', 'rust-path', 'sdk/custom-types/generated-control-managed/bounded_records.rs'),
    ('tests/ros_javascript_owned_service/model_contract.mjs', '../../sdk/custom-types/generated-control-managed/bounded_records.mjs'):
        ('bounded_records.mjs', 'relative-module', 'sdk/custom-types/generated-control-managed/bounded_records.mjs'),
}
CONTROL_GENERATED_PRODUCERS = {
    'sdk/tools/custom_types.py': ('main',),
    'sdk/custom-types/bounded_profile.py': ('generate', 'generate_control', 'control_managed_products',
        'emit_control_rust', 'emit_control_js', 'emit_control_as', 'emit_control_go'),
    'sdk/rmw_wasm/default_control/managed/generate_managed.py': ('run', 'main'),
}
CONTROL_RECORD_PRODUCTS = {'bounded_records.rs': 'emit_control_rust', 'bounded_records.mjs': 'emit_control_js',
                           'bounded_records.ts': 'emit_control_as', 'bounded_records.go': 'emit_control_go'}
CONTROL_SOURCE_RECIPE = 'sdk/rmw_wasm/default_control/managed/recipes.json'
CONTROL_CATALOG = 'sdk/rmw_wasm/default_control/catalog.preparation.json'
CONTROL_GO_MODULE = 'tests/ros_original_control_managed/go/go.mod'
CONTROL_CANONICAL_COMMAND = ['${PYTHON}', '${SOURCE_ROOT}/sdk/tools/custom_types.py', '${CATALOG}',
                             '--guest-only', '--out', '${FRESH_CANONICAL}']
CONTROL_MANAGED_COMMAND = ['${PYTHON}', '${SOURCE_ROOT}/sdk/rmw_wasm/default_control/managed/generate_managed.py',
    '--source-root', '${SOURCE_ROOT}', '--catalog', '${CATALOG}', '--canonical', '${FRESH_CANONICAL}',
    '--parent-sha256', '${ACTUAL_PARENT_SHA}', '--bounded-sha256', '${ACTUAL_BOUNDED_SHA}',
    '--out', '${STAGED_ROOT}/sdk/custom-types/generated-control-managed', '--records-only']
CONTROL_CANONICAL_PRODUCTS = ['bounded_records.h', 'bounded_records.hpp', 'bounded_interfaces.h',
                             'bounded_interfaces.hpp', 'control_wire.hpp', 'control_manifest.hpp', 'control-schema.json']


def generated_contract_read(path):
    """Capture one bounded ordinary FD; no blocking FIFO open or symlink read.

    This admits source DATA only, not an executable module or a generated
    product. A terminal reopen below compares all captured bytes again.
    """
    limit = 2 * 1024 * 1024
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or first.st_size > limit:
            raise ValueError('bounded ordinary generated-contract source required')
        chunks, count = [], 0
        while True:
            part = os.read(fd, min(65536, limit - count + 1))
            if not part:
                break
            count += len(part)
            if count > limit:
                raise ValueError('generated-contract source grew beyond bound')
            chunks.append(part)
        final, named = os.fstat(fd), os.lstat(path)
        def identity(s):
            return (s.st_dev, s.st_ino, s.st_mode, s.st_size, s.st_mtime_ns, s.st_ctime_ns, s.st_nlink)
        if identity(first) != identity(final) or identity(first) != identity(named) or count != first.st_size:
            raise ValueError('generated-contract source FD/name/extent changed')
        return b''.join(chunks)
    finally:
        os.close(fd)


def generated_contract_json(raw):
    def unique(rows):
        result = {}
        for key, value in rows:
            if key in result:
                raise ValueError('duplicate generated-contract member')
            result[key] = value
        return result
    def finite_float(text):
        value = float(text)
        if not math.isfinite(value):
            raise ValueError('nonfinite generated contract exponent')
        return value
    return json.loads(raw, object_pairs_hook=unique, parse_float=finite_float,
        parse_constant=lambda _: (_ for _ in ()).throw(ValueError('nonfinite generated contract')))


def generated_contract_content(raw):
    return {'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}


def generated_function_source(raw, name):
    source = raw.decode('utf-8')
    nodes = [node for node in ast.parse(source).body if isinstance(node, ast.FunctionDef) and node.name == name]
    if len(nodes) != 1:
        raise ValueError('one actual producer function required')
    return ast.get_source_segment(source, nodes[0]).encode('utf-8')


def literal_control_references(source, syntax):
    """Inspect actual literal source syntax, never resolve a compiler/module ABI."""
    if syntax == 'relative-module':
        return re.findall(r'''(?:from\s+|import\s*)['"](\.[^'"]+)['"]''', source)
    if syntax == 'rust-path':
        return re.findall(r'''#\s*\[\s*path\s*=\s*"([^"]+)"\s*\]\s*(?:pub\s+)?mod\s+[A-Za-z_][A-Za-z0-9_]*\s*;''', source)
    if syntax == 'go-module':
        blocks = re.findall(r'\bimport\s*\((.*?)\)', source, re.DOTALL)
        blocks += re.findall(r'\bimport\s+((?:[A-Za-z_][A-Za-z0-9_]*\s+)?"[^"\n]+")', source)
        return [value for block in blocks for value in re.findall(r'"([^"\n]+)"', block)]
    raise ValueError('unknown generated source syntax')


def control_record_product_chain(bounded_raw, operator_raw):
    """Require the actual closed record dictionary/call, not comment filenames."""
    source = generated_function_source(bounded_raw, 'control_managed_products').decode()
    function = ast.parse(source).body[0]
    dictionaries = [node.value for node in function.body if isinstance(node, ast.Assign) and
                    any(isinstance(t, ast.Name) and t.id == 'products' for t in node.targets) and isinstance(node.value, ast.Dict)]
    if len(dictionaries) != 1:
        return False
    table = dictionaries[0]
    if len(table.keys) != 4:
        return False
    found = {}
    for key, value in zip(table.keys, table.values):
        if not isinstance(key, ast.Constant) or not isinstance(value, ast.Call) or not isinstance(value.func, ast.Name):
            return False
        if [ast.dump(x, include_attributes=False) for x in value.args] != [ast.dump(ast.Name(id=n, ctx=ast.Load()), include_attributes=False) for n in ('parent', 'records', 'ordered')] or value.keywords:
            return False
        found[key.value] = value.func.id
    if found != CONTROL_RECORD_PRODUCTS:
        return False
    run = ast.parse(generated_function_source(operator_raw, 'run').decode()).body[0]
    wanted = ast.dump(ast.parse('bounded.control_managed_products(parent,records,ordered,interfaces,pins)', mode='eval').body, include_attributes=False)
    return sum(isinstance(node, ast.Assign) and isinstance(node.targets[0], ast.Tuple) and
        [x.id for x in node.targets[0].elts if isinstance(x, ast.Name)] == ['products', 'proof'] and
        ast.dump(node.value, include_attributes=False) == wanted for node in run.body) == 1


def declared_control_generated_input(relative, target):
    """Return a declaration for one exact current records-only consumer.

    All producer/catalog/definition/recipe/source bytes are captured, checked and
    reopened. No producer is imported/evaluated and no proof/output is made.
    This neither admits a Service transport nor changes any import permission.
    """
    selected = CONTROL_GENERATED_REFERENCES.get((relative, target))
    if selected is None:
        return None
    captured = {}
    def read(name):
        if name not in captured:
            captured[name] = generated_contract_read(ROOT / name)
        return captured[name]
    def pinned(row):
        if type(row) is not dict or set(row) != {'source', 'content'} or type(row['source']) is not str:
            raise ValueError('exact source content row required')
        if row['content'] != generated_contract_content(read(row['source'])):
            raise ValueError('generated contract source differs from reviewed bytes')
    try:
        whole = generated_contract_json(read(ADDITIONAL_GENERATED_RECIPE))
        if whole['format'] != 'uwvm-sdk-exact-generated-reference-contracts-v1' or whole['status'] != 'SOURCE_DECLARATIONS_NOT_MATERIALIZED_OR_COMPILED' or whole['default_authority'] != []:
            return None
        contract = whole['control_managed']
        if type(contract) is not dict or set(contract) != {'format', 'status', 'default_authority', 'profile', 'producers', 'recipe', 'catalog', 'products', 'proof_product', 'canonical_proof', 'canonical_products', 'support_sources', 'references', 'qualification'}:
            return None
        if contract['format'] != 'uwvm-sdk-exact-control-managed-record-references-v1' or contract['status'] != whole['status'] or contract['default_authority'] != [] or contract['profile'] != 'uwvm-ros-original-node-control-c-cpp-v1':
            return None
        if contract['products'] != CONTROL_RECORD_PRODUCTS or contract['proof_product'] != 'generation.control-managed.json' or contract['canonical_proof'] != 'generation.json' or contract['canonical_products'] != CONTROL_CANONICAL_PRODUCTS:
            return None
        rows = contract['producers']
        if type(rows) is not list or len(rows) != 3 or {x['source'] for x in rows} != set(CONTROL_GENERATED_PRODUCERS):
            return None
        for row in rows:
            if set(row) != {'source', 'content', 'functions'} or set(row['functions']) != set(CONTROL_GENERATED_PRODUCERS[row['source']]):
                return None
            raw = read(row['source'])
            if generated_contract_content(raw) != row['content'] or any(hashlib.sha256(generated_function_source(raw, name)).hexdigest() != value for name, value in row['functions'].items()):
                return None
        if not control_record_product_chain(read('sdk/custom-types/bounded_profile.py'), read('sdk/rmw_wasm/default_control/managed/generate_managed.py')):
            return None
        recipe = contract['recipe']
        if set(recipe) != {'source', 'content', 'canonical_command', 'managed_command'} or recipe['source'] != CONTROL_SOURCE_RECIPE or recipe['content'] != generated_contract_content(read(CONTROL_SOURCE_RECIPE)):
            return None
        actual_recipe = generated_contract_json(read(CONTROL_SOURCE_RECIPE))
        if actual_recipe['format'] != 'uwvm-original-control-managed-source-recipe-v1' or actual_recipe['implicit_authority'] != [] or recipe['canonical_command'] != CONTROL_CANONICAL_COMMAND or recipe['managed_command'] != CONTROL_MANAGED_COMMAND or actual_recipe['canonical_command'] != recipe['canonical_command'] or actual_recipe['managed_command'] != recipe['managed_command']:
            return None
        catalog = contract['catalog']
        if set(catalog) != {'source', 'content', 'definitions'} or catalog['source'] != CONTROL_CATALOG or catalog['content'] != generated_contract_content(read(CONTROL_CATALOG)):
            return None
        actual_catalog = generated_contract_json(read(CONTROL_CATALOG))
        if set(actual_catalog) != {'format', 'types'} or actual_catalog['format'] != contract['profile'] or type(actual_catalog['types']) is not list or len(actual_catalog['types']) != 25:
            return None
        names = [row['file'] for row in actual_catalog['types']]
        if len(set(names)) != 25 or any(type(name) is not str or not name.startswith('original/') or '..' in Path(name).parts or '\\' in name for name in names):
            return None
        expected = {str(Path(CONTROL_CATALOG).parent / name) for name in names}
        definitions = catalog['definitions']
        if type(definitions) is not list or len(definitions) != 25 or {row['source'] for row in definitions} != expected:
            return None
        for row in definitions:
            pinned(row)
        support = contract['support_sources']
        if type(support) is not list or len(support) != 1 or support[0]['source'] != CONTROL_GO_MODULE:
            return None
        pinned(support[0])
        if not re.search(r'^module example\.local/control-record-contracts\s*$', read(CONTROL_GO_MODULE).decode(), re.MULTILINE):
            return None
        references = contract['references']
        if type(references) is not list or len(references) != len(CONTROL_GENERATED_REFERENCES):
            return None
        found = set()
        for row in references:
            if type(row) is not dict or set(row) != {'source', 'target', 'output', 'syntax', 'staged_product', 'source_content'}:
                return None
            key = (row['source'], row['target'])
            if key in found or key not in CONTROL_GENERATED_REFERENCES or (row['output'], row['syntax'], row['staged_product']) != CONTROL_GENERATED_REFERENCES[key]:
                return None
            found.add(key)
            if row['source_content'] != generated_contract_content(read(row['source'])) or literal_control_references(read(row['source']).decode('utf-8'), row['syntax']).count(row['target']) != 1:
                return None
        if found != set(CONTROL_GENERATED_REFERENCES):
            return None
        for name, raw in captured.items():
            if generated_contract_read(ROOT / name) != raw:
                return None
    except (OSError, UnicodeError, SyntaxError, ValueError, TypeError, KeyError, IndexError, RecursionError):
        return None
    output, syntax, staged = selected
    return {'source': relative, 'target': target, 'generated_output': output, 'staged_product': staged,
            'generator': 'sdk/rmw_wasm/default_control/managed/generate_managed.py', 'generator_function': 'run',
            'bounded_generator': 'sdk/custom-types/bounded_profile.py', 'bounded_function': 'control_managed_products',
            'actual_producer_content': {row['source']: row['content'] for row in contract['producers']},
            'build_recipe': CONTROL_SOURCE_RECIPE, 'reference_contract': ADDITIONAL_GENERATED_RECIPE,
            'canonical_proof_required_separately': 'generation.json', 'managed_proof_required_separately': 'generation.control-managed.json',
            'status': 'DECLARED_GENERATED_BUILD_INPUT', 'generated_input_materialized': False,
            'generated_header_materialized': False, 'actual_compilation': 'REQUIRED_SEPARATELY',
            'Native_qualification': 'REQUIRED_SEPARATELY', 'implicit_authority': []}


def declares_managed_product(path, filename):
    """Recognize the real closed conditional facade emitter without running it.

    Service/Action-only outputs use the literal four-extension loop rather than
    a literal products dictionary key. Require its actual call/update chain,
    that exact loop and assignment, and the real selected template. A filename
    in a comment, unrelated function or another producer cannot satisfy this.
    """
    if filename not in {'bounded_interfaces.' + suffix for suffix in ('rs', 'mjs', 'go', 'ts')}:
        return False
    try:
        tree = ast.parse(path.read_text())
        generate = next(node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == 'generate')
        emitter = next(node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == 'emit_managed_interfaces')
        call = ast.dump(ast.parse('emit_managed_interfaces(parent, interfaces)', mode='eval').body, include_attributes=False)
        update = ast.dump(ast.parse('products.update(facade_products)', mode='eval').body, include_attributes=False)
        called = any(isinstance(node, ast.Assign) and ast.dump(node.value, include_attributes=False) == call and
                     isinstance(node.targets[0], ast.Tuple) and
                     [target.id for target in node.targets[0].elts if isinstance(target, ast.Name)] == ['facade_products', 'facade_templates']
                     for node in generate.body)
        updated = any(isinstance(node, ast.Expr) and ast.dump(node.value, include_attributes=False) == update for node in generate.body)
        loops = [node for node in emitter.body if isinstance(node, ast.For) and isinstance(node.target, ast.Name) and node.target.id == 'extension']
        if len(loops) != 1 or ast.literal_eval(loops[0].iter) != ('rs', 'mjs', 'go', 'ts'):
            return False
        expected = ast.dump(ast.parse("products['bounded_interfaces.' + extension] = body").body[0], include_attributes=False)
        assigned = any(ast.dump(node, include_attributes=False) == expected for node in loops[0].body)
        template = path.parent / 'interface_templates' / (filename + '.in')
        return called and updated and assigned and template.is_file() and template.read_text().count('@@INTERFACES@@') == 1
    except (OSError, UnicodeError, SyntaxError, ValueError, TypeError, StopIteration, IndexError):
        return False


def declared_additional_generated_input(relative, target):
    """Admit one exact reviewed source reference with current recipe/body pins.

    These are source declarations, never real generated artifacts. The closed
    contract binds the actual maintained staging function and full compiler
    recipe bytes. Changed recipes or producer bodies require explicit review
    and rebinding; ancestor paths and matching basenames provide no authority.
    """
    selected = ADDITIONAL_GENERATED_REFERENCES.get((relative, target))
    if selected is None:
        return None
    def unique(rows):
        result = {}
        for key, value in rows:
            if key in result:
                raise ValueError('duplicate generated reference contract member')
            result[key] = value
        return result
    def content(path):
        raw = path.read_bytes()
        return {'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}
    try:
        contract = json.loads((ROOT / ADDITIONAL_GENERATED_RECIPE).read_text(), object_pairs_hook=unique,
            parse_constant=lambda value: (_ for _ in ()).throw(ValueError('nonfinite generated reference contract')))
        if contract['format'] != 'uwvm-sdk-exact-generated-reference-contracts-v1' or contract['default_authority'] != []:
            return None
        generator = contract['generator']
        if set(generator) != {'producer', 'function', 'content'} or generator['producer'] != 'sdk/custom-types/bounded_profile.py' or generator['function'] != 'generate':
            return None
        path = ROOT / generator['producer']
        if content(path) != generator['content'] or set(contract['routes']) != set(ADDITIONAL_GENERATED_ROUTES):
            return None
        rows = contract['references']
        expected = [{'source': source, 'target': name, 'output': output, 'route': route}
                    for (source, name), (output, route) in ADDITIONAL_GENERATED_REFERENCES.items()]
        if type(rows) is not list or len(rows) != 11 or any(type(row) is not dict or set(row) != {'source', 'target', 'output', 'route'} for row in rows) or \
                any(rows.count(row) != 1 for row in expected) or any(row not in expected for row in rows):
            return None
        for route, (producer, function, recipe) in ADDITIONAL_GENERATED_ROUTES.items():
            proof = contract['routes'][route]
            if set(proof) != {'producer', 'function', 'function_source_sha256', 'recipe', 'recipe_content'} or \
                    (proof['producer'], proof['function'], proof['recipe']) != (producer, function, recipe) or content(ROOT / recipe) != proof['recipe_content']:
                return None
            source = (ROOT / producer).read_text()
            syntax = ast.parse(source)
            body = next(node for node in syntax.body if isinstance(node, ast.FunctionDef) and node.name == function)
            if hashlib.sha256(ast.get_source_segment(source, body).encode()).hexdigest() != proof['function_source_sha256']:
                return None
        output, route = selected
        if not declares_generated_product(path, 'generate', output) and not declares_managed_product(path, output):
            return None
    except (OSError, UnicodeError, SyntaxError, ValueError, TypeError, KeyError, StopIteration):
        return None
    return {'source': relative, 'target': target, 'generated_output': output,
            'generator': generator['producer'], 'generator_function': 'generate',
            'build_recipe': contract['routes'][route]['recipe'],
            'staging_producer': contract['routes'][route]['producer'],
            'staging_function': contract['routes'][route]['function'],
            'reference_contract': ADDITIONAL_GENERATED_RECIPE, 'status': 'DECLARED_GENERATED_BUILD_INPUT',
            'generated_header_materialized': False, 'generated_input_materialized': False,
            'actual_compilation': 'REQUIRED_SEPARATELY'}


def declares_generated_product(path, function, filename):
    """Inspect literal product tables without evaluating a producer's code.

    The maintained producers assign ``products`` either a dictionary of output
    names or a list of (name, bytes) pairs. Dynamic keys and literals in comments
    or unrelated functions do not declare a generated product.
    """
    try:
        tree = ast.parse(path.read_text())
    except (OSError, UnicodeError, SyntaxError):
        return False
    for node in tree.body:
        if not isinstance(node, ast.FunctionDef) or node.name != function:
            continue
        pending = list(node.body)
        while pending:
            statement = pending.pop()
            # A nested helper's output table is not this producer's table.
            if isinstance(statement, (ast.FunctionDef, ast.AsyncFunctionDef,
                                      ast.ClassDef, ast.Lambda)):
                continue
            pending.extend(ast.iter_child_nodes(statement))
            if not isinstance(statement, ast.Assign) or not any(
                    isinstance(target, ast.Name) and target.id == 'products'
                    for target in statement.targets):
                continue
            value = statement.value
            keys = value.keys if isinstance(value, ast.Dict) else [
                row.elts[0] for row in value.elts
                if isinstance(row, (ast.List, ast.Tuple)) and len(row.elts) == 2
            ] if isinstance(value, (ast.List, ast.Tuple)) else []
            if any(isinstance(key, ast.Constant) and key.value == filename for key in keys):
                return True
    return False


def declared_generated_include(relative, filename):
    """Return an explicit source-only dependency record, or reject an unknown pair."""
    typed = declared_typed_generated_input(relative, filename)
    if typed is not None:
        return typed
    additional = declared_additional_generated_input(relative, filename)
    if additional is not None:
        return additional
    control = declared_control_generated_input(relative, filename)
    if control is not None:
        return control
    selection = GENERATED_INCLUDES.get((relative, filename))
    if selection is None:
        return None
    producer, function, recipe = selection
    if not (ROOT / recipe).is_file() or not declares_generated_product(
            ROOT / producer, function, filename):
        return None
    return {'source': relative, 'target': filename, 'generator': producer,
            'generator_function': function, 'build_recipe': recipe,
            'status': 'DECLARED_GENERATED_BUILD_INPUT',
            'generated_header_materialized': False,
            'actual_compilation': 'REQUIRED_SEPARATELY'}


def declared_typed_generated_input(relative, target):
    """Check an exact new source/reference, real recipe row and literal output.

    The data-only recipe is not executed and cannot nominate another producer
    function or wildcard path. The actual generated module remains absent until
    a separately owned native-bound generation/staging/compile job proves it.
    """
    output = TYPED_GENERATED_REFERENCES.get((relative, target))
    if output is None:
        return None
    def unique(rows):
        result = {}
        for key, value in rows:
            if key in result:
                raise ValueError('duplicate generated recipe member')
            result[key] = value
        return result
    try:
        recipe = json.loads((ROOT / TYPED_GENERATED_RECIPE).read_text(),
            object_pairs_hook=unique,
            parse_constant=lambda value: (_ for _ in ()).throw(ValueError('nonfinite recipe')))
        selection = recipe['generated_build_inputs']
        if recipe['format'] != 'uwvm-unitree-typed-example-recipes-v1' or recipe['default_authority'] != [] or \
                selection['producer'] != 'sdk/custom-types/bounded_profile.py' or selection['function'] != 'generate':
            return None
        rows = selection['references']
        expected = {'source': relative, 'target': target, 'output': output}
        if type(rows) is not list or len(rows) != 7 or rows.count(expected) != 1 or \
                any(type(row) is not dict or set(row) != {'source', 'target', 'output'} for row in rows) or \
                {(row['source'], row['target']): row['output'] for row in rows} != TYPED_GENERATED_REFERENCES:
            return None
        if not declares_generated_product(ROOT / selection['producer'], selection['function'], output):
            return None
    except (OSError, UnicodeError, ValueError, TypeError, KeyError):
        return None
    return {'source': relative, 'target': target, 'generator': selection['producer'],
            'generator_function': selection['function'], 'generated_output': output,
            'build_recipe': TYPED_GENERATED_RECIPE, 'status': 'DECLARED_GENERATED_BUILD_INPUT',
            'generated_header_materialized': False, 'generated_input_materialized': False,
            'actual_compilation': 'REQUIRED_SEPARATELY'}


def source_snapshot():
    """Hash the SDK's checked files, excluding generated verification reports.

    Historical language reports remain inputs. This aggregate report lives
    outside the hash set to avoid self-reference. Build caches are not sources.
    The Runtime registry is an additional explicit input, not a guessed ABI.
    """
    files = [p for p in SDK.rglob('*') if p.is_file()
             and not any(part in {'target', 'node_modules', '__pycache__', '.git', 'build'}
                         for part in p.relative_to(SDK).parts)
             and not p.name.startswith('verification-')]
    # The two maintained external staging bodies now participate in the exact
    # declaration contract; preserve their whole bytes with the SDK snapshot.
    files.extend(ROOT / relative for relative in (
        'src/uwvm2/uwvm/imported/ros/host.h',
        'tests/ros_unitree/prepare_direct_guests.py',
        'tests/ros_unitree/prepare_legacy_guests.py'))
    # Exact outside-sdk consumers/support also belong to the captured source
    # snapshot. A declaration is never an untracked missing-import exception.
    files.extend(ROOT / relative for relative in sorted({source for source, _ in CONTROL_GENERATED_REFERENCES}
                 | {CONTROL_GO_MODULE}) if not relative.startswith('sdk/'))
    return [{'path': str(p.relative_to(ROOT)), 'bytes': p.stat().st_size,
             'sha256': hashlib.sha256(p.read_bytes()).hexdigest()}
            for p in sorted(files)]


def documentation_links():
    """Resolve actual relative Markdown links, including directory entry points.

    Absolute web/app links and local anchors do not designate source files.
    Fenced code is omitted so example Markdown strings are not treated as links.
    This checks existence; it does not attest English API documentation quality.
    """
    checked = []
    missing = []
    for path in sorted(SDK.rglob('*.md')):
        body = re.sub(r'^```[^\n]*\n.*?^```[^\n]*$', '', path.read_text(),
                      flags=re.MULTILINE | re.DOTALL)
        for target in re.findall(r'\[[^\]\n]+\]\(([^\)\n]+)\)', body):
            target = target.strip().strip('<>')
            parsed = urlsplit(target)
            if parsed.scheme or not parsed.path or parsed.path.startswith('/'):
                continue
            resolved = (path.parent / unquote(parsed.path)).resolve()
            row = {'source': str(path.relative_to(ROOT)), 'target': target,
                   'exists': resolved.exists()}
            checked.append(row)
            if not row['exists']:
                missing.append(row)
    return {'status': 'PASS' if not missing else 'FAIL',
            'links_checked': len(checked), 'missing': missing}


def relative_source_links():
    """Check relative JS/AS modules and quoted C/C++ forwarding includes.

    Extensionless AssemblyScript paths may resolve to .ts or index.ts. System
    includes, Cargo resolution and compiler-specific module rules are outside
    this existence check; actual language compilation remains necessary.
    Explicit generated includes are reported separately with their literal
    producer and build contract, never reported as an existing source file.
    """
    checked = 0
    missing = []
    generated = []
    sources = {path for path in SDK.rglob('*') if path.is_file()}
    sources.update(ROOT / source for source, _ in CONTROL_GENERATED_REFERENCES
                   if not source.startswith('sdk/') and (ROOT / source).is_file())
    for path in sorted(sources):
        if path.suffix in {'.mjs', '.ts'}:
            names = re.findall(r'''(?:from\s+|import\s*)['"](\.[^'"]+)['"]''',
                               path.read_text())
            for name in names:
                target = path.parent / name
                checked += 1
                if not any(p.exists() for p in (target, target.with_suffix('.ts'),
                                                target.with_suffix('.mjs'), target / 'index.ts')):
                    relative = str(path.relative_to(ROOT))
                    declared = declared_typed_generated_input(relative, name)
                    if declared is None:
                        declared = declared_additional_generated_input(relative, name)
                    if declared is None:
                        declared = declared_control_generated_input(relative, name)
                    if declared is None:
                        missing.append({'source': str(path.relative_to(ROOT)), 'target': name})
                    else:
                        generated.append(declared)
        elif path.suffix in {'.rs', '.go'}:
            # This narrow source check knows only these exact generated path /
            # Go local-package consumers. General Cargo/Go resolution is left
            # to the actual compiler; no basename or module-prefix fallback.
            relative = str(path.relative_to(ROOT))
            selections = [value for (source, _), value in CONTROL_GENERATED_REFERENCES.items() if source == relative]
            if selections:
                syntax = selections[0][1]
                names = literal_control_references(path.read_text(), syntax)
                if syntax == 'go-module':
                    # Only this real local module's imports are filesystem
                    # references here. Standard/external Go packages still need
                    # the actual Go toolchain, never a prefix allowlist.
                    names = [name for name in names if name.startswith('example.local/control-record-contracts/')]
                for name in names:
                    checked += 1
                    if syntax == 'rust-path':
                        exists = (path.parent / name).is_file()
                    else:
                        tail = name[len('example.local/control-record-contracts/'):]
                        exists = (path.parent / tail).is_dir()
                    if not exists:
                        declared = declared_control_generated_input(relative, name)
                        if declared is None:
                            missing.append({'source': relative, 'target': name})
                        else:
                            generated.append(declared)
        elif path.suffix in {'.c', '.h', '.cpp', '.hpp'}:
            for name in re.findall(r'^\s*#include\s+"([^"]+)"', path.read_text(), re.MULTILINE):
                checked += 1
                # Repository-root quoted includes are used by the real policy
                # fixture's explicit -I root recipe as well as local includes.
                if not (path.parent / name).exists() and not (ROOT / name).is_file():
                    relative = str(path.relative_to(ROOT))
                    declared = declared_generated_include(relative, name)
                    if declared is None:
                        missing.append({'source': relative, 'target': name})
                    else:
                        generated.append(declared)
    return {'status': 'PASS' if not missing else 'FAIL',
            'references_checked': checked, 'missing': missing,
            'declared_generated_build_inputs': generated}


def run_check(name, arguments):
    """Capture one bounded pure check, preserving failures and both raw streams.

    Arguments are an exact list passed without a shell. A missing executable
    is BLOCKED rather than a successful skipped test. No compiler is in the
    fixed check list, and caller input cannot insert additional commands.
    """
    try:
        result = subprocess.run(arguments, cwd=ROOT, capture_output=True,
                                text=True, timeout=45)
        return {'name': name, 'status': 'PASS' if result.returncode == 0 else 'FAIL',
                'arguments': arguments, 'exit_code': result.returncode,
                'stdout': result.stdout, 'stderr': result.stderr}
    except (OSError, subprocess.TimeoutExpired) as error:
        return {'name': name, 'status': 'BLOCKED', 'arguments': arguments,
                'reason': str(error)}


def verify(node):
    """Check immutable input bytes with registry, fixture and pure-model tools."""
    before = source_snapshot()
    commands = [
        ('Runtime ABI metadata', [sys.executable, '-B', 'sdk/tools/abi_contract.py']),
        ('Complete language declarations', [sys.executable, '-B', 'sdk/tools/check_bindings.py']),
        ('Canonical specification vectors', [sys.executable, '-B', 'sdk/tools/wire_vectors.py']),
        ('Synthetic binary reader models', [sys.executable, '-B', 'sdk/tests/test_import_inspector.py']),
        ('Bounded artifact descriptor models', [sys.executable, '-B', 'sdk/tests/artifact_read_models.py']),
        ('Rust/TinyGo source contracts', [sys.executable, '-B', 'sdk/rust/tests/check_sources.py']),
        ('Exclusive TinyGo backend constraint models', [sys.executable, '-B', 'sdk/tests/go_build_constraint_models.py']),
        ('Declared generated include contracts', [sys.executable, '-B', 'sdk/tests/generated_source_include_models.py']),
        ('Exact control records source declarations', [sys.executable, '-B', 'sdk/tests/control_generated_reference_models.py']),
        ('C/C++ full ABI and typed recipe contracts', [sys.executable, '-B', 'sdk/tests/matrix_coverage_models.py']),
        ('Embedded JavaScript closed recipe/source contracts', [sys.executable, '-B', 'sdk/tests/javascript_guest_models.py']),
        ('Unitree pure-provider Wasm linker/source contracts', [sys.executable, '-B', 'sdk/tests/unitree_preload_models.py']),
    ]
    for path in ('sdk/javascript/tests/test.mjs',
                 'sdk/javascript/tests/factory_transactions.mjs',
                 'sdk/javascript/tests/executor_contracts.mjs',
                 'sdk/javascript/tests/unified_events_models.mjs',
                 'sdk/javascript/tests/event_dispatch_contracts.mjs',
                 'sdk/javascript/tests/run_pure_wasm_options.mjs',
                 'sdk/javascript/tests/run_pure_wasm_cgroup_models.mjs',
                 'sdk/javascript/tests/run_pure_wasm_artifact_models.mjs'):
        commands.append(('Pure JavaScript ' + Path(path).name, [node, path]))
    checks = [run_check(name, arguments) for name, arguments in commands]
    links = documentation_links()
    source_links = relative_source_links()
    after = source_snapshot()
    stable = before == after
    passed = (stable and links['status'] == 'PASS' and source_links['status'] == 'PASS'
              and all(c['status'] == 'PASS' for c in checks))
    return {'status': 'SOURCE_AND_MODEL_PASS' if passed else 'SOURCE_OR_MODEL_INCOMPLETE',
            'scope': 'Source, specification, handcrafted binary and fake-bridge models only; no compiler, guest, ROS, Gateway or ARM execution',
            'generated_at_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
            'source_stable_during_checks': stable, 'source_files': before,
            'documentation_links': links, 'relative_source_links': source_links, 'checks': checks,
            'actual_language_compilation': 'NOT_EXECUTED',
            'actual_ROS_and_aarch64_behavior': 'NOT_EXECUTED'}


def main():
    """Select an existing Node executable and optionally create a fresh report."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--node', default=shutil.which('node') or 'node',
                        help='Existing Node executable for pure fake-bridge tests; never installed by this tool')
    parser.add_argument('--report', type=Path,
                        help='New report path; an existing file is refused')
    args = parser.parse_args()
    if args.report and args.report.exists():
        parser.error('report already exists; choose a new path')
    report = verify(args.node)
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        with args.report.open('x') as output:
            output.write(json.dumps(report, indent=2) + '\n')
    print(report['status'] + ': ' + str(len(report['source_files'])) + ' source inputs, '
          + str(report['documentation_links']['links_checked']) + ' documentation links')
    for check in report['checks']:
        print(check['status'] + ': ' + check['name'])
    if report['documentation_links']['missing']:
        print(json.dumps(report['documentation_links']['missing'], indent=2))
    if report['relative_source_links']['missing']:
        print(json.dumps(report['relative_source_links']['missing'], indent=2))
    return 0 if report['status'] == 'SOURCE_AND_MODEL_PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
