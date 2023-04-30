#include <Insane/Insane.h>
#include <Insane/InsaneTest.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>

String InsaneIO::Insane::Test::TestExtensions::GetTestTypeHeader(const TestType &testType)
{
    switch (testType)
    {
    case TestType::Equals:
        return __TEST_HEADER_EQUALS_STRING;
    case TestType::NotEquals:
        return __TEST_HEADER_NOT_EQUALS_STRING;
    default:
        throw InsaneIO::Insane::Exception::NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__,__LINE__, TestTypeEnumExtensions::ToString(testType));
    }
}
