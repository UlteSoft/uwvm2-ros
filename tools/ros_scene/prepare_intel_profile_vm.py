#!/usr/bin/env python3
"""Prepare a finite, symbol-only Intel profiling variant of one frozen VM.

This never patches the live runtime. The accepted base manifest is exact;
every source replacement requires a unique literal anchor. The variant is a
trusted qualification artifact, not production profiling support. It supports
ordinary full LLVM, zero extra compile threads, max policy and disabled object
cache; debug/checkpoint, replacement and threaded-child materializers are denied.
Original guest source rows are deliberately absent.
"""
import argparse
import difflib
import hashlib
import json
from pathlib import Path
import shutil

ROOT=Path(__file__).resolve().parents[2]
BASE_MANIFEST='b8abeae850d52c076866ef8c23e185837723d00e617b4c970f480584cedaa2d5'
MACRO='UWVM_SCENE_INTEL_JIT_QUALIFICATION'
ON=f'#if defined({MACRO}) && {MACRO} == 1\n'

def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',required=True,type=Path)
    p.add_argument('--helper-receipt',required=True,type=Path)
    p.add_argument('--output',required=True,type=Path)
    a=p.parse_args();source=a.source.resolve();out=a.output.resolve()
    assert source.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve()) and not out.exists()
    manifest=source/'source-manifest.json';assert sha(manifest)==BASE_MANIFEST
    files=json.loads(manifest.read_text());assert all(sha(source/n)==v for n,v in files.items())
    helper=json.loads(a.helper_receipt.read_text());assert helper['complete']
    headers=['src/uwvm2/runtime/lib/uwvm_runtime_intel_jit_profile.h','src/uwvm2/runtime/lib/uwvm_runtime_intel_jit_listener.h']
    for name in headers:assert helper['inputs'][str(ROOT/name)]==sha(ROOT/name)
    # The qualified helper allocation protocol must match the frozen VM.
    for name in files:
        if (name.startswith('src/uwvm2/utils/allocator/fast_io_strict/') and name.endswith('.h')) or name=='src/uwvm2/utils/container/standard.h':
            assert sha(ROOT/name)==files[name],name
    shutil.copytree(source,out,symlinks=True)
    replacements=[];deltas=[]
    def patch(name,changes):
        path=out/name;before=path.read_text();text=before
        for label,old,new in changes:
            assert text.count(old)==1,(label,text.count(old))
            text=text.replace(old,new,1);replacements.append(dict(file=name,label=label))
        path.write_text(text)
        deltas.append(dict(file=name,before=hashlib.sha256(before.encode()).hexdigest(),after=sha(path)))
        return ''.join(difflib.unified_diff(before.splitlines(True),text.splitlines(True),fromfile='base/'+name,tofile='qualified/'+name))
    runtime='src/uwvm2/runtime/lib/uwvm_runtime.default.cpp'
    engine_decl='            ::uwvm2::utils::container::delete_owned_ptr<::llvm::ExecutionEngine> engine{};\n            explicit full_code_publication'
    include='#  include <llvm/ExecutionEngine/MCJIT.h>\n'
    retire='            rec.llvm_jit_full_publication->engine.reset();\n'
    local='            ::uwvm2::utils::container::delete_owned_ptr<::llvm::ExecutionEngine> llvm_jit_engine{raw_engine};\n'
    adopt='            rec.llvm_jit_full_publication->context = ::std::move(llvm_context_holder);\n            rec.llvm_jit_full_publication->engine = ::std::move(llvm_jit_engine);\n'
    exit_release='                // Preserve the existing late-LLVM static destruction exception.\n                static_cast<void>(engine.release());\n'
    terminal='                if(llvm_jit_full_publication)\n                { static_cast<void>(llvm_jit_full_publication.release()); }\n'
    compiler='            auto const compile_uwvm_int_translation{runtime_compiler_requests_uwvm_int_translation()};\n\n            // Compilation and reset share one publication lock.'
    policy='''            namespace profile_mode = ::uwvm2::uwvm::runtime::runtime_mode;
            if(!compile_llvm_jit_translation || compile_uwvm_int_translation ||
               profile_mode::global_runtime_mode != profile_mode::runtime_mode_t::full_compile ||
               profile_mode::global_runtime_compile_threads_resolved != 0u ||
               profile_mode::global_runtime_llvm_jit_policy != profile_mode::runtime_llvm_jit_policy_t::max ||
               profile_mode::global_runtime_llvm_jit_cache_path_mode != profile_mode::runtime_llvm_jit_cache_path_mode_t::disabled ||
               g_runtime.debug_pause_control || g_runtime.checkpoint_profile)
            {
                ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                    u8"[intel-jit-qualification] unsupported runtime: require ordinary full LLVM, max, compile-threads=0, cache=disable; no debug/checkpoint");
                ::_exit(78);
            }
'''
    final_check='''            if(llvm_jit_engine->hasError() || memory_manager_observer->has_finalization_failure() ||
               !intel_profile_owner || intel_profile_owner->observation_failed() ||
               !intel_profile_owner->publish_finalized() ||
               (intel_profile_owner->state() == ::uwvm2::runtime::lib::details::intel_jit_profile::activation::ready &&
                intel_profile_owner->methods() == 0u))
            {
                llvm_jit_materialize_error(u8"Intel qualification: finalized code observation failed before entry publication.");
                return false;
            }
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                u8"[intel-jit-qualification] finalized-symbols=", intel_profile_owner->methods(),
                u8" original-guest-source-lines=unavailable");
'''
    replacement='''    {
        if(!production_update && (!::uwvm2::utils::control::debug_permissions::full_allowed() ||
'''
    # Require the owner field's reverse destruction order and explicit reset,
    # abort and process-exit paths. No numeric PC map owns this registration.
    changes=[
        ('sdk_include',include,include+ON+'# if !defined(__linux__) || !defined(__x86_64__) || !defined(UWVM_RUNTIME_INTEL_JIT_PROFILE) || UWVM_RUNTIME_INTEL_JIT_PROFILE != 1\n#  error "Intel qualification requires Linux x86_64 and the explicitly selected official SDK"\n# endif\n# include "uwvm_runtime_intel_jit_listener.h"\n#endif\n'),
        ('arena_owner',engine_decl,'            ::uwvm2::utils::container::delete_owned_ptr<::llvm::ExecutionEngine> engine{};\n'+ON+'            // Reverse destruction: observer UNLOAD/detach, then the actual engine arena.\n            ::uwvm2::runtime::lib::details::intel_jit_profile::listener_owner intel_profile{};\n#endif\n            explicit full_code_publication'),
        ('explicit_reset',retire,ON+'            rec.llvm_jit_full_publication->intel_profile.reset();\n#endif\n'+retire),
        ('attach_before_materialization',local,local+ON+'            auto intel_profile_owner = ::uwvm2::runtime::lib::details::intel_jit_profile::make_listener_owner(\n                *llvm_jit_engine, true, "uwvm.qualification.full");\n            if(!intel_profile_owner)\n            { llvm_jit_materialize_error(u8"Intel qualification observer allocation failed."); return false; }\n#endif\n'),
        ('publish_finalized_and_adopt',adopt,ON+final_check+'#endif\n'+adopt+ON+'            rec.llvm_jit_full_publication->intel_profile = ::std::move(intel_profile_owner);\n#endif\n'),
        ('retained_arena_exit',exit_release,'                // Preserve the existing late-LLVM static destruction exception.\n'+ON+'                if(intel_profile) { intel_profile->detach_keep_code(); static_cast<void>(intel_profile.release()); }\n#endif\n                static_cast<void>(engine.release());\n'),
        ('terminal_publication_leak',terminal,'                if(llvm_jit_full_publication)\n                {\n'+ON+'                    if(llvm_jit_full_publication->intel_profile) { llvm_jit_full_publication->intel_profile->detach_keep_code(); }\n#endif\n                    static_cast<void>(llvm_jit_full_publication.release());\n                }\n'),
        ('closed_launch_scope',compiler,'            auto const compile_uwvm_int_translation{runtime_compiler_requests_uwvm_int_translation()};\n'+ON+policy+'#endif\n\n            // Compilation and reset share one publication lock.'),
        ('deny_replacement',replacement,'    {\n'+ON+'        return {llvm_jit_debug_replace_status::unsupported_mode, 0u, nullptr};\n#endif\n        if(!production_update && (!::uwvm2::utils::control::debug_permissions::full_allowed() ||\n'),
    ]
    diff=patch(runtime,changes)
    child='src/uwvm2/runtime/lib/uwvm_runtime_threaded_llvm_child.h'
    anchor='''        {
            auto result=build(world,compiler,backend_storage_budget,0u);
'''
    diff+=patch(child,[('deny_threaded_child',anchor,'        {\n'+ON+'            return {{},preparation_status::unsupported_shape};\n#endif\n            auto result=build(world,compiler,backend_storage_budget,0u);\n')])
    for name in headers:
        target=out/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(ROOT/name,target)
        deltas.append(dict(file=name,before=files.get(name),after=sha(target)))
    # Enumerate every file in the frozen logical source/fixture manifest, plus
    # the two new helper files; do not follow unrelated third-party symlinks.
    manifest_files={name:sha(out/name) for name in sorted(set(files)|set(headers))}
    (out/'source-manifest.json').write_text(json.dumps(manifest_files,indent=2)+'\n')
    (out/'intel-jit-qualification.patch').write_text(diff)
    report=dict(complete=True,scope=__doc__,trusted_compile_time_opt_in=MACRO,source=str(source),source_manifest_sha256=BASE_MANIFEST,
        source_files=len(manifest_files),qualified_manifest_sha256=sha(out/'source-manifest.json'),helper_receipt=str(a.helper_receipt.resolve()),
        helper_receipt_sha256=sha(a.helper_receipt),helper_inputs={name:sha(ROOT/name) for name in headers},deltas=deltas,
        replacements=replacements,patch_sha256=sha(out/'intel-jit-qualification.patch'),runner_sha256=sha(__file__),
        ordinary_full_only=True,extra_compile_threads=0,object_cache_supported=False,debug_supported=False,
        threaded_child_supported=False,replacement_supported=False,guest_source_mapping_supported=False,production_integration=False)
    (out/'intel-jit-qualification.json').write_text(json.dumps(report,indent=2)+'\n')
    assert all(sha(source/n)==v for n,v in files.items())
    print('Prepared',out,len(manifest_files),'files;',len(replacements),'literal replacements')

if __name__=='__main__':main()
