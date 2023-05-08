#pragma once
#ifndef INSANE_BASE_H
#define INSANE_BASE_H
#define INSANE_STRING "Insane"s

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

#include <Insane/InsanePreprocessor.h>

typedef uint8_t UnsignedInt8;
typedef int8_t SignedInt8;
typedef uint16_t UnsignedInt16;
typedef int16_t SignedInt16;
typedef uint32_t UnsignedInt32;
typedef int32_t SignedInt32;
typedef uint64_t UnsignedInt64;
typedef int64_t SignedInt64;

typedef unsigned char UnsignedChar;
typedef char SignedChar;
typedef std::string String;
typedef std::wstring WString;

#define thisvalue (*this)
#define USING_NS_INSANE using namespace Insane
#define IS_DEBUG InsaneIO::Insane::UtilityExtensions::IsDebug()
using namespace std::string_literals;
using namespace std::chrono_literals;
namespace InsaneIO::Insane
{
	// ███ Concepts ███
	template <typename T>
	concept HasOstream = requires(std::ostream & os, T t) {
		os << t;
	};

	template <typename T>
	concept PrintableAndEqualityComparable = HasOstream<T> && std::equality_comparable<T>;

	// ███ IClone ███

	template <typename T>
	class INSANE_API IClone {
	public:
		virtual std::unique_ptr<T> Clone() const = 0;
	};

	// ███ UtilityExtensions ███

	class INSANE_API UtilityExtensions {
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

}

#endif // !INSANE_BASE_H
