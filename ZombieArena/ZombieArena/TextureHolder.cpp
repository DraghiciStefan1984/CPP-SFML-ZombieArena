#include "pch.h"
#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::mInstance = nullptr;

TextureHolder::TextureHolder()
{
	assert(mInstance == nullptr);
	mInstance = this;
}

TextureHolder::~TextureHolder()
{
}

Texture & TextureHolder::GetTexture(string const filename)
{
	auto& textures = mInstance->mTextures;
	auto keyValuePair = textures.find(filename);

	if (keyValuePair != textures.end())
	{
		return keyValuePair->second;
	}
	else
	{
		auto& texture = textures[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}
