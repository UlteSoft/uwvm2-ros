/** @file
 * @brief Fixed-scope transform history with transactional bounded publication.
 * @details This private, externally serialized cache grants no authority. The
 * broker must hold the transform and bound clock authority leases across every
 * call, supply a sampled live clock, and separately admit static replacement.
 * All frame names and possible directed edges are deployment-owned. Acyclic
 * edge profiles permit time-dependent reparenting without any query escaping
 * the configured frame set. A different possible-cycle topology needs a new
 * explicit profile, not an unchecked guest mutation.
 *
 * Configure reserves two fast_io-backed sample banks before making the cache
 * usable. Each bank includes one maximum publication batch of scratch space;
 * data_bytes and workspace_bytes must each cover that bank. Runtime operations
 * allocate nothing. Publication stages, validates and prunes a complete batch,
 * then swaps banks once. Failed publication leaves history/generation unchanged.
 * The configured history keeps one predecessor for boundary interpolation;
 * insufficient capacity refuses the batch rather than shortening that history.
 * Query outputs remain unchanged on any error. There is no transport, thread,
 * timer, quota attenuation or autonomous watchdog in this numerical store.
 */
#pragma once
#include "transform_contract.h"
#include <uwvm2/utils/container/standard.h>
#include <array>
#include <span>
#include <algorithm>
#include <new>
#include <stdexcept>

namespace uwvm2::imported::ros::scene::transforms
{
    namespace contract=::uwvm2::imported::ros::transform_contract;
    using error=resources::error;
    inline constexpr std::size_t maximum_edges=512;
    struct edge
    {
        std::uint16_t parent{},child{};
        bool dynamic{},fixed{};
    };
    /** @brief All capacities and temporal allowances are explicit, initially zero.
     * @details Frames are unique sorted UTF-8 names. Edge indices address that
     * exact list. Future allowance limits sample time relative to the trusted
     * clock. History is relative to each child's latest retained sample. Linear
     * and angular speed limits reject velocity results; they never authorize a
     * physical command or repair undersampled motion.
     */
    struct cache_plan
    {
        std::array<contract::name,contract::maximum_frames> frames{};
        std::array<edge,maximum_edges> edges{};
        std::size_t frame_count{},edge_count{},max_samples{},samples_per_child{},max_batch{};
        std::uint64_t history_ns{},future_ns{},data_bytes{},workspace_bytes{},velocity_window_ns{};
        math::bounds geometry{};
        double linear_speed{},angular_speed{};
    };
    /** @brief Trusted live clock context; guest-supplied stamps cannot construct authority. */
    struct clock_context {std::uint64_t identity{},epoch{};std::int64_t now{};bool started{};};
    struct cache_usage {std::uint64_t generation{},epoch{},samples{},data_bytes{},workspace_bytes{};};

    class cache
    {
        struct sample
        {
            std::uint16_t parent{},child{};bool fixed{};
            std::int64_t time{};math::rigid pose{};
        };
        using bank=::uwvm2::utils::container::std_vector<sample>;
        cache_plan policy_{};
        bank current_,candidate_;
        std::uint64_t clock_{},epoch_{},generation_{};
        static std::uint64_t distance(std::int64_t later,std::int64_t earlier) noexcept
        {return static_cast<std::uint64_t>(later)-static_cast<std::uint64_t>(earlier);}
        static bool less(sample const& a,sample const& b) noexcept
        {return a.child<b.child||(a.child==b.child&&a.time<b.time);}
        static bool name_valid(contract::name const& name) noexcept
        {
            return name.size&&name.size<=contract::maximum_frame_bytes&&name.view().find('\0')==std::string_view::npos&&
                wire::utf8({reinterpret_cast<std::uint8_t const*>(name.bytes.data()),name.size});
        }
        static bool equivalent(math::rigid a,math::rigid b) noexcept
        {
            // Unit quaternions with opposite signs encode the same orientation.
            return a.translation==b.translation&&(a.rotation==b.rotation||
                a.rotation==math::quaternion{-b.rotation.x,-b.rotation.y,-b.rotation.z,-b.rotation.w});
        }
        std::size_t frame(contract::name const& name) const noexcept
        {
            if(!name_valid(name))return policy_.frame_count;
            auto begin=policy_.frames.begin(),end=begin+policy_.frame_count;
            auto item=std::lower_bound(begin,end,name,[](auto const& a,auto const& b){return a.view()<b.view();});
            return item!=end&&item->view()==name.view()?static_cast<std::size_t>(item-begin):policy_.frame_count;
        }
        bool permits(std::size_t parent,std::size_t child,bool fixed) const noexcept
        {
            for(std::size_t i{};i<policy_.edge_count;++i)
            {
                auto const& item=policy_.edges[i];
                if(item.parent==parent&&item.child==child)return fixed?item.fixed:item.dynamic;
            }
            return false;
        }
        error clock_check(clock_context clock) const noexcept
        {
            if(!generation_)return error::bad_handle;
            if(clock.identity!=clock_)return error::denied;
            if(!clock.started)return error::again;
            return clock.epoch==epoch_?error::success:error::conflict;
        }
        error stamp_check(contract::stamp stamp) const noexcept
        {return stamp.clock!=clock_?error::denied:stamp.epoch!=epoch_?error::conflict:error::success;}
        auto range(std::uint16_t child) const noexcept
        {
            auto begin=std::lower_bound(current_.begin(),current_.end(),child,[](auto const& a,auto b){return a.child<b;});
            auto end=std::upper_bound(begin,current_.end(),child,[](auto a,auto const& b){return a<b.child;});
            return std::pair{begin,end};
        }
        error selected(std::uint16_t child,std::int64_t time,bool latest,sample& out,bool& root) const noexcept
        {
            auto [begin,end]=range(child);root=begin==end;if(root)return error::success;
            if(begin->fixed){out=*begin;return error::success;}
            if(latest){out=*(end-1);return error::success;}
            if(time<begin->time||time>(end-1)->time)return error::again;
            // A predecessor retained for interpolation cannot extend the
            // declared history window itself.
            if(distance((end-1)->time,time)>policy_.history_ns)return error::again;
            auto upper=std::lower_bound(begin,end,time,[](auto const& a,auto b){return a.time<b;});
            if(upper!=end&&upper->time==time){out=*upper;return error::success;}
            if(upper==begin||upper==end)return error::again;
            auto lower=upper-1;out=*lower;
            // A changed parent is a discrete topology transition, never a
            // blend of coordinates belonging to unrelated parent frames.
            if(lower->parent!=upper->parent)return error::success;
            return math::interpolate(lower->pose,lower->time,upper->pose,upper->time,time,policy_.geometry,out.pose)?error::success:error::too_large;
        }
        struct step {std::uint16_t frame{};math::rigid pose{};std::int64_t edge_time{};bool dynamic{};};
        struct chain {std::array<step,contract::maximum_frames> nodes{};std::size_t size{};error stopped{error::success};};
        chain path(std::uint16_t start,std::int64_t time,bool latest) const noexcept
        {
            chain result;result.nodes[result.size++]={start,{},{},false};
            while(result.size<policy_.frame_count)
            {
                auto const& previous=result.nodes[result.size-1];sample item;bool root{};
                result.stopped=selected(previous.frame,time,latest,item,root);
                if(result.stopped!=error::success||root)return result;
                math::rigid pose;
                // Latest discovery walks topology only: these edge records
                // may have different times and must not be numerically mixed.
                if(!latest&&!math::compose(item.pose,previous.pose,policy_.geometry,pose)){result.stopped=error::too_large;return result;}
                result.nodes[result.size++]={item.parent,pose,item.time,!item.fixed};
            }
            return result;
        }
        static bool common(chain const& a,chain const& b,std::size_t& ai,std::size_t& bi) noexcept
        {
            for(std::size_t i{};i<a.size;++i)for(std::size_t j{};j<b.size;++j)
                if(a.nodes[i].frame==b.nodes[j].frame){ai=i;bi=j;return true;}
            return false;
        }
        error latest_time(std::uint16_t target,std::uint16_t source,clock_context clock,std::int64_t& time) const noexcept
        {
            auto a=path(source,0,true),b=path(target,0,true);std::size_t ai{},bi{};
            if(!common(a,b,ai,bi))return a.stopped!=error::success?a.stopped:b.stopped!=error::success?b.stopped:error::again;
            bool dynamic{};auto minimum=clock.now;
            auto visit=[&](chain const& p,std::size_t count)
            {
                for(std::size_t i=1;i<=count;++i)if(p.nodes[i].dynamic)
                {if(!dynamic||p.nodes[i].edge_time<minimum)minimum=p.nodes[i].edge_time;dynamic=true;}
            };
            visit(a,ai);visit(b,bi);time=minimum;return error::success;
        }
        error at(std::uint16_t target,std::uint16_t source,std::int64_t time,math::rigid& out,bool& fixed) const noexcept
        {
            auto a=path(source,time,false),b=path(target,time,false);std::size_t ai{},bi{};
            if(!common(a,b,ai,bi))return a.stopped!=error::success?a.stopped:b.stopped!=error::success?b.stopped:error::again;
            math::rigid inverse,value;
            if(!math::inverse(b.nodes[bi].pose,policy_.geometry,inverse)||!math::compose(inverse,a.nodes[ai].pose,policy_.geometry,value))return error::too_large;
            bool all_fixed=true;
            for(std::size_t i=1;i<=ai;++i)all_fixed=all_fixed&&!a.nodes[i].dynamic;
            for(std::size_t i=1;i<=bi;++i)all_fixed=all_fixed&&!b.nodes[i].dynamic;
            out=value;fixed=all_fixed;return error::success;
        }
    public:
        cache()=default;
        cache(cache const&)=delete;cache& operator=(cache const&)=delete;
        /** @brief Exact bytes reserved for each bank, checked before allocation. */
        static bool bank_bytes(cache_plan const& plan,std::uint64_t& bytes) noexcept
        {
            if(plan.max_batch>contract::maximum_batch||plan.max_samples>SIZE_MAX-plan.max_batch)return false;
            auto count=plan.max_samples+plan.max_batch;
            constexpr auto max_allocation{SIZE_MAX < static_cast<::std::uint64_t>(INT64_MAX) ? SIZE_MAX : static_cast<::std::uint64_t>(INT64_MAX)};
            if(count>max_allocation/sizeof(sample))return false;
            bytes=static_cast<std::uint64_t>(count)*sizeof(sample);return true;
        }
        /** @brief Validate a finite acyclic profile without allocating or activating it. */
        static bool valid(cache_plan const& plan) noexcept
        {
            std::uint64_t bytes{};
            if(!plan.frame_count||plan.frame_count>contract::maximum_frames||plan.edge_count>maximum_edges||
                !plan.max_samples||!plan.max_batch||!plan.samples_per_child||plan.samples_per_child>plan.max_samples||
                plan.history_ns>INT64_MAX||plan.future_ns>INT64_MAX||plan.velocity_window_ns>INT64_MAX||
                !math::valid(plan.geometry)||!std::isfinite(plan.linear_speed)||plan.linear_speed<0||
                !std::isfinite(plan.angular_speed)||plan.angular_speed<0||!bank_bytes(plan,bytes)||
                bytes>plan.data_bytes||bytes>plan.workspace_bytes||plan.data_bytes>INT64_MAX||plan.workspace_bytes>INT64_MAX)return false;
            for(std::size_t i{};i<plan.frame_count;++i)
                if(!name_valid(plan.frames[i])||(i&&plan.frames[i-1].view()>=plan.frames[i].view()))return false;
            std::array<std::uint16_t,contract::maximum_frames> indegree{};
            for(std::size_t i{};i<plan.edge_count;++i)
            {
                auto a=plan.edges[i];
                if(a.parent>=plan.frame_count||a.child>=plan.frame_count||a.parent==a.child||(!a.dynamic&&!a.fixed))return false;
                for(std::size_t j{};j<i;++j)if(plan.edges[j].parent==a.parent&&plan.edges[j].child==a.child)return false;
                ++indegree[a.child];
            }
            std::array<std::uint16_t,contract::maximum_frames> queue{};std::size_t begin{},end{};
            for(std::size_t i{};i<plan.frame_count;++i)if(!indegree[i])queue[end++]=static_cast<std::uint16_t>(i);
            while(begin!=end)
            {
                auto parent=queue[begin++];
                for(std::size_t i{};i<plan.edge_count;++i)if(plan.edges[i].parent==parent)
                {auto child=plan.edges[i].child;if(!--indegree[child])queue[end++]=child;}
            }
            return end==plan.frame_count;
        }
        /** @brief Reserve recoverably; allocation failure leaves this cache inert. */
        error configure(cache_plan const& plan,clock_context clock) noexcept
        {
            if(generation_)return error::conflict;
            if(!clock.identity||!valid(plan))return error::invalid;
            if(!clock.started)return error::again;
            try
            {
                bank data,scratch;auto count=plan.max_samples+plan.max_batch;
                data.reserve(count);scratch.reserve(count);
                if(data.capacity()>plan.data_bytes/sizeof(sample)||scratch.capacity()>plan.workspace_bytes/sizeof(sample))return error::too_large;
                policy_=plan;current_=std::move(data);candidate_=std::move(scratch);clock_=clock.identity;epoch_=clock.epoch;generation_=1;
                return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}
            catch(std::length_error const&){return error::too_large;}
        }
        /** @brief Cached metadata only; never sample a clock or expire anything. */
        cache_usage inspect() const noexcept
        {return {generation_,epoch_,current_.size(),current_.capacity()*sizeof(sample),candidate_.capacity()*sizeof(sample)};}
        /** @brief Atomic exact-clock publication; static changes require a separately admitted action.
         * @details Same child/time with different dynamic data is a conflict.
         * Static/dynamic mode changes require explicit clear. A repeated equal
         * static pose is harmless regardless of quaternion sign. Mutation-key
         * replay belongs to the enclosing session registry, not this cache.
         * A nonzero expected_generation applies a compare-and-swap condition
         * to the entire batch. Actual static replacement additionally requires
         * that condition; merely holding the extra action cannot overwrite an
         * intervening calibration update.
         */
        error publish(std::span<contract::transform const> input,clock_context clock,bool replace_static,std::uint64_t expected_generation=0) noexcept
        {
            auto checked=clock_check(clock);if(checked!=error::success)return checked;
            if(expected_generation&&expected_generation!=generation_)return error::conflict;
            if(input.empty()||input.size()>policy_.max_batch)return error::too_large;
            if(generation_==UINT64_MAX)return error::too_large;
            std::array<sample,contract::maximum_batch> batch{};
            for(std::size_t i{};i<input.size();++i)
            {
                auto const& value=input[i];auto p=frame(value.parent),c=frame(value.child);
                if(p==policy_.frame_count||c==policy_.frame_count||!permits(p,c,value.is_static))return error::denied;
                checked=stamp_check(value.time);if(checked!=error::success)return checked;
                if(value.time.nanoseconds>clock.now&&distance(value.time.nanoseconds,clock.now)>policy_.future_ns)return error::invalid;
                auto& item=batch[i];item.parent=static_cast<std::uint16_t>(p);item.child=static_cast<std::uint16_t>(c);
                item.fixed=value.is_static;item.time=value.is_static?0:value.time.nanoseconds;
                if(!math::normalize(value.pose,policy_.geometry,item.pose))return error::invalid;
            }
            // Admission is independent of record ordering within a batch.
            // No newly supplied sample may silently disappear below the final
            // history cutoff merely because a newer sibling was inserted later.
            std::array<std::int64_t,contract::maximum_frames> latest{};
            std::array<bool,contract::maximum_frames> has_latest{};
            auto observe=[&](sample const& item)
            {
                if(!item.fixed&&(!has_latest[item.child]||item.time>latest[item.child]))
                {latest[item.child]=item.time;has_latest[item.child]=true;}
            };
            for(auto const& item:current_)observe(item);
            for(std::size_t i{};i<input.size();++i)observe(batch[i]);
            for(std::size_t i{};i<input.size();++i)
                if(!batch[i].fixed&&distance(latest[batch[i].child],batch[i].time)>policy_.history_ns)return error::again;
            candidate_.assign(current_.begin(),current_.end());
            for(std::size_t i{};i<input.size();++i)
            {
                auto const& item=batch[i];
                auto first=std::lower_bound(candidate_.begin(),candidate_.end(),item.child,[](auto const& a,auto b){return a.child<b;});
                auto last=std::upper_bound(first,candidate_.end(),item.child,[](auto a,auto const& b){return a<b.child;});
                if(first!=last)
                {
                    if(first->fixed!=item.fixed)return error::conflict;
                    if(item.fixed)
                    {
                        if(first->parent==item.parent&&equivalent(first->pose,item.pose))continue;
                        if(!replace_static)return error::denied;
                        if(!expected_generation)return error::conflict;
                        *first=item;continue;
                    }
                }
                auto where=std::lower_bound(first,last,item,less);
                if(where!=last&&where->time==item.time)
                {
                    if(where->parent!=item.parent||!equivalent(where->pose,item.pose))return error::conflict;
                    continue;
                }
                // Both banks were reserved for max_samples + max_batch. No
                // allocation, reentrancy or callback occurs during staging.
                candidate_.insert(where,item);
            }
            std::size_t read{},written{};
            while(read<candidate_.size())
            {
                auto end=read+1;while(end<candidate_.size()&&candidate_[end].child==candidate_[read].child)++end;
                auto keep=read;
                if(!candidate_[read].fixed)
                {
                    auto latest=candidate_[end-1].time;
                    while(keep<end&&distance(latest,candidate_[keep].time)>policy_.history_ns)++keep;
                    if(keep>read)--keep; // exactly one interpolation predecessor
                }
                if(end-keep>policy_.samples_per_child)return error::too_large;
                for(auto i=keep;i<end;++i)candidate_[written++]=candidate_[i];
                read=end;
            }
            candidate_.resize(written);
            if(written>policy_.max_samples)return error::too_large;
            current_.swap(candidate_);candidate_.clear();++generation_;return error::success;
        }
        /** @brief Clear all samples and explicitly acknowledge a live clock epoch. */
        error clear(clock_context clock,std::uint64_t expected) noexcept
        {
            if(!generation_)return error::bad_handle;
            if(clock.identity!=clock_)return error::denied;
            if(!clock.started)return error::again;
            if(expected!=generation_)return error::conflict;
            if(generation_==UINT64_MAX)return error::too_large;
            current_.clear();candidate_.clear();epoch_=clock.epoch;++generation_;return error::success;
        }
        /** @brief Resolve only scoped frames; exact zero time is never a latest sentinel.
         * @details A fixed frame joins independently timed source and target
         * legs. Without it, explicit times must agree. Latest selects the latest
         * common dynamic time, then performs ordinary checked lookup; missing
         * history/topology produces again, never extrapolation. Root identity
         * lookups are allowed only for names already in the profile.
         */
        error lookup(contract::query const& query,clock_context clock,contract::transform& out) const noexcept
        {
            auto checked=clock_check(clock);if(checked!=error::success)return checked;
            auto target=frame(query.target),source=frame(query.source),fixed=frame(query.fixed);
            if(target==policy_.frame_count||source==policy_.frame_count||(query.has_fixed&&fixed==policy_.frame_count))return error::denied;
            if(query.target_stamped&&(checked=stamp_check(query.target_time))!=error::success)return checked;
            if(query.source_stamped&&(checked=stamp_check(query.source_time))!=error::success)return checked;
            std::int64_t target_time=query.target_time.nanoseconds,source_time=query.source_time.nanoseconds;
            math::rigid value;bool all_fixed{};
            if(!query.has_fixed)
            {
                if(query.target_stamped&&query.source_stamped&&target_time!=source_time)return error::invalid;
                if(query.source_stamped)target_time=source_time;
                else if(!query.target_stamped&&(checked=latest_time(target,source,clock,target_time))!=error::success)return checked;
                checked=at(target,source,target_time,value,all_fixed);if(checked!=error::success)return checked;
            }
            else
            {
                if(!query.target_stamped&&(checked=latest_time(target,fixed,clock,target_time))!=error::success)return checked;
                if(!query.source_stamped&&(checked=latest_time(fixed,source,clock,source_time))!=error::success)return checked;
                math::rigid target_leg,source_leg;bool target_fixed{},source_fixed{};
                checked=at(target,fixed,target_time,target_leg,target_fixed);if(checked!=error::success)return checked;
                checked=at(fixed,source,source_time,source_leg,source_fixed);if(checked!=error::success)return checked;
                if(!math::compose(target_leg,source_leg,policy_.geometry,value))return error::too_large;
                all_fixed=target_fixed&&source_fixed;
            }
            out={query.target,query.source,{clock_,epoch_,target_time},value,all_fixed};return error::success;
        }
        /** @brief Full backward-window velocity, with explicit point and reference basis.
         * @details The requested time is the window end. Never clip a window
         * to cached data. Epoch mismatch, unknown frames and missing endpoints
         * all refuse. Angular principal-angle ambiguity remains a sampling
         * policy obligation; this method does not certify motion safety.
         */
        error velocity(contract::velocity_query const& query,clock_context clock,math::velocity& out) const noexcept
        {
            auto checked=clock_check(clock);if(checked!=error::success)return checked;
            if((checked=stamp_check(query.time))!=error::success)return checked;
            if(!query.averaging_ns||query.averaging_ns>policy_.velocity_window_ns||query.averaging_ns>INT64_MAX)return error::too_large;
            auto interval=static_cast<std::int64_t>(query.averaging_ns);
            if(query.time.nanoseconds<INT64_MIN+interval)return error::invalid;
            auto tracking=frame(query.tracking),observation=frame(query.observation),reference=frame(query.reference),point=frame(query.point_frame);
            if(tracking==policy_.frame_count||observation==policy_.frame_count||reference==policy_.frame_count||point==policy_.frame_count)return error::denied;
            math::rigid first,last,point_pose,reference_pose;bool ignored{};
            checked=at(observation,tracking,query.time.nanoseconds-interval,first,ignored);if(checked!=error::success)return checked;
            checked=at(observation,tracking,query.time.nanoseconds,last,ignored);if(checked!=error::success)return checked;
            checked=at(observation,point,query.time.nanoseconds,point_pose,ignored);if(checked!=error::success)return checked;
            checked=at(reference,observation,query.time.nanoseconds,reference_pose,ignored);if(checked!=error::success)return checked;
            if(!math::finite(query.point)||math::norm(query.point)>policy_.geometry.translation)return error::invalid;
            auto position=math::add(point_pose.translation,math::rotate_unit(point_pose.rotation,query.point));
            math::velocity observed,value;
            if(!math::differentiate(first,last,query.averaging_ns,policy_.geometry,policy_.linear_speed,policy_.angular_speed,observed)||
                !math::reference(observed,position,last.translation,reference_pose.rotation,policy_.geometry,policy_.linear_speed,policy_.angular_speed,value))return error::too_large;
            out=value;return error::success;
        }
    };
}
