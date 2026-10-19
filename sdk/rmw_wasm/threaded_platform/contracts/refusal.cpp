// SPDX-License-Identifier: Apache-2.0
// Feature-zero executable; no pretend WASI structs or success mocks.
#include <uwvm_rmw/wasi_signal.hpp>
#include <csignal>
#include <cstdint>
using namespace uwvm_rmw::wasi_signal;
static void event(int){} static void failure()noexcept{}
int main(){
 profile p{profile_version,3,1,1,maximum_slice_ns};
 handler previous=&event;diagnostics out{};out.first_error=731;
 if(in_receiver_thread())return 9;
 if(prepare(p)!=95)return 1;
 if(exchange_handler(SIGINT,&event,&previous)!=95||previous!=&event)return 2;
 if(activate(interrupt_mask,&failure)!=95)return 3;
 if(stop_and_join()!=95)return 4;
 if(terminal_error()!=95)return 5;
 if(observe(&out)!=95||out.first_error!=731)return 6;
 if(observe(nullptr)!=22)return 7;
 if(exchange_handler(SIGINT,&event,nullptr)!=22)return 8;
 return 0;
}
