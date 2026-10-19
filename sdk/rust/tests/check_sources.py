#!/usr/bin/env python3
"""Check Rust/TinyGo registry bindings and source contracts without compiling.

The checks are source/model evidence only. They cannot validate compiler-emitted
Wasm, native ROS behavior, guest quotas or runtime/cgroup process ancestry.
"""
from pathlib import Path
import hashlib
import json
import os
import re
import stat
import sys
import tomllib
import types
R=Path(__file__).resolve().parents[3]
sys.dont_write_bytecode=True
# These reviewed helper sources belong to this check's source closure. The
# enclosing Linux launcher must admit this entry, interpreter/startup/stdlib,
# and all repository inputs before execution. These pins do not attest that
# launcher or make this script a sandbox. Do not import a local helper through
# sys.path: even -B can read a pre-existing, valid but unrelated .pyc.
HELPER_PINS = {
    'sdk/tools/abi_contract.py': (17812, '879acb7d4cdfe7278b74078791c53a3a840a94058249780fbcf276a464da0c54'),
    'sdk/tools/go_build_constraints.py': (5895, '1f69bcd2ba3bf5d07a23265925c655064e7c1cd7ebca3492ab0bb2d8b54961d3'),
}
SOURCE_FIELDS = ('st_dev', 'st_ino', 'st_mode', 'st_uid', 'st_gid',
                 'st_size', 'st_mtime_ns', 'st_ctime_ns')
helper_sources = []


def captured_helper(relative, name):
    """Execute exactly one bounded pinned source capture, never a local pyc.

    Keep its no-follow regular descriptor until evaluation finishes. Match the
    named file before and afterwards too; path aliases are not a root-security
    assertion. Reviewed helpers import only the externally admitted stdlib.
    """
    size, expected = HELPER_PINS[relative]
    path = R / relative
    flags = os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC
    descriptor = os.open(path, flags)
    try:
        first = os.fstat(descriptor)
        named = os.stat(path, follow_symlinks=False)
        identity = tuple(getattr(first, field) for field in SOURCE_FIELDS)
        if (not stat.S_ISREG(first.st_mode) or first.st_size != size or
                tuple(getattr(named, field) for field in SOURCE_FIELDS) != identity):
            raise ValueError('ordinary exact-size helper source required: ' + relative)
        blocks = []
        count = 0
        while True:
            block = os.read(descriptor, min(65536, size + 1 - count))
            if not block:
                break
            count += len(block)
            if count > size:
                raise ValueError('helper source grew: ' + relative)
            blocks.append(block)
        source = b''.join(blocks)
        if len(source) != size or hashlib.sha256(source).hexdigest() != expected:
            raise ValueError('reviewed helper source differs: ' + relative)
        if name in sys.modules:
            raise ValueError('fresh helper module name required: ' + name)
        module = types.ModuleType(name)
        module.__file__ = str(path)
        module.__package__ = ''
        sys.modules[name] = module
        try:
            exec(compile(source, str(path), 'exec', dont_inherit=True, optimize=0), module.__dict__)
            if sys.modules.get(name) is not module:
                raise ValueError('helper replaced its module binding: ' + relative)
            for observed in (os.fstat(descriptor), os.stat(path, follow_symlinks=False)):
                if tuple(getattr(observed, field) for field in SOURCE_FIELDS) != identity:
                    raise ValueError('helper source changed during evaluation: ' + relative)
        finally:
            sys.modules.pop(name, None)
        helper_sources.append({'path': relative, 'bytes': size, 'sha256': expected,
                               'stat': {field: str(getattr(first, field)) for field in SOURCE_FIELDS},
                               'loading': 'same captured source bytes; no local import or pyc'})
        return module
    finally:
        os.close(descriptor)


abi_helper = captured_helper('sdk/tools/abi_contract.py', '_uwvm_rust_check_abi_source')
validate_backend_source = captured_helper('sdk/tools/go_build_constraints.py',
                                         '_uwvm_rust_check_go_constraints_source').validate_backend_source
schema_path=R/'sdk/abi/abi-v1.json'
schema=json.loads(schema_path.read_text())
checks=[]
def check(name,condition,detail=''):
    checks.append({'name':name,'pass':bool(condition),'detail':detail})
# The original81 catalog keeps its historical whole-Host hash. The extractor
# accepts a current Host only after checking the optional catalog's full actual
# Host binding, the exact historical catalog bytes and original module bodies,
# both optional three-function registries and every original81 metadata field.
# A changed current Host without that complete attestation still fails closed.
try:
    current_schema = abi_helper.extract(R)
except (OSError, UnicodeError, ValueError, KeyError, TypeError) as error:
    check('schema/source-binding',False,str(error))
else:
    check('schema/source-binding',current_schema == schema,
          'exact original81 plus separately attested current optional Host; historical catalog retained')
for v in (1,2):
    module='uwvm_ros'+str(v)+'_v1'
    expected=schema['modules'][module]['functions']
    rust_path=R/f'sdk/rust/src/raw/ros{v}.rs'
    go_path=R/f'sdk/tinygo/raw_ros{v}.go'
    rust=rust_path.read_text();go=go_path.read_text()
    rdecls={m.group(1):[(t,n) for n,t in re.findall(r'(\w+)\s*:\s*(u32|u64)',m.group(2))] for m in re.finditer(r'pub fn\s+(\w+)\s*\((.*?)\)\s*->\s*u32\s*;',rust,re.S)}
    gdecls={m.group(1):re.findall(r'(\w+)\s+(uint32|uint64)',m.group(2)) for m in re.finditer(r'//go:wasmimport '+module+r' (\w+)\nfunc\s+\w+\s*\((.*?)\)\s+uint32',go,re.S)}
    check(module+'/rust-exact-names',set(rdecls)=={f['name'] for f in expected})
    check(module+'/go-exact-names',set(gdecls)=={f['name'] for f in expected})
    check(module+'/rust-module',f'wasm_import_module = "{module}"' in rust)
    # Check the complete Boolean meaning, including the real tinygo.wasm target
    # and the both/neither-backend refusals. A matching substring is not enough.
    try:
        constraint=validate_backend_source(go,v)
    except ValueError as error:
        check(module+'/go-exclusive-tags',False,str(error))
    else:
        check(module+'/go-exclusive-tags',constraint['assignments_checked']==32)
    for f in expected:
        name=f['name'];params=f['params'];rp=[('u32' if p['type']=='i32' else 'u64',p['name']) for p in params];gp=[(p['name'],'uint32' if p['type']=='i32' else 'uint64') for p in params]
        check(module+'/'+name+'/rust-widths',rdecls.get(name)==rp)
        check(module+'/'+name+'/go-widths',gdecls.get(name)==gp)
        check(module+'/'+name+'/rust-safety-doc',bool(re.search(r'/// # Safety\s*\n\s*///[^\n]+\n\s*pub fn '+name,rust)))
        check(module+'/'+name+'/go-doc',bool(re.search(r'// Raw\w+ calls '+name+r' in '+module,go)))
        if f['form']=='rpc':
            variant=''.join(w.title() for w in name.split('_'))
            cap=(R/'sdk/rust/src/capability.rs').read_text()
            check(module+'/'+name+'/rust-dispatch',f'raw::ros{v}::{name}(h, 0, a, n, b, c, z)' in cap)
            check(module+'/'+name+'/go-dispatch',f'case Op{variant}:' in go)
manifest=tomllib.loads((R/'sdk/rust/Cargo.toml').read_text())
check('rust/no-default-backend',manifest['features']['default']==[])
check('rust/no-crate-dependencies','dependencies' not in manifest)
lib=(R/'sdk/rust/src/lib.rs').read_text()
check('rust/no-std','#![no_std]' in lib)
check('rust/docs-required-at-compile','#![deny(missing_docs)]' in lib)
cap=(R/'sdk/rust/src/capability.rs').read_text()
check('rust/no-clone-capability','pub struct Capability' in cap and not re.search(r'#\[derive\([^\]]*(Clone|Copy)[^\]]*\)\]\s*pub struct Capability',cap))
retire=cap.split('fn retire(&mut self)')[1].split('/// Retire ownership')[0];close=cap.split('pub fn close(&mut self)')[1].split('/// Revoke the alias')[0]
check('rust/close-clears-before-import','self.handle.take()' in retire and 'state.handle.take()' in retire and close.index('let h = self.retire()?')<close.index('capability_close, h'))
check('rust/drop-only-live','if self.handle.is_some()' in cap)
registry=(R/'sdk/rust/src/session.rs').read_text();node=(R/'sdk/rust/src/node.rs').read_text()
check('rust/session-no-grant-or-native-creation','Capability::open(backend, alias)?' in registry and 'backend.abi_version() != 1' in registry and 'maximum_resources > 1024' in registry)
check('rust/session-retire-before-close',registry.index('state.handle.take()')<registry.index('state.backend.close_handle(handle)') and 'self.closed.set(true)' in registry)
check('rust/shared-callback-lease-before-retirement',retire.index('crate::callback::idle(self.owner_busy())?')<retire.index('self.handle.take()') and 'return &state.callback_busy' in cap)
group_close=registry.split('fn close(&self)')[1].split('pub(crate) fn states_idle')[0]
node_close=node.split('pub fn close(&self) -> alloc::vec::Vec')[1].split('/// Disable factories locally')[0]
check('rust/group-busy-preflight-preserves-owner-and-factories',group_close.index('states_idle(&entries)')<group_close.index('self.closed.set(true)') and node_close.index('crate::session::states_idle(&entries)')<node_close.index('self.closed.set(true)'))
check('rust/session-refuses-factory-after-close','self.closed.get()' in registry and 'registry.open(self.backend,alias)?' in node)
check('rust/session-does-not-clone-public-capability','Rc<HandleState>' in registry and 'derive(Clone' not in cap.split('pub struct Capability')[0][-100:])
check('rust/typed-resource-checked-lifetimes','checked_owner!(Publisher, Subscription, Client, ActionClient, Parameters, Graph, Clock, Logger, Tf, Lifecycle)' in node and 'first.and(second)' in node)
check('rust/executor-checked-close-and-remove','pub fn close(&mut self)' in node and 'pub fn remove(&mut self, index: usize)' in node and 'if self.spinning.replace(true)' in node)
callback=(R/'sdk/rust/src/callback.rs').read_text()
check('rust/service-callback-owned-id-before-response','output[..8]!=request.id.raw().to_le_bytes()' in callback and 'codec::decode_service_response(&output[8..used])?' in callback and 'crate::callback::service_once(&self.spinning' in node)
check('rust/sentinel-output','u64::MAX.to_le_bytes()' in cap and 'n != u64::MAX' in cap)
check('rust/wasm32-explicit-rejection','target_pointer_width = "32"' in cap and 'codes::UNSUPPORTED' in cap)
executor=(R/'sdk/tinygo/executor.go').read_text()
check('go/executor-reentry-before-scratch','if x.spinning {' in executor and 'return 0, Error{Code: Invalid}' in executor)
check('go/executor-deferred-guard-normal-return','defer func() { x.spinning = false }()' in executor)
check('go/executor-ignores-busy-and-nil-registration','x.spinning || source == nil || callback == nil' in executor)
check('go/executor-malformed-callback-before-poll',executor.index('entry.callback == nil') < executor.index('entry.source.Poll'))
check('go/executor-close-busy-before-retirement',executor[executor.index('func (x *Executor) Close'):].index('if x.spinning') < executor[executor.index('func (x *Executor) Close'):].index('entry.source.Close()'))
gocap=(R/'sdk/tinygo/capability.go').read_text()
check('go/shared-retirement-state','state *capabilityState' in gocap or 'state *capabilityState' in gocap.replace('\t',' '))
goowner=(R/'sdk/tinygo/owner_state.go').read_text();gonode=(R/'sdk/tinygo/node.go').read_text();gogroup=(R/'sdk/tinygo/session.go').read_text()
check('go/close-clears-before-import','retireOwner(c.state' in gocap and 'RawCapabilityClose(handle)' in gocap and goowner.index('state.live=false')<goowner.index('return notify(handle)'))
check('go/shared-callback-lease-before-retirement',goowner.index('callbackIdle(state.callback)')<goowner.index('state.live=false') and 'events.state.callback=state; respond.state.callback=state' in gonode)
check('go/group-busy-preflight-preserves-owner-and-factories',gonode[gonode.index('func (n *Node) Close'):].index('ownersIdle(n.states)')<gonode[gonode.index('func (n *Node) Close'):].index('n.closed = true') and gogroup[gogroup.index('func (s *Session) Close'):].index('ownersIdle(s.states)')<gogroup[gogroup.index('func (s *Session) Close'):].index('s.closed = true'))
check('go/pointer-width-rejection','unsafe.Sizeof(uintptr(0)) != 4' in gocap)
check('go/no-automatic-finalizer','SetFinalizer' not in gocap)
check('go/keeps-memory-live','runtime.KeepAlive(request)' in gocap and 'runtime.KeepAlive(output)' in gocap)
check('go/request-rejects-overlap','a < b+uint64(len(output))' in gocap)
check('go/sentinel-output','255, 255, 255, 255, 255, 255, 255, 255' in gocap and 'n != ^uint64(0)' in (R/'sdk/tinygo/types.go').read_text())
check('go/explicit-selection-error','select_exactly_one_of_ros1_or_ros2' in (R/'sdk/tinygo/selection_error.go').read_text())
coverage=(R/'sdk/examples/rust/coverage.rs').read_text()
for version in (1,2):
    go=(R/f'sdk/examples/tinygo/coverage/probe_ros{version}.go').read_text()
    rblock=coverage.split(f'#[cfg(feature = "ros{version}")]')[1].split('        };')[0]
    expected=schema['modules'][f'uwvm_ros{version}_v1']['functions']
    check(f'coverage/rust/{version}/exact-source-inventory',set(re.findall(r'imports::(\w+)\(',rblock))=={f['name'] for f in expected})
    check(f'coverage/go/{version}/exact-source-inventory',set(re.findall(r'ros\.(Raw\w+)\(',go))=={'Raw'+''.join(x.title() for x in f['name'].split('_')) for f in expected})
check('coverage/rust/empty-main','fn main()' in coverage and 'Compile/import inspection only. No raw ROS probe is invoked.' in coverage)
check('coverage/go/export-directive','//go:wasmexport roswasm_abi_coverage' in (R/'sdk/examples/tinygo/coverage/main.go').read_text())
for stage in ('raw','typed','node','resources'):
    check('examples/rust/'+stage,(R/f'sdk/examples/rust/{stage}.rs').is_file())
    check('examples/tinygo/'+stage,(R/f'sdk/examples/tinygo/{stage}/main.go').is_file())
wire=json.loads((R/'sdk/abi/wire-vectors-v1.json').read_text())
rtest=(R/'sdk/rust/tests/wire_vectors.rs').read_text();gtest=(R/'sdk/tinygo/wire_vectors_test.go').read_text()
for v in wire['positive']:
    check('fixture/'+v['name']+'/rust',v['hex'] in rtest)
    check('fixture/'+v['name']+'/go',v['hex'] in gtest)
for i,v in enumerate(wire['negative']):
    check('negative/'+str(i)+'/rust',v['hex'] in rtest)
    check('negative/'+str(i)+'/go',v['hex'] in gtest)
# Public Go functions/types carry immediate English comments. Signature directives
# are skipped only for raw imports, whose preceding declaration docs are checked above.
for file in sorted((R/'sdk/tinygo').glob('*.go')):
    if file.name.endswith('_test.go'):continue
    lines=file.read_text().splitlines()
    for i,line in enumerate(lines):
        public=re.match(r'func (?:\([^)]*\) )?([A-Z]\w*)\(',line) or re.match(r'type ([A-Z]\w*) ',line)
        if not public:continue
        j=i-1
        if lines[j].startswith('//go:wasmimport'):j-=1
        check('docs/go/'+file.name+'/'+public.group(1),j>=0 and lines[j].startswith('//'))
files=[]
for parent in ('sdk/rust','sdk/tinygo','sdk/examples/rust','sdk/examples/tinygo'):
    for f in sorted((R/parent).rglob('*')):
        if f.is_file() and '__pycache__' not in f.parts and 'target' not in f.parts:
            files.append({'path':str(f.relative_to(R)),'bytes':f.stat().st_size,'sha256':hashlib.sha256(f.read_bytes()).hexdigest()})
report={'status':'PASS_SOURCE_ONLY' if all(c['pass'] for c in checks) else 'FAIL_SOURCE_ONLY','scope':'Registry, source and fixture presence checks; no Rust/Go/TinyGo compilation, Wasm import-table inspection or ROS runtime verdict','schema_sha256':hashlib.sha256(schema_path.read_bytes()).hexdigest(),'helper_sources':helper_sources,'checks':checks,'check_count':len(checks),'failed':[c for c in checks if not c['pass']],'prepared_language_cases':{'positive':len(wire['positive']),'negative':len(wire['negative']),'output_slot':7,'pure_Go_executor':4,'compile_only_raw_coverage_backends':2},'files':files}
print(json.dumps(report,indent=2))
sys.exit(0 if all(c['pass'] for c in checks) else 1)
