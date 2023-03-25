#pragma once
#ifndef INSANE_STRING_H
#define INSANE_STRING_H

#include <Insane/Insane.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneException.h>
#include <concepts>

#define USING_NS_INSANE_STR using namespace Insane::Strings

#define EMOJI_UPWARDS_BUTTON_STRING ("🔼"s)
#define EMOJI_DOWNWARDS_BUTTON_STRING ("🔽"s)
#define EMOJI_SQUARE_VS_STRING ("🆚"s)
#define EMOJI_RED_TRIANGLE_POINTED_UP_STRING ("🔺"s)
#define EMOJI_RED_TRIANGLE_POINTED_DOWN_STRING ("🔻"s)

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
#define TRUE_LITERAL_STRING ("true"s)
#define FALSE_LITERAL_STRING ("false"s)

#define EMPTY_CHAR_ARRAY ("")
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

#define URL_ENCODED_PLUS_SIGN_STRING ("%2B"s)
#define URL_ENCODED_SLASH_STRING ("%2F"s)
#define URL_ENCODED_EQUAL_SIGN_STRING ("%3D"s)
#define PLUS_SIGN_STRING ("+"s)
#define MINUS_SIGN_STRING ("-"s)
#define SLASH_STRING ("/"s)
#define UNDERSCORE_STRING ("_"s)
#define EQUAL_SIGN_STRING ("="s)

#define URL_ENCODED_PLUS_SIGN_CHAR_ARRAY ("%2B")
#define URL_ENCODED_SLASH_CHAR_ARRAY ("%2F")
#define URL_ENCODED_EQUAL_SIGN_CHAR_ARRAY ("%3D")
#define PLUS_SIGN_CHAR ('+')
#define MINUS_SIGN_CHAR ('-')
#define SLASH_CHAR ('/')
#define UNDERSCORE_CHAR ('_')
#define EQUAL_SIGN_CHAR ('=')

#define DEFAULT_LOCALE_STRING ("en_US"s)

#ifdef WINDOWS_PLATFORM
#define NEW_LINE_STR "\r\n"
#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
#define NEW_LINE_STR "\n"
#endif

#define DEFAULT_LOCALE_STR "en_US.UTF-8"
#define TOSTR(data) Insane::Strings::StringExtensions::ToString(data)
#define TOCSTR(data) Insane::Strings::StringExtensions::ToString(data).c_str()

#define NAMEOF(name) Insane::Preprocessor::Nameof(#name##s)
#define CNAMEOF(name) Insane::Preprocessor::Nameof(#name##s).c_str()

#define StringsWideStringToString(data) Insane::Strings::StringExtensions::WideStringToString(data)
#define StringsStringToWideString(data) Insane::Strings::StringExtensions::StringToWideString(data)

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
		[[nodiscard]] static T BinaryToDecimal(const std::string &str)
		{
			auto number = std::stoull(str, nullptr, 2);
			return (T)number;
		}

		template <std::integral T>
		[[nodiscard]] static T OctalToDecimal(const std::string &str)
		{
			auto number = std::stoull(str, nullptr, 8);
			return (T)number;
		}

		template <std::integral T>
		[[nodiscard]] static T HexadecimalToDecimal(const std::string &str)
		{
			auto number = std::stoull(str, nullptr, 16);
			return (T)number;
		}

		[[nodiscard]] static bool Contains(const String &data, const String &content, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String Empty();
		[[nodiscard]] static bool EndsWith(const String &data, const String &suffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String GetCharUTF8(const String &data, size_t pos);
		[[nodiscard]] static String InsertRepeat(const String &data, size_t distance, const String &toRepeat, bool includeEnd = false);
		[[nodiscard]] static bool IsMatch(const String &input, const String &pattern);
		[[nodiscard]] static size_t LengthUTF8(const String &data);
		[[nodiscard]] static String PadLeft(const String &data, const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		[[nodiscard]] static String PadLeft(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		[[nodiscard]] static String PadLeftUTF8(const String &data, const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		[[nodiscard]] static String PadLeftUTF8(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		[[nodiscard]] static String PadRight(const String &data, const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		[[nodiscard]] static String PadRight(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		[[nodiscard]] static String PadRightUTF8(const String &data, const size_t &totalWidth, const SignedChar &padding = SPACE_CHAR);
		[[nodiscard]] static String PadRightUTF8(const String &data, const size_t &totalWidth, const String &padding = SPACE_STRING);
		[[nodiscard]] static String Remove(const String &data, const String &toRemove);
		[[nodiscard]] static String Remove(const String &data, const std::initializer_list<String> &toRemove);
		[[nodiscard]] static String RemoveBlankSpaces(const String &data);
		[[nodiscard]] static String Replace(const String &data, const String &toFind, const String &toReplace);
		[[nodiscard]] static String Replace(const String &data, const std::initializer_list<std::pair<String, String>> &toFindToReplacePairs = {});
		[[nodiscard]] static String ReplaceLastOf(const String &data, const String &toFind, const String &toReplace);
		[[nodiscard]] static String Reverse(const String &data);
		[[nodiscard]] static String ReverseUTF8(const String &data);
		[[nodiscard]] static std::vector<String> Split(const String &data, const String &toFind);
		[[nodiscard]] static bool StartsWith(const String &data, const String &preffix, const bool &caseSensitive = true, const String &locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String SubstringUTF8(const String &data, size_t startIndex, size_t length = SIZE_MAX);
		[[nodiscard]] static String ToLower(const String &data, const String &locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String ToUpper(const String &data, const String &locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String Trim(const String &data);
		[[nodiscard]] static String TrimEnd(const String &data);
		[[nodiscard]] static String TrimStart(const String &data);

		[[nodiscard]] static String WideStringToString(const WString &wstr);
		[[nodiscard]] static WString StringToWideString(const String &str);

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

namespace Insane::Preprocessor
{
	[[nodiscard]] static inline String Nameof(const String &name)
	{
		std::smatch groups;
		String str = Insane::Strings::StringExtensions::Trim(name);
		if (std::regex_match(str, groups, std::regex(R"(^&?([_a-zA-Z]\w*(->|\.|::))*([_a-zA-Z]\w*)$)")))
		{
			if (groups.size() == 4)
			{
				return groups[3];
			}
		}
		throw Insane::Exception::ArgumentException("nameof(" + name + "). Invalid identifier.");
	}
} // namespace Insane::Preprocessor

typedef Insane::Strings::Xtring Xtring;

#endif // !INSANE_STRING_H
