#include <uwvm2/imported/ros/ipc/action_control.h>
#include <cassert>
#include <iostream>
#include <vector>

int main()
{
    namespace a=uwvm2::imported::ros::ipc::action_control;
    using a::command;
    unsigned checks{};
    auto check=[&](bool ok){++checks;assert(ok);};
    auto put=[](auto& v,std::size_t at,std::uint64_t n,unsigned bytes)
    {for(unsigned i{};i<bytes;++i)v[at+i]=static_cast<std::uint8_t>(n>>(8*i));};
    check(a::parse("action.send-begin")==command::send);
    check(a::parse("topic.publish")==command::none);
    check(a::capability(command::cancel_response)=="action.cancel-own-goal");
    check(a::valid_control_request(command::ready,nullptr,0));
    std::vector<std::uint8_t> id(8);put(id,0,42,8);
    check(a::valid_control_request(command::response,id.data(),8));
    check(!a::valid_control_request(command::response,id.data(),7));
    auto zero=id;zero[0]=0;check(!a::valid_control_request(command::response,zero.data(),8));
    std::uint8_t yes=1,no=2;
    check(a::valid_response(command::ready,0,&yes,1,1));
    check(!a::valid_response(command::ready,0,&no,1,1));
    check(a::valid_response(command::send,0,id.data(),8,1));
    check(a::valid_response(command::cancel,42,id.data(),8,1));
    check(!a::valid_response(command::cancel,43,id.data(),8,1));
    std::vector<std::uint8_t> accepted(35);accepted[0]=1;put(accepted,1,42,8);
    accepted[9]=1;accepted[10]=7;put(accepted,26,0xffffffffULL,4);
    put(accepted,30,999999999,4);accepted[34]=2;
    check(a::valid_response(command::response,42,accepted.data(),accepted.size(),1));
    for(std::size_t i{};i<accepted.size();++i)
        check(!a::valid_response(command::response,42,accepted.data(),i,1));
    check(!a::valid_response(command::response,43,accepted.data(),35,1));
    auto bad=accepted;bad[34]=7;check(!a::valid_response(command::response,42,bad.data(),35,1));
    bad=accepted;bad[10]=0;check(!a::valid_response(command::response,42,bad.data(),35,1));
    bad=accepted;put(bad,30,1000000000,4);check(!a::valid_response(command::response,42,bad.data(),35,1));
    auto status=accepted;status.erase(status.begin()+9);
    check(a::valid_response(command::status,42,status.data(),34,1));
    auto rejected=accepted;rejected.resize(10);rejected[9]=0;
    check(a::valid_response(command::response,42,rejected.data(),10,1));
    std::vector<std::uint8_t> cancel(38);cancel[0]=1;put(cancel,1,42,8);put(cancel,10,1,4);
    cancel[14]=7;put(cancel,34,999999999,4);
    check(a::valid_response(command::cancel_response,42,cancel.data(),38,1));
    check(!a::valid_response(command::cancel_response,42,cancel.data(),38,0));
    check(!a::valid_response(command::cancel_response,42,cancel.data(),37,1));
    cancel.push_back(0);check(!a::valid_response(command::cancel_response,42,cancel.data(),39,1));
    std::cout<<checks<<" Action control frame checks passed\n";
}
