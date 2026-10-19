/*************************************************************
 * Test-only real RMW Graph dispatch observer. APL-2.0.      *
 *************************************************************/
#pragma once
// Include this header in a separately qualified test Gateway translation unit
// before including gateways/ros2/main.cc, and link libdl. Ordinary Gateways do
// not include it. Constructors, queries, results and profiles are never mocked.
// The observer forwards each call to the actual selected RMW implementation and
// records that native invocation's outcome in a bounded task-owned ledger.
// Set UWVM_ROS_TEST_GRAPH_LEDGER to an already created empty 0600 regular file,
// and UWVM_ROS_TEST_GRAPH_NODE to the exact trusted Gateway node name. Compare
// before/after ledger rows for each controlled Guest test; initialization and
// other native calls require separate baselines and cannot be called Guest work.
#include "../../gateways/common/native_io.h"
#include <rmw/rmw.h>
#include <rmw/get_topic_names_and_types.h>
#include <rmw/get_service_names_and_types.h>
#include <rmw/get_node_info_and_types.h>
#include <rmw/get_topic_endpoint_info.h>
#include <dlfcn.h>
#include <atomic>
#include <cstdlib>
#include <cstring>

namespace uwvm_ros_graph_observer
{
    namespace io=uwvm2::ros_gateway::native_io;
    template<typename Function> Function next(char const* name) noexcept
    {return reinterpret_cast<Function>(::dlsym(RTLD_NEXT,name));}
    struct ledger
    {
        io::owned_fd file;
        std::array<char,256> node{};
        std::size_t node_size{};
        std::atomic<std::uint64_t> sequence{};
        bool valid{};
        ledger():file{open_file()}
        {
            auto name{::getenv("UWVM_ROS_TEST_GRAPH_NODE")};
            bool selected{::getenv("UWVM_ROS_TEST_GRAPH_LEDGER")||name};
            if(!name||file.get()<0) {if(selected) {::_exit(74);}return;}
            while(node_size<node.size()&&name[node_size])
            {
                char c{name[node_size]};
                if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_')) {::_exit(74);}
                node[node_size++]=c;
            }
            valid=node_size&&node_size<node.size();
            if(!valid) {::_exit(74);}
            struct stat state{};if(::fstat(file.get(),&state)) {::_exit(74);}
            io::fixed_text<128> header;
            if(!header.append("RMW_GRAPH_LEDGER ")||!header.number(static_cast<std::uint64_t>(state.st_dev))||
               !header.append(" ")||!header.number(static_cast<std::uint64_t>(state.st_ino))||!header.append("\n")) {::_exit(74);}
            if(!io::write_all(file.get(),header.view())) {::_exit(74);}
        }
        static int open_file() noexcept
        {
            auto path{::getenv("UWVM_ROS_TEST_GRAPH_LEDGER")};
            if(!path||path[0]!='/') {return -1;}
            int fd{::open(path,O_WRONLY|O_APPEND|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK)};struct stat state{};
            if(fd<0) {return -1;}
            if(::fstat(fd,&state)||!S_ISREG(state.st_mode)||state.st_uid!=::geteuid()||
               (state.st_mode&0777)!=0600||state.st_size!=0)
            {(void)::close(fd);return -1;}return fd;
        }
        void record(char const* method,rmw_node_t const* source,rmw_ret_t result) noexcept
        {
            if(!valid||!source||!source->name||std::string_view{source->name}!=std::string_view{node.data(),node_size}) {return;}
            auto id{sequence.fetch_add(1,std::memory_order_relaxed)+1};
            // Refuse an overflowing ledger instead of silently reporting zero.
            // Exit belongs to this test-only instrumented process; the runner
            // must treat the nonzero exit and unfinished receipt as FAIL.
            if(id>4096) {::_exit(74);}
            io::fixed_text<128> line;
            if(!line.append("RMW_GRAPH ")||!line.number(id)||!line.append(" ")||!line.append(method)||
               !line.append(" ")||!line.number(result)||!line.append("\n")) {::_exit(74);}
            ssize_t written;
            do {written=::write(file.get(),line.view().data(),line.view().size());}while(written<0&&errno==EINTR);
            if(written<0||static_cast<std::size_t>(written)!=line.view().size()) {::_exit(74);}
        }
    };
    inline ledger& sink() {static ledger value;return value;}
    inline rmw_ret_t observed(char const* name,rmw_node_t const* node,rmw_ret_t result) noexcept
    {sink().record(name,node,result);return result;}
    inline rmw_ret_t missing(char const* name,rmw_node_t const* node) noexcept
    {return observed(name,node,RMW_RET_UNSUPPORTED);}
}

extern "C" rmw_ret_t rmw_get_topic_names_and_types(rmw_node_t const* node,rcutils_allocator_t* allocator,bool demangle,rmw_names_and_types_t* output)
{
    using function=rmw_ret_t(*)(rmw_node_t const*,rcutils_allocator_t*,bool,rmw_names_and_types_t*);
    static auto real{uwvm_ros_graph_observer::next<function>("rmw_get_topic_names_and_types")};
    return real?uwvm_ros_graph_observer::observed("topics",node,real(node,allocator,demangle,output)):uwvm_ros_graph_observer::missing("topics",node);
}
extern "C" rmw_ret_t rmw_get_node_names(rmw_node_t const* node,rcutils_string_array_t* names,rcutils_string_array_t* namespaces)
{
    using function=rmw_ret_t(*)(rmw_node_t const*,rcutils_string_array_t*,rcutils_string_array_t*);
    static auto real{uwvm_ros_graph_observer::next<function>("rmw_get_node_names")};
    return real?uwvm_ros_graph_observer::observed("nodes",node,real(node,names,namespaces)):uwvm_ros_graph_observer::missing("nodes",node);
}
extern "C" rmw_ret_t rmw_get_node_names_with_enclaves(rmw_node_t const* node,rcutils_string_array_t* names,rcutils_string_array_t* namespaces,rcutils_string_array_t* enclaves)
{
    using function=rmw_ret_t(*)(rmw_node_t const*,rcutils_string_array_t*,rcutils_string_array_t*,rcutils_string_array_t*);
    static auto real{uwvm_ros_graph_observer::next<function>("rmw_get_node_names_with_enclaves")};
    return real?uwvm_ros_graph_observer::observed("nodes-enclaves",node,real(node,names,namespaces,enclaves)):uwvm_ros_graph_observer::missing("nodes-enclaves",node);
}
extern "C" rmw_ret_t rmw_get_service_names_and_types(rmw_node_t const* node,rcutils_allocator_t* allocator,rmw_names_and_types_t* output)
{
    using function=rmw_ret_t(*)(rmw_node_t const*,rcutils_allocator_t*,rmw_names_and_types_t*);
    static auto real{uwvm_ros_graph_observer::next<function>("rmw_get_service_names_and_types")};
    return real?uwvm_ros_graph_observer::observed("services",node,real(node,allocator,output)):uwvm_ros_graph_observer::missing("services",node);
}
#define UWVM_GRAPH_SERVICE_BY_NODE(Function,Label) \
extern "C" rmw_ret_t Function(rmw_node_t const* node,rcutils_allocator_t* allocator,char const* name,char const* ns,rmw_names_and_types_t* output) \
{ using function=rmw_ret_t(*)(rmw_node_t const*,rcutils_allocator_t*,char const*,char const*,rmw_names_and_types_t*); \
  static auto real{uwvm_ros_graph_observer::next<function>(#Function)}; \
  return real?uwvm_ros_graph_observer::observed(Label,node,real(node,allocator,name,ns,output)):uwvm_ros_graph_observer::missing(Label,node); }
UWVM_GRAPH_SERVICE_BY_NODE(rmw_get_client_names_and_types_by_node,"clients-by-node")
UWVM_GRAPH_SERVICE_BY_NODE(rmw_get_service_names_and_types_by_node,"services-by-node")
#undef UWVM_GRAPH_SERVICE_BY_NODE
#define UWVM_GRAPH_TOPIC_BY_NODE(Function,Label) \
extern "C" rmw_ret_t Function(rmw_node_t const* node,rcutils_allocator_t* allocator,char const* name,char const* ns,bool demangle,rmw_names_and_types_t* output) \
{ using function=rmw_ret_t(*)(rmw_node_t const*,rcutils_allocator_t*,char const*,char const*,bool,rmw_names_and_types_t*); \
  static auto real{uwvm_ros_graph_observer::next<function>(#Function)}; \
  return real?uwvm_ros_graph_observer::observed(Label,node,real(node,allocator,name,ns,demangle,output)):uwvm_ros_graph_observer::missing(Label,node); }
UWVM_GRAPH_TOPIC_BY_NODE(rmw_get_publisher_names_and_types_by_node,"publishers-by-node")
UWVM_GRAPH_TOPIC_BY_NODE(rmw_get_subscriber_names_and_types_by_node,"subscriptions-by-node")
#undef UWVM_GRAPH_TOPIC_BY_NODE
#define UWVM_GRAPH_ENDPOINTS(Function,Label) \
extern "C" rmw_ret_t Function(rmw_node_t const* node,rcutils_allocator_t* allocator,char const* topic,bool demangle,rmw_topic_endpoint_info_array_t* output) \
{ using function=rmw_ret_t(*)(rmw_node_t const*,rcutils_allocator_t*,char const*,bool,rmw_topic_endpoint_info_array_t*); \
  static auto real{uwvm_ros_graph_observer::next<function>(#Function)}; \
  return real?uwvm_ros_graph_observer::observed(Label,node,real(node,allocator,topic,demangle,output)):uwvm_ros_graph_observer::missing(Label,node); }
UWVM_GRAPH_ENDPOINTS(rmw_get_publishers_info_by_topic,"publisher-info")
UWVM_GRAPH_ENDPOINTS(rmw_get_subscriptions_info_by_topic,"subscription-info")
#undef UWVM_GRAPH_ENDPOINTS
