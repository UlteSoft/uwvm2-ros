#pragma once
inline int ros1_scalar_witness(int argc,char** argv)
{
    require(argc==3,"scalar ROS1 witness requires a kind");std::string mode{argv[1]},kind{argv[2]};
    require(mode=="--scalar-witness"||mode=="--scalar-observe-set"||mode=="--scalar-observe-create"||mode=="--scalar-observe-denied"||mode=="--scalar-wrong-kind","known scalar ROS1 witness mode");
    auto initial{scalar_value(core::version::ros1,kind)},changed{scalar_value(core::version::ros1,kind,true)};
    ros::M_string remapping;remapping["__ns"]="/uwvm_resources";
    ros::init(remapping,"native",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node;ros::AsyncSpinner spinner{2};spinner.start();
    auto assign=[&](ipc::parameter_value const& value) {
        if(value.type==ipc::parameter_kind::boolean) { node.setParam("/uwvm_resources/value",value.bool_value); }
        else if(value.type==ipc::parameter_kind::int32) { node.setParam("/uwvm_resources/value",static_cast<int>(value.integer_value)); }
        else { node.setParam("/uwvm_resources/value",value.double_value); }
    };
    bool wrong{mode=="--scalar-wrong-kind"},creation{mode=="--scalar-observe-create"};
    if(wrong) { node.setParam("/uwvm_resources/value",std::string{"hello"}); }else if(creation) { node.deleteParam("/uwvm_resources/value"); }else { assign(initial); }
    auto read=[&]() {
        XmlRpc::XmlRpcValue native;require(node.getParam("/uwvm_resources/value",native),"scalar native ROS1 value exists");
        if(wrong) { require(native.getType()==XmlRpc::XmlRpcValue::TypeString&&static_cast<std::string>(native)=="hello","wrong-kind native String remains unchanged");return std::vector<std::uint8_t>{}; }
        auto value{initial};
        if(value.type==ipc::parameter_kind::boolean) { require(native.getType()==XmlRpc::XmlRpcValue::TypeBoolean,"native Bool kind retained");value.bool_value=static_cast<bool>(native); }
        else if(value.type==ipc::parameter_kind::int32) { require(native.getType()==XmlRpc::XmlRpcValue::TypeInt,"native int32 kind retained");value.integer_value=static_cast<int>(native); }
        else { require(native.getType()==XmlRpc::XmlRpcValue::TypeDouble,"native double kind retained");value.double_value=static_cast<double>(native); }
        return scalar_bytes(value);
    };
    std::cout<<"READY scalar ROS1 "<<kind<<'\n'<<std::flush;
    bool observed{};auto until{std::chrono::steady_clock::now()+std::chrono::seconds(mode=="--scalar-witness"||mode=="--scalar-observe-set"||creation?30:5)};
    while(std::chrono::steady_clock::now()<until)
    {
        if(creation&&!node.hasParam("/uwvm_resources/value")) { std::this_thread::sleep_for(std::chrono::milliseconds(20));continue; }
        auto bytes{read()};if((mode=="--scalar-observe-set"||creation)&&bytes==scalar_bytes(changed)) { observed=true;break; }
        if(mode=="--scalar-observe-denied") { require(bytes==scalar_bytes(initial),"denied scalar write leaves native kind and exact value unchanged"); }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    spinner.stop();ros::shutdown();
    if(mode=="--scalar-observe-set"||creation) { require(observed,"native scalar set observed");std::cout<<(creation?"OBSERVED_SCALAR_CREATE\n":"OBSERVED_SCALAR_SET\n"); }
    if(mode=="--scalar-observe-denied"||wrong) { std::cout<<"NO_UNAUTHORIZED_SCALAR_WRITE\n"; }
    return 0;
}
