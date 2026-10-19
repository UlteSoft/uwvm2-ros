#pragma once
#include <cstdlib>
#include <stdexcept>

namespace uwvm2::ros_gateway::ros2_environment
{
    // Called only during trusted startup, before the context and spinner exist.
    // An unsuccessful environment edit must never leave an inherited value in
    // place while the IPC manifest claims the explicitly configured value.
    inline void set(char const* name,char const* value)
    {
        if(::setenv(name,value,1)!=0)
        { throw std::runtime_error("cannot install explicit ROS2 environment"); }
    }
    inline void clear(char const* name)
    {
        if(::unsetenv(name)!=0)
        { throw std::runtime_error("cannot clear inherited ROS2 environment"); }
    }
    inline void clear_enclave_override()
    {
        // Jazzy rcl_get_secure_root prefers this environment variable over the
        // enclave passed in --ros-args. Only the validated configuration chooses
        // the Gateway's signed identity; this creates no Guest capability.
        clear("ROS_SECURITY_ENCLAVE_OVERRIDE");
        clear("ROS_SECURITY_NODE_DIRECTORY");
        clear("ROS_SECURITY_ROOT_DIRECTORY");
        // No SDK generation inherits a second security selector. Modern setup
        // explicitly sets KEYSTORE; the legacy branch explicitly sets ROOT and
        // the exact checked NODE_DIRECTORY, never a prefix-search default.
        clear("ROS_SECURITY_KEYSTORE");
    }
}
