/** @file
 * @brief Test inert model configuration and transactional native-owner preparation.
 * @details The owner fixture deliberately performs no inference or process
 * launch. Actual sandbox execution has separate qualification. These tests
 * verify that a failed native attachment cannot publish a partial deployment,
 * and that native lifetimes end before their referenced dispatcher is freed.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include "model_deployment_fixture.h"
#include <cassert>
#include <iostream>
namespace s=uwvm2::imported::ros::scene;
using s::error;
static s::models::plan plan()
{
    s::models::plan value;
    assert(value.model.identity.assign("vision.model")&&value.task.identity.assign("vision.task"));
    value.model.sha256.fill(17);value.task.sha256.fill(29);
    assert(value.parameter_type.assign("vision.parameters")&&value.output_type.assign("vision.predictions"));
    assert(s::models::select_codec("text.utf8",value));return value;
}
static s::grant grant(std::string_view alias)
{
    s::grant value;value.alias=alias;value.resource="approved.model";value.binding="local-model";
    value.kind=s::purpose::vision_model;value.actions={"model.open","model.describe","model.submit","request.describe","request.release","resource.close"};
    value.max_handles=1;value.max_resources=1;return value;
}
static std::array<std::uint64_t,9> limits{1,2,8,2,16,32,2,4096,1000000000};
static s::models::budget budget{2,4,16,64,4,8192};
static void configure(s::deployment& deployment,std::string_view alias,bool worker=true)
{
    assert(deployment.allow("robot",grant(alias))==error::success);
    assert(deployment.model_plan("robot",uwvm2::utils::container::std_string(alias),plan())==error::success);
    assert(deployment.model_limits("robot",uwvm2::utils::container::std_string(alias),limits)==error::success);
    if(worker)configure_inert_model_worker(deployment,"robot",alias);
}
static void environment(s::deployment& deployment)
{
    assert(deployment.create("robot")==error::success&&deployment.bind("robot","main")==error::success);
    assert(deployment.expose("robot","capability.open",s::exposure::allow)==error::success);
    for(auto const& operation:uwvm2::imported::ros::model_contract::operations)
        assert(deployment.expose("robot",uwvm2::utils::container::std_string(operation.name),s::exposure::allow)==error::success);
}
struct counts{unsigned prepared{},stop{},retired{},destroyed{};};
struct owner final:s::models::provider_lifetime
{
    s::model_dispatch& dispatcher;uwvm2::utils::container::std_string alias;counts& observed;std::uint64_t generation{};bool stopped{};
    owner(s::model_dispatch& state,std::string_view name,counts& values,std::uint64_t epoch):dispatcher(state),alias(name),observed(values),generation(epoch){++observed.prepared;}
    void request_stop() noexcept override{++observed.stop;}
    void retire() noexcept override
    {
        if(stopped)return;
        stopped=true;++observed.retired;
        s::models::usage usage;assert(dispatcher.try_inspect(alias,usage)==error::success);
        auto status=dispatcher.detach(alias,generation);
        assert(status==error::success||status==error::denied); // A revoked scope is already disconnected.
    }
    ~owner() override{retire();++observed.destroyed;}
};
static auto no_input=[](auto const&,auto const&,auto,auto&){assert(false);return error::denied;};
static void inert_and_finite()
{
    s::deployment missing;environment(missing);assert(missing.allow("robot",grant("brain"))==error::success);
    assert(missing.validate()==error::invalid);
    s::deployment no_capacity;environment(no_capacity);assert(no_capacity.allow("robot",grant("brain"))==error::success);
    assert(no_capacity.model_plan("robot","brain",plan())==error::success&&no_capacity.model_budget("robot",budget)==error::success);
    assert(no_capacity.validate()==error::invalid);
    s::deployment no_budget;environment(no_budget);configure(no_budget,"brain");assert(no_budget.validate()==error::invalid);
    s::deployment no_worker;environment(no_worker);configure(no_worker,"brain",false);
    assert(no_worker.model_budget("robot",budget)==error::success&&no_worker.validate()==error::invalid);
    assert(no_worker.model_worker_limit("robot","missing","image-bytes",4096)==error::denied);
    s::deployment short_budget;environment(short_budget);configure(short_budget,"brain");
    assert(short_budget.model_budget("robot",{})==error::success&&short_budget.validate()==error::invalid);
    s::deployment configured;environment(configured);configure(configured,"brain");
    assert(configured.model_budget("robot",budget)==error::success&&configured.validate()==error::success);
    int instance{};std::shared_ptr<s::broker> selected;
    assert(configured.select("main",{&instance,1},selected)==error::denied&&!selected);
    assert(configured.prepare_resources(no_input)==error::unsupported);
    assert(configured.select("main",{&instance,1},selected)==error::denied&&!selected);
}
static void exact_configuration()
{
    s::deployment value;environment(value);
    assert(value.model_plan("robot","brain",plan())==error::denied);
    auto wrong=grant("brain");wrong.kind=s::purpose::topic;assert(value.allow("robot",wrong)==error::invalid);
    assert(value.allow("robot",grant("brain"))==error::success);
    auto bad=plan();bad.encoding.size=0;assert(value.model_plan("robot","brain",bad)==error::invalid);
    assert(value.model_plan("robot","brain",plan())==error::success&&value.model_plan("robot","brain",plan())==error::conflict);
    auto over=limits;over[3]=65;assert(value.model_limits("robot","brain",over)==error::invalid);
    over=limits;over[8]=UINT64_MAX;assert(value.model_limits("robot","brain",over)==error::invalid);
    assert(value.model_limits("robot","brain",limits)==error::success&&value.model_limits("robot","brain",limits)==error::conflict);
    auto invalid_budget=budget;invalid_budget.requests=UINT64_MAX;assert(value.model_budget("robot",invalid_budget)==error::invalid);
    assert(value.model_budget("robot",budget)==error::success&&value.model_budget("robot",budget)==error::invalid);
    configure_inert_model_worker(value,"robot","brain");
    assert(value.model_worker_limit("robot","brain","stderr-bytes",1)==error::conflict);
    assert(value.model_worker_limit("robot","brain","unknown",1)==error::invalid);
    assert(value.validate()==error::success&&value.model_limits("robot","brain",limits)==error::invalid);
}
static void whole_deployment_and_cleanup()
{
    counts events;
    {
        s::deployment value;environment(value);configure(value,"first");configure(value,"second");assert(value.model_budget("robot",budget)==error::success);
        auto partial=[&](auto,auto alias,auto const&,auto const& manifest,std::uint64_t generation,s::model_dispatch& model,std::shared_ptr<s::models::provider_lifetime>& lifetime)
        {
            assert(manifest.frozen());
            if(alias=="second")return error::denied;
            lifetime=uwvm2::utils::container::make_shared<owner>(model,alias,events,generation);return model.attach(alias,generation);
        };
        assert(value.prepare_resources(no_input,partial)==error::denied);
        assert(events.prepared==1&&events.stop==1&&events.retired==1&&events.destroyed==1);
        int instance{};std::shared_ptr<s::broker> state;assert(value.select("main",{&instance,1},state)==error::denied&&!state);
        auto complete=[&](auto,auto alias,auto const&,auto const& manifest,std::uint64_t generation,s::model_dispatch& model,std::shared_ptr<s::models::provider_lifetime>& lifetime)
        {assert(manifest.frozen());lifetime=uwvm2::utils::container::make_shared<owner>(model,alias,events,generation);return model.attach(alias,generation);};
        assert(value.prepare_resources(no_input,complete)==error::success&&events.prepared==3);
        assert(value.select("main",{&instance,1},state)==error::success&&state);
        auto retired=events.retired;assert(state->revoke("first")==error::success&&events.retired==retired);
        // Revocation may signal but never join while holding authority.
        state.reset();
    }
    assert(events.retired==3&&events.destroyed==3);
}
static void codecs_and_atomic_budget()
{
    auto value=plan();std::array<std::uint8_t,3> text{0xe4,0xb8,0xad};std::array<std::uint8_t,2> malformed{0xc0,0x80};
    std::array<std::uint8_t,1> nul{};
    assert(value.validate_parameters(text)&&value.validate_output(text,true)&&!value.validate_parameters(malformed)&&!value.validate_output(nul,false));
    assert(!s::models::select_codec("plugin/path",value)&&s::models::select_codec("bytes",value)&&value.validate_parameters(malformed));
    value.sessions=1;value.requests=2;value.input_bytes=8;value.output_bytes=32;value.output_items=2;value.workspace_bytes=4096;
    s::models::budget total;assert(s::models::accumulate(budget,value,total));auto before=total;
    assert(s::models::accumulate(budget,value,total));assert(!s::models::accumulate(budget,value,total));
    assert(total.sessions==2*before.sessions&&total.workspace_bytes==2*before.workspace_bytes);
    auto huge=budget;huge.workspace_bytes=UINT64_MAX;assert(!s::models::accumulate(huge,value,total));
}
int main(){inert_and_finite();exact_configuration();whole_deployment_and_cleanup();codecs_and_atomic_budget();std::cout<<"passed 4 model deployment security groups\n";}
