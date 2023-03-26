#pragma once
#ifndef INSANE_TEST_H
#define INSANE_TEST_H

#include <Insane/Insane.h>
#include <Insane/InsanePreprocessor.h>
#include <type_traits>

#define __TEST_OK_STRING ("Test OK. ✅"s)
#define __TEST_FAILED_STRING ("Test Failed. ❌"s)
#define __TEST_HEADER_STRING ("█████ Test - "s)
#define __TEST_RESULT_LABEL_STRING ("Test result: "s)
#define __TEST_LABEL_EXPECTED_STRING ("Expected:"s)
#define __TEST_LABEL_COMPUTED_STRING ("Computed:"s)
#define __TEST_HEADER_EQUALS_STRING ("Equals"s)
#define __TEST_HEADER_NOT_EQUALS_STRING ("NotEquals"s)

#define USING_NS_INSANE_TEST using namespace Insane::Test

namespace Insane::Test
{

    enum class TestType
    {
        Equals,
        NotEquals
    };

    class TestExtensions
    {
    public:
        template <typename T>
        static void Equals(const String &testLabel, const T &expected, const T &value, const bool &showValues = true)
            requires Insane::PrintableAndEqualityComparable<T>
        {
            ShowValues(testLabel, TestType::Equals, expected, value, expected == value, __TEST_OK_STRING, __TEST_FAILED_STRING, showValues);
        }

        template <typename T>
        static void NotEquals(const String &testLabel, const T &expected, const T &value, const bool &showValues = true)
            requires Insane::PrintableAndEqualityComparable<T>
        {
            ShowValues(testLabel, TestType::NotEquals, expected, value, expected != value, __TEST_OK_STRING, __TEST_FAILED_STRING, showValues);
        }

    private:
        static String GetTestTypeHeader(const TestType &testType);

        template <typename T>
        static void ShowValues(const String &testLabel, const TestType &testType, const T &expected, const T &value, const bool &testResult, const String &positiveMessage, const String &negativeMessage, const bool &showValues)
            requires Insane::PrintableAndEqualityComparable<T>
        {
            std::ostringstream oss;
            std::ostringstream auxOss;
            oss << __TEST_HEADER_STRING + GetTestTypeHeader(testType) + ": " + testLabel << std::endl;
            String openQuote = "<";
            String closedQuote = ">";
            if constexpr (std::is_same_v<T, String> || std::is_same_v<T, char[]> || std::is_same_v<T, char *>)
            {
                openQuote += QUOTATION_MARK_STRING;
                closedQuote = QUOTATION_MARK_STRING + closedQuote;
            }
            if (showValues)
            {
                auxOss << expected;
                String space = (auxOss.str().find(LINE_FEED_STRING) != std::string::npos ? LINE_FEED_STRING : SPACE_STRING);
                auxOss.clear();
                oss << __TEST_LABEL_EXPECTED_STRING << space << openQuote << expected << closedQuote << std::endl;
                oss << EMOJI_SQUARE_VS_STRING << std::endl;
                auxOss << value;
                space = (auxOss.str().find(LINE_FEED_STRING) != std::string::npos ? LINE_FEED_STRING : SPACE_STRING);
                auxOss.clear();
                oss << __TEST_LABEL_COMPUTED_STRING << space << openQuote << value << closedQuote << std::endl;
            }
            oss << __TEST_RESULT_LABEL_STRING << (testResult ? TRUE_LITERAL_STRING : FALSE_LITERAL_STRING) << std::endl;
            if (testResult)
            {
                oss << positiveMessage << std::endl;
            }
            else
            {
                oss << negativeMessage << std::endl;
            }
            std::cout << oss.str();
        }
    };
}

#endif // !INSANE_TEST_H