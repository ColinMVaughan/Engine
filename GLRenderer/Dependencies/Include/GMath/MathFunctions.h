//Colin Vaughan 
//December 2016

#pragma once
#include "Matrix.h"
#include "Vector.h"
#include "Quaternion.h"

//#define PI 3.14159265358979323846f

namespace GMath
{

	void SetIdentity(mat4f& matrix);
	inline void SetZero(mat4f& matrix);
	mat4f GetInverse(mat4f matrix);

	mat4f GetMat4Identity();

	void SetFrustum(mat4f& camera, float l, float r, float b, float t, float n, float f);
	void LookAt(mat4f& camera, vec3f position, vec3f target, vec3f direction);
	void SetFrustumProjection(mat4f& camera, float fov, float aspect, float near, float far);
	void SetOrthographic(mat4f& matrix, float top, float bottom, float left, float right, float near, float far);

	void SetRotate(mat4f& matrix, Quaternion rotation);
	void Translate(mat4f& matrix, vec3f translate);

	void SetScale(mat4f& matrix, vec3f scale);

	void RotateX(mat4f& matrix, float rotation);
	void RotateY(mat4f& matrix, float rotation);
	void RotateZ(mat4f& matrix, float rotation);



	template<class T>
	//- clamps data to the range [min, max]
	T Clamp(const T &data, const T &min, const T &max)
	{
		if (data > max)
		{
			return max;
		}
		else if (data < min)
		{
			return min;
		}
		else
		{
			return data;
		}
	}

	//Linear interpolation
	template<class T>
	T LERP(const T &data1, const T &data2, float u)
	{
		return data1 * (1.0f - u) + data2 * u;
	}
}

