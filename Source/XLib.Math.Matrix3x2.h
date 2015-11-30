#pragma once

#include "XLib.Types.h"
#include "XLib.Vectors.h"
#include "XLib.Math.h"

struct Matrix3x2
{
	float32 data[3][2];

	inline void clear()
	{
		data[0][0] = 0.0f;
		data[0][1] = 0.0f;
		data[1][0] = 0.0f;
		data[1][1] = 0.0f;
		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
	}
	inline void identity()
	{
		data[0][0] = 1.0f;
		data[0][1] = 0.0f;
		data[1][0] = 0.0f;
		data[1][1] = 1.0f;
		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
	}
	inline void translation(float32 x, float32 y)
	{
		data[0][0] = 1.0f;
		data[0][1] = 0.0f;
		data[1][0] = 0.0f;
		data[1][1] = 1.0f;
		data[2][0] = x;
		data[2][1] = y;
	}
	inline void scale(float32 scale)
	{
		data[0][0] = scale;
		data[0][1] = 0.0f;
		data[1][0] = 0.0f;
		data[1][1] = scale;
		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
	}
	inline void scale(float32 xscale, float32 yscale)
	{
		data[0][0] = xscale;
		data[0][1] = 0.0f;
		data[1][0] = 0.0f;
		data[1][1] = yscale;
		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
	}
	inline void scale(float32 xscale, float32 yscalse, float32 x, float32 y)
	{
		data[0][0] = xscale;
		data[0][1] = 0.0f;
		data[1][0] = 0.0f;
		data[1][1] = yscalse;
		data[2][0] = x * (1.0f - xscale);
		data[2][1] = y * (1.0f - yscalse);
	}
	inline void rotation(float32 angle)
	{
		float32 a = Math::Cos(angle);
		float32 b = Math::Sin(angle);
		data[0][0] = a;
		data[0][1] = -b;
		data[1][0] = b;
		data[1][1] = a;
		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
	}
	inline void rotation(float32 x, float32 y, float32 angle)
	{
		float32 a = Math::Cos(angle);
		float32 b = Math::Sin(angle);
		data[0][0] = a;
		data[0][1] = -b;
		data[1][0] = b;
		data[1][1] = a;
		data[2][0] = x * (1.0f - a) - b * y;
		data[2][1] = y * (1.0f - a) + b * x;
	}
	inline void verticalReflection()
	{
		data[0][0] = 1.0f;
		data[0][1] = 0.0f;
		data[1][0] = 0.0f;
		data[1][1] = -1.0f;
		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
	}
	inline void horizontalReflection()
	{
		data[0][0] = -1.0f;
		data[0][1] = 0.0f;
		data[1][0] = 0.0f;
		data[1][1] = 1.0f;
		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
	}
	inline void reflection(float32 x, float32 y)
	{
		float32 d = sqrval(x) + sqrval(y);
		float32 m = 2.0f * x * y / d;
		float32 s = (sqrval(x) - sqrval(y)) / d;

		data[0][0] = s;
		data[0][1] = m;
		data[1][0] = m;
		data[1][1] = -s;
		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
	}

	inline void translation(float32x2 transVector)
	{
		translation(transVector.x, transVector.y);
	}
	inline void scale(float32x2 scaleVector)
	{
		scale(scaleVector.x, scaleVector.y);
	}
	inline void scale(float32x2 scaleVector, float32x2 positionVector)
	{
		scale(scaleVector.x, scaleVector.y, positionVector.x, positionVector.y);
	}
	inline void rotation(float32x2 positionVector, float32 angle)
	{
		rotation(positionVector.x, positionVector.y, angle);
	}
	inline void reflection(float32x2 reflectionVector)
	{
		reflection(reflectionVector.x, reflectionVector.y);
	}

	static inline Matrix3x2 Identity()
	{
		Matrix3x2 matrix;
		matrix.identity();
		return matrix;
	}
	static inline Matrix3x2 Translation(float32 x, float32 y)
	{
		Matrix3x2 result;
		result.translation(x, y);
		return result;
	}
	static inline Matrix3x2 Translation(float32x2 translationVector)
	{
		Matrix3x2 result;
		result.translation(translationVector);
		return result;
	}
	static inline Matrix3x2 Scale(float32 scaleVector)
	{
		Matrix3x2 matrix;
		matrix.scale(scaleVector);
		return matrix;
	}
	static inline Matrix3x2 Scale(float32 x, float32 y)
	{
		Matrix3x2 matrix;
		matrix.scale(x, y);
		return matrix;
	}
	static inline Matrix3x2 Scale(float32x2 scaleVector)
	{
		Matrix3x2 matrix;
		matrix.scale(scaleVector);
		return matrix;
	}
	static inline Matrix3x2 Rotation(float32 angle)
	{
		Matrix3x2 matrix;
		matrix.rotation(angle);
		return matrix;
	}
	static inline Matrix3x2 Rotation(float32 x, float32 y, float32 angle)
	{
		Matrix3x2 matrix;
		matrix.rotation(x, y, angle);
		return matrix;
	}
	static inline Matrix3x2 Rotation(float32x2 position, float32 angle)
	{
		Matrix3x2 matrix;
		matrix.rotation(position, angle);
		return matrix;
	}
	static inline Matrix3x2 VerticalReflection()
	{
		Matrix3x2 matrix;
		matrix.verticalReflection();
		return matrix;
	}
	static inline Matrix3x2 HorizontalReflection()
	{
		Matrix3x2 matrix;
		matrix.horizontalReflection();
		return matrix;
	}
	static inline Matrix3x2 Reflection(float32 x, float32 y)
	{
		Matrix3x2 matrix;
		matrix.reflection(x, y);
		return matrix;
	}
	static inline Matrix3x2 Reflection(float32x2 reflectionVector)
	{
		Matrix3x2 matrix;
		matrix.reflection(reflectionVector);
		return matrix;
	}
};

inline float32x2 operator * (const float32x2& vector, const Matrix3x2& matrix)
{
	return float32x2(
		matrix.data[0][0] * vector.x + matrix.data[1][0] * vector.y + matrix.data[2][0],
		matrix.data[0][1] * vector.x + matrix.data[1][1] * vector.y + matrix.data[2][1]);
}
inline Matrix3x2 operator * (const Matrix3x2& a, const Matrix3x2& b)
{
	Matrix3x2 result;
	result.data[0][0] = a.data[0][0] * b.data[0][0] + a.data[1][0] * b.data[0][1];
	result.data[0][1] = a.data[0][1] * b.data[0][0] + a.data[1][1] * b.data[0][1];
	result.data[1][0] = a.data[0][0] * b.data[1][0] + a.data[1][0] * b.data[1][1];
	result.data[1][1] = a.data[0][1] * b.data[1][0] + a.data[1][1] * b.data[1][1];
	result.data[2][0] = a.data[0][0] * b.data[2][0] + a.data[1][0] * b.data[2][1] + a.data[2][0];
	result.data[2][1] = a.data[0][1] * b.data[2][0] + a.data[1][1] * b.data[2][1] + a.data[2][1];
	return result;
}