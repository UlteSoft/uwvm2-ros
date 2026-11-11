// Exact registered host interface identities. A catalogue entry is not a grant.
// Licensed under the APL-2.0 License (see LICENSE file).
#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
namespace uwvm2::utils::control::host_interfaces
{
    enum class family : unsigned char { wasip1, ros1, ros2 };
    enum class access : unsigned char { hidden, deny, allow };
    struct entry { family group; ::std::string_view name_space, function, key; };
    inline constexpr ::std::array catalogue{
        entry{family::wasip1,"wasi_snapshot_preview1","args_get","args_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","args_sizes_get","args_sizes_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","clock_res_get","clock_res_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","clock_time_get","clock_time_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","environ_get","environ_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","environ_sizes_get","environ_sizes_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_advise","fd_advise"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_allocate","fd_allocate"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_close","fd_close"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_datasync","fd_datasync"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_fdstat_get","fd_fdstat_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_fdstat_set_flags","fd_fdstat_set_flags"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_fdstat_set_rights","fd_fdstat_set_rights"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_filestat_get","fd_filestat_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_filestat_set_size","fd_filestat_set_size"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_filestat_set_times","fd_filestat_set_times"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_pread","fd_pread"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_prestat_dir_name","fd_prestat_dir_name"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_prestat_get","fd_prestat_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_pwrite","fd_pwrite"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_read","fd_read"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_readdir","fd_readdir"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_renumber","fd_renumber"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_seek","fd_seek"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_sync","fd_sync"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_tell","fd_tell"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_write","fd_write"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_create_directory","path_create_directory"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_filestat_get","path_filestat_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_filestat_set_times","path_filestat_set_times"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_link","path_link"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_open","path_open"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_readlink","path_readlink"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_remove_directory","path_remove_directory"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_rename","path_rename"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_symlink","path_symlink"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_unlink_file","path_unlink_file"},
        entry{family::wasip1,"wasi_snapshot_preview1","poll_oneoff","poll_oneoff"},
        entry{family::wasip1,"wasi_snapshot_preview1","proc_raise","proc_raise"},
        entry{family::wasip1,"wasi_snapshot_preview1","random_get","random_get"},
        entry{family::wasip1,"wasi_snapshot_preview1","sched_yield","sched_yield"},
        entry{family::wasip1,"wasi_snapshot_preview1","sock_accept","sock_accept"},
        entry{family::wasip1,"wasi_snapshot_preview1","sock_recv","sock_recv"},
        entry{family::wasip1,"wasi_snapshot_preview1","sock_send","sock_send"},
        entry{family::wasip1,"wasi_snapshot_preview1","sock_shutdown","sock_shutdown"},
        entry{family::wasip1,"wasi_snapshot_preview1","args_get_wasm64","args_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","args_sizes_get_wasm64","args_sizes_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","clock_res_get_wasm64","clock_res_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","clock_time_get_wasm64","clock_time_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","environ_get_wasm64","environ_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","environ_sizes_get_wasm64","environ_sizes_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_advise_wasm64","fd_advise_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_allocate_wasm64","fd_allocate_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_close_wasm64","fd_close_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_datasync_wasm64","fd_datasync_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_fdstat_get_wasm64","fd_fdstat_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_fdstat_set_flags_wasm64","fd_fdstat_set_flags_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_fdstat_set_rights_wasm64","fd_fdstat_set_rights_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_filestat_get_wasm64","fd_filestat_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_filestat_set_size_wasm64","fd_filestat_set_size_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_filestat_set_times_wasm64","fd_filestat_set_times_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_pread_wasm64","fd_pread_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_prestat_dir_name_wasm64","fd_prestat_dir_name_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_prestat_get_wasm64","fd_prestat_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_pwrite_wasm64","fd_pwrite_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_read_wasm64","fd_read_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_readdir_wasm64","fd_readdir_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_renumber_wasm64","fd_renumber_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_seek_wasm64","fd_seek_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_sync_wasm64","fd_sync_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_tell_wasm64","fd_tell_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","fd_write_wasm64","fd_write_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_create_directory_wasm64","path_create_directory_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_filestat_get_wasm64","path_filestat_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_filestat_set_times_wasm64","path_filestat_set_times_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_link_wasm64","path_link_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_open_wasm64","path_open_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_readlink_wasm64","path_readlink_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_remove_directory_wasm64","path_remove_directory_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_rename_wasm64","path_rename_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_symlink_wasm64","path_symlink_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","path_unlink_file_wasm64","path_unlink_file_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","poll_oneoff_wasm64","poll_oneoff_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","proc_raise_wasm64","proc_raise_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","random_get_wasm64","random_get_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","sched_yield_wasm64","sched_yield_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","sock_accept_wasm64","sock_accept_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","sock_recv_wasm64","sock_recv_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","sock_send_wasm64","sock_send_wasm64"},
        entry{family::wasip1,"wasi_snapshot_preview1","sock_shutdown_wasm64","sock_shutdown_wasm64"},
        entry{family::ros1,"uwvm_ros1_v1","abi_version","uwvm_ros1_v1.abi_version"},
        entry{family::ros1,"uwvm_ros1_v1","capability_open","uwvm_ros1_v1.capability_open"},
        entry{family::ros1,"uwvm_ros1_v1","capability_close","uwvm_ros1_v1.capability_close"},
        entry{family::ros1,"uwvm_ros1_v1","capability_revoke","uwvm_ros1_v1.capability_revoke"},
        entry{family::ros1,"uwvm_ros1_v1","topic_publish","uwvm_ros1_v1.topic_publish"},
        entry{family::ros1,"uwvm_ros1_v1","topic_subscribe","uwvm_ros1_v1.topic_subscribe"},
        entry{family::ros1,"uwvm_ros1_v1","event_poll","uwvm_ros1_v1.event_poll"},
        entry{family::ros1,"uwvm_ros1_v1","event_watch","uwvm_ros1_v1.event_watch"},
        entry{family::ros1,"uwvm_ros1_v1","event_poll_v1","uwvm_ros1_v1.event_poll_v1"},
        entry{family::ros1,"uwvm_ros1_v1","event_ready","uwvm_ros1_v1.event_ready"},
        entry{family::ros1,"uwvm_ros1_v1","service_submit","uwvm_ros1_v1.service_submit"},
        entry{family::ros1,"uwvm_ros1_v1","service_call","uwvm_ros1_v1.service_call"},
        entry{family::ros1,"uwvm_ros1_v1","service_advertise","uwvm_ros1_v1.service_advertise"},
        entry{family::ros1,"uwvm_ros1_v1","service_respond","uwvm_ros1_v1.service_respond"},
        entry{family::ros1,"uwvm_ros1_v1","action_send_goal","uwvm_ros1_v1.action_send_goal"},
        entry{family::ros1,"uwvm_ros1_v1","action_cancel_own_goal","uwvm_ros1_v1.action_cancel_own_goal"},
        entry{family::ros1,"uwvm_ros1_v1","action_cancel_any_goal","uwvm_ros1_v1.action_cancel_any_goal"},
        entry{family::ros1,"uwvm_ros1_v1","action_receive_feedback","uwvm_ros1_v1.action_receive_feedback"},
        entry{family::ros1,"uwvm_ros1_v1","action_receive_result","uwvm_ros1_v1.action_receive_result"},
        entry{family::ros1,"uwvm_ros1_v1","action_advertise","uwvm_ros1_v1.action_advertise"},
        entry{family::ros1,"uwvm_ros1_v1","action_respond","uwvm_ros1_v1.action_respond"},
        entry{family::ros1,"uwvm_ros1_v1","tf_lookup","uwvm_ros1_v1.tf_lookup"},
        entry{family::ros1,"uwvm_ros1_v1","tf_broadcast","uwvm_ros1_v1.tf_broadcast"},
        entry{family::ros1,"uwvm_ros1_v1","parameter_get","uwvm_ros1_v1.parameter_get"},
        entry{family::ros1,"uwvm_ros1_v1","parameter_set","uwvm_ros1_v1.parameter_set"},
        entry{family::ros1,"uwvm_ros1_v1","parameter_delete","uwvm_ros1_v1.parameter_delete"},
        entry{family::ros1,"uwvm_ros1_v1","parameter_exists","uwvm_ros1_v1.parameter_exists"},
        entry{family::ros1,"uwvm_ros1_v1","parameter_list","uwvm_ros1_v1.parameter_list"},
        entry{family::ros1,"uwvm_ros1_v1","parameter_search","uwvm_ros1_v1.parameter_search"},
        entry{family::ros1,"uwvm_ros1_v1","graph_inspect_topic","uwvm_ros1_v1.graph_inspect_topic"},
        entry{family::ros1,"uwvm_ros1_v1","graph_inspect_node","uwvm_ros1_v1.graph_inspect_node"},
        entry{family::ros1,"uwvm_ros1_v1","graph_inspect_service","uwvm_ros1_v1.graph_inspect_service"},
        entry{family::ros1,"uwvm_ros1_v1","graph_list_topics","uwvm_ros1_v1.graph_list_topics"},
        entry{family::ros1,"uwvm_ros1_v1","graph_list_nodes","uwvm_ros1_v1.graph_list_nodes"},
        entry{family::ros1,"uwvm_ros1_v1","graph_list_services","uwvm_ros1_v1.graph_list_services"},
        entry{family::ros1,"uwvm_ros1_v1","time_read_ros","uwvm_ros1_v1.time_read_ros"},
        entry{family::ros1,"uwvm_ros1_v1","time_read_wall","uwvm_ros1_v1.time_read_wall"},
        entry{family::ros1,"uwvm_ros1_v1","log_write","uwvm_ros1_v1.log_write"},
        entry{family::ros1,"uwvm_ros1_client_v1","abi_version","uwvm_ros1_client_v1.abi_version"},
        entry{family::ros1,"uwvm_ros1_client_v1","features","uwvm_ros1_client_v1.features"},
        entry{family::ros1,"uwvm_ros1_client_v1","service_available","uwvm_ros1_client_v1.service_available"},
        entry{family::ros2,"uwvm_ros2_v1","abi_version","uwvm_ros2_v1.abi_version"},
        entry{family::ros2,"uwvm_ros2_v1","capability_open","uwvm_ros2_v1.capability_open"},
        entry{family::ros2,"uwvm_ros2_v1","capability_close","uwvm_ros2_v1.capability_close"},
        entry{family::ros2,"uwvm_ros2_v1","capability_revoke","uwvm_ros2_v1.capability_revoke"},
        entry{family::ros2,"uwvm_ros2_v1","topic_publish","uwvm_ros2_v1.topic_publish"},
        entry{family::ros2,"uwvm_ros2_v1","topic_subscribe","uwvm_ros2_v1.topic_subscribe"},
        entry{family::ros2,"uwvm_ros2_v1","event_poll","uwvm_ros2_v1.event_poll"},
        entry{family::ros2,"uwvm_ros2_v1","event_watch","uwvm_ros2_v1.event_watch"},
        entry{family::ros2,"uwvm_ros2_v1","event_poll_v1","uwvm_ros2_v1.event_poll_v1"},
        entry{family::ros2,"uwvm_ros2_v1","event_ready","uwvm_ros2_v1.event_ready"},
        entry{family::ros2,"uwvm_ros2_v1","service_submit","uwvm_ros2_v1.service_submit"},
        entry{family::ros2,"uwvm_ros2_v1","service_call","uwvm_ros2_v1.service_call"},
        entry{family::ros2,"uwvm_ros2_v1","service_advertise","uwvm_ros2_v1.service_advertise"},
        entry{family::ros2,"uwvm_ros2_v1","service_respond","uwvm_ros2_v1.service_respond"},
        entry{family::ros2,"uwvm_ros2_v1","action_send_goal","uwvm_ros2_v1.action_send_goal"},
        entry{family::ros2,"uwvm_ros2_v1","action_server_is_ready","uwvm_ros2_v1.action_server_is_ready"},
        entry{family::ros2,"uwvm_ros2_v1","action_send_begin","uwvm_ros2_v1.action_send_begin"},
        entry{family::ros2,"uwvm_ros2_v1","action_goal_response","uwvm_ros2_v1.action_goal_response"},
        entry{family::ros2,"uwvm_ros2_v1","action_goal_status","uwvm_ros2_v1.action_goal_status"},
        entry{family::ros2,"uwvm_ros2_v1","action_cancel_begin_own","uwvm_ros2_v1.action_cancel_begin_own"},
        entry{family::ros2,"uwvm_ros2_v1","action_cancel_response","uwvm_ros2_v1.action_cancel_response"},
        entry{family::ros2,"uwvm_ros2_v1","action_cancel_own_goal","uwvm_ros2_v1.action_cancel_own_goal"},
        entry{family::ros2,"uwvm_ros2_v1","action_cancel_any_goal","uwvm_ros2_v1.action_cancel_any_goal"},
        entry{family::ros2,"uwvm_ros2_v1","action_receive_feedback","uwvm_ros2_v1.action_receive_feedback"},
        entry{family::ros2,"uwvm_ros2_v1","action_receive_result","uwvm_ros2_v1.action_receive_result"},
        entry{family::ros2,"uwvm_ros2_v1","action_advertise","uwvm_ros2_v1.action_advertise"},
        entry{family::ros2,"uwvm_ros2_v1","action_respond","uwvm_ros2_v1.action_respond"},
        entry{family::ros2,"uwvm_ros2_v1","tf_lookup","uwvm_ros2_v1.tf_lookup"},
        entry{family::ros2,"uwvm_ros2_v1","tf_broadcast","uwvm_ros2_v1.tf_broadcast"},
        entry{family::ros2,"uwvm_ros2_v1","parameter_get","uwvm_ros2_v1.parameter_get"},
        entry{family::ros2,"uwvm_ros2_v1","parameter_set","uwvm_ros2_v1.parameter_set"},
        entry{family::ros2,"uwvm_ros2_v1","parameter_list","uwvm_ros2_v1.parameter_list"},
        entry{family::ros2,"uwvm_ros2_v1","parameter_describe","uwvm_ros2_v1.parameter_describe"},
        entry{family::ros2,"uwvm_ros2_v1","parameter_declare","uwvm_ros2_v1.parameter_declare"},
        entry{family::ros2,"uwvm_ros2_v1","parameter_undeclare","uwvm_ros2_v1.parameter_undeclare"},
        entry{family::ros2,"uwvm_ros2_v1","graph_inspect_topic","uwvm_ros2_v1.graph_inspect_topic"},
        entry{family::ros2,"uwvm_ros2_v1","graph_inspect_node","uwvm_ros2_v1.graph_inspect_node"},
        entry{family::ros2,"uwvm_ros2_v1","graph_inspect_service","uwvm_ros2_v1.graph_inspect_service"},
        entry{family::ros2,"uwvm_ros2_v1","graph_inspect_action","uwvm_ros2_v1.graph_inspect_action"},
        entry{family::ros2,"uwvm_ros2_v1","parameter_subscribe_events","uwvm_ros2_v1.parameter_subscribe_events"},
        entry{family::ros2,"uwvm_ros2_v1","graph_list_topics","uwvm_ros2_v1.graph_list_topics"},
        entry{family::ros2,"uwvm_ros2_v1","graph_list_nodes","uwvm_ros2_v1.graph_list_nodes"},
        entry{family::ros2,"uwvm_ros2_v1","graph_list_services","uwvm_ros2_v1.graph_list_services"},
        entry{family::ros2,"uwvm_ros2_v1","lifecycle_get_state","uwvm_ros2_v1.lifecycle_get_state"},
        entry{family::ros2,"uwvm_ros2_v1","lifecycle_change_state","uwvm_ros2_v1.lifecycle_change_state"},
        entry{family::ros2,"uwvm_ros2_v1","time_read_ros","uwvm_ros2_v1.time_read_ros"},
        entry{family::ros2,"uwvm_ros2_v1","time_read_system","uwvm_ros2_v1.time_read_system"},
        entry{family::ros2,"uwvm_ros2_v1","time_read_steady","uwvm_ros2_v1.time_read_steady"},
        entry{family::ros2,"uwvm_ros2_v1","log_write","uwvm_ros2_v1.log_write"},
        entry{family::ros2,"uwvm_ros2_client_v1","abi_version","uwvm_ros2_client_v1.abi_version"},
        entry{family::ros2,"uwvm_ros2_client_v1","features","uwvm_ros2_client_v1.features"},
        entry{family::ros2,"uwvm_ros2_client_v1","service_available","uwvm_ros2_client_v1.service_available"},
        entry{family::ros2,"uwvm_ros2_rmw_v1","abi_version","uwvm_ros2_rmw_v1.abi_version"},
        entry{family::ros2,"uwvm_ros2_rmw_v1","publisher_activate","uwvm_ros2_rmw_v1.publisher_activate"},
        entry{family::ros2,"uwvm_ros2_rmw_v1","publisher_inspect","uwvm_ros2_rmw_v1.publisher_inspect"},
        entry{family::ros2,"uwvm_ros2_rmw_v1","publisher_publish","uwvm_ros2_rmw_v1.publisher_publish"},
        entry{family::ros2,"uwvm_ros2_rmw_v1","publisher_retire","uwvm_ros2_rmw_v1.publisher_retire"},
        entry{family::ros2,"uwvm_ros2_rmw_sub_v1","abi_version","uwvm_ros2_rmw_sub_v1.abi_version"},
        entry{family::ros2,"uwvm_ros2_rmw_sub_v1","subscription_activate","uwvm_ros2_rmw_sub_v1.subscription_activate"},
        entry{family::ros2,"uwvm_ros2_rmw_sub_v1","subscription_inspect","uwvm_ros2_rmw_sub_v1.subscription_inspect"},
        entry{family::ros2,"uwvm_ros2_rmw_sub_v1","subscription_peek","uwvm_ros2_rmw_sub_v1.subscription_peek"},
        entry{family::ros2,"uwvm_ros2_rmw_sub_v1","subscription_commit","uwvm_ros2_rmw_sub_v1.subscription_commit"},
        entry{family::ros2,"uwvm_ros2_rmw_sub_v1","subscription_cancel","uwvm_ros2_rmw_sub_v1.subscription_cancel"},
        entry{family::ros2,"uwvm_ros2_rmw_sub_v1","subscription_retire","uwvm_ros2_rmw_sub_v1.subscription_retire"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","abi_version","uwvm_ros2_rmw_srv_v1.abi_version"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","client_activate","uwvm_ros2_rmw_srv_v1.client_activate"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","service_activate","uwvm_ros2_rmw_srv_v1.service_activate"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","endpoint_inspect","uwvm_ros2_rmw_srv_v1.endpoint_inspect"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","client_submit","uwvm_ros2_rmw_srv_v1.client_submit"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","client_abandon","uwvm_ros2_rmw_srv_v1.client_abandon"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","service_respond","uwvm_ros2_rmw_srv_v1.service_respond"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","endpoint_peek","uwvm_ros2_rmw_srv_v1.endpoint_peek"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","endpoint_commit","uwvm_ros2_rmw_srv_v1.endpoint_commit"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","endpoint_cancel","uwvm_ros2_rmw_srv_v1.endpoint_cancel"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","client_available","uwvm_ros2_rmw_srv_v1.client_available"},
        entry{family::ros2,"uwvm_ros2_rmw_srv_v1","endpoint_retire","uwvm_ros2_rmw_srv_v1.endpoint_retire"},
        entry{family::ros2,"uwvm_portable_robot_v1","abi_version","uwvm_portable_robot_v1.abi_version"},
        entry{family::ros2,"uwvm_portable_robot_v1","open","uwvm_portable_robot_v1.open"},
        entry{family::ros2,"uwvm_portable_robot_v1","describe","uwvm_portable_robot_v1.describe"},
        entry{family::ros2,"uwvm_portable_robot_v1","read_state","uwvm_portable_robot_v1.read_state"},
        entry{family::ros2,"uwvm_portable_robot_v1","submit","uwvm_portable_robot_v1.submit"},
        entry{family::ros2,"uwvm_portable_robot_v1","wait","uwvm_portable_robot_v1.wait"},
        entry{family::ros2,"uwvm_portable_robot_v1","close","uwvm_portable_robot_v1.close"},
        entry{family::wasip1,"wasi_snapshot_preview1","proc_exit","proc_exit"},
        entry{family::wasip1,"wasi_snapshot_preview1","proc_exit_wasm64","proc_exit_wasm64"},
        entry{family::wasip1,"wasi","thread-spawn","thread-spawn"}
    };
    inline constexpr auto invalid{catalogue.size()};
    constexpr ::std::string_view name(family value) noexcept
    { return value==family::wasip1 ? "wasip1" : value==family::ros1 ? "ros1":"ros2"; }
    template<typename Text>
    constexpr bool equal(Text const& value, ::std::string_view expected) noexcept
    {
        if(value.size()!=expected.size()) { return false; }
        for(::std::size_t i{};i!=expected.size();++i)
        { if(static_cast<unsigned char>(value[i])!=static_cast<unsigned char>(expected[i])) { return false; } }
        return true;
    }
    namespace lookup_details
    {
        template<typename Text>
        constexpr ::std::uint32_t hash_text(Text const& text, ::std::uint32_t value = 2166136261u) noexcept
        {
            for(::std::size_t i{}; i != text.size(); ++i)
            {
                value ^= static_cast<unsigned char>(text[i]);
                value *= 16777619u;
            }
            return value;
        }
        template<typename Text>
        constexpr ::std::uint32_t key_hash(family group, Text const& key) noexcept
        { return hash_text(key, (2166136261u ^ static_cast<unsigned char>(group)) * 16777619u); }
        template<typename Namespace, typename Function>
        constexpr ::std::uint32_t import_hash(Namespace const& ns, Function const& function) noexcept
        { return hash_text(function, (hash_text(ns) ^ 0xffu) * 16777619u); }

        // Derived from the catalogue, not an independent registry capacity.
        inline constexpr ::std::size_t table_size = []() constexpr
        {
            ::std::size_t size{1u};
            while(size <= catalogue.size()) { size *= 2u; }
            return size * 2u;
        }();
        using table_type = ::std::array<::std::size_t, table_size>;
        constexpr table_type make_table(bool imports)
        {
            table_type result{};
            for(auto& slot:result) { slot=invalid; }
            for(::std::size_t i{}; i != catalogue.size(); ++i)
            {
                auto const& item{catalogue[i]};
                auto const hash{imports ? import_hash(item.name_space,item.function) : key_hash(item.group,item.key)};
                ::std::size_t slot{hash & (table_size-1u)};
                while(result[slot] != invalid) { slot = (slot+1u) & (table_size-1u); }
                result[slot] = i;
            }
            return result;
        }
        inline constexpr auto keys{make_table(false)};
        inline constexpr auto imports{make_table(true)};
        inline constexpr auto longest = []() constexpr
        {
            ::std::array<::std::size_t,3u> result{};
            for(auto const& item:catalogue)
            {
                if(item.key.size() > result[0]) { result[0] = item.key.size(); }
                if(item.name_space.size() > result[1]) { result[1] = item.name_space.size(); }
                if(item.function.size() > result[2]) { result[2] = item.function.size(); }
            }
            return result;
        }();
    }
    template<typename Text>
    constexpr ::std::size_t find(family group, Text const& key) noexcept
    {
        // A longer name cannot match any registered entry. This avoids hashing
        // arbitrary unregistered input in full; adding an entry updates this
        // derived length automatically.
        if(key.size() > lookup_details::longest[0]) { return invalid; }
        ::std::size_t slot{lookup_details::key_hash(group,key) & (lookup_details::table_size-1u)};
        for(;;)
        {
            auto const index{lookup_details::keys[slot]};
            if(index == invalid) { return invalid; }
            auto const& item{catalogue[index]};
            if(item.group == group && equal(key,item.key)) { return index; }
            slot = (slot+1u) & (lookup_details::table_size-1u);
        }
    }
    template<typename Namespace,typename Function>
    constexpr ::std::size_t find(Namespace const& ns,Function const& function) noexcept
    {
        if(ns.size() > lookup_details::longest[1] || function.size() > lookup_details::longest[2]) { return invalid; }
        ::std::size_t slot{lookup_details::import_hash(ns,function) & (lookup_details::table_size-1u)};
        for(;;)
        {
            auto const index{lookup_details::imports[slot]};
            if(index == invalid) { return invalid; }
            auto const& item{catalogue[index]};
            if(equal(ns,item.name_space) && equal(function,item.function)) { return index; }
            slot = (slot+1u) & (lookup_details::table_size-1u);
        }
    }
    template<typename Text>
    constexpr bool parse_access(Text const& text,access& value) noexcept
    {
        if(equal(text,"hidden")) { value=access::hidden; }
        else if(equal(text,"deny")) { value=access::deny; }
        else if(equal(text,"allow")) { value=access::allow; }
        else { return false; }
        return true;
    }
    constexpr ::std::string_view name(access value) noexcept
    { return value==access::hidden ? "hidden":value==access::deny ? "deny":"allow"; }
    struct policy
    {
        // Size follows the actual registry, not a configurable capacity limit.
        ::std::array<access,catalogue.size()> entries{};
        ::std::array<bool,catalogue.size()> configured{};
        constexpr bool any_configured() const noexcept { for(bool value:configured) if(value) return true; return false; }
        constexpr bool set(::std::size_t index,access value) noexcept
        {
            if(index>=entries.size() || configured[index]) { return false; }
            entries[index]=value;configured[index]=true;return true;
        }
        constexpr bool exposed(::std::size_t index) const noexcept
        { return index<entries.size() && entries[index]!=access::hidden; }
        constexpr bool allowed(::std::size_t index) const noexcept
        { return index<entries.size() && entries[index]==access::allow; }
    };
}
