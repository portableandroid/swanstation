#pragma once
#include "types.h"

#ifdef _WIN32
#include <intrin.h>
#endif

/// Returns the number of zero bits before the first set bit, going MSB->LSB.
template<typename T>
ALWAYS_INLINE unsigned CountLeadingZeros(T value)
{
#ifdef _WIN32
  unsigned long index;
  if constexpr (sizeof(value) >= sizeof(u64))
    _BitScanReverse64(&index, ZeroExtend64(value));
  else
    _BitScanReverse(&index, ZeroExtend32(value));
  return static_cast<unsigned>(index) ^ static_cast<unsigned>((sizeof(value) * 8u) - 1u);
#else
  if constexpr (sizeof(value) >= sizeof(u64))
    return static_cast<unsigned>(__builtin_clzl(ZeroExtend64(value)));
  else if constexpr (sizeof(value) == sizeof(u32))
    return static_cast<unsigned>(__builtin_clz(ZeroExtend32(value)));
  return static_cast<unsigned>(__builtin_clz(ZeroExtend32(value))) & static_cast<unsigned>((sizeof(value) * 8u) - 1u);
#endif
}

/// Returns the number of zero bits before the first set bit, going LSB->MSB.
template<typename T>
ALWAYS_INLINE unsigned CountTrailingZeros(T value)
{
#ifdef _WIN32
  unsigned long index;
  if constexpr (sizeof(value) >= sizeof(u64))
    _BitScanForward64(&index, ZeroExtend64(value));
  else
    _BitScanForward(&index, ZeroExtend32(value));
  return index;
#else
  if constexpr (sizeof(value) >= sizeof(u64))
    return static_cast<unsigned>(__builtin_ctzl(ZeroExtend64(value)));
  return static_cast<unsigned>(__builtin_ctz(ZeroExtend32(value)));
#endif
}
