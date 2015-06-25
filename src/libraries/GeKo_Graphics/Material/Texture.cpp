#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(char* fileName)
{
	m_textureID = INVALID_OGL_VALUE;

	//save path without RESSOURCES_PATH
	std::string str(fileName);
	std::string delimiter = std::string(RESOURCES_PATH);
	str = str.substr(str.find(delimiter) + delimiter.length(), str.length());
	char* ch = new char[str.length() + 1];
	strcpy(ch, str.c_str());
	filepath = ch;

	load(fileName);
}

Texture::Texture(GLuint texture)
{
	setTexture(texture);
}

Texture::~Texture()
{
	if (m_textureID != INVALID_OGL_VALUE) glDeleteTextures(1, &m_textureID);
}

void Texture::use(GLenum texturePosition)
{
	glActiveTexture(texturePosition);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Texture::unUse()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

bool Texture::load(char* fileName)
{
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	int bytesPerPixel = 0;

	unsigned char* data = stbi_load(fileName, &m_width, &m_heigth, &bytesPerPixel, 0);

	//flip image vertically
	unsigned char* s = data;
	for (int y = 0; y<m_heigth / 2; y++)
	{
		unsigned char* e = data + (m_heigth - y - 1)*m_width*bytesPerPixel;
		for (int x = 0; x<m_width*bytesPerPixel; x++)
		{
			unsigned char temp = *s;
			*s = *e;
			*e = temp;
			s++;
			e++;
		}
	}

	//send image data to the new texture
	if (bytesPerPixel < 3)
	{
		printf("ERROR: Unable to load texture image %s\n", fileName);
		return false;
	}
	else if (bytesPerPixel == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (bytesPerPixel == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		printf("RESOLVED: Unknown format for bytes per pixel in texture image %s, changed to 4\n", fileName);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	stbi_image_free(data);
	glGenerateMipmap(GL_TEXTURE_2D);

	//printf("SUCCESS: Texture image %s loaded\n", filepath);

	unUse();

	return true;
}

void Texture::setTexture(GLuint texture)
{
	m_textureID = texture;
}

unsigned int Texture::getTexture()
{
	return m_textureID;
}

char* Texture::getFilepath()
{
	return filepath;
}