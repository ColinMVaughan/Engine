#ifndef _VOXEL_RENDER_SYSTEM_H_
#define _VOXEL_RENDER_SYSTEM_H_

#include "Renderer.h"
#include "BaseSystem.h"
#include "ECS.h"
#include "PhysicsSystem.h"
#include <tuple>
#include <ComponentReflection.h>
#include <AssetRegistration.h>
#include <Asset.h>


//--------------------------------------------------------
//
//------------------------------------------------------
class VoxelContainer
{
public:
	VoxelContainer(std::string file) {ReadVoxFile(file);}
	VoxelContainer() = default;

	void ReadQubicBinaryFile(std::string file);
	void ReadVoxFile(std::string File);

	bool ReadChunk(char* data, Uint32& offset);

	std::vector<glm::mat4> m_Matricies;
	std::vector<unsigned int> m_MaterialIndex;
	std::vector<glm::vec3> m_Palette;

	unsigned int m_Width;
	unsigned int m_Depth;
	unsigned int m_Height;
	unsigned int m_maxMaterialIndex = 0;

private:
	void ConstructVoxelMesh(Uint32* VoxelMatrix, Mesh* mesh, size_t sizeX, size_t sizeY, size_t sizeZ);
	void ConstructVoxelMaterial();
};
REGISTER_ASSET(".vox", "VoxelContainer", VoxelContainer)


//------------------------------------------------------------------------------------
//							Voxel Mesh
//
//------------------------------------------------------------------------------------
class VoxelMesh
{
public:
	void ExposeToEditor()
	{
		//If the user attaches a new voxel container, clear the current material properties and set new ones
		ImGui::Text("Voxel Material");
		if (EditorRequestAsset<VoxelContainer>(m_VoxelContainer, "VoxelContainer", "Voxel Container: "))
		{
			m_LightProperties.clear();
			for (int i = 0; i < m_VoxelContainer.m_Asset.m_MaterialIndex.size(); ++i)
			{
				m_LightProperties.push_back(std::make_pair<glm::vec3, glm::vec3>(glm::vec3(0), glm::vec3(0)));
			}

			GenerateMaterialTexture();
		}

		bool changedMaterial = false; // set flag for if the material changes from the editor
		for(int i=0; i< m_LightProperties.size(); ++i)
		{
			glm::vec3* colour = &std::get<0>(m_LightProperties[i]);
			glm::vec3* light = &std::get<1>(m_LightProperties[i]);

			changedMaterial = changedMaterial || ImGui::ColorEdit3(std::string("Colour" + std::to_string(i)).c_str(), &colour->x);
			if(ImGui::TreeNode(std::string("Lighting Properties" + std::to_string(i)).c_str()))
			{
				changedMaterial = changedMaterial || ImGui::SliderFloat(std::string("Metallic"+ std::to_string(i)).c_str(), &light->x, 0.0f, 1.0f);
				changedMaterial = changedMaterial || ImGui::SliderFloat(std::string("Roughness"+ std::to_string(i)).c_str(), &light->y, 0.0f, 1.0f);
				changedMaterial = changedMaterial || ImGui::SliderFloat(std::string("Emissive"+ std::to_string(i)).c_str(), &light->z, 0.0f, 1.0f);
				ImGui::TreePop();
			}

			ImGui::Separator();
		}

		//if the material has changed, we must generate a new material texture for it to take effect.
		if (changedMaterial)
			GenerateMaterialTexture();
	}

	//Generates the material texture that will be accessed by the voxel mesh.
	//Material info is acces though a texel fetch using the voxel's material index.
	bool GenerateMaterialTexture()
	{
		if(m_VoxelMaterial.TexObj == 0)
			glGenTextures(1, &m_VoxelMaterial.TexObj);

		glBindTexture(GL_TEXTURE_2D, m_VoxelMaterial.TexObj);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 2, m_LightProperties.size(), 0, GL_RGB, GL_FLOAT, &std::get<0>(m_LightProperties[0]).x);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //U axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); //V axis

		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}

	//Material properties, first is colour(r,g,b), second is metallic,roughness,emissive(x,y,z)
	std::vector<std::tuple<glm::vec3, glm::vec3>> m_LightProperties;

	//Position and material index data for voxels.
	Asset<VoxelContainer> m_VoxelContainer;

	//Mesh holding the instanced voxels
	Mesh m_VoxelMesh;

	//Texture holding material data
	Texture m_VoxelMaterial;
};
COMPONENT_REGISTER(VoxelMesh, "VoxelMesh")



//-------------------------------------------------------------------------------------
class VoxelRenderSystem : public ECS::System<VoxelMesh, Transform>
{
private:
	Renderer * m_Renderer;
public:

	VoxelRenderSystem(ECS::ComponentManager* a_Cmanager, EventManager& a_eManager)
		:System(a_Cmanager, a_eManager)
	{}

	Renderer* GetRenderer()
	{
		return m_Renderer;
	}

	//THIS IS BAD! LETS FIX THIS.
	void SetRenderer(Renderer* a_renderer)
	{
		if (a_renderer != nullptr)
			m_Renderer = a_renderer;
		else
			std::cout << "\nDid not send RenderSystem a valid Renderer";
	}

	void PreUpdate(double deltaTime) override
	{
		m_Renderer->PreRender();
	}

	void Update(double deltaTime, ECS::Entity& entity) override
	{
		auto mat = entity.GetComponent<Transform>()->GetGlobalTransformMatrix();
		auto voxel = entity.GetComponent<VoxelMesh>();

		//Render
		m_Renderer->RenderVoxel(&voxel->m_VoxelMesh, &voxel->m_VoxelMaterial, mat.front());

	}
	void PostUpdate(double deltaTime) override
	{
		m_Renderer->PointLightPass();
		m_Renderer->SSAOPass();
		m_Renderer->CombineLighting();
		//Post Processing pass
		m_Renderer->CombineDebug();
		m_Renderer->CombineUI();
		m_Renderer->SubmitFrame();
	}

};





#endif