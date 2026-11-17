/** @file @brief Cold, conservative system thread ceiling and exact f32 attenuation.
 * @details Linux uses the minimum of kernel threads-max, the finite soft
 * RLIMIT_NPROC, and every visible cgroup-v2 ancestor pids.max. These are ceilings,
 * not an available-thread guarantee: other processes, memory/stack limits and
 * native creation can still reject admission. The snapshot is immutable after
 * CLI configuration; changing OS limits never silently expands authority.
 * Unsupported or unobservable system ceilings reject ratio configuration.
 */
#pragma once
#include "wasithread.h"
#include <fast_io.h>
#include <array>
#include <bit>
#include <limits>
#if defined(__linux__)
#include <cerrno>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/vfs.h>
#include <unistd.h>
#endif
namespace uwvm2::utils::control::wasithread
{
    /** @brief Parse exactly one binary32 value using fast_io; reject NaN, infinity and out-of-range values. */
    [[nodiscard]] inline bool parse_ratio(::std::u8string_view input,float& out) noexcept
    {
        static_assert(sizeof(float)==4&&::std::numeric_limits<float>::is_iec559&&::std::numeric_limits<float>::digits==24);
        if(input.empty())return false;
        float value{};auto result=::fast_io::parse_by_scan(input.data(),input.data()+input.size(),value);
        if(result.code!=::fast_io::parse_code::ok||result.iter!=input.data()+input.size()||!(value>=0.0f&&value<=1.0f))return false;
        out=value;return true;
    }
    /** @brief floor(ceiling * ratio), using the exact parsed f32 rather than rounded floating multiplication.
     * @details The 64-by-24-bit product is represented by two integer limbs.
     * No uint128 extension, host rounding-mode dependency or upward rounding is
     * required, including for ceilings above 2^53 and subnormal ratios.
     */
    [[nodiscard]] inline bool scale_limit(::std::uint64_t ceiling,float ratio,::std::uint64_t& out) noexcept
    {
        if(!(ratio>=0.0f&&ratio<=1.0f)||ceiling==unlimited)return false;
        auto bits=::std::bit_cast<::std::uint32_t>(ratio)&0x7fffffffu;
        auto exponent=bits>>23u;auto fraction=bits&0x7fffffu;
        if(exponent)fraction|=0x800000u;
        auto shift=exponent?150u-exponent:149u;
        ::std::uint64_t a=(ceiling&0xffffffffu)*fraction,b=(ceiling>>32u)*fraction;
        ::std::uint64_t low=a+(b<<32u),high=(b>>32u)+(low<a);
        out=shift>=128u?0u:shift>=64u?high>>(shift-64u):(low>>shift)|(high<<(64u-shift));
        return true;
    }
    struct system_ceiling
    {
        bool available{};
        ::std::uint64_t effective{},kernel{},process{unlimited},cgroup{unlimited};
        ::std::size_t cgroup_limits{};
    };
#if defined(__linux__)
    namespace system_details
    {
        // Kernel control files have small scalar values. Paths are bounded by
        // Linux PATH_MAX; reject truncation rather than infer a larger ceiling.
        template<::std::size_t Size>
        bool read(char const* path,::std::array<char,Size>& buffer,::std::string_view& out,bool& missing) noexcept
        {
            missing=false;int fd;
            do{fd=::open(path,O_RDONLY|O_CLOEXEC|O_NOFOLLOW);}while(fd<0&&errno==EINTR);
            if(fd<0){missing=errno==ENOENT;return false;}
            ::std::size_t used{};bool okay=true;
            while(used<Size)
            {
                auto count=::read(fd,buffer.data()+used,Size-used);
                if(count==0)break;
                if(count<0){if(errno==EINTR)continue;okay=false;break;}
                used+=static_cast<::std::size_t>(count);
            }
            ::close(fd);if(!okay||used==Size)return false;
            out={buffer.data(),used};return true;
        }
        inline bool number(::std::string_view text,::std::uint64_t& value) noexcept
        {
            if(!text.empty()&&text.back()=='\n')text.remove_suffix(1);
            if(text=="max"){value=unlimited;return true;}
            return parse_limit({reinterpret_cast<char8_t const*>(text.data()),text.size()},value);
        }
    }
#endif
    /** @brief Observe the actual host ceiling; no resources, permissions or cgroups are changed. */
    [[nodiscard]] inline system_ceiling inspect_system_ceiling() noexcept
    {
        system_ceiling result;
#if defined(__linux__)
        using namespace system_details;
        ::std::array<char,128> scalar{};::std::string_view text;bool missing{};
        if(!read("/proc/sys/kernel/threads-max",scalar,text,missing)||!number(text,result.kernel)||
            result.kernel==0u||result.kernel==unlimited)return result;
        ::rlimit per_user{};if(::getrlimit(RLIMIT_NPROC,&per_user)!=0)return result;
        if(per_user.rlim_cur!=RLIM_INFINITY)result.process=static_cast<::std::uint64_t>(per_user.rlim_cur);
        struct ::statfs filesystem{};if(::statfs("/sys/fs/cgroup",&filesystem)!=0||filesystem.f_type!=0x63677270)return result;
        ::std::array<char,4096> membership{};
        if(!read("/proc/self/cgroup",membership,text,missing)||!text.starts_with("0::/")||!text.ends_with('\n'))return result;
        text.remove_prefix(3);text.remove_suffix(1);
        if(text.find('\n')!=text.npos||text.find('\0')!=text.npos||text.find("/../")!=text.npos||text.ends_with("/.."))return result;
        ::std::array<char,4128> path{};constexpr ::std::string_view root="/sys/fs/cgroup";
        if(text.size()+root.size()+sizeof("/pids.max")>=path.size())return result;
        ::std::memcpy(path.data(),root.data(),root.size());::std::memcpy(path.data()+root.size(),text.data(),text.size());
        auto size=root.size()+text.size();if(path[size-1u]=='/')--size;
        path[size]='\0';
        int directory=::open(path.data(),O_RDONLY|O_DIRECTORY|O_CLOEXEC);
        if(directory<0)return result;
        struct ::statfs actual{};bool same=::fstatfs(directory,&actual)==0&&actual.f_type==filesystem.f_type;
        ::close(directory);if(!same)return result;
        for(;;)
        {
            ::std::memcpy(path.data()+size,"/pids.max",sizeof("/pids.max"));
            ::std::uint64_t limit{};
            if(read(path.data(),scalar,text,missing))
            {
                if(!number(text,limit))return result;
                if(limit<result.cgroup)result.cgroup=limit;
                ++result.cgroup_limits;
            }
            else if(!missing)return result;
            if(size==root.size())break;
            while(size>root.size()&&path[size-1u]!='/')--size;
            if(size>root.size())--size;
        }
        result.effective=result.kernel;
        if(result.process<result.effective)result.effective=result.process;
        if(result.cgroup<result.effective)result.effective=result.cgroup;
        result.available=true;
#endif
        return result;
    }
}
