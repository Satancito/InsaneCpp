// ███ RapidJsonExtensions ███
#include <Insane/__InsaneCore.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

String InsaneIO::Insane::Internal::Core::RapidJsonExtensions::ToJson(const rapidjson::Value& value)
{
	rapidjson::Document doc;
	doc.CopyFrom(value, doc.GetAllocator());
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	return String(buffer.GetString(), buffer.GetSize());
}

String InsaneIO::Insane::Internal::Core::RapidJsonExtensions::Prettify(const String& json)
{
	rapidjson::Document doc;
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	doc.Parse(json.data(), json.length());
	doc.Accept(writer);
	return String(sb.GetString(), sb.GetSize());
}

String InsaneIO::Insane::Internal::Core::RapidJsonExtensions::GetStringValue(const rapidjson::Value& object, const String& propertyName)
{
	rapidjson::Document doc;
	doc.CopyFrom(object, doc.GetAllocator());
	return String(doc[propertyName.data()].GetString(), doc[propertyName.data()].GetStringLength());
}