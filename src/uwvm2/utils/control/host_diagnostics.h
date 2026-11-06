// Host-owned import diagnostics. APL-2.0 (see LICENSE).
#pragma once
#include "host_interfaces.h"
#include <string>
#include <string_view>
#include <array>
#include <cstdint>
#include <vector>
namespace uwvm2::utils::control::host_diagnostics
{
    namespace api=host_interfaces;
    enum class severity : unsigned char { off, warning, error };
    enum class event : unsigned char { denied, limited };
    struct settings
    {
        std::array<severity,api::catalogue.size()> denied{},limited{};
        constexpr settings() { denied.fill(severity::warning); }
    };
    // Configured by host CLI before workers start; guest code has no setter.
    inline settings configuration{};
    inline void (*sink)(std::string_view) noexcept{};
    inline void (*fatal)() noexcept{};
    inline constexpr severity selected(std::size_t id,event kind) noexcept
    { return id<api::invalid?(kind==event::denied?configuration.denied[id]:configuration.limited[id]):severity::off; }
    template<typename Text> bool configure(Text const& input,severity value)
    {
        std::string text;for(auto c:input)text+=static_cast<char>(c);
        bool deny{},limit{};std::string_view selector{text};
        if(selector=="all") { configuration.denied.fill(value);configuration.limited.fill(value);return true; }
        if(selector.starts_with("all_host_calls")){deny=limit=true;selector.remove_prefix(14);}
        else if(selector.starts_with("all_host_denied")){deny=true;selector.remove_prefix(15);}
        else if(selector.starts_with("all_host_limited")){limit=true;selector.remove_prefix(16);}
        else
        {
            if(selector.starts_with("host_call_")){deny=limit=true;selector.remove_prefix(10);}
            else if(selector.starts_with("host_denied_")){deny=true;selector.remove_prefix(12);}
            else if(selector.starts_with("host_limited_")){limit=true;selector.remove_prefix(13);}
            else return false;
            auto dot=selector.find('.');if(dot==selector.npos)return false;
            auto family=selector.substr(0,dot);auto key=selector.substr(dot+1);
            for(std::size_t i{};i<api::invalid;++i)
                if(api::name(api::catalogue[i].group)==family && api::catalogue[i].key==key)
                { if(deny)configuration.denied[i]=value;if(limit)configuration.limited[i]=value;return true; }
            return false;
        }
        if(!selector.empty() && selector!="_wasip1" && selector!="_ros1" && selector!="_ros2")return false;
        for(std::size_t i{};i<api::invalid;++i)
            if(selector.empty() || selector.substr(1)==api::name(api::catalogue[i].group))
            {if(deny)configuration.denied[i]=value;if(limit)configuration.limited[i]=value;}
        return true;
    }
    // Diagnostic text never executes terminal controls, shell substitutions or
    // new lines from module/target names. No input length cap is introduced.
    inline std::string quote_argument(std::string_view value)
    {
        constexpr char digits[]="0123456789abcdef";std::string result{"'"};
        for(unsigned char c:value)
        {
            if(c=='\'')result+="'\\''";
            else if(c<32 || c==127){result+="\\x";result+=digits[c>>4];result+=digits[c&15];}
            else result+=static_cast<char>(c);
        }
        result+='\'';return result;
    }
    template<typename Text> inline std::string_view view(Text const& text) noexcept
    {return {reinterpret_cast<char const*>(text.data()),text.size()};}
    inline std::string command(api::family family,std::string_view scope,std::string_view target,std::string_view action)
    {
        std::string out{"--"};out+=api::name(family);out+='-';out+=scope;out+='-';out+=action;
        if(scope!="global"){out+=' ';out+=quote_argument(target);}return out;
    }
    inline void report(std::size_t id,event kind,std::string_view scope,std::string_view target,
                       std::string_view reason,std::string_view remedy={}) noexcept
    {
        auto level=selected(id,kind);if(level==severity::off)return;
#if defined(__cpp_exceptions)
        try
#endif
        {
            auto const& entry=api::catalogue[id];
            std::string text=level==severity::error?"uwvm: [error] ":"uwvm: [warning] ";
            text+=api::name(entry.group);text+='.';text+=entry.key;text+=" ";text+=kind==event::denied?"denied":"limited";
            text+=" for ";text+=scope;text+=' ';text+=quote_argument(target);text+=": ";text+=reason;
            text+=". Configure: ";text+=remedy.empty()?command(entry.group,scope,target,"interface")+" "+std::string(entry.key)+" allow":std::string(remedy);
            text+=". [host_";text+=kind==event::denied?"denied_":"limited_";text+=api::name(entry.group);text+='.';text+=entry.key;text+="]";
            if(sink)sink(text);
        }
#if defined(__cpp_exceptions)
        catch(...) {} // Logging failures never mutate operation results or rights.
#endif
        if(level==severity::error && fatal)fatal();
    }
    inline void resource_denied(std::size_t id,std::string_view scope,std::string_view target,std::string_view reason) noexcept
    {
        auto level=selected(id,event::denied);if(level==severity::off)return;
#if defined(__cpp_exceptions)
        try
#endif
        {
            auto remedy=command(api::catalogue[id].group,scope,target,"grant")+
                " <alias> <operation> <resource> <type> <trusted-identity>; inspect the contract with --"+
                std::string(api::name(api::catalogue[id].group))+"-dump-effective-policy";
            report(id,event::denied,scope,target,reason,remedy);return;
        }
#if defined(__cpp_exceptions)
        catch(...) {}
#endif
        // Error policy still applies even when the sink cannot allocate.
        if(level==severity::error && fatal)fatal();
    }
    inline void missing(std::size_t id,api::family family,std::string_view scope,std::string_view target,
                        bool created,bool enabled,std::string_view name_space,std::string_view function) noexcept
    {
#if defined(__cpp_exceptions)
        try
#endif
        {
            std::string text{"uwvm: [error] Cannot link import "};text+=quote_argument(name_space);text+='.';text+=quote_argument(function);
            text+=" for ";text+=scope;text+=' ';text+=quote_argument(target);text+=": ";
            if(id==api::invalid)text+="unknown host interface; no permission can enable an unregistered interface";
            else
            {
                text+=enabled?"the interface exposure permission is hidden":"the namespace permission is disabled";
                text+=". Configure: ";
                if(!created && scope!="global"){text+=command(family,scope,target,"create");text+=' ';}
                if(!enabled){text+=command(family,scope,target,"enable");text+=' ';}
                text+=command(family,scope,target,"interface");text+=' ';text+=api::catalogue[id].key;
                text+=" deny (expose and reject calls), or allow (expose and permit calls subject to resource grants)";
            }
            if(sink)sink(text);
        }
#if defined(__cpp_exceptions)
        catch(...) {}
#endif
    }
    struct call_scope;
    inline thread_local call_scope* active{};
    struct call_scope
    {
        struct notice {std::string key,detail;bool wait;};
        std::size_t id;std::string_view scope,target;call_scope* previous{};
        bool observing{},installed{},limit_seen{};std::vector<notice> notices;
        std::string denied_reason;
        call_scope(std::size_t index,std::string_view s,std::string_view t,bool capture_denials=false) noexcept:id(index),scope(s),target(t)
        {
            observing=selected(id,event::limited)!=severity::off || (capture_denials && selected(id,event::denied)!=severity::off);
            installed=observing || active!=nullptr;
            if(installed){previous=active;active=observing?this:nullptr;}
        }
        void flush() noexcept
        {
            if(!limit_seen)return;
            limit_seen=false;
            auto level=selected(id,event::limited);
#if defined(__cpp_exceptions)
            try
#endif
            {
                auto const& entry=api::catalogue[id];std::string reason,remedy;
                for(auto const& notice:notices)
                {
                    if(!reason.empty())reason+="; ";
                    reason+=notice.wait?"synchronously waiting for quota ":"quota rejected: ";reason+=notice.key;
                    if(!notice.detail.empty()){reason+=" (";reason+=notice.detail;reason+=')';}
                    if(!remedy.empty())remedy+="; ";
                    remedy+=command(entry.group,scope,target,entry.group==api::family::wasip1?"limit":"set");
                    remedy+=' ';if(entry.group!=api::family::wasip1){remedy+=notice.detail.empty()?"<capability-alias>":quote_argument(notice.detail);remedy+=' ';}
                    remedy+=notice.key;remedy+=" <approved-limit>";
                }
                notices.clear();
                if(!reason.empty()){report(id,event::limited,scope,target,reason,remedy);return;}
            }
#if defined(__cpp_exceptions)
            catch(...) {notices.clear();}
#endif
            // Allocation/sink failure cannot suppress an explicit fatal policy.
            if(level==severity::error && fatal)fatal();
        }
        ~call_scope(){if(installed)active=previous;flush();}
        call_scope(call_scope const&)=delete;
    };
    inline void limited(std::string_view key,bool wait,std::string_view detail={}) noexcept
    {
        if(!active || selected(active->id,event::limited)==severity::off)return;
        active->limit_seen=true;
#if defined(__cpp_exceptions)
        try
#endif
        {
            // Coalesce the same exhausted field within one attempt, not calls.
            for(auto const& notice:active->notices)
                if(notice.key==key && notice.wait==wait && notice.detail==detail)return;
            active->notices.push_back({std::string(key),std::string(detail),wait});
        }
#if defined(__cpp_exceptions)
        catch(...) {}
#endif
    }
    inline void denial_detail(std::string_view reason) noexcept
    {
        if(!active || selected(active->id,event::denied)==severity::off)return;
#if defined(__cpp_exceptions)
        try
#endif
        {active->denied_reason=reason;}
#if defined(__cpp_exceptions)
        catch(...) {}
#endif
    }
    inline void flush_limits() noexcept {if(active)active->flush();}
}
