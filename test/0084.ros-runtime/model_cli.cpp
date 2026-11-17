/** @file @brief Exercise actual model worker CLI callbacks without native I/O. */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/cmdline/params/scene.h>
#include <cassert>
#include <cstdio>
namespace p=uwvm2::uwvm::cmdline::params;
namespace cli=uwvm2::utils::cmdline;
namespace s=uwvm2::imported::ros::scene;
static constexpr auto zero="0000000000000000000000000000000000000000000000000000000000000000";
static cli::parameter_return_type call(cli::parameter const& parameter,std::initializer_list<std::string_view> arguments,bool occupied=false)
{
    std::array<cli::parameter_parsing_results,32> parsed{};assert(arguments.size()<parsed.size());
    parsed[0].para=&parameter;parsed[0].type=cli::parameter_parsing_results_type::parameter;
    std::size_t index{1};for(auto text:arguments)
    {
        parsed[index].str=uwvm2::utils::container::u8cstring_view{fast_io::containers::null_terminated,reinterpret_cast<char8_t const*>(text.data()),text.size()};
        parsed[index++].type=occupied?cli::parameter_parsing_results_type::occupied_arg:cli::parameter_parsing_results_type::arg;
    }
    auto result=parameter.handle(parsed.data(),parsed.data(),parsed.data()+index);
    if(!occupied)for(std::size_t i{1};i<index;++i)
        assert(parsed[i].type==(result==cli::parameter_return_type::def||result==cli::parameter_return_type::return_imme?
            cli::parameter_parsing_results_type::occupied_arg:cli::parameter_parsing_results_type::arg));
    return result;
}
int main()
{
    constexpr auto ok=cli::parameter_return_type::def,fail=cli::parameter_return_type::return_m1_imme;
    assert(call(p::ros_scene_create,{"robot"})==ok&&call(p::ros_scene_bind,{"robot","main"})==ok);
    assert(call(p::ros_scene_grant,{"robot","brain","model.vision","approved","local-model","model.open,model.submit,model.take,model.cancel,model.describe,request.describe,request.release"})==ok);
    assert(call(p::ros_scene_model,{"robot","brain","model",zero,"task",zero,"parameters","result","bytes","terminal"})==ok);
    assert(call(p::ros_scene_model_capacity,{"robot","brain","1","2","0","0","8","32","2","4096","1000000000"})==ok);
    assert(call(p::ros_scene_model_budget,{"robot","1","2","0","32","2","4096"})==ok);
    assert(call(p::ros_scene_check,{})==fail); // Mandatory worker closure absent; no freeze is committed.
    assert(call(p::ros_scene_model_worker_placement,{"robot","absent","inherit","inherit","none"})==fail);
    for(auto value:{"-1","1,0","1,1","1-","+1","1 ","8388608"})
        assert(call(p::ros_scene_model_worker_placement,{"robot","brain",value,"inherit","none"})==fail);
    assert(call(p::ros_scene_model_worker_placement,{"robot","brain","inherit","bind","none"})==fail);
    assert(call(p::ros_scene_model_worker_placement,{"robot","brain","inherit","inherit","0"})==fail);
    assert(call(p::ros_scene_model_worker_placement,{"robot","brain","inherit","inherit"})==fail);
    assert(call(p::ros_scene_model_worker_placement,{"robot","brain","0-2,64,1025","bind","0-1"},true)==fail);
    // Static configuration does not probe topology, even when IDs are unavailable here.
    assert(call(p::ros_scene_model_worker_placement,{"robot","brain","0-2,64,1025","bind","0-1"})==ok);
    assert(call(p::ros_scene_model_worker_placement,{"robot","brain","inherit","inherit","none"})==fail);
    for(auto value:{"unlimited","-1","18446744073709551616","garbage"})
        assert(call(p::ros_scene_model_worker_limit,{"robot","brain","image-bytes",value})==fail);
    assert(call(p::ros_scene_model_worker_limit,{"robot","brain","unknown","1"})==fail);
    assert(call(p::ros_scene_model_worker_limit,{"robot","brain","image-bytes"})==fail);
    assert(call(p::ros_scene_model_worker_limit,{"robot","brain","image-bytes","4096"},true)==fail);
    assert(call(p::ros_scene_model_worker_launcher,{"robot","brain","relative","64",zero})==fail);
    assert(call(p::ros_scene_model_worker_launcher,{"robot","brain","/missing/launcher","64","FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"})==fail);
    assert(call(p::ros_scene_model_worker_launcher,{"robot","brain","/missing/launcher","unlimited",zero})==fail);
    assert(call(p::ros_scene_model_worker_launcher,{"robot","brain","/missing/launcher","64",zero})==ok);
    assert(call(p::ros_scene_model_worker_launcher,{"robot","brain","/missing/launcher","64",zero})==fail);
    assert(call(p::ros_scene_model_worker_image,{"robot","brain","anything","/worker/backend.so","/missing/backend","64",zero})==fail);
    assert(call(p::ros_scene_model_worker_image,{"robot","brain","runtime","/worker/../escape","/missing/backend","64",zero})==fail);
    assert(call(p::ros_scene_model_worker_image,{"robot","brain","bootstrap","/worker/bootstrap","/missing/bootstrap","64",zero})==ok);
    assert(call(p::ros_scene_model_worker_image,{"robot","brain","runtime","/worker/backend.so","/missing/backend","64",zero})==ok);
    assert(call(p::ros_scene_model_worker_image,{"robot","brain","runtime","/worker/backend.so","/missing/other","64",zero})==fail);
    s::model_sandbox::limits limits{4096,64u*1024u*1024u,1,4096,64,4096,0,4096,4096,0,1000000000,1000000,512u*1024u};
    for(auto field:s::model_worker::limit_fields)
    {
        std::array<char,32> number{};auto parsed=std::to_chars(number.data(),number.data()+number.size()-1,limits.*field.member);assert(parsed.ec==std::errc{});
        assert(call(p::ros_scene_model_worker_limit,{"robot","brain",field.name,{number.data(),static_cast<std::size_t>(parsed.ptr-number.data())}})==ok);
    }
    assert(call(p::ros_scene_model_worker_limit,{"robot","brain","stderr-bytes","0"})==fail);
    assert(call(p::ros_scene_check,{})==cli::parameter_return_type::return_imme); // Missing paths prove validation performed no native I/O.
    assert(call(p::ros_scene_model_worker_limit,{"robot","brain","poll-ns","1"})==fail);
    assert(call(p::ros_scene_model_worker_placement,{"robot","brain","inherit","inherit","none"})==fail);
    assert(call(p::ros_scene_model_worker_image,{"robot","brain","weights","/model/new","/missing/weights","64",zero})==fail);
    int identity{};std::shared_ptr<s::broker> state;
    assert(uwvm2::uwvm::imported::scene::configuration.select("main",{&identity,1},state)==s::error::denied&&!state);
    std::puts("PASS actual worker CLI callbacks, mandatory closure, finite values, exact images, duplicate rejection and no-I/O validation");
}
