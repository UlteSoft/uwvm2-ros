#include "../../gateways/ros2/resources.h"
#include "../../gateways/common/native_io.h"
#include <cstdlib>
#include <new>
#include <thread>
#include <chrono>
#if defined(__cpp_concepts)
#include <fast_io.h>
#endif

namespace measured {
    inline bool active{};
    inline std::size_t calls{},bytes{};
    struct interval {
        interval() { calls=bytes=0;active=true; }
        std::pair<std::size_t,std::size_t> finish() { active=false;return {calls,bytes}; }
        ~interval() { active=false; }
    };
}
void* operator new(std::size_t size) {
    void* p=std::malloc(size?size:1);if(!p) throw std::bad_alloc{};
    if(measured::active) { ++measured::calls;measured::bytes+=size; }return p;
}
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* p) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete(void* p,std::size_t) noexcept { std::free(p); }
void operator delete[](void* p,std::size_t) noexcept { std::free(p); }

namespace {
    namespace core=uwvm2::imported::ros;
    namespace wire=core::ipc;
    namespace resources=uwvm2::ros_gateway::ros2_resources;
    unsigned checks{};
    void require(bool ok) { ++checks;if(!ok) throw std::runtime_error{"borrowed native String result"}; }
    void line(std::string_view name,std::size_t calls,std::size_t bytes) {
#if defined(__cpp_concepts)
        fast_io::io::println(name," calls=",calls," bytes=",bytes);
#else
        uwvm2::ros_gateway::native_io::fixed_text<160> text;
        require(text.append(name)&&text.append(" calls=")&&text.number(calls)&&text.append(" bytes=")&&text.number(bytes)&&text.append("\n"));
        require(uwvm2::ros_gateway::native_io::write_all(1,text.view()));
#endif
    }
    core::capability_spec cap(std::size_t maximum) {
        core::capability_spec out;out.alias="value";out.operation="param.get";out.type="builtin/string-v1";out.type_identity="builtin:param-string-v1";
        out.limits.emplace("max-parameter-bytes",std::to_string(maximum));return out;
    }
    template<class Value> void exercise(std::string_view name,Value const& native,core::capability_spec const& grant,
                                       std::uint32_t expected_error,std::vector<std::uint8_t> const& expected={}) {
        measured::interval count;auto result=resources::parameter_result(grant,native);auto totals=count.finish();
        require(result.error==expected_error&&result.payload==expected);line(name,totals.first,totals.second);
#ifdef UWVM_ROS_BORROWED_STRING_OPTIMIZED
        require(totals.first==(expected_error?0u:1u));
        if(!expected_error) require(totals.second==expected.size());
#endif
    }
    std::vector<std::uint8_t> payload(std::string const& text) {
        std::vector<std::uint8_t> out;out.reserve(4+text.size());wire::put(out,text.size(),4);out.insert(out.end(),text.begin(),text.end());return out;
    }
}
int main() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    try {
        std::string valid(128*1024,'x');valid[3]=char(0);valid.replace(50,3,"中");auto expected=payload(valid);
        rcl_interfaces::msg::ParameterValue message;message.type=rcl_interfaces::msg::ParameterType::PARAMETER_STRING;message.string_value=valid;
        rclcpp::ParameterValue native{valid};auto exact=cap(expected.size());auto small=cap(expected.size()-1);
        exercise("generated-valid",message,exact,0,expected);exercise("native-valid",native,exact,0,expected);
        exercise("generated-byte-refusal",message,small,90);exercise("native-byte-refusal",native,small,90);
        std::string malformed=valid;malformed[50]=char(255);message.string_value=malformed;rclcpp::ParameterValue invalid{malformed};
        exercise("generated-UTF8-refusal",message,exact,22);exercise("native-UTF8-refusal",invalid,exact,22);
        exercise("generated-quota-before-UTF8",message,small,90);exercise("native-quota-before-UTF8",invalid,small,90);
        message.type=rcl_interfaces::msg::ParameterType::PARAMETER_BOOL;rclcpp::ParameterValue wrong{true};
        exercise("generated-kind-refusal",message,exact,95);exercise("native-kind-refusal",wrong,exact,95);
        auto big=valid;big.resize(wire::parameter_wire_limit,'x');message.type=rcl_interfaces::msg::ParameterType::PARAMETER_STRING;message.string_value=big;
        rclcpp::ParameterValue ceiling{big};auto generous=cap(wire::max_payload_bytes);
        exercise("generated-hard-refusal",message,generous,90);exercise("native-hard-refusal",ceiling,generous,90);
        line("PASS borrowed native String checks",checks,0);return 0;
    } catch(std::exception const& e) {
#if defined(__cpp_concepts)
        fast_io::io::perrln(fast_io::mnp::os_c_str(e.what()));
#else
        uwvm2::ros_gateway::native_io::write_all(2,e.what());
#endif
        return 1;
    }
}
