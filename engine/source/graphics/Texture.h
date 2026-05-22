#pragma once

#include <GL/glew.h>

namespace eng
{
	class Texture
	{
	public:
		Texture(int witdh, int height, int numChannels, unsigned char* data);
		~Texture();
		GLuint GetId() const;

	private:
		int m_width = 0;
		int m_height = 0;
		int m_numChannels = 0;
		GLuint m_textureID = 0;
	};
}