/** @file
 * @brief Diagnostic native bridge from the unchanged guest adapter to scene policy.
 * @details Native caller pointers are trusted fixture storage, not guest memory.
 * This deliberately omits VM entry/FP/memory checking and host request staging;
 * the real authority, capacity, mutation ledger and readback broker remain.
 * Setup runs once before main. No retries/queries are added to timed workloads.
 * This diagnostic is additional to, never a replacement for, the vector-copy
 * native reference and does not establish native ROS or device performance.
 */
#include <uwvm2/imported/ros/scene/broker.h>
#include <cassert>
#include <cstring>
#include <string_view>

#ifndef OUTPUT_CHUNK
#define OUTPUT_CHUNK 4096
#endif
namespace native_scene
{
    namespace s=uwvm2::imported::ros::scene;
    namespace output=uwvm2::imported::ros::output_contract;
    struct fixture
    {
        s::broker broker;int identity{};s::caller who{&identity,1};s::handle capability{};
        fixture()
        {
            assert(broker.permissions.expose("capability.open",s::exposure::allow)==s::error::success);
            s::grant grant;grant.alias="xmlout";grant.resource="serialized-xml";grant.kind=s::purpose::cache;grant.binding="volatile-output";
            grant.max_handles=1;grant.actions={"artifact.begin","artifact.append","artifact.commit","artifact.remove","artifact.read"};
            for(auto const& action:grant.actions)assert(broker.permissions.expose(action,s::exposure::allow)==s::error::success);
            assert(broker.permissions.configure(grant)==s::error::success);
            assert(broker.output_store.bind("xmlout","serialized-xml",{65536,1,65536,OUTPUT_CHUNK})==s::error::success);
            assert(broker.permissions.enroll(who)==s::error::success);
        }
    };
    static fixture state;
    static std::uint32_t invoke(output::operation operation,std::uint64_t cap,std::uint32_t memory,
        std::uint64_t input,std::uint64_t size,std::uint64_t output_address,std::uint64_t capacity,std::uint64_t length_address) noexcept
    {
        if(memory||size>SIZE_MAX||capacity>SIZE_MAX||!input||!output_address||!length_address)return 22;
        try
        {
            auto deliver=[&](s::bytes data)
            {
                if(data.size()>capacity)return false;
                auto length=static_cast<std::uint64_t>(data.size());std::memcpy(reinterpret_cast<void*>(length_address),&length,8);
                if(!data.empty())std::memcpy(reinterpret_cast<void*>(output_address),data.data(),data.size());return true;
            };
            std::size_t required{};
            auto result=state.broker.invoke_output(state.who,operation,cap,
                {reinterpret_cast<std::uint8_t const*>(input),static_cast<std::size_t>(size)},static_cast<std::size_t>(capacity),required,std::ref(deliver));
            return static_cast<unsigned>(result);
        }
        catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
    }
}
extern "C" std::uint32_t output_capability_open(std::uint32_t memory,std::uint64_t alias,std::uint64_t size,std::uint64_t output) noexcept
{
    using namespace native_scene;
    if(memory||!alias||!output||size!=6||std::string_view{reinterpret_cast<char const*>(alias),6}!="xmlout")return 22;
    try
    {
        auto result=state.broker.permissions.open(state.who,"xmlout",state.capability);
        if(result==s::error::success)std::memcpy(reinterpret_cast<void*>(output),&state.capability,8);
        return static_cast<unsigned>(result);
    }
    catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
}
#define NATIVE_SCENE_OUTPUT(symbol,action) \
extern "C" std::uint32_t symbol(std::uint64_t cap,std::uint32_t memory,std::uint64_t input,std::uint64_t size, \
    std::uint64_t output,std::uint64_t capacity,std::uint64_t length) noexcept \
{return native_scene::invoke(native_scene::output::operation::action,cap,memory,input,size,output,capacity,length);}
NATIVE_SCENE_OUTPUT(output_begin,begin)
NATIVE_SCENE_OUTPUT(output_append,append)
NATIVE_SCENE_OUTPUT(output_commit,commit)
NATIVE_SCENE_OUTPUT(output_remove,remove)
NATIVE_SCENE_OUTPUT(output_status,status)
NATIVE_SCENE_OUTPUT(output_receipt,receipt)
#undef NATIVE_SCENE_OUTPUT
extern "C" std::uint32_t output_read(std::uint64_t cap,std::uint32_t memory,std::uint64_t input,std::uint64_t size,
    std::uint64_t output,std::uint64_t capacity,std::uint64_t length_address) noexcept
{
    using namespace native_scene;
    if(memory||!input||!length_address||size>SIZE_MAX||capacity>SIZE_MAX||(!output&&capacity))return 22;
    try
    {
        auto deliver=[&](s::bytes data)
        {
            if(data.size()>capacity)return false;
            auto length=static_cast<std::uint64_t>(data.size());std::memcpy(reinterpret_cast<void*>(length_address),&length,8);
            if(!data.empty())std::memcpy(reinterpret_cast<void*>(output),data.data(),data.size());return true;
        };
        std::size_t required{};
        auto result=state.broker.invoke_artifact(state.who,uwvm2::imported::ros::artifact_contract::operation::read,cap,
            {reinterpret_cast<std::uint8_t const*>(input),static_cast<std::size_t>(size)},static_cast<std::size_t>(capacity),required,std::ref(deliver));
        return static_cast<unsigned>(result);
    }
    catch(std::bad_alloc const&){return 12;}catch(std::length_error const&){return 90;}
}
