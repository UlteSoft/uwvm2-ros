// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#ifndef UWVM_MODULE
#include <charconv>
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
#include <uwvm2/utils/debug/host_calls.h>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::host_trace
{
    namespace data=::uwvm2::utils::debug::host_calls;
    enum class action {read,on,off,clear,bytes};
    struct request
    {
        action operation{};
        unsigned channels{7};
        std::uint64_t after{},call{},segment{},offset{},count{8},stop{};
    };
    inline bool valid(request const& q) noexcept
    {
        return q.operation<=action::bytes&&q.channels&&q.channels<=7&&q.count&&
            q.count<=(q.operation==action::bytes?4096u:8u)&&
            (q.operation==action::bytes?q.call!=0:!(q.call||q.segment||q.offset));
    }
    inline bool parse(::fast_io::string_view line,request& out) noexcept
    {
        std::string_view s{line.data(),line.size()};
        auto word=[&]() {
            auto n=s.find_first_not_of(" \t");
            if(n==s.npos) {s={};return std::string_view{};}
            s.remove_prefix(n);n=s.find_first_of(" \t");
            auto w=s.substr(0,n);s=n==s.npos?std::string_view{}:s.substr(n);return w;
        };
        if(word()!="trace"||word()!="host") {return false;}
        request q{};auto op=word();
        if(op=="on") {q.operation=action::on;}
        else if(op=="off") {q.operation=action::off;}
        else if(op=="clear") {q.operation=action::clear;}
        else if(op=="bytes") {q.operation=action::bytes;q.count=256;}
        else if(op!="read") {return false;}
        unsigned seen{};
        for(auto w=word();!w.empty();w=word())
        {
            auto eq=w.find('=');if(eq==w.npos) {return false;}
            auto key=w.substr(0,eq),v=w.substr(eq+1);unsigned bit{};
            if(key=="family")
            {
                bit=1;
                if(v=="all") {q.channels=7;}else if(v=="wasip1") {q.channels=1;}
                else if(v=="ros1") {q.channels=2;}else if(v=="ros2") {q.channels=4;}else {return false;}
            }
            else
            {
                std::uint64_t n{};auto p=std::from_chars(v.data(),v.data()+v.size(),n);
                if(p.ec!=std::errc{}||p.ptr!=v.data()+v.size()) {return false;}
                if(key=="after") {bit=2;q.after=n;}
                else if(key=="call") {bit=4;q.call=n;}
                else if(key=="segment") {bit=8;q.segment=n;}
                else if(key=="offset") {bit=16;q.offset=n;}
                else if(key=="count") {bit=32;q.count=n;}
                else if(key=="if-stop") {bit=64;q.stop=n;if(!n) {return false;}}
                else {return false;}
            }
            if(seen&bit) {return false;}seen|=bit;
        }
        if((q.operation==action::on||q.operation==action::off||q.operation==action::clear)&&(seen&~65u)) {return false;}
        if(q.operation==action::clear&&seen&1u) {return false;}
        if(q.operation==action::bytes&&(seen&3u)) {return false;}
        if(!valid(q)) {return false;}out=q;return true;
    }
    template<typename O> inline void quoted(O& out,std::string_view s)
    {
        constexpr char hex[]="0123456789abcdef";::fast_io::io::print(out,"\"");
        for(unsigned char c:s)
        {
            if(c>=32&&c<127&&c!='"'&&c!='\\') {::fast_io::io::print(out,::fast_io::mnp::chvw(c));}
            else {char b[]{'\\','x',hex[c>>4],hex[c&15]};::fast_io::io::print(out,std::string_view{b,4});}
        }
        ::fast_io::io::print(out,"\"");
    }
    inline std::string apply(request const& q)
    {
        if(!valid(q)) {return "host-trace status=invalid\n";}
        if(q.operation==action::on&&!data::supported) {return "host-trace status=recording-unavailable-no-exceptions\n";}
        if(q.operation==action::on) {data::enabled.fetch_or(q.channels,std::memory_order_relaxed);}
        if(q.operation==action::off) {data::enabled.fetch_and(~q.channels,std::memory_order_relaxed);}
        if(q.operation==action::clear) {data::clear();}
        std::lock_guard lock{data::mutex};
        std::string text;auto out=::fast_io::ostring_ref_std{&text};
        ::fast_io::io::print(out,"host-trace enabled=",data::enabled.load(std::memory_order_relaxed),
            " epoch=",data::epoch," cleared-through=",data::cleared_through," newest=",data::sequence,
            " allocation-failures=",data::allocation_failures.load()," invalid-ranges=",data::invalid_ranges.load(),
            " sequence-failures=",data::sequence_failures.load(),"\n");
        if(q.operation==action::bytes)
        {
            for(auto const& p:data::records)
            {
                auto const& r=*p;if(r.id!=q.call) {continue;}
                if(q.segment==UINT64_MAX)
                {
                    if(q.offset>r.module_name.size()) {return text+"host-bytes status=invalid-offset\n";}
                    auto n=std::min<std::uint64_t>(q.count,r.module_name.size()-q.offset);
                    ::fast_io::io::print(out,"host-label call=",r.id," size=",r.module_name.size()," offset=",q.offset," next=",q.offset+n," value=");
                    quoted(out,std::string_view{r.module_name}.substr(static_cast<std::size_t>(q.offset),static_cast<std::size_t>(n)));
                    ::fast_io::io::print(out,"\n");return text;
                }
                if(q.segment>=r.segments.size()) {return text+"host-bytes status=unknown-segment\n";}
                auto const& s=r.segments[static_cast<std::size_t>(q.segment)];
                if(q.offset>s.bytes.size()) {return text+"host-bytes status=invalid-offset\n";}
                auto n=std::min<std::uint64_t>(q.count,s.bytes.size()-q.offset);
                ::fast_io::io::print(out,"host-bytes call=",r.id," segment=",q.segment," memory=",s.memory,
                    " guest-offset=",s.offset," direction=",std::string_view{s.output?"output":"input"}," observation=",std::string_view{s.snapshot?"snapshot":"transfer"},
                    " size=",s.bytes.size()," offset=",q.offset," next=",q.offset+n," hex=");
                constexpr char hex[]="0123456789abcdef";
                for(std::uint64_t i{};i<n;++i) {auto c=std::to_integer<unsigned char>(s.bytes[static_cast<std::size_t>(q.offset+i)]);char b[]{hex[c>>4],hex[c&15]};::fast_io::io::print(out,std::string_view{b,2});}
                ::fast_io::io::print(out,"\n");return text;
            }
            return text+"host-bytes status=unknown-call\n";
        }
        if(q.operation!=action::read) {return text;}
        std::uint64_t shown{},next=q.after;bool more{};
        for(auto const& p:data::records)
        {
            auto const& r=*p;if(r.id<=q.after||!(q.channels&static_cast<unsigned>(r.family))) {continue;}
            if(shown==q.count) {more=true;break;}++shown;next=r.id;
            ::fast_io::io::print(out,"host-call id=",r.id," family=",static_cast<unsigned>(r.family)," namespace=");quoted(out,r.name_space);
            ::fast_io::io::print(out," function=");quoted(out,r.name);::fast_io::io::print(out," module-name=");quoted(out,std::string_view{r.module_name}.substr(0,255));
            ::fast_io::io::print(out," module-name-bytes=",r.module_name.size()," module-name-truncated=",r.module_name.size()>255);
            ::fast_io::io::print(out," module=",r.module," participant=",r.participant," generation=",r.generation," environment=",r.environment," args=");
            for(std::size_t i{};i<r.count;++i) {if(i) {::fast_io::io::print(out,",");}::fast_io::io::print(out,static_cast<unsigned>(r.widths[i]),":",r.arguments[i]);}
            ::fast_io::io::print(out," returned=",r.returned," has-result=",r.has_result," result=",r.result,
                " timed=",r.timed," duration-ns=",r.duration," segments=",r.segments.size()," incomplete=",r.incomplete,"\n");
        }
        ::fast_io::io::print(out,"host-calls next=",next," more=",more,"\n");return text;
    }
}
