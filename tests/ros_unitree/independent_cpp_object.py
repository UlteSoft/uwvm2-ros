"""Requalify one genuine completed official libcxxabi object, never Typed14.

This consumer writes only a current metadata sidecar. It starts no actor and
changes no SDK, source, object or raw historical status. Original compilation
facts remain separate from current opened pins and current cgroup admission.
"""
from __future__ import annotations
import base64
import hashlib
import itertools
import json
import os
from pathlib import Path
import re
import stat
import types

MIB=1024**2
PIN_KEYS={'path','device','inode','bytes','mtime_ns','ctime_ns','sha256'}
COMMIT='4434dabb69916856b824f68a64b029c67175e532'
PRODUCER_SHA='31340e874df8848a9e7710d824483ae4588fbb9c320fe79ca12f056a33407981'
POLICY='OFFICIAL_LIBCXXABI_BAREMETAL_ABORT_TU_AND_HEADER_VERBOSE_TRAP_V1'
TRAP='-D_LIBCPP_VERBOSE_ABORT(...)=__builtin_trap()'
FILES=('libcxxabi/src/abort_message.cpp','libcxxabi/src/abort_message.h',
       'libcxxabi/include/cxxabi.h','libcxxabi/include/__cxxabi_config.h')
TREES=('', 'libcxxabi','libcxxabi/src','libcxxabi/include')
STATUS='PASS_INDEPENDENT_OFFICIAL_CPP_LIBRARY_INPUT_QUALIFICATION_ONLY'


def require(value,reason):
    if not value:raise ValueError(reason)


def exact(value,keys,label):
    require(type(value) is dict and set(value)==set(keys),'closed '+label+' required')


def decode(raw):
    def pairs(rows):
        result={}
        for key,value in rows:
            require(key not in result,'duplicate independent C++ evidence key');result[key]=value
        return result
    return json.loads(raw,object_pairs_hook=pairs,parse_constant=lambda _:require(False,'nonfinite independent C++ evidence'))


def reopen(context,expected,limit=16*MIB):
    """Use only the existing finite Evidence accessor; never return old as new."""
    actual=context.evidence.pin(expected)
    raw=context.evidence.api.bounded(Path(actual['path']),limit)
    require(len(raw)==actual['bytes'] and hashlib.sha256(raw).hexdigest()==actual['sha256'] and
            context.evidence.frame.pin(actual['path'])==actual,'current official input bytes changed')
    return raw


# official_sources below is copied byte-for-byte from the genuine SHA-bound
# original31340 producer. Only its module-level reopened-file primitive above
# uses the existing current Evidence bridge. No source/JSON/status projection
# or historical cgroup continuity is used to satisfy its source proof.
def official_sources(frame, selected):
    lock, lock_pin = frame.common.document(frame.input(selected))
    require(lock_pin == frame.inputs[selected], "first selected official source lock physical pin changed")
    acquisition_scope = None
    if lock.get("kind") == "wasi-sdk33-official-libcxxabi-source-inputs":
        # Root's real TLS/Git acquisition report is a valid source lock. Its
        # first metadata attempt may have failed on a later source download;
        # retain that history, and admit source files only from the completed
        # new producer. The original five successfully fetched JSON documents
        # remain physically pinned provenance, never promoted to a build PASS.
        require(lock.get("status") == "PREPARED_OFFICIAL_SDK_MATCHED_SOURCE_ONLY" and
                lock.get("upstream") == "https://github.com/llvm/llvm-project" and lock.get("commit") == COMMIT and
                lock.get("official_source_complete") is True and not lock.get("errors") and
                all(lock.get(key) is True for key in ("source_preserved", "inputs_preserved", "products_preserved")) and
                lock.get("cleanup", {}).get("status") == "PASS" and not lock["cleanup"]["remaining"],
                "actual completed preserved official source acquisition required")
        # Historical network/tool state belongs to the SHA-bound acquisition
        # report, not this offline compiler's current input closure. Reopen only
        # the selected Git documents, raw files, lock and SDK VERSION below.
        # The full old ledger is validated structurally and associations remain
        # exact, but unrelated DNS/CA/curl/log pins are deliberately not opened.
        ledger, selected_artifacts = {}, {}
        for category in ("input_pins", "product_pins"):
            rows = lock.get(category)
            require(type(rows) is list and len(rows) <= 4096, "bounded acquisition pin ledger required")
            for pin in rows:
                exact(pin, PIN_KEYS, "historical acquisition ledger pin")
                require(type(pin["path"]) is str and Path(pin["path"]).is_absolute() and
                        re.fullmatch(r"[0-9a-f]{64}", pin["sha256"]) and
                        all(type(pin[key]) is int and pin[key] >= 0 for key in
                            ("device", "inode", "bytes", "mtime_ns", "ctime_ns")) and
                        pin["path"] not in ledger, "unique valid historical acquisition pin required")
                ledger[pin["path"]] = pin
        def artifact(pin, limit=16*MIB):
            exact(pin, PIN_KEYS, "selected current official artifact")
            require(ledger.get(pin["path"]) == pin,
                    "selected current artifact missing from actual acquisition ledger")
            body = reopen(frame, pin, limit)
            selected_artifacts[pin["path"]] = pin
            return body
        metadata = lock["metadata_provenance"]
        require(type(metadata) is list and len(metadata) == 5, "five retained actual Git API documents required")
        names = {}
        for row in metadata:
            exact(row, ("pin", "actual_original_url"), "retained API JSON document")
            pin, data = row["pin"], decode(artifact(row["pin"]))
            name = Path(pin["path"]).name
            require(name not in names, "duplicate actual API document")
            suffix = "commits/"+COMMIT if name == "actual-commit.json" else "git/trees/"+data["sha"]
            require(row["actual_original_url"] == "https://api.github.com/repos/llvm/llvm-project/"+suffix,
                    "actual metadata acquisition URL differs from selected official object")
            names[name] = (pin, data)
        expected = ("actual-commit.json", "actual-root-tree.json", "actual-libcxxabi-tree.json",
                    "actual-libcxxabi-src-tree.json", "actual-libcxxabi-include-tree.json")
        exact(names, expected, "actual nonrecursive Git API documents")
        files = {}
        require(type(lock.get("files")) is list and len(lock["files"]) == 5, "four official compile files and license required")
        for row in lock["files"]:
            exact(row, ("relative", "input", "git_blob_sha1", "git_mode", "git_tree_size", "blob_document"), "actual acquired source")
            require(row["relative"] in (*FILES, "LICENSE.TXT") and row["relative"] not in files and
                    row["git_mode"] == "100644" and row["input"]["bytes"] == row["git_tree_size"],
                    "closed actual source/license inventory differs")
            body = artifact(row["input"], 128*1024)
            blob = decode(artifact(row["blob_document"], 256*1024))
            require(blob["encoding"] == "base64" and blob["sha"] == row["git_blob_sha1"] and
                    blob["size"] == len(body) and type(blob["content"]) is str and
                    base64.b64decode(blob["content"].replace("\n", ""), validate=True) == body,
                    "actual official raw blob document does not decode to the selected source")
            require(hashlib.sha1(b"blob " + str(len(body)).encode() + b"\0" + body, usedforsecurity=False).hexdigest() == row["git_blob_sha1"],
                    "actual acquired source/license blob changed")
            files[row["relative"]] = {"pin": row["input"], "git_blob_oid": row["git_blob_sha1"]}
        license_entry = next(entry for entry in names["actual-root-tree.json"][1]["tree"] if entry["path"] == "LICENSE.TXT")
        require(license_entry["type"] == "blob" and license_entry["mode"] == "100644" and
                license_entry["sha"] == files["LICENSE.TXT"]["git_blob_oid"] and
                license_entry["size"] == files["LICENSE.TXT"]["pin"]["bytes"], "official license/tree differs")
        original_lock = decode(artifact(lock["source_lock"]))
        require(original_lock.get("kind") == "SDK33-official-libcxxabi-source-lock" and
                original_lock.get("commit") == COMMIT and original_lock.get("metadata_provenance") == metadata and
                original_lock.get("files") == lock["files"], "actual source-lock/producer association differs")
        version = artifact(original_lock["sdk_version"], 4096)
        require(b"llvm: 4434dabb6991\n" in version and b"llvm-version: 22.1.0\n" in version,
                "acquisition SDK VERSION does not match the selected official source revision")
        require(len(selected_artifacts) == 17, "exact five API/five blob/five source/lock/VERSION artifacts required")
        acquisition_scope = {
            "current_source_artifacts": [selected_artifacts[name] for name in sorted(selected_artifacts)],
            "historical_input_pin_count": len(lock["input_pins"]),
            "historical_product_pin_count": len(lock["product_pins"]),
            "unselected_historical_pin_count": len(ledger)-len(selected_artifacts),
            "unselected_historical_pins_reopened": False,
            "historical_network_state_current_match_claimed": False,
            "history_location": "Original complete SHA-bound transport_report; not a current compiler input ledger"}
        lock = {"format_version": 1, "kind": "official-llvm-termination-source-lock",
                "repository": lock["upstream"], "commit": COMMIT,
                "tree": names["actual-commit.json"][1]["commit"]["tree"]["sha"],
                "commit_document": names[expected[0]][0],
                "tree_documents": dict(zip(TREES, [names[name][0] for name in expected[1:]])),
                "files": {key: files[key] for key in FILES}, "transport_report": lock_pin}
    exact(lock, ("format_version", "kind", "repository", "commit", "tree", "commit_document",
                 "tree_documents", "files", "transport_report"), "official LLVM source lock")
    require(lock["format_version"] == 1 and lock["kind"] == "official-llvm-termination-source-lock" and
            lock["repository"] == "https://github.com/llvm/llvm-project" and lock["commit"] == COMMIT and
            re.fullmatch(r"[0-9a-f]{40}", lock["tree"]), "exact SDK LLVM revision required")
    commit = decode(reopen(frame, lock["commit_document"]))
    require(commit["sha"] == COMMIT and commit["commit"]["tree"]["sha"] == lock["tree"],
            "official commit/tree association differs")
    exact(lock["tree_documents"], TREES, "nonrecursive official tree documents")
    trees = {}
    for name in TREES:
        document = decode(reopen(frame, lock["tree_documents"][name]))
        require(document.get("truncated") is False and type(document.get("tree")) is list and
                len(document["tree"]) <= 4096 and re.fullmatch(r"[0-9a-f]{40}", document["sha"]),
                "complete nonrecursive Git tree required")
        entries = {}
        for entry in document["tree"]:
            require(type(entry.get("path")) is str and Path(entry["path"]).name == entry["path"] and
                    entry["path"] not in (".", "..") and entry["path"] not in entries,
                    "safe unique nonrecursive Git-tree entry required")
            entries[entry["path"]] = entry
        if not name:
            require(document["sha"] == lock["tree"], "root tree differs from commit")
        else:
            parent, leaf = str(Path(name).parent), Path(name).name
            parent = "" if parent == "." else parent
            entry = trees[parent]["entries"][leaf]
            require(entry["type"] == "tree" and entry["mode"] == "040000" and
                    entry["sha"] == document["sha"], "official parent/child tree differs")
        trees[name] = {"sha": document["sha"], "entries": entries}
    exact(lock["files"], FILES, "four exact official source files")
    files, bodies = {}, {}
    for name in FILES:
        row = lock["files"][name]
        exact(row, ("pin", "git_blob_oid"), "official Git blob")
        entry = trees[str(Path(name).parent)]["entries"][Path(name).name]
        require(entry["type"] == "blob" and entry["mode"] == "100644" and entry["sha"] == row["git_blob_oid"],
                "selected source is not the recorded regular official tree blob")
        body = reopen(frame, row["pin"], 128*1024)
        require(entry["size"] == len(body) and hashlib.sha1(b"blob " + str(len(body)).encode() + b"\0" + body, usedforsecurity=False).hexdigest()
                == row["git_blob_oid"], "official Git blob/body differs")
        files[name], bodies[name] = row["pin"], body
    # The four files retain their original adjacent src/include layout. No
    # generated stand-in header or hand-written termination implementation is
    # substituted for this exact upstream code.
    source = Path(files[FILES[0]]["path"])
    root = source.parents[2]
    require(all(Path(files[name]["path"]) == root/name for name in FILES), "original upstream file layout required")
    cpp = bodies[FILES[0]].decode("utf-8", "strict")
    require(cpp.count("#if !defined(NDEBUG) || !defined(LIBCXXABI_BAREMETAL)") == 1 and
            len(re.findall(r"\bNDEBUG\b", cpp)) == 1 and
            not re.search(r"\b(?:assert|static_assert|_LIBCXXABI_ASSERT|_LIBCPP_ASSERT\w*)\s*\(", cpp) and
            len(re.findall(r"\babort\s*\(\s*\)\s*;", cpp)) == 1 and
            re.search(r"\babort\s*\(\s*\)\s*;\s*}\s*$", cpp),
            "one local diagnostics guard, no assertion invocation and unconditional final abort required")
    transport = decode(reopen(frame, lock["transport_report"]))
    require(type(transport) is dict, "actual retained TLS/acquisition metadata required")
    return {"lock": lock_pin, "repository": lock["repository"], "commit": COMMIT, "tree": lock["tree"],
            "files": files, "transport_report": lock["transport_report"], "acquisition_scope": acquisition_scope,
            "no_assertion_site_in_compiled_TU": True}, root


def terminal(report):
    """A standalone producer must already be terminal and preserved itself."""
    require(type(report) is dict and report.get('kind')=='wasi-cpp-official-termination-overlay' and
            report.get('status')=='PASS_OFFICIAL_CPP_TERMINATION_OBJECT_PREPARATION_ONLY' and
            not report.get('error') and not report.get('errors') and not report.get('finalization_errors') and
            all(report.get(k) is True for k in ('source_preserved','input_pins_preserved',
                'whole_roots_preserved','overlay_products_preserved')) and
            report.get('runtime_executed') is False and report.get('SDK_modified') is False and
            report.get('granted_rights')==[],
            'genuine terminal standalone official object report required')
    cleanup=report.get('cleanup')
    require(type(cleanup) is list and cleanup and all(type(r) is dict and r.get('status')=='PASS' and
            not r.get('remaining') and not r.get('errors') for r in cleanup),'standalone object owned cleanup required')
    return report['overlay']


def compile_association(proof,obj,dep,source,compiler,sysroot):
    """Check the unchanged per-TU policy and exact real full-MD argv roles."""
    require(proof['policy']==POLICY and proof['Guest_compile_flags']==['-UNDEBUG',TRAP] and
            proof['official']['commit']==COMMIT and proof['official']['no_assertion_site_in_compiled_TU'] is True and
            proof['runtime_executed'] is False and proof['SDK_modified'] is False and
            proof['compiler']['physical']==compiler and proof['sysroot']==sysroot,
            'unchanged official object policy/compiler/sysroot required')
    command=proof['compile'];argv=command['argv']
    expected=[proof['compiler']['path'],'--driver-mode=g++','--target=wasm32-wasip1',
        '--sysroot='+sysroot,'-std=c++20','-O1','-g0','-fno-builtin','-fno-exceptions','-fno-rtti',
        '-DLIBCXXABI_BAREMETAL','-DNDEBUG','-I'+str(Path(source['path']).parent.parent/'include'),
        '-MD','-MF',dep['path'],'-MT',obj['path'],'-c',source['path'],'-o',obj['path']]
    require(argv==expected and type(argv) is list and all(type(v) is str for v in argv) and command['returncode']==0 and
            all(argv.count(flag)==1 for flag in ('-MD','-MF','-MT','-c','-o')) and
            '-DNDEBUG' in argv and '-DLIBCXXABI_BAREMETAL' in argv and '-UNDEBUG' not in argv and
            argv[argv.index('-MF')+1]==dep['path'] and argv[argv.index('-MT')+1]==obj['path'] and
            argv[argv.index('-o')+1]==obj['path'] and argv[argv.index('-c')+1]==source['path'],
            'actual unchanged isolated official full-MD compile association required')
    return command


def same_device(previous,current,path,mappings):
    """Permit only an explicit matching device transition, not other changes."""
    require(type(previous) is int and type(current) is int and previous>=0 and current>=0,
            'integer historical/current device facts required')
    if previous==current:return
    matches=[r for r in mappings if Path(path).is_relative_to(Path(r['root'])) and
             r['historical_device']==previous and r['current_device']==current]
    require(matches,'catalog device differs without explicit finite restart mapping')


def compare_row(old,now,root,mappings,*,observed_external_regular=False):
    """Compare the entire original catalog row; no generic JSON projection."""
    require(type(old) is dict and type(now) is dict and set(old)==set(now) and
            old.get('kind') in ('file','symlink') and old.get('kind')==now.get('kind') and
            type(old.get('relative')) is str,'same complete original/current catalog row required')
    rel=Path(old['relative'])
    require(str(rel)==old['relative'] and not rel.is_absolute() and rel.parts and
            all(p not in ('.','..') for p in rel.parts),'safe catalog relative path required')
    require(all(old[k]==now[k] for k in old if k not in ('device','target_file')),
            'original catalog names/content/inode/times/link topology changed')
    same_device(old['device'],now['device'],Path(root)/rel,mappings)
    if 'target_file' in old:
        require(old['kind']=='symlink' and old['resolved']==old['target_file']['path'] and
                now['resolved']==now['target_file']['path'] and
                set(old['target_file'])==set(now['target_file'])==PIN_KEYS,
                'literal symlink target-file association changed')
        require(all(old['target_file'][k]==now['target_file'][k] for k in PIN_KEYS-{'device'}),
                'original regular symlink target changed')
        require(type(observed_external_regular) is bool,'explicit catalog target observation role required')
        # An observed external regular target is a finite name in this exact
        # original/current catalog pair, not an added selected compiler root.
        # Only the catalog source root's explicit device transition may apply.
        device_scope=Path(root)/rel if observed_external_regular else old['resolved']
        same_device(old['target_file']['device'],now['target_file']['device'],device_scope,mappings)
    return old.get('target_file'),now.get('target_file')


def _current_catalog_link(restart,root,row):
    """Recheck the literal source alias without opening an external parent root."""
    path=str(Path(root)/row['relative']);parent,name=restart._parent(path)
    try:
        before=os.stat(name,dir_fd=parent,follow_symlinks=False)
        require(stat.S_ISLNK(before.st_mode) and os.readlink(name,dir_fd=parent)==row['target'] and
                all(getattr(before,attr)==row[key] for attr,key in (('st_dev','device'),('st_ino','inode'),
                    ('st_mtime_ns','mtime_ns'),('st_ctime_ns','ctime_ns'))) and
                str(Path(path).resolve(strict=True))==row['resolved'],
                'actual catalog source alias changed')
        require(restart._same_stat(before,os.stat(name,dir_fd=parent,follow_symlinks=False)),
                'catalog source alias changed during observation')
        restart._verify_named_file(path,parent,name,before)
    finally:os.close(parent)


def _observe_catalog_target(frame,evidence,previous,current,old_catalog_current,old,now,ledger):
    """Hash one exact external REGULAR observation, never an Evidence input.

    Only catalog_pair calls this role, inside its complete two-catalog digest
    and held-descriptor comparison. Both raw rows and both actual catalog pins
    remain attached to the separate observation. This is no generic pathname
    admission fallback and supplies no parent-directory/provider authority.
    """
    require(type(ledger) is dict and len(ledger)<=4096,'finite separate external catalog observation ledger required')
    root=previous['root'];require(root==current['root'] and Path(root) in frame.roots and
        current in frame.catalogs and old.get('kind')==now.get('kind')=='symlink' and
        'target_file' in old and 'target_file' in now,'same complete selected catalog symlink source required')
    a,b=compare_row(old,now,root,evidence.mappings,observed_external_regular=True)
    evidence.restart.historical_pin(a);evidence.restart.historical_pin(b)
    path=Path(a['path'])
    require(path.is_absolute() and path.resolve(strict=True)==path and
        not (path==frame.source or path.is_relative_to(frame.source) or
             any(path==selected or path.is_relative_to(selected) for selected in frame.roots)),
        'dedicated role accepts only external canonical catalog regular targets')
    require(a['bytes']<=512*MIB,'external observed catalog regular target exceeds512MiB')
    witness={'source_root':root,'relative':old['relative'],'original_catalog_historical':previous['catalog'],
        'original_catalog_current':old_catalog_current,'current_catalog':current['catalog'],
        'original_source_row':old,'current_source_row':now}
    _current_catalog_link(evidence.restart,root,now)
    prior=ledger.get(str(path))
    if prior is None:
        require(len(ledger)<4096,'external catalog target count exceeds finite4096 bound')
        if a['device']!=b['device']:
            # compare_row already requires the explicit selected source-root
            # transition. The strict bridge reopens the actual external file
            # through held nofollow descriptors and verifies every full7 field.
            bridge=evidence.restart.requalify_persistent_pin(a,str(path),frame.guard,
                historical_device=a['device'],current_device=b['device'],observed_current=b)
            actual=bridge['current_pin']
        else:
            actual=evidence.restart.opened_file(str(path),a['bytes'],frame.guard)
            require(actual==a==b,'same-device external catalog regular target changed')
            bridge=None
        ledger[str(path)]={'historical_pin':a,'current_pin':actual,'device_bridge':bridge,
            'scope':'COMPLETE_CATALOG_OBSERVED_EXTERNAL_REGULAR_TARGET_ONLY_NOT_SELECTED_COMPILER_INPUT',
            'parent_directory_authority_granted':False,'source_witnesses':[witness]}
    else:
        require(prior['historical_pin']==a and prior['current_pin']==b,
            'repeated external catalog target identity differs')
        if witness not in prior['source_witnesses']:prior['source_witnesses'].append(witness)
    _current_catalog_link(evidence.restart,root,now)
    # Deliberately do not call Evidence.pin, fill Evidence.cache, add Frame
    # roots/inputs/pins or rewrite original rows. Consumed MD/official source
    # dependencies must still pass the unchanged selected-input accessor.
    return ledger[str(path)]


def preserve_catalog_observations(frame,evidence,ledger):
    """Reopen all finite observations after MD/official proof, before sidecar.

    Normal Frame.finish separately repeats its whole before/after catalogs,
    whose actual target_file rows cover these same external observations after
    Guest compilation. Nothing here replaces that terminal preservation.
    """
    require(type(ledger) is dict and len(ledger)<=4096,'finite separate catalog observations required')
    for path,row in ledger.items():
        frame.guard();actual=evidence.restart.opened_file(path,row['current_pin']['bytes'],frame.guard)
        require(actual==row['current_pin'],'observed external catalog target changed before qualification publication')
        for witness in row['source_witnesses']:
            _current_catalog_link(evidence.restart,witness['source_root'],witness['current_source_row'])
    return [ledger[path] for path in sorted(ledger)]


def catalog_pair(frame,evidence,previous,current,*,observed_targets=None):
    """Stream both complete authentic JSONL files with independent held fds.

    Common's original catalog scope contains files and literal symlinks, not
    empty-directory metadata. This comparison preserves that exact scope;
    it makes no invented claim about historical directory or hardlink counts.
    """
    require(previous['root']==current['root'] and all(previous[k]==current[k] for k in ('files','bytes','symlinks')),
            'complete original/current root counts differ')
    oldpin=evidence.pin(previous['catalog']);newpin=frame.pin(current['catalog']['path'])
    require(newpin==current['catalog'] and oldpin['sha256']==previous['content_sha256'] and
            newpin['sha256']==current['content_sha256'],'complete root catalog digest association differs')
    restart=evidence.restart;held=[];targets=[]
    try:
        for pin in (oldpin,newpin):
            fd=None
            require(0<=pin['bytes']<=512*MIB,'bounded complete CPP prerequisite catalog required')
            parent,name=restart._parent(pin['path'])
            try:
                fd=os.open(name,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC,dir_fd=parent)
                info=os.fstat(fd)
                require(stat.S_ISREG(info.st_mode) and info.st_size==pin['bytes'] and
                        all(getattr(info,a)==pin[b] for a,b in (('st_dev','device'),('st_ino','inode'),
                            ('st_mtime_ns','mtime_ns'),('st_ctime_ns','ctime_ns'))),'opened CPP prerequisite catalog changed')
            except BaseException:
                if fd is not None:os.close(fd)
                os.close(parent);raise
            held.append((parent,name,fd,info,pin))
        streams=[os.fdopen(item[2],'rb',closefd=False) for item in held]
        digests=[hashlib.sha256(),hashlib.sha256()];sizes=[0,0]
        try:
            rows=[iter(lambda s=s:s.readline(65537),b'') for s in streams]
            for number,pair in enumerate(itertools.zip_longest(*rows)):
                frame.guard();require(number<262144 and all(v is not None and len(v)<=65536 and v.endswith(b'\n') for v in pair),
                    'bounded identical complete CPP prerequisite catalog row sets required')
                for i,line in enumerate(pair):digests[i].update(line);sizes[i]+=len(line)
                old,now=map(decode,pair)
                external=False
                if old.get('kind')=='symlink' and 'target_file' in old:
                    path=Path(old['target_file']['path'])
                    external=not (path==frame.source or path.is_relative_to(frame.source) or
                        any(path==selected or path.is_relative_to(selected) for selected in frame.roots))
                a,b=compare_row(old,now,previous['root'],evidence.mappings,observed_external_regular=external)
                if a is not None and external:
                    _observe_catalog_target(frame,evidence,previous,current,oldpin,old,now,observed_targets)
                elif a is not None:
                    actual=evidence.pin(a);require(actual==b,'current symlink regular-file target differs');targets.append(actual)
        finally:
            for stream in streams:stream.close()
        for i,(parent,name,fd,before,pin) in enumerate(held):
            require(sizes[i]==pin['bytes'] and digests[i].hexdigest()==pin['sha256'] and
                    restart._same_stat(before,os.fstat(fd)) and
                    restart._same_stat(before,os.stat(name,dir_fd=parent,follow_symlinks=False)),
                    'complete CPP prerequisite catalog changed during comparison')
            restart._verify_named_file(pin['path'],parent,name,before)
    finally:
        for parent,name,fd,info,pin in held:os.close(fd);os.close(parent)
    return {'root':previous['root'],'original':previous,'current':current,'regular_link_targets_current':targets,
            'relation':'ALL_ORIGINAL_ROWS_EXACT_EXCEPT_EXPLICIT_DEVICE_MAPPING','execution':'NOT_PERFORMED'}


def qualify(frame,evidence,label,values,support):
    """Independently qualify a library input and write a real current sidecar."""
    frame.guard();report=evidence.selected(label);proof=terminal(report)
    original_source,frozen=evidence.source(report)
    config=evidence.document(report['configuration']);historical_cg=config['cgroup']
    require(historical_cg['memory_max']==64*1024**3 and historical_cg['memory_swap_max']==0 and
            historical_cg['expected_leaf']==frame.config['cgroup']['expected_leaf'] and
            historical_cg['expected_cpus']==frame.config['cgroup']['expected_cpus'],
            'original hard64/swap0 scope must remain historical provenance of this owned leaf')
    # Check only consistency of the two raw historical observations here.
    # Neither is presented as the current cgroup admission.
    frame.common.cgroup_preserved(report['cgroup_before'],report['cgroup_after'])
    old_observation=report['cgroup_before']
    require(all(old_observation[k]==historical_cg[v] for k,v in (
        ('boot_id','expected_boot_id'),('device','expected_device'),('inode','expected_inode'),
        ('path','expected_leaf'),('cpus','expected_cpus'),('memory_max','memory_max'),
        ('memory_swap_max','memory_swap_max'))),'original cgroup configuration/observation association differs')
    frame.common.cgroup_preserved(frame.before,frame.common.cgroup_state(frame.config['cgroup']))
    roots=config['input_roots'];inventories=report['inventories_after']
    require(type(roots) is list and len(roots)<=64 and len(set(roots))==len(roots) and
            type(inventories) is list and len(inventories)==len(roots) and
            {v['root'] for v in inventories}==set(roots),'complete original object prerequisite inventories required')
    root_proofs=[];observed_targets={}
    for ordinal,old in enumerate(inventories):
        path=Path(old['root']);require(path in frame.roots,'all original CPP prerequisite roots must be whole current Frame inputs')
        matches=[r for r in frame.catalogs if r['root']==str(path)]
        require(len(matches)==1,'one actual current whole CPP prerequisite root catalog required')
        root_proofs.append(catalog_pair(frame,evidence,old,matches[0],observed_targets=observed_targets))
    for expected in report['input_pins']:evidence.pin(expected)
    producer=evidence.pin(report['producer'])
    require(producer['bytes']==27601 and producer['sha256']==PRODUCER_SHA,'unchanged original31340 object producer required')
    obj=evidence.pin(proof['object']);dep=evidence.pin(proof['dependency_file'])
    evidence.retained(proof['products']);md_helper=evidence.pin(proof['build_helper'])
    require(md_helper['bytes']==48461 and md_helper['sha256']=='a7e89266486dba5ada416a31841b12646482001089b45765b3e05ec7b24c050f',
            'unchanged original reviewed full-MD helper required')
    source=evidence.pin(proof['official']['files'][FILES[0]])
    compiler=str(Path(values['WASI_CLANGXX']).resolve(strict=True));command=compile_association(proof,obj,dep,source,compiler,values['WASI_SYSROOT'])
    actual=support.full_md(frame,Path(dep['path']),Path(obj['path']),Path(source['path']),Path(command['cwd']))
    require(actual==[evidence.pin(p) for p in command['dependencies']],'actual original object full-MD closure changed')
    labels=[name for name,p in frame.inputs.items() if p['path']==proof['official']['lock']['path']]
    require(len(labels)==1,'original official LLVM source-lock must be an explicit current input')
    # This context contains only actual Frame I/O and the existing Evidence
    # accessor. It supplies no synthetic source, status, physical pin or cgroup.
    context=types.SimpleNamespace(common=frame.common,input=frame.input,inputs=frame.inputs,evidence=evidence)
    official,_=official_sources(context,labels[0])
    require(set(official)==set(proof['official']) and all(official[k]==proof['official'][k] for k in official if k not in ('lock','transport_report')) and
            all(evidence.pin(official[k])==evidence.pin(proof['official'][k]) for k in ('lock','transport_report')),
            'same unchanged official Git/source qualification required')
    catalog_observations=preserve_catalog_observations(frame,evidence,observed_targets)
    qualification={'format_version':1,'kind':'independent-official-cpp-object-current-qualification','status':STATUS,
        'current_source':str(frame.source),'current_manifest':frame.pin(frame.manifest),
        'current_consumer_helper':frame.pin(frame.source/'tests/ros_unitree/independent_cpp_object.py'),
        'current_cgroup_configuration':frame.config['cgroup'],'current_cgroup_observation':frame.before,
        'original_report':frame.inputs[label],'original_configuration':report['configuration'],
        'original_source':report['source'],'original_source_manifest_sha256':report['source_manifest_sha256'],
        'original_cgroup_configuration':historical_cg,'original_object_producer':report['producer'],
        'original_overlay':proof,'actual_object':obj,'actual_producer':producer,'actual_full_MD':actual,
        'whole_current_root_proofs':root_proofs,'current_input_pins':list(evidence.cache.values()),
        'device_bridges':list(evidence.bridges),'whole_catalog_observed_external_regular_targets':catalog_observations,
        'catalog_observations_are_selected_compiler_inputs':False,'original_status_unchanged':True,
        'historical_actor_or_cgroup_continuity_claimed':False,'Typed14_main_required_or_promoted':False,
        'SDK_modified':False,'runtime_executed':False,'Guest_authority':[],
        'scope':'INDEPENDENT_OFFICIAL_LIBRARY_INPUT_ONLY_NOT_GUEST_OR_TYPED14_ACCEPTANCE'}
    path=frame.out/'independent-official-cpp-current-qualification.json'
    require(not path.exists() and not path.is_symlink(),'fresh independent CPP sidecar required')
    frame.common.atomic_json(path,qualification);current=frame.pin(path)
    return {'object':obj,'original_overlay_report':frame.inputs[label],'original_compilation_provenance':{
        'source':report['source'],'source_manifest_sha256':report['source_manifest_sha256'],
        'configuration':report['configuration'],'producer':report['producer'],'status':report['status']},
        'current_qualification':current,'actual_full_MD':actual,
        'scope':'QUALIFIED_OFFICIAL_LIBRARY_INPUT_ONLY_NO_TYPED14_GO_OR_HG_GUEST_REUSE'}
