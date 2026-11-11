// Explicit serialization of the pinned LLVM TargetMachine inputs. Never hash
// C++ padding, pointers, diagnostic destinations or original command-line text.
// Keep the field coverage test synchronized when upgrading bundled LLVM.
#pragma once
#if defined(UWVM_RUNTIME_LLVM_JIT)
UWVM_MODULE_EXPORT namespace uwvm2::runtime::llvm_jit_cache
{
    [[nodiscard]] inline ::uwvm2::utils::container::u8string target_machine_identity(
        ::llvm::TargetMachine const& machine) noexcept
    {
        auto out{details::make_cache_key(u8"llvm-target-options-v1")};
        auto integer = [&](auto const& name, auto value) noexcept
        { details::append_cache_key_value_u64(out, name, static_cast<::std::uint_least64_t>(value)); };
        auto string = [&](auto const& name, auto const& value) noexcept
        {
            details::append_cache_key_value(out, name, ::uwvm2::utils::container::u8string_view{
                reinterpret_cast<char8_t const*>(value.data()), value.size()});
        };
        integer(u8"codegen-opt", machine.getOptLevel());
        integer(u8"relocation-model", machine.getRelocationModel());
        integer(u8"code-model", machine.getCodeModel());
        auto const& o{machine.Options};
        integer(u8"BinutilsVersion.major", o.BinutilsVersion.first);
        integer(u8"BinutilsVersion.minor", o.BinutilsVersion.second);
        integer(u8"NoTrappingFPMath", o.NoTrappingFPMath);
        integer(u8"EnableAIXExtendedAltivecABI", o.EnableAIXExtendedAltivecABI);
        integer(u8"HonorSignDependentRoundingFPMathOption", o.HonorSignDependentRoundingFPMathOption);
        integer(u8"NoZerosInBSS", o.NoZerosInBSS);
        integer(u8"GuaranteedTailCallOpt", o.GuaranteedTailCallOpt);
        integer(u8"StackSymbolOrdering", o.StackSymbolOrdering);
        integer(u8"EnableFastISel", o.EnableFastISel);
        integer(u8"EnableGlobalISel", o.EnableGlobalISel);
        integer(u8"UseInitArray", o.UseInitArray);
        integer(u8"DisableIntegratedAS", o.DisableIntegratedAS);
        integer(u8"FunctionSections", o.FunctionSections);
        integer(u8"DataSections", o.DataSections);
        integer(u8"IgnoreXCOFFVisibility", o.IgnoreXCOFFVisibility);
        integer(u8"XCOFFTracebackTable", o.XCOFFTracebackTable);
        integer(u8"UniqueSectionNames", o.UniqueSectionNames);
        integer(u8"UniqueBasicBlockSectionNames", o.UniqueBasicBlockSectionNames);
        integer(u8"SeparateNamedSections", o.SeparateNamedSections);
        integer(u8"TrapUnreachable", o.TrapUnreachable);
        integer(u8"NoTrapAfterNoreturn", o.NoTrapAfterNoreturn);
        integer(u8"TLSSize", o.TLSSize);
        integer(u8"EmulatedTLS", o.EmulatedTLS);
        integer(u8"EnableTLSDESC", o.EnableTLSDESC);
        integer(u8"EnableIPRA", o.EnableIPRA);
        integer(u8"EmitStackSizeSection", o.EmitStackSizeSection);
        integer(u8"EnableMachineOutliner", o.EnableMachineOutliner);
        integer(u8"EnableMachineFunctionSplitter", o.EnableMachineFunctionSplitter);
        integer(u8"EnableStaticDataPartitioning", o.EnableStaticDataPartitioning);
        integer(u8"SupportsDefaultOutlining", o.SupportsDefaultOutlining);
        integer(u8"EmitAddrsig", o.EmitAddrsig);
        integer(u8"BBAddrMap", o.BBAddrMap);
        integer(u8"EmitCallGraphSection", o.EmitCallGraphSection);
        integer(u8"EmitCallSiteInfo", o.EmitCallSiteInfo);
        integer(u8"SupportsDebugEntryValues", o.SupportsDebugEntryValues);
        integer(u8"EnableDebugEntryValues", o.EnableDebugEntryValues);
        integer(u8"ValueTrackingVariableLocations", o.ValueTrackingVariableLocations);
        integer(u8"ForceDwarfFrameSection", o.ForceDwarfFrameSection);
        integer(u8"XRayFunctionIndex", o.XRayFunctionIndex);
        integer(u8"DebugStrictDwarf", o.DebugStrictDwarf);
        integer(u8"Hotpatch", o.Hotpatch);
        integer(u8"PPCGenScalarMASSEntries", o.PPCGenScalarMASSEntries);
        integer(u8"JMCInstrument", o.JMCInstrument);
        integer(u8"EnableCFIFixup", o.EnableCFIFixup);
        integer(u8"XCOFFReadOnlyPointers", o.XCOFFReadOnlyPointers);
        integer(u8"VerifyArgABICompliance", o.VerifyArgABICompliance);
        integer(u8"GlobalISelAbort", o.GlobalISelAbort);
        integer(u8"SwiftAsyncFramePointer", o.SwiftAsyncFramePointer);
        integer(u8"BBSections", o.BBSections);
        integer(u8"LoopAlignment", o.LoopAlignment);
        integer(u8"FloatABIType", o.FloatABIType);
        integer(u8"AllowFPOpFusion", o.AllowFPOpFusion);
        integer(u8"ThreadModel", o.ThreadModel);
        integer(u8"EABIVersion", o.EABIVersion);
        integer(u8"DebuggerTuning", o.DebuggerTuning);
        integer(u8"VecLib", o.VecLib);
        integer(u8"ExceptionModel", o.ExceptionModel);
        integer(u8"BBSectionsFuncListBuf.present", static_cast<bool>(o.BBSectionsFuncListBuf));
        if(o.BBSectionsFuncListBuf) { string(u8"BBSectionsFuncListBuf.contents", o.BBSectionsFuncListBuf->getBuffer()); }
        string(u8"ObjectFilenameForDebug", o.ObjectFilenameForDebug);
        auto const& m{o.MCOptions};
        integer(u8"MC.MCRelaxAll", m.MCRelaxAll);
        integer(u8"MC.MCNoExecStack", m.MCNoExecStack);
        integer(u8"MC.MCNoTypeCheck", m.MCNoTypeCheck);
        integer(u8"MC.MCSaveTempLabels", m.MCSaveTempLabels);
        integer(u8"MC.MCIncrementalLinkerCompatible", m.MCIncrementalLinkerCompatible);
        integer(u8"MC.FDPIC", m.FDPIC);
        integer(u8"MC.Dwarf64", m.Dwarf64);
        integer(u8"MC.EmitCompactUnwindNonCanonical", m.EmitCompactUnwindNonCanonical);
        integer(u8"MC.EmitSFrameUnwind", m.EmitSFrameUnwind);
        integer(u8"MC.Crel", m.Crel);
        integer(u8"MC.ImplicitMapSyms", m.ImplicitMapSyms);
        integer(u8"MC.X86RelaxRelocations", m.X86RelaxRelocations);
        integer(u8"MC.X86Sse2Avx", m.X86Sse2Avx);
        integer(u8"MC.RelocSectionSym", m.RelocSectionSym);
        integer(u8"MC.EmitDwarfUnwind", m.EmitDwarfUnwind);
        integer(u8"MC.DwarfVersion", m.DwarfVersion);
        integer(u8"MC.MCUseDwarfDirectory", m.MCUseDwarfDirectory);
        integer(u8"MC.CompressDebugSections", m.CompressDebugSections);
        integer(u8"MC.LargeEHEncoding", m.LargeEHEncoding);
        string(u8"MC.ABIName", m.ABIName);
        string(u8"MC.AssemblyLanguage", m.AssemblyLanguage);
        string(u8"MC.SplitDwarfFile", m.SplitDwarfFile);
        string(u8"MC.Argv0", m.Argv0);
        string(u8"MC.CommandlineArgs", m.CommandlineArgs);
        // The production compiler supplies no file-reading inline assembly.
        // Search paths remain an input if an embedder enables that facility.
        for(auto const& directory: m.IASSearchPaths) { string(u8"MC.IASSearchPath", directory); }
        return out;
    }
}
#endif
