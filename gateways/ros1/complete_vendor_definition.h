/*************************************************************
 * UWVM2 complete ROS1 generated Definition proof. APL-2.0.  *
 *************************************************************/
#pragma once
#include <array>
#include <cstddef>
#include <string_view>
#include <uwvm2/imported/ros/ipc/complete_vendor_domain.h>

namespace uwvm2::ros_gateway::ros1_complete_vendor
{
    inline bool space(char c) noexcept
    {return c==' '||c=='\t'||c=='\r'||c=='\f'||c=='\v';}
    inline ::std::string_view trim(::std::string_view text) noexcept
    {
        while(!text.empty()&&space(text.front()))text.remove_prefix(1);
        while(!text.empty()&&space(text.back()))text.remove_suffix(1);
        return text;
    }
    /** Match the complete source parser's quoted-comment grammar. A '#' inside
     * an original quoted string is data. Escapes are retained for exact source
     * comparison; no pointer, allocation, locale or permissive numeric parser
     * is involved in this startup-only check. */
    inline bool next_line(::std::string_view& input,::std::string_view& line,bool& valid) noexcept
    {
        while(!input.empty())
        {
            auto end=input.find('\n');line=input.substr(0,end);
            input.remove_prefix(end==::std::string_view::npos?input.size():end+1);
            char quote{};bool escaped{};
            for(::std::size_t i{};i!=line.size();++i)
            {
                char c=line[i];
                if(escaped)escaped=false;
                else if(quote&&c=='\\')escaped=true;
                else if(quote){if(c==quote)quote=0;}
                else if(c=='\''||c=='"')quote=c;
                else if(c=='#'){line=line.substr(0,i);break;}
            }
            if(quote){valid=false;return false;}
            line=trim(line);if(!line.empty())return true;
        }
        return false;
    }
    inline bool identifier(::std::string_view name,bool constant=false) noexcept
    {
        if(name.empty()||name.size()>128)return false;
        if(!((name.front()>='A'&&name.front()<='Z')||
             (!constant&&name.front()>='a'&&name.front()<='z')))return false;
        for(char c:name)
            if(!((c>='A'&&c<='Z')||(!constant&&c>='a'&&c<='z')||
                 (c>='0'&&c<='9')||c=='_'))return false;
        return true;
    }
    inline bool split(::std::string_view line,::std::string_view& type,
                      ::std::string_view& name,::std::string_view& value,bool& constant) noexcept
    {
        ::std::size_t end{};while(end<line.size()&&!space(line[end]))++end;
        if(!end||end==line.size())return false;
        type=line.substr(0,end);auto rest=trim(line.substr(end));auto equal=rest.find('=');
        constant=equal!=::std::string_view::npos;
        if(constant)
        {
            name=trim(rest.substr(0,equal));value=trim(rest.substr(equal+1));
            if(!identifier(name,true)||value.empty())return false;
            for(auto scalar:{"bool","int8","uint8","int16","uint16","int32","uint32", "int64","uint64","float32","float64","string","char","byte"})
                if(type==scalar)return true;
            return false;
        }
        name=rest;value={};return identifier(name);
    }
    /** Constant values are also checked through their actual generated native
     * C++ members. Here the complete declaration literal is additionally exact
     * after outside whitespace/comments; unknown textual rewrites fail closed.
     * Fields normalize only ROS1's documented primitive/Header/package aliases.
     */
    template<class SameField>
    inline bool same(::std::string_view native,::std::string_view expected,
                     ::std::string_view section,SameField same_field) noexcept
    {
        ::std::string_view at,an,av,bt,bn,bv;bool ac{},bc{};
        if(!split(native,at,an,av,ac)||!split(expected,bt,bn,bv,bc)||ac!=bc)return false;
        if(ac)return at==bt&&an==bn&&av==bv;
        return same_field(native,expected,section);
    }
    inline bool separator(::std::string_view line) noexcept
    {
        if(line.size()<3)return false;
        for(char c:line)if(c!='=')return false;
        return true;
    }
    /** Every dependency must occur exactly once, and every line must match.
     * Native Definition and expected Source are borrowed immutable strings.
     * This function grants no resource and performs no ROS API operation. */
    template<class Entry,class SameField>
    bool matches(::std::string_view root,::std::string_view native,
                 Entry const* expected,::std::size_t count,SameField same_field) noexcept
    {
        namespace ipc=::uwvm2::imported::ros::ipc;
        if(!expected||!count||count>128||expected[0].type!=root||native.size()>16U*1024U*1024U)return false;
        for(::std::size_t i{};i!=count;++i)
        {
            if(!ipc::complete_vendor_native_name(1,expected[i].type)||expected[i].fields.size()>1024U*1024U)return false;
            for(::std::size_t j{};j!=i;++j)if(expected[i].type==expected[j].type)return false;
        }
        ::std::array<bool,128> visited{};visited[0]=true;
        ::std::size_t current{},lines{};bool pending{},valid=true;
        auto remaining=expected[0].fields;::std::string_view line,next;
        while(next_line(native,line,valid))
        {
            if(separator(line))
            {
                if(pending||next_line(remaining,next,valid)||!valid)return false;
                pending=true;continue;
            }
            if(line.substr(0,4)=="MSG:")
            {
                if(!pending)return false;auto dependency=trim(line.substr(4));::std::size_t index=count;
                for(::std::size_t i{1};i!=count;++i)if(expected[i].type==dependency){index=i;break;}
                if(index==count||visited[index])return false;
                visited[index]=true;current=index;remaining=expected[index].fields;pending=false;continue;
            }
            if(pending||++lines>4096||!next_line(remaining,next,valid)||!valid||
               !same(line,next,expected[current].type,same_field))return false;
        }
        if(!valid||pending||next_line(remaining,next,valid)||!valid)return false;
        for(::std::size_t i{};i!=count;++i)if(!visited[i])return false;
        return true;
    }
}
