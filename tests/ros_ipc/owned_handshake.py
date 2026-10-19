"""Root-owned real IPC handshake contracts for both supported C++ standards.

The entry point is run(frame, load, configuration_input='native-build-config').
Root supplies a genuinely admitted current source Frame with the exact selected
native compiler configuration and22 complete roots. This module performs no
project import or process launch when loaded. The caller must always perform
normal Frame.finish, including after an exception. All seven actual commands
are serial, pidfd-owned and bounded; real compiler full-MD, link inputs, native
ELF targets, logs and clean teardown are retained. Use a short Frame.out path
because Linux filesystem sockets have a108-byte address limit.

Each of the two actual executables exchanges three real socket handshakes;
these six transport cases are not ROS or upstream official test cases. The
producer uses the selected original Clang23/GCC15 native tool configuration,
without selecting or modifying a ROS SDK, LLVM provider or Guest permission.
"""
from __future__ import annotations
import hashlib
import os
from pathlib import Path
import re
import resource
import stat
import sys
import time
import traceback
import types

MIB, GIB = 1024**2, 1024**3
CONTRACTS = (
    ('ipc-cxx17','tests/ros_ipc/client_handshake_contract.cc',
     ('src/uwvm2/imported/ros/ipc/unix.h','src/uwvm2/imported/ros/ipc/protocol.h'),'c++17'),
    ('ipc-cxx26','tests/ros_ipc/client_handshake_contract.cc',
     ('src/uwvm2/imported/ros/ipc/unix.h','src/uwvm2/imported/ros/ipc/protocol.h'),'c++26'),
)
# The reviewed fixture defines exactly three actual socket exchanges per run.
FIXTURE_BODIES = {'tests/ros_ipc/client_handshake_contract.cc': {'bytes': 4588, 'sha256': '48757f74fbcd15e8c90b79424d1cdb55b60664394f7545dd4667a6a793f3661c'}}
HELPERS = ('tests/ros_debugger/build_runtime.py', 'tests/ros_unitree/build_interpreter_workers.py')


def need(value, reason):
    if not value: raise ValueError(reason)


def run(frame, load, *, configuration_input='native-build-config'):
    """Compile and execute the three real socket cases under C++17 and C++26.

    load(relative, unique_name) MUST verify exact current complete-MF bytes
    before executing them, never import a project module through sys.path.
    This function does not set/extend Frame roots, inputs, cgroup or limits.
    Root must call its normal Frame.finish even if this function raises.
    Root's external owner retains actual coordinator RSS/deadline/pidfd scope.
    """
    need(sys.platform == 'linux' and hasattr(os,'pidfd_open') and callable(load), 'actual admitted Linux pidfd Frame/loader required')
    common, source = frame.common, Path(frame.source)
    need(not frame.actors and not frame.report.get('commands') and not frame.config['upstreams'] and
         all(frame.config[name] is None for name in ('simulation','benchmark','profiler')),
         'fresh compiler-only Frame required; no SDK/ROS/Runtime actor may share it')
    need(frame.limits['jobs'] == 1 and frame.limits['rss_bytes'] <= 768*MIB and
         frame.limits['command_seconds'] <= 60 and frame.limits['log_bytes'] <= 16*MIB,
         'actual serial <=768MiB/60-second/16MiB-log command limits required')
    frame.guard()
    frozen, mf_pin = common.document(frame.manifest)
    need(frame.pin(frame.manifest) == mf_pin and mf_pin['sha256'] == frame.report['source_manifest_sha256'],
         'actual current complete Frame source manifest changed')
    helpers = {}
    prefix = 'ipc_handshake_' + hashlib.sha256(str(frame.out).encode()).hexdigest()[:16] + '_'
    for relative in HELPERS:
        module = load(relative,prefix+Path(relative).stem)
        pin = frame.pin(source/relative)
        need(Path(module.__file__) == source/relative and module.__name__ == prefix+Path(relative).stem and
             frozen.get(relative) == {key:pin[key] for key in ('bytes','sha256')}, 'actual complete-MF native helper role required')
        helpers[relative] = module
    builder, worker = (helpers[name] for name in HELPERS)
    configuration, config_pin = common.document(frame.input(configuration_input))
    need(frame.inputs[configuration_input] == config_pin and frame.pin(config_pin['path']) == config_pin,
         'actual selected immutable native configuration changed')
    roots, tools, selections, headers, gcc = worker.configuration(configuration,source,builder)
    need(not any(selections.values()) and not any(headers.values()) and
         configuration['cgroup'] == frame.config['cgroup'], 'no ROS registry flags or alternate cgroup are admitted')
    # Exactly the Core constructor's21 immutable roots plus its independently
    # sealed actual controls root. The old Source37 config is metadata only;
    # no old cgroup/source label supplies this Frame's actual admission.
    control_root = Path(config_pin['path']).parent
    need(len(roots) == 21 and len(frame.roots) == 22 and set(frame.roots) == set(roots)|{control_root} and
         control_root not in roots and all(root != source and source not in root.parents and root not in source.parents
         for root in frame.roots),
         'actual original21 complete native roots plus one sealed control root required')
    need(stat.S_IMODE(control_root.stat().st_mode) == 0o500 and
         stat.S_IMODE(Path(config_pin['path']).stat().st_mode) == 0o400,
         'actual native controls must already be sealed readonly')
    need(len(frame.catalogs) == 22 and {row['root'] for row in frame.catalogs} == {str(root) for root in frame.roots},
         'Frame must already retain all22 actual complete before-root catalogs')
    cgroup = configuration['cgroup']
    need(cgroup['memory_max'] == 64*GIB and cgroup['memory_swap_max'] == 0, 'original current64GiB/zero swap required')
    aliases = {name:worker.aliases(path) for name,path in tools.items()}
    limits = {key:frame.limits[key] for key in ('jobs','rss_bytes','command_seconds','log_bytes')}
    out = frame.out/'hs'
    need(not out.exists() and not out.is_symlink(), 'fresh owned IPC handshake contract output required')
    out.mkdir(mode=0o700)
    for name in ('home','tmp'): (out/name).mkdir(mode=0o700)
    started = time.monotonic(); cleanup_started = None; finalizing = False; next_output_walk = 0.0
    owner = None; proxy = None; cleanup_result = None
    inputs = {value['path']:value for value in (mf_pin,config_pin)}; products = {}; source_before = None
    report = {'kind':'actual-ipc-handshakes-cxx17-cxx26','status':'RUNNING',
        'source':str(source),'source_manifest_sha256':mf_pin['sha256'],'native_configuration':config_pin,
        'standard':'c++17-and-c++26','parallelism':1,'limits':limits,'contracts':[],'commands':[],
        'source_defined_groups_per_contract':3,'source_defined_groups_total':6,'actual_executables_completed':0,
        'ROS_executed':False,'Runtime_executed':False,'Guest_executed':False,'SDK_stub_created':False,
        'Guest_rights_changed':False,'mapped_stop_qualification':'NOT_PERFORMED_NO_LIVE_MAPPING_CLAIM',
        'scope':'REAL_UNIX_CLIENT_HANDSHAKES_CXX17_AND_CXX26_NOT_ROS_DELIVERY',
        'clang_version_observed':False,'GCC15_compiler_invoked':False,
        'GCC15_scope':'Actual selected installation directory/whole roots and consumed headers+libstdc++ bytes; no extra GCC executable is guessed'}
    frame.report['ipc_handshake_contracts'] = report
    def save():
        report['commands'] = owner.commands if owner is not None else []
        report['input_pins'],report['product_pins'] = list(inputs.values()),list(products.values())
        common.atomic_json(out/'report.json',report); frame.save()
    def remember(path, expected=None):
        path = Path(path).resolve(strict=True)
        need(path.is_relative_to(source) or worker.inside(path,frame.roots), 'actual dependency outside complete source/22 roots')
        actual = frame.pin(path)
        need((expected is None or expected == actual) and (actual['path'] not in inputs or inputs[actual['path']] == actual),
             'actual observed source/tool/dependency bytes changed')
        inputs[actual['path']] = actual; return actual
    def product(path, maximum=512*MIB):
        actual = common.opened(Path(path),maximum)
        need(Path(actual['path']).is_relative_to(out) and
             (actual['path'] not in products or products[actual['path']] == actual), 'owned native product changed or escaped')
        products[actual['path']] = actual; return actual
    def storage():
        for path in (frame.out,*frame.out.parents):
            facts = os.statvfs(path); need(facts.f_bavail*facts.f_frsize >= 8*GIB, 'active backing reserve below8GiB')
    def budget():
        size = 0
        for directory,dirs,files in os.walk(out,followlinks=False):
            for name in [*dirs,*files]:
                item = Path(directory)/name
                try:
                    info = item.lstat()
                except FileNotFoundError:
                    # Only compiler-owned temporary retirement and the exact
                    # atomic JSON publication suffix can disappear during the
                    # active output scan. Immutable inputs never use this rule.
                    need(not finalizing and (item.is_relative_to(out/'tmp') or name.endswith('.json.pending')),
                         'owned native output unexpectedly disappeared')
                    continue
                if stat.S_ISSOCK(info.st_mode):
                    # The real client fixture needs an owner-only pathname
                    # socket. Admit only its exact temporary leaf shape during
                    # execution; input roots never use this output exception.
                    need(not finalizing and item.is_relative_to(out/'tmp'),
                         'socket outside active owned handshake temporary scope')
                    relative = item.relative_to(out/'tmp')
                    need(len(relative.parts)==2 and relative.name=='socket' and
                         re.fullmatch(r'ros-client-[A-Za-z0-9]{6}',relative.parts[0]) and
                         info.st_uid==os.geteuid() and stat.S_IMODE(info.st_mode)==0o600,
                         'unexpected owned handshake socket identity or access mode')
                    size += info.st_size
                    continue
                need(stat.S_ISDIR(info.st_mode) or stat.S_ISREG(info.st_mode), 'owned C++ model output contains alias or special file')
                if stat.S_ISREG(info.st_mode): size += info.st_size
        need(size <= GIB, 'one-GiB IPC handshake contract output budget exhausted')
    def guard():
        nonlocal next_output_walk
        now = time.monotonic()
        need(now-cleanup_started < 600 if finalizing else now-started < min(configuration['build_seconds'],1800),
             'finite actual IPC handshake build or independent600-second finalization deadline exhausted')
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024 <= 768*MIB, 'native contract coordinator RSS exceeds768MiB')
        common.cgroup_preserved(frame.before,common.cgroup_state(cgroup))
        need(all(worker.aliases(tools[name]) == old for name,old in aliases.items()), 'actual tool invocation alias changed')
        if not finalizing:
            frame.guard(); storage()
        if now >= next_output_walk:
            budget(); next_output_walk = time.monotonic()+.5
    def cleanup():
        nonlocal cleanup_result
        if cleanup_result is None:
            # Cache only a genuinely returned cleanup result, never an exception.
            value = owner.close() if owner is not None else {'status':'PASS','remaining':[],'actors_started':0}
            cleanup_result = value
        return cleanup_result
    environment = {**configuration['environment'],'HOME':str(out/'home'),'TMPDIR':str(out/'tmp'),
        'PYTHONDONTWRITEBYTECODE':'1','MAKEFLAGS':'-j1','CMAKE_BUILD_PARALLEL_LEVEL':'1',
        'OMP_NUM_THREADS':'1','OPENBLAS_NUM_THREADS':'1','MKL_NUM_THREADS':'1'}
    def command(label,argv):
        guard()
        leaf = Path(cgroup['expected_leaf'])
        need(int((leaf/'memory.current').read_text())+limits['rss_bytes']+2*GIB < 60*GIB,
             'actual aggregate cgroup compiler/parent headroom unavailable')
        available = [line.split() for line in Path('/proc/meminfo').read_text().splitlines() if line.startswith('MemAvailable:')]
        need(len(available) == 1 and len(available[0]) == 3 and available[0][1].isdecimal() and available[0][2] == 'kB' and
             int(available[0][1])*1024 >= limits['rss_bytes']+10*GIB,
             'actual host must retain eight-GiB reserve plus compiler/parent headroom')
        try:
            row = owner.run(label,[str(value) for value in argv],out,environment)
            owner.discover(); terminal = []
            for key,item in owner.owned.items():
                need(common.pidfd_terminal(item['fd']), 'owned descendant remains live between serial native commands')
                terminal.append({'original_birth':item['identity'],'pidfd_terminal':True})
            for key in owner.unproven:
                actual = common.identity(key[0])
                need(common.birth(actual) != key or actual['state'] in ('Z','X'), 'unproven live descendant remains')
            for key,item in list(owner.owned.items()):
                os.close(item['fd']); del owner.owned[key]
            row['owned_terminal_barrier'] = terminal
            return row
        finally:
            if owner.commands and 'log' in owner.commands[-1]:
                row = owner.commands[-1]
                need(product(row['log']['path'],limits['log_bytes']) == row['log'], 'actual command log changed')
            save()
    def trace(row):
        raw,actual = common.body_pin(Path(row['log']['path']),limits['log_bytes'])
        need(actual == row['log'], 'actual link trace changed')
        linked, observations = {},[]
        for line in raw.decode('utf-8','strict').splitlines():
            if not line.strip(): continue
            spelling = line.split('(',1)[0]
            selected = Path(spelling); selected = selected if selected.is_absolute() else out/selected
            path = selected.resolve(strict=True)
            need(path.is_file() and worker.inside(path,[out,*roots]), 'actual trace input escaped complete source/provider closure')
            builder.no_extra_llvm([str(path)],'SDK-free IPC handshake link input')
            current = product(path) if path.is_relative_to(out) else remember(path)
            with path.open('rb') as stream: header = stream.read(64)
            if header.startswith(b'!<arch>\n'):
                qualified,members = builder.archive(common,path,configuration['target'])
                need(qualified == current, 'actual native archive changed')
                observation = {'format':'NATIVE_REGULAR_ARCHIVE_ALL_MEMBERS_ELF_TARGET_CHECKED','members':members}
            elif header.startswith(b'\x7fELF'):
                need(builder.elf(common,path,configuration['target'],(1,2,3)) == current, 'actual ELF link input changed')
                observation = {'format':'ACTUAL_TARGET_ELF_OBJECT_OR_DSO'}
            else:
                # Genuine libstdc++/libgcc/libc selections can contain a GNU ld
                # script. Keep the original raw pin distinct from an ELF, and
                # require every actually opened referenced object/DSO to appear
                # in the same retained trace inside the complete roots. This
                # is no assertion about unconsumed AS_NEEDED alternatives.
                body,pin = common.body_pin(path,65536)
                need(pin == current and b'\0' not in body and
                     (b'GNU ld script' in body or re.match(rb'\s*(?:GROUP|INPUT|OUTPUT_FORMAT)\s*\(',body)),
                     'actual link input is neither target ELF/archive nor a bounded GNU ld script')
                observation = {'format':'ORIGINAL_GNU_LD_SCRIPT_RAW_PIN_NOT_ELF','scope':'Consumed trace inputs only; no guessed unconsumed references'}
            linked[str(path)] = current
            observations.append({'raw_trace_line':line,'actual_file':current,**observation})
        return list(linked.values()),observations
    try:
        guard(); source_before = common.source_state(source,frame.manifest)
        need(source_before == frame.source_before, 'actual complete source differs from Frame initial proof')
        report['source_before'] = source_before
        for relative in HELPERS:
            remember(source/relative)
        for name,path in tools.items():
            current = remember(path)
            need(builder.elf(common,Path(path).resolve(strict=True),configuration['target']) == current,
                 'all actual selected native tools must be physical target ELF executables')
        report['tools'] = {name:remember(path) for name,path in tools.items()}
        report['tool_aliases_before'],report['actual_GCC_installation'] = aliases,str(gcc)
        report['current_cgroup_before'] = common.cgroup_state(cgroup)
        owner = common.SerialRunner(out,limits,guard)
        proxy = types.SimpleNamespace(close=cleanup); frame.actors.append(proxy)
        row = command('clang23-version',[tools['cxx'],'--driver-mode=g++','--version'])
        raw,pin = common.body_pin(Path(row['log']['path']),limits['log_bytes'])
        need(pin == row['log'] and re.search(rb'\bclang version 23(?:\.|\s)',raw), 'actual selected compiler is not Clang23')
        report['clang_version_observed'] = True
        for label,relative,required,standard in CONTRACTS:
            primary = source/relative; primary_pin = remember(primary)
            need(frozen[relative] == FIXTURE_BODIES[relative] == {key:primary_pin[key] for key in ('bytes','sha256')},
                 'actual original three-exchange fixture body differs from complete current MF')
            obj,dep,binary = out/(label+'.o'),out/(label+'.d'),out/label
            flags = ['--driver-mode=g++','--target='+configuration['target']['triple'],'-std='+standard,'-O2','-g0',
                '-pthread','-fno-fast-math','-ffp-contract=off',*configuration['cxxflags'],
                '-I',str(source/'src'),'-I',str(source/'third-parties/fast_io/include'),'-UNDEBUG']
            need(not any('NDEBUG' in value and value != '-UNDEBUG' for value in flags) and flags.count('-UNDEBUG') == 1,
                 'assert-off flags are forbidden')
            if configuration['sysroot']: flags.append('--sysroot='+configuration['sysroot'])
            compiled = command(label+'-compile',[tools['cxx'],*flags,'-MD','-MF',dep,'-MT',obj,'-c',primary,'-o',obj])
            dep_pin,dependencies = builder.dependency_inputs(common,dep,obj,source,roots)
            required_names = {str(primary),*[str(source/name) for name in required]}
            need(required_names <= {value['path'] for value in dependencies}, 'actual full-MD omitted primary or real production IPC handshake header')
            for value in dependencies: remember(value['path'],value)
            object_pin = builder.elf(common,obj,configuration['target'],(1,)); need(product(obj) == object_pin, 'actual object changed')
            need(product(dep) == dep_pin, 'actual full-MD file changed')
            link = [tools['cxx'],'--driver-mode=g++','--target='+configuration['target']['triple'],*configuration['link_flags'],
                '-fuse-ld='+tools['linker'],'-pthread','-Wl,--threads=1','-Wl,--trace','-Wl,--no-undefined']
            if configuration['sysroot']: link.append('--sysroot='+configuration['sysroot'])
            link += [str(obj),'-o',str(binary)]
            linked = command(label+'-link',link); link_inputs,observations = trace(linked)
            need(object_pin in link_inputs and any(Path(value['path']).name.startswith('libstdc++') for value in link_inputs),
                 'actual link trace omitted real object or selected libstdc++')
            binary_pin = builder.elf(common,binary,configuration['target']); need(product(binary) == binary_pin and os.access(binary,os.X_OK),
                 'actual new native contract executable required')
            executed = command(label+'-run',[binary])
            need(not any((out/'tmp').iterdir()), 'actual handshake fixture left a socket or temporary directory')
            raw,log_pin = common.body_pin(Path(executed['log']['path']),limits['log_bytes'])
            need(log_pin == executed['log'] and raw == b'', 'actual production contract must exit zero with original silent stdout/stderr')
            report['contracts'].append({'name':label,'status':'PASS_ACTUAL_IPC_HANDSHAKE_EXECUTABLE_ZERO_EXIT',
                'source_defined_groups':3,'groups_count_scope':'THREE ACTUAL UNIX SOCKET EXCHANGES PER STANDARD; NOT ROS DELIVERY',
                'primary':primary_pin,'required_consumed_headers':[str(source/name) for name in required],
                'object':object_pin,'dependency_file':dep_pin,'full_MD':dependencies,'compile_argv':compiled['argv'],
                'compile_cwd':compiled['cwd'],'link_argv':link,'link_inputs':link_inputs,'link_observations':observations,
                'binary':binary_pin,'actual_exit':executed['returncode'],'actual_original_silent_output':log_pin,
                'standard':standard,'assertions':'-UNDEBUG; no assert-off compiler flag',
                'execution_command_index':len(owner.commands)-1})
            report['actual_executables_completed'] += 1; save()
        need(len(owner.commands) == 7 and report['actual_executables_completed'] == 2, 'exact seven actual serial commands/two native contracts required')
        report['all_requested_contracts_completed'] = True
    except BaseException as error:
        report.update(status='FAIL',error=repr(error),traceback=traceback.format_exc())
    finally:
        finalizing = True; cleanup_started = time.monotonic(); next_output_walk = 0.0; errors = []
        def attempt(label, action):
            try: action()
            except BaseException as error:
                errors.append({'stage':label,'error':repr(error)}); report['status'] = 'FAIL'
        def close():
            value = cleanup(); report['cleanup'] = value
            need(value['status'] == 'PASS' and not value.get('remaining') and not value.get('errors'), 'owned native contract cleanup incomplete')
            if proxy is not None: frame.actors.remove(proxy)
        attempt('owned-pidfd-cleanup',close)
        if owner is not None:
            for row in owner.commands:
                if 'log' in row: attempt('retain-original-command-log',lambda row=row:product(row['log']['path'],limits['log_bytes']))
            attempt('retain-original-command-ledger',lambda:product(out/'commands.json',32*MIB))
        def preserve():
            need(source_before is not None, 'complete initial source proof unavailable')
            report['source_after'] = common.source_state(source,frame.manifest)
            need(report['source_after'] == source_before, 'current complete frozen source changed')
            after = []
            for index,root in enumerate(frame.roots):
                guard(); current = common.catalog(root,out/('inputs-after-%02d.jsonl'%index),frame.roots)
                previous = next(value for value in frame.catalogs if value['root'] == str(root))
                need({key:value for key,value in current.items() if key != 'catalog'} ==
                     {key:value for key,value in previous.items() if key != 'catalog'}, 'complete actual Frame input root changed')
                after.append(current); product(current['catalog']['path'])
            report['whole_roots_before'],report['whole_roots_after'] = frame.catalogs,after
            need(all(common.opened(Path(value['path'])) == value for value in [*inputs.values(),*products.values()]),
                 'actual input or native product/log changed')
            need(aliases == {name:worker.aliases(path) for name,path in tools.items()}, 'tool alias changed')
            report.update(source_preserved=True,inputs_preserved=True,products_preserved=True,whole_roots_preserved=True)
            guard(); report['current_cgroup_after'] = common.cgroup_state(cgroup)
        attempt('complete-source-root-input-product-preservation',preserve)
        attempt('terminal-fresh-output-budget',budget)
        # Reserve failure remains an acceptance failure; it cannot prevent the
        # earlier owned cleanup or complete final input/source preservation.
        attempt('terminal-eight-GiB-backing-reserve',storage)
        report['finalization_errors'] = errors
        if report.get('all_requested_contracts_completed') and report['status'] == 'RUNNING' and not errors:
            report['status'] = 'PASS_ACTUAL_IPC_HANDSHAKES_CXX17_CXX26_SIX_SOCKET_CASES_ONLY'
        save()
    return report
