#!/usr/bin/env python3
"""Read-only verification of the bounded IPC client deadline evidence.

Writes only a fresh proof file. It does not compile, execute test binaries, mount,
or change original reports. Partial failures remain failures and have no inferred
postflight/OOM claim. The local protection mode reads the actual repository.
"""
import argparse
import difflib
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import sys
import traceback

B = Path('/home/macromodel/.local/share/uwvm-ros-extension')
PREFIX = '20261013-client-deadline-'
UNIX = 'src/uwvm2/imported/ros/ipc/unix.h'
OLD_UNIX = '38123c90aebc002b65e5f6f70f329b11a44ee3bff7354d2cb721e858bcc54f6e'
NEW_UNIX = 'ff6fe70858fca1ec62bb30e8d75cbdec1bf7e29eb0464ebf54afb6db135b9f8f'
UNIT = 'tests/ros_ipc_client_deadline/unit.cc'
NEW_UNIT = '44f9c66ca9a5717ffe219b1bfd17b28c00c13df8eb023bd267552ffdf73fcb1d'
UNIT_DRIVER = '326132af0c2abe6fd462285e91641dff24cacd6fd89009e19e87c7139f4ce68c'
NATIVE = 'tests/ros_ipc_client_deadline/native_ros2.cc'
NATIVE_SHA = '2a5c56d0d09af80d2af7422c3b4ef16f6ac37631433583eaa7b7df8e91f65321'
NATIVE_DRIVER = '1e3b852ab543b64535bd977336d4dca45108a9f4f4359d69e5576bee4235f175'
PROTECTED_SHA = '920375c92235c5fe2c663311c2348f199a50aa5e6019b66ce2cab39e00a785ee'
REPORTS = {
    'before-r1': ('9757f61ceeaeffa5cfc3132860b1e3b290d13d8a8a550ca09c315d9bb59c905d', 'FAIL', 3),
    'before-r2': ('45c329015fdfca95214382121b634b6934b7cc673fd0482f5f52770656d671cb', 'CONFIRMED_BEFORE_SEMANTIC_FAILURES', 6),
    'after-r1': ('0eaf32ea36d1e330fa8f8b6a66f4fa98578b7c4afa0c01e985e749a7856e901d', 'FAIL', 7),
    'after-r2': ('a58afe2b208ddc30182ed2c2959fd7d1a31da7e9c2b58f84bd79c271be271420', 'FAIL', 22),
    'after-r3': ('cde9ca5a3249e023744b1a7baa9955af7aa869fdb77609febe0b0349eb379ba4', 'PASS', 33),
}
MODES = ('queue-expired', 'queue-remaining', 'encode-expired', 'encode-remaining')
CG_LIMIT = 68719476736
checks = 0
indexed = {}
cache = {}


def need(condition, message):
    global checks
    checks += 1
    if not condition:
        raise AssertionError(message)


def digest(path):
    p = Path(path)
    st = p.stat()
    key = (str(p), st.st_dev, st.st_ino, st.st_size, st.st_mtime_ns)
    if key not in cache:
        h = hashlib.sha256()
        with p.open('rb') as stream:
            for block in iter(lambda: stream.read(1048576), b''):
                h.update(block)
        cache[key] = h.hexdigest()
    return cache[key]


def bind(path, expected=None):
    p = Path(path)
    actual = digest(p)
    need(expected is None or actual == expected, 'file SHA mismatch: ' + str(p))
    indexed[str(p)] = {'sha256': actual, 'bytes': p.stat().st_size}
    return actual


def read(path, expected=None):
    bind(path, expected)
    return json.loads(Path(path).read_bytes())


def make_words(raw):
    # GNU make escaped spaces and continuations, parsed independently of runners.
    text = raw.replace('\\\r\n', '').replace('\\\n', '')
    words, current, escaped = [], [], False
    for ch in text:
        if escaped:
            current.append(ch)
            escaped = False
        elif ch == '\\':
            escaped = True
        elif ch.isspace():
            if current:
                words.append(''.join(current)); current = []
        else:
            current.append(ch)
    need(not escaped, 'unfinished make escape')
    if current:
        words.append(''.join(current))
    need(words and words[0].endswith(':'), 'MMD missing target')
    return words[0][:-1], words[1:]


def mmd(path, source, expected_map=None, normalized=True, alias=None, expected_target=None):
    bind(path)
    target, paths = make_words(Path(path).read_text())
    need(expected_target is None or target == str(expected_target), 'MMD target mismatch')
    root = str(alias or source).rstrip('/') + '/'
    values = {}
    for p in paths:
        if not p.startswith(root):
            continue
        literal = p[len(root):]
        name = os.path.normpath(literal) if normalized else literal
        need(not name.startswith('../') and not name.startswith('/'), 'MMD escapes project')
        sha = bind(Path(source) / literal)
        need(name not in values or values[name] == sha, 'MMD alias collision')
        values[name] = sha
    need(bool(values), 'MMD contains no project inputs')
    need(expected_map is None or values == expected_map, 'MMD project input set/hash mismatch: ' + str(path))
    return {os.path.normpath(n): s for n, s in values.items()}


def cgroup_one(c):
    need(c['status'] == 'PASS' and c['effective_memory_max'] == CG_LIMIT, 'cgroup status/effective limit')
    need(c['expected_memory_max'] == CG_LIMIT, 'expected cgroup limit')
    membership = c['membership']
    need(len(membership) == 1 and membership[0].startswith('0::/'), 'unified cgroup membership')
    rows = c['cgroup_ancestors']
    leaf = Path('/sys/fs/cgroup') / membership[0][4:]
    expected_paths = []
    while True:
        expected_paths.append(str(leaf))
        if leaf == Path('/sys/fs/cgroup'):
            break
        leaf = leaf.parent
    need([x['path'] for x in rows] == expected_paths, 'complete ordered cgroup ancestors')
    limits = []
    for r in rows:
        for field in ('memory.max', 'memory.current', 'memory.peak', 'memory.events', 'cpu.max', 'pids.max', 'cpuset.cpus.effective'):
            need(field in r, 'missing ancestor field ' + field)
        need((r['memory.max'] is None) == (r['memory.events'] is None), 'readable/unavailable memory fields')
        if r['memory.max'] is not None and r['memory.max'] != 'max':
            need(int(r['memory.max']) > 0, 'invalid ancestor memory.max')
            limits.append(int(r['memory.max']))
        if r['memory.events'] is not None:
            events = dict(line.split() for line in r['memory.events'].splitlines())
            for k in ('oom', 'oom_kill', 'oom_group_kill'):
                need(k in events and int(events[k]) >= 0, 'missing OOM counter')
    need(limits and min(limits) == CG_LIMIT, 'ancestor effective memory limit')
    return rows


def cgroup_pair(doc):
    a, b = cgroup_one(doc['cgroup_before']), cgroup_one(doc['cgroup_after'])
    need(doc['cgroup_before']['membership'] == doc['cgroup_after']['membership'], 'cgroup membership changed')
    need(len(a) == len(b), 'ancestor row count changed')
    expected = []
    for x, y in zip(a, b):
        for field in ('path', 'memory.max'):
            need(x[field] == y[field], 'ancestor identity/limit changed')
        for field in ('memory.current', 'memory.peak', 'memory.events', 'cpu.max', 'pids.max', 'cpuset.cpus.effective'):
            need((x[field] is None) == (y[field] is None), 'ancestor field availability changed')
        if x['memory.events'] is None:
            continue
        old, new = [dict(line.split() for line in r['memory.events'].splitlines()) for r in (x, y)]
        delta = {k: int(new[k])-int(old[k]) for k in ('oom', 'oom_kill', 'oom_group_kill')}
        need(not any(delta.values()), 'OOM delta nonzero')
        expected.append({'path': x['path'], **delta})
    need(doc['oom_deltas'] == expected, 'OOM delta evidence incomplete')
    return {'readable_ancestors': len(expected), 'unavailable_ancestors': len(a)-len(expected)}


def command(row, expected_exit=0):
    need(row['returncode'] == expected_exit, 'actual command returncode')
    need(row['wrapper_pid'] > 0 and row['seconds'] >= 0, 'command execution fields')
    bind(row['log'], row['log_sha256'])
    return Path(row['log']).read_text()


def live(row, binary, before):
    p = row['main_proof']
    need(isinstance(p, dict), 'missing actual main ELF proof')
    need(p['pid'] == row['wrapper_pid'] and p['parent_pid'] == before['pid'], 'main PID/PPid')
    need(p['ancestry'] == [p['parent_pid'], p['pid']], 'main ancestry')
    need(p['membership'] == before['membership'] and p['live_cgroup_procs'] is True, 'main live cgroup membership')
    need(p['live_identity_captured_before_fd_hash'] is True and p['capture_monotonic_ns'] > 0, 'pinned FD sampling order')
    need(p['exe_path'] == str(binary), 'main executable path')
    st = Path(binary).stat()
    need((p['exe_device'], p['exe_inode']) == (st.st_dev, st.st_ino), 'main ELF device/inode')
    bind(binary, p['exe_sha256'])
    need(Path(binary).read_bytes()[:4] == b'\x7fELF', 'main ELF magic')


def merge(union, values):
    for n, sha in values.items():
        need(n not in union or union[n] == sha, 'compiled input versions conflict: ' + n)
        union[n] = sha


def observed(text, mode, before):
    lines = re.findall(r'^OBSERVED mode=(\S+) (.+)$', text, re.M)
    need(len(lines) == 1 and lines[0][0] == mode, 'exact unit observation mode')
    fields = {k: int(v) for k, v in (x.split('=') for x in lines[0][1].split())}
    expired, queue = mode.endswith('expired'), mode.startswith('queue-')
    need(fields['error'] == (0 if before or not expired else 110), 'unit error oracle')
    need(fields['peer_requests'] == ((2 if queue else 1) if before or not expired else (1 if queue else 0)), 'unit native peer effects')
    budget = fields['queued_wire_budget' if queue else 'wire_budget']
    need(budget == (30 if expired else 1000) if before else (budget == 0 if expired else 0 < budget <= 870), 'wire remaining budget')
    if queue:
        need(fields['actual_wait_ms'] >= 130, 'actual mutex deadline barrier')
    else:
        need(fields['allocation_hits'] == 1, 'exact calling-thread encode allocation barrier')
    markers = {'queue-expired': 'expired queued client request was transmitted',
               'queue-remaining': 'queued call reset wire timeout',
               'encode-expired': 'expired encoded client request was transmitted',
               'encode-remaining': 'encoded call reset wire timeout'}
    need(markers[mode] in text and 'FAIL ' in text if before else 'PASS client call entry deadline' in text and 'FAIL ' not in text, 'unit result marker')
    return fields


def unit_evidence():
    facts, current, histories = {}, {}, {}
    for label, (sha, status, count) in REPORTS.items():
        w = B / (PREFIX + label); out = w / 'results'
        doc = read(out / 'report.json', sha)
        need(doc['status'] == status and len(doc['commands']) == count, 'unit report status/count')
        s = Path(doc['source'])
        need(s == w/'source', 'unit frozen source path')
        bind(s/'tests/ros_ipc_client_deadline/run_unit.py', doc['executed_driver_sha256'])
        bind(s/UNIX, OLD_UNIX if label.startswith('before') else NEW_UNIX)
        cgroup_one(doc['cgroup_before'])
        if status == 'FAIL':
            need('cgroup_after' not in doc and 'oom_deltas' not in doc, 'partial FAIL must not invent postflight')
            need(doc['commands'][-1]['returncode'] != 0 and 'unexpected exit' in doc['error'], 'original actual FAIL')
            for row in doc['commands']:
                bind(row['log'], row['log_sha256'])
            histories[label] = {'report_sha256': sha, 'status': status, 'error': doc['error'], 'postflight_not_recorded': True}
            continue
        need(doc['executed_driver_sha256'] == UNIT_DRIVER, 'frozen unit driver')
        cg = cgroup_pair(doc)
        before = label == 'before-r2'
        expected_profiles = [('c++20', 'libc++')] if before else [(a,b) for a in ('c++17','c++20','c++26') for b in ('libc++','libstdc++')]
        need([(p['standard'], p['library']) for p in doc['profiles']] == expected_profiles, 'six SAN profile matrix')
        compiler = doc['commands'][0]['command'][0]
        need(doc['commands'][0]['command'] == [compiler, '--version'], 'actual compiler version argv')
        need('clang version 23' in command(doc['commands'][0]).lower(), 'actual Clang 23')
        union, observation_rows = {}, []
        at = 1
        for profile in doc['profiles']:
            std, lib = profile['standard'], profile['library']
            tag = std.replace('+','p')+'-'+lib.replace('+','p')
            binary, dep = out/tag, out/(tag+'.d')
            build = doc['commands'][at]; at += 1
            argv = [compiler, '-std='+std, '-stdlib='+lib, '-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-pie', '-no-pie', '-MMD', '-MF', str(dep), '-I', str(s/'src'), str(s/UNIT), '-pthread', '-o', str(binary)]
            need(build['command'] == argv and build['status'] == 'PASS', 'unit exact build argv/flags')
            command(build); bind(dep, build['dependency_sha256'])
            merge(union, mmd(dep, s, build['compiled_project_inputs'], normalized=False, expected_target=binary))
            bind(binary, profile['binary_sha256'])
            need(binary.read_bytes()[16:18] == b'\x02\x00', 'SAN ET_EXEC')
            need(profile['build_log_sha256'] == build['log_sha256'], 'profile build log binding')
            need([r['mode'] for r in profile['runs']] == list(MODES), 'unit mode order')
            for mode, summary in zip(MODES, profile['runs']):
                row = doc['commands'][at]; at += 1
                need(row['command'] == [str(binary), mode], 'unit mode argv')
                need(row['status'] == ('CONFIRMED_BEFORE_SEMANTIC_FAILURE' if before else 'PASS') and summary['status'] == row['status'] and summary['log_sha256'] == row['log_sha256'], 'unit summary binding')
                text = command(row, 1 if before else 0); live(row, binary, doc['cgroup_before'])
                observation_rows.append({'standard':std,'library':lib,'mode':mode,**observed(text,mode,before)})
        if not before:
            build, row = doc['commands'][at:at+2]; at += 2
            binary, dep = out/'request-view-regression', out/'request-view-regression.d'
            expected = [compiler,'-std=c++20','-stdlib=libc++','-O1','-g','-fsanitize=address,undefined','-fno-pie','-no-pie','-MMD','-MF',str(dep),'-I',str(s/'src'),str(s/'tests/ros_ipc/request_encode_view_test.cc'),'-pthread','-o',str(binary)]
            need(build['command'] == expected and row['command'] == [str(binary)], 'request-view regression exact argv')
            command(build); bind(dep,build['dependency_sha256'])
            merge(union,mmd(dep,s,build['compiled_project_inputs'],normalized=False,expected_target=binary))
            need('PASS' in command(row),'request-view actual PASS');live(row,binary,doc['cgroup_before'])
            bind(s/UNIT,NEW_UNIT)
            current.update(union)
            current['tests/ros_ipc_client_deadline/run_unit.py'] = UNIT_DRIVER
        need(at == len(doc['commands']), 'no unverified unit commands')
        literal_union = {}
        for row in doc['commands']:
            literal_union.update(row.get('compiled_project_inputs',{}))
        need(literal_union == doc['compiled_project_inputs'], 'reported unit dependency union')
        facts[label] = {'report_sha256':sha,'status':status,'commands':count,'actual_main_proofs':len(observation_rows)+(0 if before else 1),'observations':observation_rows,'cgroup':cg,'compiled_inputs':union}
    old = (B/(PREFIX+'before-r2')/'source'/UNIT).read_text()
    new = (B/(PREFIX+'after-r3')/'source'/UNIT).read_text()
    need(new.replace('#include <exception>\n','').replace('#include <stdexcept>\n','') == old, 'before/final unit fixture semantic bytes changed beyond two missing includes')
    return facts, histories, current


def protected(repo, manifest):
    values = read(manifest, PROTECTED_SHA)
    need(len(values) == 48 and all(isinstance(x,dict) for x in values.values()), 'exact formal protection set48')
    for name, expected in values.items():
        need(not name.startswith('/') and '..' not in Path(name).parts, 'protected path confinement')
        p = repo/name
        bind(p, expected['sha256']); need(p.stat().st_size == expected['bytes'], 'protected file size')
    return {'protected_manifest_sha256':PROTECTED_SHA,'actual_repository':str(repo),'protected_file_count':48,'protected_files':values,'runtime_or_remote_copy_claimed':False}


def unit_entry(current):
    w = B/(PREFIX+'unit-entry-r1'); s, out = w/'source', w/'results'
    doc = read(out/'results.json','0b87dd87b05a656dcd6c14cd33f1223158ee78690cb50e6fb2c66dbbef670106')
    launch = read(w/'launch.json','ef6c1839b4793b1e4fa877e8ad2d85947e6f0cf74c3068f87ba2d74b94fd1184')
    execution = read(w/'execution.json','9c970419809c71023337490736af38d06ec07f23196eb234eff635867c51e924')
    entry = s/'tests/ros/run_unit.py'
    bind(entry,'2b85b4e3b793aa2cc6bf939f16f6f3d3a4dad8ad05640b32977872cfdf65fec5')
    need(execution['returncode'] == 0 and execution['executed_entry_sha256'] == digest(entry),'unit entry actual execution binding')
    need(launch['command'] == ['python3',str(entry),'--suite','ipc','--out',str(out)],'unit entry launch argv')
    need(launch['environment']['ROS_TEST_CXXFLAGS'] == '-std=c++26 -stdlib=libc++ -O1 -g0 -MMD -fsanitize=address,undefined -fno-pie -no-pie','unit entry SAN flags')
    need(doc == {'status':'PASS','suites':[{'suite':'ipc','status':'PASS','returncode':0},{'suite':'ipc-client-deadline','status':'PASS','returncode':0}]},'unit entry suites unchanged plus deadline')
    old = B/'20261013-contracts-integrated-r1/source/tests/ros/run_unit.py'
    bind(old,'2a9184ce50605f6d42e37fd027d6e85cb3989c89b0e16c68ed88b75a72920a6c')
    revised = old.read_text().replace("['security', 'ipc', 'sdk', 'all']","['security', 'ipc', 'ipc-client-deadline', 'sdk', 'all']").replace("('ipc', root/'tests/ros_ipc/protocol_test.cc'), ('sdk'","('ipc', root/'tests/ros_ipc/protocol_test.cc'), ('ipc-client-deadline', root/'tests/ros_ipc_client_deadline/unit.cc'), ('sdk'").replace("if a.suite not in ('all', name):","if a.suite not in ('all', name) and not (a.suite=='ipc' and name=='ipc-client-deadline'):")
    need(revised == entry.read_text(),'unit entry exact three bounded mutations only')
    union = {}
    for name, cpp in [('ipc','tests/ros_ipc/protocol_test.cc'),('ipc-client-deadline',UNIT)]:
        merge(union,mmd(out/(name+'.d'),s,expected_target=out/name))
        need(cpp in union,'unit entry actual CPP dependency')
        for suffix in ('.build.log','.log'):
            bind(out/(name+suffix))
        bind(out/name)
    need('PASS Linux SO_PEERCRED' in (out/'ipc.log').read_text(),'old protocol actual marker')
    text = (out/'ipc-client-deadline.log').read_text()
    for mode in MODES:
        line = next(x for x in text.splitlines() if x.startswith('OBSERVED mode='+mode+' '))
        observed(line+'\nPASS client call entry deadline\n',mode,False)
    bind(s/UNIT,NEW_UNIT);bind(s/UNIX,NEW_UNIX)
    merge(current,union);current['tests/ros/run_unit.py']=digest(entry)
    return {'status':'PASS','report_sha256':digest(out/'results.json'),'suite_count':2,'MMD_count':2,'main_proof_claimed':False,'hosted_CI_execution_claimed':False,'compiled_inputs':union}


def workers(current):
    w1,w2 = [B/(PREFIX+x) for x in ('workers-r1','workers-r2')]
    old = read(w1/'report.json','a2507652dee90bd612a9017b78f2d9c06d139c6d21cc224083e0e0461e8332b3')
    doc = read(w2/'report.json','4cf78b94cdf67ae93fb323ef56c909295bb8c9811d39913fcc456de068f2c6f3')
    need(old['status']=='FAIL' and 'boost/unordered/../container_hash/detail/../hash_fwd.hpp' in old['error'],'original Worker wrapper failure retained')
    need('cgroup_after' not in old and 'oom_deltas' not in old,'failed Worker wrapper lacks postflight; no zero OOM claim')
    need(doc['status']=='PASS' and len(doc['commands'])==5,'reuse driver actual command count')
    d1,d2 = [B/(PREFIX+'worker-inputs-'+x)/'run_product.py' for x in ('r1','r2')]
    bind(d1,'dc8a4f715b8e51631f034af76c5aff008d4f3c2d4ece4c18a17df978c285b340')
    bind(d2,'3c0d04d92f89074d187953d75473b0f859253265be4b148af3039ae0bf9971dd')
    need(old['executed_driver_sha256']==digest(d1) and doc['executed_driver_sha256']==digest(d2),'actual Worker wrapper revisions')
    s=Path(doc['source']);build=w1/'build'; frozen=build/'source'
    need(s==B/(PREFIX+'after-r3')/'source' and old['source']==str(s),'Worker frozen source identity')
    raw=read(build/'results.json','3194cf3d18e2ebad29b923b18a972b86a221cfa3500042b90968dedacaed2e46')
    manifest=read(build/'source-manifest.json','b76b7a6c433330213c90bc6bfbf37952727d6496c3cd1cd5b08de6502dd488b9')
    need(raw['status']=='PASS' and len(raw['profiles'])==4,'four actual raw builds PASS')
    need(doc['reused_build']=={'path':str(build),'original_driver_report_sha256':digest(w1/'report.json'),'raw_build_report_sha256':digest(build/'results.json'),'source_manifest_sha256':digest(build/'source-manifest.json'),'scope':'Reuse four actual successful builds; earlier driver failure retained unchanged'},'reuse full origin binding')
    for n,sha in manifest.items():
        bind(frozen/n,sha);bind(s/n,sha)
    budget=read(build/'build-budget.json')
    need(budget['memory_max']==CG_LIMIT and budget['membership']==old['cgroup_before']['membership'] and budget['jobs']==1,'raw build actual cgroup budget')
    need(len(old['commands'])==2 and old['commands'][1]['command']==['/usr/bin/python3',str(s/'tests/ros/build_product.py'),'--profile','all','--out',str(build)],'original actual four-build invocation')
    for row in old['commands']:
        command(row);need(row['main_proof'] is None,'original wrapper does not prove runtime main')
    need(Path(old['commands'][1]['log']).read_text().splitlines() == ['PASS none', 'PASS ros1', 'PASS ros2', 'PASS dual'], 'exact four actual original build PASS lines')
    cgroup_one(old['cgroup_before']);cg=cgroup_pair(doc)
    compiler=doc['commands'][0]['command'][0]
    need('clang version 23' in command(doc['commands'][0]).lower(),'Worker actual compiler version')
    profiles=('none','ros1','ros2','dual');union={};pids=[];tests=0
    need([p['profile'] for p in raw['profiles']]==list(profiles) and [p['profile'] for p in doc['profiles']]==list(profiles),'Worker profile ordering')
    regression_names=('wasi_preview1_compile','wasi_preview1','wasi_memory64_compile','wasi_memory64','runtime_tail_compile','runtime_tail','trusted_dl_compile','trusted_dl_guest_compile','trusted_dl')
    for index,(profile,p,q) in enumerate(zip(profiles,raw['profiles'],doc['profiles']),1):
        need(p['status']=='PASS' and p['compile_returncodes']==[0,0,0] and p['link_returncode']==p['version_returncode']==0,'raw actual compile/link/version returncodes')
        need(all(q[k]==v for k,v in p.items()),'reuse preserves actual raw build metadata')
        binary=build/profile/'uwvm';bind(binary,p['binary_sha256'])
        cmds=read(build/profile/'commands.json');processes=read(build/profile/'compiler-processes.json')
        need(len(cmds)==4 and len(processes)==3,'12 compilation commands and PID rows')
        need([x['translation_unit'] for x in processes]==['runtime','main','host_api'],'actual compiler TU ordering')
        for tu,c,proc in zip(('runtime','main','host_api'),cmds[:3],processes):
            need(c[0]==compiler and all(x in c for x in ('-std=c++26','-stdlib=libc++','-O1','-g0','-MMD','-DUWVM_DISABLE_JIT')),'Worker unchanged compiler/flags')
            need(c[c.index('-o')+1]==str(build/profile/(tu+'.o')),'Worker compile output')
            need(proc['pid']>0 and proc['membership']==old['cgroup_before']['membership'],'actual compiler PID membership')
            pids.append(proc['pid']);bind(build/profile/(tu+'.log'));bind(build/profile/(tu+'.o'))
        need(cmds[-1][cmds[-1].index('-o')+1]==str(binary),'Worker actual link output');bind(build/profile/'link.log');bind(build/profile/'version.log')
        need('version' in (build/profile/'version.log').read_text().lower(),'Worker --version executed log')
        need(set(q['MMD'])=={'runtime.d','main.d','host_api.d'},'three exact Worker MMDs')
        for dep,record in q['MMD'].items():
            path=build/profile/dep;bind(path,record['sha256']);need(path.stat().st_size==record['bytes'],'Worker MMD bytes')
            values=mmd(path,frozen,record['compiled_project_inputs'],expected_target=build/profile/(Path(dep).stem+'.o'))
            need(all(manifest[n]==sha for n,sha in values.items()),'actual compiled inputs match full frozen build inventory')
            merge(union,values)
        row=doc['commands'][index];reg=w2/(profile+'-regressions')
        need(row['command']==['/usr/bin/python3',str(s/'tests/ros/regression.py'),'--uwvm',str(binary),'--out',str(reg)],'actual regression argv uses original fresh ELF')
        text=command(row);need(row['main_proof'] is None,'regression wrapper supplies no pinned runtime main proof')
        rd=read(reg/'results.json',q['regression_report_sha256']);rc=read(reg/'commands.json')
        need(rd['status']=='PASS' and len(rd['tests'])==9 and q['regression_tests']==rd['tests'],'nine actual per-profile tests')
        need([r['name'] for r in rd['tests']]==list(regression_names) and len(rc)==9,'exact bounded regression subset')
        for r,c in zip(rd['tests'],rc):
            name=r['name'];need(r['status']=='PASS' and r['returncode']==0 and 'PASS '+name in text,'actual regression row/log status')
            bind(reg/(name+'.stdout.log'));bind(reg/(name+'.stderr.log'))
            if not name.endswith('_compile'):
                need(c[0]==str(binary),'regression runtime ELF binding')
            else:
                need(c[0] in ('wat2wasm','cc'),'regression compiler command')
            if name=='wasi_preview1':need((reg/(name+'.stdout.log')).read_bytes()==b'WASI\n','WASI exact output')
            tests+=1
    need(len(pids)==len(set(pids))==12,'distinct actual compiler PIDs')
    need(doc['compiled_project_inputs']==union,'Worker exact MMD union')
    merge(current,{n:sha for n,sha in union.items() if n.startswith('src/uwvm2/imported/ros/')})
    current['tests/ros_ipc_client_deadline/run_product.py']=digest(d2)
    return {'status':'PASS','wrapper_failure_sha256':digest(w1/'report.json'),'wrapper_failure_oom_after_not_recorded':True,'raw_build_report_sha256':digest(build/'results.json'),'completion_report_sha256':digest(w2/'report.json'),'actual_fresh_build_workspace':str(w1),'completion_workspace':str(w2),'source_manifest_files':len(manifest),'actual_fresh_profiles':4,'compile_PIDs':pids,'MMD_count':12,'compiled_inputs_count':len(union),'regression_count':tests,'recompiled_by_completion_driver':False,'A15_claimed':False,'runtime_main_proof_claimed_for_regressions':False,'cgroup':cg,'compiled_ROS_inputs':{n:sha for n,sha in union.items() if n.startswith('src/uwvm2/imported/ros/')}}


def native_evidence(w,expected_sha,phase,current):
    p=w/'reports';s=w/'source';t=w/'native-temp';r=w/'rootfs'
    doc=read(p/'report.json',expected_sha)
    need(doc['phase']==phase and doc['workspace']==str(w),'native phase/workspace')
    need(doc['status']==('PASS' if phase=='after' else 'CONFIRMED_BEFORE_SEMANTIC_FAILURES'),'native aggregate retains actual before semantics')
    bind(w/'executed-driver.py',NATIVE_DRIVER);need(doc['executed_driver_sha256']==NATIVE_DRIVER,'actual native driver SHA')
    need(len(doc['commands'])==7,'native seven actual compile/link/test commands')
    bind(s/NATIVE,NATIVE_SHA);bind(s/UNIX,NEW_UNIX if phase=='after' else OLD_UNIX)
    frozen=read(p/'source-before.json')
    for n,sha in frozen.items():bind(s/n,sha)
    need(doc['source_preserved'] is True and all(doc['baseline_preserved'].values()),'native source/baseline guards')
    argv=doc['argv'];options=dict(zip(argv[1::2],argv[2::2]))
    need(set(options)=={'--workspace','--baseline','--sdk','--inputs','--phase'} and options['--workspace']==str(w) and options['--phase']==phase,'native actual launch options')
    base=Path(options['--baseline']);inputs=Path(options['--inputs'])
    bind(argv[0],NATIVE_DRIVER)
    bind(base/'reports/report.json',doc['baseline_report_sha256'])
    for n,sha in doc['baseline_source_inputs'].items():bind(base/'source'/n,sha)
    need(len(doc['baseline_18_native_binaries'])==18,'18 prior native ELF guard')
    for n,sha in doc['baseline_18_native_binaries'].items():bind(base/'native-binaries'/n,sha)
    overlays={NATIVE} if phase=='before' else {NATIVE,UNIX,'gateways/common/server.h','gateways/common/native_io.h','gateways/ros2/resources.h'}
    need(set(doc['overlay_inputs'])==overlays,'native exact explicit overlay paths')
    for n,sha in doc['overlay_inputs'].items():bind(s/n,sha);bind(inputs/n,sha)
    baseline_delta={n for n in set(frozen)|set(doc['baseline_source_inputs']) if frozen.get(n)!=doc['baseline_source_inputs'].get(n)}
    need(baseline_delta<=overlays,'native frozen production/probe changes confined to declared overlays')
    cg=cgroup_pair(doc)
    need(doc['mount_cleanup']==[{'returncode':0,'output':''}],'native first cleanup success')
    need(doc['remaining_owned_mounts']==[] and doc['remaining_owned_processes']==[] and doc['unavailable_process_samples']==[],'native no owned residue/no unavailable samples')
    prefix=['/usr/bin/chroot',str(r),'/usr/bin/env','-u','CC','-u','CXX','-u','LD_LIBRARY_PATH','PYTHONDONTWRITEBYTECODE=1','ROS_DOMAIN_ID=196','RMW_IMPLEMENTATION=rmw_fastrtps_cpp','ROS_SECURITY_ENABLE=false','ROS_LOG_DIR=/tmp/ros-logs','/bin/bash','-lc','source /opt/ros/jazzy/setup.bash && exec "$@"','client-entry-deadline']
    names=['compiler-version','gateway-compile','gateway-link','native-client-compile','native-client-link','native-expired','native-remaining']
    need([x['name'] for x in doc['commands']]==names,'native actual command order')
    flags={}
    bind(p/'flags.make');bind(p/'link.txt')
    for line in (p/'flags.make').read_text().splitlines():
        if ' = ' in line:
            k,v=line.split(' = ',1);flags[k]=shlex.split(v)
    link_template=shlex.split((p/'link.txt').read_text())
    union={};observations=[]
    for row in doc['commands']:
        name=row['name'];need(row['argv'][:len(prefix)]==prefix,'native exact chroot/RMW/domain environment')
        bind(row['log'],row['log_sha256']);text=Path(row['log']).read_text()
        pre=read(p/(name+'-preexec.json'),row['preexec_sha256']);cgroup_one(pre)
        need(pre['pid']==row['wrapper_pid'] and pre['membership']==doc['cgroup_before']['membership'],'native actual preexec PID/cgroup')
        suffix=row['argv'][len(prefix):]
        expected_rc=1 if phase=='before' and name=='native-expired' else 0
        need(row['returncode']==row['expected_returncode']==expected_rc and row['status']==('EXPECTED_SEMANTIC_FAIL' if expected_rc else 'PASS'),'native actual returncode/status')
        if name=='compiler-version':need(suffix==['/usr/bin/c++','--version'] and '13.' in text,'native GCC13 actual version');continue
        if name.endswith('-compile'):
            label=name[:-8];cpp='gateways/ros2/main.cc' if label=='gateway' else NATIVE
            need(suffix==['/usr/bin/c++',*flags['CXX_DEFINES'],*flags['CXX_INCLUDES'],'-std=gnu++17','-O1','-g0','-MMD','-MF','/tmp/'+label+'.d','-c','/work/'+cpp,'-o','/tmp/'+label+'.o'],'native exact compile argv')
            continue
        if name.endswith('-link'):
            label=name[:-5];expected=list(link_template);expected[expected.index('CMakeFiles/uwvm-ros2-gateway.dir/main.cc.o')]='/tmp/'+label+'.o';expected[expected.index('-o')+1]='/tmp/'+label
            need(suffix==expected,'native exact official SDK link template');continue
        mode=name[7:];need(suffix==['/tmp/native-client','/tmp/gateway',mode],'native exact mode argv')
        found=re.findall(r'^OBSERVED native-client mode=(\S+) error=(\d+) native_quick=(\d+) futex_wait=(\d+) elapsed_ms=(\d+)$',text,re.M)
        need(len(found)==1 and found[0][0]==mode,'native exact DDS observation')
        _,error,effects,futex,elapsed=found[0];error,effects,futex,elapsed=map(int,(error,effects,futex,elapsed))
        need(futex==1 and error==(110 if phase=='after' and mode=='expired' else 0) and effects==(0 if phase=='after' and mode=='expired' else 1),'native error/actual DDS effect oracle')
        if mode=='expired':need(elapsed>=150,'native bounded actual mutex deadline barrier')
        marker='expired queued native request reached DDS' if expected_rc else ('PASS IPC client short queued deadline, native effects0, same-session exact retry count1' if mode=='expired' else 'PASS IPC client long queued budget, actual mutex wait, exact native response count1')
        need(marker in text and (expected_rc!=0 or 'FAIL ' not in text),'native expected semantic marker or after exact response/session marker')
        need(set(row['actual_mains'])=={str(t/'gateway'),str(t/'native-client')},'native probe and Gateway actual main witnesses')
        for binary,proof in row['actual_mains'].items():
            path=Path(binary);st=path.stat();cgroup_one(proof)
            need(proof['membership']==doc['cgroup_before']['membership'] and proof['live_cgroup_procs'] is True,'native actual ELF live membership')
            need(proof['ancestry'][0]==row['wrapper_pid'] and proof['ancestry'][-1]==proof['pid'] and len(set(proof['ancestry']))==len(proof['ancestry']),'native actual PPid ancestry')
            need(proof['parent_pid']==(doc['cgroup_before']['pid'] if len(proof['ancestry'])==1 else proof['ancestry'][-2]),'native actual immediate PPid')
            need((proof['device'],proof['inode'])==(st.st_dev,st.st_ino),'native actual ELF device/inode')
            need(proof['live_identity_captured_before_fd_hash'] is True and proof['capture_monotonic_ns']>0,'native pinned FD live capture before hashing')
            need(proof['exe']==str(r/'tmp'/path.name),'native actual mounted ELF path')
            bind(path,proof['sha256']);need(proof['sha256']==doc['binary_sha256'][path.name],'native binary manifest match')
        observations.append({'mode':mode,'error':error,'native_quick':effects,'futex_wait':futex,'elapsed_ms':elapsed})
    need(set(doc['raw_MMD'])=={'gateway','native-client'},'two fresh native MMDs')
    for label,record in doc['raw_MMD'].items():
        dep=t/(label+'.d');need(record['path']==str(dep) and record['bytes']==dep.stat().st_size,'native MMD actual path/size');bind(dep,record['sha256'])
        merge(union,mmd(dep,s,record['project_inputs'],alias='/work',expected_target='/tmp/'+label+'.o'))
        bind(t/label,doc['binary_sha256'][label])
        need((NATIVE if label=='native-client' else 'gateways/ros2/main.cc') in record['project_inputs'],'native actual TU dependency')
    if phase=='after':merge(current,union)
    current[NATIVE]=NATIVE_SHA;current['tests/ros_ipc_client_deadline/run_native.py']=NATIVE_DRIVER
    return {'status':doc['status'],'report_sha256':digest(p/'report.json'),'phase':phase,'commands':7,'actual_main_proofs':4,'MMD_count':2,'observations':observations,'compiled_inputs':union,'frozen_source_files':len(frozen),'changed_baseline_inputs':sorted(baseline_delta),'cgroup':cg,'first_cleanup_returncode':0,'owned_mounts':0,'owned_processes':0,'RMW_scope':'Jazzy rmw_fastrtps_cpp domain196 Trigger services'}


def main():
    global B
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode',choices=('protected','closure'),required=True)
    parser.add_argument('--base',type=Path,default=B)
    parser.add_argument('--repo',type=Path)
    parser.add_argument('--protected-manifest',type=Path)
    parser.add_argument('--native-after',type=Path)
    parser.add_argument('--native-after-report-sha256')
    parser.add_argument('--out',type=Path,required=True)
    a = parser.parse_args(); B = a.base
    if a.out.exists():
        parser.error('proof output must be new; previous actual failure/proof is preserved')
    result = {'status':'FAIL','scope':'Bounded shared IPC client deadline evidence; no A15 or hosted CI claim','executed_verifier_sha256':digest(__file__),'argv':sys.argv,'evidence_files':indexed}
    try:
        if a.mode == 'protected':
            need(a.repo is not None and a.protected_manifest is not None,'local protection requires actual repo and manifest')
            result.update(protected(a.repo.resolve(),a.protected_manifest))
        else:
            result['unit'], result['preserved_unit_failures'], result['current_scoped_inputs'] = unit_evidence()
            result['unit_entry'] = unit_entry(result['current_scoped_inputs'])
            result['workers'] = workers(result['current_scoped_inputs'])
            result['native_before'] = native_evidence(B/(PREFIX+'native-before-r1'),'2bf38f0f0e51a9490b65f3454dc43e50b94262a8c16a4842781e987692a7880e','before',result['current_scoped_inputs'])
            if a.native_after:
                need(bool(a.native_after_report_sha256),'actual after report must be explicitly SHA-bound')
                result['native_after'] = native_evidence(a.native_after,a.native_after_report_sha256,'after',result['current_scoped_inputs'])
            result['native_after_verified'] = bool(a.native_after)
        result['status']='PASS'
    except Exception as error:
        result.update(error=str(error),traceback=traceback.format_exc())
    result['checks']=checks
    a.out.parent.mkdir(parents=True,exist_ok=True)
    a.out.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'checks':checks,'proof':str(a.out),'proof_sha256':digest(a.out),'error':result.get('error')}))
    return 0 if result['status']=='PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
