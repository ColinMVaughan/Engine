#ifndef VOXEL_CONTAINTER_H
#define VOXEL_CONTAINTER_H

#include <PhysicsSystem.h>
#include <ECS.h>
#include <Renderer.h>


class Voxel
{
public:
	int x, y, z;
	int MaterialIndex = 0;
};

class VoxelContainer
{
public:
	void Initalize(int x, int y, int z)
	{

	}

private:
	struct ContainerSize { int x, y, z; };
	ContainerSize m_ContainerSize;

	Voxel voxels[10][10][10];
	physx::PxRigidStatic* m_Body;
	Material m_Material;
};



class VoxelRenderSystem: public System<VoxelContainer, Transform>
{
public:
	void PreUpdate(double deltaTime) override
	{
		if (m_Renderer == nullptr)
			std::cout << "";



	}
	void Update(double deltaTime, unsigned int entity) override
	{
		m_Renderer->RenderInstanced();
	}
	void PostUpdate(double deltaTime) override
	{

	}

private:
	Renderer* m_Renderer;
};
#endif