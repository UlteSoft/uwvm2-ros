/** @file
 * @brief CLI configuration for independent robot scene environments.
 * @details These callbacks never mutate WASIp1/ROS-v1 state. Every module must
 * be bound explicitly; every function and resource remains denied until the
 * corresponding setting is present. Private payload capture explicitly consumes a provisioned host FD.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#if defined(UWVM_IMPORT_ROSV2)
#ifndef UWVM_MODULE
#include <uwvm2/uwvm/imported/scene/storage.h>
#include <uwvm2/utils/cmdline/impl.h>
#include <uwvm2/uwvm/io/impl.h>
#endif
namespace uwvm2::uwvm::cmdline::params::scene_details
{
    namespace scene=::uwvm2::imported::ros::scene;
    using result=::uwvm2::utils::cmdline::parameter_return_type;
    enum class operation{call_audit,driver_budget,driver_kernel_channel,driver_channel,driver_schema,module_call_budget,module_call,module_call_clear_input,module_buffer_journal,module_buffer_capture,child,create,bind,expose,grant,limit,input_budget,input,output_budget,output,publication_budget,publication,managed_lifecycle,buffer_budget,buffer,model_budget,model,model_capacity,model_kernel_channel,model_channel,transform_budget,transform_profile,transform_limit,transform_definition,transform_definition_hex,recording_budget,recording_profile,recording_limit,recording_definition,recording_definition_hex,replay,log_diagnostic_budget,log_limit,log_logger,log_loss,diagnostic_limit,diagnostic_source,device_budget,device_endpoints,device_profile,device_limit,device_definition,device_definition_hex,control_profile,control_limit,control_definition,control_definition_hex,bridge_budget,bridge_profile,bridge_limit,bridge_definition,bridge_definition_hex,media_budget,media_profile,media_limit,media_definition,media_definition_hex,sensor_budget,sensor_profile,sensor_limit,sensor_descriptor,sensor_definition,sensor_definition_hex,proposal_budget,proposal_profile,proposal_limit,proposal_definition,proposal_definition_hex,proposal_arbiter,communication_budget,communication_worker,communication_binding,communication_event,communication_pair,communication_worker_limit,communication_channel,parameter_budget,parameter_profile,parameter_limit,parameter_definition,parameter_definition_hex,check,dump_effective_policy};
    inline scene::deployment& command_configuration(::uwvm2::utils::cmdline::parameter_parsing_results* end);
    template<operation Action,std::size_t Count>
    inline result configure([[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
        auto failure=[&](std::string_view reason)
        {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,u8"uwvm: [error] ROS scene policy: ",
                ::uwvm2::utils::container::u8string_view{reinterpret_cast<char8_t const*>(reason.data()),reason.size()},
                u8". Usage: ",::uwvm2::utils::cmdline::print_usage(*current->para));
            return result::return_m1_imme;
        };
        try
        {
            if(static_cast<std::size_t>(end-current)<=Count)return failure("missing argument");
            std::array<std::string_view,Count> args{};
            for(std::size_t i{};i<Count;++i)
            {
                if(current[i+1].type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)return failure("explicit argument required");
                auto text=current[i+1].str;args[i]={reinterpret_cast<char const*>(text.data()),text.size()};
            }
            auto& config=command_configuration(end);scene::error status{};
            if constexpr(Action==operation::call_audit)
            {
                scene::call_audit::scope target;
                if(args[0]=="global")target=scene::call_audit::scope::global;
                else if(args[0]=="group")target=scene::call_audit::scope::group;
                else if(args[0]=="single")target=scene::call_audit::scope::single;
                else return failure("audit scope requires global, group or single");
                if(args[2]!="all" && args[2]!="denied")return failure("audit mode requires all or denied");
                scene::call_audit::ceiling limits;
                auto fields=std::array{&limits.records,&limits.bytes,&limits.rate_hz,&limits.burst};
                for(std::size_t i{};i<fields.size();++i)
                    if(!scene::parse_unsigned(args[3+i],*fields[i]) || *fields[i]==UINT64_MAX)
                        return failure("audit ceilings require finite decimal integers");
                status=config.configure_call_audit(target,args[1],args[2]=="denied",limits);
            }
            if constexpr(Action==operation::driver_schema)
            {
                std::array<std::uint64_t,3> values{};
                for(std::size_t i{};i<values.size();++i)if(!scene::parse_unsigned(args[3+i],values[i])||values[i]>65536)return failure("driver schema requires bounded traversal limits");
                status=config.driver_schema(args[0],args[1],args[2],{0,static_cast<std::size_t>(values[0]),static_cast<std::size_t>(values[1]),static_cast<unsigned>(values[2])});
            }
            if constexpr(Action==operation::driver_budget)
            {
                std::uint64_t count{},bytes{};
                if(!scene::parse_unsigned(args[1],count)||!scene::parse_unsigned(args[2],bytes))return failure("driver budget requires finite bindings and bytes");
                status=config.driver_budget(args[0],count,bytes);
            }
            if constexpr(Action==operation::driver_kernel_channel||Action==operation::driver_channel)
            {
                std::array<std::uint64_t,8> number{};
                for(std::size_t i{};i<4;++i)if(!scene::parse_unsigned(args[i+3],number[i]))return failure("driver descriptor and peer credentials require exact integers");
                for(std::size_t i{};i<4;++i)if(!scene::parse_unsigned(args[i+13],number[i+4]))return failure("driver limits require exact finite integers");
                if(number[0]>INT32_MAX||number[4]>scene::drivers::protocol::maximum_payload||number[5]>scene::drivers::protocol::maximum_payload)return failure("driver descriptor or carrier exceeds bound");
                scene::drivers::protocol::binding binding;
                std::array<scene::drivers::protocol::identity*,5> ids{&binding.deployment,&binding.vm,&binding.module,&binding.driver,&binding.session};
                auto hex=[](std::string_view value,std::span<std::uint64_t> words)noexcept
                {
                    if(value.size()!=words.size()*16)return false;
                    for(std::size_t i{};i<words.size();++i)
                    {
                        auto part=value.substr(i*16,16);for(auto c:part)if(!((c>='0'&&c<='9')||(c>='a'&&c<='f')))return false;
                        std::uint64_t word{};for(unsigned n{};n<8;++n)
                        {auto digit=[](char c){return c<='9'?c-'0':c-'a'+10;};word|=std::uint64_t(digit(part[2*n])*16+digit(part[2*n+1]))<<(8*n);}
                        words[i]=word;
                    }
                    return true;
                };
                for(std::size_t i{};i<ids.size();++i)if(!hex(args[7+i],*ids[i]))return failure("driver identities require 32 lowercase hex characters");
                if(!hex(args[12],binding.contract))return failure("driver contract requires 64 lowercase hex characters");
                int key_fd=-1,time_fd=-1;
                if constexpr(Action==operation::driver_channel)
                {
                    std::uint64_t key{},clock{};
                    if(!scene::parse_unsigned(args[17],key)||key>INT32_MAX)return failure("TLS identity requires a sealed descriptor");
                    if(!scene::parse_unsigned(args[18],clock)||clock>INT32_MAX)return failure("peer clock requires a provisioned time-namespace descriptor");
                    key_fd=static_cast<int>(key);time_fd=static_cast<int>(clock);
                }
                status=config.driver_bind(args[0],args[1],args[2],static_cast<int>(number[0]),{number[1],number[2],number[3]},binding,
                    {static_cast<std::size_t>(number[4]),static_cast<std::size_t>(number[5]),number[6],number[7],key_fd,time_fd});
            }
            if constexpr(Action==operation::create)status=config.create(::uwvm2::utils::container::std_string(args[0]));
            if constexpr(Action==operation::child)status=config.platform_setting(args[0],args[1],args[2],args[3]);
            if constexpr(Action==operation::module_buffer_journal)
            {
                std::uint64_t fd{},records{},bytes{},payload{};
                if(!scene::parse_unsigned(args[0],fd)||fd>1048575||!scene::parse_unsigned(args[1],records)||
                   !scene::parse_unsigned(args[2],bytes)||!scene::parse_unsigned(args[3],payload)||
                   config.configure_payload_journal(static_cast<int>(fd),{records,bytes,payload})!=scene::error::success)
                    return failure("invalid private journal descriptor or limits");
            }
            if constexpr(Action==operation::module_buffer_capture)
            {
                scene::module_buffer_audit::stage stage{};
                if(!scene::module_buffer_audit::parse_stage(args[2],stage))
                    return failure("capture stage requires submit, take, respond or result");
                status=config.module_call_buffer_capture(args[0],args[1],stage);
            }
            if constexpr(Action==operation::module_call_budget)
            {
                std::uint64_t bytes{},routes{};
                if(!scene::parse_unsigned(args[1],bytes)||!scene::parse_unsigned(args[2],routes))return failure("module call reservation requires finite bytes and routes");
                status=config.module_call_budget(args[0],bytes,routes);
            }
            if constexpr(Action==operation::module_call)
            {
                scene::module_exchange::plan value;
                value.route=args[1];value.client=args[2];value.server=args[3];value.semantic=args[4];value.request_schema=args[5];value.response_schema=args[6];
                if(!scene::parse_unsigned(args[7],value.request_bytes)||!scene::parse_unsigned(args[8],value.response_bytes)||
                   !scene::parse_unsigned(args[9],value.slots)||!scene::parse_unsigned(args[10],value.deadline_ns))return failure("module call contract requires finite capacities and deadline");
                status=config.module_call_plan(args[0],std::move(value));
            }
            if constexpr(Action==operation::module_call_clear_input)
            {
                if(args[2]!="true"&&args[2]!="false")return failure("input clearing requires true or false");
                bool clear=args[2]=="true";
                if(!clear&&::uwvm2::uwvm::io::show_retained_ros_input_warning)
                {
                    bool fatal=::uwvm2::uwvm::io::retained_ros_input_warning_fatal;
                    ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                        fatal?::uwvm2::utils::container::u8string_view{u8"uwvm: [error] "}:
                              ::uwvm2::utils::container::u8string_view{u8"uwvm: [warning] "},
                        u8"Copied ROSv2 request bytes will remain in sender memory. Response and broker cleanup remain enabled. (retained-ros-input)");
                    if(fatal)return result::return_m1_imme;
                }
                status=config.module_call_input_clear(args[0],args[1],clear);
            }
            if constexpr(Action==operation::bind)status=config.bind(args[0],::uwvm2::utils::container::std_string(args[1]));
            if constexpr(Action==operation::expose)
            {
                auto state=args[2]=="hidden"?scene::exposure::hidden:args[2]=="deny"?scene::exposure::deny:scene::exposure::allow;
                if(args[2]!="hidden"&&args[2]!="deny"&&args[2]!="allow")return failure("interface requires hidden, deny or allow");
                status=config.expose(args[0],::uwvm2::utils::container::std_string(args[1]),state);
            }
            if constexpr(Action==operation::grant)
            {
                auto purpose=scene::parse_purpose(args[2]);if(!purpose)return failure("unknown resource purpose");
                scene::grant value;value.alias=args[1];value.kind=*purpose;value.resource=args[3];value.binding=args[4];
                auto actions=args[5];if(actions.empty()||actions.back()==',')return failure("empty action list or action");
                while(!actions.empty())
                {
                    auto at=actions.find(',');auto action=actions.substr(0,at);
                    if(action.empty()||!value.actions.emplace(action).second)return failure("empty or duplicated action");
                    if(at==actions.npos)break;
                    actions.remove_prefix(at+1);
                }
                status=config.allow(args[0],std::move(value));
            }
            if constexpr(Action==operation::limit)status=config.limit(args[0],args[1],::uwvm2::utils::container::std_string(args[2]),args[3]);
            if constexpr(Action==operation::log_diagnostic_budget||Action==operation::log_limit||Action==operation::diagnostic_limit)
            {
                constexpr std::size_t index=Action==operation::log_diagnostic_budget?2:3;std::uint64_t value{};
                if(args[index]=="unlimited"||!scene::parse_unsigned(args[index],value))return failure("log and diagnostic bounds require exact finite integers");
                if constexpr(Action==operation::log_diagnostic_budget)status=config.log_diagnostic_budget_limit(args[0],args[1],value);
                if constexpr(Action==operation::log_limit)status=config.log_limit(args[0],args[1],args[2],value);
                if constexpr(Action==operation::diagnostic_limit)status=config.diagnostic_limit(args[0],args[1],args[2],value);
            }
            if constexpr(Action==operation::log_logger)status=config.log_logger(args[0],args[1],args[2]);
            if constexpr(Action==operation::log_loss)status=config.log_loss(args[0],args[1],args[2]);
            if constexpr(Action==operation::diagnostic_source)status=config.diagnostic_source(args[0],args[1],args[2]);
            if constexpr(Action==operation::input_budget)
            {
                std::uint64_t bytes{},objects{};
                if(args[1]=="unlimited"||args[2]=="unlimited"||!scene::parse_unsigned(args[1],bytes)||!scene::parse_unsigned(args[2],objects))
                    return failure("input reservation requires exact finite byte and object capacities");
                status=config.input_budget(args[0],bytes,objects);
            }
            if constexpr(Action==operation::transform_budget)
            {
                std::array<std::uint64_t,6> values{};
                for(std::size_t i{};i<values.size();++i)
                    if(args[i+1]=="unlimited"||!scene::parse_unsigned(args[i+1],values[i]))return failure("transform budgets require six exact finite capacities");
                status=config.transform_budget(args[0],{values[0],values[1],values[2],values[3],values[4],values[5]});
            }
            if constexpr(Action==operation::transform_profile)
            {
                scene::buffers::profile profile;
                if(!profile.identity.assign(args[2])||!scene::artifacts::parse_digest(args[3],profile.sha256))return failure("transform definition requires identity and lowercase SHA256");
                status=config.transform_profile(args[0],::uwvm2::utils::container::std_string(args[1]),profile);
            }
            if constexpr(Action==operation::transform_limit)
            {
                std::uint64_t value{};
                if(args[3]=="unlimited"||!scene::parse_unsigned(args[3],value))return failure("transform limit requires an exact finite integer");
                status=config.transform_limit(args[0],args[1],args[2],value);
            }
            if constexpr(Action==operation::transform_definition)
            {
                std::uint64_t bytes{};scene::buffers::digest digest{};
                if(args[3]=="unlimited"||!scene::parse_unsigned(args[3],bytes)||!scene::artifacts::parse_digest(args[4],digest))return failure("definition requires exact bytes and lowercase SHA256");
                status=config.transform_definition_file(args[0],args[1],args[2],bytes,digest);
            }
            if constexpr(Action==operation::transform_definition_hex)
            {
                if(args[2].empty()||args[2].size()%2||args[2].size()/2>scene::transforms::maximum_definition_bytes)return failure("bounded even-length definition hex required");
                auto nibble=[](char c)->int{return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:-1;};
                scene::byte_buffer owned;owned.resize_for_overwrite(args[2].size()/2);auto destination=owned.writable();
                for(std::size_t i{};i<destination.size();++i)
                {auto high=nibble(args[2][i*2]),low=nibble(args[2][i*2+1]);if(high<0||low<0)return failure("definition hex must be lowercase");destination[i]=static_cast<std::uint8_t>((high<<4)|low);}
                status=config.transform_definition(args[0],args[1],owned.view());
            }
            if constexpr(Action==operation::recording_budget||Action==operation::recording_limit)
            {
                constexpr std::size_t index=Action==operation::recording_budget?2:3;std::uint64_t value{};
                if(args[index]=="unlimited"||!scene::parse_unsigned(args[index],value))return failure("recording reservations require exact finite integers");
                if constexpr(Action==operation::recording_budget)status=config.recording_budget_limit(args[0],args[1],value);
                else status=config.recording_limit(args[0],args[1],args[2],value);
            }
            if constexpr(Action==operation::recording_profile)
            {
                scene::buffers::profile profile;
                if(!profile.identity.assign(args[2])||!scene::artifacts::parse_digest(args[3],profile.sha256))return failure("recording definition requires identity and lowercase SHA256");
                status=config.recording_profile(args[0],args[1],profile);
            }
            if constexpr(Action==operation::recording_definition)
            {
                std::uint64_t bytes{};scene::buffers::digest digest{};
                if(args[3]=="unlimited"||!scene::parse_unsigned(args[3],bytes)||!scene::artifacts::parse_digest(args[4],digest))return failure("recording definition requires exact bytes and lowercase SHA256");
                status=config.recording_definition_file(args[0],args[1],args[2],bytes,digest);
            }
            if constexpr(Action==operation::recording_definition_hex)status=config.recording_definition_hex(args[0],args[1],args[2]);
            if constexpr(Action==operation::parameter_budget||Action==operation::parameter_limit)
            {
                constexpr std::size_t index=Action==operation::parameter_budget?2:3;std::uint64_t value{};
                if(args[index]=="unlimited"||!scene::parse_unsigned(args[index],value))return failure("parameter reservations require exact finite integers");
                if constexpr(Action==operation::parameter_budget)status=config.parameter_budget_limit(args[0],args[1],value);
                else status=config.parameter_limit(args[0],args[1],args[2],value);
            }
            if constexpr(Action==operation::parameter_profile)
            {
                scene::buffers::profile profile,node;
                if(!profile.identity.assign(args[3])||!scene::artifacts::parse_digest(args[4],profile.sha256)||
                   !node.identity.assign(args[5])||!scene::artifacts::parse_digest(args[6],node.sha256)||
                   (args[7]!="ros1"&&args[7]!="ros2"))return failure("parameter profile requires exact node/definition digests and ros1 or ros2 value algebra");
                status=config.parameter_profile(args[0],args[1],args[2],profile,node,args[7]=="ros1"?scene::parameter_values::algebra::ros1:scene::parameter_values::algebra::ros2);
            }
            if constexpr(Action==operation::parameter_definition)
            {
                std::uint64_t bytes{};scene::buffers::digest digest{};
                if(args[3]=="unlimited"||!scene::parse_unsigned(args[3],bytes)||!scene::artifacts::parse_digest(args[4],digest))return failure("parameter definition requires exact bytes and lowercase SHA256");
                status=config.parameter_definition_file(args[0],args[1],args[2],bytes,digest);
            }
            if constexpr(Action==operation::parameter_definition_hex)status=config.parameter_definition_hex(args[0],args[1],args[2]);
            if constexpr(Action==operation::replay)
            {
                scene::buffers::profile profile;std::uint64_t handles{};
                if(!profile.identity.assign(args[3])||!scene::artifacts::parse_digest(args[4],profile.sha256)||
                   args[5]=="unlimited"||!scene::parse_unsigned(args[5],handles))return failure("replay requires an exact source profile and finite handle capacity");
                status=config.replay_plan(args[0],args[1],args[2],profile,handles);
            }
            if constexpr(Action==operation::bridge_budget)
            {
                scene::bridge_dispatch::limits capacity;
                if(!scene::parse_unsigned(args[1],capacity.bindings)||!scene::parse_unsigned(args[2],capacity.storage_bytes)||!scene::parse_unsigned(args[3],capacity.supervisor_bytes)||!scene::parse_unsigned(args[4],capacity.poll_ns))return failure("bridge configuration requires exact finite values and lowercase SHA256");
                status=config.bridge_budget(args[0],capacity);
            }
            if constexpr(Action==operation::device_budget)
            {
                std::array<std::uint64_t,6> values{};
                for(std::size_t i{};i<values.size();++i)
                    if(args[i+1]=="unlimited"||!scene::parse_unsigned(args[i+1],values[i]))return failure("device capacities and supervision require exact finite values");
                status=config.device_budget(args[0],{values[0],values[1],values[2]},{values[3],values[4],values[5]});
            }
            if constexpr(Action==operation::device_endpoints)
            {
                std::array<std::uint64_t,4> values{};
                for(std::size_t i{};i<values.size();++i)
                    if(!scene::parse_unsigned(args[i+2],values[i]))return failure("device endpoints require explicit startup descriptor numbers");
                status=config.device_endpoints(args[0],args[1],values);
            }
            if constexpr(Action==operation::device_profile)
            {
                scene::buffers::profile profile;
                if(!profile.identity.assign(args[2])||!scene::artifacts::parse_digest(args[3],profile.sha256))return failure("device configuration requires exact finite values and lowercase SHA256");
                status=config.device_profile(args[0],::uwvm2::utils::container::std_string(args[1]),profile);
            }
            if constexpr(Action==operation::device_limit)
            {
                std::uint64_t value{};if(!scene::parse_unsigned(args[3],value))return failure("device configuration requires exact finite values and lowercase SHA256");
                status=config.device_limit(args[0],args[1],args[2],value);
            }
            if constexpr(Action==operation::device_definition)
            {
                std::uint64_t bytes{};scene::buffers::digest digest;
                if(!scene::parse_unsigned(args[3],bytes)||!scene::artifacts::parse_digest(args[4],digest))return failure("device configuration requires exact finite values and lowercase SHA256");
                status=config.device_definition_file(args[0],args[1],args[2],bytes,digest);
            }
            if constexpr(Action==operation::device_definition_hex)status=config.device_definition_hex(args[0],args[1],args[2]);
            if constexpr(Action==operation::control_profile)
            {
                scene::buffers::profile profile;
                if(!profile.identity.assign(args[2])||!scene::artifacts::parse_digest(args[3],profile.sha256))return failure("control configuration requires exact finite values and lowercase SHA256");
                status=config.control_profile(args[0],::uwvm2::utils::container::std_string(args[1]),profile);
            }
            if constexpr(Action==operation::control_limit)
            {
                std::uint64_t value{};if(!scene::parse_unsigned(args[3],value))return failure("control configuration requires exact finite values and lowercase SHA256");
                status=config.control_limit(args[0],args[1],args[2],value);
            }
            if constexpr(Action==operation::control_definition)
            {
                std::uint64_t bytes{};scene::buffers::digest digest;
                if(!scene::parse_unsigned(args[3],bytes)||!scene::artifacts::parse_digest(args[4],digest))return failure("control configuration requires exact finite values and lowercase SHA256");
                status=config.control_definition_file(args[0],args[1],args[2],bytes,digest);
            }
            if constexpr(Action==operation::control_definition_hex)status=config.control_definition_hex(args[0],args[1],args[2]);








            if constexpr(Action==operation::bridge_profile)
            {
                scene::buffers::profile profile;
                if(!profile.identity.assign(args[2])||!scene::artifacts::parse_digest(args[3],profile.sha256))return failure("bridge configuration requires exact finite values and lowercase SHA256");
                status=config.bridge_profile(args[0],::uwvm2::utils::container::std_string(args[1]),profile);
            }
            if constexpr(Action==operation::bridge_limit)
            {
                std::uint64_t value{};if(!scene::parse_unsigned(args[3],value))return failure("bridge configuration requires exact finite values and lowercase SHA256");
                status=config.bridge_limit(args[0],args[1],args[2],value);
            }
            if constexpr(Action==operation::bridge_definition)
            {
                std::uint64_t bytes{};scene::buffers::digest digest;
                if(!scene::parse_unsigned(args[3],bytes)||!scene::artifacts::parse_digest(args[4],digest))return failure("bridge configuration requires exact finite values and lowercase SHA256");
                status=config.bridge_definition_file(args[0],args[1],args[2],bytes,digest);
            }
            if constexpr(Action==operation::bridge_definition_hex)status=config.bridge_definition_hex(args[0],args[1],args[2]);
            if constexpr(Action==operation::media_budget)
            {
                scene::media_dispatch::limits capacity;
                if(!scene::parse_unsigned(args[1],capacity.bindings)||!scene::parse_unsigned(args[2],capacity.storage_bytes))return failure("media configuration requires exact finite values and lowercase SHA256");
                status=config.media_budget(args[0],capacity);
            }
            if constexpr(Action==operation::media_profile)
            {
                scene::buffers::profile profile;
                if(!profile.identity.assign(args[2])||!scene::artifacts::parse_digest(args[3],profile.sha256))return failure("media configuration requires exact finite values and lowercase SHA256");
                status=config.media_profile(args[0],::uwvm2::utils::container::std_string(args[1]),profile);
            }
            if constexpr(Action==operation::media_limit)
            {
                std::uint64_t value{};if(!scene::parse_unsigned(args[3],value))return failure("media configuration requires exact finite values and lowercase SHA256");
                status=config.media_limit(args[0],args[1],args[2],value);
            }
            if constexpr(Action==operation::media_definition)
            {
                std::uint64_t bytes{};scene::buffers::digest digest;
                if(!scene::parse_unsigned(args[3],bytes)||!scene::artifacts::parse_digest(args[4],digest))return failure("media configuration requires exact finite values and lowercase SHA256");
                status=config.media_definition_file(args[0],args[1],args[2],bytes,digest);
            }
            if constexpr(Action==operation::media_definition_hex)status=config.media_definition_hex(args[0],args[1],args[2]);
            if constexpr(Action==operation::sensor_budget)
            {
                scene::sensor_dispatch::limits capacity;
                if(args[1]=="unlimited"||args[2]=="unlimited"||!scene::parse_unsigned(args[1],capacity.bindings)||!scene::parse_unsigned(args[2],capacity.storage_bytes))return failure("sensor bindings and retained bytes require finite integers");
                status=config.sensor_budget(args[0],capacity);
            }
            if constexpr(Action==operation::sensor_profile||Action==operation::proposal_profile)
            {
                scene::buffers::profile profile;
                if(!profile.identity.assign(args[2])||!scene::artifacts::parse_digest(args[3],profile.sha256))return failure("profile requires identity and lowercase SHA256");
                if constexpr(Action==operation::sensor_profile)status=config.sensor_profile(args[0],::uwvm2::utils::container::std_string(args[1]),profile,args[4]);
                else status=config.proposal_profile(args[0],::uwvm2::utils::container::std_string(args[1]),profile,args[4]);
            }
            if constexpr(Action==operation::sensor_limit||Action==operation::proposal_limit||Action==operation::sensor_descriptor)
            {
                constexpr std::size_t index=Action==operation::sensor_descriptor?2:3;std::uint64_t value{};
                if(args[index]=="unlimited"||!scene::parse_unsigned(args[index],value))return failure("sensor and proposal settings require exact finite integers");
                if constexpr(Action==operation::sensor_limit)status=config.sensor_limit(args[0],args[1],args[2],value);
                else if constexpr(Action==operation::proposal_limit)status=config.proposal_limit(args[0],args[1],args[2],value);
                else status=config.sensor_descriptor(args[0],args[1],value);
            }
            if constexpr(Action==operation::sensor_definition||Action==operation::proposal_definition)
            {
                std::uint64_t bytes{};scene::buffers::digest digest{};
                if(args[3]=="unlimited"||!scene::parse_unsigned(args[3],bytes)||!scene::artifacts::parse_digest(args[4],digest))return failure("definition requires exact bytes and lowercase SHA256");
                if constexpr(Action==operation::sensor_definition)status=config.sensor_definition_file(args[0],args[1],args[2],bytes,digest);
                else status=config.proposal_definition_file(args[0],args[1],args[2],bytes,digest);
            }
            if constexpr(Action==operation::sensor_definition_hex||Action==operation::proposal_definition_hex)
            {
                if constexpr(Action==operation::sensor_definition_hex)status=config.sensor_definition_hex(args[0],args[1],args[2]);
                else status=config.proposal_definition_hex(args[0],args[1],args[2]);
            }
            if constexpr(Action==operation::proposal_budget)
            {
                std::array<std::uint64_t,5> values{};
                for(std::size_t i{};i<values.size();++i)if(args[i+1]=="unlimited"||!scene::parse_unsigned(args[i+1],values[i]))return failure("proposal reservations require finite integers");
                if(values[0]>32||values[1]>32)return failure("proposal provider and arbiter capacities exceed fixed tables");
                status=config.proposal_budget(args[0],{static_cast<std::size_t>(values[0]),static_cast<std::size_t>(values[1])},{values[2],values[3],values[4]});
            }
            if constexpr(Action==operation::proposal_arbiter)
            {
                std::uint64_t mask{};if(args[4]=="unlimited"||!scene::parse_unsigned(args[4],mask))return failure("arbiter requires an exact finite tool mask");
                status=config.proposal_arbiter(args[0],::uwvm2::utils::container::std_string(args[1]),args[2],args[3],mask);
            }
            if constexpr(Action==operation::input)
            {
                scene::artifacts::input_plan plan;plan.path=args[2];
                if(!scene::artifacts::parse_digest(args[3],plan.digest))return failure("input requires an exact lowercase SHA256");
                if(args[4]=="unlimited"||args[5]=="unlimited"||args[6]=="unlimited"||
                   !scene::parse_unsigned(args[4],plan.generation)||!scene::parse_unsigned(args[5],plan.bytes)||!scene::parse_unsigned(args[6],plan.transfer))
                    return failure("input generation, exact bytes and transfer must be finite integers");
                status=config.input(args[0],::uwvm2::utils::container::std_string(args[1]),std::move(plan));
            }
            if constexpr(Action==operation::managed_lifecycle)
            {
                std::array<std::uint64_t,4> values{};
                for(std::size_t n{};n<values.size();++n)
                    if(args[n+2]=="unlimited"||!scene::parse_unsigned(args[n+2],values[n]))return failure("finite lifecycle capacity required");
                status=config.managed_lifecycle_plan(args[0],::uwvm2::utils::container::std_string(args[1]),{values[0],values[1],values[2],values[3]});
            }
            if constexpr(Action==operation::buffer_budget)
            {
                std::array<std::uint64_t,4> values{};
                for(std::size_t n{};n<values.size();++n)
                    if(args[n+1]=="unlimited"||!scene::parse_unsigned(args[n+1],values[n]))return failure("buffer budget requires finite capacities");
                status=config.buffer_budget(args[0],values[0],values[1],values[2],values[3]);
            }
            if constexpr(Action==operation::model_budget||Action==operation::model_capacity)
            {
                constexpr auto offset=Action==operation::model_budget?1u:2u;
                std::array<std::uint64_t,Count-offset> values{};
                for(std::size_t n{};n<values.size();++n)
                    if(args[n+offset]=="unlimited"||!scene::parse_unsigned(args[n+offset],values[n]))return failure("model reservations require finite capacities");
                if constexpr(Action==operation::model_budget)
                    status=config.model_budget(args[0],{values[0],values[1],values[2],values[3],values[4],values[5]});
                else status=config.model_limits(args[0],::uwvm2::utils::container::std_string(args[1]),values);
            }
            if constexpr(Action==operation::model)
            {
                scene::models::plan plan;
                if(!plan.model.identity.assign(args[2])||!scene::artifacts::parse_digest(args[3],plan.model.sha256)||
                   !plan.task.identity.assign(args[4])||!scene::artifacts::parse_digest(args[5],plan.task.sha256)||
                   !plan.parameter_type.assign(args[6])||!plan.output_type.assign(args[7])||!scene::models::select_codec(args[8],plan))
                    return failure("model requires exact profiles, lowercase SHA256 and an implemented transport codec");
                if(args[9]!="stream"&&args[9]!="terminal")return failure("model output mode requires stream or terminal");
                plan.streaming=args[9]=="stream";
                status=config.model_plan(args[0],::uwvm2::utils::container::std_string(args[1]),plan);
            }
            if constexpr(Action==operation::model_kernel_channel||Action==operation::model_channel)
            {
                std::array<std::uint64_t,10> numbers{};
                for(std::size_t i{};i<4;++i)if(!scene::parse_unsigned(args[i+2],numbers[i]))return failure("model descriptor and credentials require exact integers");
                for(std::size_t i{};i<6;++i)if(!scene::parse_unsigned(args[i+12],numbers[i+4]))return failure("model channel limits require finite integers");
                if(numbers[0]>INT32_MAX)return failure("model descriptor exceeds bound");
                scene::model_attachment::protocol::binding binding;
                std::array<scene::model_attachment::protocol::identity*,5> ids{&binding.deployment,&binding.vm,&binding.scope,&binding.provider,&binding.session};
                auto hex=[](std::string_view text,std::span<std::uint64_t> words)noexcept
                {
                    if(text.size()!=words.size()*16)return false;
                    for(std::size_t i{};i<words.size();++i)
                    {
                        std::uint64_t word{};
                        for(unsigned j{};j<8;++j)
                        {
                            auto digit=[](char c)noexcept{return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:-1;};
                            auto high=digit(text[i*16+j*2]),low=digit(text[i*16+j*2+1]);if(high<0||low<0)return false;
                            word|=std::uint64_t(high*16+low)<<(j*8);
                        }
                        words[i]=word;
                    }
                    return true;
                };
                for(std::size_t i{};i<ids.size();++i)if(!hex(args[6+i],*ids[i]))return failure("model channel identities require lowercase hex128");
                if(!hex(args[11],binding.contract))return failure("model channel contract requires lowercase SHA256");
                int key_fd=-1,time_fd=-1;
                if constexpr(Action==operation::model_channel)
                {
                    std::uint64_t key{},clock{};
                    if(!scene::parse_unsigned(args[18],key)||key>INT32_MAX)return failure("TLS identity requires a sealed descriptor");
                    if(!scene::parse_unsigned(args[19],clock)||clock>INT32_MAX)return failure("peer clock requires a provisioned time-namespace descriptor");
                    key_fd=static_cast<int>(key);time_fd=static_cast<int>(clock);
                }
                status=config.model_channel(args[0],args[1],static_cast<int>(numbers[0]),{numbers[1],numbers[2],numbers[3]},binding,
                    {numbers[4],numbers[5],numbers[6],numbers[7],numbers[8],numbers[9],key_fd,time_fd});
            }
            if constexpr(Action==operation::buffer)
            {
                scene::buffers::plan plan;
                if(!scene::wire::utf8({reinterpret_cast<std::uint8_t const*>(args[2].data()),args[2].size()})||
                   !plan.schema.identity.assign(args[2])||!scene::artifacts::parse_digest(args[3],plan.schema.sha256))
                    return failure("buffer profile requires bounded UTF-8 identity and exact lowercase SHA256");
                std::array<std::uint64_t,11> values{};
                for(std::size_t n{};n<values.size();++n)
                    if(args[n+4]=="unlimited"||!scene::parse_unsigned(args[n+4],values[n]))return failure("buffer plan requires finite bounds and closed masks");
                if(values[6]>scene::buffers::maximum_rank)return failure("buffer rank exceeds supported maximum");
                plan.bytes=values[0];plan.objects=values[1];plan.object_bytes=values[2];plan.transfer_bytes=values[3];
                plan.workspace_bytes=values[4];plan.leases=values[5];plan.rank=static_cast<std::size_t>(values[6]);
                plan.elements=values[7];plan.element_mask=values[8];plan.order_mask=values[9];plan.color_mask=values[10];
                status=config.buffer_plan(args[0],::uwvm2::utils::container::std_string(args[1]),plan);
            }
            if constexpr(Action==operation::publication_budget||Action==operation::publication)
            {
                constexpr auto offset=Action==operation::publication_budget?1u:2u;
                std::array<std::uint64_t,2> values{};
                for(std::size_t n{};n<values.size();++n)
                    if(args[n+offset]=="unlimited"||!scene::parse_unsigned(args[n+offset],values[n]))return failure("publication reservation requires finite capacities");
                if constexpr(Action==operation::publication_budget)status=config.publication_budget(args[0],values[0],values[1]);
                else status=config.publication(args[0],::uwvm2::utils::container::std_string(args[1]),values[0],values[1]);
            }
            if constexpr(Action==operation::output_budget||Action==operation::output)
            {
                constexpr auto offset=Action==operation::output_budget?1u:2u;
                std::array<std::uint64_t,Count-offset> values{};
                for(std::size_t n{};n<values.size();++n)
                    if(args[offset+n]=="unlimited"||!scene::parse_unsigned(args[offset+n],values[n]))return failure("output reservations require finite integers");
                if constexpr(Action==operation::output_budget)status=config.output_budget(args[0],values[0],values[1],values[2]);
                else status=config.output(args[0],::uwvm2::utils::container::std_string(args[1]),{values[0],values[1],values[2],values[3]});
            }
            if constexpr(Action==operation::communication_budget||Action==operation::communication_worker_limit)
            {
                constexpr std::size_t at=Action==operation::communication_budget?2:3;std::uint64_t value{};
                if(args[at]=="unlimited"||!scene::parse_unsigned(args[at],value))return failure("communication reservations require exact finite integers");
                if constexpr(Action==operation::communication_budget)status=config.communication_limit(args[0],args[1],value);
                else status=config.communication_worker_limit(args[0],args[1],args[2],value);
            }
            if constexpr(Action==operation::communication_worker)
            {
                std::uint64_t bytes{};scene::buffers::digest digest;
                if(args[5]=="unlimited"||!scene::parse_unsigned(args[5],bytes)||!scene::artifacts::parse_digest(args[6],digest))return failure("communication startup requires exact bytes and lowercase SHA256");
                status=config.communication_worker(args[0],args[1],args[2],args[3],args[4],bytes,digest);
            }
            if constexpr(Action==operation::communication_binding)status=config.communication_binding(args[0],args[1],args[2],args[3]);
            if constexpr(Action==operation::communication_event)
            {
                scene::communication::profile selected;selected.identity=args[3];std::array<std::uint64_t,7> values{};
                if(!scene::artifacts::parse_digest(args[4],selected.digest))return failure("event profile requires lowercase SHA256");
                for(std::size_t i{};i<values.size();++i)if(args[5+i]=="unlimited"||!scene::parse_unsigned(args[5+i],values[i]))return failure("event reservations require finite integers");
                status=config.communication_event(args[0],args[1],args[2],std::move(selected),values);
            }
            if constexpr(Action==operation::communication_pair)
            {
                std::uint64_t deadline{};if(args[4]=="unlimited"||!scene::parse_unsigned(args[4],deadline))return failure("communication retirement requires a finite deadline");
                status=config.communication_pair(args[0],args[1],args[2],args[3],deadline);
            }
            if constexpr(Action==operation::communication_channel)
            {
                scene::communication_attachment::registration registration;
                std::array<std::uint64_t,14> numbers{};
                for(std::size_t i{};i<11;++i)if(!scene::parse_unsigned(args[2+i],numbers[i]))return failure("communication channel requires exact descriptor and identity integers");
                for(std::size_t i{};i<3;++i)if(!scene::parse_unsigned(args[19+i],numbers[11+i]))return failure("communication channel requires finite time and diagnostic bounds");
                if(numbers[0]>INT32_MAX)return failure("communication control descriptor exceeds bound");
                registration.control=static_cast<int>(numbers[0]);registration.peer={numbers[1],numbers[2],numbers[3]};
                for(std::size_t i{};i<7;++i){if(numbers[4+i]>INT32_MAX)return failure("communication pipe exceeds descriptor bound");registration.pipes[i]=static_cast<int>(numbers[4+i]);}
                std::array<scene::drivers::protocol::identity*,5> ids{&registration.identity.deployment,&registration.identity.vm,&registration.identity.module,&registration.identity.driver,&registration.identity.session};
                auto hex=[](std::string_view text,std::span<std::uint64_t> words)noexcept
                {
                    if(text.size()!=words.size()*16)return false;
                    for(std::size_t i{};i<words.size();++i)
                    {
                        std::uint64_t word{};
                        for(unsigned j{};j<8;++j)
                        {
                            auto digit=[](char c)noexcept{return c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:-1;};
                            auto high=digit(text[i*16+j*2]),low=digit(text[i*16+j*2+1]);if(high<0||low<0)return false;
                            word|=std::uint64_t(high*16+low)<<(j*8);
                        }
                        words[i]=word;
                    }
                    return true;
                };
                for(std::size_t i{};i<5;++i)if(!hex(args[13+i],*ids[i]))return failure("communication identities require lowercase hex128");
                if(!hex(args[18],registration.identity.contract))return failure("communication contract requires lowercase SHA256");
                registration.startup_ns=numbers[11];registration.session_until=numbers[12];registration.stderr_bytes=numbers[13];
                status=config.communication_channel(args[0],args[1],registration);
            }
            if constexpr(Action==operation::dump_effective_policy)
            {
                if(current+1!=end)return failure("effective policy dump must terminate the host command");
                for(auto item=begin+1;item!=current;++item)
                    if(item->type==::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)
                        return failure("unexpected argument in offline policy review");
                status=config.validate();
                if(status!=scene::error::success)return failure("scene validation failed before policy output");
                ::uwvm2::utils::container::std_string document;
                status=config.dump_effective_policy(document);
                if(status!=scene::error::success)return failure("effective policy unavailable or bounded output limit reached");
                ::fast_io::io::println(::fast_io::out(),document);return result::return_imme;
            }
            if constexpr(Action==operation::check)status=config.validate();
            if(status==scene::error::unsupported)return failure("purpose/provider has no registered implementation");
            if(status!=scene::error::success)return failure("invalid, duplicate, conflicting or late configuration");
            for(std::size_t i{};i<Count;++i)current[i+1].type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;
            if constexpr(Action==operation::check)
            {::fast_io::io::println("ROS scene policy valid; no provider activated.");return result::return_imme;}
            return result::def;
        }
        catch(std::bad_alloc const&){return failure("configuration allocation failed");}
        catch(std::length_error const&){return failure("configuration exceeds extent bounds");}
    }
}
namespace uwvm2::uwvm::cmdline::params
{
#define UWVM_SCENE_PARAMETER(symbol,flag,description,usage_text,action,arguments) \
    inline constexpr ::uwvm2::utils::cmdline::parameter symbol{ \
        .name{u8##flag},.describe{u8##description},.usage{u8##usage_text}, \
        .handle=::std::addressof(scene_details::configure<scene_details::operation::action,arguments>), \
        .cate=::uwvm2::utils::cmdline::categorization::ros_scene};
    UWVM_SCENE_PARAMETER(ros_scene_call_audit,"--ros-scene-call-audit","Audit completed ROSv2 imports to stderr with intersecting host-only limits. No guest payload is recorded.","<scope:str> <label:str> <mode:str> <records:u64> <bytes:u64> <rate-hz:u64> <burst:u64>",call_audit,7)
    UWVM_SCENE_PARAMETER(ros_scene_driver_schema,"--ros-scene-driver-schema","Require a compiled canonical operation shape for both driver request and reply.","<environment:str> <alias:str> <operation:str> <nodes:u64> <elements:u64> <depth:u64>",driver_schema,6)
    UWVM_SCENE_PARAMETER(ros_scene_driver_budget,"--ros-scene-driver-budget","Reserve external-driver bindings and retained bytes; no process authority.","<environment:str> <bindings:u64> <bytes:u64>",driver_budget,3)
#if defined(UWVM_ROS_TEST_KERNEL_ATTACHMENT)
    UWVM_SCENE_PARAMETER(ros_scene_driver_kernel_channel,"--ros-scene-driver-kernel-channel","Opt into kernel-only driver IPC on a private inherited local socket. This mode sends unencrypted application frames.","<environment:str> <alias:str> <module:str> <fd:u32> <peer-pid:u32> <peer-uid:u32> <peer-gid:u32> <deployment:hex128> <vm:hex128> <module:hex128> <driver:hex128> <session:hex128> <contract:sha256> <request-bytes:u64> <response-bytes:u64> <request-ns:u64> <session-until-ns:u64>",driver_kernel_channel,17)
#endif
    UWVM_SCENE_PARAMETER(ros_scene_driver_channel,"--ros-scene-driver-channel","Require pinned mutual TLS 1.3 and provisioned peer clock evidence. Read the secret only from a sealed identity descriptor.","<environment:str> <alias:str> <module:str> <fd:u32> <peer-pid:u32> <peer-uid:u32> <peer-gid:u32> <deployment:hex128> <vm:hex128> <module:hex128> <driver:hex128> <session:hex128> <contract:sha256> <request-bytes:u64> <response-bytes:u64> <request-ns:u64> <session-until-ns:u64> <identity-fd:u32> <peer-time-namespace-fd:u32>",driver_channel,19)
    UWVM_SCENE_PARAMETER(ros_scene_call_buffer_journal,"--ros-scene-call-buffer-journal","Consume a private writable file descriptor for bounded host-only message capture.","<fd:u32> <records:u64> <file-bytes:u64> <payload-bytes:u64>",module_buffer_journal,4)
    UWVM_SCENE_PARAMETER(ros_scene_call_buffer_capture,"--ros-scene-call-buffer-capture","Persist the complete body of one successful registered message stage to the private host journal.","<environment:str> <route:str> <submit|take|respond|result>",module_buffer_capture,3)
    UWVM_SCENE_PARAMETER(ros_scene_call_budget,"--ros-scene-call-budget","Reserve cross-Wasm message storage; creates no authority.","<environment:str> <bytes:u64> <routes:u64>",module_call_budget,3)
    UWVM_SCENE_PARAMETER(ros_scene_call,"--ros-scene-call","Register an exact caller, callee and bounded message contract; no arbitrary export invocation.","<environment:str> <route:str> <caller:str> <callee:str> <operation:str> <request-schema:str> <response-schema:str> <request-bytes:u64> <response-bytes:u64> <slots:u64> <deadline-ns:u64>",module_call,11)
    UWVM_SCENE_PARAMETER(ros_scene_call_clear_input,"--ros-scene-call-clear-input","Set copied-input erasure for one registered route (default true); false risks retained sender data.","<environment:str> <route:str> <clear-after-copy:bool>",module_call_clear_input,3)
    UWVM_SCENE_PARAMETER(ros_scene_child,"--ros-scene","Configure a ROSv2 child or reduce read-only permission reports; platform enablement grants no authority.","<environment:str> <platform_extension|platform_extension.bind|permission.report> <module-or-function:str> <enable|disable|device-alias|deny>",child,4)
    UWVM_SCENE_PARAMETER(ros_scene_communication_budget,"--ros-scene-communication-budget","Reserve finite communication registries, carriers, worker images and pair supervisor resources; every named field is explicit.","<environment:str> <name:str> <value:u64>",communication_budget,3)
    UWVM_SCENE_PARAMETER(ros_scene_parameter_budget,"--ros-scene-parameter-budget","Set one mandatory finite aggregate parameter reservation; grants no node or parameter key.","<environment:str> <name:str> <value:u64>",parameter_budget,3)
    UWVM_SCENE_PARAMETER(ros_scene_parameter_profile,"--ros-scene-parameter-profile","Bind local parameter semantics to one exact node grant and immutable profiles.","<environment:str> <alias:str> <node-alias:str> <profile:str> <sha256:hex> <node-profile:str> <node-sha256:hex> <algebra:ros1|ros2>",parameter_profile,8)
    UWVM_SCENE_PARAMETER(ros_scene_parameter_limit,"--ros-scene-parameter-limit","Set one explicit parameter storage, parser, cursor or validation bound; zero is literal.","<environment:str> <alias:str> <name:str> <value:u64>",parameter_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_parameter_definition,"--ros-scene-parameter-definition","Read an exact bounded authenticated regular parameter definition file without starting native work.","<environment:str> <alias:str> <path:path> <bytes:u64> <sha256:hex>",parameter_definition,5)
    UWVM_SCENE_PARAMETER(ros_scene_parameter_definition_hex,"--ros-scene-parameter-definition-hex","Parse one bounded canonical parameter definition after explicit profile and capacity configuration.","<environment:str> <alias:str> <definition:hex>",parameter_definition_hex,3)
    UWVM_SCENE_PARAMETER(ros_scene_communication_worker,"--ros-scene-communication-worker","Authenticate a bounded startup definition; grants no capability and starts no native code.","<environment:str> <worker:str> <profile:str> <ros-prefix:str> <absolute-path:str> <bytes:u64> <sha256:hex>",communication_worker,7)
    UWVM_SCENE_PARAMETER(ros_scene_communication_binding,"--ros-scene-communication-bind","Select one exact worker profile for an existing native-communication grant.","<environment:str> <alias:str> <worker:str> <profile:str>",communication_binding,4)
    UWVM_SCENE_PARAMETER(ros_scene_event_stream,"--ros-scene-event-stream","Bind a bounded host-local readiness stream; its source capabilities remain independently granted.","<environment:str> <alias:str> <domain:str> <profile:str> <sha256:str> <streams:u64> <sources:u64> <events:u64> <bytes:u64> <workspace:u64> <deadline_ns:u64> <fields:u64>",communication_event,12)
    UWVM_SCENE_PARAMETER(ros_scene_communication_pair,"--ros-scene-communication-pair","Bind two isolated workers to one explicit transport and cleanup failure domain.","<environment:str> <pair:str> <left-worker:str> <right-worker:str> <retirement-ns:u64>",communication_pair,5)
    UWVM_SCENE_PARAMETER(ros_scene_communication_worker_limit,"--ros-scene-communication-worker-limit","Set a mandatory local RPC or adapter reservation; explicit zero never becomes unlimited.","<environment:str> <worker:str> <name:str> <value:u64>",communication_worker_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_communication_channel,"--ros-scene-communication-channel","Attach a deployment-owned ROS provider through authenticated control and seven directional pipes; no native lifecycle control.","<environment:str> <worker:str> <control-fd:u32> <peer-pid:u32> <peer-uid:u32> <peer-gid:u32> <request-write:u32> <response-read:u32> <diagnostic-read:u32> <discovery-write:u32> <discovery-read:u32> <user-write:u32> <user-read:u32> <deployment:hex128> <vm:hex128> <scope:hex128> <provider:hex128> <session:hex128> <contract:sha256> <startup-ns:u64> <session-until-ns:u64> <stderr-bytes:u64>",communication_channel,22)
    UWVM_SCENE_PARAMETER(ros_scene_create,"--ros-scene-create","Create an empty independent environment; grants no interfaces or resources.","<environment:str>",create,1)
    UWVM_SCENE_PARAMETER(ros_scene_bind,"--ros-scene-bind","Bind one exact module; no fallback to any other host policy.","<environment:str> <module:str>",bind,2)
    UWVM_SCENE_PARAMETER(ros_scene_interface,"--ros-scene-interface","Configure an exact uwvm_ros_v2 function; exposure and resource authority are independent.","<environment:str> <function:str> <hidden|deny|allow>",expose,3)
    UWVM_SCENE_PARAMETER(ros_scene_grant,"--ros-scene-grant","Grant exact purpose, resource, trusted binding and action set. Opening an alias never elevates it.","<environment:str> <alias:str> <purpose:str> <resource:str> <binding:str> <actions:csv>",grant,6)
    UWVM_SCENE_PARAMETER(ros_scene_limit,"--ros-scene-limit","Set a capacity or rate limit. Finite window budgets require delivery=may-reject.","<environment:str> <alias:str> <field:str> <value:str>",limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_buffer_budget,"--ros-scene-buffer-budget","Reserve aggregate buffer bytes, object slots, read workspace and sealed leases; defaults are zero.","<environment:str> <bytes:u64> <objects:u64> <workspace:u64> <leases:u64>",buffer_budget,5)
    UWVM_SCENE_PARAMETER(ros_scene_buffer,"--ros-scene-buffer","Bind an exact buffer schema, finite capacities and closed layout masks; allocates no payload.","<environment:str> <alias:str> <schema:str> <sha256:hex> <bytes:u64> <objects:u64> <object-bytes:u64> <transfer:u64> <workspace:u64> <leases:u64> <rank:u64> <elements:u64> <dtype-mask:u64> <order-mask:u64> <color-mask:u64>",buffer,15)
    UWVM_SCENE_PARAMETER(ros_scene_model_budget,"--ros-scene-model-budget","Reserve aggregate model sessions, requests, pinned inputs, outputs and workspace; all default to zero.","<environment:str> <sessions:u64> <requests:u64> <input-bytes:u64> <output-bytes:u64> <output-items:u64> <workspace:u64>",model_budget,7)
    UWVM_SCENE_PARAMETER(ros_scene_model,"--ros-scene-model","Bind immutable model/task profiles and transport codec; native worker preparation is independently required.","<environment:str> <alias:str> <model:str> <model-sha256:hex> <task:str> <task-sha256:hex> <parameter-type:str> <output-type:str> <text.utf8|bytes> <stream|terminal>",model,10)
    UWVM_SCENE_PARAMETER(ros_scene_model_capacity,"--ros-scene-model-capacity","Set one model binding's finite reservations and deadline independently of optional rate limits.","<environment:str> <alias:str> <sessions:u64> <requests:u64> <input-bytes:u64> <input-count:u64> <parameter-bytes:u64> <output-bytes:u64> <output-items:u64> <workspace:u64> <deadline-ns:u64>",model_capacity,11)
#if defined(UWVM_ROS_TEST_KERNEL_ATTACHMENT)
    UWVM_SCENE_PARAMETER(ros_scene_model_kernel_channel,"--ros-scene-model-kernel-channel","Opt into kernel-only model IPC on a private inherited local socket. This mode sends unencrypted application frames.","<environment:str> <alias:str> <fd:u32> <peer-pid:u32> <peer-uid:u32> <peer-gid:u32> <deployment:hex128> <vm:hex128> <scope:hex128> <provider:hex128> <session:hex128> <contract:sha256> <request-bytes:u64> <response-bytes:u64> <startup-ns:u64> <poll-ns:u64> <supervisor-bytes:u64> <session-until-ns:u64>",model_kernel_channel,18)
#endif
    UWVM_SCENE_PARAMETER(ros_scene_model_channel,"--ros-scene-model-channel","Require pinned mutual TLS 1.3 and provisioned peer clock evidence. Read the secret only from a sealed identity descriptor.","<environment:str> <alias:str> <fd:u32> <peer-pid:u32> <peer-uid:u32> <peer-gid:u32> <deployment:hex128> <vm:hex128> <scope:hex128> <provider:hex128> <session:hex128> <contract:sha256> <request-bytes:u64> <response-bytes:u64> <startup-ns:u64> <poll-ns:u64> <supervisor-bytes:u64> <session-until-ns:u64> <identity-fd:u32> <peer-time-namespace-fd:u32>",model_channel,20)
    UWVM_SCENE_PARAMETER(ros_scene_transform_budget,"--ros-scene-transform-budget","Reserve aggregate transform sessions, queries, data, scratch, query records and carrier copies; no implicit capacity.","<environment:str> <sessions:u64> <queries:u64> <data-bytes:u64> <workspace-bytes:u64> <query-bytes:u64> <carrier-bytes:u64>",transform_budget,7)
    UWVM_SCENE_PARAMETER(ros_scene_transform_profile,"--ros-scene-transform-profile","Select one exact canonical frame definition under a local-transform grant.","<environment:str> <alias:str> <identity:str> <sha256:hex>",transform_profile,4)
    UWVM_SCENE_PARAMETER(ros_scene_transform_limit,"--ros-scene-transform-limit","Set one mandatory finite transform bound by semantic name; zero is literal.","<environment:str> <alias:str> <name:str> <value:u64>",transform_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_transform_definition,"--ros-scene-transform-definition","Read one bounded regular host configuration file and verify its SHA256 before freezing policy; no guest file authority.","<environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>",transform_definition,5)
    UWVM_SCENE_PARAMETER(ros_scene_transform_definition_hex,"--ros-scene-transform-definition-hex","Supply an owned canonical frame definition without filesystem access; same digest and schema checks.","<environment:str> <alias:str> <canonical-bytes:hex>",transform_definition_hex,3)
    UWVM_SCENE_PARAMETER(ros_scene_recording_budget,"--ros-scene-recording-budget","Set one mandatory finite reservation for private recordings and immutable replay cursors.","<environment:str> <name:str> <value:u64>",recording_budget,3)
    UWVM_SCENE_PARAMETER(ros_scene_recording_profile,"--ros-scene-recording-profile","Select an exact hashed ROS channel definition under an existing private-recording grant.","<environment:str> <alias:str> <identity:str> <sha256:hex>",recording_profile,4)
    UWVM_SCENE_PARAMETER(ros_scene_recording_limit,"--ros-scene-recording-limit","Set one mandatory finite recording storage, message or definition capacity; zero remains zero.","<environment:str> <alias:str> <name:str> <value:u64>",recording_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_recording_definition,"--ros-scene-recording-definition","Read one bounded authenticated regular configuration file after grant and capacity checks; grants no guest filesystem access.","<environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>",recording_definition,5)
    UWVM_SCENE_PARAMETER(ros_scene_recording_definition_hex,"--ros-scene-recording-definition-hex","Decode bounded lowercase canonical channel bytes using the compiled schema registry and configured logical digest.","<environment:str> <alias:str> <canonical-bytes:hex>",recording_definition_hex,3)
    UWVM_SCENE_PARAMETER(ros_scene_replay,"--ros-scene-replay","Bind immutable replay to one exact recording source, profile and finite handle count; no paths or native ROS transport.","<environment:str> <alias:str> <source-alias:str> <identity:str> <sha256:hex> <handles:u64>",replay,6)
    UWVM_SCENE_PARAMETER(ros_scene_log_diagnostic_budget,"--ros-scene-log-diagnostic-budget","Set one mandatory finite reservation for private logs and cached diagnostic pages.","<environment:str> <name:str> <value:u64>",log_diagnostic_budget,3)
    UWVM_SCENE_PARAMETER(ros_scene_log_limit,"--ros-scene-log-limit","Set one mandatory record, byte or closed severity bound for an existing private-log grant.","<environment:str> <alias:str> <name:str> <value:u64>",log_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_log_logger,"--ros-scene-log-logger","Allow one exact logger identity; no console, filesystem or network appender is selected.","<environment:str> <alias:str> <logger:str>",log_logger,3)
    UWVM_SCENE_PARAMETER(ros_scene_log_loss,"--ros-scene-log-loss","Explicitly choose reject or drop-oldest behavior when the finite log is full.","<environment:str> <alias:str> <mode:str>",log_loss,3)
    UWVM_SCENE_PARAMETER(ros_scene_diagnostic_limit,"--ros-scene-diagnostic-limit","Set one mandatory scope, field mask, capacity or lifetime for cached diagnostic pages.","<environment:str> <alias:str> <name:str> <value:u64>",diagnostic_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_diagnostic_source,"--ros-scene-diagnostic-source","Select one exact private-log source; every read also needs that source's resource.observe capability.","<environment:str> <alias:str> <source-alias:str>",diagnostic_source,3)
    UWVM_SCENE_PARAMETER(ros_scene_input_budget,"--ros-scene-input-budget","Reserve finite retained immutable-input bytes and objects; both default to zero.","<environment:str> <bytes:u64> <objects:u64>",input_budget,3)
    UWVM_SCENE_PARAMETER(ros_scene_input,"--ros-scene-input","Pin an existing sealed-input grant to an exact SHA256 generation and length; activation occurs only before launch.","<environment:str> <alias:str> <absolute-path:str> <sha256:str> <generation:u64> <bytes:u64> <transfer:u64>",input,7)
    UWVM_SCENE_PARAMETER(ros_scene_lifecycle,"--ros-scene-lifecycle","Bind a finite local lifecycle graph; no native node or device authority.","<environment:str> <alias:str> <handles:u64> <deadline-ns:u64> <transition-mask:u64> <role-mask:u64>",managed_lifecycle,6)
    UWVM_SCENE_PARAMETER(ros_scene_output_budget,"--ros-scene-output-budget","Reserve finite volatile output bytes, objects and last-request recovery bytes; defaults are zero.","<environment:str> <bytes:u64> <objects:u64> <receipt-bytes:u64>",output_budget,4)
    UWVM_SCENE_PARAMETER(ros_scene_output,"--ros-scene-output","Configure an explicit cache grant as a bounded session-local output; no durable storage or host path.","<environment:str> <alias:str> <bytes:u64> <objects:u64> <object-bytes:u64> <transfer:u64>",output,6)
    UWVM_SCENE_PARAMETER(ros_scene_publication_budget,"--ros-scene-publication-budget","Reserve aggregate complete-publication carrier bytes and in-flight calls; defaults are zero.","<environment:str> <bytes:u64> <calls:u64>",publication_budget,3)
    UWVM_SCENE_PARAMETER(ros_scene_publication,"--ros-scene-publication","Bind finite publication scratch to an existing output plan; does not grant any action.","<environment:str> <alias:str> <scratch-bytes:u64> <scratch-calls:u64>",publication,4)
    UWVM_SCENE_PARAMETER(ros_scene_device_budget,"--ros-scene-device-budget","Reserve device/control records, an inherited-placement supervisor and bounded shutdown; grants remain separate.","<environment:str> <devices:u64> <controllers:u64> <retained-bytes:u64> <supervisor-stack-bytes:u64> <poll-ns:u64> <shutdown-ns:u64>",device_budget,7)
    UWVM_SCENE_PARAMETER(ros_scene_device_endpoints,"--ros-scene-device-endpoints","Bind explicit directional command and independent stop channels; raw mode and channel identity are checked only on activation.","<environment:str> <alias:str> <command-tx:u64> <command-ack-rx:u64> <stop-tx:u64> <stop-ack-rx:u64>",device_endpoints,6)
    UWVM_SCENE_PARAMETER(ros_scene_device_profile,"--ros-scene-device-profile","Select an exact digest-bound closed scalar device definition under an existing device grant.","<environment:str> <alias:str> <identity:str> <sha256:hex>",device_profile,4)
    UWVM_SCENE_PARAMETER(ros_scene_device_limit,"--ros-scene-device-limit","Assign one mandatory device capacity; optional rates remain separate.","<environment:str> <alias:str> <name:str> <value:u64>",device_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_device_definition,"--ros-scene-device-definition","Read a bounded authenticated device definition; performs no native device I/O.","<environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>",device_definition,5)
    UWVM_SCENE_PARAMETER(ros_scene_device_definition_hex,"--ros-scene-device-definition-hex","Supply a canonical authenticated closed scalar device definition without file access.","<environment:str> <alias:str> <canonical-bytes:hex>",device_definition_hex,3)
    UWVM_SCENE_PARAMETER(ros_scene_control_profile,"--ros-scene-control-profile","Select an exact digest-bound closed scalar control definition under an existing control grant.","<environment:str> <alias:str> <identity:str> <sha256:hex>",control_profile,4)
    UWVM_SCENE_PARAMETER(ros_scene_control_limit,"--ros-scene-control-limit","Assign one mandatory control capacity; optional rates remain separate.","<environment:str> <alias:str> <name:str> <value:u64>",control_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_control_definition,"--ros-scene-control-definition","Read a bounded authenticated control definition; performs no native device I/O.","<environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>",control_definition,5)
    UWVM_SCENE_PARAMETER(ros_scene_control_definition_hex,"--ros-scene-control-definition-hex","Supply a canonical authenticated closed scalar control definition without file access.","<environment:str> <alias:str> <canonical-bytes:hex>",control_definition_hex,3)
    UWVM_SCENE_PARAMETER(ros_scene_bridge_budget,"--ros-scene-bridge-budget","Reserve finite directed topic bridge providers; creates no input, output or egress authority.","<environment:str> <bindings:u64> <storage-bytes:u64> <supervisor-stack-bytes:u64> <poll-ns:u64>",bridge_budget,5)
    UWVM_SCENE_PARAMETER(ros_scene_bridge_profile,"--ros-scene-bridge-profile","Select an exact digest-bound directed topic bridge definition under an existing bridge grant.","<environment:str> <alias:str> <identity:str> <sha256:hex>",bridge_profile,4)
    UWVM_SCENE_PARAMETER(ros_scene_bridge_limit,"--ros-scene-bridge-limit","Assign one mandatory bridge capacity; optional rates remain separate.","<environment:str> <alias:str> <name:str> <value:u64>",bridge_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_bridge_definition,"--ros-scene-bridge-definition","Read a bounded authenticated bridge definition; never loads codec libraries.","<environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>",bridge_definition,5)
    UWVM_SCENE_PARAMETER(ros_scene_bridge_definition_hex,"--ros-scene-bridge-definition-hex","Supply a canonical authenticated directed topic bridge definition without file access.","<environment:str> <alias:str> <canonical-bytes:hex>",bridge_definition_hex,3)
    UWVM_SCENE_PARAMETER(ros_scene_media_budget,"--ros-scene-media-budget","Reserve finite immutable-buffer media providers; creates no input, output or egress authority.","<environment:str> <bindings:u64> <storage-bytes:u64>",media_budget,3)
    UWVM_SCENE_PARAMETER(ros_scene_media_profile,"--ros-scene-media-profile","Select an exact digest-bound raw media definition under an existing media grant.","<environment:str> <alias:str> <identity:str> <sha256:hex>",media_profile,4)
    UWVM_SCENE_PARAMETER(ros_scene_media_limit,"--ros-scene-media-limit","Assign one mandatory media capacity; optional rates remain separate.","<environment:str> <alias:str> <name:str> <value:u64>",media_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_media_definition,"--ros-scene-media-definition","Read a bounded authenticated media definition; never loads codec libraries.","<environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>",media_definition,5)
    UWVM_SCENE_PARAMETER(ros_scene_media_definition_hex,"--ros-scene-media-definition-hex","Supply a canonical authenticated raw media definition without file access.","<environment:str> <alias:str> <canonical-bytes:hex>",media_definition_hex,3)
    UWVM_SCENE_PARAMETER(ros_scene_sensor_budget,"--ros-scene-sensor-budget","Reserve finite receive-only sensor bindings and native retained storage; grants no read authority.","<environment:str> <bindings:u64> <storage-bytes:u64>",sensor_budget,3)
    UWVM_SCENE_PARAMETER(ros_scene_sensor_profile,"--ros-scene-sensor-profile","Select an authenticated sensor definition and fixed compiled schema octets; no validator address or guest endpoint.","<environment:str> <alias:str> <identity:str> <sha256:hex> <schema:str>",sensor_profile,5)
    UWVM_SCENE_PARAMETER(ros_scene_sensor_limit,"--ros-scene-sensor-limit","Set one mandatory finite sensor capacity after its definition. Configure rate limits separately.","<environment:str> <alias:str> <name:str> <value:u64>",sensor_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_sensor_descriptor,"--ros-scene-sensor-fd","Select a trusted inherited descriptor; activation requires a nonblocking read-only pipe or raw terminal, and check never probes it.","<environment:str> <alias:str> <descriptor:u64>",sensor_descriptor,3)
    UWVM_SCENE_PARAMETER(ros_scene_sensor_definition,"--ros-scene-sensor-definition","Load one bounded exact-hash sensor definition; no native sensor access during configuration.","<environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>",sensor_definition,5)
    UWVM_SCENE_PARAMETER(ros_scene_sensor_definition_hex,"--ros-scene-sensor-definition-hex","Supply authenticated sensor definition bytes as lowercase hex without reading a configuration file.","<environment:str> <alias:str> <canonical-bytes:hex>",sensor_definition_hex,3)
    UWVM_SCENE_PARAMETER(ros_scene_proposal_budget,"--ros-scene-proposal-budget","Reserve exact finite proposal/arbiter bindings and producer/capture recovery tables; zero remains zero.","<environment:str> <providers:u64> <arbiters:u64> <producer-sessions:u64> <capture-handles:u64> <retained-bytes:u64>",proposal_budget,6)
    UWVM_SCENE_PARAMETER(ros_scene_proposal_profile,"--ros-scene-proposal-profile","Select exact proposal semantics and an already configured model/task pair; no implicit source, model or target permission.","<environment:str> <alias:str> <identity:str> <sha256:hex> <model-alias:str>",proposal_profile,5)
    UWVM_SCENE_PARAMETER(ros_scene_proposal_limit,"--ros-scene-proposal-limit","Set one mandatory finite proposal capacity; capture, publish, read and accept rights remain independent.","<environment:str> <alias:str> <name:str> <value:u64>",proposal_limit,4)
    UWVM_SCENE_PARAMETER(ros_scene_proposal_definition,"--ros-scene-proposal-definition","Load one bounded exact-hash source/tool definition; current target adapters accept local nonphysical lifecycle only.","<environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>",proposal_definition,5)
    UWVM_SCENE_PARAMETER(ros_scene_proposal_definition_hex,"--ros-scene-proposal-definition-hex","Supply canonical proposal definition bytes as bounded lowercase hex.","<environment:str> <alias:str> <canonical-bytes:hex>",proposal_definition_hex,3)
    UWVM_SCENE_PARAMETER(ros_scene_proposal_arbiter,"--ros-scene-proposal-arbiter","Bind one existing arbiter grant to exact proposal and target aliases plus a finite tool mask; acceptance never executes.","<environment:str> <alias:str> <proposal-alias:str> <target-alias:str> <tool-mask:u64>",proposal_arbiter,5)
    UWVM_SCENE_PARAMETER(ros_scene_dump_effective_policy,"--ros-scene-dump-effective-policy","Validate and print bounded core scene authorization as JSON. Place last in a scene-only offline command.","",dump_effective_policy,0)
    UWVM_SCENE_PARAMETER(ros_scene_check,"--ros-scene-check","Validate and freeze scene policy without executing guests or activating native providers.","",check,0)
#undef UWVM_SCENE_PARAMETER
}
namespace uwvm2::uwvm::cmdline::params::scene_details
{
    inline scene::deployment& command_configuration(::uwvm2::utils::cmdline::parameter_parsing_results* end)
    {
        if(end[-1].para==&ros_scene_dump_effective_policy)
        {
            static scene::deployment review{scene::deployment::mode::offline_review};
            return review;
        }
        return ::uwvm2::uwvm::imported::scene::configuration;
    }
}
#endif
