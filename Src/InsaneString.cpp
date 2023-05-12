#include <Insane/InsaneString.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneCore.h>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <unicode/ucsdet.h>
#include <unicode/regex.h>


// ███ Internal ███

std::string InternalToOctal(const std::integral auto &value)
{
	const auto size = sizeof(value) * 8;
	std::bitset<size> bits(value);
	auto uc = bits.to_ullong();
	int width = static_cast<int>(std::ceil(static_cast<double>(size) / 3));
	std::ostringstream oss;
	oss << std::oct << std::setfill('0') << std::setw(width) << uc;
	return oss.str();
}

std::string InternalToHexadecimal(const std::integral auto &value)
{
	const auto size = sizeof(value) * 8;
	std::bitset<size> bits(value);
	auto uc = bits.to_ullong();
	int width = static_cast<int>(std::ceil(static_cast<double>(size) / 4));
	std::ostringstream oss;
	oss << std::hex << std::setfill('0') << std::setw(width) << uc;
	return oss.str();
}

// ███ StringExtensions ███
String InsaneIO::Insane::Strings::StringExtensions::RemoveBlankSpaces(const String &data)
{
	return StringExtensions::Remove(data, {CARRIAGE_RETURN_STRING, LINE_FEED_STRING, VERTICAL_TAB_STRING, FORM_FEED_STRING, TAB_STRING, SPACE_STRING, NULL_STRING});
}

String InsaneIO::Insane::Strings::StringExtensions::Empty()
{
	return EMPTY_STRING;
}

bool InsaneIO::Insane::Strings::StringExtensions::IsMatch(const String &input, const String &pattern)
{
	//std::regex regex(pattern, std::regex_constants::ECMAScript );
	//return std::regex_match(input, regex);
	UErrorCode status = U_ZERO_ERROR;
	icu::UnicodeString uniPattern = icu::UnicodeString::fromUTF8(icu::StringPiece(pattern.data(), static_cast<int32_t>(pattern.length())));
	icu::UnicodeString uniInput = icu::UnicodeString::fromUTF8(icu::StringPiece(input.data(), static_cast<int32_t>(input.length())));
	icu::RegexMatcher matcher = icu::RegexMatcher(uniPattern, 0, status);
	matcher.reset(uniInput);
	auto match= matcher.matches(status);
	return match;
}

String InsaneIO::Insane::Strings::StringExtensions::TrimStart(const String &data)
{
	String s = data;
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
									{ return !std::isspace(ch); }));
	return s;
}

String InsaneIO::Insane::Strings::StringExtensions::Trim(const String& data, const String& value)
{
	return TrimEnd(TrimStart(data, value),value);
}

String InsaneIO::Insane::Strings::StringExtensions::TrimEnd(const String& data, const String& value)
{
	if (data.size() < value.size())
	{
		return data;
	}
	size_t end = data.size();
	while (data.substr(end - value.size(), value.size()) == value) {
		end -= value.size();
	}
	return data.substr(0, end);
}

String InsaneIO::Insane::Strings::StringExtensions::TrimStart(const String& data, const String& value)
{
	if (data.size() < value.size())
	{
		return data;
	}
	size_t start = 0;
	while (data.substr(start, value.size()) == value) {
		start += value.size();
	}
	return data.substr(start);
}

String InsaneIO::Insane::Strings::StringExtensions::Nameof(const String& name)
{
	std::smatch groups;
	String str = InsaneIO::Insane::Strings::StringExtensions::Trim(name);
	if (std::regex_match(str, groups, std::regex(R"(^&?([_a-zA-Z]\w*(->|\.|::))*([_a-zA-Z]\w*)$)")))
	{
		if (groups.size() == 4)
		{
			return groups[3];
		}
	}
	throw InsaneIO::Insane::Exception::ArgumentException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, "NAMEOF("s + name + "). Invalid identifier.");
}

String InsaneIO::Insane::Strings::StringExtensions::NameofTrimGet(const String& name)
{
	String result = Nameof(name);
	result = Trim(result, "Getter_");
	result = Trim(result, "getter_");
	result = Trim(result, "Get_");
	result = Trim(result, "get_");
	result = Trim(result, "Getter");
	result = Trim(result, "getter");
	result = Trim(result, "Get");
	result = Trim(result, "get");
	return result;
}

String InsaneIO::Insane::Strings::StringExtensions::TrimEnd(const String &data)
{
	String s = data;
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
						 { return !std::isspace(ch); })
				.base(),
			s.end());
	return s;
}

String InsaneIO::Insane::Strings::StringExtensions::Trim(const String &data)
{
	return TrimEnd(TrimStart(data));
}

String InsaneIO::Insane::Strings::StringExtensions::ReplaceLastOf(const String &data, const String &toFind, const String &toReplace)
{
	String result = data;
	size_t pos = result.rfind(toFind);
	if (pos == String::npos)
	{
		return data;
	}
	return result.replace(pos, toFind.length(), toReplace);
}

String InsaneIO::Insane::Strings::StringExtensions::Replace(const String &data, const String &toFind, const String &toReplace)
{
	String result = data;
	size_t pos = 0;
	while ((pos = result.find(toFind, pos)) != String::npos)
	{
		result.replace(pos, toFind.length(), toReplace);
		pos += toReplace.length();
		pos = result.find(toFind, pos);
	}
	return result;
}

String InsaneIO::Insane::Strings::StringExtensions::Replace(const String &data, const std::initializer_list<std::pair<String, String>> &toFindToReplacePairs)
{
	String result = data;
	for (std::pair<String, String> value : toFindToReplacePairs)
	{
		result = StringExtensions::Replace(result, value.first, value.second);
	}
	return result;
}

String InsaneIO::Insane::Strings::StringExtensions::Remove(const String &data, const String &toRemove)
{
	String result = data;
	size_t pos = 0;
	while ((pos = result.find(toRemove, pos)) != String::npos)
	{
		result.erase(pos, toRemove.length());
	}
	return result;
}

String InsaneIO::Insane::Strings::StringExtensions::Remove(const String &data, const std::initializer_list<String> &toRemove)
{
	String result = data;
	for (String value : toRemove)
	{
		result = StringExtensions::Remove(result, value);
	}
	return result;
}

String InsaneIO::Insane::Strings::StringExtensions::InsertRepeat(const String &data, size_t distance, const String &toRepeat, bool includeEnd)
{
	String result = data;
	if (toRepeat.empty() || result.empty() || distance > data.length())
	{
		return result;
	}
	size_t pos = distance;
	while (pos < result.size())
	{
		result.insert(pos, toRepeat);
		pos += distance + toRepeat.length();
	}
	if (pos == result.length() && includeEnd)
	{
		result += toRepeat;
	}
	return result;
}

std::vector<String> InsaneIO::Insane::Strings::StringExtensions::Split(const String &data, const String &toFind)
{
	std::vector<String> v;
	if (data.empty() || toFind.empty())
	{
		v.push_back(data);
		return v;
	}
	size_t ini = 0;
	size_t pos;
	while ((pos = data.find(toFind, ini)) != String::npos)
	{
		String s = data.substr(ini, pos - ini);
		if (!s.empty())
		{
			v.push_back(s);
		}
		ini = pos + toFind.length();
	}
	if (ini < data.length())
	{
		v.push_back(data.substr(ini));
	}

	return v;
}

String InsaneIO::Insane::Strings::StringExtensions::ToUpper(const String &data, const String &locale)
{
	icu::Locale loc = icu::Locale(locale.c_str());
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(data.data(), static_cast<int32_t>(data.length())));
	uniStr.toUpper(loc);
	std::string str;
	uniStr.toUTF8String(str);
	return str;
}

String InsaneIO::Insane::Strings::StringExtensions::ToLower(const String &data, const String &locale)
{
	icu::Locale loc = icu::Locale(locale.c_str());
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(data.data(), static_cast<int32_t>(data.length())));
	uniStr.toLower(loc);
	std::string str;
	uniStr.toUTF8String(str);
	return str;
}

size_t InsaneIO::Insane::Strings::StringExtensions::LengthUTF8(const String &data)
{
	size_t ret = 0;
	for (char value : data)
	{
		if ((value & 0xc0) != 0x80)
		{
			++ret;
		}
	}
	return ret;
}

String InsaneIO::Insane::Strings::StringExtensions::Reverse(const String &data)
{

	String result = data;
	std::reverse(result.begin(), result.end());
	return result;
}

String InsaneIO::Insane::Strings::StringExtensions::ReverseUTF8(const String &data)
{
	String result;
	String::const_iterator it = data.begin();
	String::const_iterator beginIterator(it);
	if ((data[0] & 0xc0) != 0x80)
	{
		it++;
	}
	while (it != data.end())
	{
		char value = *it;
		if ((value & 0xc0) != 0x80)
		{
			result.insert(0, String(beginIterator, it));
			beginIterator = it;
		}
		it++;
	}
	result.insert(0, String(beginIterator, it));
	return result;
}

String InsaneIO::Insane::Strings::StringExtensions::SubstringUTF8(const String &data, size_t startIndex, size_t length)
{
	size_t units = LengthUTF8(data);
	if (startIndex >= units)
	{
		throw std::invalid_argument("Invalid UTF8 character position.");
	}
	String result;
	String::const_iterator it = data.begin();
	String::const_iterator beginIterator = data.begin();
	size_t endIndex = length == SIZE_MAX ? units : startIndex + length;
	size_t utf8pos = 0;
	while (it != data.end())
	{
		char value = *it;
		if ((value & 0xc0) != 0x80) // Es inicio de caracter //utf8pos = 0 / beginIterator = 0 / endIndex = 1+12
		{
			if (utf8pos == startIndex)
			{
				beginIterator = it;
				if (length >= units)
				{
					return String(beginIterator, data.end());
				}
			}

			if (utf8pos == endIndex)
			{
				break;
			}
			utf8pos += 1;
		}
		it++;
	}
	return String(beginIterator, it);
}

String InsaneIO::Insane::Strings::StringExtensions::GetCharUTF8(const String &data, size_t pos)
{
	return SubstringUTF8(data, pos, 1);
}

String InsaneIO::Insane::Strings::StringExtensions::PadRight(const String &data, const size_t &totalWidth, const String &padding)
{

	if (data.length() >= totalWidth)
	{
		return data;
	}
	size_t modulo = (totalWidth - data.length()) % padding.length();
	size_t paddingUnits = (totalWidth - data.length()) / padding.length();
	String ret = data;
	for (size_t i = 0; i < paddingUnits; i++)
	{
		ret.append(padding);
	}
	ret.append(padding.substr(0, modulo));
	return ret;
}

String InsaneIO::Insane::Strings::StringExtensions::PadLeft(const String &data, const size_t &totalWidth, const String &padding)
{
	if (data.length() >= totalWidth)
	{
		return data;
	}
	size_t modulo = (totalWidth - data.length()) % padding.length();
	size_t paddingUnits = (totalWidth - data.length()) / padding.length();
	String ret = data;
	for (size_t i = 0; i < paddingUnits; i++)
	{
		ret.insert(0, padding);
	}
	ret.insert(0, padding.substr(0, modulo));
	return ret;
}

String InsaneIO::Insane::Strings::StringExtensions::PadRightUTF8(const String &data, const size_t &totalWidth, const String &padding)
{
	size_t units = LengthUTF8(data);
	size_t paddingUnits = LengthUTF8(padding);
	if (units >= totalWidth)
	{
		return data;
	}
	size_t modulo = (totalWidth - units) % paddingUnits;
	size_t n = (totalWidth - units) / paddingUnits;
	String ret = data;
	for (size_t i = 0; i < n; i++)
	{
		ret.append(padding);
	}
	ret.append(SubstringUTF8(padding, 0, modulo));
	return ret;
}

String InsaneIO::Insane::Strings::StringExtensions::PadLeftUTF8(const String &data, const size_t &totalWidth, const String &padding)
{
	size_t units = LengthUTF8(data);
	size_t paddingUnits = LengthUTF8(padding);
	if (units >= totalWidth)
	{
		return data;
	}
	size_t modulo = (totalWidth - units) % paddingUnits;
	size_t n = (totalWidth - units) / paddingUnits;
	String ret = data;
	for (size_t i = 0; i < n; i++)
	{
		ret.insert(0, padding);
	}
	ret.insert(0, SubstringUTF8(padding, 0, modulo));
	return ret;
}

String InsaneIO::Insane::Strings::StringExtensions::WideStringToString(const WString &wstr)
{
	if (wstr.empty())
	{
		return String();
	}
	size_t pos = 0;
	size_t begin = 0;
	String ret;

#if WINDOWS_PLATFORM
	/*size_t size;
	pos = wstr.find(static_cast<wchar_t>(0), begin);
	while (pos != WString::npos && begin < wstr.length())
	{
		WString segment = WString(&wstr[begin], pos - begin);
		size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), NULL, 0, NULL, NULL);
		String converted = String(size, 0);
		WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.size(), NULL, NULL);
		ret.append(converted);
		ret.append({0});
		begin = pos + 1;
		pos = wstr.find(static_cast<wchar_t>(0), begin);
	}
	if (begin <= wstr.length())
	{
		WString segment = WString(&wstr[begin], wstr.length() - begin);
		size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), NULL, 0, NULL, NULL);
		String converted = String(size, 0);
		WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.size(), NULL, NULL);
		ret.append(converted);
	}*/
#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
	size_t size;
	pos = wstr.find(static_cast<wchar_t>(0), begin);
	while (pos != WString::npos && begin < wstr.length())
	{
		WString segment = WString(&wstr[begin], pos - begin);
		size = wcstombs(nullptr, segment.c_str(), 0);
		String converted = String(size, 0);
		wcstombs(&converted[0], segment.c_str(), converted.size());
		ret.append(converted);
		ret.append({0});
		begin = pos + 1;
		pos = wstr.find(static_cast<wchar_t>(0), begin);
	}
	if (begin <= wstr.length())
	{
		WString segment = WString(&wstr[begin], wstr.length() - begin);
		size = wcstombs(nullptr, segment.c_str(), 0);
		String converted = String(size, 0);
		wcstombs(&converted[0], segment.c_str(), converted.size());
		ret.append(converted);
	}
#else
	static_assert(false, "Unknown Platform");
#endif
	return ret;
}

WString InsaneIO::Insane::Strings::StringExtensions::StringToWideString(const String &str)
{
	if (str.empty())
	{
		return WString();
	}

	size_t pos = 0;
	size_t begin = 0;
	WString ret;
#ifdef WINDOWS_PLATFORM
	/*int size = 0;
	pos = str.find(static_cast<char>(0), begin);
	while (pos != String::npos)
	{
		String segment = String(&str[begin], pos - begin);
		std::wstring converted = std::wstring(segment.size() + 1, 0);
		size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &segment[0], segment.size(), &converted[0], converted.length());
		converted.resize(size);
		ret.append(converted);
		ret.append({0});
		begin = pos + 1;
		pos = str.find(static_cast<char>(0), begin);
	}
	if (begin < str.length())
	{
		String segment = String(&str[begin], str.length() - begin);
		std::wstring converted = std::wstring(segment.size() + 1, 0);
		size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, segment.c_str(), segment.size(), &converted[0], converted.length());
		converted.resize(size);
		ret.append(converted);
	}*/

#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
	size_t size;
	pos = str.find(static_cast<char>(0), begin);
	while (pos != String::npos)
	{
		String segment = String(&str[begin], pos - begin);
		WString converted = WString(segment.size(), 0);
		size = mbstowcs(&converted[0], &segment[0], converted.size());
		converted.resize(size);
		ret.append(converted);
		ret.append({0});
		begin = pos + 1;
		pos = str.find(static_cast<char>(0), begin);
	}
	if (begin < str.length())
	{
		String segment = String(&str[begin], str.length() - begin);
		WString converted = WString(segment.size(), 0);
		size = mbstowcs(&converted[0], &segment[0], converted.size());
		converted.resize(size);
		ret.append(converted);
	}
#else
	static_assert(false, "Unknown Platform");
#endif
	return ret;
}

bool InsaneIO::Insane::Strings::StringExtensions::StartsWith(const String &data, const String &preffix, const bool &caseSensitive, const String &locale)
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(data, locale);
		String npreffix = StringExtensions::ToUpper(preffix, locale);
		return ndata.find(npreffix) == 0;
	}
	return data.find(preffix) == 0;
}

bool InsaneIO::Insane::Strings::StringExtensions::EndsWith(const String &data, const String &suffix, const bool &caseSensitive, const String &locale)
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(data, locale);
		String nsuffix = StringExtensions::ToUpper(suffix, locale);
		return ndata.find(nsuffix, ndata.size() - nsuffix.size()) != String::npos;
	}
	return data.find(suffix, data.size() - suffix.size()) != String::npos;
}

bool InsaneIO::Insane::Strings::StringExtensions::Contains(const String &data, const String &content, const bool &caseSensitive, const String &locale)
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(data, locale);
		String ncontent = StringExtensions::ToUpper(content, locale);
		return ndata.find(ncontent) != String::npos;
	}
	return data.find(content) != String::npos;
}

bool InsaneIO::Insane::Strings::StringExtensions::IsValidUTF8(const std::string &data)
{
	UErrorCode status = U_ZERO_ERROR;
	UCharsetDetector *detector = ucsdet_open(&status);
	ucsdet_setText(detector, data.c_str(), static_cast<int32_t>(data.length()), &status);
	int32_t detectedNumber = 0;
	auto matches = ucsdet_detectAll(detector, &detectedNumber, &status);
	if (!matches)
	{
		return false;
	}
	bool valid = false;
	for (int32_t i = 0; i < detectedNumber; i++)
	{
		const char *charsetName = ucsdet_getName(matches[i], &status);
		if (UTF8_CHARSET_NAME_STRING == charsetName)
		{
			valid = true;
			break;
		}
	}
	ucsdet_close(detector);
	return valid;
}

String InsaneIO::Insane::Strings::StringExtensions::Join(std::vector<String> strings, String connector)
{
	if (strings.size() == 0)
	{
		return EMPTY_STRING;
	}

	if (strings.size() == 1)
	{
		return strings[0];
	}

	String result = EMPTY_STRING;
	for (size_t i = 0; i < strings.size() - 1; i++) {
		result += strings[i] + connector;
	}
	result += strings[strings.size() - 1];
	return result;
}

String InsaneIO::Insane::Strings::StringExtensions::ToOctalEscapedCodeLiteral(const String &data, const bool quotationMarkEnclosed)
{
	USING_NS_INSANE_STR;
	String result = EMPTY_STRING;
	String quotes = (quotationMarkEnclosed ? QUOTATION_MARK_STRING : EMPTY_STRING);
	for (size_t i = 0; i < data.length(); i++)
	{
		result += BACKSLASH_STRING + InternalToOctal<char>(data[i]);
	}
	return quotes + result + quotes;
}

String InsaneIO::Insane::Strings::StringExtensions::ToUnicodeEscapedCodeLiteral(const String &data, const bool quotationMarkEnclosed)
{
	USING_NS_INSANE_STR;
	if (!IsValidUTF8(data))
	{
		return ToOctalEscapedCodeLiteral(data);
	}
	String result = EMPTY_STRING;
	String quotes = (quotationMarkEnclosed ? QUOTATION_MARK_STRING : EMPTY_STRING);
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(data.data(), static_cast<int32_t>(data.length())));
	for (int32_t i = 0; i < uniStr.countChar32(); i++)
	{
		result += "\\U" + InternalToHexadecimal<UChar32>(uniStr.char32At(i));
	}
	return quotes + result + quotes;
}

String InsaneIO::Insane::Strings::StringExtensions::ToCodeLiteral(const String &data, const bool quotationMarkEnclosed, const bool &escaped)
{
	USING_NS_INSANE_STR;
	bool isvalidUtf8 = IsValidUTF8(data);
	if (!isvalidUtf8 || escaped)
	{
		return ToOctalEscapedCodeLiteral(data);
	}

	String result = EMPTY_STRING;
	String quotes = (quotationMarkEnclosed ? QUOTATION_MARK_STRING : EMPTY_STRING);
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(data.data(), static_cast<int32_t>(data.length())));
	bool added;
	for (size_t i = 0; i < data.length(); i++)
	{
		added = false;
		if (data[i] >= 0x00 && data[i] <= 0x1f)
		{
			added = true;
			switch ((char)data[i])
			{
			case (char)7:
				result += "\\a";
				break;
			case (char)8:
				result += "\\b";
				break;
			case (char)9:
				result += "\\t";
				break;
			case (char)10:
				result += data[i];
				break;
			case (char)11:
				result += "\\v";
				break;
			case (char)12:
				result += "\\f";
				break;
			case (char)13:
				result += "\\r";
				break;
			case '\"':
				result += "\\\"";
			case '\\':
				result += "\\\\";
				break;
			default:
				result += "\\" + InternalToOctal<char>(data[i]);
				break;
			}
		}
		if (data[i] >= static_cast<char>(0x7f) && data[i] <= static_cast<char>(0x9f) && !added)
		{
			result += "\\" + InternalToOctal<char>(data[i]);
		}
		else
		{
			if (!added)
			{
				result += data[i];
			}
		}
	}
	return quotes + result + quotes;
}

// ███ Xtring ███
InsaneIO::Insane::Strings::Xtring::Xtring(const String &str) : String(str)
{
}

Xtring InsaneIO::Insane::Strings::Xtring::TrimStart()
{
	erase(begin(), std::find_if(begin(), end(), [](unsigned char ch)
								{ return !std::isspace(ch); }));
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::TrimEnd()
{
	erase(std::find_if(rbegin(), rend(), [](unsigned char ch)
					   { return !std::isspace(ch); })
			  .base(),
		  end());
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::Trim()
{
	return TrimStart().TrimEnd();
}

Xtring InsaneIO::Insane::Strings::Xtring::ReplaceLastOf(const String &toFind, const String &toReplace)
{
	size_t pos = rfind(toFind);
	if (pos == String::npos)
	{
		return thisvalue;
	}
	replace(pos, toFind.length(), toReplace);
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::Replace(const String &toFind, const String &toReplace)
{
	size_t pos = 0;
	while ((pos = find(toFind, pos)) != String::npos)
	{
		replace(pos, toFind.length(), toReplace);
		pos += toReplace.length();
		pos = find(toFind, pos);
	}
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::Replace(const std::initializer_list<std::pair<String, String>> &toFindToReplacePairs)
{
	for (std::pair<String, String> value : toFindToReplacePairs)
	{
		Replace(value.first, value.second);
	}
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::Remove(const String &toRemove)
{
	size_t pos = 0;
	while ((pos = find(toRemove, pos)) != String::npos)
	{
		erase(pos, toRemove.length());
	}
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::Remove(const std::initializer_list<String> &toRemove)
{
	for (String value : toRemove)
	{
		Remove(value);
	}
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::InsertRepeat(size_t distance, const String &toRepeat, bool includeEnd)
{
	if (toRepeat.empty() || thisvalue.empty() || distance == 0)
	{
		return thisvalue;
	}
	size_t pos = distance;
	while (pos < size())
	{
		thisvalue.insert(pos, toRepeat);
		pos += distance + toRepeat.length();
	}
	if (pos == length() && includeEnd)
	{
		append(toRepeat);
	}
	return thisvalue;
}

size_t InsaneIO::Insane::Strings::Xtring::LengthUTF8() const
{
	size_t ret = 0;
	for (char value : thisvalue)
	{
		if ((value & 0xc0) != 0x80)
		{
			++ret;
		}
	}
	return ret;
}

Xtring InsaneIO::Insane::Strings::Xtring::ReverseUTF8()
{

	Xtring result;
	String::const_iterator it = begin();
	String::const_iterator beginIterator(it);
	if ((thisvalue[0] & 0xc0) != 0x80)
	{
		it++;
	}
	while (it != end())
	{
		char value = *it;
		if ((value & 0xc0) != 0x80)
		{
			result.insert(0, String(beginIterator, it));
			beginIterator = it;
		}
		it++;
	}
	result.insert(0, String(beginIterator, it));
	assign(result);
	result.clear();
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::Reverse()
{
	std::reverse(thisvalue.begin(), thisvalue.end());
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::SubstringUTF8(size_t startIndex, size_t length) const
{
	size_t units = LengthUTF8();
	if (startIndex >= units)
	{
		throw std::invalid_argument("Invalid UTF8 character position.");
	}
	Xtring result;
	String::const_iterator it = thisvalue.begin();
	String::const_iterator beginIterator = thisvalue.begin();
	size_t endIndex = length == SIZE_MAX ? units : startIndex + length;
	size_t utf8pos = 0;
	while (it != thisvalue.end())
	{
		char value = *it;
		if ((value & 0xc0) != 0x80)
		{
			if (utf8pos == startIndex)
			{
				beginIterator = it;
				if (length >= units)
				{
					return Xtring(beginIterator, static_cast<String::const_iterator>(thisvalue.end()));
				}
			}

			if (utf8pos == endIndex)
			{
				break;
			}
			utf8pos += 1;
		}
		it++;
	}
	return Xtring(beginIterator, it);
}

Xtring InsaneIO::Insane::Strings::Xtring::GetCharUTF8(size_t utf8pos) const
{
	return SubstringUTF8(utf8pos, 1);
}

Xtring InsaneIO::Insane::Strings::Xtring::PadRight(const size_t &totalWidth, const Xtring &padding)
{

	if (thisvalue.length() >= totalWidth)
	{
		return thisvalue;
	}
	size_t modulo = (totalWidth - thisvalue.length()) % padding.length();
	size_t paddingUnits = (totalWidth - thisvalue.length()) / padding.length();
	for (size_t i = 0; i < paddingUnits; i++)
	{
		thisvalue.append(padding);
	}
	thisvalue.append(padding.substr(0, modulo));
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::PadLeft(const size_t &totalWidth, const Xtring &padding)
{
	if (thisvalue.length() >= totalWidth)
	{
		return thisvalue;
	}
	size_t modulo = (totalWidth - thisvalue.length()) % padding.length();
	size_t paddingUnits = (totalWidth - thisvalue.length()) / padding.length();
	for (size_t i = 0; i < paddingUnits; i++)
	{
		thisvalue.insert(0, padding);
	}
	thisvalue.insert(0, padding.substr(0, modulo));
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::PadRightUTF8(const size_t &totalWidth, const Xtring &padding)
{
	size_t units = LengthUTF8();
	size_t paddingUnits = StringExtensions::LengthUTF8(padding);
	if (units >= totalWidth)
	{
		return thisvalue;
	}
	size_t modulo = (totalWidth - units) % paddingUnits;
	size_t n = (totalWidth - units) / paddingUnits;
	for (size_t i = 0; i < n; i++)
	{
		thisvalue.append(padding);
	}
	thisvalue.append(StringExtensions::SubstringUTF8(padding, 0, modulo));
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::PadLeftUTF8(const size_t &totalWidth, const Xtring &padding)
{
	size_t units = LengthUTF8();
	size_t paddingUnits = StringExtensions::LengthUTF8(padding);
	if (units >= totalWidth)
	{
		return thisvalue;
	}
	size_t modulo = (totalWidth - units) % paddingUnits;
	size_t n = (totalWidth - units) / paddingUnits;
	for (size_t i = 0; i < n; i++)
	{
		thisvalue.insert(0, padding);
	}
	thisvalue.insert(0, StringExtensions::SubstringUTF8(padding, 0, modulo));
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::ToUpper(const String &locale)
{
	icu::Locale loc = icu::Locale(locale.c_str());
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(thisvalue.data(), static_cast<int32_t>(thisvalue.length())));
	uniStr.toUpper(loc);
	clear();
	uniStr.toUTF8String(thisvalue);
	return thisvalue;
}

Xtring InsaneIO::Insane::Strings::Xtring::ToLower(const String &locale)
{
	icu::Locale loc = icu::Locale(locale.c_str());
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(thisvalue.data(), static_cast<int32_t>(thisvalue.length())));
	uniStr.toLower(loc);
	clear();
	uniStr.toUTF8String(thisvalue);
	return thisvalue;
}

std::vector<String> InsaneIO::Insane::Strings::Xtring::Split(const String &toFind) const
{
	std::vector<String> v;
	if (empty() || toFind.empty())
	{
		v.push_back(thisvalue);
		return v;
	}
	size_t ini = 0;
	size_t pos;
	while ((pos = find(toFind, ini)) != String::npos)
	{
		String s = substr(ini, pos - ini);
		if (!s.empty())
		{
			v.push_back(s);
		}
		ini = pos + toFind.length();
	}
	if (ini < length())
	{
		v.push_back(substr(ini));
	}
	return v;
}

bool InsaneIO::Insane::Strings::Xtring::IsMatch(const String &pattern) const
{
	std::regex regex(pattern, std::regex_constants::ECMAScript);
	return std::regex_match(thisvalue, regex);
}

bool InsaneIO::Insane::Strings::Xtring::StartsWith(const String &preffix, const bool &caseSensitive, const String &locale) const
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(thisvalue, locale);
		String npreffix = StringExtensions::ToUpper(preffix, locale);
		return ndata.find(npreffix) == 0;
	}
	return find(preffix) == 0;
}

bool InsaneIO::Insane::Strings::Xtring::EndsWith(const String &suffix, const bool &caseSensitive, const String &locale) const
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(thisvalue, locale);
		String nsuffix = StringExtensions::ToUpper(suffix, locale);
		return ndata.find(nsuffix, ndata.size() - nsuffix.size()) != String::npos;
	}
	return find(suffix, size() - suffix.size()) != String::npos;
}

bool InsaneIO::Insane::Strings::Xtring::Contains(const String &content, const bool &caseSensitive, const String &locale) const
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(thisvalue, locale);
		String ncontent = StringExtensions::ToUpper(content, locale);
		return ndata.find(ncontent) != String::npos;
	}
	return find(content) != String::npos;
}
