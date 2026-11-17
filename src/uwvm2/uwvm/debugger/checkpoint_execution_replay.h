// Licensed under APL-2.0 (see LICENSE).
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
# include "checkpoint_replay_memory.h"
# include <uwvm2/runtime/diagnostic/snapshot_wasip1.h>
# include <uwvm2/runtime/diagnostic/snapshot_ros.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::checkpoint::execution_replay
{
    // Versioned runtime journal. These event records carry DATA only. The
    // execution adapter must independently resolve the current native provider,
    // current source, caller environment and actual memory owners.
    inline constexpr ::std::uint16_t environment_kind=15u, import_memtag_report_kind=14u, import_finish_kind=13u, import_memory_kind=12u, import_begin_kind=11u, wait_ready_kind=10u, wait_endpoint_kind=9u, instruction_kind=8u, import_kind=7u, endpoint_kind=6u, entropy_kind=5u, tags_kind=4u, thread_exit_kind=3u, wait_begin_kind=2u, wait_end_kind=1u;
    inline constexpr ::std::uint64_t schema=0x32504c5258455755ull; // UWEXRLP2
    struct decoded
    {
        state origin{};
        ::uwvm2::utils::container::std_vector<object> events{};
        ::uwvm2::utils::container::std_vector<::std::uint64_t> times{};
        ::std::uint64_t logical_thread{};
        ::std::size_t thread_count{};
    };
    [[nodiscard]] inline bool valid_wait_guard(object const& event,object const& memory,unsigned width,unsigned bits) noexcept
    {
        if(event.bytes.size()!=32u)return false;
        auto guard=[&](unsigned n) noexcept
        {
            ::std::uint64_t value{};
            for(unsigned byte{};byte<8u;++byte)value|=::std::uint64_t{::std::to_integer<unsigned>(event.bytes[n*8u+byte])}<<(byte*8u);
            return value;
        };
        auto const length=guard(0),address=guard(1),value=guard(2),tag=guard(3);
        return !(length%65536u || length/65536u<memory.words[1] ||
            (memory.words[2] && length/65536u>memory.words[3]) ||
            (address&(width-1u)) || address>length || width>length-address ||
            (width==4u && value>UINT32_MAX) || (tag&~::std::uint64_t{0xff01u}) ||
            (!(tag&1u) && tag) || (bits==32u && (tag>>8u)>15u));
    }
    [[nodiscard]] inline error supported_origin(state const& graph,::std::uint64_t& thread) noexcept
    {
        thread=0;::std::uint64_t count{};
        for(auto const& item:graph.objects)
        {
            if(item.kind==object_kind::thread)
            {
                if((item.flags!=0u && item.flags!=2u) || item.words[0]!=count+1u || item.words[1]!=count)
                    return error::unavailable_capability;
                if(!thread)thread=item.words[0];++count;
            }
        }
        return count && graph.next_logical_thread==count+1u?error::none:error::unavailable_capability;
    }

    namespace details
    {
    // The prefix form is only a detached observation. It must never be supplied
    // to an execution adapter as a complete replay plan.
    [[nodiscard]] inline error decode_journal(state const& graph,decoded& output,limits const& budget,bool pending_prefix)
    {
        auto valid=validate_graph(graph,budget);if(valid!=error::none)return valid;
        if(graph.replay_event_cursor || graph.logical_instruction)return error::invalid_shape;
        decoded candidate{};candidate.origin=graph;
        auto first=::std::find_if(candidate.origin.objects.begin(),candidate.origin.objects.end(),
            [](auto const& item){return item.kind==object_kind::event;});
        if(first==candidate.origin.objects.end())return error::non_replayable_import;
        candidate.events.assign(first,candidate.origin.objects.end());candidate.origin.objects.erase(first,candidate.origin.objects.end());
        valid=validate_graph(candidate.origin,budget);if(valid!=error::none)return valid;
        valid=supported_origin(candidate.origin,candidate.logical_thread);if(valid!=error::none)return valid;
        candidate.thread_count=static_cast<::std::size_t>(candidate.origin.next_logical_thread-1u);
        ::uwvm2::utils::container::std_vector<bool> live_instruction(candidate.thread_count),ended(candidate.thread_count);
        ::uwvm2::utils::container::std_vector<::std::uint64_t> waiting(candidate.thread_count);
        for(auto const& item:candidate.origin.objects)if(item.kind==object_kind::thread && item.flags==2u)
            waiting[item.words[0]-1u]=UINT64_MAX;
        ::uwvm2::utils::container::std_vector<bool> ready(candidate.thread_count);
        ::uwvm2::utils::container::std_vector<::std::uint64_t> completions(candidate.thread_count,UINT64_MAX);
        for(auto const& item:candidate.origin.objects)if(item.kind==object_kind::thread && item.flags==2u)
        {
            auto flag=candidate.origin.objects[item.links.back()-1u].flags;
            ready[item.words[0]-1u]=flag!=0u;
            if(flag)completions[item.words[0]-1u]=flag==1u?0u:2u;
        }
        ::uwvm2::utils::container::std_vector<::std::uint64_t> importing(candidate.thread_count);
        ::std::uint64_t time{},sequence{},instruction{};bool ending{};::std::size_t endpoints{};
        for(auto& event:candidate.events)
        {
            // Each event carries its recording-relative CLOCK_MONOTONIC sample.
            // Strip it only in the private decoded copy; immutable file DATA is unchanged.
            if(event.bytes.size()<8u)return error::invalid_shape;
            ::std::uint64_t stamp{};auto const begin=event.bytes.size()-8u;
            for(unsigned n{};n<8u;++n)stamp|=::std::uint64_t{::std::to_integer<unsigned>(event.bytes[begin+n])}<<(8u*n);
            if(stamp<time || stamp>INT64_MAX)return error::invalid_shape;
            time=stamp;candidate.times.push_back(stamp);event.bytes.resize(begin);
            auto const& w=event.words;
            if(event.kind!=object_kind::event || w[0]!=++sequence || !w[2] || w[2]>candidate.thread_count)
                return error::invalid_shape;
            auto const thread=static_cast<::std::size_t>(w[2]-1u);
            if(ended[thread] || (ending && event.flags!=endpoint_kind && event.flags!=wait_endpoint_kind))return error::invalid_shape;
            if(event.flags==environment_kind)
            {
                if(w[1]!=instruction || w[3] || w[4] || w[5] || w[6]!=schema || w[7] ||
                   !event.links.empty() || !event.values.empty())return error::invalid_shape;
                // This is an observation only. Validate all detached environment
                // records here; none can supply execution/resource authority.
                namespace archive=::uwvm2::runtime::diagnostic::snapshot_archive;
                archive::decoded saved{};
                if(!archive::decode(event.bytes,saved) || saved.disclosure!=archive::visibility::full)return error::invalid_shape;
                for(auto const& row:saved.records)
                {
                    if(row.kind==archive::component::wasip1)
                    {
                        namespace wasi=::uwvm2::runtime::diagnostic::snapshot_wasip1;
                        wasi::environment e{};
                        if(!wasi::decode(saved.disclosure,row,e) || e.recording!=graph.recording_id)return error::invalid_shape;
                    }
                    else if(row.kind==archive::component::ros1 || row.kind==archive::component::ros2)
                    {
                        namespace ros=::uwvm2::runtime::diagnostic::snapshot_ros;
                        ros::environment e{};
                        if(!ros::decode(saved.disclosure,row,e) || e.recording!=graph.recording_id)return error::invalid_shape;
                    }
                    else return error::invalid_shape;
                }
            }
            else if(event.flags==instruction_kind || event.flags==endpoint_kind)
            {
                if(waiting[thread] || importing[thread])return error::invalid_shape;
                if(w[1]!=instruction+1u || w[4] || w[5] || !event.values.empty() || !event.links.empty() || !event.bytes.empty() ||
                   w[6]>=candidate.origin.root_instances.size() || w[7]>UINT32_MAX)return error::invalid_shape;
                if(event.flags==endpoint_kind){ending=true;ended[thread]=true;++endpoints;continue;}
                ++instruction;live_instruction[thread]=true;
            }
            else if(event.flags==wait_endpoint_kind)
            {
                if(!waiting[thread] || w[1]!=instruction+1u || w[4]!=9u || w[5] || event.bytes.size()!=32u || event.links.size()!=1u ||
                    !event.links[0] || event.links[0]>candidate.origin.objects.size() || w[6]>=candidate.origin.root_instances.size() || w[7]>UINT32_MAX)
                    return error::invalid_shape;
                auto const& memory=candidate.origin.objects[event.links[0]-1u];
                if(memory.kind!=object_kind::memory || memory.flags!=1u)return error::invalid_reference;
                for(auto const& value:event.values)if(value.type.kind!=value_kind::i64 || !value.initialized)return error::incompatible_type;
                auto v=[&](::std::size_t n){return event.values[n].low_bits;};
                if(v(8)>2u || (v(1)!=4u && v(1)!=8u) || (v(0)&(v(1)-1u)) || v(7)!=memory.words[0] ||
                    (v(3)>INT64_MAX && v(3)!=UINT64_MAX) || (v(8)!=0u && v(4)!=UINT64_MAX) || (v(8)==2u && v(3)) ||
                    (v(8)==0u && v(4)>=candidate.thread_count) || (v(1)==4u && v(2)>UINT32_MAX) ||
                    // The memory may have grown during the recorded execution.
                    // Its declared maximum bounds file DATA; the replay adapter
                    // additionally checks the actual live extent at this stop.
                    v(1)-1u>UINT64_MAX-v(0) || (v(0)+v(1)-1u)/65536u>=memory.words[3] ||
                    !valid_wait_guard(event,memory,static_cast<unsigned>(v(1)),static_cast<unsigned>(v(7))))return error::invalid_shape;
                if((!ready[thread] && v(8)!=0u) || (ready[thread] &&
                   (completions[thread]==1u || v(8)!=(completions[thread]==0u?1u:2u))))return error::invalid_shape;
                ending=true;ended[thread]=true;++endpoints;
            }
            else if(event.flags==tags_kind)
            {
                if((!live_instruction[thread] && !importing[thread]) || w[1]!=instruction || w[6]!=schema || w[7] || w[4] || w[5] ||
                   !event.values.empty() || event.links.empty() || event.links.size()>SIZE_MAX/32u ||
                   event.bytes.size()!=event.links.size()*32u || (w[3]>>32u)>=candidate.origin.root_instances.size())
                    return error::invalid_shape;
                object_id previous{};
                for(auto id:event.links)
                {
                    if(id<=previous || id>candidate.origin.objects.size() || candidate.origin.objects[id-1u].kind!=object_kind::memory)
                        return error::invalid_reference;
                    previous=id;
                }
            }
            else if(event.flags==entropy_kind)
            {
                // A raw entropy byte is data, never a native RNG state/address.
                // It is consumed only by the exact validated random-tag opcode.
                if(!live_instruction[thread] || w[1]!=instruction || w[6]!=schema ||
                   (w[7]!=32u && w[7]!=64u) || w[4] || w[5] || !event.values.empty() ||
                   event.links.size()!=1u || !event.links[0] || event.links[0]>candidate.origin.objects.size() ||
                   candidate.origin.objects[event.links[0]-1u].kind!=object_kind::memory ||
                   candidate.origin.objects[event.links[0]-1u].words[0]!=w[7] ||
                   event.bytes.size()!=2u || ::std::to_integer<unsigned>(event.bytes[0])>1u)
                    return error::invalid_shape;
                live_instruction[thread]=false;
            }
            else if(event.flags==import_kind || event.flags==import_begin_kind)
            {
                if(!live_instruction[thread] || importing[thread] || w[1]!=instruction || w[6]!=schema || w[7] ||
                   (w[3]>>32u)>=candidate.origin.root_instances.size() ||
                   (event.flags==import_begin_kind && w[5]))return error::invalid_shape;
                for(auto const& value:event.values)
                    if(value.type.kind<value_kind::i32 || value.type.kind>value_kind::v128 || !value.initialized)
                        return error::incompatible_type;
                object_id prior{};
                for(auto id:event.links)
                {
                    if(id<=prior || id>candidate.origin.objects.size() || candidate.origin.objects[id-1u].kind!=object_kind::memory)
                        return error::invalid_reference;
                    prior=id;
                }
                if(event.bytes.size()<16u || !::std::equal(replay_memory_detail::magic.begin(),replay_memory_detail::magic.end(),event.bytes.begin()))
                    return error::invalid_shape;
                live_instruction[thread]=false;
                if(event.flags==import_begin_kind)importing[thread]=w[0];
            }
            else if(event.flags==import_memtag_report_kind)
            {
                if(!importing[thread] || w[1]!=instruction || w[6]!=schema || w[7]!=importing[thread] ||
                   w[4]!=10u || w[5] || !event.links.empty() || !event.bytes.empty())return error::invalid_shape;
                auto const& begin=candidate.events[static_cast<::std::size_t>(w[7]-1u)];
                if(begin.flags!=import_begin_kind || begin.words[2]!=w[2] || begin.words[3]!=w[3])return error::invalid_shape;
                for(auto const& value:event.values)
                    if(value.type.kind!=value_kind::i64 || !value.initialized)return error::incompatible_type;
                auto v=[&](::std::size_t n){return event.values[n].low_bits;};
                // Source coordinates and tags are diagnostic DATA. They cannot
                // select a native provider, memory write, descriptor or pointer.
                if((v(4)!=UINT64_MAX && v(4)>=candidate.origin.root_instances.size()) ||
                   (v(5)!=UINT64_MAX && v(5)>UINT32_MAX) ||
                   (v(6)!=UINT64_MAX && v(6)>UINT32_MAX) ||
                   v(7)>UINT8_MAX || v(8)>UINT8_MAX || v(7)==v(8) || v(9)>1u)
                    return error::invalid_shape;
            }
            else if(event.flags==import_memory_kind || event.flags==import_finish_kind)
            {
                if(!importing[thread] || w[1]!=instruction || w[6]!=schema || w[7]!=importing[thread])return error::invalid_shape;
                auto const& begin=candidate.events[static_cast<::std::size_t>(w[7]-1u)];
                if(begin.flags!=import_begin_kind || begin.words[2]!=w[2] || begin.words[3]!=w[3])return error::invalid_shape;
                if(event.flags==import_memory_kind)
                {
                    if(w[4]!=4u || w[5]!=1u || event.values.size()!=5u)return error::invalid_shape;
                    for(auto const& value:event.values)
                        if(value.type.kind!=value_kind::i64 || !value.initialized)return error::incompatible_type;
                    if(event.values[0].low_bits>2u || event.values[4].low_bits>1u)return error::invalid_shape;
                    if(event.bytes.size()<16u || !::std::equal(replay_memory_detail::magic.begin(),replay_memory_detail::magic.end(),event.bytes.begin()))return error::invalid_shape;
                    object_id previous{};
                    for(auto id:event.links)
                    {
                        if(id<=previous || id>candidate.origin.objects.size() || candidate.origin.objects[id-1u].kind!=object_kind::memory)return error::invalid_reference;
                        previous=id;
                    }
                }
                else
                {
                    if(w[4] || !event.links.empty() || !event.bytes.empty())return error::invalid_shape;
                    for(auto const& value:event.values)
                        if(value.type.kind<value_kind::i32 || value.type.kind>value_kind::v128 || !value.initialized)return error::incompatible_type;
                    importing[thread]=0u;
                }
            }
            else if(event.flags==wait_begin_kind)
            {
                if(!live_instruction[thread] || waiting[thread] || importing[thread] || w[1]!=instruction || w[3]>2u || w[4]!=4u || w[5] ||
                   w[6]!=schema || (w[7]!=32u && w[7]!=64u) || event.bytes.size()!=32u || event.links.size()!=1u ||
                   !event.links[0] || event.links[0]>candidate.origin.objects.size() ||
                   candidate.origin.objects[event.links[0]-1u].kind!=object_kind::memory ||
                   candidate.origin.objects[event.links[0]-1u].words[0]!=w[7])return error::invalid_shape;
                for(auto const& value:event.values)
                    if(value.type.kind!=value_kind::i64 || !value.initialized)return error::incompatible_type;
                auto const width=w[3]==2u?8u:4u;
                auto const& memory=candidate.origin.objects[event.links[0]-1u];
                if(!valid_wait_guard(event,memory,width,static_cast<unsigned>(w[7])))return error::invalid_shape;
                waiting[thread]=w[0];ready[thread]=false;completions[thread]=UINT64_MAX;live_instruction[thread]=false;
            }
            else if(event.flags==wait_ready_kind)
            {
                if(!waiting[thread] || ready[thread] || w[7]!=waiting[thread] || w[1]!=instruction ||
                   w[3]<1u || w[3]>2u || w[4] || w[5]!=1u || w[6]!=schema || !event.links.empty() ||
                   !event.bytes.empty() || event.values[0].type.kind!=value_kind::i32 ||
                   !event.values[0].initialized || event.values[0].low_bits>2u)return error::invalid_shape;
                if(w[7]!=UINT64_MAX)
                {
                    auto const& begin=candidate.events[static_cast<::std::size_t>(w[7]-1u)];
                    if(begin.flags!=wait_begin_kind || begin.words[3]!=w[3])return error::invalid_shape;
                }
                else
                {
                    auto origin_thread=::std::find_if(candidate.origin.objects.begin(),candidate.origin.objects.end(),
                        [&](auto const& item){return item.kind==object_kind::thread && item.words[0]==w[2];});
                    if(origin_thread==candidate.origin.objects.end() || origin_thread->flags!=2u ||
                       w[3]!=(candidate.origin.objects[origin_thread->links.back()-1u].words[1]==4u?1u:2u))return error::invalid_shape;
                }
                ready[thread]=true;completions[thread]=event.values[0].low_bits;
            }
            else if(event.flags==wait_end_kind)
            {
                if(!waiting[thread] || (w[3]!=0u && !ready[thread]) || w[7]!=waiting[thread] || w[1]!=instruction || w[3]>2u || w[4] || w[5]!=1u || w[6]!=schema ||
                   !event.links.empty() || !event.bytes.empty() || event.values[0].type.kind!=value_kind::i32 ||
                    !event.values[0].initialized || (w[3]!=0u &&
                    (event.values[0].low_bits>2u || event.values[0].low_bits!=completions[thread])))return error::invalid_shape;
                if(w[7]!=UINT64_MAX)
                {
                    auto const& begin=candidate.events[static_cast<::std::size_t>(w[7]-1u)];
                    if(begin.flags!=wait_begin_kind || begin.words[2]!=w[2] || begin.words[3]!=w[3])return error::invalid_shape;
                }
                else
                {
                    auto origin_thread=::std::find_if(candidate.origin.objects.begin(),candidate.origin.objects.end(),
                        [&](auto const& item){return item.kind==object_kind::thread && item.words[0]==w[2];});
                    if(origin_thread==candidate.origin.objects.end() || origin_thread->flags!=2u)return error::invalid_shape;
                    auto const& pending=candidate.origin.objects[origin_thread->links.back()-1u];
                    if(w[3]!=(pending.words[1]==4u?1u:2u))return error::invalid_shape;
                }
                waiting[thread]=0u;
            }
            else if(event.flags==thread_exit_kind)
            {
                if(waiting[thread] || importing[thread] || w[1]!=instruction || w[3] || w[4] || w[5] || w[6]!=schema || w[7] ||
                   !event.links.empty() || !event.values.empty() || !event.bytes.empty())return error::invalid_shape;
                ended[thread]=true;
            }
            else return error::unsupported_version;
        }
        if(pending_prefix)
        {
            if(ending || endpoints || !instruction ||
               ::std::find_if(importing.begin(),importing.end(),[](auto id){return id!=0u;})==importing.end())
                return error::invalid_shape;
        }
        else if(!endpoints || !instruction || ::std::find(ended.begin(),ended.end(),false)!=ended.end())return error::invalid_shape;
        output=::std::move(candidate);return error::none;
    }
    }
    [[nodiscard]] inline error decode(state const& graph,decoded& output,limits const& budget={})
    {return details::decode_journal(graph,output,budget,false);}
    // An incomplete native result remains unknown. This parses only known
    // historical data; the ordinary executable decoder still rejects it.
    [[nodiscard]] inline error inspect_pending_prefix(state const& graph,decoded& output,limits const& budget={})
    {return details::decode_journal(graph,output,budget,true);}
}
