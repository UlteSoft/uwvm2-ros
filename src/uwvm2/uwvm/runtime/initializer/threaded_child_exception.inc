// Escape a private engine exception through the same checked, opaque reference
// registry used by GC, globals and tables. No native arena pointer is exposed.
// Reader supplies one complete, validated field from the engine-owned payload.
    template<typename Reader>
    [[nodiscard]] inline ::uwvm2::uwvm::runtime::storage::gc_object_status
        make_threaded_exception_reference(ThreadedChildWorld const& world,::std::size_t module,
            ::uwvm2::uwvm::runtime::storage::local_defined_tag_storage_t const& tag,
            Reader&& read,::uwvm2::uwvm::runtime::storage::gc_reference& output) noexcept
    {
        namespace st=::uwvm2::uwvm::runtime::storage;
        namespace ex=::uwvm2::runtime::exception;
        using status=st::gc_object_status;
        auto const* instance=world.module(module);
        if(!instance||!instance->gc_store||!tag.function_type_ptr||!tag.exception_identity)return status::invalid_store;
        auto const& parameters=tag.function_type_ptr->parameter;
        auto const count=parameters.begin==parameters.end?0u:static_cast<::std::size_t>(parameters.end-parameters.begin);
        constexpr auto overhead=sizeof(ex::value)+4u*sizeof(void*);
        if(count>(SIZE_MAX-overhead)/sizeof(ex::payload_field)||
           !world.charge_execution_storage(overhead+count*sizeof(ex::payload_field)))return status::out_of_memory;
        try
        {
            ::uwvm2::utils::container::std_vector<ex::payload_field> fields;fields.reserve(count);
            for(::std::size_t i{};i<count;++i)
            {
                auto const code=static_cast<unsigned>(parameters.begin[i]);ex::payload_kind kind{};bool reference{};
                switch(code)
                {
                    case 0x7f:kind=ex::payload_kind::i32;break;case 0x7e:kind=ex::payload_kind::i64;break;
                    case 0x7d:kind=ex::payload_kind::f32;break;case 0x7c:kind=ex::payload_kind::f64;break;
                    case 0x7b:kind=ex::payload_kind::v128;break;
                    case 0x70:case 0x6f:case 0x69:reference=true;kind=ex::payload_kind::wasm_reference;break;
                    default:return status::invalid_value;
                }
                auto const bits=read(i,ex::payload_width(kind));
                if(bits.size()!=ex::payload_width(kind))return status::invalid_value;
                if(reference)
                {
                    st::gc_reference ref{};::std::memcpy(&ref,bits.data(),sizeof(ref));
                    auto const retained=instance->gc_store->retain_gc_reference(ref);
                    if(retained!=status::ok)return retained;
                    auto field=ex::payload_field::wasm_reference(bits,instance->gc_store->root_reference(ref));
                    if(!field)return status::invalid_value;fields.push_back(::std::move(*field));
                }
                else
                {
                    auto field=ex::payload_field::numeric(kind,bits);if(!field)return status::invalid_value;
                    fields.push_back(::std::move(*field));
                }
            }
            auto value=ex::value::make_owned(tag.exception_identity,::std::move(fields));
            if(!value)return status::invalid_value;
            return instance->gc_store->make_exn_reference(value,output);
        }
        catch(...) { return status::out_of_memory; }
    }
