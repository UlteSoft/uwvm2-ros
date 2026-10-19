/** @file
 * @brief Checked C++ ownership for the optional service-client extension.
 *
 * This class owns only an independently selected availability alias and borrows
 * an already-open call owner. It never grants, infers or reopens either alias.
 * The borrowed Capability, Context and explicit extension table must outlive
 * the object. Moving or closing the borrowed owner while this facade lives is a
 * caller lifetime error; local close/revoke observations are still checked.
 * Actual Host leases remain the authority boundary across every native effect.
 */
#pragma once
#include "roswasmcpp.hpp"
#include "service_client.h"
namespace roswasmcpp {
class ServiceAvailability {
    roswasmc_service_client_backend const* api_{};
    Capability& source_;
    std::uint64_t source_handle_{};
    Capability available_;
    std::uint32_t error_{};
    std::uint32_t cleanup_status_{};
    bool cleanup_attempted_{};
    static std::uint32_t preflight(roswasmc_context const& context,
        roswasmc_service_client_backend const& api,Capability const& source) noexcept {
        if(!context.backend||api.version<1||api.version>2||api.version!=context.backend->version)return UWVM_ROS_INVALID;
        if(!source.is_open())return UWVM_ROS_BAD_HANDLE;
        if(source.cap_.context!=&context)return UWVM_ROS_INVALID;
        if(!api.abi_version||!api.features||!api.query)return UWVM_ROS_UNSUPPORTED;
        return api.abi_version()==1?0u:UWVM_ROS_UNSUPPORTED;
    }
public:
    /** @brief Validate the local table/source before opening the one explicit
     * query alias. A failed open remains empty and records the original errno.
     * No feature RPC/native client is created by this constructor.
     */
    ServiceAvailability(roswasmc_context const& context,roswasmc_service_client_backend const& api,
        Capability& source,std::string_view alias):api_{&api},source_{source},source_handle_{source.handle()},
        available_{Capability::checked(context,alias,preflight(context,api,source))},error_{available_.error()} {}
    ServiceAvailability(ServiceAvailability const&)=delete;
    ServiceAvailability& operator=(ServiceAvailability const&)=delete;
    ServiceAvailability(ServiceAvailability&&)=delete;
    ServiceAvailability& operator=(ServiceAvailability&&)=delete;
    ~ServiceAvailability(){(void)close();}
    /** @brief Inspect constructor status without an import or native query. */
    std::uint32_t error() const noexcept{return error_;}
    /** @brief Inspect only the separately owned local query capability. */
    bool is_open() const noexcept{return available_.is_open();}
    /** @brief Read actual validated feature bits; errors preserve the output. */
    std::uint32_t features(std::uint64_t& output) const {
        if(error_)return error_;
        if(source_.handle()!=source_handle_)return UWVM_ROS_BAD_HANDLE;
        return roswasmc_service_client_features(api_,&available_.cap_,&source_.cap_,&output);
    }
    /** @brief One finite observation; only success changes output.
     * This method has no hidden spin, request, clock acquisition or retry.
     */
    std::uint32_t query(std::uint64_t wait_ns,bool& output) const {
        if(error_)return error_;int next{};
        if(source_.handle()!=source_handle_)return UWVM_ROS_BAD_HANDLE;
        auto code=roswasmc_service_client_query(api_,&available_.cap_,&source_.cap_,wait_ns,&next);
        if(!code)output=next!=0;return code;
    }
    /** @brief Retire only the query owner with one checked close attempt.
     * The borrowed call owner stays with its original owner. Failure can follow
     * actual Host retirement and must not be retried as an assumed live handle.
     */
    std::uint32_t close(){
        if(!available_.is_open())return 0u;
        cleanup_attempted_=true;cleanup_status_=available_.close();return cleanup_status_;
    }
    /** @brief Revoke only the separately granted query alias. */
    std::uint32_t revoke(){
        if(!available_.is_open())return 0u;
        cleanup_attempted_=true;cleanup_status_=available_.revoke();return cleanup_status_;
    }
    /** Last real retirement result; repeated local close never replays it. */
    std::uint32_t cleanup_status()const noexcept{return cleanup_status_;}
    bool cleanup_attempted()const noexcept{return cleanup_attempted_;}
};
}
