/** Actual generated C Action calls with complete constructor rollback visibility.
 * This owner adds no serializer/backend/authority. Definition delegates directly
 * to the matching complete generated C typed functions. Context/table storage
 * and all potential owner slots exist before the first open. No heap is used.
 */
#pragma once
#include <array>
#include <cstdint>
#include <roswasm/action_bundle.h>
namespace uwvm_industrial {
template<class Definition> class ActionOwner final {
    roswasmc_context context_{};
    std::array<std::uint64_t,16> goals_{};
    roswasmc_action_bundle bundle_{};
public:
    using Goal=typename Definition::Goal;using Result=typename Definition::Result;using Feedback=typename Definition::Feedback;
    ActionOwner(roswasmc_context const& context,roswasmc_action_aliases const& aliases,std::size_t slots=1)
        :context_{context.backend}{
        if(!slots||slots>goals_.size()){bundle_.error=90;bundle_.rollback=roswasmc_cleanup_init(90);return;}
        roswasmc_action_bundle_init(&context_,&bundle_,&aliases,goals_.data(),slots);
    }
    ActionOwner(ActionOwner const&)=delete;ActionOwner& operator=(ActionOwner const&)=delete;
    ActionOwner(ActionOwner&&)=delete;
    ~ActionOwner(){if(bundle_.active){std::array<uint32_t,5> statuses{};(void)roswasmc_action_bundle_close_checked(&bundle_,0,statuses.data(),statuses.size());}}
    uint32_t error()const noexcept{return bundle_.error?bundle_.error:bundle_.active?0u:9u;}
    bool is_open()const noexcept{return bundle_.active;}
    roswasmc_cleanup_report constructor_report()const noexcept{return bundle_.rollback;}
    std::array<uint32_t,5> constructor_results()const noexcept{std::array<uint32_t,5> out{};for(size_t i=0;i<5;++i)out[i]=bundle_.rollback_status[i];return out;}
    roswasmc_result send_goal(Goal const& value,uint64_t& id){return Definition::send(&bundle_,&value,&id);}
    roswasmc_result feedback(uint64_t id,Feedback& out){return Definition::feedback(&bundle_,id,&out);}
    roswasmc_result result(uint64_t id,uint8_t& kind,Result& out){return Definition::result(&bundle_,id,&kind,&out);}
    roswasmc_result cancel_own(uint64_t id,int& accepted){return roswasmc_action_bundle_cancel_own(&bundle_,id,&accepted);}
    roswasmc_cleanup_report close_checked(uint32_t primary,std::array<uint32_t,5>& statuses){return roswasmc_action_bundle_close_checked(&bundle_,primary,statuses.data(),statuses.size());}
};
}
