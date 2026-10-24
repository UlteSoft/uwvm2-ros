#pragma once
#include "array_common.h"

namespace array_probe
{
    inline XmlRpc::XmlRpcValue xml(ipc::parameter_value const& value)
    {
        XmlRpc::XmlRpcValue result;auto size{ipc::parameter_array_size(value)};result.setSize(static_cast<int>(size));
        for(std::size_t n{};n<size;++n)
        {
            if(value.type==ipc::parameter_kind::boolean_array) { result[static_cast<int>(n)]=value.bool_array_value[n]!=0; }
            else if(value.type==ipc::parameter_kind::int32_array) { result[static_cast<int>(n)]=static_cast<int>(value.int32_array_value[n]); }
            else if(value.type==ipc::parameter_kind::float64_array) { result[static_cast<int>(n)]=value.double_array_value[n]; }
            else { result[static_cast<int>(n)]=value.string_array_value[n]; }
        }
        return result;
    }
    inline ipc::parameter_value from_xml(std::string const& kind,XmlRpc::XmlRpcValue& native)
    {
        auto result{value(core::version::ros1,kind,true)};require(native.getType()==XmlRpc::XmlRpcValue::TypeArray,"native ROS1 value has actual XmlRpc Array kind");
        for(int n{};n<native.size();++n)
        {
            auto type{native[n].getType()};
            if(kind=="bool") { require(type==XmlRpc::XmlRpcValue::TypeBoolean,"native ROS1 array element exact Bool");result.bool_array_value.push_back(static_cast<bool>(native[n])?1:0); }
            else if(kind=="int32") { require(type==XmlRpc::XmlRpcValue::TypeInt,"native ROS1 array element exact int32");result.int32_array_value.push_back(static_cast<int>(native[n])); }
            else if(kind=="float64") { require(type==XmlRpc::XmlRpcValue::TypeDouble,"native ROS1 array element exact Float64");result.double_array_value.push_back(static_cast<double>(native[n])); }
            else { require(type==XmlRpc::XmlRpcValue::TypeString,"native ROS1 array element exact String");result.string_array_value.push_back(static_cast<std::string&>(native[n])); }
        }
        return result;
    }
    inline XmlRpc::XmlRpcValue mixed(std::string const& kind)
    {
        auto native{xml(value(core::version::ros1,kind))};native.setSize(2);
        if(kind=="string") { native[1]=true; }else { native[1]=std::string{"wrong element"}; }return native;
    }
}
inline int ros1_array_witness(int argc,char** argv)
{
    require(argc==3||argc==4,"array ROS1 witness requires kind and optional bad-native scenario");std::string mode{argv[1]},kind{argv[2]};
    require(mode=="--array-witness"||mode=="--array-observe-set"||mode=="--array-observe-empty"||mode=="--array-observe-create"||mode=="--array-observe-denied"||mode=="--array-wrong-kind"||mode=="--array-bad-native","known native ROS1 array witness mode");
    auto changed{array_probe::value(core::version::ros1,kind)},initial{array_probe::value(core::version::ros1,kind,mode=="--array-observe-set")};
    if(mode=="--array-observe-empty") { changed=array_probe::value(core::version::ros1,kind,true); }
    bool wrong{mode=="--array-wrong-kind"},creation{mode=="--array-observe-create"},bad{mode=="--array-bad-native"};
    std::string scenario{argc==4?argv[3]:""};require(bad?(argc==4):(argc==3),"array witness scenario arguments match mode");
    if(bad) { initial=array_probe::invalid(core::version::ros1,kind,scenario); }
    ros::M_string remapping;remapping["__ns"]="/uwvm_resources";ros::init(remapping,"native",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);
    ros::NodeHandle node;ros::AsyncSpinner spinner{2};spinner.start();
    if(wrong) { node.setParam("/uwvm_resources/value",std::string{"hello"}); }
    else if(creation) { node.deleteParam("/uwvm_resources/value"); }
    else { node.setParam("/uwvm_resources/value",bad&&scenario=="mixed"?array_probe::mixed(kind):array_probe::xml(initial)); }
    auto read=[&]() { XmlRpc::XmlRpcValue native;require(node.getParam("/uwvm_resources/value",native),"native array witness parameter exists");return native; };
    std::string before;if(!creation) { auto native{read()};before=native.toXml(); }
    std::cout<<"READY array ROS1 kind="<<kind<<" mode="<<mode<<"\n"<<std::flush;
    bool observed{};auto until{std::chrono::steady_clock::now()+std::chrono::seconds(mode=="--array-observe-set"||mode=="--array-observe-empty"||creation?30:5)};
    while(std::chrono::steady_clock::now()<until)
    {
        if(creation&&!node.hasParam("/uwvm_resources/value")) { std::this_thread::sleep_for(std::chrono::milliseconds(20));continue; }
        auto native{read()};
        if(wrong) { require(native.getType()==XmlRpc::XmlRpcValue::TypeString&&static_cast<std::string>(native)=="hello","array wrong-kind refusal retains native scalar String"); }
        else if(bad) { require(native.toXml()==before,"bad native array remains exactly unchanged during denied get"); }
        else
        {
            auto raw{array_probe::raw(array_probe::from_xml(kind,native))};
            if((mode=="--array-observe-set"||mode=="--array-observe-empty"||creation)&&raw==array_probe::raw(changed)) { observed=true;break; }
            if(mode=="--array-observe-denied"||mode=="--array-witness") { require(raw==array_probe::raw(initial),"native array exact type/elements remain unchanged"); }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    spinner.stop();ros::shutdown();
    if(mode=="--array-observe-set"||mode=="--array-observe-empty"||creation) { require(observed,"actual native array mutation observed");std::cout<<"OBSERVED_ARRAY_SET exact_native_array=1\n"; }
    else { std::cout<<"NO_UNAUTHORIZED_ARRAY_WRITE exact_native_value_unchanged=1\n"; }
    std::cout<<"ROS1_ARRAY_SCOPE empty_native_array_element_kind=unavailable\nPASS native array parameter witness\n"<<std::flush;return 0;
}
