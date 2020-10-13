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

// ███ HashManager ███
static const String base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

bool DLLCALL is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

Insane::Crypto::HashManager::HashManager() = default;

Insane::Crypto::HashManager::~HashManager() = default;

String Insane::Crypto::HashManager::ToBase64(const String &data, size_t lineBreaks, bool urlEncoded)
{
	// size_t in_len = data.length();
	// String ret;
	// int i = 0;
	// unsigned char char_array_3[3];
	// unsigned char char_array_4[4];
	// int ac = 0;
	// while (in_len--)
	// {
	// 	char_array_3[i++] = static_cast<unsigned char>(data[ac++]);
	// 	if (i == 3)
	// 	{
	// 		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	// 		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	// 		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	// 		char_array_4[3] = char_array_3[2] & 0x3f;

	// 		for (i = 0; (i < 4); i++)
	// 		{
	// 			ret += base64_chars[char_array_4[i]];
	// 		}
	// 		i = 0;
	// 	}
	// }

	// if (i)
	// {
	// 	for (int j = i; j < 3; j++)
	// 	{
	// 		char_array_3[j] = '\0';
	// 	}

	// 	char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	// 	char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	// 	char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	// 	char_array_4[3] = char_array_3[2] & 0x3f;

	// 	for (int j = 0; (j < i + 1); j++)
	// 	{
	// 		ret += base64_chars[char_array_4[j]];
	// 	}

	// 	while ((i++ < 3))
	// 	{
	// 		ret += '=';
	// 	}
	// }
	// if (lineBreaks > 0)
	// {
	// 	ret = InsertLineBreaks(ret, lineBreaks);
	// }
	
	// ret = urlEncoded ? ToUrlEncodedBase64(ret) : ret;
	// return ret;
	String ret = Botan::base64_encode(std::vector<uint8_t>(data.begin(), data.end()));
	if (lineBreaks > 0)
	{
		ret = InsertLineBreaks(ret, lineBreaks);
	}
	
	ret = urlEncoded ? ToUrlEncodedBase64(ret) : ret;
	return ret;
}

String Insane::Crypto::HashManager::FromBase64(const String &base64)
{
	auto result = Botan::base64_decode(RemoveLineBreaks(ToDefaultEncodedBase64(base64)));
	return String(result.begin(), result.end());
	// size_t in_len = result.size();
	// size_t i = 0;
	// size_t in_ = 0;
	// unsigned char char_array_4[4], char_array_3[3];
	// String ret;
	// while (in_len-- && (result[in_] != '=') && is_base64(result[in_]))
	// {
	// 	char_array_4[i++] = result[in_];
	// 	in_++;
	// 	if (i == 4)
	// 	{
	// 		for (i = 0; i < 4; i++)
	// 			char_array_4[i] = static_cast<UChar>(base64_chars.find(char_array_4[i]));

	// 		char_array_3[0] = static_cast<unsigned char>((char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4));
	// 		char_array_3[1] = static_cast<unsigned char>(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
	// 		char_array_3[2] = static_cast<unsigned char>(((char_array_4[2] & 0x3) << 6) + char_array_4[3]);

	// 		for (i = 0; (i < 3); i++)
	// 			ret += char_array_3[i];
	// 		i = 0;
	// 	}
	// }

	// if (i)
	// {
	// 	for (size_t j = 0; j < i; j++)
	// 		char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

	// 	char_array_3[0] = static_cast<unsigned char>((char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4));
	// 	char_array_3[1] = static_cast<unsigned char>(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));

	// 	for (size_t j = 0; (j < i - 1); j++)
	// 		ret += char_array_3[j];
	// }
	// return ret;
}

String Insane::Crypto::HashManager::InsertLineBreaks(const String &base64, size_t lineBreakAppear)
{
	return Str::Strings::InsertRepeat(base64, lineBreakAppear, NEW_LINE_STR);
}

String Insane::Crypto::HashManager::RemoveLineBreaks(const String &base64)
{
	USING_INSANE_STR;
	return Strings::RemoveAll(base64, {CRLF_STRING, LF_STRING, CR_STRING});
	// String str = Str::Strings::RemoveAll(base64, CRLF_STR);
	// str = Str::Strings::RemoveAll(str, LF_STR);
	// str = Str::Strings::RemoveAll(str, CR_STR);
	// return str;
}

String Insane::Crypto::HashManager::ToUrlEncodedBase64(const String &base64)
{
	return Str::Strings::ReplaceAll(base64, {{u8"+", u8"-"}, {u8"/", u8"_"}, {u8"=", u8","}});
}

String Insane::Crypto::HashManager::ToDefaultEncodedBase64(const String &base64)
{
	return Str::Strings::ReplaceAll(base64, {{u8"-", u8"+"}, {u8"_", u8"/"}, {u8",", u8"="}});
}

String Insane::Crypto::HashManager::ToRawHash(const String &data, HashAlgorithm algorithm)
{
	switch (algorithm)
	{
	case HashAlgorithm::MD5:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("MD5"));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::SHA1:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-1"));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::SHA256:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-256"));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::SHA384:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-384"));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::SHA512:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create("SHA-512"));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	default:

		throw;
	}
}

String Insane::Crypto::HashManager::ToBase64Hash(const String &data, HashAlgorithm algorithm, size_t lineBreaks, bool urlEncoded)
{
	return HashManager::ToBase64(ToRawHash(data, algorithm), lineBreaks, urlEncoded);
}

String Insane::Crypto::HashManager::ToRawHmac(const String &data, const String &key, HashAlgorithm algorithm)
{
	int blockSize = 0;
	String secret = key;
	switch (algorithm)
	{
	case HashAlgorithm::MD5:
		[[fallthrough]];
	case HashAlgorithm::SHA1:
		[[fallthrough]];
	case HashAlgorithm::SHA256:
		blockSize = HMAC_64_BYTES_BLOCK_SIZE;
		break;
	case HashAlgorithm::SHA384:
		[[fallthrough]];
	case HashAlgorithm::SHA512:
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

String Insane::Crypto::HashManager::ToBase64Hmac(const String &data, const String &key, HashAlgorithm algorithm, size_t lineBreaks, bool urlEncoded)
{
	return HashManager::ToBase64(ToRawHmac(data, key, algorithm), lineBreaks, urlEncoded);
}

// ███ AesManager ███

Insane::Crypto::AesManager::AesManager() = default;

Insane::Crypto::AesManager::~AesManager() = default;

String Insane::Crypto::AesManager::EncryptRaw(const String &data, const String &key) noexcept(false)
{
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
		throw Insane::Exception::CryptoException();
	}
}

// String Insane::Crypto::AesManager::DecryptRaw(const String &data, const String &key)
// {
// 	try
// 	{
// 		String secretKey = GenerateValidKey(key);
// 		String data1 = data;
// 		String iv = data.substr(data.length() - AES_MAX_IV_LENGHT, AES_MAX_IV_LENGHT);
// 		data1.resize(data.size() - AES_MAX_IV_LENGHT);
// 		std::unique_ptr<Botan::Cipher_Mode> dec = Botan::Cipher_Mode::create("AES-256/CBC/PKCS7", Botan::DECRYPTION);
// 		dec->set_key(Botan::SecureVector<uint8_t>(secretKey.begin(), secretKey.end()));
// 		Botan::secure_vector<uint8_t> dataBytes(data1.begin(), data1.end());
// 		dec->start(Botan::secure_vector<uint8_t>(iv.begin(), iv.end()));
// 		dec->finish(dataBytes);
// 		return std::string(dataBytes.begin(), dataBytes.end());
// 	}
// 	catch (const Botan::Exception &e)
// 	{
// 		throw Insane::Exception::CryptoException();
// 	}
// 	catch (...)
// 	{
// 		throw;
// 	}
// }

// String secretKey = GenerateValidKey(key);
// 		std::unique_ptr<Botan::Cipher_Mode> dec = Botan::Cipher_Mode::create(u8"AES-256/CBC/PKCS7"s, Botan::DECRYPTION);
// 		dec->set_key(Botan::SecureVector<uint8_t>(secretKey.begin(), secretKey.end()));
// 		Botan::secure_vector<uint8_t> dataBytes(data.begin(), data.end() - AES_MAX_IV_LENGHT);
// 		dec->start(Botan::secure_vector<uint8_t>(data.end() - AES_MAX_IV_LENGHT, data.end()));
// 		dec->finish(dataBytes);
// 		return std::string(dataBytes.begin(), dataBytes.end());

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
		throw;
	}
}

String Insane::Crypto::AesManager::EncryptToBase64(const String &data, const String &key, bool urlEncoded) noexcept(false)
{
	return HashManager::ToBase64(EncryptRaw(data, key), false, urlEncoded);
}

String Insane::Crypto::AesManager::DecryptFromBase64(const String &data, const String &key) noexcept(false)
{
	return DecryptRaw(HashManager::FromBase64(data), key);
}

String Insane::Crypto::AesManager::GenerateValidKey(const String &key)
{
	String hash = HashManager::ToRawHash(key, HashAlgorithm::SHA256);
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
		throw Insane::Exception::ExceptionBase();
	}
}

Insane::Crypto::RsaKeyPair Insane::Crypto::RsaKeyPair::Deserialize(String json) noexcept(false)
{
	rapidjson::Document document;
	document.Parse(json.c_str(), json.length());
	if (document.HasParseError())
	{
		throw Insane::Exception::ParseException();
	}
	RsaKeyPair keypair(document[u8"PublicKey"].GetString(), document[u8"PrivateKey"].GetString());
	return keypair;
}

// ███ RsaManager ███

Insane::Crypto::RsaManager::RsaManager() = default;

Insane::Crypto::RsaManager::~RsaManager() = default;

Insane::Crypto::RsaKeyPair Insane::Crypto::RsaManager::CreateKeyPair(const SizeT &keySize, bool keyAsXml, bool indentXml) noexcept(false)
{
	std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
	std::unique_ptr<Botan::RSA_PrivateKey> keyPair = std::make_unique<Botan::RSA_PrivateKey>(*rng, keySize);
	if (keyAsXml)
	{
		std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
		rapidxml::xml_node<> *mainNode = doc->allocate_node(rapidxml::node_type::node_element, RSA_XML_KEY_MAIN_NODE_STR);
		doc->append_node(mainNode);

		std::string modulus = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_n()));
		rapidxml::xml_node<> *childNode = doc->allocate_node(rapidxml::node_type::node_element, RSA_XML_KEY_MODULUS_NODE_STR, modulus.c_str(), 0, modulus.length());
		mainNode->append_node(childNode);

		std::string exponent = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_e()));
		childNode = doc->allocate_node(rapidxml::node_type::node_element, RSA_XML_KEY_EXPONENT_NODE_STR, exponent.c_str(), 0, exponent.length());
		mainNode->append_node(childNode);

		std::string publicKey;
		rapidxml::print(std::back_inserter(publicKey), *doc, 0);

		std::string p = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_p()));
		childNode = doc->allocate_node(rapidxml::node_type::node_element, RSA_XML_KEY_P_NODE_STR, p.c_str(), 0, p.length());
		mainNode->append_node(childNode);

		std::string q = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_q()));
		childNode = doc->allocate_node(rapidxml::node_type::node_element, RSA_XML_KEY_Q_NODE_STR, q.c_str(), 0, q.length());
		mainNode->append_node(childNode);

		std::string dp = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d1()));
		childNode = doc->allocate_node(rapidxml::node_type::node_element, RSA_XML_KEY_DP_NODE_STR, dp.c_str(), 0, dp.length());
		mainNode->append_node(childNode);

		std::string dq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d2()));
		childNode = doc->allocate_node(rapidxml::node_type::node_element, RSA_XML_KEY_DQ_NODE_STR, dq.c_str(), 0, dq.length());
		mainNode->append_node(childNode);

		std::string inverseq = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_c()));
		childNode = doc->allocate_node(rapidxml::node_type::node_element, RSA_XML_KEY_INVERSEQ_NODE_STR, inverseq.c_str(), 0, inverseq.length());
		mainNode->append_node(childNode);

		std::string d = Botan::base64_encode(Botan::BigInt::encode(keyPair->get_d()));
		childNode = doc->allocate_node(rapidxml::node_type::node_element, RSA_XML_KEY_D_NODE_STR, d.c_str(), 0, d.length());
		mainNode->append_node(childNode);
		std::string privateKey;
		rapidxml::print(std::back_inserter(privateKey), *doc, 0);
		doc->clear();
		return RsaKeyPair(publicKey, privateKey);
	}
	else
	{
		std::string privateKey = Botan::base64_encode(Botan::PKCS8::BER_encode(*keyPair));
		std::string publicKey = Botan::base64_encode(Botan::X509::BER_encode(*keyPair));
		return RsaKeyPair(publicKey, privateKey);
	}
}

String Insane::Crypto::RsaManager::EncryptRaw(const String &data, const String &publicKey, bool keyAsXml) noexcept(false)
{
	try
	{
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		if (keyAsXml)
		{
			std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
			std::string xml = std::string(publicKey);
			doc->parse<0>(xml.data());
			std::unique_ptr<Botan::BigInt> modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STR)->first_node(RSA_XML_KEY_MODULUS_NODE_STR)->value()));
			std::unique_ptr<Botan::BigInt> exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STR)->first_node(RSA_XML_KEY_EXPONENT_NODE_STR)->value()));
			std::unique_ptr<Botan::Public_Key> pbk = std::make_unique<Botan::RSA_PublicKey>(*modulus, *exponent);
			std::unique_ptr<Botan::PK_Encryptor_EME> enc = std::make_unique<Botan::PK_Encryptor_EME>(*pbk, *rng, RSA_PADDING_ALGORITHM);
			Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
			std::vector<uint8_t> encrypted = enc->encrypt(dataBytes, *rng);
			return std::string(encrypted.begin(), encrypted.end());
		}
		else
		{
			Botan::SecureVector<uint8_t> keyBytes(Botan::base64_decode(publicKey));
			std::unique_ptr<Botan::Public_Key> pbk(Botan::X509::load_key(std::vector(keyBytes.begin(), keyBytes.end())));
			std::unique_ptr<Botan::PK_Encryptor_EME> enc = std::make_unique<Botan::PK_Encryptor_EME>(*pbk, *rng, RSA_PADDING_ALGORITHM);
			Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
			std::vector<uint8_t> encrypted = enc->encrypt(dataBytes, *rng);
			return std::string(encrypted.begin(), encrypted.end());
		}
	}
	catch (const Botan::Exception &e)
	{
		throw Insane::Exception::CryptoException();
	}
}

String Insane::Crypto::RsaManager::DecryptRaw(const String &data, const String &privateKey, bool keyAsXml) noexcept(false)
{
	try
	{
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		if (keyAsXml)
		{
			std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
			std::string xml = std::string(privateKey);
			doc->parse<0>(xml.data());
			std::unique_ptr<Botan::BigInt> modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STR)->first_node(RSA_XML_KEY_MODULUS_NODE_STR)->value()));
			std::unique_ptr<Botan::BigInt> exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STR)->first_node(RSA_XML_KEY_EXPONENT_NODE_STR)->value()));
			std::unique_ptr<Botan::BigInt> P = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STR)->first_node(RSA_XML_KEY_P_NODE_STR)->value()));
			std::unique_ptr<Botan::BigInt> Q = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STR)->first_node(RSA_XML_KEY_Q_NODE_STR)->value()));
			std::unique_ptr<Botan::BigInt> D = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STR)->first_node(RSA_XML_KEY_D_NODE_STR)->value()));
			std::unique_ptr<Botan::RSA_PrivateKey> pk = std::make_unique<Botan::RSA_PrivateKey>(*P, *Q, *exponent, *D, *modulus);
			std::unique_ptr<Botan::PK_Decryptor_EME> dec = std::make_unique<Botan::PK_Decryptor_EME>(*pk, *rng, RSA_PADDING_ALGORITHM);
			Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
			Botan::SecureVector<uint8_t> decrypted = dec->decrypt(dataBytes);
			return std::string(decrypted.begin(), decrypted.end());
		}
		else
		{
			Botan::SecureVector<uint8_t> keyBytes(Botan::base64_decode(privateKey));
			std::unique_ptr<Botan::DataSource_Memory> source = std::make_unique<Botan::DataSource_Memory>(keyBytes);
			std::unique_ptr<Botan::Private_Key> pk(Botan::PKCS8::load_key(*source));
			std::unique_ptr<Botan::PK_Decryptor_EME> dec = std::make_unique<Botan::PK_Decryptor_EME>(*pk, *rng, RSA_PADDING_ALGORITHM);
			Botan::SecureVector<uint8_t> dataBytes(data.begin(), data.end());
			Botan::SecureVector<uint8_t> decrypted = dec->decrypt(dataBytes);
			return std::string(decrypted.begin(), decrypted.end());
		}
	}
	catch (const Botan::Exception &e)
	{
		throw Insane::Exception::CryptoException(e.what());
	}
}

String Insane::Crypto::RsaManager::EncryptToBase64(const String &data, const String &publicKey, bool keyAsXml, bool urlEncoded) noexcept(false)
{
	return HashManager::ToBase64(EncryptRaw(data, publicKey, keyAsXml), NO_LINE_BREAKS, urlEncoded);
}

String Insane::Crypto::RsaManager::DecryptFromBase64(const String &data, const String &privateKey, bool keyAsXml) noexcept(false)
{
	return DecryptRaw(HashManager::FromBase64(data), privateKey, keyAsXml);
}

//https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range