#ifndef IOS_FRSTOR_H
#define IOS_FRSTOR_H

#include <iostream>
#include <utility>
#include <algorithm>

// acts as a RAII wrapper for saving and restoring formatting states for std::ostream objects.
// at all times, each instance is either empty or holds a restore contract for a given stream object.
// when the iosfrstor object is destroyed, the stream's format data is restored.
// copying is prohibited, but move semantics can be used to transfer restore contract ownership between instances.
// ------------------
// the data stored in the restoring contract includes: flags, width, precision, and fill.
template<typename charT, typename traits = std::char_traits<charT>>
class basic_iosfrstor
{
private: // -- data -- //

	// pointer to the stream to manage
	std::basic_ios<charT, traits> *stream;

	// the stored format data
	std::streamsize    precision;
	std::streamsize    width;
	std::ios::fmtflags flags;
	charT              fill;

private: // -- helpers -- //

	// BEGINS BY DISCARDING THE CURRENT CONTRACT.
	// new format data is then copied for the specified stream object - null allowed for no contract.
	// on success, a new contract is created (unless source was null, which always succeeds but does not create a contract).
	// on failure, no contract is created, this instance is empty, and an exception is thrown.
	inline void form_contract(decltype(stream) _stream)
	{
		// discard current contract
		stream = nullptr;

		// if source is non-null
		if (_stream)
		{
			// copy format data
			precision = _stream->precision();
			width = _stream->width();
			flags = _stream->flags();
			fill = _stream->fill();

			// mark as valid contract (do this last so if anything above throws it won't try to restore undefined data)
			stream = _stream;
		}
	}

	// completes the current contract, but DOES NOT MARK THIS INSTANCE AS EMPTY.
	inline void complete_contract() noexcept
	{
		if (stream)
		{
			try
			{
				stream->precision(precision);
				stream->width(width);
				stream->flags(flags);
				stream->fill(fill);
			}
			catch (...) {}
		}
	}

	// transfers ownership of other's contract to us.
	// WARNING: the source is assumed to not be this instance.
	// WARNING: if this instance currently holds a contract, it is discarded
	inline void transfer_contract(basic_iosfrstor &&other) noexcept
	{
		// take on other's contract
		stream = other.stream;
		precision = other.precision;
		width = other.width;
		flags = other.flags;
		fill = other.fill;

		// mark other as empty
		other.stream = nullptr;
	}

	// swaps the contracts currently held by this instance and other
	inline void swap_contract(basic_iosfrstor &other) noexcept
	{
		using std::swap;

		swap(stream, other.stream);
		swap(precision, other.precision);
		swap(width, other.width);
		swap(flags, other.flags);
		swap(fill, other.fill);
	}

public: // -- ctor / dtor / asgn -- //

	// creates an iosfrstor that has no associated stream object (empty)
	inline basic_iosfrstor() noexcept : stream(nullptr) {}

	// creates an iosfrstor object for the provided stream object
	inline explicit basic_iosfrstor(std::basic_ios<charT, traits> &_stream) { form_contract(&_stream); }

	// upon destruction, completes any stored contract
	inline ~basic_iosfrstor() { complete_contract(); }

	basic_iosfrstor(const basic_iosfrstor&) = delete;
	basic_iosfrstor &operator=(const basic_iosfrstor&) = delete;

	// creates an iosfrstor by transfering the contract from another instance.
	// the moved-from instance is made empty.
	inline basic_iosfrstor(basic_iosfrstor &&other) noexcept { transfer_contract(std::move(other)); }
	// completes the current contract, then transfers other's contract to this instance.
	// the moved-from instance is made empty.
	// in the special case of self-assignment, does nothing.
	inline basic_iosfrstor &operator=(basic_iosfrstor &&other) noexcept
	{
		if (this != &other)
		{
			complete_contract();
			transfer_contract(std::move(other));
		}
	}

public: // -- utility -- //

	// releases the current contract from this instance without completing it.
	// this instance is made empty.
	inline void release() noexcept { stream = nullptr; }

	// restores the formatting settings of the stream in the current contract without releasing the contract.
	inline void rstor() noexcept { complete_contract(); }

	// returns true iff this instance does not currently hold a contract
	inline bool empty() const noexcept { return !stream; }

	// returns true iff this instance currently holds a contract
	inline explicit operator bool() const noexcept { return stream; }

	// swaps the contracts held by the two iosfrstor objects
	inline friend void swap(basic_iosfrstor &a, basic_iosfrstor &b) noexcept { a.swap_contract(b); }
};

// convenience typedefs for normal stream types
typedef basic_iosfrstor<char>    iosfrstor;
typedef basic_iosfrstor<wchar_t> wiosfrstor;

#endif
