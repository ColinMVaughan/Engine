#pragma once 
#include <vector>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <cereal\types\string.hpp>


#include <list>




class Mesh
{
public:
	
	typedef glm::fvec3 vec3;
	typedef glm::fvec2 vec2;
	typedef glm::mat4 mat4;
public:
	Mesh();
	Mesh(const std::string& file)
	{
		LoadFromFile(file);
	}
	~Mesh();

	bool LoadFromFile(const std::string &file);
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVertices() const;


	bool SetInstancing(vec3* dataBuffer, size_t bufferSize);
	bool SetInstancing(mat4* dataBuffer, size_t bufferSize);


	inline void BufferSubData(GLuint targetBuffer, GLintptr offset, GLsizeiptr size, const void* data)
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, targetBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void ExposeToEditor();

	template<typename Archive>
	void save(Archive& arc) const
	{
		arc(Filepath);
	}

	template<typename Archive>
	void load(Archive& arc)
	{
		arc(Filepath);
		LoadFromFile(Filepath);
	}


	GLuint VBO_Verticies = 0;
	GLuint VBO_UVs		 = 0;
	GLuint VBO_Normals	 = 0;
	GLuint VBO_Instance  = 0;
	GLuint VAO			 = 0;
	GLuint EBO			 = 0;

	bool IsInstanced = false;
	unsigned int InstanceNumber = 0;
	std::string Filepath;

	unsigned int _NumVertices = 0;
	unsigned int _NumFaces = 0;

private:



};


//-------------------------------------------------------
//
//-------------------------------------------------------

class SkinnedMesh : public Mesh
{
public:
	SkinnedMesh() = default;
	SkinnedMesh(std::string filePath);
};


class Joint
{
public:

	Joint(int index, std::string name, const glm::mat4& bindLocalTransform)
		:m_Index(index), m_Name(name), m_LocalBindTransform(bindLocalTransform) {}

	void AddChild(Joint child)
	{
		m_ChildJoints.push_back(child);
		return;
	}

	glm::mat4 GetAnimatedTransform() { return m_AnimatedTransform; }
	void SetAnimationTransform(const glm::mat4& animationTransform) { m_AnimatedTransform = animationTransform; }
	glm::mat4 GetInverseBindTransform() { return m_InverseBindTransform; }


	void calcInverseBindTransform(const glm::mat4& parentBindTransform)
	{
		glm::mat4 bindTransform = parentBindTransform * m_LocalBindTransform;
		m_InverseBindTransform = glm::inverse(bindTransform);

		for (auto& child : m_ChildJoints)
		{
			child.calcInverseBindTransform(bindTransform);
		}
	}

private:
	int m_Index;
	std::string m_Name;

	glm::mat4 m_AnimatedTransform;
	glm::mat4 m_LocalBindTransform;
	glm::mat4 m_InverseBindTransform;

	std::list<Joint> m_ChildJoints;
};


class Skeleton
{
public:
	
	void ConstructDummySkeleton()
	{
	}

private:
	Joint RootJoint;
	unsigned int jointCount = 0;
};

class Animator
{

};