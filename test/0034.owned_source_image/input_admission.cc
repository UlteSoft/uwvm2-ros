// Supply one unused pathname in the test directory. The fixture creates a
// nineteen-byte module with a name section, without executing guest code.
#include <uwvm2/uwvm/wasm/loader/wasm_file.h>

int main(int argc, char** argv)
{
    if(argc != 2) { return 64; }
    namespace wasm = ::uwvm2::uwvm::wasm;
    namespace policy = wasm::storage::module_policy;
    namespace source = wasm::storage::input;
    namespace resource = ::uwvm2::utils::allocator::resource;
    namespace control = ::uwvm2::utils::control;
    using image = control::owned_file_image;
    using result = wasm::loader::load_wasm_file_rtl;
    constexpr unsigned char named[]{0,97,115,109,1,0,0,0, 0,9,4,'n','a','m','e',0,2,1,'m'};
    auto data{::std::as_bytes(::std::span{named})};
    auto path{::uwvm2::utils::container::u8concat(::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(argv[1])))};
    ::fast_io::u8cstring_view filename{::fast_io::containers::null_terminated, path.c_str(), path.size()};
    {
        ::fast_io::native_file file{filename, ::fast_io::open_mode::out | ::fast_io::open_mode::excl};
        ::fast_io::operations::write_all_bytes(file, data.data(), data.data() + data.size());
    }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
    {
        ::fast_io::native_file file{filename, ::fast_io::open_mode::in};
        ::fast_io::native_mmap_options options{::fast_io::mmap_prot::prot_read, ::fast_io::mmap_flags::map_private};
        for(auto extent : {data.size() + 1uz, SIZE_MAX})
        {
            try { ::fast_io::native_file_loader bad{options, ::fast_io::at(file), extent}; return 14; }
            catch(::fast_io::error) {}
        }
        if(::fast_io::file_size(file) != data.size()) { return 15; }
    }
#endif
    constexpr auto pool{static_cast<unsigned>(resource::pool::parser_input)};
    auto& budget{resource::policies[pool]};
    auto& live{resource::live_bytes[pool]};
    budget = {data.size(), data.size()};
    wasm::type::wasm_parameter_t config{};
    config.resolve_module_policy = true;
    auto& group{policy::groups.try_emplace(policy::owned_name{u8"group"}).first->second};
    policy::membership.try_emplace(policy::owned_name{u8"m"}, policy::owned_name{u8"group"});
    if(!policy::set(policy::global, u8"parser.limit.input-bytes", u8"19") ||
       !policy::set(group.settings, u8"parser.limit.input-bytes", u8"18")) { return 1; }
    wasm::type::wasm_file_t loaded{};
    if(wasm::loader::load_wasm_file(loaded, filename, {}, config) != result::load_error || live.load()) { return 2; }
    ::std::optional<::std::size_t> observed_limit{};
    auto denied{source::read_wasm_image(path, {}, config, image::maximum_bytes, &observed_limit)};
    if(denied.error != control::owned_image_error::quota_exceeded || observed_limit != 18uz || live.load()) { return 3; }
    auto& single{policy::singles.try_emplace(policy::owned_name{u8"m"}).first->second};
    if(!policy::set(single, u8"parser.limit.input-bytes", u8"20") ||
       wasm::loader::load_wasm_file(loaded, filename, {}, config) != result::ok || live.load() != data.size()) { return 4; }
    // The full live pool refuses this read before its scoped name can be probed.
    denied = source::read_wasm_image(path, {}, config, image::maximum_bytes, &observed_limit);
    if(denied.error != control::owned_image_error::quota_exceeded || observed_limit || live.load() != data.size()) { return 27; }
    auto measured_input = [&]
    {
        ::std::array<::std::size_t, ::std::size(policy::parser_counts)> words{};
        ::std::size_t i{};
        policy::visit_measured_parser_limits(loaded.wasm_parameter, [&](::std::size_t value) { words[i++] = value; });
        return words[policy::parser_input_index];
    };
    if(measured_input() != data.size() || policy::set(single, u8"parser.limit.input-bytes", u8"18") ||
       policy::set(single, u8"parser.limit.input_bytes", u8"18")) { return 20; }
    budget.single = data.size() + 3uz;
    if(measured_input() != data.size() + 1uz) { return 21; }
    budget.single = data.size();
    if(image::copy_wasm_bytes(data, data.size()) || live.load() != data.size()) { return 5; }
    auto checkpoint{image::copy_owned_bytes(data, data.size())};
    if(!checkpoint || live.load() != data.size()) { return 6; }
    // A second attempted mapping rolls back while the previous successful one survives.
    if(wasm::loader::load_wasm_file(loaded, filename, u8"m", config) != result::load_error ||
       loaded.source_size() != data.size() || live.load() != data.size()) { return 7; }
    loaded = {};
    if(live.load()) { return 8; }
    auto snapshot{source::read_wasm_image(path, u8"m", config, image::maximum_bytes)};
    if(!snapshot || live.load() != data.size()) { return 9; }
    auto begin{snapshot.image->cbegin()};
    if(wasm::loader::load_wasm_file(loaded, filename, u8"m", config, ::std::move(snapshot.image)) != result::ok ||
       loaded.source_cbegin() != begin || live.load() != data.size()) { return 10; }
    loaded = {};
    budget.single = data.size() - 1;
    if(wasm::loader::load_wasm_file(loaded, filename, u8"m", config) != result::load_error || live.load() ||
       loaded.adopt_unparsed_source_image(::std::move(checkpoint.image))) { return 11; }
    budget.single = data.size();
    auto direct{image::copy_owned_bytes(data)};
    if(!direct || !loaded.adopt_unparsed_source_image(::std::move(direct.image)) || live.load() != data.size()) { return 12; }
    loaded = {};
    {
        auto cold{::uwvm2::utils::container::make_shared<wasm::type::wasm_file_t>()};
        auto first{image::copy_wasm_bytes(data, data.size())};
        if(!first || !cold->adopt_unparsed_source_image(::std::move(first.image))) { return 22; }
        auto installed{wasm::type::wasm_file_t::pin_owned_source_image(cold)};
        auto alias{image::retain_or_copy_wasm_bytes(installed, data, data.size())};
        if(!installed || alias != installed || alias->cbegin() != cold->source_cbegin() || live.load() != data.size() ||
           image::retain_or_copy_wasm_bytes(installed, data, data.size() - 1uz)) { return 23; }
        auto next{::std::to_array(named)};
        next.back() = 'n';
        auto changed{::std::as_bytes(::std::span{next})};
        if(image::retain_or_copy_wasm_bytes(installed, changed, data.size()) || live.load() != data.size()) { return 24; }
        cold.reset();
        budget.live = 2 * data.size();
        auto target{image::retain_or_copy_wasm_bytes(installed, changed, data.size())};
        if(!target || target == installed || target->unsigned_bytes().back() != 'n' || live.load() != budget.live) { return 25; }
        auto data_segments{target};
        target.reset();
        installed.reset();
        alias.reset();
        if(live.load() != data.size() || data_segments->unsigned_bytes().back() != 'n') { return 26; }
    }
    policy::layer overflow{};
    budget.single = resource::host_bytes(512ull << 20u);
    if(resource::parser_input_limit(SIZE_MAX) != budget.single) { return 16; }
    budget.single = SIZE_MAX;
    if(live.load() || resource::parser_input_limit(SIZE_MAX) != static_cast<::std::size_t>(PTRDIFF_MAX) ||
       policy::set(overflow, u8"parser.limit.input-bytes", u8"340282366920938463463374607431768211456")) { return 13; }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
    // A private reparse can reject an otherwise valid module without losing
    // the previous mapped generation or its input and parser reservations.
    budget = {data.size(), 2 * data.size()};
    constexpr auto parser{static_cast<unsigned>(resource::pool::parser)};
    auto const previous_live{resource::live_bytes[parser].load()};
    if(wasm::loader::load_wasm_file(loaded, filename, u8"m", config) != result::ok) { return 17; }
    begin = loaded.source_cbegin();
    auto const retained{resource::live_bytes[parser].load()};
    resource::policies[parser].live = retained;
    auto candidate{image::copy_wasm_bytes(data)};
    if(!candidate || wasm::loader::load_wasm_file(loaded, filename, u8"m", config, ::std::move(candidate.image)) != result::wasm_parser_error ||
       retained <= previous_live || loaded.source_cbegin() != begin || live.load() != data.size() || resource::live_bytes[parser].load() != retained) { return 18; }
    loaded = {};
    if(live.load() || resource::live_bytes[parser].load() != previous_live) { return 19; }
#endif
}
