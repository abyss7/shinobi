#pragma once

#include <base/stl_include.hh>

#include STL(cstdint)
#include STL(list)
#include STL(memory)
#include STL(string)
#include STL(vector)

namespace shinobi {

using i8 = int8_t;
using ui8 = uint8_t;
using i16 = int16_t;
using ui16 = uint16_t;
using i32 = int32_t;
using ui32 = uint32_t;
using i64 = int64_t;
using ui64 = uint64_t;

template <class T>
using List = std::list<T>;

template <class U, class V = U>
using Pair = std::pair<U, V>;

template <class T>
using SharedPtr = std::shared_ptr<T>;

using String = std::string;

template <class T>
using UniquePtr = std::unique_ptr<T>;

template <class T>
using Vector = std::vector<T>;

}  // namespace shinobi
