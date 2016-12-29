#pragma once

#include "XLib.Types.h"

class CRC8
{
private:
	uint8 value = 0;

public:
	static uint8 Compute(const void* data, uintptr size, uint8 seed = 0);

	inline void update(const void* data, uintptr size) { value = Compute(data, size, value); }
	inline uint8 getValue() { return value; }
	inline void reset() { value = 0; }

	template <typename Type>
	inline void update(const Type& data) { update(&data, sizeof(data)); }
	template <typename Type>
	static inline uint8 Compute(const Type& data) { return Compute(&data, sizeof(data)); }
};

class CRC16
{
private:
	uint16 value = 0;

public:
	static uint16 Compute(const void* data, uintptr size, uint16 seed = 0);

	inline void update(const void* data, uintptr size) { value = Compute(data, size, value); }
	inline uint16 getValue() { return value; }
	inline void reset() { value = 0; }

	template <typename Type>
	inline void update(const Type& data) { update(&data, sizeof(data)); }
	template <typename Type>
	static inline uint16 Compute(const Type& data) { return Compute(&data, sizeof(data)); }
};