/** @file
 * @brief Filtered graph observations through a finite isolated worker query.
 * @details The source retains a counted host node and an immutable graph plan.
 * It accepts no socket, plugin, host path or native node pointer from the guest.
 * A trusted supervisor connects its query queue to the sealed worker binding.
 * The caller's complete graph token is revalidated before send and reply copy.
 * No authority from the first opener is reused for subsequent captures.
 *
 * Replies are validated in full before allocating entity strings. Every row must
 * match an approved node, field mask, canonical name and finite extent. Provider
 * ordering is normalized, so enumeration order alone cannot invent a new graph
 * revision. This is untrusted discovery metadata, never peer authentication.
 * CPU/NUMA placement and worker cgroup reservation are supervisor obligations;
 * this adapter adds neither a hidden thread nor an unbounded receive buffer.
 */
#pragma once
#include "communication_graph.h"
#include "communication_queries.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
#if defined(__linux__)
    class graph_source final:public communication::graph::source
    {
        using error=communication::error;
        using plan=communication::graph::plan;
        using entity=communication::graph::entity;
        // The callback owner outlives queries, including retained failed work.
        std::shared_ptr<void> authority_owner;
        communication::node_lease node;
        plan policy;
        queries observations;
        std::atomic<bool> configured{};
        pid_t process{::getpid()};
        bool same_process()const noexcept{return process==::getpid();}
        error decode(wire::bytes bytes,::uwvm2::utils::container::std_vector<entity>& output)
        {
            wire::reader input{bytes};std::uint64_t count{};
            if(!input.u64(count)||count>policy.max_rows)return error::invalid;
            std::uint64_t extent{};
            for(std::uint64_t i{};i<count;++i)
            {
                std::uint64_t kind{};std::string_view node_name,endpoint,type;
                if(!input.u64(kind)||!kind||(kind&(kind-1))||(kind&~std::uint64_t{31})||!(policy.fields&kind)||
                    !input.text(node_name)||!input.text(endpoint)||!input.text(type)||node_name.size()>255||endpoint.size()>255||type.size()>255||
                    std::find(policy.nodes.begin(),policy.nodes.end(),node_name)==policy.nodes.end()||
                    type.find('\0')!=std::string_view::npos||((policy.fields&32)==0&&!type.empty())||
                    (kind==1?(!endpoint.empty()||!type.empty()):!communication::absolute_name(endpoint)))return error::invalid;
                auto size=std::uint64_t{32}+node_name.size()+endpoint.size()+type.size();
                if(size>policy.max_bytes-extent)return error::invalid;
                extent+=size;
            }
            if(!input.done())return error::invalid;
            // snapshot owns this detached destination and discards it on any
            // failed capture. Reuse its bounded row reservation; partial string
            // construction after allocation failure is never a successful result.
            output.clear();output.reserve(static_cast<std::size_t>(count));
            input=wire::reader{bytes};if(!input.u64(count))return error::invalid;
            for(std::uint64_t i{};i<count;++i)
            {
                std::uint64_t kind{};std::string_view node_name,endpoint,type;
                if(!input.u64(kind)||!input.text(node_name)||!input.text(endpoint)||!input.text(type))return error::invalid;
                output.push_back({static_cast<communication::graph::kind>(kind),::uwvm2::utils::container::std_string{node_name},
                    ::uwvm2::utils::container::std_string{endpoint},::uwvm2::utils::container::std_string{type}});
            }
            std::sort(output.begin(),output.end(),[](entity const& a,entity const& b)
            {
                if(a.type!=b.type)return a.type<b.type;
                if(a.node!=b.node)return a.node<b.node;
                if(a.endpoint!=b.endpoint)return a.endpoint<b.endpoint;
                return a.message_type<b.message_type;
            });
            output.erase(std::unique(output.begin(),output.end()),output.end());return error::ok;
        }
    public:
        graph_source()=default;graph_source(graph_source const&)=delete;graph_source& operator=(graph_source const&)=delete;
        /** @brief Freeze exact origin, filter and independent RPC storage before publication.
         * @details The queue's stable resource is the graph binding's host identity;
         * a readiness token additionally binds its exact watch incarnation. The
         * queue must reserve room for the complete bounded native snapshot, not
         * merely a guest page. No native operation or admission charge occurs.
         */
        error configure(plan selected,communication::node_lease attachment,communication_jobs::plan queue,
                        communication_jobs::admit revalidate,communication_jobs::charge charge,void* context,
                        std::shared_ptr<void> retained_authority={})noexcept
        {
            if(!same_process()||configured.load()||(retained_authority&&retained_authority.get()!=context))return error::denied;
            if(!selected.authority||!communication::valid(selected.selected)||!attachment.live()||selected.context!=attachment.context()||
                selected.authority.environment!=attachment.authority().environment||selected.nodes.empty()||!selected.fields||
                (selected.fields&~std::uint64_t{63})||!(selected.fields&31)||!selected.max_rows||selected.max_rows>65536||
                !selected.max_bytes||selected.max_bytes>4u*1024u*1024u-8||!selected.max_page_items||selected.max_page_items>selected.max_rows||
                !selected.capture_deadline_ns||selected.capture_deadline_ns>INT64_MAX||!selected.cursor_lifetime_ns||selected.cursor_lifetime_ns>INT64_MAX||
                queue.authority!=selected.authority||queue.resource!=selected.authority.grant||queue.family!=communication::operation_family::graph||
                queue.reply_bytes<selected.max_bytes+8||queue.max_deadline_ns<selected.capture_deadline_ns)return error::invalid;
            for(std::size_t i{};i<selected.nodes.size();++i)
            {
                if(!communication::absolute_name(selected.nodes[i]))return error::invalid;
                for(std::size_t n{};n<i;++n)if(selected.nodes[n]==selected.nodes[i])return error::invalid;
            }
            authority_owner=std::move(retained_authority);
            auto status=observations.configure(queue,revalidate,charge,context);if(status!=error::ok)return status;
            policy=std::move(selected);node=std::move(attachment);configured.store(true,std::memory_order_release);return error::ok;
        }
        bool live()const noexcept override{return same_process()&&configured.load(std::memory_order_acquire)&&node.live();}
        communication::handle context()const noexcept override{return same_process()&&configured.load()?node.context():0;}
        communication::owner authority()const noexcept override{return same_process()&&configured.load()?node.authority():communication::owner{};}
        bool queued_captures()const noexcept override{return true;}
        /** @brief Check exact immutable factory identity without native I/O or admission. */
        bool matches(plan const& selected,communication::native_node const* parent)const noexcept
        {return live()&&selected==policy&&node.get()==parent;}
        error capture(plan const&,::uwvm2::utils::container::std_vector<entity>&)noexcept override{return error::denied;}
        error capture(plan const& selected,::uwvm2::utils::container::std_vector<entity>& output,communication::operation_token const& origin)noexcept override
        {
            if(!live()||selected!=policy||origin.authority!=policy.authority||origin.resource!=policy.authority.grant||
                origin.origins[1].authority!=node.authority()||origin.origins[2].resource!=node.context())return error::denied;
            auto now=communication::now();if(now>INT64_MAX-policy.capture_deadline_ns)return error::too_large;
            return observations.graph_capture(origin,now+policy.capture_deadline_ns,[&](wire::bytes input){return decode(input,output);});
        }
        error pump(communication_worker_async::transport& transport)noexcept{return observations.pump(transport);}
        error cancel(communication_worker_async::transport& transport,communication_jobs::receipt& out)noexcept{return observations.cancel(transport,out);}
        bool occupied()const noexcept{return observations.occupied();}
        void request_stop()noexcept{observations.request_stop();}
        void retire()noexcept{observations.retire();}
    };
#endif
}
