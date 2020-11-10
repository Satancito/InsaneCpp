#include <Insane/InsaneString.h>

// ███ Strings ███ 

Insane::Str::Strings::Strings()
= default;

Insane::Str::Strings::~Strings()
= default;

String Insane::Str::Strings::Empty()
{
	return EMPTY_STRING;
}

String Insane::Str::Strings::ReplaceAll(const String & data, const String & toFind, const String & toReplace)
{
	String result = data;
	size_t pos = 0;
	while ((pos = result.find(toFind, pos)) != String::npos) {
		result.replace(pos, toFind.length(), toReplace);
		pos += toReplace.length();
		pos = result.find(toFind, pos);
	}
	return result;
}

String Insane::Str::Strings::ReplaceAll(const String & data, const std::initializer_list<std::pair<String, String>> & toFindAndReplace)
{
	String result = data;
	for (std::pair<String, String> value : toFindAndReplace)
	{
		size_t pos = 0;
		while ((pos = result.find(value.first, pos)) != String::npos) {
			result.replace(pos, value.first.length(), value.second);
			pos += value.second.length();
			pos = result.find(value.first, pos);
		}
	}
	return result;
}

String Insane::Str::Strings::RemoveAll(const String & data, const String & toRemove)
{
	String result = data;
	size_t pos = 0;
	while ((pos = result.find(toRemove, pos)) != String::npos) {
		result.erase(pos, toRemove.length());
	}
	return result;
}

String Insane::Str::Strings::RemoveAll(const String & data, const std::initializer_list<String> & toRemove)
{
	String result = data;
	for (String value : toRemove)
	{
		size_t pos = 0;
		while ((pos = result.find(value, pos)) != String::npos) {
			result.erase(pos, value.length());
		}
	}
	return result;
}

String Insane::Str::Strings::InsertRepeat(const String & data, size_t repeatEvery, const String & toRepeat, bool includeEnd)
{
	String result = data;
	if (repeatEvery < 1 || toRepeat.empty() || result.empty() || toRepeat.length() < 1) {
		return result;
	}
	size_t pos = repeatEvery;
	while (pos < result.size()) {
		result.insert(pos, toRepeat);
		pos += repeatEvery + toRepeat.length();
	}
	if (pos == result.length() && includeEnd) {
		result += toRepeat;
	}
	return result;
}

std::vector<String> Insane::Str::Strings::Split(const String & data, const String & toFind)
{
	std::vector<String> v;
	if (data.empty() || toFind.empty()) {
		v.push_back(data);
		return v;
	}
	size_t ini = 0;
	size_t pos;
	while ((pos = data.find(toFind, ini)) != String::npos) {
		String s = data.substr(ini, pos - ini);
		if (!s.empty())
		{
			v.push_back(s);
		}
		ini = pos + 1;
	}
	if (ini < data.length())
	{
		v.push_back(data.substr(ini));
	}

	return v;
}

WString Insane::Str::Strings::ToUpper(const WString & data)
{
	WString result = data;
	auto &f = std::use_facet<std::ctype<wchar_t>>(std::locale());

	f.toupper(&result[0], &result[0] + result.size());
	return result;
}

WString Insane::Str::Strings::ToLower(const WString & data)
{
	WString result = data;
	auto &f = std::use_facet<std::ctype<wchar_t>>(std::locale());
	f.tolower(&result[0], &result[0] + result.size());
	return result;
}

String Insane::Str::Strings::ToUpper(const String & data)
{
	return WideStringToString(ToUpper(StringToWideString(data)));
}

String Insane::Str::Strings::ToLower(const String & data)
{
	return WideStringToString(ToLower(StringToWideString(data)));
}

size_t Insane::Str::Strings::TotalChars(const String & data)
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

String Insane::Str::Strings::Reverse(const String & data, bool asUTF8)
{
	size_t ret = 0;
	String result;
	if (asUTF8)
	{
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
	else
	{
		result = data;
		std::reverse(result.begin(), result.end());
		return result;
	}
}

WString Insane::Str::Strings::EmptyW()
{
	return EMPTY_WSTRING;
}

WString Insane::Str::Strings::ReplaceAll(const WString & data, const WString & toFind, const WString & toReplace)
{
	WString result = data;
	if (result.empty() || toFind.empty())
	{
		return result;
	}

	size_t pos = 0;
	while ((pos = result.find(toFind, pos)) != WString::npos) {
		result.replace(pos, toFind.length(), toReplace);
		pos += toReplace.length();
	}
	return result;
}

WString Insane::Str::Strings::RemoveAll(const WString & data, const WString & toRemove)
{
	WString result = data;
	if (toRemove.empty() || result.empty()) {
		return result;
	}
	size_t pos = 0;
	while ((pos = result.find(toRemove, pos)) != WString::npos) {
		result.erase(pos, toRemove.length());
	}
	return result;
}

WString Insane::Str::Strings::InsertRepeat(const WString & data, size_t repeatEvery, const WString & toRepeat, bool includeEnd)
{
	WString result = data;
	if (repeatEvery < 1 || toRepeat.empty() || result.length() < 1 || toRepeat.length() < 1) {
		return result;
	}
	size_t pos = repeatEvery;
	while (pos < result.size()) {
		result.insert(pos, toRepeat);
		pos += repeatEvery + toRepeat.length();
	}
	if (pos == result.length() && includeEnd) {
		result += toRepeat;
	}
	return result;
}

std::vector<WString> Insane::Str::Strings::Split(const WString & data, const WString & toFind)
{
	std::vector<WString> v;
	if (data.empty() || toFind.empty()) {
		v.push_back(data);
		return v;
	}
	size_t ini = 0;
	size_t pos;
	while ((pos = data.find(toFind, ini)) != WString::npos) {
		WString s = data.substr(ini, pos - ini);
		if (!s.empty())
		{
			v.push_back(s);
		}
		ini = pos + 1;
	}
	if (ini < data.length())
	{
		v.push_back(data.substr(ini));
	}
	return v;
}

String Insane::Str::Strings::WideStringToString(const WString & wstr)
{
	if (wstr.empty())
	{
		return String();
	}
	size_t pos;
	size_t begin = 0;
	String ret;
	size_t size;
#if WINDOWS_PLATFORM
	pos = wstr.find(static_cast<wchar_t>(0), begin);
	while (pos != WString::npos && begin < wstr.length())
	{
		WString segment = WString(&wstr[begin], pos - begin);
		wcstombs_s(&size, nullptr, 0, &segment[0], _TRUNCATE);
		String converted = String(size, 0);
		wcstombs_s(&size, &converted[0], size, &segment[0], _TRUNCATE);
		ret.append(converted);
		begin = pos + 1;
		pos = wstr.find(static_cast<wchar_t>(0), begin);
	}
	if (begin <= wstr.length())
	{
		WString segment = WString(&wstr[begin], wstr.length() - begin);
		wcstombs_s(&size, nullptr, 0, &segment[0], _TRUNCATE);
		String converted = String(size, 0);
		wcstombs_s(&size, &converted[0], size, &segment[0], _TRUNCATE);
		converted.resize(size - 1);
		ret.append(converted);
	}
#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
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
#endif
	return ret;
}

WString Insane::Str::Strings::StringToWideString(const String & str)
{
	if (str.empty())
	{
		return WString();
	}

	size_t pos;
	size_t begin = 0;
	WString ret;
	size_t size;

#ifdef WINDOWS_PLATFORM
	pos = str.find(static_cast<char>(0), begin);
	while (pos != String::npos)
	{
		String segment = String(&str[begin], pos - begin);
		WString converted = WString(segment.size() + 1, 0);

		mbstowcs_s(&size, &converted[0], converted.size(), &segment[0], _TRUNCATE);
		converted.resize(size - 1);
		ret.append(converted);
		ret.append({0});
		begin = pos + 1;
		pos = str.find(static_cast<char>(0), begin);
	}
	if (begin < str.length())
	{
		String segment = String(&str[begin], str.length() - begin);
		WString converted = WString(segment.size() + 1, 0);
		mbstowcs_s(&size, &converted[0], converted.size(), &segment[0], _TRUNCATE);
		converted.resize(size - 1);
		ret.append(converted);
	}
#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
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
#endif
	return ret;
}

bool Insane::Str::Strings::StartsWith(const String & data, const String & preffix, const bool & caseSensitive)
{
	if(!caseSensitive)
	{
		String ndata = Strings::ToUpper(data);
		String npreffix = Strings::ToUpper(preffix);
		return ndata.find(npreffix) == 0;	
	}
	return data.find(preffix) == 0;
}

bool Insane::Str::Strings::EndsWith(const String & data, const String & suffix , const bool & caseSensitive)
{
	if(!caseSensitive)
	{
		String ndata = Strings::ToUpper(data);
		String nsuffix = Strings::ToUpper(suffix);
		return ndata.find(nsuffix, ndata.size() - nsuffix.size()) != std::string::npos;
	}
	return data.find(suffix, data.size() - suffix.size()) != std::string::npos;
}

bool Insane::Str::Strings::Contains(const String & data, const String & content , const bool & caseSensitive)
{
	if(!caseSensitive)
	{
		String ndata = Strings::ToUpper(data);
		String ncontent = Strings::ToUpper(content);
		return ndata.find(ncontent) != std::string::npos;
	}
	return data.find(content) != std::string::npos;
}

