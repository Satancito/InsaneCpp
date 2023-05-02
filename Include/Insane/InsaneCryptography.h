﻿#pragma once
#ifndef INSANE_CRYPOGRAPHY_H
#define INSANE_CRYPOGRAPHY_H

#include <Insane/Insane.h>
#include <Insane/InsaneString.h>
#include <Insane/InsaneCore.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>
#include <algorithm>
#include <functional>

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

#define HMAC_INNER_PADDING (static_cast<SignedChar>(0x36))
#define HMAC_OUTER_PADDING (static_cast<SignedChar>(0x5c))
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

namespace InsaneIO::Insane::Cryptography
{

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

	// ███
	class HexEncodingExtensions
	{
	public:
		[[nodiscard]] static String FromHex(const String& data);
		[[nodiscard]] static String ToHex(const String& data, const bool& toUpper = false);

	private:
	};

	// ███
	class Base32EncodingExtensions
	{
	public:
		[[nodiscard]] static String FromBase32(const String& data);
		[[nodiscard]] static String ToBase32(const String& data, const bool& removePadding = false, const bool& toLower = false);

	private:
	};

	// ███
	class Base64EncodingExtensions
	{
	public:
		[[nodiscard]] static String FromBase64(const String& data);
		[[nodiscard]] static String ToBase64(const String& data, const size_t& lineBreaksLength = NO_LINE_BREAKS, const bool& removePadding = false);
		[[nodiscard]] static String ToUrlSafeBase64(const String& data);
		[[nodiscard]] static String ToFilenameSafeBase64(const String& data);
		[[nodiscard]] static String ToUrlEncodedBase64(const String& data);

		[[nodiscard]] static String Base64ToUrlSafeBase64(const String& base64);
		[[nodiscard]] static String Base64ToFilenameSafeBase64(const String& base64);
		[[nodiscard]] static String Base64ToUrlEncodedBase64(const String& base64);

	private:
	};

	// ███
	class HashExtensions
	{
	public:
		[[nodiscard]] static String ToHash(const String& data, const HashAlgorithm& algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ToHmac(const String& data, const String& key, const HashAlgorithm& algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ToScrypt(const String& data, const String& salt, const size_t& iterations = SCRYPT_ITERATIONS, const size_t& blockSize = SCRYPT_BLOCKSIZE, const size_t& parallelism = SCRYPT_PARALLELISM, const size_t& derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
		[[nodiscard]] static String ToArgon2(const String& data, const String& salt, const size_t& iterations = ARGON2_ITERATIONS, const size_t& memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB, const size_t& parallelism = ARGON2_DEGREEOF_PARALLELISM, const Argon2Variant& variant = Argon2Variant::Argon2id, const size_t& derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);

	private:
	};

	// ███ IBaseSerialize ███
	class IBaseSerialize
	{
	public:
		IBaseSerialize(String name);
		[[nodiscard]] String GetName() const;
	private:
		const String _Name;
	};


	// ███ IJsonSerialize ███
	template <typename T>
	class IJsonSerialize : public IBaseSerialize
	{
	public:
		IJsonSerialize(String name) :IBaseSerialize(name) {

		}

		[[nodiscard]] virtual String Serialize(const bool& indent = false) const = 0;


		[[nodiscard]] static std::unique_ptr<T> Deserialize(const String& json, const std::function<std::unique_ptr<T>(const String&)>& resolver)
		{
			USING_NS_INSANE_EXCEPTION;
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}

		[[nodiscard]] static std::function<std::unique_ptr<T>(const String&)> DefaultDeserializeResolver()
		{
			USING_NS_INSANE_EXCEPTION;
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}

	private:
	};

	// ███
	class IEncoder : public IJsonSerialize<IEncoder>, public IClone<IEncoder>
	{
	public:
		IEncoder(String name);

		[[nodiscard]] virtual String Encode(const String& data) const = 0;
		[[nodiscard]] virtual String Decode(const String& data) const = 0;
		[[nodiscard]] static std::unique_ptr<IEncoder> Deserialize(const String& json, const std::function<std::unique_ptr<IEncoder>(const String&)>& resolver);
		[[nodiscard]] static const std::unique_ptr<IEncoder> DefaultInstance();
		[[nodiscard]] static std::function<std::unique_ptr<IEncoder>(const String&)> DefaultDeserializeResolver();
	private:
	};

	// ███
	class HexEncoder : public IEncoder {
	public:
		HexEncoder(const bool& toUpper = false);

		bool GetToUpper() const;

		[[nodiscard]] virtual String Encode(const String& data) const override;
		[[nodiscard]] virtual String Decode(const String& data) const override;
		[[nodiscard]] virtual String Serialize(const bool& indent = false) const override;
		[[nodiscard]] std::unique_ptr<IEncoder> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IEncoder> Deserialize(const String& json, const std::function<std::unique_ptr<IEncoder>(const String&)>& resolver = DefaultDeserializeResolver());
		[[nodiscard]] static std::unique_ptr<IEncoder> DefaultInstance();
		[[nodiscard]] static std::function<std::unique_ptr<IEncoder>(const String&)> DefaultDeserializeResolver();

	private:
		static const std::unique_ptr<IEncoder> _DefaultInstance;
		static const std::function<std::unique_ptr<IEncoder>(const String&)> _DefaultDeserializeResolver;
		const bool ToUpper;
	};

	// ███
	class Base32Encoder : public IEncoder
	{
	public:
		Base32Encoder(const size_t& removePadding = false, const bool& toLower = false);

		[[nodiscard]] size_t GetToLower() const;
		[[nodiscard]] bool GetRemovePadding() const;

		[[nodiscard]] virtual String Encode(const String& data) const override;
		[[nodiscard]] virtual String Decode(const String& data) const override;
		[[nodiscard]] virtual String Serialize(const bool& indent = false) const override;
		[[nodiscard]] std::unique_ptr<IEncoder> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IEncoder> DefaultInstance();
		[[nodiscard]] static std::unique_ptr<IEncoder> Deserialize(const String& json, const std::function<std::unique_ptr<IEncoder>(const String&)>& resolver = DefaultDeserializeResolver());
		[[nodiscard]] static std::function<std::unique_ptr<IEncoder>(const String&)> DefaultDeserializeResolver();
	private:
		static const std::unique_ptr<IEncoder> _DefaultInstance;
		static const std::function<std::unique_ptr<IEncoder>(const String&)> _DefaultDeserializeResolver;
		const bool RemovePadding;
		const bool ToLower;
	};

	// ███
	class Base64Encoder : public IEncoder
	{
	public:
		Base64Encoder(const size_t& lineBreaksLength = NO_LINE_BREAKS, const bool& removePadding = false, const Base64Encoding& encodingType = Base64Encoding::Base64);

		[[nodiscard]] size_t GetLineBreaksLength() const;
		[[nodiscard]] bool GetRemovePadding() const;
		[[nodiscard]] Base64Encoding GetEncodingType() const;

		[[nodiscard]] virtual String Encode(const String& data) const override;
		[[nodiscard]] virtual String Decode(const String& data) const override;
		[[nodiscard]] virtual String Serialize(const bool& indent = false) const override;
		[[nodiscard]] std::unique_ptr<IEncoder> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IEncoder> Deserialize(const String& json, const std::function<std::unique_ptr<IEncoder>(const String&)>& resolver = DefaultDeserializeResolver());
		[[nodiscard]] static std::unique_ptr<IEncoder> DefaultInstance();
		[[nodiscard]] static std::function<std::unique_ptr<IEncoder>(const String&)> DefaultDeserializeResolver();
	private:
		static const std::unique_ptr<IEncoder> _DefaultInstance;
		static const std::function<std::unique_ptr<IEncoder>(const String&)> _DefaultDeserializeResolver;
		const size_t LineBreaksLength;
		const bool RemovePadding;
		const Base64Encoding EncodingType;
	};

	// ███
	class RsaKeyPair : public IJsonSerialize<RsaKeyPair>
	{
	public:
		RsaKeyPair(const String& publicKey, const String& privateKey);
		[[nodiscard]] String GetPublicKey() const;
		[[nodiscard]] String GetPrivateKey() const;
		[[nodiscard]] virtual String Serialize(const bool& indent = false) const override;
		[[nodiscard]] static RsaKeyPair Deserialize(const String& json);
	private:
		const String PublicKey;
		const String PrivateKey;
	};

	// ███
	class AesExtensions
	{
	public:
		[[nodiscard]] static String EncryptAesCbc(const String& data, const String& key, const AesCbcPadding& padding = AesCbcPadding::Pkcs7) noexcept(false);
		[[nodiscard]] static String DecryptAesCbc(const String& data, const String& key, const AesCbcPadding& padding = AesCbcPadding::Pkcs7) noexcept(false);
	private:
		[[nodiscard]] static String GenerateNormalizedKey(const String& key);
		[[nodiscard]] static void ValidateKey(const String& key);
	};

	// ███
	class RsaExtensions
	{
	public:
		[[nodiscard]] static RsaKeyPair CreateRsaKeyPair(const size_t& keySize = 4096, const RsaKeyEncoding& encoding = RsaKeyEncoding::Ber);
		[[nodiscard]] static String EncryptRsa(const String& data, const String& publicKey, const RsaPadding& padding = RsaPadding::OaepSha256);
		[[nodiscard]] static String DecryptRsa(const String& data, const String& privateKey, const RsaPadding& padding = RsaPadding::OaepSha256);
		[[nodiscard]] static RsaKeyEncoding GetRsaKeyEncoding(const String& key);
		[[nodiscard]] static bool ValidateRsaPublicKey(const String& publicKey);
		[[nodiscard]] static bool ValidateRsaPrivateKey(const String& privateKey);
	private:
	};

	// ███
	class IHasher : public IJsonSerialize<IHasher>, IClone<IHasher>
	{
	public:
		IHasher(const String& name);
		[[nodiscard]] virtual String Compute(const String& data) = 0;
		[[nodiscard]] virtual bool Verify(const String& data, const String& expected) = 0;
		[[nodiscard]] virtual String ComputeEncoded(const String& data) = 0;
		[[nodiscard]] virtual bool VerifyEncoded(const String& data, const String& expected) = 0;
		[[nodiscard]] virtual String Serialize(const bool& indent = false) const override = 0;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver = DefaultDeserializeResolver());
		[[nodiscard]] static std::function<std::unique_ptr<IHasher>(String)> DefaultDeserializeResolver();
	private:
	};

	// ███
	class ShaHasher : public IHasher
	{
	public:
		ShaHasher(const HashAlgorithm& hashAlgorithm = HashAlgorithm::Sha512, std::unique_ptr<IEncoder>&& encoder = std::move(Base64Encoder::DefaultInstance()));
		ShaHasher(const ShaHasher& instance);

		[[nodiscard]] HashAlgorithm GetHashAlgorithm() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;

		[[nodiscard]] virtual String Compute(const String& data) override;
		[[nodiscard]] virtual bool Verify(const String& data, const String& expected) override;
		[[nodiscard]] virtual String ComputeEncoded(const String& data) override;
		[[nodiscard]] virtual bool VerifyEncoded(const String& data, const String& expected) override;
		[[nodiscard]] virtual String Serialize(const bool& indent = false) const override;
		[[nodiscard]] std::unique_ptr<IHasher> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver = DefaultDeserializeResolver());
		[[nodiscard]] static std::function<std::unique_ptr<IHasher>(String)> DefaultDeserializeResolver();
	private:
		static const std::function<std::unique_ptr<IHasher>(String)> _DefaultDeserializeResolver;
		const HashAlgorithm _HashAlgorithm;
		const std::unique_ptr<IEncoder> _Encoder;
	};

	// ███
	class RandomExtensions
	{
	public:
		~RandomExtensions() = default;
		[[nodiscard]] static String Next(size_t sz);
		[[nodiscard]] static int Next(int min, int max);
		[[nodiscard]] static int Next();

	private:
		[[nodiscard]] RandomExtensions() = default;
	};


	// ███
	class HmacHasher : public IHasher
	{
	public:
		HmacHasher(const String& key = RandomExtensions::Next(HMAC_KEY_SIZE), const HashAlgorithm& hashAlgorithm = HashAlgorithm::Sha512, std::unique_ptr<IEncoder>&& encoder = std::move(Base64Encoder::DefaultInstance()));
		HmacHasher(const HmacHasher& instance);

		[[nodiscard]] HashAlgorithm GetHashAlgorithm() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;
		[[nodiscard]] String GetKey() const;
		[[nodiscard]] String GetKeyEncoded() const;

		[[nodiscard]] virtual String Compute(const String& data) override;
		[[nodiscard]] virtual bool Verify(const String& data, const String& expected) override;
		[[nodiscard]] virtual String ComputeEncoded(const String& data) override;
		[[nodiscard]] virtual bool VerifyEncoded(const String& data, const String& expected) override;

		[[nodiscard]] virtual String Serialize(const bool& indent = false) const override;
		[[nodiscard]] std::unique_ptr<IHasher> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver = DefaultDeserializeResolver());
		[[nodiscard]] static std::function<std::unique_ptr<IHasher>(String)> DefaultDeserializeResolver();
	private:
		static const std::function<std::unique_ptr<IHasher>(String)> _DefaultDeserializeResolver;
		const HashAlgorithm _HashAlgorithm;
		const std::unique_ptr<IEncoder> _Encoder;
		const String _Key;
	};

	//TODO. Arreglar los Uint y Uint64 en Serialize y deserialize
	// ███
	class Argon2Hasher : public IHasher
	{
	public:
		Argon2Hasher(const String& salt = RandomExtensions::Next(ARGON2_SALTSIZE),
			const size_t& iterations = ARGON2_ITERATIONS,
			const size_t& memorySizeKiB = ARGON2_MEMORY_SIZE_IN_KIB,
			const size_t& degreeOfParallelism = ARGON2_DEGREEOF_PARALLELISM,
			const Argon2Variant argon2Variant = Argon2Variant::Argon2id,
			const size_t& derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH,
			std::unique_ptr<IEncoder>&& encoder = std::move(Base64Encoder::DefaultInstance()));
		Argon2Hasher(const Argon2Hasher& instance);

		[[nodiscard]] String GetSalt() const;
		[[nodiscard]] String GetSaltEncoded() const;
		[[nodiscard]] size_t GetIterations() const;
		[[nodiscard]] size_t GetMemorySizeKiB() const;
		[[nodiscard]] size_t GetDegreeOfParallelism() const;
		[[nodiscard]] size_t GetDerivedKeyLength() const;
		[[nodiscard]] Argon2Variant GetArgon2Variant() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;

		[[nodiscard]] virtual String Compute(const String& data) override;
		[[nodiscard]] virtual bool Verify(const String& data, const String& expected) override;
		[[nodiscard]] virtual String ComputeEncoded(const String& data) override;
		[[nodiscard]] virtual bool VerifyEncoded(const String& data, const String& expected) override;

		[[nodiscard]] virtual String Serialize(const bool& indent = false) const override;
		[[nodiscard]] std::unique_ptr<IHasher> Clone() const override;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver = DefaultDeserializeResolver());
		[[nodiscard]] static std::function<std::unique_ptr<IHasher>(String)> DefaultDeserializeResolver();
	private:
		const String _Salt;
		const size_t _Iterations;
		const size_t _MemorySizeKiB;
		const size_t _DegreeOfParallelism;
		const size_t _DerivedKeyLength;
		const Argon2Variant _Argon2Variant;
		const std::unique_ptr<IEncoder> _Encoder;

		static const std::function<std::unique_ptr<IHasher>(String)> _DefaultDeserializeResolver;
	};

	// ███
	class ScryptHasher : public IHasher
	{
	public:
		ScryptHasher(const String& salt = RandomExtensions::Next(SCRYPT_SALT_SIZE),
			const size_t& iterations = SCRYPT_ITERATIONS,
			const size_t& blockSize = SCRYPT_BLOCKSIZE,
			const size_t& parallelism = SCRYPT_PARALLELISM,
			const size_t& derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH,
			std::unique_ptr<IEncoder>&& encoder = std::move(Base64Encoder::DefaultInstance()));
		ScryptHasher(const ScryptHasher& instance);

		[[nodiscard]] String GetSalt() const;
		[[nodiscard]] String GetSaltEncoded() const;
		[[nodiscard]] size_t GetIterations() const;
		[[nodiscard]] size_t GetBlockSize() const;
		[[nodiscard]] size_t GetParallelism() const;
		[[nodiscard]] size_t GetDerivedKeyLength() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;

		[[nodiscard]] virtual String Compute(const String& data) override;
		[[nodiscard]] virtual bool Verify(const String& data, const String& expected) override;
		[[nodiscard]] virtual String ComputeEncoded(const String& data) override;
		[[nodiscard]] virtual bool VerifyEncoded(const String& data, const String& expected) override;

		[[nodiscard]] std::unique_ptr<IHasher> Clone() const override;
		[[nodiscard]] static std::function<std::unique_ptr<IHasher>(String)> DefaultDeserializeResolver();
		[[nodiscard]] virtual String Serialize(const bool& indent = false) const override;
		[[nodiscard]] static std::unique_ptr<IHasher> Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver = DefaultDeserializeResolver());
	private:
		const String _Salt;
		const size_t _Iterations;
		const size_t _BlockSize;
		const size_t _Parallelism;
		const size_t _DerivedKeyLength;
		const std::unique_ptr<IEncoder> _Encoder;

		static const std::function<std::unique_ptr<IHasher>(String)> _DefaultDeserializeResolver;
	};
	//TODO. Resolver el tipo de los resolvers de String -> const String&.

	// ███ ISecretProtector ███
	class ISecretProtector {
	public:
		ISecretProtector(const String& name);
		String GetName();
		virtual String Protect(const String& secret, const String& key) = 0;
		virtual String Unprotect(const String& secret, const String& key) = 0;
		static std::unique_ptr<ISecretProtector> DefaultInstance() {
			USING_NS_INSANE_EXCEPTION;
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}
	private:
		const String _Name;
	};

	// ███ AesCbcProtector ███
	class AesCbcProtector : public ISecretProtector {
	public:
		AesCbcProtector();
		virtual String Protect(const String& secret, const String& key) override;
		virtual String Unprotect(const String& secret, const String& key) override;
		static std::unique_ptr<ISecretProtector> DefaultInstance();
	private:
		static const std::unique_ptr<ISecretProtector> _DefaultInstance;
	};

	// ███ ProtectorResolver ███
	using ProtectorResolver = std::function<std::unique_ptr<ISecretProtector>(const String& protectorName)>;

	// ███ SecureDeserializeResolver ███
	template<typename T>
	using SecureDeserializeResolver = std::function<std::unique_ptr<T>(const String& json, const String& serializeKey)>;

	// ███ ISecureJsonSerialize ███
	template <typename T>
	class ISecureJsonSerialize : public IBaseSerialize
	{
	public:
		ISecureJsonSerialize(String name) :IBaseSerialize(name) {

		}

		[[nodiscard]] virtual String Serialize(const String& serializeKey, const bool& indent = false, const std::unique_ptr<ISecretProtector>& protector = AesCbcProtector::DefaultInstance()) const = 0;

		[[nodiscard]] static std::unique_ptr<T> Deserialize(const String& json, const String& serializeKey, const SecureDeserializeResolver<T>& deserializeResolver)
		{
			USING_NS_INSANE_EXCEPTION;
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}

		[[nodiscard]] static SecureDeserializeResolver<T> DefaultDeserializeResolver()
		{
			USING_NS_INSANE_EXCEPTION;
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}

		[[nodiscard]] static ProtectorResolver DefaultProtectorResolver()
		{
			USING_NS_INSANE_EXCEPTION;
			throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
		}
	private:

	};

	// ███ 
	class IEncryptor :public ISecureJsonSerialize<IEncryptor>, public IClone<IEncryptor>
	{
	public:
		IEncryptor(const String& name);
		[[nodiscard]] virtual String Encrypt(const String& data) const = 0;
		[[nodiscard]] virtual String EncryptEncoded(const String& data) const = 0;
		[[nodiscard]] virtual String Decrypt(const String& data) const = 0;
		[[nodiscard]] virtual String DecryptEncoded(const String& data) const = 0;
	private:
	};

	// ███ 
	class AesCbcEncryptor : public IEncryptor
	{
	public:
		AesCbcEncryptor(const String& key, const AesCbcPadding& padding = AesCbcPadding::Pkcs7, std::unique_ptr<IEncoder>&& encoder = std::move(Base64Encoder::DefaultInstance()));
		AesCbcEncryptor(const AesCbcEncryptor& instance);

		[[nodiscard]] String GetKey() const;
		[[nodiscard]] String GetKeyEncoded() const;
		[[nodiscard]] AesCbcPadding GetPadding() const;
		[[nodiscard]] std::unique_ptr<IEncoder> GetEncoder() const;


		[[nodiscard]] virtual String Encrypt(const String& data) const override;
		[[nodiscard]] virtual String EncryptEncoded(const String& data) const override;
		[[nodiscard]] virtual String Decrypt(const String& data) const override;
		[[nodiscard]] virtual String DecryptEncoded(const String& data) const override;

		[[nodiscard]] std::unique_ptr<IEncryptor> Clone() const override;
		[[nodiscard]] virtual String Serialize(const String& serializeKey, const bool& indent = false, const std::unique_ptr<ISecretProtector>& protector = AesCbcProtector::DefaultInstance()) const override;

		[[nodiscard]] static ProtectorResolver DefaultProtectorResolver();
		[[nodiscard]] static SecureDeserializeResolver<IEncryptor> DefaultDeserializeResolver();
		[[nodiscard]] static std::unique_ptr<IEncryptor> Deserialize(const String& json, const String& serializeKey, const SecureDeserializeResolver<IEncryptor>& deserializeResolver = DefaultDeserializeResolver());

	private:
		static const SecureDeserializeResolver<IEncryptor> _DefaultDeserializeResolver;
		static const ProtectorResolver _DefaultProtectorResolver;
		const String _Key;
		const AesCbcPadding _Padding;
		const std::unique_ptr<IEncoder> _Encoder;
	};










	class CryptoTests
	{
	public:
		static void HexEncodingExtensionsTests(const bool& showValues = true);
		static void Base32EncodingExtensionsTests(const bool& showValues = true);
		static void Base64EncodingExtensionsTests(const bool& showValues = true);

	private:
	};
} // namespace InsaneIO::Insane::Crypto

#endif // !INSANE_CRYPOGRAPHY_H

/* TODO
 Converter de claves
 */