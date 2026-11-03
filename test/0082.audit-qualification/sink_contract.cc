#include <uwvm2/imported/ros/common/audit_io.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <string>
namespace a=uwvm2::imported::ros::audit_io;
int main(int argc,char** argv)
{
    if(std::getenv("UWVM_AUDIT_HOLD")) {std::raise(SIGSTOP);}
    if(argc!=2) {return 64;}std::string mode{argv[1]},payload{"{\"event\":\"audit-qualification\"}"};
    a::failure expected{a::failure::none};bool should_succeed=mode=="regular"||mode=="pipe"||mode=="socket";
    if(mode=="oversized") {payload.assign(4096,'x');expected=a::failure::oversized;}
    if(mode=="full") {expected=a::failure::would_block;}
    if(mode=="closed"||mode=="socket-closed") {expected=a::failure::peer_closed;}
    if(mode=="invalid") {::close(STDERR_FILENO);expected=a::failure::bad_descriptor;}
    if(mode=="readonly") {expected=a::failure::bad_descriptor;}
    auto flags=::fcntl(STDERR_FILENO,F_GETFL,0),fdflags=::fcntl(STDERR_FILENO,F_GETFD,0);errno=741;
    auto ok=a::write_record(payload);
    if(errno!=741||ok!=should_succeed||a::delivered_records()!=(ok?1u:0u)||a::dropped_records()!=(ok?0u:1u)) {return 1;}
    if(!ok&&a::failed_records(expected)!=1) {return 2;}
    if(::fcntl(STDERR_FILENO,F_GETFL,0)!=flags||::fcntl(STDERR_FILENO,F_GETFD,0)!=fdflags) {return 3;}
    std::printf("PASS_AUDIT_SINK mode=%s delivered=%llu dropped=%llu reason=%u\n",mode.c_str(),
        static_cast<unsigned long long>(a::delivered_records()),static_cast<unsigned long long>(a::dropped_records()),static_cast<unsigned>(expected));
}
