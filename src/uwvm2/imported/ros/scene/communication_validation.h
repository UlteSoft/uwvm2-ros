/** @file
 * @brief Lifetime-safe exact validators for compiled codecs and frozen message schemas.
 * @details A bounded validator owns its immutable description through a
 * recoverable fast_io shared allocation. Copies preserve that ownership across
 * asynchronous worker replies. No borrowed schema pointer or function address
 * is read from guest/configuration bytes. Compiled function identity remains
 * exact; a generic schema can never impersonate a compiled native codec.
 */
#pragma once
#include "bounded_message.h"
#include <uwvm2/utils/container/shared.h>

namespace uwvm2::imported::ros::scene::communication
{
    class message_validator
    {
        using function=bool(*)(wire::bytes)noexcept;
        function compiled_{};
        std::shared_ptr<bounded_message::schema const> schema_;
    public:
        message_validator()noexcept=default;
        message_validator(function value)noexcept:compiled_(value){}
        explicit operator bool()const noexcept{return compiled_||bool(schema_);}
        bool operator()(wire::bytes bytes)const noexcept
        {return compiled_?compiled_(bytes):schema_&&schema_->validate(bytes);}
        /** @brief Atomically install a complete owned description; failure preserves the old validator. */
        bounded_message::status assign(wire::bytes description)noexcept
        {
            try
            {
                auto value=::uwvm2::utils::container::make_shared<bounded_message::schema>();
                auto result=value->parse(description);if(result!=bounded_message::status::ok)return result;
                compiled_=nullptr;schema_=std::move(value);return bounded_message::status::ok;
            }
            catch(std::bad_alloc const&){return bounded_message::status::no_memory;}
        }
        bounded_message::schema const* bounded()const noexcept{return schema_.get();}
        friend bool operator==(message_validator const& left,message_validator const& right)noexcept
        {
            if(left.compiled_!=right.compiled_)return false;
            if(left.schema_==right.schema_)return true;
            if(!left.schema_||!right.schema_)return false;
            auto a=left.schema_->description(),b=right.schema_->description();
            return std::equal(a.begin(),a.end(),b.begin(),b.end());
        }
    };
}
