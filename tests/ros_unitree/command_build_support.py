#!/usr/bin/env python3
"""Manifest-bound fresh command compiler evidence; no retained batch admission.

These ordinary helpers preserve complete source copies, invocation aliases,
full MD/link evidence, actual Wasm binary tables and the same-PID Rust linker
barrier. They start no process on import. The calling maintained producer owns
all compiler processes, limits, current cgroup, complete input roots and final
preservation. Historical Source40 batch/restart/retention code is not included.

The Rust barrier records compiler-owned temporary objects before real linker
exec. Their retired historical facts must never be confused with current pins.
The TinyGo target is a new complete copy with only the declared memory bound;
its original buildmode field stays intact, while actual CLI selects default.
"""
from __future__ import annotations
import csv, hashlib, io, json, os, re, shlex, stat, sys
from pathlib import Path
MIB=1024**2


def need(value,reason):
    if not value:raise ValueError(reason)


def read(path,maximum=64*MIB):
    path=Path(path);need(path.is_absolute(),'absolute input required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK)
    try:
        first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<=first.st_size<=maximum,'bounded regular input required')
        body=bytearray()
        while True:
            part=os.read(fd,min(MIB,maximum+1-len(body)))
            if not part:break
            body.extend(part);need(len(body)<=maximum,'input grew')
        last,named=os.fstat(fd),path.stat(follow_symlinks=False)
        fields=('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')
        need(len(body)==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k) for k in fields),'opened input changed')
        raw=bytes(body);return raw,{'path':str(path),'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),'device':first.st_dev,'inode':first.st_ino,'mtime_ns':first.st_mtime_ns,'ctime_ns':first.st_ctime_ns}
    finally:os.close(fd)


def pairs(rows):
    result={}
    for key,value in rows:need(key not in result,'duplicate JSON member');result[key]=value
    return result


def doc(path):
    raw,pin=read(path);return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:need(False,'nonfinite JSON')),pin


def source_pin(frame,frozen,path):
    """Bind an original source to this complete current freeze, not a label."""
    path=Path(path).resolve(strict=True)
    need(path.is_relative_to(frame.source),'actual current source required')
    pin=frame.pin(path)
    need(frozen.get(str(path.relative_to(frame.source)))=={k:pin[k] for k in ('bytes','sha256')},'source body absent from actual complete current manifest')
    return pin


def put(frame,path,body):
    path=Path(path);need(path.is_relative_to(frame.out) and not path.exists() and not path.is_symlink(),'fresh owned regular product required')
    path.parent.mkdir(mode=0o700,parents=True,exist_ok=True)
    with path.open('xb') as stream:stream.write(body if isinstance(body,bytes) else body.encode())
    return frame.pin(path)


def copy_file(frame,old,new,frozen=None):
    old=Path(old).resolve(strict=True)
    before=source_pin(frame,frozen,old) if frozen is not None else frame.pin(old)
    raw,pin=read(old,max(before['bytes'],1));need(pin==before,'opened source changed during independent copy')
    current=put(frame,new,raw)
    need(all(current[k]==before[k] for k in ('bytes','sha256')) and
         (current['device'],current['inode'])!=(before['device'],before['inode']),'independent exact owned source bytes required')
    return {'source':before,'copy':current}


def copy_tree(frame,frozen,relative,stage,associations):
    root=frame.source/relative
    need(root.is_dir(),'complete actual SDK source tree missing')
    for old in sorted(root.rglob('*')):
        if old.is_dir():continue
        need(old.is_file() and not old.is_symlink(),'only MF-bound ordinary staged SDK sources admitted')
        associations.append(copy_file(frame,old,stage/old.relative_to(frame.source),frozen))


def expand(token,values):
    need(type(token) is str and '\0' not in token and '\n' not in token,'clean literal recipe token required')
    for key,value in values.items():token=token.replace('{'+key+'}',str(value))
    need('{' not in token and '}' not in token,'unresolved closed recipe placeholder: '+token)
    return token


def cpp_link_evidence(frame,utilities,linked,why,overlay,links):
    need(overlay['object'] in links and frame.pin(overlay['object']['path'])==overlay['object'],'actual trace omitted selected official object')
    raw,pin=read(why,16*MIB);rows=list(csv.reader(io.StringIO(raw.decode('utf-8','strict')),delimiter='\t'))
    need(rows and rows[0]==['reference','extracted','symbol'] and all(len(r)==3 for r in rows),'actual LLVM22 extraction table required')
    def default_handler(value):
        if not value.endswith(')') or '(' not in value:return False
        archive,member=value.rsplit('(',1)
        return Path(archive).name in ('libc++.a','libc++abi.a') and member[:-1] in (
            'verbose_abort.cpp.o','abort_message.cpp.o','verbose_abort.cpp.obj','abort_message.cpp.obj')
    log,_=read(Path(linked['log']['path']),16*MIB)
    need(not any(default_handler(r[1]) for r in rows[1:]) and
         not any(default_handler(line.strip()) for line in log.decode('utf-8','strict').splitlines()),
         'default diagnostic stderr termination member still selected')
    return {'why_extract':frame.pin(why),'archive_extractions':[dict(zip(rows[0],r)) for r in rows[1:]],
            'default_verbose_and_abort_members_absent':True,'Guest_assertions_disabled':False,'stdio_or_WASI_stubs':False}


def prepare_rust_linker(frame,cell,values,entry,utilities):
    """Capture successful stable-Rust linker stdout outside Rustc's pipe.

    The wrapper is a fresh owned native Python script, not a Guest or a fake
    linker. It checks the same selected physical tools and invocation alias,
    records actual linker argv/cwd, redirects only stdout to a new trace, and
    execs the already opened genuine ELF with the original wasm-ld argv[0].
    The original Rust diagnostics remain in the ordinary compiler job log.
    """
    python=frame.pin(Path(values['PYTHON']).resolve(strict=True))
    need(python['path']=='/usr/bin/python3.14','selected actual Python3.14 required for owned linker wrapper')
    alias=utilities.alias(values['WASM_LD']);linker=frame.pin(alias['physical'])
    wrapper=cell/'rust-linker/wasm-ld';trace=cell/'rust-wasm-ld.trace.log';receipt=cell/'rust-wasm-ld.argv.json'
    config={'python':python,'linker':linker,'alias':alias,'cwd':str(cell),'entry_object':entry['object'],
            'artifact':str(cell/'guest.wasm'),'trace':str(trace),'receipt':str(receipt)}
    body='#!/usr/bin/python3.14\n'+r'''"""Owned one-link stdout capture; never a substitute linker implementation."""
import hashlib,json,os,resource,stat,sys
from pathlib import Path
sys.dont_write_bytecode=True
CONFIG=__CONFIG__
FIELDS=('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')
def need(value,reason):
    if not value:raise ValueError(reason)
def opened(path):
    path=Path(path);need(path.is_absolute() and path.resolve(strict=True)==path,'physical regular input required')
    fd=os.open(path,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
    try:
        first=os.fstat(fd);need(stat.S_ISREG(first.st_mode) and 0<first.st_size<=256*1024**2,'bounded physical tool required')
        digest=hashlib.sha256();count=0
        while True:
            block=os.read(fd,1024**2)
            if not block:break
            digest.update(block);count+=len(block);need(count<=first.st_size,'input grew')
        last,named=os.fstat(fd),path.stat(follow_symlinks=False)
        need(count==first.st_size and all(getattr(first,k)==getattr(last,k)==getattr(named,k) for k in FIELDS),'opened input changed')
        pin=dict(path=str(path),bytes=count,sha256=digest.hexdigest(),device=first.st_dev,inode=first.st_ino,mtime_ns=first.st_mtime_ns,ctime_ns=first.st_ctime_ns)
        os.lseek(fd,0,os.SEEK_SET);return fd,pin
    except BaseException:os.close(fd);raise
def alias(path):
    path=Path(path);need(path.is_absolute() and path.parent.resolve(strict=True)==path.parent,'canonical invocation parent required')
    first=path.lstat();target=os.readlink(path) if stat.S_ISLNK(first.st_mode) else None
    need(stat.S_ISREG(first.st_mode) or stat.S_ISLNK(first.st_mode),'ordinary selected alias required')
    physical=path.resolve(strict=True);last=path.lstat()
    need(all(getattr(first,k)==getattr(last,k) for k in (*FIELDS,'st_mode')) and (target is None or os.readlink(path)==target),'tool alias changed')
    return dict(path=str(path),physical=str(physical),target=target,device=first.st_dev,inode=first.st_ino,mode=first.st_mode,bytes=first.st_size,mtime_ns=first.st_mtime_ns,ctime_ns=first.st_ctime_ns)
need(sys.platform=='linux' and os.geteuid()==0 and os.execve in os.supports_fd,'Linux Root opened-ELF exec required')
need(os.getcwd()==CONFIG['cwd'],'actual linker cwd differs from owned compiler cell')
need(len(sys.argv)<=4096 and sum(len(x.encode())+1 for x in sys.argv)<=1024**2 and all('\0' not in x and '\n' not in x for x in sys.argv),'bounded actual linker argv required')
args=sys.argv[1:]
need(args.count('--trace')==1 and args.count('--threads=1')==1 and args.count('--max-memory=67108864')==1 and args.count('--export=_start')==1 and args.count(CONFIG['entry_object']['path'])==1,'actual unchanged strict linker flags/entry required')
need(args.count('-o')==1 and args[args.index('-o')+1]==CONFIG['artifact'],'actual owned output required')
need(not any(x.startswith('@') for x in args),'unopened response-file indirection is outside this closed linker receipt')
fd,linker=opened(CONFIG['linker']['path']);need(linker==CONFIG['linker'] and alias(CONFIG['alias']['path'])==CONFIG['alias'],'selected real linker/alias differs')
for key,path in (('python',Path(sys.executable).resolve(strict=True)),('entry_object',Path(CONFIG['entry_object']['path']))):
    handle,pin=opened(path);os.close(handle);need(pin==CONFIG[key],'selected actual tool/entry differs')
handle,own=opened(Path(__file__).resolve(strict=True));os.close(handle)
# Rustc can remove its own temporary objects after a successful link. Capture
# every actual ordinary file argument at this genuine pre-exec barrier, before
# those files can disappear. This is historical opened-byte provenance, not a
# promise that a temporary still exists after Rustc returns.
file_arguments=[]
for index,token in enumerate(args):
    if token.startswith('-'):continue
    candidate=Path(token);candidate=candidate if candidate.is_absolute() else Path.cwd()/candidate
    if candidate.is_file():
        handle,pin=opened(candidate.resolve(strict=True));os.close(handle)
        file_arguments.append(dict(argument_index=index+1,spelling=token,pin=pin))
need(any(row['pin']==CONFIG['entry_object'] for row in file_arguments),'pre-exec actual entry object absent')
record=dict(kind='uwvm-owned-rust-linker-exec-v1',cwd=os.getcwd(),pid=os.getpid(),argv=[CONFIG['alias']['path'],*args],wrapper=own,python=CONFIG['python'],linker=linker,invocation_alias=CONFIG['alias'],trace=CONFIG['trace'],file_arguments=file_arguments,execution='same-PID opened physical ELF; original invocation argv0 preserved')
raw=(json.dumps(record,indent=2)+'\n').encode();need(len(raw)<=1024**2,'bounded linker receipt required')
out=os.open(CONFIG['receipt'],os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,0o600)
with os.fdopen(out,'wb') as stream:stream.write(raw)
# Bound every linker-created file independently of the parent's finite RSS,
# wall-time, whole-output and disk guards. The trace is rechecked at16MiB.
resource.setrlimit(resource.RLIMIT_FSIZE,(64*1024**2,64*1024**2))
stdout=os.open(CONFIG['trace'],os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,0o600)
os.dup2(stdout,1,inheritable=True);os.close(stdout)
os.execve(fd,[CONFIG['alias']['path'],*args],dict(os.environ))
'''
    body=body.replace('__CONFIG__',repr(config))
    # Final permissions must precede the FIRST stored physical pin: chmod
    # changes ctime even when the bytes are identical.
    need(wrapper.is_relative_to(frame.out) and not wrapper.exists() and not wrapper.is_symlink(),'fresh owned linker wrapper required')
    wrapper.parent.mkdir(mode=0o700,parents=True,exist_ok=True)
    with wrapper.open('xb') as stream:stream.write(body.encode())
    os.chmod(wrapper,0o700);pin=frame.pin(wrapper)
    return {'wrapper':pin,'python':python,'linker':linker,'alias':alias,'trace_path':str(trace),'receipt_path':str(receipt),
            'cell':str(cell),'entry_object':entry['object'],'artifact':str(cell/'guest.wasm')}


def rust_compiler_temporary(path, output, cell):
    """Recognize only the declared compiler-owned temporary object locations.

    This predicate grants no input authority. The caller must already possess
    the same-PID linker's complete pre-exec file pin and actual successful
    trace, and may use this spelling policy only after that file disappeared.
    The raw pattern makes the literal dots in ordinary ``*.rcgu.o`` names
    explicit; unrelated cell-local objects remain persistent required inputs.
    """
    path,output,cell=Path(path),Path(output),Path(cell)
    return (path.is_absolute() and path.suffix=='.o' and
            (path.is_relative_to(output/'tmp') or
             (path.parent==cell and re.fullmatch(r'[A-Za-z0-9_.-]+\.rcgu\.o',path.name) is not None)))


def qualify_rust_linker(frame, proof, utilities):
    """Require genuine successful-link trace and actual recorded invocation."""
    receipt, rp = doc(Path(proof['receipt_path']))
    trace = frame.pin(proof['trace_path'])
    need(trace['bytes'] <= 16 * MIB and rp == frame.pin(rp['path']), 'bounded actual linker receipt/trace required')
    need(set(receipt) == {'kind', 'cwd', 'pid', 'argv', 'wrapper', 'python', 'linker', 'invocation_alias', 'trace', 'file_arguments', 'execution'} and receipt['kind'] == 'uwvm-owned-rust-linker-exec-v1' and (receipt['cwd'] == proof['cell']) and (type(receipt['pid']) is int) and (receipt['pid'] > 0) and (receipt['wrapper'] == proof['wrapper']) and (receipt['python'] == proof['python']) and (receipt['linker'] == proof['linker']) and (receipt['invocation_alias'] == proof['alias']) and (receipt['trace'] == trace['path']) and (receipt['execution'] == 'same-PID opened physical ELF; original invocation argv0 preserved'), 'actual owned-linker execution receipt differs')
    need(frame.pin(proof['wrapper']['path']) == proof['wrapper'] and frame.pin(proof['python']['path']) == proof['python'] and (frame.pin(proof['linker']['path']) == proof['linker']) and (utilities.alias(proof['alias']['path']) == proof['alias']), 'selected wrapper/tool bytes or alias changed')
    argv = receipt['argv']
    need(type(argv) is list and argv[0] == proof['alias']['path'] and all((type(x) is str for x in argv)) and (argv.count('--trace') == argv.count('--threads=1') and argv.count('--threads=1') == argv.count('--max-memory=67108864') and (argv.count('--max-memory=67108864') == argv.count('--export=_start')) and (argv.count('--export=_start') == 1)) and (argv.count(proof['entry_object']['path']) == 1) and (argv.count('-o') == 1) and (argv[argv.index('-o') + 1] == proof['artifact']), 'actual strict linker flags/output/entry evidence required')
    captured = {}
    temporary = []
    persistent = {}
    need(type(receipt['file_arguments']) is list and 1 <= len(receipt['file_arguments']) <= 4096, 'bounded genuine pre-exec file argument ledger required')
    for row in receipt['file_arguments']:
        need(type(row) is dict and set(row) == {'argument_index', 'spelling', 'pin'} and (type(row['argument_index']) is int) and (0 < row['argument_index'] < len(argv)) and (argv[row['argument_index']] == row['spelling']), 'pre-exec file argument/actual argv association differs')
        pin = row['pin']
        need(type(pin) is dict and set(pin) == {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}, 'actual pre-exec full7 required')
        path = Path(pin['path'])
        spelled = Path(row['spelling'])
        spelled = spelled if spelled.is_absolute() else Path(proof['cell']) / spelled
        need(spelled.resolve(strict=False) == path and path.is_absolute(), 'actual file argument physical spelling differs')
        need(pin['path'] not in captured or captured[pin['path']] == pin, 'duplicate pre-exec path has inconsistent identity')
        captured[pin['path']] = pin
        if path.exists() or path.is_symlink():
            current = frame.pin(path)
            need(current == pin, 'persistent actual Rust linker input changed')
            persistent[pin['path']] = current
        else:
            need(rust_compiler_temporary(path, frame.out, proof['cell']) and (pin != proof['entry_object']), 'unproved disappeared linker input refused')
            temporary.append({'pre_exec_full7': pin, 'post_rustc_exists': False, 'scope': 'Actual compiler-owned temporary object hashed before linker exec; not a current retained input pin'})

    def historical_facts(pin):
        return {'physical_path': pin['path'], **{k: v for k, v in pin.items() if k != 'path'}}
    raw, tp = read(Path(trace['path']), 16 * MIB)
    need(tp == trace, 'actual trace changed during complete parsing')
    trace_entries = []
    seen = set()
    for line in raw.decode('utf-8', 'strict').splitlines():
        value = line.strip()
        if not value:
            continue
        candidate = value.split('(', 1)[0] if value.endswith(')') else value
        need(candidate and (not any((ch in candidate for ch in '\x00\t'))), 'invalid actual trace file syntax')
        path = Path(candidate)
        path = path if path.is_absolute() else Path(proof['cell']) / path
        path = path.resolve(strict=False)
        name = str(path)
        seen.add(name)
        if path.exists() or path.is_symlink():
            pin = frame.pin(path)
            if name in captured:
                need(pin == captured[name], 'trace input differs from genuine pre-exec bytes')
            persistent[name] = pin
            trace_entries.append({'raw': line, 'current_full7': pin})
        else:
            matches = [row for row in temporary if row['pre_exec_full7']['path'] == name]
            need(len(matches) == 1, 'actual trace names an uncaptured or unsupported missing file')
            trace_entries.append({'raw': line, 'pre_exec_temporary_opened_facts': historical_facts(matches[0]['pre_exec_full7']), 'current_file_present': False, 'scope': 'Historical actual pre-exec compiler temporary only'})
    need(trace_entries and proof['entry_object']['path'] in seen and (proof['entry_object'] in list(persistent.values())), 'actual retained Rust linker trace omitted direct persistent entry object')
    need(all((row['pre_exec_full7']['path'] in seen for row in temporary)), 'retired compiler object was not witnessed in actual linker trace')
    links = utilities.trace_inputs(frame, {'log': trace}, Path(proof['cell']))
    need(all((pin in persistent.values() for pin in links)), 'ordinary utility trace closure differs')
    return {'wrapper': proof['wrapper'], 'python': proof['python'], 'selected_linker': proof['linker'], 'selected_invocation_alias': proof['alias'], 'actual_linker_receipt': rp, 'actual_linker_argv': argv, 'actual_cwd': receipt['cwd'], 'actual_linker_pid': receipt['pid'], 'retained_stdout_trace': trace, 'actual_link_inputs': list(persistent.values()), 'actual_trace_entries': trace_entries, 'pre_exec_file_arguments_historical_facts': [{'argument_index': r['argument_index'], 'spelling': r['spelling'], 'opened_at_linker_barrier': historical_facts(r['pin'])} for r in receipt['file_arguments']], 'retired_compiler_temporaries_historical_only': [{'opened_at_linker_barrier': historical_facts(r['pre_exec_full7']), 'post_rustc_exists': False, 'scope': r['scope']} for r in temporary], 'rust_diagnostics_remain_in_original_compiler_log': True, 'same_PID_real_linker_exec': True}


def derive_tinygo_target(frame,values,cell):
    original=Path(values['TINYGO_ROOT'])/'targets/wasm-unknown.json';raw,pin=read(original,MIB)
    need(frame.pin(original)==pin and len(raw)==790 and pin['sha256']=='63531e39ab6c1a3bcd0c1a78d7a68bdae917a530c0b64fe80541b163a1b037f3','exact installed official pure reactor target required')
    target=json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:need(False,'nonfinite target'))
    need(target['llvm-target']=='wasm32-unknown-unknown' and target['buildmode']=='c-shared' and target['scheduler']=='none' and
         target['libc']=='wasmbuiltins' and target['gc']=='leaking' and not target.get('inherits') and
         target['ldflags']==['--stack-first','--no-demangle','--no-entry'],'self-contained actual pure target required')
    target['ldflags']=[*target['ldflags'],'--max-memory=67108864']
    path=cell/'bounded-wasm-unknown.json';owned=put(frame,path,json.dumps(target,indent=2)+'\n')
    return path,{'original':pin,'owned':owned,'only_semantic_changes':{'ldflags_append':['--max-memory=67108864']},'actual_CLI_buildmode':'default','SDK_modified':False}


def raw_file(path, limit=16*MIB):
    """Bootstrap stable regular bytes; do not import a helper before admission."""
    path = Path(path)
    need(path.is_absolute(), "absolute file required")
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        first = os.fstat(fd)
        need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= limit, "bounded regular file required")
        body = bytearray()
        while True:
            part = os.read(fd, 65536)
            if not part:
                break
            body.extend(part)
            need(len(body) <= limit, "file grew beyond its bound")
        last, named = os.fstat(fd), path.stat(follow_symlinks=False)
        need(len(body) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
             for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")), "opened file changed")
        return bytes(body)
    finally:
        os.close(fd)


def alias(path):
    """Preserve multicall invocation spelling independently of physical bytes."""
    path = Path(path)
    need(path.is_absolute() and path.parent.resolve(strict=True) == path.parent, "canonical invocation parent required")
    first = path.lstat()
    target = os.readlink(path) if stat.S_ISLNK(first.st_mode) else None
    need(stat.S_ISREG(first.st_mode) or stat.S_ISLNK(first.st_mode), "regular tool or selected tool alias required")
    resolved = path.resolve(strict=True)
    last = path.lstat()
    need(all(getattr(first, key) == getattr(last, key) for key in
         ("st_dev", "st_ino", "st_mode", "st_size", "st_mtime_ns", "st_ctime_ns")) and
         (target is None or os.readlink(path) == target), "tool invocation alias changed")
    return {"path": str(path), "physical": str(resolved), "target": target,
        "device": first.st_dev, "inode": first.st_ino, "mode": first.st_mode,
        "bytes": first.st_size, "mtime_ns": first.st_mtime_ns, "ctime_ns": first.st_ctime_ns}


def contained(frame, path, owned=False):
    path = Path(path).resolve(strict=True)
    need(path.is_relative_to(frame.source) or any(path.is_relative_to(root) for root in frame.roots) or
         (owned and path.is_relative_to(frame.out)), "input lacks a complete frozen/tool/SDK inventory: " + str(path))
    return path


def wasm_structure(data, inspector):
    """Reuse the maintained bounded core reader, adding actual export/memory data.

    Full wasm-tools validation remains a separate actual child command. New GC,
    memory64, shared memories or unsupported constant expressions fail closed;
    these scalar examples do not claim those proposals as their own coverage.
    """
    R = inspector.Reader
    source = R(data)
    need(bytes(source.take(8)) == b"\0asm\1\0\0\0", "actual core Wasm binary required")
    types_, functions, globals_, memories, exports, start = [], [], [], [], {}, None
    imported_globals = {}
    seen = set()
    def limits(body):
        flags = body.uint()
        need(flags in (0, 1), "shared/memory64 limits are outside this recipe")
        minimum = body.uint()
        return {"minimum": minimum, "maximum": body.uint() if flags else None}
    def global_type(body):
        value, mutable = body.value(), body.byte()
        need(mutable in (0, 1), "invalid global mutability")
        return {"type": value, "mutable": bool(mutable)}
    def expression(body):
        operation = body.byte()
        if operation in (0x41, 0x42):
            for _ in range(10):
                if not body.byte() & 128:
                    break
            else:
                raise ValueError("unbounded initializer integer")
        elif operation in (0x43, 0x44):
            body.take(4 if operation == 0x43 else 8)
        elif operation in (0x23, 0xd2):
            body.uint()
        elif operation == 0xd0:
            body.value()
        else:
            raise ValueError("unsupported initializer expression in scalar artifact")
        need(body.byte() == 0x0b, "nontrivial initializer outside scalar recipe")
    while source.position < len(source.data):
        section, body = source.byte(), None
        body = R(source.take(source.uint()))
        if section and section in seen:
            raise ValueError("duplicate Wasm section")
        seen.add(section)
        if section == 1:
            for _ in range(body.count()):
                need(body.byte() == 0x60, "ordinary function type required")
                types_.append(([body.value() for _ in range(body.count())], [body.value() for _ in range(body.count())]))
        elif section == 2:
            for _ in range(body.count()):
                imported_module, imported_name, kind = body.name(), body.name(), body.byte()
                if kind == 0:
                    functions.append(body.uint())
                elif kind == 1:
                    body.value(); limits(body)
                elif kind == 2:
                    memories.append(dict(limits(body), imported=True))
                elif kind == 3:
                    value = global_type(body)
                    globals_.append(value)
                    imported_globals[imported_module+"."+imported_name] = value
                else:
                    raise ValueError("tag import outside scalar recipe")
        elif section == 3:
            functions.extend(body.uint() for _ in range(body.count()))
        elif section == 5:
            memories.extend(dict(limits(body), imported=False) for _ in range(body.count()))
        elif section == 6:
            for _ in range(body.count()):
                globals_.append(global_type(body)); expression(body)
        elif section == 7:
            for _ in range(body.count()):
                name, kind, index = body.name(), body.byte(), body.uint()
                need(name not in exports, "duplicate export name")
                value = {"kind": kind, "index": index}
                if kind == 0:
                    need(index < len(functions) and functions[index] < len(types_), "exported function type absent")
                    value.update(params=types_[functions[index]][0], results=types_[functions[index]][1])
                elif kind == 3:
                    need(index < len(globals_), "exported global absent")
                    value.update(globals_[index])
                exports[name] = value
        elif section == 8:
            start = body.uint()
        else:
            continue
        body.end()
    return {"exports": exports, "memories": memories, "start_function": start, "imports": inspector.inspect(data),
            "imported_globals": imported_globals}


def full_md(frame, dependency, obj, source, cwd):
    body, _ = frame.common.body_pin(dependency)
    text = body.decode("utf-8").replace("\\\n", " ")
    need(":" in text, "actual complete dependency rule required")
    left, right = text.split(":", 1)
    need(shlex.split(left) == [str(obj)], "dependency target differs from fresh actual object")
    names = shlex.split(right)
    need(names and not any(value.endswith(":") for value in names), "single full-MD rule required")
    result = {}
    for value in names:
        path = Path(value)
        path = contained(frame, path if path.is_absolute() else cwd/path, owned=True)
        pin = frame.pin(path); result[pin["path"]] = pin
    need(str(source.resolve(strict=True)) in result, "actual source absent from compiler dependencies")
    return list(result.values())


def trace_inputs(frame, command, cwd):
    """Retain all existing linker trace inputs; no nonexistent file is invented."""
    text = raw_file(Path(command["log"]["path"]), frame.limits["log_bytes"]).decode("utf-8", "strict")
    result = {}
    for line in text.splitlines():
        value = line.strip()
        candidate = value.split("(", 1)[0] if value.endswith(")") else value
        if not candidate or any(c in candidate for c in "\0\t"):
            continue
        path = Path(candidate)
        if not path.is_absolute():
            path = cwd / path
        if path.is_file():
            path = contained(frame, path, owned=True)
            pin = frame.pin(path); result[pin["path"]] = pin
    return list(result.values())


def run(frame, owner, label, argv, cwd, environment):
    """Run one real tool and close its observed terminal births before the next.

    This is the unchanged fresh-command terminal barrier from the reviewed
    generic utility. It retains parent/child ownership and all actual logs;
    numeric PIDs are observations, never unowned signal targets.
    """
    need(Path(argv[0]).is_absolute() and Path(argv[0]).resolve(strict=True).is_file(), "actual explicit command tool required")
    row = owner.run(label, argv, cwd, environment)
    owner.discover()
    retired = []
    for key, item in list(owner.owned.items()):
        need(frame.common.pidfd_terminal(item["fd"]), "command left a live owned descendant; refuse overlapping command")
        retired.append({"first": item["identity"], "after_pidfd": item["identity_after"], "terminal": True})
        os.close(item["fd"]); del owner.owned[key]
    for key, item in owner.unproven.items():
        now = frame.common.identity(item["pid"])
        need(frame.common.birth(now) != key or now["state"] == "Z", "unproven command descendant still live")
    row["terminal_owned_births"] = retired
    frame.pin(row["log"]["path"])
    frame.common.atomic_json(frame.out/"commands.json", owner.commands)
    frame.save()
    return row


def rust_dependencies(frame, dependency, artifact, source, cwd):
    """Read actual Rust dep-info without pretending it is Clang full-MD.

    Rust emits rules for the depfile and artifact, followed by optional empty
    source rules. Require the actual artifact rule and original source, retain
    every declared ordinary input, and cover omitted system crates through the
    independently inventoried complete selected Rust SDK. No missing file is
    skipped, and no dependency is invented from a filename or old receipt.
    """
    raw, pin = read(dependency,16*MIB)
    need(frame.pin(dependency)==pin,'actual Rust dep-info changed')
    result={};matched=False;environment=[]
    for line in raw.decode('utf-8','strict').replace('\\\n',' ').splitlines():
        if not line.strip():continue
        if line.startswith('# env-dep:'):
            environment.append(line);continue
        need(':' in line,'actual Rust make rule required')
        left,right=line.split(':',1);targets=shlex.split(left)
        need(targets,'nonempty Rust dep-info target required')
        if str(artifact) in targets:matched=True
        for token in shlex.split(right):
            path=Path(token);path=contained(frame,path if path.is_absolute() else cwd/path,owned=True)
            actual=frame.pin(path);result[actual['path']]=actual
    need(matched and str(source.resolve(strict=True)) in result,'actual Rust artifact/source rule missing')
    return {'dependency_file':pin,'artifact_target':str(artifact),'dependencies':list(result.values()),
            'environment_comments':environment,'scope':'ACTUAL_RUST_DEP_INFO_PLUS_COMPLETE_SELECTED_SDK_NOT_CLANG_FULL_MD'}
