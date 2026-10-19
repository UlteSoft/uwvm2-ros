# Shared source setup for real ROS2 custom-topic adapter consumers. The caller
# supplies its actual provider/manifest definitions, installed ROS dependencies,
# language/optimization settings and a real executable target before this call.
include_guard(GLOBAL)
set_property(GLOBAL PROPERTY UWVM_ROS2_CUSTOM_TOPICS_SOURCE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

# Partition count, never a row count or a permission. No new mandatory input.
set(UWVM_ROS2_CUSTOM_TOPIC_SHARDS 32 CACHE STRING "Custom ROS2 topic template partitions (1..256)")

function(uwvm_ros2_custom_topic_sources target)
  if(NOT TARGET ${target})
    message(FATAL_ERROR "Custom ROS2 topic sources require an existing target")
  endif()
  get_property(custom_topic_already_added TARGET ${target} PROPERTY UWVM_ROS2_CUSTOM_TOPICS_SHARDED)
  if(custom_topic_already_added)
    message(FATAL_ERROR "Custom ROS2 topic sources were already added to ${target}")
  endif()
  if(NOT UWVM_ROS2_CUSTOM_TOPIC_SHARDS MATCHES "^[1-9][0-9]?[0-9]?$" OR UWVM_ROS2_CUSTOM_TOPIC_SHARDS GREATER 256)
    message(FATAL_ERROR "UWVM_ROS2_CUSTOM_TOPIC_SHARDS must be an integer in 1..256")
  endif()
  get_property(custom_topic_source_directory GLOBAL PROPERTY UWVM_ROS2_CUSTOM_TOPICS_SOURCE_DIRECTORY)
  target_compile_definitions(${target} PRIVATE UWVM_ROS2_CUSTOM_TOPIC_SHARDS=${UWVM_ROS2_CUSTOM_TOPIC_SHARDS})
  target_include_directories(${target} PRIVATE "${custom_topic_source_directory}")
  target_sources(${target} PRIVATE "${custom_topic_source_directory}/custom_topics_dispatch.cc")
  set(custom_topic_shard_directory "${CMAKE_CURRENT_BINARY_DIR}/custom-topic-shards")
  file(MAKE_DIRECTORY "${custom_topic_shard_directory}")
  math(EXPR custom_topic_shard_last "${UWVM_ROS2_CUSTOM_TOPIC_SHARDS} - 1")
  foreach(UWVM_ROS2_CUSTOM_TOPIC_SHARD_INDEX RANGE 0 ${custom_topic_shard_last})
    set(custom_topic_shard "${custom_topic_shard_directory}/custom_topics_shard_${UWVM_ROS2_CUSTOM_TOPIC_SHARD_INDEX}.cc")
    configure_file("${custom_topic_source_directory}/custom_topics_shard.cc.in" "${custom_topic_shard}" @ONLY)
    target_sources(${target} PRIVATE "${custom_topic_shard}")
  endforeach()
  set_property(TARGET ${target} PROPERTY UWVM_ROS2_CUSTOM_TOPICS_SHARDED TRUE)
endfunction()
