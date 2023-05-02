#include <stdexcept>
#include <memory>
#include <stdint.h>

#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneException.h>
#include <Insane/InsaneString.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneCore.h>
#include <Insane/InsaneTest.h>

#include <botan/block_cipher.h>
#include <botan/cipher_mode.h>
#include <botan/filters.h>
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
#include <botan/pwdhash.h>
#include <botan/argon2.h>
#include <botan/sym_algo.h>
#include <botan/mode_pad.h>
#include <botan/pipe.h>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <unicode/regex.h>

USING_NS_INSANE_CRYPTO;

// ███ RandomExtensions ███

String InsaneIO::Insane::Cryptography::RandomExtensions::Next(size_t sz)
{
	String result = String(sz, 0);
	std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
	Botan::secure_vector<uint8_t> bytes = (*rng).random_vec(sz);
	return String(bytes.begin(), bytes.end());
}

int InsaneIO::Insane::Cryptography::RandomExtensions::Next(int min, int max)
{
	Botan::AutoSeeded_RNG rng;
	// if (min >= max)
	//             {
	//                 throw new ArgumentException("Min value is greater or equals than Max value.");
	//             }
	//             byte[] intBytes = new byte[4];
	//             RandomNumberGenerator.Fill(intBytes);
	//             return min + Math.Abs(BitConverter.ToInt32(intBytes, 0)) % (max - min + 1);
	// Genera el número aleatorio de tipo uint32_t
	std::random_device device = std::random_device{};
	std::mt19937 mersenneEngine(device());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(mersenneEngine);
}

int InsaneIO::Insane::Cryptography::RandomExtensions::Next()
{
	return Next(INT_MIN, INT_MAX);
}

// ███ AesExtensions ███

class ZeroPadding : public Botan::BlockCipherModePaddingMethod
{
public:
	void add_padding(Botan::secure_vector<uint8_t>& buffer, size_t final_block_bytes, size_t block_size) const override
	{
		const size_t pad_bytes = block_size - final_block_bytes;
		buffer.resize(buffer.size() + pad_bytes, 0x00);
	}

	size_t unpad(const uint8_t block[], size_t len) const override
	{
		size_t i = len;
		while (i > 0 && block[i - 1] == 0x00) {
			i--;
		}
		return i;
	}

	bool valid_blocksize(size_t block_size) const override
	{
		return true;
	}

	std::string name() const override
	{
		return "ZeroPadding";
	}
};

String InsaneIO::Insane::Cryptography::AesExtensions::EncryptAesCbc(const String& data, const String& key, const AesCbcPadding& padding) noexcept(false)
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	try
	{
		ValidateKey(key);
		String secretKey = GenerateNormalizedKey(key);
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		Botan::secure_vector<uint8_t> ivBytes = rng->random_vec(AES_MAX_IV_LENGTH);
		Botan::secure_vector<uint8_t> dataBytes(data.begin(), data.end());
		Botan::secure_vector<uint8_t> keyBytes(secretKey.begin(), secretKey.end());
		size_t finalBlockSize = dataBytes.size() - ((dataBytes.size() / AES_BLOCK_SIZE_LENGTH) * AES_BLOCK_SIZE_LENGTH);
		switch (padding)
		{
		case AesCbcPadding::None:
		{
			break;
		}
		case AesCbcPadding::Zeros:
		{
			ZeroPadding paddingMethod = ZeroPadding();
			paddingMethod.add_padding(dataBytes, finalBlockSize, AES_BLOCK_SIZE_LENGTH);
			break;
		}
		case AesCbcPadding::Pkcs7:
		{
			Botan::PKCS7_Padding paddingMethod = Botan::PKCS7_Padding();
			paddingMethod.add_padding(dataBytes, finalBlockSize, AES_BLOCK_SIZE_LENGTH);
			break;
		}
		case AesCbcPadding::AnsiX923:
		{
			Botan::ANSI_X923_Padding paddingMethod = Botan::ANSI_X923_Padding();
			paddingMethod.add_padding(dataBytes, finalBlockSize, AES_BLOCK_SIZE_LENGTH);
			break;
		}
		default:
			throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, AesCbcPaddingEnumExtensions::ToString(padding));
		}

		std::unique_ptr<Botan::Cipher_Mode> enc = Botan::Cipher_Mode::create(StringExtensions::Join({ __AES_256_ALGORITHM_STRING, __AES_MODE_CBC_STRING, __AES_PADDING_MODE_NONE_STRING }, SLASH_STRING), Botan::ENCRYPTION);
		enc->set_key(keyBytes);
		enc->start(ivBytes);
		enc->finish(dataBytes);
		String result = String(dataBytes.begin(), dataBytes.end());
		result += String(ivBytes.begin(), ivBytes.end());
		return result;
	}
	catch (const NotImplementedException& e)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, e.what(), e.GetErrorCode());
	}
	catch (...)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
}

String InsaneIO::Insane::Cryptography::AesExtensions::DecryptAesCbc(const String& data, const String& key, const AesCbcPadding& padding)
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	try
	{
		ValidateKey(key);
		String secretKey = GenerateNormalizedKey(key);
		Botan::secure_vector<uint8_t> dataBytes(data.begin(), data.end() - AES_MAX_IV_LENGTH);
		Botan::secure_vector<uint8_t> keyBytes(secretKey.begin(), secretKey.end());
		Botan::secure_vector<uint8_t> ivBytes(data.end() - AES_MAX_IV_LENGTH, data.end());

		std::unique_ptr<Botan::Cipher_Mode> dec = Botan::Cipher_Mode::create(StringExtensions::Join({ __AES_256_ALGORITHM_STRING, __AES_MODE_CBC_STRING, __AES_PADDING_MODE_NONE_STRING }, SLASH_STRING), Botan::DECRYPTION);
		dec->set_key(keyBytes);
		dec->start(ivBytes);
		dec->finish(dataBytes);
		size_t newSize = dataBytes.size();
		switch (padding)
		{
		case AesCbcPadding::None:
		{
			break;
		}
		case AesCbcPadding::Zeros:
		{
			ZeroPadding paddingMethod = ZeroPadding();
			newSize = paddingMethod.unpad(dataBytes.data(), dataBytes.size());
			break;
		}
		case AesCbcPadding::Pkcs7:
		{
			Botan::PKCS7_Padding paddingMethod = Botan::PKCS7_Padding();
			newSize = paddingMethod.unpad(dataBytes.data(), dataBytes.size());
			break;
		}
		case AesCbcPadding::AnsiX923:
		{
			Botan::ANSI_X923_Padding paddingMethod = Botan::ANSI_X923_Padding();
			newSize = paddingMethod.unpad(dataBytes.data(), dataBytes.size());
			break;
		}
		default:
			throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, AesCbcPaddingEnumExtensions::ToString(padding));
		}

		dataBytes.resize(newSize);
		return String(dataBytes.begin(), dataBytes.end());
	}
	catch (const NotImplementedException& ex)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, ex.GetErrorMessage());
	}
	catch (...)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
}

String InsaneIO::Insane::Cryptography::AesExtensions::GenerateNormalizedKey(const String& key)
{
	String hash = HashExtensions::ToHash(key, HashAlgorithm::Sha512);
	hash.resize(AES_MAX_KEY_LENGTH);
	return hash;
}

void InsaneIO::Insane::Cryptography::AesExtensions::ValidateKey(const String& key)
{
	USING_NS_INSANE_EXCEPTION;
	if (key.length() < 8)
	{
		throw new ArgumentException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, "Key must be at least 8 bytes.");
	}
}

// ███ RsaKeyPair ███

InsaneIO::Insane::Cryptography::RsaKeyPair::RsaKeyPair(const String& publicKey, const String& privateKey) : IJsonSerialize(EMPTY_STRING), PublicKey(publicKey), PrivateKey(privateKey)
{
}

String InsaneIO::Insane::Cryptography::RsaKeyPair::GetPublicKey() const
{
	return PublicKey;
}

String InsaneIO::Insane::Cryptography::RsaKeyPair::GetPrivateKey() const
{
	return PrivateKey;
}

String InsaneIO::Insane::Cryptography::RsaKeyPair::Serialize(const bool& indent) const noexcept(false)
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CORE;
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

InsaneIO::Insane::Cryptography::RsaKeyPair InsaneIO::Insane::Cryptography::RsaKeyPair::Deserialize(const String& json)
{
	USING_NS_INSANE_EXCEPTION;
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError())
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

// ███ RsaExtensions ███

InsaneIO::Insane::Cryptography::RsaKeyEncoding InsaneIO::Insane::Cryptography::RsaExtensions::GetRsaKeyEncoding(const String& key)
{
	USING_NS_INSANE_STR;
	USING_NS_INSANE_EXCEPTION;
	String rsaKey = StringExtensions::Trim(key);
	if (StringExtensions::IsMatch(rsaKey, RSA_XML_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING))
	{
		return RsaKeyEncoding::Xml;
	}

	if (StringExtensions::IsMatch(rsaKey, RSA_PEM_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING))
	{
		return RsaKeyEncoding::Pem;
	}

	if (StringExtensions::IsMatch(rsaKey, BASE64_VALUE_REGEX_CHAR_STRING))
	{
		return RsaKeyEncoding::Ber;
	}

	throw ParseException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, "Invalid key encoding.");
}

static inline std::unique_ptr<Botan::Public_Key> InternalParsePublicKey(const String& key)
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
		RsaKeyEncoding encoding = RsaExtensions::GetRsaKeyEncoding(publicKey);
		switch (encoding)
		{
		case RsaKeyEncoding::Xml:
		{
			std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
			String xml = String(publicKey);
			doc->parse<0>(xml.data());
			modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_MODULUS_NODE_STRING.c_str())->value()));
			exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_EXPONENT_NODE_STRING.c_str())->value()));
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
		default:
			throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, RsaKeyEncodingEnumExtensions::ToString(encoding, true));
		}
	}
	catch (const ExceptionBase& ex)
	{
		throw ParseException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, ex.GetErrorMessage());
	}
	catch (...)
	{
		throw ParseException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
	return pbk;
}

static inline std::unique_ptr<Botan::Private_Key> InternalParsePrivateKey(const String& key)
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
		RsaKeyEncoding encoding = RsaExtensions::GetRsaKeyEncoding(privateKey);
		switch (encoding)
		{
		case RsaKeyEncoding::Xml:
		{
			std::unique_ptr<rapidxml::xml_document<>> doc = std::make_unique<rapidxml::xml_document<>>();
			String xml = String(privateKey);
			doc->parse<0>(xml.data());
			modulus = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_MODULUS_NODE_STRING.c_str())->value()));
			exponent = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_EXPONENT_NODE_STRING.c_str())->value()));
			P = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_P_NODE_STRING.c_str())->value()));
			Q = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_Q_NODE_STRING.c_str())->value()));
			D = std::make_unique<Botan::BigInt>(Botan::base64_decode(doc->first_node(RSA_XML_KEY_MAIN_NODE_STRING.c_str())->first_node(RSA_XML_KEY_D_NODE_STRING.c_str())->value()));
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
		default:
			throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, RsaKeyEncodingEnumExtensions::ToString(encoding, true));
		}
	}
	catch (const ExceptionBase& ex)
	{
		throw ParseException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, ex.GetErrorMessage());
	}
	catch (...)
	{
		throw ParseException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
	return pvk;
}

bool InsaneIO::Insane::Cryptography::RsaExtensions::ValidateRsaPublicKey(const String& publicKey)
{
	USING_NS_INSANE_STR;
	try
	{
		if (StringExtensions::Trim(publicKey).empty()) return false;
		InternalParsePublicKey(publicKey);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool InsaneIO::Insane::Cryptography::RsaExtensions::ValidateRsaPrivateKey(const String& privateKey)
{
	USING_NS_INSANE_STR;
	try
	{
		if (StringExtensions::Trim(privateKey).empty()) return false;
		InternalParsePrivateKey(privateKey);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

InsaneIO::Insane::Cryptography::RsaKeyPair InsaneIO::Insane::Cryptography::RsaExtensions::CreateRsaKeyPair(const size_t& keySize, const RsaKeyEncoding& encoding)
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	try
	{
		std::unique_ptr<Botan::RandomNumberGenerator> rng = std::make_unique<Botan::AutoSeeded_RNG>();
		std::unique_ptr<Botan::RSA_PrivateKey> keyPair = std::make_unique<Botan::RSA_PrivateKey>(*rng, keySize);
		switch (encoding)
		{
		case RsaKeyEncoding::Ber:
		{
			String privateKey = Botan::base64_encode(Botan::PKCS8::BER_encode(*keyPair));
			String publicKey = Botan::base64_encode(Botan::X509::BER_encode(*keyPair));
			return RsaKeyPair(StringExtensions::Trim(publicKey), StringExtensions::Trim(privateKey));
		}
		case RsaKeyEncoding::Pem:
		{

			String privateKey = Botan::PKCS8::PEM_encode(*keyPair);
			String publicKey = Botan::X509::PEM_encode(*keyPair);
			return RsaKeyPair(StringExtensions::Trim(publicKey), StringExtensions::Trim(privateKey));
		}
		case RsaKeyEncoding::Xml:
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
			rapidxml::xml_node<>* mainNode = doc->allocate_node(rapidxml::node_type::node_element, rsaValueName.c_str());
			doc->append_node(mainNode);

			String modulusName = RSA_XML_KEY_MODULUS_NODE_STRING;
			rapidxml::xml_node<>* childNode = doc->allocate_node(rapidxml::node_type::node_element, modulusName.c_str(), modulus.c_str(), 0, modulus.length());
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
			throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, RsaKeyEncodingEnumExtensions::ToString(encoding, true));
		}
	}
	catch (const NotImplementedException& ex)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, ex.GetErrorMessage());
	}
	catch (...)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
}

String InsaneIO::Insane::Cryptography::RsaExtensions::EncryptRsa(const String& data, const String& publicKey, const RsaPadding& padding)
{
	USING_NS_INSANE_STR;
	USING_NS_INSANE_EXCEPTION;
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
		return String(encrypted.begin(), encrypted.end());
	}
	catch (const Botan::Exception& e)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, e.what(), e.error_code());
	}
	catch (const NotImplementedException& ex)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, ex.GetErrorMessage());
	}
	catch (...)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
}

String InsaneIO::Insane::Cryptography::RsaExtensions::DecryptRsa(const String& data, const String& privateKey, const RsaPadding& padding)
{
	USING_NS_INSANE_EXCEPTION;
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
		return String(decrypted.begin(), decrypted.end());
	}
	catch (const Botan::Exception& e)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, e.what(), e.error_code());
	}
	catch (const NotImplementedException& ex)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, ex.GetErrorMessage());
	}
	catch (...)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
}


// ███ HexEncodingExtensions ███

String InsaneIO::Insane::Cryptography::HexEncodingExtensions::FromHex(const String& data)
{
	auto decoded = Botan::hex_decode(data);
	return String(decoded.begin(), decoded.end());
}

String InsaneIO::Insane::Cryptography::HexEncodingExtensions::ToHex(const String& data, const bool& toUpper)
{
	return Botan::hex_encode(std::vector<uint8_t>(data.begin(), data.end()), toUpper);
}

// ███ Base32EncodingExtensions ███

String InsaneIO::Insane::Cryptography::Base32EncodingExtensions::FromBase32(const String& data)
{
	USING_NS_INSANE_STR;
	auto decoded = Botan::base32_decode(StringExtensions::ToUpper(data));
	return String(decoded.begin(), decoded.end());
}

String InsaneIO::Insane::Cryptography::Base32EncodingExtensions::ToBase32(const String& data, const bool& removePadding, const bool& toLower)
{
	USING_NS_INSANE_STR;
	String encoded = Botan::base32_encode(std::vector<uint8_t>(data.begin(), data.end()));
	encoded = removePadding ? StringExtensions::Replace(encoded, EQUAL_SIGN_STRING, EMPTY_STRING) : encoded;
	return toLower ? StringExtensions::ToLower(encoded) : encoded;
}

// ███ Base64EncodingExtensions ███

String InsaneIO::Insane::Cryptography::Base64EncodingExtensions::ToBase64(const String& data, const size_t& lineBreaksLength, const bool& removePadding)
{
	USING_NS_INSANE_STR;
	String ret = Botan::base64_encode(std::vector<uint8_t>(data.begin(), data.end()));
	if (lineBreaksLength > 0)
	{
		ret = StringExtensions::InsertRepeat(ret, lineBreaksLength, NEW_LINE_STRING);
	}
	return removePadding ? StringExtensions::Remove(ret, EQUAL_SIGN_STRING) : ret;
}

String InsaneIO::Insane::Cryptography::Base64EncodingExtensions::FromBase64(const String& data)
{
	USING_NS_INSANE_STR;
	USING_NS_INSANE_CORE;
	String base64 = data;
	base64 = StringExtensions::Replace(base64, { {URL_ENCODED_PLUS_SIGN_STRING, PLUS_SIGN_STRING}, {URL_ENCODED_SLASH_STRING, SLASH_STRING}, {URL_ENCODED_EQUAL_SIGN_STRING, EQUAL_SIGN_STRING}, {MINUS_SIGN_STRING, PLUS_SIGN_STRING}, {UNDERSCORE_STRING, SLASH_STRING} });
	base64 = StringExtensions::RemoveBlankSpaces(base64);
	size_t modulo = base64.length() % 4;
	base64 = StringExtensions::PadRight(base64, base64.length() + (modulo > 0 ? 4 - modulo : 0), EQUAL_SIGN_STRING);
	Botan::secure_vector<uint8_t> result = Botan::base64_decode(base64);
	return String(result.begin(), result.end());
}

String InsaneIO::Insane::Cryptography::Base64EncodingExtensions::ToUrlSafeBase64(const String& data)
{
	USING_NS_INSANE_STR;
	return StringExtensions::Replace(ToBase64(data), { {PLUS_SIGN_STRING, MINUS_SIGN_STRING}, {SLASH_STRING, UNDERSCORE_STRING}, {EQUAL_SIGN_STRING, EMPTY_STRING} });
}

String InsaneIO::Insane::Cryptography::Base64EncodingExtensions::ToFilenameSafeBase64(const String& data)
{
	USING_NS_INSANE_STR;
	return ToUrlSafeBase64(data);
}

String InsaneIO::Insane::Cryptography::Base64EncodingExtensions::ToUrlEncodedBase64(const String& data)
{
	USING_NS_INSANE_STR;
	return StringExtensions::Replace(ToBase64(data), { {PLUS_SIGN_STRING, URL_ENCODED_PLUS_SIGN_STRING}, {SLASH_STRING, URL_ENCODED_SLASH_STRING}, {EQUAL_SIGN_STRING, URL_ENCODED_EQUAL_SIGN_STRING} });
}

String InsaneIO::Insane::Cryptography::Base64EncodingExtensions::Base64ToUrlSafeBase64(const String& base64)
{
	return ToUrlSafeBase64(FromBase64(base64));
}

String InsaneIO::Insane::Cryptography::Base64EncodingExtensions::Base64ToFilenameSafeBase64(const String& base64)
{
	return ToFilenameSafeBase64(FromBase64(base64));
}

String InsaneIO::Insane::Cryptography::Base64EncodingExtensions::Base64ToUrlEncodedBase64(const String& base64)
{
	return ToUrlEncodedBase64(FromBase64(base64));
}

// ███ CryptoTests ███
void InsaneIO::Insane::Cryptography::CryptoTests::HexEncodingExtensionsTests(const bool& showValues)
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_TEST;
	String testTytes = { (char)0xff, 0xa, 1, 0x22 };
	String hexStringUppercase = "FF0A0122";
	String hexStringLowercase = "ff0a0122";

	String data = testTytes;
	String result = HexEncodingExtensions::ToHex(data);
	String expected = hexStringLowercase;
	TestExtensions::Equals(NAMEOF(HexEncodingExtensions) + "- Encode - 1", expected, result, showValues);

	data = testTytes;
	result = HexEncodingExtensions::ToHex(data, true);
	expected = hexStringUppercase;
	TestExtensions::Equals(NAMEOF(HexEncodingExtensions) + "- Encode - 2", expected, result, showValues);

	data = hexStringUppercase;
	result = HexEncodingExtensions::FromHex(data);
	expected = testTytes;
	TestExtensions::Equals(NAMEOF(HexEncodingExtensions) + "- Decode - 1", expected, result, showValues);

	data = hexStringLowercase;
	result = HexEncodingExtensions::FromHex(data);
	expected = testTytes;
	TestExtensions::Equals(NAMEOF(HexEncodingExtensions) + "- Decode - 2", expected, result, showValues);
}

void InsaneIO::Insane::Cryptography::CryptoTests::Base32EncodingExtensionsTests(const bool& showValues)
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_TEST;

	String TestString = "helloworld";
	String TestString2 = "A";
	String UpperBase32Result = "NBSWY3DPO5XXE3DE";
	String LowerBase32Result = "nbswy3dpo5xxe3de";
	String UpperBase32Result2 = "IE======";
	String LowerBase32Result2 = "ie======";
	String UpperBase32Result2NoPadding = "IE";
	String LowerBase32Result2NoPadding = "ie";

	String data = TestString2;
	String result = Base32EncodingExtensions::ToBase32(data);
	String expected = UpperBase32Result2;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + "- Encode - 1", expected, result, showValues);

	data = TestString;
	result = Base32EncodingExtensions::ToBase32(data);
	expected = UpperBase32Result;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 2", expected, result, showValues);

	data = TestString;
	result = Base32EncodingExtensions::ToBase32(data, false, true);
	expected = LowerBase32Result;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 3", expected, result, showValues);

	data = TestString2;
	result = Base32EncodingExtensions::ToBase32(data, false);
	expected = UpperBase32Result2;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 4", expected, result, showValues);

	data = TestString2;
	result = Base32EncodingExtensions::ToBase32(data, false, true);
	expected = LowerBase32Result2;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 5", expected, result, showValues);

	data = TestString2;
	result = Base32EncodingExtensions::ToBase32(data, true, false);
	expected = UpperBase32Result2NoPadding;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 6", expected, result, showValues);

	data = TestString2;
	result = Base32EncodingExtensions::ToBase32(data, true, true);
	expected = LowerBase32Result2NoPadding;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Encode - 7", expected, result, showValues);

	data = UpperBase32Result;
	result = Base32EncodingExtensions::FromBase32(data);
	expected = TestString;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 1", expected, result, showValues);

	data = LowerBase32Result;
	result = Base32EncodingExtensions::FromBase32(data);
	expected = TestString;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 2", expected, result, showValues);

	data = UpperBase32Result2;
	result = Base32EncodingExtensions::FromBase32(data);
	expected = TestString2;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 3", expected, result, showValues);

	data = LowerBase32Result2;
	result = Base32EncodingExtensions::FromBase32(data);
	expected = TestString2;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 4", expected, result, showValues);

	data = UpperBase32Result2NoPadding;
	result = Base32EncodingExtensions::FromBase32(data);
	expected = TestString2;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 5", expected, result, showValues);

	data = LowerBase32Result2NoPadding;
	result = Base32EncodingExtensions::FromBase32(data);
	expected = TestString2;
	TestExtensions::Equals(NAMEOF(Base32EncodingExtensions) + " - Decode - 6", expected, result, showValues);
}

void InsaneIO::Insane::Cryptography::CryptoTests::Base64EncodingExtensionsTests(const bool& showValues)
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_TEST;
	auto testBytes = { (char)0x30, (char)0x82, (char)0x02, (char)0x22, (char)0x30, (char)0x0d, (char)0x06, (char)0x09, (char)0x2a, (char)0x86, (char)0x48, (char)0x86, (char)0xf7, (char)0x0d, (char)0x01, (char)0x01, (char)0x01, (char)0x05, (char)0x00, (char)0x03, (char)0x82, (char)0x02, (char)0x0f, (char)0x00, (char)0x30, (char)0x82, (char)0x02, (char)0x0a, (char)0x02, (char)0x82, (char)0x02, (char)0x01, (char)0x00, (char)0xf2, (char)0xe8, (char)0xe5, (char)0x81, (char)0x32, (char)0x36, (char)0xb8, (char)0xb6, (char)0x3f, (char)0xb5, (char)0xbe, (char)0x76, (char)0x65, (char)0x65, (char)0xd1, (char)0x8f, (char)0x2d, (char)0xc4, (char)0xc5, (char)0xa1, (char)0x91, (char)0x3b, (char)0x8b, (char)0xdc, (char)0x8b, (char)0xf6, (char)0x4f, (char)0x42, (char)0x64, (char)0xd1, (char)0xea, (char)0xdc, (char)0x75, (char)0x6c, (char)0x83, (char)0x0b, (char)0x81, (char)0x1f, (char)0x57, (char)0xeb, (char)0xac, (char)0xe5, (char)0xd0, (char)0x5c, (char)0x6b, (char)0x5f, (char)0x37, (char)0xa8, (char)0x53, (char)0x1c, (char)0x65, (char)0x6b, (char)0x75, (char)0x5e, (char)0xbc, (char)0xd3, (char)0x59, (char)0xd2, (char)0x54, (char)0x17, (char)0xf7, (char)0x69, (char)0x4d, (char)0x23, (char)0x92, (char)0x7e, (char)0x78, (char)0x47, (char)0xf1, (char)0x06, (char)0x04, (char)0x5b, (char)0x55, (char)0x00, (char)0xb1, (char)0xaa, (char)0x82, (char)0x70, (char)0x70, (char)0xc0, (char)0xff, (char)0x3c, (char)0x29, (char)0x4a, (char)0x2f, (char)0xc3, (char)0xff, (char)0x56, (char)0x60, (char)0x4a, (char)0x22, (char)0x12, (char)0xfe, (char)0x10, (char)0xa4, (char)0xe1, (char)0xeb, (char)0x9d, (char)0x82, (char)0xb3, (char)0x76, (char)0x1c, (char)0xa0, (char)0x18, (char)0x4c, (char)0xca, (char)0xcd, (char)0x68, (char)0x40, (char)0x2e, (char)0x6a, (char)0x21, (char)0x2a, (char)0x7b, (char)0x7b, (char)0xc6, (char)0x0b, (char)0x85, (char)0x14, (char)0x19, (char)0x03, (char)0x40, (char)0xe9, (char)0x78, (char)0x54, (char)0xfe, (char)0x97, (char)0xf4, (char)0xe8, (char)0x39, (char)0x45, (char)0x06, (char)0x76, (char)0x8e, (char)0x5e, (char)0x0e, (char)0xdb, (char)0x62, (char)0x41, (char)0x60, (char)0x2b, (char)0xfb, (char)0x1e, (char)0x1a, (char)0x65, (char)0x3a, (char)0x25, (char)0x48, (char)0xba, (char)0xe6, (char)0x73, (char)0x8f, (char)0x35, (char)0xf0, (char)0xfd, (char)0x99, (char)0xe4, (char)0x1d, (char)0xe9, (char)0xbf, (char)0x67, (char)0x8b, (char)0xf4, (char)0x1d, (char)0xfa, (char)0xfa, (char)0x58, (char)0x8e, (char)0xe7, (char)0x1b, (char)0x7b, (char)0xb5, (char)0x7d, (char)0x74, (char)0x90, (char)0x26, (char)0x41, (char)0x88, (char)0xbd, (char)0x4d, (char)0x20, (char)0x69, (char)0x4b, (char)0x4c, (char)0x8a, (char)0xef, (char)0x47, (char)0x87, (char)0xc1, (char)0xf3, (char)0x5b, (char)0x42, (char)0x79, (char)0x04, (char)0xd7, (char)0x9d, (char)0x42, (char)0xa7, (char)0xdf, (char)0xca, (char)0x0d, (char)0xf4, (char)0x19, (char)0x4a, (char)0x8d, (char)0x7c, (char)0x93, (char)0x3f, (char)0x1a, (char)0xa5, (char)0x39, (char)0xef, (char)0xcd, (char)0x6d, (char)0xe5, (char)0x0a, (char)0xe5, (char)0xf0, (char)0x41, (char)0x16, (char)0x96, (char)0x58, (char)0x14, (char)0x99, (char)0x77, (char)0xdc, (char)0x69, (char)0x27, (char)0xc7, (char)0xa6, (char)0x11, (char)0xb4, (char)0xd3, (char)0xa2, (char)0x17, (char)0x23, (char)0x50, (char)0xa0, (char)0xbd, (char)0x06, (char)0x7d, (char)0x5a, (char)0x72, (char)0xa0, (char)0xb1, (char)0xed, (char)0x48, (char)0xd1, (char)0x42, (char)0xfc, (char)0x66, (char)0x3e, (char)0x4a, (char)0x22, (char)0x69, (char)0xac, (char)0xe4, (char)0xee, (char)0x82, (char)0xbc, (char)0x48, (char)0x83, (char)0x81, (char)0x34, (char)0x6e, (char)0x29, (char)0x4b, (char)0x64, (char)0x71, (char)0x37, (char)0x25, (char)0x13, (char)0x28, (char)0x52, (char)0x71, (char)0x5b, (char)0xd5, (char)0x95, (char)0x20, (char)0xa5, (char)0xb4, (char)0x66, (char)0xa7, (char)0x9e, (char)0x06, (char)0x5f, (char)0x2d, (char)0x8e, (char)0x78, (char)0xf5, (char)0x37, (char)0xcf, (char)0xed, (char)0x65, (char)0x84, (char)0xdf, (char)0xda, (char)0x78, (char)0x27, (char)0xa9, (char)0x09, (char)0xaa, (char)0x70, (char)0x73, (char)0x5a, (char)0xc6, (char)0xa9, (char)0xba, (char)0xb7, (char)0xce, (char)0x38, (char)0x2c, (char)0x28, (char)0x4b, (char)0x3e, (char)0xae, (char)0x11, (char)0x3c, (char)0xed, (char)0x94, (char)0xd9, (char)0x2a, (char)0x26, (char)0xd2, (char)0xbc, (char)0xa5, (char)0x19, (char)0x7c, (char)0x3a, (char)0x98, (char)0x0a, (char)0x51, (char)0xdb, (char)0x14, (char)0x99, (char)0xd8, (char)0x4e, (char)0xc3, (char)0x5d, (char)0x0a, (char)0xc9, (char)0x93, (char)0xa9, (char)0xce, (char)0xb0, (char)0x12, (char)0x62, (char)0x6b, (char)0x6b, (char)0x48, (char)0x42, (char)0x42, (char)0x04, (char)0x95, (char)0x29, (char)0x77, (char)0x49, (char)0xaa, (char)0x88, (char)0x2a, (char)0x94, (char)0xcd, (char)0x55, (char)0x7d, (char)0xb6, (char)0xcb, (char)0xb1, (char)0x1e, (char)0x93, (char)0xa9, (char)0xa2, (char)0xba, (char)0x73, (char)0xff, (char)0x2e, (char)0xa6, (char)0xff, (char)0xd6, (char)0x14, (char)0x65, (char)0x3b, (char)0x8c, (char)0x7d, (char)0x0b, (char)0xa7, (char)0xda, (char)0xbd, (char)0x50, (char)0x2c, (char)0x1d, (char)0x2e, (char)0xf1, (char)0xd9, (char)0xf5, (char)0x8a, (char)0x08, (char)0xe9, (char)0x54, (char)0x7d, (char)0x4a, (char)0x25, (char)0xf5, (char)0xb7, (char)0x53, (char)0xd8, (char)0x3f, (char)0xad, (char)0x98, (char)0x5f, (char)0xea, (char)0xa9, (char)0xd5, (char)0x3d, (char)0x13, (char)0x7d, (char)0x26, (char)0x5d, (char)0xab, (char)0x0e, (char)0xa6, (char)0xcd, (char)0xe7, (char)0xc1, (char)0x81, (char)0x0f, (char)0x12, (char)0x8c, (char)0x59, (char)0x77, (char)0xa9, (char)0x67, (char)0xa4, (char)0x37, (char)0xf3, (char)0x8e, (char)0xdf, (char)0xe5, (char)0x5c, (char)0x0c, (char)0x65, (char)0x07, (char)0x93, (char)0xcd, (char)0xb0, (char)0xeb, (char)0x19, (char)0x89, (char)0x6f, (char)0x81, (char)0x90, (char)0x9a, (char)0xf4, (char)0x99, (char)0xb8, (char)0x33, (char)0x35, (char)0xdb, (char)0x40, (char)0x8e, (char)0x85, (char)0x53, (char)0x26, (char)0x4a, (char)0xe9, (char)0x8c, (char)0x5a, (char)0x5d, (char)0x68, (char)0xd5, (char)0x4e, (char)0xff, (char)0x21, (char)0x77, (char)0xb9, (char)0xcb, (char)0xc1, (char)0xaf, (char)0x69, (char)0x69, (char)0x10, (char)0x56, (char)0x6d, (char)0x9e, (char)0xbd, (char)0xe4, (char)0xa4, (char)0x2b, (char)0xd9, (char)0xf9, (char)0x65, (char)0x63, (char)0xb5, (char)0x00, (char)0x48, (char)0xb0, (char)0x04, (char)0xca, (char)0x98, (char)0x10, (char)0x8e, (char)0x2a, (char)0x4f, (char)0x18, (char)0x47, (char)0xef, (char)0x5e, (char)0x26, (char)0x07, (char)0x72, (char)0xf9, (char)0xbe, (char)0x25, (char)0x02, (char)0x03, (char)0x01, (char)0x00, (char)0x01 };
	String str = String(testBytes.begin(), testBytes.end());

	String resultWith0Pad = "QUFB";
	String resultWith1Pad = "QUE=";
	String resultWith2Pad = "QQ==";

	String resultWith0PadRemoved = "QUFB";
	String resultWith1PadRemoved = "QUE";
	String resultWith2PadRemoved = "QQ";

	String inputFor0Pad = "AAA";
	String inputFor1Pad = "AA";
	String inputFor2Pad = "A";

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

	String TestBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXRjy3ExaGRO4vci/ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J+eEfxBgRbVQCxqoJwcMD/PClKL8P/VmBKIhL+EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT+l/ToOUUGdo5eDttiQWAr+x4aZTolSLrmc4818P2Z5B3pv2eL9B36+liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe/NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC/GY+SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z+1lhN/aeCepCapwc1rGqbq3zjgsKEs+rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c/8upv/WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX+qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN/OO3+VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7/IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ==)";
	String TestBase64StringNoPadding = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY/tb52ZWXRjy3ExaGRO4vci/ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J+eEfxBgRbVQCxqoJwcMD/PClKL8P/VmBKIhL+EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT+l/ToOUUGdo5eDttiQWAr+x4aZTolSLrmc4818P2Z5B3pv2eL9B36+liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe/NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC/GY+SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z+1lhN/aeCepCapwc1rGqbq3zjgsKEs+rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c/8upv/WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX+qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN/OO3+VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7/IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ)";
	String TestUrlSafeBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY_tb52ZWXRjy3ExaGRO4vci_ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J-eEfxBgRbVQCxqoJwcMD_PClKL8P_VmBKIhL-EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT-l_ToOUUGdo5eDttiQWAr-x4aZTolSLrmc4818P2Z5B3pv2eL9B36-liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe_NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC_GY-SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z-1lhN_aeCepCapwc1rGqbq3zjgsKEs-rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c_8upv_WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX-qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN_OO3-VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7_IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ)";
	String TestFileNameSafeBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY_tb52ZWXRjy3ExaGRO4vci_ZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J-eEfxBgRbVQCxqoJwcMD_PClKL8P_VmBKIhL-EKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT-l_ToOUUGdo5eDttiQWAr-x4aZTolSLrmc4818P2Z5B3pv2eL9B36-liO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe_NbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC_GY-SiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z-1lhN_aeCepCapwc1rGqbq3zjgsKEs-rhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c_8upv_WFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX-qp1T0TfSZdqw6mzefBgQ8SjFl3qWekN_OO3-VcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7_IXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ)";
	String TestUrlEncodedBase64String = R"(MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA8ujlgTI2uLY%2Ftb52ZWXRjy3ExaGRO4vci%2FZPQmTR6tx1bIMLgR9X66zl0FxrXzeoUxxla3VevNNZ0lQX92lNI5J%2BeEfxBgRbVQCxqoJwcMD%2FPClKL8P%2FVmBKIhL%2BEKTh652Cs3YcoBhMys1oQC5qISp7e8YLhRQZA0DpeFT%2Bl%2FToOUUGdo5eDttiQWAr%2Bx4aZTolSLrmc4818P2Z5B3pv2eL9B36%2BliO5xt7tX10kCZBiL1NIGlLTIrvR4fB81tCeQTXnUKn38oN9BlKjXyTPxqlOe%2FNbeUK5fBBFpZYFJl33Gknx6YRtNOiFyNQoL0GfVpyoLHtSNFC%2FGY%2BSiJprOTugrxIg4E0bilLZHE3JRMoUnFb1ZUgpbRmp54GXy2OePU3z%2B1lhN%2FaeCepCapwc1rGqbq3zjgsKEs%2BrhE87ZTZKibSvKUZfDqYClHbFJnYTsNdCsmTqc6wEmJra0hCQgSVKXdJqogqlM1VfbbLsR6TqaK6c%2F8upv%2FWFGU7jH0Lp9q9UCwdLvHZ9YoI6VR9SiX1t1PYP62YX%2Bqp1T0TfSZdqw6mzefBgQ8SjFl3qWekN%2FOO3%2BVcDGUHk82w6xmJb4GQmvSZuDM120COhVMmSumMWl1o1U7%2FIXe5y8GvaWkQVm2eveSkK9n5ZWO1AEiwBMqYEI4qTxhH714mB3L5viUCAwEAAQ%3D%3D)";

	String data = inputFor2Pad;
	String expected = resultWith2Pad;
	String result = Base64EncodingExtensions::ToBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 1", expected, result, showValues);

	data = inputFor1Pad;
	expected = resultWith1Pad;
	result = Base64EncodingExtensions::ToBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 2", expected, result, showValues);

	data = inputFor0Pad;
	expected = resultWith0Pad;
	result = Base64EncodingExtensions::ToBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 3", expected, result, showValues);

	data = inputFor2Pad;
	expected = resultWith2PadRemoved;
	result = Base64EncodingExtensions::ToBase64(data, 0, true);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 4", expected, result, showValues);

	data = inputFor1Pad;
	expected = resultWith1PadRemoved;
	result = Base64EncodingExtensions::ToBase64(data, 0, true);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 5", expected, result, showValues);

	data = inputFor0Pad;
	expected = resultWith0PadRemoved;
	result = Base64EncodingExtensions::ToBase64(data, 0, true);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 6", expected, result, showValues);

	data = str;
	expected = TestMimeBase64String;
	result = Base64EncodingExtensions::ToBase64(data, MIME_LINE_BREAKS_LENGTH);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 7", expected, result, showValues);

	data = str;
	expected = TestPemBase64String;
	result = Base64EncodingExtensions::ToBase64(data, PEM_LINE_BREAKS_LENGTH);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 8", expected, result, showValues);

	data = str;
	expected = TestBase64String;
	result = Base64EncodingExtensions::ToBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 9", expected, result, showValues);

	data = str;
	expected = TestBase64StringNoPadding;
	result = Base64EncodingExtensions::ToBase64(data, 0, true);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 10", expected, result, showValues);

	data = str;
	expected = TestUrlSafeBase64String;
	result = Base64EncodingExtensions::ToUrlSafeBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 11", expected, result, showValues);

	data = str;
	expected = TestFileNameSafeBase64String;
	result = Base64EncodingExtensions::ToFilenameSafeBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 12", expected, result, showValues);

	data = str;
	expected = TestUrlEncodedBase64String;
	result = Base64EncodingExtensions::ToUrlEncodedBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 13", expected, result, showValues);

	data = TestBase64String;
	expected = TestUrlSafeBase64String;
	result = Base64EncodingExtensions::Base64ToUrlSafeBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 14", expected, result, showValues);

	data = TestBase64String;
	expected = TestFileNameSafeBase64String;
	result = Base64EncodingExtensions::Base64ToFilenameSafeBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 15", expected, result, showValues);

	data = TestBase64String;
	expected = TestUrlEncodedBase64String;
	result = Base64EncodingExtensions::Base64ToUrlEncodedBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Encode - 16", expected, result, showValues);

	data = resultWith2Pad;
	expected = inputFor2Pad;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 1", expected, result, showValues);

	data = resultWith1Pad;
	expected = inputFor1Pad;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 2", expected, result, showValues);

	data = resultWith0Pad;
	expected = inputFor0Pad;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 3", expected, result, showValues);

	data = resultWith2PadRemoved;
	expected = inputFor2Pad;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 4", expected, result, showValues);

	data = resultWith1PadRemoved;
	expected = inputFor1Pad;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 5", expected, result, showValues);

	data = resultWith0PadRemoved;
	expected = inputFor0Pad;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 6", expected, result, showValues);

	data = TestMimeBase64String;
	expected = str;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 7", expected, result, showValues);

	data = TestPemBase64String;
	expected = str;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 8", expected, result, showValues);

	data = TestBase64String;
	expected = str;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 9", expected, result, showValues);

	data = TestBase64StringNoPadding;
	expected = str;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 10", expected, result, showValues);

	data = TestUrlSafeBase64String;
	expected = str;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 11", expected, result, showValues);

	data = TestFileNameSafeBase64String;
	expected = str;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 12", expected, result, showValues);

	data = TestUrlEncodedBase64String;
	expected = str;
	result = Base64EncodingExtensions::FromBase64(data);
	TestExtensions::Equals(NAMEOF(Base64EncodingExtensions) + " - Decode  - 13", expected, result, showValues);
}

// ███ HashExtensions ███

String InsaneIO::Insane::Cryptography::HashExtensions::ToHash(const String& data, const HashAlgorithm& algorithm)
{
	USING_NS_INSANE_EXCEPTION;
	switch (algorithm)
	{
	case HashAlgorithm::Md5:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(MD5_ALGORITHM_NAME_STRING));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::Sha1:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(SHA1_ALGORITHM_NAME_STRING));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::Sha256:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(SHA256_ALGORITHM_NAME_STRING));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::Sha384:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(SHA384_ALGORITHM_NAME_STRING));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	case HashAlgorithm::Sha512:
	{
		std::unique_ptr<Botan::HashFunction> hash(Botan::HashFunction::create(SHA512_ALGORITHM_NAME_STRING));
		hash->update(data);
		Botan::SecureVector<uint8_t> result = hash->final();
		return String(result.begin(), result.end());
	}
	default:
		throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, HashAlgorithmEnumExtensions::ToString(algorithm));
	}
}

String InsaneIO::Insane::Cryptography::HashExtensions::ToHmac(const String& data, const String& key, const HashAlgorithm& algorithm)
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_EXCEPTION;
	size_t blockSize = 0;
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
	default:
		throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, HashAlgorithmEnumExtensions::ToString(algorithm));
	}

	if (secret.length() > blockSize)
	{
		secret = ToHash(secret, algorithm);
	}

	if (secret.length() < blockSize)
	{
		secret.append(String((size_t)(blockSize - secret.length()), static_cast<char>(0)));
	}
	String outerKeyPadding = String(blockSize, HMAC_OUTER_PADDING);
	String innerKeyPadding = String(blockSize, HMAC_INNER_PADDING);
	for (size_t i = 0; i < blockSize; i++)
	{
		innerKeyPadding[i] = (SignedChar)(secret[i] ^ innerKeyPadding[i]);
		outerKeyPadding[i] = (SignedChar)(secret[i] ^ outerKeyPadding[i]);
	}
	innerKeyPadding.append(data);
	String ret = ToHash(innerKeyPadding, algorithm);
	outerKeyPadding.append(ret);
	ret = ToHash(outerKeyPadding, algorithm);
	return ret;
}

String InsaneIO::Insane::Cryptography::HashExtensions::ToScrypt(const String& data, const String& salt, const size_t& iterations, const size_t& blockSize, const size_t& parallelism, const size_t& derivedKeyLength)
{
	USING_NS_INSANE_EXCEPTION;
	try
	{
		std::unique_ptr<Botan::PasswordHashFamily> family = Botan::PasswordHashFamily::create("Scrypt");
		std::unique_ptr<Botan::PasswordHash> hash = family->from_params(iterations, blockSize, parallelism);
		std::vector<uint8_t> out(derivedKeyLength);
		hash->derive_key(out.data(), out.size(), data.data(), data.size(), reinterpret_cast<const uint8_t*>(salt.data()), salt.size());
		return String(out.begin(), out.end());
	}
	catch (...)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
}

String InsaneIO::Insane::Cryptography::HashExtensions::ToArgon2(const String& data, const String& salt, const size_t& iterations, const size_t& memorySizeKiB, const size_t& parallelism, const Argon2Variant& variant, const size_t& derivedKeyLength)
{
	USING_NS_INSANE_EXCEPTION;
	try
	{
		std::unique_ptr<Botan::PasswordHashFamily> family = Botan::PasswordHashFamily::create(Argon2VariantEnumExtensions::ToString(variant));
		std::unique_ptr<Botan::PasswordHash> hash = family->from_params(memorySizeKiB, iterations, parallelism);
		std::vector<uint8_t> out(derivedKeyLength);
		hash->derive_key(out.data(), out.size(), data.data(), data.size(), reinterpret_cast<const uint8_t*>(salt.data()), salt.size());
		return String(out.begin(), out.end());
	}
	catch (...)
	{
		throw CryptoException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
}

// ███ IBaseSerialize ███

InsaneIO::Insane::Cryptography::IBaseSerialize::IBaseSerialize(String name) : _Name(name)
{
}

String InsaneIO::Insane::Cryptography::IBaseSerialize::GetName() const
{
	return _Name;
}

// ███ IEncoder ███

InsaneIO::Insane::Cryptography::IEncoder::IEncoder(String name)
	: IJsonSerialize(name)
{
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::IEncoder::Deserialize(const String& json, const std::function<std::unique_ptr<IEncoder>(const String&)>& resolver = nullptr)
{
	return IJsonSerialize::Deserialize(json, resolver);
}

const std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::IEncoder::DefaultInstance()
{
	USING_NS_INSANE_EXCEPTION;
	throw AbstractImplementationException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
}

std::function<std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder>(const String&)> InsaneIO::Insane::Cryptography::IEncoder::DefaultDeserializeResolver()
{
	return IJsonSerialize::DefaultDeserializeResolver();
}

// ███ HexEncoder ███

InsaneIO::Insane::Cryptography::HexEncoder::HexEncoder(const bool& toUpper)
	: IEncoder(HEX_ENCODER_NAME_STRING), ToUpper(toUpper)
{
}

bool InsaneIO::Insane::Cryptography::HexEncoder::GetToUpper() const
{
	return ToUpper;
}

String InsaneIO::Insane::Cryptography::HexEncoder::Encode(const String& data) const
{
	return HexEncodingExtensions::ToHex(data, ToUpper);
}

String InsaneIO::Insane::Cryptography::HexEncoder::Decode(const String& data) const
{
	return HexEncodingExtensions::FromHex(data);
}

String InsaneIO::Insane::Cryptography::HexEncoder::Serialize(const bool& indent) const
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CORE;
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


std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::HexEncoder::DefaultInstance()
{
	HexEncoder* instance = dynamic_cast<HexEncoder*>(_DefaultInstance.get());
	return std::move(std::make_unique<HexEncoder>(*instance));
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::HexEncoder::Clone() const
{
	return std::move(std::make_unique<HexEncoder>(*this));
}

const std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::HexEncoder::_DefaultInstance = std::make_unique<InsaneIO::Insane::Cryptography::HexEncoder>();

const std::function<std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder>(const String&)> InsaneIO::Insane::Cryptography::HexEncoder::_DefaultDeserializeResolver = [](const String& json)->std::unique_ptr<IEncoder> {
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError() || !(document.IsObject() && document.HasMember(CSTRINGIFY(ToUpper))))
		{
			throw true;
		}
		return std::move(std::make_unique<HexEncoder>(document[CNAMEOF(ToUpper)].GetBool()));
	}
	catch (...)
	{
		throw InsaneIO::Insane::Exception::DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
};

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::HexEncoder::Deserialize(const String& json, const std::function<std::unique_ptr<IEncoder>(const String&)>& resolver)
{
	return std::move(resolver(json));
}

std::function<std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder>(const String&)> InsaneIO::Insane::Cryptography::HexEncoder::DefaultDeserializeResolver()
{
	return _DefaultDeserializeResolver;
}


// ███ Base32Encoder ███

InsaneIO::Insane::Cryptography::Base32Encoder::Base32Encoder(const size_t& removePadding, const bool& toLower)
	: IEncoder(BASE32_ENCODER_NAME_STRING), RemovePadding(removePadding), ToLower(toLower)
{
}

size_t InsaneIO::Insane::Cryptography::Base32Encoder::GetToLower() const
{
	return ToLower;
}

bool InsaneIO::Insane::Cryptography::Base32Encoder::GetRemovePadding() const
{
	return RemovePadding;
}

String InsaneIO::Insane::Cryptography::Base32Encoder::Encode(const String& data) const
{
	return Base32EncodingExtensions::ToBase32(data, RemovePadding, ToLower);
}

String InsaneIO::Insane::Cryptography::Base32Encoder::Decode(const String& data) const
{
	return Base32EncodingExtensions::FromBase32(data);
}

String InsaneIO::Insane::Cryptography::Base32Encoder::Serialize(const bool& indent) const
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CORE;
	try
	{
		rapidjson::StringBuffer sb;
		rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

		String number;
		writer.StartObject();

		writer.Key(CNAMEOF(Name));
		writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

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

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::Base32Encoder::Clone() const
{
	return std::move(std::make_unique<Base32Encoder>(*this));
}

const std::function<std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder>(const String&)> InsaneIO::Insane::Cryptography::Base32Encoder::_DefaultDeserializeResolver = [](const String& json)->std::unique_ptr<IEncoder>
{
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError() ||
			!(document.IsObject() && document.HasMember(CSTRINGIFY(ToLower)) && document.HasMember(CSTRINGIFY(RemovePadding))))
		{
			throw true;
		}
		return std::move(std::make_unique<Base32Encoder>(document[CNAMEOF(RemovePadding)].GetBool(),
			document[CNAMEOF(ToLower)].GetBool()));
	}
	catch (...)
	{
		throw InsaneIO::Insane::Exception::DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
};

const std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::Base32Encoder::_DefaultInstance = std::make_unique<InsaneIO::Insane::Cryptography::Base32Encoder>();

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::Base32Encoder::DefaultInstance()
{
	Base32Encoder* instance = dynamic_cast<Base32Encoder*>(_DefaultInstance.get());
	return std::move(std::make_unique<Base32Encoder>(*instance));
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::Base32Encoder::Deserialize(const String& json, const std::function<std::unique_ptr<IEncoder>(const String&)>& resolver)
{
	return std::move(resolver(json));
}

std::function<std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder>(const String&)> InsaneIO::Insane::Cryptography::Base32Encoder::DefaultDeserializeResolver()
{
	return _DefaultDeserializeResolver;
}

// ███ Base64Encoder ███

InsaneIO::Insane::Cryptography::Base64Encoder::Base64Encoder(const size_t& lineBreaksLength, const bool& removePadding, const Base64Encoding& encodingType)
	: IEncoder(BASE64_ENCODER_NAME_STRING), LineBreaksLength(lineBreaksLength), RemovePadding(removePadding), EncodingType(encodingType)
{
}


inline size_t InsaneIO::Insane::Cryptography::Base64Encoder::GetLineBreaksLength() const {
	return LineBreaksLength;
}

inline bool InsaneIO::Insane::Cryptography::Base64Encoder::GetRemovePadding() const {
	return RemovePadding;
}

inline InsaneIO::Insane::Cryptography::Base64Encoding InsaneIO::Insane::Cryptography::Base64Encoder::GetEncodingType() const {
	return EncodingType;
}

String InsaneIO::Insane::Cryptography::Base64Encoder::Encode(const String& data) const
{
	USING_NS_INSANE_EXCEPTION;
	switch (EncodingType)
	{
	case Base64Encoding::Base64:
		return Base64EncodingExtensions::ToBase64(data, LineBreaksLength, RemovePadding);
	case Base64Encoding::UrlSafeBase64:
		return Base64EncodingExtensions::ToUrlSafeBase64(data);
	case Base64Encoding::FileNameSafeBase64:
		return Base64EncodingExtensions::ToFilenameSafeBase64(data);
	case Base64Encoding::UrlEncodedBase64:
		return Base64EncodingExtensions::ToUrlEncodedBase64(data);
	default:
		throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, Base64EncodingEnumExtensions::ToString(EncodingType, true));
	}
}

String InsaneIO::Insane::Cryptography::Base64Encoder::Decode(const String& data) const
{
	return Base64EncodingExtensions::FromBase64(data);
}

String InsaneIO::Insane::Cryptography::Base64Encoder::Serialize(const bool& indent) const {
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CORE;
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
#if SIZE_MAX == UINT32_MAX
		writer->Uint(LineBreaksLength());
#elif SIZE_MAX == UINT64_MAX
		writer.Uint64(LineBreaksLength);
#endif

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

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::Base64Encoder::Clone() const
{
	return std::move(std::make_unique<Base64Encoder>(*this));
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::Base64Encoder::Deserialize(const String& json, const std::function<std::unique_ptr<IEncoder>(const String&)>& resolver)
{
	return std::move(resolver(json));
}

const std::function<std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder>(const String&)> InsaneIO::Insane::Cryptography::Base64Encoder::_DefaultDeserializeResolver = [](const String& json)->std::unique_ptr<IEncoder>
{
	USING_NS_INSANE_EXCEPTION;
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError() ||
			!(document.IsObject() && document.HasMember(CSTRINGIFY(LineBreaksLength)) && document.HasMember(CSTRINGIFY(RemovePadding)) && document.HasMember(CSTRINGIFY(EncodingType))))
		{
			throw true;
		}
		return std::move(std::make_unique<Base64Encoder>(document[CNAMEOF(LineBreaksLength)].GetUint(),
			document[CNAMEOF(RemovePadding)].GetBool(),
			Base64EncodingEnumExtensions::Parse(document[CNAMEOF(EncodingType)].GetInt())));
	}
	catch (...)
	{
		throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
};

const std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::Base64Encoder::_DefaultInstance = std::make_unique<InsaneIO::Insane::Cryptography::Base64Encoder>();

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::Base64Encoder::DefaultInstance()
{
	Base64Encoder* instance = dynamic_cast<Base64Encoder*>(_DefaultInstance.get());
	return std::move(std::make_unique<Base64Encoder>(*instance));
}

std::function<std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder>(const String&)> InsaneIO::Insane::Cryptography::Base64Encoder::DefaultDeserializeResolver()
{
	return _DefaultDeserializeResolver;
}

// ███ DefaultEncoderFunctions ███

static inline const std::map<String, std::function<std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder>()>> DefaultEncoderFunctions = {
	{ HEX_ENCODER_NAME_STRING,
		[]() { return std::move(InsaneIO::Insane::Cryptography::HexEncoder::DefaultInstance()); }
	} ,

	{ BASE32_ENCODER_NAME_STRING,
		[]() { return std::move(InsaneIO::Insane::Cryptography::Base32Encoder::DefaultInstance()); }
	} ,

	{ BASE64_ENCODER_NAME_STRING,
		[]() { return std::move(InsaneIO::Insane::Cryptography::Base64Encoder::DefaultInstance()); }
	} ,
};

// ███ DefaultProtectorsFunctions ███
static inline const std::map<String, std::function< std::unique_ptr<ISecretProtector>()>> DefaultProtectorFunctions = {
	{
		AES_CBC_PROTECTOR_NAME_STRING,
		[]() { return std::move(InsaneIO::Insane::Cryptography::AesCbcProtector::DefaultInstance()); }
	}
};


// ███ IHasher ███

InsaneIO::Insane::Cryptography::IHasher::IHasher(const String& name) : IJsonSerialize(name)
{

}

std::unique_ptr<InsaneIO::Insane::Cryptography::IHasher> InsaneIO::Insane::Cryptography::IHasher::Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver)
{
	return IJsonSerialize::Deserialize(json, resolver);
}

std::function<std::unique_ptr<IHasher>(String)> InsaneIO::Insane::Cryptography::IHasher::DefaultDeserializeResolver()
{
	return IJsonSerialize::DefaultDeserializeResolver();
}

// ███ ShaHasher ███

InsaneIO::Insane::Cryptography::ShaHasher::ShaHasher(const InsaneIO::Insane::Cryptography::HashAlgorithm& hashAlgorithm, std::unique_ptr<IEncoder>&& encoder) :
	IHasher(SHA_HASHER_NAME_STRING), _HashAlgorithm(hashAlgorithm), _Encoder(encoder ? std::move(encoder) : Base64Encoder::DefaultInstance())
{
}

InsaneIO::Insane::Cryptography::ShaHasher::ShaHasher(const ShaHasher& instance) : ShaHasher(instance._HashAlgorithm, std::move(instance.GetEncoder()))
{
}

InsaneIO::Insane::Cryptography::HashAlgorithm InsaneIO::Insane::Cryptography::ShaHasher::GetHashAlgorithm() const
{
	return _HashAlgorithm;
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::ShaHasher::GetEncoder() const
{
	return std::move(_Encoder->Clone());
}

String InsaneIO::Insane::Cryptography::ShaHasher::Compute(const String& data)
{
	return HashExtensions::ToHash(data, _HashAlgorithm);
}

bool InsaneIO::Insane::Cryptography::ShaHasher::Verify(const String& data, const String& expected)
{
	return Compute(data) == expected;
}

String InsaneIO::Insane::Cryptography::ShaHasher::ComputeEncoded(const String& data)
{
	return _Encoder->Encode(Compute(data));
}

bool InsaneIO::Insane::Cryptography::ShaHasher::VerifyEncoded(const String& data, const String& expected)
{
	return ComputeEncoded(data) == expected;
}

String InsaneIO::Insane::Cryptography::ShaHasher::Serialize(const bool& indent) const
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CORE;
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

std::unique_ptr<InsaneIO::Insane::Cryptography::IHasher> InsaneIO::Insane::Cryptography::ShaHasher::Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver)
{
	return std::move(resolver(json));
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IHasher> InsaneIO::Insane::Cryptography::ShaHasher::Clone() const
{
	return std::move(std::make_unique<ShaHasher>(*this));
}


std::function<std::unique_ptr<IHasher>(String)> InsaneIO::Insane::Cryptography::ShaHasher::DefaultDeserializeResolver()
{
	return _DefaultDeserializeResolver;
}

static inline std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InternalDefaultDeserializeIEncoder(const rapidjson::Value& value)
{
	USING_NS_INSANE_CRYPTO;
	USING_NS_INSANE_CORE;
	USING_NS_INSANE_EXCEPTION;
	try
	{
		String json = RapidJsonExtensions::ToJson(value);
		String name = RapidJsonExtensions::GetStringValue(value, STRINGIFY(Name));
		std::function<std::unique_ptr<IEncoder>()> encoderFx = DefaultEncoderFunctions.at(name);
		return std::move(encoderFx());
	}
	catch (...)
	{
		throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
}

const std::function<std::unique_ptr<IHasher>(String)> InsaneIO::Insane::Cryptography::ShaHasher::_DefaultDeserializeResolver = [](const String& json)->std::unique_ptr<IHasher>
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CRYPTO;
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError() ||
			!(document.IsObject() && document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetHashAlgorithm)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
		{
			throw true;
		}
		std::unique_ptr<IEncoder> encoder = InternalDefaultDeserializeIEncoder(document[CNAMEOF_TRIM_GET(GetEncoder)]);
		HashAlgorithm algorithm = HashAlgorithmEnumExtensions::Parse(document[CNAMEOF_TRIM_GET(GetHashAlgorithm)].GetInt());
		return std::move(std::make_unique<ShaHasher>(algorithm, std::move(encoder)));
	}
	/*catch(const NotImplementedException & ex)
	{
		DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, EMPTY_STRING, 0, std ex);
	}*/
	catch (...)
	{
		throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
};

// ███ HmacHasher ███

InsaneIO::Insane::Cryptography::HmacHasher::HmacHasher(const String& key, const HashAlgorithm& hashAlgorithm, std::unique_ptr<IEncoder>&& encoder)
	: IHasher(HMAC_HASHER_NAME_STRING), _Key(key.empty() ? RandomExtensions::Next(SHA512_DIGEST_LENGTH) : key), _HashAlgorithm(hashAlgorithm), _Encoder(encoder ? std::move(encoder) : std::move(Base64Encoder::DefaultInstance()))
{
}

InsaneIO::Insane::Cryptography::HmacHasher::HmacHasher(const HmacHasher& instance)
	: HmacHasher(instance.GetKey(), instance.GetHashAlgorithm(), std::move(instance.GetEncoder()))
{
}

InsaneIO::Insane::Cryptography::HashAlgorithm InsaneIO::Insane::Cryptography::HmacHasher::GetHashAlgorithm() const
{
	return _HashAlgorithm;
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::HmacHasher::GetEncoder() const
{
	return std::move(_Encoder->Clone());
}

String InsaneIO::Insane::Cryptography::HmacHasher::GetKey() const
{
	return _Key;
}

String InsaneIO::Insane::Cryptography::HmacHasher::GetKeyEncoded() const
{
	return _Encoder->Encode(_Key);
}

String InsaneIO::Insane::Cryptography::HmacHasher::Compute(const String& data)
{
	return HashExtensions::ToHmac(data, _Key, _HashAlgorithm);
}

bool InsaneIO::Insane::Cryptography::HmacHasher::Verify(const String& data, const String& expected)
{
	return Compute(data) == expected;
}

String InsaneIO::Insane::Cryptography::HmacHasher::ComputeEncoded(const String& data)
{
	return _Encoder->Encode(Compute(data));
}

bool InsaneIO::Insane::Cryptography::HmacHasher::VerifyEncoded(const String& data, const String& expected)
{
	return ComputeEncoded(data) == expected;
}

String InsaneIO::Insane::Cryptography::HmacHasher::Serialize(const bool& indent) const
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	USING_NS_INSANE_CORE;
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

std::unique_ptr<InsaneIO::Insane::Cryptography::IHasher> InsaneIO::Insane::Cryptography::HmacHasher::Clone() const
{
	return std::move(std::make_unique<HmacHasher>(*this));
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IHasher> InsaneIO::Insane::Cryptography::HmacHasher::Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver)
{
	return std::move(resolver(json));
}

const std::function<std::unique_ptr<IHasher>(String)> InsaneIO::Insane::Cryptography::HmacHasher::_DefaultDeserializeResolver = [](const String& json)->std::unique_ptr<IHasher>
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CRYPTO;
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError() ||
			!(document.IsObject() && document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetKey)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetHashAlgorithm)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
		{
			throw true;
		}
		std::unique_ptr<IEncoder> encoder = InternalDefaultDeserializeIEncoder(document[CNAMEOF_TRIM_GET(GetEncoder)]);
		HashAlgorithm algorithm = HashAlgorithmEnumExtensions::Parse(document[CNAMEOF_TRIM_GET(GetHashAlgorithm)].GetInt());
		String key = encoder->Decode(document[CNAMEOF_TRIM_GET(GetKey)].GetString());
		return std::move(std::make_unique<HmacHasher>(key, algorithm, std::move(encoder)));
	}
	catch (...)
	{
		throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
};

std::function<std::unique_ptr<IHasher>(String)> InsaneIO::Insane::Cryptography::HmacHasher::DefaultDeserializeResolver()
{
	return _DefaultDeserializeResolver;
}

// ███ Argon2Hasher ███

InsaneIO::Insane::Cryptography::Argon2Hasher::Argon2Hasher(const String& salt,
	const size_t& iterations,
	const size_t& memorySizeKiB,
	const size_t& degreeOfParallelism,
	const Argon2Variant argon2Variant,
	const size_t& derivedKeyLength,
	std::unique_ptr<IEncoder>&& encoder) : IHasher(ARGON2_HASHER_NAME_STRING),
	_Salt(salt),
	_Iterations(iterations),
	_MemorySizeKiB(memorySizeKiB),
	_DegreeOfParallelism(degreeOfParallelism),
	_Argon2Variant(argon2Variant),
	_DerivedKeyLength(derivedKeyLength),
	_Encoder(encoder ? std::move(encoder) : std::move(Base64Encoder::DefaultInstance()))
{
}

InsaneIO::Insane::Cryptography::Argon2Hasher::Argon2Hasher(const Argon2Hasher& instance)
	:Argon2Hasher(instance.GetSalt(), instance.GetIterations(), instance.GetMemorySizeKiB(), instance.GetDegreeOfParallelism(), instance.GetArgon2Variant(), instance.GetDerivedKeyLength(), std::move(instance.GetEncoder()))
{
}

String InsaneIO::Insane::Cryptography::Argon2Hasher::GetSalt() const
{
	return _Salt;
}

String InsaneIO::Insane::Cryptography::Argon2Hasher::GetSaltEncoded() const
{
	return _Encoder->Encode(_Salt);
}

size_t InsaneIO::Insane::Cryptography::Argon2Hasher::GetIterations() const
{
	return _Iterations;
}

size_t InsaneIO::Insane::Cryptography::Argon2Hasher::GetMemorySizeKiB() const
{
	return _MemorySizeKiB;
}

size_t InsaneIO::Insane::Cryptography::Argon2Hasher::GetDegreeOfParallelism() const
{
	return _DegreeOfParallelism;
}

size_t InsaneIO::Insane::Cryptography::Argon2Hasher::GetDerivedKeyLength() const
{
	return _DerivedKeyLength;
}

InsaneIO::Insane::Cryptography::Argon2Variant InsaneIO::Insane::Cryptography::Argon2Hasher::GetArgon2Variant() const
{
	return _Argon2Variant;
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::Argon2Hasher::GetEncoder() const
{
	return std::move(_Encoder->Clone());
}

String InsaneIO::Insane::Cryptography::Argon2Hasher::Compute(const String& data)
{
	return HashExtensions::ToArgon2(data, _Salt, _Iterations, _MemorySizeKiB, _DegreeOfParallelism, _Argon2Variant, _DerivedKeyLength);
}

bool InsaneIO::Insane::Cryptography::Argon2Hasher::Verify(const String& data, const String& expected)
{
	return Compute(data) == expected;
}

String InsaneIO::Insane::Cryptography::Argon2Hasher::ComputeEncoded(const String& data)
{
	return _Encoder->Encode(Compute(data));
}

bool InsaneIO::Insane::Cryptography::Argon2Hasher::VerifyEncoded(const String& data, const String& expected)
{
	return ComputeEncoded(data) == expected;
}

String InsaneIO::Insane::Cryptography::Argon2Hasher::Serialize(const bool& indent) const
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	USING_NS_INSANE_CORE;
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

std::unique_ptr<InsaneIO::Insane::Cryptography::IHasher> InsaneIO::Insane::Cryptography::Argon2Hasher::Clone() const
{
	return std::move(std::make_unique<Argon2Hasher>(*this));
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IHasher> InsaneIO::Insane::Cryptography::Argon2Hasher::Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver)
{
	return std::move(resolver(json));
}

std::function<std::unique_ptr<IHasher>(String)> InsaneIO::Insane::Cryptography::Argon2Hasher::DefaultDeserializeResolver()
{
	return _DefaultDeserializeResolver;
}

const std::function<std::unique_ptr<IHasher>(String)> InsaneIO::Insane::Cryptography::Argon2Hasher::_DefaultDeserializeResolver = [](const String& json)->std::unique_ptr<IHasher>
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CRYPTO;
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError() ||
			!(document.IsObject() && document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
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
		std::unique_ptr<IEncoder> encoder = InternalDefaultDeserializeIEncoder(document[CNAMEOF_TRIM_GET(GetEncoder)]);
		String salt = encoder->Decode(document[CNAMEOF_TRIM_GET(GetSalt)].GetString());
#if SIZE_MAX == UINT32_MAX
		size_t iterations = document[CNAMEOF_TRIM_GET(GetIterations)].GetUint();
		size_t memorySizeKiB = document[CNAMEOF_TRIM_GET(GetMemorySizeKiB)].GetUint();
		size_t degreeOfParallelism = document[CNAMEOF_TRIM_GET(GetDegreeOfParallelism)].GetUint();
		size_t derivedKeyLength = document[CNAMEOF_TRIM_GET(GetDerivedKeyLength)].GetUint();
#elif SIZE_MAX == UINT64_MAX
		size_t iterations = document[CNAMEOF_TRIM_GET(GetIterations)].GetUint64();
		size_t memorySizeKiB = document[CNAMEOF_TRIM_GET(GetMemorySizeKiB)].GetUint64();
		size_t degreeOfParallelism = document[CNAMEOF_TRIM_GET(GetDegreeOfParallelism)].GetUint64();
		size_t derivedKeyLength = document[CNAMEOF_TRIM_GET(GetDerivedKeyLength)].GetUint64();
#endif
		Argon2Variant argon2Variant = Argon2VariantEnumExtensions::Parse(document[CNAMEOF_TRIM_GET(GetArgon2Variant)].GetInt());
		return std::move(std::make_unique<Argon2Hasher>(salt, iterations, memorySizeKiB, degreeOfParallelism, argon2Variant, derivedKeyLength, std::move(encoder)));
	}
	catch (...)
	{
		throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
};


// ███ ScryptHasher ███

InsaneIO::Insane::Cryptography::ScryptHasher::ScryptHasher(const String& salt,
	const size_t& iterations,
	const size_t& blocksize,
	const size_t& parallelism,
	const size_t& derivedKeyLength,
	std::unique_ptr<IEncoder>&& encoder)
	: IHasher(SCRYPT_HASHER_NAME_STRING),
	_Salt(salt),
	_Iterations(iterations),
	_BlockSize(blocksize),
	_Parallelism(parallelism),
	_DerivedKeyLength(derivedKeyLength),
	_Encoder(encoder ? std::move(encoder) : std::move(Base64Encoder::DefaultInstance()))
{
}

InsaneIO::Insane::Cryptography::ScryptHasher::ScryptHasher(const ScryptHasher& instance)
	:ScryptHasher(instance.GetSalt(), instance.GetIterations(), instance.GetBlockSize(), instance.GetParallelism(), instance.GetDerivedKeyLength(), std::move(instance.GetEncoder()))
{
}

String InsaneIO::Insane::Cryptography::ScryptHasher::GetSalt() const
{
	return _Salt;
}

String InsaneIO::Insane::Cryptography::ScryptHasher::GetSaltEncoded() const
{
	return _Encoder->Encode(_Salt);
}

size_t InsaneIO::Insane::Cryptography::ScryptHasher::GetIterations() const
{
	return _Iterations;
}

size_t InsaneIO::Insane::Cryptography::ScryptHasher::GetBlockSize() const
{
	return _BlockSize;
}

size_t InsaneIO::Insane::Cryptography::ScryptHasher::GetParallelism() const
{
	return _Parallelism;
}

size_t InsaneIO::Insane::Cryptography::ScryptHasher::GetDerivedKeyLength() const
{
	return _DerivedKeyLength;
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::ScryptHasher::GetEncoder() const
{
	return std::move(_Encoder->Clone());
}

String InsaneIO::Insane::Cryptography::ScryptHasher::Compute(const String& data)
{
	return HashExtensions::ToScrypt(data, _Salt, _Iterations, _BlockSize, _Parallelism, _DerivedKeyLength);
}

bool InsaneIO::Insane::Cryptography::ScryptHasher::Verify(const String& data, const String& expected)
{
	return Compute(data) == expected;
}

String InsaneIO::Insane::Cryptography::ScryptHasher::ComputeEncoded(const String& data)
{
	return _Encoder->Encode(Compute(data));
}

bool InsaneIO::Insane::Cryptography::ScryptHasher::VerifyEncoded(const String& data, const String& expected)
{
	return ComputeEncoded(data) == expected;
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IHasher> InsaneIO::Insane::Cryptography::ScryptHasher::Clone() const
{
	return std::move(std::make_unique<ScryptHasher>(*this));
}

std::function<std::unique_ptr<IHasher>(String)> InsaneIO::Insane::Cryptography::ScryptHasher::DefaultDeserializeResolver()
{
	return _DefaultDeserializeResolver;
}

String InsaneIO::Insane::Cryptography::ScryptHasher::Serialize(const bool& indent) const
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	USING_NS_INSANE_CORE;
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

std::unique_ptr<InsaneIO::Insane::Cryptography::IHasher> InsaneIO::Insane::Cryptography::ScryptHasher::Deserialize(const String& json, const std::function<std::unique_ptr<IHasher>(String)>& resolver)
{
	return std::move(resolver(json));
}

const std::function<std::unique_ptr<IHasher>(String)> InsaneIO::Insane::Cryptography::ScryptHasher::_DefaultDeserializeResolver = [](const String& json)->std::unique_ptr<IHasher>
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CRYPTO;
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError() ||
			!(document.IsObject() && document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetSalt)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetIterations)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetBlockSize)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetParallelism)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetDerivedKeyLength)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
		{
			throw true;
		}
		std::unique_ptr<IEncoder> encoder = InternalDefaultDeserializeIEncoder(document[CNAMEOF_TRIM_GET(GetEncoder)]);
		String salt = encoder->Decode(document[CNAMEOF_TRIM_GET(GetSalt)].GetString());
#if SIZE_MAX == UINT32_MAX
		size_t iterations = document[CNAMEOF_TRIM_GET(GetIterations)].GetUint();
		size_t blockSize = document[CNAMEOF_TRIM_GET(GetBlockSize)].GetUint();
		size_t parallelism = document[CNAMEOF_TRIM_GET(GetParallelism)].GetUint();
		size_t derivedKeyLength = document[CNAMEOF_TRIM_GET(GetDerivedKeyLength)].GetUint();
#elif SIZE_MAX == UINT64_MAX
		size_t iterations = document[CNAMEOF_TRIM_GET(GetIterations)].GetUint64();
		size_t blockSize = document[CNAMEOF_TRIM_GET(GetBlockSize)].GetUint64();
		size_t parallelism = document[CNAMEOF_TRIM_GET(GetParallelism)].GetUint64();
		size_t derivedKeyLength = document[CNAMEOF_TRIM_GET(GetDerivedKeyLength)].GetUint64();
#endif
		return std::move(std::make_unique<ScryptHasher>(salt, iterations, blockSize, parallelism, derivedKeyLength, std::move(encoder)));
	}
	catch (...)
	{
		throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
};

// ███ IEncryptor ███

InsaneIO::Insane::Cryptography::IEncryptor::IEncryptor(const String& name) : ISecureJsonSerialize(name)
{
}


// ███ AesCbcEncryptor ███
InsaneIO::Insane::Cryptography::AesCbcEncryptor::AesCbcEncryptor(const String& key, const AesCbcPadding& padding, std::unique_ptr<IEncoder>&& encoder)
	: IEncryptor(AES_CBC_ENCRYPTOR_NAME_STRING), _Key(key), _Padding(padding), _Encoder(encoder ? std::move(encoder) : std::move(Base64Encoder::DefaultInstance()))
{
}

InsaneIO::Insane::Cryptography::AesCbcEncryptor::AesCbcEncryptor(const AesCbcEncryptor& instance)
	: AesCbcEncryptor(instance.GetKey(), instance.GetPadding(), std::move(instance.GetEncoder()))
{
}

String InsaneIO::Insane::Cryptography::AesCbcEncryptor::GetKey() const
{
	return _Key;
}

String InsaneIO::Insane::Cryptography::AesCbcEncryptor::GetKeyEncoded() const
{
	return _Encoder->Encode(_Key);
}

InsaneIO::Insane::Cryptography::AesCbcPadding InsaneIO::Insane::Cryptography::AesCbcEncryptor::GetPadding() const
{
	return _Padding;
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncoder> InsaneIO::Insane::Cryptography::AesCbcEncryptor::GetEncoder() const
{
	return std::move(_Encoder->Clone());
}

String InsaneIO::Insane::Cryptography::AesCbcEncryptor::Encrypt(const String& data) const
{
	return AesExtensions::EncryptAesCbc(data, _Key, _Padding);
}

String InsaneIO::Insane::Cryptography::AesCbcEncryptor::EncryptEncoded(const String& data) const
{
	return _Encoder->Encode(Encrypt(data));
}

String InsaneIO::Insane::Cryptography::AesCbcEncryptor::Decrypt(const String& data) const
{
	return AesExtensions::DecryptAesCbc(data, _Key, _Padding);
}

String InsaneIO::Insane::Cryptography::AesCbcEncryptor::DecryptEncoded(const String& data) const
{
	return Decrypt(_Encoder->Decode(data));
}

std::unique_ptr<InsaneIO::Insane::Cryptography::IEncryptor> InsaneIO::Insane::Cryptography::AesCbcEncryptor::Clone() const
{
	return std::make_unique<AesCbcEncryptor>(*this);
}

String InsaneIO::Insane::Cryptography::AesCbcEncryptor::Serialize(const String& serializeKey, const bool& indent, const std::unique_ptr<ISecretProtector>& protector) const
{
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_STR;
	USING_NS_INSANE_CORE;
	try
	{
		rapidjson::StringBuffer sb;
		rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
		writer.StartObject();

		writer.Key(CNAMEOF_TRIM_GET(GetName));
		writer.String(GetName().c_str(), static_cast<rapidjson::SizeType>(GetName().length()));

		writer.Key(CNAMEOF(Protector));
		writer.String(protector->GetName().c_str(), static_cast<rapidjson::SizeType>(protector->GetName().length()));

		writer.Key(CNAMEOF_TRIM_GET(GetKey));
		String value = _Encoder->Encode(protector->Protect(GetKey(), serializeKey));
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

const ProtectorResolver AesCbcEncryptor::_DefaultProtectorResolver =
[](const String& name)-> std::unique_ptr<ISecretProtector> {
	USING_NS_INSANE_EXCEPTION;
	try
	{
		std::function<std::unique_ptr<ISecretProtector>()> protectorFx = DefaultProtectorFunctions.at(name);
		return std::move(protectorFx());
	}
	catch (...)
	{
		throw NotImplementedException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
};

InsaneIO::Insane::Cryptography::ProtectorResolver InsaneIO::Insane::Cryptography::AesCbcEncryptor::DefaultProtectorResolver()
{
	return _DefaultProtectorResolver;
}

const SecureDeserializeResolver<InsaneIO::Insane::Cryptography::IEncryptor> InsaneIO::Insane::Cryptography::AesCbcEncryptor::_DefaultDeserializeResolver =
[](const String& json, const String& serializeKey) ->std::unique_ptr<IEncryptor> {
	USING_NS_INSANE_EXCEPTION;
	USING_NS_INSANE_CRYPTO;
	try
	{
		rapidjson::Document document;
		document.Parse(json.c_str(), json.length());
		if (document.HasParseError() ||
			!(document.IsObject() && document.HasMember(CNAMEOF_TRIM_GET(GetName)) &&
				document.HasMember(CNAMEOF(Protector)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetKey)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetPadding)) &&
				document.HasMember(CNAMEOF_TRIM_GET(GetEncoder))))
		{
			throw true;
		}
		String protectorName = document[CNAMEOF(Protector)].GetString();
		std::unique_ptr<IEncoder> encoder = InternalDefaultDeserializeIEncoder(document[CNAMEOF_TRIM_GET(GetEncoder)]);
		std::unique_ptr<ISecretProtector> protector = _DefaultProtectorResolver(protectorName);
		String key = protector->Unprotect(encoder->Decode(document[CNAMEOF_TRIM_GET(GetKey)].GetString()), serializeKey);
		AesCbcPadding padding = AesCbcPaddingEnumExtensions::Parse(document[CNAMEOF_TRIM_GET(GetPadding)].GetInt());
		return std::move(std::make_unique<AesCbcEncryptor>(key, padding, std::move(encoder)));
	}
	catch (...)
	{
		throw DeserializeException(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__);
	}
};

SecureDeserializeResolver<IEncryptor> InsaneIO::Insane::Cryptography::AesCbcEncryptor::DefaultDeserializeResolver()
{
	return _DefaultDeserializeResolver;
}

std::unique_ptr<IEncryptor> InsaneIO::Insane::Cryptography::AesCbcEncryptor::Deserialize(const String& json, const String& serializeKey, const SecureDeserializeResolver<IEncryptor>& deserializeResolver)
{
	return deserializeResolver(json, serializeKey);
}


// ███ ISecretProtector ███
InsaneIO::Insane::Cryptography::ISecretProtector::ISecretProtector(const String& name) : _Name(name)
{

}

String InsaneIO::Insane::Cryptography::ISecretProtector::GetName()
{
	return _Name;
}

// ███ AesCbcProtector ███
InsaneIO::Insane::Cryptography::AesCbcProtector::AesCbcProtector() : ISecretProtector(AES_CBC_PROTECTOR_NAME_STRING)
{
}

String InsaneIO::Insane::Cryptography::AesCbcProtector::Protect(const String& secret, const String& key)
{
	return AesExtensions::EncryptAesCbc(secret, key);
}

String InsaneIO::Insane::Cryptography::AesCbcProtector::Unprotect(const String& secret, const String& key)
{
	return AesExtensions::DecryptAesCbc(secret, key);
}

const std::unique_ptr<ISecretProtector> InsaneIO::Insane::Cryptography::AesCbcProtector::_DefaultInstance = std::make_unique<AesCbcProtector>(AesCbcProtector());

std::unique_ptr<InsaneIO::Insane::Cryptography::ISecretProtector> InsaneIO::Insane::Cryptography::AesCbcProtector::DefaultInstance()
{
	AesCbcProtector* instance = dynamic_cast<AesCbcProtector*>(_DefaultInstance.get());
	return std::move(std::make_unique<AesCbcProtector>(*instance));
}
