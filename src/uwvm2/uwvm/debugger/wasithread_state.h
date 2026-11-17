/** @file @brief Detached, read-only independent I1T authority and accounting. */
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/utils/control/wasithread.h>
#include <uwvm2/utils/control/debug_permissions.h>
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
#include <array>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::wasithread_state
{
    namespace model=::uwvm2::utils::control::wasithread;
    enum class selection { policy, members };
    enum class status { unavailable, ok, forbidden, stale_stop };
    // A bounded reply page is not a limit on policy size or name length.
    inline constexpr ::std::size_t maximum_page=8;
    struct request { selection selected{};::std::size_t first{},count{maximum_page};::std::uint64_t stop{}; };
    struct view
    {
        status result{};selection selected{};::std::size_t first{},total{},size{};bool sealed{};
        ::std::array<model::observation,maximum_page> policies{};
        ::std::array<model::membership,maximum_page> members{};
    };
    inline bool valid(request const& q) noexcept
    {return (q.selected==selection::policy||q.selected==selection::members)&&q.count&&q.count<=maximum_page&&q.first<=SIZE_MAX-q.count;}
    inline bool parse(::fast_io::string_view line,request& result) noexcept
    {
        ::std::array<::fast_io::string_view,6> words{};::std::size_t cursor{},count{};
        while(cursor<line.size())
        {
            while(cursor<line.size()&&line[cursor]==' ')++cursor;
            if(cursor==line.size())break;
            if(count==words.size())return false;auto start=cursor;
            while(cursor<line.size()&&line[cursor]!=' ')++cursor;
            words[count++]=line.subview(start,cursor-start);
        }
        if(count!=6||words[0]!="info"||words[1]!="wasithread")return false;
        request out;
        if(words[2]=="policy")out.selected=selection::policy;
        else if(words[2]=="members")out.selected=selection::members;
        else return false;
        auto number=[](auto text,auto& n) noexcept
        {
            if(text.empty())return false;
            for(auto c:text)if(c<'0'||c>'9')return false;
            auto parsed=::fast_io::parse_by_scan(text.cbegin(),text.cend(),::fast_io::mnp::dec_get<true,true>(n));
            return parsed.code==::fast_io::parse_code::ok&&parsed.iter==text.cend();
        };
        if(!number(words[3],out.first)||!number(words[4],out.count)||!number(words[5],out.stop)||!out.stop||!valid(out))return false;
        result=out;return true;
    }
    inline void read(request const& query,view& out) noexcept
    {
        out={};out.selected=query.selected;out.first=query.first;
        if(!valid(query))return;
        if(!::uwvm2::utils::control::debug_permissions::inspection_allowed()){out.result=status::forbidden;return;}
        auto& owner=model::runtime_policy();model::page page;
        if(query.selected==selection::policy)page=owner.inspect_targets(query.first,{out.policies.data(),query.count});
        else page=owner.inspect_memberships(query.first,{out.members.data(),query.count});
        out.total=page.total;out.size=page.copied;out.sealed=page.sealed;out.result=status::ok;
    }
    inline ::std::string_view name(selection value) noexcept{return value==selection::policy?"policy":"members";}
    inline ::std::string_view name(status value) noexcept
    {switch(value){case status::ok:return "available";case status::forbidden:return "forbidden";case status::stale_stop:return "stale-stop";default:return "unavailable";}}
    template<typename Output> inline void hex_text(Output& out,::std::u8string_view text)
    {
        constexpr char digits[]="0123456789abcdef";
        if(text.empty()){::fast_io::io::print(out,"-");return;}
        for(auto c:text){auto n=static_cast<unsigned char>(c);char pair[]{digits[n>>4],digits[n&15]};::fast_io::io::print(out,::std::string_view{pair,2});}
    }
    template<typename Output> inline void print(Output& out,view const& value,::std::uint64_t stop)
    {
        ::fast_io::io::print(out,"wasithread stop=",stop," selection=",name(value.selected)," status=",name(value.result),"\n");
        if(value.result!=status::ok||value.size>maximum_page)return;
        for(::std::size_t i{};i<value.size;++i)
        {
            if(value.selected==selection::policy)
            {
                auto const& row=value.policies[i];::fast_io::io::print(out,"policy scope=",model::name(row.kind)," target=");hex_text(out,row.target);
                ::fast_io::io::print(out," enabled=",row.enabled," interface=",model::name(row.interface),
                    " mains=",row.occupied.mains," extra=",row.occupied.children," admissions=",row.occupied.spawn_admissions,
                    " max-extra=",row.bound.extra_threads," max-threads=",row.bound.threads," max-spawns=",row.bound.spawns,"\n");
            }
            else
            {
                auto const& row=value.members[i];::fast_io::io::print(out,"member scope=",model::name(row.kind)," target=");hex_text(out,row.target);
                ::fast_io::io::print(out," module=");hex_text(out,row.module);::fast_io::io::print(out,"\n");
            }
        }
        ::fast_io::io::print(out,"page first=",value.first," total=",value.total," count=",value.size," sealed=",value.sealed,"\n");
    }
}
