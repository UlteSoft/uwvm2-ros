/** @file @brief Complete closed ROS Bool recording definition used by deployment and actual CLI tests. */
#pragma once
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
namespace recording_fixture
{
    namespace s=uwvm2::imported::ros::scene;
    using s::error;using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
    template<typename Function> bytes encode(Function&& fn)
    {std::array<std::uint8_t,8192> data{};s::wire::writer out{data};fn(out);return {out.written().begin(),out.written().end()};}
    inline s::buffers::profile profile(std::string_view name)
    {s::buffers::profile p;assert(p.identity.assign(name));p.sha256[0]=7;return p;}
    inline auto buffer_profile(){return profile("serialized.bool");}
    inline auto replay_profile(){return profile("recorded.bool.replay");}
    inline auto definition_body()
    {
        return encode([](auto& out)
        {
            auto p=buffer_profile();assert(out.text("ros1")&&out.u64(1)&&out.u64(1)&&out.u64(1)&&out.text("/state")&&
                out.text("ros1-std-bool")&&out.text("std_msgs/Bool")&&out.text("ros1msg")&&out.text("ros1")&&out.text("")&&
                out.blob(s::recording::mcap::raw("bool data\n"))&&out.u64(1)&&out.text("input")&&out.text(p.identity.view())&&out.blob(p.sha256));
        });
    }
    inline auto recording_profile()
    {auto p=profile("recorded.bool");p.sha256=s::buffers::hash(definition_body());return p;}
    inline auto definition()
    {
        auto p=recording_profile();auto result=encode([&](auto& out){assert(out.text(s::recording::definition_magic)&&out.text(p.identity.view())&&out.blob(p.sha256));});
        auto body=definition_body();result.insert(result.end(),body.begin(),body.end());return result;
    }
    inline uwvm2::utils::container::std_string hex(s::bytes input)
    {constexpr char digits[]="0123456789abcdef";uwvm2::utils::container::std_string out;out.reserve(input.size()*2);for(auto x:input){out.push_back(digits[x>>4]);out.push_back(digits[x&15]);}return out;}
    inline constexpr std::array<std::uint64_t,9> limits{2,1024,4,2,512,4,131072,8192,4};
    inline constexpr std::array<std::uint64_t,5> budgets{1,1,1024*1024,32768,8};
    inline s::grant grant(s::purpose kind,bool export_data=true)
    {
        s::grant value;value.kind=kind;value.resource="bool.records";
        if(kind==s::purpose::recording)
        {value.alias="recording";value.binding="private-recording";value.actions={"recording.open","recording.append","recording.checkpoint","recording.stop","resource.close","resource.observe"};}
        else if(kind==s::purpose::replay)
        {value.alias="replay";value.binding="private-replay";value.actions={"replay.open","replay.read","replay.seek","resource.close"};}
        else
        {value.alias="input";value.binding="sealed-buffer";value.actions={"buffer.create","buffer.write","buffer.seal","buffer.read","resource.close"};}
        if(export_data&&kind!=s::purpose::replay)value.actions.emplace("data.export");
        return value;
    }
    inline s::buffers::plan buffer_plan()
    {
        s::buffers::plan p;p.schema=buffer_profile();p.objects=8;p.bytes=16;p.object_bytes=4;p.transfer_bytes=4;
        p.workspace_bytes=32;p.leases=128;p.elements=4;p.element_mask=p.order_mask=p.color_mask=1;p.rank=1;return p;
    }
    inline void environment(s::deployment& d,bool expose=true)
    {
        assert(d.create("robot")==error::success&&d.bind("robot","main")==error::success);
        if(expose)
        {
            for(auto action:{"capability.open","resource.close","buffer.read"})assert(d.expose("robot",action,s::exposure::allow)==error::success);
            for(auto const& action:uwvm2::imported::ros::recording_contract::operations)assert(d.expose("robot",uwvm2::utils::container::std_string(action.name),s::exposure::allow)==error::success);
        }
    }
    inline void budget(s::deployment& d,std::size_t skip=5)
    {for(std::size_t i{};i<budgets.size();++i)if(i!=skip)assert(d.recording_budget_limit("robot",s::recording_budget_configuration::names[i],budgets[i])==error::success);}
    inline void configure(s::deployment& d,bool source_export=true,bool recording_export=true,bool include_definition=true,std::size_t skip=9)
    {
        assert(d.allow("robot",grant(s::purpose::recording,recording_export))==error::success&&d.allow("robot",grant(s::purpose::replay))==error::success&&
               d.allow("robot",grant(s::purpose::buffer,source_export))==error::success);
        assert(d.buffer_budget("robot",16,8,32,128)==error::success&&d.buffer_plan("robot","input",buffer_plan())==error::success);
        assert(d.recording_profile("robot","recording",recording_profile())==error::success);
        for(std::size_t i{};i<limits.size();++i)if(i!=skip)assert(d.recording_limit("robot","recording",s::recording::configuration::limit_names[i],limits[i])==error::success);
        if(include_definition)assert(d.recording_definition("robot","recording",definition())==error::success);
        assert(d.replay_plan("robot","replay","recording",replay_profile(),2)==error::success);
    }
}
