#pragma once

#ifndef CORE_BASE_COMMON_H_
#define CORE_BASE_COMMON_H_

#define DEBUG_BREAK assert(false)

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <algorithm>
#include <fstream>
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <type_traits>
#include <array>
#include <set>
#include <utility>
#include <limits>
#include <tuple>
#include <complex>
#include <queue> 
#include <random>
#include <iomanip>
#include <assert.h>


namespace boost {
namespace serialization {

class access;

}
}

namespace ml
{

#define __FUNCTION__ __func__
#ifndef __LINE__
#define __LINE__
#endif

#define NOEXCEPT noexcept

class MLibException : public std::exception {
public:
	MLibException(const std::string& what) : std::exception() {
		m_msg = what;
	}
	MLibException(const char* what) : std::exception() {
		m_msg = std::string(what);
	}
	const char* what() const NOEXCEPT {
		return m_msg.c_str();
	}
private:
	std::string m_msg;
};


#define FUNCTION_LINE_STRING (std::string(__FUNCTION__) + ":" + std::to_string(__LINE__))

#ifndef MLIB_EXCEPTION
#define MLIB_EXCEPTION(s) ml::MLibException(std::string(__FUNCTION__).append(":").append(std::to_string(__LINE__)).append(": ").append(s).c_str())
#endif

#ifndef MLIB_QUIET
#define MLIB_WARNING(s) ml::warningFunctionMLIB(std::string(FUNCTION_LINE_STRING) + std::string() + ": " + std::string(s))
void warningFunctionMLIB(const std::string &description);
#else
#define MLIB_WARNING(s)
#endif

#define MLIB_ERROR(s) ml::errorFunctionMLIB(std::string(FUNCTION_LINE_STRING) + ": " + std::string(s))
void errorFunctionMLIB(const std::string &description);

#if defined(DEBUG) || defined(_DEBUG)
#define MLIB_ASSERT_STR(b,s) { if(!(b)) ml::assertFunctionMLIB(b, std::string(FUNCTION_LINE_STRING) + ": " + std::string(s)); }
#define MLIB_ASSERT(b) { if(!(b)) ml::assertFunctionMLIB(b, FUNCTION_LINE_STRING); }
void assertFunctionMLIB(bool statement, const std::string &description);
#else
#define MLIB_ASSERT_STR(b,s)
#define MLIB_ASSERT(b)
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; } }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p) { if (p) { free (p);   (p)=nullptr; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if (p) { p->Release();   (p)=nullptr; } }
#endif

}  // namespace ml


#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef INT64
typedef int64_t INT64;
#endif

#ifndef UINT32
typedef uint32_t UINT32;
#endif

#ifndef UINT64
typedef uint64_t UINT64;
#endif

#ifndef FLOAT
typedef float FLOAT;
#endif

#ifndef DOUBLE
typedef double DOUBLE;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef USHORT
typedef unsigned short USHORT;
#endif

#ifndef sint
typedef signed int sint;
#endif

#ifndef uint
typedef unsigned int uint;
#endif 

#ifndef slong 
typedef signed long slong;
#endif

#ifndef ulong
typedef unsigned long ulong;
#endif

#ifndef uchar
typedef unsigned char uchar;
#endif

#ifndef schar
typedef signed char schar;
#endif

#endif  // CORE_BASE_COMMON_H_
