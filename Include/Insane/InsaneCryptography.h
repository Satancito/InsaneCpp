#pragma once
#include <memory>
#ifndef INSANE_CRYPOGRAPHY_H
#define INSANE_CRYPOGRAPHY_H

#include <Insane/Insane.h>
#include <Insane/InsaneCore.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneString.h>

#define USING_NS_INSANE_CRYPTO using namespace InsaneIO::Insane::Cryptography

#define MD5_DIGEST_LENGTH 16
#define SHA1_DIGEST_LENGTH 20
#define SHA256_DIGEST_LENGTH 32
#define SHA384_DIGEST_LENGTH 48
#define SHA512_DIGEST_LENGTH 64

#define MD5_ALGORITHM_NAME_STRING (R"(MD5)"s)
#define SHA1_ALGORITHM_NAME_STRING (R"(SHA-1)"s)
#define SHA256_ALGORITHM_NAME_STRING (R"(SHA-256)"s)
#define SHA384_ALGORITHM_NAME_STRING (R"(SHA-384)"s)
#define SHA512_ALGORITHM_NAME_STRING (R"(SHA-512)"s)

#define NO_LINE_BREAKS ((size_t)0)
#define MIME_LINE_BREAKS_LENGTH ((size_t)76)
#define PEM_LINE_BREAKS_LENGTH ((size_t)64)

#define HMAC_KEY_SIZE ((size_t)(16))

#define MD5_HASH_SIZE_IN_BYTES (static_cast<size_t>(16))
#define SHA1_HASH_SIZE_IN_BYTES (static_cast<size_t>(20))
#define SHA256_HASH_SIZE_IN_BYTES (static_cast<size_t>(32))
#define SHA384_HASH_SIZE_IN_BYTES (static_cast<size_t>(48))
#define SHA512_HASH_SIZE_IN_BYTES (static_cast<size_t>(64))

#define SCRYPT_ITERATIONS_FOR_INTERACTIVE_LOGIN (static_cast<size_t>(2048))
#define SCRYPT_ITERATIONS_FOR_ENCRYPTION (static_cast<size_t>(1048576))
#define SCRYPT_ITERATIONS (static_cast<size_t>(16384))
#define SCRYPT_SALT_SIZE (static_cast<size_t>(16))
#define SCRYPT_BLOCK_SIZE (static_cast<size_t>(8))
#define SCRYPT_PARALLELISM (static_cast<size_t>(1))
#define SCRYPT_DERIVED_KEY_LENGTH (static_cast<size_t>(64))

#define ARGON2_DERIVED_KEY_LENGTH (static_cast<size_t>(64))
#define ARGON2_SALT_SIZE (static_cast<size_t>(16))
#define ARGON2_ITERATIONS (static_cast<size_t>(2))
#define ARGON2_MEMORY_SIZE_IN_KIB (static_cast<size_t>(16384))
#define ARGON2_DEGREE_OF_PARALLELISM (static_cast<size_t>(4))

#define AES_MAX_IV_LENGTH ((size_t)16)
#define AES_BLOCK_SIZE_LENGTH ((size_t)16)
#define AES_MAX_KEY_LENGTH ((size_t)32)
#define __AES_PADDING_MODE_NONE_STRING (R"(NoPadding)"s)
#define __AES_PADDING_MODE_ZEROS_STRING (R"(ZeroPadding)"s)
#define __AES_PADDING_MODE_ANSIX923_STRING (R"(X9.23)"s)
#define __AES_PADDING_MODE_PKCS7_STRING (R"(PKCS7)"s)
#define __AES_MODE_CBC_STRING (R"(CBC)"s)
#define __AES_256_ALGORITHM_STRING (R"(AES-256)"s)

#define HMAC_INNER_PADDING (static_cast<uint8_t>(0x36))
#define HMAC_OUTER_PADDING (static_cast<uint8_t>(0x5c))
#define HMAC_64_BYTES_BLOCK_SIZE (static_cast<size_t>(64))
#define HMAC_128_BYTES_BLOCK_SIZE (static_cast<size_t>(128))

#define RSA_XML_KEY_MAIN_TAG_STRING (R"(<RSAKeyValue>)"s)
#define RSA_XML_KEY_MAIN_NODE_STRING (R"(RSAKeyValue)"s)
#define RSA_XML_KEY_P_NODE_STRING (R"(P)"s)
#define RSA_XML_KEY_Q_NODE_STRING (R"(Q)"s)
#define RSA_XML_KEY_DP_NODE_STRING (R"(DP)"s)
#define RSA_XML_KEY_DQ_NODE_STRING (R"(DQ)"s)
#define RSA_XML_KEY_INVERSEQ_NODE_STRING (R"(InverseQ)"s)
#define RSA_XML_KEY_D_NODE_STRING (R"(D)"s)
#define RSA_XML_KEY_MODULUS_NODE_STRING (R"(Modulus)"s)
#define RSA_XML_KEY_EXPONENT_NODE_STRING (R"(Exponent)"s)
#define __RSA_PADDING_PKCS1_V1_5_ALGORITHM_STRING (R"(EME-PKCS1-v1_5)"s)
#define __RSA_PADDING_OAEP_ALGORITHM_STRING (R"(EME-OAEP(SHA-1))"s)
#define __RSA_PADDING_OAEP_256_ALGORITHM_STRING (R"(EME-OAEP(SHA-256))"s)
#define __RSA_PADDING_OAEP_384_ALGORITHM_STRING (R"(EME-OAEP(SHA-384))"s)
#define __RSA_PADDING_OAEP_512_ALGORITHM_STRING (R"(EME-OAEP(SHA-512))"s)

#define RSA_PEM_INITIAL_TEXT_HEADER_STRING (R"(-----BEGIN )"s)
#define RSA_PEM_PRIVATE_KEY_INITIAL_STRING (R"(-----BEGIN PRIVATE KEY-----)"s)
#define RSA_PEM_PUBLIC_KEY_INITIAL_STRING (R"(-----BEGIN PUBLIC KEY-----)"s)
#define RSA_PEM_PRIVATE_KEY_FINAL_STRING (R"(-----END PRIVATE KEY-----)"s)
#define RSA_PEM_PUBLIC_KEY_FINAL_STRING (R"(-----END PUBLIC KEY-----)"s)

#define RSA_PEM_PUBLIC_KEY_REGEX_PATTERN (R"(^(?:(-----BEGIN PUBLIC KEY-----)(?:\r|\n|\r\n)((?:(?:(?:[A-Za-z0-9+\/]{4}){16}(?:\r|\n|\r\n))+)(?:(?:[A-Za-z0-9+\/]{4}){0,15})(?:(?:[A-Za-z0-9+\/]{4}|[A-Za-z0-9+\/]{2}==|[A-Za-z0-9+\/]{3}=)))(?:\r|\n|\r\n)(-----END PUBLIC KEY-----))$)"s)
#define RSA_PEM_PRIVATE_KEY_REGEX_PATTERN (R"(^(?:(-----BEGIN PRIVATE KEY-----)(?:\r|\n|\r\n)((?:(?:(?:[A-Za-z0-9+\/]{4}){16}(?:\r|\n|\r\n))+)(?:(?:[A-Za-z0-9+\/]{4}){0,15})(?:(?:[A-Za-z0-9+\/]{4}|[A-Za-z0-9+\/]{2}==|[A-Za-z0-9+\/]{3}=)))(?:\r|\n|\r\n)(-----END PRIVATE KEY-----))$)"s)
#define RSA_PEM_RSA_PUBLIC_KEY_REGEX_PATTERN (R"(^(?:(-----BEGIN RSA PUBLIC KEY-----)(?:\r|\n|\r\n)((?:(?:(?:[A-Za-z0-9+\/]{4}){16}(?:\r|\n|\r\n))+)(?:(?:[A-Za-z0-9+\/]{4}){0,15})(?:(?:[A-Za-z0-9+\/]{4}|[A-Za-z0-9+\/]{2}==|[A-Za-z0-9+\/]{3}=)))(?:\r|\n|\r\n)(-----END RSA PUBLIC KEY-----))$)"s)
#define RSA_PEM_RSA_PRIVATE_KEY_REGEX_PATTERN (R"(^(?:(-----BEGIN RSA PRIVATE KEY-----)(?:\r|\n|\r\n)((?:(?:(?:[A-Za-z0-9+\/]{4}){16}(?:\r|\n|\r\n))+)(?:(?:[A-Za-z0-9+\/]{4}){0,15})(?:(?:[A-Za-z0-9+\/]{4}|[A-Za-z0-9+\/]{2}==|[A-Za-z0-9+\/]{3}=)))(?:\r|\n|\r\n)(-----END RSA PRIVATE KEY-----))$)"s)
#define RSA_XML_PUBLIC_KEY_REGEX_PATTERN (R"(^(\s*<RSAKeyValue>\s*(?:\s*<Modulus>[a-zA-Z0-9\+\/]+={0,2}<\/Modulus>()|\s*<Exponent>[a-zA-Z0-9\+\/]+={0,2}<\/Exponent>()){2}\s*<\/\s*RSAKeyValue\s*>\2\3\s*)$)"s)
#define RSA_XML_PRIVATE_KEY_REGEX_PATTERN (R"(^(\s*<RSAKeyValue>\s*(?:\s*<Modulus>[a-zA-Z0-9\+\/]+={0,2}<\/Modulus>()|\s*<Exponent>[a-zA-Z0-9\+\/]+={0,2}<\/Exponent>()|\s*<P>[a-zA-Z0-9\+\/]+={0,2}<\/P>()|\s*<Q>[a-zA-Z0-9\+\/]+={0,2}<\/Q>()|\s*<DP>[a-zA-Z0-9\+\/]+={0,2}<\/DP>()|\s*<DQ>[a-zA-Z0-9\+\/]+={0,2}<\/DQ>()|\s*<InverseQ>[a-zA-Z0-9\+\/]+={0,2}<\/InverseQ>()|\s*<D>[a-zA-Z0-9\+\/]+={0,2}<\/D>()){8}\s*<\/\s*RSAKeyValue\s*>\2\3\4\5\6\7\8\9\s*)$)"s)

#define HEX_ENCODER_NAME_STRING (R"(InsaneIO.Insane.Cryptography.HexEncoder, InsaneIO.Insane)"s)
#define BASE32_ENCODER_NAME_STRING (R"(InsaneIO.Insane.Cryptography.Base32Encoder, InsaneIO.Insane)"s)
#define BASE64_ENCODER_NAME_STRING (R"(InsaneIO.Insane.Cryptography.Base64Encoder, InsaneIO.Insane)"s)

#define SHA_HASHER_NAME_STRING (R"(InsaneIO.Insane.Cryptography.ShaHasher, InsaneIO.Insane)"s)
#define HMAC_HASHER_NAME_STRING (R"(InsaneIO.Insane.Cryptography.HmacHasher, InsaneIO.Insane)"s)
#define ARGON2_HASHER_NAME_STRING (R"(InsaneIO.Insane.Cryptography.Argon2Hasher, InsaneIO.Insane)"s)
#define SCRYPT_HASHER_NAME_STRING (R"(InsaneIO.Insane.Cryptography.ScryptHasher, InsaneIO.Insane)"s)

#define AES_CBC_ENCRYPTOR_NAME_STRING (R"(InsaneIO.Insane.Cryptography.AesCbcEncryptor, InsaneIO.Insane)"s)
#define RSA_ENCRYPTOR_NAME_STRING (R"(InsaneIO.Insane.Cryptography.RsaEncryptor, InsaneIO.Insane)"s)

#define AES_CBC_PROTECTOR_NAME_STRING (R"(InsaneIO.Insane.Cryptography.AesCbcProtector, InsaneIO.Insane)"s)

#define BASE64_VALUE_REGEX_PATTERN_STRING (R"(^(?:(?:[A-Za-z0-9+\/]{4})*)(?:[A-Za-z0-9+\/]{2}==|[A-Za-z0-9+\/]{3}=)?$)"s)

USING_NS_INSANE_EXCEPTION;
USING_NS_INSANE_INTERFACES;
USING_NS_INSANE_CORE;
namespace InsaneIO::Insane::Cryptography
{

    // ███ Enums
    INSANE_ENUM(AesCbcPadding,
                None, _, _,
                Zeros, _, _,
                Pkcs7, _, _,
                AnsiX923, _, _);

    INSANE_ENUM(Argon2Variant,
                Argon2d, _, _,
                Argon2i, _, _,
                Argon2id, _, _);

    INSANE_ENUM(Base64Encoding,
                Base64, _, _,
                UrlSafeBase64, _, _,
                FileNameSafeBase64, _, _,
                UrlEncodedBase64, _, _);

    INSANE_ENUM(HashAlgorithm,
                Md5, _, _,
                Sha1, _, _,
                Sha256, _, _,
                Sha384, _, _,
                Sha512, _, _);

    INSANE_ENUM(RsaKeyPairEncoding,
                Ber, _, _,
                Pem, _, _,
                Xml, _, _);

    INSANE_ENUM(RsaKeyEncoding,
                Unknown, _, _,
                BerPublic, _, _,
                BerPrivate, _, _,
                PemPublic, _, _,
                PemPrivate, _, _,
                XmlPublic, _, _,
                XmlPrivate, _, _);

    INSANE_ENUM(RsaPadding,
                Pkcs1, _, _,
                OaepSha1, _, _,
                OaepSha256, _, _,
                OaepSha384, _, _,
                OaepSha512, _, _);

    // ███ RandomExtensions ███
    class INSANE_API RandomExtensions
    {
    public:
        [[nodiscard]] static int NextValue();
        [[nodiscard]] static int NextValue(int min, int max);
        [[nodiscard]] static StdVectorUint8 NextBytes(size_t sz);

    private:
    };

    // ███ HexEncodingExtensions ███
    class INSANE_API HexEncodingExtensions
    {
    public:
        [[nodiscard]] static StdVectorUint8 DecodeFromHex(const String &data);
        [[nodiscard]] static String EncodeToHex(const StdVectorUint8 &data, const bool &toUpper = false);
        [[nodiscard]] static String EncodeToHex(const String &data, const bool &toUpper = false);

    private:
    };

    // ███ Base32EncodingExtensions ███
    class INSANE_API Base32EncodingExtensions
    {
    public:
        [[nodiscard]] static StdVectorUint8 DecodeFromBase32(const String &data);
        [[nodiscard]] static String EncodeToBase32(const StdVectorUint8 &data, const bool &removePadding = false, const bool &toLower = false);
        [[nodiscard]] static String EncodeToBase32(const String &data, const bool &removePadding = false, const bool &toLower = false);

    private:
    };

    // ███ Base64EncodingExtensions ███
    class INSANE_API Base64EncodingExtensions
    {
    public:
        [[nodiscard]] static StdVectorUint8 DecodeFromBase64(const String &data);
        [[nodiscard]] static String EncodeToBase64(const StdVectorUint8 &data, const size_t &lineBreaksLength = NO_LINE_BREAKS, const bool &removePadding = false);
        [[nodiscard]] static String EncodeToBase64(const String &data, const size_t &lineBreaksLength = NO_LINE_BREAKS, const bool &removePadding = false);
        [[nodiscard]] static String EncodeToUrlSafeBase64(const StdVectorUint8 &data);
        [[nodiscard]] static String EncodeToUrlSafeBase64(const String &data);
        [[nodiscard]] static String EncodeToFilenameSafeBase64(const StdVectorUint8 &data);
        [[nodiscard]] static String EncodeToFilenameSafeBase64(const String &data);
        [[nodiscard]] static String EncodeToUrlEncodedBase64(const StdVectorUint8 &data);
        [[nodiscard]] static String EncodeToUrlEncodedBase64(const String &data);
        [[nodiscard]] static String EncodeBase64ToUrlSafeBase64(const String &base64);
        [[nodiscard]] static String EncodeBase64ToFilenameSafeBase64(const String &base64);
        [[nodiscard]] static String EncodeBase64ToUrlEncodedBase64(const String &base64);

    private:
    };

    // ███ IEncoder ███
    class INSANE_API IEncoder : public IJsonSerialize<IEncoder>,
                                public IClone<IEncoder>
    {
    public:
        virtual ~IEncoder() = default;
        IEncoder(const String &name);
        [[nodiscard]] virtual String Encode(const StdVectorUint8 &data) const = 0;
        [[nodiscard]] virtual String Encode(const String &data) const = 0;
        [[nodiscard]] virtual StdVectorUint8 Decode(const String &data) const = 0;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override = 0;
        [[nodiscard]] static std::shared_ptr<IEncoder> Deserialize(const String &json);
        [[nodiscard]] static const std::shared_ptr<IEncoder> DefaultInstance();

    private:
    };

    // ███ HexEncoder ███
    class INSANE_API HexEncoder : public IEncoder
    {
    public:
        virtual ~HexEncoder() = default;
        HexEncoder(const bool &toUpper = false);
        [[nodiscard]] bool GetToUpper() const;
        [[nodiscard]] virtual String Encode(const StdVectorUint8 &data) const override;
        [[nodiscard]] virtual String Encode(const String &data) const override;
        [[nodiscard]] virtual StdVectorUint8 Decode(const String &data) const override;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] std::shared_ptr<IEncoder> Clone() const override;
        [[nodiscard]] static std::shared_ptr<IEncoder> Deserialize(const String &json);
        [[nodiscard]] static std::shared_ptr<IEncoder> DefaultInstance();
        [[nodiscard]] static std::shared_ptr<IEncoder> CreateInstance(const bool &toUpper = false);

    private:
        const bool ToUpper;
    };

    // ███ Base32Encoder ███
    class INSANE_API Base32Encoder : public IEncoder
    {
    public:
        virtual ~Base32Encoder() = default;
        Base32Encoder(const bool &removePadding = false, const bool &toLower = false);
        [[nodiscard]] size_t GetToLower() const;
        [[nodiscard]] bool GetRemovePadding() const;
        [[nodiscard]] virtual String Encode(const StdVectorUint8 &data) const override;
        [[nodiscard]] virtual String Encode(const String &data) const override;
        [[nodiscard]] virtual StdVectorUint8 Decode(const String &data) const override;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] std::shared_ptr<IEncoder> Clone() const override;
        [[nodiscard]] static std::shared_ptr<IEncoder> DefaultInstance();
        [[nodiscard]] static std::shared_ptr<IEncoder> Deserialize(const String &json);
        [[nodiscard]] static std::shared_ptr<IEncoder> CreateInstance(const bool &removePadding = false, const bool &toLower = false);

    private:
        const bool RemovePadding;
        const bool ToLower;
    };

    // ███ Base64Encoder ███
    class INSANE_API Base64Encoder : public IEncoder
    {
    public:
        virtual ~Base64Encoder() = default;
        Base64Encoder(const size_t &lineBreaksLength = NO_LINE_BREAKS, const bool &removePadding = false, const Base64Encoding &encodingType = Base64Encoding::Base64);
        [[nodiscard]] size_t GetLineBreaksLength() const;
        [[nodiscard]] bool GetRemovePadding() const;
        [[nodiscard]] Base64Encoding GetEncodingType() const;
        [[nodiscard]] virtual String Encode(const StdVectorUint8 &data) const override;
        [[nodiscard]] virtual String Encode(const String &data) const override;
        [[nodiscard]] virtual StdVectorUint8 Decode(const String &data) const override;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] std::shared_ptr<IEncoder> Clone() const override;
        [[nodiscard]] static std::shared_ptr<IEncoder> Deserialize(const String &json);
        [[nodiscard]] static std::shared_ptr<IEncoder> DefaultInstance();
        [[nodiscard]] static std::shared_ptr<IEncoder> CreateInstance(const size_t &lineBreaksLength = NO_LINE_BREAKS, const bool &removePadding = false, const Base64Encoding &encodingType = Base64Encoding::Base64);

    private:
        const size_t LineBreaksLength;
        const bool RemovePadding;
        const Base64Encoding EncodingType;
    };

    // ███ HashExtensions ███
    class INSANE_API HashExtensions
    {
    public:
        [[nodiscard]] static StdVectorUint8 ComputeHash(const StdVectorUint8 &data, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static StdVectorUint8 ComputeHash(const String &data, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static String ComputeEncodedHash(const StdVectorUint8 &data, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static String ComputeEncodedHash(const String &data, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);

        [[nodiscard]] static bool VerifyHash(const StdVectorUint8 &data, const StdVectorUint8 expected, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyHash(const String &data, const StdVectorUint8 expected, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyEncodedHash(const StdVectorUint8 &data, const String expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyEncodedHash(const String &data, const String expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);

        [[nodiscard]] static StdVectorUint8 ComputeHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static StdVectorUint8 ComputeHmac(const String &data, const String &key, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static StdVectorUint8 ComputeHmac(const StdVectorUint8 &data, const String &key, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static StdVectorUint8 ComputeHmac(const String &data, const StdVectorUint8 &key, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static String ComputeEncodedHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static String ComputeEncodedHmac(const String &data, const String &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static String ComputeEncodedHmac(const StdVectorUint8 &data, const String &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static String ComputeEncodedHmac(const String &data, const StdVectorUint8 &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);

        [[nodiscard]] static bool VerifyHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, const StdVectorUint8 &expected, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyHmac(const String &data, const String &key, const StdVectorUint8 &expected, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyHmac(const StdVectorUint8 &data, const String &key, const StdVectorUint8 &expected, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyHmac(const String &data, const StdVectorUint8 &key, const StdVectorUint8 &expected, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyEncodedHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyEncodedHmac(const String &data, const String &key, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyEncodedHmac(const StdVectorUint8 &data, const String &key, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
        [[nodiscard]] static bool VerifyEncodedHmac(const String &data, const StdVectorUint8 &key, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);

        [[nodiscard]] static StdVectorUint8 ComputeArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static StdVectorUint8 ComputeArgon2(const String &data, const String &salt, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static StdVectorUint8 ComputeArgon2(const StdVectorUint8 &data, const String &salt, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static StdVectorUint8 ComputeArgon2(const String &data, const StdVectorUint8 &salt, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static String ComputeEncodedArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static String ComputeEncodedArgon2(const String &data, const String &salt, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static String ComputeEncodedArgon2(const StdVectorUint8 &data, const String &salt, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static String ComputeEncodedArgon2(const String &data, const StdVectorUint8 &salt, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);

        [[nodiscard]] static bool VerifyArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, const StdVectorUint8 &expected, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyArgon2(const String &data, const String &salt, const StdVectorUint8 &expected, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyArgon2(const StdVectorUint8 &data, const String &salt, const StdVectorUint8 &expected, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyArgon2(const String &data, const StdVectorUint8 &salt, const StdVectorUint8 &expected, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyEncodedArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyEncodedArgon2(const String &data, const String &salt, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyEncodedArgon2(const StdVectorUint8 &data, const String &salt, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyEncodedArgon2(const String &data, const StdVectorUint8 &salt, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);

        [[nodiscard]] static StdVectorUint8 ComputeScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static StdVectorUint8 ComputeScrypt(const String &data, const String &salt, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static StdVectorUint8 ComputeScrypt(const StdVectorUint8 &data, const String &salt, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static StdVectorUint8 ComputeScrypt(const String &data, const StdVectorUint8 &salt, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static String ComputeEncodedScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static String ComputeEncodedScrypt(const String &data, const String &salt, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static String ComputeEncodedScrypt(const StdVectorUint8 &data, const String &salt, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static String ComputeEncodedScrypt(const String &data, const StdVectorUint8 &salt, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);

        [[nodiscard]] static bool VerifyScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, const StdVectorUint8 &expected, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyScrypt(const String &data, const String &salt, const StdVectorUint8 &expected, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyScrypt(const StdVectorUint8 &data, const String &salt, const StdVectorUint8 &expected, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyScrypt(const String &data, const StdVectorUint8 &salt, const StdVectorUint8 &expected, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyEncodedScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyEncodedScrypt(const String &data, const String &salt, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyEncodedScrypt(const StdVectorUint8 &data, const String &salt, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
        [[nodiscard]] static bool VerifyEncodedScrypt(const String &data, const StdVectorUint8 &salt, const String &expected, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCK_SIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);

    private:
    };

    // ███ AesExtensions ███
    class INSANE_API AesExtensions
    {
    public:
        [[nodiscard]] static StdVectorUint8 EncryptAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
        [[nodiscard]] static StdVectorUint8 EncryptAesCbc(const String &data, const String &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
        [[nodiscard]] static StdVectorUint8 EncryptAesCbc(const StdVectorUint8 &data, const String &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
        [[nodiscard]] static StdVectorUint8 EncryptAesCbc(const String &data, const StdVectorUint8 &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);

        [[nodiscard]] static String EncryptEncodedAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
        [[nodiscard]] static String EncryptEncodedAesCbc(const String &data, const String &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
        [[nodiscard]] static String EncryptEncodedAesCbc(const StdVectorUint8 &data, const String &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
        [[nodiscard]] static String EncryptEncodedAesCbc(const String &data, const StdVectorUint8 &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);

        [[nodiscard]] static StdVectorUint8 DecryptAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
        [[nodiscard]] static StdVectorUint8 DecryptAesCbc(const StdVectorUint8 &data, const String &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);

        [[nodiscard]] static StdVectorUint8 DecryptEncodedAesCbc(const String &data, const StdVectorUint8 &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
        [[nodiscard]] static StdVectorUint8 DecryptEncodedAesCbc(const String &data, const String &key, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);

    private:
        [[nodiscard]] static StdVectorUint8 GenerateNormalizedKey(const StdVectorUint8 &key);
        static void ValidateKey(const StdVectorUint8 &key);
    };

    // ███ RsaKeyPair ███
    class INSANE_API RsaKeyPair : public IJsonSerialize<RsaKeyPair>
    {
    public:
        RsaKeyPair(const String &publicKey, const String &privateKey);
        [[nodiscard]] String GetPublicKey() const;
        [[nodiscard]] String GetPrivateKey() const;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] static RsaKeyPair Deserialize(const String &json);

    private:
        const String PublicKey;
        const String PrivateKey;
        const String Provider;
    };

    // ███ RsaExtensions ███
    class INSANE_API RsaExtensions
    {
    public:
        [[nodiscard]] static RsaKeyPair CreateRsaKeyPair(const size_t &keySize = 4096, const RsaKeyPairEncoding &encoding = RsaKeyPairEncoding::Ber);
        [[nodiscard]] static StdVectorUint8 EncryptRsa(const StdVectorUint8 &data, const String &publicKey, const RsaPadding &padding = RsaPadding::OaepSha256);
        [[nodiscard]] static StdVectorUint8 EncryptRsa(const String &data, const String &publicKey, const RsaPadding &padding = RsaPadding::OaepSha256);
        [[nodiscard]] static String EncryptEncodedRsa(const StdVectorUint8 &data, const String &publicKey, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const RsaPadding &padding = RsaPadding::OaepSha256);
        [[nodiscard]] static String EncryptEncodedRsa(const String &data, const String &publicKey, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const RsaPadding &padding = RsaPadding::OaepSha256);
        [[nodiscard]] static StdVectorUint8 DecryptRsa(const StdVectorUint8 &data, const String &privateKey, const RsaPadding &padding = RsaPadding::OaepSha256);
        [[nodiscard]] static StdVectorUint8 DecryptEncodedRsa(const String &data, const String &privateKey, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance(), const RsaPadding &padding = RsaPadding::OaepSha256);
        [[nodiscard]] static RsaKeyEncoding GetRsaKeyEncoding(const String &key);
        [[nodiscard]] static bool ValidateRsaPublicKey(const String &publicKey);
        [[nodiscard]] static bool ValidateRsaPrivateKey(const String &privateKey);

    private:
    };

    // ███ IHasher ███
    class INSANE_API IHasher : public IJsonSerialize<IHasher>, public IClone<IHasher>
    {
    public:
        virtual ~IHasher() = default;
        IHasher(const String &name);
        [[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) = 0;
        [[nodiscard]] virtual StdVectorUint8 Compute(const String &data) = 0;
        [[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) = 0;
        [[nodiscard]] virtual String ComputeEncoded(const String &data) = 0;
        [[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) = 0;
        [[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) = 0;
        [[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) = 0;
        [[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) = 0;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override = 0;
        [[nodiscard]] static std::shared_ptr<IHasher> Deserialize(const String &json);

    private:
    };

    // ███ ShaHasher ███
    class INSANE_API ShaHasher : public IHasher
    {
    public:
        virtual ~ShaHasher() = default;
        ShaHasher(const HashAlgorithm &hashAlgorithm = HashAlgorithm::Sha512, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance());
        ShaHasher(const ShaHasher &instance);
        [[nodiscard]] HashAlgorithm GetHashAlgorithm() const;
        [[nodiscard]] std::shared_ptr<IEncoder> GetEncoder() const;
        [[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) override;
        [[nodiscard]] virtual StdVectorUint8 Compute(const String &data) override;
        [[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) override;
        [[nodiscard]] virtual String ComputeEncoded(const String &data) override;
        [[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) override;
        [[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) override;
        [[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) override;
        [[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) override;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] std::shared_ptr<IHasher> Clone() const override;
        [[nodiscard]] static std::shared_ptr<IHasher> Deserialize(const String &json);

    private:
        const HashAlgorithm _HashAlgorithm;
        const std::shared_ptr<IEncoder> _Encoder;
    };

    // ███ HmacHasher ███
    class INSANE_API HmacHasher : public IHasher
    {
    public:
        virtual ~HmacHasher() = default;
        HmacHasher(const StdVectorUint8 &key = RandomExtensions::NextBytes(HMAC_KEY_SIZE), const HashAlgorithm &hashAlgorithm = HashAlgorithm::Sha512, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance());
        HmacHasher(const HmacHasher &instance);
        [[nodiscard]] HashAlgorithm GetHashAlgorithm() const;
        [[nodiscard]] std::shared_ptr<IEncoder> GetEncoder() const;
        [[nodiscard]] StdVectorUint8 GetKey() const;
        [[nodiscard]] String GetKeyEncoded() const;
        [[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) override;
        [[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) override;
        [[nodiscard]] virtual StdVectorUint8 Compute(const String &data) override;
        [[nodiscard]] virtual String ComputeEncoded(const String &data) override;
        [[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) override;
        [[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) override;
        [[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) override;
        [[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) override;
        [[nodiscard]] std::shared_ptr<IHasher> Clone() const override;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] static std::shared_ptr<IHasher> Deserialize(const String &json);

    private:
        const HashAlgorithm _HashAlgorithm;
        const std::shared_ptr<IEncoder> _Encoder;
        const StdVectorUint8 _Key;
    };

    // ███ Argon2Hasher ███
    class INSANE_API Argon2Hasher : public IHasher
    {
    public:
        virtual ~Argon2Hasher() = default;
        Argon2Hasher(const StdVectorUint8 &salt = RandomExtensions::NextBytes(ARGON2_SALT_SIZE), const unsigned int &iterations = ARGON2_ITERATIONS, const unsigned int &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const unsigned int &degreeOfParallelism = ARGON2_DEGREE_OF_PARALLELISM, const Argon2Variant argon2Variant = Argon2Variant::Argon2id, const unsigned int &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance());
        Argon2Hasher(const Argon2Hasher &instance);
        [[nodiscard]] StdVectorUint8 GetSalt() const;
        [[nodiscard]] String GetSaltEncoded() const;
        [[nodiscard]] unsigned int GetIterations() const;
        [[nodiscard]] unsigned int GetMemorySizeKiB() const;
        [[nodiscard]] unsigned int GetDegreeOfParallelism() const;
        [[nodiscard]] unsigned int GetDerivedKeyLength() const;
        [[nodiscard]] Argon2Variant GetArgon2Variant() const;
        [[nodiscard]] std::shared_ptr<IEncoder> GetEncoder() const;
        [[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) override;
        [[nodiscard]] virtual StdVectorUint8 Compute(const String &data) override;
        [[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) override;
        [[nodiscard]] virtual String ComputeEncoded(const String &data) override;
        [[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) override;
        [[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) override;
        [[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) override;
        [[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) override;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] std::shared_ptr<IHasher> Clone() const override;
        [[nodiscard]] static std::shared_ptr<IHasher> Deserialize(const String &json);

    private:
        const StdVectorUint8 _Salt;
        const unsigned int _Iterations;
        const unsigned int _MemorySizeKiB;
        const unsigned int _DegreeOfParallelism;
        const unsigned int _DerivedKeyLength;
        const Argon2Variant _Argon2Variant;
        const std::shared_ptr<IEncoder> _Encoder;
    };

    // ███ ScryptHasher ███
    class INSANE_API ScryptHasher : public IHasher
    {
    public:
        virtual ~ScryptHasher() = default;
        ScryptHasher(const StdVectorUint8 &salt = RandomExtensions::NextBytes(SCRYPT_SALT_SIZE), const unsigned int &iterations = SCRYPT_ITERATIONS, const unsigned int &blockSize = SCRYPT_BLOCK_SIZE, const unsigned int &parallelism = SCRYPT_PARALLELISM, const unsigned int &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance());
        ScryptHasher(const ScryptHasher &instance);
        [[nodiscard]] StdVectorUint8 GetSalt() const;
        [[nodiscard]] String GetSaltEncoded() const;
        [[nodiscard]] unsigned int GetIterations() const;
        [[nodiscard]] unsigned int GetBlockSize() const;
        [[nodiscard]] unsigned int GetParallelism() const;
        [[nodiscard]] unsigned int GetDerivedKeyLength() const;
        [[nodiscard]] std::shared_ptr<IEncoder> GetEncoder() const;
        [[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) override;
        [[nodiscard]] virtual StdVectorUint8 Compute(const String &data) override;
        [[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) override;
        [[nodiscard]] virtual String ComputeEncoded(const String &data) override;
        [[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) override;
        [[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) override;
        [[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) override;
        [[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) override;
        [[nodiscard]] std::shared_ptr<IHasher> Clone() const override;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] static std::shared_ptr<IHasher> Deserialize(const String &json);

    private:
        const StdVectorUint8 _Salt;
        const unsigned int _Iterations;
        const unsigned int _BlockSize;
        const unsigned int _Parallelism;
        const unsigned int _DerivedKeyLength;
        const std::shared_ptr<IEncoder> _Encoder;
    };

    // ███ IEncryptor ███
    class INSANE_API IEncryptor : public IJsonSerialize<IEncryptor>, public IClone<IEncryptor>
    {
    public:
        virtual ~IEncryptor() = default;
        IEncryptor(const String &name);
        [[nodiscard]] virtual StdVectorUint8 Encrypt(const StdVectorUint8 &data) const = 0;
        [[nodiscard]] virtual StdVectorUint8 Encrypt(const String &data) const = 0;
        [[nodiscard]] virtual String EncryptEncoded(const StdVectorUint8 &data) const = 0;
        [[nodiscard]] virtual String EncryptEncoded(const String &data) const = 0;
        [[nodiscard]] virtual StdVectorUint8 Decrypt(const StdVectorUint8 &data) const = 0;
        [[nodiscard]] virtual StdVectorUint8 DecryptEncoded(const String &data) const = 0;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override = 0;
        [[nodiscard]] static std::shared_ptr<IEncryptor> Deserialize(const String &json);

    private:
    };

    // ███ AesCbcEncryptor ███
    class INSANE_API AesCbcEncryptor : public IEncryptor
    {
    public:
        virtual ~AesCbcEncryptor() = default;
        AesCbcEncryptor(const StdVectorUint8 &key = RandomExtensions::NextBytes(AES_MAX_KEY_LENGTH), const AesCbcPadding &padding = AesCbcPadding::Pkcs7, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance());
        AesCbcEncryptor(const AesCbcEncryptor &instance);
        [[nodiscard]] StdVectorUint8 GetKey() const;
        [[nodiscard]] String GetKeyEncoded() const;
        [[nodiscard]] AesCbcPadding GetPadding() const;
        [[nodiscard]] std::shared_ptr<IEncoder> GetEncoder() const;
        [[nodiscard]] virtual StdVectorUint8 Encrypt(const StdVectorUint8 &data) const override;
        [[nodiscard]] virtual String EncryptEncoded(const StdVectorUint8 &data) const override;
        [[nodiscard]] virtual StdVectorUint8 Encrypt(const String &data) const override;
        [[nodiscard]] virtual String EncryptEncoded(const String &data) const override;
        [[nodiscard]] virtual StdVectorUint8 Decrypt(const StdVectorUint8 &data) const override;
        [[nodiscard]] virtual StdVectorUint8 DecryptEncoded(const String &data) const override;
        [[nodiscard]] std::shared_ptr<IEncryptor> Clone() const override;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] static std::shared_ptr<IEncryptor> Deserialize(const String &json);

    private:
        const StdVectorUint8 _Key;
        const AesCbcPadding _Padding;
        const std::shared_ptr<IEncoder> _Encoder;
    };

    // ███ RsaEncryptor ███
    class INSANE_API RsaEncryptor : public IEncryptor
    {
    public:
        virtual ~RsaEncryptor() = default;
        RsaEncryptor(const RsaKeyPair &keyPair, const RsaPadding &padding = RsaPadding::OaepSha256, const std::shared_ptr<IEncoder> &encoder = Base64Encoder::DefaultInstance());
        RsaEncryptor(const RsaEncryptor &instance);
        [[nodiscard]] RsaKeyPair GetKeyPair() const;
        [[nodiscard]] RsaPadding GetPadding() const;
        [[nodiscard]] std::shared_ptr<IEncoder> GetEncoder() const;
        [[nodiscard]] virtual StdVectorUint8 Encrypt(const StdVectorUint8 &data) const override;
        [[nodiscard]] virtual String EncryptEncoded(const StdVectorUint8 &data) const override;
        [[nodiscard]] virtual StdVectorUint8 Encrypt(const String &data) const override;
        [[nodiscard]] virtual String EncryptEncoded(const String &data) const override;
        [[nodiscard]] virtual StdVectorUint8 Decrypt(const StdVectorUint8 &data) const override;
        [[nodiscard]] virtual StdVectorUint8 DecryptEncoded(const String &data) const override;
        [[nodiscard]] std::shared_ptr<IEncryptor> Clone() const override;
        [[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
        [[nodiscard]] static std::shared_ptr<IEncryptor> Deserialize(const String &json);

    private:
        const RsaKeyPair _KeyPair;
        const RsaPadding _Padding;
        const std::shared_ptr<IEncoder> _Encoder;
    };

    class INSANE_API CryptoTests
    {
    public:
        static void HexEncodingExtensionsTests(const bool &showValues = true);
        static void Base32EncodingExtensionsTests(const bool &showValues = true);
        static void Base64EncodingExtensionsTests(const bool &showValues = true);

    private:
    };
} // namespace InsaneIO::Insane::Cryptography

#endif // !INSANE_CRYPOGRAPHY_H