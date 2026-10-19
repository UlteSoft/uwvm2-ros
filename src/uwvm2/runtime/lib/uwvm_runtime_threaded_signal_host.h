// SPDX-License-Identifier: Apache-2.0
// Include INSIDE the actual runtime_threaded_profile_bridge class, before its
// lifecycle definitions. Only that bridge can construct/use this Native owner.
private:
    [[nodiscard]] static bool signal_policy_admitted(threaded_signal_policy const& value) noexcept
    {
        if(value.read_fd==0u&&value.cookie==0u&&value.maximum_frames==0u&&value.maximum_pipe_bytes==0u)return true;
        return value.read_fd>=3u&&value.read_fd<=INT32_MAX&&value.cookie!=0u&&
            value.maximum_frames!=0u&&value.maximum_frames<=1048576u&&
            value.maximum_pipe_bytes>=32u&&value.maximum_pipe_bytes<=1048576u;
    }
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
    // Additional pre-effect census for this optional private map. The actual
    // interpreter/LLVM complete import admission still independently runs before
    // Guest entry; this check never widens those ten allowed WASI operations.
    [[nodiscard]] static bool signal_source_census(admission const& actual) noexcept
    {
        if(!actual_source_providers(actual))return false;
        auto const& imports=actual.source_module().imported_function_vec_storage;
        for(::std::size_t i{};i<imports.size();++i)
        {
            auto const& row=imports.index_unchecked(i);
            builtin_kind member{};
            if(builtin_loader::actual_member(row.target.local_imported.module_ptr,member))continue;
            ::uwvm2::uwvm::runtime::full::builtin_wasip1_function_data wasi{};
            if(!actual.source_owner()->actual_builtin_wasip1_function(actual.source_module_id(),
                actual.source_validation_epoch(),&actual.source_module(),i,wasi))return false;
            if(wasi.exits_guest)continue; // real separate Group proc_exit route
            auto const information=row.target.local_imported.module_ptr->get_function_information_from_index(wasi.function_index);
            if(!information.successed)return false;
            auto const name=information.function_name;
            if(name!=u8"args_sizes_get"&&name!=u8"args_get"&&name!=u8"environ_sizes_get"&&name!=u8"environ_get"&&
               name!=u8"clock_time_get"&&name!=u8"clock_res_get"&&name!=u8"sched_yield"&&
               name!=u8"fd_read"&&name!=u8"fd_write"&&name!=u8"poll_oneoff")return false;
        }
        return true;
    }
    class signal_owner final
    {
        friend class runtime_threaded_profile_bridge;
        using environment=::uwvm2::uwvm::imported::wasi::wasip1::storage::wasip1_env_type;
        using descriptor=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_t;
        using descriptor_ref=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_ref_t;
        using descriptor_ptr=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_unique_ptr_t;
        using no_construct=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_no_construct_t;
        ::std::mutex mutex_;
        ::std::shared_ptr<admission const> actual_;
        environment* environment_{};
        threaded_signal_report facts_{};
        descriptor_ref read_{no_construct{}};
        descriptor* map_cell_{};
        ::fast_io::native_file writer_;
        bool closing_{};
        int terminal_{};
        ::std::array<::std::byte,32> frame_{};
        // Native control-budget reservation, not a claim of exact allocator RSS.
        static constexpr ::std::size_t bookkeeping_bytes=4096u;
        static void put(::std::byte* out,::std::uint64_t value,unsigned width) noexcept
        {for(unsigned i{};i<width;++i)out[i]=static_cast<::std::byte>(value>>(8u*i));}
        void note(int value) noexcept
        {if(value){if(!facts_.first_error)facts_.first_error=value;facts_.last_error=value;}}
        struct map_exclusion final
        {
            ::uwvm2::utils::mutex::rwlock_t* actual{};
            explicit map_exclusion(::uwvm2::utils::mutex::rwlock_t& value) noexcept
            {
                unsigned expected{};
                if(value.bits.compare_exchange_strong(expected,::uwvm2::utils::mutex::rwlock_writer_mask(),
                    ::std::memory_order_acquire,::std::memory_order_relaxed))actual=&value;
            }
            ~map_exclusion(){if(actual)actual->bits.fetch_and(~::uwvm2::utils::mutex::rwlock_writer_mask(),::std::memory_order_release);}
            map_exclusion(map_exclusion const&)=delete;
        };
        signal_owner(::std::shared_ptr<admission const> actual,environment& env,threaded_signal_policy const& selected) noexcept
            :actual_{::std::move(actual)},environment_{&env}
        {
            facts_.source_generation=actual_->source_generation();facts_.cookie=selected.cookie;
            facts_.read_fd=selected.read_fd;facts_.maximum_frames=selected.maximum_frames;
        }
        [[nodiscard]] int install(threaded_signal_policy const& selected)
        {
            // Caller holds actual cold maintenance and canonical Source admission;
            // no compiler, initial-main or child has been submitted yet.
            auto& table=environment_->fd_storage;
            {
                map_exclusion map{table.fds_rwlock};if(!map.actual)return EBUSY;
                if(selected.read_fd<table.opens.size()||table.renumber_map.find(static_cast<::std::int32_t>(selected.read_fd))!=table.renumber_map.end())return EEXIST;
                if(table.fd_limit==0u||table.opens.size()>=table.fd_limit||
                    table.renumber_map.size()>=table.fd_limit-table.opens.size())return EMFILE;
                // All map node/descriptor/reference storage precedes pipe2. A
                // throwing map allocation unwinds to the actual cold admission
                // rollback; install deliberately is not noexcept. The
                // placeholder is not observable by any Guest before admission.
                descriptor_ptr cell{};map_cell_=cell.fd_p;
                auto const inserted=table.renumber_map.emplace(static_cast<::std::int32_t>(selected.read_fd),::std::move(cell));
                if(!inserted.second){map_cell_=nullptr;return EEXIST;}
                read_=map_cell_->wasi_fd;facts_.installed=true;
            }
            int raw[2]{-1,-1};
            if(::syscall(SYS_pipe2,raw,O_NONBLOCK|O_CLOEXEC)!=0){note(errno);return errno;}
            ::fast_io::native_file reading{raw[0]};writer_=::fast_io::native_file{raw[1]};
            int probe_error{};
            auto const read_flags=::fcntl(reading.native_handle(),F_GETFL);if(read_flags<0){probe_error=errno;note(probe_error);}
            auto const write_flags=::fcntl(writer_.native_handle(),F_GETFL);if(write_flags<0){probe_error=errno;note(probe_error);}
            auto const read_fd_flags=::fcntl(reading.native_handle(),F_GETFD);if(read_fd_flags<0){probe_error=errno;note(probe_error);}
            auto const write_fd_flags=::fcntl(writer_.native_handle(),F_GETFD);if(write_fd_flags<0){probe_error=errno;note(probe_error);}
            // Same selected Linux pipe-class operation as the real existing
            // threaded WASI fd_pin; this is not a handwritten WASI ABI.
            constexpr int linux_getpipe_size{1032};
#ifdef F_GETPIPE_SZ
            static_assert(F_GETPIPE_SZ==linux_getpipe_size);
#endif
            auto const capacity=::fcntl(reading.native_handle(),linux_getpipe_size);if(capacity<0){probe_error=errno;note(probe_error);}
            // Capture the read OFD even on a subsequent validation failure so
            // explicit rollback observes and closes every acquired owner once.
            auto& storage=read_.ptr->wasi_fd_storage;
            storage.reset_type(::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e::file);
            storage.storage.file_fd=::std::move(reading);
            if(probe_error)return probe_error;
            if((read_flags&O_NONBLOCK)==0||(write_flags&O_NONBLOCK)==0||
                (read_flags&O_ACCMODE)!=O_RDONLY||(write_flags&O_ACCMODE)!=O_WRONLY||
                (read_fd_flags&FD_CLOEXEC)==0||(write_fd_flags&FD_CLOEXEC)==0||
                capacity<32||static_cast<::std::uint64_t>(capacity)>selected.maximum_pipe_bytes)
            {note(EINVAL);return EINVAL;}
            facts_.pipe_capacity=static_cast<::std::uint64_t>(capacity);
            using right=::uwvm2::imported::wasi::wasip1::abi::rights_t;
            map_cell_->rights_base=right::right_fd_read|right::right_poll_fd_readwrite;
            map_cell_->rights_inherit={};
            frame_[0]=::std::byte{'U'};frame_[1]=::std::byte{'R'};frame_[2]=::std::byte{'S'};frame_[3]=::std::byte{'1'};
            put(frame_.data()+4,1u,2);put(frame_.data()+6,32u,2);put(frame_.data()+8,facts_.cookie,8);
            return 0;
        }
        [[nodiscard]] int remove_map() noexcept
        {
            if(!facts_.installed)return 0;
            auto& table=environment_->fd_storage;map_exclusion map{table.fds_rwlock};if(!map.actual)return EBUSY;
            auto found=table.renumber_map.find(static_cast<::std::int32_t>(facts_.read_fd));
            if(found==table.renumber_map.end()||found->second.fd_p!=map_cell_||
                !map_cell_||map_cell_->wasi_fd.ptr!=read_.ptr)return EIO;
            auto const locked=::pthread_mutex_trylock(::std::addressof(map_cell_->fd_mutex.mutex));
            if(locked)return locked;
            auto const unlocked=::pthread_mutex_unlock(::std::addressof(map_cell_->fd_mutex.mutex));if(unlocked)return unlocked;
            // Actual writer exclusion prevents a new map→descriptor handoff.
            // Physical Group join has already ended every previous handoff.
            table.renumber_map.erase(found);map_cell_=nullptr;facts_.installed=false;return 0;
        }
        static int close_once(::fast_io::native_file& owner,bool&attempted,int&fact) noexcept
        {
            auto const fd=owner.release();if(fd<0)return 0;
            attempted=true;auto const result=::close(fd);fact=result==0?0:errno;return fact;
        }
        // Only bridge calls this from its never-launched rollback or AFTER its
        // real join_until result. There is no public bool/callback cleanup proof.
        [[nodiscard]] int retire() noexcept
        {
            ::std::unique_lock lock{mutex_,::std::try_to_lock};if(!lock.owns_lock())return EBUSY;
            if(facts_.retired)return terminal_?terminal_:(facts_.write_close_error?facts_.write_close_error:facts_.read_close_error);
            closing_=true;auto const removed=remove_map();if(removed){note(removed);return removed;}
            if(read_.ptr&&read_.ptr->refcount.load(::std::memory_order_acquire)!=1u){note(EBUSY);return EBUSY;}
            // Preserve read endpoint until writer release: no SIGPIPE window.
            note(close_once(writer_,facts_.write_close_attempted,facts_.write_close_error));
            if(read_.ptr)
            {
                auto& storage=read_.ptr->wasi_fd_storage;
                using type=::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_type_e;
                if(storage.type==type::file)note(close_once(storage.storage.file_fd,facts_.read_close_attempted,facts_.read_close_error));
                read_.reset();
            }
            facts_.retired=true;actual_.reset();environment_=nullptr;
            return terminal_?terminal_:(facts_.write_close_error?facts_.write_close_error:facts_.read_close_error);
        }
        struct push_result {int error{};bool terminal{};};
        [[nodiscard]] push_result push(threaded_signal_event event) noexcept
        {
            ::std::unique_lock lock{mutex_,::std::try_to_lock};if(!lock.owns_lock())return {EBUSY,false};
            if(closing_||facts_.retired)return {ECANCELED,false};
            if(facts_.poisoned)return {terminal_?terminal_:EIO,true};
            auto const value=static_cast<::std::uint32_t>(event);if(value!=1u&&value!=2u)return {EINVAL,false};
            if(facts_.sent_frames>=facts_.maximum_frames||facts_.last_sequence==UINT64_MAX||facts_.native_write_attempts==UINT64_MAX)
            {note(EOVERFLOW);facts_.poisoned=true;terminal_=EOVERFLOW;return {terminal_,true};}
            put(frame_.data()+16,facts_.last_sequence+1u,8);put(frame_.data()+24,value,4);put(frame_.data()+28,0u,4);
            // Known genuine Linux nonblocking pipe; 32 <= PIPE_BUF. One atomic
            // record write, never fast_io write-all/replay after unknown progress.
            ++facts_.native_write_attempts;facts_.native_write_error=0;facts_.partial_write_bytes=0;
            auto const count=::write(writer_.native_handle(),frame_.data(),frame_.size());
            if(count==static_cast<::ssize_t>(frame_.size())){++facts_.sent_frames;++facts_.last_sequence;return {0,false};}
            if(count<0){facts_.native_write_error=errno;note(errno);if(errno==EAGAIN)return {EAGAIN,false};}
            else{facts_.partial_write_bytes=static_cast<::std::size_t>(count);note(EIO);}
            facts_.poisoned=true;terminal_=facts_.native_write_error?facts_.native_write_error:EIO;return {terminal_,true};
        }
        [[nodiscard]] int snapshot(threaded_signal_report&out) noexcept
        {::std::unique_lock lock{mutex_,::std::try_to_lock};if(!lock.owns_lock())return EBUSY;out=facts_;return 0;}
    public:
        ~signal_owner(){if(facts_.installed||(!facts_.retired&&(writer_.native_handle()>=0||read_.ptr)))::std::terminate();}
    };
    static_assert(sizeof(signal_owner)<=4096u,"Actual fixed bookkeeping reservation must cover this owner");
#else
    class signal_owner;
#endif
