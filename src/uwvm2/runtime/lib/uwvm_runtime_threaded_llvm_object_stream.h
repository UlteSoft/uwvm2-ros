// Include inside ThreadedLlvmChild (or a test owner) after LLVM stream headers.
// The configured code budget is an upper bound, not an eager allocation request.
// Grow exact owned arrays only as bytes arrive. Overflow is sticky and is checked
// before any allocation/copy; an invalid stream is never passed to the loader.
        class bounded_object_stream final:public ::llvm::raw_pwrite_stream
        {
            ::std::unique_ptr<char[]> bytes_{};
            ::std::size_t capacity_{},maximum_{},used_{};
            ::std::uint64_t position_{};
            bool overflow_{};
            void grow(::std::size_t required)
            {
                if(required<=capacity_)return;
                auto next=capacity_==0u?(::std::min)(maximum_,::std::size_t{4096u}):capacity_;
                while(next<required)
                    next=next>maximum_-next?maximum_:next+next;
                auto replacement=::std::make_unique_for_overwrite<char[]>(next);
                if(used_!=0u)::std::memcpy(replacement.get(),bytes_.get(),used_);
                bytes_=::std::move(replacement);capacity_=next;
            }
            void write_impl(char const* data,::std::size_t size) override
            {
                if(!overflow_)
                {
                    if(position_>maximum_||size>maximum_-static_cast<::std::size_t>(position_))overflow_=true;
                    else if(size!=0u)
                    {
                        auto const end=static_cast<::std::size_t>(position_)+size;
                        grow(end);
                        ::std::memcpy(bytes_.get()+static_cast<::std::size_t>(position_),data,size);
                        used_=(::std::max)(used_,end);
                    }
                }
                if(size>UINT64_MAX-position_){position_=UINT64_MAX;overflow_=true;}else position_+=size;
            }
            void pwrite_impl(char const* data,::std::size_t size,::std::uint64_t offset) override
            {
                if(overflow_)return;
                if(offset>used_||size>used_-static_cast<::std::size_t>(offset)){overflow_=true;return;}
                if(size!=0u)::std::memcpy(bytes_.get()+static_cast<::std::size_t>(offset),data,size);
            }
            [[nodiscard]] ::std::uint64_t current_pos() const override{return position_;}
        public:
            explicit bounded_object_stream(::std::size_t maximum):maximum_{maximum}{SetUnbuffered();}
            ~bounded_object_stream() override=default;
            [[nodiscard]] bool valid() const noexcept{return !overflow_&&used_!=0u;}
            [[nodiscard]] ::llvm::StringRef bytes() const noexcept{return {bytes_.get(),used_};}
        };
