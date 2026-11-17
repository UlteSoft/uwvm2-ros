#pragma once

namespace fast_io
{
namespace details
{
inline ::fast_io::intfpos_t posix_seek_impl(int fd, ::fast_io::intfpos_t offset, seekdir s)
{
#if defined(__MSDOS__)
	if constexpr (sizeof(off_t) < sizeof(::fast_io::intfpos_t))
	{
		if (offset < static_cast<::fast_io::intfpos_t>(::std::numeric_limits<off_t>::min()) ||
			offset > static_cast<::fast_io::intfpos_t>(::std::numeric_limits<off_t>::max()))
		{
			throw_posix_error(EOVERFLOW);
		}
	}
	auto ret{noexcept_call(::lseek, fd, static_cast<off_t>(offset), static_cast<int>(s))};
	if (ret == -1)
	{
		throw_posix_error();
	}
	return static_cast<::fast_io::intfpos_t>(static_cast<my_make_unsigned_t<off_t>>(ret));
// Linux32 uses a five-argument split-offset syscall, regardless of libc's
// off_t width. Passing an int64 offset to the three-word lseek ABI shifts
// whence into the wrong register (SEEK_CUR can silently become SEEK_SET).
#elif defined(__linux__) && (defined(__NR_llseek) || defined(__NR__llseek) || defined(__NR_lseek))
#if defined(__NR_llseek) || defined(__NR__llseek)
	if constexpr (sizeof(::std::uintptr_t) == 4u)
	{
		if constexpr (sizeof(::fast_io::intfpos_t) > sizeof(::std::int64_t))
		{
			if (offset < static_cast<::fast_io::intfpos_t>((::std::numeric_limits<::std::int64_t>::min)()) ||
				offset > static_cast<::fast_io::intfpos_t>((::std::numeric_limits<::std::int64_t>::max)()))
			{
				throw_posix_error(EOVERFLOW);
			}
		}
# if defined(__NR_llseek) && defined(__NR__llseek)
		static_assert(__NR_llseek == __NR__llseek);
# endif
# if defined(__NR_llseek)
		constexpr auto syscall_number{__NR_llseek};
# else
		constexpr auto syscall_number{__NR__llseek};
# endif
		::std::int64_t result{};
		auto const bits{static_cast<::std::uint64_t>(offset)};
		auto const ret{system_call<syscall_number, int>(fd,
			static_cast<unsigned long>(bits >> 32u),
			static_cast<unsigned long>(static_cast<::std::uint32_t>(bits)),
			__builtin_addressof(result), static_cast<unsigned int>(s))};
		system_call_throw_error(ret);
		if (ret != 0 || result < 0) { throw_posix_error(EIO); }
		return static_cast<::fast_io::intfpos_t>(result);
	}
	else
#endif
	{
# if defined(__NR_lseek)
		if constexpr (sizeof(::std::ptrdiff_t) < sizeof(::fast_io::intfpos_t))
		{
			if (offset < static_cast<::fast_io::intfpos_t>((::std::numeric_limits<::std::ptrdiff_t>::min)()) ||
				offset > static_cast<::fast_io::intfpos_t>((::std::numeric_limits<::std::ptrdiff_t>::max)()))
			{
				throw_posix_error(EOVERFLOW);
			}
		}
		auto const ret{system_call<__NR_lseek, ::std::ptrdiff_t>(fd, static_cast<::std::ptrdiff_t>(offset), static_cast<int>(s))};
		system_call_throw_error(ret);
		return static_cast<::fast_io::intfpos_t>(ret);
# else
		// No single-register syscall was advertised by this target SDK.
		// The branch above handles Linux32; retain libc for any wider ABI.
		auto const ret{noexcept_call(::lseek, fd, static_cast<off_t>(offset), static_cast<int>(s))};
		if (ret == -1) { throw_posix_error(); }
		return static_cast<::fast_io::intfpos_t>(ret);
# endif
	}
#elif (defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__)) && !defined(__CYGWIN__)
	if constexpr (sizeof(::std::int_least64_t) < sizeof(::fast_io::intfpos_t))
	{
		if (offset < static_cast<::fast_io::intfpos_t>(::std::numeric_limits<::std::int_least64_t>::min()) ||
			offset > static_cast<::fast_io::intfpos_t>(::std::numeric_limits<::std::int_least64_t>::max()))
		{
			throw_posix_error(EOVERFLOW);
		}
	}
	auto ret(::fast_io::noexcept_call(::_lseeki64, fd, static_cast<::std::int_least64_t>(offset), static_cast<int>(s)));
	if (ret == -1) [[unlikely]]
	{
		throw_posix_error();
	}
	return static_cast<::fast_io::intfpos_t>(static_cast<::std::uint_least64_t>(ret));

#else
	if constexpr (sizeof(off_t) < sizeof(::fast_io::intfpos_t))
	{
		if (offset < static_cast<::fast_io::intfpos_t>(::std::numeric_limits<off_t>::min()) ||
			offset > static_cast<::fast_io::intfpos_t>(::std::numeric_limits<off_t>::max()))
		{
			throw_posix_error(EOVERFLOW);
		}
	}
	auto ret(::fast_io::noexcept_call(::lseek, fd, static_cast<off_t>(offset), static_cast<int>(s)));
	if (ret == -1) [[unlikely]]
	{
		throw_posix_error();
	}
	return static_cast<::fast_io::intfpos_t>(static_cast<::std::uint_least64_t>(ret));
#endif
}
} // namespace details

template <::fast_io::posix_family family, ::std::integral char_type>
inline ::fast_io::intfpos_t io_stream_seek_bytes_define(::fast_io::basic_posix_family_io_observer<family, char_type> bpiob,
														::fast_io::intfpos_t off, ::fast_io::seekdir sdir)
{
	return ::fast_io::details::posix_seek_impl(bpiob.fd, off, sdir);
}

} // namespace fast_io
