#include <Insane/InsaneCore.h>
#include <Insane/InsaneString.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


String InsaneIO::Insane::Core::DateTimeExtensions::CurrentISO8601DateTime(bool toUTC)
{
	using namespace std::chrono;
	system_clock::time_point now = system_clock::now();
	time_t timet = system_clock::to_time_t(now);
	std::tm tm{};
	String format = String("%FT%T.").append(std::to_string(duration_cast<milliseconds>(now.time_since_epoch()).count() % static_cast<SignedInt64>(1000)));
	if (toUTC)
	{
#ifdef WINDOWS_PLATFORM
		gmtime_s(&tm, &timet);
#elif LINUX_PLATFORM || EMSCRIPTEN_PLATFORM || MACOS_PLATFORM
		gmtime_r(&timet, &tm);
#else
		static_assert(false, "Unknown Platform");
#endif
		format = format.append("Z");
	}
	else
	{
#ifdef WINDOWS_PLATFORM
		localtime_s(&tm, &timet);
#elif LINUX_PLATFORM || EMSCRIPTEN_PLATFORM || MACOS_PLATFORM
		localtime_r(&timet, &tm);
#else
		static_assert(false, "Unknown Platform");
#endif
		format.append("%z");
	}
	String result = String(255, 0);
	const size_t length = std::strftime(&result[0], result.size(), format.c_str(), &tm);
	result.resize(length);
	return result;
}

void InsaneIO::Insane::Core::Console::Clear()
{

#ifdef WINDOWS_PLATFORM
	std::system("cls");
#elif LINUX_PLATFORM || defined MACOS_PLATFORM
	std::system("clear");
#elif EMSCRIPTEN_PLATFORM
	emscripten::val::global()["console"].call<void>("clear");
#else
	static_assert(false, "Unknown Platform");
#endif
}

void InsaneIO::Insane::Core::Console::EnableVirtualTermimalProcessing()
{
#if defined WINDOWS_PLATFORM
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	if (!(dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING))
	{
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
	}
#endif
}

void InsaneIO::Insane::Core::Console::ResetVirtualTerminalFormat()
{
	std::cout << "\033[0m";
}

void InsaneIO::Insane::Core::Console::SetVirtualTerminalFormat(ConsoleForeground foreground, ConsoleBackground background, std::set<ConsoleTextStyle> styles)
{
	String format = "\033[";
	format.append(std::to_string(static_cast<int>(foreground)));
	format.append(";");
	format.append(std::to_string(static_cast<int>(background)));
	if (styles.size() > 0)
	{
		for (auto it = styles.begin(); it != styles.end(); ++it)
		{
			format.append(";");
			format.append(std::to_string(static_cast<int>(*it)));
		}
	}
	format.append("m");
	std::cout << format;
}

void InsaneIO::Insane::Core::Console::WriteLine()
{
	std::cout << std::endl;
}

void InsaneIO::Insane::Core::Console::Pause(const String& message, const std::set<String>& validInputValues)
{
	std::string line; 
	do
	{
		std::cout << message << SPACE_STRING;
		std::getline(std::cin, line);
	} while (!validInputValues.contains(line));
}

int InsaneIO::Insane::Core::Console::PauseAny(const bool& printWhenPressed, const String& message)
{
	std::cout << message << SPACE_STRING;
	int ch;
#ifdef WINDOWS_PLATFORM
	while (!_kbhit()) {
	}
	ch = _getch();
#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#else
	static_assert(false, "Unknown Platform");
#endif
	Console::WriteLine(printWhenPressed? String(1, ch) : EMPTY_STRING); 
	return static_cast<int>(ch);
}

// ███ RapidJsonExtensions ███

String InsaneIO::Insane::Core::RapidJsonExtensions::ToJson(const rapidjson::Value& value)
{
	rapidjson::Document doc;
	doc.CopyFrom(value, doc.GetAllocator());
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	return String(buffer.GetString(), buffer.GetSize());
}

String InsaneIO::Insane::Core::RapidJsonExtensions::Prettify(const String& json)
{
	rapidjson::Document doc;
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	doc.Parse(json.data(), json.length());
	doc.Accept(writer);
	return String(sb.GetString(), sb.GetSize());
}

String InsaneIO::Insane::Core::RapidJsonExtensions::GetStringValue(const rapidjson::Value& object, const String& propertyName)
{
	rapidjson::Document doc;
	doc.CopyFrom(object, doc.GetAllocator());
	return String(doc[propertyName.data()].GetString(), doc[propertyName.data()].GetStringLength());
}

// ███ RgbColor ███

InsaneIO::Insane::Core::RgbColor InsaneIO::Insane::Core::RgbColor::Create(const int& r, const int& g, const int& b)
{
	return RgbColor(r, g, b);
}

int InsaneIO::Insane::Core::RgbColor::GetR() const
{
	return _Red;
}

int InsaneIO::Insane::Core::RgbColor::GetG() const
{
	return _Green;
}

int InsaneIO::Insane::Core::RgbColor::GetB() const
{
	return _Blue;
}

InsaneIO::Insane::Core::RgbColor::RgbColor(const int& r, const int& g, const int& b) : _Red(ValidateColorComponent(r)), _Green(ValidateColorComponent(g)), _Blue(ValidateColorComponent(b)) {

}

int InsaneIO::Insane::Core::RgbColor::ValidateColorComponent(const int& value)
{
	USING_NS_INSANE_EXCEPTION;
	if (!(value >= COLOR_COMPONENT_MIN_VALUE && value <= COLOR_COMPONENT_MAX_VALUE))
	{
		throw ArgumentException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, "Invalid color component range 0 - 255, value = <" + IntegralExtensions::ToString(value) + ">.");
	}
	return value;
}


