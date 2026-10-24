#pragma once
#include "probe_common.h"

namespace ros_probe
{
    class active_ipc_witness
    {
        std::atomic<bool> running_{true},unexpected_endpoint_{};
        std::atomic<unsigned> received_{};
        std::thread sampler_;
    public:
        explicit active_ipc_witness(std::function<bool()> graph_empty)
            : sampler_{[this,graph_empty=std::move(graph_empty)] {
                while(running_)
                {
                    try { if(!graph_empty()) { unexpected_endpoint_=true; } }
                    catch(...) { unexpected_endpoint_=true; }
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            }} {}
        void receive() noexcept { ++received_; }
        void stop() noexcept { running_=false;if(sampler_.joinable()) { sampler_.join(); } }
        ~active_ipc_witness() { stop(); }
        void finish()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(600));
            stop();
            require(!received_&&!unexpected_endpoint_,"active native IPC attack witnesses saw no publication or publisher endpoint throughout attempts and drain");
            ::ros_probe::print_fixture(STDOUT_FILENO,"PASS active native IPC attack publication counters0 and concurrent publisher graph sampling\n");
        }
    };
}
