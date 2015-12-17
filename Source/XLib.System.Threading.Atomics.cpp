#include <Windows.h>

#include "XLib.System.Threading.Atomics.h"

uint32 Atomics::Core<sizeof(uint32)>::Add(volatile uint32* target, uint32 value) { return _InterlockedAdd((volatile LONG*)target, value); }
uint32 Atomics::Core<sizeof(uint32)>::Sub(volatile uint32* target, uint32 value) { return _InterlockedAdd((volatile LONG*)target, -sint32(value)); }
uint32 Atomics::Core<sizeof(uint32)>::Exchange(volatile uint32* target, uint32 value) { return _InterlockedExchange(target, value); }
uint32 Atomics::Core<sizeof(uint32)>::Increment(volatile uint32* target) { return _InterlockedIncrement(target); }
uint32 Atomics::Core<sizeof(uint32)>::Decrement(volatile uint32* target) { return _InterlockedDecrement(target); }
uint32 Atomics::Core<sizeof(uint32)>::CompareExchange(volatile uint32* target, uint32 exchange, uint32 comparand) { return _InterlockedCompareExchange(target, exchange, comparand); }
uint32 Atomics::Core<sizeof(uint32)>::And(volatile uint32* target, uint32 value) { return _InterlockedAnd((volatile LONG*)target, value); }
uint32 Atomics::Core<sizeof(uint32)>::Or(volatile uint32* target, uint32 value) { return _InterlockedOr((volatile LONG*)target, value); }
uint32 Atomics::Core<sizeof(uint32)>::Xor(volatile uint32* target, uint32 value) { return _InterlockedXor((volatile LONG*)target, value); }
//uint32 Atomics::Core<sizeof(uint32)>::Load(volatile uint32* target) { return *target; }
//void Atomics::Store(volatile uint32* target, uint32 value) {}