#include "Model.h"

Model::Model()
{
}


Model::~Model()
{
}

void Model::Load(const char * name)
{
	std::string err;
	tinyobj::LoadObj(&m_attribs, &m_shapes, &m_materials, &err, name);
}

unsigned int Model::LoadNewTexture(char* path)
{
	m_tex = new aie::Texture();
	m_tex->load(path);
	m_tex->getHeight();
	return m_tex->getHandle();
}

void Model::SetTexture(aie::Texture* tex)
{
	m_tex = tex;
}

void Model::CreateBuffers(float scale)
{
	m_glInfo.resize(m_shapes.size());
	int shapeIndex = 0;
	for (auto& shape : m_shapes)
	{
		// setup OpenGL data
		glGenVertexArrays(1, &m_glInfo[shapeIndex].m_VAO);
		glGenBuffers(1, &m_glInfo[shapeIndex].m_VBO);
		glBindVertexArray(m_glInfo[shapeIndex].m_VAO);
		m_glInfo[shapeIndex].m_faceCount = shape.mesh.num_face_vertices.size();

		// collect triangle vertices
		std::vector<OBJVertex> vertices;
		int index = 0;
		for (auto face : shape.mesh.num_face_vertices)
		{
			for (int i = 0; i < 3; i++)
			{
				tinyobj::index_t idx = shape.mesh.indices[index + i];

				OBJVertex v = { 0 };
				// positions
				v.x = m_attribs.vertices[3 * idx.vertex_index + 0] * scale;
				v.y = m_attribs.vertices[3 * idx.vertex_index + 1] * scale;
				v.z = m_attribs.vertices[3 * idx.vertex_index + 2] * scale;

				// normals
				if (m_attribs.normals.size() > 0)
				{
					v.nx = m_attribs.normals[3 * idx.normal_index + 0];
					v.ny = m_attribs.normals[3 * idx.normal_index + 1];
					v.nz = m_attribs.normals[3 * idx.normal_index + 2];
				}

				// texture coordinates
				if (m_attribs.texcoords.size() > 0)
				{
					v.u = m_attribs.texcoords[2 * idx.texcoord_index + 0];
					v.v = m_attribs.texcoords[2 * idx.texcoord_index + 1];
				}

				vertices.push_back(v);
			}
			index += face;
		}

		// bind vertex data
		glBindBuffer(GL_ARRAY_BUFFER, m_glInfo[shapeIndex].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(OBJVertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), 0);
		glEnableVertexAttribArray(1); // normal data
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), (void*)12);
		glEnableVertexAttribArray(2); // texture data
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void*)24);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		shapeIndex++;
	}
}

void Model::Draw(glm::mat4 transform, glm::mat4 cameraMatrix, unsigned int programID)
{
	glUseProgram(programID);

	// Get view-projection matrix
	glm::mat4 mvp = cameraMatrix * transform;

	// Pass in projection-view matrix
	unsigned int projectionViewUniform = glGetUniformLocation(programID, "MVP");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(mvp));

	// Pass in model uniform
	unsigned int modelUniform = glGetUniformLocation(programID, "M");
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, (float*)&transform);

	// Pass in texture handle
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tex->getHandle());
	unsigned int diffuseUniform = glGetUniformLocation(programID, "diffuse");
	glUniform1i(diffuseUniform, 0);

	// Draw the model on the screen
	for (unsigned int i = 0; i < m_glInfo.size(); i++)
	{
		glBindVertexArray(m_glInfo[i].m_VAO);
		//glDrawElements(GL_TRIANGLES, m_glInfo[i].m_faceCount, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, m_glInfo[i].m_faceCount * 3);
	}
}

aie::Texture* Model::GetTexture()
{
	return m_tex;
}

unsigned int Model::GetTextureHandle()
{
	return m_tex->getHandle();
}
