/* SPDX-License-Identifier: Apache-2.0
 * Compile with the explicit rclcpp_model_binding.hpp selection macro. This is
 * an authored pure host model over the actual source adapter, NOT native ROS.
 */
#include <rclcpp/rclcpp.hpp>
#include <cassert>
#include <limits>
#include <cmath>
using S=example_interfaces::srv::AddTwoInts;using Client=rclcpp::Client<S>;
namespace m=service_model;namespace q=service_client_model;
template<class F>static void refused(F&& operation,unsigned expected){bool seen{};try{operation();}catch(rclcpp::exceptions::RCLError const& e){seen=e.code==expected;}assert(seen);}
struct Fixture {
 rclcpp::Node::SharedPtr node;Client::SharedPtr client;
 explicit Fixture(bool enabled=true,bool create=true){q::reset(enabled);rclcpp::init(0,static_cast<char const*const*>(nullptr));node=rclcpp::Node::make_shared("minimal_client");if(create)client=node->create_client<S>("add_two_ints");}
 ~Fixture(){q::during_query=nullptr;m::close_error=0;if(client)client->close();client.reset();node.reset();rclcpp::shutdown();assert(m::empty());}
};
static void old_trailing_binding_stays_unsupported(){Fixture f(false);auto opens=m::opens;
 refused([&]{f.client->service_is_ready();},95);refused([&]{f.client->wait_for_service(std::chrono::seconds(1));},95);
 assert(m::opens==opens&&opens==2&&q::queries==0&&m::submits==0&&m::sync_calls==0);}
static void actual_query_false_then_true(){Fixture f;assert(!f.client->service_is_ready()&&q::queries==1&&q::last_wait==0);
 q::ready=true;assert(f.client->wait_for_service(std::chrono::seconds(1))&&q::last_wait==UINT64_C(1000000000));
 assert(m::submits==0&&m::sync_calls==0&&m::steps==0&&m::logs==0&&m::opens==3);}
static void high_handles_unchanged(){Fixture f(true,false);m::next_handle=UINT64_C(0xf123456780000001);f.client=f.node->create_client<S>("add_two_ints");
 assert(!f.client->service_is_ready()&&q::last_source==UINT64_C(0xf123456780000001)&&q::last_available==UINT64_C(0xf123456780000003));}
static void invalid_durations_before_query(){Fixture f;
 refused([&]{f.client->wait_for_service(std::chrono::nanoseconds(-1));},22);
 refused([&]{f.client->wait_for_service(std::chrono::seconds(61));},22);
 refused([&]{f.client->wait_for_service(std::chrono::duration<double>{std::numeric_limits<double>::infinity()});},22);
 refused([&]{f.client->wait_for_service(std::chrono::duration<double>{std::numeric_limits<double>::quiet_NaN()});},22);
 assert(q::queries==0&&m::submits==0);}
static void maximum_and_fractional_finite_wait(){Fixture f;
 assert(!f.client->wait_for_service(std::chrono::seconds(60))&&q::last_wait==UINT64_C(60000000000));
 assert(!f.client->wait_for_service(std::chrono::duration<long double,std::nano>{0.75L})&&q::last_wait==0&&q::queries==2);}
static void unsupported_infinite_is_not_clamped(){Fixture f;refused([&]{f.client->wait_for_service();},95);assert(q::queries==0);}
static void query_errors_are_not_false(){Fixture f;for(auto code:{11u,13u,95u,110u,125u}){q::query_error=code;refused([&]{f.client->service_is_ready();},code);}
 assert(q::queries==5&&m::submits==0&&m::sync_calls==0);}
static void noncanonical_native_reply_is_error(){Fixture f;q::raw_mode=true;q::raw_answer=2;refused([&]{f.client->service_is_ready();},22);assert(q::queries==1);}
static void denied_optional_open_rolls_back_all_original_owners(){Fixture f(true,false);m::deny_alias="available";m::close_error=110;
 refused([&]{f.node->create_client<S>("add_two_ints");},13);
 assert(m::opens==3&&m::closes==2&&m::empty()&&q::queries==0&&roswasm_rclcpp_cleanup_error()==110);m::close_error=0;}
static void query_cleanup_is_independent_and_reverse(){Fixture f;m::close_error=110;assert(f.client->close()==110);
 auto report=f.client->cleanup_report();assert(report.attempted==3&&report.failed==3&&report.cleanup_error==110);
 assert(f.client->availability_cleanup_status()==110&&f.client->cleanup_statuses()[0]==110&&f.client->cleanup_statuses()[1]==110);
 auto closes=m::closes;assert(f.client->close()==0&&m::closes==closes);m::close_error=0;}
static Client::SharedPtr* active_client;static rclcpp::Node::SharedPtr* active_node;
static std::uint32_t reentrant(){
 assert((*active_client)->close()==17);refused([&]{rclcpp::shutdown();},17);
 refused([&]{(*active_node)->create_client<S>("add_two_ints");},17);
 refused([&]{(*active_client)->service_is_ready();},17);return 0;
}
static void real_group_lease_covers_query_callback(){Fixture f;active_client=&f.client;active_node=&f.node;q::during_query=reentrant;
 assert(!f.client->service_is_ready()&&q::queries==1&&m::closes==0&&m::opens==3);
 assert(!rclcpp::detail::state().service_dispatching);q::during_query=nullptr;}
static void metadata_query_does_not_consume_or_submit(){Fixture f;auto request=std::make_shared<S::Request>();request->a=41;request->b=1;auto future=f.client->async_send_request(request);
 m::queue(3,future.request_id);auto original=m::event;q::ready=true;
 assert(f.client->service_is_ready()&&m::event==original&&m::submits==1&&m::sync_calls==0&&!future._ready());
 assert(f.client->poll(0)==0&&future.get()->sum==42);}
static void closed_source_event_blocks_new_query(){Fixture f;m::queue(6,0,0,9,1);assert(f.client->poll(0)==0);
 refused([&]{f.client->service_is_ready();},9);assert(q::queries==0);}
static std::uint32_t release_last(){active_client->reset();assert(m::closes==0);return 0;}
static void group_retains_current_facade_until_query_returns(){Fixture f;active_client=&f.client;q::during_query=release_last;
 // The real group lease retains the object despite releasing the last public
 // shared_ptr inside the transport. No ownership destructor runs mid-query.
 assert(!f.client->service_is_ready());assert(!f.client&&m::closes==3&&!rclcpp::detail::state().service_dispatching);q::during_query=nullptr;}
int main(){old_trailing_binding_stays_unsupported();actual_query_false_then_true();high_handles_unchanged();invalid_durations_before_query();
 maximum_and_fractional_finite_wait();unsupported_infinite_is_not_clamped();query_errors_are_not_false();noncanonical_native_reply_is_error();
 denied_optional_open_rolls_back_all_original_owners();query_cleanup_is_independent_and_reverse();real_group_lease_covers_query_callback();metadata_query_does_not_consume_or_submit();
 closed_source_event_blocks_new_query();group_retains_current_facade_until_query_returns();return 0;}
