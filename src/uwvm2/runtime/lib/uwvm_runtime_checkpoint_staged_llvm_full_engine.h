// Native cold compilation DATA only; included inside uwvm2::runtime::lib
// after the real full-materializer/owned replacement parser helpers.
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && \
    defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
#include "uwvm_runtime_checkpoint_staged_native_endpoints.h"
extern "C++"
{
    class runtime_checkpoint_world_transaction;
    // No public construction, entry getter or publication operation. The real
    // world transaction alone consumes this DATA after its independent graph,
    // resources, aliases, generation, source/cache/product and drain proofs.
    class runtime_checkpoint_staged_llvm_full_engine final
    {
        friend class runtime_checkpoint_world_transaction;
        using translator = ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::compile_option;
        using module_owner = ::uwvm2::uwvm::runtime::initializer::staged_compiler_module_owner;
        using local_storage = ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::local_func_storage_t;
        using profile_owner = ::uwvm2::runtime::checkpoint::compilation_profile::owner;
        struct effective_function_body
        {
            // Strict local order, with the genuine graph's public index and
            // actual committed generation. Generation one uses owned FINAL
            // parser code; later generations contain the complete body encoding
            // (locals + instructions + end), never an erased native pointer.
            ::std::size_t public_function_index{};
            ::std::uint64_t generation{};
            ::uwvm2::utils::container::std_vector<::std::byte> body{};
        };
        enum class call_stack_policy : unsigned { unwind };
        enum class preparation_status : unsigned
        { ok, invalid_input, invalid_body, unsupported_native, lowering_declined, exhausted, native_failure, native_endpoint_failure,
          native_endpoint_observer_failure, native_endpoint_freeze_failure };
        struct native_entries
        { ::std::uintptr_t typed{}, raw{}, resume_typed{}, resume_raw{}; };

        module_owner owner_;
        profile_owner profile_{};
        ::std::size_t module_id_{};
        // Stable typed parser owners precede all borrowed descriptors. Their
        // original section bytes and local declarations survive native teardown.
        ::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_unique_ptr<llvm_jit_debug_replace_transaction>> effective_bodies_{};
        ::uwvm2::utils::container::std_vector<::std::uint64_t> generations_{};
        ::uwvm2::utils::container::std_vector<local_storage> locals_{};
        // The actual runtime record owns this SAME container type. A real
        // world commit moves its allocation; copying into a new buffer would
        // invalidate target addresses already embedded in the private LLVM IR.
        ::uwvm2::utils::container::vector<::std::uintptr_t> typed_targets_{};
        ::uwvm2::utils::container::std_vector<native_entries> entries_{};
        unsigned debug_shutdown_abi_revision_{};
        // Borrowed from THIS engine-owned real memory manager until native teardown.
        ::uwvm2::runtime::compiler::llvm_jit::details::runtime_llvm_jit_section_memory_manager const* debug_cfi_manager_{};
        // Reverse member destruction detaches the pending listener first, then
        // destroys the engine/CFI allocations before context/types/body/source.
        ::uwvm2::runtime::compiler::llvm_jit::fast_io_support::context_owner context_{};
        ::uwvm2::utils::container::delete_owned_ptr<::llvm::ExecutionEngine> engine_{};
        ::uwvm2::utils::container::std_unique_ptr<details::pending_llvm_jit_code_ranges> ranges_{};
        // Comparison receipts minted only by the private world handoff.
        // The FINAL full-code record supplies ownership, never these pointers.
        ::llvm::ExecutionEngine const* handed_off_engine_{};
        ::llvm::LLVMContext const* handed_off_context_{};
        ::std::uintptr_t const* handed_off_targets_{};
#include "uwvm_runtime_checkpoint_staged_native_endpoint_declarations.h"

        explicit runtime_checkpoint_staged_llvm_full_engine(module_owner const& owner,
            profile_owner profile, ::std::size_t module_id)
            : owner_{owner}, profile_{::std::move(profile)}, module_id_{module_id} {}
        runtime_checkpoint_staged_llvm_full_engine(runtime_checkpoint_staged_llvm_full_engine const&) = delete;
        runtime_checkpoint_staged_llvm_full_engine& operator=(runtime_checkpoint_staged_llvm_full_engine const&) = delete;
        runtime_checkpoint_staged_llvm_full_engine(runtime_checkpoint_staged_llvm_full_engine&&) = delete;
        runtime_checkpoint_staged_llvm_full_engine& operator=(runtime_checkpoint_staged_llvm_full_engine&&) = delete;
        struct preparation_result
        {
            ::uwvm2::utils::container::std_unique_ptr<runtime_checkpoint_staged_llvm_full_engine> data{};
            preparation_status status{preparation_status::invalid_input};
        };

        [[nodiscard]] static bool extent_contains(::std::byte const* begin, ::std::size_t size,
            ::std::byte const* body_begin, ::std::byte const* expression_begin, ::std::byte const* body_end) noexcept
        {
            // No pointer arithmetic or foreign pointee read: compare only native
            // identities after the strong owner established one actual image.
            auto const base{reinterpret_cast<::std::uintptr_t>(begin)};
            auto const body{reinterpret_cast<::std::uintptr_t>(body_begin)};
            auto const expression{reinterpret_cast<::std::uintptr_t>(expression_begin)};
            auto const end{reinterpret_cast<::std::uintptr_t>(body_end)};
            return begin != nullptr && body_begin != nullptr && expression_begin != nullptr && body_end != nullptr &&
                body >= base && expression >= body && end > expression && end >= base &&
                body - base < size && expression - base < size && end - base <= size;
        }

        // Source membership precedes file access. Repeat this check at code
        // handoff so a policy/evidence change cannot publish a stale receipt.
        [[nodiscard]] bool security_allows_original_code() const noexcept
        {
            if(!owner_.matches_unpublished_file()) { return false; }
            auto const& file{*owner_.file()};
            auto const selected{file.requires_verified_execution_authorization() ||
                ::uwvm2::utils::security::wsea::module_selected(::std::string_view{
                    reinterpret_cast<char const*>(file.file_name.cbegin()), file.file_name.size()})};
            if(!selected) { return true; }
            if(!file.has_verified_execution_authorization()) { return false; }
            for(auto generation : generations_) { if(generation != 1u) { return false; } }
            for(auto const& body : effective_bodies_) { if(body) { return false; } }
            return true;
        }

        [[nodiscard]] static preparation_result prepare(module_owner const& actual_owner,
            ::std::size_t actual_module_id, profile_owner actual_profile,
            ::uwvm2::utils::container::std_vector<effective_function_body> const& effective,
            call_stack_policy stack_policy,
            ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::llvm_jit_debug_safe_point_granularity granularity,
            void* native_budget_owner,native_budget_charge native_charge, llvm_jit_checkpoint_engine_location& location) noexcept
        {
            using stage = llvm_jit_checkpoint_engine_location::stage;
            namespace compile = ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm;
            namespace emit = ::uwvm2::runtime::compiler::llvm_jit::compile_all_from_uwvm::details;
            namespace checkpoint = ::uwvm2::runtime::checkpoint;
            if(!actual_owner.matches_unpublished_file() || actual_module_id == SIZE_MAX || !actual_profile ||
                actual_profile->purpose() != checkpoint::compilation_purpose::resumable ||
                actual_profile->cache_identity()[1u] != checkpoint::compilation_profile::resumable_policy_revision || checkpoint::native_resume_abi_revision != 2u ||
                stack_policy != call_stack_policy::unwind ||
                (granularity != compile::llvm_jit_debug_safe_point_granularity::entry_loop &&
                 granularity != compile::llvm_jit_debug_safe_point_granularity::instruction))
            { return {}; }
            auto const& verified_file{*actual_owner.file()};
            auto const secure_source{verified_file.requires_verified_execution_authorization() ||
                ::uwvm2::utils::security::wsea::module_selected(::std::string_view{
                    reinterpret_cast<char const*>(verified_file.file_name.cbegin()), verified_file.file_name.size()})};
            if(secure_source && !verified_file.has_verified_execution_authorization()) { return {}; }
            if(secure_source)
            {
                for(auto const& body : effective)
                {
                    // A base module signature does not authenticate a saved
                    // hot-replacement body from an earlier insecure checkpoint.
                    if(body.generation != 1u || !body.body.empty())
                    { return {{}, preparation_status::invalid_body}; }
                }
            }
            if(stack_policy == call_stack_policy::unwind && !runtime_llvm_jit_unwind_can_replace_instruction_frames())
            { return {{}, preparation_status::unsupported_native}; }
            return ::uwvm2::utils::allocator::resource::recoverable_parser_allocations::prepare_candidate([&]() -> preparation_result
            {
            try
            {
                    auto data{::uwvm2::utils::container::std_unique_ptr<runtime_checkpoint_staged_llvm_full_engine>{
                    ::uwvm2::utils::allocator::fast_io_strict::object<runtime_checkpoint_staged_llvm_full_engine>([&](runtime_checkpoint_staged_llvm_full_engine* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) runtime_checkpoint_staged_llvm_full_engine{actual_owner, ::std::move(actual_profile), actual_module_id}; })}};
                data->native_budget_owner_=native_budget_owner;data->native_charge_=native_charge;
                // [native context-minted source/module/FINAL file] end
                // [safe] exact membership was rechecked BEFORE borrowing parser
                // or runtime fields; the record retains every source allocation.
                auto const& runtime_module{*data->owner_.module()};
                auto const& source_file{*data->owner_.file()};
                auto const& parser_module{source_file.wasm_module_storage.wasm_binfmt_ver1_storage};
                auto const& features{source_file.wasm_parameter.binfmt1_para};
                auto const imported{runtime_module.imported_function_vec_storage.size()};
                auto const count{runtime_module.local_defined_function_vec_storage.size()};
                if(count != effective.size() || count > SIZE_MAX - imported)
                { return {}; }
                data->effective_bodies_.resize(count);
                data->generations_.resize(count);
                data->locals_.resize(count);
                // This allocation never moves with the nonmoving owner. Zero
                // private targets remain inaccessible until every symbol resolves.
                data->typed_targets_.resize(count, 0u);
                data->entries_.resize(count);
                auto validation_module{emit::build_runtime_validation_module(runtime_module)};
                auto& code_section{::uwvm2::parser::wasm::concepts::operation::get_first_type_in_tuple<
                    emit::validation_module_traits_t::code_section_storage_t>(validation_module.sections)};
                if(code_section.codes.size() != count) { return {}; }
                ::uwvm2::validation::error::code_validation_error_impl validation_error{};
                emit::require_runtime_module_declaration_policy(runtime_module, features, validation_error);
                auto const limits{effective_code_body_limits(features)};
                ::std::size_t total_body_bytes{};
                location.operation=stage::parse;
                for(::std::size_t index{}; index != count; ++index)
                {
                    location.function=imported+index;
                    // [0..count actual ordered body DATA] end
                    // [safe] index<count selects DATA, module declaration and
                    // private code slot before any raw expression pointer borrow.
                    auto const& body{effective[index]};
                    if(body.public_function_index != imported + index || body.generation == 0u ||
                        (body.generation == 1u ? !body.body.empty() : (body.body.size() < 2u || body.body.size() > 65536u)))
                    { return {}; }
                    auto local{emit::get_runtime_local_func_storage(runtime_module, index, validation_error)};
                    auto const* original{runtime_module.local_defined_function_vec_storage.index_unchecked(index).wasm_code_ptr};
                    if(original != ::std::addressof(code_section.codes.index_unchecked(index)) &&
                        (original == nullptr || original->body.code_begin != code_section.codes.index_unchecked(index).body.code_begin ||
                         original->body.expr_begin != code_section.codes.index_unchecked(index).body.expr_begin ||
                         original->body.code_end != code_section.codes.index_unchecked(index).body.code_end))
                    { return {}; }
                    if(!extent_contains(reinterpret_cast<::std::byte const*>(source_file.source_cbegin()), source_file.source_size(),
                        reinterpret_cast<::std::byte const*>(original->body.code_begin), local.code_begin, local.code_end))
                    { return {}; }
                    auto const size{body.generation == 1u ? encoded_body_size(*original) : body.body.size()};
                    if(!limits.append(total_body_bytes, size)) { return {{}, preparation_status::invalid_body}; }
                    if(body.generation != 1u)
                    {
                        auto parsed{::uwvm2::utils::container::make_unique<llvm_jit_debug_replace_transaction>()};
                        parsed->source = data->owner_.source(); parsed->local_index = index;
                        // The real feature-aware original code-section parser
                        // owns the complete saved body; no semantic/body prepass.
                        parse_llvm_jit_replacement_body(*parsed, parser_module, features, body.body.data(), body.body.size());
                        auto const expression{reinterpret_cast<::std::byte const*>(parsed->code.body.expr_begin)};
                        auto const end{reinterpret_cast<::std::byte const*>(parsed->code.body.code_end)};
                        if(!extent_contains(parsed->section_bytes.data(), parsed->section_bytes.size(),
                            reinterpret_cast<::std::byte const*>(parsed->code.body.code_begin), expression, end))
                        { return {{}, preparation_status::invalid_body}; }
                        code_section.codes.index_unchecked(index) = parsed->code;
                        code_section.locals_require_function_references |= parsed->locals_require_function_references;
                        // [actual parsed owned complete section] end
                        // [safe] both expression/end were checked inside that
                        // owner before replacing the private descriptor pointers.
                        local.wasm_code_ptr = ::std::addressof(parsed->code);
                        local.code_begin = expression; local.code_end = end;
                        data->effective_bodies_[index] = ::std::move(parsed);
                    }
                    local.module_id = actual_module_id;
                    auto const begin{reinterpret_cast<::std::uintptr_t>(local.code_begin)};
                    auto const end{reinterpret_cast<::std::uintptr_t>(local.code_end)};
                    auto const bytes{end - begin}; // same-owned extent proof above
                    local.debug_safe_point_bits.resize(bytes / 8u + (bytes % 8u != 0u));
                    for(auto& octet : local.debug_safe_point_bits) { octet = 0u; }
                    data->generations_[index] = body.generation;
                    data->locals_[index] = ::std::move(local);
                }

                location.function=SIZE_MAX;
                location.operation=stage::module;
                if(!data->security_allows_original_code()) { return {}; }
                if(!ensure_llvm_jit_native_target_initialized())
                { return {{}, preparation_status::unsupported_native}; }
                auto const cpu{get_llvm_jit_host_cpu_name_storage()};
                auto const attributes{get_llvm_jit_host_target_attribute_storage()};
                ::llvm::SmallVector<::llvm::StringRef, 16> attribute_refs{};
                append_llvm_jit_host_target_attribute_refs(attributes, attribute_refs);
                auto const policy{resolve_runtime_llvm_jit_full_materialize_strategy(
                    ::llvm::CodeGenOptLevel::Aggressive, runtime_module.translation, nullptr, data->profile_.get())};
                ::llvm::EngineBuilder target_builder{};
                target_builder.setEngineKind(::llvm::EngineKind::JIT).setOptLevel(policy.codegen_opt_level)
                    .setMCPU(emit::get_llvm_string_ref(cpu)).setMAttrs(attribute_refs);
                auto target{select_runtime_llvm_jit_target(target_builder, cpu, attributes)};
                if(target == nullptr) { return {{}, preparation_status::unsupported_native}; }
                if(policy.codegen_opt_level == ::llvm::CodeGenOptLevel::None) { target->setFastISel(true); }
                translator options{};
                options.curr_wasm_id = actual_module_id;
                options.compiler_registry = ::std::addressof(data->owner_.source()->registry());
                options.compilation_mode = compile::llvm_jit_compilation_mode::full;
                options.execute_memtag = runtime_module.memory_policy.check_tags;
                options.emit_debug_safe_points = true; options.checkpoint_profile = data->profile_;
                options.debug_safe_point_granularity = granularity;
                options.debug_full_patchable_typed_target_base_address = count == 0u ? 0u :
                    reinterpret_cast<::std::uintptr_t>(data->typed_targets_.data());
                options.debug_full_patchable_typed_target_count = count;
                options.validator_feature_parameter = ::std::addressof(features);
                options.verify_llvm_jit_ir = true;
                options.emit_call_stack_frames = false;
                options.emit_unwind_call_stack_frames = true; // ROS crash stacks and native EH are mandatory.
                options.emit_precise_gc_root_frames = true;
                if constexpr(details::native_exception_host::available)
                {
                    if(::uwvm2::runtime::compiler::llvm_jit::native_exception_symbols::supports_itanium_dwarf_object(*target))
                    { options.native_exception_target_machine = target.get(); }
                }
                struct code_partition
                {
                    ::std::size_t first{}, last{};
                    compile::llvm_jit_module_storage_t ir{};
                    ::uwvm2::utils::container::u8string bitcode{};
                    ::uwvm2::runtime::compiler::llvm_jit::native_exception_symbols::declarations imports{};
                    ::uwvm2::runtime::compiler::llvm_jit::native_exception_landingpad::catch_runtime catches{};
                };
                ::uwvm2::utils::container::std_vector<code_partition> parts{};
                parts.emplace_back();
#if defined(__linux__) && defined(__i386__)
                // Both worlds keep their complete guarded linear memories during preparation.
                // LLVM's O0 machine debug passes allocate per function and retain assembler data
                // until the end of each object. Keep large functions whole in their own partition.
                if(policy.codegen_opt_level == ::llvm::CodeGenOptLevel::None &&
                   policy.pipeline == runtime_llvm_jit_full_pipeline_kind::none)
                {
                    auto const target_bytes{(::std::max)(8192uz, total_body_bytes / 512u)};
                    ::std::size_t used{};
                    for(::std::size_t index{}; index != count; ++index)
                    {
                        auto const& local{data->locals_[index]};
                        auto const bytes{static_cast<::std::size_t>(local.code_end - local.code_begin)};
                        if(used != 0u && (used >= target_bytes || bytes > target_bytes - used))
                        {
                            parts.back().last = index;
                            parts.emplace_back().first = index;
                            used = 0u;
                        }
                        used += bytes;
                    }
                }
#endif
                parts.back().last = count;
                auto native_level{policy.codegen_opt_level};
#if defined(UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2) && UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2 == 1
                ::uwvm2::utils::container::std_vector<details::pending_actual_native_endpoints::expected_function> endpoint_declarations{};
#endif
                auto declare_eh = [](auto& module, auto& machine, auto& imports, auto& catches) -> preparation_status
                {
                    auto const native_eh_requested{module.getNamedValue(
                        ::uwvm2::runtime::compiler::llvm_jit::native_exception_symbols::type_info_symbol) != nullptr};
                    if constexpr(details::native_exception_host::available)
                    {
                        if(::uwvm2::runtime::compiler::llvm_jit::native_exception_symbols::supports_itanium_dwarf_object(machine))
                        {
                            imports = ::uwvm2::runtime::compiler::llvm_jit::native_exception_symbols::declare_itanium_dwarf_symbols(module, machine);
                            catches = ::uwvm2::runtime::compiler::llvm_jit::native_exception_landingpad::declare_catch_runtime(module);
                            if(imports.status != ::uwvm2::runtime::compiler::llvm_jit::native_exception_symbols::error::ok || !static_cast<bool>(catches))
                            { return preparation_status::native_failure; }
                        }
                        else if(native_eh_requested) { return preparation_status::unsupported_native; }
                    }
                    else if(native_eh_requested) { return preparation_status::unsupported_native; }
                    return preparation_status::ok;
                };
                auto lower = [&](code_partition& part) -> preparation_status
                {
                    auto& ir{part.ir};
                    if(!emit::try_prepare_runtime_llvm_jit_module_storage(runtime_module, ir,
                        options.emit_unwind_call_stack_frames, options.native_exception_target_machine))
                    { return preparation_status::lowering_declined; }
                    {
                    emit::private_host_object_emission_scope owned_objects{data->owner_, *ir.llvm_module};
                    location.operation=stage::lower;
                    for(auto index{part.first}; index != part.last; ++index)
                    {
                        location.function=imported+index;
                        auto& local{data->locals_[index]};
                        checkpoint::function_plan work{}; work.profile = data->profile_;
                        // [actual strong source-owned immutable registry] end
                        // [safe] cold borrow for this single fused emit only; the
                        // record/source and all aliases remain retained above.
                        local.compiler_registry = options.compiler_registry;
                        // Exactly one instruction walk validates this effective
                        // body while emitting LLVM and its rich continuation plan.
                        emit::validate_runtime_local_func(validation_module, local, validation_error, ::std::addressof(ir),
                            true, false,
                            options.emit_call_stack_frames, options.emit_unwind_call_stack_frames,
                            options.validator_feature_parameter,
                            true, options.compilation_mode, options.debug_safe_point_granularity,
                            options.debug_full_patchable_typed_target_base_address, count,
                            ::std::addressof(local.debug_safe_point_bits), true, nullptr,
                            data->generations_[index], ::std::addressof(work), nullptr, nullptr, options.execute_memtag);
                        // [finished emitter state] no remaining registry borrow
                        // [safe] clear cold metadata before returning owned DATA.
                        local.compiler_registry = nullptr;
                        if(ir.llvm_module == nullptr || ir.llvm_context_holder == nullptr)
                        { return preparation_status::lowering_declined; }
                        local.checkpoint_plan = checkpoint::sealed_function_plan::seal_compiler_metadata(::std::move(work));
                        if(!local.checkpoint_plan) { return preparation_status::lowering_declined; }
                        auto const& plan{local.checkpoint_plan->get()};
                        if(plan.module != actual_module_id || plan.function != imported + index ||
                            plan.function_generation != data->generations_[index] || plan.profile.get() != data->profile_.get() ||
                            plan.profile.owner_before(data->profile_) || data->profile_.owner_before(plan.profile) ||
                            plan.producer_availability != checkpoint::status::ok || plan.resume_abi_revision != 2u || plan.resume_sites.empty())
                        { return preparation_status::lowering_declined; }
                    }
                    location.function=SIZE_MAX;
                    location.operation=stage::verify;
                    if(!owned_objects.valid() || !emit::finalize_runtime_llvm_jit_module_storage(ir, true) ||
                        ir.llvm_module == nullptr || ir.llvm_context_holder == nullptr)
                    { return preparation_status::lowering_declined; }
                    } // Restore compiler TLS before native optimization/materialization.
                    auto& module{ir.llvm_module};
                    set_llvm_module_target_triple_from_machine(*module, *target);
                    module->setDataLayout(target->createDataLayout());
                    apply_runtime_llvm_jit_native_target_function_attrs(*module, cpu, cpu, *target);
                    location.operation=stage::optimize;
                    if(!optimize_runtime_llvm_jit_module(*module, *target, policy.pipeline, policy.codegen_opt_level, true, false))
                    { return preparation_status::native_failure; }
                    if(native_level != ::llvm::CodeGenOptLevel::None && policy.uses_debug_session_budget() &&
                       runtime_llvm_jit_native_ir_size{*module}.exceeds_debug_budget())
                    {
                        native_level = ::llvm::CodeGenOptLevel::None;
                        target = make_runtime_llvm_jit_materialize_target_machine(cpu, attributes, native_level);
                        if(!target) { return preparation_status::unsupported_native; }
                        target->setFastISel(true);
                    }
                    location.operation=stage::qualify;
                    for(auto index{part.first}; index != part.last; ++index)
                    { for(auto& octet : data->locals_[index].debug_safe_point_bits) { octet = 0u; } }
                    if(!visit_optimized_llvm_jit_debug_safe_points(*module,
                        [&](::std::uint64_t module_id, ::std::uint64_t function, ::std::uint64_t offset) noexcept
                        {
                            if(module_id != actual_module_id || function < imported || function - imported < part.first || function - imported >= part.last) { return false; }
                            auto& local{data->locals_[static_cast<::std::size_t>(function - imported)]};
                            auto const& plan{local.checkpoint_plan->get()};
                            auto& bits{local.debug_safe_point_bits};
                            if(offset >= plan.expression_bytes || offset / 8u >= bits.size()) { return false; }
                            // [actual optimized expression bitmap] end
                            // [safe] checked local/byte index before writing retained
                            // DATA; neither Wasm pointer nor a published PC advances.
                            auto& octet{bits.index_unchecked(static_cast<::std::size_t>(offset / 8u))};
                            octet = static_cast<::std::uint_least8_t>(octet | (1u << (offset % 8u)));
                            return true;
                        })) { return preparation_status::native_failure; }
                    data->debug_shutdown_abi_revision_ = optimized_llvm_jit_debug_shutdown_ready(*module) ? 1u : 0u;
                    if(count != 0u && data->debug_shutdown_abi_revision_ != 1u) { return preparation_status::lowering_declined; }
                    auto const eh{declare_eh(*module, *target, part.imports, part.catches)};
                    if(eh != preparation_status::ok) { return eh; }
#if defined(UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2) && UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2 == 1
                    if(!data->prepare_native_endpoint_declarations(*module, *ir.llvm_context_holder,
                        part.first, part.last, endpoint_declarations)) { return preparation_status::native_endpoint_failure; }
#endif
                    if(parts.size() != 1u)
                    {
                        // Retain only this compiler's verified bitcode until all declarations and
                        // sealed plans are known. No archive or persistent object cache supplies IR.
                        if(!serialize_runtime_llvm_jit_module_bitcode(*module, part.bitcode) || part.bitcode.empty())
                        { return preparation_status::native_failure; }
                        part.imports = {}; part.catches = {}; ir.native_exception_imports = {};
                        module.reset(); ir.llvm_context_holder.reset();
                    }
                    return preparation_status::ok;
                };
                for(auto& part : parts)
                {
                    auto const status{lower(part)};
                    if(status != preparation_status::ok) { return {{}, status}; }
                }
                ::uwvm2::runtime::compiler::shared::strict_float_jit::register_symbols();
                if(parts.size() == 1u) { data->context_ = ::std::move(parts.front().ir.llvm_context_holder); }
                else { data->context_ = ::uwvm2::runtime::compiler::llvm_jit::fast_io_support::make_context(); }
                if(!data->context_) { return {{}, preparation_status::exhausted}; }
#if defined(__linux__) && (defined(__arm__) || defined(__i386__))
                ::llvm::Module* last_ir{};
#endif
                for(auto& part : parts)
                {
                    location.operation=stage::emit;
                    auto& module{part.ir.llvm_module};
                    if(parts.size() != 1u)
                    {
                        auto const bytes{::std::move(part.bitcode)};
                        auto restored{::llvm::parseBitcodeFile(::llvm::MemoryBufferRef{emit::get_llvm_string_ref(bytes),
                            "uwvm.private.checkpoint"}, *data->context_)};
                        if(!restored) { ::llvm::consumeError(restored.takeError());return {{}, preparation_status::native_failure}; }
                        module.reset(restored->release());
                        auto& machine{data->engine_ ? *data->engine_->getTargetMachine() : *target};
                        if(module->getDataLayout() != machine.createDataLayout() || ::llvm::verifyModule(*module))
                        { return {{}, preparation_status::native_failure}; }
                        auto const eh{declare_eh(*module, machine, part.imports, part.catches)};
                        if(eh != preparation_status::ok) { return {{}, eh}; }
                    }
                    auto* const actual_ir{module.get()};
                    if(!data->engine_)
                    {
                        auto memory{::uwvm2::utils::container::make_unique<
                            ::uwvm2::runtime::compiler::llvm_jit::details::runtime_llvm_jit_section_memory_manager>()};
                        if(memory == nullptr) { return {{}, preparation_status::exhausted}; }
                        auto* const memory_observer{memory.get()};
                        auto* const engine{::llvm::EngineBuilder(llvm_module_owner_t{module.release()})
                            .setEngineKind(::llvm::EngineKind::JIT).setOptLevel(native_level)
                            .setMCPU(emit::get_llvm_string_ref(cpu)).setMAttrs(attribute_refs)
                            .setMCJITMemoryManager(llvm_jit_memory_manager_owner_t{memory.release()}).create(target.release())};
                        if(engine == nullptr) { return {{}, preparation_status::native_failure}; }
                        data->engine_.reset(engine);
                        data->debug_cfi_manager_=memory_observer;
                        // No object cache, old-code mapping, pending numeric plan,
                        // diagnostic/permission registration or future epoch exists.
                        data->ranges_ = ::uwvm2::utils::container::make_unique<details::pending_llvm_jit_code_ranges>(*data->engine_, true);
                        if(data->ranges_ == nullptr) { return {{}, preparation_status::exhausted}; }
                        data->ranges_->configure_debug_full_capture(true);
#if defined(UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2) && UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2 == 1
                        if(count!=0u)
                        {
                            data->native_endpoints_.reset(::uwvm2::utils::allocator::fast_io_strict::object<details::pending_actual_native_endpoints>([&](details::pending_actual_native_endpoints* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) details::pending_actual_native_endpoints{
                                *data->engine_,data.get(),data->owner_,data->profile_,actual_module_id,
                                ::std::move(endpoint_declarations),native_budget_owner,native_charge}; }));
                            if(!data->native_endpoints_->valid_ || !data->native_endpoints_->attached_)
                            { return {{},preparation_status::native_endpoint_observer_failure}; }
                        }
#endif
                    }
                    else { data->engine_->addModule(llvm_module_owner_t{module.release()}); }
                    if(part.imports.type_info != nullptr && !details::native_exception_host::bind<::uwvm2::runtime::exception::guest_exception>(
                        *data->engine_, part.imports, part.catches,
                        +[]() UWVM_THROWS { return ::uwvm2::runtime::exception::guest_exception{
                            ::uwvm2::runtime::exception::value::make(::uwvm2::utils::container::make_shared<unsigned char const>(0), {})}; }
#if defined(UWVM_RUNTIME_NATIVE_EXCEPTION_HOST_GNU_CXX) && defined(UWVM_EXPERIMENTAL_EXCEPTION_SOURCE_RETENTION) && \
        UWVM_EXPERIMENTAL_EXCEPTION_SOURCE_RETENTION == 1
                        , &runtime_source_exception_end_catch
#endif
                        )) { return {{}, preparation_status::native_failure}; }
                    // All objects and relocations belong to this private MCJIT. An early
                    // partition can have unresolved calls into a later partition. Finalize
                    // them together after the last load, before freezing any entry table.
                    bool const last{part.last == count};
                    if(last) { data->engine_->finalizeObject(); }
                    else { data->engine_->generateCodeForModule(actual_ir); }
                    if(data->debug_cfi_manager_->has_finalization_failure() || data->engine_->hasError() ||
                       data->ranges_->has_observation_failure()) { return {{}, preparation_status::native_failure}; }
                    auto resolve = [&](::uwvm2::utils::container::u8string const& name) -> ::std::uintptr_t
                    {
                        // Real engine-owned LLVM definition BEFORE native lookup.
                        // Never accept a same-name process-global/old-world symbol
                        // for a missing declaration in this private generation.
                        auto* const function{data->engine_->FindFunctionNamed(emit::get_llvm_string_ref(name))};
                        if(function == nullptr || function->getParent() != actual_ir || function->isDeclaration()) { return 0u; }
                        auto const callable{reinterpret_cast<::std::uintptr_t>(data->engine_->getPointerToFunction(function))};
                        if(callable == 0u) { return 0u; }
                        // This callable was obtained synchronously from this
                        // engine's actual defined LLVM Function, never wire DATA,
                        // an arbitrary integer or a process-global missing symbol.
                        // Only this real loader-owned callable may name an ELFv1
                        // entry/TOC/environment descriptor or carry a Thumb tag.
                        auto const code{details::native_function_code_address(callable)};
                        // [same private loaded object function entry/text extent]
                        // [safe] the actual code address must exactly match this
                        // engine listener's observed function and loaded text,
                        // before retaining any callable address as private DATA.
                        if(!data->ranges_->owns_pending_loaded_function_entry(code)) { return 0u; }
                        return callable;
                    };
                    location.operation=stage::resolve;
                    for(auto index{part.first}; index != part.last; ++index)
                    {
                        auto const public_index{imported + index};
                        location.function=public_index;
                        auto const typed_name{get_runtime_llvm_jit_wasm_function_name(runtime_module, public_index)};
                        auto& entries{data->entries_[index]};
                        entries.typed = resolve(typed_name);
                        entries.raw = resolve(get_runtime_llvm_jit_wasm_raw_function_name(runtime_module, public_index));
                        auto const resume_name{::uwvm2::utils::container::u8concat_uwvm(typed_name, u8".checkpoint.resume.v2")};
                        entries.resume_typed = resolve(resume_name);
                        entries.resume_raw = resolve(::uwvm2::utils::container::u8concat_uwvm(resume_name, u8".raw"));
                        if(entries.typed == 0u || entries.raw == 0u || entries.resume_typed == 0u || entries.resume_raw == 0u)
                        { return {{}, preparation_status::native_failure}; }
                    }
                    part.imports = {}; part.catches = {}; part.ir.native_exception_imports = {};
#if defined(__linux__) && (defined(__arm__) || defined(__i386__))
                    if(last) { last_ir = actual_ir; }
                    else
                    {
                        if(!data->engine_->removeModule(actual_ir)) { return {{}, preparation_status::native_failure}; }
                        ::uwvm2::runtime::compiler::llvm_jit::fast_io_support::module_deleter{}(actual_ir);
                    }
#endif
                }
                location.function=SIZE_MAX;
                location.operation=stage::freeze;
                if(data->ranges_->has_observation_failure()) { return {{}, preparation_status::native_failure}; }
#if defined(UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2) && UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2 == 1
                if(!data->freeze_native_endpoint_capture())
                { return {{},preparation_status::native_endpoint_freeze_failure}; }
#endif
                if(!data->owner_.matches_unpublished_file()) { return {}; }
                for(::std::size_t index{}; index != count; ++index)
                { data->typed_targets_[index] = data->entries_[index].typed; }
#if defined(__linux__) && (defined(__arm__) || defined(__i386__))
                if(data->engine_->removeModule(last_ir))
                { ::uwvm2::runtime::compiler::llvm_jit::fast_io_support::module_deleter{}(last_ir); }
#endif
                // DATA stays exclusively in the returned private nonmoving
                // owner. No listener commit/provenance epoch/source validation
                // seal/global slot/publication/readiness call is made here.
                return {::std::move(data), preparation_status::ok};
            }
            catch(::fast_io::error const&) { return {{}, preparation_status::invalid_body}; }
            catch(::std::bad_alloc const&) { return {{}, preparation_status::exhausted}; }
            catch(...) { return {{}, preparation_status::native_failure}; }
            }, [&]() -> preparation_result { return {{}, preparation_status::exhausted}; });
        }
    public:
        ~runtime_checkpoint_staged_llvm_full_engine() = default;
    };
}
#endif
