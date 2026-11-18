/** @file Emit the native stopped scene observation codec's bounded first pages. */
#include <cassert>
#include <iostream>
#include <uwvm2/uwvm/debugger/scene_state.h>

namespace scene=uwvm2::uwvm::debugger::scene_state;
namespace data=scene::data;

int main()
{
    uwvm2::utils::container::std_vector<std::byte> blob;
    assert(!scene::capture_observation(3,blob));
    data::snapshot_hook.store(+[](data::request const& query,data::view& page) noexcept
    {
        assert(query.stop==3 && query.count==8 && query.first==0);
        page.result=data::status::forbidden;
    });
    assert(!scene::capture_observation(3,blob) && blob.empty());
    data::snapshot_hook.store(+[](data::request const& query,data::view& page) noexcept
    {
        page.result=data::status::ok;
        if(query.selected==data::selection::audit)
        {
            page.audit.configured=true;page.audit.admitted=UINT64_MAX;
            page.size=1;page.next=1;page.total=1;
        }
        else if(query.selected==data::selection::resources)page.result=data::status::busy;
        else
        {
            data::row grant;
            grant.alias.assign("signal");grant.environment.assign("robot");grant.type.assign("guard");
            grant.resource.assign("signal");grant.binding.assign("local");
            for(unsigned i{};i<9;++i)data::add(query,page,grant);
        }
    });
    assert(!scene::capture_observation(0,blob) && blob.empty());
    assert(scene::capture_observation(3,blob));
    std::cout.write(reinterpret_cast<char const*>(blob.data()),blob.size());
}
