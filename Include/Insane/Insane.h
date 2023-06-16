#pragma once
#ifndef INSANE_BASE_H
#define INSANE_BASE_H
#define INSANE_STRING ("Insane"s)

#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include <climits>
#include <array>
#include <random>
#include <functional>
#include <vector>
#include <locale>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <vector>
#include <set>
#include <cstdlib>
#include <regex>
#include <cstdint>
#include <memory>

#include <Insane/InsanePreprocessor.h>

// typedef uint8_t UnsignedInt8; //TODO
// typedef int8_t SignedInt8;
// typedef uint16_t UnsignedInt16;
// typedef int16_t SignedInt16;
// typedef uint32_t UnsignedInt32;
// typedef int32_t SignedInt32;
// typedef uint64_t UnsignedInt64;
// typedef int64_t SignedInt64;

typedef unsigned char UnsignedChar;
typedef char SignedChar;
using String = std::string;
using StdString = std::string;

template <typename T>
using StdVector = std::vector<T>;
using StdVectorUint8 = StdVector<uint8_t>;
using StdVectorInt8 = StdVector<int8_t>;
using StdVectorChar = StdVector<char>;

template<typename T>
struct __UniquePtr {
    using Type = std::unique_ptr<T>;
};

template <typename T>
using StdUniquePtr = __UniquePtr<T>::Type;

template <typename T>
using OutFunction = std::function<std::ostream &(const T &, std::ostream &)>;

#define thisvalue (*this)
#define USING_NS_INSANE using namespace InsaneIO::Insane
#define IS_DEBUG InsaneIO::Insane::UtilityExtensions::IsDebug()

using namespace std::string_literals;
using namespace std::chrono_literals;
namespace InsaneIO::Insane
{

	// ███ Concepts ███
	template <typename T>
	concept HasOstream = requires(std::ostream &os, T t) {
		os << t;
	};

	template <typename T>
	concept IsPrintable = HasOstream<T>;

	template <typename T>
	concept Printable = IsPrintable<T>;

	template <typename T>
	concept PrintableAndEqualityComparable = HasOstream<T> && std::equality_comparable<T>;

	template <typename T>
	concept EqualityComparable = std::equality_comparable<T>;

	template <typename T, T value, T min, T max>
	concept CheckRange = (value >= min && value <= max);

	// ███ UtilityExtensions ███

	class INSANE_API UtilityExtensions
	{
	public:
		[[nodiscard]] static bool IsDebug()
		{
#if defined(_DEBUG)
			return true;
#else
			return false;
#endif // !
		}
	};

	namespace Interfaces
	{
		// ███ IClone ███
		template <typename T>
		class INSANE_API IClone
		{
		public:
			virtual std::unique_ptr<T> Clone() const = 0;
		};
	}
}

#endif // !INSANE_BASE_H
