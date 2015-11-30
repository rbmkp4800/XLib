#pragma once

#include "XLib.Types.h"

namespace _Private
{
	template <uint32 size>
	class _AtomicBase;

	template <>
	class _AtomicBase<sizeof(uint32)>
	{
	protected:
		static uint32 _increment(volatile void* ptr);
	};
}

template <typename Type>
class Atomic : public _Private::_AtomicBase<sizeof(Type)>
{
private:
	volatile Type value;

public:
	Atomic() = default;
	inline Atomic(Type _value) : value(_value) {}

	inline Type increment() { return _increment(&value); }
	inline Type load() { return value; /*LOL*/ }
	inline void store(const Type& _value) { value = _value; /*LOL*/ }

	inline Type get() { return value; }
	inline void set(const Type& _value) { value = _value; }
};