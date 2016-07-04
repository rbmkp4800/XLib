#pragma once

#include "XLib.Types.h"
#include "XLib.Util.h"
#include "XLib.Vectors.h"
#include "XLib.Math.h"
#include "XLib.Vectors.Math.h"

class Matrix4x4
{
public:
	float32 data[4][4];

	inline float32* operator [] (uint32 row) { return data[row]; }
	inline const float32* operator [] (uint32 row) const { return data[row]; }

	inline void clear()
	{
		data[0][0] = 0.0f;
		data[0][1] = 0.0f;
		data[0][2] = 0.0f;
		data[0][3] = 0.0f;

		data[1][0] = 0.0f;
		data[1][1] = 0.0f;
		data[1][2] = 0.0f;
		data[1][3] = 0.0f;

		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
		data[2][2] = 0.0f;
		data[2][3] = 0.0f;

		data[3][0] = 0.0f;
		data[3][1] = 0.0f;
		data[3][2] = 0.0f;
		data[3][3] = 0.0f;
	}
	inline void identity()
	{
		data[0][0] = 1.0f;
		data[0][1] = 0.0f;
		data[0][2] = 0.0f;
		data[0][3] = 0.0f;

		data[1][0] = 0.0f;
		data[1][1] = 1.0f;
		data[1][2] = 0.0f;
		data[1][3] = 0.0f;

		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
		data[2][2] = 1.0f;
		data[2][3] = 0.0f;

		data[3][0] = 0.0f;
		data[3][1] = 0.0f;
		data[3][2] = 0.0f;
		data[3][3] = 1.0f;
	}
	inline void transpose()
	{
		for (uint32 i = 0; i < 4; i++)
			for (uint32 j = 0; j < 4; j++)
				swap(data[i][j], data[j][i]);
	}
	inline void translation(float32 x, float32 y, float32 z)
	{
		data[0][0] = 1.0f;
		data[0][1] = 0.0f;
		data[0][2] = 0.0f;
		data[0][3] = 0.0f;

		data[1][0] = 0.0f;
		data[1][1] = 1.0f;
		data[1][2] = 0.0f;
		data[1][3] = 0.0f;

		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
		data[2][2] = 1.0f;
		data[2][3] = 0.0f;

		data[3][0] = x;
		data[3][1] = y;
		data[3][2] = z;
		data[3][3] = 1.0f;
	}
	inline void scale(float32 x, float32 y, float32 z)
	{
		data[0][0] = x;
		data[0][1] = 0.0f;
		data[0][2] = 0.0f;
		data[0][3] = 0.0f;

		data[1][0] = 0.0f;
		data[1][1] = y;
		data[1][2] = 0.0f;
		data[1][3] = 0.0f;

		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
		data[2][2] = z;
		data[2][3] = 0.0f;

		data[3][0] = 0.0f;
		data[3][1] = 0.0f;
		data[3][2] = 0.0f;
		data[3][3] = 1.0f;
	}
	inline void rotationX(float32 angle)
	{
		float32 sin = Math::Sin(angle), cos = Math::Cos(angle);
		data[0][0] = 1.0f;
		data[0][1] = 0.0f;
		data[0][2] = 0.0f;
		data[0][3] = 0.0f;

		data[1][0] = 0.0f;
		data[1][1] = cos;
		data[1][2] = -sin;
		data[1][3] = 0.0f;

		data[2][0] = 0.0f;
		data[2][1] = sin;
		data[2][2] = cos;
		data[2][3] = 0.0f;

		data[3][0] = 0.0f;
		data[3][1] = 0.0f;
		data[3][2] = 0.0f;
		data[3][3] = 1.0f;
	}
	inline void rotationY(float32 angle)
	{
		float32 sin = Math::Sin(angle), cos = Math::Cos(angle);
		data[0][0] = cos;
		data[0][1] = 0.0f;
		data[0][2] = sin;
		data[0][3] = 0.0f;

		data[1][0] = 0.0f;
		data[1][1] = 1.0f;
		data[1][2] = 0.0f;
		data[1][3] = 0.0f;

		data[2][0] = -sin;
		data[2][1] = 0.0f;
		data[2][2] = cos;
		data[2][3] = 0.0f;

		data[3][0] = 0.0f;
		data[3][1] = 0.0f;
		data[3][2] = 0.0f;
		data[3][3] = 1.0f;
	}
	inline void rotationZ(float32 angle)
	{
		float32 sin = Math::Sin(angle), cos = Math::Cos(angle);
		data[0][0] = cos;
		data[0][1] = -sin;
		data[0][2] = 0.0f;
		data[0][3] = 0.0f;

		data[1][0] = sin;
		data[1][1] = cos;
		data[1][2] = 0.0f;
		data[1][3] = 0.0f;

		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
		data[2][2] = 1.0f;
		data[2][3] = 0.0f;

		data[3][0] = 0.0f;
		data[3][1] = 0.0f;
		data[3][2] = 0.0f;
		data[3][3] = 1.0f;
	}
	inline void perspective(float32 fov, float32 aspect, float32 zNear, float32 zFar)
	{
		float32 yScale = 1.0f / Math::Tan(fov / 2.0f);
		float32 xScale = yScale / aspect;

		data[0][0] = xScale;
		data[0][1] = 0.0f;
		data[0][2] = 0.0f;
		data[0][3] = 0.0f;

		data[1][0] = 0.0f;
		data[1][1] = yScale;
		data[1][2] = 0.0f;
		data[1][3] = 0.0f;

		data[2][0] = 0.0f;
		data[2][1] = 0.0f;
		data[2][2] = zFar / (zFar - zNear);
		data[2][3] = 1.0f;

		data[3][0] = 0.0f;
		data[3][1] = 0.0f;
		data[3][2] = zNear * zFar / (zNear - zFar);
		data[3][3] = 0.0f;
	}
	inline void lookAtCentered(const float32x3& position, const float32x3& centeredTarget, const float32x3& up)
	{
		float32x3 zaxis = -VectorMath::Normalize(centeredTarget);
		float32x3 xaxis = VectorMath::Normalize(VectorMath::Cross(up, zaxis));
		float32x3 yaxis = VectorMath::Cross(zaxis, xaxis);

		data[0][0] = xaxis.x;
		data[0][1] = yaxis.x;
		data[0][2] = zaxis.x;
		data[0][3] = 0.0f;

		data[1][0] = xaxis.y;
		data[1][1] = yaxis.y;
		data[1][2] = zaxis.y;
		data[1][3] = 0.0f;

		data[2][0] = xaxis.z;
		data[2][1] = yaxis.z;
		data[2][2] = zaxis.z;
		data[2][3] = 0.0f;

		data[3][0] = VectorMath::Dot(xaxis, position);
		data[3][1] = VectorMath::Dot(yaxis, position);
		data[3][2] = VectorMath::Dot(zaxis, position);
		data[3][3] = 1.0f;
	}
	inline void lookAt(const float32x3& position, const float32x3& target, const float32x3& up)
	{
		lookAtCentered(position, target - position, up);
	}

	static inline Matrix4x4 Identity()
	{
		Matrix4x4 result;
		result.identity();
		return result;
	}
	static inline Matrix4x4 Transpose(const Matrix4x4& matrix)
	{
		Matrix4x4 result = matrix;
		result.transpose();
		return result;
	}
	static inline Matrix4x4 Translation(float32 x, float32 y, float32 z)
	{
		Matrix4x4 result;
		result.translation(x, y, z);
		return result;
	}
	static inline Matrix4x4 Translation(float32x3 vector)
	{
		Matrix4x4 result;
		result.translation(vector.x, vector.y, vector.z);
		return result;
	}
	static inline Matrix4x4 Scale(float32 x, float32 y, float32 z)
	{
		Matrix4x4 result;
		result.scale(x, y, z);
		return result;
	}
	static inline Matrix4x4 Scale(float32x3 vector)
	{
		Matrix4x4 result;
		result.scale(vector.x, vector.y, vector.z);
		return result;
	}
	static inline Matrix4x4 RotationX(float32 angle)
	{
		Matrix4x4 result;
		result.rotationX(angle);
		return result;
	}
	static inline Matrix4x4 RotationY(float32 angle)
	{
		Matrix4x4 result;
		result.rotationY(angle);
		return result;
	}
	static inline Matrix4x4 RotationZ(float32 angle)
	{
		Matrix4x4 result;
		result.rotationZ(angle);
		return result;
	}
	static inline Matrix4x4 Perspective(float32 fov, float32 aspect, float32 zNear, float32 zFar)
	{
		Matrix4x4 result;
		result.perspective(fov, aspect, zNear, zFar);
		return result;
	}
	static inline Matrix4x4 LookAt(const float32x3& position, const float32x3& target, const float32x3& up)
	{
		Matrix4x4 result;
		result.lookAt(position, target, up);
		return result;
	}
	static inline Matrix4x4 LookAtCentered(const float32x3& position, const float32x3& centerdTarget, const float32x3& up)
	{
		Matrix4x4 result;
		result.lookAtCentered(position, centerdTarget, up);
		return result;
	}
};

/*inline Matrix4x4 operator * (const Matrix4x4& matrix1, const Matrix4x4& matrix2)
{
	Matrix4x4 result;
	result.clear();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				result[i][j] += matrix1[i][k] * matrix2[k][j];
	return result;
}*/

inline Matrix4x4 operator * (const Matrix4x4& a, const Matrix4x4& b)
{
	Matrix4x4 result;
	result[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0];
	result[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1];
	result[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2];
	result[0][3] = a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3];

	result[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0];
	result[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1];
	result[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2];
	result[1][3] = a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3];

	result[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0];
	result[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1];
	result[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2];
	result[2][3] = a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3];

	result[3][0] = a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0];
	result[3][1] = a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1];
	result[3][2] = a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2];
	result[3][3] = a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3];
	return result;
}

inline float32x4 operator * (const float32x3& vector, const Matrix4x4& matrix)
{
	return float32x4(
		vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + matrix[3][0],
		vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + matrix[3][1],
		vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + matrix[3][2],
		vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + matrix[3][3]);
}
inline float32x4 operator * (const float32x4& vector, const Matrix4x4& matrix)
{
	return float32x4(
		vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + vector.w * matrix[3][0],
		vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + vector.w * matrix[3][1],
		vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + vector.w * matrix[3][2],
		vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + vector.w * matrix[3][3]);
}