#pragma once
#ifndef INSANE_STRING_H
#define INSANE_STRING_H

#include <Insane/Insane.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneException.h>
#include <sstream>

#define USING_NS_INSANE_STR using namespace InsaneIO::Insane::Strings

#define TOSTR(data) InsaneIO::Insane::Strings::StringExtensions::ToString(data)
#define TOCSTR(data) InsaneIO::Insane::Strings::StringExtensions::ToString(data).c_str()

#define NAMEOF(name) InsaneIO::Insane::Strings::StringExtensions::Nameof(#name##s)
#define CNAMEOF(name) InsaneIO::Insane::Strings::StringExtensions::Nameof(#name##s).c_str()

#define NAMEOF_TRIM_GET(name) InsaneIO::Insane::Strings::StringExtensions::NameofTrimGet(#name##s)
#define CNAMEOF_TRIM_GET(name) InsaneIO::Insane::Strings::StringExtensions::NameofTrimGet(#name##s).c_str()

namespace InsaneIO::Insane::Strings
{

	class StringExtensions
	{
	public:
		template <typename ParamType,
			typename = std::void_t<std::enable_if_t<std::is_integral_v<ParamType> ||
			std::is_floating_point_v<ParamType>>>>
			[[nodiscard]] static String ToString(const ParamType& value)
		{
			return std::to_string(value);
		}

		template <typename T>
		[[nodiscard]] static T ToNumber(const std::string& str)
			requires std::is_integral_v<T>
		{
			std::stringstream ss(str);
			T result = 0;
			ss >> result;
			return result;
		}

		template <std::integral T>
		[[nodiscard]] static T BinaryToDecimal(const std::string& str)
		{
			auto number = std::stoull(str, nullptr, 2);
			return (T)number;
		}

		template <std::integral T>
		[[nodiscard]] static T OctalToDecimal(const std::string& str)
		{
			auto number = std::stoull(str, nullptr, 8);
			return (T)number;
		}

		template <std::integral T>
		[[nodiscard]] static T HexadecimalToDecimal(const std::string& str)
		{
			auto number = std::stoull(str, nullptr, 16);
			return (T)number;
		}

		[[nodiscard]] static bool Contains(const String& data, const String& content, const bool& caseSensitive = true, const String& locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String Empty();
		[[nodiscard]] static bool EndsWith(const String& data, const String& suffix, const bool& caseSensitive = true, const String& locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String GetCharUTF8(const String& data, size_t pos);
		[[nodiscard]] static String InsertRepeat(const String& data, size_t distance, const String& toRepeat, bool includeEnd = false);
		[[nodiscard]] static bool IsMatch(const String& input, const String& pattern);
		[[nodiscard]] static bool IsValidUTF8(const std::string& data);
		[[nodiscard]] static String Join(std::vector<String> vec, String connector);
		[[nodiscard]] static size_t LengthUTF8(const String& data);
		[[nodiscard]] static String PadLeft(const String& data, const size_t& totalWidth, const String& padding = SPACE_STRING);
		[[nodiscard]] static String PadLeftUTF8(const String& data, const size_t& totalWidth, const String& padding = SPACE_STRING);
		[[nodiscard]] static String PadRight(const String& data, const size_t& totalWidth, const String& padding = SPACE_STRING);
		[[nodiscard]] static String PadRightUTF8(const String& data, const size_t& totalWidth, const String& padding = SPACE_STRING);
		[[nodiscard]] static String Remove(const String& data, const String& toRemove);
		[[nodiscard]] static String Remove(const String& data, const std::initializer_list<String>& toRemove);
		[[nodiscard]] static String RemoveBlankSpaces(const String& data);
		[[nodiscard]] static String Replace(const String& data, const String& toFind, const String& toReplace);
		[[nodiscard]] static String Replace(const String& data, const std::initializer_list<std::pair<String, String>>& toFindToReplacePairs = {});
		[[nodiscard]] static String ReplaceLastOf(const String& data, const String& toFind, const String& toReplace);
		[[nodiscard]] static String Reverse(const String& data);
		[[nodiscard]] static String ReverseUTF8(const String& data);
		[[nodiscard]] static std::vector<String> Split(const String& data, const String& toFind);
		[[nodiscard]] static bool StartsWith(const String& data, const String& preffix, const bool& caseSensitive = true, const String& locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String SubstringUTF8(const String& data, size_t startIndex, size_t length = SIZE_MAX);
		[[nodiscard]] static String ToCodeLiteral(const String& data, const bool quotationMarkEnclosed = true, const bool& escaped = false);
		[[nodiscard]] static String ToOctalEscapedCodeLiteral(const String& data, const bool quotationMarkEnclosed = true);
		[[nodiscard]] static String ToLower(const String& data, const String& locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String ToUpper(const String& data, const String& locale = DEFAULT_LOCALE_STRING);
		[[nodiscard]] static String ToUnicodeEscapedCodeLiteral(const String& data, const bool quotationMarkEnclosed = true);
		[[nodiscard]] static String Trim(const String& data);
		[[nodiscard]] static String TrimEnd(const String& data);
		[[nodiscard]] static String TrimStart(const String& data);

		[[nodiscard]] static String Trim(const String& data, const String& value);
		[[nodiscard]] static String TrimEnd(const String& data, const String& value);
		[[nodiscard]] static String TrimStart(const String& data, const String& value);
		[[nodiscard]] static String Nameof(const String& name);
		[[nodiscard]] static String NameofTrimGet(const String& name);

	private:
		StringExtensions() = default;
		~StringExtensions() = default;
	};

} // namespace InsaneIO::Insane::Strings

namespace InsaneIO::Insane::Preprocessor
{

} // namespace InsaneIO::Insane::Preprocessor


#endif // !INSANE_STRING_H
