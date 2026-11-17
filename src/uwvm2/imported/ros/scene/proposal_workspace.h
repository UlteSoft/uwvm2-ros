/** @file @brief Recoverable finite proposal/capture workspace with retained ownership. */
#pragma once
#include "authority.h"
#include <uwvm2/utils/container/shared.h>
namespace uwvm2::imported::ros::scene::proposals
{
    class workspace
    {
        struct account
        {
            std::atomic<std::uint64_t> bytes{},calls{};
            std::uint64_t maximum_bytes{},maximum_calls{};
            std::atomic<bool> retired{};
        };
        std::shared_ptr<account> value;
        static bool add(std::atomic<std::uint64_t>& used,std::uint64_t amount,std::uint64_t maximum) noexcept
        {
            auto current=used.load(std::memory_order_relaxed);
            while(current<=maximum&&amount<=maximum-current)
                if(used.compare_exchange_weak(current,current+amount,std::memory_order_acq_rel)) { return true; }
            return false;
        }
    public:
        class lease
        {
            friend class workspace;
            std::shared_ptr<account> value;
            std::uint64_t bytes_{};
        public:
            lease()=default;lease(lease const&)=delete;lease& operator=(lease const&)=delete;
            lease(lease&& other) noexcept:value(std::move(other.value)),bytes_(std::exchange(other.bytes_,0)){}
            lease& operator=(lease&& other) noexcept
            {if(this!=&other){reset();value=std::move(other.value);bytes_=std::exchange(other.bytes_,0);}return *this;}
            ~lease(){reset();}
            explicit operator bool() const noexcept{return bool(value);}
            /** @brief Reserve additional actual bytes before allocation, without another call slot. */
            error grow(std::uint64_t amount) noexcept
            {
                if(!value) { return error::denied; }if(value->retired.load()) { return error::cancelled; }
                if(!add(value->bytes,amount,value->maximum_bytes)) { return error::again; }
                bytes_+=amount;return error::success;
            }
            void reset() noexcept
            {if(value){value->bytes.fetch_sub(bytes_,std::memory_order_release);value->calls.fetch_sub(1,std::memory_order_release);value.reset();bytes_=0;}}
        };
        error configure(std::uint64_t bytes,std::uint64_t calls)
        {
            if(value) { return error::conflict; }if(bytes>INT64_MAX||calls>1024) { return error::invalid; }
            try{auto next=::uwvm2::utils::container::make_shared<account>();next->maximum_bytes=bytes;next->maximum_calls=calls;value=std::move(next);return error::success;}
            catch(std::bad_alloc const&){return error::no_memory;}
        }
        error reserve(std::uint64_t bytes,lease& out) noexcept
        {
            if(out) { return error::conflict; }if(!value) { return error::denied; }if(value->retired.load()) { return error::cancelled; }
            if(!add(value->calls,1,value->maximum_calls)) { return error::again; }
            if(!add(value->bytes,bytes,value->maximum_bytes)){value->calls.fetch_sub(1);return error::again;}
            out.value=value;out.bytes_=bytes;
            if(value->retired.load()){out.reset();return error::cancelled;}return error::success;
        }
        void retire() noexcept{if(value)value->retired.store(true,std::memory_order_release);}
        void inspect(std::uint64_t& bytes,std::uint64_t& calls) const noexcept
        {bytes=value?value->bytes.load():0;calls=value?value->calls.load():0;}
    };
}
