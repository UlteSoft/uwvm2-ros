// Included in the genuine private runtime bridge. Caller holds the retirement
// exclusion and an authentic all-stop, or runs before launch / after native join.
// No snapshot token, user boolean or recorded environment authorizes this read.
        [[nodiscard]] static bool capture_environment_sample(profile_state& state,
            ::std::array<::std::byte,16> const& label,::std::uint64_t cursor) noexcept
        {
            namespace ar=::uwvm2::runtime::diagnostic::snapshot_archive;
            namespace wasi=::uwvm2::runtime::diagnostic::snapshot_wasip1;
            namespace ros=::uwvm2::runtime::diagnostic::snapshot_ros;
            namespace cp=::uwvm2::imported::ros::checkpoint;
            try
            {
                auto source_owner=::uwvm2::uwvm::runtime::full::selected_full_source_owner_pin();
                if(!source::has_canonical_owner(source_owner)||state.generation!=current_runtime_generation())return false;
                ::uwvm2::utils::container::std_vector<wasi::environment_stream> wasi_streams;
                ::uwvm2::utils::container::std_vector<ros::environment_stream> ros_streams;
                ::uwvm2::utils::container::std_vector<ar::stream_record> records;
                // Stream storage must not move after its addresses enter records.
                wasi_streams.resize(g_runtime.modules.size());
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
                for(::std::size_t actual{};actual<g_runtime.modules.size();++actual)
                {
                    if(!is_wasip1_import_visible_for_runtime_module_id_slow(actual))continue;
                    auto data=llvm_jit_wasip1_environment_capsule::capture_threaded_history_data(source_owner,actual,label);
                    if(!data)return false;
                    auto logical=::uwvm2::utils::container::make_shared<wasi::environment>(*data);
                    logical->module=logical_source_module(actual);
                    for(auto& member:logical->members)member=logical_source_module(static_cast<::std::size_t>(member));
                    ::std::sort(logical->members.begin(),logical->members.end());
                    if(!wasi_streams[actual].bind(::std::move(logical)))return false;
                    records.push_back(wasi_streams[actual].record());
                }
#endif
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
                auto capture=cp::capture_hook.load(::std::memory_order_acquire);
                if(capture)
                {
                    ::uwvm2::utils::container::std_vector<cp::source_member> members;
                    for(::std::size_t actual{};actual<g_runtime.modules.size();++actual)
                    {
                        auto const& entry=g_runtime.modules.index_unchecked(actual);
                        if(!entry.runtime_module||!source_owner->actual_validated_file(actual,current_runtime_generation(),entry.runtime_module))return false;
                        members.push_back({entry.runtime_module,current_runtime_generation(),logical_source_module(actual),
                            {reinterpret_cast<char const*>(entry.module_name.data()),entry.module_name.size()},false});
                    }
                    ::uwvm2::utils::container::std_vector<cp::environment_owner> environments;
                    unsigned mask{};
#if defined(UWVM_IMPORT_ROS1)
                    mask|=1u;
#endif
#if defined(UWVM_IMPORT_ROS2)
                    mask|=2u;
#endif
                    if(capture(members,label,mask,environments)!=cp::status::ready)return false;
                    ros_streams.resize(environments.size());
                    for(::std::size_t i{};i<environments.size();++i)
                    {if(!ros_streams[i].bind(environments[i]))return false;records.push_back(ros_streams[i].record());}
                }
#endif
                ::std::sort(records.begin(),records.end(),[](auto const& a,auto const& b)
                    {return a.kind<b.kind||(a.kind==b.kind&&a.id<b.id);});
                ::uwvm2::runtime::wasm_threads::threaded_environment_sample sample;sample.cursor=cursor;
                auto append=[](void* context,::std::span<::std::byte const> bytes) noexcept
                {try{auto& out=*static_cast<::uwvm2::utils::container::std_vector<::std::byte>*>(context);
                    out.insert(out.end(),bytes.begin(),bytes.end());return true;}catch(...){return false;}};
                if(!ar::encode_stream(ar::native_platform(),records,&sample.archive,append,{},ar::visibility::full))return false;
                state.journal_environments.push_back(::std::move(sample));return true;
            }
            catch(...){return false;}
        }
