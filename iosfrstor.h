#ifndef IOS_FRSTOR_H
#define IOS_FRSTOR_H

#include <iostream>
#include <string>

// holds a collection of formatting info for a standard stream.
// extract from a stream to copy formatting - inject into a stream to apply formatting.
template<typename charT, typename traits = std::char_traits<charT>>
class basic_iosfmt
{
private: // -- data -- //

	std::streamsize    _precision;
	std::streamsize    _width;
	std::ios::fmtflags _flags;
	charT              _fill;

public: // -- ctor / dtor / asgn -- //

	// creates an uninitialized format object - contents of all format settings is undefined
	basic_iosfmt() noexcept = default;

	// creates a format object by copying the formatting from the specified stream
	explicit basic_iosfmt(std::basic_ios<charT, traits> &stream) { stream >> *this; }

	basic_iosfmt(const basic_iosfmt&) noexcept = default;
	basic_iosfmt(basic_iosfmt&&) noexcept = default;

	basic_iosfmt &operator=(const basic_iosfmt&) noexcept = default;
	basic_iosfmt &operator=(basic_iosfmt&&) noexcept = default;

public: // -- operators -- //

	// applies the the format object's settings to the stream - returns the stream
	friend std::basic_ios<charT, traits> &operator<<(std::basic_ios<charT, traits> &stream, const basic_iosfmt &fmt)
	{
		stream.precision(fmt._precision);
		stream.width(fmt._width);
		stream.flags(fmt._flags);
		stream.fill(fmt._fill);

		return stream;
	}
	// copies the format settings from the stream - returns the stream
	friend std::basic_ios<charT, traits> &operator>>(std::basic_ios<charT, traits> &stream, basic_iosfmt &fmt)
	{
		fmt._precision = stream.precision();
		fmt._width = stream.width();
		fmt._flags = stream.flags();
		fmt._fill = stream.fill();

		return stream;
	}

public: // -- direct manipulation interface -- //

	std::streamsize precision() const noexcept { return _precision; }
	basic_iosfmt &precision(std::streamsize s) noexcept { _precision = s; return *this; }

	std::streamsize width() const noexcept { return _width; }
	basic_iosfmt &width(std::streamsize w) noexcept { _width = w; return *this; }

	std::ios::fmtflags flags() const noexcept { return _flags; }
	basic_iosfmt &flags(std::ios::fmtflags f) noexcept { _flags = f; return *this; }

	charT fill() const noexcept { return _fill; }
	basic_iosfmt &fill(charT c) noexcept { _fill = c; return *this; }
};

// upon construction, binds to a stream and copies its formatting.
// upon destruction, applies the formatting to the stream, effectively restoring its formatting state.
template<typename charT, typename traits = std::char_traits<charT>>
class basic_iosfrstor
{
private: // -- data -- //

	std::basic_ios<charT, traits> &_stream;
	basic_iosfmt<charT, traits>    _fmt;

public: // -- ctor / dtor / asgn -- //

	// creates an iosfrstor object for the provided stream object
	explicit basic_iosfrstor(std::basic_ios<charT, traits> &s) : _stream(s), _fmt(s) {}

	~basic_iosfrstor() { try { _stream << _fmt; } catch (...) {} }

	basic_iosfrstor() = delete;

	basic_iosfrstor(const basic_iosfrstor&) = delete;
	basic_iosfrstor &operator=(const basic_iosfrstor&) = delete;

	basic_iosfrstor(basic_iosfrstor &&other) = delete;
	basic_iosfrstor &operator=(basic_iosfrstor &&other) = delete;

public: // -- interface -- //

	// accesses the formatting object that will be used to restore formatting upon destruction
	basic_iosfmt<charT, traits> &fmt() noexcept { return _fmt; }
};

// -- convenience typedefs for normal stream types -- //

typedef basic_iosfmt<char>    iosfmt;
typedef basic_iosfmt<wchar_t> wiosfmt;

typedef basic_iosfrstor<char>    iosfrstor;
typedef basic_iosfrstor<wchar_t> wiosfrstor;

#endif
