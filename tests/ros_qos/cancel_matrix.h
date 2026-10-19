#pragma once
#include <array>

namespace cancellation_qos_test
{
    /** Independent expectations, deliberately not generated from production fields. */
    struct mutation
    {
        char const* key;
        char const* baseline;
        char const* replacement;
        bool individually_valid;
    };
    inline constexpr std::array<mutation,8> mutations{{
        {"qos.cancel-service.reliability","reliable","best-effort",true},
        {"qos.cancel-service.durability","volatile","transient-local",true},
        {"qos.cancel-service.history","keep-last","keep-all",false},
        {"qos.cancel-service.depth","8","9",true},
        {"qos.cancel-service.deadline","infinite","1000000000",true},
        {"qos.cancel-service.lifespan","infinite","1000000000",true},
        {"qos.cancel-service.liveliness","automatic","manual-by-topic",true},
        {"qos.cancel-service.liveliness-lease-duration","infinite","1000000000",true}
    }};
}
