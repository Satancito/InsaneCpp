#include <Insane/InsaneString.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>
#include <unicode/unistr.h>
#include <unicode/locid.h>

// ███ Strings ███

String Insane::Strings::StringExtensions::Empty()
{
	return EMPTY_STRING;
}

bool Insane::Strings::StringExtensions::IsMatch(const String &input, const String &pattern)
{
	std::regex regex(pattern, std::regex_constants::ECMAScript);
	return std::regex_match(input, regex);
}

String Insane::Strings::StringExtensions::TrimStart(const String &data)
{
	String s = data;
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
				return !std::isspace(ch);
			}));
	return s;
}

String Insane::Strings::StringExtensions::TrimEnd(const String &data)
{
	String s = data;
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
			}).base(),
			s.end());
	return s;
}

String Insane::Strings::StringExtensions::Trim(const String &data)
{
	return TrimEnd(TrimStart(data));
}

String Insane::Strings::StringExtensions::ReplaceLastOf(const String &data, const String &toFind, const String &toReplace)
{
	String result = data;
	size_t pos = result.rfind(toFind);
	if (pos == String::npos)
	{
		return data;
	}
	return result.replace(pos, toFind.length(), toReplace);
}

String Insane::Strings::StringExtensions::Replace(const String &data, const String &toFind, const String &toReplace)
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

String Insane::Strings::StringExtensions::Replace(const String &data, const std::initializer_list<std::pair<ToFind, ToReplace>> &pairs)
{
	String result = data;
	for (std::pair<String, String> value : pairs)
	{
		result = StringExtensions::Replace(result, value.first, value.second);
	}
	return result;
}

String Insane::Strings::StringExtensions::Remove(const String &data, const String &toRemove)
{
	String result = data;
	size_t pos = 0;
	while ((pos = result.find(toRemove, pos)) != String::npos)
	{
		result.erase(pos, toRemove.length());
	}
	return result;
}

String Insane::Strings::StringExtensions::Remove(const String &data, const std::initializer_list<String> &toRemove)
{
	String result = data;
	for (String value : toRemove)
	{
		result = StringExtensions::Remove(result, value);
	}
	return result;
}

String Insane::Strings::StringExtensions::InsertRepeat(const String &data, size_t distance, const String &toRepeat, bool includeEnd)
{
	String result = data;
	if (toRepeat.empty() || result.empty())
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

std::vector<String> Insane::Strings::StringExtensions::Split(const String &data, const String &toFind)
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

String Insane::Strings::StringExtensions::ToUpper(const String &data, const String &locale)
{
	icu::Locale loc = icu::Locale(locale.c_str());
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(data));
    uniStr.toUpper(loc);
    std::string str;
    uniStr.toUTF8String(str);
	return str;
}

String Insane::Strings::StringExtensions::ToLower(const String &data, const String &locale)
{
	icu::Locale loc = icu::Locale(locale.c_str());
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(data));
    uniStr.toLower(loc);
    std::string str;
    uniStr.toUTF8String(str);
	return str;
}

size_t Insane::Strings::StringExtensions::LengthUTF8(const String &data)
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

String Insane::Strings::StringExtensions::Reverse(const String &data)
{

	String result = data;
	std::reverse(result.begin(), result.end());
	return result;
}

String Insane::Strings::StringExtensions::ReverseUTF8(const String &data)
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

String Insane::Strings::StringExtensions::SubstringUTF8(const String &data, size_t startIndex, size_t length)
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

String Insane::Strings::StringExtensions::GetCharUTF8(const String &data, size_t pos)
{
	return SubstringUTF8(data, pos, 1);
}

String Insane::Strings::StringExtensions::PadRight(const String &data, const size_t &totalWidth, const char &padding)
{
	if (data.length() >= totalWidth)
	{
		return data;
	}
	String ret = data;
	ret.resize(totalWidth, padding);
	return ret;
}

String Insane::Strings::StringExtensions::PadLeft(const String &data, const size_t &totalWidth, const char &padding)
{
	if (data.length() >= totalWidth)
	{
		return data;
	}
	String ret = data;
	ret.insert(0, totalWidth - ret.length(), padding);
	return ret;
}

String Insane::Strings::StringExtensions::PadRight(const String &data, const size_t &totalWidth, const String &padding)
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

String Insane::Strings::StringExtensions::PadLeft(const String &data, const size_t &totalWidth, const String &padding)
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

String Insane::Strings::StringExtensions::PadRightUTF8(const String &data, const size_t &totalWidth, const char &padding)
{
	size_t totalUtf8 = LengthUTF8(data);
	if (totalUtf8 >= totalWidth)
	{
		return data;
	}
	String ret = data;
	ret.resize(ret.length() + (totalWidth - totalUtf8), padding);
	return ret;
}

String Insane::Strings::StringExtensions::PadLeftUTF8(const String &data, const size_t &totalWidth, const char &padding)
{
	size_t totalUtf8 = LengthUTF8(data);
	if (totalUtf8 >= totalWidth)
	{
		return data;
	}
	String ret = data;
	ret.insert(0, totalWidth - totalUtf8, padding);
	return ret;
}

String Insane::Strings::StringExtensions::PadRightUTF8(const String &data, const size_t &totalWidth, const String &padding)
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

String Insane::Strings::StringExtensions::PadLeftUTF8(const String &data, const size_t &totalWidth, const String &padding)
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

String Insane::Strings::StringExtensions::WideStringToString(const WString &wstr)
{
	if (wstr.empty())
	{
		return String();
	}
	size_t pos;
	size_t begin = 0;
	String ret;

#if WINDOWS_PLATFORM
	int size;
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
	}
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

WString Insane::Strings::StringExtensions::StringToWideString(const String &str)
{
	if (str.empty())
	{
		return WString();
	}

	size_t pos;
	size_t begin = 0;
	WString ret;
#ifdef WINDOWS_PLATFORM
	int size = 0;
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
	}

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

bool Insane::Strings::StringExtensions::StartsWith(const String &data, const String &preffix, const bool &caseSensitive, const String &locale)
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(data,locale);
		String npreffix = StringExtensions::ToUpper(preffix, locale);
		return ndata.find(npreffix) == 0;
	}
	return data.find(preffix) == 0;
}

bool Insane::Strings::StringExtensions::EndsWith(const String &data, const String &suffix, const bool &caseSensitive,const String &locale)
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(data, locale);
		String nsuffix = StringExtensions::ToUpper(suffix,locale);
		return ndata.find(nsuffix, ndata.size() - nsuffix.size()) != String::npos;
	}
	return data.find(suffix, data.size() - suffix.size()) != String::npos;
}

bool Insane::Strings::StringExtensions::Contains(const String &data, const String &content, const bool &caseSensitive,const String &locale)
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(data, locale);
		String ncontent = StringExtensions::ToUpper(content,locale);
		return ndata.find(ncontent) != String::npos;
	}
	return data.find(content) != String::npos;
}

// ███ Xtring ███
Insane::Strings::Xtring::Xtring(const String &str)
{
	assign(str);
}

Xtring Insane::Strings::Xtring::TrimStart()
{
	erase(begin(), std::find_if(begin(), end(), [](unsigned char ch) {
			  return !std::isspace(ch);
		  }));
	return thisvalue;
}

Xtring Insane::Strings::Xtring::TrimEnd()
{
	erase(std::find_if(rbegin(), rend(), [](unsigned char ch) {
			  return !std::isspace(ch);
		  }).base(),
		  end());
	return thisvalue;
}

Xtring Insane::Strings::Xtring::Trim()
{
	return TrimStart().TrimEnd();
}

Xtring Insane::Strings::Xtring::ReplaceLastOf(const String &toFind, const String &toReplace)
{
	size_t pos = rfind(toFind);
	if (pos == String::npos)
	{
		return thisvalue;
	}
	replace(pos, toFind.length(), toReplace);
	return thisvalue;
}

Xtring Insane::Strings::Xtring::Replace(const String &toFind, const String &toReplace)
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

Xtring Insane::Strings::Xtring::Replace(const std::initializer_list<std::pair<ToFind, ToReplace>> &pairs)
{
	for (std::pair<String, String> value : pairs)
	{
		Replace(value.first, value.second);
	}
	return thisvalue;
}

Xtring Insane::Strings::Xtring::Remove(const String &toRemove)
{
	size_t pos = 0;
	while ((pos = find(toRemove, pos)) != String::npos)
	{
		erase(pos, toRemove.length());
	}
	return thisvalue;
}

Xtring Insane::Strings::Xtring::Remove(const std::initializer_list<String> &toRemove)
{
	for (String value : toRemove)
	{
		Remove(value);
	}
	return thisvalue;
}

Xtring Insane::Strings::Xtring::InsertRepeat(size_t distance, const String &toRepeat, bool includeEnd)
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

size_t Insane::Strings::Xtring::LengthUTF8() const
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

Xtring Insane::Strings::Xtring::ReverseUTF8()
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

Xtring Insane::Strings::Xtring::Reverse()
{
	std::reverse(thisvalue.begin(), thisvalue.end());
	return thisvalue;
}

Xtring Insane::Strings::Xtring::SubstringUTF8(size_t startIndex, size_t length) const
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

Xtring Insane::Strings::Xtring::GetCharUTF8(size_t utf8pos) const
{
	return SubstringUTF8(utf8pos, 1);
}

Xtring Insane::Strings::Xtring::PadRight(const size_t &totalWidth, const char &padding)
{
	if (thisvalue.length() >= totalWidth)
	{
		return thisvalue;
	}
	thisvalue.resize(totalWidth, padding);
	return thisvalue;
}

Xtring Insane::Strings::Xtring::PadLeft(const size_t &totalWidth, const char &padding)
{
	if (thisvalue.length() >= totalWidth)
	{
		return thisvalue;
	}
	thisvalue.insert(0, totalWidth - thisvalue.length(), padding);
	return thisvalue;
}

Xtring Insane::Strings::Xtring::PadRight(const size_t &totalWidth, const Xtring &padding)
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

Xtring Insane::Strings::Xtring::PadLeft(const size_t &totalWidth, const Xtring &padding)
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

Xtring Insane::Strings::Xtring::PadRightUTF8(const size_t &totalWidth, const char &padding)
{
	size_t totalUtf8 = LengthUTF8();
	if (totalUtf8 >= totalWidth)
	{
		return thisvalue;
	}
	thisvalue.resize(thisvalue.length() + (totalWidth - totalUtf8), padding);
	return thisvalue;
}

Xtring Insane::Strings::Xtring::PadLeftUTF8(const size_t &totalWidth, const char &padding)
{
	size_t totalUtf8 = LengthUTF8();
	if (totalUtf8 >= totalWidth)
	{
		return thisvalue;
	}
	thisvalue.insert(0, totalWidth - totalUtf8, padding);
	return thisvalue;
}

Xtring Insane::Strings::Xtring::PadRightUTF8(const size_t &totalWidth, const Xtring &padding)
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

Xtring Insane::Strings::Xtring::PadLeftUTF8(const size_t &totalWidth, const Xtring &padding)
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

Xtring Insane::Strings::Xtring::ToUpper(const String &locale)
{
	icu::Locale loc = icu::Locale(locale.c_str());
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(thisvalue));
    uniStr.toUpper(loc);
    clear();
    uniStr.toUTF8String(thisvalue);
	return thisvalue;
}

Xtring Insane::Strings::Xtring::ToLower(const String &locale)
{
	icu::Locale loc = icu::Locale(locale.c_str());
	icu::UnicodeString uniStr = icu::UnicodeString::fromUTF8(icu::StringPiece(thisvalue));
    uniStr.toLower(loc);
    clear();
    uniStr.toUTF8String(thisvalue);
	return thisvalue;
}

std::vector<String> Insane::Strings::Xtring::Split(const String &toFind) const
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

bool Insane::Strings::Xtring::IsMatch(const String &pattern) const
{
	std::regex regex(pattern, std::regex_constants::ECMAScript);
	return std::regex_match(thisvalue, regex);
}

bool Insane::Strings::Xtring::StartsWith(const String &preffix, const bool &caseSensitive, const String &locale = DEFAULT_LOCALE_STRING) const
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(thisvalue, locale);
		String npreffix = StringExtensions::ToUpper(preffix, locale);
		return ndata.find(npreffix) == 0;
	}
	return find(preffix) == 0;
}

bool Insane::Strings::Xtring::EndsWith(const String &suffix, const bool &caseSensitive, const String &locale = DEFAULT_LOCALE_STRING) const
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(thisvalue, locale);
		String nsuffix = StringExtensions::ToUpper(suffix, locale);
		return ndata.find(nsuffix, ndata.size() - nsuffix.size()) != String::npos;
	}
	return find(suffix, size() - suffix.size()) != String::npos;
}

bool Insane::Strings::Xtring::Contains(const String &content, const bool &caseSensitive, const String &locale = DEFAULT_LOCALE_STRING) const
{
	if (!caseSensitive)
	{
		String ndata = StringExtensions::ToUpper(thisvalue, locale);
		String ncontent = StringExtensions::ToUpper(content, locale);
		return ndata.find(ncontent) != String::npos;
	}
	return find(content) != String::npos;
}