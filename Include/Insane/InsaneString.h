#pragma once
#ifndef INSANE_STRING_H
#define INSANE_STRING_H

#include <Insane/Insane.h>
#include <concepts>

#define USING_NS_INSANE_STR using namespace Insane::Strings

#define EMPTY_STRING (""s)
#define NULL_STRING ("\000"s)
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
#define BACKSLASH_STRING ("\\"s)
#define QUESTION_MARK_STRING ("\?"s)

#define EMPTY_CHAR ('')
#define NULL_CHAR ('\000')
#define CR_CHAR ('\r')
#define CARRIAGE_RETURN_CHAR ('\r')
#define LF_CHAR ('\n')
#define LINE_FEED_CHAR ('\n')
#define CRLF_CHAR_ARRAY ("\r\n")
#define TAB_CHAR ('\t')
#define VERTICAL_TAB_CHAR ('\v')
#define FORM_FEED_CHAR ('\f')
#define QUOTATION_MARK_CHAR ('\"')
#define UNDERSCORE_CHAR ('_')
#define SPACE_CHAR (' ')
#define BACKSLASH_CHAR ('\\')
#define QUESTION_MARK_CHAR ('\?')



#define DEFAULT_LOCALE_STRING ("en_US"s)

#ifdef WINDOWS_PLATFORM
#define NEW_LINE_STR "\r\n"
#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
#define NEW_LINE_STR "\n"
#endif

#define DEFAULT_LOCALE_STR "en_US.UTF-8"
#define TO_STR(data) Insane::Strings::StringExtensions::ToString(data)
#define TO_CSTR(data) Insane::Strings::StringExtensions::ToString(data).c_str()

namespace Insane::Strings
{
	class IntegralExtensions
	{
	public:
		static std::string ToOctal(std::integral auto value)
		{
			const auto size = sizeof(value) * 8;
			std::bitset<size> bits(value);
			auto uc = bits.to_ullong();
			int width = static_cast<int>(std::ceil(static_cast<double>(size) / 3));
			std::ostringstream oss;
			oss << std::oct << std::setfill('0') << std::setw(width) << uc;
			return oss.str();
		}

		static std::string ToHexadecimal(std::integral auto value)
		{
			const auto size = sizeof(value) * 8;
			std::bitset<size> bits(value);
			auto uc = bits.to_ullong();
			int width = static_cast<int>(std::ceil(static_cast<double>(size) / 4));
			std::ostringstream oss;
			oss << std::hex << std::setfill('0') << std::setw(width) << uc;
			return oss.str();
		}

		static std::string ToBinary(std::integral auto value)
		{
			const auto size = sizeof(value) * 8;
			std::bitset<size> bits(value);
			std::ostringstream oss;
			oss << bits;
			return oss.str();
		}

	private:
	};

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

		template <std::integral T>
		static T BinaryToDecimal(const std::string &str)
		{
			auto number = std::stoull(str, nullptr, 2);
			return (T)number;
		}

		template <std::integral T>
		static T OctalToDecimal(const std::string &str)
		{
			auto number = std::stoull(str, nullptr, 8);
			return (T)number;
		}

		template <std::integral T>
		static T HexadecimalToDecimal(const std::string &str)
		{
			auto number = std::stoull(str, nullptr, 16);
			return (T)number;
		}

		static bool Contains(const String &data, const String &content, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING);
		static String Empty();
		static bool EndsWith(const String &data, const String &suffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING);
		static String GetCharUTF8(const String &data, size_t pos);
		static String InsertRepeat(const String &data, size_t distance, const String &toRepeat, bool includeEnd = false);
		static bool IsMatch(const String &input, const String &pattern);
		static size_t LengthUTF8(const String &data);
		static String PadLeft(const String &data, const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		static String PadLeft(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		static String PadLeftUTF8(const String &data, const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		static String PadLeftUTF8(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		static String PadRight(const String &data, const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		static String PadRight(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		static String PadRightUTF8(const String &data, const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		static String PadRightUTF8(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		static String Remove(const String &data, const String &toRemove);
		static String Remove(const String &data, const std::initializer_list<String> &toRemove);
		static String Replace(const String &data, const String &toFind, const String &toReplace);
		static String Replace(const String &data, const std::initializer_list<std::pair<String, String>> &toFindToReplacePairs = {});
		static String ReplaceLastOf(const String &data, const String &toFind, const String &toReplace);
		static String Reverse(const String &data);
		static String ReverseUTF8(const String &data);
		static std::vector<String> Split(const String &data, const String &toFind);
		static bool StartsWith(const String &data, const String &preffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING);
		static String SubstringUTF8(const String &data, size_t startIndex, size_t length = SIZE_MAX);
		static String ToLower(const String &data, const String &locale);
		static String ToUpper(const String &data, const String &locale);
		static String Trim(const String &data);
		static String TrimEnd(const String &data);
		static String TrimStart(const String &data);

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
		Xtring(const String &str);
		bool Contains(const String &content, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING) const;
		bool EndsWith(const String &suffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING) const;
		Xtring GetCharUTF8(size_t utf8pos) const;
		Xtring InsertRepeat(size_t distance, const String &toRepeat, bool includeEnd = false);
		bool IsMatch(const String &pattern) const;
		size_t LengthUTF8() const;
		Xtring PadLeft(const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		Xtring PadLeft(const size_t &totalWidth, const Xtring &padding = SPACE_STRING);
		Xtring PadLeftUTF8(const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		Xtring PadLeftUTF8(const size_t &totalWidth, const Xtring &padding = SPACE_STRING);
		Xtring PadRight(const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		Xtring PadRight(const size_t &totalWidth, const Xtring &padding = SPACE_STRING);
		Xtring PadRightUTF8(const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		Xtring PadRightUTF8(const size_t &totalWidth, const Xtring &padding = SPACE_STRING);
		Xtring Remove(const String &toRemove);
		Xtring Remove(const std::initializer_list<String> &toRemove);
		Xtring Replace(const String &toFind, const String &toReplace);
		Xtring Replace(const std::initializer_list<std::pair<String, String>> &toFindToReplacePairs);
		Xtring ReplaceLastOf(const String &toFind, const String &toReplace);
		Xtring Reverse();
		Xtring ReverseUTF8();
		std::vector<String> Split(const String &toFind) const;
		bool StartsWith(const String &preffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING) const;
		Xtring SubstringUTF8(size_t startIndex, size_t length = SIZE_MAX) const;
		Xtring ToLower(const String &locale);
		Xtring ToUpper(const String &locale);
		Xtring Trim();
		Xtring TrimEnd();
		Xtring TrimStart();
	};

} // namespace Insane::Strings

#define StringsWideStringToString(data) Insane::Strings::StringExtensions::WideStringToString(data)
#define StringsStringToWideString(data) Insane::Strings::StringExtensions::StringToWideString(data)
typedef Insane::Strings::Xtring Xtring;

#endif // !INSANE_STRING_H
