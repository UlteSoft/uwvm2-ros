/** @file
 * @brief Allocation-free bounded rigid-transform arithmetic for scoped frame caches.
 * @details Units are metres, radians and nanoseconds. A transform maps a child
 * point into its parent as R*p+t. Quaternion order is x,y,z,w. All externally
 * supplied values must be finite, translation magnitude is deployment-bounded,
 * and quaternion normalization is accepted only within an explicit tolerance.
 * Outputs are transactional: refusal leaves the caller's result unchanged.
 * This layer grants no frame/clock/source authority and performs no I/O.
 */
#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace uwvm2::imported::ros::scene::transforms::math
{
    struct vector {double x{},y{},z{};friend bool operator==(vector const&,vector const&)=default;};
    struct quaternion {double x{},y{},z{},w{1};friend bool operator==(quaternion const&,quaternion const&)=default;};
    struct rigid {vector translation;quaternion rotation;friend bool operator==(rigid const&,rigid const&)=default;};
    /** @brief No implicit physical extent or quaternion-repair allowance. */
    struct bounds {double translation{},quaternion_tolerance{};};
    inline bool finite(vector value) noexcept{return std::isfinite(value.x)&&std::isfinite(value.y)&&std::isfinite(value.z);}
    inline bool finite(quaternion value) noexcept{return std::isfinite(value.x)&&std::isfinite(value.y)&&std::isfinite(value.z)&&std::isfinite(value.w);}
    inline double norm(vector value) noexcept{return std::hypot(value.x,value.y,value.z);}
    inline double norm(quaternion value) noexcept{return std::hypot(std::hypot(value.x,value.y),std::hypot(value.z,value.w));}
    inline vector add(vector a,vector b) noexcept{return {a.x+b.x,a.y+b.y,a.z+b.z};}
    inline vector subtract(vector a,vector b) noexcept{return {a.x-b.x,a.y-b.y,a.z-b.z};}
    inline vector scale(vector a,double factor) noexcept{return {a.x*factor,a.y*factor,a.z*factor};}
    inline vector cross(vector a,vector b) noexcept{return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
    inline quaternion conjugate(quaternion value) noexcept{return {-value.x,-value.y,-value.z,value.w};}
    inline quaternion multiply(quaternion a,quaternion b) noexcept
    {
        return {a.w*b.x+a.x*b.w+a.y*b.z-a.z*b.y,
                a.w*b.y-a.x*b.z+a.y*b.w+a.z*b.x,
                a.w*b.z+a.x*b.y-a.y*b.x+a.z*b.w,
                a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z};
    }
    /** @brief Rotate by an already normalized quaternion; no frame lookup or allocation. */
    inline vector rotate_unit(quaternion q,vector value) noexcept
    {
        vector axis{q.x,q.y,q.z};auto twice=scale(cross(axis,value),2);
        return add(value,add(scale(twice,q.w),cross(axis,twice)));
    }
    inline bool valid(bounds value) noexcept
    {return std::isfinite(value.translation)&&value.translation>=0&&std::isfinite(value.quaternion_tolerance)&&value.quaternion_tolerance>=0&&value.quaternion_tolerance<1;}
    /** @brief Validate and normalize one raw record within explicitly configured error. */
    inline bool normalize(rigid value,bounds limit,rigid& output) noexcept
    {
        if(!valid(limit)||!finite(value.translation)||!finite(value.rotation)||norm(value.translation)>limit.translation)return false;
        auto length=norm(value.rotation);
        if(!std::isfinite(length)||length==0||std::abs(length-1)>limit.quaternion_tolerance)return false;
        value.rotation={value.rotation.x/length,value.rotation.y/length,value.rotation.z/length,value.rotation.w/length};
        output=value;return true;
    }
    namespace detail
    {
        // Only arithmetic on already validated unit rotations reaches this path.
        inline bool result(rigid value,double extent,rigid& output) noexcept
        {
            auto length=norm(value.rotation);
            if(!finite(value.translation)||norm(value.translation)>extent||!finite(value.rotation)||!std::isfinite(length)||length==0)return false;
            value.rotation={value.rotation.x/length,value.rotation.y/length,value.rotation.z/length,value.rotation.w/length};
            output=value;return true;
        }
    }
    /** @brief Compose parent<-middle and middle<-child, bounding the final extent. */
    inline bool compose(rigid parent,rigid child,bounds limit,rigid& output) noexcept
    {
        rigid a,b;if(!normalize(parent,limit,a)||!normalize(child,limit,b))return false;
        return detail::result({add(a.translation,rotate_unit(a.rotation,b.translation)),multiply(a.rotation,b.rotation)},limit.translation,output);
    }
    /** @brief Invert a checked rigid transform without matrix allocation. */
    inline bool inverse(rigid input,bounds limit,rigid& output) noexcept
    {
        rigid value;if(!normalize(input,limit,value))return false;auto rotation=conjugate(value.rotation);
        return detail::result({rotate_unit(rotation,scale(value.translation,-1)),rotation},limit.translation,output);
    }
    /** @brief Interpolate within an exact inclusive integer-time interval.
     * @details Exact zero and negative timestamps are ordinary instants. The
     * full signed nanosecond range is supported without signed subtraction
     * overflow. Translation is linear; rotation follows the shortest spherical
     * path, treating q and -q as the same orientation. No extrapolation or
     * implicit time-window clipping is performed. Frame/epoch agreement must
     * have been established by the enclosing cache before calling this method.
     */
    inline bool interpolate(rigid first,std::int64_t first_time,rigid last,std::int64_t last_time,std::int64_t time,bounds limit,rigid& output) noexcept
    {
        if(first_time>=last_time||time<first_time||time>last_time)return false;
        rigid a,b;if(!normalize(first,limit,a)||!normalize(last,limit,b))return false;
        if(time==first_time){output=a;return true;}if(time==last_time){output=b;return true;}
        auto elapsed=static_cast<std::uint64_t>(time)-static_cast<std::uint64_t>(first_time);
        auto span=static_cast<std::uint64_t>(last_time)-static_cast<std::uint64_t>(first_time);
        double ratio=static_cast<double>(elapsed)/static_cast<double>(span);
        double dot=a.rotation.x*b.rotation.x+a.rotation.y*b.rotation.y+a.rotation.z*b.rotation.z+a.rotation.w*b.rotation.w;
        if(dot<0){b.rotation={-b.rotation.x,-b.rotation.y,-b.rotation.z,-b.rotation.w};dot=-dot;}
        dot=std::clamp(dot,0.0,1.0);double first_weight=1-ratio,last_weight=ratio;
        if(dot<1-64*std::numeric_limits<double>::epsilon())
        {auto angle=std::acos(dot),sine=std::sin(angle);first_weight=std::sin((1-ratio)*angle)/sine;last_weight=std::sin(ratio*angle)/sine;}
        quaternion rotation{a.rotation.x*first_weight+b.rotation.x*last_weight,a.rotation.y*first_weight+b.rotation.y*last_weight,
                            a.rotation.z*first_weight+b.rotation.z*last_weight,a.rotation.w*first_weight+b.rotation.w*last_weight};
        return detail::result({{std::lerp(a.translation.x,b.translation.x,ratio),std::lerp(a.translation.y,b.translation.y,ratio),
                               std::lerp(a.translation.z,b.translation.z,ratio)},rotation},limit.translation,output);
    }
    struct velocity {vector linear,angular;};
    /** @brief Differentiate a body pose over a complete, explicit time interval.
     * @details Both endpoints map the same body into the same observation
     * frame. Angular velocity is expressed in that observation frame and uses
     * the shortest relative rotation (principal angle at most pi). Sampling
     * faster rotations is ambiguous; the deployment must enforce its sampling
     * and angular-speed limits separately. The supplied positive maximum speed
     * bounds are hard refusals, not silent clamping or command safety limits.
     */
    inline bool differentiate(rigid first,rigid last,std::uint64_t interval_ns,bounds limit,double linear_limit,double angular_limit,velocity& output) noexcept
    {
        if(!interval_ns||!std::isfinite(linear_limit)||linear_limit<0||!std::isfinite(angular_limit)||angular_limit<0)return false;
        rigid a,b;if(!normalize(first,limit,a)||!normalize(last,limit,b))return false;
        double seconds=static_cast<double>(interval_ns)*1e-9;
        auto delta=multiply(conjugate(a.rotation),b.rotation);
        if(delta.w<0)delta={-delta.x,-delta.y,-delta.z,-delta.w};
        vector axis{delta.x,delta.y,delta.z};auto sine=norm(axis);vector angular;
        if(sine!=0)angular=rotate_unit(a.rotation,scale(axis,(2*std::atan2(sine,delta.w)/seconds)/sine));
        velocity value{scale(subtract(b.translation,a.translation),1/seconds),angular};
        if(!finite(value.linear)||!finite(value.angular)||norm(value.linear)>linear_limit||norm(value.angular)>angular_limit)return false;
        output=value;return true;
    }
    /** @brief Express body-origin velocity at another physical point and basis.
     * @details observation_point and observation_origin are positions in the
     * same observation frame; reference_from_observation changes only vector
     * coordinates. The point shift uses omega cross displacement, never
     * component-wise multiplication. It does not subtract reference-frame
     * motion: velocity remains relative to the stated observation frame.
     */
    inline bool reference(velocity observed,vector observation_point,vector observation_origin,quaternion reference_from_observation,
                          bounds limit,double linear_limit,double angular_limit,velocity& output) noexcept
    {
        rigid rotation;if(!normalize({{},reference_from_observation},limit,rotation)||!finite(observed.linear)||!finite(observed.angular)||
            !finite(observation_point)||!finite(observation_origin)||norm(observation_point)>limit.translation||norm(observation_origin)>limit.translation||
            !std::isfinite(linear_limit)||linear_limit<0||!std::isfinite(angular_limit)||angular_limit<0)return false;
        auto displacement=subtract(observation_point,observation_origin);
        velocity value{rotate_unit(rotation.rotation,add(observed.linear,cross(observed.angular,displacement))),rotate_unit(rotation.rotation,observed.angular)};
        if(!finite(value.linear)||!finite(value.angular)||norm(value.linear)>linear_limit||norm(value.angular)>angular_limit)return false;
        output=value;return true;
    }
}
