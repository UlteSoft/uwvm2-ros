#include <uwvm2/utils/control/dump_permissions.h>
#include <uwvm2/runtime/diagnostic/snapshot_policy.h>
#include <cstdio>
#include <initializer_list>
#include <cstdlib>
namespace d=uwvm2::utils::control::dump_permissions;
namespace p=uwvm2::utils::control::debug_permissions;
namespace s=uwvm2::runtime::diagnostic::snapshot_policy;
static unsigned checks{};
#define REQUIRE(x) do{++checks;if(!(x)){std::fprintf(stderr,"FAIL line %u: %s\n",__LINE__,#x);std::exit(1);}}while(false)
int main()
{
 for(auto permission:{p::permission_level::limited,p::permission_level::full,p::permission_level::read_only,p::permission_level::none})
 for(auto kind:{d::constraint::maximum,d::constraint::forced})for(auto level:{d::disclosure::none,d::disclosure::diagnostic,d::disclosure::full})
 {
  d::selection choice{kind,level};d::launch_policy policy;
  REQUIRE(policy.selected()==d::selection{});
  auto const valid=d::valid(choice);REQUIRE(policy.configure_trusted_launch(choice)==valid);
  if(!valid)continue;
  REQUIRE(policy.selected()==choice);REQUIRE(policy.configure_trusted_launch(choice));
  for(auto next:{d::selection{},d::selection{d::constraint::forced,d::disclosure::none},d::selection{d::constraint::maximum,d::disclosure::diagnostic}})
   REQUIRE(policy.configure_trusted_launch(next)==(choice==next));
  for(auto request:{d::disclosure::none,d::disclosure::diagnostic,d::disclosure::full})
  {
   auto const r=policy.permit(permission,request);auto const ceiling=d::debug_ceiling(permission);
   // Independent truth table: distinguish a ceiling (can choose less) from a
   // forced selection (must choose exactly), including the implicit default.
   auto const debug_limit=permission==p::permission_level::none?0u:permission==p::permission_level::limited?1u:2u;
   auto const configured=static_cast<unsigned>(level), asked=static_cast<unsigned>(request);
   auto const limit=configured<debug_limit?configured:debug_limit;
   bool const expected=limit && (kind==d::constraint::forced ? configured<=debug_limit && (!asked || asked==configured) : !asked || asked<=limit);
   REQUIRE((r.status==d::denial::none)==expected);
   if(expected)REQUIRE(static_cast<unsigned>(r.level)==(asked?asked:limit));
   if(r.status==d::denial::none)
   {REQUIRE(r.level!=d::disclosure::none&&r.level<=ceiling&&r.level<=level);
    REQUIRE(kind!=d::constraint::forced||r.level==level);REQUIRE(request==d::disclosure::none||request==r.level);}
   else REQUIRE(r.level==d::disclosure::none);
  }
 }
 d::launch_policy sealed;sealed.seal_default_before_execution();REQUIRE(!sealed.configure_trusted_launch({d::constraint::forced,d::disclosure::none}));
 REQUIRE(d::resolve({d::constraint::maximum,d::disclosure::full},p::permission_level::limited).level==d::disclosure::diagnostic);
 REQUIRE(d::resolve({d::constraint::forced,d::disclosure::full},p::permission_level::limited).status==d::denial::debug_permission);
 REQUIRE(d::resolve({d::constraint::forced,d::disclosure::diagnostic},p::permission_level::full,d::disclosure::full).status==d::denial::forced_mismatch);
 for(auto permission:{s::permission::none,s::permission::limited,s::permission::read_only,s::permission::full})
 {
  s::policy policy{permission};
  for(auto action:{s::action::restore,s::action::replay})
  {
   REQUIRE(policy.authorize(action,s::level::diagnostic,true,true,true)!=s::refusal::none);
   REQUIRE(policy.authorize(action,s::level::replay,false,true,true)!=s::refusal::none);
   REQUIRE(policy.authorize(action,s::level::replay,true,true,false)!=s::refusal::none);
   REQUIRE((policy.authorize(action,s::level::replay,true,true,true)==s::refusal::none)==(permission==s::permission::full));
  }
 }
 std::printf("PASS dump launch/disclosure policy %u checks\n",checks);
}
