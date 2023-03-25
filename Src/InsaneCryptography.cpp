#include <stdexcept>

#include <Insane/InsaneException.h>
#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneString.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneCore.h>
#include <Insane/InsaneTest.h>

#include <botan/cipher_mode.h>
#include <botan/auto_rng.h>
#include <botan/hex.h>
#include <botan/base32.h>
#include <botan/base64.h>
#include <botan/pkcs8.h>
#include <botan/rsa.h>
#include <botan/x509_key.h>
#include <botan/data_src.h>
#include <botan/bigint.h>
#include <botan/pubkey.h>
#include <botan/hash.h>
#include <botan/exceptn.h>
#include <botan/scrypt.h>
#include <botan/argon2.h>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "InsaneCryptography.h"

#define AES_MAX_IV_LENGHT ((size_t)16)
#define AES_MAX_KEY_LENGTH ((size_t)32)

#define MD5_DIGEST_LENGTH 16
#define SHA1_DIGEST_LENGTH 20
#define SHA256_DIGEST_LENGTH 32
#define SHA384_DIGEST_LENGTH 48
#define SHA512_DIGEST_LENGTH 64

#define HMAC_INNER_PADDING ((SignedChar)0x36)
#define HMAC_OUTER_PADDING ((SignedChar)0x5c)
#define HMAC_64_BYTES_BLOCK_SIZE ((size_t)64)
#define HMAC_128_BYTES_BLOCK_SIZE ((size_t)128)

#define RSA_KEY_MAIN_NODE_STRING "RSAKeyValue"s
#define RSA_KEY_P_NODE_STRING "P"s
#define RSA_KEY_Q_NODE_STRING "Q"s
#define RSA_KEY_DP_NODE_STRING "DP"s
#define RSA_KEY_DQ_NODE_STRING "DQ"s
#define RSA_KEY_INVERSEQ_NODE_STRING "InverseQ"s
#define RSA_KEY_D_NODE_STRING "D"s
#define RSA_KEY_MODULUS_NODE_STRING "Modulus"s
#define RSA_KEY_EXPONENT_NODE_STRING "Exponent"s
#define RSA_PADDING_ALGORITHM_STRING "EME-PKCS1-v1_5"s

#define XML_RSA_PUBLIC_AND_PRIVATE_KEY_INITIAL_STRING "<RSAKeyValue>"s
#define XML_RSA_PUBLIC_AND_PRIVATE_KEY_FINAL_STRING "</RSAKeyValue>"s
#define JSON_RSA_PUBLIC_AND_PRIVATE_KEY_INITIAL_STRING "{"
#define JSON_RSA_PUBLIC_AND_PRIVATE_KEY_FINAL_STRING "}"
#define PEM_RSA_PRIVATE_KEY_INITIAL_STRING "-----BEGIN PRIVATE KEY-----"
#define PEM_RSA_PUBLIC_KEY_INITIAL_STRING "-----BEGIN PUBLIC KEY-----"
#define PEM_RSA_PRIVATE_KEY_FINAL_STRING "-----END PRIVATE KEY-----"
#define PEM_RSA_PUBLIC_KEY_FINAL_STRING "-----END PUBLIC KEY-----"

#define JSON_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING u8R"((\s*\{(?:\s*"Modulus"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"Exponent"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"P"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"Q"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"DP"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"DQ"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"InverseQ"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"D"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()){8}\s*\2\3\4\5\6\7\8\9\}\s*)|(\s*\{(?:\s*"Modulus"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"Exponent"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()){2}\s*\11\12\}\s*))"																																																 // https://regex101.com/r/v5lUWw/4 //Add final Comma.
#define PEM_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING u8R"((-----BEGIN PUBLIC KEY-----(\n|\r|\r\n)([0-9a-zA-Z\+\/=]{64}(\n|\r|\r\n))*([0-9a-zA-Z\+\/=]{1,63}(\n|\r|\r\n))?-----END PUBLIC KEY-----)|(-----BEGIN PRIVATE KEY-----(\n|\r|\r\n)([0-9a-zA-Z\+\/=]{64}(\n|\r|\r\n))*([0-9a-zA-Z\+\/=]{1,63}(\n|\r|\r\n))?-----END PRIVATE KEY-----))"																																																																																																																								 // https://regex101.com/r/mGnr7I/1
#define XML_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING u8R"((\s*<\s*RSAKeyValue\s*>\s*(?:\s*<\s*Modulus\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Modulus\s*>()|\s*<\s*Exponent\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Exponent\s*>()|\s*<\s*P\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*P\s*>()|\s*<\s*Q\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Q\s*>()|\s*<\s*DP\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*DP\s*>()|\s*<\s*DQ\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*DQ\s*>()|\s*<\s*InverseQ\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*InverseQ\s*>()|\s*<\s*D\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*D\s*>()){8}\s*<\/\s*RSAKeyValue\s*>\s*\2\3\4\5\6\7\8\9)|(\s*<\s*RSAKeyValue\s*>\s*(?:\s*<\s*Modulus\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Modulus\s*>()|\s*<\s*Exponent\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Exponent\s*>()){2}\s*<\/\s*RSAKeyValue\s*>\s*\11\12))" // https://regex101.com/r/fQV2VN/4

// // ███ HmacResult ███
// Insane::Cryptography::HmacResult::HmacResult(const String &hash, const String &key, const HashAlgorithm algorithm)
// {
// 	this->hash = hash;
// 	this->key = key;
// 	this->algorithm = algorithm;
// }

// String Insane::Cryptography::HmacResult::Hash() const
// {
// 	return this->hash;
// }

// String Insane::Cryptography::HmacResult::Key() const
// {
// 	return this->key;
// }

// Insane::Cryptography::HashAlgorithm Insane::Cryptography::HmacResult::Algorithm() const
// {
// 	return this->algorithm;
// }

// String Insane::Cryptography::HmacResult::RawHash() const
// {
// 	return HashManager::FromBase64(this->hash);
// }

// String Insane::Cryptography::HmacResult::RawKey() const
// {
// 	return HashManager::FromBase64(this->key);
// }

// String Insane::Cryptography::HmacResult::Serialize() const
// {
// 	USING_NS_INSANE_STR;
// 	try
// 	{
// 		rapidjson::StringBuffer sb;
// 		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
// 		writer.StartObject();
// 		writer.String(cnameof(Hash));
// 		writer.String(hash.c_str(), static_cast<rapidjson::SizeType>(hash.length()));
// 		writer.String(cnameof(Key));
// 		writer.String(key.c_str(), static_cast<rapidjson::SizeType>(key.length()));
// 		writer.String(cnameof(Algorithm));
// 		writer.Int(static_cast<int>(algorithm));
// 		writer.EndObject();
// 		return String(sb.GetString(), sb.GetSize());
// 	}
// 	catch (...)
// 	{
// 		throw Insane::Exception::CryptoException(StringExtensions::Replace(R"(Unable to serialize "#".)", "#", nameof(HmacResult)));
// 	}
// }

// Insane::Cryptography::HmacResult Insane::Cryptography::HmacResult::Deserialize(const String &json)
// {

// 	USING_NS_INSANE_STR;
// 	try
// 	{
// 		rapidjson::Document document;
// 		document.Parse(json.c_str(), json.length());
// 		if (document.HasParseError())
// 		{
// 			throw 1;
// 		}
// 		return HmacResult(document[cnameof(Hash)].GetString(),
// 						  document[cnameof(Key)].GetString(),
// 						  static_cast<HashAlgorithm>(document[cnameof(Algorithm)].GetInt()));
// 	}
// 	catch (...)
// 	{
// 		throw Insane::Exception::CryptoException(StringExtensions::Replace(R"(Unable to deserialize "#".)", "#", nameof(HmacResult)));
// 	}
// }

// // ███ ScryptResult ███

// Insane::Cryptography::ScryptResult::ScryptResult(const String &hash, const String &salt, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
// {
// 	this->hash = hash;
// 	this->salt = salt;
// 	this->iterations = iterations;
// 	this->blockSize = blockSize;
// 	this->parallelism = parallelism;
// 	this->derivedKeyLength = derivedKeyLength;
// }

// String Insane::Cryptography::ScryptResult::Hash() const
// {
// 	return hash;
// }

// String Insane::Cryptography::ScryptResult::Salt() const
// {
// 	return salt;
// }

// size_t Insane::Cryptography::ScryptResult::Iterations() const
// {
// 	return iterations;
// }

// size_t Insane::Cryptography::ScryptResult::BlockSize() const
// {
// 	return blockSize;
// }

// size_t Insane::Cryptography::ScryptResult::Parallelism() const
// {
// 	return parallelism;
// }

// size_t Insane::Cryptography::ScryptResult::DerivedKeyLength() const
// {
// 	return derivedKeyLength;
// }

// String Insane::Cryptography::ScryptResult::RawHash() const
// {
// 	return HashManager::FromBase64(hash);
// }

// String Insane::Cryptography::ScryptResult::RawSalt() const
// {
// 	return HashManager::FromBase64(salt);
// }

// String Insane::Cryptography::ScryptResult::Serialize() const
// {
// 	USING_NS_INSANE_STR;
// 	try
// 	{
// 		rapidjson::StringBuffer sb;
// 		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
// 		writer.StartObject();
// 		writer.String(cnameof(Hash));
// 		writer.String(hash.c_str(), static_cast<rapidjson::SizeType>(hash.length()));

// 		writer.String(cnameof(Salt));
// 		writer.String(salt.c_str(), static_cast<rapidjson::SizeType>(salt.length()));

// 		writer.String(cnameof(Iterations));
// 		String number = tostr(iterations);
// 		writer.RawNumber(number.c_str(), static_cast<rapidjson::SizeType>(number.length()));

// 		writer.String(cnameof(BlockSize));
// 		number = tostr(blockSize);
// 		writer.RawNumber(number.c_str(), static_cast<rapidjson::SizeType>(number.length()));

// 		writer.String(cnameof(Parallelism));
// 		number = tostr(parallelism);
// 		writer.RawNumber(number.c_str(), static_cast<rapidjson::SizeType>(number.length()));

// 		writer.String(cnameof(DerivedKeyLength));
// 		number = tostr(derivedKeyLength);
// 		writer.RawNumber(number.c_str(), static_cast<rapidjson::SizeType>(number.length()));
// 		writer.EndObject();
// 		return String(sb.GetString(), sb.GetSize());
// 	}
// 	catch (...)
// 	{
// 		throw Insane::Exception::CryptoException(StringExtensions::Replace(R"(Unable to serialize "#".)", "#", nameof(ScryptResult)));
// 	}
// }

// Insane::Cryptography::ScryptResult Insane::Cryptography::ScryptResult::Deserialize(const String &json)
// {
// 	USING_NS_INSANE_STR;
// 	try
// 	{
// 		rapidjson::Document document;
// 		document.Parse(json.c_str(), json.length());
// 		if (document.HasParseError())
// 		{
// 			throw 1;
// 		}
// 		return ScryptResult(document[cnameof(Hash)].GetString(),
// 							document[cnameof(Salt)].GetString(),
// 							static_cast<size_t>(sizeof(size_t) == sizeof(uint64_t) ? document[cnameof(Iterations)].GetUint64() : document[cnameof(Iterations)].GetUint()),
// 							static_cast<size_t>(sizeof(size_t) == sizeof(uint64_t) ? document[cnameof(BlockSize)].GetUint64() : document[cnameof(BlockSize)].GetUint()),
// 							static_cast<size_t>(sizeof(size_t) == sizeof(uint64_t) ? document[cnameof(Parallelism)].GetUint64() : document[cnameof(Parallelism)].GetUint()),
// 							static_cast<size_t>(sizeof(size_t) == sizeof(uint64_t) ? document[cnameof(DerivedKeyLength)].GetUint64() : document[cnameof(DerivedKeyLength)].GetUint()));
// 	}
// 	catch (...)
// 	{
// 		throw Insane::Exception::CryptoException(StringExtensions::Replace(R"(Unable to deserialize "#".)", "#", nameof(ScryptResult)));
// 	}
// }

// // ███ Argon2Result ███
// Insane::Cryptography::Argon2Result::Argon2Result(const String &hash, const String &salt, const Argon2Variant &variant, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const size_t &derivedKeyLength)
// {
// 	this->hash = hash;
// 	this->salt = salt;
// 	this->variant = variant;
// 	this->iterations = iterations;
// 	this->memorySizeKiB = memorySizeKiB;
// 	this->parallelism = parallelism;
// 	this->derivedKeyLength = derivedKeyLength;
// }

// String Insane::Cryptography::Argon2Result::Hash() const
// {
// 	return hash;
// }

// String Insane::Cryptography::Argon2Result::Salt() const
// {
// 	return salt;
// }

// Insane::Cryptography::Argon2Variant Insane::Cryptography::Argon2Result::Variant() const
// {
// 	return variant;
// }

// size_t Insane::Cryptography::Argon2Result::Iterations() const
// {
// 	return iterations;
// }

// size_t Insane::Cryptography::Argon2Result::MemorySizeKiB() const
// {
// 	return memorySizeKiB;
// }

// size_t Insane::Cryptography::Argon2Result::Parallelism() const
// {
// 	return parallelism;
// }

// size_t Insane::Cryptography::Argon2Result::DerivedKeyLength() const
// {
// 	return derivedKeyLength;
// }

// String Insane::Cryptography::Argon2Result::RawHash() const
// {
// 	return HashManager::FromBase64(hash);
// }

// String Insane::Cryptography::Argon2Result::RawSalt() const
// {
// 	return HashManager::FromBase64(salt);
// }

// String Insane::Cryptography::Argon2Result::Serialize() const
// {
// 	USING_NS_INSANE_STR;
// 	try
// 	{
// 		rapidjson::StringBuffer sb;
// 		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
// 		writer.StartObject();
// 		writer.String(cnameof(Hash));
// 		writer.String(hash.c_str(), static_cast<rapidjson::SizeType>(hash.length()));

// 		writer.String(cnameof(Salt));
// 		writer.String(salt.c_str(), static_cast<rapidjson::SizeType>(salt.length()));

// 		writer.String(cnameof(Variant));
// 		writer.Int(static_cast<int>(variant));

// 		writer.String(cnameof(Iterations));
// 		String number = tostr(iterations);
// 		writer.RawNumber(number.c_str(), static_cast<rapidjson::SizeType>(number.length()));

// 		writer.String(cnameof(MemorySizeKiB));
// 		number = tostr(memorySizeKiB);
// 		writer.RawNumber(number.c_str(), static_cast<rapidjson::SizeType>(number.length()));

// 		writer.String(cnameof(Parallelism));
// 		number = tostr(parallelism);
// 		writer.RawNumber(number.c_str(), static_cast<rapidjson::SizeType>(number.length()));

// 		writer.String(cnameof(DerivedKeyLength));
// 		number = tostr(derivedKeyLength);
// 		writer.RawNumber(number.c_str(), static_cast<rapidjson::SizeType>(number.length()));
// 		writer.EndObject();
// 		return String(sb.GetString(), sb.GetSize());
// 	}
// 	catch (...)
// 	{
// 		throw Insane::Exception::CryptoException(StringExtensions::Replace(R"(Unable to serialize "#".)", "#", nameof(Argon2Result)));
// 	}
// }

// Insane::Cryptography::Argon2Result Insane::Cryptography::Argon2Result::Deserialize(const String &json)
// {
// 	USING_NS_INSANE_STR;
// 	try
// 	{
// 		rapidjson::Document document;
// 		document.Parse(json.c_str(), json.length());
// 		if (document.HasParseError())
// 		{
// 			throw 1;
// 		}
// 		return Argon2Result(document[cnameof(Hash)].GetString(),
// 							document[cnameof(Salt)].GetString(),
// 							static_cast<Argon2Variant>(document[cnameof(Variant)].GetInt()),
// 							static_cast<size_t>(sizeof(size_t) == sizeof(uint64_t) ? document[cnameof(Iterations)].GetUint64() : document[cnameof(Iterations)].GetUint()),
// 							static_cast<size_t>(sizeof(size_t) == sizeof(uint64_t) ? document[cnameof(MemorySizeKiB)].GetUint64() : document[cnameof(MemorySizeKiB)].GetUint()),
// 							static_cast<size_t>(sizeof(size_t) == sizeof(uint64_t) ? document[cnameof(Parallelism)].GetUint64() : document[cnameof(Parallelism)].GetUint()),
// 							static_cast<size_t>(sizeof(size_t) == sizeof(uint64_t) ? document[cnameof(DerivedKeyLength)].GetUint64() : document[cnameof(DerivedKeyLength)].GetUint()));
// 	}
// 	catch (...)
// 	{
// 		throw Insane::Exception::CryptoException(StringExtensions::Replace(R"(Unable to deserialize "#".)", "#", nameof(Argon2Result)));
// 	}
// }

// ███ RandomManager ███

String Insane::Cryptography::RandomManager::Next(size_t sz)
{
	String result = String(sz, 0);
	std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
	Botan::secure_vector<uint8_t> bytes = (*rng).random_vec(sz);
	return String(bytes.begin(), bytes.end());
}

int Insane::Cryptography::RandomManager::Next(int min, int max)
{
	std::random_device device = std::random_device{};
	std::mt19937 mersenneEngine(device());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(mersenneEngine);
}

int Insane::Cryptography::RandomManager::Next()
{
	return Next(INT_MIN, INT_MAX);
}

// ███ HashManager ███

// String Insane::Cryptography::HashManager::ToRawHash(const String &data, const HashAlgorithm &algorithm)
// {
// 	USING_NS_INSANE_EXCEPTION;
// 	switch (algorithm)
// 	{
// 	case HashAlgorithm::Md5:
// 	{
// 		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("MD5"s));
// 		hash->update(data);
// 		Botan::SecureVector<uint8_t> result = hash->final();
// 		return String(result.begin(), result.end());
// 	}
// 	case HashAlgorithm::Sha1:
// 	{
// 		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-1"s));
// 		hash->update(data);
// 		Botan::SecureVector<uint8_t> result = hash->final();
// 		return String(result.begin(), result.end());
// 	}
// 	case HashAlgorithm::Sha256:
// 	{
// 		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-256"s));
// 		hash->update(data);
// 		Botan::SecureVector<uint8_t> result = hash->final();
// 		return String(result.begin(), result.end());
// 	}
// 	case HashAlgorithm::Sha384:
// 	{
// 		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-384"s));
// 		hash->update(data);
// 		Botan::SecureVector<uint8_t> result = hash->final();
// 		return String(result.begin(), result.end());
// 	}
// 	case HashAlgorithm::Sha512:
// 	{
// 		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-512"s));
// 		hash->update(data);
// 		Botan::SecureVector<uint8_t> result = hash->final();
// 		return String(result.begin(), result.end());
// 	}
// 	default:
// 		throw CryptoException("Not implemented algorithm."s);
// 	}
// }

// String Insane::Cryptography::HashManager::ToBase64Hash(const String &data, const HashAlgorithm &algorithm)
// {
// 	return Base64EncodingExtensions::ToBase64(ToRawHash(data, algorithm));
// }

// String Insane::Cryptography::HashManager::ToRawHmac(const String &data, const String &key, const HashAlgorithm &algorithm)
// {
// 	size_t blockSize = 0;
// 	String secret = key;
// 	switch (algorithm)
// 	{
// 	case HashAlgorithm::Md5:
// 		[[fallthrough]];
// 	case HashAlgorithm::Sha1:
// 		[[fallthrough]];
// 	case HashAlgorithm::Sha256:
// 		blockSize = HMAC_64_BYTES_BLOCK_SIZE;
// 		break;
// 	case HashAlgorithm::Sha384:
// 		[[fallthrough]];
// 	case HashAlgorithm::Sha512:
// 		blockSize = HMAC_128_BYTES_BLOCK_SIZE;
// 		break;
// 	}

// 	if (secret.length() > blockSize)
// 	{
// 		secret = HashManager::ToRawHash(secret, algorithm);
// 	}

// 	if (secret.length() < blockSize)
// 	{
// 		secret.append(String((size_t)(blockSize - secret.length()), static_cast<char>(0)));
// 	}
// 	String outerKeyPadding = String(blockSize, HMAC_OUTER_PADDING);
// 	String innerKeyPadding = String(blockSize, HMAC_INNER_PADDING);
// 	for (size_t i = 0; i < blockSize; i++)
// 	{
// 		innerKeyPadding[i] = (SignedChar)(secret[i] ^ innerKeyPadding[i]);
// 		outerKeyPadding[i] = (SignedChar)(secret[i] ^ outerKeyPadding[i]);
// 	}
// 	innerKeyPadding.append(data);
// 	String ret = HashManager::ToRawHash(innerKeyPadding, algorithm);
// 	outerKeyPadding.append(ret);
// 	ret = HashManager::ToRawHash(outerKeyPadding, algorithm);
// 	return ret;
// }

// Insane::Cryptography::HmacResult Insane::Cryptography::HashManager::ToBase64Hmac(const String &data, const String &key, const HashAlgorithm &algorithm)
// {
// 	return HmacResult(Base64EncodingExtensions::ToBase64(ToRawHmac(data, key, algorithm)), Base64EncodingExtensions::ToBase64(key), algorithm);
// }

// Insane::Cryptography::HmacResult Insane::Cryptography::HashManager::ToBase64Hmac(const String &data, const size_t &keySize, const HashAlgorithm &algorithm)
// {
// 	return ToBase64Hmac(data, RandomManager::Next(keySize), algorithm);
// }

// String Insane::Cryptography::HashManager::ToRawScrypt(const String &data, const String &salt, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
// {
// 	USING_NS_INSANE_EXCEPTION;
// 	try
// 	{
// 		Botan::Scrypt engine = Botan::Scrypt(iterations, blockSize, parallelism);
// 		std::vector<uint8_t> out(derivedKeyLength);
// 		engine.derive_key(out.data(), out.size(), data.data(), data.size(), reinterpret_cast<const uint8_t *>(salt.data()), salt.size());
// 		return String(out.begin(), out.end());
// 	}
// 	catch (std::exception &ex)
// 	{
// 		throw CryptoException("Error when generating the derived key. "s + String(ex.what()));
// 	}
// }

// Insane::Cryptography::ScryptResult Insane::Cryptography::HashManager::ToBase64Scrypt(const String &data, const String &salt, const bool &isBase64Salt, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
// {
// 	return ScryptResult(Base64EncodingExtensions::ToBase64(ToRawScrypt(data, isBase64Salt ? Base64EncodingExtensions::FromBase64(salt) : salt, iterations, blockSize, parallelism, derivedKeyLength)),
// 						isBase64Salt ? salt : Base64EncodingExtensions::ToBase64(salt),
// 						iterations,
// 						blockSize,
// 						parallelism,
// 						derivedKeyLength);
// }

// Insane::Cryptography::ScryptResult Insane::Cryptography::HashManager::ToBase64Scrypt(const String &data, const size_t &saltSize, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength)
// {
// 	String salt = RandomManager::Next(saltSize);
// 	return ToBase64Scrypt(data, salt, false, iterations, blockSize, parallelism, derivedKeyLength);
// }

// String Insane::Cryptography::HashManager::ToRawArgon2(const String &data, const String &salt, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
// {
// 	USING_NS_INSANE_EXCEPTION;
// 	try
// 	{
// 		Botan::Argon2 engine(static_cast<uint8_t>(variant), memorySizeKiB, iterations, parallelism);
// 		std::vector<uint8_t> out(derivedKeyLength);
// 		engine.derive_key(out.data(), out.size(), data.data(), data.size(), reinterpret_cast<const uint8_t *>(salt.data()), salt.size());
// 		return String(out.begin(), out.end());
// 	}
// 	catch (std::exception &ex)
// 	{
// 		throw CryptoException("Error when generating the derived key. "s + String(ex.what()));
// 	}
// }

// Insane::Cryptography::Argon2Result Insane::Cryptography::HashManager::ToBase64Argon2(const String &data, const String &salt, const bool &isBase64Salt, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant, const size_t &derivedKeyLength)
// {
// 	return Argon2Result(
// 		Base64EncodingExtensions::ToBase64(ToRawArgon2(data, isBase64Salt ? Base64EncodingExtensions::FromBase64(salt) : salt, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength)),
// 		isBase64Salt ? salt : Base64EncodingExtensions::ToBase64(salt),
// 		variant,
// 		iterations,
// 		memorySizeKiB,
// 		parallelism,
// 		derivedKeyLength);
// }

// Insane::Cryptography::Argon2Result Insane::Cryptography::HashManager::ToBase64Argon2(const String &data, size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const size_t saltSize, const Argon2Variant &variant, const size_t &derivedKeyLength)
// {
// 	String salt = RandomManager::Next(saltSize);
// 	return ToBase64Argon2(data, salt, false, iterations, memorySizeKiB, parallelism, variant, derivedKeyLength);
// }

// ███ AesManager ███

String Insane::Cryptography::AesManager::EncryptRaw(const String &data, const String &key) noexcept(false)
{
	USING_NS_INSANE_EXCEPTION;
	try
	{
		String secretKey = GenerateValidKey(key);
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		std::unique_ptr<Botan::Cipher_Mode> enc = Botan::Cipher_Mode::create("AES-256/CBC/PKCS7"s, Botan::ENCRYPTION);
		enc->set_key(Botan::secure_vector<uint8_t>(secretKey.begin(), secretKey.end()));
		Botan::secure_vector<uint8_t> ivBytes = rng->random_vec(AES_MAX_IV_LENGHT);
		Botan::secure_vector<uint8_t> dataBytes(data.begin(), data.end());
		enc->start(ivBytes);
		enc->finish(dataBytes);
		String result = String(dataBytes.begin(), dataBytes.end());
		result += String(ivBytes.begin(), ivBytes.end());
		return result;
	}
	catch (const Botan::Exception &e)
	{
		throw CryptoException(e.what(), e.error_code());
	}
	catch (...)
	{
		throw CryptoException();
	}
}

String Insane::Cryptography::AesManager::DecryptRaw(const String &data, const String &key)
{
	try
	{
		String secretKey = GenerateValidKey(key);
		std::unique_ptr<Botan::Cipher_Mode> dec = Botan::Cipher_Mode::create("AES-256/CBC/PKCS7"s, Botan::DECRYPTION);
		dec->set_key(Botan::SecureVector<uint8_t>(secretKey.begin(), secretKey.end()));
		Botan::secure_vector<uint8_t> dataBytes(data.begin(), data.end() - AES_MAX_IV_LENGHT);
		dec->start(Botan::secure_vector<uint8_t>(data.end() - AES_MAX_IV_LENGHT, data.end()));
		dec->finish(dataBytes);
		return String(dataBytes.begin(), dataBytes.end());
	}
	catch (const Botan::Exception &e)
	{
		throw Insane::Exception::CryptoException(e.what(), e.error_code());
	}
	catch (...)
	{
		throw Insane::Exception::CryptoException();
	}
}

String Insane::Cryptography::AesManager::EncryptToBase64(const String &data, const String &key) noexcept(false)
{
	return Base64EncodingExtensions::ToBase64(EncryptRaw(data, key));
}

String Insane::Cryptography::AesManager::DecryptFromBase64(const String &data, const String &key) noexcept(false)
{
	return DecryptRaw(Base64EncodingExtensions::FromBase64(data), key);
}

String Insane::Cryptography::AesManager::GenerateValidKey(const String &key)
{
	// String hash = HashManager::ToRawHash(key, HashAlgorithm::Sha512);
	// hash.resize(AES_MAX_KEY_LENGTH);
	// return hash;
}

// ███ RsaKeyPair ███

Insane::Cryptography::RsaKeyPair::RsaKeyPair(const String &publicKey, const String &privateKey)
{
	this->privateKey = privateKey;
	this->publicKey = publicKey;
}

String Insane::Cryptography::RsaKeyPair::PublicKey() const
{
	return publicKey;
}

String Insane::Cryptography::RsaKeyPair::PrivateKey() const
{
	return privateKey;
}

void Insane::Cryptography::RsaKeyPair::SetPublicKey(const String &npublicKey)
{
	this->publicKey = npublicKey;
}

void Insane::Cryptography::RsaKeyPair::SetPrivateKey(const String &nprivateKey)
{
	this->privateKey = nprivateKey;
}

String Insane::Cryptography::RsaKeyPair::Serialize() const noexcept(false)
{
	try
	{
		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
		writer.StartObject();
		writer.String(CNAMEOF(PublicKey));
		writer.String(publicKey.c_str(), static_cast<rapidjson::SizeType>(publicKey.length()));
		writer.String(CNAMEOF(PrivateKey));
		writer.String(privateKey.c_str(), static_cast<rapidjson::SizeType>(privateKey.length()));
		writer.EndObject();
		return String(sb.GetString(), sb.GetSize());
	}
	catch (...)
	{
		throw Insane::Exception::CryptoException("Unable to serialize keypair."s);
	}
}

Insane::Cryptography::RsaKeyPair Insane::Cryptography::RsaKeyPair::Deserialize(String json) noexcept(false)
{
	USING_NS_INSANE_STR;
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError())
		{
			throw 1;
		}
		return RsaKeyPair(document["PublicKey"].GetString(), document["PrivateKey"].GetString());
	}
	catch (...)
	{
		throw Insane::Exception::CryptoException(StringExtensions::Replace(R"(Unable to deserialize "#".)", "#", NAMEOF(RsaKeyPair)));
	}
}

// ███ RsaManager ███

Insane::Cryptography::RsaKeyPair Insane::Cryptography::RsaManager::CreateKeyPair(const Size &keySize, const RsaKeyEncoding &encoding, const bool &indent)
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
	std::unique_ptr<Botan::RSA_PrivateKey> keyPair = std::make_unique<Botan::RSA_PrivateKey>(*rng, keySize);
	switch (encoding)
	{
	case RsaKeyEncoding::Ber:
	{
		try
		{
			String privateKey = Botan::base64_encode(Botan::PKCS8::BER_encode(*keyPair));
			String publicKey = Botan::base64_encode(Botan::X509::BER_encode(*keyPair));
			return RsaKeyPair(StringExtensions::Trim(publicKey), StringExtensions::Trim(privateKey));
		}
		catch (...)
		{
			throw Insane::Exception::CryptoException("Unable to generate BER keypair."s);
		}
	}
	case RsaKeyEncoding::Pem:
	{
		try
		{
			String privateKey = Botan::PKCS8::PEM_encode(*keyPair);
			String publicKey = Botan::X509::PEM_encode(*keyPair);
			return RsaKeyPair(StringExtensions::Trim(publicKey), StringExtensions::Trim(privateKey));
		}
		catch (...)
		{
			throw Insane::Exception::CryptoException("Unable to generate PEM keypair."s);
		}
	}
	case RsaKeyEncoding::Xml:
	{
		try
		{

			std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
			String rsaValueName = RSA_KEY_MAIN_NODE_STRING;
			rapidxml::xml_node<> *mainNode = doc->allocate_node(rapidxml::node_type::node_element, rsaValueName.c_str());
			doc->append_node(mainNode);

			String modulus = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_n()));
			String modulusName = RSA_KEY_MODULUS_NODE_STRING;
			rapidxml::xml_node<> *childNode = doc->allocate_node(rapidxml::node_type::node_element, modulusName.c_str(), modulus.c_str(), 0, modulus.length());
			mainNode->append_node(childNode);

			String exponent = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_e()));
			String exponentName = RSA_KEY_EXPONENT_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, exponentName.c_str(), exponent.c_str(), 0, exponent.length());
			mainNode->append_node(childNode);

			String publicKey;
			rapidxml::print(std::back_inserter(publicKey), *doc, indent ? 0 : rapidxml::print_no_indenting);

			String p = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_p()));
			String pName = RSA_KEY_P_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, pName.c_str(), p.c_str(), 0, p.length());
			mainNode->append_node(childNode);

			String q = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_q()));
			String qName = RSA_KEY_Q_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, qName.c_str(), q.c_str(), 0, q.length());
			mainNode->append_node(childNode);

			String dp = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d1()));
			String dpName = RSA_KEY_DP_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, dpName.c_str(), dp.c_str(), 0, dp.length());
			mainNode->append_node(childNode);

			String dq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d2()));
			String dqName = RSA_KEY_DQ_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, dqName.c_str(), dq.c_str(), 0, dq.length());
			mainNode->append_node(childNode);

			String inverseq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_c()));
			String inverseqName = RSA_KEY_INVERSEQ_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, inverseqName.c_str(), inverseq.c_str(), 0, inverseq.length());
			mainNode->append_node(childNode);

			String d = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d()));
			String dName = RSA_KEY_D_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, dName.c_str(), d.c_str(), 0, d.length());
			mainNode->append_node(childNode);
			String privateKey;
			rapidxml::print(std::back_inserter(privateKey), *doc, indent ? 0 : rapidxml::print_no_indenting);
			doc->clear();
			return RsaKeyPair(StringExtensions::Trim(publicKey), StringExtensions::Trim(privateKey));
		}
		catch (...)
		{
			throw Insane::Exception::CryptoException("Unable to serialize xml keypair."s);
		}
	}
	case RsaKeyEncoding::Json:
	{
		try
		{
			String modulus = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_n()));
			String exponent = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_e()));
			String p = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_p()));
			String q = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_q()));
			String dp = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d1()));
			String dq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d2()));
			String inverseq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_c()));
			String d = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d()));

			rapidjson::StringBuffer sb;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
			if (indent)
			{
				writer.SetIndent(SPACE_CHAR, 2);
			}
			else
			{
				writer.SetIndent(SPACE_CHAR, 0);
			}

			writer.StartObject();
			writer.String(RSA_KEY_MODULUS_NODE_STRING.c_str());
			writer.String(modulus.c_str());

			writer.String(RSA_KEY_EXPONENT_NODE_STRING.c_str());
			writer.String(exponent.c_str());

			writer.EndObject();
			String publicKey = String(sb.GetString(), sb.GetSize());

			sb.Clear();
			writer.Reset(sb);

			writer.StartObject();
			writer.String(RSA_KEY_MODULUS_NODE_STRING.c_str());
			writer.String(modulus.c_str());

			writer.String(RSA_KEY_EXPONENT_NODE_STRING.c_str());
			writer.String(exponent.c_str());

			writer.String(RSA_KEY_P_NODE_STRING.c_str());
			writer.String(p.c_str());

			writer.String(RSA_KEY_Q_NODE_STRING.c_str());
			writer.String(q.c_str());

			writer.String(RSA_KEY_DP_NODE_STRING.c_str());
			writer.String(dp.c_str());

			writer.String(RSA_KEY_DQ_NODE_STRING.c_str());
			writer.String(dq.c_str());

			writer.String(RSA_KEY_INVERSEQ_NODE_STRING.c_str());
			writer.String(inverseq.c_str());

			writer.String(RSA_KEY_D_NODE_STRING.c_str());
			writer.String(d.c_str());
			writer.EndObject();
			String privateKey = String(sb.GetString(), sb.GetSize());
			return RsaKeyPair(StringExtensions::RemoveBlankSpaces(publicKey), StringExtensions::RemoveBlankSpaces(privateKey));
		}
		catch (...)
		{
			throw Insane::Exception::CryptoException("Unable to serialize json keypair."s);
		}
	}
	default:
		throw CryptoException("Not implemented encoding.");
	}
}

static Insane::Cryptography::RsaKeyEncoding GetKeyEncoding(const String &key)
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_STR;
	USING_NS_INSANE_CORE;
	String rsaKey = StringExtensions::Trim(key);
#ifdef __clang__
	if (StringExtensions::StartsWith(rsaKey, JSON_RSA_PUBLIC_AND_PRIVATE_KEY_INITIAL_STRING) && StringExtensions::EndsWith(rsaKey, JSON_RSA_PUBLIC_AND_PRIVATE_KEY_FINAL_STRING))
#else
	if (Strings::IsMatch(Strings::ReplaceLastOf(rsaKey, "\"", "\","), JSON_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING))
#endif
	{
		return RsaKeyEncoding::Json;
	}

#ifdef __clang__
	if (StringExtensions::StartsWith(rsaKey, XML_RSA_PUBLIC_AND_PRIVATE_KEY_INITIAL_STRING) && StringExtensions::EndsWith(rsaKey, XML_RSA_PUBLIC_AND_PRIVATE_KEY_FINAL_STRING))
#else
	if (Strings::IsMatch(Strings::ReplaceLastOf(rsaKey, "\"", "\","), XML_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING))
#endif
	{
		return RsaKeyEncoding::Xml;
	}

#ifdef __clang__
	if ((StringExtensions::StartsWith(rsaKey, PEM_RSA_PUBLIC_KEY_INITIAL_STRING) && StringExtensions::EndsWith(rsaKey, PEM_RSA_PUBLIC_KEY_FINAL_STRING)) || (StringExtensions::StartsWith(rsaKey, PEM_RSA_PRIVATE_KEY_INITIAL_STRING) && StringExtensions::EndsWith(rsaKey, PEM_RSA_PRIVATE_KEY_FINAL_STRING)))
#else
	if (Strings::IsMatch(Strings::ReplaceLastOf(rsaKey, "\"", "\","), PEM_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING))
#endif
	{
		return RsaKeyEncoding::Pem;
	}
	return RsaKeyEncoding::Ber;
}

static std::unique_ptr<Botan::Public_Key> ParsePublicKey(const String &key)
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	std::unique_ptr<Botan::BigInt> modulus;
	std::unique_ptr<Botan::BigInt> exponent;
	std::unique_ptr<Botan::Public_Key> pbk;

	try
	{
		String publicKey = StringExtensions::Trim(key);
		RsaKeyEncoding encoding = GetKeyEncoding(publicKey);
		switch (encoding)
		{
		case RsaKeyEncoding::Json:
		{
			rapidjson::Document document;
			document.Parse(publicKey.c_str(), publicKey.length());
			if (document.HasParseError())
			{
				throw Insane::Exception::CryptoException();
			}
			modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(document[RSA_KEY_MODULUS_NODE_STRING.c_str()].GetString()));
			exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(document[RSA_KEY_EXPONENT_NODE_STRING.c_str()].GetString()));
			pbk = std::make_unique<Botan::RSA_PublicKey>(*modulus, *exponent);
			break;
		}
		case RsaKeyEncoding::Xml:
		{
			std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
			String xml = String(publicKey);
			doc->parse<0>(xml.data());
			modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_KEY_MODULUS_NODE_STRING.c_str())->value()));
			exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_KEY_EXPONENT_NODE_STRING.c_str())->value()));
			pbk = std::make_unique<Botan::RSA_PublicKey>(*modulus, *exponent);
			break;
		}
		case RsaKeyEncoding::Pem:
		{
			pbk.reset(Botan::X509::load_key(std::vector<uint8_t>(publicKey.begin(), publicKey.end())));
			break;
		}
		case RsaKeyEncoding::Ber:
		{
			Botan::SecureVector<uint8_t> keyBytes = Botan::base64_decode(publicKey);
			pbk.reset(Botan::X509::load_key(std::vector<uint8_t>(keyBytes.begin(), keyBytes.end())));
			break;
		}
		}
	}
	catch (...)
	{
		throw CryptoException("Unable to parse key.");
	}
	return pbk;
}

static std::unique_ptr<Botan::Private_Key> ParsePrivateKey(const String &key)
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	std::unique_ptr<Botan::BigInt> modulus;
	std::unique_ptr<Botan::BigInt> exponent;
	std::unique_ptr<Botan::BigInt> P;
	std::unique_ptr<Botan::BigInt> Q;
	std::unique_ptr<Botan::BigInt> D;
	std::unique_ptr<Botan::Private_Key> pvk;
	try
	{
		String privateKey = StringExtensions::Trim(key);
		RsaKeyEncoding encoding = GetKeyEncoding(privateKey);
		switch (encoding)
		{
		case RsaKeyEncoding::Json:
		{
			rapidjson::Document document;
			document.Parse(privateKey.c_str(), privateKey.length());
			if (document.HasParseError())
			{
				throw Insane::Exception::CryptoException();
			}
			modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(document[RSA_KEY_MODULUS_NODE_STRING.c_str()].GetString()));
			exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(document[RSA_KEY_EXPONENT_NODE_STRING.c_str()].GetString()));
			P = std::make_unique<Botan::BigInt>(Botan::base64_decode(document[RSA_KEY_P_NODE_STRING.c_str()].GetString()));
			Q = std::make_unique<Botan::BigInt>(Botan::base64_decode(document[RSA_KEY_Q_NODE_STRING.c_str()].GetString()));
			D = std::make_unique<Botan::BigInt>(Botan::base64_decode(document[RSA_KEY_D_NODE_STRING.c_str()].GetString()));
			pvk = std::make_unique<Botan::RSA_PrivateKey>(*P, *Q, *exponent, *D, *modulus);
			break;
		}
		case RsaKeyEncoding::Xml:
		{
			std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
			String xml = String(privateKey);
			doc->parse<0>(xml.data());
			modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_KEY_MODULUS_NODE_STRING.c_str())->value()));
			exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_KEY_EXPONENT_NODE_STRING.c_str())->value()));
			P = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_KEY_P_NODE_STRING.c_str())->value()));
			Q = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_KEY_Q_NODE_STRING.c_str())->value()));
			D = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_KEY_D_NODE_STRING.c_str())->value()));
			pvk = std::make_unique<Botan::RSA_PrivateKey>(*P, *Q, *exponent, *D, *modulus);
			break;
		}
		case RsaKeyEncoding::Pem:
		{
			Botan::SecureVector<uint8_t> keyBytes(privateKey.begin(), privateKey.end());
			std::unique_ptr<Botan::DataSource_Memory> source = std::make_unique<Botan::DataSource_Memory>(keyBytes);
			pvk = Botan::PKCS8::load_key(*source);
			break;
		}
		case RsaKeyEncoding::Ber:
		{
			Botan::SecureVector<uint8_t> keyBytes(Botan::base64_decode(privateKey));
			std::unique_ptr<Botan::DataSource_Memory> source = std::make_unique<Botan::DataSource_Memory>(keyBytes);
			pvk = Botan::PKCS8::load_key(*source);
			break;
		}
		}
	}
	catch (Botan::Exception &exc)
	{
		throw CryptoException("Unable to parse key.");
	}
	catch (...)
	{
		throw CryptoException("Unable to parse key.");
	}
	return pvk;
}

String Insane::Cryptography::RsaManager::EncryptRaw(const String &data, const String &publicKey) noexcept(false)
{
	USING_NS_INSANE_STR;
	USING_NS_INSANE_EXCEPTION;
	try
	{
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		std::unique_ptr<Botan::Public_Key> pbk = ParsePublicKey(publicKey);
		std::unique_ptr<Botan::PK_Encryptor_EME> enc = std::make_unique<Botan::PK_Encryptor_EME>(*pbk, *rng, RSA_PADDING_ALGORITHM_STRING);
		Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
		std::vector<uint8_t> encrypted = enc->encrypt(dataBytes, *rng);
		return String(encrypted.begin(), encrypted.end());
	}
	catch (const Botan::Exception &e)
	{
		throw CryptoException(e.what(), e.error_code());
	}
	catch (...)
	{
		throw CryptoException("Fatal error. Unable to RsaEncrypt.");
	}
}

String Insane::Cryptography::RsaManager::DecryptRaw(const String &data, const String &privateKey) noexcept(false)
{
	USING_NS_INSANE_EXCEPTION;
	try
	{
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		std::unique_ptr<Botan::Private_Key> pvk = ParsePrivateKey(privateKey);
		std::unique_ptr<Botan::PK_Decryptor_EME> dec = std::make_unique<Botan::PK_Decryptor_EME>(*pvk, *rng, RSA_PADDING_ALGORITHM_STRING);
		Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
		Botan::SecureVector<uint8_t> decrypted = dec->decrypt(dataBytes);
		return String(decrypted.begin(), decrypted.end());
	}
	catch (const Botan::Exception &e)
	{
		throw CryptoException(e.what(), e.error_code());
	}
	catch (...)
	{
		throw CryptoException();
	}
}

String Insane::Cryptography::RsaManager::EncryptToBase64(const String &data, const String &publicKey) noexcept(false)
{
	return Base64EncodingExtensions::ToBase64(EncryptRaw(data, publicKey));
}

String Insane::Cryptography::RsaManager::DecryptFromBase64(const String &data, const String &privateKey) noexcept(false)
{
	return DecryptRaw(Base64EncodingExtensions::FromBase64(data), privateKey);
}

// ███ HexEncodingExtensions ███

String Insane::Cryptography::HexEncodingExtensions::FromHex(const String &data)
{
	auto decoded = Botan::hex_decode(data);
	return String(decoded.begin(), decoded.end());
}

String Insane::Cryptography::HexEncodingExtensions::ToHex(const String &data, const bool &toUpper)
{
	return Botan::hex_encode(std::vector<uint8_t>(data.begin(), data.end()), toUpper);
}

// ███ Base32EncodingExtensions ███

String Insane::Cryptography::Base32EncodingExtensions::FromBase32(const String &data)
{
	USING_NS_INSANE_STR;
	auto decoded = Botan::base32_decode(StringExtensions::ToUpper(data));
	return String(decoded.begin(), decoded.end());
}

String Insane::Cryptography::Base32EncodingExtensions::ToBase32(const String &data, const bool &removePadding, const bool &toLower)
{
	USING_NS_INSANE_STR;
	auto encoded = Botan::base32_encode(std::vector<uint8_t>(data.begin(), data.end()));
	encoded = removePadding ? StringExtensions::Replace(encoded, EQUAL_SIGN_STRING, EMPTY_STRING) : encoded;
	return toLower ? StringExtensions::ToLower(encoded) : encoded;
}

// ███ Base64EncodingExtensions ███

String Insane::Cryptography::Base64EncodingExtensions::ToBase64(const String &data, const size_t &lineBreaksLength, const bool &removePadding)
{
	USING_NS_INSANE_STR;
	String ret = Botan::base64_encode(std::vector<uint8_t>(data.begin(), data.end()));
	if (lineBreaksLength > 0)
	{
		ret = StringExtensions::InsertRepeat(ret, lineBreaksLength, NEW_LINE_STR);
	}
	return removePadding ? StringExtensions::Remove(ret, EQUAL_SIGN_STRING) : ret;
}

String Insane::Cryptography::Base64EncodingExtensions::FromBase64(const String &data)
{
	USING_NS_INSANE_STR;
	USING_NS_INSANE_CORE;
	String base64 = data;
	base64 = StringExtensions::Replace(base64, {{URL_ENCODED_PLUS_SIGN_STRING, PLUS_SIGN_STRING}, {URL_ENCODED_SLASH_STRING, SLASH_STRING}, {URL_ENCODED_EQUAL_SIGN_STRING, EQUAL_SIGN_STRING}, {MINUS_SIGN_STRING, PLUS_SIGN_STRING}, {UNDERSCORE_STRING, SLASH_STRING}});
	base64 = StringExtensions::RemoveBlankSpaces(base64);
	int modulo = base64.length() % 4;
	base64 = StringExtensions::PadRight(base64, base64.length() + (modulo > 0 ? 4 - modulo : 0), EQUAL_SIGN_STRING);
	Botan::secure_vector<uint8_t> result = Botan::base64_decode(base64);
	return String(result.begin(), result.end());
}

String Insane::Cryptography::Base64EncodingExtensions::ToUrlSafeBase64(const String &data)
{
	USING_NS_INSANE_STR;
	return StringExtensions::Replace(ToBase64(data), {{PLUS_SIGN_STRING, MINUS_SIGN_STRING}, {SLASH_STRING, UNDERSCORE_STRING}, {EQUAL_SIGN_STRING, EMPTY_STRING}});
}

String Insane::Cryptography::Base64EncodingExtensions::ToFilenameSafeBase64(const String &data)
{
	USING_NS_INSANE_STR;
	return ToUrlSafeBase64(data);
}

String Insane::Cryptography::Base64EncodingExtensions::ToUrlEncodedBase64(const String &data)
{
	USING_NS_INSANE_STR;
	return StringExtensions::Replace(ToBase64(data), {{PLUS_SIGN_STRING, URL_ENCODED_PLUS_SIGN_STRING}, {SLASH_STRING, URL_ENCODED_SLASH_STRING}, {EQUAL_SIGN_STRING, URL_ENCODED_EQUAL_SIGN_STRING}});
}

String Insane::Cryptography::Base64EncodingExtensions::Base64ToUrlSafeBase64(const String &base64)
{
	return ToUrlSafeBase64(FromBase64(base64));
}

String Insane::Cryptography::Base64EncodingExtensions::Base64ToFilenameSafeBase64(const String &base64)
{
	return ToFilenameSafeBase64(FromBase64(base64));
}

String Insane::Cryptography::Base64EncodingExtensions::Base64ToUrlEncodedBase64(const String &base64)
{
	return ToUrlEncodedBase64(FromBase64(base64));
}

// ███ CryptoTests ███
void Insane::Cryptography::CryptoTests::HexEncodingExtensionsTests()
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_TEST;
}

void Insane::Cryptography::CryptoTests::Base32EncodingExtensionsTests()
{
	/*
	Console::Log(Base32EncodingExtensions::ToBase32("A"));
    Console::Log(Base32EncodingExtensions::ToBase32("helloworld", false, false));
    Console::Log(Base32EncodingExtensions::ToBase32("helloworld", false, true));
    Console::Log(Base32EncodingExtensions::ToBase32("A", false, false));
    Console::Log(Base32EncodingExtensions::ToBase32("A", false, true));
    Console::Log(Base32EncodingExtensions::ToBase32("A", true, false));
    Console::Log(Base32EncodingExtensions::ToBase32("A", true, true));
    Console::Log("Decoded: " + Base32EncodingExtensions::FromBase32("IE======"));
    Console::Log("Decoded: " + Base32EncodingExtensions::FromBase32("IE"));
    Console::Log("Decoded: " + Base32EncodingExtensions::FromBase32("ie======"));
    Console::Log("Decoded: "+ Base32EncodingExtensions::FromBase32("ie"));
	*/
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_TEST;

}

void Insane::Cryptography::CryptoTests::Base64EncodingExtensionsTests()
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_TEST;
	auto testBytes = {(char)0x30, (char)0x82, (char)0x02, (char)0x22, (char)0x30, (char)0x0d, (char)0x06, (char)0x09, (char)0x2a, (char)0x86, (char)0x48, (char)0x86, (char)0xf7, (char)0x0d, (char)0x01, (char)0x01, (char)0x01, (char)0x05, (char)0x00, (char)0x03, (char)0x82, (char)0x02, (char)0x0f, (char)0x00, (char)0x30, (char)0x82, (char)0x02, (char)0x0a, (char)0x02, (char)0x82, (char)0x02, (char)0x01, (char)0x00, (char)0xf2, (char)0xe8, (char)0xe5, (char)0x81, (char)0x32, (char)0x36, (char)0xb8, (char)0xb6, (char)0x3f, (char)0xb5, (char)0xbe, (char)0x76, (char)0x65, (char)0x65, (char)0xd1, (char)0x8f, (char)0x2d, (char)0xc4, (char)0xc5, (char)0xa1, (char)0x91, (char)0x3b, (char)0x8b, (char)0xdc, (char)0x8b, (char)0xf6, (char)0x4f, (char)0x42, (char)0x64, (char)0xd1, (char)0xea, (char)0xdc, (char)0x75, (char)0x6c, (char)0x83, (char)0x0b, (char)0x81, (char)0x1f, (char)0x57, (char)0xeb, (char)0xac, (char)0xe5, (char)0xd0, (char)0x5c, (char)0x6b, (char)0x5f, (char)0x37, (char)0xa8, (char)0x53, (char)0x1c, (char)0x65, (char)0x6b, (char)0x75, (char)0x5e, (char)0xbc, (char)0xd3, (char)0x59, (char)0xd2, (char)0x54, (char)0x17, (char)0xf7, (char)0x69, (char)0x4d, (char)0x23, (char)0x92, (char)0x7e, (char)0x78, (char)0x47, (char)0xf1, (char)0x06, (char)0x04, (char)0x5b, (char)0x55, (char)0x00, (char)0xb1, (char)0xaa, (char)0x82, (char)0x70, (char)0x70, (char)0xc0, (char)0xff, (char)0x3c, (char)0x29, (char)0x4a, (char)0x2f, (char)0xc3, (char)0xff, (char)0x56, (char)0x60, (char)0x4a, (char)0x22, (char)0x12, (char)0xfe, (char)0x10, (char)0xa4, (char)0xe1, (char)0xeb, (char)0x9d, (char)0x82, (char)0xb3, (char)0x76, (char)0x1c, (char)0xa0, (char)0x18, (char)0x4c, (char)0xca, (char)0xcd, (char)0x68, (char)0x40, (char)0x2e, (char)0x6a, (char)0x21, (char)0x2a, (char)0x7b, (char)0x7b, (char)0xc6, (char)0x0b, (char)0x85, (char)0x14, (char)0x19, (char)0x03, (char)0x40, (char)0xe9, (char)0x78, (char)0x54, (char)0xfe, (char)0x97, (char)0xf4, (char)0xe8, (char)0x39, (char)0x45, (char)0x06, (char)0x76, (char)0x8e, (char)0x5e, (char)0x0e, (char)0xdb, (char)0x62, (char)0x41, (char)0x60, (char)0x2b, (char)0xfb, (char)0x1e, (char)0x1a, (char)0x65, (char)0x3a, (char)0x25, (char)0x48, (char)0xba, (char)0xe6, (char)0x73, (char)0x8f, (char)0x35, (char)0xf0, (char)0xfd, (char)0x99, (char)0xe4, (char)0x1d, (char)0xe9, (char)0xbf, (char)0x67, (char)0x8b, (char)0xf4, (char)0x1d, (char)0xfa, (char)0xfa, (char)0x58, (char)0x8e, (char)0xe7, (char)0x1b, (char)0x7b, (char)0xb5, (char)0x7d, (char)0x74, (char)0x90, (char)0x26, (char)0x41, (char)0x88, (char)0xbd, (char)0x4d, (char)0x20, (char)0x69, (char)0x4b, (char)0x4c, (char)0x8a, (char)0xef, (char)0x47, (char)0x87, (char)0xc1, (char)0xf3, (char)0x5b, (char)0x42, (char)0x79, (char)0x04, (char)0xd7, (char)0x9d, (char)0x42, (char)0xa7, (char)0xdf, (char)0xca, (char)0x0d, (char)0xf4, (char)0x19, (char)0x4a, (char)0x8d, (char)0x7c, (char)0x93, (char)0x3f, (char)0x1a, (char)0xa5, (char)0x39, (char)0xef, (char)0xcd, (char)0x6d, (char)0xe5, (char)0x0a, (char)0xe5, (char)0xf0, (char)0x41, (char)0x16, (char)0x96, (char)0x58, (char)0x14, (char)0x99, (char)0x77, (char)0xdc, (char)0x69, (char)0x27, (char)0xc7, (char)0xa6, (char)0x11, (char)0xb4, (char)0xd3, (char)0xa2, (char)0x17, (char)0x23, (char)0x50, (char)0xa0, (char)0xbd, (char)0x06, (char)0x7d, (char)0x5a, (char)0x72, (char)0xa0, (char)0xb1, (char)0xed, (char)0x48, (char)0xd1, (char)0x42, (char)0xfc, (char)0x66, (char)0x3e, (char)0x4a, (char)0x22, (char)0x69, (char)0xac, (char)0xe4, (char)0xee, (char)0x82, (char)0xbc, (char)0x48, (char)0x83, (char)0x81, (char)0x34, (char)0x6e, (char)0x29, (char)0x4b, (char)0x64, (char)0x71, (char)0x37, (char)0x25, (char)0x13, (char)0x28, (char)0x52, (char)0x71, (char)0x5b, (char)0xd5, (char)0x95, (char)0x20, (char)0xa5, (char)0xb4, (char)0x66, (char)0xa7, (char)0x9e, (char)0x06, (char)0x5f, (char)0x2d, (char)0x8e, (char)0x78, (char)0xf5, (char)0x37, (char)0xcf, (char)0xed, (char)0x65, (char)0x84, (char)0xdf, (char)0xda, (char)0x78, (char)0x27, (char)0xa9, (char)0x09, (char)0xaa, (char)0x70, (char)0x73, (char)0x5a, (char)0xc6, (char)0xa9, (char)0xba, (char)0xb7, (char)0xce, (char)0x38, (char)0x2c, (char)0x28, (char)0x4b, (char)0x3e, (char)0xae, (char)0x11, (char)0x3c, (char)0xed, (char)0x94, (char)0xd9, (char)0x2a, (char)0x26, (char)0xd2, (char)0xbc, (char)0xa5, (char)0x19, (char)0x7c, (char)0x3a, (char)0x98, (char)0x0a, (char)0x51, (char)0xdb, (char)0x14, (char)0x99, (char)0xd8, (char)0x4e, (char)0xc3, (char)0x5d, (char)0x0a, (char)0xc9, (char)0x93, (char)0xa9, (char)0xce, (char)0xb0, (char)0x12, (char)0x62, (char)0x6b, (char)0x6b, (char)0x48, (char)0x42, (char)0x42, (char)0x04, (char)0x95, (char)0x29, (char)0x77, (char)0x49, (char)0xaa, (char)0x88, (char)0x2a, (char)0x94, (char)0xcd, (char)0x55, (char)0x7d, (char)0xb6, (char)0xcb, (char)0xb1, (char)0x1e, (char)0x93, (char)0xa9, (char)0xa2, (char)0xba, (char)0x73, (char)0xff, (char)0x2e, (char)0xa6, (char)0xff, (char)0xd6, (char)0x14, (char)0x65, (char)0x3b, (char)0x8c, (char)0x7d, (char)0x0b, (char)0xa7, (char)0xda, (char)0xbd, (char)0x50, (char)0x2c, (char)0x1d, (char)0x2e, (char)0xf1, (char)0xd9, (char)0xf5, (char)0x8a, (char)0x08, (char)0xe9, (char)0x54, (char)0x7d, (char)0x4a, (char)0x25, (char)0xf5, (char)0xb7, (char)0x53, (char)0xd8, (char)0x3f, (char)0xad, (char)0x98, (char)0x5f, (char)0xea, (char)0xa9, (char)0xd5, (char)0x3d, (char)0x13, (char)0x7d, (char)0x26, (char)0x5d, (char)0xab, (char)0x0e, (char)0xa6, (char)0xcd, (char)0xe7, (char)0xc1, (char)0x81, (char)0x0f, (char)0x12, (char)0x8c, (char)0x59, (char)0x77, (char)0xa9, (char)0x67, (char)0xa4, (char)0x37, (char)0xf3, (char)0x8e, (char)0xdf, (char)0xe5, (char)0x5c, (char)0x0c, (char)0x65, (char)0x07, (char)0x93, (char)0xcd, (char)0xb0, (char)0xeb, (char)0x19, (char)0x89, (char)0x6f, (char)0x81, (char)0x90, (char)0x9a, (char)0xf4, (char)0x99, (char)0xb8, (char)0x33, (char)0x35, (char)0xdb, (char)0x40, (char)0x8e, (char)0x85, (char)0x53, (char)0x26, (char)0x4a, (char)0xe9, (char)0x8c, (char)0x5a, (char)0x5d, (char)0x68, (char)0xd5, (char)0x4e, (char)0xff, (char)0x21, (char)0x77, (char)0xb9, (char)0xcb, (char)0xc1, (char)0xaf, (char)0x69, (char)0x69, (char)0x10, (char)0x56, (char)0x6d, (char)0x9e, (char)0xbd, (char)0xe4, (char)0xa4, (char)0x2b, (char)0xd9, (char)0xf9, (char)0x65, (char)0x63, (char)0xb5, (char)0x00, (char)0x48, (char)0xb0, (char)0x04, (char)0xca, (char)0x98, (char)0x10, (char)0x8e, (char)0x2a, (char)0x4f, (char)0x18, (char)0x47, (char)0xef, (char)0x5e, (char)0x26, (char)0x07, (char)0x72, (char)0xf9, (char)0xbe, (char)0x25, (char)0x02, (char)0x03, (char)0x01, (char)0x00, (char)0x01};
	String str = String(testBytes.begin(), testBytes.end());
	String data = "A";
	String expected = "QQ==";
	String result = Base64EncodingExtensions::ToBase64(data);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 1", expected, result);

	data = "AA";
	expected = "QUE=";
	result = Base64EncodingExtensions::ToBase64(data);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 2", expected, result);

	data = "AAA";
	expected = "QUFB";
	result = Base64EncodingExtensions::ToBase64(data);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 3", expected, result);

	data = "A";
	expected = "QQ";
	result = Base64EncodingExtensions::ToBase64(data, 0, true);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 4", expected, result);

	data = "AA";
	expected = "QUE";
	result = Base64EncodingExtensions::ToBase64(data, 0, true);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 5", expected, result);

	data = "AAA";
	expected = "QUFB";
	result = Base64EncodingExtensions::ToBase64(data, 0, true);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 6", expected, result);

	String TestMimeBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXRjy3ExaGRO4vc
i/ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J+eEfxBgRbVQCxqoJwcMD/
PClKL8P/VmBKIhL+EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT+l/ToOUUGdo5eDtti
QWAr+x4aZTolSLrmc4818P2Z5B3pv2eL9B36+liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTX
nUKn38oN9BlKjXyTPxqlOe/NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC/GY+
SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z+1lhN/aeCepCapwc1rGqbq3
zjgsKEs+rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbL
sR6TqaK6c/8upv/WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX+qp1T0TfSZdqw6mzefB
gQ8SjFl3qWekN/OO3+VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7/IXe5y8GvaWkQ
Vm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ==)";
	result = Base64EncodingExtensions::ToBase64(str, MIME_LINE_BREAKS_LENGTH);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 7", TestMimeBase64String, result);

	String TestPemBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXR
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
	result = Base64EncodingExtensions::ToBase64(str, PEM_LINE_BREAKS_LENGTH);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 8", TestPemBase64String, result);

	String TestBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXRjy3ExaGRO4vci/ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J+eEfxBgRbVQCxqoJwcMD/PClKL8P/VmBKIhL+EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT+l/ToOUUGdo5eDttiQWAr+x4aZTolSLrmc4818P2Z5B3pv2eL9B36+liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe/NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC/GY+SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z+1lhN/aeCepCapwc1rGqbq3zjgsKEs+rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c/8upv/WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX+qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN/OO3+VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7/IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ==)";
	result = Base64EncodingExtensions::ToBase64(str);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 9", TestBase64String, result);

	String TestBase64StringNoPadding = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXRjy3ExaGRO4vci/ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J+eEfxBgRbVQCxqoJwcMD/PClKL8P/VmBKIhL+EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT+l/ToOUUGdo5eDttiQWAr+x4aZTolSLrmc4818P2Z5B3pv2eL9B36+liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe/NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC/GY+SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z+1lhN/aeCepCapwc1rGqbq3zjgsKEs+rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c/8upv/WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX+qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN/OO3+VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7/IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ)";
	result = Base64EncodingExtensions::ToBase64(str, 0, true);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 10", TestBase64StringNoPadding, result);

	String TestUrlSafeBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY_tb52ZWXRjy3ExaGRO4vci_ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J-eEfxBgRbVQCxqoJwcMD_PClKL8P_VmBKIhL-EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT-l_ToOUUGdo5eDttiQWAr-x4aZTolSLrmc4818P2Z5B3pv2eL9B36-liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe_NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC_GY-SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z-1lhN_aeCepCapwc1rGqbq3zjgsKEs-rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c_8upv_WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX-qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN_OO3-VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7_IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ)";
	result = Base64EncodingExtensions::ToUrlSafeBase64(str);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 11", TestUrlSafeBase64String, result);

	String TestFileNameSafeBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY_tb52ZWXRjy3ExaGRO4vci_ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J-eEfxBgRbVQCxqoJwcMD_PClKL8P_VmBKIhL-EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT-l_ToOUUGdo5eDttiQWAr-x4aZTolSLrmc4818P2Z5B3pv2eL9B36-liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe_NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC_GY-SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z-1lhN_aeCepCapwc1rGqbq3zjgsKEs-rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c_8upv_WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX-qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN_OO3-VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7_IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ)";
	result = Base64EncodingExtensions::ToFilenameSafeBase64(str);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 12", TestFileNameSafeBase64String, result);

	String TestUrlEncodedBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY%2Ftb52ZWXRjy3ExaGRO4vci%2FZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J%2BeEfxBgRbVQCxqoJwcMD%2FPClKL8P%2FVmBKIhL%2BEKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT%2Bl%2FToOUUGdo5eDttiQWAr%2Bx4aZTolSLrmc4818P2Z5B3pv2eL9B36%2BliO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe%2FNbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC%2FGY%2BSiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z%2B1lhN%2FaeCepCapwc1rGqbq3zjgsKEs%2BrhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c%2F8upv%2FWFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX%2Bqp1T0TfSZdqw6mzefBgQ8SjFl3qWekN%2FOO3%2BVcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7%2FIXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ%3D%3D)";
	result = Base64EncodingExtensions::ToUrlEncodedBase64(str);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 13", TestUrlEncodedBase64String, result);

	result = Base64EncodingExtensions::Base64ToUrlSafeBase64(TestBase64String);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 14", TestUrlSafeBase64String, result);

	result = Base64EncodingExtensions::Base64ToFilenameSafeBase64(TestBase64String);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 15", TestFileNameSafeBase64String, result);

	result = Base64EncodingExtensions::Base64ToUrlEncodedBase64(TestBase64String);
	TestExtensions::Equals(NAMEOF(Base64ExtensionsTests) + " 16", TestUrlEncodedBase64String, result);
}
