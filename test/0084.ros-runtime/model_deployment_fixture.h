/** @file
 * @brief Complete inert worker settings for policy-only injected-owner fixtures.
 * @details These paths intentionally do not exist. Tests using this helper
 * supply a trusted native-owner fixture and do not claim sandbox qualification.
 */
#pragma once
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
inline void configure_inert_model_worker(uwvm2::imported::ros::scene::deployment& deployment,std::string_view environment,std::string_view alias)
{
    namespace s=uwvm2::imported::ros::scene;
    s::model_sandbox::limits limits{4096,64u*1024u*1024u,1,4096,64,4096,0,4096,4096,0,1000000000,1000000,512u*1024u};
    for(auto field:s::model_worker::limit_fields)
        assert(deployment.model_worker_limit(environment,alias,field.name,limits.*field.member)==s::error::success);
    assert(deployment.model_worker_launcher(environment,alias,"/not-opened-by-policy-check/launcher",64,{})==s::error::success);
    assert(deployment.model_worker_image(environment,alias,s::model_sandbox::image_role::bootstrap,"/worker/bootstrap","/not-opened-by-policy-check/bootstrap",64,{})==s::error::success);
    assert(deployment.model_worker_image(environment,alias,s::model_sandbox::image_role::runtime,"/worker/backend.so","/not-opened-by-policy-check/backend",64,{})==s::error::success);
}
