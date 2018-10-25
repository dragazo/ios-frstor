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
	std::basic_ios<charT, traits> *_stream;

	// the stored format data
	std::streamsize    _precision;
	std::streamsize    _width;
	std::ios::fmtflags _flags;
	charT              _fill;

private: // -- helpers -- //

	// BEGINS BY DISCARDING THE CURRENT CONTRACT.
	// new format data is then copied for the specified stream object - null allowed for no contract.
	// on success, a new contract is created (unless source was null, which always succeeds but does not create a contract).
	// on failure, no contract is created, this instance is empty, and an exception is thrown.
	inline void form_contract(std::basic_ios<charT, traits> *s)
	{
		// discard current contract
		_stream = nullptr;

		// if source is non-null
		if (s)
		{
			// copy format data
			_precision = s->precision();
			_width = s->width();
			_flags = s->flags();
			_fill = s->fill();

			// mark as valid contract (do this last so if anything above throws it won't try to restore undefined data)
			_stream = s;
		}
	}

	// completes the current contract, but DOES NOT MARK THIS INSTANCE AS EMPTY.
	inline void complete_contract() noexcept
	{
		if (_stream)
		{
			try
			{
				_stream->precision(_precision);
				_stream->width(_width);
				_stream->flags(_flags);
				_stream->fill(_fill);
			}
			// if for some reason those can/do throw, just ignore it
			catch (...) {}
		}
	}

	// transfers ownership of other's contract to us.
	// WARNING: the source is assumed to not be this instance.
	// WARNING: if this instance currently holds a contract, it is discarded
	inline void transfer_contract(basic_iosfrstor &&other) noexcept
	{
		// take on other's contract
		_stream = other._stream;
		_precision = other._precision;
		_width = other._width;
		_flags = other._flags;
		_fill = other._fill;

		// mark other as empty
		other._stream = nullptr;
	}

	// swaps the contracts currently held by this instance and other
	inline void swap_contract(basic_iosfrstor &other) noexcept
	{
		using std::swap;

		swap(_stream, other._stream);
		swap(_precision, other._precision);
		swap(_width, other._width);
		swap(_flags, other._flags);
		swap(_fill, other._fill);
	}

public: // -- ctor / dtor / asgn -- //

	// creates an iosfrstor that has no associated stream object (empty)
	inline basic_iosfrstor() noexcept : _stream(nullptr) {}

	// creates an iosfrstor object for the provided stream object
	inline explicit basic_iosfrstor(std::basic_ios<charT, traits> &s) { form_contract(&s); }

	// upon destruction, completes any stored contract
	inline ~basic_iosfrstor() { complete_contract(); }

	basic_iosfrstor(const basic_iosfrstor&) = delete;
	basic_iosfrstor &operator=(const basic_iosfrstor&) = delete;

	// creates an iosfrstor by transfering the contract from another instance. the moved-from instance is made empty.
	inline basic_iosfrstor(basic_iosfrstor &&other) noexcept { transfer_contract(std::move(other)); }
	// completes the current contract, then transfers other's contract to this instance. the moved-from instance is made empty.
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

	// if this object is non-empty, returns a pointer to the tied stream. otherwise returns null.
	std::basic_ios<charT, traits> *get() const noexcept { return _stream; }

	// releases the current contract from this instance without completing it.
	// this instance is made empty.
	void release() noexcept { _stream = nullptr; }

	// restores the formatting settings of the stream in the current contract without releasing the contract.
	basic_iosfrstor &operator()() noexcept { complete_contract(); return *this; }

	// returns true iff this instance does not currently hold a contract
	bool empty() const noexcept { return !_stream; }

	// returns true iff this instance currently holds a contract
	explicit operator bool() const noexcept { return _stream; }
	// returns true iff this instance does not currently hold a contract
	bool operator!() const noexcept { return !_stream; }

	// swaps the contracts held by the two iosfrstor objects
	friend void swap(basic_iosfrstor &a, basic_iosfrstor &b) noexcept { a.swap_contract(b); }

public: // -- stored-format access -- //

	// gets or sets the stored precision value (new value will be used instead upon restoring formatting)
	std::streamsize precision() const { return _precision; }
	void precision(std::streamsize new_precision) { _precision = new_precision; }

	// gets or sets the stored width value (new value will be used instead upon restoring formatting)
	std::streamsize width() const { return _width; }
	void width(std::streamsize new_width) { _width = new_width; }

	// gets or sets the stored flags value (new value will be used instead upon restoring formatting)
	std::ios::fmtflags flags() const { return _flags; }
	void flags(std::ios::fmtflags new_flags) { _flags = new_flags; }

	// gets or sets the stored fill value (new value will be used instead upon restoring formatting)
	charT fill() const { return _fill; }
	void fill(charT new_fill) { _fill = new_fill; }
};

// convenience typedefs for normal stream types
typedef basic_iosfrstor<char>    iosfrstor;
typedef basic_iosfrstor<wchar_t> wiosfrstor;

#endif
