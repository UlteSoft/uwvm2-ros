/** @file
 * @brief Explicit transform definitions and finite deployment budgets without I/O.
 * @details The caller supplies already owned configuration bytes. The selected
 * profile SHA256 covers the complete canonical definition, including its
 * identity, frame/edge rules and physical bounds; the definition does not
 * contain its own digest. No filename, socket, ROS node or native symbol is
 * interpreted. Limits are individually named, mandatory and assigned once.
 * A successful freeze makes all data immutable. Parsing/failure grants nothing.
 */
#pragma once
#include "transform_registry.h"

namespace uwvm2::imported::ros::scene::transforms
{
    inline constexpr std::string_view definition_magic="uwvm.ros.transform.definition";
    inline constexpr std::size_t maximum_definition_bytes=65536;
    inline constexpr std::array limit_names{
        std::string_view{"cache.samples"},std::string_view{"cache.samples-per-child"},std::string_view{"publish.batch"},
        std::string_view{"cache.history-ns"},std::string_view{"publish.future-ns"},std::string_view{"velocity.window-ns"},
        std::string_view{"session.requests"},std::string_view{"query.deadline-ns"},std::string_view{"sessions"},
        std::string_view{"budget.data-bytes"},std::string_view{"budget.workspace-bytes"},std::string_view{"budget.request-bytes"},
        std::string_view{"budget.carrier-bytes"}};
    /** @brief Aggregate reservations across configured aliases, with zero authority by default. */
    struct budget
    {
        std::uint64_t sessions{},queries{},data_bytes{},workspace_bytes{},request_bytes{},carrier_bytes{};
        friend bool operator==(budget const&,budget const&)=default;
    };
    inline bool accumulate(budget const& maximum,plan const& value,budget& total) noexcept
    {
        if(!valid(value)||value.sessions==UINT64_MAX||(value.session.requests&&(value.sessions>UINT64_MAX/value.session.requests)))return false;
        budget next=total;
        auto add=[](std::uint64_t cap,std::uint64_t n,std::uint64_t& sum)
        {if(cap>INT64_MAX||sum>cap||n>cap-sum)return false;sum+=n;return true;};
        if(!add(maximum.sessions,value.sessions,next.sessions)||!add(maximum.queries,value.sessions*value.session.requests,next.queries)||
            !add(maximum.data_bytes,value.data_bytes,next.data_bytes)||!add(maximum.workspace_bytes,value.workspace_bytes,next.workspace_bytes)||
            !add(maximum.request_bytes,value.request_bytes,next.request_bytes)||!add(maximum.carrier_bytes,value.carrier_bytes,next.carrier_bytes))return false;
        total=next;return true;
    }
    class configuration
    {
        plan selected_{};
        std::array<bool,limit_names.size()> assigned_{};
        bool profile_set_{},definition_set_{},frozen_{};
    public:
        bool frozen() const noexcept{return frozen_;}
        /** @brief Permit trusted file loading only while the selected definition is missing. */
        bool definition_pending() const noexcept{return profile_set_&&!definition_set_&&!frozen_;}
        plan const* limits() const noexcept{return frozen_?&selected_:nullptr;}
        /** @brief Select the exact logical definition and digest; neither is native authority. */
        error profile(buffers::profile value) noexcept
        {
            if(frozen_)return error::denied;
            if(profile_set_)return error::conflict;
            if(!value.identity.size||value.identity.size>buffers::maximum_identity||value.identity.view().find('\0')!=std::string_view::npos||
                !wire::utf8({reinterpret_cast<std::uint8_t const*>(value.identity.bytes.data()),value.identity.size}))return error::invalid;
            selected_.session.profile=value;profile_set_=true;return error::success;
        }
        /** @brief Parse exact ordered frames/edges and four physical bounds, with no allocation.
         * @details Wire fields: magic text, identity text, frame count and text
         * names, edge count and (parent index, child index, dynamic, static),
         * translation metres, quaternion tolerance, linear m/s, angular rad/s.
         * Counts/booleans are canonical u64; physical bounds are finite f64.
         * Topology validity and all storage requirements are checked at freeze.
         */
        error definition(wire::bytes bytes) noexcept
        {
            if(frozen_)return error::denied;
            if(!profile_set_)return error::denied;
            if(definition_set_)return error::conflict;
            if(bytes.size()>maximum_definition_bytes)return error::too_large;
            if(buffers::hash(bytes)!=selected_.session.profile.sha256)return error::conflict;
            wire::reader input{bytes};std::string_view magic,identity;std::uint64_t frames{},edges{};
            cache_plan value;
            if(!input.text(magic)||magic!=definition_magic||!input.text(identity)||identity!=selected_.session.profile.identity.view()||
                !input.u64(frames)||!frames||frames>contract::maximum_frames)return error::invalid;
            value.frame_count=static_cast<std::size_t>(frames);
            for(std::size_t i{};i<value.frame_count;++i)if(!contract::read_name(input,value.frames[i]))return error::invalid;
            if(!input.u64(edges)||edges>maximum_edges)return error::invalid;
            value.edge_count=static_cast<std::size_t>(edges);
            for(std::size_t i{};i<value.edge_count;++i)
            {
                std::uint64_t parent{},child{};auto& edge=value.edges[i];
                if(!input.u64(parent)||!input.u64(child)||parent>=frames||child>=frames||!input.boolean(edge.dynamic)||!input.boolean(edge.fixed))return error::invalid;
                edge.parent=static_cast<std::uint16_t>(parent);edge.child=static_cast<std::uint16_t>(child);
            }
            if(!input.f64(value.geometry.translation)||!input.f64(value.geometry.quaternion_tolerance)||!input.f64(value.linear_speed)||!input.f64(value.angular_speed)||
                !input.done()||!math::valid(value.geometry)||!std::isfinite(value.linear_speed)||value.linear_speed<0||!std::isfinite(value.angular_speed)||value.angular_speed<0)return error::invalid;
            // Definition and limits may be entered in either order; copying
            // graph data must not erase previously assigned capacity fields.
            auto& history=selected_.session.history;
            history.frames=value.frames;history.edges=value.edges;history.frame_count=value.frame_count;history.edge_count=value.edge_count;
            history.geometry=value.geometry;history.linear_speed=value.linear_speed;history.angular_speed=value.angular_speed;
            definition_set_=true;return error::success;
        }
        /** @brief Set one finite named bound; zero is literal and never means unlimited. */
        error limit(std::string_view name,std::uint64_t value) noexcept
        {
            if(frozen_)return error::denied;
            auto found=std::find(limit_names.begin(),limit_names.end(),name);if(found==limit_names.end())return error::invalid;
            auto index=static_cast<std::size_t>(found-limit_names.begin());if(assigned_[index])return error::conflict;
            if(value>INT64_MAX||((index==0||index==1||index==2||index==6)&&value>SIZE_MAX))return error::too_large;
            auto& p=selected_.session.history;
            switch(index)
            {
            case 0:p.max_samples=static_cast<std::size_t>(value);break;
            case 1:p.samples_per_child=static_cast<std::size_t>(value);break;
            case 2:p.max_batch=static_cast<std::size_t>(value);break;
            case 3:p.history_ns=value;break;
            case 4:p.future_ns=value;break;
            case 5:p.velocity_window_ns=value;break;
            case 6:selected_.session.requests=static_cast<std::size_t>(value);break;
            case 7:selected_.session.deadline_ns=value;break;
            case 8:selected_.sessions=value;break;
            case 9:selected_.data_bytes=value;break;
            case 10:selected_.workspace_bytes=value;break;
            case 11:selected_.request_bytes=value;break;
            case 12:selected_.carrier_bytes=value;break;
            default:return error::invalid;
            }
            assigned_[index]=true;return error::success;
        }
        /** @brief Require a complete definition and every explicit limit before binding. */
        error freeze() noexcept
        {
            if(frozen_)return error::conflict;
            if(!profile_set_||!definition_set_||std::find(assigned_.begin(),assigned_.end(),false)!=assigned_.end())return error::invalid;
            auto value=selected_;std::uint64_t bytes{};
            if(!cache::bank_bytes(value.session.history,bytes))return error::too_large;
            value.session.history.data_bytes=value.session.history.workspace_bytes=bytes;
            if(!session::request_storage(value.session,bytes))return error::too_large;
            value.session.request_bytes=bytes;if(!valid(value))return error::invalid;
            selected_=value;frozen_=true;return error::success;
        }
    };
}
