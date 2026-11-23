#include <uwvm2/uwvm/debugger/command.h>
#include <uwvm2/uwvm/debugger/host_interfaces.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace dbg = uwvm2::uwvm::debugger;
namespace permissions = uwvm2::utils::control::debug_permissions;

#define REQUIRE(expression) do { if (!(expression)) { \
    std::fprintf(stderr, "FAIL line %u: %s\n", __LINE__, #expression); std::exit(1); } } while (false)

static auto parse(std::string const& text)
{
    return dbg::parse_console_command(fast_io::string_view{text.data(), text.size()});
}

int main(int argc, char** argv)
{
    REQUIRE(argc == 2);
    auto level = permissions::permission_level::none;
    if (!std::strcmp(argv[1], "full")) level = permissions::permission_level::full;
    else if (!std::strcmp(argv[1], "read-only")) level = permissions::permission_level::read_only;
    else if (!std::strcmp(argv[1], "limited")) level = permissions::permission_level::limited;
    else REQUIRE(!std::strcmp(argv[1], "none"));
    REQUIRE(permissions::configure_trusted_launch(level));
    for (auto const& input : {"info host-interfaces", "info host-interfaces 32", "info host-interfaces 18446744073709551615"})
    {
        auto command = parse(input);
        REQUIRE(command.kind == dbg::console_command_kind::host_interfaces);
        REQUIRE(dbg::command_permission_allowed(command) == permissions::inspection_allowed());
    }
    REQUIRE(parse("info host-interfaces").module_help_page == 0);
    REQUIRE(parse("info host-interfaces 18446744073709551615").module_help_page == UINT64_MAX);
    for (auto const& input : {"info host-interfaces -1", "info host-interfaces 18446744073709551616", "info host-interfaces 0 extra"})
        REQUIRE(parse(input).kind == dbg::console_command_kind::invalid);

#if defined(UWVM_ENABLE_PRELOAD_DL) && UWVM_ENABLE_PRELOAD_DL
    // Metadata is copied without consulting a library, opening its path, or
    // resolving its function addresses. Include control bytes and long names
    // to verify transport framing and explicit truncation on later pages.
    auto& libraries = uwvm2::uwvm::wasm::storage::preloaded_dl;
    std::u8string long_name(4100, u8'x');
    for (unsigned index{}; index != 35; ++index)
    {
        libraries.emplace_back();
        auto& library = libraries.back();
        library.module_name = uwvm2::utils::container::u8string_view{u8"driver\n<module>", 15};
        library.file_name = uwvm2::utils::container::u8cstring_view{u8"/nonexistent/driver\n.so"};
        library.wasm_dl_storage.capi_function_vec.function_size = index + 1;
    }
    libraries.back().module_name = uwvm2::utils::container::u8string_view{long_name.data(), long_name.size()};
#endif
    for (auto first : {std::uint64_t{0}, std::uint64_t{32}, std::uint64_t{100}, UINT64_MAX})
    {
        auto text = dbg::host_interfaces::inventory(first);
        REQUIRE(text.find("native-pc") == text.npos);
        std::fputs("PAGE\n", stdout);
        REQUIRE(std::fwrite(text.data(), 1, text.size(), stdout) == text.size());
    }
#if defined(UWVM_ENABLE_PRELOAD_DL) && UWVM_ENABLE_PRELOAD_DL
    std::u8string long_path(4100, u8'p');
    for (auto& library : libraries)
    {
        library.module_name = uwvm2::utils::container::u8string_view{long_name.data(), long_name.size()};
        library.file_name = uwvm2::utils::container::u8cstring_view{fast_io::containers::null_terminated, long_path.c_str(), long_path.size()};
    }
    std::size_t first{};
    while (first != libraries.size())
    {
        auto text = dbg::host_interfaces::inventory(first);
        REQUIRE(text.size() <= 65536);
        auto count_at = text.find(" count=");
        REQUIRE(count_at != text.npos);
        auto count = std::strtoull(text.data() + count_at + 7, nullptr, 10);
        REQUIRE(count > 0 && count <= 3 && count <= libraries.size() - first);
        first += count;
        std::fputs("LONG-PAGE\n", stdout);
        REQUIRE(std::fwrite(text.data(), 1, text.size(), stdout) == text.size());
    }
#endif
    std::fputs("PASS host inventory and permission gate\n", stdout);
}
