#include <GeKo_Graphics/Geometry/Skybox.h>

Skybox::Skybox(const char **fileNames)
{
	m_points = 36;
	m_indices = 36;
	setIndexFalse();
	setNormalsTrue();
	setUVTrue();

	GLfloat vertices[] = {
		// Front face
		-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0,
		// Right face
		1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0,
		1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0,
		// Back face
		-1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0,
		1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0,
		// Left face
		-1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0,
		-1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0,
		// Bottom face   
		-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0,
		1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0,
		// Top Face
		-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0,
		1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0
	};

	GLfloat normals[] = {
		// Front face
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		// Right face
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		// Back face
		0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
		0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
		// Left face
		-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
		// Bottom face
		0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
		0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
		// Top face
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0
	};

	GLfloat texCoords[] = {
		0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0
	};

	for (int i = 0; i<m_points; i++)
	{
		m_vertices.push_back(glm::vec4(vertices[i * 3] * 0.5, vertices[i * 3 + 1] * 0.5, vertices[i * 3 + 2] * 0.5, 1.0f));
		m_normals.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
		m_uvs.push_back(glm::vec2(texCoords[i * 2], texCoords[i * 2 + 1]));
	}

	for (int i = 0; i < m_indices; i++)
	{
		m_index.push_back(i);
	}

	Init(fileNames);
}

Skybox::~Skybox()
{
	m_vertices.clear();
	m_normals.clear();
	m_uvs.clear();
	m_index.clear();
}

bool Skybox::Init(const char **fileNames)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	load(fileNames);

	return true;
}

#include <stb_image.h>
bool Skybox::load(const char **fileNames)
{
	GLint types[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (unsigned int i = 0; i<6; i++)
	{
		const char* fileName = fileNames[i];

		int bytesPerPixel = 0;

		unsigned char *data = stbi_load(fileName, &m_width, &m_height, &bytesPerPixel, 0);

		//send image data to the new texture
		GLint internalFormat;
		if (bytesPerPixel < 3)
		{
			printf("ERROR: Unable to load texture image %s\n", fileName);
			return false;
		}
		else if (bytesPerPixel == 3)
		{
			internalFormat = GL_RGB;
		}
		else if (bytesPerPixel == 4)
		{
			internalFormat = GL_RGBA;
		}
		else
		{
			printf("RESOLVED: Unknown format for bytes per pixel in texture image %s, changed to 4\n", fileName);
			internalFormat = GL_RGBA;
		}

		glTexImage2D(types[i], 0, internalFormat, m_width, m_height, 0, internalFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		//printf("SUCCESS: Texture image %s loaded\n", fileName);
	}
	return true;
}

unsigned int Skybox::getSkyboxTexture()
{
	return m_textureID;
}