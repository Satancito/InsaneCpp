#pragma once
#include "Insane/Insane.h"
#include <ostream>
#ifndef INSANE_BASE_H
#define INSANE_BASE_H
#define INSANE_STRING ("Insane"s)

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <vector>
#include <cstdint>
#include <memory>
#include <map>

#include <Insane/InsanePreprocessor.h>

using SignedChar = char;
using SignedInt8 = int8_t;
using SignedInt16 = int16_t;
using SignedInt32 = int32_t;
using SignedInt64 = int64_t;
using UnsignedInt8 = uint8_t;
using UnsignedInt16 = uint16_t;
using UnsignedInt32 = uint32_t;
using UnsignedInt64 = uint64_t;
using UnsignedChar = unsigned char;

using String = std::string;
using StdString = std::string;

template <typename T>
using StdVector = std::vector<T>;
using StdVectorUint8 = StdVector<UnsignedInt8>;
using StdVectorInt8 = StdVector<SignedInt8>;
using StdVectorChar = StdVector<SignedChar>;
using StdVectorUchar = StdVector<UnsignedChar>;

template <typename T>
using StdUniquePtr = std::unique_ptr<T>;

template <typename T>
using StdSharedPtr = std::shared_ptr<T>;

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
	concept IsPrintableAndEqualityComparable = HasOstream<T> && std::equality_comparable<T>;

	template <typename T>
	concept IsEqualityComparable = std::equality_comparable<T>;

	template <typename T, T value, T min, T max>
	concept CheckRange = (value >= min && value <= max);

	// ███ UtilityExtensions ███

	class INSANE_API DebugExtensions
	{
	public:
		static bool Debug(bool);
		[[nodiscard]] static bool IsDebug();
	private:
	};

	namespace Interfaces
	{
		// ███ IClone ███
		template <typename T>
		class INSANE_API IClone
		{
		public:
			virtual std::shared_ptr<T> Clone() const = 0;
		};
	}
}

#endif // !INSANE_BASE_H
