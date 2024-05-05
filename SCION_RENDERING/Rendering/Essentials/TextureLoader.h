#pragma once
#include "Texture.h"
#include <memory>

namespace SCION_RENDERING {
	class TextureLoader
	{
	private:
		static bool LoadTexture(const std::string& filepath, GLuint& id, int& width, int& height, bool blended = false);
		static bool LoadFBTexture(GLuint& id, int& width, int& height);

	public:
		TextureLoader() = delete;

		/*
		* @brief Loads a new texture into open GL and creates a new Texture object
		* @params Takes in the type of texture to be created and a string for the filepath of the texture.
		* @return Returns a shared_ptr<Texture> if successful, nullptr otherwise.
		*/
		static std::shared_ptr<Texture> Create(Texture::TextureType type, const std::string& texturePath);
		static std::shared_ptr<Texture> Create(Texture::TextureType type, int width, int height);

		static std::shared_ptr<Texture> CreateFromMemory(const unsigned char* imageData, size_t length, bool blended = false);
	};
}