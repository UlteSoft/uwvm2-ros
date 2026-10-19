#!/usr/bin/env python3
"""DRAFT callable for Root's existing current64 owner; never executed here.

The function writes no file and starts no actor. Root passes actual current
full-seven observations for the completed Typed14 report, already-staged shared
metadata helper and selected source manifest. Fresh output strings/context come
from Root. No future Typed14 pin is supplied or guessed by this source draft.
"""
from pathlib import Path


R5_CONFIGURATION = {
    'path': '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source40-unitree-command14-boot3-inputs-r5/controls/frame-config.json',
    'bytes': 19016, 'sha256': 'db92cd86f00fd3caf42d224ef2cf5819a4e3418023a4775b57711f9dbbf90e7b',
    'device': 81, 'inode': 774389, 'mtime_ns': 1792168167315897572, 'ctime_ns': 1792168167315897572}


def derive_direct_request(common, *, typed_build_current, shared_helper_current,
                          source, manifest_current, cgroup, fresh_paths,
                          restart_mappings):
    """Use only actual preserved config fields and existing file bytes.

    ``common`` must already be loaded from Root's exact SHA-bound frozen source
    under its current cgroup guard. The caller retains its normal live guard
    around these metadata reads and independently SHA-binds this draft before
    use. The maintained Direct maker performs all final independent checks.
    """
    def need(value, reason):
        if not value:
            raise ValueError(reason)

    def current(pin):
        actual = common.opened(Path(pin['path']))
        need(actual == pin, 'supplied CURRENT full-seven observation changed')
        return actual

    def selected(thin):
        # Frame inputs use the physical resolved target of an explicit input
        # spelling. Preserve that original spelling in the provenance list;
        # do not rewrite an original producer document to look newly current.
        spelled = Path(thin['path'])
        physical = spelled.resolve(strict=True)
        actual = common.opened(physical)
        need(all(actual[key] == thin[key] for key in ('bytes', 'sha256')),
             'actual selected config bytes differ')
        provenance.append({'original_selection': thin, 'actual_current_pin': actual,
                           'physical_target_of_explicit_spelling': str(physical)})
        return actual

    provenance = []
    current(R5_CONFIGURATION)
    cfg, cfg_pin = common.document(Path(R5_CONFIGURATION['path']))
    need(cfg_pin == R5_CONFIGURATION and cfg['cgroup'] == cgroup, 'actual preserved r5 config/current context differs')
    current(typed_build_current)
    typed, tp = common.document(Path(typed_build_current['path']))
    need(tp == typed_build_current and typed['status'] ==
         'PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY' and
         typed['configuration'] == cfg_pin and typed['retained_command_products'] == 9 and typed['fresh_command_products'] == 5,
         'the actual completed Typed14 build must consume this exact successful r5 configuration')
    current(shared_helper_current)
    need(shared_helper_current['sha256'] == '86d87f16cde0ddcdb91d893a2520a73d3851f4ef0cf13aeda6c6409ddf8262fb',
         'already-staged exact reviewed shared metadata helper required')
    current(manifest_current)
    need(manifest_current['sha256'] == '9a01f114e4091d4329392b291b5785952e99b22d8e2a48be52b7e02a7d8f0178',
         'Root selected actual complete Source43 manifest required')
    inputs = cfg['inputs']
    js_pin = selected(inputs['embedded_js_inputs'])
    js, actual_js = common.document(Path(js_pin['path']))
    need(actual_js == js_pin and js['schema'] == 'uwvm-embedded-js-build-inputs-v1',
         'actual pinned original embedded-JavaScript SDK metadata required')
    tools = {role: selected(inputs[label]) for role, label in
             (('cc', 'wasi_cc'), ('cxx', 'wasi_cxx'), ('python', 'python'), ('validator', 'wasm_tools'))}
    for role in ('libc', 'libm', 'printscan', 'builtins'):
        tools[role] = selected(js['inputs'][role])
    # These two exact archive basenames are mandated by the maintained Direct
    # consumer; the sysroot itself is read from the actual qualified producer.
    sysroot = Path(js['paths']['sysroot'])
    for role, basename in (('libcxx', 'libc++.a'), ('libcxxabi', 'libc++abi.a')):
        tools[role] = common.opened((sysroot / 'lib/wasm32-wasip1' / basename).resolve(strict=True))
    need(tools['cc'] == selected(js['inputs']['clang']) and
         tools['validator'] == selected(js['inputs']['validator']),
         'actual typed/WASI/embedded compiler and validator selections must agree')
    sdk_paths = {'sysroot': js['paths']['sysroot'], 'resource_dir': js['paths']['resource_dir'],
                 'wasm_ld_alias': js['paths']['linker_alias'], 'quickjs': js['paths']['quickjs']}
    official = typed['official_cpp_object']['qualification']
    need(official['compiler']['physical'] == tools['cxx']['path'] and official['sysroot'] == sdk_paths['sysroot'],
         'actual official C++ termination qualification uses a different compiler/sysroot')
    # Its artifact81→81 equality is represented by exact pins, not a forbidden
    # same-device transition row in the Direct Evidence API.
    host_roots = sorted({str(Path(pin['path']).parent) for pin in tools.values()
                         if pin['device'] != 81})
    # The actual r5 config contains no restart_mappings field. Root must pass
    # its separately selected explicit scopes, including66307 and66308 for the
    # current66312 host if those original references are consumed. The caller
    # must pass actual explicitly selected distinct-device scopes; no scope is
    # inferred merely from the host-tool parent observations above.
    need(type(restart_mappings) is list and all(type(row) is dict and set(row) ==
         {'root', 'historical_device', 'current_device'} and row['historical_device'] != row['current_device']
         for row in restart_mappings), 'explicit current historical-device scopes required')
    request = {'format_version': 1, 'kind': 'uwvm-unitree-direct-build-configuration-request-v1',
        'source': {'root': source, 'manifest': manifest_current}, 'cgroup': cgroup,
        'shared_helper': shared_helper_current, 'typed_build': typed_build_current,
        'tools': tools, 'sdk_paths': sdk_paths, 'restart_mappings': restart_mappings, 'paths': fresh_paths,
        'limits': {'jobs': 1, 'command_seconds': 600, 'rss_bytes': 2 * 1024**3,
                   'log_bytes': 16 * 1024**2, 'output_bytes': 4 * 1024**3}}
    return request, {'configuration': cfg_pin, 'embedded_js_metadata': js_pin,
        'actual_tool_provenance': provenance, 'actual_host_tool_parent_observations_only': host_roots,
        'restart_scopes_supplied_explicitly': True, 'compiler_or_Runtime_executed': False}
