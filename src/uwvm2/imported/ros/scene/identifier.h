/** @file @brief Canonical ASCII identifier shared by scene policy and diagnostic records. */
#pragma once
#include <string_view>
extern "C++" {
namespace uwvm2::imported::ros::scene
{
    inline bool identifier(std::string_view name) noexcept
    {
        if(name.empty()||name.size()>255||name=="."||name=="..")return false;
        for(auto c:name)if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='-'||c=='.'))return false;
        return true;
    }
}
}
