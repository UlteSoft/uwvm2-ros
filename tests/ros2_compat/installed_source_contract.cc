/** Actual selected-SDK metadata/source admission contracts. No Node or grant.
 * Generated typed/default/constant proofs execute against genuine Native types.
 * Mutated copies below are rejection models, not modified SDKs or ROS traffic. */
#include "native_source.hpp"
#include "native_source_types.h"
#include <array>
#include <cassert>
#include <string>
#include <vector>

namespace source=::uwvm2::ros_gateway::ros2_compat::installed_source;
namespace compat=::uwvm2::ros_gateway::ros2_compat;
namespace ipc=::uwvm2::imported::ros::ipc;

struct no_installed_proof {};
static_assert(!source::traits<no_installed_proof>::enabled,"source proof must be explicit");
template<class Native> struct changed_members
{
    inline static compat::message_members const* value{};
    static compat::message_members const* get(){return value;}
};
template<class Native> void check(std::string_view name)
{
    auto original=source::traits<Native>::schema();
    assert(source::traits<Native>::prove());
    auto identity=source::identity<Native>();assert(identity.size()==71);
    assert(source::qualify<Native>(original,name,identity));
    auto wrong=identity;wrong.back()=wrong.back()=='0'?'1':'0';
    assert(!source::qualify<Native>(original,name,wrong));
    assert(!source::qualify<Native>(original,"wrong/msg/Name",identity));
    assert(!source::qualify<Native>({},name,identity));
    std::vector<ipc::custom_native_record> records(original.records,original.records+original.record_count);
    auto root=records.begin();while(root!=records.end()&&root->type!=name)++root;
    assert(root!=records.end()&&root->field_count);
    std::vector<ipc::custom_native_field> fields(root->fields,root->fields+root->field_count);
    root->fields=fields.data();ipc::custom_native_schema changed{records.data(),records.size()};
    fields[0].has_declared_default=!fields[0].has_declared_default;
    assert(!source::qualify<Native>(changed,name,identity));
    fields[0].has_declared_default=!fields[0].has_declared_default;
    assert(source::same_schema(changed,original));
    fields[0].name="different";assert(!source::qualify<Native>(changed,name,identity));
    assert(!source::digest(""));assert(!source::digest(std::string(64,'g')));
    std::array<source::witness,1> duplicate{};
    assert(!source::records(original,duplicate.data(),duplicate.size()));
    // These mutations use an actual shallow copy of this SDK's metadata.
    // No pointer is changed in any installed typesupport library.
    auto native=compat::message_members_for<Native>();assert(native);
    auto copy=*native;changed_members<Native>::value=&copy;
    std::vector<source::field_layout> layout;
    std::vector<source::witness> witnesses;
    // Construct only the selected leaf view when it has no nested dependency;
    // full rooted closures above still exercise every genuine nested witness.
    if(original.record_count==1&&native->member_count_==original.records[0].field_count)
    {
        auto const& record=original.records[0];assert(native->member_count_==record.field_count);
        for(std::size_t i{};i<record.field_count;++i)
            layout.push_back({native->members_[i].offset_,1});
        witnesses.push_back({name,std::string_view{"0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"},
            &changed_members<Native>::get,[](){return true;},native->size_of_,layout.data(),record.field_count});
        // This smaller model proves only metadata rejection. Genuine typed
        // sizes/constructor/constant values are checked by traits::prove above.
        assert(source::records(original,witnesses.data(),witnesses.size()));
        // Some genuine legacy SDKs omit the sole empty-Message placeholder
        // from introspection. Zero is therefore a valid alternate view for
        // that exact one-byte record, rather than a universal negative model.
        copy.member_count_=native->member_count_+1;
        assert(!source::records(original,witnesses.data(),witnesses.size()));copy=*native;
        copy.size_of_=0;assert(!source::records(original,witnesses.data(),witnesses.size()));copy=*native;
        auto good=witnesses[0];witnesses[0].installed_idl_sha256="bad";
        assert(!source::records(original,witnesses.data(),witnesses.size()));witnesses[0]=good;
        witnesses.push_back(good);assert(!source::records(original,witnesses.data(),witnesses.size()));
    }
    changed_members<Native>::value=nullptr;
}
int main()
{
#define UWVM_CHECK_SOURCE(Native,Name) check<Native>(Name);
    UWVM_ROS_INSTALLED_SOURCE_MESSAGES(UWVM_CHECK_SOURCE)
#undef UWVM_CHECK_SOURCE
}
