#include <Insane/InsaneCore.h>

String Insane::Core::DateTimeManager::CurrentISO8601DateTime(bool toUTC)
{
	using namespace std::chrono;
	system_clock::time_point now = system_clock::now();
	time_t timet = system_clock::to_time_t(now);
	std::tm tm{};
	char * localeStr = setlocale(LC_ALL, nullptr);
	setlocale(LC_ALL, EMPTY_STR);
	String format = String(u8"%FT%T.").append(std::to_string(duration_cast<milliseconds>(now.time_since_epoch()).count() % static_cast<Int64>(1000)));
	if (toUTC)
	{
#ifdef WINDOWSLIB
		gmtime_s(&tm, &timet);
#elif LINUXLIB
		gmtime_r(&timet, &tm);
#elif EMSCRIPTENLIB
		gmtime_r(&timet, &tm);
#elif MACOSLIB
#endif
		format = format.append(u8"Z");
	}
	else
	{
#ifdef WINDOWSLIB
		localtime_s(&tm, &timet);
#elif LINUXLIB || EMSCRIPTENLIB
		localtime_r(&timet, &tm);
#elif EMSCRIPTENLIB
		localtime_r(&timet, &tm);
#elif MACOSLIB
#endif
		format.append(u8"%z");
	}
	String result = String(255, 0);
	const size_t length = std::strftime(&result[0], result.size(), format.c_str(), &tm);
	result.resize(length);
	setlocale(LC_ALL, localeStr);
	return result;
}
