#include <uwvm2/extension/memtag/color.h>
#include <cstdio>
#include <cstdlib>
namespace mt=uwvm2::extension::memtag;
int main()
{
    for(unsigned tag{};tag!=256u;++tag)
    {
        auto const color{mt::color_for_tag(tag)};
        auto const hex{mt::hex_color_for_tag(tag)};
        auto const ansi{mt::ansi_color_for_tag(tag)};
        char expected[32]{};
        auto count{std::snprintf(expected,sizeof expected,"\033[38;2;%u;%u;%um",color.red,color.green,color.blue)};
        if(count<0 || static_cast<unsigned>(count)!=ansi.size || hex[0]!='#') {std::abort();}
        for(unsigned index{};index!=ansi.size;++index)
        {if(ansi.bytes[index]!=static_cast<char8_t>(expected[index])){std::abort();}}
        for(unsigned other{};other!=tag;++other)
        {if(mt::hex_color_for_tag(other)==hex){std::abort();}}
    }
}
