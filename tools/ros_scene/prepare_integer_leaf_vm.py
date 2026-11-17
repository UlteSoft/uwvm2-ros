#!/usr/bin/env python3
"""Derive a closed private-integer-leaf experiment from the symbol-qualified VM.

Only SDK .tmp sources are patched. The existing public-entry and Intel observer
owners remain unchanged. An explicit trusted compile-time opt-in inserts one
post-O3 pass; live runtime/compiler sources are never modified.
"""
import argparse,difflib,hashlib,json,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
BASE_MANIFEST='d6123791f1d032e4777c4e048c7c4d04436ba0ed06c674ba6cf8636c38a308b4'
MACRO='UWVM_SCENE_INTEGER_LEAF_QUALIFICATION'
def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--unit-receipt',type=Path,required=True);a=p.parse_args()
    source=a.source.resolve();out=a.output.resolve();tmp=(ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve()
    assert source.is_relative_to(tmp) and out.is_relative_to(tmp) and not out.exists()
    manifest=source/'source-manifest.json';assert sha(manifest)==BASE_MANIFEST
    files=json.loads(manifest.read_text());assert all(sha(source/n)==v for n,v in files.items())
    helper=ROOT/'tools/ros_scene/qualification_integer_leaf_inline.h'
    unit=json.loads(a.unit_receipt.read_text());assert unit['complete'] and unit['inputs'][str(helper)]==sha(helper)
    shutil.copytree(source,out,symlinks=True)
    runtime='src/uwvm2/runtime/lib/uwvm_runtime.default.cpp';path=out/runtime;before=path.read_text();text=before
    changes=[('include','# include "uwvm_runtime_intel_jit_listener.h"\n',
        '# include "uwvm_runtime_intel_jit_listener.h"\n# if defined('+MACRO+') && '+MACRO+' == 1\n#  include "qualification_integer_leaf_inline.h"\n# endif\n')]
    anchor='''                if(!details::outline_runtime_llvm_jit_gc_private_fallbacks(*merged_module,gc_private_clones))
                { return false; }
'''
    insertion='''#if defined(UWVM_SCENE_INTEGER_LEAF_QUALIFICATION) && UWVM_SCENE_INTEGER_LEAF_QUALIFICATION == 1
                // Trusted experiment: only exact typed entries owned by this
                // materializer; public/raw/indirect identities stay untouched.
                try
                {
                    if(rec.runtime_module == nullptr) { return false; }
                    auto const& owner_module{*rec.runtime_module};
                    auto const imported_count{owner_module.imported_function_vec_storage.size()};
                    auto const& owned_locals{rec.llvm_jit_compiled.local_funcs};
                    if(owned_locals.size() != owner_module.local_defined_function_vec_storage.size()) { return false; }
                    ::uwvm2::utils::container::std_vector<::llvm::Function*> typed_entries{};
                    for(::std::size_t index{}; index != owned_locals.size(); ++index)
                    {
                        auto const function_index{imported_count + index};
                        if(owned_locals.index_unchecked(index).function_index != function_index) { return false; }
                        auto const name{get_runtime_llvm_jit_wasm_function_name(owner_module, function_index)};
                        auto* function{merged_module->getFunction(::llvm::StringRef{
                            reinterpret_cast<char const*>(name.data()), name.size()})};
                        if(function != nullptr) { typed_entries.push_back(function); }
                    }
                    auto const result{::uwvm2::runtime::qualification::integer_leaf_inline::run(*merged_module, typed_entries)};
                    ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                        u8"[integer-leaf-qualification] leaves=", result.leaves, u8" edges=", result.edges,
                        u8" rejected-attrs=", result.rejected_attributes, u8" rejected-metadata=", result.rejected_metadata,
                        u8" rejected-tail=", result.rejected_tail, u8" rejected-shape=", result.rejected_shape,
                        u8" status=", static_cast<unsigned>(result.state));
                    if(result.state != ::uwvm2::runtime::qualification::integer_leaf_inline::status::complete) { return false; }
                }
                catch(...) { return false; }
#endif
'''
    changes.append(('post-optimizer-closed-pass',anchor,anchor+insertion))
    for label,old,new in changes:
        assert text.count(old)==1,(label,text.count(old));text=text.replace(old,new,1)
    path.write_text(text)
    target='src/uwvm2/runtime/lib/qualification_integer_leaf_inline.h';shutil.copyfile(helper,out/target)
    manifest_files={n:sha(out/n) for n in sorted(set(files)|{target})}
    (out/'source-manifest.json').write_text(json.dumps(manifest_files,indent=2)+'\n')
    # The inherited observer remains exactly the previously qualified integration;
    # pin its parent receipt and explicitly record the derived frozen manifest.
    inherited=out/'intel-jit-qualification.json';info=json.loads(inherited.read_text())
    info['parent_qualification_sha256']=sha(source/'intel-jit-qualification.json')
    info['qualified_manifest_sha256']=sha(out/'source-manifest.json')
    info['integer_leaf_experiment']=True;inherited.write_text(json.dumps(info,indent=2)+'\n')
    patch=''.join(difflib.unified_diff(before.splitlines(True),text.splitlines(True),fromfile='symbol/'+runtime,tofile='integer-leaf/'+runtime))
    (out/'integer-leaf-qualification.patch').write_text(patch)
    report=dict(complete=True,scope=__doc__,production_integration=False,trusted_compile_time_opt_in=MACRO,
        source=str(source),parent_manifest_sha256=BASE_MANIFEST,qualified_manifest_sha256=sha(out/'source-manifest.json'),
        source_files=len(manifest_files),unit_receipt=str(a.unit_receipt.resolve()),unit_receipt_sha256=sha(a.unit_receipt),
        helper_sha256=sha(helper),runner_sha256=sha(__file__),replacements=[x[0] for x in changes],
        deltas=[dict(file=runtime,before=files[runtime],after=sha(path)),dict(file=target,before=None,after=sha(helper))],
        patch_sha256=sha(out/'integer-leaf-qualification.patch'),max_instructions=32,ordinary_full_only=True,compile_threads=0)
    (out/'integer-leaf-qualification.json').write_text(json.dumps(report,indent=2)+'\n')
    assert all(sha(source/n)==v for n,v in files.items())
    print('Prepared',out,len(manifest_files),'files')
if __name__=='__main__':main()
