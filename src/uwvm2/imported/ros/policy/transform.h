/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <array>
#include "schema.h"

namespace uwvm2::imported::ros
{
    struct transform_fields
    {
        ::std::string source_frame{}, target_frame{};
        ::std::array<double, 3> translation{};
        ::std::array<double, 4> quaternion{};
        ::std::uint32_t nanoseconds{};
    };

    // Run after generated ROS decoding, before guest delivery or broadcasting.
    // This primitive alone does not make a TF adapter or TF grant supported.
    inline result filter_transform(capability_spec const& cap, transform_fields const& transform)
    {
        if(cap.operation != "tf.lookup" && cap.operation != "tf.broadcast") { return {errc::permission_denied, "independent TF operation capability required"}; }
        auto source{cap.limits.find("source-frame")}, target{cap.limits.find("target-frame")};
        if(source == cap.limits.end() || target == cap.limits.end() || source->second != transform.source_frame || target->second != transform.target_frame ||
           !canonical_resource("/" + transform.source_frame) || !canonical_resource("/" + transform.target_frame) || transform.source_frame == transform.target_frame)
        { return {errc::permission_denied, "decoded transform falls outside exact frame grant"}; }
        auto maximum{cap.limits.find("tf.max-translation-abs")}, tolerance{cap.limits.find("tf.quaternion-norm-tolerance")};
        double bound{}, epsilon{};
        if(maximum == cap.limits.end() || tolerance == cap.limits.end() || !parse_positive_double(maximum->second, bound) || !parse_positive_double(tolerance->second, epsilon) || epsilon > 0.001)
        { return {errc::invalid_argument, "explicit finite translation bound and quaternion tolerance required"}; }
        if(transform.nanoseconds >= 1000000000) { return {errc::invalid_argument, "invalid transform timestamp"}; }
        for(double value : transform.translation)
        { if(!::std::isfinite(value)) { return {errc::invalid_argument, "nonfinite translation"}; } if(::std::abs(value) > bound) { return {errc::permission_denied, "translation exceeds explicit application bound"}; } }
        double norm{};
        for(double value : transform.quaternion)
        { if(!::std::isfinite(value) || ::std::abs(value) > 1. + epsilon) { return {errc::invalid_argument, "invalid rotation quaternion"}; } norm += value * value; }
        if(!::std::isfinite(norm) || ::std::abs(norm - 1.) > epsilon) { return {errc::invalid_argument, "rotation quaternion is not unit length"}; }
        return {};
    }
}
