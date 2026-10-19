#pragma once
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/resource.h>
#include <time.h>
namespace ros_perf
{
    inline std::uint64_t now(){timespec t{};if(clock_gettime(CLOCK_MONOTONIC,&t)){throw std::runtime_error("monotonic clock");}return std::uint64_t(t.tv_sec)*1000000000+t.tv_nsec;}
    inline std::string payload(unsigned bytes){std::string out(bytes,'x');auto stamp{now()};for(unsigned i{};i!=16;++i){out[i]="0123456789abcdef"[(stamp>>(60-i*4))&15];}return out;}
    struct samples
    {
        std::mutex mutex;std::vector<std::uint64_t> latency;unsigned bytes,count;std::uint64_t first{},last{};rusage start{};std::uint64_t began{now()};
        samples(unsigned b,unsigned n):bytes{b},count{n}{getrusage(RUSAGE_SELF,&start);}
        void receive(std::string const& data)
        {
            if(data.size()!=bytes){throw std::runtime_error("wrong native benchmark message size");}std::uint64_t stamp{};
            if(data==std::string(bytes,'w')) { return; } // Explicit discovery warm-up, excluded from measured workload.
            for(unsigned i{};i!=16;++i){auto c{data[i]};unsigned nibble{c>='0'&&c<='9'?unsigned(c-'0'):c>='a'&&c<='f'?unsigned(c-'a'+10):16};if(nibble==16){throw std::runtime_error("wrong timestamp");}stamp=(stamp<<4)|nibble;}
            for(unsigned i=16;i!=bytes;++i){if(data[i]!='x'){throw std::runtime_error("wrong native benchmark payload");}}
            auto current{now()};if(stamp>current){throw std::runtime_error("future benchmark timestamp");}
            std::lock_guard<std::mutex> lock{mutex};if(latency.empty()){first=current;}last=current;latency.push_back(current-stamp);
        }
        bool done(){std::lock_guard<std::mutex> lock{mutex};return latency.size()>=count;}
        void report()
        {
            std::lock_guard<std::mutex> lock{mutex};if(latency.size()!=count){throw std::runtime_error("bounded benchmark lost or duplicated messages");}
            std::sort(latency.begin(),latency.end());rusage end{};getrusage(RUSAGE_SELF,&end);
            auto cpu{[](timeval v){return double(v.tv_sec)+double(v.tv_usec)/1e6;}};
            auto seconds{double(now()-began)/1e9};
            std::cout<<"{\"status\":\"PASS\",\"sent\":"<<count<<",\"received\":"<<latency.size()<<",\"dropped\":0,\"payload_bytes\":"<<bytes<<",\"queue_depth\":32,\"latency_us_p50\":"<<double(latency[count/2])/1000<<",\"latency_us_p95\":"<<double(latency[std::min(count-1,unsigned(count*.95))])/1000<<",\"latency_us_max\":"<<double(latency.back())/1000<<",\"receive_hz\":"<<double(count-1)*1e9/double(last-first)<<",\"cpu_seconds\":"<<cpu(end.ru_utime)+cpu(end.ru_stime)-cpu(start.ru_utime)-cpu(start.ru_stime)<<",\"elapsed_seconds\":"<<seconds<<",\"max_rss_kib\":"<<end.ru_maxrss<<",\"voluntary_context_switches\":"<<end.ru_nvcsw-start.ru_nvcsw<<",\"involuntary_context_switches\":"<<end.ru_nivcsw-start.ru_nivcsw<<"}\n"<<std::flush;
        }
    };
}
