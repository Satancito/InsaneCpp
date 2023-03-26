#include <Insane/Insane.h>
#include <Insane/InsaneTest.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>

String Insane::Test::TestExtensions::GetTestTypeHeader(const TestType &testType)
{
    switch (testType)
    {
    case TestType::Equals:
        return __TEST_HEADER_EQUALS_STRING;
    case TestType::NotEquals:
        return __TEST_HEADER_NOT_EQUALS_STRING;
    default:
        throw Insane::Exception::NotImplementedException(STRINGIFY(Insane::Test::TestExtensions::GetTestTypeHeader - TestType = ) + std::to_string(static_cast<int>(testType)));
    }
}
