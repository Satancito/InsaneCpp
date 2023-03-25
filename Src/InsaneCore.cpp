#include <Insane/InsaneCore.h>
#include <Insane/InsaneString.h>
String Insane::Core::DateTimeManager::CurrentISO8601DateTime(bool toUTC)
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

void Insane::Core::Console::Clear()
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

void Insane::Core::Console::EnableVirtualTermimalProcessing()
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

void Insane::Core::Console::ResetTerminalFormat()
{
	std::cout << "\033[0m";
}

void Insane::Core::Console::SetVirtualTerminalFormat(ConsoleForeground foreground, ConsoleBackground background, std::set<ConsoleTextStyle> styles)
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

void Insane::Core::Console::Write(const String &s, ConsoleForeground foreground, ConsoleBackground background, std::set<ConsoleTextStyle> styles)
{
#ifndef EMSCRIPTEN_PLATFORM
	EnableVirtualTermimalProcessing();
	SetVirtualTerminalFormat(foreground, background, styles);
#endif
	String str = s;
#ifdef WINDOWS_PLATFORM
	WString unicode = Strings::StringToWideString(str);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), unicode.c_str(), static_cast<DWORD>(unicode.length()), nullptr, nullptr);
#elif defined LINUX_PLATFORM || defined MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
	std::cout << str;
#else
	static_assert(false, "Unknown Platform");
#endif

#ifndef EMSCRIPTEN_PLATFORM
	ResetTerminalFormat();
#endif
}

void Insane::Core::Console::WriteLine(const String &s, ConsoleForeground foreground, ConsoleBackground background, std::set<ConsoleTextStyle> styles)
{
	Write(s, foreground, background, styles);
	std::cout << std::endl;
}

void Insane::Core::Console::Write(const WString &s, ConsoleForeground foreground, ConsoleBackground background, std::set<ConsoleTextStyle> styles)
{
	USING_NS_INSANE_STR;
#ifndef EMSCRIPTEN_PLATFORM
	EnableVirtualTermimalProcessing();
	SetVirtualTerminalFormat(foreground, background, styles);
#endif
	WString str = s;

#ifdef WINDOWS_PLATFORM
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), static_cast<DWORD>(str.length()), nullptr, nullptr);
#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
	std::cout << StringExtensions::WideStringToString(str);
#else
	static_assert(false, "Unknown Platform");
#endif

#ifndef EMSCRIPTEN_PLATFORM
	ResetTerminalFormat();
#endif
}

void Insane::Core::Console::WriteLine(const WString &s, ConsoleForeground foreground, ConsoleBackground background, std::set<ConsoleTextStyle> styles)
{
	Write(s, foreground, background, styles);
	std::cout << std::endl;
}

void Insane::Core::Console::WriteLine()
{
	std::cout << std::endl;
}

void Insane::Core::Console::Pause()
{
	char c;
	do
	{
		c = getchar();
		std::cout<< "Press Key " << std::endl;
	} while (c !=  64);
	std::cout<<"KeyPressed"<<std::endl;
}

int Insane::Core::Console::PauseAny(bool printWhenPressed)
{
	int ch;
#ifdef WINDOWS_PLATFORM
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
	if (printWhenPressed)
	{
		Console::Write(String(1, ch));
	}
	return ch;
}