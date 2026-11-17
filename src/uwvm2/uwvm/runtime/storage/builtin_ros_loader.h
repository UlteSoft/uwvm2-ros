// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
// Concrete built-in provider provenance. Debug and cache admission retain their
// own sealed censuses; this identity grants neither execution nor resources.
// This layer deliberately imports no ROS Host, cache or checkpoint operations.
#ifndef UWVM_MODULE
# include <array>
# include <cstddef>
# include <cstdint>
# include <memory>
# include <utility>
# include <fast_io.h>
# include <fast_io_crypto.h>
# include <uwvm2/uwvm/wasm/type/impl.h>
# include <uwvm2/uwvm/wasm/storage/impl.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
namespace uwvm2::uwvm::imported::ros
{
    // Explicit global C++ attachment: the definition lives with the genuine
    // ROS tuple, not in a second named-module owner of those concrete types.
    extern "C++"
    {
        class sealed_debug_loader_factory;
    }
}  // namespace uwvm2::uwvm::imported::ros

namespace uwvm2::uwvm::imported::scene { extern "C++" { class sealed_loader_factory; } }

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::runtime::full
{
    enum class builtin_ros_kind : unsigned
    {
        ros1,
        ros1_client,
        ros2,
        ros2_client,
        ros2_publisher,
        ros2_subscription,
        ros2_service,
        portable_robot,
        scene
    };

    struct builtin_ros_function_data
    {
        static constexpr unsigned interface_revision{1u};
        builtin_ros_kind kind{};
        ::std::uint64_t function_index{};
        ::std::array<::std::byte, 32u> interface_sha256{};
        unsigned parameter_count{}, result_count{};
        ::std::array<unsigned char, 16u> parameters{}, results{};
    };

    class builtin_ros_loader_identity final
    {
        friend class ::uwvm2::uwvm::imported::ros::sealed_debug_loader_factory;
        friend class ::uwvm2::uwvm::imported::scene::sealed_loader_factory;
        using module = ::uwvm2::uwvm::wasm::type::local_imported_t;
        using implementation = decltype(::std::declval<module>().ptr);
        implementation implementation_{};
        ::std::size_t ordinal_{}, function_count_{};
        builtin_ros_kind kind_{};
        ::std::array<builtin_ros_function_data, 64u> functions_{};
        ::uwvm2::utils::container::std_unique_ptr<builtin_ros_function_data[]> scene_tail_{};
        ::std::size_t scene_tail_count_{};
        ::std::weak_ptr<builtin_ros_loader_identity const> canonical_owner_{};
        inline static ::std::array<::std::shared_ptr<builtin_ros_loader_identity const>, 9u> selected_{};
        builtin_ros_loader_identity() noexcept = default;

        /** @brief Address already bounded immutable provenance, never a guest index.
         * @details The original 64 entries and legacy provider ceiling remain
         * unchanged. Only the concrete scene factory may reserve a tail, sized
         * from its trusted compiled tuple. No guest controls that count, and
         * adding a trusted import does not require another numeric ceiling.
         * Allocation size overflow is rejected by the recoverable allocator.
         * Silent fast_io allocation failure declines source publication; no
         * partial identity or extra import authority becomes visible. The tail
         * stays owned by the same canonical loader identity through retirement.
         */
        builtin_ros_function_data& captured_function(::std::size_t index) noexcept
        { return index < functions_.size() ? functions_[index] : scene_tail_[index-functions_.size()]; }

        template <class Tuple, ::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
        static constexpr auto const& known_functions(::uwvm2::uwvm::wasm::type::feature_list<Fs...>) noexcept
        { return ::uwvm2::uwvm::wasm::type::details::all_function_information_cache<Tuple, Fs...>::function_information; }

        template <class Concrete>
        bool capture_tuple() noexcept
        {
            auto const& known{known_functions<typename Concrete::local_function_tuple>(::uwvm2::uwvm::wasm::type::binfmt_ver1_feature_list_t{})};
            if(known.size() == 0u || (kind_!=builtin_ros_kind::scene && known.size() > functions_.size())) { return false; }
            if(known.size() > functions_.size())
            {
                static_assert(::std::is_nothrow_default_constructible_v<builtin_ros_function_data>);
                auto count{known.size()-functions_.size()};
                ::uwvm2::utils::container::std_unique_ptr<builtin_ros_function_data[]> tail{
                    ::uwvm2::utils::allocator::fast_io_strict::try_array<builtin_ros_function_data>(count)};
                if(!tail) { return false; }
                scene_tail_=::std::move(tail);
                scene_tail_count_=count;
            }
            ::fast_io::sha256_context hash{};
            auto number = [&](::std::uint64_t n) noexcept
            {
                ::std::array<unsigned char, 8u> bytes{};
                ::fast_io::basic_obuffer_view<unsigned char> out{bytes.data(), bytes.data() + bytes.size()};
                ::fast_io::io::print(out, ::fast_io::mnp::le_put<64u>(n));
                auto p = reinterpret_cast<::std::byte const*>(bytes.data());
                hash.update(p, p + bytes.size());
            };
            auto text = [&](auto name) noexcept
            {
                if(name.empty() || name.size() > 1024u || name.data() == nullptr) { return false; }
                number(name.size());
                auto p = reinterpret_cast<::std::byte const*>(name.data());
                hash.update(p, p + name.size());
                return true;
            };
            auto signature = [](auto const& range, auto& dest, unsigned& count) noexcept
            {
                if(range.begin == nullptr || range.end == nullptr) { return range.begin == nullptr && range.end == nullptr; }
                if(range.end < range.begin) { return false; }
                auto n = static_cast<::std::size_t>(range.end - range.begin);
                if(n > dest.size()) { return false; }
                for(::std::size_t i{}; i != n; ++i)
                {
                    auto code = static_cast<unsigned>(range.begin[i]);
                    if(code != 0x7fu && code != 0x7eu) { return false; }
                    dest[i] = static_cast<unsigned char>(code);
                }
                count = static_cast<unsigned>(n);
                return true;
            };
            number(builtin_ros_function_data::interface_revision);
            number(static_cast<unsigned>(kind_));
            if(!text(Concrete::module_name)) { return false; }
            number(known.size());
            for(::std::size_t i{}; i != known.size(); ++i)
            {
                auto const& entry{known[i]};
                auto& value{captured_function(i)};
                if(entry.index != i || !text(entry.function_name)) { return false; }
                value.kind = kind_;
                value.function_index = i;
                number(i);
                if(!signature(entry.function_type.parameter, value.parameters, value.parameter_count) ||
                   !signature(entry.function_type.result, value.results, value.result_count))
                {
                    return false;
                }
                number(value.parameter_count);
                number(value.result_count);
                auto p = reinterpret_cast<::std::byte const*>(value.parameters.data());
                hash.update(p, p + value.parameter_count);
                auto r = reinterpret_cast<::std::byte const*>(value.results.data());
                hash.update(r, r + value.result_count);
            }
            hash.do_final();
            ::std::array<::std::byte, 32u> digest{};
            hash.digest_to_byte_ptr(digest.data());
            for(::std::size_t i{}; i != known.size(); ++i) { captured_function(i).interface_sha256 = digest; }
            function_count_ = known.size();
            return true;
        }

        // Only the friend defined beside the actual ROS tuples can mint
        // identities. It supplies a concrete type, never an external ptr/ready
        // flag/signature. This method performs the real original construction.
        template <class Concrete>
        static bool append_typed(builtin_ros_kind kind) noexcept
        {
            auto index = static_cast<::std::size_t>(kind);
            if(index >= selected_.size()) { return false; }
            auto& providers{::uwvm2::uwvm::wasm::storage::preload_local_imported};
            selected_[index].reset();  // Old source identities never survive a new concrete insertion.
#if defined(UWVM_CPP_EXCEPTIONS)
            try
            {
                // Both the ordinary CLI cache and the debugger need concrete
                // provider provenance. Mint it at construction, independently
                // of source-image ownership; each consumer still enforces its
                // own source, lifetime and permission admission requirements.
                ::std::shared_ptr<builtin_ros_loader_identity> identity{::uwvm2::utils::container::adopt_shared(::uwvm2::utils::allocator::fast_io_strict::object<builtin_ros_loader_identity>([&](builtin_ros_loader_identity* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) builtin_ros_loader_identity; }))};
                identity->kind_ = kind;
                if(!identity->template capture_tuple<Concrete>()) { return false; }
                auto ordinal = providers.size();
                providers.emplace_back(Concrete{});
                if(ordinal >= providers.size() || providers.size() - ordinal != 1u || providers.index_unchecked(ordinal).ptr == nullptr) { return false; }
                if(identity)
                {
                    identity->ordinal_ = ordinal;
                    identity->implementation_ = providers.index_unchecked(ordinal).ptr;
                    identity->canonical_owner_ = identity;
                    selected_[index] = ::std::move(identity);
                }
                return true;
            }
            catch(...)
            {
                return false;
            }
#else
            providers.emplace_back(Concrete{});
            return true;
#endif
        }

    public:
        using owner = ::std::shared_ptr<builtin_ros_loader_identity const>;
        using census = decltype(selected_);
        builtin_ros_loader_identity(builtin_ros_loader_identity const&) = delete;
        builtin_ros_loader_identity& operator= (builtin_ros_loader_identity const&) = delete;
        ~builtin_ros_loader_identity() = default;

        [[nodiscard]] static census snapshot_current() noexcept { return selected_; }

        [[nodiscard]] static bool canonical(owner const& value) noexcept
        { return value && !value->canonical_owner_.owner_before(value) && !value.owner_before(value->canonical_owner_); }

        [[nodiscard]] static bool still_selected(owner const& value) noexcept
        {
            if(!canonical(value)) { return false; }
            auto i = static_cast<::std::size_t>(value->kind_);
            if(i >= selected_.size()) { return false; }
            auto const& current{selected_[i]};
            return current && current.get() == value.get() && !current.owner_before(value) && !value.owner_before(current);
        }

        // Caller retains actual closed-host/all-stop/publication/source lease.
        // No opaque provider method is called; only the actual container's
        // member is read after ordinal bounds and compared with this factory.
        [[nodiscard]] module const* current_member() const noexcept
        {
            auto const& providers{::uwvm2::uwvm::wasm::storage::preload_local_imported};
            if(ordinal_ >= providers.size()) { return nullptr; }
            auto const& member{providers.index_unchecked(ordinal_)};
            if(member.ptr == nullptr || member.ptr != implementation_) { return nullptr; }
            ::std::size_t memberships{};
            for(auto const& entry: ::uwvm2::uwvm::wasm::storage::all_module)
            {
                if(entry.second.type == ::uwvm2::uwvm::wasm::type::module_type_t::local_import &&
                   entry.second.module_storage_ptr.li == ::std::addressof(member))
                {
                    if(++memberships != 1u) { return nullptr; }
                }
            }
            return memberships == 1u ? ::std::addressof(member) : nullptr;
        }

        [[nodiscard]] bool copy_function(module const* actual, ::std::size_t index, builtin_ros_function_data& out) const noexcept
        {
            if(actual == nullptr || current_member() != actual || index >= function_count_ ||
               function_count_ > functions_.size()+scene_tail_count_) { return false; }
            if(index < functions_.size()) { out = functions_[index]; }
            else
            {
                if(kind_!=builtin_ros_kind::scene || !scene_tail_ || index-functions_.size()>=scene_tail_count_) { return false; }
                out=scene_tail_[index-functions_.size()];
            }
            return true;  // detached DATA, no provider/Guest pointer.
        }
    };
}
