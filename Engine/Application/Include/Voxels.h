#ifndef VOXEL_CONTAINTER_H
#define VOXEL_CONTAINTER_H

#include <PhysicsSystem.h>
#include <Entity.h>
#include <ECS.h>
#include <ComponentReflection.h>

#include <Renderer.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "AssetRegistration.h"
#include <Asset.h>
#include <imgui.h>

//--------------------------------------------------------------------------------------
//									VOXELS
//--------------------------------------------------------------------------------------

class VoxelContainer
{
public:
	VoxelContainer(std::string file) {/* ReadQubicBinaryFile(file);*/ }
	VoxelContainer() = default;

	void ReadQubicBinaryFile(std::string file);
	void ReadVoxFile(std::string File, Mesh* mesh);
	
	std::vector<glm::mat4> m_Matricies;
	std::vector<unsigned int> m_MaterialIndex;

	unsigned int m_Width;
	unsigned int m_Depth;
	unsigned int m_Height;

private:
	void ConstructVoxelMesh(Uint32* VoxelMatrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ);
	void ConstructVoxelMaterial();
};
REGISTER_ASSET(".vox", "VoxelContainer", VoxelContainer)




//--------------------------------------------------------
//					Voxel Container Filter
//
//
//-------------------------------------------------------
class VoxelContainerFilter
{
public:
	VoxelContainerFilter() = default;
	void ExposeToEditor()
	{
		EDITOR_REQUEST_ASSET(VoxelContainer, m_VoxelContainer, "VoxelContainer")
	}

	COMPONENT_SERIALIZE(m_VoxelContainer)
	SERIALIZE_ASSET(m_VoxelContainer)

	Asset<VoxelContainer> m_VoxelContainer;
	bool dynamic = false;
};
COMPONENT_REGISTER(VoxelContainerFilter, "VoxelContainerFilter")




#endif