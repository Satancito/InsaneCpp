#include <Insane/Insane.h>
#include <Insane/InsaneCore.h>
#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneString.h>
#include <Insane/InsaneTest.h>
#include <Insane/__InsaneCore.h>

#include <botan/auto_rng.h>
#include <botan/base32.h>
#include <botan/base64.h>
#include <botan/bigint.h>
#include <botan/block_cipher.h>
#include <botan/cipher_mode.h>
#include <botan/data_src.h>
#include <botan/exceptn.h>
#include <botan/filters.h>
#include <botan/hash.h>
#include <botan/hex.h>
#include <botan/pkcs8.h>
#include <botan/pubkey.h>
#include <botan/pwdhash.h>
#include <botan/rsa.h>
#include <botan/x509_key.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <stdint.h>
#include <string_view>
#include <unicode/regex.h>

#include <cmath>
#include <memory>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

USING_NS_INSANE_CRYPTO;
USING_NS_INSANE_CORE;
USING_NS_INSANE_STR;
USING_NS_INSANE_INTERNAL_CORE;
USING_NS_INSANE_TEST;

// MARK: ███ HexEncodingExtensions ███

StdVectorUint8 HexEncodingExtensions::DecodeFromHex(const String &data)
{
    return Botan::hex_decode(data);
}

String HexEncodingExtensions::EncodeToHex(const StdVectorUint8 &data, const bool &toUpper)
{
    return Botan::hex_encode(data, toUpper);
}

String HexEncodingExtensions::EncodeToHex(const String &data, const bool &toUpper)
{
    return EncodeToHex(ConverterExtensions::StringToStdVectorUint8(data), toUpper);
}

// MARK: ███ Base32EncodingExtensions ███

StdVectorUint8 Base32EncodingExtensions::DecodeFromBase32(const String &data)
{
    Botan::SecureVector<uint8_t> decoded = Botan::base32_decode(StringExtensions::ToUpper(data));
    return StdVectorUint8(decoded.begin(), decoded.end());
}

String Base32EncodingExtensions::EncodeToBase32(const StdVectorUint8 &data, const bool &removePadding, const bool &toLower)
{
    String encoded = Botan::base32_encode(data);
    encoded = removePadding ? StringExtensions::Replace(encoded, EQUAL_SIGN_STRING, EMPTY_STRING) : encoded;
    return toLower ? StringExtensions::ToLower(encoded) : encoded;
}

String InsaneIO::Insane::Cryptography::Base32EncodingExtensions::EncodeToBase32(const String &data, const bool &removePadding, const bool &toLower)
{
    return EncodeToBase32(ConverterExtensions::StringToStdVectorUint8(data), removePadding, toLower);
}

// MARK: ███ Base64EncodingExtensions ███
StdVectorUint8 Base64EncodingExtensions::DecodeFromBase64(const String &data)
{
    String base64 = data;
    base64 = StringExtensions::Replace(base64, {{URL_ENCODED_PLUS_SIGN_STRING, PLUS_SIGN_STRING}, {URL_ENCODED_SLASH_STRING, SLASH_STRING}, {URL_ENCODED_EQUAL_SIGN_STRING, EQUAL_SIGN_STRING}, {MINUS_SIGN_STRING, PLUS_SIGN_STRING}, {UNDERSCORE_STRING, SLASH_STRING}});
    base64 = StringExtensions::RemoveBlankSpaces(base64);
    size_t modulo = base64.length() % 4;
    base64 = StringExtensions::PadRight(base64, base64.length() + (modulo > 0 ? 4 - modulo : 0), EQUAL_SIGN_STRING);
    Botan::secure_vector<uint8_t> result = Botan::base64_decode(base64);
    return StdVectorUint8(result.begin(), result.end());
}

String Base64EncodingExtensions::EncodeToBase64(const StdVectorUint8 &data, const size_t &lineBreaksLength, const bool &removePadding)
{
    String ret = Botan::base64_encode(data);
    if (lineBreaksLength > 0)
    {
        ret = StringExtensions::InsertRepeat(ret, lineBreaksLength, LINE_FEED_STRING);
    }
    return removePadding ? StringExtensions::Remove(ret, EQUAL_SIGN_STRING) : ret;
}

String Base64EncodingExtensions::EncodeToBase64(const String &data, const size_t &lineBreaksLength, const bool &removePadding)
{
    return EncodeToBase64(ConverterExtensions::StringToStdVectorUint8(data), lineBreaksLength, removePadding);
}

String Base64EncodingExtensions::EncodeToUrlSafeBase64(const StdVectorUint8 &data)
{
    return StringExtensions::Replace(EncodeToBase64(data),
                                     {{PLUS_SIGN_STRING, MINUS_SIGN_STRING},
                                      {SLASH_STRING, UNDERSCORE_STRING},
                                      {EQUAL_SIGN_STRING, EMPTY_STRING}});
}

String Base64EncodingExtensions::EncodeToUrlSafeBase64(const String &data)
{
    return EncodeToUrlSafeBase64(ConverterExtensions::StringToStdVectorUint8(data));
}

String Base64EncodingExtensions::EncodeToFilenameSafeBase64(const StdVectorUint8 &data)
{
    return EncodeToUrlSafeBase64(data);
}

String Base64EncodingExtensions::EncodeToFilenameSafeBase64(const String &data)
{
    return EncodeToUrlSafeBase64(data);
}

String Base64EncodingExtensions::EncodeToUrlEncodedBase64(const StdVectorUint8 &data)
{
    return StringExtensions::Replace(
        EncodeToBase64(data),
        {{PLUS_SIGN_STRING, URL_ENCODED_PLUS_SIGN_STRING},
         {SLASH_STRING, URL_ENCODED_SLASH_STRING},
         {EQUAL_SIGN_STRING, URL_ENCODED_EQUAL_SIGN_STRING}});
}

String Base64EncodingExtensions::EncodeToUrlEncodedBase64(const String &data)
{
    return EncodeToUrlEncodedBase64(ConverterExtensions::StringToStdVectorUint8(data));
}

String
Base64EncodingExtensions::EncodeBase64ToUrlSafeBase64(const String &base64)
{
    return EncodeToUrlSafeBase64(DecodeFromBase64(base64));
}

String Base64EncodingExtensions::EncodeBase64ToFilenameSafeBase64(const String &base64)
{
    return EncodeToFilenameSafeBase64(DecodeFromBase64(base64));
}

String
Base64EncodingExtensions::EncodeBase64ToUrlEncodedBase64(const String &base64)
{
    return EncodeToUrlEncodedBase64(DecodeFromBase64(base64));
}

// MARK: ███ IEncoder ███

IEncoder::IEncoder(const String &name) : IJsonSerialize(name) {}

std::unique_ptr<IEncoder> IEncoder::Deserialize(INSANE_MAYBE_UNUSED_ATTRIB const String &json)
{
    throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
}

const std::unique_ptr<IEncoder> IEncoder::DefaultInstance()
{
    throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
}

// MARK: ███ HexEncoder ███

HexEncoder::HexEncoder(const bool &toUpper) : IEncoder(HEX_ENCODER_NAME_STRING), ToUpper(toUpper)
{
}

bool HexEncoder::GetToUpper() const
{
    return ToUpper;
}

String HexEncoder::Encode(const StdVectorUint8 &data) const
{
    return HexEncodingExtensions::EncodeToHex(data, ToUpper);
}

String HexEncoder::Encode(const String &data) const
{
    return Encode(ConverterExtensions::StringToStdVectorUint8(data));
}

StdVectorUint8 HexEncoder::Decode(const String &data) const
{
    return HexEncodingExtensions::DecodeFromHex(data);
}

String HexEncoder::Serialize(const bool &indent) const
{
    try
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

        String number;
        writer.StartObject();

        writer.Key(CNAMEOF(Name));
        writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

        writer.Key(CNAMEOF(ToUpper));
        writer.Bool(ToUpper);

        writer.EndObject();
        String json = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(json) : json;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IEncoder> HexEncoder::DefaultInstance()
{
    return std::make_unique<HexEncoder>();
}

std::unique_ptr<IEncoder> HexEncoder::Clone() const
{
    return std::make_unique<HexEncoder>(*this);
}

std::unique_ptr<IEncoder> HexEncoder::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() ||
            !(document.IsObject() && document.HasMember(CSTRINGIFY(ToUpper))))
        {
            throw true;
        }
        return std::make_unique<HexEncoder>(document[CNAMEOF(ToUpper)].GetBool());
    }
    catch (...)
    {
        throw InsaneIO::Insane::Exception::DeserializeException(
            INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

UniquePtrIEncoder InsaneIO::Insane::Cryptography::HexEncoder::CreateInstance(const bool &toUpper)
{
    return std::make_unique<HexEncoder>(toUpper);
}

// MARK: ███ Base32Encoder ███

Base32Encoder::Base32Encoder(const bool &removePadding, const bool &toLower)
    : IEncoder(BASE32_ENCODER_NAME_STRING), RemovePadding(removePadding), ToLower(toLower)
{
}

size_t Base32Encoder::GetToLower() const
{
    return ToLower;
}

bool Base32Encoder::GetRemovePadding() const
{
    return RemovePadding;
}

String Base32Encoder::Encode(const StdVectorUint8 &data) const
{
    return Base32EncodingExtensions::EncodeToBase32(data, RemovePadding, ToLower);
}

String Base32Encoder::Encode(const String &data) const
{
    return Encode(ConverterExtensions::StringToStdVectorUint8(data));
}

StdVectorUint8 Base32Encoder::Decode(const String &data) const
{
    return Base32EncodingExtensions::DecodeFromBase32(data);
}

String Base32Encoder::Serialize(const bool &indent) const
{
    try
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

        String number;
        writer.StartObject();

        writer.Key(CNAMEOF(Name));
        writer.String(GetName().c_str(),
                      static_cast<rapidjson::SizeType>(GetName().length()));

        writer.Key(CNAMEOF(RemovePadding));
        writer.Bool(RemovePadding);

        writer.Key(CNAMEOF(ToLower));
        writer.Bool(ToLower);

        writer.EndObject();
        String json = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(json) : json;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IEncoder> Base32Encoder::Clone() const
{
    return (std::make_unique<Base32Encoder>(*this));
}

std::unique_ptr<IEncoder> Base32Encoder::DefaultInstance()
{
    return std::make_unique<Base32Encoder>();
}

std::unique_ptr<IEncoder> Base32Encoder::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() ||
            !(document.IsObject() && document.HasMember(CSTRINGIFY(ToLower)) &&
              document.HasMember(CSTRINGIFY(RemovePadding))))
        {
            throw true;
        }
        return std::make_unique<Base32Encoder>(
            document[CNAMEOF(RemovePadding)].GetBool(),
            document[CNAMEOF(ToLower)].GetBool());
    }
    catch (...)
    {
        throw InsaneIO::Insane::Exception::DeserializeException(
            INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

UniquePtrIEncoder InsaneIO::Insane::Cryptography::Base32Encoder::CreateInstance(const bool &removePadding, const bool &toLower)
{
    return std::make_unique<Base32Encoder>(removePadding, toLower);
}

// MARK: ███ Base64Encoder ███

Base64Encoder::Base64Encoder(const size_t &lineBreaksLength, const bool &removePadding, const Base64Encoding &encodingType) : IEncoder(BASE64_ENCODER_NAME_STRING), LineBreaksLength(lineBreaksLength), RemovePadding(removePadding), EncodingType(encodingType)
{
}

size_t Base64Encoder::GetLineBreaksLength() const
{
    return LineBreaksLength;
}

bool Base64Encoder::GetRemovePadding() const
{
    return RemovePadding;
}

Base64Encoding Base64Encoder::GetEncodingType() const
{
    return EncodingType;
}

String Base64Encoder::Encode(const StdVectorUint8 &data) const
{
    switch (EncodingType)
    {
        case Base64Encoding::Base64:
            return Base64EncodingExtensions::EncodeToBase64(data, LineBreaksLength, RemovePadding);
        case Base64Encoding::UrlSafeBase64:
            return Base64EncodingExtensions::EncodeToUrlSafeBase64(data);
        case Base64Encoding::FileNameSafeBase64:
            return Base64EncodingExtensions::EncodeToFilenameSafeBase64(data);
        case Base64Encoding::UrlEncodedBase64:
            return Base64EncodingExtensions::EncodeToUrlEncodedBase64(data);
        default:
            throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, Base64EncodingEnumExtensions::ToString(EncodingType, true));
    }
}

String Base64Encoder::Encode(const String &data) const
{
    return Encode(ConverterExtensions::StringToStdVectorUint8(data));
}

StdVectorUint8 Base64Encoder::Decode(const String &data) const
{
    return Base64EncodingExtensions::DecodeFromBase64(data);
}

String Base64Encoder::Serialize(const bool &indent) const
{
    try
    {
        using namespace rapidjson;
        rapidjson::StringBuffer sb;
        Writer<StringBuffer> writer(sb);

        String number;
        writer.StartObject();

        writer.Key(CNAMEOF(Name));
        writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

        writer.Key(CNAMEOF(LineBreaksLength));
        writer.Uint64(LineBreaksLength);

        writer.Key(CNAMEOF(RemovePadding));
        writer.Bool(RemovePadding);

        writer.Key(CNAMEOF(EncodingType));
        writer.Int(Base64EncodingEnumExtensions::ToIntegral(EncodingType));

        writer.EndObject();
        auto result = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(result) : result;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IEncoder> Base64Encoder::Clone() const
{
    return std::make_unique<Base64Encoder>(*this);
}

std::unique_ptr<IEncoder> Base64Encoder::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() ||
            !(document.IsObject() &&
              document.HasMember(CSTRINGIFY(LineBreaksLength)) &&
              document.HasMember(CSTRINGIFY(RemovePadding)) &&
              document.HasMember(CSTRINGIFY(EncodingType))))
        {
            throw true;
        }
        return std::make_unique<Base64Encoder>(
            document[CNAMEOF(LineBreaksLength)].GetUint(),
            document[CNAMEOF(RemovePadding)].GetBool(),
            Base64EncodingEnumExtensions::Parse(
                document[CNAMEOF(EncodingType)].GetInt()));
    }
    catch (...)
    {
        throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IEncoder> Base64Encoder::DefaultInstance()
{
    return std::make_unique<Base64Encoder>();
}

UniquePtrIEncoder Base64Encoder::CreateInstance(const size_t &lineBreaksLength, const bool &removePadding, const Base64Encoding &encodingType)
{
    return std::make_unique<Base64Encoder>(lineBreaksLength, removePadding, encodingType);
}

// MARK: ███ HashExtensions ███

StdVectorUint8 HashExtensions::ComputeHash(const StdVectorUint8 &data, const HashAlgorithm &algorithm)
{
    switch (algorithm)
    {
        case HashAlgorithm::Md5:
        {
            std::unique_ptr<Botan::HashFunction> hash = Botan::HashFunction::create(MD5_ALGORITHM_NAME_STRING);
            hash->update(data);
            Botan::SecureVector<uint8_t> result = hash->final();
            return StdVectorUint8(result.begin(), result.end());
        }
        case HashAlgorithm::Sha1:
        {
            std::unique_ptr<Botan::HashFunction> hash = Botan::HashFunction::create(SHA1_ALGORITHM_NAME_STRING);
            hash->update(data);
            Botan::SecureVector<uint8_t> result = hash->final();
            return StdVectorUint8(result.begin(), result.end());
        }
        case HashAlgorithm::Sha256:
        {
            std::unique_ptr<Botan::HashFunction> hash = Botan::HashFunction::create_or_throw(SHA256_ALGORITHM_NAME_STRING);
            hash->update(data);
            Botan::SecureVector<uint8_t> result = hash->final();
            return StdVectorUint8(result.begin(), result.end());
        }
        case HashAlgorithm::Sha384:
        {
            std::unique_ptr<Botan::HashFunction> hash = Botan::HashFunction::create(SHA384_ALGORITHM_NAME_STRING);
            hash->update(data);
            Botan::SecureVector<uint8_t> result = hash->final();
            return StdVectorUint8(result.begin(), result.end());
        }
        case HashAlgorithm::Sha512:
        {
            std::unique_ptr<Botan::HashFunction> hash = Botan::HashFunction::create(SHA512_ALGORITHM_NAME_STRING);
            hash->update(data);
            Botan::SecureVector<uint8_t> result = hash->final();
            return StdVectorUint8(result.begin(), result.end());
        }
        default:
            throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, HashAlgorithmEnumExtensions::ToString(algorithm));
    }
}

StdVectorUint8 HashExtensions::ComputeHash(const String &data, const HashAlgorithm &algorithm)
{
    return ComputeHash(ConverterExtensions::StringToStdVectorUint8(data), algorithm);
}

String HashExtensions::ComputeEncodedHash(const StdVectorUint8 &data, std::unique_ptr<IEncoder> &&encoder, const HashAlgorithm &algorithm)
{
    return encoder->Encode(ComputeHash(data, algorithm));
}

String HashExtensions::ComputeEncodedHash(const String &data, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return encoder->Encode(ComputeHash(data, algorithm));
}

bool HashExtensions::VerifyHash(const StdVectorUint8 &data, const StdVectorUint8 expected, const HashAlgorithm &algorithm)
{
    return ComputeHash(data, algorithm) == expected;
}

bool HashExtensions::VerifyHash(const String &data, const StdVectorUint8 expected, const HashAlgorithm &algorithm)
{
    return ComputeHash(data, algorithm) == expected;
}

bool HashExtensions::VerifyEncodedHash(const StdVectorUint8 &data, const String expected, std::unique_ptr<IEncoder> &&encoder, const HashAlgorithm &algorithm)
{
    return ComputeEncodedHash(data, std::move(encoder), algorithm) == expected;
}

bool HashExtensions::VerifyEncodedHash(const String &data, const String expected, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return ComputeEncodedHash(data, std::move(encoder), algorithm) == expected;
}

StdVectorUint8 HashExtensions::ComputeHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, const HashAlgorithm &algorithm)
{
    size_t blockSize = 0;
    StdVectorUint8 secret = key;
    switch (algorithm)
    {
        case HashAlgorithm::Md5:
            [[fallthrough]];
        case HashAlgorithm::Sha1:
            [[fallthrough]];
        case HashAlgorithm::Sha256:
            blockSize = HMAC_64_BYTES_BLOCK_SIZE;
            break;
        case HashAlgorithm::Sha384:
            [[fallthrough]];
        case HashAlgorithm::Sha512:
            blockSize = HMAC_128_BYTES_BLOCK_SIZE;
            break;
        default:
            throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, HashAlgorithmEnumExtensions::ToString(algorithm));
    }

    if (secret.size() > blockSize)
    {
        secret = ComputeHash(secret, algorithm);
    }

    if (secret.size() < blockSize)
    {
        secret.insert(secret.end(), blockSize - secret.size(), static_cast<uint8_t>(0));
    }
    StdVectorUint8 outerKeyPadding = StdVectorUint8(blockSize, HMAC_OUTER_PADDING);
    StdVectorUint8 innerKeyPadding = StdVectorUint8(blockSize, HMAC_INNER_PADDING);
    for (size_t i = 0; i < blockSize; i++)
    {
        innerKeyPadding[i] = (uint8_t)(secret[i] ^ innerKeyPadding[i]);
        outerKeyPadding[i] = (uint8_t)(secret[i] ^ outerKeyPadding[i]);
    }
    innerKeyPadding.insert(innerKeyPadding.end(), data.begin(), data.end());
    StdVectorUint8 ret = ComputeHash(innerKeyPadding, algorithm);
    outerKeyPadding.insert(outerKeyPadding.end(), ret.begin(), ret.end());
    return ComputeHash(outerKeyPadding, algorithm);
}

StdVectorUint8 HashExtensions::ComputeHmac(const String &data, const String &key, const HashAlgorithm &algorithm)
{
    return ComputeHmac(ConverterExtensions::StringToStdVectorUint8(data), ConverterExtensions::StringToStdVectorUint8(key), algorithm);
}

StdVectorUint8 HashExtensions::ComputeHmac(const StdVectorUint8 &data, const String &key, const HashAlgorithm &algorithm)
{
    return ComputeHmac(data, ConverterExtensions::StringToStdVectorUint8(key), algorithm);
}

StdVectorUint8 HashExtensions::ComputeHmac(const String &data, const StdVectorUint8 &key, const HashAlgorithm &algorithm)
{
    return ComputeHmac(ConverterExtensions::StringToStdVectorUint8(data), key, algorithm);
}

String HashExtensions::ComputeEncodedHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return encoder->Encode(ComputeHmac(data, key, algorithm));
}

String HashExtensions::ComputeEncodedHmac(const String &data, const String &key, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return encoder->Encode(ComputeHmac(data, key, algorithm));
}

String HashExtensions::ComputeEncodedHmac(const StdVectorUint8 &data, const String &key, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return encoder->Encode(ComputeHmac(data, key, algorithm));
}

String HashExtensions::ComputeEncodedHmac(const String &data, const StdVectorUint8 &key, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return encoder->Encode(ComputeHmac(data, key, algorithm));
}

bool HashExtensions::VerifyHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, const StdVectorUint8 &expected, const HashAlgorithm &algorithm)
{
    return ComputeHmac(data, key, algorithm) == expected;
}

bool HashExtensions::VerifyHmac(const String &data, const String &key, const StdVectorUint8 &expected, const HashAlgorithm &algorithm)
{
    return ComputeHmac(data, key, algorithm) == expected;
}

bool HashExtensions::VerifyHmac(const StdVectorUint8 &data, const String &key, const StdVectorUint8 &expected, const HashAlgorithm &algorithm)
{
    return ComputeHmac(data, key, algorithm) == expected;
}

bool HashExtensions::VerifyHmac(const String &data, const StdVectorUint8 &key, const StdVectorUint8 &expected, const HashAlgorithm &algorithm)
{
    return ComputeHmac(data, key, algorithm) == expected;
}

bool HashExtensions::VerifyEncodedHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, const String &expected, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return ComputeEncodedHmac(data, key, std::move(encoder), algorithm) == expected;
}

bool HashExtensions::VerifyEncodedHmac(const String &data, const String &key, const String &expected, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return ComputeEncodedHmac(data, key, std::move(encoder), algorithm) == expected;
}

bool HashExtensions::VerifyEncodedHmac(const StdVectorUint8 &data, const String &key, const String &expected, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return ComputeEncodedHmac(data, key, std::move(encoder), algorithm) == expected;
}

bool HashExtensions::VerifyEncodedHmac(const String &data, const StdVectorUint8 &key, const String &expected, UniquePtrIEncoder &&encoder, const HashAlgorithm &algorithm)
{
    return ComputeEncodedHmac(data, key, std::move(encoder), algorithm) == expected;
}

StdVectorUint8 HashExtensions::ComputeArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    try
    {
        std::unique_ptr<Botan::PasswordHashFamily> family = Botan::PasswordHashFamily::create(Argon2VariantEnumExtensions::ToString(variant));
        std::unique_ptr<Botan::PasswordHash> hash = family->from_params(memorySizeKiB, iterations, parallelism);
        std::vector<uint8_t> out(derivedKeyLength);
        hash->derive_key(out.data(), out.size(), reinterpret_cast<const char *>(data.data()), data.size(), salt.data(), salt.size());
        return out;
    }
    catch (...)
    {
        throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

StdVectorUint8 HashExtensions::ComputeArgon2(const String &data, const String &salt, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeArgon2(ConverterExtensions::StringToStdVectorUint8(data), ConverterExtensions::StringToStdVectorUint8(salt), iterations, memorySizeKiB, parallelism, variant, derivedKeyLength);
}

StdVectorUint8 HashExtensions::ComputeArgon2(const StdVectorUint8 &data, const String &salt, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeArgon2(data, ConverterExtensions::StringToStdVectorUint8(salt), iterations, memorySizeKiB, parallelism, variant, derivedKeyLength);
}

StdVectorUint8 HashExtensions::ComputeArgon2(const String &data, const StdVectorUint8 &salt, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeArgon2(ConverterExtensions::StringToStdVectorUint8(data), salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength);
}

String HashExtensions::ComputeEncodedArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return encoder->Encode(ComputeArgon2(data, salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength));
}

String HashExtensions::ComputeEncodedArgon2(const String &data, const String &salt, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return encoder->Encode(ComputeArgon2(data, salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength));
}

String HashExtensions::ComputeEncodedArgon2(const StdVectorUint8 &data, const String &salt, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return encoder->Encode(ComputeArgon2(data, salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength));
}

String HashExtensions::ComputeEncodedArgon2(const String &data, const StdVectorUint8 &salt, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return encoder->Encode(ComputeArgon2(data, salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength));
}

bool HashExtensions::VerifyArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, const StdVectorUint8 &expected, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeArgon2(data, salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyArgon2(const String &data, const String &salt, const StdVectorUint8 &expected, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeArgon2(data, salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyArgon2(const StdVectorUint8 &data, const String &salt, const StdVectorUint8 &expected, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeArgon2(data, salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyArgon2(const String &data, const StdVectorUint8 &salt, const StdVectorUint8 &expected, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeArgon2(data, salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyEncodedArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, const String &expected, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeEncodedArgon2(data, salt, std::move(encoder), iterations, memorySizeKiB, parallelism, variant, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyEncodedArgon2(const String &data, const String &salt, const String &expected, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeEncodedArgon2(data, salt, std::move(encoder), iterations, memorySizeKiB, parallelism, variant, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyEncodedArgon2(const StdVectorUint8 &data, const String &salt, const String &expected, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeEncodedArgon2(data, salt, std::move(encoder), iterations, memorySizeKiB, parallelism, variant, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyEncodedArgon2(const String &data, const StdVectorUint8 &salt, const String &expected, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
{
    return ComputeEncodedArgon2(data, salt, std::move(encoder), iterations, memorySizeKiB, parallelism, variant, derivedKeyLength) == expected;
}

StdVectorUint8 HashExtensions::ComputeScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    try
    {
        std::unique_ptr<Botan::PasswordHashFamily> family = Botan::PasswordHashFamily::create("Scrypt");
        std::unique_ptr<Botan::PasswordHash> hash = family->from_params(iterations, blockSize, parallelism);
        std::vector<uint8_t> out(derivedKeyLength);
        hash->derive_key(out.data(), out.size(), reinterpret_cast<const char *>(data.data()), data.size(), salt.data(), salt.size());
        return out;
    }
    catch (...)
    {
        throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

StdVectorUint8 HashExtensions::ComputeScrypt(const String &data, const String &salt, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeScrypt(ConverterExtensions::StringToStdVectorUint8(data), ConverterExtensions::StringToStdVectorUint8(salt), iterations, blockSize, parallelism, derivedKeyLength);
}

StdVectorUint8 HashExtensions::ComputeScrypt(const StdVectorUint8 &data, const String &salt, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeScrypt(data, ConverterExtensions::StringToStdVectorUint8(salt), iterations, blockSize, parallelism, derivedKeyLength);
}

StdVectorUint8 HashExtensions::ComputeScrypt(const String &data, const StdVectorUint8 &salt, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeScrypt(ConverterExtensions::StringToStdVectorUint8(data), salt, iterations, blockSize, parallelism, derivedKeyLength);
}

String HashExtensions::ComputeEncodedScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return encoder->Encode(ComputeScrypt(data, salt, iterations, blockSize, parallelism, derivedKeyLength));
}

String HashExtensions::ComputeEncodedScrypt(const String &data, const String &salt, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return encoder->Encode(ComputeScrypt(data, salt, iterations, blockSize, parallelism, derivedKeyLength));
}

String HashExtensions::ComputeEncodedScrypt(const StdVectorUint8 &data, const String &salt, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return encoder->Encode(ComputeScrypt(data, salt, iterations, blockSize, parallelism, derivedKeyLength));
}

String HashExtensions::ComputeEncodedScrypt(const String &data, const StdVectorUint8 &salt, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return encoder->Encode(ComputeScrypt(data, salt, iterations, blockSize, parallelism, derivedKeyLength));
}

bool HashExtensions::VerifyScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, const StdVectorUint8 &expected, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeScrypt(data, salt, iterations, blockSize, parallelism, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyScrypt(const String &data, const String &salt, const StdVectorUint8 &expected, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeScrypt(data, salt, iterations, blockSize, parallelism, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyScrypt(const StdVectorUint8 &data, const String &salt, const StdVectorUint8 &expected, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeScrypt(data, salt, iterations, blockSize, parallelism, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyScrypt(const String &data, const StdVectorUint8 &salt, const StdVectorUint8 &expected, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeScrypt(data, salt, iterations, blockSize, parallelism, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyEncodedScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, const String &expected, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeEncodedScrypt(data, salt, std::move(encoder), iterations, blockSize, parallelism, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyEncodedScrypt(const String &data, const String &salt, const String &expected, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeEncodedScrypt(data, salt, std::move(encoder), iterations, blockSize, parallelism, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyEncodedScrypt(const StdVectorUint8 &data, const String &salt, const String &expected, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeEncodedScrypt(data, salt, std::move(encoder), iterations, blockSize, parallelism, derivedKeyLength) == expected;
}

bool HashExtensions::VerifyEncodedScrypt(const String &data, const StdVectorUint8 &salt, const String &expected, UniquePtrIEncoder &&encoder, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
{
    return ComputeEncodedScrypt(data, salt, std::move(encoder), iterations, blockSize, parallelism, derivedKeyLength) == expected;
}

// MARK: ███ DefaultEncoderFunctions ███

static inline const std::map<String, std::function<UniquePtrIEncoder()>> DefaultEncoders = {
    {HEX_ENCODER_NAME_STRING,  &HexEncoder::DefaultInstance},
    {BASE32_ENCODER_NAME_STRING, &Base32Encoder::DefaultInstance},
    {BASE64_ENCODER_NAME_STRING, &Base64Encoder::DefaultInstance},
};

// MARK: ███ AesExtensions ███
class PaddingBase
{
public:
    virtual ~PaddingBase() = default;
    virtual void pad(Botan::secure_vector<uint8_t> &buffer, size_t final_block_bytes, size_t block_size) const = 0;
    virtual size_t unpad(const uint8_t block[], size_t len) const = 0;
    virtual bool valid_blocksize(size_t block_size) const = 0;
    virtual std::string name() const = 0;
};

class ZeroPadding : public PaddingBase
{
public:
    virtual ~ZeroPadding() = default;
    void pad(Botan::secure_vector<uint8_t> &buffer, size_t final_block_bytes, size_t block_size) const override
    {
        const size_t pad_bytes = block_size - final_block_bytes;
        buffer.resize(buffer.size() + pad_bytes, 0x00);
    }

    size_t unpad(const uint8_t block[], size_t len) const override
    {
        size_t i = len;
        while (i > 0 && block[i - 1] == 0x00)
        {
            i--;
        }
        return i;
    }

    bool valid_blocksize(size_t block_size) const override
    {
        return (block_size > 2 && block_size < 256);
    }

    String name() const override
    {
        return "ZeroPadding";
    }
};

class Pkcs7Padding : public PaddingBase
{
public:
    virtual ~Pkcs7Padding() = default;
    void pad(Botan::secure_vector<uint8_t> &buffer, size_t final_block_bytes, size_t block_size) const override
    {
        const size_t pad_bytes = block_size - final_block_bytes;
        for (size_t i = 0; i < pad_bytes; ++i)
        {
            buffer.push_back(static_cast<uint8_t>(pad_bytes));
        }
    }

    size_t unpad(const uint8_t block[], size_t len) const override
    {
        if (len == 0)
        {
            return 0;
        }
        const uint8_t pad_value = block[len - 1];
        if (pad_value <= len && std::all_of(block + len - pad_value, block + len, [pad_value](uint8_t val)
                                            { return val == pad_value; }))
        {
            return len - pad_value;
        }
        else
        {
            __INSANE_THROW_EXCEPTION(CryptoException, "Invalid Pkcs7 padding.", 0, nullptr, DebugType::Trace);
        }
    }

    bool valid_blocksize(size_t block_size) const override
    {
        return (block_size > 0 && block_size <= 256);
    }

    String name() const override
    {
        return __AES_PADDING_MODE_PKCS7_STRING;
    }
};

class X923Padding : public PaddingBase
{
public:
    virtual ~X923Padding() = default;
    void pad(Botan::secure_vector<uint8_t> &buffer, size_t final_block_bytes,
             size_t block_size) const override
    {
        const size_t pad_bytes = block_size - final_block_bytes;
        buffer.resize(buffer.size() + pad_bytes, 0x00);
        buffer[buffer.size() - 1] = static_cast<uint8_t>(pad_bytes);
    }

    size_t unpad(const uint8_t block[], size_t len) const override
    {
        if (len == 0)
        {
            return 0;
        }

        size_t pad_bytes = block[len - 1];

        if (pad_bytes <= len)
        {
            for (size_t i = len - pad_bytes; i < len - 1; ++i)
            {
                if (block[i] != 0x00)
                {
                    __INSANE_THROW_EXCEPTION(CryptoException, "Invalid X923 padding.", 0,
                                             nullptr, DebugType::Trace);
                }
            }
            return len - pad_bytes;
        }
        else
        {
            __INSANE_THROW_EXCEPTION(CryptoException, "Invalid X923 padding.", 0,
                                     nullptr, DebugType::Trace);
        }
    }

    bool valid_blocksize(size_t block_size) const override
    {
        return (block_size > 0 && block_size <= 256);
    }

    std::string name() const override
    {
        return __AES_PADDING_MODE_ANSIX923_STRING;
    }
};

StdVectorUint8 AesExtensions::EncryptAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, const AesCbcPadding &padding)
{
    try
    {
        ValidateKey(key);
        StdVectorUint8 secretKey = GenerateNormalizedKey(key);
        std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
        Botan::secure_vector<uint8_t> ivBytes = rng->random_vec(AES_MAX_IV_LENGTH);
        Botan::secure_vector<uint8_t> dataBytes(data.begin(), data.end());
        Botan::secure_vector<uint8_t> keyBytes(secretKey.begin(), secretKey.end());
        size_t finalBlockSize = dataBytes.size() - ((dataBytes.size() / AES_BLOCK_SIZE_LENGTH) * AES_BLOCK_SIZE_LENGTH);
        String paddingAlgo = __AES_PADDING_MODE_NONE_STRING;
        switch (padding)
        {
            case AesCbcPadding::None:
            {
                paddingAlgo = __AES_PADDING_MODE_NONE_STRING;
            }
            break;
            case AesCbcPadding::Zeros:
            {
                ZeroPadding paddingMethod = ZeroPadding();
                paddingMethod.pad(dataBytes, finalBlockSize, AES_BLOCK_SIZE_LENGTH);
                paddingAlgo = __AES_PADDING_MODE_NONE_STRING;
            }
            break;
            case AesCbcPadding::Pkcs7:
            {
                paddingAlgo = __AES_PADDING_MODE_PKCS7_STRING;
            }
            break;
            case AesCbcPadding::AnsiX923:
            {
                paddingAlgo = __AES_PADDING_MODE_ANSIX923_STRING;
            }
            break;
            default:
                throw NotImplementedException(
                    INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__,
                    AesCbcPaddingEnumExtensions::ToString(padding));
        }

        std::unique_ptr<Botan::Cipher_Mode> enc = Botan::Cipher_Mode::create(StringExtensions::Join({__AES_256_ALGORITHM_STRING, __AES_MODE_CBC_STRING, paddingAlgo}, SLASH_STRING), Botan::Cipher_Dir::Encryption);
        enc->set_key(keyBytes);
        enc->start(ivBytes);
        enc->finish(dataBytes);
        dataBytes.insert(dataBytes.end(), ivBytes.begin(), ivBytes.end());
        return StdVectorUint8(dataBytes.begin(), dataBytes.end());
    }
    catch (const Botan::Exception &e)
    {
        __INSANE_THROW_EXCEPTION(CryptoException, e.what(), e.error_code(), nullptr, DebugType::Debug);
    }
    catch (const ExceptionBase &ex)
    {
        __INSANE_THROW_EXCEPTION(CryptoException, "Unable to encrypt data, check the parameters.", ex.GetErrorCode(), ex.Clone(), DebugType::Debug);
    }
    catch (...)
    {
        __INSANE_THROW_DEFAULT_EXCEPTION(CryptoException, DebugType::Debug);
    }
}

StdVectorUint8 AesExtensions::EncryptAesCbc(const String &data, const String &key, const AesCbcPadding &padding)
{
    return EncryptAesCbc(ConverterExtensions::StringToStdVectorUint8(data), ConverterExtensions::StringToStdVectorUint8(key), padding);
}

StdVectorUint8 AesExtensions::EncryptAesCbc(const StdVectorUint8 &data, const String &key, const AesCbcPadding &padding)
{
    return EncryptAesCbc(data, ConverterExtensions::StringToStdVectorUint8(key), padding);
}

StdVectorUint8 AesExtensions::EncryptAesCbc(const String &data, const StdVectorUint8 &key, const AesCbcPadding &padding)
{
    return EncryptAesCbc(ConverterExtensions::StringToStdVectorUint8(data), key, padding);
}

String AesExtensions::EncryptEncodedAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, std::unique_ptr<IEncoder> &&encoder, const AesCbcPadding &padding)
{
    return encoder->Encode(EncryptAesCbc(data, key, padding));
}

String AesExtensions::EncryptEncodedAesCbc(const String &data, const String &key, std::unique_ptr<IEncoder> &&encoder, const AesCbcPadding &padding)
{
    return encoder->Encode(EncryptAesCbc(data, key, padding));
}

String AesExtensions::EncryptEncodedAesCbc(const StdVectorUint8 &data, const String &key, std::unique_ptr<IEncoder> &&encoder, const AesCbcPadding &padding)
{
    return encoder->Encode(EncryptAesCbc(data, key, padding));
}

String AesExtensions::EncryptEncodedAesCbc(const String &data, const StdVectorUint8 &key, std::unique_ptr<IEncoder> &&encoder, const AesCbcPadding &padding)
{
    return encoder->Encode(EncryptAesCbc(data, key, padding));
}

StdVectorUint8 AesExtensions::DecryptAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, const AesCbcPadding &padding)
{
    try
    {
        ValidateKey(key);
        StdVectorUint8 secretKey = GenerateNormalizedKey(key);
        Botan::secure_vector<uint8_t> dataBytes(data.begin(), data.end() - AES_MAX_IV_LENGTH);
        Botan::secure_vector<uint8_t> keyBytes(secretKey.begin(), secretKey.end());
        Botan::secure_vector<uint8_t> ivBytes(data.end() - AES_MAX_IV_LENGTH, data.end());
        String paddingAlgo = __AES_PADDING_MODE_NONE_STRING;
        switch (padding)
        {
            case AesCbcPadding::None:
            {
                paddingAlgo = __AES_PADDING_MODE_NONE_STRING;
            }
            break;
            case AesCbcPadding::Zeros:
            {
                paddingAlgo = __AES_PADDING_MODE_NONE_STRING;
            }
            break;
            case AesCbcPadding::Pkcs7:
            {
                paddingAlgo = __AES_PADDING_MODE_PKCS7_STRING;
            }
            break;
            case AesCbcPadding::AnsiX923:
            {
                paddingAlgo = __AES_PADDING_MODE_ANSIX923_STRING;
            }
            break;
            default:
                throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, AesCbcPaddingEnumExtensions::ToString(padding));
        }

        std::unique_ptr<Botan::Cipher_Mode> dec = Botan::Cipher_Mode::create(StringExtensions::Join({__AES_256_ALGORITHM_STRING, __AES_MODE_CBC_STRING, paddingAlgo}, SLASH_STRING), Botan::Cipher_Dir::Decryption);
        dec->set_key(keyBytes);
        dec->start(ivBytes);
        dec->finish(dataBytes);
        if (padding == AesCbcPadding::Zeros)
        {
            dataBytes.resize(ZeroPadding().unpad(dataBytes.data(), dataBytes.size()));
        }
        return StdVectorUint8(dataBytes.begin(), dataBytes.end());
    }
    catch (const NotImplementedException &ex)
    {
        __INSANE_THROW_EXCEPTION(CryptoException, ex.GetErrorMessage(), ex.GetErrorCode(), ex.Clone(), DebugType::Debug)
    }
    catch (...)
    {
        __INSANE_THROW_DEFAULT_EXCEPTION(CryptoException, DebugType::Debug)
    }
}

StdVectorUint8 AesExtensions::DecryptAesCbc(const StdVectorUint8 &data, const String &key, const AesCbcPadding &padding)
{
    return DecryptAesCbc(data, ConverterExtensions::StringToStdVectorUint8(key), padding);
}

StdVectorUint8 AesExtensions::DecryptEncodedAesCbc(const String &data, const StdVectorUint8 &key, std::unique_ptr<IEncoder> &&encoder, const AesCbcPadding &padding)
{
    return DecryptAesCbc(encoder->Decode(data), key, padding);
}

StdVectorUint8 AesExtensions::DecryptEncodedAesCbc(const String &data, const String &key, std::unique_ptr<IEncoder> &&encoder, const AesCbcPadding &padding)
{
    return DecryptEncodedAesCbc(data, ConverterExtensions::StringToStdVectorUint8(key), std::move(encoder), padding);
}

StdVectorUint8 AesExtensions::GenerateNormalizedKey(const StdVectorUint8 &key)
{
    StdVectorUint8 hash = HashExtensions::ComputeHash(key, HashAlgorithm::Sha512);
    hash.resize(AES_MAX_KEY_LENGTH);
    return hash;
}

void AesExtensions::ValidateKey(const StdVectorUint8 &key)
{
    if (key.size() < 8)
    {
        throw new ArgumentException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, "Key must be at least 8 bytes.");
    }
}

// MARK: ███ RandomExtensions ███

StdVectorUint8 RandomExtensions::NextBytes(size_t sz)
{
    String result = String(sz, 0);
    std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
    Botan::secure_vector<uint8_t> bytes = (*rng).random_vec(sz);
    return StdVectorUint8(bytes.begin(), bytes.end());
}

int RandomExtensions::NextValue(int min, int max)
{
    Botan::AutoSeeded_RNG rng;
    if (min >= max)
    {
        throw ArgumentException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, "Min value is greater or equals than Max value."s);
    }
    StdVectorUint8 intBytes = NextBytes(4);
    int num = (intBytes[0] << 24) | (intBytes[1] << 16) | (intBytes[2] << 8) | intBytes[3];
    return min + std::abs(num) % (max - min + 1);
}

int RandomExtensions::NextValue()
{
    StdVectorUint8 intBytes = NextBytes(4);
    int num = (intBytes[0] << 24) | (intBytes[1] << 16) | (intBytes[2] << 8) | intBytes[3];
    return num;
}

// MARK: ███ RsaKeyPair ███

RsaKeyPair::RsaKeyPair(const String &publicKey, const String &privateKey) : IJsonSerialize(EMPTY_STRING), PublicKey(publicKey), PrivateKey(privateKey)
{
}

String RsaKeyPair::GetPublicKey() const
{
    return PublicKey;
}

String RsaKeyPair::GetPrivateKey() const
{
    return PrivateKey;
}

String RsaKeyPair::Serialize(const bool &indent) const noexcept(false)
{
    try
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
        writer.StartObject();

        writer.Key(CNAMEOF(PublicKey));
        writer.String(PublicKey.data(), static_cast<rapidjson::SizeType>(PublicKey.length()));

        writer.Key(CNAMEOF(PrivateKey));
        writer.String(PrivateKey.data(), static_cast<rapidjson::SizeType>(PrivateKey.length()));

        writer.EndObject();
        String json = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(json) : json;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

RsaKeyPair RsaKeyPair::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() || !(document.IsObject() && document.HasMember(CNAMEOF_TRIM_GET(PublicKey)) && document.HasMember(CNAMEOF_TRIM_GET(PrivateKey))))
        {
            throw 1;
        }
        return RsaKeyPair(document[CNAMEOF(PublicKey)].GetString(), document[CNAMEOF(PrivateKey)].GetString());
    }
    catch (...)
    {
        throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

// MARK: ███ RsaExtensions ███

static inline std::pair<RsaKeyEncoding, std::unique_ptr<Botan::Public_Key>>
InternalGetRsaKeyEncodingWithKey(const String &key)
{
    String rsaKeyString = StringExtensions::Trim(key);
    if (rsaKeyString.empty())
    {
        return {RsaKeyEncoding::Unknown, nullptr};
    }

    std::unique_ptr<Botan::Public_Key> rsaKey = nullptr;
    if (StringExtensions::IsMatch(rsaKeyString, BASE64_VALUE_REGEX_PATTERN_STRING))
    {
        try
        {
            Botan::SecureVector<uint8_t> keyBytes = Botan::base64_decode(rsaKeyString);
            std::unique_ptr<Botan::DataSource_Memory> source = std::make_unique<Botan::DataSource_Memory>(keyBytes);
            return {RsaKeyEncoding::BerPrivate, Botan::PKCS8::load_key(*source)};
        }
        catch (...)
        {
            try
            {
                Botan::SecureVector<uint8_t> keyBytes = Botan::base64_decode(rsaKeyString);
                rsaKey = Botan::X509::load_key(StdVectorUint8(keyBytes.begin(), keyBytes.end()));
                return {RsaKeyEncoding::BerPublic, std::move(rsaKey)};
            }
            catch (...)
            {
                return {RsaKeyEncoding::Unknown, nullptr};
            }
        }
    }

    try
    {
        if (StringExtensions::StartsWith(rsaKeyString, RSA_XML_KEY_MAIN_TAG_STRING))
        {
            if (StringExtensions::IsMatch(rsaKeyString, RSA_XML_PRIVATE_KEY_REGEX_PATTERN))
            {
                std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
                doc->parse<0>(rsaKeyString.data());
                std::unique_ptr<Botan::BigInt> modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_MODULUS_NODE_STRING.c_str())->value()));
                std::unique_ptr<Botan::BigInt> exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_EXPONENT_NODE_STRING.c_str())->value()));
                std::unique_ptr<Botan::BigInt> P = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_P_NODE_STRING.c_str())->value()));
                std::unique_ptr<Botan::BigInt> Q = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_Q_NODE_STRING.c_str())->value()));
                std::unique_ptr<Botan::BigInt> D = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_D_NODE_STRING.c_str())->value()));
                return {RsaKeyEncoding::XmlPrivate, std::make_unique<Botan::RSA_PrivateKey>(*P, *Q, *exponent, *D, *modulus)};
            }

            if (StringExtensions::IsMatch(rsaKeyString, RSA_XML_PUBLIC_KEY_REGEX_PATTERN))
            {
                std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
                doc->parse<0>(rsaKeyString.data());
                std::unique_ptr<Botan::BigInt> modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_MODULUS_NODE_STRING.c_str())->value()));
                std::unique_ptr<Botan::BigInt> exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_EXPONENT_NODE_STRING.c_str())->value()));
                return {RsaKeyEncoding::XmlPublic, std::make_unique<Botan::RSA_PublicKey>(*modulus, *exponent)};
            }

            return {RsaKeyEncoding::Unknown, nullptr};
        }

        if (StringExtensions::StartsWith(rsaKeyString, RSA_PEM_INITIAL_TEXT_HEADER_STRING))
        {
            if (StringExtensions::IsMatch(rsaKeyString, RSA_PEM_PRIVATE_KEY_REGEX_PATTERN))
            {
                Botan::SecureVector<uint8_t> keyBytes(rsaKeyString.begin(), rsaKeyString.end());
                std::unique_ptr<Botan::DataSource_Memory> source = std::make_unique<Botan::DataSource_Memory>(keyBytes);
                return {RsaKeyEncoding::PemPrivate, Botan::PKCS8::load_key(*source)};
            }

            if (StringExtensions::IsMatch(rsaKeyString, RSA_PEM_PUBLIC_KEY_REGEX_PATTERN))
            {
                rsaKey = Botan::X509::load_key(StdVectorUint8(rsaKeyString.begin(), rsaKeyString.end()));
                return {RsaKeyEncoding::PemPublic, std::move(rsaKey)};
            }

            return {RsaKeyEncoding::Unknown, nullptr};
        }
    }
    catch (...)
    {
        return {RsaKeyEncoding::Unknown, nullptr};
    }

    return {RsaKeyEncoding::Unknown, nullptr};
}

RsaKeyEncoding RsaExtensions::GetRsaKeyEncoding(const String &key)
{
    return InternalGetRsaKeyEncodingWithKey(key).first;
}

static inline std::pair<bool, std::unique_ptr<Botan::Public_Key>> InternalValidateRsaPublicKeyWithKey(const String &publicKey)
{
    std::pair<RsaKeyEncoding, std::unique_ptr<Botan::Public_Key>> encodingResult = InternalGetRsaKeyEncodingWithKey(publicKey);
    return {encodingResult.first == RsaKeyEncoding::XmlPublic || encodingResult.first == RsaKeyEncoding::PemPublic || encodingResult.first == RsaKeyEncoding::BerPublic, std::move(encodingResult.second)};
}

static inline std::pair<bool, std::unique_ptr<Botan::Public_Key>> InternalValidateRsaPrivateKeyWithKey(const String &privateKey)
{
    std::pair<RsaKeyEncoding, std::unique_ptr<Botan::Public_Key>> encodingResult = InternalGetRsaKeyEncodingWithKey(privateKey);
    return {encodingResult.first == RsaKeyEncoding::XmlPrivate || encodingResult.first == RsaKeyEncoding::PemPrivate || encodingResult.first == RsaKeyEncoding::BerPrivate, std::move(encodingResult.second)};
}

static inline std::unique_ptr<Botan::Public_Key> InternalParsePublicKey(const String &key)
{
    std::pair<bool, std::unique_ptr<Botan::Public_Key>> validationResult = InternalValidateRsaPublicKeyWithKey(key);
    if (validationResult.first)
    {
        return std::move(validationResult.second);
    }
    __INSANE_THROW_EXCEPTION(CryptoException, "Unable to parse public key.", 0, nullptr, DebugType::Debug);
}

static inline std::unique_ptr<Botan::Private_Key> InternalParsePrivateKey(const String &key)
{
    auto validation = InternalValidateRsaPrivateKeyWithKey(key);
    if (validation.first)
    {
        Botan::Private_Key *privateKey = dynamic_cast<Botan::Private_Key *>(validation.second.release());
        return std::unique_ptr<Botan::Private_Key>(privateKey);
    }
    __INSANE_THROW_EXCEPTION(CryptoException, "Unable to parse private key.", 0, nullptr, DebugType::Debug);
}

bool RsaExtensions::ValidateRsaPublicKey(const String &publicKey)
{
    return InternalValidateRsaPublicKeyWithKey(publicKey).first;
}

bool RsaExtensions::ValidateRsaPrivateKey(const String &privateKey)
{
    return InternalValidateRsaPrivateKeyWithKey(privateKey).first;
}

RsaKeyPair RsaExtensions::CreateRsaKeyPair(const size_t &keySize, const RsaKeyPairEncoding &encoding)
{
    try
    {
        std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
        std::unique_ptr<Botan::RSA_PrivateKey> keyPair = std::make_unique<Botan::RSA_PrivateKey>(*rng, keySize);
        switch (encoding)
        {
            case RsaKeyPairEncoding::Ber:
            {
                String privateKey = Botan::base64_encode(Botan::PKCS8::BER_encode(*keyPair));
                String publicKey = Botan::base64_encode(Botan::X509::BER_encode(*keyPair));
                return RsaKeyPair(StringExtensions::Trim(publicKey), StringExtensions::Trim(privateKey));
            }
            case RsaKeyPairEncoding::Pem:
            {
                String privateKey = Botan::PKCS8::PEM_encode(*keyPair);
                String publicKey = Botan::X509::PEM_encode(*keyPair);
                return RsaKeyPair(StringExtensions::Trim(publicKey), StringExtensions::Trim(privateKey));
            }
            case RsaKeyPairEncoding::Xml:
            {
                String modulus = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_n()));
                String exponent = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_e()));
                String p = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_p()));
                String q = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_q()));
                String dp = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d1()));
                String dq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d2()));
                String inverseq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_c()));
                String d = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d()));

                std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
                String rsaValueName = RSA_XML_KEY_MAIN_NODE_STRING;
                rapidxml::xml_node<> *mainNode = doc->allocate_node(rapidxml::node_type::node_element, rsaValueName.c_str());
                doc->append_node(mainNode);

                String modulusName = RSA_XML_KEY_MODULUS_NODE_STRING;
                rapidxml::xml_node<> *childNode = doc->allocate_node(rapidxml::node_type::node_element, modulusName.c_str(), modulus.c_str(), 0, modulus.length());
                mainNode->append_node(childNode);

                String exponentName = RSA_XML_KEY_EXPONENT_NODE_STRING;
                childNode = doc->allocate_node(rapidxml::node_type::node_element, exponentName.c_str(), exponent.c_str(), 0, exponent.length());
                mainNode->append_node(childNode);

                String publicKey;
                rapidxml::print(std::back_inserter(publicKey), *doc, 0);

                String pName = RSA_XML_KEY_P_NODE_STRING;
                childNode = doc->allocate_node(rapidxml::node_type::node_element, pName.c_str(), p.c_str(), 0, p.length());
                mainNode->append_node(childNode);

                String qName = RSA_XML_KEY_Q_NODE_STRING;
                childNode = doc->allocate_node(rapidxml::node_type::node_element, qName.c_str(), q.c_str(), 0, q.length());
                mainNode->append_node(childNode);

                String dpName = RSA_XML_KEY_DP_NODE_STRING;
                childNode = doc->allocate_node(rapidxml::node_type::node_element, dpName.c_str(), dp.c_str(), 0, dp.length());
                mainNode->append_node(childNode);

                String dqName = RSA_XML_KEY_DQ_NODE_STRING;
                childNode = doc->allocate_node(rapidxml::node_type::node_element, dqName.c_str(), dq.c_str(), 0, dq.length());
                mainNode->append_node(childNode);

                String inverseqName = RSA_XML_KEY_INVERSEQ_NODE_STRING;
                childNode = doc->allocate_node(rapidxml::node_type::node_element, inverseqName.c_str(), inverseq.c_str(), 0, inverseq.length());
                mainNode->append_node(childNode);

                String dName = RSA_XML_KEY_D_NODE_STRING;
                childNode = doc->allocate_node(rapidxml::node_type::node_element, dName.c_str(), d.c_str(), 0, d.length());
                mainNode->append_node(childNode);
                String privateKey;
                rapidxml::print(std::back_inserter(privateKey), *doc, 0);
                doc->clear();
                return RsaKeyPair(StringExtensions::Trim(publicKey), StringExtensions::Trim(privateKey));
            }
            default:
                throw NotImplementedException(
                    INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__,
                    RsaKeyPairEncodingEnumExtensions::ToString(encoding, true));
        }
    }
    catch (const Botan::Exception &e)
    {
        __INSANE_THROW_EXCEPTION(CryptoException, e.what(), e.error_code(), nullptr, DebugType::Debug);
    }
    catch (const ExceptionBase &ex)
    {
        __INSANE_THROW_EXCEPTION(CryptoException, "Unable to create keypair.", 0, ex.Clone(), DebugType::Debug);
    }
    catch (...)
    {
        __INSANE_THROW_DEFAULT_EXCEPTION(CryptoException, DebugType::Debug);
    }
}

StdVectorUint8 RsaExtensions::EncryptRsa(const StdVectorUint8 &data, const String &publicKey, const RsaPadding &padding)
{
    try
    {
        String paddingStr;
        switch (padding)
        {
            case RsaPadding::Pkcs1:
                paddingStr = __RSA_PADDING_PKCS1_V1_5_ALGORITHM_STRING;
                break;
            case RsaPadding::OaepSha1:
                paddingStr = __RSA_PADDING_OAEP_ALGORITHM_STRING;
                break;
            case RsaPadding::OaepSha256:
                paddingStr = __RSA_PADDING_OAEP_256_ALGORITHM_STRING;
                break;
            case RsaPadding::OaepSha384:
                paddingStr = __RSA_PADDING_OAEP_384_ALGORITHM_STRING;
                break;
            case RsaPadding::OaepSha512:
                paddingStr = __RSA_PADDING_OAEP_512_ALGORITHM_STRING;
                break;
            default:
                throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, RsaPaddingEnumExtensions::ToString(padding, true));
        }
        std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
        std::unique_ptr<Botan::Public_Key> pbk = InternalParsePublicKey(publicKey);
        std::unique_ptr<Botan::PK_Encryptor_EME> enc = std::make_unique<Botan::PK_Encryptor_EME>(*pbk, *rng, paddingStr);
        Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
        std::vector<uint8_t> encrypted = enc->encrypt(dataBytes, *rng);
        return StdVectorUint8(encrypted.begin(), encrypted.end());
    }
    catch (const Botan::Exception &e)
    {
        __INSANE_THROW_EXCEPTION(CryptoException, e.what(), e.error_code(), nullptr, DebugType::Debug);
    }
    catch (const ExceptionBase &ex)
    {
        __INSANE_THROW_EXCEPTION(CryptoException, "Unable to encrypt data, check the parameters.", 0, ex.Clone(), DebugType::Debug);
    }
    catch (...)
    {
        __INSANE_THROW_DEFAULT_EXCEPTION(CryptoException, DebugType::Debug)
    }
}

StdVectorUint8 RsaExtensions::EncryptRsa(const String &data, const String &publicKey, const RsaPadding &padding)
{
    return EncryptRsa(ConverterExtensions::StringToStdVectorUint8(data), publicKey, padding);
}

String RsaExtensions::EncryptEncodedRsa(const StdVectorUint8 &data, const String &publicKey, std::unique_ptr<IEncoder> &&encoder, const RsaPadding &padding)
{
    return encoder->Encode(EncryptRsa(data, publicKey, padding));
}

String RsaExtensions::EncryptEncodedRsa(const String &data, const String &publicKey, std::unique_ptr<IEncoder> &&encoder, const RsaPadding &padding)
{
    return EncryptEncodedRsa(ConverterExtensions::StringToStdVectorUint8(data), publicKey, std::move(encoder), padding);
}

StdVectorUint8 RsaExtensions::DecryptRsa(const StdVectorUint8 &data, const String &privateKey, const RsaPadding &padding)
{
    try
    {
        String paddingStr;
        switch (padding)
        {
            case RsaPadding::Pkcs1:
                paddingStr = __RSA_PADDING_PKCS1_V1_5_ALGORITHM_STRING;
                break;
            case RsaPadding::OaepSha1:
                paddingStr = __RSA_PADDING_OAEP_ALGORITHM_STRING;
                break;
            case RsaPadding::OaepSha256:
                paddingStr = __RSA_PADDING_OAEP_256_ALGORITHM_STRING;
                break;
            case RsaPadding::OaepSha384:
                paddingStr = __RSA_PADDING_OAEP_384_ALGORITHM_STRING;
                break;
            case RsaPadding::OaepSha512:
                paddingStr = __RSA_PADDING_OAEP_512_ALGORITHM_STRING;
                break;
            default:
                throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, RsaPaddingEnumExtensions::ToString(padding, true));
                break;
        }
        std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
        std::unique_ptr<Botan::Private_Key> pvk = InternalParsePrivateKey(privateKey);
        std::unique_ptr<Botan::PK_Decryptor_EME> dec = std::make_unique<Botan::PK_Decryptor_EME>(*pvk, *rng, paddingStr);
        Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
        Botan::SecureVector<uint8_t> decrypted = dec->decrypt(dataBytes);
        return StdVectorUint8(decrypted.begin(), decrypted.end());
    }
    catch (const Botan::Exception &e)
    {
        __INSANE_THROW_EXCEPTION(CryptoException, e.what(), e.error_code(), nullptr, DebugType::Debug);
    }
    catch (const ExceptionBase &ex)
    {
        __INSANE_THROW_EXCEPTION(CryptoException, "Unable to decrypt data, check the parameters."s, 0, ex.Clone(), DebugType::Debug);
    }
    catch (...)
    {
        __INSANE_THROW_DEFAULT_EXCEPTION(CryptoException, DebugType::Debug)
    }
}

StdVectorUint8
RsaExtensions::DecryptEncodedRsa(const String &data, const String &privateKey, std::unique_ptr<IEncoder> &&encoder, const RsaPadding &padding)
{
    return DecryptRsa(encoder->Decode(data), privateKey, padding);
}

// MARK: ███ IHasher ███

IHasher::IHasher(const String &name) : IJsonSerialize(name)
{
}

std::unique_ptr<IHasher> IHasher::Deserialize(INSANE_MAYBE_UNUSED_ATTRIB const String &json)
{
    throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
}

// MARK: ███ ShaHasher ███

ShaHasher::ShaHasher(const HashAlgorithm &hashAlgorithm, std::unique_ptr<IEncoder> &&encoder) : IHasher(SHA_HASHER_NAME_STRING), _HashAlgorithm(hashAlgorithm), _Encoder(encoder ? std::move(encoder) : Base64Encoder::DefaultInstance())
{
}

ShaHasher::ShaHasher(const ShaHasher &instance) : ShaHasher(instance._HashAlgorithm, instance.GetEncoder())
{
}

HashAlgorithm ShaHasher::GetHashAlgorithm() const
{
    return _HashAlgorithm;
}

std::unique_ptr<IEncoder> ShaHasher::GetEncoder() const
{
    return _Encoder->Clone();
}

StdVectorUint8 ShaHasher::Compute(const StdVectorUint8 &data)
{
    return HashExtensions::ComputeHash(data, _HashAlgorithm);
}

StdVectorUint8 ShaHasher::Compute(const String &data)
{
    return HashExtensions::ComputeHash(data, _HashAlgorithm);
}

String ShaHasher::ComputeEncoded(const StdVectorUint8 &data)
{
    return HashExtensions::ComputeEncodedHash(data, _Encoder->Clone(), _HashAlgorithm);
}

String ShaHasher::ComputeEncoded(const String &data)
{
    return HashExtensions::ComputeEncodedHash(data, _Encoder->Clone(), _HashAlgorithm);
}

bool ShaHasher::Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected)
{
    return HashExtensions::VerifyHash(data, expected);
}

bool ShaHasher::Verify(const String &data, const StdVectorUint8 &expected)
{
    return HashExtensions::VerifyHash(data, expected);
}

bool ShaHasher::VerifyEncoded(const StdVectorUint8 &data, const String &expected)
{
    return HashExtensions::VerifyEncodedHash(data, expected, _Encoder->Clone(), _HashAlgorithm);
}

bool ShaHasher::VerifyEncoded(const String &data, const String &expected)
{
    return HashExtensions::VerifyEncodedHash(data, expected, _Encoder->Clone(), _HashAlgorithm);
}

String ShaHasher::Serialize(const bool &indent) const
{
    try
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
        writer.StartObject();

        writer.Key(CNAMEOF(Name));
        writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

        writer.Key(CNAMEOF(HashAlgorithm));
        writer.Int(HashAlgorithmEnumExtensions::ToIntegral(_HashAlgorithm));

        writer.Key(CNAMEOF(Encoder));
        String serialized = _Encoder->Serialize(indent);
        writer.RawValue(serialized.c_str(), serialized.size(), rapidjson::kObjectType);

        writer.EndObject();
        String json = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(json) : json;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IHasher> ShaHasher::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() ||
            !(document.IsObject() &&
              document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetHashAlgorithm)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
        {
            throw true;
        }
        std::unique_ptr<IEncoder> encoder = DefaultEncoders.at(document[CNAMEOF_TRIM_GET(GetEncoder)][CNAMEOF_TRIM_GET(GetName)].GetString())();
        HashAlgorithm algorithm = HashAlgorithmEnumExtensions::Parse(
            document[CNAMEOF_TRIM_GET(GetHashAlgorithm)].GetInt());
        return std::make_unique<ShaHasher>(algorithm, std::move(encoder));
    }
    catch (...)
    {
        throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IHasher> ShaHasher::Clone() const
{
    return std::make_unique<ShaHasher>(*this);
}

// MARK: ███ HmacHasher ███

HmacHasher::HmacHasher(const StdVectorUint8 &key, const HashAlgorithm &hashAlgorithm, std::unique_ptr<IEncoder> &&encoder) : IHasher(HMAC_HASHER_NAME_STRING), _HashAlgorithm(hashAlgorithm), _Encoder(encoder ? std::move(encoder) : std::move(Base64Encoder::DefaultInstance())), _Key(key.empty() ? RandomExtensions::NextBytes(SHA512_DIGEST_LENGTH) : key)
{
}

HmacHasher::HmacHasher(const HmacHasher &instance) : HmacHasher(instance.GetKey(), instance.GetHashAlgorithm(), instance.GetEncoder())
{
}

HashAlgorithm HmacHasher::GetHashAlgorithm() const
{
    return _HashAlgorithm;
}

std::unique_ptr<IEncoder> HmacHasher::GetEncoder() const
{
    return _Encoder->Clone();
}

StdVectorUint8 HmacHasher::GetKey() const
{
    return _Key;
}

String HmacHasher::GetKeyEncoded() const
{
    return _Encoder->Encode(_Key);
}

StdVectorUint8 HmacHasher::Compute(const StdVectorUint8 &data)
{
    return HashExtensions::ComputeHmac(data, _Key, _HashAlgorithm);
}

StdVectorUint8 HmacHasher::Compute(const String &data)
{
    return Compute(ConverterExtensions::StringToStdVectorUint8(data));
}

String HmacHasher::ComputeEncoded(const StdVectorUint8 &data)
{
    return _Encoder->Encode(Compute(data));
}

String HmacHasher::ComputeEncoded(const String &data)
{
    return ComputeEncoded(ConverterExtensions::StringToStdVectorUint8(data));
}

bool HmacHasher::Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected)
{
    return Compute(data) == expected;
}

bool HmacHasher::Verify(const String &data, const StdVectorUint8 &expected)
{
    return Compute(data) == expected;
}

bool HmacHasher::VerifyEncoded(const StdVectorUint8 &data, const String &expected)
{
    return ComputeEncoded(data) == expected;
}

bool HmacHasher::VerifyEncoded(const String &data, const String &expected)
{
    return ComputeEncoded(data) == expected;
}

String HmacHasher::Serialize(const bool &indent) const
{
    try
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
        writer.StartObject();

        writer.Key(CNAMEOF_TRIM_GET(GetName));
        writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

        writer.Key(CNAMEOF_TRIM_GET(GetKey));
        String key = GetKeyEncoded();
        writer.String(key.data(), static_cast<rapidjson::SizeType>(key.length()));

        writer.Key(CNAMEOF_TRIM_GET(GetHashAlgorithm));
        writer.Int(HashAlgorithmEnumExtensions::ToIntegral(_HashAlgorithm));

        writer.Key(CNAMEOF_TRIM_GET(GetEncoder));
        String serialized = _Encoder->Serialize(indent);
        writer.RawValue(serialized.c_str(), serialized.size(), rapidjson::kStringType);

        writer.EndObject();
        String json = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(json) : json;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IHasher> HmacHasher::Clone() const
{
    return std::make_unique<HmacHasher>(*this);
}

std::unique_ptr<IHasher> HmacHasher::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() ||
            !(document.IsObject() &&
              document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetKey)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetHashAlgorithm)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
        {
            throw true;
        }
        std::unique_ptr<IEncoder> encoder = DefaultEncoders.at(document[CNAMEOF_TRIM_GET(GetEncoder)][CNAMEOF_TRIM_GET(GetName)].GetString())();
        HashAlgorithm algorithm = HashAlgorithmEnumExtensions::Parse(
            document[CNAMEOF_TRIM_GET(GetHashAlgorithm)].GetInt());
        StdVectorUint8 key =
            encoder->Decode(document[CNAMEOF_TRIM_GET(GetKey)].GetString());
        return std::make_unique<HmacHasher>(key, algorithm, std::move(encoder));
    }
    catch (...)
    {
        throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

// MARK: ███ Argon2Hasher ███

Argon2Hasher::Argon2Hasher(const StdVectorUint8 &salt, const unsigned int &iterations, const unsigned int &memorySizeKiB, const unsigned int &degreeOfParallelism, const Argon2Variant argon2Variant, const unsigned int &derivedKeyLength, std::unique_ptr<IEncoder> &&encoder)
    : IHasher(ARGON2_HASHER_NAME_STRING), _Salt(salt), _Iterations(iterations), _MemorySizeKiB(memorySizeKiB), _DegreeOfParallelism(degreeOfParallelism), _DerivedKeyLength(derivedKeyLength), _Argon2Variant(argon2Variant), _Encoder(encoder ? std::move(encoder) : std::move(Base64Encoder::DefaultInstance()))
{
}

Argon2Hasher::Argon2Hasher(const Argon2Hasher &instance)
    : Argon2Hasher(instance.GetSalt(), instance.GetIterations(), instance.GetMemorySizeKiB(), instance.GetDegreeOfParallelism(), instance.GetArgon2Variant(), instance.GetDerivedKeyLength(), instance.GetEncoder())
{
}

StdVectorUint8 Argon2Hasher::GetSalt() const
{
    return _Salt;
}

String Argon2Hasher::GetSaltEncoded() const
{
    return _Encoder->Encode(_Salt);
}

unsigned int Argon2Hasher::GetIterations() const
{
    return _Iterations;
}

unsigned int Argon2Hasher::GetMemorySizeKiB() const
{
    return _MemorySizeKiB;
}

unsigned int Argon2Hasher::GetDegreeOfParallelism() const
{
    return _DegreeOfParallelism;
}

unsigned int Argon2Hasher::GetDerivedKeyLength() const
{
    return _DerivedKeyLength;
}

Argon2Variant Argon2Hasher::GetArgon2Variant() const
{
    return _Argon2Variant;
}

std::unique_ptr<IEncoder> Argon2Hasher::GetEncoder() const
{
    return _Encoder->Clone();
}

StdVectorUint8 Argon2Hasher::Compute(const StdVectorUint8 &data)
{
    return HashExtensions::ComputeArgon2(data, _Salt, _Iterations, _MemorySizeKiB, _DegreeOfParallelism, _Argon2Variant, _DerivedKeyLength);
}

StdVectorUint8 Argon2Hasher::Compute(const String &data)
{
    return Compute(ConverterExtensions::StringToStdVectorUint8(data));
}

String Argon2Hasher::ComputeEncoded(const StdVectorUint8 &data)
{
    return _Encoder->Encode(Compute(data));
}

String Argon2Hasher::ComputeEncoded(const String &data)
{
    return ComputeEncoded(ConverterExtensions::StringToStdVectorUint8(data));
}

bool Argon2Hasher::Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected)
{
    return Compute(data) == expected;
}

bool Argon2Hasher::Verify(const String &data, const StdVectorUint8 &expected)
{
    return Compute(data) == expected;
}

bool Argon2Hasher::VerifyEncoded(const StdVectorUint8 &data, const String &expected)
{
    return ComputeEncoded(data) == expected;
}

bool Argon2Hasher::VerifyEncoded(const String &data, const String &expected)
{
    return ComputeEncoded(data) == expected;
}

String Argon2Hasher::Serialize(const bool &indent) const
{
    try
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
        writer.StartObject();

        writer.Key(CNAMEOF_TRIM_GET(GetName));
        writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

        writer.Key(CNAMEOF_TRIM_GET(GetSalt));
        String salt = GetSaltEncoded();
        writer.String(salt.data(), static_cast<rapidjson::SizeType>(salt.length()));

        writer.Key(CNAMEOF_TRIM_GET(GetIterations));
        String numberSizeT = IntegralExtensions::ToString(GetIterations());
        writer.RawValue(numberSizeT.data(), numberSizeT.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetMemorySizeKiB));
        numberSizeT = IntegralExtensions::ToString(GetMemorySizeKiB());
        writer.RawValue(numberSizeT.data(), numberSizeT.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetDegreeOfParallelism));
        numberSizeT = IntegralExtensions::ToString(GetDegreeOfParallelism());
        writer.RawValue(numberSizeT.data(), numberSizeT.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetArgon2Variant));
        writer.Int(Argon2VariantEnumExtensions::ToIntegral(GetArgon2Variant()));

        writer.Key(CNAMEOF_TRIM_GET(GetDerivedKeyLength));
        numberSizeT = IntegralExtensions::ToString(GetDerivedKeyLength());
        writer.RawValue(numberSizeT.data(), numberSizeT.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetEncoder));
        String serialized = _Encoder->Serialize(indent);
        writer.RawValue(serialized.c_str(), serialized.size(), rapidjson::kObjectType);

        writer.EndObject();
        String json = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(json) : json;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IHasher> Argon2Hasher::Clone() const
{
    return std::make_unique<Argon2Hasher>(*this);
}

std::unique_ptr<IHasher> Argon2Hasher::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() ||
            !(document.IsObject() &&
              document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetSalt)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetIterations)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetMemorySizeKiB)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetDegreeOfParallelism)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetDerivedKeyLength)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetArgon2Variant)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
        {
            throw true;
        }
        std::unique_ptr<IEncoder> encoder = DefaultEncoders.at(document[CNAMEOF_TRIM_GET(GetEncoder)][CNAMEOF_TRIM_GET(GetName)].GetString())();
        StdVectorUint8 salt = encoder->Decode(document[CNAMEOF_TRIM_GET(GetSalt)].GetString());
        size_t iterations = document[CNAMEOF_TRIM_GET(GetIterations)].GetUint();
        size_t memorySizeKiB = document[CNAMEOF_TRIM_GET(GetMemorySizeKiB)].GetUint();
        size_t degreeOfParallelism = document[CNAMEOF_TRIM_GET(GetDegreeOfParallelism)].GetUint();
        size_t derivedKeyLength = document[CNAMEOF_TRIM_GET(GetDerivedKeyLength)].GetUint();
        Argon2Variant argon2Variant = Argon2VariantEnumExtensions::Parse(document[CNAMEOF_TRIM_GET(GetArgon2Variant)].GetInt());
        return std::make_unique<Argon2Hasher>(salt, iterations, memorySizeKiB, degreeOfParallelism, argon2Variant, derivedKeyLength, std::move(encoder));
    }
    catch (...)
    {
        throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

// MARK: ███ ScryptHasher ███

ScryptHasher::ScryptHasher(const StdVectorUint8 &salt, const unsigned int &iterations, const unsigned int &blocksize, const unsigned int &parallelism, const unsigned int &derivedKeyLength, std::unique_ptr<IEncoder> &&encoder)
    : IHasher(SCRYPT_HASHER_NAME_STRING), _Salt(salt), _Iterations(iterations), _BlockSize(blocksize), _Parallelism(parallelism), _DerivedKeyLength(derivedKeyLength), _Encoder(encoder ? std::move(encoder) : std::move(Base64Encoder::DefaultInstance()))
{
}

ScryptHasher::ScryptHasher(const ScryptHasher &instance)
    : ScryptHasher(instance.GetSalt(), instance.GetIterations(), instance.GetBlockSize(), instance.GetParallelism(), instance.GetDerivedKeyLength(), instance.GetEncoder())
{
}

StdVectorUint8 ScryptHasher::GetSalt() const
{
    return _Salt;
}

String ScryptHasher::GetSaltEncoded() const
{
    return _Encoder->Encode(_Salt);
}

unsigned int ScryptHasher::GetIterations() const
{
    return _Iterations;
}

unsigned int ScryptHasher::GetBlockSize() const
{
    return _BlockSize;
}

unsigned int ScryptHasher::GetParallelism() const
{
    return _Parallelism;
}

unsigned int ScryptHasher::GetDerivedKeyLength() const
{
    return _DerivedKeyLength;
}

std::unique_ptr<IEncoder> ScryptHasher::GetEncoder() const
{
    return _Encoder->Clone();
}

StdVectorUint8 ScryptHasher::Compute(const StdVectorUint8 &data)
{
    return HashExtensions::ComputeScrypt(data, _Salt, _Iterations, _BlockSize, _Parallelism, _DerivedKeyLength);
}

StdVectorUint8 ScryptHasher::Compute(const String &data)
{
    return Compute(ConverterExtensions::StringToStdVectorUint8(data));
}

String ScryptHasher::ComputeEncoded(const StdVectorUint8 &data)
{
    return _Encoder->Encode(Compute(data));
}

String ScryptHasher::ComputeEncoded(const String &data)
{
    return ComputeEncoded(ConverterExtensions::StringToStdVectorUint8(data));
}

bool ScryptHasher::Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected)
{
    return Compute(data) == expected;
}

bool ScryptHasher::Verify(const String &data, const StdVectorUint8 &expected)
{
    return Compute(data) == expected;
}

bool ScryptHasher::VerifyEncoded(const StdVectorUint8 &data, const String &expected)
{
    return ComputeEncoded(data) == expected;
}

bool ScryptHasher::VerifyEncoded(const String &data, const String &expected)
{
    return ComputeEncoded(data) == expected;
}

std::unique_ptr<IHasher> ScryptHasher::Clone() const
{
    return std::make_unique<ScryptHasher>(*this);
}

String ScryptHasher::Serialize(const bool &indent) const
{
    try
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

        writer.StartObject();

        writer.Key(CNAMEOF_TRIM_GET(GetName));
        writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

        writer.Key(CNAMEOF_TRIM_GET(GetSalt));
        String salt = GetSaltEncoded();
        writer.String(salt.data(), static_cast<rapidjson::SizeType>(salt.length()));

        writer.Key(CNAMEOF_TRIM_GET(GetIterations));
        String numberSizeT = IntegralExtensions::ToString(GetIterations());
        writer.RawValue(numberSizeT.data(), numberSizeT.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetBlockSize));
        numberSizeT = IntegralExtensions::ToString(GetBlockSize());
        writer.RawValue(numberSizeT.data(), numberSizeT.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetParallelism));
        numberSizeT = IntegralExtensions::ToString(GetParallelism());
        writer.RawValue(numberSizeT.data(), numberSizeT.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetDerivedKeyLength));
        numberSizeT = IntegralExtensions::ToString(GetDerivedKeyLength());
        writer.RawValue(numberSizeT.data(), numberSizeT.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetEncoder));
        String serialized = _Encoder->Serialize(indent);
        writer.RawValue(serialized.c_str(), serialized.size(), rapidjson::kObjectType);

        writer.EndObject();
        String json = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(json) : json;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IHasher> ScryptHasher::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() ||
            !(document.IsObject() &&
              document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetSalt)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetIterations)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetBlockSize)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetParallelism)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetDerivedKeyLength)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
        {
            throw true;
        }
        std::unique_ptr<IEncoder> encoder = DefaultEncoders.at(document[CNAMEOF_TRIM_GET(GetEncoder)][CNAMEOF_TRIM_GET(GetName)].GetString())();
        StdVectorUint8 salt = encoder->Decode(document[CNAMEOF_TRIM_GET(GetSalt)].GetString());
        size_t iterations = document[CNAMEOF_TRIM_GET(GetIterations)].GetUint();
        size_t blockSize = document[CNAMEOF_TRIM_GET(GetBlockSize)].GetUint();
        size_t parallelism = document[CNAMEOF_TRIM_GET(GetParallelism)].GetUint();
        size_t derivedKeyLength = document[CNAMEOF_TRIM_GET(GetDerivedKeyLength)].GetUint();
        return std::make_unique<ScryptHasher>(salt, iterations, blockSize, parallelism, derivedKeyLength, std::move(encoder));
    }
    catch (...)
    {
        throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

// MARK: ███ IEncryptor ███

IEncryptor::IEncryptor(const String &name) : IJsonSerialize(name)
{
}

// MARK:███ AesCbcEncryptor ███
AesCbcEncryptor::AesCbcEncryptor(const StdVectorUint8 &key, const AesCbcPadding &padding, std::unique_ptr<IEncoder> &&encoder)
    : IEncryptor(AES_CBC_ENCRYPTOR_NAME_STRING), _Key(key), _Padding(padding), _Encoder(encoder ? std::move(encoder) : std::move(Base64Encoder::DefaultInstance()))
{
}

AesCbcEncryptor::AesCbcEncryptor(const AesCbcEncryptor &instance)
    : AesCbcEncryptor(instance.GetKey(), instance.GetPadding(), instance.GetEncoder()) {}

StdVectorUint8 AesCbcEncryptor::GetKey() const
{
    return _Key;
}

String AesCbcEncryptor::GetKeyEncoded() const
{
    return _Encoder->Encode(_Key);
}

AesCbcPadding AesCbcEncryptor::GetPadding() const
{
    return _Padding;
}

std::unique_ptr<IEncoder> AesCbcEncryptor::GetEncoder() const
{
    return _Encoder->Clone();
}

StdVectorUint8 AesCbcEncryptor::Encrypt(const StdVectorUint8 &data) const
{
    return AesExtensions::EncryptAesCbc(data, _Key, _Padding);
}

String AesCbcEncryptor::EncryptEncoded(const StdVectorUint8 &data) const
{
    return _Encoder->Encode(Encrypt(data));
}

StdVectorUint8 AesCbcEncryptor::Encrypt(const String &data) const
{
    return Encrypt(ConverterExtensions::StringToStdVectorUint8(data));
}

String AesCbcEncryptor::EncryptEncoded(const String &data) const
{
    return EncryptEncoded(ConverterExtensions::StringToStdVectorUint8(data));
}

StdVectorUint8 AesCbcEncryptor::Decrypt(const StdVectorUint8 &data) const
{
    return AesExtensions::DecryptAesCbc(data, _Key, _Padding);
}

StdVectorUint8 AesCbcEncryptor::DecryptEncoded(const String &data) const
{
    return Decrypt(_Encoder->Decode(data));
}

std::unique_ptr<IEncryptor> AesCbcEncryptor::Clone() const
{
    return std::make_unique<AesCbcEncryptor>(*this);
}

String AesCbcEncryptor::Serialize(const bool &indent) const
{
    try
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
        writer.StartObject();

        writer.Key(CNAMEOF_TRIM_GET(GetName));
        writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

        writer.Key(CNAMEOF_TRIM_GET(GetKey));
        String value = _Encoder->Encode(GetKey());
        writer.String(value.data(), static_cast<rapidjson::SizeType>(value.length()));

        writer.Key(CNAMEOF_TRIM_GET(GetPadding));
        value = AesCbcPaddingEnumExtensions::ToIntegralString(GetPadding());
        writer.RawValue(value.data(), value.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetEncoder));
        value = _Encoder->Serialize(indent);
        writer.RawValue(value.c_str(), value.size(), rapidjson::kObjectType);

        writer.EndObject();
        String json = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(json) : json;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IEncryptor> AesCbcEncryptor::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() ||
            !(document.IsObject() &&
              document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
              document.HasMember(CNAMEOF(Protector)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetKey)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetPadding)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
        {
            throw true;
        }
        String protectorName = document[CNAMEOF(Protector)].GetString();
        std::unique_ptr<IEncoder> encoder = DefaultEncoders.at(document[CNAMEOF_TRIM_GET(GetEncoder)][CNAMEOF_TRIM_GET(GetName)].GetString())();
        StdVectorUint8 key = encoder->Decode(document[CNAMEOF_TRIM_GET(GetKey)].GetString());
        AesCbcPadding padding = AesCbcPaddingEnumExtensions::Parse(document[CNAMEOF_TRIM_GET(GetPadding)].GetInt());
        return std::make_unique<AesCbcEncryptor>(key, padding, std::move(encoder));
    }
    catch (...)
    {
        throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

// MARK: ███ RsaEncryptor ███

RsaEncryptor::RsaEncryptor(const RsaKeyPair &keyPair, const RsaPadding &padding, std::unique_ptr<IEncoder> &&encoder)
    : IEncryptor(RSA_ENCRYPTOR_NAME_STRING), _KeyPair(keyPair), _Padding(padding), _Encoder(encoder ? std::move(encoder) : std::move(Base64Encoder::DefaultInstance()))
{
}

RsaEncryptor::RsaEncryptor(const RsaEncryptor &instance)
    : RsaEncryptor(instance.GetKeyPair(), instance.GetPadding(), instance.GetEncoder())
{
}

RsaKeyPair RsaEncryptor::GetKeyPair() const
{
    return _KeyPair;
}

RsaPadding RsaEncryptor::GetPadding() const
{
    return _Padding;
}

std::unique_ptr<IEncoder> RsaEncryptor::GetEncoder() const
{
    return _Encoder->Clone();
}

StdVectorUint8 RsaEncryptor::Encrypt(const StdVectorUint8 &data) const
{
    return RsaExtensions::EncryptRsa(data, _KeyPair.GetPublicKey(), _Padding);
}

String RsaEncryptor::EncryptEncoded(const StdVectorUint8 &data) const
{
    return _Encoder->Encode(Encrypt(data));
}

StdVectorUint8 RsaEncryptor::Encrypt(const String &data) const
{
    return Encrypt(ConverterExtensions::StringToStdVectorUint8(data));
}

String RsaEncryptor::EncryptEncoded(const String &data) const
{
    return EncryptEncoded(ConverterExtensions::StringToStdVectorUint8(data));
}

StdVectorUint8 RsaEncryptor::Decrypt(const StdVectorUint8 &data) const
{
    return RsaExtensions::DecryptRsa(data, _KeyPair.GetPrivateKey(), _Padding);
}

StdVectorUint8 RsaEncryptor::DecryptEncoded(const String &data) const
{
    return Decrypt(_Encoder->Decode(data));
}

std::unique_ptr<IEncryptor> RsaEncryptor::Clone() const
{
    return std::make_unique<RsaEncryptor>(*this);
}

String RsaEncryptor::Serialize(const bool &indent) const
{
    try
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
        writer.StartObject();

        writer.Key(CNAMEOF_TRIM_GET(GetName));
        writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

        String value;
        writer.Key(CNAMEOF_TRIM_GET(GetKeyPair));
        value = _KeyPair.Serialize();
        writer.RawValue(value.data(), value.length(), rapidjson::kObjectType);

        writer.Key(CNAMEOF_TRIM_GET(GetPadding));
        value = RsaPaddingEnumExtensions::ToIntegralString(GetPadding());
        writer.RawValue(value.data(), value.length(), rapidjson::kNumberType);

        writer.Key(CNAMEOF_TRIM_GET(GetEncoder));
        value = _Encoder->Serialize(indent);
        writer.RawValue(value.c_str(), value.size(), rapidjson::kObjectType);

        writer.EndObject();
        String json = String(sb.GetString(), sb.GetSize());
        return indent ? RapidJsonExtensions::Prettify(json) : json;
    }
    catch (...)
    {
        throw SerializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

std::unique_ptr<IEncryptor> RsaEncryptor::Deserialize(const String &json)
{
    try
    {
        rapidjson::Document document;
        document.Parse(json.c_str(), json.length());
        if (document.HasParseError() ||
            !(document.IsObject() &&
              document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
              document.HasMember(CNAMEOF(Protector)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetKeyPair)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetPadding)) &&
              document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
        {
            throw true;
        }
        std::unique_ptr<IEncoder> encoder = DefaultEncoders.at(document[CNAMEOF_TRIM_GET(GetEncoder)][CNAMEOF_TRIM_GET(GetName)].GetString())();
        String publicKey = RapidJsonExtensions::GetStringValue(document[CNAMEOF_TRIM_GET(GetKeyPair)], CNAMEOF_TRIM_GET(RsaKeyPair::GetPublicKey));
        String privateKey = RapidJsonExtensions::GetStringValue(document[CNAMEOF_TRIM_GET(GetKeyPair)], CNAMEOF_TRIM_GET(RsaKeyPair::GetPrivateKey));
        publicKey = ConverterExtensions::StdVectorUint8ToString(encoder->Decode(publicKey));
        privateKey = ConverterExtensions::StdVectorUint8ToString(encoder->Decode(privateKey));

        RsaKeyPair keypair{publicKey, privateKey};
        RsaPadding padding = RsaPaddingEnumExtensions::Parse(document[CNAMEOF_TRIM_GET(GetPadding)].GetInt());
        return std::make_unique<RsaEncryptor>(keypair, padding, std::move(encoder));
    }
    catch (...)
    {
        throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
    }
}

// MARK: ███ CryptoTests ███
void CryptoTests::HexEncodingExtensionsTests(const bool &showValues)
{
    StdVectorUint8 testTytes = {0xff, 0xa, 1, 0x22};
    String hexStringUppercase = "FF0A0122";
    String hexStringLowercase = "ff0a0122";

    String dataStr;
    StdVectorUint8 dataStdVector8;
    String resultStr;
    StdVectorUint8 resultStdVector8;
    String expectedStr;
    StdVectorUint8 expectedStdVector8;

    dataStdVector8 = testTytes;
    resultStr = HexEncodingExtensions::EncodeToHex(dataStdVector8);
    expectedStr = hexStringLowercase;
    TestExtensions::Equals(NAMEOF(HexEncodingExtensions) + "- Encode - 1", expectedStr, resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = testTytes;
    resultStr = HexEncodingExtensions::EncodeToHex(dataStdVector8, true);
    expectedStr = hexStringUppercase;
    TestExtensions::Equals(NAMEOF(HexEncodingExtensions) + "- Encode - 2", expectedStr, resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStr = hexStringUppercase;
    resultStdVector8 = HexEncodingExtensions::DecodeFromHex(dataStr);
    expectedStdVector8 = testTytes;
    TestExtensions::Equals(NAMEOF(HexEncodingExtensions) + "- Decode - 1", expectedStdVector8, resultStdVector8, LambdaExtensions::GetStdVectorUint8OutFunction(), showValues);

    dataStr = hexStringLowercase;
    resultStdVector8 = HexEncodingExtensions::DecodeFromHex(dataStr);
    expectedStdVector8 = testTytes;
    TestExtensions::Equals(NAMEOF(HexEncodingExtensions) + "- Decode - 2", expectedStdVector8, resultStdVector8, LambdaExtensions::GetStdVectorUint8OutFunction(), showValues);
}

void CryptoTests::Base32EncodingExtensionsTests(const bool &showValues)
{
    String TestString = "helloworld";
    String TestString2 = "A";
    String UpperBase32Result = "NBSWY3DPO5XXE3DE";
    String LowerBase32Result = "nbswy3dpo5xxe3de";
    String UpperBase32Result2 = "IE======";
    String LowerBase32Result2 = "ie======";
    String UpperBase32Result2NoPadding = "IE";
    String LowerBase32Result2NoPadding = "ie";

    String dataStr;
    StdVectorUint8 dataStdVector8;
    String resultStr;
    StdVectorUint8 resultStdVector8;
    String expectedStr;
    StdVectorUint8 expectedStdVector8;

    dataStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString2);
    resultStr = Base32EncodingExtensions::EncodeToBase32(dataStdVector8);
    expectedStr = UpperBase32Result2;
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + "- Encode - 1", expectedStr, resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString);
    resultStr = Base32EncodingExtensions::EncodeToBase32(dataStdVector8);
    expectedStr = UpperBase32Result;
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 2", expectedStr, resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString);
    resultStr =
        Base32EncodingExtensions::EncodeToBase32(dataStdVector8, false, true);
    expectedStr = LowerBase32Result;
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 3", expectedStr, resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString2);
    resultStr = Base32EncodingExtensions::EncodeToBase32(dataStdVector8, false);
    expectedStr = UpperBase32Result2;
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 4", expectedStr, resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString2);
    resultStr = Base32EncodingExtensions::EncodeToBase32(dataStdVector8, false, true);
    expectedStr = LowerBase32Result2;
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 5", expectedStr, resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString2);
    resultStr = Base32EncodingExtensions::EncodeToBase32(dataStdVector8, true, false);
    expectedStr = UpperBase32Result2NoPadding;
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 6", expectedStr, resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString2);
    resultStr = Base32EncodingExtensions::EncodeToBase32(dataStdVector8, true, true);
    expectedStr = LowerBase32Result2NoPadding;
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 7", expectedStr, resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStr = UpperBase32Result;
    resultStdVector8 = Base32EncodingExtensions::DecodeFromBase32(dataStr);
    expectedStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString);
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 1",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = LowerBase32Result;
    resultStdVector8 = Base32EncodingExtensions::DecodeFromBase32(dataStr);
    expectedStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString);
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 2",
                           expectedStdVector8,
                           resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = UpperBase32Result2;
    resultStdVector8 = Base32EncodingExtensions::DecodeFromBase32(dataStr);
    expectedStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString2);
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 3",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = LowerBase32Result2;
    resultStdVector8 = Base32EncodingExtensions::DecodeFromBase32(dataStr);
    expectedStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString2);
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 4",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = UpperBase32Result2NoPadding;
    resultStdVector8 = Base32EncodingExtensions::DecodeFromBase32(dataStr);
    expectedStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString2);
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 5",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = LowerBase32Result2NoPadding;
    resultStdVector8 = Base32EncodingExtensions::DecodeFromBase32(dataStr);
    expectedStdVector8 = ConverterExtensions::StringToStdVectorUint8(TestString2);
    TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 6",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);
}

void CryptoTests::Base64EncodingExtensionsTests(const bool &showValues)
{
    StdVectorUint8 testBytes = {
        0x30, 0x82, 0x02, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
        0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x02, 0x0f, 0x00,
        0x30, 0x82, 0x02, 0x0a, 0x02, 0x82, 0x02, 0x01, 0x00, 0xf2, 0xe8, 0xe5,
        0x81, 0x32, 0x36, 0xb8, 0xb6, 0x3f, 0xb5, 0xbe, 0x76, 0x65, 0x65, 0xd1,
        0x8f, 0x2d, 0xc4, 0xc5, 0xa1, 0x91, 0x3b, 0x8b, 0xdc, 0x8b, 0xf6, 0x4f,
        0x42, 0x64, 0xd1, 0xea, 0xdc, 0x75, 0x6c, 0x83, 0x0b, 0x81, 0x1f, 0x57,
        0xeb, 0xac, 0xe5, 0xd0, 0x5c, 0x6b, 0x5f, 0x37, 0xa8, 0x53, 0x1c, 0x65,
        0x6b, 0x75, 0x5e, 0xbc, 0xd3, 0x59, 0xd2, 0x54, 0x17, 0xf7, 0x69, 0x4d,
        0x23, 0x92, 0x7e, 0x78, 0x47, 0xf1, 0x06, 0x04, 0x5b, 0x55, 0x00, 0xb1,
        0xaa, 0x82, 0x70, 0x70, 0xc0, 0xff, 0x3c, 0x29, 0x4a, 0x2f, 0xc3, 0xff,
        0x56, 0x60, 0x4a, 0x22, 0x12, 0xfe, 0x10, 0xa4, 0xe1, 0xeb, 0x9d, 0x82,
        0xb3, 0x76, 0x1c, 0xa0, 0x18, 0x4c, 0xca, 0xcd, 0x68, 0x40, 0x2e, 0x6a,
        0x21, 0x2a, 0x7b, 0x7b, 0xc6, 0x0b, 0x85, 0x14, 0x19, 0x03, 0x40, 0xe9,
        0x78, 0x54, 0xfe, 0x97, 0xf4, 0xe8, 0x39, 0x45, 0x06, 0x76, 0x8e, 0x5e,
        0x0e, 0xdb, 0x62, 0x41, 0x60, 0x2b, 0xfb, 0x1e, 0x1a, 0x65, 0x3a, 0x25,
        0x48, 0xba, 0xe6, 0x73, 0x8f, 0x35, 0xf0, 0xfd, 0x99, 0xe4, 0x1d, 0xe9,
        0xbf, 0x67, 0x8b, 0xf4, 0x1d, 0xfa, 0xfa, 0x58, 0x8e, 0xe7, 0x1b, 0x7b,
        0xb5, 0x7d, 0x74, 0x90, 0x26, 0x41, 0x88, 0xbd, 0x4d, 0x20, 0x69, 0x4b,
        0x4c, 0x8a, 0xef, 0x47, 0x87, 0xc1, 0xf3, 0x5b, 0x42, 0x79, 0x04, 0xd7,
        0x9d, 0x42, 0xa7, 0xdf, 0xca, 0x0d, 0xf4, 0x19, 0x4a, 0x8d, 0x7c, 0x93,
        0x3f, 0x1a, 0xa5, 0x39, 0xef, 0xcd, 0x6d, 0xe5, 0x0a, 0xe5, 0xf0, 0x41,
        0x16, 0x96, 0x58, 0x14, 0x99, 0x77, 0xdc, 0x69, 0x27, 0xc7, 0xa6, 0x11,
        0xb4, 0xd3, 0xa2, 0x17, 0x23, 0x50, 0xa0, 0xbd, 0x06, 0x7d, 0x5a, 0x72,
        0xa0, 0xb1, 0xed, 0x48, 0xd1, 0x42, 0xfc, 0x66, 0x3e, 0x4a, 0x22, 0x69,
        0xac, 0xe4, 0xee, 0x82, 0xbc, 0x48, 0x83, 0x81, 0x34, 0x6e, 0x29, 0x4b,
        0x64, 0x71, 0x37, 0x25, 0x13, 0x28, 0x52, 0x71, 0x5b, 0xd5, 0x95, 0x20,
        0xa5, 0xb4, 0x66, 0xa7, 0x9e, 0x06, 0x5f, 0x2d, 0x8e, 0x78, 0xf5, 0x37,
        0xcf, 0xed, 0x65, 0x84, 0xdf, 0xda, 0x78, 0x27, 0xa9, 0x09, 0xaa, 0x70,
        0x73, 0x5a, 0xc6, 0xa9, 0xba, 0xb7, 0xce, 0x38, 0x2c, 0x28, 0x4b, 0x3e,
        0xae, 0x11, 0x3c, 0xed, 0x94, 0xd9, 0x2a, 0x26, 0xd2, 0xbc, 0xa5, 0x19,
        0x7c, 0x3a, 0x98, 0x0a, 0x51, 0xdb, 0x14, 0x99, 0xd8, 0x4e, 0xc3, 0x5d,
        0x0a, 0xc9, 0x93, 0xa9, 0xce, 0xb0, 0x12, 0x62, 0x6b, 0x6b, 0x48, 0x42,
        0x42, 0x04, 0x95, 0x29, 0x77, 0x49, 0xaa, 0x88, 0x2a, 0x94, 0xcd, 0x55,
        0x7d, 0xb6, 0xcb, 0xb1, 0x1e, 0x93, 0xa9, 0xa2, 0xba, 0x73, 0xff, 0x2e,
        0xa6, 0xff, 0xd6, 0x14, 0x65, 0x3b, 0x8c, 0x7d, 0x0b, 0xa7, 0xda, 0xbd,
        0x50, 0x2c, 0x1d, 0x2e, 0xf1, 0xd9, 0xf5, 0x8a, 0x08, 0xe9, 0x54, 0x7d,
        0x4a, 0x25, 0xf5, 0xb7, 0x53, 0xd8, 0x3f, 0xad, 0x98, 0x5f, 0xea, 0xa9,
        0xd5, 0x3d, 0x13, 0x7d, 0x26, 0x5d, 0xab, 0x0e, 0xa6, 0xcd, 0xe7, 0xc1,
        0x81, 0x0f, 0x12, 0x8c, 0x59, 0x77, 0xa9, 0x67, 0xa4, 0x37, 0xf3, 0x8e,
        0xdf, 0xe5, 0x5c, 0x0c, 0x65, 0x07, 0x93, 0xcd, 0xb0, 0xeb, 0x19, 0x89,
        0x6f, 0x81, 0x90, 0x9a, 0xf4, 0x99, 0xb8, 0x33, 0x35, 0xdb, 0x40, 0x8e,
        0x85, 0x53, 0x26, 0x4a, 0xe9, 0x8c, 0x5a, 0x5d, 0x68, 0xd5, 0x4e, 0xff,
        0x21, 0x77, 0xb9, 0xcb, 0xc1, 0xaf, 0x69, 0x69, 0x10, 0x56, 0x6d, 0x9e,
        0xbd, 0xe4, 0xa4, 0x2b, 0xd9, 0xf9, 0x65, 0x63, 0xb5, 0x00, 0x48, 0xb0,
        0x04, 0xca, 0x98, 0x10, 0x8e, 0x2a, 0x4f, 0x18, 0x47, 0xef, 0x5e, 0x26,
        0x07, 0x72, 0xf9, 0xbe, 0x25, 0x02, 0x03, 0x01, 0x00, 0x01};

    String resultWith0Pad = "QUFB";
    String resultWith1Pad = "QUE=";
    String resultWith2Pad = "QQ==";

    String resultWith0PadRemoved = "QUFB";
    String resultWith1PadRemoved = "QUE";
    String resultWith2PadRemoved = "QQ";

    StdVectorUint8 inputFor0Pad =
        ConverterExtensions::StringToStdVectorUint8("AAA");
    StdVectorUint8 inputFor1Pad =
        ConverterExtensions::StringToStdVectorUint8("AA");
    StdVectorUint8 inputFor2Pad =
        ConverterExtensions::StringToStdVectorUint8("A");

    String TestMimeBase64String =
        R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXRjy3ExaGRO4vc
i/ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J+eEfxBgRbVQCxqoJwcMD/
PClKL8P/VmBKIhL+EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT+l/ToOUUGdo5eDtti
QWAr+x4aZTolSLrmc4818P2Z5B3pv2eL9B36+liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTX
nUKn38oN9BlKjXyTPxqlOe/NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC/GY+
SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z+1lhN/aeCepCapwc1rGqbq3
zjgsKEs+rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbL
sR6TqaK6c/8upv/WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX+qp1T0TfSZdqw6mzefB
gQ8SjFl3qWekN/OO3+VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7/IXe5y8GvaWkQ
Vm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ==)";

    String TestPemBase64String =
        R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXR
jy3ExaGRO4vci/ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lN
I5J+eEfxBgRbVQCxqoJwcMD/PClKL8P/VmBKIhL+EKTh652Cs3YcoBhMys1oQC5q
ISp7e8YLhRQZA0DpeFT+l/ToOUUGdo5eDttiQWAr+x4aZTolSLrmc4818P2Z5B3p
v2eL9B36+liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyT
PxqlOe/NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC/GY+SiJp
rOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z+1lhN/aeCepCapw
c1rGqbq3zjgsKEs+rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hC
QgSVKXdJqogqlM1VfbbLsR6TqaK6c/8upv/WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9
SiX1t1PYP62YX+qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN/OO3+VcDGUHk82w6xmJ
b4GQmvSZuDM120COhVMmSumMWl1o1U7/IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiw
BMqYEI4qTxhH714mB3L5viUCAwEAAQ==)";

    String TestBase64String =
        R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXRjy3ExaGRO4vci/ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J+eEfxBgRbVQCxqoJwcMD/PClKL8P/VmBKIhL+EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT+l/ToOUUGdo5eDttiQWAr+x4aZTolSLrmc4818P2Z5B3pv2eL9B36+liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe/NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC/GY+SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z+1lhN/aeCepCapwc1rGqbq3zjgsKEs+rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c/8upv/WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX+qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN/OO3+VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7/IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ==)";
    String TestBase64StringNoPadding =
        R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXRjy3ExaGRO4vci/ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J+eEfxBgRbVQCxqoJwcMD/PClKL8P/VmBKIhL+EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT+l/ToOUUGdo5eDttiQWAr+x4aZTolSLrmc4818P2Z5B3pv2eL9B36+liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe/NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC/GY+SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z+1lhN/aeCepCapwc1rGqbq3zjgsKEs+rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c/8upv/WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX+qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN/OO3+VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7/IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ)";
    String TestUrlSafeBase64String =
        R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY_tb52ZWXRjy3ExaGRO4vci_ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J-eEfxBgRbVQCxqoJwcMD_PClKL8P_VmBKIhL-EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT-l_ToOUUGdo5eDttiQWAr-x4aZTolSLrmc4818P2Z5B3pv2eL9B36-liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe_NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC_GY-SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z-1lhN_aeCepCapwc1rGqbq3zjgsKEs-rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c_8upv_WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX-qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN_OO3-VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7_IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ)";
    String TestFileNameSafeBase64String =
        R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY_tb52ZWXRjy3ExaGRO4vci_ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J-eEfxBgRbVQCxqoJwcMD_PClKL8P_VmBKIhL-EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT-l_ToOUUGdo5eDttiQWAr-x4aZTolSLrmc4818P2Z5B3pv2eL9B36-liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe_NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC_GY-SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z-1lhN_aeCepCapwc1rGqbq3zjgsKEs-rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c_8upv_WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX-qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN_OO3-VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7_IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ)";
    String TestUrlEncodedBase64String =
        R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY%2Ftb52ZWXRjy3ExaGRO4vci%2FZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J%2BeEfxBgRbVQCxqoJwcMD%2FPClKL8P%2FVmBKIhL%2BEKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT%2Bl%2FToOUUGdo5eDttiQWAr%2Bx4aZTolSLrmc4818P2Z5B3pv2eL9B36%2BliO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe%2FNbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC%2FGY%2BSiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z%2B1lhN%2FaeCepCapwc1rGqbq3zjgsKEs%2BrhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c%2F8upv%2FWFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX%2Bqp1T0TfSZdqw6mzefBgQ8SjFl3qWekN%2FOO3%2BVcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7%2FIXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ%3D%3D)";

    String dataStr;
    StdVectorUint8 dataStdVector8;
    String resultStr;
    StdVectorUint8 resultStdVector8;
    String expectedStr;
    StdVectorUint8 expectedStdVector8;

    dataStdVector8 = inputFor2Pad;
    expectedStr = resultWith2Pad;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 1", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = inputFor1Pad;
    expectedStr = resultWith1Pad;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 2", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = inputFor0Pad;
    expectedStr = resultWith0Pad;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 3", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = inputFor2Pad;
    expectedStr = resultWith2PadRemoved;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8, 0, true);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 4", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = inputFor1Pad;
    expectedStr = resultWith1PadRemoved;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8, 0, true);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 5", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = inputFor0Pad;
    expectedStr = resultWith0PadRemoved;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8, 0, true);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 6", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = testBytes;
    expectedStr = TestMimeBase64String;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8,
                                                         MIME_LINE_BREAKS_LENGTH);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 7", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = testBytes;
    expectedStr = TestPemBase64String;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8,
                                                         PEM_LINE_BREAKS_LENGTH);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 8", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = testBytes;
    expectedStr = TestBase64String;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 9", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = testBytes;
    expectedStr = TestBase64StringNoPadding;
    resultStr = Base64EncodingExtensions::EncodeToBase64(dataStdVector8, 0, true);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 10", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = testBytes;
    expectedStr = TestUrlSafeBase64String;
    resultStr = Base64EncodingExtensions::EncodeToUrlSafeBase64(dataStdVector8);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 11", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = testBytes;
    expectedStr = TestFileNameSafeBase64String;
    resultStr =
        Base64EncodingExtensions::EncodeToFilenameSafeBase64(dataStdVector8);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 12", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStdVector8 = testBytes;
    expectedStr = TestUrlEncodedBase64String;
    resultStr =
        Base64EncodingExtensions::EncodeToUrlEncodedBase64(dataStdVector8);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 13", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStr = TestBase64String;
    expectedStr = TestUrlSafeBase64String;
    resultStr = Base64EncodingExtensions::EncodeBase64ToUrlSafeBase64(dataStr);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 14", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStr = TestBase64String;
    expectedStr = TestFileNameSafeBase64String;
    resultStr =
        Base64EncodingExtensions::EncodeBase64ToFilenameSafeBase64(dataStr);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 15", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStr = TestBase64String;
    expectedStr = TestUrlEncodedBase64String;
    resultStr = Base64EncodingExtensions::EncodeBase64ToUrlEncodedBase64(dataStr);
    TestExtensions::Equals(
        NAMEOF(Base64EncodingExtensions) + " - Encode - 16", expectedStr,
        resultStr, LambdaExtensions::GetDefaultOutFunction<String>(), showValues);

    dataStr = resultWith2Pad;
    expectedStdVector8 = inputFor2Pad;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 1",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = resultWith1Pad;
    expectedStdVector8 = inputFor1Pad;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 2",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = resultWith0Pad;
    expectedStdVector8 = inputFor0Pad;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 3",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = resultWith2PadRemoved;
    expectedStdVector8 = inputFor2Pad;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 4",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = resultWith1PadRemoved;
    expectedStdVector8 = inputFor1Pad;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 5",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = resultWith0PadRemoved;
    expectedStdVector8 = inputFor0Pad;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 6",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = TestMimeBase64String;
    expectedStdVector8 = testBytes;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 7",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = TestPemBase64String;
    expectedStdVector8 = testBytes;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 8",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = TestBase64String;
    expectedStdVector8 = testBytes;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 9",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = TestBase64StringNoPadding;
    expectedStdVector8 = testBytes;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 10",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = TestUrlSafeBase64String;
    expectedStdVector8 = testBytes;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 11",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = TestFileNameSafeBase64String;
    expectedStdVector8 = testBytes;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 12",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);

    dataStr = TestUrlEncodedBase64String;
    expectedStdVector8 = testBytes;
    resultStdVector8 = Base64EncodingExtensions::DecodeFromBase64(dataStr);
    TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 13",
                           expectedStdVector8, resultStdVector8,
                           LambdaExtensions::GetStdVectorUint8OutFunction(),
                           showValues);
}