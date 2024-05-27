#ifndef INSANE_INTERNAL_CORE_H
#define INSANE_INTERNAL_CORE_H
#include <Insane/Insane.h>
#include <rapidjson/document.h>

#define USING_NS_INSANE_INTERNAL_CORE using namespace InsaneIO::Insane::Internal::Core
namespace InsaneIO::Insane::Internal::Core
{
	class INSANE_PRIVATE RapidJsonExtensions {
	public:
		[[nodiscard]] static String ToJson(const rapidjson::Value& value);
		[[nodiscard]] static String Prettify(const String& json);
		[[nodiscard]] static String GetStringValue(const rapidjson::Value& object, const String& propertyName);
	};
}
#endif // !INSANE_INTERNAL_CORE_H