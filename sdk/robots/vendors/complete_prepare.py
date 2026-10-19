"""Explicit complete-vendor source preparation, separate from bounded-v2.

This trusted operator API writes source files. It never executes a compiler,
ROS process, Runtime, signing tool, or robot command. The caller supplies its
ordinary-file admission function ``read(role, full7)`` and current-source loader
``load(repository_relative_name, private_module_name)``. A Guest cannot select
these functions or paths. Original constants, defaults, fields and licenses
are retained; old custom/standard/legacy generators are not patched or retried.

Call ``prepare`` with an explicit complete profile. After a separately owned
native exporter run, call ``bind_and_generate`` with the actual raw metadata
and owner evidence. Both receipts describe SOURCE ONLY and grant no authority.
"""
from pathlib import Path
import hashlib
import re
import uuid

PROFILE = 'uwvm-ros-vendor-complete-bounded-v1'
PREPARER = 'sdk/robots/vendors/vendor_prepare.py'
ADAPTER = 'sdk/robots/industrial/vendor_profile.py'
PARENT = 'sdk/tools/custom_types.py'
BOUNDED = 'sdk/custom-types/bounded_profile.py'
HELPER = 'tests/ros_custom_types/bounded-v2/native_identity.cc'
TEMPLATES = tuple('sdk/custom-types/interface_templates/bounded_interfaces.' +
                  extension + '.in' for extension in ('rs','mjs','go','ts'))
NATIVE_DOMAIN = (
    'src/uwvm2/imported/ros/ipc/custom_type.h',
    'src/uwvm2/imported/ros/ipc/custom_interface.h',
    'src/uwvm2/imported/ros/ipc/complete_vendor_domain.h',
    'gateways/ros1/custom_topics.h',
    'gateways/ros1/complete_vendor_definition.h',
    'gateways/ros1/custom_services.h',
    'gateways/ros1/custom_actions.h',
    'gateways/ros2/custom_topics.h',
    'gateways/ros2/custom_services.h',
    'gateways/ros2/custom_actions.h',
)
SUPPORT = (PREPARER, ADAPTER, PARENT, BOUNDED, HELPER,
           'tests/ros_custom_types/bounded-v2/native_protocol.h',
           'gateways/common/native_io.h', *TEMPLATES, *NATIVE_DOMAIN)
MAX_TOTAL = 64 * 1024 * 1024
MAX_PREPARATION_FILES = 512
MAX_GENERATED_PRODUCTS = 32
MAX_CATALOG = 2 * 1024 * 1024
MAX_DEFINITION = 1024 * 1024


def need(condition, message):
    if not condition:
        raise ValueError(message)


def ordinary_pin(pin, maximum=16*1024*1024):
    need(type(pin) is dict and set(pin) == {
        'path','bytes','sha256','device','inode','mtime_ns','ctime_ns'} and
        type(pin['path']) is str and Path(pin['path']).is_absolute() and
        type(pin['bytes']) is int and 0 <= pin['bytes'] <= maximum and
        type(pin['sha256']) is str and re.fullmatch('[0-9a-f]{64}',pin['sha256']) and
        all(type(pin[key]) is int and pin[key] >= 0 for key in
            ('device','inode','mtime_ns','ctime_ns')) and pin['inode'] > 0,
        'exact bounded ordinary full7 required')


def modules(request, *, read, load):
    """Admit fixed source roles; execute only the two named source modules.

    The complete adapter privately executes the captured parent/bounded bytes
    with isolated globals. There is no import of an ambient cached generator.
    The loader itself is a trusted operator dependency, never a Guest hook.
    """
    support = request.get('support')
    need(type(support) is dict and set(support) == set(SUPPORT),
         'all exact complete-profile source roles required')
    # Admission is performed before loading even the shared admission helpers.
    raw = {}; total = 0
    for name in SUPPORT:
        pin = support[name]
        ordinary_pin(pin)
        need(pin['bytes'] <= MAX_TOTAL-total, 'finite cumulative source-role budget required')
        total += pin['bytes']
        body = read('complete-source:' + name,pin)
        need(type(body) is bytes and len(body) == pin['bytes'] and
             hashlib.sha256(body).hexdigest() == pin['sha256'],
             'captured source differs: ' + name)
        raw[name] = body
    need(sum(map(len,raw.values())) <= MAX_TOTAL, 'finite source-role budget required')
    suffix = uuid.uuid4().hex  # Module isolation only; never a native identity.
    preparer = load(PREPARER,'_vendor_complete_admission_' + suffix)
    adapter = load(ADAPTER,'_vendor_complete_adapter_' + suffix)
    for name, module in ((PREPARER,preparer),(ADAPTER,adapter)):
        need(Path(module.__file__).resolve(strict=True) == Path(support[name]['path']),
             'loaded current source pathname differs: ' + name)
    need(adapter.PROFILE == PROFILE and hasattr(preparer,'reviewed_inventory'),
         'explicit complete-profile and reviewed inventory implementation required')
    for name in SUPPORT:
        need(preparer.admitted(read,'complete-source-reopen:' + name,support[name]) == raw[name],
             'current source changed after loading: ' + name)
    api = adapter.API(raw[PARENT],raw[BOUNDED],
        parent_path=support[PARENT]['path'], bounded_path=support[BOUNDED]['path'],
        parent_sha256=support[PARENT]['sha256'], bounded_sha256=support[BOUNDED]['sha256'],
        templates={name.removeprefix('sdk/custom-types/'):raw[name] for name in TEMPLATES},
        adapter_raw=raw[ADAPTER], adapter_sha256=support[ADAPTER]['sha256'])
    return preparer, api, raw


def inputs(preparer, inventory, request, *, read):
    """Read whole original definitions and per-repository provenance DATA.

    Blob SHA1 checks associate each copied definition with its captured Git
    object bytes. Commit/tree/capture associations still require independent
    source review; this function does not turn a declared Git hash or capture
    receipt into an upstream trust, installed SDK, or execution attestation.
    """
    need(type(request) is dict and set(request) == {
        'format_version','profile','vendor','model','sdk_revision','ros_version',
        'source_lock','support'}, 'closed explicit complete-profile request required')
    need(type(request['format_version']) is int and request['format_version'] == 1 and
         request['profile'] == PROFILE and type(request['ros_version']) is int and
         request['ros_version'] in (1,2), 'explicit complete profile and ROS major required')
    need(preparer.reviewed_inventory(inventory), 'reviewed inventory DATA required')
    rows = [row for row in inventory['vendors'] if row['id'] == request['vendor']]
    need(len(rows) == 1, 'one individually reviewed vendor required')
    need(not rows[0]['ros' + str(request['ros_version'])].startswith(
        ('NOT_ESTABLISHED','EXPLICITLY_NOT_SUPPORTED')),
        'no source evidence for the selected vendor ROS major')
    for key in ('model','sdk_revision'):
        need(type(request[key]) is str and 1 <= len(request[key]) <= 128 and
             '\0' not in request[key], 'exact separately reviewed model/revision required')
    consumed = 0
    def admitted(role, pin, maximum=16*1024*1024):
        nonlocal consumed
        preparer.ordinary_pin(pin)
        need(pin['bytes'] <= maximum and pin['bytes'] <= MAX_TOTAL-consumed,
             'finite cumulative complete input budget required')
        consumed += pin['bytes']
        return preparer.admitted(read,role,pin,maximum)
    lock_raw = admitted('complete-source-lock',request['source_lock'])
    lock = preparer.strict(lock_raw)
    need(type(lock) is dict and set(lock) == {
        'format_version','kind','vendor','model','sdk_revision','ros_version',
        'repositories','catalog','definitions','source_review','default_authority'} and
        type(lock['format_version']) is int and lock['format_version'] == 1 and
        lock['kind'] == 'vendor-complete-multiple-repository-source-lock-v1' and
        lock['source_review'] == 'CLOSED_SOURCE_REVIEW_NOT_EXECUTION' and
        lock['default_authority'] == [] and all(lock[key] == request[key] for key in
            ('vendor','model','sdk_revision','ros_version')),
        'complete source lock must bind this vendor/model/revision/backend')
    repositories = lock['repositories']
    need(type(repositories) is list and 1 <= len(repositories) <= 64,
         'finite original repository closure required')
    assets = {'source-lock.json':lock_raw}; by_repository = {}; used_repositories = set()
    for row in repositories:
        need(type(row) is dict and set(row) == {
            'id','url','commit','tree_git_sha1','license','capture','source_files'} and
            type(row['id']) is str and re.fullmatch('[a-z][a-z0-9_-]{0,63}',row['id']) and
            row['id'] not in by_repository and type(row['url']) is str and
            row['url'].startswith('https://') and '\0' not in row['url'] and
            all(type(row[key]) is str and re.fullmatch('[0-9a-f]{40}',row[key])
                for key in ('commit','tree_git_sha1')),
            'unique exact original repository/source revision required')
        by_repository[row['id']] = row
        assets['provenance/' + row['id'] + '/LICENSE'] = admitted(
            'complete-license:' + row['id'],row['license'])
        assets['provenance/' + row['id'] + '/capture.json'] = admitted(
            'complete-capture:' + row['id'],row['capture'])
        need(type(row['source_files']) is list and len(row['source_files']) <= 128,
             'finite selected driver/SDK source inventory required')
        for source in row['source_files']:
            need(type(source) is dict and set(source) == {'relative','git_blob_sha1','pin'} and
                 type(source['git_blob_sha1']) is str and re.fullmatch('[0-9a-f]{40}',source['git_blob_sha1']),
                 'closed original driver/SDK source role required')
            name = preparer.relative(source['relative'])
            body = admitted('complete-selected-source:' + row['id'] + ':' + name,source['pin'])
            need(hashlib.sha1(b'blob ' + str(len(body)).encode('ascii') + b'\0' + body).hexdigest() ==
                 source['git_blob_sha1'], 'original selected source Git blob differs')
            target = 'selected-source/' + row['id'] + '/' + name
            need(target not in assets, 'duplicate selected original driver/SDK role')
            assets[target] = body; used_repositories.add(row['id'])
    need(any(row['url'] in rows[0]['official_primary_sources'] for row in repositories),
         'selected vendor official source missing from the transitive closure')
    catalog_raw = admitted('complete-catalog',lock['catalog'],MAX_CATALOG)
    catalog = preparer.strict(catalog_raw)
    need(type(catalog) is dict and set(catalog) == {'format','types'} and
         catalog['format'] == PROFILE and type(catalog['types']) is list and
         1 <= len(catalog['types']) <= 128, 'whole unbound complete-profile catalog required')
    assets['input/catalog.json'] = catalog_raw
    definitions = lock['definitions']
    need(type(definitions) is list and 1 <= len(definitions) <= 128,
         'finite full original definition closure required')
    original_roles = set(); generated_roles = set(); authored_files = set()
    def sdk_source(row):
        need(type(row) is dict and set(row) == {'repository','original_file','git_blob_sha1','pin'} and
             row['repository'] in by_repository and type(row['git_blob_sha1']) is str and
             re.fullmatch('[0-9a-f]{40}',row['git_blob_sha1']), 'complete original SDK source role required')
        original = preparer.relative(row['original_file'])
        body = admitted('bridge-original-sdk:' + row['repository'] + ':' + original,row['pin'])
        need(hashlib.sha1(b'blob ' + str(len(body)).encode('ascii') + b'\0' + body).hexdigest() ==
             row['git_blob_sha1'], 'original SDK Git blob bytes differ')
        used_repositories.add(row['repository'])
        name = 'bridge-sdk/' + row['repository'] + '/' + original
        need(name not in assets or assets[name] == body, 'conflicting original SDK source role')
        assets[name] = body
    for row in definitions:
        need(type(row) is dict and row.get('origin') in ('OFFICIAL_GIT','AUTHORED_BRIDGE'),
             'explicit original Git or independently authored bridge domain required')
        generated = preparer.relative(row['generation_file'])
        need(generated not in generated_roles and
             generated != 'catalog.json' and generated.endswith(('.msg','.srv','.action')),
             'unique complete Message/Service/Action definition required')
        generated_roles.add(generated)
        if row['origin'] == 'OFFICIAL_GIT':
            need(set(row) == {'origin','repository','original_file','generation_file','git_blob_sha1','pin'} and
                 row['repository'] in by_repository and type(row['git_blob_sha1']) is str and
                 re.fullmatch('[0-9a-f]{40}',row['git_blob_sha1']), 'complete official definition role required')
            original = preparer.relative(row['original_file'])
            need((row['repository'],original) not in original_roles, 'duplicate original definition role')
            original_roles.add((row['repository'],original)); used_repositories.add(row['repository'])
            body = admitted('complete-definition:' + row['repository'] + ':' + original,row['pin'],MAX_DEFINITION)
            need(hashlib.sha1(b'blob ' + str(len(body)).encode('ascii') + b'\0' + body).hexdigest() ==
                 row['git_blob_sha1'], 'original definition Git blob bytes differ')
        else:
            need(set(row) == {'origin','generation_file','pin','sdk_sources','mapping'} and
                 type(row['sdk_sources']) is list and 1 <= len(row['sdk_sources']) <= 128,
                 'authored bridge needs its original SDK sources and actual typed mapping source')
            body = admitted('authored-bridge-definition:' + generated,row['pin'],MAX_DEFINITION)
            authored_files.add(generated)
            for source in row['sdk_sources']:
                sdk_source(source)
            assets['bridge-mapping/' + generated + '.source'] = admitted(
                'authored-bridge-mapping:' + generated,row['mapping'])
        assets['input/' + generated] = body
    need(used_repositories == set(by_repository),
         'repository inventory must exactly cover the complete definition closure')
    files = set(); native_names = set()
    for row in catalog['types']:
        need(type(row) is dict and {'ros_version','type','file'} <= set(row) <= {
            'ros_version','type','file','bounds','registered','wire_limits'} and
            type(row['ros_version']) is int and row['ros_version'] == request['ros_version'] and
            type(row['type']) is str and row['type'] not in native_names and
            row['file'] in generated_roles and row['file'] not in files,
            'complete unbound catalog roles must match original definitions')
        native_names.add(row['type']); files.add(row['file'])
        if row['file'] in authored_files:
            need(row['type'].split('/')[0].startswith('uwvm_'),
                 'authored bridge requires a distinct UWVM package; never an official namespace')
    need(files == generated_roles and sum(map(len,assets.values())) <= MAX_TOTAL,
         'exact complete dependency closure and finite retained source budget required')
    return assets


def final_sources(preparer, request, raw, *, read):
    for name, expected in raw.items():
        need(preparer.admitted(read,'complete-source-final:' + name,request['support'][name]) == expected,
             'current complete implementation changed during source work')


def generated_files(preparer, root, prefix, proof):
    outputs = proof['outputs']
    need(type(outputs) is dict and 1 <= len(outputs) <= MAX_GENERATED_PRODUCTS,
         'finite actual complete source output inventory required')
    pins = {}
    for name, expected in outputs.items():
        preparer.relative(name)
        pin = preparer.fact(root / prefix / name)
        need({key:pin[key] for key in ('bytes','sha256')} == expected,
             'actual complete generated source differs: ' + name)
        pins[prefix + '/' + name] = pin
    pins[prefix + '/generation.vendor.json'] = preparer.fact(root / prefix / 'generation.vendor.json')
    need(sum(pin['bytes'] for pin in pins.values()) <= MAX_TOTAL,
         'finite actual complete generated source budget required')
    return pins


def prepare(inventory, request, *, read, load, out):
    """Write original Source plus six complete Guest codecs/native exporter.

    Installed native headers are intentionally not invented at source time.
    Their exact installed bytes, whole compiler dependency/link closure,
    constants and stopped process maps belong to the later real native run.
    A refusal leaves any newly created partial directory available for review.
    """
    need(type(request) is dict, 'explicit operator request required')
    preparer, api, raw = modules(request,read=read,load=load)
    bodies = inputs(preparer,inventory,request,read=read)
    # Reserve every possible generated product plus generation.vendor.json
    # before the first output effect; every published preparation is bindable
    # within the same finite file-count domain.
    need(len(bodies) <= MAX_PREPARATION_FILES - MAX_GENERATED_PRODUCTS - 1,
         'complete source assets exceed the finite bindable preparation inventory')
    root = preparer.fresh(out)
    pins = {name:preparer.write(root,name,body) for name,body in bodies.items()}
    proof = api.generate(root/'input/catalog.json',root/'guest-only',guest_only=True,
                         helper_source=raw[HELPER])
    pins.update(generated_files(preparer,root,'guest-only',proof))
    need(len(pins) <= MAX_PREPARATION_FILES, 'finite complete preparation inventory required')
    need(inputs(preparer,inventory,request,read=read) == bodies,
         'original complete input changed after source work')
    final_sources(preparer,request,raw,read=read)
    receipt = {'format_version':1,'kind':'vendor-complete-profile-source-preparation-v1',
        'status':'SOURCE_PREPARED_NOT_NATIVE_OR_HARDWARE_QUALIFIED','profile':PROFILE,
        'request':request,'files':pins,'default_authority':[],
        'native_identity':'NOT_ACQUIRED','native_registry_emitted':False,
        'compiler_executed':False,'Runtime_executed':False,'official_cases_run':0,
        'native_delivery_qualified':False,'hardware_qualified':False,
        'required_next':['build exact native exporter against original installed SDK and matching complete native domain provider',
            'verify actual cold constants, original full metadata/traits and complete native field witnesses',
            'retain actual exporter owner, whole dependencies, link and stopped native library mapping evidence',
            'bind only the genuine finite raw exporter output, then compile and run the independent default-denied Guest']}
    preparer.write(root,'preparation.complete.json',preparer.canonical(receipt))
    return receipt


def bind_and_generate(inventory, preparation_pin, native_metadata, native_owner_evidence,
                      *, read, load, out, installed_source_proof=None):
    """Bind genuine complete native metadata without accepting a success flag.

    Native owner evidence is retained DATA. Matching IDs do not prove delivery,
    robot safety, a fresh live process, or permissions. The complete adapter
    compares constants and full native fields/defaults/protocol graphs before
    source identities can be copied. There is no old-binder fallback.
    """
    # Read through a caller-supplied admitted current source module first.
    # The receipt is bounded ordinary DATA, never executable source.
    ordinary_pin(preparation_pin)
    initial = read('complete-source-preparation',preparation_pin)
    need(type(initial) is bytes and len(initial) <= 16*1024*1024 and
         type(preparation_pin) is dict and type(preparation_pin.get('bytes')) is int and
         len(initial) == preparation_pin['bytes'] and
         hashlib.sha256(initial).hexdigest() == preparation_pin.get('sha256'),
         'actual admitted complete source preparation required')
    import json
    def pairs(rows):
        result = {}
        for key, value in rows:
            need(key not in result, 'duplicate preparation JSON key')
            result[key] = value
        return result
    original = json.loads(initial,object_pairs_hook=pairs,
        parse_constant=lambda _: need(False,'nonfinite preparation JSON'))
    need(type(original) is dict and original.get('kind') == 'vendor-complete-profile-source-preparation-v1' and
         original.get('status') == 'SOURCE_PREPARED_NOT_NATIVE_OR_HARDWARE_QUALIFIED' and
         original.get('profile') == PROFILE and original.get('default_authority') == [],
         'complete-profile source preparation receipt required')
    request = original['request']
    preparer, api, raw = modules(request,read=read,load=load)
    need(preparer.admitted(read,'complete-preparation-reopen',preparation_pin) == initial,
         'preparation changed during source admission')
    bodies = inputs(preparer,inventory,request,read=read)
    files = original['files']
    need(type(files) is dict and len(files) <= MAX_PREPARATION_FILES, 'finite whole preparation output inventory required')
    for name, pin in files.items():
        preparer.relative(name)
        body = preparer.admitted(read,'complete-prepared:' + name,pin)
        if name in bodies:
            need(body == bodies[name], 'prepared original differs from admitted source: ' + name)
    need(set(bodies) <= set(files), 'complete original source missing from preparation')
    metadata = preparer.admitted(read,'complete-actual-native-metadata',native_metadata)
    owner = preparer.admitted(read,'complete-actual-native-owner-evidence',native_owner_evidence)
    source_proof = (preparer.admitted(read,'complete-original-installed-source-proof',installed_source_proof)
                    if installed_source_proof is not None else None)
    root = preparer.fresh(out)
    retained = {name:preparer.write(root,name,body) for name,body in bodies.items()}
    document, binding = api.bind(root/'input/catalog.json',metadata,installed_source_proof=source_proof)
    retained['actual-native-metadata.json'] = preparer.write(root,'actual-native-metadata.json',metadata)
    retained['actual-native-owner-evidence.json'] = preparer.write(root,'actual-native-owner-evidence.json',owner)
    if source_proof is not None:
        retained['actual-installed-source-proof.json'] = preparer.write(root,'actual-installed-source-proof.json',source_proof)
    bound = preparer.write(root,'bound/catalog.json',preparer.canonical(document))
    retained['bound/catalog.json'] = bound
    for name, body in bodies.items():
        if name.startswith('input/') and name != 'input/catalog.json':
            retained['bound/' + name[6:]] = preparer.write(root,'bound/' + name[6:],body)
    proof = api.generate(root/'bound/catalog.json',root/'native-bound',guest_only=False,
                         helper_source=raw[HELPER])
    retained.update(generated_files(preparer,root,'native-bound',proof))
    need(inputs(preparer,inventory,request,read=read) == bodies,
         'complete original source changed after native source binding')
    for name, pin in files.items():
        preparer.admitted(read,'complete-prepared-final:' + name,pin)
    need(preparer.admitted(read,'complete-metadata-final',native_metadata) == metadata and
         preparer.admitted(read,'complete-owner-evidence-final',native_owner_evidence) == owner and
         preparer.admitted(read,'complete-preparation-final',preparation_pin) == initial,
         'native inputs or original preparation changed during source binding')
    final_sources(preparer,request,raw,read=read)
    if source_proof is not None:
        need(preparer.admitted(read,'complete-installed-source-proof-final',installed_source_proof) == source_proof,
             'original installed-source proof changed during binding')
    receipt = {'format_version':1,'kind':'vendor-complete-profile-native-source-binding-v1',
        'profile':PROFILE,'status':'SOURCE_BOUND_NOT_NATIVE_OR_HARDWARE_QUALIFIED',
        'preparation':preparation_pin,'actual_native_metadata':native_metadata,
        'native_owner_evidence':native_owner_evidence,'binding':binding,'files':retained,
        'default_authority':[],'native_registry_emitted':True,
        'compiler_executed':False,'Runtime_executed':False,'official_cases_run':0,
        'native_delivery_qualified':False,'hardware_qualified':False}
    if source_proof is not None:
        receipt['installed_source_proof']=installed_source_proof
    preparer.write(root,'binding.complete.json',preparer.canonical(receipt))
    return receipt
