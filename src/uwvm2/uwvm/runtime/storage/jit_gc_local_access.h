// Native compiler layout contract. Guest tokens remain opaque integers.
// Only a retained store's actual membership index authenticates a local header.
        static_assert(::std::is_standard_layout_v<object>);
        static_assert(sizeof(::std::atomic_flag)==1uz && alignof(::std::atomic_flag)==1uz);
        static_assert(sizeof(::std::atomic<object*>)==sizeof(object*));
        static_assert(alignof(::std::atomic<object*>)==alignof(object*));
        static_assert(sizeof(gc_type::composite_kind)==4uz);
        // Generated numeric accesses may use this genuine tail alignment.
        // The byte tail of a packed array obeys the same allocation geometry.
        static_assert(allocation_alignment % alignof(gc_object_value) == 0uz);
        static_assert(object_value_offset % alignof(gc_object_value) == 0uz);
        inline static constexpr ::std::size_t jit_local_token_offset{offsetof(object,token)};
        inline static constexpr ::std::size_t jit_local_owner_offset{offsetof(object,owner)};
        inline static constexpr ::std::size_t jit_local_type_offset{offsetof(object,type_index)};
        inline static constexpr ::std::size_t jit_local_kind_offset{offsetof(object,kind)};
        inline static constexpr ::std::size_t jit_local_length_offset{offsetof(object,length)};
        inline static constexpr ::std::size_t jit_local_values_offset{offsetof(object,values)+offsetof(object_values_view,pointer)};
        inline static constexpr ::std::size_t jit_local_lock_offset{offsetof(object,mutation_lock)};
        inline static constexpr ::std::size_t jit_local_bucket_mask{bucket_count-1uz};
        // Both words are immutable for the retained store lifetime. Collection
        // may change bucket heads only with the existing complete-reader drain.
        // No individual object pointer is returned or cached across safepoints.
        void jit_local_index_domain(::std::uintptr_t (&words)[2]) const noexcept
        {
            if(!valid_||!membership_buckets_) { words[0]=words[1]=0u;return; }
            words[0]=reinterpret_cast<::std::uintptr_t>(this);
            words[1]=reinterpret_cast<::std::uintptr_t>(membership_buckets_.get());
        }
