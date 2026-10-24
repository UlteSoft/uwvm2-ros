/** Isolated formatter, bounded stdin queue and canonical TF validation checks.
 * No Runtime, Guest, Gateway, ROS middleware, socket, actor or benchmark. The
 * fixture exercises actual public header functions; it grants no capability.
 * Authored source only until a separately guarded current-source build runs.
 */
#include <uwvm2/uwvm/debugger/console_memory.h>
#include <uwvm2/uwvm/debugger/console_stdio.h>
#include <uwvm2/imported/ros/common/message.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
#include <string>
#include <vector>
namespace memory=uwvm2::uwvm::debugger::console_memory;
namespace console=uwvm2::uwvm::debugger::console_stdio;
namespace ros=uwvm2::imported::ros;
namespace ipc=ros::ipc;
static unsigned checks{},failures{};
static void check(bool value,char const* message)
{
    ++checks;
    if(!value)
    { ++failures;fast_io::io::perrln("FAIL_FAST_IO_CONSOLE_TF ",fast_io::mnp::os_c_str(message)); }
}
static std::string text(fast_io::string const& value)
{ return value.empty()?std::string{}:std::string{value.data(),value.size()}; }
static void memory_group()
{
    check(memory::row_bytes(87)==16&&memory::row_bytes(86)==8&&memory::row_bytes(55)==8&&
          memory::row_bytes(54)==4&&memory::row_bytes(39)==4&&memory::row_bytes(38)==1,
          "all historical width transitions");
    std::array<std::byte,2> bytes{std::byte{0},std::byte{255}};
    check(text(memory::dump(bytes,16,39))=="0x0000000000000010: 00 ff " "      " " |..|\n",
          "full-width address, padded final row and lowercase two-digit byte hex");
    check(text(memory::dump({},0)).empty(),"empty dump");
    check(text(memory::dump(bytes,UINT64_MAX))=="invalid Wasm memory range\n","address overflow refusal");
    check(text(memory::interpret({},"unknown"))=="unknown memory format\n","unknown format refusal");
    check(text(memory::interpret({},"i64"))=="i64=<needs 8 bytes>\n","short numeric extent");
    check(text(memory::interpret({},"all",true)).empty(),"compact mode omits unavailable types");
    std::array<std::byte,1> negative{std::byte{255}};
    check(text(memory::interpret(negative,"i8"))=="i8=-1\n","signed extension");
    check(text(memory::interpret(bytes,"u16"))=="u16=65280\n","little-endian unsigned value");
    std::array<std::byte,4> special{std::byte{'"'},std::byte{'\\'},std::byte{10},std::byte{255}};
    check(text(memory::interpret(special,"char"))==R"(char="\"\\\x0a\xff")" "\n","quoted control and non-ASCII char bytes");
    std::array<std::byte,1> invalid_utf8{std::byte{192}};
    check(text(memory::interpret(invalid_utf8,"utf8"))==R"(utf8="\xc0" (invalid UTF-8))" "\n","invalid UTF-8 remains explicit");
    std::array<std::byte,4> emoji{std::byte{240},std::byte{159},std::byte{152},std::byte{128}};
    check(text(memory::interpret(emoji,"utf8"))=="utf8=\"\xf0\x9f\x98\x80\"\n","valid UTF-8 bytes preserved");
    // Width admission precedes Unicode decoding: one byte needs a complete
    // UTF-16 unit, while three bytes decode A and retain the odd-extent error.
    std::array<std::byte,1> odd_utf16{std::byte{0}};
    check(text(memory::interpret(odd_utf16,"utf16"))=="utf16=<needs 2 bytes>\n","short UTF-16 code-unit extent");
    std::array<std::byte,3> odd_utf16_after_unit{std::byte{'A'},std::byte{0},std::byte{0}};
    check(text(memory::interpret(odd_utf16_after_unit,"utf16"))=="utf16=\"A\" (invalid Unicode/code-unit length)\n",
          "odd code-unit extent after a complete UTF-16 unit");
    std::array<std::byte,2> surrogate{std::byte{0},std::byte{216}};
    check(text(memory::interpret(surrogate,"utf16"))==R"(utf16="\u{d800}" (invalid Unicode/code-unit length))" "\n","unpaired surrogate");
    std::vector<std::byte> all;
    for(unsigned i{};i!=256;++i) { all.push_back(static_cast<std::byte>(i)); }
    auto rendered{text(memory::dump(all,0,87))};
    check(rendered.find("00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f ")!=std::string::npos&&
          rendered.find("f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff ")!=std::string::npos,
          "complete byte-domain endpoint rows");
    check(static_cast<std::size_t>(std::count(rendered.begin(),rendered.end(),'\n'))==16,"one output line per16-byte row");
}
static void reset_console()
{
    // No live WASI callback exists in this isolated fixture. Reset only under
    // the same mutex used by all actual queue operations.
    std::lock_guard lock{console::mutex};console::pending.clear();console::input.clear();
    console::input_head=0;console::enabled.store(false,std::memory_order_release);
}
static std::string read_console(std::size_t maximum)
{
    std::string result(maximum,'?');fast_io::io_scatter_t scatter{result.data(),result.size()};std::size_t consumed{};
    check(console::read(0,&scatter,1,consumed)&&consumed<=maximum,"synchronous bounded read callback");
    result.resize(consumed);return result;
}
static void stdin_group()
{
    reset_console();check(console::queue_input("abcdef"),"input command adds one newline");
    check(read_console(2)=="ab","partial input read");
    { std::lock_guard lock{console::mutex};check(console::input_head==2&&console::input.size()==7,"partial read retains prefix without compaction"); }
    console::enabled.store(true,std::memory_order_release);
    check(read_console(2)=="cd","next partial read starts at head");
    auto audit{console::take()};check(audit.find("[stdin] cd")!=std::string::npos&&audit.find("[stdin] ab")==std::string::npos,
                                  "stdin audit contains only currently consumed bytes");
    console::enabled.store(false,std::memory_order_release);
    check(console::queue_input("Z"),"append with a consumed prefix and spare physical space");
    std::array<char,2> first{};std::array<char,8> last{};
    std::array<fast_io::io_scatter_t,3> scatter{{{first.data(),first.size()},{last.data(),0},{last.data(),last.size()}}};
    std::size_t consumed{};check(console::read(0,scatter.data(),scatter.size(),consumed)&&consumed==5,"scatter order and zero-length entry");
    check(std::string{first.data(),2}=="ef"&&std::string{last.data(),3}=="\nZ\n","fragmented append byte order");
    check(read_console(3).empty(),"empty input remains immediate EOF");
    { std::lock_guard lock{console::mutex};check(console::input.empty()&&console::input_head==0,"drained storage and cursor reset"); }
    consumed=77;check(!console::read(1,nullptr,0,consumed)&&consumed==77,"invalid stream leaves output count untouched");
    reset_console();std::string full(console::capacity-1,'x');
    check(console::queue_input(fast_io::string_view{full.data(),full.size()}),"exact32768 unread bytes accepted");
    check(!console::queue_input(""),"newline cannot exceed full unread quota");
    check(read_console(2)=="xx","consume two bytes from a full queue");
    // A caller-owned view can happen to alias the queue. Snapshot append DATA
    // before compaction so even this native alias remains valid.
    fast_io::string_view alias;
    { std::lock_guard lock{console::mutex};alias=fast_io::string_view{console::input.data(),1}; }
    check(console::queue_input(alias),"aliased append preserved across required compaction");
    { std::lock_guard lock{console::mutex};check(console::input_head==0&&console::input.size()==console::capacity,
                                               "compaction preserves exact physical and unread limit"); }
    auto rest{read_console(console::capacity)};
    check(rest.size()==console::capacity&&rest.substr(rest.size()-3)=="\nx\n","old newline and appended native bytes preserved");
    std::string too_large(console::capacity,'y');check(!console::queue_input(fast_io::string_view{too_large.data(),too_large.size()}),
                                                     "text plus newline cannot exceed limit");
    check(console::queue_input(""),"empty command still queues a newline");
    consumed=9;check(console::read(0,nullptr,0,consumed)&&consumed==0,"zero scatter count does not consume data");
    check(read_console(1)=="\n","newline retained after zero-capacity read");reset_console();
}
static ros::capability_spec tf_capability()
{
    ros::capability_spec cap;cap.limits={{"max-message-bytes","586"},{"source-frame","base"},{"target-frame","map"},
        {"tf.max-translation-abs","1"},{"tf.quaternion-norm-tolerance","0.001"}};return cap;
}
static ros::result validate(ros::version backend,ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes)
{ return ros::validate_transform_payload(backend,cap,std::as_bytes(std::span<std::uint8_t const>{bytes})); }
static void transform_group()
{
    check(!ipc::valid_transform_extent(0)&&!ipc::valid_transform_extent(77)&&ipc::valid_transform_extent(78)&&
          ipc::valid_transform_extent(586)&&!ipc::valid_transform_extent(587)&&
          !ipc::valid_transform_extent((std::numeric_limits<std::size_t>::max)()),"strict decoder extent boundaries");
    ipc::transform_data tf;tf.parent="map";tf.child="base";auto cap{tf_capability()};auto raw{ipc::encode_transform(tf)};
    check(static_cast<bool>(validate(ros::version::ros2,cap,raw)),"valid copied canonical payload fully decoded");
    auto wrong{cap};wrong.limits["source-frame"]="other";
    check(validate(ros::version::ros2,wrong,raw).code==ros::errc::permission_denied,"frame authorization still applies after copy");
    tf.values[0]=2;auto translated{ipc::encode_transform(tf)};
    check(validate(ros::version::ros2,cap,translated).code==ros::errc::permission_denied,"translation bound still applies");
    tf.values[0]=0;tf.sequence=1;auto sequence{ipc::encode_transform(tf)};
    check(validate(ros::version::ros2,cap,sequence).code==ros::errc::invalid_argument&&
          static_cast<bool>(validate(ros::version::ros1,cap,sequence)),"backend sequence policy unchanged");
    std::vector<std::uint8_t> impossible(587);
    check(validate(ros::version::ros2,cap,impossible).code==ros::errc::too_large,"quota refusal precedes structural refusal");
    cap.limits["max-message-bytes"]="4193786";
    auto rejected{validate(ros::version::ros2,cap,impossible)};
    check(rejected.code==ros::errc::invalid_argument&&rejected.detail=="invalid canonical TransformStamped","admitted quota does not admit impossible extent");
    std::vector<std::uint8_t> malformed(78);
    check(validate(ros::version::ros2,cap,malformed).code==ros::errc::invalid_argument,"valid extent alone never admits payload fields");
    ipc::transform_data retained;retained.parent="sentinel";
    check(!ipc::decode_transform(malformed,retained)&&retained.parent=="sentinel","decoder output unchanged on refusal");
    auto missing{cap};missing.limits.erase("max-message-bytes");
    check(validate(ros::version::ros2,missing,impossible).detail=="explicit transform byte quota required","missing quota precedence unchanged");
}
int main()
{
    memory_group();stdin_group();transform_group();
    fast_io::io::println(failures?"FAIL_FAST_IO_CONSOLE_TF_CONTRACT":"PASS_FAST_IO_CONSOLE_TF_CONTRACT",
        " groups=3 checks=",checks," failures=",failures);
    return failures?1:0;
}
