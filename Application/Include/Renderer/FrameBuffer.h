#pragma once
#include <GL/glew.h>

class FrameBuffer
{
public:
	FrameBuffer() = delete;
	FrameBuffer(unsigned numColourAttachments);
	~FrameBuffer();

	void InitDepthTexture(unsigned width, unsigned height);
	void InitColorTexture(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint Wrap);
	bool CheckFBO();

	//- Clears all opengl memory
	void Unload();

	//- Clears all attached textures
	void Clear();

	void Bind();
	void UnBind();

	void MoveToBackBuffer(int windowWidth, int windowHeight);

	GLuint GetDepthHandle() const;
	GLuint GetColorHandle(unsigned index) const;

private:

	GLuint _FBO = GL_NONE;
	GLuint _DepthAttachment = GL_NONE;
	GLuint *_ColorAttachments = nullptr;
	GLenum *_Buffs = nullptr;

	unsigned int _NumColorAttachments;
};