#pragma once
#ifndef INSANE_TEST_H
#define INSANE_TEST_H

#include <Insane/Insane.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneString.h>
#include <unicode/unistr.h>
#include <unicode/ucnv.h>
#include <type_traits>
#include <Insane/InsaneCore.h>
#include <iostream>
#include <map>

#define __TEST_OK_STRING ("Test OK. ✅"s)
#define __TEST_FAILED_STRING ("Test Failed. ❌"s)
#define __TEST_HEADER_STRING ("█████ Test - "s)
#define __TEST_RESULT_LABEL_STRING ("Test result: "s)
#define __TEST_LABEL_EXPECTED_STRING ("Expected:"s)
#define __TEST_LABEL_COMPUTED_STRING ("Computed:"s)
#define __TEST_HEADER_EQUALS_STRING ("Equals"s)
#define __TEST_HEADER_NOT_EQUALS_STRING ("NotEquals"s)

#define USING_NS_INSANE_TEST using namespace InsaneIO::Insane::Test
USING_NS_INSANE;
namespace InsaneIO::Insane::Test
{
	INSANE_ENUM(TestType, Equals, _, _, NotEquals, _, _);

	class TestExtensions
	{
	public:
		template <typename T>
		static void Equals(const String &testLabel, const T &expected, const T &value, const OutFunction<T> &fx, const bool &showValues = true)
			requires IsEqualityComparable<T>
		{
			ShowValues(testLabel, TestType::Equals, expected, value, expected == value, __TEST_OK_STRING, __TEST_FAILED_STRING, fx, showValues);
		}

		template <typename T>
		static void NotEquals(const String &testLabel, const T &expected, const T &value, const OutFunction<T> &fx, const bool &showValues = true)
			requires IsEqualityComparable<T>
		{
			ShowValues(testLabel, TestType::NotEquals, expected, value, expected != value, __TEST_OK_STRING, __TEST_FAILED_STRING, fx, showValues);
		}

	private:
		static String GetTestTypeHeader(const TestType &testType);

		template <typename T>
		static void ShowValues(const String &testLabel, const TestType &testType, const T &expected, const T &value, const bool &testResult, const String &positiveMessage, const String &negativeMessage, const OutFunction<T> &fx, const bool &showValues)
			requires IsEqualityComparable<T>
		{
			USING_NS_INSANE_STR;
			USING_NS_INSANE_CORE;
			std::ostringstream oss;
			std::ostringstream auxOss;
			oss << __TEST_HEADER_STRING + GetTestTypeHeader(testType) + ": " + testLabel << (showValues ? LINE_FEED_STRING : FULL_STOP_STRING + SPACE_STRING);
			if (showValues)
			{
				String openQuote = LESS_THAN_SIGN_STRING;
				String closedQuote = GREATER_THAN_SIGN_STRING;
				bool quotationMarkEnclosed = false;
				if constexpr (std::is_same_v<T, String> || std::is_same_v<T, char[]> || std::is_same_v<T, char *>)
				{
					quotationMarkEnclosed = true;
				}
				fx(expected, auxOss);
				// auxOss << expected;
				bool validUtf8 = StringExtensions::IsValidUTF8(auxOss.str());
				String space = (auxOss.str().find(LINE_FEED_STRING) != std::string::npos && validUtf8 ? LINE_FEED_STRING : SPACE_STRING);
				oss << __TEST_LABEL_EXPECTED_STRING << space << openQuote << StringExtensions::ToCodeLiteral(auxOss.str(), quotationMarkEnclosed) << closedQuote << std::endl;
				oss << EMOJI_SQUARE_VS_STRING << std::endl;
				auxOss.str(EMPTY_STRING);
				fx(value, auxOss);
				// auxOss << value;
				validUtf8 = StringExtensions::IsValidUTF8(auxOss.str());
				space = (auxOss.str().find(LINE_FEED_STRING) != std::string::npos && validUtf8 ? LINE_FEED_STRING : SPACE_STRING);
				oss << __TEST_LABEL_COMPUTED_STRING << space << openQuote << StringExtensions::ToCodeLiteral(auxOss.str(), quotationMarkEnclosed) << closedQuote << std::endl;
				auxOss.clear();
			}
			oss << __TEST_RESULT_LABEL_STRING << (testResult ? TRUE_LITERAL_STRING : FALSE_LITERAL_STRING) << (showValues ? LINE_FEED_STRING : FULL_STOP_STRING + SPACE_STRING);
			if (testResult)
			{
				oss << positiveMessage << std::endl;
			}
			else
			{
				oss << negativeMessage << std::endl;
			}

			oss << std::endl;
			std::cout << oss.str();
		}
	};
}

#endif // !INSANE_TEST_H