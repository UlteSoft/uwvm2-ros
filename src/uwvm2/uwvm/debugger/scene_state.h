/** @file
 * @brief Read-only console grammar for the independent robot scene namespace.
 * @details `info scene authority|resources|audit|policy` is metadata inspection only.
 * Filters cannot be interpreted as provider commands. The controller binds
 * requests to a cooperative stop and checks the existing debugger permission.
 */
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/scene/debug.h>
#include <uwvm2/utils/container/standard.h>
#include <fast_io.h>
#include <fast_io_dsal/array.h>
#include <fast_io_dsal/string_view.h>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::scene_state
{
    namespace data=::uwvm2::imported::ros::scene::debug;
    using request=data::request;using view=data::view;using selection=data::selection;using status=data::status;
    inline bool valid(request const& query) noexcept{return data::valid(query);}
    inline void read(request const& query,view& output) noexcept{data::snapshot(query,output);}
    inline ::std::string_view selection_text(selection value) noexcept
    {return value==selection::authority?"authority":value==selection::resources?"resources":value==selection::audit?"audit":value==selection::policy?"policy":"invalid";}
    inline ::std::string_view status_text(status value) noexcept
    {
        switch(value)
        {
        case status::ok:return "available";case status::invalid_request:return "invalid request";
        case status::not_selected:return "scene Host not selected";case status::busy:return "busy";
        case status::forbidden:return "inspection forbidden";case status::requires_cooperative_stop:return "requires current cooperative stop";
        case status::stale_stop:return "stale stop";case status::resource_limit:return "bounded scan limit";
        }
        return "invalid status";
    }
    /** @brief Parse exact labels and bounded unsigned page/stop values. */
    inline bool parse(::fast_io::string_view line,request& output) noexcept
    {
        ::fast_io::array<::fast_io::string_view,8> words{};::std::size_t count{},cursor{};
        while(cursor<line.size())
        {
            while(cursor<line.size()&&::fast_io::char_category::c_space::char_is(line[cursor]))++cursor;
            if(cursor==line.size())break;
            if(count==words.size())return false;
            auto start=cursor;
            while(cursor<line.size()&&!::fast_io::char_category::c_space::char_is(line[cursor]))
            {if(!::fast_io::char_category::is_c_graph(static_cast<unsigned char>(line[cursor])))return false;++cursor;}
            words[count++]=line.subview(start,cursor-start);
        }
        if(count<3||words[0]!="info"||words[1]!="scene")return false;
        request query{};
        if(words[2]=="authority")query.selected=selection::authority;
        else if(words[2]=="resources")query.selected=selection::resources;
        else if(words[2]=="audit")query.selected=selection::audit;
        else if(words[2]=="policy")query.selected=selection::policy;
        else return false;
        unsigned seen{};
        for(::std::size_t i=3;i<count;++i)
        {
            auto token=words[i];auto equal=token.find_character('=');
            if(equal==::fast_io::containers::npos||!equal||equal+1==token.size())return false;
            auto key=token.subview(0,equal),value=token.subview(equal+1);unsigned bit{};
            if(key=="environment"||key=="alias"||key=="operation")
            {
                bit=key=="environment"?1:key=="alias"?2:32;
                for(char c:value)if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='-'||c=='.'))return false;
                if(key=="operation")query.operation.assign({value.data(),value.size()});
                else (key=="environment"?query.environment:query.alias).assign({value.data(),value.size()});
            }
            else
            {
                ::std::uint64_t number{};
                auto parsed=::fast_io::parse_by_scan(value.cbegin(),value.cend(),::fast_io::mnp::dec_get<true,true>(number));
                if(parsed.code!=::fast_io::parse_code::ok||parsed.iter!=value.cend())return false;
                if(key=="first"){bit=4;query.first=number;}
                else if(key=="count"){bit=8;query.count=number;}
                else if(key=="if-stop"){bit=16;if(!number)return false;query.stop=number;}
                else return false;
            }
            if(seen&bit)return false;
            seen|=bit;
        }
        if(!data::valid(query))return false;
        output=query;return true;
    }
    template<typename Output,::std::size_t N> inline void quoted(Output& output,data::text<N> const& label)
    {
        ::fast_io::io::print(output,"\"");constexpr char hex[]="0123456789abcdef";
        for(unsigned char c:label.view())
        {
            if(c>=32&&c<127&&c!='"'&&c!='\\')::fast_io::io::print(output,::fast_io::mnp::chvw(static_cast<char>(c)));
            else{char escape[]{'\\','x',hex[c>>4],hex[c&15]};::fast_io::io::print(output,::std::string_view{escape,4});}
        }
        ::fast_io::io::print(output,"\"",label.truncated ? ::std::string_view{"[truncated]"} : ::std::string_view{});
    }
    /** @brief Serialize only complete bounded metadata pages under the broker reply cap. */
    template<typename Output> inline void print(Output& output,view const& value,::std::uint64_t stop)
    {
        ::fast_io::io::print(output,"scene scope=process-owned stop=",stop," selection=",selection_text(value.selected)," status=",status_text(value.result),"\n");
        if(value.result!=status::ok)return;
        if(value.selected==selection::policy)
        {
            if(value.size>value.policy.size()){::fast_io::io::print(output,"invalid bounded snapshot\n");return;}
            for(::std::size_t i{};i<value.size;++i)
            {
                auto const& row=value.policy[i];
                if(!data::valid(row)){::fast_io::io::print(output,"invalid policy snapshot\n");return;}
                ::fast_io::io::print(output,"policy environment=");quoted(output,row.environment);
                ::fast_io::io::print(output," operation=");quoted(output,row.operation);
                ::fast_io::io::print(output," configured=",row.configured," effective=",row.effective," origin=",row.origin,"\n");
            }
            ::fast_io::io::print(output,"page total=",value.total," next=",value.next," more=",value.more,"\n");
            return;
        }
        if(value.selected==selection::audit)
        {
            auto const& counters=value.audit;
            if(value.size) ::fast_io::io::print(output,"audit configured=",counters.configured,
                " admitted=",counters.admitted," limited=",counters.limited," busy=",counters.busy,
                " clock_failed=",counters.clock_failed," sink_failed=",counters.sink_failed,"\n");
            ::fast_io::io::print(output,"page total=",value.total," next=",value.next," more=",value.more,"\n");
            return;
        }
        if(value.size>value.rows.size()){::fast_io::io::print(output,"invalid bounded snapshot\n");return;}
        for(::std::size_t i=0;i<value.size;++i)
        {
            if(!data::valid(value.rows[i]))
            {::fast_io::io::print(output,"invalid bounded snapshot\n");return;}
        }
        for(::std::size_t i=0;i<value.size;++i)
        {
            auto const& row=value.rows[i];::fast_io::io::print(output,"row kind=",::std::string_view{row.kind==data::row_kind::grant?"grant":"resource"}," environment=");quoted(output,row.environment);
            ::fast_io::io::print(output," alias=");quoted(output,row.alias);::fast_io::io::print(output," resource=");quoted(output,row.resource);
            ::fast_io::io::print(output," binding=");quoted(output,row.binding);::fast_io::io::print(output," type=");quoted(output,row.type);::fast_io::io::print(output," state=");quoted(output,row.state);
            ::fast_io::io::print(output," platform=");quoted(output,row.platform);
            ::fast_io::io::print(output," id=",row.id," open_handles=",row.open_handles," admissions=",row.admissions," refused=",row.refused,
                " sequence=",row.sequence," members=",row.members," size_bytes=",row.size_bytes,
                " accepted_bytes=",row.accepted_bytes," reserved_bytes=",row.reserved_bytes," retained_receipts=",row.retained_receipts,
                " generation=",row.generation," transition=",row.transition," deadline_ns=",row.deadline_ns," status_code=",row.status_code," lease_count=",row.lease_count," workspace_bytes=",row.workspace_bytes,
                " input_bytes=",row.input_bytes," output_bytes=",row.output_bytes," output_items=",row.output_items,
                " sample_count=",row.sample_count," query_count=",row.query_count," pending_queries=",row.pending_queries,
                " query_bytes=",row.query_bytes," carrier_bytes=",row.carrier_bytes,
                " emitted_records=",row.emitted_records," exported_records=",row.exported_records," lost_records=",row.lost_records,
                " retained_bytes=",row.retained_bytes," copy_failures=",row.copy_failures,
                " revoked=",row.revoked," attenuated=",row.attenuated," retired=",row.retired," armed=",row.armed," immutable=",row.immutable," published=",row.published," durable=",row.durable,
                " callback_pending=",row.callback_pending," callback_delivered=",row.callback_delivered,
                " counters_saturated=",row.counters_saturated," pending_page=",row.pending_page," busy=",row.busy," exit_observed=",row.exit_observed,
                " platform_enabled=",row.platform_enabled," clear_after_copy=",row.clear_after_copy,"\n");
        }
        ::fast_io::io::print(output,"page total=",value.total," next=",value.next," more=",value.more,"\n");
    }

    /** @brief Capture bounded first pages for a full inspection-only Core Dump.
     * The caller holds the cooperative stop. Each page uses the installed
     * permission-checked metadata hook and its own process-state read guard.
     */
    inline constexpr ::std::uint32_t observation_schema{0x52534331u};
    [[nodiscard]] inline bool capture_observation(::std::uint64_t stop,
        ::uwvm2::utils::container::std_vector<::std::byte>& output)
    {
        if(!stop)return false;
        ::uwvm2::utils::container::std_vector<::std::byte> blob;
        auto put=[&](::std::uint64_t value,unsigned bytes)
        {
            for(unsigned i{};i<bytes;++i)
            {
                blob.push_back(static_cast<::std::byte>(value & 255u));
                value>>=8u;
            }
        };
        constexpr char magic[8]{'U','W','S','C','N','1',0,0};
        for(char c:magic)blob.push_back(static_cast<::std::byte>(c));
        put(stop,8u);
        bool selected{};
        for(auto choice:{selection::authority,selection::resources,selection::audit})
        {
            request query{};query.selected=choice;query.stop=stop;
            view page{};read(query,page);
            if(page.selected!=choice)return false;
            if(page.result==status::ok)
            {
                selected|=choice==selection::audit ? page.audit.configured : page.total!=0u;
                // Live labels can be marked truncated. Only publish pages the
                // shared wire reader can represent completely.
                if(choice!=selection::audit)
                {
                    bool valid_page=page.size<=page.rows.size();
                    for(::std::size_t i{};valid_page && i<page.size;++i)
                    {
                        auto const& row=page.rows[i];
                        valid_page=data::valid(row) && !row.environment.truncated && !row.alias.truncated &&
                            !row.resource.truncated && !row.binding.truncated && !row.type.truncated &&
                            !row.state.truncated && !row.platform.truncated;
                    }
                    if(!valid_page)page.result=status::resource_limit;
                }
            }
            else if(page.result!=status::busy && page.result!=status::resource_limit)return false;
            ::uwvm2::utils::container::std_string packet;
            ::uwvm2::utils::container::ostring_ref sink{&packet};
            ::uwvm2::uwvm::debugger::scene_state::print(sink,page,stop);
            if(packet.size()>65536u)return false;
            put(packet.size(),4u);
            auto const* first=reinterpret_cast<::std::byte const*>(packet.data());
            blob.insert(blob.end(),first,first+packet.size());
        }
        if(!selected)return false;
        output.swap(blob);
        return true;
    }
}
