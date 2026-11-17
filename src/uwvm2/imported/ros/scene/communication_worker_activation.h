/** @file
 * @brief Current bilateral authority for one exact isolated communication pair.
 * @details The factory retains this callback context through retirement. Weak
 * dispatcher references avoid a factory/dispatcher ownership cycle; expired or
 * forked contexts deny activation. Both dispatchers must share the same actual
 * authority object and environment. Their recursive leases remain held across
 * the bounded activation step, so revocation cannot interleave between peers.
 * No remembered grant, matching profile label or numerical handle substitutes
 * for the exact accepted canonical node-startup draft.
 */
#pragma once
#include "communication_dispatch.h"
#include "communication_provider.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
    class activation_admission
    {
        std::array<std::weak_ptr<communication_dispatch>,2> dispatchers;
#if defined(__linux__)
        pid_t process{::getpid()};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process==::getpid();
#else
            return true;
#endif
        }
    public:
        explicit activation_admission(std::array<std::shared_ptr<communication_dispatch>,2> const& value)noexcept
            :dispatchers{value[0],value[1]}{}
        /** @brief Retain complete original authority for both actual pending nodes.
         * @details Failed admission releases every lease acquired in this call.
         * Output must initially be empty. No allocation, native work, rate debit
         * or implicit authority transfer occurs here.
         */
        static communication::error admit(void* context,std::array<communication::activation_origin,2> const& original,
                                          communication::activation_guard& output)noexcept
        {
            using error=communication::error;
            if(!context||output.participants[0]||output.participants[1])return error::denied;
            auto& self=*static_cast<activation_admission*>(context);if(!self.same_process())return error::denied;
            std::array<std::shared_ptr<communication_dispatch>,2> targets{self.dispatchers[0].lock(),self.dispatchers[1].lock()};
            if(!targets[0]||!targets[1]||!targets[0]->shares_authority(*targets[1]))return error::denied;
            if(!original[0].node||!original[1].node||original[0].node->authority==original[1].node->authority)return error::denied;
            communication::activation_guard held;
            for(std::size_t i{};i<2;++i)
            {
                if(!original[i].node||!original[i].context||!original[i].request)return error::denied;
                auto status=targets[i]->admit_node_activation(*original[i].node,*original[i].context,*original[i].request,held.participants[i]);
                if(status!=scene::error::success)return static_cast<error>(status);
            }
            output=std::move(held);return error::ok;
        }
    };
}
