/** @file
 * @brief Inert deployment registration for an already running model provider.
 * @details No executable, image, weights, placement or process resource knobs
 * enter the VM. The provisioner retains endpoint ownership until preparation
 * duplicates it and must not reuse or share it. Session registration does not
 * grant guest access. Freeze validates finite local memory and poll bounds;
 * the external model/task/codec handshake is exact and grants no authority.
 */
#pragma once
#include "model_attachment.h"
namespace uwvm2::imported::ros::scene::model_attachment
{
    class configuration
    {
        int fd_{-1};drivers::credentials peer_;protocol::binding binding_;model_attachment::limits bounds_;
        bool assigned_{},frozen_{};
    public:
        error channel(int fd,drivers::credentials peer,protocol::binding binding,limits bounds,bool declarations_only=false)noexcept
        {
            if(frozen_)return error::denied;
            if(assigned_)return error::conflict;
            if(fd<0||!peer.pid||peer.pid>INT32_MAX||peer.uid>UINT32_MAX||peer.gid>UINT32_MAX||
                !protocol::valid(binding)||!valid(bounds))return error::invalid;
            if(!declarations_only && bounds.tls_identity_fd>=0)
                if(auto checked=attachment_tls::check_identity(bounds.tls_identity_fd);checked!=error::success)return checked;
            fd_=fd;peer_=peer;binding_=binding;bounds_=bounds;assigned_=true;return error::success;
        }
        error freeze(models::plan const& selected)noexcept
        {
            if(frozen_)return error::conflict;
            if(!assigned_||!models::valid(selected)||!valid(bounds_))return error::invalid;
            frozen_=true;return error::success;
        }
        auto limits()const noexcept -> model_attachment::limits{return bounds_;}
        drivers::credentials peer()const noexcept{return peer_;}
        bool frozen()const noexcept{return frozen_;}
        int descriptor()const noexcept{return fd_;}
        protocol::binding const& identity()const noexcept{return binding_;}
        error prepare(model_dispatch& dispatcher,std::string_view alias,std::uint64_t generation,
                      std::shared_ptr<models::provider_lifetime>& output)const noexcept
        {
            if(!frozen_)return error::denied;
#if defined(__linux__)
            return service::prepare(dispatcher,alias,generation,fd_,peer_,binding_,bounds_,output);
#else
            (void)dispatcher;(void)alias;(void)generation;(void)output;return error::unsupported;
#endif
        }
    };
}
