#pragma once
#ifndef INSANE_TEST_H
#define INSANE_TEST_H

#define __TEST_OK_STRING ("Test OK. ✅"s)
#define __TEST_FAILED_STRING ("Test Failed. ❌"s)
#define __TEST_HEADER_STRING ("█████ Test - "s)
#define __TEST_RESULT_LABEL_STRING ("Result: "s)
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
        static void Equals(const String &testLabel, const String &expected, const String &value, const bool &showValues = true)
        {
            ShowValues(testLabel, TestType::Equals, expected, value, expected == value, __TEST_OK_STRING, __TEST_FAILED_STRING, showValues);
        }

        static void NotEquals(const String &testLabel, const String &expected, const String &value, const bool &showValues = true)
        {
            ShowValues(testLabel, TestType::NotEquals, expected, value, expected != value, __TEST_OK_STRING, __TEST_FAILED_STRING, showValues);
        }

    private:
        static String GetTestTypeHeader(const TestType &testType);

        static void ShowValues(const String &testLabel, const TestType &testType, const String &expected, const String &value, const bool &testResult, const String &positiveMessage, const String &negativeMessage, const bool &showValues)
        {
            USING_NS_INSANE_STR;
            std::ostringstream oss;

            oss << __TEST_HEADER_STRING + GetTestTypeHeader(testType) + ": " + testLabel << std::endl;
            if (showValues)
            {
                oss << expected << std::endl;
                oss << EMOJI_SQUARE_VS_STRING << std::endl;
                oss << value << std::endl;
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