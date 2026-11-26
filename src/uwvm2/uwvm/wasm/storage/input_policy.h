// Input selection runs against the same open file that supplies the parser.
#pragma once
#include <uwvm2/utils/allocator/resource_limit.h>
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
# include <algorithm>
# include <climits>
# include <cstddef>
# include <cstdint>
# include <optional>
# include <uwvm2/utils/control/owned_file_image.h>
# include <uwvm2/utils/utf/impl.h>
# include "module_policy.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::wasm::storage::input
{
    using input_name = ::uwvm2::utils::container::u8string_view;
    namespace input_resource = ::uwvm2::utils::allocator::resource;
    namespace input_options = ::uwvm2::uwvm::wasm::storage::module_policy;
    using input_parameters = ::uwvm2::uwvm::wasm::type::wasm_parameter_t;
    using input_image = ::uwvm2::utils::control::owned_file_image;
    using input_result = ::uwvm2::utils::control::owned_image_result;

    struct input_range { ::std::size_t begin{}, end{}; };
    [[nodiscard]] inline constexpr bool legal_input_name(input_name value) noexcept
    {
        return ::uwvm2::utils::utf::check_legal_utf8_unchecked<::uwvm2::utils::utf::utf8_specification::utf8_rfc3629>(
            value.cbegin(), value.cend()).err == ::uwvm2::utils::utf::utf_error_code::success;
    }
    // Both the open-handle probe and final parser backing use the same section
    // framing, first-name-section rule and UTF-8 fallback.
    template<typename Source> constexpr input_name framed_policy_name(Source& source, input_name fallback)
    {
        auto field = [&](::std::size_t& at, ::std::size_t end, input_range& payload)
        {
            auto const count{::std::min(5uz, end - at)};
            if(!count) { return false; }
            auto bytes{source.window(at, count)};
            if(bytes.empty()) { return false; }
            ::std::uint32_t size{};
            auto parsed{::fast_io::parse_by_scan(bytes.cbegin(), bytes.cbegin() + count, ::fast_io::mnp::leb128_get(size))};
            if(parsed.code != ::fast_io::parse_code::ok) { return false; }
            auto const start{at + static_cast<::std::size_t>(parsed.iter - bytes.cbegin())};
            if(size > end - start) { return false; }
            payload = {start, start + size};
            at = payload.end;
            return true;
        };
        auto frame = [&](::std::size_t& at, ::std::size_t end, char8_t& id, input_range& payload)
        {
            auto bytes{source.window(at++, 1)};
            if(bytes.empty()) { return false; }
            id = bytes.front();
            return field(at, end, payload);
        };
        ::std::size_t at{8};
        while(at < source.extent)
        {
            char8_t id{};
            input_range section{}, custom{};
            if(!frame(at, source.extent, id, section)) { break; }
            if(id) { continue; }
            auto inner{section.begin};
            if(!field(inner, section.end, custom) || custom.end - custom.begin != 4) { continue; }
            auto kind{source.window(custom.begin, 4)};
            if(kind.empty()) { break; }
            if(kind.subview(0, 4) != u8"name") { continue; }
            while(inner < section.end)
            {
                input_range sub{}, name{};
                if(!frame(inner, section.end, id, sub)) { return fallback; }
                if(id) { continue; }
                auto contents{sub.begin};
                if(field(contents, sub.end, name) && name.begin != name.end)
                { if(auto module{source.read_name(name)}) { return *module; } }
            }
            break;
        }
        return fallback;
    }
    [[nodiscard]] inline constexpr input_name policy_module_name(char8_t const* begin, char8_t const* end, input_name fallback) noexcept
    {
        struct image_view
        {
            char8_t const* data;
            ::std::size_t extent;
            constexpr input_name window(::std::size_t at, ::std::size_t count) const noexcept
            { return input_name{data + at, count}; }
            constexpr ::std::optional<input_name> read_name(input_range range) const noexcept
            {
                auto name{window(range.begin, range.end - range.begin)};
                if(legal_input_name(name)) { return name; }
                return {};
            }
        } source{begin, static_cast<::std::size_t>(end - begin)};
        return framed_policy_name(source, fallback);
    }

    template<typename ReadAt> struct input_probe
    {
        static constexpr ::std::size_t capacity{4096};
        ReadAt& read_at;
        ::std::size_t extent;
        ::std::size_t base{}, length{};
        char8_t buffer[capacity];
        bool failed{};

        input_name window(::std::size_t at, ::std::size_t count)
        {
            if(failed) { return {}; }
            if(at < base || at - base > length || count > length - (at - base))
            {
                base = at;
                length = ::std::min(capacity, extent - at);
                if(!read_at(at, buffer, length)) { failed = true; return {}; }
#if CHAR_BIT > 8
                for(::std::size_t i{}; i != length; ++i) { buffer[i] &= 0xffu; }
#endif
            }
            return input_name{buffer + (at - base), length - (at - base)};
        }
        bool equals(input_range value, input_name key)
        {
            if(value.end - value.begin != key.size()) { return false; }
            ::std::size_t done{};
            while(done != key.size())
            {
                auto const count{::std::min(capacity, key.size() - done)};
                auto data{window(value.begin + done, count)};
                if(failed || data.subview(0, count) != key.subview(done, count)) { return false; }
                done += count;
            }
            return true;
        }
        ::std::optional<input_name> read_name(input_range name)
        {
            auto const size{name.end - name.begin};
            if(size <= capacity)
            {
                auto bytes{window(name.begin, size)};
                if(failed) { return {}; }
                bytes = bytes.subview(0, size);
                if(legal_input_name(bytes)) { return bytes; }
                return {};
            }
            // Long legal names have the same identity as in the full parser.
            // Validate in bounded windows, splitting only at UTF-8 boundaries.
            auto at{name.begin};
            while(at < name.end)
            {
                auto count{::std::min(capacity, name.end - at)};
                auto bytes{window(at, count)};
                if(failed) { return {}; }
                if(count < name.end - at)
                {
                    --count;
                    while(count && (bytes[count] & 0xc0u) == 0x80u) { --count; }
                    if(!count) { return {}; }
                }
                if(!legal_input_name(bytes.subview(0, count))) { return {}; }
                at += count;
            }
            for(auto const& entry : input_options::singles)
            { input_name key{entry.first.cbegin(), entry.first.size()}; if(equals(name, key)) { return key; } }
            for(auto const& entry : input_options::membership)
            { input_name key{entry.first.cbegin(), entry.first.size()}; if(equals(name, key)) { return key; } }
            // A valid unknown name selects only the global layer.
            return input_name{};
        }
    };

    struct input_policy
    {
        input_parameters const& parameters;
        input_name path, rename;

        bool needs_name() const noexcept
        {
            return parameters.resolve_module_policy && rename.empty() &&
                (!input_options::singles.empty() || !input_options::membership.empty());
        }
        ::std::size_t limit_for(input_name name) const noexcept
        {
            auto limit{::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1_parameter(parameters.binfmt1_para).parser_limit.max_input_bytes};
            if(parameters.resolve_module_policy)
            {
                input_options::visit_layers(name, [&](input_options::layer const& layer)
                { if(layer.input_bytes) { limit = *layer.input_bytes; } });
            }
            return input_resource::parser_input_limit(limit);
        }
        ::std::size_t initial_limit() const noexcept
        { return needs_name() ? input_resource::parser_input_limit(SIZE_MAX) : limit_for(rename); }
        template<typename ReadAt> bool probe(::std::size_t extent, ReadAt& read_at) const
        {
            if(!needs_name()) { return extent <= initial_limit(); }
            input_probe<ReadAt> source{read_at, extent};
            auto name{framed_policy_name(source, path)};
            return !source.failed && extent <= limit_for(name);
        }
        bool accepts(char const* bytes, ::std::size_t size) const noexcept
        {
            if(size > initial_limit()) { return false; }
            if(!needs_name()) { return true; }
            auto begin{reinterpret_cast<char8_t const*>(bytes)};
            return size <= limit_for(size < 8u ? path : policy_module_name(begin, begin + size, path));
        }
    };

    [[nodiscard]] inline input_result read_wasm_image(::uwvm2::utils::container::std_u8string const& path,
        input_name rename, input_parameters const& parameters, ::std::size_t maximum)
    {
        using error = ::uwvm2::utils::control::owned_image_error;
        if(maximum > input_image::maximum_bytes) { return {{}, error::quota_exceeded, {}}; }
        input_policy const policy{parameters, input_name{path.data(), path.size()}, rename};
        auto image{input_image::read_wasm(path, ::std::min(maximum, policy.initial_limit()),
            [&](::std::size_t extent, auto& read_at) { return policy.probe(extent, read_at); })};
        if(image && !policy.accepts(image.image->cbegin(), image.image->size()))
        { return {{}, error::quota_exceeded, {}}; }
        return image;
    }
    [[nodiscard]] inline bool admit_owned_input(input_image::owner& image, input_policy const& policy) noexcept
    {
        return input_image::admit_wasm_input(image, policy.initial_limit(),
            [&](input_image const& actual) noexcept { return policy.accepts(actual.cbegin(), actual.size()); });
    }
}
