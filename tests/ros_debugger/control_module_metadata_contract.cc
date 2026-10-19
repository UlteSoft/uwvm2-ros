/** Real native controller module-metadata dispatch contract.
 *
 * This component uses the production controller, registration API, parser,
 * formatter, and private-control dispatcher. Its copied setup-time labels are
 * explicitly diagnostic fixture metadata, not parsed/JIT-published modules or
 * Guest identities. There is no fake controller, Runtime API stub, transport
 * reply, active Guest, ROS grant, or simulated credential handshake here.
 *
 * A future owner must link the actual matching Runtime/host API closure, bind
 * the current complete source manifest and full consumed headers, and run this
 * bounded executable. Source authorship alone does not qualify a transport,
 * native LLVM replacement, DAP exchange, or any network/official ROS test.
 */
#include <uwvm2/uwvm/debugger/console.h>
#include <fast_io.h>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>

namespace dbg = ::uwvm2::uwvm::debugger;
namespace ctl = ::uwvm2::utils::control;
using view = ::fast_io::string_view;
static unsigned checks{}, groups{};
static void check(bool value, char const* label)
{
    ++checks;
    if(!value)
    {
        ::fast_io::io::perrln("FAIL_CONTROL_MODULE_METADATA ", ::fast_io::mnp::os_c_str(label));
        ::fast_io::fast_terminate();
    }
}
static void group() { ++groups; }
static ::std::string expected(dbg::controller& actual, ::std::uint64_t page)
{
    auto const text{dbg::console_module_help::format(actual.source_module_page(page), false, 80u)};
    return ::std::string{text.data(), text.size()};
}
int main()
{
    ctl::launch_config config{};
    config.debug_enabled = true;
    config.compiler = ctl::backend::llvm;
    config.mode = ctl::compile_mode::full;
    config.origin = ctl::launch_origin::console;
    config.instance[0u] = 1u; // Native component routing identity, not a Guest principal.
#if defined(_WIN32) && !defined(__CYGWIN__)
    config.vm_process = ::fast_io::win32::GetCurrentProcessId();
#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    config.vm_process = static_cast<::std::uint64_t>(dbg::posix_abi::getpid_noexcept());
#else
# error an actual native process identity adapter is required
#endif
    auto actual{dbg::controller::create(config, 1u)};
    check(actual && actual->status() == ctl::error::none, "actual native console authority");
    auto const list{dbg::parse_console_command("info modules")};
    check(list.kind == dbg::console_command_kind::module_list && list.module_help_page == 1u,
          "production parser identifies the default metadata page");
    auto text{dbg::details::execute_control_command(*actual, list)};
    check(text == expected(*actual, 1u) && text == "No Wasm module metadata is available in this session.\n",
          "empty real inventory uses the existing human-console renderer");
    group();

    // Sequential IDs here are assigned explicitly by this native fixture.
    // They are not guesses about an executed Wasm module or a runtime registry.
    for(::std::uint64_t id{}; id != 5u; ++id)
    {
        auto name{::fast_io::concat_fast_io(::fast_io::mnp::dec(9u - id))};
        auto file{::fast_io::concat_fast_io("/fixture/", ::fast_io::mnp::dec(id), ".wasm")};
        check(actual->install_source_module(id, {}, {}, false, name.subview(0u), file.subview(0u)),
              "production setup API accepts actual copied diagnostic labels");
        name.clear(); file.clear();
    }
    auto first{actual->source_module_page(1u)};
    check(first.valid && first.total == 5u && first.pages == 2u && first.size == 4u &&
          first.rows[0u].identifier == 0u && first.rows[3u].identifier == 3u && first.rows[0u].name == "9",
          "page is derived from controller-owned registration and copies caller labels");
    group();

    // This is deliberately not added to controller::execute's Runtime grammar.
    // It must keep rejecting the unset protocol operation; the trusted native
    // adapter handles this read-only display kind before that entry point.
    check(actual->execute(list).status == ctl::error::malformed,
          "metadata does not become a new generic Runtime wire operation");
    text = dbg::details::execute_control_command(*actual, list);
    check(text == expected(*actual, 1u) && text.find("0 = 0.wasm") != ::std::string::npos &&
          text.find("name=\"9\"") != ::std::string::npos && text.find("Next: info modules 2") != ::std::string::npos,
          "private metadata dispatch returns actual first page rather than malformed11");
    group();

    auto page{dbg::parse_console_command("info modules 2")};
    auto last{actual->source_module_page(page.module_help_page)};
    text = dbg::details::execute_control_command(*actual, page);
    check(last.valid && last.size == 1u && last.rows[0u].identifier == 4u && text == expected(*actual, 2u) &&
          text.find("4 = 4.wasm") != ::std::string::npos && text.find("Next:") == ::std::string::npos,
          "actual final page contains only remaining controller metadata");
    for(auto number : {::std::uint64_t{0u}, ::std::uint64_t{3u}, (::std::numeric_limits<::std::uint64_t>::max)()})
    {
        auto invalid{list}; invalid.module_help_page = number;
        auto const refused{dbg::details::execute_control_command(*actual, invalid)};
        check(!actual->source_module_page(number).valid && refused == expected(*actual, number) && refused.starts_with("error:"),
              "zero, unavailable and overflowing-offset pages refuse without indexing");
    }
    check(dbg::parse_console_command("info modules 0").kind == dbg::console_command_kind::invalid,
          "ordinary input parser still rejects zero pagination before native dispatch");
    group();

    // A numeric-looking display name is not an alternate selector. The same
    // current parser that later consumes user commands still demands IDs.
    check(text.find("numeric ID before '='") != ::std::string::npos,
          "module page explicitly distinguishes display names from authority");
    check(dbg::parse_console_command("bs name.wasm fixture.cc:1").kind == dbg::console_command_kind::invalid &&
          dbg::parse_console_command("bs 0 fixture.cc:1").kind == dbg::console_command_kind::source_breakpoint,
          "existing strict numeric module selection remains unchanged");
    group();

    auto name{::fast_io::concat_fast_io("name\x1b[2J\n", ::std::string(4096u, 'x'))};
    auto file{::fast_io::concat_fast_io("/fixture/", ::std::string(4096u, 'f'))};
    for(::std::uint64_t id{}; id != 4u; ++id)
    {
        check(actual->install_source_module(id, {}, {}, false, name.subview(0u), file.subview(0u)),
              "real registration retains its existing per-label bound");
    }
    text = dbg::details::execute_control_command(*actual, list);
    check(text == expected(*actual, 1u) && text.find('\x1b') == ::std::string::npos &&
          text.find("\\x") != ::std::string::npos && text.size() < 65536u,
          "actual registered control bytes are escaped and page fits existing transport cap");
    group();

    auto before{::std::make_unique<dbg::controller_reply>(actual->inspect())};
    static_cast<void>(dbg::details::execute_control_command(*actual, list));
    static_cast<void>(dbg::details::execute_control_command(*actual, page));
    auto after{::std::make_unique<dbg::controller_reply>(actual->inspect())};
    check(before->status == after->status && before->execution == after->execution &&
          before->stop_identifier == after->stop_identifier && before->threads.size() == after->threads.size() &&
          !actual->domain()->is_closed(), "metadata queries do not park, resume, close or mint a current stop");
    group();

    // These actual read-only commands are stable across consecutive calls.
    // Compare with the original expression, not invented reply bytes, so every
    // non-metadata path keeps the existing execution and error formatting.
    for(auto input : {view{"status"}, view{"info threads"}, view{"info modules 0"}})
    {
        auto const command{dbg::parse_console_command(input)};
        auto const legacy{dbg::details::format_reply(actual->execute(command), command)};
        check(dbg::details::execute_control_command(*actual, command) == legacy,
              "ordinary command and malformed-command behavior is byte-identical");
    }
    group();
    check(groups == 8u, "all eight source-defined component groups reached completion");
    ::fast_io::io::println("PASS_CONTROL_MODULE_METADATA groups=", groups, " checks=", checks);
}
