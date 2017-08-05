#pragma once
#include<BaseSystem.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

class Transform
{
public:

	glm::fvec3 Position;
	glm::fvec3 Scale;
	glm::fquat Rotation;



	//Builds a model transform matrix and returns it's const reference
	const glm::fmat4& GetMatrix()
	{
		m_Matrix = glm::mat4();
		m_Matrix = glm::scale(m_Matrix, Scale)* glm::mat4_cast(Rotation) * glm::translate(m_Matrix, Position);

		return m_Matrix;
	}

private:
	glm::fmat4 m_Matrix;

};

