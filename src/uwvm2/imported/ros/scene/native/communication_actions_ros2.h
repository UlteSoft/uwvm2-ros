/** @file
 * @brief Real ROS2 action protocol over its three services and two topics.
 * @details Server send_goal/cancel responses are deferred until the owned scene
 * action decision. High-level auto-accept callbacks are deliberately not used.
 * Clients request results immediately after acceptance, independently of guest
 * polling. Goal/result/cancel correlations have separate finite reservations;
 * feedback cannot consume their storage. Every protocol endpoint name is derived
 * from the immutable action name, never a guest-supplied generic service/socket.
 * ROS message/container types below are the external ROS ABI; all VM-owned rows,
 * scratch, strings and shared control blocks use the strict fast_io adapters.
 */
#pragma once
#include "external_process_only.h"
#include "communication_ros2.h"
#include "../communication_actions.h"
#include "communication_action_codecs.h"
#include <action_msgs/msg/goal_status_array.hpp>
#include <action_msgs/srv/cancel_goal.hpp>

namespace uwvm2::imported::ros::scene::communication::ros2
{
    namespace action = ::uwvm2::imported::ros::scene::communication::actions;

    template <class Action, class GoalCodec, class FeedbackCodec, class ResultCodec>
    class action_protocol final : public action::native, public std::enable_shared_from_this<action_protocol<Action, GoalCodec, FeedbackCodec, ResultCodec>>
    {
        using self = action_protocol;
        using GoalService = typename Action::Impl::SendGoalService;
        using ResultService = typename Action::Impl::GetResultService;
        using Feedback = typename Action::Impl::FeedbackMessage;
        using CancelService = action_msgs::srv::CancelGoal;
        using Status = action_msgs::msg::GoalStatusArray;

        struct cancellation
        {
            handle token{};
            rmw_request_id_t header{};
            std::int64_t native_request{};
            bool pending{}, native_pending{};
        };

        struct slot
        {
            handle goal{}, token{};
            action::uuid uuid{};
            action::state state{};
            builtin_interfaces::msg::Time stamp;
            rmw_request_id_t goal_header{}, result_header{};
            std::int64_t goal_native_request{}, result_native_request{};
            bool allocated{}, goal_pending{}, result_pending{}, goal_native_pending{}, result_native_pending{}, uncertain{};
            std::uint64_t retention_until{};
            typename Action::Result result;
            ::uwvm2::utils::container::std_vector<cancellation> cancels;
        };

        node_lease attachment;
        action::plan configuration;
        std::weak_ptr<action::endpoint> receiver;
        std::mutex mutex, callback_mutex;
        std::atomic_bool active{true};
        ::uwvm2::utils::container::std_vector<slot> rows;
        ::uwvm2::utils::container::std_vector<std::uint8_t> scratch;
        handle next_token{};
        std::shared_ptr<rclcpp::Client<GoalService>> goal_client;
        std::shared_ptr<rclcpp::Client<ResultService>> result_client;
        std::shared_ptr<rclcpp::Client<CancelService>> cancel_client;
        std::shared_ptr<rclcpp::Service<GoalService>> goal_server;
        std::shared_ptr<rclcpp::Service<ResultService>> result_server;
        std::shared_ptr<rclcpp::Service<CancelService>> cancel_server;
        std::shared_ptr<rclcpp::Publisher<Feedback>> feedback_publisher;
        std::shared_ptr<rclcpp::Subscription<Feedback>> feedback_subscription;
        std::shared_ptr<rclcpp::Publisher<Status>> status_publisher;
        std::shared_ptr<rclcpp::Subscription<Status>> status_subscription;
        Status status_message;
#if defined(__linux__)
        ::pid_t process{::getpid()};
#endif
        bool same_process() const noexcept
        {
#if defined(__linux__)
            return process == ::getpid();
#else
            return true;
#endif
        }

        node* selected_node() const noexcept { return dynamic_cast<node*>(attachment.get()); }

        slot* find(action::uuid const& uuid) noexcept
        {
            for(auto& row: rows)
            {
                if(row.allocated && row.uuid == uuid) { return &row; }
            }
            return nullptr;
        }

        slot* find(handle goal) noexcept
        {
            for(auto& row: rows)
            {
                if(row.allocated && row.goal == goal && goal) { return &row; }
            }
            return nullptr;
        }

        slot* reserve() noexcept
        {
            auto time = now();
            for(auto& row: rows)
            {
                bool pending{};
                for(auto const& cancel: row.cancels) { pending |= cancel.pending || cancel.native_pending; }
                if(!row.allocated || (action::terminal(row.state) && !row.uncertain && !row.goal_pending && !row.result_pending && !row.goal_native_pending &&
                                      !row.result_native_pending && !pending && time >= row.retention_until))
                {
                    row.goal = row.token = 0;
                    row.uuid = {};
                    row.state = action::state::pending;
                    row.stamp = builtin_interfaces::msg::Time{};
                    row.goal_header = {};
                    row.result_header = {};
                    row.goal_pending = row.result_pending = row.goal_native_pending = row.result_native_pending = row.uncertain = false;
                    row.retention_until = 0;
                    std::fill(row.cancels.begin(), row.cancels.end(), cancellation{});
                    row.allocated = true;
                    return &row;
                }
            }
            return nullptr;
        }

        handle token() noexcept { return next_token == UINT64_MAX ? 0 : ++next_token; }

        static bool nonzero(action::uuid const& uuid) noexcept
        {
            return std::any_of(uuid.begin(), uuid.end(), [](auto b) { return b; });
        }

        static bool system_stamp(builtin_interfaces::msg::Time& output) noexcept
        {
            auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            if(nanos < 0 || nanos / 1000000000 > INT32_MAX) { return false; }
            output.sec = static_cast<std::int32_t>(nanos / 1000000000);
            output.nanosec = static_cast<std::uint32_t>(nanos % 1000000000);
            return true;
        }

        void publish_status()
        {
            status_message.status_list.clear();
            auto time = now();
            for(auto const& row: rows)
            {
                if(!row.allocated || row.state == action::state::pending || row.state == action::state::rejected ||
                   (action::terminal(row.state) && time >= row.retention_until))
                {
                    continue;
                }
                action_msgs::msg::GoalStatus status;
                status.goal_info.goal_id.uuid = row.uuid;
                status.goal_info.stamp = row.stamp;
                status.status = static_cast<std::int8_t>(row.state);
                status_message.status_list.push_back(std::move(status));
            }
            status_publisher->publish(status_message);
        }

        void reject_goal(rmw_request_id_t header) noexcept
        {
            try
            {
                typename GoalService::Response response;
                response.accepted = false;
                goal_server->send_response(header, response);
            }
            catch(...)
            {
            }
        }

        void reject_cancel(rmw_request_id_t header, std::int8_t code) noexcept
        {
            try
            {
                typename CancelService::Response response;
                response.return_code = code;
                cancel_server->send_response(header, response);
            }
            catch(...)
            {
            }
        }

        void fault(handle goal) noexcept
        {
            {
                std::lock_guard lock{mutex};
                if(auto row = find(goal)) { row->uncertain = true; }
            }
            if(auto core = receiver.lock()) { core->mark_unknown(goal); }
        }

        void receive_goal(std::shared_ptr<rmw_request_id_t> header, std::shared_ptr<typename GoalService::Request> request) noexcept
        {
            if(!header || !request || !live()) { return; }
            std::lock_guard callback_lock{callback_mutex};
            std::size_t size{};
            handle assigned{};
            {
                std::lock_guard lock{mutex};
                if(!live()) { return; }
                if(!nonzero(request->goal_id.uuid) || find(request->goal_id.uuid) || !GoalCodec::encode(request->goal, scratch, size))
                {
                    reject_goal(*header);
                    return;
                }
                auto row = reserve();
                assigned = token();
                if(!row || !assigned)
                {
                    if(row) { row->allocated = false; }
                    reject_goal(*header);
                    return;
                }
                row->uuid = request->goal_id.uuid;
                row->token = assigned;
                row->goal_header = *header;
                row->goal_pending = true;
            }
            auto core = receiver.lock();
            handle goal{};
            auto status = core ? core->offer(request->goal_id.uuid, assigned, {scratch.data(), size}, goal) : error::cancelled;
            std::lock_guard lock{mutex};
            auto row = find(request->goal_id.uuid);
            if(!row || row->token != assigned) { return; }
            if(status == error::ok) { row->goal = goal; }
            else
            {
                reject_goal(*header);
                row->allocated = false;
            }
        }

        void receive_result_request(std::shared_ptr<rmw_request_id_t> header, std::shared_ptr<typename ResultService::Request> request) noexcept
        {
            if(!header || !request || !live()) { return; }
            std::lock_guard lock{mutex};
            if(!live()) { return; }
            auto row = find(request->goal_id.uuid);
            try
            {
                if(!row || row->state == action::state::rejected || (action::terminal(row->state) && now() >= row->retention_until) || row->result_pending)
                {
                    typename ResultService::Response response;
                    response.status = 0;
                    result_server->send_response(*header, response);
                    return;
                }
                if(action::terminal(row->state))
                {
                    typename ResultService::Response response;
                    response.status = static_cast<std::int8_t>(row->state);
                    response.result = row->result;
                    result_server->send_response(*header, response);
                }
                else
                {
                    row->result_header = *header;
                    row->result_pending = true;
                }
            }
            catch(...)
            {
                if(row) { row->uncertain = true; }
            }
        }

        void receive_cancel(std::shared_ptr<rmw_request_id_t> header, std::shared_ptr<typename CancelService::Request> request) noexcept
        {
            if(!header || !request || !live()) { return; }
            handle assigned{};
            {
                std::lock_guard lock{mutex};
                if(!live()) { return; }
                auto row = find(request->goal_info.goal_id.uuid);
                // Zero UUID/time-based sets have broader scope than this exact-goal profile.
                if(!nonzero(request->goal_info.goal_id.uuid) || request->goal_info.stamp.sec || request->goal_info.stamp.nanosec)
                {
                    reject_cancel(*header, CancelService::Response::ERROR_REJECTED);
                    return;
                }
                if(!row)
                {
                    reject_cancel(*header, CancelService::Response::ERROR_UNKNOWN_GOAL_ID);
                    return;
                }
                if(action::terminal(row->state))
                {
                    reject_cancel(*header, CancelService::Response::ERROR_GOAL_TERMINATED);
                    return;
                }
                if(row->state != action::state::accepted && row->state != action::state::executing)
                {
                    reject_cancel(*header, CancelService::Response::ERROR_REJECTED);
                    return;
                }
                cancellation* selected{};
                for(auto& entry: row->cancels)
                {
                    if(!entry.token)
                    {
                        selected = &entry;
                        break;
                    }
                }
                assigned = token();
                if(!selected || !assigned)
                {
                    reject_cancel(*header, CancelService::Response::ERROR_REJECTED);
                    return;
                }
                selected->token = assigned;
                selected->header = *header;
                selected->pending = true;
            }
            auto core = receiver.lock();
            handle goal{};
            auto status = core ? core->offer_cancel(request->goal_info.goal_id.uuid, assigned, goal) : error::cancelled;
            if(status != error::ok)
            {
                std::lock_guard lock{mutex};
                if(auto row = find(request->goal_info.goal_id.uuid))
                {
                    for(auto& entry: row->cancels)
                    {
                        if(entry.token == assigned)
                        {
                            entry = {};
                            break;
                        }
                    }
                }
                reject_cancel(*header, CancelService::Response::ERROR_REJECTED);
            }
        }

        void result_response(handle goal, typename rclcpp::Client<ResultService>::SharedFuture future) noexcept
        {
            if(!live()) { return; }
            std::lock_guard callback_lock{callback_mutex};
            try
            {
                auto response = future.get();
                std::size_t size{};
                if(!response || response->status < 4 || response->status > 6 || !ResultCodec::encode(response->result, scratch, size))
                {
                    fault(goal);
                    return;
                }
                auto core = receiver.lock();
                auto status = core ? core->observe(goal, action::event_kind::result, static_cast<action::state>(response->status), {scratch.data(), size})
                                   : error::cancelled;
                if(core && status != error::ok) { core->mark_unknown(goal); }
                std::lock_guard lock{mutex};
                if(auto row = find(goal))
                {
                    row->result_native_pending = false;
                    if(status == error::ok)
                    {
                        row->state = static_cast<action::state>(response->status);
                        row->retention_until = now() + configuration.retention_ns;
                    }
                    else
                    {
                        row->uncertain = true;
                    }
                }
            }
            catch(...)
            {
                fault(goal);
            }
        }

        void goal_response(handle goal, typename rclcpp::Client<GoalService>::SharedFuture future) noexcept
        {
            if(!live()) { return; }
            try
            {
                auto response = future.get();
                if(!response)
                {
                    fault(goal);
                    return;
                }
                auto core = receiver.lock();
                auto state = response->accepted ? action::state::accepted : action::state::rejected;
                auto status = core ? core->observe(goal, action::event_kind::acceptance, state) : error::cancelled;
                if(core && status != error::ok) { core->mark_unknown(goal); }
                std::lock_guard lock{mutex};
                auto row = find(goal);
                if(!row) { return; }
                row->goal_native_pending = false;
                if(status != error::ok)
                {
                    row->uncertain = true;
                    return;
                }
                row->state = state;
                row->stamp = response->stamp;
                if(!response->accepted)
                {
                    row->retention_until = now() + configuration.retention_ns;
                    return;
                }
                auto request = ::uwvm2::utils::container::make_shared<typename ResultService::Request>();
                request->goal_id.uuid = row->uuid;
                auto weak = this->weak_from_this();
                row->result_native_pending = true;
                auto pending = result_client->async_send_request(request,
                                                                 [weak, goal](typename rclcpp::Client<ResultService>::SharedFuture result)
                                                                 {
                                                                     if(auto selected = weak.lock()) { selected->result_response(goal, std::move(result)); }
                                                                 });
                row->result_native_request = pending.request_id;
            }
            catch(...)
            {
                fault(goal);
            }
        }

        void cancel_response(handle goal, handle token, typename rclcpp::Client<CancelService>::SharedFuture future) noexcept
        {
            if(!live()) { return; }
            try
            {
                auto response = future.get();
                if(!response)
                {
                    fault(goal);
                    return;
                }
                action::state state{};
                action::uuid uuid{};
                {
                    std::lock_guard lock{mutex};
                    auto row = find(goal);
                    if(!row) { return; }
                    state = row->state;
                    uuid = row->uuid;
                    for(auto& entry: row->cancels)
                    {
                        if(entry.token == token)
                        {
                            entry.native_pending = false;
                            break;
                        }
                    }
                }
                if(response->return_code == CancelService::Response::ERROR_NONE)
                {
                    if(response->goals_canceling.size() != 1 || response->goals_canceling[0].goal_id.uuid != uuid)
                    {
                        fault(goal);
                        return;
                    }
                    state = action::state::cancelling;
                }
                else if(!response->goals_canceling.empty() || response->return_code < 1 || response->return_code > 3)
                {
                    fault(goal);
                    return;
                }
                std::array<std::uint8_t, 8> body{};
                wire::writer encoded{body};
                (void)encoded.u64(static_cast<std::uint64_t>(response->return_code));
                auto core = receiver.lock();
                auto status = core ? core->observe(goal, action::event_kind::cancel_response, state, body, token) : error::cancelled;
                if(core && status != error::ok) { core->mark_unknown(goal); }
                std::lock_guard lock{mutex};
                if(auto row = find(goal))
                {
                    if(status == error::ok) { row->state = state; }
                    else
                    {
                        row->uncertain = true;
                    }
                }
            }
            catch(...)
            {
                fault(goal);
            }
        }

        void feedback(typename Feedback::ConstSharedPtr message) noexcept
        {
            if(!message || !live()) { return; }
            std::lock_guard callback_lock{callback_mutex};
            handle goal{};
            action::state state{};
            {
                std::lock_guard lock{mutex};
                auto row = find(message->goal_id.uuid);
                if(!row) { return; }
                goal = row->goal;
                state = row->state;
            }
            std::size_t size{};
            if(!FeedbackCodec::encode(message->feedback, scratch, size)) { return; }
            if(auto core = receiver.lock()) { (void)core->observe(goal, action::event_kind::feedback, state, {scratch.data(), size}); }
        }

        void status(typename Status::ConstSharedPtr message) noexcept
        {
            if(!message || !live()) { return; }
            for(auto const& update: message->status_list)
            {
                if(update.status != 2 && update.status != 3) { continue; }
                handle goal{};
                {
                    std::lock_guard lock{mutex};
                    auto row = find(update.goal_info.goal_id.uuid);
                    if(!row) { continue; }
                    goal = row->goal;
                }
                if(auto core = receiver.lock())
                {
                    auto observed = static_cast<action::state>(update.status);
                    if(core->observe(goal, action::event_kind::status, observed) == error::ok)
                    {
                        std::lock_guard lock{mutex};
                        if(auto row = find(goal)) { row->state = observed; }
                    }
                }
            }
        }

    public:
        action_protocol(action::plan const& selected, node_lease parent) : attachment(std::move(parent)), configuration(selected)
        {
            rows.resize(selected.max_goals);
            for(auto& row: rows) { row.cancels.resize(selected.max_cancels_per_goal); }
            scratch.resize(std::max({selected.max_goal_bytes, selected.max_feedback_bytes, selected.max_result_bytes}));
            status_message.status_list.reserve(selected.max_goals);
        }

        bool live() const noexcept override { return same_process() && active.load(std::memory_order_acquire) && attachment.live() && selected_node(); }

        error start(std::shared_ptr<action::endpoint> const& core) noexcept
        {
            if(!live() || !core) { return error::denied; }
            try
            {
                receiver = core;
                auto selected = selected_node()->value;
                auto weak = this->weak_from_this();
                auto goal = configuration.resolved_name + "/_action/send_goal";
                auto result = configuration.resolved_name + "/_action/get_result";
                auto cancel = configuration.resolved_name + "/_action/cancel_goal";
                auto feedback_name = configuration.resolved_name + "/_action/feedback";
                auto status_name = configuration.resolved_name + "/_action/status";
                auto feedback_qos = native_qos(configuration.feedback_quality), status_qos = native_qos(configuration.status_quality);
                if(configuration.direction == action::role::client)
                {
                    goal_client = rclcpp::create_client<GoalService>(selected->base,
                                                                     selected->graph,
                                                                     selected->services,
                                                                     goal.c_str(),
                                                                     native_qos(configuration.goal_quality),
                                                                     nullptr);
                    result_client = rclcpp::create_client<ResultService>(selected->base,
                                                                         selected->graph,
                                                                         selected->services,
                                                                         result.c_str(),
                                                                         native_qos(configuration.result_quality),
                                                                         nullptr);
                    cancel_client = rclcpp::create_client<CancelService>(selected->base,
                                                                         selected->graph,
                                                                         selected->services,
                                                                         cancel.c_str(),
                                                                         native_qos(configuration.cancel_quality),
                                                                         nullptr);
                    feedback_subscription =
                        selected->template create_subscription<Feedback>(feedback_name.c_str(),
                                                                         rclcpp::QoS{rclcpp::QoSInitialization::from_rmw(feedback_qos), feedback_qos},
                                                                         [weak](typename Feedback::ConstSharedPtr message)
                                                                         {
                                                                             if(auto selected = weak.lock()) { selected->feedback(std::move(message)); }
                                                                         });
                    status_subscription =
                        selected->template create_subscription<Status>(status_name.c_str(),
                                                                       rclcpp::QoS{rclcpp::QoSInitialization::from_rmw(status_qos), status_qos},
                                                                       [weak](typename Status::ConstSharedPtr message)
                                                                       {
                                                                           if(auto selected = weak.lock()) { selected->status(std::move(message)); }
                                                                       });
                }
                else
                {
                    goal_server = rclcpp::create_service<GoalService>(
                        selected->base,
                        selected->services,
                        goal.c_str(),
                        [weak](std::shared_ptr<rmw_request_id_t> header, std::shared_ptr<typename GoalService::Request> request)
                        {
                            if(auto selected = weak.lock()) { selected->receive_goal(std::move(header), std::move(request)); }
                        },
                        native_qos(configuration.goal_quality),
                        nullptr);
                    result_server = rclcpp::create_service<ResultService>(
                        selected->base,
                        selected->services,
                        result.c_str(),
                        [weak](std::shared_ptr<rmw_request_id_t> header, std::shared_ptr<typename ResultService::Request> request)
                        {
                            if(auto selected = weak.lock()) { selected->receive_result_request(std::move(header), std::move(request)); }
                        },
                        native_qos(configuration.result_quality),
                        nullptr);
                    cancel_server = rclcpp::create_service<CancelService>(
                        selected->base,
                        selected->services,
                        cancel.c_str(),
                        [weak](std::shared_ptr<rmw_request_id_t> header, std::shared_ptr<typename CancelService::Request> request)
                        {
                            if(auto selected = weak.lock()) { selected->receive_cancel(std::move(header), std::move(request)); }
                        },
                        native_qos(configuration.cancel_quality),
                        nullptr);
                    feedback_publisher =
                        selected->template create_publisher<Feedback>(feedback_name.c_str(),
                                                                      rclcpp::QoS{rclcpp::QoSInitialization::from_rmw(feedback_qos), feedback_qos});
                    status_publisher = selected->template create_publisher<Status>(status_name.c_str(),
                                                                                   rclcpp::QoS{rclcpp::QoSInitialization::from_rmw(status_qos), status_qos});
                }
                auto check = [](rclcpp::QoS const& actual, qos const& expected)
                {
                    qos observed;
                    return observed_qos(actual.get_rmw_qos_profile(), observed) && equivalent(expected, observed);
                };
                bool valid = true;
                if(goal_client)
                {
                    valid = check(goal_client->get_request_publisher_actual_qos(), configuration.goal_quality) &&
                            check(goal_client->get_response_subscription_actual_qos(), configuration.goal_quality) &&
                            check(result_client->get_request_publisher_actual_qos(), configuration.result_quality) &&
                            check(result_client->get_response_subscription_actual_qos(), configuration.result_quality) &&
                            check(cancel_client->get_request_publisher_actual_qos(), configuration.cancel_quality) &&
                            check(cancel_client->get_response_subscription_actual_qos(), configuration.cancel_quality) &&
                            check(feedback_subscription->get_actual_qos(), configuration.feedback_quality) &&
                            check(status_subscription->get_actual_qos(), configuration.status_quality);
                }
                else
                {
                    valid = check(goal_server->get_request_subscription_actual_qos(), configuration.goal_quality) &&
                            check(goal_server->get_response_publisher_actual_qos(), configuration.goal_quality) &&
                            check(result_server->get_request_subscription_actual_qos(), configuration.result_quality) &&
                            check(result_server->get_response_publisher_actual_qos(), configuration.result_quality) &&
                            check(cancel_server->get_request_subscription_actual_qos(), configuration.cancel_quality) &&
                            check(cancel_server->get_response_publisher_actual_qos(), configuration.cancel_quality) &&
                            check(feedback_publisher->get_actual_qos(), configuration.feedback_quality) &&
                            check(status_publisher->get_actual_qos(), configuration.status_quality);
                }
                if(!valid)
                {
                    retire();
                    return error::denied;
                }
                return error::ok;
            }
            catch(std::bad_alloc const&)
            {
                retire();
                return error::no_memory;
            }
            catch(...)
            {
                retire();
                return error::io;
            }
        }

        native_result submit(handle goal, action::uuid const& uuid, std::span<std::uint8_t const> body, std::uint64_t deadline) noexcept override
        {
            if(!live() || !goal_client) { return {error::denied, effect::none}; }
            try
            {
                auto request = ::uwvm2::utils::container::make_shared<typename GoalService::Request>();
                request->goal_id.uuid = uuid;
                if(!GoalCodec::decode(body, request->goal)) { return {error::invalid, effect::none}; }
                std::lock_guard lock{mutex};
                if(!live() || deadline <= now()) { return {error::timeout, effect::none}; }
                if(find(uuid)) { return {error::stale, effect::none}; }
                auto row = reserve();
                if(!row) { return {error::again, effect::none}; }
                row->goal = goal;
                row->uuid = uuid;
                row->goal_native_pending = true;
                auto weak = this->weak_from_this();
                try
                {
                    auto pending = goal_client->async_send_request(request,
                                                                   [weak, goal](typename rclcpp::Client<GoalService>::SharedFuture value)
                                                                   {
                                                                       if(auto selected = weak.lock()) { selected->goal_response(goal, std::move(value)); }
                                                                   });
                    row->goal_native_request = pending.request_id;
                    return {deadline <= now() ? error::timeout : error::ok, effect::locally_accepted};
                }
                catch(...)
                {
                    row->uncertain = true;
                    return {error::io, effect::unknown};
                }
            }
            catch(std::bad_alloc const&)
            {
                return {error::no_memory, effect::none};
            }
            catch(...)
            {
                return {error::invalid, effect::none};
            }
        }

        native_result respond(action::response const& command, action::uuid const& uuid, action::state after, std::uint64_t deadline) noexcept override
        {
            if(!live() || !goal_server) { return {error::denied, effect::none}; }
            typename Action::Feedback feedback_value;
            typename Action::Result result_value;
            try
            {
                if(command.change == action::transition::feedback && !FeedbackCodec::decode(command.body, feedback_value))
                {
                    return {error::invalid, effect::none};
                }
                if(action::terminal(after) && after != action::state::rejected && !ResultCodec::decode(command.body, result_value))
                {
                    return {error::invalid, effect::none};
                }
            }
            catch(std::bad_alloc const&)
            {
                return {error::no_memory, effect::none};
            }
            catch(...)
            {
                return {error::invalid, effect::none};
            }
            std::lock_guard lock{mutex};
            auto row = find(uuid);
            if(!live() || !row || (row->goal && row->goal != command.goal)) { return {error::denied, effect::none}; }
            if(deadline <= now()) { return {error::timeout, effect::none}; }
            row->goal = command.goal;
            try
            {
                if(command.change == action::transition::accept || command.change == action::transition::reject)
                {
                    if(!row->goal_pending || row->token != command.request) { return {error::denied, effect::none}; }
                    typename GoalService::Response response;
                    response.accepted = command.change == action::transition::accept;
                    if(response.accepted && !system_stamp(response.stamp)) { return {error::invalid, effect::none}; }
                    row->stamp = response.stamp;
                    row->state = after;
                    row->goal_pending = false;
                    goal_server->send_response(row->goal_header, response);
                    if(after == action::state::rejected) { row->retention_until = now() + configuration.retention_ns; }
                    else
                    {
                        publish_status();
                    }
                }
                else if(command.change == action::transition::feedback)
                {
                    Feedback response;
                    response.goal_id.uuid = uuid;
                    response.feedback = std::move(feedback_value);
                    feedback_publisher->publish(response);
                }
                else if(command.change == action::transition::cancel_response)
                {
                    cancellation* entry{};
                    for(auto& candidate: row->cancels)
                    {
                        if(candidate.token == command.request && candidate.pending)
                        {
                            entry = &candidate;
                            break;
                        }
                    }
                    if(!entry) { return {error::denied, effect::none}; }
                    typename CancelService::Response response;
                    response.return_code = command.accept_cancel ? CancelService::Response::ERROR_NONE : CancelService::Response::ERROR_REJECTED;
                    if(command.accept_cancel)
                    {
                        action_msgs::msg::GoalInfo info;
                        info.goal_id.uuid = uuid;
                        info.stamp = row->stamp;
                        response.goals_canceling.push_back(info);
                    }
                    entry->pending = false;
                    row->state = after;
                    cancel_server->send_response(entry->header, response);
                    if(command.accept_cancel) { publish_status(); }
                }
                else
                {
                    row->state = after;
                    if(action::terminal(after))
                    {
                        row->result = std::move(result_value);
                        row->retention_until = now() + configuration.retention_ns;
                    }
                    publish_status();
                    if(action::terminal(after) && row->result_pending)
                    {
                        typename ResultService::Response response;
                        response.status = static_cast<std::int8_t>(after);
                        response.result = row->result;
                        row->result_pending = false;
                        result_server->send_response(row->result_header, response);
                    }
                }
                return {deadline <= now() ? error::timeout : error::ok, effect::locally_accepted};
            }
            catch(...)
            {
                row->uncertain = true;
                return {error::io, effect::unknown};
            }
        }

        native_result cancel(handle goal, handle token, action::uuid const& uuid, std::uint64_t deadline) noexcept override
        {
            if(!live() || !cancel_client) { return {error::denied, effect::none}; }
            try
            {
                auto request = ::uwvm2::utils::container::make_shared<typename CancelService::Request>();
                request->goal_info.goal_id.uuid = uuid;
                std::lock_guard lock{mutex};
                auto row = find(goal);
                if(!live() || !row || row->uuid != uuid) { return {error::denied, effect::none}; }
                if(deadline <= now()) { return {error::timeout, effect::none}; }
                cancellation* entry{};
                for(auto& candidate: row->cancels)
                {
                    if(!candidate.token)
                    {
                        entry = &candidate;
                        break;
                    }
                }
                if(!entry) { return {error::again, effect::none}; }
                entry->token = token;
                entry->native_pending = true;
                auto weak = this->weak_from_this();
                try
                {
                    auto pending = cancel_client->async_send_request(request,
                                                                     [weak, goal, token](typename rclcpp::Client<CancelService>::SharedFuture value)
                                                                     {
                                                                         if(auto selected = weak.lock())
                                                                         {
                                                                             selected->cancel_response(goal, token, std::move(value));
                                                                         }
                                                                     });
                    entry->native_request = pending.request_id;
                    return {deadline <= now() ? error::timeout : error::ok, effect::locally_accepted};
                }
                catch(...)
                {
                    row->uncertain = true;
                    return {error::io, effect::unknown};
                }
            }
            catch(std::bad_alloc const&)
            {
                return {error::no_memory, effect::none};
            }
            catch(...)
            {
                return {error::invalid, effect::none};
            }
        }

        void retire() noexcept override
        {
            if(!same_process() || !active.exchange(false, std::memory_order_acq_rel)) { return; }
            std::lock_guard lock{mutex};
            try
            {
                for(auto& row: rows)
                {
                    if(goal_client && row.goal_native_pending) { goal_client->remove_pending_request(row.goal_native_request); }
                    if(result_client && row.result_native_pending) { result_client->remove_pending_request(row.result_native_request); }
                    if(cancel_client)
                    {
                        for(auto const& entry: row.cancels)
                        {
                            if(entry.native_pending) { cancel_client->remove_pending_request(entry.native_request); }
                        }
                    }
                }
                if(auto selected = selected_node())
                {
                    auto base = selected->value->base;
                    auto group = base->get_default_callback_group();
                    rclcpp::node_interfaces::NodeWaitables waitables{base.get()};
                    auto remove = [&](auto const& endpoint)
                    {
                        if(endpoint)
                        {
                            for(auto const& [type, event]: endpoint->get_event_handlers())
                            {
                                static_cast<void>(type);
                                waitables.remove_waitable(event, group);
                            }
                        }
                    };
                    remove(feedback_publisher);
                    remove(feedback_subscription);
                    remove(status_publisher);
                    remove(status_subscription);
                    base->trigger_notify_guard_condition();
                    group->trigger_notify_guard_condition();
                }
                goal_client.reset();
                result_client.reset();
                cancel_client.reset();
                goal_server.reset();
                result_server.reset();
                cancel_server.reset();
                feedback_publisher.reset();
                feedback_subscription.reset();
                status_publisher.reset();
                status_subscription.reset();
            }
            catch(...)
            {
            }
        }

        ~action_protocol() override { retire(); }
    };

    /** @brief Open the complete LookupTransform action binding on a counted node.
     * @details Unknown schemas/validators are refused before native endpoints.
     * The caller must reserve the counted node attachment and scene action
     * authority. This first qualified binding does not stand for arbitrary IDL.
     */
    inline error open_lookup_action(action::plan const& plan, node_lease attachment, handle id, std::shared_ptr<action::endpoint>& output) noexcept
    {
        if(plan.action_type != "tf2_msgs/action/LookupTransform" || plan.goal.type_identity != lookup_goal_codec::identity ||
           plan.feedback.type_identity != lookup_feedback_codec::identity || plan.result.type_identity != lookup_result_codec::identity ||
           plan.goal.encoding != "scene-canonical" || plan.feedback.encoding != "scene-canonical" || plan.result.encoding != "scene-canonical" ||
           plan.goal.validate != lookup_goal_codec::validate || plan.feedback.validate != lookup_feedback_codec::validate ||
           plan.result.validate != lookup_result_codec::validate || !attachment.live() || attachment.authority().environment != plan.authority.environment)
        {
            return error::denied;
        }
        try
        {
            using adapter = action_protocol<tf2_msgs::action::LookupTransform, lookup_goal_codec, lookup_feedback_codec, lookup_result_codec>;
            auto native = ::uwvm2::utils::container::make_shared<adapter>(plan, std::move(attachment));
            auto core = ::uwvm2::utils::container::make_shared<action::endpoint>();
            auto status = core->configure(plan, id, native);
            if(status != error::ok) { return status; }
            // Publish trusted ownership before exposing native endpoints. On a
            // later startup failure, the caller must retain any cleanup debt.
            output = core;
            status = native->start(core);
            if(status != error::ok)
            {
                core->retire();
                return status;
            }
            output = std::move(core);
            return error::ok;
        }
        catch(std::bad_alloc const&)
        {
            return error::no_memory;
        }
        catch(...)
        {
            return error::io;
        }
    }
}  // namespace uwvm2::imported::ros::scene::communication::ros2
