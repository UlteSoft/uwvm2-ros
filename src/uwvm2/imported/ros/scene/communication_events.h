/** @file
 * @brief Trusted communication-readiness adapter with retained provider lifetime.
 * @details A source object grants no authority. stream::configure/poll additionally
 * require broker admission of each origin resource.observe action. inspect checks
 * the communication resource owner and generation again. Topic payloads remain
 * inaccessible; only explicit subscription readiness is currently supported.
 */
#pragma once
#include "event_streams.h"

namespace uwvm2::imported::ros::scene::events
{
    class communication_source final : public source
    {
        std::shared_ptr<communication::registry> provider;

    public:
        explicit communication_source(std::shared_ptr<communication::registry> selected) : provider(std::move(selected)) {}

        error inspect(owner authority, handle resource, handle generation, snapshot& output) noexcept override
        {
            if(!provider) { return error::denied; }
            try
            {
                snapshot value;
                value.generation = generation;
                auto result = provider->observe(authority, resource, generation, value.revision, value.ready, value.terminal);
                if(result == error::ok) { output = value; }
                return result;
            }
            catch(...)
            {
                return error::io;
            }
        }
    };
}  // namespace uwvm2::imported::ros::scene::events
