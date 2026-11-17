/** @file @brief Finite canonical TF policy shared by production-entry fixtures. */
#pragma once
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
namespace transform_fixture
{
    namespace s=uwvm2::imported::ros::scene;
    namespace t=s::transforms;
    namespace c=uwvm2::imported::ros::transform_contract;
    using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
    template<typename Fn> bytes encode(Fn fn)
    {std::array<std::uint8_t,8192> storage{};s::wire::writer out{storage};fn(out);return {out.written().begin(),out.written().end()};}
    inline bytes definition()
    {
        return encode([](auto& out)
        {
            assert(out.text(t::definition_magic)&&out.text("frames")&&out.u64(3)&&out.text("base")&&out.text("camera")&&out.text("map")&&out.u64(2));
            assert(out.u64(2)&&out.u64(0)&&out.boolean(true)&&out.boolean(false));
            assert(out.u64(0)&&out.u64(1)&&out.boolean(false)&&out.boolean(true));
            assert(out.f64(1000)&&out.f64(1e-12)&&out.f64(100)&&out.f64(100));
        });
    }
    inline s::buffers::profile profile()
    {s::buffers::profile value;assert(value.identity.assign("frames"));value.sha256=s::buffers::hash(definition());return value;}
    inline constexpr std::array<std::uint64_t,t::limit_names.size()> limits{16,8,8,10000000000,0,1000000000,4,1000000000,2,1000000,1000000,1000000,65536};
    inline constexpr t::budget budget{2,8,1000000,1000000,1000000,65536};
    inline s::grant grant(std::string_view alias="frames")
    {
        s::grant value;value.alias=alias;value.resource="robot.frames";value.kind=s::purpose::transform;value.binding="local-transform";
        value.max_resources=2;for(auto action:c::operations)value.actions.emplace(action.name);
        for(auto action:{"transform.static.replace","resource.close","request.describe","request.release"})value.actions.emplace(action);
        return value;
    }
    inline void environment(s::deployment& value,std::string_view env="robot",std::string_view module="main")
    {
        assert(value.create(uwvm2::utils::container::std_string(env))==s::error::success&&value.bind(env,uwvm2::utils::container::std_string(module))==s::error::success);
        for(auto action:c::operations)assert(value.expose(env,uwvm2::utils::container::std_string(action.name),s::exposure::allow)==s::error::success);
        for(auto action:{"capability.open","capability.close","resource.close","request.describe","request.release","clock.create","clock.read"})
            assert(value.expose(env,action,s::exposure::allow)==s::error::success);
    }
    inline void configure(s::deployment& value,std::string_view alias="frames",std::string_view env="robot",std::size_t skip=SIZE_MAX)
    {
        assert(value.allow(env,grant(alias))==s::error::success&&value.transform_profile(env,uwvm2::utils::container::std_string(alias),profile())==s::error::success);
        for(std::size_t i{};i<limits.size();++i)if(i!=skip)assert(value.transform_limit(env,alias,t::limit_names[i],limits[i])==s::error::success);
        assert(value.transform_definition(env,alias,definition())==s::error::success);
    }
    inline void clock(s::deployment& value,std::string_view env="robot")
    {
        s::grant timing;timing.alias="clock";timing.resource="robot.clock";timing.kind=s::purpose::ros_clock;timing.binding="local";
        timing.actions={"clock.create","clock.read","resource.close"};timing.max_resources=1;
        assert(value.allow(env,timing)==s::error::success);
    }
}
