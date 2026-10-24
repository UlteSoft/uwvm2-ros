"""Prepare literal source-path rebinding for physically relocated SDK checks."""
import argparse
import ast
import difflib
import hashlib
import json
import posixpath
from pathlib import Path, PurePosixPath
import re
import sys
import tarfile


def require(value,message):
    if not value:raise ValueError(message)


def digest(body):return hashlib.sha256(body).hexdigest()


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive',type=Path,required=True);p.add_argument('--sha256',required=True)
    p.add_argument('--source-map',type=Path,required=True);p.add_argument('--tools',type=Path,required=True)
    a=p.parse_args();sys.path.insert(0,str(a.tools.resolve(strict=True)))
    from resource_contract import validate_resource_owner
    proof=validate_resource_owner()
    with a.archive.open('rb')as stream:require(hashlib.file_digest(stream,'sha256').hexdigest()==a.sha256,'exact inert source capture required')
    mapping=json.loads(a.source_map.read_bytes())['sources'];files={}
    with tarfile.open(a.archive,'r:')as capture:
        for item in capture:
            if item.isfile()and item.name in mapping:files[item.name]=capture.extractfile(item).read()
    report=dict(kind='relocated-sdk-literal-source-rebinding-v1',status='PREPARED_NOT_APPLIED',resource=proof,
        source_capture_sha256=a.sha256,changes=[],unhandled=[],default_authority=[],actual_tests_executed=0)
    roots={}
    for source,row in mapping.items():
        if not source.startswith('sdk/origin/'):continue
        for parent in PurePosixPath(source).parents:
            if parent==PurePosixPath('.'):break
            tail=PurePosixPath(source).relative_to(parent)
            destination=PurePosixPath(row['destination'])
            if destination.parts[-len(tail.parts):]==tail.parts:
                roots.setdefault(str(parent),set()).add(str(destination.parents[len(tail.parts)-1]))
    def destination(path):
        if path in mapping:return mapping[path]['destination']
        options=roots.get(path,set())
        return next(iter(options))if len(options)==1 else path
    def relative(source,target,spelling):
        if not spelling.startswith('.'):return spelling
        before=posixpath.normpath(posixpath.join(str(PurePosixPath(source).parent),spelling))
        after=destination(before)
        if after==before and not PurePosixPath(before).suffix:
            for extension in('.ts','.mjs','.js','.rs','.zig'):
                if before+extension in mapping:
                    after=str(PurePosixPath(mapping[before+extension]['destination']).with_suffix(''));break
        selected=mapping[source]['destination']
        rebound=posixpath.relpath(after,str(PurePosixPath(selected).parent))
        return rebound if rebound.startswith('.')else'./'+rebound
    patches=[]
    for source,raw in files.items():
        if mapping[source]['disposition']=='EXACT_CONTENT_AND_PROVENANCE_ALIAS':continue
        target=mapping[source]['destination']
        if '/support/records/'in target:continue
        suffix=PurePosixPath(source).suffix
        if suffix not in('.py','.mjs','.js','.ts','.rs','.zig','.c','.cpp','.cc','.h','.hpp','.md','.rb','.php','.lua','.R','.toml','.mod','.csproj','.fsproj','.vbproj'):continue
        try:body=raw.decode('utf-8')
        except UnicodeDecodeError:continue
        kinds=[]
        if suffix=='.py':
            tree=ast.parse(body,filename=source);values={};edits=[]
            def value(node):
                if isinstance(node,ast.Name):return values.get(node.id)
                if isinstance(node,ast.Call):
                    if isinstance(node.func,ast.Name)and node.func.id=='Path'and len(node.args)==1 and isinstance(node.args[0],ast.Name)and node.args[0].id=='__file__':return source
                    if isinstance(node.func,ast.Attribute)and node.func.attr in('resolve','absolute')and not node.args:return value(node.func.value)
                    if isinstance(node.func,ast.Attribute)and node.func.attr=='with_name'and len(node.args)==1 and isinstance(node.args[0],ast.Constant)and isinstance(node.args[0].value,str):
                        base=value(node.func.value)
                        if base:return str(PurePosixPath(base).with_name(node.args[0].value))
                if isinstance(node,ast.Attribute)and node.attr=='parent':
                    base=value(node.value)
                    if base:return str(PurePosixPath(base).parent)
                if isinstance(node,ast.Subscript)and isinstance(node.value,ast.Attribute)and node.value.attr=='parents'and isinstance(node.slice,ast.Constant)and type(node.slice.value)is int:
                    base=value(node.value.value)
                    if base and 0<=node.slice.value<len(PurePosixPath(base).parents):return str(PurePosixPath(base).parents[node.slice.value])
                if isinstance(node,ast.BinOp)and isinstance(node.op,ast.Div)and isinstance(node.right,ast.Constant)and isinstance(node.right.value,str):
                    base=value(node.left)
                    if base:return posixpath.normpath(posixpath.join(base,node.right.value))
                return None
            for node in tree.body:
                if isinstance(node,ast.Assign):
                    resolved=value(node.value)
                    if resolved is not None:
                        for name in node.targets:
                            if isinstance(name,ast.Name):values[name.id]=resolved
            lines=raw.splitlines(keepends=True);starts=[0]
            for line in lines:starts.append(starts[-1]+len(line))
            def span(node):return starts[node.lineno-1]+node.col_offset,starts[node.end_lineno-1]+node.end_col_offset
            def text(node):
                start,end=span(node);return raw[start:end].decode()
            def add(node,replacement,kind):
                start,end=span(node)
                if any(not(end<=a or start>=b)for a,b,_,_ in edits):return
                if replacement!=text(node):edits.append((start,end,replacement.encode(),kind))
            def walk(node):
                # Preserve a caller's SDK/ROOT variable when selecting a moved
                # path; synthetic source-model roots therefore still work.
                if isinstance(node,ast.BinOp)and isinstance(node.op,ast.Div):
                    parts=[];base=node
                    while isinstance(base,ast.BinOp)and isinstance(base.op,ast.Div):parts.insert(0,base.right);base=base.left
                    resolved=value(base)
                    if isinstance(base,ast.Name)and resolved is not None:
                        if resolved=='sdk/origin':
                            add(node,'_sdk_path('+text(base)+', '+', '.join(text(part)for part in parts)+')','SDK path resolver');return
                        if resolved.startswith('sdk/origin/'):
                            arguments=[repr(resolved[len('sdk/origin/'):]),*(text(part)for part in parts)]
                            add(node,"_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', "+', '.join(arguments)+')','SDK helper path resolver');return
                        if resolved=='.'and parts and isinstance(parts[0],ast.Constant)and isinstance(parts[0].value,str)and(parts[0].value=='sdk/origin'or parts[0].value.startswith('sdk/origin/')):
                            first=ast.Constant(value=parts[0].value[len('sdk/origin/'):])
                            arguments=([repr(first.value)]if first.value else[])+[text(part)for part in parts[1:]]
                            if not arguments:return
                            add(node,'_sdk_path('+text(base)+" / 'sdk/origin', "+', '.join(arguments)+')','repository SDK path resolver');return
                resolved=value(node)
                # Only file-derived anchors are fixed here. Ordinary caller
                # paths and strings that model an input contract stay intact.
                if resolved is not None and not isinstance(node,ast.Name)and '__file__'in text(node):
                    selected=destination(resolved)
                    if resolved=='.'or resolved.startswith('sdk/origin')or resolved in mapping:
                        add(node,'_CHECKOUT_ROOT'+(' / '+repr(selected)if selected!='.'else''),'file-derived source anchor');return
                for child in ast.iter_child_nodes(node):walk(child)
            walk(tree)
            for start,end,replacement,kind in sorted(edits,reverse=True):raw=raw[:start]+replacement+raw[end:];kinds.append(kind)
            if edits:
                tree=ast.parse(raw,filename=target)
                insertion=0
                for node in tree.body:
                    if isinstance(node,ast.Expr)and isinstance(node.value,ast.Constant)and isinstance(node.value.value,str)or isinstance(node,ast.ImportFrom)and node.module=='__future__':insertion=node.end_lineno
                    else:break
                parts=raw.splitlines(keepends=True)
                bootstrap=("\n# Test sources are separate from the selected production SDK.\n"
                    "import sys as _source_sys\nfrom pathlib import Path as _SourcePath\n"
                    "_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())\n"
                    "_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))\n"
                    "from source_paths import sdk_path as _sdk_path\n\n").encode()
                raw=b''.join(parts[:insertion])+bootstrap+b''.join(parts[insertion:]);ast.parse(raw,filename=target)
                body=raw.decode()
        elif suffix in('.mjs','.js','.ts'):
            pattern=r'(\bfrom\s*["\x27]|\bimport\s*["\x27]|\bimport\s*\(\s*["\x27]|\bnew\s+URL\(\s*["\x27])(\.[^"\x27\n]+)(["\x27])'
            body=re.sub(pattern,lambda m:m[1]+relative(source,target,m[2])+m[3],body);kinds.append('literal ECMAScript module paths')
        elif suffix in('.rs','.zig'):
            pattern=r'(#\s*\[\s*path\s*=\s*"|@import\(\s*")(\.[^"\n]+)(")'
            body=re.sub(pattern,lambda m:m[1]+relative(source,target,m[2])+m[3],body);kinds.append('literal native module paths')
        elif suffix in('.c','.cpp','.cc','.h','.hpp'):
            body=re.sub(r'(#\s*include\s*")(\.[^"\n]+)(")',lambda m:m[1]+relative(source,target,m[2])+m[3],body);kinds.append('quoted source includes')
        elif suffix=='.md':
            body=re.sub(r'(\]\()(\.[^)\n#]+)(#[^)\n]*)?(\))',lambda m:m[1]+relative(source,target,m[2])+(m[3]or'')+m[4],body)
            for original in sorted(mapping,key=len,reverse=True):
                if original.startswith('sdk/origin/'):body=body.replace(original,mapping[original]['destination'])
            kinds.append('editable source documentation links')
        elif suffix=='.rb':
            body=re.sub(r'(require_relative\s+["\x27])(\.[^"\x27\n]+)(["\x27])',lambda m:m[1]+relative(source,target,m[2])+m[3],body);kinds.append('Ruby source include')
        elif suffix=='.php':
            body=re.sub(r'(__DIR__\s*\.\s*["\x27])/(\.[^"\x27\n]+)(["\x27])',lambda m:m[1]+'/'+relative(source,target,m[2])+m[3],body);kinds.append('PHP source include')
        elif suffix in('.toml','.mod'):
            pattern=r'((?:\b(?:path|license-file)\s*=\s*"|=>\s*))(\.{1,2}/[^"\s\n]+)'
            body=re.sub(pattern,lambda m:m[1]+relative(source,target,m[2]),body);kinds.append('explicit test package paths')
        elif suffix in('.csproj','.fsproj','.vbproj'):
            body=re.sub(r'((?:Include|Project)\s*=\s*")(\.{1,2}/[^"\n]+)(")',
                lambda m:m[1]+relative(source,target,m[2])+m[3],body);kinds.append('explicit managed test project paths')
        elif suffix in('.lua','.R')and re.search(r'__file__|\.\./|getwd\(|scriptdir|debug\.getinfo',body):
            report['unhandled'].append(dict(source=source,destination=target,reason='review dynamic language root selection explicitly'))
        updated=body.encode()
        if updated!=files[source]:
            patches.extend(difflib.unified_diff(files[source].decode().splitlines(keepends=True),
                body.splitlines(keepends=True),fromfile='a/'+target,tofile='b/'+target))
            report['changes'].append(dict(original=source,destination=target,before_sha256=digest(files[source]),
                after_sha256=digest(updated),kinds=sorted(set(kinds))))
    Path('source-rebindings.json').write_text(json.dumps(report,indent=2,sort_keys=True)+'\n')
    Path('source-rebinding.patch').write_text(''.join(patches))
    print(json.dumps({key:value for key,value in report.items()if key!='changes'}),flush=True)


if __name__=='__main__':main()
