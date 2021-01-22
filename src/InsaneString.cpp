#include <Insane/InsaneString.h>

// ███ Strings ███

Insane::Str::Strings::Strings() = default;

Insane::Str::Strings::~Strings() = default;

String Insane::Str::Strings::Empty()
{
	return EMPTY_STRING;
}

bool Insane::Str::Strings::IsMatch(const String &input, const String &pattern)
{
	std::regex regex(pattern, std::regex_constants::ECMAScript);
	return std::regex_match(input, regex);
}

String Insane::Str::Strings::TrimStart(const std::string &data)
{
	String s = data;
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
				return !std::isspace(ch);
			}));
	return s;
}

String Insane::Str::Strings::TrimEnd(const std::string &data)
{
	String s = data;
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
			}).base(),
			s.end());
	return s;
}

String Insane::Str::Strings::Trim(const std::string &data)
{
	return TrimEnd(TrimStart(data));
}

String Insane::Str::Strings::ReplaceLastOf(const String &data, const String &toFind, const String &toReplace)
{
	String result = data;
	size_t pos = result.rfind(toFind);
	if (pos == std::string::npos)
	{
		return data;
	}
	return result.replace(pos, toFind.length(), toReplace);
}

String Insane::Str::Strings::Replace(const String &data, const String &toFind, const String &toReplace)
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

String Insane::Str::Strings::Replace(const String &data, const std::initializer_list<std::pair<ToFind, ToReplace>> &pairs)
{
	String result = data;
	for (std::pair<String, String> value : pairs)
	{
		result = Strings::Replace(result, value.first, value.second);
	}
	return result;
}

String Insane::Str::Strings::Remove(const String &data, const String &toRemove)
{
	String result = data;
	size_t pos = 0;
	while ((pos = result.find(toRemove, pos)) != String::npos)
	{
		result.erase(pos, toRemove.length());
	}
	return result;
}

String Insane::Str::Strings::Remove(const String &data, const std::initializer_list<String> &toRemove)
{
	String result = data;
	for (String value : toRemove)
	{
		result = Strings::Remove(result, value);
	}
	return result;
}

String Insane::Str::Strings::InsertRepeat(const String &data, size_t distance, const String &toRepeat, bool includeEnd)
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

std::vector<String> Insane::Str::Strings::Split(const String &data, const String &toFind)
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

WString Insane::Str::Strings::ToUpper(const WString &data)
{
	WString result = data;
	auto &f = std::use_facet<std::ctype<wchar_t>>(std::locale());

	f.toupper(&result[0], &result[0] + result.size());
	return result;
}

WString Insane::Str::Strings::ToLower(const WString &data)
{
	WString result = data;
	auto &f = std::use_facet<std::ctype<wchar_t>>(std::locale());
	f.tolower(&result[0], &result[0] + result.size());
	return result;
}

String Insane::Str::Strings::ToUpper(const String &data)
{
	return WideStringToString(ToUpper(StringToWideString(data)));
}

String Insane::Str::Strings::ToLower(const String &data)
{
	return WideStringToString(ToLower(StringToWideString(data)));
}

size_t Insane::Str::Strings::LengthUTF8(const String &data)
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

String Insane::Str::Strings::Reverse(const String &data)
{

	String result = data;
	std::reverse(result.begin(), result.end());
	return result;
}

String Insane::Str::Strings::ReverseUTF8(const String &data)
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

String Insane::Str::Strings::SubstringUTF8(const String &data, size_t startIndex, size_t length)
{
	size_t units = LengthUTF8(data);
	if (startIndex >= units)
	{
		throw std::invalid_argument(u8"Invalid UTF8 character position.");
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

String Insane::Str::Strings::GetCharUTF8(const String &data, size_t pos)
{
	return SubstringUTF8(data, pos, 1);
}

String Insane::Str::Strings::PadRight(const String &data, const size_t &totalWidth, const char &padding)
{
	if (data.length() >= totalWidth)
	{
		return data;
	}
	String ret = data;
	ret.resize(totalWidth, padding);
	return ret;
}

String Insane::Str::Strings::PadLeft(const String &data, const size_t &totalWidth, const char &padding)
{
	if (data.length() >= totalWidth)
	{
		return data;
	}
	String ret = data;
	ret.insert(0, totalWidth - ret.length(), padding);
	return ret;
}

String Insane::Str::Strings::PadRight(const String &data, const size_t &totalWidth, const String &padding)
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

String Insane::Str::Strings::PadLeft(const String &data, const size_t &totalWidth, const String &padding)
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

String Insane::Str::Strings::PadRightUTF8(const String &data, const size_t &totalWidth, const char &padding)
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

String Insane::Str::Strings::PadLeftUTF8(const String &data, const size_t &totalWidth, const char &padding)
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

String Insane::Str::Strings::PadRightUTF8(const String &data, const size_t &totalWidth, const String &padding)
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

String Insane::Str::Strings::PadLeftUTF8(const String &data, const size_t &totalWidth, const String &padding)
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

WString Insane::Str::Strings::EmptyW()
{
	return EMPTY_WSTRING;
}

WString Insane::Str::Strings::Replace(const WString &data, const WString &toFind, const WString &toReplace)
{
	WString result = data;
	if (result.empty() || toFind.empty())
	{
		return result;
	}

	size_t pos = 0;
	while ((pos = result.find(toFind, pos)) != WString::npos)
	{
		result.replace(pos, toFind.length(), toReplace);
		pos += toReplace.length();
	}
	return result;
}

WString Insane::Str::Strings::Remove(const WString &data, const WString &toRemove)
{
	WString result = data;
	if (toRemove.empty() || result.empty())
	{
		return result;
	}
	size_t pos = 0;
	while ((pos = result.find(toRemove, pos)) != WString::npos)
	{
		result.erase(pos, toRemove.length());
	}
	return result;
}

WString Insane::Str::Strings::InsertRepeat(const WString &data, size_t repeatEvery, const WString &toRepeat, bool includeEnd)
{
	WString result = data;
	if (repeatEvery < 1 || toRepeat.empty() || result.length() < 1 || toRepeat.length() < 1)
	{
		return result;
	}
	size_t pos = repeatEvery;
	while (pos < result.size())
	{
		result.insert(pos, toRepeat);
		pos += repeatEvery + toRepeat.length();
	}
	if (pos == result.length() && includeEnd)
	{
		result += toRepeat;
	}
	return result;
}

std::vector<WString> Insane::Str::Strings::Split(const WString &data, const WString &toFind)
{
	std::vector<WString> v;
	if (data.empty() || toFind.empty())
	{
		v.push_back(data);
		return v;
	}
	size_t ini = 0;
	size_t pos;
	while ((pos = data.find(toFind, ini)) != WString::npos)
	{
		WString s = data.substr(ini, pos - ini);
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

String Insane::Str::Strings::WideStringToString(const WString &wstr)
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

WString Insane::Str::Strings::StringToWideString(const String &str)
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
	while (pos != std::string::npos)
	{
		std::string segment = std::string(&str[begin], pos - begin);
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
		std::string segment = std::string(&str[begin], str.length() - begin);
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

bool Insane::Str::Strings::StartsWith(const String &data, const String &preffix, const bool &caseSensitive)
{
	if (!caseSensitive)
	{
		String ndata = Strings::ToUpper(data);
		String npreffix = Strings::ToUpper(preffix);
		return ndata.find(npreffix) == 0;
	}
	return data.find(preffix) == 0;
}

bool Insane::Str::Strings::EndsWith(const String &data, const String &suffix, const bool &caseSensitive)
{
	if (!caseSensitive)
	{
		String ndata = Strings::ToUpper(data);
		String nsuffix = Strings::ToUpper(suffix);
		return ndata.find(nsuffix, ndata.size() - nsuffix.size()) != std::string::npos;
	}
	return data.find(suffix, data.size() - suffix.size()) != std::string::npos;
}

bool Insane::Str::Strings::Contains(const String &data, const String &content, const bool &caseSensitive)
{
	if (!caseSensitive)
	{
		String ndata = Strings::ToUpper(data);
		String ncontent = Strings::ToUpper(content);
		return ndata.find(ncontent) != std::string::npos;
	}
	return data.find(content) != std::string::npos;
}

// ███ Xtring ███
Insane::Str::Xtring::Xtring(const String &str)
{
	assign(str);
}

Xtring Insane::Str::Xtring::TrimStart()
{
	erase(begin(), std::find_if(begin(), end(), [](unsigned char ch) {
			  return !std::isspace(ch);
		  }));
	return thisvalue;
}

Xtring Insane::Str::Xtring::TrimEnd()
{
	erase(std::find_if(rbegin(), rend(), [](unsigned char ch) {
			  return !std::isspace(ch);
		  }).base(),
		  end());
	return thisvalue;
}

Xtring Insane::Str::Xtring::Trim()
{
	return TrimStart().TrimEnd();
}

Xtring Insane::Str::Xtring::ReplaceLastOf(const String &toFind, const String &toReplace)
{
	size_t pos = rfind(toFind);
	if (pos == std::string::npos)
	{
		return thisvalue;
	}
	replace(pos, toFind.length(), toReplace);
	return thisvalue;
}

Xtring Insane::Str::Xtring::Replace(const String &toFind, const String &toReplace)
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

Xtring Insane::Str::Xtring::Replace(const std::initializer_list<std::pair<ToFind, ToReplace>> &pairs)
{
	for (std::pair<String, String> value : pairs)
	{
		Replace(value.first, value.second);
	}
	return thisvalue;
}

Xtring Insane::Str::Xtring::Remove(const String &toRemove)
{
	size_t pos = 0;
	while ((pos = find(toRemove, pos)) != String::npos)
	{
		erase(pos, toRemove.length());
	}
	return thisvalue;
}
Xtring Insane::Str::Xtring::Remove(const std::initializer_list<String> &toRemove)
{
	for (String value : toRemove)
	{
		Remove(value);
	}
	return thisvalue;
}
Xtring Insane::Str::Xtring::InsertRepeat(size_t distance, const String &toRepeat, bool includeEnd)
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

size_t Insane::Str::Xtring::LengthUTF8() const
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

Xtring Insane::Str::Xtring::ReverseUTF8()
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

Xtring Insane::Str::Xtring::Reverse()
{
	std::reverse(thisvalue.begin(), thisvalue.end());
	return thisvalue;
}

Xtring Insane::Str::Xtring::SubstringUTF8(size_t startIndex, size_t length) const
{
	size_t units = LengthUTF8();
	if (startIndex >= units)
	{
		throw std::invalid_argument(u8"Invalid UTF8 character position.");
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
	auto v7 = 64.0L;
	return Xtring(beginIterator, it);
}

Xtring Insane::Str::Xtring::GetCharUTF8(size_t utf8pos) const
{
	return SubstringUTF8(utf8pos, 1);
}

Xtring Insane::Str::Xtring::PadRight(const size_t &totalWidth, const char &padding)
{
	if (thisvalue.length() >= totalWidth)
	{
		return thisvalue;
	}
	thisvalue.resize(totalWidth, padding);
	return thisvalue;
}

Xtring Insane::Str::Xtring::PadLeft(const size_t &totalWidth, const char &padding)
{
	if (thisvalue.length() >= totalWidth)
	{
		return thisvalue;
	}
	thisvalue.insert(0, totalWidth - thisvalue.length(), padding);
	return thisvalue;
}

Xtring Insane::Str::Xtring::PadRight(const size_t &totalWidth, const Xtring &padding)
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

Xtring Insane::Str::Xtring::PadLeft(const size_t &totalWidth, const Xtring &padding)
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

Xtring Insane::Str::Xtring::PadRightUTF8(const size_t &totalWidth, const char &padding)
{
	size_t totalUtf8 = LengthUTF8();
	if (totalUtf8 >= totalWidth)
	{
		return thisvalue;
	}
	thisvalue.resize(thisvalue.length() + (totalWidth - totalUtf8), padding);
	return thisvalue;
}

Xtring Insane::Str::Xtring::PadLeftUTF8(const size_t &totalWidth, const char &padding)
{
	size_t totalUtf8 = LengthUTF8();
	if (totalUtf8 >= totalWidth)
	{
		return thisvalue;
	}
	thisvalue.insert(0, totalWidth - totalUtf8, padding);
	return thisvalue;
}

Xtring Insane::Str::Xtring::PadRightUTF8(const size_t &totalWidth, const Xtring &padding)
{
	size_t units = LengthUTF8();
	size_t paddingUnits = Strings::LengthUTF8(padding);
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
	thisvalue.append(Strings::SubstringUTF8(padding, 0, modulo));
	return thisvalue;
}

Xtring Insane::Str::Xtring::PadLeftUTF8(const size_t &totalWidth, const Xtring &padding)
{
	size_t units = LengthUTF8();
	size_t paddingUnits = Strings::LengthUTF8(padding);
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
	thisvalue.insert(0, Strings::SubstringUTF8(padding, 0, modulo));
	return thisvalue;
}

WString Insane::Str::Xtring::ToWideString()
{
	return Strings::StringToWideString(thisvalue);
}

Xtring Insane::Str::Xtring::operator=(const WString &wstr)
{
	return Xtring(Strings::WideStringToString(wstr));
}

Xtring Insane::Str::Xtring::ToUpper()
{
	return operator=(Strings::ToUpper(ToWideString()));
}

Xtring Insane::Str::Xtring::ToLower()
{

	return operator=(Strings::ToLower(ToWideString()));
}

std::vector<String> Insane::Str::Xtring::Split(const String &toFind) const
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

bool Insane::Str::Xtring::IsMatch(const String &pattern) const
{
	std::regex regex(pattern, std::regex_constants::ECMAScript);
	return std::regex_match(thisvalue, regex);
}

bool Insane::Str::Xtring::StartsWith(const String &preffix, const bool &caseSensitive) const
{
	if (!caseSensitive)
	{
		String ndata = Strings::ToUpper(thisvalue);
		String npreffix = Strings::ToUpper(preffix);
		return ndata.find(npreffix) == 0;
	}
	return find(preffix) == 0;
}

bool Insane::Str::Xtring::EndsWith(const String &suffix, const bool &caseSensitive) const
{
	if (!caseSensitive)
	{
		String ndata = Strings::ToUpper(thisvalue);
		String nsuffix = Strings::ToUpper(suffix);
		return ndata.find(nsuffix, ndata.size() - nsuffix.size()) != std::string::npos;
	}
	return find(suffix, size() - suffix.size()) != std::string::npos;
}

bool Insane::Str::Xtring::Contains(const String &content, const bool &caseSensitive) const
{
	if (!caseSensitive)
	{
		String ndata = Strings::ToUpper(thisvalue);
		String ncontent = Strings::ToUpper(content);
		return ndata.find(ncontent) != std::string::npos;
	}
	return find(content) != std::string::npos;
	// 	auto a = u8"\u0123"; // UTF-8:  const char[]:     0xC4 0xA3 0x00
	//  const char16_t * b = u"\u0123"; // UTF-16: const char16_t[]: 0x0123 0x0000
	//  auto c = U"\u0123"; // UTF-32: const char32_t[]: 0x00000123 0x00000000
	//   auto d = "\u0123"; // ???:    const char[]:     ???
	//  auto e = L"\u0123"; // ???:    const wchar_t[]:  ???
}