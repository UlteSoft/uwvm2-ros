// Included in the actual deployment fixture after both nodes are published.
    namespace ac=uwvm2::imported::ros::action_contract;
    namespace act=c::actions;
    auto action_call=[&](member& item,ac::operation operation,bytes const& body,bytes& output,bool copy=true,std::size_t capacity=65536)
    {return broker.invoke_communication_action(who,operation,item.action_cap,body,capacity,required,
        [&](wire::bytes data){output.assign(data.begin(),data.end());return copy;});};
    auto action_wait=[&](auto&& work)
    {
        auto deadline=c::now()+10000000000ull;s::error status;
        while((status=work())==s::error::again){assert(c::now()<deadline);::poll(nullptr,0,1);}
        return status;
    };
    for(auto& item:members)
    {
        auto const& p=item.action_plan.policy;auto opening=encode([&](auto& out)
        {return out.u64(item.node_cap)&&out.u64(item.node)&&out.u64(3)&&out.u64(item.action_schema_caps[0])&&out.u64(item.action_schema_caps[1])&&out.u64(item.action_schema_caps[2])&&
            contract::encode_profile(out,p.selected)&&out.text(p.resolved_name)&&out.text(p.action_type)&&out.text(p.direction==act::role::client?"client":"server")&&
            out.u64(1)&&out.u64(c::now()+std::min<std::uint64_t>(3500000000,p.max_deadline_ns));});
        bytes result;
        auto short_status=action_wait([&]{return action_call(item,ac::operation::open,opening,result,true,47);});
        if(short_status!=s::error::too_large||required!=48)
            std::fprintf(stderr,"action short output: status=%u required=%zu alias=%s\n",static_cast<unsigned>(short_status),required,item.action_alias.c_str());
        assert(short_status==s::error::too_large&&required==48);
        assert(action_wait([&]{return action_call(item,ac::operation::open,opening,result,false);})==s::error::io);
        auto first=result;assert(action_wait([&]{return action_call(item,ac::operation::open,opening,result);})==s::error::success&&first==result);
        wire::reader receipt{result};std::uint64_t key{};assert(receipt.u64(key)&&key==1&&receipt.u64(item.action_request)&&item.action_request);
    }
    for(auto& item:members)
    {
        assert(action_wait([&]{return take(item.action_cap,item.action_request,item.action_id,4);})==s::error::success);
        s::communication_action_dispatch::usage usage;assert(host.inspect_action(item.action_alias,usage)==s::error::success&&usage.handles==1&&usage.configured==1);
    }
    // Give the native ROS discovery protocol a finite observation interval.
    auto discovered=c::now()+2000000000;while(c::now()<discovered)::poll(nullptr,0,2);
    auto mutation=[&](bytes const& body,std::uint64_t generation)
    {
        wire::reader in{body};std::uint64_t request{},actual{};std::string_view phase,effect,reason;
        assert(in.u64(request)&&request&&in.text(phase)&&phase=="admitted"&&in.text(effect)&&effect=="locally_accepted"&&
            in.u64(actual)&&actual==generation&&in.text(reason)&&reason=="none"&&in.done());return request;
    };
    auto action_take=[&](member& item,act::event_kind wanted,std::size_t index)
    {
        auto body=encode([&](auto& out){return out.u64(item.action_id);});auto deadline=c::now()+10000000000ull;bytes output;
        while(true)
        {
            auto status=action_call(item,ac::operation::take,body,output,false);
            if(status==s::error::io)
            {
                auto first=output;assert(action_wait([&]{return action_call(item,ac::operation::take,body,output);})==s::error::success&&first==output);
                w::action_record record;assert(w::decode_action_record(output,item.action_plan.policy,item.action_id,peers[index].definition.network.generation,record));
                if(record.kind==wanted){record.payload={};return record;}
                assert(record.kind==act::event_kind::status||record.kind==act::event_kind::acceptance);
            }
            else assert(status==s::error::again);
            assert(c::now()<deadline);::poll(nullptr,0,1);
        }
    };
    auto& client=members[0];auto& server=members[1];act::uuid uuid{};uuid[0]=91;
    auto goal=encode([](auto& out){return out.text("world")&&out.text("robot")&&out.i64(0)&&out.u64(0)&&out.i64(0)&&out.u64(0)&&out.i64(0)&&out.u64(0)&&out.text("")&&out.boolean(false);});
    auto submit=encode([&](auto& out){auto const& schema=client.action_plan.policy.goal;return out.u64(client.action_id)&&out.u64(++client.action_sequence)&&out.u64(client.action_id)&&out.blob(uuid)&&
        contract::encode_message(out,{schema.type_identity,schema.encoding,goal})&&out.u64(c::now()+900000000);});
    bytes action_receipt;
    assert(action_wait([&]{return action_call(client,ac::operation::submit,submit,action_receipt);})==s::error::success);
    auto client_goal=mutation(action_receipt,peers[0].definition.network.generation);
    auto proposal=action_take(server,act::event_kind::proposal,1);assert(proposal.identity==uuid&&proposal.native_goal!=proposal.native_request&&proposal.native_goal!=client_goal);
    auto result=encode([](auto& out){if(!out.i64(0)||!out.u64(0)||!out.text("world")||!out.text("robot"))return false;for(double v:{0.,0.,0.,0.,0.,0.,1.})if(!out.f64(v))return false;return out.u64(0)&&out.text("");});
    auto respond=[&](act::transition change,c::handle request,bool accepts=false)
    {
        auto const& schema=change==act::transition::feedback?server.action_plan.policy.feedback:server.action_plan.policy.result;
        bool message=change==act::transition::feedback||change==act::transition::cancelled;
        auto body=encode([&](auto& out){return out.u64(server.action_id)&&out.u64(++server.action_sequence)&&out.u64(server.action_id)&&out.u64(request)&&out.u64(proposal.native_goal)&&
            out.text(ac::transition_names[static_cast<std::size_t>(change)-1])&&out.boolean(message)&&(!message||contract::encode_message(out,{schema.type_identity,schema.encoding,change==act::transition::cancelled?wire::bytes{result}:wire::bytes{}}))&&
            out.u64(accepts?1:0)&&(!accepts||out.u64(proposal.native_goal))&&out.u64(c::now()+900000000);});
        assert(action_wait([&]{return action_call(server,ac::operation::respond,body,action_receipt);})==s::error::success);
        auto first=action_receipt;assert(action_wait([&]{return action_call(server,ac::operation::respond,body,action_receipt);})==s::error::success&&first==action_receipt);
    };
    respond(act::transition::accept,proposal.native_request);
    assert(action_take(client,act::event_kind::acceptance,0).native_goal==client_goal);
    respond(act::transition::execute,proposal.native_request);
    respond(act::transition::feedback,proposal.native_request);assert(action_take(client,act::event_kind::feedback,0).native_goal==client_goal);
#if defined(UWVM_COMMUNICATION_DEPLOYMENT_SELECTION)
    auto cancel=encode([&](auto& out){return out.u64(client.action_id)&&out.u64(++client.action_sequence)&&out.u64(client.action_id)&&out.u64(1)&&out.u64(client_goal)&&out.u64(c::now()+900000000);});
    assert(action_wait([&]{return action_call(client,ac::operation::cancel_set,cancel,action_receipt,false);})==s::error::io);
    auto lost_selection=action_receipt;
    assert(action_call(client,ac::operation::cancel_set,cancel,action_receipt)==s::error::success);
    // Settlement may refine admission evidence between these two reads; it
    // cannot allocate another native cancellation correlation for exact retry.
    auto selection_request=[&](bytes const& record)
    {
        wire::reader input{record};std::uint64_t batch{},generation{},count{},goal{},request{},status{},effect{};std::string_view phase,observed,reason;
        assert(input.u64(batch)&&batch&&input.text(phase)&&input.text(observed)&&input.u64(generation)&&input.text(reason)&&
            input.u64(count)&&count==1&&input.u64(goal)&&goal==client_goal&&input.u64(request)&&request&&input.u64(generation)&&
            generation==peers[0].definition.network.generation&&input.u64(status)&&status==0&&input.u64(effect)&&effect&&effect<=2&&input.done());
        return request;
    };
    auto cancellation=selection_request(action_receipt);assert(selection_request(lost_selection)==cancellation);
#else
    auto cancel=encode([&](auto& out){return out.u64(client.action_id)&&out.u64(++client.action_sequence)&&out.u64(client.action_id)&&out.u64(client_goal)&&out.u64(c::now()+900000000);});
    assert(action_wait([&]{return action_call(client,ac::operation::cancel,cancel,action_receipt);})==s::error::success);
    auto cancellation=mutation(action_receipt,peers[0].definition.network.generation);
#endif
    auto offered=action_take(server,act::event_kind::cancel_request,1);assert(offered.native_goal==proposal.native_goal&&offered.native_request!=proposal.native_request);
    respond(act::transition::cancel_response,offered.native_request,true);assert(action_take(client,act::event_kind::cancel_response,0).native_request==cancellation);
    respond(act::transition::cancelled,proposal.native_request);auto completed=action_take(client,act::event_kind::result,0);
    assert(completed.native_goal==client_goal&&completed.observed==act::state::cancelled);
    for(auto& item:members)
    {
        auto describe=encode([&](auto& out){return out.u64(item.action_id)&&out.boolean(false);});
        assert(action_wait([&]{return action_call(item,ac::operation::describe,describe,action_receipt);})==s::error::success);
        auto closing=encode([&](auto& out){return out.u64(item.action_id);});
        assert(action_wait([&]{return broker.invoke(who,uwvm2::imported::ros::runtime_contract::operation::resource_close,item.action_cap,closing,0,required,[](wire::bytes data){return data.empty();});})==s::error::success);
        auto releasing=encode([&](auto& out){return out.u64(item.action_request);});
        assert(action_wait([&]{return broker.invoke_model(who,uwvm2::imported::ros::model_contract::operation::request_release,item.action_cap,releasing,8,required,[](wire::bytes data)
            {wire::reader in{data};bool released{};return in.boolean(released)&&released&&in.done();});})==s::error::success);
    }
