// SPDX-License-Identifier: Apache-2.0
// Deployment and native-frame boundaries. No ROS endpoint is simulated here.
#include <uwvm2/imported/ros/portable/native_client.h>
#include <cstdio>
#include <cstdlib>

namespace p=uwvm2::imported::ros::portable;
static unsigned checks{};
static void check(bool value,char const* label)
{ ++checks;if(!value) { ::std::fprintf(stderr,"FAIL %u %s\n",checks,label);::std::exit(1); } }
static ::std::string hex(p::digest const& bytes)
{
    ::std::string result;for(auto c:bytes) { result.push_back("0123456789abcdef"[c/16]);result.push_back("0123456789abcdef"[c%16]); }return result;
}
static ::std::string replace(::std::string value,::std::string_view before,::std::string_view after)
{ auto at=value.find(before);check(at!=value.npos,"fixture mutation selects existing bytes");value.replace(at,before.size(),after);return value; }
static ::std::string const policy=R"({"format":"uwvm-portable-robot-grant-v1","alias":"robot","module":"main","capability":"joint.position.bounded.v1","wasm_sha256":"1111111111111111111111111111111111111111111111111111111111111111","policy_epoch":2,"rights":15,"native":{"socket":"/work/native.sock","uid":1000,"executable_sha256":"2222222222222222222222222222222222222222222222222222222222222222","provenance_sha256":"3333333333333333333333333333333333333333333333333333333333333333","generation":3},"resources":{"joint_state_topic":"/joint_states","position_action":"/arm/follow_joint_trajectory","controller_status_service":"/controller_manager/list_controllers"}})";
static ::std::string adapter(::std::string_view grant)
{
    return R"({"format":"uwvm-portable-robot-adapter-v1","contract_version":"0.1.0","platform_id":"component_fixture","execution_class":"software_simulation","capability":"joint.position.bounded.v1","official_source":{"repository":"https://example.invalid/component-only","commit":"0123456789012345678901234567890123456789","license":"fixture","mode":"fixture","scope":"No simulator acceptance"},"ros":{"release":"humble","os_abi":"linux-x86_64","rmw":"rmw_cyclonedds_cpp","transport":"local","security":"fixture"},"frame":"joint_space","resources":{"joint_state_topic":"/joint_states","position_action":"/arm/follow_joint_trajectory","controller_status_service":"/controller_manager/list_controllers"},"joints":[{"slot":0,"native_joint":"joint_1","kind":"bounded_revolute","source_unit":"rad","sign":1,"zero_offset_microrad":0,"lower_microrad":-1000000,"upper_microrad":1000000,"max_step_microrad":10000,"tolerance_microrad":100}],"policy_sha256":")"+hex(p::hash_bytes(grant))+R"(","mapping_epoch":1})";
}
static void rejected(::std::string const& a,::std::string const& g,char const* label)
{
    p::joint_deployment output{};output.alias="unchanged";output.mapping.epoch=91;
    check(p::parse_joint_deployment(a,g,output)!=p::error::ok,label);
    check(output.alias=="unchanged"&&output.mapping.epoch==91,"rejected deployment preserves output");
}
int main()
{
    check(hex(p::hash_bytes("abc"))=="ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad","independent SHA256 vector");
    check(hex(p::hash_bytes(""))=="e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855","empty SHA256 vector");
    auto a=adapter(policy);p::joint_deployment parsed{};
    check(p::parse_joint_deployment(a,policy,parsed)==p::error::ok,"closed exact deployment");
    check(parsed.adapter_sha256==p::hash_bytes(a)&&parsed.policy_sha256==p::hash_bytes(policy)&&
          parsed.mapping.mapping==parsed.adapter_sha256&&parsed.native_joints[0]=="joint_1"&&parsed.mapping.count==1&&
          parsed.mapping.epoch==1&&parsed.policy_epoch==2&&parsed.native_generation==3,"whole bytes and epochs retained");
    rejected(a,policy+" ","policy raw bytes are pinned, including whitespace");
    check(parsed.controller_type=="joint_trajectory_controller/JointTrajectoryController","v0.1.0 retains exact standard controller class");
    for(auto const& name:{::std::string("A motor"),::std::string("wrist-axis.1"),::std::string("base/arm"),
                         ::std::string("\xe5\x85\xb3\xe8\x8a\x82_\xce\xb1"),::std::string(128,'x')})
    {
        auto named=replace(a,"joint_1",name);p::joint_deployment exact{};
        check(p::parse_joint_deployment(named,policy,exact)==p::error::ok&&exact.native_joints[0]==name,
              "native joint is an exact bounded message string");
        check(exact.adapter_sha256==p::hash_bytes(named)&&exact.mapping.mapping==exact.adapter_sha256,
              "joint spelling is bound by whole adapter bytes");
    }
    auto escaped=replace(a,"joint_1","A\\u0020motor");p::joint_deployment escaped_name{};
    check(p::parse_joint_deployment(escaped,policy,escaped_name)==p::error::ok&&escaped_name.native_joints[0]=="A motor",
          "decoded escaped name is preserved exactly");
    check(escaped_name.adapter_sha256!=p::hash_bytes(replace(a,"joint_1","A motor")),
          "equivalent decoded names do not erase exact adapter spelling");
    rejected(replace(a,"joint_1",""),policy,"empty native joint refused");
    rejected(replace(a,"joint_1",::std::string(129,'x')),policy,"native joint byte bound enforced");
    rejected(replace(a,"joint_1","bad\\u0000name"),policy,"embedded NUL native joint refused");
    rejected(replace(a,"/joint_states","/joint states"),policy,"ROS graph resource still forbids spaces");
    auto with_type=[](::std::string value,::std::string_view type)
    {
        return replace(value,"\"controller_status_service\":\"/controller_manager/list_controllers\"",
            ::std::string("\"controller_status_service\":\"/controller_manager/list_controllers\",\"controller_type\":\"")+::std::string(type)+"\"");
    };
    for(auto type:{"joint_trajectory_controller/JointTrajectoryController","fanuc_controllers/ScaledJointTrajectoryController"})
    {
        auto granted=with_type(policy,type);
        auto selected=with_type(replace(adapter(granted),"\"contract_version\":\"0.1.0\"","\"contract_version\":\"0.1.1\""),type);
        p::joint_deployment explicit_type{};
        check(p::parse_joint_deployment(selected,granted,explicit_type)==p::error::ok&&explicit_type.controller_type==type,
              "v0.1.1 binds exact supported controller class in separate grant");
        rejected(replace(selected,"\"contract_version\":\"0.1.1\"","\"contract_version\":\"0.1.0\""),granted,
                 "legacy profile refuses new controller field");
        rejected(replace(selected,"\"contract_version\":\"0.1.1\"","\"contract_version\":\"0.1.2\""),granted,
                 "unknown profile refused");
        auto omitted=replace(adapter(granted),"\"contract_version\":\"0.1.0\"","\"contract_version\":\"0.1.1\"");
        rejected(omitted,granted,"explicit profile requires controller binding on both sides");
    }
    auto fanuc_grant=with_type(policy,"fanuc_controllers/ScaledJointTrajectoryController");
    auto mismatch=with_type(replace(adapter(fanuc_grant),"\"contract_version\":\"0.1.0\"","\"contract_version\":\"0.1.1\""),
                            "joint_trajectory_controller/JointTrajectoryController");
    rejected(mismatch,fanuc_grant,"grant cannot authorize a different native controller class");
    auto unknown_grant=with_type(policy,"fixture/UnreviewedController");
    auto unknown=with_type(replace(adapter(unknown_grant),"\"contract_version\":\"0.1.0\"","\"contract_version\":\"0.1.1\""),
                           "fixture/UnreviewedController");
    rejected(unknown,unknown_grant,"unknown native class is not inferred from action name");
    for(auto change:{::std::pair{"\"rights\":15","\"rights\":7"},
                     ::std::pair{"\"policy_epoch\":2","\"policy_epoch\":0"},
                     ::std::pair{"\"generation\":3","\"generation\":0"},
                     ::std::pair{"\"uid\":1000","\"uid\":4294967295"},
                     ::std::pair{"\"uid\":1000","\"uid\":-1"},
                     ::std::pair{"/work/native.sock","relative.sock"},
                     ::std::pair{"/work/native.sock","/work/../native.sock"},
                     ::std::pair{"\"alias\":\"robot\"","\"alias\":\"\""},
                     ::std::pair{"\"alias\":\"robot\"","\"alias\":\"robot/other\""},
                     ::std::pair{"/joint_states","/different"},
                     ::std::pair{"\"rights\":15","\"rights\":15,\"extra\":1"}})
    { auto g=replace(policy,change.first,change.second);rejected(adapter(g),g,"closed grant semantic boundary"); }
    for(auto change:{::std::pair{"\"slot\":0","\"slot\":1"},
                     ::std::pair{"\"sign\":1","\"sign\":-1"},
                     ::std::pair{"\"zero_offset_microrad\":0","\"zero_offset_microrad\":1"},
                     ::std::pair{"\"max_step_microrad\":10000","\"max_step_microrad\":10001"},
                     ::std::pair{"\"tolerance_microrad\":100","\"tolerance_microrad\":1001"},
                     ::std::pair{"\"upper_microrad\":1000000","\"upper_microrad\":-1000001"},
                     ::std::pair{"\"mapping_epoch\":1","\"mapping_epoch\":0"},
                     ::std::pair{"\"mapping_epoch\":1","\"mapping_epoch\":18446744073709551616"},
                     ::std::pair{"\"mapping_epoch\":1","\"mapping_epoch\":1.0"},
                     ::std::pair{"\"mapping_epoch\":1","\"mapping_epoch\":01"},
                     ::std::pair{"\"mapping_epoch\":1","\"mapping_epoch\":-0"},
                     ::std::pair{"\"mapping_epoch\":1","\"mapping_epoch\":1,\"mapping_epoch\":1"},
                     ::std::pair{"\"mapping_epoch\":1","\"mapping_epoch\":1,\"mapping_\\u0065poch\":1"},
                     ::std::pair{"bounded_revolute","continuous"},
                     ::std::pair{"\"frame\":\"joint_space\"","\"frame\":\"joint_space\",\"unknown\":1"},
                     ::std::pair{"software_simulation","hardware"}})
    { rejected(replace(a,change.first,change.second),policy,"closed adapter semantic boundary"); }
    auto begin=a.find("\"joints\":[")+::std::string_view("\"joints\":[").size();
    auto end=a.find("],\"policy_sha256\"",begin);check(end!=a.npos,"joint array boundary");
    auto one=a.substr(begin,end-begin),two=replace(one,"\"slot\":0","\"slot\":1");
    auto duplicate=a;duplicate.replace(begin,end-begin,replace(one,"joint_1","A motor")+","+replace(two,"joint_1","A\\u0020motor"));
    rejected(duplicate,policy,"duplicate decoded native names refused");
    auto distinct=a;distinct.replace(begin,end-begin,replace(one,"joint_1","A motor")+","+replace(two,"joint_1","A  motor"));
    p::joint_deployment exact_spacing{};
    check(p::parse_joint_deployment(distinct,policy,exact_spacing)==p::error::ok&&
          exact_spacing.native_joints[0]=="A motor"&&exact_spacing.native_joints[1]=="A  motor",
          "native joint names are never whitespace normalized");
    for(::std::size_t i{};i<a.size();++i) { rejected(a.substr(0,i),policy,"every adapter prefix rejected"); }
    for(auto text:{"{} trailing","{\"a\":true}","{\"a\":null}","{\"a\":1,}","[1,]","[NaN]","[1e2]",
                   "{\"a\":1,\"\\u0061\":2}","\"\\u0000\"","\"\\ud800\"","\"\\udc00\"","\"\\ud800\\u0001\""})
    { p::deployment_detail::value v{};check(!p::deployment_detail::parser{text}.parse(v),"strict bounded JSON grammar"); }
    for(auto text:{"\"\xc0\xaf\"","\"\xed\xa0\x80\"","\"\xf4\x90\x80\x80\"","\"\x80\"","\"\xe2\x82\""})
    { p::deployment_detail::value v{};check(!p::deployment_detail::parser{text}.parse(v),"malformed UTF8 rejected"); }
    for(auto text:{"\"\\u00e9\"","\"\\u4e2d\"","\"\\ud83d\\ude00\"","\"\xf0\x9f\x98\x80\""})
    { p::deployment_detail::value v{};check(p::deployment_detail::parser{text}.parse(v),"valid Unicode scalar supported"); }
    for(auto text:{::std::string(32769,' '),::std::string(9,'[')+"1"+::std::string(9,']'),"\""+::std::string(4097,'x')+"\""})
    { p::deployment_detail::value v{};check(!p::deployment_detail::parser{text}.parse(v),"configuration allocation bounds"); }
    ::std::array<::std::uint8_t,32> frame{};for(unsigned i{};i!=16;++i) { frame[i]=static_cast<::std::uint8_t>(i+1); }
    frame[16]=42;frame[24]=2;p::native_event event{};
    check(p::decode_native_event(frame,event)&&event.goal_uuid[15]==16&&event.observed_ns==42&&event.result==p::outcome::succeeded,"all native UUID bytes retained");
    for(unsigned size{};size!=32;++size) { check(!p::decode_native_event({frame.data(),size},event),"truncated native event refused"); }
    auto wrong=frame;::std::fill(wrong.begin(),wrong.begin()+16,0);check(!p::decode_native_event(wrong,event),"zero UUID refused");
    wrong=frame;wrong[16]=0;check(!p::decode_native_event(wrong,event),"zero monotonic observation refused");
    for(unsigned kind:{0u,5u,255u}) { wrong=frame;wrong[24]=static_cast<::std::uint8_t>(kind);check(!p::decode_native_event(wrong,event),"unknown native outcome refused"); }
#if defined(__linux__)
    char directory[]{"portable-deployment-XXXXXX"};check(::mkdtemp(directory)!=nullptr,"owned test directory");
    auto path=::std::string(directory)+"/grant.json",link=::std::string(directory)+"/alias.json";
    auto file=::std::fopen(path.c_str(),"wb");check(file!=nullptr,"owned regular test file");
    check(::std::fwrite(policy.data(),1,policy.size(),file)==policy.size()&&::std::fclose(file)==0,"write bounded exact file");
    ::std::array<char,4096> cwd{};check(::getcwd(cwd.data(),cwd.size())!=nullptr,"owned working directory");
    auto absolute=::std::string(cwd.data())+"/"+path;::std::string read{};
    check(p::deployment_detail::read_exact_file(absolute,p::hash_bytes(policy),read)==p::error::ok&&read==policy,"regular exact file read");
    read="unchanged";check(p::deployment_detail::read_exact_file(absolute,p::hash_bytes(policy+" "),read)==p::error::stale&&read=="unchanged","file content pin rejects mismatch");
    check(::symlink("grant.json",link.c_str())==0,"owned symlink fixture");
    check(p::deployment_detail::read_exact_file(::std::string(cwd.data())+"/"+link,p::hash_bytes(policy),read)!=p::error::ok,"configuration final symlink refused");
    check(::unlink(link.c_str())==0&&::unlink(path.c_str())==0&&::rmdir(directory)==0,"owned fixture cleanup");
#endif
    ::std::printf("{\"passed\":true,\"checks\":%u,\"scope\":\"deployment and native frame component only\"}\n",checks);
}
