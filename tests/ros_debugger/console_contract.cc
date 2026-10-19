/** Grammar/render/completion component; no Runtime, Gateway or native ROS.
 * The tests deliberately populate detached diagnostic DATA to check escaping
 * and the broker byte limit. This is not evidence of authenticated ROS use.
 */
#include <uwvm2/uwvm/debugger/command.h>
#include <uwvm2/uwvm/debugger/console_completion.h>
#include <uwvm2/uwvm/debugger/console_help_catalog.h>
#include <fast_io_unit/string.h>
#include <string>
#include <limits>
namespace dbg=uwvm2::uwvm::debugger;
namespace ros=dbg::ros_state;
namespace data=uwvm2::imported::ros::debug;
static unsigned checks{};
static void check(bool value,char const* message)
{
    ++checks;if(!value) {fast_io::io::perrln("FAIL_ROS_DEBUG_CONSOLE ",fast_io::mnp::os_c_str(message));fast_io::fast_terminate();}
}
static std::string render(data::view const& value)
{std::string text;fast_io::ostring_ref_std output{&text};ros::print(output,value,19);return text;}
int main()
{
    static_assert(static_cast<unsigned>(dbg::console_command_kind::execution_relocate)==29);
    static_assert(static_cast<unsigned>(dbg::console_command_kind::ros_state)==30);
    for(auto name:{fast_io::string_view{"capabilities"},fast_io::string_view{"sessions"},fast_io::string_view{"queues"},
        fast_io::string_view{"services"},fast_io::string_view{"actions"},fast_io::string_view{"parameters"},
        fast_io::string_view{"tf"},fast_io::string_view{"graph"},fast_io::string_view{"stats"}})
    {
        auto text=fast_io::concat_fast_io("info ros ",name);auto command=dbg::parse_console_command(text.subview(0));
        check(command.kind==dbg::console_command_kind::ros_state&&command.payload_size==0&&
              command.operation==uwvm2::utils::control::operation::status&&!command.wait_for_event,"all nine selectors are read-only status requests");
    }
    auto command=dbg::parse_console_command("info\t ros   queues backend=ros2 instance=18446744073709551615 module=client.wasm alias=events first=3 count=8");
    check(command.kind==dbg::console_command_kind::ros_state&&command.ros_state_request.backend==2&&
          command.ros_state_request.instance==UINT64_MAX&&command.ros_state_request.first==3&&
          command.ros_state_request.module.view()=="client.wasm","outer parser preserves bounded whitespace and u64 filters");
    command=dbg::parse_console_command("trace ros backend=ros1 after=12 count=16 error=0 function=service_submit op=service.call");
    check(command.kind==dbg::console_command_kind::ros_state&&command.ros_state_request.error_filter&&
          command.ros_state_request.function.view()=="service_submit"&&command.ros_state_request.operation.view()=="service.call",
          "trace exposes exact metadata filters without a control operation");
    command=dbg::parse_console_command("info ros stats if-stop=19");
    check(command.kind==dbg::console_command_kind::ros_state&&command.ros_state_request.stop==19,"optional current-stop assertion is DATA only");
    for(auto text:{fast_io::string_view{"info ros"},fast_io::string_view{"info ros unknown"},
        fast_io::string_view{"info ros queues count=0"},fast_io::string_view{"info ros queues count=9"},
        fast_io::string_view{"trace ros count=17"},fast_io::string_view{"trace ros first=0"},
        fast_io::string_view{"info ros stats after=0"},fast_io::string_view{"info ros stats error=0"},
        fast_io::string_view{"info ros stats function=abi_version"},fast_io::string_view{"info ros stats op=topic.publish"},
        fast_io::string_view{"info ros stats count=1 count=1"},fast_io::string_view{"trace ros function=a function=b"},
        fast_io::string_view{"info ros stats backend=ros3"},fast_io::string_view{"info ros stats instance=-1"},
        fast_io::string_view{"trace ros error=4294967296"},fast_io::string_view{"trace ros after=18446744073709551616"},
        fast_io::string_view{"info ros stats first=4097"},fast_io::string_view{"info ros stats module_id=1"},
        fast_io::string_view{"info ros queues clear=1"},fast_io::string_view{"trace ros enable=1"},
        fast_io::string_view{"info ros stats alias="},fast_io::string_view{"info ros stats count=1x"}})
    {check(dbg::parse_console_command(text).kind==dbg::console_command_kind::invalid,"malformed or mutating grammar is refused");}
    check(dbg::parse_console_command("trace ros if-stop=0").kind==dbg::console_command_kind::invalid,"zero explicit stop assertion refuses");
    data::request before{};before.instance=42;data::request query=before;
    check(!ros::parse("trace ros count=0",query)&&query.instance==42,"direct parser leaves caller DATA unchanged on refusal");
    auto long_name=fast_io::concat_fast_io("info ros capabilities module=",std::string(256,'m'));
    check(dbg::parse_console_command(long_name.subview(0)).kind==dbg::console_command_kind::invalid,"truncated filters cannot match a prefix");
    auto long_line=fast_io::concat_fast_io("trace ros ",std::string(513,'x'));
    check(dbg::parse_console_command(long_line.subview(0)).kind==dbg::console_command_kind::invalid,"historical 512-byte command budget remains");

    data::view value{};value.selected=data::selection::queues;value.result=data::status::ok;value.size=1;
    value.rows[0].module.assign("real_module");value.rows[0].generation=7;value.rows[0].alias.assign("a\n\"\\");
    value.rows[0].resource_redacted=true;value.rows[0].resource.assign("file:/not-rendered");
    auto text=render(value);
    check(text.find("scope=process-owned stop=19")!=std::string::npos&&text.find("generation=7 module=\"real_module\"")!=std::string::npos,
          "render identifies actual module name/generation and process-owned scope");
    check(text.find("a\\x0a\\x22\\x5c")!=std::string::npos&&text.find("not-rendered")==std::string::npos,
          "untrusted metadata cannot inject a line or bypass explicit resource redaction");
    value.result=data::status::busy;text=render(value);
    check(text.find("busy (snapshot unavailable)")!=std::string::npos&&text.find("row kind=")==std::string::npos,
          "BUSY never renders a complete empty inventory");
    value.result=data::status::ok;value.size=value.rows.size()+1;
    check(render(value).find("invalid bounded snapshot")!=std::string::npos,"renderer bounds detached row counts");

    // The broker cap includes the rendered footer/header. Test every bounded
    // field with maximally expanded four-byte escapes and maximum u64 values.
    value={};value.result=data::status::ok;value.size=value.rows.size();
    for(auto& row:value.rows)
    {
        row.module.assign(std::string(row.module.bytes.size(),'\1'));row.alias.assign(std::string(row.alias.bytes.size(),'\1'));
        row.operation.assign(std::string(row.operation.bytes.size(),'\1'));row.resource.assign(std::string(row.resource.bytes.size(),'\1'));
        row.type.assign(std::string(row.type.bytes.size(),'\1'));row.instance=row.generation=UINT64_MAX;
        row.open_count=row.active_handles=row.observations=row.requests=row.delivered=row.timeout_ms=UINT64_MAX;
        row.audit_records=row.audit_bytes=row.audit_suppressed=UINT64_MAX;
        row.depth=row.bytes=row.pending_depth=row.pending_bytes=row.reserved_depth=row.reserved_bytes=UINT64_MAX;
        row.high_water_depth=row.high_water_bytes=row.enqueued=row.dequeued=row.dropped_oldest=row.dropped_newest=UINT64_MAX;
        row.rejected=row.backpressured=row.admission_refused=row.completed=row.closed=UINT64_MAX;
        row.operation_tokens=row.creation_tokens=row.byte_tokens=std::numeric_limits<double>::max();
    }
    check(render(value).size()<65536,"maximum escaped state page fits existing broker cap");
    value={};value.result=data::status::ok;value.selected=data::selection::trace;value.trace_size=value.trace.size();
    for(auto& row:value.trace)
    {
        row.module.assign(std::string(row.module.bytes.size(),'\1'));row.alias.assign(std::string(row.alias.bytes.size(),'\1'));
        row.function.assign(std::string(row.function.bytes.size(),'\1'));row.operation.assign(std::string(row.operation.bytes.size(),'\1'));
        row.instance=row.generation=row.sequence=row.monotonic_ns=UINT64_MAX;row.error=UINT32_MAX;
    }
    check(render(value).size()<65536,"maximum escaped trace page fits existing broker cap");
    auto completion=dbg::console_completion::context("info ros que",12);dbg::console_completion::candidates words{};
    dbg::console_completion::ros_words(completion,words);
    check(completion.type==dbg::console_completion::kind::ros&&words.size==1&&words.words[0]=="queues",
          "selector completion is static DATA");
    completion=dbg::console_completion::context("trace ros fun",13);words={};dbg::console_completion::ros_words(completion,words);
    check(words.size==1&&words.words[0]=="function=","trace filter completion is static DATA");
    bool state_help{},trace_help{};
    for(auto const& entry:dbg::console_help::entries) {state_help|=entry.key=="info ros";trace_help|=entry.key=="trace ros";}
    check(state_help&&trace_help,"same help catalogue exposes both commands");
    fast_io::io::println("PASS_ROS_DEBUG_CONSOLE checks=",checks);
}
