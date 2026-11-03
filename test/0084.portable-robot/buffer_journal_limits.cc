// SPDX-License-Identifier: Apache-2.0
// Host journal boundary tests. No ROS transport or robot acceptance is implied.
#include <uwvm2/imported/ros/common/buffer_audit.h>
#include <cassert>
#include <csignal>
#include <string>
#include <thread>
#include <vector>
#include <sys/resource.h>
using namespace uwvm2::imported::ros::buffer_audit;

int main(int argc,char** argv)
{
    assert(argc==3);
    std::string mode{argv[1]};
    int fd=::open(argv[2],O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC,0600);assert(fd>=3);
    channel c;
    limits selected{65536,1000,67108864,524288};
    if(mode=="count")selected.operation_records=2;
    if(mode=="bytes")selected.total_bytes=8192;
    if(mode=="record")selected.record_bytes=1024;
    if(mode=="payload")selected.payload_bytes=65535;
    if(mode=="protocol-max"||mode=="allocation-failure"){selected.payload_bytes=4194304;selected.record_bytes=33554432;}
    if(mode!="default")assert(c.configure_limits(selected,std::string(64,'a')));
    if(mode=="allocation-failure")
    {
        rlimit limit{1,1};assert(!::setrlimit(RLIMIT_AS,&limit));
        assert(!c.configure(mode,unsigned(fd)));return 0;
    }
    assert(c.configure(mode,unsigned(fd)));
    assert(!c.configure_limits(selected,std::string(64,'b')));
    std::array<std::uint8_t,65536> input{},output{};
    for(std::size_t i{};i<input.size();++i){input[i]=std::uint8_t(i);output[i]=std::uint8_t(255-i);}
    transfer t{};t.module="boundary";t.alias="bounded";t.operation="boundary.test";
    t.resource="/boundary";t.type="complete-binary";t.identity="test-source";
    t.source_known=true;t.instance=1;t.generation=1;t.input_read=true;t.output_written=true;
    t.input=input;t.output=output;
    std::vector<std::uint8_t> full_input,full_output;
    if(mode=="protocol-max")
    {
        full_input.assign(4194304,0x31);full_output.assign(4194304,0xa9);
        t.input=full_input;t.output=full_output;
    }
    if(mode=="short-write")
    {
        std::signal(SIGXFSZ,SIG_IGN);rlimit limit{4096,4096};assert(!::setrlimit(RLIMIT_FSIZE,&limit));
        c.emit(t);assert(!c.enabled());c.finish();return 0;
    }
    if(mode=="count"||mode=="concurrent")
    {
        t.input={input.data(),4};t.output={output.data(),4};
        if(mode=="count"){for(unsigned i{};i!=3;++i){c.emit(t);}}
        else
        {
            std::vector<std::thread> threads;
            for(unsigned i{};i!=8;++i)threads.emplace_back([&,i]{auto local=t;local.instance=i+1;for(unsigned j{};j!=50;++j){local.handle=j;c.emit(local);}});
            for(auto& thread:threads)thread.join();
        }
    }
    else if(mode=="source"){t.source_known=false;c.emit(t);}
    else if(mode=="capture"){t.input_snapshot_failed=true;c.emit(t);}
    else if(mode=="default")
    {
        t.input={input.data(),8192};t.output={};c.emit(t);
        t.input={input.data(),8193};c.emit(t);
    }
    else c.emit(t);
    c.finish();c.finish();return 0;
}
