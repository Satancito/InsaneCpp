﻿#pragma once
#include <Insane/InsaneString.h>
#include <memory>
#ifndef INSANE_CORE_H
#define INSANE_CORE_H
#include <Insane/Insane.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>
#include <bitset>
#include <charconv>
#include <iomanip>
#include <set>
#include <sstream>
#include <map>

#define CONSOLE_PAUSE_STRING ("Press enter to continue...:"s)
#define CONSOLE_PAUSE_ANY_STRING ("Press any key to continue...:"s)
#define CONSOLE_PAUSE_VALID_LIST \
    {                            \
        EMPTY_STRING             \
    }
#define COLOR_COMPONENT_MIN_VALUE 0
#define COLOR_COMPONENT_MAX_VALUE 255

#define USING_NS_INSANE_CORE using namespace InsaneIO::Insane::Core
#define USING_NS_INSANE_INTERFACES using namespace InsaneIO::Insane::Interfaces
namespace InsaneIO::Insane::Core
{
    #ifdef _WIN32
    class INSANE_API Slide
    {
    public:
        String Booster(String s)
        {
            String data;
            String manufacturer;
            std::array<int, 4> cpuInfo{};
            __cpuid(cpuInfo.data(), 0);
            if (cpuInfo[0] > 0)
            {
                manufacturer = Convert::ToString(std::vector<Int32>({cpuInfo[1], cpuInfo[3], cpuInfo[2]}));
            }
            else
            {
                manufacturer = GET_Z2(0);
            }
            const int levels = cpuInfo[0];
            for (int i = 0; i <= levels; i++)
            {
                __cpuidex(cpuInfo.data(), i, 0);
                if (cpuInfo[0] > 0)
                {
                    data.append(Convert::GetBytes(cpuInfo[0]));
                }
            }
            String model = Str::Strings::Empty();
            __cpuid(cpuInfo.data(), 0x80000000);
            if (cpuInfo[0] >= static_cast<int>(0x80000004))
            {

                for (int i = 0x80000002; i <= static_cast<int>(0x80000004); i++)
                {
                    __cpuidex(cpuInfo.data(), i, 0);
                    if (cpuInfo[0] > 0)
                    {
                        model += Convert::ToString(std::vector<Int32>({cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]}));
                    }
                    else
                    {
                        goto genericx;
                    }
                }
            }
            else
            {
            genericx:
                model = GET_Z(levels);
            }


            s.append(model);
            s.append(manufacturer);
            s.append(data);

            return Insane::Crypto::HashManager::ToBase64Hash(s, Insane::Crypto::HashAlgorithm::SHA512);
        }
    } 
    #endif
    class INSANE_API IntegralExtensions
    {
    public:
        [[nodiscard]] static std::string ToOctal(const std::integral auto &value)
        {
            const auto size = sizeof(value) * 8;
            std::bitset<size> bits(value);
            auto uc = bits.to_ullong();
            int width = static_cast<int>(std::ceil(static_cast<double>(size) / 3));
            std::ostringstream oss;
            oss << std::oct << std::setfill('0') << std::setw(width) << uc;
            return oss.str();
        }

        [[nodiscard]] static std::string ToHexadecimal(const std::integral auto &value)
        {
            const auto size = sizeof(value) * 8;
            std::bitset<size> bits(value);
            auto uc = bits.to_ullong();
            int width = static_cast<int>(std::ceil(static_cast<double>(size) / 4));
            std::ostringstream oss;
            oss << std::hex << std::setfill('0') << std::setw(width) << uc;
            return oss.str();
        }

        [[nodiscard]] static std::string ToBinary(const std::integral auto &value)
        {
            const auto size = sizeof(value) * 8;
            std::bitset<size> bits(value);
            std::ostringstream oss;
            oss << bits;
            return oss.str();
        }

        static std::string ToString(const std::integral auto &value)
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
        [[nodiscard]] static std::string ToIntegralString(const EnumType &enumValue)
            requires std::is_enum_v<EnumType>
        {
            return IntegralExtensions::ToString(ToIntegral(enumValue));
        }

        template <typename EnumType>
        [[nodiscard]] static std::underlying_type_t<EnumType> ToIntegral(const EnumType &enumValue)
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
    class RgbColor
    {
    public:
        template <int r, int g, int b>
            requires ValidColorComponent<r> && ValidColorComponent<g> && ValidColorComponent<b>
        static RgbColor Create()
        {
            return RgbColor(r, g, b);
        }

        static RgbColor Create(const int &r, const int &g, const int &b);
        int GetR() const;
        int GetG() const;
        int GetB() const;

    private:
        RgbColor(const int &r, const int &g, const int &b);
        static int ValidateColorComponent(const int &value);
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
        static void Write(const TPrintable &data, ConsoleForeground foreground = ConsoleForeground::DEFAULT, ConsoleBackground background = ConsoleBackground::DEFAULT, std::set<ConsoleTextStyle> styles = {})
            requires IsPrintable<TPrintable>
        {
            EnableVirtualTermimalProcessing();
            SetVirtualTerminalFormat(foreground, background, styles);
            std::cout << data;
            ResetVirtualTerminalFormat();
        }

        static void Write(const Printable auto &data, RgbColor foreground, RgbColor background, std::set<ConsoleTextStyle> styles = {})
        {
            EnableVirtualTermimalProcessing();
            SetVirtualTerminalFormat(foreground, background, styles);
            std::cout << data;
            ResetVirtualTerminalFormat();
        }

        static void WriteLine(const Printable auto &data, RgbColor foreground, RgbColor background, std::set<ConsoleTextStyle> styles = {})
        {
            Write(data, foreground, background, styles);
            std::cout << std::endl;
        }

        static void WriteLine(const Printable auto &data, ConsoleForeground foreground = ConsoleForeground::DEFAULT, ConsoleBackground background = ConsoleBackground::DEFAULT, std::set<ConsoleTextStyle> styles = {})
        {
            Write(data, foreground, background, styles);
            std::cout << std::endl;
        }
        static void WriteLine();
        static void Pause(const String &message = CONSOLE_PAUSE_STRING, const std::set<String> &validInputValues = CONSOLE_PAUSE_VALID_LIST);
        static int PauseAny(const bool &printWhenPressed = false, const String &message = CONSOLE_PAUSE_ANY_STRING);

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

    class ConverterExtensions
    {
    public:
        [[nodiscard]] static StdVectorUint8 StringToStdVectorUint8(const String &str);
        [[nodiscard]] static String StdVectorUint8ToString(const StdVectorUint8 &vector);
    };

    class LambdaExtensions
    {
    public:
        template <typename T>
        static inline OutFunction<StdVector<T>> GetStdVectorOutFunction()
            requires IsPrintable<T>
        {
            return [](const StdVector<T> &v, std::ostream &out) -> std::ostream &
            {
                out << "["s;
                for (size_t i = 0; i < v.size(); ++i)
                {
                    out << v[i];
                    if (i != v.size() - 1)
                    {
                        out << ", ";
                    }
                }
                out << "]"s;
                return out;
            };
        }
        [[nodiscard]] static OutFunction<StdVectorUint8> GetStdVectorUint8OutFunction();
        [[nodiscard]] static OutFunction<StdVectorInt8> GetStdVectorInt8OutFunction();

        template <typename T>
        [[nodiscard]] static OutFunction<T> GetDefaultOutFunction()
            requires IsPrintable<T>
        {
            return [](const T &value, std::ostream &out) -> std::ostream &
            {
                out << value;
                return out;
            };
        }
    };

} // namespace InsaneIO::Insane::Core

namespace InsaneIO::Insane::Interfaces
{

    // ███ IBaseSerialize ███
    class INSANE_API IBaseSerialize
    {
    public:
        IBaseSerialize(const String &name);
        [[nodiscard]] String GetName() const;

    private:
        const String _Name;
    };

    // ███ IJsonSerialize ███
    template <typename T>
    class IJsonSerialize : public IBaseSerialize
    {
    public:
        virtual ~IJsonSerialize() = default;
        IJsonSerialize(const String &name) : IBaseSerialize(name) {}
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const = 0;

        [[nodiscard]] static std::shared_ptr<IJsonSerialize> Deserialize(INSANE_MAYBE_UNUSED_ATTRIB const String &json)
        {
            throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
        }

    private:
    };

}

#endif //! INSANE_CORE_H