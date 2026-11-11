/*************************************************************
 * UWVM2 read-only ROS debugger commands. APL-2.0.         *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <uwvm2/imported/ros/common/debug.h>
# include <fast_io.h>
# include <fast_io_dsal/array.h>
# include <fast_io_dsal/string_view.h>
# include <initializer_list>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::ros_state
{
    namespace data=::uwvm2::imported::ros::debug;
    using request=data::request;using view=data::view;using selection=data::selection;using status=data::status;
    inline bool valid(request const& query) noexcept {return data::valid(query);}
    inline void read(request const& query,view& output) noexcept {data::snapshot(query,output);}
    inline ::fast_io::string_view error_text(::std::uint32_t error) noexcept
    {
        // Stable ROS Host ABI categories only. Arbitrary gateway detail text
        // can contain payload, connection secrets or native filesystem names.
        switch(error)
        {
            case 0:return "success";case 2:return "unknown_module";case 9:return "bad_handle";
            case 11:return "rate_limited";case 13:return "permission_denied";case 17:return "conflict";
            case 22:return "invalid_argument";case 90:return "too_large";case 95:return "unsupported";
            case 107:return "disconnected";case 110:return "timeout";case 125:return "revoked";
        }
        return "unclassified";
    }
    inline ::fast_io::string_view status_text(status value) noexcept
    {
        switch(value)
        {
            case status::ok:return "available";case status::invalid_request:return "invalid request";
            case status::not_selected:return "ROS Host not selected";case status::busy:return "busy (snapshot unavailable)";
            case status::requires_cooperative_stop:return "requires current cooperative stop";
            case status::stale_stop:return "stale stop";case status::resource_limit:return "bounded scan limit";
        }
        return "invalid status";
    }
    inline ::fast_io::string_view selection_text(selection value) noexcept
    {
        switch(value)
        {
            case selection::capabilities:return "capabilities";case selection::sessions:return "sessions";
            case selection::queues:return "queues";case selection::services:return "services";case selection::actions:return "actions";
            case selection::parameters:return "parameters";case selection::tf:return "tf";case selection::graph:return "graph";
            case selection::statistics:return "stats";case selection::calls:return "calls";case selection::trace:return "trace";
            case selection::policy:return "policy";
        }
        return "invalid";
    }
    inline bool parse_selection(::fast_io::string_view text,selection& value) noexcept
    {
        for(auto item:{selection::capabilities,selection::sessions,selection::queues,selection::services,selection::actions,
            selection::parameters,selection::tf,selection::graph,selection::statistics,selection::calls,selection::policy})
        {if(selection_text(item)==text) {value=item;return true;}}
        return false;
    }
    inline ::fast_io::string_view policy_scope_text(data::policy_scope value) noexcept
    {
        switch(value)
        {
            case data::policy_scope::all:return "all";case data::policy_scope::global:return "global";
            case data::policy_scope::single:return "single";case data::policy_scope::group:return "group";
        }
        return "invalid";
    }
    inline ::fast_io::string_view policy_kind_text(data::policy_kind value) noexcept
    {
        switch(value)
        {
            case data::policy_kind::target:return "target";case data::policy_kind::binding:return "binding";
            case data::policy_kind::interface:return "interface";case data::policy_kind::capability:return "capability";
            case data::policy_kind::limit:return "limit";case data::policy_kind::connection:return "connection";
        }
        return "invalid";
    }
    // This bounded HOST grammar accepts DATA filters only. No command can open,
    // revoke, refresh, resume or call a native ROS operation. Diagnostic trace
    // collection has an independent host-only on/off control.
    inline bool parse(::fast_io::string_view line,request& output) noexcept
    {
        ::fast_io::array<::fast_io::string_view,16> words{};::std::size_t count{},cursor{};
        while(cursor<line.size())
        {
            while(cursor<line.size()&&::fast_io::char_category::c_space::char_is(line[cursor])) {++cursor;}
            if(cursor==line.size()) {break;}if(count==words.size()) {return false;}auto first{cursor};
            while(cursor<line.size()&&!::fast_io::char_category::c_space::char_is(line[cursor]))
            {if(!::fast_io::char_category::is_c_graph(static_cast<unsigned char>(line[cursor]))) {return false;}++cursor;}
            words[count++]=line.subview(first,cursor-first);
        }
        if(count<2||words[1]!="ros"||(words[0]!="info"&&words[0]!="trace")) {return false;}
        request query{};::std::size_t first{2};
        if(words[0]=="trace") {query.selected=selection::trace;}
        else {if(count<3||!parse_selection(words[2],query.selected)) {return false;}first=3;}
        if(query.selected==selection::policy) {query.count=data::maximum_policy_rows;}
        if(query.selected==selection::trace&&count>2&&(words[2]=="on"||words[2]=="off"))
        {query.trace_control=words[2]=="on"?1u:2u;first=3;}
        unsigned seen{};
        for(auto i{first};i<count;++i)
        {
            auto token{words[i]};auto equals{token.find_character('=')};
            if(equals==::fast_io::containers::npos||equals==0||equals+1==token.size()) {return false;}
            auto key{token.subview(0,equals)},value{token.subview(equals+1)};unsigned bit{};
            if(key=="backend") {bit=1;if(value=="all") {query.backend=0;}else if(value=="ros1") {query.backend=1;}else if(value=="ros2") {query.backend=2;}else {return false;}}
            else if(key=="alias") {bit=2;query.alias.assign({value.data(),value.size()});}
            else if(key=="module") {bit=4;query.module.assign({value.data(),value.size()});}
            else if(key=="scope")
            {
                bit=2048;if(query.selected!=selection::policy) {return false;}
                if(value=="all") {query.scope=data::policy_scope::all;}
                else if(value=="global") {query.scope=data::policy_scope::global;}
                else if(value=="single") {query.scope=data::policy_scope::single;}
                else if(value=="group") {query.scope=data::policy_scope::group;}
                else {return false;}
            }
            else if(key=="target")
            {bit=4096;if(query.selected!=selection::policy) {return false;}query.target.assign({value.data(),value.size()});}
            else if(key=="target-hex"||key=="module-hex")
            {
                bit=key=="target-hex"?4096:4;if(query.selected!=selection::policy) {return false;}
                auto& text{key=="target-hex"?query.target:query.module};
                if(value.size()%2||value.size()/2>text.bytes.size()) {return false;}
                auto digit=[](char c) noexcept -> int
                {return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:c>='A'&&c<='F'?c-'A'+10:-1;};
                for(::std::size_t j{};j<value.size();j+=2)
                {
                    int const high{digit(value[j])},low{digit(value[j+1])};if(high<0||low<0) {return false;}
                    text.bytes[text.size++]=static_cast<char>(high*16+low);
                }
            }
            else if(key=="function"||key=="op")
            {
                if(query.selected!=selection::trace&&query.selected!=selection::calls) {return false;}
                bit=key=="function"?256:512;auto& text{key=="function"?query.function:query.operation};
                text.assign({value.data(),value.size()});
            }
            else
            {
                ::std::uint64_t number{};auto parsed{::fast_io::parse_by_scan(value.cbegin(),value.cend(),::fast_io::mnp::dec_get<true,true>(number))};
                if(parsed.code!=::fast_io::parse_code::ok||parsed.iter!=value.cend()) {return false;}
                if(key=="instance") {bit=8;query.instance=number;}
                else if(key=="first") {bit=16;if(query.selected==selection::trace) {return false;}query.first=number;}
                else if(key=="count") {bit=32;query.count=number;}
                else if(key=="after") {bit=64;if(query.selected!=selection::trace) {return false;}query.after=number;}
                else if(key=="error") {bit=128;if(number>UINT32_MAX) {return false;}query.error=static_cast<::std::uint32_t>(number);query.error_filter=true;}
                else if(key=="if-stop") {bit=1024;if(!number) {return false;}query.stop=number;}
                else {return false;}
            }
            if(seen&bit) {return false;}seen|=bit;
        }
        if(!data::valid(query)) {return false;}output=query;return true;
    }
    template<typename Output,::std::size_t N> inline void quoted(Output& output,data::text<N> const& text)
    {
        if(text.size>text.bytes.size()) {::fast_io::io::print(output,"\"<invalid>\"");return;}
        ::fast_io::io::print(output,"\"");
        constexpr char digits[]{"0123456789abcdef"};
        for(char c:text.view())
        {
            auto byte{static_cast<unsigned char>(c)};
            if(byte>=32&&byte<127&&c!='"'&&c!='\\') {::fast_io::io::print(output,::fast_io::mnp::chvw(c));}
            else {char escaped[]{'\\','x',digits[byte>>4],digits[byte&15]};::fast_io::io::print(output,::std::string_view{escaped,sizeof escaped});}
        }
        ::fast_io::io::print(output,"\"",text.truncated ? ::std::string_view{"[truncated]"} : ::std::string_view{});
    }
    template<typename Output> inline void print(Output& output,view const& value,::std::uint64_t stop)
    {
        ::fast_io::io::print(output,"ros scope=process-owned stop=",::fast_io::mnp::dec(stop)," selection=",selection_text(value.selected)," status=",status_text(value.result),"\n");
        if(value.result!=status::ok) {return;}
        if(value.policy_size>value.policy.size()||
           (value.selected==selection::policy?(value.size!=0||value.interface_size!=0||value.trace_size!=0):value.policy_size!=0)||
           value.interface_size>value.interfaces.size()||
           (value.selected==selection::calls?(value.size!=0||value.trace_size!=0):value.interface_size!=0)||
           value.size>value.rows.size()||value.trace_size>value.trace.size()||
           (value.selected==selection::trace?value.size!=0:value.trace_size!=0))
        {::fast_io::io::print(output,"invalid bounded snapshot\n");return;}
        for(::std::size_t i{};i<value.policy_size;++i)
        {
            auto const& row{value.policy[i]};
            auto complete=[](auto const& text) noexcept {return text.size<=text.bytes.size()&&!text.truncated;};
            if(row.kind>data::policy_kind::connection||row.scope<data::policy_scope::global||row.scope>data::policy_scope::group||
               row.backend<1||row.backend>2||row.value_offset>row.value_total||row.value.size>row.value_total-row.value_offset||!complete(row.target)||!complete(row.module)||!complete(row.alias)||!complete(row.key)||!complete(row.value))
            {::fast_io::io::print(output,"invalid bounded policy snapshot\n");return;}
        }
        for(::std::size_t i{};i<value.policy_size;++i)
        {
            auto const& row{value.policy[i]};
            ::fast_io::io::print(output,"policy kind=",policy_kind_text(row.kind)," backend=ros",::fast_io::mnp::dec(row.backend),
                " scope=",policy_scope_text(row.scope)," target=");quoted(output,row.target);
            ::fast_io::io::print(output," module=");quoted(output,row.module);::fast_io::io::print(output," alias=");quoted(output,row.alias);
            ::fast_io::io::print(output," key=");quoted(output,row.key);::fast_io::io::print(output," value=");
            if(row.redacted) {::fast_io::io::print(output,"\"<redacted>\"");}else {quoted(output,row.value);}
            ::fast_io::io::print(output," value_offset=",::fast_io::mnp::dec(row.value_offset)," value_total=",::fast_io::mnp::dec(row.value_total)," configured=",row.configured," enabled=",row.enabled," redacted=",row.redacted,"\n");
        }
        for(::std::size_t i{};i<value.size;++i)
        {
            auto const& row{value.rows[i]};
            ::fast_io::io::print(output,"row kind=",::fast_io::mnp::dec(static_cast<unsigned>(row.kind))," backend=ros",::fast_io::mnp::dec(row.backend),
                " instance=",::fast_io::mnp::dec(row.instance)," generation=",::fast_io::mnp::dec(row.generation)," module=");quoted(output,row.module);
            ::fast_io::io::print(output," alias=");quoted(output,row.alias);::fast_io::io::print(output," op=");quoted(output,row.operation);
            ::fast_io::io::print(output," resource=");if(row.resource_redacted) {::fast_io::io::print(output,"<redacted>");}else {quoted(output,row.resource);}
            ::fast_io::io::print(output," type=");quoted(output,row.type);
            ::fast_io::io::print(output," enabled=",row.enabled," alive=",row.alive," revoked=",row.revoked," connected=",row.connected," retired=",row.retired,
                " opens=",::fast_io::mnp::dec(row.open_count)," active_handles=",::fast_io::mnp::dec(row.active_handles),
                " observations=",::fast_io::mnp::dec(row.observations)," requests=",::fast_io::mnp::dec(row.requests)," delivered=",::fast_io::mnp::dec(row.delivered),
                " stored_tokens=",row.operation_tokens," stored_creation_tokens=",row.creation_tokens," stored_byte_tokens=",row.byte_tokens,
                " audit_records=",::fast_io::mnp::dec(row.audit_records)," audit_bytes=",::fast_io::mnp::dec(row.audit_bytes)," audit_suppressed=",::fast_io::mnp::dec(row.audit_suppressed),
                " depth=",::fast_io::mnp::dec(row.depth)," bytes=",::fast_io::mnp::dec(row.bytes)," pending=",::fast_io::mnp::dec(row.pending_depth)," pending_bytes=",::fast_io::mnp::dec(row.pending_bytes),
                " reserved=",::fast_io::mnp::dec(row.reserved_depth)," reserved_bytes=",::fast_io::mnp::dec(row.reserved_bytes)," high_water=",::fast_io::mnp::dec(row.high_water_depth),
                " high_water_bytes=",::fast_io::mnp::dec(row.high_water_bytes)," enqueued=",::fast_io::mnp::dec(row.enqueued)," dequeued=",::fast_io::mnp::dec(row.dequeued),
                " drop_oldest=",::fast_io::mnp::dec(row.dropped_oldest)," drop_newest=",::fast_io::mnp::dec(row.dropped_newest)," rejected=",::fast_io::mnp::dec(row.rejected),
                " backpressured=",::fast_io::mnp::dec(row.backpressured)," admission_refused=",::fast_io::mnp::dec(row.admission_refused)," completed=",::fast_io::mnp::dec(row.completed)," closed=",::fast_io::mnp::dec(row.closed));
            if(row.kind==data::row_kind::session&&row.client_bindings)
            { ::fast_io::io::print(output," client_bindings=",::fast_io::mnp::dec(row.client_bindings)," client_session_label=",::fast_io::mnp::dec(row.client_session_label)); }
            ::fast_io::io::print(output,"\n");
        }
        for(::std::size_t i{};i<value.interface_size;++i)
        {
            auto const& row{value.interfaces[i]};
            ::fast_io::io::print(output,"interface backend=ros",::fast_io::mnp::dec(row.backend)," instance=",::fast_io::mnp::dec(row.instance),
                " generation=",::fast_io::mnp::dec(row.generation)," module=");quoted(output,row.module);
            ::fast_io::io::print(output," alias=");quoted(output,row.alias);::fast_io::io::print(output," op=");quoted(output,row.operation);
            ::fast_io::io::print(output," function=");quoted(output,row.function);
            ::fast_io::io::print(output," calls=",::fast_io::mnp::dec(row.calls)," errors=",::fast_io::mnp::dec(row.errors)," incomplete=",::fast_io::mnp::dec(row.incomplete),
                " timed=",::fast_io::mnp::dec(row.timed)," total_ns=",::fast_io::mnp::dec(row.total_ns)," min_ns=",::fast_io::mnp::dec(row.min_ns),
                " max_ns=",::fast_io::mnp::dec(row.max_ns)," window_ns=",::fast_io::mnp::dec(row.window_ns),
                " unresolved=",row.unresolved," saturated=",row.saturated,"\n");
        }
        for(::std::size_t i{};i<value.trace_size;++i)
        {
            auto const& row{value.trace[i]};::fast_io::io::print(output,"call seq=",::fast_io::mnp::dec(row.sequence)," ns=",::fast_io::mnp::dec(row.monotonic_ns),
                " backend=ros",::fast_io::mnp::dec(row.backend)," instance=",::fast_io::mnp::dec(row.instance)," generation=",::fast_io::mnp::dec(row.generation)," module=");quoted(output,row.module);
            ::fast_io::io::print(output," function=");quoted(output,row.function);::fast_io::io::print(output," op=");quoted(output,row.operation);
            ::fast_io::io::print(output," alias=");quoted(output,row.alias);
            ::fast_io::io::print(output," resource=");
            if(row.resource.size>row.resource.bytes.size()) { ::fast_io::io::print(output,"<invalid>"); }
            else if(row.resource_redacted || (!row.resource.view().empty() && !data::public_resource(row.resource.view())))
            { ::fast_io::io::print(output,"<redacted>"); }
            else { quoted(output,row.resource); }
            if(row.source_alias.size)
            {
                ::fast_io::io::print(output," source_alias=");quoted(output,row.source_alias);
                ::fast_io::io::print(output," client_session_label=",::fast_io::mnp::dec(row.client_session_label));
                if(row.client_features_observed) { ::fast_io::io::print(output," client_features=",::fast_io::mnp::dec(row.client_features)); }
            }
            ::fast_io::io::print(output," errno=",::fast_io::mnp::dec(row.error)," reason=",
                row.result_known ? error_text(row.error) : ::fast_io::string_view{"unobserved"},
                " result_known=",row.result_known," unresolved=",row.unresolved,"\n");
        }
        ::fast_io::io::print(output,"total=",::fast_io::mnp::dec(value.total)," next=",::fast_io::mnp::dec(value.next)," more=",value.more,
            " gap=",value.gap," overwritten=",::fast_io::mnp::dec(value.overwritten)," dropped_busy=",::fast_io::mnp::dec(value.dropped_busy),
            " dropped_sequence=",::fast_io::mnp::dec(value.dropped_sequence));
        if(value.selected==selection::calls)
        {::fast_io::io::print(output," dropped_full=",::fast_io::mnp::dec(value.dropped_full)," dropped_labels=",::fast_io::mnp::dec(value.dropped_labels));}
        ::fast_io::io::print(output,"\n");
    }
}
