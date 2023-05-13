#pragma once
#ifndef INSANE_CORE_H
#define INSANE_CORE_H
#include <charconv>
#include <bitset>
#include <map>
#include <Insane/Insane.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneException.h>
#include <rapidjson/document.h>

#define CONSOLE_PAUSE_STRING ("Press enter to continue...:"s)
#define CONSOLE_PAUSE_ANY_STRING ("Press any key to continue...:"s)
#define CONSOLE_PAUSE_VALID_LIST { EMPTY_STRING }
#define COLOR_COMPONENT_MIN_VALUE 0
#define COLOR_COMPONENT_MAX_VALUE 255

#define USING_NS_INSANE_CORE using namespace InsaneIO::Insane::Core
namespace InsaneIO::Insane::Core
{
	class INSANE_API RapidJsonExtensions {
	public:
		[[nodiscard]] static String ToJson(const rapidjson::Value& value);
		[[nodiscard]] static String Prettify(const String& json);
		[[nodiscard]] static String GetStringValue(const rapidjson::Value& object, const String& propertyName);
	};

	class INSANE_API IntegralExtensions
	{
	public:
		[[nodiscard]] static std::string ToOctal(const std::integral auto& value)
		{
			const auto size = sizeof(value) * 8;
			std::bitset<size> bits(value);
			auto uc = bits.to_ullong();
			int width = static_cast<int>(std::ceil(static_cast<double>(size) / 3));
			std::ostringstream oss;
			oss << std::oct << std::setfill('0') << std::setw(width) << uc;
			return oss.str();
		}

		[[nodiscard]] static std::string ToHexadecimal(const std::integral auto& value)
		{
			const auto size = sizeof(value) * 8;
			std::bitset<size> bits(value);
			auto uc = bits.to_ullong();
			int width = static_cast<int>(std::ceil(static_cast<double>(size) / 4));
			std::ostringstream oss;
			oss << std::hex << std::setfill('0') << std::setw(width) << uc;
			return oss.str();
		}

		[[nodiscard]] static std::string ToBinary(const std::integral auto& value)
		{
			const auto size = sizeof(value) * 8;
			std::bitset<size> bits(value);
			std::ostringstream oss;
			oss << bits;
			return oss.str();
		}

		static std::string ToString(const std::integral auto& value)
		{
			USING_NS_INSANE_EXCEPTION;
			char buffer[32] = {};
			auto [p, ec] = std::to_chars(buffer, buffer + sizeof(buffer), value);
			if (ec == std::errc{})
			{
				return std::string(buffer, p - buffer);
			}
			throw ConvertException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}

	private:
	};

	class INSANE_API EnumExtensions
	{
	public:
		template <typename EnumType>
		[[nodiscard]] static std::string ToIntegralString(const EnumType& enumValue)
			requires std::is_enum_v<EnumType>
		{
			return IntegralExtensions::ToString(ToIntegral(enumValue));
		}

		template <typename EnumType>
		[[nodiscard]] static std::underlying_type_t<EnumType> ToIntegral(const EnumType& enumValue)
			requires std::is_enum_v<EnumType>
		{
			return static_cast<std::underlying_type_t<EnumType>>(enumValue);
		}

	private:
	};


	INSANE_ENUM(ConsoleTextStyle,
		DEFAULT, EQ, 0,
		BOLD, EQ, 1,
		FAINT, EQ, 2,
		ITALIC, EQ, 3,
		UNDERLINE, EQ, 4,
		SLOW_BLINK, EQ, 5,
		RAPID_BLINK, EQ, 6,
		INVERT, EQ, 7,
		INVISIBLE, EQ, 8,
		STRIKE, EQ, 9);

	/*
	Restablecer todos los estilos: "\033[0m"
Estilo de texto en negrita o intenso: "\033[1m"
Estilo de texto en modo débil o atenuado: "\033[2m"
Estilo de texto subrayado: "\033[4m"
Estilo de texto parpadeante (parpadeo): "\033[5m"
Estilo de texto invertido (cambio de colores de texto y fondo): "\033[7m"
Estilo de texto invisible (ocultar el texto): "\033[8m"
	*/
	INSANE_ENUM(ConsoleForeground,
		DEFAULT, EQ, 39,
		BLACK, EQ, 30,
		DARK_RED, EQ, 31,
		DARK_GREEN, EQ, 32,
		DARK_YELLOW, EQ, 33,
		DARK_BLUE, EQ, 34,
		DARK_MAGENTA, EQ, 35,
		DARK_CYAN, EQ, 36,
		GRAY, EQ, 37,
		DARK_GRAY, EQ, 90,
		RED, EQ, 91,
		GREEN, EQ, 92,
		YELLOW, EQ, 93,
		BLUE, EQ, 94,
		MAGENTA, EQ, 95,
		CYAN, EQ, 96,
		WHITE, EQ, 97);

	INSANE_ENUM(ConsoleBackground,
		DEFAULT, EQ, 49,
		BLACK, EQ, 40,
		DARK_RED, EQ, 41,
		DARK_GREEN, EQ, 42,
		DARK_YELLOW, EQ, 43,
		DARK_BLUE, EQ, 44,
		DARK_MAGENTA, EQ, 45,
		DARK_CYAN, EQ, 46,
		GRAY, EQ, 47,
		DARK_GRAY, EQ, 100,
		RED, EQ, 101,
		GREEN, EQ, 102,
		YELLOW, EQ, 103,
		BLUE, EQ, 104,
		MAGENTA, EQ, 105,
		CYAN, EQ, 106,
		WHITE, EQ, 107);

	template <int Value>
	concept ValidColorComponent = CheckRange<int, Value, COLOR_COMPONENT_MIN_VALUE, COLOR_COMPONENT_MAX_VALUE>;

	// ███ RgbColor ███
	class RgbColor {
	public:
		template <int r , int g, int b> requires ValidColorComponent<r>&& ValidColorComponent<g>&& ValidColorComponent<b>
		static RgbColor Create()
		{
			return RgbColor(r, g, b);
		}

		static RgbColor Create(const int& r, const int& g, const int& b);
		int GetR() const;
		int GetG() const;
		int GetB() const;
	private:
		RgbColor(const int& r, const int& g, const int& b);
		static int ValidateColorComponent(const int& value);
		const int _Red;
		const int _Green;
		const int _Blue;
	};

	// ███ Console ███
	class INSANE_API Console
	{
	public:
		static void Clear();
		template <typename TPrintable>
		static void Write(const TPrintable& data, ConsoleForeground foreground = ConsoleForeground::DEFAULT, ConsoleBackground background = ConsoleBackground::DEFAULT, std::set<ConsoleTextStyle> styles = {}) requires IsPrintable<TPrintable>
		{
			EnableVirtualTermimalProcessing();
			SetVirtualTerminalFormat(foreground, background, styles);
			std::cout << data;
			ResetVirtualTerminalFormat();
		}

		static void Write(const Printable auto& data, RgbColor foreground, RgbColor background, std::set<ConsoleTextStyle> styles = {}) 
		{
			EnableVirtualTermimalProcessing();
			SetVirtualTerminalFormat(foreground, background, styles);
			std::cout << data;
			ResetVirtualTerminalFormat();
		}

		static void WriteLine(const Printable auto& data, RgbColor foreground, RgbColor background, std::set<ConsoleTextStyle> styles = {})
		{
			Write(data, foreground, background, styles);
			std::cout << std::endl;
		}

		static void WriteLine(const Printable auto& data, ConsoleForeground foreground = ConsoleForeground::DEFAULT, ConsoleBackground background = ConsoleBackground::DEFAULT, std::set<ConsoleTextStyle> styles = {})
		{
			Write(data, foreground, background, styles);
			std::cout << std::endl;
		}
		static void WriteLine();
		static void Pause(const String& message = CONSOLE_PAUSE_STRING, const std::set<String>& validInputValues = CONSOLE_PAUSE_VALID_LIST);
		static int PauseAny(const bool& printWhenPressed = false, const String& message = CONSOLE_PAUSE_ANY_STRING);

	private:
		static void EnableVirtualTermimalProcessing();
		static void SetVirtualTerminalFormat(ConsoleForeground foreground, ConsoleBackground background, std::set<ConsoleTextStyle> styles);
		static void SetVirtualTerminalFormat(RgbColor foreground, RgbColor background, std::set<ConsoleTextStyle> styles = {});
		static void ResetVirtualTerminalFormat();
	};


	class INSANE_API DateTimeExtensions
	{
	public:
		[[nodiscard]] static String CurrentISO8601DateTime(bool toUTC = true);
	private:
	};

} // namespace InsaneIO::Insane::Core

#endif //! INSANE_CORE_H