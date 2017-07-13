#pragma once
#include "Vector.h"
#include "Matrix.h"

namespace GMath
{
	class Quaternion
	{
	public:
		Quaternion();
		

		void SetRotation(vec3f rotationAxis, float rotationAngle);
		void Rotate(vec3f rotationAxis, float rotationAngle);

		void Normalize();
		mat4f ToMatrix();

		Quaternion operator*(Quaternion& RHS);
		void operator=(Quaternion& RHS);
		bool operator==(Quaternion& RHS) const ;


		float x, y, z, w;
	};

}
