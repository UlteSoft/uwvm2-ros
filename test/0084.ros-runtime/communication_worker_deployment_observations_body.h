/** @brief Native graph/event regression body; included in the actual deployment fixture. */
    namespace gc=uwvm2::imported::ros::graph_contract;namespace ec=uwvm2::imported::ros::event_contract;
    auto wait_observation=[&](auto&& operation)
    {
        s::error result{};until([&]{result=operation();assert(result==s::error::success||result==s::error::again);return result==s::error::success;});
    };
    for(auto& item:members)
    {
        assert(broker.permissions.open(who,item.graph_alias,item.graph_cap)==s::error::success);
        assert(broker.permissions.open(who,item.event_alias,item.event_cap)==s::error::success);
        auto base=[&](auto& out){return out.u64(item.context_cap)&&out.u64(item.context)&&out.u64(item.node_cap)&&out.u64(item.node)&&contract::encode_profile(out,item.graph_plan.policy.selected);};
        auto request=encode([&](auto& out){return base(out)&&out.boolean(false)&&out.u64(2);});
        assert(host.invoke_observation<gc::operation::snapshot>(who,item.graph_cap,request,16,required,[](wire::bytes){return true;})==s::error::too_large&&required==24);
        bytes page;bool copied{};
        auto lost=[&](wire::bytes data){page.assign(data.begin(),data.end());copied=true;return false;};
        until([&]{auto status=host.invoke_observation<gc::operation::snapshot>(who,item.graph_cap,request,65536,required,lost);
            assert(status==s::error::again||status==s::error::io);return copied;});
        bytes retried;
        wait_observation([&]{return host.invoke_observation<gc::operation::snapshot>(who,item.graph_cap,request,65536,required,[&](wire::bytes data){retried.assign(data.begin(),data.end());return true;});});
        assert(page==retried);
        wire::reader rows{page};std::uint64_t revision{},cursor{},count{};bool more{};
        assert(rows.u64(revision)&&revision&&rows.boolean(more)&&(!more||rows.u64(cursor))&&rows.u64(count)&&count&&count<=2);
        for(std::uint64_t n{};n<count;++n)
        {
            std::string_view schema,encoding;wire::bytes payload;assert(rows.text(schema)&&schema=="uwvm/graph/Entity"&&rows.text(encoding)&&encoding=="scene-canonical"&&rows.blob(payload));
            wire::reader row{payload};std::uint64_t kind{};std::string_view node,endpoint,type;
            assert(row.u64(kind)&&kind&&!(kind&(kind-1))&&(kind&31)&&row.text(node)&&row.text(endpoint)&&row.text(type)&&row.done());
            assert(std::find(item.graph_plan.policy.nodes.begin(),item.graph_plan.policy.nodes.end(),node)!=item.graph_plan.policy.nodes.end());
        }
        assert(rows.done());
        auto watch=encode(base);
        assert(host.invoke_observation<gc::operation::watch>(who,item.graph_cap,watch,7,required,[](wire::bytes){return true;})==s::error::too_large&&required==8);
        assert(host.invoke_observation<gc::operation::watch>(who,item.graph_cap,watch,8,required,[](wire::bytes){return false;})==s::error::io);
        assert(host.invoke_observation<gc::operation::watch>(who,item.graph_cap,watch,8,required,[&](wire::bytes data){wire::reader in{data};return in.u64(item.watch)&&item.watch&&in.done();})==s::error::success);
        // Domain labels reserve storage only: a cross-domain topic still needs
        // its separately granted capability and actual typed origin chain.
        auto const& other=members[1];
        auto opening=encode([&](auto& out){return out.u64(2)&&out.u64(item.graph_cap)&&out.u64(item.watch)&&out.u64(other.topic_cap)&&out.u64(other.topic_id)&&
            contract::encode_profile(out,item.event_profile)&&out.u64(65536)&&out.u64(8)&&out.u64(65536);});
        assert(host.invoke_observation<ec::operation::open>(who,item.event_cap,opening,7,required,[](wire::bytes){return true;})==s::error::too_large&&required==8);
        until([&]{auto status=host.invoke_observation<ec::operation::open>(who,item.event_cap,opening,8,required,[](wire::bytes){return false;});
            assert(status==s::error::again||status==s::error::io);return status==s::error::io;});
        wait_observation([&]{return host.invoke_observation<ec::operation::open>(who,item.event_cap,opening,8,required,[&](wire::bytes data){wire::reader in{data};return in.u64(item.stream)&&item.stream&&in.done();});});
        auto poll=encode([&](auto& out){return out.u64(item.stream)&&out.u64(8)&&out.u64(c::now()+900000000);});
        bytes event;copied=false;
        until([&]{auto status=host.invoke_observation<ec::operation::poll>(who,item.event_cap,poll,65536,required,[&](wire::bytes data){event.assign(data.begin(),data.end());copied=true;return false;});
            assert(status==s::error::again||status==s::error::io);return copied;});
        wait_observation([&]{return host.invoke_observation<ec::operation::poll>(who,item.event_cap,poll,65536,required,[&](wire::bytes data){retried.assign(data.begin(),data.end());return true;});});
        assert(event==retried);wire::reader records{event};assert(records.u64(count)&&count&&count<=8);
        for(std::uint64_t n{};n<count;++n)
        {
            std::uint64_t sequence{},index{},generation{},lost{};std::string_view name,schema,encoding;bool stamp{};wire::bytes payload;
            assert(records.u64(sequence)&&sequence&&records.u64(index)&&index>=1&&index<=2&&records.u64(generation)&&generation&&records.text(name)&&name=="readiness.snapshot"&&
                records.boolean(stamp)&&!stamp&&records.u64(lost)&&!lost&&records.text(schema)&&schema=="uwvm/events/Readiness"&&records.text(encoding)&&encoding=="scene-canonical"&&records.blob(payload));
            wire::reader value{payload};std::uint64_t fields{};bool ready{},terminal{};assert(value.u64(fields)&&fields==3&&value.boolean(ready)&&value.boolean(terminal)&&value.done());
        }
        assert(records.done());
        auto closing=encode([&](auto& out){return out.u64(item.stream);});
        assert(host.invoke_observation<ec::operation::close>(who,item.event_cap,closing,8,required,[](wire::bytes){return false;})==s::error::io);
        assert(host.invoke_observation<ec::operation::close>(who,item.event_cap,closing,8,required,[](wire::bytes data){wire::reader in{data};bool closed{};return in.boolean(closed)&&closed&&in.done();})==s::error::success);
        wait_observation([&]{return host.invoke_observation<ec::operation::open>(who,item.event_cap,opening,8,required,[&](wire::bytes data){wire::reader in{data};return in.u64(item.stream)&&item.stream&&in.done();});});
        assert(host.close(who,item.graph_cap,item.watch)==s::error::success);
        poll=encode([&](auto& out){return out.u64(item.stream)&&out.u64(8)&&out.u64(c::now()+900000000);});
        assert(host.invoke_observation<ec::operation::poll>(who,item.event_cap,poll,65536,required,[](wire::bytes){return true;})==s::error::bad_handle);
        assert(host.close(who,item.event_cap,item.stream)==s::error::success);
        assert(host.close(who,item.event_cap,item.stream)==s::error::success);
        assert(broker.revoke(item.graph_alias)==s::error::success);
    }
