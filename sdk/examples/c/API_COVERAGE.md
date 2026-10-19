# C and C++ API and executable example coverage

This is source coverage of the registered ABI and real application entry paths.
It is not a Runtime/native success assertion. The machine inventory contains
exactly81 version-specific rows (ROS1:38; ROS2:43) and preserves numeric denial13,
unsupported95 and pending11 boundaries. See [ADVANCED.md](ADVANCED.md) for cases,
complete CLI policy generation, lifetime rules and native prerequisites.

| Import | ROS1 | ROS2 | C typed/owner layer | C++ typed/owner layer | Runnable case |
|---|---|---|---|---|---|
| abi_version | yes | yes | roswasmc_session_check_abi | Session::check_abi | 09_session_ownership.c / 09_session_ownership.cpp; default |
| action_advertise | yes | yes | roswasmc_node_create_action_server_checked | Node::create_action_server | 07_service_action_servers.c / 05_service_action_servers.cpp; 1 |
| action_cancel_any_goal | yes | yes | roswasmc_action_cancel_all | ActionClient::cancel_all | 04_service_action_resources.c / 04_services_actions.cpp; 6 |
| action_cancel_own_goal | yes | yes | roswasmc_action_cancel | ActionClient::cancel | 04_service_action_resources.c / 04_services_actions.cpp; 5 |
| action_receive_feedback | yes | yes | roswasmc_action_receive | ActionClient::receive | 04_service_action_resources.c / 04_services_actions.cpp; 3 |
| action_receive_result | yes | yes | roswasmc_action_receive | ActionClient::receive | 04_service_action_resources.c / 04_services_actions.cpp; 4 |
| action_respond | yes | yes | roswasmc_action_reply | ActionServer::respond | 07_service_action_servers.c / 05_service_action_servers.cpp; 1 |
| action_send_goal | yes | yes | roswasmc_fibonacci_send_goal | ActionClient::send_goal | 04_service_action_resources.c / 04_services_actions.cpp; 2 |
| capability_close | yes | yes | roswasmc_session_close / roswasmc_cleanup_owner | close / close_checked / Session::close | 09_session_ownership.c / 09_session_ownership.cpp; default |
| capability_open | yes | yes | roswasmc_session_open / Node factories | Session::open / Node factories | 09_session_ownership.c / 09_session_ownership.cpp; default |
| capability_revoke | yes | yes | roswasmc_revoke | Capability::revoke | 09_session_ownership.c / 09_session_ownership.cpp; REVOKE=1 |
| event_poll | yes | yes | roswasmc_poll / roswasmc_spin_some | Subscription/ServiceServer/ActionServer::poll_once + Executor | 06_runtime_delivery.c / 06_runtime_delivery.cpp; default |
| event_watch | yes | yes | roswasmc_event_watch | EventStream constructor | 11_unified_events.c / .cpp; actual application |
| event_poll_v1 | yes | yes | roswasmc_event_poll | EventStream::poll | 11_unified_events.c / .cpp; actual application |
| event_ready | yes | yes | roswasmc_event_ready | EventStream::ready | 11_unified_events.c / .cpp; actual application |
| service_submit | yes | yes | roswasmc_service_submit | EventStream::submit | events_contract.c / .cpp; fake-backend contract fixture |
| graph_inspect_action | 95 | yes | roswasmc_graph_read | Graph::inspect | 05_queries_and_events.c / 06_queries_and_control.cpp; 13 |
| graph_inspect_node | yes | yes | roswasmc_graph_read | Graph::inspect | 05_queries_and_events.c / 06_queries_and_control.cpp; 11 |
| graph_inspect_service | yes | yes | roswasmc_graph_read | Graph::inspect | 05_queries_and_events.c / 06_queries_and_control.cpp; 12 |
| graph_inspect_topic | yes | yes | roswasmc_graph_read | Graph::inspect | 05_queries_and_events.c / 06_queries_and_control.cpp; 10 |
| graph_list_nodes | yes | yes | roswasmc_graph_read | Graph::inspect | 05_queries_and_events.c / 06_queries_and_control.cpp; 15 |
| graph_list_services | yes | yes | roswasmc_graph_read | Graph::inspect | 05_queries_and_events.c / 06_queries_and_control.cpp; 16 |
| graph_list_topics | yes | yes | roswasmc_graph_read | Graph::inspect | 05_queries_and_events.c / 06_queries_and_control.cpp; 14 |
| lifecycle_change_state | 95 | yes | roswasmc_lifecycle_change | Lifecycle::change_state | 05_queries_and_events.c / 06_queries_and_control.cpp; 25 |
| lifecycle_get_state | 95 | yes | roswasmc_lifecycle_read | Lifecycle::get_state | 05_queries_and_events.c / 06_queries_and_control.cpp; 24 |
| log_write | yes | yes | roswasmc_logger_write | Logger::write | 05_queries_and_events.c / 06_queries_and_control.cpp; 21 |
| parameter_declare | 95 | yes | roswasmc_parameter_write | Parameters::set<T> | 05_queries_and_events.c / 06_queries_and_control.cpp; 7 |
| parameter_delete | yes | 95 | roswasmc_request | Parameters::erase | 05_queries_and_events.c / 06_queries_and_control.cpp; 2 |
| parameter_describe | 95 | yes | roswasmc_parameter_metadata | Parameters::metadata | 05_queries_and_events.c / 06_queries_and_control.cpp; 6 |
| parameter_exists | yes | 95 | roswasmc_parameter_exists_value | Parameters::exists | 05_queries_and_events.c / 06_queries_and_control.cpp; 3 |
| parameter_get | yes | yes | roswasmc_parameter_read | Parameters::get<T> | 05_queries_and_events.c / 06_queries_and_control.cpp; 0 |
| parameter_list | yes | yes | roswasmc_parameter_metadata | Parameters::metadata | 05_queries_and_events.c / 06_queries_and_control.cpp; 4 |
| parameter_search | yes | 95 | roswasmc_parameter_metadata | Parameters::metadata | 05_queries_and_events.c / 06_queries_and_control.cpp; 5 |
| parameter_set | yes | yes | roswasmc_parameter_write | Parameters::set<T> | 05_queries_and_events.c / 06_queries_and_control.cpp; 1 |
| parameter_subscribe_events | 95 | yes | roswasmc_node_create_parameter_events + roswasmc_decode_typed_parameter_event | Node::create_parameter_events + Subscription<TypedParameterEvent> | 05_queries_and_events.c / 06_queries_and_control.cpp; 9 |
| parameter_undeclare | 95 | yes | roswasmc_request | Parameters::erase | 05_queries_and_events.c / 06_queries_and_control.cpp; 8 |
| service_advertise | yes | yes | roswasmc_node_create_service_checked | Node::create_service<Trigger/SetBool> | 07_service_action_servers.c / 05_service_action_servers.cpp; 0 |
| service_call | yes | yes | roswasmc_trigger_call / roswasmc_set_bool_call | ServiceClient<Trigger/SetBool>::call | 04_service_action_resources.c / 04_services_actions.cpp; 0 |
| service_respond | yes | yes | roswasmc_service_reply | ServiceServer::poll_once checked reply | 07_service_action_servers.c / 05_service_action_servers.cpp; 0 |
| tf_broadcast | yes | yes | roswasmc_tf_write | TF::broadcast | 05_queries_and_events.c / 06_queries_and_control.cpp; 23 |
| tf_lookup | yes | yes | roswasmc_tf_read | TF::lookup | 05_queries_and_events.c / 06_queries_and_control.cpp; 22 |
| time_read_ros | yes | yes | roswasmc_clock_read | Clock::now | 05_queries_and_events.c / 06_queries_and_control.cpp; 17 |
| time_read_steady | 95 | yes | roswasmc_clock_read | Clock::now | 05_queries_and_events.c / 06_queries_and_control.cpp; 20 |
| time_read_system | 95 | yes | roswasmc_clock_read | Clock::now | 05_queries_and_events.c / 06_queries_and_control.cpp; 19 |
| time_read_wall | yes | 95 | roswasmc_clock_read | Clock::now | 05_queries_and_events.c / 06_queries_and_control.cpp; 18 |
| topic_publish | yes | yes | roswasmc_publish_string/bool/twist | Publisher<Message>::publish | 06_runtime_delivery.c / 06_runtime_delivery.cpp; default |
| topic_subscribe | yes | yes | roswasmc_node_create_subscription_checked | Node::create_subscription<Message> | 06_runtime_delivery.c / 06_runtime_delivery.cpp; default |

Typed trusted message codecs include String, Bool, Twist, Image and Transform;
service codecs include Trigger/SetBool; Action codecs include owned goal IDs,
feedback/result/cancel and advertised server goals. Parameter codecs cover
all Runtime-supported scalar/array tags, typed events and deletion envelopes.
The golden-vector fixtures exercise canonical codec forms, not native delivery.

| Local concept | C | C++ | Executable entry |
|---|---|---|---|
| Context/local shutdown | roswasmc_local_context | Context | 08_context_future_timer |
| bounded Session/checked rollback | session.h / roswasmc_cleanup_report | Session / close_checked | 09_session_ownership |
| deferred local Future | executor.h / roswasmc_future | Future<T> / Executor::post / ServiceClient::async_call | 08_context_future_timer |
| CallbackGroup | roswasmc_callback_group | CallbackGroup | 08_context_future_timer |
| explicit-time Timer | roswasmc_timer_tick | Timer / Executor::spin_some_at | 08_context_future_timer |
| codecs/scalar+arrays+typed/deletion events | codec.h caller-buffer/view codecs | wire + ParameterCodec + MessageCodec | golden_vectors model: native Runtime coverage separately required |

Local scheduling has no native grant/thread/time dependency. Future readiness
describes completion of a caller-driven synchronous operation, not automatic
native asynchronous execution. Session factories use stable ownership storage
and transactional rollback; every checked close attempts each live owner once.
Scope limits and actual test reports remain separate from this source matrix.

## Additional resource/lifetime concepts

C `roswasmc_node_create_action_bundle` and C++ Node's `ActionClientAliases`
overload group four independently granted roles, with optional explicit fifth
cancel-any. Both `10_action_client_bundle` entries run0..4 actual selected flows.
A bounded1..1024 table tracks only returned IDs; feedback/result/cancel preserve
ownership/error boundaries and checked reverse close/revoke exposes all statuses.
C++ `async_send_goal` adds only local deferred scheduling of one bounded call.

Pollable C++ resources protect direct nested poll/close/revoke17, including calls
outside an Executor. Shared callable snapshots preserve mutable callback state
and safe reassignment; closed facades return9 and RAII resets after exceptions.
Action.respond remains usable from an active handler. These concept extensions
do not add ABI imports or claim native rclcpp compatibility/native success.

The three unified observation methods have a complete String/closed application
entry. The service-submit row honestly designates an owned fake-backend contract
fixture, whose status does not establish a real service call. The independently
owned Runtime seven-event integration fixture separately exercises real
service_submit and all seven event kinds through Wasm and native ROS. Current
new source is prepared for unified acceptance; no historical fixture or receipt
is relabeled as an81-function runtime pass.
