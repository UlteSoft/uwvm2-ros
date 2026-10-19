inline bool encode_typed_parameter_event(typed_parameter_event const& value,bytes& out) {
    auto tag=static_cast<unsigned char>(value.tag);
    if(value.seconds>2147483647u||value.nanoseconds>=1000000000u||value.kind<1||value.kind>3||tag>9||(tag==0&&value.kind!=3)||
       !valid_parameter_event_name(value.node,true)||!valid_parameter_event_name(value.name,false)) return false;
    // Account for the complete envelope before allocating a value payload.
    auto available=parameter_array_detail::maximum_bytes-23-value.node.size()-value.name.size();
    std::size_t payload_size{};
    if(value.kind!=3) {
        switch(value.tag) {
        case parameter_event_value_tag::boolean:payload_size=1;break;
        case parameter_event_value_tag::integer:payload_size=8;break;
        case parameter_event_value_tag::float64:payload_size=8;if(!std::isfinite(value.double_value)) return false;break;
        case parameter_event_value_tag::string:
            if(available<4||value.string_value.size()>available-4||!parameter_array_detail::text_valid(value.string_value,true)) return false;
            payload_size=4+value.string_value.size();break;
        case parameter_event_value_tag::string_array:
            if(value.string_array_value.size()>parameter_array_detail::maximum_elements||available<4) return false;
            payload_size=4;
            for(auto const& text:value.string_array_value) {
                if(available-payload_size<4||text.size()>available-payload_size-4||!parameter_array_detail::text_valid(text)) return false;
                payload_size+=4+text.size();
            }
            break;
        default: {
            auto count=tag==5?value.byte_array_value.size():tag==6?value.bool_array_value.size():tag==7?value.integer_array_value.size():value.double_array_value.size();
            auto width=tag<=6?1u:8u;
            if(available<4||count>parameter_array_detail::maximum_elements||count>(available-4)/width) return false;
            payload_size=4+count*width;
            if(tag==6) for(auto item:value.bool_array_value) { if(item>1) return false; }
            if(tag==8) for(auto item:value.double_array_value) { if(!std::isfinite(item)) return false; }
            break;
        }
        }
        if(payload_size>available) return false;
    }
    bytes payload;
    if(value.kind!=3) {
        switch(value.tag) {
        case parameter_event_value_tag::boolean:payload=parameter_bool(value.bool_value);break;
        case parameter_event_value_tag::integer:payload=parameter_int64(value.integer_value);break;
        case parameter_event_value_tag::float64:payload=parameter_float64(value.double_value);break;
        case parameter_event_value_tag::string:payload=string(value.string_value);break;
        case parameter_event_value_tag::byte_array:if(!encode_parameter_byte_array(value.byte_array_value,payload)) return false;break;
        case parameter_event_value_tag::boolean_array:if(!encode_parameter_bool_array(value.bool_array_value,payload)) return false;break;
        case parameter_event_value_tag::integer_array:if(!encode_parameter_int64_array(value.integer_array_value,payload)) return false;break;
        case parameter_event_value_tag::float64_array:if(!encode_parameter_float64_array(value.double_array_value,payload)) return false;break;
        case parameter_event_value_tag::string_array:if(!encode_parameter_string_array(value.string_array_value,payload)) return false;break;
        case parameter_event_value_tag::deletion:return false;
        }
    }
    bytes next;next.reserve(23+value.node.size()+value.name.size()+payload_size);append_u32(next,value.seconds);append_u32(next,value.nanoseconds);
    next.push_back(value.kind);next.push_back(2);next.push_back(tag);
    if(!append_string(next,value.node)||!append_string(next,value.name)) return false;
    append_u32(next,static_cast<std::uint32_t>(payload.size()));next.insert(next.end(),payload.begin(),payload.end());
    out=std::move(next);return true;
}
