/** Exact ABI v1 import inventory derived from host.h and abi/abi-v1.json. No functions contact ROS.
 * The immutable metadata can inspect a compiled module or validate an explicit bridge. */
const inventory = {
  "1": {
    "module": "uwvm_ros1_v1",
    "functions": [
      {
        "name": "abi_version",
        "parameters": [],
        "result": "i32"
      },
      {
        "name": "capability_open",
        "parameters": [
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "alias_offset",
            "type": "i64"
          },
          {
            "name": "alias_length",
            "type": "i64"
          },
          {
            "name": "handle_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "capability_close",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "capability_revoke",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "topic_publish",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "payload_offset",
            "type": "i64"
          },
          {
            "name": "payload_length",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "topic_subscribe",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "event_poll",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "output_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "event_watch",
        "parameters": [
          {
            "name": "observer_handle",
            "type": "i64"
          },
          {
            "name": "source_handle",
            "type": "i64"
          },
          {
            "name": "correlation",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "event_poll_v1",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "output_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "event_ready",
        "parameters": [
          {
            "name": "observer_handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "output_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "service_submit",
        "parameters": [
          {
            "name": "source_handle",
            "type": "i64"
          },
          {
            "name": "observer_handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "request_id_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "service_call",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "service_advertise",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "service_respond",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_send_goal",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_cancel_own_goal",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_cancel_any_goal",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_receive_feedback",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_receive_result",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_advertise",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_respond",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "tf_lookup",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "tf_broadcast",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_get",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_set",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_delete",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_exists",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_list",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_search",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_inspect_topic",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_inspect_node",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_inspect_service",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_list_topics",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_list_nodes",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_list_services",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "time_read_ros",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "time_read_wall",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "log_write",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "payload_offset",
            "type": "i64"
          },
          {
            "name": "payload_length",
            "type": "i64"
          }
        ],
        "result": "i32"
      }
    ]
  },
  "2": {
    "module": "uwvm_ros2_v1",
    "functions": [
      {
        "name": "abi_version",
        "parameters": [],
        "result": "i32"
      },
      {
        "name": "capability_open",
        "parameters": [
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "alias_offset",
            "type": "i64"
          },
          {
            "name": "alias_length",
            "type": "i64"
          },
          {
            "name": "handle_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "capability_close",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "capability_revoke",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "topic_publish",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "payload_offset",
            "type": "i64"
          },
          {
            "name": "payload_length",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "topic_subscribe",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "event_poll",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "output_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "event_watch",
        "parameters": [
          {
            "name": "observer_handle",
            "type": "i64"
          },
          {
            "name": "source_handle",
            "type": "i64"
          },
          {
            "name": "correlation",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "event_poll_v1",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "output_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "event_ready",
        "parameters": [
          {
            "name": "observer_handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "output_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "service_submit",
        "parameters": [
          {
            "name": "source_handle",
            "type": "i64"
          },
          {
            "name": "observer_handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "request_id_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "service_call",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "service_advertise",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "service_respond",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_send_goal",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_cancel_own_goal",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_cancel_any_goal",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_receive_feedback",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_receive_result",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_advertise",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "action_respond",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "tf_lookup",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "tf_broadcast",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_get",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_set",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_list",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_describe",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_declare",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_undeclare",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_inspect_topic",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_inspect_node",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_inspect_service",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_inspect_action",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "parameter_subscribe_events",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_list_topics",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_list_nodes",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "graph_list_services",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "lifecycle_get_state",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "lifecycle_change_state",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "time_read_ros",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "time_read_system",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "time_read_steady",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "request_offset",
            "type": "i64"
          },
          {
            "name": "request_length",
            "type": "i64"
          },
          {
            "name": "response_offset",
            "type": "i64"
          },
          {
            "name": "capacity",
            "type": "i64"
          },
          {
            "name": "length_out_offset",
            "type": "i64"
          }
        ],
        "result": "i32"
      },
      {
        "name": "log_write",
        "parameters": [
          {
            "name": "handle",
            "type": "i64"
          },
          {
            "name": "memory",
            "type": "i32"
          },
          {
            "name": "payload_offset",
            "type": "i64"
          },
          {
            "name": "payload_length",
            "type": "i64"
          }
        ],
        "result": "i32"
      }
    ]
  }
};
/** Recursively freeze metadata to prevent accidental ABI mutation by applications. */
function freeze(value) { if (value && typeof value === "object") { for (const item of Object.values(value)) freeze(item); Object.freeze(value); } return value; }
export const ABI = freeze(inventory);
