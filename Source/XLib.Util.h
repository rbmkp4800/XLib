#pragma once

#include "XLib.Types.h"

template <typename type, uintptr size> constexpr uintptr elemcntof(type(&)[size]) { return size; }

template <typename type> struct RemoveReference abstract final { using Type = type; };
template <typename type> struct RemoveReference<type&> abstract final { using Type = type; };
template <typename type> struct RemoveReference<type&&> abstract final { using Type = type; };

template <typename type> typename RemoveReference<type>::Type&& move(type&& object) { return (typename RemoveReference<type>::Type&&)object; }

template <typename resultType, typename argumentType>
inline typename RemoveReference<resultType>::Type as(argumentType&& argument)
{
	static_assert(sizeof(resultType) == sizeof(argumentType), "Xlib.Util: as() function types must be same size");
	return *((resultType*)&argument);
}
template <typename resultType, typename argumentType>
inline typename RemoveReference<resultType>::Type& as(argumentType& argument)
{
	static_assert(sizeof(resultType) == sizeof(argumentType), "Xlib.Util: as() function types must be same size");
	return *((resultType*)&argument);
}

template <typename type>
void xorswap(type& value1, type& value2)
{
	value1 ^= value2;
	value2 ^= value1;
	value1 ^= value2;
}
template <typename type>
void swap(type& object1, type& object2)
{
	type tmp(move(object1));
	object1 = move(object2);
	object2 = move(tmp);
}

template <typename type> static constexpr inline type sqrval(type val) { return val * val; }
template <typename type> static constexpr inline type absval(type val) { return val >= type(0) ? val : -val; }
template <typename type> static constexpr inline type minval(type val1, type val2) { return val1 < val2 ? val1 : val2; }
template <typename type> static constexpr inline type maxval(type val1, type val2) { return val1 > val2 ? val1 : val2; }
template <typename type> static constexpr inline type divceil(type val, type divider) { return (val - 1) / divider + 1; }