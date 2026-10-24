#!/usr/bin/env python3
"""Root-only outer owner for three fresh Humble Gateways and portable peer.

The adjacent current-source control still needs actual Root staging paths.
This author has run no helper, namespace actor, compiler, exporter or test.
The child retains all SDK COPY/mount/process responsibility; this owner waits
cooperatively before final exact-pidfd cleanup and verifies its actual receipt.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import signal
import stat
import sys
import time
import traceback
import types

sys.dont_write_bytecode = True
MIB = 1024**2
GIB = 1024**3
PIN_KEYS = {'path','device','inode','bytes','mtime_ns','ctime_ns','sha256'}
WORKER_SHA = 'f1f6e9efa9cc13b8569b4f66fe492dac2aee1e7e56e88405218e066056b5fd24'
HOST_METADATA_SHA = '01b3d985342d63756082f48679134e117b5a89b83c520b964e5fc846c944e88a'
UNSHARE_SHA = '3568cbd5652923d43a080e4e4edbd516edc1aa9978744f2b035c004a704de93c'
CG = {'memory_max':64*GIB,'memory_swap_max':0,'expected_device':29,'expected_inode':18315,
      'expected_boot_id':'ec0d5f0f-3060-4220-bee2-2a7791ed4e72',
      'expected_leaf':'/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
      'expected_cpus':'0-31'}


# Historical source/SDK bytes are admitted separately after this exact reboot.
# The support module never rewrites raw reports or re-labels historical actors.
HISTORICAL_CG = dict(CG, expected_inode=18151, expected_boot_id='af121220-8c6e-4302-9853-f08bf4c50dbb')
RESTART = None
RESTART_SUPPORT_SHA = '22492fdfadb84b5016cc0624229283f34289a144764a46fde4dfa95cdf94a485'


def install_restart(config, common, source, manifest_pin):
    global RESTART
    selected = config['restart_requalification']
    need(type(selected) is dict and set(selected) == {'helper','parallel_helper','support','completed_receipt'},
         'explicit reviewed restart support/helper/completed receipt required')
    raw, pin = read(Path(selected['support']['path']), MIB)
    need(pin == selected['support'] and pin['sha256'] == RESTART_SUPPORT_SHA,
         'reviewed physical restart support bytes changed')
    module = types.ModuleType('native74_r4_explicit_restart_support')
    module.__file__ = pin['path']
    exec(compile(raw, module.__file__, 'exec'), module.__dict__)
    RESTART = module.RestartAdmission(selected, common, read, CG, source, manifest_pin)


def same_pin(actual, previous):
    if actual == previous:
        return True
    need(RESTART is not None, 'post-restart selected pin lacks explicit content admission')
    return RESTART.compare(actual, previous)



def need(value, reason):
    if not value:
        raise ValueError(reason)


def pairs(items):
    result = {}
    for key, value in items:
        need(key not in result, 'duplicate JSON key')
        result[key] = value
    return result


def read(path, limit=64*MIB):
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute input required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        first = os.fstat(fd)
        need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= limit, 'bounded regular input required')
        raw = bytearray()
        while True:
            block = os.read(fd, min(MIB, limit + 1 - len(raw)))
            if not block:
                break
            raw.extend(block)
            need(len(raw) <= limit, 'input grew beyond bound')
        last = os.fstat(fd)
        named = path.stat(follow_symlinks=False)
        facts = lambda s:(s.st_dev,s.st_ino,s.st_size,s.st_mtime_ns,s.st_ctime_ns)
        need(facts(first) == facts(last) == facts(named) and len(raw) == first.st_size, 'opened input changed')
        body = bytes(raw)
        return body, dict(path=str(path), device=first.st_dev, inode=first.st_ino, bytes=len(body),
                          mtime_ns=first.st_mtime_ns, ctime_ns=first.st_ctime_ns,
                          sha256=hashlib.sha256(body).hexdigest())
    finally:
        os.close(fd)


def document(path, limit=64*MIB):
    raw, pin = read(path, limit)
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda value: need(False, 'nonfinite JSON')), pin


def alias_binding(value):
    path = Path(value)
    need(path.is_absolute(), 'absolute alias selection required')
    prefix = Path(path.anchor)
    links = []
    for component in path.parts[1:]:
        prefix /= component
        if not prefix.is_symlink():
            continue
        before = prefix.lstat()
        target = os.readlink(prefix)
        resolved = prefix.resolve(strict=True)
        after = prefix.lstat()
        need(all(getattr(before,key) == getattr(after,key) for key in
             ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode')) and os.readlink(prefix) == target,
             'selected alias changed')
        links.append({'path':str(prefix),'target':target,'resolved':str(resolved),'device':before.st_dev,
            'inode':before.st_ino,'bytes':before.st_size,'mtime_ns':before.st_mtime_ns,'ctime_ns':before.st_ctime_ns})
    return {'selection':str(path),'resolved':str(path.resolve(strict=True)),'links':links}


REUSE_REPORTS = {'inner': {'path': '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source39-native-gateways-physics-peer-r1/report.json', 'bytes': 7986560, 'sha256': '75248cdddba3ab42f0020758cec81b0388c59d45ebd46d356ad434f6207e04d2', 'device': 81, 'inode': 677463, 'mtime_ns': 1792118141739107205, 'ctime_ns': 1792118141748107338}, 'outer': {'path': '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/source39-native-gateways-physics-peer-root-supervisor-r1/report.json', 'bytes': 86618, 'sha256': '542a61a25de916899bbaa2e3ecbd57eb5583480fe13305826e16053d489e401c', 'device': 81, 'inode': 677496, 'mtime_ns': 1792118156347323565, 'ctime_ns': 1792118156352323639}}


def admit_reused_sdk(config, source, mf_pin):
    """Admit only Root's preserved failed-r1 COPY, never any old target object."""
    expected = REUSE_REPORTS
    need(type(config['reuse_execution_view']) is dict and config['reuse_execution_view'] == expected,
         'only the two exact Root-observed failed-r1 report full7s may select SDK reuse')
    docs = {}; pins = []
    for name, selected in expected.items():
        doc, pin = document(Path(selected['path']), 64*MIB)
        need(same_pin(pin, selected), 'original failed-r1 report full7 changed: '+name)
        docs[name] = doc; pins.append(pin)
    old, outer = docs['inner'], docs['outer']
    for name, doc in docs.items():
        need(doc.get('status') == 'FAIL' and doc.get('cleanup',{}).get('status') == 'PASS' and
             doc['cleanup'].get('remaining') == [] and doc.get('finalization_errors') == [] and
             all(doc.get(key) is True for key in ('source_preserved','inputs_preserved','products_preserved')),
             'only terminal failed-r1 reports with complete preservation and cleanup are admitted: '+name)
        need(doc['source'] == str(source) and doc['source_manifest_sha256'] == mf_pin['sha256'] and
             doc['source_before'] == doc['source_after'] and same_pin(mf_pin, doc['source_before']['manifest']),
             'failed-r1 source identity must remain its actual unchanged Source39')
    need(old.get('execution_view_preserved') is True and old.get('remaining_mounts') == [] and
         old.get('qualified_source_translation_units') == 72 and old.get('reuse') is False and
         old.get('error') == "ValueError('native launcher exited before cooperative mapped stop')",
         'exact original failed exporter attempt required; 72 old TUs are provenance only')
    need(outer['native_configuration'] == old['configuration'] and outer['worker']['sha256'] ==
         '9a083274f67ed2a91b74fe7ccf38bbbb249f29730c7e6bd4e738537ff8a2cbc4' and
         len(outer['commands']) == 1 and outer['commands'][0]['status'] == 'FAIL' and
         outer['commands'][0].get('returncode') == 1,
         'original outer must bind the actual r1 worker/configuration and failed direct child')
    original_out = Path(expected['inner']['path']).parent
    argv = outer['commands'][0]['argv']
    for option, value in (('--out',str(original_out)),('--config',old['configuration']['path']),
                          ('--config-sha256',old['configuration']['sha256']),('--source',str(source)),
                          ('--manifest',mf_pin['path']),('--manifest-sha256',mf_pin['sha256'])):
        need(argv.count(option) == 1 and argv[argv.index(option)+1] == value,
             'original failed child argument association changed: '+option)
    need(old['producer_reports'] == config['reports'], 'reused COPY must retain the same three admitted SDK/provider reports')
    original_config, pin = document(Path(old['configuration']['path']),4*MIB)
    need(same_pin(pin, old['configuration']) and set(original_config) ==
         {'format_version','kind','cgroup','reports','host_tools','host_input_roots','control_roots'},
         'original r1 configuration full7/schema changed')
    need(original_config['cgroup'] == HISTORICAL_CG and config['cgroup'] == CG, 'separate exact historical/current cgroup selections required')
    for key in ('format_version','kind','reports','host_tools','host_input_roots'):
        need(original_config[key] == config[key], 'SDK reuse cannot change original input selection: '+key)
    pins.append(pin)
    for selected in (outer['worker'],outer['control']):
        _, pin = read(Path(selected['path']),4*MIB);need(same_pin(pin, selected),'original r1 control/source changed');pins.append(pin)
    before, after = old['execution_view_before'], old['execution_view_after']
    need(len(before) == len(after) == 4 and all(
         {key:value for key,value in a.items() if key != 'catalog'} ==
         {key:value for key,value in b.items() if key != 'catalog'} for a,b in zip(before,after)),
         'original complete execution-view before/after identity differs')
    root = original_out/'rootfs'; catalog = after[0]
    need(catalog['root'] == str(root) and root.resolve(strict=True) == root and root.is_dir() and
         (catalog['files'],catalog['symlinks'],catalog['bytes'],catalog['content_sha256']) ==
         (32047,1265,1266016500,'3ef3dc72e08aa1341f6ce054b49c9065059c4831c8bd76fca9f4ab719ab334ca'),
         'only the complete actual preserved r1 physical rootfs may be reused')
    for row in (before[0],after[0]):
        _, pin = read(Path(row['catalog']['path']),64*MIB)
        need(same_pin(pin, row['catalog']),'original r1 SDK catalog full7 changed');pins.append(pin)
    ledger = Path(old['base_COPY']['ledger'])
    matches = [pin for pin in old['product_pins'] if pin['path'] == str(ledger)]
    need(len(matches) == 1 and ledger == original_out/'base-copy-bindings.jsonl' and
         old['base_COPY'] == {'file_bytes':1266016500,'files':32046,'ledger':str(ledger),'symlinks':1265},
         'complete original independent SDK COPY ledger required')
    _, pin = read(ledger,64*MIB);need(same_pin(pin, matches[0]),'original COPY ledger full7 changed');pins.append(pin)
    loader = old['execution_loader']['physical_loader']['input']
    need(Path(loader['path']).is_relative_to(root), 'original physical SDK loader escaped rootfs')
    _, pin = read(Path(loader['path']),64*MIB);need(same_pin(pin, loader),'original physical SDK loader changed');pins.append(pin)
    return {'scope':'PRESERVED_R1_SDK_COPY_ONLY_NO_COMPILED_OBJECT_OR_ELF_REUSE',
            'reports':expected,'original_status':'FAIL','original_qualified_TUs_provenance_only':72,
            'source_before':old['source_before'],'source_after':old['source_after'],
            'original_configuration':old['configuration'],'original_worker':outer['worker'],
            'original_COPY':old['base_COPY'],'original_catalog':catalog,
            'original_loader':loader,'root':str(root),'pins':pins,
            'compiled_object_reuse':False,'all_74_TUs_must_be_fresh':True}


def verify_reused_COPY(common, selection, base, guard):
    """Reopen every original source/COPY full7 through the exact bound ledger."""
    ledger = Path(selection['original_COPY']['ledger']);root = Path(selection['root'])
    selected = next(pin for pin in selection['pins'] if pin['path'] == str(ledger))
    need(common.opened(ledger,64*MIB) == selected,'original COPY ledger changed before streaming')
    names = set();files = links = total = 0
    with ledger.open('rb') as stream:
        while True:
            guard();raw = stream.readline(65537)
            if not raw:break
            need(len(raw) <= 65536 and raw.endswith(b'\n'),'bounded complete COPY ledger record required')
            row = json.loads(raw,object_pairs_hook=pairs)
            relative = row['relative'];part = Path(relative)
            need(type(relative) is str and not part.is_absolute() and '..' not in part.parts and
                 str(part) == relative and relative not in names,'ordinary distinct original COPY path required')
            names.add(relative);need(len(names) <= 65536,'COPY ledger record bound exceeded')
            a,b = base/relative,root/relative
            need(a.parent.resolve(strict=True).is_relative_to(base) and b.parent.resolve(strict=True).is_relative_to(root),
                 'original COPY input escaped admitted SDK/rootfs')
            if row['kind'] == 'file':
                need(set(row) == {'relative','kind','source','copy','identical_existing'} and
                     row['source']['path'] == str(a) and row['copy']['path'] == str(b) and
                     row['identical_existing'] is False,'closed original independent file COPY record required')
                need(same_pin(common.opened(a), row['source']) and same_pin(common.opened(b), row['copy']),
                     'original SDK source or independent COPY physical full7 changed')
                need(all(row['source'][key] == row['copy'][key] for key in ('bytes','sha256')) and
                     (row['source']['device'],row['source']['inode']) !=
                     (row['copy']['device'],row['copy']['inode']),'original COPY was not independent identical bytes')
                files += 1;total += row['source']['bytes']
            else:
                need(row['kind'] == 'symlink' and set(row) ==
                     {'relative','kind','target','source_device','source_inode','identical_existing'} and
                     row['identical_existing'] is False and a.is_symlink() and b.is_symlink() and
                     os.readlink(a) == os.readlink(b) == row['target'] and
                     RESTART.link_source(a,row['source_device'],row['source_inode'],row['target']),
                     'original source/COPY alias identity changed')
                links += 1
    need(common.opened(ledger,64*MIB) == selected and
         (files,links,total) == (32046,1265,1266016500),'complete original SDK COPY ledger changed')
    return {'ledger':selected,'source_root':str(base),'reused_root':str(root),
            'files':files,'symlinks':links,'file_bytes':total,
            'every_original_source_and_COPY_full7_reopened':True,'no_old_target_object_admitted':True}


def reused_directory_state(root, output, guard):
    """Retain current directory metadata too; mounting must not mutate old dirs."""
    count = 0;digest = hashlib.sha256()
    with output.open('xb') as stream:
        for directory,dirs,names in os.walk(root,followlinks=False):
            guard();dirs.sort();names.sort();path = Path(directory);info = path.lstat()
            need(stat.S_ISDIR(info.st_mode) and not path.is_symlink(),'ordinary reused SDK directory required')
            row = {'relative':str(path.relative_to(root)),'device':info.st_dev,'inode':info.st_ino,
                   'mode':info.st_mode,'nlink':info.st_nlink,'bytes':info.st_size,
                   'mtime_ns':info.st_mtime_ns,'ctime_ns':info.st_ctime_ns}
            raw = (json.dumps(row,sort_keys=True,separators=(',',':'))+'\n').encode()
            stream.write(raw);digest.update(raw);count += 1
            need(count <= 65536,'reused SDK directory metadata bound exceeded')
        stream.flush();os.fsync(stream.fileno())
    return {'root':str(root),'directories':count,'content_sha256':digest.hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--control', type=Path, required=True)
    parser.add_argument('--control-sha256', required=True)
    args = parser.parse_args()
    need(sys.platform == 'linux' and os.geteuid() == 0 and hasattr(os,'pidfd_open'), 'actual Linux Root pidfd owner required')
    need(re.fullmatch('[0-9a-f]{64}', args.control_sha256), 'actual control SHA required')
    control, control_pin = document(args.control, 4*MIB)
    need(control_pin['sha256'] == args.control_sha256, 'actual filled control changed')
    need(type(control) is dict and set(control) == {'format_version','source','manifest','manifest_sha256',
         'worker','native_configuration','native_configuration_sha256','host_metadata_report','unshare','out','native_out'} and
         type(control['format_version']) is int and control['format_version'] == 1, 'closed outer control required')
    for key in ('source','manifest','worker','native_configuration','host_metadata_report','out','native_out'):
        need(type(control[key]) is str and Path(control[key]).is_absolute(), 'Root must fill actual absolute ' + key)
    for key in ('manifest_sha256','native_configuration_sha256'):
        need(type(control[key]) is str and re.fullmatch('[0-9a-f]{64}',control[key]), 'Root must fill actual SHA ' + key)
    need(type(control['unshare']) is dict and set(control['unshare']) == {'invocation','bytes','sha256'} and
         control['unshare']['invocation'] == '/usr/bin/unshare' and control['unshare']['bytes'] == 47872 and
         control['unshare']['sha256'] == UNSHARE_SHA, 'explicit actual observed unshare selection required')
    source = Path(control['source'])
    need(source.is_dir() and source.resolve(strict=True) == source and source != Path('/'), 'canonical actual frozen source required')
    manifest_path = Path(control['manifest'])
    manifest, manifest_pin = document(manifest_path, 16*MIB)
    need(manifest_pin['sha256'] == control['manifest_sha256'] and type(manifest) is dict and manifest, 'actual complete source MF required')
    bodies = {}
    helper_pins = []
    for relative in ('tools/ros_distro/common.py','tests/ros_cgroup/probe.py',
                     'tests/ros_javascript_guest/native_peer.cpp','tests/ros_javascript_guest/exec_gate.cpp',
                     'sdk/integrations/unitree/native_identity.cpp'):
        raw, pin = read(source/relative, 4*MIB)
        need(manifest.get(relative) == {key:pin[key] for key in ('bytes','sha256')}, 'MF-bound source bytes required: ' + relative)
        bodies[relative] = raw
        helper_pins.append(pin)
    common = types.ModuleType('unitree_native_native_outer_common')
    common.__file__ = str(source/'tools/ros_distro/common.py')
    sys.modules[common.__name__] = common
    exec(compile(bodies['tools/ros_distro/common.py'],common.__file__,'exec'),common.__dict__)
    cancelled = common.install_interrupts()
    config, config_pin = document(Path(control['native_configuration']),4*MIB)
    need(config_pin['sha256'] == control['native_configuration_sha256'] and type(config) is dict and
         set(config) == {'format_version','kind','cgroup','reports','host_tools','host_input_roots','control_roots','reuse_execution_view','restart_requalification'} and
         config['format_version'] == 1 and config['kind'] == 'uwvm-humble-gateway-portable-peer-build-inputs-v1', 'actual closed view configuration required')
    install_restart(config,common,source,manifest_pin)
    cg = config['cgroup']
    need(cg == CG and set(cg) == {'memory_max','memory_swap_max','expected_device','expected_inode','expected_boot_id','expected_leaf','expected_cpus'} and
         type(cg['memory_max']) is int and cg['memory_max'] == 64*GIB and type(cg['memory_swap_max']) is int and cg['memory_swap_max'] == 0,
         'actual original64/swap0 cgroup required')
    # The child additionally closes exact dev/inode/boot/leaf/CPU values and
    # applies the narrow historical cross-mount-namespace projection. This
    # owner always compares raw snapshots in its unchanged original namespace.
    worker_path = Path(control['worker'])
    _, worker_pin = read(worker_path, MIB)
    need(worker_pin['sha256'] == WORKER_SHA and worker_pin['bytes'] == 117327, 'reviewed native74 Gateway/peer/controller producer required')
    host_metadata, host_metadata_pin = document(Path(control['host_metadata_report']), MIB)
    need(host_metadata_pin['sha256'] == HOST_METADATA_SHA and host_metadata_pin['bytes'] == 15986,
         'actual preserved host-tool metadata report required')
    need(type(config['host_tools']) is dict and set(config['host_tools']) == {'python','mount','umount','chroot'}, 'four selected actual view tools required')
    roots = [Path(value) for value in config['host_input_roots']]
    need(1 <= len(roots) <= 32 and len(set(roots)) == len(roots) and
         all(root.is_absolute() and root.resolve(strict=True) == root and root.is_dir() and root != Path('/') for root in roots), 'complete canonical host tool roots required')
    # Compare inert actual metadata recursively without assuming an envelope
    # field not supplied by its producer. The exact recorded tool row, not a
    # guessed tool label or content digest alone, must be present in that JSON.
    metadata_rows = []
    pending = [host_metadata]
    while pending:
        item = pending.pop()
        need(len(metadata_rows)+len(pending) <= 50000, 'finite host metadata structure required')
        if type(item) is dict:
            metadata_rows.append(item)
            pending.extend(item.values())
        elif type(item) is list:
            pending.extend(item)
    tool_pins = []
    selected_aliases = {}
    for name, row in config['host_tools'].items():
        need(type(row) is dict and set(row) == {'invocation','pin'} and set(row['pin']) == PIN_KEYS and row in metadata_rows,
             'exact originally observed full host-tool row required: ' + name)
        invocation = Path(row['invocation'])
        need(invocation.is_absolute() and invocation.parent.resolve(strict=True) == invocation.parent and
             invocation.resolve(strict=True) == Path(row['pin']['path']) and
             any(Path(row['pin']['path']).is_relative_to(root) for root in roots), 'literal selected tool and whole-root binding required')
        raw, pin = read(Path(row['pin']['path']),64*MIB)
        need(same_pin(pin, row['pin']) and raw[:4] == b'\x7fELF' and raw[4:6] == b'\x02\x01' and
             int.from_bytes(raw[18:20],'little') == 62 and os.access(invocation,os.X_OK), 'actual full-pin ELF62 tool required')
        tool_pins.append(pin)
        selected_aliases[str(invocation)] = alias_binding(invocation)
    python = config['host_tools']['python']['invocation']
    need(Path(python).resolve(strict=True) == Path(sys.executable).resolve(strict=True), 'actual selected outer Python interpreter required')
    unshare = Path(control['unshare']['invocation'])
    unshare_alias = alias_binding(unshare)
    unshare_body, unshare_pin = read(unshare.resolve(strict=True), MIB)
    need(all(unshare_pin[key] == control['unshare'][key] for key in ('bytes','sha256')) and
         unshare_body[:4] == b'\x7fELF' and unshare_body[4:6] == b'\x02\x01' and
         int.from_bytes(unshare_body[18:20],'little') == 62 and any(Path(unshare_pin['path']).is_relative_to(root) for root in roots),
         'fresh actual unshare ELF/pin/whole-root admission failed')
    selected_aliases[str(unshare)] = unshare_alias
    out, native_out = Path(control['out']), Path(control['native_out'])
    inputs_to_protect = [source,*roots,*[Path(value).resolve(strict=True) for value in config['control_roots']]]
    need(type(config['reports']) is dict and set(config['reports']) == {'view','native','worker_registries'},
         'three exact actual input reports required')
    expected_reports = {'view':'8c281ba2eec9ea5f7e83ae4a6eb8e1721f9f184c66b227e88894eb9b73905697',
        'native':'c52bc5ed98e52904f52e1f8dac7031cd65a53f94d5128375d8de2eea23ae4d73',
        'worker_registries':'bbbdc3c0121949fc5c30ab417d3a7f6c63e011430a632b803fe8a28e73a948a6'}
    prerequisite_pins, prerequisites = [], {}
    for name, selected in config['reports'].items():
        need(type(selected) is dict and set(selected) == PIN_KEYS, 'actual full7 input report required')
        packet, pin = document(Path(selected['path']),64*MIB)
        need(same_pin(pin, selected) and pin['sha256'] == expected_reports[name], 'actual fixed input report changed')
        prerequisites[name] = packet
        prerequisite_pins.append(pin)
        inputs_to_protect.append(Path(pin['path']).parent)
    # Fail before namespace/build setup if any exact retained model input is
    # outside the reviewed restart closure. In particular recreated resolver
    # metadata is admitted only for equal complete bytes, with fresh identity.
    for previous in prerequisites['view']['input_pins'] + prerequisites['view']['product_pins']:
        actual = common.opened(Path(previous['path']), max(64*MIB, previous['bytes']))
        need(same_pin(actual, previous), 'retained model-view input lacks current content admission')
    reused = admit_reused_sdk(config,source,manifest_pin)
    reused_base = Path(prerequisites['view']['execution_view_after']['root'])
    inputs_to_protect += [reused_base,Path(reused['root'])]
    family_rows = {row['family']:row for row in prerequisites['native']['families']}
    need(set(family_rows) == {'unitree_go','unitree_hg','unitree_api'}, 'complete three native families required')
    inputs_to_protect += [Path(row['derived_tree']['root']) for row in family_rows.values()]
    inputs_to_protect += [Path(prerequisites['worker_registries']['ros2_generation']['path']).parent,
        Path(prerequisites['worker_registries']['composition']['path']).parent,
        Path(family_rows['unitree_hg']['generation']['path']).parent]
    # bbbd is ONLY the immutable Source33 GO+API registry byte provenance.
    # It is not a current Worker selection and is never changed to Source34.
    roots = list(dict.fromkeys([*roots,*inputs_to_protect[1:]]))
    need(len(roots) <= 64, 'finite complete host/SDK/control/source-provider roots required')
    need(all(path.is_absolute() and path.resolve(strict=True) == path and path.is_dir() for path in inputs_to_protect),
         'canonical complete protected producer roots required')
    for path in (out,native_out):
        need(path.parent.resolve(strict=True) == path.parent and not path.exists() and not path.is_symlink() and
             all(not path.is_relative_to(root) and not root.is_relative_to(path) for root in inputs_to_protect), 'fresh independent outer and view outputs required')
    need(not out.is_relative_to(native_out) and not native_out.is_relative_to(out), 'outer and inner outputs must be independent siblings')
    parent_namespace = os.readlink('/proc/self/ns/mnt')
    need(re.fullmatch(r'mnt:\[\d+\]',parent_namespace), 'actual original mount namespace required')
    out.mkdir(mode=0o700)
    (out/'home').mkdir(mode=0o700)
    (out/'tmp').mkdir(mode=0o700)
    RESTART.start(out)
    report = {'format_version':1,'kind':'unitree-three-native-gateways-portable-peer-root-supervisor','status':'RUNNING',
              'source':str(source),'source_manifest_sha256':manifest_pin['sha256'],'control':control_pin,
              'native_configuration':config_pin,'worker':worker_pin,'host_metadata':host_metadata_pin,
              'unshare':{'current_physical':unshare_pin,'reviewed_byte_selection':control['unshare'],'alias':unshare_alias},
              'original_mount_namespace':parent_namespace,'expected_gateway_profiles':['builtin','go-api','hg'],'expected_qualified_source_translation_units':74,'parallelism_policy':{'maximum_jobs':4,'per_heavy_job_reservation_bytes':5*GIB,'compile_command_aggregate_rss_bytes':20*GIB},
              'reused_execution_sdk':reused,'compiled_object_reuse':False,'SDK_COPY_performed':False,
              'limits':{'rss_bytes':24*GIB,'command_seconds':16200,'log_bytes':32*MIB,'cooperative_cleanup_seconds':600,
                        'metadata_rss_bytes':GIB,'backing_free_bytes':8*GIB},
              'Runtime_executed':False,'ROS_DDS_interoperability_qualified':False,'Guest_grants_changed':False,
              'scope':'FRESH_SHARDED_NATIVE_GATEWAYS_PORTABLE_PEER_GATE_STRING_IDENTITY_TYPED_WITNESS_AND_GO2_STAND_CONTROLLER_ONLY_NO_RUNTIME_DDS_OR_PHYSICS_QUALIFICATION','commands':[]}
    before = source_before = None
    before_roots = []
    pins = {}
    products = {}
    runner = None
    started = time.monotonic()
    finalizing = False
    def remember(pin):
        actual = common.opened(Path(pin['path']),max(64*MIB,pin['bytes']))
        need(same_pin(actual,pin), 'initial parsed input pin changed')
        pin = actual
        need(pin['path'] not in pins or pins[pin['path']] == pin, 'input physical identity changed')
        pins[pin['path']] = pin
        return pin
    def product(path,limit=64*MIB):
        pin = common.opened(path,limit)
        need(pin['path'] not in products or products[pin['path']] == pin, 'outer product changed')
        products[pin['path']] = pin
        return pin
    def guard():
        need(finalizing or not cancelled['cancelled'], 'cooperative cancellation requested')
        need(finalizing or time.monotonic()-started < 18000, 'finite outer view deadline exceeded')
        need(finalizing or resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024 <= GIB, 'outer metadata RSS exceeds one GiB')
        need(os.readlink('/proc/self/ns/mnt') == parent_namespace, 'outer mount namespace changed')
        current = common.cgroup_state(cg)
        if before is not None:
            common.cgroup_preserved(before,current)
        need(all(alias_binding(path) == expected for path,expected in selected_aliases.items()), 'selected tool alias changed')
        for folder in (out,*out.parents,native_out.parent,*native_out.parent.parents):
            fs = os.statvfs(folder)
            need(fs.f_bavail*fs.f_frsize >= 8*GIB, 'original output/backing reserve below eight GiB')
        return current
    def save():
        report['restart_content_requalification'] = RESTART.summary()
        report['commands'] = runner.commands if runner else []
        report['input_pins'] = list(pins.values())
        report['product_pins'] = list(products.values())
        common.atomic_json(out/'report.json',report)
    environment = {'PATH':'/usr/bin:/bin','LANG':'C.UTF-8','LC_ALL':'C.UTF-8','HOME':str(out/'home'),
                   'TMPDIR':str(out/'tmp'),'PYTHONDONTWRITEBYTECODE':'1','MAKEFLAGS':'-j1','GOMAXPROCS':'1',
                   'OMP_NUM_THREADS':'1','OPENBLAS_NUM_THREADS':'1'}
    try:
        before = guard()
        need(all(row['events'].get(key,0) == 0 for row in before['ancestors'] for key in ('oom','oom_kill','oom_group_kill')), 'initial ancestor OOM nonzero')
        report['cgroup_before'] = before
        source_before = common.source_state(source,manifest_path)
        need(source_before['manifest'] == manifest_pin, 'initial source MF pin changed')
        report['source_before'] = source_before
        for pin in (control_pin,manifest_pin,config_pin,worker_pin,host_metadata_pin,unshare_pin,*helper_pins,*tool_pins,*prerequisite_pins,*RESTART.inputs):
            remember(pin)
        for pin in reused['pins']:remember(pin)
        report['reused_COPY_before'] = verify_reused_COPY(common,reused,reused_base,guard)
        _, own_pin = read(Path(__file__).resolve(strict=True),MIB)
        remember(own_pin)
        for index,root in enumerate(roots):
            guard()
            row = common.catalog(root,out/('host-before-%02d.jsonl'%index),roots)
            before_roots.append(row)
            product(row['catalog']['path'])
        runner = common.SerialRunner(out,{'rss_bytes':24*GIB,'command_seconds':16200,'log_bytes':32*MIB},guard)
        # No --fork: util-linux unshare execs the selected Python in the same
        # direct Popen birth. The child independently checks actual ancestry
        # and distinct namespace, so an incompatible wrapper fails closed.
        argv = [str(unshare),'--mount','--propagation','private',python,'-B',str(worker_path),
                '--source',str(source),'--manifest',str(manifest_path),'--manifest-sha256',manifest_pin['sha256'],
                '--config',config_pin['path'],'--config-sha256',config_pin['sha256'],
                '--parent-mount-namespace',parent_namespace,'--out',str(native_out)]
        row = runner.run('actual-three-native-gateways-portable-peer',argv,out,environment)
        runner.discover()
        need(all(common.pidfd_terminal(item['fd']) for item in runner.owned.values()), 'owned native-view descendant remains live')
        for key,item in runner.unproven.items():
            now = common.identity(key[0])
            need(common.birth(now) != key or now['state'] in ('Z','X'), 'unproven view actor remains live')
        row['owned_terminal_barrier'] = {'all_observed_pidfds_terminal':True,'Popen_roots_and_unproven_ledger_retained':True}
        result, result_pin = document(native_out/'report.json',64*MIB)
        need(result['schema'] == 'uwvm-embedded-js-native-products-v1' and result['status'] == 'PREPARED_NATIVE_PRODUCTS_NOT_EXECUTED' and result['source'] == str(source) and
             result['source_manifest_sha256'] == manifest_pin['sha256'] and result['configuration'] == config_pin and
             result['mount_namespace']['actual_parent'] == parent_namespace and result['mount_namespace']['current'] != parent_namespace,
             'actual current-source isolated view completion required')
        need(result['cleanup']['status'] == 'PASS' and not result['cleanup']['remaining'] and not result['remaining_mounts'] and
             not result['finalization_errors'] and all(result.get(key) is True for key in
             ('source_preserved','inputs_preserved','products_preserved','execution_view_preserved')), 'view cleanup and complete preservation required')
        need(result.get('preparation_complete') is True and result.get('qualified_source_translation_units') == 74 and
             len(result['products']) == 3 and {row['profile'] for row in result['products']} == {'builtin','go-api','hg'} and
             all(row['backend'] == 2 and row['full_dependencies_verified'] is True for row in result['products']) and
             len(result['execution_view_after']) == 4 and result.get('execution_file_catalog_rows',0) > 0,
             'actual fresh74-TU/three-profile/typed-witness/controller products and complete copied SDK catalog required')
        need(result.get('typed_peer_executed') is False and result.get('typed_example_cases_qualified') is False and
             result.get('typed_peer_protocol')=='UTN1/M/Q/P/A/Z/F' and
             result['typed_peer']['input']['path']==str(native_out/'work/typed-peer/build/unitree-native-typed-peer') and
             result['typed_peer']['machine']==62 and len(result['typed_peer_compile']['compile_records'])==1 and
             result['typed_peer_compile']['compile_records'][0]['source']['path']==str(source/'tests/ros_unitree/native_typed_ros2_peer.cpp'),
             'actual typed native witness compile product required without DDS/test promotion')
        physics=result['physics_controller'];physics_compile=result['physics_controller_compile']
        physics_contract=result['physics_controller_contract']
        need(result.get('physics_controller_executed') is False and
             result.get('physics_controller_DDS_initialized') is False and
             result.get('matched_physics_baseline_qualified') is False and
             physics['input']['path']==str(native_out/'work/physics-controller/build/unitree-native-ros2-controller') and
             physics['machine']==62 and len(physics_compile['compile_records'])==1 and
             physics_compile['compile_records'][0]['source']['path']==str(source/'tests/ros_unitree/native_ros2_controller.cpp') and
             len(physics_compile['file_api']['sources'])==1 and
             physics_compile['file_api']['sources'][0]['path']==str(source/'tests/ros_unitree/native_ros2_controller.cpp') and
             physics_contract['family']=='unitree_go' and physics_contract['mode']=='go2-stand' and
             physics_contract['cxx_standard']==20 and physics_contract['protocol']=='UCK1/R/Q',
             'separate actual native Go2 stand compile product required without physics/DDS promotion')
        need(all(result.get(key) is False for key in ('installation_performed','downloads_performed','Runtime_executed',
             'DDS_initialized_by_worker','ROS_WASI_grants_changed','compiled_object_reuse','SDK_COPY_performed')) and
             result.get('reuse') is True and result.get('reuse_scope')=='SDK_COPY_ONLY_ALL74_TARGET_TUS_FRESH' and
             result.get('reused_execution_sdk')==reused and result.get('reused_execution_sdk_preserved') is True and
             result['execution_view_before'][0]['root']==result['execution_view_after'][0]['root']==reused['root'] and
             RESTART.same_tree(result['execution_view_after'][0],reused['original_catalog']),
             'only exact preserved SDK COPY reuse is admitted; all74 TUs and products must be fresh')
        need(result.get('parallelism_policy') == {'maximum_jobs':4,'per_heavy_job_reservation_bytes':5*GIB,
             'compile_command_aggregate_rss_bytes':20*GIB} and result.get('build_job_plans') and
             all(type(plan['jobs']) is int and 1<=plan['jobs']<=4 and
                 plan['per_job_reservation_bytes']==5*GIB and
                 plan['aggregate_owned_rss_limit_bytes']==20*GIB and
                 plan['jobs']*5*GIB<=plan['memory_headroom_bytes'] and
                 plan['jobs']*5*GIB<=plan['host_memory_headroom_bytes']
                 for plan in result['build_job_plans']), 'actual bounded parallel build policy differs')
        for pin in result['input_pins']:
            remember(pin)
        for pin in result['product_pins']:
            need(product(pin['path'],max(64*MIB,pin['bytes'])) == pin, 'retained inner product changed')
        report['native_product_report'] = product(result_pin['path'],64*MIB)
        report['prepared_profiles'] = [row['profile'] for row in result['products']]
        report['actual_qualified_source_translation_units'] = result['qualified_source_translation_units']
        report['historical_registry_source'] = config['reports']['worker_registries']
        report['historical_registry_scope'] = 'SOURCE33_IDENTICAL_CURRENT_GENERATOR_GO_API_REGISTRY_BYTES_ONLY_NOT_SOURCE34_WORKER_QUALIFICATION'
        report['physics_controller'] = physics
        report['physics_controller_contract'] = physics_contract
        report.update(physics_controller_executed=False,physics_controller_DDS_initialized=False,matched_physics_baseline_qualified=False)
        report['preparation_complete'] = True
    except BaseException as error:
        report.update(status='FAIL',error=repr(error),traceback=traceback.format_exc())
    finally:
        finalizing = True
        errors = []
        def finish(stage,action):
            try:
                action()
            except BaseException as error:
                errors.append({'stage':stage,'error':repr(error)})
                report['status'] = 'FAIL'
        def cooperative():
            if not runner:
                return
            for child in runner.children:
                if child.poll() is not None:
                    continue
                first = runner.roots.get(child.pid)
                now = common.identity(child.pid)
                if now is None and child.poll() is not None:
                    continue
                need(first is not None and common.birth(first) == common.birth(now) and now['parent_pid'] == os.getpid(), 'retained exact direct view coordinator required')
                runner.acquire(child.pid,os.getpid())
                if common.birth(first) not in runner.owned:
                    need(child.poll() is not None, 'live view coordinator has no owned pidfd')
                    continue
                try:
                    signal.pidfd_send_signal(runner.owned[common.birth(first)]['fd'],signal.SIGTERM)
                except ProcessLookupError:
                    pass
                until = time.monotonic()+600
                while child.poll() is None and time.monotonic() < until:
                    guard()
                    runner.discover()
                    need(runner.rss() <= 24*GIB, 'cooperative cleanup RSS exceeds24GiB')
                    need(all(not (out/(row['label']+'.log')).exists() or (out/(row['label']+'.log')).stat().st_size <= 32*MIB for row in runner.commands), 'cooperative cleanup log bound exceeded')
                    time.sleep(.05)
        finish('cooperative-child-owned-cleanup',cooperative)
        def cleanup():
            report['cleanup'] = runner.close() if runner else {'status':'PASS','remaining':[],'scope':'NO_ACTOR_STARTED'}
            need(report['cleanup']['status'] == 'PASS' and not report['cleanup']['remaining'], 'outer exact-owned cleanup incomplete')
        finish('outer-pidfd-cleanup',cleanup)
        if runner:
            for row in runner.commands:
                path = out/(row['label']+'.log')
                if path.exists():
                    finish('retain-final-raw-log',lambda path=path:product(path,32*MIB))
            if (out/'commands.json').exists():
                finish('retain-final-command-ledger',lambda:product(out/'commands.json',64*MIB))
        def preserved():
            need(source_before is not None and len(before_roots) == len(roots), 'complete initial source/tool roots missing')
            report['source_after'] = common.source_state(source,manifest_path)
            need(report['source_after'] == source_before, 'frozen current source changed')
            after = []
            for index,root in enumerate(roots):
                guard()
                row = common.catalog(root,out/('host-after-%02d.jsonl'%index),roots)
                product(row['catalog']['path'])
                need({k:v for k,v in row.items() if k != 'catalog'} == {k:v for k,v in before_roots[index].items() if k != 'catalog'}, 'complete original host tool root changed')
                after.append(row)
            report['host_input_roots_before'] = before_roots
            report['host_input_roots_after'] = after
            need(all(common.opened(Path(pin['path']),max(64*MIB,pin['bytes'])) == pin for pin in pins.values()), 'outer original/inner source input pin changed')
            need(all(common.opened(Path(pin['path']),max(64*MIB,pin['bytes'])) == pin for pin in products.values()), 'outer/inner retained product changed')
            report['selected_tool_aliases'] = selected_aliases
            report['cgroup_after'] = guard()
            report['reused_COPY_after']=verify_reused_COPY(common,reused,reused_base,guard)
            need(report['reused_COPY_after']==report['reused_COPY_before'] and
                 admit_reused_sdk(config,source,manifest_pin)==reused,'original failed-r1 SDK/report/source pins changed')
            report.update(source_preserved=True,inputs_preserved=True,products_preserved=True,reused_execution_sdk_preserved=True)
        finish('outer-and-inner-retained-evidence-preservation',preserved)
        def seal_restart():
            report['restart_content_requalification']=RESTART.finish()
            if RESTART.terminal_pin:product(RESTART.terminal_pin['path'],128*MIB)
            for tree in RESTART.trees.values():product(tree['current_catalog']['path'],64*MIB)
        finish('restart-selected-content-preservation',seal_restart)
        report['finalization_errors'] = errors
        if report.get('preparation_complete') and report['status'] == 'RUNNING' and not errors:
            report['status'] = 'PREPARED_THREE_NATIVE_GATEWAYS_AND_PORTABLE_PEER_NOT_EXECUTED'
        save()
    return 0 if report['status'] == 'PREPARED_THREE_NATIVE_GATEWAYS_AND_PORTABLE_PEER_NOT_EXECUTED' else 1


if __name__ == '__main__':
    raise SystemExit(main())
