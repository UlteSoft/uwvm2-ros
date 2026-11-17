/** @file @brief Compare receiver identity/sequence gates with their native arbitration core.
 * @details Both loops preserve exclusive ownership and effect completion. The
 * receiver adds exact channel-origin/credential/deadline/epoch checks; this is
 * incremental host overhead, not direct-vs-IPC or native-vs-Wasm equivalence.
 */
#include <uwvm2/imported/ros/scene/driver_control.h>
#include <cassert>
#include <chrono>
#include <cstdio>
namespace d=uwvm2::imported::ros::scene::drivers;namespace c=uwvm2::imported::ros::scene::control;using d::error;
int main(int argc,char** argv)
{
 assert(argc==3);std::uint64_t count{};auto end=argv[2]+std::char_traits<char>::length(argv[2]);auto parsed=std::from_chars(argv[2],end,count);assert(parsed.ec==std::errc{}&&parsed.ptr==end&&count&&count<100000000);
 bool gate=std::string_view(argv[1])=="receiver";assert(gate||std::string_view(argv[1])=="arbiter");
 d::protocol::binding origin{{1,0},{2,0},{3,0},{4,0},{5,0},{1,2,3,4}};d::credentials peer{1,1000,1000};
 d::receiver_control receiver;c::arbitration arbiter;c::handle route{};c::lease lease;c::principal caller{1,2,3,4};
 if(gate){assert(receiver.configure(origin.driver,origin.contract,1,1,{1000,100,1,false})==error::success);assert(receiver.bind(origin,peer,2000,1000,route)==error::success);assert(receiver.freeze()==error::success);assert(receiver.grant(route,1,900,1000,lease)==error::success);}
 else{assert(arbiter.configure(1,3,1,{1000,100,1,false})==error::success);c::prepared value;assert(arbiter.prepare({caller,1,900},1000,value)==error::success);assert(arbiter.commit(value,1000)==error::success);lease=value.candidate;}
 std::uint64_t sum{};d::protocol::header request{d::protocol::kind::request,origin,1,1800,0};auto start=std::chrono::steady_clock::now();
 for(std::uint64_t i{};i<count;++i)
 {
  c::handle effect{};if(gate){request.sequence=i+1;assert(receiver.begin(route,peer,request,17,lease.epoch,1001,effect)==error::success);assert(receiver.finish(route,effect,c::effect_result::executed)==error::success);}
  else{assert(arbiter.begin_effect(caller,lease,1001,effect)==error::success);assert(arbiter.finish_effect(effect,c::effect_result::executed)==error::success);}sum+=effect;
 }
 auto elapsed=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count();
 std::printf("{\"mode\":\"%s\",\"iterations\":%llu,\"ns_per_pair\":%.6f,\"checksum\":%llu}\n",argv[1],(unsigned long long)count,double(elapsed)/double(count),(unsigned long long)sum);
}
