#pragma once
#include<BaseSystem.h>
#include <Renderer.h>

class Transform
{
public:

	void SetPosition();
	void SetRotation();
	void SetScale();

private:
	glm::fmat4 m_Matrix;
};

