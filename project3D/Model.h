#pragma once
#include <Texture.h>
#include <gl_core_4_4.h>
#include "tiny_obj_loader.h"
#include <glm\ext.hpp>

struct OBJVertex
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_faceCount;
};

class Model
{
private:
	aie::Texture* m_tex;
	tinyobj::attrib_t m_attribs;
	std::vector<tinyobj::shape_t> m_shapes;
	std::vector<tinyobj::material_t> m_materials;

	std::vector<OpenGLInfo> m_glInfo;
public:
	Model();
	~Model();

	void Load(const char* name);

	unsigned int LoadNewTexture(char * path);
	void SetTexture(aie::Texture * tex);
	aie::Texture* GetTexture();
	unsigned int GetTextureHandle();

	void CreateBuffers(float scale);
	void Draw(glm::mat4 transform, glm::mat4 cameraMatrix, unsigned int programID);

};

