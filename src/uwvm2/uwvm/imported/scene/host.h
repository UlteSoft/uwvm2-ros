/** @file
 * @brief Checked Wasm entry points for the independent robot-scene ABI.
 * @details Functions validate trusted runtime identity, explicit exposure and
 * exact resource authority. Guest memory is accessed only through checked
 * runtime APIs. Result slots are disjoint and bounded. Event consumption and
 * handle publication roll back failed delivery; output mutations retain their
 * recovery receipt after effects even if delivery fails. No legacy namespace
 * is aliased.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#if defined(UWVM_IMPORT_ROSV2)
#ifndef UWVM_MODULE
#include "storage.h"
#include "input.h"
#include "permission_observer.h"
#include "module_observer.h"
#include "vm_observer.h"
#include <uwvm2/imported/ros/scene/erased_buffer.h>
#include <uwvm2/imported/ros/scene/lifecycle.h>
#include <uwvm2/imported/ros/common/synchronous_wait.h>
#include <uwvm2/imported/ros/common/audit_io.h>
#if defined(UWVM_VERSION_DEV)
#include <uwvm2/utils/debug/host_calls.h>
#endif
#include <uwvm2/runtime/lib/uwvm_runtime.h>
#include <uwvm2/runtime/lib/uwvm_runtime_wasm_fp_native_control.h>
#include <uwvm2/uwvm/wasm/type/impl.h>
#include <uwvm2/uwvm/runtime/storage/builtin_ros_loader.h>
#include <uwvm2/utils/control/debug_permissions.h>
#include <functional>
#include <type_traits>
#endif

namespace uwvm2::uwvm::imported::scene
{
    namespace model=::uwvm2::imported::ros::scene;
    namespace runtime=::uwvm2::runtime::lib;
    namespace contract=::uwvm2::imported::ros::runtime_contract;

    inline std::uint32_t identify(std::string_view function,model::caller& caller,std::shared_ptr<model::broker>& state)
    {
        runtime::ros_caller_identity identity{};
        if(!runtime::ros_caller_identity_host_api(identity,false))return 13;
        std::string_view module{reinterpret_cast<char const*>(identity.module_name.data()),identity.module_name.size()};
        caller={identity.runtime_instance,identity.generation};
        return static_cast<std::uint32_t>(configuration.select_exposed(module,function,caller,state));
    }
    inline bool range(std::uint32_t memory,std::uint64_t offset,std::uint64_t size) noexcept
    {return size<=SIZE_MAX&&runtime::preload_memory_range_host_api(memory,offset,static_cast<std::size_t>(size));}
    /** @brief Validate a tagged guest span and obtain its physical memory offset.
     * @details Copy still uses the original pointer to retain tag checks.
     * Span-disjointness compares normalized addresses so different pointer
     * encodings cannot alias a result-length slot into a request or output.
     */
    inline bool normalized_range(std::uint32_t memory,std::uint64_t offset,std::uint64_t size,std::uint64_t& normalized) noexcept
    {return size<=SIZE_MAX&&runtime::preload_memory_normalize_range_host_api(memory,offset,static_cast<std::size_t>(size),normalized);}
    inline bool overlap(std::uint64_t a,std::uint64_t an,std::uint64_t b,std::uint64_t bn) noexcept
    {return an&&bn&&(a<=b?b-a<an:a-b<bn);}
    inline bool length(std::uint32_t memory,std::uint64_t offset,std::uint64_t value) noexcept
    {
        std::array<std::uint8_t,8> bytes{};model::put(bytes,0,value);
        return runtime::preload_memory_write_host_api(memory,offset,bytes.data(),bytes.size());
    }
    /** @brief Observe only the immediate caller's policy; zero execution authority.
     * @details The reserved capability must be zero. All spans are checked and
     * disjoint before copying. This path deliberately does not call identify(),
     * select_exposed(), freeze(), enroll(), open() or any provider. Observation
     * uses trusted caller provenance and independently attenuable report policy.
     * An unbuilt family returns unsupported with a supported=false header;
     * a built disabled family succeeds with enabled=false. Denial writes no
     * report or length. Owned request/result buffers are overwritten on exit.
     */
    inline std::uint32_t invoke_permission_report(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace report=model::permission_report;
            runtime::ros_caller_identity identity{};
            if(!runtime::ros_caller_identity_host_api(identity,true))return 13;
            std::string_view module{reinterpret_cast<char const*>(identity.module_name.data()),identity.module_name.size()};
            if(configuration.visible(module,report::import_name)!=model::exposure::allow)return 13;
            std::uint64_t a{},b{},c{};
            if(cap||size>report::maximum_request||capacity>report::maximum_response||
               !normalized_range(memory,request,size,a)||!normalized_range(memory,output,capacity,b)||!normalized_range(memory,length_output,8,c)||
               overlap(a,size,b,capacity)||overlap(a,size,c,8)||overlap(b,capacity,c,8))return 22;
            model::erased_buffer input;input.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,input.writable().data(),static_cast<std::size_t>(size)))return 22;
            report::query query;if(!report::decode(input.view(),query))return 22;
            if(auto status=configuration.report_allowed(module,query);status!=model::error::success)return static_cast<unsigned>(status);
            report::owned_page response{query.first};auto& page=response.get();
            if(query.target==report::family::rosv2&&query.function.empty())
                if(auto described=module_observer::identity(module,page);described!=model::error::success)return static_cast<unsigned>(described);
            auto status=query.target==report::family::rosv2?configuration.report_self(module,query,page):permission_observer::read(module,query,page);
            if(status!=model::error::success&&status!=model::error::unsupported)return static_cast<unsigned>(status);
            // Unknown exact functions are unsupported without a fabricated page.
            if(status==model::error::unsupported&&!query.function.empty())return 95;
            bool insufficient=page.bytes().size()>capacity;
            auto delivered=configuration.deliver_report(module,query,[&]()noexcept
            {
                if(!length(memory,length_output,page.bytes().size()))return false;
                return insufficient||runtime::preload_memory_write_host_api(memory,output,page.bytes().data(),page.bytes().size());
            });
            if(delivered!=model::error::success)return static_cast<unsigned>(delivered);
            return insufficient?90:static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}catch(...){return 5;}
    }
    /** @brief Observe one explicitly granted other module without modifying it.
     * @details The request contains a grant alias and pagination cursor only;
     * the target module is fixed in trusted deployment. All peer/VM operations
     * start hidden and require independent exact actions. No handle is opened,
     * execution policy is frozen, caller enrolled or quota debited by a query.
     */
    template<model::permission_report::module_operation Operation,bool VirtualMachine=false>
    inline std::uint32_t invoke_module_observation(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace report=model::permission_report;
            constexpr auto action=VirtualMachine?report::vm_import_name:report::module_operations[static_cast<std::size_t>(Operation)];
            runtime::ros_caller_identity identity{};if(!runtime::ros_caller_identity_host_api(identity,true))return 13;
            std::string_view module{reinterpret_cast<char const*>(identity.module_name.data()),identity.module_name.size()};
            if(configuration.visible(module,action)!=model::exposure::allow)return 13;
            std::uint64_t a{},b{},c{};
            if(cap||size>271||capacity>report::maximum_response||!normalized_range(memory,request,size,a)||
               !normalized_range(memory,output,capacity,b)||!normalized_range(memory,length_output,8,c)||
               overlap(a,size,b,capacity)||overlap(a,size,c,8)||overlap(b,capacity,c,8))return 22;
            model::erased_buffer input;input.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,input.writable().data(),static_cast<std::size_t>(size)))return 22;
            model::wire::reader reader{input.view()};std::string_view alias;std::uint64_t first{};
            if(!reader.text(alias)||!model::identifier(alias)||!reader.u64(first)||first>report::maximum_scan||!reader.done())return 22;
            // Refuse absent authority before reserving a response buffer.
            auto authorize=[&](auto&& observe)
            {
                if constexpr(VirtualMachine)return configuration.observe_vm(module,alias,observe);
                else return configuration.observe_module(module,alias,action,observe);
            };
            auto checked=authorize([](auto){return model::error::success;});
            if(checked!=model::error::success)return static_cast<unsigned>(checked);
            report::owned_page response{first};auto& page=response.get();
            auto status=authorize([&](auto target)
            {
                model::error result;
                if constexpr(VirtualMachine)result=vm_observer::read(page);
                else result=Operation==report::module_operation::group?configuration.report_module_group_locked(target,page):
                    module_observer::read(target,Operation,page);
                if(result!=model::error::success)return result;
                if(!length(memory,length_output,page.bytes().size()))return model::error::invalid;
                if(page.bytes().size()>capacity)return model::error::too_large;
                return runtime::preload_memory_write_host_api(memory,output,page.bytes().data(),page.bytes().size())?model::error::success:model::error::invalid;
            });
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}catch(...){return 5;}
    }
    /** @brief Read another module's permissions through one exact observation grant.
     * @details The request is alias/text followed by the self-report query.
     * Its target is fixed by the grant. No path, policy setter or capability
     * issuer is reachable. Authority is rechecked while delivering the report.
     */
    inline std::uint32_t invoke_module_permission_report(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace report=model::permission_report;constexpr std::string_view action="module.permission.report";
            runtime::ros_caller_identity identity{};if(!runtime::ros_caller_identity_host_api(identity,true))return 13;
            std::string_view module{reinterpret_cast<char const*>(identity.module_name.data()),identity.module_name.size()};
            if(configuration.visible(module,action)!=model::exposure::allow)return 13;
            std::uint64_t a{},b{},c{};
            if(cap||size>263+report::maximum_request||capacity>report::maximum_response||!normalized_range(memory,request,size,a)||
               !normalized_range(memory,output,capacity,b)||!normalized_range(memory,length_output,8,c)||
               overlap(a,size,b,capacity)||overlap(a,size,c,8)||overlap(b,capacity,c,8))return 22;
            model::erased_buffer input;input.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,input.writable().data(),static_cast<std::size_t>(size)))return 22;
            model::wire::reader reader{input.view()};std::string_view alias;
            if(!reader.text(alias)||!model::identifier(alias))return 22;
            report::query query;if(!report::decode(input.view().subspan(8+alias.size()),query))return 22;
            ::uwvm2::utils::container::std_string target;
            auto checked=configuration.observe_module(module,alias,action,[&](auto name){target=name;return model::error::success;},&query);
            if(checked!=model::error::success)return static_cast<unsigned>(checked);
            report::owned_page response{query.first};auto& page=response.get();
            // Every family describes the exact registered target, including
            // a native dl's separate identity. Absent names disclose no policy.
            if(auto described=module_observer::identity(target,page);described!=model::error::success)return static_cast<unsigned>(described);
            auto status=query.target==report::family::rosv2?configuration.report_self(target,query,page):permission_observer::read(target,query,page,true);
            if(status!=model::error::success&&status!=model::error::unsupported)return static_cast<unsigned>(status);
            if(status==model::error::unsupported&&!query.function.empty())return 95;
            auto delivered=configuration.observe_module(module,alias,action,[&](auto name)
            {
                if(name!=target)return model::error::denied;
                if(!length(memory,length_output,page.bytes().size()))return model::error::invalid;
                if(page.bytes().size()>capacity)return model::error::too_large;
                return runtime::preload_memory_write_host_api(memory,output,page.bytes().data(),page.bytes().size())?model::error::success:model::error::invalid;
            },&query);
            return static_cast<unsigned>(delivered==model::error::success?status:delivered);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}catch(...){return 5;}
    }
    /** @brief Route a registered message, retaining only sealed erasure descriptors.
     * @details Both modules need explicit exposure and operation grants. The
     * route additionally fixes their exact names. Exchange storage must be an
     * owned shared memory with no memory exports; private nonshared heaps and
     * native/imported memories cannot become remote erase targets. Input is
     * copied with byte atomics and its entire carrier is zeroed before dispatch
     * by default. A trusted route may opt out of input erasure; retained sender
     * bytes then remain that sender's responsibility. This grants no authority
     * and never disables erasure of host storage or delivered buffers.
     * Delivered buffers remain readable until respond or acknowledge. Revoke,
     * cancel, observed expiry and retirement erase them through the runtime.
     * Payload reads/writes bypass persistent replay capture, never tag checks.
     */
    template<model::module_exchange::operation Operation>
    inline std::uint32_t invoke_module_call_bound(runtime::ros_exchange_memory_access& access,std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace exchange=model::module_exchange;
            auto const& identity=access.identity();
            std::string_view module{reinterpret_cast<char const*>(identity.module_name.data()),identity.module_name.size()};
            model::caller who{identity.runtime_instance,identity.generation};std::shared_ptr<model::broker> state;
            bool cleanup=Operation==exchange::operation::cancel||Operation==exchange::operation::acknowledge;
            model::lease admitted;
            bool clear_after_copy=true;
            auto status=configuration.preflight_module_call(module,who,cap,Operation,state,admitted,&clear_after_copy);
            if(status!=model::error::success)return static_cast<unsigned>(status);
            std::uint64_t a{},b{},c{};
            if(size<16||size>exchange::maximum_bytes+16||capacity>exchange::maximum_bytes+16||(!cleanup&&capacity<16))return 22;
            if(!access.capture(memory,request,static_cast<std::size_t>(size),a)||
               !access.capture(memory,output,static_cast<std::size_t>(capacity),b)||
               !access.capture(memory,length_output,8,c))return 95;
            if(overlap(a,size,b,capacity)||overlap(a,size,c,8)||overlap(b,capacity,c,8))return 22;
            auto eraser=[](model::caller owner,std::uint64_t index,std::uint64_t offset,std::uint64_t count,std::uint64_t length_offset,std::uint64_t length_size)noexcept
            {return runtime::ros_exchange_memory_erase_host_api(owner.instance,owner.generation,index,offset,count,length_offset,length_size);};
            exchange::region source{who,memory,a,size,eraser};
            exchange::region destination{who,memory,b,capacity,eraser,c,8};
            if(!state->module_calls.available(source,destination))return 17;
            // This input belongs to the currently executing owner. Reuse its
            // scoped proof; delayed/remote deliveries keep the sealed eraser.
            struct input_cleanup
            {
                runtime::ros_exchange_memory_access& access;exchange::region& value;bool enabled;
                bool clear()noexcept
                {if(!value.erase||!enabled)return true;if(!access.erase(value.offset,value.size))return false;value={};return true;}
                ~input_cleanup(){(void)clear();}
            } wipe{access,source,clear_after_copy};
            model::erased_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!access.read_input(memory,request,owned.writable().data(),static_cast<std::size_t>(size),clear_after_copy))
            {return wipe.clear()?22:5;}
            // With default policy the checked copy also erased every input byte.
            // Explicit nonclearing policy retains sender bytes, never host data.
            // Fallback cleanup remains active for refusal/allocation failure
            // exactly when that route requires erasure.
            source={};
            model::wire::reader reader{owned.view()};std::uint64_t token{};std::span<std::uint8_t const> payload;
            if(!reader.u64(token)||!reader.blob(payload)||!reader.done())return 22;
            admitted.release();
            status=state->permissions.admit(who,cap,Operation,cleanup?0:size+capacity,admitted,!cleanup);
            if(status!=model::error::success)return static_cast<unsigned>(status);
            auto deliver=[&](std::uint64_t id,std::span<std::uint8_t const> data)noexcept
            {
                std::array<std::uint8_t,16> header{};std::array<std::uint8_t,8> count{};
                auto total=cleanup?0:16+data.size();if(total>capacity)return false;
                model::put(header,0,id);model::put(header,1,data.size());model::put(count,0,total);
                bool accepted=access.copy(memory,length_output,count.data(),count.size(),true)&&
                    (cleanup||access.write_frame(memory,output,header.data(),header.size(),data.data(),data.size()));
                model::erase_bytes(header);model::erase_bytes(count);
                if(!accepted)(void)destination.clear();return accepted;
            };
            return static_cast<unsigned>(state->module_calls.invoke(who,module,admitted.specification(),Operation,token,payload,destination,
                capacity>=16?capacity-16:0,::uwvm2::imported::ros::resources::steady_now(),deliver));
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}catch(...){return 5;}
    }
    /** @brief Keep authenticated memory access inside one synchronous builtin call.
     * @details The callback never invokes Wasm or a provider. It retains all
     * import, grant, role, quota, overlap, tag and erasure checks; only repeated
     * identity/declaration lookups within this call share the runtime proof.
     */
    template<model::module_exchange::operation Operation>
    inline std::uint32_t invoke_module_call(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        struct arguments {std::uint64_t cap,request,size,output,capacity,length;std::uint32_t memory;};
        arguments args{cap,request,size,output,capacity,length_output,memory};
        return runtime::with_ros_exchange_memory_host_api([](void* opaque,runtime::ros_exchange_memory_access& access)noexcept
        {
            auto const& a=*static_cast<arguments const*>(opaque);
            return invoke_module_call_bound<Operation>(access,a.cap,a.memory,a.request,a.size,a.output,a.capacity,a.length);
        },&args);
    }
    /** @brief Read the scene ABI major for the actual caller without preparing a broker. */
    inline std::uint32_t abi() noexcept
    {
        runtime::ros_caller_identity identity{};
        if(!runtime::ros_caller_identity_host_api(identity,true))return 13;
        std::string_view module{reinterpret_cast<char const*>(identity.module_name.data()),identity.module_name.size()};
        return configuration.visible(module,"abi")==model::exposure::allow?2u:13u;
    }
    /** @brief Open an exact granted alias and atomically deliver its handle.
     * @details An invalid output, wrong caller or hidden/denied interface has
     * no authority effect. Failed delivery abandons the unpublished handle even
     * when the guest has no capability.close interface.
     */
    inline std::uint32_t open(std::uint32_t memory,std::uint64_t offset,std::uint64_t size,std::uint64_t output) noexcept
    {
        try
        {
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify("capability.open",caller,state))return code;
            if(!size||size>255||!range(memory,offset,size)||!range(memory,output,8))return 22;
            ::uwvm2::utils::container::std_string alias(static_cast<std::size_t>(size),'\0');
            if(!runtime::preload_memory_read_host_api(memory,offset,alias.data(),alias.size())||!model::identifier(alias))return 22;
            model::handle id{};auto result=state->permissions.open(caller,alias,id);if(result!=model::error::success)return static_cast<unsigned>(result);
            if(length(memory,output,id))return 0;
            state->permissions.abandon_open(id);return 22;
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Close an environment capability handle without resetting its budgets. */
    inline std::uint32_t close(std::uint64_t id) noexcept
    {
        try{model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify("capability.close",caller,state))return code;return static_cast<unsigned>(state->close_capability(caller,id));}
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Checked self-revocation carrier with no allocation or guest pointer retention.
     * @details Both writable spans and their normalized overlap are validated
     * before fencing. The fixed request is owned on the stack. A failed checked
     * copy after fencing is recoverable only with the same key and capability.
     * Full native FP restoration remains enabled because provider retirement
     * notifications are not part of the audited FP-preserving subset.
     */
    inline std::uint32_t invoke_revocation(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify("capability.revoke",caller,state))return code;
            if(auto status=state->permissions.preflight_revoke(caller,cap);status!=model::error::success)return static_cast<unsigned>(status);
            std::uint64_t request_address{},output_address{},length_address{};
            if(size!=16||capacity>model::transfer_limit||!normalized_range(memory,request,size,request_address)||
                !normalized_range(memory,output,capacity,output_address)||!normalized_range(memory,length_output,8,length_address)||
                overlap(output_address,capacity,length_address,8)||overlap(request_address,size,length_address,8))return 22;
            std::array<std::uint8_t,16> input;
            if(!runtime::preload_memory_read_host_api(memory,request,input.data(),input.size()))return 22;
            auto deliver=[&](model::bytes bytes) noexcept
            {return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size());};
            auto cancelled=[]() noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto status=state->invoke_revocation(caller,cap,input,static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}catch(...){return 5;}
    }
    /** @brief Common checked record ABI for scene scheduling operations.
     * @details Request is copied before effects. Invalid/overlapping output
     * length slots cannot consume an event. A wait retains no guest pointer and
     * revalidates after wake, cancellation and trusted runtime retirement.
     */
    template<contract::operation Action>
    inline std::uint32_t invoke(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size!=description.words*8u||capacity>model::transfer_limit||!normalized_range(memory,request,size,request_address)||
                !normalized_range(memory,output,capacity,output_address)||!normalized_range(memory,length_output,8,length_address)||
                overlap(output_address,capacity,length_address,8)||overlap(request_address,size,length_address,8))return 22;
            std::array<std::uint8_t,40> input{};
            if(size&&!runtime::preload_memory_read_host_api(memory,request,input.data(),static_cast<std::size_t>(size)))return 22;
            std::size_t required{};
            auto deliver=[&](model::bytes bytes) noexcept
            {
                if(bytes.size()>capacity||!length(memory,length_output,bytes.size()))return false;
                return bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size());
            };
            auto cancelled=[]() noexcept
            {
                using context=::uwvm2::imported::ros::synchronous_wait_context;
                return context::stop_requested()||context::process_cancelled();
            };
            // invoke is synchronous, including wait.poll. Its delivery callback
            // never escapes this stack frame. A reference_wrapper avoids an
            // owning std::function allocation for the four captured span fields;
            // every checked copy and transactional commit still runs unchanged.
            auto result=state->invoke(caller,Action,cap,{input.data(),static_cast<std::size_t>(size)},
                static_cast<std::size_t>(capacity),required,std::ref(deliver),cancelled);
            if(result==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(result);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Checked immutable-input entry; no guest path or lazy provisioning.
     * @details The common carrier validates memory index, normalized spans and
     * result-length disjointness before admission. Request bytes are copied;
     * failed handle delivery rolls back its private resource. Reads are bounded
     * and nonconsuming. The three closed artifact wrappers use the audited
     * native register-control guard below where available; imported-memory
     * callbacks independently retain their complete FP boundary.
     */
    template<::uwvm2::imported::ros::artifact_contract::operation Action>
    inline std::uint32_t invoke_artifact(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            constexpr auto description=::uwvm2::imported::ros::artifact_contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size!=description.words*8u||capacity>model::transfer_limit||!normalized_range(memory,request,size,request_address)||
                !normalized_range(memory,output,capacity,output_address)||!normalized_range(memory,length_output,8,length_address)||
                overlap(output_address,capacity,length_address,8)||overlap(request_address,size,length_address,8))return 22;
            std::array<std::uint8_t,40> input{};
            if(!runtime::preload_memory_read_host_api(memory,request,input.data(),static_cast<std::size_t>(size)))return 22;
            std::size_t required{};
            auto deliver=[&](model::bytes bytes) noexcept
            {
                if(bytes.size()>capacity||!length(memory,length_output,bytes.size()))return false;
                return bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size());
            };
            auto cancelled=[]() noexcept
            {
                using context=::uwvm2::imported::ros::synchronous_wait_context;
                return context::stop_requested()||context::process_cancelled();
            };
            auto result=state->invoke_artifact(caller,Action,cap,{input.data(),static_cast<std::size_t>(size)},
                static_cast<std::size_t>(capacity),required,std::ref(deliver),cancelled);
            if(result==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(result);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Checked volatile output call with owned request bytes and exact spans.
     * @details Exact authority, transfer and receipt capacity are checked before
     * payload allocation. Append copies its complete bounded payload before
     * charged admission, so
     * request/output aliases and guest mutation after copying cannot change an
     * accepted request. Physical aliases of the result length are rejected.
     * Output failure retains the provider receipt for exact-key replay. Each
     * output uses its separately audited FP policy declared below.
     */
    template<::uwvm2::imported::ros::output_contract::operation Action>
    inline std::uint32_t invoke_output(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            constexpr auto description=::uwvm2::imported::ros::output_contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>model::transfer_limit+24u||capacity>model::transfer_limit||
               (description.words<0?size<static_cast<unsigned>(-description.words)*8u:size!=static_cast<unsigned>(description.words)*8u)||
               !normalized_range(memory,request,size,request_address)||!normalized_range(memory,output,capacity,output_address)||
               !normalized_range(memory,length_output,8,length_address)||overlap(output_address,capacity,length_address,8)||
               overlap(request_address,size,length_address,8))return 22;
            if(auto checked=state->preflight_output(caller,Action,cap,static_cast<std::size_t>(size),
                capacity>=::uwvm2::imported::ros::output_contract::receipt_bytes);checked!=model::error::success)
            {
                if(checked==model::error::too_large&&!length(memory,length_output,0))return 22;
                return static_cast<unsigned>(checked);
            }
            if(capacity<::uwvm2::imported::ros::output_contract::receipt_bytes)
                return length(memory,length_output,::uwvm2::imported::ros::output_contract::receipt_bytes)?90:22;
            model::outputs::registry::publication_scratch publication;
            model::outputs::registry::publication_scratch const* reserved{};
            if constexpr(Action==::uwvm2::imported::ros::output_contract::operation::publish)
            {
                auto checked=state->reserve_output_publication(caller,cap,static_cast<std::size_t>(size),publication);
                if(checked!=model::error::success)return static_cast<unsigned>(checked);
                reserved=&publication;
            }
            model::byte_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,owned.writable().data(),owned.writable().size()))return 22;
            auto input=owned.view();
            auto deliver=[&](model::bytes data) noexcept
            {return data.size()<=capacity&&length(memory,length_output,data.size())&&runtime::preload_memory_write_host_api(memory,output,data.data(),data.size());};
            auto cancelled=[]() noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto result=state->invoke_output(caller,Action,cap,input,static_cast<std::size_t>(capacity),required,std::ref(deliver),cancelled,reserved);
            if(result==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(result);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Checked lifecycle carrier with fixed owned records and exact authority.
     * @details No native provider or guest callback executes here. Memory spans
     * and aliases validate before admission, request copying precedes effects,
     * and deferred callbacks use checked delivery with provider rollback. The
     * full floating-point boundary remains enabled for these new operations.
     */
    template<::uwvm2::imported::ros::managed_lifecycle_contract::operation Action>
    inline std::uint32_t invoke_managed(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            constexpr auto description=::uwvm2::imported::ros::managed_lifecycle_contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size!=description.words*8u||capacity>model::transfer_limit||!normalized_range(memory,request,size,request_address)||
                !normalized_range(memory,output,capacity,output_address)||!normalized_range(memory,length_output,8,length_address)||
                overlap(output_address,capacity,length_address,8)||overlap(request_address,size,length_address,8))return 22;
            std::array<std::uint8_t,40> input{};
            if(!runtime::preload_memory_read_host_api(memory,request,input.data(),static_cast<std::size_t>(size)))return 22;
            auto deliver=[&](model::bytes bytes) noexcept
            {return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size());};
            auto cancelled=[]() noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto result=state->invoke_managed(caller,Action,cap,{input.data(),static_cast<std::size_t>(size)},
                static_cast<std::size_t>(capacity),required,std::ref(deliver),cancelled);
            if(result==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(result);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Canonical sealed-buffer carrier with owned bounded input bytes.
     * @details Exact policy and transfer preflight precedes allocation. Result
     * and length spans are physically disjoint even with tagged aliases. Read
     * prefixes and payloads use separate checked writes under one synchronous
     * provider delivery, avoiding a second large host copy. The conservative
     * full FP boundary remains enabled for all buffer operations.
     */
    template<::uwvm2::imported::ros::buffer_contract::operation Action>
    inline std::uint32_t invoke_buffer(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,
        std::uint64_t size,std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            constexpr auto description=::uwvm2::imported::ros::buffer_contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>model::transfer_limit+24u||capacity>model::transfer_limit+::uwvm2::imported::ros::buffer_contract::maximum_layout_wire||
               !normalized_range(memory,request,size,request_address)||!normalized_range(memory,output,capacity,output_address)||
               !normalized_range(memory,length_output,8,length_address)||overlap(output_address,capacity,length_address,8)||
               overlap(request_address,size,length_address,8))return 22;
            auto checked=state->preflight_buffer(caller,Action,cap,static_cast<std::size_t>(size),
                capacity>=(Action==::uwvm2::imported::ros::buffer_contract::operation::seal?16u:8u));
            if(checked!=model::error::success)return static_cast<unsigned>(checked);
            if constexpr(Action==::uwvm2::imported::ros::buffer_contract::operation::create||
                         Action==::uwvm2::imported::ros::buffer_contract::operation::write||
                         Action==::uwvm2::imported::ros::buffer_contract::operation::seal)
            {
                constexpr std::uint64_t minimum=Action==::uwvm2::imported::ros::buffer_contract::operation::seal?16:8;
                if(capacity<minimum)return length(memory,length_output,minimum)?90:22;
            }
            model::buffers::registry::workspace_lease workspace;
            if(size>model::byte_buffer::inline_capacity)
            {
                checked=state->reserve_buffer_workspace(caller,Action,cap,static_cast<std::size_t>(size),workspace);
                if(checked!=model::error::success)return static_cast<unsigned>(checked);
            }
            model::byte_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,owned.writable().data(),owned.writable().size()))return 22;
            if(workspace&&!workspace.live())return static_cast<unsigned>(model::error::cancelled);
            auto deliver=[&](model::bytes prefix,model::bytes data) noexcept
            {
                if(prefix.size()>capacity||prefix.size()>UINT64_MAX-output||data.size()>capacity-prefix.size()||
                   !length(memory,length_output,prefix.size()+data.size()))return false;
                if(!prefix.empty()&&!runtime::preload_memory_write_host_api(memory,output,prefix.data(),prefix.size()))return false;
                return data.empty()||runtime::preload_memory_write_host_api(memory,output+prefix.size(),data.data(),data.size());
            };
            auto cancelled=[]() noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};
            auto status=state->invoke_buffer(caller,Action,cap,owned.view(),static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Copy a bounded canonical model carrier before native job admission.
     * @details Every memory32/64 range is normalized by the runtime. Request
     * and result may alias because the complete request is owned before any
     * result copy; the length cell may alias neither. Large copies reserve
     * model workspace before fast_io allocation and retain it until bytes are
     * freed. Worker execution is asynchronous and never borrows guest memory.
     * No native model operation inherits the local FP-preservation whitelist.
     */
    template<::uwvm2::imported::ros::model_contract::operation Action>
    inline std::uint32_t invoke_model(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                     std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            namespace contract=::uwvm2::imported::ros::model_contract;
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>model::transfer_limit+12288u||capacity>model::transfer_limit+512u||
               !normalized_range(memory,request,size,request_address)||!normalized_range(memory,output,capacity,output_address)||
               !normalized_range(memory,length_output,8,length_address)||overlap(output_address,capacity,length_address,8)||
               overlap(request_address,size,length_address,8))return 22;
            std::uint64_t minimum=(Action==contract::operation::submit||Action==contract::operation::cancel||
                Action==contract::operation::request_describe)?160:8;
            if constexpr(Action==contract::operation::request_describe||Action==contract::operation::request_release)
            {
                model::lease selected;auto status=state->permissions.admit(caller,cap,description.name,0,selected,false);
                if(status!=model::error::success)return static_cast<unsigned>(status);
                if(selected.specification().kind==model::purpose::arbiter)
                {
                    if constexpr(Action==contract::operation::request_describe)
                    {auto size=model::wire::writer::measuring(512);model::proposals::admission::receipt receipt;
                     if(!::uwvm2::imported::ros::proposal_contract::encode_receipt(size,receipt))return 22;
                     minimum=size.size();}
                    else minimum=8;
                }
            }
            auto checked=state->preflight_model_or_request(caller,Action,cap,static_cast<std::size_t>(size),capacity>=minimum);
            if(checked!=model::error::success)return static_cast<unsigned>(checked);
            if(capacity<minimum)return length(memory,length_output,minimum)?90:22;
            model::models::registry::workspace_lease workspace;
            if(size>model::byte_buffer::inline_capacity)
            {
                checked=state->model_store.reserve_workspace(caller,Action,cap,static_cast<std::size_t>(size),workspace);
                if(checked!=model::error::success)return static_cast<unsigned>(checked);
            }
            model::byte_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,owned.writable().data(),owned.writable().size()))return 22;
            if(workspace&&!workspace.live())return static_cast<unsigned>(model::error::cancelled);
            auto deliver=[&](model::bytes bytes) noexcept
            {
                return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                    (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));
            };
            auto cancelled=[]() noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};
            auto status=state->invoke_model(caller,Action,cap,owned.view(),static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Checked attachment-only driver carrier; no native driver code enters this VM.
     * @details Runtime origin and exact grant are checked before copying. The
     * dispatcher rechecks under a retained authority lease before any effect.
     * Guest addresses never reach a driver. Copies use recoverable fast_io
     * allocation and unconditional secure erasure. A failed result delivery is
     * retained for an exact sequence retry; effects are never silently resent.
     */
    template<model::drivers::operation Action>
    inline std::uint32_t invoke_driver(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                      std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            runtime::ros_caller_identity identity{};
            if(!runtime::ros_caller_identity_host_api(identity,false))return 13;
            std::string_view module{reinterpret_cast<char const*>(identity.module_name.data()),identity.module_name.size()};
            model::caller caller{identity.runtime_instance,identity.generation};std::shared_ptr<model::broker> state;
            // Refused principals cannot probe carrier extents or memory identity.
            // Use zero for malformed sizes so their sum cannot wrap before the
            // permission gate; authorized malformed carriers still fail below.
            bool bounded=size<=model::drivers::protocol::maximum_payload+24&&capacity<=model::drivers::protocol::maximum_payload+16;
            auto checked=configuration.preflight_driver(module,caller,cap,Action,bounded?static_cast<std::size_t>(size+capacity):0,state);
            if(checked!=model::error::success)return static_cast<unsigned>(checked);
            std::uint64_t input_address{},output_address{},length_address{};
            if(!bounded||
               !normalized_range(memory,request,size,input_address)||!normalized_range(memory,output,capacity,output_address)||
               !normalized_range(memory,length_output,8,length_address)||overlap(input_address,size,length_address,8)||
               overlap(output_address,capacity,length_address,8))return 22;
            auto& dispatch=*state->driver_store;
            model::erased_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(size&&!runtime::preload_memory_read_host_api(memory,request,owned.writable().data(),static_cast<std::size_t>(size)))return 22;
            auto deliver=[&](model::bytes bytes)noexcept{return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            std::size_t required{};
            auto status=dispatch.invoke(state->permissions,caller,module,Action,cap,owned.view(),static_cast<std::size_t>(capacity),required,deliver);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}catch(...){return 5;}
    }
    /** @brief Seven-argument device carrier with full FP, extent, authority and workspace checks. */
    template<::uwvm2::imported::ros::scene::robot_contract::operation Action>
    inline std::uint32_t invoke_device(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                      std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace contract=::uwvm2::imported::ros::scene::robot_contract;
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>16384||capacity>4096||!normalized_range(memory,request,size,request_address)||
                !normalized_range(memory,output,capacity,output_address)||!normalized_range(memory,length_output,8,length_address)||
                overlap(output_address,capacity,length_address,8)||overlap(request_address,size,length_address,8))return 22;
            if(!state->device_store)return static_cast<unsigned>(model::error::denied);
            auto checked=state->device_store->preflight(caller,Action,cap,static_cast<std::size_t>(size));if(checked!=model::error::success)return static_cast<unsigned>(checked);
            // The full raw device request fits this fixed local carrier. No guest
            // address, input alias or tagged pointer survives its checked copy.
            std::array<std::uint8_t,16384> owned;
            if(!runtime::preload_memory_read_host_api(memory,request,owned.data(),static_cast<std::size_t>(size)))return 22;
            auto deliver=[&](model::bytes bytes)noexcept{return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]()noexcept{using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto status=state->invoke_device(caller,Action,cap,{owned.data(),static_cast<std::size_t>(size)},static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Seven-argument bridge carrier with full FP, extent, authority and workspace checks. */
    template<::uwvm2::imported::ros::scene::robot_contract::operation Action>
    inline std::uint32_t invoke_bridge(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                      std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace contract=::uwvm2::imported::ros::scene::robot_contract;
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>4096||capacity>1024||!normalized_range(memory,request,size,request_address)||
                !normalized_range(memory,output,capacity,output_address)||!normalized_range(memory,length_output,8,length_address)||
                overlap(output_address,capacity,length_address,8)||overlap(request_address,size,length_address,8))return 22;
            if(!state->bridge_store)return static_cast<unsigned>(model::error::denied);
            auto checked=state->bridge_store->preflight(caller,Action,cap,static_cast<std::size_t>(size));if(checked!=model::error::success)return static_cast<unsigned>(checked);
            // The full raw bridge request fits this fixed local carrier. No guest
            // address, input alias or tagged pointer survives its checked copy.
            std::array<std::uint8_t,4096> owned;
            if(!runtime::preload_memory_read_host_api(memory,request,owned.data(),static_cast<std::size_t>(size)))return 22;
            auto deliver=[&](model::bytes bytes)noexcept{return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]()noexcept{using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto status=state->invoke_bridge(caller,Action,cap,{owned.data(),static_cast<std::size_t>(size)},static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Seven-argument media carrier with full FP, extent, authority and workspace checks. */
    template<::uwvm2::imported::ros::scene::robot_contract::operation Action>
    inline std::uint32_t invoke_media(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                      std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace contract=::uwvm2::imported::ros::scene::robot_contract;
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>4096||capacity>1024||!normalized_range(memory,request,size,request_address)||
                !normalized_range(memory,output,capacity,output_address)||!normalized_range(memory,length_output,8,length_address)||
                overlap(output_address,capacity,length_address,8)||overlap(request_address,size,length_address,8))return 22;
            if(!state->media_store)return static_cast<unsigned>(model::error::denied);
            auto checked=state->media_store->preflight(caller,Action,cap,static_cast<std::size_t>(size));if(checked!=model::error::success)return static_cast<unsigned>(checked);
            // The full raw media request fits this fixed local carrier. No guest
            // address, input alias or tagged pointer survives its checked copy.
            std::array<std::uint8_t,4096> owned;
            if(!runtime::preload_memory_read_host_api(memory,request,owned.data(),static_cast<std::size_t>(size)))return 22;
            auto deliver=[&](model::bytes bytes)noexcept{return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]()noexcept{using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto status=state->invoke_media(caller,Action,cap,{owned.data(),static_cast<std::size_t>(size)},static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Seven-argument sensor carrier with full FP, extent, authority and workspace checks. */
    template<::uwvm2::imported::ros::sensor_contract::operation Action>
    inline std::uint32_t invoke_sensor(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                      std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace contract=::uwvm2::imported::ros::sensor_contract;
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>contract::maximum_request_bytes||capacity>contract::maximum_reply_bytes||!normalized_range(memory,request,size,request_address)||
                !normalized_range(memory,output,capacity,output_address)||!normalized_range(memory,length_output,8,length_address)||
                overlap(output_address,capacity,length_address,8)||overlap(request_address,size,length_address,8))return 22;
            if(!state->sensor_store)return static_cast<unsigned>(model::error::denied);
            auto checked=state->sensor_store->preflight(caller,Action,cap,static_cast<std::size_t>(size));if(checked!=model::error::success)return static_cast<unsigned>(checked);
            // The full sensor request fits this fixed local carrier. No guest
            // address, input alias or tagged pointer survives its checked copy.
            std::array<std::uint8_t,contract::maximum_request_bytes> owned;
            if(!runtime::preload_memory_read_host_api(memory,request,owned.data(),static_cast<std::size_t>(size)))return 22;
            auto deliver=[&](model::bytes bytes)noexcept{return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]()noexcept{using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto status=state->invoke_sensor(caller,Action,cap,{owned.data(),static_cast<std::size_t>(size)},static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Own complete proposal envelopes before decode; every path keeps the full FP guard. */
    template<::uwvm2::imported::ros::proposal_contract::operation Action>
    inline std::uint32_t invoke_proposal(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                        std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace contract=::uwvm2::imported::ros::proposal_contract;
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>contract::maximum_request_bytes||capacity>model::proposals::maximum_envelope_bytes||!normalized_range(memory,request,size,request_address)||
                !normalized_range(memory,output,capacity,output_address)||!normalized_range(memory,length_output,8,length_address)||
                overlap(output_address,capacity,length_address,8)||overlap(request_address,size,length_address,8))return 22;
            if(!state->proposal_store)return static_cast<unsigned>(model::error::denied);
            auto checked=state->proposal_store->preflight(caller,Action,cap,static_cast<std::size_t>(size));if(checked!=model::error::success)return static_cast<unsigned>(checked);
            model::proposal_guest_dispatch::workspace_lease workspace;
            if(size>model::byte_buffer::inline_capacity)
            {checked=state->proposal_store->reserve_workspace(caller,Action,cap,static_cast<std::size_t>(size),workspace);if(checked!=model::error::success)return static_cast<unsigned>(checked);}
            model::byte_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,owned.writable().data(),owned.writable().size()))return 22;
            auto deliver=[&](model::bytes bytes)noexcept{return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]()noexcept{using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto status=state->invoke_proposal(caller,Action,cap,owned.view(),static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Own bounded TF records before dispatch; every numerical path keeps the full FP guard.
     * @details Length output may overlap neither request nor result. Request
     * and result may alias because the entire request is copied first. Large
     * copies hold a separate carrier reservation until their bytes are freed.
     * No unchecked imported-memory span reaches frame math or a delivery sink.
     */
    template<::uwvm2::imported::ros::transform_contract::operation Action>
    inline std::uint32_t invoke_transform(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                         std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            namespace contract=::uwvm2::imported::ros::transform_contract;
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>contract::request_bound(contract::operation::publish,contract::maximum_batch)||capacity>model::transfer_limit||
                !normalized_range(memory,request,size,request_address)||!normalized_range(memory,output,capacity,output_address)||
                !normalized_range(memory,length_output,8,length_address)||overlap(output_address,capacity,length_address,8)||
                overlap(request_address,size,length_address,8))return 22;
            constexpr std::uint64_t minimum=Action==contract::operation::open?8:Action==contract::operation::lookup?622:
                Action==contract::operation::available?64:Action==contract::operation::velocity?48:Action==contract::operation::take?790:160;
            auto checked=state->transform_store.preflight(caller,Action,cap,static_cast<std::size_t>(size),capacity>=minimum);
            if(checked!=model::error::success)return static_cast<unsigned>(checked);
            if(capacity<minimum)return length(memory,length_output,minimum)?90:22;
            model::transforms::registry::carrier_lease workspace;
            if(size>model::byte_buffer::inline_capacity)
            {
                checked=state->transform_store.reserve_workspace(caller,Action,cap,static_cast<std::size_t>(size),workspace);
                if(checked!=model::error::success)return static_cast<unsigned>(checked);
            }
            model::byte_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,owned.writable().data(),owned.writable().size()))return 22;
            if(workspace&&!workspace.live())return static_cast<unsigned>(model::error::cancelled);
            auto deliver=[&](model::bytes bytes) noexcept
            {return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]() noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};
            auto status=state->transform_store.invoke(caller,Action,cap,owned.view(),static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Copy a complete private log or cached diagnostic request under finite carrier capacity.
     * @details The primary capability is checked before allocation or guest reads.
     * Large records use recoverable fast_io memory and a separately retained
     * workspace reservation. Length cells cannot alias request or result. The
     * full FP guard remains active, and failed copy cannot acknowledge a page.
     */
    template<::uwvm2::imported::ros::log_diagnostic_contract::operation Action>
    inline std::uint32_t invoke_log_diagnostic(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                             std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            namespace contract=::uwvm2::imported::ros::log_diagnostic_contract;
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            if(!state->log_diagnostic_store)return static_cast<unsigned>(model::error::denied);
            auto& dispatcher=*state->log_diagnostic_store;
            std::uint64_t request_address{},output_address{},length_address{};
            constexpr auto bound=Action==contract::operation::emit?model::transfer_limit:contract::maximum_read_request;
            if(size>bound||capacity>model::transfer_limit||
               !normalized_range(memory,request,size,request_address)||!normalized_range(memory,output,capacity,output_address)||
               !normalized_range(memory,length_output,8,length_address)||overlap(output_address,capacity,length_address,8)||
               overlap(request_address,size,length_address,8))return 22;
            constexpr std::uint64_t minimum=Action==contract::operation::emit?contract::receipt_bytes:24;
            auto checked=dispatcher.preflight(caller,Action,cap,static_cast<std::size_t>(size),capacity>=minimum);
            if(checked!=model::error::success)return static_cast<unsigned>(checked);
            if(capacity<minimum)return length(memory,length_output,minimum)?90:22;
            model::log_diagnostic_dispatch::workspace_lease workspace;
            if(size>model::byte_buffer::inline_capacity)
            {
                checked=dispatcher.reserve_workspace(caller,Action,cap,static_cast<std::size_t>(size),workspace);
                if(checked!=model::error::success)return static_cast<unsigned>(checked);
            }
            model::byte_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,owned.writable().data(),owned.writable().size()))return 22;
            auto deliver=[&](model::bytes bytes) noexcept
            {return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]() noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto status=dispatcher.invoke(caller,Action,cap,owned.view(),static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Copy a bounded recording request before resolving actual sealed inputs.
     * @details Primary permission and the complete minimum output are checked
     * before copying guest data. The owned request has independent finite
     * workspace, and no source-buffer pointer crosses the Wasm boundary.
     * Result-length cells cannot alias input or output, including tagged aliases.
     * The generic wrapper retains its full floating-point control guard.
     */
    template<::uwvm2::imported::ros::recording_contract::operation Action>
    inline std::uint32_t invoke_recording(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
                                          std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output) noexcept
    {
        try
        {
            namespace contract=::uwvm2::imported::ros::recording_contract;
            constexpr auto description=contract::operations[static_cast<std::size_t>(Action)];
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(description.name,caller,state))return code;
            if(!state->recording_store)return static_cast<unsigned>(model::error::denied);
            auto& dispatcher=*state->recording_store;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>contract::maximum_request||capacity>model::transfer_limit||
               !normalized_range(memory,request,size,request_address)||!normalized_range(memory,output,capacity,output_address)||
               !normalized_range(memory,length_output,8,length_address)||overlap(output_address,capacity,length_address,8)||
               overlap(request_address,size,length_address,8))return 22;
            constexpr std::uint64_t minimum=Action==contract::operation::recording_append||Action==contract::operation::recording_checkpoint||
                Action==contract::operation::recording_stop?contract::receipt_bytes:Action==contract::operation::replay_read?16:8;
            auto checked=dispatcher.preflight(caller,Action,cap,static_cast<std::size_t>(size));
            if(checked!=model::error::success)return static_cast<unsigned>(checked);
            if(capacity<minimum)return length(memory,length_output,minimum)?90:22;
            model::recording_dispatch::workspace_lease workspace;
            if(size>model::byte_buffer::inline_capacity)
            {
                checked=dispatcher.reserve_workspace(caller,Action,cap,static_cast<std::size_t>(size),workspace);
                if(checked!=model::error::success)return static_cast<unsigned>(checked);
            }
            model::byte_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(!runtime::preload_memory_read_host_api(memory,request,owned.writable().data(),owned.writable().size()))return 22;
            auto deliver=[&](model::bytes bytes) noexcept
            {return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]() noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};auto status=dispatcher.invoke(caller,Action,cap,owned.view(),static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
    /** @brief Prepare sealed inputs and bounded output stores before guest activation.
     * @details initialize=false validates metadata only and never calls the
     * provider. Validation alone performs no I/O. Failure leaves every input
     * environment unselectable and drops all unpublished sealed snapshots.
     */
    inline model::error prepare(bool initialize=true) noexcept
    {
        try{return initialize?configuration.prepare_resources(prepare_input,model::deployment::external_model_provider{},model::deployment::external_communication_provider{}):configuration.validate();}
        catch(std::bad_alloc const&){return model::error::no_memory;}
        catch(std::length_error const&){return model::error::too_large;}
    }
    namespace wrapper
    {
        /** @brief Closed local callback whitelist with an explicit FP control boundary.
         * @details Every listed LLVM wrapper constructs a register-only control
         * guard before calling Fn, including before identify/activation/enrollment
         * can allocate. Integer-only code is insufficient by itself: allocator
         * interposition or a nested host callback can change rounding, exception
         * masks, flush controls or precision before returning/throwing. Owning
         * scene allocations use recoverable fast_io, not std::new_handler.
         * The guard restores controls on every returning path, including replay,
         * refusal, cancellation and allocation failure. It remains alive until
         * the result tuple has been written. Sticky status flags are caller-saved.
         * Only targets with the existing native control guard may advertise
         * preserves_wasm_control. Other targets and interpreter builds retain
         * may_modify and the complete outer runtime guard. New entries inherit
         * neither this whitelist nor a weaker boundary by default.
         * LLVM's checked-memory API independently guards arbitrary imported-memory
         * callbacks. Runtime bridge suspension, memory checks and retirement stay
         * active. Local scheduling/output callbacks call no native ROS, device or
         * model provider. resource.close retires local scheduling, sealed-input
         * or lifecycle handles; logical finalization does not stop a real device,
         * and sealed bytes stay retained in their binding without native close.
         * Artifact open/describe use pinned metadata, while read copies either
         * the private volatile cache or a concrete sealed-memfd input via pread;
         * none invokes an extensible provider or performs lazy provisioning.
         * Registered module messages select only an already prepared broker.
         * Their integer byte carrier, admission and router use owned shared
         * memory with native/imported memory rejected; their only callbacks are
         * VM-owned checked copy/erase functions. No decoder, exported function
         * or provider is invoked. Allocation/clock failures and cleanup still
         * execute inside this register-control guard, including destruction.
         * Observation wrappers project only const deployment policy and the
         * execution-leased declaration registry. They neither enroll callers
         * nor invoke a provider, loader or guest export. Their bounded page
         * allocation, checked memory callbacks, failed delivery and secure
         * erasure all complete before this guard is destroyed. Legacy/WASI
         * policy projection is metadata-only; it never calls those host APIs.
         * Registered driver carriers reach only the closed integer schema
         * validator, local admission/receipt state and authenticated channel
         * syscalls. They cannot call vendor code, a dynamic decoder or an
         * extensible provider. The local control guard covers identity hooks,
         * allocations, syscalls, checked memory delivery and secure erasure,
         * including failures. Driver-side floating-point work runs in the
         * external process. A future in-process callback invalidates this
         * classification and must restore the complete outer boundary.
         * Publication's two-phase final-check/rollback path and managed lifecycle
         * carriers remain outside this closed whitelist pending separate audit.
         * Adding external provider work requires
         * reconsidering the local boundary, not inheriting this declaration.
         */
        template<auto Fn> inline constexpr bool audited_local_fp_preserving{};
#if defined(UWVM_RUNTIME_LLVM_JIT)
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_permission_report>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_module_permission_report>{true};
#define UWVM_OBSERVATION_FP_POLICY(symbol,action,external) \
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_module_observation<model::permission_report::module_operation::action>>{true};
        UWVM_ROS_MODULE_OBSERVATION_FUNCTIONS(UWVM_OBSERVATION_FP_POLICY)
#undef UWVM_OBSERVATION_FP_POLICY
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_module_observation<model::permission_report::module_operation::describe,true>>{true};
#define UWVM_MODULE_CALL_FP(symbol,action,external) \
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_module_call<model::module_exchange::operation::action>>{true};
        UWVM_ROS_MODULE_CALL_FUNCTIONS(UWVM_MODULE_CALL_FP)
#undef UWVM_MODULE_CALL_FP
#if !defined(UWVM_ROS_ATTACHMENT_OPENSSL)
#define UWVM_DRIVER_FP_POLICY(symbol,action,external) \
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_driver<model::drivers::operation::action>>{true};
        UWVM_ROS_DRIVER_FUNCTIONS(UWVM_DRIVER_FP_POLICY)
#undef UWVM_DRIVER_FP_POLICY
#endif
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::guard_create>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::guard_trigger>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::guard_take>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::clock_create>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::clock_read>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::clock_update>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::timer_create>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::timer_reset>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::timer_cancel>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::timer_take>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::wait_set_create>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::wait_set_add>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::wait_set_remove>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::wait_set_acknowledge>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke<contract::operation::wait_set_wait>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_artifact<::uwvm2::imported::ros::artifact_contract::operation::open>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_artifact<::uwvm2::imported::ros::artifact_contract::operation::read>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_artifact<::uwvm2::imported::ros::artifact_contract::operation::describe>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_output<::uwvm2::imported::ros::output_contract::operation::begin>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_output<::uwvm2::imported::ros::output_contract::operation::append>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_output<::uwvm2::imported::ros::output_contract::operation::commit>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_output<::uwvm2::imported::ros::output_contract::operation::abort>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_output<::uwvm2::imported::ros::output_contract::operation::remove>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_output<::uwvm2::imported::ros::output_contract::operation::status>>{true};
        template<> inline constexpr bool audited_local_fp_preserving<&invoke_output<::uwvm2::imported::ros::output_contract::operation::receipt>>{true};
#endif
        inline constexpr bool native_control_guard_available=runtime::details::wasm_fp_has_native_control_guard;
        /** @brief Empty fallback keeps unsupported native guard types uninstantiated. */
        struct no_local_fp_control_guard {};
        using features=::uwvm2::uwvm::wasm::type::binfmt_ver1_feature_list_t;
        using value=::uwvm2::uwvm::wasm::type::feature_list_final_value_type_t<features>;
        template<typename T> consteval value mapped(){return sizeof(T)<=4?value::i32:value::i64;}
        /** @brief Capture a monotonic entry time only while the operator's trace is enabled. */
        struct trace_entry
        {
#if defined(UWVM_VERSION_DEV)
            bool selected{};
            std::uint64_t started{};
#else
            static constexpr bool selected = false;
#endif
        };
        inline trace_entry begin_trace() noexcept
        {
#if defined(UWVM_VERSION_DEV)
            namespace trace=::uwvm2::utils::debug::host_calls;
            if(trace::observing(trace::channel::rosv2)) return {true,trace::now()};
#endif
            return {};
        }
        /** @brief Attribute a completed host result and its optional local elapsed time.
         * @details Clock reads occur only for active observation. Elapsed time ends
         * before caller lookup, journal allocation and sink delivery. It includes
         * time waiting inside the registered operation and excludes remote work
         * performed after the host has returned.
         */
        inline void observe_result(std::string_view name,std::uint32_t result,[[maybe_unused]] trace_entry entry,
                                   [[maybe_unused]] bool audit) noexcept
        {
#if defined(UWVM_VERSION_DEV)
            namespace trace=::uwvm2::utils::debug::host_calls;
            auto finished=entry.started?trace::now():0;
#endif
            runtime::ros_caller_identity identity{};
            // The sealed runtime may still identify the caller of a rejected operation
            // after its authority retires. This observation never authorizes a call.
            bool const known = runtime::ros_observed_caller_identity_host_api(identity);
            std::string_view module;
            if(known)module={reinterpret_cast<char const*>(identity.module_name.data()),identity.module_name.size()};
#if defined(UWVM_VERSION_DEV)
            if(entry.selected)trace::observe_scene_result("uwvm_ros_v2",name,result,module,known?identity.generation:0,
                                                         entry.started,finished);
            if(!audit)return;
#endif
            auto time=::uwvm2::utils::host_clock::now();
            auto ns=::uwvm2::utils::host_clock::available() ?
                std::chrono::duration_cast<std::chrono::nanoseconds>(time.time_since_epoch()).count() : 0;
            configuration.observe_call(module,known?identity.generation:0,name,result,ns>0?static_cast<std::uint64_t>(ns):0,
                [](std::string_view record) noexcept {return ::uwvm2::imported::ros::audit_io::write_record(record);});
        }
        template<typename Signature,Signature Fn,auto& Name> struct function;
        template<typename R,typename... A,R(*Fn)(A...) noexcept,auto& Name>
        struct function<R(*)(A...) noexcept,Fn,Name>
        {
            using result_tuple=::uwvm2::uwvm::wasm::type::import_function_result_tuple_t<features,mapped<R>()>;
            using parameter_tuple=::uwvm2::uwvm::wasm::type::import_function_parameter_tuple_t<features,mapped<A>()...>;
            using local_imported_function_type=::uwvm2::uwvm::wasm::type::local_imported_function_type_t<result_tuple,parameter_tuple>;
            inline static constexpr ::uwvm2::utils::container::u8string_view function_name{Name};
            inline static constexpr bool guards_fp_controls=audited_local_fp_preserving<Fn>&&native_control_guard_available;
            using fp_control_guard=std::conditional_t<guards_fp_controls,
                runtime::details::scoped_wasm_native_fp_control_restore,no_local_fp_control_guard>;
            inline static constexpr auto wasm_fp_control_policy=guards_fp_controls?
                ::uwvm2::uwvm::wasm::type::local_imported_wasm_fp_control_policy_t::preserves_wasm_control:
                ::uwvm2::uwvm::wasm::type::local_imported_wasm_fp_control_policy_t::may_modify;
            static void call(local_imported_function_type& call) noexcept
            {
                [[maybe_unused]] fp_control_guard const controls;
                auto invoke=[&]()
                {
                    auto entry=begin_trace();
                    auto result=[&]<std::size_t... I>(std::index_sequence<I...>) noexcept
                    {return Fn(static_cast<A>(::uwvm2::utils::container::get<I>(call.params))...);}(std::make_index_sequence<sizeof...(A)>{});
                    ::uwvm2::utils::container::get<0>(call.res)=result;
                    bool const audit = configuration.auditing_calls();
                    if(audit || entry.selected) [[unlikely]]
                        observe_result({reinterpret_cast<char const*>(Name),sizeof(Name)-1},result,entry,audit);
                };
                if constexpr(std::u8string_view{Name}==u8"service.available")
                {
                    if(configuration.auditing_calls())
                    {
                        model::service_observation_audit::frame frame{true};
                        invoke();
                    }
                    else invoke();
                }
                else invoke();
            }
        };
        template<auto Fn,auto& Name> using native=function<decltype(Fn),Fn,Name>;
        inline constexpr char8_t abi_name[]=u8"abi",open_name[]=u8"capability.open",close_name[]=u8"capability.close";
#define UWVM_REPORT_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_PERMISSION_REPORT_FUNCTIONS(UWVM_REPORT_NAME)
        UWVM_ROS_MODULE_OBSERVATION_FUNCTIONS(UWVM_REPORT_NAME)
        UWVM_ROS_MODULE_REPORT_FUNCTIONS(UWVM_REPORT_NAME)
        UWVM_ROS_VM_OBSERVATION_FUNCTIONS(UWVM_REPORT_NAME)
        UWVM_ROS_MODULE_CALL_FUNCTIONS(UWVM_REPORT_NAME)
        UWVM_ROS_DRIVER_FUNCTIONS(UWVM_REPORT_NAME)
#undef UWVM_REPORT_NAME
#define UWVM_REVOCATION_NAME(symbol,action,external,words,bytes) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_REVOCATION_FUNCTIONS(UWVM_REVOCATION_NAME)
#undef UWVM_REVOCATION_NAME
#define UWVM_SCENE_NAME(symbol,external,grant,words,bytes) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_RUNTIME_FUNCTIONS(UWVM_SCENE_NAME)
#undef UWVM_SCENE_NAME
#define UWVM_ARTIFACT_NAME(symbol,action,external,words,bytes) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_ARTIFACT_FUNCTIONS(UWVM_ARTIFACT_NAME)
#undef UWVM_ARTIFACT_NAME
#define UWVM_OUTPUT_NAME(symbol,action,external,words) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_OUTPUT_FUNCTIONS(UWVM_OUTPUT_NAME)
#undef UWVM_OUTPUT_NAME
#define UWVM_MANAGED_NAME(symbol,action,external,words,bytes) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_MANAGED_LIFECYCLE_FUNCTIONS(UWVM_MANAGED_NAME)
#undef UWVM_MANAGED_NAME
#define UWVM_BUFFER_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_BUFFER_FUNCTIONS(UWVM_BUFFER_NAME)
#undef UWVM_BUFFER_NAME
#define UWVM_MODEL_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_MODEL_FUNCTIONS(UWVM_MODEL_NAME)
        UWVM_ROS_REQUEST_FUNCTIONS(UWVM_MODEL_NAME)
#undef UWVM_MODEL_NAME
#define UWVM_SENSOR_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_SENSOR_FUNCTIONS(UWVM_SENSOR_NAME)
#undef UWVM_SENSOR_NAME
#define UWVM_DEVICE_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_DEVICE_FUNCTIONS(UWVM_DEVICE_NAME)
        UWVM_ROS_CONTROL_FUNCTIONS(UWVM_DEVICE_NAME)
#undef UWVM_DEVICE_NAME
#define UWVM_MEDIA_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_MEDIA_FUNCTIONS(UWVM_MEDIA_NAME)
#undef UWVM_MEDIA_NAME
#define UWVM_BRIDGE_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_BRIDGE_FUNCTIONS(UWVM_BRIDGE_NAME)
#undef UWVM_BRIDGE_NAME
#define UWVM_PROPOSAL_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_PROPOSAL_FUNCTIONS(UWVM_PROPOSAL_NAME)
#undef UWVM_PROPOSAL_NAME
#define UWVM_TRANSFORM_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_TRANSFORM_FUNCTIONS(UWVM_TRANSFORM_NAME)
#undef UWVM_TRANSFORM_NAME
#define UWVM_LOG_DIAGNOSTIC_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_LOG_DIAGNOSTIC_FUNCTIONS(UWVM_LOG_DIAGNOSTIC_NAME)
#undef UWVM_LOG_DIAGNOSTIC_NAME
#define UWVM_RECORDING_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_RECORDING_FUNCTIONS(UWVM_RECORDING_NAME)
#undef UWVM_RECORDING_NAME
#define UWVM_COMMUNICATION_NAME(symbol,action,external) inline constexpr char8_t symbol##_name[]=u8##external;
        UWVM_ROS_COMMUNICATION_FUNCTIONS(UWVM_COMMUNICATION_NAME)
        UWVM_ROS_COMMUNICATION_RECEIVE_FUNCTIONS(UWVM_COMMUNICATION_NAME)
        UWVM_ROS_COMMUNICATION_CREATION_FUNCTIONS(UWVM_COMMUNICATION_NAME)
        UWVM_ROS_PARAMETER_FUNCTIONS(UWVM_COMMUNICATION_NAME)
        UWVM_ROS_ACTION_FUNCTIONS(UWVM_COMMUNICATION_NAME)
        UWVM_ROS_GRAPH_FUNCTIONS(UWVM_COMMUNICATION_NAME)
        UWVM_ROS_EVENT_FUNCTIONS(UWVM_COMMUNICATION_NAME)
#undef UWVM_COMMUNICATION_NAME
    }
    /** @brief Checked communication carrier with detached, bounded guest input.
     * @details Exact permission and carrier limits precede allocation. Variable
     * communication requests reserve their worker's workspace; receive and
     * creation controls use fixed stack records. No guest pointer survives an
     * invocation, and output/length overlaps are rejected before any effect.
     * Creation publication and receive acknowledgement remain separate calls.
     * All wrappers retain the normal full FP boundary; provider work is not
     * classified as a locally audited floating-point-preserving shortcut.
     */
    template<auto Action>
    inline std::uint32_t invoke_communication(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
        std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            namespace communication=::uwvm2::imported::ros::communication_contract;
            namespace receive=::uwvm2::imported::ros::communication_receive_contract;
            namespace actions=::uwvm2::imported::ros::action_contract;
            constexpr bool ordinary=std::is_same_v<decltype(Action),communication::operation>;
            constexpr bool receiving=std::is_same_v<decltype(Action),receive::operation>;
            constexpr bool acting=std::is_same_v<decltype(Action),actions::operation>;
            constexpr bool graphing=std::is_same_v<decltype(Action),::uwvm2::imported::ros::graph_contract::operation>;
            constexpr bool streaming=std::is_same_v<decltype(Action),::uwvm2::imported::ros::event_contract::operation>;
            constexpr bool observing=graphing||streaming;
            constexpr auto name=[]
            {
                if constexpr(ordinary)return communication::operations[static_cast<std::size_t>(Action)].name;
                else if constexpr(receiving)return receive::operations[static_cast<std::size_t>(Action)].name;
                else if constexpr(acting)return actions::operations[static_cast<std::size_t>(Action)].name;
                else if constexpr(graphing)return ::uwvm2::imported::ros::graph_contract::operations[static_cast<std::size_t>(Action)].name;
                else if constexpr(streaming)return ::uwvm2::imported::ros::event_contract::operations[static_cast<std::size_t>(Action)].name;
                else return model::communication_open_dispatch::names[static_cast<std::size_t>(Action)];
            }();
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(name,caller,state))return code;
            if(!state->communication_store)return 13;
            auto& dispatcher=*state->communication_store;
            std::uint64_t request_address{},output_address{},length_address{};
            if(size>model::transfer_limit||capacity>model::transfer_limit||
                !normalized_range(memory,request,size,request_address)||!normalized_range(memory,output,capacity,output_address)||
                !normalized_range(memory,length_output,8,length_address)||overlap(output_address,capacity,length_address,8)||
                overlap(request_address,size,length_address,8))return 22;
            model::error checked;
            if constexpr(ordinary)checked=dispatcher.preflight(caller,Action,cap,static_cast<std::size_t>(size));
            else if constexpr(receiving)checked=dispatcher.preflight_receive(caller,Action,cap,static_cast<std::size_t>(size));
            else if constexpr(acting)checked=dispatcher.preflight_action(caller,Action,cap,static_cast<std::size_t>(size));
            else if constexpr(observing)checked=dispatcher.preflight_observation<Action>(caller,cap,static_cast<std::size_t>(size));
            else checked=dispatcher.preflight_creation(caller,Action,cap,static_cast<std::size_t>(size));
            if(checked!=model::error::success)return static_cast<unsigned>(checked);
            std::conditional_t<acting||observing,model::communication_workspace::lease,model::communication_dispatch::workspace_lease> reservation;
            model::byte_buffer dynamic;std::array<std::uint8_t,48> fixed;
            model::bytes input;
            if constexpr(ordinary||acting||observing)
            {
                if constexpr(observing)checked=dispatcher.reserve_observation_workspace<Action>(caller,cap,static_cast<std::size_t>(size),reservation);
                else if constexpr(acting)checked=dispatcher.reserve_action_workspace(caller,Action,cap,static_cast<std::size_t>(size),reservation);
                else checked=dispatcher.reserve_workspace(caller,Action,cap,static_cast<std::size_t>(size),reservation);
                if(checked!=model::error::success)return static_cast<unsigned>(checked);
                dynamic.resize_for_overwrite(static_cast<std::size_t>(size));
                if(size&&!runtime::preload_memory_read_host_api(memory,request,dynamic.writable().data(),static_cast<std::size_t>(size)))return 22;
                input=dynamic.view();
            }
            else
            {
                if(size>fixed.size()||(size&&!runtime::preload_memory_read_host_api(memory,request,fixed.data(),static_cast<std::size_t>(size))))return 22;
                input={fixed.data(),static_cast<std::size_t>(size)};
            }
            auto deliver=[&](model::bytes bytes)noexcept
            {return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]()noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};model::error status;
            if constexpr(ordinary)status=state->invoke_communication(caller,Action,cap,input,static_cast<std::size_t>(capacity),required,deliver,cancelled);
            else if constexpr(receiving)status=state->invoke_communication_receive(caller,Action,cap,input,static_cast<std::size_t>(capacity),required,deliver,cancelled);
            else if constexpr(acting)status=state->invoke_communication_action(caller,Action,cap,input,static_cast<std::size_t>(capacity),required,deliver,cancelled);
            else if constexpr(observing)status=state->invoke_communication_observation<Action>(caller,cap,input,static_cast<std::size_t>(capacity),required,deliver,cancelled);
            else status=state->invoke_communication_creation(caller,Action,cap,input,static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}catch(...){return 5;}
    }
    /** @brief Detached bounded parameter carrier under exact originating-node authority.
     * @details All memory extents and output/length aliasing are checked before
     * preflight. The current grant and explicit workspace capacity precede a
     * recoverable fast_io copy. No guest address survives the call. Parameter
     * store mutations retain exact recovery receipts after a failed delivery;
     * short output is measured before commit. Floating-point parameter values
     * use the complete runtime FP boundary without an integer-only exemption.
     */
    template<::uwvm2::imported::ros::parameter_contract::operation Action>
    inline std::uint32_t invoke_parameter(std::uint64_t cap,std::uint32_t memory,std::uint64_t request,std::uint64_t size,
        std::uint64_t output,std::uint64_t capacity,std::uint64_t length_output)noexcept
    {
        try
        {
            constexpr auto name=::uwvm2::imported::ros::parameter_contract::operations[static_cast<std::size_t>(Action)].name;
            model::caller caller;std::shared_ptr<model::broker> state;if(auto code=identify(name,caller,state))return code;
            if(!state->parameter_store)return 13;
            std::uint64_t input_address{},output_address{},length_address{};
            if(size>model::transfer_limit||capacity>model::transfer_limit||
                !normalized_range(memory,request,size,input_address)||!normalized_range(memory,output,capacity,output_address)||
                !normalized_range(memory,length_output,8,length_address)||overlap(output_address,capacity,length_address,8)||
                overlap(input_address,size,length_address,8))return 22;
            auto& dispatch=*state->parameter_store;
            auto status=dispatch.preflight(caller,Action,cap,static_cast<std::size_t>(size),capacity>=8);
            if(status!=model::error::success)return static_cast<unsigned>(status);
            model::parameters::workspace_lease reserved;
            status=dispatch.reserve_workspace(caller,Action,cap,static_cast<std::size_t>(size),reserved,capacity>=8);
            if(status!=model::error::success)return static_cast<unsigned>(status);
            model::byte_buffer owned;owned.resize_for_overwrite(static_cast<std::size_t>(size));
            if(size&&!runtime::preload_memory_read_host_api(memory,request,owned.writable().data(),static_cast<std::size_t>(size)))return 22;
            auto deliver=[&](model::bytes bytes)noexcept
            {return bytes.size()<=capacity&&length(memory,length_output,bytes.size())&&
                (bytes.empty()||runtime::preload_memory_write_host_api(memory,output,bytes.data(),bytes.size()));};
            auto cancelled=[]()noexcept
            {using context=::uwvm2::imported::ros::synchronous_wait_context;return context::stop_requested()||context::process_cancelled();};
            std::size_t required{};status=state->invoke_parameter(caller,Action,cap,owned.view(),static_cast<std::size_t>(capacity),required,deliver,cancelled);
            if(status==model::error::too_large&&!length(memory,length_output,required))return 22;
            return static_cast<unsigned>(status);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}catch(...){return 5;}
    }
    /** @brief Concrete new module; imports never select old ROS implementations. */
    struct local_imported_module_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view module_name{u8"uwvm_ros_v2"};
        using local_function_tuple=::uwvm2::utils::container::tuple<
#define UWVM_REPORT_FUNCTION(symbol,action,external) wrapper::native<&invoke_permission_report,wrapper::symbol##_name>,
            UWVM_ROS_PERMISSION_REPORT_FUNCTIONS(UWVM_REPORT_FUNCTION)
#undef UWVM_REPORT_FUNCTION
#define UWVM_MODULE_OBSERVATION_FUNCTION(symbol,action,external) wrapper::native<&invoke_module_observation<model::permission_report::module_operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_MODULE_OBSERVATION_FUNCTIONS(UWVM_MODULE_OBSERVATION_FUNCTION)
#undef UWVM_MODULE_OBSERVATION_FUNCTION
#define UWVM_VM_OBSERVATION_FUNCTION(symbol,action,external) wrapper::native<&invoke_module_observation<model::permission_report::module_operation::describe,true>,wrapper::symbol##_name>,
            UWVM_ROS_VM_OBSERVATION_FUNCTIONS(UWVM_VM_OBSERVATION_FUNCTION)
#undef UWVM_VM_OBSERVATION_FUNCTION
#define UWVM_MODULE_REPORT_FUNCTION(symbol,action,external) wrapper::native<&invoke_module_permission_report,wrapper::symbol##_name>,
            UWVM_ROS_MODULE_REPORT_FUNCTIONS(UWVM_MODULE_REPORT_FUNCTION)
#undef UWVM_MODULE_REPORT_FUNCTION
#define UWVM_MODULE_CALL_FUNCTION(symbol,action,external) wrapper::native<&invoke_module_call<model::module_exchange::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_MODULE_CALL_FUNCTIONS(UWVM_MODULE_CALL_FUNCTION)
#undef UWVM_MODULE_CALL_FUNCTION
#define UWVM_REVOCATION_FUNCTION(symbol,action,external,words,bytes) wrapper::native<&invoke_revocation,wrapper::symbol##_name>,
            UWVM_ROS_REVOCATION_FUNCTIONS(UWVM_REVOCATION_FUNCTION)
#undef UWVM_REVOCATION_FUNCTION
#define UWVM_SCENE_FUNCTION(symbol,external,grant,words,bytes) wrapper::native<&invoke<contract::operation::symbol>,wrapper::symbol##_name>,
            UWVM_ROS_RUNTIME_FUNCTIONS(UWVM_SCENE_FUNCTION)
#undef UWVM_SCENE_FUNCTION
#define UWVM_ARTIFACT_FUNCTION(symbol,action,external,words,bytes) wrapper::native<&invoke_artifact<::uwvm2::imported::ros::artifact_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_ARTIFACT_FUNCTIONS(UWVM_ARTIFACT_FUNCTION)
#undef UWVM_ARTIFACT_FUNCTION
#define UWVM_OUTPUT_FUNCTION(symbol,action,external,words) wrapper::native<&invoke_output<::uwvm2::imported::ros::output_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_OUTPUT_FUNCTIONS(UWVM_OUTPUT_FUNCTION)
#undef UWVM_OUTPUT_FUNCTION
#define UWVM_MANAGED_FUNCTION(symbol,action,external,words,bytes) wrapper::native<&invoke_managed<::uwvm2::imported::ros::managed_lifecycle_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_MANAGED_LIFECYCLE_FUNCTIONS(UWVM_MANAGED_FUNCTION)
#undef UWVM_MANAGED_FUNCTION
#define UWVM_BUFFER_FUNCTION(symbol,action,external) wrapper::native<&invoke_buffer<::uwvm2::imported::ros::buffer_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_BUFFER_FUNCTIONS(UWVM_BUFFER_FUNCTION)
#undef UWVM_BUFFER_FUNCTION
#define UWVM_MODEL_FUNCTION(symbol,action,external) wrapper::native<&invoke_model<::uwvm2::imported::ros::model_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_MODEL_FUNCTIONS(UWVM_MODEL_FUNCTION)
            UWVM_ROS_REQUEST_FUNCTIONS(UWVM_MODEL_FUNCTION)
#undef UWVM_MODEL_FUNCTION
#define UWVM_SENSOR_FUNCTION(symbol,action,external) wrapper::native<&invoke_sensor<::uwvm2::imported::ros::sensor_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_SENSOR_FUNCTIONS(UWVM_SENSOR_FUNCTION)
#undef UWVM_SENSOR_FUNCTION
#define UWVM_DRIVER_FUNCTION(symbol,action,external) wrapper::native<&invoke_driver<model::drivers::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_DRIVER_FUNCTIONS(UWVM_DRIVER_FUNCTION)
#undef UWVM_DRIVER_FUNCTION
#define UWVM_DEVICE_FUNCTION(symbol,action,external) wrapper::native<&invoke_device<::uwvm2::imported::ros::scene::robot_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_DEVICE_FUNCTIONS(UWVM_DEVICE_FUNCTION)
            UWVM_ROS_CONTROL_FUNCTIONS(UWVM_DEVICE_FUNCTION)
#undef UWVM_DEVICE_FUNCTION
#define UWVM_MEDIA_FUNCTION(symbol,action,external) wrapper::native<&invoke_media<::uwvm2::imported::ros::scene::robot_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_MEDIA_FUNCTIONS(UWVM_MEDIA_FUNCTION)
#undef UWVM_MEDIA_FUNCTION
#define UWVM_BRIDGE_FUNCTION(symbol,action,external) wrapper::native<&invoke_bridge<::uwvm2::imported::ros::scene::robot_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_BRIDGE_FUNCTIONS(UWVM_BRIDGE_FUNCTION)
#undef UWVM_BRIDGE_FUNCTION
#define UWVM_PROPOSAL_FUNCTION(symbol,action,external) wrapper::native<&invoke_proposal<::uwvm2::imported::ros::proposal_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_PROPOSAL_FUNCTIONS(UWVM_PROPOSAL_FUNCTION)
#undef UWVM_PROPOSAL_FUNCTION
#define UWVM_TRANSFORM_FUNCTION(symbol,action,external) wrapper::native<&invoke_transform<::uwvm2::imported::ros::transform_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_TRANSFORM_FUNCTIONS(UWVM_TRANSFORM_FUNCTION)
#undef UWVM_TRANSFORM_FUNCTION
#define UWVM_LOG_DIAGNOSTIC_FUNCTION(symbol,action,external) wrapper::native<&invoke_log_diagnostic<::uwvm2::imported::ros::log_diagnostic_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_LOG_DIAGNOSTIC_FUNCTIONS(UWVM_LOG_DIAGNOSTIC_FUNCTION)
#undef UWVM_LOG_DIAGNOSTIC_FUNCTION
#define UWVM_RECORDING_FUNCTION(symbol,action,external) wrapper::native<&invoke_recording<::uwvm2::imported::ros::recording_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_RECORDING_FUNCTIONS(UWVM_RECORDING_FUNCTION)
#undef UWVM_RECORDING_FUNCTION
#define UWVM_COMMUNICATION_FUNCTION(symbol,action,external) wrapper::native<&invoke_communication<::uwvm2::imported::ros::communication_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_COMMUNICATION_FUNCTIONS(UWVM_COMMUNICATION_FUNCTION)
#undef UWVM_COMMUNICATION_FUNCTION
#define UWVM_COMMUNICATION_RECEIVE_FUNCTION(symbol,action,external) wrapper::native<&invoke_communication<::uwvm2::imported::ros::communication_receive_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_COMMUNICATION_RECEIVE_FUNCTIONS(UWVM_COMMUNICATION_RECEIVE_FUNCTION)
#undef UWVM_COMMUNICATION_RECEIVE_FUNCTION
#define UWVM_COMMUNICATION_CREATION_FUNCTION(symbol,action,external) wrapper::native<&invoke_communication<model::communication_open_dispatch::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_COMMUNICATION_CREATION_FUNCTIONS(UWVM_COMMUNICATION_CREATION_FUNCTION)
#undef UWVM_COMMUNICATION_CREATION_FUNCTION
#define UWVM_PARAMETER_FUNCTION(symbol,action,external) wrapper::native<&invoke_parameter<::uwvm2::imported::ros::parameter_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_PARAMETER_FUNCTIONS(UWVM_PARAMETER_FUNCTION)
#undef UWVM_PARAMETER_FUNCTION
#define UWVM_ACTION_FUNCTION(symbol,action,external) wrapper::native<&invoke_communication<::uwvm2::imported::ros::action_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_ACTION_FUNCTIONS(UWVM_ACTION_FUNCTION)
#undef UWVM_ACTION_FUNCTION
#define UWVM_GRAPH_FUNCTION(symbol,action,external) wrapper::native<&invoke_communication<::uwvm2::imported::ros::graph_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_GRAPH_FUNCTIONS(UWVM_GRAPH_FUNCTION)
#undef UWVM_GRAPH_FUNCTION
#define UWVM_EVENT_FUNCTION(symbol,action,external) wrapper::native<&invoke_communication<::uwvm2::imported::ros::event_contract::operation::action>,wrapper::symbol##_name>,
            UWVM_ROS_EVENT_FUNCTIONS(UWVM_EVENT_FUNCTION)
#undef UWVM_EVENT_FUNCTION
            wrapper::native<&abi,wrapper::abi_name>,wrapper::native<&open,wrapper::open_name>,wrapper::native<&close,wrapper::close_name>>;
    };
    /** @brief Check every policy catalogue entry against the actual linked wrapper tuple.
     * @details Each ASCII operation must appear exactly once. Changes to either the
     * host module or its configurable policy fail at compile time if the names drift.
     */
    template<typename... Functions>
    consteval bool matches_policy_catalog(::uwvm2::utils::container::tuple<Functions...>*)
    {
        auto const& names=model::callable_names;
        if(sizeof...(Functions)!=names.size())return false;
        std::array<bool,sizeof...(Functions)> seen{};
        auto take=[&](auto const& name)
        {
            std::array<char,64> ascii{};
            if(name.empty()||name.size()>ascii.size())return false;
            for(std::size_t i{};i<name.size();++i)
            {
                if(name[i]>127)return false;
                ascii[i]=static_cast<char>(name[i]);
            }
            std::string_view key{ascii.data(),name.size()};
            auto it=std::lower_bound(names.begin(),names.end(),key);
            if(it==names.end()||*it!=key)return false;
            auto i=static_cast<std::size_t>(it-names.begin());
            if(seen[i])return false;
            seen[i]=true;return true;
        };
        return (take(Functions::function_name)&&...);
    }
    static_assert(matches_policy_catalog(static_cast<local_imported_module_t::local_function_tuple*>(nullptr)));
    inline constexpr local_imported_module_t local_imported_module{};
    extern "C++"
    {
        /** @brief Mint loader identity only for the concrete scene module type. */
        class sealed_loader_factory final
        {
        public:
            static bool append() noexcept
            {return ::uwvm2::uwvm::runtime::full::builtin_ros_loader_identity::append_typed<local_imported_module_t>(::uwvm2::uwvm::runtime::full::builtin_ros_kind::scene);}
        };
        inline bool append_sealed_ros_builtin(local_imported_module_t const&) noexcept{return sealed_loader_factory::append();}
    }
    inline void retire(void const* instance,std::uint64_t generation,bool all) noexcept
    {try{if(all)configuration.retire_generation(generation);else configuration.retire_member({instance,generation});}catch(...){std::terminate();}}
    inline void snapshot(model::debug::request const& query,model::debug::view& output) noexcept
    {
        output={};output.selected=query.selected;
        if(!::uwvm2::utils::control::debug_permissions::inspection_allowed())
        {output.result=model::debug::status::forbidden;return;}
        try{configuration.inspect(query,output);}
        catch(...){output={};output.selected=query.selected;output.result=model::debug::status::resource_limit;}
    }
    struct lifecycle_registration
    {
        lifecycle_registration()
        {model::lifecycle::hook.store(&retire,std::memory_order_release);model::debug::snapshot_hook.store(&snapshot,std::memory_order_release);}
        ~lifecycle_registration()
        {model::lifecycle::hook.store(nullptr,std::memory_order_release);model::debug::snapshot_hook.store(nullptr,std::memory_order_release);}
    };
    inline lifecycle_registration lifecycle_hook;
}
#endif
