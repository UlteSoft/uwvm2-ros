// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include <limits>
# include <memory>
# include <type_traits>
# include <vector>
# include <fast_io.h>
# include <uwvm2/runtime/checkpoint/host_memory_phase.h>
# include <uwvm2/imported/wasi/wasip1/abi/impl.h>
# include <uwvm2/imported/wasi/wasip1/memory/impl.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::func
{
    // Only an authenticated recording import installs the native consumer.
    // Kernel I/O then sees detached buffers while other guest workers run. The
    // ordinary path retains its original scatter buffers and allocates nothing.
    template<bool Writing,typename Scatter=::fast_io::io_scatter_t>
    class checkpoint_scatter_io final
    {
        using error=::uwvm2::imported::wasi::wasip1::abi::errno_t;
        using consumer=::uwvm2::runtime::checkpoint::host_memory_phase::consumer;
        struct row { ::std::uint64_t raw{};::std::size_t size{};void const* original{}; };
        consumer const* actual_{};
        ::std::vector<row> rows_{};
        ::std::vector<::std::byte> buffer_{};
        Scatter* scatter_{};
        void const* bound_memory_{};
        void* pin_{};
        void (*release_pin_)(void*) noexcept{};
        void (*acquire_pin_)(void*) noexcept{};
        bool suspended_{};
        [[nodiscard]] static void const* base(Scatter const& value) noexcept
        {
            if constexpr(requires{value.iov_base;})return value.iov_base;
            else return value.base;
        }
        [[nodiscard]] static ::std::size_t length(Scatter const& value) noexcept
        {
            if constexpr(requires{value.iov_len;})return value.iov_len;
            else return value.len;
        }
        static void assign(Scatter& value,void const* pointer) noexcept
        {
            if constexpr(requires{value.iov_base;})value.iov_base=const_cast<void*>(pointer);
            else value.base=pointer;
        }
        void resume() noexcept
        {
            if(!suspended_)return;
            // Take the instruction turn before pinning the possibly relocated
            // allocation. Another guest may have grown it during native I/O.
            actual_->begin(actual_->context,true);
            if(acquire_pin_)acquire_pin_(pin_);
            suspended_=false;
        }
        void restore_rows() noexcept
        {
            if(!scatter_)return;
            for(::std::size_t i{};i<rows_.size();++i)assign(scatter_[i],rows_[i].original);
            scatter_=nullptr;
        }
    public:
        constexpr checkpoint_scatter_io() noexcept
        {
            if(!::std::is_constant_evaluated())actual_=::uwvm2::runtime::checkpoint::host_memory_phase::current;
        }
        template<typename Memory>
        constexpr checkpoint_scatter_io(Memory const& memory,
            decltype(::uwvm2::imported::wasi::wasip1::memory::lock_memory(memory))& guard) noexcept
            :checkpoint_scatter_io{}
        {
            if(actual_)
            {
                bound_memory_=::std::addressof(memory);
                if constexpr(requires{guard.enter_operation();guard.exit_operation();})
                {
                    using pin_type=::std::remove_reference_t<decltype(guard)>;
                    pin_=::std::addressof(guard);
                    release_pin_=[](void* pin) noexcept {static_cast<pin_type*>(pin)->exit_operation();};
                    acquire_pin_=[](void* pin) noexcept {static_cast<pin_type*>(pin)->enter_operation();};
                }
            }
        }
        constexpr ~checkpoint_scatter_io()
        {
            if(!::std::is_constant_evaluated()){resume();restore_rows();}
        }
        checkpoint_scatter_io(checkpoint_scatter_io const&)=delete;
        checkpoint_scatter_io& operator=(checkpoint_scatter_io const&)=delete;
        [[nodiscard]] bool remember(::std::uint64_t raw,::std::size_t size) noexcept
        {
            if(!actual_)return true;
#ifdef UWVM_CPP_EXCEPTIONS
            try { rows_.push_back({raw,size,{}});return true; }catch(...){return false;}
#else
            return false;
#endif
        }
        template<typename Memory>
        [[nodiscard]] error detach(Memory const& memory,Scatter* scatter,::std::size_t count) noexcept
        {
            if(!actual_)return error::esuccess;
            // A relocating owner requires its genuine enclosing memory guard.
            // The I/O helper must die before that guard: every error path first
            // reacquires its turn/pin, so the original RAII release stays balanced.
            if constexpr(!Memory::can_mmap)
                if(bound_memory_!=::std::addressof(memory))return error::enotsup;
            if(count!=rows_.size() || scatter_)return error::eio;
#ifdef UWVM_CPP_EXCEPTIONS
            try
            {
                ::std::size_t total{};
                for(::std::size_t i{};i<count;++i)
                {
                    auto& row=rows_[i];
                    if(length(scatter[i])!=row.size || row.size>(::std::numeric_limits<::std::size_t>::max)()-total)return error::eoverflow;
                    // Only mmap addresses survive a grow. Non-mmap rows are
                    // cleared on completion instead of restoring dangling guest
                    // pointers; result publication resolves offsets afresh.
                    if constexpr(Memory::can_mmap)row.original=base(scatter[i]);
                    total+=row.size;
                }
                buffer_.resize(total);::std::size_t cursor{};
                if constexpr(Writing)
                {
                    for(auto const& row:rows_)
                    {
                        if(row.size && !::uwvm2::imported::wasi::wasip1::memory::copy_guest_bytes_unlocked<false>(
                            memory,row.raw,buffer_.data()+cursor,row.size))return error::efault;
                        cursor+=row.size;
                    }
                }
                cursor=0u;
                for(::std::size_t i{};i<count;++i)
                {
                    assign(scatter[i],rows_[i].size?buffer_.data()+cursor:nullptr);cursor+=rows_[i].size;
                }
                scatter_=scatter;
                actual_->end(actual_->context);
                if(release_pin_)release_pin_(pin_);
                suspended_=true;return error::esuccess;
            }
            catch(...){return error::enomem;}
#else
            return error::enotsup;
#endif
        }
        template<typename Memory>
        [[nodiscard]] error commit(Memory const& memory,::std::uint64_t count,
            ::std::uint64_t result_address,::std::size_t result_size,
            ::std::uint64_t extra_address=0u,::std::size_t extra_size=0u) noexcept
        {
            if(!actual_)return error::esuccess;
            resume();restore_rows();
            if(count>buffer_.size())return error::eio;
            if(!::uwvm2::imported::wasi::wasip1::memory::checked_span_unlocked(memory,result_address,result_size))return error::efault;
            if(extra_size && !::uwvm2::imported::wasi::wasip1::memory::checked_span_unlocked(memory,extra_address,extra_size))return error::efault;
            if constexpr(!Writing)
            {
                // Recheck every destination before the first write. A guest
                // may have retagged memory while the native read was blocked.
                ::std::size_t remaining=static_cast<::std::size_t>(count);
                for(auto const& row:rows_)
                {
                    auto bytes=remaining<row.size?remaining:row.size;
                    if(bytes && !::uwvm2::imported::wasi::wasip1::memory::checked_span_unlocked(memory,row.raw,bytes))return error::efault;
                    remaining-=bytes;
                }
                remaining=static_cast<::std::size_t>(count);::std::size_t cursor{};
                for(auto const& row:rows_)
                {
                    auto bytes=remaining<row.size?remaining:row.size;
                    if(bytes && !::uwvm2::imported::wasi::wasip1::memory::copy_guest_bytes_unlocked<true>(
                        memory,row.raw,buffer_.data()+cursor,bytes))return error::efault;
                    remaining-=bytes;cursor+=row.size;
                }
            }
            return error::esuccess;
        }
    };
}
