#include <array>
#include <cstdio>
#include <cstdlib>
#include <uwvm2/imported/wasi/wasip1/func/path_open_host.h>

int main()
{
#if defined(__linux__) || defined(__FreeBSD__) || (defined(__APPLE__) && defined(__MACH__))
    using mode=fast_io::open_mode;
    constexpr std::array<mode,11> emitted{mode::creat,mode::directory,mode::excl,mode::trunc,
        mode::app,mode::no_block,mode::sync,mode::dsync,mode::rsync,mode::in,mode::out};
    for(unsigned mask{};mask!=(1u<<emitted.size());++mask)
    {
        auto value=mode::explicit_disposition;
        for(unsigned bit{};bit!=emitted.size();++bit) { if(mask&(1u<<bit)) { value|=emitted[bit]; } }
        auto const actual=uwvm2::imported::wasi::wasip1::func::path_open_explicit_posix_flags(value);
        // Header-only oracle: production modules cannot name this private helper.
        auto const original=fast_io::details::calculate_posix_open_mode(value);
        if(actual!=original)
        {
            std::fprintf(stderr,"mode-set %u: native flags %x, expected %x\n",mask,actual,original);
            std::abort();
        }
    }
    std::puts("PASS all2048 emitted explicit path_open mode combinations match original POSIX flags");
#endif
}
