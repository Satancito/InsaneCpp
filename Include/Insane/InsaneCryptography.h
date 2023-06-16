#pragma once
#ifndef INSANE_CRYPOGRAPHY_H
#define INSANE_CRYPOGRAPHY_H

#include <Insane/Insane.h>
#include <Insane/InsaneString.h>
#include <Insane/InsaneCore.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>
#include <algorithm>
#include <functional>
#include <utility>

#define USING_NS_INSANE_CRYPTO using namespace InsaneIO::Insane::Cryptography

#define MD5_DIGEST_LENGTH 16
#define SHA1_DIGEST_LENGTH 20
#define SHA256_DIGEST_LENGTH 32
#define SHA384_DIGEST_LENGTH 48
#define SHA512_DIGEST_LENGTH 64

#define MD5_ALGORITHM_NAME_STRING ("MD5"s)
#define SHA1_ALGORITHM_NAME_STRING ("SHA-1"s)
#define SHA256_ALGORITHM_NAME_STRING ("SHA-256"s)
#define SHA384_ALGORITHM_NAME_STRING ("SHA-384"s)
#define SHA512_ALGORITHM_NAME_STRING ("SHA-512"s)

#define NO_LINE_BREAKS ((size_t)0)
#define MIME_LINE_BREAKS_LENGTH ((size_t)76)
#define PEM_LINE_BREAKS_LENGTH ((size_t)64)

#define HMAC_KEY_SIZE ((size_t)(16))

#define MD5_HASH_SIZE_IN_BYTES (static_cast<size_t>(16))
#define SHA1_HASH_SIZE_IN_BYTES (static_cast<size_t>(20))
#define SHA256_HASH_SIZE_IN_BYTES (static_cast<size_t>(32))
#define SHA384_HASH_SIZE_IN_BYTES (static_cast<size_t>(48))
#define SHA512_HASH_SIZE_IN_BYTES (static_cast<size_t>(64))

#define SCRYPT_ITERATIONS_FORINTERACTIVE_LOGIN (static_cast<size_t>(2048))
#define SCRYPT_ITERATIONS_FOR_ENCRYPTION (static_cast<size_t>(1048576))
#define SCRYPT_ITERATIONS (static_cast<size_t>(16384))
#define SCRYPT_SALT_SIZE (static_cast<size_t>(16))
#define SCRYPT_BLOCKSIZE (static_cast<size_t>(8))
#define SCRYPT_PARALLELISM (static_cast<size_t>(1))
#define SCRYPT_DERIVED_KEY_LENGTH (static_cast<size_t>(64))

#define ARGON2_DERIVED_KEY_LENGTH (static_cast<size_t>(64))
#define ARGON2_SALTSIZE (static_cast<size_t>(16))
#define ARGON2_ITERATIONS (static_cast<size_t>(2))
#define ARGON2_MEMORY_SIZE_IN_KIB (static_cast<size_t>(16384))
#define ARGON2_DEGREEOF_PARALLELISM (static_cast<size_t>(4))

#define AES_MAX_IV_LENGTH ((size_t)16)
#define AES_BLOCK_SIZE_LENGTH ((size_t)16)
#define AES_MAX_KEY_LENGTH ((size_t)32)
#define __AES_PADDING_MODE_NONE_STRING ("NoPadding"s)
#define __AES_PADDING_MODE_ZEROS_STRING ("ZeroPadding"s)
#define __AES_PADDING_MODE_ANSIX923_STRING ("X9.23"s)
#define __AES_PADDING_MODE_PKCS7_STRING ("PKCS7"s)
#define __AES_MODE_CBC_STRING ("CBC")
#define __AES_256_ALGORITHM_STRING ("AES-256")

#define HMAC_INNER_PADDING (static_cast<uint8_t>(0x36))
#define HMAC_OUTER_PADDING (static_cast<uint8_t>(0x5c))
#define HMAC_64_BYTES_BLOCK_SIZE (static_cast<size_t>(64))
#define HMAC_128_BYTES_BLOCK_SIZE (static_cast<size_t>(128))

#define RSA_XML_KEY_MAIN_NODE_STRING ("RSAKeyValue"s)
#define RSA_XML_KEY_P_NODE_STRING ("P"s)
#define RSA_XML_KEY_Q_NODE_STRING ("Q"s)
#define RSA_XML_KEY_DP_NODE_STRING ("DP"s)
#define RSA_XML_KEY_DQ_NODE_STRING ("DQ"s)
#define RSA_XML_KEY_INVERSEQ_NODE_STRING ("InverseQ"s)
#define RSA_XML_KEY_D_NODE_STRING ("D"s)
#define RSA_XML_KEY_MODULUS_NODE_STRING ("Modulus"s)
#define RSA_XML_KEY_EXPONENT_NODE_STRING ("Exponent"s)
#define __RSA_PADDING_PKCS1_V1_5_ALGORITHM_STRING ("EME-PKCS1-v1_5"s)
#define __RSA_PADDING_OAEP_ALGORITHM_STRING ("EME-OAEP(SHA-1)"s)
#define __RSA_PADDING_OAEP_256_ALGORITHM_STRING ("EME-OAEP(SHA-256)"s)
#define __RSA_PADDING_OAEP_384_ALGORITHM_STRING ("EME-OAEP(SHA-384)"s)
#define __RSA_PADDING_OAEP_512_ALGORITHM_STRING ("EME-OAEP(SHA-512)"s)
#define RSA_PEM_PRIVATE_KEY_INITIAL_STRING ("-----BEGIN PRIVATE KEY-----"s)
#define RSA_PEM_PUBLIC_KEY_INITIAL_STRING ("-----BEGIN PUBLIC KEY-----"s)
#define RSA_PEM_PRIVATE_KEY_FINAL_STRING ("-----END PRIVATE KEY-----"s)
#define RSA_PEM_PUBLIC_KEY_FINAL_STRING ("-----END PUBLIC KEY-----"s)
#define RSA_PEM_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING (R"(^(?:(-----BEGIN PUBLIC KEY-----)(?:\r|\n|\r\n)((?:(?:(?:[A-Za-z0-9+\/]{4}){16}(?:\r|\n|\r\n))+)(?:(?:[A-Za-z0-9+\/]{4}){0,15})(?:(?:[A-Za-z0-9+\/]{4}|[A-Za-z0-9+\/]{2}==|[A-Za-z0-9+\/]{3}=)))(?:\r|\n|\r\n)(-----END PUBLIC KEY-----)|(-----BEGIN PRIVATE KEY-----)(?:\r|\n|\r\n)((?:(?:(?:[A-Za-z0-9+\/]{4}){16}(?:\r|\n|\r\n))+)(?:(?:[A-Za-z0-9+\/]{4}){0,15})(?:(?:[A-Za-z0-9+\/]{4}|[A-Za-z0-9+\/]{2}==|[A-Za-z0-9+\/]{3}=)))(?:\r|\n|\r\n)(-----END PRIVATE KEY-----))$)"s)
#define RSA_XML_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING (R"((\s*<\s*RSAKeyValue\s*>\s*(?:\s*<\s*Modulus\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Modulus\s*>()|\s*<\s*Exponent\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Exponent\s*>()|\s*<\s*P\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*P\s*>()|\s*<\s*Q\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Q\s*>()|\s*<\s*DP\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*DP\s*>()|\s*<\s*DQ\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*DQ\s*>()|\s*<\s*InverseQ\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*InverseQ\s*>()|\s*<\s*D\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*D\s*>()){8}\s*<\/\s*RSAKeyValue\s*>\s*\2\3\4\5\6\7\8\9)|(\s*<\s*RSAKeyValue\s*>\s*(?:\s*<\s*Modulus\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Modulus\s*>()|\s*<\s*Exponent\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Exponent\s*>()){2}\s*<\/\s*RSAKeyValue\s*>\s*\11\12))"s)

#define HEX_ENCODER_NAME_STRING ("InsaneIO.Insane.Cryptography.HexEncoder, InsaneIO.Insane"s)
#define BASE32_ENCODER_NAME_STRING ("InsaneIO.Insane.Cryptography.Base32Encoder, InsaneIO.Insane"s)
#define BASE64_ENCODER_NAME_STRING ("InsaneIO.Insane.Cryptography.Base64Encoder, InsaneIO.Insane"s)

#define SHA_HASHER_NAME_STRING ("InsaneIO.Insane.Cryptography.ShaHasher, InsaneIO.Insane"s)
#define HMAC_HASHER_NAME_STRING ("InsaneIO.Insane.Cryptography.HmacHasher, InsaneIO.Insane"s)
#define ARGON2_HASHER_NAME_STRING ("InsaneIO.Insane.Cryptography.Argon2Hasher, InsaneIO.Insane"s)
#define SCRYPT_HASHER_NAME_STRING ("InsaneIO.Insane.Cryptography.ScryptHasher, InsaneIO.Insane"s)

#define AES_CBC_ENCRYPTOR_NAME_STRING ("InsaneIO.Insane.Cryptography.AesCbcEncryptor, InsaneIO.Insane"s)
#define RSA_ENCRYPTOR_NAME_STRING ("InsaneIO.Insane.Cryptography.RsaEncryptor, InsaneIO.Insane"s)

#define AES_CBC_PROTECTOR_NAME_STRING ("InsaneIO.Insane.Cryptography.AesCbcProtector, InsaneIO.Insane"s)

#define BASE64_VALUE_REGEX_CHAR_STRING (R"(^(?:(?:[A-Za-z0-9+\/]{4})*)(?:[A-Za-z0-9+\/]{2}==|[A-Za-z0-9+\/]{3}=)?$)"s)

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

	INSANE_ENUM(RsaKeyEncoding,
				Ber, _, _,
				Pem, _, _,
				Xml, _, _);

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
		[[nodiscard]] static StdVectorUint8 Next(size_t sz);
		[[nodiscard]] static int Next(int min, int max);
		[[nodiscard]] static int Next();
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
	class INSANE_API IEncoder : public IJsonSerialize<IEncoder>, public IClone<IEncoder>
	{
	public:
		virtual ~IEncoder() = default;
		IEncoder(const String &name);
		[[nodiscard]] virtual String Encode(const StdVectorUint8 &data) const = 0;
		[[nodiscard]] virtual String Encode(const String &data) const = 0;
		[[nodiscard]] virtual StdVectorUint8 Decode(const String &data) const = 0;
		[[nodiscard]] static std::unique_ptr<IEncoder> Deserialize(const String &json, const DeserializeResolver<IEncoder> &resolver);
		[[nodiscard]] static const std::unique_ptr<IEncoder> DefaultInstance();
		[[nodiscard]] static std::function<std::unique_ptr<IEncoder>(const String &)> DefaultDeserializeResolver();
	private:
	};

	using UniquePtrIEncoder = StdUniquePtr<IEncoder>;
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
		[[nodiscard]] std::unique_ptr<IEncoder> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IEncoder> Deserialize(const String &json, const DeserializeResolver<IEncoder> &resolver = DefaultDeserializeResolver());
		[[nodiscard]] static std::unique_ptr<IEncoder> DefaultInstance();
		[[nodiscard]] static DeserializeResolver<IEncoder> DefaultDeserializeResolver();
		[[nodiscard]] static UniquePtrIEncoder CreateInstance(const bool &toUpper = false);
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
		[[nodiscard]] std::unique_ptr<IEncoder> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IEncoder> DefaultInstance();
		[[nodiscard]] static std::unique_ptr<IEncoder> Deserialize(const String &json, const DeserializeResolver<IEncoder> &resolver = DefaultDeserializeResolver());
		[[nodiscard]] static DeserializeResolver<IEncoder> DefaultDeserializeResolver();
		[[nodiscard]] static UniquePtrIEncoder CreateInstance(const bool &removePadding = false, const bool &toLower = false);
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
		[[nodiscard]] std::unique_ptr<IEncoder> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IEncoder> Deserialize(const String &json, const DeserializeResolver<IEncoder> &resolver = DefaultDeserializeResolver());
		[[nodiscard]] static std::unique_ptr<IEncoder> DefaultInstance();
		[[nodiscard]] static DeserializeResolver<IEncoder> DefaultDeserializeResolver();
		[[nodiscard]] static UniquePtrIEncoder CreateInstance(const size_t &lineBreaksLength = NO_LINE_BREAKS, const bool &removePadding = false, const Base64Encoding &encodingType = Base64Encoding::Base64);
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
		[[nodiscard]] static StdVectorUint8 ComputeHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static StdVectorUint8 ComputeArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREEOF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
		[[nodiscard]] static StdVectorUint8 ComputeScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCKSIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);

		[[nodiscard]] static StdVectorUint8 ComputeHash(const String &data, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static StdVectorUint8 ComputeHmac(const String &data, const String &key, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static StdVectorUint8 ComputeArgon2(const String &data, const String &salt, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREEOF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
		[[nodiscard]] static StdVectorUint8 ComputeScrypt(const String &data, const String &salt, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCKSIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);

		[[nodiscard]] static String ComputeEncodedHash(const StdVectorUint8 &data, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ComputeEncodedHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, UniquePtrIEncoder &&encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ComputeEncodedArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, UniquePtrIEncoder &&encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREEOF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
		[[nodiscard]] static String ComputeEncodedScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, UniquePtrIEncoder &&encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCKSIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);

		[[nodiscard]] static String ComputeEncodedHash(const String &data, UniquePtrIEncoder &&encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ComputeEncodedHmac(const String &data, const String &key, UniquePtrIEncoder &&encoder = Base64Encoder::DefaultInstance(), const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ComputeEncodedArgon2(const String &data, const String &salt, UniquePtrIEncoder &&encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREEOF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
		[[nodiscard]] static String ComputeEncodedScrypt(const String &data, const String &salt, UniquePtrIEncoder &&encoder = Base64Encoder::DefaultInstance(), const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCKSIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);

		[[nodiscard]] static String ComputeEncodedHash(const StdVectorUint8 &data, const IEncoder *encoder, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ComputeEncodedHmac(const StdVectorUint8 &data, const StdVectorUint8 &key, const IEncoder *encoder, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ComputeEncodedArgon2(const StdVectorUint8 &data, const StdVectorUint8 &salt, const IEncoder *encoder, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREEOF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
		[[nodiscard]] static String ComputeEncodedScrypt(const StdVectorUint8 &data, const StdVectorUint8 &salt, const IEncoder *encoder, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCKSIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);

		[[nodiscard]] static String ComputeEncodedHash(const String &data, const IEncoder *encoder, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ComputeEncodedHmac(const String &data, const String &key, const IEncoder *encoder, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ComputeEncodedArgon2(const String &data, const String &salt, const IEncoder *encoder, const size_t &iterations = ARGON2_ITERATIONS, const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t &parallelism = ARGON2_DEGREEOF_PARALLELISM, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
		[[nodiscard]] static String ComputeEncodedScrypt(const String &data, const String &salt, const IEncoder *encoder, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCKSIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
	private:
	};

	// ███ AesExtensions ███
	class INSANE_API AesExtensions
	{
	public:
		[[nodiscard]] static StdVectorUint8 EncryptAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static StdVectorUint8 EncryptAesCbc(const String &data, const String &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static String EncryptEncodedAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static String EncryptEncodedAesCbc(const String &data, const String &key, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static String EncryptEncodedAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, const IEncoder *encoder, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static String EncryptEncodedAesCbc(const String &data, const String &key, const IEncoder *encoder, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);

		[[nodiscard]] static StdVectorUint8 DecryptAesCbc(const StdVectorUint8 &data, const StdVectorUint8 &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static StdVectorUint8 DecryptAesCbc(const StdVectorUint8 &data, const String &key, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static StdVectorUint8 DecryptEncodedAesCbc(const String &data, const StdVectorUint8 &key, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static StdVectorUint8 DecryptEncodedAesCbc(const String &data, const String &key, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance(), const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static StdVectorUint8 DecryptEncodedAesCbc(const String &data, const StdVectorUint8 &key, const IEncoder *encoder, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
		[[nodiscard]] static StdVectorUint8 DecryptEncodedAesCbc(const String &data, const String &key, const IEncoder *encoder, const AesCbcPadding &padding = AesCbcPadding::Pkcs7);
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
		[[nodiscard]] static RsaKeyPair CreateRsaKeyPair(const size_t &keySize = 4096, const RsaKeyEncoding &encoding = RsaKeyEncoding::Ber);
		[[nodiscard]] static StdVectorUint8 EncryptRsa(const StdVectorUint8 &data, const String &publicKey, const RsaPadding &padding = RsaPadding::OaepSha256);
		[[nodiscard]] static StdVectorUint8 EncryptRsa(const String &data, const String &publicKey, const RsaPadding &padding = RsaPadding::OaepSha256);
		[[nodiscard]] static String EncryptEncodedRsa(const StdVectorUint8 &data, const String &publicKey, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance(), const RsaPadding &padding = RsaPadding::OaepSha256);
		[[nodiscard]] static String EncryptEncodedRsa(const String &data, const String &publicKey, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance(), const RsaPadding &padding = RsaPadding::OaepSha256);
		[[nodiscard]] static String EncryptEncodedRsa(const StdVectorUint8 &data, const String &publicKey, const IEncoder *encoder, const RsaPadding &padding = RsaPadding::OaepSha256);
		[[nodiscard]] static String EncryptEncodedRsa(const String &data, const String &publicKey, const IEncoder *encoder, const RsaPadding &padding = RsaPadding::OaepSha256);

		[[nodiscard]] static StdVectorUint8 DecryptRsa(const StdVectorUint8 &data, const String &privateKey, const RsaPadding &padding = RsaPadding::OaepSha256);
		[[nodiscard]] static StdVectorUint8 DecryptEncodedRsa(const String &data, const String &privateKey, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance(), const RsaPadding &padding = RsaPadding::OaepSha256);
		[[nodiscard]] static StdVectorUint8 DecryptEncodedRsa(const String &data, const String &privateKey, const IEncoder *encoder, const RsaPadding &padding = RsaPadding::OaepSha256);
		[[nodiscard]] static RsaKeyEncoding GetRsaKeyEncoding(const String &key);
		[[nodiscard]] static bool ValidateRsaPublicKey(const String &publicKey);
		[[nodiscard]] static bool ValidateRsaPrivateKey(const String &privateKey);
	private:
	};

	// ███ IHasher ███
	class INSANE_API IHasher : public IJsonSerialize<IHasher>, IClone<IHasher>
	{
	public:
		virtual ~IHasher() = default;
		IHasher(const String &name);
		[[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) = 0;
		[[nodiscard]] virtual StdVectorUint8 Compute(const String &data) = 0;
		[[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) = 0;
		[[nodiscard]] virtual String ComputeEncoded(const String &data) = 0;
		[[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) = 0;
		[[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) = 0;
		[[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) = 0;
		[[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) = 0;
		[[nodiscard]] virtual String Serialize(const bool &indent = false) const override = 0;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String &json, const DeserializeResolver<IHasher> &resolver = DefaultDeserializeResolver());
		[[nodiscard]] static DeserializeResolver<IHasher> DefaultDeserializeResolver();

	private:
	};

	// ███ ShaHasher ███
	class INSANE_API ShaHasher : public IHasher
	{
	public:
		virtual ~ShaHasher() = default;
		ShaHasher(const HashAlgorithm &hashAlgorithm = HashAlgorithm::Sha512, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance());
		ShaHasher(const ShaHasher &instance);

		[[nodiscard]] HashAlgorithm GetHashAlgorithm() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;

		[[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) override;
		[[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) override;
		[[nodiscard]] virtual StdVectorUint8 Compute(const String &data) override;
		[[nodiscard]] virtual String ComputeEncoded(const String &data) override;
		
		[[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) override;
		[[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) override;
		[[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) override;
		[[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) override;
		
		[[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
		[[nodiscard]] std::unique_ptr<IHasher> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String &json, const std::function<std::unique_ptr<IHasher>(String)> &resolver = DefaultDeserializeResolver());
		[[nodiscard]] static DeserializeResolver<IHasher> DefaultDeserializeResolver();
	private:
		const HashAlgorithm _HashAlgorithm;
		const std::unique_ptr<IEncoder> _Encoder;
	};

	// ███ HmacHasher ███
	class INSANE_API HmacHasher : public IHasher
	{
	public:
		virtual ~HmacHasher() = default;
		HmacHasher(const StdVectorUint8 &key = RandomExtensions::Next(HMAC_KEY_SIZE), const HashAlgorithm &hashAlgorithm = HashAlgorithm::Sha512, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance());
		HmacHasher(const HmacHasher &instance);

		[[nodiscard]] HashAlgorithm GetHashAlgorithm() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;
		[[nodiscard]] StdVectorUint8 GetKey() const;
		[[nodiscard]] String GetKeyEncoded() const;

		[[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) override;
		[[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) override;
		[[nodiscard]] virtual StdVectorUint8 Compute(const String &data) override;
		[[nodiscard]] virtual String ComputeEncoded(const String &data) override;
		
		[[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) override;
		[[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) override;
		[[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) override;
		[[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) override;
		
		[[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
		[[nodiscard]] std::unique_ptr<IHasher> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String &json, const DeserializeResolver<IHasher> &resolver = DefaultDeserializeResolver());
		[[nodiscard]] static DeserializeResolver<IHasher> DefaultDeserializeResolver();
	private:
		const HashAlgorithm _HashAlgorithm;
		const std::unique_ptr<IEncoder> _Encoder;
		const StdVectorUint8 _Key;
	};

	// ███ Argon2Hasher ███
	class INSANE_API Argon2Hasher : public IHasher
	{
	public:
		virtual ~Argon2Hasher() = default;
		Argon2Hasher(const StdVectorUint8 &salt = RandomExtensions::Next(ARGON2_SALTSIZE),
					 const size_t &iterations = ARGON2_ITERATIONS,
					 const size_t &memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB,
					 const size_t &degreeOfParallelism = ARGON2_DEGREEOF_PARALLELISM,
					 const Argon2Variant argon2Variant = Argon2Variant::Argon2id,
					 const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH,
					 std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance());
		Argon2Hasher(const Argon2Hasher &instance);

		[[nodiscard]] StdVectorUint8 GetSalt() const;
		[[nodiscard]] String GetSaltEncoded() const;
		[[nodiscard]] size_t GetIterations() const;
		[[nodiscard]] size_t GetMemorySizeKiB() const;
		[[nodiscard]] size_t GetDegreeOfParallelism() const;
		[[nodiscard]] size_t GetDerivedKeyLength() const;
		[[nodiscard]] Argon2Variant GetArgon2Variant() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;

		[[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) override;
		[[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) override;
		[[nodiscard]] virtual StdVectorUint8 Compute(const String &data) override;
		[[nodiscard]] virtual String ComputeEncoded(const String &data) override;
		
		[[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) override;
		[[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) override;
		[[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) override;
		[[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) override;

		[[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
		[[nodiscard]] std::unique_ptr<IHasher> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String &json, const DeserializeResolver<IHasher> &resolver = DefaultDeserializeResolver());
		[[nodiscard]] static DeserializeResolver<IHasher> DefaultDeserializeResolver();
	private:
		const StdVectorUint8 _Salt;
		const size_t _Iterations;
		const size_t _MemorySizeKiB;
		const size_t _DegreeOfParallelism;
		const size_t _DerivedKeyLength;
		const Argon2Variant _Argon2Variant;
		const std::unique_ptr<IEncoder> _Encoder;
	};

	// ███ ScryptHasher ███
	class INSANE_API ScryptHasher : public IHasher
	{
	public:
		virtual ~ScryptHasher() = default;
		ScryptHasher(const StdVectorUint8 &salt = RandomExtensions::Next(SCRYPT_SALT_SIZE),
					 const size_t &iterations = SCRYPT_ITERATIONS,
					 const size_t &blockSize = SCRYPT_BLOCKSIZE,
					 const size_t &parallelism = SCRYPT_PARALLELISM,
					 const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH,
					 std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance());
		ScryptHasher(const ScryptHasher &instance);

		[[nodiscard]] StdVectorUint8 GetSalt() const;
		[[nodiscard]] String GetSaltEncoded() const;
		[[nodiscard]] size_t GetIterations() const;
		[[nodiscard]] size_t GetBlockSize() const;
		[[nodiscard]] size_t GetParallelism() const;
		[[nodiscard]] size_t GetDerivedKeyLength() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;

		[[nodiscard]] virtual StdVectorUint8 Compute(const StdVectorUint8 &data) override;
		[[nodiscard]] virtual String ComputeEncoded(const StdVectorUint8 &data) override;
		[[nodiscard]] virtual StdVectorUint8 Compute(const String &data) override;
		[[nodiscard]] virtual String ComputeEncoded(const String &data) override;
		
		[[nodiscard]] virtual bool Verify(const StdVectorUint8 &data, const StdVectorUint8 &expected) override;
		[[nodiscard]] virtual bool VerifyEncoded(const StdVectorUint8 &data, const String &expected) override;
		[[nodiscard]] virtual bool Verify(const String &data, const StdVectorUint8 &expected) override;
		[[nodiscard]] virtual bool VerifyEncoded(const String &data, const String &expected) override;

		[[nodiscard]] std::unique_ptr<IHasher> Clone() const override;
		[[nodiscard]] static DeserializeResolver<IHasher> DefaultDeserializeResolver();
		[[nodiscard]] virtual String Serialize(const bool &indent = false) const override;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String &json, const DeserializeResolver<IHasher> &resolver = DefaultDeserializeResolver());
	private:
		const StdVectorUint8 _Salt;
		const size_t _Iterations;
		const size_t _BlockSize;
		const size_t _Parallelism;
		const size_t _DerivedKeyLength;
		const std::unique_ptr<IEncoder> _Encoder;
	};

	// ███ ISecretProtector ███
	class INSANE_API ISecretProtector
	{
	public:
		virtual ~ISecretProtector() = default;
		ISecretProtector(const String &name);
		[[nodiscard]] String GetName();
		[[nodiscard]] virtual StdVectorUint8 Protect(const StdVectorUint8 &secret, const StdVectorUint8 &key) = 0;
		[[nodiscard]] virtual StdVectorUint8 Unprotect(const StdVectorUint8 &secret, const StdVectorUint8 &key) = 0;
		[[nodiscard]] static std::unique_ptr<ISecretProtector> DefaultInstance()
		{
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}
	private:
		const String _Name;
	};

	// ███ AesCbcProtector ███
	class INSANE_API AesCbcProtector : public ISecretProtector
	{
	public:
		virtual ~AesCbcProtector() = default;
		AesCbcProtector();
		[[nodiscard]] virtual StdVectorUint8 Protect(const StdVectorUint8 &secret, const StdVectorUint8 &key) override;
		[[nodiscard]] virtual StdVectorUint8 Unprotect(const StdVectorUint8 &secret, const StdVectorUint8 &key) override;
		[[nodiscard]] static std::unique_ptr<ISecretProtector> DefaultInstance();

	private:
	};

	// ███ ProtectorResolver ███
	using ProtectorResolver = std::function<std::unique_ptr<ISecretProtector>(const String &protectorName)>;

	// ███ SecureDeserializeResolver ███
	template <typename T>
	using SecureDeserializeResolver = std::function<std::unique_ptr<T>(const String &json, const StdVectorUint8 &serializeKey)>;

	// ███ ISecureJsonSerialize ███
	template <typename T>
	class ISecureJsonSerialize : public IBaseSerialize
	{
	public:
		virtual ~ISecureJsonSerialize() = default;
		ISecureJsonSerialize(String name) : IBaseSerialize(name) {}

		[[nodiscard]] virtual String Serialize(const StdVectorUint8 &serializeKey, const bool &indent = false, const std::unique_ptr<ISecretProtector> &protector = AesCbcProtector::DefaultInstance()) const = 0;

		[[nodiscard]] static std::unique_ptr<T> Deserialize([[maybe_unused]] const String &json, [[maybe_unused]] const StdVectorUint8 &serializeKey, [[maybe_unused]] const SecureDeserializeResolver<T> &deserializeResolver)
		{
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}

		[[nodiscard]] static SecureDeserializeResolver<T> DefaultDeserializeResolver()
		{
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}

		[[nodiscard]] static ProtectorResolver DefaultProtectorResolver()
		{
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}

	private:
	};

	// ███ IEncryptor ███
	class INSANE_API IEncryptor : public ISecureJsonSerialize<IEncryptor>, public IClone<IEncryptor>
	{
	public:
		virtual ~IEncryptor() = default;
		IEncryptor(const String &name);
		[[nodiscard]] virtual StdVectorUint8 Encrypt(const StdVectorUint8 &data) const = 0;
		[[nodiscard]] virtual String EncryptEncoded(const StdVectorUint8 &data) const = 0;
		[[nodiscard]] virtual StdVectorUint8 Encrypt(const String &data) const = 0;
		[[nodiscard]] virtual String EncryptEncoded(const String &data) const = 0;
		
		[[nodiscard]] virtual StdVectorUint8 Decrypt(const StdVectorUint8 &data) const = 0;
		[[nodiscard]] virtual StdVectorUint8 DecryptEncoded(const String &data) const = 0;
	private:
	};

	// ███ AesCbcEncryptor ███
	class INSANE_API AesCbcEncryptor : public IEncryptor
	{
	public:
		virtual ~AesCbcEncryptor() = default;
		AesCbcEncryptor(const StdVectorUint8 &key = RandomExtensions::Next(AES_MAX_KEY_LENGTH), const AesCbcPadding &padding = AesCbcPadding::Pkcs7, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance());
		AesCbcEncryptor(const AesCbcEncryptor &instance);

		[[nodiscard]] StdVectorUint8 GetKey() const;
		[[nodiscard]] String GetKeyEncoded() const;
		[[nodiscard]] AesCbcPadding GetPadding() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;

		[[nodiscard]] virtual StdVectorUint8 Encrypt(const StdVectorUint8 &data) const override;
		[[nodiscard]] virtual String EncryptEncoded(const StdVectorUint8 &data) const override;
		[[nodiscard]] virtual StdVectorUint8 Encrypt(const String &data) const override;
		[[nodiscard]] virtual String EncryptEncoded(const String &data) const override;
		
		[[nodiscard]] virtual StdVectorUint8 Decrypt(const StdVectorUint8 &data) const override;
		[[nodiscard]] virtual StdVectorUint8 DecryptEncoded(const String &data) const override;

		[[nodiscard]] std::unique_ptr<IEncryptor> Clone() const override;
		[[nodiscard]] virtual String Serialize(const StdVectorUint8 &serializeKey, const bool &indent = false, const std::unique_ptr<ISecretProtector> &protector = AesCbcProtector::DefaultInstance()) const override;
		[[nodiscard]] static ProtectorResolver DefaultProtectorResolver();
		[[nodiscard]] static SecureDeserializeResolver<IEncryptor> DefaultDeserializeResolver();
		[[nodiscard]] static std::unique_ptr<IEncryptor> Deserialize(const String &json, const StdVectorUint8 &serializeKey, const SecureDeserializeResolver<IEncryptor> &deserializeResolver = DefaultDeserializeResolver());
	private:
		const StdVectorUint8 _Key;
		const AesCbcPadding _Padding;
		const std::unique_ptr<IEncoder> _Encoder;
	};

	// ███ RsaEncryptor ███
	class INSANE_API RsaEncryptor : public IEncryptor
	{
	public:
		virtual ~RsaEncryptor() = default;
		RsaEncryptor(const RsaKeyPair &keyPair, const RsaPadding &padding = RsaPadding::OaepSha256, std::unique_ptr<IEncoder> &&encoder = Base64Encoder::DefaultInstance());
		RsaEncryptor(const RsaEncryptor &instance);

		[[nodiscard]] RsaKeyPair GetKeyPair() const;
		[[nodiscard]] RsaPadding GetPadding() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;

		[[nodiscard]] virtual StdVectorUint8 Encrypt(const StdVectorUint8 &data) const override;
		[[nodiscard]] virtual String EncryptEncoded(const StdVectorUint8 &data) const override;
		[[nodiscard]] virtual StdVectorUint8 Encrypt(const String &data) const override;
		[[nodiscard]] virtual String EncryptEncoded(const String &data) const override;
		
		[[nodiscard]] virtual StdVectorUint8 Decrypt(const StdVectorUint8 &data) const override;
		[[nodiscard]] virtual StdVectorUint8 DecryptEncoded(const String &data) const override;

		[[nodiscard]] std::unique_ptr<IEncryptor> Clone() const override;
		[[nodiscard]] virtual String Serialize(const StdVectorUint8 &serializeKey, const bool &indent = false, const std::unique_ptr<ISecretProtector> &protector = AesCbcProtector::DefaultInstance()) const override;

		[[nodiscard]] static ProtectorResolver DefaultProtectorResolver();
		[[nodiscard]] static SecureDeserializeResolver<IEncryptor> DefaultDeserializeResolver();
		[[nodiscard]] static std::unique_ptr<IEncryptor> Deserialize(const String &json, const StdVectorUint8 &serializeKey, const SecureDeserializeResolver<IEncryptor> &deserializeResolver = DefaultDeserializeResolver());
	private:
		const RsaKeyPair _KeyPair;
		const RsaPadding _Padding;
		const std::unique_ptr<IEncoder> _Encoder;
	};

	class INSANE_API CryptoTests
	{
	public:
		static void HexEncodingExtensionsTests(const bool &showValues = true);
		static void Base32EncodingExtensionsTests(const bool &showValues = true);
		static void Base64EncodingExtensionsTests(const bool &showValues = true);
	private:
	};
} // namespace InsaneIO::Insane::Crypto

#endif // !INSANE_CRYPOGRAPHY_H

/* TODO
	Converter de claves.
*/