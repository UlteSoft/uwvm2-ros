/** @file
 * @brief Read-only validation of provisioned directional anonymous pipes.
 * @details This helper creates no endpoint and changes no descriptor flags.
 * Exclusivity and opposite-end custody remain deployment responsibilities.
 */
#pragma once
#include <algorithm>
#include <array>
#include <charconv>
#include <string_view>
#if defined(__linux__)
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
namespace uwvm2::imported::ros::scene::communication_pipe
{
    inline bool anonymous(int fd,bool reading,struct stat& information)noexcept
    {
        auto flags=::fcntl(fd,F_GETFL);
        if(fd<0||flags<0||::fstat(fd,&information)||!S_ISFIFO(information.st_mode)||
           (flags&O_ACCMODE)!=(reading?O_RDONLY:O_WRONLY)||!(flags&O_NONBLOCK))return false;
        std::array<char,64> path{};constexpr std::string_view prefix="/proc/self/fd/";
        std::copy(prefix.begin(),prefix.end(),path.begin());
        auto converted=std::to_chars(path.data()+prefix.size(),path.data()+path.size()-1,fd);
        if(converted.ec!=std::errc{})return false;
        std::array<char,128> target{};auto size=::readlink(path.data(),target.data(),target.size());
        if(size<8||static_cast<std::size_t>(size)>=target.size())return false;
        std::string_view value{target.data(),static_cast<std::size_t>(size)};
        return value.starts_with("pipe:[")&&value.back()==']'&&
            value.substr(6,value.size()-7).find_first_not_of("0123456789")==std::string_view::npos;
    }
}
#endif
