// Included in the signal detail namespace after protected_memory_segment_t.
// A writer owns the links. A fault may interrupt that very writer, so readers
// never lock, allocate, or wait for a writer to finish.
class segment_registry
{
    struct node
    {
        protected_memory_segment_t segment;
        ::std::atomic<node*> next{};
    };
    static_assert(::std::atomic<node*>::is_always_lock_free && ::std::atomic_uint::is_always_lock_free && ::std::atomic_size_t::is_always_lock_free);
    ::std::mutex writers_{};
    ::std::atomic<node*> first_{};
    node* last_{};
    ::std::atomic_uint epoch_{}, readers_[2]{};

    void drain_prior_readers() noexcept
    {
        // Unlink before switching epochs. An old reader either validates its
        // epoch before this switch and is counted, or retries before loading a
        // link. New readers can finish independently of this grace period.
        auto const prior{epoch_.fetch_xor(1u)};
        while(readers_[prior].load() != 0u)
        {
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
            ::fast_io::this_thread::yield();
#endif
        }
    }

public:
    static constexpr ::std::size_t entry_bytes{sizeof(node)};

    template<typename Visit>
    [[nodiscard]] bool any(Visit visit) noexcept
    {
        static_assert(noexcept(visit(::std::declval<protected_memory_segment_t const&>())));
        unsigned epoch{};
        for(;;)
        {
            epoch = epoch_.load();
            readers_[epoch].fetch_add(1u);
            if(epoch_.load() == epoch) { break; }
            readers_[epoch].fetch_sub(1u);
        }
        // Only copy fault evidence here. External callbacks may leave the
        // handler permanently and must run after the read-side pin is released.
        bool found{};
        for(auto* item{first_.load()}; item != nullptr; item = item->next.load())
        {
            if(visit(static_cast<protected_memory_segment_t const&>(item->segment))) { found = true; break; }
        }
        readers_[epoch].fetch_sub(1u);
        return found;
    }

    [[nodiscard]] ::std::size_t size() noexcept
    {
        ::std::size_t count{};
        (void)any([&](auto const&) noexcept { ++count; return false; });
        return count;
    }

    template<typename Install>
    [[nodiscard]] registration_result insert(protected_memory_segment_t segment, Install install) noexcept
    {
        auto item{::uwvm2::utils::allocator::resource::try_initializer_object<node>(
            [&](node* raw) noexcept { return ::new(static_cast<void*>(raw)) node{segment}; })};
        if(!item) { return registration_result::metadata_refused; }
        ::std::lock_guard lock{writers_};
        if(!install()) { return registration_result::handler_unavailable; }
        auto* next{item.release()};
        (last_ == nullptr ? first_ : last_->next).store(next);
        last_ = next;
        return registration_result::registered;
    }

    void erase(::std::byte const* begin, ::std::byte const* end) noexcept
    {
        ::std::lock_guard lock{writers_};
        node* previous{};
        for(auto* item{first_.load()}; item != nullptr; item = item->next.load())
        {
            if(item->segment.begin == begin && item->segment.end == end)
            {
                (previous == nullptr ? first_ : previous->next).store(item->next.load());
                if(last_ == item) { last_ = previous; }
                drain_prior_readers();
                ::uwvm2::utils::allocator::resource::initializer_object_deleter<node>{}(item);
                return;
            }
            previous = item;
        }
    }

    void clear() noexcept
    {
        ::std::lock_guard lock{writers_};
        auto* item{first_.exchange(nullptr)};
        last_ = nullptr;
        if(item == nullptr) { return; }
        drain_prior_readers();
        while(item != nullptr)
        {
            auto* next{item->next.load()};
            ::uwvm2::utils::allocator::resource::initializer_object_deleter<node>{}(item);
            item = next;
        }
    }

#if defined(__linux__) && defined(__aarch64__)
    [[nodiscard]] bool attach_memtag(::std::byte const* reservation, ::std::byte const* base,
                                     ::std::uint8_t const* tags, bool shared) noexcept
    {
        if(base == nullptr || tags == nullptr) { return false; }
        ::std::lock_guard lock{writers_};
        for(auto* item{first_.load()}; item != nullptr; item = item->next.load())
        {
            auto& seg{item->segment};
            if(seg.begin != reservation || seg.length_p == nullptr) { continue; }
            auto const start{reinterpret_cast<::std::uintptr_t>(base)};
            auto const end{reinterpret_cast<::std::uintptr_t>(seg.end)};
            if(start < reinterpret_cast<::std::uintptr_t>(seg.begin) || start >= end ||
               seg.length_p->load(::std::memory_order_acquire) > end - start) { return false; }
            ::std::atomic_ref published{seg.memtag_base};
            if(auto prior{published.load(::std::memory_order_acquire)}; prior != nullptr)
            { return prior == base && seg.memtag_tags == tags && seg.memtag_shared == shared; }
            seg.memtag_tags = tags;
            seg.memtag_shared = shared;
            published.store(base, ::std::memory_order_release);
            return true;
        }
        return false;
    }
#endif
};
inline constinit segment_registry segments{};
