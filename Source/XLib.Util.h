#pragma once

#include "XLib.Types.h"

template <typename type, uintptr size> constexpr uintptr countof(type(&)[size]) { return size; }

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
void swap(type& object1, type& object2)
{
	type tmp(move(object1));
	object1 = move(object2);
	object2 = move(tmp);
}

#undef min
#undef max

template <typename type> constexpr inline type sqr(type val) { return val * val; }
template <typename type> constexpr inline type abs(type val) { return val >= type(0) ? val : -val; }
template <typename type> constexpr inline type min(type val1, type val2) { return val1 < val2 ? val1 : val2; }
template <typename type> constexpr inline type max(type val1, type val2) { return val1 > val2 ? val1 : val2; }
//template <typename type> static constexpr inline type divceil(type val, type divider) { return (val - 1) / divider + 1; }

template <typename type>
constexpr inline type clamp(type val, type _min, type _max)
{
	if (val < _min) val = _min;
	if (val > _max) val = _max;
	return val;
}
template <typename type> constexpr inline type saturate(type val) { return clamp(val, type(0), type(1)); }
template <typename type> constexpr inline type lincoef(type left, type right, type x) { return (left - x) / (left - right); }
template <typename vectorType, typename scalarType> constexpr inline vectorType lerp(const vectorType& x, const vectorType& y, scalarType coef) { return x + coef * (y - x); }