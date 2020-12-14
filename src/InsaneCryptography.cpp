#include <Insane/InsaneException.h>
#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneCore.h>
#include <botan/cipher_mode.h>
#include <botan/auto_rng.h>
#include <botan/base64.h>
#include <botan/pkcs8.h>
#include <botan/rsa.h>
#include <botan/x509_key.h>
#include <botan/data_src.h>
#include <botan/bigint.h>
#include <botan/pubkey.h>
#include <botan/hash.h>
#include <stdexcept>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <botan/exceptn.h>

#define AES_MAX_IV_LENGHT ((size_t)16)
#define AES_MAX_KEY_LENGTH ((size_t)32)

#define MD5_DIGEST_LENGTH 16
#define SHA1_DIGEST_LENGTH 20
#define SHA256_DIGEST_LENGTH 32
#define SHA384_DIGEST_LENGTH 48
#define SHA512_DIGEST_LENGTH 64

#define HMAC_INNER_PADDING ((Char)0x36)
#define HMAC_OUTER_PADDING ((Char)0x5c)
#define HMAC_64_BYTES_BLOCK_SIZE ((Int32)64)
#define HMAC_128_BYTES_BLOCK_SIZE ((Int32)128)

#define RSA_KEY_MAIN_NODE_STRING u8"RSAKeyValue"s
#define RSA_KEY_P_NODE_STRING u8"P"s
#define RSA_KEY_Q_NODE_STRING u8"Q"s
#define RSA_KEY_DP_NODE_STRING u8"DP"s
#define RSA_KEY_DQ_NODE_STRING u8"DQ"s
#define RSA_KEY_INVERSEQ_NODE_STRING u8"InverseQ"s
#define RSA_KEY_D_NODE_STRING u8"D"s
#define RSA_KEY_MODULUS_NODE_STRING u8"Modulus"s
#define RSA_KEY_EXPONENT_NODE_STRING u8"Exponent"s
#define RSA_PADDING_ALGORITHM_STRING u8"EME-PKCS1-v1_5"s

#define XML_RSA_PUBLIC_AND_PRIVATE_KEY_INITIAL_STRING u8"<RSAKeyValue>"s
#define XML_RSA_PUBLIC_AND_PRIVATE_KEY_FINAL_STRING u8"</RSAKeyValue>"s
#define JSON_RSA_PUBLIC_AND_PRIVATE_KEY_INITIAL_STRING u8"{"
#define JSON_RSA_PUBLIC_AND_PRIVATE_KEY_FINAL_STRING u8"}"
#define PEM_RSA_PRIVATE_KEY_INITIAL_STRING u8"-----BEGIN PRIVATE KEY-----"
#define PEM_RSA_PUBLIC_KEY_INITIAL_STRING u8"-----BEGIN PUBLIC KEY-----"
#define PEM_RSA_PRIVATE_KEY_FINAL_STRING u8"-----END PRIVATE KEY-----"
#define PEM_RSA_PUBLIC_KEY_FINAL_STRING u8"-----END PUBLIC KEY-----"

#define JSON_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING u8R"((\s*\{(?:\s*"Modulus"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"Exponent"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"P"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"Q"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"DP"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"DQ"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"InverseQ"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"D"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()){8}\s*\2\3\4\5\6\7\8\9\}\s*)|(\s*\{(?:\s*"Modulus"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"Exponent"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()){2}\s*\11\12\}\s*))"																																																 //https://regex101.com/r/v5lUWw/4 //Add final Comma.
#define PEM_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING u8R"((-----BEGIN PUBLIC KEY-----(\n|\r|\r\n)([0-9a-zA-Z\+\/=]{64}(\n|\r|\r\n))*([0-9a-zA-Z\+\/=]{1,63}(\n|\r|\r\n))?-----END PUBLIC KEY-----)|(-----BEGIN PRIVATE KEY-----(\n|\r|\r\n)([0-9a-zA-Z\+\/=]{64}(\n|\r|\r\n))*([0-9a-zA-Z\+\/=]{1,63}(\n|\r|\r\n))?-----END PRIVATE KEY-----))"																																																																																																																								 //https://regex101.com/r/mGnr7I/1
#define XML_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING u8R"((\s*<\s*RSAKeyValue\s*>\s*(?:\s*<\s*Modulus\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Modulus\s*>()|\s*<\s*Exponent\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Exponent\s*>()|\s*<\s*P\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*P\s*>()|\s*<\s*Q\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Q\s*>()|\s*<\s*DP\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*DP\s*>()|\s*<\s*DQ\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*DQ\s*>()|\s*<\s*InverseQ\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*InverseQ\s*>()|\s*<\s*D\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*D\s*>()){8}\s*<\/\s*RSAKeyValue\s*>\s*\2\3\4\5\6\7\8\9)|(\s*<\s*RSAKeyValue\s*>\s*(?:\s*<\s*Modulus\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Modulus\s*>()|\s*<\s*Exponent\s*>\s*[a-zA-Z0-9\+\/]+={0,2}\s*<\/\s*Exponent\s*>()){2}\s*<\/\s*RSAKeyValue\s*>\s*\11\12))" //https://regex101.com/r/fQV2VN/4
// ███ HashManager ███

Insane::Crypto::HashManager::HashManager() = default;

Insane::Crypto::HashManager::~HashManager() = default;

String Insane::Crypto::HashManager::ToBase64(const String &data, size_t lineBreaks, const bool &removePadding)
{
	USING_INSANE_STR;
	String ret = Botan::base64_encode(std::vector<uint8_t>(data.begin(), data.end()));
	ret = InsertLineBreaks(ret, lineBreaks);
	return removePadding ? Strings::RemoveAll(ret, u8"=") : ret;
}

String Insane::Crypto::HashManager::ToUrlSafeBase64(const String &data)
{
	USING_INSANE_STR;
	return Strings::ReplaceAll(ToBase64(data), {{u8"+", u8"-"}, {u8"/", u8"_"}, {u8"=", EMPTY_STRING}});
}

String Insane::Crypto::HashManager::ToFilenameSafeBase64(const String &data)
{
	USING_INSANE_STR;
	return ToUrlSafeBase64(data);
}

String Insane::Crypto::HashManager::ToUrlEncodedBase64(const String &data)
{
	USING_INSANE_STR;
	return Strings::ReplaceAll(ToBase64(data), {{u8"+", u8"%2B"}, {u8"/", u8"%2F"}, {u8"=", u8"%3D"}});
}

String Insane::Crypto::HashManager::ToAlphanumericBase64(const String &data, size_t lineBreaks)
{
	USING_INSANE_STR;
	return Strings::RemoveAll(ToBase64(data, lineBreaks), {u8"+"s, u8"-"s, u8"/"s, u8"_"s, u8"="s, u8","s});
}

String Insane::Crypto::HashManager::FromBase64(const String &base64)
{
	USING_INSANE_STR;
	USING_INSANE_CORE;
	String data = base64;
	data = RemoveLineBreaks(Strings::ReplaceAll(data, {{u8"%2B", u8"+"}, {u8"%2F", u8"/"}, {u8"%3D", u8"="}, {u8"-", u8"+"}, {u8"_", u8"/"}}));
	int modulo = data.length() % 4;
	data = modulo == 0 ? data : Strings::PadRight(data, data.length() + modulo, u8'=');
	auto result = Botan::base64_decode(data);
	return String(result.begin(), result.end());
}

String Insane::Crypto::HashManager::InsertLineBreaks(const String &base64, size_t lineBreakAppear)
{
	if (lineBreakAppear == 0)
	{
		return base64;
	}
	return Str::Strings::InsertRepeat(base64, lineBreakAppear, NEW_LINE_STR);
}

String Insane::Crypto::HashManager::RemoveLineBreaks(const String &base64)
{
	USING_INSANE_STR;
	return Strings::RemoveAll(base64, {CARRIAGE_RETURN_STRING, LINE_FEED_STRING, VERTICAL_TAB_STRING, FORM_FEED_STRING, TAB_STRING, SPACE_STRING});
}

String Insane::Crypto::HashManager::ToRawHash(const String &data, HashAlgorithm algorithm)
{
	USING_INSANE_EXCEPTION;
	switch (algorithm)
	{
	case HashAlgorithm::Md5:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(u8"MD5"s));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::Sha1:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(u8"SHA-1"s));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::Sha256:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(u8"SHA-256"s));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::Sha384:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(u8"SHA-384"s));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::Sha512:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(u8"SHA-512"s));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	default:
		throw CryptoException(u8"Not implemented algorithm."s);
	}
}

String Insane::Crypto::HashManager::ToBase64Hash(const String &data, HashAlgorithm algorithm, size_t lineBreaks)
{
	return HashManager::ToBase64(ToRawHash(data, algorithm), lineBreaks);
}

String Insane::Crypto::HashManager::ToRawHmac(const String &data, const String &key, HashAlgorithm algorithm)
{
	int blockSize = 0;
	String secret = key;
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
	}

	if (secret.length() > blockSize)
	{
		secret = HashManager::ToRawHash(secret, algorithm);
	}

	if (secret.length() < blockSize)
	{
		secret.append(String((size_t)(blockSize - secret.length()), static_cast<char>(0)));
	}
	String outerKeyPadding = String(blockSize, HMAC_OUTER_PADDING);
	String innerKeyPadding = String(blockSize, HMAC_INNER_PADDING);
	for (int i = 0; i < blockSize; i++)
	{
		innerKeyPadding[i] = (Char)(secret[i] ^ innerKeyPadding[i]);
		outerKeyPadding[i] = (Char)(secret[i] ^ outerKeyPadding[i]);
	}
	innerKeyPadding.append(data);
	String ret = HashManager::ToRawHash(innerKeyPadding, algorithm);
	outerKeyPadding.append(ret);
	ret = HashManager::ToRawHash(outerKeyPadding, algorithm);
	return ret;
}

String Insane::Crypto::HashManager::ToBase64Hmac(const String &data, const String &key, HashAlgorithm algorithm, size_t lineBreaks)
{
	return HashManager::ToBase64(ToRawHmac(data, key, algorithm), lineBreaks);
}

// ███ AesManager ███

Insane::Crypto::AesManager::AesManager() = default;

Insane::Crypto::AesManager::~AesManager() = default;

String Insane::Crypto::AesManager::EncryptRaw(const String &data, const String &key) noexcept(false)
{
	USING_INSANE_EXCEPTION;
	try
	{
		String secretKey = GenerateValidKey(key);
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		std::unique_ptr<Botan::Cipher_Mode> enc = Botan::Cipher_Mode::create(u8"AES-256/CBC/PKCS7"s, Botan::ENCRYPTION);
		enc->set_key(Botan::secure_vector<uint8_t>(secretKey.begin(), secretKey.end()));
		Botan::secure_vector<uint8_t> ivBytes = rng->random_vec(AES_MAX_IV_LENGHT);
		Botan::secure_vector<uint8_t> dataBytes(data.begin(), data.end());
		enc->start(ivBytes);
		enc->finish(dataBytes);
		std::string result = std::string(dataBytes.begin(), dataBytes.end());
		result += std::string(ivBytes.begin(), ivBytes.end());
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

String Insane::Crypto::AesManager::DecryptRaw(const String &data, const String &key)
{
	try
	{
		String secretKey = GenerateValidKey(key);
		std::unique_ptr<Botan::Cipher_Mode> dec = Botan::Cipher_Mode::create(u8"AES-256/CBC/PKCS7"s, Botan::DECRYPTION);
		dec->set_key(Botan::SecureVector<uint8_t>(secretKey.begin(), secretKey.end()));
		Botan::secure_vector<uint8_t> dataBytes(data.begin(), data.end() - AES_MAX_IV_LENGHT);
		dec->start(Botan::secure_vector<uint8_t>(data.end() - AES_MAX_IV_LENGHT, data.end()));
		dec->finish(dataBytes);
		return std::string(dataBytes.begin(), dataBytes.end());
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

String Insane::Crypto::AesManager::EncryptToBase64(const String &data, const String &key) noexcept(false)
{
	return HashManager::ToBase64(EncryptRaw(data, key));
}

String Insane::Crypto::AesManager::DecryptFromBase64(const String &data, const String &key) noexcept(false)
{
	return DecryptRaw(HashManager::FromBase64(data), key);
}

String Insane::Crypto::AesManager::GenerateValidKey(const String &key)
{
	String hash = HashManager::ToRawHash(key, HashAlgorithm::Sha512);
	hash.resize(AES_MAX_KEY_LENGTH);
	return hash;
}

// ███ RandomManager ███

Insane::Crypto::RandomManager::RandomManager() = default;

Insane::Crypto::RandomManager::~RandomManager() = default;

String Insane::Crypto::RandomManager::Generate(int sz)
{
	std::mt19937 mersenneEngine(std::random_device{}());
	std::uniform_int_distribution<> dist(CHAR_MIN, CHAR_MAX);
	const auto generator = std::bind(dist, mersenneEngine);
	std::vector<char> v(sz);
	std::generate(v.begin(), v.end(), generator);
	return String(v.data(), v.size());
}

int Insane::Crypto::RandomManager::Generate(int min, int max)
{
	std::random_device device = std::random_device{};
	std::mt19937 mersenneEngine(device());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(mersenneEngine);
}

int Insane::Crypto::RandomManager::Generate()
{
	return Generate(INT_MIN, INT_MAX);
}

// ███ RsaKeyPair ███

Insane::Crypto::RsaKeyPair::RsaKeyPair() = default;

Insane::Crypto::RsaKeyPair::RsaKeyPair(const std::string &publicKey, const std::string &privateKey)
{
	this->privateKey = privateKey;
	this->publicKey = publicKey;
}

Insane::Crypto::RsaKeyPair::~RsaKeyPair() = default;

std::string Insane::Crypto::RsaKeyPair::GetPublicKey() const
{
	return publicKey;
}

std::string Insane::Crypto::RsaKeyPair::GetPrivateKey() const
{
	return privateKey;
}

void Insane::Crypto::RsaKeyPair::SetPublicKey(const std::string &publicKey)
{
	this->publicKey = publicKey;
}

void Insane::Crypto::RsaKeyPair::SetPrivateKey(const std::string &privateKey)
{
	this->privateKey = privateKey;
}

String Insane::Crypto::RsaKeyPair::Serialize() const noexcept(false)
{
	try
	{
		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
		writer.StartObject();
		writer.String(u8"PublicKey");
		writer.String(publicKey.c_str(), static_cast<rapidjson::SizeType>(publicKey.length()));
		writer.String(u8"PrivateKey");
		writer.String(privateKey.c_str(), static_cast<rapidjson::SizeType>(privateKey.length()));
		writer.EndObject();
		return std::string(sb.GetString(), sb.GetSize());
	}
	catch (...)
	{
		throw Insane::Exception::CryptoException(u8"Unable to serialize keypair."s);
	}
}

Insane::Crypto::RsaKeyPair Insane::Crypto::RsaKeyPair::Deserialize(String json) noexcept(false)
{
	rapidjson::Document document;
	document.Parse(json.c_str(), json.length());
	if (document.HasParseError())
	{
		throw Insane::Exception::CryptoException(u8"Unable to parse keypair."s);
	}
	RsaKeyPair keypair(document[u8"PublicKey"].GetString(), document[u8"PrivateKey"].GetString());
	return keypair;
}

// ███ RsaManager ███

Insane::Crypto::RsaManager::RsaManager() = default;

Insane::Crypto::RsaManager::~RsaManager() = default;

Insane::Crypto::RsaKeyPair Insane::Crypto::RsaManager::CreateKeyPair(const Size &keySize, const RsaKeyEncoding &encoding, const bool &indent)
{
	USING_INSANE_EXCEPTION;
	USING_INSANE_STR;
	std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
	std::unique_ptr<Botan::RSA_PrivateKey> keyPair = std::make_unique<Botan::RSA_PrivateKey>(*rng, keySize);
	switch (encoding)
	{
	case RsaKeyEncoding::Ber:
	{
		try
		{
			std::string privateKey = Botan::base64_encode(Botan::PKCS8::BER_encode(*keyPair));
			std::string publicKey = Botan::base64_encode(Botan::X509::BER_encode(*keyPair));
			return RsaKeyPair(Strings::Trim(publicKey), Strings::Trim(privateKey));
		}
		catch (...)
		{
			throw Insane::Exception::CryptoException(u8"Unable to generate BER keypair."s);
		}
	}
	case RsaKeyEncoding::Pem:
	{
		try
		{
			std::string privateKey = Botan::PKCS8::PEM_encode(*keyPair);
			std::string publicKey = Botan::X509::PEM_encode(*keyPair);
			return RsaKeyPair(Strings::Trim(publicKey), Strings::Trim(privateKey));
		}
		catch (...)
		{
			throw Insane::Exception::CryptoException(u8"Unable to generate PEM keypair."s);
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

			std::string modulus = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_n()));
			String modulusName = RSA_KEY_MODULUS_NODE_STRING;
			rapidxml::xml_node<> *childNode = doc->allocate_node(rapidxml::node_type::node_element, modulusName.c_str(), modulus.c_str(), 0, modulus.length());
			mainNode->append_node(childNode);

			std::string exponent = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_e()));
			String exponentName = RSA_KEY_EXPONENT_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, exponentName.c_str(), exponent.c_str(), 0, exponent.length());
			mainNode->append_node(childNode);

			std::string publicKey;
			rapidxml::print(std::back_inserter(publicKey), *doc, indent ? 0 : rapidxml::print_no_indenting);

			std::string p = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_p()));
			String pName = RSA_KEY_P_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, pName.c_str(), p.c_str(), 0, p.length());
			mainNode->append_node(childNode);

			std::string q = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_q()));
			String qName = RSA_KEY_Q_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, qName.c_str(), q.c_str(), 0, q.length());
			mainNode->append_node(childNode);

			std::string dp = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d1()));
			String dpName = RSA_KEY_DP_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, dpName.c_str(), dp.c_str(), 0, dp.length());
			mainNode->append_node(childNode);

			std::string dq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d2()));
			String dqName = RSA_KEY_DQ_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, dqName.c_str(), dq.c_str(), 0, dq.length());
			mainNode->append_node(childNode);

			std::string inverseq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_c()));
			String inverseqName = RSA_KEY_INVERSEQ_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, inverseqName.c_str(), inverseq.c_str(), 0, inverseq.length());
			mainNode->append_node(childNode);

			std::string d = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d()));
			String dName = RSA_KEY_D_NODE_STRING;
			childNode = doc->allocate_node(rapidxml::node_type::node_element, dName.c_str(), d.c_str(), 0, d.length());
			mainNode->append_node(childNode);
			std::string privateKey;
			rapidxml::print(std::back_inserter(privateKey), *doc, indent ? 0 : rapidxml::print_no_indenting);
			doc->clear();
			return RsaKeyPair(Strings::Trim(publicKey), Strings::Trim(privateKey));
		}
		catch (...)
		{
			throw Insane::Exception::CryptoException(u8"Unable to serialize xml keypair."s);
		}
	}
	case RsaKeyEncoding::Json:
	{
		try
		{
			std::string modulus = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_n()));
			std::string exponent = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_e()));
			std::string p = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_p()));
			std::string q = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_q()));
			std::string dp = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d1()));
			std::string dq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d2()));
			std::string inverseq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_c()));
			std::string d = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d()));

			rapidjson::StringBuffer sb;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
			if (indent)
			{
				writer.SetIndent(u8'\t', 1);
			}
			else
			{
				writer.SetIndent(u8' ', 0);
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
			return RsaKeyPair(HashManager::RemoveLineBreaks(publicKey), HashManager::RemoveLineBreaks(privateKey));
		}
		catch (...)
		{
			throw Insane::Exception::CryptoException(u8"Unable to serialize json keypair."s);
		}
	}
	default:
		throw CryptoException("Not implemented encoding.");
	}
}

namespace Insane::Crypto
{
	static Insane::Crypto::RsaKeyEncoding GetKeyEncoding(const String &key)
	{
		USING_INSANE_STR;
		USING_INSANE_CORE;
		String rsaKey = Strings::Trim(key);
#ifdef EMSCRIPTEN_PLATFORM
		if (Strings::StartsWith(rsaKey, JSON_RSA_PUBLIC_AND_PRIVATE_KEY_INITIAL_STRING) && Strings::EndsWith(rsaKey, JSON_RSA_PUBLIC_AND_PRIVATE_KEY_FINAL_STRING))
#else
		if (Strings::IsMatch(Strings::ReplaceLastOf(rsaKey, u8"\"", u8"\","), JSON_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING))
#endif
		{
			return RsaKeyEncoding::Json;
		}

#ifdef EMSCRIPTEN_PLATFORM
		if (Strings::StartsWith(rsaKey, XML_RSA_PUBLIC_AND_PRIVATE_KEY_INITIAL_STRING) && Strings::EndsWith(rsaKey, XML_RSA_PUBLIC_AND_PRIVATE_KEY_FINAL_STRING))
#else
		if (Strings::IsMatch(Strings::ReplaceLastOf(rsaKey, u8"\"", u8"\","), XML_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING))
#endif
		{
			return RsaKeyEncoding::Xml;
		}

#ifdef EMSCRIPTEN_PLATFORM
		if ((Strings::StartsWith(rsaKey, PEM_RSA_PUBLIC_KEY_INITIAL_STRING) && Strings::EndsWith(rsaKey, PEM_RSA_PUBLIC_KEY_FINAL_STRING)) || (Strings::StartsWith(rsaKey, PEM_RSA_PRIVATE_KEY_INITIAL_STRING) && Strings::EndsWith(rsaKey, PEM_RSA_PRIVATE_KEY_FINAL_STRING)))
#else
		if (Strings::IsMatch(Strings::ReplaceLastOf(rsaKey, u8"\"", u8"\","), PEM_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING))
#endif
		{
			return RsaKeyEncoding::Pem;
		}

		return RsaKeyEncoding::Ber;
	}

	static std::unique_ptr<Botan::Public_Key> ParsePublicKey(const String &key)
	{
		USING_INSANE_CRYPTO;
		USING_INSANE_EXCEPTION;
		USING_INSANE_STR;
		std::unique_ptr<Botan::BigInt> modulus;
		std::unique_ptr<Botan::BigInt> exponent;
		std::unique_ptr<Botan::Public_Key> pbk;

		try
		{
			String publicKey = Strings::Trim(key);
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
				std::string xml = std::string(publicKey);
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
		USING_INSANE_CRYPTO;
		USING_INSANE_EXCEPTION;
		USING_INSANE_STR;
		std::unique_ptr<Botan::BigInt> modulus;
		std::unique_ptr<Botan::BigInt> exponent;
		std::unique_ptr<Botan::BigInt> P;
		std::unique_ptr<Botan::BigInt> Q;
		std::unique_ptr<Botan::BigInt> D;
		std::unique_ptr<Botan::Private_Key> pvk;
		try
		{
			String privateKey = Strings::Trim(key);
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
				std::string xml = std::string(privateKey);
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

} // namespace Insane::Crypto

String Insane::Crypto::RsaManager::EncryptRaw(const String &data, const String &publicKey) noexcept(false)
{
	USING_INSANE_STR;
	USING_INSANE_EXCEPTION;
	try
	{
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		std::unique_ptr<Botan::Public_Key> pbk = ParsePublicKey(publicKey);
		std::unique_ptr<Botan::PK_Encryptor_EME> enc = std::make_unique<Botan::PK_Encryptor_EME>(*pbk, *rng, RSA_PADDING_ALGORITHM_STRING);
		Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
		std::vector<uint8_t> encrypted = enc->encrypt(dataBytes, *rng);
		return std::string(encrypted.begin(), encrypted.end());
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

String Insane::Crypto::RsaManager::DecryptRaw(const String &data, const String &privateKey) noexcept(false)
{
	USING_INSANE_EXCEPTION;
	try
	{
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		std::unique_ptr<Botan::Private_Key> pvk = ParsePrivateKey(privateKey);
		std::unique_ptr<Botan::PK_Decryptor_EME> dec = std::make_unique<Botan::PK_Decryptor_EME>(*pvk, *rng, RSA_PADDING_ALGORITHM_STRING);
		Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
		Botan::SecureVector<uint8_t> decrypted = dec->decrypt(dataBytes);
		return std::string(decrypted.begin(), decrypted.end());
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

String Insane::Crypto::RsaManager::EncryptToBase64(const String &data, const String &publicKey) noexcept(false)
{
	return HashManager::ToBase64(EncryptRaw(data, publicKey));
}

String Insane::Crypto::RsaManager::DecryptFromBase64(const String &data, const String &privateKey) noexcept(false)
{
	return DecryptRaw(HashManager::FromBase64(data), privateKey);
}
