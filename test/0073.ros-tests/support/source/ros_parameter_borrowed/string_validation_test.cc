#include "../../../../../src/uwvm2/imported/ros/ipc/parameter.h"
#include "../../../../../src/ros/gateway/common/native_io.h"
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
    unsigned checks{};
    void require(bool ok) { ++checks;if(!ok) throw std::runtime_error{"borrowed scalar String contract"}; }
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
    std::vector<std::uint8_t> payload(std::string const& text) {
        std::vector<std::uint8_t> out;out.reserve(4+text.size());wire::put(out,text.size(),4);out.insert(out.end(),text.begin(),text.end());return out;
    }
}
int main() {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    try {
        auto grant=cap(1024*1024);
        std::string large(128*1024,'x');large[3]='\0';large.replace(50,3,"\xe4\xb8\xad");
        auto data=payload(large);auto bad=data;bad[54]=0xff;
        auto short_quota=cap(data.size()-1);auto bad_contract=grant;bad_contract.type_identity="builtin:param-bool-v1";
        for(auto const& sample:std::vector<std::vector<std::uint8_t>>{{0,0,0,0},payload("abc"),data}) {
            measured::interval count;auto error=wire::validate_parameter_value(core::version::ros2,grant,sample.data(),sample.size());auto totals=count.finish();
            require(!error);line("validate-only",totals.first,totals.second);
#ifdef UWVM_ROS_BORROWED_STRING_OPTIMIZED
            require(totals.first==0&&totals.second==0);
#endif
            wire::parameter_value decoded;decoded.type=wire::parameter_kind::int64;decoded.integer_value=41;
            require(!wire::validate_parameter_value(core::version::ros2,grant,sample.data(),sample.size(),&decoded));
            require(decoded.type==wire::parameter_kind::string&&payload(decoded.string_value)==sample);
        }
        for(auto const& c:std::vector<std::pair<core::capability_spec,std::uint32_t>>{{short_quota,90},{bad_contract,13}}) {
            wire::parameter_value sentinel;sentinel.type=wire::parameter_kind::int64;sentinel.integer_value=41;
            measured::interval count;auto error=wire::validate_parameter_value(core::version::ros2,c.first,data.data(),data.size(),&sentinel);auto totals=count.finish();
            require(error==c.second&&sentinel.type==wire::parameter_kind::int64&&sentinel.integer_value==41);require(totals.first==0);line("policy-refusal",totals.first,totals.second);
        }
        for(auto const& invalid:std::vector<std::vector<std::uint8_t>>{{},{0,0,0},{1,0,0,0},{0,0,0,0,0},bad}) {
            wire::parameter_value sentinel;sentinel.type=wire::parameter_kind::int64;sentinel.integer_value=41;
            measured::interval count;auto error=wire::validate_parameter_value(core::version::ros2,grant,invalid.data(),invalid.size(),&sentinel);auto totals=count.finish();
            require(error==22&&sentinel.type==wire::parameter_kind::int64&&sentinel.integer_value==41);require(totals.first==0);line("shape-refusal",totals.first,totals.second);
        }
        require(wire::validate_parameter_value(core::version::ros2,grant,nullptr,4)==22);
        measured::interval count;wire::parameter_value decoded;auto error=wire::validate_parameter_value(core::version::ros2,grant,data.data(),data.size(),&decoded);auto totals=count.finish();
        require(!error&&decoded.string_value==large);require(totals.first==1&&totals.second>=large.size()+1&&totals.second<=large.size()+32);line("requested-decode",totals.first,totals.second);
        line("PASS borrowed scalar String checks",checks,0);return 0;
    } catch(std::exception const& e) {
#if defined(__cpp_concepts)
        fast_io::io::perrln(fast_io::mnp::os_c_str(e.what()));
#else
        uwvm2::ros_gateway::native_io::write_all(2,e.what());
#endif
        return 1;
    }
}
