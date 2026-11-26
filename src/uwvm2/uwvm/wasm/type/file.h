/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @copyright   APL-2.0 License
 */

/****************************************
 *  _   _ __        ____     __ __  __  *
 * | | | |\ \      / /\ \   / /|  \/  | *
 * | | | | \ \ /\ / /  \ \ / / | |\/| | *
 * | |_| |  \ V  V /    \ V /  | |  | | *
 *  \___/    \_/\_/      \_/   |_|  |_| *
 *                                      *
 ****************************************/

#pragma once
#include <uwvm2/utils/allocator/resource_limit.h>

#ifndef UWVM_MODULE
// std
# include <cstdint>
# include <cstddef>
# include <cstring>
# include <new>
# include <memory>
# include <type_traits>
# include <utility>
// macro
# include <uwvm2/utils/macro/push_macros.h>
// import
# include <fast_io.h>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/utils/control/owned_file_image.h>
# include <uwvm2/utils/security/wsea.h>
# include <uwvm2/parser/wasm/concepts/impl.h>
# include <uwvm2/parser/wasm/standard/wasm1/type/impl.h>
# include <uwvm2/parser/wasm_custom/customs/impl.h>
# include <uwvm2/uwvm/wasm/base/impl.h>
# include <uwvm2/uwvm/wasm/feature/impl.h>
# include "para.h"
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::wasm::type { struct wasm_file_t; }
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::wasm::loader
{
    // Global C++ linkage keeps this single friend declaration shared by the
    // separate type and loader named modules.
    extern "C++" {
    enum class load_wasm_file_rtl;
    inline constexpr load_wasm_file_rtl load_wasm_file(::uwvm2::uwvm::wasm::type::wasm_file_t&,
        ::uwvm2::utils::container::u8cstring_view, ::uwvm2::utils::container::u8string_view,
        ::uwvm2::uwvm::wasm::type::wasm_parameter_t, ::uwvm2::utils::control::owned_file_image::owner) noexcept;
    }
}
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::wasm::type
{
    class wasm_input_mapping
    {
        using charge = ::uwvm2::utils::allocator::resource::reservation<::uwvm2::utils::allocator::resource::pool::parser_input>;
        charge charge_{};
        ::fast_io::native_file_loader mapping_{};
    public:
        constexpr wasm_input_mapping() noexcept = default;
        constexpr wasm_input_mapping(wasm_input_mapping&&) noexcept = default;
        constexpr wasm_input_mapping& operator=(wasm_input_mapping&& other) noexcept
        {
            if(this != &other)
            {
                mapping_ = ::std::move(other.mapping_);
                charge_ = ::std::move(other.charge_);
            }
            return *this;
        }
        // An embedding caller can hand over a mapping it already owns. The VM
        // acquires the charge on adoption; the ordinary file loader reserves before mapping.
        [[nodiscard]] bool try_adopt(::fast_io::native_file_loader&& mapping) noexcept
        {
            charge pending{};
            if(mapping.capacity() > ::uwvm2::utils::allocator::resource::parser_input_limit(SIZE_MAX) || !pending.resize(mapping.capacity())) { return false; }
            mapping_ = ::std::move(mapping);
            charge_ = ::std::move(pending);
            return true;
        }
        wasm_input_mapping& operator=(::fast_io::native_file_loader&& mapping) noexcept
        {
            if(!try_adopt(::std::move(mapping)))
            { ::uwvm2::utils::allocator::resource::reject_allocation("uwvm: [error] parser: input mapping exceeds input-bytes or input-live-bytes.\n"); }
            return *this;
        }
        template<typename Probe>
        [[nodiscard]] bool load(::fast_io::native_file& file, ::std::size_t limit, Probe&& probe)
        {
            auto const extent{::fast_io::file_size(file)};
            charge pending{};
            if(extent > ::uwvm2::utils::allocator::resource::parser_input_limit(limit) || !pending.resize(extent)) { return false; }
            auto read_at = [&](::std::size_t offset, char8_t* destination, ::std::size_t count)
            {
                if(offset > extent || count > extent - offset) { return false; }
                auto out{reinterpret_cast<::std::byte*>(destination)};
                auto end{out + count};
                while(out != end)
                {
                    auto next{::fast_io::operations::pread_some_bytes(file, out, end, static_cast<::fast_io::intfpos_t>(offset))};
                    if(next == out) { return false; }
                    offset += static_cast<::std::size_t>(next - out);
                    out = next;
                }
                return true;
            };
            if(!probe(extent, read_at) || ::fast_io::file_size(file) != extent) { return false; }
            ::fast_io::native_mmap_options options{::fast_io::mmap_prot::prot_read | ::fast_io::mmap_prot::prot_write,
                                                   ::fast_io::mmap_flags::map_private};
            ::fast_io::native_file_loader mapping{options, ::fast_io::at(file), extent};
            if(::fast_io::file_size(file) != extent) { return false; }
            mapping_ = ::std::move(mapping);
            charge_ = ::std::move(pending);
            return true;
        }
        void close() noexcept { mapping_.close(); charge_.reset(); }
        [[nodiscard]] constexpr bool empty() const noexcept { return mapping_.empty(); }
        [[nodiscard]] constexpr ::std::size_t size() const noexcept { return mapping_.size(); }
        [[nodiscard]] constexpr char const* cbegin() const noexcept { return mapping_.cbegin(); }
        [[nodiscard]] constexpr char const* cend() const noexcept { return mapping_.cend(); }
        [[nodiscard]] constexpr char* begin() noexcept { return mapping_.begin(); }
        [[nodiscard]] constexpr char* end() noexcept { return mapping_.end(); }
    };

    struct wasm_file_t
    {
        // support binfmt ver1 currently
        static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");

        inline static constexpr ::std::size_t sizeof_wasm_file_module_storage_u{sizeof(::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_module_storage_t)};

        union wasm_file_module_storage_u
        {
            // This type can be initialized with all zeros
            ::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_module_storage_t wasm_binfmt_ver1_storage;
            static_assert(::fast_io::freestanding::is_trivially_copyable_or_relocatable_v<decltype(wasm_binfmt_ver1_storage)> &&
                          ::fast_io::freestanding::is_zero_default_constructible_v<decltype(wasm_binfmt_ver1_storage)>);

            /// @todo wasm component module (storage)

            // Full occupancy is used to initialize the union, set the union to all zero.
            [[maybe_unused]] ::std::byte wasm_file_module_storage_u_reserve[sizeof_wasm_file_module_storage_u]{};

            // destructor of 'wasm_file_module_storage_u' is implicitly deleted because variant field 'typeidx_u8_vector' has a non-trivial destructor
            inline constexpr ~wasm_file_module_storage_u() {}

            // The release of wasm_file_module_storage_u is managed by struct wasm1_element_t, there is no issue of raii resources being unreleased.
        };

        // wasm file name
        ::uwvm2::utils::container::u8cstring_view file_name{};
        // Accurate module names that must work
        ::uwvm2::utils::container::u8string_view module_name{};
        // Recording the binfmt version of the wasm module
        ::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32 binfmt_ver{};
        // Memory-mapped or memory-copy (for platforms that don't support memory mapping) open wasm files
        wasm_input_mapping wasm_file{};
    private:
        friend constexpr auto ::uwvm2::uwvm::wasm::loader::load_wasm_file(::uwvm2::uwvm::wasm::type::wasm_file_t&,
            ::uwvm2::utils::container::u8cstring_view, ::uwvm2::utils::container::u8string_view,
            ::uwvm2::uwvm::wasm::type::wasm_parameter_t, ::uwvm2::utils::control::owned_file_image::owner) noexcept
            -> ::uwvm2::uwvm::wasm::loader::load_wasm_file_rtl;
        bool secure_required_{};
        ::std::shared_ptr<::uwvm2::utils::security::wsea::verified_module const> execution_authorization_{};
        // Exclusive stable source bytes, adopted only before parsing. No mutable
        // view escapes; retaining this actual file retains every parsed borrow.
        ::uwvm2::utils::control::owned_file_image::owner source_image_{};
    public:
        [[nodiscard]] bool adopt_unparsed_source_image(::uwvm2::utils::control::owned_file_image::owner image) noexcept
        {
            if(!image || source_image_ || binfmt_ver != 0u || !wasm_file.empty()) { return false; }
            auto const limit{::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1_parameter(wasm_parameter.binfmt1_para).parser_limit.max_input_bytes};
            if(!::uwvm2::utils::control::owned_file_image::admit_wasm_input(image, limit)) { return false; }
            source_image_ = ::std::move(image); return true;
        }
        [[nodiscard]] static ::uwvm2::utils::control::owned_file_image::shared_owner pin_owned_source_image(
            ::std::shared_ptr<wasm_file_t const> const& file) noexcept
        {
            if(!file || !file->source_image_) { return {}; }
            return {file, file->source_image_.get()};
        }
        [[nodiscard]] bool has_owned_source_image() const noexcept { return source_image_ != nullptr; }
        [[nodiscard]] bool requires_verified_execution_authorization() const noexcept { return secure_required_; }
        [[nodiscard]] bool has_verified_execution_authorization() const noexcept
        {
            return source_image_ && execution_authorization_ && binfmt_ver == 1u &&
                execution_authorization_->matches(reinterpret_cast<::std::byte const*>(source_cbegin()), source_size());
        }
        [[nodiscard]] ::std::uint64_t authorized_capabilities() const noexcept
        { return execution_authorization_ ? execution_authorization_->capabilities() : 0u; }
        [[nodiscard]] char const* source_cbegin() const noexcept
        { return source_image_ ? source_image_->cbegin() : wasm_file.cbegin(); }
        [[nodiscard]] char const* source_cend() const noexcept
        { return source_image_ ? source_image_->cend() : wasm_file.cend(); }
        [[nodiscard]] ::std::size_t source_size() const noexcept
        { return source_image_ ? source_image_->size() : wasm_file.size(); }
        // Module parsing results
        wasm_file_module_storage_u wasm_module_storage{};
        // wasm_parameter_t
        ::uwvm2::uwvm::wasm::type::wasm_parameter_t wasm_parameter{};
        // (Optional) Module name + symbol name
        ::uwvm2::parser::wasm_custom::customs::name_storage_t wasm_custom_name{};

        // Since the default is to initialize to binfmt 0, there is no need to do any constructs
        inline constexpr wasm_file_t() noexcept = default;

        inline explicit constexpr wasm_file_t(::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32 new_binfmt_ver) noexcept : binfmt_ver{new_binfmt_ver}
        {
            switch(this->binfmt_ver)
            {
                case 1u:
                {
                    ::new(::std::addressof(this->wasm_module_storage.wasm_binfmt_ver1_storage)) decltype(this->wasm_module_storage.wasm_binfmt_ver1_storage){};
                    break;
                }
                /// @todo wasm component module: judge
                [[unlikely]] default:
                {
                    static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");
                    break;
                }
            }
        }

        inline constexpr wasm_file_t(wasm_file_t const& other) noexcept = delete;

        inline constexpr wasm_file_t(wasm_file_t&& other) noexcept :
            file_name{::std::move(other.file_name)}, module_name{::std::move(other.module_name)}, binfmt_ver{::std::move(other.binfmt_ver)},
            wasm_file{::std::move(other.wasm_file)}, secure_required_{other.secure_required_}, execution_authorization_{::std::move(other.execution_authorization_)}, source_image_{::std::move(other.source_image_)}, wasm_parameter{::std::move(other.wasm_parameter)}, wasm_custom_name{::std::move(other.wasm_custom_name)}
        {
            switch(this->binfmt_ver)
            {
                case 1u:
                {
                    ::new(::std::addressof(this->wasm_module_storage.wasm_binfmt_ver1_storage)) decltype(this->wasm_module_storage.wasm_binfmt_ver1_storage){
                        ::std::move(other.wasm_module_storage.wasm_binfmt_ver1_storage)};
                    break;
                }
                /// @todo wasm component module: judge
                [[unlikely]] default:
                {
                    static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");
                    break;
                }
            }
        }

        inline constexpr wasm_file_t& operator= (wasm_file_t const& other) noexcept = delete;

        inline constexpr wasm_file_t& operator= (wasm_file_t&& other) noexcept
        {
            if(::std::addressof(other) == this) [[unlikely]] { return *this; }

            switch(this->binfmt_ver)
            {
                case 1u:
                {
                    ::std::destroy_at(::std::addressof(this->wasm_module_storage.wasm_binfmt_ver1_storage));
                    break;
                }
                /// @todo wasm component module: judge
                [[unlikely]] default:
                {
                    static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");
                    break;
                }
            }

            this->file_name = ::std::move(other.file_name);
            this->module_name = ::std::move(other.module_name);
            this->binfmt_ver = ::std::move(other.binfmt_ver);
            this->wasm_file = ::std::move(other.wasm_file);
            this->secure_required_ = other.secure_required_;
            this->execution_authorization_ = ::std::move(other.execution_authorization_);
            this->source_image_ = ::std::move(other.source_image_);
            this->wasm_parameter = ::std::move(other.wasm_parameter);
            this->wasm_custom_name = ::std::move(other.wasm_custom_name);

            switch(this->binfmt_ver)
            {
                case 1u:
                {
                    ::new(::std::addressof(this->wasm_module_storage.wasm_binfmt_ver1_storage)) decltype(this->wasm_module_storage.wasm_binfmt_ver1_storage){
                        ::std::move(other.wasm_module_storage.wasm_binfmt_ver1_storage)};
                    break;
                }
                /// @todo wasm component module: judge
                [[unlikely]] default:
                {
                    static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");
                    break;
                }
            }

            return *this;
        }

        inline constexpr ~wasm_file_t()
        {
            switch(this->binfmt_ver)
            {
                case 1u:
                {
                    ::std::destroy_at(::std::addressof(this->wasm_module_storage.wasm_binfmt_ver1_storage));
                    break;
                }
                /// @todo wasm component module: judge
                [[unlikely]] default:
                {
                    static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");
                    break;
                }
            }
        }

        inline constexpr void change_binfmt_ver(::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32 ver) noexcept
        {
            switch(this->binfmt_ver)
            {
                case 1u:
                {
                    ::std::destroy_at(::std::addressof(this->wasm_module_storage.wasm_binfmt_ver1_storage));
                    break;
                }
                /// @todo wasm component module: judge
                [[unlikely]] default:
                {
                    static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");
                    break;
                }
            }

            this->binfmt_ver = ver;

            switch(this->binfmt_ver)
            {
                case 1u:
                {
                    ::new(::std::addressof(this->wasm_module_storage.wasm_binfmt_ver1_storage)) decltype(this->wasm_module_storage.wasm_binfmt_ver1_storage){};
                    break;
                }
                /// @todo wasm component module: judge
                [[unlikely]] default:
                {
                    static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");
                    break;
                }
            }
        }

        template <::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32 binfmt_ver>
        inline constexpr auto& get_curr_binfmt_version_wasm_storage() noexcept
        {
            static_assert(binfmt_ver == 1u, "Unsupported binfmt version");
            static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");
            /// @todo wasm component module: judge
            if constexpr(binfmt_ver == 1u) { return this->wasm_module_storage.wasm_binfmt_ver1_storage; }
        }

        template <::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32 binfmt_ver>
        inline constexpr auto const& get_curr_binfmt_version_wasm_storage() const noexcept
        {
            static_assert(binfmt_ver == 1u, "Unsupported binfmt version");
            static_assert(::uwvm2::uwvm::wasm::feature::max_binfmt_version == 1u, "missing implementation of other binfmt version");
            /// @todo wasm component module: judge
            if constexpr(binfmt_ver == 1u) { return this->wasm_module_storage.wasm_binfmt_ver1_storage; }
        }
    };
}  // namespace uwvm2::uwvm::wasm::storage

UWVM_MODULE_EXPORT namespace fast_io::freestanding
{
    template <>
    struct is_trivially_copyable_or_relocatable<::uwvm2::uwvm::wasm::type::wasm_file_t>
    {
        inline static constexpr bool value = true;
    };
}

#ifndef UWVM_MODULE
// macro
# include <uwvm2/utils/macro/pop_macros.h>
#endif
