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

	void ReadQubicBinaryFile(std::string file, Mesh* mesh);

private:
	void ImportQB(Uint32* Matrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ);



	Voxel voxels[10][10][10];
	physx::PxRigidStatic* m_Body;


	Material m_Material;
	Mesh m_Mesh;
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