#pragma once
#ifndef INSANE_STRING_H
#define INSANE_STRING_H

#include <Insane/Insane.h>
#define USING_NS_INSANE_STR using namespace Insane::Strings

#define EMPTY_STRING (""s)
#define NULL_STRING ("\0"s)
#define CR_STRING ("\r"s)
#define CARRIAGE_RETURN_STRING ("\r"s)
#define LF_STRING ("\n"s)
#define LINE_FEED_STRING ("\n"s)
#define CRLF_STRING ("\r\n"s)
#define TAB_STRING ("\t"s)
#define VERTICAL_TAB_STRING ("\v"s)
#define FORM_FEED_STRING ("\f"s)
#define QUOTATION_MARK_STRING ("\"")
#define UNDERSCORE_STRING ("_"s)
#define SPACE_STRING (" "s)

#define NULL_CHAR (static_cast<char>(0))
#define LINE_FEED_CHAR (static_cast<char>(10))
#define SPACE_CHAR (static_cast<char>(32))

#define DEFAULT_LOCALE_STRING ("en-US"s)

#ifdef WINDOWS_PLATFORM
#define NEW_LINE_STR "\r\n"
#define NEW_LINE_WSTR L"\r\n"
#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
#define NEW_LINE_STR "\n"
#define NEW_LINE_WSTR L"\n"
#endif

#define DEFAULT_LOCALE_STR "en_US.UTF-8"
#define tostr(x) Insane::Strings::StringExtensions::ToString(x)
#define tocstr(x) Insane::Strings::StringExtensions::ToString(x).c_str()

#define ToFind String
#define ToReplace String

namespace Insane::Strings
{
	class StringExtensions
	{
	public:
		template <typename ParamType,
				  typename = std::void_t<std::enable_if_t<std::is_integral_v<ParamType> ||
														  std::is_floating_point_v<ParamType>>>>
		static String ToString(const ParamType &value)
		{
			return std::to_string(value);
		}

		static String Empty();
		static String TrimStart(const String &data);
		static String TrimEnd(const String &data);
		static String Trim(const String &data);
		static bool IsMatch(const String &input, const String &pattern);
		static String ReplaceLastOf(const String &data, const String &toFind, const String &toReplace);
		static String Replace(const String &data, const String &toFind, const String &toReplace);
		static String Replace(const String &data, const std::initializer_list<std::pair<ToFind, ToReplace>> &pairs);
		static String Remove(const String &data, const String &toRemove);
		static String Remove(const String &data, const std::initializer_list<String> &toRemove);
		static String InsertRepeat(const String &data, size_t distance, const String &toRepeat, bool includeEnd = false);
		static std::vector<String> Split(const String &data, const String &toFind);
		static String ToUpper(const String &data, const String &locale);
		static String ToLower(const String &data, const String &locale);
		static bool StartsWith(const String &data, const String &preffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING);
		static bool EndsWith(const String &data, const String &suffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING);
		static bool Contains(const String &data, const String &content, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING);

		static size_t LengthUTF8(const String &data);
		static String Reverse(const String &data);
		static String ReverseUTF8(const String &data);

		static String PadRight(const String &data, const size_t &totalWidth, const Char &padding = SPACE_CHAR);
		static String PadLeft(const String &data, const size_t &totalWidth, const Char &padding = SPACE_CHAR);
		static String PadRight(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		static String PadLeft(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		static String SubstringUTF8(const String &data, size_t startIndex, size_t length = SIZE_MAX);
		static String GetCharUTF8(const String &data, size_t pos);
		static String PadRightUTF8(const String &data, const size_t &totalWidth, const Char &padding = SPACE_CHAR);
		static String PadLeftUTF8(const String &data, const size_t &totalWidth, const Char &padding = SPACE_CHAR);
		static String PadRightUTF8(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		static String PadLeftUTF8(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);

		static String WideStringToString(const WString &wstr);
		static WString StringToWideString(const String &str);

	private:
		StringExtensions() = default;
		~StringExtensions() = default;
	};

	class Xtring : public String
	{
	public:
		using std::string::string;
		Xtring(const String &str) : String(str) {}
		Xtring SubstringUTF8(size_t startIndex, size_t length = SIZE_MAX) const;
		Xtring GetCharUTF8(size_t utf8pos) const;
		Xtring TrimStart();
		Xtring TrimEnd();
		Xtring Trim();
		Xtring ReplaceLastOf(const String &toFind, const String &toReplace);
		Xtring Replace(const String &toFind, const String &toReplace);
		Xtring Replace(const std::initializer_list<std::pair<ToFind, ToReplace>> &pairs);
		Xtring Remove(const String &toRemove);
		Xtring Remove(const std::initializer_list<String> &toRemove);
		Xtring InsertRepeat(size_t distance, const String &toRepeat, bool includeEnd = false);
		size_t LengthUTF8() const;
		Xtring Reverse();
		Xtring ReverseUTF8();
		Xtring PadRight(const size_t &totalWidth, const Char &padding = SPACE_CHAR);
		Xtring PadLeft(const size_t &totalWidth, const Char &padding = SPACE_CHAR);
		Xtring PadRight(const size_t &totalWidth, const Xtring &padding = SPACE_STRING);
		Xtring PadLeft(const size_t &totalWidth, const Xtring &padding = SPACE_STRING);
		Xtring PadRightUTF8(const size_t &totalWidth, const Char &padding = SPACE_CHAR);
		Xtring PadLeftUTF8(const size_t &totalWidth, const Char &padding = SPACE_CHAR);
		Xtring PadRightUTF8(const size_t &totalWidth, const Xtring &padding = SPACE_STRING);
		Xtring PadLeftUTF8(const size_t &totalWidth, const Xtring &padding = SPACE_STRING);
		Xtring ToUpper(const String &locale);
		Xtring ToLower(const String &locale);
		std::vector<String> Split(const String &toFind) const;
		bool IsMatch(const String &pattern) const;
		bool StartsWith(const String &preffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING) const;
		bool EndsWith(const String &suffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING) const;
		bool Contains(const String &content, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING) const;
	};

#define StringsWideStringToString(data) Insane::Str::Strings::WideStringToString(data)
#define StringsStringToWideString(data) Insane::Str::Strings::StringToWideString(data)
} // namespace Insane::Str

typedef Insane::Strings::Xtring Xtring;

#endif // !INSANE_STRING_H
