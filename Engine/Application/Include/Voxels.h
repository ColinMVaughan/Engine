#ifndef VOXEL_CONTAINTER_H
#define VOXEL_CONTAINTER_H

#include <PhysicsSystem.h>
#include <Entity.h>
#include <ECS.h>
#include <ComponentReflection.h>

#include <Renderer.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//--------------------------------------------------------------------------------------
//									VOXELS
//--------------------------------------------------------------------------------------

class VoxelContainer
{
public:

	void ReadQubicBinaryFile(std::string file, Mesh* mesh);
	void ReadVoxFile(std::string File, Mesh* mesh);

	void ExposeToEditor();
	
	std::vector<glm::mat4> m_Matricies;
	std::vector<glm::vec2> m_UVs;

	bool dynamic = false;

private:
	void ConstructVoxelMesh(Uint32* VoxelMatrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ);
	void ConstructVoxelMaterial();

	Material m_Material;
	Mesh m_Mesh;

};
COMPONENT_REGISTER(VoxelContainer, "VoxelContainer")




//--------------------------------------------------------
//					Revamp Voxel Container
//
//
//-------------------------------------------------------
class NewVoxelContainer
{

};


#endif