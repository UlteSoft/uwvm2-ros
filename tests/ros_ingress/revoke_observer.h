#pragma once
#include "../ros_ipc/probe_common.h"
#include <array>
#include <charconv>
#include <limits>

struct ros_revoke_observer
{
    std::string directory;
    std::set<std::string> messages;
    bool wrong{};
    explicit ros_revoke_observer(std::string path) : directory{std::move(path)} {}
    static void write_marker(std::string const& path,std::string const& contents)
    {
        ros_probe::write_fixture_file(path,contents);
    }
    /** Read only the trusted writer's unsigned-decimal/LF count protocol.
     * revoke_bridge emits mnp::dec(atomic<unsigned>)+LF. A fixed buffer covers
     * that entire representation; signs, overflow and trailing junk do not
     * acquire a successful observation. Missing, empty or transiently truncated
     * files keep the existing caller-driven wait pending. Never mmap this file:
     * another actor may truncate it while this observer is reading it. */
    bool read_count(unsigned& expected) const
    {
        auto path{directory+"/revoked.count"};
        int file{::open(path.c_str(),O_RDONLY|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK)};
        if(file<0) { return false; }
        std::array<char,std::numeric_limits<unsigned>::digits10+3> bytes{};
        struct stat state{};
        if(::fstat(file,&state)||!S_ISREG(state.st_mode)||state.st_uid!=::geteuid()||
           (state.st_mode&0022)||state.st_size<=0||static_cast<std::uint64_t>(state.st_size)>=bytes.size())
        { (void)::close(file);return false; }
        std::size_t used{};
#if __cplusplus >= 202002L && defined(__cpp_concepts)
        try
        {
            fast_io::posix_file input{fast_io::posix_file_factory{file}};
            while(used<bytes.size())
            {
                auto first{bytes.data()+used};
                auto next{fast_io::operations::read_some(input,first,bytes.data()+bytes.size())};
                if(next==first) { break; }used=static_cast<std::size_t>(next-bytes.data());
            }
            input.close();
        }
        catch(...) { return false; }
#else
        bool readable{true};
        while(used<bytes.size())
        {
            auto count{::read(file,bytes.data()+used,bytes.size()-used)};
            if(count<0&&errno==EINTR) { continue; }
            if(count<0) { readable=false;break; }
            if(!count) { break; }used+=static_cast<std::size_t>(count);
        }
        int closed{::close(file)};if(!readable||closed) { return false; }
#endif
        if(used<2||used==bytes.size()||bytes[used-1]!='\n'||bytes[0]<'0'||bytes[0]>'9') { return false; }
        unsigned candidate{};
#if __cplusplus >= 202002L && defined(__cpp_concepts)
        auto const [next,error]{fast_io::parse_by_scan(bytes.data(),bytes.data()+used-1,candidate)};
        if(error!=fast_io::parse_code::ok||next!=bytes.data()+used-1) { return false; }
#else
        auto const [next,error]{std::from_chars(bytes.data(),bytes.data()+used-1,candidate)};
        if(error!=std::errc{}||next!=bytes.data()+used-1) { return false; }
#endif
        expected=candidate;return true;
    }
    void receive(std::string const& value)
    {
        if(value == "warmup") { write_marker(directory + "/warmup.ready","READY\n"); return; }
        bool valid{value.size() >= 6 && value.size() <= 8 && value.rfind("race-", 0) == 0};
        for(std::size_t i{5}; i < value.size(); ++i) { valid = valid && value[i] >= '0' && value[i] <= '9'; }
        if(!valid || !messages.insert(value).second) { wrong = true; }
        else { write_marker(directory + "/" + value + ".seen","SEEN\n"); }
    }
    bool complete()
    {
        unsigned expected{};
        if(!read_count(expected)) { return false; }
        ros_probe::require(!wrong && messages.size() <= expected, "revoke race emitted only unique admitted native messages");
        return messages.size() == expected;
    }
};
