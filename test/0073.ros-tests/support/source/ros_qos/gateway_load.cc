#include "common.h"
#include <sys/stat.h>

int main()
{
    using namespace action_qos_test;
    std::string directory;unsigned cases{};
    try
    {
        char pattern[]{"/tmp/uwvm-action-qos-load-XXXXXX"};auto created{::mkdtemp(pattern)};
        check(created!=nullptr,"owned fixture directory failed");directory=created;
        auto path{directory+"/policy.txt"},socket{directory+"/forbidden.sock"};
        auto load=[&](core::target_policy const& policy,char const* expected,std::string const& label) {
            auto text{config_text(policy,socket)};
            int fd{::open(path.c_str(),O_CREAT|O_TRUNC|O_WRONLY|O_CLOEXEC|O_NOFOLLOW,0600)};
            check(fd>=0,"private configuration create failed");
            bool written{gateway::native_io::write_all(fd,text)};int closed{::close(fd)};
            check(written&&!closed,"private configuration write failed");
            std::string failure;
            try { auto actual{gateway::load(path)};check(actual.policy.capabilities.size()==policy.capabilities.size(),"loaded grant count differs"); }
            catch(std::runtime_error const& error) { failure=error.what(); }
            check(expected?failure.find(expected)!=std::string::npos:failure.empty(),label+": unexpected load diagnostic: "+failure);
            struct stat stat{};check(::lstat(socket.c_str(),&stat)<0&&errno==ENOENT,label+": load created IPC socket");
            ++cases;line("PASS trusted Gateway load "+label); 
        };
        load(pair(),nullptr,"matching 40 fields");
        for(auto const& row:mutations)
        {
            auto policy{pair()};policy.capabilities.at("respond").limits[row.key]=row.value;
            load(policy,row.valid?diagnostic:"invalid explicit Action QoS",std::string{row.key}+" mismatch");
            policy.capabilities.at("advertise").limits[row.key]=row.value;
            load(policy,row.valid?nullptr:"invalid explicit Action QoS",std::string{row.key}+" equal mutation");
            if(row.valid)
            {
                policy.capabilities.at("advertise").limits[row.key]=row.baseline;
                policy.capabilities.at("respond").resource="/uwvm_action_qos/independent";
                load(policy,nullptr,std::string{row.key}+" independent resource");
            }
        }
        for(auto const& row:mutations)
        {
            if(std::string_view{row.key}.find(".depth")==std::string_view::npos) { continue; }
            auto policy{pair()};policy.capabilities.at("respond").limits[row.key]="08";
            load(policy,diagnostic,std::string{row.key}+" exact 8 versus 08");
            policy.capabilities.at("advertise").limits[row.key]="08";
            load(policy,nullptr,std::string{row.key}+" matching 08");
        }
        // This isolates matcher scope. A Worker rejects incompatible trusted
        // types earlier; unsupported fabricated types are not valid policies.
        for(bool identity:{false,true})
        {
            auto policy{pair()};auto& response{policy.capabilities.at("respond")};
            response.limits["qos.feedback.depth"]="9";
            if(identity) { response.type_identity="different-trusted-identity"; }
            else { response.type="different_package/action/Different"; }
            gateway::validate_action_qos_contract(policy);++cases;
        }
        line("PASS Gateway load Action contract cases="+std::to_string(cases)+"; no ROS Context or native endpoint was constructed by this fixture");
        ::unlink(path.c_str());::rmdir(directory.c_str());return 0;
    }
    catch(std::exception const& error)
    {
        if(!directory.empty()) { ::unlink((directory+"/policy.txt").c_str());::rmdir(directory.c_str()); }
        gateway::native_io::write_all(STDERR_FILENO,std::string{"FAIL "}+error.what()+"\n");return 1;
    }
}
